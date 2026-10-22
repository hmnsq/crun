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
#ifndef RING_বাফার_H
#define RING_বাফার_H

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "error.h"
#অন্তর্ভাব "utils.h"

কাঠামো ring_বাফার;

size_t ring_বাফার_get_তথ্য_available (কাঠামো ring_বাফার *rb);

size_t ring_বাফার_get_space_available (কাঠামো ring_বাফার *rb);

size_t ring_বাফার_get_আকার (কাঠামো ring_বাফার *rb);

পূর্ণসংখ্যা ring_বাফার_read (কাঠামো ring_বাফার *rb, পূর্ণসংখ্যা fd, bool *is_eagain, libcrun_error_t *err);

পূর্ণসংখ্যা ring_বাফার_write (কাঠামো ring_বাফার *rb, পূর্ণসংখ্যা fd, bool *is_eagain, libcrun_error_t *err);

কাঠামো ring_বাফার *ring_বাফার_make (size_t আকার_2);

শূন্য ring_বাফার_free (কাঠামো ring_বাফার *rb);

#define cleanup_ring_বাফার __attribute__ ((cleanup (cleanup_ring_bufferp)))

স্থির অন্তঃরেখ শূন্য
cleanup_ring_bufferp (কাঠামো ring_বাফার **p)
{
  কাঠামো ring_বাফার *rb = *p;
  যদি (rb)
    ring_বাফার_free (rb);
}

#endif
