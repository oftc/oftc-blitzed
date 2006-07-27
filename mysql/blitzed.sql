# phpMyAdmin MySQL-Dump
# version 2.3.0-rc2
# http://phpwizard.net/phpMyAdmin/
# http://www.phpmyadmin.net/ (download page)
#
# Host: localhost
# Generation Time: Jul 25, 2002 at 01:42 AM
# Server version: 4.00.02
# PHP Version: 4.2.2
# Database : `services`
# --------------------------------------------------------

#
# Table structure for table `admin`
#

DROP TABLE IF EXISTS `admin`;
CREATE TABLE `admin` (
  `admin_id` int(10) unsigned NOT NULL auto_increment,
  `nick_id` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`admin_id`),
  UNIQUE KEY `admin_id` (`admin_id`),
  UNIQUE KEY `nick_id` (`nick_id`)
) TYPE=MyISAM COMMENT='List of services admins';
# --------------------------------------------------------

#
# Table structure for table `akick`
#

DROP TABLE IF EXISTS `akick`;
CREATE TABLE `akick` (
  `akick_id` int(10) unsigned NOT NULL auto_increment,
  `channel_id` int(10) unsigned NOT NULL default '0',
  `idx` mediumint(8) unsigned NOT NULL default '0',
  `mask` varchar(255) default NULL,
  `nick_id` int(10) unsigned NOT NULL default '0',
  `reason` text,
  `who` varchar(32) NOT NULL default '',
  `added` int(10) unsigned NOT NULL default '0',
  `last_used` int(10) unsigned NOT NULL default '0',
  `last_matched` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`akick_id`),
  UNIQUE KEY `channel_id_mask` (`channel_id`,`mask`),
  KEY `idx` (`channel_id`,`idx`)
) TYPE=MyISAM COMMENT='Channel AutoKicks. nick_id=0 where mask is in use.';
# --------------------------------------------------------

#
# Table structure for table `akill`
#

DROP TABLE IF EXISTS `akill`;
CREATE TABLE `akill` (
  `akill_id` int(10) unsigned NOT NULL auto_increment,
  `mask` varchar(255) NOT NULL default '',
  `reason` varchar(255) NOT NULL default '',
  `who` varchar(32) NOT NULL default '',
  `time` int(10) unsigned NOT NULL default '0',
  `expires` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`akill_id`),
  UNIQUE KEY `akill_id` (`akill_id`),
  UNIQUE KEY `mask` (`mask`),
  KEY `expires` (`expires`),
  KEY `akill_id_2` (`akill_id`),
  KEY `mask_2` (`mask`)
) TYPE=MyISAM COMMENT='AutoKills, time and expires are both seconds-since-epoch val';
# --------------------------------------------------------

#
# Table structure for table `auth`
#

DROP TABLE IF EXISTS `auth`;
CREATE TABLE `auth` (
  `auth_id` int(10) unsigned NOT NULL auto_increment,
  `code` varchar(41) NOT NULL default '',
  `name` varchar(32) NOT NULL default '',
  `command` enum('NICK_REG','NICK_DROP','NICK_SENDPASS','CHAN_SENDPASS') NOT NULL default 'NICK_REG',
  `params` text NOT NULL,
  `create_time` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`auth_id`),
  KEY `name` (`name`),
  KEY `command` (`command`),
  KEY `when` (`create_time`),
  KEY `code` (`code`)
) TYPE=MyISAM COMMENT='pending email-auth actions';
# --------------------------------------------------------

#
# Table structure for table `autojoin`
#

DROP TABLE IF EXISTS `autojoin`;
CREATE TABLE `autojoin` (
  `autojoin_id` int(10) unsigned NOT NULL auto_increment,
  `nick_id` int(10) unsigned NOT NULL default '0',
  `idx` int(10) unsigned NOT NULL default '0',
  `channel_id` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`autojoin_id`),
  UNIQUE KEY `channel` (`nick_id`,`channel_id`)
) TYPE=MyISAM;
# --------------------------------------------------------

