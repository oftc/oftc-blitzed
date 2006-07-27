/* FloodServ core structures.
 *
 * Add an way to break flood attack,
 * -primary by checking flood from channel from a nick
 * -or from a host
 * -or from a ident
 * Copyright (c) 2001 Philippe Levesque <EMail: yagmoth555@yahoo.com>
 * See www.flamez.net for more info
 *
 * $Id$
 */

#include "services.h"
#include "tools.h"
#include "pseudo.h"
#include "floodserv.h"

// current extern function: floodserv, privmsg_flood, check_grname, fs_init
//							fs_add_chan

/******************************************************************/
/* Local Functions declarations                                   */
/******************************************************************/
void fs_add_akill(time_t expires, TxtFloods *pnt);
void add_floodtxt(User *u, char *buf, int id);
void add_floodtxt_user(User *u, TxtFloods *pnt);
void expire_floodtxt();
char *strstrip(char *d, const char *s);
void do_chan(User *u);
static void do_help(User *u);
void do_set(User *u);
void list_chan(User *u);
void list_floodtxt(User *u);
void rejoin_chan(User *u);
void check_channel();

/******************************************************************/
/* Declarations of our main list we will use                      */
/******************************************************************/
static dlink_list flood = {0,0,0};
//static Timeout  *Expire_Timeout = NULL;

/******************************************************************/
/* Cmd Struct Declaration                                         */
/******************************************************************/
static Command cmds[] = {
    { "HELP",		do_help,	NULL, -1, -1,-1,-1,-1 },
    { "CHAN",		do_chan,	is_services_oper, FLOOD_HELP_CHAN,		-1,-1,-1,-1 },
    { "SET",		do_set,		is_services_admin, FLOOD_HELP_SET,		-1,-1,-1,-1 },
    { NULL }
};


/*******************************************************************/
/* CheckUp Routine                                                 */
/* Called of PRIVMSG Check -> m_privmsg>Message.c				   */
/* av[0] = channel name                                            */
/* av[1] and so on = the text writted                              */
/*******************************************************************/
void privmsg_flood(const char *source, int ac, char **av)
{
	char *text;
	char text_d[BUFSIZE];
	dlink_node *txt_p;
    dlink_node *tmp;
	time_t now = time(NULL);
	time_t expires = FloodServAkillExpiry;
    Channel *chan = findchan(av[0]);
    

	User *u = finduser(source);
	if (!u || !chan) 
        return;
	
	if (is_oper(source))
		return;

	text = av[1];
	strstrip(text_d, text);

	expire_floodtxt();

	/* first check - does we monitor that channel ? - because oper can raw 
     * command a bot to join a channel. 
     * Here's an idea, lets NOT nest a whole load of shit */
    if(chan->floodserv == 0)
        return;
    
	DLINK_FOREACH_SAFE(txt_p, tmp, flood.head)
    {
        TxtFloods *txt = txt_p->data;
        if(txt->chanid != chan->channel_id)
            continue;
        if(stricmp(text_d, txt->txtbuffer)==0) 
        {
            add_floodtxt_user(u, txt);
            txt->repeat++;
            if (txt->repeat >= FloodServTFNumLines) 
            {
                /* he repeated more than the limit, but we need to check the 
                 * delay, thrusting expire_floodtxt is not safe - jabea */
                expires += now;
                if (now <= (txt->time + FloodServTFSec)) 
                {
                    if ((FloodServWarnFirst) && (!txt->warned)) 
                    {
                        txt->warned = 1;
                        wallops(s_FloodServ, "(FloodServ) Flood Detected: "
                                "(Text: [%45s]) (In: [\2%s\2]) (Last Said by:"
                                " [\2%s\2] (%s@%s)) has been said %d times in "
                                "less than %d seconds", text, av[0], u->nick, 
                                u->username, u->host, txt->repeat, 
                                FloodServTFSec);
                        kill_user(s_FloodServ, source, FloodServWarnMsg);
                    } 
					else 
                        fs_add_akill(expires, txt);
                } 
                else if ((now <= (txt->time + FloodServTFSecWarned)) && 
                        (txt->warned) && (txt->repeat >= FloodServTFNLWarned)) 
                    fs_add_akill(expires, txt);
            }
            return; /* found a match, might as well bail */
        }
    }
	add_floodtxt(u, text_d, chan->channel_id);
	return;
}



