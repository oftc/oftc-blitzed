
/* OperServ functions.
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 *    E-mail: services@lists.blitzed.org
 * Based on ircservices-4.4.8:
 * copyright (c) 1996-1999 Andrew Church
 *    E-mail: <achurch@dragonfire.net>
 * copyright (c) 1999-2000 Andrew Kempe.
 *    E-mail: <theshadow@shadowfire.org>
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include "services.h"
#include "pseudo.h"
#include <unistd.h>

RCSID("$Id$");

/*************************************************************************/

static void do_help(User *u);
static void do_global(User *u);
static void do_stats(User *u);
static void do_admin(User *u);
static void do_oper(User *u);
static void do_os_mode(User *u);
static void do_clearmodes(User *u);
static void do_os_kick(User *u);
static void do_set(User *u);
static void do_jupe(User *u);
static void do_raw(User *u);
static void do_os_quit(User *u);
static void do_shutdown(User *u);
static void do_restart(User *u);
static void do_listignore(User *u);
static void do_killclones(User *u);
static void do_loadlangs(User *u);

static unsigned int num_serv_admins(void);
static unsigned int num_serv_opers(void);

/*************************************************************************/

static Command cmds[] = {
	{"HELP", do_help, NULL, -1, -1, -1, -1, -1, NULL, NULL, NULL, NULL},
	{"GLOBAL", do_global, NULL, OPER_HELP_GLOBAL, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"STATS", do_stats, NULL, OPER_HELP_STATS, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"UPTIME", do_stats, NULL, OPER_HELP_STATS, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},

	/*
	 * Anyone can use the LIST option to the ADMIN and OPER commands; those
	 * routines check privileges to ensure that only authorized users
	 * modify the list.
	 */
	{"ADMIN", do_admin, NULL, OPER_HELP_ADMIN, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"OPER", do_oper, NULL, OPER_HELP_OPER, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	/*
	 * Similarly, anyone can use *NEWS LIST, but *NEWS {ADD,DEL} are
	 * reserved for Services admins.
	 */
	{"LOGONNEWS", do_logonnews, NULL, NEWS_HELP_LOGON, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"OPERNEWS", do_opernews, NULL, NEWS_HELP_OPER, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	/*
	 * Opers can add akills up to 12hrs and remove their own akills.
	 * Services opers can add any length akill and remove any akill.
	 * Services admins can also use FORCE to add akills that they otherwise
	 * could not.
	 */
	{"AKILL", do_akill, NULL, OPER_HELP_AKILL, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	
	{"SESSION", do_session, NULL, OPER_HELP_SESSION, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},

	/* Commands for Services opers: */
	{"MODE", do_os_mode, is_services_oper, OPER_HELP_MODE, -1, -1, -1,
	    -1, NULL, NULL, NULL, NULL},
	{"CLEARMODES", do_clearmodes, is_services_oper,
	    OPER_HELP_CLEARMODES, -1, -1, -1, -1, NULL, NULL, NULL, NULL},
	{"KICK", do_os_kick, is_services_oper, OPER_HELP_KICK, -1, -1, -1,
	    -1, NULL, NULL, NULL, NULL},

	/* Commands for Services admins: */
	{"SET", do_set, is_services_admin, OPER_HELP_SET, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET DEBUG", 0, 0, OPER_HELP_SET_DEBUG, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	{"SET LOGSQL", 0, 0, OPER_HELP_SET_LOGSQL, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"JUPE", do_jupe, is_services_admin, OPER_HELP_JUPE, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"RAW", do_raw, is_services_admin, OPER_HELP_RAW, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"QUIT", do_os_quit, is_services_admin, OPER_HELP_QUIT, -1, -1, -1,
	    -1, NULL, NULL, NULL, NULL},
	{"SHUTDOWN", do_shutdown, is_services_admin, OPER_HELP_SHUTDOWN, -1,
	    -1, -1, -1, NULL, NULL, NULL, NULL},
	{"RESTART", do_restart, is_services_admin, OPER_HELP_RESTART, -1,
	    -1, -1, -1, NULL, NULL, NULL, NULL},
	{"LISTIGNORE", do_listignore, is_services_admin, -1, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"KILLCLONES", do_killclones, is_services_admin,
	    OPER_HELP_KILLCLONES, -1, -1, -1, -1, NULL, NULL, NULL, NULL},
	{"EXCEPTION", do_exception, is_services_admin, OPER_HELP_EXCEPTION,
	    -1, -1, -1, -1, NULL, NULL, NULL, NULL},
	{"QUARANTINE", do_quarantine, is_services_admin,
	    OPER_HELP_QUARANTINE, -1, -1, -1, -1, NULL, NULL, NULL, NULL},
	{"LOADLANGS", do_loadlangs, is_services_admin, OPER_HELP_LOADLANGS,
	    -1, -1, -1, -1, NULL, NULL, NULL, NULL},

	/* Commands for Services root: */

	{"ROTATELOG", rotate_log, is_services_root, -1, -1, -1, -1,
	    OPER_HELP_ROTATELOG, NULL, NULL, NULL, NULL},

#ifdef DEBUG_COMMANDS
#if 0
	{"LISTSERVERS", send_server_list, is_services_root, -1, -1, -1, -1,
	 -1},
	{"LISTCHANS", send_channel_list, is_services_root, -1, -1, -1, -1,
	 -1},
	{"LISTCHAN", send_channel_users, is_services_root, -1, -1, -1, -1,
	 -1},
	{"LISTUSERS", send_user_list, is_services_root, -1, -1, -1, -1, -1},
	{"LISTUSER", send_user_info, is_services_root, -1, -1, -1, -1, -1},
	{"LISTTIMERS", send_timeout_list, is_services_root, -1, -1, -1, -1,
	 -1},
	{"MATCHWILD", do_matchwild, is_services_root, -1, -1, -1, -1, -1},
#endif
#endif

	/* Fencepost: */
	{NULL, NULL, NULL, -1, -1, -1, -1, -1, NULL, NULL, NULL, NULL}
};

/*************************************************************************/

/*************************************************************************/

/* OperServ initialization. */

void os_init(void)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	Command *cmd;
	MYSQL_RES *result;

	maxusercnt = 0;
	maxusertime = 0;

	cmd = lookup_cmd(cmds, "GLOBAL");
	if (cmd)
		cmd->help_param1 = s_GlobalNoticer;
	cmd = lookup_cmd(cmds, "ADMIN");
	if (cmd)
		cmd->help_param1 = s_NickServ;
	cmd = lookup_cmd(cmds, "OPER");
	if (cmd)
		cmd->help_param1 = s_NickServ;

	/* get the max user details */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT name, value FROM stat "
	    "WHERE name='maxusercnt' || name='maxusertime'");

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		if (stricmp(row[0], "maxusercnt") == 0)
			maxusercnt = atoi(row[1]);
		else if (stricmp(row[0], "maxusertime") == 0)
			maxusertime = atoi(row[1]);
	}
	mysql_free_result(result);
}

