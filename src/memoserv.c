/*!
 * \file memoserv.c
 * \brief MemoServ functions.
 */

/*
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 *     E-mail: services@lists.blitzed.org
 * Based on ircservices-4.4.8:
 * copyright (c) 1996-1999 Andrew Church
 *     E-mail: <achurch@dragonfire.net>
 * copyright (c) 1999-2000 Andrew Kempe.
 *     E-mail: <theshadow@shadowfire.org>
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include "services.h"
#include "pseudo.h"

RCSID("$Id$");

#define MAX(a, b)	(a) > (b) ? (a) : (b)

/*************************************************************************/

static void do_help(User *u);
static void do_send(User *u);
static void do_globalsend(User *u);
static void do_opersend(User *u);
static void do_csopsend(User *u);
static void do_list(User *u);
static void do_read(User *u);
static void do_del(User *u);
static void do_set(User *u);
static void do_set_notify(User *u, char *param);
static void do_set_limit(User *u, char *param);
static void do_info(User *u);

static unsigned int count_memos(const char *owner);
static unsigned int count_new_memos(const char *owner);
static unsigned int count_memos_with_flags(const char *owner, int flags);
static unsigned int get_memoinfo_from_owner(MemoInfo *mi,
    const char *owner, int *ischan, int *isverboten);
static void free_memoinfo(MemoInfo *mi);
static void renumber_memos(const char *owner);
static unsigned int get_memo_idx(unsigned int memo_id);
static void mark_memo_read(unsigned int memo_id);
static void set_memo_limit(unsigned int memoinfo_id, int max);
static void send_memo(User *u, const char *name, const char *source,
    char *text);

/*************************************************************************/