/*******************************************************************/
/* fs_add_akill : akill anyone that flooded (KEY by HOST)          */
/*******************************************************************/
void fs_add_akill(time_t expires, TxtFloods *pnt) 
{
	User *u, *unext;
	char buf[BUFSIZE];
	char timebuf[128];
	dlink_node *fnode;
	
    DLINK_FOREACH(fnode, pnt->flooder.head)
    {
        Flooders *fpnt = fnode->data;
        if (!fpnt->akilled) 
        {
            snprintf(buf, sizeof(buf), "*@%s", fpnt->host);
            if(add_akill(buf, FloodServAkillReason, s_FloodServ, expires)
                    != AKILL_PRESENT)
                /* if ImmediatelySendAKill is off, that give us heachache there...
                 * lets kill that poor guy & his bots with a homemade autokill, 
                 * and hope he sign again to make the autokill *finally* active
                 * argthh -jabea */
                if (!ImmediatelySendAkill) 
                {
                    /* here, we will try to find user matching that mask....  */
                    /* if there is a better way to do that, let me know -jabea */
                    u = firstuser();
                    while (u)
                    {
                        unext = nextuser();
                        if (stricmp(u->host, fpnt->host)==0) 
                            kill_user(s_FloodServ, u->nick, FloodServAkillReason);
                        u = unext;
                    }
                    /* end of search */
                }
            if (WallOSAkill) 
            {
                expires_in_lang(timebuf, sizeof(timebuf), u, expires);
                wallops(s_OperServ, "FloodServ added an AKILL for \2%s\2 (%s)", buf, timebuf);
            }
        }
        fpnt->akilled = 1;
    }
}


/*******************************************************************/
/* add_floodtxt : link a msg from a channel to a channel struct    */
/*******************************************************************/
void add_floodtxt(User *u, char *buf, int id)
{
	TxtFloods *txt  = NULL;
	dlink_node *pnt  = NULL;

    pnt = smalloc(sizeof(dlink_node));
    pnt->prev = NULL;
    pnt->next = NULL;
    pnt->data = NULL;
	
	txt = smalloc(sizeof(*txt));
	
	txt->repeat = 1;
	txt->warned = 0;
	txt->txtbuffer = sstrdup(buf);
	txt->time = time(NULL);
	txt->chanid = id;
    memset(&txt->flooder, 0, sizeof(txt->flooder));
    dlinkAdd(txt, pnt, &flood);
	
	add_floodtxt_user(u, txt);
}


/*******************************************************************/
/* add_floodtxt_user : link a user to a txtbuffer struct           */
/*******************************************************************/
void add_floodtxt_user(User *u, TxtFloods *txt)
{
	Flooders  *fdr;
    dlink_node *fnode;

    /* If the user is already linked to this struct, return now */
    DLINK_FOREACH(fnode, txt->flooder.head)
    {
        Flooders *fptr = fnode->data;
		if(stricmp(u->host, fptr->host)==0) 
			return;
    }
	
	fdr = smalloc(sizeof(*fdr));
    fnode = smalloc(sizeof(dlink_node));
    fnode->prev = NULL;
    fnode->next = NULL;
    fnode->data = NULL;

	strscpy(fdr->nick, u->nick, NICKMAX);
	fdr->host = sstrdup(u->host);
	fdr->akilled = 0;
    dlinkAdd(fdr, fnode, &txt->flooder);
}


