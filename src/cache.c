/*
 * Local caching of SQL data.
 *
 * The idea being that certain SQL queries are made very very often and the
 * data they return can usually only be set whilst on IRC, or is relevant
 * only when on IRC.
 *
 * For example, language setting.  This changes nothing except what
 * language services notices are sent in, it is needless to get it from
 * SQL every single time.
 *
 * Therefore, language settings and nick status will now be cached within
 * the services process.  Every time their value is looked up it will first
 * be looked up in the cache.  If an entry less than one hour old appears
 * there then it will be used.  Otherwise it will be fetched fresh from
 * SQL, stored in the cache and then returned.
 *
 * Blitzed Services is copyright (c) 2000-2002 Blitzed Services Team
 *     E-mail: <services@lists.blitzed.org>
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include "services.h"

#define M 151
#define HASH(id)	((id) % M)

RCSID("$Id$");

struct nickcache {
	struct nickcache *next;
	unsigned int nick_id;
	int status;
	time_t status_timestamp;
	unsigned short int language;
	time_t language_timestamp;
};

static struct nickcache *nickcacheheads[M];

static struct nickcache *nickcache_insert(unsigned int nick_id);
static void dump_cache_chain(unsigned int head_id);
static void nickcache_del(unsigned int nick_id);
static struct nickcache *nickcache_find(unsigned int nick_id);

/*************************************************************************/
/*************************************************************************/

void nickcache_init()
{
	int i;

	for (i = 0; i < M; i++) {
		nickcacheheads[i] = smalloc(sizeof(struct nickcache));
		nickcacheheads[i]->next = nickcacheheads[i];
		nickcacheheads[i]->nick_id = 0;
	}
}

/* delete the entire nick cache hash */
void nickcache_delall()
{
	int i;
	struct nickcache *nc, *x;
	
	for (i = 0; i < M; i++) {
		nc = nickcacheheads[i];
		while (nc->next != nickcacheheads[i]) {
			x = nc;
			nc = nc->next;
			free(x);
		}
		free(nc);
	}
}

/* insert a nick cache into the hash */
static struct nickcache *nickcache_insert(unsigned int nick_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	MYSQL_RES *result;
	struct nickcache *head, *prev, *nc, *new;

	assert(nick_id);

	new = smalloc(sizeof *new);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT status, language FROM nick WHERE nick_id=%u", nick_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		new->nick_id = nick_id;
		new->status_timestamp = new->language_timestamp = time(NULL);
		new->status = atoi(row[0]);
		new->language = atoi(row[1]);

		if (debug > 1) {
			snoop(s_OperServ, "[NICKCACHE] Inserting entry for "
			    "nick_id %u, status = %d, language = %d",
			    new->nick_id, new->status, new->language);
			log("[NICKCACHE] Inserting entry for nick_id %u, "
			    "status = %d, language = %d", new->nick_id,
			    new->status, new->language);
		}
	} else {
		mysql_free_result(result);
		log("nickcache: tried to insert nonexistent nick ID %u!",
		    nick_id);
		snoop(s_OperServ, "[NICKCACHE] Tried to insert nonexistent "
		    "nick ID %u!", nick_id);
		return(NULL);
	}
	
	mysql_free_result(result);
	
	prev = head = nickcacheheads[HASH(nick_id)];
	nc = head->next;

	while (nc != head && nc->nick_id < new->nick_id) {
		prev = nc;
		nc = nc->next;
	}
	
	new->next = prev->next;
	prev->next = new;

	if (debug > 1)
		dump_cache_chain(HASH(nick_id));

	return(new);
}

static void dump_cache_chain(unsigned int head_id)
{
	struct nickcache *head, *nc;

	snoop(s_OperServ, "[NICKCACHE] Nicks on cache chain %u:", head_id);
	log("[NICKCACHE] Nicks on cache chain %u:", head_id);

	head = nc = nickcacheheads[head_id];

	do {
		snoop(s_OperServ, "[NICKCACHE]     %u", nc->nick_id);
		log("[NICKCACHE]     %u", nc->nick_id);
		nc = nc->next;
	} while (nc->next != head);

	snoop(s_OperServ, "[NICKCACHE]     %u", nc->nick_id);
	log("[NICKCACHE]     %u", nc->nick_id);
}

/* find a nick cache entry and return it */
struct nickcache *nickcache_find(unsigned int nick_id)
{
	struct nickcache *head, *nc;

	assert(nick_id);
	
	if (debug > 1) {
		snoop(s_OperServ, "[NICKCACHE] Searching for entry for "
		    "nick_id %u", nick_id);
		log("[NICKCACHE] Searching for entry for nick_id %u",
		    nick_id);
	}

	head = nickcacheheads[HASH(nick_id)];
	nc = head->next;

	while (1) {
		if (nc == head ||		/* end of list */
		    nc->nick_id > nick_id) {	/* can't be present */
			if (debug > 1) {
				snoop(s_OperServ, "[NICKCACHE] Entry for "
				    "nick_id %u not found!", nick_id);
				log("[NICKCACHE] Entry for nick_id %u not "
				    "found!", nick_id);
			}
			return(NULL);
		} else if (nc->nick_id == nick_id) {
			if (debug > 1) {
				snoop(s_OperServ,
				    "[NICKCACHE]     Find %u, status = %d, "
				    "language = %d", nc->nick_id,
				    nc->status, nc->language);
				log("[NICKCACHE]     Find %u, status = %d, "
				    "language = %d", nc->nick_id,
				    nc->status, nc->language);
			}
			return(nc);
		} else {
			nc = nc->next;
		}
	}
}