static Command cmds[] = {
	{"HELP", do_help, NULL, -1, -1, -1, -1, -1, NULL, NULL, NULL, NULL},
	{"SEND", do_send, NULL, MEMO_HELP_SEND, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	{"GLOBALSEND", do_globalsend, is_services_admin, -1, -1,
	    MEMO_SERVADMIN_HELP_GLOBALSEND, MEMO_SERVADMIN_HELP_GLOBALSEND,
	    MEMO_SERVADMIN_HELP_GLOBALSEND, NULL, NULL, NULL, NULL},
	{"OPERSEND", do_opersend, is_services_admin, -1, -1,
	    MEMO_SERVADMIN_HELP_OPERSEND, MEMO_SERVADMIN_HELP_OPERSEND,
	    MEMO_SERVADMIN_HELP_OPERSEND, NULL, NULL, NULL, NULL},
	{"CSOPSEND", do_csopsend, is_services_admin, -1, -1,
	    MEMO_SERVADMIN_HELP_CSOPSEND, MEMO_SERVADMIN_HELP_CSOPSEND,
	    MEMO_SERVADMIN_HELP_CSOPSEND, NULL, NULL, NULL, NULL},
	{"LIST", do_list, NULL, MEMO_HELP_LIST, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	{"READ", do_read, NULL, MEMO_HELP_READ, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	{"DEL", do_del, NULL, MEMO_HELP_DEL, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	{"SET", do_set, NULL, MEMO_HELP_SET, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	{"SET NOTIFY", NULL, NULL, MEMO_HELP_SET_NOTIFY, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET LIMIT", NULL, NULL, -1, MEMO_HELP_SET_LIMIT,
	    MEMO_SERVADMIN_HELP_SET_LIMIT, MEMO_SERVADMIN_HELP_SET_LIMIT,
	    MEMO_SERVADMIN_HELP_SET_LIMIT, NULL, NULL, NULL, NULL},
	{"INFO", do_info, NULL, -1, MEMO_HELP_INFO,
	    MEMO_SERVADMIN_HELP_INFO, MEMO_SERVADMIN_HELP_INFO,
	    MEMO_SERVADMIN_HELP_INFO, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, -1, -1, -1, -1, -1, NULL, NULL, NULL, NULL}
};

/*************************************************************************/

/*************************************************************************/

/* MemoServ initialization. */

void ms_init(void)
{
	Command *cmd;

	cmd = lookup_cmd(cmds, "SET LIMIT");
	if (cmd)
		cmd->help_param1 = (char *)(long)MSMaxMemos;
}

/*************************************************************************/

/*
 * memoserv:  Main MemoServ routine.
 *            Note that the User structure passed to the do_* routines will
 *            always be valid (non-NULL) and will always have a valid
 *            u->nick_id
 */

void memoserv(const char *source, char *buf)
{
	char *cmd, *s;
	User *u = finduser(source);

	if (!u) {
		log("%s: user record for %s not found", s_MemoServ, source);
		notice(s_MemoServ, source,
		    getstring(0, USER_RECORD_NOT_FOUND));
		return;
	}

	cmd = strtok(buf, " ");
	if (!cmd) {
		return;
	} else if (stricmp(cmd, "\1PING") == 0) {
		if (!(s = strtok(NULL, "")))
			s = "\1";
		notice(s_MemoServ, source, "\1PING %s", s);
	} else if (stricmp(cmd, "\1VERSION\1") == 0) {
		notice(s_MemoServ, source, "\1VERSION Blitzed-Based OFTC IRC Services v%s\1",
		    VERSION);
	} else {
		if (!u->nick_id && stricmp(cmd, "HELP") != 0) {
			notice_lang(s_MemoServ, u, NICK_NOT_REGISTERED_HELP,
			    s_NickServ);
		} else {
			run_cmd(s_MemoServ, u, cmds, cmd);
		}
	}
}

/*************************************************************************/

/* Return memo stats.  Assumes pointers are valid. */

void get_memoserv_stats(long *nrec, long *memuse)
{
	get_table_stats(mysqlconn, "memo", (unsigned int *)nrec,
			(unsigned int *)memuse);
	if (*memuse)
		*memuse /= 1024;
}

/*************************************************************************/

/*
 * check_memos:  See if the given user has any unread memos, and send a NOTICE
 * to that user if so (and if the appropriate flag is set).
 */

void check_memos(User * u)
{
	MemoInfo mi;
	MYSQL_ROW row;
	unsigned int cnt, newcnt, fields, rows;
	int flags, ischan, isverboten;
	char *nick, *esc_owner;
	MYSQL_RES *result;

	newcnt = 0;
	flags = 0;
	mi.memoinfo_id = 0;

	if (!u->real_id || !nick_recognized(u) ||
	    !(get_nick_flags(u->real_id) & NI_MEMO_SIGNON))
		return;

	nick = get_nick_from_id(u->real_id);
	get_memoinfo_from_owner(&mi, nick, &ischan, &isverboten);

	cnt = count_memos(nick);
	newcnt = count_new_memos(nick);

	if (newcnt > 0) {
		notice_lang(s_MemoServ, u,
		    newcnt == 1 ? MEMO_HAVE_NEW_MEMO : MEMO_HAVE_NEW_MEMOS,
		    newcnt);

		if (newcnt == 1) {
			/*
			 * Check their last memo to see if it is the
			 * unread one.
			 */
			esc_owner = smysql_escape_string(mysqlconn, nick);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT flags FROM memo "
			    "WHERE owner='%s' ORDER BY idx DESC LIMIT 1",
			    esc_owner);
			
			if (rows) {
				row = smysql_fetch_row(mysqlconn, result);
				flags = atoi(row[0]);
			}

			mysql_free_result(result);

			if (flags & MF_UNREAD) {
				notice_lang(s_MemoServ, u,
				    MEMO_TYPE_READ_LAST, s_MemoServ);
			} else {
				/*
				 * Only one new one, but it isn't the last,
				 * so let's find it.
				 */
				result = smysql_bulk_query(mysqlconn,
				    &fields, &rows, "SELECT idx FROM memo "
				    "WHERE owner='%s' && (flags & %d) "
				    "ORDER BY idx DESC LIMIT 1", esc_owner,
				    MF_UNREAD);

				if (rows) {
					row = smysql_fetch_row(mysqlconn,
					    result);
					notice_lang(s_MemoServ, u,
					    MEMO_TYPE_READ_NUM, s_MemoServ,
					    atoi(row[0]));
				}

				mysql_free_result(result);
			}
			free(esc_owner);
		} else {
			notice_lang(s_MemoServ, u, MEMO_TYPE_LIST_NEW,
			    s_MemoServ);
		}
	}

	if (mi.max > 0 && (int)cnt >= mi.max) {
		if ((int)cnt > mi.max)
			notice_lang(s_MemoServ, u, MEMO_OVER_LIMIT, mi.max);
		else
			notice_lang(s_MemoServ, u, MEMO_AT_LIMIT, mi.max);
	}

	if (mi.memoinfo_id)
		free_memoinfo(&mi);
}

/*************************************************************************/

/*********************** MemoServ private routines ***********************/

/*************************************************************************/

/*************************************************************************/

/*
 * Return the number of channel memos that are newer than the last read time.
 */

int new_chan_memos(unsigned int channel_id, time_t last_read)
{
	MYSQL_ROW row;
	unsigned int fields, rows, count;
	MYSQL_RES *result;

	count = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT COUNT(memo.memo_id) FROM memo, channel "
	    "WHERE memo.owner=channel.name && channel.channel_id=%u && "
	    "memo.time > %d", channel_id, last_read);

	row = smysql_fetch_row(mysqlconn, result);
	count = atoi(row[0]);

	mysql_free_result(result);
	return(count);
}

/*************************************************************************/

/*********************** MemoServ command routines ***********************/

/*************************************************************************/

/* Return a help message. */

static void do_help(User * u)
{
	char *cmd = strtok(NULL, "");

	notice_help(s_MemoServ, u, MEMO_HELP_START);

	if (!cmd) {
		if (is_services_oper(u)) {
			notice_help(s_MemoServ, u,
				    MEMO_SERVADMIN_HELP,
				    s_ChanServ);
		} else {
			notice_help(s_MemoServ, u, MEMO_HELP,
				    s_ChanServ);
		}
	} else {
		help_cmd(s_MemoServ, u, cmds, cmd);
	}

	notice_help(s_MemoServ, u, MEMO_HELP_END);
}

/*************************************************************************/

/* Send a memo to a nick/channel. */

static void do_send(User * u)
{
	MemoInfo mi;
	unsigned int channel_id, nick_id;
	int ischan, isverboten, is_servadmin;
	char *name, *text, *source;
	char *master_nick;
	time_t now;

	source = u->nick;
	name = strtok(NULL, " ");
	text = strtok(NULL, "");
	now = time(NULL);
	is_servadmin = is_services_admin(u);
	mi.memoinfo_id = 0;
	master_nick = NULL;

	if (name && !strchr(name, '#')) {
		/*
		 * It isn't a channel, so it must be a nick.  We want to get
		 * the master nick.
		 */
		nick_id = mysql_findnick(name);
		nick_id = mysql_getlink(nick_id);

		if (nick_id) {
			master_nick = get_nick_from_id(nick_id);
			name = master_nick;
		}
	}

	if (!text) {
		syntax_error(s_MemoServ, u, "SEND", MEMO_SEND_SYNTAX);

	} else if (!nick_recognized(u)) {
		notice_lang(s_MemoServ, u, NICK_IDENTIFY_REQUIRED,
		    s_NickServ);

	} else if (!(get_memoinfo_from_owner(&mi, name, &ischan,
	    &isverboten))) {
		if (isverboten) {
			notice_lang(s_MemoServ, u,
			    ischan ? CHAN_X_FORBIDDEN : NICK_X_FORBIDDEN,
			    name);
		} else {
			notice_lang(s_MemoServ, u, ischan ?
			    CHAN_X_NOT_REGISTERED : NICK_X_NOT_REGISTERED,
			    name);
		}

	} else if (MSSendDelay > 0 && u && u->lastmemosend + MSSendDelay >
	    now && !is_servadmin) {
		u->lastmemosend = now;
		notice_lang(s_MemoServ, u, MEMO_SEND_PLEASE_WAIT,
		    MSSendDelay);

	} else if (mi.max == 0 && !is_servadmin) {
		notice_lang(s_MemoServ, u, MEMO_X_GETS_NO_MEMOS, name);

	} else if (mi.max > 0 && (int)count_memos(name) >= mi.max &&
	    !is_servadmin) {
		notice_lang(s_MemoServ, u, MEMO_X_HAS_TOO_MANY_MEMOS, name);

	} else {

		if (ischan && (channel_id = mysql_findchan(name))) {
			if (!check_access(u, channel_id, CA_MEMO_SEND)) {
				notice_lang(s_MemoServ, u, ACCESS_DENIED);
				free_memoinfo(&mi);
				if (master_nick)
					free(master_nick);
				return;
			}
		}

		u->lastmemosend = now;

		send_memo(u, name, source, text);
	}
	if (mi.memoinfo_id)
		free_memoinfo(&mi);
	if (master_nick)
		free(master_nick);
}

/*************************************************************************/

/* Send a memo to every nick */

static void do_globalsend(User *u)
{
	MYSQL_ROW row;
	time_t now;
	unsigned int fields, rows, nick_id, idx;
	char *text, *esc_sender, *esc_text, *orig;
	MYSQL_RES *result;
	User *u2;

	text = strtok(NULL, "");

	if (!text) {
		syntax_error(s_MemoServ, u, "GLOBALSEND",
		    MEMO_GLOBALSEND_SERVADMIN_SYNTAX);
		return;
	} else if (!nick_recognized(u)) {
		notice_lang(s_MemoServ, u, NICK_IDENTIFY_REQUIRED,
		    s_NickServ);
		return;
	}

	orig = sstrdup(text);
	
	now = time(NULL);
	esc_sender = smysql_escape_string(mysqlconn, s_GlobalNoticer);
	esc_text = smysql_escape_string(mysqlconn, MSROT13 ? rot13(text) :
	    text);

	u->lastmemosend = now;
	
	/*
	 * We can't do this without a temporary table. :(
	 * Nuke our temp table.
	 */

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "TRUNCATE private_tmp_memo");
	mysql_free_result(result);

	/*
	 * Now insert a new memo for every single master nick, into the
	 * temporary table.  We need to check against the memo table to see
	 * what the next index is for those people who already have memos.
	 */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO private_tmp_memo "
	    "(memo_id, owner, idx, flags, time, sender, text, rot13) "
	    "SELECT NULL, memoinfo.owner, IFNULL(MAX(memo.idx)+1, 1), %d, "
	    "%d, '%s', '%s', '%c' FROM nick, memoinfo LEFT "
	    "JOIN memo ON memo.owner=memoinfo.owner WHERE "
	    "nick.nick=memoinfo.owner && nick.link_id=0 GROUP BY "
	    "memoinfo.owner", MF_UNREAD | MF_GLOBALSEND, now, esc_sender,
	    esc_text, MSROT13 ? 'Y' : 'N');
	mysql_free_result(result);

	free(esc_sender);
	free(esc_text);

	/* Copy over these rows into the main memo table. */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO memo "
	    "(memo_id, owner, idx, flags, time, sender, text, rot13) "
	    "SELECT NULL, owner, idx, flags, time, sender, text, rot13 "
	    "FROM private_tmp_memo");
	mysql_free_result(result);

	/* Notify anyone that needs notifying. */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT nick.nick_id, MAX(memo.idx), nick.nick "
	    "FROM nick, memoinfo LEFT JOIN memo "
	    "ON memo.owner=memoinfo.owner WHERE nick.nick=memo.owner && "
	    "(nick.flags & %d) && nick.link_id=0 GROUP BY memo.owner",
	    NI_MEMO_RECEIVE);

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		nick_id = atoi(row[0]);
		idx = atoi(row[1]);
		
		if (MSNotifyAll) {
			for (u2 = firstuser(); u2; u2 = nextuser()) {
				if (u2->real_id != nick_id)
					continue;

				notice_lang(s_MemoServ, u2,
				    MEMO_NEW_MEMO_ARRIVED, s_GlobalNoticer,
				    s_MemoServ, idx);
			}
		} else {
			u2 = finduser(row[2]);
			if (u2) {
				notice_lang(s_MemoServ, u2,
				    MEMO_NEW_MEMO_ARRIVED, s_GlobalNoticer,
				    s_MemoServ, idx);
			}
		}
	}

	mysql_free_result(result);
	
	notice_lang(s_MemoServ, u, MEMO_SERVADMIN_GLOBALSENT);
	snoop(s_MemoServ, "[GLOBALSEND] %s sent a global memo: %s", u->nick,
	    orig);
	log("memo: %s just sent a global memo: %s", u->nick, orig);
	wallops(s_MemoServ, "\2%s\2 has just sent a global memo", u->nick);
	free(orig);
}

