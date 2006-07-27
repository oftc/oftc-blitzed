/* MySQL-specific functions.
 *  
 * copyright (c) 2001 Blitzed IRC Network.
 *     E-mail: <services@lists.blitzed.org>
 *     
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include <stdlib.h>

#include "services.h"

RCSID("$Id$");

int log_sql;

static FILE *mysql_logfile;

static int safe_mysql_query(MYSQL *dbh, const char *query);

/*************************************************************************/

/* Structure we'll initialise and a pointer that will later point to it */
MYSQL mysql, *mysqlconn;

void smysql_init(void)
{
	unsigned int fields, rows, err;
	MYSQL_RES *result;

	mysql_init(&mysql);
	open_mysql_log();
	log_sql = 1;
	smysql_log("MySQL client version %s", mysql_get_client_info());

	if (! (mysqlconn = mysql_real_connect(&mysql, mysqlHost, mysqlUser,
	    mysqlPass, mysqlDB, mysqlPort, mysqlSocket, 0))) {
		/* There was an error connecting to MySQL. */
		fatal("Couldn't connect to MySQL - %s",
		    mysql_error(&mysql));
	}

	if (log_sql) {
		smysql_log("MySQL connected to %s using protocol v%u",
		    mysql_get_host_info(mysqlconn),
		    mysql_get_proto_info(mysqlconn));
		smysql_log("MySQL server version %s",
		    mysql_get_server_info(mysqlconn));
	}

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "USE %s", mysqlDB);
	mysql_free_result(result);

	/* Check our user-defined functions are present. */
	err = 0;
	result = mysql_simple_query(mysqlconn, &err, "SELECT SHA1('moo')");
	mysql_free_result(result);

	if (err) {
		fatal("User-defined MySQL function SHA1() does not seem "
		    "to be present and working");
	}

	result = mysql_simple_query(mysqlconn, &err,
	    "SELECT IRC_MATCH('*@*', 'foo@bar')");
	mysql_free_result(result);

	if (err) {
		fatal("User-defined MySQL function IRC_MATCH() does not seem "
		    "to be present and working");
	}
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "TRUNCATE TABLE session");
	mysql_free_result(result);
}

/* error, so cry about it and quit for now */
void handle_mysql_error(MYSQL *mysql, const char *msg)
{
	static char buf[500];

	if (mysql_error(mysql)) {
		quitting = 1;
		snprintf(buf, sizeof(buf), "MySQL %s error: %s", msg,
			 mysql_error(mysql));
		fatal_error(buf);
	}
}

/* perform a mysql_query and then grab the result, handling errors */
MYSQL_RES *smysql_bulk_query(MYSQL *mysql, unsigned int *num_fields,
    unsigned int *num_rows, const char *fmt, ...)
{
	char buf[4096];
	va_list args;
	MYSQL_RES *result;

	*num_fields = 0;
	*num_rows = 0;
	
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	
	if (log_sql)
		smysql_log("Query: %s", buf);
	
	if (debug > 2) {
		snoop(s_OperServ, "[MySQL] %s", buf);
	}

	if (safe_mysql_query(mysql, buf))
		handle_mysql_error(mysql, "query");

	if ((result = mysql_store_result(mysql))) {
		*num_fields = mysql_num_fields(result);
		*num_rows = mysql_num_rows(result);
	} else {
		/*
		 * It returned nothing - should it have?  Note: "no rows"
		 * is not "nothing".
		 */
		if (mysql_field_count(mysql) == 0) {
			/*
			 * It wasn't a SELECT, find out how many rows it
			 * affected.
			 */
			*num_rows = mysql_affected_rows(mysql);
		} else {
			/* Yes, so an error. */
			handle_mysql_error(mysql, "store_result");
		}
	}

	return(result);
}

