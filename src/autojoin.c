
/*
 * NickServ AUTOJOIN functions.
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed IRC Network
 *      E-mail: services@lists.blitzed.org
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include "services.h"
#include "pseudo.h"

RCSID("$Id$");

static unsigned int count_autojoins(unsigned int nick_id);

/*************************************************************************/


/*
 * Manipulate a user's AutoJoin list.
 */
void do_autojoin(User *u)
{
	char base[BUFSIZE];
	MYSQL_ROW row;
	unsigned int nick_id, fields, rows, channel_id, ajcount, err;
	int sent_header;
	char *cmd, *channel, *esc_channel, *query;
	MYSQL_RES *result;

	cmd = strtok(NULL, " ");
	channel = strtok(NULL, " ");

	if (!cmd || (stricmp(cmd, "ADD") == 0 && !channel) ||
	    (stricmp(cmd, "DEL") == 0 && !channel)) {
		syntax_error(s_NickServ, u, "AUTOJOIN",
		    NICK_AUTOJOIN_SYNTAX);
	} else if (!(nick_id = u->nick_id)) {
		notice_lang(s_NickServ, u, NICK_NOT_REGISTERED);
	} else if (!nick_identified(u)) {
		notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED,
		    s_NickServ);
	} else if (stricmp(cmd, "LIST") == 0) {
		if (channel &&
		    strspn(channel, "1234567890,-") == strlen(channel)) {
			/*
			 * We have been given a list or range of indices
			 * of existing channel access to match.
			 */
			snprintf(base, sizeof(base),
			    "SELECT autojoin.idx, channel.name "
			    "FROM autojoin, channel "
			    "WHERE autojoin.nick_id=%u && "
			    "autojoin.channel_id=channel.channel_id && (",
			    nick_id);

			/*
			 * Build a query based on the above base query and
			 * matching the range or list of indices against
			 * the quarantine index, order by the index.
			 */
			query = mysql_build_query(mysqlconn, channel, base,
			    "autojoin.idx", NSAutoJoinMax);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s) ORDER BY autojoin.idx ASC", query);
			free(query);
		} else if (channel) {
			/* It is a wildcard channel match. */
			esc_channel = smysql_escape_string(mysqlconn,
			    channel);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT autojoin.idx, channel.name "
			    "FROM autojoin, channel "
			    "WHERE autojoin.nick_id=%u && "
			    "autojoin.channel_id=channel.channel_id && "
			    "IRC_MATCH('%s', channel.name) "
			    "ORDER by autojoin.idx ASC", nick_id,
			    esc_channel);
			free(esc_channel);
		} else {
			/* They want the whole list. */
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT autojoin.idx, channel.name "
			    "FROM autojoin, channel "
			    "WHERE autojoin.nick_id=%u && "
			    "autojoin.channel_id=channel.channel_id "
			    "ORDER by autojoin.idx ASC", nick_id);
		}

		sent_header = 0;

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			if (!sent_header) {
				notice_lang(s_NickServ, u,
				    NICK_AUTOJOIN_LIST_HEADER);
				sent_header = 1;
			}

			notice_lang(s_NickServ, u,
			    NICK_AUTOJOIN_LIST_FORMAT, atoi(row[0]),
			    row[1]);
		}

		mysql_free_result(result);

		if (!sent_header) {
			notice_lang(s_NickServ, u, NICK_AUTOJOIN_NO_MATCH);
		}
	} else if (stricmp(cmd, "ADD") == 0) {
		ajcount = count_autojoins(nick_id);
		
		if (ajcount >= NSAutoJoinMax) {
			notice_lang(s_NickServ, u,
			    NICK_AUTOJOIN_REACHED_LIMIT, NSAutoJoinMax);
			return;
		}
			
		if (!(channel_id = mysql_findchan(channel))) {
			notice_lang(s_NickServ, u,
			    CHAN_X_NOT_REGISTERED, channel);
			return;
		}
		
		result = mysql_simple_query(mysqlconn, &err,
		    "INSERT INTO autojoin (autojoin_id, idx, nick_id, "
		    "channel_id) VALUES ('NULL', %u, %u, %u)", ajcount + 1,
		    nick_id, channel_id);
		mysql_free_result(result);

		switch (err) {
		case 0:
			break;
		case ER_DUP_ENTRY:
			notice_lang(s_NickServ, u,
			    NICK_AUTOJOIN_ALREADY_PRESENT, channel);
			return;
		default:
			handle_mysql_error(mysqlconn, "add_quarantine");
			return;		/* Not reached. */
		}

		notice_lang(s_NickServ, u, NICK_AUTOJOIN_ADDED, channel);

	} else if (stricmp(cmd, "DEL") == 0) {
		if (isdigit(*channel) &&
		    strspn(channel, "1234567890,-") == strlen(channel)) {
			snprintf(base, sizeof(base),
			    "DELETE FROM autojoin WHERE nick_id=%u && (",
			    nick_id);

			query = mysql_build_query(mysqlconn, channel, base,
			    "idx", NSAutoJoinMax);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s)", query);

			free(query);
		} else {
			/* Match wildcard channel. */
			esc_channel = smysql_escape_string(mysqlconn,
			    channel);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "DELETE autojoin FROM autojoin, channel "
			    "WHERE autojoin.nick_id=%u && "
			    "autojoin.channel_id=channel.channel_id && "
			    "IRC_MATCH('%s', channel.name)", nick_id,
			    esc_channel);

			free(esc_channel);
		}

		mysql_free_result(result);

		if (!rows) {
			notice_lang(s_NickServ, u,
			    NICK_AUTOJOIN_NO_MATCH);
		} else if (rows == 1) {
			notice_lang(s_NickServ, u,
			    NICK_AUTOJOIN_DELETED_ONE);
		} else {
			notice_lang(s_NickServ, u,
			    NICK_AUTOJOIN_DELETED_SEVERAL, rows);
		}

		if (rows)
			renumber_autojoins(nick_id);
	} else {
		syntax_error(s_NickServ, u, "AUTOJOIN",
		    NICK_AUTOJOIN_SYNTAX);
	}
}

