
/* Channel-handling routines.
 *
 * Services is copyright (c) 1996-1999 Andrew Church.
 *     E-mail: <achurch@dragonfire.net>
 * Services is copyright (c) 1999-2000 Andrew Kempe.
 *     E-mail: <theshadow@shadowfire.org>
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include "services.h"

RCSID("$Id$");

#define HASH(chan)	((chan)[1] ? ((chan)[1]&31)<<5 | ((chan)[2]&31) : 0)

static Channel *chanlist[1024];

static void mysql_init_channel(Channel *c);
static void mysql_destroy_channel(Channel *c);

/*************************************************************************/

/* Return statistics.  Pointers are assumed to be valid. */

void get_channel_stats(long *nrec, long *memuse)
{
	long count, mem;
	size_t j;
	unsigned int i;
	Channel *chan;
	struct c_userlist *cu;

	count = 0;
	mem = 0;

	for (i = 0; i < 1024; i++) {
		for (chan = chanlist[i]; chan; chan = chan->next) {
			count++;
			mem += sizeof(*chan);
			if (chan->topic)
				mem += strlen(chan->topic) + 1;
			if (chan->key)
				mem += strlen(chan->key) + 1;
			mem += sizeof(char *) * chan->bansize;

			for (j = 0; j < chan->bancount; j++) {
				if (chan->newbans[j]) {
					if (chan->newbans[j]->mask)
						mem += strlen(chan->newbans[j]->mask) + 1;
					mem += sizeof(chan->newbans[j]);
				}
			}
			for (cu = chan->users; cu; cu = cu->next)
				mem += sizeof(*cu);
			for (cu = chan->chanops; cu; cu = cu->next)
				mem += sizeof(*cu);
			for (cu = chan->voices; cu; cu = cu->next)
				mem += sizeof(*cu);
		}
	}
	*nrec = count;
	*memuse = mem;
}

/*************************************************************************/

#ifdef DEBUG_COMMANDS

/* Send the current list of channels to the named user. */

void send_channel_list(User * user)
{
	Channel *c;
	char s[16], buf[512], *end;
	struct c_userlist *u, *u2;
	int isop, isvoice;
	const char *source = user->nick;

	for (c = firstchan(); c; c = nextchan()) {
		snprintf(s, sizeof(s), " %d", c->limit);
		notice(s_OperServ, source,
		       "%s %d +%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s %s", c->name,
		       c->creation_time, (c->mode & CMODE_i) ? "i" : "",
		       (c->mode & CMODE_m) ? "m" : "",
		       (c->mode & CMODE_M) ? "M" : "",
		       (c->mode & CMODE_n) ? "n" : "",
		       (c->mode & CMODE_p) ? "p" : "",
		       (c->mode & CMODE_s) ? "s" : "",
		       (c->mode & CMODE_t) ? "t" : "",
		       (c->mode & CMODE_c) ? "c" : "",
		       (c->mode & CMODE_O) ? "O" : "",
		       (c->mode & CMODE_R) ? "R" : "",
		       (c->limit) ? "l" : "", (c->key) ? "k" : "",
		       (c->limit) ? s : "", (c->key) ? " " : "",
		       (c->key) ? c->key : "", c->topic ? c->topic : "");
		end = buf;
		end +=
		    snprintf(end, sizeof(buf) - (end - buf), "%s", c->name);
		for (u = c->users; u; u = u->next) {
			isop = isvoice = 0;
			for (u2 = c->chanops; u2; u2 = u2->next) {
				if (u2->user == u->user) {
					isop = 1;
					break;
				}
			}
			for (u2 = c->voices; u2; u2 = u2->next) {
				if (u2->user == u->user) {
					isvoice = 1;
					break;
				}
			}
			end +=
			    snprintf(end, sizeof(buf) - (end - buf),
				     " %s%s%s", isvoice ? "+" : "",
				     isop ? "@" : "", u->user->nick);
		}
		notice(s_OperServ, source, buf);
	}
}


/* Send list of users on a single channel, taken from strtok(). */

