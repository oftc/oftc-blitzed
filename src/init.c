
/*
 * Initalization and related routines.
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

/* Imported from main.c */
extern void sighandler(int signum);
extern void sigusr2_handler(int signum);

/*************************************************************************/

/* Send a NICK command for the given pseudo-client.  If `user' is NULL,
 * send NICK commands for all the pseudo-clients. */

#if 0
// NICK <nick> <hops> <TS> <umode> <user> <host> <server> <svsid> :<ircname>
#  define NICK(nick,name) \
    do { \
	send_cmd(NULL, "NICK %s 1 %ld + %s %s %s 0 0 :%s", (nick), time(NULL), \
		ServiceUser, ServiceHost, ServerName, (name)); \
    } while (0)
#endif				/* if 0 */

#define NICK(nick,name) \
    send_nick((nick), ServiceUser, ServiceHost, ServerName, (name));

void introduce_user(const char *user)
{
	/* Watch out for infinite loops... */
#define LTSIZE 20
	static int lasttimes[LTSIZE];

	if (lasttimes[0] >= time(NULL) - 3)
		fatal("introduce_user() loop detected");
	memmove(lasttimes, lasttimes + 1, sizeof(lasttimes) - sizeof(int));

	lasttimes[LTSIZE - 1] = time(NULL);
#undef LTSIZE

	if (!user || stricmp(user, s_NickServ) == 0) {
		NICK(s_NickServ, desc_NickServ);
		send_cmd(s_NickServ, "MODE %s +o", s_NickServ);
	}
	if (!user || stricmp(user, s_ChanServ) == 0) {
		NICK(s_ChanServ, desc_ChanServ);
		send_cmd(s_ChanServ, "MODE %s +o", s_ChanServ);
	}
	if (!user || stricmp(user, s_HelpServ) == 0) {
		NICK(s_HelpServ, desc_HelpServ);
	}
	if (s_IrcIIHelp && (!user || stricmp(user, s_IrcIIHelp) == 0)) {
		NICK(s_IrcIIHelp, desc_IrcIIHelp);
	}
	if (!user || stricmp(user, s_MemoServ) == 0) {
		NICK(s_MemoServ, desc_MemoServ);
		send_cmd(s_MemoServ, "MODE %s +o", s_MemoServ);
	}
	if (!user || stricmp(user, s_OperServ) == 0) {
		NICK(s_OperServ, desc_OperServ);
		send_cmd(s_OperServ, "MODE %s +oi", s_OperServ);
	}
	if (s_DevNull && (!user || stricmp(user, s_DevNull) == 0)) {
		NICK(s_DevNull, desc_DevNull);
		send_cmd(s_DevNull, "MODE %s +i", s_DevNull);
	}
	if (!user || stricmp(user, s_GlobalNoticer) == 0) {
		NICK(s_GlobalNoticer, desc_GlobalNoticer);
		send_cmd(s_GlobalNoticer, "MODE %s +oi", s_GlobalNoticer);
	}
    if (!user || stricmp(user, s_FloodServ) == 0) {
        NICK(s_FloodServ, desc_FloodServ);
        send_cmd(s_FloodServ, "MODE %s +oi", s_FloodServ);
    }
}

#undef NICK

/*************************************************************************/

/*
 * Parse command-line options.  Return 0 if all went well or -1 for an error
 * with an option.
 */

static int parse_options(int ac, char **av)
{
	int i;
	char *s, *t;

	for (i = 1; i < ac; i++) {
		s = av[i];
		
		if (*s != '-') {
			fprintf(stderr, "Non-option arguments not allowed\n");
			return(-1);
		}

		s++;
			
		if (strcmp(s, "remote") == 0) {
			if (++i >= ac) {
				fprintf(stderr,
				    "-remote requires hostname[:port]\n");
				return(-1);
			}
				
			s = av[i];
			t = strchr(s, ':');
				
			if (t) {
				*t++ = 0;
					
				if (atoi(t) > 0) {
					RemotePort = atoi(t);
				} else {
					fprintf(stderr,
					    "-remote: port number must be "
					    "a positive integer.  Using "
					    "default.");
				return(-1);
				}
			}
				
			RemoteServer = s;
		} else if (strcmp(s, "local") == 0) {
			if (++i >= ac) {
				fprintf(stderr,
				    "-local requires hostname[:port]\n");
				return(-1);
			}
				
			s = av[i];
			t = strchr(s, ':');
				
			if (t) {
				*t++ = 0;
					
				if (atoi(t) >= 0) {
					LocalPort = atoi(t);
				} else {
					fprintf(stderr,
					    "-local: port number must be "
					    "a positive integer or 0.  "
					    "Using default.");
					return(-1);
				}
			}
				
			LocalHost = s;
		} else if (strcmp(s, "name") == 0) {
			if (++i >= ac) {
				fprintf(stderr,
				    "-name requires a parameter\n");
				return(-1);
			}
				
			ServerName = av[i];
		} else if (strcmp(s, "desc") == 0) {
			if (++i >= ac) {
				fprintf(stderr,
				    "-desc requires a parameter\n");
				return(-1);
			}

			ServerDesc = av[i];
		} else if (strcmp(s, "user") == 0) {
			if (++i >= ac) {
				fprintf(stderr,
				    "-user requires a parameter\n");
				return(-1);
			}
				
			ServiceUser = av[i];
		} else if (strcmp(s, "host") == 0) {
			if (++i >= ac) {
				fprintf(stderr,
				    "-host requires a parameter\n");
				return(-1);
			}
				
			ServiceHost = av[i];
		} else if (strcmp(s, "log") == 0) {
			if (++i >= ac) {
				fprintf(stderr,
				    "-log requires a parameter\n");
				return(-1);
			}
				
			log_filename = av[i];
		} else if (strcmp(s, "expire") == 0) {
			if (++i >= ac) {
				fprintf(stderr,
				    "-expire requires a parameter\n");
				return(-1);
			}
				
			s = av[i];
				
			if (atoi(s) <= 0) {
				fprintf(stderr,
				    "-expire: number of seconds must be "
				    "positive");
				return(-1);
			}
				
			ExpireTimeout = atol(s);
		} else if (strcmp(s, "debug") == 0) {
			debug++;
		} else if (strcmp(s, "nofork") == 0) {
			nofork = 1;
		} else if (strcmp(s, "noexpire") == 0) {
			opt_noexpire = 1;
		} else {
			fprintf(stderr, "Unknown option -%s\n", s);
			return(-1);
		}
	}
	return(0);
}

