
/* ChanServ functions.
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

/*************************************************************************/

#include "services.h"
#include "pseudo.h"

RCSID("$Id$");

/*************************************************************************/

ChannelInfo *chanlists[256];

static int def_levels[][2] = {
	{CA_AUTOOP, 5},
	{CA_AUTOVOICE, 3},
	{CA_AUTODEOP, -1},
	{CA_NOJOIN, -2},
	{CA_INVITE, 5},
	{CA_AKICK, 10},
	{CA_SET, ACCESS_INVALID},
	{CA_CLEAR, ACCESS_INVALID},
	{CA_UNBAN, 5},
	{CA_OPDEOP, 5},
	{CA_ACCESS_LIST, 1},
	{CA_ACCESS_CHANGE, 10},
	{CA_MEMO_READ, 1},
	{CA_LEVEL_LIST, 1},
	{CA_LEVEL_CHANGE, ACCESS_INVALID},
	{CA_SYNC, 10},
	{CA_MEMO_SEND, 10},
	{CA_AKICK_LIST, 10},
	{-1}
};

typedef struct {
	int what;
	char *name;
	int desc;
} LevelInfo;
static LevelInfo levelinfo[] = {
	{CA_AUTOOP, "AUTOOP", CHAN_LEVEL_AUTOOP},
	{CA_AUTOVOICE, "AUTOVOICE", CHAN_LEVEL_AUTOVOICE},
	{CA_AUTODEOP, "AUTODEOP", CHAN_LEVEL_AUTODEOP},
	{CA_NOJOIN, "NOJOIN", CHAN_LEVEL_NOJOIN},
	{CA_INVITE, "INVITE", CHAN_LEVEL_INVITE},
	{CA_AKICK, "AKICK", CHAN_LEVEL_AKICK},
	{CA_SET, "SET", CHAN_LEVEL_SET},
	{CA_CLEAR, "CLEAR", CHAN_LEVEL_CLEAR},
	{CA_UNBAN, "UNBAN", CHAN_LEVEL_UNBAN},
	{CA_OPDEOP, "OPDEOP", CHAN_LEVEL_OPDEOP},
	{CA_ACCESS_LIST, "ACC-LIST", CHAN_LEVEL_ACCESS_LIST},
	{CA_ACCESS_CHANGE, "ACC-CHANGE", CHAN_LEVEL_ACCESS_CHANGE},
	{CA_MEMO_READ, "MEMO-READ", CHAN_LEVEL_MEMO_READ},
	{CA_LEVEL_LIST, "LEV-LIST", CHAN_LEVEL_LEVEL_LIST},
	{CA_LEVEL_CHANGE, "LEV-CHANGE", CHAN_LEVEL_LEVEL_CHANGE},
	{CA_SYNC, "SYNC", CHAN_LEVEL_SYNC},
	{CA_MEMO_SEND, "MEMO-SEND", CHAN_LEVEL_MEMO_SEND},
	{CA_AKICK_LIST, "AKICK-LIST", CHAN_LEVEL_AKICK_LIST},
	{-1, 0, -1}
};
static unsigned int levelinfo_maxwidth = 0;

/*************************************************************************/

static void reset_levels(unsigned int channel_id);
static int is_founder(User *user, unsigned int channel_id);
static int is_identified(User *user, unsigned int channel_id);
static unsigned int check_chan_password(const char *pass,
    unsigned int channel_id);
static void mysql_findchanflags(const char *chan, unsigned int *channel_id,
    int *flags);
static unsigned int get_founder_master_id(unsigned int channel_id);
static unsigned int get_founder_id(unsigned int channel_id);
static unsigned int get_chanaccess_id(unsigned int channel_id,
    unsigned int nick_id);
static int get_chanaccess_level(unsigned int channel_id,
    unsigned int nick_id);
static void update_chan_last_used(unsigned int channel_id);
static void update_akick_last_used(unsigned int akick_id, User *u);
static time_t get_founder_memo_read(unsigned int channel_id);
static time_t get_access_memo_read(User *u, unsigned int channel_id);
static unsigned int insert_new_channel(ChannelInfo *ci, MemoInfo *mi);
static void mysql_delchan(unsigned int channel_id);
static int get_mlock_on(unsigned int channel_id);
static int get_mlock_off(unsigned int channel_id);
static unsigned int get_mlock_limit(unsigned int channel_id);
static char *get_mlock_key(unsigned int channel_id);
static void set_chan_founder(unsigned int channel_id,
    unsigned int nfounder_id);
static unsigned int get_channelmax(unsigned int nick_id);
static unsigned int count_chans_reged_by(unsigned int nick_id);
static void set_chan_successor(unsigned int channel_id,
    unsigned int nsuccessor_id);
static void set_chan_pass(unsigned int channel_id, const char *pass);
static void set_chan_level(unsigned int channel_id, int what, int level);
static void set_chan_bantime(unsigned int channel_id, unsigned int bantime);
static void set_chan_regtime(unsigned int channel_id, time_t regtime);
static void set_chan_autolimit(unsigned int channel_id,
    unsigned int offset, unsigned int tolerance, unsigned int period);
static int lookup_def_level(int what);
static void renumber_akicks(unsigned int channel_id);
static unsigned int chanakick_count(unsigned int channel_id);

static void do_help(User *u);
static void do_register(User *u);
static void do_identify(User *u);
static void do_drop(User *u);
static void do_set(User *u);
static void do_set_founder(User *u, unsigned int channel_id, char *param);
static void do_set_successor(User *u, unsigned int channel_id, char *param);
static void do_set_password(User *u, unsigned int channel_id, char *param);
static void do_set_desc(User *u, unsigned int channel_id, char *param);
static void do_set_url(User *u, unsigned int channel_id, char *param);
static void do_set_email(User *u, unsigned int channel_id, char *param);
static void do_set_entrymsg(User *u, unsigned int channel_id, char *param);
static void do_set_topic(User *u, unsigned int channel_id, char *param);
static void do_set_mlock(User *u, unsigned int channel_id, char *param);
static void do_set_keeptopic(User *u, unsigned int channel_id, char *param);
static void do_set_topiclock(User *u, unsigned int channel_id, char *param);
static void do_set_private(User *u, unsigned int channel_id, char *param);
static void do_set_secureops(User *u, unsigned int channel_id, char *param);
static void do_set_leaveops(User *u, unsigned int channel_id, char *param);
static void do_set_restricted(User *u, unsigned int channel_id, char *param);
static void do_set_secure(User *u, unsigned int channel_id, char *param);
static void do_set_verbose(User *u, unsigned int channel_id, char *param);
static void do_set_noexpire(User *u, unsigned int channel_id, char *param);
static void do_set_autolimit(User *u, unsigned int channel_id, char *param);
static void do_set_clearbans(User *u, unsigned int channel_id, char *param);
static void do_set_regtime(User *u, unsigned int channel_id, char *param);
static void do_set_floodserv(User *u, unsigned int channel_id, char *param);
static void do_akick(User *u);
static void do_info(User *u);
static void do_list(User *u);
static void do_invite(User *u);
static void do_levels(User *u);
static void do_op(User *u);
static void do_deop(User *u);
static void do_voice(User *u);
static void do_devoice(User *u);
static void do_unban(User *u);
static void do_clear(User *u);
static void do_sendpass(User *u);
static void do_forbid(User *u);
static void do_status(User *u);
static void do_sync(User *u);

/*************************************************************************/

static Command cmds[] = {
	{"HELP", do_help, NULL, -1, -1, -1, -1, -1, NULL, NULL, NULL, NULL},
	{"REGISTER", do_register, NULL, CHAN_HELP_REGISTER, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"REG", do_register, NULL, CHAN_HELP_REGISTER, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"IDENTIFY", do_identify, NULL, CHAN_HELP_IDENTIFY, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"ID", do_identify, NULL, CHAN_HELP_IDENTIFY, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"DROP", do_drop, NULL, -1, CHAN_HELP_DROP,
	    CHAN_SERVADMIN_HELP_DROP, CHAN_SERVADMIN_HELP_DROP,
	    CHAN_SERVADMIN_HELP_DROP, NULL, NULL, NULL, NULL},
	{"SET", do_set, NULL, CHAN_HELP_SET, -1, CHAN_SERVADMIN_HELP_SET,
	    CHAN_SERVADMIN_HELP_SET, CHAN_SERVADMIN_HELP_SET, NULL, NULL,
	    NULL, NULL},
	{"SET FOUNDER", NULL, NULL, CHAN_HELP_SET_FOUNDER, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET SUCCESSOR", NULL, NULL, CHAN_HELP_SET_SUCCESSOR, -1, -1, -1,
	    -1, NULL, NULL, NULL, NULL},
	{"SET PASSWORD", NULL, NULL, CHAN_HELP_SET_PASSWORD, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET DESC", NULL, NULL, CHAN_HELP_SET_DESC, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"SET URL", NULL, NULL, CHAN_HELP_SET_URL, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"SET EMAIL", NULL, NULL, CHAN_HELP_SET_EMAIL, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET ENTRYMSG", NULL, NULL, CHAN_HELP_SET_ENTRYMSG, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
        {"SET FLOODSERV", NULL, NULL, CHAN_HELP_SET_FLOODSERV, -1, -1, -1, -1,
            NULL, NULL, NULL, NULL},
	{"SET TOPIC", NULL, NULL, CHAN_HELP_SET_TOPIC, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"SET KEEPTOPIC", NULL, NULL, CHAN_HELP_SET_KEEPTOPIC, -1, -1, -1,
	    -1, NULL, NULL, NULL, NULL},
	{"SET TOPICLOCK", NULL, NULL, CHAN_HELP_SET_TOPICLOCK, -1, -1, -1,
	    -1, NULL, NULL, NULL, NULL},
	{"SET MLOCK", NULL, NULL, CHAN_HELP_SET_MLOCK, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"SET PRIVATE", NULL, NULL, CHAN_HELP_SET_PRIVATE, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET RESTRICTED", NULL, NULL, CHAN_HELP_SET_RESTRICTED, -1, -1, -1,
	    -1, NULL, NULL, NULL, NULL},
	{"SET SECURE", NULL, NULL, CHAN_HELP_SET_SECURE, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET SECUREOPS", NULL, NULL, CHAN_HELP_SET_SECUREOPS, -1, -1, -1,
	    -1, NULL, NULL, NULL, NULL},
	{"SET LEAVEOPS", NULL, NULL, CHAN_HELP_SET_LEAVEOPS, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET VERBOSE", NULL, NULL, CHAN_HELP_SET_VERBOSE, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"SET NOEXPIRE", NULL, NULL, -1, -1,
	    CHAN_SERVADMIN_HELP_SET_NOEXPIRE,
	    CHAN_SERVADMIN_HELP_SET_NOEXPIRE,
	    CHAN_SERVADMIN_HELP_SET_NOEXPIRE, NULL, NULL, NULL, NULL},
	{"SET AUTOLIMIT", NULL, NULL, CHAN_HELP_SET_AUTOLIMIT, -1, -1, -1,
	    -1, NULL, NULL, NULL, NULL},
	{"SET CLEARBANS", NULL, NULL, CHAN_HELP_SET_CLEARBANS, -1, -1, -1,
	    -1, NULL, NULL, NULL, NULL},
	{"SET REGTIME", NULL, NULL, -1, -1, CHAN_SERVADMIN_HELP_SET_REGTIME,
	    CHAN_SERVADMIN_HELP_SET_REGTIME,
	    CHAN_SERVADMIN_HELP_SET_REGTIME, NULL, NULL, NULL, NULL},
	{"ACCESS", do_cs_access, NULL, CHAN_HELP_ACCESS, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
        {"VOP", do_cs_vop, NULL, CHAN_HELP_VOP, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
        {"AOP", do_cs_aop, NULL, CHAN_HELP_AOP, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
        {"SOP", do_cs_sop, NULL, CHAN_HELP_SOP, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	{"ACCESS LEVELS", NULL, NULL, CHAN_HELP_ACCESS_LEVELS, -1, -1, -1,
	    -1, NULL, NULL, NULL, NULL},
	{"AKICK", do_akick, NULL, CHAN_HELP_AKICK, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"LEVELS", do_levels, NULL, CHAN_HELP_LEVELS, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"INFO", do_info, NULL, CHAN_HELP_INFO, -1,
	    CHAN_SERVADMIN_HELP_INFO, CHAN_SERVADMIN_HELP_INFO,
	    CHAN_SERVADMIN_HELP_INFO, NULL, NULL, NULL, NULL},
	{"LIST", do_list, NULL, -1, CHAN_HELP_LIST,
	    CHAN_SERVADMIN_HELP_LIST, CHAN_SERVADMIN_HELP_LIST,
	    CHAN_SERVADMIN_HELP_LIST, NULL, NULL, NULL, NULL},
	{"OP", do_op, NULL, CHAN_HELP_OP, -1, -1, -1, -1, NULL, NULL, NULL,
	    NULL},
	{"DEOP", do_deop, NULL, CHAN_HELP_DEOP, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	{"VOICE", do_voice, NULL, CHAN_HELP_VOICE, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"DEVOICE", do_devoice, NULL, CHAN_HELP_DEVOICE, -1, -1, -1, -1,
	    NULL, NULL, NULL, NULL},
	{"INVITE", do_invite, NULL, CHAN_HELP_INVITE, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"UNBAN", do_unban, NULL, CHAN_HELP_UNBAN, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"CLEAR", do_clear, NULL, CHAN_HELP_CLEAR, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"SENDPASS", do_sendpass, NULL, CHAN_HELP_SENDPASS, -1, -1, -1, -1, NULL,
	    NULL, NULL, NULL},
	{"FORBID", do_forbid, is_services_admin, -1, -1,
	    CHAN_SERVADMIN_HELP_FORBID, CHAN_SERVADMIN_HELP_FORBID,
	    CHAN_SERVADMIN_HELP_FORBID, NULL, NULL, NULL, NULL},
	{"STATUS", do_status, is_services_admin, -1, -1,
	    CHAN_SERVADMIN_HELP_STATUS, CHAN_SERVADMIN_HELP_STATUS,
	    CHAN_SERVADMIN_HELP_STATUS, NULL, NULL, NULL, NULL},
	{"SYNC", do_sync, NULL, CHAN_HELP_SYNC, -1, -1, -1, -1, NULL, NULL,
	    NULL, NULL},
	{NULL, NULL, NULL, -1, -1, -1, -1, -1, NULL, NULL, NULL, NULL}
};

/*************************************************************************/

/*************************************************************************/

/* Return information on memory use.  Assumes pointers are valid. */

void get_chanserv_stats(long *nrec, long *memuse)
{
	long count, mem;

	count = 0;
	mem = 0;

	get_table_stats(mysqlconn, "channel", (unsigned int *)&count,
	    (unsigned int *)&mem);

	*nrec += count;
	*memuse += (mem / 1024);

	get_table_stats(mysqlconn, "akick", (unsigned int *)&count,
	    (unsigned int *)&mem);

	*memuse += (mem / 1024);

	get_table_stats(mysqlconn, "chanaccess", (unsigned int *)&count,
	    (unsigned int *)&mem);

	*memuse += (mem / 1024);

	get_table_stats(mysqlconn, "chansuspend", (unsigned int *)&count,
	    (unsigned int *)&mem);

	*memuse += (mem / 1024);
}

/*************************************************************************/

/*************************************************************************/

/* ChanServ initialization. */

void cs_init(void)
{
	Command *cmd;

	cmd = lookup_cmd(cmds, "REGISTER");
	if (cmd)
		cmd->help_param1 = s_NickServ;
	cmd = lookup_cmd(cmds, "SET SECURE");
	if (cmd)
		cmd->help_param1 = s_NickServ;
	cmd = lookup_cmd(cmds, "SET SUCCESSOR");
	if (cmd)
		cmd->help_param1 = (char *)(long)CSMaxReg;
}

/*************************************************************************/

/* Main ChanServ routine. */

void chanserv(const char *source, char *buf)
{
	char *cmd, *s;
	User *u;
	
	u = finduser(source);

	if (!u) {
		log("%s: user record for %s not found", s_ChanServ, source);
		notice(s_NickServ, source,
		    getstring(0, USER_RECORD_NOT_FOUND));
		return;
	}

	cmd = strtok(buf, " ");

	if (!cmd) {
		return;
	} else if (stricmp(cmd, "\1PING") == 0) {
		if (!(s = strtok(NULL, "")))
			s = "\1";
		notice(s_ChanServ, source, "\1PING %s", s);
	} else if (stricmp(cmd, "\1VERSION\1") == 0) {
		notice(s_ChanServ, source, "\1VERSION Blitzed-Based OFTC IRC Services v%s\1", VERSION);
	} else {
		run_cmd(s_ChanServ, u, cmds, cmd);
	}
}

/*************************************************************************/

/* Check the current modes on a channel; if they conflict with a mode lock,
 * fix them. */

void check_modes(const char *chan)
{
	Channel *c;
	unsigned int channel_id, mlock_limit;
	char newmodes[32];
	int32 newlimit;
	int modes, set_limit, set_key, mlock_on, mlock_off;
	char *av[3];
	char *end, *mlock_key;
	char *newkey;

	c = findchan(chan);
	newkey = NULL;
	newlimit = 0;
	end = newmodes;
	set_limit = 0;
	set_key = 0;

	if (!c || c->bouncy_modes)
		return;

	/* Check for mode bouncing */
	if (c->server_modecount >= 3 && c->chanserv_modecount >= 3) {
		wallops(NULL, "Bouncy modes on channel %s. %d %d", chan,
        c->server_modecount, c->chanserv_modecount);
		log("%s: Bouncy modes on channel %s", s_ChanServ, c->name);
    /* Don't set bouncy modes, this is a weird unknown thing we'll log it
     * for information gathering, though
		c->bouncy_modes = 1;
    */
	}

	if (c->chanserv_modetime != time(NULL)) {
		c->chanserv_modecount = 0;
		c->chanserv_modetime = time(NULL);
	}
	c->chanserv_modecount++;

	channel_id = c->channel_id;

	*end++ = '+';

	mlock_on = get_mlock_on(channel_id);
	
	modes = ~c->mode & mlock_on;
	if (modes & CMODE_i) {
		*end++ = 'i';
		c->mode |= CMODE_i;
	}
	if (modes & CMODE_m) {
		*end++ = 'm';
		c->mode |= CMODE_m;
	}
	if (modes & CMODE_M) {
		*end++ = 'M';
		c->mode |= CMODE_M;
	}
	if (modes & CMODE_n) {
		*end++ = 'n';
		c->mode |= CMODE_n;
	}
	if (modes & CMODE_p) {
		*end++ = 'p';
		c->mode |= CMODE_p;
	}
	if (modes & CMODE_s) {
		*end++ = 's';
		c->mode |= CMODE_s;
	}
	if (modes & CMODE_t) {
		*end++ = 't';
		c->mode |= CMODE_t;
	}
	if (modes & CMODE_c) {
		*end++ = 'c';
		c->mode |= CMODE_c;
	}

	if (modes & CMODE_O) {
		*end++ = 'O';
		c->mode |= CMODE_O;
	}
	if (modes & CMODE_R) {
		*end++ = 'R';
		c->mode |= CMODE_R;
	}

	mlock_limit = get_mlock_limit(channel_id);

	if (mlock_limit && mlock_limit != c->limit) {
		*end++ = 'l';
		newlimit = mlock_limit;
		c->limit = newlimit;
		set_limit = 1;
	}

	mlock_key = get_mlock_key(channel_id);
	
	if (!c->key && mlock_key && mlock_key[0]) {
		*end++ = 'k';
		newkey = mlock_key;
		c->key = sstrdup(newkey);
		set_key = 1;
	} else if (c->key && mlock_key && mlock_key[0] &&
	    strcmp(c->key, mlock_key) != 0) {
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -k %s", c->name,
		    c->key);
		av[0] = sstrdup(c->name);
		av[1] = sstrdup("-k");
		av[2] = sstrdup(c->key);
		do_cmode(s_ChanServ, 3, av);
		free(av[0]);
		free(av[1]);
		free(av[2]);
		free(c->key);
		*end++ = 'k';
		newkey = mlock_key;
		c->key = sstrdup(newkey);
		set_key = 1;
	}

	if (end[-1] == '+')
		end--;

	*end++ = '-';

	mlock_off = get_mlock_off(channel_id);
	
	modes = c->mode & mlock_off;
	if (modes & CMODE_i) {
		*end++ = 'i';
		c->mode &= ~CMODE_i;
	}
	if (modes & CMODE_m) {
		*end++ = 'm';
		c->mode &= ~CMODE_m;
	}
	if (modes & CMODE_M) {
		*end++ = 'M';
		c->mode &= ~CMODE_M;
	}
	if (modes & CMODE_n) {
		*end++ = 'n';
		c->mode &= ~CMODE_n;
	}
	if (modes & CMODE_p) {
		*end++ = 'p';
		c->mode &= ~CMODE_p;
	}
	if (modes & CMODE_s) {
		*end++ = 's';
		c->mode &= ~CMODE_s;
	}
	if (modes & CMODE_t) {
		*end++ = 't';
		c->mode &= ~CMODE_t;
	}
	if (modes & CMODE_c) {
		*end++ = 'c';
		c->mode &= ~CMODE_c;
	}
	if (modes & CMODE_O) {
		*end++ = 'O';
		c->mode &= ~CMODE_O;
	}
	if (modes & CMODE_R) {
		*end++ = 'R';
		c->mode &= ~CMODE_R;
	}

	if (c->key && (mlock_off & CMODE_k)) {
		*end++ = 'k';
		newkey = sstrdup(c->key);
		free(c->key);
		c->key = NULL;
		set_key = 1;
	}
	if (c->limit && (mlock_off & CMODE_l)) {
		*end++ = 'l';
		c->limit = 0;
	}

	if (end[-1] == '-')
		end--;

	if (end == newmodes)
		return;
	*end = 0;
	if (set_limit) {
		if (set_key) {
			send_cmd(MODE_SENDER(s_ChanServ),
			    "MODE %s %s %d :%s", c->name, newmodes,
			    newlimit, c->key ? c->key : "");
		} else {
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s %d",
			    c->name, newmodes, newlimit);
		}
	} else if (set_key) {
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s", c->name,
		    newmodes, newkey ? newkey : "");
	} else {
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s", c->name,
		    newmodes);
	}

	if (mlock_key)
		free(mlock_key);
	
	if (newkey && !c->key)
		free(newkey);
}

/*************************************************************************/

/* Check whether a user is allowed to be opped on a channel; if they
 * aren't, deop them.  If serverop is 1, the +o was done by a server.
 * Return 1 if the user is allowed to be opped, 0 otherwise. */

int check_valid_op(User * user, const char *chan, int serverop)
{
	unsigned int channel_id;
	int flags;

	mysql_findchanflags(chan, &channel_id, &flags);

	if (!channel_id || (flags & CI_LEAVEOPS))
		return 1;

	if (CSOpOper && (is_oper(user->nick) || is_services_admin(user)))
		return 1;

	if (flags & CI_VERBOTEN) {
		/* check_kick() will get them out; we needn't explain. */
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -o %s", chan,
			 user->nick);
		return 0;
	}

	if (serverop && time(NULL) - start_time >= CSRestrictDelay
	    && !check_access(user, channel_id, CA_AUTOOP)) {
		notice_lang(s_ChanServ, user, CHAN_IS_REGISTERED, s_ChanServ);
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -o %s", chan,
			 user->nick);
		return 0;
	}

	if (check_access(user, channel_id, CA_AUTODEOP)) {
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -o %s", chan,
			 user->nick);
		return 0;
	}

	return 1;
}

/*************************************************************************/

/* Check whether a user should be opped on a channel, and if so, do it.
 * Return 1 if the user was opped, 0 otherwise.  (Updates the channel's
 * last used time if the user was opped. Fixed - Matthew Sullivan 1/4/03) */

int check_should_op(User * user, const char *chan)
{
	unsigned int channel_id;
	int flags;

	if (chan[0] == '+' || chan[0] == '&')
		return 0;

	mysql_findchanflags(chan, &channel_id, &flags);

	if (!channel_id || (flags & CI_VERBOTEN))
		return 0;

	if ((flags & CI_SECURE) && !nick_identified(user))
		return 0;

	if (check_access(user, channel_id, CA_AUTOOP)) {
        int reqlev;

        reqlev = get_chan_level(channel_id, CA_AUTOOP);

        if (reqlev >0)
        {
		  send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", chan,
			 user->nick);
          update_chan_last_used(channel_id);
          return(1);
        }
		update_chan_last_used(channel_id);
		return(0);
	} else if (get_access(user, channel_id) > 0) {
		update_chan_last_used(channel_id);
	}

	return(0);
}

