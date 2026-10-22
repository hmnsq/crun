/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CRUN_UTILS_H
#define CRUN_UTILS_H

/* Self-contained helpers for the crun binary so that it does not have to
   include the internal libcrun/utils.h (which pulls in the internal,
   non-public headers).  The binary is a pure consumer of the public
   <libcrun.h> API; everything here is either libc or a trivial inline.  */

#شامل <stdlib.h>
#شامل <stdio.h>
#شامل <string.h>
#شامل <unistd.h>

#ifdef __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_unused __attribute__ ((unused))
#وگرنه
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_unused
#endif

#define OOM()                            \
  انجامبده                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } وقتی (0)

#define crun_make_error libcrun_make_error

__attribute__ ((malloc)) ایستا درونخطی تهی *
xmalloc (size_t size)
{
  تهی *res = malloc (size);
  اگر (UNLIKELY (res == پوچ))
    OOM ();
  بازگشت res;
}

__attribute__ ((malloc)) ایستا درونخطی تهی *
xmalloc0 (size_t size)
{
  تهی *res = calloc (1, size);
  اگر (UNLIKELY (res == پوچ))
    OOM ();
  بازگشت res;
}

__attribute__ ((malloc)) ایستا درونخطی تهی *
xrealloc (تهی *ptr, size_t size)
{
  تهی *res = realloc (ptr, size);
  اگر (UNLIKELY (res == پوچ))
    OOM ();
  بازگشت res;
}

ایستا درونخطی نویسه *
xstrdup (ثابت نویسه *str)
{
  نویسه *ret;
  اگر (str == پوچ)
    بازگشت پوچ;
  ret = strdup (str);
  اگر (UNLIKELY (ret == پوچ))
    OOM ();
  بازگشت ret;
}

ایستا درونخطی تهی
cleanup_freep (تهی *p)
{
  تهی **pp = (تهی **) p;
  آزاد (*pp);
}

ایستا درونخطی تهی
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  اگر (file)
    (تهی) fclose (file);
}

ایستا درونخطی تهی
cleanup_closep (تهی *p)
{
  صحیح *pp = (صحیح *) p;
  اگر (*pp >= 0)
    close (*pp);
}

#define cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#define cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#define cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
ایستا درونخطی تهی
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  اگر (*p)
    libcrun_context_free (*p);
}

#define cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

ایستا درونخطی تهی
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  اگر (*p)
    libcrun_container_free (*p);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#endif