void send_channel_users(User * user)
{
	char *chan = strtok(NULL, " ");
	Channel *c = chan ? findchan(chan) : NULL;
	struct c_userlist *u;
	const char *source = user->nick;

	if (!c) {
		notice(s_OperServ, source, "Channel %s not found!",
		       chan ? chan : "(null)");
		return;
	}
	notice(s_OperServ, source, "Channel %s users:", chan);
	for (u = c->users; u; u = u->next)
		notice(s_OperServ, source, "%s", u->user->nick);
	notice(s_OperServ, source, "Channel %s chanops:", chan);
	for (u = c->chanops; u; u = u->next)
		notice(s_OperServ, source, "%s", u->user->nick);
	notice(s_OperServ, source, "Channel %s voices:", chan);
	for (u = c->voices; u; u = u->next)
		notice(s_OperServ, source, "%s", u->user->nick);
}

#endif				/* DEBUG_COMMANDS */

/*************************************************************************/

/* Return the Channel structure corresponding to the named channel, or NULL
 * if the channel was not found.  chan is assumed to be non-NULL and valid
 * (i.e. pointing to a channel name of 2 or more characters). */

Channel *findchan(const char *chan)
{
	Channel *c;

	if (debug >= 3)
		log("debug: findchan(%p)", chan);
	c = chanlist[HASH(chan)];
	while (c) {
		if (irc_stricmp(c->name, chan) == 0)
			return c;
		c = c->next;
	}
	if (debug >= 3)
		log("debug: findchan(%s) -> %p", chan, c);
	return NULL;
}

/*
 * Return the Channel structure corresponding to the given channel_id, or NULL
 * if the channel was not found.  channel_id is assumed to be non-zero.
 */
Channel *findchan_by_id(unsigned int channel_id)
{
	Channel *c;

	if (debug >= 3)
		log("debug: findchan_by_ip(%u)", channel_id);

	for (c = firstchan(); c; c = nextchan()) {
		if (c->channel_id == channel_id)
			return(c);
	}

	return(NULL);
}
	

/*************************************************************************/

/* Iterate over all channels in the channel list.  Return NULL at end of
 * list.
 */

static Channel *current;
static int next_index;

Channel *firstchan(void)
{
	next_index = 0;
	while (next_index < 1024 && current == NULL)
		current = chanlist[next_index++];
	if (debug >= 3)
		log("debug: firstchan() returning %s",
		    current ? current->name : "NULL (end of list)");
	return current;
}

Channel *nextchan(void)
{
	if (current)
		current = current->next;
	if (!current && next_index < 1024) {
		while (next_index < 1024 && current == NULL)
			current = chanlist[next_index++];
	}
	if (debug >= 3)
		log("debug: nextchan() returning %s",
		    current ? current->name : "NULL (end of list)");
	return current;
}

/*************************************************************************/

/*************************************************************************/

/* Add/remove a user to/from a channel, creating or deleting the channel as
 * necessary.  If creating the channel, restore mode lock and topic as
 * necessary.  Also check for auto-opping and auto-voicing. */

void chan_adduser(User * user, const char *chan)
{
	Channel *c = findchan(chan);
	Channel **list;
	int newchan = !c;
	struct c_userlist *u;

	if (newchan) {
		if (debug)
			log("debug: Creating channel %s", chan);
		/* Allocate pre-cleared memory */
		c = scalloc(sizeof(Channel), 1);
		strscpy(c->name, chan, sizeof(c->name));
		list = &chanlist[HASH(c->name)];
		c->next = *list;
		if (*list)
			(*list)->prev = c;
		*list = c;
		c->creation_time = time(NULL);
		/* Store channel ID in channel record */
		c->channel_id = mysql_findchan(chan);

		mysql_init_channel(c);

		/* Restore locked modes and saved topic */
		check_modes(chan);
		restore_topic(chan);

        if(c->floodserv)
          send_cmd(ServerName, "SJOIN %d %s + :%s", time(NULL), c->name, s_FloodServ);
	}

	if (check_should_op(user, chan)) {
		u = smalloc(sizeof(struct c_userlist));

		u->next = c->chanops;
		u->prev = NULL;
		if (c->chanops)
			c->chanops->prev = u;
		c->chanops = u;
		u->user = user;
	} else if (check_should_voice(user, chan)) {
		u = smalloc(sizeof(struct c_userlist));

		u->next = c->voices;
		u->prev = NULL;
		if (c->voices)
			c->voices->prev = u;
		c->voices = u;
		u->user = user;
	}
	
	u = smalloc(sizeof(struct c_userlist));

	u->next = c->users;
	u->prev = NULL;
	if (c->users)
		c->users->prev = u;
	c->users = u;
	u->user = user;
}