/*************************************************************************/

/* Main OperServ routine. */

void operserv(const char *source, char *buf)
{
	char *cmd;
	char *s;
	User *u;

	u = finduser(source);

	if (!u) {
		log("%s: user record for %s not found", s_OperServ, source);
		notice(s_OperServ, source,
		    getstring(0, USER_RECORD_NOT_FOUND));
		return;
	}

	log("%s: %s: %s", s_OperServ, source, buf);
	snoop(s_OperServ, "[OS] %s (%s@%s): %s", u->nick, u->username,
	    u->host, buf);

	cmd = strtok(buf, " ");
	if (!cmd) {
		return;
	} else if (stricmp(cmd, "\1PING") == 0) {
		if (!(s = strtok(NULL, "")))
			s = "\1";
		notice(s_OperServ, source, "\1PING %s", s);
	} else if (stricmp(cmd, "\1VERSION\1") == 0) {
		notice(s_OperServ, source, "\1VERSION Blitzed-Based OFTC IRC Services v%s\1",
		    VERSION);
	} else {
		run_cmd(s_OperServ, u, cmds, cmd);
	}
}

/*************************************************************************/

/**************************** Privilege checks ***************************/

/*************************************************************************/


/* Does the given user have Services root privileges? */

int is_services_root(User * u)
{
	if (!(u->mode & UMODE_o) || stricmp(u->nick, ServicesRoot) != 0)
		return 0;
	if (nick_identified(u))
		return 1;
	return 0;
}

/*************************************************************************/

/* Does the given user have Services admin privileges? */

int is_services_admin(User * u)
{
	unsigned int fields, rows;
	MYSQL_RES *result;


	if (!(u->mode & UMODE_o))
		return(0);
	if (!nick_identified(u))
		return(0);
	if (is_services_root(u))
		return(1);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT 1 FROM admin WHERE nick_id=%u || nick_id=%u",
	    u->nick_id, u->real_id);
	mysql_free_result(result);

	return(rows? 1 : 0);
}

/*************************************************************************/

/* Does the given user have Services oper privileges? */

int is_services_oper(User * u)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	if (!(u->mode & UMODE_o))
		return(0);
	if (!nick_identified(u))
		return(0);
	if (is_services_admin(u))
		return(1);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT 1 FROM oper WHERE nick_id=%u || nick_id=%u", u->nick_id,
	    u->real_id);
	mysql_free_result(result);

	return(rows? 1 : 0);
}

/*************************************************************************/

/* Is the given nick a Services admin/root nick? */

/*
 * NOTE: Do not use this to check if a user who is online is a services admin
 * or root. This function only checks if a user has the ABILITY to be a
 * services admin. Rather use is_services_admin(User *u).
 * -TheShadow
 */

