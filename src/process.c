/* Main processing code for Services.
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

RCSID("$Id$");

/*************************************************************************/
/*************************************************************************/

/* Use ignore code? */
int allow_ignore = 1;

/* People to ignore (hashed by first character of nick). */
IgnoreData *ignore[256];

/*************************************************************************/

/* add_ignore: Add someone to the ignorance list for the next `delta'
 *             seconds.
 */

void add_ignore(const char *nick, time_t delta)
{
    IgnoreData *ign;
    char who[NICKMAX];
    time_t now = time(NULL);
    IgnoreData **whichlist = &ignore[tolower(nick[0])];

    strscpy(who, nick, NICKMAX);
    for (ign = *whichlist; ign; ign = ign->next) {
	if (stricmp(ign->who, who) == 0)
	    break;
    }
    if (ign) {
	if (ign->time > now)
	    ign->time += delta;
	else
	    ign->time = now + delta;
    } else {
	ign = smalloc(sizeof(*ign));
	strscpy(ign->who, who, sizeof(ign->who));
	ign->time = now + delta;
	ign->next = *whichlist;
	*whichlist = ign;
    }
}

/*************************************************************************/

/* get_ignore: Retrieve an ignorance record for a nick.  If the nick isn't
 *             being ignored, return NULL and flush the record from the
 *             in-core list if it exists (i.e. ignore timed out).
 */

IgnoreData *get_ignore(const char *nick)
{
    IgnoreData *ign, *prev;
    time_t now = time(NULL);
    IgnoreData **whichlist = &ignore[tolower(nick[0])];

    for (ign = *whichlist, prev = NULL; ign; prev = ign, ign = ign->next) {
	if (irc_stricmp(ign->who, nick) == 0)
	    break;
    }
    if (ign && ign->time <= now) {
	if (prev)
	    prev->next = ign->next;
	else
	    *whichlist = ign->next;
	free(ign);
	ign = NULL;
    }
    return ign;
}

/*************************************************************************/
/*************************************************************************/

/* split_buf:  Split a buffer into arguments and store the arguments in an
 *             argument vector pointed to by argv (which will be malloc'd
 *             as necessary); return the argument count.  If colon_special
 *             is non-zero, then treat a parameter with a leading ':' as
 *             the last parameter of the line, per the IRC RFC.  Destroys
 *             the buffer by side effect.
 */

int split_buf(char *buf, char ***argv, int colon_special)
{
	size_t argvsize;
	unsigned int argc;
	char *s;

	USE_VAR(colon_special);

	argvsize = 8;

	*argv = smalloc(sizeof(char *) * argvsize);
	argc = 0;

	while (*buf) {
		if (argc == argvsize) {
			argvsize += 8;
			*argv = srealloc(*argv, sizeof(char *) * argvsize);
		}
		
		if (*buf == ':') {
			(*argv)[argc++] = buf+1;
			buf = "";
		} else {
			s = strpbrk(buf, " ");

			if (s) {
				*s++ = 0;

				while (isspace(*s))
					s++;
			} else {
				s = buf + strlen(buf);
			}

			(*argv)[argc++] = buf;
			buf = s;
		}
	}

	return(argc);
}

/*************************************************************************/

/* process:  Main processing routine.  Takes the string in inbuf (global
 *           variable) and does something appropriate with it. */

void process()
{
	char buf[512];		/* Longest legal IRC command line */
	char source[64];
	char cmd[64];
	int ac;			/* Parameters for the command */
	char *s, *bufptr;
	char **av;
	Message *m;


	/* If debugging, log the buffer. */
	if (debug)
		log("debug: Received: %s", inbuf);

	/*
	 * First make a copy of the buffer so we have the original in case we
	 * crash - in that case, we want to know what we crashed on.
	 */

	/*
	 * XXX - Sometimes we seem to get a -1 byte at the start of the buffer
	 * when log rotating.  This is a hack to get around it until I get to
	 * the bottom of it.
	 */
	bufptr = inbuf;
	if (inbuf[0] == (char)-1) {
		snoop(s_OperServ,
		    "[NET] Got that weird read of -1 from network (%s)",
		    inbuf);
		log("Got that weird read of -1 from network (%s)", inbuf);
		bufptr++;
	}

	strscpy(buf, bufptr, sizeof(buf));

	/* Split the buffer into pieces. */
	if (*buf == ':') {
		s = strpbrk(buf, " ");

		if (!s)
			return;

		*s = 0;

		while (isspace(*++s))
			;

		strscpy(source, buf+1, sizeof(source));
		memmove(buf, s, strlen(s)+1);
	} else {
		*source = 0;
	}

	if (!*buf)
		return;

	s = strpbrk(buf, " ");

	if (s) {
		*s = 0;

		while (isspace(*++s))
			;
	} else {
		s = buf + strlen(buf);
	}

	strscpy(cmd, buf, sizeof(cmd));
	ac = split_buf(s, &av, 1);

	/* Do something with the message. */
	m = find_message(cmd);

	if (m) {
		if (m->func)
			m->func(source, ac, av);
	} else {
		snoop(s_OperServ, "[NET] Unknown message from server (%s)",
		    bufptr);
		log("unknown message from server (%s)", bufptr);
	}

	/* Free argument list we created. */
	free(av);
}

/*************************************************************************/