#
# Table structure for table `chanaccess`
#

DROP TABLE IF EXISTS `chanaccess`;
CREATE TABLE `chanaccess` (
  `chanaccess_id` int(10) unsigned NOT NULL auto_increment,
  `channel_id` int(10) unsigned NOT NULL default '0',
  `idx` int(10) unsigned NOT NULL default '0',
  `level` mediumint(9) NOT NULL default '0',
  `nick_id` int(10) unsigned NOT NULL default '0',
  `added` int(10) unsigned NOT NULL default '0',
  `last_modified` int(10) unsigned NOT NULL default '0',
  `last_used` int(10) unsigned NOT NULL default '0',
  `memo_read` int(10) unsigned NOT NULL default '0',
  `added_by` char(32) NOT NULL default '',
  `modified_by` char(32) NOT NULL default '',
  PRIMARY KEY  (`chanaccess_id`),
  UNIQUE KEY `chanaccess_id` (`chanaccess_id`),
  UNIQUE KEY `channel_id_nick_id` (`channel_id`,`nick_id`),
  KEY `channel_id` (`channel_id`),
  KEY `nick_id` (`nick_id`),
  KEY `added` (`added`),
  KEY `last_used` (`last_used`),
  KEY `memo_read` (`memo_read`),
  KEY `added_by` (`added_by`),
  KEY `idx` (`idx`)
) TYPE=MyISAM COMMENT='Who has access in which channels';
# --------------------------------------------------------

#
# Table structure for table `chanlevel`
#

DROP TABLE IF EXISTS `chanlevel`;
CREATE TABLE `chanlevel` (
  `channel_id` int(10) unsigned NOT NULL default '0',
  `what` mediumint(8) NOT NULL default '0',
  `level` int(11) NOT NULL default '0',
  PRIMARY KEY  (`channel_id`,`what`)
) TYPE=MyISAM COMMENT='Access level required for various privileges';
# --------------------------------------------------------

#
# Table structure for table `channel`
#

