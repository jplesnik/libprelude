# strsep.m4 serial 1
dnl Copyright (C) 2002, 2003, 2004 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

AC_DEFUN([gl_FUNC_STRSEP],
[
  AC_REPLACE_FUNCS(strsep)
  AC_CHECK_DECLS_ONCE(strsep)
  gl_PREREQ_STRSEP
])

# Prerequisites of lib/strsep.c.
AC_DEFUN([gl_PREREQ_STRSEP], [:])
