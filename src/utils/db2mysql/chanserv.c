
/*
 * Channel-related functions for db2mysql.
 * 
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 * 
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include <time.h>
#include <stdio.h>
#include <sys/param.h>
#include <ctype.h>
#include <mysql.h>
#include <stdlib.h>

#include "../sysconf.h"
#include "db2mysql.h"
#include "extern.h"

RCSID("$Id$");

extern MYSQL *mysqlconn;

ChannelInfo *chanlists[256];

static int def_levels[][2] = {
	{CA_AUTOOP, 5},
	{CA_AUTOVOICE, 3},
	{CA_AUTODEOP, -1},
	{CA_NOJOIN, -2},
	{CA_INVITE, 5},
	{CA_AKICK, 10},
	{CA_SET, ACCESS_INVALID},
	{CA_CLEAR, ACCESS_INVALID},
	{CA_UNBAN, 5},
	{CA_OPDEOP, 5},
	{CA_ACCESS_LIST, 1},
	{CA_ACCESS_CHANGE, 10},
	{CA_MEMO_READ, 1},
	{CA_LEVEL_LIST, 1},
	{CA_LEVEL_CHANGE, ACCESS_INVALID},
	{CA_SYNC, 10},
	{CA_MEMO_SEND, 10},
	{CA_AKICK_LIST, 10},
	{-1}
};

static void reset_levels(ChannelInfo *ci);
static int delchan(ChannelInfo *ci);
static unsigned int insert_new_channel(ChannelInfo *ci);
static void db_set_level(unsigned int channel_id, int what, int level);
static void db_add_access(unsigned int channel_id, unsigned int idx,
    ChanAccess *access);
static void db_add_akick(unsigned int channel_id, unsigned int idx,
    AutoKick *akick);

/*************************************************************************/

