# Japanese language file (EUC encoding) / 日本語のメッセージファイル
#
# Services is copyright (c) 1996-1999 Andrew Church.
#     E-mail: <achurch@dragonfire.net>
# Services is copyright (c) 1999-2000 Andrew Kempe.
#     E-mail: <theshadow@shadowfire.org>
# This program is free but copyrighted software; see the file COPYING for
# details.

###########################################################################
#
# 言語の名前
#
###########################################################################

LANG_NAME
	日本語（EUCコード） (Japanese / EUC encoding)

###########################################################################
#
# 多用のメッセージ
#
###########################################################################

# strftime()の日時フォーマット
STRFTIME_DATE_TIME_FORMAT
	%Y.%m.%d %H:%M:%S %Z
# strftime()の年月日フォーマット（2）
STRFTIME_SHORT_DATE_FORMAT
	%Y.%m.%d

# strftime()のフォーマットで使用されるテキスト
STRFTIME_DAYS_SHORT
	日
	月
	火
	水
	木
	金
	土
STRFTIME_DAYS_LONG
STRFTIME_MONTHS_SHORT
STRFTIME_MONTHS_LONG


# ChanServ・NickServの情報表で使われるテキスト
COMMA_SPACE
	、


# エラーのメッセージ
USER_RECORD_NOT_FOUND
	エラーが発生し進行できません。
UNKNOWN_COMMAND
	%s というコマンドは分かりません。
UNKNOWN_COMMAND_HELP
	%s というコマンドは分かりません。「/msg %s HELP」を入力して下さい。
SYNTAX_ERROR
	シンタクス：%s
MORE_INFO
	「/msg %s HELP %s」を入力して下さい。
NO_HELP_AVAILABLE
	「%s」のヘルプがありません。

BAD_USERHOST_MASK
	マスクを user@host という形で入力して下さい。

ACCESS_DENIED
	アクセス不能。
PERMISSION_DENIED
	アクセス不能。
PASSWORD_INCORRECT
	パスワードは違います。

MORE_OBSCURE_PASSWORD
	もっと不明なパスワードにして下さい。パスワードは、お名前・ニック等のような分かりやすいことでなく、５文字以上のパスワードにして下さい。また、スペース（空白）はパスワードで使えません。
PASSWORD_TRUNCATED
	注意：入力したパスワードが %d 文字で切られました。

NICK_NOT_REGISTERED
	ニックは登録されていません。
NICK_NOT_REGISTERED_HELP
	ニックは登録されていません。登録したい場合は「/msg %s HELP」を入力して下さい。
NICK_X_NOT_REGISTERED
	%s というニックは登録されていません。
NICK_X_NOT_IN_USE
	%s というニックは現在使用されていません。
NICK_X_FORBIDDEN
	%s というニックを使用禁止です。
CHAN_X_NOT_REGISTERED
	%s というチャンネルは登録されていません。
CHAN_X_NOT_IN_USE
	%s というチャンネルは現在存在しません。
CHAN_X_FORBIDDEN
	%s というチャンネルは使用禁止です。
NICK_IDENTIFY_REQUIRED
	そのコマンドを使うにはパスワードを入力する必要があります。
	「/msg %s IDENTIFY password」を入力して下さい。
CHAN_IDENTIFY_REQUIRED
	そのコマンドを使うにはパスワードを入力する必要があります。
	「/msg %s IDENTIFY %s password」を入力して下さい。

###########################################################################
#
# NickServのメッセージ
#
###########################################################################

# 自動的なメッセージ
NICK_IS_REGISTERED
	このニックは登録されています。他のニックに変えて下さい。
	（保有者なら、「/msg %s IDENTIFY password」を入力して下さい。）
NICK_IS_SECURE
	このニックは登録され庇られています。保有者なら、
	「/msg %s IDENTIFY password」を入力して下さい。
	保有者でなければ、他のニックに変えて下さい。
NICK_MAY_NOT_BE_USED
	このニックはお使いになれません。

# REGISTERの反応
NICK_REGISTER_SYNTAX
	REGISTER password
NICK_REGISTRATION_FAILED
	登録は失敗しました。
NICK_REG_PLEASE_WAIT
	REGISTERのコマンドを再入力する前に、%d 秒お待ち下さい。
NICK_CANNOT_BE_REGISTERED
	%s というニックは使用禁止ですので、登録出来ません。
NICK_ALREADY_REGISTERED
	%s というニックは既に登録されています。
NICK_REGISTERED
	%s というニックは登録されました。
NICK_PASSWORD_IS
	ニックのパスワードは %s です。覚えておいて下さい。

# IDENTIFYの反応
NICK_IDENTIFY_SYNTAX
	IDENTIFY password
NICK_IDENTIFY_FAILED
	IDENTIFYは失敗しました。
NICK_IDENTIFY_SUCCEEDED
	正しいパスワードです。

# DROPの反応
NICK_DROP_SYNTAX
	DROP
NICK_DROPPED
	ニックは消去されました。
NICK_X_DROPPED
	%s というニックの登録は消去されました。

# SETの反応
NICK_SET_SYNTAX
	SET option parameters
NICK_SET_SERVADMIN_SYNTAX
	SET [nick] option parameters
NICK_SET_UNKNOWN_OPTION
	%s というオプションは分かりません。
NICK_SET_UNKNOWN_OPTION_OR_BAD_NICK
	%s はオプションでも登録したニックでもありません。

# SET PASSWORDの反応
NICK_SET_PASSWORD_CHANGED
	パスワードは変更されました。
NICK_SET_PASSWORD_CHANGED_TO
	パスワードは %s に変更されました。

# SET LANGUAGEの反応
NICK_SET_LANGUAGE_SYNTAX
	SET LANGUAGE number
NICK_SET_LANGUAGE_UNKNOWN
	%d 番の言語は存在しません。言語表を見る場合、「msg %s HELP SET LANGUAGE」を入力して下さい。
NICK_SET_LANGUAGE_CHANGED
	言語は日本語（EUCコード）に変更されました。
	注意：このテキストはアメリカ人に作られましたので、間違いは多いでしょうが、我慢して下さい。もしこのテキストを直して頂ければ、どうぞ achurch@dragonfire.net へ連絡をお願い致します。

# SET URLの反応
NICK_SET_URL_CHANGED
	ホームページのアドレスは %s に変更されました。
NICK_SET_URL_UNSET
	ホームページのアドレスは消去されました。

# SET EMAILの反応
NICK_SET_EMAIL_CHANGED
	電子メールのアドレスは %s に変更されました。

# SET SECUREの反応
NICK_SET_SECURE_SYNTAX
	SET SECURE {ON | OFF}
NICK_SET_SECURE_ON
	セキュアオプションは ON です。
NICK_SET_SECURE_OFF
	セキュアオプションは OFF です。

# SET PRIVATEの反応
NICK_SET_PRIVATE_SYNTAX
	SET PRIVATE {ON | OFF}
NICK_SET_PRIVATE_ON
	私的オプションは ON です。
NICK_SET_PRIVATE_OFF
	私的オプションは OFF です。

# SET HIDEの反応
NICK_SET_HIDE_SYNTAX
	SET HIDE {EMAIL | USERMASK | QUIT} {ON | OFF}
NICK_SET_HIDE_EMAIL_ON
	電子メールのアドレスは %s の INFO 表で表れません。
NICK_SET_HIDE_EMAIL_OFF
	電子メールのアドレスは %s の INFO 表で表れます。
NICK_SET_HIDE_MASK_ON
	前のユーザーアドレスは %s の INFO 表で表れません。
NICK_SET_HIDE_MASK_OFF
	前のユーザーアドレスは %s の INFO 表で表れます。
NICK_SET_HIDE_QUIT_ON
	前の切断メッセージは %s の INFO 表で表れません。
NICK_SET_HIDE_QUIT_OFF
	前の切断メッセージは %s の INFO 表で表れます。

# SET NOEXPIREの反応
NICK_SET_NOEXPIRE_SYNTAX
	SET [nickname] NOEXPIRE {ON | OFF}
NICK_SET_NOEXPIRE_ON
	%s というニックの登録は切れません。
NICK_SET_NOEXPIRE_OFF
	%s というニックの登録は切れます。

# ACCESSの反応
NICK_ACCESS_SYNTAX
	ACCESS {ADD | DEL | LIST} [mask]
NICK_ACCESS_REACHED_LIMIT
	ニックのアクセス表のマスク数は %d に限られていますので追加できません。
NICK_ACCESS_ALREADY_PRESENT
	%s はすでにアクセス表で存在しています。
NICK_ACCESS_ADDED
	%s はアクセス表に追加されました。
NICK_ACCESS_NOT_FOUND
	%s はアクセス表で存在しません。
NICK_ACCESS_DELETED
	%s はアクセス表から消去されました。
NICK_ACCESS_LIST
	アクセス表：
NICK_ACCESS_LIST_X
	%s のアクセス表：

# LINKの反応
NICK_LINK_SYNTAX
	LINK nick password
NICK_LINK_DISABLED
	LINK はただ今使用不可能です。
NICK_LINK_FAILED
	リンクは失敗しました。
NICK_NO_LINK_SAME
	同じニックにリンクできません。
NICK_LINKED
	ニックは %s にリンクされました。

# UNLINKの反応
NICK_UNLINK_SYNTAX
	LINK [nickname password]
NICK_UNLINK_FAILED
	アンリンクは失敗しました。
NICK_NOT_LINKED
	ニックはリンクされていません。
NICK_UNLINKED
	ニックは %s からアンリンクされました。
NICK_X_NOT_LINKED
	%s はリンクされていません。
NICK_X_UNLINKED
	%s は %s からアンリンクされました。

# LISTLINKSの反応
NICK_LISTLINKS_HEADER
	%s にリンクしているニック：
NICK_LISTLINKS_FOOTER
	%d 個見つかりました。

# INFOの反応
NICK_INFO_SYNTAX
	INFO nick
NICK_INFO_REALNAME
	%s は %s です
NICK_INFO_ADDRESS
	前のユーザーアドレス：%s
NICK_INFO_TIME_REGGED
	　　　　登録した日時：%s (%s ago)
NICK_INFO_LAST_SEEN
	　　　　前の使用日時：%s (%s ago)
NICK_INFO_LAST_QUIT
	　前の切断メッセージ：%s
NICK_INFO_URL
	　　　　ホームページ：%s
NICK_INFO_EMAIL
	電子メールのアドレス：%s
NICK_INFO_OPTIONS
	　　　　　オプション：%s
NICK_INFO_NO_EXPIRE
	このニックの登録は切れません。
# These strings MUST NOT be empty
NICK_INFO_OPT_SECURE
	セキュア
NICK_INFO_OPT_PRIVATE
	私的
NICK_INFO_OPT_NONE
	無し

# LISTの反応
NICK_LIST_SYNTAX
	LIST pattern
NICK_LIST_SERVADMIN_SYNTAX
	LIST pattern [FORBID] [NOEXPIRE]
NICK_LIST_HEADER
	%s に合うニック表：
NICK_LIST_RESULTS
	LIST 終了。

# RECOVERの反応
NICK_RECOVER_SYNTAX
	RECOVER nickname [password]
NICK_NO_RECOVER_SELF
	自分をリカバー出来ません。
NICK_RECOVERED
	ニックを使用していたユーザーは切断されました。
	1 分のタイムアウトが経つ前にニックを使用したい場合は、
	「/msg %s RELEASE %s」を入力して下さい。

# RELEASEの反応
#@@@ 「ホルド」
NICK_RELEASE_SYNTAX
	RELEASE nickname [password]
NICK_RELEASE_NOT_HELD
	%s はホルドされていません。
NICK_RELEASED
	%s は解放されました。

# GHOSTの反応
NICK_GHOST_SYNTAX
	GHOST nickname [password]
NICK_NO_GHOST_SELF
	自分をゴースト出来ません。
