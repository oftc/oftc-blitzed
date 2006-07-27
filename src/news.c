
/* News functions.
 * Based on code by Andrew Kempe (TheShadow)
 *     E-mail: <theshadow@shadowfire.org>
 *
 * Services is copyright (c) 1996-1999 Andrew Church.
 *     E-mail: <achurch@dragonfire.net>
 * Services is copyright (c) 1999-2000 Andrew Kempe.
 *     E-mail: <theshadow@shadowfire.org>
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include "services.h"
#include "pseudo.h"

RCSID("$Id$");

/*************************************************************************/

/* List of messages for each news type.  This simplifies message sending. */

#define MSG_SYNTAX	0
#define MSG_LIST_HEADER	1
#define MSG_LIST_ENTRY	2
#define MSG_LIST_NONE	3
#define MSG_ADD_SYNTAX	4
#define MSG_ADD_FULL	5
#define MSG_ADDED	6
#define MSG_DEL_SYNTAX	7
#define MSG_DEL_NOT_FOUND 8
#define MSG_DELETED	9
#define MSG_DEL_NONE	10
#define MSG_DELETED_ALL	11
#define MSG_MAX		11

struct newsmsgs {
	int type;
	char *name;
	unsigned int msgs[MSG_MAX + 1];
};
struct newsmsgs msgarray[] = {
	{NEWS_LOGON, "LOGON",
	 {NEWS_LOGON_SYNTAX,
	  NEWS_LOGON_LIST_HEADER,
	  NEWS_LOGON_LIST_ENTRY,
	  NEWS_LOGON_LIST_NONE,
	  NEWS_LOGON_ADD_SYNTAX,
	  NEWS_LOGON_ADD_FULL,
	  NEWS_LOGON_ADDED,
	  NEWS_LOGON_DEL_SYNTAX,
	  NEWS_LOGON_DEL_NOT_FOUND,
	  NEWS_LOGON_DELETED,
	  NEWS_LOGON_DEL_NONE,
	  NEWS_LOGON_DELETED_ALL}
	 },
	{NEWS_OPER, "OPER",
	 {NEWS_OPER_SYNTAX,
	  NEWS_OPER_LIST_HEADER,
	  NEWS_OPER_LIST_ENTRY,
	  NEWS_OPER_LIST_NONE,
	  NEWS_OPER_ADD_SYNTAX,
	  NEWS_OPER_ADD_FULL,
	  NEWS_OPER_ADDED,
	  NEWS_OPER_DEL_SYNTAX,
	  NEWS_OPER_DEL_NOT_FOUND,
	  NEWS_OPER_DELETED,
	  NEWS_OPER_DEL_NONE,
	  NEWS_OPER_DELETED_ALL}
	 }
};

static int *findmsgs(int type, char **typename)
{
	size_t i;

	for (i = 0; i < lenof(msgarray); i++) {
		if (msgarray[i].type == type) {
			if (typename)
				*typename = msgarray[i].name;
			return msgarray[i].msgs;
		}
	}
	return NULL;
}

/*************************************************************************/

/* Called by the main OperServ routine in response to a NEWS command. */
static void do_news(User * u, int type);

/* Lists all a certain type of news. */
static void do_news_list(User * u, int type, unsigned int *msgs);

/* Add news items. */
static void do_news_add(User * u, int type, unsigned int *msgs,
    const char *typename);
static int add_newsitem(User * u, const char *text, int type);

/* Immediately notify relevant people. */
static void notify_newsitem(const char *text, int type);

/* Delete news items. */
static void do_news_del(User * u, int type, unsigned int *msgs,
    const char *typename);
static int del_newsitem(unsigned int num, int type);

static unsigned int num_news(void);

/*************************************************************************/

/****************************** Statistics *******************************/

/*************************************************************************/

void get_news_stats(long *nrec, long *memuse)
{
	get_table_stats(mysqlconn, "news", (unsigned int *)nrec,
			(unsigned int *)memuse);
	if (*memuse)
		*memuse /= 1024;
}

/*************************************************************************/

/***************************** News display ******************************/

/*************************************************************************/

void display_news(User * u, int type)
{
	char timebuf[64];
	MYSQL_ROW row;
	time_t news_time;
	unsigned int fields, rows, msg;
	MYSQL_RES *result;
	struct tm *tm;

	if (type == NEWS_LOGON) {
		msg = NEWS_LOGON_TEXT;
	} else if (type == NEWS_OPER) {
		msg = NEWS_OPER_TEXT;
	} else {
		log("news: Invalid type (%d) to display_news()", type);
		return;
	}

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT text, time FROM news WHERE type=%u ORDER BY time DESC",
	    type);

	while (rows && (row = smysql_fetch_row(mysqlconn, result))) {
		news_time = atoi(row[1]);

		tm = localtime(&news_time);
		strftime_lang(timebuf, sizeof(timebuf), u,
		    STRFTIME_SHORT_DATE_FORMAT, tm);
		notice_lang(s_GlobalNoticer, u, msg, timebuf, row[0]);
	}
	mysql_free_result(result);
}

/*************************************************************************/

/***************************** News editing ******************************/

/*************************************************************************/

/* Declared in extern.h */
void do_logonnews(User * u)
{
	do_news(u, NEWS_LOGON);
}

/* Declared in extern.h */
void do_opernews(User * u)
{
	do_news(u, NEWS_OPER);
}

/*************************************************************************/