/* FIXME: I think this is where all the problems relating to the AKICK ENFORCE
 * coredumps are occuring. Remove debug code when things are back to normal.
 *
 * Previously, we bailed out of this function if the user was not in the user
 * list. Now we go through the entire thing to make sure we remove them from
 * the op and voice lists. This raises issues about the code that handles
 * these lists in the first place - why is it possible for users to be in op
 * and voice lists when they're not in the user list?
 * -TheShadow
 */

void chan_deluser(User * user, Channel * c)
{
	struct c_userlist *u;
	size_t i;
	int in_userlist = 1;

	if (debug)
		log("channel: chan_deluser() called...");

	for (u = c->users; u && u->user != user; u = u->next) ;
	if (!u) {
		in_userlist = 0;
		log
		    ("channel: BUG(?) chan_deluser() called for %s in %s but they "
		     "were not found on the channel's userlist.", user->nick,
		     c->name);
		/* This is where we bailed out in the past! */
		//return;
	} else {
		if (u->next)
			u->next->prev = u->prev;
		if (u->prev)
			u->prev->next = u->next;
		else
			c->users = u->next;
		free(u);
	}

	for (u = c->chanops; u && u->user != user; u = u->next) ;
	if (u) {
		if (!in_userlist)
			log
			    ("channel: BUG(?) found user in ops list but not in userlist.");
		if (u->next)
			u->next->prev = u->prev;
		if (u->prev)
			u->prev->next = u->next;
		else
			c->chanops = u->next;
		free(u);
	}

	for (u = c->voices; u && u->user != user; u = u->next) ;
	if (u) {
		if (!in_userlist)
			log
			    ("channel: BUG(?) found user in voices list but not in userlist.");
		if (u->next)
			u->next->prev = u->prev;
		if (u->prev)
			u->prev->next = u->next;
		else
			c->voices = u->next;
		free(u);
	}

	if (!c->users) {
		if (debug)
			log("debug: Deleting channel %s", c->name);

		mysql_destroy_channel(c);

		if (c->topic)
			free(c->topic);
		if (c->key)
			free(c->key);
		for (i = 0; i < c->bancount; ++i) {
			if (c->newbans[i]) {
				if (c->newbans[i]->mask)
					free(c->newbans[i]->mask);
				free(c->newbans[i]);
			} else
				log
				    ("channel: BUG freeing %s: bans[%d] is NULL!",
				     c->name, i);
		}
		if (c->bansize)
			free(c->newbans);
		if (c->chanops || c->voices)
			log
			    ("channel: Memory leak freeing %s: %s%s%s %s non-NULL!",
			     c->name, c->chanops ? "c->chanops" : "",
			     c->chanops
			     && c->voices ? " and " : "",
			     c->voices ? "c->voices" : "", c->chanops
			     && c->voices ? "are" : "is");
		if (c->next)
			c->next->prev = c->prev;
		if (c->prev)
			c->prev->next = c->next;
		else
			chanlist[HASH(c->name)] = c->next;
		free(c);
	} else {
		check_autolimit(c, time(NULL), 0);
	}

	if (debug)
		log("channel: chan_deluser() complete.");
}

/*************************************************************************/

/* Handle a channel MODE command. */

