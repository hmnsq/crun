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
#अगर_अघोषित CRUN_UTILS_H
#घोषणा CRUN_UTILS_H

/* Self-contained helpers for the crun binary so that it does not have to
   include the internal libcrun/utils.h (which pulls in the internal,
   non-public headers).  The binary is a pure consumer of the public
   <libcrun.h> API; everything here is either libc or a trivial inline.  */

#समावेश <stdlib.h>
#समावेश <stdio.h>
#समावेश <string.h>
#समावेश <unistd.h>

#अगर_घोषित __GNUC__
#  define LIKELY(x) __builtin_expect ((x), 1)
#  define UNLIKELY(x) __builtin_expect ((x), 0)
#  define arg_unused __attribute__ ((unused))
#अन्यथा
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#  define arg_unused
#पूर्ण_अगर

#घोषणा OOM()                            \
  करो                                     \
    {                                    \
      ख_लिखो (मानक_त्रुटि, "out of memory"); \
      _निकास (निकास_त्रुटि);              \
  } जबतक (0)

#घोषणा crun_make_error libcrun_make_error

__attribute__ ((दो_स्मृति)) अटल अंतरभूत व्योम *
xmalloc (माप_प्रकार आकार)
{
  व्योम *res = दो_स्मृति (आकार);
  अगर (UNLIKELY (res == शून्य))
    OOM ();
  वापस res;
}

__attribute__ ((दो_स्मृति)) अटल अंतरभूत व्योम *
xmalloc0 (माप_प्रकार आकार)
{
  व्योम *res = सुस्मृति (1, आकार);
  अगर (UNLIKELY (res == शून्य))
    OOM ();
  वापस res;
}

__attribute__ ((दो_स्मृति)) अटल अंतरभूत व्योम *
xrealloc (व्योम *ptr, माप_प्रकार आकार)
{
  व्योम *res = पुनः_स्मृति (ptr, आकार);
  अगर (UNLIKELY (res == शून्य))
    OOM ();
  वापस res;
}

अटल अंतरभूत अक्षर *
xstrdup (स्थिर अक्षर *str)
{
  अक्षर *ret;
  अगर (str == शून्य)
    वापस शून्य;
  ret = strdup (str);
  अगर (UNLIKELY (ret == शून्य))
    OOM ();
  वापस ret;
}

अटल अंतरभूत व्योम
cleanup_freep (व्योम *p)
{
  व्योम **pp = (व्योम **) p;
  मुक्त (*pp);
}

अटल अंतरभूत व्योम
cleanup_filep (खाता **f)
{
  खाता *file = *f;
  अगर (file)
    (व्योम) ख_बंद (file);
}

अटल अंतरभूत व्योम
cleanup_closep (व्योम *p)
{
  पूर्णांक *pp = (पूर्णांक *) p;
  अगर (*pp >= 0)
    बंद (*pp);
}

#घोषणा cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#घोषणा cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#घोषणा cleanup_close __attribute__ ((cleanup (cleanup_closep)))

/* Relies on <libcrun.h> (included before this header via crun.h) for the
   opaque libcrun_context_t and libcrun_context_free.  */
अटल अंतरभूत व्योम
cleanup_libcrun_contextp (libcrun_context_t **p)
{
  अगर (*p)
    libcrun_context_free (*p);
}

#घोषणा cleanup_context __attribute__ ((cleanup (cleanup_libcrun_contextp)))

अटल अंतरभूत व्योम
cleanup_libcrun_containerp (libcrun_container_t **p)
{
  अगर (*p)
    libcrun_container_free (*p);
}

#घोषणा cleanup_container __attribute__ ((cleanup (cleanup_libcrun_containerp)))

#पूर्ण_अगर
