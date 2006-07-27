
/* Autokill list functions.
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed IRC Network
 *      E-mail: services@lists.blitzed.org
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

extern int match_debug;

/*************************************************************************/

typedef struct akill Akill;
struct akill {
	unsigned int id;
	char *mask;
	char *reason;
	char who[NICKMAX];
	time_t time;
	time_t expires;		/* or 0 for no expiry */
};

static unsigned int mysql_insert_akill(MYSQL *mysql, Akill *new_akill);
static void mysql_delete_akill(MYSQL *mysql, unsigned int id);


/*************************************************************************/

/****************************** Statistics *******************************/

/*************************************************************************/

void get_akill_stats(long *nrec, long *memuse)
{
	get_table_stats(mysqlconn, "akill", (unsigned int *)nrec,
			(unsigned int *)memuse);
	if (*memuse)
		*memuse /= 1024;
}


unsigned int num_akills(void)
{
	return get_table_rows(mysqlconn, "akill");
}

/*************************************************************************/

/************************** Internal functions ***************************/

/*************************************************************************/

static void send_akill(const Akill * akill)
{
	time_t now;
	char *username, *host;

	now = time(NULL);

	username = sstrdup(akill->mask);
	host = strchr(username, '@');
	
	if (!host) {
		/*
		 * Glurp... this oughtn't happen, but if it does, let's not
		 * play with null pointers.  Yell and bail out.
		 */
		wallops(NULL, "Missing @ in AKILL: %s", akill->mask);
		log("Missing @ in AKILL: %s", akill->mask);
		return;
	}
	
	*host++ = 0;

    send_cmd(s_OperServ, "KLINE * 0 %s %s :autokilled: %s",
        username, host, akill->reason ? akill->reason : StaticAkillReason);
	free(username);
}



/*************************************************************************/

/************************** External functions ***************************/

/*************************************************************************/

/* Does the user match any AKILLs? */

int check_akill(const char *nick, const char *username, const char *host)
{
	char buf[512];
	Akill cakill;
	MYSQL_ROW row;
	size_t i;
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_buf;

	memset(buf, 0, sizeof(buf));

	strscpy(buf, username, sizeof(buf) - 2);
	i = strlen(buf);
	buf[i++] = '@';
	strlower(strscpy(buf + i, host, sizeof(buf) - i));
	esc_buf = smysql_escape_string(mysqlconn, buf);
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT akill_id, mask, reason, who, expires FROM akill "
	    "WHERE IRC_MATCH(mask, '%s') ORDER BY akill_id DESC", esc_buf);

	if (rows) {
		/* Only the first match matters. */
		row = smysql_fetch_row(mysqlconn, result);
		cakill.id = atoi(row[0]);
		cakill.mask = sstrdup(row[1]);
		cakill.reason = sstrdup(row[2]);
		strscpy(cakill.who, row[3], NICKMAX);
		cakill.expires = atoi(row[4]);

		/* We got a match so lets kill it. */

		/*
		 * Don't use kill_user(); that's for people who have already
		 * signed on.  This is called before the User structure is
		 * created.
		 */
		send_cmd(s_OperServ, "KILL %s :%s ([%u] %s)", nick,
		    s_OperServ, cakill.id,
		    StaticAkillReason ? StaticAkillReason : cakill.reason);
		
		send_akill(&cakill);
		
		free(cakill.mask);
		free(cakill.reason);
	}
	
	mysql_free_result(result);
	free(esc_buf);

	return(rows ? 1 : 0);
}

/*************************************************************************/

/* Delete any expired autokills. */

void expire_akills(void)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	time_t now;
	MYSQL_RES *result;
	char *s;

	now = time(NULL);

	if (opt_noexpire)
		return;

	if (WallAkillExpire) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT akill_id, mask, reason, who, time, expires "
		    "FROM akill WHERE expires != 0 && expires < %ld", now);

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			wallops(s_OperServ, "AKILL on %s has expired",
			    row[1]);
			log("OperServ: AKILL on %s has expired", row[1]);
			snoop(s_OperServ, "[AKILL] %s expired.",
			    row[1]);

			s = strchr(row[1], '@');
			
			if (s) {
				*s++ = 0;
				strlower(s);
				send_cmd(s_OperServ, "UNKLINE * %s %s",
				    row[1], s);
			}
		}
		mysql_free_result(result);
	}

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM akill WHERE expires != 0 && expires < %ld", now);
	mysql_free_result(result);
}

/*************************************************************************/

/************************** AKILL list editing ***************************/

/*************************************************************************/

/* Note that all parameters except expiry are assumed to be non-NULL.  A
 * value of NULL for expiry indicates that the AKILL should not expire.
 *
 * Not anymore. Now expiry represents the exact expiry time and may not be 
 * NULL. -TheShadow
 */