/*************************************************************************/

/* Send a memo to every registered IRC operator */

static void do_opersend(User *u)
{
	MYSQL_ROW row;
	time_t now;
	unsigned int fields, rows, nick_id, idx;
	char *text, *esc_sender, *esc_text;
	MYSQL_RES *result;
	User *u2;

	text = strtok(NULL, "");

	if (!text) {
		syntax_error(s_MemoServ, u, "GLOBALSEND",
		    MEMO_OPERSEND_SERVADMIN_SYNTAX);
		return;
	} else if (!nick_recognized(u)) {
		notice_lang(s_MemoServ, u, NICK_IDENTIFY_REQUIRED,
		    s_NickServ);
		return;
	}

	now = time(NULL);
	esc_sender = smysql_escape_string(mysqlconn, s_GlobalNoticer);
	esc_text = smysql_escape_string(mysqlconn, MSROT13 ? rot13(text) :
	    text);

	u->lastmemosend = now;
	
	/*
	 * We can't do this without a temporary table. :(
	 * Nuke our temp table.
	 */

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "TRUNCATE private_tmp_memo");
	mysql_free_result(result);

	/*
	 * Now insert a new memo for every single master nick, into the
	 * temporary table.  We need to check against the memo table to see
	 * what the next index is for those people who already have memos.
	 */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO private_tmp_memo "
	    "(memo_id, owner, idx, flags, time, sender, text) "
	    "SELECT NULL, memoinfo.owner, IFNULL(MAX(memo.idx)+1, 1), %d, "
	    "%d, '%s', '%s', '%c' FROM nick, memoinfo LEFT "
	    "JOIN memo ON memo.owner=memoinfo.owner WHERE "
	    "nick.nick=memoinfo.owner && nick.link_id=0 && "
	    "(nick.flags & %d) GROUP BY memoinfo.owner",
	    MF_UNREAD | MF_OPERSEND, now, esc_sender, esc_text,
	    MSROT13 ? 'Y' : 'N', NI_IRCOP);
	mysql_free_result(result);

	free(esc_sender);
	free(esc_text);

	/* Copy over these rows into the main memo table. */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO memo "
	    "(memo_id, owner, idx, flags, time, sender, text, rot13) "
	    "SELECT NULL, owner, idx, flags, time, sender, text, rot13 "
	    "FROM private_tmp_memo");
	mysql_free_result(result);

	/* Notify anyone that needs notifying. */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT nick.nick_id, MAX(memo.idx), nick.nick "
	    "FROM nick, memoinfo LEFT JOIN memo "
	    "ON memo.owner=memoinfo.owner WHERE nick.nick=memo.owner && "
	    "(nick.flags & %d) && nick.link_id=0 GROUP BY memo.owner",
	    NI_MEMO_RECEIVE | NI_IRCOP);

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		nick_id = atoi(row[0]);
		idx = atoi(row[1]);
		
		if (MSNotifyAll) {
			for (u2 = firstuser(); u2; u2 = nextuser()) {
				if (u2->real_id != nick_id)
					continue;

				notice_lang(s_MemoServ, u2,
				    MEMO_NEW_MEMO_ARRIVED, s_GlobalNoticer,
				    s_MemoServ, idx);
			}
		} else {
			u2 = finduser(row[2]);
			if (u2) {
				notice_lang(s_MemoServ, u2,
				    MEMO_NEW_MEMO_ARRIVED, s_GlobalNoticer,
				    s_MemoServ, idx);
			}
		}
	}

	mysql_free_result(result);
	
	notice_lang(s_MemoServ, u, MEMO_SERVADMIN_OPERSENT);
	snoop(s_MemoServ, "[OPERSEND] %s sent an oper memo: %s", u->nick,
	    text);
	log("memo: %s just sent an oper memo: %s", u->nick, text);
	wallops(s_MemoServ, "\2%s\2 has just sent an oper memo", u->nick);
}

/*************************************************************************/

static void do_csopsend(User *u)
{
	MYSQL_ROW row;
	time_t now;
	unsigned int fields, rows, nick_id, idx;
	char *text, *esc_sender, *esc_text;
	MYSQL_RES *result;
	User *u2;

	text = strtok(NULL, "");

	if (!text) {
		syntax_error(s_MemoServ, u, "GLOBALSEND",
		    MEMO_CSOPSEND_SERVADMIN_SYNTAX);
		return;
	} else if (!nick_recognized(u)) {
		notice_lang(s_MemoServ, u, NICK_IDENTIFY_REQUIRED,
		    s_NickServ);
		return;
	}

	now = time(NULL);
	esc_sender = smysql_escape_string(mysqlconn, s_GlobalNoticer);
	esc_text = smysql_escape_string(mysqlconn, MSROT13 ? rot13(text) :
	    text);

	u->lastmemosend = now;
	
	/*
	 * We can't do this without a temporary table. :(
	 * Nuke our temp table.
	 */

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "TRUNCATE private_tmp_memo");
	mysql_free_result(result);

	/*
	 * Now insert a new memo for every single master nick, into the
	 * temporary table.  We need to check against the memo table to see
	 * what the next index is for those people who already have memos.
	 */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO private_tmp_memo "
	    "(memo_id, owner, idx, flags, time, sender, text, rot13) "
	    "SELECT NULL, memoinfo.owner, IFNULL(MAX(memo.idx)+1, 1), %d, "
	    "%d, '%s', '%s', '%c' FROM nick, memoinfo, oper LEFT JOIN memo ON "
	    "memo.owner=memoinfo.owner WHERE nick.nick=memoinfo.owner && "
	    "nick.nick_id=oper.nick_id && nick.link_id=0 GROUP BY "
	    "memoinfo.owner", MF_UNREAD | MF_CSOPSEND, now, esc_sender,
	    esc_text, MSROT13 ? 'Y' : 'N');
	mysql_free_result(result);

	free(esc_sender);
	free(esc_text);

	/* Copy over these rows into the main memo table. */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO memo "
	    "(memo_id, owner, idx, flags, time, sender, text, rot13) "
	    "SELECT NULL, owner, idx, flags, time, sender, text, rot13 "
	    "FROM private_tmp_memo");
	mysql_free_result(result);

	/* Notify anyone that needs notifying. */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT nick.nick_id, MAX(memo.idx), nick.nick "
	    "FROM nick, memoinfo, oper LEFT JOIN memo ON "
	    "memo.owner=memoinfo.owner WHERE nick.nick=memo.owner && "
	    "nick.nick_id=oper.nick_id && (nick.flags & %d) && "
	    "nick.link_id=0 GROUP BY memo.owner", NI_MEMO_RECEIVE);

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		nick_id = atoi(row[0]);
		idx = atoi(row[1]);
		
		if (MSNotifyAll) {
			for (u2 = firstuser(); u2; u2 = nextuser()) {
				if (u2->real_id != nick_id)
					continue;

				notice_lang(s_MemoServ, u2,
				    MEMO_NEW_MEMO_ARRIVED, s_GlobalNoticer,
				    s_MemoServ, idx);
			}
		} else {
			u2 = finduser(row[2]);
			if (u2) {
				notice_lang(s_MemoServ, u2,
				    MEMO_NEW_MEMO_ARRIVED, s_GlobalNoticer,
				    s_MemoServ, idx);
			}
		}
	}

	mysql_free_result(result);
	
	notice_lang(s_MemoServ, u, MEMO_SERVADMIN_CSOPSENT);
	snoop(s_MemoServ, "[CSOPSEND] %s sent a services oper memo: %s",
	    u->nick, text);
	log("memo: %s just sent a global memo: %s", u->nick, text);
	wallops(s_MemoServ, "\2%s\2 has just sent a services oper memo",
	    u->nick);
}

