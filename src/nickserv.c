
/* NickServ functions.
 *
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

/*
 * Note that with the addition of nick links, most functions in Services
 * access the "effective nick" (u->ni) to determine privileges and such.
 * The only functions which access the "real nick" (u->real_ni) are:
 *      various functions which set/check validation flags
 *          (validate_user, cancel_user, nick_{identified,recognized},
 *           do_identify)
 *      validate_user (adding a collide timeout on the real nick)
 *      cancel_user (deleting a timeout on the real nick)
 *      do_register (checking whether the real nick is registered)
 *      do_drop (dropping the real nick)
 *      do_link (linking the real nick to another)
 *      do_unlink (unlinking the real nick)
 *      chanserv.c/do_register (setting the founder to the real nick)
 * plus a few functions in users.c relating to nick creation/changing.
 */

#include "services.h"
#include "pseudo.h"

RCSID("$Id$");

/*************************************************************************/

NickInfo *nicklists[256];       /* One for each initial character */

#define TO_COLLIDE   0		/* Collide the user with this nick */
#define TO_RELEASE   1		/* Release a collided nick */

/*************************************************************************/

static unsigned int add_nicksuspend(unsigned int nick_id,
				    const char *reason, const char *who,
				    const time_t expires);
static void del_nicksuspend(unsigned int nicksuspend_id);
static void unsuspend(unsigned int nick_id);

static int is_on_access(User *u, unsigned int nick_id);
static void delink(unsigned int nick_id);

static void collide(unsigned int nick_id, int from_timeout);
static void release(unsigned int nick_id, int from_timeout);
static void add_ns_timeout(unsigned int nick_id, int type, time_t delay);
static void del_ns_timeout(unsigned int nick_id, int type);

static void add_nick_status(unsigned int nick_id, int new_status);
static void remove_nick_status(unsigned int nick_id, int remove_status);
static void set_nick_regtime(unsigned int nick_id, time_t regtime);
static int insert_new_nick(NickInfo *ni, MemoInfo *mi, char *access);
static int mysql_delnick(unsigned int nick_id);
static void mysql_remove_links(unsigned int nick_id);
static void ns_mysql_remove_access(unsigned int nick_id);
static void ns_mysql_remove_memos(unsigned int nick_id);
static void mysql_delete_nick(unsigned int nick_id);
static unsigned int check_nick_password(const char *pass,
					unsigned int nick_id);
static unsigned int nickaccess_count(unsigned int nick_id);
static void mysql_renumber_nick_access(unsigned int nick_id);
static void register_nick(NickInfo *ni, MemoInfo *mi, User *u,
    const char *pass, const char *email);
static unsigned int nick_has_links(unsigned int nick_id);

#ifdef DEBUG_COMMANDS
static void do_commands(User *u);
#endif
static void do_help(User *u);
static void do_register(User *u);
static void do_identify(User *u);
static void do_drop(User *u);
static void do_set(User *u);
static void do_set_password(User *u, unsigned int nick_id, char *param);
static void do_set_language(User *u, unsigned int nick_id, char *param);
static void do_set_url(User *u, unsigned int nick_id, char *param);
static void do_set_email(User *u, unsigned int nick_id, char *param);
static void do_set_cloakstring(User *u, unsigned int nick_id, char *param);
static void do_set_enforce(User *u, unsigned int nick_id, char *param);
static void do_set_secure(User *u, unsigned int nick_id, char *param);
static void do_set_private(User *u, unsigned int nick_id, char *param);
static void do_set_hide(User *u, unsigned int nick_id, char *param);
static void do_set_cloak(User *u, unsigned int nick_id, char *param);
static void do_set_noexpire(User *u, unsigned int nick_id, char *param);
static void do_set_noop(User *u, unsigned int nick_id, char *param);
static void do_set_ircop(User *u, unsigned int nick_id, char *param);
static void do_set_mark(User *u, unsigned int nick_id, char *param);
static void do_set_regtime(User *u, unsigned int nick_id, char *param);
static void do_set_sendpass(User *u, unsigned int nick_id, char *param);
static void do_set_autojoin(User *u, unsigned int nick_id, char *param);
static void do_access(User *u);
static void do_link(User *u);
static void do_unlink(User *u);
static void do_listchanaccess(User *u);
static void do_listlinks(User *u);
static void do_info(User *u);
static void do_list(User *u);
static void do_recover(User *u);
static void do_release(User *u);
static void do_ghost(User *u);
/*static void do_regain(User *u);*/
static void do_status(User *u);
static void do_forbid(User *u);
static void do_suspend(User *u);
static void do_unsuspend(User *u);
static void do_sendpass(User *u);

/*************************************************************************/

static Command cmds[] = {
#ifdef DEBUG_COMMANDS
	{"COMMANDS", do_commands, NULL, -1, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
#endif
	{"HELP", do_help, NULL, -1, -1, -1, -1, -1, NULL, NULL, NULL,
	    NULL},
	{"REGISTER", do_register, NULL, NICK_HELP_REGISTER, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"REG", do_register, NULL, NICK_HELP_REGISTER, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"IDENTIFY", do_identify, NULL, NICK_HELP_IDENTIFY, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"ID", do_identify, NULL, NICK_HELP_IDENTIFY, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"SIDENTIFY", do_identify, NULL, -1, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	{"DROP", do_drop, NULL, -1, NICK_HELP_DROP,
	    NICK_SERVADMIN_HELP_DROP, NICK_SERVADMIN_HELP_DROP,
	    NICK_SERVADMIN_HELP_DROP, NULL, NULL, NULL, NULL},
	{"ACCESS", do_access, NULL, NICK_HELP_ACCESS, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"LINK", do_link, NULL, NICK_HELP_LINK, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	{"UNLINK", do_unlink, NULL, NICK_HELP_UNLINK, -1,
	    NICK_SERVADMIN_HELP_UNLINK, NICK_SERVADMIN_HELP_UNLINK,
	    NICK_SERVADMIN_HELP_UNLINK, NULL, NULL, NULL, NULL},
	{"SET", do_set, NULL, NICK_HELP_SET, -1, NICK_SERVADMIN_HELP_SET,
	    NICK_SERVADMIN_HELP_SET, NICK_SERVADMIN_HELP_SET, NULL, NULL,
	    NULL, NULL},
	{"SET PASSWORD", NULL, NULL, NICK_HELP_SET_PASSWORD, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET URL", NULL, NULL, NICK_HELP_SET_URL, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"SET EMAIL", NULL, NULL, NICK_HELP_SET_EMAIL, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"SET ENFORCE", NULL, NULL, NICK_HELP_SET_ENFORCE, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET SECURE", NULL, NULL, NICK_HELP_SET_SECURE, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET PRIVATE", NULL, NULL, NICK_HELP_SET_PRIVATE, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET NOOP", NULL, NULL, NICK_HELP_SET_NOOP, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"SET IRCOP", NULL, NULL, NICK_HELP_SET_IRCOP, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"SET HIDE", NULL, NULL, NICK_HELP_SET_HIDE, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"SET NOEXPIRE", NULL, NULL, -1, -1,
	    NICK_SERVADMIN_HELP_SET_NOEXPIRE,
	    NICK_SERVADMIN_HELP_SET_NOEXPIRE,
	    NICK_SERVADMIN_HELP_SET_NOEXPIRE, NULL, NULL, NULL, NULL},
	{"SET MARK", NULL, NULL, -1, -1, NICK_SERVADMIN_HELP_SET_MARK,
	    NICK_SERVADMIN_HELP_SET_MARK, NICK_SERVADMIN_HELP_SET_MARK,
	    NULL, NULL, NULL, NULL},
	{"SET REGTIME", NULL, NULL, -1, -1, NICK_SERVADMIN_HELP_SET_REGTIME,
	    NICK_SERVADMIN_HELP_SET_REGTIME,
	    NICK_SERVADMIN_HELP_SET_REGTIME, NULL, NULL, NULL, NULL},
	{"SET SENDPASS", NULL, NULL, NICK_HELP_SET_SENDPASS, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET AUTOJOIN", NULL, NULL, NICK_HELP_SET_AUTOJOIN, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"RECOVER", do_recover, NULL, NICK_HELP_RECOVER, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"RELEASE", do_release, NULL, NICK_HELP_RELEASE, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"GHOST", do_ghost, NULL, NICK_HELP_GHOST, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
/*	{"REGAIN", do_regain, NULL, NICK_HELP_REGAIN, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},*/
	{"INFO", do_info, NULL, NICK_HELP_INFO, -1, NICK_HELP_INFO,
	    NICK_SERVADMIN_HELP_INFO, NICK_SERVADMIN_HELP_INFO, NULL, NULL,
	    NULL, NULL},
	{"LIST", do_list, NULL, -1, NICK_HELP_LIST,
	    NICK_SERVADMIN_HELP_LIST, NICK_SERVADMIN_HELP_LIST,
	    NICK_SERVADMIN_HELP_LIST, NULL, NULL, NULL, NULL},
	{"STATUS", do_status, NULL, NICK_HELP_STATUS, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"LISTCHANS", do_listchanaccess, NULL, NICK_HELP_LISTCHANS, -1,
	    NICK_SERVADMIN_HELP_LISTCHANS, NICK_SERVADMIN_HELP_LISTCHANS,
	    NICK_SERVADMIN_HELP_LISTCHANS, NULL, NULL, NULL, NULL},
	{"LISTLINKS", do_listlinks, NULL, NICK_HELP_LISTLINKS, -1,
	    NICK_SERVADMIN_HELP_LISTLINKS, NICK_SERVADMIN_HELP_LISTLINKS,
	    NICK_SERVADMIN_HELP_LISTLINKS, NULL, NULL, NULL, NULL},
	{"FORBID", do_forbid, is_services_admin, -1, -1,
	    NICK_SERVADMIN_HELP_FORBID, NICK_SERVADMIN_HELP_FORBID,
	    NICK_SERVADMIN_HELP_FORBID, NULL, NULL, NULL, NULL},
	{"SUSPEND", do_suspend, is_services_admin, -1, -1,
	    NICK_SERVADMIN_HELP_SUSPEND, NICK_SERVADMIN_HELP_SUSPEND,
	    NICK_SERVADMIN_HELP_SUSPEND, NULL, NULL, NULL, NULL},
	{"UNSUSPEND", do_unsuspend, is_services_admin, -1, -1,
	    NICK_SERVADMIN_HELP_UNSUSPEND, NICK_SERVADMIN_HELP_UNSUSPEND,
	    NICK_SERVADMIN_HELP_UNSUSPEND, NULL, NULL, NULL, NULL},
	{"SENDPASS", do_sendpass, NULL, NICK_HELP_SENDPASS, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"AUTOJOIN", do_autojoin, NULL, NICK_HELP_AUTOJOIN, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
        {"SET CLOAKSTRING", NULL, NULL, -1, -1, NICK_SERVADMIN_HELP_SET_CLOAKSTRING,
            NICK_SERVADMIN_HELP_SET_CLOAKSTRING, NICK_SERVADMIN_HELP_SET_CLOAKSTRING,
        NULL, NULL, NULL, NULL},
        {"SET CLOAK", NULL, NULL, NICK_HELP_SET_CLOAK, -1, -1, -1, -1, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, -1, -1, -1, -1, -1, NULL, NULL, NULL, NULL}
};

/*************************************************************************/

/*************************************************************************/

/* Return information on memory use.  Assumes pointers are valid. */

void get_nickserv_stats(long *nrec, long *memuse)
{
	long count = 0, mem = 0;

	get_table_stats(mysqlconn, "nick", (unsigned int *)&count,
			(unsigned int *)&mem);
	*nrec += count;
	*memuse += (mem / 1024);

	get_table_stats(mysqlconn, "nickaccess", (unsigned int *)&count,
			(unsigned int *)&mem);

	*memuse += (mem / 1024);
}

/*************************************************************************/

/*************************************************************************/

/* NickServ initialization. */

void ns_init(void)
{
	/* set status & regainid for all nicks to 0 */
	MYSQL_RES *result;
	unsigned int fields, rows;

  /* Don't reset status 2(NS_VERBOTEN) because forbid is forever 
   * This should also not reset no_expire, but atm, it does.
   */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE nick SET status=0, regainid=0 WHERE status != 2");
	mysql_free_result(result);
	nickcache_init();
}

/*************************************************************************/

/* Main NickServ routine. */

void nickserv(const char *source, char *buf)
{
	char *cmd, *s;
	User *u = finduser(source);

	if (!u) {
		log("%s: user record for %s not found", s_NickServ, source);
		notice(s_NickServ, source,
		       getstring(0, USER_RECORD_NOT_FOUND));
		return;
	}

	cmd = strtok(buf, " ");

	if (!cmd) {
		return;
	} else if (stricmp(cmd, "\1PING") == 0) {
		if (!(s = strtok(NULL, "")))
			s = "\1";
		notice(s_NickServ, source, "\1PING %s", s);
	} else if (stricmp(cmd, "\1VERSION\1") == 0) {
		notice(s_NickServ, source, "\1VERSION Blitzed-Based OFTC IRC Services v%s\1",
		    VERSION);
	} else {
		run_cmd(s_NickServ, u, cmds, cmd);
	}

}

/*************************************************************************/

/* Check whether a user is on the access list of the nick they're using,
 * and set the NS_ON_ACCESS flag if so.  Return 1 if on the access list, 0
 * if not.
 */

int check_on_access(User *u)
{
	int on_access;

	if (!u->nick_id)
		return 0;
	on_access = is_on_access(u, u->nick_id);
	if (on_access) {
		MYSQL_RES *result;
		unsigned int fields, rows;

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "UPDATE nick SET "
					   "status=status | %d "
					   "WHERE nick_id=%u",
					   NS_ON_ACCESS, u->real_id);
		mysql_free_result(result);

		/* keep our nick cache in sync */
		nickcache_add_status(u->real_id, NS_ON_ACCESS);
	}
	return on_access;
}

/*************************************************************************/

/* Check whether a user is on the access list of the nick they're using, or
 * if they're the same user who last identified for the nick.  If not, send
 * warnings as appropriate.  If so (and not NI_SECURE), update last seen
 * info.  Return 1 if the user is valid and recognized, 0 otherwise (note
 * that this means an NI_SECURE nick will return 0 from here unless the
 * user's timestamp matches the last identify timestamp).  If the user's
 * nick is not registered, 0 is returned.
 *
 * This function now also handles REGAINed nicks by going through the
 * process of identifying them to services here.
 */

int validate_user(User *u)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	int on_access, nick_flags;

	if (debug)
		log("debug: validating nick %u", u->real_id);

	if (!u->real_id)
		return 0;

	nick_flags = get_nick_flags(u->nick_id);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT 1 FROM nick "
				   "WHERE nick_id=%u && "
				   "((status & %d) || (flags & %d))",
				   u->real_id, NS_VERBOTEN, NI_SUSPENDED);
	mysql_free_result(result);

	if (rows) {
		notice_lang(s_NickServ, u, NICK_MAY_NOT_BE_USED);
		notice_lang(s_NickServ, u,
			    FORCENICKCHANGE_IN_1_MINUTE);
		add_ns_timeout(u->real_id, TO_COLLIDE, 60);
		return(0);
	}

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT 1 FROM nick "
				   "WHERE nick_id=%u",
				   u->real_id);

			mysql_free_result(result);

	/* IDENTIFIED may be set if another (valid) user was using the nick and
	 * changed to a different, linked one.  Clear it here or people can
	 * steal privileges. */
	remove_nick_status(u->real_id, NS_TEMPORARY);

	if (!NoSplitRecovery) {
		char buf[256];
		char *esc_buf;

		snprintf(buf, sizeof(buf), "%s@%s", u->username, u->host);
		esc_buf = smysql_escape_string(mysqlconn, buf);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "UPDATE nick SET "
					   "status=status | %d "
					   "WHERE id_stamp != 0 && "
					   "id_stamp=%u && "
					   "last_usermask='%s' && "
					   "nick_id=%u",
					   NS_IDENTIFIED,
					   u->services_stamp, esc_buf,
					   u->real_id);
		mysql_free_result(result);
		free(esc_buf);

		if (debug && rows) {
			log("NickServ: Accepting nick_id %u as IDENTIFIED "
			    "because services_stamp matches", u->real_id);
			snoop(s_NickServ, "[ID] Accepting nick_id %u as "
			      "IDENTIFIED because services_stamp matches",
			      u->real_id);
		}

		if (rows) {
			/* keep our nick cache in sync */
			nickcache_add_status(u->real_id, NS_IDENTIFIED);

			check_autojoins(u);

			return(1);
		}
	}


	on_access = is_on_access(u, u->nick_id);
	if (on_access)
		add_nick_status(u->real_id, NS_ON_ACCESS);

	if (!(nick_flags & NI_SECURE) && on_access) {
		char *last_usermask;
		char *esc_last_usermask;
		char *esc_last_realname;

		last_usermask = smalloc(strlen(u->username) +
					strlen(u->host) + 2);
		sprintf(last_usermask, "%s@%s", u->username, u->host);
		esc_last_usermask = smysql_escape_string(mysqlconn,
							 last_usermask);
		esc_last_realname = smysql_escape_string(mysqlconn,
							 u->realname);
		
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "UPDATE nick SET "
					   "status=status | %d, "
					   "last_seen=%d, "
					   "last_usermask='%s', "
					   "last_realname='%s' "
					   "WHERE nick_id=%u",
					   NS_RECOGNIZED, time(NULL),
					   esc_last_usermask,
					   esc_last_realname, u->real_id);
		mysql_free_result(result);
		free(esc_last_realname);
		free(esc_last_usermask);
		free(last_usermask);

		/* keep our nick cache in sync */
		nickcache_add_status(u->real_id, NS_RECOGNIZED);
		return(1);
	}

	if (nick_flags & NI_SECURE) {
		notice_lang(s_NickServ, u, NICK_IS_SECURE, s_NickServ);
	} else {
		notice_lang(s_NickServ, u, NICK_IS_REGISTERED, s_NickServ);
	}

	if ((nick_flags & NI_ENFORCE) && !on_access) {
		if (nick_flags & NI_ENFORCEQUICK) {
			notice_lang(s_NickServ, u,
				    FORCENICKCHANGE_IN_20_SECONDS);
			add_ns_timeout(u->real_id, TO_COLLIDE, 20);
		} else {
			notice_lang(s_NickServ, u,
				    FORCENICKCHANGE_IN_1_MINUTE);
			add_ns_timeout(u->real_id, TO_COLLIDE, 60);
		}
	}

	return 0;
}

