
/*
 * Miscellaneous routines.
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

#include <stdlib.h>
#include <time.h>

#include "services.h"
#include "pseudo.h"

RCSID("$Id$");

extern unsigned int FascistCheck(const char *pw, char *dictpath);

/*************************************************************************/

/* toupper/tolower:  Like the ANSI functions, but make sure we return an
 *                   int instead of a (signed) char.
 */

int toupper(int c)
{
	if (islower(c))
		return (unsigned char)c - ('a' - 'A');
	else
		return (unsigned char)c;
}

int tolower(int c)
{
	if (isupper(c))
		return (unsigned char)c + ('a' - 'A');
	else
		return (unsigned char)c;
}

/*************************************************************************/

/* irc_toupper/tolower:  Like toupper/tolower, but for nicknames and
 *                       channel names; the RFC requires that '[' and '{',
 *                       '\' and '|', ']' and '}' be pairwise equivalent.
 */

int irc_toupper(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= '{' && c <= '}'))
		return c - ('a'-'A');
	else
		return (unsigned char)c;
}
    
int irc_tolower(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= '[' && c <= ']'))
		return c + ('a'-'A');
	else
		return (unsigned char)c;
}

/*************************************************************************/
        
/* irc_stricmp:  Like stricmp, but for nicknames and channel names. */
            
int irc_stricmp(const char *s1, const char *s2)
{           
	int c;

	while ((c = tolower(*s1)) == tolower(*s2)) {
		if (c == 0)
			return 0;
		s1++;
		s2++;
	}
	if (c < tolower(*s2))
		return -1;
	return 1;
}           

/*************************************************************************/

/* strscpy:  Copy at most len-1 characters from a string to a buffer, and
 *           add a null terminator after the last character copied.
 */

char *strscpy(char *d, const char *s, size_t len)
{
	char *d_orig = d;

	if (!len)
		return d;
	while (--len && (*d++ = *s++)) ;
	*d = 0;
	return d_orig;
}

/*************************************************************************/

/* stristr:  Search case-insensitively for string s2 within string s1,
 *           returning the first occurrence of s2 or NULL if s2 was not
 *           found.
 */

char *stristr(char *s1, char *s2)
{
	register char *s = s1, *d = s2;

	while (*s1) {
		if (tolower(*s1) == tolower(*d)) {
			s1++;
			d++;
			if (*d == 0)
				return s;
		} else {
			s = ++s1;
			d = s2;
		}
	}
	return NULL;
}

/*************************************************************************/

/* strupper, strlower:  Convert a string to upper or lower case.
 */

char *strupper(char *s)
{
	char *t;

	for (t = s; *t; t++)
		*t = toupper(*t);
	return s;
}

char *strlower(char *s)
{
	char *t;

	for (t = s; *t; t++)
		*t = tolower(*t);
	return s;
}

/*************************************************************************/

/* strnrepl:  Replace occurrences of `old' with `new' in string `s'.  Stop
 *            replacing if a replacement would cause the string to exceed
 *            `size' bytes (including the null terminator).  Return the
 *            string.
 */

char *strnrepl(char *s, size_t size, const char *old, const char *new)
{
	size_t left, avail, oldlen, newlen, diff;
	char *ptr;

	ptr = s;
	left = strlen(s);
	avail = size - (left + 1);
	oldlen = strlen(old);
	newlen = strlen(new);
	diff = newlen - oldlen;

	while (left >= oldlen) {
		if (strncmp(ptr, old, oldlen) != 0) {
			left--;
			ptr++;
			continue;
		}
		if (diff > avail)
			break;
		if (diff != 0)
			memmove(ptr + oldlen + diff, ptr + oldlen, left + 1);
		strncpy(ptr, new, newlen);
		ptr += newlen;
		left -= oldlen;
	}
	return s;
}

/*************************************************************************/

/*************************************************************************/

/* merge_args:  Take an argument count and argument vector and merge them
 *              into a single string in which each argument is separated by
 *              a space.
 */

char *merge_args(int argc, char **argv)
{
	int i;
	static char s[4096];
	char *t;

	t = s;
	for (i = 0; i < argc; i++)
		t +=
		    snprintf(t, sizeof(s) - (t - s), "%s%s", *argv++,
			     (i < argc - 1) ? " " : "");
	return s;
}