int nick_is_services_admin(unsigned int nick_id)
{
	unsigned int link_id, fields, rows;
	MYSQL_RES *result;
	char *nick, *link_nick;

	if (!nick_id)
		return 0;

	link_id = mysql_getlink(nick_id);

	nick = get_nick_from_id(nick_id);
	link_nick = get_nick_from_id(link_id);

	if (stricmp(nick, ServicesRoot) == 0 ||
	    stricmp(link_nick, ServicesRoot) == 0) {
		free(nick);
		free(link_nick);
		return 1;
	}

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT 1 FROM admin WHERE nick_id=%u || nick_id=%u", nick_id,
	    link_id);
	mysql_free_result(result);

	free(nick);
	free(link_nick);

	return(rows? 1 : 0);
}

/*************************************************************************/


/* Expunge a deleted nick from the Services admin/oper lists. */

void os_mysql_remove_nick(unsigned int nick_id)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM admin WHERE nick_id=%u", nick_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM oper WHERE nick_id=%u", nick_id);
	mysql_free_result(result);
}

/*************************************************************************/

/*********************** OperServ command functions **********************/

/*************************************************************************/

/* HELP command. */

static void do_help(User * u)
{
	const char *cmd;

	cmd = strtok(NULL, " ");

	notice_help(s_OperServ, u, OPER_HELP_START);

	if (!cmd) {
		notice_help(s_OperServ, u, OPER_HELP);
	} else {
		help_cmd(s_OperServ, u, cmds, cmd);
	}
	notice_help(s_OperServ, u, OPER_HELP_END);
}

/*************************************************************************/

/* Global notice sending via GlobalNoticer. */

static void do_global(User * u)
{
	char *msg;

	msg = strtok(NULL, "");

	if (!msg) {
		syntax_error(s_OperServ, u, "GLOBAL", OPER_GLOBAL_SYNTAX);
		return;
	}
	global_notice(s_GlobalNoticer, "%s [from %s]", msg, u->nick);
}

/*************************************************************************/

/* STATS command. */

static void do_stats(User * u)
{
	char timebuf[64];
	time_t uptime;
	long count, mem, count2, mem2;
	int days, hours, mins, secs, timeout;
	struct tm *tm;
	char *extra;

	uptime = time(NULL) - start_time;
	extra = strtok(NULL, "");
	days = uptime / 86400;
	hours = (uptime / 3600) % 24;
	mins = (uptime / 60) % 60;
	secs = uptime % 60;

	if (extra && stricmp(extra, "ALL") != 0) {
		if (stricmp(extra, "AKILL") == 0) {
			timeout = AutokillExpiry + 59;

			notice_lang(s_OperServ, u, OPER_STATS_AKILL_COUNT,
			    num_akills());

			if (timeout >= 172800) {
				notice_lang(s_OperServ, u,
				    OPER_STATS_AKILL_EXPIRE_DAYS,
				    timeout / 86400);
			} else if (timeout >= 86400) {
				notice_lang(s_OperServ, u,
				    OPER_STATS_AKILL_EXPIRE_DAY);
			} else if (timeout >= 7200) {
				notice_lang(s_OperServ, u,
				    OPER_STATS_AKILL_EXPIRE_HOURS,
				    timeout / 3600);
			} else if (timeout >= 3600) {
				notice_lang(s_OperServ, u,
				    OPER_STATS_AKILL_EXPIRE_HOUR);
			} else if (timeout >= 120) {
				notice_lang(s_OperServ, u,
				    OPER_STATS_AKILL_EXPIRE_MINS,
				    timeout / 60);
			} else if (timeout >= 60) {
				notice_lang(s_OperServ, u,
				    OPER_STATS_AKILL_EXPIRE_MIN);
			} else {
				notice_lang(s_OperServ, u,
				    OPER_STATS_AKILL_EXPIRE_NONE);
			}

			return;
		} else {
			notice_lang(s_OperServ, u, OPER_STATS_UNKNOWN_OPTION,
				    strupper(extra));
		}
	}

	notice_lang(s_OperServ, u, OPER_STATS_CURRENT_USERS, usercnt, opcnt);
	tm = localtime(&maxusertime);
	strftime_lang(timebuf, sizeof(timebuf), u,
	    STRFTIME_DATE_TIME_FORMAT, tm);
	notice_lang(s_OperServ, u, OPER_STATS_MAX_USERS, maxusercnt, timebuf);

	if (days > 1) {
		notice_lang(s_OperServ, u, OPER_STATS_UPTIME_DHMS, days,
		    hours, mins, secs);
	} else if (days == 1) {
		notice_lang(s_OperServ, u, OPER_STATS_UPTIME_1DHMS, days,
		    hours, mins, secs);
	} else {
		if (hours > 1) {
			if (mins != 1) {
				if (secs != 1) {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_HMS, hours,
					    mins, secs);
				} else {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_HM1S, hours,
					    mins, secs);
				}
			} else {
				if (secs != 1) {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_H1MS, hours,
					    mins, secs);
				} else {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_H1M1S, hours,
					    mins, secs);
				}
			}
		} else if (hours == 1) {
			if (mins != 1) {
				if (secs != 1) {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_1HMS, hours,
					    mins, secs);
				} else {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_1HM1S, hours,
					    mins, secs);
				}
			} else {
				if (secs != 1) {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_1H1MS, hours,
					    mins, secs);
				} else {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_1H1M1S,
					    hours, mins, secs);
				}
			}
		} else {
			if (mins != 1) {
				if (secs != 1) {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_MS, mins,
					    secs);
				} else {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_M1S, mins,
					    secs);
				}
			} else {
				if (secs != 1) {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_1MS, mins,
					    secs);
				} else {
					notice_lang(s_OperServ, u,
					    OPER_STATS_UPTIME_1M1S, mins,
					    secs);
				}
			}
		}
	}

	if (extra && stricmp(extra, "ALL") == 0 && is_services_admin(u)) {
		notice_lang(s_OperServ, u, OPER_STATS_BYTES_READ,
		    total_read / 1024);
		notice_lang(s_OperServ, u, OPER_STATS_BYTES_WRITTEN,
		    total_written / 1024);

		get_user_stats(&count, &mem);
		notice_lang(s_OperServ, u, OPER_STATS_USER_MEM, count,
		    (mem + 512) / 1024);
		get_server_stats(&count, &mem);
		notice_lang(s_OperServ, u, OPER_STATS_SERVER_MEM, count,
		    (mem + 512) / 1024);
		get_channel_stats(&count, &mem);
		notice_lang(s_OperServ, u, OPER_STATS_CHANNEL_MEM, count,
		    (mem + 512) / 1024);
		get_nickserv_stats(&count, &mem);
		notice_lang(s_OperServ, u, OPER_STATS_NICKSERV_MEM, count,
		    (mem + 512) / 1024);
		get_chanserv_stats(&count, &mem);
		notice_lang(s_OperServ, u, OPER_STATS_CHANSERV_MEM, count,
		    (mem + 512) / 1024);
		get_memoserv_stats(&count, &mem);
		notice_lang(s_OperServ, u, OPER_STATS_MEMOSERV_MEM, count,
		    (mem + 512) / 1024);
		count = 0;
		get_akill_stats(&count2, &mem2);
		count += count2;
		mem += mem2;
		get_news_stats(&count2, &mem2);
		count += count2;
		mem += mem2;
		get_exception_stats(&count2, &mem2);
		count += count2;
		mem += mem2;
		notice_lang(s_OperServ, u, OPER_STATS_OPERSERV_MEM, count,
		    (mem + 512) / 1024);

		get_session_stats(&count, &mem);
		notice_lang(s_OperServ, u, OPER_STATS_SESSIONS_MEM, count,
		    (mem + 512) / 1024);
	}
}

