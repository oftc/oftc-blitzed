#!/usr/bin/perl -w

###############################################################################
#
# Script to bulk-register channels in a Blitzed Services database.  Channel
# name, founder's nick, password and description should be read from STDIN and
# are tab-separated.
#
# Channels that already exist will not be registered and a warning will be
# printed on STDERR, unless the --quiet option is used.
#
# In case of other non-fatal errors (e.g. those that prevent one or more
# channels from being registered correctly), a warning will be printed on
# STDERR, unless the --quiet option is used.
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

# Maximum length of channels on your network.
my $MAXCHAN = 64;

# Default for the maximum number of memos.
my $MSMaxMemos = 40;

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

# Connect to MySQL as which user.
my $user = 'root';


###############################################################################
# NO EDITABLE THINGS BELOW HERE!
###############################################################################

my %ci;
my ($sth, $salt, $founder_id, $r, $chan_id);

my $CI_KEEPTOPIC	= 0x00000001;
my $CI_SECURE		= 0x00000040;

my $def_chan_flags = $CI_KEEPTOPIC | $CI_SECURE;

my $CMODE_n = 0x00000004;
my $CMODE_t = 0x00000020;

my $mlock_on = $CMODE_n | $CMODE_t;

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
	($ci{'name'}, $ci{'founder'}, $ci{'pass'}, $ci{'desc'}) =
	    split(/\t+/, $_, 4);
	chomp($ci{'name'});
	chomp($ci{'founder'});
	chomp($ci{'pass'});
	chomp($ci{'desc'});

	$founder_id = 0;

	if (!is_valid_channel($ci{'name'})) {
		if (! $quiet) {
			print STDERR "Warning: '$ci{'name'}' is not a " .
			    "valid channel name, skipping.\n";
		}
		next;
	}

	$salt = '';
	$salt .= chr(65 + int rand 56) for 1 .. 16;

	# Check that the founder nick actually exists, get its nick_id.
	$sth = $dbh->prepare(qq{
SELECT nick_id
FROM nick
WHERE nick=?
	    });
	$sth->execute($ci{'founder'});

	while ($r = $sth->fetchrow_arrayref()) {
		$founder_id = $$r[0];
	}

	$sth->finish();

	if (!$founder_id) {
		if (! $quiet) {
			print STDERR "Warning: Nick '$ci{'founder'}' " .
			    "(founder of $ci{'name'}) does not exist, " .
			    "skipping.\n";
		}
		next;
	}

	if ($sth->err) {
		print STDERR "Fatal: MySQL error on checking founder nick.\n";
		die $sth->errstr;
	}

	# Make sure they haven't registered too many channels already.
	if (!check_channel_limit($founder_id)) {
		if (! $quiet) {
			print STDERR "Warning: Nick '$ci{'founder'}' " .
			    "(founder of $ci{'name'}) has already " .
			    "registered too many channels, skipping.\n";
		}
		next;
	}

	$sth = $dbh->prepare(qq{
INSERT INTO channel
(channel_id, name, founder, founderpass, salt, description,
    time_registered, last_used, flags, mlock_on)
VALUES
('NULL',	# channel_id
?,		# name
?,		# founder
SHA1(?),	# founderpass + salt
?,		# salt
?,		# description
?,		# time_registered
?,		# last_used
?,		# flags
?		# mlock_on
)
	    });
	$sth->execute($ci{'name'}, $founder_id, $ci{'pass'} . $salt,
	    $salt, $ci{'desc'}, time(), time(), $def_chan_flags,
	    $mlock_on);


	# MySQL specific, comes from mysqld_error.h, indicates an attempt to
	# insert a duplicate row.  i.e. this channel already exists.
	if ($sth->err && $sth->err == 1062) {
		if (! $quiet) {
			print STDERR "Warning: '$ci{'name'}' is already " .
			    "registered, skipping.\n";
		}
		$sth->finish;
		next;
	}

	if ($sth->err) {
		if (! $quiet) {
			print STDERR "Warning: MySQL error INSERTing " .
			    "into channel table, skipping.\n";
			print STDERR $sth->errstr;
		}
		$sth->finish;
		next;
	}

	$sth->finish;

	$chan_id = $dbh->{'mysql_insertid'};

	$sth = $dbh->prepare(qq{
INSERT INTO memoinfo
(memoinfo_id, owner, max)
VALUES
('NULL', ?, ?)
	    });
	$sth->execute($ci{'name'}, $MSMaxMemos);

	if ($sth->err) {
		print STDERR "Fatal: MySQL error INSERTing into " .
		    "memoinfo table, database now in inconsistent state!\n";
		die $sth->errstr;
	}

	$sth->finish;

	if ($verbose) {
		print "Successfully registered '$ci{'name'}' as " .
		    "channel_id $chan_id!\n";
	}
}



$dbh->disconnect();
exit;

sub is_valid_channel {
	return 0 if length $_[0] > $MAXCHAN or length $_[0] < 1;
	return 0 if $_[0] !~ /^#[\S]*/;
	return 1;
}

sub usage
{
	print STDERR "Blitzed Services bulk channel registration tool.\n\n";
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

sub check_channel_limit
{
	my $nick_id = getlink(shift);
	my $max = get_channelmax($nick_id);
	my $count = count_chans_reged_by($nick_id);

	return ($count < $max);
}

sub getlink
{
	my $nick_id = shift;
	my ($r, $master_id);

	$master_id = 0;

	my $sth = $dbh->prepare(qq{
SELECT link_id
FROM nick
WHERE nick_id=?
	    });
	$sth->execute($nick_id);

	while ($r = $sth->fetchrow_arrayref()) {
		$master_id = $$r[0];
	}

	$sth->finish;

	die $sth->errstr if ($sth->err);
	return($master_id ? $master_id : $nick_id);
}

sub get_channelmax
{
	my $nick_id = shift;

	my ($r, $max);

	my $sth = $dbh->prepare(qq{
SELECT channelmax
FROM nick
WHERE nick_id=?
	    });
	$sth->execute($nick_id);

	while ($r = $sth->fetchrow_arrayref()) {
		$max = $$r[0];
	}

	$sth->finish;

	die $sth->errstr if ($sth->err);
	return($max);
}

sub count_chans_reged_by
{
	my $nick_id = shift;
	my ($chans, $r);

	$chans = 0;

	my $sth = $dbh->prepare(qq{
SELECT COUNT(channel_id)
FROM channel
WHERE founder=?
	    });
	$sth->execute($nick_id);

	while ($r = $sth->fetchrow_arrayref()) {
		$chans = $$r[0];
	}

	$sth->finish;
	die $sth->errstr if ($sth->err);
	return($chans);
}
