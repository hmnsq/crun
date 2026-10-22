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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

ಸಂರಚನಾ ring_buffer;

size_t ring_buffer_get_data_available (ಸಂರಚನಾ ring_buffer *rb);

size_t ring_buffer_get_space_available (ಸಂರಚನಾ ring_buffer *rb);

size_t ring_buffer_get_size (ಸಂರಚನಾ ring_buffer *rb);

ಪೂರ್ಣಾಙ್ಕ ring_buffer_read (ಸಂರಚನಾ ring_buffer *rb, ಪೂರ್ಣಾಙ್ಕ fd, bool *is_eagain, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ ring_buffer_write (ಸಂರಚನಾ ring_buffer *rb, ಪೂರ್ಣಾಙ್ಕ fd, bool *is_eagain, libcrun_error_t *err);

ಸಂರಚನಾ ring_buffer *ring_buffer_make (size_t size);

ಶೂನ್ಯ ring_buffer_free (ಸಂರಚನಾ ring_buffer *rb);

#define cleanup_ring_buffer __attribute__ ((cleanup (cleanup_ring_bufferp)))

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_ring_bufferp (ಸಂರಚನಾ ring_buffer **p)
{
  ಸಂರಚನಾ ring_buffer *rb = *p;
  ಯದಿ (rb)
    ring_buffer_free (rb);
}

#endif
