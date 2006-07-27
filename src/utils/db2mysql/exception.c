
/*
 * Session limit exception functions for db2mysql.
 * 
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 * 
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/param.h>
#include <mysql.h>

#include "../sysconf.h"
#include "db2mysql.h"
#include "extern.h"

RCSID("$Id$");

extern MYSQL *mysqlconn;

static void db_add_exception(Exception *ex);

/*************************************************************************/

void load_exception_dbase(void)
{
	int ver;
	unsigned int i, nexceptions;
	int32 tmp32;
	int16 tmp16;
	dbFILE *f;
	Exception *exceptions;
	
	if (!(f = open_db(s_OperServ, ExceptionDBName, "r")))
		return;

	ver = get_file_version(f);

	if (ver != 11) {
		fprintf(stderr, "Unsupported file version %d in %s!\n", ver,
		    ExceptionDBName);
		return;
	}

	debug("Got Exception DB version %d", ver);

	read_int16(&tmp16, f);
	nexceptions = tmp16;

	exceptions = smalloc(sizeof(*exceptions) * nexceptions);

	if (!exceptions) {
		fprintf(stderr, "Error allocating storage for exceptions\n");
		close_db(f);
		return;
	}

	for (i = 0; i < nexceptions; i++) {
		read_string(&exceptions[i].mask, f);
		read_int16(&tmp16, f);
		exceptions[i].limit = tmp16;
		read_buffer(exceptions[i].who, f);
		read_string(&exceptions[i].reason, f);
		read_int32(&tmp32, f);
		exceptions[i].time = tmp32;
		read_int32(&tmp32, f);
		exceptions[i].expires = tmp32;
		
		db_add_exception(&(exceptions[i]));
		info("Added exception '%s - %s'", exceptions[i].mask,
		    exceptions[i].reason);
	}

	close_db(f);
}

static void db_add_exception(Exception *ex)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_mask, *esc_who, *esc_reason;

	esc_mask = smysql_escape_string(mysqlconn, ex->mask);
	esc_who = smysql_escape_string(mysqlconn, ex->who);
	esc_reason = smysql_escape_string(mysqlconn, ex->reason);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO exception (exception_id, mask, ex_limit, who, "
	    "reason, time, expires) VALUES ('NULL', '%s', %u, '%s', '%s', "
	    "%lu, %lu)", esc_mask, ex->limit, esc_who, esc_reason,
	    ex->time, ex->expires);
	mysql_free_result(result);

	free(esc_reason);
	free(esc_who);
	free(esc_mask);

	return;
}