void do_cmode(const char *source, int ac, char **av)
{
	/* 31 modes + leeway. */
	static char lastmodes[40];
	size_t i;
	/* 1 if adding modes, 0 if deleting. */
	int add;
	Channel *chan;
	struct c_userlist *u;
	User *user;
	char *s, *nick;
	char *modestr;
	Ban **b;
	char modechar;

	add = 1;
	modestr = av[1];
	chan = findchan(av[0]);

	if (!chan) {
		log("channel: MODE %s for nonexistent channel %s",
		    merge_args(ac - 1, av + 1), av[0]);
		return;
	}

	/* This shouldn't trigger on +o, etc. */
	if (strchr(source, '.') && strcmp(source, ServerName) != 0 &&
	    !modestr[strcspn(modestr, "bov")]) {

		if (time(NULL) != chan->server_modetime ||
		    strcmp(modestr, lastmodes) != 0) {
			chan->server_modecount = 0;
			chan->server_modetime = time(NULL);
			strscpy(lastmodes, modestr, sizeof(lastmodes));
		}
		chan->server_modecount++;
	}

	s = modestr;
	ac -= 2;
	av += 2;

	while (*s) {
		modechar = *s++;

		switch (modechar) {

		case '+':
			add = 1;
			break;

		case '-':
			add = 0;
			break;

		case 'i':
			if (add)
				chan->mode |= CMODE_i;
			else
				chan->mode &= ~CMODE_i;
			break;

		case 'm':
			if (add)
				chan->mode |= CMODE_m;
			else
				chan->mode &= ~CMODE_m;
			break;

		case 'M':
			if (add)
				chan->mode |= CMODE_M;
			else
				chan->mode &= ~CMODE_M;
			break;

		case 'n':
			if (add)
				chan->mode |= CMODE_n;
			else
				chan->mode &= ~CMODE_n;
			break;

		case 'p':
			if (add)
				chan->mode |= CMODE_p;
			else
				chan->mode &= ~CMODE_p;
			break;

		case 's':
			if (add)
				chan->mode |= CMODE_s;
			else
				chan->mode &= ~CMODE_s;
			break;

		case 't':
			if (add)
				chan->mode |= CMODE_t;
			else
				chan->mode &= ~CMODE_t;
			break;

		case 'c':
			if (add)
				chan->mode |= CMODE_c;
			else
				chan->mode &= ~CMODE_c;
			break;

		case 'O':
			if (add)
				chan->mode |= CMODE_O;
			else
				chan->mode &= ~CMODE_O;
			break;

		case 'R':
			if (add)
				chan->mode |= CMODE_R;
			else
				chan->mode &= ~CMODE_R;
			break;

#if 0
		case 'r':
			if (add)
				chan->mode |= CMODE_r;
			else
				chan->mode &= ~CMODE_r;
			break;
#endif

		case 'k':
			if (--ac < 0) {
				log("channel: MODE %s %s: missing "
				    "parameter for %ck", chan->name,
				    modestr, add ? '+' : '-');
				break;
			}
			if (chan->key) {
				free(chan->key);
				chan->key = NULL;
			}
			if (add) {
				chan->key = sstrdup(*av);
				av++;
			}
			break;

		case 'l':
			if (add) {
				if (--ac < 0) {
					log("channel: MODE %s %s: missing "
					    "parameter for +l", chan->name,
					    modestr);
					break;
				}
				chan->limit = strtoul(*av, NULL, 10);
				av++;
			} else {
				chan->limit = 0;
			}
			break;

		case 'b':
			if (--ac < 0) {
				log("channel: MODE %s %s: missing "
				    "parameter for %cb", chan->name,
				    modestr, add ? '+' : '-');
				break;
			}
			if (add) {
				if (chan->bancount >= chan->bansize) {
					chan->bansize += 8;
					chan->newbans =
					    srealloc(chan->newbans,
					    sizeof(Ban *) * chan->bansize);
				}
				chan->newbans[chan->bancount] =
				    smalloc(sizeof(Ban));
				chan->newbans[chan->bancount]->mask =
				    sstrdup(*av++);
				chan->newbans[chan->bancount]->time =
				    time(NULL);
				chan->bancount++;
			} else {
				b = chan->newbans;
				i = 0;

				while (i < chan->bancount &&
				    strcmp((*b)->mask, *av) != 0) {
					i++;
					b++;
				}
				if (i < chan->bancount) {
					chan->bancount--;
					if (i < chan->bancount) {
						free((*b)->mask);
						memmove(b, b + 1,
						    sizeof(Ban *) *
						    (chan->bancount - i));
					}
				}
				av++;
			}
			break;

		case 'o':
			if (--ac < 0) {
				log("channel: MODE %s %s: missing "
				    "parameter for %co", chan->name,
				    modestr, add ? '+' : '-');
				break;
			}
			nick = *av++;
			if (add) {
				for (u = chan->chanops;
				    u && stricmp(u->user->nick, nick) != 0;
				    u = u->next) ;
				if (u)
					break;
				user = finduser(nick);
				if (!user) {
					log("channel: MODE %s +o for "
					    "nonexistent user %s",
					    chan->name, nick);
					break;
				}
				if (debug)
					log("debug: Setting +o on %s for %s",
					    chan->name, nick);
				if (!check_valid_op(user, chan->name,
				    !!strchr(source, '.')))
					break;
				u = smalloc(sizeof(*u));
				u->next = chan->chanops;
				u->prev = NULL;
				if (chan->chanops)
					chan->chanops->prev = u;
				chan->chanops = u;
				u->user = user;
			} else {
				for (u = chan->chanops;
				    u && stricmp(u->user->nick, nick);
				    u = u->next) ;
				if (!u)
					break;
				if (u->next)
					u->next->prev = u->prev;
				if (u->prev)
					u->prev->next = u->next;
				else
					chan->chanops = u->next;
				free(u);
			}
			break;

		case 'v':
			if (--ac < 0) {
				log("channel: MODE %s %s: missing "
				    "parameter for %cv", chan->name,
				    modestr, add ? '+' : '-');
				break;
			}
			nick = *av++;
			if (add) {
				for (u = chan->voices;
				    u && stricmp(u->user->nick, nick);
				    u = u->next) ;
				if (u)
					break;
				user = finduser(nick);
				if (!user) {
					log("channe: MODE %s +v for "
					    "nonexistent user %s",
					    chan->name, nick);
					break;
				}
				if (debug)
					log("debug: Setting +v on %s for %s",
					    chan->name, nick);
				u = smalloc(sizeof(*u));
				u->next = chan->voices;
				u->prev = NULL;
				if (chan->voices)
					chan->voices->prev = u;
				chan->voices = u;
				u->user = user;
			} else {
				for (u = chan->voices;
				    u && stricmp(u->user->nick, nick);
				    u = u->next) ;
				if (!u)
					break;
				if (u->next)
					u->next->prev = u->prev;
				if (u->prev)
					u->prev->next = u->next;
				else
					chan->voices = u->next;
				free(u);
			}
			break;

		}		/* switch */

	}			/* while (*s) */

	/* Check modes against ChanServ mode lock */
	check_modes(chan->name);
}

