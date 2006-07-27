
/* ChanServ ACCESS functions.
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 *     E-mail: services@lists.blitzed.org
 * Based on ircservices-4.4.8:
 * copyright (c) 1996-1999 Andrew Church.
 *     E-mail: <achurch@dragonfire.net>
 * copyright (c) 1999-2000 Andrew Kempe.
 *     E-mail: <theshadow@shadowfire.org>
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

/*************************************************************************/

#include "services.h"
#include "pseudo.h"

RCSID("$Id$");

/*************************************************************************/

static void modify_access(ChanAccess *ca, unsigned int channel_id,
    int level, User *u);
static unsigned int chanaccess_count(unsigned int channel_id);
static void insert_access(unsigned int channel_id, unsigned int nick_id,
    int level, User *u);
static int access_del_callback(User * u, unsigned int num, va_list args);
static void sort_chan_access(unsigned int channel_id);
static void do_cs_access_add(User *u, ChannelInfo *ci, const char *nick,
    const char *s);
static void do_cs_access_del(User *u, ChannelInfo *ci, const char *nick);
static void do_cs_access_list(User *u, ChannelInfo *ci, const char *nick);
static void do_cs_access_view(User *u, ChannelInfo *ci, const char *nick);

/*************************************************************************/

/*************************************************************************/

/*************************************************************************/

static int access_del_callback(User * u, unsigned int num, va_list args)
{
	char name[CHANMAX];
	char their_nick[NICKMAX];
	MYSQL_ROW row;
	unsigned int channel_id, ca_id, fields, rows;
	int uacc, their_level, akick_lev;
	int *last, *perm;
	MYSQL_RES *result;

	channel_id = va_arg(args, unsigned int);
	last = va_arg(args, int *);
	perm = va_arg(args, int *);
	uacc = va_arg(args, int);
	akick_lev = va_arg(args, int);

	if (num < 1)
		return(0);
	*last = num;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT chanaccess.chanaccess_id, chanaccess.level, nick.nick "
	    "FROM chanaccess, nick "
	    "WHERE chanaccess.nick_id=nick.nick_id && "
	    "chanaccess.channel_id=%u && chanaccess.idx=%u", channel_id,
	    num);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		ca_id = atoi(row[0]);
		their_level = atoi(row[1]);
		strscpy(their_nick, row[2], NICKMAX);
	} else {
		/* Indicates no such access entry. */
		ca_id = 0;
		their_level = ACCESS_INVALID;
		their_nick[0] = '\0';
	}

	mysql_free_result(result);

	if (!ca_id)
		return(0);

	if (uacc <= their_level ||
	    (their_level < 0 && uacc <= akick_lev)) {
		(*perm)++;
		return(0);
	}

	/* Delete it. */

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM chanaccess WHERE chanaccess_id=%u", ca_id);
	mysql_free_result(result);

	if (get_chan_flags(channel_id) & CI_VERBOSE) {
		get_chan_name(channel_id, name);
		opnotice(s_ChanServ, name,
		    "%s removed \2%s\2 (level \2%d\2) from the access list",
		    u->nick, their_nick, their_level);
	}
	return(1);
}