unsigned int add_akill(const char *mask, const char *reason,
		       const char *who, const time_t expiry)
{
	Akill new_akill;
	unsigned int err;

	if (num_akills() >= 32767) {
		log("%s: Attempt to add AKILL to full list!", s_OperServ);
		return(TOO_MANY_AKILLS);
	}

	new_akill.mask = strlower(sstrdup(mask));
	new_akill.reason = sstrdup(reason);
	new_akill.time = time(NULL);
	new_akill.expires = expiry;
	strscpy(new_akill.who, who, NICKMAX);

	err = mysql_insert_akill(mysqlconn, &new_akill);

	/* we will still send it even if there was an error */
	if (ImmediatelySendAkill)
			send_akill(&new_akill);

	free(new_akill.mask);
	free(new_akill.reason);

	switch (err) {
		case ER_DUP_ENTRY:
			/* The dupe wasn't added but it isn't a serious
			 * error */
			return(AKILL_PRESENT);
		default:
			return(0);
	}
}

/*************************************************************************/

/* Return whether the AKILL was deleted.  IRC Operators may delete their
 * own AKILLs, Services Operators and Admins may delete any AKILL.
 */

static int del_akill(User * u, const char *mask)
{
	Akill dakill;
	char expirebuf[256], buf[256];
	struct tm tm;
	MYSQL_ROW row;
	time_t t;
	unsigned int nick_id, fields, rows;
	MYSQL_RES *result;
	char *escaped_mask;

	t = time(NULL);
	escaped_mask = smysql_escape_string(mysqlconn, mask);

	dakill.id = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT akill_id, mask, reason, who, time, expires "
	    "FROM akill where mask='%s'", escaped_mask);

	free(escaped_mask);

	/*
	 * You can't have dupe masks in the table so we expect this to
	 * loop just once
	 */
	if ((row = smysql_fetch_row(mysqlconn, result))) {
		dakill.id = (unsigned int)atoi(row[0]);
		dakill.mask = sstrdup(row[1]);
		dakill.reason = sstrdup(row[2]);
		strscpy(dakill.who, row[3], NICKMAX);
		dakill.time = atoi(row[4]);
		dakill.expires = atoi(row[5]);
	}
	mysql_free_result(result);

	if (dakill.id) {
		if (!is_services_oper(u)) {
			nick_id = mysql_findnick(dakill.who);

			if (!nick_id ||
			    (mysql_getlink(nick_id) !=
			    mysql_getlink(u->nick_id))) {
				notice_lang(s_OperServ, u,
				    OPER_AKILL_ONLY_DELETE_OWN);
				free(dakill.mask);
				free(dakill.reason);
				return(0);
			}
		}

		if (dakill.expires == 0) {
			snprintf(expirebuf, sizeof(expirebuf),
			    "Would not expire");
		} else {
			snprintf(expirebuf, sizeof(expirebuf),
			    "Would have expired in %s",
			    disect_time(dakill.expires - t));
		}

		tm = *localtime(&dakill.time);
		strftime(buf, sizeof(buf) - 1, "%a %b %d %H:%M:%S %Y %Z",
		    &tm);

		wallops(s_OperServ,
		    "\2%s\2 removed an AKILL for \2%s\2 (%s) [%s]",
		    u->nick, dakill.mask, expirebuf, dakill.reason);
		wallops(s_OperServ, "Originally set by \2%s\2 on \2%s\2",
		    dakill.who, buf);

		mysql_delete_akill(mysqlconn, dakill.id);

		return(1);
	} else {
		notice_lang(s_OperServ, u, OPER_AKILL_NOT_FOUND, mask);
		return(0);
	}
}

/*************************************************************************/

/* Handle an OperServ AKILL command. */

/* Any oper may set an akill for up to 12 hours that does not affect more
 * than 20 people.
 * A services oper may set an akill for any amount of time that affects
 * any number of people (wild card threshold still applies, however).
 * A services admin may also use FORCE to bypass wildcard threshold.
 */ 

