
/*
 * Routines to maintain a list of online users.
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
#include "language.h"

RCSID("$Id$");

#define HASH(nick)	(((nick)[0]&31)<<5 | ((nick)[1]&31))
static User *userlist[1024];

int32 usercnt = 0, opcnt = 0, maxusercnt = 0;
time_t maxusertime;

int match_debug = 0;

static void mysql_quit_user(User *user, const time_t last_seen,
    const char *quit_msg);
static void display_entry_message(User *user, unsigned int channel_id);

/*************************************************************************/

/*************************************************************************/

/* Allocate a new User structure, fill in basic values, link it to the
 * overall list, and return it.  Always successful.
 */

static User *new_user(const char *nick)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	User *user, **list;

	user = scalloc(sizeof(User), 1);

	if (!nick)
		nick = "";
	
	strscpy(user->nick, nick, NICKMAX);
	list = &userlist[HASH(user->nick)];
	user->next = *list;
	
	if (*list)
		(*list)->prev = user;
	
	*list = user;
	user->real_id = mysql_findnick(nick);
	
	if (user->real_id)
		user->nick_id = mysql_getlink(user->real_id);
	else
		user->nick_id = 0;
	
	usercnt++;

	if (usercnt > maxusercnt) {
		maxusercnt = usercnt;
		maxusertime = time(NULL);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "REPLACE INTO stat (name, value) "
		    "VALUES ('maxusercnt', '%d')", maxusercnt);
		mysql_free_result(result);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "REPLACE INTO stat (name, value) "
		    "VALUES ('maxusertime', '%d')", maxusertime);
		mysql_free_result(result);
		
		if (LogMaxUsers) {
			log("user: New maximum user count: %d", maxusercnt);
			wallops(s_NickServ,
			    "New maximum user count: \2%d\2", maxusercnt);
		}
	}
	return user;
}

/*************************************************************************/

/* Change the nickname of a user, and move pointers as necessary. */

static void change_user_nick(User * user, const char *nick, int ni_changed)
{
	int status;
	User **list;

	if (user->prev)
		user->prev->next = user->next;
	else
		userlist[HASH(user->nick)] = user->next;
	
	if (user->next)
		user->next->prev = user->prev;

	/* Paranoia for zero-length nicks. */
	user->nick[1] = 0;
	strscpy(user->nick, nick, NICKMAX);
	list = &userlist[HASH(user->nick)];
	user->next = *list;
	user->prev = NULL;
	
	if (*list)
		(*list)->prev = user;
	
	*list = user;
	user->real_id = mysql_findnick(nick);
	
	if (user->real_id) {
		if (!ni_changed) {
			status = get_nick_status(user->nick_id);
			set_nick_status(user->real_id, status);
		}
		user->nick_id = mysql_getlink(user->real_id);
	} else {
		user->nick_id = 0;
	}
}

/*************************************************************************/

/* Remove and free a User structure. */

static void delete_user(User * user)
{
	struct u_chanlist *c, *c2;
	struct u_chanidlist *ci, *ci2;

	if (debug >= 2)
		log("debug: delete_user() called");

	usercnt--;
	if (user->mode & UMODE_o)
		opcnt--;

	cancel_user(user);
	
	if (debug >= 2)
		log("debug: delete_user(): free user data");
	
	free(user->username);
	free(user->host);
	free(user->realname);
	
	if (debug >= 2)
		log("debug: delete_user(): remove from channels");
	
	c = user->chans;
	
	while (c) {
		c2 = c->next;
		chan_deluser(user, c->chan);
		free(c);
		c = c2;
	}
	
	if (debug >= 2)
		log("debug: delete_user(): free founder data");
	
	ci = user->founder_chans;
	
	while (ci) {
		ci2 = ci->next;
		free(ci);
		ci = ci2;
	}
	
	if (debug >= 2)
		log("debug: delete_user(): delete from list");
	
	if (user->prev)
		user->prev->next = user->next;
	else
		userlist[HASH(user->nick)] = user->next;
	
	if (user->next)
		user->next->prev = user->prev;
	if (debug >= 2)
		log("debug: delete_user(): free user structure");
	
	free(user);
	
	if (debug >= 2)
		log("debug: delete_user() done");
}

