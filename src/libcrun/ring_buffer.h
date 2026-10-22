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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "error.h"
#અન્તર્ભાવ "utils.h"

સંરચના ring_buffer;

size_t ring_buffer_get_data_available (સંરચના ring_buffer *rb);

size_t ring_buffer_get_space_available (સંરચના ring_buffer *rb);

size_t ring_buffer_get_size (સંરચના ring_buffer *rb);

પૂર્ણાંક ring_buffer_read (સંરચના ring_buffer *rb, પૂર્ણાંક fd, bool *is_eagain, libcrun_error_t *err);

પૂર્ણાંક ring_buffer_write (સંરચના ring_buffer *rb, પૂર્ણાંક fd, bool *is_eagain, libcrun_error_t *err);

સંરચના ring_buffer *ring_buffer_make (size_t size);

શૂન્ય ring_buffer_free (સંરચના ring_buffer *rb);

#define cleanup_ring_buffer __attribute__ ((cleanup (cleanup_ring_bufferp)))

સ્થિર અંતઃરેખ શૂન્ય
cleanup_ring_bufferp (સંરચના ring_buffer **p)
{
  સંરચના ring_buffer *rb = *p;
  જો (rb)
    ring_buffer_free (rb);
}

#endif