/*************************************************************************/

/* Cancel validation flags for a nick (i.e. when the user with that nick
 * signs off or changes nicks).  Also cancels any impending collide. */

void cancel_user(User *u)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	char realname[NICKMAX + 16];	/*Long enough for s_NickServ and " Enforcement" */

	if (debug)
		log("debug: cancelling user %s", u->nick);

	if (u->real_id) {
		snprintf(realname, sizeof(realname), "%s Enforcement",
			 s_NickServ);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "UPDATE nick "
					   "SET status=status & ~%d "
					   "WHERE nick_id=%u && "
					   "(status & %d)", NS_TEMPORARY,
					   u->real_id, NS_GUESTED);
		mysql_free_result(result);

		if (rows) {
			/* nick was guested */
			if (debug) {
				log("debug: %s is guested, introducing enforcer",
				    u->nick);
			}

			send_nick(u->nick, NSEnforcerUser, NSEnforcerHost,
				  ServerName, realname);
			add_ns_timeout(u->real_id, TO_RELEASE,
				       NSReleaseTimeout);

			add_nick_status(u->real_id, NS_KILL_HELD);
		} else {
			remove_nick_status(u->real_id, NS_TEMPORARY);
		}
		del_ns_timeout(u->real_id, TO_COLLIDE);
	}
}

/*************************************************************************/

/* Return whether a user has identified for their nickname. */

int nick_identified(User *u)
{
	return(u->real_id && (get_nick_status(u->real_id) & NS_IDENTIFIED));
}

/*************************************************************************/

/* Return whether a user is recognized for their nickname. */

int nick_recognized(User *u)
{
	return(u->real_id && (get_nick_status(u->real_id) &
			      (NS_IDENTIFIED | NS_RECOGNIZED)));
}

/*************************************************************************/

/* Remove nickname suspensions that have expired. */

void expire_nicksuspends(void)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows;
	time_t now = time(NULL);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT nick.nick, nick.nick_id, "
				   "nicksuspend.nicksuspend_id "
				   "FROM nick, nicksuspend, suspend WHERE "
				   "nick.nick_id=nicksuspend.nick_id && "
				   "suspend.suspend_id=nicksuspend.suspend_id && "
				   "suspend.expires != 0 && "
				   "suspend.expires <= %d", now);
	
	while ((row = smysql_fetch_row(mysqlconn, result))) {
		log("Expiring nick-suspend for %s", row[0]);
		unsuspend(strtoul(row[1], NULL, 10));
		del_nicksuspend(strtoul(row[2], NULL, 10));
	}
	mysql_free_result(result);
}

/*************************************************************************/

/* Remove all nicks which have expired.  Also update last-seen time for all
 * nicks.
 */

void expire_nicks()
{
	MYSQL_ROW row;
	time_t now;
	unsigned int fields, rows, nick_id, expire_count;
	MYSQL_RES *result;

	now = time(NULL);

	if (debug >= 2) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT nick FROM nick WHERE (status & %d)",
		    (NS_IDENTIFIED | NS_RECOGNIZED));

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			log("debug: NickServ: updating last seen time for %s",
			    row[0]);
		}
		mysql_free_result(result);
	}

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE nick SET last_seen=%d WHERE (status & %d)", now,
	    (NS_IDENTIFIED | NS_RECOGNIZED));
	mysql_free_result(result);

	if (!NSExpire || opt_noexpire)
		return;

	expire_count = 0;

	if (NSExtraGrace) {
		/*
		 * Extra Grace option - nicks that are older than NSExpire*1.5
		 * seconds are allowed to be inactive for 3*NSExpire seconds as
		 * opposed to just NSExpire seconds.
		 */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT nick_id, nick FROM nick "
		    "WHERE (%d - time_registered >= %d) && "
		    "(%d - last_seen >= %d) && !((status & %d) || "
		    "(flags & %d))", now, (int)(NSExpire * 1.5), now,
		    (NSExpire * 3), (NS_VERBOTEN | NS_NO_EXPIRE),
		    (NI_SUSPENDED | NI_IRCOP));

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			nick_id = atoi(row[0]);

			log("Expiring nickname %s", row[1]);
			snoop(s_NickServ, "[EXPIRE] Expiring %s",
			    row[1]);

			mysql_delnick(nick_id);
			expire_count++;
		}

		mysql_free_result(result);
		
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT nick_id, nick FROM nick "
		    "WHERE (%d - time_registered < %d) && "
		    "(%d - last_seen >= %d) && !((status & %d) || "
		    "(flags & %d))", now, (int)(NSExpire * 1.5), now,
		    NSExpire, (NS_VERBOTEN | NS_NO_EXPIRE),
		    (NI_SUSPENDED | NI_IRCOP));
	
	} else {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT nick_id, nick FROM nick "
		    "WHERE (%d - last_seen >= %d) && !((status & %d) || "
		    "(flags & %d))", now, NSExpire,
		    (NS_VERBOTEN | NS_NO_EXPIRE),
		    (NI_SUSPENDED | NI_IRCOP));
	}

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		nick_id = atoi(row[0]);
		
		log("Expiring nickname %s", row[1]);
		snoop(s_NickServ, "[EXPIRE] Expiring %s", row[1]);

		mysql_delnick(nick_id);
		expire_count++;
	}
	mysql_free_result(result);
	
	if (expire_count) {
		snoop(s_NickServ, "[EXPIRE] %d nick%s expired",
		    expire_count, expire_count == 1 ? "" : "s");
	}
}

/*************************************************************************/

/* return the "master" nick_id for the given nick_id, note in the new scheme
 * we do not allow for multiple levels of linking so this is trivial */
unsigned int mysql_getlink(unsigned int nick_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows, master_id = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT link_id FROM nick "
				   "WHERE nick_id=%u", nick_id);
	if (rows) {
		row = smysql_fetch_row(mysqlconn, result);
		master_id = atoi(row[0]);
	}

	mysql_free_result(result);

	return(master_id? master_id : nick_id);
}

/*************************************************************************/

/*********************** NickServ private routines ***********************/

/*************************************************************************/

/* Insert a new NickSuspend info structure into the suspended nick list and
 * fill in the values. 
 */

static unsigned int add_nicksuspend(unsigned int nick_id,
				    const char *reason, const char *who,
				    const time_t expires)
{
	MYSQL_RES *result;
	unsigned int fields, rows, suspend_id;
	char *esc_who = smysql_escape_string(mysqlconn, who);
	char *esc_reason = smysql_escape_string(mysqlconn, reason);
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "INSERT INTO suspend (suspend_id, who, "
				   "reason, suspended, expires) VALUES "
				   "('NULL', '%s', '%s', %d, %d)",
				   esc_who, esc_reason, time(NULL),
				   expires);
	mysql_free_result(result);

	free(esc_reason);
	free(esc_who);

	suspend_id = mysql_insert_id(mysqlconn);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "INSERT INTO nicksuspend "
				   "(nicksuspend_id, nick_id, suspend_id) "
				   "VALUES ('NULL', %u, %u)", nick_id,
				   suspend_id);
	mysql_free_result(result);

	return(mysql_insert_id(mysqlconn));
}

/* Delete the structure containing the suspension info for the given nick. */

/* FIXME: Possible redundant debug log code below. */

static void del_nicksuspend(unsigned int nicksuspend_id)
{
	if (nicksuspend_id) {
		MYSQL_RES *result;
		MYSQL_ROW row;
		unsigned int fields, rows, suspend_id = 0;
		
		if (debug)
			log("debug: del_nicksuspend called for "
			    "nicksuspend_id=%u", nicksuspend_id);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "SELECT suspend_id "
					   "FROM nicksuspend "
					   "WHERE nicksuspend_id=%u",
					   nicksuspend_id);

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			suspend_id = atoi(row[0]);
		}

		mysql_free_result(result);

		if (suspend_id) {
			result = smysql_bulk_query(mysqlconn, &fields,
						   &rows, "DELETE FROM "
						   "suspend WHERE "
					   	   "suspend_id=%u",
						   suspend_id);
			mysql_free_result(result);
		}

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "DELETE FROM nicksuspend "
					   "WHERE nicksuspend_id=%u",
					   nicksuspend_id);
		mysql_free_result(result);
		if (debug)
			log("debug: del_nicksuspend completed.");
	}
}

/*
 * Do everything nescessary to unsuspend the given nickname and remove the
 * suspension information. We also alter the last_seen value to ensure that
 * it does not expire within the next NSSuspendGrace seconds; giving the
 * owner a chance to reclaim it.
 */

/* FIXME: Possible redundant debug log code below. */

static void unsuspend(unsigned int nick_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	time_t now = time(NULL);
	char *nick = get_nick_from_id(nick_id);

	if (debug)
		log("debug: unsuspend called for %s", nick);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE nick SET flags=flags & ~%d "
				   "WHERE nick_id=%u", NI_SUSPENDED,
				   nick_id);
	mysql_free_result(result);

	if (NSExpire &&NSSuspendGrace) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "UPDATE nick SET last_seen=%d "
					   "WHERE nick_id=%u && "
					   "(%d - last_seen >= %u - %u)",
					   now - NSExpire + NSSuspendGrace,
					   nick_id, now, NSExpire,
					   NSSuspendGrace);
		mysql_free_result(result);

		if (rows) {
			log("Altering last_seen time for %s to %d.", nick,
			    now - NSExpire + NSSuspendGrace);
		}
	}
	
	if (debug)
		log("debug: unsuspend completed.");
	free(nick);
}

/*************************************************************************/

/* Is the given user's address on the given nick's access list?  Return 1
 * if so, 0 if not. */

static int is_on_access(User *u, unsigned int nick_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows, i;
	char *buf, *esc_buf;

	i = strlen(u->username);
	buf = smalloc(i + strlen(u->host) + 2);
	sprintf(buf, "%s@%s", u->username, u->host);
	strlower(buf + i + 1);
	esc_buf = smysql_escape_string(mysqlconn, buf);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT 1 FROM nickaccess WHERE nick_id=%u && "
	    "IRC_MATCH(userhost, '%s')", nick_id, esc_buf);
	mysql_free_result(result);
	free(esc_buf);
	free(buf);

	return(rows? 1 : 0);
}

/*************************************************************************/

/* handle the removal of a nick from the database */
static int mysql_delnick(unsigned int nick_id)
{
	/* we now need to delete these nicks from every OperServ, Chanserv
	 * and nick suspension table */
	del_nicksuspend(nick_id);
	cs_mysql_remove_nick(nick_id);
	os_mysql_remove_nick(nick_id);
	mysql_remove_links(nick_id);
	ns_mysql_remove_access(nick_id);
	ns_mysql_remove_memos(nick_id);
	mysql_delete_nick(nick_id);
	return(1);
}

/* remove any links to this nick */
static void mysql_remove_links(unsigned int nick_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE nick SET link_id=0 "
				   "WHERE link_id=%u", nick_id);
	mysql_free_result(result);
}

/* Break a link from this nick to its parent */
static void delink(unsigned int nick_id)
{
	NickInfo ni, master_ni;
	MYSQL_ROW row;
	unsigned int fields, rows, master_memomax;
	MYSQL_RES *result;
	char *esc_nick;

	get_nickinfo_from_id(&ni, nick_id);

	if (!ni.link_id) {
		/* There is no link, this nick is a master, do nothing */
		free_nickinfo(&ni);
		return;
	}

	get_nickinfo_from_id(&master_ni, ni.link_id);

	esc_nick = smysql_escape_string(mysqlconn, master_ni.nick);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT max FROM memoinfo WHERE owner='%s'", esc_nick);

	row = smysql_fetch_row(mysqlconn, result);
	master_memomax = atoi(row[0]);

	mysql_free_result(result);
	free(esc_nick);

	if (debug) {
		log("breaking link between %s (%u) and its master, %s (%u)",
		    ni.nick, nick_id, master_ni.nick, master_ni.nick_id);
	}

	/* first remove the link */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE nick SET link_id=0 WHERE nick_id=%u", nick_id);
	mysql_free_result(result);
	
	/* now copy nick flags, channelmax and language from the master */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE nick SET flags=%d, channelmax=%u, language=%u "
	    "WHERE nick_id=%u", master_ni.flags, master_ni.channelmax,
	    master_ni.language, nick_id);
	mysql_free_result(result);

	/* keep our nickcache in sync */
	nickcache_set_language(nick_id, master_ni.language);

	/* now copy master's access list to scratch table */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO private_tmp_access (nick_id, userhost) "
	    "SELECT nick_id, userhost FROM nickaccess WHERE nick_id=%u",
	    master_ni.nick_id);
	mysql_free_result(result);

	if (rows) {
		/* now delete any access the linked nick has */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "DELETE FROM nickaccess WHERE nick_id=%u", nick_id);
		mysql_free_result(result);

		/* and finally copy it back */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "INSERT INTO nickaccess (nick_id, userhost) "
		    "SELECT %u, userhost FROM private_tmp_access "
		    "WHERE nick_id=%u", nick_id, master_ni.nick_id);
		mysql_free_result(result);

		/* and nuke the temp table */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "TRUNCATE TABLE private_tmp_access");
		mysql_free_result(result);
	}

	esc_nick = smysql_escape_string(mysqlconn, ni.nick);
	
	/* last thing to do is copy over the master's memomax */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE memoinfo SET max=%u WHERE owner='%s'", master_memomax,
	    esc_nick);
	mysql_free_result(result);
	free(esc_nick);
	free_nickinfo(&ni);
	free_nickinfo(&master_ni);
}

/* remove any nick access entries belonging to this user */
static void ns_mysql_remove_access(unsigned int nick_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "DELETE FROM nickaccess WHERE nick_id=%u",
				   nick_id);
	mysql_free_result(result);
}

/* remove the actual row from the nick table */
static void mysql_delete_nick(unsigned int nick_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "DELETE FROM nick WHERE nick_id=%u",
				   nick_id);
	mysql_free_result(result);
}

/* delete all memo info for a nick that is being deleted, including memos
 * that a nick owns */
static void ns_mysql_remove_memos(unsigned int nick_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	char *owner = get_nick_from_id(nick_id);
	char *esc_owner = smysql_escape_string(mysqlconn, owner);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "DELETE FROM memoinfo WHERE owner='%s'",
				   esc_owner);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "DELETE FROM memo WHERE owner='%s'",
				   esc_owner);
	mysql_free_result(result);

	free(esc_owner);
	free(owner);
}

/*************************************************************************/

/*************************************************************************/

/* Collide a nick. 
 *
 * When connected to a network using DALnet servers, version 4.4.15 and above, 
 * Services is able to force a nick change instead of killing the user. 
 * The new nick takes the form "Guest######". If a nick change is forced, we
 * do not introduce the enforcer nick until the user's nick actually changes. 
 * This is watched for and done in cancel_user(). 
 * 
 * The number that is used to build the "Guest" nick should be:
 * 	- Atomic
 * 	- Unique within a 24 hour period - basically this means that if someone
 * 	  has a "Guest" nick for more than 24 hours, there is a slight chance
 * 	  that a new "Guest" could be collided. I really don't see this as a 
 * 	  problem.
 * 	- Reasonably unpredictable
 * 
 * -TheShadow 
 */

static void collide(unsigned int nick_id, int from_timeout)
{
	User *u;
	char guestnick[NICKMAX];
	static uint32 counter = 0;
	char *nick = get_nick_from_id(nick_id);

	u = finduser(nick);

	if (!from_timeout)
		del_ns_timeout(nick_id, TO_COLLIDE);

	snprintf(guestnick, sizeof(guestnick), "%s%d",
		 NSGuestNickPrefix, counter);
	counter++;
	notice_lang(s_NickServ, u, FORCENICKCHANGE_NOW, guestnick);
	send_cmd(ServerName, "SVSNICK %s :%s", nick,
		 guestnick);

	free(nick);
	add_nick_status(nick_id, NS_GUESTED);
}

/*************************************************************************/

/* Release hold on a nick. */

static void release(unsigned int nick_id, int from_timeout)
{
	char *nick = get_nick_from_id(nick_id);
	
	if (!from_timeout)
		del_ns_timeout(nick_id, TO_RELEASE);
	send_cmd(nick, "QUIT");
	free(nick);
	
	remove_nick_status(nick_id, NS_KILL_HELD);
}

/*************************************************************************/

/*************************************************************************/

static struct my_timeout {
	struct my_timeout *next, *prev;
	unsigned int nick_id;
	Timeout *to;
	int type;
} *my_timeouts;

/*************************************************************************/

/* Remove a collide/release timeout from our private list. */

static void rem_ns_timeout(unsigned int nick_id, int type)
{
	struct my_timeout *t, *t2;

	t = my_timeouts;
	while (t) {
		if (t->nick_id == nick_id && t->type == type) {
			t2 = t->next;
			if (t->next)
				t->next->prev = t->prev;
			if (t->prev)
				t->prev->next = t->next;
			else
				my_timeouts = t->next;
			free(t);
			t = t2;
		} else {
			t = t->next;
		}
	}
}

/*************************************************************************/

/* Collide a nick on timeout. */

static void timeout_collide(Timeout * t)
{
	unsigned int nick_id = *(unsigned int *)t->data;
	User *u;
	char *nick = get_nick_from_id(nick_id);

	log("timeout_collide for %s (id %u) triggered at %d", nick,
	    nick_id, t->timeout);

	rem_ns_timeout(nick_id, TO_COLLIDE);
	/* If they identified or don't exist anymore, don't kill them. */
	if ((get_nick_status(nick_id) & NS_IDENTIFIED)
	    || !(u = finduser(nick))
	    || (u->my_signon > t->settime)) {
		free(nick);
		return;
	}

	free(nick);
	
	/* The RELEASE timeout will always add to the beginning of the
	 * list, so we won't see it.  Which is fine because it can't be
	 * triggered yet anyway. */
	collide(nick_id, 1);
}

/*************************************************************************/

/* Release a nick on timeout. */

static void timeout_release(Timeout * t)
{
	unsigned int nick_id = *(unsigned int *)t->data;

	rem_ns_timeout(nick_id, TO_RELEASE);
	release(nick_id, 1);
}

/*************************************************************************/

/* Add a collide/release timeout. */