/*************************************************************************/

/*!
 * \brief List the memos (if any) for the source nick or given channel.
 * \param u Pointer to \link user_ User structure \endlink for user who triggered this function.
 */

static void do_list(User *u)
{
	char base[BUFSIZE];
	char timebuf[64];
	ChannelInfo ci;
	MemoInfo mi;
	struct tm tm;
	MYSQL_ROW row;
	time_t memo_time;
	unsigned int cnt, idx, fields, rows;
	int ischan, isverboten, sent_header, new, flags;
	char *param, *chan, *owner, *esc_owner, *query;
	MYSQL_RES *result;

	param = strtok(NULL, " ");
	chan = NULL;
	ci.channel_id = 0;
	mi.memoinfo_id = 0;
	new = 0;

	if (param && *param == '#') {
		chan = param;
		param = strtok(NULL, " ");
		if (!(get_channelinfo_from_id(&ci, mysql_findchan(chan)))) {
			notice_lang(s_MemoServ, u, CHAN_X_NOT_REGISTERED,
			    chan);
			return;
		} else if (ci.flags & CI_VERBOTEN) {
			notice_lang(s_MemoServ, u, CHAN_X_FORBIDDEN, chan);
			free_channelinfo(&ci);
			return;
		} else if (!check_access(u, ci.channel_id, CA_MEMO_READ)) {
			notice_lang(s_MemoServ, u, ACCESS_DENIED);
			free_channelinfo(&ci);
			return;
		}

		owner = sstrdup(chan);
	} else {
		if (!nick_identified(u)) {
			notice_lang(s_MemoServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);
			return;
		}
		
		owner = get_nick_from_id(u->real_id);
	}

	get_memoinfo_from_owner(&mi, owner, &ischan, &isverboten);

	if (param && !isdigit(*param) && stricmp(param, "NEW") != 0) {
		syntax_error(s_MemoServ, u, "LIST", MEMO_LIST_SYNTAX);
	} else if ((cnt = count_memos(owner)) == 0) {
		if (chan) {
			notice_lang(s_MemoServ, u, MEMO_X_HAS_NO_MEMOS,
			    chan);
		} else {
			notice_lang(s_MemoServ, u, MEMO_HAVE_NO_MEMOS);
		}
	} else {

		esc_owner = smysql_escape_string(mysqlconn, owner);

		if (param && isdigit(*param)) {
			/*
			 * We have been given a list or range of indices
			 * of existing memos to match.
			 */
			snprintf(base, sizeof(base),
			    "SELECT idx, flags, time, sender, text "
			    "FROM memo WHERE owner='%s' && (", esc_owner);

			/*
			 * Build a query based on the above base query and
			 * matching the range or list of indices against
			 * the memo index for this owner, order by the
			 * index.
			 */
			query = mysql_build_query(mysqlconn, param, base,
			    "idx", MAX((unsigned int)(mi.max + 1), cnt + 1));

			result = smysql_query(mysqlconn,
			    "%s) ORDER BY idx ASC", query);

			free(query);
		} else if (param) {
			/*
			 * Only accepted param is NEW - list only new
			 * memos.
			 */
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT idx, flags, time, sender, text "
			    "FROM memo WHERE owner='%s' && flags & %d "
			    "ORDER BY idx ASC", esc_owner, MF_UNREAD);

			new = 1;
		} else {
			/* List all. */
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT idx, flags, time, sender, text "
			    "FROM memo WHERE owner='%s' ORDER BY idx ASC",
			    esc_owner);
		}

		free(esc_owner);

		sent_header = 0;

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			if (!sent_header) {
				if (chan) {
					notice_lang(s_MemoServ, u,
					    new ? MEMO_LIST_CHAN_NEW_MEMOS :
					    MEMO_LIST_CHAN_MEMOS, chan,
					    s_MemoServ, chan);
				} else {
					notice_lang(s_MemoServ, u,
					    new ? MEMO_LIST_NEW_MEMOS :
					    MEMO_LIST_MEMOS, u->nick,
					    s_MemoServ);
				}
				notice_lang(s_MemoServ, u, MEMO_LIST_HEADER);
				sent_header = 1;
			}

			idx = atoi(row[0]);
			flags = atoi(row[1]);
			memo_time = atoi(row[2]);
			tm = *localtime(&memo_time);
			strftime_lang(timebuf, sizeof(timebuf), u,
			    STRFTIME_DATE_TIME_FORMAT, &tm);
			/* Just in case. */
			timebuf[sizeof(timebuf) - 1] = 0;
			notice_lang(s_MemoServ, u, MEMO_LIST_FORMAT,
			    (flags & MF_UNREAD) ? '*' : ' ', idx, row[3],
			    timebuf);
		}

		mysql_free_result(result);
	}

	free(owner);

	if (mi.memoinfo_id)
		free_memoinfo(&mi);
	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/


/*
 * Read memos.
 */

