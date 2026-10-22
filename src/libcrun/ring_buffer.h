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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ "error.h"
#অন্তর্ভাৱ "utils.h"

সংরচনা ring_buffer;

size_t ring_buffer_get_data_available (সংরচনা ring_buffer *rb);

size_t ring_buffer_get_space_available (সংরচনা ring_buffer *rb);

size_t ring_buffer_get_size (সংরচনা ring_buffer *rb);

পূর্ণাঙ্ক ring_buffer_read (সংরচনা ring_buffer *rb, পূর্ণাঙ্ক fd, bool *is_eagain, libcrun_error_t *err);

পূর্ণাঙ্ক ring_buffer_write (সংরচনা ring_buffer *rb, পূর্ণাঙ্ক fd, bool *is_eagain, libcrun_error_t *err);

সংরচনা ring_buffer *ring_buffer_make (size_t size);

শূন্য ring_buffer_free (সংরচনা ring_buffer *rb);

#define cleanup_ring_buffer __attribute__ ((cleanup (cleanup_ring_bufferp)))

স্থির অন্তঃস্থ শূন্য
cleanup_ring_bufferp (সংরচনা ring_buffer **p)
{
  সংরচনা ring_buffer *rb = *p;
  যদি (rb)
    ring_buffer_free (rb);
}

#endif
