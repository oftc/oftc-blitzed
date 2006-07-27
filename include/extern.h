/*
 * Prototypes and external variable declarations.
 *
 * Blitzed Services copyright (c) 2000-2001 Blitzed Services team
 *    E-mail: services@lists.blitzed.org
 * Based on ircservices-4.4.8:
 * copyright (c) 1996-1999 Andrew Church
 *    E-mail: <achurch@dragonfire.net>
 * copyright (c) 1999-2000 Andrew Kempe.
 *    E-mail: <theshadow@shadowfire.org>
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 *
 * $Id$
 */

#ifndef EXTERN_H
#define EXTERN_H


#define E extern


/**** actions.c ****/

E void kill_user(const char *source, const char *user, const char *reason);
E void svskill(const char *source, const char *user, const char *reason);
E void bad_password(User *u);


/**** akill.c ****/

E void get_akill_stats(long *nrec, long *memuse);
E unsigned int num_akills(void);
E void load_akill(void);
E void save_akill(void);
E int check_akill(const char *nick, const char *username, const char *host);
E void expire_akills(void);
E void do_akill(User *u);
E unsigned int add_akill(const char *mask, const char *reason,
			 const char *who, const time_t expiry);


/**** auth.c ****/

E void expire_auth(time_t now);


/**** autojoin.c ****/

E void do_autojoin(User *u);
E void check_autojoins(User *u);
E void renumber_autojoins(unsigned int nick_id);


/**** cache.c ****/

E void nickcache_init(void);
E void nickcache_delall(void);
E int nickcache_get_language(unsigned int nick_id);
E void nickcache_set_language(unsigned int nick_id, unsigned int language);
E int nickcache_get_status(unsigned int nick_id);
E void nickcache_set_status(unsigned int nick_id, int status);
E void nickcache_add_status(unsigned int nick_id, int new_status);
E void nickcache_remove_status(unsigned int nick_id, int remove_status);
E struct nickcache *nickcache_update(unsigned int nick_id);


/**** channels.c ****/

#ifdef DEBUG_COMMANDS
E void send_channel_list(User *user);
E void send_channel_users(User *user);
#endif

E void get_channel_stats(long *nrec, long *memuse);
E Channel *findchan(const char *chan);
E Channel *findchan_by_id(unsigned int channel_id);
E Channel *firstchan(void);
E Channel *nextchan(void);

E void chan_adduser(User *user, const char *chan);
E void chan_deluser(User *user, Channel *c);

E void do_cmode(const char *source, int ac, char **av);
E void do_topic(const char *source, int ac, char **av);

E int only_one_user(const char *chan);

E void chan_update_events(time_t now);
E unsigned int chan_count_regex_match(regex_t *r);
E void check_autolimit(Channel *chan, time_t now, int allow_raise);

/**** chanserv.c ****/

E ChannelInfo *chanlists[256];

E void listchans(int count_only, const char *chan);
E void get_chanserv_stats(long *nrec, long *memuse);
E void cs_init(void);
E void chanserv(const char *source, char *buf);
E void load_cs_dbase(void);
E void save_cs_dbase(void);
E void check_modes(const char *chan);
E int check_valid_op(User *user, const char *chan, int newchan);
E int check_should_op(User *user, const char *chan);
E int check_should_voice(User *user, const char *chan);
E void check_chan_memos(User *u, unsigned int channel_id);
E int check_kick(User *user, const char *chan);
E void record_topic(const char *chan);
E void restore_topic(const char *chan);
E int check_topiclock(const char *chan, const char *setter, Channel *c);
E void expire_chans(void);
E void cs_mysql_remove_nick(unsigned int nick_id);
E ChannelInfo *cs_findchan(const char *chan);
E int check_access(User *user, unsigned int channel_id, int what);
E int check_chan_limit(unsigned int nick_id);
E void update_memo_time(User *u, const char *chan, time_t memo_time);
E void get_chan_name(unsigned int channel_id, char *name);
E unsigned int mysql_findchan(const char *chan);
E char *get_channame_from_id(unsigned int channel_id);
E void add_mlock_on(unsigned int channel_id, int modes);
E void remove_mlock_off(unsigned int channel_id, int modes);
E char *get_entrymsg(unsigned int channel_id);
E int get_chan_flags(unsigned int channel_id);
E int get_channelinfo_from_id(ChannelInfo *ci, unsigned int channel_id);
E void free_channelinfo(ChannelInfo *ci);
E void add_chan_flags(unsigned int channel_id, int flags);
E void remove_chan_flags(unsigned int channel_id, int flags);
E int get_access(User *user, unsigned int channel_id);
E int get_chanaccess_from_nickid(ChanAccess *ca, unsigned int nick_id,
    unsigned int channel_id);
