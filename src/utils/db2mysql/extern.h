/*
 * External references.
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 *     E-mail: services@lists.blitzed.org
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 *
 * $Id$
 */

/* akill.c */
extern void load_akill_dbase(void);

/* chanserv.c */
extern void load_cs_dbase(void);
extern ChannelInfo *cs_findchan(const char *chan);

/* db2mysql.c */
extern void debug(const char *data, ...);
extern void info(const char *data, ...);

/* exception.c */
extern void load_exception_dbase(void);

/* files.c */
extern void make_backup(const char *name);
extern dbFILE *open_db_read(const char *service, const char *filename);
extern dbFILE *open_db(const char *service, const char *filename,
    const char *mode);
extern int get_file_version(dbFILE *f);
extern int read_int16(uint16 *ret, dbFILE *f);
extern int read_int32(uint32 *ret, dbFILE *f);
extern int read_string(char **ret, dbFILE *f);
extern void close_db(dbFILE *f);

/* memo.c */
extern void db_add_memoinfo(const char *nick, unsigned int memomax);
extern void db_add_memo(const char *owner, Memo *memo);

/* memory.c */
extern void *smalloc(size_t size);
extern void *scalloc(size_t elsize, size_t els);
extern char *sstrdup(const char *s);

/* mysql.c */
extern void smysql_init(void);
extern char *smysql_escape_string(MYSQL *mysql, const char *string);
extern MYSQL_RES *smysql_query(MYSQL *mysql, unsigned int *num_fields,
    unsigned int *num_rows, const char *fmt, ...);
extern MYSQL_ROW smysql_fetch_row(MYSQL *mysql, MYSQL_RES *result);

/* news.c */
extern void load_news_dbase(void);

/* nickserv.c */
extern void load_ns_dbase(void);
extern NickInfo *findnick(const char *nick);

/* operserv.c */
extern void load_os_dbase(void);

/* string.c */
extern int stricmp(const char *s1, const char *s2);
extern int strnicmp(const char *s1, const char *s2, size_t len);
extern char *strscpy(char *d, const char *s, size_t len);
extern void make_random_string(char *buffer, size_t length);
