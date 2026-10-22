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
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ
#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <sys/uio.h>

#ಅನ್ತರ್ಭಾವ "ring_buffer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

ರಚನೆ ring_ಬಫರ್
{
  ಅಕ್ಷರ *ಬಫರ್;
  size_t ಗಾತ್ರ_2;
  size_t ತಲೆ;
  size_t ಬಾಲ;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ring_ಬಫರ್_get_read_iov (ರಚನೆ ring_ಬಫರ್ *rb, ರಚನೆ iovec *iov)
{
  ಪೂರ್ಣಾಂಕ iov_ಎಣಿಕೆ = 0;

  /* Buffer is empty.  */
  ಆದರೆ (rb->ತಲೆ == rb->ಬಾಲ)
    ಹಿಂತಿರುಗಿ 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  ಆದರೆ (rb->ಬಾಲ > rb->ತಲೆ)
    {
      iov[iov_ಎಣಿಕೆ].iov_base = rb->ಬಫರ್ + rb->ತಲೆ;
      iov[iov_ಎಣಿಕೆ].iov_len = rb->ಬಾಲ - rb->ತಲೆ;
      iov_ಎಣಿಕೆ++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  ಇಲ್ಲದಿದ್ದರೆ
    {
      iov[iov_ಎಣಿಕೆ].iov_base = rb->ಬಫರ್ + rb->ತಲೆ;
      iov[iov_ಎಣಿಕೆ].iov_len = rb->ಗಾತ್ರ_2 - rb->ತಲೆ;
      iov_ಎಣಿಕೆ++;

      ಆದರೆ (rb->ಬಾಲ > 0)
        {
          iov[iov_ಎಣಿಕೆ].iov_base = rb->ಬಫರ್;
          iov[iov_ಎಣಿಕೆ].iov_len = rb->ಬಾಲ;
          iov_ಎಣಿಕೆ++;
        }
    }
  ಹಿಂತಿರುಗಿ iov_ಎಣಿಕೆ;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ring_ಬಫರ್_get_write_iov (ರಚನೆ ring_ಬಫರ್ *rb, ರಚನೆ iovec *iov)
{
  ಪೂರ್ಣಾಂಕ iov_ಎಣಿಕೆ = 0;

  /* Buffer is full.  */
  ಆದರೆ ((rb->ಬಾಲ + 1) % rb->ಗಾತ್ರ_2 == rb->ತಲೆ)
    ಹಿಂತಿರುಗಿ 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  ಆದರೆ (rb->ತಲೆ > rb->ಬಾಲ + 1)
    {
      iov[iov_ಎಣಿಕೆ].iov_base = rb->ಬಫರ್ + rb->ಬಾಲ;
      iov[iov_ಎಣಿಕೆ].iov_len = rb->ತಲೆ - rb->ಬಾಲ - 1;
      iov_ಎಣಿಕೆ++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  ಇಲ್ಲದಿದ್ದರೆ
    {
      iov[iov_ಎಣಿಕೆ].iov_base = rb->ಬಫರ್ + rb->ಬಾಲ;
      iov[iov_ಎಣಿಕೆ].iov_len = rb->ತಲೆ == 0 ? rb->ಗಾತ್ರ_2 - rb->ಬಾಲ - 1 : rb->ಗಾತ್ರ_2 - rb->ಬಾಲ;
      iov_ಎಣಿಕೆ++;

      ಆದರೆ (rb->ತಲೆ > 1)
        {
          iov[iov_ಎಣಿಕೆ].iov_base = rb->ಬಫರ್;
          iov[iov_ಎಣಿಕೆ].iov_len = rb->ತಲೆ - 1;
          iov_ಎಣಿಕೆ++;
        }
    }
  ಹಿಂತಿರುಗಿ iov_ಎಣಿಕೆ;
}

/* manually advance the head after a successful read.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ring_ಬಫರ್_advance_nocheck_ತಲೆ (ರಚನೆ ring_ಬಫರ್ *rb, size_t amount)
{
  rb->ತಲೆ = (rb->ತಲೆ + amount) % rb->ಗಾತ್ರ_2;
}

/* manually advance the tail after a successful write.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ring_ಬಫರ್_advance_nocheck_ಬಾಲ (ರಚನೆ ring_ಬಫರ್ *rb, size_t amount)
{
  rb->ಬಾಲ = (rb->ಬಾಲ + amount) % rb->ಗಾತ್ರ_2;
}

size_t
ring_ಬಫರ್_get_ದತ್ತಾಂಶ_ಲಭ್ಯ (ರಚನೆ ring_ಬಫರ್ *rb)
{
  ಆದರೆ (rb->ತಲೆ <= rb->ಬಾಲ)
    ಹಿಂತಿರುಗಿ rb->ಬಾಲ - rb->ತಲೆ;

  ಹಿಂತಿರುಗಿ rb->ಗಾತ್ರ_2 - rb->ತಲೆ + rb->ಬಾಲ;
}

size_t
ring_ಬಫರ್_get_ಗಾತ್ರ (ರಚನೆ ring_ಬಫರ್ *rb)
{
  ಹಿಂತಿರುಗಿ rb->ಗಾತ್ರ_2 - 1;
}

size_t
ring_ಬಫರ್_get_space_ಲಭ್ಯ (ರಚನೆ ring_ಬಫರ್ *rb)
{
  ಹಿಂತಿರುಗಿ rb->ಗಾತ್ರ_2 - ring_ಬಫರ್_get_ದತ್ತಾಂಶ_ಲಭ್ಯ (rb) - 1;
}

ಪೂರ್ಣಾಂಕ
ring_ಬಫರ್_read (ರಚನೆ ring_ಬಫರ್ *rb, ಪೂರ್ಣಾಂಕ fd, bool *is_eagain, libcrun_error_t *err)
{
  ರಚನೆ iovec iov[2];
  ಪೂರ್ಣಾಂಕ iov_ಎಣಿಕೆ = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_ಎಣಿಕೆ = ring_ಬಫರ್_get_write_iov (rb, iov);
  ಆದರೆ (iov_ಎಣಿಕೆ == 0)
    {
      *is_eagain = true;
      ಹಿಂತಿರುಗಿ 0;
    }

  ret = readv (fd, iov, iov_ಎಣಿಕೆ);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      ಆದರೆ (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ಹಿಂತಿರುಗಿ 0;
        }
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  ಆದರೆ (ret == 0)
    *is_eagain = true;
  ring_ಬಫರ್_advance_nocheck_ಬಾಲ (rb, ret);
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
ring_ಬಫರ್_write (ರಚನೆ ring_ಬಫರ್ *rb, ಪೂರ್ಣಾಂಕ fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  ರಚನೆ iovec iov[2];
  ಪೂರ್ಣಾಂಕ iov_ಎಣಿಕೆ = 0;

  *is_eagain = false;

  iov_ಎಣಿಕೆ = ring_ಬಫರ್_get_read_iov (rb, iov);
  ಆದರೆ (iov_ಎಣಿಕೆ == 0)
    {
      *is_eagain = true;
      ಹಿಂತಿರುಗಿ 0;
    }

  ret = writev (fd, iov, iov_ಎಣಿಕೆ);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      ಆದರೆ (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ಹಿಂತಿರುಗಿ 0;
        }
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "writev");
    }
  ring_ಬಫರ್_advance_nocheck_ತಲೆ (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  ಆದರೆ (rb->ತಲೆ == rb->ಬಾಲ)
    {
      rb->ತಲೆ = 0;
      rb->ಬಾಲ = 0;
    }
  ಹಿಂತಿರುಗಿ ret;
}

ರಚನೆ ring_ಬಫರ್ *
ring_ಬಫರ್_make (size_t ಗಾತ್ರ_2)
{
  ರಚನೆ ring_ಬಫರ್ *rb = xmalloc (ಗಾತ್ರ (ರಚನೆ ring_ಬಫರ್));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->ಗಾತ್ರ_2 = ಗಾತ್ರ_2 + 1;
  rb->ಬಫರ್ = xmalloc (rb->ಗಾತ್ರ_2);
  rb->ತಲೆ = 0;
  rb->ಬಾಲ = 0;

  ಹಿಂತಿರುಗಿ rb;
}

ಶೂನ್ಯ
ring_ಬಫರ್_free (ರಚನೆ ring_ಬಫರ್ *rb)
{
  ಆದರೆ (rb == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;
  ಬಿಡುಗಡೆ (rb->ಬಫರ್);
  ಬಿಡುಗಡೆ (rb);
}
