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
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#अन्तर्भाव <config.h>

#अन्तर्भाव "error.h"
#अन्तर्भाव "utils.h"

संरचना ring_buffer;

size_t ring_buffer_get_data_available (संरचना ring_buffer *rb);

size_t ring_buffer_get_space_available (संरचना ring_buffer *rb);

size_t ring_buffer_get_size (संरचना ring_buffer *rb);

पूर्णाङ्क ring_buffer_read (संरचना ring_buffer *rb, पूर्णाङ्क fd, bool *is_eagain, libcrun_error_t *err);

पूर्णाङ्क ring_buffer_write (संरचना ring_buffer *rb, पूर्णाङ्क fd, bool *is_eagain, libcrun_error_t *err);

संरचना ring_buffer *ring_buffer_make (size_t size);

शून्य ring_buffer_free (संरचना ring_buffer *rb);

#define cleanup_ring_buffer __attribute__ ((cleanup (cleanup_ring_bufferp)))

स्थिर अन्तःस्थ शून्य
cleanup_ring_bufferp (संरचना ring_buffer **p)
{
  संरचना ring_buffer *rb = *p;
  यदि (rb)
    ring_buffer_free (rb);
}

#endif
