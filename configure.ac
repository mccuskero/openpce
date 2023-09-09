## Process this file with autoconf to create configure. -*- autoconf -*-
# Copyright 2001  Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307  USA

AC_PREREQ(2.59)

## ------------------------ ##
## Autoconf initialisation. ##
## ------------------------ ##
AC_INIT([opce], [0.0.1], [mccuskero@ct.metrocast.net])


## ------------------------ ##
## Config initialization    ##
## ------------------------ ##
AC_CONFIG_SRCDIR([m4/ltmain.sh])
AC_CONFIG_AUX_DIR(m4)
AC_CONFIG_HEADER(config.h)

## ------------------------ ##
## Automake Initialisation. ##
## ------------------------ ##
AM_INIT_AUTOMAKE(AC_PACKAGE_TARNAME, AC_PACKAGE_VERSION)

## ------------------------ ##
## Function declarations.   ##
## ------------------------ ##
AC_DEFUN([SN_CHECK_DECL],[
AC_MSG_CHECKING([whether $1 must be declared])
AC_CACHE_VAL(sn_cv_decl_needed_$1,
[AC_TRY_COMPILE([
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <syslog.h>
],
[char *(*pfn) = (char *(*)) $1],
eval "sn_cv_decl_needed_$1=no",eval "sn_cv_decl_needed_$1=yes") ])

if eval "test \"`echo '$sn_cv_decl_needed_'$1`\" != no"; then
 AC_MSG_RESULT(yes)
 ifelse([$2], , :, [$2])
else
 AC_MSG_RESULT(no)
 ifelse([$3], , ,[$3])
fi
])dnl

AC_DEFUN([SN_CHECK_DECLS],
[for sn_decl in $1
do
sn_def_decl=`echo $sn_decl | tr [a-z] [A-Z]`
SN_CHECK_DECL($sn_decl,
[
AC_DEFINE_UNQUOTED(NEED_DECL_$sn_def_decl, 1,
                  [you have this cuz autoheader is dumb])
$2], $3)dnl
done
])

AC_DEFUN([FAIL_MESSAGE],[
   echo
   echo
   echo "**********************************************"
   echo "  ERROR: unable to find" $1
   echo "  checked in the following places"
   for i in `echo $2`; do
     echo "        $i"
   done
   echo "**********************************************"
   echo
   exit 1
])              

## ------------------------ ##
## Release making           ##
## ------------------------ ##
OPCE_MAJOR_VERSION=0
OPCE_MINOR_VERSION=1
OPCE_MICRO_VERSION=0
OPCE_INTERFACE_AGE=0
OPCE_BINARY_AGE=0
OPCE_VERSION=$OPCE_MAJOR_VERSION.$OPCE_MINOR_VERSION.$OPCE_MICRO_VERSION
AC_SUBST(OPCE_MAJOR_VERSION)
AC_SUBST(OPCE_MINOR_VERSION)
AC_SUBST(OPCE_MICRO_VERSION)
AC_SUBST(OPCE_INTERFACE_AGE)
AC_SUBST(OPCE_BINARY_AGE)
AC_SUBST(OPCE_VERSION)

# libtool versioning
LT_RELEASE=$OPCE_MAJOR_VERSION.$OPCE_MINOR_VERSION
LT_CURRENT=`expr $OPCE_MICRO_VERSION - $OPCE_INTERFACE_AGE`
LT_REVISION=$OPCE_INTERFACE_AGE
LT_AGE=`expr $OPCE_BINARY_AGE - $OPCE_INTERFACE_AGE`

# used in Makefile.am
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

## ---------------- ##
## compiler checks. ##
## ---------------- ##
AC_PROG_CC
AC_PROG_CXX
AC_C_CONST
AC_EXEEXT

## ----------------------- ##
## Libtool initialisation. ##
## ----------------------- ##
AC_LIBTOOL_DLOPEN
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL
AC_SUBST([LIBTOOL_DEPS])

if ./libtool --features | grep '^enable static libraries$' > /dev/null 2>&1; then
  STATIC=-static
else
  STATIC=
fi
AC_SUBST([STATIC])

AM_CONDITIONAL([BINARY_HELLDL],
    [grep '^global_symbol_pipe=..*$' ./libtool >/dev/null])

