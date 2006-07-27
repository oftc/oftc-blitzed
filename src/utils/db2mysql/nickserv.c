
/*
 * Nick-related functions for db2mysql.
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

static NickSuspend *add_nicksuspend(NickInfo *ni, const char *reason,
    const char *who, const time_t expires);
static int insert_new_nick(NickInfo *ni);
static void db_add_nicksuspend(unsigned int nick_id, NickSuspend *snick);
static void db_add_nickaccess(unsigned int nick_id, unsigned int idx,
    const char *mask);
static NickInfo *getlink(NickInfo *ni);
static void db_nicklink(unsigned int nick_id, unsigned int link_id);

/* All this is initialized to zeros */
NickInfo *nicklists[256];
NickSuspend *nicksuspends;

/*************************************************************************/

void load_ns_dbase(void)
{
	char salt[SALTMAX];
	int ver, c;
	unsigned int i, j;
	int32 tmp32;
	NickInfo *ni, **last, *prev, *master_ni;
	dbFILE *f;
	NickSuspend *snick;
	char **access;
	Memo *memos;

	snick = NULL;

	if (!(f = open_db(s_NickServ, NickDBName, "r")))
		return;

	ver = get_file_version(f);

	if (ver != 11) {
		fprintf(stderr, "Unsupported file version %d in %s!\n", ver,
		    NickDBName);
		return;
	}

	debug("Got %s DB version %d", s_NickServ, ver);

	for (i = 0; i < 255; i++) {
		last = &nicklists[i];
		prev = NULL;

		while ((c = getc_db(f)) == 1) {
			if (c != 1) {
				fprintf(stderr, "Invalid format in %s\n",
				    NickDBName);
				return;
			}

			ni = scalloc(sizeof(*ni), 1);
			*last = ni;
			last =&ni->next;
			ni->prev = prev;
			prev = ni;
			ni->chanlist = NULL;
			read_buffer(ni->nick, f);
			read_buffer(ni->pass, f);
			read_string(&ni->url, f);
			read_string(&ni->email, f);
			read_string(&ni->last_usermask, f);
			if (!ni->last_usermask)
				ni->last_usermask = sstrdup("@");
			read_string(&ni->last_realname, f);
			if (!ni->last_realname)
				ni->last_realname = sstrdup("");
			read_string(&ni->last_quit, f);
			read_int32(&tmp32, f);
			ni->time_registered = tmp32;
			read_int32(&tmp32, f);
			ni->last_seen = tmp32;
			read_int16(&ni->status, f);
			ni->status &= ~NS_TEMPORARY;

			read_string((char **)&ni->link, f);
			read_int16(&ni->linkcount, f);

			debug("Got nick '%s'", ni->nick);
			debug("  Pass: '%s'", ni->pass);
			debug("  URL: '%s'", ni->url ? ni->url : "(none)");
			debug("  email: '%s'", ni->email ? ni->email : "(none)");
			debug("  last address: '%s'", ni->last_usermask);
			debug("  last realname: '%s'", ni->last_realname);
			debug("  last quit: '%s'", ni->last_quit);
			debug("  time reg'd: '%lu'", ni->time_registered);
			debug("  last seen: '%lu'", ni->last_seen);
			debug("  status: '%d'", ni->status);
			
			if (ni->link) {
				read_int16(&ni->channelcount, f);
				ni->flags = 0;
				ni->accesscount = 0;
				ni->access = NULL;
				ni->memos.memocount = 0;
				ni->memos.memomax = MSMaxMemos;
				ni->memos.memos = NULL;
				ni->channelmax = CSMaxReg;
				ni->language = DEF_LANGUAGE;
				debug("  linked to: '%s'", (char *)ni->link);
			} else {
				read_int32(&ni->flags, f);

				ni->flags |= NI_AUTOJOIN;

				debug("  flags: '%d'", ni->flags);
				if (ni->flags & NI_SUSPENDED) {
					snick = add_nicksuspend(ni, "\0",
					    "\0", 0);
					read_buffer(snick->suspendinfo.who,
					    f);
					read_string(&snick->suspendinfo.reason,
					    f);
					read_int32(&tmp32, f);
					snick->suspendinfo.suspended = tmp32;
					read_int32(&tmp32, f);
					snick->suspendinfo.expires = tmp32;
					debug("  nick is suspended:");
					debug("    by: '%s'", snick->suspendinfo.who);
					debug("    reason: '%s'", snick->suspendinfo.reason);
					debug("    when: '%lu'", snick->suspendinfo.suspended);
					debug("    expires: '%lu'", snick->suspendinfo.expires);
				}

				read_int16(&ni->accesscount, f);

				if (ni->accesscount) {
					access = smalloc(sizeof(*access) *
					    ni->accesscount);
					ni->access = access;

					debug("  %d access list entries:", ni->accesscount);

					for(j = 0;
					    j < (unsigned int)ni->accesscount;
					    j++, access++) {
						read_string(access, f);
						debug("    '%s'", *access);
					}
				}

				read_int16(&ni->memos.memocount, f);
				read_int16(&ni->memos.memomax, f);

				if (ni->memos.memocount) {
					memos = smalloc(sizeof(*memos) *
					    ni->memos.memocount);
					ni->memos.memos = memos;

					debug("  %d of %d memos:",
					    ni->memos.memocount,
					    ni->memos.memomax);
					
					for (j = 0;
					    j < (unsigned int)ni->memos.memocount;
					    j++, memos++) {
						read_int32(&memos->number, f);
						read_int16(&memos->flags, f);
						read_int32(&tmp32, f);
						memos->time = tmp32;
						read_buffer(memos->sender, f);
						read_string(&memos->text, f);
						debug("    memo %d:",
						    memos->number);
						debug("      from: '%s'",
						    memos->sender);
						debug("      time: '%lu'",
						    memos->time);
						debug("      flags: '%d'",
						    memos->flags);
						debug("      text: '%s'",
						    memos->text);
						debug("");
					}
				}

				read_int16(&ni->channelcount, f);
				read_int16(&ni->channelmax, f);
				read_int16(&ni->language, f);

				debug("  channels: %d of %d",
				    ni->channelcount, ni->channelmax);
				debug("  language: %d", ni->language);
				debug("");
			}
			
			memset(salt, 0, sizeof(salt));
			make_random_string(salt, sizeof(salt));
			strscpy(ni->salt, salt, SALTMAX);

			ni->nick_id = insert_new_nick(ni);
			info("Added nick '%s' as nick_id %u", ni->nick,
			    ni->nick_id);

			if (ni->flags & NI_SUSPENDED && snick) {
				db_add_nicksuspend(ni->nick_id, snick);
				info("Added nick suspend for '%s'", ni->nick);
			}

			for (j = 0; j < (unsigned int)ni->accesscount; j++) {
				db_add_nickaccess(ni->nick_id, j + 1,
				    ni->access[j]);
				info("Added access list entry '%s' for "
				    "nick '%s'", ni->access[j], ni->nick);
			}

			db_add_memoinfo(ni->nick,
			    (unsigned int)ni->memos.memomax);
			info("Added memoinfo for nick '%s'", ni->nick);

			for (j = 0; j < (unsigned int)ni->memos.memocount;
			    j++) {
				db_add_memo(ni->nick, &(ni->memos.memos[j]));
				info("Added memo number %u for nick '%s'",
				    ni->memos.memos[j].number, ni->nick);
			}
					
			ni->id_stamp = 0;
		}

		*last = NULL;
	}

	for (i = 0; i < 256; i++) {
		for (ni = nicklists[i]; ni; ni = ni->next) {
			if (ni->link)
				ni->link = findnick((char *)ni->link);
		}
	}

	for (i = 0; i < 256; i++) {
		for (ni = nicklists[i]; ni; ni = ni->next) {
			if (ni->link) {
				master_ni = getlink(ni->link);
				ni->link_id = master_ni->nick_id;
				db_nicklink(ni->nick_id, ni->link_id);
			}
		}
	}

	close_db(f);
}