void load_cs_dbase(void)
{
	int ver, c;
	unsigned int i, j, n_levels;
	int32 tmp32;
	int16 tmp16;
	ChannelInfo *ci, *prev, *next, **last;
	dbFILE *f;
	char *s;
	Memo *memos;

	if (!(f = open_db(s_ChanServ, ChanDBName, "r")))
		return;

	ver = get_file_version(f);

	if (ver != 11) {
		fprintf(stderr, "Unsupported file version %d in %s!\n", ver,
		    ChanDBName);
		exit(EXIT_FAILURE);
	}

	debug("Got %s DB version %d", s_ChanServ, ver);

	for (i = 0; i < 255; i++) {
		last = &chanlists[i];
		prev = NULL;

		while ((c = getc_db(f)) != 0) {
			if (c != 1) {
				fprintf(stderr, "Invalid format in %s - "
				    "expected 1, got %d (%o octal)\n",
				    ChanDBName, c, c);
				exit(EXIT_FAILURE);
			}

			ci = smalloc(sizeof(*ci));
			*last = ci;
			last = &ci->next;
			ci->prev = prev;
			prev = ci;

			read_buffer(ci->name, f);

			debug("Got channel: '%s'", ci->name);

			read_string(&s, f);

			if (s) {
				ci->founder_ni = findnick(s);
				debug("  Founder: '%s'", ci->founder_ni->nick);
			} else {
				ci->founder_ni = NULL;
			}

			read_string(&s, f);

			if (s) {
				ci->successor_ni = findnick(s);
				if (ci->successor_ni) {
					debug("  successor: '%s'",
					    ci->successor_ni->nick);
				}
			} else {
				ci->successor_ni = NULL;
			}

			if (ci->founder_ni == ci->successor_ni)
				ci->successor_ni = NULL;

			read_buffer(ci->founderpass, f);
			debug("  password: '%s'", ci->founderpass);
			read_string(&ci->desc, f);

			if (!ci->desc)
				ci->desc = sstrdup("");
			debug("  description: '%s'", ci->desc);

			read_string(&ci->url, f);
			debug("  url: '%s'", ci->url);
			read_string(&ci->email, f);
			debug("  email: '%s'", ci->email);

			read_int32(&tmp32, f);
			ci->time_registered = tmp32;
			debug("  registered: '%lu'", ci->time_registered);

			read_int32(&tmp32, f);
			ci->last_used = tmp32;
			debug("  last used: '%lu'", ci->last_used);

			read_int32(&tmp32, f);
			ci->founder_memo_read = tmp32;
			debug("  founder memo read: '%lu'", ci->founder_memo_read);

			read_string(&ci->last_topic, f);
			debug("  topic: '%s'", ci->last_topic);
			read_buffer(ci->last_topic_setter, f);
			debug("  topic setter: '%s'", ci->last_topic_setter);
			read_int32(&tmp32, f);
			ci->last_topic_time = tmp32;
			debug("  topic time: '%lu'", ci->last_topic_time);
			read_int32(&ci->flags, f);
			debug("  flags: '%d'", ci->flags);

			read_int16(&tmp16, f);
			n_levels = tmp16;
			debug("  %d levels:", n_levels);
			ci->levels = smalloc(2 * CA_SIZE);
			reset_levels(ci);

			for (j = 0; j < n_levels; j++) {
				if (j < CA_SIZE) {
					read_int16(&ci->levels[j], f);
					debug("    %d: %d", j + 1,
					    ci->levels[j]);
				} else {
					read_int16(&tmp16, f);
				}
			}

			read_int16(&ci->accesscount, f);

			if (ci->accesscount) {
				ci->access =
				    scalloc((unsigned int)ci->accesscount,
				    sizeof(ChanAccess));

				debug("  %d access entries:", ci->accesscount);

				for (j = 0;
				    j < (unsigned int)ci->accesscount;
				    j++) {
					read_int16(&ci->access[j].in_use,
					    f);
					
					if (!ci->access[j].in_use) {
						debug("    %d: (unused)",
						    j + 1);
						continue;
					}
							
					read_int16(&ci->access[j].level, f);
					read_string(&s, f);

					if (s) {
						ci->access[j].ni = findnick(s);
						free(s);
					}

					if (ci->access[j].ni == NULL)
						ci->access[j].in_use = 0;

					read_int32(&tmp32, f);
					ci->access[j].added = tmp32;

					read_int32(&tmp32, f);
					ci->access[j].last_modified = tmp32;
						
					read_int32(&tmp32, f);
					ci->access[j].last_used = tmp32;

					read_int32(&tmp32, f);
					ci->access[j].memo_read = tmp32;

					read_buffer(ci->access[j].added_by, f);
					read_buffer(ci->access[j].modified_by, f);
					debug("    %d: %s (level %d)", j + 1,
					    ci->access[j].ni->nick,
					    ci->access[j].level);
				}
			} else {
				ci->access = NULL;
			}

			read_int16(&ci->akickcount, f);

			if (ci->akickcount) {
				ci->akick =
				    scalloc((unsigned int)ci->akickcount,
				    sizeof(AutoKick));

				debug("  %d akicks:", ci->akickcount);

				for (j = 0; j < (unsigned int)ci->akickcount;
				    j++) {
					read_int16(&ci->akick[j].in_use, f);

					if (!ci->akick[j].in_use)
						continue;

					read_int16(&ci->akick[j].is_nick, f);
					read_string(&s, f);

					if (ci->akick[j].is_nick) {
						ci->akick[j].u.ni = findnick(s);

						if (!ci->akick[j].u.ni)
							ci->akick[j].in_use = 0;

						free(s);
					} else {
						ci->akick[j].u.mask = s;
					}

					read_string(&s, f);

					if (ci->akick[j].in_use)
						ci->akick[j].reason = s;
					else if (s)
						free(s);

					read_buffer(ci->akick[j].who, f);

					read_int32(&tmp32, f);
					ci->akick[j].added = tmp32;

					read_int32(&tmp32, f);
					ci->akick[j].last_used = tmp32;

					debug("    %d: %s '%s'", j + 1,
					    ci->akick[j].is_nick ?
					    ci->akick[j].u.ni->nick :
					    ci->akick[j].u.mask,
					    ci->akick[j].reason);
				}
			} else {
				ci->akick = NULL;
			}

			read_int16(&ci->mlock_on, f);
			debug("  mlock on: '%d'", ci->mlock_on);
			read_int16(&ci->mlock_off, f);
			debug("  mlock off: '%d'", ci->mlock_off);
			read_int32(&ci->mlock_limit, f);
			debug("  mlock limit: '%d'", ci->mlock_limit);
			read_string(&ci->mlock_key, f);
			debug("  mlock key: '%s'", ci->mlock_key);

			read_int16(&ci->memos.memocount, f);
			read_int16(&ci->memos.memomax, f);

			debug("  %d of %d memos:", ci->memos.memocount,
			    ci->memos.memomax);
			
			if (ci->memos.memocount) {
				memos = smalloc(sizeof(Memo) * ci->memos.memocount);
				ci->memos.memos = memos;

				for (j = 0;
				    j < (unsigned int)ci->memos.memocount;
				    j++, memos++) {
					read_int32(&memos->number, f);
					read_int16(&memos->flags, f);
					read_int32(&tmp32, f);
					memos->time = tmp32;
					read_buffer(memos->sender, f);
					read_string(&memos->text, f);
					debug("  %d: From '%s' '%s'",
					    memos->number, memos->sender,
					    memos->text);
				}
			}

			ci->last_limit_time = time(NULL);
			read_int16(&tmp16, f);
			ci->limit_offset = tmp16;
			debug("  limit offset: '%d'", ci->limit_offset);
			read_int16(&tmp16, f);
			ci->limit_tolerance = tmp16;
			debug("  limit tolerance: '%d'", ci->limit_tolerance);
			read_int16(&tmp16, f);
			ci->limit_period = tmp16;
			debug("  limit period: '%d'", ci->limit_period);

			read_int16(&tmp16, f);
			ci->bantime = tmp16;
			debug("  ban time: '%d'", ci->bantime);

			read_string(&ci->entry_message, f);
			debug("  entry message: '%s'", ci->entry_message);

			debug("");
		}

		*last = NULL;
	}

	close_db(f);

	for (i = 0; i < 256; i++) {
		for (ci = chanlists[i]; ci; ci = next) {
			next = ci->next;

			if (!(ci->flags & CI_VERBOTEN) && !ci->founder_ni) {
				fprintf(stderr,
				    "Deleting founderless channel %s\n",
				    ci->name);
				delchan(ci);
			}

			if (!(ci->flags & CI_VERBOTEN))
				ci->founder = ci->founder_ni->nick_id;
			else
				ci->founder = 0;

			if (ci->successor_ni)
				ci->successor = ci->successor_ni->nick_id;
			else
				ci->successor = 0;

			ci->channel_id = insert_new_channel(ci);

			for (j = 0; j < CA_SIZE; j++) {
				if (ci->levels[j] != def_levels[j][1]) {
					db_set_level(ci->channel_id,
					    (signed)j, ci->levels[j]);
					info("Set custom level %d at %d "
					    "for %s", j, ci->levels[j],
					    ci->name);
				}
			}

			for (j = 0; j < (unsigned int)ci->accesscount;
			    j++) {
				if (!ci->access[j].in_use)
					continue;
				
				db_add_access(ci->channel_id,
				    j + 1, &(ci->access[j]));
				info("Added access in %s for %s (level %d)",
				    ci->name, ci->access[j].ni->nick,
				    ci->access[j].level);
			}

			for (j = 0; j < (unsigned int)ci->akickcount;
			    j++) {
				if (!ci->akick[j].in_use)
					continue;

				db_add_akick(ci->channel_id, j + 1,
				    &(ci->akick[j]));
				info("Added akick in %s for %s", ci->name,
				    ci->akick[j].is_nick ?
				    ci->akick[j].u.ni->nick :
				    ci->akick[j].u.mask);
			}

			db_add_memoinfo(ci->name,
			    (unsigned int)ci->memos.memomax);
			info("Added memoinfo for channel '%s'", ci->name);

			for (j = 0;
			    j < (unsigned int)ci->memos.memocount;
			    j++) {
				db_add_memo(ci->name, &(ci->memos.memos[j]));
				info("Added memo number %u for channel '%s'",
				    ci->memos.memos[j].number, ci->name);
			}
					
		}
	}
}
		
