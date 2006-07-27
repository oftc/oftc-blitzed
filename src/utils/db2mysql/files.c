
/*
 * File and database handling functions for db2mysql.
 * 
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 * 
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include <time.h>
#include <stdio.h>
#include <sys/param.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <mysql.h>

#include "../sysconf.h"
#include "db2mysql.h"
#include "extern.h"

RCSID("$Id$");

extern char *dbdir;

/*************************************************************************/

/* Generic routine to make a backup copy of a file. */

void make_backup(const char *name)
{
	char buf[PATH_MAX];
	FILE *in, *out;
	int n;

	snprintf(buf, sizeof(buf), "%s~", name);
	if (strcmp(buf, name) == 0) {
		fprintf(stderr, "Can't back up %s: Path too long\n", name);
		exit(1);
	}
	in = fopen(name, "rb");
	if (!in) {
		fprintf(stderr, "Can't open %s for writing", buf);
		perror("");
		exit(1);
	}
	out = fopen(buf, "wb");
	if (!out) {
		fprintf(stderr, "Can't open %s for writing", buf);
		perror("");
		exit(1);
	}
	while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
		if (fwrite(buf, 1, (size_t)n, out) != (size_t)n) {
			fprintf(stderr, "Write error on %s", buf);
			perror("");
			exit(1);
		}
	}
	fclose(in);
	fclose(out);
}

dbFILE *open_db_read(const char *service, const char *filename)
{
	int errno_save;
	dbFILE *f;
	FILE *fp;

	f = malloc(sizeof(*f));

	if (!f) {
		fprintf(stderr, "Can't read %s database %s\n", service,
		    filename);
		return(NULL);
	}

	snprintf(f->filename, sizeof(f->filename), "%s/%s", dbdir, filename);
	f->mode = 'r';
	fp = fopen(f->filename, "rb");

	if (!fp) {
		errno_save = errno;
		
		fprintf(stderr, "Can't read %s database %s\n", service,
		    f->filename);
		free(f);
		errno = errno_save;
		return(NULL);
	}

	f->fp = fp;
	f->backupfp = NULL;

	return(f);
}

dbFILE *open_db(const char *service, const char *filename, const char *mode)
{
	if (*mode == 'r') {
		return open_db_read(service, filename);
	} else {
		errno = EINVAL;
		return(NULL);
	}
}

void close_db(dbFILE *f)
{
	fclose(f->fp);
	free(f);
}

int get_file_version(dbFILE *f)
{
	FILE *fp = f->fp;
	int version = fgetc(fp)<<24 | fgetc(fp)<<16 | fgetc(fp)<<8 | fgetc(fp);

	if (ferror(fp)) {
		fprintf(stderr, "Error reading version number on %s\n",
		    f->filename);
		return(0);
	} else if (feof(fp)) {
		fprintf(stderr, "Error reading version number on %s: End "
		    "of file detected\n", f->filename);
		return(0);
	} else if (version > FILE_VERSION || version < 1) {
		fprintf(stderr, "Invalid version number (%d) on %s\n",
		    version, f->filename);
		return(0);
	}

	return(version);
}

int read_int16(uint16 *ret, dbFILE *f)
{
	int c1, c2;

	c1 = fgetc(f->fp);
	c2 = fgetc(f->fp);

	if (c1 == EOF || c2 == EOF)
		return(-1);

	*ret = c1<<8 | c2;
	return(0);
}

int read_int32(uint32 *ret, dbFILE *f)
{
	int c1, c2, c3, c4;

	c1 = fgetc(f->fp);
	c2 = fgetc(f->fp);
	c3 = fgetc(f->fp);
	c4 = fgetc(f->fp);

	if (c1 == EOF || c2 == EOF || c3 == EOF || c4 == EOF)
		return(-1);

	*ret = c1<<24 | c2<<16 | c3 <<8 | c4;
	return(0);
}

int read_string(char **ret, dbFILE *f)
{
	uint16 len;
	char *s;

	if (read_int16(&len, f) < 0)
		return(-1);

	if (len == 0) {
		*ret = NULL;
		return(0);
	}

	s = smalloc(len);
	if (len != fread(s, 1, len, f->fp)) {
		free(s);
		return(-1);
	}

	*ret = s;
	return(0);
}