/* execute a query, with caller handling errors and result */
MYSQL_RES *mysql_simple_query(MYSQL *mysql, unsigned int *err,
		              const char *fmt, ...)
{
	va_list args;
	char buf[4096];
	MYSQL_RES *result;

	*err = 0;
	
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);

	if (log_sql)
		smysql_log("Query: %s", buf);

	if (debug > 2) {
		snoop(s_OperServ, "[MySQL] %s", buf);
	}

	if (safe_mysql_query(mysql, buf)) {
		*err = mysql_errno(mysql);
		result = NULL;
	} else {
		result = mysql_store_result(mysql);
	}
	
	return(result);
}

/* perform a query but don't grab the result, caller needs to later use
 * mysql_fetch_row() for every row */
MYSQL_RES *smysql_query(MYSQL *mysql, const char *fmt, ...)
{
	va_list args;
	char buf[4096];
	MYSQL_RES *result;

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);

	if (log_sql)
		smysql_log("Query: %s", buf);

	if (debug > 2) {
		snoop(s_OperServ, "[MySQL] %s", buf);
	}

	if (safe_mysql_query(mysql, buf))
		handle_mysql_error(mysql, "query");

	if (! (result = mysql_use_result(mysql)))
		handle_mysql_error(mysql, "use_result");

	return(result);
}

/* fetch a single row from mysql */
MYSQL_ROW smysql_fetch_row(MYSQL *mysql, MYSQL_RES *result)
{
	MYSQL_ROW row;

	if ((! (row = mysql_fetch_row(result))) && mysql_errno(mysql))
		handle_mysql_error(mysql, "fetch_row");

	return(row);
}

/* return the number of rows and size of the table in bytes */
void get_table_stats(MYSQL *mysql, const char *table, unsigned int *nrec,
		     unsigned int *bytes)
{
	MYSQL_RES *result;
	unsigned int num_fields, num_rows;
	char *esc_db, *esc_table;

	esc_db = smysql_escape_string(mysql, mysqlDB);
	esc_table = smysql_escape_string(mysql, table);
	
	result = smysql_bulk_query(mysql, &num_fields, &num_rows,
				   "show table status from %s like '%s'",
				   esc_db, esc_table);

	free(esc_table);
	free(esc_db);

	if (num_fields < 8 || num_rows < 1) {
		if (log_sql) {
			smysql_log("ERROR: get_table_stats(%s) got "
			    "incorrect data (%u row%s of %u column%s)",
			    table, num_rows, num_rows == 1 ? "" : "s",
			    num_fields, num_fields == 1 ? "" : "s");
		}
		*nrec = 0;
		*bytes = 0;
	} else {
		MYSQL_ROW row;

		row = smysql_fetch_row(mysql, result);
		*nrec = atoi(row[3]);
		*bytes = atoi(row[5]) + atoi(row[7]);
	}

        mysql_free_result(result);
}

/* return just the number of rows in a table */
unsigned int get_table_rows(MYSQL *mysql, const char *table)
{
	unsigned int nrec, bytes;

	get_table_stats(mysql, table, &nrec, &bytes);

	return(nrec);
}

/* return the number of rows in the given table which have a single column
 * that matches the given string */
unsigned int count_matching_rows(MYSQL *mysql, const char *table,
				 const char *column, const char *string)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	char *esc_table, *esc_column, *esc_string;

	esc_table = smysql_escape_string(mysql, table);
	esc_column = smysql_escape_string(mysql, column);
	esc_string = smysql_escape_string(mysql, string);

	result = smysql_bulk_query(mysql, &fields, &rows,
				   "SELECT 1 FROM %s where %s='%s'",
				   esc_table, esc_column, esc_string);
	mysql_free_result(result);
	free(esc_string);
	free(esc_column);
	free(esc_table);

	return(rows);
}

/* the following two locking functions just to save a little typing; if
 * more than one table needs to be locked we'll have to do it ourselves
 * because all locks have to be made in one command. */

/* lock a table for READ (services and any other users may only read the
 * table) */
