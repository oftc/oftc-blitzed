/*! \file services.h 
 *  \brief Main header for Services.
 */

/*
 * Blitzed Services copyright (c) 2000-2001 Blitzed Services team
 *     E-mail: services@lists.blitzed.org
 * Based on ircservices-4.4.8:
 * copyright (c) 1996-1999 Andrew Church
 *     E-mail: <achurch@dragonfire.net>
 * copyright (c) 1999-2000 Andrew Kempe.
 *     E-mail: <theshadow@shadowfire.org>
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 *
 * $Id$
 */

#ifndef SERVICES_H
#define SERVICES_H

/*************************************************************************/

#include "sysconf.h"
#include "config.h"


#define VERSION PACKAGE_VERSION

#define log slog

/*!
 * \brief Some Linux boxes (or maybe glibc includes) require this for the
 * prototype of strsignal().
 */
#if INTTYPE_WORKAROUND
/*! \brief Some AIX boxes define int16 and int32 on their own.  Blarph. */
# define int16 builtin_int16
# define int32 builtin_int32
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <grp.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <sys/socket.h>
#include <sys/stat.h>		/* For umask() on some systems */
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <mysql.h>
#include <mysqld_error.h>
#include <errmsg.h>
#include <regex.h>
#include <assert.h>

#if HAVE_STRINGS_H
# include <strings.h>
#endif

#if HAVE_SYS_SELECT_H
# include <sys/select.h>
#endif

#if !HAVE_STRICMP && HAVE_STRCASECMP
/*!
 * \brief Alias stricmp to strcasecmp if we have the later but not the
 * former.
 */
# define stricmp strcasecmp
/*!
 * \brief Alias strnicmp to strncasecmp if we have the later but not the
 * former.
 */
# define strnicmp strncasecmp
#endif

#include <ctype.h>
#undef tolower
#undef toupper
/*! \brief We have our own version of tolower(). */
#define tolower tolower_
/*! \brief We have our own version of toupper(). */
#define toupper toupper_
extern int toupper(int), tolower(int);

/*! \brief We have our own encrypt(). */
#define encrypt encrypt_


#if INTTYPE_WORKAROUND
# undef int16
# undef int32
#endif


#include "defs.h"

/*************************************************************************/

/* Configuration sanity-checking: */
#if CHANNEL_MAXREG > 32767
# undef CHANNEL_MAXREG
# define CHANNEL_MAXREG	0
#endif
#if DEF_MAX_MEMOS > 32767
# undef DEF_MAX_MEMOS
# define DEF_MAX_MEMOS	0
#endif
#if MAX_SERVOPERS > 32767
# undef MAX_SERVOPERS
# define MAX_SERVOPERS 32767
#endif
#if MAX_SERVADMINS > 32767
# undef MAX_SERVADMINS
# define MAX_SERVADMINS 32767
#endif

/*************************************************************************/

/*************************************************************************/

typedef struct memo_ Memo;

/*!
 * \brief Memo data structures.
 *
 * Since both nicknames and channels can have memos, we encapsulate memo data
 * in a generic Memo structure to make it easier to handle.
 */

struct memo_ {
	/*! \brief Unique memo identifier. */
	unsigned int memo_id;
	
	/*! \brief Index within the user's memos. */
	unsigned int idx;
	
	/*! \brief Flags for memo metadata. */
	int flags;
	
	/*!
	 * \brief String identifying owner of this memo (nick or
	 * channel).
	 */
	char *owner;
	
	/*! \brief When the memo was sent. */
	time_t time;
	
	/*! \brief Nick of memo sender. */
	char sender[NICKMAX];
	
	/*! \brief Memo content. */
	char *text;
};

/*!
 * \name Memo Flags
 * Flags for memo metadata.
 */
/* @{ */

/*! \brief Memo has not yet been read. */
#define MF_UNREAD	0x0001

/*! \brief Memo went to all registered nicks. */
#define MF_GLOBALSEND	0x0002

/*! \brief Memo went to all registered ircops. */
#define MF_OPERSEND	0x0004

/*! \brief Memo went to all registered services ops. */
#define MF_CSOPSEND	0x0008

/* @} */

/*!
 * \brief Data regarding the owner of a list of \link memo_ memos
 * \endlink.
 */
typedef struct {
	/*! \brief Unique MemoInfo identifier. */
	unsigned int memoinfo_id;
	
	/*! \brief Maximum number of memos this entity is allowed to own. */
	int max;
	
	/*! \brief Channel name or nickname of the owner. */
	char *owner;
} MemoInfo;

