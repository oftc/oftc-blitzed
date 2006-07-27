
/*
 * Quarantine (QLINE) functions.
 *
 * Blitzed Services copyright (c) 2000-2002 Blitzed IRC Network
 *      E-mail: services@lists.blitzed.org
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include "services.h"
#include "pseudo.h"

RCSID("$Id$");

static void renumber_quarantines(void);

/*************************************************************************/

/*
 * Handle an incoming SQLINE.  We want to check this SQLINE regex against
 * the ones we already have, and add it if it is new, notifying the
 * network of this fact.
 *
 * Arguments:
 *
 * 0	regex
 * 1	reason
 * 
 */
void do_sqline(const char *source, int ac, char **av)
{
	char buf[128];
	regex_t r;
	unsigned int err, qcount, ischan, matches;
	int regerr;
	MYSQL_RES *result;
	char *esc_server, *esc_regex, *esc_reason;

	USE_VAR(ac);

	if ((regerr = regcomp(&r, av[0], (REG_EXTENDED | REG_ICASE)))) {
		wallops(s_OperServ,
		    "Ignoring broken QLINE %s from server %s (%s)",
		    av[0], source, av[1]);
		regerror(regerr, &r, buf, sizeof(buf));
		wallops(s_OperServ, "Error: %s", buf);
		regfree(&r);
		return;
	}

	if (strchr(av[0], '#')) {
		ischan = 1;
	} else {
		ischan = 0;
	}

	qcount = get_table_rows(mysqlconn, "quarantine");

	esc_server = smysql_escape_string(mysqlconn, source);
	esc_regex = smysql_escape_string(mysqlconn, av[0]);
	esc_reason = smysql_escape_string(mysqlconn, av[1]);

	result = mysql_simple_query(mysqlconn, &err,
	    "INSERT INTO quarantine (quarantine_id, idx, regex, added_by, "
	    "added_time, reason) VALUES ('NULL', %u, '%s', '%s', %d, '%s')",
	    qcount + 1, esc_regex, esc_server, time(NULL), esc_reason);
	mysql_free_result(result);

	free(esc_reason);
	free(esc_regex);
	free(esc_server);
	
	switch (err) {
	case 0:
		wallops(s_OperServ, "Quarantine %s learned from "
		    "server %s (%s)", av[0], source, av[1]);
		send_cmd(MODE_SENDER(s_OperServ), "SQLINE %s :%s", av[0],
		    av[1]);

		if (ischan) {
			matches = chan_count_regex_match(&r);
			wallops(s_OperServ,
			    "Matches %u currently in use channel%s",
			    matches, matches == 1 ? "" : "s");
		} else {
			matches = user_count_regex_match(&r);
			wallops(s_OperServ,
			    "Matches %u currently in use nick%s",
			    matches, matches == 1 ? "" : "s");
		}

		break;

	case ER_DUP_ENTRY:
		/* SQLINE already known, ignore. */
		break;

	default:
		/* Some other problem. */
		regfree(&r);
		handle_mysql_error(mysqlconn, "do_sqline");
		break;		/* Not reached. */
	}

	regfree(&r);
}

/*
 * Manipulate the quarantine list.
 */