static void reset_levels(ChannelInfo *ci)
{
	int i;

	if (ci->levels)
		free(ci->levels);

	ci->levels = smalloc(CA_SIZE * sizeof(*ci->levels));

	for (i = 0; def_levels[i][0] >= 0; i++)
		ci->levels[def_levels[i][0]] = def_levels[i][1];
}

static int delchan(ChannelInfo *ci)
{
	int i;
	NickInfo *ni;

	ni = ci->founder_ni;

	if (ci->next)
		ci->next->prev = ci->prev;
	
	if (ci->prev)
		ci->prev->next = ci->next;
	else
		chanlists[tolower(ci->name[1])] = ci->next;
	
	if (ci->desc)
		free(ci->desc);
	
	if (ci->mlock_key)
		free(ci->mlock_key);
	
	if (ci->last_topic)
		free(ci->last_topic);
	
	if (ci->access)
		free(ci->access);

	for (i = 0; i < ci->akickcount; i++) {
		if (!ci->akick[i].is_nick && ci->akick[i].u.mask)
			free(ci->akick[i].u.mask);

		if (ci->akick[i].reason)
			free(ci->akick[i].reason);
	}

	if (ci->akick)
		free(ci->akick);

	if (ci->levels)
		free(ci->levels);

	if (ci->memos.memos) {
		for (i = 0; i < ci->memos.memocount; i++) {
			if (ci->memos.memos[i].text)
				free(ci->memos.memos[i].text);
		}
		free(ci->memos.memos);
	}
	free(ci);

	while (ni) {
		if (ni->channelcount > 0)
			ni->channelcount--;
		ni = ni->link;
	}

	return(1);
}