void do_cs_access(User * u)
{
	ChannelInfo ci;
	NickInfo ni;
	/* Is true when command is LIST, COUNT or VIEW */
	int is_list;
	/* access level of the user calling this command */
	int level, last, perm;
	unsigned int channel_id;
	char *chan, *cmd, *nick, *s;
	
	level = 0;
	ci.channel_id = 0;
	ni.nick_id = 0;
	last = -1;
	perm = 0;

	chan = strtok(NULL, " ");
	cmd = strtok(NULL, " ");
	nick = strtok(NULL, " ");
	s = strtok(NULL, " ");

	is_list = (cmd && (stricmp(cmd, "LIST") == 0 ||
	    stricmp(cmd, "COUNT") == 0 || stricmp(cmd, "VIEW") == 0));

	if (!is_list && !nick_identified(u)) {
		notice_lang(s_ChanServ, u, NICK_IDENTIFY_REQUIRED,
		    s_ChanServ);
		return;
	}
	
	/*
	 * If LIST/COUNT, we don't *require* any parameters, but we can take any.
	 * If DEL, we require a nick and no level.
	 * Else (ADD), we require a level (which implies a nick).
	 */
	if (!cmd ||
	    (is_list ? 0 :
	    (stricmp(cmd, "DEL") == 0) ? (!nick || s) : !s)) {
		syntax_error(s_ChanServ, u, "ACCESS", CHAN_ACCESS_SYNTAX);
	} else if (!(channel_id = mysql_findchan(chan))) {
	       notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (!(get_channelinfo_from_id(&ci, channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (((is_list &&
	    !check_access(u, channel_id, CA_ACCESS_LIST)) || (!is_list &&
	    !check_access(u, channel_id, CA_ACCESS_CHANGE))) &&
	    !is_services_admin(u)) {
		notice_lang(s_ChanServ, u, ACCESS_DENIED);
	} else if (stricmp(cmd, "ADD") == 0) {
		do_cs_access_add(u, &ci, nick, s);
	} else if (stricmp(cmd, "DEL") == 0) {
		do_cs_access_del(u, &ci, nick);
	} else if (stricmp(cmd, "LIST") == 0) {
		do_cs_access_list(u, &ci, nick);
	} else if (stricmp(cmd, "VIEW") == 0) {
		do_cs_access_view(u, &ci, nick);
	} else if (stricmp(cmd, "COUNT") == 0) {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_COUNT, ci.name,
		    chanaccess_count(channel_id));
	} else {
		syntax_error(s_ChanServ, u, "ACCESS", CHAN_ACCESS_SYNTAX);
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*
 * Handle user command to add someone to a channel's access list, or modify
 * existing access.
 */
static void do_cs_access_add(User *u, ChannelInfo *ci, const char *nick,
    const char *s)
{
	NickInfo ni;
	ChanAccess ca;
	int level, ulev;
	unsigned int nick_id;

	assert(u);
	assert(ci);
	assert(ci->channel_id);
	assert(nick);
	assert(s);

	ni.nick_id = 0;
	
	/* Access level they're trying to add at / modify to. */
	level = atoi(s);

	/* Their own access level. */
	ulev = get_access(u, ci->channel_id);

	if (level == 0) {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_LEVEL_NONZERO);
	} else if (level <= ACCESS_INVALID || level >= ACCESS_FOUNDER) {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_LEVEL_RANGE,
		    ACCESS_INVALID + 1, ACCESS_FOUNDER - 1);
	} else if (level >= ulev) {
		notice_lang(s_ChanServ, u, ACCESS_DENIED);
	} else if (level < 0 &&
	    !check_access(u, ci->channel_id, CA_AKICK)) {
		/*
		 * Adding someone at negative access is effectively AKICKing
		 * them, so doing so also requires CA_AKICK access.
		 */
		notice_lang(s_ChanServ, u, ACCESS_DENIED);
	} else if (!(nick_id = mysql_findnick(nick)) ||
	    !get_nickinfo_from_id(&ni, nick_id)) {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_NICKS_ONLY);
	} else if (ni.status & NS_VERBOTEN) {
		notice_lang(s_ChanServ, u, NICK_X_FORBIDDEN, nick);
	} else if (ni.flags & NI_NOOP) {
		notice_lang(s_ChanServ, u, NICK_X_NOOP, nick);
	} else if (get_chanaccess_from_nickid(&ca, nick_id, ci->channel_id)) {
		/*
		 * This nick already has access in the channel, so assume they
		 * want to modify it.
		 */
			
		/* Don't allow lowering from a level >= ulev. */
		if (ca.level >= ulev) {
			notice_lang(s_ChanServ, u, PERMISSION_DENIED);
		} else if (ca.level == level) {
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_LEVEL_UNCHANGED, ni.nick, ci->name,
			    level);
		} else {
			modify_access(&ca, ci->channel_id, level, u);
		}

		free_chanaccess(&ca);
	} else if (chanaccess_count(ci->channel_id) >= CSAccessMax) {
		/* Too many access entries already for this channel. */
		notice_lang(s_ChanServ, u, CHAN_ACCESS_REACHED_LIMIT,
		    CSAccessMax);
	} else {
		insert_access(ci->channel_id, nick_id, level, u);
	}

	if (ni.nick_id)
		free_nickinfo(&ni);
}

/*
 * Handle user command to delete one or more channel access.  "nick" may be a
 * nickname or a list of access entries.
 */
static void do_cs_access_del(User *u, ChannelInfo *ci, const char *nick)
{
	NickInfo ni;
	MYSQL_ROW row;
	int akick_lev, count, last, perm, their_level;
	unsigned int deleted, ca_id, fields, rows;
	MYSQL_RES *result;

	assert(u);
	assert(ci);
	assert(ci->channel_id);
	assert(nick);

	ni.nick_id = 0;

	/*
	 * Level required to manipulate AKICK list.  We need this later in case
	 * someone with negative access is deleted - negative access is similar
	 * to AKICK so deleting them should require AKICK access.
	 */
	akick_lev = get_level(ci->channel_id, CA_AKICK);

	/* Special case: is it a number/list?  Only do search if it isn't. */
	if (isdigit(*nick) &&
	    strspn(nick, "1234567890,-") == strlen(nick)) {

		deleted = process_numlist(nick, &count, 1, CSAccessMax - 1,
		    access_del_callback, u, ci->channel_id, &last, &perm,
		    get_access(u, ci->channel_id), akick_lev);
			
		if (!deleted) {
			if (perm) {
				notice_lang(s_ChanServ, u,
				    PERMISSION_DENIED);
			} else if (count == 1) {
				notice_lang(s_ChanServ, u,
				    CHAN_ACCESS_NO_SUCH_ENTRY, last,
				    ci->name);
			} else {
				notice_lang(s_ChanServ, u,
				    CHAN_ACCESS_NO_MATCH, ci->name);
			}
		} else if (deleted == 1) {
			sort_chan_access(ci->channel_id);
			notice_lang(s_ChanServ, u, CHAN_ACCESS_DELETED_ONE,
			    ci->name);
		} else {
			sort_chan_access(ci->channel_id);
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_DELETED_SEVERAL, deleted,
			    ci->name);
		}
	} else {
		mysql_findnickinfo(&ni, nick);

		if (!ni.nick_id) {
			notice_lang(s_ChanServ, u, NICK_X_NOT_REGISTERED,
			    nick);
			return;
		}

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT chanaccess_id, level FROM chanaccess "
		    "WHERE channel_id=%u && nick_id=%u", ci->channel_id,
		    ni.nick_id);

		if ((row = smysql_fetch_row(mysqlconn, result))) {
			ca_id = atoi(row[0]);
			their_level = atoi(row[1]);
		} else {
			/* Indicates no such access entry. */
			ca_id = 0;
			their_level = ACCESS_INVALID;
		}
		
		mysql_free_result(result);

		if (!ca_id) {
			free_nickinfo(&ni);
			notice_lang(s_ChanServ, u, CHAN_ACCESS_NOT_FOUND,
			    nick, ci->name);
			return;
		}

		if (get_access(u, ci->channel_id) <= their_level) {
			free_nickinfo(&ni);
			notice_lang(s_ChanServ, u, PERMISSION_DENIED);
			return;
		}

		/* Delete it. */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "DELETE FROM chanaccess WHERE chanaccess_id=%u", ca_id);
		mysql_free_result(result);
			
		sort_chan_access(ci->channel_id);

		if (get_chan_flags(ci->channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, ci->name, "%s removed \2%s\2 "
			    "(level \2%d\2) from the access list", u->nick,
			    ni.nick, their_level);
		}
			
		notice_lang(s_ChanServ, u, CHAN_ACCESS_DELETED, ni.nick,
		    ci->name);
		free_nickinfo(&ni);
	}
}

