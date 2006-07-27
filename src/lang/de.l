# German language file.
#
# Blitzed Services copyright (c) 2000-2002 Blitzed Services team
#     E-mail: services@lists.blitzed.org
# This program is free but copyrighted software; see the file COPYING for
# details.
#
# $Id$
#
# When translating this file to another language, keep in mind that the order
# of parameters for sprintf() is fixed in the source code, so your messages
# need to take the same parameters in the same order as the English messages
# do.  (Obviously, this doesn't hold for the strftime() format lines
# immediately below.)  If you can't get a natural translation of a message
# without changing the order of the parameters, let us know
# (services@lists.blitzed.org) which message is causing a problem and we'll see
# what we can do.
#
# In help messages, "%S" (capital S, not lowercase) refers to the name of
# the service sending the message; for example, in NickServ help messages,
# "%S" is replaced by "NickServ" (or whatever it is renamed to in
# config.h).  The %S's do not count as sprintf() parameters, so they can be
# rearranged, removed, or added as necessary.
#
# Also in help messages, please try to limit line lengths to 60 characters
# of text (not including the leading tab).  This length was chosen because
# it does not cause line wrap under default settings on most current IRC
# clients.  Remember that format characters (control-B, control-_) are not
# included in that 60-character limit (since they don't show on the user's
# screen).  Also remember that format specifiers (%S, etc.) will expand
# when displayed, so remember to take this into account; you can assume
# that the length of a pseudoclient name (%S replacement) will be eight
# characters, so reduce the maximum line length by 6 for every %S on a
# line.
#
# Finally, remember to put a tab at the beginning of every line of text
# (including empty lines).  This has to be a tab, not spaces.
#
###########################################################################
#
# Name of this language
#
###########################################################################
#
# For languages other than English, this string should have the following
# format:
#     language-name-in-language (language-name-in-English)
# For example, "Español (Spanish)" or "Français (French)".
#
LANG_NAME
	Deutsch (German)
#
###########################################################################
#
# General messages
#
###########################################################################
#
# strftime() format strings.  man 3 strftime for information on the
# meanings of the format specifiers.  Short version:
#	%a = weekday name (short)	%H = hour
#	%b = month name (short)		%M = minute
#	%d = day of month		%S = second
#	%Y = year			%Z = time zone
#
# This is used as the format string for strftime() for a date and time
# together.
STRFTIME_DATE_TIME_FORMAT
	%d. %b %H:%M:%S %Y %Z
# This is used as the format string for strftime() for a date alone in
# short format (English: without weekday).
STRFTIME_SHORT_DATE_FORMAT
	%d.%m.%Y
#
# These tell our strftime() what the names of months and days are.  If you
# don't use %a, %A, %b, or %B in your strftime() strings above, you can
# leave these empty.  However, if you enter names, they MUST stay in order,
# one per line, and the list MUST be complete!
#
# %a
STRFTIME_DAYS_SHORT
	So
	Mo
	Di
	Mi
	Do
	Fr
	Sa
# %A
STRFTIME_DAYS_LONG
	Sonntag
	Montag
	Dienstag
	Mittwoch
	Donnerstag
	Freitag
	Samstag
# %b
STRFTIME_MONTHS_SHORT
	Jan
	Feb
	Mrz
	Apr
	Mai
	Jun
	Jul
	Aug
	Sep
	Okt
	Nov
	Dez
# %B
STRFTIME_MONTHS_LONG
	Januar
	Februar
	März
	April
	Mai
	Juni
	Juli
	August
	September
	Oktober
	November
	Dezember
#
#
# This is used in ChanServ/NickServ INFO displays.
COMMA_SPACE
	, 
#
# This is used in ChanServ/NickServ INFO displays.
INFO_END
	*** Ende der Information
#
# This is used in ChanServ/NickServ REGISTER
#
# Various error messages.
USER_RECORD_NOT_FOUND
	Interner Fehler - unfähig Auftrag zu bearbeiten.
UNKNOWN_COMMAND
	Unbekannter Befehl %s.
UNKNOWN_COMMAND_HELP
	Unbekannter Befehl %s.  "/msg %s HELP" für Hilfe.
SYNTAX_ERROR
	Syntax: %s
MORE_INFO
	/msg %s HELP %s für weitere Informationen.
NO_HELP_AVAILABLE
	Keine Hilfe für %s verfügbar.
#
BAD_USERHOST_MASK
	Maske muß folgendes Format haben: user@host.
BAD_EXPIRY_TIME
	Ungültige Verfallsdauer.
BAD_EMAIL
	%s ist keine gültige E-Mail-Adresse.

PASSWORD_INCORRECT
	Falsches Passwort.
#
# What's the difference between "Access denied" and "Permission denied"?
# Very little; you can safely make them the same message with no real loss
# of meaning.  If you want to make a distinction, "Access denied" is
# usually used more generally; for example, a non-oper trying to access
# OperServ gets "Access denied", while a non-Services admin trying to use
# NickServ SET NOEXPIRE gets "Permission denied".
ACCESS_DENIED
	Zugriff verweigert.
PERMISSION_DENIED
	Nicht erlaubt.
#
MORE_OBSCURE_PASSWORD
	Bitte versuche ein komplizierteres Passwort. Passwörter sollten mindestens fünf Zeichen lang oder nichts leicht zu erratendes (z.B. deinen Namen oder Nick) sein und dürfen keine Leerzeichen oder Tabstops beinhalten.
PASSWORD_TRUNCATED
	Hinweis: Dein Passwort ist auf %d Zeichen verkürzt worden,
	die letzten Zeichen wurden abgeschnitten.

#NEED_EMAIL
#	Du benötigst eine gültige E-Mail Adresse, bevor du %s kannst.
#@grifferz: this one could not make sense...
#
SENDPASS_UNAVAILABLE
	SENDPASS nicht verfügbar.

SENDPASS_FAILED
	SENDPASS fehlgeschlagen: %s

# These are the emails that are going to be sent to people when their nick
# or channel is sendpassed
#
#NICK_SENDPASS_MAIL
#	From: %s
#	To: %s <%s>
#	Subject: NickServ Password Retrieval
#	X-Abuse: %s
#	
#	You requested the password to the nickname '%s' on the %s IRC Network.
#	As per our password retrieval policies, a new password has been created for
#	you and e-mailed to the address you set in NickServ, %s.
#	
#	The new password for %s is %s
#	
#	Now that you have a new password you may want to change it.  You can do this
#	by typing:
#	
#	/msg NickServ SET PASSWORD NewPassword
#	
#	Note that you must identify to NickServ with your current password before
#	being able to do this.  Also note that passwords are CaSe SeNsItIvE.
#	
#	Thank you for using the %s IRC Network.
#
#CHAN_SENDPASS_MAIL
#	From: %s
#	To: %s <%s>
#	Subject: ChanServ Password Retrieval
#	X-Abuse: %s
#	
#	You requested the password to the channel '%s' on the %s IRC Network.
#	As per our password retrieval policies, a new password has been randomly
#	generated and sent to the founder's address you set in NickServ, %s.
#	
#	The new password for %s is %s
#	
#	If you did not request this password retrieval, please contact us at %s.
#	
#	Now that you have your password you may want to change it.  You can do this
#	by typing:
#	
#	/msg ChanServ SET #channel PASSWORD NewPassword
#	
#	Note that you must identify to ChanServ with your current password before
#	being able to do this.  Also note that passwords are CaSe SeNsItIvE.
#	
#	Thank you for using the %s IRC Network.
#
#NICK_REG_AUTH_MAIL
#	From: %s
#	To: %s <%s>
#	Subject: OFTC Nick Registration
#	X-Abuse: %s
#	
#	Hi!  Someone (you, hopefully) has just completed the first step of registering
#	'%s' as a new nickname on the OFTC IRC Network.  To complete the process,
#	you need to return to OFTC, change to your chosen nick, and give the following
#	command:
#	
#	/msg NickServ REGISTER AUTH %s
#	
#	After that point, your nick should be fully registered to you.  You may want to
#	look into the nick settings which allow you to hide details such as your E-mail
#	address from other users:
#	
#	/msg NickServ HELP SET HIDE
#	
#	We'll reserve your nickname for 24 hours from the date of this E-mail.  If you
#	don't complete the AUTH process before then, you will need to begin again from
#	the start, and the nickname will be available for other people to register
#	also.
#	
#	If you have received this E-mail in error, and know nothing about what is going
#	on, please report the matter to %s.
#	
#	Thank you for using the %s IRC Network.
#
#NICK_DROP_AUTH_MAIL
#	From: %s
#	To: %s <%s>
#	Subject: OFTC Nick Drop
#	X-Abuse: %s
#
#	Hi!  Someone (you, hopefully) has just completed the first step of dropping
#	(unregistering) the nickname '%s' on the OFTC IRC Network.  To complete
#	the process, you need to return to OFTC, change to your chosen nick, and
#	give the following command:
#	
#	/msg NickServ DROP AUTH %s
#	
#	After that point, your nick should be fully unregistered.
#	
#	We'll reserve this code for 24 hours from the date of this E-mail.  If you
#	don't complete the AUTH process before then, you'll need to begin again
#	from the start.
#	
#	If you have received this E-mail in error, and know nothing about what is
#	going on, please report the matter to %s.
#	
#	Thank you for using the %s IRC Network.
#
#NICK_SENDPASS_AUTH_MAIL
#	From: %s
#	To: %s <%s>
#	Subject: OFTC Password Retrieval Request
#	X-Abuse: %s
#	
#	Hi!  Someone with the following nick, ident and host:
#	
#	    %s!%s@%s
#	
#	has just requested a password retrieval for the nickname '%s' on the
#	OFTC IRC Network.  As per our password retrieval policies, we have
#	sent a code to the email address we have on record for that nick name.
#	
#	To complete the process, you need to return to OFTC, and give the
#	following command:
#	
#	/msg NickServ SENDPASS AUTH %s
#	
#	After that point, you will receive another email containing your
#	password details.
#	
#	If you have received this email in error or know nothing about what is
#	going on, please DO NOT PANIC!  Your password has not yet been revealed
#	to anyone or changed in any way.  Other than reporting the matter to
#	%s, you can simply ignore this email.
#	
#	Thank you for using the %s IRC Network.
#
#CHAN_SENDPASS_AUTH_MAIL
#	From: %s
#	To: %s <%s>
#	Subject: OFTC Password Retrieval Request
#	X-Abuse: %s
#	
#	Hi!  Someone with the following nick, ident and host:
#	
#	    %s!%s@%s
#	
#	has just requested a password retrieval for the channel '%s' on the
#	%s IRC Network.  As per our password retrieval policies, we have
#	sent a code to the email address we have on record for the founder
#	of that channel.
#	
#	To complete the process, you need to return to OFTC, and give the
#	following command:
#	
#	/msg ChanServ SENDPASS AUTH %s
#	
#	After that point, you will receive another email containing your
#	password details.
#	
#	If you have received this email in error or know nothing about what is
#	going on, please DO NOT PANIC!  Your password has not yet been revealed
#	to anyone or changed in any way.  Other than reporting the matter to
#	%s, you can simply ignore this email.
#	
#	Thank you for using the %s IRC Network.
#
#NICK_SENDPASS_AUTH_MAILED
#	You have requested a password retrieval for %s; this requires
#	you to use an AUTH code.
#	
#	An AUTH code has been E-mailed to the address we have on record for
#	%s.  You will need to read that email and then execute the
#	command:
#	
#	/msg NickServ SENDPASS AUTH code
#	
#	Where code is the code you will find in the E-mail.
#
#NICK_REG_AUTH_MAILED
#	An AUTH code has been E-mailed to %s.  You will need to read
#	that email and then execute the command:
#	
#	/msg NickServ REGISTER AUTH code
#	
#	Where code is the code you will find in the E-mail.
#
#NICK_DROP_AUTH_MAILED
#	An AUTH code has been E-mailed to %s.  You will need to read
#	that email and then execute the command:
#	
#	/msg NickServ DROP AUTH code
#	
#	Where code is the code you will find in the E-mail.
#
NICK_BEG_SET_EMAIL
	Bitte setze eine E-Mail. Du kannst keine verlorenen Passwörter ohne gültige E-Mail bekommen.
	Gib /msg %s SET EMAIL deine@email-adresse.de um eine E-Mail Adresse zu setzen
NICK_NOT_IRCOP
	Du hast nicht die IRCOP Eigenschaft für deinen Nick gesetzt.
	Tu das bitte mit /msg %s SET IRCOP ON.
NICK_NOT_REGISTERED
	Dein Nick ist nicht registriert.
NICK_NOT_REGISTERED_HELP
	Dein Nick ist nicht registriert.  Gib /msg %s HELP für Informationen zum Registrieren ein.
NICK_X_MARKED
	Der Nick %s is markiert, du kannst kein SETPASS oder SENDPASS auf ihn anwenden.
NICK_X_NOT_REGISTERED
	Der Nick %s ist nicht registriert.
NICK_X_NOT_IN_USE
	Der Nick %s ist momentan nicht in Benutzung.
NICK_X_FORBIDDEN
	Der Nick %s könnte nicht registriert oder in Benutztung sein. Das bezieht die Benutzung von jeglichen Diensten für verbundene Befehle mit ein.
NICK_X_SUSPENDED
	Der Nick %s is suspendiert und ist vom einloggen ausgeschlossen. 
NICK_IDENTIFY_REQUIRED
	Passwortbestätigung für diesen Befehl benötigt.
	Versuche es nach Eingabe von /msg %s IDENTIFY password nochmal.
NICK_X_NOOP
	%s möchte nicht zur Channel Access List hinzugefügt werden.

CHAN_X_NOT_REGISTERED
	Channel %s ist nicht registriert.
CHAN_X_NOT_IN_USE
	Channel %s existiert nicht.
CHAN_X_FORBIDDEN
	Channel %s ist nicht registriert oder nicht in Benutzung.
CHAN_IDENTIFY_REQUIRED
	Passwortbestätigung für diesen Befehl benötigt.
	Versuche es nach Eingabe von /msg %s IDENTIFY %s password nochmal.


###########################################################################
#
# NickServ messages
#
###########################################################################
#
# Automatic messages
NICK_IS_REGISTERED
	Dieser Nick gehört bereits jemanden. Bitte wähle einen anderen.
	(Ist es dein Nick, gib /msg %s IDENTIFY passwort ein)
NICK_IS_SECURE
	Dieser Nick ist registriert und geschützt.  Sollte es deiner
	sein, gib bitte /msg %s IDENTIFY passwort ein.
	Ansonsten wähle einen anderen.
NICK_MAY_NOT_BE_USED
	Dieser Nick darf nicht benutzt werden. Bitte wähle einen anderen.
FORCENICKCHANGE_IN_1_MINUTE
	Wenn du deinen Nick nicht innerhalb von einer Minute änderst, wird er für dich geändert.
FORCENICKCHANGE_IN_20_SECONDS
	Wenn du ihn nicht innerhalb von 20 Sekunden änderst, wird er für dich geändert.
FORCENICKCHANGE_NOW
	Dieser Nick wurde registriert - du darfst ihn nicht benutzen.
	Dein Nick wurde in %s geändert.
#
# REGISTER responses
NICK_REGISTER_AUTH_SYNTAX
	REGISTER Passwort E-mail
	        REGISTER AUTH code
NICK_REGISTER_SYNTAX
	REGISTER Passwort E-mail
NICK_REGISTRATION_FAILED
	Registrierung fehlgeschlagen.
NICK_REG_PLEASE_WAIT
	Bitte warte %d Sekunden bevor du REGISTER wieder benutzt.
NICK_CANNOT_BE_REGISTERED
	Nickname %s konnte nicht registriert werden.
NICK_ALREADY_REGISTERED
	Nickname %s ist schon registriert!
NICK_REGISTERED
	Nickname %s unter deinem Konto registriert: %s
NICK_PASSWORD_IS
	Dein Passwort ist %s - merke es dir für später.
NICK_REGISTER_NEWNICK
	Für mehr Informationen über den Besitz eines eigenen Nicks, geh bitte zu:
	%s
NICK_REG_NO_SUCH_AUTH
	Tut uns leid, aber wir haben keine Aufzeichnungen dieses AUTH 
	Codes. Bitte überprüfe, dass du ihn richtig geschrieben hast.
	
	Um einen AUTH Code zu bekommen, musst du vorher den REGISTER 
	Befehl benutzt und uns dein PAsswort sowie die email Adresse
	gegeben haben. Ist das einmal getan, wird dir der AUTH Code 
	gemailt.
NICK_ALREADY_PART_REGISTERED
	Nickname %s wurde bereits Teil-Registriert und wartet auf AUTH.
	Wenn der jetzige Besitzer die REgistration nicht innerhalb von 24
	Stunden abschließt, wird der Nick wieder verfügbar sein.
#
# IDENTIFY responses
NICK_IDENTIFY_SYNTAX
	IDENTIFY Passwort
NICK_IDENTIFY_FAILED
	Tut uns leid, aber die Identifizierung schlug fehl.
NICK_IDENTIFY_SUCCEEDED
	Passwort angenommen - du bist nun eingeloggt.
#
# DROP responses
NICK_DROP_SYNTAX
	DROP
NICK_DROPPED
	Dein Nick wurde abgelegt.
NICK_X_DROPPED
	Nickname %s wurde abgelegt.
NICK_DROP_NO_SUCH_AUTH
	Tut uns leid, aber wir haben keine Aufzeichnungen dieses AUTH 
	Codes. Bitte überprüfe, dass du ihn richtig geschrieben hast.
	
	Um einen AUTH Code zu bekommen, musst du vorher den DROP 
	Befehl benutzt haben. Ist das einmal getan, wird dir der AUTH 
	Code gemailt.
NICK_ALREADY_PART_DROPPED
	Nickname %s wurde bereits Teil-Abgelegtund wartet auf AUTH.
	Um einen anderen AUTH Code zu erhalten, musst du 24 Stunden
	warten und den DROP Prozess abermals starten.
# SET responses
NICK_SET_SYNTAX
	SET Option Parameter
NICK_SET_SERVADMIN_SYNTAX
	SET [nick] Pption Parameter
NICK_SET_UNKNOWN_OPTION
	Unbekannte SET Option %s.
NICK_SET_UNKNOWN_OPTION_OR_BAD_NICK
	Unbekannte SET Option %s, oder Nick nicht registriert.
#
# SET AUTOJOIN responses
NICK_SET_AUTOJOIN_ON
	AutoJoin ist nun angeschaltet.
NICK_SET_AUTOJOIN_OFF
	AutoJoin ist nun ausgeschaltet.
NICK_SET_AUTOJOIN_SYNTAX
	SET AUTOJOIN {ON | OFF}
#
# SET PASSWORD responses
NICK_SET_PASSWORD_CHANGED
	Passwort geändert.
NICK_SET_PASSWORD_CHANGED_TO
	Password in %s geändert.
#
# SET LANGUAGE responses
NICK_SET_LANGUAGE_SYNTAX
	SET LANGUAGE Nummer
