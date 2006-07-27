
/*
 * String functions for db2mysql.
 * 
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 * 
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/param.h>
#include <mysql.h>

#include "../sysconf.h"
#include "db2mysql.h"
#include "extern.h"

RCSID("$Id$");

/*************************************************************************/

/*
 * Case-insensitive versions of strcmp() and strncmp().
 */
int stricmp(const char *s1, const char *s2)
{
	register int c;

	while ((c = tolower(*s1)) == tolower(*s2)) {
		if (c == 0)
			return(0);
		
		s1++;
		s2++;
	}

	if (c < tolower(*s2))
		return(-1);

	return(1);
}

int strnicmp(const char *s1, const char *s2, size_t len)
{
	register int c;

	if (!len)
		return(0);

	while ((c = tolower(*s1)) == tolower(*s2) && len > 0) {
		if (c == 0 || --len == 0)
			return(0);

		s1++;
		s2++;
	}

	if (c < tolower(*s2))
		return(-1);

	return(1);
}

char *strscpy(char *d, const char *s, size_t len)
{
	char *d_orig = d;

	if (!len)
		return(d);

	while (--len && (*d++ = *s++))
		;

	*d = 0;
	return(d_orig);
}

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

