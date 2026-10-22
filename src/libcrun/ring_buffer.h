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
#جے_بنا_تعریف RING_بفر_H
#تعریف_کرو RING_بفر_H

#ضمن <config.h>

#ضمن "error.h"
#ضمن "utils.h"

بنية ring_بفر;

size_t ring_بفر_get_انکڑے_دستیاب (بنية ring_بفر *rb);

size_t ring_بفر_get_space_دستیاب (بنية ring_بفر *rb);

size_t ring_بفر_get_ناپ (بنية ring_بفر *rb);

صحيح ring_بفر_read (بنية ring_بفر *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err);

صحيح ring_بفر_write (بنية ring_بفر *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err);

بنية ring_بفر *ring_بفر_make (size_t ناپ);

فراغ ring_بفر_free (بنية ring_بفر *rb);

#تعریف_کرو cleanup_ring_بفر __attribute__ ((cleanup (cleanup_ring_bufferp)))

ساكن مضمن فراغ
cleanup_ring_bufferp (بنية ring_بفر **p)
{
  بنية ring_بفر *rb = *p;
  إذا (rb)
    ring_بفر_free (rb);
}

#جے_انت
