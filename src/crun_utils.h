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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CRUN_UTILS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CRUN_UTILS_H

/* Self-contained helpers for the crun binary so that it does not have to
   include the internal libcrun/utils.h (which pulls in the internal,
   non-public headers).  The binary is a pure consumer of the public
   <libcrun.h> API; everything here is either libc or a trivial inline.  */

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_ਅਣਵਰਤਿਆ __attribute__ ((ਅਣਵਰਤਿਆ))
#ਨਹੀਂਤਾਂ
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_ਅਣਵਰਤਿਆ
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OOM()                            \
  ਕਰੋ                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ crun_make_error libcrun_make_error

__attribute__ ((ਸਮ੍ਰਿਤੀ_ਦਿਓ)) ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *
xmalloc (size_t ਮਾਪ)
{
  ਖਾਲੀ *res = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਮਾਪ);
  ਜੇ (UNLIKELY (res == ਨਿਰਾ))
    OOM ();
  ਵਾਪਸ res;
}

__attribute__ ((ਸਮ੍ਰਿਤੀ_ਦਿਓ)) ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *
xmalloc0 (size_t ਮਾਪ)
{
  ਖਾਲੀ *res = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ (1, ਮਾਪ);
  ਜੇ (UNLIKELY (res == ਨਿਰਾ))
    OOM ();
  ਵਾਪਸ res;
}

__attribute__ ((ਸਮ੍ਰਿਤੀ_ਦਿਓ)) ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *
xrealloc (ਖਾਲੀ *ptr, size_t ਮਾਪ)
{
  ਖਾਲੀ *res = realloc (ptr, ਮਾਪ);
  ਜੇ (UNLIKELY (res == ਨਿਰਾ))
    OOM ();
  ਵਾਪਸ res;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ *
xstrdup (ਅਚਲ ਅੱਖਰ *str)
{
  ਅੱਖਰ *ret;
  ਜੇ (str == ਨਿਰਾ)
    ਵਾਪਸ ਨਿਰਾ;
  ret = strdup (str);
  ਜੇ (UNLIKELY (ret == ਨਿਰਾ))
    OOM ();
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_freep (ਖਾਲੀ *p)
{
  ਖਾਲੀ **pp = (ਖਾਲੀ **) p;
  ਮੁਕਤ (*pp);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  ਜੇ (file)
    (ਖਾਲੀ) fclose (file);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_closep (ਖਾਲੀ *p)
{
  ਪੂਰਨ *pp = (ਪੂਰਨ *) p;
  ਜੇ (*pp >= 0)
    close (*pp);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  ਜੇ (*p)
    libcrun_context_free (*p);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  ਜੇ (*p)
    libcrun_container_free (*p);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#ਜੇ_ਅੰਤ
