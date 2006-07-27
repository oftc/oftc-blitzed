
/*
 * Routies related to the auth table and AUTH system, which provides for
 * email authorisation of certain important services commands.
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 *     E-mail: services@lists.blitzed.org
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include <stdlib.h>
#include <time.h>

#include "services.h"

#define SECS_24_HRS 86400

RCSID("$Id$");

/*
 * expire_auth: arrange for rows in the auth table to be correctly expired.
 * Currently this means any rows older than 24 hours.
 */
void expire_auth(time_t now)
{
	char tbuf[100];
	MYSQL_ROW row;
	MYSQL_RES *result;
	unsigned int fields, rows;
	time_t created;
	struct tm *tp;

	write_lock_table(mysqlconn, "auth");
	
	if (debug) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT name, command, create_time FROM auth "
		    "WHERE (create_time + %u) < %d", SECS_24_HRS, now);
		while ((row = smysql_fetch_row(mysqlconn, result))) {
			created = atoi(row[2]);
			tp = gmtime(&created);
			strftime(tbuf, sizeof(tbuf), "%d/%b/%Y %H:%M:%S", tp);
			log("auth_expire: Expiring command %s by %s "
			    "created %s", row[1], row[0], tbuf);
			snoop(s_OperServ,
			    "[AUTH] Expiring command %s by %s created %s",
			    row[1], row[0], tbuf);
		}
		mysql_free_result(result);
	}
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM auth WHERE (create_time + %u) < %d",
	    SECS_24_HRS, now);
	mysql_free_result(result);
	
	unlock(mysqlconn);
	return;
}
