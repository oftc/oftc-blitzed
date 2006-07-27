
/*
 * Services -- main source file.
 * Blitzed Services copyright (c) 2000-2002 Blitzed Services team
 *     E-mail: services@lists.blitzed.org
 * Based on ircservices-4.4.8:
 * Copyright (c) 1996-1999 Andy Church <achurch@dragonfire.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file COPYING); if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "services.h"
#include "timeout.h"
#include "version.h"

RCSID("$Id$");

void sigusr2_handler(int sig_unused);
void sighandler(int signum);

/******** Global variables! ********/

/* Command-line options: (note that configuration variables are in config.c) */
char *services_dir = SERVICES_DIR;	/* -dir dirname */
char *etcdir = ETCDIR;
char *log_filename = LOG_FILENAME;	/* -log filename */
int debug = 0;			/* -debug */
int nofork = 0;			/* -nofork */
int forceload = 0;		/* -forceload */
int opt_noexpire = 0;		/* -noexpire */

/* Set to 1 if we are to quit */
int quitting = 0;

/* set to 1 if we are to restart */
int restarting = 0;

/* Set to 1 if we are to quit after saving databases */
int delayed_quit = 0;

/* Contains a message as to why services is terminating */
char *quitmsg = NULL;

/* Input buffer - global, so we can dump it if something goes wrong */
char inbuf[BUFSIZE];

/* Socket for talking to server */
int servsock = -1;

/* Should we force database update now? */
int save_data = 0;

/* At what time were we started? */
time_t start_time;

/* When did we last check channel event timers? */
volatile time_t last_chanevent;

/* When did we last run through expire on euth table? */
volatile time_t last_auth_expire;
int logtochan = 0;

/******** Local variables! ********/

/* Set to 1 if we are waiting for input */
static int waiting = 0;

/* Set to 1 after we've set everything up */
static int started = 0;

/* If we get a signal, use this to jump out of the main loop. */
sigjmp_buf panic_jmp;

/*************************************************************************/

/* signal handlers */

void sigusr2_handler(int sig_unused)
{
	USE_VAR(sig_unused);	/* just to get rid of compiler warning */
	log("Received SIGUSR2, cycling log file.");
	smysql_log("Received SIGUSR2, cycling log file.");
	snoop(s_OperServ, "[LOG] Received SIGUSR2, cycling log file.");
	close_log();
	close_mysql_log();
	open_log();
	open_mysql_log();

	signal(SIGUSR2, sigusr2_handler);
}

/* If we get a weird signal, come here. */

void sighandler(int signum)
{
	if (started) {
		if (signum == SIGHUP) {	/* SIGHUP = force database update and restart */
			save_data = -2;
			signal(SIGHUP, sighandler);
			log("Received SIGHUP, restarting.");
			if (!quitmsg)
				quitmsg = "Restarting on SIGHUP";
			quitting = 1;
			restarting = 1;
			siglongjmp(panic_jmp, 1);
		} else if (signum == SIGTERM) {
			save_data = 1;
			delayed_quit = 1;
			signal(SIGTERM, SIG_IGN);
			signal(SIGHUP, SIG_IGN);
			log("Received SIGTERM, exiting.");
			quitmsg = "Shutting down on SIGTERM";
			siglongjmp(panic_jmp, 1);
		} else if (signum == SIGINT || signum == SIGQUIT) {
			/* nothing -- terminate below */
		} else if (!waiting) {
			log("PANIC! buffer = %s", inbuf);
			/* Cut off if this would make IRC command >510 characters. */
			if (strlen(inbuf) > 448) {
				inbuf[446] = '>';
				inbuf[447] = '>';
				inbuf[448] = 0;
			}
			wallops(NULL, "PANIC! buffer = %s\r\n", inbuf);
		} else if (waiting < 0) {
			/* This is static on the off-chance we run low on stack */
			static char buf[BUFSIZE];

			switch (waiting) {
			case -1:
				snprintf(buf, sizeof(buf), "in timed_update");
				break;
			case -21:
				snprintf(buf, sizeof(buf),
					 "expiring nicknames");
				break;
			case -22:
				snprintf(buf, sizeof(buf),
					 "expiring channels");
				break;
			case -23:
				snprintf(buf, sizeof(buf),
					 "expiring nick suspends");
				break;
			case -25:
				snprintf(buf, sizeof(buf),
					 "expiring autokills & exceptions");
				break;
			case -4:
				snprintf(buf, sizeof(buf),
					 "expiring auth");
				break;
			default:
				snprintf(buf, sizeof(buf), "waiting=%d",
					 waiting);
			}
			wallops(NULL, "PANIC! %s (%s)", buf,
				strsignal(signum));
			log("PANIC! %s (%s)", buf, strsignal(signum));
		}
	}
	if (signum == SIGUSR1 || !(quitmsg = malloc(BUFSIZE))) {
		quitmsg = "Out of memory!";
		quitting = 1;
	} else {
#if HAVE_STRSIGNAL
		snoop(s_OperServ, "[CORE] Services terminating: %s",
			strsignal(signum));
/*#ifdef NETWORK_DOMAIN
		snprintf(nick, sizeof(nick), "$*.%s", NETWORK_DOMAIN);
                notice(s_GlobalNoticer, nick,
		       "Services terminating: %s", strsignal(signum));
#endif*/ /* #ifdef NETWORK_DOMAIN */

		snprintf(quitmsg, BUFSIZE, "Services terminating: %s",
			 strsignal(signum));
#else
		snoop(s_OperServ, "[CORE] Services terminating: %d", signum);
		snprintf(quitmsg, BUFSIZE,
			 "Services terminating on signal %d", signum);
#endif
		quitting = 1;
	}
	if (started)
		siglongjmp(panic_jmp, 1);
	else {
		log("%s", quitmsg);
		if (isatty(2))
			fprintf(stderr, "%s\n", quitmsg);
		exit(1);
	}
}