void do_quarantine(User *u)
{
	char base[BUFSIZE], buf[BUFSIZE];
	MYSQL_ROW row;
	regex_t r;
	time_t added;
	unsigned int fields, rows, qcount, err, matches;
	int sent_header, regerr, ischan;
	char *cmd, *regex, *reason, *query, *esc_regex, *esc_added_by;
	char *esc_reason;
	MYSQL_RES *result;
	struct tm *tm;

	cmd = strtok(NULL, " ");
	regex = strtok(NULL, " ");
	reason = strtok(NULL, "");

	if (!cmd || (stricmp(cmd, "ADD") == 0 && (!regex || !reason)) ||
	    (stricmp(cmd, "DEL") == 0 && !regex)) {
		syntax_error(s_OperServ, u, "QUARANTINE",
		    OPER_QUARANTINE_SYNTAX);
	} else if (stricmp(cmd, "LIST") == 0) {
		if (regex && strspn(regex, "1234567890,-") == strlen(regex)) {
			/*
			 * We have been given a list or range of indices
			 * of existing channel access to match.
			 */
			snprintf(base, sizeof(base),
			    "SELECT idx, regex, added_by, added_time, "
			    "reason FROM quarantine WHERE (");

			/*
			 * Build a query based on the above base query and
			 * matching the range or list of indices against
			 * the quarantine index, order by the index.
			 */
			query = mysql_build_query(mysqlconn, regex, base,
			    "idx", 32767);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s) ORDER BY idx ASC", query);
			free(query);
		} else if (regex) {
			/* It is an exact match. */
			esc_regex = smysql_escape_string(mysqlconn, regex);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT idx, regex, added_by, "
			    "added_time, reason FROM quarantine "
			    "WHERE regex='%s' ORDER by idx ASC", esc_regex);
			free(esc_regex);
		} else {
			/* They want the whole list. */
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT idx, regex, added_by, "
			    "added_time, reason FROM quarantine "
			    "ORDER by idx ASC");
		}

		sent_header = 0;

		while ((row = smysql_fetch_row(mysqlconn, result))) {
			added = atoi(row[3]);
			tm = gmtime(&added);
			strftime_lang(buf, sizeof(buf), u,
			    STRFTIME_DATE_TIME_FORMAT, tm);
			
			if (!sent_header) {
				notice_lang(s_OperServ, u,
				    OPER_QUARANTINE_LIST_HEADER);
				sent_header = 1;
			}

			notice_lang(s_OperServ, u,
			    OPER_QUARANTINE_LIST_FORMAT1, atoi(row[0]),
			    row[1], row[4]);
			notice_lang(s_OperServ, u,
			    OPER_QUARANTINE_LIST_FORMAT2, row[2], buf);
		}

		mysql_free_result(result);

		if (!sent_header) {
			notice_lang(s_OperServ, u, OPER_QUARANTINE_NO_MATCH);
		}
	} else if (stricmp(cmd, "ADD") == 0) {
		if ((regerr = regcomp(&r, regex, (REG_EXTENDED | REG_ICASE)))) {
			notice_lang(s_OperServ, u,
			    OPER_QUARANTINE_BROKEN_REGEX, regex);
			regfree(&r);
			return;
		}
		
		qcount = get_table_rows(mysqlconn, "quarantine");
		esc_regex = smysql_escape_string(mysqlconn, regex);
		esc_added_by = smysql_escape_string(mysqlconn, u->nick);
		esc_reason = smysql_escape_string(mysqlconn, reason);

		result = mysql_simple_query(mysqlconn, &err,
		    "INSERT INTO quarantine (quarantine_id, idx, regex, "
		    "added_by, added_time, reason) "
		    "VALUES ('NULL', %u, '%s', '%s', %d, '%s')",
		    qcount + 1, esc_regex, esc_added_by, time(NULL),
		    esc_reason);
		mysql_free_result(result);

		free(esc_reason);
		free(esc_added_by);
		free(esc_regex);

		switch (err) {
		case 0:
			break;
		case ER_DUP_ENTRY:
			notice_lang(s_OperServ, u,
			    OPER_QUARANTINE_ALREADY_PRESENT, regex);
			return;
		default:
			handle_mysql_error(mysqlconn, "add_quarantine");
			return;		/* Not reached. */
		}

		if (strchr(regex, '#')) {
			ischan = 1;
			matches = chan_count_regex_match(&r);
		} else {
			ischan = 0;
			matches = user_count_regex_match(&r);
		}
		
		notice_lang(s_OperServ, u, OPER_QUARANTINE_ADDED, regex);

		if (ischan) {
			notice_lang(s_OperServ, u,
			    matches == 1 ?
			    OPER_QUARANTINE_CHAN_MATCHES_ONE : 
			    OPER_QUARANTINE_CHAN_MATCHES, matches);
		} else {
			notice_lang(s_OperServ, u,
			    matches == 1 ?
			    OPER_QUARANTINE_NICK_MATCHES_ONE :
			    OPER_QUARANTINE_NICK_MATCHES, matches);
		}

		send_cmd(MODE_SENDER(s_OperServ), "SQLINE %s :%s", regex,
		    reason);
		
		wallops(s_OperServ,
		    "%s added Quarantine on %s (%s)", u->nick,
		    regex, reason);

		if (ischan) {
			wallops(s_OperServ,
			    "Matches %u currently in use channel%s",
			    matches, matches == 1? "" : "s");
		} else {
			wallops(s_OperServ,
			    "Matches %u currently in use nick%s",
			    matches, matches == 1? "" : "s");
		}
	} else if (stricmp(cmd, "DEL") == 0) {
		if (isdigit(*regex) &&
		strspn(regex, "1234567890,-") == strlen(regex)) {
			/* We've been given a list of indices to match. */
			snprintf(base, sizeof(base),
			    "SELECT regex, reason FROM quarantine WHERE (");

			/*
			 * Build a query based on the above base query and
			 * matching the range or list of indices against
			 * the quarantine indices.
			 */
			query = mysql_build_query(mysqlconn, regex, base,
			    "idx", 32767);
			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s)", query);

			while ((row = smysql_fetch_row(mysqlconn, result))) {
				send_cmd(MODE_SENDER(s_OperServ),
				    "UNSQLINE %s", row[0]);
				wallops(s_OperServ,
				    "%s removed Quarantine on %s "
				    "(%s)", u->nick, row[0], row[1]);
			}

			mysql_free_result(result);
			free(query);
			base[0] = '\0';
			
			snprintf(base, sizeof(base),
			    "DELETE FROM quarantine WHERE (");

			query = mysql_build_query(mysqlconn, regex, base,
			    "idx", 32767);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "%s)", query);

			free(query);
		} else {
			/* Match exact regex. */
			esc_regex = smysql_escape_string(mysqlconn, regex);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "SELECT regex, reason FROM quarantine "
			    "WHERE regex='%s'", esc_regex);

			while ((row = smysql_fetch_row(mysqlconn, result))) {
				send_cmd(MODE_SENDER(s_OperServ),
				    "UNSQLINE %s", row[0]);
				wallops(s_OperServ,
				    "%s removed Quarantine on %s "
				    "(%s)", u->nick, row[0], row[1]);
			}

			mysql_free_result(result);

			result = smysql_bulk_query(mysqlconn, &fields,
			    &rows, "DELETE FROM quarantine "
			    "WHERE regex='%s'", esc_regex);

			free(esc_regex);
		}

		mysql_free_result(result);

		if (!rows) {
			notice_lang(s_OperServ, u,
			    OPER_QUARANTINE_NO_MATCH);
		} else if (rows == 1) {
			notice_lang(s_OperServ, u,
			    OPER_QUARANTINE_DELETED_ONE);
		} else {
			notice_lang(s_OperServ, u,
			    OPER_QUARANTINE_DELETED_SEVERAL, rows);
		}

		if (rows)
			renumber_quarantines();
	}
}