/*************************************************************************/

/*************************************************************************/

/* Return statistics.  Pointers are assumed to be valid. */

void get_user_stats(long *nusers, long *memuse)
{
	long count, mem;
	int i;
	User *user;
	struct u_chanlist *uc;
	struct u_chanidlist *uci;

	count = 0;
	mem = 0;

	for (i = 0; i < 1024; i++) {
		for (user = userlist[i]; user; user = user->next) {
			count++;
			mem += sizeof(*user);

			if (user->username)
				mem += strlen(user->username) + 1;
			
			if (user->host)
				mem += strlen(user->host) + 1;
			
			if (user->realname)
				mem += strlen(user->realname) + 1;
			
			for (uc = user->chans; uc; uc = uc->next)
				mem += sizeof(*uc);
			
			for (uci = user->founder_chans; uci;
			    uci = uci->next)
				mem += sizeof(*uci);
		}
	}
	
	*nusers = count;
	*memuse = mem;
}

/*************************************************************************/

/* Find a user by nick.  Return NULL if user could not be found. */

User *finduser(const char *nick)
{
	User *user;

	if (debug >= 3)
		log("debug: finduser(%p)", nick);
	
	user = userlist[HASH(nick)];
	
	while (user && irc_stricmp(user->nick, nick) != 0)
		user = user->next;
	
	if (debug >= 3)
		log("debug: finduser(%s) -> %p", nick, user);
	
	return(user);
}

/*************************************************************************/

/* Iterate over all users in the user list.  Return NULL at end of list. */

static User *current;
static int next_index;

User *firstuser(void)
{
	next_index = 0;
	while (next_index < 1024 && current == NULL)
		current = userlist[next_index++];
	if (debug >= 3)
		log("debug: firstuser() returning %s",
		    current ? current->nick : "NULL (end of list)");
	return(current);
}

User *nextuser(void)
{
	if (current)
		current = current->next;
	if (!current && next_index < 1024) {
		while (next_index < 1024 && current == NULL)
			current = userlist[next_index++];
	}
	if (debug >= 3)
		log("debug: nextuser() returning %s",
		    current ? current->nick : "NULL (end of list)");

	return(current);
}

/*************************************************************************/

/*************************************************************************/

/* Handle a server NICK command.
 *
 * Received: NICK othy 2 1049625382 +i ~othy p362-tnt1.brs.ihug.com.au irc2.soe.uq.edu.au :othy
 *
 *	On CONNECT
 *		av[0] = nick
 *		av[1] = hop count
 *		av[2] = time of signon
 *		av[3] = usermodes
 *		av[4] = username
 *		av[5] = hostname
 *		av[6] = hosting server
 *		av[7] = realname
 *		
 *	Received: :dave NICK Dave :1080677604
 *	
 *	ELSE:
 *		av[0] = new nick
 *		av[1] = time of change
 */