void read_lock_table(MYSQL *mysql, const char *table)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	char *esc_table = smysql_escape_string(mysql, table);

	result = smysql_bulk_query(mysql, &fields, &rows,
				   "LOCK TABLES %s READ", esc_table);
	mysql_free_result(result);

	free(esc_table);
	
	if (log_sql)
		smysql_log("LOCK: READ lock on %s", table);
}

/* lock a table for WRITE (services may read or write the table, no other
 * users can access the table( */
void write_lock_table(MYSQL *mysql, const char *table)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	char *esc_table = smysql_escape_string(mysql, table);

	result = smysql_bulk_query(mysql, &fields, &rows,
				   "LOCK TABLES %s WRITE", esc_table);
	mysql_free_result(result);

	free(esc_table);
	
	if (log_sql)
		smysql_log("LOCK: WRITE lock on %s", table);
}

/* remove locks */
void unlock(MYSQL *mysql)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysql, &fields, &rows, "UNLOCK TABLES");
	mysql_free_result(result);
}

/* cleanup and close MySQL connection */
void smysql_cleanup(MYSQL *mysql)
{
	if (log_sql)
		smysql_log("Closing connection to server");
	mysql_close(mysql);
	close_mysql_log();
}

/*
 * Build a SELECT query from a numlist in the style of misc.c:process_numlist()
 */

/*
 * Check that the sprintf managed to complete, we can tell because i (returned
 * from sprintf) will be between 0 and *size.  If i > *size then that indicates
 * that sprintf wanted to store more than it was able, and the buffer must be
 * increased in size.  Older glibc used to return -1, so we can't tell how much
 * it wanted.  In this case we just double the size each time.
 */
int check_chunk(int i, size_t *size, char *chunk)
{
	snoop(s_OperServ, "check_chunk: i=%d, size=%u, chunk='%s'", i, *size, chunk);
	if (i > -1 && (unsigned int)i < *size)
		return(1);

	if (i > -1)	/* glibc 2.1+ */
		*size = i + 1;
	else		/* glibc 2.0 */
		*size *= 2;
	chunk = realloc(chunk, *size);
	return(0);
}	

char *mysql_build_query(MYSQL *mysql, const char *numstr, const char *base,
			const char *column, unsigned int max)
{
	/* Size of our main query buffer. */
	size_t size;

	/* Length of each chunk of the query. */
	size_t chunk_len;

	/* Start and end of each range. */
	unsigned int n1, n2;

	/* Toggle, is this the first clause? */
	int first;

	/* Start of the query. */
	char *query;

	/* Column to compare against. */
	char *esc_column;

	/* Each chunk of the query will be kept here. */
	char chunk[BUFSIZE];

	first = 1;
	query = sstrdup(base);
	size = strlen(query) + 1;
	esc_column = smysql_escape_string(mysql, column);

	/* Whilst there is any string left.. */
	while (*numstr) {
		/*
		 * n1 and n2 will either both be the same first integer or else
		 * they'll be zero.  numstr will advance to first non-digit
		 * character.
		 */
		n1 = n2 = strtoul(numstr, (char **)&numstr, 10);

		/* Skip forward to next character out of list below. */
		numstr += strcspn(numstr, "0123456789,-");

		/*
		 * If it's a dash then this is a range and we should expect
		 * another integer now.
		 */
		if (*numstr == '-') {
			/*
			 * Advance past the dash and skip to the next integer
			 * or comma.
			 */
			numstr++;
			numstr += strcspn(numstr, "0123456789,");

			/*
			 * If there's a digit then get it and put it in n2 to
			 * form the end of our range.
			 */
			if (isdigit(*numstr)) {
				n2 = strtoul(numstr, (char **)&numstr, 10);
				numstr += strcspn(numstr, "0123456789,-");
			}
		}

		/* Sanity check. */
		if (n1 > max)
			n1 = max;

		if (n2 > max)
			n2 = max;

		if (n1 == n2) {
			/*
			 * If n1 == n2 then we did not get a range, just a
			 * single value.
			 */

			memset(chunk, 0, sizeof(chunk));
			snprintf(chunk, sizeof(chunk), "%s(%s = %u)",
			    first ? "" : " || ", esc_column, n1);

			/* No longer the first clause of the query. */
			first = 0;

			/*
			 * Increase the size of our main query buffer to
			 * account for this chunk.
			 */
			chunk_len = strlen(chunk);
			query = realloc(query, chunk_len + size);

			/* And then strncat it in. */
			strncat(query, chunk, chunk_len);

			/* Calculate new size for the query buffer. */
			size = strlen(query) + 1;

		} else if (n1 < n2) {
			/*
			 * If n1 < n2 then we got a range, so we need to put a
			 * "BETWEEN X AND Y" in the query.
			 */

			memset(chunk, 0, sizeof(chunk));
			snprintf(chunk, sizeof(chunk),
			    "%s(%s BETWEEN %u AND %u)", first ? "" : " || ",
			    esc_column, n1, n2);

			first = 0;

			chunk_len = strlen(chunk);
			query = realloc(query, chunk_len + size);
			strncat(query, chunk, chunk_len);
			size = strlen(query) + 1;
		}

		numstr += strcspn(numstr, ",");

		if (*numstr)
			 numstr++;
	}
	
	free(esc_column);
	
	return(query);
}

