# Archivo de idioma Español.
#
# Services esta protegido por derechos de autor (c) 1996-1999 Andy Church.
#     E-mail: <achurch@dragonfire.net>
# Este programa es gratis pero esta registrado; vea el archivo COPYING para
# detalles.
# Traducción por:
# José R. Holzmann "Joselito" joselito@sandnet.org
# Raúl S. Villarreal "CheMaN" cheman@sandnet.org

# When translating this file to another language, keep in mind that the
# order of parameters for sprintf() is fixed in the source code, so your
# messages need to take the same parameters in the same order as the
# English messages do.  (Obviously, this doesn't hold for the strftime()
# format lines immediately below.)  If you can't get a natural translation
# of a message without changing the order of the parameters, let me know
# (achurch@dragonfire.net) which message is causing a problem and I'll see
# what I can do.
#
# In help messages, "%S" (capital S, not lowercase) refers to the name of
# the service sending the message; for example, in NickServ help messages,
# "%S" is replaced by "NickServ" (or whatever it is renamed to in
# config.h).  The %S's do not count as sprintf() parameters, so they can be
# rearranged, removed, or added as necessary.

###########################################################################
#
# Nombre de este idioma
#
###########################################################################

# For languages other than English, this string should have the following
# format:
#     language-name-in-language (language-name-in-English)
# For example, "Español (Spanish)" or "Français (French)".

LANG_NAME
	Español (Spanish)

###########################################################################
#
# Mensajes Generales
#
###########################################################################

# strftime() format strings.  man 3 strftime for information on the
# meanings of the format specifiers.  Short version:
#	%a = weekday name (short)	%H = hour
#	%b = month name (short)		%M = minute
#	%d = day of month		%S = second
#	%Y = year			%Z = time zone

# This is used to display a date and time together.
STRFTIME_DATE_TIME_FORMAT
	%b %d %H:%M:%S %Y %Z
STRFTIME_SHORT_DATE_FORMAT
# This is used as the format string for strftime() for a date alone in
# short format (English: without weekday).
	%b %d %Y

# These tell our strftime() what the names of months and days are.  If you
# don't use %a, %A, %b, or %B in your strftime() strings above, you can
# leave these empty.  However, if you enter names, they MUST stay in order,
# one per line, and the list MUST be complete!

# %a
STRFTIME_DAYS_SHORT
	Dom
	Lun
	Mar
	Mie
	Jue
	Vie
	Sab
# %A
STRFTIME_DAYS_LONG
	Domingo
	Lunes
	Martes
	Miércoles
	Jueves
	Viernes
	Sábado
# %b
STRFTIME_MONTHS_SHORT
	Ene
	Feb
	Mar
	Abr
	May
	Jun
	Jul
	Ago
	Sep
	Oct
	Nov
	Dic
# %B
STRFTIME_MONTHS_LONG
	Enero
	Febrero
	Marzo
	Abril
	Mayo
	Junio
	Julio
	Agosto
	Septiembre
	Octubre
	Noviembre
	Diciembre


# This is used in ChanServ/NickServ INFO displays.
COMMA_SPACE
	,


# Various error messages.
USER_RECORD_NOT_FOUND
	Error Interno - imposible procesar comando.
UNKNOWN_COMMAND
	Comando desconocido %s.
UNKNOWN_COMMAND_HELP
	Comando desconocido %s.  "/msg %s HELP" para ayuda.
SYNTAX_ERROR
	Sintaxis: %s
MORE_INFO
	/msg %s HELP %s para mas información.
NO_HELP_AVAILABLE
	No ayuda disponible para %s.

BAD_USERHOST_MASK
	Mascara debe ser en formato usuario@host.

ACCESS_DENIED
	Acceso negado.
PERMISSION_DENIED
	Permiso negado.
PASSWORD_INCORRECT
	Contraseña incorrecta.

MORE_OBSCURE_PASSWORD
	Favor de intentar de nuevo con una contraseña mas difícil. Contraseñas deben ser de 5 caracteres mínimo,no debe ser algo fácil de descifrar (ejemplo. su verdadero nombre o su nick), tampoco debe contener espacios.
PASSWORD_TRUNCATED
	Noticia:  Su contraseña a sido limitada a %d caracteres.

NICK_NOT_REGISTERED
	Su nick no esta registrado.
NICK_NOT_REGISTERED_HELP
	Su nick no esta registrado. Escriba /msg %s HELP para mayor información de como registrar su nick.
NICK_X_NOT_REGISTERED
	Nick %s no esta registrado.
NICK_X_NOT_IN_USE
	Nick %s no esta en uso en este momento.
NICK_X_FORBIDDEN
	Nick %s no puede ser registrado o utilizado.
CHAN_X_NOT_REGISTERED
	Canal %s no esta registrado.
CHAN_X_NOT_IN_USE
	Canal %s no existe.
CHAN_X_FORBIDDEN
	Canal %s no puede ser registrado o utilizado.
NICK_IDENTIFY_REQUIRED
	Contraseña es requerida para ese comando.
	Vuelva a intentar después de escribir /msg %s IDENTIFY contraseña.
CHAN_IDENTIFY_REQUIRED
	Contraseña es requerida para ese comando.
	Vuelva a intentar después de escribir /msg %s IDENTIFY contraseña.

###########################################################################
#
# Mensajes de Nickserv
#
###########################################################################

# Automatic messages
NICK_IS_REGISTERED
	Este nick es pertenece a otra persona. Favor de escoger otro.
	(Si este es su nick, escriba /msg %s IDENTIFY contraseña.)
NICK_IS_SECURE
	Este nick esta registrado y protegido.  Si es su nick,
	escriba /msg %s IDENTIFY contraseña.  De otra manera,
	escoga un nick diferente.
NICK_MAY_NOT_BE_USED
	Este nick no se puede utilizar.  Favor de escoger otro.

# REGISTER responses
NICK_REGISTRATION_FAILED
	Lo siento, el registro fallo.
NICK_REG_PLEASE_WAIT
	Favor de esperar %d segundos antes de volver a utilizar el comando REGISTER.
NICK_CANNOT_BE_REGISTERED
	Nick %s no puede ser registrado.
NICK_ALREADY_REGISTERED
	Nick %s ya esta registrado!
NICK_REGISTERED
	Nick %s esta registrado bajo tu cuenta: %s
NICK_PASSWORD_IS
	Tu contraseña es %s - recuerdala para uso posterior.

# IDENTIFY responses
NICK_IDENTIFY_FAILED
	Lo siento, la identificación fallo.
NICK_IDENTIFY_SUCCEEDED
	Contraseña aceptada - ahora eres reconocido.

# DROP responses
NICK_DROPPED
	Su nick a sido cancelado.
NICK_X_DROPPED
	Nick %s a sido cancelado.

# SET responses
NICK_SET_UNKNOWN_OPTION
	Opción SET %s desconocida.
NICK_SET_UNKNOWN_OPTION_OR_BAD_NICK
	Opción SET %s desconocida, o su nick no esta registrado.

# SET PASSWORD responses
NICK_SET_PASSWORD_CHANGED
	Contraseña cambiada.
NICK_SET_PASSWORD_CHANGED_TO
	Contraseña cambiada a %s.

# SET URL responses
NICK_SET_URL_CHANGED
	URL cambiada a %s.
NICK_SET_URL_UNSET
	URL deshabilitada.

# SET EMAIL responses
NICK_SET_EMAIL_CHANGED
	Dirección de E-mail cambiada a %s.

# SET LANGUAGE responses
NICK_SET_LANGUAGE_UNKNOWN
	Numero %d de lenguaje desconocido.  Escriba /msg %s HELP SET LANGUAGE para una lista de lenguajes disponibles.
NICK_SET_LANGUAGE_CHANGED
	Lenguaje cambiado a Español.

# SET SECURE responses
NICK_SET_SECURE_ON
	Opción SECURE activada.
NICK_SET_SECURE_OFF
	Opción SECURE desactivada.

# SET PRIVATE responses
NICK_SET_PRIVATE_ON
	Opción PRIVATE activada.
NICK_SET_PRIVATE_OFF
	Opción PRIVATE desactivada.

# SET HIDE responses
NICK_SET_HIDE_EMAIL_ON
	Su dirección de E-mail esta oculta para exhibición de %s INFO.
NICK_SET_HIDE_EMAIL_OFF
	Su dirección de E-mail esta visible para exhibición de %s INFO.
NICK_SET_HIDE_MASK_ON
	Su ultima mascara de usuario@host esta oculta para exhibición de %s INFO.
NICK_SET_HIDE_MASK_OFF
	Su ultima mascara de usuario@host esta visible para exhibición de %s INFO.
NICK_SET_HIDE_QUIT_ON
	Su ultimo mensaje de quit esta oculto para exhibición de %s INFO.
NICK_SET_HIDE_QUIT_OFF
	Su ultimo mensaje de quit esta visible para exhibición de %s INFO.

# SET NOEXPIRE responses
NICK_SET_NOEXPIRE_ON
	Nick %s no expirará.
NICK_SET_NOEXPIRE_OFF
	Nick %s si expirará.

# ACCESS responses
NICK_ACCESS_ALREADY_PRESENT
	Mascara %s ya existe en su lista de acceso.
NICK_ACCESS_ADDED
	%s agregada a su lista de acceso.
NICK_ACCESS_NOT_FOUND
	%s no encontrada en su lista de acceso.
NICK_ACCESS_DELETED
	%s borrada de su lista de acceso.
NICK_ACCESS_LIST
	Lista de acceso:
NICK_ACCESS_LIST_X
	Lista de acceso para %s:

# LINK responses
NICK_LINK_FAILED
	Lo siento, liga fallo.
NICK_NO_LINK_SAME
	No puede ligar su nick actual!
NICK_LINKED
	Su nick a sido ligado a %s.

# UNLINK responses
NICK_NOT_LINKED
	Su nick no esta ligado.
NICK_UNLINKED
	Su nick a sido desligado de %s.

# INFO responses
NICK_INFO_REALNAME
	%s es %s
NICK_INFO_ADDRESS
	        Ultima dirección: %s
NICK_INFO_TIME_REGGED
	        Hora de registro: %s (%s ago)
NICK_INFO_LAST_SEEN
	  Ultima hora que se vio: %s (%s ago)
NICK_INFO_LAST_QUIT
	Ultimo mensaje de salida: %s
NICK_INFO_URL
	                     URL: %s
NICK_INFO_EMAIL
	                  E-mail: %s
NICK_INFO_OPTIONS
	                Opciones: %s
# These strings MUST NOT be empty
NICK_INFO_OPT_SECURE
	Seguro
NICK_INFO_OPT_PRIVATE
	Privado
NICK_INFO_OPT_NONE
	Ninguna

# LIST responses
NICK_LIST_HEADER
	Lista de entradas que satisfacen %s:
