# Services is copyright (c) 1996-1999 Andrew Church.
#     E-mail: <achurch@dragonfire.net>
# Services is copyright (c) 1999-2000 Andrew Kempe.
#     E-mail: <theshadow@shadowfire.org>
# This program is free but copyrighted software; see the file COPYING for
# details.

LANG_NAME
	Türkçe (Turkish)
###################################################
# Translated by mikado (mikado@holyfire.com)      #
# Updates from SuPeRGiRL (piano3@superonline.com) #
###################################################

###########################################################################
#
# General messages
#
###########################################################################

# strftime() format strings.  man 3 strftime for information on the
# meanings of the format specifiers.  Short version:
#	%a = weekday name (short)	%H = hour
#	%b = month name (short)		%M = minute
#	%d = day of month		%S = second
#	%Y = year			%Z = time zone

# This is used as the format string for strftime() for a date and time
# together.
STRFTIME_DATE_TIME_FORMAT
	%b %d %H:%M:%S %Y %Z
# This is used as the format string for strftime() for a date alone in
# short format (English: without weekday).
STRFTIME_SHORT_DATE_FORMAT
	%b %d %Y

# These tell our strftime() what the names of months and days are.  If you
# don't use %a, %A, %b, or %B in your strftime() strings above, you can
# leave these empty.  However, if you enter names, they MUST stay in order,
# one per line, and the list MUST be complete!

# %a
STRFTIME_DAYS_SHORT
	Paz
	Pzt
	Sal
	Çar
	Per
	Cum
	Cmt
# %A
STRFTIME_DAYS_LONG
	Pazar
	Pazartesi
	Salý
	Çarsamba
	Perþembe
	Cuma
	Cumartesi
# %b
STRFTIME_MONTHS_SHORT
	Oca
	Þub
	Mar
	Nis
	May
	Haz
	Tem
	Aðu
	Eyl
	Ekm
	Ksm
	Ara
# %B
STRFTIME_MONTHS_LONG
	Ocak
	Þubat
	Mart
	Nisan
	Mayýs
	Haziran
	Temmuz
	Aðustos
	Eylül
	Ekim
	Kasým
	Aralýk

# This is used in ChanServ/NickServ INFO displays.
COMMA_SPACE
	,

# Various error messages.
USER_RECORD_NOT_FOUND
	Dahili hata - Kullanýci kaydý bulunamadý.	
UNKNOWN_COMMAND
	%s diye bir komut yok.
UNKNOWN_COMMAND_HELP
	%s diye bir komut yok. "/msg %s HELP" yazýnýz.
SYNTAX_ERROR
	Kullanimi: %s
MORE_INFO
	Daha fazla bilgi için /msg %s HELP %s yazýnýz.
NO_HELP_AVAILABLE
	%s hakkýnda yardým bulunamadý.
	
BAD_USERHOST_MASK
	"mask"  kullanýcý@IP þeklinde tanýmlanmalý.

PASSWORD_INCORRECT
	Geçersiz þifre.
ACCESS_DENIED
	Bu komuta eriþiminiz yok.
PERMISSION_DENIED
	Bu komutu kullanma izniniz yok.

MORE_OBSCURE_PASSWORD
	Lütfen daha kolay olmayan bir þifre ile yeniden deneyiniz. 
	Þifreler en az beþ karakter olmalý,tahmin edilmesi kolay olan (adýnýz,doðum
	tarihiniz vb.gibi).
	þeyler olmamalý ve boþluk içermemelidir.

PASSWORD_TRUNCATED
	Uyarý:  Þifreniz %d karaktere kýsaltýlmýþtýr.

NICK_NOT_REGISTERED
	Nick'iniz kayýtlý deðil.

NICK_NOT_REGISTERED_HELP
	Nick'iniz kayýtlý deðil. /msg %s HELP yazarak nasýl kaydedebileceðiniz
	konusunda detaylý bilgi alabilirsiniz.

NICK_X_NOT_REGISTERED
	%s kayýtlý bir nick deðil.
NICK_X_NOT_IN_USE
	%s nick'i su anda kullanimda degil.
NICK_X_FORBIDDEN
	%s kaydedilemez ve/veya kullanýlamaz.
CHAN_X_NOT_REGISTERED
	%s kanalý kayýtlý deðil.
CHAN_X_NOT_IN_USE
	%s kanalý þu an kullanýmda deðil.
CHAN_X_FORBIDDEN
	%s kanalý kaydedilemez ve/veya kullanýlamaz.
NICK_IDENTIFY_REQUIRED
	Bu komutu kullanabilmeniz için sifrenizi girmelisiniz.
	/msg %s IDENTIFY þifreniz yazarak yeniden deneyiniz.
CHAN_IDENTIFY_REQUIRED
	Bu komutu kullanabilmek için þifrenizi girmelisiniz.
	(/msg %s IDENTIFY %s þifreniz) yazarak yeniden deneyiniz.
	
###########################################################################
#
# NickServ messages
#
###########################################################################

# Automatic messages
NICK_IS_REGISTERED
	Bu kayitli bir nick'tir. Lütfen deðiþtiriniz.
	(Eðer sizinse,/msg %s IDENTIFY þifreniz yazýnýz).
NICK_IS_SECURE
	Bu kayýtlý ve þifre korumalý bir nick'tir.Eðer sizinse
	/msg %s IDENTIFY þifreniz yazýnýz. Aksi takdirde lütfen
	baþka bir nick seçiniz.
NICK_MAY_NOT_BE_USED
	Bu nick'in kullanýmý yasaklanmýþtýr.  Lütfen deðiþtiriniz.

# REGISTER responses
NICK_REGISTRATION_FAILED
	Nick kaydý baþarýsýz.
NICK_REG_PLEASE_WAIT
	Lütfen %d saniye bekleyerek yeniden deneyiniz.
NICK_CANNOT_BE_REGISTERED
	%s nick'i kaydedilemez.
NICK_ALREADY_REGISTERED
	%s daha önceden kaydedilmiþ.
NICK_REGISTERED
	%s nick'i baþarý ile kaydedilmiþtir.
NICK_PASSWORD_IS
	Nick sifreniz: %s 'dir. Lütfen þifrenizi unutmayýnýz ve kimseye söylemeyiniz.

# IDENTIFY responses
NICK_IDENTIFY_FAILED
	Tanýmlama baþarýsýz.
NICK_IDENTIFY_SUCCEEDED
	Þifre kabul edildi.

# DROP responses
NICK_DROPPED
	Nick'inizin kaydý silinmiþtir. (dropped).
NICK_X_DROPPED
	%s nick'inin kaydý silinmiþtir (dropped).

# SET responses
NICK_SET_UNKNOWN_OPTION
	Varolmayan SET seçeneði: %s.
NICK_SET_UNKNOWN_OPTION_OR_BAD_NICK
	Varolmayan SET seçeneði: %s, veya nick kayýtlý deðil.

# SET PASSWORD responses
NICK_SET_PASSWORD_CHANGED
	Þifreniz deðiþtirildi.
NICK_SET_PASSWORD_CHANGED_TO
	Nick þifresi %s olarak deðiþtirildi.

# SET URL responses
NICK_SET_URL_CHANGED
	URL %s olarak deðiþtirildi.
NICK_SET_URL_UNSET
	URL silindi.

# SET EMAIL responses
NICK_SET_EMAIL_CHANGED
	E-mail adresi %s olarak deðiþtirildi.

# SET LANGUAGE responses
NICK_SET_LANGUAGE_UNKNOWN
	Varolmayan dil numarasý: %d.  /msg %s HELP SET LANGUAGE yazarak mevcut dillerin bir listesini alabilirsiniz.
NICK_SET_LANGUAGE_CHANGED
	Dil Türkçe 'ye çevrilmiþtir.

# SET SECURE responses
NICK_SET_SECURE_ON
	Güvenlik seçeneði: AKTÝF.
NICK_SET_SECURE_OFF
	Güvenlik seçeneði: ÝPTAL.

# SET PRIVATE responses
NICK_SET_PRIVATE_ON
	Private seçeneði: AKTÝF.
NICK_SET_PRIVATE_OFF
	Private seçeneði: ÝPTAL.

# SET HIDE responses
NICK_SET_HIDE_EMAIL_ON
	E-mail adresiniz %s INFO listelerinden gizlendi.
NICK_SET_HIDE_EMAIL_OFF
	E-mail adresiniz %s INFO listelerinde görülebilecek.
NICK_SET_HIDE_MASK_ON
	"last seen" 'iniz %s INFO listelerinden gizlendi.
NICK_SET_HIDE_MASK_OFF
	"last seen" 'iniz %s INFO listelerinde görülebilecek.
NICK_SET_HIDE_QUIT_ON
	Son "/quit" mesajýnýz %s INFO listelerinden gizlendi.
NICK_SET_HIDE_QUIT_OFF
	Son "/quit" mesajýnýz %s INFO listelerinde görülebilecek.

# SET NOEXPIRE responses
NICK_SET_NOEXPIRE_ON
	%s nick'i zaman aþýmýndan çýkarýldý
NICK_SET_NOEXPIRE_OFF
	%s nick'i zaman aþýmýna eklendi.

# ACCESS responses
NICK_ACCESS_ALREADY_PRESENT
	%s eriþim listenizde zaten var.
NICK_ACCESS_ADDED
	%s eriþim listenize eklendi.
NICK_ACCESS_NOT_FOUND
	%s eriþim listenizde bulunamadý.
NICK_ACCESS_DELETED
	%s eriþim listenizden silindi.
NICK_ACCESS_LIST
	Eriþim Listesi:
NICK_ACCESS_LIST_X
	%s nickinin eriþim listesi:

# LINK responses
NICK_LINK_FAILED
	Nickler arasý baðlantý baþarýsýz.
NICK_NO_LINK_SAME
	Bir nick kendine linklenemez!
NICK_LINKED
	Nick'iniz %s'e linklendi.

# UNLINK responses
NICK_NOT_LINKED
	Nick baðlantýsý yok.
NICK_UNLINKED
	Nick'inizin %s ile olan baðlantisi (link) silindi.

# INFO responses
NICK_INFO_REALNAME
	      %s     isim: %s
NICK_INFO_ADDRESS
	Son gorülen adres: %s
NICK_INFO_TIME_REGGED
	     Kayýt zamaný: %s (%s ago)
NICK_INFO_LAST_SEEN
	Son görülen zaman: %s (%s ago)
NICK_INFO_LAST_QUIT
	  Son quit mesajý: %s
NICK_INFO_URL
	              URL: %s
NICK_INFO_EMAIL
	    E-mail adresi: %s
NICK_INFO_OPTIONS
	            Ekler: %s
# These strings MUST NOT be empty
NICK_INFO_OPT_SECURE
	Ek Güvenlik
NICK_INFO_OPT_PRIVATE
	Özel
NICK_INFO_OPT_NONE
	YOK

# LIST responses
NICK_LIST_HEADER
	%s 'a uyan veriler:
NICK_LIST_RESULTS
	liste sonu - %d/%d .

# RECOVER responses
NICK_NO_RECOVER_SELF
	NickServ'e kendinizi killetemezsiniz!
NICK_RECOVERED
	Nick'inizi kullanan kiþinin IRC baðlantýsý kesilmiþtir.
	/msg %s RELEASE %s yazarak bir dakikalik süreden önce nick'inizi alabilirsiniz.

# RELEASE responses
NICK_RELEASE_NOT_HELD
	%s nick'i zaten serbest.
NICK_RELEASED
	Nickiniz serbest býrakýldý.

# GHOST responses
NICK_NO_GHOST_SELF
	NickServ'e kendinizi killetemezsiniz!
NICK_GHOST_KILLED
	Asýlý kalan nick'inizin baðlantýsý kesilmiþtir.