int do_nick(const char *source, int ac, char **av)
{
#ifdef HAVE_GETTIMEOFDAY
	// struct timeval tv;
#endif
	/* New master nick. */
	unsigned int new_nick_id = 0;
	/* Did master nick change? */
	int ni_changed = 1;
	User *user = NULL;

	if (!*source) {
		/* This is a new user; create a User structure for it. */
		if (debug)
			log("debug: new user: %s", av[0]);

		/*
		 * We used to ignore the ~ which a lot of ircd's use to
		 * indicate no identd response.  That caused channel bans to
		 * break, so now we just take what the server gives us.  People
		 * are still encouraged to read the RFCs and stop doing
		 * anything to usernames depending on the result of an identd
		 * lookup.
		 */

		/* First check for AKILLs. */
		if (check_akill(av[0], av[4], av[5]))
			return(0);

		/* Now check for session limits */
		if (LimitSessions && !add_session(av[0], av[5]))
			return(0);

		/* Allocate User structure and fill it in. */
		user = new_user(av[0]);
		user->signon = atol(av[2]);
		user->username = sstrdup(av[4]);
		user->host = sstrdup(av[5]);
		user->server = findserver(av[6]);
		user->realname = sstrdup(av[7]);
		/* Not Used for Blitzed/OFTC 
		 * user->ip.s_addr = ntohl(strtoul(av[7], NULL, 0));
		 * user->ipstring = sstrdup(inet_ntoa(user->ip));
		 */
		user->my_signon = time(NULL);

		if (debug) {
			log("debug: got a new user, %s [%s]", user->nick,
		    	    user->ipstring);
			snoop(s_NickServ, "[IP] new user: %s [%s]",
			      user->nick, user->ipstring);
		}

		/*
		 * We use the current time to the tenth of a second as the
		 * Services stamp.  While not guaranteed to be unique, it's
		 * much simpler than trying to guarantee uniqueness between
		 * invocations of Services using an arbitrary value.  If the
		 * stamp we derive happens to be zero, we set it to one.
		 * NOTE that if we don't have gettimeofday(), we just take the
		 * time to the second, which is slightly more vulnerable to
		 * collision.
		 */
		/*i = atoi(av[8]);
		if (i) {
			user->services_stamp = i;
			if (debug) {
				log("debug: new user %s already has a "
				    "services_stamp of %d", user->nick,
				    (unsigned long)user->services_stamp);
			}
		} else {
#ifdef HAVE_GETTIMEOFDAY
			gettimeofday(&tv, NULL);
			user->services_stamp = (uint32)(tv.tv_sec*10 +
			    tv.tv_usec);
#else
			user->services_stamp = user->my_signon;
#endif
			if (!user->services_stamp)
				user->services_stamp = 1;

			if (debug) {
				log("debug: new user %s with no "
				    "services_stamp, sending %d",
				    user->nick,
				    (unsigned long)user->services_stamp);
			}
			send_cmd(ServerName, "SVSMODE %s +d %u",
				 user->nick, user->services_stamp);
		}
                */

		display_news(user, NEWS_LOGON);

	} else {
		/* An old user changing nicks. */

		user = finduser(source);
		
		if (!user) {
			log("user: NICK from nonexistent nick %s: %s", source,
			    merge_args(ac, av));
			return(0);
		}
		
		if (debug)
			log("debug: %s changes nick to %s", source, av[0]);

		/*
		 * Changing nickname case isn't a real change.  Only update
		 * my_signon if the nicks aren't the same, case-insensitively.
		 */
		if (irc_stricmp(av[0], source) == 0) 
			ni_changed = 0;
        else
			user->my_signon = time(NULL);

		new_nick_id = mysql_findnick(av[0]);
		
		if (new_nick_id)
			new_nick_id = mysql_getlink(new_nick_id);
		
		/*if (new_nick_id != user->nick_id) {*/
			if (debug) {
				log("user: cancelling %s because nick "
				    "ID's differ on nick change",
				    user->nick);
				snoop(s_NickServ, "[USER] Cancelling %s "
				      "because nick ID's differ on nick "
				      "change", user->nick);
			}
			cancel_user(user);
		/*} else
			ni_changed = 0;*/
		change_user_nick(user, av[0], ni_changed);
	}

	if (ni_changed) {
		if (validate_user(user))
			check_memos(user);
	} else {
		/* Make sure NS_ON_ACCESS flag gets set for current nick. */
		check_on_access(user);
	}

	if (nick_identified(user)) {
		if (!ni_changed && user->real_id &&
		    user->real_id != user->nick_id) {
			mysql_update_last_seen(user, 0);
		}

		send_cmd(ServerName, "SVSMODE %s :+Rr", av[0]);
		user->mode |= UMODE_r;
	}

	return(1);
}

/*************************************************************************/

/* Handle a JOIN command.
 *	av[0] = channels to join
 */

