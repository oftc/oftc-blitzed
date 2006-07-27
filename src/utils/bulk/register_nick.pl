#!/usr/bin/perl -w

###############################################################################
#
# Script to bulk-register nicknames in a Blitzed Services database.  Nick,
# password and email address should be read from STDIN and are tab-separated.
#
# Nicks that already exist will not be registered and a warning will be printed
# on STDERR, unless the --quiet option is used.
#
# In case of other non-fatal errors (e.g. those that prevent one or more nicks
# from being registered correctly), a warning will be printed on STDERR, unless
# the --quiet option is used.
#
# Fatal errors will always print a warning.
#
# Progress information will be printed to STDOUT if the --verbose option is
# used.
#
# Note that there are a number of "network settings" that are set as variables
# in this script, which would normally be set in the services.conf.  If someone
# finds this script useful and would like to submit patches that read
# services.conf, that would be appreciated.  Until then the Blitzed defaults
# will remain in here. :)
#
# Blitzed Services copyright (c) 2000-2002 Blitzed Services team
#     E-mail: services@lists.blitzed.org
#
# This program is free but copyrighted software; see the file COPYING for
# details.
#
# $Id$
#
###############################################################################

use strict;
use Getopt::Long;
use DBI;

# Maximum length of nicks on your network.
my $MAXNICK = 30;

# Default nick options, set to 1 to mark them as default, 0 otherwise.
my $NSDefEnforce	= 0;
my $NSDefEnforceQuick	= 0;
my $NSDefSecure		= 1;
my $NSDefPrivate	= 0;
my $NSDefHideEmail	= 0;
my $NSDefHideUsermask	= 0;
my $NSDefHideQuit	= 0;
my $NSDefAutoJoin	= 1;
my $NSDefMemoSignon	= 1;
my $NSDefMemoReceive	= 1;

# Default language.  See services.h for the list of available languages.
# 0 is US English.
my $DEF_LANGUAGE = 0;

# Default for the maximum number of memos.
my $MSMaxMemos = 40;

# Default for the maximum number of channels allowed to register.
my $CSMaxReg = 20;

# There are defaults that can be altered with command line options.

# Default DB to use.
my $database = 'services';

# Default MySQL host.
my $host = 'localhost';

# MySQL password.
my $password = '';

# MySQL port.
my $port = '3306';

# Run quietly or not.
my $quiet = '';

# Run verbosely or not.
my $verbose = '';

# Connect to MySQl as which user.
my $user = 'root';


###############################################################################
# NO EDITABLE THINGS BELOW HERE!
###############################################################################

my %ni;
my ($sth, $salt, $nick_id);

my $NI_ENFORCE		= 0x00000001;
my $NI_SECURE		= 0x00000002;
my $NI_MEMO_HARDMAX	= 0x00000008;
my $NI_MEMO_SIGNON	= 0x00000010;
my $NI_MEMO_RECEIVE	= 0x00000020;
my $NI_PRIVATE		= 0x00000040;
my $NI_HIDE_EMAIL	= 0x00000080;
my $NI_HIDE_MASK	= 0x00000100;
my $NI_HIDE_QUIT	= 0x00000200;
my $NI_ENFORCEQUICK	= 0x00000400;
my $NI_NOOP		= 0x00000800;
my $NI_IRCOP		= 0x00001000;
my $NI_MARKED		= 0x00002000;
my $NI_NOSENDPASS	= 0x00004000;
my $NI_AUTOJOIN		= 0x00008000;
my $NI_SUSPENDED	= 0x10000000;

my $def_nick_flags;

$def_nick_flags |= $NI_ENFORCE		if ($NSDefEnforce);
$def_nick_flags |= $NI_ENFORCEQUICK	if ($NSDefEnforceQuick);
$def_nick_flags |= $NI_SECURE		if ($NSDefSecure);
$def_nick_flags |= $NI_PRIVATE		if ($NSDefPrivate);
$def_nick_flags |= $NI_HIDE_EMAIL	if ($NSDefHideEmail);
$def_nick_flags |= $NI_HIDE_MASK	if ($NSDefHideUsermask);
$def_nick_flags |= $NI_HIDE_QUIT	if ($NSDefHideQuit);
$def_nick_flags |= $NI_MEMO_SIGNON	if ($NSDefMemoSignon);
$def_nick_flags |= $NI_MEMO_RECEIVE	if ($NSDefMemoReceive);
$def_nick_flags |= $NI_AUTOJOIN		if ($NSDefEnforce);


