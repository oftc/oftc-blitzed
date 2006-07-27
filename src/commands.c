
/* Routines for looking up commands in a *Serv command list.
 *
 * Services is copyright (c) 1996-1999 Andrew Church.
 *     E-mail: <achurch@dragonfire.net>
 * Services is copyright (c) 1999-2000 Andrew Kempe.
 *     E-mail: <theshadow@shadowfire.org>
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include "services.h"
#include "commands.h"
#include "language.h"

RCSID("$Id$");

/*************************************************************************/

/* Return the Command corresponding to the given name, or NULL if no such
 * command exists.
 */

Command *lookup_cmd(Command * list, const char *cmd)
{
	Command *c;

	for (c = list; c->name; c++) {
		if (stricmp(c->name, cmd) == 0)
			return c;
	}
	return NULL;
}

/*************************************************************************/

/* Run the routine for the given command, if it exists and the user has
 * privilege to do so; if not, print an appropriate error message.
 */

void run_cmd(const char *service, User * u, Command * list, const char *cmd)
{
	Command *c = lookup_cmd(list, cmd);

	if (c && c->routine) {
		if ((c->has_priv == NULL) || c->has_priv(u))
			c->routine(u);
		else
			notice_lang(service, u, ACCESS_DENIED);
	} else {
		notice_lang(service, u, UNKNOWN_COMMAND_HELP, cmd, service);
	}
}

/*************************************************************************/

/* Print a help message for the given command. */

void help_cmd(const char *service, User * u, Command * list, const char *cmd)
{
	const char *p1, *p2, *p3, *p4;
	char *newcmd;
	Command *c;

	newcmd = (char *)cmd + strspn(cmd, " ");
	c = lookup_cmd(list, newcmd);

	if (c) {
		p1 = c->help_param1;
		p2 = c->help_param2;
		p3 = c->help_param3;
		p4 = c->help_param4;

		if (c->helpmsg_all >= 0) {
			notice_help(service, u,
			    (unsigned int)c->helpmsg_all, p1, p2, p3, p4);
		}

		if (is_services_root(u)) {
			if (c->helpmsg_root >= 0) {
				notice_help(service, u,
				    (unsigned int)c->helpmsg_root, p1, p2,
				    p3, p4);
			} else if (c->helpmsg_all < 0) {
				notice_lang(service, u, NO_HELP_AVAILABLE,
				    cmd);
			}
		} else if (is_services_admin(u)) {
			if (c->helpmsg_admin >= 0) {
				notice_help(service, u,
				    (unsigned int)c->helpmsg_admin, p1, p2,
				    p3, p4);
			} else if (c->helpmsg_all < 0) {
				notice_lang(service, u, NO_HELP_AVAILABLE,
				    cmd);
			}
		} else if (is_services_oper(u)) {
			if (c->helpmsg_oper >= 0) {
				notice_help(service, u,
				    (unsigned int)c->helpmsg_oper, p1, p2,
				    p3, p4);
			} else if (c->helpmsg_all < 0) {
				notice_lang(service, u, NO_HELP_AVAILABLE,
				    cmd);
			}
		} else {
			if (c->helpmsg_reg >= 0) {
				notice_help(service, u,
				    (unsigned int)c->helpmsg_reg, p1, p2,
				    p3, p4);
			} else if (c->helpmsg_all < 0) {
				notice_lang(service, u, NO_HELP_AVAILABLE,
				    cmd);
			}
		}

	} else {

		notice_lang(service, u, NO_HELP_AVAILABLE, cmd);

	}
}

/*************************************************************************/