NICK_SET_LANGUAGE_UNKNOWN
	Unbekannte Sprach-Nummer %d.  Gib /msg %s HELP SET LANGUAGE für eine Sprachliste ein.
NICK_SET_LANGUAGE_CHANGED
	Sprache in Deutsch geändert.
#
# SET URL responses
NICK_SET_URL_CHANGED
	URL in %s geändert.
NICK_SET_URL_UNSET
	URL gelöscht.
#
# SET EMAIL responses
NICK_SET_EMAIL_CHANGED
	E-mail Addresse auf %s gesetzt.  Gib /msg %s HELP SET HIDE für Informationen über das Verstecken deiner E-mail ein.
NICK_SET_EMAIL_UNCHANGED
	E-mail Addresse bleibt bei %s.
NICK_SET_EMAIL_MANDATORY
	Alle Nicks müssen eine gültige E-Mail gesetzt haben. Gib /msg %s HELP SET HIDE für Informationen über das Verstecken deiner E-Mail ein.
#
# SET ENFORCE responses
NICK_SET_ENFORCE_SYNTAX
	SET ENFORCE {ON | QUICK | OFF}
NICK_SET_ENFORCE_ON
	Nick-Zwang ist nun eingeschaltet.
NICK_SET_ENFORCE_QUICK
	Nick-Zwang ist nun mit einer reduzierten Zwangsverzögerung eingeschaltet.
NICK_SET_ENFORCE_OFF
	Nick-Zwang ist nun ausgeschaltet.
#
# SET SECURE responses
NICK_SET_SECURE_SYNTAX
	SET SECURE {ON | OFF}
NICK_SET_SECURE_ON
	Secure-Option ist nun eingeschaltet.
NICK_SET_SECURE_OFF
	Secure-Option ist nun ausgeschaltet.
#
# SET PRIVATE responses
NICK_SET_PRIVATE_SYNTAX
	SET PRIVATE {ON | OFF}
NICK_SET_PRIVATE_ON
	Private-Option ist nun eingeschaltet.
NICK_SET_PRIVATE_OFF
	Private-Option ist non ausgeschaltet.
#
# SET HIDE responses
NICK_SET_HIDE_SYNTAX
	SET HIDE {EMAIL | USERMASK | QUIT} {ON | OFF}
NICK_SET_HIDE_EMAIL_ON
	Deine E-mail Addresse wird nun in %s INFO Anzeigen versteckt.
NICK_SET_HIDE_EMAIL_OFF
	Deine E-mail Addresse wird nun in %s INFO Anzeigen gezeigt.
NICK_SET_HIDE_MASK_ON
	Deine zuletzt gesehene user@host Maske wird nun in %s INFO Anzeigen versteckt.
NICK_SET_HIDE_MASK_OFF
	Deine zuletzt gesehene user@host Maske wird nun in %s INFO Anzeigen gezeigt.
NICK_SET_HIDE_QUIT_ON
	Deine zuletzt gesehene Quit-Nachricht wird nun in %s INFO Anzeigen versteckt.
NICK_SET_HIDE_QUIT_OFF
	Deine zuletzt gesehene Quit-Nachricht wird nun in %s INFO Anzeigen gezeigt.
# SET NOEXPIRE responses
NICK_SET_NOEXPIRE_SYNTAX
	SET [nickname] NOEXPIRE {ON | OFF}
NICK_SET_NOEXPIRE_ON
	Nick %s wird nicht auslaufen.
NICK_SET_NOEXPIRE_OFF
	Nick %s wirdl auslaufen.
#
# SET NOOP responses
NICK_SET_NOOP_SYNTAX
	SET [nickname] NOOP {ON | OFF}
NICK_SET_NOOP_ON
	Nick %s kann nun nicht zur Access-Liste hinzugefügt werden.
NICK_SET_NOOP_OFF
	Nick %s kann nun zur Access-Liste hinzugefügt werden.
#
# SET IRCOP responses
NICK_SET_IRCOP_SYNTAX
	SET [nickname] IRCOP {ON | OFF}
NICK_SET_IRCOP_ON
	Nick %s ist nun als IRC Operator eingeloggt.
NICK_SET_IRCOP_OFF
	Nick %s ist nichtmehr als IRC Operator eingeloggt.
#
# SET MARK responses
NICK_SET_MARK_SYNTAX
	SET [nickname] MARK {ON | OFF}
NICK_SET_MARK_ON
	Nick %s ist nun markiert.
NICK_SET_MARK_OFF
	Nick %s ist nichtmehr markiert.
#
# SET REGTIME responses
NICK_SET_REGTIME_SYNTAX
	SET [nickname] REGTIME JJJJ-MM-DD-HH-MM-SS
NICK_SET_REGTIME
	Registrierungszeit von %s wurde auf %s gesetzt.
#
# SET SENDPASS responses
NICK_SET_SENDPASS_SYNTAX
	SET [nickname] SENDPASS {ON | OFF}
NICK_SET_SENDPASS_ON
	Nick %s hat SENDPASS eingeschaltet.
NICK_SET_SENDPASS_OFF
	Nick %s hat SENDPASS ausgeschaltet.  ACHTUNG: Verlierst du dein
	Nick Passwort solange SENDPASS ausgeschaltet ist, wird dein Nick
	gewiß auch verloren gehen!
#
# ACCESS responses
NICK_ACCESS_SYNTAX
	ACCESS {ADD | DEL | LIST} [mask | list]
#NICK_ACCESS_ALREADY_PRESENT
	Maske %s existiert bereits auf deiner Zugriffsliste.
NICK_ACCESS_REACHED_LIMIT
	Du kannst nur %d Access Einträge für einen Nicknamen haben.
NICK_ACCESS_ADDED
	%s wurde zu deiner Zugriffsliste hinzugefügt.
NICK_ACCESS_NOT_FOUND
	%s wurde in deiner Zugriffsliste nicht gefunden.
NICK_ACCESS_DELETED
	%s wurde von deiner Zugriffsliste entfernt.
NICK_ACCESS_LIST
	Zugriffsliste:
NICK_ACCESS_LIST_X
	Zugriffsliste für %s:
NICK_ACCESS_NO_MATCH
	Keine zutreffenden Einträge in der Zugriffsliste für %s vorhanden.
NICK_ACCESS_DELETED_ONE
	1 Eintrag aus der Zugriffsliste für %s gelöscht.
NICK_ACCESS_DELETED_SEVERAL
	%d Einträge aus der Zugriffsliste für %s gelöscht.

# LINK responses
NICK_LINK_SYNTAX
	LINK Nick Passwort
NICK_LINK_DISABLED
	Der LINK Befehl ist nicht aktiviert.
NICK_LINK_FAILED
	Linken fehlgeschlagen.
NICK_NO_LINK_SAME
	Du kannst nicht zu deinem jetzigem Namen linken!
NICK_LINK_TOO_MANY_CHANNELS
	Kann nicht zu folgendem Nick linken: %s - zu viele Channels registriert.
	(Gesamt-Maximum %d).
NICK_LINKED
	Dein Nick wurde auf %s gelinkt.
NICK_X_IS_LINK
	Nick %s ist ein link zu %s, du kannst keine unterschiedlichen
	Ebenen von Links haben!
NICK_X_HAS_LINKS
	Nick %s hat bereits %u Nicks auf sich gelinkt; du kannst keine 
	unterschiedlichen Ebenen von Links haben!

# UNLINK responses
NICK_UNLINK_SYNTAX
	UNLINK [Nick Passwort]
NICK_UNLINK_FAILED
	Entketten fehlgeschlangen.
NICK_NOT_LINKED
	Dein nicht ist nicht gelinkt.
NICK_UNLINKED
	Dein Nick wurde von %s entkettet.
NICK_X_NOT_LINKED
	Nick %s ist nicht gelinkt.
NICK_X_UNLINKED
	Nick %s wurde von %s entkettet.
#
# LISTLINKS responses
NICK_LISTLINKS_HEADER
	Links zum Nicknamen %s:
NICK_LISTLINKS_FOOTER
	%d Nicknamen mit Link.
NICK_X_IS_LINKED
	    %s

# LISTCHANS responses
NICK_LISTCHANS_FOUNDER_HEADER
	%s ist Gründer von:
NICK_LISTCHANS_ACCESS_HEADER
	%s hat Zugriff in:
	  Channel                      Zugriff
NICK_LISTCHANS_FOOTER
	Gründer von %d channel%s, Zugriff in %d Channel%s.
#@grifferz: Not sure what this does :/
#NICK_X_IS_FOUNDER
#	  %-30s
#NICK_X_HAS_ACCESS
#	  %-30s %4d
#
# INFO responses
NICK_INFO_SYNTAX
	INFO Nickname [ALL]
NICK_INFO_REALNAME
	*** %s ist %s
NICK_INFO_ADDRESS
	Letzte Addresse: %s
NICK_INFO_ADDRESS_ONLINE
	        Ist online als: %s
NICK_INFO_ADDRESS_ONLINE_NOHOST
	%s ist momentan online.
NICK_INFO_TIME_REGGED
	    Registierungsdatum: %s (%s her)
NICK_INFO_LAST_SEEN
	Das letzte Mal gesehen: %s (%s her)
NICK_INFO_LAST_QUIT
	 Letzte QUIT-Nachricht: (%s)
NICK_INFO_LAST_QUIT_TIME
	       Der letzte QUIT: %s (%s her)
NICK_INFO_URL
	                   URL: %s
NICK_INFO_EMAIL
	        E-Mail-Adresse: %s
NICK_INFO_OPTIONS
	              Optionen: %s
NICK_INFO_LINKED_TO
	            Gelinkt zu: %s
# These strings MUST NOT be empty
NICK_INFO_OPT_ENFORCE
	Übernahmeschutz
NICK_INFO_OPT_SECURE
	Sicherheit
NICK_INFO_OPT_PRIVATE
	Privat
NICK_INFO_OPT_NONE
	Keine
NICK_INFO_OPT_NOOP
	Kein Op
NICK_INFO_OPT_IRCOP
	Oper
NICK_INFO_OPT_MARKED
	Markiert
NICK_INFO_OPT_NOSENDPASS
	Kein SendPass
NICK_INFO_NO_EXPIRE
	Dieser Nick wird nicht verfallen.
NICK_INFO_SUSPEND_DETAILS
	Suspendiert von %s am %s (%s).
NICK_INFO_SUSPEND_REASON
	Grund: %s
NICK_INFO_SHOW_ALL
	Für weitere Informationen: /msg %s INFO %s ALL

# LIST responses
NICK_LIST_SYNTAX
	LIST Form
NICK_LIST_SERVADMIN_SYNTAX
	LIST Form [FORBIDDEN] [IRCOP] [NOEXPIRE] [SUSPENDED]
NICK_LIST_HEADER
	Liste von Einträgen, zutreffend auf %s:
NICK_LIST_RESULTS
	End der Liste - %d/%d Treffer gezeigt.
#
# RECOVER responses
NICK_RECOVER_SYNTAX
	RECOVER Nick [Passwort]
NICK_NO_RECOVER_SELF
	Du kannst dich nicht selbst töten!
NICK_RECOVERED
	Der User mit deinem Nick wurde aus dem Netzwerk entfernt.
	Mit /msg %s RELEASE %s [Passwort] kannst du deinen Nick in
	der Zeit von einer Minute zurückerhalten.

# RELEASE responses
NICK_RELEASE_SYNTAX
	RELEASE Nick [Passwort]
NICK_RELEASE_NOT_HELD
	Nick %s wird nicht beibehalten.
NICK_RELEASED
	Nick wurde freigestellt.

# GHOST responses
NICK_GHOST_SYNTAX
	GHOST Nick [Passwort]
NICK_NO_GHOST_SELF
	Du kannst dich nicht selbst töten!
NICK_GHOST_KILLED
	Die Verbindung des hängengebliebenen Nicks wurde unterbrochen.

# REGAIN responses
NICK_REGAIN_SYNTAX
	REGAIN Nick Passwort
NICK_NO_REGAIN_SELF
	Du kannst deinen eigenen Nick nicht zurückerlangen!
NICK_REGAIN_REGAINED
	Du hast deinen Nick zurückerlangt.

# SENDPASS responses
NICK_SENDPASS_SYNTAX
	SENDPASS [Nickname]
	SENDPASS AUTH Code
NICK_X_HAS_NO_EMAIL
	SENDPASS ist nicht verfügbar, weil %s keine E-Mail-Adresse angegeben hat.
NICK_X_SENDPASSED
	Das NickServ Passwort für %s wurde zugemailt.
NICK_SENDPASS_NO_SUCH_AUTH
	Leider haben wir keinen Eintrag mit diesem Authentifikations-Code.
	Bitte überprüfe, ob du ihn korrekt geschrieben hast.
	
	Um einen Authentifikations-Code zu bekommen musst du vorher den
	SENDPASS-Befehl benutzt haben. Sobald du das tust, wird dir der
	Code zugemailt.
NICK_SENDPASS_ALREADY_REQUESTED
	Ein SENDPASS wurde bereits für %s beantragt. Wenn du das Passwort
	für diesen Nicknamen benötigst frage bitte einen Services Admin.
NICK_X_DISABLED_SENDPASS
	%s hat SENDPASS für diesen Nickname deaktiviert.

# FORBID responses
NICK_FORBID_SYNTAX
	FORBID Nickname
NICK_FORBID_SUCCEEDED
	Der Nick %s ist jetzt gesperrt.
NICK_FORBID_FAILED
	Der Nick %s konnte nicht gesperrt werden!

# SUSPEND responses
NICK_SUSPEND_SYNTAX
	SUSPEND [+Dauer] Nickname Grund
NICK_SUSPEND_SUCCEEDED
	Der Nick %s und alle mit ihm verbundenen Nicks sind suspendiert.
NICK_SUSPEND_ALREADY_SUSPENDED
	Der Nick %s ist bereits suspendiert.

# UNSUSPEND responses
NICK_UNSUSPEND_SYNTAX
	UNSUSPEND nickname
NICK_UNSUSPEND_SUCCEEDED
	Suspend von %s wurde aufgehoben.
NICK_SUSPEND_NOT_SUSPENDED
	Der Nickname %s ist nicht suspendiert.

# AUTOJOIN responses
NICK_AUTOJOIN_SYNTAX
	AUTOJOIN ADD Raum
	        AUTOJOIN DEL {Raum | Liste}
	        AUTOJOIN LIST [Liste]
NICK_AUTOJOIN_LIST_HEADER
	Nummer  Raum
NICK_AUTOJOIN_LIST_FORMAT
	%3d     %s
NICK_AUTOJOIN_NO_MATCH
	Keine passenden AutoJoin - Einträge vorhanden.
NICK_AUTOJOIN_REACHED_LIMIT
	Deine AutoJoin-Liste hat ihr Limit von %u Einträgen erreicht.
NICK_AUTOJOIN_ALREADY_PRESENT
	%s ist bereits in deiner AutoJoin-Liste vorhanden.
NICK_AUTOJOIN_ADDED
	%s wurde zu deiner AutoJoin-Liste hinzugefügt.
NICK_AUTOJOIN_DELETED_ONE
	1 AutoJoin-Eintrag wurde gelöscht.
NICK_AUTOJOIN_DELETED_SEVERAL
	%u AutoJoin-Einträge wurden gelöscht.

###########################################################################
#
# ChanServ messages
#
###########################################################################
#
# Access level descriptions
CHAN_LEVEL_AUTOOP
	Autmatischer Raumoperator
CHAN_LEVEL_AUTOVOICE
	Automatischer Moderator
CHAN_LEVEL_AUTODEOP
	Raumoperator-Status nicht erlaubt
#CHAN_LEVEL_NOJOIN
#	Not allowed to join channel if RESTRICTED
#CHAN_LEVEL_INVITE
#	Allowed to use INVITE command
#CHAN_LEVEL_AKICK
#	Allowed to use AKICK command
#CHAN_LEVEL_SET
#	Allowed to use SET command (not FOUNDER/PASSWORD)
#CHAN_LEVEL_CLEAR
#	Allowed to use CLEAR command
#CHAN_LEVEL_UNBAN
#	Allowed to use UNBAN command
#CHAN_LEVEL_OPDEOP
#	Allowed to use OP/DEOP/DEVOICE commands
#CHAN_LEVEL_ACCESS_LIST
#	Allowed to view the access list
#CHAN_LEVEL_ACCESS_CHANGE
#	Allowed to modify the access list
#CHAN_LEVEL_MEMO_READ
#	Allowed to list/read channel memos
#CHAN_LEVEL_LEVEL_LIST
#	Allowed to list the current LEVEL settings
#CHAN_LEVEL_LEVEL_CHANGE
#	Allowed to change the current LEVEL settings
#CHAN_LEVEL_SYNC
#	Allowed to use SYNC command
#CHAN_LEVEL_MEMO_SEND
#	Allowed to send memos to the channel
#CHAN_LEVEL_AKICK_LIST
#	Allowed to LIST or VIEW the channel's AutoKicks
#
# Automatic responses
CHAN_IS_REGISTERED
	Dieser Raum wurde mit %s registriert.
CHAN_MAY_NOT_BE_USED
	Dieser Raum darf nicht benutzt werden.
CHAN_NOT_ALLOWED_TO_JOIN
	Dir ist es nicht erlaubt, diesen Raum zu betreten.
#CHAN_HAS_NEW_MEMOS
#	%d new memo%s ha%s been sent to %s since the last time
#	you checked.  Type /msg %s LIST %s for more details.

# REGISTER responses
CHAN_REGISTER_SYNTAX
	REGISTER Raum Passwort Beschreibung
CHAN_REGISTER_NOT_LOCAL
	Lokale Räume können nicht registriert werden.
CHAN_MUST_REGISTER_NICK
	Du must zuerst deinen Nicknamen registrieren.
	Für weitere Informationen zur Nicknamen-Registrierung gib bitte
	/msg %s HELP ein.
CHAN_MUST_IDENTIFY_NICK
	Bitte identifiziere dich erst mit %s, indem du folgenden Befehl
	benutzst: /msg %s IDENTIFY Passwort
CHAN_MAY_NOT_BE_REGISTERED
	Der Raum %s darf nicht registriert werden.
CHAN_ALREADY_REGISTERED
	Der Raum %s ist bereits registriert!
CHAN_MUST_BE_CHANOP
	Du must ein Raumoperator sein, um diesen Raum zu registrieren.
CHAN_REACHED_CHANNEL_LIMIT
	Limit der %d registrierten Räume erreicht.
CHAN_EXCEEDED_CHANNEL_LIMIT
	Limit der %d registrierten Räume überschritten.
CHAN_REGISTRATION_FAILED
	Die Registration ist leider fehlgeschlagen.
CHAN_REGISTERED
	Der Raum %s ist für deinen Nickname "%s" registriert.
CHAN_PASSWORD_IS
	Dein Raum-Passwort lautet %s - bitte vergiss es nicht.

# IDENTIFY responses
CHAN_IDENTIFY_SYNTAX
	IDENTIFY Raum Passwort