NICK_GHOST_KILLED
	%s のゴーストは切断されました。

# FORBIDの反応
NICK_FORBID_SYNTAX
	FORBID nickname
NICK_FORBID_SUCCEEDED
	%s というニックは使用禁止にされました。
NICK_FORBID_FAILED
	%s というニックを使用禁止にするのが失敗しました。

###########################################################################
#
# ChanServ messages
#
###########################################################################

# アクセスレベルの名状
CHAN_LEVEL_AUTOOP
	チャンネルに入ると +o モード
CHAN_LEVEL_AUTOVOICE
	チャンネルに入ると +v モード
CHAN_LEVEL_AUTODEOP
	+o モードを禁じる
CHAN_LEVEL_NOJOIN
	RESTRICTED であればチャンネルに入れない
CHAN_LEVEL_INVITE
	INVITE のコマンドが使える
CHAN_LEVEL_AKICK
	AKICK のコマンドが使える
CHAN_LEVEL_SET
	SET（FOUNDER・PASSWORD 以外）が使える
CHAN_LEVEL_CLEAR
	CLEAR のコマンドが使える
CHAN_LEVEL_UNBAN
	UNBAN のコマンドが使える
CHAN_LEVEL_OPDEOP
	OP・DEOP のコマンドが使える
CHAN_LEVEL_ACCESS_LIST
	アクセス表を見ることが出来る
CHAN_LEVEL_ACCESS_CHANGE
	アクセス表を変更できる
CHAN_LEVEL_MEMO_READ
	メモが読める

# 自動的なメッセージ
CHAN_IS_REGISTERED
	このチャンネルは登録されています。
CHAN_MAY_NOT_BE_USED
	このチャンネルは使用禁止です。
CHAN_NOT_ALLOWED_TO_JOIN
	このチャンネルには入れません。

# REGISTERの反応
CHAN_REGISTER_SYNTAX
	REGISTER channel password description
CHAN_REGISTER_NOT_LOCAL
	ロカルチャンネルの登録は出来ません。
CHAN_MUST_REGISTER_NICK
	チャンネルを登録するのには、ニックを登録する必要があります。ニック登録情報を見る場合、「/msg %s HELP」を入力して下さい。
CHAN_MUST_IDENTIFY_NICK
	チャンネルを登録するのには、ニックのパスワードを入力する必要があります。「/msg %s IDENTIFY password」を入力して下さい。
CHAN_MAY_NOT_BE_REGISTERED
	%s は使用禁止ですので、登録出来ません。
CHAN_ALREADY_REGISTERED
	%s は既に登録されています。
CHAN_MUST_BE_CHANOP
	チャンネルを登録するには、そのオペレターである必要があります。
CHAN_REACHED_CHANNEL_LIMIT
#@@@ こえる != reach
	%d チャンネルの限りを越えていますので登録できません。
CHAN_EXCEEDED_CHANNEL_LIMIT
	%d チャンネルの限りを越えていますので登録できません。
CHAN_REGISTRATION_FAILED
	登録は失敗しました。
CHAN_REGISTERED
	%s は登録されました。
CHAN_PASSWORD_IS
	チャンネルのパスワードは %s です。覚えておいて下さい。

# IDENTIFYの反応
CHAN_IDENTIFY_SYNTAX
	IDENTIFY channel password
CHAN_IDENTIFY_SUCCEEDED
#@@@
	正しいパスワードです。これから %s でファウンダーの特権を持っています。

# DROPの反応
CHAN_DROP_SYNTAX
	DROP channel
CHAN_DROPPED
	%s というチャンネルの登録は消去されました。

# SETの反応
CHAN_SET_SYNTAX
	SET channel option parameters
CHAN_SET_UNKNOWN_OPTION
	%s というオプションは分かりません。

# SET FOUNDERの反応
CHAN_SET_FOUNDER_TOO_MANY_CHANS
	%s はチャンネルの数の限りを越えていますのでファウンダーを変更出来ません。
CHAN_FOUNDER_CHANGED
	%s のファウンダーは %s に変更されました。

# SET SUCCESSORの反応
CHAN_SUCCESSOR_CHANGED
	%s の次期ファウンダーは %s に変更されました。
CHAN_SUCCESSOR_UNSET
#@@@
	%s の次期ファウンダー設定はキャンセルされました。

# SET PASSWORDの反応
CHAN_PASSWORD_CHANGED
	%s のパスワードは変更されました。
CHAN_PASSWORD_CHANGED_TO
	%s のパスワードは %s に変更されました。

# SET DESCの反応
CHAN_DESC_CHANGED
	%s の名状は「%s」に変更されました。

# SET URLの反応
CHAN_URL_CHANGED
	%s のホームページのアドレスは %s に変更されました。
CHAN_URL_UNSET
	%s のホームページのアドレスは消去されました。

# SET EMAILの反応
CHAN_EMAIL_CHANGED
	%s の電子メールのアドレスは %s に変更されました。
CHAN_EMAIL_UNSET
	%s の電子メールのアドレスは消去されました。

# SET ENTRYMSGの反応
CHAN_ENTRY_MSG_CHANGED
	%s の入メッセージは変更されました。
CHAN_ENTRY_MSG_UNSET
	%s の入メッセージは消去されました。

# SET MLOCKの反応
CHAN_SET_MLOCK_KEY_REQUIRED
	モードロックを +k にするにはパラメーターが必要です。
CHAN_SET_MLOCK_LIMIT_REQUIRED
	モードロックを +l にするにはパラメーターが必要です。
CHAN_SET_MLOCK_LIMIT_POSITIVE
	モードロックを +l にするには正数のパラメーターが必要です。
CHAN_SET_MLOCK_UNKNOWN_CHAR
	%c というモードは間違っていますので無視されました。
CHAN_MLOCK_CHANGED
	%s のモードロックは %s に変更されました。
CHAN_MLOCK_REMOVED
	%s のモードロックは消去されました。

# SET KEEPTOPICの反応
CHAN_SET_KEEPTOPIC_SYNTAX
	SET channel KEEPTOPIC {ON | OFF}
CHAN_SET_KEEPTOPIC_ON
	トピック記録オプションは ON です。
CHAN_SET_KEEPTOPIC_OFF
	トピック記録オプションは OFF です。

# SET TOPICLOCKの反応
CHAN_SET_TOPICLOCK_SYNTAX
	SET channel TOPICLOCK {ON | OFF}
CHAN_SET_TOPICLOCK_ON
	トピックロックオプションは ON です。
CHAN_SET_TOPICLOCK_OFF
	トピックロックオプションは OFF です。

# SET PRIVATEの反応
CHAN_SET_PRIVATE_SYNTAX
	SET channel PRIVATE {ON | OFF}
CHAN_SET_PRIVATE_ON
	私的オプションは ON です。
CHAN_SET_PRIVATE_OFF
	私的オプションは OFF です。

# SET SECUREOPSの反応
CHAN_SET_SECUREOPS_SYNTAX
	SET channel SECUREOPS {ON | OFF}
CHAN_SET_SECUREOPS_ON
	+o モード制御オプションは ON です。
CHAN_SET_SECUREOPS_OFF
	+o モード制御オプションは OFF です。

# SET LEAVEOPSの反応
CHAN_SET_LEAVEOPS_SYNTAX
	SET channel LEAVEOPS {ON | OFF}
#@@@
CHAN_SET_LEAVEOPS_ON
	Leave ops option is now ON.
CHAN_SET_LEAVEOPS_OFF
	Leave ops option is now OFF.

# SET RESTRICTEDの反応
CHAN_SET_RESTRICTED_SYNTAX
	SET channel RESTRICTED {ON | OFF}
CHAN_SET_RESTRICTED_ON
	アクセス制御オプションは ON です。
CHAN_SET_RESTRICTED_OFF
	アクセス制御オプションは OFF です。

# SET SECUREの反応
CHAN_SET_SECURE_SYNTAX
	SET channel SECURE {ON | OFF}
CHAN_SET_SECURE_ON
	セキュアオプションは ON です。
CHAN_SET_SECURE_OFF
	セキュアオプションは OFF です。

# SET NOEXPIREの反応
CHAN_SET_NOEXPIRE_SYNTAX
	SET channel NOEXPIRE {ON | OFF}
CHAN_SET_NOEXPIRE_ON
	%s というチャンネルの登録は切れません。
CHAN_SET_NOEXPIRE_OFF
	%s というチャンネルの登録は切れます。

# ACCESS messages
CHAN_ACCESS_SYNTAX
	ACCESS channel {ADD|DEL|LIST} [nick [level] | entry-list]
CHAN_ACCESS_LEVEL_NONZERO
	アクセスレベルはゼロであってはなりません。
CHAN_ACCESS_LEVEL_RANGE
	アクセスレベルは、%d 以上で、%d 以下の数にして下さい。
CHAN_ACCESS_NICKS_ONLY
	チャンネルのアクセス表に追加出来るのは登録されたニックのみです。
CHAN_ACCESS_REACHED_LIMIT
	チャンネルのアクセス表のニック数は %d に限られていますので追加できません。
CHAN_ACCESS_LEVEL_UNCHANGED
	%s の %s でのアクセスレベルは %d から変更されませんでした。
CHAN_ACCESS_LEVEL_CHANGED
	%s の %s でのアクセスレベルは %d に変更されました。
CHAN_ACCESS_ADDED
	%s は %s のアクセス表に、レベル %d で追加されました。
CHAN_ACCESS_NO_SUCH_ENTRY
	%d 番のエントリーは %s のアクセス表に存在しません。
CHAN_ACCESS_NO_MATCH
	%s に合うエントリーは %s のアクセス表に存在しません。
CHAN_ACCESS_NOT_FOUND
	%s は %s のアクセス表に存在しません。
CHAN_ACCESS_DELETED
	%s は %s のアクセス表から消去されました。
CHAN_ACCESS_DELETED_ONE
	%s のアクセス表からエントリーを 1 消去しました。
CHAN_ACCESS_DELETED_SEVERAL
	%s のアクセス表からエントリーを %d 消去しました。
CHAN_ACCESS_LIST_EMPTY
	%s のアクセス表は空です。
CHAN_ACCESS_LIST_HEADER
	%s のアクセス表：
	 番号  レベル   ニック
CHAN_ACCESS_LIST_FORMAT
	%5d    %4d   %s%s%s%s

# AKICKの反応
CHAN_AKICK_SYNTAX
	AKICK channel {ADD | DEL | LIST} [nick-or-usermask]
CHAN_AKICK_ALREADY_EXISTS
	%s は既に %s の自動蹴表で存在しています。
CHAN_AKICK_REACHED_LIMIT
	自動蹴表のマスクの数は %d に限っていますので追加できません。
CHAN_AKICK_ADDED
	%s は %s の自動蹴表に追加されました。
CHAN_AKICK_NO_MATCH
	%s に合うエントリーは %s の自動蹴表で存在しません。
CHAN_AKICK_DELETED
	%s は %s の自動蹴表から消去されました。
CHAN_AKICK_DELETED_ONE
	%s の自動蹴表からエントリーを 1 消去しました。
CHAN_AKICK_DELETED_SEVERAL
	%s の自動蹴表からエントリーを %d 消去しました。
CHAN_AKICK_LIST_EMPTY
	%s の自動蹴表は空です。
CHAN_AKICK_LIST_HEADER
	%sの自動蹴表：
CHAN_AKICK_LIST_FORMAT
	  %3d %s%s

# LEVELSの反応
CHAN_LEVELS_SYNTAX
	LEVELS channel {SET | DIS[ABLE] | LIST | RESET} [item [level]]
CHAN_LEVELS_RANGE
	レベルは、%d 以上で、%d 以下の数を入力して下さい。
CHAN_LEVELS_CHANGED
	%s のレベルを %s で %d に変更されました。
