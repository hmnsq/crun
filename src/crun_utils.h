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

#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>

#ifdef __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_unused __attribute__ ((unused))
#ଅନ୍ୟଥା
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_unused
#endif

#define OOM()                            \
  କର                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } ଯେତେବେଳେ (0)

#define crun_make_error libcrun_make_error

__attribute__ ((malloc)) ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ *
xmalloc (size_t size)
{
  ଶୂନ୍ୟ *res = malloc (size);
  ଯଦି (UNLIKELY (res == ଖାଲି))
    OOM ();
  ଫେରନ୍ତୁ res;
}

__attribute__ ((malloc)) ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ *
xmalloc0 (size_t size)
{
  ଶୂନ୍ୟ *res = calloc (1, size);
  ଯଦି (UNLIKELY (res == ଖାଲି))
    OOM ();
  ଫେରନ୍ତୁ res;
}

__attribute__ ((malloc)) ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ *
xrealloc (ଶୂନ୍ୟ *ptr, size_t size)
{
  ଶୂନ୍ୟ *res = realloc (ptr, size);
  ଯଦି (UNLIKELY (res == ଖାଲି))
    OOM ();
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅକ୍ଷର *
xstrdup (ଅଚଳ ଅକ୍ଷର *str)
{
  ଅକ୍ଷର *ret;
  ଯଦି (str == ଖାଲି)
    ଫେରନ୍ତୁ ଖାଲି;
  ret = strdup (str);
  ଯଦି (UNLIKELY (ret == ଖାଲି))
    OOM ();
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_freep (ଶୂନ୍ୟ *p)
{
  ଶୂନ୍ୟ **pp = (ଶୂନ୍ୟ **) p;
  ମୁକ୍ତ (*pp);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  ଯଦି (file)
    (ଶୂନ୍ୟ) fclose (file);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_closep (ଶୂନ୍ୟ *p)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pp = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *) p;
  ଯଦି (*pp >= 0)
    close (*pp);
}

#define cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#define cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#define cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  ଯଦି (*p)
    libcrun_context_free (*p);
}

#define cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  ଯଦି (*p)
    libcrun_container_free (*p);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#endif