NICK_LIST_RESULTS
	Fin de lista - %d/%d entradas mostradas.

# RECOVER responses
NICK_NO_RECOVER_SELF
	No puede recuperar su propio nick!
NICK_RECOVERED
	Usuario utilizando su nick a sido desconectado.
	/msg %s RELEASE %s para recuperar antes de un minuto.

# RELEASE responses
NICK_RELEASE_NOT_HELD
	Nick %s no esta siendo protegido en este momento.
NICK_RELEASED
	La protección de Services en su nick ha sido liberada.

# GHOST responses
NICK_NO_GHOST_SELF
	No puede aplicar ghost a si mismo!
NICK_GHOST_KILLED
	Ghost con su nick a sido desconectado.

# FORBID responses
NICK_FORBID_SUCCEEDED
	Nick %s esta prohibido de ser usado.
NICK_FORBID_FAILED
	No se puede prohibir nick %s!

###########################################################################
#
# Mensajes de ChanServ
#
###########################################################################

# Access level descriptions
CHAN_LEVEL_AUTOOP
	Status de operador de canal automático
CHAN_LEVEL_AUTOVOICE
	Modo +v automático
CHAN_LEVEL_AUTODEOP
	No puede ser operador de canal
CHAN_LEVEL_NOJOIN
	No puede entrar a canal si esta en modo restringido (RESTRICTED)
CHAN_LEVEL_INVITE
	Permitido a usar comando INVITE
CHAN_LEVEL_AKICK
	Permitido a usar comando AKICK
CHAN_LEVEL_SET
	Permitido a usar comando SET (no siendo FUNDADOR)
CHAN_LEVEL_CLEAR
	Permitido a usar comando CLEAR
CHAN_LEVEL_UNBAN
	Permitido a usar comando UNBAN
CHAN_LEVEL_OPDEOP
	Permitido a usar comandos OP/DEOP
CHAN_LEVEL_ACCESS_LIST
	Permitido a ver lista de acceso
CHAN_LEVEL_ACCESS_CHANGE
	Permitido a modificar lista de acceso
CHAN_LEVEL_MEMO_READ
	Permitido a ver mensajes del canal

# Automatic responses
CHAN_IS_REGISTERED
	Este canal esta registrado con %s.
CHAN_MAY_NOT_BE_USED
	Este canal no puede ser utilizado.
CHAN_NOT_ALLOWED_TO_JOIN
	Usted no puede permanecer en este canal.

# REGISTER responses
CHAN_REGISTER_NOT_LOCAL
	Canales locales no pueden ser registrados.
CHAN_MUST_REGISTER_NICK
	Usted debe de registrar primero su nick.  Escriba /msg %s HELP para obtener más información de como registrar nicks.
CHAN_MUST_IDENTIFY_NICK
	Por favor identifiquese primero con %s, utilizando el comando:
	/msg %s IDENTIFY contraseña
CHAN_MAY_NOT_BE_REGISTERED
	Canal %s no puede ser registrado.
CHAN_ALREADY_REGISTERED
	Canal %s ya esta registrado!
CHAN_MUST_BE_CHANOP
	Usted debe ser operador de canal para poder registrarlo.
CHAN_REACHED_CHANNEL_LIMIT
	Lo siento, usted ya alcanzo su limite de %d canales.
CHAN_EXCEEDED_CHANNEL_LIMIT
	Lo siento, usted ya excedió su limite de %d canales.
CHAN_REGISTRATION_FAILED
	Lo siento, el registro fallo.
CHAN_REGISTERED
	Canal %s registrado bajo su nick: %s
CHAN_PASSWORD_IS
	Su contraseña de canal es %s - recuerdela para uso posterior.

# IDENTIFY responses
CHAN_IDENTIFY_SUCCEEDED
	Contraseña aceptada - usted es reconocido con nivel de fundador de %s.

# DROP responses
CHAN_DROPPED
	Canal %s a sido cancelado.

# SET responses
CHAN_SET_UNKNOWN_OPTION
	Opción SET %s desconocida.

# SET FOUNDER responses
CHAN_SET_FOUNDER_TOO_MANY_CHANS
	%s tiene demasiados canales registrados.
CHAN_FOUNDER_CHANGED
	Fundador de %s cambiado a %s.

# SET SUCCESSOR responses

# SET PASSWORD responses
CHAN_PASSWORD_CHANGED
	%s contraseña cambiada.
CHAN_PASSWORD_CHANGED_TO
	%s contraseña cambiada a %s.

# SET DESC responses
CHAN_DESC_CHANGED
	Descripción de %s cambiada a %s.

# SET URL responses
CHAN_URL_CHANGED
	URL para %s cambiada a %s.
CHAN_URL_UNSET
	URL para %s desactivada.

# SET EMAIL responses
CHAN_EMAIL_CHANGED
	Dirección de E-mail para %s cambiada a %s.
CHAN_EMAIL_UNSET
	Dirección de E-mail para %s desactivada.

# SET ENTRYMSG responses
CHAN_ENTRY_MSG_CHANGED
	Mensaje de entrada para %s cambiado.
CHAN_ENTRY_MSG_UNSET
	Mensaje de entrada para %s desactivada.

# SET MLOCK responses
CHAN_SET_MLOCK_KEY_REQUIRED
	Parámetro requerido para MLOCK +k.
CHAN_SET_MLOCK_LIMIT_REQUIRED
	Parámetro requerido para MLOCK +l.
CHAN_SET_MLOCK_LIMIT_POSITIVE
	Parámetro para MLOCK +l debe ser un número positivo.
CHAN_SET_MLOCK_UNKNOWN_CHAR
	Modo %c es desconocido e ignorado.
CHAN_MLOCK_CHANGED
	Candado de modo de canal %s cambiado a %s.
CHAN_MLOCK_REMOVED
	Candado de modo de canal %s removido.

# SET KEEPTOPIC responses
CHAN_SET_KEEPTOPIC_ON
	Opción de retención de tópico activada.
CHAN_SET_KEEPTOPIC_OFF
	Opción de retención de tópico desactivada.

# SET TOPICLOCK responses
CHAN_SET_TOPICLOCK_ON
	Opción de candado de tópico activada.
CHAN_SET_TOPICLOCK_OFF
	Opción de candado de tópico desactivada.

# SET PRIVATE responses
CHAN_SET_PRIVATE_ON
	Opción de privacidad activada.
CHAN_SET_PRIVATE_OFF
	Opción de privacidad desactivada.

# SET SECUREOPS responses
CHAN_SET_SECUREOPS_ON
	Opción de Secure ops activada.
CHAN_SET_SECUREOPS_OFF
	Opción de Secure ops desactivada.

# SET LEAVEOPS responses

# SET RESTRICTED responses
CHAN_SET_RESTRICTED_ON
	Opción de acceso restringido activada.
CHAN_SET_RESTRICTED_OFF
	Opción de acceso restringido desactivada.

# SET SECURE responses
CHAN_SET_SECURE_ON
	Opción Secure activada.
CHAN_SET_SECURE_OFF
	Opción Secure activada.

# SET OPNOTICE responses

# SET NOEXPIRE responses
CHAN_SET_NOEXPIRE_ON
	Canal %s no expirará.
CHAN_SET_NOEXPIRE_OFF
	Canal %s si expirará.

# ACCESS messages
CHAN_ACCESS_LEVEL_NONZERO
	Nivel de acceso debe ser diferente de cero.
CHAN_ACCESS_LEVEL_RANGE
	Nivel de acceso debe estar entre %d y %d.
CHAN_ACCESS_NICKS_ONLY
	Lista de acceso de canal debe contener solamente nicks registrados.
CHAN_ACCESS_LEVEL_UNCHANGED
	Nivel de acceso para %s en %s no cambio de %d.
CHAN_ACCESS_LEVEL_CHANGED
	Nivel de acceso para %s en %s fue cambiado a %d.
CHAN_ACCESS_ADDED
	%s agregado a lista de acceso de %s con nivel %d.
