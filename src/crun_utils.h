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

#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <unistd.h>

#ifdef __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_unused __attribute__ ((unused))
#অন্যথা
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_unused
#endif

#define OOM()                            \
  কৰা                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } যেতিয়ালৈ (0)

#define crun_make_error libcrun_make_error

__attribute__ ((malloc)) স্থির অন্তঃস্থ শূন্য *
xmalloc (size_t size)
{
  শূন্য *res = malloc (size);
  যদি (UNLIKELY (res == রিক্ত))
    OOM ();
  ঘূৰাই res;
}

__attribute__ ((malloc)) স্থির অন্তঃস্থ শূন্য *
xmalloc0 (size_t size)
{
  শূন্য *res = calloc (1, size);
  যদি (UNLIKELY (res == রিক্ত))
    OOM ();
  ঘূৰাই res;
}

__attribute__ ((malloc)) স্থির অন্তঃস্থ শূন্য *
xrealloc (শূন্য *ptr, size_t size)
{
  শূন্য *res = realloc (ptr, size);
  যদি (UNLIKELY (res == রিক্ত))
    OOM ();
  ঘূৰাই res;
}

স্থির অন্তঃস্থ ৱর্ণ *
xstrdup (ধ্রুৱ ৱর্ণ *str)
{
  ৱর্ণ *ret;
  যদি (str == রিক্ত)
    ঘূৰাই রিক্ত;
  ret = strdup (str);
  যদি (UNLIKELY (ret == রিক্ত))
    OOM ();
  ঘূৰাই ret;
}

স্থির অন্তঃস্থ শূন্য
cleanup_freep (শূন্য *p)
{
  শূন্য **pp = (শূন্য **) p;
  মুক্ত (*pp);
}

স্থির অন্তঃস্থ শূন্য
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  যদি (file)
    (শূন্য) fclose (file);
}

স্থির অন্তঃস্থ শূন্য
cleanup_closep (শূন্য *p)
{
  পূর্ণাঙ্ক *pp = (পূর্ণাঙ্ক *) p;
  যদি (*pp >= 0)
    close (*pp);
}

#define cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#define cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#define cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
স্থির অন্তঃস্থ শূন্য
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  যদি (*p)
    libcrun_context_free (*p);
}

#define cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

স্থির অন্তঃস্থ শূন্য
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  যদি (*p)
    libcrun_container_free (*p);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#endif