/*
 * Count the number of AutoJoins for a given nick.
 */
static unsigned int count_autojoins(unsigned int nick_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows, cnt;
	MYSQL_RES *result;

	cnt = 0;
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT COUNT(autojoin_id) FROM autojoin WHERE nick_id=%u",
	    nick_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		cnt = atoi(row[0]);
	}
	
	mysql_free_result(result);

	return(cnt);
}


/*
 * Renumber a given nick's AutoJoin list.
 */
void renumber_autojoins(unsigned int nick_id)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "TRUNCATE private_tmp_autojoin");
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO private_tmp_autojoin (autojoin_id, idx, "
	    "channel_id) SELECT autojoin.autojoin_id, NULL, "
	    "autojoin.channel_id FROM autojoin, channel "
	    "WHERE autojoin.channel_id=channel.channel_id && nick_id=%u "
	    "ORDER BY channel.name", nick_id);
	mysql_free_result(result);
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "REPLACE INTO autojoin (autojoin_id, nick_id, idx, "
	    "channel_id) SELECT autojoin_id, %u, idx, channel_id "
	    "FROM private_tmp_autojoin", nick_id);
	mysql_free_result(result);
}

/*
 * Check the AutoJoins for a user to see if we need to join them to any
 * channels.  Here is where it all happens.
 */
void check_autojoins(User *u)
{
	MYSQL_ROW row;
	unsigned int fields, rows, channel_id;
	MYSQL_RES *result;
	Channel *c;

	/*
	 * Shouldn't be possible to be called by non-identified nick, but best
	 * check.
	 */
	if (!u->nick_id)
		return;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT channel.name FROM autojoin, channel, nick "
	    "WHERE autojoin.nick_id=%u && "
	    "autojoin.nick_id=nick.nick_id && nick.flags & %d && "
	    "autojoin.channel_id=channel.channel_id", u->nick_id, NI_AUTOJOIN);

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		if (is_on_chan(u->nick, row[0])) {
			/*
			 * No need to bother joining them to a channel they're
			 * already on!
			 */
			continue;
		}

		c = findchan(row[0]);

		/* Have they got enough access to /cs invite themselves? */
		if (c && (channel_id = c->channel_id) &&
		    check_access(u, channel_id, CA_INVITE)) {
			if ((c->mode & CMODE_i) || (c->mode & CMODE_k) ||
			    ((c->mode & CMODE_l) &&
			    chan_count_users(c) >= c->limit) ||
			    is_banned(u, c)) {
				if (get_chan_flags(channel_id) &
				    CI_VERBOSE) {
					opnotice(s_ChanServ, c->name,
					    "AUTOJOIN using INVITE for %s",
					    u->nick);
				}

				send_cmd(s_ChanServ, "INVITE %s %s",
				    u->nick, c->name);
			}
		}
			
		send_cmd(ServerName, "SVSJOIN %s %s", u->nick, row[0]);
	}
}
