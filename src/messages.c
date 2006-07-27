
/* Definitions of IRC message functions and list of messages.
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

#include "services.h"
#include "messages.h"
#include "language.h"

RCSID("$Id$");

/* List of messages is at the bottom of the file. */

/*************************************************************************/

/*************************************************************************/

static void m_nickcoll(char *source, int ac, char **av)
{
	USE_VAR(source);
	
	if (ac < 1)
		return;

	introduce_user(av[0]);
}

/*************************************************************************/

static void m_ping(char *source, int ac, char **av)
{
	USE_VAR(source);
	
	if (ac < 1)
		return;
	send_cmd(ServerName, "PONG %s %s", ac > 1 ? av[1] : ServerName,
		 av[0]);
}

/*************************************************************************/

static void m_away(char *source, int ac, char **av)
{
	User *u = finduser(source);

	if (u && (ac == 0 || *av[0] == 0))	/* un-away */
		check_memos(u);
}

/*************************************************************************/

static void m_info(char *source, int ac, char **av)
{
	int i;
	struct tm *tm;
	char timebuf[64];

	USE_VAR(ac);
	USE_VAR(av);

	tm = localtime(&start_time);
	strftime(timebuf, sizeof(timebuf), "%a %b %d %H:%M:%S %Y %Z", tm);

	for (i = 0; info_text[i]; i++)
		send_cmd(ServerName, "371 %s :%s", source, info_text[i]);
	send_cmd(ServerName, "371 %s :Version %s", source, VERSION);
	send_cmd(ServerName, "371 %s :On-line since %s", source, timebuf);
	send_cmd(ServerName, "374 %s :End of /INFO list.", source);
}

/*************************************************************************/

static void m_join(char *source, int ac, char **av)
{
	if (ac != 1)
		return;
	do_join(source, ac, av);
}

/*************************************************************************/

static void m_kick(char *source, int ac, char **av)
{
	if (ac != 3)
		return;
	do_kick(source, ac, av);
}

/*************************************************************************/

static void m_kill(char *source, int ac, char **av)
{
	if (ac != 2)
		return;

	/* Recover if someone kills us. */
	if (stricmp(av[0], s_OperServ) == 0 ||
	    stricmp(av[0], s_NickServ) == 0 ||
	    stricmp(av[0], s_ChanServ) == 0 ||
	    stricmp(av[0], s_MemoServ) == 0 ||
	    stricmp(av[0], s_HelpServ) == 0 ||
	    stricmp(av[0], s_GlobalNoticer) == 0 ||
	    (s_IrcIIHelp && stricmp(av[0], s_IrcIIHelp) == 0) ||
	    (s_DevNull && stricmp(av[0], s_DevNull) == 0)) {
		introduce_user(av[0]);
	} else {
		do_kill(source, ac, av);
	}
}

/*************************************************************************/

static void m_mode(char *source, int ac, char **av)
{
	if (*av[0] == '#' || *av[0] == '&') {
		if (ac < 2)
			return;
		do_cmode(source, ac, av);
	} else {
		if (ac != 2)
			return;
		do_umode(source, ac, av);
	}
}

/*************************************************************************/

static void m_motd(char *source, int ac, char **av)
{
	char buf[BUFSIZE];
	FILE *f;

	USE_VAR(ac);
	USE_VAR(av);

	f = fopen(MOTDFilename, "r");
	send_cmd(ServerName, "375 %s :- %s Message of the Day", source,
	    ServerName);
	
	if (f) {
		while (fgets(buf, sizeof(buf), f)) {
			buf[strlen(buf) - 1] = 0;
			send_cmd(ServerName, "372 %s :- %s", source, buf);
		}
		fclose(f);
	} else {
		send_cmd(ServerName, "372 %s : ", source);
	}

	do_motd(source);

	/* Look, people.  I'm not asking for payment, praise, or anything like
	 * that for using Services... is it too much to ask that I at least get
	 * some recognition for my work?  Please don't remove the copyright
	 * message below.
	 */

	/* Retained Andy's copyright message and added info for Andrew Kempe
	 * and us.
	 * -grifferz
	 */

	send_cmd(ServerName, "372 %s :-", source);
	send_cmd(ServerName, "372 %s :- Blitzed Services copyright (c) "
		 "2000-2002 Blitzed Services Team.", source);
	send_cmd(ServerName, "372 %s :- Based upon ircservices 4.4.8,",
		 source);
	send_cmd(ServerName,
		 "372 %s :-     copyright (c) 1996-1999 Andy Church.",
		 source);
	send_cmd(ServerName,
		 "372 %s :-     copyright (c) 1999-2000 Andrew Kempe.",
		 source);
	send_cmd(ServerName, "376 %s :End of /MOTD command.", source);
}