static void do_read(User * u)
{
	char base[BUFSIZE];
	char timebuf[64];
	ChannelInfo ci;
	MemoInfo mi;
	MYSQL_ROW row;
	struct tm tm;
	time_t memo_time;
	unsigned int fields, rows, cnt, memo_id, idx;
	int num, ischan, isverboten, flags;
	char *numstr, *chan, *owner, *esc_owner, *query;
	MYSQL_RES *result;

	numstr = strtok(NULL, " ");
	chan = NULL;
	ci.channel_id = 0;
	mi.memoinfo_id = 0;

	if (numstr && *numstr == '#') {
		chan = numstr;
		numstr = strtok(NULL, " ");
		
		if (!(get_channelinfo_from_id(&ci, mysql_findchan(chan)))) {
			notice_lang(s_MemoServ, u, CHAN_X_NOT_REGISTERED,
			    chan);
			return;
		} else if (ci.flags & CI_VERBOTEN) {
			notice_lang(s_MemoServ, u, CHAN_X_FORBIDDEN, chan);
			free_channelinfo(&ci);
			return;
		} else if (!check_access(u, ci.channel_id, CA_MEMO_READ)) {
			notice_lang(s_MemoServ, u, ACCESS_DENIED);
			free_channelinfo(&ci);
			return;
		}

		owner = sstrdup(chan);
	} else {
		if (!nick_identified(u)) {
			notice_lang(s_MemoServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);
			return;
		}

		owner = get_nick_from_id(u->real_id);
	}

	get_memoinfo_from_owner(&mi, owner, &ischan, &isverboten);

	num = numstr ? atoi(numstr) : -1;
	if (!numstr || (stricmp(numstr, "LAST") != 0 &&
	    stricmp(numstr, "NEW") != 0 && stricmp(numstr, "ALL") != 0 &&
	    num <= 0)) {
		syntax_error(s_MemoServ, u, "READ", MEMO_READ_SYNTAX);

	} else if ((cnt = count_memos(owner)) == 0) {
		if (chan) {
			notice_lang(s_MemoServ, u, MEMO_X_HAS_NO_MEMOS,
			    chan);
		} else {
			notice_lang(s_MemoServ, u, MEMO_HAVE_NO_MEMOS);
		}

	} else {

		esc_owner = smysql_escape_string(mysqlconn, owner);

		if (stricmp(numstr, "NEW") == 0) {
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT memo_id, idx, flags, time, "
			    "sender, text, rot13 FROM memo "
			    "WHERE owner='%s' && flags & %d ORDER BY idx "
			    "ASC", esc_owner, MF_UNREAD);
		} else if (stricmp(numstr, "ALL") == 0) {
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT memo_id, idx, flags, time, "
			    "sender, text, rot13 FROM memo "
			    "WHERE owner='%s' ORDER BY idx ASC", esc_owner);
		} else if (stricmp(numstr, "LAST") == 0) {
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT memo_id, idx, flags, time, "
			    "sender, text, rot13 FROM memo "
			    "WHERE owner='%s' ORDER BY idx DESC LIMIT 1",
			    esc_owner);
		} else {
			/*
			 * We have been given a list or range of indices
			 * of existing memos to match.
			 */
			snprintf(base, sizeof(base),
			    "SELECT memo_id, idx, flags, time, sender, "
			    "text, rot13 FROM memo WHERE owner='%s' && (",
			    esc_owner);

			/*
			 * Build a query based on the above base query and
			 * matching the range or list of indices against
			 * the memo index for this owner, order by the
			 * index.
			 */
			query = mysql_build_query(mysqlconn, numstr, base,
			    "idx", MAX((unsigned int)(mi.max + 1), cnt + 1));

			result = smysql_bulk_query(mysqlconn, &fields, &rows,
			    "%s) ORDER BY idx ASC", query);

			free(query);
		}

		free(esc_owner);

		if (rows) {
			while ((row = smysql_fetch_row(mysqlconn, result))) {
				memo_id = atoi(row[0]);
				idx = atoi(row[1]);
				flags = atoi(row[2]);
				memo_time = atoi(row[3]);
				tm = *localtime(&memo_time);
				
				strftime_lang(timebuf, sizeof(timebuf), u,
				    STRFTIME_DATE_TIME_FORMAT, &tm);
				timebuf[sizeof(timebuf) - 1] = '\0';

				if (chan) {
					notice_lang(s_MemoServ, u,
					    MEMO_CHAN_HEADER, idx, row[4],
					    timebuf, s_MemoServ, chan,
					    idx);
					update_memo_time(u, chan,
					    memo_time);
				} else {
					notice_lang(s_MemoServ, u,
					    MEMO_HEADER, idx, row[4],
					    timebuf, s_MemoServ, idx);
				}

				if (row[6][0] == 'Y')
					rot13(row[5]);

				if (flags & MF_GLOBALSEND) {
					notice_lang(s_MemoServ, u,
					    MEMO_GLOBAL_TEXT, row[5]);
				} else if (flags & MF_OPERSEND) {
					notice_lang(s_MemoServ, u,
					    MEMO_OPER_TEXT, row[5]);
				} else if (flags & MF_CSOPSEND) {
					notice_lang(s_MemoServ, u,
					    MEMO_CSOP_TEXT, row[5]);
				} else {
					notice_lang(s_MemoServ, u,
					    MEMO_TEXT, row[5]);
				}

				mark_memo_read(memo_id);
			}
		} else {
			if (chan) {
				notice_lang(s_MemoServ, u,
				    MEMO_X_HAS_NO_NEW_MEMOS, chan);
			} else {
				notice_lang(s_MemoServ, u,
				    MEMO_HAVE_NO_NEW_MEMOS);
			}
		}

		mysql_free_result(result);
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
	if (mi.memoinfo_id)
		free_memoinfo(&mi);
}

/*************************************************************************/

/*
 * Delete memos.
 */

static void do_del(User * u)
{
	char base[BUFSIZE];
	ChannelInfo ci;
	MemoInfo mi;
	unsigned int fields, rows, cnt;
	int ischan, isverboten;
	char *numstr, *chan, *owner, *esc_owner, *query;
	MYSQL_RES *result;

	numstr = strtok(NULL, "");
	chan = NULL;
	ci.channel_id = 0;
	mi.memoinfo_id = 0;

	if (numstr && *numstr == '#') {
		chan = strtok(numstr, " ");
		numstr = strtok(NULL, "");
		if (!(get_channelinfo_from_id(&ci, mysql_findchan(chan)))) {
			notice_lang(s_MemoServ, u, CHAN_X_NOT_REGISTERED,
			    chan);
			return;
		} else if (ci.flags & CI_VERBOTEN) {
			notice_lang(s_MemoServ, u, CHAN_X_FORBIDDEN, chan);
			free_channelinfo(&ci);
			return;
		} else if (!check_access(u, ci.channel_id, CA_MEMO_SEND)) {
			notice_lang(s_MemoServ, u, ACCESS_DENIED);
			free_channelinfo(&ci);
			return;
		}

		owner = sstrdup(chan);
	} else {
		if (!nick_identified(u)) {
			notice_lang(s_MemoServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);
			return;
		}

		owner = get_nick_from_id(u->real_id);
	}

	get_memoinfo_from_owner(&mi, owner, &ischan, &isverboten);

	if (!numstr || (!isdigit(*numstr) && stricmp(numstr, "ALL") != 0)) {
		syntax_error(s_MemoServ, u, "DEL", MEMO_DEL_SYNTAX);
	} else if ((cnt = count_memos(owner)) == 0) {
		if (chan) {
			notice_lang(s_MemoServ, u, MEMO_X_HAS_NO_MEMOS,
			    chan);
		} else {
			notice_lang(s_MemoServ, u, MEMO_HAVE_NO_MEMOS);
		}
	} else {

		esc_owner = smysql_escape_string(mysqlconn, owner);
		
		if (isdigit(*numstr)) {
			/*
			 * We have been given a list or range of indices
			 * of existing memos to match.
			 */
			snprintf(base, sizeof(base),
			    "DELETE FROM memo WHERE owner='%s' && (",
			    esc_owner);

			/*
			 * Build a query based on the above base query and
			 * matching the range or list of indices against
			 * the memo index for this owner.
			 */
			query = mysql_build_query(mysqlconn, numstr, base,
			    "idx", MAX((unsigned int)(mi.max + 1), cnt + 1));

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s)", query);

			free(query);

			renumber_memos(owner);
		} else {
			/* Delete all. */
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "DELETE FROM memo WHERE owner='%s'",
			    esc_owner);
		}

		free(esc_owner);
		mysql_free_result(result);

		if (rows) {
			notice_lang(s_MemoServ, u, MEMO_DELETED_X, rows,
			    rows == 1 ? "" : "s", rows == 1 ? "s" : "ve");
		} else {
			notice_lang(s_MemoServ, u, MEMO_DELETED_NONE,
			    rows);
		}
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
	if (mi.memoinfo_id)
		free_memoinfo(&mi);
}

/*************************************************************************/

static void do_set(User * u)
{
	char *cmd, *param;

	cmd = strtok(NULL, " ");
	param = strtok(NULL, "");

	if (!param) {
		syntax_error(s_MemoServ, u, "SET", MEMO_SET_SYNTAX);
		return;
	} else if (!nick_identified(u)) {
		notice_lang(s_MemoServ, u, NICK_IDENTIFY_REQUIRED,
		    s_NickServ);
		return;
	}

	if (stricmp(cmd, "NOTIFY") == 0) {
		do_set_notify(u, param);
	} else if (stricmp(cmd, "LIMIT") == 0) {
		do_set_limit(u, param);
	} else {
		notice_lang(s_MemoServ, u, MEMO_SET_UNKNOWN_OPTION,
		    strupper(cmd));
		notice_lang(s_MemoServ, u, MORE_INFO, s_MemoServ, "SET");
	}
}

/*************************************************************************/