static int insert_new_nick(NickInfo *ni)
{
	MYSQL_ROW row;
	unsigned int fields, rows, nick_id;
	MYSQL_RES *result;
	char *esc_nick, *esc_pass, *esc_email, *esc_url;
	char *esc_last_usermask, *esc_last_realname;
	
	esc_nick = smysql_escape_string(mysqlconn, ni->nick);
	esc_pass = smysql_escape_string(mysqlconn, ni->pass);
	esc_email = ni->email ? smysql_escape_string(mysqlconn, ni->email) :
	    sstrdup("");
	esc_url = ni->url ? smysql_escape_string(mysqlconn, ni->url) :
	    sstrdup("");
	esc_last_usermask = smysql_escape_string(mysqlconn,
	    ni->last_usermask);
	esc_last_realname = smysql_escape_string(mysqlconn,
	    ni->last_realname);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO nick (nick_id, nick, pass, salt, email, url, "
	    "last_usermask, last_realname, time_registered, last_seen, "
	    "status, flags, channelmax, language, id_stamp, regainid) "
	    "VALUES ('NULL', '%s', SHA1('%s%s'), '%s', '%s', '%s', '%s', "
	    "'%s', %lu, %lu, %d, %d, %u, %u, %lu, %u)", esc_nick, esc_pass,
	    ni->salt, ni->salt, esc_email, esc_url, esc_last_usermask,
	    esc_last_realname, ni->time_registered, ni->last_seen,
	    ni->status, ni->flags, ni->channelmax, ni->language,
	    ni->id_stamp, ni->regainid);
	mysql_free_result(result);

	nick_id = mysql_insert_id(mysqlconn);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "SELECT pass FROM nick WHERE nick_id='%u'", nick_id);

	row = smysql_fetch_row(mysqlconn, result);
	strscpy(ni->pass, row[0], PASSMAX);
	mysql_free_result(result);

	free(esc_nick);
	free(esc_pass);
	free(esc_email);
	free(esc_url);
	free(esc_last_usermask);
	free(esc_last_realname);

	return(nick_id);
}