static void do_cs_access_list(User *u, ChannelInfo *ci, const char *nick)
{
	char base[BUFSIZE];
	MYSQL_ROW row;
	int sent_header;
	unsigned int fields, rows;
	char *query, *esc_nick;
	MYSQL_RES *result;

	assert(u);
	assert(ci);
	assert(ci->channel_id);

	if (chanaccess_count(ci->channel_id) == 0) {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_LIST_EMPTY,
		    ci->name);
		return;
	}

	if (nick && strspn(nick, "1234567890,-") == strlen(nick)) {
		/*
		 * We have been given a list or range of indices of existing
		 * channel access to match.
		 */
		snprintf(base, sizeof(base), "SELECT chanaccess.idx, "
		    "chanaccess.level, nick.nick, IF(nick.flags & %d, "
		    "NULL, nick.last_usermask) FROM chanaccess, nick "
		    "WHERE chanaccess.nick_id=nick.nick_id && "
		    "chanaccess.channel_id=%u && (", NI_HIDE_MASK,
		    ci->channel_id);

		/*
		 * Build a query based on the above base query and matching the
		 * range or list of indices against the chanaccess index for
		 * this channel, order by the index.
		 */
		query = mysql_build_query(mysqlconn, nick, base,
		    "chanaccess.idx", CSAccessMax);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "%s) ORDER BY chanaccess.idx ASC", query);
		free(query);

	} else if (nick) {
		/* It is a nick match. */
		esc_nick = smysql_escape_string(mysqlconn, nick);
			
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT chanaccess.idx, chanaccess.level, nick.nick, "
		    "IF(nick.flags & %d, NULL, nick.last_usermask) "
		    "FROM chanaccess, nick WHERE "
		    "chanaccess.nick_id=nick.nick_id && "
		    "chanaccess.channel_id=%u && nick.nick='%s' ORDER BY "
		    "chanaccess.idx ASC", NI_HIDE_MASK, ci->channel_id,
		    esc_nick);
		free(esc_nick);
	} else {
		/* They want the whole list. */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT chanaccess.idx, chanaccess.level, nick.nick, "
		    "IF(nick.flags & %d, NULL, nick.last_usermask) "
		    "FROM chanaccess, nick "
		    "WHERE chanaccess.nick_id=nick.nick_id && "
		    "chanaccess.channel_id=%u ORDER BY chanaccess.idx ASC",
		    NI_HIDE_MASK, ci->channel_id);
	}

	sent_header = 0;

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		if (!sent_header) {
			notice_lang(s_ChanServ, u, CHAN_ACCESS_LIST_HEADER,
			    ci->name);
			sent_header = 1;
		}
		
		notice_lang(s_ChanServ, u, CHAN_ACCESS_LIST_FORMAT,
		    atoi(row[0]), atoi(row[1]), row[2], row[3] ? " (" : "",
		    row[3] ? row[3] : "", row[3] ? ")" : "");
	}
		
	mysql_free_result(result);

	if (!sent_header) {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_NO_MATCH, ci->name);
	}
}