# FORBID responses
NICK_FORBID_SUCCEEDED
	%s nick'inin kullanýmý yasaklandý.
NICK_FORBID_FAILED
	%s nick'inin kullanýmý yasaklanamadý!

###########################################################################
#
# ChanServ messages
#
###########################################################################

# Access level descriptions
CHAN_LEVEL_AUTOOP
	Kanala giriþte ChanServ tarafýndan otomatik op.
CHAN_LEVEL_AUTOVOICE
	Kanala giriþte ChanServ tarafýndan otomatik voice (+v)
CHAN_LEVEL_AUTODEOP
	Kanal içinde op verilemez/alamaz.
CHAN_LEVEL_NOJOIN
	RESTRICTED seçeneðinin açýk olmasý durumunda kanala girilemez.
CHAN_LEVEL_INVITE
	INVITE komutunu kullanmaya yetkili.
CHAN_LEVEL_AKICK
	AKICK komutunu kullanmaya yetkili.
CHAN_LEVEL_SET
	SET komutunu kullanmaya yetkili (FOUNDER/PASSWORD hariç).
CHAN_LEVEL_CLEAR
	CLEAR komutunu kullanmaya yetkili.
CHAN_LEVEL_UNBAN
	UNBAN komutunu kullanmaya yetkili.
CHAN_LEVEL_OPDEOP
	OP/DEOP komutlarýný kullanmaya yetkili.
CHAN_LEVEL_ACCESS_LIST
	Kanalýn eriþim listesini (access list) görmeye yetkili.
CHAN_LEVEL_ACCESS_CHANGE
	Kanalýn eriþim listesini (access list)
	deðiþtirmeye yetkili.
CHAN_LEVEL_MEMO_READ
	Kanala gelen notlarý okumaya yetkili.

# Automatic responses
CHAN_IS_REGISTERED
	Bu kanal %s ile kaydedilmiþtir.
CHAN_MAY_NOT_BE_USED
	Bu kanal kullanýlamaz.
CHAN_NOT_ALLOWED_TO_JOIN
	Bu kanalda bulunmaya yetkili deðilsiniz.

# REGISTER responses
CHAN_REGISTER_NOT_LOCAL
	Local kanallar kaydedilemezler.
CHAN_MUST_REGISTER_NICK
	Öncelikle nick'inizi kaydetmelisiniz. /msg %s HELP yazarak daha fazla bilgi alabilirsiniz.
CHAN_MUST_IDENTIFY_NICK
	Lütfen önce /msg %s IDENTIFY þifre
	komutu ile kendinizi tanýtýnýz.
CHAN_MAY_NOT_BE_REGISTERED
	%s kanalý kaydedilemez.
CHAN_ALREADY_REGISTERED
	%s kanalý daha önceden kaydedilmiþ.
CHAN_MUST_BE_CHANOP
	Kanalý kaydedebilmek için op olmalýsýnýz.
CHAN_REACHED_CHANNEL_LIMIT
	%d kanallýk limitinizi doldurdunuz.
CHAN_EXCEEDED_CHANNEL_LIMIT
	%d kanallýk limitinizi aþtýnýz.
CHAN_REGISTRATION_FAILED
	Kanal kaydý (registration) baþarýsýz.
CHAN_REGISTERED
	%s kanalý %s nick'inize kaydedilmiþtir.
CHAN_PASSWORD_IS
	Kanal þifrenizi (%s) kimseye söylemeyiniz ve unutmayýnýz.

# IDENTIFY responses
CHAN_IDENTIFY_SUCCEEDED
	Þifre kabul edildi: %s kanalýnda en yüksek düzeyde yetki kazandýnýz.

# DROP responses
CHAN_DROPPED
	%s isimli kanalýnýzýn kaydý silinmiþtir (dropped).

# SET responses
CHAN_SET_UNKNOWN_OPTION
	Varolmayan SET özelliði: %s.

# SET FOUNDER responses
CHAN_SET_FOUNDER_TOO_MANY_CHANS
	%s çok fazla kanala sahip.
CHAN_FOUNDER_CHANGED
	%s kanalýnýn sahibi (founder) %s olarak deðiþtirildi.

# SET SUCCESSOR responses

# SET PASSWORD responses
CHAN_PASSWORD_CHANGED
	%s kanalýnýn þifresi deðiþtirildi.
CHAN_PASSWORD_CHANGED_TO
	%s kanalýnýn þifresi %s olarak deðiþtirildi.

# SET DESC responses
CHAN_DESC_CHANGED
	%s kanalýnýn açýklamasý (Description) %s olarak deðiþtirildi.

# SET URL responses
CHAN_URL_CHANGED
	%s kanalýnýn web adresi (URL) %s olarak deðiþtirildi.
CHAN_URL_UNSET
	%s kanalýnýn web adresi (URL) silindi.

# SET EMAIL responses
CHAN_EMAIL_CHANGED
	%s kanalýnýn E-Mail adresi %s olarak deðiþtirildi.
CHAN_EMAIL_UNSET
	%s kanalýnýn E-mail adresi silindi.

# SET ENTRYMSG responses
CHAN_ENTRY_MSG_CHANGED
	%s kanalý için giriþ mesajý deðiþtirildi.
CHAN_ENTRY_MSG_UNSET
	%s kanalý için giriþ mesajý silindi.

# SET MLOCK responses
CHAN_SET_MLOCK_KEY_REQUIRED
	MLOCK +k komutu için bir anahtar (key) belirtmelisiniz.
CHAN_SET_MLOCK_LIMIT_REQUIRED
	MLOCK +l komutu icin bir "limit" belirtmelisiniz.
CHAN_SET_MLOCK_LIMIT_POSITIVE
	MLOCK +l olarak pozitif bir sayý kullanmalisiniz.
CHAN_SET_MLOCK_UNKNOWN_CHAR
	Varolmayan mode %c dikkate alýnmadý.
CHAN_MLOCK_CHANGED
	%s kanalýndaki "mode lock" %s olarak deðiþtirildi.
CHAN_MLOCK_REMOVED
	%s kanalýndaki "mode lock" silindi.

# SET KEEPTOPIC responses
CHAN_SET_KEEPTOPIC_ON
	Topic hatýrlama (retention) AKTÝF.
CHAN_SET_KEEPTOPIC_OFF
	Topic hatýrlama (retention) KAPALI.

# SET TOPICLOCK responses
CHAN_SET_TOPICLOCK_ON
	Topic kilidi (topiclock) AKTÝF.
CHAN_SET_TOPICLOCK_OFF
	Topic kilidi (topiclock) KAPALI.

# SET PRIVATE responses
CHAN_SET_PRIVATE_ON
	Özel (PRIVATE) AKTÝF.
CHAN_SET_PRIVATE_OFF
	Özel (PRIVATE) KAPALI.

# SET SECUREOPS responses
CHAN_SET_SECUREOPS_ON
	Kýsýtlý Op (SECUREOPS) AKTÝF.
CHAN_SET_SECUREOPS_OFF
	Kýsýtlý Op (SECUREOPS) KAPALI.

# SET LEAVEOPS responses

# SET RESTRICTED responses
CHAN_SET_RESTRICTED_ON
	Kýsýtlý eriþim AKTÝF.
CHAN_SET_RESTRICTED_OFF
	Kýsýtlý eriþim KAPALI.

# SET SECURE responses
CHAN_SET_SECURE_ON
	Güvenlik AKTÝF.
CHAN_SET_SECURE_OFF
	Güvenlik KAPALI.

# SET OPNOTICE responses

# SET NOEXPIRE responses
CHAN_SET_NOEXPIRE_ON
	%s kanalý zaman aþýmýndan çýkarýldý .
CHAN_SET_NOEXPIRE_OFF
	%s kanalý zaman aþýmýna eklendi.

# ACCESS messages
CHAN_ACCESS_LEVEL_NONZERO
	Eriþim seviyesi sýfýrdan farklý bir sayý olmalýdýr.
CHAN_ACCESS_LEVEL_RANGE
	Eriþim seviyesi %d ve %d arasýnda olmalýdýr.
CHAN_ACCESS_NICKS_ONLY
	Kanalýn eriþim listesi sadece kayýtlý (registered) nick'lerden oluþabilir.
CHAN_ACCESS_LEVEL_UNCHANGED
	%s nick'inin %s kanalýndaki eriþim seviyesi zaten %d.
CHAN_ACCESS_LEVEL_CHANGED
	%s nick'inin %s kanalýndaki eriþim seviyesi %d olarak deðiþtirildi.
CHAN_ACCESS_ADDED
	%s nick'i %s kanalýnýn eriþim listesine %d seviyeden eklendi.
