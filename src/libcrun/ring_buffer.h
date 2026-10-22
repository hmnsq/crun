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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ RING_ಬಫರ್_H
#ವ್ಯಾಖ್ಯಾನಿಸು RING_ಬಫರ್_H

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

ರಚನೆ ring_ಬಫರ್;

size_t ring_ಬಫರ್_get_ದತ್ತಾಂಶ_ಲಭ್ಯ (ರಚನೆ ring_ಬಫರ್ *rb);

size_t ring_ಬಫರ್_get_space_ಲಭ್ಯ (ರಚನೆ ring_ಬಫರ್ *rb);

size_t ring_ಬಫರ್_get_ಗಾತ್ರ (ರಚನೆ ring_ಬಫರ್ *rb);

ಪೂರ್ಣಾಂಕ ring_ಬಫರ್_read (ರಚನೆ ring_ಬಫರ್ *rb, ಪೂರ್ಣಾಂಕ fd, bool *is_eagain, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ ring_ಬಫರ್_write (ರಚನೆ ring_ಬಫರ್ *rb, ಪೂರ್ಣಾಂಕ fd, bool *is_eagain, libcrun_error_t *err);

ರಚನೆ ring_ಬಫರ್ *ring_ಬಫರ್_make (size_t ಗಾತ್ರ_2);

ಶೂನ್ಯ ring_ಬಫರ್_free (ರಚನೆ ring_ಬಫರ್ *rb);

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_ring_ಬಫರ್ __attribute__ ((cleanup (cleanup_ring_bufferp)))

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_ring_bufferp (ರಚನೆ ring_ಬಫರ್ **p)
{
  ರಚನೆ ring_ಬಫರ್ *rb = *p;
  ಆದರೆ (rb)
    ring_ಬಫರ್_free (rb);
}

#ಆದರೆ_ಅಂತ್ಯ