void do_join(const char *source, int ac, char **av)
{
	time_t now;
	unsigned int channel_id;
	User *user;
	char *s, *t;
	struct u_chanlist *c, *nextc;

	user = finduser(source);
	now = time(NULL);
	
	if (!user) {
		log("user: JOIN from nonexistent user %s: %s", source,
		    merge_args(ac, av));
		return;
	}
	
	t = av[0];
	
	while (*(s = t)) {
		t = s + strcspn(s, ",");
		
		if (*t)
			*t++ = 0;
		if (debug)
			log("debug: %s joins %s", source, s);

		if (*s == '0') {
			c = user->chans;
			while (c) {
				nextc = c->next;
				chan_deluser(user, c->chan);
				free(c);
				c = nextc;
			}
			user->chans = NULL;
			continue;
		}

		/*
		 * Make sure check_kick comes before chan_adduser, so banned users
		 * don't get to see things like channel keys.
		 */
		if (check_kick(user, s))
			continue;
		
		chan_adduser(user, s);
		
		if ((channel_id = mysql_findchan(s))) {
			if ((now - start_time) >= 10)
				display_entry_message(user, channel_id);
		
			check_chan_memos(user, channel_id);
		}

		c = smalloc(sizeof(*c));
		c->next = user->chans;
		c->prev = NULL;
		
		if (user->chans)
			user->chans->prev = c;
		
		user->chans = c;
		c->chan = findchan(s);
	}
}

/*************************************************************************/

/*
 * Handle an SJOIN command.
 * Basic format:
 *      av[0] = TS3 timestamp
 *      av[1] = TS3 timestamp - channel creation time
 *      av[2] = channel
 *      av[3] = channel modes
 *      av[4] = limit / key (depends on modes in av[3])
 *      av[5] = limit / key (depends on modes in av[3])
 *      av[4|5|6] = nickname(s), with modes, joining channel
 * SSJOIN format (server source):
 *      av[0] = TS3 timestamp - channel creation time
 *      av[1] = channel
 *      av[2] = modes (limit/key in av[3]/av[4] as needed)
 *      av[3|4|5] = users
 * SSJOIN format (client source):
 *      av[0] = TS3 timestamp - channel creation time
 *      av[1] = channel
 */

void do_sjoin(const char *source, int ac, char **av)
{
	time_t now;
	int modecnt;		/* Number of modes for a user (+o and/or +v) */
	int joins;		/* Number of users that actually joined (after akick)*/
	unsigned int channel_id;
	char modebuf[4];	/* Used to hold a nice mode string */
	User *user;
	char *av_cmode[4];
	char *t, *nick, *channel;
	struct u_chanlist *c;

	joins = 0;
	now = time(NULL);

	if (isdigit(av[1][0])) {
		/* Plain SJOIN format, zap join timestamp. */
		memmove(&av[0], &av[1], sizeof(char *) * (ac-1));
		ac--;
	}

	channel = av[1];
	
	if (ac >= 4) {
		/* SJOIN from server: nick list in last param. */
		t = av[ac-1];
	} else {
		/*
		 * SJOIN for a single client: source is nick. We assume the
		 * nick has no spaces, so we can discard const
		 */
		t = (char *)source;
	}

	while (*(nick = t)) {
		t = nick + strcspn(nick, " ");
		
		if (*t)
			*t++ = 0;

		modecnt = 0;
		modebuf[0] = '+';
		
		while (*nick == '@' || *nick == '+') {
			switch (*nick) {
			case '@':
				modecnt++;
				modebuf[modecnt] = 'o';
				break;
			case '+':
				modecnt++;
				modebuf[modecnt] = 'v';
				break;
			}
			nick++;
		}
		
		modebuf[modecnt + 1] = 0;
		user = finduser(nick);
		
		if (!user) {
			log("channel: SJOIN to channel %s for "
			    "non-existant nick %s (%s)", channel, nick,
			    merge_args(ac - 1, av));
			continue;
		}

		if (debug)
			log("debug: %s SJOINs %s", nick, channel);

		/*
		 * Make sure check_kick comes before chan_adduser, so
		 * banned users don't get to see things like channel
		 * keys.
		 */
		if (check_kick(user, channel))
			continue;

		chan_adduser(user, channel);
		joins++;

		if ((channel_id = mysql_findchan(channel))) {
			if ((now - start_time) >= 10)
				display_entry_message(user, channel_id);

			check_chan_memos(user, channel_id);
		}

		c = smalloc(sizeof(*c));
		c->next = user->chans;
		c->prev = NULL;
		
		if (user->chans)
			user->chans->prev = c;
		
		user->chans = c;
		c->chan = findchan(channel);
    c->chan->creation_time = av[0];

		if (modecnt > 0) {
			/* Set channel modes for user. */
			if (debug) {
				log("debug: channel modes for %s are %s",
				    nick, modebuf);
			}
			
			av_cmode[0] = channel;
			av_cmode[1] = modebuf;
			av_cmode[2] = nick;
			av_cmode[3] = nick;
			do_cmode(source, modecnt + 2, av_cmode);
		}
	}

	/*
	 * Did anyone actually join the channel and are there really any
	 * modes?
	 */
	if (joins && ac > 2 && av[2][1] != '\0') {
		/*
		 * Set channel modes.
		 * We need to watch out for the additional params for +k and
		 * +l.  But we don't have to try and figure out which is which,
		 * because the remote server will put them in the right order
		 * for us.
		 */
		av_cmode[0] = channel;
		av_cmode[1] = av[2];
		
		if (ac >= 5)
			av_cmode[2] = av[3];
		
		if (ac >= 6)
			av_cmode[3] = av[4];
		
		do_cmode(source, ac - 2, av_cmode);
	}
}