/*************************************************************************/

/*************************************************************************/

/* match_wild:  Attempt to match a string to a pattern which might contain
 *              '*' or '?' wildcards.  Return 1 if the string matches the
 *              pattern, 0 if not.
 */

static int do_match_wild(const char *pattern, const char *str, int docase)
{
	char c;
	const char *s;

	/*
	 * This WILL eventually terminate: either by *pattern == 0, or by a
	 * trailing '*'.
	 */

	for (;;) {
		switch (c = *pattern++) {
		case 0:
			if (!*str)
				return 1;
			return 0;
		case '?':
			if (!*str)
				return 0;
			str++;
			break;
		case '*':
			if (!*pattern)
				return 1;	/* trailing '*' matches everything else */
			s = str;
			while (*s) {
				if (
				    (docase ? (*s == *pattern)
				     : (tolower(*s) == tolower(*pattern)))
				    && do_match_wild(pattern, s, docase))
					return 1;
				s++;
			}
			break;
		default:
			if (docase ? (*str++ != c)
			    : (tolower(*str++) != tolower(c)))
				return 0;
			break;
		}		/* switch */
	}
}


int match_wild(const char *pattern, const char *str)
{
	return do_match_wild(pattern, str, 1);
}

int match_wild_nocase(const char *pattern, const char *str)
{
	return do_match_wild(pattern, str, 0);
}

/*************************************************************************/

/*************************************************************************/

/* Process a string containing a number/range list in the form
 * "n1[-n2][,n3[-n4]]...", calling a caller-specified routine for each
 * number in the list.  If the callback returns -1, stop immediately.
 * Returns the sum of all nonnegative return values from the callback.
 * If `count' is non-NULL, it will be set to the total number of times the
 * callback was called.
 *
 * The callback should be of type range_callback_t, which is defined as:
 *	int (*range_callback_t)(User *u, unsigned int num, va_list args)
 */

int process_numlist(const char *numstr, int *count_ret, unsigned int min,
    unsigned int max, range_callback_t callback, User * u, ...)
{
	static char numflag[65537];
	unsigned int i, imin, imax, n1, n2;
	int retval, numcount, res;
	va_list args;

	imin = 65536;
	imax = 0;
	retval = 0;
	numcount = 0;

	memset(numflag, 0, sizeof(numflag));

	va_start(args, u);

	/*
	 * This algorithm ignores invalid characters, ignores a dash
	 * when it precedes a comma, and ignores everything from the
	 * end of a valid number or range to the next comma or null.
	 */
	while (*numstr) {
		n1 = n2 = strtol(numstr, (char **)&numstr, 10);
		numstr += strcspn(numstr, "0123456789,-");
		if (*numstr == '-') {
			numstr++;
			numstr += strcspn(numstr, "0123456789,");
			if (isdigit(*numstr)) {
				n2 = strtol(numstr, (char **)&numstr, 10);
				numstr += strcspn(numstr, "0123456789,-");
			}
		}

		if (n1 < min)
			n1 = min;

		if (n2 > max)
			n2 = max;

		if (n2 > 65536)
			n2 = 65536;

		if (n1 < imin)
			imin = n1;
		if (n2 > imax)
			imax = n2;

		while (n1 <= n2) {
			numflag[n1] = 1;
			n1++;
		}
		
		numstr += strcspn(numstr, ",");
		
		if (*numstr)
			numstr++;
	}

	/* Now call the callback routine for each index. */
	numcount = 0;
	
	for (i = imin; i <= imax; i++) {
		if (!numflag[i])
			continue;

		numcount++;
		res = callback(u, i, args);

		if (debug) {
			log("debug: process_numlist: tried to do %d; "
			    "result = %d", i, res);
		}

		if (res < 0)
			break;

		retval += res;
	}

	va_end(args);

	if (count_ret)
		*count_ret = numcount;
	return retval;
}

/*************************************************************************/

