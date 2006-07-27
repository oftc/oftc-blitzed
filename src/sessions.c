
/* Session Limiting functions.
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 *     E-mail: services@lists.blitzed.org
 * Based on ircservices-4.4.8:
 * copyright (c) 1996-1999 Andrew Church.
 *     E-mail: <achurch@dragonfire.net>
 * copyright (c) 1999-2000 Andrew Kempe.
 *     E-mail: <theshadow@shadowfire.org>
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include "services.h"
#include "pseudo.h"

RCSID("$Id$");

/*************************************************************************/

/* SESSION LIMITING
 *
 * The basic idea of session limiting is to prevent one host from having more 
 * than a specified number of sessions (client connections/clones) on the 
 * network at any one time. To do this we have a list of sessions and 
 * exceptions. Each session structure records information about a single host, 
 * including how many clients (sessions) that host has on the network. When a 
 * host reaches it's session limit, no more clients from that host will be 
 * allowed to connect.
 *
 * When a client connects to the network, we check to see if their host has 
 * reached the default session limit per host, and thus whether it is allowed 
 * any more. If it has reached the limit, we kill the connecting client; all 
 * the other clients are left alone. Otherwise we simply increment the counter 
 * within the session structure. When a client disconnects, we decrement the 
 * counter. When the counter reaches 0, we free the session.
 *
 * Exceptions allow one to specify custom session limits for a specific host 
 * or a range thereof. The first exception that the host matches is the one 
 * used.
 *
 * "Session Limiting" is likely to slow down services when there are frequent 
 * client connects and disconnects. The size of the exception list can also 
 * play a large role in this performance decrease. It is therefore recommened 
 * that you keep the number of exceptions to a minimum. A very simple hashing 
 * method is currently used to store the list of sessions. I'm sure there is 
 * room for improvement and optimisation of this, along with the storage of 
 * exceptions. Comments and suggestions are more than welcome!
 *
 * -TheShadow (02 April 1999)
 */

/*************************************************************************/

typedef struct exception_ Exception;
struct exception_ {
	unsigned int id;	/* ID for MySQL */
	char *mask;		/* Hosts to which this exception applies */
	int limit;		/* Session limit for exception */
	char who[NICKMAX];	/* Nick of person who added the exception */
	char *reason;		/* Reason for exception's addition */
	time_t time;		/* When this exception was added */
	time_t expires;		/* Time when it expires. 0 == no expiry */
};

/*************************************************************************/

static int exception_add(const char *mask, const int limit,
			 const char *reason, const char *who,
			 const time_t expires);
static unsigned int mysql_insert_exception(MYSQL *mysql, Exception *ex);

/*************************************************************************/

/****************************** Statistics *******************************/

/*************************************************************************/

void get_session_stats(long *nrec, long *memuse)
{
	get_table_stats(mysqlconn, "session", (unsigned int *)nrec,
			(unsigned int *)memuse);
	if (*memuse)
		*memuse /= 1024;
}

void get_exception_stats(long *nrec, long *memuse)
{
	get_table_stats(mysqlconn, "exception", (unsigned int *)nrec,
			(unsigned int *)memuse);
	if (*memuse)
		*memuse /= 1024;
}

/*************************************************************************/

/************************* Session List Display **************************/

/*************************************************************************/

/* Syntax: SESSION LIST threshold
 *	Lists all sessions with atleast threshold clients.
 *	The threshold value must be greater than 1. This is to prevent 
 * 	accidental listing of the large number of single client sessions.
 *
 * Syntax: SESSION VIEW host
 *	Displays detailed session information about the supplied host.
 */