static void do_cs_access_view(User *u, ChannelInfo *ci, const char *nick)
{
	char base[BUFSIZE], buf[BUFSIZE];
	MYSQL_ROW row;
	time_t added, last_modified, last_used;
	int sent_header;
	unsigned int fields, rows;
	char *query, *esc_nick;
	MYSQL_RES *result;
	struct tm *tm;

	assert(u);
	assert(ci);
	assert(ci->channel_id);

	if (chanaccess_count(ci->channel_id) == 0) {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_LIST_EMPTY,
		    ci->name);
		return;
	}

	if (nick && strspn(nick, "1234567890,-") == strlen(nick)) {
		/*
		 * We have been given a list or range of indices of existing
		 * channel access to match.
		 */
		snprintf(base, sizeof(base), "SELECT chanaccess.idx, "
		    "chanaccess.level, nick.nick, IF(nick.flags & %d, "
		    "NULL, nick.last_usermask), chanaccess.added, "
		    "chanaccess.last_modified, chanaccess.added_by, "
		    "chanaccess.modified_by, chanaccess.last_used "
		    "FROM chanaccess, nick WHERE "
		    "chanaccess.nick_id=nick.nick_id && "
		    "chanaccess.channel_id=%u && (", NI_HIDE_MASK,
		    ci->channel_id);

		/*
		 * Build a query based on the above base query and matching the
		 * range or list of indices against the chanaccess index for
		 * this channel, order by the index.
		 */
		query = mysql_build_query(mysqlconn, nick, base,
		    "chanaccess.idx", CSAccessMax);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "%s) ORDER BY chanaccess.idx ASC", query);
		free(query);
	} else if (nick) {
		/* It is a nick match. */
		esc_nick = smysql_escape_string(mysqlconn, nick);
			
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT chanaccess.idx, chanaccess.level, nick.nick, "
		    "IF(nick.flags & %d, NULL, nick.last_usermask), "
		    "chanaccess.added, chanaccess.last_modified, "
		    "chanaccess.added_by, chanaccess.modified_by, "
		    "chanaccess.last_used FROM chanaccess, nick WHERE "
		    "chanaccess.nick_id=nick.nick_id && "
		    "chanaccess.channel_id=%u && nick.nick='%s' "
		    "ORDER BY chanaccess.idx ASC", NI_HIDE_MASK,
		    ci->channel_id, esc_nick);
		free(esc_nick);
	} else {
		/* They want the whole list. */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT chanaccess.idx, chanaccess.level, nick.nick, "
		    "IF(nick.flags & %d, NULL, nick.last_usermask), "
		    "chanaccess.added, chanaccess.last_modified, "
		    "chanaccess.added_by, chanaccess.modified_by, "
		    "chanaccess.last_used FROM chanaccess, nick WHERE "
		    "chanaccess.nick_id=nick.nick_id && "
		    "chanaccess.channel_id=%u ORDER BY chanaccess.idx ASC",
		    NI_HIDE_MASK, ci->channel_id);
	}

	sent_header = 0;

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		if (!sent_header) {
			notice_lang(s_ChanServ, u, CHAN_ACCESS_VIEW_HEADER,
			    ci->name);
			sent_header = 1;
		}

		notice_lang(s_ChanServ, u, CHAN_ACCESS_VIEW_FORMAT1,
		    atoi(row[0]), atoi(row[1]), row[2], row[3] ? " (" : "",
		    row[3] ? row[3] : "", row[3] ? ")" : "");

		added = atoi(row[4]);
		last_modified = atoi(row[5]);
		last_used = atoi(row[8]);

		if (added) {
			tm = localtime(&added);
			strftime_lang(buf, sizeof(buf), u,
			    STRFTIME_DATE_TIME_FORMAT, tm);
			
			if (last_modified) {
				notice_lang(s_ChanServ, u,
				    CHAN_ACCESS_VIEW_FORMAT2, row[6],
				    row[7]);
			} else {
				notice_lang(s_ChanServ, u,
				    CHAN_ACCESS_VIEW_FORMAT2_NOMODIFY,
				    row[6]);
			}
			
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_VIEW_FORMAT3, buf, time_ago(added));
		}

		if (last_modified) {
			tm = localtime(&last_modified);
			strftime_lang(buf, sizeof(buf), u,
			    STRFTIME_DATE_TIME_FORMAT, tm);
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_VIEW_FORMAT4, buf,
			    time_ago(last_modified));
		}

		if (last_used) {
			tm = localtime(&last_used);
			strftime_lang(buf, sizeof(buf), u,
			    STRFTIME_DATE_TIME_FORMAT, tm);
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_VIEW_FORMAT5, buf,
			    time_ago(last_used));
		} else {
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_VIEW_FORMAT5_NEVER_USED);
		}
	}
		
	mysql_free_result(result);
		
	if (!sent_header) {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_NO_MATCH, ci->name);
	}
}