CHAN_LEVELS_UNKNOWN
	%s というレベルは分かりません。レベル表を見る場合は、「/msg %s HELP LEVELS DESC」を入力して下さい。
CHAN_LEVELS_LIST_HEADER
	%s のレベル表：
CHAN_LEVELS_LIST_DISABLED
	    %-*s  (不動)
CHAN_LEVELS_LIST_NORMAL
	    %-*s  %d
CHAN_LEVELS_RESET
	%s のレベル表は既定値に戻されました。

# INFOの反応
CHAN_INFO_SYNTAX
	INFO channel
CHAN_INFO_HEADER
	%sの情報：
CHAN_INFO_FOUNDER
	　　　　ファウンダー：%s (%s)
CHAN_INFO_NO_FOUNDER
	　　　　ファウンダー：%s
CHAN_INFO_DESCRIPTION
	　　　　　　　　名状：%s
CHAN_INFO_TIME_REGGED
	　　　　登録した日時：%s
CHAN_INFO_LAST_USED
	　前の使用された日時：%s
CHAN_INFO_LAST_TOPIC
	　　　　前のトピック：%s
CHAN_INFO_TOPIC_SET_BY
	　　トピックの設定者：%s
CHAN_INFO_URL
	　　　　ホームページ：%s
CHAN_INFO_EMAIL
	電子メールのアドレス：%s
CHAN_INFO_OPTIONS
	　　　　　オプション：%s
CHAN_INFO_OPT_PRIVATE
	私的
CHAN_INFO_OPT_KEEPTOPIC
	トピック記録
CHAN_INFO_OPT_TOPICLOCK
	トピックロック
CHAN_INFO_OPT_SECUREOPS
	+o モード制御
CHAN_INFO_OPT_LEAVEOPS
#@@@
CHAN_INFO_OPT_RESTRICTED
	アクセス制御
CHAN_INFO_OPT_SECURE
	セキュア
CHAN_INFO_OPT_NONE
	無し
CHAN_INFO_MODE_LOCK
	　　　　モードロック：%s

# LISTの反応
CHAN_LIST_SYNTAX
	LIST pattern
CHAN_LIST_HEADER
	%s に合うチャンネル表：
CHAN_LIST_END
	LIST 終了。

# INVITEの反応
CHAN_INVITE_SYNTAX
	INVITE channel

# OPの反応
CHAN_OP_SYNTAX
	OP channel nick

# DEOPの反応
CHAN_DEOP_SYNTAX
	DEOP channel nick

# UNBANの反応
CHAN_UNBAN_SYNTAX
	UNBAN channel
CHAN_UNBANNED
	%s での追放マスクは解かれました。

# CLEARの反応
CHAN_CLEAR_SYNTAX
	CLEAR channel what
CHAN_CLEARED_BANS
	%s の追放マスクは全て消去されました。
CHAN_CLEARED_MODES
	%s のモードは既定値に戻されました。
CHAN_CLEARED_OPS
	+o モードは %s から消去されました。
CHAN_CLEARED_VOICES
	+v モードは %s から消去されました。
CHAN_CLEARED_USERS
	%s のユーザーは全て蹴り出されました。

# FORBIDの反応
CHAN_FORBID_SYNTAX
	FORBID channel
CHAN_FORBID_SUCCEEDED
	%s は使用禁止にされました。
CHAN_FORBID_FAILED
	%s を使用禁止にするのが失敗しました。

###########################################################################
#
# MemoServのメッセージ
#
###########################################################################

# 自動的なメッセージ
MEMO_HAVE_NEW_MEMO
	新メモが1あります。
MEMO_HAVE_NEW_MEMOS
	新メモが%dあります。
MEMO_TYPE_READ_LAST
	読むのには「/msg %s READ LAST」を入力して下さい。
MEMO_TYPE_READ_NUM
	読むのには「/msg %s READ %d」を入力して下さい。
MEMO_TYPE_LIST_NEW
	表すのには「/msg %s LIST NEW」を入力して下さい。
MEMO_AT_LIMIT
	注意：メモの数の限り（%d）になりました。メモを一つ以上消去するまで新たなメモは届きません。
MEMO_OVER_LIMIT
	注意：メモの数の限り（%d）を過ぎました。メモを消去するまで新たなメモは届きません。
MEMO_NEW_MEMO_ARRIVED
	%sからメモが届きました。
	読むのには「/msg %s READ %d」を入力して下さい。

# 多用のメッセージ
MEMO_HAVE_NO_MEMOS
	メモがありません。
MEMO_X_HAS_NO_MEMOS
	%s はメモがありません。

# SENDの反応
MEMO_SEND_SYNTAX
	SEND {nick | channel} memo-text
MEMO_SEND_PLEASE_WAIT
	SEND のコマンドを再入力する前に %d 秒お待ち下さい。
MEMO_X_HAS_TOO_MANY_MEMOS
	%s は現在メモが多すぎますので新メモを受信できません。
MEMO_X_GETS_NO_MEMOS
	%s はメモを受信できません。
MEMO_SENT
	%s にメモを送信ました。

# LISTの反応
MEMO_LIST_SYNTAX
	LIST [channel] [list | NEW]
MEMO_HAVE_NO_NEW_MEMOS
	新メモがありません。
MEMO_X_HAS_NO_NEW_MEMOS
	%s は新メモがありません。
MEMO_LIST_MEMOS
	%s のメモ表。読むには「/msg %s READ num」
MEMO_LIST_NEW_MEMOS
	%s の新メモ表。読むには「/msg %s READ num」
MEMO_LIST_CHAN_MEMOS
	%s のメモ表。読むには「/msg %s READ %s num」
MEMO_LIST_CHAN_NEW_MEMOS
	%s の新メモ表。読むには「/msg %s READ %s num」
MEMO_LIST_HEADER
	 番号   差出人            日時
MEMO_LIST_FORMAT
	%c%4d   %-16s  %s

# READの反応
MEMO_READ_SYNTAX
	READ [channel] {list | LAST | NEW}
MEMO_HEADER
	%d 番のメモ（%s から %s に送信された）。消すには「/msg %s DEL %d」
MEMO_CHAN_HEADER
	%d 番のメモ（%s から %s に送信された）。消すには「/msg %s DEL %s %d」
MEMO_TEXT
	%s

# DELの反応
MEMO_DEL_SYNTAX
	DEL [channel] {num | list | ALL}
MEMO_DELETED_NONE
	入力した番号に合うメモはありません。

# SETの反応
MEMO_SET_SYNTAX
	SET option parameters
MEMO_SET_UNKNOWN_OPTION
	%s というオプションは分かりません。

# SET NOTIFYの反応
MEMO_SET_NOTIFY_SYNTAX
	SET NOTIFY {ON | LOGON | NEW | OFF}
MEMO_SET_NOTIFY_ON
	%s はこれからログオンする時に新メモがあれば報告し、メモが届く時にも報告します。
MEMO_SET_NOTIFY_LOGON
	%s はこれからログオンする時とIRCに戻る時に新メモがあれば報告します。
MEMO_SET_NOTIFY_NEW
	%s はこれからメモが届く時に報告します。
MEMO_SET_NOTIFY_OFF
	%s はこれからメモの報告をしません。

# SET LIMITの反応
MEMO_SET_LIMIT_SYNTAX
	SET LIMIT [channel] limit
MEMO_SET_LIMIT_SERVADMIN_SYNTAX
	SET LIMIT [user | channel] {limit | NONE} [HARD]
MEMO_SET_YOUR_LIMIT_FORBIDDEN
	メモの数の限りを変更できません。
MEMO_SET_LIMIT_FORBIDDEN
	%s のメモの数の限りを変更できません。
MEMO_SET_YOUR_LIMIT_TOO_HIGH
	メモの数の限りは、ゼロか %d 以下の正数にしか設定できません。
MEMO_SET_LIMIT_TOO_HIGH
	%s のメモの数の限りは、ゼロか %d 以下の正数にしか設定できません。
MEMO_SET_LIMIT_OVERFLOW
	限りの数値は大きすぎますので、%d になります。
MEMO_SET_YOUR_LIMIT
	メモの数の限りは %d に設定されました。
MEMO_SET_YOUR_LIMIT_ZERO
	これからメモを受信しません。
MEMO_UNSET_YOUR_LIMIT
	メモの数の限りは不動にされました。
MEMO_SET_LIMIT
	%s のメモの数の限りは %d に設定されました。
MEMO_SET_LIMIT_ZERO
	%s のメモの数の限りは 0 に設定されました。
MEMO_UNSET_LIMIT
	%s のメモの数の限りは不動にされました。

# INFOの反応
MEMO_INFO_NO_MEMOS
	メモはありません。
MEMO_INFO_MEMO
	メモが 1 あります。
MEMO_INFO_MEMO_UNREAD
	メモが 1 あり、まだ読んでいません。
MEMO_INFO_MEMOS
	メモが %d あります。
MEMO_INFO_MEMOS_ONE_UNREAD
	メモが %d あり、1 は読んでいません。
MEMO_INFO_MEMOS_SOME_UNREAD
	メモが %d あり、%d は読んでいません。
MEMO_INFO_MEMOS_ALL_UNREAD
	メモが %d あり、全ては読んでいません。
MEMO_INFO_LIMIT
	メモの数の限りは %d です。
MEMO_INFO_HARD_LIMIT
	メモの数の限りは %d で、その限りは変更できません。
MEMO_INFO_LIMIT_ZERO
	メモの数の限りは 0 ですので、メモを受けられません。
MEMO_INFO_HARD_LIMIT_ZERO
	メモの数の限りは 0 ですので、メモを受けられません。その限りは変更できません。
MEMO_INFO_NO_LIMIT
	メモの数は限られていません。
MEMO_INFO_X_NO_MEMOS
	%s はメモがありません。
MEMO_INFO_X_MEMO
	%s はメモが 1 あります。
MEMO_INFO_X_MEMO_UNREAD
	%s はメモが 1 あり、まだ読んでいません。
MEMO_INFO_X_MEMOS
	%s はメモが %d あります。
MEMO_INFO_X_MEMOS_ONE_UNREAD
	%s はメモが %d あり、1 は読んでいません。
MEMO_INFO_X_MEMOS_SOME_UNREAD
	%s はメモが %d あり、%d は読んでいません。
MEMO_INFO_X_MEMOS_ALL_UNREAD
	%s はメモが %d あり、全ては読んでいません。
MEMO_INFO_X_LIMIT
	%s のメモの数の限りは %d です。
MEMO_INFO_X_HARD_LIMIT
	%s のメモの数の限りは %d で、限りを変更できません。
MEMO_INFO_X_NO_LIMIT
	%s のメモの数は限られていません。

###########################################################################
#
# OperServのメッセージ
#
###########################################################################

# 多用のメッセージ
OPER_BOUNCY_MODES
	Servicesはモードを変更できません。サーバーの設定をチェックして下さい。
OPER_BOUNCY_MODES_U_LINE
	Servicesはモードを変更できません。サーバーの「U:ライン」の設定をチェックして下さい。

# GLOBALの反応
OPER_GLOBAL_SYNTAX
	GLOBAL message

# STATSの反応
OPER_STATS_UNKNOWN_OPTION
	%s というSTATSのオプションは分かりません。
OPER_STATS_CURRENT_USERS
	現在ユーザー数：%d
	オペレーター数：%d
OPER_STATS_MAX_USERS
	最大ユーザー数：%d（%s）
OPER_STATS_UPTIME_DHMS
	起動から　　　：%d日%02d時間%02d分
OPER_STATS_UPTIME_1DHMS
	起動から　　　：%d日%02d時間%02d分
OPER_STATS_UPTIME_HMS
	起動から　　　：%02d時間%02d分
OPER_STATS_UPTIME_HM1S
	起動から　　　：%02d時間%02d分
OPER_STATS_UPTIME_H1MS
	起動から　　　：%02d時間%02d分
