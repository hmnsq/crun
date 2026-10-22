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

#شامل <config.h>

#شامل "error.h"
#شامل "utils.h"

ساختار ring_buffer;

size_t ring_buffer_get_data_available (ساختار ring_buffer *rb);

size_t ring_buffer_get_space_available (ساختار ring_buffer *rb);

size_t ring_buffer_get_size (ساختار ring_buffer *rb);

صحیح ring_buffer_read (ساختار ring_buffer *rb, صحیح fd, bool *is_eagain, libcrun_error_t *err);

صحیح ring_buffer_write (ساختار ring_buffer *rb, صحیح fd, bool *is_eagain, libcrun_error_t *err);

ساختار ring_buffer *ring_buffer_make (size_t size);

تهی ring_buffer_free (ساختار ring_buffer *rb);

#define cleanup_ring_buffer __attribute__ ((cleanup (cleanup_ring_bufferp)))

ایستا درونخطی تهی
cleanup_ring_bufferp (ساختار ring_buffer **p)
{
  ساختار ring_buffer *rb = *p;
  اگر (rb)
    ring_buffer_free (rb);
}

#endif
