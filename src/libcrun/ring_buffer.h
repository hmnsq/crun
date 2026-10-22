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
#यदि_अपरिभाषितम् RING_बफर_H
#परिभाषय RING_बफर_H

#अन्तर्भाव <config.h>

#अन्तर्भाव "error.h"
#अन्तर्भाव "utils.h"

संरचना ring_बफर;

size_t ring_बफर_get_दत्त_उपलब्धम् (संरचना ring_बफर *rb);

size_t ring_बफर_get_space_उपलब्धम् (संरचना ring_बफर *rb);

size_t ring_बफर_get_आकार (संरचना ring_बफर *rb);

पूर्णाङ्क ring_बफर_read (संरचना ring_बफर *rb, पूर्णाङ्क fd, bool *is_eagain, libcrun_error_t *err);

पूर्णाङ्क ring_बफर_write (संरचना ring_बफर *rb, पूर्णाङ्क fd, bool *is_eagain, libcrun_error_t *err);

संरचना ring_बफर *ring_बफर_make (size_t आकार);

शून्य ring_बफर_free (संरचना ring_बफर *rb);

#परिभाषय cleanup_ring_बफर __attribute__ ((cleanup (cleanup_ring_bufferp)))

स्थिर अन्तःस्थ शून्य
cleanup_ring_bufferp (संरचना ring_बफर **p)
{
  संरचना ring_बफर *rb = *p;
  यदि (rb)
    ring_बफर_free (rb);
}

#यदि_अन्तः