void do_session(User *u)
{
	MYSQL_ROW row;
	int mincount;
	unsigned int fields, rows;
	char *cmd, *param1;
	MYSQL_RES *result;
			

	cmd = strtok(NULL, " ");
	param1 = strtok(NULL, " ");

	if (!LimitSessions) {
		notice_lang(s_OperServ, u, OPER_SESSION_DISABLED);
		return;
	}

	if (!cmd)
		cmd = "";

	if (stricmp(cmd, "LIST") == 0) {
		if (!param1) {
			syntax_error(s_OperServ, u, "SESSION",
			    OPER_SESSION_LIST_SYNTAX);

		} else if ((mincount = atoi(param1)) <= 1) {
			notice_lang(s_OperServ, u,
			    OPER_SESSION_INVALID_THRESHOLD);

		} else {
			notice_lang(s_OperServ, u, OPER_SESSION_LIST_HEADER,
			    mincount);
			notice_lang(s_OperServ, u,
			    OPER_SESSION_LIST_COLHEAD);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT session.host, session.count, "
			    "IFNULL(exception.ex_limit, %d) FROM session "
			    "LEFT JOIN exception ON "
			    "IRC_MATCH(exception.mask, session.host) "
			    "WHERE session.count >= %d "
			    "ORDER BY session.count DESC", DefSessionLimit,
			    mincount);

			while ((row = smysql_fetch_row(mysqlconn, result))) {
				notice_lang(s_OperServ, u,
				    OPER_SESSION_LIST_FORMAT, atoi(row[1]),
				    atoi(row[2]), row[0]);
			}

			mysql_free_result(result);
		}
	} else if (stricmp(cmd, "VIEW") == 0) {
		if (!param1) {
			syntax_error(s_OperServ, u, "SESSION",
			    OPER_SESSION_VIEW_SYNTAX);

		} else {
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT session.host, session.count, "
			    "IFNULL(exception.ex_limit, %d) FROM session "
			    "LEFT JOIN exception ON "
			    "IRC_MATCH(exception.mask, session.host) "
			    "WHERE session.host='%s'", DefSessionLimit,
			    param1);

			while ((row = smysql_fetch_row(mysqlconn, result))) {
				notice_lang(s_OperServ, u,
				    OPER_SESSION_VIEW_FORMAT, row[0],
				    atoi(row[1]), atoi(row[2]));
			}
			mysql_free_result(result);

			if (!rows) {
				notice_lang(s_OperServ, u,
				    OPER_SESSION_NOT_FOUND, param1);
			}
		}
	} else {
		syntax_error(s_OperServ, u, "SESSION", OPER_SESSION_SYNTAX);
	}
}

/*************************************************************************/

/********************* Internal Session Functions ************************/

/*************************************************************************/

/* Attempt to add a host to the session list. If the addition of the new host
 * causes the the session limit to be exceeded, kill the connecting user.
 * Returns 1 if the host was added or 0 if the user was killed.
 */

/* try to INSERT first, if that fails due to the row already existing, we
 * can use UPDATE */

int add_session(const char *nick, const char *host)
{
	char mask[BUFSIZE];
	MYSQL_ROW row;
	time_t expiry, now;
	unsigned int err, fields, rows, count, sessionlimit, killcount;
	unsigned int matches;
	int killed;
	MYSQL_RES *result;

	count = sessionlimit = killcount = matches = killed = 0;

	result = mysql_simple_query(mysqlconn, &err, "INSERT INTO session "
	    "(host, count, killcount) VALUES ('%s', 1, 0)", host);

	mysql_free_result(result);

	switch (err) {
	case 0:
		return(1);
	case ER_DUP_ENTRY:
		/* This session already exists! */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT session.host, session.count, "
		    "session.killcount, IFNULL(exception.ex_limit, %d) "
		    "FROM session LEFT JOIN exception ON "
		    "IRC_MATCH(exception.mask, session.host) "
		    "WHERE session.host='%s'", DefSessionLimit, host);

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			count = atoi(row[1]);
			killcount = atoi(row[2]);
			sessionlimit = atoi(row[3]);
		}

		mysql_free_result(result);

		if (sessionlimit != 0 && count >= sessionlimit) {
			if (SessionLimitExceeded) {
				notice(s_OperServ, nick,
				    SessionLimitExceeded, host);
			}
			
			if (SessionLimitDetailsLoc) {
				notice(s_OperServ, nick,
				    SessionLimitDetailsLoc);
			}

			killcount++;

			if (SessionKillClues &&
			    killcount > SessionKillClues) {
				/*
				 * They've already been killed several times.
				 * Akill them now.
				 */
				now = time(NULL);

				snprintf(mask, sizeof(mask), "*@%s", host);
				killcount = 0;

				expiry = SessionAkillExpiry ?
				    SessionAkillExpiry : AutokillExpiry;
				expiry += now;

				if (WallOSAkill)
					matches = count_mask_matches(mask);
					
				add_akill(mask, "Session limit exceeded",
				    s_OperServ, expiry);

				if (WallOSAkill) {
					wallops(s_OperServ,
					    "\2%s\2 added an AKILL for "
					    "\2%s\2 (%s) [Session limit "
					    "exceeded] - %u clients killed",
					    s_OperServ, mask,
					    disect_time(expiry - now),
					    matches);
				}
			} else {
				send_cmd(s_OperServ, "KILL %s :%s (Session "
				    "limit exceeded)", nick, s_OperServ);
			}
			killed = 1;
		} else {
			count++;
		}

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE session SET count=%u, killcount=%u "
		    "WHERE host='%s'", count, killcount, host);
		mysql_free_result(result);

		return(killed ? 0 : 1);

	default:
		handle_mysql_error(mysqlconn, "insert_session");
		return(0);	/* Never actually reached. */
	}
}