E void free_chanaccess(ChanAccess *ca);
E int get_level(unsigned int channel_id, int what);
E int get_chan_level(unsigned int channel_id, int what);

/**** compat.c ****/

#if !HAVE_SNPRINTF
# if BAD_SNPRINTF
#  define snprintf my_snprintf
# endif
# define vsnprintf my_vsnprintf
E int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
E int snprintf(char *buf, size_t size, const char *fmt, ...);
#endif
#if !HAVE_STRICMP && !HAVE_STRCASECMP
E int stricmp(const char *s1, const char *s2);
E int strnicmp(const char *s1, const char *s2, size_t len);
#endif
#if !HAVE_STRDUP
E char *strdup(const char *s);
#endif
#if !HAVE_STRSPN
E size_t strspn(const char *s, const char *accept);
#endif
#if !HAVE_STRERROR
E char *strerror(int errnum);
#endif
#if !HAVE_STRSIGNAL
char *strsignal(int signum);
#endif


/**** config.c ****/

E char *RemoteServer;
E int   RemotePort;
E char *RemotePassword;
E char *LocalHost;
E int   LocalPort;

E char *ServerName;
E char *ServerDesc;
E char *ServiceUser;
E char *ServiceHost;

E char *s_NickServ;
E char *s_ChanServ;
E char *s_MemoServ;
E char *s_HelpServ;
E char *s_OperServ;
E char *s_GlobalNoticer;
E char *s_IrcIIHelp;
E char *s_DevNull;
E char *s_FloodServ;
E char *desc_NickServ;
E char *desc_ChanServ;
E char *desc_MemoServ;
E char *desc_HelpServ;
E char *desc_OperServ;
E char *desc_StatServ;
E char *desc_GlobalNoticer;
E char *desc_IrcIIHelp;
E char *desc_DevNull;
E char *desc_FloodServ;

E char *PIDFilename;
E char *MOTDFilename;
E char *HelpDir;

E char *mysqlHost;
E char *mysqlUser;
E char *mysqlPass;
E char *mysqlDB;
E unsigned int mysqlPort;
E char *mysqlSocket;

E int   NoSplitRecovery;
E int   StrictPasswords;
E int   BadPassLimit;
E int   BadPassTimeout;
E int   ExpireTimeout;
E int   AuthExpireTimeout;
E int   ReadTimeout;
E int   WarningTimeout;
E int   TimeoutCheck;
E char *SnoopChan;
E char *SendmailPath;
E char *AbuseEmail;
E char *NetworkName;
E int   StrictPassReject;
E int   StrictPassWarn;
E char *DefaultCloak;

E char *NSGuestNickPrefix;
E int	NSDefAutoJoin;
E int   NSDefEnforce;
E int   NSDefEnforceQuick;
E int   NSDefSecure;
E int   NSDefPrivate;
E int   NSDefHideEmail;
E int   NSDefHideUsermask;
E int   NSDefHideQuit;
E int   NSDefMemoSignon;
E int   NSDefMemoReceive;
E int   NSDefCloak;
E int   NSRegDelay;
E int   NSExpire;
E int   NSExtraGrace;
E unsigned int NSAccessMax;
E unsigned int NSAutoJoinMax;
E char *NSEnforcerUser;
E char *NSEnforcerHost;
E int   NSReleaseTimeout;
E int   NSDisableLinkCommand;
E int   NSListOpersOnly;
E int   NSListMax;
E int   NSSecureAdmins;
E int   NSSuspendExpire;
E int   NSSuspendGrace;
E int	NSNeedMail;
E char *NSNickURL;
E char *NSMyEmail;
E int	NSWallReg;
E char *NSRegExtraInfo;
E int	NSRegNeedAuth;

E int   CSMaxReg;
E int   CSExpire;
E unsigned int CSAccessMax;
E int   CSAutokickMax;
E char *CSAutokickReason;
E int   CSInhabit;
E int   CSRestrictDelay;
E int   CSListOpersOnly;
E int   CSListMax;
E int   CSSuspendExpire;
E int   CSSuspendGrace;
E int	CSOpOper;
E int	CSAkickOper;
E char *CSMyEmail;
E int	CSWallReg;
E int   CSExtraGrace;

E int   MSMaxMemos;
E int	MSROT13;
E int   MSSendDelay;
E int   MSNotifyAll;

