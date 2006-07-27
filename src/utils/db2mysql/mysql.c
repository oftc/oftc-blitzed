
/*
 * MySQL-related functions for db2mysql.
 * 
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 * 
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include <time.h>
#include <stdio.h>
#include <sys/param.h>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../sysconf.h"
#include "db2mysql.h"
#include "extern.h"

RCSID("$Id$");

extern char *mysql_user, *mysql_host, *mysql_db, *mysql_pass;

MYSQL mysql, *mysqlconn;

static void handle_mysql_error(MYSQL *mysql, const char *msg,
    const char *buf);

/*************************************************************************/

void smysql_init(void)
{
	mysql_init(&mysql);
        debug("MySQL client version %s", mysql_get_client_info());

	mysqlconn = mysql_real_connect(&mysql, mysql_host, mysql_user,
	    mysql_pass, mysql_db, mysql_port, mysql_socket, 0);

	if (!mysqlconn) {
                /* There was an error connecting to mysql. */
                fprintf(stderr, "Couldn't connect to MySQL - %s",
		    mysql_error(&mysql));
		exit(EXIT_FAILURE);
        }

        debug("MySQL connected to %s using protocol v%u",
	    mysql_get_host_info(mysqlconn), mysql_get_proto_info(mysqlconn));
        debug("MySQL server version %s", mysql_get_server_info(mysqlconn));
}

char *smysql_escape_string(MYSQL *mysql, const char *string)
{
	unsigned int len;
	char *s;

	len = strlen(string);
	s = malloc(1 + (len * 2));
	mysql_real_escape_string(mysql, s, string, len);
	return(s);
}

MYSQL_RES *smysql_query(MYSQL *mysql, unsigned int *num_fields,
    unsigned int *num_rows, const char *fmt, ...)
{
	char buf[4096];
	va_list args;
	MYSQL_RES *result;

	*num_fields = 0;
	*num_rows = 0;

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);

	if (mysql_query(mysql, buf))
		handle_mysql_error(mysql, "query", buf);

	if ((result = mysql_store_result(mysql))) {
		*num_fields = mysql_num_fields(result);
		*num_rows = mysql_num_rows(result);
	} else {
		if (mysql_field_count(mysql) == 0) {
			*num_rows = mysql_affected_rows(mysql);
		} else {
			handle_mysql_error(mysql, "store_result", buf);
		}
	}

	return(result);
}

static void handle_mysql_error(MYSQL *mysql, const char *msg,
    const char *buf)
{
	if (mysql_error(mysql)) {
		fprintf(stderr, "MySQL %s error: %s\n", msg,
		    mysql_error(mysql));
		if (buf)
			fprintf(stderr, "Query: %s\n", buf);
		exit(EXIT_FAILURE);
	}
}

MYSQL_ROW smysql_fetch_row(MYSQL *mysql, MYSQL_RES *result)
{
	MYSQL_ROW row;

	if ((! (row = mysql_fetch_row(result))) && mysql_errno(mysql))
		handle_mysql_error(mysql, "fetch_row", NULL);

	return(row);
}