/*************************************************************************/

/* Channel mode changing (MODE command). */

static void do_os_mode(User * u)
{
	int argc;
	char **argv;
	char *chan, *modes, *s;
	Channel *c;

	s = strtok(NULL, "");

	if (!s) {
		syntax_error(s_OperServ, u, "MODE", OPER_MODE_SYNTAX);
		return;
	}
	
	chan = s;
	s += strcspn(s, " ");

	if (!*s) {
		syntax_error(s_OperServ, u, "MODE", OPER_MODE_SYNTAX);
		return;
	}

	*s = 0;
	modes = (s + 1) + strspn(s + 1, " ");

	if (!*modes) {
		syntax_error(s_OperServ, u, "MODE", OPER_MODE_SYNTAX);
		return;
	}

	if (!(c = findchan(chan))) {
		notice_lang(s_OperServ, u, CHAN_X_NOT_IN_USE, chan);
	} else if (c->bouncy_modes) {
		notice_lang(s_OperServ, u, OPER_BOUNCY_MODES_U_LINE);
		return;
	} else {
		send_cmd(s_OperServ, "MODE %s %s", chan, modes);
		if (WallOSMode) {
			wallops(s_OperServ, "%s used MODE %s on %s",
			    u->nick, modes, chan);
		}

		*s = ' ';
		argc = split_buf(chan, &argv, 1);
		do_cmode(s_OperServ, argc, argv);
	}
}

/*************************************************************************/

/* Clear all modes from a channel. */