/*************************************************************************/

/* Handle a PART command.
 *	av[0] = channels to leave
 *	av[1] = reason (optional)
 */

void do_part(const char *source, int ac, char **av)
{
	User *user;
	char *s, *t;
	struct u_chanlist *c;

	user = finduser(source);
	
	if (!user) {
		log("user: PART from nonexistent user %s: %s", source,
		    merge_args(ac, av));
		return;
	}
	
	t = av[0];
	
	while (*(s = t)) {
		t = s + strcspn(s, ",");
		
		if (*t)
			*t++ = 0;
		
		if (debug)
			log("debug: %s leaves %s", source, s);
		
		for (c = user->chans; c && stricmp(s, c->chan->name) != 0;
		     c = c->next) ;	/* Empty. */
		
		if (c) {
			if (!c->chan) {
				log("user: BUG parting %s: channel entry "
				    "found but c->chan NULL", s);
				return;
			}
			
			chan_deluser(user, c->chan);
			
			if (c->next)
				c->next->prev = c->prev;
			
			if (c->prev)
				c->prev->next = c->next;
			else
				user->chans = c->next;
			
			free(c);
		}
	}
}

/*************************************************************************/

/* Handle a KICK command.
 *	av[0] = channel
 *	av[1] = nick(s) being kicked
 *	av[2] = reason
 */

void do_kick(const char *source, int ac, char **av)
{
	User *user;
	char *s, *t;
	struct u_chanlist *c;

	USE_VAR(source);

	t = av[1];
	
	while (*(s = t)) {
		t = s + strcspn(s, ",");
		
		if (*t)
			*t++ = 0;
		
		user = finduser(s);
		
		if (!user) {
			log("user: KICK for nonexistent user %s on %s: %s", s,
			    av[0], merge_args(ac - 2, av + 2));
			continue;
		}
		
		if (debug)
			log("debug: kicking %s from %s", s, av[0]);
		
		for (c = user->chans; c && stricmp(av[0], c->chan->name) != 0;
		     c = c->next) ;	/* Empty. */
		
		if (c) {
			chan_deluser(user, c->chan);
			
			if (c->next)
				c->next->prev = c->prev;
			
			if (c->prev)
				c->prev->next = c->next;
			else
				user->chans = c->next;
			
			free(c);
		}
	}
}

/*************************************************************************/

/* Handle a MODE command for a user.
 *	av[0] = nick to change mode for
 *	av[1] = modes
 */