CHAN_ACCESS_NO_SUCH_ENTRY
	Entrada desconocida (#%d) en la lista de acceso de %s.
CHAN_ACCESS_NOT_FOUND
	%s no fue encontrado en la lista de acceso de %s.
CHAN_ACCESS_DELETED
	%s borrado de la lista de acceso de %s .
CHAN_ACCESS_LIST_HEADER
	Lista de acceso de %s:
	  Num   Niv  Nick
CHAN_ACCESS_LIST_FORMAT
	  %3d  %4d  %s%s%s%s

# AKICK responses
CHAN_AKICK_ALREADY_EXISTS
	%s ya existe en la lista de autorkicks de %s.
CHAN_AKICK_REACHED_LIMIT
	Lo siento, solo puede tener %d mascaras de autokick en un canal.
CHAN_AKICK_ADDED
	%s agregado a la lista de autokick de %s.
CHAN_AKICK_DELETED
	%s borrado de la lista de autokick de %s.
CHAN_AKICK_LIST_HEADER
	Lista de autokick de %s:
CHAN_AKICK_LIST_FORMAT
	  %3d %s%s

# LEVELS responses
CHAN_LEVELS_RANGE
	Nivel debe de estar entre %d y %d.
CHAN_LEVELS_CHANGED
	Nivel para %s en canal %s cambiado a %d.
CHAN_LEVELS_UNKNOWN
	Opción %s desconocida.  Escriba /msg %s HELP LEVELS DESC para una lista de opciones validas.
CHAN_LEVELS_LIST_HEADER
	Configuración de nivel de acceso para canal %s:
CHAN_LEVELS_LIST_DISABLED
	    %-*s  (deshabilitado)
CHAN_LEVELS_LIST_NORMAL
	    %-*s  %d
CHAN_LEVELS_RESET
	Niveles de acceso de %s configurados a niveles originales.

# INFO responses
CHAN_INFO_HEADER
	Información de canal %s:
CHAN_INFO_FOUNDER
	           Fundador: %s (%s)
CHAN_INFO_NO_FOUNDER
	           Fundador: %s
CHAN_INFO_DESCRIPTION
	        Descripción: %s
CHAN_INFO_TIME_REGGED
	         Registrado: %s
CHAN_INFO_LAST_USED
	         Ultimo uso: %s
CHAN_INFO_LAST_TOPIC
	      Ultimo tópico: %s
CHAN_INFO_TOPIC_SET_BY
	Tópico ajustado por: %s
CHAN_INFO_URL
	                URL: %s
CHAN_INFO_EMAIL
	             E-mail: %s
CHAN_INFO_OPTIONS
	           Opciones: %s
CHAN_INFO_OPT_PRIVATE
	Privado
CHAN_INFO_OPT_KEEPTOPIC
	Retención de tópico
CHAN_INFO_OPT_TOPICLOCK
	Candado de tópico
CHAN_INFO_OPT_SECUREOPS
	Secure Ops
CHAN_INFO_OPT_RESTRICTED
	Acceso restringido
CHAN_INFO_OPT_SECURE
	Seguro
CHAN_INFO_OPT_NONE
	Ninguna
CHAN_INFO_MODE_LOCK
	    Candado de modo: %s

# LIST responses
CHAN_LIST_HEADER
	Lista de entradas que satisfacen %s:
CHAN_LIST_END
	Fin de lista - %d/%d entradas mostradas.

# INVITE responses

# OP responses

# DEOP responses

# UNBAN responses
CHAN_UNBANNED
	Usted a sido desbaneado de %s.

# CLEAR responses
CHAN_CLEARED_BANS
	Todos los bans en canal %s han sido borrados.
CHAN_CLEARED_MODES
	Todos los modos en canal %s han sido borrados.
CHAN_CLEARED_OPS
	Modo +o de operadores han sido desactivados del canal %s.
CHAN_CLEARED_VOICES
	Modo +v de algunos usuarios han sido desactivados del canal %s.
CHAN_CLEARED_USERS
	Todos los usuarios del han sido kickeados del canal %s.

# FORBID responses
CHAN_FORBID_SUCCEEDED
	Canal %s esta prohibido de ser utilizado.
CHAN_FORBID_FAILED
	No fue posible prohibir el canal %s!

###########################################################################
#
# Mensajes de MemoServ
#
###########################################################################

# Automatic messages
MEMO_HAVE_NEW_MEMO
	Usted tiene 1 mensaje nuevo.
MEMO_HAVE_NEW_MEMOS
	Usted tiene %d mensajes nuevos.
MEMO_TYPE_READ_LAST
	Escriba /msg %s READ LAST para leer.
MEMO_TYPE_READ_NUM
	Escriba /msg %s READ %d para leer.
MEMO_TYPE_LIST_NEW
	Escriba /msg %s LIST NEW para listar mensajes nuevos.
MEMO_AT_LIMIT
	Atención: Usted ha llegado al máximo numero de mensajes (%d). Usted no podrá recibir mensajes nuevos hasta que borre algunos de los mensajes que tiene.
MEMO_OVER_LIMIT
	Atención: Usted ha pasado el limite máximo de mensajes (%d).  Usted no podrá recibir mensajes nuevos hasta que borre algunos de los mensajes que tiene.
MEMO_NEW_MEMO_ARRIVED
	Usted tiene un mensaje nuevo de %s.
	Escriba /msg %s READ %d para leerlo.

# Multi-use responses
MEMO_HAVE_NO_MEMOS
	Usted no tiene mensajes.
MEMO_X_HAS_NO_MEMOS
	%s no tiene mensajes.

# SEND responses
MEMO_SEND_PLEASE_WAIT
	Por favor espere %d segundos antes de utilizar el comando SEND nuevamente.
MEMO_X_GETS_NO_MEMOS
	%s no puede recibir mensajes.
MEMO_X_HAS_TOO_MANY_MEMOS
	%s tiene demasiados mensajes y no puede recibir mas.
MEMO_SENT
	Mensaje enviado a %s.

# LIST responses
MEMO_HAVE_NO_NEW_MEMOS
	Usted no tiene nuevos mensajes.
MEMO_X_HAS_NO_NEW_MEMOS
	%s no tiene nuevos mensajes.
MEMO_LIST_MEMOS
	Mensajes para %s.  Para leer, escriba: /msg %s READ numero
MEMO_LIST_NEW_MEMOS
	Nuevos mensajes para %s.  Para leer, escriba: /msg %s READ numero
MEMO_LIST_CHAN_MEMOS
	Mensaje para %s.  Para leer, escriba: /msg %s READ %s numero
MEMO_LIST_CHAN_NEW_MEMOS
	Nuevos mensajes para %s.  Para leer, escriba: /msg %s READ %s numero
MEMO_LIST_HEADER
	 Num  Remitente         Día/Hora
MEMO_LIST_FORMAT
	%c%3d  %-16s  %s

# READ responses
MEMO_HEADER
	Mensaje %d de %s (%s).  Para borrar, escriba: /msg %s DEL %d
MEMO_CHAN_HEADER
	Mensaje %d de %s (%s).  Para borrar, escriba: /msg %s DEL %s %d
MEMO_TEXT
	%s

# DEL responses
MEMO_DELETED_NONE
	Ningún mensaje fue borrado.

# SET responses
MEMO_SET_UNKNOWN_OPTION
	Opción SET %s desconocida.

# SET NOTIFY responses
MEMO_SET_NOTIFY_ON
	%s le notificara sus mensajes cuando se conecte o cuando le sean enviados.
MEMO_SET_NOTIFY_LOGON
	%s le notificara sus mensajes cuando se conecte o cancele su modo /AWAY.
MEMO_SET_NOTIFY_NEW
	%s le notificara sus mensajes cuando le sean enviados.
MEMO_SET_NOTIFY_OFF
	%s no le notificara sus mensajes.

# SET LIMIT responses
MEMO_SET_YOUR_LIMIT_FORBIDDEN
	Usted no puede cambiar su limite de mensajes.
MEMO_SET_LIMIT_FORBIDDEN
	El limite de mensajes para %s no puede ser cambiado.
MEMO_SET_YOUR_LIMIT_TOO_HIGH
	Usted no puede ajustar su limite de mensajes mayor a %d.
MEMO_SET_LIMIT_TOO_HIGH
	Usted no puede ajustar el limite de mensajes para %s mas alto que %d.
MEMO_SET_LIMIT_OVERFLOW
	Limite de mensajes muy alto; limitando a %d.
MEMO_SET_YOUR_LIMIT
	Su limite de mensaje ha sido ajustado a %d.
MEMO_SET_YOUR_LIMIT_ZERO
	Usted no podrá recibir mensajes.
MEMO_SET_LIMIT
	Limite de mensajes para %s ajustado a %d.
MEMO_SET_LIMIT_ZERO
	Limite de mensajes para %s ajustado a 0.
MEMO_UNSET_YOUR_LIMIT
	Su limite de mensajes ha sido deshabilitado.
MEMO_UNSET_LIMIT
	Limite de mensajes desactivado para %s.

# INFO responses
MEMO_INFO_NO_MEMOS
	Usted no tiene mensajes.
MEMO_INFO_MEMO
	Usted tiene 1 mensaje.
MEMO_INFO_MEMO_UNREAD
	Usted tiene 1 mensaje, y no ha sido leído.
MEMO_INFO_MEMOS
	Usted tiene %d mensajes.
MEMO_INFO_MEMOS_ONE_UNREAD
	Usted tiene %d mensajes, de los cuales 1 no ha sido leído.
MEMO_INFO_MEMOS_SOME_UNREAD
	Usted tiene %d mensajes, de los cuales %d no han sido leídos.
MEMO_INFO_MEMOS_ALL_UNREAD
	Usted tiene %d mensajes; de los cuales ninguno ha sido leído.
MEMO_INFO_LIMIT
	Su limite de mensajes es %d.
MEMO_INFO_HARD_LIMIT
	Su limite de mensajes es %d, y no puede ser cambiado.
MEMO_INFO_LIMIT_ZERO
	Su limite de mensajes es 0; usted no recibirá ningún nuevo mensaje.
MEMO_INFO_HARD_LIMIT_ZERO
	Su limite de mensajes es 0; usted no recibirá ningún nuevo mensaje. Usted no puede cambiar este limite.
MEMO_INFO_NO_LIMIT
	Usted no tiene limite en la cantidad de mensajes que puede guardar.
MEMO_INFO_X_NO_MEMOS
	%s no tiene mensajes.
MEMO_INFO_X_MEMO
	%s tiene actualmente 1 mensaje.
MEMO_INFO_X_MEMO_UNREAD
	%s tiene actualmente 1 mensaje, y no ha sido leído aun.
MEMO_INFO_X_MEMOS
	%s tiene actualmente %d mensajes.
MEMO_INFO_X_MEMOS_ONE_UNREAD
	%s tiene actualmente %d mensajes, de los cuales 1 esta sin leer.
MEMO_INFO_X_MEMOS_SOME_UNREAD
	%s tiene actualmente %d mensajes, de los cuales %d están sin leer.
MEMO_INFO_X_MEMOS_ALL_UNREAD
	%s tiene actualmente %d mensajes; de los cuales todos están sin leer.
MEMO_INFO_X_LIMIT
	El limite de mensajes de %s es %d.
MEMO_INFO_X_HARD_LIMIT
	El limite de mensajes de %s es %d, y no puede ser cambiado por el usuario.
MEMO_INFO_X_NO_LIMIT
	%s no tiene limite de mensajes.

###########################################################################
#
# Mensajes de OperServ
#
###########################################################################

# General messages
OPER_BOUNCY_MODES
	Services no puede cambiar modos.
	Sus servidoras están configuradas correctamente?
OPER_BOUNCY_MODES_U_LINE
	Services no puede cambiar modos.
	La líneas U: de sus servidoras están configuradas correctamente?

# GLOBAL responses

# STATS responses
OPER_STATS_UNKNOWN_OPTION
	Opción STATS %s desconocida.
OPER_STATS_CURRENT_USERS
	Usuarios actuales : %d (%d ops)
OPER_STATS_MAX_USERS
	Máximo de usuarios: %d (%s)
# This is ugly, but at least it's language-portable...
OPER_STATS_UPTIME_DHMS
	Services activos %d días, %02d:%02d
OPER_STATS_UPTIME_1DHMS
	Services activos %d día, %02d:%02d
OPER_STATS_UPTIME_HMS
	Services activos %d horas, %d minutos
OPER_STATS_UPTIME_HM1S
	Services activos %d horas, %d minutos
OPER_STATS_UPTIME_H1MS
	Services activos %d horas, %d minuto
OPER_STATS_UPTIME_H1M1S
	Services activos %d horas, %d minuto
OPER_STATS_UPTIME_1HMS
	Services activos %d hora, %d minutos
OPER_STATS_UPTIME_1HM1S
	Services activos %d hora, %d minutos
OPER_STATS_UPTIME_1H1MS
	Services activos %d hora, %d minuto
OPER_STATS_UPTIME_1H1M1S
	Services activos %d hora, %d minuto
OPER_STATS_UPTIME_MS
	Services activos %d minutos, %d segundos
OPER_STATS_UPTIME_M1S
	Services activos %d minutos, %d segundos
OPER_STATS_UPTIME_1MS
	Services activos %d minuto, %d segundos
OPER_STATS_UPTIME_1M1S
	Services activos %d minuto, %d segundo
OPER_STATS_USER_MEM
	User    : %6d registros, %5d kB
OPER_STATS_CHANNEL_MEM
	Channel : %6d registros, %5d kB
OPER_STATS_NICKSERV_MEM
	NickServ: %6d registros, %5d kB
OPER_STATS_CHANSERV_MEM
	ChanServ: %6d registros, %5d kB
OPER_STATS_OPERSERV_MEM
	OperServ: %6d registros, %5d kB
OPER_STATS_AKILL_COUNT
	Numero actual de AKILLs: %d
OPER_STATS_AKILL_EXPIRE_DAYS
	Tiempo default para expirar AKILL: %d días
OPER_STATS_AKILL_EXPIRE_DAY
	Tiempo default para expirar AKILL: 1 día
OPER_STATS_AKILL_EXPIRE_HOURS
	Tiempo default para expirar AKILL: %d horas
OPER_STATS_AKILL_EXPIRE_HOUR
	Tiempo default para expirar AKILL: 1 hora
OPER_STATS_AKILL_EXPIRE_MINS
	Tiempo default para expirar AKILL: %d minutos
OPER_STATS_AKILL_EXPIRE_MIN
	Tiempo default para expirar AKILL: 1 minuto
OPER_STATS_AKILL_EXPIRE_NONE
	Tiempo default para expirar AKILL: No expira

# MODE responses

# CLEARMODES responses
OPER_CLEARMODES_DONE
	Modos y bans del canal %s fueron eliminados.
OPER_CLEARMODES_ALL_DONE
	Todos los modos del canal %s fueron eliminados.

# KICK responses

# ADMIN responses
OPER_ADMIN_EXISTS
	%s ya existe en la lista de Administradores de Services.
OPER_ADMIN_ADDED
	%s agregado a la lista de Administradores de Services.
OPER_ADMIN_TOO_MANY
	Muchas entradas (%d) en la lista de Administradores de Services; no se puede agregar mas.
OPER_ADMIN_REMOVED
	%s borrado de la lista de Administradores de Services.
OPER_ADMIN_NOT_FOUND
	%s no fue encontrado en la lista de Administradores de Services.
OPER_ADMIN_LIST_HEADER
	Lista de Administradores de Services:

# OPER responses
OPER_OPER_EXISTS
	%s ya existe en la lista de Operadores de Services.
OPER_OPER_ADDED
	%s agregado a la lista de Operadores de Services.
OPER_OPER_TOO_MANY
	Muchas entradas (%d) en la lista de Operadores de Services; no se pueden agregar mas.
OPER_OPER_REMOVED
	%s borrado de la lista de Operadores de Services.
OPER_OPER_NOT_FOUND
	%s no fue encontrado en la lista de Operadores de Services.
OPER_OPER_LIST_HEADER
	Lista de Operadores de Services:

# AKILL responses
OPER_AKILL_NO_NICK
	Atención: Mascaras de AKILL no pueden contener nicks;
	Tenga la certeza de no incluir una parte de nick en su mascara.

OPER_AKILL_REMOVED
	%s eliminado de la lista de AKILL.
OPER_AKILL_NOT_FOUND
	%s no fue encontrado en la lista de AKILL.
OPER_AKILL_LIST_HEADER
	Lista actual de AKILL:
# mask, reason
OPER_AKILL_LIST_FORMAT
	%-32s  %s
# mask, set-by, set-time, expires, reason
OPER_AKILL_VIEW_FORMAT
	%s (por %s en %s; %s)
	    %s
OPER_AKILL_NO_EXPIRE
	no expira
OPER_AKILL_EXPIRES_SOON
	expira en la siguiente actualización de base de datos
OPER_AKILL_EXPIRES_M
	expira en %d minutos
OPER_AKILL_EXPIRES_1M
	expira en %d minuto
OPER_AKILL_EXPIRES_HM
	expira en %d horas, %d minutos
OPER_AKILL_EXPIRES_H1M
	expira en %d horas, %d minuto
OPER_AKILL_EXPIRES_1HM
	expira en %d hora, %d minutos
OPER_AKILL_EXPIRES_1H1M
	expira en %d hora, %d minuto
OPER_AKILL_EXPIRES_D
	expira en %d días
OPER_AKILL_EXPIRES_1D
	expira en %d día

# SET responses
OPER_SET_IGNORE_ON
	Ignore code sera usado.
OPER_SET_IGNORE_OFF
	Ignore code no sera usado.
OPER_SET_IGNORE_ERROR
	Parámetro para IGNORE debe ser ON o OFF.
OPER_SET_DEBUG_ON
	Services esta ahora en modo debug.
OPER_SET_DEBUG_OFF
	Services esta ahora en modo non-debug.
OPER_SET_DEBUG_LEVEL
	Services esta ahora en modo debug (nivel %d).
OPER_SET_DEBUG_ERROR
	Parámetro para DEBUG debe ser ON, OFF, o un numero positivo.
OPER_SET_UNKNOWN_OPTION
	Opción %s desconocida.

# JUPE responses

# RAW responses

# RESTART responses
OPER_CANNOT_RESTART
	SERVICES_BIN no definida; no es posible reiniciar.  Ejecute nuevamente el script \2configure\2 y recompile Services para habilitar el comando RESTART.

# LISTIGNORE responses
OPER_IGNORE_LIST
	Lista de ignoración de Services:
OPER_IGNORE_LIST_EMPTY
	Lista de ignoración esta vacía.

###########################################################################
#
# Mensajes de ayuda de NickServ
#
###########################################################################

NICK_HELP
	%S le permite "registrar" un nick y previene que otros
	hagan uso de el.  Si el nick no es utilizado por
	%d días, el registro expirará.  Los siguientes comandos
	permiten el registro y mantenimiento de
	nicks; para utilizarlos, escriba /msg %S comando.
	Para mayor información de un comando specifico, escriba
	/msg %S HELP comando.
	
	    REGISTER   Registra un nick
	    IDENTIFY   Identificarse con su contraseña
	    ACCESS     Modificar la lista de direcciones autorizadas
	    LINK       Hacer su nick un alias de otro
	    SET        Ajustar opciones, incluyendo protección de kill
	    DROP       Cancela el registro de un nick
	    RECOVER    Desconecta a otro usuario que esta usando tu
	                   nick
	    RELEASE    Toma custodia de su nick después de utilizar el
	                   comando RECOVER
	
	Otros comandos: UNLINK, GHOST, INFO, LIST, STATUS, AUTOJOIN
	
	NOTA: Este servicio esta diseñado para proveer un modo en
	que los usuarios de IRC aseguran que su identidad no se
	comprometida.  Su intención NO es para facilitar el "robo"
	de nicks o otras acciones mal intencionadas.  Abuso de
	%S resultara, por lo menos, de la perdida de el/los
	nick(s) abusivo(s).

NICK_HELP_REGISTER
	Sintaxis: REGISTER contraseña
	
	Registra su nick la base de datos de %S.  Una vez registrado
	su nick, usted pude utilizar los comandos SET y ACCESS para
	configurar los parámetros de su nick como usted los desee.
	Asegúrese de recordar la contraseña que utilizo para
	registrarse--la necesitara para hacer cambios a su nick
	posteriormente.  (Nótese que las mayúsculas importan
	FIDO, Fido, y fido son diferentes todas!)
	
	Sugerencias para escoger una contraseña:
	
	Contraseñas no deben ser fácil de adivinar.  Por ejemplo,
	usar su verdadero nombre como contraseña es una mala idea.
	Utilizar su nick como contraseña es una idea peor ;) y, por
	supuesto que, %S no lo permitirá.  También, contraseñas
	cortas son vulnerables a búsquedas de prueba y error, por lo
	que usted debe escoger una contraseña de por lo menos 5
	caracteres.  Finalmente, el carácter de "espacio" no se
	puede utilizar en contraseñas.