E char *ServicesRoot;
E int   LogMaxUsers;
E char *StaticAkillReason;
E int   ImmediatelySendAkill;
E int	AkillWildThresh;
E int   AutokillExpiry;
E int   WallOper;
E int   WallBadOS;
E int   WallOSMode;
E int   WallOSClearmodes;
E int   WallOSKick;
E int	WallOSException;
E int   WallOSAkill;
E int   WallAkillExpire;
E int   WallExceptionExpire;
E int   WallGetpass;
E int   WallSetpass;

E int   LimitSessions;
E int   DefSessionLimit;
E int   ExceptionExpiry;
E unsigned int MaxSessionLimit;
E int	SessionAkillExpiry;
E unsigned int SessionKillClues;
E char *SessionLimitDetailsLoc;
E char *SessionLimitExceeded;

E int	FloodServAkillExpiry;
E char	*FloodServAkillReason;
E int	FloodServTFNumLines;
E int	FloodServTFSec;
E int	FloodServTFNLWarned;
E int	FloodServTFSecWarned;
E int	FloodServWarnFirst;
E char	*FloodServWarnMsg;

E int read_config(void);
E void free_directives(void);


/**** cs_access.c ****/
E void do_cs_access(User *u);
E void do_cs_vop(User *u);
E void do_cs_aop(User *u);
E void do_cs_sop(User *u);


/**** helpserv.c ****/

E void helpserv(const char *whoami, const char *source, char *buf);


/**** init.c ****/

E void introduce_user(const char *user);
E int init(int ac, char **av);


/**** language.c ****/

E char **langtexts[NUM_LANGS];
E char *langnames[NUM_LANGS];
E int langlist[NUM_LANGS];

E void lang_init(void);
E size_t strftime_lang(char *buf, size_t size, User *u, int format,
    struct tm *tm);
E void expires_in_lang(char *buf, size_t size, User *u, time_t seconds);
E void free_langs(void);
E void syntax_error(const char *service, User *u, const char *command,
    unsigned int msgnum);
E const char *getstring(unsigned int nick_id, unsigned int index);


/**** list.c ****/

E void do_listnicks(int ac, char **av);
E void do_listchans(int ac, char **av);


/**** log.c ****/

E int open_log(void);
E void close_log(void);
E void log(const char *fmt, ...)		FORMAT(printf,1,2);
E void log_perror(const char *fmt, ...)		FORMAT(printf,1,2);
E void snoop(const char *source, const char *fmt, ...)
        FORMAT(printf,2,3);
E void fatal(const char *fmt, ...)		FORMAT(printf,1,2);
E void fatal_perror(const char *fmt, ...)	FORMAT(printf,1,2);

E void rotate_log(User *u);


/**** main.c ****/

E const char version_branchstatus[];
E const char version_number[];
E const char version_build[];
E const char version_protocol[];
E const char *info_text[];

E char *services_dir;
E char *etcdir;
E char *log_filename;
E int   debug;
E int   skeleton;
E int   nofork;
E int   forceload;
E int	opt_noexpire;

E int   quitting;
E int   delayed_quit;
E char *quitmsg;
E char  inbuf[BUFSIZE];
E int   servsock;
E int   save_data;
E int   got_alarm;
E time_t start_time;
E volatile time_t last_update;
E volatile time_t last_chanevent;
E sigjmp_buf panic_jmp;


/**** memory.c ****/

E void *smalloc(size_t size);
E void *scalloc(size_t elsize, size_t els);
E void *srealloc(void *oldptr, size_t newsize);
E char *sstrdup(const char *s);


/**** memoserv.c ****/

E void ms_init(void);
E void memoserv(const char *source, char *buf);
E void get_memoserv_stats(long *nrec, long *memuse);
E void load_old_ms_dbase(void);
E void check_memos(User *u);
E int new_chan_memos(unsigned int channel_id, time_t last_read);


/**** messages.c ****/

E void m_version(char *source, int ac, char **av);
E void m_whois(char *source, int ac, char **av);


/**** misc.c ****/

E int irc_toupper(char c);
E int irc_tolower(char c);
E int irc_stricmp(const char *s1, const char *s2);
E char *strscpy(char *d, const char *s, size_t len);
E char *stristr(char *s1, char *s2);
E char *strupper(char *s);
E char *strlower(char *s);
E char *strnrepl(char *s, size_t size, const char *old, const char *new);

E char *merge_args(int argc, char **argv);

E int match_wild(const char *pattern, const char *str);
E int match_wild_nocase(const char *pattern, const char *str);

typedef int (*range_callback_t)(User *u, unsigned int num, va_list args);
E int process_numlist(const char *numstr, int *count_ret, unsigned int min,
    unsigned int max, range_callback_t callback, User *u, ...);