/*************************************************************************/

/* Main routine.  (What does it look like? :-) ) */

int main(int ac, char **av, char **envp)
{
	volatile time_t last_expire;	/* When did we last expire nicks/channels? */
	volatile time_t last_check;	/* When did we last check timeouts? */
	int i;
	char *progname;


	/* Find program name. */
	if ((progname = strrchr(av[0], '/')) != NULL)
		progname++;
	else
		progname = av[0];

	/* Initialization stuff. */
	if ((i = init(ac, av)) != 0)
		return i;


	/* We have a line left over from earlier, so process it first. */
	process();

	/* Set up timers. */
	last_check = last_expire = time(NULL);
	last_auth_expire = last_chanevent = last_check;

	/* The signal handler routine will drop back here with quitting != 0
	 * if it gets called. */
	sigsetjmp(panic_jmp, 1);

	started = 1;

	/*** Main loop. ***/

	while (!quitting) {
		time_t t = time(NULL);

		if (debug >= 2)
			log("debug: Top of main loop");
		if (save_data || ((t - last_expire) >= ExpireTimeout)) {
			waiting = -3;
			
			if (debug)
				log("debug: Running expire routines");
			
			waiting = -21;
			//expire_nicks();
			waiting = -22;
			//expire_chans();
			waiting = -23;
			//expire_nicksuspends();
			
			waiting = -25;
			expire_akills();
			//expire_exceptions();
			last_expire = t;
		}

		if (save_data ||
		    ((t - last_auth_expire) >= AuthExpireTimeout)) {
			waiting = -4;
			expire_auth(t);
			save_data = 0;
			last_auth_expire = t;
		}

		if (save_data || ((t - last_chanevent) >= 60)) {
			chan_update_events(t);
			last_chanevent = t;
		}

		if (delayed_quit)
			break;
		
		waiting = -1;
		
		if (t - last_check >= TimeoutCheck) {
			check_timeouts();
			last_check = t;
		}
		
		waiting = 1;
		i = sgets2(inbuf, sizeof(inbuf), servsock);
		waiting = 0;
		
		if (i > 0) {
			process();
		} else if (i == 0) {
			int errno_save = errno;

			quitmsg = malloc(BUFSIZE);
			
			if (quitmsg) {
				snprintf(quitmsg, BUFSIZE,
				    "Read error from server: %s",
				    strerror(errno_save));
			} else {
				quitmsg = "Read error from server";
			}
			
			quitting = 1;
			restarting = 1;
		}
		waiting = -4;
	}


	/* Check for restart instead of exit. */
	if (save_data == -2) {
#ifdef SERVICES_BIN
		log("Restarting");
		
		if (!quitmsg)
			quitmsg = "Restarting";
		
		send_cmd(ServerName, "SQUIT %s :%s", ServerName, quitmsg);
		disconn(servsock);
		smysql_cleanup(mysqlconn);
		close_log();
		free_directives();
		free_langs();
		execve(SERVICES_BIN, av, envp);
		open_log();
		log_perror("Restart failed");
		close_log();
		return 1;
#else
		quitmsg = "Restart attempt failed--SERVICES_BIN not "
		    "defined (rerun configure)";
#endif
	}

	/* Disconnect and exit */
	if (!quitmsg)
		quitmsg = "Terminating, reason unknown";
	
	log("%s", quitmsg);
	
	if (started)
		send_cmd(ServerName, "SQUIT %s :%s", ServerName, quitmsg);
	
	disconn(servsock);
	smysql_cleanup(mysqlconn);
	nickcache_delall();
	close_log();
	free_directives();
	free_langs();

	if (restarting) {
		sleep(5);
		execve(SERVICES_BIN, av, envp);
	}

	return 0;
}

/*************************************************************************/
