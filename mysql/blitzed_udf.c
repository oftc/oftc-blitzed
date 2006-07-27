/*
 * MySQL User-Defined Functions
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 *     E-mail: services@lists.blitzed.org
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * To compile this function, you need to know the compiler flags for making a
 * shared library.  On Linux with gcc, this is:
 *
 * -shared -o filename.so filename.c
 *
 * as the "make" example script shows.
 *
 * After the .so file is created, it must be copied to a directory that is
 * searched by the dlopen() function.  "man dlopen" will tell you this, but
 * /usr/lib is a safe bet.  Adjusting LD_LIBRARY_PATH in the safe_mysqld script
 * may also help.
 *
 * Now you must tell MySQL that the function is available.  To do this you must
 * have write access to the mysql.func table.  Then:
 *
 * CREATE FUNCTION irc_match RETURNS INTEGER SONAME "blitzed_udf.so";
 *
 * From now on, IRC_MATCH should always be available even between restarts of
 * mysqld.  If you ever want to remove it:
 *
 * DROP FUNCTION irc_match;
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <mysql.h>
#include "../src/defs.h"

RCSID("$Id$");

my_bool irc_match_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void irc_match_deinit(UDF_INIT *initid);
static int do_match_wild(const char *pattern, const char *str, int docase);
long long irc_match(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
    char *error);

my_bool irc_match_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
	if (args->arg_count < 2 || args->arg_count > 3) {
		strncpy(message, "IRC_MATCH requires 2 or 3 arguments",
		    MYSQL_ERRMSG_SIZE);
		return(1);
	}

	/*
	 * this function wants the first two arguments as strings, so
	 * let's coerce them.
	 */
	args->arg_type[0] = STRING_RESULT;
	args->arg_type[1] = STRING_RESULT;

	/* If there is a third, it must be an integer */
	if (args->arg_count == 3)
		args->arg_type[2] = INT_RESULT;
	return(0);
}

void irc_match_deinit(UDF_INIT *initid)
{
}

/*
 * Attempt to match a string to a pattern which might contain '*' or '?'
 * wildcards.  Return 1 if the string matches, 0 if not.  Based on code
 * from ircservices (c) Andrew Church.
 */

static int do_match_wild(const char *pattern, const char *str, int docase)
{
	const char *s;
	char c;
#ifdef DEBUG
	FILE *fp;

	fp = fopen("/tmp/match.log", "a");
	fprintf(fp, "do_match_wild: '%s' vs '%s', docase=%d\n", pattern, str, docase);
#endif
	
	for (;;) {
		switch (c = *pattern++) {
		case 0:
			if (!*str) {
#ifdef DEBUG
				fprintf(fp, "do_match_wild: match!\n\n");
				fclose(fp);
#endif
				return(1);
			}
#ifdef DEBUG
			fprintf(fp, "do_match_wild: no match!\n\n");
			fclose(fp);
#endif
			return(0);
			
		case '?':
			if (!*str) {
#ifdef DEBUG
				fprintf(fp, "do_match_wild: no match!\n\n");
				fclose(fp);
#endif
				return(0);
			}
			str++;
			break;
			
		case '*':
			if (!*pattern) {
				/* trailing '*' matches everything else */
#ifdef DEBUG
				fprintf(fp, "do_match_wild: match!\n\n");
				fclose(fp);
#endif
				return(1);
			}

			s = str;
			while (*s) {
				if ((docase ? (*s == *pattern) :
				    (tolower(*s) == tolower(*pattern))) &&
				    do_match_wild(pattern, s, docase)) {
#ifdef DEBUG
					fprintf(fp, "do_match_wild: match!\n\n");
					fclose(fp);
#endif
					return(1);
				}
				s++;
			}
			break;

		default:
			if (docase ? (*str++ != c) :
			    (tolower(*str++) != tolower(c))) {
#ifdef DEBUG
				fprintf(fp, "do_match_wild: no match!\n\n");
				fclose(fp);
#endif
				return(0);
			}
			break;
		}
	}
}

long long irc_match(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
    char *error)
{
	long long res;
	char *p, *s;

	p = malloc((args->lengths[0] + 1) * sizeof(*p));
	s = malloc((args->lengths[1] + 1) * sizeof(*s));

	strncpy(p, args->args[0], args->lengths[0]);
	strncpy(s, args->args[1], args->lengths[1]);

	/* Make sure they are null terminated. */
	p[args->lengths[0]] = '\0';
	s[args->lengths[1]] = '\0';

	/*
	 * If there's a 3rd argument which is nonzero, they want it case
	 * sensitive.
	 */
	if (args->arg_count == 3 && args->args[2] &&
	    *((long long *)args->args[2]) != 0) {
		res = (long long)do_match_wild(p, s, 1);
	} else {
		res = (long long)do_match_wild(p, s, 0);
	}

	free(p);
	free(s);
	return(res);
}