void do_cs_vop(User * u)
{
        ChannelInfo ci;
	NickInfo ni;
	ChanAccess ca;
	unsigned int channel_id;
	int reqlev, ulev;
        char *chan, *cmd, *nick;

	chan = strtok(NULL, " ");
	cmd = strtok(NULL, " ");
	nick = strtok(NULL, " ");
	ci.channel_id = 0;
	ni.nick_id = 0;
	reqlev = 0;

        if (!cmd || !chan || !nick) {
                syntax_error(s_ChanServ, u, "VOP", CHAN_VOP_SYNTAX);
        } else if (!(channel_id = mysql_findchan(chan))) {
                notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (!(get_channelinfo_from_id(&ci, channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
        } else if (get_chan_flags(channel_id) & CI_VERBOTEN) {
                notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
        } else if ((!check_access(u, channel_id, CA_ACCESS_CHANGE)) &&
	    !is_services_admin(u)) {
                notice_lang(s_ChanServ, u, ACCESS_DENIED);

        } else if (stricmp(cmd, "ADD") == 0) {
                reqlev = get_chan_level(channel_id, CA_AUTOVOICE);

                if (reqlev <= 0) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_VOP_DISABLED);
			return;
		}

                ulev = get_access(u, channel_id);
		
                if (reqlev >= ulev) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, PERMISSION_DENIED);
			return;
		}
		
		if (reqlev == 0) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_LEVEL_NONZERO);
                        return;
		} else if (reqlev <= ACCESS_INVALID ||
		    reqlev >= ACCESS_FOUNDER) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_ACCESS_LEVEL_RANGE,
			    ACCESS_INVALID + 1, ACCESS_FOUNDER - 1);
                        return;
                }

		mysql_findnickinfo(&ni, nick);

		if (!ni.nick_id) { 
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_ACCESS_NICKS_ONLY);
			return;
		} else if (ni.status & NS_VERBOTEN) {
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, NICK_X_FORBIDDEN, nick);
			return;
		} else if (ni.flags & NI_NOOP) {
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, NICK_X_NOOP, nick);
			return;
		}
		
		if (get_chanaccess_from_nickid(&ca, ni.nick_id,
		    channel_id)) {
			/*
			 * Access already exists, don't allow lowering from
			 * a level >= ulev
			 */
			if (ca.level >= ulev) {
				notice_lang(s_ChanServ, u,
				    PERMISSION_DENIED);
				return;
			} else if (ca.level == reqlev) {
				notice_lang(s_ChanServ, u,
				    CHAN_ACCESS_LEVEL_UNCHANGED, ni.nick,
				    ci.name, reqlev);
			} else {
				modify_access(&ca, channel_id, reqlev, u);
			}
			free_chanaccess(&ca);
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			return;
		}

		if (chanaccess_count(channel_id) >=
		    (unsigned int)CSAccessMax) {
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_REACHED_LIMIT, CSAccessMax);
		} else {
			insert_access(channel_id, ni.nick_id, reqlev, u);
		}

		free_nickinfo(&ni);
        } else {
                syntax_error(s_ChanServ, u, "VOP", CHAN_VOP_SYNTAX);
        }

	if (ci.channel_id)
		free_channelinfo(&ci);
}