/* build a query for only the rows that match a mask */
char *mysql_build_query_match_mask(MYSQL *mysql, const char *mask,
				   const char *base, const char *table,
				   const char *id_col, const char *mask_col)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int size, first = 1;
	char *query = sstrdup(base);
	char *esc_id_col, *esc_mask_col, *esc_table;

	esc_id_col = smysql_escape_string(mysql, id_col);
	esc_mask_col = smysql_escape_string(mysql, mask_col);
	esc_table = smysql_escape_string(mysql, table);
	
	size = strlen(query) + 1;

	result = smysql_query(mysql, "SELECT %s, %s FROM %s ORDER BY %s",
			      esc_id_col, esc_mask_col, esc_table,
			      esc_id_col);
	while ((row = smysql_fetch_row(mysql, result))) {
		if (match_wild(mask, row[1])) {
			char *chunk = malloc(sizeof(char));
			unsigned int chunk_size = 1;
			*chunk = 0;

			while (1) {
				int i = snprintf(chunk, chunk_size,
						 "%s(%s = %s)",
						 first ? "" : " || ",
						 esc_id_col, row[0]);
				if (check_chunk(i, &chunk_size, chunk))
					break;
			}
			first = 0;

			query = realloc(query, strlen(chunk) + 1 + size);
			strncat(query, chunk, chunk_size);
			free(chunk);
			size = strlen(query) + 1;
		}
	}
	mysql_free_result(result);

	free(esc_table);
	free(esc_mask_col);
	free(esc_id_col);
	
	return(query);
}

/* escape a string for mysql usage, returns a pointer to allocated storage
 * so must be later free'd.  MySQL manual states that a string at least
 * twice the length of the original is needed since in worst case every
 * character may need to be escaped */
char *smysql_escape_string(MYSQL *mysql, const char *string)
{
	unsigned int len = strlen(string);
	char *s = malloc((1 + (len * 2)) * sizeof(char));

	mysql_real_escape_string(mysql, s, string, len);
	return(s);
}

/*
 * Wrapper to mysql_query() to handle some common errors which we can deal
 * with.
 */
