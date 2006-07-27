/*
 * Required data structures and other definitions for db2mysql.
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 *     E-mail: services@lists.blitzed.org
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 *
 * $Id$
 */

#ifndef DB2MYSQL_H
#define DB2MYSQL_H

/* Stuff to shut up warnings about rcsid being unused. */
#define USE_VAR(var)    static char sizeof##var = sizeof(sizeof##var) + sizeof(var)
#define RCSID(x)        static char rcsid[] = x; USE_VAR(rcsid);

#define NickDBName	"nick.db"
#define OperDBName	"oper.db"
#define ChanDBName	"chan.db"
#define AutoKillDBName	"akill.db"
#define NewsDBName	"news.db"
#define ExceptionDBName	"exception.db"
#define s_NickServ	"NickServ"
#define s_OperServ	"OperServ"
#define s_ChanServ	"ChanServ"

#define DEF_LANGUAGE    LANG_EN_US
#define MAX_SERVADMINS  32
#define MAX_SERVOPERS   64
#define CHANMAX         64
#define NICKMAX         32
#define PASSMAX         32
#define SALTMAX		17
#define FILE_VERSION    11
#define MSMaxMemos	20
#define CSMaxReg	20
#define LANG_EN_US	0

#define mysql_port	0
#define mysql_socket	NULL

#define read_db(f,buf,len)	(fread((buf),1,(len),(f)->fp))
#define getc_db(f)		(fgetc((f)->fp))
#define read_buffer(buf,f)	(read_db((f),(buf),sizeof(buf)) == sizeof(buf))

typedef struct memo_ Memo;
struct memo_ {
	uint32 number;
	int16 flags;
	time_t time;
	char sender[NICKMAX];
	char *text;
};

typedef struct {
	int16 memocount, memomax;
	Memo *memos;
} MemoInfo;

typedef struct channellist_ ChannelList;
typedef struct nickinfo_ NickInfo;

struct nickinfo_ {
	NickInfo *next, *prev;
	char nick[NICKMAX];
	char pass[PASSMAX];
	unsigned int nick_id;
	unsigned int link_id;
	char salt[SALTMAX];
	char last_sendpass_pass[PASSMAX];
	char last_sendpass_salt[SALTMAX];
	time_t last_sendpass_time;
	char *url;
	char *email;
	char *last_usermask;
	char *last_realname;
	char *last_quit;
	time_t time_registered;
	time_t last_seen;
	int16 status;
	NickInfo *link;
	int16 linkcount;
	ChannelList *chanlist;
	int32 flags;
	int16 accesscount;
	char **access;
	MemoInfo memos;
	uint16 channelcount;
	uint16 channelmax;
	uint16 language;
	time_t id_stamp;
	uint32 regainid;
};

#define NS_ENCRYPTEDPW  0x0001
#define NS_VERBOTEN     0x0002
#define NS_NO_EXPIRE    0x0004
#define NS_IDENTIFIED   0x8000
#define NS_RECOGNIZED   0x4000
#define NS_ON_ACCESS    0x2000
#define NS_KILL_HELD    0x1000
#define NS_GUESTED      0x0100
#define NS_REGAINED     0x0200
#define NS_TEMPORARY    0xFF00

#define NI_ENFORCE      0x00000001
#define NI_SECURE       0x00000002
#define NI_MEMO_HARDMAX 0x00000008
#define NI_MEMO_SIGNON  0x00000010
#define NI_MEMO_RECEIVE 0x00000020
#define NI_PRIVATE      0x00000040
#define NI_HIDE_EMAIL   0x00000080
#define NI_HIDE_MASK    0x00000100
#define NI_HIDE_QUIT    0x00000200
#define NI_ENFORCEQUICK 0x00000400
#define NI_NOOP         0x00000800
#define NI_IRCOP        0x00001000
#define NI_MARKED       0x00002000
#define NI_NOSENDPASS   0x00004000
#define NI_AUTOJOIN	0x00008000
#define NI_SUSPENDED    0x10000000