/*************************************************************************/

typedef struct channellist_ ChannelList;

typedef struct nickinfo_ NickInfo;

/*! \brief Nickname info structure. */

/*!
 * All services data is now stored in MySQL.  The NickInfo structure maps
 * directly to the structure of the nick table and is used in situations where
 * all services information about a nickname must be stored or passed between
 * functions.
 */

struct nickinfo_ {
	/*! \brief Unique identifier for each nick. */
	unsigned int nick_id;
	
	/*! \brief Nickname. */
	char nick[NICKMAX];
	
	/*! \brief Hashed password. */
	char pass[PASSMAX];
	
	/*!
	 * \brief Randomly generated salt.
	 *
	 * Used to hash the \link nickinfo_::pass password \endlink.
	 */
	char salt[SALTMAX];
	
	/*! \brief Pointer to user's web site. */
	char *url;
	
	/*! \brief Email address of user. */
	char *email;
	
	/*! \brief Last user@host that this user was seen using. */
	char *last_usermask;
	
	/*! \brief Last "realname" that this user was seen using. */
	char *last_realname;
	
	/*! \brief Last quit message sent by the user. */
	char *last_quit;
	
	/*! \brief Time of user's last quit. */
	time_t last_quit_time;
	
	/*! \brief Time that the user registered their nick. */
	time_t time_registered;
	
	/*! \brief Time that this user was last seen on IRC. */
	time_t last_seen;
	
	/*! \brief Nick status flags (NS_*). */
	int status;
	
	/*!
	 * \brief \link nickinfo_::nick_id nick_id \endlink to which this
	 * is linked.
	 *
	 * This will be 0 if this nick is the "master" nick.
	 */
	unsigned int link_id;
	
	/*!
	 * \brief The maximum number of channels that this nick is allowed
	 * to register.  \note All information from this point down is
	 * governed by links.
	 */
	unsigned int channelmax;


	/*! \brief Nick setting flags (NI_*). */
	int flags;
	
	/*! \brief Language selected by nickname owner (LANG_*). */
	unsigned int language;
	
	/*!
	 * \brief Services stamp of user who last identified for this
	 * nick.
	 */
	time_t id_stamp;
	
	/*!
	 * \brief Services stamp of user who we're expecting to REGAIN into
	 * this nick.
	 *
	 * This will be 0 if this nick has not yet been REGAINed.
	 */
	int regainid;
	
	/*!
	 * \brief Copy of the \link nickinfo_::pass password \endlink hash
	 * from the last time SENDPASS was used.
	 */
	char last_sendpass_pass[PASSMAX];
	
	/*!
	 * \brief Copy of the \link nickinfo_::salt salt \endlink from the
	 * last time SENDPASS was used.
	 */
	char last_sendpass_salt[SALTMAX];
	
	/*! \brief Time that SENDPASS was last used. */
	time_t last_sendpass_time;
};

/*!
 * \name Nickname Status Flags
 *
 * These flags typically store "online" information which the user has no
 * direct control over.
 */
/* @{ */

/*! \brief Nick may not be registered or used. */
#define NS_VERBOTEN	0x0002
				   
/*! \brief Nick never expires. */
#define NS_NO_EXPIRE	0x0004

/*! \brief User has IDENTIFY'd. */
#define NS_IDENTIFIED	0x8000

/*! \brief ON_ACCESS true && SECURE flag not set. */
#define NS_RECOGNIZED	0x4000

/*! \brief User comes from a known address. */
#define NS_ON_ACCESS	0x2000

/*! \brief Nick is being held after a kill or nick enforcement. */
#define NS_KILL_HELD	0x1000

/*!
 * \brief SVSNICK has been sent but nick has not yet changed. An enforcer
 * will be introduced when it does change.
 */
#define NS_GUESTED	0x0100

/*! \brief SVSNICK sent to REGAIN a nick but nick has not yet changed. */
#define NS_REGAINED	0x0200

/*! \brief All temporary status flags. */
#define NS_TEMPORARY	0xFF00

/* @} */

/*!
 * \name Nickname Setting Flags
 *
 * These flags typically store information about the user's preferences.
 */

/* @{ */

/*! \brief SVSNICK others who take this nick. */
#define NI_ENFORCE	0x00000001

/*! \brief Don't recognise unless IDENTIFY'd. */
#define NI_SECURE	0x00000002

/*! \brief Don't allow user to change memo limit. */
#define NI_MEMO_HARDMAX	0x00000008

