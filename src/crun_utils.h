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
#यदि_अपरिभाषितम् CRUN_UTILS_H
#परिभाषय CRUN_UTILS_H

/* Self-contained helpers for the crun binary so that it does not have to
   include the internal libcrun/utils.h (which pulls in the internal,
   non-public headers).  The binary is a pure consumer of the public
   <libcrun.h> API; everything here is either libc or a trivial inline.  */

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>

#यदि_परिभाषितम् __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_अप्रयुक्तम् __attribute__ ((अप्रयुक्तम्))
#अन्यथा
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_अप्रयुक्तम्
#यदि_अन्तः

#परिभाषय OOM()                            \
  कुरु                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _exit (EXIT_FAILURE);              \
  } यावत् (0)

#परिभाषय crun_make_error libcrun_make_error

__attribute__ ((स्मृतिं_देहि)) स्थिर अन्तःस्थ शून्य *
xmalloc (size_t आकार)
{
  शून्य *res = स्मृतिं_देहि (आकार);
  यदि (UNLIKELY (res == रिक्त))
    OOM ();
  प्रत्यागम res;
}

__attribute__ ((स्मृतिं_देहि)) स्थिर अन्तःस्थ शून्य *
xmalloc0 (size_t आकार)
{
  शून्य *res = शून्यस्मृतिं_देहि (1, आकार);
  यदि (UNLIKELY (res == रिक्त))
    OOM ();
  प्रत्यागम res;
}

__attribute__ ((स्मृतिं_देहि)) स्थिर अन्तःस्थ शून्य *
xrealloc (शून्य *ptr, size_t आकार)
{
  शून्य *res = realloc (ptr, आकार);
  यदि (UNLIKELY (res == रिक्त))
    OOM ();
  प्रत्यागम res;
}

स्थिर अन्तःस्थ वर्ण *
xstrdup (ध्रुव वर्ण *str)
{
  वर्ण *ret;
  यदि (str == रिक्त)
    प्रत्यागम रिक्त;
  ret = strdup (str);
  यदि (UNLIKELY (ret == रिक्त))
    OOM ();
  प्रत्यागम ret;
}

स्थिर अन्तःस्थ शून्य
cleanup_freep (शून्य *p)
{
  शून्य **pp = (शून्य **) p;
  मुक्त (*pp);
}

स्थिर अन्तःस्थ शून्य
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  यदि (file)
    (शून्य) fclose (file);
}

स्थिर अन्तःस्थ शून्य
cleanup_closep (शून्य *p)
{
  पूर्णाङ्क *pp = (पूर्णाङ्क *) p;
  यदि (*pp >= 0)
    close (*pp);
}

#परिभाषय cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#परिभाषय cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#परिभाषय cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
स्थिर अन्तःस्थ शून्य
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  यदि (*p)
    libcrun_context_free (*p);
}

#परिभाषय cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

स्थिर अन्तःस्थ शून्य
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  यदि (*p)
    libcrun_container_free (*p);
}

#परिभाषय cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#यदि_अन्तः
