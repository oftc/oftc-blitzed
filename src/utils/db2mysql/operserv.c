
/*
 * OperServ functions for db2mysql.
 * 
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 * 
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <ctype.h>
#include <mysql.h>
#include <stdlib.h>

#include "../sysconf.h"
#include "db2mysql.h"
#include "extern.h"

RCSID("$Id$");

extern MYSQL *mysqlconn;

NickInfo *services_admins[MAX_SERVADMINS];
NickInfo *services_opers[MAX_SERVOPERS];

static void db_add_admin(unsigned int nick_id);
static void db_add_oper(unsigned int nick_id);
static void db_set_maxusercnt(unsigned int cnt);
static void db_set_maxusertime(time_t time);

/*************************************************************************/

void load_os_dbase(void)
{
	time_t maxusertime;
	unsigned int maxusercnt;
	int i;
	int32 tmp32;
	int16 ver, n;
	dbFILE *f;
	char *s;

	if (!(f = open_db(s_NickServ, OperDBName, "r")))
		return;

	ver = get_file_version(f);

	if (ver != 11) {
		fprintf(stderr, "Unsupported file version %d in %s!\n", ver,
		    NickDBName);
		return;
	}

	debug("Got %s DB version %d", s_OperServ, ver);

	read_int16(&n, f);
	
	for (i = 0; i < n; i++) {
		read_string(&s, f);
		if (s && i < MAX_SERVADMINS) {
			services_admins[i] = findnick(s);
			db_add_admin(services_admins[i]->nick_id);
			info("Added '%s' as a services admin",
			    services_admins[i]->nick);
		}

		if (s)
			free(s);
	}

	read_int16(&n, f);

	for (i = 0; i < n; i++) {
		read_string(&s, f);
		if (s && i < MAX_SERVOPERS) {
			services_opers[i] = findnick(s);
			db_add_oper(services_opers[i]->nick_id);
			info("Added '%s' as a services oper",
			    services_opers[i]->nick);
		}

		if (s)
			free(s);
	}

	read_int32(&maxusercnt, f);
	read_int32(&tmp32, f);
	maxusertime = tmp32;

	db_set_maxusercnt(maxusercnt);
	db_set_maxusertime(maxusertime);

	close_db(f);
}

static void db_add_admin(unsigned int nick_id)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO admin (admin_id, nick_id) VALUES ('NULL', %u)",
	    nick_id);
	mysql_free_result(result);

	return;
}

static void db_add_oper(unsigned int nick_id)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO oper (oper_id, nick_id) VALUES ('NULL', %u)",
	    nick_id);
	mysql_free_result(result);

	return;
}

static void db_set_maxusercnt(unsigned int cnt)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_query(mysqlconn, &fields, &rows,
	    "REPLACE INTO stat (name, value) VALUES ('maxusercnt', '%u')",
	    cnt);
	mysql_free_result(result);

	return;
}

static void db_set_maxusertime(time_t time)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_query(mysqlconn, &fields, &rows,
	    "REPLACE INTO stat (name, value) VALUES ('maxusertime', '%lu')",
	    time);
	mysql_free_result(result);

	return;
}