/*************************************************************************/

/* Check whether a user should be voiced on a channel, and if so, do it.
 * Return 1 if the user was voiced, 0 otherwise. */

int check_should_voice(User * user, const char *chan)
{
	unsigned int channel_id;
	int flags;
	
	if (chan[0] == '+' || chan[0] == '&')
		return 0;
	
	mysql_findchanflags(chan, &channel_id, &flags);

	if (!channel_id || (flags & CI_VERBOTEN))
		return 0;

	if ((flags & CI_SECURE) && !nick_identified(user))
		return 0;

	if (check_access(user, channel_id, CA_AUTOVOICE)) {
        int reqlev;
        reqlev = get_chan_level(channel_id, CA_AUTOVOICE);

        if(reqlev > 0)
        {
		  send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +v %s", chan,
			 user->nick);
          return 1;
        }
        update_chan_last_used(channel_id);
	}

	return 0;
}

/*************************************************************************/

/* check if there are any memos for the channel which have arrived after
 * the last time they checked */

void check_chan_memos(User *u, unsigned int channel_id)
{
	time_t last_read_time;
	int access_level = 0, memoread_level = 0, new_memos;
	
	if (!channel_id)		/* this is not registered */
		return;

	access_level = get_access(u, channel_id);

	if (access_level <= 0)
		return;			/* channel not registered, nick not
					   id'd, no access */

	memoread_level = get_level(channel_id, CA_MEMO_READ);

					/* they can't read memos anyway */
	if (access_level < memoread_level)	
		return;

	if (access_level == ACCESS_FOUNDER)
		last_read_time = get_founder_memo_read(channel_id);
	else
		last_read_time = get_access_memo_read(u, channel_id);

	if ((new_memos = new_chan_memos(channel_id, last_read_time)) > 0) {
		char name[CHANMAX];

		get_chan_name(channel_id, name);
		notice_lang(s_MemoServ, u, CHAN_HAS_NEW_MEMOS, new_memos,
			    new_memos == 1 ? "" : "s",
			    new_memos == 1 ? "s" : "ve", name, s_MemoServ,
			    name);
	}
}

/*************************************************************************/

/* Copy the sent time from the memo to the last read time of the founder
 * or appropriate ChanAccess */

void update_memo_time(User *u, const char *chan, time_t memo_time)
{
	unsigned int nick_id = u->nick_id;
	unsigned int channel_id;
	unsigned int chanaccess_id = 0;
	MYSQL_RES *result;
	unsigned int fields, rows;

	if (!(channel_id = mysql_findchan(chan)))
		return;
	
	if (is_founder(u, channel_id)) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "UPDATE channel "
					   "SET founder_memo_read=%ld "
					   "WHERE channel_id=%u",
					   memo_time, channel_id);
		mysql_free_result(result);
		return;
	}

	if ((chanaccess_id = get_chanaccess_id(channel_id, nick_id))) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
					   "UPDATE chanaccess "
					   "SET memo_read=%ld "
					   "WHERE chanaccess_id=%u",
					   memo_time, chanaccess_id);
		mysql_free_result(result);
	}
}

/*************************************************************************/

/* Tiny helper routine to get ChanServ out of a channel after it went in. */

static void timeout_leave(Timeout * to)
{
	char *chan = to->data;

	send_cmd(s_ChanServ, "PART %s", chan);
	free(to->data);
}


/*
 * Check whether a user is permitted to be on a channel.  If so, return 0;
 * else, kickban the user with an appropriate message (could be either
 * AKICK or restricted access) and return 1.  Note that this is called
 * _before_ the user is added to internal channel lists (so do_kick() is
 * not called).
 */

int check_kick(User * user, const char *chan)
{
	MYSQL_ROW row;
	unsigned int fields, rows, channel_id, nick_id, akick_id;
	unsigned int akick_nick_id;
	int stay, flags, nick;
	char *av[3], *mask, *esc_nick, *esc_username, *esc_host, *reason;
	char *nick_mask = "*!", *joined_mask;
	MYSQL_RES *result;
	Timeout *t;

	nick = akick_nick_id = 0;

	mysql_findchanflags(chan, &channel_id, &flags);

	if (!channel_id)
		return 0;

	if (!CSAkickOper &&
	    (is_oper(user->nick) || is_services_admin(user)))
		return 0;

	if (flags & CI_VERBOTEN) {
		mask = create_mask(user);
		reason = sstrdup(getstring(user->nick_id,
		    CHAN_MAY_NOT_BE_USED));
		goto kick;
	}

	if (nick_recognized(user))
		nick_id = user->nick_id;
	else
		nick_id = 0;

	esc_nick = smysql_escape_string(mysqlconn, user->nick);
	esc_username = smysql_escape_string(mysqlconn, user->username);
	esc_host = smysql_escape_string(mysqlconn, user->host);
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT akick.akick_id, akick.mask, akick.nick_id, "
	    "akick.reason FROM akick LEFT JOIN nick ON "
	    "akick.nick_id=nick.nick_id WHERE akick.channel_id=%u && "
	    "((akick.nick_id!=0 && %u=IF(nick.link_id, nick.link_id, "
	    "nick.nick_id)) || (akick.nick_id=0 && IRC_MATCH(akick.mask, "
	    "'%s!%s@%s')))", channel_id, nick_id, esc_nick, esc_username,
	    esc_host);
	free(esc_host);
	free(esc_username);
	free(esc_nick);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		akick_id = atoi(row[0]);
		akick_nick_id = atoi(row[2]);

		if (akick_nick_id)
        {
            nick = 1;
	    mask = create_mask(user);
        }
	else
        {
            nick = 0;
	    mask = sstrdup(row[1]);
	    nick_mask = "";
        }
		if (debug >= 2) {
			log("debug: %s matched akick %s", user->nick,
			    akick_nick_id ?
			    get_nick_from_id(akick_nick_id) : mask);
		}

		if (row[3])
			reason = sstrdup(row[3]);
		else
			reason = sstrdup(CSAutokickReason);

		mysql_free_result(result);
		update_akick_last_used(akick_id, user);
		goto kick;
	}
	mysql_free_result(result);

	if (time(NULL) - start_time >= CSRestrictDelay
	    && check_access(user, channel_id, CA_NOJOIN)) {
		mask = create_mask(user);
		reason = sstrdup(getstring(user->nick_id,
		    CHAN_NOT_ALLOWED_TO_JOIN));
		goto kick;
	}

	return 0;

      kick:
	if (debug) {
		log("debug: channel: AutoKicking %s!%s@%s", user->nick,
		    user->username, user->host);
	}
	/*
	 * Remember that the user has not been added to our channel user list
	 * yet, so we check whether the channel does not exist
	 */
	stay = !findchan(chan);
	av[0] = sstrdup(chan);
	if (stay) {
		send_cmd(NULL, "SJOIN %ld %s + %s", time(NULL), chan, s_ChanServ);
/* XXX		strcpy(av[0], chan); */
	}
/* XXX	strcpy(av[0], chan); */
	joined_mask = malloc(strlen(nick_mask) + strlen(mask) + 1);
	sprintf(joined_mask, "%s%s", nick_mask, mask);
	av[1] = sstrdup("+b");
	av[2] = joined_mask;
	send_cmd(s_ChanServ, "MODE %s +b: %s", chan, av[2]);
	do_cmode(s_ChanServ, 3, av);
	free(av[0]);
	free(av[1]);
	free(mask);
	free(joined_mask);
	send_cmd(s_ChanServ, "KICK %s %s :%s", chan, user->nick, reason);
	free(reason);
	if (stay) {
		t = add_timeout(CSInhabit, timeout_leave, 0);
		t->data = sstrdup(chan);
	}
	return 1;
}

/*************************************************************************/

/* Record the current channel topic in the ChannelInfo structure. */

void record_topic(const char *chan)
{
	unsigned int channel_id, fields, rows;
	Channel *c;
	char *esc_topic, *esc_topic_setter;
	MYSQL_RES *result;

	c = findchan(chan);
	if (!c || !(channel_id = c->channel_id))
		return;

	esc_topic = smysql_escape_string(mysqlconn,
	    c->topic ? c->topic : "");
	esc_topic_setter = smysql_escape_string(mysqlconn,
	    c->topic_setter ? c->topic_setter : "");
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET last_topic='%s', last_topic_setter='%s', "
	    "last_topic_time='%ld' WHERE channel_id=%u", esc_topic,
	    esc_topic_setter, c->topic_time, channel_id);
	mysql_free_result(result);
}

/*************************************************************************/

/* Restore the topic in a channel when it's created, if we should. */

void restore_topic(const char *chan)
{
	MYSQL_ROW row;
	unsigned int channel_id, fields, rows;
	Channel *c;
	MYSQL_RES *result;

	c = findchan(chan);

	if (!c || !(channel_id = c->channel_id) ||
	    !(get_chan_flags(channel_id) & CI_KEEPTOPIC))
		return;
	if (c->topic)
		free(c->topic);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT last_topic, last_topic_setter, last_topic_time "
	    "FROM channel WHERE channel_id=%u", channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result)) && strlen(row[0])) {
		c->topic = sstrdup(row[0]);
		strscpy(c->topic_setter, row[1], NICKMAX);
		c->topic_time = atoi(row[2]);
	} else {
		c->topic = NULL;
		strscpy(c->topic_setter, s_ChanServ, NICKMAX);
	}
	mysql_free_result(result);

	/* If this channel doesn't have a topic - don't send an empty topic
	 * Matthew Sullivan 1/4/03
	 */

    c->topic_time = time(NULL);
	if (c->topic)
        send_cmd(ServerName, "TBURST %ld %s %ld %s :%s", c->creation_time, chan, 
            c->topic_time, c->topic_setter, c->topic);
}

/*************************************************************************/

/*
 * See if the topic is locked on the given channel, and return 1 (and fix
 * the topic) if so.
 */

int check_topiclock(const char *chan, const char *setter, Channel *c)
{
	MYSQL_ROW row;
	unsigned int channel_id, fields, rows;
	MYSQL_RES *result;
	User *u;

	if (!c || !(channel_id = c->channel_id) ||
	    !(get_chan_flags(channel_id) & CI_TOPICLOCK))
		return 0;

	u = finduser(setter);

	/*
	 * If the person who set the topic has enough access in the channel
	 * to unset TOPICLOCK, then allow the topic change to go through as
	 * normal.
	 */
	if (u && u->nick_id && check_access(u, channel_id, CA_SET))
		return 0;

	if (c->topic)
		free(c->topic);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	   "SELECT last_topic, last_topic_setter, last_topic_time "
	   "FROM channel WHERE channel_id=%u", channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
	       	if (strlen(row[0]))
			c->topic = sstrdup(row[0]);
		else
			c->topic = NULL;
		strscpy(c->topic_setter, row[1], NICKMAX);
		c->topic_time = atoi(row[2]);
	}
	mysql_free_result(result);

	/* 
	 * send_cmd(NULL, "TOPIC %s %s %d :%s", chan,
	 *     c->topic_setter, c->topic_time, c->topic ? c->topic : "");
	 */

	/* If STOPICLOCK is set - reset the topic (to NULL if nessesary)
	 * Matthew Sullivan 1/4/03
	 */

  c->topic_time = time(NULL);
  send_cmd(ServerName, "TBURST %ld %s %ld %s :%s", c->creation_time, chan,
      c->topic_time, c->topic_setter, c->topic ? c->topic : "");

	return 1;
}

/*************************************************************************/

/* Remove all channels which have expired. */

void expire_chans()
{
	MYSQL_ROW row;
	unsigned int fields, rows, channel_id, expire_count;
	time_t now;
	MYSQL_RES *result;

	now = time(NULL);

	if (!CSExpire || opt_noexpire)
		return;

	expire_count = 0;

	if (CSExtraGrace) {
		/*
		 * Extra Grace option - channels that have been in
		 * existence for longer than CSExpire*1.5 seconds are permitted
		 * to be inactive for up to 3*CSExpire seconds instead of the
		 * normal CSExpire.
		 */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT channel_id, name FROM channel "
		    "WHERE (%ld - time_registered >= %d) && "
		    "(%ld - last_used >= %d) && !(flags & %d)", now,
		    (int)(CSExpire * 1.5), now, (CSExpire * 3),
		    (CI_VERBOTEN | CI_NO_EXPIRE));

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			channel_id = atoi(row[0]);
			log("Expiring channel %s", row[1]);
			snoop(s_ChanServ, "[EXPIRE] Expiring %s",
			    row[1]);
			mysql_delchan(channel_id);
			expire_count++;
		}

		mysql_free_result(result);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT channel_id, name FROM channel "
		    "WHERE (%ld - time_registered < %d) && "
		    "(%ld - last_used >= %d) && !(flags & %d)", now,
		    (int)(CSExpire * 1.5), now, CSExpire,
		    (CI_VERBOTEN | CI_NO_EXPIRE));
	} else {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT channel_id, name FROM channel "
		    "WHERE (%ld - last_used >= %d) && !(flags & %d)", now,
		    CSExpire, (CI_VERBOTEN | CI_NO_EXPIRE));
	}
	
	while ((row = smysql_fetch_row(mysqlconn, result))) {
		channel_id = atoi(row[0]);
		log("Expiring channel %s", row[1]);
		snoop(s_ChanServ, "[EXPIRE] Expiring %s", row[1]);
		mysql_delchan(channel_id);
		expire_count++;
	}

	mysql_free_result(result);

	if (expire_count) {
		snoop(s_ChanServ, "[EXPIRE] %d channel%s expired",
		    expire_count, expire_count == 1 ? "" : "s");
	}
}

/*************************************************************************/

/* Remove a (deleted or expired) nickname from all channel lists. */

void cs_mysql_remove_nick(unsigned int nick_id)
{
	MYSQL_ROW row;
	size_t i;
	unsigned int fields, rows;
	MYSQL_RES *result;
	unsigned int *chan_id, *successor_id;
	char *del_nick, *successor_nick, *chan;

	del_nick = get_nick_from_id(nick_id);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT channel_id, successor FROM channel "
	    "WHERE founder=%u && successor!=0", nick_id);

	if (rows) {
		chan_id = smalloc(rows * sizeof(unsigned int));
		successor_id = smalloc(rows * sizeof(unsigned int));

		for (i = 0; (row = smysql_fetch_row(mysqlconn, result));
		    i++) {
			chan_id[i] = atoi(row[0]);
			successor_id[i] = atoi(row[1]);
		}

		mysql_free_result(result);

		for (i = 0; i < rows; i++) {
			successor_nick = get_nick_from_id(successor_id[i]);
			chan = get_channame_from_id(chan_id[i]);

			if (check_chan_limit(successor_id[i]) != -1) {
				log("%s: Successor (%s) of %s owns too "
				    "many channels, deleting channel",
				    s_ChanServ, successor_nick, chan);
				snoop(s_ChanServ, "[DROP] Successor %s "
				    "of %s owns too many channels, "
				    "deleting channel", successor_nick,
				    chan);
				mysql_delchan(chan_id[i]);
			} else {
				log("%s: Transferring foundership of %s "
				    "from deleted nick %s to successor %s",
				    s_ChanServ, chan, del_nick,
				    successor_nick);
				snoop(s_ChanServ,
				    "[SUCCESSOR] Transferring foundership "
				    "of %s from deleted nick %s "
				    "to successor %s", chan, del_nick,
				    successor_nick);

				result = smysql_bulk_query(mysqlconn,
				    &fields, &rows, "UPDATE channel "
				    "SET founder=%u, successor=0 "
				    "WHERE channel_id=%u && founder=%u && "
				    "successor=%u", successor_id[i],
				    chan_id[i], nick_id, successor_id[i]);
				mysql_free_result(result);
			}
			
			free(chan);
			free(successor_nick);
		}

		free(successor_id);
		free(chan_id);
	}

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT channel_id FROM channel "
	    "WHERE founder=%u && successor=0", nick_id);

	if (rows) {
		chan_id = smalloc(rows * sizeof(unsigned int));

		for (i = 0; (row = smysql_fetch_row(mysqlconn, result));
		    i++) {
			chan_id[i] = atoi(row[0]);
		}

		mysql_free_result(result);

		for (i = 0; i < rows; i++) {
			chan = get_channame_from_id(chan_id[i]);

			log("%s: Deleting channel %s owned by deleted "
			    "nick %s", s_ChanServ, chan, del_nick);
			snoop(s_ChanServ, "[DROP] Deleting channel %s "
			    "owned by deleted nick %s", chan, del_nick);
			free(chan);
			mysql_delchan(chan_id[i]);
		}
	}

	free(del_nick);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM chanaccess WHERE nick_id=%u", nick_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM akick WHERE nick_id=%u", nick_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET successor=0 WHERE successor=%u", nick_id);
	mysql_free_result(result);
	
}

/*************************************************************************/

/* Return the ID of the channel with the given name, or 0 if the channel
 * isn't registered */
unsigned int mysql_findchan(const char *chan)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows, channel_id = 0;
	char *esc_chan = smysql_escape_string(mysqlconn, chan);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT channel_id FROM channel "
				   "WHERE name='%s'", esc_chan);
	if ((row = smysql_fetch_row(mysqlconn, result))) {
		channel_id = atoi(row[0]);
	}
	mysql_free_result(result);
	free(esc_chan);

	return(channel_id);
}

/* Find the channel with the given name and set its channel_id and flags
 * appropriately.  channel_id will be set to 0 if the channel is not found */
static void mysql_findchanflags(const char *chan, unsigned int *channel_id,
				int *flags)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows;
	char *esc_chan = smysql_escape_string(mysqlconn, chan);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT channel_id, flags FROM channel "
				   "WHERE name='%s'", esc_chan);
	if ((row = smysql_fetch_row(mysqlconn, result))) {
		*channel_id = atoi(row[0]);
		*flags = atoi(row[1]);
	} else {
		*channel_id = 0;
		*flags = 0;
	}
	mysql_free_result(result);
	return;
}

/* return the master nick_id of the founder of the channel, bearing in
 * mind that nicks can have one level of linking */
static unsigned int get_founder_master_id(unsigned int channel_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows, nick_id = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT IF(nick.link_id, nick.link_id, "
				   "nick.nick_id) FROM channel, nick "
				   "WHERE channel.founder=nick.nick_id && "
				   "channel.channel_id=%u", channel_id);
	if ((row = smysql_fetch_row(mysqlconn, result))) {
		nick_id = atoi(row[0]);
	}
	mysql_free_result(result);

	return(nick_id);
}

/*
 * Return the nick_id of the founder of the channel.
 */
static unsigned int get_founder_id(unsigned int channel_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows, nick_id;

	nick_id = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT founder FROM channel WHERE channel_id=%u", channel_id);
	if ((row = smysql_fetch_row(mysqlconn, result))) {
		nick_id = atoi(row[0]);
	}
	mysql_free_result(result);

	return(nick_id);
}

/*************************************************************************/

/* Return 1 if the user's access level on the given channel falls into the
 * given category, 0 otherwise.  Note that this may seem slightly confusing
 * in some cases: for example, check_access(..., CA_NOJOIN) returns true if
 * the user does _not_ have access to the channel (i.e. matches the NOJOIN
 * criterion). */

int check_access(User * user, unsigned int channel_id, int what)
{
	int what_level, user_level;

	/*
	 * Better to get the access level separately because they might be
	 * founder or just have access, which is a little too complicated
	 * to do in one.
	 */
	user_level = get_access(user, channel_id);

	if (user_level == ACCESS_FOUNDER) {
		/*
		 * They're the founder, this is an easy answer, they can do
		 * anything
		 */
		return((what == CA_AUTODEOP || what == CA_NOJOIN) ? 0 : 1);
	}

	what_level = get_chan_level(channel_id, what);
	
	/* Hacks to make flags work */
	if (what == CA_AUTODEOP && user_level == 0 &&
	    (get_chan_flags(channel_id) & CI_SECUREOPS))
		return(1);
	if (what_level == ACCESS_INVALID)
		return(0);
	if (what == CA_AUTODEOP || what == CA_NOJOIN)
		return(user_level <= what_level);
	else
		return(user_level >= what_level);
}

/*************************************************************************/

/*
 * Check the nick's number of registered channels against its limit, and
 * return -1 if below the limit, 0 if at it exactly, and 1 if over it.
 */

int check_chan_limit(unsigned int nick_id)
{
	unsigned int max, count;

	/*
	 * Make sure we're looking at the master nick - we only care about
	 * the channelmax of the master nick
	 */
	nick_id = mysql_getlink(nick_id);

	max = get_channelmax(nick_id);

	/*
	 * This function takes care of counting ALL channels registered by
	 * master nick and ALL links
	 */
	count = count_chans_reged_by(nick_id);

	if (!max)
		max = MAX_CHANNELCOUNT;

	return(count < max ? -1 : count == max ? 0 : 1);
}

/*************************************************************************/

/*********************** ChanServ private routines ***********************/

/*************************************************************************/

/*************************************************************************/

/*
 * Return the given user's allowed maximum channels.
 */
static unsigned int get_channelmax(unsigned int nick_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows, max;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT channelmax FROM nick WHERE nick_id=%u", nick_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		max = atoi(row[0]);
	} else {
		max = 0;
	}

	mysql_free_result(result);

	return(max);
}

/*
 * Return the number of channels that this user has registered so far.  The
 * nick_id passed in should be the master id, this function then works out all
 * linked id's and counts channels registered by those too.
 */
static unsigned int count_chans_reged_by(unsigned int nick_id)
{
	char chunk[BUFSIZE];
	MYSQL_ROW row;
	size_t chunk_len;
	unsigned int fields, rows, count, i, size;
	unsigned int *ids;
	MYSQL_RES *result;
	char *query;

	i = 0;
	ids = NULL;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT nick_id FROM nick WHERE link_id=%u", nick_id);

	if (rows) {
		ids = malloc(rows * sizeof(unsigned int));
		
		while ((row = smysql_fetch_row(mysqlconn, result))) {
			ids[i] = atoi(row[0]);
			i++;
		}
	}

	mysql_free_result(result);

	if (rows && ids) {
		query = sstrdup("SELECT COUNT(channel_id) FROM channel "
		    "WHERE founder IN (");
		size = strlen(query) + 1;

		for (i = 0; i < rows; i++) {
			memset(chunk, 0, sizeof(chunk));

			if (i == 0) {
				snprintf(chunk, sizeof(chunk), "%u,%u%s",
				    nick_id, ids[i],
				    (i == (rows - 1)) ? ")" : "");
			} else {
				snprintf(chunk, sizeof(chunk), ",%u%s",
				    ids[i], (i == (rows - 1)) ? ")" : "");
			}

			chunk_len = strlen(chunk);
			query = srealloc(query, chunk_len + size);
			strncat(query, chunk, chunk_len);
			size = strlen(query) + 1;
		}

		result = smysql_bulk_query(mysqlconn, &fields, &rows, query);
		free(query);
		free(ids);
	} else {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT COUNT(channel_id) FROM channel "
		    "WHERE founder=%u", nick_id);
	}

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		count = atoi(row[0]);
	} else {
		count = 0;
	}
	
	mysql_free_result(result);

	return(count);
}

/* Reset channel access level values to their default state. */

static void reset_levels(unsigned int channel_id)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM chanlevel WHERE channel_id=%u", channel_id);
	mysql_free_result(result);
}

/*************************************************************************/

/* Does the given user have founder access to the channel? */

static int is_founder(User * user, unsigned int channel_id)
{
	if (user->nick_id == get_founder_master_id(channel_id)) {
		if (nick_identified(user) ||
		    (nick_recognized(user) &&
		    !(get_chan_flags(channel_id) & CI_SECURE)))
			return 1;
	}
	if (is_identified(user, channel_id))
		return 1;
	return 0;
}

/*************************************************************************/

/* Has the given user password-identified as founder for the channel? */

static int is_identified(User * user, unsigned int channel_id)
{
	struct u_chanidlist *c;

	for (c = user->founder_chans; c; c = c->next) {
		if (c->channel_id == channel_id)
			return 1;
	}
	return 0;
}

/*************************************************************************/


/* Return the correct chanaccess_id for the user and channel, or 0 if
 * there is no matching access.  Note that this does not match channel
 * founders */