OPER_STATS_UPTIME_H1M1S
	起動から　　　：%02d時間%02d分
OPER_STATS_UPTIME_1HMS
	起動から　　　：%02d時間%02d分
OPER_STATS_UPTIME_1HM1S
	起動から　　　：%02d時間%02d分
OPER_STATS_UPTIME_1H1MS
	起動から　　　：%02d時間%02d分
OPER_STATS_UPTIME_1H1M1S
	起動から　　　：%02d時間%02d分
OPER_STATS_UPTIME_MS
	起動から　　　：%02d分%02d秒
OPER_STATS_UPTIME_M1S
	起動から　　　：%02d分%02d秒
OPER_STATS_UPTIME_1MS
	起動から　　　：%02d分%02d秒
OPER_STATS_UPTIME_1M1S
	起動から　　　：%02d分%02d秒
OPER_STATS_USER_MEM
	ユーザー　：%6dレコード・%5dキロ
OPER_STATS_CHANNEL_MEM
	チャンネル：%6dレコード・%5dキロ
OPER_STATS_NICKSERV_MEM
	NickServ　：%6dレコード・%5dキロ
OPER_STATS_CHANSERV_MEM
	ChanServ　：%6dレコード・%5dキロ
OPER_STATS_OPERSERV_MEM
	OperServ　：%6dレコード・%5dキロ
OPER_STATS_AKILL_COUNT
	自動切断マスクの数：%d
OPER_STATS_AKILL_EXPIRE_DAYS
	自動切断マスクの既定寿命：%d日
OPER_STATS_AKILL_EXPIRE_DAY
	自動切断マスクの既定寿命：%d日
OPER_STATS_AKILL_EXPIRE_HOURS
	自動切断マスクの既定寿命：%d時間
OPER_STATS_AKILL_EXPIRE_HOUR
	自動切断マスクの既定寿命：%d時間
OPER_STATS_AKILL_EXPIRE_MINS
	自動切断マスクの既定寿命：%d分
OPER_STATS_AKILL_EXPIRE_MIN
	自動切断マスクの既定寿命：%d分
OPER_STATS_AKILL_EXPIRE_NONE
	自動切断マスクの既定寿命：無し

# MODEの反応
OPER_MODE_SYNTAX
	MODE channel modes

# CLEARMODESの反応
OPER_CLEARMODES_SYNTAX
	CLEARMODES channel [ALL]
OPER_CLEARMODES_DONE
	%s の入切モードと追放マスクはクリアされました。
OPER_CLEARMODES_ALL_DONE
	%s のモードは全てクリアされました。

# KICKの反応
OPER_KICK_SYNTAX
	KICK channel user reason

# ADMINの反応
OPER_ADMIN_SYNTAX
	ADMIN {ADD | DEL | LIST} [nick]
OPER_ADMIN_ADD_SYNTAX
	ADMIN ADD nick
OPER_ADMIN_DEL_SYNTAX
	ADMIN ADD nick
OPER_ADMIN_EXISTS
	%s は既に Services のアドミン表で存在しています。
OPER_ADMIN_ADDED
	%s は Services のアドミン表に追加されました。
OPER_ADMIN_TOO_MANY
	Services のアドミン表は満杯ですので追加出来ません。
OPER_ADMIN_REMOVED
	%s は Services のアドミン表から消去されました。
OPER_ADMIN_NOT_FOUND
	%s は Services のアドミン表で存在しません。
OPER_ADMIN_LIST_HEADER
	Services のアドミン表：

# OPERの反応
OPER_OPER_SYNTAX
	OPER {ADD | DEL | LIST} [nick]
OPER_OPER_ADD_SYNTAX
	OPER ADD nick
OPER_OPER_DEL_SYNTAX
	OPER ADD nick
OPER_OPER_EXISTS
	%s は既に Services のオペレーター表で存在しています。
OPER_OPER_ADDED
	%s は Services のオペレーター表に追加されました。
OPER_OPER_TOO_MANY
	Services のオペレーター表は満杯ですので追加出来ません。
OPER_OPER_REMOVED
	%s は Services のオペレーター表から消去されました。
OPER_OPER_NOT_FOUND
	%s は Services のオペレーター表で存在しません。
OPER_OPER_LIST_HEADER
	Services のオペレーター表：

# AKILLの反応
OPER_AKILL_SYNTAX
	AKILL {ADD | DEL | LIST | VIEW} [[+expiry] mask [reason]]
OPER_AKILL_ADD_SYNTAX
	AKILL ADD [+expiry] mask reason
OPER_AKILL_DEL_SYNTAX
	AKILL DEL mask
OPER_AKILL_NO_NICK
	注意：自動切断マスクはユーザー名とホスト名をだけ含みますので、ニックを入力しなかったように確認して下さい。
OPER_TOO_MANY_AKILLS
	自動切断表は満杯ですので追加できません。
OPER_AKILL_REMOVED
	%s は自動切断表から消去されました。
OPER_AKILL_NOT_FOUND
	%s は自動切断表で存在しません。
OPER_AKILL_LIST_HEADER
	自動切断表：
# mask, reason
OPER_AKILL_LIST_FORMAT
	%-32s  %s
# mask, set-by, set-time, expires, reason
OPER_AKILL_VIEW_FORMAT
	%s (%s に %s に追加され、%s)
	    %s
OPER_AKILL_NO_EXPIRE
	切れない
OPER_AKILL_EXPIRES_SOON
	次のデータ保存ときに切れる
OPER_AKILL_EXPIRES_M
	%d分後切れる
OPER_AKILL_EXPIRES_1M
	%d分後切れる
OPER_AKILL_EXPIRES_HM
	%d時%d分後切れる
OPER_AKILL_EXPIRES_H1M
	%d時%d分後切れる
OPER_AKILL_EXPIRES_1HM
	%d時%d分後切れる
OPER_AKILL_EXPIRES_1H1M
	%d時%d分後切れる
OPER_AKILL_EXPIRES_D
	%d日後切れる
OPER_AKILL_EXPIRES_1D
	%d日後切れる

# SETの反応
OPER_SET_SYNTAX
	SET option setting
OPER_SET_IGNORE_ON
	イグノアコードは作動します。
OPER_SET_IGNORE_OFF
	イグノアコードは作動しません。
OPER_SET_IGNORE_ERROR
	IGNORE のパラメーターは ON か OFF にして下さい。
OPER_SET_DEBUG_ON
	Services はこれからデバッグモードで動きます。
OPER_SET_DEBUG_OFF
	Services はこれから通常モードで動きます。
OPER_SET_DEBUG_LEVEL
	Services はこれからデバーグモードで動きます。
OPER_SET_DEBUG_ERROR
	DEBUG のパラメーターは、ON か OFF か正数にして下さい。
OPER_SET_UNKNOWN_OPTION
	%s というオプションは分かりません。

# JUPEの反応
OPER_JUPE_SYNTAX
	JUPE servername [reason]

# RAWの反応
OPER_RAW_SYNTAX
	RAW text

# RESTARTの反応
OPER_CANNOT_RESTART
	SERVICES_BIN は設定されていませんので再起動できません。
	RESTART のコマンドを使用可能にするのには、もう一回 configure
	を実行し、Services を再コンパイルして下さい。

# IGNOREの反応
OPER_IGNORE_LIST
	Servicesのイグノア表：
OPER_IGNORE_LIST_EMPTY
	Servicesのイグノア表は空です。

###########################################################################
#
# 案内システムのメッセージ
#
###########################################################################

NEWS_LOGON_TEXT
	（ログオン案内  %s） %s
NEWS_OPER_TEXT
	（オペレーター案内  %s） %s

NEWS_LOGON_SYNTAX
	Syntax: LOGONNEWS {ADD|DEL|LIST} [text|num]
NEWS_LOGON_ADD_SYNTAX
	Syntax: LOGONNEWS ADD text
NEWS_LOGON_DEL_SYNTAX
	Syntax: LOGONNEWS DEL {num | ALL}
NEWS_LOGON_LIST_HEADER
	ログオン案内表：
NEWS_LOGON_LIST_ENTRY
	%5d (%s  %s)
	    %s
NEWS_LOGON_LIST_NONE
	ログオン案内件はありません。
NEWS_LOGON_ADD_FULL
	ログオン案内表は満杯です。
NEWS_LOGON_ADDED
	新ログオン案内件（%d番）を追加しました。
NEWS_LOGON_DEL_NOT_FOUND
	%d 番目のログオン案内件は存在しません。
NEWS_LOGON_DELETED
	%d 番目のログオン案内件は消去されました。
NEWS_LOGON_DEL_NONE
	ログオン案内件はありません。
NEWS_LOGON_DELETED_ALL
	ログオン案内件は全て消去されました。

NEWS_OPER_SYNTAX
	Syntax: OPERNEWS {ADD|DEL|LIST} [text|num]
NEWS_OPER_ADD_SYNTAX
	Syntax: OPERNEWS ADD text
NEWS_OPER_DEL_SYNTAX
	Syntax: OPERNEWS DEL {num | ALL}
NEWS_OPER_LIST_HEADER
	オペレーター案内表：
NEWS_OPER_LIST_ENTRY
	%5d (%s  %s)
	    %s
NEWS_OPER_LIST_NONE
	オペレーター案内件はありません。
NEWS_OPER_ADD_FULL
	オペレーター案内表は満杯です。
NEWS_OPER_ADDED
	新オペレーター案内件（%d番）を追加しました。
NEWS_OPER_DEL_NOT_FOUND
	%d 番目のオペレーター案内件は存在しません。
NEWS_OPER_DELETED
	%d 番目のオペレーター案内件は消去されました。
NEWS_OPER_DEL_NONE
	オペレーター案内件はありません。
NEWS_OPER_DELETED_ALL
	オペレーター案内件は全て消去されました。

NEWS_HELP_LOGON
	シンタクス：LOGONNEWS ADD text
	　　　　　　LOGONNEWS DEL {num | ALL}
	　　　　　　LOGONNEWS LIST
	
	ログオン案内表の編集と表示のコマンドです。ユーザーはネット
	ワークに接続と、Services はこのメッセージをユーザーに送りま
	す。ただし、メッセージが４つ以上の場合、ユーザーに大量のメッ
	セージを送らないように最新の３つだけを送ります。
	
	LOGONNEWS LIST という形で、IRC オペレーターはログオン案内表
	を表すことが出来ますが、ADD・DEL の編集コマンドを使うのは
	Services のアドミンのみです。

NEWS_HELP_OPER
	シンタクス：OPERNEWS ADD text
	　　　　　　OPERNEWS DEL {num | ALL}
	　　　　　　OPERNEWS LIST
	
	オペレーター案内表の編集と表示のコマンドです。ユーザーが
	IRC オペレーターになると、Services はこのメッセージをユー
	ザーに送ります。ただし、メッセージが４つ以上の場合、ユー
	ザーに大量のメッセージを送らないように最新の３つだけを送り
	ます。
	
	OPERNEWS LIST という形で、IRC オペレーターはログオン案内表
	を表すことが出来ますが、ADD・DEL の編集コマンドを使うのは
	Services のアドミンのみです。

###########################################################################
#
# NickServのヘルプメッセージ
#
###########################################################################

NICK_HELP
	%S は、ニックの登録サービスです。このサービスを使い
	ニックを登録すれば、他のユーザーはそのニックを使えなくなり
	ます。ただし、ニックを%d日間使わないと登録は切れてしまいま
	す。次のコマンドでニックを登録やオプションの変更が出来ます。
	コマンドを使う場合は「/msg %S command」を入力して下
	さい。コマンドのヘルプを読む場合は、「/msg %S HELP
	command」を入力して下さい。
	
	    REGISTER   ニックの登録
