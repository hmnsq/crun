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

#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <unistd.h>

#ifdef __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_unused __attribute__ ((unused))
#இல்லையெனில்
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_unused
#endif

#define OOM()                            \
  செய்                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } போது (0)

#define crun_make_error libcrun_make_error

__attribute__ ((malloc)) நிலையான உள்வரி வெறுமை *
xmalloc (size_t size)
{
  வெறுமை *res = malloc (size);
  எனில் (UNLIKELY (res == வெற்று))
    OOM ();
  திரும்பு res;
}

__attribute__ ((malloc)) நிலையான உள்வரி வெறுமை *
xmalloc0 (size_t size)
{
  வெறுமை *res = calloc (1, size);
  எனில் (UNLIKELY (res == வெற்று))
    OOM ();
  திரும்பு res;
}

__attribute__ ((malloc)) நிலையான உள்வரி வெறுமை *
xrealloc (வெறுமை *ptr, size_t size)
{
  வெறுமை *res = realloc (ptr, size);
  எனில் (UNLIKELY (res == வெற்று))
    OOM ();
  திரும்பு res;
}

நிலையான உள்வரி எழுத்து *
xstrdup (மாறிலி எழுத்து *str)
{
  எழுத்து *ret;
  எனில் (str == வெற்று)
    திரும்பு வெற்று;
  ret = strdup (str);
  எனில் (UNLIKELY (ret == வெற்று))
    OOM ();
  திரும்பு ret;
}

நிலையான உள்வரி வெறுமை
cleanup_freep (வெறுமை *p)
{
  வெறுமை **pp = (வெறுமை **) p;
  விடுவி (*pp);
}

நிலையான உள்வரி வெறுமை
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  எனில் (file)
    (வெறுமை) fclose (file);
}

நிலையான உள்வரி வெறுமை
cleanup_closep (வெறுமை *p)
{
  முழு *pp = (முழு *) p;
  எனில் (*pp >= 0)
    close (*pp);
}

#define cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#define cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#define cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
நிலையான உள்வரி வெறுமை
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  எனில் (*p)
    libcrun_context_free (*p);
}

#define cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

நிலையான உள்வரி வெறுமை
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  எனில் (*p)
    libcrun_container_free (*p);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#endif
