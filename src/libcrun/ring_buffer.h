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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ RING_ਬਫਰ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RING_ਬਫਰ_H

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

ਬਣਤਰ ring_ਬਫਰ;

size_t ring_ਬਫਰ_get_ਅੰਕੜੇ_ਉਪਲਬਧ (ਬਣਤਰ ring_ਬਫਰ *rb);

size_t ring_ਬਫਰ_get_space_ਉਪਲਬਧ (ਬਣਤਰ ring_ਬਫਰ *rb);

size_t ring_ਬਫਰ_get_ਮਾਪ (ਬਣਤਰ ring_ਬਫਰ *rb);

ਪੂਰਨ ring_ਬਫਰ_read (ਬਣਤਰ ring_ਬਫਰ *rb, ਪੂਰਨ fd, bool *is_eagain, libcrun_error_t *err);

ਪੂਰਨ ring_ਬਫਰ_write (ਬਣਤਰ ring_ਬਫਰ *rb, ਪੂਰਨ fd, bool *is_eagain, libcrun_error_t *err);

ਬਣਤਰ ring_ਬਫਰ *ring_ਬਫਰ_make (size_t ਮਾਪ);

ਖਾਲੀ ring_ਬਫਰ_free (ਬਣਤਰ ring_ਬਫਰ *rb);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_ring_ਬਫਰ __attribute__ ((cleanup (cleanup_ring_bufferp)))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_ring_bufferp (ਬਣਤਰ ring_ਬਫਰ **p)
{
  ਬਣਤਰ ring_ਬਫਰ *rb = *p;
  ਜੇ (rb)
    ring_ਬਫਰ_free (rb);
}

#ਜੇ_ਅੰਤ