void do_umode(const char *source, int ac, char **av)
{
	/* 1 if adding modes, 0 if deleting. */
	int add;
	User *user;
	char *s;

	add = 1;

	if (stricmp(source, av[0]) != 0) {
		log("user: MODE %s %s from different nick %s!", av[0], av[1],
		    source);
		wallops(NULL, "%s attempted to change mode %s for %s",
		    source, av[1], av[0]);
		return;
	}
	
	user = finduser(source);
	
	if (!user) {
		log("user: MODE %s for nonexistent nick %s: %s", av[1],
		    source, merge_args(ac, av));
		return;
	}
	
	if (debug)
		log("debug: Changing mode for %s to %s", source, av[1]);
	
	s = av[1];
	
	while (*s) {
		switch (*s++) {
		case '+':
			add = 1;
			break;
		case '-':
			add = 0;
			break;

                case 'R':
                        add ? (user->mode |= UMODE_r):
                          (user->mode &= ~UMODE_r);

                        if (add && user->real_id)
                        {
                           mysql_update_last_seen(user, NS_IDENTIFIED);
                           nickcache_update(user->real_id);
                           send_cmd(s_NickServ, "NOTICE %s :You have been automatically identified", user->nick);
                           send_cmd(ServerName, "SVSMODE %s :+R", user->nick);
                           if (strlen(get_email_from_id(user->real_id)) <= 1)
                               notice_lang(s_NickServ, user, NICK_BEG_SET_EMAIL,s_NickServ);
                           if (!(get_nick_status(user->real_id) & NS_RECOGNIZED))
                               check_memos(user);
                           if (!(get_nick_status(user->real_id) & NS_RECOGNIZED) && (get_nick_flags(user->real_id)) & NI_CLOAK) {
                               char *cloak = get_cloak_from_id(user->real_id);
                               if (cloak && *cloak != '\0') {
                                   send_cmd(ServerName, "SVSCLOAK %s :%s", user->nick, cloak);
                                   free(cloak);
                               }
                               check_autojoins(user);
                           }
                       }
                       break;
			
			
		case 'a':
			if (is_oper(user->nick) &&
			    is_services_admin(user)) {
				if (!add) {
					send_cmd(s_NickServ,
					    "SVSMODE %s :+a", av[0]);
				}
			} else {
				if (add) {
					send_cmd(s_NickServ,
					    "SVSMODE %s :-a", av[0]);
				}
			}
			break;

		case 'i':
			add ? (user->mode |= UMODE_i) :
			    (user->mode &= ~UMODE_i);
			break;
			
		case 'w':
			add ? (user->mode |= UMODE_w) :
			    (user->mode &= ~UMODE_w);
			break;
			
		case 'g':
			add ? (user->mode |= UMODE_g) :
			    (user->mode &= ~UMODE_g);
			break;
			
		case 's':
			add ? (user->mode |= UMODE_s) :
			    (user->mode &= ~UMODE_s);
			break;
			
		case 'o':
			if (add) {
				user->mode |= UMODE_o;
				
				if (WallOper) {
					wallops(s_OperServ,
					    "\2%s\2 is now an IRC operator.",
					    user->nick);
				}

				display_news(user, NEWS_OPER);
				opcnt++;
			} else {
				user->mode &= ~UMODE_o;
				opcnt--;
			}
			break;
		}
	}
}

/*************************************************************************/

/* Handle a QUIT command.
 *	av[0] = reason
 */

void do_quit(const char *source, int ac, char **av)
{
	User *user;

	user = finduser(source);

	if (!user) {
		log("user: QUIT from nonexistent user %s: %s", source,
		    merge_args(ac, av));
		return;
	}
	if (debug)
		log("debug: %s quits", source);
	if (user->nick_id) {
		/* update the quit times and messages etc */
		mysql_quit_user(user, time(NULL), av[0]);
	}

	if (LimitSessions)
		del_session(user->host);
	delete_user(user);
}

/*************************************************************************/

/* Handle a KILL command.
 *	av[0] = nick being killed
 *	av[1] = reason
 */

void do_kill(const char *source, int ac, char **av)
{
	User *user;

	USE_VAR(source);
	USE_VAR(ac);

	user = finduser(av[0]);
	
	if (!user)
		return;
	
	if (debug)
		log("debug: %s killed", av[0]);
	
	if (user->nick_id) {
		/* Update the quit times and messages etc. */
		mysql_quit_user(user, time(NULL), av[1]);
	}
	
	if (LimitSessions)
		del_session(user->host);
	
	delete_user(user);
}

/*************************************************************************/

/*************************************************************************/

/* Is the given nick an oper? */