/*************************************************************************/

/* Handle a TOPIC command. */

void do_topic(const char *nick, int ac, char **av)
{
	Channel *c = findchan(av[0]);

	if (!c) {
		log("channel: TOPIC %s for nonexistent channel %s",
		    merge_args(ac - 1, av + 1), av[0]);
		return;
	}
	if (check_topiclock(av[0], nick, c))
		return;
	strscpy(c->topic_setter, nick, sizeof(c->topic_setter));
	c->topic_time = time(NULL);
	if (c->topic) {
		free(c->topic);
		c->topic = NULL;
	}
	if (ac > 1 && *av[1])
		c->topic = sstrdup(av[1]);
	record_topic(av[0]);
}

/*************************************************************************/

/* Does the given channel have only one user? */

/* Note:  This routine is not currently used, but is kept around in case it
 * might be handy someday. */

#if 0
int only_one_user(const char *chan)
{
	Channel *c = findchan(chan);

	return (c && c->users && !c->users->next) ? 1 : 0;
}

#endif

/*************************************************************************/

/*
 * Go through every channel that is currently active and has a limit
 * offset or ban clearance time.  If it's more than ci->limit_tolerance
 * off from the current user count and it's beyond the time period for
 * updates, then set the limit.  Then check the ban list for any that need
 * expiring.
 *  -grifferz
 */