## --------------------- ##
## Internationalization. ##
## --------------------- ##
#dnl Set of available languages.
#ALL_LINGUAS="de fr es ko nl no pl pt sl sv"
# dnl internationalization macros
# AM_GNU_GETTEXT

## ---------------- ##
## Misc progs.      ##
## ---------------- ##
dnl Checks for programs.
AC_PROG_MAKE_SET
AC_PROG_INSTALL

## ---------------- ##
## documentation.   ##
## ---------------- ##
BB_ENABLE_DOXYGEN

## ---------------- ##
## compiler checks. ##
## ---------------- ##

## ---------------------------- ##
## Checks for header files.     ##
## ---------------------------- ##
AC_STDC_HEADERS
AC_CHECK_HEADERS([string.h math.h fcntl.h sys/file.h sys/param.h])

# Use the specified CC and LD
AC_PROG_CC
AC_EXEEXT
AM_PROG_LD
AC_SUBST(LD)
AM_PROG_NM
AC_SUBST(NM)
AC_PROG_LN_S

## --------------------------- ##
## Libraries required  ##
## --------------------------- ##
AC_CHECK_LIBM
AC_SUBST([LIBM])

## --------------------------- ##
## Libraries required  ##
## --------------------------- ##
#AC_FUNC_ALLOCA

## --------------------------- ##
## Debug and Profile options   ##
## --------------------------- ##
AC_ARG_ENABLE(debug,
[  --enable-debug           Enable debugging options (bugreports and developers only)],
                [ if test -n "$GCC"; then
                      # get rid of default autofoo CFLAGS
                      CFLAGS=`echo $CFLAGS | sed -e "s/-O2//" -e "s/-g//"`
                      CFLAGS="$CFLAGS -g -O0 -DDEBUG"
                  else
                    CFLAGS="$CFLAGS -DDEBUG"
                  fi
                ],)

AC_ARG_ENABLE(profile,
[  --enable-profile         Enable profiling options (developers only)],
        [ if test -n "$GCC"; then
            CFLAGS="$CFLAGS -DPROFILE -pg"
          else
            CFLAGS="$CFLAGS -DPROFILE"
          fi
        ],)

## ---------------------------- ##
## Platform specific checkts.   ##
## ---------------------------- ##
case "$host" in
  *-openbsd2.6|*-openbsd2.5|*-openbsd2.4|*-openbsd2.3*)
    AC_DEFINE(OPENBSD,,[Define if OPENBSD])
    AC_DEFINE(BROKEN_SIOCGIFMTU,,[Define if BROKEN_SIOCGIFMTU])
    ;;
  *-openbsd*)
    AC_DEFINE(OPENBSD)
    ;;
  *-linux*)
    linux=yes
    AC_DEFINE(LINUX,,[Define if LINUX])
    # libpcap doesn't even LOOK at the timeout you give it under Linux
       AC_DEFINE(PCAP_TIMEOUT_IGNORED,,[Define if PCAP_TIMEOUT_IGNORED])
    AC_SUBST(extra_incl)
    extra_incl=-I/usr/include/pcap
    ;;
  *-freebsd*)
    AC_DEFINE(FREEBSD,,[Define if FREEBSD])
    ;;
  *-apple*)
    AC_DEFINE(MACOS,,[Define if MACOS])
    AC_DEFINE(BROKEN_SIOCGIFMTU)
    LDFLAGS="${LDFLAGS} -L/sw/lib"
    extra_incl=-I/sw/include
esac

# any sparc platform has to have this one defined.
AC_MSG_CHECKING(for sparc alignment)
if eval "echo $host_cpu|grep -i sparc >/dev/null"; then
     AC_DEFINE(WORDS_MUSTALIGN,,[Define if WORDS_MUSTALIGN])
     AC_MSG_RESULT(yes)
else
     AC_MSG_RESULT(no)
fi


## ---------------------------- ##
## Check decls and functions.   ##
## Perform a build.             ##
## ---------------------------- ##
SN_CHECK_DECLS(printf fprintf syslog puts fputs fputc fopen  \
           fclose fwrite fflush getopt bzero bcopy memset strtol  \
           strcasecmp strncasecmp strerror perror socket sendto   \
           vsnprintf snprintf strtoul)