CHAN_ACCESS_NO_SUCH_ENTRY
	(#%d) þeklinde bir kayýt %s kanalýnýn eriþim listesinde bulunamadý.
CHAN_ACCESS_NOT_FOUND
	%s  %s kanalýnýn eriþim listesinde bulunamadý.
CHAN_ACCESS_DELETED
	%s  %s kanalýnýn eriþim listesinden silindi.
CHAN_ACCESS_LIST_HEADER
	%s Kanalý Eriþim Listesi:
	  Sýra   Seviye  Nick
CHAN_ACCESS_LIST_FORMAT
	  %3d  %4d  %s%s%s%s

# AKICK responses
CHAN_AKICK_ALREADY_EXISTS
	%s nick'i %s kanalýnýnn akick listesinde zaten var.
CHAN_AKICK_REACHED_LIMIT
	Bir kanalýn akick listesinde en fazla %d kiþi olabilir.
CHAN_AKICK_ADDED
	%s nick'i %s kanalýnýn akick listesine eklendi.
CHAN_AKICK_DELETED
	%s  %s kanalýnýn akick listesinden silindi.
CHAN_AKICK_LIST_HEADER
	%s kanalýnýn akick listesi:
CHAN_AKICK_LIST_FORMAT
	  %3d %s%s

# LEVELS responses
CHAN_LEVELS_RANGE
	Seviye %d ve %d arasýnda olmalýdýr.
CHAN_LEVELS_CHANGED
	%s kanalýnda %s için gereken seviye %d olarak deðisti.
CHAN_LEVELS_UNKNOWN
	Varolmayan seçenek %s .  /msg %s HELP LEVELS DESC komutu ile geçerli listeyi alýnýz.
CHAN_LEVELS_LIST_HEADER
	%s kanalýndaki eriþim haklarý:
CHAN_LEVELS_LIST_DISABLED
	    %-*s  (devre dýþý
CHAN_LEVELS_LIST_NORMAL
	    %-*s  %d
CHAN_LEVELS_RESET
	%s için gereken eriþim haklarý sýfýrlanmýþtýr

# INFO responses
CHAN_INFO_HEADER
	  %s Kanalý Bilgisi
CHAN_INFO_FOUNDER
	   Kanal Sahibi: %s (%s)
CHAN_INFO_NO_FOUNDER
	   Kanal Sahibi: %s
CHAN_INFO_DESCRIPTION
	       Açýklama: %s
CHAN_INFO_TIME_REGGED
	   Kayýt tarihi: %s
CHAN_INFO_LAST_USED
	   Son Kullaným: %s
CHAN_INFO_LAST_TOPIC
	      Son Topic: %s
CHAN_INFO_TOPIC_SET_BY
	   Topici yazan: %s
CHAN_INFO_URL
	            URL: %s
CHAN_INFO_EMAIL
	  E-mail adresi: %s
CHAN_INFO_OPTIONS
	          Ekler: %s
CHAN_INFO_OPT_PRIVATE
	  Gizli (LIST komutunda cikmaz)
CHAN_INFO_OPT_KEEPTOPIC
	  Topic Hatirlama (retention)
CHAN_INFO_OPT_TOPICLOCK
	  Topic Kilidi (topiclock)
CHAN_INFO_OPT_SECUREOPS
	  Kýsýtlý Op
CHAN_INFO_OPT_RESTRICTED
	  Kýsýtlý Giris
CHAN_INFO_OPT_SECURE
	  Ek güvenlik
CHAN_INFO_OPT_NONE
	  YOK
CHAN_INFO_MODE_LOCK
	  Mode Kilidi: %s

# LIST responses
CHAN_LIST_HEADER
	%s 'a uyan verilerin listesi:
CHAN_LIST_END
	Liste sonu - %d/%d veri listelendi.

# INVITE responses

# OP responses

# DEOP responses

# UNBAN responses
CHAN_UNBANNED
	%s kanalindaki ban'iniz kaldýrýldý.

# CLEAR responses
CHAN_CLEARED_BANS
	%s kanalýndaki bütün ban'lar (+b) kaldýrýldý.
CHAN_CLEARED_MODES
	%s kanalýndaki bütün mode'lar (+bntispklm) kaldýrýldý.
CHAN_CLEARED_OPS
	%s kanalýndaki +o mode'u kaldýrýldý.(mass deop)
CHAN_CLEARED_VOICES
	%s kanalýndaki +v mode'u kaldýrýldý.(mass devoice)
CHAN_CLEARED_USERS
	%s kanalýndaki tüm kullanýcýlar atýldý.

# FORBID responses
CHAN_FORBID_SUCCEEDED
	%s kanalýnýn kullanýmý yasaklandý.(forbidden)
CHAN_FORBID_FAILED
	%s kanalýnýn kullanýmý yasaklanamadý!

###########################################################################
#
# MemoServ messages
#
###########################################################################

# Automatic messages
MEMO_HAVE_NEW_MEMO
	1 yeni notunuz var.
MEMO_HAVE_NEW_MEMOS
	%d yeni notunuz var.
MEMO_TYPE_READ_LAST
	/msg %s READ LAST yazarak okuyabilirsiniz.
MEMO_TYPE_READ_NUM
	/msg %s READ %d yazarak okuyabilirsiniz.
MEMO_TYPE_LIST_NEW
	/msg %s LIST NEW yazarak bir liste alabilirsiniz.
MEMO_AT_LIMIT
	UYARI: Maximum not sayýnýza ulaþtýnýz (%d).
	Mevcut notlardan bir kýsmýný silmediðiniz takdirde yeni notlar alamayacaksýnýz.
MEMO_OVER_LIMIT
	UYARI: Maximum not sayinizi astiniz (%d).
	Mevcut notlardan bir kýsmýný silmediðiniz takdirde yeni notlar alamayacaksýnýz.
MEMO_NEW_MEMO_ARRIVED
	%s nick'inden yeni bir notunuz var.
	/msg %s READ %d yazarak okuyabilirsiniz.

# Multi-use responses
MEMO_HAVE_NO_MEMOS
	Size herhangi bir not yok.
MEMO_X_HAS_NO_MEMOS
	%s nick'inin herhangi bir notu yok.

# SEND responses
MEMO_SEND_PLEASE_WAIT
	Lütfen SEND komutunu yeniden kullanmadan önce %d saniye bekleyiniz.
MEMO_X_GETS_NO_MEMOS
	%s nick'ine not býrakýlamaz.
MEMO_X_HAS_TOO_MANY_MEMOS
	%s nick'i þu anda limitini doldurduðundan daha fazla not alamaz.
MEMO_SENT
	Notunuz  %s nick'ine gönderilmiþtir..

# LIST responses
MEMO_HAVE_NO_NEW_MEMOS
	Herhangi bir notunuz yok.
MEMO_X_HAS_NO_NEW_MEMOS
	%s nick'inin yeni notu yok.
MEMO_LIST_MEMOS
	%s nick'ine ait notlar.  Okumak için, /msg %s READ num yazýnýz.
MEMO_LIST_NEW_MEMOS
	%s nick'i için yeni notlar var. Okumak için, /msg %s READ num yazýnýz.
MEMO_LIST_CHAN_MEMOS
	%s kanalý için notlar var. Okumak için, /msg %s READ %s num yazýnýz.
MEMO_LIST_CHAN_NEW_MEMOS
	%s kanalýnýn yeni notu var. Okumak için, /msg %s READ %s num yazýnýz.
MEMO_LIST_HEADER
	 Sýra  Gönderen            Tarih/Saat
MEMO_LIST_FORMAT
	%c%3d  %-16s  %s

# READ responses
MEMO_HEADER
	%d numaralý not %s nick'inden (%s) .  Silmek için, /msg %s DEL %d yazýnýz.
MEMO_CHAN_HEADER
	%d numaralý not %s nick'inden (%s).  Silmek için, /msg %s DEL %s %d yazýnýz.
MEMO_TEXT
	%s

# DEL responses
MEMO_DELETED_NONE
	Herhangi bir not silinmedi.

# SET responses
MEMO_SET_UNKNOWN_OPTION
	Varolmayan seçenek: %s.

# SET NOTIFY responses
MEMO_SET_NOTIFY_ON
	%s artýk size gönderilen notlarda online olduðunuzda ve/veya anýnda uyarý verecek.
MEMO_SET_NOTIFY_LOGON
	%s artýk size gönderilen notlarda online olduðunuzda ve/veya /AWAY'den döndüðünüzde uyarý verecek.
MEMO_SET_NOTIFY_NEW
	%s artýk size gönderilen notlarda anýnda uyarý verecek.
MEMO_SET_NOTIFY_OFF
	%s artýk size herhangi bir uyarý vermeyecek.

# SET LIMIT responses
MEMO_SET_YOUR_LIMIT_FORBIDDEN
	Not limitinizi belirleme yetkiniz yok.
MEMO_SET_LIMIT_FORBIDDEN
	%s nick'inin not limiti deðiþtirilemez.
MEMO_SET_YOUR_LIMIT_TOO_HIGH
	Not limitiniz en fazla %d olabilir.
MEMO_SET_LIMIT_TOO_HIGH
	%s nick'inin limiti en fazla %d olabilir.
MEMO_SET_LIMIT_OVERFLOW
	Not limitiniz fazla. Limit %d olarak düþürüldü.
MEMO_SET_YOUR_LIMIT
	Not limitiniz %d olarak ayarlandý.
MEMO_SET_YOUR_LIMIT_ZERO
	Limitiniz sýfýr olduðundan bir daha not alamayacaksýnýz.
MEMO_UNSET_YOUR_LIMIT
	Not limitiniz iptal edildi.
MEMO_SET_LIMIT
	%s nick'inin not limiti: %d.
MEMO_SET_LIMIT_ZERO
	%s nick'inin not limiti: 0.
MEMO_UNSET_LIMIT
	%s nick'inin not limiti: devre dýþý .

# INFO responses
MEMO_INFO_NO_MEMOS
	Þu anda notunuz yok.
MEMO_INFO_MEMO
	Þu anda 1 yeni notunuz var.
MEMO_INFO_MEMO_UNREAD
	Þu anda 1 notunuz var, ve henüz okumadýnýz.
MEMO_INFO_MEMOS
	Þu anda %d notunuz var.
MEMO_INFO_MEMOS_ONE_UNREAD
	Þu anda 1 'i okunmamýþ olmak üzere %d notunuz var.
MEMO_INFO_MEMOS_SOME_UNREAD
	Þu anda %d adedi okunmamis olmak uzere %d notunuz var.
MEMO_INFO_MEMOS_ALL_UNREAD
	Þu anda okunmamýþ  %d notunuz var.
MEMO_INFO_LIMIT
	Not limiti: %d.
MEMO_INFO_HARD_LIMIT
	Not limiti: %d, ve deðiþtirme yetkiniz yok.
MEMO_INFO_LIMIT_ZERO
	Not limiti: 0; herhangi bir not alamayacaksýnýz.
MEMO_INFO_HARD_LIMIT_ZERO
	Not limitiniz: 0; ve deðiþtirme yetkiniz yok. Herhangi bir not alamayacaksýnýz!
MEMO_INFO_NO_LIMIT
	Not limiti yok.
MEMO_INFO_X_NO_MEMOS
	%s nick'inin þu anda notu yok.
MEMO_INFO_X_MEMO
	%s nick'inin þu anda 1 notu var.
MEMO_INFO_X_MEMO_UNREAD
	%s nick'inin þu anda henüz okunmamýþ 1 notu var, ve henuz.
MEMO_INFO_X_MEMOS
	%s nick'inin %d notu var.
MEMO_INFO_X_MEMOS_ONE_UNREAD
	%s nick'inin 1 okunmamýþ olmak üzere %d notu var.
MEMO_INFO_X_MEMOS_SOME_UNREAD
	%s nick'inin %d okunmamýþ olmak üzere %d notu var.
MEMO_INFO_X_MEMOS_ALL_UNREAD
	%s nick'inin þu anda hepsi okunmuþ %d notu var.
MEMO_INFO_X_LIMIT
	%s nick'inin memo limiti: %d.
MEMO_INFO_X_HARD_LIMIT
	%s nick'inin memo limiti: %d.
MEMO_INFO_X_NO_LIMIT
	%s nick'inin memo limiti yok.

###########################################################################
#
# OperServ messages
#
###########################################################################

# General messages
OPER_BOUNCY_MODES
	Servisler mode deðiþtiremiyor. Server'larýnýzýn doðru ayarlandýðýna emin misiniz?
OPER_BOUNCY_MODES_U_LINE
	Servisler mode deðiþtiremiyor. Server'larinizin U:line 'larýný kontrol edin.

# GLOBAL responses

# STATS responses
OPER_STATS_UNKNOWN_OPTION
	Varolmayan STATS seçeneði: %s.
OPER_STATS_CURRENT_USERS
	Mevcut Kullanýcý : %d (%d ops)
OPER_STATS_MAX_USERS
	Maximum Kullanýcý: %d (%s)
# This is ugly, but at least it's language-portable...
OPER_STATS_UPTIME_DHMS
	Services %d gündür çalýþýyor.
OPER_STATS_UPTIME_1DHMS
	Services %d gündür çalýþýyor.
OPER_STATS_UPTIME_HMS
	Services %d saat, %d dakikadýr çalýþýyor.
OPER_STATS_UPTIME_HM1S
	Services %d saat, %d dakikadýr çalýþýyor.
OPER_STATS_UPTIME_H1MS
	Services %d saat, %d dakikadýr çalýþýyor.
OPER_STATS_UPTIME_H1M1S
	Services %d saat, %d dakikadýr çalýþýyor.
OPER_STATS_UPTIME_1HMS
	Services %d saat, %d dakikadýr çalýþýyor.
OPER_STATS_UPTIME_1HM1S
	Services %d saat, %d dakikadýr çalýþýyor.
OPER_STATS_UPTIME_1H1MS
	Services %d saat, %d dakikadýr çalýþýyor.
OPER_STATS_UPTIME_1H1M1S
	Services %d saat, %d dakikadýr çalýþýyor.
OPER_STATS_UPTIME_MS
	Services %d saat, %d dakikadýr çalýþýyor.
OPER_STATS_UPTIME_M1S
	Services %d dakika, %d saniyedir çalýþýyor.
OPER_STATS_UPTIME_1MS
	Services %d dakika, %d saniyedir çalýþýyor.
OPER_STATS_UPTIME_1M1S
	Services %d dakika, %d saniyedir çalýþýyor.
OPER_STATS_USER_MEM
	Kullanici: %6d kayýt, %5d kB
OPER_STATS_CHANNEL_MEM
	Kanal    : %6d kayýt, %5d kB
OPER_STATS_NICKSERV_MEM
	NickServ : %6d kayýt, %5d kB
OPER_STATS_CHANSERV_MEM
	ChanServ : %6d kayýt, %5d kB
OPER_STATS_OPERSERV_MEM
	Haydar   : %6d kayýt, %5d kB
OPER_STATS_AKILL_COUNT
	Þu anki AKILL adedi         : %d
OPER_STATS_AKILL_EXPIRE_DAYS
	Varsayýlan AKILL zaman aþýmý: %d gün
OPER_STATS_AKILL_EXPIRE_DAY
	Varsayýlan AKILL zaman aþýmý: %d gün1 gün
OPER_STATS_AKILL_EXPIRE_HOURS
	Varsayýlan AKILL zaman aþýmý: %d gün%d saat
OPER_STATS_AKILL_EXPIRE_HOUR
	Varsayýlan AKILL zaman aþýmý: %d gün1 saat
OPER_STATS_AKILL_EXPIRE_MINS
	Varsayýlan AKILL zaman aþýmý: %d gün%d dakika
OPER_STATS_AKILL_EXPIRE_MIN
	Varsayýlan AKILL zaman aþýmý: %d gün1 dakika
OPER_STATS_AKILL_EXPIRE_NONE
	Varsayýlan AKILL zaman aþýmý: %d günYOK

# MODE responses

# CLEARMODES responses
OPER_CLEARMODES_DONE
	Kanal mode'larý ve banlar %s kanalýnda temizlendi.
OPER_CLEARMODES_ALL_DONE
	%s kanalýndaki bütün mode'lar temizlendi.

# KICK responses

# ADMIN responses
OPER_ADMIN_EXISTS
	%s zaten Services admin listesinde.
OPER_ADMIN_ADDED
	%s Services admin listesine eklendi.
OPER_ADMIN_TOO_MANY
	%s Services admin listesine eklendi.
OPER_ADMIN_REMOVED
	%s Services admin listesinden silindi.
OPER_ADMIN_NOT_FOUND
	%s Services admin listesinde bulunamadý.
OPER_ADMIN_LIST_HEADER
	Services admin listesi:

# OPER responses
OPER_OPER_EXISTS
	%s zaten Services operatör listesinde.
OPER_OPER_ADDED
	%s Services operatör listesine eklendi.
OPER_OPER_TOO_MANY
	Services operatör listesi dolu (%d); ekleme iptal edildi.
OPER_OPER_REMOVED
	%s Services operatör listesinden silindi.
OPER_OPER_NOT_FOUND
	%s Services operatör listesinde bulunamadý.
OPER_OPER_LIST_HEADER
	Services operatör listesi:
	
# AKILL responses
OPER_AKILL_NO_NICK
	Hatýrlatma: AKILL'lerde nick bulunamaz.  AKILL'inize nick yazmadýðýnýzdan emin olun.
OPER_AKILL_REMOVED
	%s AKILL listesinden silindi.
OPER_AKILL_NOT_FOUND
	%s AKILL listesinde bulunamadý.
OPER_AKILL_LIST_HEADER
	Su Anki AKILL Listesi:
# mask, reason
OPER_AKILL_LIST_FORMAT
	%-32s  %s
# mask, set-by, set-time, expires, reason
OPER_AKILL_VIEW_FORMAT
	%s (by %s on %s; %s)
	    %s
OPER_AKILL_NO_EXPIRE
	Zaman aþýmý yok.
OPER_AKILL_EXPIRES_SOON
	Ýlk veritabaný güncellemesinde silinecek.
OPER_AKILL_EXPIRES_M
	%d dakika sonra silinecek.
OPER_AKILL_EXPIRES_1M
	%d dakika sonra silinecek.
OPER_AKILL_EXPIRES_HM
	%d saat, %d dakika sonra silinecek.
OPER_AKILL_EXPIRES_H1M
	%d saat, %d dakika sonra silinecek.
OPER_AKILL_EXPIRES_1HM
	%d saat, %d dakika sonra silinecek.
OPER_AKILL_EXPIRES_1H1M
	%d saat, %d dakika sonra silinecek.
OPER_AKILL_EXPIRES_D
	%d gün sonra silinecek.
OPER_AKILL_EXPIRES_1D
	%d gün sonra silinecek.

# SET responses
OPER_SET_IGNORE_ON
	Ignore kodu AKTÝF.
OPER_SET_IGNORE_OFF
	Ignore kodu KAPALI.
OPER_SET_IGNORE_ERROR
	IGNORE ayarý ON veya OFF þeklinde olmalý.
OPER_SET_DEBUG_ON
	Services þu anda debug mode'unda çalýþýyor.
OPER_SET_DEBUG_OFF
	Services þu anda non-debug mode'unda çalýþýyor.
OPER_SET_DEBUG_LEVEL
	Services þu anda debug mode'unda (level %d).
OPER_SET_DEBUG_ERROR
	DEBUG ayarý ON, OFF formatýnda veya pozitif bir sayý olmalý.
OPER_SET_UNKNOWN_OPTION
	Varolmayan seçenek: %s.

# JUPE responses

# RAW responses

# RESTART responses
OPER_CANNOT_RESTART
	SERVICES_BIN tanýmlanmamýþ; restart edilemiyor.  Yeniden \2configure\2 ederek Services'i RESTART komutu eklenmiþ olarak derleyin.

# LISTIGNORE responses
OPER_IGNORE_LIST
	Services ignore listesi:
OPER_IGNORE_LIST_EMPTY
	Ignore listesi bos.

### Changes 5 September Monday / 15.58




###########################################################################
#
# NickServ help messages
#
###########################################################################

NICK_HELP
	%S size bir nick'i kaydetme ve baþkalarýnýn
	kullanmasýný engelleme imkaný saðlar.Eger nick %d gün
	kullanýlmazsa otomatik olarak drop edilecektir(kayýt silinecektir).
	Aþaðýdaki komutlarý nick'inizi kaydedebilmeniz ve sonrasý için
	/msg %S komut þeklinde kullanabilirsiniz.
	Belirli bir komut hakkýnda daha fazla bilgi almak içinse
	/msg %S help komut yazmanýz yeterlidir.
	
	    REGISTER   O an kullandýðýnýz nick'i sizin adýnýza kaydeder.
	    IDENTIFY   Sizi nick'in sahibi olarak tanýtýr.
	    ACCESS     Nick'inizin eriþim listesini düzenler.
	    SET        Çeþitli %S özelliklerini düzenler.
	    DROP       Nick'inizin kaydýný siler.
	    RECOVER    Nickinizi kullanan bir baþkasýný KILL'ler.
	    RELEASE    RECOVER komutundan sonra tutulan nick'i býrakýr.	
	
	Diðer Komutlar: UNLINK, GHOST, INFO, LIST, STATUS, AUTOJOIN
	
	ÖNEMLI: %S kullanýcýlarýn IRC'deki kimliklerini
	güvence altýna almak amacý ile düzenlenmiþtir.Kesinlikle bir
	baþkasýnýn nick'ini çalmayý kolaylaþtýrma amaçlý deðildir.
	%S'un amaç dýþý kullanýmý kesinlikle yasaktýr ve
	kötü amaçlý kullanýmýn tespit edilmesi halinde, 
	adýnýza kayýtlý nick ve kanallarýn
	yasaklanmasý ile sonuçlanacaktýr.

NICK_HELP_REGISTER
	Kullanýmý: REGISTER þifre
	
	Nick'inizi %S veritabanina kaydeder.Nick'iniz kaydedildikten
	sonra (register) SET ve ACCESS komutlarýný kullanarak
	ayarlarýnýzý istediðiniz þekilde deðiþtirebilirsiniz.Kayýt
	sýrasýnda kullandýðýnýz þifreyi unutmamanýz önemlidir.
	Þifrenizde küçük ve büyük harfler farklý karakterlerdir!
	
	Þifreniz hakkýnda öneriler:
	
	Þifrenizi seçerken gerçek adýnýz-soyadýnýz gibi kolay tahmin
	edilebilecek þeyleri kullanmamaya dikkat edin.Kýsa þifreler de güvenli
	deðildir.Bu nedenle en azýndan 5 karakter uzunluðunda ve
	içinde space (boþluk) karakteri bulunmayan,hatta içinde rakam olan
	karakter serileri seçmeniz yararýnýza olacaktýr.

NICK_HELP_IDENTIFY
	Kullanýmý: IDENTIFY þifre
	
	Kendinizi %S 'e nick'in sahibi olarak tanýtmanýzý saðlar.
	Ayrýca identify (kimlik denetimi),nick ayarlarýnýzla ilgili birçok komutu 
	kullanabilmeniz için  gereklidir.Þifreniz
	REGISTER komutu ile nickinizi ilk kayýt ettirdiðiniz sýrada 
	kullandýðýnýz  þifre ile ayný olmalýdýr.

NICK_HELP_DROP
	Kullanýmý: DROP
	
	Nick'inizin %S veritabanýndaki kaydýný siler.Drop
	edilen bir nick'i bir baþkasý kaydedebilir.Bu komutu
	kullanabilmeniz için önce þifrenizi kullanarak kendinizi
	tanýtmalýsýnýz.(/msg %S HELP IDENTIFY yazarak daha
	geniþ bilgi alabilirsiniz).

NICK_HELP_ACCESS
	Kullanýmý: ACCESS ADD mask
	           ACCESS DEL mask
	           ACCESS LIST
	
	Nick'inizin eriþim listesini düzenler ve/veya görüntüler.
	Bu liste %S tarafýndan otomatik olarak tanýnacak
	adresleri belirler.Bu durumda farklý bir adresle
	nickinizi kullanacaðýnýzda IDENTIFY komutunu ile önce
	kendinizi tanýtmanýz gerekir.
	
	Örnekler:
	
	    ACCESS ADD *!mikado@*.holyfire.com
	        mikado isimli kullanicinin holyfire.com
	        domain'indeki herhangi bir makineden tanýnmasi.
	
	    ACCESS DEL *!mikado@*.holyfire.com
	        Önceki komutu geri alýr.
	
	    ACCESS LIST
	        O anki access list'i görüntüler.

NICK_HELP_LINK
	Kullanýmý: LINK linkleneceðiniz nick linkleneceðiniz nickin þifresi
	
	Nick'inizi bir baþka nick ile "linkler".Linklemenin mantýðý
	iki farkli nick'in birbiri ile ayný özelliklere (ayrýcalýk
	ve kýsýtlamalar) sahip bir nick haline getirmektir.Link
	gerçeklestiði anda notlar,access list ortaklaþa hale
	gelecektir.Bu yüzden,( örneðin yenilerini alabilmek için) bazý
	notlari silmeniz gerekebilir.Link "DROP" komutunu
	kapsamayacak,bir nick'in kaydýnýn silinmesi halinde sadece
	link ortadan kalkacaktýr.
	
	Bu komutu kullanabilmeniz için önce þifrenizi kullanarak
	kendinizi tanýtmalý ve linklemek istediðiniz nick'in de
	þifresini belirtmelisiniz.

NICK_HELP_UNLINK
	Kullanýmý: UNLINK
	
	Nick'inizin bir baþkasý ile olan linkini siler.Link
	silindiði zaman örneðin ortak eriþim listesi alýnmakla
	beraber ortak notlar kopyalanmaz.Tüm notlar linki ayrýlan
	nick'in veritabaninda kayýtlý kalýr.

NICK_HELP_SET
	Kullanýmý: SET seçenek parametreler
	
	Çeþitli %S seçeneklerini ayarlar.
	Geçerli seçenek'ler:
	
	    PASSWORD   Nick'inizin þifresini deðiþtirir.
	    LANGUAGE   Services'in size hitap edeceði dili belirler.
	    URL        Belirtilen URL'yi nick'inizle iliþkilendirir.
	    EMAIL      Belirtilen E-Mail'i nick'inizle iliþkilendirir.
	    KILL       Kill (baðlantý kesme) seçeneðini açar/kapar.
	    SECURE     (Güvenlik)Security seçeneðini açar/kapar.
	    PRIVATE    /msg %S LIST komutu ile cikacak listede
	                  nick'inizin gorunmesini engeller.
	    HIDE       (Gizleme)Nick bilgilerinizin bir kýsmýný "INFO"
	                  kýsmýndan saklar.
	
	Bu komutu kullanabilmeniz için önce þifrenizi kullanarak
	kendinizi tanýtmalýsýnýz.(/msg %S HELP IDENTIFY
	yazarak daha geniþ bilgi alabilirsiniz).
	
	Belirli bir komut hakkýnda daha fazla bilgi almak için
	/msg %S HELP SET seçenek yazabilirsiniz.

NICK_HELP_SET_PASSWORD
	Kullanýmý: SET PASSWORD yeni þifre
	
	Sizi %S'e nick'in sahibi olarak tanýtacak olan þifreyi
	deðiþtirir.

NICK_HELP_SET_LANGUAGE
	Kullanýmý: SET LANGUAGE numara
	
	Services'in size hitap edeceði dili deðiþtirir.numara
	desteklenen dillerden birine ait bir numara olmalýdýr:

NICK_HELP_SET_URL
	Kullanýmý: SET URL url
	
	Belirtilen URL'yi nick'inizle iliþkilendirir.Bu URL INFO
	komutu ile nick'iniz hakkýnda alýnabilecek bilgiye (info)
	eklenecektir.

NICK_HELP_SET_EMAIL
	Kullanýmý: SET EMAIL adres
	
	Belirtilen E-Mail adresini nick'inizle iliþkilendirir.Bu
	E-Mail INFO komutu ile nick'iniz hakkýnda alýnabilecek
	bilgiye eklenecektir.

NICK_HELP_SET_SECURE
	Kullanýmý: SET SECURE {ON | OFF}
	
	%S's güvenlik fonksiyonlarýný açar/kapar. SECURE
	seçeneði aktif iken nick'inizin sahibi olarak tanýnmanýz
	için eriþim listenize bakýlmaksýzýn þifrenizi kullanarak
	kendinizi %S'e tanýtmanýz gerekir.Ancak %S 'in
	eriþim listesinde iseniz eðer,sizi tanýmamasýna ragmen
	"KILL" 'lemeyecektir.

NICK_HELP_SET_PRIVATE
	Kullanýmý: SET PRIVATE {ON | OFF}
	
	/msg %S LIST komutu ile cikacak listede nick'inizin
	görünmesini engeller.Ancak yine de INFO komutu ile
	nick'iniz hakkýnda bilgi alabilir.

NICK_HELP_SET_HIDE
	Kullanýmý: SET HIDE {EMAIL | USERMASK | QUIT} {ON | OFF}
	
	Nick bilgilerinizdeki bazý kýsýmlarý saklamanýzý saðlar.Her
	kullanýcýnýn "/msg %S INFO" komutu ile görebileceði
	bilgilerden E-mail adresinizi (EMAIL), IRC'de son olarak
	görüldüðünüz zaman (last seen) , user@host'u (USERMASK) ve
	son quit mesajýnýzdan (QUIT) istediklerinizi
	saklayabilirsiniz.Hangi kýsmýn saklanacaðýndan hemen sonraki
	parametre (OFF) veya (ON) þeklinde olmalýdýr.

NICK_HELP_RECOVER
	Kullanýmý: RECOVER nickiniz [þifreniz]
	
	Nick'inizi kullanan bir kullanýcýyý server'dan atmanýzý
	saðlar. Bu , "KILL" seçeneðinin aktif olmasý halinde
	%S 'in otomatik olarak yapacaðý iþlemdir.
	
	Bu komutu verdiðinizde , %S server'a belirtilen nick'i
	taþýyan bir clone -kopya kullanýcý- sokar.Bu durum da IRC server'larýnýn öteki
	kullanýcýyý hattan atmalarýna yol açar.Bu "clone" karþý
	tarafýn hemen yeniden baðlanýp nick'i yeniden almasýna imkan
	vermemek için bir dakika süreyle hatta kalýr.Nick'inizi bu
	bir dakikalýk sürenin sonunda geri alabilir veya RELEASE
	komutunu kullanarak daha erken serbest býrakýlmasýný
	saðlayabilirsiniz.(/msg %S HELP RELEASE)
	
	Bu komutu kullanabilmeniz için o anki /WHOIS 'iniz eriþim
	listenizde olmalý veya þifrenizi kullanarak kendinizi
	%S'e tanýtmalýsýnýz (identify).

NICK_HELP_RELEASE
	Kullanýmý: RELEASE nickiniz [þifreniz]
	
	%S tarafýndan tutulan nick'inizin , 1 dakikalýk süre
	dolmadan serbest býrakýlmasýný saðlar.
	
	Bu komutu kullanabilmeniz için o anki /WHOIS 'iniz eriþim
	listenizde olmalý veya þifrenizi kullanarak kendinizi
	%S'e tanýtmalýsýnýz.

NICK_HELP_GHOST
	Kullanýmý: GHOST nickiniz [þifreniz]
	
	IRC'de asýlý kalmýþ olan nick'inizi temizler."GHOST" genelde
	bilgisayarýnýzýn takýlmasý gibi bir sebeple siz IRC'den
	çýkmadan internet baðlantýnýzýn kesilmesi sonucunda
	server'da asýlý kalan nick'inize verilen addýr.
	
	Bu komutu kullanabilmeniz için, o anki /WHOIS 'iniz eriþim
	listenizde olmalý veya þifrenizi kullanarak kendinizi
	%S'e tanýtmalýsýnýz.	

NICK_HELP_INFO
	Kullanýmý: INFO nick
	
	Belirtilen nick'in bilgilerini (nick sahibi,seçenekler,URL
	vs.) listeler.