void do_cs_aop(User * u)
{
        ChannelInfo ci;
	NickInfo ni;
	ChanAccess ca;
	unsigned int channel_id;
	int reqlev, ulev;
        char *chan, *cmd, *nick;

	chan = strtok(NULL, " ");
	cmd = strtok(NULL, " ");
	nick = strtok(NULL, " ");
	ci.channel_id = 0;
	ni.nick_id = 0;
	reqlev = 0;

        if (!cmd || !chan || !nick) {
                syntax_error(s_ChanServ, u, "AOP", CHAN_AOP_SYNTAX);
        } else if (!(channel_id = mysql_findchan(chan))) {
                notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (!(get_channelinfo_from_id(&ci, channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
        } else if (get_chan_flags(channel_id) & CI_VERBOTEN) {
                notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
        } else if ((!check_access(u, channel_id, CA_ACCESS_CHANGE)) &&
	    !is_services_admin(u)) {
                notice_lang(s_ChanServ, u, ACCESS_DENIED);

        } else if (stricmp(cmd, "ADD") == 0) {
                reqlev = get_chan_level(channel_id, CA_AUTOOP);

                if (reqlev <= 0) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_AOP_DISABLED);
			return;
		}

                ulev = get_access(u, channel_id);
		
                if (reqlev >= ulev) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, PERMISSION_DENIED);
			return;
		}
		
		if (reqlev == 0) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_LEVEL_NONZERO);
                        return;
		} else if (reqlev <= ACCESS_INVALID ||
		    reqlev >= ACCESS_FOUNDER) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_ACCESS_LEVEL_RANGE,
			    ACCESS_INVALID + 1, ACCESS_FOUNDER - 1);
                        return;
                }

		mysql_findnickinfo(&ni, nick);

		if (!ni.nick_id) { 
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_ACCESS_NICKS_ONLY);
			return;
		} else if (ni.status & NS_VERBOTEN) {
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, NICK_X_FORBIDDEN, nick);
			return;
		} else if (ni.flags & NI_NOOP) {
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, NICK_X_NOOP, nick);
			return;
		}
		
		if (get_chanaccess_from_nickid(&ca, ni.nick_id,
		    channel_id)) {
			/*
			 * Access already exists, don't allow lowering from
			 * a level >= ulev
			 */
			if (ca.level >= ulev) {
				notice_lang(s_ChanServ, u,
				    PERMISSION_DENIED);
				return;
			} else if (ca.level == reqlev) {
				notice_lang(s_ChanServ, u,
				    CHAN_ACCESS_LEVEL_UNCHANGED, ni.nick,
				    ci.name, reqlev);
			} else {
				modify_access(&ca, channel_id, reqlev, u);
			}
			free_chanaccess(&ca);
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			return;
		}

		if (chanaccess_count(channel_id) >= 
		    (unsigned int)CSAccessMax) {
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_REACHED_LIMIT, CSAccessMax);
		} else {
			insert_access(channel_id, ni.nick_id, reqlev, u);
		}

		free_nickinfo(&ni);
        } else {
                syntax_error(s_ChanServ, u, "AOP", CHAN_AOP_SYNTAX);
        }

	if (ci.channel_id)
		free_channelinfo(&ci);
}