DROP TABLE IF EXISTS `channel`;
CREATE TABLE `channel` (
  `channel_id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(64) NOT NULL default '',
  `founder` int(10) unsigned NOT NULL default '0',
  `successor` int(11) NOT NULL default '0',
  `founderpass` varchar(41) NOT NULL default '',
  `salt` varchar(17) NOT NULL default '',
  `description` text NOT NULL,
  `url` text NOT NULL,
  `email` text NOT NULL,
  `time_registered` int(10) unsigned NOT NULL default '0',
  `last_used` int(10) unsigned NOT NULL default '0',
  `founder_memo_read` int(10) unsigned NOT NULL default '0',
  `last_topic` text NOT NULL,
  `last_topic_setter` varchar(32) NOT NULL default '',
  `last_topic_time` int(10) unsigned NOT NULL default '0',
  `flags` int(11) NOT NULL default '0',
  `mlock_on` int(11) NOT NULL default '0',
  `mlock_off` int(11) NOT NULL default '0',
  `mlock_limit` int(10) unsigned NOT NULL default '0',
  `mlock_key` varchar(255) NOT NULL default '',
  `entry_message` text NOT NULL,
  `last_limit_time` int(10) unsigned NOT NULL default '0',
  `limit_offset` smallint(5) unsigned NOT NULL default '0',
  `limit_tolerance` smallint(5) unsigned NOT NULL default '0',
  `limit_period` smallint(5) unsigned NOT NULL default '0',
  `bantime` smallint(5) unsigned NOT NULL default '0',
  `last_sendpass_pass` varchar(41) NOT NULL default '',
  `last_sendpass_salt` varchar(17) NOT NULL default '',
  `last_sendpass_time` int(10) unsigned NOT NULL default '0',
  `floodserv_protected` smallint(2) NOT NULL default '0',
  PRIMARY KEY  (`channel_id`),
  UNIQUE KEY `channel_id` (`channel_id`),
  UNIQUE KEY `name` (`name`),
  KEY `founder` (`founder`),
  KEY `last_used` (`last_used`),
  KEY `flags` (`flags`)
) TYPE=MyISAM COMMENT='All registered channel data';
# --------------------------------------------------------

#
# Table structure for table `chansuspend`
#

DROP TABLE IF EXISTS `chansuspend`;
CREATE TABLE `chansuspend` (
  `chansuspend_id` int(10) unsigned NOT NULL auto_increment,
  `chan_id` int(10) unsigned NOT NULL default '0',
  `suspend_id` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`chansuspend_id`),
  UNIQUE KEY `suspend_id` (`suspend_id`),
  UNIQUE KEY `nicksuspend_id` (`chansuspend_id`),
  UNIQUE KEY `nick_id` (`chan_id`)
) TYPE=MyISAM COMMENT='Suspended nicks';
# --------------------------------------------------------

#
# Table structure for table `exception`
#

DROP TABLE IF EXISTS `exception`;
CREATE TABLE `exception` (
  `exception_id` int(10) unsigned NOT NULL auto_increment,
  `mask` varchar(255) NOT NULL default '',
  `ex_limit` smallint(6) unsigned NOT NULL default '0',
  `who` varchar(32) NOT NULL default '',
  `reason` varchar(255) NOT NULL default '',
  `time` int(10) unsigned NOT NULL default '0',
  `expires` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`exception_id`),
  UNIQUE KEY `mask` (`mask`),
  UNIQUE KEY `exception_id` (`exception_id`),
  KEY `ex_limit` (`ex_limit`),
  KEY `who` (`who`),
  KEY `time` (`time`),
  KEY `expires` (`expires`),
  KEY `mask_2` (`mask`)
) TYPE=MyISAM COMMENT='Exceptions to normal session limiting policy, time and expir';
# --------------------------------------------------------

#
# Table structure for table `memo`
#

DROP TABLE IF EXISTS `memo`;
CREATE TABLE `memo` (
  `memo_id` int(10) unsigned NOT NULL auto_increment,
  `owner` varchar(64) NOT NULL default '',
  `idx` int(10) unsigned NOT NULL default '0',
  `flags` mediumint(8) unsigned NOT NULL default '0',
  `time` int(11) unsigned NOT NULL default '0',
  `sender` varchar(32) NOT NULL default '0',
  `text` text NOT NULL,
  `rot13` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`memo_id`),
  UNIQUE KEY `memo_id` (`memo_id`),
  KEY `owner` (`owner`),
  KEY `flags` (`flags`),
  KEY `time` (`time`),
  KEY `sender` (`sender`),
  KEY `idx` (`idx`)
) TYPE=MyISAM COMMENT='Holds memos for channels and users';
# --------------------------------------------------------

#
# Table structure for table `memoinfo`
#

DROP TABLE IF EXISTS `memoinfo`;
CREATE TABLE `memoinfo` (
  `memoinfo_id` int(10) unsigned NOT NULL auto_increment,
  `owner` char(32) NOT NULL default '',
  `max` mediumint(8) NOT NULL default '0',
  PRIMARY KEY  (`memoinfo_id`),
  UNIQUE KEY `owner` (`owner`),
  UNIQUE KEY `memoinfo_id` (`memoinfo_id`)
) TYPE=MyISAM COMMENT='Info about user''s and channel''s memos';
# --------------------------------------------------------

#
# Table structure for table `news`
#

DROP TABLE IF EXISTS `news`;
CREATE TABLE `news` (
  `news_id` mediumint(8) unsigned NOT NULL auto_increment,
  `type` tinyint(3) unsigned NOT NULL default '0',
  `text` text NOT NULL,
  `who` varchar(32) NOT NULL default '',
  `time` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`news_id`),
  UNIQUE KEY `news_id` (`news_id`),
  KEY `type` (`type`),
  KEY `who` (`who`),
  KEY `time` (`time`)
) TYPE=MyISAM COMMENT='Stores logon and oper news (and maybe others later)';
# --------------------------------------------------------

#
# Table structure for table `nick`
#

DROP TABLE IF EXISTS `nick`;
CREATE TABLE `nick` (
  `nick_id` int(10) unsigned NOT NULL auto_increment,
  `nick` varchar(32) NOT NULL default '',
  `pass` varchar(41) NOT NULL default '',
  `salt` varchar(17) NOT NULL default '',
  `url` varchar(255) NOT NULL default '',
  `email` varchar(255) NOT NULL default '',
  `last_usermask` varchar(255) NOT NULL default '',
  `last_realname` varchar(255) NOT NULL default '',
  `last_quit` text NOT NULL,
  `last_quit_time` int(10) unsigned NOT NULL default '0',
  `time_registered` int(10) unsigned NOT NULL default '0',
  `last_seen` int(10) unsigned NOT NULL default '0',
  `status` mediumint(9) NOT NULL default '0',
  `link_id` int(10) unsigned NOT NULL default '0',
  `flags` int(11) NOT NULL default '0',
  `channelmax` mediumint(8) unsigned NOT NULL default '0',
  `language` mediumint(8) unsigned NOT NULL default '0',
  `id_stamp` int(10) unsigned NOT NULL default '0',
  `regainid` int(10) unsigned NOT NULL default '0',
  `last_sendpass_pass` varchar(41) NOT NULL default '',
  `last_sendpass_salt` varchar(17) NOT NULL default '',
  `last_sendpass_time` int(10) unsigned NOT NULL default '0',
  `cloak_string` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`nick_id`),
  UNIQUE KEY `nick` (`nick`),
  UNIQUE KEY `nick_id` (`nick_id`),
  KEY `last_usermask` (`last_usermask`),
  KEY `last_seen` (`last_seen`),
  KEY `status` (`status`),
  KEY `flags` (`flags`)
) TYPE=MyISAM COMMENT='All registered nicks';
# --------------------------------------------------------

#
# Table structure for table `nickaccess`
#

DROP TABLE IF EXISTS `nickaccess`;
CREATE TABLE `nickaccess` (
  `nickaccess_id` int(10) unsigned NOT NULL auto_increment,
  `nick_id` int(10) unsigned NOT NULL default '0',
  `idx` int(10) unsigned NOT NULL default '0',
  `userhost` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`nickaccess_id`),
  UNIQUE KEY `nickaccess_id` (`nickaccess_id`),
  UNIQUE KEY `nick_id` (`nick_id`,`userhost`),
  KEY `idx` (`idx`)
) TYPE=MyISAM COMMENT='Access lists for nicks';
# --------------------------------------------------------

#
# Table structure for table `nicksuspend`
#

DROP TABLE IF EXISTS `nicksuspend`;
CREATE TABLE `nicksuspend` (
  `nicksuspend_id` int(10) unsigned NOT NULL auto_increment,
  `nick_id` int(10) unsigned NOT NULL default '0',
  `suspend_id` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`nicksuspend_id`),
  UNIQUE KEY `nick_id` (`nick_id`),
  UNIQUE KEY `nicksuspend_id` (`nicksuspend_id`),
  UNIQUE KEY `suspend_id` (`suspend_id`)
) TYPE=MyISAM COMMENT='Suspended nicks';
# --------------------------------------------------------

#
# Table structure for table `oper`
#

DROP TABLE IF EXISTS `oper`;
CREATE TABLE `oper` (
  `oper_id` int(10) unsigned NOT NULL auto_increment,
  `nick_id` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`oper_id`),
  UNIQUE KEY `oper_id` (`oper_id`),
  UNIQUE KEY `nick_id` (`nick_id`)
) TYPE=MyISAM COMMENT='List of services opers';
# --------------------------------------------------------

#
# Table structure for table `private_tmp_access`
#

DROP TABLE IF EXISTS `private_tmp_access`;
CREATE TABLE `private_tmp_access` (
  `private_tmp_access_id` bigint(20) unsigned NOT NULL auto_increment,
  `nick_id` int(10) unsigned NOT NULL default '0',
  `userhost` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`private_tmp_access_id`),
  UNIQUE KEY `nick_id` (`nick_id`,`userhost`),
  UNIQUE KEY `private_tmp_access_id` (`private_tmp_access_id`)
) TYPE=MyISAM COMMENT='Access lists for nicks';
# --------------------------------------------------------

#
# Table structure for table `private_tmp_akick`
#

DROP TABLE IF EXISTS `private_tmp_akick`;
CREATE TABLE `private_tmp_akick` (
  `akick_id` int(10) unsigned NOT NULL default '0',
  `idx` mediumint(8) unsigned NOT NULL auto_increment,
  `mask` varchar(255) default NULL,
  `nick_id` int(10) unsigned NOT NULL default '0',
  `reason` text,
  `who` varchar(32) NOT NULL default '',
  `added` int(10) unsigned NOT NULL default '0',
  `last_used` int(10) unsigned NOT NULL default '0',
  `last_matched` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`idx`)
) TYPE=MyISAM COMMENT='Channel AutoKicks. nick_id=0 where mask is in use.';
# --------------------------------------------------------

#
# Table structure for table `private_tmp_autojoin`
#

DROP TABLE IF EXISTS `private_tmp_autojoin`;
CREATE TABLE `private_tmp_autojoin` (
  `autojoin_id` int(10) unsigned NOT NULL default '0',
  `idx` int(10) unsigned NOT NULL auto_increment,
  `channel_id` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`idx`)
) TYPE=MyISAM;
# --------------------------------------------------------

#
# Table structure for table `private_tmp_chanaccess`
#

DROP TABLE IF EXISTS `private_tmp_chanaccess`;
CREATE TABLE `private_tmp_chanaccess` (
  `chanaccess_id` int(10) unsigned NOT NULL default '0',
  `idx` int(10) unsigned NOT NULL auto_increment,
  `level` mediumint(9) NOT NULL default '0',
  `nick_id` int(10) unsigned NOT NULL default '0',
  `added` int(10) unsigned NOT NULL default '0',
  `last_modified` int(10) unsigned NOT NULL default '0',
  `last_used` int(10) unsigned NOT NULL default '0',
  `memo_read` int(10) unsigned NOT NULL default '0',
  `added_by` char(32) NOT NULL default '',
  `modified_by` char(32) NOT NULL default '',
  PRIMARY KEY  (`idx`)
) TYPE=MyISAM COMMENT='Who has access in which channels';
# --------------------------------------------------------

#
# Table structure for table `private_tmp_memo`
#

DROP TABLE IF EXISTS `private_tmp_memo`;
CREATE TABLE `private_tmp_memo` (
  `memo_id` int(10) unsigned NOT NULL auto_increment,
  `owner` varchar(64) NOT NULL default '',
  `idx` int(10) unsigned NOT NULL default '0',
  `flags` mediumint(8) unsigned NOT NULL default '0',
  `time` int(11) unsigned NOT NULL default '0',
  `sender` varchar(32) NOT NULL default '0',
  `text` text NOT NULL,
  `rot13` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`memo_id`),
  UNIQUE KEY `memo_id` (`memo_id`),
  KEY `owner` (`owner`),
  KEY `flags` (`flags`),
  KEY `time` (`time`),
  KEY `sender` (`sender`),
  KEY `idx` (`idx`)
) TYPE=MyISAM COMMENT='Holds memos for channels and users';
# --------------------------------------------------------

#
# Table structure for table `private_tmp_memo2`
#

DROP TABLE IF EXISTS `private_tmp_memo2`;
CREATE TABLE `private_tmp_memo2` (
  `memo_id` int(10) unsigned NOT NULL default '0',
  `idx` int(10) unsigned NOT NULL auto_increment,
  `flags` mediumint(8) unsigned NOT NULL default '0',
  `time` int(11) unsigned NOT NULL default '0',
  `sender` varchar(32) NOT NULL default '0',
  `text` text NOT NULL,
  `rot13` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`idx`)
) TYPE=MyISAM COMMENT='Holds memos for channels and users';
# --------------------------------------------------------

#
# Table structure for table `private_tmp_nickaccess2`
#

DROP TABLE IF EXISTS `private_tmp_nickaccess2`;
CREATE TABLE `private_tmp_nickaccess2` (
  `nickaccess_id` int(10) unsigned NOT NULL default '0',
  `idx` int(10) unsigned NOT NULL auto_increment,
  `userhost` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`idx`)
) TYPE=MyISAM COMMENT='Access lists for nicks';
# --------------------------------------------------------

#
# Table structure for table `private_tmp_quarantine`
#

DROP TABLE IF EXISTS `private_tmp_quarantine`;
CREATE TABLE `private_tmp_quarantine` (
  `quarantine_id` int(10) unsigned NOT NULL default '0',
  `idx` int(10) unsigned NOT NULL auto_increment,
  `regex` varchar(255) NOT NULL default '',
  `added_by` varchar(255) NOT NULL default '',
  `added_time` int(10) unsigned NOT NULL default '0',
  `reason` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`idx`)
) TYPE=MyISAM;
# --------------------------------------------------------

#
# Table structure for table `quarantine`
#

DROP TABLE IF EXISTS `quarantine`;
CREATE TABLE `quarantine` (
  `quarantine_id` int(10) unsigned NOT NULL auto_increment,
  `idx` int(10) unsigned NOT NULL default '0',
  `regex` varchar(255) NOT NULL default '',
  `added_by` varchar(255) NOT NULL default '',
  `added_time` int(10) unsigned NOT NULL default '0',
  `reason` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`quarantine_id`),
  UNIQUE KEY `regex` (`regex`),
  KEY `idx` (`idx`)
) TYPE=MyISAM;
# --------------------------------------------------------

#
# Table structure for table `session`
#

DROP TABLE IF EXISTS `session`;
CREATE TABLE `session` (
  `host` varchar(255) NOT NULL default '',
  `count` smallint(5) unsigned NOT NULL default '0',
  `killcount` smallint(5) unsigned NOT NULL default '0',
  PRIMARY KEY  (`host`),
  UNIQUE KEY `host` (`host`),
  KEY `count` (`count`)
) TYPE=MyISAM COMMENT='Online data regarding connections per host';
# --------------------------------------------------------

#
# Table structure for table `stat`
#

DROP TABLE IF EXISTS `stat`;
CREATE TABLE `stat` (
  `name` varchar(255) NOT NULL default '',
  `value` text NOT NULL,
  PRIMARY KEY  (`name`),
  UNIQUE KEY `name` (`name`)
) TYPE=MyISAM COMMENT='Various statistics';
# --------------------------------------------------------

#
# Table structure for table `suspend`
#

DROP TABLE IF EXISTS `suspend`;
CREATE TABLE `suspend` (
  `suspend_id` int(10) unsigned NOT NULL auto_increment,
  `who` varchar(32) NOT NULL default '',
  `reason` text NOT NULL,
  `suspended` int(10) unsigned NOT NULL default '0',
  `expires` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`suspend_id`),
  UNIQUE KEY `suspend_id` (`suspend_id`),
  KEY `who` (`who`),
  KEY `expires` (`expires`)
) TYPE=MyISAM COMMENT='Details of suspensions of nicks and channels';