void add_ns_timeout(unsigned int nick_id, int type, time_t delay)
{
	Timeout *to;
	struct my_timeout *t;
	void (*timeout_routine) (Timeout *);
	unsigned int *nnick_id = malloc(sizeof(unsigned int));

	*nnick_id = nick_id;

	if (type == TO_COLLIDE)
		timeout_routine = timeout_collide;
	else if (type == TO_RELEASE)
		timeout_routine = timeout_release;
	else {
		log
		    ("NickServ: unknown timeout type %d!  nick_id=%u, delay=%d",
		     type, nick_id, delay);
		return;
	}
	to = add_timeout(delay, timeout_routine, 0);
	to->data = nnick_id;
	t = smalloc(sizeof(*t));
	t->next = my_timeouts;
	my_timeouts = t;
	t->prev = NULL;
	t->nick_id = nick_id;
	t->to = to;
	t->type = type;
}

/*************************************************************************/

/* Delete a collide/release timeout. */

static void del_ns_timeout(unsigned int nick_id, int type)
{
	struct my_timeout *t, *t2;

	t = my_timeouts;
	while (t) {
		if (t->nick_id == nick_id && t->type == type) {
			t2 = t->next;
			if (t->next)
				t->next->prev = t->prev;
			if (t->prev)
				t->prev->next = t->next;
			else
				my_timeouts = t->next;
			free(t->to->data);
			del_timeout(t->to);
			free(t);
			t = t2;
		} else {
			t = t->next;
		}
	}
}

/*************************************************************************/

/*********************** NickServ command routines ***********************/

/*************************************************************************/

#ifdef DEBUG_COMMANDS

/* List NickServ commands */

static void do_commands(User *u)
{
	Command *c;
	char buf[50], *end;

	end = buf;
	*end = 0;
	for (c = cmds; c->name; c++) {
		if ((c->has_priv == NULL) || c->has_priv(u)) {
			if ((end - buf) + strlen(c->name) + 2 >= sizeof(buf)) {
				notice(s_NickServ, u->nick, "%s,", buf);
				end = buf;
				*end = 0;
			}
			end +=
			    snprintf(end, sizeof(buf) - (end - buf), "%s%s",
				     (end == buf) ? "" : ", ", c->name);
		}
	}
	if (buf[0])
		notice(s_NickServ, u->nick, "%s", buf);
}

#endif				/* DEBUG_COMMANDS */

/* Return a help message. */

static void do_help(User *u)
{
	char *cmd = strtok(NULL, "");

	notice_help(s_NickServ, u, NICK_HELP_START);

	if (!cmd) {
		if (NSExpire >= 86400)
			notice_help(s_NickServ, u, NICK_HELP,
				    NSExpire / 86400);
		else
			notice_help(s_NickServ, u, NICK_HELP_EXPIRE_ZERO);
		if (is_services_oper(u))
			notice_help(s_NickServ, u, NICK_SERVADMIN_HELP);

	} else if (stricmp(cmd, "SET LANGUAGE") == 0) {
		int i;

		notice_help(s_NickServ, u, NICK_HELP_SET_LANGUAGE);
		for (i = 0; i < NUM_LANGS && langlist[i] >= 0; i++) {
			notice(s_NickServ, u->nick, "    %2d) %s", i + 1,
			       langnames[langlist[i]]);
		}
	} else {
		help_cmd(s_NickServ, u, cmds, cmd);
	}
	notice_help(s_NickServ, u, NICK_HELP_END);
}

/*************************************************************************/

/* Register a nick. */

static void do_register(User *u)
{
	char cmdbuf[500], buf[4096], rstr[SALTMAX];
	char codebuf[PASSMAX + NICKMAX], code[PASSMAX];
	NickInfo ni;
	MemoInfo mi;
	MYSQL_RES *result;
	MYSQL_ROW row;
	FILE *fp;
	size_t prefixlen, nicklen;
	unsigned int fields, rows;
	char *pass, *email, *authcode, *esc_authcode;
	char *esc_name;
	const char *mailfmt;

	prefixlen = strlen(NSGuestNickPrefix);
	nicklen = strlen(u->nick);

	pass = strtok(NULL, " ");
	email = strtok(NULL, " ");

	/* Prevent "Guest" nicks from being registered. -TheShadow */

	/* A guest nick is defined as a nick...
	 *      - starting with NSGuestNickPrefix
	 *      - with a series of between, and including, 1 and 10 digits
	 */
	if (nicklen <= prefixlen + 10 && nicklen >= prefixlen + 1 &&
	    stristr(u->nick, NSGuestNickPrefix) == u->nick &&
	    strspn(u->nick + prefixlen, "1234567890") ==
	    nicklen - prefixlen) {
		notice_lang(s_NickServ, u, NICK_CANNOT_BE_REGISTERED,
			    u->nick);
		return;
	}

	if (pass && stricmp(pass, "AUTH") == 0) {
		authcode = email;

		if (!authcode) {
			syntax_error(s_NickServ, u, "REGISTER",
			    NICK_REGISTER_AUTH_SYNTAX);
			return;
		}

		esc_name = smysql_escape_string(mysqlconn, u->nick);
		esc_authcode = smysql_escape_string(mysqlconn, authcode);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT params FROM auth "
		    "WHERE command='NICK_REG' && code='%s' && name='%s'",
		    esc_authcode, esc_name);

		if ((row = smysql_fetch_row(mysqlconn, result))) {
			pass = strtok(row[0], " ");
			email = strtok(NULL, "");

			register_nick(&ni, &mi, u, pass, email);
		} else {
			notice_lang(s_NickServ, u, NICK_REG_NO_SUCH_AUTH);
		}
		
		free(esc_authcode);
		mysql_free_result(result);
		return;
	}

	if (!pass || (stricmp(pass, u->nick) == 0 && strtok(NULL, " "))) {
		if (NSRegNeedAuth) {
			syntax_error(s_NickServ, u, "REGISTER",
			    NICK_REGISTER_AUTH_SYNTAX);
		} else {
			syntax_error(s_NickServ, u, "REGISTER",
			    NICK_REGISTER_SYNTAX);
		}

	} else if (time(NULL) < u->lastnickreg + NSRegDelay) {
		notice_lang(s_NickServ, u, NICK_REG_PLEASE_WAIT, NSRegDelay);

	} else if (!email) {
		syntax_error(s_NickServ, u, "REGISTER",
		    NICK_REGISTER_SYNTAX);

	} else if (email && !validate_email(email)) {
		notice_lang(s_NickServ, u, BAD_EMAIL, email);
		syntax_error(s_NickServ, u, "REGISTER",
		    NICK_REGISTER_SYNTAX);

	} else if (u->real_id) {	/* i.e. there's already such a nick regged */
		if (get_nick_status(u->real_id) & NS_VERBOTEN) {
			log("%s: %s@%s tried to register FORBIDden nick %s",
			    s_NickServ, u->username, u->host, u->nick);
			notice_lang(s_NickServ, u, NICK_CANNOT_BE_REGISTERED,
				    u->nick);
		} else if (get_nick_flags(u->nick_id) & NI_SUSPENDED) {
			log("%s: %s@%s tried to register SUSPENDED nick %s",
			    s_NickServ, u->username, u->host, u->nick);
			notice_lang(s_NickServ, u, NICK_CANNOT_BE_REGISTERED,
				    u->nick);
		} else {
			notice_lang(s_NickServ, u, NICK_ALREADY_REGISTERED,
				    u->nick);
		}

	} else if (!validate_password(u, pass, NULL)) {
		notice_lang(s_NickServ, u, MORE_OBSCURE_PASSWORD);

	} else if (NSRegNeedAuth) {
		/*
		 * At this point we have their email address and their
		 * password, but first we need to check there is no
		 * existing AUTH code for this nick pending.
		 */
		esc_name = smysql_escape_string(mysqlconn, u->nick);
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT 1 FROM auth WHERE command='NICK_REG' && "
		    "name='%s'", esc_name);
		
		mysql_free_result(result);
		
		if (rows) {
			notice_lang(s_NickServ, u,
			    NICK_ALREADY_PART_REGISTERED, u->nick);
			return;
		}
		
		/*
		 * All OK, so now we need to insert them into the auth
		 * table and send them a mail with an AUTH code in it.
		 */
	
		/*
		 * We need to make an AUTH code now.  It needs to be
		 * non-guessable and unique.  I figure a SHA1 hash of the
		 * user's nick and a random string will do it.
		 */
		memset(codebuf, 0, sizeof(codebuf));
		make_random_string(rstr, sizeof(rstr));
		strncat(codebuf, u->nick, NICKMAX);
		strncat(codebuf, rstr, PASSMAX);
		make_sha_hash(codebuf, code);

		/*
		 * We can't make this multilingual because their nick isn't
		 * registered yet :(
		 */
		mailfmt = langtexts[DEF_LANGUAGE][NICK_REG_AUTH_MAIL];

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "INSERT INTO auth SET auth_id='NULL', "
		    "code='%s', name='%s', command='NICK_REG', "
		    "params='%s %s', create_time=%d", code, esc_name,
		    pass, email, time(NULL));

		free(esc_name);

		mysql_free_result(result);
		
		snprintf(cmdbuf, sizeof(cmdbuf), "%s -t", SendmailPath);
		snprintf(buf, sizeof(buf), mailfmt, NSMyEmail, u->nick,
		    email, AbuseEmail, NSMyEmail, u->nick, code,
		    AbuseEmail, NetworkName);

		log("%s: %s!%s@%s used REGISTER, sending AUTH code",
		    s_NickServ, u->nick, u->username, u->host);
		snoop(s_NickServ,
		    "[REGISTER] %s (%s@%s) used REGISTER, sending AUTH",
		    u->nick, u->username, u->host);

		if ((fp = popen(cmdbuf, "w")) == NULL) {
			log("%s: Failed to create pipe for AUTH mail to %s",
			    s_NickServ, email);
			snoop(s_NickServ,
			    "[REGISTER] popen() failed sending mail to %s",
			    email);
			return;
		}

		fputs(buf, fp);
		pclose(fp);
		notice_lang(s_NickServ, u, NICK_REG_AUTH_MAILED, email);
	} else {
		register_nick(&ni, &mi, u, pass, email);
	}
}

/*************************************************************************/

static void do_identify(User *u)
{
	char *pass = strtok(NULL, " ");
	int res;

	if (!pass) {
		syntax_error(s_NickServ, u, "IDENTIFY", NICK_IDENTIFY_SYNTAX);

	} else if (!u->real_id) {
		notice(s_NickServ, u->nick, "Your nick isn't registered.");

	} else if (get_nick_flags(u->real_id) & NI_SUSPENDED) {
		notice_lang(s_NickServ, u, NICK_X_SUSPENDED, u->nick);

	} else if (!(res = check_nick_password(pass, u->real_id))) {
		log("%s: Failed IDENTIFY for %s!%s@%s", s_NickServ, u->nick,
		    u->username, u->host);
		notice_lang(s_NickServ, u, PASSWORD_INCORRECT);
		if (!BadPassLimit) {
			snoop(s_NickServ,
			      "[BADPASS] %s (%s@%s) Failed IDENTIFY",
			      u->nick, u->username, u->host);
		} else {
			snoop(s_NickServ,
			      "[BADPASS] %s (%s@%s) Failed IDENTIFY (%d of %d)",
			      u->nick, u->username, u->host,
			      u->invalid_pw_count + 1, BadPassLimit);
		}
		bad_password(u);

	} else if (res == -1) {
		notice_lang(s_NickServ, u, NICK_IDENTIFY_FAILED);

	} else {
		mysql_update_last_seen(u, NS_IDENTIFIED);

		/* Update the nick cache now. */
		nickcache_update(u->real_id);
		
        send_cmd(ServerName, "SVSMODE %s :+R", u->nick);
		log("%s: %s!%s@%s identified for nick %s", s_NickServ,
		    u->nick, u->username, u->host, u->nick);
		notice_lang(s_NickServ, u, NICK_IDENTIFY_SUCCEEDED);
		snoop(s_NickServ, "[ID] %s (%s@%s)", u->nick,
			u->username, u->host);
		if (is_oper(u->nick) &&
		    !(get_nick_flags(u->nick_id) & NI_IRCOP)) {
			notice_lang(s_NickServ, u, NICK_NOT_IRCOP,
				    s_NickServ);
		}

		if (strlen(get_email_from_id(u->real_id)) <= 1) {
			notice_lang(s_NickServ, u, NICK_BEG_SET_EMAIL,
				    s_NickServ);
		}

		if (!(get_nick_status(u->real_id) & NS_RECOGNIZED))
			check_memos(u);

        if(!(get_nick_status(u->real_id) & NS_RECOGNIZED)
          && (get_nick_flags(u->real_id)) & NI_CLOAK) {
            char *cloak = get_cloak_from_id(u->real_id);
            if(cloak && *cloak != '\0')
              send_cmd(ServerName, "SVSCLOAK %s :%s", u->nick, cloak);
            free(cloak);
        }

        check_autojoins(u);
	}
}

/*************************************************************************/

static void do_drop(User *u)
{
	char buf[4096], cmdbuf[500], rstr[SALTMAX];
	char codebuf[PASSMAX + NICKMAX], code[PASSMAX];
	User *u2;
	MYSQL_RES *result;
	FILE *fp;
	unsigned int nick_id, fields, rows;
	char *nick, *dropped_nick, *authcode, *esc_authcode, *email;
	char *esc_name;
	const char *mailfmt;

	nick = strtok(NULL, " ");

	if (nick && stricmp(nick, "AUTH") == 0) {
		authcode = strtok(NULL, " ");

		if (!authcode) {
			syntax_error(s_NickServ, u, "DROP",
			    NICK_DROP_SYNTAX);
			return;
		}

		esc_name = smysql_escape_string(mysqlconn, u->nick);
		esc_authcode = smysql_escape_string(mysqlconn, authcode);

		/* DROP has no params */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT 1 FROM auth WHERE "
		    "command='NICK_DROP' && code='%s' && name='%s'",
		    esc_authcode, esc_name);
		mysql_free_result(result);
		
		if (rows) {
			nick_id = u->real_id;
			dropped_nick = get_nick_from_id(nick_id);
			u->real_id = u->nick_id = 0;

			send_cmd(ServerName, "SVSMODE %s :-R", dropped_nick);
			mysql_delnick(nick_id);
			log("%s: %s!%s@%s dropped nickname %s", s_NickServ,
			    u->nick, u->username, u->host, dropped_nick);
			snoop(s_NickServ, "[DROP] %s (%s@%s) dropped %s",
			    u->nick, u->username, u->host, dropped_nick);
			notice_lang(s_NickServ, u, NICK_DROPPED);
			free(dropped_nick);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "DELETE FROM auth WHERE code='%s' && "
			    "command='NICK_DROP' && name='%s'",
			    esc_authcode, esc_name);
			mysql_free_result(result);
		} else {
			notice_lang(s_NickServ, u, NICK_DROP_NO_SUCH_AUTH);
		}

		free(esc_name);
		free(esc_authcode);
		return;
	}

	if (nick)
		nick_id = mysql_findnick(nick);
	else
		nick_id = u->real_id;

	if (nick && !is_services_admin(u)) {
		syntax_error(s_NickServ, u, "DROP", NICK_DROP_SYNTAX);

	} else if (!nick_id) {
		if (nick)
			notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED,
				    nick);
		else
			notice_lang(s_NickServ, u, NICK_NOT_REGISTERED);

	} else if (NSSecureAdmins && nick && nick_is_services_admin(nick_id)
		   && !is_services_root(u)) {
		notice_lang(s_NickServ, u, PERMISSION_DENIED);

	} else if (!nick && !nick_identified(u)) {
		notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);

	} else if (nick) {
		/* We allow Services Admins to drop nicks immediately */
		char *dropped_nick = get_nick_from_id(nick_id);

		send_cmd(ServerName, "SVSMODE %s :-R", dropped_nick);
		mysql_delnick(nick_id);
		log("%s: %s!%s@%s dropped nickname %s", s_NickServ,
		    u->nick, u->username, u->host, dropped_nick);
		snoop(s_NickServ, "[DROP] %s (%s@%s) dropped %s", u->nick,
		    u->username, u->host, dropped_nick);
		notice_lang(s_NickServ, u, NICK_X_DROPPED, nick);
		wallops(s_NickServ,
		    "\2%s\2 DROPped nick \2%s\2 as services admin",
		    u->nick, dropped_nick);

		free(dropped_nick);
		
		if ((u2 = finduser(nick)))
			u2->nick_id = u2->real_id = 0;
	} else {
		/*
		 * At this point they are ready to DROP, we need to check
		 * there is no pending DROP though.
		 */
		esc_name = smysql_escape_string(mysqlconn, u->nick);
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT 1 FROM auth "
		    "WHERE command='NICK_DROP' && name='%s'", esc_name);

		mysql_free_result(result);

		if (rows) {
			notice_lang(s_NickServ, u,
			    NICK_ALREADY_PART_DROPPED, u->nick);
			return;
		}

		/*
		 * All OK, so now we need to insert them into the auth
		 * table and send them a mail with an AUTH code in it.
		 */

		/*
		 * We need to make an AUTH code now.  It needs to be
		 * non-guessable and unique.  I figure a SHA1 hash of the
		 * user's nick and a random string will do it.
		 */
		memset(codebuf, 0, sizeof(codebuf));
		make_random_string(rstr, sizeof(rstr));
		strncat(codebuf, u->nick, NICKMAX);
		strncat(codebuf, rstr, PASSMAX);
		make_sha_hash(codebuf, code);

		mailfmt = getstring(u->nick_id, NICK_DROP_AUTH_MAIL);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "INSERT INTO auth SET auth_id='NULL', "
		    "code='%s', name='%s', command='NICK_DROP', "
		    "params='', create_time=%d", code, esc_name,
		    time(NULL));

		free(esc_name);

		mysql_free_result(result);

		snprintf(cmdbuf, sizeof(cmdbuf), "%s -t", SendmailPath);
		
		email = get_email_from_id(u->real_id);

		snprintf(buf, sizeof(buf), mailfmt, NSMyEmail, u->nick,
		    email, AbuseEmail, NSMyEmail, u->nick, code,
		    AbuseEmail, NetworkName);


		log("%s: %s!%s@%s used DROP, sending AUTH code",
		    s_NickServ, u->nick, u->username, u->host);
		snoop(s_NickServ,
		    "[DROP] %s (%s@%s) used DROP, sending AUTH", u->nick,
		    u->username, u->host);

		if ((fp = popen(cmdbuf, "w")) == NULL) {
			log("%s: Failed to create pipe for AUTH mail to %s",
			    s_NickServ, email);
			snoop(s_NickServ,
			    "[REGISTER] popen() failed sending mail to %s",
			    email);
			free(email);
			return;
		}

		fputs(buf, fp);
		pclose(fp);
		notice_lang(s_NickServ, u, NICK_DROP_AUTH_MAILED, email);
		free(email);
	}
}