/* dotime:  Return the number of seconds corresponding to the given time
 *          string.  If the given string does not represent a valid time,
 *          return -1.
 *
 *          A time string is either a plain integer (representing a number
 *          of seconds), or an integer followed by one of these characters:
 *          "s" (seconds), "m" (minutes), "h" (hours), or "d" (days), or a
 *          sequence of such integer-character pairs (without separators,
 *          e.g. "1h30m").
 */

int dotime(const char *s)
{
	int amount;

	amount = strtol(s, (char **)&s, 10);
	if (*s) {
		char c = *s++;
		int rest = dotime(s);

		if (rest < 0)
			return -1;
		
		switch (c) {
		case 's':
			return rest + amount;
		case 'm':
			return rest + (amount * 60);
		case 'h':
			return rest + (amount * 3600);
		case 'd':
			return rest + (amount * 86400);
		default:
			return -1;
		}
	} else {
		return amount;
	}
}

/*************************************************************************/

/* time_ago: return a string describing in plain english the difference
 *           between now and that tm. e.g. "3 weeks, 2 days ago"
 * borrowed mostly from magick/wrecked
 * -grifferz
 */

char *time_ago(time_t mytime)
{
	return (disect_time(time(NULL) - mytime));
}

/* disect_time: return a string which splits a time_t into its
 *              english language components.
 * borrowed mostly from magick/wrecked
 * -grifferz
 */

char *disect_time(time_t time)
{
	static char buf[64];
	int years, months, weeks, days, hours, minutes, seconds;

	years = months = weeks = days = hours = minutes = seconds = 0;

	while (time >= 60 * 60 * 24 * 365) {
		time -= 60 * 60 * 24 * 365;
		years++;
	}

	while (time >= 60 * 60 * 24 * 30) {
		time -= 60 * 60 * 24 * 30;
		months++;
	}
	
	while (time >= 60 * 60 * 24 * 7) {
		time -= 60 * 60 * 24 * 7;
		weeks++;
	}

	while (time >= 60 * 60 * 24) {
		time -= 60 * 60 * 24;
		days++;
	}

	while (time >= 60 * 60) {
		time -= 60 * 60;
		hours++;
	}

	while (time >= 60) {
		time -= 60;
		minutes++;
	}

	seconds = time;

	if (years) {
		snprintf(buf, sizeof(buf),
			 "%d year%s, %d month%s, %d week%s, %d day%s, "
			 "%02d:%02d:%02d", years, years == 1 ? "" : "s",
			 months, months == 1 ? "" : "s", weeks,
			 weeks == 1 ? "" : "s", days, days == 1 ? "" : "s",
			 hours, minutes, seconds);
	} else if (months) {
		snprintf(buf, sizeof(buf),
		    "%d month%s, %d week%s, %d day%s, %02d:%02d:%02d",
		    months, months == 1 ? "" : "s", weeks,
		    weeks == 1 ? "" : "s", days, days == 1 ? "" : "s",
		    hours, minutes, seconds);
	} else if (weeks) {
		snprintf(buf, sizeof(buf),
			 "%d week%s, %d day%s, %02d:%02d:%02d", weeks,
			 weeks == 1 ? "" : "s", days, days == 1 ? "" : "s",
			 hours, minutes, seconds);
	} else if (days) {
		snprintf(buf, sizeof(buf), "%d day%s, %02d:%02d:%02d", days,
			 days == 1 ? "" : "s", hours, minutes, seconds);
	} else if (hours) {
		if (minutes || seconds) {
			snprintf(buf, sizeof(buf),
				 "%d hour%s, %d minute%s, %d second%s",
				 hours, hours == 1 ? "" : "s", minutes,
				 minutes == 1 ? "" : "s", seconds,
				 seconds == 1 ? "" : "s");
		} else {
			snprintf(buf, sizeof(buf), "%d hour%s", hours,
					hours == 1 ? "" : "s");
		}
	} else if (minutes) {
		snprintf(buf, sizeof(buf), "%d minute%s, %d second%s",
			 minutes, minutes == 1 ? "" : "s", seconds,
			 seconds == 1 ? "" : "s");
	} else {
		snprintf(buf, sizeof(buf), "%d second%s", seconds,
			 seconds == 1 ? "" : "s");
	}

	return buf;
}


