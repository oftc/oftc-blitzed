/* Initalization and related routines.
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

RCSID("$Id$");

/*************************************************************************/

/* Send a NICK command to the server with the appropriate formatting for
 * the type of server it is (dalnet, ircu, ircii, bahamut etc).
 */

// NICK <nick> <hops> <TS> <umode> <user> <host> <server> <svsid> :<ircname>
#  define NICK(nick,name) \
    do { \
        send_cmd(NULL, "NICK %s 1 %ld + %s %s %s :%s", (nick), time(NULL), \
                ServiceUser, ServiceHost, ServerName, (name)); \
    } while (0)
