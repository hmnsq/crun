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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CRUN_UTILS_H
#ವ್ಯಾಖ್ಯಾನಿಸು CRUN_UTILS_H

/* Self-contained helpers for the crun binary so that it does not have to
   include the internal libcrun/utils.h (which pulls in the internal,
   non-public headers).  The binary is a pure consumer of the public
   <libcrun.h> API; everything here is either libc or a trivial inline.  */

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_ಬಳಸದ __attribute__ ((ಬಳಸದ))
#ಇಲ್ಲದಿದ್ದರೆ
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_ಬಳಸದ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು OOM()                            \
  ಮಾಡು                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು crun_make_error libcrun_make_error

__attribute__ ((ಸ್ಮರಣೆ_ಕೊಡು)) ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ *
xmalloc (size_t ಗಾತ್ರ_2)
{
  ಶೂನ್ಯ *res = ಸ್ಮರಣೆ_ಕೊಡು (ಗಾತ್ರ_2);
  ಆದರೆ (UNLIKELY (res == ಖಾಲಿ))
    OOM ();
  ಹಿಂತಿರುಗಿ res;
}

__attribute__ ((ಸ್ಮರಣೆ_ಕೊಡು)) ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ *
xmalloc0 (size_t ಗಾತ್ರ_2)
{
  ಶೂನ್ಯ *res = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು (1, ಗಾತ್ರ_2);
  ಆದರೆ (UNLIKELY (res == ಖಾಲಿ))
    OOM ();
  ಹಿಂತಿರುಗಿ res;
}

__attribute__ ((ಸ್ಮರಣೆ_ಕೊಡು)) ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ *
xrealloc (ಶೂನ್ಯ *ptr, size_t ಗಾತ್ರ_2)
{
  ಶೂನ್ಯ *res = realloc (ptr, ಗಾತ್ರ_2);
  ಆದರೆ (UNLIKELY (res == ಖಾಲಿ))
    OOM ();
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ *
xstrdup (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
  ಅಕ್ಷರ *ret;
  ಆದರೆ (str == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  ret = strdup (str);
  ಆದರೆ (UNLIKELY (ret == ಖಾಲಿ))
    OOM ();
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_freep (ಶೂನ್ಯ *p)
{
  ಶೂನ್ಯ **pp = (ಶೂನ್ಯ **) p;
  ಬಿಡುಗಡೆ (*pp);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  ಆದರೆ (file)
    (ಶೂನ್ಯ) fclose (file);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_closep (ಶೂನ್ಯ *p)
{
  ಪೂರ್ಣಾಂಕ *pp = (ಪೂರ್ಣಾಂಕ *) p;
  ಆದರೆ (*pp >= 0)
    close (*pp);
}

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  ಆದರೆ (*p)
    libcrun_context_free (*p);
}

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  ಆದರೆ (*p)
    libcrun_container_free (*p);
}

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#ಆದರೆ_ಅಂತ್ಯ