static unsigned int insert_new_channel(ChannelInfo *ci)
{
	char salt[SALTMAX];
	unsigned int channel_id, fields, rows;
	MYSQL_RES *result;
	char *esc_name, *esc_desc, *esc_url, *esc_email, *esc_last_topic;
	char *esc_last_topic_setter, *esc_mlock_key, *esc_entry_message;
	char *esc_last_sendpass_pass, *esc_last_sendpass_salt;

	make_random_string(salt, sizeof(salt));

	esc_name = smysql_escape_string(mysqlconn, ci->name);
	esc_desc = smysql_escape_string(mysqlconn, ci->desc);
	esc_url = smysql_escape_string(mysqlconn, ci->url ? ci->url : "");
	esc_email = smysql_escape_string(mysqlconn,
	    ci->email ? ci->email : "");
	esc_last_topic = smysql_escape_string(mysqlconn,
	    ci->last_topic ? ci->last_topic : "");
	esc_last_topic_setter = smysql_escape_string(mysqlconn,
	    ci->last_topic_setter ? ci->last_topic_setter : "");
	esc_mlock_key = smysql_escape_string(mysqlconn,
	    ci->mlock_key ? ci->mlock_key : "");
	esc_entry_message = smysql_escape_string(mysqlconn,
	    ci->entry_message ? ci->entry_message : "");
	esc_last_sendpass_pass = smysql_escape_string(mysqlconn,
	    ci->last_sendpass_pass ? ci->last_sendpass_pass : "");
	esc_last_sendpass_salt = smysql_escape_string(mysqlconn,
	    ci->last_sendpass_salt ? ci->last_sendpass_salt : "");

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO channel (channel_id, name, founder, successor, "
	    "founderpass, salt, description, url, email, time_registered, "
	    "last_used, founder_memo_read, last_topic, last_topic_setter, "
	    "last_topic_time, flags, mlock_on, mlock_off, mlock_limit, "
	    "mlock_key, entry_message, last_limit_time, limit_offset, "
	    "limit_tolerance, limit_period, bantime, last_sendpass_pass, "
	    "last_sendpass_salt, last_sendpass_time) "
	    "VALUES ('NULL', "	/* channel_id */
	    "'%s', "		/* name */
	    "'%u', "		/* founder */
	    "'%u', "		/* successor */
	    "SHA1('%s%s'), "	/* founderpass */
	    "'%s', "		/* salt */
	    "'%s', "		/* description */
	    "'%s', "		/* url */
	    "'%s', "		/* email */
	    "'%lu', "		/* time_registered */
	    "'%lu', "		/* last_used */
	    "'%lu', "		/* founder_memo_read */
	    "'%s', "		/* last_topic */
	    "'%s', "		/* last_topic_setter */
	    "'%lu', "		/* last_topic_time */
	    "'%d', "		/* flags */
	    "'%d', "		/* mlock_on */
	    "'%d', "		/* mlock_off */
	    "'%u', "		/* mlock_limit */
	    "'%s', "		/* mlock_key */
	    "'%s', "		/* entry_message */
	    "'%lu', "		/* last_limit_time */
	    "'%u', "		/* limit_offset */
	    "'%u', "		/* limit_tolerance */
	    "'%u', "		/* limit_period */
	    "'%u', "		/* bantime */
	    "'%s', "		/* last_sendpass_pass */
	    "'%s', "		/* last_sendpass_salt */
	    "'%lu')",		/* last_sendpass_time */
	    esc_name, ci->founder, ci->successor, ci->founderpass, salt,
	    salt, esc_desc, esc_url, esc_email, ci->time_registered,
	    ci->last_used, ci->founder_memo_read, esc_last_topic,
	    esc_last_topic_setter, ci->last_topic_time, ci->flags,
	    ci->mlock_on, ci->mlock_off, ci->mlock_limit, esc_mlock_key,
	    esc_entry_message, ci->last_limit_time, ci->limit_offset,
	    ci->limit_tolerance, ci->limit_period, ci->bantime,
	    esc_last_sendpass_pass, esc_last_sendpass_salt,
	    ci->last_sendpass_time);

	mysql_free_result(result);

	channel_id = mysql_insert_id(mysqlconn);

	free(esc_last_sendpass_salt);
	free(esc_last_sendpass_pass);
	free(esc_entry_message);
	free(esc_mlock_key);
	free(esc_last_topic_setter);
	free(esc_last_topic);
	free(esc_email);
	free(esc_url);
	free(esc_desc);
	free(esc_name);

	return(channel_id);
}