static void do_set_notify(User *u, char *param)
{
	if (stricmp(param, "ON") == 0) {
		add_nick_flags(u->nick_id,
		    NI_MEMO_SIGNON | NI_MEMO_RECEIVE);
		notice_lang(s_MemoServ, u, MEMO_SET_NOTIFY_ON, s_MemoServ);
	} else if (stricmp(param, "LOGON") == 0) {
		add_nick_flags(u->nick_id, NI_MEMO_SIGNON);
		remove_nick_flags(u->nick_id, NI_MEMO_RECEIVE);
		notice_lang(s_MemoServ, u, MEMO_SET_NOTIFY_LOGON,
		    s_MemoServ);
	} else if (stricmp(param, "NEW") == 0) {
		remove_nick_flags(u->nick_id, NI_MEMO_SIGNON);
		add_nick_flags(u->nick_id, NI_MEMO_RECEIVE);
		notice_lang(s_MemoServ, u, MEMO_SET_NOTIFY_NEW, s_MemoServ);
	} else if (stricmp(param, "OFF") == 0) {
		remove_nick_flags(u->nick_id,
		    (NI_MEMO_SIGNON | NI_MEMO_RECEIVE));
		notice_lang(s_MemoServ, u, MEMO_SET_NOTIFY_OFF, s_MemoServ);
	} else {
		syntax_error(s_MemoServ, u, "SET NOTIFY",
		    MEMO_SET_NOTIFY_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_limit(User *u, char *param)
{
	ChannelInfo ci;
	NickInfo ni;
	MemoInfo mi;
	unsigned int nick_id;
	int limit, is_servadmin, ischan, isverboten;
	char *p1, *p2, *p3, *user, *chan;

	p1 = strtok(param, " ");
	p2 = strtok(NULL, " ");
	p3 = strtok(NULL, " ");
	user = NULL;
	chan = NULL;
	is_servadmin = is_services_admin(u);
	ci.channel_id = 0;
	ni.nick_id = 0;
	mi.memoinfo_id = 0;

	if (p1 && *p1 == '#') {
		chan = p1;
		p1 = p2;
		p2 = p3;
		p3 = strtok(NULL, " ");
		if (!(get_channelinfo_from_id(&ci, mysql_findchan(chan)))) {
			notice_lang(s_MemoServ, u, CHAN_X_NOT_REGISTERED,
			    chan);
			return;
		} else if (ci.flags & CI_VERBOTEN) {
			notice_lang(s_MemoServ, u, CHAN_X_FORBIDDEN, chan);
			free_channelinfo(&ci);
			return;
		} else if (!is_servadmin &&
		    !check_access(u, ci.channel_id, CA_SET)) {
			notice_lang(s_MemoServ, u, ACCESS_DENIED);
			free_channelinfo(&ci);
			return;
		}

		get_memoinfo_from_owner(&mi, chan, &ischan, &isverboten);
	}

	if (is_servadmin) {
		if (p2 && stricmp(p2, "HARD") != 0 && !chan) {
			if (!(nick_id = mysql_findnick(p1))) {
				notice_lang(s_MemoServ, u,
				    NICK_X_NOT_REGISTERED, p1);
				return;
			}

			nick_id = mysql_getlink(nick_id);
			
			if (!get_nickinfo_from_id(&ni, nick_id)) {
				notice_lang(s_MemoServ, u,
				    NICK_X_NOT_REGISTERED, p1);
				return;
			}
			user = p1;
			p1 = p2;
			p2 = p3;
		} else if (!p1) {
			syntax_error(s_MemoServ, u, "SET LIMIT",
			    MEMO_SET_LIMIT_SERVADMIN_SYNTAX);
			if (ci.channel_id)
				free_channelinfo(&ci);
			return;
		} else {
			user = u->nick;
		}

		get_memoinfo_from_owner(&mi, user, &ischan, &isverboten);

		if ((!isdigit(*p1) && stricmp(p1, "NONE") != 0) ||
		    (p2 && stricmp(p2, "HARD") != 0)) {
			syntax_error(s_MemoServ, u, "SET LIMIT",
			    MEMO_SET_LIMIT_SERVADMIN_SYNTAX);
			if (ci.channel_id)
				free_channelinfo(&ci);
			if (ni.nick_id)
				free_nickinfo(&ni);
			if (mi.memoinfo_id)
				free_memoinfo(&mi);
			return;
		}

		if (chan) {
			if (p2) {
				add_chan_flags(ci.channel_id,
				    CI_MEMO_HARDMAX);
			} else {
				remove_chan_flags(ci.channel_id,
				    CI_MEMO_HARDMAX);
			}
		} else {
			if (p2) {
				add_nick_flags(ni.nick_id, NI_MEMO_HARDMAX);
			} else {
				remove_nick_flags(ni.nick_id,
				    NI_MEMO_HARDMAX);
			}
		}

		limit = atoi(p1);

		if (limit < 0 || limit > 32767) {
			notice_lang(s_MemoServ, u, MEMO_SET_LIMIT_OVERFLOW,
				    32767);
			limit = 32767;
		}

		if (stricmp(p1, "NONE") == 0)
			limit = -1;
	} else {
		if (!chan) {
			get_nickinfo_from_id(&ni, u->real_id);
			get_memoinfo_from_owner(&mi, ni.nick, &ischan,
			    &isverboten);
		}

		if (!p1 || p2 || !isdigit(*p1)) {
			syntax_error(s_MemoServ, u, "SET LIMIT",
			    MEMO_SET_LIMIT_SYNTAX);
			if (ci.channel_id)
				free_channelinfo(&ci);
			if (ni.nick_id)
				free_nickinfo(&ni);
			if (mi.memoinfo_id)
				free_memoinfo(&mi);
			return;
		}

		if (chan && (ci.flags & CI_MEMO_HARDMAX)) {
			notice_lang(s_MemoServ, u, MEMO_SET_LIMIT_FORBIDDEN,
			    chan);
			if (ci.channel_id)
				free_channelinfo(&ci);
			if (ni.nick_id)
				free_nickinfo(&ni);
			if (mi.memoinfo_id)
				free_memoinfo(&mi);
			return;
		} else if (!chan && (ni.flags & NI_MEMO_HARDMAX)) {
			notice_lang(s_MemoServ, u,
			    MEMO_SET_YOUR_LIMIT_FORBIDDEN);
			if (ci.channel_id)
				free_channelinfo(&ci);
			if (ni.nick_id)
				free_nickinfo(&ni);
			if (mi.memoinfo_id)
				free_memoinfo(&mi);
			return;
		}

		limit = atoi(p1);

		/*
		 * The first character is a digit, but we could still go
		 * negative from overflow... watch out!
		 */
		if (limit < 0 || (MSMaxMemos > 0 && limit > MSMaxMemos)) {
			if (chan) {
				notice_lang(s_MemoServ, u,
				    MEMO_SET_LIMIT_TOO_HIGH, chan,
				    MSMaxMemos);
			} else {
				notice_lang(s_MemoServ, u,
				    MEMO_SET_YOUR_LIMIT_TOO_HIGH,
				    MSMaxMemos);
			}
			if (ci.channel_id)
				free_channelinfo(&ci);
			if (ni.nick_id)
				free_nickinfo(&ni);
			if (mi.memoinfo_id)
				free_memoinfo(&mi);
			return;
		} else if (limit > 32767) {
			notice_lang(s_MemoServ, u, MEMO_SET_LIMIT_OVERFLOW,
			    32767);
			limit = 32767;
		}
	}
	
	set_memo_limit(mi.memoinfo_id, limit);

	if (limit > 0) {
		if (!chan && ni.nick_id == u->nick_id) {
			notice_lang(s_MemoServ, u, MEMO_SET_YOUR_LIMIT,
			    limit);
		} else {
			notice_lang(s_MemoServ, u, MEMO_SET_LIMIT,
			    chan ? chan : user, limit);
		}
	} else if (limit == 0) {
		if (!chan && ni.nick_id == u->nick_id) {
			notice_lang(s_MemoServ, u,
			    MEMO_SET_YOUR_LIMIT_ZERO);
		} else {
			notice_lang(s_MemoServ, u, MEMO_SET_LIMIT_ZERO,
			    chan ? chan : user);
		}
	} else {
		if (!chan && ni.nick_id == u->nick_id) {
			notice_lang(s_MemoServ, u, MEMO_UNSET_YOUR_LIMIT);
		} else {
			notice_lang(s_MemoServ, u, MEMO_UNSET_LIMIT,
			    chan ? chan : user);
		}
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
	if (ni.nick_id)
		free_nickinfo(&ni);
	if (mi.memoinfo_id)
		free_memoinfo(&mi);
}

/*************************************************************************/

static void do_info(User * u)
{
	ChannelInfo ci;
	NickInfo ni;
	MemoInfo mi;
	unsigned int nick_id, channel_id, cnt, ucnt;
	int is_servadmin, hardmax, ischan, isverboten;
	char *name, *owner;

	name = strtok(NULL, " ");
	is_servadmin = is_services_admin(u);
	hardmax = 0;
	ci.channel_id = 0;
	ni.nick_id = 0;
	mi.memoinfo_id = 0;
	cnt = 0;
	ucnt = 0;

	if (is_servadmin && name && *name != '#') {
		nick_id = mysql_findnick(name);
		nick_id = mysql_getlink(nick_id);

		if (!nick_id) {
			notice_lang(s_MemoServ, u, NICK_X_NOT_REGISTERED,
			    name);
			return;
		} else if (!get_nickinfo_from_id(&ni, nick_id)) {
			notice_lang(s_MemoServ, u, NICK_X_NOT_REGISTERED,
			    name);
			return;
		} else if (ni.status & NS_VERBOTEN) {
			notice_lang(s_MemoServ, u, NICK_X_FORBIDDEN, name);
			free_nickinfo(&ni);
			return;
		}

		owner = ni.nick;
		get_memoinfo_from_owner(&mi, ni.nick, &ischan, &isverboten);
		hardmax = ni.flags & NI_MEMO_HARDMAX ? 1 : 0;
	} else if (name && *name == '#') {
		channel_id = mysql_findchan(name);
		
		if (!channel_id) {
			notice_lang(s_MemoServ, u, CHAN_X_NOT_REGISTERED,
			    name);
			return;
		}

		if (!get_channelinfo_from_id(&ci, channel_id)) {
			notice_lang(s_MemoServ, u, CHAN_X_NOT_REGISTERED,
			    name);
			return;
		} else if (ci.flags & CI_VERBOTEN) {
			notice_lang(s_MemoServ, u, CHAN_X_FORBIDDEN, name);
			free_channelinfo(&ci);
			return;
		} else if (!check_access(u, channel_id, CA_MEMO_READ)) {
			notice_lang(s_MemoServ, u, ACCESS_DENIED);
			return;
		}

		owner = ci.name;
		get_memoinfo_from_owner(&mi, ci.name, &ischan, &isverboten);
		hardmax = ci.flags & CI_MEMO_HARDMAX ? 1 : 0;
	} else {		/* !name */
		if (!nick_identified(u)) {
			notice_lang(s_MemoServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);
			return;
		}

		get_nickinfo_from_id(&ni, u->real_id);
		owner = ni.nick;
		get_memoinfo_from_owner(&mi, ni.nick, &ischan, &isverboten);
	}

	cnt = count_memos(owner);
	if (cnt)
		ucnt = count_new_memos(owner);
		
	if (name && ni.nick_id != u->nick_id) {
		if (!cnt) {
			notice_lang(s_MemoServ, u, MEMO_INFO_X_NO_MEMOS,
			    owner);
		} else if (cnt == 1) {
			if (ucnt == 1) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_X_MEMO_UNREAD, owner);
			} else {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_X_MEMO, owner);
			}
		} else {
			if (cnt == ucnt) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_X_MEMOS_ALL_UNREAD, owner,
				    cnt);
			} else if (ucnt == 0) {
				notice_lang(s_MemoServ, u, MEMO_INFO_X_MEMOS,
				    owner, cnt);
			} else if (ucnt == 1) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_X_MEMOS_ONE_UNREAD, owner,
				    cnt);
			} else {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_X_MEMOS_SOME_UNREAD, owner,
				    cnt, ucnt);
			}
		}
		if (mi.max >= 0) {
			notice(s_MemoServ, u->nick, "hardmax: %d",
			    hardmax);
			if (hardmax) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_X_HARD_LIMIT, owner, mi.max);
			} else {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_X_LIMIT, owner, mi.max);
			}
		} else {
			notice_lang(s_MemoServ, u, MEMO_INFO_X_NO_LIMIT,
			    owner);
		}

		/* Only nicks are notified of new memos. */
		if (ni.nick_id) {
			if ((ni.flags & NI_MEMO_RECEIVE) &&
			    (ni.flags & NI_MEMO_SIGNON)) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_X_NOTIFY_ON, owner);
			} else if (ni.flags & NI_MEMO_RECEIVE) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_X_NOTIFY_RECEIVE, owner);
			} else if (ni.flags & NI_MEMO_SIGNON) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_X_NOTIFY_SIGNON, owner);
			} else {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_X_NOTIFY_OFF, owner);
			}
		}

	} else {		/* !name || ni.nick_id == u->nick_id */

		if (!cnt) {
			notice_lang(s_MemoServ, u, MEMO_INFO_NO_MEMOS);
		} else if (cnt == 1) {
			if (ucnt == 1) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_MEMO_UNREAD);
			} else {
				notice_lang(s_MemoServ, u, MEMO_INFO_MEMO);
			}
		} else {
			if (cnt == ucnt) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_MEMOS_ALL_UNREAD, ucnt);
			} else if (ucnt == 0) {
				notice_lang(s_MemoServ, u, MEMO_INFO_MEMOS,
				    cnt);
			} else if (ucnt == 1) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_MEMOS_ONE_UNREAD, cnt);
			} else {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_MEMOS_SOME_UNREAD, cnt, ucnt);
			}
		}

		if (mi.max == 0) {
			if (!is_servadmin && hardmax) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_HARD_LIMIT_ZERO);
			} else {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_LIMIT_ZERO);
			}
		} else if (mi.max > 0) {
			if (!is_servadmin && hardmax) {
				notice_lang(s_MemoServ, u,
				    MEMO_INFO_HARD_LIMIT, mi.max);
			} else {
				notice_lang(s_MemoServ, u, MEMO_INFO_LIMIT,
				    mi.max);
			}
		} else {
			notice_lang(s_MemoServ, u, MEMO_INFO_NO_LIMIT);
		}
		
		if ((ni.flags & NI_MEMO_RECEIVE) &&
		    (ni.flags & NI_MEMO_SIGNON)) {
			notice_lang(s_MemoServ, u, MEMO_INFO_NOTIFY_ON);
		} else if (ni.flags & NI_MEMO_RECEIVE) {
			notice_lang(s_MemoServ, u, MEMO_INFO_NOTIFY_RECEIVE);
		} else if (ni.flags & NI_MEMO_SIGNON) {
			notice_lang(s_MemoServ, u, MEMO_INFO_NOTIFY_SIGNON);
		} else {
			notice_lang(s_MemoServ, u, MEMO_INFO_NOTIFY_OFF);
		}

	}			/* if (name && ni != u->ni) */

	if (ci.channel_id)
		free_channelinfo(&ci);
	if (ni.nick_id)
		free_nickinfo(&ni);
	if (mi.memoinfo_id)
		free_memoinfo(&mi);
}