static void do_clearmodes(User * u)
{
	int i, all;
	int count;		/* For saving ban info */
	char *s;
	char *argv[3];
	char *chan;
	Channel *c;
	Ban **bans;		/* For saving ban info */
	struct c_userlist *cu, *next;

	chan = strtok(NULL, " ");
	all = 0;

	if (!chan) {
		syntax_error(s_OperServ, u, "CLEARMODES",
		    OPER_CLEARMODES_SYNTAX);
	} else if (!(c = findchan(chan))) {
		notice_lang(s_OperServ, u, CHAN_X_NOT_IN_USE, chan);
	} else if (c->bouncy_modes) {
		notice_lang(s_OperServ, u, OPER_BOUNCY_MODES_U_LINE);
		return;
	} else {
		s = strtok(NULL, " ");

		if (s) {
			if (stricmp(s, "ALL") == 0) {
				all = 1;
			} else {
				syntax_error(s_OperServ, u, "CLEARMODES",
				    OPER_CLEARMODES_SYNTAX);
				return;
			}
		}

		if (WallOSClearmodes) {
			wallops(s_OperServ, "%s used CLEARMODES%s on %s",
			    u->nick, all ? " ALL" : "", chan);
		}

		if (all) {
			/* Clear mode +o */
			for (cu = c->chanops; cu; cu = next) {
				next = cu->next;
				argv[0] = sstrdup(chan);
				argv[1] = sstrdup("-o");
				argv[2] = sstrdup(cu->user->nick);
				send_cmd(MODE_SENDER(s_ChanServ),
				    "MODE %s %s :%s", argv[0], argv[1],
				    argv[2]);
				do_cmode(s_ChanServ, 3, argv);
				free(argv[2]);
				free(argv[1]);
				free(argv[0]);
			}

			/* Clear mode +v */
			for (cu = c->voices; cu; cu = next) {
				next = cu->next;
				argv[0] = sstrdup(chan);
				argv[1] = sstrdup("-v");
				argv[2] = sstrdup(cu->user->nick);
				send_cmd(MODE_SENDER(s_ChanServ),
				    "MODE %s %s :%s", argv[0], argv[1],
				    argv[2]);
				do_cmode(s_ChanServ, 3, argv);
				free(argv[2]);
				free(argv[1]);
				free(argv[0]);
			}
		}

		/* Clear modes */
		send_cmd(MODE_SENDER(s_OperServ),
		    "MODE %s -ciklmMnOpsRt :%s", chan,
		    c->key ? c->key : "");
		argv[0] = sstrdup(chan);
		argv[1] = sstrdup("-ciklmMnOpsRt");
		argv[2] = c->key ? c->key : sstrdup("");
		do_cmode(s_OperServ, 2, argv);
		free(argv[0]);
		free(argv[1]);
		free(argv[2]);
		c->key = NULL;
		c->limit = 0;

		/* Clear bans */
		count = c->bancount;
		bans = smalloc(sizeof(Ban *) * count);

		for (i = 0; i < count; i++) {
			bans[i] = smalloc(sizeof(Ban));
			bans[i]->mask = sstrdup(c->newbans[i]->mask);
		}
		for (i = 0; i < count; i++) {
			argv[0] = sstrdup(chan);
			argv[1] = sstrdup("-b");
			argv[2] = bans[i]->mask;
			send_cmd(MODE_SENDER(s_OperServ), "MODE %s %s :%s",
			    argv[0], argv[1], argv[2]);
			do_cmode(s_OperServ, 3, argv);
			free(argv[2]);
			free(argv[1]);
			free(argv[0]);
			free(bans[i]);
		}
		free(bans);

		if (all) {
			notice_lang(s_OperServ, u, OPER_CLEARMODES_ALL_DONE,
			    chan);
		} else {
			notice_lang(s_OperServ, u, OPER_CLEARMODES_DONE,
			    chan);
		}
	}
}

/*************************************************************************/

/* Kick a user from a channel (KICK command). */

static void do_os_kick(User * u)
{
	char *argv[3];
	char *chan, *nick, *s;
	Channel *c;

	chan = strtok(NULL, " ");
	nick = strtok(NULL, " ");
	s = strtok(NULL, "");

	if (!chan || !nick || !s) {
		syntax_error(s_OperServ, u, "KICK", OPER_KICK_SYNTAX);
		return;
	}
	if (!(c = findchan(chan))) {
		notice_lang(s_OperServ, u, CHAN_X_NOT_IN_USE, chan);
	} else if (c->bouncy_modes) {
		notice_lang(s_OperServ, u, OPER_BOUNCY_MODES_U_LINE);
		return;
	}
	send_cmd(s_OperServ, "KICK %s %s :%s (%s)", chan, nick, u->nick, s);
	
	if (WallOSKick) {
		wallops(s_OperServ, "%s used KICK on %s/%s", u->nick, nick,
		    chan);
	}
	
	argv[0] = sstrdup(chan);
	argv[1] = sstrdup(nick);
	argv[2] = sstrdup(s);
	do_kick(s_OperServ, 3, argv);
	free(argv[2]);
	free(argv[1]);
	free(argv[0]);
}

/*************************************************************************/

/* Services admin list viewing/modification. */