/*! \brief Notify of memos at signon and un-away. */
#define NI_MEMO_SIGNON	0x00000010

/*! \brief Notify of new memos when sent. */
#define NI_MEMO_RECEIVE	0x00000020

/*! \brief Don't show in NickServ LIST to non-services-admins. */
#define NI_PRIVATE	0x00000040

/*! \brief Don't show E-mail address in NickServ INFO. */
#define NI_HIDE_EMAIL	0x00000080

/*! \brief Don't show last seen address in NickServ INFO. */
#define NI_HIDE_MASK	0x00000100

/*! \brief Don't show last quit message in NickServ INFO. */
#define NI_HIDE_QUIT	0x00000200

/*! \brief SVSNICK in 20 seconds instead of 60. */
#define NI_ENFORCEQUICK	0x00000400

/*! \brief Don't add user to channel access lists. */
#define NI_NOOP		0x00000800

/*!
 * \brief User is an oper.
 *
 * Additionally, this means that the nick will not expire and the user will
 * be shown as an oper in NickServ INFO displays.
 */
#define NI_IRCOP	0x00001000

/*! \brief Only a services admin can SETPASS this nick. */
#define NI_MARKED	0x00002000

/*! \brief Nobody can SENDPASS this nick. */
#define NI_NOSENDPASS	0x00004000

/*! \brief Activate NickServ AUTOJOIN functions. */
#define NI_AUTOJOIN	0x00008000

#define NI_CLOAK	 0x00010000

/*! \brief This nick (and its links) cannot be used. */
#define NI_SUSPENDED	0x10000000

/* @} */

/*!
 * \name Languages
 *
 * Never insert anything in the middle of this list, or everybody will start
 * getting the wrong language!  If you want to change the order the languages
 * are displayed in for NickServ HELP SET LANGUAGE, do it in language.c.
 */

/* @{ */

/*! \brief United States English. */
#define LANG_EN_US	0

/*! \brief Japanese (JIS encoding). */
#define LANG_JA_JIS	1

/*! \brief Japanese (EUC encoding). */
#define LANG_JA_EUC	2

/*! \brief Japanese (SJIS encoding). */
#define LANG_JA_SJIS	3

/*! \brief Spanish. */
#define LANG_ES		4

/*! \brief Portugese. */
#define LANG_PT		5

/*! \brief French. */
#define LANG_FR		6

/*! \brief Turkish. */
#define LANG_TR		7

/*! \brief Italian. */
#define LANG_IT		8

/*! \brief Psychotic. */
#define LANG_PSYCHO	9

/*! \brief German. */
#define LANG_DE		10

/*! \brief Danish. */
#define LANG_DK		11

/*! \brief Total number of languages. */
#define NUM_LANGS	12

/* @} */

/* Sanity-check on default language value */
#if DEF_LANGUAGE < 0 || DEF_LANGUAGE >= NUM_LANGS
# error Invalid value for DEF_LANGUAGE: must be >= 0 and < NUM_LANGS
#endif

/*************************************************************************/

/*!
 * \brief Channel access levels for users.
 *
 * All channel access data is stored in the chanaccess table in MySQL, and
 * is identified by its \link ChanAccess::channel_id channel_id \endlink.
 * \link ChanAccess::idx idx \endlink is maintained so that access entries
 * for each channel can be displayed in a sensible order.
 *
 * The channel access data is currently overloaded to store information
 * about channel memo read times also, because it is the only thing that
 * links a user to the channel (a user must have access in order to read
 * channel memos).
 */
typedef struct {
	/*! \brief Unique identifier for each channel access entry. */
	unsigned int chanaccess_id;
	
	/*! \brief Which channel this access entry belongs to. */
	unsigned int channel_id;
	
	/*!
	 * \brief Index (for ordering) of this access entry amongst the
	 * others that a particular channel has.
	 */
	unsigned int idx;
	
	/*! \brief Access level. */
	int level;
	
	/*! \brief Nick that this access relates to. */
	unsigned int nick_id;
	
	/*! \brief Time that this access was added. */
	time_t added;
	
	/*! \brief When the access was last modified. */
	time_t last_modified;
	
	/*! \brief When the access was last used. */
	time_t last_used;
	
	/*! \brief When they last read the channel's memos. */
	time_t memo_read;
	
	/*! \brief Nickname of the user who added this access entry. */
	char added_by[NICKMAX];
	
	/*!
	 * \brief Nickname of the user who last modified this access
	 * entry.
	 */
	char modified_by[NICKMAX];
} ChanAccess;