NICK_HELP_IDENTIFY
	Sintaxis: IDENTIFY contraseña
	
	Notifica a %S que usted es el verdadero dueño de este
	nick.  Muchos de los comandos requieren que usted se
	identifique con este comando antes de poder utilizarlos.  La
	contraseña debe ser la misma que utilizo con el comando
	REGISTER.

NICK_HELP_DROP
	Sintaxis: DROP
	
	Elimina su nick de la base de datos de %S.  Un nick
	que ha sido eliminado esta disponible para que cualquier
	persona lo vuelva a registrar.
	
	Para utilizar este comando , usted tiene que identificarse
	con su  contraseña (/msg %S HELP IDENTIFY para mayor
	información).

NICK_HELP_ACCESS
	Sintaxis: ACCESS ADD mascara
	          ACCESS DEL mascara
	          ACCESS LIST
	
	Modifica o muestra la lista de acceso para su nick. Esta
	es la lista de direcciones que %S reconocerá automáticamente
	como validas para utilizar este nick. Si desea utilizar este
	nick desde otra dirección, necesita enviar el comando
	IDENTIFY para que %S lo reconozca a usted.
	
	Ejemplos:
	
	    ACCESS ADD achurch@*.dragonfire.net
	        Permite el acceso al usuario achurch de cualquier
		computadora en el dominio dragonfire.net.
	
	    ACCESS DEL achurch@*.dragonfire.net
	        Hace lo contrario de el comando anterior.
	
	    ACCESS LIST
	        Muestra la lista de acceso.

NICK_HELP_LINK
	Sintaxis: LINK nick contraseña
	
	Liga su nick a otro, haciendo efectivamente un alias para el
	nick que usted utiliza actualmente con otro nick
	especificado.  Cuando usted utiliza este comando, la lista
	de acceso de el nick que tiene es eliminada y reemplazada
	por la del nick con la que usted hace la liga; todos los
	mensajes para el nick actual son agregados a la lista de
	mensajes de el nick a ligar (esto puede causar que el nick
	se exceda en su limite de mensajes, en este caso necesitara
	borrar algunos de los mensajes antes de poder recibir nuevos
	mensajes para cualquiera de ambos nicks).
	
	Unas vez establecida la liga, su nick sera convertido
	transparentemente a el nick nuevo en todas las bases de
	datos de Services, excepto para los comandos %S UNLINK y
	DROP.  Por ejemplo, puede utilizar cualquiera de los
	nicks para leer los mismos mensajes (y los mensajes que se
	envían a cualquier de los nicks irán a la misma lista).  Si
	se identifica para un nick, automáticamente se identificara
	para el otro. Si cualquiera de ambos nicks esta en la lista
	de acceso de un canal, ambos nicks tendrán los mismos
	privilegios y accesos.
	
	Para utilizar este comando, usted debe de identificarse para
	su nick actual (usando el comando IDENTIFY), y debe
	proporcionar la contraseña para el nick al que desea ligar.

NICK_HELP_UNLINK
	Sintaxis: UNLINK
	
	Elimina la liga de su nick hacia otro. Cuando la liga se
	elimina, los modos de un nick y la lista de acceso de el
	nick previamente ligado son copiados a su nick actual y
	pueden ser modificados independientemente.  Mensajes no son
	copiados; todos los mensajes permanecen con el nick
	previamente ligado.

