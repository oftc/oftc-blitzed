
/*
 * Configuration file handling.
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
 */

#include "services.h"

RCSID("$Id$");

/*************************************************************************/

void error(int linenum, char *message, ...);
int parse(char *buf, int linenum);

/* Configurable variables: */

char *RemoteServer;
int RemotePort;
char *RemotePassword;
char *LocalHost;
int LocalPort;

char *ServerName;
char *ServerDesc;
char *ServiceUser;
char *ServiceHost;
static char *temp_userhost;

char *s_NickServ;
char *s_ChanServ;
char *s_MemoServ;
char *s_HelpServ;
char *s_OperServ;
char *s_GlobalNoticer;
char *s_IrcIIHelp;
char *s_DevNull;
char *s_FloodServ;
char *desc_NickServ;
char *desc_ChanServ;
char *desc_MemoServ;
char *desc_HelpServ;
char *desc_OperServ;
char *desc_StatServ;
char *desc_GlobalNoticer;
char *desc_IrcIIHelp;
char *desc_DevNull;
char *desc_FloodServ;

char *PIDFilename;
char *MOTDFilename;
char *HelpDir;

char *mysqlHost;
char *mysqlUser;
char *mysqlPass;
char *mysqlDB;
unsigned int mysqlPort;
char *mysqlSocket;

int NoSplitRecovery;
int BadPassLimit;
int BadPassTimeout;
int ExpireTimeout;
int AuthExpireTimeout;
int ReadTimeout;
int WarningTimeout;
int TimeoutCheck;
char *SnoopChan;
char *SendmailPath;
char *AbuseEmail;
char *NetworkName;
int StrictPassWarn;
int StrictPassReject;
char *DefaultCloak;

static int NSDefNone;
char *NSGuestNickPrefix;
int NSDefEnforce;
int NSDefEnforceQuick;
int NSDefSecure;
int NSDefPrivate;
int NSDefHideEmail;
int NSDefHideUsermask;
int NSDefHideQuit;
int NSDefMemoSignon;
int NSDefMemoReceive;
int NSDefCloak;
int NSDefAutoJoin;
int NSRegDelay;
int NSExpire;
unsigned int NSAccessMax;
unsigned int NSAutoJoinMax;
char *NSEnforcerUser;
char *NSEnforcerHost;
static char *temp_nsuserhost;
int NSReleaseTimeout;
int NSDisableLinkCommand;
int NSListOpersOnly;
int NSListMax;
int NSSecureAdmins;
int NSSuspendExpire;
int NSSuspendGrace;
char *NSNickURL;
char *NSMyEmail;
int NSWallReg;
char *NSRegExtraInfo;
int NSRegNeedAuth;
int NSExtraGrace;

int CSMaxReg;
int CSExpire;
unsigned int CSAccessMax;
int CSAutokickMax;
char *CSAutokickReason;
int CSInhabit;
int CSRestrictDelay;
int CSListOpersOnly;
int CSListMax;
int CSSuspendExpire;
int CSSuspendGrace;
int CSOpOper;
int CSAkickOper;
char *CSMyEmail;
int CSWallReg;
int CSExtraGrace;

int MSMaxMemos;
int MSSendDelay;
int MSNotifyAll;
int MSROT13;

char *ServicesRoot;
int LogMaxUsers;
char *StaticAkillReason;
int ImmediatelySendAkill;
int AutokillExpiry;
int AkillWildThresh;
int WallOper;
int WallBadOS;
int WallOSMode;
int WallOSClearmodes;
int WallOSKick;
int WallOSAkill;
int WallOSException;
int WallAkillExpire;
int WallExceptionExpire;
int WallGetpass;
int WallSetpass;

int LimitSessions;
int DefSessionLimit;
int ExceptionExpiry;
unsigned int MaxSessionLimit;
int SessionAkillExpiry;
unsigned int SessionKillClues;
char *SessionLimitExceeded;
char *SessionLimitDetailsLoc;