/*!
 * \name Channel Access Limits
 *
 * Note that these two levels also serve as exclusive boundaries for valid
 * access levels.  ACCESS_FOUNDER may be assumed to be strictly greater than
 * any valid access level, and ACCESS_INVALID may be assumed to be strictly
 * less than any valid access level.
 */

/* @{ */

/*! \brief Numeric level indicating founder access. */
#define ACCESS_FOUNDER	10000

/*! \brief Used in levels[] for disabled settings. */
#define ACCESS_INVALID	-10000

/* @} */

/*!
 * \brief AutoKick data.
 *
 * All AutoKick data is stored in the akick table in MySQL.
 */
typedef struct {
	/*! \brief Unique identifier for each AutoKick. */
	unsigned int akick_id;
	
	/*! \brief Channel to which this AutoKick belongs. */
	unsigned int channel_id;
	
	/*! \brief Index used to order each channel's AutoKicks. */
	unsigned int idx;
	
	/*!
	 * \brief Hostmask to be affected by the AutoKick.
	 *
	 * If the AutoKick is placed on a hostmask then \link
	 * AutoKick::nick_id nick_id \endlink will be zero.  Otherwise, if
	 * the AutoKick is for a specific registered nick, then this member
	 * will be NULL.
	 */
	char *mask;
	
	/*!
	 * \brief ID of the nick to be affected by this AutoKick.
	 *
	 * If the AutoKick is placed on a specific registered nick then its
	 * \link NickInfo::nick_id nick_id \endlink will be stored here.
	 * Otherwise, this member will be 0.
	 */
	unsigned int nick_id;
	
	/*! \brief Reason for the AutoKick. */
	char *reason;
	
	/*! \brief Nickname of the user who added this AutoKick. */
	char who[NICKMAX];
	
	/*! \brief Time that this AutoKick was added. */
	time_t added;
	
	/*! \brief Time that this AutoKick was last matched. */
	time_t last_used;
} AutoKick;

typedef struct chaninfo_ ChannelInfo;

/*! \brief Channel data structure. */
struct chaninfo_ {
	/*! \brief Unique channel identifier. */
	unsigned int channel_id;
	
	/*! \brief Name of the channel including the leading #. */
	char name[CHANMAX];
	
	/*!
	 * \brief \link NickInfo::nick_id nick_id \endlink of the channel
	 * founder.
	 */
	unsigned int founder;
	
	/*!
	 * \brief \link NickInfo::nick_id nick_id \endlink of the user who
	 * becomes channel founder if/when the channel expires or the
	 * founder's nick is dropped.
	 */
	unsigned int successor;
	
	/*! \brief Hashed password for founder access to the channel. */
	char founderpass[PASSMAX];
	
	/*!
	 * \brief Salt used to encrypt the \link ChannelInfo::founderpass
	 * founder password \endlink.
	 */
	char salt[SALTMAX];
	
	/*! \brief Description of the channel. */
	char *desc;
	
	/*! \brief Pointer to the channel's web site. */
	char *url;
	
	/*!
	 * \brief Contact E-mail address for the channel.
	 *
	 * \note This address is \em not used for ChanServ SENDPASS; the E-mail
	 * address of the founder's nick is, instead.
	 */
	char *email;

	/*! \brief Time that this channel was registered. */
	time_t time_registered;
	
	/*!
	 * \brief Time that this channel was last used.
	 *
	 * This time is updated anytime the founder joins, or any channel
	 * access entry is used.
	 */
	time_t last_used;
	
	/*!
	 * \brief Time that the channel's founder last read channel
	 * memos.
	 */
	time_t founder_memo_read;
	
	/*! \brief Text of the channel's latest topic. */
	char *last_topic;
	
	/*! \brief Nickname of the last user to set the topic. */
	char last_topic_setter[NICKMAX];
	
	/*! \brief Time that the channel's topic was last set. */
	time_t last_topic_time;

	/*! \brief Flags for channel settings (CI_*). */
	int flags;

	/*! \brief Mask of channel modes locked on. */
	int mlock_on;
	
	/*! \brief Mask of channel modes locked off. */
	int mlock_off;
	
	/*!
	 * \brief Locked channel limit.
	 *
	 * Will be 0 if no limit is locked.
	 */
	unsigned int mlock_limit;
	
	/*!
	 * \brief Locked channel key.
	 *
	 * Will be NULL if no key is locked.
	 */
	char *mlock_key;

	/*! \brief Notice sent upon entering channel. */
	char *entry_message;

	/*! \brief Time that the channel limit was last set by AutoLimit. */
	time_t last_limit_time;
	