AC_CHECK_FUNCS(snprintf)
AC_CHECK_FUNCS(strlcpy)
AC_CHECK_FUNCS(strlcat)
AC_CHECK_FUNCS(strerror)
AC_CHECK_FUNCS(vswprintf)
AC_CHECK_FUNCS(wprintf)

AC_MSG_CHECKING([for sizeof(unsigned long)])
AC_RUN_IFELSE(
[AC_LANG_PROGRAM(
[],
[[
    if (sizeof(unsigned long) == 8)
        return 0;

    return 1;
]])],
[ulong_is_64_bit="yes"],
[ulong_is_64_bit="no"])
if test "x$ulong_is_64_bit" = "xyes"; then
    AC_MSG_RESULT([64 bits])
    AC_DEFINE([ULONGIS64BIT], [], [If unsigned long is 64 bits])
else
    AC_MSG_RESULT([32 bits])
fi

AC_TRY_COMPILE([
#include <stdio.h>
],[char *foo; foo = sys_errlist[0];],
AC_DEFINE(ERRLIST_PREDEFINED,,[Define if ERRLIST_PREDEFINED]))

AC_MSG_CHECKING(for __FUNCTION__)
AC_TRY_COMPILE([
#include <stdio.h>
],[printf ("%s", __FUNCTION__);],
sn_cv_have___FUNCTION__=yes, sn_cv__have___FUNCTION__=no)
if test "x$sn_cv_have___FUNCTION__" = "xyes"; then
   AC_MSG_RESULT(yes)
   AC_DEFINE(HAVE___FUNCTION__, 1,
	     [Define if the compiler understands __FUNCTION__.])
else
   AC_MSG_RESULT(no)
   AC_MSG_CHECKING(for __func__)
   AC_TRY_COMPILE([
#include <stdio.h>
],[printf ("%s", __func__);],
sn_cv_have___func__=yes, sn_cv__have___func__=no)
   if test "x$sn_cv_have___func__" = "xyes"; then
      AC_MSG_RESULT(yes)
      AC_DEFINE(HAVE___func__, 1,
		[Define if the compiler understands __func__.])
      AC_DEFINE(__FUNCTION__, __func__, [Define __FUNCTION__ as required.])
   else
      AC_MSG_RESULT(no)
      AC_DEFINE(__FUNCTION__, "mystery function")
   fi
fi

## ---------------- ##
## silk check       ##
## ---------------- ##
CT_CHECK_SILKTOOLS

## ----------------------- ##
## Pcap checks.            ##
## ----------------------- ##
AC_ARG_WITH(libpcap_includes,
	[  --with-libpcap-includes=DIR  libpcap include directory],
	[with_libpcap_includes="$withval"],[with_libpcap_includes=no])

AC_ARG_WITH(libpcap_libraries,
	[  --with-libpcap-libraries=DIR  libpcap library directory],
	[with_libpcap_libraries="$withval"],[with_libpcap_libraries=no])


if test "$with_libpcap_includes" != "no"; then
   CPPFLAGS="${CPPFLAGS} -I${with_libpcap_includes}"
fi

if test "$with_libpcap_libraries" != "no"; then
   LDFLAGS="${LDFLAGS}  -L${with_libpcap_libraries}"
fi

# --with-libpfring-* options
AC_ARG_WITH(libpfring_includes,
       [  --with-libpfring-includes=DIR  libpfring include directory],
       [with_libpfring_includes="$withval"],[with_libpfring_includes=no])

AC_ARG_WITH(libpfring_libraries,
       [  --with-libpfring-libraries=DIR  libpfring library directory],
       [with_libpfring_libraries="$withval"],[with_libpfring_libraries=no])

if test "$with_libpfring_includes" != "no"; then
   CPPFLAGS="${CPPFLAGS} -I${with_libpfring_includes}"
fi

if test "$with_libpfring_libraries" != "no"; then
   LDFLAGS="${LDFLAGS} -L${with_libpfring_libraries}"
fi

AC_CHECK_LIB(m, floor,,)

LPCAP=""
AC_CHECK_LIB(pcap, pcap_datalink,, LPCAP="no")

# If the normal AC_CHECK_LIB for pcap fails then check to see if we are
# using a pfring-enabled pcap.
if test "$LPCAP" = "no"; then
  PFRING_H=""
  AC_CHECK_HEADERS(pfring.h,, PFRING_H="no")

