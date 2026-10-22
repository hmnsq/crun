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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/uio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ring_buffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

ਬਣਤਰ ring_ਬਫਰ
{
  ਅੱਖਰ *ਬਫਰ;
  size_t ਮਾਪ;
  size_t ਸਿਰ;
  size_t ਪੂਛ;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
ਸਥਿਰ ਪੂਰਨ
ring_ਬਫਰ_get_read_iov (ਬਣਤਰ ring_ਬਫਰ *rb, ਬਣਤਰ iovec *iov)
{
  ਪੂਰਨ iov_ਗਿਣਤੀ = 0;

  /* Buffer is empty.  */
  ਜੇ (rb->ਸਿਰ == rb->ਪੂਛ)
    ਵਾਪਸ 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  ਜੇ (rb->ਪੂਛ > rb->ਸਿਰ)
    {
      iov[iov_ਗਿਣਤੀ].iov_base = rb->ਬਫਰ + rb->ਸਿਰ;
      iov[iov_ਗਿਣਤੀ].iov_len = rb->ਪੂਛ - rb->ਸਿਰ;
      iov_ਗਿਣਤੀ++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  ਨਹੀਂਤਾਂ
    {
      iov[iov_ਗਿਣਤੀ].iov_base = rb->ਬਫਰ + rb->ਸਿਰ;
      iov[iov_ਗਿਣਤੀ].iov_len = rb->ਮਾਪ - rb->ਸਿਰ;
      iov_ਗਿਣਤੀ++;

      ਜੇ (rb->ਪੂਛ > 0)
        {
          iov[iov_ਗਿਣਤੀ].iov_base = rb->ਬਫਰ;
          iov[iov_ਗਿਣਤੀ].iov_len = rb->ਪੂਛ;
          iov_ਗਿਣਤੀ++;
        }
    }
  ਵਾਪਸ iov_ਗਿਣਤੀ;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
ਸਥਿਰ ਪੂਰਨ
ring_ਬਫਰ_get_write_iov (ਬਣਤਰ ring_ਬਫਰ *rb, ਬਣਤਰ iovec *iov)
{
  ਪੂਰਨ iov_ਗਿਣਤੀ = 0;

  /* Buffer is full.  */
  ਜੇ ((rb->ਪੂਛ + 1) % rb->ਮਾਪ == rb->ਸਿਰ)
    ਵਾਪਸ 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  ਜੇ (rb->ਸਿਰ > rb->ਪੂਛ + 1)
    {
      iov[iov_ਗਿਣਤੀ].iov_base = rb->ਬਫਰ + rb->ਪੂਛ;
      iov[iov_ਗਿਣਤੀ].iov_len = rb->ਸਿਰ - rb->ਪੂਛ - 1;
      iov_ਗਿਣਤੀ++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  ਨਹੀਂਤਾਂ
    {
      iov[iov_ਗਿਣਤੀ].iov_base = rb->ਬਫਰ + rb->ਪੂਛ;
      iov[iov_ਗਿਣਤੀ].iov_len = rb->ਸਿਰ == 0 ? rb->ਮਾਪ - rb->ਪੂਛ - 1 : rb->ਮਾਪ - rb->ਪੂਛ;
      iov_ਗਿਣਤੀ++;

      ਜੇ (rb->ਸਿਰ > 1)
        {
          iov[iov_ਗਿਣਤੀ].iov_base = rb->ਬਫਰ;
          iov[iov_ਗਿਣਤੀ].iov_len = rb->ਸਿਰ - 1;
          iov_ਗਿਣਤੀ++;
        }
    }
  ਵਾਪਸ iov_ਗਿਣਤੀ;
}

/* manually advance the head after a successful read.  */
ਸਥਿਰ ਖਾਲੀ
ring_ਬਫਰ_advance_nocheck_ਸਿਰ (ਬਣਤਰ ring_ਬਫਰ *rb, size_t amount)
{
  rb->ਸਿਰ = (rb->ਸਿਰ + amount) % rb->ਮਾਪ;
}

/* manually advance the tail after a successful write.  */
ਸਥਿਰ ਖਾਲੀ
ring_ਬਫਰ_advance_nocheck_ਪੂਛ (ਬਣਤਰ ring_ਬਫਰ *rb, size_t amount)
{
  rb->ਪੂਛ = (rb->ਪੂਛ + amount) % rb->ਮਾਪ;
}

size_t
ring_ਬਫਰ_get_ਅੰਕੜੇ_ਉਪਲਬਧ (ਬਣਤਰ ring_ਬਫਰ *rb)
{
  ਜੇ (rb->ਸਿਰ <= rb->ਪੂਛ)
    ਵਾਪਸ rb->ਪੂਛ - rb->ਸਿਰ;

  ਵਾਪਸ rb->ਮਾਪ - rb->ਸਿਰ + rb->ਪੂਛ;
}

size_t
ring_ਬਫਰ_get_ਮਾਪ (ਬਣਤਰ ring_ਬਫਰ *rb)
{
  ਵਾਪਸ rb->ਮਾਪ - 1;
}

size_t
ring_ਬਫਰ_get_space_ਉਪਲਬਧ (ਬਣਤਰ ring_ਬਫਰ *rb)
{
  ਵਾਪਸ rb->ਮਾਪ - ring_ਬਫਰ_get_ਅੰਕੜੇ_ਉਪਲਬਧ (rb) - 1;
}

ਪੂਰਨ
ring_ਬਫਰ_read (ਬਣਤਰ ring_ਬਫਰ *rb, ਪੂਰਨ fd, bool *is_eagain, libcrun_error_t *err)
{
  ਬਣਤਰ iovec iov[2];
  ਪੂਰਨ iov_ਗਿਣਤੀ = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_ਗਿਣਤੀ = ring_ਬਫਰ_get_write_iov (rb, iov);
  ਜੇ (iov_ਗਿਣਤੀ == 0)
    {
      *is_eagain = true;
      ਵਾਪਸ 0;
    }

  ret = readv (fd, iov, iov_ਗਿਣਤੀ);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      ਜੇ (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ਵਾਪਸ 0;
        }
      ਵਾਪਸ crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  ਜੇ (ret == 0)
    *is_eagain = true;
  ring_ਬਫਰ_advance_nocheck_ਪੂਛ (rb, ret);
  ਵਾਪਸ ret;
}

ਪੂਰਨ
ring_ਬਫਰ_write (ਬਣਤਰ ring_ਬਫਰ *rb, ਪੂਰਨ fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  ਬਣਤਰ iovec iov[2];
  ਪੂਰਨ iov_ਗਿਣਤੀ = 0;

  *is_eagain = false;

  iov_ਗਿਣਤੀ = ring_ਬਫਰ_get_read_iov (rb, iov);
  ਜੇ (iov_ਗਿਣਤੀ == 0)
    {
      *is_eagain = true;
      ਵਾਪਸ 0;
    }

  ret = writev (fd, iov, iov_ਗਿਣਤੀ);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      ਜੇ (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ਵਾਪਸ 0;
        }
      ਵਾਪਸ crun_make_error (err, errno, "writev");
    }
  ring_ਬਫਰ_advance_nocheck_ਸਿਰ (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  ਜੇ (rb->ਸਿਰ == rb->ਪੂਛ)
    {
      rb->ਸਿਰ = 0;
      rb->ਪੂਛ = 0;
    }
  ਵਾਪਸ ret;
}

ਬਣਤਰ ring_ਬਫਰ *
ring_ਬਫਰ_make (size_t ਮਾਪ)
{
  ਬਣਤਰ ring_ਬਫਰ *rb = xmalloc (ਆਕਾਰ (ਬਣਤਰ ring_ਬਫਰ));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->ਮਾਪ = ਮਾਪ + 1;
  rb->ਬਫਰ = xmalloc (rb->ਮਾਪ);
  rb->ਸਿਰ = 0;
  rb->ਪੂਛ = 0;

  ਵਾਪਸ rb;
}

ਖਾਲੀ
ring_ਬਫਰ_free (ਬਣਤਰ ring_ਬਫਰ *rb)
{
  ਜੇ (rb == ਨਿਰਾ)
    ਵਾਪਸ;
  ਮੁਕਤ (rb->ਬਫਰ);
  ਮੁਕਤ (rb);
}
