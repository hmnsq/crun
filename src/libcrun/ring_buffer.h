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
#اگر_غیر_تعریف_شدہ RING_بفر_H
#تعریف_کرو RING_بفر_H

#شامل <config.h>

#شامل "error.h"
#شامل "utils.h"

ساخت ring_بفر;

size_t ring_بفر_get_ڈیٹا_دستیاب (ساخت ring_بفر *rb);

size_t ring_بفر_get_space_دستیاب (ساخت ring_بفر *rb);

size_t ring_بفر_get_سائز (ساخت ring_بفر *rb);

صحیح ring_بفر_read (ساخت ring_بفر *rb, صحیح fd, bool *is_eagain, libcrun_error_t *err);

صحیح ring_بفر_write (ساخت ring_بفر *rb, صحیح fd, bool *is_eagain, libcrun_error_t *err);

ساخت ring_بفر *ring_بفر_make (size_t سائز);

خالی ring_بفر_free (ساخت ring_بفر *rb);

#تعریف_کرو cleanup_ring_بفر __attribute__ ((cleanup (cleanup_ring_bufferp)))

ساکن اندرونی خالی
cleanup_ring_bufferp (ساخت ring_بفر **p)
{
  ساخت ring_بفر *rb = *p;
  اگر (rb)
    ring_بفر_free (rb);
}

#اگر_ختم
