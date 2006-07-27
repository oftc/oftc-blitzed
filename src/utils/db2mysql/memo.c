
/*
 * Memo functions for db2mysql.
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

/*************************************************************************/

void db_add_memoinfo(const char *nick, unsigned int memomax)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_owner;

	esc_owner = smysql_escape_string(mysqlconn, nick);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO memoinfo (memoinfo_id, owner, max) "
	    "VALUES ('NULL', '%s', %u)", esc_owner, memomax);
	mysql_free_result(result);

	free(esc_owner);

	return;
}

void db_add_memo(const char *owner, Memo *memo)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_owner, *esc_sender, *esc_text;

	esc_owner = smysql_escape_string(mysqlconn, owner);
	esc_sender = smysql_escape_string(mysqlconn, memo->sender);
	esc_text = smysql_escape_string(mysqlconn, memo->text);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO memo (memo_id, owner, idx, flags, time, sender, "
	    "text) VALUES ('NULL', '%s', %u, %d, %lu, '%s', '%s')",
	    esc_owner, memo->number, memo->flags, memo->time, esc_sender,
	    esc_text);
	mysql_free_result(result);

	free(esc_text);
	free(esc_sender);
	free(esc_owner);

	return;
}

