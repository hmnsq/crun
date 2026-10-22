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
#اگر_غیر_تعریف_شدہ CRUN_UTILS_H
#تعریف_کرو CRUN_UTILS_H

/* Self-contained helpers for the crun binary so that it does not have to
   include the internal libcrun/utils.h (which pulls in the internal,
   non-public headers).  The binary is a pure consumer of the public
   <libcrun.h> API; everything here is either libc or a trivial inline.  */

#شامل <stdlib.h>
#شامل <stdio.h>
#شامل <string.h>
#شامل <unistd.h>

#اگر_تعریف_شدہ __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_غیر_مستعمل __attribute__ ((غیر_مستعمل))
#ورنہ
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_غیر_مستعمل
#اگر_ختم

#تعریف_کرو OOM()                            \
  کرو                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } جب (0)

#تعریف_کرو crun_make_error libcrun_make_error

__attribute__ ((حافظہ_دو)) ساکن اندرونی خالی *
xmalloc (size_t سائز)
{
  خالی *res = حافظہ_دو (سائز);
  اگر (UNLIKELY (res == تہی))
    OOM ();
  واپس res;
}

__attribute__ ((حافظہ_دو)) ساکن اندرونی خالی *
xmalloc0 (size_t سائز)
{
  خالی *res = صفر_حافظہ_دو (1, سائز);
  اگر (UNLIKELY (res == تہی))
    OOM ();
  واپس res;
}

__attribute__ ((حافظہ_دو)) ساکن اندرونی خالی *
xrealloc (خالی *ptr, size_t سائز)
{
  خالی *res = realloc (ptr, سائز);
  اگر (UNLIKELY (res == تہی))
    OOM ();
  واپس res;
}

ساکن اندرونی حرف *
xstrdup (ثابت حرف *str)
{
  حرف *ret;
  اگر (str == تہی)
    واپس تہی;
  ret = strdup (str);
  اگر (UNLIKELY (ret == تہی))
    OOM ();
  واپس ret;
}

ساکن اندرونی خالی
cleanup_freep (خالی *p)
{
  خالی **pp = (خالی **) p;
  آزاد (*pp);
}

ساکن اندرونی خالی
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  اگر (file)
    (خالی) fclose (file);
}

ساکن اندرونی خالی
cleanup_closep (خالی *p)
{
  صحیح *pp = (صحیح *) p;
  اگر (*pp >= 0)
    close (*pp);
}

#تعریف_کرو cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#تعریف_کرو cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#تعریف_کرو cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
ساکن اندرونی خالی
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  اگر (*p)
    libcrun_context_free (*p);
}

#تعریف_کرو cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

ساکن اندرونی خالی
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  اگر (*p)
    libcrun_container_free (*p);
}

#تعریف_کرو cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#اگر_ختم