	/*! \brief How much AutoLimit should alter the limit by. */
	unsigned int limit_offset;
	
	/*!
	 * \brief Tolerance before AutoLimit will change the channel
	 * limit.
	 */
	unsigned int limit_tolerance;
	
	/*!
	 * \brief How long to wait in minutes before AutoLimit should check
	 * if the channel limit needs updating.
	 */
	unsigned int limit_period;

	/*!
	 * \brief Time in minutes that ClearBans should allow bans to be in
	 * place for.
	 *
	 * Will be 0 if bans should never be expired.
	 */
	unsigned int bantime;
	
	/*!
	 * \brief Copy of the \link ChannelInfo::founderpass password hash
	 * \endlink from the last time ChanServ SENDPASS was used.
	 */
	char last_sendpass_pass[PASSMAX];
	
	/*!
	 * \brief Copy of the \link ChannelInfo::salt salt \endlink from
	 * the last time ChanServ SENDPASS was used.
	 */
	char last_sendpass_salt[SALTMAX];
	
	/*! \brief Time that ChanServ SENDPASS was last used. */
	time_t last_sendpass_time;

	int floodserv;
};

/*!
 * \name Channel Settings Flags
 *
 * These flags typically record channel settings that users can specify via
 * the ChanServ SET commands.
 */

/* @{ */

/*! \brief Retain topic even after last person leaves channel. */
#define CI_KEEPTOPIC	0x00000001

/*! \brief Don't allow non-authorised users to be opped. */
#define CI_SECUREOPS	0x00000002

/*! \brief Hide channel from ChanServ LIST command. */
#define CI_PRIVATE	0x00000004

/*! \brief Topic can only be changed by ChanServ SET TOPIC. */
#define CI_TOPICLOCK	0x00000008

/*! \brief Those not allowed ops are kickbanned. */
#define CI_RESTRICTED	0x00000010

/*! \brief Don't auto-deop anyone. */
#define CI_LEAVEOPS	0x00000020

/*!
 * \brief Don't allow any privileges unless a user is IDENTIFY'd with
 * NickServ.
 */
#define CI_SECURE	0x00000040

/*! \brief Don't allow the channel to be registered or used. */
#define CI_VERBOTEN	0x00000080

/*! \brief Channel does not expire. */
#define CI_NO_EXPIRE	0x00000200

/*! \brief Channel memo limit may not be changed. */
#define CI_MEMO_HARDMAX	0x00000400

/*! \brief Send notice to channel ops on use of certain commands. */
#define CI_VERBOSE	0x00000800

/* @} */

/*!
 * \name Channel Capabilities
 *
 * Each of these denotes a specific set of capabilities that channel founders
 * can assign access levels to, therefore controlling who on their access list
 * can do what.
 */

/* @{ */

/*! \brief Use of the ChanServ INVITE command. */
#define CA_INVITE		0

/*! \brief Use of the ChanServ AKICK command. */
#define CA_AKICK		1

/*!
 * \brief Use of the ChanServ SET command.
 *
 * \note This does not include ChanServ SET FOUNDER or
 * ChanServ SET PASSWORD.
 */
#define CA_SET			2

/*! \brief Use of the ChanServ UNBAN command. */
#define CA_UNBAN		3

/*! \brief Level required to be auto-opped by ChanServ. */
#define CA_AUTOOP		4

/*!
 * \brief Level required to be auto-deopped by ChanServ.
 *
 * \note This is a \em maximum, \em not a minimum.
 */
#define CA_AUTODEOP		5

/*! \brief  Level required to be auto-voiced by ChanServ. */
#define CA_AUTOVOICE		6

/*! \brief Use of ChanServ OP and ChanServ DEOP. */
#define CA_OPDEOP		7

/*! \brief Use of ChanServ ACCESS LIST. */
#define CA_ACCESS_LIST		8

/*! \brief Use of ChanServ CLEAR. */
#define CA_CLEAR		9

/*!
 * \brief Level required to be allowed to join the channel.
 *
 * \note This is a \em maximum, \em not a minimum.
 */
#define CA_NOJOIN		10

/*! \brief Level required to \em change access entries on the channel. */
#define CA_ACCESS_CHANGE	 11

/*! \brief Level required to read channel memos. */
#define CA_MEMO_READ	12

/*! \brief Level required to view the current level settings. */
#define CA_LEVEL_LIST	13

/*! \brief Level required to \em change the level settings. */
#define CA_LEVEL_CHANGE	14