/*************************************************************************/

/*
 * NICK grifferz 1 980838847 +oiwrRaAh _ooh_ mini-me.noc.strugglers.net andy-old.freebsd.test.blitzed.org 1218468636 [IPADDRESS] :gibdidi
 *
 * Wrong Wrong Wrong....
 *
 * NICK othy 2 1049625382 +i ~othy p362-tnt1.brs.ihug.com.au irc2.soe.uq.edu.au :othy
 *
 */

static void m_nick(char *source, int ac, char **av)
{

	/*
	 * New nicks must now have 8 parameters, nick changes must have 2
	 */

	if (*source == '\0')
	{
	    if(ac != 8)
	    {		
		if (debug) {
			log("debug: NICK message: wrong number of "
			    "parameters (%d) for source `%s'", ac,
			    source);
		}
		return;
	    }
	}
	else
	{
	    if(ac != 2)
	    {
                if (debug)
		{
		    log("debug: NICK message: wrong number of "
		        "parameters (%d) for source `%s'", ac, source);
		}
		return;
	    }
	}

	if (do_nick(source, ac, av)) {
		char *t[] = { av[0], ac>2 ? av[3] : "", NULL };
		do_umode(t[0], 2, t);
	}
}

/*************************************************************************/

static void m_part(char *source, int ac, char **av)
{
	if (ac < 1 || ac > 2)
		return;
	do_part(source, ac, av);
}

/*************************************************************************/

static void m_privmsg(char *source, int ac, char **av)
{
	time_t starttime, stoptime;	/* When processing started and finished */
	char *s;

	if (ac != 2)
		return;

	/* Check if we should ignore.  Operators always get through. */
	if (allow_ignore && !is_oper(source)) {
		IgnoreData *ign = get_ignore(source);

		if (ign && ign->time > time(NULL)) {
			log("Ignored message from %s: \"%s\"", source, inbuf);
			return;
		}
	}

	/* If a server is specified (nick@server format), make sure it matches
	 * us, and strip it off. */
	s = strchr(av[0], '@');
	if (s) {
		*s++ = 0;
		if (stricmp(s, ServerName) != 0)
			return;
	}

	starttime = time(NULL);

	if (stricmp(av[0], s_OperServ) == 0) {
		if (is_oper(source)) {
			operserv(source, av[1]);
		} else {
			User *u = finduser(source);

			if (u)
				notice_lang(s_OperServ, u, ACCESS_DENIED);
			else
				notice(s_OperServ, source, "Access denied.");
			if (WallBadOS)
				wallops(s_OperServ,
					"Denied access to %s from %s (non-oper)",
					s_OperServ, source);
		}
	} else if (stricmp(av[0], s_NickServ) == 0) {
		nickserv(source, av[1]);
	} else if (stricmp(av[0], s_ChanServ) == 0) {
		chanserv(source, av[1]);
	} else if (stricmp(av[0], s_MemoServ) == 0) {
		memoserv(source, av[1]);
	} else if (stricmp(av[0], s_HelpServ) == 0) {
		helpserv(s_HelpServ, source, av[1]);
    } else if (s_FloodServ && stricmp(av[0], s_FloodServ) == 0) {
        floodserv(source, av[1]);
	} else if (s_IrcIIHelp && stricmp(av[0], s_IrcIIHelp) == 0) {
		char buf[BUFSIZE];

		snprintf(buf, sizeof(buf), "ircII %s", av[1]);
		helpserv(s_IrcIIHelp, source, buf);
	} else {
      privmsg_flood(source, ac, av);
      return;
    }

	/* Add to ignore list if the command took a significant amount of time. */
	if (allow_ignore) {
		stoptime = time(NULL);
		if (stoptime > starttime && *source && !strchr(source, '.'))
			add_ignore(source, stoptime - starttime);
	}
}

/*************************************************************************/

static void m_quit(char *source, int ac, char **av)
{
	if (ac != 1)
		return;
	do_quit(source, ac, av);
}

/*************************************************************************/

static void m_server(char *source, int ac, char **av)
{
	do_server(source, ac, av);
}

/*************************************************************************/

static void m_squit(char *source, int ac, char **av)
{
	do_squit(source, ac, av);
}

/*************************************************************************/

