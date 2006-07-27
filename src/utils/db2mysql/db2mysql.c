
/*
 * Read a set of Blitzed Services v2.0.x databases and output MySQL queries
 * necessary to populate a 3.0.0 MySQL database.
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 * 
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>
#include <getopt.h>
#include <mysql.h>

#include "../sysconf.h"
#include "db2mysql.h"
#include "extern.h"

RCSID("$Id$");

/*************************************************************************/

char *dbdir, *mysql_user, *mysql_host, *mysql_db, *mysql_pass;
int verbosity;

#if 0
int nakill;
struct akill *akills;
int nnews;
NewsItem *news;
int32 maxusercnt;
time_t maxusertime;
#endif

static void help(char *prog);

/*************************************************************************/

void debug(const char *data, ...)
{
	char data2[513];
	va_list arglist;

	if (verbosity < 2)
		return;

	va_start(arglist, data);
	vsnprintf(data2, 512, data, arglist);
	va_end(arglist);

	fprintf(stderr, "%s\n", data2);
}

void info(const char *data, ...)
{
	char data2[513];
	va_list arglist;

	if (verbosity < 1)
		return;

	va_start(arglist, data);
	vsnprintf(data2, 512, data, arglist);
	va_end(arglist);

	fprintf(stderr, "%s\n", data2);
}

static void help(char *prog)
{
	fprintf(stderr, "Usage: %s [OPTION] ... DATABASE\n", prog);
	fprintf(stderr, "Populate MySQL database DATABASE from services files.\n");
	fprintf(stderr, "\nRequired parameters:\n");
	fprintf(stderr, " -p, --pass=PASSWORD MySQL password\n");
	fprintf(stderr, " -u, --user=USER     MySQL username\n");
	fprintf(stderr, "\nOptional parameters:\n");
	fprintf(stderr, " -d, --dir=DIRECTORY directory containing database files\n");
	fprintf(stderr, "                     (otherwise current directory is assumed)\n");
	fprintf(stderr, " -h, --host=HOST     hostname of MySQL server\n");
	fprintf(stderr, "                     (otherwise localhost is assumed)\n");
	fprintf(stderr, " -v, --verbose       increase verbosity of debugging\n");
}

/*************************************************************************/


/*************************************************************************/

int main(int argc, char **argv)
{
	int c, option_index;
	
	static struct option long_options[] = {
		{"dir", 1, 0, 'd'},
		{"host", 1, 0, 'h'},
		{"pass", 1, 0, 'p'},
		{"user", 1, 0, 'u'},
		{"verbose", 0, 0, 'v'},
		{0, 0, 0, 0}
	};

	dbdir = mysql_user = mysql_host = mysql_db = mysql_pass = NULL;
	verbosity = 0;

	while (1) {
		option_index = 0;
		c = getopt_long(argc, argv, "d:h:p:u:v", long_options,
		    &option_index);

		if (c == -1)
			break;

		switch (c) {
		case 'd':
			dbdir = sstrdup(optarg);
			break;

		case 'h':
			mysql_host = sstrdup(optarg);
			break;

		case 'p':
			mysql_pass = sstrdup(optarg);
			break;

		case 'u':
			mysql_user = sstrdup(optarg);
			break;
			
		case 'v':
			verbosity++;
			break;
			
		default:
			help(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (optind < argc)
		mysql_db = sstrdup(argv[optind]);

	if (!mysql_user || !mysql_pass || !mysql_db) {
		help(argv[0]);
		exit(EXIT_FAILURE);
	}

	if (!dbdir)
		dbdir = sstrdup(".");
	if (!mysql_host)
		mysql_host = sstrdup("localhost");

	smysql_init();

	load_ns_dbase();
	load_os_dbase();
	load_cs_dbase();
	load_akill_dbase();
	load_news_dbase();
	load_exception_dbase();

	exit(EXIT_SUCCESS);
}