/*
 * Renumber the indices of the quarantine table - needed after quarantines
 * are deleted.
 */
static void renumber_quarantines(void)
{
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "TRUNCATE private_tmp_quarantine");
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "INSERT INTO private_tmp_quarantine (quarantine_id, idx, "
	    "regex, added_by, added_time, reason) SELECT quarantine_id, "
	    "NULL, regex, added_by, added_time, reason FROM quarantine "
	    "ORDER BY added_time");
	mysql_free_result(result);

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "REPLACE INTO quarantine (quarantine_id, idx, regex, "
	    "added_by, added_time, reason) SELECT quarantine_id, idx, "
	    "regex, added_by, added_time, reason FROM "
	    "private_tmp_quarantine");
	mysql_free_result(result);
}

/*
 * Send all our quarantines out to the network as SQLINEs.
 */
void send_sqlines(void)
{
	MYSQL_ROW row;
	unsigned int fields, rows;
	MYSQL_RES *result;

	result = smysql_bulk_query(mysqlconn, &fields, &rows,
	    "SELECT regex, reason FROM quarantine");

	while ((row = smysql_fetch_row(mysqlconn, result))) {
		send_cmd(MODE_SENDER(s_OperServ), "SQLINE %s :%s", row[0],
		    row[1]);
	}

	mysql_free_result(result);
}