static void m_stats(char *source, int ac, char **av)
{
	if (ac < 1)
		return;
	switch (*av[0]) {
	case 'u':{
			int uptime = time(NULL) - start_time;

			send_cmd(NULL,
				 "242 %s :Services up %d day%s, %02d:%02d:%02d",
				 source, uptime / 86400,
				 (uptime / 86400 == 1) ? "" : "s",
				 (uptime / 3600) % 24, (uptime / 60) % 60,
				 uptime % 60);
			send_cmd(NULL,
				 "250 %s :Current users: %d (%d ops); maximum %d",
				 source, usercnt, opcnt, maxusercnt);
			send_cmd(NULL, "219 %s u :End of /STATS report.",
				 source);
			break;
		}		/* case 'u' */

	case 'l':
        if (is_oper(source)) {
		send_cmd(NULL,
			 "211 %s Server SendBuf SentBytes SentMsgs RecvBuf "
			 "RecvBytes RecvMsgs ConnTime", source);
		send_cmd(NULL, "211 %s %s %d %d %d %d %d %d %d", source,
			 RemoteServer, read_buffer_len(), total_read, -1,
			 write_buffer_len(), total_written, -1, start_time);
		send_cmd(NULL, "219 %s l :End of /STATS report.", source);
        }
		break;

	case 'c':
	case 'h':
	case 'i':
	case 'k':
	case 'm':
	case 'o':
	case 'y':
		send_cmd(NULL,
			 "219 %s %c :/STATS %c not applicable or not supported.",
			 source, *av[0], *av[0]);
		break;
	}
}

/*************************************************************************/

static void m_time(char *source, int ac, char **av)
{
	time_t t;
	struct tm *tm;
	char buf[64];

	USE_VAR(source);
	USE_VAR(ac);
	USE_VAR(av);

	time(&t);
	tm = localtime(&t);
	strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y %Z", tm);
	send_cmd(NULL, "391 :%s", buf);
}

/*************************************************************************/

static void m_topic(char *source, int ac, char **av)
{
	USE_VAR(source);

	if (ac != 2)
		return;
	do_topic(source, ac, av);
}

/*************************************************************************/

static void m_user(char *source, int ac, char **av)
{
	USE_VAR(source);
	USE_VAR(ac);
	USE_VAR(av);
	/* Do nothing - we get everything we need from the NICK command. */
}

/*************************************************************************/

void m_version(char *source, int ac, char **av)
{
	USE_VAR(ac);
	USE_VAR(av);

	if (source) {
		send_cmd(ServerName, "351 %s blitzed-%s %s", source,
		    VERSION, ServerName);
	}
}

/*************************************************************************/

void m_whois(char *source, int ac, char **av)
{
	const char *clientdesc;

	if (source && ac >= 1) {
		if (stricmp(av[0], s_NickServ) == 0)
			clientdesc = desc_NickServ;
		else if (stricmp(av[0], s_ChanServ) == 0)
			clientdesc = desc_ChanServ;
		else if (stricmp(av[0], s_MemoServ) == 0)
			clientdesc = desc_MemoServ;
		else if (stricmp(av[0], s_HelpServ) == 0)
			clientdesc = desc_HelpServ;
		else if (s_IrcIIHelp && stricmp(av[0], s_IrcIIHelp) == 0)
			clientdesc = desc_IrcIIHelp;
		else if (stricmp(av[0], s_OperServ) == 0)
			clientdesc = desc_OperServ;
		else if (stricmp(av[0], s_GlobalNoticer) == 0)
			clientdesc = desc_GlobalNoticer;
		else if (s_DevNull && stricmp(av[0], s_DevNull) == 0)
			clientdesc = desc_DevNull;
		else {
			send_cmd(ServerName, "401 %s %s :No such service.",
				 source, av[0]);
			return;
		}
		send_cmd(ServerName, "311 %s %s %s %s :%s", source, av[0],
			 ServiceUser, ServiceHost, clientdesc);
		send_cmd(ServerName, "312 %s %s %s :%s", source, av[0],
			 ServerName, ServerDesc);
		send_cmd(ServerName, "318 End of /WHOIS response.");
	}
}

/*************************************************************************/

/************************ Bahamut Specific Functions *********************/

/*************************************************************************/

static void m_sjoin(char *source, int ac, char **av)
{
	if (ac == 3 || ac < 2) {
		if (debug)
			log("SJOIN: expected 2 or >=4 params, got %d", ac);
		return;
	}
	do_sjoin(source, ac, av);
}

static void m_sqline(char *source, int ac, char **av)
{
	if (ac != 2) {
		if (debug)
			log("SQLINE: expected 2 params, for %d", ac);
		return;
	}
	do_sqline(source, ac, av);
}

/*************************************************************************/

/*************************************************************************/

