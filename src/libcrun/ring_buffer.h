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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "error.h"
#അന്തര്ഭാവ "utils.h"

ഘടന ring_buffer;

size_t ring_buffer_get_data_available (ഘടന ring_buffer *rb);

size_t ring_buffer_get_space_available (ഘടന ring_buffer *rb);

size_t ring_buffer_get_size (ഘടന ring_buffer *rb);

പൂർണ്ണസംഖ്യ ring_buffer_read (ഘടന ring_buffer *rb, പൂർണ്ണസംഖ്യ fd, bool *is_eagain, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ ring_buffer_write (ഘടന ring_buffer *rb, പൂർണ്ണസംഖ്യ fd, bool *is_eagain, libcrun_error_t *err);

ഘടന ring_buffer *ring_buffer_make (size_t size);

ശൂന്യം ring_buffer_free (ഘടന ring_buffer *rb);

#define cleanup_ring_buffer __attribute__ ((cleanup (cleanup_ring_bufferp)))

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_ring_bufferp (ഘടന ring_buffer **p)
{
  ഘടന ring_buffer *rb = *p;
  എങ്കിൽ (rb)
    ring_buffer_free (rb);
}

#endif
