/* FloodServ structures.
 *
 * Add an way to break flood attack,
 * -primary by checking flood from channel from a nick
 * -or from a host
 * Copyright (c) 2001 Philippe Levesque <EMail: yagmoth555@yahoo.com>
 * See www.flamez.net for more info
 */

#ifndef FLOODSERV_H
#define FLOODSERV_H

/* Default value for the text-flood
 * 5 lines in 5 sec seem a good value =] -jabea.
 *
#define TXTFLOOD_LINE	5		Numeric value representing the number(s) of line(s) writted
#define TXTFLOOD_SEC	5		Numeric value representating the second(s)
#define TXTFLOOD_LWN	7   	Numeric value representing the number(s) of line(s) writted
#define TXTFLOOD_WARN  20		Numeric value representating the second(s)
*/

typedef struct txtflood_ TxtFloods;
typedef struct flooder_ Flooders;

struct flooder_ {
		Flooders *next;
		char nick[NICKMAX];			/* The *first nick* from the host we stopped on */
		char *host;					/* From what host he's on */
		int	 akilled;				/* AKILLED? */
};

struct txtflood_ {
		dlink_list flooder;			/* Who triggered it [key of the struct] */
		char *txtbuffer;			/* TEXT / CTCP */
		time_t time;				/* When the txt was last said */
		int repeat;					/* How many time */
		int warned;					/* WARNED? */
        int chanid;                 /* Channel_is this text relates to */
};

#endif	/* FLOODSERV_H */