NICK_HELP_LIST
	Kullanýmý: LIST parametre
	
	Belirtilen parametreye uyan büyün kayýtlý nick'leri
	nick!user@host formatýnda listeler.PRIVATE özelliði aktif
	olan nick'ler bu listede görünmezler.
	
	Örnekler:
	
	    LIST *!mikado@holyfire.com
	        Sahibinin *!mikado@holyfire.com oldugu büytün kayýtlý
	        nickleri listeler.
	
	    LIST *Bot*!*@*
	        Ýsminde Bot kelimesi geçen kayýtlý nick'leri
	        listeler.(küçük büyük harf farký vardýr.yani "a" ve
	        "A" farklýdýr).
	
	    LIST *!*@*.holyfire.com
	        holyfire.com domain'ine ait bütün nick'leri
	        listeler.

NICK_HELP_STATUS
	Kullanýmý: STATUS [nick]
	
	Belirtilen kullanýcýnýn nick'in sahibi olrak tanýnýp
	tanýnmadýðýný görüntüler.Cevap þu þekildedir:
	
	    nick statu-kodu
	
	Burada nick sizin komutta belirttiðiniz nick iken statu-kodu
	aþaðýdakilerden biri olabilir:
	
	    0 - Bu kullanýcý hatta deðil (offline) veya bu kayýtlý
	            bir nick deðil.
	    1 - Bu kullanici %S tarafindan nick'in sahibi
	            olarak taninmamis.
	    2 - Bu kullanýcý eriþim listesi ile tanýnmýþ.
	    3 - Bu kullanýcý nick þifresi ile  tanýnmýþ.
	
	Her komutta 16 nick'e kadar belirtilebilir.Kalanlar dikkate
	alýnmayacaktýr.Eger nick belirtilmez ise herhangi bir hata
	mesajý verilmez.

