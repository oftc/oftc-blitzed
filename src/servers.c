
/*
 * Routines to maintain a list of online servers.
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
#include "pseudo.h"

RCSID("$Id$");

/*************************************************************************/

static Server *serverlist;	/* Pointer to first server in the list */
static int16 servercnt = 0;	/* Number of online servers */

/*
 * Points to the server last returned by findserver(). This should improve
 * performance during netbursts of NICKs.
 */

static Server *lastserver = NULL;

static void recursive_squit(Server * parent, const char *reason);

/*************************************************************************/

/*************************************************************************/

/****************************** Statistics *******************************/

/*************************************************************************/

/* Return information on memory use. Assumes pointers are valid. */

void get_server_stats(long *nservers, long *memuse)
{
	Server *server;
	long mem;

	mem = sizeof(Server) * servercnt;
	for (server = serverlist; server; server = server->next)
		mem += strlen(server->name) + 1;

	*nservers = servercnt;
	*memuse = mem;
}

/*************************************************************************/

#ifdef DEBUG_COMMANDS
void send_server_list(User * user)
{
	Server *server;

	notice(s_OperServ, user->nick, "Servers: %d", servercnt);
	for (server = serverlist; server; server = server->next) {
		notice(s_OperServ, user->nick, "%s [%s] {%s}", server->name,
		       server->hub ? server->hub->name : "no hub",
		       server->child ? server->child->name : "no child");

	}
}

#endif				/* DEBUG_COMMANDS */

/*************************************************************************/

/**************************** Internal Functions *************************/

/*************************************************************************/

/* Allocate a new Server structure, fill in basic values, link it to the
 * overall list, and return it. Always successful.
 */

static Server *new_server(const char *servername)
{
	Server *server;

	servercnt++;
	server = scalloc(sizeof(Server), 1);
	server->name = sstrdup(servername);

	server->next = serverlist;
	if (server->next)
		server->next->prev = server;
	serverlist = server;

	return server;
}

/* Remove and free a Server structure. */

static void delete_server(Server * server)
{
	if (debug >= 2)
		log("debug: delete_server() called");

	if (server == lastserver)
		lastserver = NULL;

	servercnt--;
	free(server->name);
	if (server->prev)
		server->prev->next = server->next;
	else
		serverlist = server->next;
	if (server->next)
		server->next->prev = server->prev;
	free(server);

	if (debug >= 2)
		log("debug: delete_server() done");
}

/*************************************************************************/

/**************************** External Functions *************************/

/*************************************************************************/

Server *findserver(const char *servername)
{
	Server *server;

	if (!servername)
		return NULL;

	if (lastserver && stricmp(servername, lastserver->name) == 0)
		return lastserver;

	for (server = serverlist; server; server = server->next) {
		if (stricmp(servername, server->name) == 0) {
			lastserver = server;
			return server;
		}
	}

	return NULL;
}

/*************************************************************************/

/*************************************************************************/

/* Handle a server SERVER command.
 * 	source = server's hub; !*source indicates this is our hub.
 * 	av[0]  = server's name
 */

void do_server(const char *source, int ac, char **av)
{
	Server *server, *tmpserver;

	USE_VAR(ac);

	server = new_server(av[0]);
	server->t_join = time(NULL);
	server->child = NULL;
	server->sibling = NULL;

	if (!*source) {
		send_sqlines();
		return;
	}

	server->hub = findserver(source);
	if (!server->hub) {
		/*
		 * FIXME: This should NEVER EVER happen - but it's here while
		 * this function is being developed. Remove it someday.  
		 * 
		 * I've heard that on older ircds it is possible for "source"
		 * not to be the new server's hub. This will cause problems.
		 * -TheShadow
		 */
		wallops(s_OperServ,
			"WARNING: Could not find server \2%s\2 which is supposed to "
			"be the hub for \2%s\2", source, av[0]);
		log("server: could not find hub %s for %s", source, av[0]);
		return;
	}

	if (!server->hub->child) {
		server->hub->child = server;
	} else {
		tmpserver = server->hub->child;
		while (tmpserver->sibling)
			tmpserver = tmpserver->sibling;
		tmpserver->sibling = server;
	}

	return;
}

/*************************************************************************/

/* "SQUIT" all servers who are linked to us via the specified server by
 * deleting them from the server list. The parent server is not deleted,
 * so this must be done by the calling function.
 */

static void recursive_squit(Server * parent, const char *reason)
{
	Server *server, *nextserver;

	server = parent->child;
	if (debug >= 2)
		log("recursive_squit, parent: %s", parent->name);
	while (server) {
		if (server->child)
			recursive_squit(server, reason);
		if (debug >= 2)
			log("recursive_squit, child: %s", server->name);
		nextserver = server->sibling;
		delete_server(server);
		server = nextserver;
	}
}

/* Handle a server SQUIT command.
 * 	av[0] = server's name
 * 	av[1] = quit message 
 */

void do_squit(const char *source, int ac, char **av)
{
	Server *server, *tmpserver;

	USE_VAR(source);
	USE_VAR(ac);

	server = findserver(av[0]);

	if (server) {
		recursive_squit(server, av[1]);
		if (server->hub) {
			if (server->hub->child == server) {
				server->hub->child = server->sibling;
			} else {
				for (tmpserver = server->hub->child;
				     tmpserver->sibling;
				     tmpserver = tmpserver->sibling) {
					if (tmpserver->sibling == server) {
						tmpserver->sibling =
						    server->sibling;
						break;
					}
				}
			}
			delete_server(server);
		}

	} else {
		wallops(s_OperServ,
			"WARNING: Tried to quit non-existent server: \2%s",
			av[0]);
		log("server: Tried to quit non-existent server: %s", av[0]);
		return;
	}
}

/*************************************************************************/