/*******************************************************************/
/* expire_floodtxt : expire all non-flood msg & free up some mems  */
/*******************************************************************/
void expire_floodtxt(void)
{
	dlink_node *msg_node = NULL;
	dlink_node *flood_node = NULL;
    dlink_node *tmp = NULL, *tmp2 = NULL;
	time_t now = time(NULL);

    DLINK_FOREACH_SAFE(msg_node, tmp, flood.head)
    {
        TxtFloods *pnt = msg_node->data;
        if(!pnt)
            continue;
        if (((!pnt->warned) && (now > (pnt->time + FloodServTFSec))) ||  
                ((pnt->warned) && (now > (pnt->time + FloodServTFSecWarned)))) 
        {
            DLINK_FOREACH_SAFE(flood_node, tmp2, pnt->flooder.head)
            {
                Flooders *fpnt = flood_node->data;
                if (fpnt->host)
                    free(fpnt->host);
                dlinkDelete(flood_node, &pnt->flooder);
            }
            free(pnt->txtbuffer);
            dlinkDelete(msg_node, &flood);
            free(msg_node);
        }
	}
}


/*******************************************************************/
/* strstrip : strip anything from <= 0x32                          */
/*******************************************************************/
char *strstrip(char *d, const char *s)
{
    char *t = d;

    while (*s) {
		if (*s > '\040') {
			*d = *s;
			d++;
		}
		s++;
	}
    *d = 0;
    return t;
}


/*******************************************************************/
/* Main routine                                                    */
/*******************************************************************/
void floodserv(const char *source, char *buf)
{
    char *cmd;
    char *s;
    User *u = finduser(source);

    if (!u) {
		log("%s: user record for %s not found", s_FloodServ, source);
		notice_lang(s_FloodServ, u, USER_RECORD_NOT_FOUND);
		return;
	}

    log("%s: %s: %s", s_FloodServ, source, buf);
    
    cmd = strtok(buf, " ");
    if (!cmd) {
		return;
    } else if (stricmp(cmd, "\001PING") == 0) {
		if (!(s = strtok(NULL, "")))
			s = "\1";
		notice(s_FloodServ, source, "\001PING %s", s);
    } else {
		run_cmd(s_FloodServ, u, cmds, cmd);
    }
}

/*******************************************************************/
/* Handle a NOTICE sent to services							       */
/*******************************************************************/
void do_notice(const char *source, int ac, char **av)
{
    User *user;
    //char *s, *t;
    //struct u_chanlist *c;

    user = finduser(source);
    if (!user) {
		log("user: NOTICE from nonexistent user %s: %s", source, merge_args(ac, av));
		return;
    }


}


/*******************************************************************/
/* Handle directed channel commmand for floodserv			       */
/*******************************************************************/
void do_chan(User *u) {
#if 0
    char *cmd;
	char *channelname;
	int i;

    cmd = strtok(NULL, " ");
    if (!cmd)
	cmd = "";

	channelname = strtok(NULL, "");

	if (stricmp(cmd, "ADD") == 0) {
		if (!channelname) {
			send_cmd(s_FloodServ, "NOTICE %s :CHAN ADD \2ChannelName\2", u->nick);
			return;
		}
		/* Make sure mask does not already exist on the list. */
		for (i = 0; i < nchan && irc_stricmp(chanprotected[i].channame, channelname) != 0; i++)
	    ;
		if (i < nchan) {
			send_cmd(s_FloodServ, "NOTICE %s :Channel already exist on the list!", u->nick);
			return;
		}
//		add_chan(channelname);
		send_cmd(s_FloodServ, "NOTICE %s :Channel \2%s\2 Succesfully added!", u->nick, channelname);
		/*if (readonly)
			notice_lang(s_FloodServ, u, READ_ONLY_MODE);*/
	} else if (stricmp(cmd, "DEL") == 0) {
		if (!channelname) {
			send_cmd(s_FloodServ, "NOTICE %s :CHAN DEL \2ChannelName\2", u->nick);
			return;
		}
		if (del_chan(channelname)) {
			send_cmd(s_FloodServ, "NOTICE %s :Channel \2%s\2 Succesfully deleted!", u->nick, channelname);
/*			if (readonly)
				notice_lang(s_FloodServ, u, READ_ONLY_MODE);*/
	    } else {
			send_cmd(s_FloodServ, "NOTICE %s :Channel \2%s\2 not found!", u->nick, channelname);
	    }
	} else if (stricmp(cmd, "LIST") == 0) {
		list_chan(u);
	} else if (stricmp(cmd, "LISTFLOODTEXT") == 0) {
		list_floodtxt(u);
	} else if (stricmp(cmd, "COUNT") == 0) {
		send_cmd(s_FloodServ, "NOTICE %s :Number of channel(s) monitored: \2%d\2", u->nick, nchan);
	} else if (stricmp(cmd, "REJOIN") == 0) {
		rejoin_chan(u);
	} else if (stricmp(cmd, "UPDATE") == 0) {
		//check_channel();
	} else {
		send_cmd(s_FloodServ, "NOTICE %s :CHAN {ADD | DEL | LIST | COUNT | REJOIN | UPDATE } [channel name].", u->nick);
	}
#endif
}