/* validate_email: check that a given email address is legal.  THIS DOES
 * NOT ACTUALLY SEND EMAIL TO THE ADDRESS, SO IT COULD STILL BE A BROKEN
 * ADDRESS.
 * returns 0 if the address is invalid.
 * -grifferz
 */
int validate_email(const char *email)
{
	if (strchr(email, '@'))
		return 1;
	else
		return 0;
}

/*
 * make_random_string: fill buffer with (length - 1) random characters
 * a-z A-Z and then add a terminating \0
 */

static char randchartab[] = {
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};

void make_random_string(char *buffer, size_t length)
{
	size_t i;
	int maxidx, j;

	maxidx = sizeof(randchartab) - 1;

	for (i = 0; i < (length - 1); i++) {
		j = rand() % (maxidx + 1);
		buffer[i] = randchartab[j];
	}
	buffer[length - 1] = 0;
}

/*
 * validate_password: use Alec Muffet's cracklib amongst other things to
 * check that a given password is strong enough.  channame should be NULL
 * if we are validating a nick's password, otherwise it will be the
 * channel's name including leading #.  Return 1 if it is OK, otherwise
 * tell the user why and return 0.
 */
unsigned int validate_password(User *u, const char *pass, char *channame)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	char *username, *esc_pass;

	esc_pass = smysql_escape_string(mysqlconn, pass);

	/*
	 * This is still needed because someone who is registering their
	 * first nick will not be caught by the check further down which
	 * only works on already registered nicks.	
	 */
	if (strcasecmp(pass, u->nick) == 0) {
		/* password == nickname */
		notice_lang(channame ? s_ChanServ : s_NickServ, u,
		    BAD_PASS_NICK_NAME);
		free(esc_pass);
		return(0);
	}

	username = u->username;

	/* skip past "no ident" marker if present */
	if (username[0] == '~')
		username++;

	if (strcasecmp(pass, username) == 0) {
		/* password == ident */
		notice_lang(channame ? s_ChanServ : s_NickServ, u,
		    BAD_PASS_USERNAME);
		free(esc_pass);
		return(0);
	}
	
	if (strcasecmp(pass, u->realname) == 0) {
		/* password == realname */
		notice_lang(channame ? s_ChanServ : s_NickServ, u,
		    BAD_PASS_REALNAME);
		free(esc_pass);
		return(0);
	}

	if (u->nick_id) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT 1 FROM nick "
		    "WHERE (nick_id=%u || link_id=%u) && "
		    "STRCMP(nick, '%s')=0", u->nick_id, u->nick_id,
		    esc_pass);

		mysql_free_result(result);
	
		if (rows) {
			/*
			 * Their password == their registered nick or one of
			 * its links
			 */
			notice_lang(channame ? s_ChanServ : s_NickServ, u,
			    BAD_PASS_NICK_OR_LINK);
			free(esc_pass);
			return(0);
		}
	}
	
	if (channame) {
		char *name;
	       
		name = channame;

		/* skip past leading # */
		if (name[0] == '#')
			name++;

		if (strcasecmp(pass, name) == 0) {
			/* password == chan name */
			notice_lang(s_ChanServ, u, BAD_PASS_CHANNEL_NAME);
			free(esc_pass);
			return(0);
		}
	}

	if (!(StrictPassWarn || StrictPassReject)) {
		/* No fascist password checking desired. */
		free(esc_pass);
		return(1);
	}
	return(1);
}

/*
 * fatal_error: for use when we need to leave a backtrace of what we've been
 * doing.  Does enough to alert the network of what is happening and then dumps
 * core.  We cannot use the standard signal handler because this loses
 * backtrace information.
 */
void fatal_error(const char *quitmsg)
{
	log("%s", quitmsg); 
	send_cmd(ServerName, "SQUIT %s :%s", ServerName, quitmsg);
	disconn(servsock); 
	smysql_cleanup(mysqlconn);
	abort();
}					

/*
 * rot13 in place.
 */
char *rot13(char *str)
{
	char base;
	char *p;

	for (p = str; *p; p++) {
		if (isalpha(*p)) {
			base = isupper(*p) ? 'A' : 'a';
			*p = (*p - base + 13) % 26 + base;
		}
	}

	return(str);
}