static void do_admin(User * u)
{
	MYSQL_ROW row;
	unsigned int nick_id, fields, rows, err;
	char *cmd, *nick;
	MYSQL_RES *result;

	cmd = strtok(NULL, " ");
	
	if (!cmd)
		cmd = "";

	if (stricmp(cmd, "ADD") == 0) {
		if (!is_services_root(u)) {
			notice_lang(s_OperServ, u, PERMISSION_DENIED);
			return;
		}
		
		nick = strtok(NULL, " ");
		
		if (nick) {
			if (!(nick_id = mysql_findnick(nick))) {
				notice_lang(s_OperServ, u,
				    NICK_X_NOT_REGISTERED, nick);
				return;
			} else if (num_serv_admins() >= MAX_SERVADMINS) {
				notice_lang(s_OperServ, u,
				    OPER_ADMIN_TOO_MANY, MAX_SERVADMINS);
				return;
			}

			nick_id = mysql_getlink(nick_id);

			result = mysql_simple_query(mysqlconn, &err,
			    "INSERT INTO admin (nick_id) VALUES (%u)",
			    nick_id);

			switch (err) {
			case 0:
				mysql_free_result(result);
				notice_lang(s_OperServ, u, OPER_ADMIN_ADDED,
				    nick);
				break;
			case ER_DUP_ENTRY:
				notice_lang(s_OperServ, u,
				    OPER_ADMIN_EXISTS, nick);
				break;
			default:
				handle_mysql_error(mysqlconn,
				    "insert_admin");
				break;	/* never reached */
			}
		} else {
			syntax_error(s_OperServ, u, "ADMIN",
			    OPER_ADMIN_ADD_SYNTAX);
		}

	} else if (stricmp(cmd, "DEL") == 0) {
		if (!is_services_root(u)) {
			notice_lang(s_OperServ, u, PERMISSION_DENIED);
			return;
		}
		nick = strtok(NULL, " ");
		if (nick) {
			if (!(nick_id = mysql_findnick(nick))) {
				notice_lang(s_OperServ, u,
				    NICK_X_NOT_REGISTERED, nick);
				return;
			}

			nick_id = mysql_getlink(nick_id);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "DELETE FROM admin WHERE nick_id=%u",
			    nick_id);

			if (rows) {
				notice_lang(s_OperServ, u,
				    OPER_ADMIN_REMOVED, nick);
			} else {
				notice_lang(s_OperServ, u,
				    OPER_ADMIN_NOT_FOUND, nick);
			}

			mysql_free_result(result);
		} else {
			syntax_error(s_OperServ, u, "ADMIN",
			    OPER_ADMIN_DEL_SYNTAX);
		}

	} else if (stricmp(cmd, "LIST") == 0) {
		notice_lang(s_OperServ, u, OPER_ADMIN_LIST_HEADER);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT nick.nick FROM admin, nick "
		    "WHERE nick.nick_id=admin.nick_id ORDER BY nick.nick");

		while ((row = smysql_fetch_row(mysqlconn, result)))
			notice(s_OperServ, u->nick, "%s", row[0]);

		mysql_free_result(result);

	} else {
		syntax_error(s_OperServ, u, "ADMIN", OPER_ADMIN_SYNTAX);
	}
}

/*************************************************************************/

/* Services oper list viewing/modification. */

static void do_oper(User * u)
{
	MYSQL_ROW row;
	unsigned int nick_id, fields, rows, err;
	MYSQL_RES *result;
	char *cmd, *nick;

	cmd = strtok(NULL, " ");
	
	if (!cmd)
		cmd = "";

	if (stricmp(cmd, "ADD") == 0) {
		if (!is_services_admin(u)) {
			notice_lang(s_OperServ, u, PERMISSION_DENIED);
			return;
		}
		
		nick = strtok(NULL, " ");
		
		if (nick) {
			if (!(nick_id = mysql_findnick(nick))) {
				notice_lang(s_OperServ, u,
				    NICK_X_NOT_REGISTERED, nick);
				return;
			} else if (num_serv_opers() >= MAX_SERVOPERS) {
				notice_lang(s_OperServ, u,
				    OPER_OPER_TOO_MANY, MAX_SERVOPERS);
				return;
			}

			nick_id = mysql_getlink(nick_id);

			result = mysql_simple_query(mysqlconn, &err,
			    "INSERT INTO oper (nick_id) VALUES (%u)",
			    nick_id);

			switch (err) {
			case 0:
				mysql_free_result(result);
				notice_lang(s_OperServ, u, OPER_OPER_ADDED,
				    nick);
				break;
			case ER_DUP_ENTRY:
				notice_lang(s_OperServ, u,
				    OPER_OPER_EXISTS, nick);
				break;
			default:
				handle_mysql_error(mysqlconn,
				    "insert_oper");
				break;	/* never reached */
			}
		} else {
			syntax_error(s_OperServ, u, "OPER",
			    OPER_OPER_ADD_SYNTAX);
		}

	} else if (stricmp(cmd, "DEL") == 0) {
		if (!is_services_admin(u)) {
			notice_lang(s_OperServ, u, PERMISSION_DENIED);
			return;
		}
		nick = strtok(NULL, " ");
		if (nick) {
			if (!(nick_id = mysql_findnick(nick))) {
				notice_lang(s_OperServ, u,
				    NICK_X_NOT_REGISTERED, nick);
				return;
			}

			nick_id = mysql_getlink(nick_id);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "DELETE FROM oper WHERE nick_id=%u",
			    nick_id);

			if (rows) {
				notice_lang(s_OperServ, u,
				    OPER_OPER_REMOVED, nick);
			} else {
				notice_lang(s_OperServ, u,
				    OPER_OPER_NOT_FOUND, nick);
			}

			mysql_free_result(result);
		} else {
			syntax_error(s_OperServ, u, "OPER",
			    OPER_OPER_DEL_SYNTAX);
		}

	} else if (stricmp(cmd, "LIST") == 0) {
		notice_lang(s_OperServ, u, OPER_OPER_LIST_HEADER);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT nick.nick FROM nick, oper "
		    "WHERE nick.nick_id=oper.nick_id ORDER BY nick.nick");

		while ((row = smysql_fetch_row(mysqlconn, result)))
			notice(s_OperServ, u->nick, "%s", row[0]);

	} else {
		syntax_error(s_OperServ, u, "OPER", OPER_OPER_SYNTAX);
	}
}