NICK_HELP_SET
	Sintaxis: SET opción parámetros
	
	Ajusta varios parámetros del nick. Una opción puede ser:
	
	    PASSWORD   Ajusta la contraseña de su nick
	    LANGUAGE   Ajusta el lenguaje que Services utilizara cuando
	                   envié mensajes a usted
	    URL        Asocia un URL con su nick
	    EMAIL      Asocia una dirección de E-mail con su nick
	    KILL       Activa/desactiva la protección de kill
	    SECURE     Activa/desactiva el modo SECURE para su nick
	    PRIVATE    Evita que su nick aparezca en
	                   /msg %S LIST
	    HIDE       Esconde ciertas informaciones sobre su nick
	
	Para utilizar este comando es necesario identificarse antes
	con su contraseña (/msg %S HELP IDENTIFY para mayor
	información).
	
	Escriba /msg %S HELP SET opción para mayor información
	sobre una opción especifica.

NICK_HELP_SET_PASSWORD
	Sintaxis: SET PASSWORD nueva-contraseña
	
	Cambia la contraseña que utiliza para identificarse como el
	dueño de el nick.

NICK_HELP_SET_LANGUAGE
	Sintaxis: SET LANGUAGE numero
	
	Cambia el lenguaje que Services utiliza cuando envía
	mensajes a usted (por ejemplo cuando responde a un comando
	que usted envió).  numero debe ser escogido de la siguiente
	lista de lenguajes soportados:
	

NICK_HELP_SET_URL
	Sintaxis: SET URL url
	
	Asocia un URL dado con su nick.  Este URL sera mostrado
	cuando solicite información de su nick con el comando INFO.

NICK_HELP_SET_EMAIL
	Sintaxis: SET EMAIL dirección
	
	Asocia la dirección de E-mail dada con su nick.  Esta
	dirección sera mostrada cuando alguien solicite información
	sobre su nick con el comando INFO.

NICK_HELP_SET_SECURE
	Sintaxis: SET SECURE {ON | OFF}
	
	Activa/desactiva las funciones de seguridad de %S para
	su nick.  Con SECURE activado, usted debe de enviar su
	contraseña antes de que se le reconozca como el dueño de el
	nick, sin importar si su dirección este o no en la lista de
	acceso.  Sin embargo, si aparece en la lista de acceso, %S
	no lo desconectara automáticamente, independientemente de
	como este la opción de KILL.

NICK_HELP_SET_PRIVATE
	Sintaxis: SET PRIVATE {ON | OFF}
	
	Activa/desactiva la opción privacidad de %S sobre su
	nick.  Con PRIVATE activado, su nick no aparecerá en las
	listas de nicks generadas con el comando %S LIST.
	(Sin embargo, cualquiera que conozca su nick puede obtener
	información usando el comando INFO.)

NICK_HELP_SET_HIDE
	Sintaxis: SET HIDE {EMAIL | USERMASK | QUIT} {ON | OFF}
	
	Permite que usted evite que ciertas informaciones sean
	mostradas cuando alguien usa el comando %S INFO en su
	nick.  Usted puede esconder su E-mail (EMAIL), ultima
	mascara user@host (USERMASK), o su ultimo mensaje de salida
	(QUIT).  El segundo parámetro specifica cual información
	debe ser mostrada (OFF) o escondida (ON).

NICK_HELP_RECOVER
	Sintaxis: RECOVER nick [contraseña]
	
	Permite que usted recupere su nick si alguien mas lo ha
	tomado; esto es lo mismo que %S hace automáticamente si
	alguien intenta utilizar un nick protegido con kill.
	
	Cuando usted envía este comando, %S conectara en línea a un
	usuario falso con el mismo nick que el usuario que usted
	desea recuperar.  Esto causara que los servidores de IRC
	desconecten al otro usuario.  El usuario falso permanecerá
	en línea por un minuto para asegurar que el otro usuario no
	se reconecte inmediatamente; después de ese minuto, usted
	puede tomar su nick.  Alternativamente, use el comando
	RELEASE (/msg %S HELP RELEASE) para obtener su nick
	mas rápido.
	
	Para usar el comando RECOVER para un nick, su dirección
	actual como se muestra en un /WHOIS debe de estar en la
	lista de acceso de el nick, o usted debe de proporcionar la
	contraseña correcta para el nick.

NICK_HELP_RELEASE
	Sintaxis: RELEASE nick [contraseña]
	
	Ordena a %S eliminar cualquier espera en su nick
	causada por un kill automático de protección o por el uso de
	el comando RECOVER.  Por default, la espera dura un minuto;
	este comando lo libera mas rápidamente.
	
	Para usar el comando RELEASE para un nick, su dirección
	actual como se muestra en un /WHOIS debe de estar en la
	lista de acceso de el nick, o usted debe de proporcionar la
	contraseña correcta para el nick.

NICK_HELP_GHOST
	Sintaxis: GHOST nick [contraseña]
	
	Elimina una sesión "fantasma" de IRC que este utilizando su
	nick.  Una sesión "fantasma" es aquella que no esta
	realmente conectada, pero que un servidor de IRC acredita
	como activa por cualquier razón.  Típicamente esto sucede si
	su computadora se atora o su conexión a Internet se
	desconecta por un periodo corto mientras esta en IRC.
	
	Para usar el comando GHOST para un nick, su dirección
	actual como se muestra en un /WHOIS debe de estar en la
	lista de acceso de el nick, o usted debe de proporcionar la
	contraseña correcta para el nick.

NICK_HELP_INFO
	Sintaxis: INFO nick
	
	Muestra información sobre un nick dado, como el quien es
	el dueño, ultima dirección utilizada y hora, y las
	diferentes opciones del nick.

NICK_HELP_LIST
	Sintaxis: LIST patrón
	
	Lista todos los nicks registrados que concuerdan con un
	patrón dado en formato nick!user@host.  Nicks con la opción
	de PRIVATE activada no serán mostrados.
	
	Ejemplos:
	
	    LIST *!joeuser@foo.com
	         Lista los nicks registrados por joeuser@foo.com.
	
	    LIST *Bot*!*@*
	        Lista todos los nicks registrados que contenga Bot
		en su nombre (no importa mayúsculas).
	
	    LIST *!*@*.bar.org
	        Lista todos los nicks registrados por usuarios del dominio bar.org.

NICK_HELP_STATUS
	Sintaxis: STATUS nick...
	
	Informa si el usuario de un nick es reconocido como el dueño
	de el mismo.  La respuesta tiene el siguiente formato:
	
	    nick código-de-status
	
	donde nick es el nick enviado con el comando,
	código-de-status es uno de los siguientes:
	
	    0 - usuario no esta conectado o nick no esta registrado
	    1 - usuario no reconocido como dueño del nick
	    2 - usuario reconocido por la lista de acceso solamente
	    3 - usuario reconocido por identificación con contraseña
	
	Hasta 16 nicks pueden ser enviados con cada comando; el
	resto serán ignorados. No se genera mensaje de error si no
	se envía nick.

NICK_SERVADMIN_HELP
	
	Los siguientes comandos están disponibles para los
	Administradores de Services:
	
	    GETPASS    Muestra la contraseña para un nick
	                   (solo si encripción esta desactivada)
	    FORBID     Evita que un nick sea utilizado
	
	Administradores de Services también cancelar cualquier nick
	sin necesidad de identificarse por el mismo, y pueden ver la
	lista de acceso de cualquier nick (/msg %S ACCESS LIST nick).

NICK_SERVADMIN_HELP_DROP
	Sintaxis: DROP [nick]
	
	Sin parámetros, cancela el registro de tu nick de la base de
	datos de %S.
	
	Con un parámetro, cancela el registro de ese nick en la base
	de datos.  Este comando esta restringido a Administradores
	de Services.

NICK_SERVADMIN_HELP_SET
	Administradores de Services pueden seleccionar la opción
	NOEXPIRE, con la cual se puede prevenir que algunos nicks no
	expiren.  Adicionalmente, los Administradores de Services
	pueden establecer opciones para cualquier nick sin necesidad
	de proveer una contraseña, utilizando el formato
	SET nick opción parámetros.

NICK_SERVADMIN_HELP_SET_NOEXPIRE
	Sintaxis: SET [nick] NOEXPIRE {ON | OFF}
	
	Define si el nick dado expirara.  Si se define ON previene
	que el nick expire.  Si no se incluye un nick, entonces se
	aplica a tu propio nick.
	
	Limitado a Administradores de Services.

NICK_SERVADMIN_HELP_FORBID
	Sintaxis: FORBID nick
	
	Prohibe el registro y la utilización de un nick. Se puede
	cancelar utilizando el comando DROP.
	
	Limitado a Administradores de Services.

###########################################################################
#
# Mensajes de Ayuda de ChanServ
#
###########################################################################

CHAN_HELP
	%S habilita a registrar y controlar varios aspectos de
	los canales.  %S puede prevenir que usuarios
	malintencionados puedan "tomar" el control limitando a quién
	se permite privilegios de operador del canal.  Los comandos
	disponibles son los siguientes.  Para utilizarlos, escriba
	/msg %S comando.  Para obtener mas información de un comando
	específico, escriba /msg %S HELP comando.
	
	    REGISTER   Registra un canal
	    IDENTIFY   Para identificarse como fundador del canal
	    SET        Fija opciones e información del canal
	    ACCESS     Modifica la lista de usuarios privilegiados
	    LEVELS     Redefine los niveles de accesos
	    AKICK      Mantiene la lista de Auto-Kick
	    DROP       Cancela la registración de un canal
	
	Otros comandos: INFO, LIST, INVITE, OP, DEOP, UNBAN,
	                CLEAR

CHAN_HELP_EXPIRES
	
	Note que si un canal no es utilizado por %d días (es decir
	qué ningún usuario en la lista de acceso del canal entra en
	ese periodo de tiempo) será automáticamente cancelado.

CHAN_HELP_REGISTER
	Sintaxis: REGISTER canal contraseña descripción
	
	Registra un nuevo canal en la base de datos de %S.
	Para poder utilizar este comando, usted primero debe ser
	operador del canal que está tratando de registrar (1).  La
	contraseña deberá ser utilizado con el comando IDENTIFY
	cuando necesite hacer modificaciones posteriores a opciones
	del canal.  El último parámetro, que debe ser incluido,
	es una descripción de los propósitos del canal.
	
	Cuando usted registra un canal, usted queda registrado como
	el "fundador" del canal. El fundador de un canal puede
	modificar cualquiera de las opciones de un canal (excepto el
	nombre mismo del canal); %S le dará automáticamente
	privilegio de operador al fundador del canal cuando el/ella
	ingresen al canal.  Vea el comando ACCESS
	(/msg %S HELP ACCESS) para obtener información de como
	dar privilegio de operadores a otros usuarios del canal.
	
	(1) AVISO: para poder registrar un canal, su nick debe estar
	registrado.  Si aún no lo registró, escriba
	/msg %s HELP para obtener información de cómo hacerlo.