#@@@ 「identify」
	    IDENTIFY   パスワードでidentifyする
	    ACCESS     アクセス表の変更
	    LINK       他のニックにリンクを作る
	    SET        オプションの設定
	    DROP       登録の消去
	    RECOVER    ニックを使用しているユーザーを切断する
	    RELEASE    RECOVER を使ってからニックを使用可能にする
	
	他のコマンド：UNLINK・GHOST・INFO・LIST・STATUS・AUTOJOIN
	
	警告：このサービスは、ユーザーの身元を守る為のサービスです。
	ニックの「盗み」等の悪用のためのサービスではありませんので、
	そのような行為はおやめ下さい。

NICK_HELP_EXPIRE_ZERO
	%S は、ニックの登録サービスです。このサービスを使い
	ニックを登録すれば、他のユーザーはそのニックを使えなくなり
	ます。次のコマンドでニックを登録やオプションの変更が出来ま
	す。コマンドを使う場合は「/msg %S command」を入力して
	下さい。コマンドのヘルプを読む場合は、「/msg %S HELP
	command」を入力して下さい。
	
	    REGISTER   ニックの登録
#@@@ 「identify」
	    IDENTIFY   パスワードでidentifyする
	    ACCESS     アクセス表の変更
	    LINK       他のニックにリンクを作る
	    SET        オプションの設定
	    DROP       登録の消去
	    RECOVER    ニックを使用しているユーザーを切断する
	    RELEASE    RECOVER を使ってからニックを使用可能にする
	
	他のコマンド：UNLINK・GHOST・INFO・LIST・STATUS
	
	警告：このサービスは、ユーザーの身元を守る為のサービスです。
	ニックの「盗み」等の悪用のためのサービスではありませんので、
	そのような行為はおやめ下さい。

NICK_HELP_REGISTER
	シンタクス：REGISTER password
	
	現在使用しているニックを%Sのデータベースに登録します。
	ニックを登録してから、SET・ACCESS等のコマンドでニックのオ
	プションの変更が出来ます。ただし、設定を変更するのにはパス
	ワードが必要ですので、登録するときに選んだパスワードをよく
	覚えておいて下さい。また、ニックやチャンネルと違って、パス
	ワードで大文字と小文字は違います。例えば、FIDO・Fido・fido
	等は全て違います。
	
	パスワードを選ぶときは、不明な方のパスワードにして下さい。
	お名前・ニック等のような当てやすいパスワードはお止め下さい。
	４文字以下のパスワードは何でも当てやすい方ですので、５文字
	上のパスワード以上にして下さい。なお、パスワードでスペース
	を使えません。一方、日本語（仮名と漢字）でパスワードを入力
	することも出来ます。

NICK_HELP_IDENTIFY
	シンタクス：IDENTIFY password
	
#@@@  「証拠」など・ChanServのもある
	%Sにニックの保有者である証拠を示します。このコマンド
	を使わなければ、ほとんどのServicesのコマンドは使えません。
	passwordはREGISTERのコマンドを入力したときのと同じパスワー
	ドです。

NICK_HELP_DROP
	シンタクス：DROP
	
	%Sのデータベースからニックを消去します。消去したニッ
	クは、誰でも再登録が出来ます。
	
	このコマンドを使うのには、パスワードを入力する必要がありま
	す。パスワードの入力のヘルプを見るのには、
	「/msg %S HELP IDENTIFY」を入力して下さい。

NICK_HELP_ACCESS
	シンタクス：ACCESS ADD mask
	　　　　　　ACCESS DEL mask
	　　　　　　ACCESS LIST
	
	ニックのアクセス表の変更と表示のコマンドです。アクセス表と
	は、保有者のユーザーアドレス（user@hostというアドレス）のリ
	ストです。ユーザーのアドレスはアクセス表のマスクの一つに合
	えば、%Sに切断されません。また、SECUREというオプションをOFF
	にすればパスワードを入力せずにServicesのコマンドを使えます。
	
	例：
	    ACCESS ADD achurch@*.dragonfire.net
	        dragonfire.netというドメーンのホストからachurchとい
	        うユーザーをアクセス表に追加します。
	
	    ACCESS DEL achurch@*.dragonfire.net
	        前の例と逆の結果です。
	
	    ACCESS LIST
	        アクセス表を表します。

NICK_HELP_LINK
	シンタクス：LINK nick password
	
	nickというニックにリンクを作り、現在のニックを入力したニッ
	クの仮名にします。このコマンドを使うと、現在のニックのアク
	セス表は消え、メモは入力したニックのメモ表に追加されます
	（それで入力したニックのメモ表がメモの数の限りを過ぎること
	もありますのでご注意下さい）。
	
	リンクが作られてから、現在のニックはServicesのデータベース
	で入力したニックに変化します。%SのUNLINKとDROPのコマ
	ンド以外、全てのServicesのコマンドは現在のニックの代わりに
	リンクされたニックを使います。例えば、リンクされたニックの
	メモを見たい場合は、どちらのニックでも見られます。そして、
	他のユーザーからメモを受ける場合は、そのユーザーはどちらの
	ニックに送っても同じメモ表に追加されます。また、チャンネル
	のアクセス表でどちらかのニックが存在していれば、どちらの
	ニックでも同じ特権を得ます。
	
	このコマンドを使うのには、パスワードを入力する必要がありま
	す。なお、リンクしたいニックのパスワードをこのコマンドのパ
	ラメーターとして入力する必要があります。

NICK_HELP_UNLINK
	シンタクス：UNLINK [nick password]
	
	ニックのリンクを消去します。このコマンドを使うと、リンクさ
	れていたニックのアクセス表とオプションの設定は現在のニック
	にコピーされますが、メモはコピーされません。
	
	このコマンドを使うのには、パスワードを入力する必要がありま
	す。パスワードの入力のヘルプを見るのには、「/msg %S
	HELP IDENTIFY」を入力して下さい。
	
	別のニックのリンクを消去したい時は、そのニックとパスワード
	を入力すれば消去できます。

NICK_HELP_SET
	シンタクス：SET option parameters
	
	ニックのオプションを設定します。optionの選択は次の通りです。
	
	    PASSWORD   パスワードの変更
	    LANGUAGE   Servicesが使う言語の設定
	    URL        ホームページのアドレスの入力
	    EMAIL      電子メールのアドレスの入力
#@@@  切断「オプション」、それと「守」...？
	    KILL       切断オプションの入切
	    SECURE     セキュアオプションの入切
	    PRIVATE    LISTのコマンドからニックを隠す
	    HIDE       INFOのコマンドからニックの情報を隠す
	
	このコマンドを使うのには、パスワードを入力する必要がありま
	す。パスワードの入力のヘルプを見るのには、「/msg %S
	HELP IDENTIFY」を入力して下さい。
	
	オプションの情報を読むのには、「/msg %S HELP SET option」を
	入力して下さい。

NICK_HELP_SET_PASSWORD
	シンタクス：SET PASSWORD new-password
	
	ニックの保有者のパスワードを変更します。

NICK_HELP_SET_LANGUAGE
	シンタクス：SET LANGUAGE number
	
	Servicesが使う言語を設定します。numberの選択は次の通りです。
	

NICK_HELP_SET_URL
	シンタクス：SET URL url
	
	urlというホームページのアドレスをニックの情報に追加します。
	このアドレスはINFOのコマンドの情報で表れます。

NICK_HELP_SET_EMAIL
	シンタクス：SET EMAIL address
	
	addressという電子メールのアドレスをニックの情報に追加します。
	このアドレスはINFOのコマンドの情報で表れます。

NICK_HELP_SET_SECURE
	シンタクス：SET SECURE {ON | OFF}
	
	セキュアオプションを設定します。ONにすると、ユーザーアドレ
	スがアクセス表に存在していてもパスワードを入力しなければ保
	有者の特権を得ません。ただし、ユーザーアドレスがアクセス表
	にあれば、切断オプションがオンに設定されても切断されません。

NICK_HELP_SET_PRIVATE
	シンタクス：SET PRIVATE {ON | OFF}
	
	「私的」というオプションを設定します。ONにすると、ニックは
	「/msg %S LIST」のリストで表れません。ただし、ONにし
	てもニックを知っているユーザーはINFOのコマンドでニックの情
	報を見ることが出来ます。

NICK_HELP_SET_HIDE
	シンタクス：SET HIDE {EMAIL | USERMASK | QUIT} {ON | OFF}
	
	ニックの情報を%SのINFOのコマンドから隠します。隠せる
	のは、電子メールのアドレス（EMAIL）、前のユーザーアドレス
	（USERMASK）、前の切断メッセージ（QUIT）です。二番目のパラ
	メーターは、ONにすると情報を隠し、OFFにすると情報を表しま
	す。

NICK_HELP_RECOVER
	シンタクス：RECOVER nickname [password]
	
	他のユーザーにニックが取られたらそのニックを取り戻します。
#@@@  切断守の「ある」
	このコマンドを使うと、切断守が設定してあるニックから一分以
	内でニックを変えない場合と同じ結果が出ます。ニックを使用し
	ているユーザーを切断してから、%Sはそのニックを取り、
	一分間誰も使えなくなります。その一分がたってからニックを取
	り戻せます。それと、%SのRELEASEのコマンドを使えば一
	分が経つ前に取り戻せます。
	
	RECOVERのコマンドを使うのには、現在のユーザーアドレスが
	nicknameというニックのアクセス表にある必要があります。また、
	パスワードを入力すればニックを取り戻せます。

NICK_HELP_RELEASE
	シンタクス：RELEASE nickname [password]
	
#@@@  「ホルド」
	ニックのホルドを解放します。ニックを使用していたユーザーが
	切断されてから、ニックは一分間使えなくなります。その一分が
	たつ前にニックを使用したい場合はこのコマンドを使うのです。
	
	RELEASEのコマンドを使うのには、現在のユーザーアドレスが
	nicknameというニックのアクセス表にある必要があります。また、
	パスワードを入力すればニックのホルドが解放されます。

NICK_HELP_GHOST
	シンタクス：GHOST nickname [password]
	
	「ゴースト」という接続を切断します。ゴーストは、本当に存在
	していない接続のことです。普通は、ゴーストが出るのはパソコ
	ンがフレーズしたりモデムの接続が切断されたりするときです。
	もちろん、ゴーストだけでなく普通のユーザーもこのコマンドで
	切断できますが、多くのIRCクライアントは切断されたらすぐ再接
	続しますので、このコマンドを使ってもニックを取り戻せないこ
	ともあります。その場合は、RECOVERのコマンドを使って下さい。
	
	GHOSTのコマンドを使うのには、現在のユーザーアドレスが
	nicknameというニックのアクセス表にある必要があります。また、
	パスワードを入力すればゴーストを切断できます。

NICK_HELP_INFO
	シンタクス：INFO nickname
	
	nicknameというニックの情報（保有者の名前・ユーザーアドレス、
	前の使用した日時、オプションの設定など）を表します。

NICK_HELP_LIST
	シンタクス：LIST pattern
	
	patternに合うニックとそのニックのユーザーアドレスを表しま
	す。ただし、私的オプションが ON のニックは表れません。
	patternをnick!user@hostという形で入力して下さい。
	
	例：
	    LIST *!joeuser@foo.com
	        joeuser@foo.comが保有しているニックを表します。
	    LIST *Bot*!*@*
	        「Bot」を含むニックを表します。
	    LIST *!*@*.bar.org
	        bar.orgのユーザーが保有しているニックを表します。

NICK_HELP_STATUS
	シンタクス：STATUS nickname...
	
#@@@ 名状・ChanServにもある
	nicknameというニックを使用しているユーザーはそのニックの保
	有者であるかそうでないかということを表示します。反応はこの
	形です：
	
	    nickname status-code
	
	nicknameはコマンドを入力したときと同じニックで、status-code
	は次の数値の一つです。
	
	    0 - ユーザーは現在接続していないか、ニックは登録されて
	        　　いない
	    1 - ユーザーは保有者ではないか、セキュアオプションは入
	        　　に設定されていてユーザーはパスワードを入力して
	        　　いない
	    2 - ユーザーのアドレスはアクセス表にあり、セキュアオプ
	        　　ションはオフに設定されている
	    3 - ユーザーはIDENTIFYでパスワードを入力した
	
	コマンドずつにニックを16以下を入力できますが、17以上の場合
	とニックを入力しない場合にはエラーメッセージは出ません。