int is_oper(const char *nick)
{
	User *user;
       
	user = finduser(nick);

	return(user && (user->mode & UMODE_o));
}

/*************************************************************************/

/* Is the given nick on the given channel? */

int is_on_chan(const char *nick, const char *chan)
{
	User *u;
	struct u_chanlist *c;

	u = finduser(nick);

	if (!u)
		return(0);
	
	for (c = u->chans; c; c = c->next) {
		if (stricmp(c->chan->name, chan) == 0)
			return(1);
	}
	
	return(0);
}

/*************************************************************************/

/* Is the given nick a channel operator on the given channel? */

int is_chanop(const char *nick, const char *chan)
{
	Channel *c;
	struct c_userlist *u;

	c = findchan(chan);

	if (!c)
		return(0);
	
	for (u = c->chanops; u; u = u->next) {
		if (irc_stricmp(u->user->nick, nick) == 0)
			return(1);
	}
	
	return(0);
}

/*************************************************************************/

/* Is the given nick voiced (channel mode +v) on the given channel? */

int is_voiced(const char *nick, const char *chan)
{
	Channel *c;
	struct c_userlist *u;

	c = findchan(chan);

	if (!c)
		return(0);
	
	for (u = c->voices; u; u = u->next) {
		if (irc_stricmp(u->user->nick, nick) == 0)
			return(1);
	}
	
	return(0);
}

/*************************************************************************/

/*************************************************************************/

/*
 * Does the user's usermask match the given mask (either nick!user@host or
 * just user@host)?
 */

int match_usermask(const char *mask, User *user)
{
	int result;
	char *mask2, *nick, *username, *host, *nick2, *host2;

	if (match_debug) {
		log("match_usermask('%s', '%s!%s@%s')", mask, user->nick,
		    user->username, user->host);
	}

	mask2 = sstrdup(mask);

	if (strchr(mask2, '!')) {
		nick = strlower(strtok(mask2, "!"));
		username = strtok(NULL, "@");
	} else {
		nick = NULL;
		username = strtok(mask2, "@");
	}
	
	host = strtok(NULL, "");
	
	if (!username || !host) {
		if (match_debug) {
			log("  mask contains missing username or missing "
			    "host.  No match");
		}
		free(mask2);
		return(0);
	}
	
	strlower(host);
	host2 = strlower(sstrdup(user->host));
	
	if (nick) {
		nick2 = strlower(sstrdup(user->nick));
		result = match_wild(nick, nick2) &&
		    match_wild(username, user->username) &&
		    (match_wild(host, host2) /*||
		    match_wild(host, user->ipstring)*/);
		free(nick2);
	} else {
		result = match_wild(username, user->username) &&
		    (match_wild(host, host2) /*||
		    match_wild(host, user->ipstring)*/);
	}
	
	free(mask2);
	free(host2);
	return(result);
}

/*************************************************************************/

/* Count the number of users who match a given mask */

int count_mask_matches(const char *mask)
{
	User *u;
	int i;

	i = 0;

	for (u = firstuser(); u; u = nextuser()) {
		if (match_usermask(mask, u))
			i++;
	}

	return(i);
}

/*************************************************************************/

/*
 * Split a usermask up into its constitutent parts.  Returned strings are
 * malloc()'d, and should be free()'d when done with.  Returns "*" for
 * missing parts.
 */

void split_usermask(const char *mask, char **nick, char **user, char **host)
{
	char *mask2;

	mask2 = sstrdup(mask);

	*nick = strtok(mask2, "!");
	*user = strtok(NULL, "@");
	*host = strtok(NULL, "");
	
	/* Handle special case: mask == user@host. */
	if (*nick && !*user && strchr(*nick, '@')) {
		*nick = NULL;
		*user = strtok(mask2, "@");
		*host = strtok(NULL, "");
	}
	
	if (!*nick)
		*nick = "*";
	
	if (!*user)
		*user = "*";
	
	if (!*host)
		*host = "*";
	
	*nick = sstrdup(*nick);
	*user = sstrdup(*user);
	*host = sstrdup(*host);
	free(mask2);
}

/*************************************************************************/

