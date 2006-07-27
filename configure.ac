dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.57)
AC_COPYRIGHT([$Id$])

AC_INIT([services], [1.9.2])
AC_CONFIG_SRCDIR(src/send.c)
AC_PREFIX_DEFAULT([\${HOME}/services])
AC_CONFIG_HEADER([include/sysconf.h])
AC_GNU_SOURCE

dnl Checks for programs.
AC_PROG_CC
AC_LANG(C)

AC_ISC_POSIX
AC_C_INLINE
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PATH_PROG(RM, rm)
AC_PATH_PROG(CP, cp)
AC_PATH_PROG(MV, mv)
AC_PATH_PROG(LN, ln)
AC_PATH_PROG(SED, sed)
AC_PATH_PROG(AR, ar)
AC_PATH_PROG(LD, ld)
AC_PATH_PROG(PERL, perl)

AC_CHECK_LIB(z, compress)
AC_CHECK_LIB(m, floor)

dnl Checks for header files.
AC_HEADER_STDC

AC_CHECK_HEADERS(strings.h sys/select.h)

AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

dnl Checks for library functions.
AC_TYPE_SIGNAL
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF

AC_CHECK_FUNCS(strerror snprintf stricmp strcasecmp strdup strspn mmap)
AC_CHECK_FUNCS(strsignal gettimeofday setgrent umask fork gethostbyname)

dnl We prefer gcc -MM because it's a lot less bloated
AC_PATH_PROG(MKDEP, mkdep)
AC_PATH_PROG(MAKEDEPEND, makedepend)

AC_MSG_CHECKING(how to generate dependency info)

STDOUT="> .depend"

if test "$ac_cv_c_compiler_gnu" = yes; then
    AC_MSG_RESULT(gcc -MM)
    MKDEP="$CC -MM"
elif test ! -z "$MKDEP"; then
    AC_MSG_RESULT(mkdep)

elif test "$SunWorkShop" = yes; then
    AC_MSG_RESULT($CC -xM)
    MKDEP="$CC -xM"
    STDOUT="> .depend 2> /dev/null"
elif test ! -z "$MAKEDEPEND"; then
    AC_MSG_RESULT(makedepend)
    MKDEP="$MAKEDEPEND -f-"
else
    AC_MSG_RESULT([nothing suitable.. forget it!])
    MKDEP=":"
fi

AC_SUBST(MKDEP)
AC_SUBST(STDOUT)


dnl Solaris has to be weird doesn't it...
AC_CHECK_LIB(nsl, gethostbyname, AC_SUBST(LNSL, [-lnsl]))

AC_ARG_WITH(domain,
    AC_HELP_STRING([--with-domain=DOMAIN],
    [Common domain name of your network]),
    [network_domain=$with_domain], [network_domain="none"])

AC_ARG_WITH(logdir,
    AC_HELP_STRING([--with-logdir=DIR],
    [Where to store logs, defaults to $prefix/log]))

AC_ARG_WITH(extra-fascism,
    AC_HELP_STRING([--with-extra-fascism],
    [Add extra gcc3-specific warning flags]),
    [CFLAGS="$CFLAGS -W -Wfloat-equal -Wbad-function-cast -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs"])

AC_ARG_WITH(profiling,
    AC_HELP_STRING([--with-profiling],
    [Add extra profiling code for use with gprof]),
    [CFLAGS="$CFLAGS -pg"; AC_SUBST(PG, [-pg])])


