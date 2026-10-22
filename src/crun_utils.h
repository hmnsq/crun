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

#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <unistd.h>

#ifdef __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_unused __attribute__ ((unused))
#അല്ലെങ്കിൽ
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_unused
#endif

#define OOM()                            \
  ചെയ്യുക                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } ആകുമ്പോൾ (0)

#define crun_make_error libcrun_make_error

__attribute__ ((malloc)) സ്ഥിതം വരിയിൽ ശൂന്യം *
xmalloc (size_t size)
{
  ശൂന്യം *res = malloc (size);
  എങ്കിൽ (UNLIKELY (res == ശൂന്യ))
    OOM ();
  തിരികെ res;
}

__attribute__ ((malloc)) സ്ഥിതം വരിയിൽ ശൂന്യം *
xmalloc0 (size_t size)
{
  ശൂന്യം *res = calloc (1, size);
  എങ്കിൽ (UNLIKELY (res == ശൂന്യ))
    OOM ();
  തിരികെ res;
}

__attribute__ ((malloc)) സ്ഥിതം വരിയിൽ ശൂന്യം *
xrealloc (ശൂന്യം *ptr, size_t size)
{
  ശൂന്യം *res = realloc (ptr, size);
  എങ്കിൽ (UNLIKELY (res == ശൂന്യ))
    OOM ();
  തിരികെ res;
}

സ്ഥിതം വരിയിൽ അക്ഷരം *
xstrdup (സ്ഥിരം അക്ഷരം *str)
{
  അക്ഷരം *ret;
  എങ്കിൽ (str == ശൂന്യ)
    തിരികെ ശൂന്യ;
  ret = strdup (str);
  എങ്കിൽ (UNLIKELY (ret == ശൂന്യ))
    OOM ();
  തിരികെ ret;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_freep (ശൂന്യം *p)
{
  ശൂന്യം **pp = (ശൂന്യം **) p;
  സ്വതന്ത്രം (*pp);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  എങ്കിൽ (file)
    (ശൂന്യം) fclose (file);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_closep (ശൂന്യം *p)
{
  പൂർണ്ണസംഖ്യ *pp = (പൂർണ്ണസംഖ്യ *) p;
  എങ്കിൽ (*pp >= 0)
    close (*pp);
}

#define cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#define cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#define cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  എങ്കിൽ (*p)
    libcrun_context_free (*p);
}

#define cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  എങ്കിൽ (*p)
    libcrun_container_free (*p);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#endif
