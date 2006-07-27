/*
 * Various routines to perform simple actions.
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

/* Remove a user from the IRC network.  `source' is the nick which should
 * generate the kill, or NULL for a server-generated kill.
 */

void kill_user(const char *source, const char *user, const char *reason)
{
    char *av[2];
    char buf[BUFSIZE];

    if (!user || !*user)
	return;
    if (!source || !*source)
	source = ServerName;
    if (!reason)
	reason = "";
    snprintf(buf, sizeof(buf), "%s", reason);
    av[0] = (char *)user;
    av[1] = buf;
    send_cmd(source, "KILL %s :%s (%s)", user, source, av[1]);
    do_kill(source, 2, av);
}

/*************************************************************************/

/*
 * Remove a user from the IRC network using the SVSKILL command which
 * instructs the user's uplink server to kill them rather than us.  This
 * differs from a normal kill as follows:
 * 	a) It does not "chase"
 * 	b) It does not produce a server notice
 */

void svskill(const char *source, const char *user, const char *reason)
{
	char buf[BUFSIZE];
	char *av[2];

	if (!user || !*user)
		return;

	if (!source || !*source)
		source = ServerName;

	if (!reason)
		reason = "";

	snprintf(buf, sizeof(buf), "%s", reason);

	av[0] = (char *)user;
	av[1] = buf;
	send_cmd(source, "SVSKILL %s :%s", user, av[1]);
	do_kill(source, 2, av);
}

/*************************************************************************/

/* Note a bad password attempt for the given user.  If they've used up
 * their limit, toss them off.
 */

void bad_password(User *u)
{
    time_t now = time(NULL);

    if (!BadPassLimit)
	return;

    if (BadPassTimeout > 0 && u->invalid_pw_time > 0
			&& u->invalid_pw_time < now - BadPassTimeout)
	u->invalid_pw_count = 0;
    u->invalid_pw_count++;
    u->invalid_pw_time = now;
    if (u->invalid_pw_count >= BadPassLimit)
	kill_user(NULL, u->nick, "Too many invalid passwords");
}

/*************************************************************************/