void del_session(const char *host)
{
	MYSQL_RES *result;
	unsigned int err;
	char *esc_host = smysql_escape_string(mysqlconn, host);
	
	if (debug >= 2)
		log("debug: del_session() called");

	result = mysql_simple_query(mysqlconn, &err, "UPDATE session SET "
				    "count=count-1 WHERE host='%s'", esc_host);

	switch (err) {
		case 0:
			mysql_free_result(result);
			break;
		default:
			wallops(s_OperServ, "WARNING: Tried to delete "
				"non-existent session: \2%s\2", esc_host);
			log("session: Tried to delete non-existent session: %s",
			    esc_host);
			break;
	}

	free(esc_host);
}


/*************************************************************************/

/********************** Internal Exception Functions *********************/

/*************************************************************************/

void expire_exceptions(void)
{
	time_t now = time(NULL);
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows;

	if (opt_noexpire)
		return;

	/* need to do a bunch of stuff so lock the table till we're
	 * finished */
	write_lock_table(mysqlconn, "exception");

	if (WallExceptionExpire) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "SELECT exception_id, mask, "
					   "reason FROM exception WHERE "
					   "expires != 0 && expires < %d",
					   now);

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			wallops(s_OperServ, "Session limit exception for "
				"%s (%s) has expired.", row[0], row[1]);
		}
		mysql_free_result(result);
	}

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "DELETE FROM exception WHERE "
				   "expires != 0 && expires < %d", now);
	mysql_free_result(result);
	unlock(mysqlconn);
}


/*************************************************************************/

/************************ Exception Manipulation *************************/

/*************************************************************************/

static int exception_add(const char *mask, const int limit,
			 const char *reason, const char *who,
			 const time_t expires)
{
	Exception ex;
	unsigned int err;
	
	ex.mask = sstrdup(mask);
	ex.limit = limit;
	ex.reason = sstrdup(reason);
	ex.time = time(NULL);
	strscpy(ex.who, who, NICKMAX);
	ex.expires = expires;

	err = mysql_insert_exception(mysqlconn, &ex);

	free(ex.reason);
	free(ex.mask);

	switch (err) {
		case 0:
			return(1);
		case ER_DUP_ENTRY:
			/* They tried to add a dupe mask but we didn't
			 * do it and probably don't care */
			return(0);
		default:
			handle_mysql_error(mysqlconn, "insert_exception");
			return(0);
	}
}

/*************************************************************************/

/* Syntax: EXCEPTION ADD [+expiry] mask limit reason
 *	Adds mask to the exception list with limit as the maximum session
 *	limit and +expiry as an optional expiry time.
 *
 * Syntax: EXCEPTION DEL mask
 *	Deletes the first exception that matches mask exactly.
 *
 * Syntax: EXCEPTION LIST [mask]
 *	Lists all exceptions or those matching mask.
 *
 * Syntax: EXCEPTION VIEW [mask]
 *	Displays detailed information about each exception or those matching
 *	mask.
 *
 * Syntax: EXCEPTION MOVE num position
 *	Moves the exception at position num to position.
 */