CHAN_IDENTIFY_SUCCEEDED
	Passwort akzeptiert - Du hast jetzt Gründer-Zugriff auf %s.

# DROP responses
CHAN_DROP_SYNTAX
	DROP Raum
CHAN_DROPPED
	Der Raum %s wurde freigegeben.

# SET responses
CHAN_SET_SYNTAX
	SET Raum Option Parameter
CHAN_SET_UNKNOWN_OPTION
	Unbekannte SET-Option %s.

# SET FOUNDER responses
CHAN_SET_FOUNDER_TOO_MANY_CHANS
	%s hat zu viele Räume registriert.
CHAN_FOUNDER_CHANGED
	Der Gründer vom Raum %s wurde zu %s geändert.

# SET SUCCESSOR responses
CHAN_SUCCESSOR_CHANGED
	Der Nachfolger für %s wurde in %s geändert
CHAN_SUCCESSOR_UNSET
	Der Nachfolger für %s wurde gelöscht.
CHAN_SUCCESSOR_IS_FOUNDER
	Der Nachfolger kann nicht gleichzeitig Gründer sein.

# SET PASSWORD responses
CHAN_PASSWORD_CHANGED
	Das Passwort von %s wurde geändert.
CHAN_PASSWORD_CHANGED_TO
	Das Passwort von %s wurde in %s geändert.

# SET DESC responses
#CHAN_DESC_CHANGED
#	Description of %s changed to %s.
#
# SET URL responses
#CHAN_URL_CHANGED
#	URL for %s changed to %s.
#CHAN_URL_UNSET
#	URL for %s unset.
#
# SET EMAIL responses
#CHAN_EMAIL_CHANGED
#	E-mail address for %s changed to %s.
#CHAN_EMAIL_UNSET
#	E-mail address for %s unset.
#
# SET ENTRYMSG responses
#CHAN_ENTRY_MSG_CHANGED
#	Entry message for %s changed.
#CHAN_ENTRY_MSG_UNSET
#	Entry message for %s unset.
#
# SET MLOCK responses
#CHAN_SET_MLOCK_SYNTAX
#	SET channel MLOCK { modes | OFF }
#CHAN_SET_MLOCK_KEY_REQUIRED
#	Parameter required for MLOCK +k.
#CHAN_SET_MLOCK_LIMIT_REQUIRED
#	Parameter required for MLOCK +l.
#CHAN_SET_MLOCK_LIMIT_POSITIVE
#	Parameter for MLOCK +l must be a positive number.
#CHAN_SET_MLOCK_UNKNOWN_CHAR
#	Unknown mode character %c ignored.
#CHAN_MLOCK_CHANGED
#	Mode lock on channel %s changed to %s.
#CHAN_MLOCK_REMOVED
#	Mode lock on channel %s removed.
#
# SET KEEPTOPIC responses
#CHAN_SET_KEEPTOPIC_SYNTAX
#	SET channel KEEPTOPIC {ON | OFF}
#CHAN_SET_KEEPTOPIC_ON
#	Topic retention option is now ON.
#CHAN_SET_KEEPTOPIC_OFF
#	Topic retention option is now OFF.
#
# SET TOPICLOCK responses
#CHAN_SET_TOPICLOCK_SYNTAX
#	SET channel TOPICLOCK {ON | OFF}
#CHAN_SET_TOPICLOCK_ON
#	Topic lock option is now ON.
#CHAN_SET_TOPICLOCK_OFF
#	Topic lock option is now OFF.
#
# SET PRIVATE responses
#CHAN_SET_PRIVATE_SYNTAX
#	SET channel PRIVATE {ON | OFF}
#CHAN_SET_PRIVATE_ON
#	Private option is now ON.
#CHAN_SET_PRIVATE_OFF
#	Private option is now OFF.
#
# SET SECUREOPS responses
#CHAN_SET_SECUREOPS_SYNTAX
#	SET channel SECUREOPS {ON | OFF}
#CHAN_SET_SECUREOPS_ON
#	Secure ops option is now ON.
#CHAN_SET_SECUREOPS_OFF
#	Secure ops option is now OFF.
#
# SET LEAVEOPS responses
#CHAN_SET_LEAVEOPS_SYNTAX
#	SET channel LEAVEOPS {ON | OFF}
#CHAN_SET_LEAVEOPS_ON
#	Leave ops option is now ON.
#CHAN_SET_LEAVEOPS_OFF
#	Leave ops option is now OFF.
#
# SET RESTRICTED responses
#CHAN_SET_RESTRICTED_SYNTAX
#	SET channel RESTRICTED {ON | OFF}
#CHAN_SET_RESTRICTED_ON
#	Restricted access option is now ON.
#CHAN_SET_RESTRICTED_OFF
#	Restricted access option is now OFF.
#
# SET SECURE responses
#CHAN_SET_SECURE_SYNTAX
#	SET channel SECURE {ON | OFF}
#CHAN_SET_SECURE_ON
#	Secure option is now ON.
#CHAN_SET_SECURE_OFF
#	Secure option is now OFF.
#
# SET VERBOSE responses
#CHAN_SET_VERBOSE_SYNTAX
#	SET channel VERBOSE {ON | OFF}
#CHAN_SET_VERBOSE_ON
#	Verbose option is now ON.
#CHAN_SET_VERBOSE_OFF
#	Verbose option is now OFF.
#
# SET NOEXPIRE responses
#CHAN_SET_NOEXPIRE_SYNTAX
#	SET channel NOEXPIRE {ON | OFF}
#CHAN_SET_NOEXPIRE_ON
#	Channel %s will not expire.
#CHAN_SET_NOEXPIRE_OFF
#	Channel %s will expire.
#
# SET AUTOLIMIT responses
#CHAN_SET_AUTOLIMIT_SYNTAX
#	SET channel AUTOLIMIT {ON | OFF}
#	SET channel AUTOLIMIT offset:tolerance:time period
#CHAN_SET_AUTOLIMIT_DISABLED
#	AutoLimit is now disabled
#CHAN_SET_AUTOLIMIT_CHANGED
#	Channel limit will be checked for update by %d users with a tolerance of %d, every %d minute%s.
#CHAN_SET_AUTOLIMIT_BAD_TOLERANCE
#	Limit tolerance must be lower than offset!  Tolerance reduced to %d.
#
# SET CLEARBANS responses
#CHAN_SET_CLEARBANS_SYNTAX
#	SET channel CLEARBANS {ON | OFF}
#	SET channel CLEARBANS time period
#CHAN_SET_CLEARBANS_OFF
#	ClearBans is now disabled
#CHAN_SET_CLEARBANS_CHANGED
#	Bans will expire after %u minute%s
#
# SET REGTIME responses
#CHAN_SET_REGTIME_SYNTAX
#	SET channel REGTIME YYYY-MM-DD-HH-MM-SS
#CHAN_SET_REGTIME
#	Registration time of %s has been set to %s.
#
# VOP/AOP/SOP messages
#CHAN_VOP_SYNTAX
#	VOP channel ADD nick
#CHAN_AOP_SYNTAX
#	AOP channel ADD nick
#CHAN_SOP_SYNTAX
#	SOP channel ADD nick
#
#CHAN_VOP_DISABLED
#	Adding VOP for this channel failed, auto voice is disabled.
#CHAN_AOP_DISABLED
#	Adding AOP for this channel failed, auto ops is disabled.
#CHAN_SOP_DISABLED
#	Adding SOP for this channel failed, akick is disabled.
#
# ACCESS messages
CHAN_ACCESS_SYNTAX
	ACCESS Raum {ADD | DEL | LIST | COUNT | VIEW} [Nickname [Stufe] | Eintragsliste]
CHAN_ACCESS_LEVEL_NONZERO
	Zugriffsstufe muß eine Zahl ausser 0 (Null) sein.
CHAN_ACCESS_LEVEL_RANGE
	Zugriffsstufe muß zwischen %d und %d liegen (einschließlich).
CHAN_ACCESS_NICKS_ONLY
	Die Zugriffslisten können nur registrierte Nicknamen enthalten.
CHAN_ACCESS_REACHED_LIMIT
	Limit der %d Einträge in der Zugriffsliste erreicht.
CHAN_ACCESS_LEVEL_UNCHANGED
	Zugriffsstufe von %s in der Liste von %s war bereits %d.
CHAN_ACCESS_LEVEL_CHANGED
	Zugriffsstufe von %s in der Liste von %s wurde in %d geändert.
CHAN_ACCESS_ADDED
	%s wurde der Zugriffsliste von %s mit der Stufe %d hinzugefügt.
#CHAN_ACCESS_NO_SUCH_ENTRY
#	No such entry (#%d) on %s access list.
#CHAN_ACCESS_NOT_FOUND
#	%s not found on %s access list.
#CHAN_ACCESS_NO_MATCH
#	No matching entries on %s access list.
#CHAN_ACCESS_DELETED
#	%s deleted from %s access list.
#CHAN_ACCESS_DELETED_ONE
#	Deleted 1 entry from %s access list.
#CHAN_ACCESS_DELETED_SEVERAL
#	Deleted %d entries from %s access list.
#CHAN_ACCESS_LIST_EMPTY
#	%s access list is empty.
#CHAN_ACCESS_LIST_HEADER
#	Access list for %s:
#	  Num   Lev  Nick
#CHAN_ACCESS_LIST_FORMAT
#	  %3d  %4d  %s%s%s%s
#CHAN_ACCESS_VIEW_HEADER
#	Access list for %s:
#	Num   Lev  Nick
#CHAN_ACCESS_VIEW_FORMAT1
#	%3d  %4d  %s%s%s%s
#CHAN_ACCESS_VIEW_FORMAT2
#	 Added by %s, last modified by %s
#CHAN_ACCESS_VIEW_FORMAT2_NOMODIFY
#	 Added by %s
#CHAN_ACCESS_VIEW_FORMAT3
#	 Added on: %s (%s ago)
#CHAN_ACCESS_VIEW_FORMAT4
#	 Modified: %s (%s ago)
#CHAN_ACCESS_VIEW_FORMAT5
#	Last used: %s (%s ago)
#	 
#CHAN_ACCESS_VIEW_FORMAT5_NEVER_USED
#	Last used: Never
#	 
#CHAN_ACCESS_COUNT
#	%s has %d entries on its access list.
#
# AKICK responses
#CHAN_AKICK_SYNTAX
#	AKICK channel {ADD | DEL | LIST | VIEW | ENFORCE | COUNT} [mask | list]
#CHAN_AKICK_ALREADY_EXISTS
#	%s already exists on %s autokick list.
#CHAN_AKICK_REACHED_LIMIT
#	Sorry, you can only have %d autokick masks on a channel.
#CHAN_AKICK_ADDED
#	%s added to %s autokick list.
#CHAN_AKICK_NO_MATCH
#	No matching entries on %s autokick list.
#CHAN_AKICK_DELETED
#	%s deleted from %s autokick list.
#CHAN_AKICK_DELETED_ONE
#	Deleted 1 entry from %s autokick list.
#CHAN_AKICK_DELETED_SEVERAL
#	Deleted %d entries from %s autokick list.
#CHAN_AKICK_LIST_EMPTY
#	%s autokick list is empty.
#CHAN_AKICK_LIST_HEADER
#	Autokick list for %s:
#CHAN_AKICK_LIST_FORMAT
#	  %3d %s%s
#CHAN_AKICK_VIEW_FORMAT1
#	  %3d %s (by %s)
#CHAN_AKICK_VIEW_FORMAT1_UNKNOWN
#	  %3d %s
#CHAN_AKICK_VIEW_FORMAT2
#	    %s
#CHAN_AKICK_VIEW_FORMAT3
#	     Added on: %s (%s ago)
#CHAN_AKICK_VIEW_FORMAT4
#	    Last used: %s (%s ago)
#CHAN_AKICK_VIEW_FORMAT4_NEVER
#	    Last used: Never
#CHAN_AKICK_VIEW_FORMAT5
#	 Last matched: %s
#CHAN_AKICK_ENFORCE_DONE
#	AKICK ENFORCE for %s complete; %d users were affected.
#CHAN_AKICK_COUNT
#	%s has %d entries on its AKICK list.
#
# LEVELS responses
#CHAN_LEVELS_SYNTAX
#	LEVELS channel {SET | DIS[ABLE] | LIST | RESET} [item [level]]
#CHAN_LEVELS_RANGE
#	Level must be between %d and %d inclusive.
#CHAN_LEVELS_CHANGED
#	Level for %s on channel %s changed to %d.
#CHAN_LEVELS_UNKNOWN
#	Setting %s not known.  Type /msg %s HELP LEVELS DESC for a list of valid settings.
#CHAN_LEVELS_DISABLED
#	%s disabled on channel %s.
#CHAN_LEVELS_LIST_HEADER
#	Access level settings for channel %s:
#CHAN_LEVELS_LIST_DISABLED
#	    %-*s  (disabled)
#CHAN_LEVELS_LIST_NORMAL
#	    %-*s  %d
#CHAN_LEVELS_RESET
#	Access levels for %s reset to defaults.
#
# INFO responses
#CHAN_INFO_SYNTAX
#	INFO channel [ALL]
#CHAN_INFO_HEADER
#	Information for channel %s:
#CHAN_INFO_FOUNDER
#	        Founder: %s (%s)
#CHAN_INFO_NO_FOUNDER
#	        Founder: %s
#CHAN_INFO_SUCCESSOR
#	      Successor: %s (%s)
#CHAN_INFO_NO_SUCCESSOR
#	      Successor: %s
#CHAN_INFO_DESCRIPTION
#	    Description: %s
#CHAN_INFO_ENTRYMSG
#	  Entry message: %s
#CHAN_INFO_TIME_REGGED
#	     Registered: %s (%s ago)
#CHAN_INFO_LAST_USED
#	      Last used: %s (%s ago)
#CHAN_INFO_LAST_TOPIC
#	     Last topic: %s
#CHAN_INFO_TOPIC_SET_BY
#	   Topic set by: %s
#CHAN_INFO_URL
#	            URL: %s
#CHAN_INFO_EMAIL
#	 E-mail address: %s
#CHAN_INFO_OPTIONS
#	        Options: %s
#CHAN_INFO_OPT_PRIVATE
#	Private
#CHAN_INFO_OPT_KEEPTOPIC
#	Topic Retention
#CHAN_INFO_OPT_TOPICLOCK
#	Topic Lock
#CHAN_INFO_OPT_SECUREOPS
#	Secure Ops
#CHAN_INFO_OPT_LEAVEOPS
#	Leave Ops
#CHAN_INFO_OPT_RESTRICTED
#	Restricted Access
#CHAN_INFO_OPT_SECURE
#	Secure
#CHAN_INFO_OPT_VERBOSE
#	Verbose
#CHAN_INFO_OPT_NONE
#	None
#CHAN_INFO_MODE_LOCK
#	      Mode lock: %s
#CHAN_INFO_NO_EXPIRE
#	This channel will not expire.
#CHAN_INFO_AUTOLIMIT
#	      AutoLimit: Offset by %d, tolerance %d, checks every %d minute%s
#CHAN_INFO_CLEARBANS
#	      ClearBans: Bans expire after %d minute%s
#
# LIST responses
#CHAN_LIST_SYNTAX
#	LIST pattern
#CHAN_LIST_SERVADMIN_SYNTAX
#	LIST pattern [FORBIDDEN] [NOEXPIRE]
#CHAN_LIST_HEADER
#	List of entries matching %s:
#CHAN_LIST_END
#	End of list - %d/%d matches shown.
#
# INVITE responses
#CHAN_INVITE_SYNTAX
#	INVITE channel
#
# OP responses
#CHAN_OP_SYNTAX
#	OP channel [nick]
#CHAN_OP_NICK_NOT_PRESENT
#	%s is not present in %s.
#
# DEOP responses
#CHAN_DEOP_SYNTAX
#	DEOP channel [nick]
#
# VOICE responses
#CHAN_VOICE_SYNTAX
#	VOICE channel [nick]
#
# DEVOICE responses
#CHAN_DEVOICE_SYNTAX
#	DEVOICE channel [nick]
#
# UNBAN responses
#CHAN_UNBAN_SYNTAX
#	UNBAN channel
#CHAN_UNBANNED
#	You have been unbanned from %s.
#
# CLEAR responses
#CHAN_CLEAR_SYNTAX
#	CLEAR channel what
#CHAN_CLEARED_BANS
#	All bans on channel %s have been removed.
#CHAN_CLEARED_MODES
#	All modes on channel %s have been reset.
#CHAN_CLEARED_OPS
#	Mode +o has been cleared from channel %s.
#CHAN_CLEARED_VOICES
#	Mode +v has been cleared from channel %s.
#CHAN_CLEARED_USERS
#	All users have been kicked from channel %s.
#
# SENDPASS responses
#CHAN_SENDPASS_SYNTAX
#	SENDPASS channel
#	SENDPASS AUTH code
#CHAN_X_HAS_NO_EMAIL
#	SENDPASS unavailable because %s (founder of %s) does not have an E-mail address set.
#CHAN_X_SENDPASSED
#	ChanServ password for %s (founder %s) has been emailed to %s.
#CHAN_SENDPASS_NO_SUCH_AUTH
#	Sorry, we have no record of that AUTH code.  Please check that
#	you have typed it correctly.
#	
#	To get an AUTH code you must have previously used the SENDPASS
#	command.  Once this is done, the AUTH code is emailed to you.
#CHAN_SENDPASS_ALREADY_REQUESTED
#	A SENDPASS has already been requested for %s.  If you need
#	the password for this channel retrieved, please ask a Services
#	Admin.
#CHAN_SENDPASS_AUTH_MAILED
#	You have requested a password retrieval for %s; this requires
#	you to use an AUTH code.
#
#	An AUTH code has been E-mailed the address we have on record for
#	%s.  You will need to read that email and then execute the
#	command:
#	
#	/msg ChanServ SENDPASS AUTH code
#	
#	Where code is the code you will find in the E-mail.
#
#
# FORBID responses
#CHAN_FORBID_SYNTAX
#	FORBID channel
#CHAN_FORBID_SUCCEEDED
#	Channel %s is now forbidden.
#CHAN_FORBID_FAILED
#	Couldn't forbid channel %s!
#
# SYNC responses
#CHAN_SYNC_SYNTAX
#	SYNC channel
#
###########################################################################
#
# MemoServ messages
#
###########################################################################
#
# Automatic messages
MEMO_HAVE_NEW_MEMO
	Du hast 1 neue Nachricht.
MEMO_HAVE_NEW_MEMOS
	Du hast %u neue Nachrichten.
MEMO_TYPE_READ_LAST
	Gib /msg %s READ LAST ein, um sie zu lesen.
MEMO_TYPE_READ_NUM
	Gib /msg %s READ %d ein, um es zu lesen.
MEMO_TYPE_LIST_NEW
	Gib /msg %s LIST NEW ein, um eine Liste zu bekommen.