/*************************************************************************/

static void do_set(User *u)
{
	char *cmd, *param, *nick;
	int is_servadmin, set_nick;
	unsigned int nick_id;

	cmd = strtok(NULL, " ");
	param = strtok(NULL, " ");

	is_servadmin = is_services_admin(u);

	set_nick = 0;

	if (is_servadmin && cmd && (nick_id = mysql_findnick(cmd))) {
		nick = get_nick_from_id(nick_id);
		cmd = param;
		param = strtok(NULL, " ");
		set_nick = 1;

		if (cmd) {
			if (param) {
				log("%s: SET: %s SET %s %s on nick %s as "
				    "services admin", s_NickServ, u->nick,
				    cmd, param, nick);
				snoop(s_NickServ, "[SET] %s issued command "
				    "\"SET %s %s\" on nick %s as services "
				    "admin", u->nick, cmd, param, nick);
				wallops(s_NickServ, "%s issued command "
				    "%s SET %s %s on nick %s as "
				    "services admin", u->nick, s_NickServ,
				    cmd, param, nick);
			} else {
				log("%s: SET: %s SET %s on nick %s as "
				    "services admin", s_NickServ, u->nick,
				    cmd, nick);
				snoop(s_NickServ, "[SET] %s issued command "
				    "SET %s on nick %s as services "
				    "admin", u->nick, cmd, nick);
				wallops(s_NickServ, "%s issued command "
				    "%s SET %s on nick %s as services "
				    "admin", u->nick, s_NickServ, cmd,
				    nick);
			}
		}
	} else {
		nick_id = u->nick_id;
		nick = get_nick_from_id(nick_id);
	}

	if (!param && (!cmd || (stricmp(cmd, "URL") != 0 &&
	    stricmp(cmd, "EMAIL") != 0))) {
		if (is_servadmin) {
			syntax_error(s_NickServ, u, "SET",
			    NICK_SET_SERVADMIN_SYNTAX);
		} else {
			syntax_error(s_NickServ, u, "SET", NICK_SET_SYNTAX);
		}
	} else if (!nick_id) {
		notice_lang(s_NickServ, u, NICK_NOT_REGISTERED);
	} else if (get_nick_status(nick_id) & NS_VERBOTEN) {
		notice_lang(s_NickServ, u, NICK_X_FORBIDDEN, nick);
	} else if (!is_servadmin && !nick_identified(u)) {
		notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);
	} else if (stricmp(cmd, "PASSWORD") == 0 || 
	    stricmp(cmd, "PASS") == 0) {
		do_set_password(u, set_nick ? nick_id : u->real_id, param);
	} else if (stricmp(cmd, "LANGUAGE") == 0) {
		do_set_language(u, nick_id, param);
	} else if (stricmp(cmd, "URL") == 0) {
    do_set_url(u, set_nick ? nick_id : u->real_id, param);
  } else if (stricmp(cmd, "EMAIL") == 0) {
    do_set_email(u, set_nick ? nick_id : u->real_id, param);
  } else if (stricmp(cmd, "CLOAKSTRING") == 0) {
    if(!is_servadmin) {
      notice_lang(s_NickServ, u, PERMISSION_DENIED);
    } else {
      do_set_cloakstring(u, set_nick ? nick_id : u->real_id, param);
    }
  } else if (stricmp(cmd, "ENFORCE") == 0) {
    do_set_enforce(u, nick_id, param);
  } else if (stricmp(cmd, "SECURE") == 0) {
    do_set_secure(u, nick_id, param);
	} else if (stricmp(cmd, "PRIVATE") == 0) {
		do_set_private(u, nick_id, param);
	} else if (stricmp(cmd, "HIDE") == 0) {
    do_set_hide(u, nick_id, param);
  } else if (stricmp(cmd, "CLOAK") == 0) {
    do_set_cloak(u, nick_id, param);
  } else if (stricmp(cmd, "NOEXPIRE") == 0) {
		do_set_noexpire(u, nick_id, param);
	} else if (stricmp(cmd, "NOOP") == 0) {
		do_set_noop(u, nick_id, param);
	} else if (stricmp(cmd, "IRCOP") == 0) {
		do_set_ircop(u, nick_id, param);
	} else if (stricmp(cmd, "MARK") == 0) {
		do_set_mark(u, nick_id, param);
	} else if (stricmp(cmd, "REGTIME") == 0) {
		do_set_regtime(u, nick_id, param);
	} else if (stricmp(cmd, "SENDPASS") == 0) {
		do_set_sendpass(u, nick_id, param);
	} else if (stricmp(cmd, "AUTOJOIN") == 0) {
		do_set_autojoin(u, nick_id, param);
	} else {
		if (is_servadmin)
			notice_lang(s_NickServ, u,
			    NICK_SET_UNKNOWN_OPTION_OR_BAD_NICK,
			    strupper(cmd));
		else
			notice_lang(s_NickServ, u, NICK_SET_UNKNOWN_OPTION,
			    strupper(cmd));
	}
	free(nick);
}

/*************************************************************************/

static void do_set_password(User *u, unsigned int nick_id, char *param)
{
	NickInfo ni;
	char salt[SALTMAX];
	MYSQL_RES *result;
	int len;
	unsigned int fields, rows;
    char *esc_pass;

    esc_pass = smysql_escape_string(mysqlconn, param);
	len = strlen(esc_pass);
	
	get_nickinfo_from_id(&ni, nick_id);

	if (NSSecureAdmins && u->real_id != nick_id &&
	    !is_services_root(u) && nick_is_services_admin(nick_id)) {
		notice_lang(s_NickServ, u, PERMISSION_DENIED);
		free_nickinfo(&ni);
		return;
	} else if ((ni.flags & NI_MARKED) &&
	    !(is_services_root(u) || is_services_admin(u))) {
		notice_lang(s_NickServ, u, NICK_X_MARKED, ni.nick);
		free_nickinfo(&ni);
		return;
	} else if (!validate_password(u, esc_pass, NULL)) {
		notice_lang(s_NickServ, u, MORE_OBSCURE_PASSWORD);
		free_nickinfo(&ni);
		return;
	}

	if (len > PASSMAX - 1)	/* -1 for null byte */
		notice_lang(s_NickServ, u, PASSWORD_TRUNCATED, PASSMAX - 1);
	strscpy(ni.pass, esc_pass, PASSMAX);
	notice_lang(s_NickServ, u, NICK_SET_PASSWORD_CHANGED_TO, ni.pass);

	if (u->real_id != nick_id) {
		log("%s: %s!%s@%s used SET PASSWORD as Services admin on %s",
		    s_NickServ, u->nick, u->username, u->host, ni.nick);
		if (WallSetpass) {
			wallops(s_NickServ,
			    "\2%s\2 used SET PASSWORD as Services admin "
			    "on \2%s\2", u->nick, ni.nick);
		}
	}

	make_random_string(salt, sizeof(salt));
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE nick SET pass=SHA1('%s%s'), salt='%s' WHERE nick_id=%u",
	    ni.pass, salt, salt, nick_id);
	mysql_free_result(result);
	free_nickinfo(&ni);
}

/*************************************************************************/

static void do_set_language(User *u, unsigned int nick_id, char *param)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	int langnum;

	if (param[strspn(param, "0123456789")] != 0) {	/* i.e. not a number */
		syntax_error(s_NickServ, u, "SET LANGUAGE",
			     NICK_SET_LANGUAGE_SYNTAX);
		return;
	}
	langnum = atoi(param) - 1;
	if (langnum < 0 || langnum >= NUM_LANGS || langlist[langnum] < 0) {
		notice_lang(s_NickServ, u, NICK_SET_LANGUAGE_UNKNOWN,
			    langnum + 1, s_NickServ);
		return;
	}
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE nick SET language=%u "
				   "WHERE nick_id=%u", langlist[langnum],
				   nick_id);
	mysql_free_result(result);

	/* keep our nickcache in sync */
	nickcache_set_language(nick_id, (unsigned int)langlist[langnum]);
	
	notice_lang(s_NickServ, u, NICK_SET_LANGUAGE_CHANGED);
}

/*************************************************************************/

static void do_set_url(User *u, unsigned int nick_id, char *param)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	char *esc_url = smysql_escape_string(mysqlconn, (param? param : ""));

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE nick SET url='%s' "
				   "WHERE nick_id=%u", esc_url, nick_id);
	mysql_free_result(result);
	free(esc_url);

	if (param) {
		notice_lang(s_NickServ, u, NICK_SET_URL_CHANGED, param);
	} else {
		notice_lang(s_NickServ, u, NICK_SET_URL_UNSET);
	}
}

/*************************************************************************/

static void do_set_email(User *u, unsigned int nick_id, char *param)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows;
	char *current_email;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT email FROM nick WHERE nick_id=%u", nick_id);
	if (rows) {
		row = smysql_fetch_row(mysqlconn, result);
		current_email = sstrdup(row[0]);
	} else {
		current_email = NULL;
	}

	mysql_free_result(result);

	if (param) {
		if (!validate_email(param)) {
			notice_lang(s_NickServ, u, BAD_EMAIL, param);

			if (current_email && *current_email) {
				notice_lang(s_NickServ, u,
				    NICK_SET_EMAIL_UNCHANGED, current_email);
			}
		} else {
			char *esc_email = smysql_escape_string(mysqlconn, param);
			
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "UPDATE nick SET email='%s' "
			    "WHERE nick_id=%u", esc_email, nick_id);
			mysql_free_result(result);
			free(esc_email);

			notice_lang(s_NickServ, u, NICK_SET_EMAIL_CHANGED,
			    param, s_NickServ);
		}
	} else {
		notice_lang(s_NickServ, u, NICK_SET_EMAIL_MANDATORY,
		    s_NickServ);
		if (current_email && *current_email) {
			notice_lang(s_NickServ, u,
			    NICK_SET_EMAIL_UNCHANGED, current_email);
		}
	}

	if (current_email)
		free(current_email);
}

static void do_set_cloakstring(User *u, unsigned int nick_id, char *param)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    unsigned int fields, rows;
    char *current_cloak;
    char *nick;

    result = smysql_bulk_query(mysqlconn, &fields, &rows, "SELECT "
            "cloak_string FROM nick WHERE nick_id=%u", nick_id);
    if (rows) {
        row = smysql_fetch_row(mysqlconn, result);
        current_cloak = sstrdup(row[0]);
    } else {
        current_cloak = NULL;
    }

    mysql_free_result(result);

    if (param) {
        char *esc_cloak = smysql_escape_string(mysqlconn, param);
        result = smysql_bulk_query(mysqlconn, &fields, &rows,
            "UPDATE nick SET cloak_string='%s' WHERE nick_id=%u", esc_cloak, nick_id);
        mysql_free_result(result);
        free(esc_cloak);

        nick = get_nick_from_id(u->nick_id);
        notice_lang(s_NickServ, u, NICK_SET_CLOAKSTRING, nick, param);
        free(nick);
    } else {
        syntax_error(s_NickServ, u, "SET CLOAKSTRING",
            NICK_SET_CLOAKSTRING_SYNTAX);
    }

    if (current_cloak)
        free(current_cloak);
}

/*************************************************************************/