GetOptions(
	'database:s'	=> \$database,
	'help'		=> \&usage,
	'host:s'	=> \$host,
	'password:s'	=> \$password,
	'port:i'	=> \$port,
	'quiet'		=> \$quiet,
	'user:s'	=> \$user,
	'verbose'	=> \$verbose
    );

my $dsn = sprintf("DBI:mysql:%s%s%s", $database,
    ($host eq "localhost" ? "" : ";host=$host"),
    ($port eq "3306" ? "" : ";port=$port"));
my $dbh = DBI->connect($dsn, $user, $password) or die;

while (<>) {
	($ni{'nick'}, $ni{'pass'}, $ni{'email'}) = split(/\t+/, $_, 3);
	chomp($ni{'nick'});
	chomp($ni{'pass'});
	chomp($ni{'email'});

	$nick_id = 0;

	if (!is_valid_nickname($ni{'nick'})) {
		if (! $quiet) {
			print STDERR "Warning: '$ni{'nick'}' is not a " .
			    "valid nickname, skipping.\n";
		}
		next;
	}

	$salt = '';
	$salt .= chr(65 + int rand 56) for 1 .. 16;

	# It's possible that some nicks might not have an email address.
	$ni{'email'} = '' if (! $ni{'email'});

	$sth = $dbh->prepare(qq{
INSERT INTO nick
(nick_id, nick, pass, salt, email, last_usermask, last_realname,
  time_registered, last_seen, status, flags, channelmax, language,
  id_stamp, regainid)
VALUES
('NULL',	# nick_id
?,		# nick
SHA1(?),	# pass + salt
?,		# salt
?,		# email
'',		# last_usermask
'',		# last_realname
?,		# time_registered
?,		# last_seen
0,		# status
?,		# flags
?,		# channelmax
?,		# language
0,		# id_stamp
0)		# regainid
	    });
	$sth->execute($ni{'nick'}, $ni{'pass'} . $salt, $salt,
	    $ni{'email'}, time(), time(), $def_nick_flags, $CSMaxReg,
	    $DEF_LANGUAGE);

	# MySQL specific, comes from mysqld_error.h, indicates an attempt to
	# insert a duplicate row.  i.e. this nick already exists.
	if ($sth->err && $sth->err == 1062) {
		if (! $quiet) {
			print STDERR "Warning: '$ni{'nick'}' is already " .
			    "registered, skipping.\n";
		}
		$sth->finish;
		next;
	}

	if ($sth->err) {
		if (! $quiet) {
			print STDERR "Warning: MySQL error INSERTing " .
			    "into nick table, skipping.\n";
			print STDERR $sth->errstr;
		}
		$sth->finish;
		next;
	}

	$nick_id = $dbh->{'mysql_insertid'};

	$sth->finish;

	$sth = $dbh->prepare(qq{
INSERT INTO memoinfo
(memoinfo_id, owner, max)
VALUES
('NULL', ?, ?)
	    });
	$sth->execute($ni{'nick'}, $MSMaxMemos);

	if ($sth->err) {
		print STDERR "Fatal: MySQL error INSERTing into " .
		    "memoinfo table, database now in inconsistent state!\n";
		die $sth->errstr;
	}

	$sth->finish;

	if ($verbose) {
		print "Successfully registered '$ni{'nick'}' as " .
		    "nick_id $nick_id!\n";
	}

	# Should warn about empty email addresses.
	if ($ni{'email'} eq '' && ! $quiet) {
		print STDERR "Warning: '$ni{'nick'}' has been registered " .
		    "without an email address.\n";
	}
}



$dbh->disconnect();
exit;

# Thanks dg!
sub is_valid_nickname {
	return 0 if length $_[0] > $MAXNICK or length $_[0] < 1;
	return 0 if $_[0] =~ /[^A-Za-z0-9-_\[\]\\\`\^\{\}\|]/;
	return 0 if $_[0] =~ /^[^A-Za-z_\\\[\]\`\^\{\}\|]/;
	return 1;
}

sub usage
{
	print STDERR "Blitzed Services bulk nick registration tool.\n\n";
	print STDERR "Command line options:\n";
	print STDERR "--database=DB    Name of MySQL database that holds your services data.\n";
	print STDERR "                 Defaults to \"services\".\n";
	print STDERR "--help           This text.\n";
	print STDERR "--host=HOST      MySQL host.  Defaults to \"localhost\".\n";
	print STDERR "--password=PASS  MySQL password.  Defaults to no password.\n";
	print STDERR "--port=PORT      MySQL port.  Defaults to 3306.\n";
	print STDERR "--quiet          Run quietly.\n";
	print STDERR "--user=USER      MySQL user.  Defaults to \"root\".\n";
	print STDERR "--verbose        Run verbosely.\n";
	exit;
}
