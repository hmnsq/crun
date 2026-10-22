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
#اگر_تعریف_نشده RING_میانگیر_H
#تعریف_کن RING_میانگیر_H

#ضمن <config.h>

#ضمن "error.h"
#ضمن "utils.h"

بنية ring_میانگیر;

size_t ring_میانگیر_get_داده_موجود (بنية ring_میانگیر *rb);

size_t ring_میانگیر_get_space_موجود (بنية ring_میانگیر *rb);

size_t ring_میانگیر_get_اندازه (بنية ring_میانگیر *rb);

صحيح ring_میانگیر_read (بنية ring_میانگیر *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err);

صحيح ring_میانگیر_write (بنية ring_میانگیر *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err);

بنية ring_میانگیر *ring_میانگیر_make (size_t اندازه);

فراغ ring_میانگیر_free (بنية ring_میانگیر *rb);

#تعریف_کن cleanup_ring_میانگیر __attribute__ ((cleanup (cleanup_ring_bufferp)))

ساكن مضمن فراغ
cleanup_ring_bufferp (بنية ring_میانگیر **p)
{
  بنية ring_میانگیر *rb = *p;
  إذا (rb)
    ring_میانگیر_free (rb);
}

#پایان_اگر
