/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2024 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */
#अगर_अघोषित RING_बफर_H
#घोषणा RING_बफर_H

#समावेश <config.h>

#समावेश "error.h"
#समावेश "utils.h"

काष्ठा ring_बफर;

माप_प्रकार ring_बफर_get_तथ्य_available (काष्ठा ring_बफर *rb);

माप_प्रकार ring_बफर_get_space_available (काष्ठा ring_बफर *rb);

माप_प्रकार ring_बफर_get_आकार (काष्ठा ring_बफर *rb);

पूर्णांक ring_बफर_read (काष्ठा ring_बफर *rb, पूर्णांक fd, bool *is_eagain, libcrun_error_t *err);

पूर्णांक ring_बफर_write (काष्ठा ring_बफर *rb, पूर्णांक fd, bool *is_eagain, libcrun_error_t *err);

काष्ठा ring_बफर *ring_बफर_make (माप_प्रकार आकार);

व्योम ring_बफर_free (काष्ठा ring_बफर *rb);

#घोषणा cleanup_ring_बफर __attribute__ ((cleanup (cleanup_ring_bufferp)))

अटल अंतरभूत व्योम
cleanup_ring_bufferp (काष्ठा ring_बफर **p)
{
  काष्ठा ring_बफर *rb = *p;
  अगर (rb)
    ring_बफर_free (rb);
}

#पूर्ण_अगर