/*
 * Given a user, return a mask that will most likely match any address the user
 * will have from that location.  For IP addresses, wildcards the appropriate
 * subnet mask (e.g. 35.1.1.1 -> 35.*; 128.2.1.1 -> 128.2.*); for named
 * addresses, wildcards the leftmost part of the name unless the name only
 * contains two parts.  If the username begins with a ~, delete it.  The
 * returned character string is malloc'd and should be free'd when done with.
 */

char *create_mask(User * u)
{
	char *mask, *s, *end;

	/*
	 * Get us a buffer the size of the username plus hostname.  The result
	 * will never be longer than this (and will often be shorter), thus we
	 * can use strcpy() and sprintf() safely.
	 */
	end = mask = smalloc(strlen(u->username) + strlen(u->host) + 2);
	end += sprintf(end, "%s@", u->username);
	
	if (strspn(u->host, "0123456789.") == strlen(u->host) &&
	    (s = strchr(u->host, '.')) && (s = strchr(s + 1, '.')) &&
	    (s = strchr(s + 1, '.')) && (!strchr(s + 1, '.'))) {
		s = sstrdup(u->host);
		*strrchr(s, '.') = 0;
		
		if (atoi(u->host) < 192)
			*strrchr(s, '.') = 0;
		
		if (atoi(u->host) < 128)
			*strrchr(s, '.') = 0;
		
		sprintf(end, "%s.*", s);
		free(s);
	} else {
		if ((s = strchr(u->host, '.')) && strchr(s + 1, '.')) {
			s = sstrdup(strchr(u->host, '.') - 1);
			*s = '*';
		} else {
			s = sstrdup(u->host);
		}
		strcpy(end, s);
		free(s);
	}
	return(mask);
}

/*************************************************************************/

/*
 * Update the quit message and last seen time for a registered &
 * recognised user who is quitting.
 */
static void mysql_quit_user(User *user, const time_t last_seen,
    const char *quit_msg)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_quit_msg;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT 1 FROM nick WHERE nick_id=%u && !(status & %d) && "
	    "(status & %d)", user->real_id, NS_VERBOTEN,
	    (NS_IDENTIFIED | NS_RECOGNIZED));
	mysql_free_result(result);

	if (rows) {
		esc_quit_msg = smysql_escape_string(mysqlconn,
		    *quit_msg ? quit_msg : "");
	
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE nick SET last_seen=%d, last_quit='%s', "
		    "last_quit_time=%d WHERE nick_id=%u", last_seen,
		    esc_quit_msg, time(NULL), user->real_id);
		mysql_free_result(result);
		free(esc_quit_msg);
	}
}

/* send the channel's entry message (if any) to the user */
static void display_entry_message(User *user, unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	MYSQL_RES *result;

	if (!channel_id)
		return;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT entry_message, name FROM channel WHERE channel_id=%u "
	    "&& LENGTH(entry_message)", channel_id);
	
	if ((row = smysql_fetch_row(mysqlconn, result)))
		notice(s_ChanServ, user->nick, "[%s] %s", row[1], row[0]);

	mysql_free_result(result);
}

/*
 * Count the number of nicks currently in us that match a given POSIX
 * regular expression.
 */
unsigned int user_count_regex_match(regex_t *r)
{
	unsigned int cnt;
	User *u;

	cnt = 0;
	
	for (u = firstuser(); u; u = nextuser()) {
		if (regexec(r, u->nick, 0, NULL, 0) == 0)
			cnt++;
	}

	return(cnt);
}

/*
 * Count the number of users in a channel.
 */
unsigned int chan_count_users(Channel *c)
{
	unsigned int cnt;
	struct c_userlist *cu;

	if (!c)
		return(0);
	
	for (cu = c->users, cnt = 0; cu; cu = cu->next)
		cnt++;

	return(cnt);
}

/*
 * Return 1 if a given user is banned on the given channel, otherwise 0.
 */
int is_banned(User *u, Channel *c)
{
	unsigned int i;

	if (!c)
		return(0);

	for (i = 0; i < c->bancount; i++) {
		if (match_usermask(c->newbans[i]->mask, u))
			return(1);
	}

	return(0);
}
