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

#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <unistd.h>

#ifdef __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_unused __attribute__ ((unused))
#وإلا
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_unused
#endif

#define OOM()                            \
  𐡏𐡁𐡃                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } طالما (0)

#define crun_make_error libcrun_make_error

__attribute__ ((malloc)) ساكن مضمن فراغ *
xmalloc (size_t size)
{
  فراغ *res = malloc (size);
  𐡄𐡍 (UNLIKELY (res == عدم))
    OOM ();
  أرجع res;
}

__attribute__ ((malloc)) ساكن مضمن فراغ *
xmalloc0 (size_t size)
{
  فراغ *res = calloc (1, size);
  𐡄𐡍 (UNLIKELY (res == عدم))
    OOM ();
  أرجع res;
}

__attribute__ ((malloc)) ساكن مضمن فراغ *
xrealloc (فراغ *ptr, size_t size)
{
  فراغ *res = realloc (ptr, size);
  𐡄𐡍 (UNLIKELY (res == عدم))
    OOM ();
  أرجع res;
}

ساكن مضمن حرف *
xstrdup (ثابت حرف *str)
{
  حرف *ret;
  𐡄𐡍 (str == عدم)
    أرجع عدم;
  ret = strdup (str);
  𐡄𐡍 (UNLIKELY (ret == عدم))
    OOM ();
  أرجع ret;
}

ساكن مضمن فراغ
cleanup_freep (فراغ *p)
{
  فراغ **pp = (فراغ **) p;
  حرر (*pp);
}

ساكن مضمن فراغ
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  𐡄𐡍 (file)
    (فراغ) fclose (file);
}

ساكن مضمن فراغ
cleanup_closep (فراغ *p)
{
  صحيح *pp = (صحيح *) p;
  𐡄𐡍 (*pp >= 0)
    close (*pp);
}

#define cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#define cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#define cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
ساكن مضمن فراغ
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  𐡄𐡍 (*p)
    libcrun_context_free (*p);
}

#define cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

ساكن مضمن فراغ
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  𐡄𐡍 (*p)
    libcrun_container_free (*p);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#endif