NICK_SERVADMIN_HELP
	
	Asaðýdaki komutlar sadece Services admin'ler içindir:
	
	    GETPASS    Belirtilen nick'in þifresini verir.
	    FORBID     Belirtilen nick'in kullanýmýný yasaklar.
	
	Services admin'ler ayrýca "IDENTIFY" komutunu kullanmaksýzýn
	bir nick'in kaydýný silebilir (DROP), þifresini
	deðiþtirebilir ve/veya eriþim listesini
	(/msg %S ACCESS LIST nick) komutu ile
	görüntüleyebilirler.

NICK_SERVADMIN_HELP_DROP
	Kullanýmý: DROP nick
	
	Herhangi bir ek parametre kullanýlmadan,nickinizi %S
	veritabanýndan silen bu komutla,sadece Services admin 'ler
	belirtecekleri baþka nick'leri de %S veritabanýndan
	silebilirler (DROP).

NICK_SERVADMIN_HELP_SET
	
	Services admin'ler ayrýca NOEXPIRE komutunu da kullanmaya
	ve bu þekilde belirtilen nick'lerin zaman asýmý yüzünden
	silinmelerini (EXPIRE) engellemeye de yetkilidirler.Services
	admin'ler þifreyi girmelerine gerek olmaksýzýn
	belirtecekleri her nick'in ayarlarýný
	SET nick seçenek parametre komutu ile deðiþtirme yetkisine
	sahiptirler.

NICK_SERVADMIN_HELP_SET_NOEXPIRE
	Kullanýmý: SET nick NOEXPIRE {ON | OFF}
	
	ON parametresinin kullanýlmasý halinde,belirtilen nick zaman
	aþýmý nedeni ile %S veritabanýndan silinmeyecektir.
	
	Bu komutu sadece Services admin'ler kullanabilir.

NICK_SERVADMIN_HELP_FORBID
	Kullanýmý: FORBID nick
	
	Belirtilen nick'in kullanýmýný yasaklar.Yasaðýn kaldýrýlmasý için
	sözkonusu nick'in %S veritabanýndan silinmesi gerekir.(DROP)
	
	Bu komutu sadece Services admin'ler kullanabilir.

###########################################################################
#
# ChanServ help messages
#
###########################################################################

CHAN_HELP
	%S size bir kanalý kendi adýnýza kaydetme ve onu
	yönetme imkaný saðlar. %S çoðu zaman kötü
	niyetli kullanýcýlarýn kanalýnýzý ele geçirmelerini
	(takeover) , örneðin kanalda sadece önceden belirlenmiþ bir listedeki
	kiþilerin op olmalarýna izin vererek engeller.Mevcut
	komutlar aþaðýda listelenmiþtir.Kullanmak icin
	/msg %S komut yazabilirsiniz.Belirli bir komut hakkýnda daha
	ayrýntýlý bilgi almak içinse /msg %S HELP komut yazmanýz
	yeterlidir.
	
	    REGISTER   Kanali sizin adiniza %S veritabanina kaydeder.
	    IDENTIFY   Þifreniz ile birlikte sizi kanal sahibi olarak tanýtýr.
	    SET        Çeþitli kanal özelliklerini ve kanal bilgisini ayarlar.
	    ACCESS     Ayrýcalýklý kullanýcýlarýn listesini düzenler.
	    LEVELS     Kullanýcýlarýn eriþim seviyelerini (access level) yeniden düzenler.
	    AKICK      OtoKick listesini düzenler.
	    DROP       Kanalýn kaydýný %S veritabanýndan siler.
	
	Diðer Komutlar: INFO, LIST, INVITE, OP, DEOP, UNBAN,
	                CLEAR

CHAN_HELP_EXPIRES
	
	Unutulmamalýdýr ki %d gün süresince kullanýlmayan kanallar otomatik
	olarak silinirler (mesela access listesindeki kimsenin bu süre içerisinde
	kanala girmemesi durumunda).