static unsigned int get_chanaccess_id(unsigned int channel_id,
				      unsigned int nick_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int fields, rows, chanaccess_id = 0;
	
        /* There will be at most 1 row in the chanaccess table describing
         * this user's access, but it could be for any of their linked
         * nicks.  We need to check u->nick_id against the master of each
         * nick in the access list to find it. */
        result = smysql_bulk_query(mysqlconn, &fields, &rows,
                                   "SELECT chanaccess.chanaccess_id "
                                   "FROM chanaccess, nick "
                                   "WHERE chanaccess.nick_id=nick.nick_id "
                                   "&& chanaccess.channel_id=%u && " 
                                   "IF(nick.link_id, nick.link_id, "
                                   "nick.nick_id)=%u", channel_id,
                                   nick_id);
        if ((row = smysql_fetch_row(mysqlconn, result))) {
                chanaccess_id = atoi(row[0]);
        }               
        mysql_free_result(result); 

	return(chanaccess_id);
}

/* Return the level of access that a given nick has in a given channel, or
 * 0 if there is no matching access.  Note that this does not match channel
 * founders */
static int get_chanaccess_level(unsigned int channel_id,
				unsigned int nick_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows, ca_id;
	int level;
	MYSQL_RES *result;

	ca_id = 0;
	level = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT chanaccess.chanaccess_id, chanaccess.level "
	    "FROM chanaccess, nick "
	    "WHERE chanaccess.nick_id=nick.nick_id && "
	    "chanaccess.channel_id=%u && "
	    "IF(nick.link_id, nick.link_id, nick.nick_id)=%u", channel_id,
	    nick_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		ca_id = atoi(row[0]);
		level = atoi(row[1]);
	}
	mysql_free_result(result);

	if (ca_id) {
		/* Update the "last used" time. */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE chanaccess SET last_used=%ld "
		    "WHERE chanaccess_id=%u", time(NULL), ca_id);
		mysql_free_result(result);
	}

	return(level);
}

/*************************************************************************/

/*********************** ChanServ command routines ***********************/

/*************************************************************************/

static void do_help(User * u)
{
	size_t len;
	int i;
	char *cmd;
	
	cmd = strtok(NULL, "");

	notice_help(s_ChanServ, u, CHAN_HELP_START);

	if (!cmd) {
		notice_help(s_ChanServ, u, CHAN_HELP);
		if (CSExpire >= 86400)
			notice_help(s_ChanServ, u, CHAN_HELP_EXPIRES,
			    CSExpire / 86400);
		if (is_services_oper(u))
			notice_help(s_ChanServ, u, CHAN_SERVADMIN_HELP);
	} else if (stricmp(cmd, "LEVELS DESC") == 0) {
		notice_help(s_ChanServ, u, CHAN_HELP_LEVELS_DESC);
		if (!levelinfo_maxwidth) {
			for (i = 0; levelinfo[i].what >= 0; i++) {
				len = strlen(levelinfo[i].name);

				if (len > levelinfo_maxwidth)
					levelinfo_maxwidth = len;
			}
		}
		for (i = 0; levelinfo[i].what >= 0; i++) {
			notice_help(s_ChanServ, u,
			    CHAN_HELP_LEVELS_DESC_FORMAT,
			    levelinfo_maxwidth, levelinfo[i].name,
			    getstring(u->nick_id,
			    (unsigned int)levelinfo[i].desc));
		}
	} else {
		help_cmd(s_ChanServ, u, cmds, cmd);
	}
	notice_help(s_ChanServ, u, CHAN_HELP_END);
}

/*************************************************************************/

static void do_register(User * u)
{
	NickInfo ni;
	ChannelInfo ci;
	MemoInfo mi;
	unsigned int channel_id;
	char *chan, *pass, *desc;
	Channel *c;
	struct u_chanidlist *uc;

	chan = strtok(NULL, " ");
	pass = strtok(NULL, " ");
	desc = strtok(NULL, "");

	if (!chan || !desc || !pass) {
		syntax_error(s_ChanServ, u, "REGISTER",
		    CHAN_REGISTER_SYNTAX);
	} else if (!validate_password(u, pass, chan)) {
		notice_lang(s_ChanServ, u, MORE_OBSCURE_PASSWORD);
	} else if (*chan == '&') {
		notice_lang(s_ChanServ, u, CHAN_REGISTER_NOT_LOCAL);
	} else if (!get_nickinfo_from_id(&ni, u->nick_id)) {
		notice_lang(s_ChanServ, u, CHAN_MUST_REGISTER_NICK,
		    s_NickServ);
	} else if (!nick_recognized(u)) {
		notice_lang(s_ChanServ, u, CHAN_MUST_IDENTIFY_NICK,
		    s_NickServ, s_NickServ);
	} else if ((channel_id = mysql_findchan(chan))) {
		if (get_chan_flags(channel_id) & CI_VERBOTEN) {
			log("%s: Attempt to register FORBIDden channel %s "
			    "by %s!%s@%s", s_ChanServ, chan, u->nick,
			    u->username, u->host);
			notice_lang(s_ChanServ, u,
			    CHAN_MAY_NOT_BE_REGISTERED, chan);
		} else {
			notice_lang(s_ChanServ, u, CHAN_ALREADY_REGISTERED,
			    chan);
		}
	} else if (!is_chanop(u->nick, chan)) {
		notice_lang(s_ChanServ, u, CHAN_MUST_BE_CHANOP);

	} else if (ni.channelmax > 0 &&
	    check_chan_limit(ni.nick_id) != -1) {
		if (check_chan_limit(ni.nick_id) == 1) {
			notice_lang(s_ChanServ, u,
			    CHAN_EXCEEDED_CHANNEL_LIMIT, ni.channelmax);
		} else {
			notice_lang(s_ChanServ, u,
			    CHAN_REACHED_CHANNEL_LIMIT, ni.channelmax);
		}
	} else if (!(c = findchan(chan))) {
		log("%s: Channel %s not found for REGISTER", s_ChanServ,
		    chan);
		notice_lang(s_ChanServ, u, CHAN_REGISTRATION_FAILED);
	} else {
		strscpy(ci.name, chan, CHANMAX);
		ci.founder = u->real_id;
		ci.successor = 0;
		strscpy(ci.founderpass, pass, PASSMAX);
		ci.desc = sstrdup(desc);
		ci.url = NULL;
		ci.email = NULL;
		ci.last_used = ci.time_registered = time(NULL);
		ci.founder_memo_read = ci.last_used;

		if (c->topic) {
			ci.last_topic = sstrdup(c->topic);
			strscpy(ci.last_topic_setter, c->topic_setter,
			    NICKMAX);
			ci.last_topic_time = c->topic_time;
		} else {
			ci.last_topic = NULL;
			ci.last_topic_setter[0] = 0;
		}

		ci.flags = CI_KEEPTOPIC | CI_SECURE;
		ci.mlock_on = CMODE_n;
		ci.mlock_off = 0;
		ci.mlock_limit = 0;
		ci.mlock_key = "";
		ci.entry_message = NULL;
		ci.last_limit_time = 0;
		ci.limit_offset = 0;
		ci.limit_tolerance = 0;
		ci.limit_period = 0;
		ci.bantime = 0;
		ci.last_sendpass_pass[0] = 0;
		ci.last_sendpass_salt[0] = 0;
		ci.last_sendpass_time = 0;
        ci.floodserv = 0;
		
		mi.max = MSMaxMemos;
		
		if (strlen(pass) > PASSMAX - 1)	/* -1 for null byte */
			notice_lang(s_ChanServ, u, PASSWORD_TRUNCATED,
			    PASSMAX - 1);
		
		
		c->channel_id = insert_new_channel(&ci, &mi);
		
		snoop(s_ChanServ, "[REG] %s by %s (%s@%s)", chan, u->nick,
		    u->username, u->host);
		log("%s: Channel %s registered by %s!%s@%s", s_ChanServ,
		    chan, u->nick, u->username, u->host);

		if (CSWallReg) {
			wallops(s_ChanServ, "[REG] %s!%s@%s registered "
			    "channel %s - %s", u->nick, u->username,
			    u->host, chan, desc);
		}     

		notice_lang(s_ChanServ, u, CHAN_REGISTERED, chan, u->nick);
		notice_lang(s_ChanServ, u, CHAN_PASSWORD_IS,
		    ci.founderpass);

		uc = smalloc(sizeof(*uc));
		uc->next = u->founder_chans;
		uc->prev = NULL;
		if (u->founder_chans)
			u->founder_chans->prev = uc;
		u->founder_chans = uc;
		uc->channel_id = ci.channel_id;
		/* Implement new mode lock */
		check_modes(ci.name);
		
		if (ci.last_topic)
			free(ci.last_topic);
		free(ci.desc);
	}
}

/*************************************************************************/

static void do_identify(User * u)
{
	int flags;
	unsigned int channel_id;
	char *chan, *pass;
	struct u_chanidlist *uc;
	
	chan = strtok(NULL, " ");
	pass = strtok(NULL, " ");

	if (!pass) {
		syntax_error(s_ChanServ, u, "IDENTIFY",
		    CHAN_IDENTIFY_SYNTAX);
	} else if (!(channel_id = mysql_findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if ((flags = get_chan_flags(channel_id) & CI_VERBOTEN)) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else {
		if (check_chan_password(pass, channel_id)) {
			if (!is_identified(u, channel_id)) {
				uc = smalloc(sizeof(*uc));
				uc->next = u->founder_chans;
				uc->prev = NULL;
				if (u->founder_chans)
					u->founder_chans->prev = uc;
				u->founder_chans = uc;
				uc->channel_id = channel_id;
				log("%s: %s!%s@%s identified for %s",
				    s_ChanServ, u->nick, u->username,
				    u->host, chan);
			}
			notice_lang(s_ChanServ, u, CHAN_IDENTIFY_SUCCEEDED,
			    chan);
			snoop(s_ChanServ, "[ID] %s by %s (%s@%s)", chan,
			    u->nick, u->username, u->host);
			if (flags & CI_VERBOSE) {
				opnotice(s_ChanServ, chan,
				    "%s identified as channel founder",
				    u->nick);
			}
		} else {
			log("%s: Failed IDENTIFY for %s by %s!%s@%s",
			    s_ChanServ, chan, u->nick, u->username,
			    u->host);
			notice_lang(s_ChanServ, u, PASSWORD_INCORRECT);
			if (!BadPassLimit) {
				snoop(s_ChanServ, "[BADPASS] %s (%s@%s) "
				    "Failed IDENTIFY for %s", u->nick,
				    u->username, u->host, chan);
			} else {
				snoop(s_ChanServ, "[BADPASS] %s (%s@%s) "
				    "Failed IDENTIFY for %s (%d of %d)",
				    u->nick, u->username, u->host, chan,
				    u->invalid_pw_count + 1, BadPassLimit);
                	}
			if (flags & CI_VERBOSE) {
				opnotice(s_ChanServ, chan, "%s (%s@%s) "
				"tried to identify as channel founder but "
				"gave an incorrect password", u->nick,
				u->username, u->host);
			}
			bad_password(u);
		}

	}
}

/*************************************************************************/

static void do_drop(User * u)
{
	int is_servadmin, flags;
	unsigned int channel_id;
	char *chan;

	chan = strtok(NULL, " ");
	is_servadmin = is_services_admin(u);

	if (!chan) {
		syntax_error(s_ChanServ, u, "DROP", CHAN_DROP_SYNTAX);
	} else if (!(channel_id = mysql_findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (((flags = get_chan_flags(channel_id) & CI_VERBOTEN)) &&
	    !is_servadmin) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!is_servadmin && !is_identified(u, channel_id)) {
		notice_lang(s_ChanServ, u, CHAN_IDENTIFY_REQUIRED, s_ChanServ,
		    chan);
	} else {
		log("%s: Channel %s dropped by %s!%s@%s", s_ChanServ, chan,
		    u->nick, u->username, u->host);
		mysql_delchan(channel_id);
		notice_lang(s_ChanServ, u, CHAN_DROPPED, chan);
        wallops(s_ChanServ, "[DROP] %s by %s (%s@%s)", chan, u->nick,
		    u->username, u->host);
		snoop(s_ChanServ, "[DROP] %s by %s (%s@%s)", chan, u->nick,
		    u->username, u->host);

		if (flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan, "%s has DROPped this "
			    "channel", u->nick);
		}
	}
}

/*************************************************************************/

/*
 * Main SET routine.  Calls other routines as follows:
 *	do_set_command(User *command_sender, unsigned int channel_id, char *param);
 * The parameter passed is the first space-delimited parameter after the
 * option name, except in the case of DESC, TOPIC, and ENTRYMSG, in which
 * it is the entire remainder of the line.  Additional parameters beyond
 * the first passed in the function call can be retrieved using
 * strtok(NULL, toks).
 */
static void do_set(User * u)
{
	unsigned int channel_id;
	int is_servadmin, flags;
	char *chan, *cmd, *param;

	chan = strtok(NULL, " ");
	cmd = strtok(NULL, " ");
	is_servadmin = is_services_admin(u);

	if (cmd) {
		if (stricmp(cmd, "DESC") == 0  ||
		    stricmp(cmd, "TOPIC") == 0 ||
		    stricmp(cmd, "ENTRYMSG") == 0)
			param = strtok(NULL, "");
		else
			param = strtok(NULL, " ");
	} else {
		param = NULL;
	}

	if (!param && (!cmd || (stricmp(cmd, "SUCCESSOR") != 0 &&
	    stricmp(cmd, "URL") != 0 && stricmp(cmd, "EMAIL") != 0 &&
	    stricmp(cmd, "ENTRYMSG") != 0))) {
		syntax_error(s_ChanServ, u, "SET", CHAN_SET_SYNTAX);
	} else if (!(channel_id = mysql_findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if ((flags = get_chan_flags(channel_id) & CI_VERBOTEN)) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!is_servadmin && !check_access(u, channel_id, CA_SET)) {
		notice_lang(s_ChanServ, u, ACCESS_DENIED);
	} else if (stricmp(cmd, "FOUNDER") == 0) {
		if (!is_servadmin &&
		    get_access(u, channel_id) < ACCESS_FOUNDER) {
			notice_lang(s_ChanServ, u, CHAN_IDENTIFY_REQUIRED,
			    s_ChanServ, chan);
		} else {
			do_set_founder(u, channel_id, param);
		}
	} else if (stricmp(cmd, "SUCCESSOR") == 0) {
		if (!is_servadmin &&
		    get_access(u, channel_id) < ACCESS_FOUNDER) {
			notice_lang(s_ChanServ, u, CHAN_IDENTIFY_REQUIRED,
			    s_ChanServ, chan);
		} else {
			do_set_successor(u, channel_id, param);
		}
	} else if (stricmp(cmd, "PASSWORD") == 0) {
		if (!is_servadmin &&
		    get_access(u, channel_id) < ACCESS_FOUNDER) {
			notice_lang(s_ChanServ, u, CHAN_IDENTIFY_REQUIRED,
			    s_ChanServ, chan);
		} else {
			do_set_password(u, channel_id, param);
		}
	} else if (stricmp(cmd, "DESC") == 0) {
		do_set_desc(u, channel_id, param);
	} else if (stricmp(cmd, "URL") == 0) {
		do_set_url(u, channel_id, param);
	} else if (stricmp(cmd, "EMAIL") == 0) {
		do_set_email(u, channel_id, param);
	} else if (stricmp(cmd, "ENTRYMSG") == 0) {
		do_set_entrymsg(u, channel_id, param);
        } else if (stricmp(cmd, "FLOODSERV") == 0) {
            if (!is_servadmin && get_access(u, channel_id) < ACCESS_FOUNDER) {
                notice_lang(s_ChanServ, u, CHAN_IDENTIFY_REQUIRED, s_ChanServ, chan);
            } else {
                do_set_floodserv(u, channel_id, param);
            }
	} else if (stricmp(cmd, "TOPIC") == 0) {
		do_set_topic(u, channel_id, param);
	} else if (stricmp(cmd, "MLOCK") == 0) {
		do_set_mlock(u, channel_id, param);
	} else if (stricmp(cmd, "KEEPTOPIC") == 0) {
		do_set_keeptopic(u, channel_id, param);
	} else if (stricmp(cmd, "TOPICLOCK") == 0) {
		do_set_topiclock(u, channel_id, param);
	} else if (stricmp(cmd, "PRIVATE") == 0) {
		do_set_private(u, channel_id, param);
	} else if (stricmp(cmd, "SECUREOPS") == 0) {
		do_set_secureops(u, channel_id, param);
	} else if (stricmp(cmd, "LEAVEOPS") == 0) {
		do_set_leaveops(u, channel_id, param);
	} else if (stricmp(cmd, "RESTRICTED") == 0) {
		do_set_restricted(u, channel_id, param);
	} else if (stricmp(cmd, "SECURE") == 0) {
		do_set_secure(u, channel_id, param);
	} else if (stricmp(cmd, "VERBOSE") == 0) {
		do_set_verbose(u, channel_id, param);
	} else if (stricmp(cmd, "NOEXPIRE") == 0) {
		do_set_noexpire(u, channel_id, param);
	} else if (stricmp(cmd, "AUTOLIMIT") == 0) {
		do_set_autolimit(u, channel_id, param);
	} else if (stricmp(cmd, "CLEARBANS") == 0) {
		do_set_clearbans(u, channel_id, param);
	} else if (stricmp(cmd, "REGTIME") == 0) {
		do_set_regtime(u, channel_id, param);
	} else {
		notice_lang(s_ChanServ, u, CHAN_SET_UNKNOWN_OPTION,
		    strupper(cmd));
		notice_lang(s_ChanServ, u, MORE_INFO, s_ChanServ, "SET");
	}
}

/*************************************************************************/

static void do_set_founder(User * u, unsigned int channel_id, char *param)
{
	MYSQL_ROW row;
	unsigned int nfounder_id, fields, rows;
	int flags;
	MYSQL_RES *result;
	char *name;
	
	nfounder_id = mysql_findnick(param);
	flags = 0;
	name = NULL;
	
	if (!nfounder_id) {
		notice_lang(s_ChanServ, u, NICK_X_NOT_REGISTERED, param);
		return;
	} else if (get_nick_status(nfounder_id) & NS_VERBOTEN) {
		notice_lang(s_ChanServ, u, NICK_X_FORBIDDEN, param);
		return;
	}
	if (check_chan_limit(nfounder_id) != -1 &&
	    !is_services_admin(u)) {
		notice_lang(s_ChanServ, u, CHAN_SET_FOUNDER_TOO_MANY_CHANS,
		    param);
		return;
	}

	set_chan_founder(channel_id, nfounder_id);

	/*
	 * It is possible that they have just set the channel's current
	 * successor as the founder.  If so then we need to nuke the successor.
	 */
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET successor=0 "
	    "WHERE channel_id=%u && founder=successor", channel_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT name, flags FROM channel WHERE channel_id=%u",
	    channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		name = sstrdup(row[0]);
		flags = atoi(row[1]);
	}

	mysql_free_result(result);

	log("%s: Changing founder of %s to %s by %s!%s@%s", s_ChanServ,
	    name, param, u->nick, u->username, u->host);
	notice_lang(s_ChanServ, u, CHAN_FOUNDER_CHANGED, name, param);
	snoop(s_ChanServ, "[FOUNDER] Changing founder of %s to %s by %s "
	    "(%s@%s)", name, param, u->nick, u->username, u->host);
	
	if (flags & CI_VERBOSE) {
		opnotice(s_ChanServ, name,
		    "%s has changed channel founder to %s", u->nick,
		    param);
	}

	free(name);
}

/*************************************************************************/

static void do_set_successor(User *u, unsigned int channel_id, char *param)
{
	unsigned int nick_id;
	char name[CHANMAX];
	
	if (param) {
		nick_id = mysql_findnick(param);
		
		if (!nick_id) {
			notice_lang(s_ChanServ, u, NICK_X_NOT_REGISTERED,
			    param);
			return;
		} else if (get_nick_status(nick_id) & NS_VERBOTEN) {
			notice_lang(s_ChanServ, u, NICK_X_FORBIDDEN, param);
			return;
		} else if (nick_id == get_founder_id(channel_id)) {
			notice_lang(s_ChanServ, u,
			    CHAN_SUCCESSOR_IS_FOUNDER);
			return;
		}
	} else {
		nick_id = 0;
	}

	set_chan_successor(channel_id, nick_id);
	get_chan_name(channel_id, name);

	if (nick_id) {

		notice_lang(s_ChanServ, u, CHAN_SUCCESSOR_CHANGED, name,
		    param);
		snoop(s_ChanServ, "[SUCCESSOR] Changing successor of %s to "
		    "%s by %s (%s@%s)", name, param, u->nick, u->username,
		    u->host);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s has changed "
			    "channel successor to %s", u->nick, param);
		}
	} else {
		notice_lang(s_ChanServ, u, CHAN_SUCCESSOR_UNSET, name);
		snoop(s_ChanServ, "[SUCCESSOR] Unsetting successor of %s "
		      "by %s (%s@%s)", name, u->nick, u->username, u->host);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name,
			    "%s has unset channel successor", u->nick);
		}
	}
}

/*************************************************************************/

static void do_set_password(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];
    char *esc_pass;

	get_chan_name(channel_id, name);

    esc_pass = smysql_escape_string(mysqlconn, param);
    
	if (!validate_password(u, esc_pass, name)) {
		notice_lang(s_ChanServ, u, MORE_OBSCURE_PASSWORD);
		return;
	}

	if (strlen(esc_pass) > PASSMAX - 1)	/* -1 for null byte */
		notice_lang(s_ChanServ, u, PASSWORD_TRUNCATED, PASSMAX - 1);

	set_chan_pass(channel_id, esc_pass);

	notice_lang(s_ChanServ, u, CHAN_PASSWORD_CHANGED_TO, name, esc_pass);

	if (get_access(u, channel_id) < ACCESS_FOUNDER) {
		log("%s: %s!%s@%s set password as Services admin for %s",
		    s_ChanServ, u->nick, u->username, u->host, name);
		snoop(s_ChanServ, "[PASS] %s (%s@%s) set password as "
		    "services admin for %s", u->nick, u->username, u->host,
		    name);
		if (WallSetpass) {
			wallops(s_ChanServ, "\2%s\2 set password as "
			    "Services admin for channel \2%s\2", u->nick,
			    name);
		}
	} else {
		snoop(s_ChanServ, "[PASS] %s (%s@%s) set password for %s",
		    u->nick, u->username, u->host, name);
	}
	if (get_chan_flags(channel_id) & CI_VERBOSE) {
		opnotice(s_ChanServ, name,
		    "%s has changed the channel password", u->nick);
	}
}

/*************************************************************************/

static void do_set_desc(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_desc;

	get_chan_name(channel_id, name);

	esc_desc = smysql_escape_string(mysqlconn, param);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET description='%s' WHERE channel_id=%u",
	    esc_desc, channel_id);
	mysql_free_result(result);

	free(esc_desc);

	notice_lang(s_ChanServ, u, CHAN_DESC_CHANGED, name, param);
	if (get_chan_flags(channel_id) & CI_VERBOSE) {
		opnotice(s_ChanServ, name, "%s has changed the channel "
		    "description to \"%s\"", u->nick, param);
	}
}

/*************************************************************************/

static void do_set_url(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_url;

	get_chan_name(channel_id, name);

	if (param) {
		esc_url = smysql_escape_string(mysqlconn, param);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE channel SET url='%s' WHERE channel_id=%u",
		    esc_url, channel_id);
		mysql_free_result(result);
		free(esc_url);

		notice_lang(s_ChanServ, u, CHAN_URL_CHANGED, name, param);
	} else {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE channel SET url='' WHERE channel_id=%u",
		    channel_id);
		mysql_free_result(result);

		notice_lang(s_ChanServ, u, CHAN_URL_UNSET, name);
	}

	if (get_chan_flags(channel_id) & CI_VERBOSE) {
		if (param) {
			opnotice(s_ChanServ, name,
			    "%s has changed the channel URL to \2%s\2",
			    u->nick, param);
		} else {
			opnotice(s_ChanServ, name,
			    "%s has unset the channel URL", u->nick);
		}
	}
}

/*************************************************************************/