#define CI_VERBOTEN	0x00000080

#define ACCESS_FOUNDER		10000
#define ACCESS_INVALID		-10000

#define CA_INVITE		0
#define CA_AKICK		1
#define CA_SET			2
#define CA_UNBAN		3
#define CA_AUTOOP		4
#define CA_AUTODEOP		5
#define CA_AUTOVOICE		6
#define CA_OPDEOP		7
#define CA_ACCESS_LIST		8
#define CA_CLEAR		9
#define CA_NOJOIN		10
#define CA_ACCESS_CHANGE	11
#define CA_MEMO_READ		12
#define CA_LEVEL_LIST		13
#define CA_LEVEL_CHANGE		14
#define CA_SYNC			15
#define CA_MEMO_SEND		16
#define CA_AKICK_LIST		17

#define CA_SIZE			18

typedef struct {
	int16 in_use;
	int16 level;
	NickInfo *ni;
	time_t added;
	time_t last_modified;
	time_t last_used;
	time_t memo_read;
	char added_by[NICKMAX];
	char modified_by[NICKMAX];
} ChanAccess;

typedef struct {
	int16 in_use;
	int16 is_nick;
	union {
		char *mask;
		NickInfo *ni;
	} u;
	char *reason;
	char who[NICKMAX];
	time_t added;
	time_t last_used;
} AutoKick;

typedef struct chaninfo_ ChannelInfo;

struct chaninfo_ {
	ChannelInfo *next, *prev;
	char name[CHANMAX];
	NickInfo *founder_ni;
	NickInfo *successor_ni;
	char founderpass[PASSMAX];
	char *desc;
	char *url;
	char *email;
	time_t time_registered;
	time_t last_used;
	time_t founder_memo_read;
	char *last_topic;
	char last_topic_setter[NICKMAX];
	time_t last_topic_time;
	int32 flags;
	int16 *levels;
	int16 accesscount;
	ChanAccess *access;
	int16 akickcount;
	AutoKick *akick;
	int16 mlock_on, mlock_off;
	int32 mlock_limit;
	char *mlock_key;
	char *entry_message;
	MemoInfo memos;
	struct channel_ *c;
	time_t last_limit_time;
	int16 limit_offset;
	int16 limit_tolerance;
	int16 limit_period;
	int16 bantime;

	unsigned int channel_id;
	unsigned int founder;
	unsigned int successor;
	char salt[SALTMAX];
	char last_sendpass_pass[PASSMAX];
	char last_sendpass_salt[SALTMAX];
	time_t last_sendpass_time;
};

struct channellist_ {
	ChannelList *next;
	ChannelInfo *ci;
	int16 level;
};

typedef struct suspendinfo_ SuspendInfo;
struct suspendinfo_ {
	char who[NICKMAX];
	char *reason;
	time_t suspended;
	time_t expires;
};

typedef struct suspendednick_ NickSuspend;
struct suspendednick_ {
	NickSuspend *prev, *next;
	NickInfo *ni;
	SuspendInfo suspendinfo;
};

typedef struct suspendedchan_ ChanSuspend;
struct suspendedChan_ {
	ChanSuspend *prev, *next;
	ChannelInfo *ci;
	SuspendInfo suspendinfo;
};

typedef struct dbFILE_ dbFILE;
struct dbFILE_ {
	int mode;
	FILE *fp;
	FILE *backupfp;
	char filename[PATH_MAX];
	char backupname[PATH_MAX];
};

typedef struct akill Akill;
struct akill {
	char *mask;
	char *reason;
	char who[NICKMAX];
	time_t time;
	time_t expires;
};

typedef struct newsitem_ NewsItem;
struct newsitem_ {
	int16 type;
	int32 num;
	char *text;
	char who[NICKMAX];
	time_t time;
};

typedef struct exception_ Exception;
struct exception_ {
	char *mask;
	int limit;
	char who[NICKMAX];
	char *reason;
	time_t time;
	time_t expires;
	int num;
};

#endif
