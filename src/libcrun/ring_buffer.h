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
#నిర్వచితం_కాకపోతే RING_బఫరు_H
#నిర్వచించు RING_బఫరు_H

#అన్తర్భావ <config.h>

#అన్తర్భావ "error.h"
#అన్తర్భావ "utils.h"

నిర్మాణం ring_బఫరు;

size_t ring_బఫరు_get_దత్తాంశం_అందుబాటు (నిర్మాణం ring_బఫరు *rb);

size_t ring_బఫరు_get_space_అందుబాటు (నిర్మాణం ring_బఫరు *rb);

size_t ring_బఫరు_get_కొలత (నిర్మాణం ring_బఫరు *rb);

పూర్ణాంకం ring_బఫరు_read (నిర్మాణం ring_బఫరు *rb, పూర్ణాంకం fd, bool *is_eagain, libcrun_error_t *err);

పూర్ణాంకం ring_బఫరు_write (నిర్మాణం ring_బఫరు *rb, పూర్ణాంకం fd, bool *is_eagain, libcrun_error_t *err);

నిర్మాణం ring_బఫరు *ring_బఫరు_make (size_t కొలత);

శూన్య ring_బఫరు_free (నిర్మాణం ring_బఫరు *rb);

#నిర్వచించు cleanup_ring_బఫరు __attribute__ ((cleanup (cleanup_ring_bufferp)))

స్థిర అంతఃవరుస శూన్య
cleanup_ring_bufferp (నిర్మాణం ring_బఫరు **p)
{
  నిర్మాణం ring_బఫరు *rb = *p;
  ఒకవేళ (rb)
    ring_బఫరు_free (rb);
}

#అయితే_ముగింపు