CHAN_HELP_REGISTER
	Kullanýmý: REGISTER kanal þifre açýklama
	
	Bir kanalý sizin adýnýza kaydeder.Bu komutu kullanabilmek
	içöin sözkonusu kanalda operatör olmanýz gerekir.Þifreniz
	daha sonra IDENTIFY komutunu kullanarak kendinizi kanalýn
	sahibi olarak %S'e tanýtacaðýnýz þifre olduðundan
	kolay tahmin edilemeyecek ve sizin de unutmayacaðýnýz bir
	þifre seçmeniz önemlidir.Açýklama kýsmý ise,kanal hakkýnda
	genel bir taným niteliðindedir.Bir kanalý kaydettiðinizde
	(register)o kanalýn sahibi olarak (founder) tanýnýrsýnýz.
	Kanal founder'ý kanalda her türlü ayar ve deðiþiklikleri yapma
	hakkýna sahiptir.Ayrica %S founder nick'ine kanala her
	giriþinde otomatik olarak op verecektir.ACCESS ve/veya
	LEVELS komutlarýný kullanarak (/msg %S HELP ACCESS) founder
	yetkilerinden bazýlarýný diðer kullanýcýlara da vermek
	mümkündür.
	
	ÖNEMLI NOT: Bir kanalý adýnýza register edebilmek icin
	öncelikle nick'inizi register etmeniz gerekir.Eðer
	nick'inizi register etmediyseniz /msg %S HELP yazarak
	yardým alabilirsiniz.

CHAN_HELP_IDENTIFY
	Kullanýmý: IDENTIFY kanal þifre
	
	Sizi %S'e belirtilen kanalýn founder'i olarak tanýtýr.
	Pek çok komutu kullanabilmek için önce founder
	olarak tanýtýlmanýz gerekir.Þifre kýsmýna,kanalýn  kayýt
	þifresi yazýlacaktýr.
	

CHAN_HELP_DROP
	Kullanýmý: DROP kanal
	Belirtilen kanalýn kaydýný siler.Sadece founder tarafýndan
	IDENTIFY komutundan sonra kullanýlabilir.

CHAN_HELP_SET
	Kullanýmý: SET kanal seçenek parametre
	
	Kanal founder'ýnýn çeþitli kanal opsiyonlarýný ve
	bilgilerini deðiþtirmesi içindir.Founder önce IDENTIFY
	komutunu kullanarak kendini %S'e tanýtmalýdýr.
	
	Kanal Seçenekleri:
	
	    FOUNDER     Kanal founder'ýný belirtir.
	    PASSWORD    Founder þifresini deðiþtirir.
	    DESC        Kanal hakkýndaki açýklamayý deðiþtirir.
	    URL         Bir internet adresini kanal ile iliþkilendirir.
	    EMAIL       Bir E-Mail adresini kanal ile iliþkilendirir.
	    TOPIC       Kanal topic'ini deðiþtirir.
	    MLOCK       Kanal modlarýný kilitler/açar.
	    KEEPTOPIC   Kanal kullanýlmadýðý zaman topici hafýzada
	                   tutar.
	    TOPICLOCK   Kullanýlmasý durumunda topic ancak SET TOPIC
	                   ile deðiþir.
	    PRIVATE     Kanalý %S listesinden saklar.
	    SECUREOPS   Kanal access list'indeki oplar dýþýnda kimse
	                   op alamaz.
	    RESTRICTED  Kanala access list'te olanlar dýþýndakiler
	                   giremez.
	    SECURE      %S güvenlik fonksiyonlarini aktif hale
	                   getirir.
	
	/msg %S HELP seçenek yazarak belirli bir seçenek hakkýnda
	daha ayrýntýlý bilgi alabilirsiniz.

CHAN_HELP_SET_FOUNDER
	Kullanýmý: SET kanal FOUNDER nick
	
	Belirtilen kanalýn founder'ýný (sahibini) deðiþtirir.Yeni
	nick te %s'e kaydedilmiþ olmalýdýr.

CHAN_HELP_SET_PASSWORD
	Kullanýmý: SET kanal PASSWORD yeni þifre
	
	Kullanýcýnýn kanalýn sahibi olarak tanýnacaðý þifreyi
	belirler.

CHAN_HELP_SET_DESC
	Kullanýmý: SET kanal DESC açýklama
	
	Belirtilen kanal hakkýndaki,LIST ve INFO komutlari ile
	görünecek olan açýklamayý belirler.Kullanabilmek için
	founder level access gerekir.

CHAN_HELP_SET_URL
	Kullanýmý: SET kanal URL webadresi
	
	Belirtilmis URL'yi kanal ile iliþkilendirir.Bu URL "INFO"
	komutu ile kanal hakkýnda bilgi istendiðinde görünecektir.
	Eðer "webadresi" belirtilmezse o anki mevcut kanal "URL" 'si
	silinecektir.

CHAN_HELP_SET_EMAIL
	Kullanýmý: SET kanal EMAIL e-mail adresi
	
	Belirtilen e-mail adresini yine belirtilen kanal ile iliþkilendirir.
	Bir kullanýcý kanal hakkýnda INFO komutu ile bilgi istediðinde bu e-mail
	adresi de gözukür.Eger "e-mail adresi" belirtilmezse o anki mevcut
	kanal "EMAIL" 'i silinecektir.

CHAN_HELP_SET_ENTRYMSG
	Kullanýmý: SET kanal ENTRYMSG mesaj
	
	Kullanýcýlarýn kanala giriþte /notice þeklinde alacaklarý
	mesajý belirler.Eðer parametre kullanýlmazsa kanala giriþte
	mesaj atýlmaz.

CHAN_HELP_SET_TOPIC
	Kullanýmý: SET kanal TOPIC topic
	
	%S 'un kanala topic atmasýný saðlar.Özellikle
	TOPICLOCK özelliðinin aktif olduðu zamanlarda
	kullanýþlýdýr.

CHAN_HELP_SET_KEEPTOPIC
	Kullanýmý: SET kanal KEEPTOPIC {ON|OFF}
	
	Kanalýn topic hatýrlama (retention) opsiyonunu açar/kapar.
	topic retention aktifken, kanalýn topici %S tarafýndan
	son kullanýcý da kanaldan ayrýldýktan sonra dahi
	hatýrlanacak ve kanala bir kullanýcý geldiðinde yeniden
	yerine yazýlacaktýr.	

CHAN_HELP_SET_TOPICLOCK
	Kullanýmý: SET kanal TOPICLOCK {ON|OFF}
	
	Kanalýn topic kilidi (topiclock) opsiyonunu açar/kapar.
	topic lock aktifken, %S kanal topicinin SET TOPIC
	dýþýndaki herhangi bir komutla deðiþtirilmesine izin
	vermeyecektir.

CHAN_HELP_SET_MLOCK
	Kullanýmý: SET kanal MLOCK modlar
	
	Kanalýn mod-kilidi (mode-lock) parametresini açar/kapar.
	%S size bazý kanal modlarýnýn daimi olarak aktif veya
	kapalý tutma imkaný saðlar.Sözkonusu modlar i, k, l, m, n,
	p, s, ve t arasýndan bir veya birkaçý olabilir.modlar
	parametresinin þekli /MODE komutundaki kullanýmý ile
	aynýdýr.(Örneðin +nt , -isp gibi.)
	
	UYARI:  Eðer aþaðýdaki ikinci örnek þeklinde bir "MLOCK"
	yapacaksanýz ek olarak RESTRICTED opsiyonunu da
	kullanmalýsýnýz (bkz. HELP SET RESTRICTED).Aksi takdirde
	kanal boþ iken kanala giren herhangi biri kanal anahtarýný
	(KEY) görebilecektir!
	
	Örnekler:
	
	    SET #kanal MLOCK +nt-iklps
	        Kanalda +nt modlarý sürekli aktif (+) iken, i, k, l,
	        p, ve s off sürekli devre dýþý (-) kalacaktýr.Kanal
	        kodu m ise serbest birakýlmýþtýr.
	
	    SET #kanal MLOCK +knst-ilmp anahtarým
	        k, n, s, and t modlarý sürekli aktifken i, l, m, ve
	        p modlarý sürekli devre dýþýdýr.Ayrýca kanalýn
	        anahtarý da (KEY) "anahtarým" þeklinde sabit
	        tutulur.
	
	    SET #kanal MLOCK +
	        Mode-lock opsiyonunu kaldýrýr.Böylece bütün kanal
	        modlarý serbest kalýr.

CHAN_HELP_SET_PRIVATE
	Kullanýmý: SET kanal PRIVATE {ON|OFF}
	
	Kanalýn private özelliðini açar/kapar.private özelliði aktif
	olan kanallar /msg %S LIST komutu ile alýnabilecek listede
	görünmezler.
	
CHAN_HELP_SET_RESTRICTED
	Kullanýmý: SET kanal RESTRICTED {ON|OFF}
	
	Kanalýn RESTRICTED özelliðini açar/kapar.Bu özelliðin aktif
	olduðu bir kanalda, normalde kanalda op olmayan
	kullanýcýlar (yani negatif eriþim seviyesi olanlar) kanala
	giremeyeceklerdir.

CHAN_HELP_SET_SECURE
	Kullanýmý: SET kanal SECURE {ON|OFF}
	
	%S'in güvenlik özelliklerini açar/kapar.SECURE
	aktifken, kanala sadece kayýtlý nick'li kullanýcýlar
	kendilerini "IDENTIFY" komutu ile %s'e tanýttýktan
	sonra girebilirler.

CHAN_HELP_SET_SECUREOPS
	Kullanýmý: SET kanal SECUREOPS {ON|OFF}
	
	Kanalýn secure ops özelliðini açar/kapar.Bu özellik aktifken
	kanalýn eriþim listesinde olmayan kullancýlar op alamazlar.

#CHAN_HELP_ACCESS
#	Kullanýmý: ACCESS kanal ADD [nick] seviye}
#		   ACCESS kanal DEL {nick | sýra-no}
#		   ACCESS kanal LIST [mask]
#		
#	Kanalýn eriþim listesi'ni (access list) düzenler.Bu liste
#	kanalda kimlerin oto-op olacaðý ve/veya %S komutlarýna
#	kimlerin eriþebileceði gibi bilgileri saklar.Deðiþik eriþim
#	haklarý için farklý eriþim seviyeleri (access level)
#	belirlenmistir.Standart olan bu seviyelerin büyük bir kýsmý
#	kanal sahibi tarafýndan deðiþtirilebilir.(bkz.
#	/msg %S HELP ACCESS LEVELS) Bir kanalýn kullanýcý
#	listesinde (access list) bulunmayan her nick'in seviyesi
#	varsayýlan deðer olarak 0'dýr.
#	
#	ACCESS ADD komutu belirtilen nick'i, yine belirtilen seviye
#	ile kanalýn eriþim listesine eklemek icin kullanýlýr.Burada
#	sözedilen seviye komutu kullanan kullanýcýnýnkinden sayýca
#	daha düþük olmak zorundadýr.Eðer eklenmek istenen kullanýcý
#	zaten listedeyse de komutu veren kullanýcýnýn seviyesi
#	(access level) daha yüksek olmalýdýr.Baþka bir deyiþle
#	sizinkinden yüksek seviyeli bir kullanýcý için bu komutu
#	kullanamazsýnýz.
#	
#	ACCESS DEL komutu belirtilen nick'i kanalýn eriþim
#	listesinden siler.
#	
#	ACCESS LIST komutu kanalýn eriþim listesini (access list)
#	görüntüler.Eger ek bir parametre belirtilmiþse,sadece buna
#	uyan veriler listelenir.

CHAN_HELP_ACCESS_LEVELS
	Kullanýcý Seviyeleri
	
	Varsayýlan deðerler olarak aþaðýdaki seviyeler
	belirlenmiþtir:
	
	   Founder  Kanal sahibi olarak %S komutlarýna tam
	                yetki,kanala giriþte otomatik op.Dikkat
	                edilirse sadece bir kiþi founder
	                özelliklerine sahip olabilir.(ACCESS komutu
	                kullanýlarak paylaþýlamaz).
	        10  AKICK komutunu kullanma yetkisi,otomatik op.
	         5  Kanala giriþte otomatik op.
	         3  Kanala giriste otomatik voice (+v).
	         0  Sýradan kullanýcý.secure-ops özelliðinin aktif
			olmadýðý durumlarda op veya voice verilebilir.
	        <0  Kullanýcý kanalda op olamaz.
	
	Bu seviyeler LEVELS komutu kullanýlarak deðiþebilir ve/veya
	yenileri eklenebilir.Daha ayrýntýlý bilgi için
	/msg %S HELP LEVELS yaziniz.

