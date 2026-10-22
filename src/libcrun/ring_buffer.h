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

#ضمن <config.h>

#ضمن "error.h"
#ضمن "utils.h"

بنية ring_buffer;

size_t ring_buffer_get_data_available (بنية ring_buffer *rb);

size_t ring_buffer_get_space_available (بنية ring_buffer *rb);

size_t ring_buffer_get_size (بنية ring_buffer *rb);

صحيح ring_buffer_read (بنية ring_buffer *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err);

صحيح ring_buffer_write (بنية ring_buffer *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err);

بنية ring_buffer *ring_buffer_make (size_t size);

فراغ ring_buffer_free (بنية ring_buffer *rb);

#define cleanup_ring_buffer __attribute__ ((cleanup (cleanup_ring_bufferp)))

ساكن مضمن فراغ
cleanup_ring_bufferp (بنية ring_buffer **p)
{
  بنية ring_buffer *rb = *p;
  𐤀𐤌 (rb)
    ring_buffer_free (rb);
}

#endif
