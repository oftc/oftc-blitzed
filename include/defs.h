/*!
 * \file defs.h
 *
 * \brief Miscellaneous definitions.
 *
 * This file sets default values for any constants that should be in include
 * files but aren't, or that have wacky values.
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
 *
 * $Id$
 */

/*************************************************************************/

#ifndef NAME_MAX
# define NAME_MAX 255
#endif

#ifndef BUFSIZ
# define BUFSIZ 256
#else
# if BUFSIZ < 256
#  define BUFSIZ 256
# endif
#endif

/* Length of an array: */
#define lenof(a)	(sizeof(a) / sizeof(*(a)))

/* Telling compilers about printf()-like functions: */
#ifdef __GNUC__
# define FORMAT(type,fmt,start) __attribute__((format(type,fmt,start)))
#else
# define FORMAT(type,fmt,start)
#endif

/* Stuff to shut up warnings about rcsid being unused. */
#define USE_VAR(var)	static char sizeof##var = sizeof(sizeof##var) + sizeof(var)
#define RCSID(x)	static char rcsid[] = x; USE_VAR(rcsid);

/*************************************************************************/