/*! \brief Use of the ChanServ SYNC command. */
#define CA_SYNC		15

/*! \brief Level required to send channel memos. */
#define CA_MEMO_SEND	16

/*! \brief Level required to view the AutoKick list. */
#define CA_AKICK_LIST	17

/*! \brief Total number of channel capabilities. */
#define CA_SIZE		18

/* @} */

/*************************************************************************/

/* Suspension info structures. */

typedef struct suspendinfo_ SuspendInfo;

/*!
 * \brief Suspension info structure.
 *
 * Since both nicks and channels can be suspended, this structure holds the
 * generic suspension details that apply to both.
 */
struct suspendinfo_ {
	/*! \brief Unique identifier for this suspend. */
	unsigned int suspend_id;
	
	/*! \brief Nick of the person who set this suspension. */
	char who[NICKMAX];
	
	/*! \brief Reason for suspension. */
	char *reason;
	
	/*! \brief Time that entity was suspended. */
	time_t suspended;
	
	/*!
	 * \brief When the suspension will expire.
	 *
	 * 0 indicates a permanent suspension.
	 */
	time_t expires;		/* 0 for no expiry */
};

typedef struct suspendednick_ NickSuspend;

/*! \brief Suspended nick info. */
struct suspendednick_ {
	/*! \brief Unique identifier for this nick suspension. */
	unsigned int nicksuspend_id;
	
	/*!
	 * \brief \link NickInfo::nick_id nick_id \endlink of the nick
	 * which is being suspended.
	 */
	unsigned int nick_id;
	
	/*!
	 * \brief \link SuspendInfo::suspend_id suspend_id \endlink which
	 * identifies the corresponding SuspendInfo structure.
	 */
	unsigned int suspend_id;
};

typedef struct suspendedchan_ ChanSuspend;

/*! \brief Suspended channel info. */
struct suspendedchan_ {
	/*! \brief Unique identifier for this channel suspension. */
	unsigned int chansuspend_id;
	
	/*!
	 * \brief \link ChannelInfo::channel_id channel_id \endlink of the
	 * channel which is being suspended.
	 */
	unsigned int channel_id;
	
	/*!
	 * \brief \link SuspendInfo::suspend_id suspend_id \endlink which
	 * identifies the corresponding SuspendInfo structure.
	 */
	unsigned int suspend_id;
};

/*************************************************************************/

typedef struct server_ Server;
typedef struct serverstats_ ServerStats;

/*! \brief Server information. */
struct server_ {
	/*! \brief Used to navigate the entire server list. */
	Server *next;
	
	/*! \brief Used to navigate the entire server list. */
	Server *prev;
	
	/*!
	 * \brief Hub of this server.
	 *
	 * Will be NULL if this is \em our hub.
	 */
	Server *hub;
	
	/*! \brief First leaf. */
	Server *child;
	
	/*! \brief List of other leaves. */
	Server *sibling;

	/*! \brief This server's name. */
	char *name;
	
	/*!
	 * \brief Time server linked.
	 *
	 * This will be 0 if the server is not currently linked.
	 */
	time_t t_join;
};

/*************************************************************************/

typedef struct user_ User;
typedef struct channel_ Channel;
typedef struct chanban_ Ban;

/*! \brief Channel ban info. */
struct chanban_ {
	/*! \brief nick!user@host mask that is banned. */
	char *mask;

	/*! \brief Time this ban was placed. */
	time_t time;
};

/*! \brief Online user info. */
struct user_ {
	/*! \brief Link to navigate entire user list. */
	User *next;
	
	/*! \brief Link to navigate entire user list. */
	User *prev;
	
	/*! \brief User's nickname. */
	char nick[NICKMAX];
	
	/*!
	 * \brief \link NickInfo::nick_id nick_id \endlink of user's main
	 * registered nick.
	 *
	 * This will always point to the "master" nick, even if they are
	 * currently using a linked nick.
	 */
	unsigned int nick_id;
	
	/*!
	 * \brief \link NickInfo::nick_id nick_id \endlink of the actual
	 * registered nick in use.
	 *
	 * This may be the \link NickInfo::nick_id nick_id \endlink of a
	 * linked nick, not the "master".
	 */
	unsigned int real_id;
	
	/*!
	 * \brief Pointer to \link server_ Server structure \endlink for
	 * the server that the user is on.
	 */
	Server *server;
	
	/*! \brief Username part of the hostmask. */
	char *username;
	
	/*! \brief Host part of the hostmask. */
	char *host;
	
	/*! \brief User's IP address. */
	struct in_addr ip;
	