void chan_update_events(time_t now)
{
	time_t threshold;
	size_t i;
	unsigned int count;
	char *av[3];
	Channel *chan;
	Ban **to_remove;

	for (chan = firstchan(); chan; chan = nextchan()) {
		if (!chan->channel_id) {
			/* Channel isn't registered. */
			continue;
		}
		
		/* Check for channels that need their limit changed. */
		if (chan->limit_offset > 0 &&
		    (unsigned)(now - chan->last_limit_time) >=
		    (chan->limit_period * 60)) {
			check_autolimit(chan, now, 1);
		}

		/* Check for bans that need to be removed. */
		if (chan->bantime) {
			to_remove = NULL;

			/*
			 * Any ban set before this time needs to be
			 * removed.
			 */
			threshold = now - (chan->bantime * 60);

			/*
			 * First find the ones to remove - we won't remove
			 * them till later because doing so would require a
			 * separate copy of the banlist and it's wasteful
			 * to take a copy every time unless we know we're
			 * going to remove something.
			 */
			for (i = 0, count = 0; i < chan->bancount; i++) {
				if (chan->newbans[i]->time < threshold) {
					/* Time to remove this one. */
					to_remove = srealloc(to_remove,
					    sizeof(*to_remove) *
					    (count + 1));
					to_remove[count] =
					    smalloc(sizeof(Ban));
					to_remove[count]->mask =
					    sstrdup(chan->newbans[i]->mask);
					count++;
				}
			}

			if (count) {
				av[0] = chan->name;
				av[1] = sstrdup("-b");

				for (i = 0; i < count; i++) {
					av[2] = sstrdup(to_remove[i]->mask);
					send_cmd(MODE_SENDER(s_ChanServ),
					    "MODE %s -b %s", av[0], av[2]);
					do_cmode(s_ChanServ, 3, av);
					free(av[2]);
					free(to_remove[i]->mask);
					free(to_remove[i]);
				}

				free(av[1]);
				free(to_remove);
			}
		}
	}
}

/*
 * Count the number of channels whose name matches a POSIX regular
 * expression.
 */
unsigned int chan_count_regex_match(regex_t *r)
{
	unsigned int cnt;
	Channel *c;

	cnt = 0;

	for (c = firstchan(); c; c = nextchan()) {
		if (regexec(r, c->name, 0, NULL, 0) == 0)
			cnt++;
	}

	return(cnt);
}

/*
 * Initialise various parts of Channel structure from SQL data.
 */
static void mysql_init_channel(Channel *c)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	MYSQL_RES *result;
	
	if (!c)
		return;

	c->last_limit_time = 0;
	c->limit_offset = 0;
	c->limit_tolerance = 0;
	c->limit_period = 0;
	c->bantime = 0;

	if (!c->channel_id)
		return;
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT last_limit_time, limit_offset, limit_tolerance, "
	    "limit_period, bantime, floodserv_protected FROM channel WHERE channel_id=%u",
	    c->channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		c->last_limit_time = atoi(row[0]);
		c->limit_offset = atoi(row[1]);
		c->limit_tolerance = atoi(row[2]);
		c->limit_period = atoi(row[3]);
		c->bantime = atoi(row[4]);
        c->floodserv = atoi(row[5]);
	}

	mysql_free_result(result);
}

/*
 * Store channel data as last user leaves the channel.
 */
static void mysql_destroy_channel(Channel *c)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	if (!c || !c->channel_id)
		return;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET last_limit_time=%d, limit_offset=%u, "
	    "limit_tolerance=%u, limit_period=%u, bantime=%u "
	    "WHERE channel_id=%u", c->last_limit_time, c->limit_offset,
	    c->limit_tolerance, c->limit_period, c->bantime,
	    c->channel_id);
	mysql_free_result(result);
}

/*
 * Check if a specific channel's autolimit must be updated.  If allow_raise is
 * 0 then the limit can never go up.  This is used when calling this function
 * after a user leaves a channel (PART, QUIT, KICK etc).
 */
void check_autolimit(Channel *chan, time_t now, int allow_raise)
{
	unsigned int fields, rows, usercnt;
	int delta;
	MYSQL_RES *result;
	struct c_userlist *cu;

	if (!(chan && chan->channel_id && chan->limit_offset))
		return;

	/*
	 * Never allow channel limit changes more often than every 10 secs,
	 * otherwise you can end up with a flood of them in situations like
	 * netsplit.
	 */
	if (now - chan->last_limit_time <= 10)
		return;

	for (cu = chan->users, usercnt = 0; cu; cu = cu->next)
		usercnt++;

	delta = usercnt + chan->limit_offset - chan->limit;

	/*
	 * We want to change the limit if it needs to go lower, or
	 * if it is high enough to go past the tolerance.
	 */
	if (delta < 0 ||
	    (((unsigned)delta > chan->limit_tolerance) && allow_raise)) {
		chan->limit = usercnt + chan->limit_offset;
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +l %d",
		    chan->name, chan->limit);
	}

	chan->last_limit_time = now;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET last_limit_time=%d WHERE channel_id=%u",
	    chan->last_limit_time, chan->channel_id);
	mysql_free_result(result);
}