/* Main news command handling routine. */
void do_news(User * u, int type)
{
	char buf[32];
	int is_servadmin;
	char *cmd, *typename;
	unsigned int *msgs;

	is_servadmin = is_services_admin(u);
	cmd = strtok(NULL, " ");

	msgs = findmsgs(type, &typename);
	if (!msgs) {
		log("news: Invalid type to do_news()");
		return;
	}

	if (!cmd)
		cmd = "";

	if (stricmp(cmd, "LIST") == 0) {
		do_news_list(u, type, msgs);

	} else if (stricmp(cmd, "ADD") == 0) {
		if (is_servadmin)
			do_news_add(u, type, msgs, typename);
		else
			notice_lang(s_OperServ, u, PERMISSION_DENIED);

	} else if (stricmp(cmd, "DEL") == 0) {
		if (is_servadmin)
			do_news_del(u, type, msgs, typename);
		else
			notice_lang(s_OperServ, u, PERMISSION_DENIED);

	} else {
		snprintf(buf, sizeof(buf), "%sNEWS", typename);
		syntax_error(s_OperServ, u, buf, msgs[MSG_SYNTAX]);
	}
}

/*************************************************************************/

/* Handle a {LOGON,OPER}NEWS LIST command. */

static void do_news_list(User * u, int type, unsigned int *msgs)
{
	char timebuf[64];
	MYSQL_ROW row;
	time_t news_time;
	unsigned int fields, rows;
	MYSQL_RES *result;
	struct tm *tm;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT news_id, text, who, time FROM news WHERE type=%u",
	    type);

	if (rows) {
		notice_lang(s_OperServ, u, msgs[MSG_LIST_HEADER]);
		while ((row = smysql_fetch_row(mysqlconn, result))) {
			news_time = atoi(row[3]);
			tm = localtime(&news_time);
			strftime_lang(timebuf, sizeof(timebuf), u,
			    STRFTIME_DATE_TIME_FORMAT, tm);
			notice_lang(s_OperServ, u, msgs[MSG_LIST_ENTRY],
			    atoi(row[0]), timebuf, row[2], row[1]);
		}
	} else {
		notice_lang(s_OperServ, u, msgs[MSG_LIST_NONE]);
	}
	mysql_free_result(result);
}

/*************************************************************************/

/* Handle a {LOGON,OPER}NEWS ADD command. */

static void do_news_add(User * u, int type, unsigned int *msgs,
    const char *typename)
{
	char *text = strtok(NULL, "");

	if (!text) {
		char buf[32];

		snprintf(buf, sizeof(buf), "%sNEWS", typename);
		syntax_error(s_OperServ, u, buf, msgs[MSG_ADD_SYNTAX]);
	} else {
		int n = add_newsitem(u, text, type);

		if (n < 0)
			notice_lang(s_OperServ, u, msgs[MSG_ADD_FULL]);
		else {
			notify_newsitem(text, type);
			notice_lang(s_OperServ, u, msgs[MSG_ADDED], n);
		}
	}
}


/* Actually add a news item.  Return the number assigned to the item, or -1
 * if the news list is full (32767 items).
 */

static int add_newsitem(User * u, const char *text, int type)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_text, *esc_who;

	if (num_news() >= 32767)
		return -1;

	esc_text = smysql_escape_string(mysqlconn, text);
	esc_who = smysql_escape_string(mysqlconn, u->nick);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO news (news_id, type, text, who, time) "
	    "VALUES ('NULL', %u, '%s', " "'%s', %d)", type, esc_text,
	    esc_who, time(NULL));

	free(esc_who);
	free(esc_text);
	mysql_free_result(result);
	return(mysql_insert_id(mysqlconn));
}

static void notify_newsitem(const char *text, int type)
{
	switch (type) {
	case NEWS_LOGON:
		global_notice(s_GlobalNoticer, "[Logon News] %s", text);
		break;

	case NEWS_OPER:
		wallops(s_GlobalNoticer, "[Oper News] %s", text);
		break;
	}
}

/*************************************************************************/

/* Handle a {LOGON,OPER}NEWS DEL command. */

static void do_news_del(User * u, int type, unsigned int *msgs,
    const char *typename)
{
	char buf[32];
	unsigned int num;
	char *text;

	text = strtok(NULL, " ");

	if (!text) {
		snprintf(buf, sizeof(buf), "%sNEWS", typename);
		syntax_error(s_OperServ, u, buf, msgs[MSG_DEL_SYNTAX]);
	} else {
		if (stricmp(text, "ALL") != 0) {
			num = atoi(text);

			if (num > 0 && del_newsitem(num, type)) {
				notice_lang(s_OperServ, u, msgs[MSG_DELETED],
				    num);
			} else {
				notice_lang(s_OperServ, u,
				    msgs[MSG_DEL_NOT_FOUND], num);
			}
		} else {
			if (del_newsitem(0, type)) {
				notice_lang(s_OperServ, u,
				    msgs[MSG_DELETED_ALL]);
			} else {
				notice_lang(s_OperServ, u,
				    msgs[MSG_DEL_NONE]);
			}
		}
	}
}


/*
 * Actually delete a news item.  If `num' is 0, delete all news items of
 * the given type.  Returns the number of items deleted.
 */

static int del_newsitem(unsigned int num, int type)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	if (num == 0) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "DELETE FROM news WHERE type=%u", type);
	} else {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "DELETE FROM news WHERE news_id=%u", num);
	}

	mysql_free_result(result);

	return(rows);
}

/*************************************************************************/

static unsigned int num_news(void)
{
	return(get_table_rows(mysqlconn, "news"));
}