static void do_set_enforce(User *u, unsigned int nick_id, char *param)
{
	if (stricmp(param, "ON") == 0) {
		add_nick_flags(nick_id, NI_ENFORCE);
		remove_nick_flags(nick_id, NI_ENFORCEQUICK);
		notice_lang(s_NickServ, u, NICK_SET_ENFORCE_ON);
	} else if (stricmp(param, "QUICK") == 0) {
		add_nick_flags(nick_id, (NI_ENFORCE | NI_ENFORCEQUICK));
		notice_lang(s_NickServ, u, NICK_SET_ENFORCE_QUICK);
	} else if (stricmp(param, "OFF") == 0) {
		remove_nick_flags(nick_id, (NI_ENFORCE | NI_ENFORCEQUICK));
		notice_lang(s_NickServ, u, NICK_SET_ENFORCE_OFF);
	} else {
		syntax_error(s_NickServ, u, "SET ENFORCE",
			     NICK_SET_ENFORCE_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_secure(User *u, unsigned int nick_id, char *param)
{
	if (stricmp(param, "ON") == 0) {
		add_nick_flags(nick_id, NI_SECURE);
		notice_lang(s_NickServ, u, NICK_SET_SECURE_ON);
	} else if (stricmp(param, "OFF") == 0) {
		remove_nick_flags(nick_id, NI_SECURE);
		notice_lang(s_NickServ, u, NICK_SET_SECURE_OFF);
	} else {
		syntax_error(s_NickServ, u, "SET SECURE",
			     NICK_SET_SECURE_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_private(User *u, unsigned int nick_id, char *param)
{
	if (stricmp(param, "ON") == 0) {
		add_nick_flags(nick_id, NI_PRIVATE);
		notice_lang(s_NickServ, u, NICK_SET_PRIVATE_ON);
	} else if (stricmp(param, "OFF") == 0) {
		remove_nick_flags(nick_id, NI_PRIVATE);
		notice_lang(s_NickServ, u, NICK_SET_PRIVATE_OFF);
	} else {
		syntax_error(s_NickServ, u, "SET PRIVATE",
			     NICK_SET_PRIVATE_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_hide(User *u, unsigned int nick_id, char *param)
{
  unsigned int onmsg, offmsg;
  int flag;

    if(stricmp(param, "EMAIL") == 0) {
        flag = NI_HIDE_EMAIL;
        onmsg = NICK_SET_HIDE_EMAIL_ON;
        offmsg = NICK_SET_HIDE_EMAIL_OFF;
    } else if (stricmp(param, "USERMASK") == 0) {
            flag = NI_HIDE_MASK;
            onmsg = NICK_SET_HIDE_MASK_ON;
            offmsg = NICK_SET_HIDE_MASK_OFF;
    } else if (stricmp(param, "QUIT") == 0) {
            flag = NI_HIDE_QUIT;
            onmsg = NICK_SET_HIDE_QUIT_ON;
            offmsg = NICK_SET_HIDE_QUIT_OFF;
    } else {
        syntax_error(s_NickServ, u, "SET HIDE",
            NICK_SET_HIDE_SYNTAX);
        return;
    }
    param = strtok(NULL, " ");
    if(!param) {
        syntax_error(s_NickServ, u, "SET HIDE",
            NICK_SET_HIDE_SYNTAX);
    } else if (stricmp(param, "ON") == 0) {
        add_nick_flags(nick_id, flag);
        notice_lang(s_NickServ, u, onmsg, s_NickServ);
    } else if (stricmp(param, "OFF") == 0) {
        remove_nick_flags(nick_id, flag);
        notice_lang(s_NickServ, u, offmsg, s_NickServ);
    } else {
        syntax_error(s_NickServ, u, "SET HIDE",
            NICK_SET_HIDE_SYNTAX);
    }
}

static void do_set_cloak(User *u, unsigned int nick_id, char *param)
{
    char *nick = get_nick_from_id(nick_id);
    char *cloak = get_cloak_from_id(nick_id);
    char newcloak[512];

    if (stricmp(param, "ON") == 0) {
      if(!is_services_admin(u))
      {
        notice_lang(s_NickServ, u, ACCESS_DENIED);
        return;
      }
        add_nick_flags(nick_id, NI_CLOAK);
        // Check for old cloakstring, if none exists: set nick.DefaultCloak
        // --mc
        if ( (! cloak) || (*cloak == '\0') ) {
            snprintf(newcloak, sizeof(newcloak)-1, "%s.%s", nick, DefaultCloak);
            do_set_cloakstring(u, nick_id, newcloak);
        }
        notice_lang(s_NickServ, u, NICK_SET_CLOAK_ON, nick);
    } else if (stricmp(param, "OFF") == 0) {
        remove_nick_flags(nick_id, NI_CLOAK);
        notice_lang(s_NickServ, u, NICK_SET_CLOAK_OFF, nick);
    } else {
        syntax_error(s_NickServ, u, "SET CLOAK",
            NICK_SET_CLOAK_SYNTAX);
    }
    free(nick);
    free(cloak);
}
/*************************************************************************/

static void do_set_noexpire(User *u, unsigned int nick_id, char *param)
{
	char *nick;

	if (!is_services_admin(u)) {
		notice_lang(s_NickServ, u, PERMISSION_DENIED);
		return;
	}
	if (!param) {
		syntax_error(s_NickServ, u, "SET NOEXPIRE",
			     NICK_SET_NOEXPIRE_SYNTAX);
		return;
	}

	nick = get_nick_from_id(nick_id);

	if (stricmp(param, "ON") == 0) {
		add_nick_status(nick_id, NS_NO_EXPIRE);
		notice_lang(s_NickServ, u, NICK_SET_NOEXPIRE_ON, nick);
		log("NOEXPIRE: %s set NOEXPIRE ON for %s", u->nick, nick);
		snoop(s_NickServ,
		      "[NOEXPIRE] %s set NOEXPIRE ON for %s",
		      u->nick, nick);
		wallops(s_NickServ, "%s set NOEXPIRE ON for %s",
			u->nick, nick);
	} else if (stricmp(param, "OFF") == 0) {
		remove_nick_status(nick_id, NS_NO_EXPIRE);
		notice_lang(s_NickServ, u, NICK_SET_NOEXPIRE_OFF, nick);
		log("NOEXPIRE: %s set NOEXPIRE OFF for %s", u->nick, nick);
		snoop(s_NickServ,
		      "[NOEXPIRE] %s set NOEXPIRE OFF for %s",
		      u->nick, nick);
		wallops(s_NickServ, "%s set NOEXPIRE OFF for %s",
			u->nick, nick);
	} else {
		syntax_error(s_NickServ, u, "SET NOEXPIRE",
			     NICK_SET_NOEXPIRE_SYNTAX);
	}

	free(nick);
}

/*************************************************************************/

static void do_set_noop(User *u, unsigned int nick_id, char *param)
{
	char *nick = get_nick_from_id(nick_id);

	if (stricmp(param, "ON") == 0) {
		add_nick_flags(nick_id, NI_NOOP);
		notice_lang(s_NickServ, u, NICK_SET_NOOP_ON, nick);
	} else if (stricmp(param, "OFF") == 0) {
		remove_nick_flags(nick_id, NI_NOOP);
		notice_lang(s_NickServ, u, NICK_SET_NOOP_OFF, nick);
	} else {
		syntax_error(s_NickServ, u, "SET NOOP",
			     NICK_SET_NOOP_SYNTAX);
	}
	free(nick);
}

/*************************************************************************/

static void do_set_ircop(User *u, unsigned int nick_id, char *param)
{
	if (! is_oper(u->nick) && ! is_services_admin(u)) {
		notice_lang(s_NickServ, u, ACCESS_DENIED);
	} else if (stricmp(param, "ON") == 0) {
		char *email = get_email_from_id(nick_id);

		if (!(*email))
			notice_lang(s_NickServ, u, NEED_EMAIL,
				    "SET IRCOP ON");
		else {
			char *nick = get_nick_from_id(nick_id);

			add_nick_flags(nick_id, NI_IRCOP);
			notice_lang(s_NickServ, u, NICK_SET_IRCOP_ON, nick);
			free(nick);
		}

		free(email);
	} else if (stricmp(param, "OFF") == 0) {
		char *nick = get_nick_from_id(nick_id);
		
		remove_nick_flags(nick_id, NI_IRCOP);
		notice_lang(s_NickServ, u, NICK_SET_IRCOP_OFF, nick);
		free(nick);
	} else {
		syntax_error(s_NickServ, u, "SET IRCOP",
			     NICK_SET_IRCOP_SYNTAX);
	}
}
		
/*************************************************************************/

static void do_set_mark(User *u, unsigned int nick_id, char *param)
{
	if (! is_services_admin(u)) {
		notice_lang(s_NickServ, u, ACCESS_DENIED);
	} else if (stricmp(param, "ON") == 0) {
		char *nick = get_nick_from_id(nick_id);
		
		add_nick_flags(nick_id, NI_MARKED);
		notice_lang(s_NickServ, u, NICK_SET_MARK_ON, nick);
		free(nick);
	} else if (stricmp(param, "OFF") == 0) {
		char *nick = get_nick_from_id(nick_id);
		
		remove_nick_flags(nick_id, NI_MARKED);
		notice_lang(s_NickServ, u, NICK_SET_MARK_OFF, nick);
		free(nick);
	} else {
		syntax_error(s_NickServ, u, "SET MARK",
			     NICK_SET_MARK_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_regtime(User *u, unsigned int nick_id, char *param)
{
	struct tm regtime;
	time_t epoch_regtime, epoch_now;
	char *nick;
	
	if (! is_services_admin(u)) {
		notice_lang(s_NickServ, u, ACCESS_DENIED);
		return;
	}

	if (! param || strptime(param, "%Y-%m-%d-%H-%M-%S",
				&regtime) == NULL) {
		syntax_error(s_NickServ, u, "SET REGTIME",
			     NICK_SET_REGTIME_SYNTAX);
		return;
	}
	
	epoch_now = time(NULL);
	epoch_regtime = mktime(&regtime);

	if (epoch_regtime == -1 || epoch_regtime >= epoch_now) {
		syntax_error(s_NickServ, u, "SET REGTIME",
			     NICK_SET_REGTIME_SYNTAX);
		return;
	}

	nick = get_nick_from_id(nick_id);
	set_nick_regtime(nick_id, epoch_regtime);

	snoop(s_NickServ, "[REGTIME] %s altered nick registration time for "
	      "%s to %s", u->nick, nick, param);
	wallops(s_NickServ, "[REGTIME] %s altered nick registration time "
		"for %s to %s", u->nick, nick, param);
	log("%s, %s altered nick registration time for %s to %s", s_NickServ,
	    u->nick, nick, param);
	notice_lang(s_NickServ, u, NICK_SET_REGTIME, nick, param);
	free(nick);
}

/*************************************************************************/

static void do_set_sendpass(User *u, unsigned int nick_id, char *param)
{
	char *nick;

	nick = get_nick_from_id(nick_id);
	
	/* note the logic of the flags is backwards here */
	if (stricmp(param, "ON") == 0) {
		remove_nick_flags(nick_id, NI_NOSENDPASS);
		notice_lang(s_NickServ, u, NICK_SET_SENDPASS_ON, nick);
	} else if (stricmp(param, "OFF") == 0) {
		add_nick_flags(nick_id, NI_NOSENDPASS);
		notice_lang(s_NickServ, u, NICK_SET_SENDPASS_OFF, nick);
	} else {
		syntax_error(s_NickServ, u, "SET SENDPASS",
		    NICK_SET_SENDPASS_SYNTAX);
	}

	free(nick);
}

/*************************************************************************/

static void do_set_autojoin(User *u, unsigned int nick_id, char *param)
{
	char *nick;

	nick = get_nick_from_id(nick_id);
	
	if (stricmp(param, "ON") == 0) {
		add_nick_flags(nick_id, NI_AUTOJOIN);
		notice_lang(s_NickServ, u, NICK_SET_AUTOJOIN_ON, nick);
	} else if (stricmp(param, "OFF") == 0) {
		remove_nick_flags(nick_id, NI_AUTOJOIN);
		notice_lang(s_NickServ, u, NICK_SET_AUTOJOIN_OFF, nick);
	} else {
		syntax_error(s_NickServ, u, "SET AUTOJOIN",
		    NICK_SET_AUTOJOIN_SYNTAX);
	}

	free(nick);
}

/*************************************************************************/

static void do_access(User *u)
{
	char *cmd = strtok(NULL, " ");
	char *mask = strtok(NULL, " ");
	unsigned int nick_id = 0;
	MYSQL_RES *result;
	unsigned int fields, rows;

	if (cmd && stricmp(cmd, "LIST") == 0 && mask && is_services_admin(u)
	    && (nick_id = mysql_findnick(mask))) {
		MYSQL_ROW row;

		notice_lang(s_NickServ, u, NICK_ACCESS_LIST_X, mask);
		mask = strtok(NULL, " ");

		if (mask) {
			char *esc_mask = smysql_escape_string(mysqlconn,
							      mask);
			
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT idx, userhost FROM nickaccess "
			    "WHERE nick_id=%u && "
			    "IRC_MATCH('%s', userhost) ORDER BY idx",
			    nick_id, esc_mask);
			
			while ((row = smysql_fetch_row(mysqlconn, result))) {
				notice(s_NickServ, u->nick, "%-4s %s", row[0], row[1]);
			}

			mysql_free_result(result);
			free(esc_mask);
		} else {
			result = smysql_bulk_query(mysqlconn, &fields,
						   &rows,
						   "SELECT idx, userhost "
						   "FROM nickaccess "
						   "WHERE nick_id=%u "
						   "ORDER BY idx",
						   nick_id);

			while ((row = smysql_fetch_row(mysqlconn, result))) {
				notice(s_NickServ, u->nick, "%-4s %s", row[0], row[1]);
			}

			mysql_free_result(result);
		}
	} else if (!cmd || ((stricmp(cmd,"LIST") == 0) ?
			    mask != NULL : mask == NULL)) {
		syntax_error(s_NickServ, u, "ACCESS", NICK_ACCESS_SYNTAX);

	} else if (mask && !strchr(mask, '@') && (stricmp(cmd, "ADD") == 0)) {
		notice_lang(s_NickServ, u, BAD_USERHOST_MASK);
		notice_lang(s_NickServ, u, MORE_INFO, s_NickServ, "ACCESS");

	} else if (!(nick_id = u->nick_id)) {
		notice_lang(s_NickServ, u, NICK_NOT_REGISTERED);

	} else if (!nick_identified(u)) {
		notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);

	} else if (stricmp(cmd, "ADD") == 0) {
		unsigned int err, count = nickaccess_count(nick_id);
		char *esc_mask;
		
		if (count >= NSAccessMax) {
			notice_lang(s_NickServ, u, NICK_ACCESS_REACHED_LIMIT,
				    NSAccessMax);
			return;
		}

		esc_mask = smysql_escape_string(mysqlconn, mask);

		result = mysql_simple_query(mysqlconn, &err,
					    "INSERT INTO nickaccess "
					    "(nickaccess_id, nick_id, idx, "
					    "userhost) VALUES "
					    "('NULL', '%u', '%u', '%s')",
					    nick_id, count + 1, esc_mask);
		mysql_free_result(result);
		free(esc_mask);
		
		switch (err) {
			case 0:
				break;
			case ER_DUP_ENTRY:
				notice_lang(s_NickServ, u,
					    NICK_ACCESS_ALREADY_PRESENT,
					    mask);
				return;
			default:
				handle_mysql_error(mysqlconn, "add_nickaccess");
				return;
		}
		notice_lang(s_NickServ, u, NICK_ACCESS_ADDED, mask);

	} else if (stricmp(cmd, "DEL") == 0) {
		const char *base = "DELETE FROM nickaccess WHERE (";
		char *query;
		
		/* check if they specified a number/list first */
		if (isdigit(*mask)
		    && strspn(mask, "1234567890,-") == strlen(mask)) {
			const char *column = "idx";

			query = mysql_build_query(mysqlconn, mask, base,
						  column, UINT_MAX);

			result = smysql_bulk_query(mysqlconn, &fields,
						   &rows, "%s) && nick_id=%u",
						   query, nick_id);
			mysql_free_result(result);
			free(query);
			
			if (!rows) {
				notice_lang(s_NickServ, u,
					    NICK_ACCESS_NO_MATCH,
					    u->nick);
			} else if (rows == 1) {
				notice_lang(s_NickServ, u,
					    NICK_ACCESS_DELETED_ONE,
					    u->nick);
			} else {
				notice_lang(s_NickServ, u,
					    NICK_ACCESS_DELETED_SEVERAL,
					    rows, u->nick);
			}
		} else {
			/* they specified a mask to match exactly */
			char *esc_mask = smysql_escape_string(mysqlconn,
							      mask);

			result = smysql_bulk_query(mysqlconn, &fields,
						   &rows, "%s nick_id=%u "
						   "&& userhost='%s')",
						   base, nick_id,
						   esc_mask);
			mysql_free_result(result);
			free(esc_mask);

			if (rows) {
				notice_lang(s_NickServ, u,
					    NICK_ACCESS_DELETED, mask);
			} else {
				notice_lang(s_NickServ, u,
					    NICK_ACCESS_NOT_FOUND, mask);
			}
		}

		if (rows)
			mysql_renumber_nick_access(nick_id);

	} else if (stricmp(cmd, "LIST") == 0) {
		MYSQL_ROW row;
		
		notice_lang(s_NickServ, u, NICK_ACCESS_LIST);
		
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "SELECT idx, userhost "
					   "FROM nickaccess "
					   "WHERE nick_id=%u ORDER BY "
					   "nickaccess_id" , nick_id);

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			notice(s_NickServ, u->nick, "%-4s %s", row[0],
			       row[1]);
		}

		mysql_free_result(result);
	} else {
		syntax_error(s_NickServ, u, "ACCESS", NICK_ACCESS_SYNTAX);

	}
}

/*************************************************************************/

static void do_link(User *u)
{
	char *nick = strtok(NULL, " ");
	char *pass = strtok(NULL, " ");
	int res;
	unsigned int nick_id = u->real_id;
	unsigned int target_id, fields, rows, tmp_id, links;
	MYSQL_RES *result;
	MYSQL_ROW row;

	if (NSDisableLinkCommand) {
		notice_lang(s_NickServ, u, NICK_LINK_DISABLED);
		return;
	}

	if (!pass) {
		syntax_error(s_NickServ, u, "LINK", NICK_LINK_SYNTAX);

	} else if (!nick_id) {
		notice_lang(s_NickServ, u, NICK_NOT_REGISTERED);

	} else if (!nick_identified(u)) {
		notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);

	} else if (!(target_id = mysql_findnick(nick))) {
		notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);

	} else if (target_id == nick_id) {
		notice_lang(s_NickServ, u, NICK_NO_LINK_SAME, nick);

	} else if ((get_nick_status(target_id)) & NS_VERBOTEN) {
		notice_lang(s_NickServ, u, NICK_X_FORBIDDEN, nick);
	} else if ((links = nick_has_links(u->nick_id))) {
		char *tmp_nick = get_nick_from_id(u->nick_id);

		/*
		 * Current nick has links pointing at it, we only allow
		 * one level of nick linking.
		 */
		notice_lang(s_NickServ, u, NICK_X_HAS_LINKS, tmp_nick,
		    links);
		free(tmp_nick);
	} else if ((tmp_id = mysql_getlink(target_id)) != target_id) {
		char *tmp_nick = get_nick_from_id(tmp_id);

		/* target is a link itself and we now don't allow multiple
		 * levels of linking */
		notice_lang(s_NickServ, u, NICK_X_IS_LINK, tmp_nick, nick);
		free(tmp_nick);
	} else if (!(res = check_nick_password(pass, target_id))) {
		log("%s: LINK: bad password for %s by %s!%s@%s", s_NickServ,
		    nick, u->nick, u->username, u->host);
		notice_lang(s_NickServ, u, PASSWORD_INCORRECT);
		bad_password(u);

	} else if (res == -1) {
		notice_lang(s_NickServ, u, NICK_LINK_FAILED);

	} else {
		char *target_nick, *link_nick;

		/* It is no longer possible to create circular links
		 * because there is no way to link to anything that is
		 * already linked */

		/* Check for exceeding the channel registration limit. */
		if (check_chan_limit(target_id) >= 0) {
			unsigned int target_channelmax;

			result = smysql_bulk_query(mysqlconn, &fields,
						   &rows, "SELECT channelmax "
						   "FROM nick WHERE "
						   "nick_id=%u", target_id);
			row = smysql_fetch_row(mysqlconn, result);
			target_channelmax = atoi(row[0]);
			mysql_free_result(result);
			
			notice_lang(s_NickServ, u,
				    NICK_LINK_TOO_MANY_CHANNELS, nick,
				    target_channelmax ?
				    target_channelmax : MAX_CHANNELCOUNT);
			return;
		}

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "UPDATE nick SET link_id=%u "
					   "WHERE nick_id=%u", target_id,
					   nick_id);
		mysql_free_result(result);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "DELETE FROM nickaccess "
					   "WHERE nick_id=%u", nick_id);
		mysql_free_result(result);

		target_nick = smysql_escape_string(mysqlconn,
						   get_nick_from_id(target_id));
		link_nick = smysql_escape_string(mysqlconn,
						 get_nick_from_id(nick_id));

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "UPDATE memo SET owner='%s' "
					   "WHERE owner='%s'", target_nick,
					   link_nick);
		mysql_free_result(result);

		free(link_nick);

		log("%s: %s!%s@%s linked nick %s to %s", s_NickServ, u->nick,
		    u->username, u->host, u->nick, nick);
		snoop(s_NickServ, "[LINK] %s linked to %s (%s@%s)", nick,
		      u->nick, u->username, u->host);
		notice_lang(s_NickServ, u, NICK_LINKED, nick);
		/* They gave the password, so they might as well have
		 * IDENTIFY'd... but don't set NS_IDENTIFIED if someone
		 * else is using the nick! */
		if (!finduser(target_nick))
			add_nick_status(target_id, NS_IDENTIFIED);
		free(target_nick);
	}
}

/*************************************************************************/

static void do_unlink(User *u)
{
	NickInfo ni;
	char *linkname;
	char *nick = strtok(NULL, " ");
	char *pass = strtok(NULL, " ");
	int res = 0;

	ni.nick_id = 0;

	if (nick) {
		int is_servadmin = is_services_admin(u);

		mysql_findnickinfo(&ni, nick);
		if (!ni.nick_id) {
			notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED,
				    nick);
		} else if (!ni.link_id) {
			notice_lang(s_NickServ, u, NICK_X_NOT_LINKED, nick);
		} else if (!is_servadmin && !pass) {
			syntax_error(s_NickServ, u, "UNLINK",
				     NICK_UNLINK_SYNTAX);
		} else if (!is_servadmin
			   && !(res = check_nick_password(pass,
					   		  ni.nick_id))) {
			log("%s: UNLINK: bad password for %s by %s!%s@%s",
			    s_NickServ, nick, u->nick, u->username, u->host);
			notice_lang(s_NickServ, u, PASSWORD_INCORRECT);
			if (!BadPassLimit) {
				snoop(s_NickServ,
				      "[BADPASS] %s (%s@%s) Failed UNLINK",
				      u->nick, u->username,
				      u->host);
			} else {
				snoop(s_NickServ,
				      "[BADPASS] %s (%s@%s) Failed UNLINK (%d of %d)",
				      u->nick, u->username, u->host,
				      u->invalid_pw_count + 1, BadPassLimit);
			}
			bad_password(u);
		} else if (res == -1) {
			notice_lang(s_NickServ, u, NICK_UNLINK_FAILED);
		} else {
			linkname = get_nick_from_id(ni.link_id);
			delink(ni.nick_id);
			notice_lang(s_NickServ, u, NICK_X_UNLINKED, ni.nick,
				    linkname);
			log("%s: %s!%s@%s unlinked nick %s from %s",
			    s_NickServ, u->nick, u->username, u->host,
			    ni.nick, linkname);
			snoop(s_NickServ,
			      "[UNLINK] %s (%s@%s) unlinked %s from %s",
			      u->nick, u->username, u->host,
			      ni.nick, linkname);
			if (is_servadmin) {
				wallops(s_NickServ,
					"\2%s\2 unlinked \2%s\2 from \2%s\2 as services admin",
					u->nick, ni.nick, linkname);
			}
			free(linkname);
		}
	} else {
		get_nickinfo_from_id(&ni, u->real_id);
		if (!ni.nick_id)
			notice_lang(s_NickServ, u, NICK_NOT_REGISTERED);
		else if (!nick_identified(u))
			notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED,
				    s_NickServ);
		else if (!ni.link_id)
			notice_lang(s_NickServ, u, NICK_NOT_LINKED);
		else {
			linkname = get_nick_from_id(ni.link_id);
			/* Effective nick now the same as real nick */
			u->nick_id = ni.nick_id;
			delink(ni.nick_id);
			notice_lang(s_NickServ, u, NICK_UNLINKED, linkname);
			log("%s: %s!%s@%s unlinked nick %s from %s",
			    s_NickServ, u->nick, u->username, u->host,
			    u->nick, linkname);
			snoop(s_NickServ,
			      "[UNLINK] %s (%s@%s) unlinked %s from %s",
			      u->nick, u->username, u->host,
			      ni.nick, linkname);
			free(linkname);
		}
	}

	if (ni.nick_id)
		free_nickinfo(&ni);
}

/*************************************************************************/

/* List nicknames linked to a specified nick.  non-Services admins can now
 * list links to their own nicknames.
 * -grifferz
 */

static void do_listlinks(User *u)
{
	char *nick = strtok(NULL, " ");
	NickInfo ni;

	if (!nick_identified(u)) {
		notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);
		return;
	}

	if (nick) {
		if (!(mysql_findnickinfo(&ni, nick))) {
			notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED,
				    nick);
			return;
		}
                                /* users can specify a nick as long as it
				 * is one of their own */
		if (mysql_getlink(u->nick_id) !=
		    mysql_getlink(ni.nick_id) &&
		    !is_services_admin(u)) {
			notice_lang(s_NickServ, u, ACCESS_DENIED, nick);
			free_nickinfo(&ni);
			return;
		}
	} else if (!(mysql_findnickinfo(&ni, u->nick))) {
			notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED,
				    u->nick);
			return;
	}

	if (ni.link_id) {
		unsigned int tmp_id = ni.link_id;

		free_nickinfo(&ni);
		get_nickinfo_from_id(&ni, tmp_id);
	}

	if (ni.status & NS_VERBOTEN) {
		notice_lang(s_NickServ, u, NICK_X_FORBIDDEN, ni.nick);

	} else {
		MYSQL_RES *result;
		MYSQL_ROW row;
		unsigned int fields, rows;
		
		notice_lang(s_NickServ, u, NICK_LISTLINKS_HEADER, ni.nick);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "SELECT nick FROM nick "
					   "WHERE link_id=%u", ni.nick_id);

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			notice_lang(s_NickServ, u, NICK_X_IS_LINKED,
				    row[0]);
		}

		mysql_free_result(result);

		notice_lang(s_NickServ, u, NICK_LISTLINKS_FOOTER, rows);
	}
	free_nickinfo(&ni);
}