static void do_set_email(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_email;

	get_chan_name(channel_id, name);

	if (param) {
		if (!validate_email(param)) {
			notice_lang(s_ChanServ, u, BAD_EMAIL, param);
			return;
		}
		
		esc_email = smysql_escape_string(mysqlconn, param);
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE channel SET email='%s' WHERE channel_id=%u",
		    esc_email, channel_id);
		mysql_free_result(result);
		free(esc_email);
		notice_lang(s_ChanServ, u, CHAN_EMAIL_CHANGED, name, param);
	} else {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE channel SET email='' WHERE channel_id=%u",
		    channel_id);
		mysql_free_result(result);
		notice_lang(s_ChanServ, u, CHAN_EMAIL_UNSET, name);
	}

	if (get_chan_flags(channel_id) & CI_VERBOSE) {
		if (param) {
			opnotice(s_ChanServ, name,
			    "%s has changed the channel E-mail to \2%s\2",
			    u->nick, param);
		} else {
			opnotice(s_ChanServ, name,
			    "%s has unset the channel E-mail", u->nick);
		}
	}
}

/*************************************************************************/

static void do_set_entrymsg(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_entrymsg;

	get_chan_name(channel_id, name);

	if (param) {
		esc_entrymsg = smysql_escape_string(mysqlconn, param);
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE channel SET entry_message='%s' "
		    "WHERE channel_id=%u", esc_entrymsg, channel_id);
		mysql_free_result(result);
		free(esc_entrymsg);
		notice_lang(s_ChanServ, u, CHAN_ENTRY_MSG_CHANGED, name,
		    param);
	} else {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE channel SET entry_message='' "
		    "WHERE channel_id=%u", channel_id);
		mysql_free_result(result);
		notice_lang(s_ChanServ, u, CHAN_ENTRY_MSG_UNSET, name);
	}

	if (get_chan_flags(channel_id) & CI_VERBOSE) {
		if (param) {
			opnotice(s_ChanServ, name, "%s has changed the "
			    "channel entry message to \"%s\"", u->nick,
			    param);
		} else {
			opnotice(s_ChanServ, name,
			    "%s has unset the channel entry message",
			    u->nick);
		}
	}
}

/*************************************************************************/

static void do_set_topic(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];
	unsigned int fields, rows;
	MYSQL_RES *result;
	Channel *c;
	char *esc_last_topic, *esc_last_topic_setter;

	get_chan_name(channel_id, name);
	c = findchan(name);

	if (!c) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, name);
		return;
	}

	esc_last_topic_setter = smysql_escape_string(mysqlconn, u->nick);
	
	if (param) {
		esc_last_topic = smysql_escape_string(mysqlconn, param);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE channel SET last_topic='%s', "
		    "last_topic_setter='%s', last_topic_time=%ld "
		    "WHERE channel_id=%u", esc_last_topic,
		    esc_last_topic_setter, c->topic_time, channel_id);
		mysql_free_result(result);
		free(esc_last_topic);
	} else {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE channel SET last_topic='' "
		    "last_topic_setter='%s', last_topic_time=%ld "
		    "WHERE channel_id=%u", esc_last_topic_setter,
		    c->topic_time, channel_id);
		mysql_free_result(result);
	}

	free(esc_last_topic_setter);

	if (c->topic) {
		free(c->topic);
		c->topic_time = time(NULL);
    }

	if (param)
		c->topic = sstrdup(param);
	else
		c->topic = NULL;

	strscpy(c->topic_setter, u->nick, NICKMAX);

	if (get_chan_flags(channel_id) & CI_VERBOSE)
		opnotice(s_ChanServ, name, "%s used SET TOPIC", u->nick);

  send_cmd(ServerName, "TBURST %ld %s %ld %s :%s", c->creation_time, name,
      c->topic_time, c->topic_setter, c->topic ? c->topic : "");

}

/*************************************************************************/

static void do_set_mlock(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX], modebuf[32];
	MYSQL_ROW row;
	int add;		/* 1 if adding, 0 if deleting, -1 if neither */
	int newlock_on, newlock_off, curlock_on, curlock_off;
	unsigned int fields, rows, newlock_limit, curlock_limit;
	char c;
	char *s, *end, *newlock_key, *curlock_key;
	MYSQL_RES *result;

	get_chan_name(channel_id, name);

	add = -1;
	newlock_on = 0;
	newlock_off = 0;
	newlock_limit = 0;
	newlock_key = NULL;

	if (!param) {
		syntax_error(s_ChanServ, u, "SET MLOCK",
		    CHAN_SET_MLOCK_SYNTAX);
		return;
	}

	if (stricmp(param, "OFF") == 0) {
		*param = '\0';
		add = 1;
		newlock_key = sstrdup("");
	}

	while (*param) {
		if (*param != '+' && *param != '-' && add < 0) {
			param++;
			continue;
		}
		switch ((c = *param++)) {
		case '+':
			add = 1;
			break;
		case '-':
			add = 0;
			break;
		case 'i':
			if (add) {
				newlock_on |= CMODE_i;
				newlock_off &= ~CMODE_i;
			} else {
				newlock_off |= CMODE_i;
				newlock_on &= ~CMODE_i;
			}
			break;
		case 'k':
			if (add) {
				if (!(s = strtok(NULL, " "))) {
					notice_lang(s_ChanServ, u,
					    CHAN_SET_MLOCK_KEY_REQUIRED);
					return;
				}
				if (newlock_key)
					free(newlock_key);
				newlock_key =
				    smysql_escape_string(mysqlconn, s);
				newlock_off &= ~CMODE_k;
			} else {
				if (newlock_key)
					free(newlock_key);
				newlock_key = sstrdup("");
				newlock_off |= CMODE_k;
			}
			break;
		case 'l':
			if (add) {
				if (!(s = strtok(NULL, " "))) {
					notice_lang(s_ChanServ, u,
					    CHAN_SET_MLOCK_LIMIT_REQUIRED);
					return;
				}
				if (atol(s) <= 0) {
					notice_lang(s_ChanServ, u,
					    CHAN_SET_MLOCK_LIMIT_POSITIVE);
					return;
				}
				newlock_limit = atol(s);
				newlock_off &= ~CMODE_l;
			} else {
				newlock_limit = 0;
				newlock_off |= CMODE_l;
			}
			break;
		case 'm':
			if (add) {
				newlock_on |= CMODE_m;
				newlock_off &= ~CMODE_m;
			} else {
				newlock_off |= CMODE_m;
				newlock_on &= ~CMODE_m;
			}
			break;
		case 'M':
			if (add) {
				newlock_on |= CMODE_M;
				newlock_off &= ~CMODE_M;
			} else {
				newlock_off |= CMODE_M;
				newlock_on &= ~CMODE_M;
			}
			break;
		case 'n':
			if (add) {
				newlock_on |= CMODE_n;
				newlock_off &= ~CMODE_n;
			} else {
				newlock_off |= CMODE_n;
				newlock_on &= ~CMODE_n;
			}
			break;
		case 'p':
			if (add) {
				newlock_on |= CMODE_p;
				newlock_off &= ~CMODE_p;
			} else {
				newlock_off |= CMODE_p;
				newlock_on &= ~CMODE_p;
			}
			break;
		case 's':
			if (add) {
				newlock_on |= CMODE_s;
				newlock_off &= ~CMODE_s;
			} else {
				newlock_off |= CMODE_s;
				newlock_on &= ~CMODE_s;
			}
			break;
		case 't':
			if (add) {
				newlock_on |= CMODE_t;
				newlock_off &= ~CMODE_t;
			} else {
				newlock_off |= CMODE_t;
				newlock_on &= ~CMODE_t;
			}
			break;
		case 'c':
			if (add) {
				newlock_on |= CMODE_c;
				newlock_off &= ~CMODE_c;
			} else {
				newlock_off |= CMODE_c;
				newlock_on &= ~CMODE_c;
			}
			break;

		case 'O':
			if (is_oper(u->nick)) {
				/*
				 * Only allow +/-O to be changed if they
				 * are an oper
				 */
				if (add) {
					newlock_on |= CMODE_O;
					newlock_off &= ~CMODE_O;
				} else {
					newlock_off |= CMODE_O;
					newlock_on &= ~CMODE_O;
				}
			} else {
				/* Keep it the same */
				if (get_mlock_on(channel_id) & CMODE_O) {
					newlock_on |= CMODE_O;
					newlock_off &= ~CMODE_O;
				}
				if (get_mlock_off(channel_id) & CMODE_O) {
					newlock_off |= CMODE_O;
					newlock_on &= ~CMODE_O;
				}
			}
			break;
		case 'R':
			if (add) {
				newlock_on |= CMODE_R;
				newlock_off &= ~CMODE_R;
			} else {
				newlock_off |= CMODE_R;
				newlock_on &= ~CMODE_R;
			}
			break;
		default:
			notice_lang(s_ChanServ, u,
			    CHAN_SET_MLOCK_UNKNOWN_CHAR, c);
			break;
		}		/* switch */
	}			/* while (*param) */

	if (add == -1) {
		syntax_error(s_ChanServ, u, "SET MLOCK",
		    CHAN_SET_MLOCK_SYNTAX);
		return;
	}

	/* Now that everything's okay, actually set the new mode lock. */
	if (newlock_key || (newlock_off & CMODE_k)) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE channel SET mlock_on=%d, mlock_off=%d, "
		    "mlock_limit=%u, mlock_key='%s' WHERE channel_id=%u",
		    newlock_on, newlock_off, newlock_limit, newlock_key,
		    channel_id);
	} else {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE channel SET mlock_on=%d, mlock_off=%d, "
		    "mlock_limit=%u, mlock_key='' WHERE channel_id=%u",
		    newlock_on, newlock_off, newlock_limit, channel_id);
	}
	mysql_free_result(result);

	if (newlock_key)
		free(newlock_key);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT mlock_on, mlock_off, mlock_limit, mlock_key "
	    "FROM channel WHERE channel_id=%u", channel_id);

	row = smysql_fetch_row(mysqlconn, result);

	curlock_on = atoi(row[0]);
	curlock_off = atoi(row[1]);
	curlock_limit = atoi(row[2]);
	curlock_key = sstrdup(row[3]);

	mysql_free_result(result);

	/* Tell the user about it. */
	end = modebuf;
	*end = 0;
	if (curlock_on || (curlock_key && curlock_key[0]) ||
	    curlock_limit)
		end += snprintf(end, sizeof(modebuf) - (end - modebuf),
		    "+%s%s%s%s%s%s%s%s%s%s%s%s",
		    (curlock_on & CMODE_i) ? "i" : "",
		    (curlock_key && curlock_key[0]) ? "k" : "",
		    (curlock_limit) ? "l" : "",
		    (curlock_on & CMODE_m) ? "m" : "",
		    (curlock_on & CMODE_M) ? "M" : "",
		    (curlock_on & CMODE_n) ? "n" : "",
		    (curlock_on & CMODE_p) ? "p" : "",
		    (curlock_on & CMODE_s) ? "s" : "",
		    (curlock_on & CMODE_t) ? "t" : "",
		    (curlock_on & CMODE_c) ? "c" : "",
		    (curlock_on & CMODE_O) ? "O" : "",
		    (curlock_on & CMODE_R) ? "R" : "");

	if (curlock_off)
		end += snprintf(end, sizeof(modebuf) - (end - modebuf),
		    "-%s%s%s%s%s%s%s%s%s%s%s%s",
		    (curlock_off & CMODE_i) ? "i" : "",
		    (curlock_off & CMODE_k) ? "k" : "",
		    (curlock_off & CMODE_l) ? "l" : "",
		    (curlock_off & CMODE_m) ? "m" : "",
		    (curlock_off & CMODE_M) ? "M" : "",
		    (curlock_off & CMODE_n) ? "n" : "",
		    (curlock_off & CMODE_p) ? "p" : "",
		    (curlock_off & CMODE_s) ? "s" : "",
		    (curlock_off & CMODE_t) ? "t" : "",
		    (curlock_off & CMODE_c) ? "c" : "",
		    (curlock_off & CMODE_O) ? "O" : "",
		    (curlock_off & CMODE_R) ? "R" : "");

	free(curlock_key);

	if (*modebuf) {
		notice_lang(s_ChanServ, u, CHAN_MLOCK_CHANGED, name,
		    modebuf);
	} else {
		notice_lang(s_ChanServ, u, CHAN_MLOCK_REMOVED, name);
	}

	if (get_chan_flags(channel_id) & CI_VERBOSE) {
		if (*modebuf) {
			opnotice(s_ChanServ, name,
			    "%s changed the channel mode lock to \2%s\2",
			    u->nick, modebuf);
		} else {
			opnotice(s_ChanServ, name,
			    "%s removed the channel mode lock", u->nick);
		}
	}

	/* Implement the new lock. */
	check_modes(name);
}

/*************************************************************************/

static void do_set_keeptopic(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];

	get_chan_name(channel_id, name);

	if (stricmp(param, "ON") == 0) {
		add_chan_flags(channel_id, CI_KEEPTOPIC);
		notice_lang(s_ChanServ, u, CHAN_SET_KEEPTOPIC_ON);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET KEEPTOPIC ON",
			    u->nick);
		}
	} else if (stricmp(param, "OFF") == 0) {
		remove_chan_flags(channel_id, CI_KEEPTOPIC);
		notice_lang(s_ChanServ, u, CHAN_SET_KEEPTOPIC_OFF);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET KEEPTOPIC OFF",
			    u->nick);
		}
	} else {
		syntax_error(s_ChanServ, u, "SET KEEPTOPIC",
		    CHAN_SET_KEEPTOPIC_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_topiclock(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];

	get_chan_name(channel_id, name);

	if (stricmp(param, "ON") == 0) {
		add_chan_flags(channel_id, CI_TOPICLOCK);
		notice_lang(s_ChanServ, u, CHAN_SET_TOPICLOCK_ON);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET TOPICLOCK ON",
			    u->nick);
		}
	} else if (stricmp(param, "OFF") == 0) {
		remove_chan_flags(channel_id, CI_TOPICLOCK);
		notice_lang(s_ChanServ, u, CHAN_SET_TOPICLOCK_OFF);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET TOPICLOCK OFF",
			    u->nick);
		}
	} else {
		syntax_error(s_ChanServ, u, "SET TOPICLOCK",
		    CHAN_SET_TOPICLOCK_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_private(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];

	get_chan_name(channel_id, name);

	if (stricmp(param, "ON") == 0) {
		add_chan_flags(channel_id, CI_PRIVATE);
		notice_lang(s_ChanServ, u, CHAN_SET_PRIVATE_ON);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET PRIVATE ON",
			    u->nick);
		}
	} else if (stricmp(param, "OFF") == 0) {
		remove_chan_flags(channel_id, CI_PRIVATE);
		notice_lang(s_ChanServ, u, CHAN_SET_PRIVATE_OFF);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET PRIVATE OFF",
			    u->nick);
		}
	} else {
		syntax_error(s_ChanServ, u, "SET PRIVATE",
		    CHAN_SET_PRIVATE_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_secureops(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];

	get_chan_name(channel_id, name);

	if (stricmp(param, "ON") == 0) {
		add_chan_flags(channel_id, CI_SECUREOPS);
		notice_lang(s_ChanServ, u, CHAN_SET_SECUREOPS_ON);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET SECUREOPS ON",
			    u->nick);
		}
	} else if (stricmp(param, "OFF") == 0) {
		remove_chan_flags(channel_id, CI_SECUREOPS);
		notice_lang(s_ChanServ, u, CHAN_SET_SECUREOPS_OFF);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET SECUREOPS OFF",
			    u->nick);
		}
	} else {
		syntax_error(s_ChanServ, u, "SET SECUREOPS",
		    CHAN_SET_SECUREOPS_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_leaveops(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];

	get_chan_name(channel_id, name);

	if (stricmp(param, "ON") == 0) {
		add_chan_flags(channel_id, CI_LEAVEOPS);
		notice_lang(s_ChanServ, u, CHAN_SET_LEAVEOPS_ON);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET LEAVEOPS ON",
			    u->nick);
		}
	} else if (stricmp(param, "OFF") == 0) {
		remove_chan_flags(channel_id, CI_LEAVEOPS);
		notice_lang(s_ChanServ, u, CHAN_SET_LEAVEOPS_OFF);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET LEAVEOPS OFF",
			    u->nick);
		}
	} else {
		syntax_error(s_ChanServ, u, "SET LEAVEOPS",
		    CHAN_SET_LEAVEOPS_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_restricted(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];

	get_chan_name(channel_id, name);

	if (stricmp(param, "ON") == 0) {
		add_chan_flags(channel_id, CI_RESTRICTED);
		if (get_chan_level(channel_id, CA_NOJOIN) < 0)
			set_chan_level(channel_id, CA_NOJOIN, 0);
		notice_lang(s_ChanServ, u, CHAN_SET_RESTRICTED_ON);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET RESTRICTED ON",
			    u->nick);
		}
	} else if (stricmp(param, "OFF") == 0) {
		remove_chan_flags(channel_id, CI_RESTRICTED);
		if (get_chan_level(channel_id, CA_NOJOIN) >= 0)
			set_chan_level(channel_id, CA_NOJOIN, -1);
		notice_lang(s_ChanServ, u, CHAN_SET_RESTRICTED_OFF);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET RESTRICTED OFF",
			    u->nick);
		}
	} else {
		syntax_error(s_ChanServ, u, "SET RESTRICTED",
		    CHAN_SET_RESTRICTED_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_secure(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];

	get_chan_name(channel_id, name);

	if (stricmp(param, "ON") == 0) {
		add_chan_flags(channel_id, CI_SECURE);
		notice_lang(s_ChanServ, u, CHAN_SET_SECURE_ON);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET SECURE ON",
			    u->nick);
		}
	} else if (stricmp(param, "OFF") == 0) {
		remove_chan_flags(channel_id, CI_SECURE);
		notice_lang(s_ChanServ, u, CHAN_SET_SECURE_OFF);
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name, "%s SET SECURE OFF",
			    u->nick);
		}
	} else {
		syntax_error(s_ChanServ, u, "SET SECURE",
		    CHAN_SET_SECURE_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_verbose(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];

	get_chan_name(channel_id, name);

	if (stricmp(param, "ON") == 0) {
		add_chan_flags(channel_id, CI_VERBOSE);
		notice_lang(s_ChanServ, u, CHAN_SET_VERBOSE_ON);
		opnotice(s_ChanServ, name, "%s SET VERBOSE ON", u->nick);
	} else if (stricmp(param, "OFF") == 0) {
		remove_chan_flags(channel_id, CI_VERBOSE);
		notice_lang(s_ChanServ, u, CHAN_SET_VERBOSE_OFF);
		opnotice(s_ChanServ, name, "%s SET VERBOSE OFF", u->nick);
	} else {
		syntax_error(s_ChanServ, u, "SET VERBOSE",
		    CHAN_SET_VERBOSE_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_noexpire(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];

	get_chan_name(channel_id, name);

	if (!is_services_admin(u)) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
		return;
	}
	if (stricmp(param, "ON") == 0) {
		add_chan_flags(channel_id, CI_NO_EXPIRE);
		notice_lang(s_ChanServ, u, CHAN_SET_NOEXPIRE_ON, name);
		log("NOEXPIRE: %s set NOEXPIRE ON for %s", u->nick, name);
		snoop(s_ChanServ, "[NOEXPIRE] %s set NOEXPIRE ON for %s",
		    u->nick, name);
		wallops(s_ChanServ, "%s set NOEXPIRE ON for %s",
		    u->nick, name);
	} else if (stricmp(param, "OFF") == 0) {
		remove_chan_flags(channel_id, CI_NO_EXPIRE);
		notice_lang(s_ChanServ, u, CHAN_SET_NOEXPIRE_OFF, name);
		log("NOEXPIRE: %s set NOEXPIRE OFF for %s", u->nick, name);
		snoop(s_ChanServ, "[NOEXPIRE] %s set NOEXPIRE OFF for %s",
		    u->nick, name);
		wallops(s_ChanServ, "%s set NOEXPIRE OFF for %s",
		    u->nick, name);
	} else {
		syntax_error(s_ChanServ, u, "SET NOEXPIRE",
		    CHAN_SET_NOEXPIRE_SYNTAX);
	}
}

/*************************************************************************/

static void do_set_autolimit(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];
	unsigned int limit_offset, limit_tolerance, limit_period;
	char *s;
	Channel *chan;

	limit_offset = limit_tolerance = limit_period = 0;

	get_chan_name(channel_id, name);

	/* Set some sane defaults */
	if (stricmp(param, "ON") == 0) {
		limit_offset = 5;
		limit_tolerance = 2;
		limit_period = 2;
	} else if (stricmp(param, "OFF") == 0) {
		limit_offset = 0;
	} else {		
		if (!(s = strtok(param, ":"))) {
			syntax_error(s_ChanServ, u, "SET AUTOLIMIT",
			    CHAN_SET_AUTOLIMIT_SYNTAX);
			return;
		}
		limit_offset = atoi(s);
	}
		
	/* if the offset is zero then we can disable it right now */	
	if (limit_offset <= 0) {
		set_chan_autolimit(channel_id, 0, 0, 0);

		if ((chan = findchan(name))) {
			chan->limit_offset = 0;
			chan->limit_tolerance = 0;
			chan->limit_period = 0;
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -l",
			    name);
		}

		notice_lang(s_ChanServ, u, CHAN_SET_AUTOLIMIT_DISABLED);

		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name,
			    "%s has disabled channel autolimit", u->nick);
		}
		return;
	}

	/* if limit_period hasn't been set by now then it is the full
	 * offset:tolerance:period format */
	if (limit_period == 0) {
		if (!(s = strtok(NULL, ":"))) {
			syntax_error(s_ChanServ, u, "SET AUTOLIMIT",
			    CHAN_SET_AUTOLIMIT_SYNTAX);
			return;
		}
		limit_tolerance = atoi(s);
	
		if (!(s = strtok(NULL, ""))) {
			syntax_error(s_ChanServ, u, "SET AUTOLIMIT",
			    CHAN_SET_AUTOLIMIT_SYNTAX);
			return;
		}
		limit_period = atoi(s);
	
		/* make sure they picked something sane... */
		if (limit_offset > 500)
			limit_offset = 500;

		if (limit_tolerance > 500)
			limit_tolerance = 500;
		if (limit_tolerance <= 0)
			limit_tolerance = 0;

		if (limit_offset <= limit_tolerance) {
			limit_tolerance = limit_offset - 1;
			notice_lang(s_ChanServ, u,
			    CHAN_SET_AUTOLIMIT_BAD_TOLERANCE,
			    limit_tolerance);
		}

		if (limit_period > 60)
			limit_period = 60;
		if (limit_period <= 0)
			limit_period = 1;
	}

	set_chan_autolimit(channel_id, limit_offset, limit_tolerance,
	    limit_period);
	
	if ((chan = findchan(name))) {
		chan->limit_offset = limit_offset;
		chan->limit_tolerance = limit_tolerance;
		chan->limit_period = limit_period;
			
		/* Update limit now. */
		check_autolimit(chan, time(NULL), 1);
	}

	notice_lang(s_ChanServ, u, CHAN_SET_AUTOLIMIT_CHANGED, limit_offset,
	    limit_tolerance, limit_period, limit_period == 1 ? "" : "s");

	if (get_chan_flags(channel_id) & CI_VERBOSE) {
		opnotice(s_ChanServ, name, "%s has set autolimit to "
		    "offset by %u with a tolerance of %u, checked every "
		    "%u minute%s", u->nick, limit_offset, limit_tolerance,
		    limit_period, limit_period == 1 ? "" : "s");
	}

}

/*************************************************************************/

static void do_set_clearbans(User *u, unsigned int channel_id, char *param)
{
	Channel *chan;
	char name[CHANMAX];
	unsigned int bantime;

	get_chan_name(channel_id, name);

	bantime = 0;
	
	if (!param) {
		syntax_error(s_ChanServ, u, "SET CLEARBANS",
		    CHAN_SET_CLEARBANS_SYNTAX);
		return;
	} else if (stricmp(param, "ON") == 0) {
		bantime = 60;
	} else if (stricmp(param, "OFF") == 0) {
		bantime = 0;
	} else {
		bantime = (unsigned int)atoi(param);
	}

	chan = findchan(name);

	if (bantime <= 0) {
		set_chan_bantime(channel_id, 0);

		if (chan)
			chan->bantime = 0;
		
		notice_lang(s_ChanServ, u, CHAN_SET_CLEARBANS_OFF);
		
		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name,
			    "%s has disabled ban clearance", u->nick);
		}
	} else {
		if (bantime > 1440)
			bantime = 1440;

		set_chan_bantime(channel_id, bantime);

		if (chan)
			chan->bantime = bantime;

		notice_lang(s_ChanServ, u, CHAN_SET_CLEARBANS_CHANGED,
		    bantime, bantime == 1 ? "" : "s");

		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, name,
			    "%s has set bans to clear after %u minute%s",
			    u->nick, bantime, bantime == 1 ? "" : "s");
		}
	}
}

