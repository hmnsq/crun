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
#تعریف_کرو GNU_ماخذ
#شامل <config.h>
#شامل <sys/uio.h>

#شامل "ring_buffer.h"
#شامل "utils.h"

ساخت ring_بفر
{
  حرف *بفر;
  size_t سائز;
  size_t سر;
  size_t دم;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
ساکن صحیح
ring_بفر_get_read_iov (ساخت ring_بفر *rb, ساخت iovec *iov)
{
  صحیح iov_گنتی = 0;

  /* Buffer is empty.  */
  اگر (rb->سر == rb->دم)
    واپس 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  اگر (rb->دم > rb->سر)
    {
      iov[iov_گنتی].iov_base = rb->بفر + rb->سر;
      iov[iov_گنتی].iov_len = rb->دم - rb->سر;
      iov_گنتی++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  ورنہ
    {
      iov[iov_گنتی].iov_base = rb->بفر + rb->سر;
      iov[iov_گنتی].iov_len = rb->سائز - rb->سر;
      iov_گنتی++;

      اگر (rb->دم > 0)
        {
          iov[iov_گنتی].iov_base = rb->بفر;
          iov[iov_گنتی].iov_len = rb->دم;
          iov_گنتی++;
        }
    }
  واپس iov_گنتی;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
ساکن صحیح
ring_بفر_get_write_iov (ساخت ring_بفر *rb, ساخت iovec *iov)
{
  صحیح iov_گنتی = 0;

  /* Buffer is full.  */
  اگر ((rb->دم + 1) % rb->سائز == rb->سر)
    واپس 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  اگر (rb->سر > rb->دم + 1)
    {
      iov[iov_گنتی].iov_base = rb->بفر + rb->دم;
      iov[iov_گنتی].iov_len = rb->سر - rb->دم - 1;
      iov_گنتی++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  ورنہ
    {
      iov[iov_گنتی].iov_base = rb->بفر + rb->دم;
      iov[iov_گنتی].iov_len = rb->سر == 0 ? rb->سائز - rb->دم - 1 : rb->سائز - rb->دم;
      iov_گنتی++;

      اگر (rb->سر > 1)
        {
          iov[iov_گنتی].iov_base = rb->بفر;
          iov[iov_گنتی].iov_len = rb->سر - 1;
          iov_گنتی++;
        }
    }
  واپس iov_گنتی;
}

/* manually advance the head after a successful read.  */
ساکن خالی
ring_بفر_advance_nocheck_سر (ساخت ring_بفر *rb, size_t amount)
{
  rb->سر = (rb->سر + amount) % rb->سائز;
}

/* manually advance the tail after a successful write.  */
ساکن خالی
ring_بفر_advance_nocheck_دم (ساخت ring_بفر *rb, size_t amount)
{
  rb->دم = (rb->دم + amount) % rb->سائز;
}

size_t
ring_بفر_get_ڈیٹا_دستیاب (ساخت ring_بفر *rb)
{
  اگر (rb->سر <= rb->دم)
    واپس rb->دم - rb->سر;

  واپس rb->سائز - rb->سر + rb->دم;
}

size_t
ring_بفر_get_سائز (ساخت ring_بفر *rb)
{
  واپس rb->سائز - 1;
}

size_t
ring_بفر_get_space_دستیاب (ساخت ring_بفر *rb)
{
  واپس rb->سائز - ring_بفر_get_ڈیٹا_دستیاب (rb) - 1;
}

صحیح
ring_بفر_read (ساخت ring_بفر *rb, صحیح fd, bool *is_eagain, libcrun_error_t *err)
{
  ساخت iovec iov[2];
  صحیح iov_گنتی = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_گنتی = ring_بفر_get_write_iov (rb, iov);
  اگر (iov_گنتی == 0)
    {
      *is_eagain = true;
      واپس 0;
    }

  ret = readv (fd, iov, iov_گنتی);
  اگر (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      اگر (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          واپس 0;
        }
      واپس crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  اگر (ret == 0)
    *is_eagain = true;
  ring_بفر_advance_nocheck_دم (rb, ret);
  واپس ret;
}

صحیح
ring_بفر_write (ساخت ring_بفر *rb, صحیح fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  ساخت iovec iov[2];
  صحیح iov_گنتی = 0;

  *is_eagain = false;

  iov_گنتی = ring_بفر_get_read_iov (rb, iov);
  اگر (iov_گنتی == 0)
    {
      *is_eagain = true;
      واپس 0;
    }

  ret = writev (fd, iov, iov_گنتی);
  اگر (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      اگر (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          واپس 0;
        }
      واپس crun_make_error (err, errno, "writev");
    }
  ring_بفر_advance_nocheck_سر (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  اگر (rb->سر == rb->دم)
    {
      rb->سر = 0;
      rb->دم = 0;
    }
  واپس ret;
}

ساخت ring_بفر *
ring_بفر_make (size_t سائز)
{
  ساخت ring_بفر *rb = xmalloc (حجم (ساخت ring_بفر));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->سائز = سائز + 1;
  rb->بفر = xmalloc (rb->سائز);
  rb->سر = 0;
  rb->دم = 0;

  واپس rb;
}

خالی
ring_بفر_free (ساخت ring_بفر *rb)
{
  اگر (rb == تہی)
    واپس;
  آزاد (rb->بفر);
  آزاد (rb);
}
