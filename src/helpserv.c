
/*
 * HelpServ functions.
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 *     E-mail: services@lists.blitzed.org
 *
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
#include "language.h"
#include <sys/stat.h>

RCSID("$Id$");

static void do_help(const char *whoami, const char *source, char *topic);

/*************************************************************************/

/* helpserv:  Main HelpServ routine.  `whoami' is what nick we should send
 * messages as: this won't necessarily be s_HelpServ, because other
 * routines call this one to display help files. */

void helpserv(const char *whoami, const char *source, char *buf)
{
	char *cmd, *topic, *s;

	topic = buf ? sstrdup(buf) : NULL;
	cmd = strtok(buf, " ");
	if (cmd && stricmp(cmd, "\1PING") == 0) {
		if (!(s = strtok(NULL, "")))
			s = "\1";
		notice(s_HelpServ, source, "\1PING %s", s);
	} else if (stricmp(cmd, "\1VERSION\1") == 0) {
		notice(s_HelpServ, source, "\1VERSION Blitzed-Based OFTC IRC Services v%s\1", VERSION);
	} else {
		do_help(whoami, source, topic);
	}
	if (topic)
		free(topic);
}

/*************************************************************************/

/*********************** HelpServ command routines ***********************/

/*************************************************************************/

/* Return a help message. */

static void do_help(const char *whoami, const char *source, char *topic)
{
	char buf[256];
	struct stat st;
	/* An unclobbered (by strtok) copy. */
	char *old_topic;
	char *ptr, *s;
	User *u;
	FILE *f;

	u = finduser(source);

	if (!topic || !*topic)
		topic = "help";

	old_topic = sstrdup(topic);

	/*
	 * As we copy path parts, (1) lowercase everything and (2) make sure we
	 * don't let any special characters through -- this includes '.' (which
	 * could get parent dir) or '/' (which couldn't _really_ do anything
	 * nasty if we keep '.' out, but better to be on the safe side).
	 * Special characters turn into '_'.
	 */
	memset(buf, 0, sizeof(buf));
	strscpy(buf, HelpDir, sizeof(buf));
	ptr = buf + strlen(buf);
	for (s = strtok(topic, " ");
	    s && (size_t)(ptr - buf) < sizeof(buf) - 1;
	    s = strtok(NULL, " ")) {
		*ptr++ = '/';
		while (*s && (size_t)(ptr - buf) < sizeof(buf) - 1) {
			if (*s == '.' || *s == '/')
				*ptr++ = '_';
			else
				*ptr++ = tolower(*s);
			++s;
		}
		*ptr = 0;
	}

	/*
	 * If we end up at a directory, go for an "index" file/dir if
	 * possible.
	 */
	while ((size_t)(ptr - buf) < sizeof(buf) - 1 &&
	    stat(buf, &st) == 0 && S_ISDIR(st.st_mode)) {
		*ptr++ = '/';
		strscpy(ptr, "index", sizeof(buf) - (ptr - buf));
		ptr += strlen(ptr);
	}

	/* Send the file, if it exists. */
	if (!(f = fopen(buf, "r"))) {
		if (debug)
			log_perror("debug: Cannot open help file %s", buf);
		if (u) {
			notice_lang(whoami, u, NO_HELP_AVAILABLE,
			    old_topic);
		} else {
			notice(whoami, source,
			    "Sorry, no help available for \2%s\2.",
			    old_topic);
		}
		free(old_topic);
		return;
	}
	while (fgets(buf, sizeof(buf), f)) {
		s = strtok(buf, "\n");
		/*
		 * Use this odd construction to prevent any %'s in the text
		 * from doing weird stuff to the output.  Also replace blank
		 * lines by spaces (see send.c/notice_list() for an explanation
		 * of why).
		 */
		notice(whoami, source, "%s", s ? s : " ");
	}
	fclose(f);
	free(old_topic);
}

/*************************************************************************/