NICK_SERVADMIN_HELP
	
	Servicesのアドミンは次のコマンドも使えます。
	
	    GETPASS    ニックのパスワードを見る
	               　　（暗号化が使われていない場合のみ）
	    FORBID     ニックを使用不可能にする
	
	なお、Servicesのアドミンはパスワードを入力せずにニックの登
	録を消去することもアクセス表を見ることも出来ます。アクセス
	表を見るのには「/msg %S ACCESS LIST nick」という形を
	使うのです。

NICK_SERVADMIN_HELP_DROP
	シンタクス：DROP [nickname]
	
	パラメーターがない場合は、現在使用しているニックを%S
	のデータベースから消去します。
	
	パラメーターがある場合は、入力したニックをデータベースから
	消去します。この使い方を使えるのはServicesのアドミンのみで
	す。

NICK_SERVADMIN_HELP_SET
	
	ServicesのアドミンはNOEXPIREというオプションを設定できます。
	このオプションでニックの登録を切れなくすることが出来ます。
	なお、Servicesのアドミンはパスワードを入力せずにどのニック
	でもオプションを設定できます。他のニックのオプションを設定
	する場合は「SET nickname option parameters」という形です。

NICK_SERVADMIN_HELP_SET_NOEXPIRE
	シンタクス：SET NOEXPIRE [nickname] {ON | OFF}
	
	nicknameというニックの登録は切れるかを設定します。ONにする
	と、登録は切れません。ニックを入力しない場合は、自分のニッ
	クのオプションを設定します。
	
	このコマンドの使用はServicesのアドミンに限っています。

NICK_SERVADMIN_HELP_UNLINK
	
	Servicesのアドミンは、パスワードを入力せずにどのニックのリン
	クでも消去できます。

NICK_SERVADMIN_HELP_LISTLINKS
	シンタクス：LISTLINKS nickname [ALL]
	
	入力したニックにリンクしているニックを表します。ALL を入力す
	ると、対象ニックと同じ権利を持っているニックを全て表し、入力
	しない場合は、対象ニックがリンクの直接対象になっているニック
	だけを表します。
	
	このコマンドの使用はServicesのアドミンに限っています。

NICK_SERVADMIN_HELP_LIST
	シンタクス：LIST pattern [FORBIDDEN] [NOEXPIRE]
	
	patternに合うニックとそのニックのユーザーアドレスを表しま
	す。ただし、私的オプションのONのニックはServicesのアドミン
	にしか表れません。patternをnick!user@hostという形で入力
	して下さい。
	
	また、Servicesのアドミンにのみ、登録の切れないニックの頭に
	「!」印が表れます。
	
	FORBIDDEN または NOEXPIRE を追加すれば、リストに表れるニッ
	クを制限することが出来ます。FORBIDDEN の場合は使用禁止ニッ
	ク、NOEXPIRE の場合は登録の切れないニックが表れます。両方
	を入力すれば、両方のタイプのニックが表れます。ただし、この
	オプションを使うのはServicesのアドミンのみです。
	
	例：
	    LIST *!joeuser@foo.com
	        joeuser@foo.comが保有しているニックを表します。
	    LIST *Bot*!*@*
	        「Bot」を含むニックを表します。
	    LIST * NOEXPIRE
	        登録の切れないニックを全て表します。

NICK_SERVADMIN_HELP_FORBID
	シンタクス：FORBID nickname
	
	ニックを使用不可能にします。キャンセルにはDROPのコマンドが
	使えます。
	
	このコマンドの使用はServicesのアドミンに限っています。

###########################################################################
#
# ChanServのヘルプメッセージ
#
###########################################################################

CHAN_HELP
	%Sは、チャンネルの登録サービスです。このサービスを使
	ってチャンネルを登録すれば、チャンネルのアクセスを制御出来
	るし、色々なオプションも設定できます。チャンネルの登録や制
	御のコマンドは次の通りです。コマンドを使う場合は、「/msg
	%S command」を入力して下さい。また、コマンドのヘルプ
	を読む場合は「/msg %S HELP command」を入力して下さい。
	
	    REGISTER   チャンネルの登録
	    IDENTIFY   チャンネルのパスワードの入力
	    SET        チャンネルのオプションの変更
	    ACCESS     特権のユーザー表の変更
#@@@ 「追放」
	    AKICK      追放したユーザー表の変更
	    DROP       チャンネルの登録の消去
	
	他のコマンド：INFO・LIST・INVITE・OP・DEOP・UNBAN・CLEAR

CHAN_HELP_EXPIRES
	
	注意：登録したチャンネルは%d日間使用されないと、登録は切
	れます。

CHAN_HELP_REGISTER
	シンタクス：REGISTER channel password description
	
	%Sのデータベースでチャンネルを登録します。このコマン
	ドを使うのは、チャンネルのオペレーターのみです。passwordは
	チャンネルのオプションを変更する為のパスワードです。最後の
	パラメーターはチャンネルの名状です。
	
	チャンネルを登録すると、登録したユーザーはそのチャンネルの
	「ファウンダー」になります。チャンネルのファウンダーは全て
	のチャンネルのオプションを変更出来るし、チャンネルに入ると
	%Sは自動的にファウンダーを+oモードにします。この特権
	の一部を他のユーザーに上げたい場合は「/msg %S HELP
	ACCESS」を入力して下さい。
	
	注意：チャンネルを登録するのには、ニックを登録する必要があ
	ります。ニックを登録していないユーザーは「/msg %s HELP」
	を入力して下さい。

CHAN_HELP_IDENTIFY
	シンタクス：IDENTIFY channel password
	
	%Sにチャンネルの保有者である証拠を示します。このコマ
	ンドを使わなければ、LEVELS・SET FOUNDER・SET PASSWORD・DROP
	のコマンドが使えません。なお、ファウンダーのニックを使用し
	ていないときは、このコマンドを入力すればファウンダーの特権
	を得ます。passwordはREGISTERのコマンドを入力したときのと
	同じパスワードです。

CHAN_HELP_DROP
	シンタクス：DROP channel
	
	channelというチャンネルを%Sのデータベースから消去します。
	このコマンドの使用はファウンダーに限っていて、パスワード
	を入力する必要もあります。

CHAN_HELP_SET
	シンタクス：SET channel option parameters
	
	チャンネルのオプションや情報を変更します。ファウンダーとパ
	スワードを変更するのには、チャンネルのパスワードをIDENTIFY
	のコマンドで入力する必要があります。オプションは次の通りで
	す。
	
	    FOUNDER     ファウンダーの変更
	    SUCCESSOR   次期ファウンダーの変更
	    PASSWORD    パスワードの変更
	    DESC        チャンネルの名状の変更
	    URL         ホームページのアドレスの入力
	    EMAIL       電子メールのアドレスの入力
	    ENTRYMSG    チャンネルに入るときに出るメッセージの入力
	    TOPIC       現在のチャンネルのトピックの変更
#@@@  「覚える」・下の「こと」、「というオプション」
	    KEEPTOPIC   チャンネルが使用されていない間にもトピック
	                　　を覚えることの入切
	    TOPICLOCK   SET TOPICコマンドでしかトピックを変更させ
	                　　ないというオプションの入切
	    MLOCK       チャンネルのモードをオン・オフにロックする
	    PRIVATE     LISTコマンドからチャンネルを隠す
	    RESTRICTED  特権を持っていないユーザーをチャンネルに入
	                　　れないというオプションの入切
	    SECURE      セキュアオプションの入切
	    SECUREOPS   +oモードの制御を厳しくする
	    LEAVEOPS	DEOPコマンドでしか-oモードを掛けない
	    VERBOSE     Send a notice to the channel's operators when
	                    certain commands are used on the channel
	
	オプションの情報を読むのには、「/msg %S HELP option」
	を入力して下さい。

CHAN_HELP_SET_FOUNDER
	シンタクス：SET channel FOUNDER nick
	
	ファウンダーを変更します。ただし、nickというニックが登録
	されていないと変更出来ません。

CHAN_HELP_SET_SUCCESSOR
	シンタクス：SET channel SUCCESSOR nick
	
	次期ファウンダーを変更します。次期ファウンダーとは、現在の
	ファウンダーのニックの登録が切れたり消去されたりするとファウ
	ンダーになるユーザーです。次期ファウンダーが設定されていない
	場合は、ファウンダーのニックの登録が切れると、チャンネルの登
	録も消去されてしまいます。ただし、ファウンダーのニックの登録
	が切れる時点で次期ファウンダーはチャンネルの限り（%d）を越え
	ていれば、次期ファウンダーが設定されていないようにチャンネル
	の登録は消えてしまいます。
	
	次期ファウンダーにするニック（nick）は登録していなければなり
	ません。

CHAN_HELP_SET_PASSWORD
	シンタクス：SET channel PASSWORD password
	
	ファウンダーのパスワードを変更します。

CHAN_HELP_SET_DESC
	シンタクス：SET channel DESC description
	
	チャンネルの名状を変更します。名状はLIST・INFOのコマンドの
	情報で表れます。

CHAN_HELP_SET_URL
	シンタクス：SET channel URL [url]
	
	urlというホームページのアドレスをチャンネルの情報に追加しま
	す。このアドレスはINFOのコマンドの情報で表れます。パラメー
	ターを入力しない場合は、ホームページのアドレスは消去されま
	す。

CHAN_HELP_SET_EMAIL
	シンタクス：SET channel EMAIL [address]
	
	addressという電子メールのアドレスをチャンネルの情報に追加し
	ます。このアドレスはINFOのコマンドの情報で表れます。パラ
	メーターを入力しない場合は、電子メールのアドレスは消去され
	ます。

CHAN_HELP_SET_ENTRYMSG
	シンタクス：SET channel ENTRYMSG [message]
	
	messageというテキストをチャンネルに入るユーザーに出します。
	パラメーターを入力しない場合は、メッセージは消去されます。

CHAN_HELP_SET_TOPIC
	シンタクス：SET channel TOPIC topic
	
	%Sにチャンネルのトピックを変更させます。だいたい、
	SET LOCKTOPICのコマンドと一緒に使われます。

CHAN_HELP_SET_KEEPTOPIC
	シンタクス：SET channel KEEPTOPIC {ON | OFF}
	
	「トピック記録」というオプションを設定します。トピック記録
	は、チャンネルの現在のトピックを記録し、チャンネルが消えて
	もそのトピックを覚え、チャンネルがまた作られるときにトピッ
	クを戻すというオプションです。

CHAN_HELP_SET_TOPICLOCK
	シンタクス：SET channel TOPICLOCK {ON | OFF}
	
	「トピックロック」というオプションを設定します。ONにすると、
	トピックは%SのSET TOPICのコマンドでしか変えられなくなりま
	す。

CHAN_HELP_SET_MLOCK
	シンタクス：SET channel MLOCK modes
	
	モードロックを設定します。%Sによって、チャンネルの入
	切モード（i・k・l・m・n・p・s・t）をオンかオフにロックでき
	ます。modesというパラメーターはIRCの「/MODE」というコマンド
	と同じ形で、「+」の後のモードはオンにロックされ、「-」の後
	のモードはオフにロックされます。
	
	注意：下の二番目の例のようにチャンネルキーをロックすれば、
	RESTRICTEDというオプションをオンにしなければ誰にでもキーが
	見えます。
	
	例：
	    SET #channel MLOCK +nt-iklps
	       n・tモードをオンにロックし、i・k・l・p・sモードをオ
	       フにロックします。mモードはロックされません。
	
	    SET #channel MLOCK +knst-ilmp my-key
	       k・n・s・tモードをオンにロックし、i・l・m・pモードを
	       オフにロックします。キーを「my-key」にロックします。
	
	    SET #channel MLOCK +
	       モードロックを解きます。