static void db_set_level(unsigned int channel_id, int what, int level)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_query(mysqlconn, &fields, &rows,
	    "REPLACE INTO chanlevel (channel_id, what, level) "
	    "VALUES (%u, %d, %d)", channel_id, what, level);
	mysql_free_result(result);
}

static void db_add_access(unsigned int channel_id, unsigned int idx,
    ChanAccess *access)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_added_by, *esc_modified_by;

	esc_added_by = smysql_escape_string(mysqlconn, access->added_by);
	esc_modified_by = smysql_escape_string(mysqlconn, access->modified_by);

	result = smysql_query(mysqlconn, &fields, &rows,
	    "INSERT INTO chanaccess (chanaccess_id, channel_id, idx, "
	    "level, nick_id, added, last_modified, last_used, "
	    "memo_read, added_by, modified_by) VALUES ('NULL', %u, %u, "
	    "%d, %u, %lu, %lu, %lu, %lu, '%s', '%s')", channel_id, idx,
	    access->level, access->ni->nick_id, access->added,
	    access->last_modified, access->last_used, access->memo_read,
	    esc_added_by, esc_modified_by);
	mysql_free_result(result);

	free(esc_modified_by);
	free(esc_added_by);

	return;
}

static void db_add_akick(unsigned int channel_id, unsigned int idx,
    AutoKick *akick)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_mask, *esc_reason, *esc_who;

	if (akick->reason) {
		esc_reason = smysql_escape_string(mysqlconn,
		    akick->reason);
	} else {
		esc_reason = NULL;
	}

	esc_who = smysql_escape_string(mysqlconn, akick->who);

	if (akick->is_nick) {
		esc_mask = NULL;
		
		if (esc_reason) {
			result = smysql_query(mysqlconn, &fields, &rows,
			    "INSERT INTO akick (akick_id, channel_id, "
			    "idx, nick_id, reason, who, added, last_used) "
			    "VALUES ('NULL', %u, %u, %u, '%s', '%s', %lu, "
			    "%lu)", channel_id, idx, akick->u.ni->nick_id,
			    esc_reason, esc_who, akick->added,
			    akick->last_used);
		} else {
			result = smysql_query(mysqlconn, &fields, &rows,
			    "INSERT INTO akick (akick_id, channel_id, "
			    "idx, nick_id, reason, who, added, last_used) "
			    "VALUES ('NULL', %u, %u, %u, NULL, '%s', %lu, "
			    "%lu)", channel_id, idx, akick->u.ni->nick_id,
			    esc_who, akick->added, akick->last_used);
		}
	} else {
		esc_mask = smysql_escape_string(mysqlconn, akick->u.mask);

		if (esc_reason) {
			result = smysql_query(mysqlconn, &fields, &rows,
			    "INSERT INTO akick (akick_id, channel_id, idx, "
			    "mask, reason, who, added, last_used) VALUES "
			    "('NULL', %u, %u, '%s', '%s', '%s', %lu, %lu)",
			    channel_id, idx, akick->u.mask, esc_reason,
			    esc_who, akick->added, akick->last_used);
		} else {
			result = smysql_query(mysqlconn, &fields, &rows,
			    "INSERT INTO akick (akick_id, channel_id, idx, "
			    "mask, reason, who, added, last_used) VALUES "
			    "('NULL', %u, %u, '%s', NULL, '%s', %lu, %lu)",
			    channel_id, idx, akick->u.mask, esc_reason,
			    esc_who, akick->added, akick->last_used);
		}
	}

	mysql_free_result(result);

	free(esc_who);
	free(esc_reason);

	if (esc_mask)
		free(esc_mask);

	return;
}