/*************************************************************************/

/* Set various Services runtime options. */

static void do_set(User * u)
{
	char *option, *setting;

	option = strtok(NULL, " ");
	setting = strtok(NULL, " ");

	if (!option || !setting) {
		syntax_error(s_OperServ, u, "SET", OPER_SET_SYNTAX);

	} else if (stricmp(option, "IGNORE") == 0) {
		if (stricmp(setting, "on") == 0) {
			allow_ignore = 1;
			notice_lang(s_OperServ, u, OPER_SET_IGNORE_ON);
		} else if (stricmp(setting, "off") == 0) {
			allow_ignore = 0;
			notice_lang(s_OperServ, u, OPER_SET_IGNORE_OFF);
		} else {
			notice_lang(s_OperServ, u, OPER_SET_IGNORE_ERROR);
		}

	} else if (stricmp(option, "DEBUG") == 0) {
		if (stricmp(setting, "on") == 0) {
			debug = 1;
			log("Debug mode activated");
			notice_lang(s_OperServ, u, OPER_SET_DEBUG_ON);
		} else if (stricmp(setting, "off") == 0 ||
		    (*setting == '0' && atoi(setting) == 0)) {
			log("Debug mode deactivated");
			debug = 0;
			notice_lang(s_OperServ, u, OPER_SET_DEBUG_OFF);
		} else if (isdigit(*setting) && atoi(setting) > 0) {
			debug = atoi(setting);
			log("Debug mode activated (level %d)", debug);
			notice_lang(s_OperServ, u, OPER_SET_DEBUG_LEVEL,
			    debug);
		} else {
			notice_lang(s_OperServ, u, OPER_SET_DEBUG_ERROR);
		}
		
	} else if (stricmp(option, "LOGSQL") == 0) {
		if (stricmp(setting, "on") == 0) {
			log_sql = 1;
			smysql_log("SQL logging activated");
			notice_lang(s_OperServ, u, OPER_SET_LOGSQL_ON);
		} else if (stricmp(setting, "off") == 0) {
			smysql_log("SQL logging deactivated");
			log_sql = 0;
			notice_lang(s_OperServ, u, OPER_SET_LOGSQL_OFF);
		} else {
			notice_lang(s_OperServ, u, OPER_SET_LOGSQL_ERROR);
		}

	} else {
		notice_lang(s_OperServ, u, OPER_SET_UNKNOWN_OPTION, option);
	}
}

/*************************************************************************/

/*
 * modified to allow nick jupes, and to squit before a jupe.
 * -grifferz
 */

static void do_jupe(User * u)
{
	char buf[NICKMAX + 16];
	char *jserver, *reason;

	jserver = strtok(NULL, " ");
	reason = strtok(NULL, "");

	if (!jserver) {
		syntax_error(s_OperServ, u, "JUPE", OPER_JUPE_SYNTAX);
	} else if (strchr(jserver, '.')) {
		/* Juping a server, squit the target first. */
		send_cmd(NULL,
		    ":OperServ SQUIT %s :Server jupitered by %s (%s)",
		    jserver, u->nick, reason ? reason : u->nick);

		/* I can't get it to work without this sleep here :/ */
		sleep(1);
		wallops(s_OperServ,
		    "\2Juping\2 %s by request of \2%s\2 (%s).", jserver,
		    u->nick, reason ? reason : u->nick);

		if (!reason) {
			snprintf(buf, sizeof(buf), "Jupitered by %s",
			    u->nick);
			reason = buf;
		}

		send_cmd(NULL, "SERVER %s 2 :%s", jserver, reason);
	} else {
		/* Juping a nick. */
		wallops(s_OperServ,
		    "\2Juping\2 %s by request of \2%s\2 (%s).", jserver,
		    u->nick, reason ? reason : u->nick);
		send_cmd(NULL, "KILL %s :Juped by %s: %s", jserver, u->nick,
		    reason ? reason : u->nick);
		snprintf(buf, sizeof(buf), "Juped (%s)",
		    reason ? reason : u->nick);
		send_nick(jserver, ServiceUser, ServiceHost, ServerName,
		    buf);
	}
}

/*************************************************************************/