/*************************************************************************/

static void do_set_regtime(User *u, unsigned int channel_id, char *param)
{
	char name[CHANMAX];
	struct tm regtime;
	time_t epoch_regtime, epoch_now;

	get_chan_name(channel_id, name);

	if (! is_services_admin(u)) {
		notice_lang(s_ChanServ, u, ACCESS_DENIED);
		return;
	}

	if (! param ||
	    strptime(param, "%Y-%m-%d-%H-%M-%S", &regtime) == NULL) {
		syntax_error(s_ChanServ, u, "SET REGTIME",
		    CHAN_SET_REGTIME_SYNTAX);
		return;
	}

	epoch_now = time(NULL);
	epoch_regtime = mktime(&regtime);

	if (epoch_regtime == -1 || epoch_regtime >= epoch_now) {
		syntax_error(s_ChanServ, u, "SET REGTIME",
		    CHAN_SET_REGTIME_SYNTAX);
		return;
	}

	set_chan_regtime(channel_id, epoch_regtime);
	snoop(s_ChanServ, "[REGTIME] %s altered channel registration time "
	    "for %s to %s", u->nick, name, param);
	wallops(s_ChanServ, "[REGTIME] %s altered channel registration "
	    "time for %s to %s", u->nick, name, param);
	log("%s, %s altered channel registration time for %s to %s",
	    s_ChanServ, u->nick, name, param);
	notice_lang(s_ChanServ, u, CHAN_SET_REGTIME, name, param);
}

static void do_set_floodserv(User *u, unsigned int channel_id, char *param)
{
  char name[CHANMAX];

  get_chan_name(channel_id, name);
  if(stricmp(param, "ON") == 0)
  {
    fs_add_chan(u, channel_id, name);
    notice_lang(s_ChanServ, u, CHAN_SET_FLOODSERV_ON, name);
  }
  else if(stricmp(param, "OFF") == 0)
  {
    fs_del_chan(u, channel_id, name);
    notice_lang(s_ChanServ, u, CHAN_SET_FLOODSERV_OFF, name);
  }
}

/*************************************************************************/
/*************************************************************************/

static void do_akick(User * u)
{
	char base[BUFSIZE], buf[BUFSIZE], buf2[BUFSIZE], buf3[BUFSIZE];
	char buf4[BUFSIZE], vbase[BUFSIZE];
	ChannelInfo ci;
	NickInfo ni;
	MYSQL_ROW row;
	time_t t_added, t_last_used;
	unsigned int channel_id, fields, rows, count, del_nick_id;
	int level, sent_header;
	char *argv[3];
	char *chan, *cmd, *mask, *reason, *nick, *user, *host, *add_mask;
	char *esc_add_mask, *esc_reason, *esc_nick, *query, *esc_mask;
        char *vquery, *vmask;
	MYSQL_RES *result;
	struct tm *tm_added, *tm_used;
	Channel *c;
	struct c_userlist *cu, *next;

	chan = strtok(NULL, " ");
	cmd = strtok(NULL, " ");
	mask = strtok(NULL, " ");
	reason = strtok(NULL, "");
	ci.channel_id = 0;
	ni.nick_id = 0;
	add_mask = NULL;
	esc_add_mask = NULL;
	esc_reason = NULL;
	esc_nick = NULL;

	if (!cmd) {
		syntax_error(s_ChanServ, u, "AKICK", CHAN_AKICK_SYNTAX);
		return;
	}

	if (cmd && (stricmp(cmd, "LIST") == 0 ||
	    stricmp(cmd, "VIEW") == 0)) {
		level = CA_AKICK_LIST;
	} else {
		level = CA_AKICK;
	}

	if (!nick_identified(u) && (stricmp(cmd, "ADD") == 0 ||
	    stricmp(cmd, "DEL") == 0 || stricmp(cmd, "ENFORCE") == 0)) {
		notice_lang(s_ChanServ, u, NICK_IDENTIFY_REQUIRED,
		    s_ChanServ);
		return;
	}
	
	if (!mask && (stricmp(cmd, "ADD") == 0 ||
	    stricmp(cmd, "DEL") == 0)) {
		syntax_error(s_ChanServ, u, "AKICK", CHAN_AKICK_SYNTAX);
	} else if (!(channel_id = mysql_findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (!(get_channelinfo_from_id(&ci, channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!check_access(u, channel_id, level) &&
	    !is_services_admin(u)) {
		if (ci.founder && mysql_getlink(ci.founder) == u->nick_id) {
			notice_lang(s_ChanServ, u, CHAN_IDENTIFY_REQUIRED,
			    s_ChanServ, chan);
		} else {
			notice_lang(s_ChanServ, u, ACCESS_DENIED);
		}

	} else if (stricmp(cmd, "ADD") == 0) {
		mysql_findnickinfo(&ni, mask);

		if (!ni.nick_id) {
			split_usermask(mask, &nick, &user, &host);
			add_mask = smalloc(strlen(nick) + strlen(user) +
			    strlen(host) + 3);
			sprintf(add_mask, "%s!%s@%s", nick, user, host);
			free(nick);
			free(user);
			free(host);
			esc_add_mask = smysql_escape_string(mysqlconn,
			    add_mask);
		} else if (ni.status & NS_VERBOTEN) {
			notice_lang(s_ChanServ, u, NICK_X_FORBIDDEN, mask);
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			return;
		}

		/* Check if the AKICK already exists. */
		if (ni.nick_id) {
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT 1 FROM akick "
			    "WHERE channel_id=%u && nick_id=%u",
			    channel_id, ni.nick_id);
		} else {
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT 1 FROM akick "
			    "WHERE channel_id=%u && nick_id=0 && mask='%s'",
			    channel_id, esc_add_mask);
		}

		mysql_free_result(result);
		
		if (rows) {
			notice_lang(s_ChanServ, u,
			    CHAN_AKICK_ALREADY_EXISTS,
			    ni.nick_id ? ni.nick : add_mask, chan);
			free(esc_add_mask);
			if (ni.nick_id)
				free_nickinfo(&ni);
			free_channelinfo(&ci);
			return;
		}

		count = chanakick_count(channel_id);
		
		if (count >= (unsigned int)CSAutokickMax) {
			free(esc_add_mask);
			if (ni.nick_id)
				free_nickinfo(&ni);
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_AKICK_REACHED_LIMIT,
			    CSAutokickMax);
			return;
		}

		if (reason) {
			esc_reason = smysql_escape_string(mysqlconn,
			    reason);
		} else {
			esc_reason = NULL;
		}

		esc_nick = smysql_escape_string(mysqlconn, u->nick);
		
		/* Add the AKICK. */

		if (ni.nick_id) {
			result = smysql_bulk_query(mysqlconn, &fields, &rows,
			    "INSERT INTO akick (akick_id, channel_id, "
			    "idx, mask, nick_id, reason, who, added, "
			    "last_used) VALUES (NULL, %u, %u, NULL, %u, "
			    "%s%s%s, '%s', %d, 0)", channel_id, count + 1,
			    ni.nick_id, esc_reason ? "'" : "",
			    esc_reason ? esc_reason : "NULL",
			    esc_reason ? "'" : "", esc_nick, time(NULL));
		} else {
			result = smysql_bulk_query(mysqlconn, &fields, &rows,
			    "INSERT INTO akick (akick_id, channel_id, "
			    "idx, mask, nick_id, reason, who, added, "
			    "last_used) VALUES (NULL, %u, %u, '%s', 0, "
			    "%s%s%s, '%s', %d, 0)", channel_id, count + 1,
			    esc_add_mask, esc_reason ? "'" : "",
			    esc_reason ? esc_reason : "NULL",
			    esc_reason ? "'" : "", esc_nick, time(NULL));
		}

		mysql_free_result(result);

		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, ci.name,
			    "%s added an AutoKick for \2%s\2 \"%s\"",
			    u->nick, ni.nick_id ? ni.nick : add_mask,
			    reason ? reason : "No reason supplied");
		}
		notice_lang(s_ChanServ, u, CHAN_AKICK_ADDED,
		    ni.nick_id ? ni.nick : add_mask, chan);

		if (add_mask)
			free(add_mask);
		if (esc_add_mask)
			free(esc_add_mask);
		if (esc_reason)
			free(esc_reason);
		if (esc_nick)
			free(esc_nick);
		if (ni.nick_id)
			free_nickinfo(&ni);

	} else if (stricmp(cmd, "DEL") == 0) {
		/*
		 * Only bother doing all these SELECTS if they have
		 * VERBOSE set -- otherwise we don't need the info.
		 */
		if (ci.flags & CI_VERBOSE) {
			if (isdigit(*mask) &&
			    strspn(mask, "1234567890,-") == strlen(mask)) {
				vmask = sstrdup(mask);
				snprintf(vbase, sizeof(vbase),
				    "SELECT akick.idx, akick.mask, "
				    "nick.nick, IF(nick.flags & %d, NULL, "
				    "nick.last_usermask) as umask, "
				    "akick.reason FROM akick LEFT JOIN nick "
				    "ON (akick.nick_id=nick.nick_id) WHERE "
				    "channel_id=%u && (", NI_HIDE_MASK,
				    channel_id);
				vquery = mysql_build_query(mysqlconn, vmask,
				    vbase, "akick.idx",
				    (unsigned int)CSAutokickMax);
				
				result = smysql_bulk_query(mysqlconn,
				    &fields, &rows,
				    "%s) ORDER BY akick.idx ASC", vquery);
				free(vquery);
				free(vmask);
			} else {
				if (!(del_nick_id = mysql_findnick(mask))) {
					esc_mask =
					    smysql_escape_string(mysqlconn,
					    mask);

					result = smysql_bulk_query(mysqlconn,
					    &fields, &rows, "SELECT "
					    "akick.idx, akick.mask, "
					    "nick.nick, IF(nick.flags & %d, "
					    "NULL, nick.last_usermask) "
					    "as umask, akick.reason FROM "
					    "akick LEFT JOIN nick ON "
					    "(akick.nick_id=nick.nick_id) "
					    "WHERE channel_id=%u && "
					    "akick.mask='%s' ORDER BY "
					    "akick.idx ASC", NI_HIDE_MASK,
					    channel_id, esc_mask);

					free(esc_mask);
				} else {
					result = smysql_bulk_query(mysqlconn,
					    &fields, &rows, "SELECT "
					    "akick.idx, akick.mask, "
					    "nick.nick, IF(nick.flags & "
					    "%d, NULL, nick.last_usermask) "
					    "as umask, akick.reason FROM "
					    "akick LEFT JOIN nick ON "
					    "(akick.nick_id=nick.nick_id) "
					    "WHERE channel_id=%u && "
					    "akick.nick_id=%u ORDER BY "
					    "akick.idx ASC", NI_HIDE_MASK,
					    channel_id, del_nick_id);
				}
			}

			while ((row = smysql_fetch_row(mysqlconn, result))) {
				opnotice(s_ChanServ, ci.name,
				    "%s removed AutoKick for \2%s\2 \"%s\"",
				    u->nick, row[2] ? row[2] : row[1],
				    row[4] ? row[4] : "No reason supplied");
			}

			mysql_free_result(result);
		}
		if (isdigit(*mask)
		    && strspn(mask, "1234567890,-") == strlen(mask)) {
			/*
			 * We have been given a list or range of indices
			 * of existing channel AKICKs to delete.
			 */
			snprintf(base, sizeof(base),
			    "DELETE FROM akick WHERE channel_id=%u && (",
			    channel_id);

			/*
			 * Build a query based on the above base query and
			 * matching the range or list of indices against
			 * the AKICK index for this channel.
			 */
			query = mysql_build_query(mysqlconn, mask, base,
			    "akick.idx", (unsigned int)CSAutokickMax);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s)", query);

			free(query);
		} else {
			/* Try as nick match first. */
			if (!(del_nick_id = mysql_findnick(mask))) {
				/* Let's try it as a mask. */
				esc_mask = smysql_escape_string(mysqlconn,
				    mask);
				
				result = smysql_bulk_query(mysqlconn,
				    &fields, &rows, "DELETE FROM akick "
				    "WHERE channel_id=%u && mask='%s'",
				    channel_id, esc_mask);

				free(esc_mask);
			} else {
				result = smysql_bulk_query(mysqlconn,
				    &fields, &rows, "DELETE FROM akick "
				    "WHERE channel_id=%u && nick_id=%u",
				    channel_id, del_nick_id);
			}
		}

		mysql_free_result(result);
			
		if (!rows) {
			notice_lang(s_ChanServ, u, CHAN_AKICK_NO_MATCH,
			    ci.name);
		} else if (rows == 1) {
			notice_lang(s_ChanServ, u, CHAN_AKICK_DELETED_ONE,
			    ci.name);
		} else {
			notice_lang(s_ChanServ, u,
			    CHAN_AKICK_DELETED_SEVERAL, rows, ci.name);
		}

		if (rows)
			renumber_akicks(channel_id);

	} else if (stricmp(cmd, "LIST") == 0) {

		if (chanakick_count(channel_id) == 0) {
			notice_lang(s_ChanServ, u, CHAN_AKICK_LIST_EMPTY,
			    chan);
			free_channelinfo(&ci);
			return;
		}
		if (mask && isdigit(*mask) &&
		    strspn(mask, "1234567890,-") == strlen(mask)) {
			/*
			 * We have been given a list or range of indices
			 * of existing channel AKICKs to match.
			 */
			snprintf(base, sizeof(base),
			    "SELECT akick.idx, akick.mask, nick.nick, "
			    "IF(nick.flags & %d, NULL, "
			    "nick.last_usermask) as umask, akick.reason "
			    "FROM akick LEFT JOIN nick ON "
			    "(akick.nick_id=nick.nick_id) WHERE "
			    "channel_id=%u && (", NI_HIDE_MASK, channel_id);

			/*
			 * Build a query based on the above base query and
			 * matching the range or list of indices against
			 * the AKICK index for this channel, order by the
			 * index.
			 */
			query = mysql_build_query(mysqlconn, mask, base,
			    "akick.idx", (unsigned int)CSAutokickMax);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s) ORDER BY akick.idx ASC", query);

			free(query);
		} else if (mask) {
			/* Wildcard match on nick or mask. */
			esc_mask = smysql_escape_string(mysqlconn, mask);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT akick.idx, akick.mask, "
			    "nick.nick, IF(nick.flags & %d, NULL, "
			    "nick.last_usermask) as umask, akick.reason "
			    "FROM akick LEFT JOIN nick ON "
			    "(akick.nick_id=nick.nick_id) WHERE "
			    "channel_id=%u && ((akick.nick_id!=0 && "
			    "IRC_MATCH('%s', nick.nick)) || "
			    "(akick.nick_id=0 && IRC_MATCH('%s', "
			    "akick.mask))) ORDER BY akick.idx ASC",
			    NI_HIDE_MASK, channel_id, esc_mask, esc_mask);
			free(esc_mask);
		} else {
			/* They want the whole list. */
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT akick.idx, akick.mask, "
			    "nick.nick, IF(nick.flags & %d, NULL, "
			    "nick.last_usermask) as umask, akick.reason "
			    "FROM akick LEFT JOIN nick ON "
			    "(akick.nick_id=nick.nick_id) WHERE "
			    "channel_id=%u ORDER BY akick.idx ASC",
			    NI_HIDE_MASK, channel_id);
		}

		sent_header = 0;

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			buf[0] = '\0';
			buf2[0] = '\0';

			if (!sent_header) {
				notice_lang(s_ChanServ, u,
				    CHAN_AKICK_LIST_HEADER, ci.name);
				sent_header = 1;
			}

			if (!row[1] && row[2]) {
				/*
				 * No mask - this is on a registered
				 * nick.
				 */
				if (!row[3]) {
					/* We have no umask. */
					strscpy(buf, row[2], sizeof(buf));
				} else {
					snprintf(buf, sizeof(buf),
					    "%s (%s)", row[2], row[3]);
				}
			} else {
				/* AKICK on a mask. */
				strscpy(buf, row[1], sizeof(buf));
			}

			if (row[4]) {
				/* We have a reason. */
				snprintf(buf2, sizeof(buf2), " (%s)",
				    row[4]);
			} else {
				buf2[0] = '\0';
			}

			notice_lang(s_ChanServ, u, CHAN_AKICK_LIST_FORMAT,
			    atoi(row[0]), buf, buf2);
		}

		mysql_free_result(result);

		if (!sent_header)
			notice_lang(s_ChanServ, u, CHAN_AKICK_NO_MATCH, chan);
	} else if (stricmp(cmd, "VIEW") == 0) {
		
		if (chanakick_count(channel_id) == 0) {
			notice_lang(s_ChanServ, u, CHAN_AKICK_LIST_EMPTY,
			    chan);
			free_channelinfo(&ci);
			return;
		}
		
		if (mask && isdigit(*mask) &&
		    strspn(mask, "1234567890,-") == strlen(mask)) {
			/*
			 * We have been given a list or range of indices
			 * of existing channel AKICKs to match.
			 */
			snprintf(base, sizeof(base),
			    "SELECT akick.idx, akick.mask, nick.nick, "
			    "IF(nick.flags & %d, NULL, "
			    "nick.last_usermask) as umask, akick.reason, "
			    "akick.who, akick.added, akick.last_used, "
			    "akick.last_matched FROM akick LEFT JOIN "
			    "nick ON (akick.nick_id=nick.nick_id) WHERE "
			    "channel_id=%u && (", NI_HIDE_MASK,
			    channel_id);

			/*
			 * Build a query based on the above base query and
			 * matching the range or list of indices against
			 * the AKICK index for this channel, order by the
			 * index.
			 */
			query = mysql_build_query(mysqlconn, mask, base,
			    "akick.idx", (unsigned int)CSAutokickMax);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s) ORDER BY akick.idx ASC", query);

			free(query);
		} else if (mask) {
			/* Wildcard match on nick or mask. */

			esc_mask = smysql_escape_string(mysqlconn, mask);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT akick.idx, akick.mask, "
			    "nick.nick, IF(nick.flags & %d, NULL, "
			    "nick.last_usermask) as umask, akick.reason, "
			    "akick.who, akick.added, akick.last_used, "
			    "akick.last_matched FROM akick LEFT JOIN "
			    "nick ON (akick.nick_id=nick.nick_id) WHERE "
			    "channel_id=%u && ((akick.nick_id!=0 && "
			    "IRC_MATCH('%s', nick.nick)) || "
			    "(akick.nick_id=0 && IRC_MATCH('%s', "
			    "akick.mask))) ORDER BY akick.idx ASC",
			    NI_HIDE_MASK, channel_id, esc_mask, esc_mask);
			free(esc_mask);
		} else {
			/* They want the whole list. */
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT akick.idx, akick.mask, "
			    "nick.nick, IF(nick.flags & %d, NULL, "
			    "nick.last_usermask) as umask, akick.reason, "
			    "akick.who, akick.added, akick.last_used, "
			    "akick.last_matched FROM akick LEFT JOIN "
			    "nick ON (akick.nick_id=nick.nick_id) WHERE "
			    "channel_id=%u ORDER BY akick.idx ASC",
			    NI_HIDE_MASK, channel_id);
		}

		sent_header = 0;

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			buf[0] = '\0';
			buf2[0] = '\0';
			buf3[0] = '\0';
			buf4[0] = '\0';
			t_added = atoi(row[6]);
			t_last_used = atoi(row[7]);

			if (!sent_header) {
				notice_lang(s_ChanServ, u,
				    CHAN_AKICK_LIST_HEADER, ci.name);
				sent_header = 1;
			}

			if (!row[1] && row[2]) {
				/*
				 * No mask - this is on a registered
				 * nick.
				 */
				if (!row[3]) {
					/* We have no umask. */
					strscpy(buf, row[2], sizeof(buf));
				} else {
					snprintf(buf, sizeof(buf),
					    "%s (%s)", row[2], row[3]);
				}
			} else {
				/* AKICK on a mask. */
				strscpy(buf, row[1], sizeof(buf));
			}

			if (row[4]) {
				/* We have a reason. */
				snprintf(buf2, sizeof(buf2), " (%s)",
				    row[4]);
			} else {
				buf2[0] = '\0';
			}

			tm_added = gmtime(&t_added);
			strftime_lang(buf3, sizeof(buf3), u,
			    STRFTIME_DATE_TIME_FORMAT, tm_added);

			if (t_last_used) {
				tm_used = gmtime(&t_last_used);
				strftime_lang(buf4, sizeof(buf4), u,
				    STRFTIME_DATE_TIME_FORMAT, tm_used);
			}

			if (row[5]) {
				notice_lang(s_ChanServ, u,
				    CHAN_AKICK_VIEW_FORMAT1, atoi(row[0]),
				    buf, row[5]);
			} else {
				notice_lang(s_ChanServ, u,
				    CHAN_AKICK_VIEW_FORMAT1_UNKNOWN,
				    atoi(row[0]), buf);
			}

			notice_lang(s_ChanServ, u, CHAN_AKICK_VIEW_FORMAT2,
			    buf2);
			notice_lang(s_ChanServ, u, CHAN_AKICK_VIEW_FORMAT3,
			    buf3, time_ago(t_added));

			if (t_last_used) {
				notice_lang(s_ChanServ, u,
				    CHAN_AKICK_VIEW_FORMAT4, buf4,
				    time_ago(t_last_used));
			} else {
				notice_lang(s_ChanServ, u,
				    CHAN_AKICK_VIEW_FORMAT4_NEVER);
			}

			if (row[8] && row[8][0]) {
				notice_lang(s_ChanServ, u,
				    CHAN_AKICK_VIEW_FORMAT5, row[8]);
			}

			notice(s_ChanServ, u->nick, " ");
		}

		mysql_free_result(result);

		if (!sent_header)
			notice_lang(s_ChanServ, u, CHAN_AKICK_NO_MATCH, chan);

	} else if (stricmp(cmd, "ENFORCE") == 0) {
		c = findchan(ci.name);
		cu = NULL;
		count = 0;

		log("debug: AKICK ENFORCE requested for %s by %s", ci.name,
		    u->nick);

		if (!c) {
			notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE,
			    ci.name);
			log("debug: ENFORCE: channel not in use.");
			free_channelinfo(&ci);
			return;
		}

		log("debug: ENFORCE: channel has users.");

		cu = c->users;
		while (cu) {
			log("debug: ENFORCE: checking user: %s",
			    cu->user->nick);
			next = cu->next;
			if (check_kick(cu->user, c->name)) {
				log("  debug: ENFORCE: building kick "
				    "args...");
				argv[0] = sstrdup(c->name);
				argv[1] = sstrdup(cu->user->nick);
				argv[2] = sstrdup(CSAutokickReason);

				log("  debug: ENFORCE: kicking user... "
				    "(%s)(%s)(%s)", argv[0], argv[1],
				    argv[2]);

				do_kick(s_ChanServ, 3, argv);

				log("  debug: ENFORCE: free'ing argv[]...");
				free(argv[2]);
				free(argv[1]);
				free(argv[0]);

				count++;
				log("  debug: ENFORCE: kick done!");
			} else {
				log("debug: ENFORCE: no kick needed.");
			}
			cu = next;
		}

		if (get_chan_flags(channel_id) & CI_VERBOSE) {
			opnotice(s_ChanServ, chan, "%s requested AKICK "
			    "ENFORCE", u->nick);
		}
		notice_lang(s_ChanServ, u, CHAN_AKICK_ENFORCE_DONE, chan,
		    count);

	} else if (stricmp(cmd, "COUNT") == 0) {
		notice_lang(s_ChanServ, u, CHAN_AKICK_COUNT, ci.name,
		    chanakick_count(channel_id));
	} else {
		syntax_error(s_ChanServ, u, "AKICK", CHAN_AKICK_SYNTAX);
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

static void do_levels(User * u)
{
	typedef struct {
		int need_def;
		int level;
	} LevelSet;

	static LevelSet levelset[CA_SIZE];
	ChannelInfo ci;
	MYSQL_ROW row;
	size_t len;
	unsigned int channel_id, fields, rows;
	int i, j;
	short level, oldlevel;
	char *chan, *cmd, *what, *s;
	MYSQL_RES *result;

	chan = strtok(NULL, " ");
	cmd = strtok(NULL, " ");
	what = strtok(NULL, " ");
	s = strtok(NULL, " ");
	ci.channel_id = 0;


	/* If SET, we want two extra parameters; if DIS[ABLE], we want only
	 * one; else, we want none.
	 */
	if (!cmd || ((stricmp(cmd, "SET") == 0) ? !s :
	    (strnicmp(cmd, "DIS", 3) == 0) ? (!what || s) : !!what)) {
		syntax_error(s_ChanServ, u, "LEVELS", CHAN_LEVELS_SYNTAX);
	} else if (!(channel_id = mysql_findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (!(get_channelinfo_from_id(&ci, channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (stricmp(cmd, "SET") == 0) {
		if (!check_access(u, channel_id, CA_LEVEL_CHANGE) &&
		    !is_services_admin(u)) {
			notice_lang(s_ChanServ, u, ACCESS_DENIED);
			free_channelinfo(&ci);
			return;
		}

		level = atoi(s);
		if (level <= ACCESS_INVALID || level >= ACCESS_FOUNDER) {
			notice_lang(s_ChanServ, u, CHAN_LEVELS_RANGE,
			    ACCESS_INVALID + 1, ACCESS_FOUNDER - 1);
			free_channelinfo(&ci);
			return;
		}
		for (i = 0; levelinfo[i].what >= 0; i++) {
			if (stricmp(levelinfo[i].name, what))
				continue;
			
			oldlevel = get_chan_level(channel_id,
			    levelinfo[i].what);
			if (ci.flags & CI_VERBOSE && oldlevel != level) {
				if (oldlevel == ACCESS_INVALID) {
					opnotice(s_ChanServ, chan,
					    "%s changed \2%s\2 level from "
					    "\2(Disabled)\2 to \2%d\2",
					    u->nick, levelinfo[i].name,
					    level);
				} else {
					opnotice(s_ChanServ, chan,
					    "%s changed \2%s\2 level from "
					    "\2%d\2 to \2%d\2", u->nick,
					    levelinfo[i].name, oldlevel,
					    level);
				}
			}
			set_chan_level(channel_id, levelinfo[i].what, level);
			notice_lang(s_ChanServ, u, CHAN_LEVELS_CHANGED,
			    levelinfo[i].name, chan, level);
			free_channelinfo(&ci);
			return;
		}
		notice_lang(s_ChanServ, u, CHAN_LEVELS_UNKNOWN, what,
		    s_ChanServ);

	} else if (stricmp(cmd, "DIS") == 0 || stricmp(cmd, "DISABLE") == 0) {
		if (!check_access(u, channel_id, CA_LEVEL_CHANGE) &&
		    !is_services_admin(u)) {
			notice_lang(s_ChanServ, u, ACCESS_DENIED);
			free_channelinfo(&ci);
			return;
		}

		for (i = 0; levelinfo[i].what >= 0; i++) {
			if (stricmp(levelinfo[i].name, what))
				continue;
			
			oldlevel = get_chan_level(channel_id,
			    levelinfo[i].what);
			if (ci.flags & CI_VERBOSE &&
			    oldlevel != ACCESS_INVALID) {
				opnotice(s_ChanServ, chan,
				    "%s changed \2%s\2 level from \2%d\2 "
				    "to \2(Disabled)\2", u->nick,
				    levelinfo[i].name, oldlevel);
				}
				set_chan_level(channel_id,
				    levelinfo[i].what, ACCESS_INVALID);
				notice_lang(s_ChanServ, u,
				    CHAN_LEVELS_DISABLED, levelinfo[i].name,
				    chan);
				free_channelinfo(&ci);
				return;
		}
		notice_lang(s_ChanServ, u, CHAN_LEVELS_UNKNOWN, what,
		    s_ChanServ);

	} else if (stricmp(cmd, "LIST") == 0) {
		if (!check_access(u, channel_id, CA_LEVEL_LIST) &&
		    !is_services_admin(u)) {
			notice_lang(s_ChanServ, u, ACCESS_DENIED);
			free_channelinfo(&ci);
			return;
		}

		notice_lang(s_ChanServ, u, CHAN_LEVELS_LIST_HEADER, chan);
		if (!levelinfo_maxwidth) {
			for (i = 0; levelinfo[i].what >= 0; i++) {
				len = strlen(levelinfo[i].name);

				if (len > levelinfo_maxwidth)
					levelinfo_maxwidth = len;
			}
		}

		/* Initialise levelset. */
		for (i = 0; i < CA_SIZE; i++) {
			levelset[i].need_def = 1;
			levelset[i].level = ACCESS_INVALID;
		}

		/*
		 * Go through the custom level settings we have and store
		 * them in our levelset.
		 */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT what, level FROM chanlevel WHERE channel_id=%u",
		    channel_id);

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			levelset[atoi(row[0])].need_def = 0;
			levelset[atoi(row[0])].level = atoi(row[1]);
		}

		mysql_free_result(result);

		for (i = 0; levelinfo[i].what >= 0; i++) {
			if (levelset[levelinfo[i].what].need_def == 1)
				j = lookup_def_level(levelinfo[i].what);
			else
				j = levelset[levelinfo[i].what].level;

			if (j == ACCESS_INVALID) {
				j = levelinfo[i].what;
				if (j == CA_AUTOOP || j == CA_AUTODEOP ||
				    j == CA_AUTOVOICE || j == CA_NOJOIN) {
					notice_lang(s_ChanServ, u,
					    CHAN_LEVELS_LIST_DISABLED,
					    levelinfo_maxwidth,
					    levelinfo[i].name);
				} else {
					notice_lang(s_ChanServ, u,
					    CHAN_LEVELS_LIST_DISABLED,
					    levelinfo_maxwidth,
					    levelinfo[i].name);
				}
			} else {
				notice_lang(s_ChanServ, u,
				    CHAN_LEVELS_LIST_NORMAL,
				    levelinfo_maxwidth, levelinfo[i].name,
				    j);
			}
		}

	} else if (stricmp(cmd, "RESET") == 0) {
		if (!check_access(u, channel_id, CA_LEVEL_CHANGE) &&
		    !is_services_admin(u)) {
			notice_lang(s_ChanServ, u, ACCESS_DENIED);
			free_channelinfo(&ci);
			return;
		}

		reset_levels(channel_id);
		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan, "%s used LEVELS RESET",
			    u->nick);
		}
		notice_lang(s_ChanServ, u, CHAN_LEVELS_RESET, chan);

	} else {
		syntax_error(s_ChanServ, u, "LEVELS", CHAN_LEVELS_SYNTAX);
	}
	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

/*
 * SADMINS and users, who have identified for a channel, can now cause its
 * Entry Message and Successor to be displayed by supplying the ALL parameter.
 * Syntax: INFO channel [ALL]
 * -TheShadow (29 Mar 1999)
 */

static void do_info(User *u)
{
	char buf[BUFSIZE];
	ChannelInfo ci;
	NickInfo ni;
	unsigned int channel_id;
	time_t tmtime;
	int is_servadmin, need_comma, show_all;
	char *chan, *param, *end;
	const char *commastr;
	struct tm *tm;
	Channel *c;

	chan = strtok(NULL, " ");
	param = strtok(NULL, " ");
	is_servadmin = is_services_admin(u);
	need_comma = 0;
	show_all = 0;
	ci.channel_id = 0;
	commastr = getstring(u->nick_id, COMMA_SPACE);

	if (!chan) {
		syntax_error(s_ChanServ, u, "INFO", CHAN_INFO_SYNTAX);
		return;
	}

	if (strlen(chan) > CHANMAX)
		chan[CHANMAX] = '\0';

	if (!(channel_id = mysql_findchan(chan)) ||
	    !get_channelinfo_from_id(&ci, channel_id)) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!ci.founder) {
		/* Paranoia... this shouldn't be able to happen */
		free_channelinfo(&ci);
		mysql_delchan(channel_id);
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else {

		/* Only show all the channel's settings to sadmins and founders. */
		if (param && stricmp(param, "ALL") == 0 &&
		    (is_servadmin || is_identified(u, channel_id)))
			show_all = 1;

		notice_lang(s_ChanServ, u, CHAN_INFO_HEADER, chan);
		get_nickinfo_from_id(&ni, ci.founder);
		assert(ni.nick_id);

		if ((ni.last_usermask && ni.last_usermask[0]) &&
		    (is_servadmin || !(ni.flags & NI_HIDE_MASK))) {
			notice_lang(s_ChanServ, u, CHAN_INFO_FOUNDER,
			    ni.nick, ni.last_usermask);
		} else {
			notice_lang(s_ChanServ, u, CHAN_INFO_NO_FOUNDER,
			    ni.nick);
		}

		free_nickinfo(&ni);
		
		if (show_all && ci.successor) {
			get_nickinfo_from_id(&ni, ci.successor);
			if ((ni.last_usermask && ni.last_usermask[0]) &&
			    (is_servadmin || !(ni.flags & NI_HIDE_MASK))) {
				notice_lang(s_ChanServ, u,
				    CHAN_INFO_SUCCESSOR, ni.nick,
				    ni.last_usermask);
			} else {
				notice_lang(s_ChanServ, u,
				    CHAN_INFO_NO_SUCCESSOR, ni.nick);
			}
			free_nickinfo(&ni);
		}

		notice_lang(s_ChanServ, u, CHAN_INFO_DESCRIPTION, ci.desc);
		tm = localtime(&ci.time_registered);
		tmtime = mktime(tm);
		strftime_lang(buf, sizeof(buf), u,
		    STRFTIME_DATE_TIME_FORMAT, tm);
		notice_lang(s_ChanServ, u, CHAN_INFO_TIME_REGGED, buf,
		   time_ago(tmtime));
		tm = localtime(&ci.last_used);
		tmtime = mktime(tm);
		strftime_lang(buf, sizeof(buf), u,
		    STRFTIME_DATE_TIME_FORMAT, tm);
		notice_lang(s_ChanServ, u, CHAN_INFO_LAST_USED, buf,
		    time_ago(tmtime));

		/*
		 * Do not show last_topic if channel is mlock'ed +s or +p, or if the
		 * channel's current modes include +s or +p. -TheShadow
		 */
		if ((ci.last_topic && ci.last_topic[0]) &&
		    !(ci.mlock_on & CMODE_s || ci.mlock_on & CMODE_p)) {
			c = findchan(ci.name);
			
			if (! (c && (c->mode & CMODE_s ||
			    c->mode & CMODE_p))) {
				notice_lang(s_ChanServ, u,
				    CHAN_INFO_LAST_TOPIC, ci.last_topic);
				notice_lang(s_ChanServ, u,
				    CHAN_INFO_TOPIC_SET_BY,
				    ci.last_topic_setter);
			}
		}

		if (ci.entry_message && ci.entry_message[0] && show_all)
			notice_lang(s_ChanServ, u, CHAN_INFO_ENTRYMSG,
			    ci.entry_message);
		if (ci.url && ci.url[0])
			notice_lang(s_ChanServ, u, CHAN_INFO_URL, ci.url);
		if (ci.email && ci.email[0])
			notice_lang(s_ChanServ, u, CHAN_INFO_EMAIL,
			    ci.email);
		end = buf;
		*end = 0;
		if (ci.flags & CI_PRIVATE) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s", getstring(u->nick_id,
			    CHAN_INFO_OPT_PRIVATE));
			need_comma = 1;
		}
		if (ci.flags & CI_KEEPTOPIC) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, CHAN_INFO_OPT_KEEPTOPIC));
			need_comma = 1;
		}
		if (ci.flags & CI_TOPICLOCK) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, CHAN_INFO_OPT_TOPICLOCK));
			need_comma = 1;
		}
		if (ci.flags & CI_SECUREOPS) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, CHAN_INFO_OPT_SECUREOPS));
			need_comma = 1;
		}
		if (ci.flags & CI_LEAVEOPS) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, CHAN_INFO_OPT_LEAVEOPS));
			need_comma = 1;
		}
		if (ci.flags & CI_RESTRICTED) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, CHAN_INFO_OPT_RESTRICTED));
			need_comma = 1;
		}
		if (ci.flags & CI_SECURE) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, CHAN_INFO_OPT_SECURE));
			need_comma = 1;
		}
		if (ci.flags & CI_VERBOSE) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "%s%s", need_comma ? commastr : "",
			    getstring(u->nick_id, CHAN_INFO_OPT_VERBOSE));
			need_comma = 1;
		}
		notice_lang(s_ChanServ, u, CHAN_INFO_OPTIONS,
		    *buf ? buf : getstring(u->nick_id, CHAN_INFO_OPT_NONE));
		end = buf;
		*end = 0;
		if (ci.mlock_on || (ci.mlock_key && ci.mlock_key[0]) ||
		    ci.mlock_limit) {
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "+%s%s%s%s%s%s%s%s%s%s%s%s",
			    (ci.mlock_on & CMODE_i) ? "i" : "",
			    (ci.mlock_key && ci.mlock_key[0]) ? "k" : "",
			    (ci.mlock_limit) ? "l" : "",
			    (ci.mlock_on & CMODE_m) ? "m" : "",
			    (ci.mlock_on & CMODE_M) ? "M" : "",
			    (ci.mlock_on & CMODE_n) ? "n" : "",
			    (ci.mlock_on & CMODE_p) ? "p" : "",
			    (ci.mlock_on & CMODE_s) ? "s" : "",
			    (ci.mlock_on & CMODE_t) ? "t" : "",
			    (ci.mlock_on & CMODE_c) ? "c" : "",
			    (ci.mlock_on & CMODE_O) ? "O" : "",
			    (ci.mlock_on & CMODE_R) ? "R" : "");
		}

		if (ci.mlock_off)
			end += snprintf(end, sizeof(buf) - (end - buf),
			    "-%s%s%s%s%s%s%s%s%s%s%s%s",
			    (ci.mlock_off & CMODE_i) ? "i" : "",
			    (ci.mlock_off & CMODE_k) ? "k" : "",
			    (ci.mlock_off & CMODE_l) ? "l" : "",
			    (ci.mlock_off & CMODE_m) ? "m" : "",
			    (ci.mlock_off & CMODE_M) ? "M" : "",
			    (ci.mlock_off & CMODE_n) ? "n" : "",
			    (ci.mlock_off & CMODE_p) ? "p" : "",
			    (ci.mlock_off & CMODE_s) ? "s" : "",
			    (ci.mlock_off & CMODE_t) ? "t" : "",
			    (ci.mlock_off & CMODE_c) ? "c" : "",
			    (ci.mlock_off & CMODE_O) ? "O" : "",
			    (ci.mlock_off & CMODE_R) ? "R" : "");

		if (*buf) {
			notice_lang(s_ChanServ, u, CHAN_INFO_MODE_LOCK,
			    buf);
		}

		if ((ci.flags & CI_NO_EXPIRE) && show_all)
			notice_lang(s_ChanServ, u, CHAN_INFO_NO_EXPIRE);

		if (ci.limit_offset) {
			notice_lang(s_ChanServ, u, CHAN_INFO_AUTOLIMIT,
			    ci.limit_offset, ci.limit_tolerance,
			    ci.limit_period,
			    ci.limit_period == 1 ? "" : "s");
		}

		if (ci.bantime) {
			notice_lang(s_ChanServ, u, CHAN_INFO_CLEARBANS,
				    ci.bantime, ci.bantime == 1 ? "" : "s");
		}
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