void do_cs_sop(User * u)
{
        ChannelInfo ci;
	NickInfo ni;
	ChanAccess ca;
	unsigned int channel_id;
	int reqlev, ulev;
        char *chan, *cmd, *nick;

	chan = strtok(NULL, " ");
	cmd = strtok(NULL, " ");
	nick = strtok(NULL, " ");
	ci.channel_id = 0;
	ni.nick_id = 0;
	reqlev = 0;

        if (!cmd || !chan || !nick) {
                syntax_error(s_ChanServ, u, "VOP", CHAN_SOP_SYNTAX);
        } else if (!(channel_id = mysql_findchan(chan))) {
                notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (!(get_channelinfo_from_id(&ci, channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
        } else if (get_chan_flags(channel_id) & CI_VERBOTEN) {
                notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
        } else if ((!check_access(u, channel_id, CA_ACCESS_CHANGE)) &&
	    !is_services_admin(u)) {
                notice_lang(s_ChanServ, u, ACCESS_DENIED);

        } else if (stricmp(cmd, "ADD") == 0) {
                reqlev = get_chan_level(channel_id, CA_AKICK);

                if (reqlev <= 0) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_SOP_DISABLED);
			return;
		}

                ulev = get_access(u, channel_id);
		
                if (reqlev >= ulev) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, PERMISSION_DENIED);
			return;
		}
		
		if (reqlev == 0) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_LEVEL_NONZERO);
                        return;
		} else if (reqlev <= ACCESS_INVALID ||
		    reqlev >= ACCESS_FOUNDER) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_ACCESS_LEVEL_RANGE,
			    ACCESS_INVALID + 1, ACCESS_FOUNDER - 1);
                        return;
                }

		mysql_findnickinfo(&ni, nick);

		if (!ni.nick_id) { 
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_ACCESS_NICKS_ONLY);
			return;
		} else if (ni.status & NS_VERBOTEN) {
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, NICK_X_FORBIDDEN, nick);
			return;
		} else if (ni.flags & NI_NOOP) {
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, NICK_X_NOOP, nick);
			return;
		}
		
		if (get_chanaccess_from_nickid(&ca, ni.nick_id,
		    channel_id)) {
			/*
			 * Access already exists, don't allow lowering from
			 * a level >= ulev
			 */
			if (ca.level >= ulev) {
				notice_lang(s_ChanServ, u,
				    PERMISSION_DENIED);
				return;
			} else if (ca.level == reqlev) {
				notice_lang(s_ChanServ, u,
				    CHAN_ACCESS_LEVEL_UNCHANGED, ni.nick,
				    ci.name, reqlev);
			} else {
				modify_access(&ca, channel_id, reqlev, u);
			}
			free_chanaccess(&ca);
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			return;
		}

		if (chanaccess_count(channel_id) >=
		    (unsigned int)CSAccessMax) {
			notice_lang(s_ChanServ, u,
			    CHAN_ACCESS_REACHED_LIMIT, CSAccessMax);
		} else {
			insert_access(channel_id, ni.nick_id, reqlev, u);
		}

		free_nickinfo(&ni);
        } else {
                syntax_error(s_ChanServ, u, "SOP", CHAN_SOP_SYNTAX);
        }

	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*
 * Modify an existing row of the chanaccess table.
 */