Message messages[] = {

	{"436", m_nickcoll}
	,
	{"AWAY", m_away}
	,
	{"INFO", m_info}
	,
	{"JOIN", m_join}
	,
	{"KICK", m_kick}
	,
	{"KILL", m_kill}
	,
	{"MODE", m_mode}
	,
	{"MOTD", m_motd}
	,
	{"NICK", m_nick}
	,
	{"NOTICE", NULL}
	,
	{"PART", m_part}
	,
	{"PASS", NULL}
	,
	{"PING", m_ping}
	,
	{"PRIVMSG", m_privmsg}
	,
	{"QUIT", m_quit}
	,
	{"SERVER", m_server}
	,
	{"SQUIT", m_squit}
	,
	{"STATS", m_stats}
	,
	{"TIME", m_time}
	,
	{"TOPIC", m_topic}
	,
	{"USER", m_user}
	,
	{"VERSION", m_version}
	,
	{"WALLOPS", NULL}
	,
	{"WHOIS", m_whois}
	,
	{"CAPAB", NULL}
	,
	{"SVINFO", NULL}
	,
	{"SJOIN", m_sjoin}
	,
	{"AKILL", NULL}
	,
	{"GLOBOPS", NULL}
	,
	{"CHATOPS", NULL}
	,
	{"GNOTICE", NULL}
	,
	{"GOPER", NULL}
	,
	{"SQLINE", m_sqline}
	,
	{"RAKILL", NULL}
	,
	{NULL, NULL}

};

/*************************************************************************/

Message *find_message(const char *name)
{
	Message *m;

	for (m = messages; m->name; m++) {
		if (stricmp(name, m->name) == 0)
			return m;
	}
	return NULL;
}

/*************************************************************************/


/* do_motd: dynamic motd showing settings that affect users and telling
 * them who to go to for help.
 * -grifferz
 */
void do_motd(const char *source)
{
	send_cmd(ServerName,
		 "372 %s :- Default settings on nick registration:", source);
	if (NSDefEnforceQuick) {
		send_cmd(ServerName,
			 "372 %s :-     NickServ SET ENFORCE QUICK", source);
	} else if (NSDefEnforce) {
		send_cmd(ServerName,
			 "372 %s :-     NickServ SET ENFORCE ON", source);
	}
	if (NSDefSecure) {
		send_cmd(ServerName,
			 "372 %s :-     NickServ SET SECURE ON", source);
	}
	if (NSDefPrivate) {
		send_cmd(ServerName,
			 "372 %s :-     NickServ SET PRIVATE ON", source);
	}
	if (NSDefHideEmail) {
		send_cmd(ServerName,
			 "372 %s :-     NickServ SET HIDE EMAIL ON", source);
	}
	if (NSDefHideUsermask) {
		send_cmd(ServerName,
			 "372 %s :-     NickServ SET HIDE USERMASK ON", source);
	}
	if (NSDefHideQuit) {
		send_cmd(ServerName,
			 "372 %s :-     NickServ SET HIDE QUIT ON", source);
	}
	if (NSDefMemoSignon && NSDefMemoReceive) {
		send_cmd(ServerName,
			 "372 %s :-     MemoServ SET NOTIFY ON", source);
	} else if (NSDefMemoSignon) {
		send_cmd(ServerName,
			 "372 %s :-     MemoServ SET NOTIFY LOGON", source);
	} else if (NSDefMemoReceive) {
		send_cmd(ServerName,
			 "372 %s :-     MemoServ SET NOTIFY NEW", source);
	}

	send_cmd(ServerName, "372 %s :-", source);

	send_cmd(ServerName,
		 "372 %s :- Nicknames expire after \2%d\2 days of non-use.",
		 source, NSExpire / 60 / 60 / 24);

	send_cmd(ServerName,
		 "372 %s :- Channels expire after \2%d\2 days of non-use.",
		 source, CSExpire / 60 / 60 / 24);
	send_cmd(ServerName,
		 "372 %s :- Nickname access lists are restricted to \2%d\2 entries.",
		 source, NSAccessMax);
	send_cmd(ServerName,
		 "372 %s :- Channel access lists are restricted to \2%d\2 entries.",
		 source, CSAccessMax);
	send_cmd(ServerName,
		 "372 %s :- Channel AKICK lists are restricted to \2%d\2 entries.",
		 source, CSAutokickMax);
	send_cmd(ServerName,
		 "372 %s :- You may keep up to \2%d\2 memos.",
		 source, MSMaxMemos);
	if (DefSessionLimit) {
		send_cmd(ServerName,
			 "372 %s :- You may load up to \2%d\2 clones.",
			 source, DefSessionLimit);
	}

	send_cmd(ServerName, "372 %s :-", source);

	send_cmd(ServerName,
		 "372 %s :- The following people can recover lost passwords,",
		 source);
	send_cmd(ServerName,
		 "372 %s :- \2bold\2 nicks are currently online:",
		 source);
}