# It is important to have the AC_CHECK_LIB for the pfring library BEFORE
# the one for pfring-enabled pcap. When the Makefile is created, all the
# libraries used during linking are added to the LIBS variable in the
# Makefile in the opposite orded that their AC_CHECK_LIB macros appear
# in configure.in. Durring linking, the pfring library (-lpfring) MUST come
# _after_ the libpcap library (-lpcap) or linking will fail.
  PFRING_L=""
  AC_CHECK_LIB(pfring, pfring_open,, PFRING_L="no")

  LPFRING_PCAP=""
  AC_CHECK_LIB(pcap, pfring_open,, LPFRING_PCAP="no",-lpfring)
fi

# If both the AC_CHECK_LIB for normal pcap and pfring-enabled pcap fail then exit.
if test "$LPCAP" = "no"; then
  if test "$LPFRING_PCAP" = "no"; then
    echo
    echo "   ERROR!  Libpcap library/headers (libpcap.a (or .so)/pcap.h)"
    echo "   not found, go get it from http://www.tcpdump.org"
    echo "   or use the --with-libpcap-* options, if you have it installed"
    echo "   in unusual place.  Also check if your libpcap depends on another"
    echo "   shared library that may be installed in an unusual place"
    exit 1
  fi
fi


# This is to determine which pcap library version is being used.  The reason being
# that versions < 0.9 do not accumulate packet statistics whereas >= 0.9 do accumulate.
# This is Linux only.  The check is done after pcre because the code below uses pcre.
pcap_version_check="yes"
if test "x$linux" = "xyes"; then
    if test "x$pcap_version_check" = "xyes"; then
        AC_MSG_CHECKING([for libpcap version >= 0.9])
        AC_RUN_IFELSE(
        [AC_LANG_PROGRAM(
        [[
        #include <pcap.h>
        #include <string.h>
        extern char pcap_version[];
        ]],
        [[
           if (strcmp(pcap_version, "0.9.0") < 0)
               return 1;
        ]])],
        [libpcap_version_09="yes"],
        [libpcap_version_09="no"])
        AC_MSG_RESULT($libpcap_version_09)
        if test "x$libpcap_version_09" = "xyes"; then
            AC_DEFINE([LIBPCAP_ACCUMULATES], [], [For libpcap versions that accumulate stats])
        fi
    else
        libpcap_version_09="no"
        AC_DEFINE([LIBPCAP_ACCUMULATES], [], [For libpcap versions that accumulate stats])
    fi

    # there is a bug in the Linux code in 0.9.0 - 0.9.4 where the pcap
    # stats are doubled.
    if test "x$libpcap_version_09" = "xyes"; then
        AC_MSG_CHECKING(for libpcap version 0.9.0 - 0.9.4)
        AC_RUN_IFELSE(
        [AC_LANG_PROGRAM(
        [[
        #include <pcap.h>
        #include <string.h>
        extern char pcap_version[];
        ]],
        [[
           if (strcmp(pcap_version, "0.9.5") < 0)
               return 1;
        ]])],
        [libpcap_version_09_bug="no"],
        [libpcap_version_09_bug="yes"])
        AC_MSG_RESULT($libpcap_version_09_bug)
    else
        libpcap_version_09_bug="no"
    fi

    if test "x$libpcap_version_09_bug" = "xyes"; then
        AC_DEFINE([LINUX_LIBPCAP_DOUBLES_STATS], [], [For Linux libpcap versions 0.9.0 to 0.9.4])
    fi
else
    AC_DEFINE([LIBPCAP_ACCUMULATES], [], [For libpcap versions that accumulate stats])
fi


## ----------------------- ##
## PCRE checks.            ##
## ----------------------- ##
AC_ARG_WITH(libpcre_includes,
       [  --with-libpcre-includes=DIR  libpcre include directory],
       [with_libpcre_includes="$withval"],[with_libpcre_includes=no])

AC_ARG_WITH(libpcre_libraries,
       [  --with-libpcre-libraries=DIR  libpcre library directory],
       [with_libpcre_libraries="$withval"],[with_libpcre_libraries=no])

if test "$with_libpcre_includes" != "no"; then
   CPPFLAGS="${CPPFLAGS} -I${with_libpcre_includes}"
