# Japanese language file (JIS encoding) / $BF|K\8l$N%a%C%;!<%8%U%!%$%k(B
#
# Services is copyright (c) 1996-1999 Andrew Church.
#     E-mail: <achurch@dragonfire.net>
# Services is copyright (c) 1999-2000 Andrew Kempe.
#     E-mail: <theshadow@shadowfire.org>
# This program is free but copyrighted software; see the file COPYING for
# details.

###########################################################################
#
# $B8@8l$NL>A0(B
#
###########################################################################

LANG_NAME
	$BF|K\8l!J(BJIS$B%3!<%I!K(B (Japanese / JIS encoding)

###########################################################################
#
# $BB?MQ$N%a%C%;!<%8(B
#
###########################################################################

# strftime()$B$NF|;~%U%)!<%^%C%H(B
STRFTIME_DATE_TIME_FORMAT
	%Y.%m.%d %H:%M:%S %Z
# strftime()$B$NG/7nF|%%U%%)!<%%^%%C%%H!J(B2$B!K(B
STRFTIME_SHORT_DATE_FORMAT
	%Y.%m.%d

# strftime()$B$N%%U%%)!<%%^%%C%%H$G;HMQ$5$l$k%%F%%-%%9%%H(B
STRFTIME_DAYS_SHORT
	$BF|(B
	$B7n(B
	$B2P(B
	$B?e(B
	$BLZ(B
	$B6b(B
	$BEZ(B
STRFTIME_DAYS_LONG
STRFTIME_MONTHS_SHORT
STRFTIME_MONTHS_LONG


# ChanServ$B!&(BNickServ$B$N>pJsI=$G;H$o$l$k%%F%%-%%9%%H(B
COMMA_SPACE
	$B!"(B


# $B%%(%%i!<$N%%a%%C%%;!<%%8(B
USER_RECORD_NOT_FOUND
	$B%%(%%i!<$,H/@8$7?J9T$G$-$^$;$s!#(B
UNKNOWN_COMMAND
	%s $B$H$$$&%%3%%^%%s%%I$OJ,$+$j$^$;$s!#(B
UNKNOWN_COMMAND_HELP
	%s $B$H$$$&%%3%%^%%s%%I$OJ,$+$j$^$;$s!#!V(B/msg %s HELP$B!W$rF~NO$7$F2<$5$$!#(B
SYNTAX_ERROR
	$B%%7%%s%%?%%/%%9!'(B%s
MORE_INFO
	$B!V(B/msg %s HELP %s$B!W$rF~NO$7$F2<$5$$!#(B
NO_HELP_AVAILABLE
	$B!V(B%s$B!W$N%%X%%k%%W$,$"$j$^$;$s!#(B

BAD_USERHOST_MASK
	$B%%^%%9%%/$r(B user@host $B$H$$$&7A$GF~NO$7$F2<$5$$!#(B

ACCESS_DENIED
	$B%%"%%/%%;%%9ITG=!#(B
PERMISSION_DENIED
	$B%%"%%/%%;%%9ITG=!#(B
PASSWORD_INCORRECT
	$B%%Q%%9%%o!<%%I$O0c$$$^$9!#(B

MORE_OBSCURE_PASSWORD
	$B$b$C$HITL@$J%%Q%%9%%o!<%%I$K$7$F2<$5$$!#%%Q%%9%%o!<%%I$O!"$*L>A0!&%%K%%C%%/Ey$N$h$&$JJ,$+$j$d$9$$$3$H$G$J$/!"#5J8;z0J>e$N%%Q%%9%%o!<%%I$K$7$F2<$5$$!#$^$?!"%%9%%Z!<%%9!J6uGr!K$O%%Q%%9%%o!<%%I$G;H$($^$;$s!#(B
PASSWORD_TRUNCATED
	$BCm0U!'(B$BF~NO$7$?%%Q%%9%%o!<%%I$,(B %d $BJ8;z$G@Z$i$l$^$7$?!#(B

NICK_NOT_REGISTERED
	$B%%K%%C%%/$OEPO?$5$l$F$$$^$;$s!#(B
NICK_NOT_REGISTERED_HELP
	$B%%K%%C%%/$OEPO?$5$l$F$$$^$;$s!#EPO?$7$?$$>l9g$O!V(B/msg %s HELP$B!W$rF~NO$7$F2<$5$$!#(B
NICK_X_NOT_REGISTERED
	%s $B$H$$$&%%K%%C%%/$OEPO?$5$l$F$$$^$;$s!#(B
NICK_X_NOT_IN_USE
	%s $B$H$$$&%%K%%C%%/$O8=:_;HMQ$5$l$F$$$^$;$s!#(B
NICK_X_FORBIDDEN
	%s $B$H$$$&%%K%%C%%/$r;HMQ6X;_$G$9!#(B
CHAN_X_NOT_REGISTERED
	%s $B$H$$$&%%A%%c%%s%%M%%k$OEPO?$5$l$F$$$^$;$s!#(B
CHAN_X_NOT_IN_USE
	%s $B$H$$$&%%A%%c%%s%%M%%k$O8=:_B8:_$7$^$;$s!#(B
CHAN_X_FORBIDDEN
	%s $B$H$$$&%%A%%c%%s%%M%%k$O;HMQ6X;_$G$9!#(B
NICK_IDENTIFY_REQUIRED
	$B$=$N%%3%%^%%s%%I$r;H$&$K$O%%Q%%9%%o!<%%I$rF~NO$9$kI,MW$,$"$j$^$9!#(B
	$B!V(B/msg %s IDENTIFY password$B!W$rF~NO$7$F2<$5$$!#(B
CHAN_IDENTIFY_REQUIRED
	$B$=$N%%3%%^%%s%%I$r;H$&$K$O%%Q%%9%%o!<%%I$rF~NO$9$kI,MW$,$"$j$^$9!#(B
	$B!V(B/msg %s IDENTIFY %s password$B!W$rF~NO$7$F2<$5$$!#(B

###########################################################################
#
# NickServ$B$N%%a%%C%%;!<%%8(B
#
###########################################################################

# $B<+F0E*$J%%a%%C%%;!<%%8(B
NICK_IS_REGISTERED
	$B$3$N%%K%%C%%/$OEPO?$5$l$F$$$^$9!#B>$N%%K%%C%%/$KJQ$($F2<$5$$!#(B
	$B!JJ]M-<T$J$i!"!V(B/msg %s IDENTIFY password$B!W$rF~NO$7$F2<$5$$!#!K(B
NICK_IS_SECURE
	$B$3$N%%K%%C%%/$OEPO?$5$lH_$i$l$F$$$^$9!#J]M-<T$J$i!"(B
	$B!V(B/msg %s IDENTIFY password$B!W$rF~NO$7$F2<$5$$!#(B
	$BJ]M-<T$G$J$1$l$P!"B>$N%%K%%C%%/$KJQ$($F2<$5$$!#(B
NICK_MAY_NOT_BE_USED
	$B$3$N%%K%%C%%/$O$*;H$$$K$J$l$^$;$s!#(B

# REGISTER$B$NH?1~(B
NICK_REGISTER_SYNTAX
	REGISTER password
NICK_REGISTRATION_FAILED
	$BEPO?$O<:GT$7$^$7$?!#(B
NICK_REG_PLEASE_WAIT
	REGISTER$B$N%%3%%^%%s%%I$r:FF~NO$9$kA0$K!"(B%d $BIC$*BT$A2<$5$$!#(B
NICK_CANNOT_BE_REGISTERED
	%s $B$H$$$&%%K%%C%%/$O;HMQ6X;_$G$9$N$G!"EPO?=PMh$^$;$s!#(B
NICK_ALREADY_REGISTERED
	%s $B$H$$$&%%K%%C%%/$O4{$KEPO?$5$l$F$$$^$9!#(B
NICK_REGISTERED
	%s $B$H$$$&%%K%%C%%/$OEPO?$5$l$^$7$?!#(B
NICK_PASSWORD_IS
	$B%%K%%C%%/$N%%Q%%9%%o!<%%I$O(B %s $B$G$9!#3P$($F$*$$$F2<$5$$!#(B

# IDENTIFY$B$NH?1~(B
NICK_IDENTIFY_SYNTAX
	IDENTIFY password
NICK_IDENTIFY_FAILED
	IDENTIFY$B$O<:GT$7$^$7$?!#(B
NICK_IDENTIFY_SUCCEEDED
	$B@5$7$$%%Q%%9%%o!<%%I$G$9!#(B

# DROP$B$NH?1~(B
NICK_DROP_SYNTAX
	DROP
NICK_DROPPED
	$B%%K%%C%%/$O>C5n$5$l$^$7$?!#(B
NICK_X_DROPPED
	%s $B$H$$$&%%K%%C%%/$NEPO?$O>C5n$5$l$^$7$?!#(B

# SET$B$NH?1~(B
NICK_SET_SYNTAX
	SET option parameters
NICK_SET_SERVADMIN_SYNTAX
	SET [nick] option parameters
NICK_SET_UNKNOWN_OPTION
	%s $B$H$$$&%%*%%W%%7%%g%%s$OJ,$+$j$^$;$s!#(B
NICK_SET_UNKNOWN_OPTION_OR_BAD_NICK
	%s $B$O%%*%%W%%7%%g%%s$G$bEPO?$7$?%%K%%C%%/$G$b$"$j$^$;$s!#(B

# SET PASSWORD$B$NH?1~(B
NICK_SET_PASSWORD_CHANGED
	$B%%Q%%9%%o!<%%I$OJQ99$5$l$^$7$?!#(B
NICK_SET_PASSWORD_CHANGED_TO
	$B%%Q%%9%%o!<%%I$O(B %s $B$KJQ99$5$l$^$7$?!#(B

# SET LANGUAGE$B$NH?1~(B
NICK_SET_LANGUAGE_SYNTAX
	SET LANGUAGE number
NICK_SET_LANGUAGE_UNKNOWN
	%d $BHV$N8@8l$OB8:_$7$^$;$s!#8@8lI=$r8+$k>l9g!"!V(Bmsg %s HELP SET LANGUAGE$B!W$rF~NO$7$F2<$5$$!#(B
NICK_SET_LANGUAGE_CHANGED
	$B8@8l$OF|K\8l!J(BJIS$B%%3!<%%I!K$KJQ99$5$l$^$7$?!#(B
	$BCm0U!'(B$B$3$N%%F%%-%%9%%H$O%%"%%a%%j%%+?M$K:n$i$l$^$7$?$N$G!"4V0c$$$OB?$$$G$7$g$&$,!"2fK}$7$F2<$5$$!#$b$7$3$N%%F%%-%%9%%H$rD>$7$FD:$1$l$P!"$I$&$>(B achurch@dragonfire.net $B$XO"Mm$r$*4j$$CW$7$^$9!#(B

# SET URL$B$NH?1~(B
NICK_SET_URL_CHANGED
	$B%%[!<%%`%%Z!<%%8$N%%"%%I%%l%%9$O(B %s $B$KJQ99$5$l$^$7$?!#(B
NICK_SET_URL_UNSET
	$B%%[!<%%`%%Z!<%%8$N%%"%%I%%l%%9$O>C5n$5$l$^$7$?!#(B

# SET EMAIL$B$NH?1~(B
NICK_SET_EMAIL_CHANGED
	$BEE;R%%a!<%%k$N%%"%%I%%l%%9$O(B %s $B$KJQ99$5$l$^$7$?!#(B

# SET SECURE$B$NH?1~(B
NICK_SET_SECURE_SYNTAX
	SET SECURE {ON | OFF}
NICK_SET_SECURE_ON
	$B%%;%%-%%e%%"%%*%%W%%7%%g%%s$O(B ON $B$G$9!#(B
NICK_SET_SECURE_OFF
	$B%%;%%-%%e%%"%%*%%W%%7%%g%%s$O(B OFF $B$G$9!#(B

# SET PRIVATE$B$NH?1~(B
NICK_SET_PRIVATE_SYNTAX
	SET PRIVATE {ON | OFF}
NICK_SET_PRIVATE_ON
	$B;dE*%%*%%W%%7%%g%%s$O(B ON $B$G$9!#(B
NICK_SET_PRIVATE_OFF
	$B;dE*%%*%%W%%7%%g%%s$O(B OFF $B$G$9!#(B

# SET HIDE$B$NH?1~(B
NICK_SET_HIDE_SYNTAX
	SET HIDE {EMAIL | USERMASK | QUIT} {ON | OFF}
NICK_SET_HIDE_EMAIL_ON
	$BEE;R%%a!<%%k$N%%"%%I%%l%%9$O(B %s $B$N(B INFO $BI=$GI=$l$^$;$s!#(B
NICK_SET_HIDE_EMAIL_OFF
	$BEE;R%%a!<%%k$N%%"%%I%%l%%9$O(B %s $B$N(B INFO $BI=$GI=$l$^$9!#(B
NICK_SET_HIDE_MASK_ON
	$BA0$N%%f!<%%6!<%%"%%I%%l%%9$O(B %s $B$N(B INFO $BI=$GI=$l$^$;$s!#(B
NICK_SET_HIDE_MASK_OFF
	$BA0$N%%f!<%%6!<%%"%%I%%l%%9$O(B %s $B$N(B INFO $BI=$GI=$l$^$9!#(B
NICK_SET_HIDE_QUIT_ON
	$BA0$N@ZCG%%a%%C%%;!<%%8$O(B %s $B$N(B INFO $BI=$GI=$l$^$;$s!#(B
NICK_SET_HIDE_QUIT_OFF
	$BA0$N@ZCG%%a%%C%%;!<%%8$O(B %s $B$N(B INFO $BI=$GI=$l$^$9!#(B

# SET NOEXPIRE$B$NH?1~(B
NICK_SET_NOEXPIRE_SYNTAX
	SET [nickname] NOEXPIRE {ON | OFF}
NICK_SET_NOEXPIRE_ON
	%s $B$H$$$&%%K%%C%%/$NEPO?$O@Z$l$^$;$s!#(B
NICK_SET_NOEXPIRE_OFF
	%s $B$H$$$&%%K%%C%%/$NEPO?$O@Z$l$^$9!#(B

# ACCESS$B$NH?1~(B
NICK_ACCESS_SYNTAX
	ACCESS {ADD | DEL | LIST} [mask]
NICK_ACCESS_REACHED_LIMIT
	$B%%K%%C%%/$N%%"%%/%%;%%9I=$N%%^%%9%%/?t$O(B %d $B$K8B$i$l$F$$$^$9$N$GDI2C$G$-$^$;$s!#(B
NICK_ACCESS_ALREADY_PRESENT
	%s $B$O$9$G$K%%"%%/%%;%%9I=$GB8:_$7$F$$$^$9!#(B
NICK_ACCESS_ADDED
	%s $B$O%%"%%/%%;%%9I=$KDI2C$5$l$^$7$?!#(B
NICK_ACCESS_NOT_FOUND
	%s $B$O%%"%%/%%;%%9I=$GB8:_$7$^$;$s!#(B
NICK_ACCESS_DELETED
	%s $B$O%%"%%/%%;%%9I=$+$i>C5n$5$l$^$7$?!#(B
NICK_ACCESS_LIST
	$B%%"%%/%%;%%9I=!'(B
NICK_ACCESS_LIST_X
	%s $B$N%%"%%/%%;%%9I=!'(B

# LINK$B$NH?1~(B
NICK_LINK_SYNTAX
	LINK nick password
NICK_LINK_DISABLED
	LINK $B$O$?$@:#;HMQIT2DG=$G$9!#(B
NICK_LINK_FAILED
	$B%%j%%s%%/$O<:GT$7$^$7$?!#(B
NICK_NO_LINK_SAME
	$BF1$8%%K%%C%%/$K%%j%%s%%/$G$-$^$;$s!#(B
NICK_LINKED
	$B%%K%%C%%/$O(B %s $B$K%%j%%s%%/$5$l$^$7$?!#(B

# UNLINK$B$NH?1~(B
NICK_UNLINK_SYNTAX
	LINK [nickname password]
NICK_UNLINK_FAILED
	$B%%"%%s%%j%%s%%/$O<:GT$7$^$7$?!#(B
NICK_NOT_LINKED
	$B%%K%%C%%/$O%%j%%s%%/$5$l$F$$$^$;$s!#(B
NICK_UNLINKED
	$B%%K%%C%%/$O(B %s $B$+$i%%"%%s%%j%%s%%/$5$l$^$7$?!#(B
NICK_X_NOT_LINKED
	%s $B$O%%j%%s%%/$5$l$F$$$^$;$s!#(B
NICK_X_UNLINKED
	%s $B$O(B %s $B$+$i%%"%%s%%j%%s%%/$5$l$^$7$?!#(B

# LISTLINKS$B$NH?1~(B
NICK_LISTLINKS_HEADER
	%s $B$K%%j%%s%%/$7$F$$$k%%K%%C%%/!'(B
NICK_LISTLINKS_FOOTER
	%d $B8D8+$D$+$j$^$7$?!#(B

# INFO$B$NH?1~(B
NICK_INFO_SYNTAX
	INFO nick
NICK_INFO_REALNAME
	%s $B$O(B %s $B$G$9(B
NICK_INFO_ADDRESS
	$BA0$N%%f!<%%6!<%%"%%I%%l%%9!'(B%s
NICK_INFO_TIME_REGGED
	$B!!!!!!!!EPO?$7$?F|;~!'(B%s (%s ago)
NICK_INFO_LAST_SEEN
	$B!!!!!!!!A0$N;HMQF|;~!'(B%s (%s ago)
NICK_INFO_LAST_QUIT
	$B!!A0$N@ZCG%%a%%C%%;!<%%8!'(B%s
NICK_INFO_URL
	$B!!!!!!!!%%[!<%%`%%Z!<%%8!'(B%s
NICK_INFO_EMAIL
	$BEE;R%%a!<%%k$N%%"%%I%%l%%9!'(B%s
NICK_INFO_OPTIONS
	$B!!!!!!!!!!%%*%%W%%7%%g%%s!'(B%s
NICK_INFO_NO_EXPIRE
	$B$3$N%%K%%C%%/$NEPO?$O@Z$l$^$;$s!#(B
# These strings MUST NOT be empty
NICK_INFO_OPT_SECURE
	$B%%;%%-%%e%%"(B
NICK_INFO_OPT_PRIVATE
	$B;dE*(B
NICK_INFO_OPT_NONE
	$BL5$7(B

# LIST$B$NH?1~(B
NICK_LIST_SYNTAX
	LIST pattern
NICK_LIST_SERVADMIN_SYNTAX
	LIST pattern [FORBID] [NOEXPIRE]
NICK_LIST_HEADER
	%s $B$K9g$&%%K%%C%%/I=!'(B
NICK_LIST_RESULTS
	LIST $B=*N;!#(B

# RECOVER$B$NH?1~(B
NICK_RECOVER_SYNTAX
	RECOVER nickname [password]
NICK_NO_RECOVER_SELF
	$B<+J,$r%%j%%+%%P!<=PMh$^$;$s!#(B
NICK_RECOVERED
	$B%%K%%C%%/$r;HMQ$7$F$$$?%%f!<%%6!<$O@ZCG$5$l$^$7$?!#(B
	1 $BJ,$N%%?%%$%%`%%"%%&%%H$,7P$DA0$K%%K%%C%%/$r;HMQ$7$?$$>l9g$O!"(B
	$B!V(B/msg %s RELEASE %s$B!W$rF~NO$7$F2<$5$$!#(B

# RELEASE$B$NH?1~(B
#@@@ $B!V%%[%%k%%I!W(B
NICK_RELEASE_SYNTAX
	RELEASE nickname [password]
NICK_RELEASE_NOT_HELD
	%s $B$O%%[%%k%%I$5$l$F$$$^$;$s!#(B
NICK_RELEASED
	%s $B$O2rJ|$5$l$^$7$?!#(B

# GHOST$B$NH?1~(B
NICK_GHOST_SYNTAX
	GHOST nickname [password]
NICK_NO_GHOST_SELF
	$B<+J,$r%%4!<%%9%%H=PMh$^$;$s!#(B
NICK_GHOST_KILLED
	%s $B$N%%4!<%%9%%H$O@ZCG$5$l$^$7$?!#(B

# FORBID$B$NH?1~(B
NICK_FORBID_SYNTAX
	FORBID nickname
NICK_FORBID_SUCCEEDED
	%s $B$H$$$&%%K%%C%%/$O;HMQ6X;_$K$5$l$^$7$?!#(B
NICK_FORBID_FAILED
	%s $B$H$$$&%%K%%C%%/$r;HMQ6X;_$K$9$k$N$,<:GT$7$^$7$?!#(B

###########################################################################
#
# ChanServ messages
#
###########################################################################

# $B%%"%%/%%;%%9%%l%%Y%%k$NL>>u(B
CHAN_LEVEL_AUTOOP
	$B%%A%%c%%s%%M%%k$KF~$k$H(B +o $B%%b!<%%I(B
CHAN_LEVEL_AUTOVOICE
	$B%%A%%c%%s%%M%%k$KF~$k$H(B +v $B%%b!<%%I(B
CHAN_LEVEL_AUTODEOP
	+o $B%%b!<%%I$r6X$8$k(B
CHAN_LEVEL_NOJOIN
	RESTRICTED $B$G$"$l$P%%A%%c%%s%%M%%k$KF~$l$J$$(B
CHAN_LEVEL_INVITE
	INVITE $B$N%%3%%^%%s%%I$,;H$($k(B
CHAN_LEVEL_AKICK
	AKICK $B$N%%3%%^%%s%%I$,;H$($k(B
CHAN_LEVEL_SET
	SET$B!J(BFOUNDER$B!&(BPASSWORD $B0J30!K$,;H$($k(B
CHAN_LEVEL_CLEAR
	CLEAR $B$N%%3%%^%%s%%I$,;H$($k(B
CHAN_LEVEL_UNBAN
	UNBAN $B$N%%3%%^%%s%%I$,;H$($k(B
CHAN_LEVEL_OPDEOP
	OP$B!&(BDEOP $B$N%%3%%^%%s%%I$,;H$($k(B
CHAN_LEVEL_ACCESS_LIST
	$B%%"%%/%%;%%9I=$r8+$k$3$H$,=PMh$k(B
CHAN_LEVEL_ACCESS_CHANGE
	$B%%"%%/%%;%%9I=$rJQ99$G$-$k(B
CHAN_LEVEL_MEMO_READ
	$B%%a%%b$,FI$a$k(B

# $B<+F0E*$J%%a%%C%%;!<%%8(B
CHAN_IS_REGISTERED
	$B$3$N%%A%%c%%s%%M%%k$OEPO?$5$l$F$$$^$9!#(B
CHAN_MAY_NOT_BE_USED
	$B$3$N%%A%%c%%s%%M%%k$O;HMQ6X;_$G$9!#(B
CHAN_NOT_ALLOWED_TO_JOIN
	$B$3$N%%A%%c%%s%%M%%k$K$OF~$l$^$;$s!#(B

# REGISTER$B$NH?1~(B
CHAN_REGISTER_SYNTAX
	REGISTER channel password description
CHAN_REGISTER_NOT_LOCAL
	$B%%m%%+%%k%%A%%c%%s%%M%%k$NEPO?$O=PMh$^$;$s!#(B
CHAN_MUST_REGISTER_NICK
	$B%%A%%c%%s%%M%%k$rEPO?$9$k$N$K$O!"%%K%%C%%/$rEPO?$9$kI,MW$,$"$j$^$9!#%%K%%C%%/EPO?>pJs$r8+$k>l9g!"!V(B/msg %s HELP$B!W$rF~NO$7$F2<$5$$!#(B
CHAN_MUST_IDENTIFY_NICK
	$B%%A%%c%%s%%M%%k$rEPO?$9$k$N$K$O!"%%K%%C%%/$N%%Q%%9%%o!<%%I$rF~NO$9$kI,MW$,$"$j$^$9!#!V(B/msg %s IDENTIFY password$B!W$rF~NO$7$F2<$5$$!#(B
CHAN_MAY_NOT_BE_REGISTERED
	%s $B$O;HMQ6X;_$G$9$N$G!"EPO?=PMh$^$;$s!#(B
CHAN_ALREADY_REGISTERED
	%s $B$O4{$KEPO?$5$l$F$$$^$9!#(B
CHAN_MUST_BE_CHANOP
	$B%%A%%c%%s%%M%%k$rEPO?$9$k$K$O!"$=$N%%*%%Z%%l%%?!<$G$"$kI,MW$,$"$j$^$9!#(B
CHAN_REACHED_CHANNEL_LIMIT
#@@@ $B$3$($k(B != reach
	%d $B%%A%%c%%s%%M%%k$N8B$j$r1[$($F$$$^$9$N$GEPO?$G$-$^$;$s!#(B
CHAN_EXCEEDED_CHANNEL_LIMIT
	%d $B%%A%%c%%s%%M%%k$N8B$j$r1[$($F$$$^$9$N$GEPO?$G$-$^$;$s!#(B
CHAN_REGISTRATION_FAILED
	$BEPO?$O<:GT$7$^$7$?!#(B
CHAN_REGISTERED
	%s $B$OEPO?$5$l$^$7$?!#(B
CHAN_PASSWORD_IS
	$B%%A%%c%%s%%M%%k$N%%Q%%9%%o!<%%I$O(B %s $B$G$9!#3P$($F$*$$$F2<$5$$!#(B

# IDENTIFY$B$NH?1~(B
CHAN_IDENTIFY_SYNTAX
	IDENTIFY channel password
CHAN_IDENTIFY_SUCCEEDED
#@@@
	$B@5$7$$%%Q%%9%%o!<%%I$G$9!#$3$l$+$i(B %s $B$G%%U%%!%%&%%s%%@!<$NFC8"$r;}$C$F$$$^$9!#(B

# DROP$B$NH?1~(B
CHAN_DROP_SYNTAX
	DROP channel
CHAN_DROPPED
	%s $B$H$$$&%%A%%c%%s%%M%%k$NEPO?$O>C5n$5$l$^$7$?!#(B

# SET$B$NH?1~(B
CHAN_SET_SYNTAX
	SET channel option parameters
CHAN_SET_UNKNOWN_OPTION
	%s $B$H$$$&%%*%%W%%7%%g%%s$OJ,$+$j$^$;$s!#(B

# SET FOUNDER$B$NH?1~(B
CHAN_SET_FOUNDER_TOO_MANY_CHANS
	%s $B$O%%A%%c%%s%%M%%k$N?t$N8B$j$r1[$($F$$$^$9$N$G%%U%%!%%&%%s%%@!<$rJQ99=PMh$^$;$s!#(B
CHAN_FOUNDER_CHANGED
	%s $B$N%%U%%!%%&%%s%%@!<$O(B %s $B$KJQ99$5$l$^$7$?!#(B

# SET SUCCESSOR$B$NH?1~(B
CHAN_SUCCESSOR_CHANGED
	%s $B$N<!4|%%U%%!%%&%%s%%@!<$O(B %s $B$KJQ99$5$l$^$7$?!#(B
CHAN_SUCCESSOR_UNSET
#@@@
	%s $B$N<!4|%%U%%!%%&%%s%%@!<@_Dj$O%%-%%c%%s%%;%%k$5$l$^$7$?!#(B

# SET PASSWORD$B$NH?1~(B
CHAN_PASSWORD_CHANGED
	%s $B$N%%Q%%9%%o!<%%I$OJQ99$5$l$^$7$?!#(B
CHAN_PASSWORD_CHANGED_TO
	%s $B$N%%Q%%9%%o!<%%I$O(B %s $B$KJQ99$5$l$^$7$?!#(B

# SET DESC$B$NH?1~(B
CHAN_DESC_CHANGED
	%s $B$NL>>u$O!V(B%s$B!W$KJQ99$5$l$^$7$?!#(B

# SET URL$B$NH?1~(B
CHAN_URL_CHANGED
	%s $B$N%%[!<%%`%%Z!<%%8$N%%"%%I%%l%%9$O(B %s $B$KJQ99$5$l$^$7$?!#(B
CHAN_URL_UNSET
	%s $B$N%%[!<%%`%%Z!<%%8$N%%"%%I%%l%%9$O>C5n$5$l$^$7$?!#(B

# SET EMAIL$B$NH?1~(B
CHAN_EMAIL_CHANGED
	%s $B$NEE;R%%a!<%%k$N%%"%%I%%l%%9$O(B %s $B$KJQ99$5$l$^$7$?!#(B
CHAN_EMAIL_UNSET
	%s $B$NEE;R%%a!<%%k$N%%"%%I%%l%%9$O>C5n$5$l$^$7$?!#(B

# SET ENTRYMSG$B$NH?1~(B
CHAN_ENTRY_MSG_CHANGED
	%s $B$NF~%%a%%C%%;!<%%8$OJQ99$5$l$^$7$?!#(B
CHAN_ENTRY_MSG_UNSET
	%s $B$NF~%%a%%C%%;!<%%8$O>C5n$5$l$^$7$?!#(B

# SET MLOCK$B$NH?1~(B
CHAN_SET_MLOCK_KEY_REQUIRED
	$B%%b!<%%I%%m%%C%%/$r(B +k $B$K$9$k$K$O%%Q%%i%%a!<%%?!<$,I,MW$G$9!#(B
CHAN_SET_MLOCK_LIMIT_REQUIRED
	$B%%b!<%%I%%m%%C%%/$r(B +l $B$K$9$k$K$O%%Q%%i%%a!<%%?!<$,I,MW$G$9!#(B
CHAN_SET_MLOCK_LIMIT_POSITIVE
	$B%%b!<%%I%%m%%C%%/$r(B +l $B$K$9$k$K$O@5?t$N%%Q%%i%%a!<%%?!<$,I,MW$G$9!#(B
CHAN_SET_MLOCK_UNKNOWN_CHAR
	%c $B$H$$$&%%b!<%%I$O4V0c$C$F$$$^$9$N$GL5;k$5$l$^$7$?!#(B
CHAN_MLOCK_CHANGED
	%s $B$N%%b!<%%I%%m%%C%%/$O(B %s $B$KJQ99$5$l$^$7$?!#(B
CHAN_MLOCK_REMOVED
	%s $B$N%%b!<%%I%%m%%C%%/$O>C5n$5$l$^$7$?!#(B

# SET KEEPTOPIC$B$NH?1~(B
CHAN_SET_KEEPTOPIC_SYNTAX
	SET channel KEEPTOPIC {ON | OFF}
CHAN_SET_KEEPTOPIC_ON
	$B%%H%%T%%C%%/5-O?%%*%%W%%7%%g%%s$O(B ON $B$G$9!#(B
CHAN_SET_KEEPTOPIC_OFF
	$B%%H%%T%%C%%/5-O?%%*%%W%%7%%g%%s$O(B OFF $B$G$9!#(B

# SET TOPICLOCK$B$NH?1~(B
CHAN_SET_TOPICLOCK_SYNTAX
	SET channel TOPICLOCK {ON | OFF}
CHAN_SET_TOPICLOCK_ON
	$B%%H%%T%%C%%/%%m%%C%%/%%*%%W%%7%%g%%s$O(B ON $B$G$9!#(B
CHAN_SET_TOPICLOCK_OFF
	$B%%H%%T%%C%%/%%m%%C%%/%%*%%W%%7%%g%%s$O(B OFF $B$G$9!#(B

# SET PRIVATE$B$NH?1~(B
CHAN_SET_PRIVATE_SYNTAX
	SET channel PRIVATE {ON | OFF}
CHAN_SET_PRIVATE_ON
	$B;dE*%%*%%W%%7%%g%%s$O(B ON $B$G$9!#(B
CHAN_SET_PRIVATE_OFF
	$B;dE*%%*%%W%%7%%g%%s$O(B OFF $B$G$9!#(B

# SET SECUREOPS$B$NH?1~(B
CHAN_SET_SECUREOPS_SYNTAX
	SET channel SECUREOPS {ON | OFF}
CHAN_SET_SECUREOPS_ON
	+o $B%%b!<%%I@)8f%%*%%W%%7%%g%%s$O(B ON $B$G$9!#(B
CHAN_SET_SECUREOPS_OFF
	+o $B%%b!<%%I@)8f%%*%%W%%7%%g%%s$O(B OFF $B$G$9!#(B

# SET LEAVEOPS$B$NH?1~(B
CHAN_SET_LEAVEOPS_SYNTAX
	SET channel LEAVEOPS {ON | OFF}
#@@@
CHAN_SET_LEAVEOPS_ON
	Leave ops option is now ON.
CHAN_SET_LEAVEOPS_OFF
	Leave ops option is now OFF.

# SET RESTRICTED$B$NH?1~(B
CHAN_SET_RESTRICTED_SYNTAX
	SET channel RESTRICTED {ON | OFF}
CHAN_SET_RESTRICTED_ON
	$B%%"%%/%%;%%9@)8f%%*%%W%%7%%g%%s$O(B ON $B$G$9!#(B
CHAN_SET_RESTRICTED_OFF
	$B%%"%%/%%;%%9@)8f%%*%%W%%7%%g%%s$O(B OFF $B$G$9!#(B

# SET SECURE$B$NH?1~(B
CHAN_SET_SECURE_SYNTAX
	SET channel SECURE {ON | OFF}
CHAN_SET_SECURE_ON
	$B%%;%%-%%e%%"%%*%%W%%7%%g%%s$O(B ON $B$G$9!#(B
CHAN_SET_SECURE_OFF
	$B%%;%%-%%e%%"%%*%%W%%7%%g%%s$O(B OFF $B$G$9!#(B

# SET NOEXPIRE$B$NH?1~(B
CHAN_SET_NOEXPIRE_SYNTAX
	SET channel NOEXPIRE {ON | OFF}
CHAN_SET_NOEXPIRE_ON
	%s $B$H$$$&%%A%%c%%s%%M%%k$NEPO?$O(B$B@Z$l$^$;$s(B$B!#(B
CHAN_SET_NOEXPIRE_OFF
	%s $B$H$$$&%%A%%c%%s%%M%%k$NEPO?$O(B$B@Z$l$^$9(B$B!#(B

# ACCESS messages
CHAN_ACCESS_SYNTAX
	ACCESS channel {ADD|DEL|LIST} [nick [level] | entry-list]
CHAN_ACCESS_LEVEL_NONZERO
	$B%%"%%/%%;%%9%%l%%Y%%k$O%%<%%m$G$"$C$F$O$J$j$^$;$s!#(B
CHAN_ACCESS_LEVEL_RANGE
	$B%%"%%/%%;%%9%%l%%Y%%k$O!"(B%d $B0J>e$G!"(B%d $B0J2<$N?t$K$7$F2<$5$$!#(B
CHAN_ACCESS_NICKS_ONLY
	$B%%A%%c%%s%%M%%k$N%%"%%/%%;%%9I=$KDI2C=PMh$k$N$OEPO?$5$l$?%%K%%C%%/$N$_$G$9!#(B
CHAN_ACCESS_REACHED_LIMIT
	$B%%A%%c%%s%%M%%k$N%%"%%/%%;%%9I=$N%%K%%C%%/?t$O(B %d $B$K8B$i$l$F$$$^$9$N$GDI2C$G$-$^$;$s!#(B
CHAN_ACCESS_LEVEL_UNCHANGED
	%s $B$N(B %s $B$G$N%%"%%/%%;%%9%%l%%Y%%k$O(B %d $B$+$iJQ99$5$l$^$;$s$G$7$?!#(B
CHAN_ACCESS_LEVEL_CHANGED
	%s $B$N(B %s $B$G$N%%"%%/%%;%%9%%l%%Y%%k$O(B %d $B$KJQ99$5$l$^$7$?!#(B
CHAN_ACCESS_ADDED
	%s $B$O(B %s $B$N%%"%%/%%;%%9I=$K!"%%l%%Y%%k(B %d $B$GDI2C$5$l$^$7$?!#(B
CHAN_ACCESS_NO_SUCH_ENTRY
	%d $BHV$N%%(%%s%%H%%j!<$O(B %s $B$N%%"%%/%%;%%9I=$KB8:_$7$^$;$s!#(B
CHAN_ACCESS_NO_MATCH
	%s $B$K9g$&%%(%%s%%H%%j!<$O(B %s $B$N%%"%%/%%;%%9I=$KB8:_$7$^$;$s!#(B
CHAN_ACCESS_NOT_FOUND
	%s $B$O(B %s $B$N%%"%%/%%;%%9I=$KB8:_$7$^$;$s!#(B
CHAN_ACCESS_DELETED
	%s $B$O(B %s $B$N%%"%%/%%;%%9I=$+$i>C5n$5$l$^$7$?!#(B
CHAN_ACCESS_DELETED_ONE
	%s $B$N%%"%%/%%;%%9I=$+$i%%(%%s%%H%%j!<$r(B 1 $B>C5n$7$^$7$?!#(B
CHAN_ACCESS_DELETED_SEVERAL
	%s $B$N%%"%%/%%;%%9I=$+$i%%(%%s%%H%%j!<$r(B %d $B>C5n$7$^$7$?!#(B
CHAN_ACCESS_LIST_EMPTY
	%s $B$N%%"%%/%%;%%9I=$O6u$G$9!#(B
CHAN_ACCESS_LIST_HEADER
	%s $B$N%%"%%/%%;%%9I=!'(B
	 $BHV9f(B  $B%%l%%Y%%k(B   $B%%K%%C%%/(B
CHAN_ACCESS_LIST_FORMAT
	%5d    %4d   %s%s%s%s

# AKICK$B$NH?1~(B
CHAN_AKICK_SYNTAX
	AKICK channel {ADD | DEL | LIST} [nick-or-usermask]
CHAN_AKICK_ALREADY_EXISTS
	%s $B$O4{$K(B %s $B$N<+F0=3I=$GB8:_$7$F$$$^$9!#(B
CHAN_AKICK_REACHED_LIMIT
	$B<+F0=3I=$N%%^%%9%%/$N?t$O(B %d $B$K8B$C$F$$$^$9$N$GDI2C$G$-$^$;$s!#(B
CHAN_AKICK_ADDED
	%s $B$O(B %s $B$N<+F0=3I=$KDI2C$5$l$^$7$?!#(B
CHAN_AKICK_NO_MATCH
	%s $B$K9g$&%%(%%s%%H%%j!<$O(B %s $B$N<+F0=3I=$GB8:_$7$^$;$s!#(B
CHAN_AKICK_DELETED
	%s $B$O(B %s $B$N<+F0=3I=$+$i>C5n$5$l$^$7$?!#(B
CHAN_AKICK_DELETED_ONE
	%s $B$N<+F0=3I=$+$i%%(%%s%%H%%j!<$r(B 1 $B>C5n$7$^$7$?!#(B
CHAN_AKICK_DELETED_SEVERAL
	%s $B$N<+F0=3I=$+$i%%(%%s%%H%%j!<$r(B %d $B>C5n$7$^$7$?!#(B
CHAN_AKICK_LIST_EMPTY
	%s $B$N<+F0=3I=$O6u$G$9!#(B
CHAN_AKICK_LIST_HEADER
	%s$B$N<+F0=3I=!'(B
CHAN_AKICK_LIST_FORMAT
	  %3d %s%s

# LEVELS$B$NH?1~(B
CHAN_LEVELS_SYNTAX
	LEVELS channel {SET | DIS[ABLE] | LIST | RESET} [item [level]]
CHAN_LEVELS_RANGE
	$B%%l%%Y%%k$O!"(B%d $B0J>e$G!"(B%d $B0J2<$N?t$rF~NO$7$F2<$5$$!#(B
CHAN_LEVELS_CHANGED
	%s $B$N%%l%%Y%%k$r(B %s $B$G(B %d $B$KJQ99$5$l$^$7$?!#(B
CHAN_LEVELS_UNKNOWN
	%s $B$H$$$&%%l%%Y%%k$OJ,$+$j$^$;$s!#%%l%%Y%%kI=$r8+$k>l9g$O!"!V(B/msg %s HELP LEVELS DESC$B!W$rF~NO$7$F2<$5$$!#(B
CHAN_LEVELS_LIST_HEADER
	%s $B$N%%l%%Y%%kI=!'(B
CHAN_LEVELS_LIST_DISABLED
	    %-*s  ($BITF0(B)
CHAN_LEVELS_LIST_NORMAL
	    %-*s  %d
CHAN_LEVELS_RESET
	%s $B$N%%l%%Y%%kI=$O4{DjCM$KLa$5$l$^$7$?!#(B

# INFO$B$NH?1~(B
CHAN_INFO_SYNTAX
	INFO channel
CHAN_INFO_HEADER
	%s$B$N>pJs!'(B
CHAN_INFO_FOUNDER
	$B!!!!!!!!%%U%%!%%&%%s%%@!<!'(B%s (%s)
CHAN_INFO_NO_FOUNDER
	$B!!!!!!!!%%U%%!%%&%%s%%@!<!'(B%s
CHAN_INFO_DESCRIPTION
	$B!!!!!!!!!!!!!!!!L>>u!'(B%s
CHAN_INFO_TIME_REGGED
	$B!!!!!!!!EPO?$7$?F|;~!'(B%s
CHAN_INFO_LAST_USED
	$B!!A0$N;HMQ$5$l$?F|;~!'(B%s
CHAN_INFO_LAST_TOPIC
	$B!!!!!!!!A0$N%%H%%T%%C%%/!'(B%s
CHAN_INFO_TOPIC_SET_BY
	$B!!!!%%H%%T%%C%%/$N@_Dj<T!'(B%s
CHAN_INFO_URL
	$B!!!!!!!!%%[!<%%`%%Z!<%%8!'(B%s
CHAN_INFO_EMAIL
	$BEE;R%%a!<%%k$N%%"%%I%%l%%9!'(B%s
CHAN_INFO_OPTIONS
	$B!!!!!!!!!!%%*%%W%%7%%g%%s!'(B%s
CHAN_INFO_OPT_PRIVATE
	$B;dE*(B
CHAN_INFO_OPT_KEEPTOPIC
	$B%%H%%T%%C%%/5-O?(B
CHAN_INFO_OPT_TOPICLOCK
	$B%%H%%T%%C%%/%%m%%C%%/(B
CHAN_INFO_OPT_SECUREOPS
	+o $B%%b!<%%I@)8f(B
CHAN_INFO_OPT_LEAVEOPS
#@@@
CHAN_INFO_OPT_RESTRICTED
	$B%%"%%/%%;%%9@)8f(B
CHAN_INFO_OPT_SECURE
	$B%%;%%-%%e%%"(B
CHAN_INFO_OPT_NONE
	$BL5$7(B
CHAN_INFO_MODE_LOCK
	$B!!!!!!!!%%b!<%%I%%m%%C%%/!'(B%s

# LIST$B$NH?1~(B
CHAN_LIST_SYNTAX
	LIST pattern
CHAN_LIST_HEADER
	%s $B$K9g$&%%A%%c%%s%%M%%kI=!'(B
CHAN_LIST_END
	LIST $B=*N;!#(B

# INVITE$B$NH?1~(B
CHAN_INVITE_SYNTAX
	INVITE channel

# OP$B$NH?1~(B
CHAN_OP_SYNTAX
	OP channel nick

# DEOP$B$NH?1~(B
CHAN_DEOP_SYNTAX
	DEOP channel nick

# UNBAN$B$NH?1~(B
CHAN_UNBAN_SYNTAX
	UNBAN channel
CHAN_UNBANNED
	%s $B$G$NDIJ|%%^%%9%%/$O2r$+$l$^$7$?!#(B

# CLEAR$B$NH?1~(B
CHAN_CLEAR_SYNTAX
	CLEAR channel what
CHAN_CLEARED_BANS
	%s $B$NDIJ|%%^%%9%%/$OA4$F>C5n$5$l$^$7$?!#(B
CHAN_CLEARED_MODES
	%s $B$N%%b!<%%I$O4{DjCM$KLa$5$l$^$7$?!#(B
CHAN_CLEARED_OPS
	+o $B%%b!<%%I$O(B %s $B$+$i>C5n$5$l$^$7$?!#(B
CHAN_CLEARED_VOICES
	+v $B%%b!<%%I$O(B %s $B$+$i>C5n$5$l$^$7$?!#(B
CHAN_CLEARED_USERS
	%s $B$N%%f!<%%6!<$OA4$F=3$j=P$5$l$^$7$?!#(B

# FORBID$B$NH?1~(B
CHAN_FORBID_SYNTAX
	FORBID channel
CHAN_FORBID_SUCCEEDED
	%s $B$O;HMQ6X;_$K$5$l$^$7$?!#(B
CHAN_FORBID_FAILED
	%s $B$r;HMQ6X;_$K$9$k$N$,<:GT$7$^$7$?!#(B

###########################################################################
#
# MemoServ$B$N%%a%%C%%;!<%%8(B
#
###########################################################################

# $B<+F0E*$J%%a%%C%%;!<%%8(B
MEMO_HAVE_NEW_MEMO
	$B?7%%a%%b$,(B1$B$"$j$^$9!#(B
MEMO_HAVE_NEW_MEMOS
	$B?7%%a%%b$,(B%d$B$"$j$^$9!#(B
MEMO_TYPE_READ_LAST
	$BFI$`$N$K$O!V(B/msg %s READ LAST$B!W$rF~NO$7$F2<$5$$!#(B
MEMO_TYPE_READ_NUM
	$BFI$`$N$K$O!V(B/msg %s READ %d$B!W$rF~NO$7$F2<$5$$!#(B
MEMO_TYPE_LIST_NEW
	$BI=$9$N$K$O!V(B/msg %s LIST NEW$B!W$rF~NO$7$F2<$5$$!#(B
MEMO_AT_LIMIT
	$BCm0U!'(B$B%%a%%b$N?t$N8B$j!J(B%d$B!K$K$J$j$^$7$?!#%%a%%b$r0l$D0J>e>C5n$9$k$^$G?7$?$J%%a%%b$OFO$-$^$;$s!#(B
MEMO_OVER_LIMIT
	$BCm0U!'(B$B%%a%%b$N?t$N8B$j!J(B%d$B!K$r2a$.$^$7$?!#%%a%%b$r>C5n$9$k$^$G?7$?$J%%a%%b$OFO$-$^$;$s!#(B
MEMO_NEW_MEMO_ARRIVED
	%s$B$+$i%%a%%b$,FO$-$^$7$?!#(B
	$BFI$`$N$K$O!V(B/msg %s READ %d$B!W$rF~NO$7$F2<$5$$!#(B

# $BB?MQ$N%%a%%C%%;!<%%8(B
MEMO_HAVE_NO_MEMOS
	$B%%a%%b$,$"$j$^$;$s!#(B
MEMO_X_HAS_NO_MEMOS
	%s $B$O%%a%%b$,$"$j$^$;$s!#(B

# SEND$B$NH?1~(B
MEMO_SEND_SYNTAX
	SEND {nick | channel} memo-text
MEMO_SEND_PLEASE_WAIT
	SEND $B$N%%3%%^%%s%%I$r:FF~NO$9$kA0$K(B %d $BIC$*BT$A2<$5$$!#(B
MEMO_X_HAS_TOO_MANY_MEMOS
	%s $B$O8=:_%%a%%b$,B?$9$.$^$9$N$G?7%%a%%b$r<u?.$G$-$^$;$s!#(B
MEMO_X_GETS_NO_MEMOS
	%s $B$O%%a%%b$r<u?.$G$-$^$;$s!#(B
MEMO_SENT
	%s $B$K%%a%%b$rAw?.$^$7$?!#(B

# LIST$B$NH?1~(B
MEMO_LIST_SYNTAX
	LIST [channel] [list | NEW]
MEMO_HAVE_NO_NEW_MEMOS
	$B?7%%a%%b$,$"$j$^$;$s!#(B
MEMO_X_HAS_NO_NEW_MEMOS
	%s $B$O?7%%a%%b$,$"$j$^$;$s!#(B
MEMO_LIST_MEMOS
	%s $B$N%%a%%bI=!#FI$`$K$O!V(B/msg %s READ num$B!W(B
MEMO_LIST_NEW_MEMOS
	%s $B$N?7%%a%%bI=!#FI$`$K$O!V(B/msg %s READ num$B!W(B
MEMO_LIST_CHAN_MEMOS
	%s $B$N%%a%%bI=!#FI$`$K$O!V(B/msg %s READ %s num$B!W(B
MEMO_LIST_CHAN_NEW_MEMOS
	%s $B$N?7%%a%%bI=!#FI$`$K$O!V(B/msg %s READ %s num$B!W(B
MEMO_LIST_HEADER
	 $BHV9f(B   $B:9=P?M(B            $BF|;~(B
MEMO_LIST_FORMAT
	%c%4d   %-16s  %s

# READ$B$NH?1~(B
MEMO_READ_SYNTAX
	READ [channel] {list | LAST | NEW}
MEMO_HEADER
	%d $BHV$N%%a%%b!J(B%s $B$+$i(B %s $B$KAw?.$5$l$?!K!#>C$9$K$O!V(B/msg %s DEL %d$B!W(B
MEMO_CHAN_HEADER
	%d $BHV$N%%a%%b!J(B%s $B$+$i(B %s $B$KAw?.$5$l$?!K!#>C$9$K$O!V(B/msg %s DEL %s %d$B!W(B
MEMO_TEXT
	%s

# DEL$B$NH?1~(B
MEMO_DEL_SYNTAX
	DEL [channel] {num | list | ALL}
MEMO_DELETED_NONE
	$BF~NO$7$?HV9f$K9g$&%%a%%b$O$"$j$^$;$s!#(B

# SET$B$NH?1~(B
MEMO_SET_SYNTAX
	SET option parameters
MEMO_SET_UNKNOWN_OPTION
	%s $B$H$$$&%%*%%W%%7%%g%%s$OJ,$+$j$^$;$s!#(B

# SET NOTIFY$B$NH?1~(B
MEMO_SET_NOTIFY_SYNTAX
	SET NOTIFY {ON | LOGON | NEW | OFF}
MEMO_SET_NOTIFY_ON
	%s $B$O$3$l$+$i%%m%%0%%*%%s$9$k;~$K?7%%a%%b$,$"$l$PJs9p$7!"%%a%%b$,FO$/;~$K$bJs9p$7$^$9!#(B
MEMO_SET_NOTIFY_LOGON
	%s $B$O$3$l$+$i%%m%%0%%*%%s$9$k;~$H(BIRC$B$KLa$k;~$K?7%%a%%b$,$"$l$PJs9p$7$^$9!#(B
MEMO_SET_NOTIFY_NEW
	%s $B$O$3$l$+$i%%a%%b$,FO$/;~$KJs9p$7$^$9!#(B
MEMO_SET_NOTIFY_OFF
	%s $B$O$3$l$+$i%%a%%b$NJs9p$r$7$^$;$s!#(B

# SET LIMIT$B$NH?1~(B
MEMO_SET_LIMIT_SYNTAX
	SET LIMIT [channel] limit
MEMO_SET_LIMIT_SERVADMIN_SYNTAX
	SET LIMIT [user | channel] {limit | NONE} [HARD]
MEMO_SET_YOUR_LIMIT_FORBIDDEN
	$B%%a%%b$N?t$N8B$j$rJQ99$G$-$^$;$s!#(B
MEMO_SET_LIMIT_FORBIDDEN
	%s $B$N%%a%%b$N?t$N8B$j$rJQ99$G$-$^$;$s!#(B
MEMO_SET_YOUR_LIMIT_TOO_HIGH
	$B%%a%%b$N?t$N8B$j$O!"%%<%%m$+(B %d $B0J2<$N@5?t$K$7$+@_Dj$G$-$^$;$s!#(B
MEMO_SET_LIMIT_TOO_HIGH
	%s $B$N%%a%%b$N?t$N8B$j$O!"%%<%%m$+(B %d $B0J2<$N@5?t$K$7$+@_Dj$G$-$^$;$s!#(B
MEMO_SET_LIMIT_OVERFLOW
	$B8B$j$N?tCM$OBg$-$9$.$^$9$N$G!"(B%d $B$K$J$j$^$9!#(B
MEMO_SET_YOUR_LIMIT
	$B%%a%%b$N?t$N8B$j$O(B %d $B$K@_Dj$5$l$^$7$?!#(B
MEMO_SET_YOUR_LIMIT_ZERO
	$B$3$l$+$i%%a%%b$r<u?.$7$^$;$s!#(B
MEMO_UNSET_YOUR_LIMIT
	$B%%a%%b$N?t$N8B$j$OITF0$K$5$l$^$7$?!#(B
MEMO_SET_LIMIT
	%s $B$N%%a%%b$N?t$N8B$j$O(B %d $B$K@_Dj$5$l$^$7$?!#(B
MEMO_SET_LIMIT_ZERO
	%s $B$N%%a%%b$N?t$N8B$j$O(B 0 $B$K@_Dj$5$l$^$7$?!#(B
MEMO_UNSET_LIMIT
	%s $B$N%%a%%b$N?t$N8B$j$OITF0$K$5$l$^$7$?!#(B

# INFO$B$NH?1~(B
MEMO_INFO_NO_MEMOS
	$B%%a%%b$O$"$j$^$;$s!#(B
MEMO_INFO_MEMO
	$B%%a%%b$,(B 1 $B$"$j$^$9!#(B
MEMO_INFO_MEMO_UNREAD
	$B%%a%%b$,(B 1 $B$"$j!"$^$@FI$s$G$$$^$;$s!#(B
MEMO_INFO_MEMOS
	$B%%a%%b$,(B %d $B$"$j$^$9!#(B
MEMO_INFO_MEMOS_ONE_UNREAD
	$B%%a%%b$,(B %d $B$"$j!"(B1 $B$OFI$s$G$$$^$;$s!#(B
MEMO_INFO_MEMOS_SOME_UNREAD
	$B%%a%%b$,(B %d $B$"$j!"(B%d $B$OFI$s$G$$$^$;$s!#(B
MEMO_INFO_MEMOS_ALL_UNREAD
	$B%%a%%b$,(B %d $B$"$j!"A4$F$OFI$s$G$$$^$;$s!#(B
MEMO_INFO_LIMIT
	$B%%a%%b$N?t$N8B$j$O(B %d $B$G$9!#(B
MEMO_INFO_HARD_LIMIT
	$B%%a%%b$N?t$N8B$j$O(B %d $B$G!"$=$N8B$j$OJQ99$G$-$^$;$s!#(B
MEMO_INFO_LIMIT_ZERO
	$B%%a%%b$N?t$N8B$j$O(B 0 $B$G$9$N$G!"%%a%%b$r<u$1$i$l$^$;$s!#(B
MEMO_INFO_HARD_LIMIT_ZERO
	$B%%a%%b$N?t$N8B$j$O(B 0 $B$G$9$N$G!"%%a%%b$r<u$1$i$l$^$;$s!#$=$N8B$j$OJQ99$G$-$^$;$s!#(B
MEMO_INFO_NO_LIMIT
	$B%%a%%b$N?t$O8B$i$l$F$$$^$;$s!#(B
MEMO_INFO_X_NO_MEMOS
	%s $B$O%%a%%b$,$"$j$^$;$s!#(B
MEMO_INFO_X_MEMO
	%s $B$O%%a%%b$,(B 1 $B$"$j$^$9!#(B
MEMO_INFO_X_MEMO_UNREAD
	%s $B$O%%a%%b$,(B 1 $B$"$j!"$^$@FI$s$G$$$^$;$s!#(B
MEMO_INFO_X_MEMOS
	%s $B$O%%a%%b$,(B %d $B$"$j$^$9!#(B
MEMO_INFO_X_MEMOS_ONE_UNREAD
	%s $B$O%%a%%b$,(B %d $B$"$j!"(B1 $B$OFI$s$G$$$^$;$s!#(B
MEMO_INFO_X_MEMOS_SOME_UNREAD
	%s $B$O%%a%%b$,(B %d $B$"$j!"(B%d $B$OFI$s$G$$$^$;$s!#(B
MEMO_INFO_X_MEMOS_ALL_UNREAD
	%s $B$O%%a%%b$,(B %d $B$"$j!"A4$F$OFI$s$G$$$^$;$s!#(B
MEMO_INFO_X_LIMIT
	%s $B$N%%a%%b$N?t$N8B$j$O(B %d $B$G$9!#(B
MEMO_INFO_X_HARD_LIMIT
	%s $B$N%%a%%b$N?t$N8B$j$O(B %d $B$G!"8B$j$rJQ99$G$-$^$;$s!#(B
MEMO_INFO_X_NO_LIMIT
	%s $B$N%%a%%b$N?t$O8B$i$l$F$$$^$;$s!#(B

###########################################################################
#
# OperServ$B$N%%a%%C%%;!<%%8(B
#
###########################################################################

# $BB?MQ$N%%a%%C%%;!<%%8(B
OPER_BOUNCY_MODES
	Services$B$O%%b!<%%I$rJQ99$G$-$^$;$s!#%%5!<%%P!<$N@_Dj$r%%A%%'%%C%%/$7$F2<$5$$!#(B
OPER_BOUNCY_MODES_U_LINE
	Services$B$O%%b!<%%I$rJQ99$G$-$^$;$s!#%%5!<%%P!<$N!V(BU:$B%%i%%$%%s!W$N@_Dj$r%%A%%'%%C%%/$7$F2<$5$$!#(B

# GLOBAL$B$NH?1~(B
OPER_GLOBAL_SYNTAX
	GLOBAL message

# STATS$B$NH?1~(B
OPER_STATS_UNKNOWN_OPTION
	%s $B$H$$$&(BSTATS$B$N%%*%%W%%7%%g%%s$OJ,$+$j$^$;$s!#(B
OPER_STATS_CURRENT_USERS
	$B8=:_%%f!<%%6!<?t!'(B%d
	$B%%*%%Z%%l!<%%?!<?t!'(B%d
OPER_STATS_MAX_USERS
	$B:GBg%%f!<%%6!<?t!'(B%d$B!J(B%s$B!K(B
OPER_STATS_UPTIME_DHMS
	$B5/F0$+$i!!!!!!!'(B%d$BF|(B%02d$B;~4V(B%02d$BJ,(B
OPER_STATS_UPTIME_1DHMS
	$B5/F0$+$i!!!!!!!'(B%d$BF|(B%02d$B;~4V(B%02d$BJ,(B
OPER_STATS_UPTIME_HMS
	$B5/F0$+$i!!!!!!!'(B%02d$B;~4V(B%02d$BJ,(B
OPER_STATS_UPTIME_HM1S
	$B5/F0$+$i!!!!!!!'(B%02d$B;~4V(B%02d$BJ,(B
OPER_STATS_UPTIME_H1MS
	$B5/F0$+$i!!!!!!!'(B%02d$B;~4V(B%02d$BJ,(B
OPER_STATS_UPTIME_H1M1S
	$B5/F0$+$i!!!!!!!'(B%02d$B;~4V(B%02d$BJ,(B
OPER_STATS_UPTIME_1HMS
	$B5/F0$+$i!!!!!!!'(B%02d$B;~4V(B%02d$BJ,(B
OPER_STATS_UPTIME_1HM1S
	$B5/F0$+$i!!!!!!!'(B%02d$B;~4V(B%02d$BJ,(B
OPER_STATS_UPTIME_1H1MS
	$B5/F0$+$i!!!!!!!'(B%02d$B;~4V(B%02d$BJ,(B
OPER_STATS_UPTIME_1H1M1S
	$B5/F0$+$i!!!!!!!'(B%02d$B;~4V(B%02d$BJ,(B
OPER_STATS_UPTIME_MS
	$B5/F0$+$i!!!!!!!'(B%02d$BJ,(B%02d$BIC(B
OPER_STATS_UPTIME_M1S
	$B5/F0$+$i!!!!!!!'(B%02d$BJ,(B%02d$BIC(B
OPER_STATS_UPTIME_1MS
	$B5/F0$+$i!!!!!!!'(B%02d$BJ,(B%02d$BIC(B
OPER_STATS_UPTIME_1M1S
	$B5/F0$+$i!!!!!!!'(B%02d$BJ,(B%02d$BIC(B
OPER_STATS_USER_MEM
	$B%%f!<%%6!<!!!'(B%6d$B%%l%%3!<%%I!&(B%5d$B%%-%%m(B
OPER_STATS_CHANNEL_MEM
	$B%%A%%c%%s%%M%%k!'(B%6d$B%%l%%3!<%%I!&(B%5d$B%%-%%m(B
OPER_STATS_NICKSERV_MEM
	NickServ$B!!!'(B%6d$B%%l%%3!<%%I!&(B%5d$B%%-%%m(B
OPER_STATS_CHANSERV_MEM
	ChanServ$B!!!'(B%6d$B%%l%%3!<%%I!&(B%5d$B%%-%%m(B
OPER_STATS_OPERSERV_MEM
	OperServ$B!!!'(B%6d$B%%l%%3!<%%I!&(B%5d$B%%-%%m(B
OPER_STATS_AKILL_COUNT
	$B<+F0@ZCG%%^%%9%%/$N?t!'(B%d
OPER_STATS_AKILL_EXPIRE_DAYS
	$B<+F0@ZCG%%^%%9%%/$N4{Dj<wL?!'(B%d$BF|(B
OPER_STATS_AKILL_EXPIRE_DAY
	$B<+F0@ZCG%%^%%9%%/$N4{Dj<wL?!'(B%d$BF|(B
OPER_STATS_AKILL_EXPIRE_HOURS
	$B<+F0@ZCG%%^%%9%%/$N4{Dj<wL?!'(B%d$B;~4V(B
OPER_STATS_AKILL_EXPIRE_HOUR
	$B<+F0@ZCG%%^%%9%%/$N4{Dj<wL?!'(B%d$B;~4V(B
OPER_STATS_AKILL_EXPIRE_MINS
	$B<+F0@ZCG%%^%%9%%/$N4{Dj<wL?!'(B%d$BJ,(B
OPER_STATS_AKILL_EXPIRE_MIN
	$B<+F0@ZCG%%^%%9%%/$N4{Dj<wL?!'(B%d$BJ,(B
OPER_STATS_AKILL_EXPIRE_NONE
	$B<+F0@ZCG%%^%%9%%/$N4{Dj<wL?!'(B$BL5$7(B

# MODE$B$NH?1~(B
OPER_MODE_SYNTAX
	MODE channel modes

# CLEARMODES$B$NH?1~(B
OPER_CLEARMODES_SYNTAX
	CLEARMODES channel [ALL]
OPER_CLEARMODES_DONE
	%s $B$NF~@Z%%b!<%%I$HDIJ|%%^%%9%%/$O%%/%%j%%"$5$l$^$7$?!#(B
OPER_CLEARMODES_ALL_DONE
	%s $B$N%%b!<%%I$OA4$F%%/%%j%%"$5$l$^$7$?!#(B

# KICK$B$NH?1~(B
OPER_KICK_SYNTAX
	KICK channel user reason

# ADMIN$B$NH?1~(B
OPER_ADMIN_SYNTAX
	ADMIN {ADD | DEL | LIST} [nick]
OPER_ADMIN_ADD_SYNTAX
	ADMIN ADD nick
OPER_ADMIN_DEL_SYNTAX
	ADMIN ADD nick
OPER_ADMIN_EXISTS
	%s $B$O4{$K(B Services $B$N%%"%%I%%_%%sI=$GB8:_$7$F$$$^$9!#(B
OPER_ADMIN_ADDED
	%s $B$O(B Services $B$N%%"%%I%%_%%sI=$KDI2C$5$l$^$7$?!#(B
OPER_ADMIN_TOO_MANY
	Services $B$N%%"%%I%%_%%sI=$OK~GU$G$9$N$GDI2C=PMh$^$;$s!#(B
OPER_ADMIN_REMOVED
	%s $B$O(B Services $B$N%%"%%I%%_%%sI=$+$i>C5n$5$l$^$7$?!#(B
OPER_ADMIN_NOT_FOUND
	%s $B$O(B Services $B$N%%"%%I%%_%%sI=$GB8:_$7$^$;$s!#(B
OPER_ADMIN_LIST_HEADER
	Services $B$N%%"%%I%%_%%sI=!'(B

# OPER$B$NH?1~(B
OPER_OPER_SYNTAX
	OPER {ADD | DEL | LIST} [nick]
OPER_OPER_ADD_SYNTAX
	OPER ADD nick
OPER_OPER_DEL_SYNTAX
	OPER ADD nick
OPER_OPER_EXISTS
	%s $B$O4{$K(B Services $B$N%%*%%Z%%l!<%%?!<I=$GB8:_$7$F$$$^$9!#(B
OPER_OPER_ADDED
	%s $B$O(B Services $B$N%%*%%Z%%l!<%%?!<I=$KDI2C$5$l$^$7$?!#(B
OPER_OPER_TOO_MANY
	Services $B$N%%*%%Z%%l!<%%?!<I=$OK~GU$G$9$N$GDI2C=PMh$^$;$s!#(B
OPER_OPER_REMOVED
	%s $B$O(B Services $B$N%%*%%Z%%l!<%%?!<I=$+$i>C5n$5$l$^$7$?!#(B
OPER_OPER_NOT_FOUND
	%s $B$O(B Services $B$N%%*%%Z%%l!<%%?!<I=$GB8:_$7$^$;$s!#(B
OPER_OPER_LIST_HEADER
	Services $B$N%%*%%Z%%l!<%%?!<I=!'(B

# AKILL$B$NH?1~(B
OPER_AKILL_SYNTAX
	AKILL {ADD | DEL | LIST | VIEW} [[+expiry] mask [reason]]
OPER_AKILL_ADD_SYNTAX
	AKILL ADD [+expiry] mask reason
OPER_AKILL_DEL_SYNTAX
	AKILL DEL mask
OPER_AKILL_NO_NICK
	$BCm0U!'(B$B<+F0@ZCG%%^%%9%%/$O%%f!<%%6!<L>$H%%[%%9%%HL>$r$@$14^$_$^$9$N$G!"%%K%%C%%/$rF~NO$7$J$+$C$?$h$&$K3NG'$7$F2<$5$$!#(B
OPER_TOO_MANY_AKILLS
	$B<+F0@ZCGI=$OK~GU$G$9$N$GDI2C$G$-$^$;$s!#(B
OPER_AKILL_REMOVED
	%s $B$O<+F0@ZCGI=$+$i>C5n$5$l$^$7$?!#(B
OPER_AKILL_NOT_FOUND
	%s $B$O<+F0@ZCGI=$GB8:_$7$^$;$s!#(B
OPER_AKILL_LIST_HEADER
	$B<+F0@ZCGI=!'(B
# mask, reason
OPER_AKILL_LIST_FORMAT
	%-32s  %s
# mask, set-by, set-time, expires, reason
OPER_AKILL_VIEW_FORMAT
	%s (%s $B$K(B %s $B$KDI2C$5$l!"(B%s)
	    %s
OPER_AKILL_NO_EXPIRE
	$B@Z$l$J$$(B
OPER_AKILL_EXPIRES_SOON
	$B<!$N%%G!<%%?J]B8$H$-$K@Z$l$k(B
OPER_AKILL_EXPIRES_M
	%d$BJ,8e@Z$l$k(B
OPER_AKILL_EXPIRES_1M
	%d$BJ,8e@Z$l$k(B
OPER_AKILL_EXPIRES_HM
	%d$B;~(B%d$BJ,8e@Z$l$k(B
OPER_AKILL_EXPIRES_H1M
	%d$B;~(B%d$BJ,8e@Z$l$k(B
OPER_AKILL_EXPIRES_1HM
	%d$B;~(B%d$BJ,8e@Z$l$k(B
OPER_AKILL_EXPIRES_1H1M
	%d$B;~(B%d$BJ,8e@Z$l$k(B
OPER_AKILL_EXPIRES_D
	%d$BF|8e@Z$l$k(B
OPER_AKILL_EXPIRES_1D
	%d$BF|8e@Z$l$k(B

# SET$B$NH?1~(B
OPER_SET_SYNTAX
	SET option setting
OPER_SET_IGNORE_ON
	$B%%$%%0%%N%%"%%3!<%%I$O(B$B:nF0$7$^$9(B$B!#(B
OPER_SET_IGNORE_OFF
	$B%%$%%0%%N%%"%%3!<%%I$O(B$B:nF0$7$^$;$s(B$B!#(B
OPER_SET_IGNORE_ERROR
	IGNORE $B$N%%Q%%i%%a!<%%?!<$O(B ON $B$+(B OFF $B$K$7$F2<$5$$!#(B
OPER_SET_DEBUG_ON
	Services $B$O$3$l$+$i%%G%%P%%C%%0%%b!<%%I$GF0$-$^$9!#(B
OPER_SET_DEBUG_OFF
	Services $B$O$3$l$+$iDL>o%%b!<%%I$GF0$-$^$9!#(B
OPER_SET_DEBUG_LEVEL
	Services $B$O$3$l$+$i%%G%%P!<%%0%%b!<%%I$GF0$-$^$9!#(B
OPER_SET_DEBUG_ERROR
	DEBUG $B$N%%Q%%i%%a!<%%?!<$O!"(BON $B$+(B OFF $B$+@5?t$K$7$F2<$5$$!#(B
OPER_SET_UNKNOWN_OPTION
	%s $B$H$$$&%%*%%W%%7%%g%%s$OJ,$+$j$^$;$s!#(B

# JUPE$B$NH?1~(B
OPER_JUPE_SYNTAX
	JUPE servername [reason]

# RAW$B$NH?1~(B
OPER_RAW_SYNTAX
	RAW text

# RESTART$B$NH?1~(B
OPER_CANNOT_RESTART
	SERVICES_BIN $B$O@_Dj$5$l$F$$$^$;$s$N$G:F5/F0$G$-$^$;$s!#(B
	RESTART $B$N%%3%%^%%s%%I$r;HMQ2DG=$K$9$k$N$K$O!"$b$&0l2s(B configure
	$B$r<B9T$7!"(BServices $B$r:F%%3%%s%%Q%%$%%k$7$F2<$5$$!#(B

# IGNORE$B$NH?1~(B
OPER_IGNORE_LIST
	Services$B$N%%$%%0%%N%%"I=!'(B
OPER_IGNORE_LIST_EMPTY
	Services$B$N%%$%%0%%N%%"I=$O6u$G$9!#(B

###########################################################################
#
# $B0FFb%%7%%9%%F%%`$N%%a%%C%%;!<%%8(B
#
###########################################################################

NEWS_LOGON_TEXT
	$B!J%%m%%0%%*%%s0FFb(B  %s$B!K(B %s
NEWS_OPER_TEXT
	$B!J%%*%%Z%%l!<%%?!<0FFb(B  %s$B!K(B %s

NEWS_LOGON_SYNTAX
	Syntax: LOGONNEWS {ADD|DEL|LIST} [text|num]
NEWS_LOGON_ADD_SYNTAX
	Syntax: LOGONNEWS ADD text
NEWS_LOGON_DEL_SYNTAX
	Syntax: LOGONNEWS DEL {num | ALL}
NEWS_LOGON_LIST_HEADER
	$B%%m%%0%%*%%s0FFbI=!'(B
NEWS_LOGON_LIST_ENTRY
	%5d (%s  %s)
	    %s
NEWS_LOGON_LIST_NONE
	$B%%m%%0%%*%%s0FFb7o$O$"$j$^$;$s!#(B
NEWS_LOGON_ADD_FULL
	$B%%m%%0%%*%%s0FFbI=$OK~GU$G$9!#(B
NEWS_LOGON_ADDED
	$B?7%%m%%0%%*%%s0FFb7o!J(B%d$BHV!K$rDI2C$7$^$7$?!#(B
NEWS_LOGON_DEL_NOT_FOUND
	%d $BHVL\$N%%m%%0%%*%%s0FFb7o$OB8:_$7$^$;$s!#(B
NEWS_LOGON_DELETED
	%d $BHVL\$N%%m%%0%%*%%s0FFb7o$O>C5n$5$l$^$7$?!#(B
NEWS_LOGON_DEL_NONE
	$B%%m%%0%%*%%s0FFb7o$O$"$j$^$;$s!#(B
NEWS_LOGON_DELETED_ALL
	$B%%m%%0%%*%%s0FFb7o$OA4$F>C5n$5$l$^$7$?!#(B

NEWS_OPER_SYNTAX
	Syntax: OPERNEWS {ADD|DEL|LIST} [text|num]
NEWS_OPER_ADD_SYNTAX
	Syntax: OPERNEWS ADD text
NEWS_OPER_DEL_SYNTAX
	Syntax: OPERNEWS DEL {num | ALL}
NEWS_OPER_LIST_HEADER
	$B%%*%%Z%%l!<%%?!<0FFbI=!'(B
NEWS_OPER_LIST_ENTRY
	%5d (%s  %s)
	    %s
NEWS_OPER_LIST_NONE
	$B%%*%%Z%%l!<%%?!<0FFb7o$O$"$j$^$;$s!#(B
NEWS_OPER_ADD_FULL
	$B%%*%%Z%%l!<%%?!<0FFbI=$OK~GU$G$9!#(B
NEWS_OPER_ADDED
	$B?7%%*%%Z%%l!<%%?!<0FFb7o!J(B%d$BHV!K$rDI2C$7$^$7$?!#(B
NEWS_OPER_DEL_NOT_FOUND
	%d $BHVL\$N%%*%%Z%%l!<%%?!<0FFb7o$OB8:_$7$^$;$s!#(B
NEWS_OPER_DELETED
	%d $BHVL\$N%%*%%Z%%l!<%%?!<0FFb7o$O>C5n$5$l$^$7$?!#(B
NEWS_OPER_DEL_NONE
	$B%%*%%Z%%l!<%%?!<0FFb7o$O$"$j$^$;$s!#(B
NEWS_OPER_DELETED_ALL
	$B%%*%%Z%%l!<%%?!<0FFb7o$OA4$F>C5n$5$l$^$7$?!#(B

NEWS_HELP_LOGON
	$B%%7%%s%%?%%/%%9!'(BLOGONNEWS ADD text
	$B!!!!!!!!!!!!(BLOGONNEWS DEL {num | ALL}
	$B!!!!!!!!!!!!(BLOGONNEWS LIST
	
	$B%%m%%0%%*%%s0FFbI=$NJT=8$HI=<($N%%3%%^%%s%%I$G$9!#%%f!<%%6!<$O%%M%%C%%H(B
	$B%%o!<%%/$K@\B3$H!"(BServices $B$O$3$N%%a%%C%%;!<%%8$r%%f!<%%6!<$KAw$j$^(B
	$B$9!#$?$@$7!"%%a%%C%%;!<%%8$,#4$D0J>e$N>l9g!"%%f!<%%6!<$KBgNL$N%%a%%C(B
	$B%%;!<%%8$rAw$i$J$$$h$&$K:G?7$N#3$D$@$1$rAw$j$^$9!#(B
	
	LOGONNEWS LIST $B$H$$$&7A$G!"(BIRC $B%%*%%Z%%l!<%%?!<$O%%m%%0%%*%%s0FFbI=(B
	$B$rI=$9$3$H$,=PMh$^$9$,!"(BADD$B!&(BDEL $B$NJT=8%%3%%^%%s%%I$r;H$&$N$O(B
	Services $B$N%%"%%I%%_%%s$N$_$G$9!#(B

NEWS_HELP_OPER
	$B%%7%%s%%?%%/%%9!'(BOPERNEWS ADD text
	$B!!!!!!!!!!!!(BOPERNEWS DEL {num | ALL}
	$B!!!!!!!!!!!!(BOPERNEWS LIST
	
	$B%%*%%Z%%l!<%%?!<0FFbI=$NJT=8$HI=<($N%%3%%^%%s%%I$G$9!#%%f!<%%6!<$,(B
	IRC $B%%*%%Z%%l!<%%?!<$K$J$k$H!"(BServices $B$O$3$N%%a%%C%%;!<%%8$r%%f!<(B
	$B%%6!<$KAw$j$^$9!#$?$@$7!"%%a%%C%%;!<%%8$,#4$D0J>e$N>l9g!"%%f!<(B
	$B%%6!<$KBgNL$N%%a%%C%%;!<%%8$rAw$i$J$$$h$&$K:G?7$N#3$D$@$1$rAw$j(B
	$B$^$9!#(B
	
	OPERNEWS LIST $B$H$$$&7A$G!"(BIRC $B%%*%%Z%%l!<%%?!<$O%%m%%0%%*%%s0FFbI=(B
	$B$rI=$9$3$H$,=PMh$^$9$,!"(BADD$B!&(BDEL $B$NJT=8%%3%%^%%s%%I$r;H$&$N$O(B
	Services $B$N%%"%%I%%_%%s$N$_$G$9!#(B

###########################################################################
#
# NickServ$B$N%%X%%k%%W%%a%%C%%;!<%%8(B
#
###########################################################################

NICK_HELP
	%S $B$O!"%%K%%C%%/$NEPO?%%5!<%%S%%9$G$9!#$3$N%%5!<%%S%%9$r;H$$(B
	$B%%K%%C%%/$rEPO?$9$l$P!"B>$N%%f!<%%6!<$O$=$N%%K%%C%%/$r;H$($J$/$J$j(B
	$B$^$9!#$?$@$7!"%%K%%C%%/$r(B%d$BF|4V;H$o$J$$$HEPO?$O@Z$l$F$7$^$$$^(B
	$B$9!#<!$N%%3%%^%%s%%I$G%%K%%C%%/$rEPO?$d%%*%%W%%7%%g%%s$NJQ99$,=PMh$^$9!#(B
	$B%%3%%^%%s%%I$r;H$&>l9g$O!V(B/msg %S command$B!W$rF~NO$7$F2<(B
	$B$5$$!#%%3%%^%%s%%I$N%%X%%k%%W$rFI$`>l9g$O!"!V(B/msg %S HELP
	command$B!W$rF~NO$7$F2<$5$$!#(B
	
	    REGISTER   $B%%K%%C%%/$NEPO?(B
#@@@ $B!V(Bidentify$B!W(B
	    IDENTIFY   $B%%Q%%9%%o!<%%I$G(Bidentify$B$9$k(B
	    ACCESS     $B%%"%%/%%;%%9I=$NJQ99(B
	    LINK       $BB>$N%%K%%C%%/$K%%j%%s%%/$r:n$k(B
	    SET        $B%%*%%W%%7%%g%%s$N@_Dj(B
	    DROP       $BEPO?$N>C5n(B
	    RECOVER    $B%%K%%C%%/$r;HMQ$7$F$$$k%%f!<%%6!<$r@ZCG$9$k(B
	    RELEASE    RECOVER $B$r;H$C$F$+$i%%K%%C%%/$r;HMQ2DG=$K$9$k(B
	
	$BB>$N%%3%%^%%s%%I!'(BUNLINK$B!&(BGHOST$B!&(BINFO$B!&(BLIST$B!&(BSTATUS$B!&(BAUTOJOIN
	
	$B7Y9p!'(B$B$3$N%%5!<%%S%%9$O!"%%f!<%%6!<$N?H85$r<i$k0Y$N%%5!<%%S%%9$G$9!#(B
	$B%%K%%C%%/$N!VEp$_!WEy$N0-MQ$N$?$a$N%%5!<%%S%%9$G$O$"$j$^$;$s$N$G!"(B
	$B$=$N$h$&$J9T0Y$O$*$d$a2<$5$$!#(B

NICK_HELP_EXPIRE_ZERO
	%S $B$O!"%%K%%C%%/$NEPO?%%5!<%%S%%9$G$9!#$3$N%%5!<%%S%%9$r;H$$(B
	$B%%K%%C%%/$rEPO?$9$l$P!"B>$N%%f!<%%6!<$O$=$N%%K%%C%%/$r;H$($J$/$J$j(B
	$B$^$9!#<!$N%%3%%^%%s%%I$G%%K%%C%%/$rEPO?$d%%*%%W%%7%%g%%s$NJQ99$,=PMh$^(B
	$B$9!#%%3%%^%%s%%I$r;H$&>l9g$O!V(B/msg %S command$B!W$rF~NO$7$F(B
	$B2<$5$$!#%%3%%^%%s%%I$N%%X%%k%%W$rFI$`>l9g$O!"!V(B/msg %S HELP
	command$B!W$rF~NO$7$F2<$5$$!#(B
	
	    REGISTER   $B%%K%%C%%/$NEPO?(B
#@@@ $B!V(Bidentify$B!W(B
	    IDENTIFY   $B%%Q%%9%%o!<%%I$G(Bidentify$B$9$k(B
	    ACCESS     $B%%"%%/%%;%%9I=$NJQ99(B
	    LINK       $BB>$N%%K%%C%%/$K%%j%%s%%/$r:n$k(B
	    SET        $B%%*%%W%%7%%g%%s$N@_Dj(B
	    DROP       $BEPO?$N>C5n(B
	    RECOVER    $B%%K%%C%%/$r;HMQ$7$F$$$k%%f!<%%6!<$r@ZCG$9$k(B
	    RELEASE    RECOVER $B$r;H$C$F$+$i%%K%%C%%/$r;HMQ2DG=$K$9$k(B
	
	$BB>$N%%3%%^%%s%%I!'(BUNLINK$B!&(BGHOST$B!&(BINFO$B!&(BLIST$B!&(BSTATUS
	
	$B7Y9p!'(B$B$3$N%%5!<%%S%%9$O!"%%f!<%%6!<$N?H85$r<i$k0Y$N%%5!<%%S%%9$G$9!#(B
	$B%%K%%C%%/$N!VEp$_!WEy$N0-MQ$N$?$a$N%%5!<%%S%%9$G$O$"$j$^$;$s$N$G!"(B
	$B$=$N$h$&$J9T0Y$O$*$d$a2<$5$$!#(B

NICK_HELP_REGISTER
	$B%%7%%s%%?%%/%%9!'(BREGISTER password
	
	$B8=:_;HMQ$7$F$$$k%%K%%C%%/$r(B%S$B$N%%G!<%%?%%Y!<%%9$KEPO?$7$^$9!#(B
	$B%%K%%C%%/$rEPO?$7$F$+$i!"(BSET$B!&(BACCESS$BEy$N%%3%%^%%s%%I$G%%K%%C%%/$N%%*(B
	$B%%W%%7%%g%%s$NJQ99$,=PMh$^$9!#$?$@$7!"@_Dj$rJQ99$9$k$N$K$O%%Q%%9(B
	$B%%o!<%%I$,I,MW$G$9$N$G!"EPO?$9$k$H$-$KA*$s$@%%Q%%9%%o!<%%I$r$h$/(B
	$B3P$($F$*$$$F2<$5$$!#$^$?!"%%K%%C%%/$d%%A%%c%%s%%M%%k$H0c$C$F!"%%Q%%9(B
	$B%%o!<%%I$GBgJ8;z$H>.J8;z$O0c$$$^$9!#Nc$($P!"(BFIDO$B!&(BFido$B!&(Bfido
	$BEy$OA4$F0c$$$^$9!#(B
	
	$B%%Q%%9%%o!<%%I$rA*$V$H$-$O!"ITL@$JJ}$N%%Q%%9%%o!<%%I$K$7$F2<$5$$!#(B
	$B$*L>A0!&%%K%%C%%/Ey$N$h$&$JEv$F$d$9$$%%Q%%9%%o!<%%I$O$*;_$a2<$5$$!#(B
	$B#4J8;z0J2<$N%%Q%%9%%o!<%%I$O2?$G$bEv$F$d$9$$J}$G$9$N$G!"#5J8;z(B
	$B>e$N%%Q%%9%%o!<%%I0J>e$K$7$F2<$5$$!#$J$*!"%%Q%%9%%o!<%%I$G%%9%%Z!<%%9(B
	$B$r;H$($^$;$s!#0lJ}!"F|K\8l!J2>L>$H4A;z!K$G%%Q%%9%%o!<%%I$rF~NO(B
	$B$9$k$3$H$b=PMh$^$9!#(B

NICK_HELP_IDENTIFY
	$B%%7%%s%%?%%/%%9!'(BIDENTIFY password
	
#@@@  $B!V>Z5r!W$J$I!&(BChanServ$B$N$b$"$k(B
	%S$B$K%%K%%C%%/$NJ]M-<T$G$"$k>Z5r$r<($7$^$9!#$3$N%%3%%^%%s%%I(B
	$B$r;H$o$J$1$l$P!"$[$H$s$I$N(BServices$B$N%%3%%^%%s%%I$O;H$($^$;$s!#(B
	password$B$O(BREGISTER$B$N%%3%%^%%s%%I$rF~NO$7$?$H$-$N$HF1$8%%Q%%9%%o!<(B
	$B%%I$G$9!#(B

NICK_HELP_DROP
	$B%%7%%s%%?%%/%%9!'(BDROP
	
	%S$B$N%%G!<%%?%%Y!<%%9$+$i%%K%%C%%/$r>C5n$7$^$9!#>C5n$7$?%%K%%C(B
	$B%%/$O!"C/$G$b:FEPO?$,=PMh$^$9!#(B
	
	$B$3$N%%3%%^%%s%%I$r;H$&$N$K$O!"%%Q%%9%%o!<%%I$rF~NO$9$kI,MW$,$"$j$^(B
	$B$9!#%%Q%%9%%o!<%%I$NF~NO$N%%X%%k%%W$r8+$k$N$K$O!"(B
	$B!V(B/msg %S HELP IDENTIFY$B!W$rF~NO$7$F2<$5$$!#(B

NICK_HELP_ACCESS
	$B%%7%%s%%?%%/%%9!'(BACCESS ADD mask
	$B!!!!!!!!!!!!(BACCESS DEL mask
	$B!!!!!!!!!!!!(BACCESS LIST
	
	$B%%K%%C%%/$N%%"%%/%%;%%9I=$NJQ99$HI=<($N%%3%%^%%s%%I$G$9!#%%"%%/%%;%%9I=$H(B
	$B$O!"J]M-<T$N%%f!<%%6!<%%"%%I%%l%%9!J(Buser@host$B$H$$$&%%"%%I%%l%%9!K$N%%j(B
	$B%%9%%H$G$9!#%%f!<%%6!<$N%%"%%I%%l%%9$O%%"%%/%%;%%9I=$N%%^%%9%%/$N0l$D$K9g(B
	$B$($P!"(B%S$B$K@ZCG$5$l$^$;$s!#$^$?!"(BSECURE$B$H$$$&%%*%%W%%7%%g%%s$r(BOFF
	$B$K$9$l$P%%Q%%9%%o!<%%I$rF~NO$;$:$K(BServices$B$N%%3%%^%%s%%I$r;H$($^$9!#(B
	
	$BNc!'(B
	    ACCESS ADD achurch@*.dragonfire.net
	        dragonfire.net$B$H$$$&%%I%%a!<%%s$N%%[%%9%%H$+$i(Bachurch$B$H$$(B
	        $B$&%%f!<%%6!<$r%%"%%/%%;%%9I=$KDI2C$7$^$9!#(B
	
	    ACCESS DEL achurch@*.dragonfire.net
	        $BA0$NNc$H5U$N7k2L$G$9!#(B
	
	    ACCESS LIST
	        $B%%"%%/%%;%%9I=$rI=$7$^$9!#(B

NICK_HELP_LINK
	$B%%7%%s%%?%%/%%9!'(BLINK nick password
	
	nick$B$H$$$&%%K%%C%%/$K%%j%%s%%/$r:n$j!"8=:_$N%%K%%C%%/$rF~NO$7$?%%K%%C(B
	$B%%/$N2>L>$K$7$^$9!#$3$N%%3%%^%%s%%I$r;H$&$H!"8=:_$N%%K%%C%%/$N%%"%%/(B
	$B%%;%%9I=$O>C$(!"%%a%%b$OF~NO$7$?%%K%%C%%/$N%%a%%bI=$KDI2C$5$l$^$9(B
	$B!J$=$l$GF~NO$7$?%%K%%C%%/$N%%a%%bI=$,%%a%%b$N?t$N8B$j$r2a$.$k$3$H(B
	$B$b$"$j$^$9$N$G$4Cm0U2<$5$$!K!#(B
	
	$B%%j%%s%%/$,:n$i$l$F$+$i!"8=:_$N%%K%%C%%/$O(BServices$B$N%%G!<%%?%%Y!<%%9(B
	$B$GF~NO$7$?%%K%%C%%/$KJQ2=$7$^$9!#(B%S$B$N(BUNLINK$B$H(BDROP$B$N%%3%%^(B
	$B%%s%%I0J30!"A4$F$N(BServices$B$N%%3%%^%%s%%I$O8=:_$N%%K%%C%%/$NBe$o$j$K(B
	$B%%j%%s%%/$5$l$?%%K%%C%%/$r;H$$$^$9!#Nc$($P!"%%j%%s%%/$5$l$?%%K%%C%%/$N(B
	$B%%a%%b$r8+$?$$>l9g$O!"$I$A$i$N%%K%%C%%/$G$b8+$i$l$^$9!#$=$7$F!"(B
	$BB>$N%%f!<%%6!<$+$i%%a%%b$r<u$1$k>l9g$O!"$=$N%%f!<%%6!<$O$I$A$i$N(B
	$B%%K%%C%%/$KAw$C$F$bF1$8%%a%%bI=$KDI2C$5$l$^$9!#$^$?!"%%A%%c%%s%%M%%k(B
	$B$N%%"%%/%%;%%9I=$G$I$A$i$+$N%%K%%C%%/$,B8:_$7$F$$$l$P!"$I$A$i$N(B
	$B%%K%%C%%/$G$bF1$8FC8"$rF@$^$9!#(B
	
	$B$3$N%%3%%^%%s%%I$r;H$&$N$K$O!"%%Q%%9%%o!<%%I$rF~NO$9$kI,MW$,$"$j$^(B
	$B$9!#$J$*!"%%j%%s%%/$7$?$$%%K%%C%%/$N%%Q%%9%%o!<%%I$r$3$N%%3%%^%%s%%I$N%%Q(B
	$B%%i%%a!<%%?!<$H$7$FF~NO$9$kI,MW$,$"$j$^$9!#(B

NICK_HELP_UNLINK
	$B%%7%%s%%?%%/%%9!'(BUNLINK [nick password]
	
	$B%%K%%C%%/$N%%j%%s%%/$r>C5n$7$^$9!#$3$N%%3%%^%%s%%I$r;H$&$H!"%%j%%s%%/$5(B
	$B$l$F$$$?%%K%%C%%/$N%%"%%/%%;%%9I=$H%%*%%W%%7%%g%%s$N@_Dj$O8=:_$N%%K%%C%%/(B
	$B$K%%3%%T!<$5$l$^$9$,!"%%a%%b$O%%3%%T!<$5$l$^$;$s!#(B
	
	$B$3$N%%3%%^%%s%%I$r;H$&$N$K$O!"%%Q%%9%%o!<%%I$rF~NO$9$kI,MW$,$"$j$^(B
	$B$9!#%%Q%%9%%o!<%%I$NF~NO$N%%X%%k%%W$r8+$k$N$K$O!"!V(B/msg %S
	HELP IDENTIFY$B!W$rF~NO$7$F2<$5$$!#(B
	
	$BJL$N%%K%%C%%/$N%%j%%s%%/$r>C5n$7$?$$;~$O!"$=$N%%K%%C%%/$H%%Q%%9%%o!<%%I(B
	$B$rF~NO$9$l$P>C5n$G$-$^$9!#(B

NICK_HELP_SET
	$B%%7%%s%%?%%/%%9!'(BSET option parameters
	
	$B%%K%%C%%/$N%%*%%W%%7%%g%%s$r@_Dj$7$^$9!#(Boption$B$NA*Br$O<!$NDL$j$G$9!#(B
	
	    PASSWORD   $B%%Q%%9%%o!<%%I$NJQ99(B
	    LANGUAGE   Services$B$,;H$&8@8l$N@_Dj(B
	    URL        $B%%[!<%%`%%Z!<%%8$N%%"%%I%%l%%9$NF~NO(B
	    EMAIL      $BEE;R%%a!<%%k$N%%"%%I%%l%%9$NF~NO(B
#@@@  $B@ZCG!V%%*%%W%%7%%g%%s!W!"$=$l$H!V<i!W(B...$B!)(B
	    KILL       $B@ZCG%%*%%W%%7%%g%%s$NF~@Z(B
	    SECURE     $B%%;%%-%%e%%"%%*%%W%%7%%g%%s$NF~@Z(B
	    PRIVATE    LIST$B$N%%3%%^%%s%%I$+$i%%K%%C%%/$r1#$9(B
	    HIDE       INFO$B$N%%3%%^%%s%%I$+$i%%K%%C%%/$N>pJs$r1#$9(B
	
	$B$3$N%%3%%^%%s%%I$r;H$&$N$K$O!"%%Q%%9%%o!<%%I$rF~NO$9$kI,MW$,$"$j$^(B
	$B$9!#%%Q%%9%%o!<%%I$NF~NO$N%%X%%k%%W$r8+$k$N$K$O!"!V(B/msg %S
	HELP IDENTIFY$B!W$rF~NO$7$F2<$5$$!#(B
	
	$B%%*%%W%%7%%g%%s$N>pJs$rFI$`$N$K$O!"!V(B/msg %S HELP SET option$B!W$r(B
	$BF~NO$7$F2<$5$$!#(B

NICK_HELP_SET_PASSWORD
	$B%%7%%s%%?%%/%%9!'(BSET PASSWORD new-password
	
	$B%%K%%C%%/$NJ]M-<T$N%%Q%%9%%o!<%%I$rJQ99$7$^$9!#(B

NICK_HELP_SET_LANGUAGE
	$B%%7%%s%%?%%/%%9!'(BSET LANGUAGE number
	
	Services$B$,;H$&8@8l$r@_Dj$7$^$9!#(Bnumber$B$NA*Br$O<!$NDL$j$G$9!#(B
	

NICK_HELP_SET_URL
	$B%%7%%s%%?%%/%%9!'(BSET URL url
	
	url$B$H$$$&%%[!<%%`%%Z!<%%8$N%%"%%I%%l%%9$r%%K%%C%%/$N>pJs$KDI2C$7$^$9!#(B
	$B$3$N%%"%%I%%l%%9$O(BINFO$B$N%%3%%^%%s%%I$N>pJs$GI=$l$^$9!#(B

NICK_HELP_SET_EMAIL
	$B%%7%%s%%?%%/%%9!'(BSET EMAIL address
	
	address$B$H$$$&EE;R%%a!<%%k$N%%"%%I%%l%%9$r%%K%%C%%/$N>pJs$KDI2C$7$^$9!#(B
	$B$3$N%%"%%I%%l%%9$O(BINFO$B$N%%3%%^%%s%%I$N>pJs$GI=$l$^$9!#(B

NICK_HELP_SET_SECURE
	$B%%7%%s%%?%%/%%9!'(BSET SECURE {ON | OFF}
	
	$B%%;%%-%%e%%"%%*%%W%%7%%g%%s$r@_Dj$7$^$9!#(BON$B$K$9$k$H!"%%f!<%%6!<%%"%%I%%l(B
	$B%%9$,%%"%%/%%;%%9I=$KB8:_$7$F$$$F$b%%Q%%9%%o!<%%I$rF~NO$7$J$1$l$PJ](B
	$BM-<T$NFC8"$rF@$^$;$s!#$?$@$7!"%%f!<%%6!<%%"%%I%%l%%9$,%%"%%/%%;%%9I=(B
	$B$K$"$l$P!"@ZCG%%*%%W%%7%%g%%s$,%%*%%s$K@_Dj$5$l$F$b@ZCG$5$l$^$;$s!#(B

NICK_HELP_SET_PRIVATE
	$B%%7%%s%%?%%/%%9!'(BSET PRIVATE {ON | OFF}
	
	$B!V;dE*!W$H$$$&%%*%%W%%7%%g%%s$r@_Dj$7$^$9!#(BON$B$K$9$k$H!"%%K%%C%%/$O(B
	$B!V(B/msg %S LIST$B!W$N%%j%%9%%H$GI=$l$^$;$s!#$?$@$7!"(BON$B$K$7(B
	$B$F$b%%K%%C%%/$rCN$C$F$$$k%%f!<%%6!<$O(BINFO$B$N%%3%%^%%s%%I$G%%K%%C%%/$N>p(B
	$BJs$r8+$k$3$H$,=PMh$^$9!#(B

NICK_HELP_SET_HIDE
	$B%%7%%s%%?%%/%%9!'(BSET HIDE {EMAIL | USERMASK | QUIT} {ON | OFF}
	
	$B%%K%%C%%/$N>pJs$r(B%S$B$N(BINFO$B$N%%3%%^%%s%%I$+$i1#$7$^$9!#1#$;$k(B
	$B$N$O!"EE;R%%a!<%%k$N%%"%%I%%l%%9!J(BEMAIL$B!K!"A0$N%%f!<%%6!<%%"%%I%%l%%9(B
	$B!J(BUSERMASK$B!K!"A0$N@ZCG%%a%%C%%;!<%%8!J(BQUIT$B!K$G$9!#FsHVL\$N%%Q%%i(B
	$B%%a!<%%?!<$O!"(BON$B$K$9$k$H>pJs$r1#$7!"(BOFF$B$K$9$k$H>pJs$rI=$7$^(B
	$B$9!#(B

NICK_HELP_RECOVER
	$B%%7%%s%%?%%/%%9!'(BRECOVER nickname [password]
	
	$BB>$N%%f!<%%6!<$K%%K%%C%%/$,<h$i$l$?$i$=$N%%K%%C%%/$r<h$jLa$7$^$9!#(B
#@@@  $B@ZCG<i$N!V$"$k!W(B
	$B$3$N%%3%%^%%s%%I$r;H$&$H!"@ZCG<i$,@_Dj$7$F$"$k%%K%%C%%/$+$i0lJ,0J(B
	$BFb$G%%K%%C%%/$rJQ$($J$$>l9g$HF1$87k2L$,=P$^$9!#%%K%%C%%/$r;HMQ$7(B
	$B$F$$$k%%f!<%%6!<$r@ZCG$7$F$+$i!"(B%S$B$O$=$N%%K%%C%%/$r<h$j!"(B
	$B0lJ,4VC/$b;H$($J$/$J$j$^$9!#$=$N0lJ,$,$?$C$F$+$i%%K%%C%%/$r<h(B
	$B$jLa$;$^$9!#$=$l$H!"(B%S$B$N(BRELEASE$B$N%%3%%^%%s%%I$r;H$($P0l(B
	$BJ,$,7P$DA0$K<h$jLa$;$^$9!#(B
	
	RECOVER$B$N%%3%%^%%s%%I$r;H$&$N$K$O!"8=:_$N%%f!<%%6!<%%"%%I%%l%%9$,(B
	nickname$B$H$$$&%%K%%C%%/$N%%"%%/%%;%%9I=$K$"$kI,MW$,$"$j$^$9!#$^$?!"(B
	$B%%Q%%9%%o!<%%I$rF~NO$9$l$P%%K%%C%%/$r<h$jLa$;$^$9!#(B

NICK_HELP_RELEASE
	$B%%7%%s%%?%%/%%9!'(BRELEASE nickname [password]
	
#@@@  $B!V%%[%%k%%I!W(B
	$B%%K%%C%%/$N%%[%%k%%I$r2rJ|$7$^$9!#%%K%%C%%/$r;HMQ$7$F$$$?%%f!<%%6!<$,(B
	$B@ZCG$5$l$F$+$i!"%%K%%C%%/$O0lJ,4V;H$($J$/$J$j$^$9!#$=$N0lJ,$,(B
	$B$?$DA0$K%%K%%C%%/$r;HMQ$7$?$$>l9g$O$3$N%%3%%^%%s%%I$r;H$&$N$G$9!#(B
	
	RELEASE$B$N%%3%%^%%s%%I$r;H$&$N$K$O!"8=:_$N%%f!<%%6!<%%"%%I%%l%%9$,(B
	nickname$B$H$$$&%%K%%C%%/$N%%"%%/%%;%%9I=$K$"$kI,MW$,$"$j$^$9!#$^$?!"(B
	$B%%Q%%9%%o!<%%I$rF~NO$9$l$P%%K%%C%%/$N%%[%%k%%I$,2rJ|$5$l$^$9!#(B

NICK_HELP_GHOST
	$B%%7%%s%%?%%/%%9!'(BGHOST nickname [password]
	
	$B!V%%4!<%%9%%H!W$H$$$&@\B3$r@ZCG$7$^$9!#%%4!<%%9%%H$O!"K\Ev$KB8:_(B
	$B$7$F$$$J$$@\B3$N$3$H$G$9!#IaDL$O!"%%4!<%%9%%H$,=P$k$N$O%%Q%%=%%3(B
	$B%%s$,%%U%%l!<%%:$7$?$j%%b%%G%%`$N@\B3$,@ZCG$5$l$?$j$9$k$H$-$G$9!#(B
	$B$b$A$m$s!"%%4!<%%9%%H$@$1$G$J$/IaDL$N%%f!<%%6!<$b$3$N%%3%%^%%s%%I$G(B
	$B@ZCG$G$-$^$9$,!"B?$/$N(BIRC$B%%/%%i%%$%%"%%s%%H$O@ZCG$5$l$?$i$9$0:F@\(B
	$BB3$7$^$9$N$G!"$3$N%%3%%^%%s%%I$r;H$C$F$b%%K%%C%%/$r<h$jLa$;$J$$$3(B
	$B$H$b$"$j$^$9!#$=$N>l9g$O!"(BRECOVER$B$N%%3%%^%%s%%I$r;H$C$F2<$5$$!#(B
	
	GHOST$B$N%%3%%^%%s%%I$r;H$&$N$K$O!"8=:_$N%%f!<%%6!<%%"%%I%%l%%9$,(B
	nickname$B$H$$$&%%K%%C%%/$N%%"%%/%%;%%9I=$K$"$kI,MW$,$"$j$^$9!#$^$?!"(B
	$B%%Q%%9%%o!<%%I$rF~NO$9$l$P%%4!<%%9%%H$r@ZCG$G$-$^$9!#(B

NICK_HELP_INFO
	$B%%7%%s%%?%%/%%9!'(BINFO nickname
	
	nickname$B$H$$$&%%K%%C%%/$N>pJs!JJ]M-<T$NL>A0!&%%f!<%%6!<%%"%%I%%l%%9!"(B
	$BA0$N;HMQ$7$?F|;~!"%%*%%W%%7%%g%%s$N@_Dj$J$I!K$rI=$7$^$9!#(B

NICK_HELP_LIST
	$B%%7%%s%%?%%/%%9!'(BLIST pattern
	
	pattern$B$K9g$&%%K%%C%%/$H$=$N%%K%%C%%/$N%%f!<%%6!<%%"%%I%%l%%9$rI=$7$^(B
	$B$9!#$?$@$7!";dE*%%*%%W%%7%%g%%s$,(B ON $B$N%%K%%C%%/$OI=$l$^$;$s!#(B
	pattern$B$r(Bnick!user@host$B$H$$$&7A$GF~NO$7$F2<$5$$!#(B
	
	$BNc!'(B
	    LIST *!joeuser@foo.com
	        joeuser@foo.com$B$,J]M-$7$F$$$k%%K%%C%%/$rI=$7$^$9!#(B
	    LIST *Bot*!*@*
	        $B!V(BBot$B!W$r4^$`%%K%%C%%/$rI=$7$^$9!#(B
	    LIST *!*@*.bar.org
	        bar.org$B$N%%f!<%%6!<$,J]M-$7$F$$$k%%K%%C%%/$rI=$7$^$9!#(B

NICK_HELP_STATUS
	$B%%7%%s%%?%%/%%9!'(BSTATUS nickname...
	
#@@@ $BL>>u!&(BChanServ$B$K$b$"$k(B
	nickname$B$H$$$&%%K%%C%%/$r;HMQ$7$F$$$k%%f!<%%6!<$O$=$N%%K%%C%%/$NJ](B
	$BM-<T$G$"$k$+$=$&$G$J$$$+$H$$$&$3$H$rI=<($7$^$9!#H?1~$O$3$N(B
	$B7A$G$9!'(B
	
	    nickname status-code
	
	nickname$B$O%%3%%^%%s%%I$rF~NO$7$?$H$-$HF1$8%%K%%C%%/$G!"(Bstatus-code
	$B$O<!$N?tCM$N0l$D$G$9!#(B
	
	    0 - $B%%f!<%%6!<$O8=:_@\B3$7$F$$$J$$$+!"%%K%%C%%/$OEPO?$5$l$F(B
	        $B!!!!$$$J$$(B
	    1 - $B%%f!<%%6!<$OJ]M-<T$G$O$J$$$+!"%%;%%-%%e%%"%%*%%W%%7%%g%%s$OF~(B
	        $B!!!!$K@_Dj$5$l$F$$$F%%f!<%%6!<$O%%Q%%9%%o!<%%I$rF~NO$7$F(B
	        $B!!!!$$$J$$(B
	    2 - $B%%f!<%%6!<$N%%"%%I%%l%%9$O%%"%%/%%;%%9I=$K$"$j!"%%;%%-%%e%%"%%*%%W(B
	        $B!!!!%%7%%g%%s$O%%*%%U$K@_Dj$5$l$F$$$k(B
	    3 - $B%%f!<%%6!<$O(BIDENTIFY$B$G%%Q%%9%%o!<%%I$rF~NO$7$?(B
	
	$B%%3%%^%%s%%I$:$D$K%%K%%C%%/$r(B16$B0J2<$rF~NO$G$-$^$9$,!"(B17$B0J>e$N>l9g(B
	$B$H%%K%%C%%/$rF~NO$7$J$$>l9g$K$O%%(%%i!<%%a%%C%%;!<%%8$O=P$^$;$s!#(B

NICK_SERVADMIN_HELP
	
	Services$B$N%%"%%I%%_%%s$O<!$N%%3%%^%%s%%I$b;H$($^$9!#(B
	
	    GETPASS    $B%%K%%C%%/$N%%Q%%9%%o!<%%I$r8+$k(B
	               $B!!!!!J0E9f2=$,;H$o$l$F$$$J$$>l9g$N$_!K(B
	    FORBID     $B%%K%%C%%/$r;HMQIT2DG=$K$9$k(B
	
	$B$J$*!"(BServices$B$N%%"%%I%%_%%s$O%%Q%%9%%o!<%%I$rF~NO$;$:$K%%K%%C%%/$NEP(B
	$BO?$r>C5n$9$k$3$H$b%%"%%/%%;%%9I=$r8+$k$3$H$b=PMh$^$9!#%%"%%/%%;%%9(B
	$BI=$r8+$k$N$K$O!V(B/msg %S ACCESS LIST nick$B!W$H$$$&7A$r(B
	$B;H$&$N$G$9!#(B

NICK_SERVADMIN_HELP_DROP
	$B%%7%%s%%?%%/%%9!'(BDROP [nickname]
	
	$B%%Q%%i%%a!<%%?!<$,$J$$>l9g$O!"8=:_;HMQ$7$F$$$k%%K%%C%%/$r(B%S
	$B$N%%G!<%%?%%Y!<%%9$+$i>C5n$7$^$9!#(B
	
	$B%%Q%%i%%a!<%%?!<$,$"$k>l9g$O!"F~NO$7$?%%K%%C%%/$r%%G!<%%?%%Y!<%%9$+$i(B
	$B>C5n$7$^$9!#$3$N;H$$J}$r;H$($k$N$O(BServices$B$N%%"%%I%%_%%s(B$B$N$_$G(B
	$B$9!#(B

NICK_SERVADMIN_HELP_SET
	
	Services$B$N%%"%%I%%_%%s$O(BNOEXPIRE$B$H$$$&%%*%%W%%7%%g%%s$r@_Dj$G$-$^$9!#(B
	$B$3$N%%*%%W%%7%%g%%s$G%%K%%C%%/$NEPO?$r@Z$l$J$/$9$k$3$H$,=PMh$^$9!#(B
	$B$J$*!"(BServices$B$N%%"%%I%%_%%s$O%%Q%%9%%o!<%%I$rF~NO$;$:$K$I$N%%K%%C%%/(B
	$B$G$b%%*%%W%%7%%g%%s$r@_Dj$G$-$^$9!#B>$N%%K%%C%%/$N%%*%%W%%7%%g%%s$r@_Dj(B
	$B$9$k>l9g$O!V(BSET nickname option parameters$B!W$H$$$&7A$G$9!#(B

NICK_SERVADMIN_HELP_SET_NOEXPIRE
	$B%%7%%s%%?%%/%%9!'(BSET NOEXPIRE [nickname] {ON | OFF}
	
	nickname$B$H$$$&%%K%%C%%/$NEPO?$O@Z$l$k$+$r@_Dj$7$^$9!#(BON$B$K$9$k(B
	$B$H!"EPO?$O@Z$l$^$;$s!#%%K%%C%%/$rF~NO$7$J$$>l9g$O!"<+J,$N%%K%%C(B
	$B%%/$N%%*%%W%%7%%g%%s$r@_Dj$7$^$9!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B

NICK_SERVADMIN_HELP_UNLINK
	
	Services$B$N%%"%%I%%_%%s$O!"%%Q%%9%%o!<%%I$rF~NO$;$:$K$I$N%%K%%C%%/$N%%j%%s(B
	$B%%/$G$b>C5n$G$-$^$9!#(B

NICK_SERVADMIN_HELP_LISTLINKS
	$B%%7%%s%%?%%/%%9!'(BLISTLINKS nickname [ALL]
	
	$BF~NO$7$?%%K%%C%%/$K%%j%%s%%/$7$F$$$k%%K%%C%%/$rI=$7$^$9!#(BALL $B$rF~NO$9(B
	$B$k$H!"BP>]%%K%%C%%/$HF1$88"Mx$r;}$C$F$$$k%%K%%C%%/$rA4$FI=$7!"F~NO(B
	$B$7$J$$>l9g$O!"BP>]%%K%%C%%/$,%%j%%s%%/$ND>@\BP>]$K$J$C$F$$$k%%K%%C%%/(B
	$B$@$1$rI=$7$^$9!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B

NICK_SERVADMIN_HELP_LIST
	$B%%7%%s%%?%%/%%9!'(BLIST pattern [FORBIDDEN] [NOEXPIRE]
	
	pattern$B$K9g$&%%K%%C%%/$H$=$N%%K%%C%%/$N%%f!<%%6!<%%"%%I%%l%%9$rI=$7$^(B
	$B$9!#$?$@$7!";dE*%%*%%W%%7%%g%%s$N(BON$B$N%%K%%C%%/$O(BServices$B$N%%"%%I%%_%%s(B
	$B$K$7$+I=$l$^$;$s!#(Bpattern$B$r(Bnick!user@host$B$H$$$&7A$GF~NO(B
	$B$7$F2<$5$$!#(B
	
	$B$^$?!"(BServices$B$N%%"%%I%%_%%s$K$N$_!"EPO?$N@Z$l$J$$%%K%%C%%/$NF,$K(B
	$B!V(B!$B!W0u$,I=$l$^$9!#(B
	
	FORBIDDEN $B$^$?$O(B NOEXPIRE $B$rDI2C$9$l$P!"%%j%%9%%H$KI=$l$k%%K%%C(B
	$B%%/$r@)8B$9$k$3$H$,=PMh$^$9!#(BFORBIDDEN $B$N>l9g$O;HMQ6X;_%%K%%C(B
	$B%%/!"(BNOEXPIRE $B$N>l9g$OEPO?$N@Z$l$J$$%%K%%C%%/$,I=$l$^$9!#N>J}(B
	$B$rF~NO$9$l$P!"N>J}$N%%?%%$%%W$N%%K%%C%%/$,I=$l$^$9!#$?$@$7!"$3$N(B
	$B%%*%%W%%7%%g%%s$r;H$&$N$O(BServices$B$N%%"%%I%%_%%s(B$B$N$_$G$9!#(B
	
	$BNc!'(B
	    LIST *!joeuser@foo.com
	        joeuser@foo.com$B$,J]M-$7$F$$$k%%K%%C%%/$rI=$7$^$9!#(B
	    LIST *Bot*!*@*
	        $B!V(BBot$B!W$r4^$`%%K%%C%%/$rI=$7$^$9!#(B
	    LIST * NOEXPIRE
	        $BEPO?$N@Z$l$J$$%%K%%C%%/$rA4$FI=$7$^$9!#(B

NICK_SERVADMIN_HELP_FORBID
	$B%%7%%s%%?%%/%%9!'(BFORBID nickname
	
	$B%%K%%C%%/$r;HMQIT2DG=$K$7$^$9!#%%-%%c%%s%%;%%k$K$O(BDROP$B$N%%3%%^%%s%%I$,(B
	$B;H$($^$9!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B

###########################################################################
#
# ChanServ$B$N%%X%%k%%W%%a%%C%%;!<%%8(B
#
###########################################################################

CHAN_HELP
	%S$B$O!"%%A%%c%%s%%M%%k$NEPO?%%5!<%%S%%9$G$9!#$3$N%%5!<%%S%%9$r;H(B
	$B$C$F%%A%%c%%s%%M%%k$rEPO?$9$l$P!"%%A%%c%%s%%M%%k$N%%"%%/%%;%%9$r@)8f=PMh(B
	$B$k$7!"?'!9$J%%*%%W%%7%%g%%s$b@_Dj$G$-$^$9!#%%A%%c%%s%%M%%k$NEPO?$d@)(B
	$B8f$N%%3%%^%%s%%I$O<!$NDL$j$G$9!#%%3%%^%%s%%I$r;H$&>l9g$O!"!V(B/msg
	%S command$B!W$rF~NO$7$F2<$5$$!#$^$?!"%%3%%^%%s%%I$N%%X%%k%%W(B
	$B$rFI$`>l9g$O!V(B/msg %S HELP command$B!W$rF~NO$7$F2<$5$$!#(B
	
	    REGISTER   $B%%A%%c%%s%%M%%k$NEPO?(B
	    IDENTIFY   $B%%A%%c%%s%%M%%k$N%%Q%%9%%o!<%%I$NF~NO(B
	    SET        $B%%A%%c%%s%%M%%k$N%%*%%W%%7%%g%%s$NJQ99(B
	    ACCESS     $BFC8"$N%%f!<%%6!<I=$NJQ99(B
#@@@ $B!VDIJ|!W(B
	    AKICK      $BDIJ|$7$?%%f!<%%6!<I=$NJQ99(B
	    DROP       $B%%A%%c%%s%%M%%k$NEPO?$N>C5n(B
	
	$BB>$N%%3%%^%%s%%I!'(BINFO$B!&(BLIST$B!&(BINVITE$B!&(BOP$B!&(BDEOP$B!&(BUNBAN$B!&(BCLEAR

CHAN_HELP_EXPIRES
	
	$BCm0U!'(B$BEPO?$7$?%%A%%c%%s%%M%%k$O(B%d$BF|4V;HMQ$5$l$J$$$H!"EPO?$O@Z(B
	$B$l$^$9!#(B

CHAN_HELP_REGISTER
	$B%%7%%s%%?%%/%%9!'(BREGISTER channel password description
	
	%S$B$N%%G!<%%?%%Y!<%%9$G%%A%%c%%s%%M%%k$rEPO?$7$^$9!#$3$N%%3%%^%%s(B
	$B%%I$r;H$&$N$O!"%%A%%c%%s%%M%%k$N%%*%%Z%%l!<%%?!<$N$_$G$9!#(Bpassword$B$O(B
	$B%%A%%c%%s%%M%%k$N%%*%%W%%7%%g%%s$rJQ99$9$k0Y$N%%Q%%9%%o!<%%I$G$9!#:G8e$N(B
	$B%%Q%%i%%a!<%%?!<$O%%A%%c%%s%%M%%k$NL>>u$G$9!#(B
	
	$B%%A%%c%%s%%M%%k$rEPO?$9$k$H!"EPO?$7$?%%f!<%%6!<$O$=$N%%A%%c%%s%%M%%k$N(B
	$B!V%%U%%!%%&%%s%%@!<!W$K$J$j$^$9!#%%A%%c%%s%%M%%k$N%%U%%!%%&%%s%%@!<$OA4$F(B
	$B$N%%A%%c%%s%%M%%k$N%%*%%W%%7%%g%%s$rJQ99=PMh$k$7!"%%A%%c%%s%%M%%k$KF~$k$H(B
	%S$B$O<+F0E*$K%%U%%!%%&%%s%%@!<$r(B+o$B%%b!<%%I$K$7$^$9!#$3$NFC8"(B
	$B$N0lIt$rB>$N%%f!<%%6!<$K>e$2$?$$>l9g$O!V(B/msg %S HELP
	ACCESS$B!W$rF~NO$7$F2<$5$$!#(B
	
	$BCm0U!'(B$B%%A%%c%%s%%M%%k$rEPO?$9$k$N$K$O!"%%K%%C%%/$rEPO?$9$kI,MW$,$"(B
	$B$j$^$9!#%%K%%C%%/$rEPO?$7$F$$$J$$%%f!<%%6!<$O!V(B/msg %s HELP$B!W(B
	$B$rF~NO$7$F2<$5$$!#(B

CHAN_HELP_IDENTIFY
	$B%%7%%s%%?%%/%%9!'(BIDENTIFY channel password
	
	%S$B$K%%A%%c%%s%%M%%k$NJ]M-<T$G$"$k>Z5r$r<($7$^$9!#$3$N%%3%%^(B
	$B%%s%%I$r;H$o$J$1$l$P!"(BLEVELS$B!&(BSET FOUNDER$B!&(BSET PASSWORD$B!&(BDROP
	$B$N%%3%%^%%s%%I$,;H$($^$;$s!#$J$*!"%%U%%!%%&%%s%%@!<$N%%K%%C%%/$r;HMQ$7(B
	$B$F$$$J$$$H$-$O!"$3$N%%3%%^%%s%%I$rF~NO$9$l$P%%U%%!%%&%%s%%@!<$NFC8"(B
	$B$rF@$^$9!#(Bpassword$B$O(BREGISTER$B$N%%3%%^%%s%%I$rF~NO$7$?$H$-$N$H(B
	$BF1$8%%Q%%9%%o!<%%I$G$9!#(B

CHAN_HELP_DROP
	$B%%7%%s%%?%%/%%9!'(BDROP channel
	
	channel$B$H$$$&%%A%%c%%s%%M%%k$r(B%S$B$N%%G!<%%?%%Y!<%%9$+$i>C5n$7$^$9!#(B
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(B$B%%U%%!%%&%%s%%@!<(B$B$K8B$C$F$$$F!"%%Q%%9%%o!<%%I(B
	$B$rF~NO$9$kI,MW$b$"$j$^$9!#(B

CHAN_HELP_SET
	$B%%7%%s%%?%%/%%9!'(BSET channel option parameters
	
	$B%%A%%c%%s%%M%%k$N%%*%%W%%7%%g%%s$d>pJs$rJQ99$7$^$9!#%%U%%!%%&%%s%%@!<$H%%Q(B
	$B%%9%%o!<%%I$rJQ99$9$k$N$K$O!"%%A%%c%%s%%M%%k$N%%Q%%9%%o!<%%I$r(BIDENTIFY
	$B$N%%3%%^%%s%%I$GF~NO$9$kI,MW$,$"$j$^$9!#%%*%%W%%7%%g%%s$O<!$NDL$j$G(B
	$B$9!#(B
	
	    FOUNDER     $B%%U%%!%%&%%s%%@!<$NJQ99(B
	    SUCCESSOR   $B<!4|%%U%%!%%&%%s%%@!<$NJQ99(B
	    PASSWORD    $B%%Q%%9%%o!<%%I$NJQ99(B
	    DESC        $B%%A%%c%%s%%M%%k$NL>>u$NJQ99(B
	    URL         $B%%[!<%%`%%Z!<%%8$N%%"%%I%%l%%9$NF~NO(B
	    EMAIL       $BEE;R%%a!<%%k$N%%"%%I%%l%%9$NF~NO(B
	    ENTRYMSG    $B%%A%%c%%s%%M%%k$KF~$k$H$-$K=P$k%%a%%C%%;!<%%8$NF~NO(B
	    TOPIC       $B8=:_$N%%A%%c%%s%%M%%k$N%%H%%T%%C%%/$NJQ99(B
#@@@  $B!V3P$($k!W!&2<$N!V$3$H!W!"!V$H$$$&%%*%%W%%7%%g%%s!W(B
	    KEEPTOPIC   $B%%A%%c%%s%%M%%k$,;HMQ$5$l$F$$$J$$4V$K$b%%H%%T%%C%%/(B
	                $B!!!!$r3P$($k$3$H$NF~@Z(B
	    TOPICLOCK   SET TOPIC$B%%3%%^%%s%%I$G$7$+%%H%%T%%C%%/$rJQ99$5$;(B
	                $B!!!!$J$$$H$$$&%%*%%W%%7%%g%%s$NF~@Z(B
	    MLOCK       $B%%A%%c%%s%%M%%k$N%%b!<%%I$r%%*%%s!&%%*%%U$K%%m%%C%%/$9$k(B
	    PRIVATE     LIST$B%%3%%^%%s%%I$+$i%%A%%c%%s%%M%%k$r1#$9(B
	    RESTRICTED  $BFC8"$r;}$C$F$$$J$$%%f!<%%6!<$r%%A%%c%%s%%M%%k$KF~(B
	                $B!!!!$l$J$$$H$$$&%%*%%W%%7%%g%%s$NF~@Z(B
	    SECURE      $B%%;%%-%%e%%"%%*%%W%%7%%g%%s$NF~@Z(B
	    SECUREOPS   +o$B%%b!<%%I$N@)8f$r87$7$/$9$k(B
	    LEAVEOPS	DEOP$B%%3%%^%%s%%I$G$7$+(B-o$B%%b!<%%I$r3]$1$J$$(B
	    VERBOSE     Send a notice to the channel's operators when
	                    certain commands are used on the channel
	
	$B%%*%%W%%7%%g%%s$N>pJs$rFI$`$N$K$O!"!V(B/msg %S HELP option$B!W(B
	$B$rF~NO$7$F2<$5$$!#(B

CHAN_HELP_SET_FOUNDER
	$B%%7%%s%%?%%/%%9!'(BSET channel FOUNDER nick
	
	$B%%U%%!%%&%%s%%@!<$rJQ99$7$^$9!#$?$@$7!"(Bnick$B$H$$$&%%K%%C%%/$,EPO?(B
	$B$5$l$F$$$J$$$HJQ99=PMh$^$;$s!#(B

CHAN_HELP_SET_SUCCESSOR
	$B%%7%%s%%?%%/%%9!'(BSET channel SUCCESSOR nick
	
	$B<!4|%%U%%!%%&%%s%%@!<$rJQ99$7$^$9!#<!4|%%U%%!%%&%%s%%@!<$H$O!"8=:_$N(B
	$B%%U%%!%%&%%s%%@!<$N%%K%%C%%/$NEPO?$,@Z$l$?$j>C5n$5$l$?$j$9$k$H%%U%%!%%&(B
	$B%%s%%@!<$K$J$k%%f!<%%6!<$G$9!#<!4|%%U%%!%%&%%s%%@!<$,@_Dj$5$l$F$$$J$$(B
	$B>l9g$O!"%%U%%!%%&%%s%%@!<$N%%K%%C%%/$NEPO?$,@Z$l$k$H!"%%A%%c%%s%%M%%k$NEP(B
	$BO?$b>C5n$5$l$F$7$^$$$^$9!#$?$@$7!"%%U%%!%%&%%s%%@!<$N%%K%%C%%/$NEPO?(B
	$B$,@Z$l$k;~E@$G<!4|%%U%%!%%&%%s%%@!<$O%%A%%c%%s%%M%%k$N8B$j!J(B%d$B!K$r1[$((B
	$B$F$$$l$P!"<!4|%%U%%!%%&%%s%%@!<$,@_Dj$5$l$F$$$J$$$h$&$K%%A%%c%%s%%M%%k(B
	$B$NEPO?$O>C$($F$7$^$$$^$9!#(B
	
	$B<!4|%%U%%!%%&%%s%%@!<$K$9$k%%K%%C%%/!J(Bnick$B!K$OEPO?$7$F$$$J$1$l$P$J$j(B
	$B$^$;$s!#(B

CHAN_HELP_SET_PASSWORD
	$B%%7%%s%%?%%/%%9!'(BSET channel PASSWORD password
	
	$B%%U%%!%%&%%s%%@!<$N%%Q%%9%%o!<%%I$rJQ99$7$^$9!#(B

CHAN_HELP_SET_DESC
	$B%%7%%s%%?%%/%%9!'(BSET channel DESC description
	
	$B%%A%%c%%s%%M%%k$NL>>u$rJQ99$7$^$9!#L>>u$O(BLIST$B!&(BINFO$B$N%%3%%^%%s%%I$N(B
	$B>pJs$GI=$l$^$9!#(B

CHAN_HELP_SET_URL
	$B%%7%%s%%?%%/%%9!'(BSET channel URL [url]
	
	url$B$H$$$&%%[!<%%`%%Z!<%%8$N%%"%%I%%l%%9$r%%A%%c%%s%%M%%k$N>pJs$KDI2C$7$^(B
	$B$9!#$3$N%%"%%I%%l%%9$O(BINFO$B$N%%3%%^%%s%%I$N>pJs$GI=$l$^$9!#%%Q%%i%%a!<(B
	$B%%?!<$rF~NO$7$J$$>l9g$O!"%%[!<%%`%%Z!<%%8$N%%"%%I%%l%%9$O>C5n$5$l$^(B
	$B$9!#(B

CHAN_HELP_SET_EMAIL
	$B%%7%%s%%?%%/%%9!'(BSET channel EMAIL [address]
	
	address$B$H$$$&EE;R%%a!<%%k$N%%"%%I%%l%%9$r%%A%%c%%s%%M%%k$N>pJs$KDI2C$7(B
	$B$^$9!#$3$N%%"%%I%%l%%9$O(BINFO$B$N%%3%%^%%s%%I$N>pJs$GI=$l$^$9!#%%Q%%i(B
	$B%%a!<%%?!<$rF~NO$7$J$$>l9g$O!"EE;R%%a!<%%k$N%%"%%I%%l%%9$O>C5n$5$l(B
	$B$^$9!#(B

CHAN_HELP_SET_ENTRYMSG
	$B%%7%%s%%?%%/%%9!'(BSET channel ENTRYMSG [message]
	
	message$B$H$$$&%%F%%-%%9%%H$r%%A%%c%%s%%M%%k$KF~$k%%f!<%%6!<$K=P$7$^$9!#(B
	$B%%Q%%i%%a!<%%?!<$rF~NO$7$J$$>l9g$O!"%%a%%C%%;!<%%8$O>C5n$5$l$^$9!#(B

CHAN_HELP_SET_TOPIC
	$B%%7%%s%%?%%/%%9!'(BSET channel TOPIC topic
	
	%S$B$K%%A%%c%%s%%M%%k$N%%H%%T%%C%%/$rJQ99$5$;$^$9!#$@$$$?$$!"(B
	SET LOCKTOPIC$B$N%%3%%^%%s%%I$H0l=o$K;H$o$l$^$9!#(B

CHAN_HELP_SET_KEEPTOPIC
	$B%%7%%s%%?%%/%%9!'(BSET channel KEEPTOPIC {ON | OFF}
	
	$B!V%%H%%T%%C%%/5-O?!W$H$$$&%%*%%W%%7%%g%%s$r@_Dj$7$^$9!#%%H%%T%%C%%/5-O?(B
	$B$O!"%%A%%c%%s%%M%%k$N8=:_$N%%H%%T%%C%%/$r5-O?$7!"%%A%%c%%s%%M%%k$,>C$($F(B
	$B$b$=$N%%H%%T%%C%%/$r3P$(!"%%A%%c%%s%%M%%k$,$^$?:n$i$l$k$H$-$K%%H%%T%%C(B
	$B%%/$rLa$9$H$$$&%%*%%W%%7%%g%%s$G$9!#(B

CHAN_HELP_SET_TOPICLOCK
	$B%%7%%s%%?%%/%%9!'(BSET channel TOPICLOCK {ON | OFF}
	
	$B!V%%H%%T%%C%%/%%m%%C%%/!W$H$$$&%%*%%W%%7%%g%%s$r@_Dj$7$^$9!#(BON$B$K$9$k$H!"(B
	$B%%H%%T%%C%%/$O(B%S$B$N(BSET TOPIC$B$N%%3%%^%%s%%I$G$7$+JQ$($i$l$J$/$J$j$^(B
	$B$9!#(B

CHAN_HELP_SET_MLOCK
	$B%%7%%s%%?%%/%%9!'(BSET channel MLOCK modes
	
	$B%%b!<%%I%%m%%C%%/$r@_Dj$7$^$9!#(B%S$B$K$h$C$F!"%%A%%c%%s%%M%%k$NF~(B
	$B@Z%%b!<%%I!J(Bi$B!&(Bk$B!&(Bl$B!&(Bm$B!&(Bn$B!&(Bp$B!&(Bs$B!&(Bt$B!K$r%%*%%s$+%%*%%U$K%%m%%C%%/$G$-(B
	$B$^$9!#(Bmodes$B$H$$$&%%Q%%i%%a!<%%?!<$O(BIRC$B$N!V(B/MODE$B!W$H$$$&%%3%%^%%s%%I(B
	$B$HF1$87A$G!"!V(B+$B!W$N8e$N%%b!<%%I$O%%*%%s$K%%m%%C%%/$5$l!"!V(B-$B!W$N8e(B
	$B$N%%b!<%%I$O%%*%%U$K%%m%%C%%/$5$l$^$9!#(B
	
	$BCm0U!'(B$B2<$NFsHVL\$NNc$N$h$&$K%%A%%c%%s%%M%%k%%-!<$r%%m%%C%%/$9$l$P!"(B
	RESTRICTED$B$H$$$&%%*%%W%%7%%g%%s$r%%*%%s$K$7$J$1$l$PC/$K$G$b%%-!<$,(B
	$B8+$($^$9!#(B
	
	$BNc!'(B
	    SET #channel MLOCK +nt-iklps
	       n$B!&(Bt$B%%b!<%%I$r%%*%%s$K%%m%%C%%/$7!"(Bi$B!&(Bk$B!&(Bl$B!&(Bp$B!&(Bs$B%%b!<%%I$r%%*(B
	       $B%%U$K%%m%%C%%/$7$^$9!#(Bm$B%%b!<%%I$O%%m%%C%%/$5$l$^$;$s!#(B
	
	    SET #channel MLOCK +knst-ilmp my-key
	       k$B!&(Bn$B!&(Bs$B!&(Bt$B%%b!<%%I$r%%*%%s$K%%m%%C%%/$7!"(Bi$B!&(Bl$B!&(Bm$B!&(Bp$B%%b!<%%I$r(B
	       $B%%*%%U$K%%m%%C%%/$7$^$9!#%%-!<$r!V(Bmy-key$B!W$K%%m%%C%%/$7$^$9!#(B
	
	    SET #channel MLOCK +
	       $B%%b!<%%I%%m%%C%%/$r2r$-$^$9!#(B

CHAN_HELP_SET_PRIVATE
	$B%%7%%s%%?%%/%%9!'(BSET channel PRIVATE {ON | OFF}
	
	$B!V;dE*!W$H$$$&%%*%%W%%7%%g%%s$r@_Dj$7$^$9!#(BON$B$K$9$k$H!"%%A%%c%%s%%M(B
	$B%%k$O!V(B/msg %S LIST$B!W$N%%j%%9%%H$GI=$l$^$;$s!#(B

CHAN_HELP_SET_RESTRICTED
	$B%%7%%s%%?%%/%%9!'(BSET channel RESTRICTED {ON | OFF}
	
	$B%%"%%/%%;%%9@)8f%%*%%W%%7%%g%%s$r@_Dj$7$^$9!#(BON$B$K$9$k$H!"(B+o$B%%b!<%%I$r(B
	$BF@$i$l$J$$%%f!<%%6!<!JIi?t$N%%l%%Y%%k$N%%f!<%%6!<!"$=$7$F%%;%%-%%e%%"(B
	$B%%*%%W%%7%%g%%s$r(BON$B$K$9$k$H%%"%%/%%;%%9I=$K$J$$%%f!<%%6!<!K$O%%A%%c%%s%%M(B
	$B%%k$+$i$KF~$l$J$/$J$j$^$9!#(B

CHAN_HELP_SET_SECURE
	$B%%7%%s%%?%%/%%9!'(BSET channel SECURE {ON | OFF}
	
	$B%%;%%-%%e%%"%%*%%W%%7%%g%%s$r@_Dj$7$^$9!#(BON$B$K$9$k$H!"%%K%%C%%/$N%%Q%%9(B
	$B%%o!<%%I$rF~NO$7$F$$$J$$%%f!<%%6!<$OFC8"$rF@$^$;$s!#%%"%%/%%;%%9@)(B
	$B8f%%*%%W%%7%%g%%s$H0l=o$K;H$($P!"%%Q%%9%%o!<%%I$rF~NO$7$F$$$J$$%%f!<(B
	$B%%6!<$O%%A%%c%%s%%M%%k$KF~$l$J$/$J$j$^$9!#(B

CHAN_HELP_SET_SECUREOPS
	$B%%7%%s%%?%%/%%9!'(BSET channel SECUREOPS {ON | OFF}
	
	+o$B%%b!<%%I$N@)8f$r87$7$/$9$k$3$H$r@_Dj$7$^$9!#(BON$B$K$9$k$H!"%%"(B
	$B%%/%%;%%9I=$K$J$$%%f!<%%6!<$O(B+o$B%%b!<%%I$rF@$i$l$J$/$J$j$^$9!#(B

CHAN_HELP_SET_LEAVEOPS
	$B%%7%%s%%?%%/%%9!'(BSET channel LEAVEOPS {ON | OFF}
	
	+o$B%%b!<%%I$N@)8f$r$d$5$7$/$9$k$3$H$r@_Dj$7$^$9!#(BON$B$K$9$k$H!"(B
	%S$B$O<+F0E*$K(B-o$B%%b!<%%I$r$+$1$^$;$s!#(B

CHAN_HELP_ACCESS
	$B%%7%%s%%?%%/%%9!'(BACCESS channel ADD nick level
	$B!!!!!!!!!!!!(BACCESS channel DEL {nick | entry-num | list}
	$B!!!!!!!!!!!!(BACCESS channel LIST [mask | list]
	
	$B%%A%%c%%s%%M%%k$N%%"%%/%%;%%9I=$NJQ99$HI=<($N%%3%%^%%s%%I$G$9!#%%"%%/%%;%%9(B
	$BI=$O!"FC8"$N$"$k%%f!<%%6!<$N%%j%%9%%H$G!"$I$N%%f!<%%6!<$,(B+o$B%%b!<%%I(B
	$B$rF@$i$l$k$+$I$N%%f!<%%6!<$,(B%S$B$N%%3%%^%%s%%I$r;H$($k$+4^$_(B
	$B$^$9!#(Blevel$B!J%%l%%Y%%k!K$N?tCM$K$h$C$F$I$NFC8"$rF@$k$+JQ$o$j(B
	$B$^$9!#>\$7$$;v$J$i!"!V(B/msg %S HELP ACCESS LEVELS$B!W$r(B
	$BF~NO$7$F2<$5$$!#(B
	
	ACCESS ADD$B$H$$$&7A$G%%f!<%%6!<$r%%"%%/%%;%%9I=$KDI2C$G$-$^$9$,!"(B
	$B$9$G$K%%"%%/%%;%%9I=$KB8:_$7$F$$$k%%f!<%%6!<$N%%l%%Y%%k$rJQ99$9$k$3(B
	$B$H$b=PMh$^$9!#DI2C$HJQ99=PMh$k$N$O!"%%3%%^%%s%%I$rF~NO$9$k%%f!<(B
	$B%%6!<$N8=:_$N%%l%%Y%%k$r4^$^$:!"$=$N%%l%%Y%%k0J2<$N$_$G$9!#$^$?!"(B
	$B%%f!<%%6!<$N%%l%%Y%%k$rJQ99$9$k>l9g$O!"$=$N%%f!<%%6!<$N8=:_$N%%l%%Y(B
	$B%%k$OF~NO$9$k%%f!<%%6!<$N%%l%%Y%%k0J>e$J$iJQ99=PMh$^$;$s!#%%U%%!%%&(B
	$B%%s%%@!<$J$i$I$N%%l%%Y%%k$G$bDI2C$G$-$^$9!#(B
	
	ACCESS DEL$B$H$$$&7A$G%%f!<%%6!<$r%%"%%/%%;%%9I=$+$i>C5n$G$-$^$9!#(B
	$B>C5n=PMh$k$N$O!"F~NO$9$k%%f!<%%6!<$N%%l%%Y%%k$r4^$^$:!"$=$N%%l%%Y(B
	$B%%k0J2<$N%%f!<%%6!<$N$_$G$9!#HV9fNs$rF~NO$9$k$3$H$b=PMh$^$9(B
	$B!J2<5-$N(BLIST$B$NNc$X;2>H!K!#(B
	
	ACCESS LIST$B$H$$$&7A$G%%"%%/%%;%%9I=$r8+$k$3$H$,=PMh$^$9!#%%^%%9%%/(B
	$B$rF~NO$9$l$P!"$=$N%%^%%9%%/$K9g$&%%f!<%%6!<$7$+I=$l$^$;$s!#2<5-$N(B
	$BNc$NDL$j!"HV9fNs$rF~NO$9$k$3$H$b=PMh$^$9!'(B
	
	    ACCESS #channel LIST 2-5,7-9
	       $B#2!A#5!"#7!A#9HVL\$N%%"%%/%%;%%9I=$N%%(%%s%%H%%j!<$rI=$7$^$9!#(B

CHAN_HELP_ACCESS_LEVELS
	$B%%A%%c%%s%%M%%k$N%%"%%/%%;%%9%%l%%Y%%k(B
	
	$B4{DjE*$J%%l%%Y%%k$O<!$NDL$j$G$9!#(B
	
	$B%%U%%!%%&%%s%%@!<(B$B!!!!A4$F$N(B%S$B$N%%3%%^%%s%%I$r;H$($k(B
	$B!!!!!!!!!!!!(B$B!!!!%%A%%c%%s%%M%%k$KF~$k$H<+F0E*$K(B+o$B%%b!<%%I(B
	$B!!!!!!(B10$B!!!!(B$B!!!!<+F0E*$K(B+o$B%%b!<%%I(B
	$B!!!!!!!!!!!!(B$B!!!!(BAKICK$B$N%%3%%^%%s%%I$b;H$($k(B
	       5$B!!!!(B$B!!!!<+F0E*$K(B+o$B%%b!<%%I(B
	       3$B!!!!(B$B!!!!<+F0E*$K(B+v$B%%b!<%%I(B
	       0$B!!!!(B$B!!!!FC8"L5$7(B
	$B!!!!!!!!!!!!(B$B!!!!%%*%%Z%%l!<%%?!<$+$i(B+o$B%%b!<%%I$rF@$i$l$k(B
	      -1$B0J2<(B$B!!!!(B+o$B%%b!<%%I$rF@$i$l$J$$(B
	
	$B$3$N%%l%%Y%%k$rJQ$($?$j?7$7$$%%l%%Y%%k$r$D$/$C$?$j$9$k$3$H$,(B
	LEVELS$B$N%%3%%^%%s%%I$G=PMh$^$9!#(B

CHAN_HELP_AKICK
	$B%%7%%s%%?%%/%%9!'(BAKICK channel ADD mask [reason]
	$B!!!!!!!!!!!!(BAKICK channel DEL mask
	$B!!!!!!!!!!!!(BAKICK channel LIST [mask]
	
	$B!V<+F0=3I=!W!J%%*!<%%H%%-%%C%%/I=!K$rJQ99$HI=<($N%%3%%^%%s%%I$G$9!#(B
	$B<+F0=3I=$K$"$k%%f!<%%6!<$,%%A%%c%%s%%M%%k$KF~$k$H!"(B%S$B$O$=$N(B
	$B%%f!<%%6!<$r=3$jDIJ|%%^%%9%%/$rIU$1$^$9!#(B
	
	AKICK ADD$B$H$$$&7A$G<+F0=3I=$K%%^%%9%%/$H%%K%%C%%/$rDI2C$G$-$^$9!#(B
	$B%%^%%9%%/$J$i!"(Buser@host$B$H$$$&7A$GF~NO$7$F2<$5$$!#(Breason$B!JM}M3!K(B
	$B$O%%-%%C%%/%%a%%C%%;!<%%8$GI=$l$^$9!#M}M3$rF~NO$7$J$$>l9g$O!"!V(BYou
	have been banned from the channel$B!W!J%%A%%c%%s%%M%%k$+$iDIJ|$5$l(B
	$B$^$7$?!K$K$J$j$^$9!#(B
	
	AKICK DEL$B$H$$$&7A$G<+F0=3I=$+$i%%^%%9%%/$H%%K%%C%%/$r>C5n$G$-$^(B
	$B$9!#$?$@$7!"DIJ|%%^%%9%%/$,%%A%%c%%s%%M%%k$KIU$$$F$$$k>l9g$O!"$=$N(B
	$B%%^%%9%%/$O<+F0$K2r$+$l$^$;$s!#(B
	
	AKICK LIST$B$H$$$&7A$G<+F0=3I=$r8+$k$3$H$,=PMh$^$9!#%%^%%9%%/$r(B
	$BF~NO$9$l$P!"$=$N%%^%%9%%/$K9g$&%%(%%s%%H%%j!<$7$+I=$l$^$;$s!#(B

CHAN_HELP_LEVELS
	$B%%7%%s%%?%%/%%9!'(BLEVELS channel SET type level
	$B!!!!!!!!!!!!(BLEVELS channel {DIS | DISABLE} type
	$B!!!!!!!!!!!!(BLEVELS channel LIST
	$B!!!!!!!!!!!!(BLEVELS channel RESET
	
	$B%%A%%c%%s%%M%%k$N%%"%%/%%;%%9%%l%%Y%%k$NHV9f$N0UL#$rJQ99$7$^$9!#$3$N%%3(B
	$B%%^%%s%%I$G(B%S$B$N<+F0E*$J:nMQ$H$[$H$s$I$N(B%S$B$N%%3%%^%%s(B
	$B%%I$,;H$($k%%l%%Y%%k$N$r@_Dj$G$-$^$9!#$?$@$7!"(BSET FOUNDER$B!&(BSET
	PASSWORD$B!&(BLEVELS$B$N%%3%%^%%s%%I$O!"%%U%%!%%&%%s%%@!<$7$+;H$($^$;$s!#(B
	
	LEVELS SET$B$H$$$&7A$G:nMQ$d%%3%%^%%s%%I$N%%l%%Y%%k$rJQ99$G$-$^$9!#(B
	LEVELS DISABLE$B$H$$$&7A!J$=$l$H(BLEVELS DIS$B$H$$$&>JN,!K$G<+F0(B
	$BE*$J:nMQ$rDd;_$9$k$3$H$H!"%%3%%^%%s%%I$r%%U%%!%%&%%s%%@!<0J30$K6X@)(B
	$B$9$k$3$H$,=PMh$^$9!#(BLEVELS LIST$B$H$$$&7A$G<+F0E*$J:nMQ$H%%3%%^(B
	$B%%s%%I$N%%l%%Y%%k$rI=<($G$-$^$9!#(BLEVELS RESET$B$H$$$&7A$G%%l%%Y%%k$r(B
	$B4{DjCM$KLa$;$^$9!#(B
	
	type$B!J:nMQ$NL>A0!K$NA*Br$r8+$k>l9g$O!"!V(B/msg %S HELP
	LEVELS DESC$B!W$rF~NO$7$F2<$5$$!#(B

CHAN_HELP_LEVELS_DESC
	LEVELS$B$N:nMQ$H%%3%%^%%s%%I$NL>A0$O<!$NDL$j$G$9!#(B
	$BCm0U!'(BAUTODEOP$B$H(BNOJOIN$B$O:GBgCM$G!"B>$N$O:G>.CM$G$9!#(B

CHAN_HELP_LEVELS_DESC_FORMAT
	    %-*s  %s

CHAN_HELP_INFO
	$B%%7%%s%%?%%/%%9!'(BINFO channel
	
	$B%%A%%c%%s%%M%%k$N>pJs!J%%U%%!%%&%%s%%@!<!"L>>u!"EPO?$NF|;~!"%%b!<%%I(B
	$B%%m%%C%%/Ey!K$rI=$7$^$9!#(B

CHAN_HELP_LIST
	$B%%7%%s%%?%%/%%9!'(BLIST pattern
	
	pattern$B$K9g$&%%A%%c%%s%%M%%k$rI=$7$^$9!#$?$@$7!";dE*%%*%%W%%7%%g%%s(B
	$B$,(B ON $B$N%%A%%c%%s%%M%%k$OI=$l$^$;$s!#(B

CHAN_HELP_OP
	$B%%7%%s%%?%%/%%9!'(BOP #channel nick
	
	$B%%A%%c%%s%%M%%k$N%%f!<%%6!<$K%%*%%Z%%l!<%%?!<$K$7$^$9!#;HMQ$O4{DjE*$K(B
	$B%%l%%Y%%k(B5$B0J>e$K8B$C$F$$$^$9!#(B

CHAN_HELP_DEOP
	$B%%7%%s%%?%%/%%9!'(BDEOP #channel nick
	
	$B%%A%%c%%s%%M%%k$N%%f!<%%6!<$r(B-o$B%%b!<%%I$K$7$^$9!#;HMQ$O4{DjE*$K%%l%%Y(B
	$B%%k(B5$B0J>e$K8B$C$F$$$^$9!#(B

CHAN_HELP_INVITE
	$B%%7%%s%%?%%/%%9!'(BINVITE channel
	
	%S$B$K<+J,$rM6$o$;$^$9!#;HMQ$O4{DjE*$K%%l%%Y%%k(B1$B0J>e$K8B$C(B
	$B$F$$$^$9!#(B

CHAN_HELP_UNBAN
	$B%%7%%s%%?%%/%%9!'(BUNBAN channel
	
	$B8=:_$N%%f!<%%6!<%%"%%I%%l%%9$K9g$&DIJ|%%^%%9%%/$r2r$-$^$9!#;HMQ$O4{(B
	$BDjE*$K%%l%%Y%%k(B1$B0J>e$K8B$C$F$$$^$9!#(B

CHAN_HELP_CLEAR
	$B%%7%%s%%?%%/%%9!'(BCLEAR channel what
	
	$B%%A%%c%%s%%M%%k$N$"$k%%b!<%%I$r%%/%%j%%"$7$?$j!"%%f!<%%6!<$rA4$F=3$C$?(B
	$B$j$7$^$9!#(Bwhat$B$NA*Br$O<!$NDL$j$G$9!#(B
	
	     modes   $B%%A%%c%%s%%M%%k$NF~@Z%%b!<%%I!J(Bi$B!&(Bk$B!&(Bl$B!&(Bm$B!&(Bn$B!&(Bp$B!&(Bs$B!&(Bt$B!K(B
	             $B!!!!$r%%/%%j%%"$9$k(B
	     bans    $BDIJ|%%^%%9%%/$rA4$F2r$/(B
	     ops     $BA4$F$N%%f!<%%6!<$+$i(B+o$B%%b!<%%I$r%%/%%j%%"$9$k(B
	     voices  $BA4$F$N%%f!<%%6!<$+$i(B+v$B%%b!<%%I$r%%/%%j%%"$9$k(B
	     users   $BA4$F$N%%f!<%%6!<$r%%A%%c%%s%%M%%k$+$i=3$k(B
	
	$B;HMQ$O4{DjE*$K%%l%%Y%%k(B 10 $B0J>e$K8B$C$F$$$^$9!#(B

CHAN_SERVADMIN_HELP
	
	Services$B$N%%"%%I%%_%%s$O<!$N%%3%%^%%s%%I$b;H$($^$9!#(B
	
	    GETPASS    $B%%A%%c%%s%%M%%k$N%%Q%%9%%o!<%%I$r8+$k(B
	               $B!!!!!J0E9f2=$,;H$o$l$F$$$J$$>l9g$N$_!K(B
	    FORBID     $B%%A%%c%%s%%M%%k$r;HMQIT2DG=$K$9$k(B
	    STATUS     $B%%f!<%%6!<$N%%A%%c%%s%%M%%k$G$N%%"%%/%%;%%9%%l%%Y%%k(B
	
	$B$J$*!"(BServices$B$N%%"%%I%%_%%s$O%%Q%%9%%o!<%%I$rF~NO$;$:$K%%A%%c%%s%%M%%k(B
	$B$NEPO?$r>C5n$9$k$3$H$b!"%%"%%/%%;%%9I=!&<+F0=3I=!&%%l%%Y%%kI=$r8+(B
	$B$k$3$H$b=PMh$^$9!#(B

CHAN_SERVADMIN_HELP_DROP
	$B%%7%%s%%?%%/%%9!'(BDROP channel
	
	channel$B$H$$$&%%A%%c%%s%%M%%k$r(B%S$B$N%%G!<%%?%%Y!<%%9$+$i>C5n$7$^$9!#(B
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(B$B%%U%%!%%&%%s%%@!<(B$B$H(BServices$B$N%%"%%I%%_%%s(B$B$K(B
	$B8B$C$F$$$F!"(BServices$B$N%%"%%I%%_%%s$G$J$1$l$P%%Q%%9%%o!<%%I$rF~NO$9(B
	$B$kI,MW$b$"$j$^$9!#(B

CHAN_SERVADMIN_HELP_SET
	
	Services$B$N%%"%%I%%_%%s$O(BNOEXPIRE$B$H$$$&%%*%%W%%7%%g%%s$r@_Dj$G$-$^$9!#(B
	$B$3$N%%*%%W%%7%%g%%s$G%%A%%c%%s%%M%%k$NEPO?$r@Z$l$J$/$9$k$3$H$,=PMh$^(B
	$B$9!#$J$*!"(BServices$B$N%%"%%I%%_%%s$O%%Q%%9%%o!<%%I$rF~NO$;$:$K%%A%%c%%s(B
	$B%%M%%k$N%%*%%W%%7%%g%%s$r@_Dj$G$-$^$9!#(B

CHAN_SERVADMIN_HELP_SET_NOEXPIRE
	$B%%7%%s%%?%%/%%9!'(BSET channel NOEXPIRE {ON | OFF}
	
	channel$B$H$$$&%%A%%c%%s%%M%%k$NEPO?$O@Z$l$k$+$r@_Dj$7$^$9!#(BON$B$K$9(B
	$B$k$H!"EPO?$O@Z$l$^$;$s!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B

CHAN_SERVADMIN_HELP_LIST
	$B%%7%%s%%?%%/%%9!'(BLIST pattern
	
	pattern$B$K9g$&%%A%%c%%s%%M%%k$rI=$7$^$9!#$?$@$7!";dE*%%*%%W%%7%%g%%s(B
	$B$rF~$7$?%%A%%c%%s%%M%%k$O(BServices$B$N%%"%%I%%_%%s$K$7$+I=$l$^$;$s!#(B

CHAN_SERVADMIN_HELP_FORBID
	$B%%7%%s%%?%%/%%9!'(BFORBID channel
	
	$B%%K%%C%%/$r;HMQIT2DG=$K$7$^$9!#%%-%%c%%s%%;%%k$K$O(BDROP$B$N%%3%%^%%s%%I$,(B
	$B;H$($^$9!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B

CHAN_SERVADMIN_HELP_STATUS
	$B%%7%%s%%?%%/%%9!'(BSTATUS channel nickname
	
	nickname$B$H$$$&%%K%%C%%/$N(Bchannel$B$H$$$&%%A%%c%%s%%M%%k$G$N%%"%%/%%;(B
	$B%%9%%l%%Y%%k$rI=<($7$^$9!#H?1~$O$3$N7A$G$9!'(B
	
	    STATUS channel nickname access-level
	
	$B%%(%%i!<$,H/@8$7$?>l9g$O!"H?1~$O<!$N7A$G$9!'(B
	
	    STATUS ERROR error-message
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B

###########################################################################
#
# MemoServ$B$N%%X%%k%%W%%a%%C%%;!<%%8(B
#
###########################################################################

MEMO_HELP
	%S$B$O!"%%f!<%%6!<$H%%A%%c%%s%%M%%k$K%%a%%b$rAw$k$?$a$N%%5!<%%S%%9(B
	$B$G$9!#%%a%%b$O(BServices$B$K$h$jJ]B8$5$l!"<u?.%%K%%C%%/$+%%A%%c%%s%%M%%k(B
	$B$N%%*%%Z%%l!<%%?!<$OFI$_$?$$;~$KFI$a$^$9!#$?$@$7!":9=P?M$N%%K%%C(B
	$B%%/$b<u?.%%K%%C%%/$+%%A%%c%%s%%M%%k$bEPO?$7$J$1$l$P(B%S$B$r;H$($^$;$s!#(B
	
	%S$B$N%%3%%^%%s%%I$O<!$NDL$j$G$9!#(B
	
	    SEND   $B%%f!<%%6!<$+%%A%%c%%s%%M%%k$K%%a%%b$rAw$k(B
	    LIST   $B<u?.%%a%%bI=$rI=$9(B
	    READ   $B%%a%%b$rFI$`(B
	    DEL    $B%%a%%b$r>C5n$9$k(B
	    SET    $B%%a%%b$N%%*%%W%%7%%g%%s$r@_Dj$9$k(B
	
	$B%%3%%^%%s%%I$N%%X%%k%%W$rFI$`>l9g$O!"!V(B/msg %S HELP command$B!W(B
	$B$rF~NO$7$F2<$5$$!#(B
	
	$B"(%%A%%c%%s%%M%%k$N%%a%%b$,FI$a$k$N$O4{DjE*$K%%l%%Y%%k(B10$B0J>e$N%%f!<%%6!<(B
	$B!!$N$_$G$9$,!"(B%s$B$N!V(BLEVELS$B!W$H$$$&%%3%%^%%s%%I$G$=$N%%l%%Y%%k$rJQ(B
	$B!!99$G$-$^$9!#(B

MEMO_HELP_SEND
	$B%%7%%s%%?%%/%%9!'(BSEND {nick | channel} memo-text
	
	nick$B$+(Bchannel$B$K(Bmemo-text$B$r4^$`%%a%%b$rAw$j$^$9!#%%K%%C%%/$K=P$9(B
	$B>l9g$K$O!"<u?.<T$N%%K%%C%%/%%*%%W%%7%%g%%s$K$h$C$F!"(B%S$B$O<u?.(B
	$B<T$K?7$?$J%%a%%b$,FO$$$?$H$N$3$H$rJs9p$7$^$9!#(B

MEMO_HELP_LIST
	$B%%7%%s%%?%%/%%9!'(BLIST [channel] [list | NEW]
	
	$B<u?.%%a%%bI=$rI=$7$^$9!#(BNEW$B$rF~NO$9$l$P!"FI$s$G$$$J$$%%a%%b$N$_(B
	$B$,I=$l$^$9!#FI$s$G$$$J$$%%a%%b$O!"%%a%%b$NHV9f$N:8B&$K!v0u$,I=(B
	$B<($5$l$^$9!#2<5-$NNc$NDL$j!"HV9fNs$bF~NO2DG=!'(B
	
	    LIST 2-5,7-9
	       $B#2!A#5!"#7!A#9HVL\$N%%a%%b$r%%j%%9%%H$7$^$9!#(B

MEMO_HELP_READ
	$B%%7%%s%%?%%/%%9!'(BREAD [channel] {num | list | LAST | NEW}
	
	$B%%a%%b$rFI$_$^$9!#HV9f$rF~NO$9$k$H$=$NHV9f$N%%a%%b$,I=$l!"(BLAST$B$r(B
	$BF~NO$9$k$H:G?7$N%%a%%b$,I=$l!"(BNEW$B$rF~NO$9$k$HFI$s$G$$$J$$%%a%%b(B
	$B$,A4$FI=$l$^$9!#2<5-$NNc$NDL$j!"HV9fNs$bF~NO2DG=!'(B
	
	    LIST 2-5,7-9
	       $B#2!A#5!"#7!A#9HVL\$N%%a%%b$r%%j%%9%%H$7$^$9!#(B

MEMO_HELP_DEL
	$B%%7%%s%%?%%/%%9!'(BDEL [channel] {num | list | ALL}
	
	$B%%a%%b$r>C5n$7$^$9!#HV9f$rF~NO$9$k$H$=$NHV9f$N%%a%%b$,>C5n$5$l!"(B
	ALL$B$rF~NO$9$k$HA4$F$N%%a%%b$,>C5n$5$l$^$9!#(B
	
	$BNc!'(B
	   DEL 1
	      $B#1HVL\$N%%a%%b$r>C5n$7$^$9!#(B
	   DEL 2-5,7-9
	      $B#2HVL\!A#5HVL\$H#7HVL\!A#9HVL\$N%%a%%b$r>C5n$7$^$9!#(B

MEMO_HELP_SET
	$B%%7%%s%%?%%/%%9!'(BSET option parameters
	
	$B%%a%%b$N%%*%%W%%7%%g%%s$r@_Dj$7$^$9!#(Boption$B$NA*Br$O<!$NDL$j$G$9!#(B
	
	    NOTIFY      $B$I$N>l9g$K(B%S$B$,?7%%a%%b$NJs9p$9$k$+$N@_Dj(B
	                $B!!!J%%K%%C%%/$N$_!K(B
	    LIMIT       $B%%a%%b$N?t$N8B$j$N@_Dj(B
	
	$B%%*%%W%%7%%g%%s$N>pJs$rFI$`>l9g$O!"!V(B/msg %S HELP SET option$B!W(B
	$B$rF~NO$7$F2<$5$$!#(B

MEMO_HELP_SET_NOTIFY
	$B%%7%%s%%?%%/%%9!'(BSET NOTIFY {ON | LOGON | NEW | OFF}
	
	$B$I$N>l9g$K(B%S$B$,?7%%a%%b$NJs9p$9$k$+$N@_Dj$N%%3%%^%%s%%I$G$9!#(B
	$B@_Dj$NA*Br$O<!$NDL$j$G$9!#(B
	
	    ON      $B%%m%%0%%*%%s$9$k$H$-!"(BIRC$B$KLa$k$H$-!"%%a%%b$,FO$/$H$-(B
	    LOGON   $B%%m%%0%%*%%s$9$k$H$-!"(BIRC$B$KLa$k$H$-(B
	    NEW     $B%%a%%b$,FO$/$H$-(B
	    OFF     $BJs9p$OL5$7(B
	
	ON$B$N7k2L$O(BLOGON$B$H(BNEW$B$N7k2L$r9g$o$;$k$3$H$H;W$($^$9!#(B

MEMO_HELP_SET_LIMIT
	$B%%7%%s%%?%%/%%9!'(BSET LIMIT [channel] limit
	
	$B%%a%%b$N?t$N8B$j$r@_Dj$7$^$9!#%%<%%m$K$9$k$H!"%%a%%b$r<u$1$i$l$J(B
	$B$/$J$j$^$9!#$?$@$7!"@5?t$N>l9g$O(B%d$B0J2<$N?tCM$7$+F~NO$G$-$^(B
	$B$;$s!#(B

MEMO_HELP_INFO
	$B%%7%%s%%?%%/%%9!'(BINFO [channel]
	
	$B%%a%%b$N>pJs$r8+$^$9!#$3$N%%3%%^%%s%%I$GI=$l$k$N$O!"8=:_$N%%a%%b$N(B
	$B?t$HFI$s$G$$$J$$%%a%%b$N?t$H%%a%%b$N?t$N8B$j$G$9!#%%Q%%i%%a!<%%?!<(B
	$B$rF~NO$9$l$P$=$N%%A%%c%%s%%M%%k$N>pJs$,I=$l$^$9!#(B

MEMO_SERVADMIN_HELP_SET_LIMIT
	$B%%7%%s%%?%%/%%9!'(BSET LIMIT [user | channel] {limit | NONE} [HARD]
	
	$B%%f!<%%6!<$+%%A%%c%%s%%M%%k$N%%a%%b$N?t$N8B$j$r@_Dj$7$^$9!#%%<%%m$K$9(B
	$B$k$H%%f!<%%6!<$O%%a%%b$r<u$1$i$l$J$/$J$j!"(BNONE$B$K$9$k$H8B$j$OL5(B
	$B$/$J$j$^$9!#%%K%%C%%/$rF~NO$7$J$$>l9g$O!"%%3%%^%%s%%I$r=P$9%%f!<%%6(B
	$B!<$N%%K%%C%%/$N%%a%%b$N8B$j$,@_Dj$5$l$^$9!#(BHARD$B$rF~NO$9$l$P!"%%f!<(B
	$B%%6!<$O8B$j$rJQ99$G$-$J$/$J$j!"F~NO$7$J$1$l$P%%f!<%%6!<$O8B$j(B
	$B$rJQ99$G$-$^$9!#(B
	
	$B$3$N;H$$J}$,;H$($k$N$O(BServices$B$N%%"%%I%%_%%s(B$B$N$_$G$9!#0lHL$N(B
	$B%%f!<%%6!<$O!"B>$N%%f!<%%6!<$N8B$j$r@_Dj$G$-$J$$$7!"8B$j$r$N$1(B
	$B$i$l$J$$$7!"(BHARD$B$bF~NO$G$-$^$;$s!#(B

MEMO_SERVADMIN_HELP_INFO
	$B%%7%%s%%?%%/%%9!'(BINFO [nick | channel]
	
	$B%%a%%b$N>pJs$r8+$^$9!#$3$N%%3%%^%%s%%I$GI=$l$k$N$O!"8=:_$N%%a%%b$N(B
	$B?t$HFI$s$G$$$J$$%%a%%b$N?t$H%%a%%b$N?t$N8B$j$G$9!#%%Q%%i%%a!<%%?!<(B
	$B$,$"$k>l9g$K$=$N%%f!<%%6!<$+%%A%%c%%s%%M%%k$N>pJs$,I=$l!"%%Q%%i%%a!<(B
	$B%%?!<$,L5$$>l9g$K<+J,$N>pJs$,I=$l$^$9!#(B
	
	$B%%K%%C%%/$N%%Q%%i%%a!<%%?!<$rF~NO$G$-$k$N$O(BServices$B$N%%"%%I%%_%%s(B$B$N$_(B
	$B$G$9!#(B

###########################################################################
#
# OperServ$B$N%%X%%k%%W%%a%%C%%;!<%%8(B
#
###########################################################################

OPER_HELP
	%S$B$N%%3%%^%%s%%I$O<!$NDL$j$G$9!#(B
	
	$B0lHL$N(BIRC$B$N%%*%%Z%%l!<%%?!<$,;H$($k%%3%%^%%s%%I!'(B
	    GLOBAL      $BA4$F$N%%f!<%%6!<$K%%a%%C%%;!<%%8$rAw$k(B
	    STATS       Services$B$H%%M%%C%%H$N%%9%%F!<%%?%%9$rI=$9(B
	    OPER LIST   Services$B$N%%*%%Z%%l!<%%?!<I=$rI=$9(B
	    ADMIN LIST  Services$B$N%%"%%I%%_%%sI=$rI=$9(B
	
	Services$B$N%%*%%Z%%l!<%%?!<$H%%"%%I%%_%%s$@$1$,;H$($k%%3%%^%%s%%I!'(B
	    MODE        $B%%A%%c%%s%%M%%k$N%%b!<%%I$rJQ99$9$k(B
	    KICK        $B%%A%%c%%s%%M%%k$+$i%%f!<%%6!<$r%%-%%C%%/$9$k(B
	    AKILL       $B<+F0@ZCGI=$rJQ99$9$k(B
	
	Services$B$N%%"%%I%%_%%s$@$1$,;H$($k%%3%%^%%s%%I!'(B
	    OPER        Services$B$N%%*%%Z%%l!<%%?!<I=$rJQ99$9$k(B
	    LOGONNEWS   $B%%f!<%%6!<$,@\B3$9$k;~$K=P$5$l$k%%a%%C%%;!<%%8$r(B
	                $B!!!!:n$k(B
	    OPERNEWS    $B%%f!<%%6!<$,%%*%%Z%%l!<%%?!<!J(B+o$B%%b!<%%I!K$K$J$k;~(B
	                $B!!!!$K=P$5$l$k%%a%%C%%;!<%%8$r:n$k(B
	    JUPE        $B%%5!<%%P!<$r!V%%8%%e%%T%%?!<!W!J@\B35qH]!K$9$k(B
	    RAW         Services$B$N@\B3%%5!<%%P!<$K%%F%%-%%9%%H$r=P$9(B
	    SET         Services$B$N%%*%%W%%7%%g%%s$N@_Dj(B
	    QUIT        Services$B$r;_$a$k(B
	    RESTART     Services$B$r:F5/F0$5$;$k(B
	
	Services root$B!J%%9!<%%Q!<%%f!<%%6!<!K$@$1$,;H$($k%%3%%^%%s%%I!'(B
	    ADMIN       Services$B$N%%"%%I%%_%%sI=$rJQ99$9$k(B
	
	$BCm0U!'(B%S$B$K=P$5$l$?%%3%%^%%s%%I$OA4$F%%m%%C%%0$5$l$^$9!#(B

OPER_HELP_GLOBAL
	$B%%7%%s%%?%%/%%9!'(BGLOBAL message
	
	$BA4$F$N%%M%%C%%H$K@\B3$7$F$$$k%%f!<%%6!<$K%%a%%C%%;!<%%8$r=P$7$^$9!#(B
	$B%%a%%C%%;!<%%8$N;X=P%%K%%C%%/$O(B%s$B$G$9!#(B

OPER_HELP_STATS
	$B%%7%%s%%?%%/%%9!'(BSTATS [AKILL | ALL]
	
	$B%%Q%%i%%a!<%%?!<$,$J$$>l9g$O!"8=:_$N%%f!<%%6!<?t$H%%*%%Z%%l!<%%?!<?t!"(B
	$B:GBg%%f!<%%6!<?t!"5/F0$+$i$N;~4V$rI=$7$^$9!#(B
	
	AKILL$B$H$$$&%%Q%%i%%a!<%%?!<$,$"$k>l9g$O!"8=:_$N<+F0@ZCG%%^%%9%%/$N(B
	$B?t$H4{DjE*$J<wL?$rI=$7$^$9!#(B
	
	ALL$B$H$$$&%%*%%W%%7%%g%%s$,;H$($k$N$O(BServices$B$N%%"%%I%%_%%s$N$_$G$9!#(B
	$B$3$N%%*%%W%%7%%g%%s$rIU$1$k$H!"IaDL$N>pJs!J%%f!<%%6!<?t$H5/F0$+$i(B
#@@@ $B!V(Bmemory usage$B!W$NK]Lu(B
	$B$N;~4V!K$N2<$G(BServices$B$N;HMQ$7$F$$$k%%a%%b%%j!<NL$bI=$l$^$9!#(B
	$B$3$N7A$r;H$&$H(BServices$B$O>/$7$N4V%%U%%j!<%%:$7$F$7$^$$$^$9$N$G!"(B
	$B$4Cm0U2<$5$$!#(B
	
	$B!V(BSTATS$B!W$NBe$o$j$K!V(BUPTIME$B!W$b;H$($^$9!#(B

OPER_HELP_OPER
	$B%%7%%s%%?%%/%%9!'(BOPER ADD nick
	$B!!!!!!!!!!!!(BOPER DEL nick
	$B!!!!!!!!!!!!(BOPER LIST
	
	Services$B$N%%*%%Z%%l!<%%?!<I=$NJQ99$HI=<($N%%3%%^%%s%%I$G$9!#$3$N%%j(B
	$B%%9%%H$K$"$k%%f!<%%6!<$O!"(B%s$B$N(BIDENTIFY$B$N%%3%%^%%s%%I$G%%Q%%9%%o!<(B
	$B%%I$rF~NO$9$l$P(BServices$B$N%%*%%Z%%l!<%%?!<$N%%3%%^%%s%%I$,;H$($^$9!#(B
	
	$BC/$G$b(BOPER LIST$B$H$$$&7A$,;H$($^$9!#B>$N;HMQ$O(BServices$B$N%%"%%I(B
	$B%%_%%s(B$B$K8B$C$F$$$^$9!#(B

OPER_HELP_ADMIN
	$B%%7%%s%%?%%/%%9!'(BADMIN ADD nick
	$B!!!!!!!!!!!!(BADMIN DEL nick
	$B!!!!!!!!!!!!(BADMIN LIST
	
	Services$B$N%%"%%I%%_%%sI=$NJQ99$HI=<($N%%3%%^%%s%%I$G$9!#$3$N%%j%%9%%H(B
	$B$K$"$k%%f!<%%6!<$O!"(B%s$B$N(BIDENTIFY$B$N%%3%%^%%s%%I$G%%Q%%9%%o!<%%I(B
	$B$rF~NO$9$l$P(BServices$B$N%%"%%I%%_%%s$N%%3%%^%%s%%I$,;H$($^$9!#(B
	
	$BC/$G$b(BADMIN LIST$B$H$$$&7A$,;H$($^$9!#B>$N;HMQ$O(BServices root
	$B$K8B$C$F$$$^$9!#(B

OPER_HELP_MODE
	$B%%7%%s%%?%%/%%9!'(BMODE channel modes
	
	$B%%A%%c%%s%%M%%k$N%%b!<%%I$rJQ99$7$^$9!#%%Q%%i%%a!<%%?!<$O(BIRC$B$N!V(B/MODE$B!W(B
	$B$N%%3%%^%%s%%I$HF1$8$G$9!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%*%%Z%%l!<%%?!<(B$B$K8B$C$F$$$^$9!#(B

OPER_HELP_CLEARMODES
	$B%%7%%s%%?%%/%%9!'(BCLEARMODES channel [ALL]
	
	$B%%A%%c%%s%%M%%k$NF~@Z%%b!<%%I!J(Bi$B!&(Bk$B!&(Bl$B!&(Bm$B!&(Bn$B!&(Bp$B!&(Bs$B!&(Bt$B!K$HDIJ|%%^%%9(B
	$B%%/$rA4$F%%/%%j%%"$7$^$9!#(BALL$B$H$$$&%%Q%%i%%a!<%%?!<$rF~NO$9$k$H!"(B
	o$B!&(Bv$B%%b!<%%I$b%%/%%j%%"$5$l$^$9!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%*%%Z%%l!<%%?!<(B$B$K8B$C$F$$$^$9!#(B

OPER_HELP_KICK
	$B%%7%%s%%?%%/%%9!'(BKICK channel user reason
	
	$B%%A%%c%%s%%M%%k$+$i%%f!<%%6!<$r%%-%%C%%/$7$^$9!#%%Q%%i%%a!<%%?!<$O(BIRC$B$N(B
	/KICK$B$H$$$&%%3%%^%%s%%I$HF1$8$G$9$,!"%%-%%C%%/%%a%%C%%;!<%%8$NA0$K$3$N(B
	$B%%3%%^%%s%%I$r;H$C$?%%f!<%%6!<$N%%K%%C%%/$,I=$l$^$9!#(B
#@@@ $BNc$,I,MW(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%*%%Z%%l!<%%?!<(B$B$K8B$C$F$$$^$9!#(B

OPER_HELP_AKILL
	$B%%7%%s%%?%%/%%9!'(BAKILL ADD [+expiry] mask reason
	$B!!!!!!!!!!!!(BAKILL DEL mask
	$B!!!!!!!!!!!!(BAKILL LIST [mask]
	$B!!!!!!!!!!!!(BAKILL VIEW [mask]
	
	$B!V<+F0@ZCGI=!W!J%%*!<%%H%%-%%kI=!K$NJQ99$HI=<($N%%3%%^%%s%%I$G$9!#(B
	$B<+F0@ZCG%%^%%9%%/$K9g$&%%f!<%%6!<$,@\B3$9$k$H!"(BServices$B$O$=$N(B
	$B%%f!<%%6!<$r@ZCG$7!"%%5!<%%P!<$N%%W%%m%%H%%3%%k$K$h$C$F$=$N%%^%%9%%/$r(B
	$B!V(BK:$B%%i%%$%%s!W$K$7$^$9!#(B
	
	AKILL ADD$B$H$$$&7A$G%%^%%9%%/$r<+F0@ZCGI=$KDI2C$G$-$^$9!#(B
	reason$B!JM}M3!K$OI,MW$G$9$,!"%%*%%Z%%l!<%%?!<$N0Y$@$1$G$9$N$G!"(B
	$B@ZCG$5$l$?%%f!<%%6!<$N@ZCG%%a%%C%%;!<%%8$KI=$l$^$;$s!#(B
	expiry$B$rF~NO$9$l$P!"<+F0@ZCG%%(%%s%%H%%j!<$N<wL?$r;XDj$G$-$^$9!#(B
	$B7A$O@5?t$K;~4V$NC10L$r;X<($9$kJ8;z$rIU$1$^$9!#C10L$NJ8;z$O(B
	$B!V(Bd$B!W!JF|!K!"!V(Bh$B!W!J;~4V!K!"!V(Bm$B!W!JJ,!K$G!"J8;z$rIU$1$J$$$H(B
	$BF|$K$J$j$^$9!#$?$@$7!"J8;z$r9g$o$;$k$3$H!J!V(B1h30m$B!W$J$I!K$O(B
	$B=PMh$^$;$s!#!V(B+0$B!W$K$9$l$P!"<+F0@ZCG%%(%%s%%H%%j!<$O@Z$l$^$;$s!#(B
	$B<wL?$rF~NO$7$J$$>l9g$O!"4{Dj<wL?$K$J$j$^$9!#(BSTATS AKILL$B$H$$(B
	$B$&%%3%%^%%s%%I$G$=$N4{DjCM$rI=$9$3$H$,=PMh$^$9!#(B
	$B!!"(%%^%%9%%/$O!V(B+$B!W$G;O$^$k>l9g$O!"<wL?$rF~NO$7$J$1$l$P$J$j$^(B
	$B!!!!$;$s!#(B
	
	AKILL DEL$B$H$$$&7A$G%%^%%9%%/$r<+F0@ZCGI=$+$i>C5n$G$-$^$9!#(B
	
	AKILL LIST$B$H$$$&7A$G<+F0@ZCGI=$rI=$;$^$9!#%%Q%%i%%a!<%%?!<$rF~(B
	$BNO$9$l$P!"$=$N%%^%%9%%/$K9g$&%%(%%s%%H%%j!<$7$+I=$l$^$;$s!#(B
	
	AKILL VIEW$B$H$$$&7A$G(BAKILL LIST$B$N$h$&$K<+F0@ZCGI=$rI=$9$3$H(B
	$B$,=PMh$^$9$,!"(BLIST$B$h$jB?$/$N>pJs!JF~NO$7$?%%f!<%%6!<$HF|;~!"(B
	$B@Z$l$kF|;~!K$,=P$^$9!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%*%%Z%%l!<%%?!<(B$B$K8B$C$F$$$^$9!#(B

OPER_HELP_JUPE
	$B%%7%%s%%?%%/%%9!'(BJUPE server [reason]
	
	$B%%5!<%%P!<$r!V%%8%%e%%T%%?!<!W$7$^$9!#%%8%%e%%T%%?!<!J(Bjupiter$B!K$H$O!"(B
	$B%%5!<%%P!<$r%%M%%C%%H$K@\B3$5$;$J$$0Y$K2>%%5!<%%P!<$r:n%%j%%M%%C%%H$K$D(B
	$B$J$2$k$3$H$G$9!#2>%%5!<%%P!<$O!V(B/SQUIT$B!W$H$$$&(BIRC$B$N%%3%%^%%s%%I$G(B
	$B>C$9$3$H$,=PMh$^$9!#(Breason$B!JM}M3!K$rF~NO$9$l$P!"$=$NJ8;z(B
	$BNs$O2>%%5!<%%P!<$N(B/LINKS$B$J$I$G%%5!<%%P!<>pJs$H$7$FI=$l$^$9!#M}M3(B
	$B$rF~NO$7$J$$>l9g$O!"%%5!<%%P!<L>$,%%5!<%%P!<>pJs$H$7$F;HMQ$5$l$^(B
	$B$9!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B

OPER_HELP_RAW
	$B%%7%%s%%?%%/%%9!'(BRAW text
	
	Services$B$,@\B3$7$F$$$k%%5!<%%P!<$K%%F%%-%%9%%H$r=P$7$^$9!#(BIRC$B$N%%W(B
	$B%%m%%H%%3%%k$,$O$C$-$jJ,$+$i$:$K$3$N%%3%%^%%s%%I$r;H$C$?$j!"F~NO$G(B
	$B4V0c$$$r$7$F$3$N%%3%%^%%s%%I$r=P$7$?$j$7$F$7$^$($P!"%%M%%C%%H$r68(B
	$B$o$;$k$3$H$,$"$j$^$9$N$G!"=<J,$4Cm0U2<$5$$!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B

OPER_HELP_SET
	$B%%7%%s%%?%%/%%9!'(BSET option setting
	
	Services$BA4BN$N%%*%%W%%7%%g%%s$r@_Dj$7$^$9!#%%*%%W%%7%%g%%s$NL>A0$O<!(B
	$B$NDL$j$G$9!#(B
	    READONLY   $B%%j!<%%I%%*%%s%%j!<%%b!<%%I$NF~@Z(B
	    DEBUG      $B%%G%%P%%C%%0%%b!<%%I$NF~@Z(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B

OPER_HELP_SET_DEBUG
	$B%%7%%s%%?%%/%%9!'(BSET DEBUG {ON | OFF | num}
	
	$B%%G%%P%%C%%0%%b!<%%I$rF~@Z$7$^$9!#(BON$B$K$9$l$P!"(BServices$B$,<u$1$?$j(B
	$B=P$7$?$j$9$k%%F%%-%%9%%H$HB>$N%%G%%P%%C%%0%%a%%C%%;!<%%8$O%%m%%C%%0$5$l$^(B
	$B$9!#(Bnum$B$H$$$&@5?t$rF~NO$9$k$H!"%%G%%P%%C%%0$N%%l%%Y%%k$O(Bnum$B$K@_Dj(B
	$B$5$l$^$9!#(B
	
	$B$3$N%%3%%^%%s%%I$O!V(B-debug$B!W$H$$$&%%3%%^%%s%%I%%i%%$%%s%%*%%W%%7%%g%%s$HF1(B
	$B$87k2L$r=P$7$^$9!#(B

OPER_HELP_QUIT
	$B%%7%%s%%?%%/%%9!'(BQUIT
	
	$B%%G!<%%?%%Y!<%%9$rJ]B8(B$B$;$:$K(BServices$B$r>C$7$^$9!#$3$N%%3%%^%%s%%I(B
	$B$r;H$&$Y$->l9g$O!"%%G!<%%?%%Y!<%%9$N68$$$N62$l$,$"$k>l9g$N$_$G(B
	$B$9!#B>$N>l9g$O!"(BSHUTDOWN$B$N%%3%%^%%s%%I$r$*;H$$2<$5$$!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B

OPER_HELP_SHUTDOWN
	$B%%7%%s%%?%%/%%9!'(BSHUTDOWN
	
	$B%%G!<%%?%%Y!<%%9$rJ]B8$7$F(BServices$B$r>C$7$^$9!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B

OPER_HELP_RESTART
	$B%%7%%s%%?%%/%%9!'(BRESTART
	
	$B%%G!<%%?%%Y!<%%9$rJ]B8$7$F(BServices$B$r:F5/F0$5$;$^$9!#(B
	
	$B$3$N%%3%%^%%s%%I$N;HMQ$O(BServices$B$N%%"%%I%%_%%s(B$B$K8B$C$F$$$^$9!#(B