/*************************************************************************/

/* List channels that a specified user has any form of access in.
 * Services admins can specify any nick, users may only specify their
 * own.
 * -grifferz
 */

static void do_listchanaccess(User *u)
{
	char *nick = strtok(NULL, " ");
	char *target_nick;
	unsigned int nick_id, fields, rows;
        unsigned int i = 0, j = 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	if (!nick_identified(u)) {
		notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);
		return;
	}

	if (nick) {
		if (!(nick_id = mysql_findnick(nick))) {
			notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED,
				    nick);
			return;
		}

		/* users can specify a nick as long as it is one of their own */
		if (u->nick_id != nick_id && !is_services_admin(u)) {
			notice_lang(s_NickServ, u, ACCESS_DENIED, nick);
			return;
		}
	} else if (!(nick_id = mysql_findnick(u->nick))) {
			notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED,
				    u->nick);
			return;
	}

	target_nick = get_nick_from_id(nick_id);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT name FROM channel "
				   "WHERE founder=%u", nick_id);

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		if (i == 0) {
			notice_lang(s_NickServ, u,
				    NICK_LISTCHANS_FOUNDER_HEADER,
				    target_nick);
		}
		notice_lang(s_NickServ, u, NICK_X_IS_FOUNDER, row[0]);
		i++;
	}

	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT channel.name, chanaccess.level "
				   "FROM channel, chanaccess "
				   "WHERE channel.channel_id="
				   "chanaccess.channel_id && "
				   "chanaccess.nick_id=%u", nick_id);

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		if (j == 0) {
			notice_lang(s_NickServ, u,
				    NICK_LISTCHANS_ACCESS_HEADER,
				    target_nick);
		}
		notice_lang(s_NickServ, u, NICK_X_HAS_ACCESS, row[0],
			    atoi(row[1]));
		j++;
	}

	free(target_nick);

	notice_lang(s_NickServ, u, NICK_LISTCHANS_FOOTER, i,
		    (i == 1) ? "" : "s", j, (j == 1) ? "" : "s");
}

/*************************************************************************/

/* Show hidden info to nick owners and sadmins when the "ALL" parameter is
 * supplied. If a nick is online, the "Last seen address" changes to "Is 
 * online from".
 * Syntax: INFO <nick> {ALL}
 * -TheShadow (13 Mar 1999)
 */

static void do_info(User *u)
{
	char buf[BUFSIZE];
	NickInfo ni, link_ni;
	MYSQL_ROW row;
	char suspend_who[NICKMAX], timebuf[32], expirebuf[256];
	time_t tmtime, suspend_suspended, suspend_expires, now;
	int need_comma, nick_online, can_show_all, show_all;
	unsigned int suspend_id, fields, rows;
	char *nick, *param, *end, *link_nick, *suspend_reason;
	const char *commastr;
	struct tm *tm;
	MYSQL_RES *result;

	nick = strtok(NULL, " ");
	param = strtok(NULL, " ");

	if (!nick) {
		syntax_error(s_NickServ, u, "INFO", NICK_INFO_SYNTAX);

	} else if (!(mysql_findnickinfo(&ni, nick))) {
		notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);

	} else if (ni.status & NS_VERBOTEN) {
		notice_lang(s_NickServ, u, NICK_X_FORBIDDEN, nick);

	} else {
		commastr = getstring(u->nick_id, COMMA_SPACE);
		need_comma = nick_online = can_show_all = show_all = 0;

		/* Is the real owner of the nick we're looking up online? -TheShadow */
		if ((finduser(nick) || finduser(ni.nick))
		    && (ni.status & (NS_IDENTIFIED | NS_RECOGNIZED)))
			nick_online = 1;

		/* Only show hidden fields to owner and sadmins and only when the ALL
		 * parameter is used. -TheShadow */
		if (nick_online && (irc_stricmp(u->nick, nick) == 0))
			can_show_all = 1;
		else if (is_services_admin(u))
			can_show_all = 1;
		else
			can_show_all = 0;

		if ((param && stricmp(param, "ALL") == 0) && can_show_all)
			show_all = 1;

		get_nickinfo_from_id(&link_ni,
		    ni.link_id ? ni.link_id : ni.nick_id);

		notice_lang(s_NickServ, u, NICK_INFO_REALNAME, nick,
		    ni.last_realname);

		if (nick_online) {
			if (show_all || !(link_ni.flags & NI_HIDE_MASK)) {
				notice_lang(s_NickServ, u,
				    NICK_INFO_ADDRESS_ONLINE,
				    ni.last_usermask);
			} else {
				notice_lang(s_NickServ, u,
				    NICK_INFO_ADDRESS_ONLINE_NOHOST,
				    ni.nick);
			}
		} else {
			if (show_all || !(link_ni.flags & NI_HIDE_MASK)) {
				notice_lang(s_NickServ, u,
				    NICK_INFO_ADDRESS, ni.last_usermask);
			}

			tm = gmtime(&ni.last_seen);
			tmtime = mktime(tm);

			strftime_lang(buf, sizeof(buf), u,
			    STRFTIME_DATE_TIME_FORMAT, tm);
			notice_lang(s_NickServ, u, NICK_INFO_LAST_SEEN,
			    buf, time_ago(tmtime));
		}

		tm = gmtime(&ni.time_registered);
		tmtime = mktime(tm);

		strftime_lang(buf, sizeof(buf), u,
		    STRFTIME_DATE_TIME_FORMAT, tm);
		notice_lang(s_NickServ, u, NICK_INFO_TIME_REGGED, buf,
		    time_ago(tmtime));
		if (strlen(ni.last_quit) &&
		    (show_all || !(link_ni.flags & NI_HIDE_QUIT))) {
			notice_lang(s_NickServ, u, NICK_INFO_LAST_QUIT,
			    ni.last_quit);
		}
		if (ni.last_quit_time &&
		    (show_all || !(link_ni.flags & NI_HIDE_QUIT))) {
			tm = gmtime(&ni.last_quit_time);
			strftime_lang(buf, sizeof(buf), u,
			    STRFTIME_DATE_TIME_FORMAT, tm);
			notice_lang(s_NickServ, u,
			    NICK_INFO_LAST_QUIT_TIME, buf,
			    time_ago(ni.last_quit_time));
		}

		if (strlen(ni.url))
			notice_lang(s_NickServ, u, NICK_INFO_URL, ni.url);
		if (strlen(ni.email) &&
		    (show_all || !(link_ni.flags & NI_HIDE_EMAIL))) {
			notice_lang(s_NickServ, u, NICK_INFO_EMAIL,
			    ni.email);
		}
		*buf = 0;
		end = buf;
		if (link_ni.flags & NI_ENFORCE) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s", getstring(u->nick_id,
			    NICK_INFO_OPT_ENFORCE));
			need_comma = 1;
		}
		if (link_ni.flags & NI_SECURE) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, NICK_INFO_OPT_SECURE));
			need_comma = 1;
		}
		if (link_ni.flags & NI_PRIVATE) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, NICK_INFO_OPT_PRIVATE));
			need_comma = 1;
		}
		if (link_ni.flags & NI_NOOP) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, NICK_INFO_OPT_NOOP));
			need_comma = 1;
		}
		if (link_ni.flags & NI_IRCOP) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, NICK_INFO_OPT_IRCOP));
			need_comma = 1;
		}
		if (link_ni.flags & NI_MARKED) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, NICK_INFO_OPT_MARKED));
			need_comma = 1;
		}
		if (link_ni.flags & NI_NOSENDPASS) {
			 end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id,
			    NICK_INFO_OPT_NOSENDPASS));
			 need_comma = 1;
		}
		notice_lang(s_NickServ, u, NICK_INFO_OPTIONS,
		    *buf ? buf : getstring(u->nick_id, NICK_INFO_OPT_NONE));

		if (ni.link_id && show_all) {
			link_nick = get_nick_from_id(ni.link_id);

			notice_lang(s_NickServ, u, NICK_INFO_LINKED_TO,
			    link_nick);
			free(link_nick);
		}

		if ((ni.status & NS_NO_EXPIRE) && show_all)
			notice_lang(s_NickServ, u, NICK_INFO_NO_EXPIRE);

		if (link_ni.flags & NI_SUSPENDED) {
			suspend_id = 0;
			suspend_expires = 0;
			suspend_reason = NULL;

			notice_lang(s_NickServ, u, NICK_X_SUSPENDED, nick);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT suspend.suspend_id, suspend.who, "
			    "suspend.reason, suspend.suspended, "
			    "suspend.expires FROM suspend, nicksuspend "
			    "WHERE suspend.suspend_id=nicksuspend.suspend_id "
			    "&& nicksuspend.nick_id=%u", link_ni.nick_id);

			if ((row = smysql_fetch_row(mysqlconn, result))) {
				suspend_id = atoi(row[0]);
				strscpy(suspend_who, row[1], NICKMAX);
				suspend_reason = sstrdup(row[2]);
				suspend_suspended = atoi(row[3]);
				suspend_expires = atoi(row[4]);
			}

			mysql_free_result(result);

			if (show_all && !suspend_id) {
				log("ERROR: Suspend info for %s could not "
				    "be found.", nick);
				notice(s_NickServ, u->nick,
				    "Could not find suspend info.");

			} else if (show_all) {
				now = time(NULL);

				tm = gmtime(&suspend_suspended);
				strftime_lang(timebuf, sizeof(timebuf), u,
				    STRFTIME_DATE_TIME_FORMAT, tm);
				if (suspend_expires == 0) {
					snprintf(expirebuf, sizeof(expirebuf),
					    getstring(u->nick_id,
					    OPER_AKILL_NO_EXPIRE));
				} else if (suspend_expires <= now) {
					snprintf(expirebuf, sizeof(expirebuf),
					    getstring(u->nick_id,
					    OPER_AKILL_EXPIRES_SOON));
				} else {
					expires_in_lang(expirebuf,
					    sizeof(expirebuf), u,
					    suspend_expires - now + 59);
				}
				notice_lang(s_NickServ, u,
				    NICK_INFO_SUSPEND_DETAILS, suspend_who,
				    timebuf, expirebuf);
				notice_lang(s_NickServ, u,
				    NICK_INFO_SUSPEND_REASON,
				    suspend_reason ?  suspend_reason : "");
			}
			free(suspend_reason);
		}
		notice_lang(s_NickServ, u, INFO_END);
		if (can_show_all && !show_all) {
			notice_lang(s_NickServ, u, NICK_INFO_SHOW_ALL,
			    s_NickServ, ni.nick);
		}

		free_nickinfo(&link_ni);
		free_nickinfo(&ni);
	}
}

/*************************************************************************/

static void do_list(User *u)
{
	char *pattern = strtok(NULL, " ");
	char *keyword;
	int nnicks;
	char buf[BUFSIZE];
	int is_servadmin = is_services_admin(u);
	int16 match_NS = 0;	/* NS_ flags a nick must match one of to qualify */
	int32 match_NI = 0;	/* NI_ flags a nick must match one of to qualify */
	char *esc_pattern;

	if (NSListOpersOnly && !(u->mode & UMODE_o)) {
		notice_lang(s_NickServ, u, PERMISSION_DENIED);
		return;
	}

	if (!pattern) {
		syntax_error(s_NickServ, u, "LIST",
			     is_servadmin ? NICK_LIST_SERVADMIN_SYNTAX :
			     NICK_LIST_SYNTAX);
	} else {
		MYSQL_RES *result;
		MYSQL_ROW row;
		unsigned int fields, rows;

		nnicks = 0;

		while (is_servadmin && (keyword = strtok(NULL, " "))) {
			if (stricmp(keyword, "FORBIDDEN") == 0)
				match_NS |= NS_VERBOTEN;
			if (stricmp(keyword, "NOEXPIRE") == 0)
				match_NS |= NS_NO_EXPIRE;
			if (stricmp(keyword, "SUSPENDED") == 0)
				match_NI |= NI_SUSPENDED;
			if (stricmp(keyword, "IRCOP") == 0)
				match_NI |= NI_IRCOP;
		}

		esc_pattern = smysql_escape_string(mysqlconn, pattern);

		if (is_servadmin) {
			if ((match_NI | match_NS) != 0) {
				result = smysql_bulk_query(mysqlconn,
				    &fields, &rows,
				    "SELECT nick, last_usermask, status, "
				    "flags FROM nick WHERE ((status & %d) "
				    "|| (flags & %d) && IRC_MATCH('%s', "
				    "CONCAT(nick, '!', last_usermask)))",
				    match_NS, match_NI, esc_pattern);
			} else {
				result = smysql_bulk_query(mysqlconn,
				    &fields, &rows,
				    "SELECT nick, last_usermask, status, "
				    "flags FROM nick WHERE "
				    "IRC_MATCH('%s', CONCAT(nick, '!', "
				    "last_usermask))", esc_pattern);
			}
		} else {
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT nick, last_usermask, status, "
			    "flags FROM nick WHERE !(flags & %d) && "
			    "!(status & %d) && IRC_MATCH('%s', "
			    "CONCAT(nick, '!', last_usermask))",
			    NI_PRIVATE, NS_VERBOTEN, esc_pattern);
		}

		free(esc_pattern);
		
		notice_lang(s_NickServ, u, NICK_LIST_HEADER, pattern);

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			if (++nnicks <= NSListMax) {
				char no_expire_char = ' ';
				char suspended_char = ' ';
				int nick_status = atoi(row[2]);
				int nick_flags = atoi(row[3]);
				
				if (is_servadmin) {
					if (nick_status & NS_NO_EXPIRE) {
						no_expire_char = '!';
					}

					if (nick_flags & NI_SUSPENDED) {
						suspended_char = '*';
					}
				}

				if (!is_servadmin &&
				    (nick_flags & NI_HIDE_MASK)) {
					snprintf(buf, sizeof(buf),
						 "%-20s  [Hidden]", row[0]);
				} else if (nick_status & NS_VERBOTEN) {
					snprintf(buf, sizeof(buf),
						 "%-20s  [Forbidden]",
						 row[0]);
				} else {
					snprintf(buf, sizeof(buf),
						 "%-20s  %s", row[0],
						 row[1]);
				}
					
				notice(s_NickServ, u->nick, "   %c%c %s",
				       suspended_char, no_expire_char, buf);
			}
		}

		mysql_free_result(result);

		notice_lang(s_NickServ, u, NICK_LIST_RESULTS,
			    nnicks > NSListMax ? NSListMax : nnicks, nnicks);
	}
}

/*************************************************************************/

static void do_recover(User *u)
{
	char *nick = strtok(NULL, " ");
	char *pass = strtok(NULL, " ");
	unsigned int nick_id;
	User *u2;

	if (!nick) {
		syntax_error(s_NickServ, u, "RECOVER", NICK_RECOVER_SYNTAX);
	} else if (!(u2 = finduser(nick))) {
		notice_lang(s_NickServ, u, NICK_X_NOT_IN_USE, nick);
	} else if (!(nick_id = u2->real_id)) {
		notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
	} else if (irc_stricmp(nick, u->nick) == 0) {
		notice_lang(s_NickServ, u, NICK_NO_RECOVER_SELF);
	} else if (pass) {
		int res = check_nick_password(pass, nick_id);

		if (res == 1) {
			collide(nick_id, 0);
			snoop(s_NickServ, "[RECOVER] %s (%s@%s) RECOVERed %s",
			      u->nick, u->username, u->host, nick);
			notice_lang(s_NickServ, u, NICK_RECOVERED, s_NickServ,
				    nick);
		} else {
			notice_lang(s_NickServ, u, ACCESS_DENIED);
			if (res == 0) {
				log
				    ("%s: RECOVER: invalid password for %s by %s!%s@%s",
				     s_NickServ, nick, u->nick, u->username,
				     u->host);
				if (!BadPassLimit) {
					snoop(s_NickServ,
					      "[BADPASS] %s (%s@%s) Failed RECOVER",
					      u->nick, u->username, u->host);
				} else {
					snoop(s_NickServ,
					      "[BADPASS] %s (%s@%s) Failed RECOVER (%d of %d)",
					      u->nick, u->username, u->host,
					      u->invalid_pw_count + 1, BadPassLimit);
				}
				bad_password(u);
			}
		}
	} else {
		if (!(get_nick_flags(nick_id) & NI_SECURE) &&
		    is_on_access(u, nick_id)) {
			collide(nick_id, 0);
			snoop(s_NickServ, "[RECOVER] %s (%s@%s) recovered %s",
			      u->nick, u->username, u->host, nick);
			notice_lang(s_NickServ, u, NICK_RECOVERED, s_NickServ,
				    nick);
		} else {
			notice_lang(s_NickServ, u, ACCESS_DENIED);
		}
	}
}

/*************************************************************************/