void do_exception(User * u)
{
	char timebuf[32], expirebuf[256], buf[128];
	MYSQL_ROW row;
	struct tm tm;
	time_t t, ex_time, ex_expires;
	unsigned int fields, rows, ex_id, ex_limit;
	int limit, expires, sent_header;
	char *cmd, *mask, *reason, *expiry, *limitstr, *query, *esc_mask;
	const char *base;
	MYSQL_RES *result;

	cmd = strtok(NULL, " ");

	if (!LimitSessions) {
		notice_lang(s_OperServ, u, OPER_EXCEPTION_DISABLED);
		return;
	}

	if (!cmd)
		cmd = "";

	if (stricmp(cmd, "ADD") == 0) {
		t = time(NULL);

		if (num_exceptions() >= 32767) {
			notice_lang(s_OperServ, u, OPER_EXCEPTION_TOO_MANY);
			return;
		}

		mask = strtok(NULL, " ");
		if (mask && *mask == '+') {
			expiry = mask;
			mask = strtok(NULL, " ");
		} else {
			expiry = NULL;
		}
		limitstr = strtok(NULL, " ");
		reason = strtok(NULL, "");

		if (!reason) {
			syntax_error(s_OperServ, u, "EXCEPTION",
			    OPER_EXCEPTION_ADD_SYNTAX);
			return;
		}

		expires = expiry ? dotime(expiry) : ExceptionExpiry;

		if (expires < 0) {
			notice_lang(s_OperServ, u, BAD_EXPIRY_TIME);
			return;
		} else if (expires > 0) {
			expires += t;
		}

		limit = (limitstr && isdigit(*limitstr)) ?
		    atoi(limitstr) : -1;

		if (limit < 0 || (unsigned int)limit > MaxSessionLimit) {
			notice_lang(s_OperServ, u,
			    OPER_EXCEPTION_INVALID_LIMIT, MaxSessionLimit);
			return;

		} else {
			if (strchr(mask, '!') || strchr(mask, '@')) {
				notice_lang(s_OperServ, u,
				    OPER_EXCEPTION_INVALID_HOSTMASK);
				return;
			} else {
				strlower(mask);
			}

			if (exception_add(mask, limit, reason, u->nick,
			    expires)) {
				notice_lang(s_OperServ, u,
				    OPER_EXCEPTION_ADDED, mask, limit);

				if (WallOSException) {
					if (expires == 0) {
						snprintf(buf, sizeof(buf),
						    getstring(u->nick_id,
						    OPER_AKILL_NO_EXPIRE));
					} else {
						expires_in_lang(buf,
						    sizeof(buf), u,
						    expires - t + 59);
					}

					wallops(s_OperServ,
					    "%s added a session limit "
					    "exception of \2%d\2 for "
					    "\2%s\2 (%s)", u->nick, limit,
					    mask, buf);
				}
			} else {
				log("session: exception already present");
				snoop(s_OperServ,
				    "[SESSION] exception already present");
				notice_lang(s_OperServ, u,
				    OPER_EXCEPTION_ALREADY_PRESENT, mask,
				    limit);
			}
		}
	} else if (stricmp(cmd, "DEL") == 0) {
		base = "DELETE FROM exception WHERE (";

		mask = strtok(NULL, " ");

		if (!mask) {
			syntax_error(s_OperServ, u, "EXCEPTION",
			    OPER_EXCEPTION_DEL_SYNTAX);
			return;
		}

		if (isdigit(*mask) &&
		    strspn(mask, "1234567890,-") == strlen(mask)) {
			query = mysql_build_query(mysqlconn, mask, base,
			    "exception_id", MaxSessionLimit);

			mask = NULL;

			if (strlen(query) == strlen(base)) {
				/*
				 * Query failed, executing it would lead to
				 * MySQL error.
				 */
				syntax_error(s_OperServ, u, "EXCEPTION",
				    OPER_EXCEPTION_DEL_SYNTAX);
				free(query);
				return;
			}

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s)", query);

			free(query);

		} else {
			esc_mask = smysql_escape_string(mysqlconn, mask);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "DELETE FROM exception WHERE "
			    "IRC_MATCH(mask, '%s')", esc_mask);

			free(esc_mask);
		} 

		mysql_free_result(result);

		if (rows == 0) {
			notice_lang(s_OperServ, u, OPER_EXCEPTION_NO_MATCH);
		} else if (rows == 1) {
			notice_lang(s_OperServ, u,
			    OPER_EXCEPTION_DELETED_ONE);
		} else {
			notice_lang(s_OperServ, u,
			    OPER_EXCEPTION_DELETED_SEVERAL, rows);
		}
	} else if (stricmp(cmd, "LIST") == 0) {
		sent_header = 0;

		expire_exceptions();
		mask = strtok(NULL, " ");
		expires = -1;	/* Do not match on expiry time */

		if (mask) {
			strlower(mask);

			expiry = strtok(NULL, " ");

			/*
			 * This is a little longwinded for what it acheives -
			 * but we can expand it later to allow for user defined
			 * expiry times.
			 */
			if (expiry && stricmp(expiry, "NOEXPIRE") == 0)
				expires = 0;	/* Exceptions that never expire */
		}

		if (mask && strspn(mask, "1234567890,-") == strlen(mask)) {
			base = "SELECT exception_id, ex_limit, mask, "
			    "expires FROM exception WHERE (";
			
			query = mysql_build_query(mysqlconn, mask, base,
			    "exception_id", MaxSessionLimit);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s)", query);

			free(query);
			mask = NULL;
		} else {
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT exception_id, ex_limit, mask, "
			    "expires FROM exception ORDER BY exception_id");
		}

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			if ((!mask || match_wild(mask, row[2])) &&
			    (expires == -1 || atoi(row[3]) == expires)) {
				if (!sent_header) {
					notice_lang(s_OperServ, u,
					    OPER_EXCEPTION_LIST_HEADER);
					notice_lang(s_OperServ, u,
					    OPER_EXCEPTION_LIST_COLHEAD);
					sent_header = 1;
				}
				notice_lang(s_OperServ, u,
				    OPER_EXCEPTION_LIST_FORMAT,
				    atoi(row[0]), atoi(row[1]), row[2]);
			}
		}
		mysql_free_result(result);
		
		if (!sent_header)
			notice_lang(s_OperServ, u, OPER_EXCEPTION_NO_MATCH);

	} else if (stricmp(cmd, "VIEW") == 0) {
		sent_header = 0;
		expire_exceptions();
		mask = strtok(NULL, " ");
		expires = -1;	/* Do not match on expiry time */

		if (mask) {
			strlower(mask);

			expiry = strtok(NULL, " ");

			/*
			 * This is a little longwinded for what it acheives -
			 * but we can expand it later to allow for user defined
			 * expiry times.
			 */
			if (expiry && stricmp(expiry, "NOEXPIRE") == 0)
				expires = 0;	/* Exceptions that never expire */
		}

		if (mask && strspn(mask, "1234567890,-") == strlen(mask)) {
			base = "SELECT exception_id, mask, ex_limit, who, "
			    "reason, time, expires FROM exception WHERE (";

			query = mysql_build_query(mysqlconn, mask, base,
			    "exception_id", MaxSessionLimit);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s)", query);

			free(query);
			mask = NULL;
		} else {
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT exception_id, mask, ex_limit, "
			    "who, reason, time, expires FROM exception "
			    "ORDER BY exception_id");
		}

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			if ((!mask || match_wild(mask, row[1])) &&
			    (expires == -1 || atoi(row[6]) == expires)) {
				t = time(NULL);
				ex_id = atoi(row[0]);
				ex_limit = atoi(row[2]);
				ex_time = atoi(row[5]);
				ex_expires = atoi(row[6]);
				
				if (!sent_header) {
					notice_lang(s_OperServ, u,
					    OPER_EXCEPTION_LIST_HEADER);
					sent_header = 1;
				}

				tm = *localtime(ex_time ? &ex_time : &t);
				strftime_lang(timebuf, sizeof(timebuf), u,
				    STRFTIME_SHORT_DATE_FORMAT, &tm);
				if (ex_expires == 0) {
					snprintf(expirebuf,
					    sizeof(expirebuf),
					    getstring(u->nick_id,
					    OPER_AKILL_NO_EXPIRE));
				} else if (ex_expires <= t) {
					snprintf(expirebuf,
					    sizeof(expirebuf),
					    getstring(u->nick_id,
					    OPER_AKILL_EXPIRES_SOON));
				} else {
					expires_in_lang(expirebuf,
					    sizeof(expirebuf), u,
					    ex_expires - t + 59);
				}

				notice_lang(s_OperServ, u,
				    OPER_EXCEPTION_VIEW_FORMAT, ex_id,
				    row[1], row[3], timebuf, expirebuf,
				    ex_limit, row[4]);
			}
		}
		mysql_free_result(result);

		if (!sent_header)
			notice_lang(s_OperServ, u, OPER_EXCEPTION_NO_MATCH);

	} else {
		syntax_error(s_OperServ, u, "EXCEPTION",
			     OPER_EXCEPTION_SYNTAX);
	}
}

/*************************************************************************/

/* count the exceptions we already have */
unsigned int num_exceptions(void)
{
	return get_table_rows(mysqlconn, "exception");
}

/* insert a new exception */
static unsigned int mysql_insert_exception(MYSQL *mysql, Exception *ex)
{
	MYSQL_RES *result;
	unsigned int err;
	char *esc_mask, *esc_who, *esc_reason;

	esc_mask= smysql_escape_string(mysql, ex->mask);
	esc_who = smysql_escape_string(mysql, ex->who);
	esc_reason = smysql_escape_string(mysql, ex->reason);

	result = mysql_simple_query(mysql, &err, "INSERT INTO exception "
				    "(exception_id, mask, ex_limit, who, "
				    "reason, time, expires) VALUES "
				    "('NULL', '%s', %u, '%s', '%s', %d, "
				    "%d)", esc_mask, ex->limit, esc_who,
				    esc_reason, ex->time, ex->expires);
	mysql_free_result(result);

	free(esc_reason);
	free(esc_who);
	free(esc_mask);
	
	ex->id = (unsigned int)mysql_insert_id(mysql);
	return(err);
}

