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
#که_نه_تعریف_شوی CRUN_UTILS_H
#تعریف_کړه CRUN_UTILS_H

/* Self-contained helpers for the crun binary so that it does not have to
   include the internal libcrun/utils.h (which pulls in the internal,
   non-public headers).  The binary is a pure consumer of the public
   <libcrun.h> API; everything here is either libc or a trivial inline.  */

#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <unistd.h>

#که_تعریف_شوی __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_ناکارول_شوی __attribute__ ((ناکارول_شوی))
#وإلا
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_ناکارول_شوی
#د_که_پای

#تعریف_کړه OOM()                            \
  افعل                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } طالما (0)

#تعریف_کړه crun_make_error libcrun_make_error

__attribute__ ((حافظه_ورکړه)) ساكن مضمن فراغ *
xmalloc (size_t اندازه)
{
  فراغ *res = حافظه_ورکړه (اندازه);
  إذا (UNLIKELY (res == عدم))
    OOM ();
  أرجع res;
}

__attribute__ ((حافظه_ورکړه)) ساكن مضمن فراغ *
xmalloc0 (size_t اندازه)
{
  فراغ *res = حافظه_صفر_ورکړه (1, اندازه);
  إذا (UNLIKELY (res == عدم))
    OOM ();
  أرجع res;
}

__attribute__ ((حافظه_ورکړه)) ساكن مضمن فراغ *
xrealloc (فراغ *ptr, size_t اندازه)
{
  فراغ *res = realloc (ptr, اندازه);
  إذا (UNLIKELY (res == عدم))
    OOM ();
  أرجع res;
}

ساكن مضمن حرف *
xstrdup (ثابت حرف *str)
{
  حرف *ret;
  إذا (str == عدم)
    أرجع عدم;
  ret = strdup (str);
  إذا (UNLIKELY (ret == عدم))
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
  إذا (file)
    (فراغ) fclose (file);
}

ساكن مضمن فراغ
cleanup_closep (فراغ *p)
{
  صحيح *pp = (صحيح *) p;
  إذا (*pp >= 0)
    close (*pp);
}

#تعریف_کړه cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#تعریف_کړه cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#تعریف_کړه cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
ساكن مضمن فراغ
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  إذا (*p)
    libcrun_context_free (*p);
}

#تعریف_کړه cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

ساكن مضمن فراغ
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  إذا (*p)
    libcrun_container_free (*p);
}

#تعریف_کړه cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#د_که_پای