dnl **** Check for mysql libs and headers **** - stolen from some random gnome app
AC_SUBST(MYSQL_LIBS)
AC_SUBST(MYSQL_INCLUDEDIR)
AC_MSG_CHECKING([for mysql])
AC_TRY_CPP([#include <mysql.h>],
[mysql_found=yes],
[  for ac_dir in \
        /opt/mysql/include      \
    /usr/local/mysql/include    \
    /usr/mysql/include      \
    /usr/unsupported/mysql/include  \
    /usr/include/mysql      \
    /usr/local/include/mysql    \
    ; \
    do
     if test -r "$ac_dir/mysql.h"; then
       mysql_found=yes ac_mysql_includes=$ac_dir
       break;
         fi
    done])

if test $mysql_found = no; then
 AC_MSG_ERROR(mysql libraries and header files were not found)
fi
if test $mysql_found = yes; then
   ac_save_LIBS="$LIBS"
   LIBS="-lmysqlclient $LIBS"
   AC_TRY_LINK(, [mysql_connect()],,
               [LIBS="$ac_save_LIBS"
              for ac_dir in \
             /usr/lib             \
                 /usr/lib/mysql       \
                 /usr/local/lib/mysql \
                 /opt/mysql/lib       \
         /usr/unsupported/mysql/lib \
                 ; \
              do
               for ac_extension in a so sl; do
                 if test -r "$ac_dir/libmysqlclient.$ac_extension"; then
                    ac_mysql_libs="-L$ac_dir -lmysqlclient"
                    break 2;
                 fi
               done
              done])
fi
AC_MSG_RESULT([libraries $ac_mysql_libs, headers $ac_mysql_includes])

if test $mysql_found = yes; then
  MYSQL_LIBS=$ac_mysql_libs
  MYSQL_INCLUDEDIR=$ac_mysql_includes
fi

AC_ARG_WITH(mysql,
    AC_HELP_STRING([--with-mysql=PFX],
    [Prefix where MySQL is installed (usually /usr/local)]))
AC_ARG_WITH(mysql-includedir, 
    AC_HELP_STRING([--with-mysql-includes=PFX],
    [Prefix where MySQL includes are found (usually /usr/local/include/mysql)]))
AC_ARG_WITH(mysql-libs,
    AC_HELP_STRING([--with-mysql-libraries=PFX],
    [Prefix where MySQL libraries are found (usually /usr/local/lib/mysql)]))

AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_C_BIGENDIAN

if test "x$with_logdir" = "x"; then
  logdir="${prefix}/logs"
else
  logdir=$with_logdir
fi

AC_DEFINE_UNQUOTED(LOGDIR, "$logdir", [Place logs will be kept])
AC_DEFINE_UNQUOTED(ETCDIR, "${prefix}/etc", [Conf file etc dir])
AC_DEFINE_UNQUOTED(SERVICES_DIR, "${prefix}", [services dir])
AC_DEFINE_UNQUOTED(SERVICES_BIN, "${prefix}/bin/services", [services])


AH_BOTTOM([
#if SIZEOF_SHORT==2
typedef signed short int16;
typedef unsigned short uint16;
#elif SIZEOF_INT==2
typedef signed int int16;
typedef unsigned int uint16;
#else
#  error "No 16-bit integer type!"
#endif

#if SIZEOF_INT==4
typedef signed int int32;
typedef unsigned int uint32;
#elif SIZEOF_LONG==4
typedef signed long int32;
typedef unsigned long uint32;
#else
#  error "No 32-bit integer type!"
#endif
])

if test "$prefix" = "NONE"; then
    AC_DEFINE_UNQUOTED(SERVICES_PREFIX, "$ac_default_prefix", [Prefix where the services are installed.])
else
dnl Don't get bitten by Cygwin's stupidity if the user specified
dnl a custom prefix with a trailing slash

    prefix=`echo $prefix | sed 's/\/$//'`
    AC_DEFINE_UNQUOTED(SERVICES_PREFIX, "$prefix", [Prefix where the services are installed.])
fi

OUR_CFLAGS="-g -Wall"

AC_SUBST(OUR_LDFLAGS, $LIBS)
AC_SUBST(OUR_CFLAGS, $OUR_CFLAGS)
        
AC_OUTPUT(Makefile src/Makefile src/lang/Makefile)
