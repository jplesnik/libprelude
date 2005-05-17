# Copyright (C) 2004 Free Software Foundation, Inc.
# This file is free software, distributed under the terms of the GNU
# General Public License.  As a special exception to the GNU General
# Public License, this file may be distributed as part of a program
# that contains a configuration script generated by Autoconf, under
# the same distribution terms as the rest of that program.
#
# Generated by gnulib-tool.
#
# Invoked as: gnulib-tool --import --dir=. --lib=libmissing --source-base=libmissing --m4-base=libmissing/m4 --libtool strdup strndup strsep snprintf vsnprintf getaddrinfo regex inet_ntop timegm fts
# Reproduce by: gnulib-tool --import --dir=. --lib=libmissing --source-base=libmissing --m4-base=libmissing/m4 --aux-dir=. --libtool  alloca alloca-opt cycle-check dirfd extensions fts getaddrinfo gettext hash inet_ntop lstat minmax mktime regex restrict snprintf stat stat-macros stdbool strdup strndup strnlen strpbrk strsep timegm time_r unistd-safer vasnprintf vsnprintf xalloc xsize

AC_DEFUN([gl_EARLY],
[
  AC_GNU_SOURCE
  gl_USE_SYSTEM_EXTENSIONS
])

AC_DEFUN([gl_INIT],
[
changequote(,)dnl
LTALLOCA=`echo "$ALLOCA" | sed 's/\.[^.]* /.lo /g;s/\.[^.]*$/.lo/'`
changequote([, ])dnl
AC_SUBST(LTALLOCA)
  gl_FUNC_ALLOCA
  gl_FUNC_DIRFD
  dnl gl_USE_SYSTEM_EXTENSIONS must be added quite early to configure.ac.
  gl_FUNC_FTS
  gl_GETADDRINFO
  dnl you must add AM_GNU_GETTEXT([external]) or similar to configure.ac.
  gl_HASH
  gl_INET_NTOP
  gl_FUNC_LSTAT
  gl_FUNC_MKTIME
  gl_REGEX
  gl_C_RESTRICT
  gl_FUNC_SNPRINTF
  gl_FUNC_STAT
  gl_STAT_MACROS
  AM_STDBOOL_H
  gl_FUNC_STRDUP
  gl_FUNC_STRNDUP
  gl_FUNC_STRNLEN
  gl_FUNC_STRPBRK
  gl_FUNC_STRSEP
  gl_FUNC_TIMEGM
  gl_TIME_R
  gl_UNISTD_SAFER
  gl_FUNC_VASNPRINTF
  gl_FUNC_VSNPRINTF
  gl_XALLOC
  gl_XSIZE
])

dnl Usage: gl_MODULES(module1 module2 ...)
AC_DEFUN([gl_MODULES], [])

dnl Usage: gl_SOURCE_BASE(DIR)
AC_DEFUN([gl_SOURCE_BASE], [])

dnl Usage: gl_M4_BASE(DIR)
AC_DEFUN([gl_M4_BASE], [])

dnl Usage: gl_LIB(LIBNAME)
AC_DEFUN([gl_LIB], [])

dnl Usage: gl_LGPL
AC_DEFUN([gl_LGPL], [])

# gnulib.m4 ends here