int FloodServAkillExpiry;
char *FloodServAkillReason;
int FloodServTFNumLines;
int FloodServTFSec;
int FloodServTFNLWarned;
int FloodServTFSecWarned;
int FloodServWarnFirst;
char *FloodServWarnMsg;

/*************************************************************************/

/* Deprecated directive (dep_) and value checking (chk_) functions: */

static void dep_ListOpersOnly(void)
{
	NSListOpersOnly = 1;
	CSListOpersOnly = 1;
}

/*************************************************************************/

#define MAXPARAMS	8

/* Configuration directives. */

typedef struct {
	char *name;
	struct {
		int type;	/* PARAM_* below */
		int flags;	/* Same */
		void *ptr;	/* Pointer to where to store the value */
	} params[MAXPARAMS];
} Directive;

#define PARAM_NONE	0
#define PARAM_INT	1
#define PARAM_POSINT	2	/* Positive integer only */
#define PARAM_PORT	3	/* 1..65535 only */
#define PARAM_STRING	4
#define PARAM_TIME	5
#define PARAM_SET	-1	/* Not a real parameter; just set the
				 *    given integer variable to 1 */
#define PARAM_DEPRECATED -2	/* Set for deprecated directives; `ptr'
				 *    is a function pointer to call */

/* Flags: */
#define PARAM_OPTIONAL	0x01
#define PARAM_FULLONLY	0x02	/* Directive only allowed if !STREAMLINED */