/*************************************************************************/

/* Remove our PID file.  Done at exit. */

static void remove_pidfile(void)
{
	remove(PIDFilename);
}

/*************************************************************************/

/* Create our PID file and write the PID to it. */

static void write_pidfile(void)
{
	FILE *pidfile;

	pidfile = fopen(PIDFilename, "w");
	if (pidfile) {
		fprintf(pidfile, "%d\n", (int)getpid());
		fclose(pidfile);
		atexit(remove_pidfile);
	} else {
		log_perror("Warning: cannot write to PID file %s",
			   PIDFilename);
	}
}

/*************************************************************************/

/* Overall initialization routine.  Returns 0 on success, -1 on failure. */

int init(int ac, char **av)
{
	int i, openlog_failed, openlog_errno, started_from_term;

	openlog_failed = openlog_errno = 0;
	started_from_term = isatty(0) && isatty(1) && isatty(2);

	/* Set the random seed. */
	srand((unsigned int)time(NULL));

	/* Chdir to Services config directory. */
	if (chdir(etcdir) < 0) {
		fprintf(stderr, "chdir(%s): %s\n", etcdir,
			strerror(errno));
		return -1;
	}

	/* Open logfile, and complain if we didn't. */
	if (open_log() < 0) {
		openlog_errno = errno;
		if (started_from_term) {
			fprintf(stderr,
				"Warning: unable to open log file %s: %s\n",
				log_filename, strerror(errno));
		} else {
			openlog_failed = 1;
		}
	}

	/* Read configuration file; exit if there are problems. */
	if (!read_config())
		return -1;

	/* Parse all remaining command-line options. */
	parse_options(ac, av);

	/* Detach ourselves if requested. */
	if (!nofork) {
		if ((i = fork()) < 0) {
			perror("fork()");
			return -1;
		} else if (i != 0) {
			exit(0);
		}
		
		if (started_from_term) {
			close(0);
			close(1);
			close(2);
		}
		
		if (setpgid(0, 0) < 0) {
			perror("setpgid()");
			return -1;
		}
	}

	/* Write our PID to the PID file. */
	write_pidfile();

	/* Announce ourselves to the logfile. */
	if (debug) {
		log("Services %s starting up (options:%s)", VERSION,
		    debug ? " debug" : "");
	} else {
		log("Services %s starting up", VERSION);
	}

	if (debug)
		log("debug: level %d", debug);

	start_time = time(NULL);

	/*
	 * Set signal handlers.  Catch certain signals to let us do things or
	 * panic as necessary, and ignore all others.
	 */

#ifdef NSIG
	for (i = 1; i <= NSIG; i++) {
#else
	for (i = 1; i <= 32; i++) {
#endif
		signal(i, SIG_IGN);
	}

	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGQUIT, sighandler);
	signal(SIGBUS, sighandler);
	signal(SIGQUIT, sighandler);
	signal(SIGHUP, sighandler);

	/* This is our "out-of-memory" panic switch. */
	signal(SIGUSR1, sighandler);

	/* Cycle log files. */
	signal(SIGUSR2, sigusr2_handler);


	/* Initialise multi-language support. */
	lang_init();
	if (debug)
		log("debug: Loaded languages");

	/* Initialise MySQL. */
	smysql_init();

	/* Initialise subservices. */
	ns_init();
	cs_init();
	ms_init();
	os_init();
    fs_init();

	/* Connect to the remote server. */
	servsock = conn(RemoteServer, RemotePort, LocalHost, LocalPort);
	
	if (servsock < 0)
		fatal_perror("Can't connect to server");
	
	send_cmd(NULL, "PASS %s :TS", RemotePassword);
	send_cmd(NULL, "CAPAB TS3 KLN");
	send_cmd(NULL, "SERVER %s 1 :%s", ServerName, ServerDesc);
	send_cmd(NULL, "SVINFO 3 3 0 :%d", time(NULL));
	sgets2(inbuf, sizeof(inbuf), servsock);
	
	if (strnicmp(inbuf, "ERROR", 5) == 0) {
		/*
		 * Close server socket first to stop wallops, since the other
		 * server doesn't want to listen to us anyway.
		 */
		disconn(servsock);
		servsock = -1;
		fatal("Remote server returned: %s", inbuf);
	}

	/* Announce a logfile error if there was one. */
	if (openlog_failed) {
		wallops(NULL, "Warning: couldn't open logfile: %s",
		    strerror(openlog_errno));
	}

	/* Bring in our pseudo-clients. */
	introduce_user(NULL);

	/* Success! */
	return 0;
}

/*************************************************************************/