static int safe_mysql_query(MYSQL *dbh, const char *query)
{
	unsigned int fields, rows;
	int rc, i;
	MYSQL *newconn;
	MYSQL_RES *result;

	rc = mysql_query(dbh, query);

	if (rc) {
		switch(mysql_errno(dbh)) {
		case CR_SERVER_GONE_ERROR:
		case CR_SERVER_LOST:
			wallops(s_OperServ, "Lost connection to MySQL "
			    "server!  I'll keep trying to reconnect for "
			    "10 secs.");
			for (i = 0; i < 10; i++) {
				wallops(s_OperServ,
				    "Reconnect attempt #%d...", i + 1);
				if ((newconn = mysql_real_connect(&mysql,
				    mysqlHost, mysqlUser, mysqlPass,
				    mysqlDB, mysqlPort, mysqlSocket, 0))) {
					wallops(s_OperServ,
					    "Reconnected to MySQL!");
					dbh = mysqlconn = newconn;
					if (log_sql) {
						smysql_log("MySQL "
						"connected to %s using "
						"protocol v%u",
						mysql_get_host_info(mysqlconn),
						mysql_get_proto_info(mysqlconn));
						smysql_log("MySQL server "
						    "version %s",
						    mysql_get_server_info(mysqlconn));
					}
					snoop(s_OperServ,
					    "MySQL connected to %s using "
					    "protocol v%u",
					    mysql_get_host_info(mysqlconn),
					    mysql_get_proto_info(mysqlconn));
					snoop(s_OperServ,
					    "MySQL server version %s",
					    mysql_get_server_info(mysqlconn));

					result = smysql_bulk_query(mysqlconn,
					    &fields, &rows, "USE %s",
					    mysqlDB);
					mysql_free_result(result);

					/* Execute query again. */
					rc = mysql_query(dbh, query);
					return(rc);
				}

				sleep(1);
			}

			/* If we get here, we could not connect. */
			fatal("Couldn't connect to MySQL - %s",
			    mysql_error(&mysql));

			/* Never reached. */
			break;

		default:
			/* Unhandled error. */
			return(rc);
		}
	}

	return(0);
}

/*
 * Open the log file.  Return -1 if the log file could not be opened, else
 * return 0.
 */
int open_mysql_log(void)
{
	if (mysql_logfile)
		return(0);

	if (chdir(LOGDIR) < 0) {
		fprintf(stderr, "chdir(%s): %s\n", LOGDIR,
		    strerror(errno));
		return(-1);
	}

	mysql_logfile = fopen("mysql.log", "a");

	if (mysql_logfile)
		setbuf(mysql_logfile, NULL);

	return(mysql_logfile != NULL ? 0 : -1);
}

/*
 * Close the log file.
 */
void close_mysql_log(void)
{
	if (!mysql_logfile)
		return;

	fclose(mysql_logfile);
	mysql_logfile = NULL;
}

/*
 * Log stuff to the log file with a datestamp.  Note that errno is preserved
 * by this routine.
 */
void smysql_log(const char *fmt, ...)
{
	char buf[256];
#if HAVE_GETTIMEOFDAY
	struct timeval tv;
#endif
	time_t t;
	int errno_save;
	va_list args;
	struct tm *tm;
	char *s;

	errno_save = errno;

	memset(buf, 0, sizeof(buf));

	va_start(args, fmt);
	time(&t);
	tm = localtime(&t);
#if HAVE_GETTIMEOFDAY
	if (debug) {
		gettimeofday(&tv, NULL);
		strftime(buf, sizeof(buf) - 1, "[%b %d %H:%M:%S", tm);
		s = buf + strlen(buf);
		s += snprintf(s, sizeof(buf) - (s - buf), ".%06ld",
		    tv.tv_usec);
		strftime(s, sizeof(buf) - (s - buf) - 1, " %Y] ", tm);
	} else {
#endif
		strftime(buf, sizeof(buf) - 1, "[%b %d %H:%M:%S %Y] ", tm);
#if HAVE_GETTIMEOFDAY
	}
#endif
	if (mysql_logfile) {
		fputs(buf, mysql_logfile);
		vfprintf(mysql_logfile, fmt, args);
		fputc('\n', mysql_logfile);
	}

	errno = errno_save;
}