void do_akill(User * u)
{
	char expirebuf[256];
	char buf[128];
	char timebuf[32];
	MYSQL_ROW row;
	struct tm tm;
	time_t expires, t, akill_time, akill_expires;
	int i, svs_operadmin, force, matches, nonwild, m_expires, add;
	char *cmd, *mask, *reason, *expiry, *s;
	MYSQL_RES *result;

	if (is_services_admin(u))
		svs_operadmin = 2;
	else if (is_services_oper(u))
		svs_operadmin = 1;
	else
		svs_operadmin = 0;

	cmd = strtok(NULL, " ");
	
	if (!cmd)
		cmd = "";

	if (stricmp(cmd, "ADD") == 0) {
		force = 0;
		t = time(NULL);

		if (!nick_identified(u)) {
			notice_lang(s_OperServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);
			return;
		} else if (num_akills() >= 32767) {
			notice_lang(s_OperServ, u, OPER_TOO_MANY_AKILLS);
			return;
		}
		
		mask = strtok(NULL, " ");
		
		if (mask && *mask == '+') {
			expiry = mask;
			mask = strtok(NULL, " ");
		} else {
			expiry = NULL;
		}

		if (mask && (stricmp(mask, "FORCE") == 0)) {
			if (svs_operadmin > 1) {
				force = 1;
			} else {
				notice_lang(s_OperServ, u, ACCESS_DENIED);
				return;
			}
			mask = strtok(NULL, " ");
		}

		expires = expiry ? dotime(expiry) : AutokillExpiry;
		
		if (expires < 0) {
			notice_lang(s_OperServ, u, BAD_EXPIRY_TIME);
			return;
		} else if (expires > 0) {
			expires += t;	/* Set an absolute time */
		}

		if (mask && (reason = strtok(NULL, ""))) {
			nonwild = 0;

			if (strchr(mask, '!')) {
				notice_lang(s_OperServ, u,
				    OPER_AKILL_NO_NICK);
				notice_lang(s_OperServ, u, BAD_USERHOST_MASK);
				return;
			}
			
			s = strchr(mask, '@');
			
			if (!s) {
				notice_lang(s_OperServ, u, BAD_USERHOST_MASK);
				return;
			}

			if (!force) {
				for (i = strlen(mask) - 1;
				     i > 0 && mask[i] != '@'; i--) {
					if (! strchr("*?.", mask[i])) {
						nonwild++;
					}
				}

				if (AkillWildThresh &&
				    nonwild < AkillWildThresh) {
					if (svs_operadmin > 1) {
						notice_lang(s_OperServ, u,
						    OPER_AKILL_MASK_TOO_GENERAL_FORCE);
					} else {
						notice_lang(s_OperServ, u,
						    OPER_AKILL_MASK_TOO_GENERAL);
					}
					return;
				}
			}

			strlower(mask);

			matches = count_mask_matches(mask);

			if (matches == 0) {
				match_debug = 1;
				matches = count_mask_matches(mask);
				match_debug = 0;
			}

			if (matches > 20 && svs_operadmin < 1) {
				notice_lang(s_OperServ, u,
				    OPER_AKILL_TOO_MANY_MATCHES, matches);
				return;
			}

			if ((expires - t) > 43200	/* secs in 12 hrs */
			    && svs_operadmin < 1) {
				notice_lang(s_OperServ, u,
				    OPER_AKILL_TOO_LONG, disect_time(43200));
				return;
			}

			add = add_akill(mask, reason, u->nick, expires);
            switch (add) {
			case AKILL_PRESENT:
				notice_lang(s_OperServ, u,
				    OPER_AKILL_MASK_ALREADY_AKILLED, mask);
				log("OperServ: mask already akilled");
				snoop(s_OperServ,
				    "[AKILL] mask already akilled");
				return;

			default:
				notice_lang(s_OperServ, u, OPER_AKILL_ADDED,
				    mask, matches);
				break;
			}
			
			if (WallOSAkill) {
				if (expires == 0) {
					snprintf(buf, sizeof(buf),
					    getstring(u->nick_id,
					    OPER_AKILL_NO_EXPIRE));
				} else {
					expires_in_lang(buf, sizeof(buf), u,
					    expires - t + 59);
				}

				wallops(s_OperServ,
				    "\2%s\2 added an AKILL for \2%s\2 "
				    "(matched \2%d\2 user%s, %s) [%s]",
				    u->nick, mask, matches,
				    matches == 1 ? "" : "s", buf, reason);
			}

		} else {
			syntax_error(s_OperServ, u, "AKILL",
			    OPER_AKILL_ADD_SYNTAX);
		}

	} else if (stricmp(cmd, "DEL") == 0) {
		mask = strtok(NULL, " ");

		if (!nick_identified(u)) {
			notice_lang(s_OperServ, u, NICK_IDENTIFY_REQUIRED,
			    s_NickServ);
		} else if (mask) {
			s = strchr(mask, '@');
			
			if (s)
				strlower(s);
			
			if (del_akill(u, mask)) {
				notice_lang(s_OperServ, u, OPER_AKILL_REMOVED,
				    mask);
				
				if (s) {
					*s++ = 0;
					send_cmd(s_OperServ, "UNKLINE * %s %s",
					    mask, s);
				} else {
					/*
					 * We lose... can't figure out what's a
					 * username and what's a hostname.  Ah
					 * well.
					 */
				}
			}
		} else {
			syntax_error(s_OperServ, u, "AKILL",
			    OPER_AKILL_DEL_SYNTAX);
		}

	} else if (stricmp(cmd, "LIST") == 0) {
		m_expires = -1;	/* Do not match on expiry time */

		expire_akills();
		s = strtok(NULL, " ");
		
		if (!s) {
			s = "*";
		} else {
			expiry = strtok(NULL, " ");

			/*
			 * This is a little longwinded for what it acheives -
			 * but we can extend it later to allow for user defined
			 * expiry times.
			 */
			if (expiry && stricmp(expiry, "NOEXPIRE") == 0)
				m_expires = 0;	/* Akills that never expire */
		}

		if (strchr(s, '@'))
			strlower(strchr(s, '@'));

		notice_lang(s_OperServ, u, OPER_AKILL_LIST_HEADER);

		result = smysql_query(mysqlconn, "SELECT mask, reason, "
		    "expires FROM akill ORDER BY akill_id");

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			if (!s || (match_wild(s, row[0]) &&
			    (m_expires == -1 ||
			    atoi(row[2]) == m_expires))) {
				notice_lang(s_OperServ, u,
				    OPER_AKILL_LIST_FORMAT, row[0], row[1]);
			}
		}

		mysql_free_result(result);

	} else if (stricmp(cmd, "VIEW") == 0) {
		m_expires = -1;	/* Do not match on expiry time */

		expire_akills();
		s = strtok(NULL, " ");
		
		if (!s) {
			s = "*";
		} else {
			expiry = strtok(NULL, " ");

			/* This is a little longwinded for what it acheives - but we can
			 * extend it later to allow for user defined expiry times. */
			if (expiry && stricmp(expiry, "NOEXPIRE") == 0)
				m_expires = 0;	/* Akills that never expire */
		}

		if (strchr(s, '@'))
			strlower(strchr(s, '@'));
		
		notice_lang(s_OperServ, u, OPER_AKILL_LIST_HEADER);

		result = smysql_query(mysqlconn, "SELECT mask, reason, "
		    "who, time, expires FROM akill ORDER BY akill_id");

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			if (!s || (match_wild(s, row[0]) &&
			    (m_expires == -1 ||
			    atoi(row[4]) == m_expires))) {
				t = time(NULL);
				akill_time = atoi(row[3]);
				akill_expires = atoi(row[4]);
				tm = *localtime(akill_time ?
				    &akill_time : &t);

				strftime_lang(timebuf, sizeof(timebuf), u,
				    STRFTIME_DATE_TIME_FORMAT, &tm);

				if (akill_expires == 0) {
					snprintf(expirebuf, sizeof(expirebuf),
					    getstring(u->nick_id,
					    OPER_AKILL_NO_EXPIRE));
				} else if (akill_expires <= t) {
					snprintf(expirebuf, sizeof(expirebuf),
					    getstring(u->nick_id,
					    OPER_AKILL_EXPIRES_SOON));
				} else {
					expires_in_lang(expirebuf,
					    sizeof(expirebuf), u,
					    akill_expires - t + 59);
				}

				notice_lang(s_OperServ, u,
				    OPER_AKILL_VIEW_FORMAT, row[0],
				    *row[2] ? row[2] : "<unknown>", timebuf,
				    expirebuf, row[1]);
			}
		}

		mysql_free_result(result);

	} else if (stricmp(cmd, "COUNT") == 0) {
		notice_lang(s_OperServ, u, OPER_AKILL_COUNT, num_akills());
	} else {
		syntax_error(s_OperServ, u, "AKILL", OPER_AKILL_SYNTAX);
	}
}