Directive directives[] = {
	{"AbuseEmail", {{PARAM_STRING, 0, &AbuseEmail}}},
	{"AkillWildThresh", {{PARAM_POSINT, 0, &AkillWildThresh}}},
	{"AuthExpireTimeout", {{PARAM_TIME, 0, &AuthExpireTimeout}}},
	{"AutokillExpiry", {{PARAM_TIME, 0, &AutokillExpiry}}},
	{"BadPassLimit", {{PARAM_POSINT, 0, &BadPassLimit}}},
	{"BadPassTimeout", {{PARAM_TIME, 0, &BadPassTimeout}}},
	{"ChanServName", {{PARAM_STRING, 0, &s_ChanServ},
			  {PARAM_STRING, 0, &desc_ChanServ}}},
	{"CSAccessMax", {{PARAM_POSINT, 0, &CSAccessMax}}},
	{"CSAutokickMax", {{PARAM_POSINT, 0, &CSAutokickMax}}},
	{"CSAutokickReason", {{PARAM_STRING, 0, &CSAutokickReason}}},
	{"CSExpire", {{PARAM_TIME, 0, &CSExpire}}},
	{"CSExtraGrace", {{PARAM_SET, 0, &CSExtraGrace}}},
	{"CSInhabit", {{PARAM_TIME, 0, &CSInhabit}}},
	{"CSListMax", {{PARAM_POSINT, 0, &CSListMax}}},
	{"CSListOpersOnly", {{PARAM_SET, 0, &CSListOpersOnly}}},
	{"CSMaxReg", {{PARAM_POSINT, 0, &CSMaxReg}}},
	{"CSMyEmail", {{PARAM_STRING, 0, &CSMyEmail}}},
	{"CSOpOper", {{PARAM_SET, 0, &CSOpOper}}},
	{"CSAkickOper", {{PARAM_SET, 0, &CSAkickOper}}},
	{"CSRestrictDelay", {{PARAM_TIME, 0, &CSRestrictDelay}}},
	{"CSSuspendExpire", {{PARAM_TIME, 0, &CSSuspendExpire}}},
	{"CSSuspendGrace", {{PARAM_TIME, 0, &CSSuspendGrace}}},
	{"CSWallReg", {{PARAM_SET, 0, &CSWallReg}}},
        {"DefaultCloak", {{PARAM_STRING, 0, &DefaultCloak}}},
	{"DefSessionLimit", {{PARAM_INT, 0, &DefSessionLimit}}},
	{"DevNullName", {{PARAM_STRING, 0, &s_DevNull},
			 {PARAM_STRING, 0, &desc_DevNull}}},
	{"ExceptionExpiry", {{PARAM_TIME, 0, &ExceptionExpiry}}},
	{"ExpireTimeout", {{PARAM_TIME, 0, &ExpireTimeout}}},
    {"FloodServAkillExpiry", {{PARAM_TIME, 0, &FloodServAkillExpiry }}},
    {"FloodServAkillReason", {{PARAM_STRING, 0, &FloodServAkillReason }}},
    {"FloodServName", {{PARAM_STRING, 0, &s_FloodServ },
			{PARAM_STRING, 0, &desc_FloodServ}}},
    {"FloodServTFNumLines", {{PARAM_INT, 0, &FloodServTFNumLines }}},
    {"FloodServTFSec", {{PARAM_INT, 0, &FloodServTFSec }}},
    {"FloodServTFNLWarned", {{PARAM_INT, 0, &FloodServTFNLWarned }}},
    {"FloodServTFSecWarned", {{PARAM_INT, 0, &FloodServTFSecWarned }}},
    {"FloodServWarnFirst", {{PARAM_SET, 0, &FloodServWarnFirst }}},
    {"FloodServWarnMsg", {{PARAM_STRING, 0, &FloodServWarnMsg }}},
	{"GlobalName", {{PARAM_STRING, 0, &s_GlobalNoticer},
			{PARAM_STRING, 0, &desc_GlobalNoticer}}},
	{"HelpDir", {{PARAM_STRING, 0, &HelpDir}}},
	{"HelpServName", {{PARAM_STRING, 0, &s_HelpServ},
			  {PARAM_STRING, 0, &desc_HelpServ}}},
	{"ImmediatelySendAkill", {{PARAM_SET, 0, &ImmediatelySendAkill}}},
	{"IrcIIHelpName", {{PARAM_STRING, 0, &s_IrcIIHelp},
			   {PARAM_STRING, 0, &desc_IrcIIHelp}}},
	{"LimitSessions", {{PARAM_SET, PARAM_FULLONLY, &LimitSessions}}},
	{"ListOpersOnly", {{PARAM_DEPRECATED, 0, dep_ListOpersOnly}}},
	{"LocalAddress", {{PARAM_STRING, 0, &LocalHost},
			  {PARAM_PORT, PARAM_OPTIONAL, &LocalPort}}},
	{"LogMaxUsers", {{PARAM_SET, 0, &LogMaxUsers}}},
	{"MaxSessionLimit", {{PARAM_POSINT, 0, &MaxSessionLimit}}},
	{"MemoServName", {{PARAM_STRING, 0, &s_MemoServ},
			  {PARAM_STRING, 0, &desc_MemoServ}}},
	{"MOTDFile", {{PARAM_STRING, 0, &MOTDFilename}}},
	{"MySQLHost", {{PARAM_STRING, 0, &mysqlHost}}},
	{"MySQLUser", {{PARAM_STRING, 0, &mysqlUser}}},
	{"MySQLPass", {{PARAM_STRING, 0, &mysqlPass}}},
	{"MySQLDB", {{PARAM_STRING, 0, &mysqlDB}}},
	{"MySQLPort", {{PARAM_POSINT, 0, &mysqlPort}}},
	{"MySQLSocket", {{PARAM_STRING, 0, &mysqlSocket}}},
	{"MSMaxMemos", {{PARAM_POSINT, 0, &MSMaxMemos}}},
	{"MSNotifyAll", {{PARAM_SET, 0, &MSNotifyAll}}},
	{"MSROT13", {{PARAM_SET, 0, &MSROT13}}},
	{"MSSendDelay", {{PARAM_TIME, 0, &MSSendDelay}}},
	{"NetworkName", {{PARAM_STRING, 0, &NetworkName}}},
	{"NickServName", {{PARAM_STRING, 0, &s_NickServ},
			  {PARAM_STRING, 0, &desc_NickServ}}},
	{"NoSplitRecovery", {{PARAM_SET, 0, &NoSplitRecovery}}},
	{"NSAccessMax", {{PARAM_POSINT, 0, &NSAccessMax}}},
	{"NSAutoJoinMax", {{PARAM_POSINT, 0, &NSAutoJoinMax}}},
	{"NSDefHideEmail", {{PARAM_SET, 0, &NSDefHideEmail}}},
	{"NSDefHideQuit", {{PARAM_SET, 0, &NSDefHideQuit}}},
	{"NSDefHideUsermask", {{PARAM_SET, 0, &NSDefHideUsermask}}},
	{"NSDefEnforce", {{PARAM_SET, 0, &NSDefEnforce}}},
	{"NSDefEnforceQuick", {{PARAM_SET, 0, &NSDefEnforceQuick}}},
	{"NSDefMemoReceive", {{PARAM_SET, 0, &NSDefMemoReceive}}},
	{"NSDefMemoSignon", {{PARAM_SET, 0, &NSDefMemoSignon}}},
	{"NSDefNone", {{PARAM_SET, 0, &NSDefNone}}},
	{"NSDefPrivate", {{PARAM_SET, 0, &NSDefPrivate}}},
	{"NSDefSecure", {{PARAM_SET, 0, &NSDefSecure}}},
	{"NSDefAutoJoin", {{PARAM_SET, 0, &NSDefAutoJoin}}},
	{"NSDisableLinkCommand", {{PARAM_SET, 0, &NSDisableLinkCommand}}},
	{"NSEnforcerUser", {{PARAM_STRING, 0, &temp_nsuserhost}}},
	{"NSExpire", {{PARAM_TIME, 0, &NSExpire}}},
	{"NSExtraGrace", {{PARAM_SET, 0, &NSExtraGrace}}},
	{"NSGuestNickPrefix", {{PARAM_STRING, 0, &NSGuestNickPrefix}}},
	{"NSListMax", {{PARAM_POSINT, 0, &NSListMax}}},
	{"NSListOpersOnly", {{PARAM_SET, 0, &NSListOpersOnly}}},
	{"NSNickURL", {{PARAM_STRING, 0, &NSNickURL}}},
	{"NSMyEmail", {{PARAM_STRING, 0, &NSMyEmail}}},
	{"NSRegDelay", {{PARAM_TIME, 0, &NSRegDelay}}},
	{"NSRegExtraInfo", {{PARAM_STRING, 0, &NSRegExtraInfo}}},
	{"NSRegNeedAuth", {{PARAM_SET, 0, &NSRegNeedAuth}}},
	{"NSReleaseTimeout", {{PARAM_TIME, 0, &NSReleaseTimeout}}},
	{"NSSecureAdmins", {{PARAM_SET, 0, &NSSecureAdmins}}},
	{"NSSuspendExpire", {{PARAM_TIME, 0, &NSSuspendExpire}}},
	{"NSSuspendGrace", {{PARAM_TIME, 0, &NSSuspendGrace}}},
	{"NSWallReg", {{PARAM_SET, 0, &NSWallReg}}},
	{"OperServName", {{PARAM_STRING, 0, &s_OperServ},
			  {PARAM_STRING, 0, &desc_OperServ}}},
	{"PIDFile", {{PARAM_STRING, 0, &PIDFilename}}},
	{"ReadTimeout", {{PARAM_TIME, 0, &ReadTimeout}}},
	{"RemoteServer", {{PARAM_STRING, 0, &RemoteServer},
			  {PARAM_PORT, 0, &RemotePort},
			  {PARAM_STRING, 0, &RemotePassword}}},
	{"ServerDesc", {{PARAM_STRING, 0, &ServerDesc}}},
	{"ServerName", {{PARAM_STRING, 0, &ServerName}}},
	{"ServicesRoot", {{PARAM_STRING, 0, &ServicesRoot}}},
	{"ServiceUser", {{PARAM_STRING, 0, &temp_userhost}}},
	{"SessionAkillExpiry", {{PARAM_TIME, 0, &SessionAkillExpiry}}},
	{"SessionKillClues", {{PARAM_POSINT, 0, &SessionKillClues}}},
	{"SessionLimitDetailsLoc",
	 {{PARAM_STRING, 0, &SessionLimitDetailsLoc}}},
	{"SessionLimitExceeded", {{PARAM_STRING, 0, &SessionLimitExceeded}}},
	{"SendmailPath", {{PARAM_STRING, 0, &SendmailPath}}},
	{"SnoopChan", {{PARAM_STRING, 0, &SnoopChan}}},
	{"StaticAkillReason", {{PARAM_STRING, 0, &StaticAkillReason}}},
	{"StrictPassReject", {{PARAM_SET, 0, &StrictPassReject}}},
	{"StrictPassWarn", {{PARAM_SET, 0, &StrictPassWarn}}},
	{"TimeoutCheck", {{PARAM_TIME, 0, &TimeoutCheck}}},
	{"WallAkillExpire", {{PARAM_SET, 0, &WallAkillExpire}}},
	{"WallExceptionExpire", {{PARAM_SET, 0, &WallExceptionExpire}}},
	{"WallBadOS", {{PARAM_SET, 0, &WallBadOS}}},
	{"WallGetpass", {{PARAM_SET, 0, &WallGetpass}}},
	{"WallOper", {{PARAM_SET, 0, &WallOper}}},
	{"WallOSAkill", {{PARAM_SET, 0, &WallOSAkill}}},
	{"WallOSClearmodes", {{PARAM_SET, 0, &WallOSClearmodes}}},
	{"WallOSException", {{PARAM_SET, 0, &WallOSException}}},
	{"WallOSKick", {{PARAM_SET, 0, &WallOSKick}}},
	{"WallOSMode", {{PARAM_SET, 0, &WallOSMode}}},
	{"WallSetpass", {{PARAM_SET, 0, &WallSetpass}}},
	{"WarningTimeout", {{PARAM_TIME, 0, &WarningTimeout}}},
    {"NSDefCloak", {{PARAM_SET, 0, &NSDefCloak}}},
};