/******************************************************************/
/* Help Function : Currently do *Nothing*                         */
/******************************************************************/
static void do_help(User *u)
{
    const char *cmd = strtok(NULL, "");

    if (!cmd) {
		notice_help(s_FloodServ, u, FLOOD_HELP);
    } else {
		help_cmd(s_FloodServ, u, cmds, cmd);
    }
}


/******************************************************************/
/* fs_init : currently only rejoin channel at start-up            */
/******************************************************************/
void fs_init(void)
{
}


/*******************************************************************/
/* Handle directed set commmand for floodserv			           */
/*******************************************************************/
void do_set(User *u) {
	char *cmd;
	char *s_value;
	int16 value;

    cmd = strtok(NULL, " ");
    if (!cmd)
	cmd = "";

	s_value = strtok(NULL, " ");

	if (stricmp(cmd, "WARN") == 0) {
		if (!s_value) {
			send_cmd(s_FloodServ, "NOTICE %s :SET \2WARN\2 ON/OFF", u->nick);
			return;
		}
		if (stricmp(s_value, "ON") == 0) {
			FloodServWarnFirst = 1;
			send_cmd(s_FloodServ, "NOTICE %s :\2WARN\2 now ON", u->nick);
		} else if (stricmp(s_value, "OFF") == 0) {
			FloodServWarnFirst = 0;
			send_cmd(s_FloodServ, "NOTICE %s :\2WARN\2 now OFF", u->nick);
		} else
			send_cmd(s_FloodServ, "NOTICE %s :SET \2WARN\2 ON/OFF", u->nick);
	} else if (stricmp(cmd, "TXTFLOODLINE") == 0) {
		if (!s_value) {
			send_cmd(s_FloodServ, "NOTICE %s :SET \2TXTFLOODLINE\2 [number >0]", u->nick);
			return;
		}
		value = atoi(s_value);
		if (value<=0) {
			send_cmd(s_FloodServ, "NOTICE %s :SET \2TXTFLOODLINE\2 [number >0]", u->nick);
			return;
		}
		FloodServTFNumLines = value;
		send_cmd(s_FloodServ, "NOTICE %s :TXTFLOODLINE \2%s\2 succesfully set!", u->nick, s_value);
	} else if (stricmp(cmd, "TXTFLOODSEC") == 0) {
		if (!s_value) {
			send_cmd(s_FloodServ, "NOTICE %s :SET \2TXTFLOODSEC\2 [number >0]", u->nick);
			return;
		}
		value = atoi(s_value);
		if (value<=0) {
			send_cmd(s_FloodServ, "NOTICE %s :SET \2TXTFLOODSEC\2 [number >0]", u->nick);
			return;
		}
		FloodServTFSec = value;
		send_cmd(s_FloodServ, "NOTICE %s :TXTFLOODSEC \2%s\2 succesfully set!", u->nick, s_value);
	} else if (stricmp(cmd, "TXTFLOODLWN") == 0) {
		if (!s_value) {
			send_cmd(s_FloodServ, "NOTICE %s :SET \2TXTFLOODLWN\2 [number >0]", u->nick);
			return;
		}
		value = atoi(s_value);
		if (value<=0) {
			send_cmd(s_FloodServ, "NOTICE %s :SET \2TXTFLOODLWN\2 [number >0]", u->nick);
			return;
		}
		FloodServTFNLWarned = value;
		send_cmd(s_FloodServ, "NOTICE %s :TXTFLOODLWN \2%s\2 succesfully set!", u->nick, s_value);
	} else if (stricmp(cmd, "TXTFLOODWARN") == 0) {
		if (!s_value) {
			send_cmd(s_FloodServ, "NOTICE %s :SET \2TXTFLOODWARN\2 [number >0]", u->nick);
			return;
		}
		value = atoi(s_value);
		if (value<=0) {
			send_cmd(s_FloodServ, "NOTICE %s :SET \2TXTFLOODWARN\2 [number >0]", u->nick);
			return;
		}
		FloodServTFSecWarned = value;
		send_cmd(s_FloodServ, "NOTICE %s :TXTFLOODWARN \2%s\2 succesfully set!", u->nick, s_value);
	} else if (stricmp(cmd, "VIEW") == 0) {
		send_cmd(s_FloodServ, "NOTICE %s :Current setting for \2%s\2", u->nick, s_FloodServ);
		if (FloodServWarnFirst == 0)
			send_cmd(s_FloodServ, "NOTICE %s :WARN         - Warn First \2OFF\2", u->nick);
		else 
			send_cmd(s_FloodServ, "NOTICE %s :WARN         - Warn First \2ON\2", u->nick);
		send_cmd(s_FloodServ, "NOTICE %s :TXTFLOODLINE - Number of lines: \2%d\2", u->nick, FloodServTFNumLines);
		send_cmd(s_FloodServ, "NOTICE %s :TXTFLOODSEC  - Number of seconds: \2%d\2", u->nick, FloodServTFSec);
		send_cmd(s_FloodServ, "NOTICE %s :[%d:%d lines/seconds]", u->nick, FloodServTFNumLines, FloodServTFSec);
		send_cmd(s_FloodServ, "NOTICE %s :TXTFLOODLWN  - Number of lines(warned): \2%d\2", u->nick, FloodServTFNLWarned);
		send_cmd(s_FloodServ, "NOTICE %s :TXTFLOODWARN - Number of seconds(warned): \2%d\2", u->nick, FloodServTFSecWarned);
		send_cmd(s_FloodServ, "NOTICE %s :[%d:%d lines/seconds, the real value is currently %d:%d]", u->nick, 
			FloodServTFNLWarned, FloodServTFSecWarned, FloodServTFNLWarned - FloodServTFNumLines, FloodServTFSecWarned - FloodServTFSec);
		send_cmd(s_FloodServ, "NOTICE %s :End of setting list (use SET to change them).", u->nick);
	} else {
		send_cmd(s_FloodServ, "NOTICE %s :SET {OPTION | VIEW } [value].", u->nick);
	}
}