CHAN_HELP_AKICK
	Kullanýmý: AKICK kanal ADD mask [sebep]
	           AKICK kanal DEL mask
	           AKICK kanal LIST	
	
	Kanalýn AutoKick listesi'ni düzenler.Eðer bu listedeki bir
	kullanýcý kanala girerse, %S önce bu kullanýcýyý
	banlayacak,hemen ardýndan da kickleyecektir.
	
	AKICK ADD komutu belirtilen nick veya mask'i kanalýn akick
	listesine alýr.Eðer belirtilmiþ ise sebep kick mesajý olarak
	kullanýlýr.
	
	AKICK DEL belirlenen nick veya mask'i kanalýn akick
	listesinden siler.Bu nick veya mask'a ait kanaldaki herhangi
	bir ban'i silmez.Bu ban'lar kanal op'lari tarafýndan
	kaldýrýlmalýdýr.
	
	AKICK LIST komutu kanalýn akick listesini görüntüler.Eðer ek
	bir parametre belirtilmiþse,sadece buna uyan veriler
	listelenir.

CHAN_HELP_LEVELS
	Kullanýmý: LEVELS kanal SET özellik seviye
	           LEVELS kanal {DIS | DISABLE} özellik
	           LEVELS kanal RESET
	
	LEVELS komutu kanaldaki eriþim seviyelerinin varsayýlan
	deðerlerini düzenlemek için kullanýlýr.Sürekli kullanýlan
	%S özelliklerine deðiþik numerik seviyeler vererek
	onlarý daha çok sayýda kullanýcýya açar veya kaparsýnýz.
	Ancak SET FOUNDER ve SET PASSWORD komutlarý hala kanal
	sahibine (founder) özeldir.
	
	LEVELS SET komutu ile bir veya birkaç özelliðin varsayýlan
	numerik deðerleri deðiþtirilebilir.LEVELS DISABLE veya DIS
	komutu ileyse bu özellik(ler) iptal edilebilir.
	
	LEVELS LIST komutuysa o anki eriþim seviyelerinin numerik
	deðerlerini listeler.LEVELS RESET komutu bütün numerik
	deðerleri baþlangýçtaki konumlarýna getirir.

CHAN_HELP_LEVELS_DESC
	Geçerli özelliklerin listesi asaðýdadýr.Dikkat edilirse
	AUTOOP ve NOJOIN azami deðerlerken; diðer hepsi asgari
	deðerlerdir.

CHAN_HELP_LEVELS_DESC_FORMAT
	    %-*s  %s

CHAN_HELP_INFO
	Kullanýmý: INFO kanal
	
	Belirtilen kanalýn bilgilerini görüntüler.Kanal bilgileri
	içinde bazý kýsýmlar saklanmýþ veya yazýlmamýþ olabilir.

CHAN_HELP_LIST
	Kullanýmý: LIST degisken
	
	Belirtilen deðiþkene uyan bütün kayýtlý kanallarý listeler.
	(PRIVATE seçeneði aktif olan kanallar listelenmez).
CHAN_HELP_OP
	Kullanýmý: OP kanal nick
	
	Belirtilen nick'e sözkonusu kanalda op verir.Varsayýlan
	deger olarak sadece kanaldaki eriþim seviyesi 5 ve üzeri
	kiþiler bu komutu kullanabilir.

CHAN_HELP_DEOP
	Kullanimi: DEOP kanal nick
	
	Belirtilen nick'in sözkonusu kanaldaki op'unu alýr.
	Varsayýlan deger olarak sadece kanaldaki eriþim seviyesi 5
	ve üzeri olan kiþiler bu komutu kullanabilir.

CHAN_HELP_INVITE
	Kullanimi: INVITE kanal
	
	%S 'un sizi belirtilen kanala çaðýrmasýný (INVITE)
	saðlar.Varsayýlan deger olarak sadece kanaldaki eriþim
	seviyesi 0'dan büyük olan kiþiler bu komutu kullanabilir.

CHAN_HELP_UNBAN
	Kullanýmý: UNBAN kanal
	
	%S 'un sizin kanala giriþinizi engelleyen tüm ban'lari
	kaldýrmasýný saðlar.Varsayýlan deðer olarak sadece kanaldaki
	eriþim seviyesi 0'dan büyük olan kiþiler bu komutu
	kullanabilir.

CHAN_HELP_CLEAR
	Kullanýmý: CLEAR kanal parametre
	
	%S'un kanaldaki belirtilen parametre'yi sýfýrlamasýný
	saðlar.Geçerli parametre'ler þunlardýr:
	
	     MODES    Kanaldaki bütün mode'larý sýfýrlar.
	                  (i,k,l,m,n,p,s,t).
	     BANS     Kanaldaki bütün ban'lari sýfýrlar.
	     OPS      Kanaldaki +o mode'unu (tüm kanal oplarýný)
	                  sýfýrlar.
	     VOICES   Kanaldaki +v mode'unu (tüm +'lari) sýfýrlar.
	     USERS    Kanaldaki tüm kullanýcýlarý atar.
	
	Varsayýlan deðer olarak sadece kanaldaki eriþim seviyesi 10
	veya üzeri olan kiþiler bu komutu kullanabilir.

CHAN_SERVADMIN_HELP
	
	Aþaðýdaki komutlarý sadece Services admin'ler kullanabilir.:
	
	    GETPASS   Belirtilen kanalýn þifresini verir.
	                 (eðer þifreleme (encryption) devre dýþýysa)
	    FORBID    Belirtilen kanalýn kullanýmýný yasaklar.
	    STATUS    Bir kullanýcýnýn kanaldaki eriþim seviyesini
	                 verir.
	
	Services admin'ler ayrýca IDENTIFY komutunu kullanmaksýzýn
	bir kanalýn kaydýný silebilir (DROP), þifresini
	deðiþtirebilir ve/veya eriþim listesine
	(/msg %S ACCESS kanal {ADD | DEL | LIST}) komutu ile
	müdahale edebilirler.

CHAN_SERVADMIN_HELP_DROP
	Kullanýmý: DROP kanal
	
	Belirtilen kanalýn kaydýný siler.Sadece Services admin'ler
	þifreyi girmelerine gerek olmadan istedikleri kanalýn
	kaydýný silebilirler.

CHAN_SERVADMIN_HELP_SET
	
	Services admin'ler NOEXPIRE komutu ile herhangi bir kanalýn
	zaman aþýmý yüzünden silinmesi özelliðini kapatabilirler.
	Buna ek olarak Services admin'ler bir kanalýn her
	özelliðini  kanal þifresini girmeksizin deðiþtirebilirler.

CHAN_SERVADMIN_HELP_SET_NOEXPIRE
	Kullanýmý: SET kanal NOEXPIRE {ON | OFF}
	
	Belirtilen kanalýn zaman aþýmý nedeniyle silineceði veya
	silinmeyecegini belirler.Bu özelligin ON þeklinde
	ayarlanmasý kanalýn zaman aþýmý ile silinmesini
	engelleyecektir.
	
	Bu komutu sadece Services admin'ler kullanabilir.

CHAN_SERVADMIN_HELP_LIST
	Kullanýmý: LIST deðiþken
	
	Belirtilen deðiþkene uyan tüm kanallarý,PRIVATE özelliði
	aktif olanlarý da içermek üzere listeler.

CHAN_SERVADMIN_HELP_FORBID
	Kullanýmý: FORBID kanal
	
	Belirtilen kanalýn kullanýmýný yasaklar.Yasaðýn kalkmasý için
	sözkonusu nick'in %S veritabanýndan silinmesi gerekir
	(DROP)
	
	Bu komutu sadece Services admin'ler kullanabilir.

CHAN_SERVADMIN_HELP_STATUS
	Kullanýmý: STATUS kanal nick
	
	Belirtilen nick'in kanal'daki eriþim seviyesini görüntüler.
	Cevabýn formu þu þekildedir:
	
	    STATUS kanal nick eriþim-seviyesi
	
	Bu komutu sadece Services admin'ler kullanabilir.


###########################################################################
#
# MemoServ help messages
#
###########################################################################

#MEMO_HELP
#	%S o anda IRC'de olan veya olmayan kullanýcýlara not
#	býrakmanýzý saðlar.Notu hem gönderen,hem de alacak kiþinin
#	nickleri kayýtlý olmalýdýr (register edilmiþ).
#	
#	%S komutlarý:
#	
#	    SEND   Belirtilen nick'e not býrakýr.
#	    LIST   Notlarýnýzý listeler.
#	    READ   Bir notu okumanýzý saðlar.
#	    DEL    Bir notu (veya hepsini) siler.
#	    SET    Notlarýnýzla ilgili seçenekleri düzenler.
#	
#	/msg %S HELP komut yazarak herhangi bir komut hakkýnda
#	daha ayrýntýlý bilgi alabilirsiniz.
#	
#	(*) Standart olarak,kanala eriþim (access) seviyesi en az 10 olan her
#	kullanýcý kanala býrakýlan notlarý okuyabilir.Bu seviye
#	%s LEVELS komutu ile deðiþtirilebilir.

MEMO_HELP_SEND
	Kullanýmý: SEND {nick | kanal} not-metni
	
	Belirtilen nick veya kanal'a not-metni'ni gönderir.
	Not gönderildiðinde karþý taraf notice þeklinde bir uyarý
	alacaktýr.Karþý taraftaki kanal veya nick kayýtlý olmalýdýr.

#MEMO_HELP_LIST
#	Kullanýmý: LIST kanal [NEW]
#	
#	O anki notlarýnýzý listeler.NEW parametresi kullanýldýðýnda
#	sadece yeni (yani okunmamýþ) notlarýnýzýn bir listesini
#	alýrsýnýz.Okunmamýþ notlarýn baþýnda "*" iþareti olacaktýr.

#MEMO_HELP_READ
#	Kullanýmý: READ [kanal] {num | LAST}
#	
#	num numaralý notu, veya LAST parametresi ile en son notunuzu
#	görüntüler.

#MEMO_HELP_DEL
#	Kullanýmý: DEL [kanal] {num[-num][,num...] | ALL}
#	
#	Eðer num kýsmýnda bir sayý verilmiþse o notu siler.
#	Birden fazla numara veya bir aralýk belirtebilirsiniz.
#	
#	ALL yazarak size ait tüm notlarý tek bir komutla silebilirsiniz.
#	
#	Örnekler:
#	
#	   DEL 1
#	      Ýlk notunuzu siler.
#	
#	   DEL 2-5,7-9
#	      2 ile 5 ve 7 ile 9 arasýndaki notlarý siler.