/*************************************************************************/

/*
 * Count a given owner's total memos.
 */
static unsigned int count_memos(const char *owner)
{
	MYSQL_ROW row;
	unsigned int fields, rows, cnt;
	char *esc_owner;
	MYSQL_RES *result;

	esc_owner = smysql_escape_string(mysqlconn, owner);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT COUNT(*) FROM memo WHERE owner='%s'", esc_owner);

	free(esc_owner);

	if (rows) {
		row = smysql_fetch_row(mysqlconn, result);
		cnt = atoi(row[0]);
	} else {
		cnt = 0;
	}

	mysql_free_result(result);
	return(cnt);
}

/*
 * Count a given owner's new memos.
 */
static unsigned int count_new_memos(const char *owner)
{
	return(count_memos_with_flags(owner, MF_UNREAD));
}

/*
 * Count memos for a given owner and flag.
 */
static unsigned int count_memos_with_flags(const char *owner, int flags)
{
	MYSQL_ROW row;
	unsigned int fields, rows, cnt;
	char *esc_owner;
	MYSQL_RES *result;

	esc_owner = smysql_escape_string(mysqlconn, owner);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT COUNT(*) FROM memo WHERE owner='%s' && (flags & %d)",
	    esc_owner, flags);

	free(esc_owner);

	if (rows) {
		row = smysql_fetch_row(mysqlconn, result);
		cnt = atoi(row[0]);
	} else {
		cnt = 0;
	}

	mysql_free_result(result);
	return(cnt);
}