/* remove a nick cache entry */
static void nickcache_del(unsigned int nick_id)
{
	struct nickcache *nc, *z;

	assert(nick_id);

	z = nc = nickcacheheads[HASH(nick_id)];
	
	while (nc->nick_id != nick_id) {
		if (nc == nc->next ||		/* end of list */
		    nc->nick_id > nick_id) {	/* can't be present */
			return;
		}
		z = nc;
		nc = nc->next;
	}

	nc->next = nc->next->next;
	free(z);
}	

/* update the contents of the cache from MySQL - nick_id must be present
 * in the cache */
struct nickcache *nickcache_update(unsigned int nick_id)
{
	MYSQL_ROW row;
	time_t now;
	unsigned int fields, rows;
	MYSQL_RES *result;
	struct nickcache *nc;

	assert(nick_id);

	nc = nickcache_find(nick_id);

	if (!nc) {
		log("nickcache: Tried to update nonexistent cache entry "
		    "for nick_id %u!", nick_id);
		snoop(s_OperServ, "[NICKCACHE] Tried to update nonexistent "
		    "cache entry for nick_id %u!", nick_id);
		nc = nickcache_insert(nick_id);
	}

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT status, language FROM nick WHERE nick_id=%u",
	    nick_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		now = time(NULL);
		
		nc->status = atoi(row[0]);
		nc->language = atoi(row[1]);
		nc->status_timestamp = nc->language_timestamp = now;
	} else {
		/*
		 * There's no longer any nick corresponding to this.
		 */
		log("nickcache: Tried to update nickcache for nonexistent "
		    "nick_id %u!  Deleting from cache.", nick_id);
		snoop(s_OperServ, "[NICKCACHE] Tried to update nickcache "
		    "for nonexistent nick_id %u!  Deleting from cache.",
		    nick_id);
		nickcache_del(nick_id);
		nc = NULL;
	}

	mysql_free_result(result);
	return(nc);
}

/* retrieve a nick's language from the cache, freshening or inserting if
 * necessary */
int nickcache_get_language(unsigned int nick_id)
{
	time_t now;
	struct nickcache *nc;

	assert(nick_id);

	now = time(NULL);
	
	if (!(nc = nickcache_find(nick_id))) {			/* not present */
		nc = nickcache_insert(nick_id);
	} else if ((now - nc->language_timestamp) >= 60 * 60) {	/* too old */
		nc = nickcache_update(nick_id);
	}

	return(nc ? nc->language : 0);
}

/* Set a nick's language in the cache, inserting if necessary */
void nickcache_set_language(unsigned int nick_id, unsigned int language)
{
	time_t now;
	struct nickcache *nc;

	assert(nick_id);

	now = time(NULL);

	if (!(nc = nickcache_find(nick_id))) {			/* not present */
		nc = nickcache_insert(nick_id);
	}

	if (nc) {
		nc->language = language;
		nc->language_timestamp = now;
	}
}

/* retrieve a nick's status from the cache, freshening or inserting if
 * necessary */
int nickcache_get_status(unsigned int nick_id)
{
	time_t now;
	struct nickcache *nc;

	assert(nick_id);

	now = time(NULL);

	if (!(nc = nickcache_find(nick_id))) {			/* not present */
		nc = nickcache_insert(nick_id);
        if(!nc)
        {
          snoop(s_OperServ, "[FUCKED] nickcache_insert failed.");
          return 0;
        }
	} else if ((now - nc->status_timestamp) >= 60 * 60) {	/* too old */
		nc = nickcache_update(nick_id);

        if(!nc)
        {
          snoop(s_OperServ, "[FUCKED] nickcache_update failed.");
          return 0;
        }
	}

	return(nc ? nc->status : 0);
}

/* set a nick's status in the cache, inserting if necessary */
void nickcache_set_status(unsigned int nick_id, int status)
{
	time_t now;
	struct nickcache *nc;

	assert(nick_id);
	
	now = time(NULL);

	if (!(nc = nickcache_find(nick_id))) {			/* not present */
		nc = nickcache_insert(nick_id);
	}

	if (nc) {
		nc->status = status;
		nc->status_timestamp = now;
	}
}

/* add a given status to the nick's current status */
void nickcache_add_status(unsigned int nick_id, int new_status)
{
	time_t now;
	struct nickcache *nc;

	assert(nick_id);

	now = time(NULL);

	if (!(nc = nickcache_find(nick_id))) {			/* not present */
		nc = nickcache_insert(nick_id);
	}

	if (nc) {
		nc->status |= new_status;
		nc->status_timestamp = now;
	}
}

/* remove a given status from the nick's current status */
void nickcache_remove_status(unsigned int nick_id, int remove_status)
{
	time_t now;
	struct nickcache *nc;

	assert(nick_id);

	now = time(NULL);

	if (!(nc = nickcache_find(nick_id))) {			/* not present */
		nc = nickcache_insert(nick_id);
	}

	if (nc) {
		nc->status &= ~remove_status;
		nc->status_timestamp = now;
	}
}
