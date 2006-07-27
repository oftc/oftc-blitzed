/* Memory management routines.
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

/*************************************************************************/
/*************************************************************************/

/* smalloc, scalloc, srealloc, sstrdup:
 *	Versions of the memory allocation functions which will cause the
 *	program to terminate with an "Out of memory" error if the memory
 *	cannot be allocated.  (Hence, the return value from these functions
 *	is never NULL.)
 */

void *smalloc(size_t size)
{
    void *buf;

    if (!size) {
	log("smalloc: Illegal attempt to allocate 0 bytes");
	size = 1;
    }
    buf = malloc(size);
    if (!buf)
	raise(SIGUSR1);
    return buf;
}

void *scalloc(size_t elsize, size_t els)
{
    void *buf;

    if (!elsize || !els) {
	log("scalloc: Illegal attempt to allocate 0 bytes");
	elsize = els = 1;
    }
    buf = calloc(elsize, els);
    if (!buf)
	raise(SIGUSR1);
    return buf;
}

void *srealloc(void *oldptr, size_t newsize)
{
    void *buf;

    if (!newsize) {
	log("srealloc: Illegal attempt to allocate 0 bytes");
	newsize = 1;
    }
    buf = realloc(oldptr, newsize);
    if (!buf)
	raise(SIGUSR1);
    return buf;
}

char *sstrdup(const char *s)
{
    char *t = strdup(s);
    if (!t)
	raise(SIGUSR1);
    return t;
}

/*************************************************************************/
/*************************************************************************/

/* In the future: malloc() replacements that tell us if we're leaking and
 * maybe do sanity checks too... */

/*************************************************************************/