#MEMO_AT_LIMIT
#	Warning: You have reached your maximum number of memos (%d).  You will be unable to receive any new memos until you delete some of your current ones.
#MEMO_OVER_LIMIT
#	Warning: You are over your maximum number of memos (%d).  You will be unable to receive any new memos until you delete some of your current ones.
#MEMO_NEW_MEMO_ARRIVED
#	You have a new memo from %s.
#	Type /msg %s READ %d to read it.
#MEMO_NEW_CHAN_MEMO_ARRIVED
#	%s has sent a memo to %s.
#	Type /msg %s READ %s %d to read it.
#
# Multi-use responses
#MEMO_HAVE_NO_MEMOS
#	You have no memos.
#MEMO_X_HAS_NO_MEMOS
#	%s has no memos.
#
# SEND responses
MEMO_SEND_SYNTAX
	SEND {Nickname | Raum} Memo-Text
MEMO_SEND_PLEASE_WAIT
	Bitte warte %d Sekunden, bevor du den SEND-Befehl erneut benutzst.
MEMO_X_GETS_NO_MEMOS
	%s kann keine Memos empfangen.
MEMO_X_HAS_TOO_MANY_MEMOS
	%s hat momentan zu viele Memos und kann daher keine weiteren empfangen.
MEMO_SENT
	Memo an %s gesendet.

# GLOBALSEND responses
MEMO_GLOBALSEND_SERVADMIN_SYNTAX
	GLOBALSEND Memo-Text
MEMO_SERVADMIN_GLOBALSENT
	Dein Memo wurde an alle registrierte Benutzer verschickt.

# OPERSEND responses
MEMO_OPERSEND_SERVADMIN_SYNTAX
	OPERSEND Memo-Text
MEMO_SERVADMIN_OPERSENT
	Dein Memo wurde an alle registrierten IRC-Operatoren verschickt.

# CSOPSEND responses
MEMO_CSOPSEND_SERVADMIN_SYNTAX
	CSOPSEND memo-text
MEMO_SERVADMIN_CSOPSENT
	Dein Memo wurde an alle Service Operatoren verschickt.

# LIST responses
MEMO_LIST_SYNTAX
	LIST [Raum] [Liste | NEW]
MEMO_HAVE_NO_NEW_MEMOS
	Du hast keine neuen Memos.
MEMO_X_HAS_NO_NEW_MEMOS
	%s hat keine neuen Memos.
MEMO_LIST_MEMOS
	Memos für %s.  Zum Lesen /msg %s READ num eingeben.
#MEMO_LIST_NEW_MEMOS
#	New memos for %s.  To read, type: /msg %s READ num
#MEMO_LIST_CHAN_MEMOS
#	Memos for %s.  To read, type: /msg %s READ %s num
#MEMO_LIST_CHAN_NEW_MEMOS
#	New memos for %s.  To read, type: /msg %s READ %s num
#MEMO_LIST_HEADER
#	 Num  Sender            Date/Time
#MEMO_LIST_FORMAT
#	%c%3d  %-16s  %s
#
# READ responses
MEMO_READ_SYNTAX
	READ [Raum] {Liste | ALL | LAST | NEW}
MEMO_HEADER
	Memo %d von %s (%s).  Zum Löschen /msg %s DEL %d eingeben.
MEMO_CHAN_HEADER
	Memo %d von %s (%s).  Zum Löschen /msg %s DEL %s %d eingeben.
MEMO_TEXT
	%s
MEMO_GLOBAL_TEXT
	[GLOBAL] %s
MEMO_OPER_TEXT
	[OPER] %s
MEMO_CSOP_TEXT
	[CSOP] %s

# DEL responses
MEMO_DEL_SYNTAX
	DEL [Raum] {Nummer | Liste | ALL}
MEMO_DELETED_NONE
	Es wurden keine Memos gelöscht.
#@grifferz: No way to put that into service ;)
#MEMO_DELETED_X
#	%u memo%s ha%s been deleted.

# SET responses
MEMO_SET_SYNTAX
	SET Option Parameter
#MEMO_SET_UNKNOWN_OPTION
	Unbekannte SET-Option %s.

# SET NOTIFY responses
MEMO_SET_NOTIFY_SYNTAX
	SET NOTIFY {ON | LOGON | NEW | OFF}
#MEMO_SET_NOTIFY_ON
#	%s will now notify you of memos when you log on and when they are sent to you.
#MEMO_SET_NOTIFY_LOGON
#	%s will now notify you of memos when you log on or unset /AWAY.
#MEMO_SET_NOTIFY_NEW
#	%s will now notify you of memos when they are sent to you.
#MEMO_SET_NOTIFY_OFF
#	%s will not send you any notification of memos.
#
# SET LIMIT responses
MEMO_SET_LIMIT_SYNTAX
	SET LIMIT [Raum] Limit
MEMO_SET_LIMIT_SERVADMIN_SYNTAX
	SET LIMIT [User | Raum] {Limit | NONE} [HARD]
MEMO_SET_YOUR_LIMIT_FORBIDDEN
	Dir ist es nicht erlaubt, dein Memo-Limit zu ändern.
MEMO_SET_LIMIT_FORBIDDEN
	Das Memo-Limit für %s kann nicht geändert werden.
MEMO_SET_YOUR_LIMIT_TOO_HIGH
	Du kannst dein Memo-Limit nicht auf eine Zahl größer als %d setzen.
MEMO_SET_LIMIT_TOO_HIGH
	Du kannst das Memo-Limit für %s nicht höher als %d setzen.
#MEMO_SET_LIMIT_OVERFLOW
#	Memo limit too large; limiting to %d instead.
#MEMO_SET_YOUR_LIMIT
#	Your memo limit has been set to %d.
#MEMO_SET_YOUR_LIMIT_ZERO
#	You will no longer be able to receive memos.
#MEMO_UNSET_YOUR_LIMIT
#	Your memo limit has been disabled.
#MEMO_SET_LIMIT
#	Memo limit for %s set to %d.
#MEMO_SET_LIMIT_ZERO
#	Memo limit for %s set to 0.
#MEMO_UNSET_LIMIT
#	Memo limit disabled for %s.
#
# INFO responses
MEMO_INFO_NO_MEMOS
	Momentan hast du keine Memos, du armer Tropf.
MEMO_INFO_MEMO
	Du hast 1 Memo.
MEMO_INFO_MEMO_UNREAD
	Du hast 1 ungelesenes Memo.
MEMO_INFO_MEMOS
	Du hast %d Memos.
MEMO_INFO_MEMOS_ONE_UNREAD
	Du hast %d Memos, von denen 1 ungelesen ist.
MEMO_INFO_MEMOS_SOME_UNREAD
	Du hast %d Memos, von denen %d ungelesen sind.
MEMO_INFO_MEMOS_ALL_UNREAD
	Du hast %d Memos; alle von ihnen sind noch nicht gelesen worden.
MEMO_INFO_LIMIT
	Dein Memo-Limit ist %d.
MEMO_INFO_HARD_LIMIT
	Dein Memo-Limit ist %d und kann nicht geändert werden.
#MEMO_INFO_LIMIT_ZERO
#	Your memo limit is 0; you will not receive any new memos.
#MEMO_INFO_HARD_LIMIT_ZERO
#	Your memo limit is 0; you will not receive any new memos.  You cannot change this limit.
#MEMO_INFO_NO_LIMIT
#	You have no limit on the number of memos you may keep.
#MEMO_INFO_NOTIFY_OFF
#	You will not be notified of new memos.
#MEMO_INFO_NOTIFY_ON
#	You will be notified of new memos at logon and when they arrive.
#MEMO_INFO_NOTIFY_RECEIVE
#	You will be notified when new memos arrive.
#MEMO_INFO_NOTIFY_SIGNON
#	%s is notified of news memos at logon.
#MEMO_INFO_X_NO_MEMOS
#	%s currently has no memos.
#MEMO_INFO_X_MEMO
#	%s currently has 1 memo.
#MEMO_INFO_X_MEMO_UNREAD
#	%s currently has 1 memo, and it has not yet been read.
#MEMO_INFO_X_MEMOS
#	%s currently has %d memos.
#MEMO_INFO_X_MEMOS_ONE_UNREAD
#	%s currently has %d memos, of which 1 is unread.
#MEMO_INFO_X_MEMOS_SOME_UNREAD
#	%s currently has %d memos, of which %d are unread.
#MEMO_INFO_X_MEMOS_ALL_UNREAD
#	%s currently has %d memos; all of them are unread.
#MEMO_INFO_X_LIMIT
#	%s's memo limit is %d.
#MEMO_INFO_X_HARD_LIMIT
#	%s's memo limit is %d, and may not be changed.
#MEMO_INFO_X_NO_LIMIT
#	%s has no memo limit.
#MEMO_INFO_X_NOTIFY_OFF
#	%s is not notified of new memos.
#MEMO_INFO_X_NOTIFY_ON
#	%s is notified of new memos at logon and when they arrive.
#MEMO_INFO_X_NOTIFY_RECEIVE
#	%s is notified when new memos arrive.
#MEMO_INFO_X_NOTIFY_SIGNON
#	%s is notified of news memos at logon.
#
###########################################################################
#
# OperServ messages
#
###########################################################################
#
# General messages
#OPER_BOUNCY_MODES
#	Services is unable to change modes.  Are your servers configured correctly?
#OPER_BOUNCY_MODES_U_LINE
#	Services is unable to change modes.  Are your servers' U:lines configured correctly?
#
# GLOBAL responses
OPER_GLOBAL_SYNTAX
	GLOBAL Nachricht

# STATS responses
OPER_STATS_UNKNOWN_OPTION
	Unbekannte STATS Option %s.
#OPER_STATS_CURRENT_USERS
#	Current users: %d (%d ops)
#OPER_STATS_MAX_USERS
#	Maximum users: %d (%s)
# This is ugly, but at least it's language-portable...
# Note that you can include seconds in any of these--my personal preference
# is to only give resolution to the nearest minute when uptime is one hour
# or more, but just add a %d to get seconds as well.
#OPER_STATS_UPTIME_DHMS
#	Services up %d days, %02d:%02d
#OPER_STATS_UPTIME_1DHMS
#	Services up %d day, %02d:%02d
#OPER_STATS_UPTIME_HMS
#	Services up %d hours, %d minutes
#OPER_STATS_UPTIME_HM1S
#	Services up %d hours, %d minutes
#OPER_STATS_UPTIME_H1MS
#	Services up %d hours, %d minute
#OPER_STATS_UPTIME_H1M1S
#	Services up %d hours, %d minute
#OPER_STATS_UPTIME_1HMS
#	Services up %d hour, %d minutes
#OPER_STATS_UPTIME_1HM1S
#	Services up %d hour, %d minutes
#OPER_STATS_UPTIME_1H1MS
#	Services up %d hour, %d minute
#OPER_STATS_UPTIME_1H1M1S
#	Services up %d hour, %d minute
#OPER_STATS_UPTIME_MS
#	Services up %d minutes, %d seconds
#OPER_STATS_UPTIME_M1S
#	Services up %d minutes, %d second
#OPER_STATS_UPTIME_1MS
#	Services up %d minute, %d seconds
#OPER_STATS_UPTIME_1M1S
#	Services up %d minute, %d second
#OPER_STATS_BYTES_READ
#	Bytes read   : %5d kB
#OPER_STATS_BYTES_WRITTEN
#	Bytes written: %5d kB
#OPER_STATS_USER_MEM
#	User    : %6d records, %5d kB
#OPER_STATS_SERVER_MEM
#	Server  : %6d records, %5d kB
#OPER_STATS_CHANNEL_MEM
#	Channel : %6d records, %5d kB
#OPER_STATS_NICKSERV_MEM
#	NickServ: %6d records, %5d kB
#OPER_STATS_CHANSERV_MEM
#	ChanServ: %6d records, %5d kB
#OPER_STATS_MEMOSERV_MEM
#	MemoServ: %6d records, %5d kB
#OPER_STATS_OPERSERV_MEM
#	OperServ: %6d records, %5d kB
#OPER_STATS_AKILL_COUNT
#	Current number of AKILLs: %d
#OPER_STATS_AKILL_EXPIRE_DAYS
#	Default AKILL expiry time: %d days
#OPER_STATS_AKILL_EXPIRE_DAY
#	Default AKILL expiry time: 1 day
#OPER_STATS_AKILL_EXPIRE_HOURS
#	Default AKILL expiry time: %d hours
#OPER_STATS_AKILL_EXPIRE_HOUR
#	Default AKILL expiry time: 1 hour
#OPER_STATS_AKILL_EXPIRE_MINS
#	Default AKILL expiry time: %d minutes
#OPER_STATS_AKILL_EXPIRE_MIN
#	Default AKILL expiry time: 1 minute
#OPER_STATS_AKILL_EXPIRE_NONE
#	Default AKILL expiry time: No expiration
#OPER_STATS_SESSIONS_MEM
#	Sessions: %6d records, %5d kB
#
# MODE responses
OPER_MODE_SYNTAX
	MODE Raum Modes

# CLEARMODES responses
OPER_CLEARMODES_SYNTAX
	CLEARMODES Raum [ALL]
OPER_CLEARMODES_DONE
	Binäre Modes und Bans vom Raum %s entfernt.
OPER_CLEARMODES_ALL_DONE
	Alle Modes wurden vom Raum %s entfernt.


# KICK responses
#OPER_KICK_SYNTAX
	KICK Raum User Grund

# ADMIN responses
OPER_ADMIN_SYNTAX
	ADMIN {ADD | DEL | LIST} [Nickname]
OPER_ADMIN_ADD_SYNTAX
	ADMIN ADD Nickname
OPER_ADMIN_DEL_SYNTAX
	ADMIN DEL Nickname
OPER_ADMIN_EXISTS
	%s ist bereits auf der Services-Admin Liste.
#OPER_ADMIN_ADDED
#	%s added to Services admin list.
#OPER_ADMIN_TOO_MANY
#	Too many entries (%d) on Services admin list; cannot add more.
#OPER_ADMIN_REMOVED
#	%s removed from Services admin list.
#OPER_ADMIN_NOT_FOUND
#	%s not found on Services admin list.
OPER_ADMIN_LIST_HEADER
	Services-Admins:

# OPER responses
OPER_OPER_SYNTAX
	OPER {ADD | DEL | LIST} [Nickname]
OPER_OPER_ADD_SYNTAX
	OPER ADD Nickname
OPER_OPER_DEL_SYNTAX
	OPER DEL Nickname
#OPER_OPER_EXISTS
#	%s already exists on Services operator list.
#OPER_OPER_ADDED
#	%s added to Services operator list.
#OPER_OPER_TOO_MANY
#	Too many entries (%d) on Services operator list; cannot add more.
#OPER_OPER_REMOVED
#	%s removed from Services operator list.
#OPER_OPER_NOT_FOUND
#	%s not found on Services operator list.
OPER_OPER_LIST_HEADER
	Services-Operatoren:

