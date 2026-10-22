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

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>

#ifdef __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_unused __attribute__ ((unused))
#ಅನ್ಯಥಾ
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_unused
#endif

#define OOM()                            \
  ಕುರು                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } ಯಾವತ್ (0)

#define crun_make_error libcrun_make_error

__attribute__ ((malloc)) ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ *
xmalloc (size_t size)
{
  ಶೂನ್ಯ *res = malloc (size);
  ಯದಿ (UNLIKELY (res == ರಿಕ್ತ))
    OOM ();
  ಪ್ರತ್ಯಾಗಮ res;
}

__attribute__ ((malloc)) ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ *
xmalloc0 (size_t size)
{
  ಶೂನ್ಯ *res = calloc (1, size);
  ಯದಿ (UNLIKELY (res == ರಿಕ್ತ))
    OOM ();
  ಪ್ರತ್ಯಾಗಮ res;
}

__attribute__ ((malloc)) ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ *
xrealloc (ಶೂನ್ಯ *ptr, size_t size)
{
  ಶೂನ್ಯ *res = realloc (ptr, size);
  ಯದಿ (UNLIKELY (res == ರಿಕ್ತ))
    OOM ();
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ವರ್ಣ *
xstrdup (ಧ್ರುವ ವರ್ಣ *str)
{
  ವರ್ಣ *ret;
  ಯದಿ (str == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  ret = strdup (str);
  ಯದಿ (UNLIKELY (ret == ರಿಕ್ತ))
    OOM ();
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_freep (ಶೂನ್ಯ *p)
{
  ಶೂನ್ಯ **pp = (ಶೂನ್ಯ **) p;
  ಮುಕ್ತ (*pp);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  ಯದಿ (file)
    (ಶೂನ್ಯ) fclose (file);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_closep (ಶೂನ್ಯ *p)
{
  ಪೂರ್ಣಾಙ್ಕ *pp = (ಪೂರ್ಣಾಙ್ಕ *) p;
  ಯದಿ (*pp >= 0)
    close (*pp);
}

#define cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#define cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#define cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  ಯದಿ (*p)
    libcrun_context_free (*p);
}

#define cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  ಯದಿ (*p)
    libcrun_container_free (*p);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#endif
