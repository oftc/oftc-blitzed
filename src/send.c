
/* Routines for sending stuff to the network.
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

RCSID("$Id$");

/*************************************************************************/

/* Send a command to the server.  The two forms here are like
 * printf()/vprintf() and friends. */

void send_cmd(const char *source, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vsend_cmd(source, fmt, args);
	va_end(args);
}

void vsend_cmd(const char *source, const char *fmt, va_list args)
{
	char buf[BUFSIZE];

	vsnprintf(buf, sizeof(buf), fmt, args);
	if (source) {
		sockprintf(servsock, ":%s %s\r\n", source, buf);
		if (debug)
			log("debug: Sent: :%s %s", source, buf);
	} else {
		sockprintf(servsock, "%s\r\n", buf);
		if (debug)
			log("debug: Sent: %s", buf);
	}
}

/*************************************************************************/

/* Send out a WALLOPS (a GLOBOPS on ircd.dal). */

void wallops(const char *source, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];

	va_start(args, fmt);
	snprintf(buf, sizeof(buf), "GNOTICE %s 1 :%s", ServerName, fmt);
	vsend_cmd(source ? source : ServerName, buf, args);
}

/*************************************************************************/


/* Send a NOTICE from the given source to the given nick. */
void notice(const char *source, const char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];

	va_start(args, fmt);
	snprintf(buf, sizeof(buf), "NOTICE %s :%s", dest, fmt);
	vsend_cmd(source, buf, args);
}


/* Send a NOTICE to the operators of a channel. */
void opnotice(const char *source, const char *chan, const char *fmt, ...)
{
	va_list args;
	char channel[2 * CHANMAX];
	char buf[BUFSIZE];

	strncpy(channel, chan, sizeof(channel));
	strnrepl(channel, sizeof(channel), "%", "%%");

	va_start(args, fmt);
	snprintf(buf, sizeof(buf), "NOTICE @%s :%s", channel, fmt);
	vsend_cmd(source, buf, args);
}


/* Send a NOTICE to every user. */
void global_notice(const char *source, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];

	va_start(args, fmt);
#ifdef NETWORK_DOMAIN
	snprintf(buf, sizeof(buf), "NOTICE $*.%s :%s", NETWORK_DOMAIN, fmt);
	vsend_cmd(source? source : ServerName, buf, args);
#else
	/* Go through all common top-level domains.  If you have others,
	 * add them here.
	 */
	snprintf(buf, sizeof(buf), "NOTICE $*.com :%s", fmt);
	vsend_cmd(source? source : ServerName, buf, args);
	snprintf(buf, sizeof(buf), "NOTICE $*.net :%s", fmt);
	vsend_cmd(source? source : ServerName, buf, args);
	snprintf(buf, sizeof(buf), "NOTICE $*.org :%s", fmt);
	vsend_cmd(source? source : ServerName, buf, args);
	snprintf(buf, sizeof(buf), "NOTICE $*.edu :%s", fmt);
	vsend_cmd(source? source : ServerName, buf, args);
#endif
}

/* Send a NULL-terminated array of text as NOTICEs. */
void notice_list(const char *source, const char *dest, const char **text)
{
	while (*text) {
		/* Have to kludge around an ircII bug here: if a notice includes
		 * no text, it is ignored, so we replace blank lines by lines
		 * with a single space.
		 */
		if (**text)
			notice(source, dest, *text);
		else
			notice(source, dest, " ");
		text++;
	}
}


/* Send a message in the user's selected language to the user using NOTICE. */
void notice_lang(const char *source, User * dest, unsigned int message,
    ...)
{
	va_list args;
	char buf[4096];		/* because messages can be really big */
	char *s, *t;
	const char *fmt;

	if (!dest)
		return;
	va_start(args, message);
	fmt = getstring(dest->nick_id, message);
	if (!fmt)
		return;
	vsnprintf(buf, sizeof(buf), fmt, args);
	s = buf;
	while (*s) {
		t = s;
		s += strcspn(s, "\n");
		if (*s)
			*s++ = 0;
		send_cmd(source, "NOTICE %s :%s", dest->nick, *t ? t : " ");
	}
}


/* Like notice_lang(), but replace %S by the source.  This is an ugly hack
 * to simplify letting help messages display the name of the pseudoclient
 * that's sending them.
 */
void notice_help(const char *source, User * dest, unsigned int message,
    ...)
{
	va_list args;
	char buf[4096], buf2[4096], outbuf[BUFSIZE];
	char *s, *t;
	const char *fmt;

	if (!dest)
		return;
	va_start(args, message);
	fmt = getstring(dest->nick_id, message);
	if (!fmt)
		return;
	/* Some sprintf()'s eat %S or turn it into just S, so change all %S's
	 * into \1\1... we assume this doesn't occur anywhere else in the
	 * string. */
	strscpy(buf2, fmt, sizeof(buf2));
	strnrepl(buf2, sizeof(buf2), "%S", "\1\1");
	vsnprintf(buf, sizeof(buf), buf2, args);
	s = buf;
	while (*s) {
		t = s;
		s += strcspn(s, "\n");
		if (*s)
			*s++ = 0;
		strscpy(outbuf, t, sizeof(outbuf));
		strnrepl(outbuf, sizeof(outbuf), "\1\1", source);
		send_cmd(source, "NOTICE %s :%s", dest->nick,
			 *outbuf ? outbuf : " ");
	}
}

/*************************************************************************/

/* Send a PRIVMSG from the given source to the given nick. */
void privmsg(const char *source, const char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];

	va_start(args, fmt);
	snprintf(buf, sizeof(buf), "PRIVMSG %s :%s", dest, fmt);
	vsend_cmd(source, buf, args);
}

/*************************************************************************/

void send_nick(const char *nick, const char *user, const char *host,
	       const char *server, const char *name)
{
	// NICK <nick> <hops> <TS> <umode> <user> <host> <server> <svsid> <IP> :<ircname>
	send_cmd(NULL, "NICK %s 1 %d + %s %s %s :%s", nick, time(NULL),
		 user, host, server, name);
}

/*************************************************************************/