# AKILL responses
#OPER_AKILL_SYNTAX
#	AKILL {ADD | DEL | LIST | VIEW | COUNT} [parameters]
#OPER_AKILL_ADD_SYNTAX
#	AKILL ADD [+expiry] [FORCE] mask reason
#OPER_AKILL_DEL_SYNTAX
#	AKILL DEL mask
#OPER_AKILL_ONLY_DELETE_OWN
#	You may only delete AKILLs which you added.  Ask a Services Oper/Admin or the person who originally set the AKILL to delete it for you.
#OPER_TOO_MANY_AKILLS
#	AKILL list is full!
#OPER_AKILL_NO_NICK
#	Reminder: AKILL masks cannot contain nicknames; make sure you have not included a nick portion in your mask.
#OPER_AKILL_MASK_TOO_GENERAL
#	The AKILL mask is too general.
#OPER_AKILL_MASK_TOO_GENERAL_FORCE
#	The AKILL mask is too general.  Use the FORCE parameter if you really need to add this AKILL.
#OPER_AKILL_TOO_MANY_MATCHES
#	That AKILL matches too many users (%d).  Only Services Opers and Admins may add such AKILLs.
#OPER_AKILL_TOO_LONG
#	You are only permitted to add AKILLs that last up to %s.  Services Opers and Admins may add longer term AKILLs.
#OPER_AKILL_ADDED
#	%s added to AKILL list (matches %d users).
#OPER_AKILL_REMOVED
#	%s removed from AKILL list.
#OPER_AKILL_NOT_FOUND
#	%s not found on AKILL list.
#OPER_AKILL_MASK_ALREADY_AKILLED
#	%s is already on AKILL list!
#OPER_AKILL_LIST_HEADER
#	Current AKILL list:
# mask, reason
#OPER_AKILL_LIST_FORMAT
#	%-32s  %s
# mask, set-by, set-time, expires, reason
#OPER_AKILL_VIEW_FORMAT
#	%s (by %s on %s; %s)
#	    %s
#OPER_AKILL_COUNT
#	There are %d host masks on the AKILL list.
#OPER_AKILL_NO_EXPIRE
#	does not expire
#OPER_AKILL_EXPIRES_SOON
#	expires at next database update
#OPER_AKILL_EXPIRES_M
#	expires in %d minutes
#OPER_AKILL_EXPIRES_1M
#	expires in %d minute
#OPER_AKILL_EXPIRES_HM
#	expires in %d hours, %d minutes
#OPER_AKILL_EXPIRES_H1M
#	expires in %d hours, %d minute
#OPER_AKILL_EXPIRES_1HM
#	expires in %d hour, %d minutes
#OPER_AKILL_EXPIRES_1H1M
#	expires in %d hour, %d minute
#OPER_AKILL_EXPIRES_D
#	expires in %d days
#OPER_AKILL_EXPIRES_1D
#	expires in %d day
#
# SET responses
#OPER_SET_SYNTAX
#	SET option setting
#OPER_SET_IGNORE_ON
#	Ignore code will be used.
#OPER_SET_IGNORE_OFF
#	Ignore code will not be used.
#OPER_SET_IGNORE_ERROR
#	Setting for IGNORE must be ON or OFF.
#OPER_SET_DEBUG_ON
#	Services is now in debug mode.
#OPER_SET_DEBUG_OFF
#	Services is now in non-debug mode.
#OPER_SET_DEBUG_LEVEL
#	Services is now in debug mode (level %d).
#OPER_SET_DEBUG_ERROR
#	Setting for DEBUG must be ON, OFF, or a positive number.
#OPER_SET_LOGSQL_ON
#	Services is now logging SQL queries.
#OPER_SET_LOGSQL_OFF
#	Services is no longer logging SQL queries.
#OPER_SET_LOGSQL_ERROR
#	Setting for LOGSQL must be ON or OFF.
#OPER_SET_UNKNOWN_OPTION
#	Unknown option %s.
#
# JUPE responses
#OPER_JUPE_SYNTAX
#	JUPE servername [reason]
#
# RAW responses
#OPER_RAW_SYNTAX
#	RAW text
#
# RESTART responses
#OPER_CANNOT_RESTART
#	SERVICES_BIN not defined; cannot restart.  Rerun the \2configure\2 script and recompile Services to enable the RESTART command.
#
# LISTIGNORE responses
#OPER_IGNORE_LIST
#	Services ignore list:
#OPER_IGNORE_LIST_EMPTY
#	Ignore list is empty.
#
# KILLCLONES responses
#OPER_KILLCLONES_SYNTAX
#	KILLCLONES nick
#OPER_KILLCLONES_UNKNOWN_NICK
#	Could not find user %s.
#
# ROTATELOG responses
#OPER_ROTATELOG_FILE_EXISTS
#	Could not rename logfile, a file with the name %s already exists!
#OPER_ROTATELOG_RENAME_DONE
#	Logfile renamed to %s
#OPER_ROTATELOG_RENAME_FAIL
#	Logfile could not be renamed: %s
#OPER_ROTATELOG_OPEN_LOG_FAIL
#	Logging could be not be restarted: %s
#
# QUARANTINE stuff
#OPER_QUARANTINE_SYNTAX
#	QUARANTINE ADD regex reason
#	        QUARANTINE DEL {regex | list}
#	        QUARANTINE LIST [list]
#OPER_QUARANTINE_LIST_HEADER
#	Currently active Quarantines:
#	Num  Regex                Reason
#OPER_QUARANTINE_LIST_FORMAT1
#	%3d  %-20s %s
#OPER_QUARANTINE_LIST_FORMAT2
#	   Added by %s on %s
#OPER_QUARANTINE_NO_MATCH
#	No matching entries on Quarantine list.
#OPER_QUARANTINE_BROKEN_REGEX
#	%s is not a legal POSIX Extended Regular Expression.
#OPER_QUARANTINE_ALREADY_PRESENT
#	%s is already present in the Quarantine list.
#OPER_QUARANTINE_ADDED
#	%s has been added to the Quarantine list.
#OPER_QUARANTINE_CHAN_MATCHES_ONE
#	Matches %u currently in use channel.
#OPER_QUARANTINE_CHAN_MATCHES
#	Matches %u currently in use channels.
#OPER_QUARANTINE_NICK_MATCHES_ONE
#	Matches %u currently in use nick.
#OPER_QUARANTINE_NICK_MATCHES
#	Matches %u currently in use nicks.
#OPER_QUARANTINE_DELETED_ONE
#	Deleted 1 Quarantine.
#OPER_QUARANTINE_DELETED_SEVERAL
#	Deleted %u Quarantines.
#
###########################################################################
#
# Session Limiting and Exception messages
#
###########################################################################
#
# EXCEPTION responses
#OPER_EXCEPTION_SYNTAX
#	EXCEPTION {ADD | DEL | LIST | VIEW} [params]
#OPER_EXCEPTION_ADD_SYNTAX
#	EXCEPTION ADD [+expiry] mask limit reason
#OPER_EXCEPTION_DEL_SYNTAX
#	EXCEPTION DEL {mask | list}
#OPER_EXCEPTION_DISABLED
#	Session limiting is disabled.
#OPER_EXCEPTION_ALREADY_PRESENT
#	Mask %s already present on exception list.
#OPER_EXCEPTION_TOO_MANY
#	Session-limit exception list is full!
#OPER_EXCEPTION_ADDED
#	Session limit for %s set to %d.
#OPER_EXCEPTION_NO_MATCH
#	No matching entries on session-limit exception list.
#OPER_EXCEPTION_DELETED
#	%s deleted from session-limit exception list.
#OPER_EXCEPTION_DELETED_ONE
#	Deleted 1 entry from session-limit exception list.
#OPER_EXCEPTION_DELETED_SEVERAL
#	Deleted %d entries from session-limit exception list.
#OPER_EXCEPTION_LIST_HEADER
#	Current Session Limit Exception list:
# mask, limit
#OPER_EXCEPTION_LIST_FORMAT
#	%3d  %4d   %s
#OPER_EXCEPTION_LIST_COLHEAD
#	Num  Limit  Host
# mask, set-by, set-time, expires, limit, reason
#OPER_EXCEPTION_VIEW_FORMAT
#	%3d.  %s  (by %s on %s; %s)
#	    Limit: %-4d  - %s
#OPER_EXCEPTION_INVALID_LIMIT
#	Invalid session limit. It must be a valid integer greater than or equal to zero and less than %d.
#OPER_EXCEPTION_INVALID_HOSTMASK
#	Invalid hostmask. Only real hostmasks are valid - exceptions are not matched against nicks or usernames.
#
# SESSION responses
#OPER_SESSION_SYNTAX
#	SESSION {LIST limit | VIEW host}
#OPER_SESSION_LIST_SYNTAX
#	SESSION LIST limit
#OPER_SESSION_VIEW_SYNTAX
#	SESSION VIEW host
#OPER_SESSION_DISABLED
#	Session limiting is disabled.
#OPER_SESSION_INVALID_THRESHOLD
#	Invalid threshold value. It must be a valid integer greater than 1.
#OPER_SESSION_NOT_FOUND
#	%s not found on session list.
#OPER_SESSION_LIST_HEADER
#	Hosts with at least %d sessions:
#OPER_SESSION_LIST_COLHEAD
#	Sessions  Limit  Host
# limit, host
#OPER_SESSION_LIST_FORMAT
#	%6d   %6d  %s
# host, sessions, limit
#OPER_SESSION_VIEW_FORMAT
#	The host %s currently has %d sessions with a limit of %d.
#
# EXCEPTION help
#
#OPER_HELP_EXCEPTION
#	Syntax: EXCEPTION ADD [+expiry] mask limit reason
#	        EXCEPTION DEL {mask | list}
#	        EXCEPTION LIST [mask | list]
#	        EXCEPTION VIEW [mask | list]
#	
#	Allows Services admins to manipulate the list of hosts that
#	have specific session limits - allowing certain machines, 
#	such as shell servers, to carry more than the default number
#	of clients at a time. Once a host reaches it's session limit,
#	all clients attempting to connect from that host will be 
#	killed. Before the user is killed, they are notified, via a
#	/NOTICE from OperServ, of a source of help regarding session
#	limiting. The content of this notice is a config setting.
#	
#	EXCEPTION ADD adds the given host mask to the exception list.
#	Note that nick!user@host and user@host masks are invalid! 
#	Only real host masks, such as box.host.dom and *.host.dom, 
#	are allowed because sessions limiting does not take nick or 
#	user names into account. limit must be a number greater than
#	or equal to zero. This determines how many sessions this host
#	may carry at a time. A value of zero means the host has an
#	unlimited session limit. See the AKILL help for details about
#	the format of the optional expiry parameter.
#	EXCEPTION DEL removes the given mask from the exception list.
#	EXCEPTION LIST and EXCEPTION VIEW show all current 
#	exceptions; if the optional mask is given, the list is limited
#	to those exceptions matching the mask. The difference is that
#	EXCEPTION VIEW is more verbose, displaying the name of the
#	person who added the exception, it's session limit, reason, 
#	host mask and the expiry date and time.
#	
#	Note that a connecting client will "use" the first exception
#	their host matches. Large exception lists and widely matching
#	exception masks are likely to degrade services' performance.
#	
#	Limited to Services admins.
#
#OPER_HELP_SESSION
#	Syntax: SESSION LIST threshold
#	        SESSION VIEW host
#	
#	Allows Services admins to view the session list.
#	
#	SESSION LIST lists hosts with atleast threshold sessions.
#	The threshold must be a number greater than 1. This is to 
#	prevent accidental listing of the large number of single 
#	session hosts.
#	SESSION VIEW displays detailed information about a specific
#	host - including the current session count and session limit.
#	The host value may not include wildcards.
#	
#	See the EXCEPTION help for more information about session
#	limiting and how to set session limits specific to certain
#	hosts and groups thereof.
#	
#	Limited to Services admins.
#
###########################################################################
#
# News system messages
#
###########################################################################

NEWS_LOGON_TEXT
	[Nachrichten - %s] %s
NEWS_OPER_TEXT
	[Oper Nachrichten - %s] %s

NEWS_LOGON_SYNTAX
	Syntax: LOGONNEWS {ADD|DEL|LIST} [Text|Nummer]
NEWS_LOGON_LIST_HEADER
	Logon-News-Einträge:
NEWS_LOGON_LIST_ENTRY
	%5d (%s von %s)
	    %s
NEWS_LOGON_LIST_NONE
	Keine Logon-Nachrichten vorhanden.
NEWS_LOGON_ADD_SYNTAX
	Syntax: LOGONNEWS ADD Text
NEWS_LOGON_ADD_FULL
	Logon-Nachrichtenliste ist voll!