MEMO_HELP_SET
	Kullanýmý: SET seçenek parametre
	
	Çeþitli not seçeneklerini düzenler.Seçenek olarak:
	
	    NOTIFY      Yeni notlarda ne zaman uyarýlacaðýnýzý
	                    belirler.(sadece nick'ler icin)
	    LIMIT       Alabileceginiz en fazla not sayýsýný
	                    belirler.
	
	/msg %S HELP SET seçenek yazarak belirli bir komut
	hakkýnda ayrýntýlý bilgi alabilirsiniz.	

MEMO_HELP_SET_NOTIFY
	Kullanýmý: SET NOTIFY {ON | LOGON | NEW | OFF}
	
	Yeni notlar geldiðinde ne zaman uyarý alacaðýnýzý belirler.
	
	    ON      IRC'ye girer girmez /AWAY 'den döner dönmez ve
	                notlar size gönderildikleri anda
	                uyarýlýrsýnýz.
	    LOGON   IRC'ye girer girmez ve /AWAY 'den doner donmez
	                uyarýlýrsýnýz.
	    NEW     Sadece notlar size gönderildikleri anda
	                uyarýlýrsýnýz.
	    OFF     Bu seçenekle herhangi bir uyarý almazsýnýz.
	
	ON seçeneði aslýnda LOGON ve NEW þeklindedir.

MEMO_HELP_SET_LIMIT
	Kullanýmý: SET LIMIT [kanal] limit
	
	Sizin veya belirtilen kanalýn alabilecegi en fazla not
	sayýsýný belirler.Eger bu limiti 0 olarak ayarlarsanýz
	size not býrakýlamaz.Bu limit en fazla %d olabilir.

MEMO_HELP_INFO
	Kullanýmý: INFO [kanal]
	
	Notlarýnýzý ve ilgili seçenekleri listeler.Notlarýnýz
	okunmuþ ve okunmamýþlar olarak ayrýlýrken,alabileceðiniz
	en fazla not sayýsýný da bu listede görebilirsiniz.Ek bir
	parametre ile belirtilen bir kanal hakkýnda da benzer bir
	liste alýnabilir.

MEMO_SERVADMIN_HELP_SET_LIMIT
	Kullanýmý: SET LIMIT [kullanýcý | kanal] {limit | NONE} [HARD]
	
	Bir kullanýcý veya kanalýn alabileceði en fazla not sayýsýný
	belirler.Bu limiti 0 yapmak kullanýcýnýn not alamamasýný ve
	NONE yapmak ta kullanýcýnýn not limitinin kalkmasýný saðlar.
	Eðer bu komutu kullanýrken bir nick veya kanal belirtmezseniz kendi 
	limitinizi belirlemiþ olursunuz.
	
	HARD parametresini eklemek kullanýcýnýn limitini
	deðiþtirmesini engeller.
	
	SET LIMIT komutunu baþka bir nick belirterek sadece
	Services admin'ler kullanabilir.Diðer kullanýcýlar sadece
	kendi veya eriþimleri olan kanallarýn limitlerini
	belirleyebilirler,kendi limitlerini kaldýramazlar ve/veya
	kendilerine %d 'den yüksek bir limit veremezler.

MEMO_SERVADMIN_HELP_INFO
	Kullanýmý: INFO [nick | kanal]
	
	Bu komut tek baþýna kullanýldýðýnda not sayýnýzý,okunmuþ ve
	okunmamýþ olarak ayýrarak,not limitinizle beraber listeler.
	
	Eðer bir kanal belirtilmiþse ayný bilgiyi o kanal için
	listeler.
	
	Eger bir nick belirtilmiþse ayni bilgiyi o kanal için
	listeler.Bu haliyle sadece Services admin'ler kullanabilir.

###########################################################################
#
# OperServ help messages #mikado#
#
###########################################################################

#OPER_HELP
#	%S komutlar:
#	    GLOBAL      Butun kullanýcýlara notice atar.
#	    STATS       Services'in ve network'un durumu.
#	    OPER LIST   Services operator'leri listeler.
#	    ADMIN LIST  Services admin'leri listeler.
#	
#	Services operator ve admin'lere özel komutlar:
#	    MODE        Bir kanalýn mode'unu deðiþtirir.
#	    KICK        Kanaldaki bir kullanýcýyý atar.
#	    AKILL       AKILL listesini düzenler.
#	
#	Services admin'lere özel komutlar:
#	    OPER        Services operator listesini düzenler
#	    JUPE        Bir server'i "Jupiter" eder
#	    RAW         IRC sunucusuna bir RAW komutu yollar
#	    SET         Çeþitli Services seçeneklerini düzenler
#	    QUIT        Services'i kapatýr.
#	    RESTART     Veritabanýný kaydederek Services'i
#	                  yenýden baþlatýr.
#	
#	Services root'a özel komutlar:
#	    ADMIN       Services admin listesini düzenler
#	
#	Önemli Uyari: %S 'a giden bütün komutlar kaydedilir!

OPER_HELP_GLOBAL
	Kullanýmý: GLOBAL metin
	
	IRCop'larýn network'teki tüm kullanýcýlara mesaj
	göndermelerini saðlar.
	
	Mesaj %s nick'inden gelen bir notice þeklinde
	görünecektir.

OPER_HELP_STATS
	Kullanýmý: STATS [AKILL | ALL]
	
	Bu komut tek baþýna kullanýldýðýnda mevcut kullanýcý ve
	IRCop sayýsýný (Services hariç) , azami kullanýcý
	sayýsýný, Services'in çalýþmaya baþladýðý tarihi ve ne
	kadar zamandýr çalýþtýðýný görüntüler.
	
	AKILL seçeneði ile beraber kullanýldýðýnda ise o anki AKILL
	listesinin durumunu ve varsayýlan zaman aþýmýný görüntüler.
	
	ALL seçeneðini sadece Services admin'ler kullanabilir.Bu
	komut Services'in kullandýðý RAM'i görüntüler.Büyük
	network'larda kullanýlmasý Services'i kýsa süreyle
	dondurabileceðinden gereksiz yere kullanmayýnýz.
	
	STATS yerine UPTIME'i da kullanabilirsiniz.

OPER_HELP_OPER
	Kullanýmý: OPER ADD nick
	           OPER DEL nick
	           OPER LIST
	
	Services admin'lerin Services operatör listesini
	düzenlemelerini saðlar.Bu listedeki her kullanýcý %s'e
	þifresini girdikten sonra Services operatör komutlarýný
	kullanabilecektir.
	
	Komutun OPER LIST þeklini her IRCop kullanabilir.Bütün diðer
	kullanýmlar Services admin'lere özeldir.

OPER_HELP_ADMIN
	Kullanýmý: ADMIN ADD nick
	           ADMIN DEL nick
	           ADMIN LIST
	
	Services root'un Services admin listesini 
	düzenlemelerini saðlar.Bu listedeki her kullanýcý %s'e
	þifresini girdikten sonra Services admin komutlarýný
	kullanabilecektir.
	
	Komutun ADMIN LIST Bütün diðer
	kullanýmlar Services root'a özeldir.


OPER_HELP_MODE
	Kullanýmý: MODE kanal mode(lar)
	
	Services operator'larin herhangi bir kanalýn mode'larýný
	düzenlemelerini saðlar.Kullanýlacak parametreler /MODE
	komutununkilerle aynýdýr.
	
	Bu komutu sadece Services operatör'ler kullanabilir.

OPER_HELP_CLEARMODES
	Kullanýmý: CLEARMODES kanal [ALL]
	
	Kanaldaki bütün "binary mode"'larý (i,k,l,m,n,p,s,t) ve
	banlarý sýfýrlar.ALL parametresi ile kullanýlýðýnda op ve
	voice'ler de (+o ve +v) sýfýrlanýr.
	
	Bu komutu sadece Services operator'lar kullanabilir.

OPER_HELP_KICK
	Kullanýmý: KICK kanal kullanici [sebep]
	
	IRCop'larýn herhangi bir kanaldaki kullanýcýyý kanaldan
	atmalarýný saðlar.Kullanýlacak parametreler /KICK
	komutununkilerle aynýdýr.Kick mesajýnýn yanýnda komutu
	kullanan IRCop'un nick'i de görünecektir.Örnegin :
	
	*** SpamMan has been kicked off channel #kanal by %S
	(mikado (Flood))
	
	Bu komutu sadece Services operator'lar kullanabilir.

OPER_HELP_AKILL
	Kullanýmý: AKILL ADD [+sure] mask sebep
	           AKILL DEL mask
	           AKILL LIST [mask]
	           AKILL VIEW [mask]
	
	Services operators'larýn AKILL listesini düzenlemelerini
	saðlar.AKILL listesindeki bir kullanýcý baðlanmaya
	çalýþtýðýnda Services o kullanýcýyý KILL edecek ve
	destekleyen sunucu türlerinde network'teki tüm sunuculardan
	da kullanicýyý banlayacaktýr (K-Line).
	
	AKILL ADD komutu belirtilen user@host formatýndaki nick'i
	AKILL listesine belirtilmek zorunda olan sebep ile ekler.
	
	AKILL DEL komutu belirtilen veriyi AKILL listesinden siler.
	
	AKILL LIST tek basina AKILL listesinin tamamini listeler.
	Eðer bir seçenek verilmiþse , sadece ona uyan veriler
	listelenir.
	
	AKILL VIEW komutu ise akill'i ekleyen kiþinin nick'i,
	otomatik olarak silineceði suregibi ayrýntýlarý da
	görüntülemesi açýsýndan AKILL LIST'e göre daha
	kullanýþlýdýr.
	
	AKILL ADD komutu ile beraber bir zaman aþýmý süresi de
	belirtilebilir.Bu süre d (gün), h (saat) veya m (dakika)
	þeklinde olabilir.(1h30m) gibi kombinasyonlar geçersizdir.
	Varsayýlan seçenek gündür.Yani +30 30 gün anlamýna gelir.Bir
	AKILL'in otomatik olarak silinmemesi için süre kýsmýnda +0
	yazýlmasý gerekir.
	
	sebep sadece diðer IRCop'larýn görmesi içindir.Kullanýcýlar
	KILL'lenirken bu mesaj kullanýlmaz.
	
	Bu komutu sadece Services operator'lar kullanabilir.

OPER_HELP_JUPE
	Kullanýmý: JUPE sunucu
	
	Services bu komutu kullanýldýðýnda,belirtilen sunucu ile
	ayný isimdeki sahte bir sunucuyu network'e dahil ederek
	aslýnýn baðlanmasýný engeller.Komut standart olarak
	SQUIT komutu ile geri alýnabilir.
	
	Bu komutu sadece Services admin'ler kullanabilir.

OPER_HELP_RAW
	Kullanýmý: RAW [metin]
	
	Direk olarak Services'in baðlý olduðu sunucuya bir metin
	gönderir. Bu komutun oldukça kýsýtlý bir kullaným alaný
	vardýr ve ne yapýldýðýndan emin olmadýkça kesinlikle
	BU KOMUTU KULLANMAYIN
	
	Bu komutu sadece Services admin'ler kullanabilir.
	
OPER_HELP_SET
	Kullanýmý: SET secenek ayar
	
	Çeþitli Services seçeneklerini ayarlar.Mevcut seçenekler:
	
	    READONLY   read-only ve read-write arasýnda geçiþ
	    DEBUG      debug mode'u açar/kapar.
	
	Bu komutu Services admin'ler kullanabilir.

OPER_HELP_SET_DEBUG
	Kullanýmý: SET DEBUG {ON | OFF | num}
	
	Debug özelliðini açar/kapar.Bu özellik aktifken Services'a
	gelen ve Services'den gönderilen her veri kaydedilir.Eðer
	num belirtilmiþse debug özelliði bu seviyede baþlatýlýr.
	
	Bu özellik komut satýrýndaki -debug ile aynýdýr.

OPER_HELP_QUIT
	Kullanýmý: QUIT
	
	Services' kapatan bu komut kullanýldýðýnda veritabaný
	GÜNCELLENMEZ. Bu yüzden veritabanýnda veya Services'in
	çalýþtýðý makinede bir sorun çýktýðý durumlar dýþýnda
	kullanýlmamalýdýr.
	
	Bu komutu sadece Services admin'ler kullanabilir.

OPER_HELP_SHUTDOWN
	Kullanýmý: SHUTDOWN
	
	QUIT'in aksine bu komut kullanýldýðýnda Services bütün
	veritabanýný güncelledikten sonra kapanýr.
	
	Bu komutu sadece Services admin'ler kullanabilir.

OPER_HELP_RESTART
	Kullanýmý: RESTART
	
	Services'i, bütün veritabanýný güncelledikten sonra yeniden
	baþlatýr.
	
	Bu komutu sadece Services admin'ler kullanabilir.