/*******************************************************************/
/* list_floodtxt : list all msg stored in fs (for debugging mostly)*/
/*******************************************************************/
void list_floodtxt(User *u)
{
#if 0
    Flooders  *fpnt = NULL;
	TxtFloods *pnt = NULL;
	time_t now = time(NULL);

	send_cmd(s_FloodServ, "NOTICE %s :Current list of Text/Ctcp stored by FloodServ", u->nick);
	send_cmd(s_FloodServ, "NOTICE %s :---------------------------------------------", u->nick);
    for (pnt = flood_head ; pnt; pnt = pnt->next) 
    {
        if (((!pnt->warned) && (now > (pnt->time + FloodServTFSec))) ||  
                ((pnt->warned) && (now > (pnt->time + FloodServTFSecWarned)))) 
            send_cmd(s_FloodServ, "NOTICE %s :Text: \2%s\2 Repeated: %d *should"
                    " be expired*", u->nick, pnt->txtbuffer, pnt->repeat);
        else
            send_cmd(s_FloodServ, "NOTICE %s :Text: \2%s\2 Repeated: %d", u->nick, pnt->txtbuffer, pnt->repeat);
			
        fpnt = pnt->flooder;
        while (fpnt) 
        {
            send_cmd(s_FloodServ, "NOTICE %s :Said by \2%s\2 With last Nick \2%s\2", u->nick, fpnt->host, fpnt->nick);
            fpnt = fpnt->next;
        }
	}
#endif
}