/*************************************************************************/

/* Print an error message to the log (and the console, if open). */

void error(int linenum, char *message, ...)
{
	char buf[4096];
	va_list args;

	va_start(args, message);
	vsnprintf(buf, sizeof(buf), message, args);
#ifndef NOT_MAIN
	if (linenum)
		log("%s:%d: %s", SERVICES_CONF, linenum, buf);
	else
		log("%s: %s", SERVICES_CONF, buf);
	if (!nofork && isatty(2)) {
#endif
		if (linenum)
			fprintf(stderr, "%s:%d: %s\n", SERVICES_CONF, linenum,
				buf);
		else
			fprintf(stderr, "%s: %s\n", SERVICES_CONF, buf);
#ifndef NOT_MAIN
	}
#endif
}

/*************************************************************************/

/* Parse a configuration line.  Return 1 on success; otherwise, print an
 * appropriate error message and return 0.  Destroys the buffer by side
 * effect.
 */

int parse(char *buf, int linenum)
{
	char *s, *t, *dir;
	size_t n;
	int i, optind, val, retval, ac;
	char *av[MAXPARAMS];
	Directive *d;

	s = NULL;
	retval = 1;
	ac = 0;

	dir = strtok(buf, " \t\r\n");

	if (dir)
		s = strtok(NULL, "");

	if (s && dir) {
		while (isspace(*s))
			s++;
		while (*s) {
			if (ac >= MAXPARAMS) {
				error(linenum,
				    "Warning: too many parameters (%d max)",
				    MAXPARAMS);
				break;
			}
			t = s;
			if (*s == '"') {
				t++;
				s++;
				while (*s && *s != '"') {
					if (*s == '\\' && s[1] != 0)
						s++;
					s++;
				}
				if (!*s) {
					error(linenum,
					    "Warning: unterminated "
					    "double-quoted string");
				} else
					*s++ = 0;
			} else {
				s += strcspn(s, " \t\r\n");
				if (*s)
					*s++ = 0;
			}
			av[ac++] = t;
			while (isspace(*s))
				s++;
		}
	}

	if (!dir)
		return 1;

	for (n = 0; n < lenof(directives); n++) {
		d = &directives[n];

		if (stricmp(dir, d->name) != 0)
			continue;
		optind = 0;
		for (i = 0; i < MAXPARAMS &&
		    d->params[i].type != PARAM_NONE; i++) {
			if (d->params[i].type == PARAM_SET) {
				*(int *)d->params[i].ptr = 1;
				continue;
			}
			if (d->params[i].type == PARAM_DEPRECATED) {
				void (*func) (void);

				error(linenum,
				    "Deprecated directive `%s' used",
				    d->name);
				func = (void (*)(void))(d->params[i].ptr);
				func();	/* For clarity */
				continue;
			}
			if (optind >= ac) {
				if (!(d->params[i].flags & PARAM_OPTIONAL)) {
					error(linenum,
					    "Not enough parameters for `%s'",
					    d->name);
					retval = 0;
				}
				break;
			}
			switch (d->params[i].type) {
			case PARAM_INT:
				val = strtol(av[optind++], &s, 0);
				if (*s) {
					error(linenum,
					    "%s: Expected an integer for "
					    "parameter %d", d->name,
					    optind);
					retval = 0;
					break;
				}
				*(int *)d->params[i].ptr = val;
				break;
			case PARAM_POSINT:
				val = strtol(av[optind++], &s, 0);
				if (*s || val <= 0) {
					error(linenum,
					    "%s: Expected a positive "
					    "integer for parameter %d",
					    d->name, optind);
					retval = 0;
					break;
				}
				*(int *)d->params[i].ptr = val;
				break;
			case PARAM_PORT:
				val = strtol(av[optind++], &s, 0);
				if (*s) {
					error(linenum,
					    "%s: Expected a port number "
					    "for parameter %d", d->name,
					    optind);
					retval = 0;
					break;
				}
				if (val < 1 || val > 65535) {
					error(linenum,
					    "Port numbers must be in the "
					    "range 1..65535");
					retval = 0;
					break;
				}
				*(int *)d->params[i].ptr = val;
				break;
			case PARAM_STRING:
				*(char **)d->params[i].ptr =
				    strdup(av[optind++]);
				if (!d->params[i].ptr) {
					error(linenum, "%s: Out of memory",
					    d->name);
					return 0;
				}
				break;
			case PARAM_TIME:
				val = dotime(av[optind++]);
				if (val < 0) {
					error(linenum,
					    "%s: Expected a time value for "
					    "parameter %d", d->name, optind);
					retval = 0;
					break;
				}
				*(int *)d->params[i].ptr = val;
				break;
			default:
				error(linenum,
				    "%s: Unknown type %d for param %d",
				    d->name, d->params[i].type, i + 1);
				return 0;	/* don't bother continuing--something's bizarre */
			}
		}
		break;		/* because we found a match */
	}

	if (n == lenof(directives)) {
		error(linenum, "Unknown directive `%s'", dir);
		return 1;	/* don't cause abort */
	}

	return retval;
}

