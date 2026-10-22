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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "error.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

ସଂରଚନା ring_buffer;

size_t ring_buffer_get_data_available (ସଂରଚନା ring_buffer *rb);

size_t ring_buffer_get_space_available (ସଂରଚନା ring_buffer *rb);

size_t ring_buffer_get_size (ସଂରଚନା ring_buffer *rb);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ring_buffer_read (ସଂରଚନା ring_buffer *rb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, bool *is_eagain, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ring_buffer_write (ସଂରଚନା ring_buffer *rb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, bool *is_eagain, libcrun_error_t *err);

ସଂରଚନା ring_buffer *ring_buffer_make (size_t size);

ଶୂନ୍ୟ ring_buffer_free (ସଂରଚନା ring_buffer *rb);

#define cleanup_ring_buffer __attribute__ ((cleanup (cleanup_ring_bufferp)))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_ring_bufferp (ସଂରଚନା ring_buffer **p)
{
  ସଂରଚନା ring_buffer *rb = *p;
  ଯଦି (rb)
    ring_buffer_free (rb);
}

#endif