NEWS_LOGON_ADDED
	Neue Logon-Nachricht eingetragen (#%d).
NEWS_LOGON_DEL_SYNTAX
	Syntax: LOGONNEWS DEL {Nummer | ALL}
NEWS_LOGON_DEL_NOT_FOUND
	Logon-Nachricht #%d konnte nicht gefunden werden!
NEWS_LOGON_DELETED
	Logon-Nachricht #%d wurde gelöscht.
NEWS_LOGON_DEL_NONE
	Keine Logon-Nachrichten zum Löschen vorhanden!
NEWS_LOGON_DELETED_ALL
	Alle Nachrichten gelöscht.

#NEWS_OPER_SYNTAX
#	Syntax: OPERNEWS {ADD|DEL|LIST} [text|num]
#NEWS_OPER_LIST_HEADER
#	Oper news items:
#NEWS_OPER_LIST_ENTRY
#	%5d (%s by %s)
#	    %s
#NEWS_OPER_LIST_NONE
#	There is no oper news.
#NEWS_OPER_ADD_SYNTAX
#	Syntax: OPERNEWS ADD text
#NEWS_OPER_ADD_FULL
#	News list is full!
#NEWS_OPER_ADDED
#	Added new oper news item (#%d).
#NEWS_OPER_DEL_SYNTAX
#	Syntax: OPERNEWS DEL {num | ALL}
#NEWS_OPER_DEL_NOT_FOUND
#	Oper news item #%d not found!
#NEWS_OPER_DELETED
#	Oper news item #%d deleted.
#NEWS_OPER_DEL_NONE
#	No oper news items to delete!
#NEWS_OPER_DELETED_ALL
#	All oper news items deleted.
#
#NEWS_HELP_LOGON
#	Syntax: LOGONNEWS ADD text
#	        LOGONNEWS DEL {num | ALL}
#	        LOGONNEWS LIST
#	
#	Edits or displays the list of logon news messages.  When a
#	user connects to the network, these messages will be sent
#	to them.  (However, no more than three messages will be
#	sent in order to avoid flooding the user.  If there are
#	more than three news messages, only the three most recent
#	will be sent.)
#	
#	LOGONNEWS LIST may be used by any IRC operator to list the
#	current news messages.  ADD and DEL may only be used by
#	Services admins.
#
#NEWS_HELP_OPER
#	Syntax: OPERNEWS ADD text
#	        OPERNEWS DEL {num | ALL}
#	        OPERNEWS LIST
#	
#	Edits or displays the list of oper news messages.  When a
#	user opers up (with the /OPER command), these messages will
#	be sent to them.  (However, no more than three messages will
#	be sent in order to avoid flooding the user.  If there are
#	more than three news messages, only the three most recent
#	will be sent.)
#	
#	OPERNEWS LIST may be used by any IRC operator to list the
#	current oper news messages.  ADD and DEL may only be used by
#	Services admins.
#
###########################################################################
#
# NickServ help messages
#
###########################################################################
#
NICK_HELP_START
	*** %S Hilfe ***

NICK_HELP_END
	*** Ende der %S Hilfe ***

NICK_HELP
	%S erlaubt dir einen Nick zu "registrieren" und hindert
	andere  daran ihn zu benutzen.  Wenn der Nick %d Tage lang
	nicht benutzt wird, läuft die Registration aus, sodass du
	ihn neu registrieren musst.  Die folgenden Befehle sind zum
	Registrieren und zum Einstellen der Nicknamen da.  Um sie
	zu benutzen, gib /msg %S Befehl ein.  Für Hilfe benutze
	/msg %S HELP Befehl.
	
	    REGISTER   Registriert einen Nick
	    IDENTIFY   Loggt dich mit dem Passwort ein
	    ACCESS     Ändert die Liste erlaubter Adressen
	    LINK       Setzt deinen Nick als Alias für einen anderen
	    SET        Setzt Optionen (Inklusive Nickname-Schutz)
	    DROP       Löst einen Nick aus der Registrierung
	    RECOVER    Entfernt einen anderen Benutzer, der deinen
	               Nick genommen hat
	    RELEASE    Erlange die Kontrolle über deinen Nick nach
	               RECOVER wieder
	    LISTLINKS  Listet Nicks auf, die ein Alias zu einem
	               Nick sind
	
	Weitere Befehle: UNLINK, GHOST, INFO, LIST, SENDPASS, STATUS,
	    REGAIN, LISTCHANS, AUTOJOIN
	
	ACHTUNG: Dieser Service ist dazu bestimmt, den Missbrauch
	von Nicks der IRC Benutzer zu verhindern.  Es ist
	NICHT dazu vorgesehen, den "Diebstahl" von Nicks zu
	erleichtern.  Missbrauch von %S wird mit einer Strafe
	geahndet, die mindestens den Verlust des Nicks zur Folge
	hat.

#NICK_HELP_EXPIRE_ZERO
#	%S allows you to "register" a nickname and
#	prevent others from using it.  The following commands allow
#	for registration and maintenance of nicknames; to use them,
#	type /msg %S command. For more information on a
#	specific command, type /msg %S HELP command.
#	
#	    REGISTER   Register a nickname
#	    IDENTIFY   Identify yourself with your password
#	    ACCESS     Modify the list of authorized addresses
#	    LINK       Make your nick an alias for another
#	    SET        Set options, including kill protection
#	    DROP       Cancel the registration of a nickname
#	    RECOVER    Remove another user who has taken your nick
#	    RELEASE    Regain custody of your nick after RECOVER
#	
#	Weitere Befehle: UNLINK, GHOST, INFO, LIST, SENDPASS, STATUS,
#	    REGAIN
#	
#	NOTICE: This service is intended to provide a way for
#	IRC users to ensure their identity is not compromised.
#	It is NOT intended to facilitate "stealing" of
#	nicknames or other malicious actions.  Abuse of %S
#	will result in, at minimum, loss of the abused
#	nickname(s).
#
#NICK_HELP_REGISTER
#	Syntax: REGISTER password E-mail
#	        REGISTER AUTH code
#	
#	Registers your nickname in the %S database.  Once
#	your nick is registered, you can use the SET and ACCESS
#	commands to configure your nick's settings as you like
#	them.  Make sure you remember the password you use when
#	registering - you'll need it to make changes to your nick
#	later.  (Note that case matters!  FIDO, Fido, and fido
#	are all different passwords!)
#	
#	Guidelines on choosing passwords:
#	
#	Passwords should not be easily guessable.  For example,
#	using your real name as a password is a bad idea.  Using
#	your nickname as a password is a much worse idea ;) and,
#	in fact, %S will not allow it.  Also, short
#	passwords are vulnerable to trial-and-error searches, so
#	you should choose a password at least 5 characters long.
#	Finally, the space character cannot be used in passwords.
#	
#	Once you have given NickServ your password and email, you will be
#	E-mailed an AUTH code.  This is a security measure designed to
#	stop your nickname from being abused, but it also means that you
#	need to tell us a real, working E-mail address in order to register
#	a nickname.  However, once your nickname is registered, you can use
#	the SET command to hide your email address should you wish to keep it
#	private.
#
#NICK_HELP_IDENTIFY
#	Syntax: IDENTIFY password
#	
#	Tells %S that you are really the owner of this
#	nick.  Many commands require you to authenticate yourself
#	with this command before you use them.  The password
#	should be the same one you sent with the REGISTER
#	command.
#
#NICK_HELP_DROP
#	Syntax: DROP
#	
#	Drops your nickname from the %S database.  A nick
#	that has been dropped is free for anyone to re-register.
#	
#	In order to use this command, you must first identify
#	with your password (/msg %S HELP IDENTIFY for more
#	information).
#
#NICK_HELP_ACCESS
#	Syntax: ACCESS ADD mask
#	        ACCESS DEL mask
#	        ACCESS LIST
#	
#	Modifies or displays the access list for your nick.  This
#	is the list of addresses which will be automatically
#	recognized by %S as allowed to use the nick.  If
#	you want to use the nick from a different address, you
#	need to send an IDENTIFY command to make %S
#	recognize you.
#	
#	Examples:
#	
#	    ACCESS ADD achurch@*.dragonfire.net
#	        Allows access to user achurch from any machine in
#	        the dragonfire.net domain.
#	
#	    ACCESS DEL achurch@*.dragonfire.net
#	        Reverses the previous command.
#	
#	    ACCESS LIST
#	        Displays the current access list.
#
#NICK_HELP_LINK
#	Syntax: LINK nick password
#	
#	Links your nickname to another, effectively making the nick
#	you are currently using an alias for the given nick.  When
#	you give this command, the access list for the nick you are
#	using is deleted and replaced by that of the nick you are
#	linking to; all memos for your current nick are added to
#	the list of memos for the target nick (this may cause the
#	nick to exceed its limit of memos, in which case you will
#	need to delete some before you can receive more for either
#	nick).
#	
#	Once the link has been established, your nick will be
#	transparently converted into the target nick everywhere in
#	Services, except for the %S UNLINK and DROP commands.  For
#	example, you can use either nick to read the same set of
#	memos (and memos sent to either nick will go into the same
#	list).  If you identify for one nick, you will
#	automatically be identified for the other as well.  If
#	either nick is on a channel's access list, then both nicks
#	will get the same access privileges.
#	
#	In order to use this command, you must identify for your
#	current nick (using the IDENTIFY command), and you must
#	supply the password for the nick you wish to link to.
#
#NICK_HELP_UNLINK
#	Syntax: UNLINK [nick pass]
#	
#	Removes a link from your nick to another.  When the link is
#	removed, the nickname flags and access list of the
#	previously linked nick are copied to your current nick and
#	can then be independently changed.  Memos are not copied;
#	all memos will remain stored with the previously linked
#	nick.
#	
#	By giving a nickname and password as parameters, you can
#	unlink a nick besides the one you are currently using.
#
#NICK_HELP_SENDPASS
#	Syntax: SENDPASS [nick]
#	        SENDPASS code
#	
#	E-mails nick's password to the address set using
#	SET EMAIL.
#
#NICK_HELP_SET
#	Syntax: SET option parameters
#	
#	Sets various nickname options.  option can be one of:
#	
#	    PASSWORD   Set your nickname password
#	    LANGUAGE   Set the language Services will use when
#	                   sending messages to you
#	    URL        Associate a URL with your nickname
#	    EMAIL      Associate an E-mail address with your nickname
#	    ENFORCE    Turn nickname enforcement on or off
#	    SECURE     Turn nickname security on or off
#	    PRIVATE    Prevent your nickname from appearing in a
#	                   /msg %S LIST
#	    HIDE       Hide certain pieces of nickname information
#	    NOOP       Prevent others from adding you to channel
#	                   ACCESS lists.
#	    IRCOP      Mark a nick as an IRC operator.
#	    SENDPASS   Allow or deny use of the SENDPASS command
#	                   on this nickname
#	    AUTOJOIN   Turn AutoJoin functionality on or off
#	
#	In order to use this command, you must first identify
#	with your password (/msg %S HELP IDENTIFY for more
#	information).
#	
#	Type /msg %S HELP SET option for more information
#	on a specific option.
#
#NICK_HELP_SET_AUTOJOIN
#	Syntax: SET AUTOJOIN {ON | OFF}
#	
#	Enable or disable AutoJoin functionality.
#	See /msg %S HELP AUTOJOIN for more details on this feature.
#
#NICK_HELP_SET_PASSWORD
#	Syntax: SET PASSWORD new-password
#	
#	Changes the password used to identify you as the nick's
#	owner.
#
#NICK_HELP_SET_LANGUAGE
#	Syntax: SET LANGUAGE number
#	
#	Changes the language Services uses when sending messages to
#	you (for example, when responding to a command you send).
#	number should be chosen from the following list of
#	supported languages:
#	
#
#NICK_HELP_SET_URL
#	Syntax: SET URL url
#	
#	Associates the given URL with your nickname.  This URL
#	will be displayed whenever someone requests information
#	on your nick with the INFO command.
#
#NICK_HELP_SET_EMAIL
#	Syntax: SET EMAIL address
#	
#	Associates the given E-mail address with your nickname.
#	This address will be displayed whenever someone requests
#	information on the channel with the INFO command.
#
#NICK_HELP_SET_ENFORCE
#	Syntax: SET ENFORCE {ON | QUICK | OFF}
#	
#	Turns the automatic enforcement option for your nick
#	on or off.  With nick enforcement on, if another user
#	tries to take your nick, they will be given one minute to
#	change to another nick, after which their nick will be
#	forcibly changed by %S.
#	
#	If you select QUICK, the user will be given only 20 seconds
#	to change nicks instead of the usual 60.
#
#NICK_HELP_SET_SECURE
#	Syntax: SET SECURE {ON | OFF}
#	
#	Turns %S's security features on or off for your
#	nick.  With SECURE set, you must enter your password
#	before you will be recognized as the owner of the nick,
#	regardless of whether your address is on the access
#	list.  However, if you are on the access list, %S
#	will not auto-enforce you regardless of the setting of the
#	ENFORCE option.
#
#NICK_HELP_SET_PRIVATE
#	Syntax: SET PRIVATE {ON | OFF}
#	
#	Turns %S's privacy option on or off for your nick.
#	With PRIVATE set, your nickname will not appear in
#	nickname lists generated with %S's LIST command.
#	(However, anyone who knows your nickname can still get
#	information on it using the INFO command.)
#
#NICK_HELP_SET_NOOP
#	Syntax: SET NOOP {ON | OFF}
#	
#	With NOOP set, it will not be possible for anyone to add
#	your nickname to any channel ACCESS list.
#
#NICK_HELP_SET_IRCOP
#	Syntax: SET IRCOP {ON | OFF}
#	
#	Identifies this nick as an IRC operator.
#
#NICK_HELP_SET_HIDE
#	Syntax: SET HIDE {EMAIL | USERMASK | QUIT} {ON | OFF}
#	
#	Allows you to prevent certain pieces of information from
#	being displayed when someone does a %S INFO on your
#	nick.  You can hide your E-mail address (EMAIL), last seen
#	user@host mask (USERMASK), and last quit message (QUIT).
#	The second parameter specifies whether the information should
#	be displayed (OFF) or hidden (ON).
#
#NICK_HELP_SET_SENDPASS
#	Syntax: SET SENDPASS {ON | OFF}
#	
#	Allows you to prevent the %S SENDPASS command being used
#	on this nickname.  CAUTION: The SENDPASS command exists
#	to allow you to securely regain control of your nickname should
#	you lose your nick password.  If you set this option to ON,
#	we will not be able to retrieve your password for you and loss
#	of password will mean loss of nickname in almost all cases.
#
#NICK_HELP_RECOVER
#	Syntax: RECOVER nickname [password]
#	
#	Allows you to recover your nickname if someone else has
#	taken it; this does the same thing that %S does
#	automatically if someone tries to use an enforce-protected
#	nick.
#	
#	When you give this command, %S will bring a fake
#	user online with the same nickname as the user you're
#	trying to recover your nick from.  This causes the IRC
#	servers to disconnect the other user.  This fake user will
#	will remain online for one minute to ensure that the other
#	user does not immediately reconnect; after that minute, you
#	can reclaim your nick.  Alternatively, use the RELEASE
#	command (/msg %S HELP RELEASE) to get the nick
#	back sooner.
#	
#	In order to use the RECOVER command for a nick, your
#	current address as shown in /WHOIS must be on that nick's
#	access list, or you must supply the correct password for
#	the nickname.
#
#NICK_HELP_RELEASE
#	Syntax: RELEASE nickname [password]
#	
#	Instructs %S to remove any hold on your nickname
#	caused by automatic nick enforcement or use of the RECOVER
#	command.  By default, such holds last for one minute;
#	this command gets rid of them sooner.
#	
#	In order to use the RELEASE command for a nick, your
#	current address as shown in /WHOIS must be on that nick's
#	access list, or you must supply the correct password for
#	the nickname.
#
#NICK_HELP_GHOST
#	Syntax: GHOST nickname [password]
#	
#	Terminates a "ghost" IRC session using your nick.  A
#	"ghost" session is one which is not actually connected,
#	but which the IRC server believes is still online for one
#	reason or another.  Typically, this happens if your
#	computer crashes or your Internet or modem connection
#	goes down while you're on IRC.
#	
#	In order to use the GHOST command for a nick, your
#	current address as shown in /WHOIS must be on that nick's
#	access list, or you must supply the correct password for
#	the nickname.
#
#NICK_HELP_REGAIN
#	Syntax: REGAIN nickname password
#	
#	Terminates a "ghost" IRC session using your nick, changes
#	your nickname to the nick specified, and uses password
#	to identify you.
#	
#	A "ghost" session is one which is not actually connected,
#	but which the IRC server believes is still online for one
#	reason or another.  Typically, this happens if your
#	computer crashes or your internet or modem connection
#	goes down while you're on IRC.
#	
#	Note that using this command on a live client may lead to
#	both of you being collided from the network if they should
#	reconnect faster than your nickname changes - only use
#	REGAIN on dead connections!  Please use the RECOVER to
#	reclaim nicks in use by other people.
#
#NICK_HELP_INFO
#	Syntax: INFO nickname [ALL]
#	
#	Displays information about the given nickname, such as
#	the nick's owner, last seen address and time, and nick
#	options. If you are identified for the nick you're 
#	getting information for and ALL is specified, you will
#	be shown all the information; regardless of whether 
#	it's hidden or not.
#
#NICK_HELP_LIST
#	Syntax: LIST pattern
#	
#	Lists all registered nicknames which match the given
#	pattern, in nick!user@host format.  Nicks with the
#	PRIVATE option set will not be displayed.
#	
#	Examples:
#	
#	    LIST *!joeuser@foo.com
#	        Lists all nicks owned by joeuser@foo.com.
#	
#	    LIST *Bot*!*@*
#	        Lists all registered nicks with Bot in their
#	        names (case insensitive).
#	
#	    LIST *!*@*.bar.org
#	        Lists all nicks owned by users in the bar.org
#	        domain.
#
#NICK_HELP_STATUS
#	Syntax: STATUS nickname...
#	
#	Returns whether the user using the given nickname is
#	recognized as the owner of the nickname.  The response has
#	this format:
#	
#	    nickname status-code
#	
#	where nickname is the nickname sent with the command, and
#	status-code is one of the following:
#	
#	    0 - no such user online or nickname not registered
#	    1 - user not recognized as nickname's owner
#	    2 - user recognized as owner via access list only
#	    3 - user recognized as owner via password identification
#	
#	Up to sixteen nicknames may be sent with each command; the
#	rest will be ignored.  No error message is generated if no
#	nickname is given.
#
#NICK_HELP_AUTOJOIN
#	Syntax: AUTOJOIN ADD channel
#	        AUTOJOIN DEL {channel | list}
#	        AUTOJOIN LIST [list]
#	
#	The AutoJoin command allows you to manage your AutoJoin
#	list.
#	
#	When you successfully identify to your nickname, Services
#	will automatically join you to each channel in this list.
#	
#	Type /msg %S HELP SET AUTOJOIN for information on how to
#	turn this feature on and off.
#
NICK_SERVADMIN_HELP
	
	Die folgenden Befehle sind nur für Services Admins zugänglich:
	
	    SUSPEND    Verhindert, dass sich jemand mit diesem Nick einloggt.
	    FORBID     Verhindert, dass ein Nick benutzt/registriert wird
	
	Services Admin können ausserdem die folgenen Befehle benutzen, ohne
	sich dafür zu Authentifizieren:
	    
	    DROP, SET, ACCESS LIST, INFO ALL
	
	Schau in die Hilfe der Befehle für weitere Informationen.

NICK_SERVADMIN_HELP_DROP
	Syntax: DROP [nickname]
	
	Ohne Parameter entfernt es deinen Nick aus der %S Datenbank.
	
	einem Parameter entfernt es den benannten Nicknamen aus der Datenbank.
	Dieser Befehl ist Services Admins vorbehalten.

NICK_SERVADMIN_HELP_SET
	
	Services Admins können ausserdem die folgenden Optionen setzen.
	
	    NOEXPIRE   Verhindert, dass ein Nick verfällt
	    MARK       Verhindert, dass auf einen Nick SETPASS
	                   oder SENDPASS angewand wird
	    REGTIME    Setzt das Registrationsdatum eines Nicks
	
	Ausserdem können Services Admin die Optionen jedes Nicks ändern,
	ohne ein Passwort angeben zu müssen. (Bei der Benutzung des
	folgenden Schemas:
	SET Nickname Option Parameter)

#NICK_SERVADMIN_HELP_SET_MARK
#	Syntax: SET [nickname] MARK {ON | OFF}
#	
#	Setting this to ON will prevent anyone who is not a
#	services admin from using SETPASS or SENDPASS on the nickname.
#
#NICK_SERVADMIN_HELP_SET_NOEXPIRE
#	Syntax: SET [nickname] NOEXPIRE {ON | OFF}
#	
#	Sets whether the given nickname will expire.  Setting this
#	to ON prevents the nickname from expiring.  If no
#	nickname is given, sets the no-expire flag for your nick.
#	
#	Limited to Services admins.
#
#NICK_SERVADMIN_HELP_SET_REGTIME
#	Syntax: SET [^_nickname^_] REGTIME YYYY-MM-DD-HH-MM-SS
#	
#	Sets the "Time registered" for nickname (or your own
#	nickname) to be the specified time, in the exact format
#	as shown above.
#	
#	Limited to Services admins.
#
#NICK_SERVADMIN_HELP_UNLINK
#	
#	Services admins may unlink any nick without entering a
#	password.
#
#NICK_HELP_LISTLINKS
#	Syntax: LISTLINKS [nick [ALL]]
#
#	Lists nicks which are linked to your nick.  If you specify
#	nick, it must be your nickname or one currently linked to
#	it.  If ALL is specified, lists all nicks with the same
#	privileges as the given nick; otherwise, lists only those
#	nicks which are directly linked to the given nick.
#
#NICK_SERVADMIN_HELP_LISTLINKS
#	
#	Services admins may list the links for any nickname.
#
#NICK_HELP_LISTCHANS
#	Syntax: LISTCHANS [nick]
#
#	Lists all channels in which you have any form of access.
#
#NICK_SERVADMIN_HELP_LISTCHANS
#	
#	Services admins may list the channels of any nickname.
#
#NICK_SERVADMIN_HELP_INFO
#	
#	Services admins may use the ALL parameter with any nick.
#
#NICK_SERVADMIN_HELP_LIST
#	Syntax: LIST pattern [FORBIDDEN] [IRCOP] [NOEXPIRE] [SUSPENDED]
#	
#	Lists all registered nicknames which match the given
#	pattern, in nick!user@host format.  Nicks with the PRIVATE
#	option set will only be displayed to Services admins. For 
#	Services admins, nicks with the NOEXPIRE option set will 
#	have a ! prepended to the nickname and those that are 
#	SUSPENDed will be prepended by a *.
#	
#	If the FORBIDDEN, IRCOP, SUSPENDED or NOEXPIRE
#	options are given, only nicks which, respectively, are FORBIDden,
#	have the IRCOP flag, SUSPENDed or have the NOEXPIRE flag set will be
#	displayed. The options can be used in any order and in any combination.
#	These options are limited to Services admins.
#	
#	Examples:
#	
#	    LIST *!joeuser@foo.com
#	        Lists all registered nicks owned by joeuser@foo.com.
#	
#	    LIST *Bot*!*@*
#	        Lists all registered nicks with Bot in their
#	        names (case insensitive).
#	
#	    LIST * NOEXPIRE
#	        Lists all registered nicks which have been set to
#	        not expire.
#
#NICK_SERVADMIN_HELP_FORBID
#	Syntax: FORBID nickname
#	
#	Disallows a nickname from being registered or used by
#	anyone.  May be cancelled by dropping the nick.
#	
#	Limited to Services admins.
#
#NICK_SERVADMIN_HELP_SUSPEND
#	Syntax: SUSPEND [+expiry] nickname reason
#	
#	Prevents a nickname, and all those nicknames with the
#	same root nickname, from being used or identified for;
#	by anyone. A nickname can be unsuspended with the 
#	UNSUSPEND command. 
#	
#	Unlike a forbidden nickname, a suspended one does not 
#	loose it's information and will expire!
#	
#	Limited to Services admins.
#
#NICK_SERVADMIN_HELP_UNSUSPEND
#	Syntax: UNSUSPEND nickname
#	
#	Allows people to use and identify for a previously
#	SUSPENDed nickname.
#	
#	Limited to Services admins.
#
#
###########################################################################
#
# ChanServ help messages
#
###########################################################################
#
CHAN_HELP_START
	*** %S Hilfe ***

CHAN_HELP_END
	*** Ende der %S Hilfe ***

CHAN_HELP
	%S ermöglicht es dir, einen Raum zu registrieren und
	zu kontrollieren. %S kann verhindern, daß dein Raum von 
	Unbefugten übernommen wird.
	Die verfügbaren Befehle sind unten aufgelistet. Um sie zu
	benutzen, versuch /msg %S Befehl. Für Hilfe zu einem
	bestimmten Befehl, versuch /msg %S HELP Befehl.
	
	    REGISTER   Registriert einen Raum
	    IDENTIFY   Identifiziert dich als Eigentümer eines Raumes
	    SET        Setzt Optionen und Informationen für einen Raum
	    ACCESS     Modifiziert die Zugriffsliste
	    LEVELS     Ändert die Bedeutung der Access-Levels
	    AKICK      Modifiziert die Auto Kick-Liste
	    DROP       Löscht einen Raum aus der Datenbank
	
	Weitere Befehle: INFO, LIST, INVITE, OP, DEOP, UNBAN,
	                CLEAR, SENDPASS, VOP, AOP, SOP, SYNC,
	                SENDPASS

CHAN_HELP_EXPIRES
	
	Vergiss bitte nicht, daß Räume, die %d Tage nicht benutzt werden
	(dass heißt, von keinem aus der Zugriffsliste in dieser Zeit
	betreten wurde), automatisch gelöscht werden.

#CHAN_HELP_REGISTER
#	Syntax: REGISTER channel password description
#	
#	Registers a channel in the %S database.  In order
#	to use this command, you must first be a channel operator
#	on the channel you're trying to register.  The password
#	is used with the IDENTIFY command to allow you to
#	make changes to the channel settings at a later time.
#	The last parameter, which must be included, is a
#	general description of the channel's purpose.
#	
#	When you register a channel, you are recorded as the
#	"founder" of the channel.  The channel founder is allowed
#	to change all of the channel settings for the channel;
#	%S will also automatically give the founder
#	channel-operator privileges when s/he enters the channel.
#	See the ACCESS command (/msg %S HELP ACCESS) for
#	information on giving a subset of these privileges to
#	other channel users.
#	
#	NOTICE: In order to register a channel, you must have
#	first registered your nickname.  If you haven't,
#	/msg %s HELP for information on how to do so.
#
#CHAN_HELP_IDENTIFY
#	Syntax: IDENTIFY channel password
#	
#	Authenticates you to %S as the founder of the given
#	channel.  Many commands require you to use this command
#	before using them.  The password should be the same one
#	you sent with the REGISTER command.
#
#CHAN_HELP_DROP
#	Syntax: DROP channel
#	
#	Unregisters the named channel.  Can only be used by
#	channel founder, who must use the IDENTIFY command first.
#
#CHAN_HELP_SENDPASS
#	Syntax: SENDPASS channel
#	
#	E-mails the channel password to the founder of the channel,
#	using the address they set on their nickname.
#
#CHAN_HELP_SET
#	Syntax: SET channel option parameters
#	
#	Allows the channel founder to set various channel options
#	and other information.  The founder must use the IDENTIFY
#	command before using SET.
#	
#	Available options:
#	
#	    FOUNDER     Set the founder of a channel
#	    SUCCESSOR   Set the successor for a channel
#	    PASSWORD    Set the founder password
#	    DESC        Set the channel description
#	    URL         Associate a URL with the channel
#	    EMAIL       Associate an E-mail address with the channel
#	    ENTRYMSG    Set a message to be sent to users when they
#	                   enter the channel
#	    TOPIC       Change the topic on the channel
#	    KEEPTOPIC   Retain topic when channel is not in use
#	    TOPICLOCK   Topic can only be changed with SET TOPIC
#	    MLOCK       Lock channel modes on or off
#	    PRIVATE     Hide channel from LIST command
#	    RESTRICTED  Restrict access to the channel
#	    SECURE      Activate %S security features
#	    SECUREOPS   Stricter control of chanop status
#	    LEAVEOPS    Never de-op users except with DEOP command
#	    VERBOSE     Send a notice to channel operators when certain
#	                   certain commands are used on the channel
#	    AUTOLIMIT   Automatically change channel limit
#	    CLEARBANS   Automatically expire bans
#	
#	Type /msg %S HELP option for more information on a
#	particular option.
#
#CHAN_HELP_SET_FOUNDER
#	Syntax: SET channel FOUNDER nick
#	
#	Changes the founder of a channel.  The new nickname must
#	be a registered one.
#
#CHAN_HELP_SET_SUCCESSOR
#	Syntax: SET channel SUCCESSOR nick
#	
#	Changes the successor of a channel.  If the founder's
#	nickname expires or is dropped while the channel is still
#	registered, the successor will become the new founder of the
#	channel.  However, if the successor already has too many
#	channels registered (%d), the channel will be dropped
#	instead, just as if no successor had been set.  The new
#	nickname must be a registered one.
#
#CHAN_HELP_SET_PASSWORD
#	Syntax: SET channel PASSWORD password
#	
#	Sets the password used to identify as the founder of the
#	channel.
#
#CHAN_HELP_SET_DESC
#	Syntax: SET channel DESC description
#	
#	Sets the description for the channel, which shows up with
#	the LIST and INFO commands.
#
#CHAN_HELP_SET_URL
#	Syntax: SET channel URL [url]
#	
#	Associates the given URL with the channel.  This URL will
#	be displayed whenever someone requests information on the
#	channel with the INFO command.  If no parameter is given,
#	deletes any current URL for the channel.
#
#CHAN_HELP_SET_EMAIL
#	Syntax: SET channel EMAIL [address]
#	
#	Associates the given E-mail address with the channel.
#	This address will be displayed whenever someone requests
#	information on the channel with the INFO command.  If no
#	parameter is given, deletes any current E-mail address for
#	the channel.
#
#CHAN_HELP_SET_ENTRYMSG
#	Syntax: SET channel ENTRYMSG [message]
#	
#	Sets the message which will be sent via /notice to users
#	when they enter the channel.  If no parameter is given,
#	causes no message to be sent upon entering the channel.
#
#CHAN_HELP_SET_TOPIC
#	Syntax: SET channel TOPIC topic
#	
#	Causes %S to set the channel topic to the one
#	specified.  This command is most useful in conjunction
#	with SET TOPICLOCK.
#
#CHAN_HELP_SET_KEEPTOPIC
#	Syntax: SET channel KEEPTOPIC {ON | OFF}
#	
#	Enables or disables the topic retention option for a
#	channel.  When topic retention is set, the topic for the
#	channel will be remembered by %S even after the
#	last user leaves the channel, and will be restored the
#	next time the channel is created.
#
#CHAN_HELP_SET_TOPICLOCK
#	Syntax: SET channel TOPICLOCK {ON | OFF}
#	
#	Enables or disables the topic lock option for a channel.
#	When topic lock is set, %S will not allow the
#	channel topic to be changed except via the SET TOPIC
#	command.
#
#CHAN_HELP_SET_MLOCK
#	Syntax: SET channel MLOCK modes
#	Syntax: SET channel MLOCK OFF
#	
#	Sets the mode-lock parameter for the channel.  %S
#	allows you to define certain channel modes to be always
#	on (or off).  The modes that can be locked are c, i,
#	k, l, m, M, n, O, p, R, s, and t; each of these modes
#	can be locked on, locked off, or not locked.  The modes
#	parameter is constructed exactly the same way as a /MODE
#	command; that is, modes followed by a + are locked on, and modes
#	followed by a - are locked off.
#	
#	Warning:  If you set a mode-locked key, as in the second
#	example below, you should also set the RESTRICTED option for
#	the channel (see HELP SET RESTRICTED), or anyone entering
#	the channel when it is empty will be able to see the key!
#	
#	Specifying OFF, or + with no modes, removes the channel mode
#	lock.
#	
#	Examples:
#	
#	    SET #channel MLOCK +nt-iklps
#	        Forces modes n and t on, and modes i, k, l, p, and
#	        s off.  Mode m is left free to be either on or off.
#	
#	    SET #channel MLOCK +knst-ilmp my-key
#	        Forces modes k, n, s, and t on, and modes i, l, m,
#	        and p off.  Also forces the channel key to be
#	        "my-key".
#	
#	    SET #channel MLOCK +
#	        Removes the mode lock; all channel modes are free
#	        to be either on or off.
#
#CHAN_HELP_SET_PRIVATE
#	Syntax: SET channel PRIVATE {ON | OFF}
#	
#	Enables or disables the private option for a channel.
#	When private is set, a /msg %S LIST will not
#	include the channel in any lists.
#
#CHAN_HELP_SET_RESTRICTED
#	Syntax: SET channel RESTRICTED {ON | OFF}
#	
#	Enables or disables the restricted access option for a
#	channel.  When restricted access is set, users who would
#	normally be disallowed from having channel operator
#	privileges (users with negative access levels and, if
#	secure ops is set, users not on the access list) will
#	instead be kicked and banned from the channel.
#
#CHAN_HELP_SET_SECURE
#	Syntax: SET channel SECURE {ON | OFF}
#	
#	Enables or disables %S's security features for a
#	channel.  When SECURE is set, only users who have
#	registered their nicknames with %s and IDENTIFY'd
#	with their password will be given access to the channel
#	as controlled by the access list.
#
#CHAN_HELP_SET_SECUREOPS
#	Syntax: SET channel SECUREOPS {ON | OFF}
#	
#	Enables or disables the secure ops option for a channel.
#	When secure ops is set, users who are not on the userlist
#	will not be allowed chanop status.
#
#CHAN_HELP_SET_LEAVEOPS
#	Syntax: SET channel LEAVEOPS {ON | OFF}
#	
#	Enables or disables the leave ops option for a channel.
#	When leave ops is set, ChanServ will never de-op a user in
#	that channel except with the DEOP command.
#
#CHAN_HELP_SET_VERBOSE
#	Syntax: SET channel VERBOSE {ON | OFF}
#	
#	Enables or disables the verbose option for a channel.
#	When verbose is set, ChanServ will send a notice to the
#	channel's operators when certain commands are used on the channel.
#
#CHAN_HELP_SET_AUTOLIMIT
#	Syntax: SET channel AUTOLIMIT {ON | OFF}
#	        SET channel AUTOLIMIT offset:tolerance:time period
#	
#	Enables or disables channel autolimit.
#	
#	When autolimit is set, ChanServ will maintain the channel limit
#	(+l) just above the number of people actually inside the channel.
#	This helps to prevent abuse by limiting the rate at which people can
#	join your channel.
#	
#	The simple ON/OFF form will enable autolimit with a sensible
#	default.  If you require more control you can use the more powerful
#	offset:tolerance:period format.
#	
#	Offset: the limit will be set to the sum of this value and the current
#	number of users in the channel.
#	
#	Tolerance: For the limit to be updated, the difference between the
#	new limit current limit must be greater than this value.
#
#	Time period: how many minutes services should wait between checking
#	if it needs to update the limit.
#	
#	offset must be between 1 and 500, tolerance must be between
#	0 and 500, time period is in minutes and must be between 1 and 60.
#	
#	Example:
#	
#	SET #channel AUTOLIMIT 5:2:2
#	
#	Update the limit in steps of 5, with a tolerance of 2, every 2 minutes.
#
#CHAN_HELP_SET_CLEARBANS
#	Syntax: SET channel CLEARBANS {ON | OFF}
#	        SET channel CLEARBANS time period
#	
#	Enables or disables automatic ban clearance.
#	
#	When clearbans is set, ChanServ will periodically remove bans that
#	have been present for a set amount of time.  This helps to keep the
#	channel's ban list from filling up with useless bans (AutoKick
#	is more suitable for long-term banning).
#	
#	The simple ON/OFF form will enable clearbans with a sensible
#	default.  If you require more control you can use the second form to
#	specify your own time period (in minutes) before bans are removed.
#	
#	Example:
#	
#	SET #channel CLEARBANS 120
#	
#	Remove bans that are two or more hours old (120 minutes).
#
#CHAN_HELP_ACCESS
#	Syntax: ACCESS channel ADD nick level
#	        ACCESS channel DEL {nick | entry-num | list}
#	        ACCESS channel LIST {mask | list}
#	        ACCESS channel VIEW {mask | list}
#	        ACCESS channel COUNT
#	
#	Maintains the access list for a channel.  The access
#	list specifies which users are allowed chanop status or
#	access to %S commands on the channel.  Different
#	user levels allow for access to different subsets of
#	privileges; /msg %S HELP ACCESS LEVELS for more
#	specific information.  Any nick not on the access list has
#	a user level of 0.
#	
#	The ACCESS ADD command adds the given nickname to the
#	access list with the given user level; if the nick is
#	already present on the list, its access level is changed to
#	the level specified in the command.  The level specified
#	must be less than that of the user giving the command, and
#	if the nick is already on the access list, the current
#	access level of that nick must be less than the access level
#	of the user giving the command.
#	
#	The ACCESS DEL command removes the given nick from the
#	access list.  If a list of entry numbers is given, those
#	entries are deleted.  (See the example for LIST below.)
#	
#	The ACCESS LIST command displays the access list.  If
#	a wildcard mask is given, only those entries matching the
#	mask are displayed.  If a list of entry numbers is given,
#	only those entries are shown; for example:
#
#	   ACCESS #channel LIST 2-5,7-9
#	      Lists access entries numbered 2 through 5 and
#	      7 through 9.
#	
#	The ACCESS VIEW command is similar to ACCESS LIST,
#	but provides more detailed information.
#	
#	The ACCESS COUNT command returns the number of entries
#	there are on the access list.
#
#CHAN_HELP_VOP
#	Syntax: VOP channel ADD nick
#	
#	The VOP ADD command adds the given nick to the access 
#	list using the channel's default level for auto voice 
#	(default level 3). This command is meant as a quick 
#	alias to ACCESS ADD.
#
#CHAN_HELP_AOP
#	Syntax: AOP channel ADD nick
#	
#	The AOP ADD command adds the given nick to the access
#	list using the channel's default level for auto op
#	(default level 5). This command is meant as a quick
#	alias to ACCESS ADD.
#
#CHAN_HELP_SOP
#	Syntax: SOP channel ADD nick
#	
#	The SOP ADD command adds the given nick to the access
#	list using the channel's default level for AKICK add
#	(default level 10). This command is meant as a quick
#	alias to ACCESS ADD.
#
#CHAN_HELP_ACCESS_LEVELS
#	User access levels
#	
#	By default, the following access levels are defined:
#	
#	   Founder   Full access to %S functions; automatic
#	                     opping upon entering channel.  Note
#	                     that only one person may have founder
#	                     status (it cannot be given using the
#	                     ACCESS command).
#	        10   Access to AKICK command; automatic opping.
#	         5   Automatic opping.
#	         3   Automatic voicing.
#	         0   No special privileges; can be opped by other
#	                     ops (unless secure-ops is set).
#	        <0   May not be opped.
#	
#	These levels may be changed, or new ones added, using the
#	LEVELS command; type /msg %S HELP LEVELS for
#	information.
#
#CHAN_HELP_AKICK
#	Syntax: AKICK channel ADD mask [reason]
#	        AKICK channel DEL mask
#	        AKICK channel LIST [mask | list]
#	        AKICK channel VIEW [mask | list]
#	        AKICK channel ENFORCE
#	        AKICK channel COUNT
#	
#	Maintains the AutoKick list for a channel.  If a user
#	on the AutoKick list attempts to join the channel,
#	%S will ban that user from the channel, then kick
#	the user.
#	
#	The AKICK ADD command adds the given nick or usermask
#	to the AutoKick list.  If a reason is given with
#	the command, that reason will be used when the user is
#	kicked; if not, the default reason is "You have been
#	banned from the channel".
#	
#	The AKICK DEL command removes the given nick or mask
#	from the AutoKick list.  It does not, however, remove any
#	bans placed by an AutoKick; those must be removed
#	manually.
#	
#	The AKICK LIST command displays the AutoKick list, or
#	optionally only those AutoKick entries which match the
#	given mask. AKICK VIEW simply shows more details.
#	
#	The AKICK ENFORCE command causes ChanServ to enforce the
#	current AKICK list by removing those users who match an
#	AKICK mask.
#	
#	The AKICK COUNT command returns how many entries there
#	are on the AutoKick list.
#
#CHAN_HELP_LEVELS
#	Syntax: LEVELS channel SET type level
#	        LEVELS channel {DIS | DISABLE} type
#	        LEVELS channel LIST
#	        LEVELS channel RESET
#	
#	The LEVELS command allows fine control over the meaning of
#	the numeric access levels used for channels.  With this
#	command, you can define the access level required for most
#	of %S's functions.  (The SET FOUNDER and SET PASSWORD
#	commands, as well as this command, are always restricted to
#	the channel founder.)
#	
#	LEVELS SET allows the access level for a function or group of
#	functions to be changed.  LEVELS DISABLE (or DIS for short)
#	disables an automatic feature or disallows access to a
#	function by anyone other than the channel founder.
#	LEVELS LIST shows the current levels for each function or
#	group of functions.  LEVELS RESET resets the levels to the
#	default levels of a newly-created channel (see
#	HELP ACCESS LEVELS).
#	
#	For a list of the features and functions whose levels can be
#	set, see HELP LEVELS DESC.
#
#CHAN_HELP_LEVELS_DESC
#	The following feature/function names are understood.  Note
#	that the levels for AUTODEOP and NOJOIN are maximum levels,
#	while all others are minimum levels.
#
#CHAN_HELP_LEVELS_DESC_FORMAT
#	    %-*s  %s
#
#CHAN_HELP_INFO
#	Syntax: INFO channel [ALL]
#	
#	Lists information about the named registered channel,
#	including its founder, time of registration, last time
#	used, description, and mode lock, if any. If you are
#	identified as the founder of the channel you're getting
#	information for and ALL is specified, the entry message
#	and successor will also be displayed.
#
#CHAN_HELP_LIST
#	Syntax: LIST pattern
#	
#	Lists all registered channels matching the given pattern.
#	(Channels with the PRIVATE option set are not listed.)
#
#CHAN_HELP_OP
#	Syntax: OP #channel [nick]
#	
#	Ops a selected nick on a channel.  By default, limited to
#	those with level 5 access and above on the channel.  If
#	nick is omitted, this command will attempt to op you
#	in the specified channel.
#
#CHAN_HELP_DEOP
#	Syntax: DEOP #channel [nick]
#	
#	Deops a selected nick on a channel.  By default, limited to
#	those with level 5 access and above on the channel.  If
#	nick is omitted, this command will attempt to deop you
#	in the specified channel.
#
#CHAN_HELP_VOICE
#	Syntax: VOICE #channel [nick]
#	
#	Voices a selected nick on a channel.  By default, limited
#	to those with level 5 access and above on the channel.  If
#	nick is omitted, this command will attempt to voice you
#	in the specified channel, this by default requires level 3
#	access.
#
#CHAN_HELP_DEVOICE
#	Syntax: DEVOICE #channel [nick]
#	
#	Devoices a selected nick on a channel.  If nick is
#	omitted, this command will attempt to devoice you in the
#	specified channel.  Devoicing yourself by default requires
#	level 3 access, devoicing others requires level 5 access by
#	default.
#
#CHAN_HELP_INVITE
#	Syntax: INVITE channel
#	
#	Tells %S to invite you into the given channel.  By
#	default, only works if your access level on that channel is
#	greater than zero.
#
#CHAN_HELP_UNBAN
#	Syntax: UNBAN channel
#	
#	Tells %S to remove all bans preventing you from
#	entering the given channel.  By default, only works if your
#	access level on that channel is greater than zero.
#
#CHAN_HELP_CLEAR
#	Syntax: CLEAR channel what
#	
#	Tells %S to clear certain settings on a channel.  what
#	can be any of the following:
#	
#	     MODES    Resets all modes on the channel (i.e. clears
#	                  modes i,k,l,m,n,p,s,t).
#	     BANS     Clears all bans on the channel.
#	     OPS      Removes channel-operator status (mode +o) from
#	                  all channel operators.
#	     VOICES   Removes "voice" status (mode +v) from anyone
#	                  with that mode set.
#	     USERS    Removes (kicks) all users from the channel.
#	
#	By default, your access level on the channel must be at
#	least 10 to use this command.
#
#CHAN_HELP_SYNC
#	Syntax: SYNC channel
#	
#	Tells %S to make sure that the channel modes, bans, ops and
#	voices that it sees are the same on all other servers.  This
#	provides a cure for the desync phenomenon that can
#	sometimes be seen on IRC.
#
#CHAN_SERVADMIN_HELP
#	
#	The following commands are available to Services admins:
#	
#	    FORBID     Prevent a channel from being used
#	    STATUS     Returns the current access level of a user
#	                   on a channel
#	
#	Services admins can also drop any channel without needing
#	to identify via password, and may view the access, AKICK,
#	and level setting lists for any channel.
#
#CHAN_SERVADMIN_HELP_DROP
#	Syntax: DROP channel
#	
#	Unregisters the named channel.  Only Services admins
#	can drop a channel for which they have not identified.
#
#CHAN_SERVADMIN_HELP_SET
#	
#	Services admins can also set the following:
#	    NOEXPIRE   Prevent a channel from expiring
#	    REGTIME    Set a channel's registration time
#	
#	Additionally, Services admins can set options for any
#	channel without identifying by password for the channel.
#
#CHAN_SERVADMIN_HELP_SET_NOEXPIRE
#	Syntax: SET channel NOEXPIRE {ON | OFF}
#	
#	Sets whether the given channel will expire.  Setting this
#	to ON prevents the channel from expiring.
#	
#	Limited to Services admins.
#
#CHAN_SERVADMIN_HELP_SET_REGTIME
#	Syntax: SET channel REGTIME YYYY-MM-DD-HH-MM-SS
#	
#	Sets the "Time registered" for channel to the specified
#	time in the exact format as shown above.
#	
#	Limited to Services admins.
#
#CHAN_SERVADMIN_HELP_INFO
#	
#	Services admins can use the ALL parameter with any channel.
#
#CHAN_SERVADMIN_HELP_LIST
#	Syntax: LIST pattern [FORBIDDEN] [NOEXPIRE]
#	
#	Lists all registered channels matching the given pattern.
#	Channels with the PRIVATE option set will only be displayed
#	to Services admins. Channels with the NOEXPIRE option set 
#	will have a ! prepended to the channel name for Services 
#	admins.
#	
#	If the FORBIDDEN or NOEXPIRE options are given, only channels
#	which, respectively, are FORBIDden or have the NOEXPIRE flag
#	set will be displayed. If both options are given, both
#	types of channels will be displayed. These options are 
#	limited to Services admins.
#
#CHAN_SERVADMIN_HELP_FORBID
#	Syntax: FORBID channel
#	
#	Disallows anyone from registering or using the given
#	channel.  May be cancelled by dropping the channel.
#	
#	Limited to Services admins.
#
#CHAN_SERVADMIN_HELP_STATUS
#	Syntax: STATUS channel nickname
#	
#	Returns the current access level of the given nick on the
#	given channel.  The reply is of the form:
#	
#	    STATUS channel nickname access-level
#	
#	If an error occurs, the reply will be in the form:
#	
#	    STATUS ERROR error-message
#	
#	Limited to Services admins.
#
###########################################################################
#
# MemoServ help messages
#
###########################################################################

MEMO_HELP_START
	*** %S Hilfe ***

MEMO_HELP_END
	*** Ende der %S Hilfe ***

MEMO_HELP
	Mit Hilfe von %S kanst du kleine Nachrichten zu anderen
	Chatten schicken, unabhängig davon, ob sich die Person im
	Moment im IRC befindet oder nicht. Desweiteren kann man Memos
	an Räume(*) schicken.
	Es ist erforderlich, daß du und der Empfänger einen registrierten
	Nicknamen haben (bzw. registrierten Raum).
	
	%S Befehle:
	
	    INFO   Zeigt Memo Informationen
	    SEND   Schickt ein Memo an einen Nicknamen oder einen Raum
	    LIST   Zeigt die Liste der eigenen Memos
	    READ   Zeigt ein Memo
	    DEL    Löscht ein oder mehrere Memos
	    SET    Setzt Optionen für Memos
	
	/msg %S HELP Befehl für weitere Informationen zu einem
	bestimmten Befehl.
	
	(*) Die Standardeinstellung ist, daß nur Nicknamen mit Stufe 10
	    in der Zugriffsliste eines Raumes die Raum-Memos
	    lesen/verändern dürfen. Diese Einstellung kann man mit
	    LEVELS ändern.

#MEMO_SERVADMIN_HELP
#	%S is a utility allowing IRC users to send short
#	messages to other IRC users, whether they are online at
#	the time or not, or to channels(*).  Both the sender's
#	nickname and the target nickname or channel must be
#	registered in order to send a memo.
#	
#	%S's commands include:
#	
#	    INFO        Display memo information
#	    SEND        Send a memo to a nick or channel
#	    GLOBALSEND  Send a memo to all registered nicks
#	    OPERSEND    Send a memo to all registered IRC Operators
#	    CSOPSEND    Send a memo to all Services Operators
#	    LIST        List your memos
#	    READ        Read a memo or memos
#	    DEL         Delete a memo or memos
#	    SET         Set options related to memos
#	
#	Type /msg %S HELP command for help on any of the
#	above commands.
#	
#	(*) By default, any user with at least level 10 access on a
#	    channel can read that channel's memos.  This can be
#	    changed with the %s LEVELS command.
#
#MEMO_HELP_SEND
#	Syntax: SEND {nick | channel} memo-text
#	
#	Sends the named nick or channel a memo containing
#	memo-text.  When sending to a nickname, the recipient will
#	receive a notice that he/she has a new memo.  The target
#	nickname/channel must be registered.
#
#MEMO_HELP_LIST
#	Syntax: LIST [channel] [list | NEW]
#	
#	Lists any memos you currently have.  With NEW, lists only
#	new (unread) memos.  Unread memos are marked with a "*"
#	to the left of the memo number.  You can also specify a list
#	of numbers, as in the example below:
#
#	   LIST 2-5,7-9
#	      Lists memos numbered 2 through 5 and 7 through 9.
#
#MEMO_HELP_READ
#	Syntax: READ [channel] {num | list | ALL | LAST | NEW}
#	
#	Sends you the text of the memos specified.  If ALL is
#	given, sends you all of your memos whether previously read
#	or unread.  If LAST is given, sends you the memo you most
#	recently received.  If NEW is given, sends you all of your
#	new memos.  Otherwise, sends you memo number num.  You
#	can also give a list of numbers, as in this example:
#	
#	   READ 2-5,7-9
#	      Displays memos numbered 2 through 5 and 7 through 9.
#
#MEMO_HELP_DEL
#	Syntax: DEL [channel] {num | list | ALL}
#	
#	Deletes the specified memo or memos.  You can supply
#	multiple memo numbers or ranges of numbers instead of a
#	single number, as in the second example below.
#	
#	If ALL is given, deletes all of your memos.
#	
#	Examples:
#	
#	   DEL 1
#	      Deletes your first memo.
#	
#	   DEL 2-5,7-9
#	      Deletes memos numbered 2 through 5 and 7 through 9.
#
#MEMO_HELP_SET
#	Syntax: SET option parameters
#	
#	Sets various memo options.  option can be one of:
#	
#	    NOTIFY      Changes when you will be notified about
#	                    new memos (only for nicknames)
#	    LIMIT       Sets the maximum number of memos you can
#	                    receive
#	
#	Type /msg %S HELP SET option for more information
#	on a specific option.
#
#MEMO_HELP_SET_NOTIFY
#	Syntax: SET NOTIFY {ON | LOGON | NEW | OFF}
#	
#	Changes when you will be notified about new memos:
#	
#	    ON      You will be notified of memos when you log on,
#	               when you unset /AWAY, and when they are sent
#	               to you.
#	    LOGON   You will only be notified of memos when you log
#	               on or when you unset /AWAY.
#	    NEW     You will only be notified of memos when they
#	               are sent to you.
#	    OFF     You will not receive any notification of memos.
#	
#	ON is essentially LOGON and NEW combined.
#
#MEMO_HELP_SET_LIMIT
#	Syntax: SET LIMIT [channel] limit
#	
#	Sets the maximum number of memos you (or the given channel)
#	are allowed to have. If you set this to 0, no one will be
#	able to send any memos to you.  However, you cannot set
#	this any higher than %d.
#
#MEMO_HELP_INFO
#	Syntax: INFO [channel]
#	
#	Displays information on the number of memos you have, how
#	many of them are unread, how many total memos you can
#	receive and when you are notified of new ones. With a 
#	parameter, displays the same information for the given 
#	channel.
#
#MEMO_SERVADMIN_HELP_SET_LIMIT
#	Syntax: SET LIMIT [user | channel] {limit | NONE} [HARD]
#	
#	Sets the maximum number of memos a user or channel is
#	allowed to have.  Setting the limit to 0 prevents the user
#	from receiving any memos; setting it to NONE allows the
#	user to receive and keep as many memos as they want.  If
#	you do not give a nickname or channel, your own limit is
#	set.
#	
#	Adding HARD prevents the user from changing the limit.  Not
#	adding HARD has the opposite effect, allowing the user to
#	change the limit (even if a previous limit was set with
#	HARD).
#	
#	This use of the SET LIMIT command is limited to Services
#	admins.  Other users may only enter a limit for themselves
#	or a channel on which they have such privileges, may not
#	remove their limit, may not set a limit above %d, and may
#	not set a hard limit.
#
#MEMO_SERVADMIN_HELP_INFO
#	Syntax: INFO [nick | channel]
#	
#	Without a parameter, displays information on the number of
#	memos you have, how many of them are unread, how many
#	total memos you can receive and when you are notified of
#	new ones.
#	
#	With a channel parameter, displays the same information for
#	the given channel.
#	
#	With a nickname parameter, displays the same information
#	for the given nickname.  This use limited to Services
#	admins.
#
#MEMO_SERVADMIN_HELP_GLOBALSEND
#	Syntax: GLOBALSEND memo-text
#
#	Sends a memo conatining memo-text to every registered
#	nick.  Suspended nicks and those at their maximum number of
#	memos will still receive this memo.
#
#	Use of this command is limited to Services admins.
#
#MEMO_SERVADMIN_HELP_OPERSEND
#	Syntax: OPERSEND memo-text
#
#	Sends a memo containing memo-text to every registered
#	nick with the IRCOP flag set.  Suspended nicks and those
#	at their maximum number of memos will still receive this
#	memo.
#
#	Use of this command is limited to Services admins.
#
#MEMO_SERVADMIN_HELP_CSOPSEND
#	Syntax: CSOPSEND memo-text
#
#	Sends a memo containing memo-text to every Services
#	Operator.  Suspended nicks and those at their maximum number
#	of memos will still receive this memo.
#
#	Use of this command is limited to Services admins.
#
###########################################################################
#
# OperServ help messages
#
###########################################################################
#
OPER_HELP_START
	*** %S Hilfe ***

OPER_HELP_END
	*** Ende der %S Hilfe ***


OPER_HELP
	%S Befehle:
	    GLOBAL      Sendet eine Nachricht an alle User
	    STATS       Statistiken über Services und Netzwerk.
	    OPER LIST   Liste der Services-Operatoren
	    ADMIN LIST  Liste der Services-Admins
	    SESSION     Zeigt die Liste der Host-Sitzungen
	    AKILL       Manipulate the AKILL list
	
	Befehle, die nur für Services-Operators und -Admins verfügbar sind:
	    MODE        Setzt Raum-Modi
	    KICK        Kickt Benutzer aus dem Raum
	
	Befehle, die nur für Services-Admins verfügbar sind:
	    EXCEPTION   Bearbeitet die Sitzungslimit-Ausnahmen
	    KILLCLONES  Killt alle Benutzer von einem Host
	    OPER        Bearbeitet die Liste der Services-Operatoren
	    LOGONNEWS   Bearbeitet die Liste der Logon-News
	    OPERNEWS    Bearbeitet die Liste der Oper-News
	    CLEARMODES  Setzt alle Modi von einem Raum neu
	    JUPE        Setzt einen Jupiter über einen Server
	    RAW         Schickt einen RAW-Befehl zum IRC-Server
	    SET         Setzt Services-Optionen
	    SHUTDOWN    Speichert die Datenbank und beendet die Services
	    QUIT        Beendet die Services ohne die Datenbank zu speichern!
	    RESTART     Speichert die Datenbank und startet die Services neu
	    QUARANTINE  Bearbeitet die Quarantäne-Liste
	    LOADLANGS   Lädt die Sprach-Dateien neu
	
	Befehle, die nur für den Services-Root verfügbar sind:
	    ADMIN       Bearbeitet die Liste der Services-Admins
	    ROTATELOG   Benennt das Logfile um und startet ein neues
	
	Hinweis: Alle Befehle zu %S werden geloggt!

#OPER_HELP_GLOBAL
#	Syntax: GLOBAL message
#	
#	Allows IRCops to send messages to all users on the network.
#	The message will be sent from the nick %s.
#
#OPER_HELP_STATS
#	Syntax: STATS [AKILL | ALL]
#	
#	Without any option, shows the current number of users and
#	IRCops online (excluding Services), the highest number of
#	users online since Services was started, and the length of
#	time Services has been running.
#	
#	With the AKILL option, displays the current size of the
#	AKILL list and the current default expiry time.
#	
#	The ALL option is available only to Services admins, and
#	displays information on Services' memory usage.  Using this
#	option can freeze Services for a short period of time on
#	large networks, so don't overuse it!
#	
#	UPTIME may be used as a synonym for STATS.
#
#OPER_HELP_OPER
#	Syntax: OPER ADD nick
#	        OPER DEL nick
#	        OPER LIST
#	
#	Allows Services admins to add or remove nicknames to or from
#	the Services operator list.  A user whose nickname is on the
#	Services operator list and who has identified to %s
#	will be able to access Services operator commands.
#	
#	Any IRC operator may use the OPER LIST form of the command.
#	All other use limited to Services admins.
#
#OPER_HELP_ADMIN
#	Syntax: ADMIN ADD nick
#	        ADMIN DEL nick
#	        ADMIN LIST
#	
#	Allows the Services super-user to add or remove nicknames
#	to or from the Services admin list.  A user whose nickname
#	is on the Services admin list and who has identified to
#	%s will be able to access Services admin commands.
#	
#	Any IRC operator may use the ADMIN LIST form of the command.
#	All other use limited to Services root.
#
#OPER_HELP_MODE
#	Syntax: MODE channel modes
#	
#	Allows Services operators to set channel modes for any
#	channel.  Parameters are the same as for the standard /MODE
#	command.
#	
#	Limited to Services operators.
#
#OPER_HELP_CLEARMODES
#	Syntax: CLEARMODES channel [ALL]
#	
#	Clears all binary modes (i,k,l,m,n,p,s,t) and bans from a
#	channel.  If ALL is given, also clears all ops, voices
#	and bans (+o, +v and +b modes) from the channel.
#	
#	Limited to Services operators.
#
#OPER_HELP_KICK
#	Syntax: KICK channel user reason
#	
#	Allows IRCops to kick a user from any channel.
#	Parameters are the same as for the standard /KICK
#	command.  The kick message will have the nickname of the
#	IRCop sending the KICK command prepended; for example:
#	
#	*** SpamMan has been kicked off channel #my_channel by %S (Alcan (Flood))
#	
#	Limited to Services operators.
#
#OPER_HELP_AKILL
#	Syntax: AKILL ADD [+expiry] [FORCE] mask reason
#	        AKILL DEL mask
#	        AKILL LIST [mask] [NOEXPIRE] 
#	        AKILL VIEW [mask] [NOEXPIRE]
#	        AKILL COUNT
#	
#	Allows IRC operators to manipulate the AKILL list.  If
#	a user matching an AKILL mask attempts to connect, Services
#	will issue a KILL for that user and, on supported server
#	types, will instruct all servers to add a ban (K-line) for
#	the mask which the user matched.
#	
#	AKILL ADD adds the given user@host mask to the AKILL
#	list for the given reason (which must be given).
#	AKILL DEL removes the given mask from the AKILL list if it
#	is present.  AKILL LIST shows all current AKILLs; if the
#	optional mask is given, the list is limited to those
#	AKILLs matching the mask.  AKILL VIEW is a more verbose
#	version of AKILL LIST, and will show who added an AKILL,
#	the date it was added, and when it expires, as well as the
#	user@host mask and reason. If the NOEXPIRE option is given,
#	then only AKILLs that will not expire, and match the given
#	user@host mask, will be displayed. AKILL COUNT simply
#	returns the number of AKILLs in the AKILL list.
#	
#	Optionally, an expiry time can be given with the AKILL ADD
#	command.  The expiry time precedes the user@host mask, and
#	is specified as an integer followed by one of d (days),
#	h (hours), or m (minutes).  Combinations (such as 1h30m)
#	are not permitted.  If a unit specifier is not included,
#	the default is days (so +30 by itself means 30 days).  To
#	add an AKILL which does not expire, use +0.  If the
#	usermask to be added starts with a +, an expiry time must
#	be given, even if it is the same as the default.  The
#	current AKILL default expiry time can be found with the
#	STATS AKILL command.
#	
#	Services Admins may specify the FORCE parameter to AKILL ADD.
#	This parameter placed before the mask causes services to ignore
#	all wildcard thresholds.  It is thus an extremely dangerous
#	option and should be used with great care.
#
#	Services Opers and Admins may delete any AKILL.  IRC Operators
#	may delete only AKILLs which they originally added.
#
#OPER_HELP_SET
#	Syntax: SET option setting
#	
#	Sets various global Services options.  Option names
#	currently defined are:
#	    DEBUG      Activate or deactivate debug mode
#	    LOGSQL     Activate or deactivate SQL logging
#	
#	Limited to Services admins.
#
#OPER_HELP_SET_DEBUG
#	Syntax: SET DEBUG {ON | OFF | num}
#	
#	Sets debug mode on or off.  In debug mode, all data sent to
#	and from Services as well as a number of other debugging
#	messages are written to the log file.  If num is
#	given, debug mode is activated, with the debugging level set
#	to num.
#	
#	This option is equivalent to the command-line option
#	-debug.
#
#OPER_HELP_SET_LOGSQL
#	Syntax: SET LOGSQL {ON | OFF}
#	
#	Turns logging of SQL queries on and off.  When on, all SQL
#	queries are logged to the file "mysql.log" in the services
#	log directory.
#
#OPER_HELP_JUPE
#	Syntax: JUPE server [reason]
#	
#	Tells Services to jupiter a server -- that is, to create
#	a fake "server" connected to Services which prevents
#	the real server of that name from connecting.  The jupe
#	may be removed using a standard SQUIT.  If a reason is
#	given, it is placed in the server information field;
#	otherwise, the server information field will contain the
#	text "Jupitered by <nick>", showing the nickname of the
#	person who jupitered the server.
#	
#	Limited to Services admins.
#
#OPER_HELP_RAW
#	Syntax: RAW text
#	
#	Sends a string of text directly to the server to which
#	Services is connected.  This command has a very limited
#	range of uses, and can wreak havoc on a network if used
#	improperly.  DO NOT USE THIS COMMAND unless you are
#	absolutely certain you know what you are doing!
#	
#	Limited to Services admins.
#
#OPER_HELP_QUARANTINE
#	Syntax: QUARANTINE ADD regex reason
#	        QUARANTINE DEL {regex | list}
#	        QUARANTINE LIST [list]
#	
#	Allows IRC operators to manipulate the QUARANTINE list.
#	Any nick or channel name which matches a quarantine regex
#	will be disallowed.  The primary use of this feature is
#	closing channels of hostile bots and restricting the use
#	of Services nicknames.  This feature should not be used
#	to censor nicknames and channels.
#	
#	The regex parameter is a POSIX regular expression
#	(as used in the grep tool).  Full documentation on
#	regex format can be found in man 7 regex under unix.
#	Failing that, do a web search for "POSIX regular expression"
#	and you should find plenty!
#
#OPER_HELP_QUIT
#	Syntax: QUIT
#	
#	Causes Services to do an immediate shutdown; databases are
#	not saved.  This command should not be used unless
#	damage to the in-memory copies of the databases is feared
#	and they should not be saved.  For normal shutdowns, use the
#	SHUTDOWN command.
#	
#	Limited to Services admins.
#
#OPER_HELP_SHUTDOWN
#	Syntax: SHUTDOWN
#	
#	Causes Services to save all databases and then shut down.
#	
#	Limited to Services admins.
#
#OPER_HELP_RESTART
#	Syntax: RESTART
#	
#	Causes Services to save all databases and then restart
#	(i.e. exit and immediately re-run the executable).
#	
#	Limited to Services admins.	
#
#OPER_HELP_KILLCLONES
#	users were killed. It's usefull for removing numerous 
#	clones from the network.
#	
#	Limited to Services admins.
#
#OPER_HELP_ROTATELOG
#	Syntax: ROTATELOG
#	
#	Renames service's log file. The current date, in the form
#	YYYYMMDD, is used to generate the new filename.
#	Example: services-19991231.log
#	
#	Limited to Services root.

BAD_PASS_NICK_NAME
	Schlechtes Passwort: Es ist das selbe wie dein Nickname.

BAD_PASS_USERNAME
	Schlechtes Passwort: Es ist das selbe wie dein Benutzername (ident).

BAD_PASS_REALNAME
	Schlechtes Passwort: Es ist das selbe wie dein "Realname".

BAD_PASS_NICK_OR_LINK
	Schlechtes Passwort: Es ist das selbe wie einer deiner anderen registrierten oder "gelinkten" Nicknamen.

BAD_PASS_CHANNEL_NAME
	Schlechtes Passwort: Es ist das selbe wie der Raumname.

BAD_PASS_WAY_TOO_SHORT
	Schlechtes Passwort: Es ist ^BVIEL^B zu kurz!

BAD_PASS_TOO_SHORT
	Schlechtes Passwort: Es ist zu kurz!

BAD_PASS_NOT_ENOUGH_DIFF
	Schlechtes Passwort: Es enthält nicht genügend ^Bverschiedene^B Zeichen.

BAD_PASS_TOO_SIMPLISTIC
	Schlechtes Passwort: Es ist zu einfach zu erraten.

BAD_PASS_NAT_INS
	Schlechtes Passwort: Es sieht wie eine Versicherungs- oder Sozialversicherungsnummer aus.

BAD_PASS_DICT_WORD
	Schlechtes Passwort: Es basiert auf einem Wort aus dem Lexikon.

BAD_PASS_REV_DICT_WORD
	Schlechtes Passwort: Es basiert auf einem umgedrehten Wort aus dem Lexikon.

WARN_BAD_PASS
	Dein Passwort ist vermutlich unsicher, wir raten dir wirklich ein anderes zu nehmen.
	Es wurde allerdings vorübergehend akzeptiert.