CHAN_HELP_SET_PRIVATE
	シンタクス：SET channel PRIVATE {ON | OFF}
	
	「私的」というオプションを設定します。ONにすると、チャンネ
	ルは「/msg %S LIST」のリストで表れません。

CHAN_HELP_SET_RESTRICTED
	シンタクス：SET channel RESTRICTED {ON | OFF}
	
	アクセス制御オプションを設定します。ONにすると、+oモードを
	得られないユーザー（負数のレベルのユーザー、そしてセキュア
	オプションをONにするとアクセス表にないユーザー）はチャンネ
	ルからに入れなくなります。

CHAN_HELP_SET_SECURE
	シンタクス：SET channel SECURE {ON | OFF}
	
	セキュアオプションを設定します。ONにすると、ニックのパス
	ワードを入力していないユーザーは特権を得ません。アクセス制
	御オプションと一緒に使えば、パスワードを入力していないユー
	ザーはチャンネルに入れなくなります。

CHAN_HELP_SET_SECUREOPS
	シンタクス：SET channel SECUREOPS {ON | OFF}
	
	+oモードの制御を厳しくすることを設定します。ONにすると、ア
	クセス表にないユーザーは+oモードを得られなくなります。

CHAN_HELP_SET_LEAVEOPS
	シンタクス：SET channel LEAVEOPS {ON | OFF}
	
	+oモードの制御をやさしくすることを設定します。ONにすると、
	%Sは自動的に-oモードをかけません。

CHAN_HELP_ACCESS
	シンタクス：ACCESS channel ADD nick level
	　　　　　　ACCESS channel DEL {nick | entry-num | list}
	　　　　　　ACCESS channel LIST [mask | list]
	
	チャンネルのアクセス表の変更と表示のコマンドです。アクセス
	表は、特権のあるユーザーのリストで、どのユーザーが+oモード
	を得られるかどのユーザーが%Sのコマンドを使えるか含み
	ます。level（レベル）の数値によってどの特権を得るか変わり
	ます。詳しい事なら、「/msg %S HELP ACCESS LEVELS」を
	入力して下さい。
	
	ACCESS ADDという形でユーザーをアクセス表に追加できますが、
	すでにアクセス表に存在しているユーザーのレベルを変更するこ
	とも出来ます。追加と変更出来るのは、コマンドを入力するユー
	ザーの現在のレベルを含まず、そのレベル以下のみです。また、
	ユーザーのレベルを変更する場合は、そのユーザーの現在のレベ
	ルは入力するユーザーのレベル以上なら変更出来ません。ファウ
	ンダーならどのレベルでも追加できます。
	
	ACCESS DELという形でユーザーをアクセス表から消去できます。
	消去出来るのは、入力するユーザーのレベルを含まず、そのレベ
	ル以下のユーザーのみです。番号列を入力することも出来ます
	（下記のLISTの例へ参照）。
	
	ACCESS LISTという形でアクセス表を見ることが出来ます。マスク
	を入力すれば、そのマスクに合うユーザーしか表れません。下記の
	例の通り、番号列を入力することも出来ます：
	
	    ACCESS #channel LIST 2-5,7-9
	       ２〜５、７〜９番目のアクセス表のエントリーを表します。

CHAN_HELP_ACCESS_LEVELS
	チャンネルのアクセスレベル
	
	既定的なレベルは次の通りです。
	
	ファウンダー　　全ての%Sのコマンドを使える
	　　　　　　　　チャンネルに入ると自動的に+oモード
	　　　10　　　　自動的に+oモード
	　　　　　　　　AKICKのコマンドも使える
	       5　　　　自動的に+oモード
	       3　　　　自動的に+vモード
	       0　　　　特権無し
	　　　　　　　　オペレーターから+oモードを得られる
	      -1以下　　+oモードを得られない
	
	このレベルを変えたり新しいレベルをつくったりすることが
	LEVELSのコマンドで出来ます。

CHAN_HELP_AKICK
	シンタクス：AKICK channel ADD mask [reason]
	　　　　　　AKICK channel DEL mask
	　　　　　　AKICK channel LIST [mask]
	
	「自動蹴表」（オートキック表）を変更と表示のコマンドです。
	自動蹴表にあるユーザーがチャンネルに入ると、%Sはその
	ユーザーを蹴り追放マスクを付けます。
	
	AKICK ADDという形で自動蹴表にマスクとニックを追加できます。
	マスクなら、user@hostという形で入力して下さい。reason（理由）
	はキックメッセージで表れます。理由を入力しない場合は、「You
	have been banned from the channel」（チャンネルから追放され
	ました）になります。
	
	AKICK DELという形で自動蹴表からマスクとニックを消去できま
	す。ただし、追放マスクがチャンネルに付いている場合は、その
	マスクは自動に解かれません。
	
	AKICK LISTという形で自動蹴表を見ることが出来ます。マスクを
	入力すれば、そのマスクに合うエントリーしか表れません。

CHAN_HELP_LEVELS
	シンタクス：LEVELS channel SET type level
	　　　　　　LEVELS channel {DIS | DISABLE} type
	　　　　　　LEVELS channel LIST
	　　　　　　LEVELS channel RESET
	
	チャンネルのアクセスレベルの番号の意味を変更します。このコ
	マンドで%Sの自動的な作用とほとんどの%Sのコマン
	ドが使えるレベルのを設定できます。ただし、SET FOUNDER・SET
	PASSWORD・LEVELSのコマンドは、ファウンダーしか使えません。
	
	LEVELS SETという形で作用やコマンドのレベルを変更できます。
	LEVELS DISABLEという形（それとLEVELS DISという省略）で自動
	的な作用を停止することと、コマンドをファウンダー以外に禁制
	することが出来ます。LEVELS LISTという形で自動的な作用とコマ
	ンドのレベルを表示できます。LEVELS RESETという形でレベルを
	既定値に戻せます。
	
	type（作用の名前）の選択を見る場合は、「/msg %S HELP
	LEVELS DESC」を入力して下さい。

CHAN_HELP_LEVELS_DESC
	LEVELSの作用とコマンドの名前は次の通りです。
	注意：AUTODEOPとNOJOINは最大値で、他のは最小値です。

CHAN_HELP_LEVELS_DESC_FORMAT
	    %-*s  %s

CHAN_HELP_INFO
	シンタクス：INFO channel
	
	チャンネルの情報（ファウンダー、名状、登録の日時、モード
	ロック等）を表します。

CHAN_HELP_LIST
	シンタクス：LIST pattern
	
	patternに合うチャンネルを表します。ただし、私的オプション
	が ON のチャンネルは表れません。

CHAN_HELP_OP
	シンタクス：OP #channel nick
	
	チャンネルのユーザーにオペレーターにします。使用は既定的に
	レベル5以上に限っています。

CHAN_HELP_DEOP
	シンタクス：DEOP #channel nick
	
	チャンネルのユーザーを-oモードにします。使用は既定的にレベ
	ル5以上に限っています。

CHAN_HELP_INVITE
	シンタクス：INVITE channel
	
	%Sに自分を誘わせます。使用は既定的にレベル1以上に限っ
	ています。

CHAN_HELP_UNBAN
	シンタクス：UNBAN channel
	
	現在のユーザーアドレスに合う追放マスクを解きます。使用は既
	定的にレベル1以上に限っています。

CHAN_HELP_CLEAR
	シンタクス：CLEAR channel what
	
	チャンネルのあるモードをクリアしたり、ユーザーを全て蹴った
	りします。whatの選択は次の通りです。
	
	     modes   チャンネルの入切モード（i・k・l・m・n・p・s・t）
	             　　をクリアする
	     bans    追放マスクを全て解く
	     ops     全てのユーザーから+oモードをクリアする
	     voices  全てのユーザーから+vモードをクリアする
	     users   全てのユーザーをチャンネルから蹴る
	
	使用は既定的にレベル 10 以上に限っています。

CHAN_SERVADMIN_HELP
	
	Servicesのアドミンは次のコマンドも使えます。
	
	    GETPASS    チャンネルのパスワードを見る
	               　　（暗号化が使われていない場合のみ）
	    FORBID     チャンネルを使用不可能にする
	    STATUS     ユーザーのチャンネルでのアクセスレベル
	
	なお、Servicesのアドミンはパスワードを入力せずにチャンネル
	の登録を消去することも、アクセス表・自動蹴表・レベル表を見
	ることも出来ます。

CHAN_SERVADMIN_HELP_DROP
	シンタクス：DROP channel
	
	channelというチャンネルを%Sのデータベースから消去します。
	このコマンドの使用はファウンダーとServicesのアドミンに
	限っていて、Servicesのアドミンでなければパスワードを入力す
	る必要もあります。

CHAN_SERVADMIN_HELP_SET
	
	ServicesのアドミンはNOEXPIREというオプションを設定できます。
	このオプションでチャンネルの登録を切れなくすることが出来ま
	す。なお、Servicesのアドミンはパスワードを入力せずにチャン
	ネルのオプションを設定できます。

CHAN_SERVADMIN_HELP_SET_NOEXPIRE
	シンタクス：SET channel NOEXPIRE {ON | OFF}
	
	channelというチャンネルの登録は切れるかを設定します。ONにす
	ると、登録は切れません。
	
	このコマンドの使用はServicesのアドミンに限っています。

CHAN_SERVADMIN_HELP_LIST
	シンタクス：LIST pattern
	
	patternに合うチャンネルを表します。ただし、私的オプション
	を入したチャンネルはServicesのアドミンにしか表れません。

CHAN_SERVADMIN_HELP_FORBID
	シンタクス：FORBID channel
	
	ニックを使用不可能にします。キャンセルにはDROPのコマンドが
	使えます。
	
	このコマンドの使用はServicesのアドミンに限っています。

CHAN_SERVADMIN_HELP_STATUS
	シンタクス：STATUS channel nickname
	
	nicknameというニックのchannelというチャンネルでのアクセ
	スレベルを表示します。反応はこの形です：
	
	    STATUS channel nickname access-level
	
	エラーが発生した場合は、反応は次の形です：
	
	    STATUS ERROR error-message
	
	このコマンドの使用はServicesのアドミンに限っています。

###########################################################################
#
# MemoServのヘルプメッセージ
#
###########################################################################

MEMO_HELP
	%Sは、ユーザーとチャンネルにメモを送るためのサービス
	です。メモはServicesにより保存され、受信ニックかチャンネル
	のオペレーターは読みたい時に読めます。ただし、差出人のニッ
	クも受信ニックかチャンネルも登録しなければ%Sを使えません。
	
	%Sのコマンドは次の通りです。
	
	    SEND   ユーザーかチャンネルにメモを送る
	    LIST   受信メモ表を表す
	    READ   メモを読む
	    DEL    メモを消去する
	    SET    メモのオプションを設定する
	
	コマンドのヘルプを読む場合は、「/msg %S HELP command」
	を入力して下さい。
	
	※チャンネルのメモが読めるのは既定的にレベル10以上のユーザー
	　のみですが、%sの「LEVELS」というコマンドでそのレベルを変
	　更できます。

MEMO_HELP_SEND
	シンタクス：SEND {nick | channel} memo-text
	
	nickかchannelにmemo-textを含むメモを送ります。ニックに出す
	場合には、受信者のニックオプションによって、%Sは受信
	者に新たなメモが届いたとのことを報告します。

MEMO_HELP_LIST
	シンタクス：LIST [channel] [list | NEW]
	
	受信メモ表を表します。NEWを入力すれば、読んでいないメモのみ
	が表れます。読んでいないメモは、メモの番号の左側に＊印が表
	示されます。下記の例の通り、番号列も入力可能：
	
	    LIST 2-5,7-9
	       ２〜５、７〜９番目のメモをリストします。

