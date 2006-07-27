/* Routines for time-delayed actions.
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
#include "timeout.h"

RCSID("$Id$");

static Timeout *timeouts = NULL;

/*************************************************************************/

#ifdef DEBUG_COMMANDS

/* Send the timeout list to the given user. */

void send_timeout_list(User *u)
{
    Timeout *to, *last;

    notice(s_OperServ, u->nick, "Now: %d", time(NULL));
    for (to = timeouts, last = NULL; to; last = to, to = to->next) {
	notice(s_OperServ, u->nick, "%p: %d: %p (%p)",
			to, to->timeout, to->code, to->data);
	if (to->prev != last)
	    notice(s_OperServ, u->nick,
			"    to->prev incorrect!  expected=%p seen=%p",
			last, to->prev);
    }
}

#endif	/* DEBUG_COMMANDS */

/*************************************************************************/

/* Check the timeout list for any pending actions. */

void check_timeouts(void)
{
    Timeout *to, *to2;
    time_t t = time(NULL);

    if (debug >= 2)
	log("debug: Checking timeouts at %d", t);

    to = timeouts;
    while (to) {
	if (t < to->timeout) {
	    to = to->next;
	    continue;
	}
	if (debug >= 4) {
	    log("debug: Running timeout %p (code=%p repeat=%d)",
			to, to->code, to->repeat);
	}
	to->code(to);
	if (to->repeat) {
	    to = to->next;
	    continue;
	}
	to2 = to->next;
	if (to->next)
	    to->next->prev = to->prev;
	if (to->prev)
	    to->prev->next = to->next;
	else
	    timeouts = to->next;
	free(to);
	to = to2;
    }
    if (debug >= 2)
	log("debug: Finished timeout list");
}

/*************************************************************************/

/* Add a timeout to the list to be triggered in `delay' seconds.  If
 * `repeat' is nonzero, do not delete the timeout after it is triggered.
 * This must maintain the property that timeouts added from within a
 * timeout routine do not get checked during that run of the timeout list.
 */

Timeout *add_timeout(int delay, void (*code)(Timeout *), int repeat)
{
    Timeout *t = smalloc(sizeof(Timeout));
    t->settime = time(NULL);
    t->timeout = t->settime + delay;
    t->code = code;
    t->repeat = repeat;
    t->next = timeouts;
    t->prev = NULL;
    if (timeouts)
	timeouts->prev = t;
    timeouts = t;
    return t;
}

/*************************************************************************/

/* Remove a timeout from the list (if it's there). */

void del_timeout(Timeout *t)
{
    Timeout *ptr;

    for (ptr = timeouts; ptr; ptr = ptr->next) {
	if (ptr == t)
	    break;
    }
    if (!ptr)
	return;
    if (t->prev)
	t->prev->next = t->next;
    else
	timeouts = t->next;
    if (t->next)
	t->next->prev = t->prev;
    free(t);
}

/*************************************************************************/