static void do_release(User *u)
{
	char *nick = strtok(NULL, " ");
	char *pass = strtok(NULL, " ");
	unsigned int nick_id;

	if (!nick) {
		syntax_error(s_NickServ, u, "RELEASE", NICK_RELEASE_SYNTAX);
	} else if (!(nick_id = mysql_findnick(nick))) {
		notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
	} else if (!(get_nick_status(nick_id) & NS_KILL_HELD)) {
		notice_lang(s_NickServ, u, NICK_RELEASE_NOT_HELD, nick);
	} else if (pass) {
		int res = check_nick_password(pass, nick_id);

		if (res == 1) {
			release(nick_id, 0);
			snoop(s_NickServ, "[RELEASE] %s (%s@%s) RELEASEd %s",
			      u->nick, u->username, u->host, nick);
			notice_lang(s_NickServ, u, NICK_RELEASED);
		} else {
			notice_lang(s_NickServ, u, ACCESS_DENIED);
			if (res == 0) {
				log
				    ("%s: RELEASE: invalid password for %s by %s!%s@%s",
				     s_NickServ, nick, u->nick, u->username,
				     u->host);
				if (!BadPassLimit) {
					snoop(s_NickServ,
					      "[BADPASS] %s (%s@%s) Failed RELEASE",
					      u->nick, u->username, u->host);
				} else {
					snoop(s_NickServ,
					      "[BADPASS] %s (%s@%s) Failed RELEASE (%d of %d)",
					      u->nick, u->username, u->host,
					      u->invalid_pw_count + 1,
					      BadPassLimit);
				}
				bad_password(u);
			}
		}
	} else {
		if (!(get_nick_flags(nick_id) & NI_SECURE) &&
		    is_on_access(u, nick_id)) {
			release(nick_id, 0);
			snoop(s_NickServ, "[RELEASE] %s (%s@%s) RELEASEd %s",
			      u->nick, u->username, u->host, nick);
			notice_lang(s_NickServ, u, NICK_RELEASED);
		} else {
			notice_lang(s_NickServ, u, ACCESS_DENIED);
		}
	}
}

/*************************************************************************/

static void do_ghost(User *u)
{
	char *nick = strtok(NULL, " ");
	char *pass = strtok(NULL, " ");
	unsigned int nick_id;
	User *u2;

	if (!nick) {
		syntax_error(s_NickServ, u, "GHOST", NICK_GHOST_SYNTAX);
	} else if (!(u2 = finduser(nick))) {
		notice_lang(s_NickServ, u, NICK_X_NOT_IN_USE, nick);
	} else if (!(nick_id = u2->real_id)) {
		notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
	} else if (irc_stricmp(nick, u->nick) == 0) {
		notice_lang(s_NickServ, u, NICK_NO_GHOST_SELF);
	} else if (pass) {
		int res = check_nick_password(pass, nick_id);

		if (res == 1) {
			char buf[NICKMAX + 32];

			snprintf(buf, sizeof(buf), "GHOST command used by %s",
				 u->nick);
			kill_user(s_NickServ, nick, buf);
			snoop(s_NickServ, "[GHOST] %s (%s@%s) GHOSTed %s",
			      u->nick, u->username, u->host, nick);
			notice_lang(s_NickServ, u, NICK_GHOST_KILLED, nick);
		} else {
			notice_lang(s_NickServ, u, ACCESS_DENIED);
			if (res == 0) {
				log
				    ("%s: GHOST: invalid password for %s by %s!%s@%s",
				     s_NickServ, nick, u->nick, u->username,
				     u->host);
				if (!BadPassLimit) {
					snoop(s_NickServ,
					      "[BADPASS] %s (%s@%s) Failed GHOST",
					      u->nick, u->username, u->host);
				} else {
					snoop(s_NickServ,
					      "[BADPASS] %s (%s@%s) Failed GHOST (%d of %d)",
					      u->nick, u->username, u->host,
					      u->invalid_pw_count + 1,
					      BadPassLimit);
				}
				bad_password(u);
			}
		}
	} else {
		if (!(get_nick_flags(nick_id) & NI_SECURE) &&
		    is_on_access(u, nick_id)) {
			char buf[NICKMAX + 32];

			snprintf(buf, sizeof(buf), "GHOST command used by %s",
				 u->nick);
			kill_user(s_NickServ, nick, buf);
			snoop(s_NickServ, "[GHOST] %s (%s@%s) GHOSTed %s",
			      u->nick, u->username, u->host, nick);
			notice_lang(s_NickServ, u, NICK_GHOST_KILLED, nick);
		} else {
			notice_lang(s_NickServ, u, ACCESS_DENIED);
		}
	}
}

/*************************************************************************/

#if O
static void do_regain(User *u)
{
	char *nick = strtok(NULL, " ");
	char *pass = strtok(NULL, " ");
	unsigned int nick_id;
	User *u2;

	if (!nick || !pass) {
		syntax_error(s_NickServ, u, "REGAIN", NICK_REGAIN_SYNTAX);
	} else if (!(u2 = finduser(nick))) {
		notice_lang(s_NickServ, u, NICK_X_NOT_IN_USE, nick);
	} else if (!(nick_id = u2->real_id)) {
		notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
	} else if (irc_stricmp(nick, u->nick) == 0) {
		notice_lang(s_NickServ, u, NICK_NO_REGAIN_SELF);
	} else {
		int res = check_nick_password(pass, nick_id);

		if (res == 1) {
			char buf[NICKMAX + 32];
			MYSQL_RES *result;
			unsigned int fields, rows;
			char *target_nick = get_nick_from_id(nick_id);

			snprintf(buf, sizeof(buf), "REGAIN command used by %s",
				 u->nick);
			snoop(s_NickServ, "[REGAIN] %s (services_stamp: %d) "
			      "(%s@%s) REGAINed %s", u->nick,
			      (long unsigned)u->services_stamp, u->username,
			      u->host, nick);
			kill_user(s_NickServ, nick, buf);

			result = smysql_bulk_query(mysqlconn, &fields,
						   &rows, "UPDATE nick "
						   "SET regainid=%u, "
						   "status=status | %d "
						   "WHERE nick_id=%u",
						   u->services_stamp,
						   NS_REGAINED, nick_id);
			mysql_free_result(result);

			send_cmd(ServerName, "SVSNICK %s :%s", u->nick,
				 target_nick);
			free(target_nick);
		} else {
			notice_lang(s_NickServ, u, ACCESS_DENIED);
			if (res == 0) {
				log
				    ("%s: REGAIN: invalid password for %s by %s!%s@%s",
				     s_NickServ, nick, u->nick, u->username,
				     u->host);
				if (!BadPassLimit) {
					snoop(s_NickServ,
					      "[BADPASS] %s (%s@%s) Failed REGAIN",
					      u->nick, u->username, u->host);
				} else {
					snoop(s_NickServ,
					      "[BADPASS] %s (%s@%s) Failed REGAIN (%d of %d)",
					      u->nick, u->username, u->host,
					      u->invalid_pw_count + 1,
					      BadPassLimit);
				}
				bad_password(u);
			}
		}
	}
}
#endif

static void do_status(User *u)
{
	char *nick;
	User *u2;
	int i = 0;

	while ((nick = strtok(NULL, " ")) && (i++ < 16)) {
		if (!(u2 = finduser(nick)))
			notice(s_NickServ, u->nick, "STATUS %s 0", nick);
		else if (nick_identified(u2))
			notice(s_NickServ, u->nick, "STATUS %s 3", nick);
		else if (nick_recognized(u2))
			notice(s_NickServ, u->nick, "STATUS %s 2", nick);
		else
			notice(s_NickServ, u->nick, "STATUS %s 1", nick);
	}
}

/*************************************************************************/

static void do_sendpass(User *u)
{
	NickInfo ni;
	MYSQL_ROW row;
	MYSQL_RES *result;
	char cmdbuf[500], buf[4096], salt[SALTMAX], newpass[11];
	char rstr[SALTMAX], codebuf[SALTMAX + NICKMAX - 1], code[PASSMAX];
	FILE *fp;
	char *nick, *authcode, *esc_authcode, *esc_name;
	const char *mailfmt;
	unsigned int fields, rows;

	nick = strtok(NULL, " ");

	if (nick && stricmp(nick, "AUTH") == 0) {
		/*
		 * They are trying to authorise an existing SENDPASS request.
		 * So, check it matches and look up the name they want
		 * sendpassing.  We probably should not guess that the name is
		 * the same as the current nick.
		 */
		authcode = strtok(NULL, " ");

		if (!authcode) {
			syntax_error(s_NickServ, u, "SENDPASS",
			    NICK_SENDPASS_SYNTAX);
			return;
		}

		esc_authcode = smysql_escape_string(mysqlconn, authcode);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT name FROM auth "
		    "WHERE command='NICK_SENDPASS' && code='%s'",
		    esc_authcode);

		
		if (!rows) {
			mysql_free_result(result);
			free(esc_authcode);
			notice_lang(s_NickServ, u,
			    NICK_SENDPASS_NO_SUCH_AUTH);
			return;
		}

		row = smysql_fetch_row(mysqlconn, result);
		if (!mysql_findnickinfo(&ni, row[0])) {
			notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED,
			    row[0]);
			free(esc_authcode);
			mysql_free_result(result);
			return;
		}

		/* Don't need the mysql data anymore */
		mysql_free_result(result);

		/* Nuke the AUTH request. */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "DELETE FROM auth "
		    "WHERE command='NICK_SENDPASS' && code='%s'",
		    esc_authcode);
		mysql_free_result(result);

		free(esc_authcode);

		/*
		 * Now we're ready to create a new password and mail it to
		 * them
		 */
		make_random_string(salt, sizeof(salt));
		make_random_string(newpass, sizeof(newpass));
		strscpy(ni.last_sendpass_pass, ni.pass, PASSMAX);
		ni.last_sendpass_time = time(NULL);
		mailfmt = getstring(ni.nick_id, NICK_SENDPASS_MAIL);

		snprintf(cmdbuf, sizeof(cmdbuf), "%s -t", SendmailPath);
		snprintf(buf, sizeof(buf), mailfmt, NSMyEmail, ni.nick,
		    ni.email, AbuseEmail, NSMyEmail, ni.nick, NetworkName,
		    ni.email, ni.nick, newpass, NetworkName);

		/*
		 * Update the database to reflect that a sendpass has taken
		 * place.  We need to give the user a new password, update the
		 * salt, and record the last password and sendpass time.
		 */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE nick SET pass=SHA1('%s%s'), salt='%s', "
		    "last_sendpass_pass='%s', last_sendpass_salt='%s', "
		    "last_sendpass_time='%d' WHERE nick_id='%u'", newpass,
		    salt, salt, ni.last_sendpass_pass,
		    ni.last_sendpass_salt, ni.last_sendpass_time,
		    ni.nick_id);

		log("%s: %s!%s@%s AUTH'd a SENDPASS on %s", s_NickServ,
		    u->nick, u->username, u->host, ni.nick);
		snoop(s_NickServ,
		    "[SENDPASS] %s (%s@%s) AUTH'd SENDPASS for %s",
		    u->nick, u->username, u->host, ni.nick);

		if ((fp = popen(cmdbuf, "w")) == NULL) {
			log("%s: Failed to create pipe for SENDPASS of %s",
			    s_NickServ, ni.nick);
			snoop(s_NickServ, "[SENDPASS] popen() failed "
			    "during SENDPASS of %s", ni.nick);
			notice_lang(s_NickServ, u, SENDPASS_FAILED,
			    "couldn't create pipe");
			free_nickinfo(&ni);
			return;
		}

		fputs(buf, fp);
		pclose(fp);
		notice_lang(s_NickServ, u, NICK_X_SENDPASSED, ni.nick);
		free_nickinfo(&ni);

		/*
		 * Note that we have NOT deleted the row from the auth table!
		 * This is so that it can serve as rate limiting.  It is
		 * impossible for a user to issue another SENDPASS for this
		 * nick until that row is deleted, and the only way the row
		 * gets deleted is in the periodic cleanup.
		 */
		return;
	}
		 
	/*
	 * They are trying to requst an AUTH code.  We need to check that
	 * sendmail is configured, and work out which nick they mean.
	 */
	if (!SendmailPath) {
		notice_lang(s_NickServ, u, SENDPASS_UNAVAILABLE);
	} else if (!nick) {
		nick = u->nick;
	}
	
	if (!mysql_findnickinfo(&ni, nick)) {
		notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	}
	
	if (strlen(ni.email) <= 1) {
		notice_lang(s_NickServ, u, NICK_X_HAS_NO_EMAIL, nick);
	} else if ((ni.flags & NI_MARKED) &&
		   !(is_services_root(u) || is_services_admin(u))) {
		notice_lang(s_NickServ, u, NICK_X_MARKED, nick);
	} else if (ni.flags & NI_NOSENDPASS) {
		notice_lang(s_NickServ, u, NICK_X_DISABLED_SENDPASS, nick);
	} else {
		/*
		 * We now have all the details we need, but first need to check
		 * that there is no existing SENDPASS AUTH entry hanging
		 * around.  This does not apply to opers, because it is mainly
		 * intended as a rate-limiting feature.
		 */
		esc_name = smysql_escape_string(mysqlconn, ni.nick);
		if (!is_oper(u->nick)) {
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT 1 FROM auth "
			    "WHERE command='NICK_SENDPASS' && name='%s'",
			    esc_name);
			mysql_free_result(result);

			if (rows) {
				notice_lang(s_NickServ, u,
				    NICK_SENDPASS_ALREADY_REQUESTED,
				    ni.nick);
				free(esc_name);
				free_nickinfo(&ni);
				return;
			}
		}
		 
		/*
		 * All seems to be well.  Now we need to send them the
		 * AUTH email.
		 */
		memset(codebuf, 0, sizeof(codebuf));
		memset(rstr, 0, sizeof(rstr));
		make_random_string(rstr, sizeof(rstr));
		strncat(codebuf, u->nick, NICKMAX);
		strncat(codebuf, rstr, SALTMAX);
		make_sha_hash(codebuf, code);

		mailfmt = getstring(ni.nick_id, NICK_SENDPASS_AUTH_MAIL);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "INSERT INTO auth SET auth_id='NULL', "
		    "code='%s', name='%s', command='NICK_SENDPASS', "
		    "params='', create_time=%d", code, esc_name,
		    time(NULL));

		free(esc_name);
		mysql_free_result(result);

		snprintf(cmdbuf, sizeof(cmdbuf), "%s -t", SendmailPath);
		snprintf(buf, sizeof(buf), mailfmt, NSMyEmail, ni.nick,
		    ni.email, AbuseEmail, NSMyEmail, u->nick, u->username,
		    u->host, ni.nick, code, AbuseEmail, NetworkName);

		log("%s: %s!%s@%s used SENDPASS for %s, sending AUTH code "
		    "to %s", s_NickServ, u->nick, u->username, u->host,
		    ni.nick, ni.email);
		snoop(s_NickServ,
		    "[SENDPASS] %s (%s@%s) used SENDPASS for %s, sending "
		    "AUTH code to %s", u->nick, u->username, u->host,
		    ni.nick, ni.email);

		if ((fp = popen(cmdbuf, "w")) == NULL) {
			log("%s: Failed to create pipe for AUTH mail to %s",
			    s_NickServ, ni.email);
			snoop(s_NickServ,
			    "[SENDPASS] popen() failed sending mail to %s",
			    ni.email);
			free_nickinfo(&ni);
			return;
		}

		fputs(buf, fp);
		pclose(fp);
		notice_lang(s_NickServ, u, NICK_SENDPASS_AUTH_MAILED,
		    ni.nick, ni.nick);
	}
	free_nickinfo(&ni);
}
	
/*************************************************************************/

static void do_forbid(User *u)
{
	static NickInfo ni;
	unsigned int nick_id;
	char *nick = strtok(NULL, " ");

	/* Assumes that permission checking has already been done. */
	if (!nick) {
		syntax_error(s_NickServ, u, "FORBID", NICK_FORBID_SYNTAX);
		return;
	}
	if ((nick_id = mysql_findnick(nick))) {
		if (NSSecureAdmins && !is_services_root(u) &&
		    nick_is_services_admin(u->nick_id)) {
			notice_lang(s_NickServ, u, PERMISSION_DENIED);
			return;
		}
		mysql_delnick(nick_id);
	}

	strncpy(ni.nick, nick, NICKMAX);
	ni.status |= NS_VERBOTEN;
	if (insert_new_nick(&ni, NULL, NULL)) {
		log("%s: %s set FORBID for nick %s", s_NickServ, u->nick,
		    nick);
		notice_lang(s_NickServ, u, NICK_FORBID_SUCCEEDED, nick);
		wallops(s_NickServ, "\2%s\2 set FORBID for nick \2%s\2",
			u->nick, nick);
		snoop(s_NickServ, "[FORBID] %s (%s@%s) set FORBID for %s",
		      u->nick, u->username, u->host, nick);
	} else {
		log("%s: Valid FORBID for %s by %s failed", s_NickServ, nick,
		    u->nick);
		notice_lang(s_NickServ, u, NICK_FORBID_FAILED, nick);
	}
}

/*************************************************************************/

static void do_suspend(User *u)
{
	NickInfo ni, real;
	char *expiry, *nick, *reason;
	time_t expires;

	/* Assumes that permission checking has already been done. */

	nick = strtok(NULL, " ");
	if (nick && *nick == '+') {
		expiry = nick;
		nick = strtok(NULL, " ");
	} else {
		expiry = NULL;
	}

	reason = strtok(NULL, "");

	if (!reason) {
		syntax_error(s_NickServ, u, "SUSPEND", NICK_SUSPEND_SYNTAX);
		return;

	}
	
	if (!(mysql_findnickinfo(&ni, nick))) {
		notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	}

	get_nickinfo_from_id(&real, ni.link_id ? ni.link_id : ni.nick_id);

	if (ni.status & NS_VERBOTEN) {
		notice_lang(s_NickServ, u, NICK_X_FORBIDDEN, nick);

	} else if (real.flags & NI_SUSPENDED) {
		notice_lang(s_NickServ, u, NICK_SUSPEND_ALREADY_SUSPENDED,
			    nick);

	} else {

		/* FIXME: add config option for default expiry */
		if (expiry) {
			expires = dotime(expiry);
			if (expires < 0) {
				notice_lang(s_NickServ, u, BAD_EXPIRY_TIME);
				free_nickinfo(&ni);
				free_nickinfo(&real);
				return;
			} else if (expires > 0) {
				expires += time(NULL);	/* Set an absolute time */
			}
		} else {
			expires = time(NULL) + NSSuspendExpire;
		}

		add_nick_flags(real.nick_id, NI_SUSPENDED);

		add_nicksuspend(real.nick_id, reason, u->nick, expires);

		notice_lang(s_NickServ, u, NICK_SUSPEND_SUCCEEDED, nick);
	}
	free_nickinfo(&real);
	free_nickinfo(&ni);
}