else
   CPPFLAGS="${CPPFLAGS} `pcre-config --cflags`"
fi

if test "$with_libpcre_libraries" != "no"; then
   LDFLAGS="${LDFLAGS}  -L${with_libpcre_libraries}"
else
   LDFLAGS="${LDFLAGS} `pcre-config --libs`"
fi

# PCRE configuration (required)
# Verify that we have the headers
PCRE_H=""
AC_CHECK_HEADERS(pcre.h,, PCRE_H="no")
if test "$PCRE_H" = "no"; then
  echo
  echo "   ERROR!  Libpcre header not found."
  echo "   Get it from http://www.pcre.org"
  exit 1
fi

# Verify that we have the library
PCRE_L=""
pcre_version_four=""
AC_CHECK_LIB(pcre, pcre_compile, ,PCRE_L="no")
if test "$PCRE_L" = "no"; then
    echo
    echo "   ERROR!  Libpcre library not found."
    echo "   Get it from http://www.pcre.org"
    echo
    exit 1
else
    AC_MSG_CHECKING(for libpcre version 4.0 or greater)
    AC_TRY_LINK(
      [#include <pcre.h>],
      [
       #if (PCRE_MAJOR < 4)
       #error "Version failure"
       #else
       int a,b,c,d;
       pcre *tmp;
       a = pcre_copy_named_substring(tmp, "", &b, c, "", "", d);
       #endif
      ],
      [pcre_version_four="yes"],
      [pcre_version_four="no"])
fi

if test "$pcre_version_four" != "yes"; then
    AC_MSG_RESULT(no)
    echo
    echo "    ERROR!  Libpcre library version >= 4.0 not found."
    echo "    Get it from http://www.pcre.org"
    echo
    exit 1
else
    AC_MSG_RESULT(yes)
fi

## ----------------------- ##
## Database checks.        ##
## ----------------------- ##

## ---------------- ##
## postgres check   ##
## ---------------- ##
CT_CHECK_POSTGRES_DB


## ---------------- ##
## mysql check      ##
## ---------------- ##
## - create m4 script like postgres   ##

AC_ARG_WITH(mysql,
    [  --with-mysql=DIR        support for mysql],
    [ with_mysql="$withval"],
    [ with_mysql=no ])

AC_ARG_WITH(mysql_includes,
	[  --with-mysql-includes=DIR  mysql include directory],
	[with_mysql_includes="$withval"; with_mysql="yes"],[with_mysql_includes=no])

AC_ARG_WITH(mysql_libraries,
	[  --with-mysql-libraries=DIR  mysql library directory],
	[with_mysql_libraries="$withval"; with_mysql="yes"],[with_mysql_libraries=no])

if test "$with_mysql" != "no"; then
  if test "$with_mysql" = "yes"; then
    if test "$with_mysql_includes" != "no"; then
        mysql_inc_directory="$with_mysql_includes";
    else
        mysql_inc_directory="$default_directory";
    fi
    if test "$with_mysql_libraries" != "no"; then
        mysql_lib_directory="$with_mysql_libraries";
    else
        mysql_lib_directory="$default_directory";
    fi
    mysql_fail="yes"
  elif test -d "$withval"; then
    AC_MSG_WARN(Providing a directory for the --with-mysql option)
    AC_MSG_WARN(will be deprecated in the future in favour of)
    AC_MSG_WARN(--with-mysql-libraries and --with-mysql-includes)
    AC_MSG_WARN(options to address issues with non-standard)
    AC_MSG_WARN(installations and 64bit platforms.)
    mysql_inc_directory="$withval"
    mysql_lib_directory="$withval"
    mysql_fail="yes"
  elif test "$with_mysql" = ""; then
    mysql_inc_directory="$default_directory"
    mysql_lib_directory="$default_directory"
    mysql_fail="yes"
  fi

  AC_MSG_CHECKING(for mysql)

  for i in $mysql_inc_directory; do
      if test -r $i/mysql.h; then
        MYSQL_INC_DIR=$i
      elif test -r $i/include/mysql.h; then
        MYSQL_INC_DIR=$i/include
      elif test -r $i/include/mysql/mysql.h; then
        MYSQL_INC_DIR=$i/include/mysql
      elif test -r $i/mysql/mysql.h; then
        MYSQL_INC_DIR=$i/mysql
      elif test -r $i/mysql/include/mysql.h; then
        MYSQL_INC_DIR=$i/mysql/include
      fi
  done

  for i in $mysql_lib_directory; do
      if test -z "$MYSQL_LIB_DIR"; then
        str="$i/libmysqlclient.*"
        for j in `echo $str`; do
          if test -r $j; then
            MYSQL_LIB_DIR=$i
            break 2
          fi
        done
      fi
      if test -z "$MYSQL_LIB_DIR"; then
        str="$i/lib/libmysqlclient.*"
        for j in `echo $str`; do
          if test -r $j; then
            MYSQL_LIB_DIR=$i/lib
            break 2
          fi
        done
      fi
      if test -z "$MYSQL_LIB_DIR"; then
        str="$i/mysql/libmysqlclient.*"
        for j in `echo $str`; do
          if test -r $j; then
            MYSQL_LIB_DIR=$i/mysql
            break 2
          fi
        done
      fi
      if test -z "$MYSQL_LIB_DIR"; then
        str="$i/mysql/lib/libmysqlclient.*"
        for j in `echo $str`; do
          if test -r $j; then
            MYSQL_LIB_DIR=$i/mysql/lib
            break 2
          fi
        done
      fi
      if test -z "$MYSQL_LIB_DIR"; then
        str="$i/lib/mysql/libmysqlclient.*"
        for j in `echo $str`; do
          if test -r $j; then
            MYSQL_LIB_DIR=$i/lib/mysql
            break 2
          fi
        done
      fi
  done

  if test -z "$MYSQL_INC_DIR"; then
    if test "$mysql_fail" != "no"; then
      tmp=""
      for i in $mysql_inc_directory; do
        tmp="$tmp $i $i/include $i/include/mysql $i/mysql $i/mysql/include"
      done
      FAIL_MESSAGE("mysql headers (mysql.h)", $tmp)
    else
      AC_MSG_RESULT(no)
    fi
  else

    if test -z "$MYSQL_LIB_DIR"; then
      if test "$mysql_fail" != "no"; then
        tmp=""
        for i in $mysql_lib_directory; do
          tmp="$tmp $i $i/lib $i/mysql $i/mysql/lib $i/lib/mysql"
        done
        FAIL_MESSAGE("mysqlclient library (libmysqlclient.*)", $tmp)
      else
        AC_MSG_RESULT(no)
      fi
    else
      AC_MSG_RESULT(yes)
      LDFLAGS="${LDFLAGS} -L${MYSQL_LIB_DIR}"
      CPPFLAGS="${CPPFLAGS} -I${MYSQL_INC_DIR} -DENABLE_MYSQL"
      AC_CHECK_LIB(z, compress)
      LIBS="-lmysqlclient ${LIBS}"
    fi
  fi
fi


## ---------------- ##
## oracle check      ##
## ---------------- ##
## - create m4 script like postgres   ##

AC_ARG_WITH(oracle,
    [  --with-oracle=DIR       support for oracle],
    [ with_oracle="$withval" ],
    [ with_oracle=no ])

if test "$with_oracle" != "no"; then
  if test "$with_oracle" = "yes"; then
    oracle_directory="$default_directory ${ORACLE_HOME}"
    oracle_fail="yes"
  elif test -d $withval; then
    oracle_directory="$withval $default_directory ${ORACLE_HOME}"
    oracle_fail="yes"
  elif test "$with_oracle" = ""; then
    oracle_directory="$default_directory ${ORACLE_HOME}"
    oracle_fail="no"
  fi

  AC_MSG_CHECKING(for oracle)

  for i in $oracle_directory; do
    if test -r $i/rdbms/demo/oci.h; then
      ORACLE_DIR=$i
    fi
  done

  if test -z "$ORACLE_DIR"; then
    if test "$oracle_fail" != "no"; then
      tmp=""
      for i in $oracle_directory; do
        tmp="$tmp $i/rdbms/demo"
      done
      FAIL_MESSAGE("OCI header file (oci.h)", $tmp)
    else
      AC_MSG_RESULT(no)
    fi
  else
    for i in rdbms/demo rdbms/public network/public; do
      ORACLE_CPP_FLAGS="$ORACLE_CPP_FLAGS -I$ORACLE_DIR/$i"
    done
    ORACLE_LIB_DIR="$ORACLE_DIR/lib"
    AC_MSG_RESULT(yes)

    LDFLAGS="${LDFLAGS} -L${ORACLE_LIB_DIR}"
    CPPFLAGS="${CPPFLAGS} ${ORACLE_CPP_FLAGS} -DENABLE_ORACLE"

    ORACLE_LIBS="-lclntsh"
    if test -r $ORACLE_LIB_DIR/libwtc9.so; then
      ORACLE_LIBS="${ORACLE_LIBS} -lwtc9"
    elif test -r $ORACLE_LIB_DIR/libwtc8.so; then
      ORACLE_LIBS="${ORACLE_LIBS} -lwtc8"
    fi
    LIBS="${LIBS} ${ORACLE_LIBS}"
  fi
fi

## ----------------------- ##
## Outputs.                ##
## ----------------------- ##
AC_CONFIG_FILES([Makefile \
           opce-api/Makefile \
           opce-api/modules/Makefile \
           opce-api/modules/libpcdao/Makefile \
           opce-api/modules/libpcdao/modules/Makefile \
           opce-api/modules/libpcdao/modules/libpcdao/Makefile \
           opce-api/modules/libpcdao/modules/libpcdao/include/Makefile \
           opce-api/modules/libpcdao/modules/libpcdao/include/opce/Makefile \
           opce-api/modules/libpcdao/modules/libpcdao/include/opce/pcdao/Makefile \
           opce-api/modules/libpcdao/modules/libpcdao/src/Makefile \
           opce-api/modules/libpcff/Makefile \
           opce-api/modules/libpcff/modules/Makefile \
           opce-api/modules/libpcff/modules/libpcff/Makefile \
           opce-api/modules/libpcff/modules/libpcff/include/Makefile \
           opce-api/modules/libpcff/modules/libpcff/include/opce/Makefile \
           opce-api/modules/libpcff/modules/libpcff/include/opce/pcff/Makefile \
           opce-api/modules/libpcff/modules/libpcff/include/opce/pcff/event/Makefile \
           opce-api/modules/libpcff/modules/libpcff/include/opce/pcff/event/proxy/Makefile \
           opce-api/modules/libpcff/modules/libpcff/include/opce/pcff/filesystem/Makefile \
           opce-api/modules/libpcff/modules/libpcff/include/opce/pcff/proxy/Makefile \
           opce-api/modules/libpcff/modules/libpcff/include/opce/pcff/serialize/Makefile \
           opce-api/modules/libpcff/modules/libpcff/include/opce/pcff/services/Makefile \
           opce-api/modules/libpcff/modules/libpcff/include/opce/pcff/socket/Makefile \
           opce-api/modules/libpcff/modules/libpcff/include/opce/pcff/thread/Makefile
           opce-api/modules/libpcff/modules/libpcff/src/Makefile \
           opce-api/modules/libpcsec/Makefile \
           opce-api/modules/libpcsec/modules/Makefile \
           opce-api/modules/libpcsec/modules/libpcsec/Makefile \
           opce-api/modules/libpcsec/modules/libpcsec/include/Makefile \
           opce-api/modules/libpcsec/modules/libpcsec/include/opce/Makefile \
           opce-api/modules/libpcsec/modules/libpcsec/include/opce/pcsec/Makefile \
           opce-api/modules/libpcsec/modules/libpcsec/include/opce/pcsec/network/Makefile \
           opce-api/modules/libpcsec/modules/libpcsec/include/opce/pcsec/network/snort/Makefile \
           opce-api/modules/libpcsec/modules/libpcsec/include/opce/pcsec/network/honeytrap/Makefile \
           opce-api/modules/libpcsec/modules/libpcsec/src/Makefile])
#           opce-api/modules/libpcsec/servers/Makefile \
#           opce-api/modules/libpcsec/servers/silkproxyd/Makefile \
#           opce-api/modules/libpcsec/servers/silkproxyd/src/Makefile \
#           opce-api/modules/libpcsec/servers/sproxyd/Makefile \
#           opce-api/modules/libpcsec/servers/sproxyd/src/Makefile])

AC_OUTPUT

