/* Convert a string representation of time to a tm structure.
   Copyright (C) 2001-2003, 2005, 2007 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef GNULIB_STRPTIME_H_
#define GNULIB_STRPTIME_H_

#include <time.h>

#if ! HAVE_STRPTIME
/* See the POSIX:2001 specification
   <http://www.opengroup.org/susv3xsh/strptime.html>.  */
extern char *strptime (const char *restrict s, const char *restrict format,
		       struct tm *restrict tm);
#endif

#endif /* GNULIB_STRPTIME_H_ */