static void db_add_nickaccess(unsigned int nick_id, unsigned int idx,
    const char *mask)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_mask;

	esc_mask = smysql_escape_string(mysqlconn, mask);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO nickaccess (nickaccess_id, nick_id, idx, userhost) "
	    "VALUES ('NULL', %u, %u, '%s')", nick_id, idx, esc_mask);
	mysql_free_result(result);

	free(esc_mask);
	return;
}

static NickInfo *getlink(NickInfo *ni)
{
	int i;
	NickInfo *orig;

	i = 0;
	orig = ni;

	while (ni->link && ++i < 512)
		ni = ni->link;

	if (i >= 512) {
		fprintf(stderr, "Infinite loop (?) found at nick %s for "
		    "nick %s, cutting link", ni->nick, orig->nick);
		orig->link = NULL;
		ni = orig;
	}

	return(ni);
}

/* Find a nickname or channel. */

NickInfo *findnick(const char *nick)
{
	NickInfo *ni;

	for (ni = nicklists[tolower(*nick)]; ni; ni = ni->next) {
		if (stricmp(ni->nick, nick) == 0)
			return(ni);
	}
	return(NULL);
}

static NickSuspend *add_nicksuspend(NickInfo *ni, const char *reason,
    const char *who, const time_t expires)
{
	NickSuspend *snick;

	snick = scalloc(sizeof(*snick), 1);
	snick->ni = ni;
	strscpy(snick->suspendinfo.who, who, NICKMAX);
	snick->suspendinfo.reason = sstrdup(reason);
	snick->suspendinfo.suspended = time(NULL);
	snick->suspendinfo.expires = expires;

	if (nicksuspends)
		nicksuspends->prev = snick;
	snick->prev = NULL;
	snick->next = nicksuspends;
	nicksuspends = snick;

	return(snick);
}

static void db_add_nicksuspend(unsigned int nick_id, NickSuspend *snick)
{
	unsigned int fields, rows, suspend_id;
	MYSQL_RES *result;
	char *esc_who, *esc_reason;

	esc_who = smysql_escape_string(mysqlconn, snick->suspendinfo.who);
	esc_reason = smysql_escape_string(mysqlconn,
	    snick->suspendinfo.reason);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO suspend (suspend_id, who, reason, suspended, "
	    "expires) VALUES ('NULL', '%s', '%s', %lu, %lu)", esc_who,
	    esc_reason, snick->suspendinfo.suspended,
	    snick->suspendinfo.expires);
	mysql_free_result(result);

	free(esc_reason);
	free(esc_who);

	suspend_id = mysql_insert_id(mysqlconn);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO nicksuspend (nicksuspend_id, nick_id, suspend_id) "
	    "VALUES ('NULL', %u, %u)", nick_id, suspend_id);
	mysql_free_result(result);

	return;
}

static void db_nicklink(unsigned int nick_id, unsigned int link_id)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	fprintf(stderr, "Linking %u to %u\n", nick_id, link_id);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "UPDATE nick SET link_id=%u WHERE nick_id=%u", link_id,
	    nick_id);
	mysql_free_result(result);

	return;
}