CHAN_HELP_IDENTIFY
	Sintaxis: IDENTIFY canal contraseña
	
	Este comando, lo identifica a usted ante %S como el
	fundador del canal.  Muchos comandos requieren que usted
	utilice ESTE comando antes de poder utilizarlos.  La
	contraseña debe ser el mismo que se ingresó al momento de
	registrar el canal (comando REGISTER).

CHAN_HELP_DROP
	Sintaxis: DROP canal
	
	Elimina el canal de la base de %S.  Puede ser
	utilizado únicamente por el fundador del canal, quien deberá
	primeramente identificarse con el comando IDENTIFY.

CHAN_HELP_SET
	Sintaxis: SET canal opción parámetros
	
	Habilita al fundador del canal a definir varias opciones y
	otra información.  El fundador, debe identificarse vía
	comando IDENTIFY antes de utilizar SET.
	
	Opciones disponibles:
	
	    FOUNDER     Cambia el fundador del canal
	    PASSWORD    Cambia la contraseña del fundador
	    DESC        Cambia la descripción del canal
	    URL         Asocia una URL al canal
	    EMAIL       Asocia una dirección de correo al canal
	    ENTRYMSG    Fija un mensaje que será mostrado a los usuarios
			cuando entren al canal
	    TOPIC       Cambia el Tema del canal
	    KEEPTOPIC   Retiene el tema (topic) cuando el canal no está en uso
	    TOPICLOCK   El Tema SOLO podrá ser cambiado vía SET TOPIC
	    MLOCK       Fija los modos del canal en 'ON' u 'OFF'
	    PRIVATE     Oculta el canal (no se ve cuando se utiliza el comando LIST)
	    RESTRICTED  Restringe el acceso al canal
	    SECURE      Activa los rasgos de seguridad de %S
	    SECUREOPS   Control estricto de Status de operador
	
	Escriba /msg %S HELP opción para obtener mas
	información de una opción en particular.

CHAN_HELP_SET_FOUNDER
	Sintaxis: SET canal FOUNDER nick
	
	Cambia el fundador del canal.  El nick dado, que será el
	nuevo fundador del canal, debe estar registrado.

CHAN_HELP_SET_PASSWORD
	Sintaxis: SET canal PASSWORD contraseña
	
	Se utiliza para definir la contraseña que será utilizado
	para identificarse como el fundador del canal.

CHAN_HELP_SET_DESC
	Sintaxis: SET canal DESC descripción
	
	Fija la descripción del canal, la cual será mostrada cuando
	se utilicen los comandos LIST e INFO.  Sea breve y concreto.

CHAN_HELP_SET_URL
	Sintaxis: SET canal URL [url]
	
	Asocia una URL al canal, la cual será visualizada cuando
	se solicite información del canal (vía comando INFO).
	Habitualmente se utiliza para indicar la dirección de la
	página web del canal.  Si no se ingresa ninguna url, se
	borrará la que tenga actualmente el canal.

CHAN_HELP_SET_EMAIL
	Sintaxis: SET canal EMAIL [dirección]
	
	Asocia la dirección de correo dada al canal.  Esta dirección
	será mostrada cuando alguien solicite (vía comando INFO)
	información del canal.  Es muy útil, por ejemplo, para que
	los usuarios tengan una vía de comunicación con el fundador
	y/u operadores del canal.  Si no se proporciona una
	dirección de correo, se eliminará la que actualmente pueda
	tener el canal.

CHAN_HELP_SET_ENTRYMSG
	Sintaxis: SET canal ENTRYMSG [mensaje]
	
	Establece el mensaje que será enviado a los usuarios (vía
	/notice) cuando ellos ingresen al canal.  Si mensaje no se
	ingresa, nada será enviado a los usuarios cuando ingresen.

CHAN_HELP_SET_TOPIC
	Sintaxis: SET canal TOPIC texto
	
	Causa que %S fije el tema del canal al indicado en
	texto.  Esta orden es muy útil en conjunción con
	SET TOPICLOCK.

CHAN_HELP_SET_KEEPTOPIC
	Sintaxis: SET canal KEEPTOPIC {ON | OFF}
	
	Habilita o deshabilita la opción de retención del tema para
	el canal.  Cuando retención del tema está en ON
	(habilitada), el tema del canal será recordado por %S
	aún después que el último usuario se haya retirado, y se
	restaurará la próxima vez que alguien ingrese al canal.

CHAN_HELP_SET_TOPICLOCK
	Sintaxis: SET canal TOPICLOCK {ON | OFF}
	
	Habilita o deshabilita la opción para trabar el tópico del
	canal.  Cuando trabar el tópico está en ON, %S no
	habilitará el cambio del tema (tópico) excepto vía SET
	TOPIC.

CHAN_HELP_SET_MLOCK
	Sintaxis: SET canal MLOCK modos
	
	Fija los distintos modos para el canal.  %S lo habilita a
	usted para definir determinados modos del canal y que queden
	siempre habilitados (o no).  Los modos que pueden ser
	fijados son: i, k, l, m, n, p, s, y t; cada
	uno de estos modos puede estar activado (ON), desactivado
	(OFF) o no fijado.  Los distintos modos se indican
	exactamente como si se utilizase el comando /MODE; esto es,
	modo seguido de + indica activado (ON), modo seguido de -
	indica desactivado (OFF).
	
	Cuidado!:  Si usted fija un modo protegido por contraseña,
	como en el segundo ejemplo, también deberá fijar la opción
	RESTRICTED para el canal (vea HELP SET RESTRICTED), o
	cualquiera que ingrese al canal cuando el mismo esté vacío,
	verá la contraseña!!
	
	Ejemplos:
	
	    SET #canal MLOCK +nt-iklps
	       Activa los modos n y t, y desactiva los modos i, k,
	       l, p, y s.  El modo m puede ser activado o desactivado.
	
	    SET #canal MLOCK +knst-ilmp my-key
	       Activa los modos k, n, s, y t.
	       Desactiva los modos i, l, m, y p.
	       También establece "my-key" como la contraseña del canal.
	
	    SET #canal MLOCK +
	       Elimina los modos activos; todos los modos del canal
	       pueden ser libremente activados o desactivados.

CHAN_HELP_SET_PRIVATE
	Sintaxis: SET canal PRIVATE {ON | OFF}
	
	Habilita o deshabilita la opción de privado para el canal.
	Cuando esta opción está activada, el comando /msg %S LIST no
	incluirá el canal en ninguna lista.

CHAN_HELP_SET_RESTRICTED
	Sintaxis: SET canal RESTRICTED {ON | OFF}
	
	Habilita o deshabilita la opción de acceso restringido para
	el canal.  Cuando el acceso restringido está activo (ON),
	usuarios que normalmente no tienen privilegio de operadores
	cuando entran al canal (usuarios con nivel de acceso
	negativo y, si secure ops está activada, usuarios que NO
	están en la lista de acceso) serán expulsados (y se les
	prohibirá el acceso) al canal.

CHAN_HELP_SET_SECURE
	Sintaxis: SET canal SECURE {ON | OFF}
	
	Habilita o deshabilita la seguridad de %S para el
	canal.  Cuando SECURE está ACTIVO, SOLO los usuarios que
	tienen registrado sus nicks con %s y se han
	IDENTIFICADO con sus respectivas contraseñas tendrán acceso
	al canal si es que figuran en la lista de ACCESS.

CHAN_HELP_SET_SECUREOPS
	Sintaxis: SET canal SECUREOPS {ON | OFF}
	
	Habilita o deshabilita la opción de operadores seguros para
	el canal.  Cuando operadores seguros está activada (ON),
	usuarios que NO estén en la lista de usuarios del canal NO
	podrán obtener status de operadores

#CHAN_HELP_ACCESS
#	Sintaxis: ACCESS canal ADD nick nivel
#	          ACCESS canal DEL {nick | posición-numero}
#	          ACCESS canal LIST [mask]
#	
#	Mantiene la lista de accesos del canal.  La lista de accesos
#	establece que usuarios están habilitados a ser operadores
#	del canal o acceder a comandos de %S en el canal.  Se pueden
#	establecer diferentes niveles de usuarios para acceder a
#	diferentes privilegios; utilice /msg %S HELP ACCESS LEVELS
#	para obtener información mas específica.  Cualquier nick que
#	no esté en la lista de acceso tiene nivel de usuario 0
#	(cero).
#	
#	El comando ACCESS ADD agrega al nick con el nivel dado a la
#	lista de acceso; si el usuario ya existe en la lista, se
#	reemplazará su nivel.  El nivel especificado debe ser menor
#	que el que tiene el usuario que está ingresando el comando,
#	y si el nick está actualmente en la lista, el nivel actual
#	de dicho nick debe ser menor que el nivel del usuario que
#	está ingresando el comando.
#	
#	El comando ACCESS DEL elimina un usuario de la lista de
#	accesos.  Se puede eliminar un usuario ingresando el nick o
#	ingresando el número que ocupa en la lista de accesos.
#	
#	El comando ACCESS LIST muestra la lista de accesos al canal;
#	se pueden utilizar caracteres comodines o mascaras, en cuyo
#	caso solo las coincidencias concurrentes serán listadas.

CHAN_HELP_ACCESS_LEVELS
	Niveles de acceso para usuarios
	
	Por defecto, los siguientes niveles son definidos:
	
	   Founder   Acceso FULL a las funciones de %S.
	                 Privilegio de operador al ingresar al canal.
	                 Note que únicamente una sola persona puede
	                 tener acceso de fundador del canal (no se
	                 puede dar acceso de founder con el comando
	                 ACCESS).
	        10   Acceso al comando AKICK; operador automático al
	                 ingresar.
	         5   Operador automático al ingresar.
	         3   Estado de VOZ automático (+v).
	         0   No tiene privilegios especiales; puede obtener
	                 status de operador dado por otro operador
	                 (a menos que secure-ops esté activado).
	        <0   No puede ser operador.
	
	Estos niveles pueden ser cambiados, o nuevos pueden ser
	agregados utilizando el comando LEVELS; escriba
	/msg %S HELP LEVELS para información.

CHAN_HELP_AKICK
	Sintaxis: AKICK canal ADD mask [razón]
	          AKICK canal DEL mask
	          AKICK canal LIST [mask]
	
	Definimos AKICK como "Expulsión automática".
	Definimos BAN como "Entrada prohibida".
	
	Este comando, mantiene la lista de AKICK del canal.  Si un
	usuario está en la lista de AKICK e intenta entrar al canal,
	%S automáticamente agrega dicho usuario a la lista BAN
	del canal, y luego lo expulsa del canal.
	
	El comando AKICK ADD agrega un usuario a la lista de AKICK.
	Si una razón es dada con el comando, entonces la razón será
	utilizada por %S cuando expulse a dicho usuario; si
	no, el mensaje que se utilizará por defecto, es "You have
	been banned from the channel".
	
	El comando AKICK DEL elimina al usuario de la lista de
	AKICK.  Este comando no remueve el BAN que pueda existir en
	el canal, el cual deberá ser removido manualmente.
	
	El comando AKICK LIST muestra la lista de usuarios en la
	lista de AKICK.  Opcionalmente, solo lista aquellos
	suministrados con la mascara dada.