/* SADMINS can search for channels based on their CI_VERBOTEN and CI_NO_EXPIRE
 * status. This works in the same way as NickServ's LIST command.
 * Syntax for sadmins: LIST pattern [FORBIDDEN] [NOEXPIRE]
 * Also fixed CI_PRIVATE channels being shown to non-sadmins.
 * -TheShadow
 */

static void do_list(User * u)
{
	char buf[BUFSIZE];
	MYSQL_ROW row;
	unsigned int fields, rows;
	int is_servadmin, flags, total;
	/* CI_ flags - a chan must match one to qualify. */
	int matchflags;
	char noexpire_char = ' ';
	char *pattern, *keyword, *esc_pattern;
	MYSQL_RES *result;
	
	pattern = strtok(NULL, " ");
	is_servadmin = is_services_admin(u);
	matchflags = 0;

	if (CSListOpersOnly && (!u || !(u->mode & UMODE_o))) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
		return;
	}

	if (!pattern) {
		syntax_error(s_ChanServ, u, "LIST",
		    is_servadmin ? CHAN_LIST_SERVADMIN_SYNTAX :
		    CHAN_LIST_SYNTAX);
	} else {
		while (is_servadmin && (keyword = strtok(NULL, " "))) {
			if (stricmp(keyword, "FORBIDDEN") == 0)
				matchflags |= CI_VERBOTEN;
			if (stricmp(keyword, "NOEXPIRE") == 0)
				matchflags |= CI_NO_EXPIRE;
		}

		notice_lang(s_ChanServ, u, CHAN_LIST_HEADER, pattern);

		esc_pattern = smysql_escape_string(mysqlconn, pattern);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT COUNT(*) FROM channel WHERE (%d = 0 || "
		    "flags & %d) && (IRC_MATCH('%s', name) || "
		    "IRC_MATCH('%s', description)) && (%d != 0 || "
		    "!(flags & %d || flags & %d))", matchflags, matchflags,
		    esc_pattern, esc_pattern, is_servadmin, CI_PRIVATE,
		    CI_VERBOTEN);

		total = 0;
		while ((row = smysql_fetch_row(mysqlconn, result))) {
			total = atoi(row[0]);
		}

		mysql_free_result(result);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT name, description, flags FROM channel "
		    "WHERE (%d = 0 || flags & %d) && "
		    "(IRC_MATCH('%s', name) || "
		    "IRC_MATCH('%s', description)) && (%d != 0 || "
		    "!(flags & %d || flags & %d)) ORDER BY 'name' LIMIT %d",
		    matchflags, matchflags, esc_pattern, esc_pattern,
		    is_servadmin, CI_PRIVATE, CI_VERBOTEN, CSListMax);

		free(esc_pattern);

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			flags = atoi(row[2]);
			
			snprintf(buf, sizeof(buf), "%-20s  %s", row[0], row[1]);

			if (is_servadmin && (flags & CI_NO_EXPIRE))
				noexpire_char = '!';
			else
				noexpire_char = ' ';

			/*
			 * This can only be true for SADMINS - normal users
			 * will never get this far with a VERBOTEN channel.
			 * -TheShadow
			 */
			if (flags & CI_VERBOTEN) {
				snprintf(buf, sizeof(buf),
				    "%-20s  [Forbidden]", row[0]);
			}

			notice(s_ChanServ, u->nick, "  %c%s", noexpire_char, buf);
		}

		mysql_free_result(result);
		
		notice_lang(s_ChanServ, u, CHAN_LIST_END, rows, total);
	}
}

/*************************************************************************/

static void do_invite(User * u)
{
	ChannelInfo ci;
	char *chan;
	Channel *c;

	chan = strtok(NULL, " ");
	ci.channel_id = 0;

	if (!chan) {
		syntax_error(s_ChanServ, u, "INVITE", CHAN_INVITE_SYNTAX);
	} else if (!(c = findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
	} else if (!(get_channelinfo_from_id(&ci, c->channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!u || !check_access(u, ci.channel_id, CA_INVITE)) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	} else {
		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan, "%s used INVITE",
			    u->nick);
		}
		send_cmd(s_ChanServ, "INVITE %s %s", u->nick, chan);
	}
	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

static void do_op(User * u)
{
	ChannelInfo ci;
	char *av[3];
	char *chan, *op_params;
	Channel *c;
	User *target_user;
	struct u_chanlist *uc;

	chan = strtok(NULL, " ");
	op_params = strtok(NULL, " ");
	ci.channel_id = 0;

	if (!op_params)
		op_params = u->nick;

	if (!chan || !op_params) {
		syntax_error(s_ChanServ, u, "OP", CHAN_OP_SYNTAX);
	} else if (!(c = findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
	} else if (!(get_channelinfo_from_id(&ci, c->channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!nick_recognized(u)) {
		notice_lang(s_ChanServ, u, CHAN_MUST_IDENTIFY_NICK,
		    s_NickServ, s_NickServ);
	} else if (!u || !check_access(u, ci.channel_id, CA_OPDEOP)) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	} else {
		if (!(target_user = finduser(op_params))) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_OP_NICK_NOT_PRESENT,
			    op_params, ci.name);
			return;
		}

		for (uc = target_user->chans; uc; uc = uc->next) {
			if (irc_stricmp(chan, uc->chan->name) == 0)
				break;
		}
		if (!uc) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_OP_NICK_NOT_PRESENT,
			    op_params, ci.name);
			return;
		}

/* --Jeremy */
/*		if (((ci.flags & CI_SECURE) &&
		    !nick_identified(target_user)) ||
		    ((ci.flags & CI_SECUREOPS) &&
		    !check_access(target_user, ci.channel_id, CA_AUTOOP))) {
			notice_lang(s_ChanServ, u, PERMISSION_DENIED);
			free_channelinfo(&ci);
			return;
		}*/

		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan,
			    "OP command used for %s by %s", op_params,
			    u->nick);
		}
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", chan,
		    op_params);
		av[0] = chan;
		av[1] = sstrdup("+o");
		av[2] = op_params;
		do_cmode(s_ChanServ, 3, av);
		free(av[1]);
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

static void do_deop(User * u)
{
	ChannelInfo ci;
	char *av[3];
	char *chan, *deop_params;
	Channel *c;
	User *target_user;
	struct u_chanlist *uc;

	chan = strtok(NULL, " ");
	deop_params = strtok(NULL, " ");
	ci.channel_id = 0;

	if (!deop_params)
		deop_params = u->nick;

	if (!chan || !deop_params) {
		syntax_error(s_ChanServ, u, "DEOP", CHAN_DEOP_SYNTAX);
	} else if (!(c = findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
	} else if (!(get_channelinfo_from_id(&ci, c->channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!u || !check_access(u, ci.channel_id, CA_OPDEOP)) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	} else {
		if (!(target_user = finduser(deop_params))) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_OP_NICK_NOT_PRESENT,
			    deop_params, ci.name);
			return;
		}

		for (uc = target_user->chans; uc; uc = uc->next) {
			if (irc_stricmp(chan, uc->chan->name) == 0)
				break;
		}
		if (!uc) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_OP_NICK_NOT_PRESENT,
			    deop_params, ci.name);
			return;
		}

		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan,
			    "DEOP command used for %s by %s", deop_params,
			    u->nick);
		}
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -o %s", chan,
		    deop_params);
		av[0] = chan;
		av[1] = sstrdup("-o");
		av[2] = deop_params;
		do_cmode(s_ChanServ, 3, av);
		free(av[1]);
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

static void do_voice(User * u)
{
	ChannelInfo ci;
	int required_access;
	char *av[3];
	char *chan, *voice_params;
	Channel *c;
	User *target_user;
	struct u_chanlist *uc;

	chan = strtok(NULL, " ");
	voice_params = strtok(NULL, " ");
	ci.channel_id = 0;

	if (!voice_params)
		voice_params = u->nick;

	if (voice_params == u->nick || (stricmp(voice_params, u->nick) == 0))
		required_access = CA_AUTOVOICE;
	else
		required_access = CA_OPDEOP;

	if (!chan || !voice_params) {
		syntax_error(s_ChanServ, u, "VOICE", CHAN_VOICE_SYNTAX);
	} else if (!(c = findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
	} else if (!(get_channelinfo_from_id(&ci, c->channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!u || !check_access(u, ci.channel_id, required_access)) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	} else {

		if (!(target_user = finduser(voice_params))) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_OP_NICK_NOT_PRESENT,
			    voice_params, ci.name);
			return;
		}

		for (uc = target_user->chans; uc; uc = uc->next) {
			if (stricmp(chan, uc->chan->name) == 0)
				break;
		}
		if (!uc) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_OP_NICK_NOT_PRESENT,
			    voice_params, ci.name);
			return;
		}

		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +v %s", chan,
			 voice_params);
		av[0] = chan;
		av[1] = sstrdup("+v");
		av[2] = voice_params;
		do_cmode(s_ChanServ, 3, av);
		free(av[1]);
		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan,
			    "VOICE command used for %s by %s",
			    voice_params, u->nick);
		}
	}
	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