MEMO_HELP_READ
	シンタクス：READ [channel] {num | list | LAST | NEW}
	
	メモを読みます。番号を入力するとその番号のメモが表れ、LASTを
	入力すると最新のメモが表れ、NEWを入力すると読んでいないメモ
	が全て表れます。下記の例の通り、番号列も入力可能：
	
	    LIST 2-5,7-9
	       ２〜５、７〜９番目のメモをリストします。

MEMO_HELP_DEL
	シンタクス：DEL [channel] {num | list | ALL}
	
	メモを消去します。番号を入力するとその番号のメモが消去され、
	ALLを入力すると全てのメモが消去されます。
	
	例：
	   DEL 1
	      １番目のメモを消去します。
	   DEL 2-5,7-9
	      ２番目〜５番目と７番目〜９番目のメモを消去します。

MEMO_HELP_SET
	シンタクス：SET option parameters
	
	メモのオプションを設定します。optionの選択は次の通りです。
	
	    NOTIFY      どの場合に%Sが新メモの報告するかの設定
	                　（ニックのみ）
	    LIMIT       メモの数の限りの設定
	
	オプションの情報を読む場合は、「/msg %S HELP SET option」
	を入力して下さい。

MEMO_HELP_SET_NOTIFY
	シンタクス：SET NOTIFY {ON | LOGON | NEW | OFF}
	
	どの場合に%Sが新メモの報告するかの設定のコマンドです。
	設定の選択は次の通りです。
	
	    ON      ログオンするとき、IRCに戻るとき、メモが届くとき
	    LOGON   ログオンするとき、IRCに戻るとき
	    NEW     メモが届くとき
	    OFF     報告は無し
	
	ONの結果はLOGONとNEWの結果を合わせることと思えます。

MEMO_HELP_SET_LIMIT
	シンタクス：SET LIMIT [channel] limit
	
	メモの数の限りを設定します。ゼロにすると、メモを受けられな
	くなります。ただし、正数の場合は%d以下の数値しか入力できま
	せん。

MEMO_HELP_INFO
	シンタクス：INFO [channel]
	
	メモの情報を見ます。このコマンドで表れるのは、現在のメモの
	数と読んでいないメモの数とメモの数の限りです。パラメーター
	を入力すればそのチャンネルの情報が表れます。

MEMO_SERVADMIN_HELP_SET_LIMIT
	シンタクス：SET LIMIT [user | channel] {limit | NONE} [HARD]
	
	ユーザーかチャンネルのメモの数の限りを設定します。ゼロにす
	るとユーザーはメモを受けられなくなり、NONEにすると限りは無
	くなります。ニックを入力しない場合は、コマンドを出すユーザ
	ーのニックのメモの限りが設定されます。HARDを入力すれば、ユー
	ザーは限りを変更できなくなり、入力しなければユーザーは限り
	を変更できます。
	
	この使い方が使えるのはServicesのアドミンのみです。一般の
	ユーザーは、他のユーザーの限りを設定できないし、限りをのけ
	られないし、HARDも入力できません。

MEMO_SERVADMIN_HELP_INFO
	シンタクス：INFO [nick | channel]
	
	メモの情報を見ます。このコマンドで表れるのは、現在のメモの
	数と読んでいないメモの数とメモの数の限りです。パラメーター
	がある場合にそのユーザーかチャンネルの情報が表れ、パラメー
	ターが無い場合に自分の情報が表れます。
	
	ニックのパラメーターを入力できるのはServicesのアドミンのみ
	です。

###########################################################################
#
# OperServのヘルプメッセージ
#
###########################################################################

OPER_HELP
	%Sのコマンドは次の通りです。
	
	一般のIRCのオペレーターが使えるコマンド：
	    GLOBAL      全てのユーザーにメッセージを送る
	    STATS       Servicesとネットのステータスを表す
	    OPER LIST   Servicesのオペレーター表を表す
	    ADMIN LIST  Servicesのアドミン表を表す
	
	Servicesのオペレーターとアドミンだけが使えるコマンド：
	    MODE        チャンネルのモードを変更する
	    KICK        チャンネルからユーザーをキックする
	    AKILL       自動切断表を変更する
	
	Servicesのアドミンだけが使えるコマンド：
	    OPER        Servicesのオペレーター表を変更する
	    LOGONNEWS   ユーザーが接続する時に出されるメッセージを
	                　　作る
	    OPERNEWS    ユーザーがオペレーター（+oモード）になる時
	                　　に出されるメッセージを作る
	    JUPE        サーバーを「ジュピター」（接続拒否）する
	    RAW         Servicesの接続サーバーにテキストを出す
	    SET         Servicesのオプションの設定
	    QUIT        Servicesを止める
	    RESTART     Servicesを再起動させる
	
	Services root（スーパーユーザー）だけが使えるコマンド：
	    ADMIN       Servicesのアドミン表を変更する
	
	注意：%Sに出されたコマンドは全てロッグされます。

OPER_HELP_GLOBAL
	シンタクス：GLOBAL message
	
	全てのネットに接続しているユーザーにメッセージを出します。
	メッセージの指出ニックは%sです。

OPER_HELP_STATS
	シンタクス：STATS [AKILL | ALL]
	
	パラメーターがない場合は、現在のユーザー数とオペレーター数、
	最大ユーザー数、起動からの時間を表します。
	
	AKILLというパラメーターがある場合は、現在の自動切断マスクの
	数と既定的な寿命を表します。
	
	ALLというオプションが使えるのはServicesのアドミンのみです。
	このオプションを付けると、普通の情報（ユーザー数と起動から
#@@@ 「memory usage」の翻訳
	の時間）の下でServicesの使用しているメモリー量も表れます。
	この形を使うとServicesは少しの間フリーズしてしまいますので、
	ご注意下さい。
	
	「STATS」の代わりに「UPTIME」も使えます。

OPER_HELP_OPER
	シンタクス：OPER ADD nick
	　　　　　　OPER DEL nick
	　　　　　　OPER LIST
	
	Servicesのオペレーター表の変更と表示のコマンドです。このリ
	ストにあるユーザーは、%sのIDENTIFYのコマンドでパスワー
	ドを入力すればServicesのオペレーターのコマンドが使えます。
	
	誰でもOPER LISTという形が使えます。他の使用はServicesのアド
	ミンに限っています。

OPER_HELP_ADMIN
	シンタクス：ADMIN ADD nick
	　　　　　　ADMIN DEL nick
	　　　　　　ADMIN LIST
	
	Servicesのアドミン表の変更と表示のコマンドです。このリスト
	にあるユーザーは、%sのIDENTIFYのコマンドでパスワード
	を入力すればServicesのアドミンのコマンドが使えます。
	
	誰でもADMIN LISTという形が使えます。他の使用はServices root
	に限っています。

OPER_HELP_MODE
	シンタクス：MODE channel modes
	
	チャンネルのモードを変更します。パラメーターはIRCの「/MODE」
	のコマンドと同じです。
	
	このコマンドの使用はServicesのオペレーターに限っています。

OPER_HELP_CLEARMODES
	シンタクス：CLEARMODES channel [ALL]
	
	チャンネルの入切モード（i・k・l・m・n・p・s・t）と追放マス
	クを全てクリアします。ALLというパラメーターを入力すると、
	o・vモードもクリアされます。
	
	このコマンドの使用はServicesのオペレーターに限っています。

OPER_HELP_KICK
	シンタクス：KICK channel user reason
	
	チャンネルからユーザーをキックします。パラメーターはIRCの
	/KICKというコマンドと同じですが、キックメッセージの前にこの
	コマンドを使ったユーザーのニックが表れます。
#@@@ 例が必要
	
	このコマンドの使用はServicesのオペレーターに限っています。

OPER_HELP_AKILL
	シンタクス：AKILL ADD [+expiry] mask reason
	　　　　　　AKILL DEL mask
	　　　　　　AKILL LIST [mask]
	　　　　　　AKILL VIEW [mask]
	
	「自動切断表」（オートキル表）の変更と表示のコマンドです。
	自動切断マスクに合うユーザーが接続すると、Servicesはその
	ユーザーを切断し、サーバーのプロトコルによってそのマスクを
	「K:ライン」にします。
	
	AKILL ADDという形でマスクを自動切断表に追加できます。
	reason（理由）は必要ですが、オペレーターの為だけですので、
	切断されたユーザーの切断メッセージに表れません。
	expiryを入力すれば、自動切断エントリーの寿命を指定できます。
	形は正数に時間の単位を指示する文字を付けます。単位の文字は
	「d」（日）、「h」（時間）、「m」（分）で、文字を付けないと
	日になります。ただし、文字を合わせること（「1h30m」など）は
	出来ません。「+0」にすれば、自動切断エントリーは切れません。
	寿命を入力しない場合は、既定寿命になります。STATS AKILLとい
	うコマンドでその既定値を表すことが出来ます。
	　※マスクは「+」で始まる場合は、寿命を入力しなければなりま
	　　せん。
	
	AKILL DELという形でマスクを自動切断表から消去できます。
	
	AKILL LISTという形で自動切断表を表せます。パラメーターを入
	力すれば、そのマスクに合うエントリーしか表れません。
	
	AKILL VIEWという形でAKILL LISTのように自動切断表を表すこと
	が出来ますが、LISTより多くの情報（入力したユーザーと日時、
	切れる日時）が出ます。
	
	このコマンドの使用はServicesのオペレーターに限っています。

OPER_HELP_JUPE
	シンタクス：JUPE server [reason]
	
	サーバーを「ジュピター」します。ジュピター（jupiter）とは、
	サーバーをネットに接続させない為に仮サーバーを作リネットにつ
	なげることです。仮サーバーは「/SQUIT」というIRCのコマンドで
	消すことが出来ます。reason（理由）を入力すれば、その文字
	列は仮サーバーの/LINKSなどでサーバー情報として表れます。理由
	を入力しない場合は、サーバー名がサーバー情報として使用されま
	す。
	
	このコマンドの使用はServicesのアドミンに限っています。

OPER_HELP_RAW
	シンタクス：RAW text
	
	Servicesが接続しているサーバーにテキストを出します。IRCのプ
	ロトコルがはっきり分からずにこのコマンドを使ったり、入力で
	間違いをしてこのコマンドを出したりしてしまえば、ネットを狂
	わせることがありますので、充分ご注意下さい。
	
	このコマンドの使用はServicesのアドミンに限っています。

OPER_HELP_SET
	シンタクス：SET option setting
	
	Services全体のオプションを設定します。オプションの名前は次
	の通りです。
	    READONLY   リードオンリーモードの入切
	    DEBUG      デバッグモードの入切
	
	このコマンドの使用はServicesのアドミンに限っています。

OPER_HELP_SET_DEBUG
	シンタクス：SET DEBUG {ON | OFF | num}
	
	デバッグモードを入切します。ONにすれば、Servicesが受けたり
	出したりするテキストと他のデバッグメッセージはロッグされま
	す。numという正数を入力すると、デバッグのレベルはnumに設定
	されます。
	
	このコマンドは「-debug」というコマンドラインオプションと同
	じ結果を出します。

OPER_HELP_QUIT
	シンタクス：QUIT
	
	データベースを保存せずにServicesを消します。このコマンド
	を使うべき場合は、データベースの狂いの恐れがある場合のみで
	す。他の場合は、SHUTDOWNのコマンドをお使い下さい。
	
	このコマンドの使用はServicesのアドミンに限っています。

OPER_HELP_SHUTDOWN
	シンタクス：SHUTDOWN
	
	データベースを保存してServicesを消します。
	
	このコマンドの使用はServicesのアドミンに限っています。

OPER_HELP_RESTART
	シンタクス：RESTART
	
	データベースを保存してServicesを再起動させます。
	
	このコマンドの使用はServicesのアドミンに限っています。
