
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

static void db_add_news(NewsItem *news);

/*************************************************************************/

void load_news_dbase(void)
{
	size_t news_size;
	int ver;
	unsigned int i, nnews;
	int32 tmp32;
	int16 tmp16;
	dbFILE *f;
	NewsItem *news;
	
	if (!(f = open_db(s_OperServ, NewsDBName, "r")))
		return;

	ver = get_file_version(f);

	if (ver != 11) {
		fprintf(stderr, "Unsupported file version %d in %s!\n", ver,
		    NewsDBName);
		return;
	}

	debug("Got News DB version %d", ver);

	read_int16(&tmp16, f);
	nnews = tmp16;

	if (nnews < 8)
		news_size = 16;
	else if (nnews > 16384)
		news_size = 32767;
	else
		news_size = 2 * nnews;

	news = smalloc(sizeof(*news) * news_size);

	if (!nnews) {
		fprintf(stderr, "Error allocating storage for news\n");
		close_db(f);
		return;
	}

	for (i = 0; i < nnews; i++) {
		read_int16(&news[i].type, f);
		read_int32(&news[i].num, f);
		read_string(&news[i].text, f);
		read_buffer(news[i].who, f);
		read_int32(&tmp32, f);
		news[i].time = tmp32;
		db_add_news(&(news[i]));
		info("Added news item '%s'", news[i].text);
	}

	close_db(f);
}

static void db_add_news(NewsItem *news)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_text, *esc_who;

	esc_text = smysql_escape_string(mysqlconn, news->text);
	esc_who = smysql_escape_string(mysqlconn, news->who);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO news (news_id, type, text, who, time) VALUES "
	    "('NULL', %u, '%s', '%s', %lu)", news->type, esc_text, esc_who,
	    news->time);
	mysql_free_result(result);

	free(esc_who);
	free(esc_text);

	return;
}