/*************************************************************************/

#define CHECK(v) do {			\
    if (!v) {				\
	error(0, #v " missing");	\
	retval = 0;			\
    }					\
} while (0)

#define CHEK2(v,n) do {			\
    if (!v) {				\
	error(0, #n " missing");	\
	retval = 0;			\
    }					\
} while (0)

/* Read the entire configuration file.  If an error occurs while reading
 * the file or a required directive is not found, print and log an
 * appropriate error message and return 0; otherwise, return 1.
 */

int read_config()
{
	FILE *config;
	int linenum = 1, retval = 1;
	char buf[1024], *s;

	if (chdir(etcdir) < 0) {
#ifndef NOT_MAIN
		log_perror("Can't chdir(%s) ", etcdir);
		if (!nofork && isatty(2))
#endif
			perror("Can't chdir ");
		return 0;
	}

	config = fopen(SERVICES_CONF, "r");
	if (!config) {
#ifndef NOT_MAIN
		log_perror("Can't open " SERVICES_CONF);
		if (!nofork && isatty(2))
#endif
			perror("Can't open " SERVICES_CONF);
		return 0;
	}
	while (fgets(buf, sizeof(buf), config)) {
		if (buf[0] != '#')
			if (!parse(buf, linenum))
				retval = 0;
		linenum++;
	}
	fclose(config);

	CHECK(RemoteServer);
	CHECK(ServerName);
	CHECK(ServerDesc);
	CHEK2(temp_userhost, ServiceUser);
	CHEK2(s_NickServ, NickServName);
	CHEK2(s_ChanServ, ChanServName);
	CHEK2(s_MemoServ, MemoServName);
	CHEK2(s_HelpServ, HelpServName);
	CHEK2(s_OperServ, OperServName);
    CHEK2(s_FloodServ, FloodServName);
	CHEK2(s_GlobalNoticer, GlobalName);
	CHEK2(PIDFilename, PIDFile);
	CHEK2(MOTDFilename, MOTDFile);
	CHECK(HelpDir);
	CHECK(ExpireTimeout);
	CHECK(AuthExpireTimeout);
	CHECK(ReadTimeout);
	CHECK(WarningTimeout);
	CHECK(TimeoutCheck);
	CHECK(NSAccessMax);
	CHEK2(temp_nsuserhost, NSEnforcerUser);
	CHECK(NSReleaseTimeout);
	CHECK(NSListMax);
	CHECK(NSSuspendExpire);
	CHECK(CSAccessMax);
	CHECK(CSAutokickMax);
	CHECK(CSAutokickReason);
	CHECK(CSInhabit);
	CHECK(CSListMax);
	//FIXME: CHECK(CSSuspendExpire);
	CHECK(ServicesRoot);
	CHECK(AkillWildThresh);
	CHECK(AutokillExpiry);
	CHECK(NSMyEmail);
	CHECK(AbuseEmail);
	CHECK(NetworkName);
	CHECK(CSMyEmail);
	CHECK(mysqlHost);
	CHECK(mysqlUser);
	CHECK(mysqlPass);
	CHECK(mysqlDB);
	CHECK(DefaultCloak);

    CHECK(FloodServAkillExpiry);
    CHECK(FloodServAkillReason);
    CHECK(FloodServTFNumLines);
    CHECK(FloodServTFSec);
    CHECK(FloodServTFNLWarned);
    CHECK(FloodServTFSecWarned);
    CHECK(FloodServWarnFirst);
    CHECK(FloodServWarnMsg);

	if (temp_userhost) {
		if (!(s = strchr(temp_userhost, '@'))) {
			error(0, "Missing `@' for ServiceUser");
		} else {
			*s++ = 0;
			ServiceUser = temp_userhost;
			ServiceHost = s;
		}
	}

	if (temp_nsuserhost) {
		if (!(s = strchr(temp_nsuserhost, '@'))) {
			NSEnforcerUser = temp_nsuserhost;
			NSEnforcerHost = ServiceHost;
		} else {
			*s++ = 0;
			NSEnforcerUser = temp_userhost;
			NSEnforcerHost = s;
		}
	}

	if (!NSDefNone && !NSDefEnforce && !NSDefEnforceQuick && !NSDefSecure
	    && !NSDefPrivate && !NSDefHideEmail && !NSDefHideUsermask
	    && !NSDefHideQuit && !NSDefMemoSignon && !NSDefMemoReceive && !NSDefCloak) {
		NSDefSecure = 1;
		NSDefMemoSignon = 1;
		NSDefMemoReceive = 1;
        NSDefCloak = 1;
	}

	if (LimitSessions) {
		CHECK(DefSessionLimit);
		CHECK(MaxSessionLimit);
		CHECK(ExceptionExpiry);
	}

	return retval;
}

/* free up memory used by our config directives */
void free_directives()
{
	size_t i, p;
	Directive *d;

	for (i = 0; i < lenof(directives); i++) {
		d = &directives[i];

		for (p = 0; p < MAXPARAMS; p++) {
			if (d->params[p].type == PARAM_STRING &&
			    (char *)d->params[p].ptr) {
				free(*(char **)d->params[p].ptr);
			}
		}
	}
}	

/*************************************************************************/
