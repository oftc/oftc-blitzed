
/*
 * Memory-related functions for db2mysql.
 * 
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 * 
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include <time.h>
#include <stdio.h>
#include <sys/param.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#include "../sysconf.h"
#include "db2mysql.h"
#include "extern.h"

RCSID("$Id$");

/*************************************************************************/

/* Safe memory allocation. */

void *smalloc(size_t size)
{
	void *ptr = malloc(size);

	if (size && !ptr) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}
	return(ptr);
}

void *scalloc(size_t elsize, size_t els)
{
	void *buf;

	if (!elsize || !els) {
		fprintf(stderr,
		    "scalloc: Illegal attempt to allocate 0 bytes");
		elsize = els = 1;
	}

	buf = calloc(elsize, els);

	if (!buf) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	return(buf);
}

char *sstrdup(const char *s)
{
	char *t;

	t = strdup(s);

	if (!t) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	return(t);
}