static void modify_access(ChanAccess *ca, unsigned int channel_id, int level,
    User *u)
{
	char name[CHANMAX];
	time_t now;
	int oldlevel;
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_nick, *their_nick;

	get_chan_name(channel_id, name);	
	now = time(NULL);
	oldlevel = ca->level;
	esc_nick = smysql_escape_string(mysqlconn, u->nick);
	their_nick = get_nick_from_id(ca->nick_id);

	if (ca->added_by && ca->added_by[0]) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE chanaccess SET level=%d, last_modified=%d, "
		    "modified_by='%s' WHERE chanaccess_id=%u && "
		    "channel_id=%u", level, now, esc_nick,
		    ca->chanaccess_id, channel_id);
	} else {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE chanaccess SET level=%d, added=%d, "
		    "last_modified=%d, added_by='%s', modified_by='%s' "
		    "WHERE chanaccess_id=%u && channel_id=%u", level, now,
		    now, esc_nick, esc_nick, ca->chanaccess_id, channel_id);
	}
	
	mysql_free_result(result);
	free(esc_nick);

	if (get_chan_flags(channel_id) & CI_VERBOSE) {
		opnotice(s_ChanServ, name, "%s changed the access level "
		    "for \2%s\2 from \2%d\2 to \2%d\2", u->nick,
		    their_nick, oldlevel, level);
	
	}
	
	notice_lang(s_ChanServ, u, CHAN_ACCESS_LEVEL_CHANGED, their_nick,
	    name, level);

	free(their_nick);
	
	sort_chan_access(channel_id);
}

/*
 * Count the number of access entries for a given channel.
 */
static unsigned int chanaccess_count(unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	unsigned int count;
	MYSQL_RES *result;
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT COUNT(chanaccess_id) FROM chanaccess "
	    "WHERE channel_id=%u", channel_id);
	row = smysql_fetch_row(mysqlconn, result);
	count = atoi(row[0]);
	mysql_free_result(result);

	return(count);
}

/*
 * Insert a new row into the chanaccess table.
 */
static void insert_access(unsigned int channel_id, unsigned int nick_id,
    int level, User *u)
{
	char name[CHANMAX];
	time_t now;
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_nick, *their_nick;

	get_chan_name(channel_id, name);
	now = time(NULL);
	esc_nick = smysql_escape_string(mysqlconn, u->nick);
	their_nick = get_nick_from_id(nick_id);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO chanaccess (chanaccess_id, channel_id, level, "
	    "nick_id, added, last_modified, last_used, memo_read, "
	    "added_by, modified_by) VALUES "
	    "(NULL, %u, %d, %u, %d, %d, 0, 0, '%s', '%s')", channel_id,
	    level, nick_id, now, now, esc_nick, esc_nick);
	mysql_free_result(result);
	free(esc_nick);

	if (get_chan_flags(channel_id) & CI_VERBOSE) {
		opnotice(s_ChanServ, name, "%s added \2%s\2 to the access "
		    "list (level \2%d\2)", u->nick, their_nick, level);
	}

	notice_lang(s_ChanServ, u, CHAN_ACCESS_ADDED, their_nick, name,
	    level);
	free(their_nick);
	
	sort_chan_access(channel_id);
}

/*
 * Renumber the idx column of a given channel's access entries.
 */
static void sort_chan_access(unsigned int channel_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "TRUNCATE private_tmp_chanaccess");
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO private_tmp_chanaccess (chanaccess_id, idx, "
	    "level, nick_id, added, last_modified, last_used, memo_read, "
	    "added_by, modified_by) SELECT chanaccess_id, NULL, level, "
	    "nick_id, added, last_modified, last_used, memo_read, "
	    "added_by, modified_by FROM chanaccess WHERE channel_id=%u "
	    "ORDER BY level DESC", channel_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "REPLACE INTO chanaccess (chanaccess_id, channel_id, idx, "
	    "level, nick_id, added, last_modified, last_used, memo_read, "
	    "added_by, modified_by) SELECT chanaccess_id, %u, idx, level, "
	    "nick_id, added, last_modified, last_used, memo_read, "
	    "added_by, modified_by FROM private_tmp_chanaccess",
	    channel_id);
	mysql_free_result(result);
}