E int dotime(const char *s);
E char *time_ago(time_t mytime);
E char *disect_time(time_t time);
E void do_motd(const char *source);
E int validate_email(const char *email);
E void make_random_string(char *buffer, size_t length);
E unsigned int validate_password(User *u, const char *pass, char *channame);
E void fatal_error(const char *quitmsg);
E char *rot13(char *str);


/**** mysql.c ****/

E int log_sql;

E MYSQL mysql;
E MYSQL *mysqlconn;
E void smysql_init(void);
E void handle_mysql_error(MYSQL *mysql, const char *msg);
E MYSQL_RES *smysql_bulk_query(MYSQL *mysql, unsigned int *num_fields,
			       unsigned int *num_rows, const char *fmt,
			       ...) FORMAT(printf,4,5);
E MYSQL_RES *mysql_simple_query(MYSQL *mysql, unsigned int *err,
				const char *fmt, ...) FORMAT(printf,3,4);
E MYSQL_RES *smysql_query(MYSQL *mysql, const char *fmt, ...)
			  FORMAT(printf,2,3);
E MYSQL_ROW smysql_fetch_row(MYSQL *mysql, MYSQL_RES *result);
E void get_table_stats(MYSQL *mysql, const char *table,
		       unsigned int *nrec, unsigned int *bytes);
E unsigned int get_table_rows(MYSQL *mysql, const char *table);
E unsigned int count_matching_rows(MYSQL *mysql, const char *table,
		                   const char *column, const char *string);
E void read_lock_table(MYSQL *mysql, const char *table);
E void write_lock_table(MYSQL *mysql, const char *table);
E void unlock(MYSQL *mysql);
E void smysql_cleanup(MYSQL *mysql);
E int check_chunk(int i, size_t *size, char *chunk);
E char *mysql_build_query(MYSQL *mysql, const char *numstr,
			  const char *base, const char *column,
			  unsigned int max);
E char *mysql_build_query_match_mask(MYSQL *mysql, const char *mask,
				     const char *base, const char *table,
				     const char *id_col, const char *mask_col);
E char *smysql_escape_string(MYSQL *mysql, const char *string);
E void smysql_log(const char *fmt, ...)		FORMAT(printf,1,2);
E int open_mysql_log(void);
E void close_mysql_log(void);


/**** news.c ****/

E void get_news_stats(long *nrec, long *memuse);
E void load_news(void);
E void save_news(void);
E void display_news(User *u, int type);
E void do_logonnews(User *u);
E void do_opernews(User *u);


/**** nickserv.c ****/

E NickInfo *nicklists[256];

E void ns_init(void);
E void listnicks(int count_only, const char *nick);
E void get_nickserv_stats(long *nrec, long *memuse);
E void nickserv(const char *source, char *buf);
E void load_ns_dbase(void);
E void save_ns_dbase(void);
E int check_on_access(User *u);
E int validate_user(User *u);
E void cancel_user(User *u);
E int nick_identified(User *u);
E int nick_recognized(User *u);
E void expire_nicksuspends(void);
E void expire_nicks(void);
E void set_nick_status(unsigned int nick_id, int status);
E int get_nick_status(unsigned int nick_id);
E NickInfo *findnick(const char *nick);
E unsigned int mysql_findnick(const char *nick);
E unsigned int mysql_findnickinfo(NickInfo *ni, const char *nick);
E NickInfo *getlink(NickInfo *ni);
E unsigned int mysql_getlink(unsigned int nick_id);
E unsigned int get_nickinfo_from_id(NickInfo *ni, unsigned int nick_id);
E void free_nickinfo(NickInfo *ni);
E char *get_nick_from_id(unsigned int nick_id);
E char *get_email_from_id(unsigned int nick_id);
E char *get_cloak_from_id(unsigned int nick_id);
E int get_nick_flags(unsigned int nick_id);
E void add_nick_flags(unsigned int nick_id, int new_flags);
E void remove_nick_flags(unsigned int nick_id, int remove_flags);
E void mysql_update_last_seen(User *u, int status);


/**** operserv.c ****/

E NickInfo *services_opers[MAX_SERVOPERS];

E void operserv(const char *source, char *buf);

E void os_init(void);
E void load_os_dbase(void);
E void save_os_dbase(void);
E int is_services_root(User *u);
E int is_services_admin(User *u);
E int is_services_oper(User *u);
E int nick_is_services_admin(unsigned int nick_id);
E void os_mysql_remove_nick(unsigned int nick_id);


/**** process.c ****/

E int allow_ignore;
E IgnoreData *ignore[];

E void add_ignore(const char *nick, time_t delta);
E IgnoreData *get_ignore(const char *nick);