static void do_unsuspend(User *u)
{
	char *nick = strtok(NULL, " ");
	unsigned int nick_id, real_id;

	/* Assumes that permission checking has already been done. */
	if (!nick) {
		syntax_error(s_NickServ, u, "UNSUSPEND",
			     NICK_UNSUSPEND_SYNTAX);
		return;
	}
	
	if (!(nick_id = mysql_findnick(nick))) {
		notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	}
	
	if (get_nick_status(nick_id) & NS_VERBOTEN) {
		notice_lang(s_NickServ, u, NICK_X_FORBIDDEN, nick);
		return;
	}

	real_id = mysql_getlink(nick_id);
	
	if (!(get_nick_flags(real_id) & NI_SUSPENDED)) {
		notice_lang(s_NickServ, u, NICK_SUSPEND_NOT_SUSPENDED, nick);

	} else {
		MYSQL_RES *result;
		MYSQL_ROW row;
		unsigned int fields, rows, ns_id;
		char *real_nick = get_nick_from_id(real_id);
		
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "SELECT nicksuspend_id FROM "
					   "nicksuspend WHERE nick_id=%u",
					   real_id);

		if (rows) {
			row = smysql_fetch_row(mysqlconn, result);
			ns_id = atoi(row[0]);
			del_nicksuspend(ns_id);
			log("%s: %s UNSUSPENDED %s", s_NickServ, u->nick,
			    real_nick);
		} else {
			log("%s: couldn't find suspend info for %s",
			    s_NickServ, real_nick);
		}
		free(real_nick);
		unsuspend(real_id);
		notice_lang(s_NickServ, u, NICK_UNSUSPEND_SUCCEEDED, nick);
	}
}

/*************************************************************************/

/* return a nick's status */
int get_nick_status(unsigned int nick_id)
{
	return(nickcache_get_status(nick_id));
}

/* return a nick's flags */
int get_nick_flags(unsigned int nick_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows;
	int flags = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT flags FROM nick WHERE "
				   "nick_id=%u", nick_id);
	
	while ((row = smysql_fetch_row(mysqlconn, result))) {
		flags = atoi(row[0]);
	}

	mysql_free_result(result);

	return(flags);
}

void set_nick_status(unsigned int nick_id, int status)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE nick SET status=%d "
				   "WHERE nick_id=%u", status, nick_id);
	mysql_free_result(result);

	/* keep our nickcache in sync */
	nickcache_set_status(nick_id, status);
}

/* logically OR the current nick flags with the given value */
void add_nick_flags(unsigned int nick_id, int new_flags)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE nick SET flags=flags | %d "
				   "WHERE nick_id=%u", new_flags, nick_id);
	mysql_free_result(result);
}

/* &= ~flags to remove the given value */
void remove_nick_flags(unsigned int nick_id, int remove_flags)
{
	MYSQL_RES *result; 
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE nick SET flags=flags & ~%d "
				   "WHERE nick_id=%u", remove_flags,
				   nick_id);
	mysql_free_result(result);
}

/* logically OR the current status with a new value */
static void add_nick_status(unsigned int nick_id, int new_status)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE nick SET status=status | %d "
				   "WHERE nick_id=%u", new_status, nick_id);
	mysql_free_result(result);

	/* keep our nickcache in sync */
	nickcache_add_status(nick_id, new_status);
}

/* &= ~status to remove the given status */
static void remove_nick_status(unsigned int nick_id, int remove_status)
{
	MYSQL_RES *result; 
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE nick SET status=status & ~%d "
				   "WHERE nick_id=%u", remove_status,
				   nick_id);
	mysql_free_result(result);

	/* keep our nick cache in sync */
	nickcache_remove_status(nick_id, remove_status);
};

/* set the registration time of the given nick */
static void set_nick_regtime(unsigned int nick_id, time_t regtime)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE nick SET regtime=%d "
				   "WHERE nick_id=%u", regtime, nick_id);
	mysql_free_result(result);
};

/* insert a newly registered nick into SQL */
static int insert_new_nick(NickInfo *ni, MemoInfo *mi, char *access)
{
	MYSQL_RES *result;
	unsigned int fields, rows, nick_id;
	char *esc_nick = smysql_escape_string(mysqlconn, ni->nick);
	char *esc_email;
	char *esc_last_usermask;
	char *esc_last_realname;
	char *esc_access = NULL;
       
	esc_email = smysql_escape_string(mysqlconn,
	    ni->email ? ni->email : "");
	esc_last_usermask = smysql_escape_string(mysqlconn,
	    ni->last_usermask ?  ni->last_usermask : "");
	esc_last_realname = smysql_escape_string(mysqlconn,
	    ni->last_realname ?  ni->last_realname : "");

	if (access) 
		esc_access = smysql_escape_string(mysqlconn, access);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO nick (nick_id, nick, pass, salt, email, "
	    "last_usermask, last_realname, time_registered, last_seen, "
	    "status, flags, channelmax, language, id_stamp, regainid) "
	    "VALUES ('NULL', '%s', SHA1('%s%s'), '%s', '%s', '%s', '%s', "
	    "%d, %d, %d, %d, " "%u, %u, %d, %u)", esc_nick, ni->pass,
	    ni->salt, ni->salt, esc_email, esc_last_usermask,
	    esc_last_realname, ni->time_registered, ni->last_seen, ni->status,
	    ni->flags, ni->channelmax, ni->language, ni->id_stamp,
	    ni->regainid);

	mysql_free_result(result);

	nick_id = mysql_insert_id(mysqlconn);

	/* keep our nick cache in sync */
	nickcache_set_status(nick_id, ni->status);
	nickcache_set_language(nick_id, ni->language);

	if (mi) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "INSERT INTO memoinfo (memoinfo_id, owner, max) "
		    "VALUES ('NULL', '%s', %u)", esc_nick, mi->max);
		mysql_free_result(result);
	}

	if (esc_access) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "INSERT INTO nickaccess (nickaccess_id, idx, nick_id, "
		    "userhost) VALUES ('NULL', 1, %u, '%s')", nick_id,
		    esc_access);
		mysql_free_result(result);
	}

	if (esc_access)
		free(esc_access);
	free(esc_last_usermask);
	free(esc_email);
	free(esc_nick);

	return(nick_id);
}

/* return the id of the nick or 0 if it isn't registered */
unsigned int mysql_findnick(const char *nick)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows, nick_id = 0;
	char *esc_nick = smysql_escape_string(mysqlconn, nick);

	/* first check if it is one of the main registered nicks */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT nick_id FROM nick "
				   "WHERE nick='%s'", esc_nick);

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		nick_id = atoi(row[0]);
	}
	mysql_free_result(result);
	free(esc_nick);

	return(nick_id);
}

/* populate a NickInfo structure with the data from MySQL, returning 1
 * if successful and 0 otherwise */
unsigned int mysql_findnickinfo(NickInfo *ni, const char *nick)
{
	MYSQL_RES *result; 
	MYSQL_ROW row;
	unsigned int fields, rows;
	char *esc_nick;
       
	esc_nick = smysql_escape_string(mysqlconn, nick);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT nick_id, nick, pass, url, email, last_usermask, "
	    "last_realname, last_quit, time_registered, last_seen, status, "
	    "flags, channelmax, language, id_stamp, regainid, "
	    "link_id, last_sendpass_pass, last_sendpass_salt, "
	    "last_sendpass_time, last_quit_time FROM nick WHERE nick='%s'",
	    esc_nick);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		ni->nick_id = atoi(row[0]);
		strscpy(ni->nick, row[1], NICKMAX);
		strscpy(ni->pass, row[2], PASSMAX);
		ni->url = sstrdup(row[3]);
		ni->email = sstrdup(row[4]);
		ni->last_usermask = sstrdup(row[5]);
		ni->last_realname = sstrdup(row[6]);
		ni->last_quit = sstrdup(row[7]);
		ni->time_registered = atoi(row[8]);
		ni->last_seen = atoi(row[9]);
		ni->status = atoi(row[10]);
		ni->flags = atoi(row[11]);
		ni->channelmax = atoi(row[12]);
		ni->language = atoi(row[13]);
		ni->id_stamp = atoi(row[14]);
		ni->regainid = atoi(row[15]);
		ni->link_id = atoi(row[16]);
		strscpy(ni->last_sendpass_pass, row[17], PASSMAX);
		strscpy(ni->last_sendpass_salt, row[18], SALTMAX);
		ni->last_sendpass_time = atoi(row[19]);
		ni->last_quit_time = atoi(row[20]);
	} else {
		ni->nick_id = 0;
	}

	mysql_free_result(result);
	free(esc_nick);

	return(rows? 1 : 0);
}

/* populate a NickInfo structure given a nick_id */
unsigned int get_nickinfo_from_id(NickInfo *ni, unsigned int nick_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT nick, pass, url, email, last_usermask, last_realname, "
	    "last_quit, time_registered, last_seen, status, flags, "
	    "channelmax, language, id_stamp, regainid, link_id, "
	    "last_sendpass_pass, last_sendpass_salt, last_sendpass_time, "
	    "last_quit_time FROM nick WHERE nick_id=%u", nick_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		ni->nick_id = nick_id;
		strscpy(ni->nick, row[0], NICKMAX);
		strscpy(ni->pass, row[1], PASSMAX);
		ni->url = sstrdup(row[2]);
		ni->email = sstrdup(row[3]);
		ni->last_usermask = sstrdup(row[4]);
		ni->last_realname = sstrdup(row[5]);
		ni->last_quit = sstrdup(row[6]);
		ni->time_registered = atoi(row[7]);
		ni->last_seen = atoi(row[8]);
		ni->status = atoi(row[9]);
		ni->flags = atoi(row[10]);
		ni->channelmax = atoi(row[11]);
		ni->language = atoi(row[12]);
		ni->id_stamp = atoi(row[13]);
		ni->regainid = atoi(row[14]);
		ni->link_id = atoi(row[15]);
		strscpy(ni->last_sendpass_pass, row[16], PASSMAX);
		strscpy(ni->last_sendpass_salt, row[17], SALTMAX);
		ni->last_sendpass_time = atoi(row[18]);
		ni->last_quit_time = atoi(row[19]);
	}

	mysql_free_result(result);
	return(rows? 1 : 0);
}

/* return a nickname given its ID, caller must free() after use */
char *get_nick_from_id(unsigned int nick_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows;
	char *nick = NULL;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT nick FROM nick WHERE nick_id=%u",
				   nick_id);
	if ((row = smysql_fetch_row(mysqlconn, result))) {
		nick = sstrdup(row[0]);
	}

	mysql_free_result(result);

	return(nick);
}

char *get_cloak_from_id(unsigned int nick_id)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    unsigned int fields, rows;
    char *cloak = NULL;

    result = smysql_bulk_query(mysqlconn, &fields, &rows,
        "SELECT cloak_string FROM nick WHERE nick_id=%u", nick_id);

    if ((row = smysql_fetch_row(mysqlconn, result))) {
        cloak = sstrdup(row[0]);
    }

    mysql_free_result(result);

    return (cloak);
}


/* return the email of the given nick, caller must free() after use */
char *get_email_from_id(unsigned int nick_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows;
	char *email = NULL;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT email FROM nick WHERE nick_id=%u",
				   nick_id);
	if ((row = smysql_fetch_row(mysqlconn, result))) {
		email = sstrdup(row[0]);
	}

	mysql_free_result(result);

	return(email);
}

/* check a given password */
static unsigned int check_nick_password(const char *pass,
					unsigned int nick_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	char *esc_pass;
	
	esc_pass = smysql_escape_string(mysqlconn, pass);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT 1 FROM nick WHERE nick_id=%u && "
	    "pass=SHA1(CONCAT('%s', salt))", nick_id, esc_pass);
	mysql_free_result(result);
	free(esc_pass);

	return(rows? 1 : 0);
}

/* free the contents of a NickInfo (but not the ni itself) */
void free_nickinfo(NickInfo *ni)
{
	if (ni->url)
		free(ni->url);
	if (ni->email)
		free(ni->email);
	if (ni->last_usermask)
		free(ni->last_usermask);
	if (ni->last_realname)
		free(ni->last_realname);
	if (ni->last_quit)
		free(ni->last_quit);
}

/* count the number of access entries for a given user */
static unsigned int nickaccess_count(unsigned int nick_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows;
	unsigned int count;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT COUNT(nickaccess_id) "
				   "FROM nickaccess WHERE nick_id=%u",
				   nick_id);
	row = smysql_fetch_row(mysqlconn, result);
	count = atoi(row[0]);
	mysql_free_result(result);
	
	return(count);
}

/* renumber the idx column of a given nick's access entries */
static void mysql_renumber_nick_access(unsigned int nick_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "TRUNCATE private_tmp_nickaccess2");
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO private_tmp_nickaccess2 (nickaccess_id, idx, "
	    "userhost) SELECT nickaccess_id, NULL, userhost "
	    "FROM nickaccess WHERE nick_id=%u ORDER BY nickaccess_id ASC",
	    nick_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "REPLACE INTO nickaccess (nickaccess_id, nick_id, idx, "
	    "userhost) SELECT nickaccess_id, %u, idx, userhost FROM "
	    "private_tmp_nickaccess2", nick_id);
	mysql_free_result(result);
}

/*
 * Common code for registering a nick.  Passed in NickInfo and MemoInfo
 * are already allocated in caller.  Password and email should already
 * have been checked for sanity.
 */
static void register_nick(NickInfo *ni, MemoInfo *mi, User *u,
    const char *pass, const char *email)
{
	char *nick_access, *esc_pass;

	strscpy(ni->nick, u->nick, NICKMAX);

	if (strlen(pass) > PASSMAX - 1)	{ /* -1 for null byte */
		notice_lang(s_NickServ, u, PASSWORD_TRUNCATED, PASSMAX - 1);
	}

	esc_pass = smysql_escape_string(mysqlconn, pass);
	strscpy(ni->pass, esc_pass, PASSMAX);
	free(esc_pass);

	ni->status = NS_IDENTIFIED | NS_RECOGNIZED;

	ni->flags = 0;
	if (NSDefEnforce)
		ni->flags |= NI_ENFORCE;
	if (NSDefEnforceQuick)
		ni->flags |= NI_ENFORCEQUICK;
	if (NSDefSecure)
		ni->flags |= NI_SECURE;
	if (NSDefPrivate)
		ni->flags |= NI_PRIVATE;
	if (NSDefHideEmail)
		ni->flags |= NI_HIDE_EMAIL;
	if (NSDefHideUsermask)
		ni->flags |= NI_HIDE_MASK;
	if (NSDefHideQuit)
		ni->flags |= NI_HIDE_QUIT;
	if (NSDefMemoSignon)
		ni->flags |= NI_MEMO_SIGNON;
	if (NSDefMemoReceive)
		ni->flags |= NI_MEMO_RECEIVE;
	if (NSDefAutoJoin)
		ni->flags |= NI_AUTOJOIN;
    if (NSDefCloak)
        ni->flags |= NI_CLOAK;
	
	mi->max = MSMaxMemos;
	ni->channelmax = CSMaxReg;
	ni->last_usermask = smalloc(strlen(u->username) +
	    strlen(u->host) + 2);
	sprintf(ni->last_usermask, "%s@%s", u->username, u->host);
	ni->last_realname = sstrdup(u->realname);
	ni->time_registered = ni->last_seen = time(NULL);

	ni->email = sstrdup(email);
	notice_lang(s_NickServ, u, NICK_SET_EMAIL_CHANGED, email,
	    s_NickServ);

	nick_access = create_mask(u);
	ni->language = DEF_LANGUAGE;

	make_random_string(ni->salt, sizeof(ni->salt));

	log("%s: `%s' registered by %s@%s", s_NickServ, u->nick,
	    u->username, u->host);
	snoop(s_NickServ, "[REG] %s (%s@%s)", u->nick, u->username,
	    u->host);

	if (NSWallReg) {
		wallops(s_NickServ, "[REG] %s@%s registered nick %s",
		    u->username, u->host, u->nick);
	}

	notice_lang(s_NickServ, u, NICK_REGISTERED, u->nick, nick_access);
	notice_lang(s_NickServ, u, NICK_PASSWORD_IS, ni->pass);
	u->lastnickreg = time(NULL);

	if (NSNickURL) {
		notice_lang(s_NickServ, u, NICK_REGISTER_NEWNICK,
		    NSNickURL);
	}

	if (NSRegExtraInfo) {
		notice(s_NickServ, u->nick, NSRegExtraInfo);
	}

	ni->nick_id = insert_new_nick(ni, mi, nick_access);
	u->real_id = u->nick_id = ni->nick_id;

	free(nick_access);
	free(ni->email);
	free(ni->last_realname);
	free(ni->last_usermask);
		
	send_cmd(ServerName, "SVSMODE %s :+R", u->nick);
	
	if (!ni->nick_id) {
		log("%s: makenick(%s) failed", s_NickServ, u->nick);
		notice_lang(s_NickServ, u, NICK_REGISTRATION_FAILED);
	}
}


/*
 * Return the number of nicks that are linked to this nick.
 */
static unsigned int nick_has_links(unsigned int nick_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows, links;
	MYSQL_RES *result;

	links = 0;
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT COUNT(nick_id) FROM nick WHERE link_id=%u", nick_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		links = atoi(row[0]);
	}

	mysql_free_result(result);

	return(links);
}

/*
 * Update the "last seen" columns and set a new status on a nick.
 */
void mysql_update_last_seen(User *u, int status)
{
	unsigned int fields, rows;
	char *last_usermask, *esc_last_usermask, *esc_last_realname;
	MYSQL_RES *result;

	last_usermask = smalloc(strlen(u->username) + strlen(u->host) + 2);
	sprintf(last_usermask, "%s@%s", u->username, u->host);

	esc_last_usermask = smysql_escape_string(mysqlconn, last_usermask);
	esc_last_realname = smysql_escape_string(mysqlconn, u->realname);
		
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE nick SET status=status | %d, id_stamp=%u, "
	    "last_seen=%d, last_usermask='%s', last_realname='%s' "
	    "WHERE nick_id=%u", status, u->services_stamp,
	    time(NULL), esc_last_usermask, esc_last_realname, u->real_id);
	
	mysql_free_result(result);
	free(esc_last_realname);
	free(esc_last_usermask);
	free(last_usermask);
}