/*******************************************************************/
/* rejoin_chan : rejoin chan (if kicked, or anything else          */
/*******************************************************************/
void rejoin_chan(User *u)
{
#if 0
    This needs to be redone *sigh*
	int i;

	for (i = 0; i < nchan; i++) {
		if (!is_on_chan(s_FloodServ, chanprotected[i].channame)) {
			send_cmd(ServerName, "SJOIN %lu %s + :%s", time(NULL), 
                    chanprotected[i].channame, s_FloodServ);
		}
	}
#endif
}


/*******************************************************************/
/* fs_add_chan : Command called from chanserv.c->do_set_floodserv  */
/*******************************************************************/
void fs_add_chan(User *u, unsigned int channel_id, const char *name)
{
    MYSQL_RES *result;
    unsigned int fields, rows;


    result = smysql_bulk_query(mysqlconn, &fields, &rows, 
            "UPDATE channel SET floodserv_protected=%u WHERE channel_id=%u",
            1, channel_id);
            
    if (!is_on_chan(s_FloodServ, name)) 
				send_cmd(ServerName, "SJOIN %i %s + :%s", time(NULL),
                        name, s_FloodServ);
    mysql_free_result(result);
}


/*******************************************************************/
/* fs_del_chan : Command called from chanserv.c->do_set_floodserv  */
/*******************************************************************/
void fs_del_chan(User *u, unsigned int channel_id, const char *name)
{
	MYSQL_RES *result;
    unsigned int fields, rows;

    result = smysql_bulk_query(mysqlconn, &fields, &rows,
            "UPDATE channel SET floodserv_protected=%u WHERE channel_id=%u",
            0, channel_id);
    
    send_cmd(NULL, ":%s PART %s", s_FloodServ, name);
    mysql_free_result(result);
    
}


/*******************************************************************/
/* check_channel : check if and quit any empty room                */
/*******************************************************************/
void check_channel()
{
}


/*************************************************************************/
/************************** INPUT/OUTPUT functions ***********************/
/*************************************************************************/
#define SAFE(x) do {					\
    if ((x) < 0) {					\
	if (!forceload)					\
	    fatal("Read error on %s", FloodServDBName);	\
	nchan = i;					\
	break;						\
    }							\
} while (0)

void load_fs_dbase(void)
{
/*    dbFILE *f;
    int i, ver;
    int16 tmp16;

    if (!(f = open_db(s_FloodServ, FloodServDBName, "r")))
	return;

    ver = get_file_version(f);

    read_int16(&tmp16, f);
    nchan = tmp16;
    if (nchan < 8)
	chan_size = 16;
    else if (nchan >= 16384)
	chan_size = 32767;
    else
	chan_size = 2*nchan;
    chanprotected = scalloc(sizeof(*chanprotected), chan_size);

    switch (ver) {
      case 11:
	for (i = 0; i < nchan; i++) {
	    SAFE(read_string(&chanprotected[i].channame, f));
		chanprotected[i].TxtFlood = NULL;
	}
	break;

      case -1:
	fatal("Unable to read version number from %s", FloodServDBName);

      default:
	fatal("Unsupported version (%d) on %s", ver, FloodServDBName);
    } *//* switch (version) */

/*    close_db(f);*/
}
