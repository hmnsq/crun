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
#నిర్వచితం_కాకపోతే CRUN_UTILS_H
#నిర్వచించు CRUN_UTILS_H

/* Self-contained helpers for the crun binary so that it does not have to
   include the internal libcrun/utils.h (which pulls in the internal,
   non-public headers).  The binary is a pure consumer of the public
   <libcrun.h> API; everything here is either libc or a trivial inline.  */

#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <unistd.h>

#నిర్వచితమైతే __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_వాడని __attribute__ ((వాడని))
#లేకపోతే
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_వాడని
#అయితే_ముగింపు

#నిర్వచించు OOM()                            \
  చేయి                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } అయితే (0)

#నిర్వచించు crun_make_error libcrun_make_error

__attribute__ ((జ్ఞాపకం_ఇవ్వు)) స్థిర అంతఃవరుస శూన్య *
xmalloc (size_t కొలత)
{
  శూన్య *res = జ్ఞాపకం_ఇవ్వు (కొలత);
  ఒకవేళ (UNLIKELY (res == ఖాళీ))
    OOM ();
  తిరిగి res;
}

__attribute__ ((జ్ఞాపకం_ఇవ్వు)) స్థిర అంతఃవరుస శూన్య *
xmalloc0 (size_t కొలత)
{
  శూన్య *res = సున్నా_జ్ఞాపకం_ఇవ్వు (1, కొలత);
  ఒకవేళ (UNLIKELY (res == ఖాళీ))
    OOM ();
  తిరిగి res;
}

__attribute__ ((జ్ఞాపకం_ఇవ్వు)) స్థిర అంతఃవరుస శూన్య *
xrealloc (శూన్య *ptr, size_t కొలత)
{
  శూన్య *res = realloc (ptr, కొలత);
  ఒకవేళ (UNLIKELY (res == ఖాళీ))
    OOM ();
  తిరిగి res;
}

స్థిర అంతఃవరుస అక్షరం *
xstrdup (స్థిరరాశి అక్షరం *str)
{
  అక్షరం *ret;
  ఒకవేళ (str == ఖాళీ)
    తిరిగి ఖాళీ;
  ret = strdup (str);
  ఒకవేళ (UNLIKELY (ret == ఖాళీ))
    OOM ();
  తిరిగి ret;
}

స్థిర అంతఃవరుస శూన్య
cleanup_freep (శూన్య *p)
{
  శూన్య **pp = (శూన్య **) p;
  విడుదల (*pp);
}

స్థిర అంతఃవరుస శూన్య
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  ఒకవేళ (file)
    (శూన్య) fclose (file);
}

స్థిర అంతఃవరుస శూన్య
cleanup_closep (శూన్య *p)
{
  పూర్ణాంకం *pp = (పూర్ణాంకం *) p;
  ఒకవేళ (*pp >= 0)
    close (*pp);
}

#నిర్వచించు cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#నిర్వచించు cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#నిర్వచించు cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
స్థిర అంతఃవరుస శూన్య
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  ఒకవేళ (*p)
    libcrun_context_free (*p);
}

#నిర్వచించు cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

స్థిర అంతఃవరుస శూన్య
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  ఒకవేళ (*p)
    libcrun_container_free (*p);
}

#నిర్వచించు cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#అయితే_ముగింపు