/*************************************************************************/

/* MySQL functions */

/* insert a new akill */
static unsigned int mysql_insert_akill(MYSQL *mysql, Akill *new_akill)
{
	MYSQL_RES *result;
	unsigned int err;
	char *esc_mask, *esc_reason, *esc_who;
	
	esc_mask = smysql_escape_string(mysqlconn, new_akill->mask);
	esc_reason = smysql_escape_string(mysqlconn, new_akill->reason);
	esc_who = smysql_escape_string(mysqlconn, new_akill->who);

	result = mysql_simple_query(mysql, &err, "INSERT INTO akill "
				    "(akill_id, mask, reason, who, time, "
				    "expires) VALUES ('NULL', '%s', '%s', "
				    "'%s', %ld, %ld)", esc_mask,
				    esc_reason, esc_who, new_akill->time,
				    new_akill->expires);
	mysql_free_result(result);

	free(esc_who);
	free(esc_reason);
	free(esc_mask);

	new_akill->id = (unsigned int)mysql_insert_id(mysql);
	return(err);
}

/* delete an existing akill by id */
static void mysql_delete_akill(MYSQL *mysql, unsigned int id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysql, &fields, &rows, "DELETE FROM "
			           "akill WHERE akill_id=%u", id);
	mysql_free_result(result);
}