	/*! \brief User's IP as a dotquad string. */
	char *ipstring;
	
	/*! \brief User's "realname". */
	char *realname;

	/*!
	 * \brief Random ID generated by the IRC server that the user
	 * connects to.
	 *
	 * This can be used in split recovery, as long as the user stays
	 * connected to their ircd, we can recognise them again and don't have
	 * to ask them to identify to services all over again.
	 */
	uint32 services_stamp;
	
	/*!
	 * \brief Timestamp sent with nick when it was introduced to us.
	 *
	 * \note This never changes!
	 */
	time_t signon;
	
	/*!
	 * \brief Time that \em we saw this user with thier current
	 * nickname.
	 */
	time_t my_signon;
	
	/*! \brief Our own unique ID for this user. */
	uint32 servicestamp;

	/*! \brief Bitmask of usermodes set (UMODE_*). */
	int32 mode;
	
	/*! \brief Channels this user has joined. */
	struct u_chanlist {
		/*! \brief Links to navigate channel list. */
		struct u_chanlist *next;

		/*! \brief Links to navigate channel list. */
		struct u_chanlist *prev;
		
		/*!
		 * \brief Pointer to Channel structure for this
		 * channel.
		 */
		Channel *chan;
	} *chans;
	
	/*!
	 * \brief Channels for which this user has identified as
	 * founder.
	 */
	struct u_chanidlist {
		/*! \brief Links to navigate channel list. */
		struct u_chanidlist *next;
		
		/*! \brief Links to navigate channel list. */
		struct u_chanidlist *prev;
		
		/*!
		 * \brief \link ChannelInfo::channel_id channel_id \endlink
		 * of the registered channel which has been identified for.
		 */
		unsigned int channel_id;
	} *founder_chans;
	
	/*! \brief Number of invalid password attempts. */
	short invalid_pw_count;
	
	/*! \brief Time of last invalid password. */
	time_t invalid_pw_time;
	
	/*! \brief Last time MemoServ SEND command was used. */
	time_t lastmemosend;
	
	/*! \brief Last time NickServ REGISTER command was used. */
	time_t lastnickreg;
};

/*! \name User Modes */

/* @{ */

/*! \brief Oper. */
#define UMODE_o 0x00000001

/*! \brief Invisible. */
#define UMODE_i 0x00000002

/*! \brief See server notices. */
#define UMODE_s 0x00000004

/*! \brief See WALLOPS. */
#define UMODE_w 0x00000008

/*! \brief See GLOBOPS. */
#define UMODE_g 0x00000010

/*! \brief Is HelpOp. */
#define UMODE_h 0x00000020

/*! \brief Is Services Admin. */
#define UMODE_a 0x00000040

/*! \brief Is identified to registered nick. */
#define UMODE_r 0x00000080

/* @} */

/*! \brief Online channel data. */
struct channel_ {
	/*! \brief Link to traverse entire list of channels. */
	Channel *next;
	
	/*! \brief Link to traverse entire list of channels. */
	Channel *prev;
	
	/*! \brief Name of channel. */
	char name[CHANMAX];
	
	/*!
	 * \brief Corresponding \link ChannelInfo::channel_id channel_id
	 * \endlink.
	 */
	unsigned int channel_id;
	
	/*! \brief Time that the channel was created. */
	time_t creation_time;

	/*! \brief Current channel topic. */
	char *topic;
	
	/*! \brief Nick of the user who set the topic. */
	char topic_setter[NICKMAX];
	
	/*! \brief Time that the topic was set. */
	time_t topic_time;

	/*! \brief Bitmask of binary channel modes (CMODE_*). */
	int32 mode;
	
	/*!
	 * \brief Current channel limit.
	 *
	 * This will be 0 if no limit is currently set.
	 */
	unsigned int limit;
	
	/*!
	 * \brief Current channel key.
	 *
	 * This will be NULL if no key is currently set.
	 */
	char *key;

	/*! \brief Number of channel bans. */
	size_t bancount;

	/*! \brief Size of the current ban array. */
	size_t bansize;
	
	/*! \brief Pointer to start of our ban array. */
	Ban **newbans;

	/*! \brief All users, chanops and voices in the channel. */
	struct c_userlist {
		/*! \brief Pointers to navigate the user list. */
		struct c_userlist *next, *prev;
		/*!
		 * \brief Pointer to \link user_ User structure \endlink
		 * for this user.
		 */
		User *user;
	} *users, *chanops, *voices;

	/*! \brief Time of last server MODE. */
	time_t server_modetime;
	