/*
 * Retrieve a row from the memoinfo table and store it in a MemoInfo
 * structure.  Caller should call free_memoinfo() when finished with this
 * data.  Returns the memoinfo_id or 0 on failure.
 *
 * Return in `ischan' 1 if the name was a channel name, else 0.
 * Return in `isverboten' 1 if the nick/chan is forbidden, else 0.
 * 
 */
static unsigned int get_memoinfo_from_owner(MemoInfo *mi,
    const char *owner, int *ischan, int *isverboten)
{
	MYSQL_ROW row;
	unsigned int channel_id, nick_id, fields, rows;
	char *esc_owner;
	MYSQL_RES *result;

	*isverboten = 0;

	if (owner[0] == '#') {
		if (ischan)
			*ischan = 1;

		channel_id = mysql_findchan(owner);

		if (channel_id) {
			if (get_chan_flags(channel_id) & CI_VERBOTEN) {
				*isverboten = 1;
				mi->memoinfo_id = 0;
				mi->owner = NULL;
				mi->max = 0;
				return(0);
			}
		} else {
			mi->memoinfo_id = 0;
			mi->owner = NULL;
			mi->max = 0;
			return(0);
		}
	} else {
		if (ischan)
			*ischan = 0;

		nick_id = mysql_findnick(owner);

		if (nick_id) {
			if (get_nick_status(nick_id) & NS_VERBOTEN) {
				*isverboten = 1;
				mi->memoinfo_id = 0;
				mi->owner = NULL;
				mi->max = 0;
				return(0);
			}
		} else {
			mi->memoinfo_id = 0;
			mi->owner = NULL;
			mi->max = 0;
			return(0);
		}
	}

	esc_owner = smysql_escape_string(mysqlconn, owner);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT memoinfo_id, owner, max FROM memoinfo WHERE owner='%s'",
	    esc_owner);

	free(esc_owner);

	if (rows) {
		row = smysql_fetch_row(mysqlconn, result);
		mi->memoinfo_id = atoi(row[0]);
		mi->owner = sstrdup(row[1]);
		mi->max = atoi(row[2]);
	} else {
		mi->memoinfo_id = 0;
		mi->owner = NULL;
		mi->max = 0;
	}

	mysql_free_result(result);
	return(mi->memoinfo_id);
}

/*
 * Free a MemoInfo structure which has been populated with the above
 * function.
 */
static void free_memoinfo(MemoInfo *mi)
{
	if (!mi || !mi->memoinfo_id)
		return;

	if (mi->owner)
		free(mi->owner);
}

/*
 * Renumber a given owner's memos.
 */
static void renumber_memos(const char *owner)
{
	unsigned int fields, rows;
	char *esc_owner;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "TRUNCATE private_tmp_memo2");
	mysql_free_result(result);
	
	esc_owner = smysql_escape_string(mysqlconn, owner);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO private_tmp_memo2 (memo_id, idx, flags, time, "
	    "sender, text, rot13) SELECT memo_id, NULL, flags, time, "
	    "sender, text, rot13 FROM memo WHERE owner='%s' ORDER BY time "
	    "ASC", esc_owner);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "REPLACE INTO memo (memo_id, owner, idx, flags, time, sender, "
	    "text, rot13) SELECT memo_id, '%s', idx, flags, time, sender, "
	    "text, rot13 FROM private_tmp_memo2", esc_owner);
	mysql_free_result(result);
	free(esc_owner);
}

/*
 * Return the index of the given memo_id or zero if it does not exist.
 */
static unsigned int get_memo_idx(unsigned int memo_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows, idx;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT idx FROM memo WHERE memo_id=%u", memo_id);

	if (rows) {
		row = smysql_fetch_row(mysqlconn, result);
		idx = atoi(row[0]);
	} else {
		idx = 0;
	}
	
	mysql_free_result(result);
	return(idx);
}

/*
 * Mark a specified memo as read.
 */
static void mark_memo_read(unsigned int memo_id)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE memo SET flags=flags & ~%d WHERE memo_id=%u", MF_UNREAD,
	    memo_id);
	mysql_free_result(result);
}

/*
 * Set a specific memoinfo's max memo limit.
 */
static void set_memo_limit(unsigned int memoinfo_id, int max)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE memoinfo SET max=%d WHERE memoinfo_id=%u", max,
	    memoinfo_id);
	mysql_free_result(result);
}

/*
 * Internal function for doing the actual work of memo sending. "u" is a
 * pointer for the User structure of the user sending the memo, and should
 * be NULL if this memo comes from services itself.
 */
static void send_memo(User *u, const char *name, const char *source,
    char *text)
{
	unsigned int memo_id, number, nick_id, fields, rows;
	int ischan;
	char *esc_name, *esc_source, *esc_text;
	MYSQL_RES *result;
	User *r;
	Channel *c;
	struct c_userlist *cu;

	if (strchr(name, '#'))
		ischan = 1;
	else
		ischan = 0;
	
	esc_name = smysql_escape_string(mysqlconn, name);
	esc_source = smysql_escape_string(mysqlconn, source);
	esc_text = smysql_escape_string(mysqlconn, MSROT13 ? rot13(text) :
	    text);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO memo (memo_id, owner, idx, flags, time, sender, "
	    "text, rot13) VALUES ('NULL', '%s', 0, %d, %d, '%s', '%s', "
	    "'%c')", esc_name, MF_UNREAD, time(NULL), esc_source, esc_text,
	    MSROT13 ? 'Y' : 'N');
	mysql_free_result(result);

	free(esc_text);
	free(esc_source);
	free(esc_name);

	memo_id = (unsigned int)mysql_insert_id(mysqlconn);
	renumber_memos(name);
	number = get_memo_idx(memo_id);

	if (u)
		notice_lang(s_MemoServ, u, MEMO_SENT, name);

	if (!ischan) {
		nick_id = mysql_getlink(mysql_findnick(name));

		if (get_nick_flags(nick_id) & NI_MEMO_RECEIVE) {
			if (MSNotifyAll) {
				for (r = firstuser(); r; r = nextuser()) {
					if (r->real_id == nick_id) {
						notice_lang(s_MemoServ, r,
						    MEMO_NEW_MEMO_ARRIVED,
						    source, s_MemoServ,
						    number);
					}
				}
			} else {
				r = finduser(name);
				if (r) {
					notice_lang(s_MemoServ, r,
					    MEMO_NEW_MEMO_ARRIVED, source,
					    s_MemoServ, number);
				}
			}	/* if (MSNotifyAll) */
		}		/* if (flags & MEMO_RECEIVE) */
	} else {		/* was sent to a channel */
		c = findchan(name);

		if (c) {
			for (cu = c->users; cu; cu = cu->next) {
				if (!nick_recognized(cu->user) ||
				    cu->user == u) {
					continue;
				}
					
				if (!check_access(cu->user, c->channel_id,
				    CA_MEMO_READ)) {
					continue;
				}

				notice_lang(s_MemoServ, cu->user,
				    MEMO_NEW_CHAN_MEMO_ARRIVED, source,
				    c->name, s_MemoServ, c->name, number);
			}
		}
	} 			/* if (!ischan) */
}
