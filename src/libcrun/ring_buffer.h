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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "error.h"
#அந்தர்பாவ "utils.h"

கட்டமைப்பு ring_buffer;

size_t ring_buffer_get_data_available (கட்டமைப்பு ring_buffer *rb);

size_t ring_buffer_get_space_available (கட்டமைப்பு ring_buffer *rb);

size_t ring_buffer_get_size (கட்டமைப்பு ring_buffer *rb);

முழு ring_buffer_read (கட்டமைப்பு ring_buffer *rb, முழு fd, bool *is_eagain, libcrun_error_t *err);

முழு ring_buffer_write (கட்டமைப்பு ring_buffer *rb, முழு fd, bool *is_eagain, libcrun_error_t *err);

கட்டமைப்பு ring_buffer *ring_buffer_make (size_t size);

வெறுமை ring_buffer_free (கட்டமைப்பு ring_buffer *rb);

#define cleanup_ring_buffer __attribute__ ((cleanup (cleanup_ring_bufferp)))

நிலையான உள்வரி வெறுமை
cleanup_ring_bufferp (கட்டமைப்பு ring_buffer **p)
{
  கட்டமைப்பு ring_buffer *rb = *p;
  எனில் (rb)
    ring_buffer_free (rb);
}

#endif
