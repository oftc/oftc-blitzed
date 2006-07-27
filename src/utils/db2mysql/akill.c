
/*
 * AutoKill functions for db2mysql.
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

static void db_add_akill(Akill *akill);

/*************************************************************************/

void load_akill_dbase(void)
{
	size_t akill_size;
	int ver;
	unsigned int i, nakill;
	int16 tmp16;
	int32 tmp32;
	dbFILE *f;
	Akill *akills;

	if (!(f = open_db("AKILL", AutoKillDBName, "r")))
		return;

	ver = get_file_version(f);

	read_int16(&tmp16, f);
	nakill = tmp16;

	if (nakill < 8)
		akill_size = 16;
	else if (nakill >= 16384)
		akill_size = 32767;
	else
		akill_size = 2 * nakill;

	akills = scalloc(sizeof(*akills), akill_size);

	if (ver != 11) {
		fprintf(stderr, "Unsupported file version %d in %s!\n", ver,
		    AutoKillDBName);
		return;
	}

	debug("Got AutoKill DB version %d", ver);

	for (i = 0; i < nakill; i++) {
		read_string(&akills[i].mask, f);
		read_string(&akills[i].reason, f);
		read_buffer(akills[i].who, f);
		read_int32(&tmp32, f);
		akills[i].time = tmp32;
		read_int32(&tmp32, f);
		akills[i].expires = tmp32;
		db_add_akill(&(akills[i]));
		info("Added akill on '%s'", akills[i].mask);
	}

	close_db(f);
}

static void db_add_akill(Akill *akill)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_mask, *esc_reason, *esc_who;

	esc_mask = smysql_escape_string(mysqlconn, akill->mask);
	esc_reason = smysql_escape_string(mysqlconn, akill->reason);
	esc_who = smysql_escape_string(mysqlconn, akill->who);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO akill (akill_id, mask, reason, who, time, expires) "
	    "VALUES ('NULL', '%s', '%s', '%s', %lu, %lu)", esc_mask,
	    esc_reason, esc_who, akill->time, akill->expires);
	mysql_free_result(result);

	free(esc_who);
	free(esc_reason);
	free(esc_mask);

	return;
}