E int split_buf(char *buf, char ***argv, int colon_special);
E void process(void);


/**** quarantine.c ****/

E void do_sqline(const char *source, int ac, char **av);
E void do_quarantine(User *u);
E void send_sqlines(void);


/**** send.c ****/

E void send_cmd(const char *source, const char *fmt, ...)
	FORMAT(printf,2,3);
E void vsend_cmd(const char *source, const char *fmt, va_list args)
	FORMAT(printf,2,0);
E void wallops(const char *source, const char *fmt, ...)
	FORMAT(printf,2,3);
E void notice(const char *source, const char *dest, const char *fmt, ...)
	FORMAT(printf,3,4);
E void opnotice(const char *source, const char *chan, const char *fmt, ...)
	FORMAT(printf,3,4);
E void global_notice(const char *source, const char *fmt, ...);
E void notice_list(const char *source, const char *dest, const char **text);
E void notice_lang(const char *source, User *dest, unsigned int message,
    ...);
E void notice_help(const char *source, User *dest, unsigned int message,
    ...);
E void privmsg(const char *source, const char *dest, const char *fmt, ...)
	FORMAT(printf,3,4);
E void send_nick(const char *nick, const char *user, const char *host,
                const char *server, const char *name);

/**** servers.c ****/

#ifdef DEBUG_COMMANDS
E void send_server_list(User *user);
#endif

E void get_server_stats(long *nservers, long *memuse);
E Server *findserver(const char *servername);
E void do_server(const char *source, int ac, char **av);
E void do_squit(const char *source, int ac, char **av);

/**** sessions.c ****/

E void get_session_stats(long *nrec, long *memuse);
E void get_exception_stats(long *nrec, long *memuse);

E void do_session(User *u);
E int add_session(const char *nick, const char *host);
E void del_session(const char *host);

E void load_exceptions(void);
E void save_exceptions(void);
E void do_exception(User *u);
E void expire_exceptions(void);
E unsigned int num_exceptions(void);

/**** sha1.c ****/

E void sha1_init(SHA1_CONTEXT *hd);
E const char *sha1_get_info(int algo, size_t *contextsize,
    unsigned char **r_asnoid, int *r_asnlen, int *r_mdlen,
    void (**r_init)(void *c),
    void (**r_write)(void *c, unsigned char *buf, size_t nbytes),
    void (**r_final)(void *c), unsigned char *(**r_read)(void *c));
E void make_sha_hash(const char *passwd, char *buffer);


/**** sockutil.c ****/

E int32 total_read, total_written;
E int32 read_buffer_len(void);
E int32 write_buffer_len(void);

E int sgetc(int s);
E char *sgets(char *buf, int len, int s);
E int sgets2(char *buf, int len, int s);
E int sread(int s, char *buf, int len);
E int sputs(char *str, int s);
E int sockprintf(int s, char *fmt,...);
E int conn(const char *host, int port, const char *lhost, int lport);
E void disconn(int s);


/**** users.c ****/

E int32 usercnt, opcnt, maxusercnt;
E time_t maxusertime;

#ifdef DEBUG_COMMANDS
#if 0
E void send_user_list(User *user);
E void send_user_info(User *user);
#endif
#endif

E void get_user_stats(long *nusers, long *memuse);
E User *finduser(const char *nick);
E User *firstuser(void);
E User *nextuser(void);

E int do_nick(const char *source, int ac, char **av);
E void do_join(const char *source, int ac, char **av);
E void do_sjoin(const char *source, int ac, char **av);
E void do_part(const char *source, int ac, char **av);
E void do_kick(const char *source, int ac, char **av);
E void do_umode(const char *source, int ac, char **av);
E void do_quit(const char *source, int ac, char **av);
E void do_kill(const char *source, int ac, char **av);

E int is_oper(const char *nick);
E int is_on_chan(const char *nick, const char *chan);
E int is_chanop(const char *nick, const char *chan);
E int is_voiced(const char *nick, const char *chan);
E int is_banned(User *u, Channel *c);

E int match_usermask(const char *mask, User *user);
E int count_mask_matches(const char *mask);
E void split_usermask(const char *mask, char **nick, char **user, char **host);
E char *create_mask(User *u);

E unsigned int user_count_regex_match(regex_t *r);

E unsigned int chan_count_users(Channel *c);

E void fs_init(void);
E void fs_add_chan(User *u, unsigned int channel_id, const char *name);
E void fs_del_chan(User *u, unsigned int channel_id, const char *name);
E void floodserv(const char *source, char *buf);
E void privmsg_flood(const char *source, int ac, char **av);

#endif	/* EXTERN_H */