CHAN_HELP_LEVELS
	Sintaxis: LEVELS canal SET tipo nivel
	          LEVELS canal {DIS | DISABLE} tipo
	          LEVELS canal LIST
	          LEVELS canal RESET
	
	El comando LEVELS permite un control fino sobre los accesos
	numéricos utilizados por los canales.  Con este comando,
	usted puede definir los niveles requeridos para muchas de
	las funciones de %S.  Los comandos SET FOUNDER,
	SET PASSWORD, así como éste comando están siempre
	restringidos al fundador.
	
	LEVELS SET habilita el acceso para una función o grupo de
	funciones a ser modificadas.
	LEVELS DISABLE (o DIS en forma abreviada) deshabilita un
	privilegio automático, o deshabilita el acceso para
	cualquier otro que no sea el fundador del canal.
	LEVELS LIST muestra los niveles actuales para cada función o
	grupo de funciones.
	LEVELS RESET vuelve todos los niveles a sus valores por
	defecto, los mismos que cuando se registra por primera vez
	un canal (vea HELP ACCESS LEVELS).
	
	Para una lista de las funciones que pueden ser establecidas,
	utilice el comando HELP LEVELS DESC.

CHAN_HELP_LEVELS_DESC
	La siguiente lista muestra los valores/funciones por
	defecto.  Note que los niveles para AUTODEOP y NOJOIN son
	los máximos niveles, mientras que todos los demás, son los
	mínimos.	

CHAN_HELP_LEVELS_DESC_FORMAT
	    %-*s  %s

CHAN_HELP_INFO
	Sintaxis: INFO canal
	
	Provee información acerca del canal (o informa que no está
	registrado).  Lista el fundador, hora y fecha de
	registración, última vez que se utilizó, descripción, y
	modos del canal (si los hubiese).

CHAN_HELP_LIST
	Sintaxis: LIST especificación
	
	Lista todos los canales que coincidan con la especificación
	dada.  Canales con la opción PRIVATE activada, no son
	listados.

CHAN_HELP_OP
	Sintaxis: OP #canal nick
	
	Convierte en Operador al usuario indicado en el canal
	indicado.  Por defecto, está limitado a quienes posean nivel
	5 o mayor en el canal.	

CHAN_HELP_DEOP
	Sintaxis: DEOP #canal nick
	
	Quita el estado de Operador al usuario indicado en el canal
	indicado.  Por defecto, está limitado a quienes posean nivel
	5 o mayor en el canal.	

CHAN_HELP_INVITE
	Sintaxis: INVITE canal
	
	Le dice a %S que lo invite a usted al canal indicado.
	Por defecto, solo funciona si su nivel de acceso en dicho
	canal es mayor que 0 (cero).

CHAN_HELP_UNBAN
	Sintaxis: UNBAN canal
	
	Le dice a %S que elimine todos los accesos prohibidos
	previniendo su ingreso a dicho canal.  Por defecto, solo
	funciona si su nivel de acceso en dicho canal es mayor que 0
	(cero).

CHAN_HELP_CLEAR
	Sintaxis: CLEAR canal que
	
	Le dice a %S que elimine ciertos parámetros del canal.
	que puede ser uno de los siguientes:
	
	     modes   Resetea todos los modos del canal (ejemplo,
	                 limpia los modos i,k,l,m,n,p,s,t).
	     bans    Elimina todos los accesos prohibidos al canal.
	     ops     Elimina (quita) el privilegio de operador a
	                 todos los operadores (modo +o) del canal.
	     voices  Elimina (quita) el privilegio de "voz" (modo
	                 +v) a cualquiera del canal que lo tuviese.
	     users   Elimina (Patea -kicks-) TODOS los usuarios del
	                 canal.
	
	Por defecto, su nivel de acceso en el canal debe ser al
	menos 10 (diez) para utilizar este comando.

CHAN_SERVADMIN_HELP
	
	Los siguiente comandos están disponibles para los
	administradores de los Servicios:	
	
	    GETPASS    Devuelve la contraseña del fundador
	               (solo si la encripción esta deshabilitada)
	    FORBID     Previene que un canal pueda ser utilizado
	    STATUS     Devuelve el nivel de acceso actual de un
	               usuario en un canal
	
	Los administradores de Servicios también pueden eliminar
	cualquier canal sin necesidad de identificarse vía
	contraseña, y pueden ver la lista de accesos, la lista de
	AKICK, y listar los valores de nivel (LEVELS) de cualquier
	canal.	

CHAN_SERVADMIN_HELP_DROP
	Sintaxis: DROP canal
	
	Elimina un canal de la lista de %S.  Únicamente
	Administradores de Servicios pueden eliminar un canal para
	lo cual ellos no necesitan identificarse.	

CHAN_SERVADMIN_HELP_SET
	Administradores de Services también pueden definir la opción
	NOEXPIRE, con la cual algún canal se puede prevenir que
	expire.  Adicionalmente, los Administradores de Services
	pueden definir opciones para cualquier canal sin necesidad
	de identificarse con contraseña para ese canal.

CHAN_SERVADMIN_HELP_SET_NOEXPIRE
	Sintaxis: SET canal NOEXPIRE {ON | OFF}
	
	Define si el canal dado expirara.  Si se define ON previene
	que el canal expire.
	
	Limitado a Administradores de Services.

CHAN_SERVADMIN_HELP_LIST
	Sintaxis: LIST especificación
	
	Lista todos los canales que coinciden con la especificación
	dada.  Los canales con la opción PRIVATE activada solo serán
	mostrados a los administradores de Servicios.

CHAN_SERVADMIN_HELP_FORBID
	Sintaxis: FORBID canal
	
	Deshabilita a cualquiera para utilizar o registrar el canal
	dado.  Puede ser cancelado eliminando el canal (dropping).	
	
	Limitado a Administradores de Servicios.

CHAN_SERVADMIN_HELP_STATUS
	Sintaxis: STATUS canal nick
	
	Devuelve el nivel de acceso actual del nick dado en el canal
	dado.  La respuesta es de la forma:
	
	    STATUS canal nick nivel-de-acceso
	
	Limitado a Administradores de Servicios.

###########################################################################
#
# MemoServ help messages (Mensajes de ayuda de MemoServ)
#
###########################################################################

#MEMO_HELP
#	%S es una útileria que permite a usuarios de IRC
#	enviar mensajes cortos a otros usuarios de IRC,
#	independientemente de si están en línea o no, igualmente se
#	puede enviar mensajes a canales(*).  Ambos nicks (remitente
#	y destinatario) deben de estar registrados, así mismo el
#	canal debe de estar registrado si se desea enviar un mensaje
#	a este.
#	
#	Comandos de %S incluyen:
#	
#	    SEND   Envía un mensaje a un nick/canal
#	    LIST   Lista todos tus mensajes
#	    READ   Lee un mensaje
#	    DEL    Borra un mensaje (o todos)
#	    SET    Define opciones relacionadas con mensajes
#	
#	Escriba /msg %S HELP comando para ayuda mas especifica
#	de los comandos arriba mencionados.
#	
#	(*) Por default, cualquier usuario con al menos nivel 10 de
#	    acceso en un canal puede leer los mensajes de ese canal.
#	    Esto puede ser cambiado con el comando %s LEVELS.

MEMO_HELP_SEND
	Sintaxis: SEND nick memo-texto
	
	Envía un memo al usuario especificado.  Dicho usuario
	recibirá una noticia de que tiene un nuevo memo.  Lo
	recibirá inmediatamente si se encuentra en línea, o si no al
	momento de identificarse la próxima vez que se conecte.
	
	Ejemplo: /msg %S SEND joselito Muy bueno tu trabajo de traducción! ;)

#MEMO_HELP_LIST
#	Sintaxis: LIST [NEW]
#	
#	Lista los memos que usted tenga actualmente.
#	Si se indica el parámetro NEW, solo los nuevos memos (los no
#	leídos) serán mostrados. Los memos no leídos están marcados
#	con un "*" a la izquierda del numero de memo.

#MEMO_HELP_READ
#	Sintaxis: READ {numero | LAST}
#	
#	Muestra el contenido del memo indicado en numero, o el
#	ultimo memo (por ejemplo, el recibido mas recientemente) si el
#	parámetro LAST es dado EN VEZ DE numero.

#MEMO_HELP_DEL
#	Sintaxis: DEL {num[-num][,num...] | ALL}
#	
#	Si un numero num es dado, elimina dicho memo.  Usted puede
#	reemplazar un único numero por un rango de números para
#	eliminar múltiples memos simultáneamente, tal como se verá
#	en el segundo ejemplo.
#	
#	Si se ingresa ALL se eliminaran TODOS los memos.
#	
#	Ejemplos:
#	
#	   DEL 1
#	      Elimina su primer memo.
#	
#	   DEL 2-5,7-9
#	      Elimina los memos 2 a 5 y 7 a 9.

MEMO_HELP_SET
	Sintaxis: SET opción parámetros
	
	Establece varias opciones para los memos. Las opciones
	pueden ser:
	
	    NOTIFY      Establece CUANDO debe ser notificado de
	                    nuevos memos
	    LIMIT       Establece el MAXIMO numero de memos que
	                    usted puede recibir
	
	Escriba /msg %S HELP SET opción para obtener mas
	información acerca de la opción específica.

MEMO_HELP_SET_NOTIFY
	Sintaxis: SET NOTIFY {ON | LOGON | NEW | OFF}
	
	Establece CUANDO debe ser notificado de nuevos memos:
	
	    ON      Será notificado cuando se conecte, cuando quite
	                su modo /AWAY y cuando un memo le sea
	                enviado estando usted conectado.
	    LOGON   Será notificado únicamente cuando se conecte,
			y cuando quite su modo /AWAY.
	    NEW     Será notificado solamente cuando le sean
	                enviados.
	    OFF     Usted no recibirá ningún tipo de notificación de
	                memos.
	
	ON es básicamente la combinación de LOGON y NEW
	simultáneamente.

MEMO_HELP_SET_LIMIT
	Sintaxis: SET LIMIT [canal] limite
	
	Establece el máximo numero de memos que usted está
	habilitado a tener.  Si establece limite a 0 (cero) nadie
	podrá enviarle memos a usted.  De todos modos, usted no puede
	establecer limite mas alto que %d.

