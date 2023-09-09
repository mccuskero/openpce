##### 
#
# SYNOPSIS
#
#   CT_CHECK_SILKTOOLS
#
# DESCRIPTION
#
#   This macro tries to find the headers and librarys for the
#   SiLK Toolset to build client applications.
#
#   If includes are found, the variable SKINCPATH will be set. If
#   librarys are found, the variable SKLIBPATH will be set. if no check
#   was successful, the script exits with a error message.
#
# LAST MODIFICATION
#
#   2008-11-13
#
# COPYLEFT
#
#   Copyright (c) 2008 Owen McCusker <mccuskero@ct.metrocast.net>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([CT_CHECK_SILKTOOLS], [

AC_ARG_WITH(silk,
	[  --with-silk=PREFIX		Prefix of your SiLK Tools installation],
	[sk_prefix=$withval], [sk_prefix=])
AC_ARG_WITH(silk-inc,
	[  --with-silk-inc=PATH		Path to the include directory of SiLK Tools],
	[sk_inc=$withval], [sk_inc=])
AC_ARG_WITH(silk-lib,
	[  --with-silk-lib=PATH		Path to the librarys of SiLK Tools],
	[sk_lib=$withval], [sk_lib=])


AC_SUBST(SKINCPATH)
AC_SUBST(SKLIBPATH)

if test "$sk_prefix" != ""; then
   AC_MSG_CHECKING([for SiLK Tools includes in $sk_prefix/include])
   if test -f "$sk_prefix/include/silk/silk.h" ; then
      SKINCPATH="-I$sk_prefix/include"
      AC_MSG_RESULT([yes])
   else
      AC_MSG_ERROR(silk.h not found)
   fi
   AC_MSG_CHECKING([for SiLK Tools librarys in $sk_prefix/lib])

   case $host_os in
   darwin*)
     if test -f "$sk_prefix/lib/libsilk.dylib" ; then
        SKLIBPATH="-L$sk_prefix/lib"
        AC_MSG_RESULT([yes])
     else
        AC_MSG_ERROR(libsilk.dylib not found)
     fi
   ;;
   *)
     if test -f "$sk_prefix/lib/libsilk.so" ; then
        SKLIBPATH="-L$sk_prefix/lib"
        AC_MSG_RESULT([yes])
     else
        AC_MSG_ERROR(libsilk.so not found)
     fi
   ;;
   esac 

else
  if test "$sk_inc" != ""; then
    AC_MSG_CHECKING([for SiLK Tools includes in $sk_inc])
    if test -f "$sk_inc/silk/silk.h" ; then
      SKINCPATH="-I$sk_inc"
      AC_MSG_RESULT([yes])
    else
      AC_MSG_ERROR(silk.h not found)
    fi
  fi
  if test "$sk_lib" != ""; then
    AC_MSG_CHECKING([for SiLK Tools librarys in $sk_lib])

    case $host_os in
    darwin*)
      if test -f "$sk_lib/libsilk.dylib" ; then
        SKLIBPATH="-L$sk_lib"
        AC_MSG_RESULT([yes])
      else
        AC_MSG_ERROR(libsilk.so not found)
      fi
    ;; 
    *)
      if test -f "$sk_lib/libsilk.so" ; then
        SKLIBPATH="-L$sk_lib"
        AC_MSG_RESULT([yes])
      else
        AC_MSG_ERROR(libsilk.so not found)
      fi
    ;;
    esac

  fi
  
fi

if test "$SKINCPATH" = "" ; then
  AC_CHECK_HEADER([silk/silk.h], [], AC_MSG_ERROR(silk.h not found))
fi
if test "$SKLIBPATH" = "" ; then
  AC_CHECK_LIB(silk, skAppRegister, [], AC_MSG_ERROR(libsilk.so not found))
fi

])