	/*!
	 * \brief Time of last check_modes().
	 *
	 * If the time has changed past 1 second, the \link
	 * channel_::server_modecount server_modecount \endlink can be
	 * zeroed.
	 */
	time_t chanserv_modetime;
	
	/*! \brief Number of server MODEs this second. */
	int16 server_modecount;
	
	/*! \brief Number of check_mode()'s this second. */
	int16 chanserv_modecount;
	
	/*! \brief Did we fail to set a mode? */
	int16 bouncy_modes;

	/*! \brief Time of last AutoLimit set. */
	time_t last_limit_time;
	
	/*! \brief How much AutoLimit should alter the channel limit by. */
	unsigned int limit_offset;
	
	/*!
	 * \brief Tolerance before AutoLimit will change the channel
	 * limit.
	 */
	unsigned int limit_tolerance;
	
	/*!
	 * \brief Period in minutes between Autolimit checks to see if the
	 * channel limit needs updating.
	 */
	unsigned int limit_period;
	
	/*!
	 * \brief Time in minutes to let bans last for when ClearBans is
	 * set.
	 *
	 * This will be 0 if ClearBans is turned off.
	 */
	unsigned int bantime;

	int floodserv;
};

/*!
 * \name Channel Modes
 *
 * Only the binary channel modes (i.e. on/off, no parameters) are defined
 * here.
 */

/* @{ */

/*! \brief Channel is invite-only. */
#define CMODE_i 0x00000001

/*! \brief Channel is moderated. */
#define CMODE_m 0x00000002

/*! \brief No messages from outside the channel. */
#define CMODE_n 0x00000004

/*! \brief Channel is private. */
#define CMODE_p 0x00000008

/*! \brief Channel is secret. */
#define CMODE_s 0x00000010

/*! \brief Only chanops may change the topic. */
#define CMODE_t 0x00000020

/*!
 * \brief Channel key is set.
 *
 * This is only used by ChanServ internally for MLOCK.
 */
#define CMODE_k 0x00000040

/*!
 * \brief Channel limit is set.
 *
 * This is only used by ChanServ internally for MLOCK.
 */
#define CMODE_l 0x00000080

/*!
 * \brief Only users who are identified to a registered nick may join the
 * channel.
 */
#define CMODE_R 0x00000100

/*! \brief No mIRC/ANSI colours in the channel. */
#define CMODE_c 0x00000400

/*! \brief Only Opers may join the channel. */
#define CMODE_O 0x00000800

/*!
 * \brief Only users who are identified to a registered nick may speak in
 * the channel.
 */
#define CMODE_M 0x00001000	/* Only identified users can speak */

/* @} */

/*! \brief Who sends channel MODE (and KICK) commands? */
#define MODE_SENDER(service) service

/*! \brief Maximum number of channels we can record. */
#define MAX_CHANNELCOUNT 65535

/*************************************************************************/

typedef struct minmax_ MinMax;

/*! \brief Used for recording statistical information. */
struct minmax_ {
	/*! \brief min/max values. */
	int min, max;

	/*! \brief Time that the min or max was recorded. */
	time_t mintime, maxtime;
};

/*************************************************************************/

/*! \name News Types */

/* @{ */

/*! \brief General logon news shown to all clients. */
#define NEWS_LOGON	0

/*! \brief Oper news - shown after a client opers up. */
#define NEWS_OPER	1

/* @} */

/*************************************************************************/

/*! \brief Ignorance list data. */
typedef struct ignore_data {
	/*! \brief Pointer to navigate list. */
	struct ignore_data *next;

	/*! \brief Nick of user that is being ignored. */
	char who[NICKMAX];
	
	/*! \brief When do we stop ignoring them? */
	time_t time;
} IgnoreData;

/*************************************************************************/

/*! \name AKILL Failure Reasons */

/* @{ */

/*! \brief Hostmask is already AutoKilled. */
#define AKILL_PRESENT 1

/*! \brief We're already storing the maximum amount of AutoKills. */
#define TOO_MANY_AKILLS 2

/* @} */

/*************************************************************************/

/*!
 * \brief SHA1 data.
 *
 * I have no idea how this works as I just ripped it out of gnupg
 * (http://www.gnupg.org) <tt>:-)</tt>
 */
typedef struct {
	uint32 h0, h1, h2, h3, h4;
	uint32 nblocks;
	unsigned char buf[64];
	int count;
} SHA1_CONTEXT;

/*************************************************************************/

#include "extern.h"

/*************************************************************************/

#endif				/* SERVICES_H */