MEMO_HELP_INFO
	Sintaxis: INFO [canal]
	
	Muestra información de la cantidad de memos que usted tiene,
	cuantos de ellos no han sido leídos, y la cantidad de memos
	que usted puede recibir.

MEMO_SERVADMIN_HELP_SET_LIMIT
	Sintaxis: SET LIMIT [usuario] {limite | NONE} [HARD]
	
	Establece el máximo número de memos que un usuario puede
	tener.  Estableciendo el limite a 0 (cero), previene que
	dicho usuario pueda recibir memos; estableciéndolo a NONE,
	habilita al usuario a recibir y mantener tantos memos como
	el desee.  Si usted NO ESPECIFICA usuario SU propio límite
	es el que se establece.
	
	Agregando el parámetro HARD, previene que el usuario pueda
	cambiar dicho límite.  No agregando HARD tiene el efecto
	opuesto, habilitando al usuario a cambiar el límite (aun si
	previamente había sido establecido con el parámetro HARD).
	
	El uso del comando SET LIMIT está limitado a los
	Administradores de Servicios.  Otros usuarios solo pueden
	establecer sus propios límites, no pueden remover sus
	límites, no pueden establecer límites superiores a %d, y no
	pueden establecer el limite HARD.

#MEMO_SERVADMIN_HELP_INFO
#	Sintaxis: INFO [usuario]
#	
#	Sin parámetros, muestra información del número de memos que
#	tu tienes, cuantos están sin leer, y cuantos memos puedes
#	recibir.
#	
#	Con el parámetro usuario, muestra la misma información, pero
#	del usuario especificado. Esta opción está limitada
#	únicamente a los Administradores de Servicios.

###########################################################################
#
# OperServ help messages (mensajes de ayuda de OperServ)
#
###########################################################################

#OPER_HELP
#	Comandos de %S:
#	    GLOBAL      Envía un mensaje a TODOS los usuarios
#	    STATS       Muestra el estado de los servicios y de la red
#	    OPER LIST   Lista todos los Operadores de los Servicios
#	    ADMIN LIST  Lista todos los Administradores de Servicios
#	
#	Comandos disponibles solo para Operadores de Servicios y
#	administradores:
#	    MODE        Cambia los modos de un canal
#	    KICK        Patea a un usuario de un canal
#	    AKILL       Manipula la lista de AKILL
#	
#	Comandos disponibles solo para los Administradores de
#	Servicios:
#	    OPER        Modifica la lista de Operadores de Servicios
#	    JUPE        "Jupiter" un servidor
#	    RAW         envía un código RAW al servidor IRC
#	    SET         Establece varias opciones globales de los
#	                    servicios
#	    QUIT        Da por finalizada la ejecución del programa
#	                    de Servicios
#	    RESTART     Graba la base de datos y reinicializa los
#	                    Servicios
#	
#	Comandos disponibles únicamente para los súper-usuarios de los
#	servicios:
#	    ADMIN       Modifica la lista de Administradores de Servicios
#	
#	Aviso: TODOS los comandos enviados a %S son
#	registrados! (log)

OPER_HELP_GLOBAL
	Sintaxis: GLOBAL mensaje
	
	Habilita a los IRCops a enviar un mensaje a todos los
	usuarios en la red.  El mensaje puede ser enviado para %s.

OPER_HELP_STATS
	Sintaxis: STATS [AKILL | ALL]
	
	Sin opciones, muestra la cantidad de usuarios e IRCops en
	línea, el mas alto numero de usuarios en línea desde que los
	servicios se pusieron en funcionamiento, y desde cuando
	están funcionando los servicios.
	
	Con el parámetro AKILL muestra la longitud de la lista de
	AKILL y la fecha actual de expiración.
	
	El parámetro ALL solo está disponible para los
	Administradores de Servicios y muestra el uso de memoria de
	los Servicios.  Utilizando esta opción puede congelar los
	Servicios por un breve período de tiempo en grandes redes,
	por lo tanto no abuse de ella.
	
	UPTIME puede ser utilizado como sinónimo de STATS.

OPER_HELP_OPER
	Sintaxis: OPER ADD nick
	          OPER DEL nick
	          OPER LIST
	
	Habilita a los Administradores de Servicios para agregar
	(ADD), borrar (DEL) o listar (LIST) usuarios de la lista de
	operadores.  Un usuario que se encuentre en la lista de
	operadores y que se identifique ante %s podrá acceder
	a los comandos de Operador de Servicios.
	
	Cualquiera de los operadores de IRC (IRCops) puede utilizar
	el comando OPER LIST. Todos los otros comandos solo pueden
	ser utilizados por los Administradores de Servicios.

OPER_HELP_ADMIN
	Sintaxis: ADMIN ADD nick
	          ADMIN DEL nick
	          ADMIN LIST
	
	Habilita a los súper-usuarios de los Servicios a agregar
	(ADD), eliminar (DEL) o listar (LIST) usuarios de la lista
	de administradores.  Un usuario que se encuentre en la lista
	de operadores y que se identifique ante %s podrá
	acceder a los comandos de Administrador de Servicios.
	
	Cualquiera de los operadores de IRC (IRCops) puede utilizar
	el comando ADMIN LIST.  Todos los otros comandos solo pueden
	ser utilizados por el súper-usuarios de los Servicios.

OPER_HELP_MODE
	Sintaxis: MODE canal modos
	
	Habilita a los operadores de Servicios a establecer los
	modos para cualquier canal.  Los parámetros son los mismos
	que se utilizan en la forma estándar de /MODE.
	
	Solo disponible para los Operadores de Servicios.

OPER_HELP_CLEARMODES
	Sintaxis: CLEARMODES canal [ALL]
	
	Elimina los BAN y los modos de un canal (i,k,l,m,n,p,s,t).
	Si el parámetro ALL es dado, también quita los modos
	Operador y voz que haya en el canal (modos +o y +v).
	
	Solo disponible para los Operadores de Servicios.

OPER_HELP_KICK
	Sintaxis: KICK canal usuario razón
	
	Habilita a un IRCop a sacar a un usuario de cualquier canal.
	Los parámetros son los mismos que para un /KICK común.  El
	mensaje de kick mostrará al usuario el nombre del IRCop que
	está enviando el comando. Por ejemplo:	
	
	*** SpamMan has been kicked off channel #my_channel by %S (Alcan (Flood))
	
	Solo disponible para los Operadores de Servicios.

OPER_HELP_AKILL
	Sintaxis: AKILL ADD [+expiry] mask razón
	          AKILL DEL mask
	          AKILL LIST [mask]
	          AKILL VIEW [mask]
	
	Habilita a los IRCops a manipular la lista de AKILL.  Si un
	usuario que figura en dicha lista, intenta conectarse, los
	Servicios bloquearán su conexión y darán instrucciones a
	todos los servidores que lo soporten, que agreguen un ban
	(k-line) para la máscara que coincida con la del usuario.
	
	AKILL ADD agrega el usuario dado (con la mascara
	usuario@host) a la lista de akill por la razón dada (que
	debe ser dada).
	AKILL DEL quita el usuario dado de la lista de AKILL si el
	mismo está presente.
	AKILL LIST muestra todos los usuarios que están actualmente
	en la lista. Si una máscara es dada, el listado estará
	limitado a las ocurrencias.
	AKILL VIEW es mas explícito que AKILL LIST, dado que muestra
	quien agregó el AKILL, la fecha en que fue hecho, cuando
	expira, la máscara utilizada y la razón.
	
	Opcionalmente, un tiempo de expiración puede ser
	suministrado con el comando AKILL ADD.  El tiempo de
	expiración PRECEDE a la máscara usuario@host y es
	especificado como un entero (esto es, sin decimales) seguido
	de: d para especificar días, h (horas), o m (minutos).
	Combinaciones (como 1h30m) no están permitidas.  Si un
	especificador de unidades no es incluido, por defecto se
	asumen DIAS (entonces, poner +30 sin nada, significa 30
	días).  Para agregar un AKILL que NO expira, utilice +0.  Si
	la máscara de usuario comienza con +, un período de
	expiración deberá ser indicado, aún si es el mismo período
	que el asignado por defecto.  El tiempo de expiración por
	defecto, se puede ver utilizando el comando STATS AKILL.
	
	La razón está indicada para uso de los IRCops, y NO será
	mostrada al usuario cuando se efectúe el KILL.
	
	Solo disponible para los Operadores de Servicios.

OPER_HELP_JUPE
	Sintaxis: JUPE server
	
	Instruye a los Servicios a crear un servidor satélite.  Esto
	es, para simular un "servidor" que esté conectado a los
	Servicios los cuales previenen la conexión del servidor real
	con ese nombre.  Esta simulación puede ser removida,
	utilizando un SQUIT estándar.
	
	Solo disponible para los Administradores de Servicios.

OPER_HELP_RAW
	Sintaxis: RAW texto
	
	Envía una cadena de caracteres directamente al servidor al
	cual los Servicios están conectados.
	Esta orden tiene un muy limitado rango de usos, y puede
	causar estragos en una red si se utiliza inadecuadamente.
	NO USE ESTE COMANDO a menos que usted esté completamente
	seguro de lo que está haciendo!
	
	Solo disponible para los Administradores de Servicios.

OPER_HELP_SET
	Sintaxis: SET opción modo
	
	Establece varas de las opciones globales de los Servicios.
	Los nombres de las opciones actualmente definidas son:	
	    READONLY   Establece el modo de "solo-lectura" o
	                   "lectura-escritura"
	    DEBUG      Activa o desactiva el modo depuración
	
	Solo disponible para los Administradores de Servicios.

OPER_HELP_SET_DEBUG
	Sintaxis: SET DEBUG {ON | OFF | num}
	
	Activa o desactiva el modo depuración.  En modo depuración,
	todos los datos enviados y recibidos así como los mensajes
	de depuración, son grabados en un archivo de registro (log).
	Si num es dado, el modo de depuración es activado, con
	nivel de depuración establecido a num.
	
	Esta opción es equivalente a la opción -debug de la
	línea de comando.

OPER_HELP_QUIT
	Sintaxis: QUIT
	
	Causa la desactivación de los Servicios; las bases de datos
	no se graban.  Este comando no debe utilizarse a menos que
	se sospeche de un daño en las bases de datos en-memoria y no
	deseen grabarse.  Para desactivación normal de los
	Servicios, utilice el comando SHUTDOWN.
	
	Limitado a Administradores de Servicios.

OPER_HELP_SHUTDOWN
	Sintaxis: SHUTDOWN
	
	Causa que los servicios graben todas las bases de datos y
	luego se desactiven.
	
	Limitado a Administradores de Servicios.

OPER_HELP_RESTART
	Sintaxis: RESTART
	
	Causa que Services grabe todas las bases de datos y
	reinicialize (ejemplo: terminar y ejecutar inmediatamente el
	programa).
	
	Limitado a Administradores de Servicios.