static void do_devoice(User * u)
{
	ChannelInfo ci;
	int required_access;
	char *av[3];
	char *chan, *devoice_params;
	Channel *c;
	User *target_user;
	struct u_chanlist *uc;

	chan = strtok(NULL, " ");
	devoice_params = strtok(NULL, " ");
	ci.channel_id = 0;

	if (!devoice_params)
		devoice_params = u->nick;

	if (devoice_params == u->nick ||
	    (stricmp(devoice_params, u->nick) == 0))
		required_access = CA_AUTOVOICE;
        else
		required_access = CA_OPDEOP;

	if (!chan || !devoice_params) {
		syntax_error(s_ChanServ, u, "DEVOICE", CHAN_DEVOICE_SYNTAX);
	} else if (!(c = findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
	} else if (!(get_channelinfo_from_id(&ci, c->channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!u || !check_access(u, ci.channel_id, required_access)) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	} else {

		if (!(target_user = finduser(devoice_params))) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_OP_NICK_NOT_PRESENT,
			    devoice_params, ci.name);
			return;
		}

		for (uc = target_user->chans; uc; uc = uc->next) {
			if (stricmp(chan, uc->chan->name) == 0)
				break;
		}
		if (!uc) {
			free_channelinfo(&ci);
			notice_lang(s_ChanServ, u, CHAN_OP_NICK_NOT_PRESENT,
			    devoice_params, ci.name);
			return;
		}

		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -v %s", chan,
			 devoice_params);
		av[0] = chan;
		av[1] = sstrdup("-v");
		av[2] = devoice_params;
		do_cmode(s_ChanServ, 3, av);
		free(av[1]);
		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan,
			         "DEVOICE command used for %s by %s",
			         devoice_params, u->nick);
		}
	}
	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

static void do_unban(User * u)
{
	ChannelInfo ci;
	size_t i, count;
	char *chan;
	Channel *c;
	char *av[3];
	Ban **bans;

	chan = strtok(NULL, " ");
	ci.channel_id = 0;

	if (!chan) {
		syntax_error(s_ChanServ, u, "UNBAN", CHAN_UNBAN_SYNTAX);
	} else if (!(c = findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
	} else if (!(get_channelinfo_from_id(&ci, c->channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!u || !check_access(u, ci.channel_id, CA_UNBAN)) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	} else {
		/* Save original ban info. */
		count = c->bancount;
		bans = smalloc(sizeof(Ban *) * count);
		memcpy(bans, c->newbans, sizeof(Ban *) * count);

		av[0] = chan;
		av[1] = sstrdup("-b");

		for (i = 0; i < count; i++) {
			if (match_usermask(bans[i]->mask, u)) {
				send_cmd(MODE_SENDER(ServerName),
				    "MODE %s -b %s", chan, bans[i]->mask);
				av[2] = sstrdup(bans[i]->mask);
				do_cmode(s_ChanServ, 3, av);
				free(av[2]);
			}
		}
		free(av[1]);
		free(bans);

		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan, "%s used UNBAN",
			    u->nick);
		}
		notice_lang(s_ChanServ, u, CHAN_UNBANNED, chan);
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

static void do_clear(User * u)
{
	char buf[256];
	ChannelInfo ci;
	size_t count, i;
	char *av[3];
	char *chan, *what;
	Channel *c;
	Ban **bans;
	struct c_userlist *cu, *next;

	chan = strtok(NULL, " ");
	what = strtok(NULL, " ");
	ci.channel_id = 0;

	if (!what) {
		syntax_error(s_ChanServ, u, "CLEAR", CHAN_CLEAR_SYNTAX);
	} else if (!(c = findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
	} else if (!(get_channelinfo_from_id(&ci, c->channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!u || !check_access(u, ci.channel_id, CA_CLEAR)) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	} else if (stricmp(what, "bans") == 0) {
		/* Save original ban info. */
		count = c->bancount;
		bans = smalloc(sizeof(Ban *) * count);

		for (i = 0; i < count; i++) {
			bans[i] = smalloc(sizeof(Ban));
			bans[i]->mask = sstrdup(c->newbans[i]->mask);
		}

		for (i = 0; i < count; i++) {
			av[0] = sstrdup(chan);
			av[1] = sstrdup("-b");
			av[2] = bans[i]->mask;
			send_cmd(MODE_SENDER(ServerName), "MODE %s %s :%s",
			    av[0], av[1], av[2]);
			do_cmode(s_ChanServ, 3, av);
			free(av[2]);
			free(av[1]);
			free(av[0]);
			free(bans[i]);
		}
		
		free(bans);

		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan, "%s cleared bans",
			    u->nick);
		}
		notice_lang(s_ChanServ, u, CHAN_CLEARED_BANS, chan);
	} else if (stricmp(what, "modes") == 0) {
		av[0] = chan;
		av[1] = sstrdup("-cimMnOpRstlk");

		if (c->key)
			av[2] = sstrdup(c->key);
		else
			av[2] = sstrdup("");

		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s", av[0],
		    av[1], av[2]);
		do_cmode(s_ChanServ, 3, av);
		free(av[2]);
		free(av[1]);
		check_modes(chan);

		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan, "%s cleared modes",
			    u->nick);
		}
		notice_lang(s_ChanServ, u, CHAN_CLEARED_MODES, chan);
	} else if (stricmp(what, "ops") == 0) {
		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan, "%s cleared ops",
			    u->nick);
		}

		for (cu = c->chanops; cu; cu = next) {
			next = cu->next;
			av[0] = sstrdup(chan);
			av[1] = sstrdup("-o");
			av[2] = sstrdup(cu->user->nick);
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
			    av[0], av[1], av[2]);
			do_cmode(s_ChanServ, 3, av);
			free(av[2]);
			free(av[1]);
			free(av[0]);
		}
		notice_lang(s_ChanServ, u, CHAN_CLEARED_OPS, chan);
	} else if (stricmp(what, "voices") == 0) {
		for (cu = c->voices; cu; cu = next) {
			next = cu->next;
			av[0] = sstrdup(chan);
			av[1] = sstrdup("-v");
			av[2] = sstrdup(cu->user->nick);
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
				 av[0], av[1], av[2]);
			do_cmode(s_ChanServ, 3, av);
			free(av[2]);
			free(av[1]);
			free(av[0]);
		}
		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan, "%s cleared voices",
			    u->nick);
		}
		notice_lang(s_ChanServ, u, CHAN_CLEARED_VOICES, chan);
	} else if (stricmp(what, "users") == 0) {
		snprintf(buf, sizeof(buf), "CLEAR USERS command from %s",
		    u->nick);

		for (cu = c->users; cu; cu = next) {
			next = cu->next;
			av[0] = sstrdup(chan);
			av[1] = sstrdup(cu->user->nick);
			av[2] = sstrdup(buf);
			send_cmd(MODE_SENDER(s_ChanServ), "KICK %s %s :%s",
			    av[0], av[1], av[2]);
			do_kick(s_ChanServ, 3, av);
			free(av[2]);
			free(av[1]);
			free(av[0]);
		}
		notice_lang(s_ChanServ, u, CHAN_CLEARED_USERS, chan);
	} else {
		syntax_error(s_ChanServ, u, "CLEAR", CHAN_CLEAR_SYNTAX);
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

static void do_sendpass(User * u)
{
	char buf[4096], cmdbuf[500], rstr[SALTMAX], salt[SALTMAX];
        char newpass[11], codebuf[PASSMAX + NICKMAX], code[PASSMAX];
	ChannelInfo ci;
	NickInfo ni;
	MYSQL_ROW row;
	unsigned int channel_id, fields, rows;
	char *chan, *authcode, *esc_authcode, *esc_chan;
	const char *mailfmt;
	FILE *fp;
	MYSQL_RES *result;

	chan = strtok(NULL, " ");
	ci.channel_id = 0;
	ni.nick_id = 0;

	if (!SendmailPath) {
		notice_lang(s_ChanServ, u, SENDPASS_UNAVAILABLE);
	} else if (!chan) {
		syntax_error(s_ChanServ, u, "SENDPASS",
		    CHAN_SENDPASS_SYNTAX);
	} else if (stricmp(chan, "AUTH") == 0) {
		/*
		 * They are trying to authorise an existing SENDPASS request.
		 * So, check it matches and look up the channel they want
		 * sendpassing.
		 */
		authcode = strtok(NULL, " ");

		if (!authcode) {
			syntax_error(s_ChanServ, u, "SENDPASS",
			    CHAN_SENDPASS_SYNTAX);
			return;
		}

		esc_authcode = smysql_escape_string(mysqlconn, authcode);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "SELECT name FROM auth "
		    "WHERE command='CHAN_SENDPASS' && code='%s'",
		    esc_authcode);


		if (!rows) {
			mysql_free_result(result);
			free(esc_authcode);
			notice_lang(s_ChanServ, u,
			    CHAN_SENDPASS_NO_SUCH_AUTH);
			return;
		}

		row = smysql_fetch_row(mysqlconn, result);

		if (!(get_channelinfo_from_id(&ci,
		    mysql_findchan(row[0])))) {
			notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED,
			    row[0]);
			mysql_free_result(result);
			return;
		}

		/* Don't need the mysql data anymore. */
		mysql_free_result(result);

		/* Nuke the AUTH request. */
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "DELETE FROM auth "
		    "WHERE command='CHAN_SENDPASS' && code='%s'",
		    esc_authcode);
		mysql_free_result(result);

		free(esc_authcode);

		/*
		 * Now we're ready to create a new password and mail it to
		 * them.
		 */
		get_nickinfo_from_id(&ni, ci.founder);

		if (!strlen(ni.email)) {
			notice_lang(s_ChanServ, u, CHAN_X_HAS_NO_EMAIL,
			    ni.nick, ci.name);
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			return;
		}

		make_random_string(salt, sizeof(salt));
		make_random_string(newpass, sizeof(newpass));

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "UPDATE channel SET founderpass=SHA1('%s%s'), "
		    "salt='%s', last_sendpass_pass='%s', "
		    "last_sendpass_salt='%s', last_sendpass_time=%d "
		    "WHERE channel_id=%u", newpass, salt, salt,
		    ci.founderpass, ci.salt, time(NULL), ci.channel_id);
		mysql_free_result(result);

		mailfmt = getstring(ni.nick_id, CHAN_SENDPASS_MAIL); 

		snprintf(cmdbuf, sizeof(cmdbuf), "%s -t", SendmailPath);
		snprintf(buf, sizeof(buf), mailfmt, CSMyEmail, ni.nick,
		    ni.email, AbuseEmail, CSMyEmail, ci.name, NetworkName,
		    ni.email, ci.name, newpass, AbuseEmail, NetworkName);

		log("%s: %s!%s@%s AUTH'd a SENDPASS on %s", s_ChanServ,
		    u->nick, u->username, u->host, ci.name);
		snoop(s_ChanServ,
		    "[SENDPASS] %s (%s@%s) AUTH'd SENDPASS for %s", u->nick,
		    u->username, u->host, ci.name);

		if ((fp = popen(cmdbuf, "w")) == NULL) {
			log("%s: Failed to create pipe for SENDPASS of %s",
			    s_ChanServ, chan);
			snoop(s_ChanServ, "[SENDPASS] popen() failed "
			    "during SENDPASS of %s", chan);
			notice_lang(s_ChanServ, u, SENDPASS_FAILED,
			    "couldn't create pipe");
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			return;
		}

		fputs(buf, fp);
		pclose(fp);
		notice_lang(s_ChanServ, u, CHAN_X_SENDPASSED, chan,
		    ni.nick, ni.email);
		
	} else if (!(channel_id = mysql_findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (!(get_channelinfo_from_id(&ci, channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else {
		/* They are trying to requst an AUTH code. */
		get_nickinfo_from_id(&ni, ci.founder);

		if (strlen(ni.email) <= 1) {
			notice_lang(s_ChanServ, u, CHAN_X_HAS_NO_EMAIL,
			    ni.nick, ci.name);
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			return;
		}

		if (ni.flags & NI_NOSENDPASS) {
			notice_lang(s_ChanServ, u, NICK_X_DISABLED_SENDPASS,
			    ni.nick);
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			return;
		}

		/*
		 * We now have all the details we need, but first need to
		 * check that there is no existing SENDPASS AUTH entry
		 * hanging around.  This does not apply to opers, because
		 * it is mainly intended as a rate-limiting feature.
		 */
		esc_chan = smysql_escape_string(mysqlconn, chan);

		if (!is_oper(u->nick)) {
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT 1 FROM auth "
			    "WHERE command='CHAN_SENDPASS' && name='%s'",
			    esc_chan);
			mysql_free_result(result);

			if (rows) {
				notice_lang(s_ChanServ, u,
				    CHAN_SENDPASS_ALREADY_REQUESTED,
				    ci.name);
				free(esc_chan);
				free_nickinfo(&ni);
				free_channelinfo(&ci);
				return;
			}
		}

		/*
		 * All seems to be well.  Now we need to send them the
		 * AUTH email.
		 */
		memset(codebuf, 0, sizeof(codebuf));
		make_random_string(rstr, sizeof(rstr));
		strncat(codebuf, u->nick, NICKMAX);
		strncat(codebuf, rstr, PASSMAX);
		make_sha_hash(codebuf, code);

		mailfmt = getstring(ni.nick_id, CHAN_SENDPASS_AUTH_MAIL);

		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "INSERT INTO auth SET auth_id='NULL', "
		    "code='%s', name='%s', command='CHAN_SENDPASS', "
		    "params='', create_time=%d", code, esc_chan,
		    time(NULL));

		free(esc_chan);
		mysql_free_result(result);

		snprintf(cmdbuf, sizeof(cmdbuf), "%s -t", SendmailPath);
		snprintf(buf, sizeof(buf), mailfmt, CSMyEmail, ni.nick,
		    ni.email, AbuseEmail, CSMyEmail, u->nick, u->username,
		    u->host, ci.name, NetworkName, code, AbuseEmail,
		    NetworkName);

		log("%s: %s!%s@%s used SENDPASS for %s, sending AUTH code "
		    "to %s", s_ChanServ, u->nick, u->username, u->host,
		    ci.name, ni.email);
		snoop(s_ChanServ,
		    "[SENDPASS] %s (%s@%s) used SENDPASS for %s, sending "
		    "AUTH code to %s", u->nick, u->username, u->host,
		    ci.name, ni.email);

		if ((fp = popen(cmdbuf, "w")) == NULL) {
			log("%s: Failed to create pipe for AUTH mail to %s",
			    s_ChanServ, ni.email);
			snoop(s_NickServ,
			    "[SENDPASS] popen() failed sending mail to %s",
			    ni.email);
			free_nickinfo(&ni);
			free_channelinfo(&ci);
			return;
		}

		fputs(buf, fp);
		pclose(fp);
		notice_lang(s_ChanServ, u, CHAN_SENDPASS_AUTH_MAILED,
		    ci.name, ni.nick);
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
	if (ni.nick_id)
		free_nickinfo(&ni);
}

/*************************************************************************/

static void do_forbid(User * u)
{
	ChannelInfo ci;
	unsigned int channel_id;
	char *chan;

	chan = strtok(NULL, " ");

	/* Assumes that permission checking has already been done. */
	if (!chan || chan[0] != '#') {
		syntax_error(s_ChanServ, u, "FORBID", CHAN_FORBID_SYNTAX);
		return;
	}

    if(!get_channelinfo_from_id(&ci, mysql_findchan(chan)))
	    notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);    

	/* Is the channel already forbidden ? */
	if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_ALREADY_FORBIDDEN, ci.name);
		return;
	}

	if ((channel_id = mysql_findchan(chan)))
		mysql_delchan(channel_id);

	strscpy(ci.name, chan, CHANMAX);
	ci.desc = sstrdup("");
	ci.founder = 0;
	ci.successor = 0;
	ci.founderpass[0] = 0;
	ci.url = 0;
	ci.email = 0;
	ci.last_used = ci.time_registered = time(NULL);
	ci.founder_memo_read = ci.last_used;
	ci.last_topic = 0;
	ci.last_topic_setter[0] = 0;
	ci.flags = CI_KEEPTOPIC | CI_SECURE;
	ci.mlock_on = CMODE_n | CMODE_t;
	ci.mlock_off = 0;
	ci.mlock_limit = 0;
	ci.mlock_key = "";
	ci.entry_message = 0;
	ci.last_limit_time = 0;
	ci.limit_offset = 0;
	ci.limit_tolerance = 0;
	ci.limit_period = 0;
	ci.bantime = 0;
	ci.last_sendpass_pass[0] = 0;
	ci.last_sendpass_salt[0] = 0;
	ci.last_sendpass_time = 0;
	
	ci.channel_id = insert_new_channel(&ci, NULL);
	
	if (ci.channel_id) {
		log("%s: %s set FORBID for channel %s", s_ChanServ,
		    u->nick, ci.name);
		add_chan_flags(ci.channel_id, CI_VERBOTEN);
		notice_lang(s_ChanServ, u, CHAN_FORBID_SUCCEEDED, ci.name);
		wallops(s_ChanServ, "\2%s\2 set FORBID for channel \2%s\2",
		    u->nick, ci.name);
		snoop(s_ChanServ, "[FORBID] %s (%s@%s) set FORBID for %s",
		    u->nick, u->username, u->host, ci.name);
		free(ci.desc);
	} else {
		log("%s: Valid FORBID for %s by %s failed", s_ChanServ,
		    chan, u->nick);
		notice_lang(s_ChanServ, u, CHAN_FORBID_FAILED, chan);
	}
}

/*************************************************************************/

static void do_status(User * u)
{
	ChannelInfo ci;
	User *u2;
	char *nick, *chan, *temp;

	chan = strtok(NULL, " ");
	nick = strtok(NULL, " ");
	ci.channel_id = 0;
	
	if (!nick || strtok(NULL, " ")) {
		notice(s_ChanServ, u->nick, "STATUS ERROR Syntax error");
		return;
	}

	if (!get_channelinfo_from_id(&ci, mysql_findchan(chan))) {
		temp = chan;
		chan = nick;
		nick = temp;

		get_channelinfo_from_id(&ci, mysql_findchan(chan));
	}

	if (!ci.channel_id) {
		notice(s_ChanServ, u->nick, "STATUS ERROR Channel %s not "
		    "registered", chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice(s_ChanServ, u->nick,
		    "STATUS ERROR Channel %s forbidden", chan);
	} else if ((u2 = finduser(nick)) != NULL) {
		notice(s_ChanServ, u->nick, "STATUS %s %s %d", chan, nick,
		    get_access(u2, ci.channel_id));
	} else {
		/* No such user. */
		notice(s_ChanServ, u->nick,
		    "STATUS ERROR Nick %s not online", nick);
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

static void do_sync(User * u)
{
	/* These store the large command lines for batched mode setting. */
	char bcmdbuf[256], ocmdbuf[256], vcmdbuf[256], nocmdbuf[256];
	char nvcmdbuf[256];
	/* And the mode characters (+o, +b etc). */
	char bmodebuf[7], omodebuf[7], vmodebuf[7], nomodebuf[7];
	char nvmodebuf[7];
	ChannelInfo ci;
	size_t i;
	int bcount, ocount, vcount, nocount, nvcount;
	Channel *c;
	char *chan;
	/* For checking userlist against +o/+v. */
	struct c_userlist *cu, *next;

	chan = strtok(NULL, " ");
	ci.channel_id = 0;

	if (!chan) {
		syntax_error(s_ChanServ, u, "SYNC", CHAN_SYNC_SYNTAX);
	} else if (!(c = findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
	} else if (!(get_channelinfo_from_id(&ci, c->channel_id))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci.flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (!u || !check_access(u, ci.channel_id, CA_SYNC)) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	} else {
		memset(bcmdbuf, 0, sizeof(bcmdbuf));
		memset(ocmdbuf, 0, sizeof(ocmdbuf));
		memset(vcmdbuf, 0, sizeof(vcmdbuf));
		memset(nocmdbuf, 0, sizeof(nocmdbuf));
		memset(nvcmdbuf, 0, sizeof(nvcmdbuf));
		memset(bmodebuf, 0, sizeof(bmodebuf));
		memset(omodebuf, 0, sizeof(omodebuf));
		memset(vmodebuf, 0, sizeof(vmodebuf));
		memset(nomodebuf, 0, sizeof(nomodebuf));
		memset(nvmodebuf, 0, sizeof(nvmodebuf));

		bcount = ocount = vcount = nocount = nvcount = 0;
		
		if (ci.flags & CI_VERBOSE) {
			opnotice(s_ChanServ, chan, "SYNC used by %s",
			    u->nick);
		}

		/* First reset channel modes. */

		/* We can't -k without using the key (-l is okay though). */
		send_cmd(MODE_SENDER(s_ChanServ),
		    "MODE %s -cimMnOpRstlk :%s", chan,
		    (c->key) ? c->key : "");
		
		/* Now put them back. */
		send_cmd(MODE_SENDER(s_ChanServ),
		     "MODE %s +%s%s%s%s%s%s%s%s%s%s", chan,
		     (c->mode & CMODE_c) ? "c" : "",
		     (c->mode & CMODE_i) ? "i" : "",
		     (c->mode & CMODE_m) ? "m" : "",
		     (c->mode & CMODE_M) ? "M" : "",
		     (c->mode & CMODE_n) ? "n" : "",
		     (c->mode & CMODE_O) ? "O" : "",
		     (c->mode & CMODE_p) ? "p" : "",
		     (c->mode & CMODE_R) ? "R" : "",
		     (c->mode & CMODE_s) ? "s" : "",
		     (c->mode & CMODE_t) ? "t" : "");

		/* Put back +k and +l. */
		if (c->key) {
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +k :%s",
			    chan, c->key);
		}

		if (c->limit) {
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +l :%d",
			    chan, c->limit);
		}

		/* Now reset the bans. */
		for (i = 0; i < c->bancount; i++, bcount++) {
			if (bcount == 6 ||
			    (strlen(bcmdbuf) + strlen(c->newbans[i]->mask)
			    + 1) > 256) {
				/*
				 * Time to send the command as we can't fit
				 * any more.
				 */
				bcount = 0;
				send_cmd(MODE_SENDER(s_ChanServ),
				    "MODE %s -%s %s", chan, bmodebuf,
				    bcmdbuf);
				send_cmd(MODE_SENDER(s_ChanServ),
				    "MODE %s +%s %s", chan, bmodebuf,
				    bcmdbuf);
				bmodebuf[0] = '\0';
				bcmdbuf[0] = '\0';
			}

			strcat(bmodebuf, "b");
			if (*bcmdbuf)
				strcat(bcmdbuf, " ");
			strcat(bcmdbuf, c->newbans[i]->mask);
		}

		/* Send any still left. */
		if (bcount) {
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -%s %s",
			    chan, bmodebuf, bcmdbuf);
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +%s %s",
			    chan, bmodebuf, bcmdbuf);
		}

		/* Now the ops, voices, and those without. */
		ocount = 0;
		vcount = 0;
		nocount = 0;
		nvcount = 0;
		
		for (cu = c->users; cu; cu = next) {
			next = cu->next;
			
			if (is_chanop(cu->user->nick, chan)) {
				ocount++;
				
				if (ocount == 6 ||
				    (strlen(ocmdbuf) +
				    strlen(cu->user->nick) + 1) > 256) {
					ocount = 0;
					
					send_cmd(MODE_SENDER(s_ChanServ),
					    "MODE %s -%s %s", chan,
					    omodebuf, ocmdbuf);
					send_cmd(MODE_SENDER(s_ChanServ),
					    "MODE %s +%s %s", chan,
					    omodebuf, ocmdbuf);
					omodebuf[0] = '\0';
					ocmdbuf[0] = '\0';
				}

				strcat(omodebuf, "o");
				if (*ocmdbuf)
					strcat(ocmdbuf, " ");
				strcat(ocmdbuf, cu->user->nick);
			} else {
				nocount++;
				
				if (nocount == 6 ||
				    (strlen(nocmdbuf) +
				    strlen(cu->user->nick) + 1) > 256) {
					nocount = 0;
					
					send_cmd(MODE_SENDER(s_ChanServ),
					    "MODE %s -%s %s", chan,
					    nomodebuf, nocmdbuf);
					nomodebuf[0] = '\0';
					nocmdbuf[0] = '\0';
				}

				strcat(nomodebuf, "o");
				if (*nocmdbuf)
					strcat(nocmdbuf, " ");
				strcat(nocmdbuf, cu->user->nick);
			}
			
			if (is_voiced(cu->user->nick, chan)) {
				vcount++;
				
				if (vcount == 6 ||
				    (strlen(vcmdbuf) +
				    strlen(cu->user->nick) + 1) > 256) {
					vcount = 0;
					
					send_cmd(MODE_SENDER(s_ChanServ),
					    "MODE %s -%s %s", chan,
					    vmodebuf, vcmdbuf);
					send_cmd(MODE_SENDER(s_ChanServ),
					    "MODE %s +%s %s", chan,
					    vmodebuf, vcmdbuf);
					vmodebuf[0] = '\0';
					vcmdbuf[0] = '\0';
				}

				strcat(vmodebuf, "v");
				if (*vcmdbuf)
					strcat(vcmdbuf, " ");
				strcat(vcmdbuf, cu->user->nick);
			} else {
				nvcount++;
				
				if (nvcount == 6 ||
				    (strlen(nvcmdbuf) +
				    strlen(cu->user->nick) + 1) > 256) {
					nvcount = 0;
					
					send_cmd(MODE_SENDER(s_ChanServ),
					    "MODE %s -%s %s", chan,
					    nvmodebuf, nvcmdbuf);
					nvmodebuf[0] = '\0';
					nvcmdbuf[0] = '\0';
				}

				strcat(nvmodebuf, "v");
				if (*nvcmdbuf)
					strcat(nvcmdbuf, " ");
				strcat(nvcmdbuf, cu->user->nick);
			}
		}

		if (ocount) {
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -%s %s",
			    chan, omodebuf, ocmdbuf);
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +%s %s",
			    chan, omodebuf, ocmdbuf);
		}

		if (nocount) {
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -%s %s",
			    chan, nomodebuf, nocmdbuf);
		}

		if (vcount) {
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -%s %s",
			    chan, vmodebuf, vcmdbuf);
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +%s %s",
			    chan, vmodebuf, vcmdbuf);
		}

		if (nvcount) {
			send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -%s %s",
			    chan, nvmodebuf, nvcmdbuf);
		}
	}

	if (ci.channel_id)
		free_channelinfo(&ci);
}