static void do_raw(User * u)
{
	char *text;

	text = strtok(NULL, "");

	if (!text)
		syntax_error(s_OperServ, u, "RAW", OPER_RAW_SYNTAX);
	else
		send_cmd(NULL, "%s", text);
}

/*************************************************************************/

static void do_os_quit(User * u)
{
	quitmsg = malloc(28 + strlen(u->nick));
	if (!quitmsg)
		quitmsg = "QUIT command received, but out of memory!";
	else
		sprintf(quitmsg, "QUIT command received from %s", u->nick);
	quitting = 1;
}

/*************************************************************************/

static void do_shutdown(User * u)
{
	quitmsg = malloc(32 + strlen(u->nick));
	if (!quitmsg)
		quitmsg = "SHUTDOWN command received, but out of memory!";
	else
		sprintf(quitmsg, "SHUTDOWN command received from %s",
			u->nick);
	save_data = 1;
	delayed_quit = 1;
}

/*************************************************************************/

static void do_restart(User * u)
{
#ifdef SERVICES_BIN
	quitmsg = malloc(31 + strlen(u->nick));
	if (!quitmsg)
		quitmsg = "RESTART command received, but out of memory!";
	else
		sprintf(quitmsg, "RESTART command received from %s", u->nick);
	raise(SIGHUP);
#else
	notice_lang(s_OperServ, u, OPER_CANNOT_RESTART);
#endif
}

/*************************************************************************/

/* XXX - this function is broken!! */

static void do_listignore(User * u)
{
	int sent_header = 0;
	IgnoreData *id;
	int i;

	notice(s_OperServ, u->nick, "Command disabled - it's broken.");
	return;

	for (i = 0; i < 256; i++) {
		for (id = ignore[i]; id; id = id->next) {
			if (!sent_header) {
				notice_lang(s_OperServ, u, OPER_IGNORE_LIST);
				sent_header = 1;
			}
			notice(s_OperServ, u->nick, "%d %s", id->time,
			       id->who);
		}
	}
	if (!sent_header)
		notice_lang(s_OperServ, u, OPER_IGNORE_LIST_EMPTY);
}

/*************************************************************************/

/*
 * Kill all users matching a certain host. The host is obtained from the
 * supplied nick. The raw hostmsk is not supplied with the command in an effort
 * to prevent abuse and mistakes from being made - which might cause *.com to
 * be killed. It also makes it very quick and simple to use - which is usually
 * what you want when someone starts loading numerous clones. In addition to
 * killing the clones, we add a temporary AKILL to prevent them from 
 * immediately reconnecting.
 * Syntax: KILLCLONES nick
 * -TheShadow (29 Mar 1999) 
 */

static void do_killclones(User * u)
{
	char killreason[NICKMAX + 32];
	char akillreason[] = "Temporary KILLCLONES akill.";
	int count;
	char *clonenick, *clonemask, *akillmask;
	User *cloneuser, *user, *tempuser;

	clonenick = strtok(NULL, " ");
	count = 0;
	
	if (!clonenick) {
		notice_lang(s_OperServ, u, OPER_KILLCLONES_SYNTAX);

	} else if (!(cloneuser = finduser(clonenick))) {
		notice_lang(s_OperServ, u, OPER_KILLCLONES_UNKNOWN_NICK,
		    clonenick);

	} else {
		clonemask = smalloc(strlen(cloneuser->host) + 5);
		sprintf(clonemask, "*!*@%s", cloneuser->host);

		akillmask = smalloc(strlen(cloneuser->host) + 3);
		sprintf(akillmask, "*@%s", strlower(cloneuser->host));

		user = firstuser();

		while (user) {
			if (match_usermask(clonemask, user) != 0) {
				tempuser = nextuser();
				count++;
				snprintf(killreason, sizeof(killreason),
				    "Cloning [%d]", count);
				kill_user(NULL, user->nick, killreason);
				user = tempuser;
			} else {
				user = nextuser();
			}
		}

		add_akill(akillmask, akillreason, u->nick,
		    time(NULL) + (60 * 60 * 6));

		wallops(s_OperServ, "\2%s\2 used KILLCLONES for \2%s\2 "
		    "killing \2%d\2 clones.  A temporary AKILL has been "
		    "added for \2%s\2.", u->nick, clonemask, count,
		    akillmask);

		log("%s: KILLCLONES: %d clone(s) matching %s killed.",
		    s_OperServ, count, clonemask);

		free(akillmask);
		free(clonemask);
	}
}

/*
 * Reload all the language files.
 */
static void do_loadlangs(User *u)
{
	lang_init();
	notice_lang(s_OperServ, u, OPER_LOADLANGS_RELOADED);
	wallops(s_OperServ, "%s used LOADLANGS", u->nick);
}

/*************************************************************************/

static unsigned int num_serv_admins(void)
{
	return(get_table_rows(mysqlconn, "admin"));
}

static unsigned int num_serv_opers(void)
{
	return(get_table_rows(mysqlconn, "oper"));
}