/*************************************************************************/

/* Set a channel's last used time to now */
static void update_chan_last_used(unsigned int channel_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	time_t now = time(NULL);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "UPDATE channel SET last_used=%d "
				   "WHERE channel_id=%u", now, channel_id);
	mysql_free_result(result);
}

/* Set an akick's last used time to now */
static void update_akick_last_used(unsigned int akick_id, User *u)
{
	char matchbuf[200];
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_matchbuf;
	
	snprintf(matchbuf, sizeof(matchbuf), "%s (%s@%s)", u->nick,
	    u->username, u->host);

	esc_matchbuf = smysql_escape_string(mysqlconn, matchbuf);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE akick SET last_used=%d, last_matched='%s' "
	    "WHERE akick_id=%u", time(NULL), esc_matchbuf, akick_id);
	mysql_free_result(result);
	free(esc_matchbuf);
}

/* return the timestamp for the last time the founder read their memos */
static time_t get_founder_memo_read(unsigned int channel_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	MYSQL_ROW row;
	time_t memoread = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT founder_memo_read FROM channel "
				   "WHERE channel_id=%u", channel_id);
	if ((row = smysql_fetch_row(mysqlconn, result))) {
		memoread = atoi(row[0]);
	}

	mysql_free_result(result);
	return(memoread);
}

/* return the timestamp for the last time the owner of a given channel
 * access entry read their memos */
static time_t get_access_memo_read(User *u, unsigned int channel_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	MYSQL_ROW row;
	time_t memoread = 0;

	if (!u || !u->nick_id)
		return(0);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT memo_read FROM chanaccess "
				   "WHERE nick_id=%u && channel_id=%u",
				   u->nick_id, channel_id);
	if ((row = smysql_fetch_row(mysqlconn, result))) {
		memoread = atoi(row[0]);
	}

	mysql_free_result(result);
	return(memoread);
}

/* copy the channel's name into the provided pointer, at most CHANMAX
 * characters will be copied */
void get_chan_name(unsigned int channel_id, char *name)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	MYSQL_ROW row;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT name FROM channel "
				   "WHERE channel_id=%u", channel_id);
	if (rows) {
		row = smysql_fetch_row(mysqlconn, result);
		strncpy(name, row[0], CHANMAX);
	} else {
		name = NULL;
	}

	mysql_free_result(result);
}

/*
 * Register a brand new channel
 */
static unsigned int insert_new_channel(ChannelInfo *ci, MemoInfo *mi)
{
	char salt[SALTMAX];
	unsigned int channel_id, fields, rows;
	MYSQL_RES *result;
	char *esc_name, *esc_desc, *esc_url, *esc_email, *esc_last_topic;
	char *esc_last_topic_setter, *esc_mlock_key, *esc_entry_message;
	char *esc_last_sendpass_pass, *esc_last_sendpass_salt;
	char *esc_founderpass;

	make_random_string(salt, sizeof(salt));
	
	esc_name = smysql_escape_string(mysqlconn, ci->name);
	esc_desc = smysql_escape_string(mysqlconn, ci->desc);
	esc_url = smysql_escape_string(mysqlconn, ci->url ? ci->url : "");
	esc_email = smysql_escape_string(mysqlconn,
	    ci->email ? ci->email : "");
	esc_last_topic = smysql_escape_string(mysqlconn,
	    ci->last_topic ? ci->last_topic : "");
	esc_last_topic_setter = smysql_escape_string(mysqlconn,
	    ci->last_topic_setter ? ci->last_topic_setter : "");
	esc_mlock_key = smysql_escape_string(mysqlconn,
	    ci->mlock_key ? ci->mlock_key : "");
	esc_entry_message = smysql_escape_string(mysqlconn,
	    ci->entry_message ? ci->entry_message : "");
	esc_last_sendpass_pass = smysql_escape_string(mysqlconn,
	    ci->last_sendpass_pass ? ci->last_sendpass_pass : "");
	esc_last_sendpass_salt = smysql_escape_string(mysqlconn,
	    ci->last_sendpass_salt ? ci->last_sendpass_salt : "");
	esc_founderpass = smysql_escape_string(mysqlconn, ci->founderpass);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO channel (channel_id, name, founder, successor, "
	    "founderpass, salt, description, url, email, time_registered, "
	    "last_used, founder_memo_read, last_topic, last_topic_setter, "
	    "last_topic_time, flags, mlock_on, mlock_off, mlock_limit, "
	    "mlock_key, entry_message, last_limit_time, limit_offset, "
	    "limit_tolerance, limit_period, bantime, last_sendpass_pass, "
	    "last_sendpass_salt, last_sendpass_time) "
	    "VALUES ('NULL', "	/* channel_id */
	    "'%s', "		/* name */
	    "'%u', "		/* founder */
	    "'%u', "		/* successor */
	    "SHA1('%s%s'), "	/* founderpass */
	    "'%s', "		/* salt */
	    "'%s', "		/* description */
	    "'%s', "		/* url */
	    "'%s', "		/* email */
	    "'%d', "		/* time_registered */
	    "'%d', "		/* last_used */
	    "'%d', "		/* founder_memo_read */
	    "'%s', "		/* last_topic */
	    "'%s', "		/* last_topic_setter */
	    "'%d', "		/* last_topic_time */
	    "'%d', "		/* flags */
	    "'%d', "		/* mlock_on */
	    "'%d', "		/* mlock_off */
	    "'%u', "		/* mlock_limit */
	    "'%s', "		/* mlock_key */
	    "'%s', "		/* entry_message */
	    "'%d', "		/* last_limit_time */
	    "'%u', "		/* limit_offset */
	    "'%u', "		/* limit_tolerance */
	    "'%u', "		/* limit_period */
	    "'%u', "		/* bantime */
	    "'%s', "		/* last_sendpass_pass */
	    "'%s', "		/* last_sendpass_salt */
	    "'%d')",		/* last_sendpass_time */
	    esc_name, ci->founder, ci->successor, esc_founderpass, salt,
	    salt, esc_desc, esc_url, esc_email, ci->time_registered,
	    ci->last_used, ci->founder_memo_read, esc_last_topic,
	    esc_last_topic_setter, ci->last_topic_time, ci->flags,
	    ci->mlock_on, ci->mlock_off, ci->mlock_limit, esc_mlock_key,
	    esc_entry_message, ci->last_limit_time, ci->limit_offset,
	    ci->limit_tolerance, ci->limit_period, ci->bantime,
	    esc_last_sendpass_pass, esc_last_sendpass_salt,
	    ci->last_sendpass_time);

	mysql_free_result(result);

	channel_id = mysql_insert_id(mysqlconn);

	if (mi) {
		result = smysql_bulk_query(mysqlconn, &fields, &rows,
		    "INSERT INTO memoinfo (memoinfo_id, owner, max) "
		    "VALUES ('NULL', '%s', %u)", esc_name, mi->max);
		mysql_free_result(result);
	}
	
	free(esc_last_sendpass_salt);
	free(esc_last_sendpass_pass);
	free(esc_entry_message);
	free(esc_mlock_key);
	free(esc_last_topic_setter);
	free(esc_last_topic);
	free(esc_email);
	free(esc_url);
	free(esc_desc);
	free(esc_name);

	return(channel_id);
}

/*
 * Return a channel's flags
 */
int get_chan_flags(unsigned int channel_id)
{
	MYSQL_ROW row;
	int flags;
	unsigned int fields, rows;
	MYSQL_RES *result;

	flags = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT flags FROM channel WHERE channel_id=%u", channel_id);

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		flags = atoi(row[0]);
	}

	mysql_free_result(result);

	return(flags);
}

/*
 * Add the specified flags to those a channel already has
 */
void add_chan_flags(unsigned int channel_id, int flags)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET flags=flags | %d WHERE channel_id=%u",
	    flags, channel_id);
	mysql_free_result(result);
}

/*
 * Remove the specified flags from those a channel already has
 */
void remove_chan_flags(unsigned int channel_id, int flags)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET flags=flags & ~%d WHERE channel_id=%u",
	    flags, channel_id);
	mysql_free_result(result);
}

/*
 * Returns a channel name given its ID, caller must free() after use
 */
char *get_channame_from_id(unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *name;

	name = NULL;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT name FROM channel WHERE channel_id=%u", channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		name = sstrdup(row[0]);
	}

	mysql_free_result(result);

	return(name);
}

/*
 * Delete everything regarding a channel from the databases
 */
static void mysql_delchan(unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows, nick_id, i, idcnt;
	MYSQL_RES *result;
	char *name, *esc_name;
	unsigned int *ids;

	ids = NULL;
	
	name = get_channame_from_id(channel_id);
	esc_name = smysql_escape_string(mysqlconn, name);

	nick_id = get_founder_id(channel_id);
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM channel WHERE channel_id=%u", channel_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM akick WHERE channel_id=%u", channel_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM chanaccess WHERE channel_id=%u", channel_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM chanlevel WHERE channel_id=%u", channel_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE suspend, chansuspend FROM suspend, chansuspend "
	    "WHERE suspend.suspend_id=chansuspend.suspend_id && "
	    "chansuspend.chan_id=%u", channel_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM memo WHERE owner='%s'", esc_name);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM memoinfo WHERE owner='%s'", esc_name);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT nick_id FROM autojoin WHERE channel_id=%u",
	    channel_id);

	if (rows) {
		idcnt = rows;
		ids = smalloc(sizeof(*ids) * idcnt);
	} else {
		idcnt = 0;
	}

	i = 0;
	
	while ((row = smysql_fetch_row(mysqlconn, result))) {
		ids[i] = atoi(row[0]);
		i++;
	}

	mysql_free_result(result);
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "DELETE FROM autojoin WHERE channel_id=%u", channel_id);
	mysql_free_result(result);

	for (i = 0; i < idcnt; i++) {
		renumber_autojoins(ids[i]);
	}

	free(ids);
	free(esc_name);
	free(name);
}

/*
 * Add the given modes to mlock_on for the specified channel
 */
void add_mlock_on(unsigned int channel_id, int modes)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET mlock_on=mlock_on | %d "
	    "WHERE channel_id=%u", modes, channel_id);
	mysql_free_result(result);
}

/*
 * Remove the given modes from mlock_off for the specified channel
 */
void remove_mlock_off(unsigned int channel_id, int modes)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET mlock_off=mlock_off | %d "
	    "WHERE channel_id=%u", modes, channel_id);
	mysql_free_result(result);
}

/*
 * Return a given channel's mlock_on setting
 */
static int get_mlock_on(unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	int mlock_on;
	MYSQL_RES *result;

	mlock_on = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT mlock_on FROM channel WHERE channel_id=%u",
	    channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		mlock_on = atoi(row[0]);
	}
	mysql_free_result(result);

	return(mlock_on);
}

/*
 * Return a given channel's mlock_off setting
 */
static int get_mlock_off(unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	int mlock_off;
	MYSQL_RES *result;

	mlock_off = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT mlock_off FROM channel WHERE channel_id=%u",
	    channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		mlock_off = atoi(row[0]);
	}
	mysql_free_result(result);

	return(mlock_off);
}

/*
 * Return a given channel's mlock_limit setting
 */
static unsigned int get_mlock_limit(unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows, mlock_limit;
	MYSQL_RES *result;

	mlock_limit = 0;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT mlock_limit FROM channel WHERE channel_id=%u",
	    channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		mlock_limit = atoi(row[0]);
	}
	mysql_free_result(result);

	return(mlock_limit);
}

/*
 * Return a given channel's mlock_key setting.  Caller must free()
 * afterwards.
 */
static char *get_mlock_key(unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *mlock_key;

	mlock_key = NULL;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT mlock_key FROM channel WHERE channel_id=%u",
	    channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		mlock_key = sstrdup(row[0]);
	}
	mysql_free_result(result);

	return(mlock_key);
}

/*
 * Return the channel's entry message, or NULL if it does not have one or
 * is unregistered.  Caller is responsible for free()ing.
 */
char *get_entrymsg(unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *msg;

	msg = NULL;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT entry_message FROM channel "
	    "WHERE channel_id=%u && LENGTH(entry_message)",
	    channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		msg = sstrdup(row[0]);
	}
	mysql_free_result(result);

	return(msg);
}

/*
 * Check a channel's password.  Return 1 if the password matches, 0
 * otherwise.
 */
static unsigned int check_chan_password(const char *pass,
    unsigned int channel_id)
{
	unsigned int fields, rows;
	MYSQL_RES *result;
	char *esc_pass;

	esc_pass = smysql_escape_string(mysqlconn, pass);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT 1 FROM channel WHERE channel_id=%u && "
	    "founderpass=SHA1(CONCAT('%s', salt))", channel_id, esc_pass);
	mysql_free_result(result);
	free(esc_pass);

	return(rows? 1 : 0);
}

/*
 * Set a given channel's founder ID.
 */
static void set_chan_founder(unsigned int channel_id,
    unsigned int nfounder_id)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET founder=%u WHERE channel_id=%u",
	    nfounder_id, channel_id);
	mysql_free_result(result);
}

/*
 * Set a given channel's successor ID.
 */
static void set_chan_successor(unsigned int channel_id,
    unsigned int nsuccessor_id)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET successor=%u WHERE channel_id=%u",
	    nsuccessor_id, channel_id);
	mysql_free_result(result);
}

/*
 * Set a given channel's password.
 */
static void set_chan_pass(unsigned int channel_id, const char *pass)
{
	char salt[SALTMAX];
	unsigned int fields, rows;
	MYSQL_RES *result;

	make_random_string(salt, sizeof(salt));

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET founderpass=SHA1('%s%s'), salt='%s' "
	    "WHERE channel_id=%u", pass, salt, salt, channel_id);
	mysql_free_result(result);
}

/*
 * Set a custom level setting for the given facility in the given channel.
 * Custom level settings go into the chanlevel table, replacing any rows for
 * that channel & facility that were there previously.
 */
static void set_chan_level(unsigned int channel_id, int what, int level)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "REPLACE INTO chanlevel (channel_id, what, level) "
	    "VALUES (%u, %d, %d)", channel_id, what, level);
	mysql_free_result(result);
}

/*
 * Set the given channel's ban clearance time (minutes).
 */
static void set_chan_bantime(unsigned int channel_id, unsigned int bantime)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET bantime=%u WHERE channel_id=%u", bantime,
	    channel_id);
	mysql_free_result(result);
}

/*
 * Set the given channel's registration time.
 */
static void set_chan_regtime(unsigned int channel_id, time_t regtime)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET time_registered=%d WHERE channel_id=%u",
	    regtime, channel_id);
	mysql_free_result(result);
}

/*
 * Set parameters related to autolimit for the given channel.  Also sets
 * the last autolimit update time to 0, to force an update.
 */
static void set_chan_autolimit(unsigned int channel_id,
    unsigned int offset, unsigned int tolerance, unsigned int period)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "UPDATE channel SET last_limit_time=0, limit_offset=%u, "
	    "limit_tolerance=%u, limit_period=%u WHERE channel_id=%u",
	    offset, tolerance, period, channel_id);
	mysql_free_result(result);
}

/*
 * Look up the default level for a given facility.
 */
static int lookup_def_level(int what)
{
	int i;

	for (i = 0; def_levels[i][0] >= 0; i++) {
		if (def_levels[i][0] == what)
			return(def_levels[i][1]);
	}

	return(ACCESS_INVALID);
}

/*
 * Retrieve all channel info at once and store in a ChannelInfo, a pointer to
 * which the caller should pass in.  This allocates storage for numerous
 * things; caller should use free_channelinfo() when finished.  Returns 1
 * if the channel was found, 0 otherwise.
 */
int get_channelinfo_from_id(ChannelInfo *ci, unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT name, founder, successor, founderpass, salt, "
	    "description, url, email, time_registered, last_used, "
	    "founder_memo_read, last_topic, last_topic_setter, "
	    "last_topic_time, flags, mlock_on, mlock_off, mlock_limit, "
	    "mlock_key, entry_message, last_limit_time, limit_offset, "
	    "limit_tolerance, limit_period, bantime, last_sendpass_pass, "
	    "last_sendpass_salt, last_sendpass_time, floodserv_protected FROM channel "
	    "WHERE channel_id=%u", channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		ci->channel_id = channel_id;
		strscpy(ci->name, row[0], CHANMAX);
		ci->founder = atoi(row[1]);
		ci->successor = atoi(row[2]);
		strscpy(ci->founderpass, row[3], PASSMAX);
		strscpy(ci->salt, row[4], SALTMAX);
		ci->desc = sstrdup(row[5]);
		ci->url = sstrdup(row[6]);
		ci->email = sstrdup(row[7]);
		ci->time_registered = atoi(row[8]);
		ci->last_used = atoi(row[9]);
		ci->founder_memo_read = atoi(row[10]);
		ci->last_topic = sstrdup(row[11]);
		strscpy(ci->last_topic_setter, row[12], NICKMAX);
		ci->last_topic_time = atoi(row[13]);
		ci->flags = atoi(row[14]);
		ci->mlock_on = atoi(row[15]);
		ci->mlock_off = atoi(row[16]);
		ci->mlock_limit = atoi(row[17]);
		ci->mlock_key = sstrdup(row[18]);
		ci->entry_message = sstrdup(row[19]);
		ci->last_limit_time = atoi(row[20]);
		ci->limit_offset = atoi(row[21]);
		ci->limit_tolerance = atoi(row[22]);
		ci->limit_period = atoi(row[23]);
		ci->bantime = atoi(row[24]);
		strscpy(ci->last_sendpass_pass, row[25], PASSMAX);
		strscpy(ci->last_sendpass_salt, row[26], SALTMAX);
		ci->last_sendpass_time = atoi(row[27]);
        ci->floodserv = atoi(row[28]);
	} else {
		ci->channel_id = 0;
	}

	mysql_free_result(result);
	return(rows? 1 : 0);
}

/*
 * Free storage previously allocated by get_channelinfo_from_id().
 */
void free_channelinfo(ChannelInfo *ci)
{
	if (!ci)
		return;

	if (ci->desc)
		free(ci->desc);
	if (ci->url)
		free(ci->url);
	if (ci->email)
		free(ci->email);
	if (ci->last_topic)
		free(ci->last_topic);
	if (ci->mlock_key)
		free(ci->mlock_key);
	if (ci->entry_message)
		free(ci->entry_message);
	ci->channel_id = 0;
}

/*
 * Renumber the idx column of a given channel's AKICK entries.
 */
static void renumber_akicks(unsigned int channel_id)
{
	MYSQL_RES *result;
	unsigned int fields, rows;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "TRUNCATE private_tmp_akick");
	mysql_free_result(result);
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO private_tmp_akick (akick_id, idx, mask, nick_id, "
	    "reason, who, added, last_used, last_matched) SELECT "
	    "akick_id, NULL, mask, nick_id, reason, who, added, last_used, "
	    "last_matched FROM akick WHERE channel_id=%u "
	    "ORDER BY added ASC", channel_id);
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "REPLACE INTO akick (akick_id, channel_id, idx, mask, "
	    "nick_id, reason, who, added, last_used, last_matched) "
	    "SELECT akick_id, %u, idx, mask, nick_id, reason, who, added, "
	    "last_used, last_matched FROM private_tmp_akick", channel_id);
	mysql_free_result(result);
}

/*
 * Count the number of AKICK entries for a given channel.
 */
static unsigned int chanakick_count(unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	unsigned int count;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT COUNT(akick_id) FROM akick WHERE channel_id=%u",
	    channel_id);
	row = smysql_fetch_row(mysqlconn, result);
	count = atoi(row[0]);
	mysql_free_result(result);

	return(count);
}

/*
 * Return the access level the given user has on the channel.  If the
 * channel doesn't exist, the user isn't on the access list, or the channel
 * is CS_SECURE and the user hasn't IDENTIFY'd with NickServ, return 0.
 */

int get_access(User * user, unsigned int channel_id)
{
    int level;
	if (!channel_id || !user->nick_id)
		return 0;
	if (is_founder(user, channel_id))
		return ACCESS_FOUNDER;
	if (nick_identified(user) || 
	    (nick_recognized(user) &&
	     !(get_chan_flags(channel_id) & CI_SECURE))) {
           level = get_chanaccess_level(channel_id, user->nick_id);
        return level;
	}
	return 0;
}

/*
 * Populate a ChanAccess structure from the nick_id of the person it
 * refers to.  Caller should use free_chanaccess() when finished.  Returns
 * 1 if the access was found, 0 otherwise.
 */
int get_chanaccess_from_nickid(ChanAccess *ca, unsigned int nick_id,
    unsigned int channel_id)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT chanaccess_id, idx, level, added, last_modified, "
	    "last_used, memo_read, added_by, modified_by FROM chanaccess "
	    "WHERE nick_id=%u && channel_id=%u", nick_id, channel_id);

	if ((row = smysql_fetch_row(mysqlconn, result))) {
		ca->nick_id = nick_id;
		ca->channel_id = channel_id;
		ca->chanaccess_id = atoi(row[0]);
		ca->idx = atoi(row[1]);
		ca->level = atoi(row[2]);
		ca->added = atoi(row[3]);
		ca->last_modified = atoi(row[4]);
		ca->last_used = atoi(row[5]);
		ca->memo_read = atoi(row[6]);
		strscpy(ca->added_by, row[7], NICKMAX);
		strscpy(ca->modified_by, row[8], NICKMAX);
	} else {
		ca->chanaccess_id = 0;
	}

	mysql_free_result(result);

	return(rows ? 1 : 0);
}

/*
 * Free the storage previously allocated by get_chanaccess_from_nickid().
 * NOTE: currently no storage is allocated so this function does nothing, but
 * using it will guard against later expansion of the ChanAccess structure.
 */
void free_chanaccess(ChanAccess *ca)
{
	USE_VAR(ca);
}

/* Return the access level required for a certain ability in the given
 * channel */
int get_level(unsigned int channel_id, int what)
{
	MYSQL_RES *result;
	unsigned int fields, rows;
	MYSQL_ROW row;
	int level;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
				   "SELECT level FROM chanlevel "
				   "WHERE channel_id=%u && what=%d",
				   channel_id, what);
	if (rows) {
		row = smysql_fetch_row(mysqlconn, result);
		level = atoi(row[0]);
	} else {
		/* they're using defaults */
		level = lookup_def_level(what);
	}

	mysql_free_result(result);
	return(level);
}

/*
 * Retrieve the level setting for the given facility in the given channel.
 * The chanlevel table only holds differences to the defaults, so if a row
 * is not found, the default can be used instead.
 */
int get_chan_level(unsigned int channel_id, int what)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	int level;
	MYSQL_RES *result;

	level = lookup_def_level(what);
	
	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT level FROM chanlevel WHERE channel_id=%u && what=%d",
	    channel_id, what);

	if (rows) {
		row = smysql_fetch_row(mysqlconn, result);
		level = atoi(row[0]);
	}

	mysql_free_result(result);

	return(level);
}

