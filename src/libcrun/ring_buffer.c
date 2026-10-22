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
#تعریف_کرو GNU_سروت
#ضمن <config.h>
#ضمن <sys/uio.h>

#ضمن "ring_buffer.h"
#ضمن "utils.h"

بنية ring_بفر
{
  حرف *بفر;
  size_t ناپ;
  size_t سر;
  size_t پوچھ;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
ساكن صحيح
ring_بفر_get_read_iov (بنية ring_بفر *rb, بنية iovec *iov)
{
  صحيح iov_گنتی = 0;

  /* Buffer is empty.  */
  إذا (rb->سر == rb->پوچھ)
    أرجع 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  إذا (rb->پوچھ > rb->سر)
    {
      iov[iov_گنتی].iov_base = rb->بفر + rb->سر;
      iov[iov_گنتی].iov_len = rb->پوچھ - rb->سر;
      iov_گنتی++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  وإلا
    {
      iov[iov_گنتی].iov_base = rb->بفر + rb->سر;
      iov[iov_گنتی].iov_len = rb->ناپ - rb->سر;
      iov_گنتی++;

      إذا (rb->پوچھ > 0)
        {
          iov[iov_گنتی].iov_base = rb->بفر;
          iov[iov_گنتی].iov_len = rb->پوچھ;
          iov_گنتی++;
        }
    }
  أرجع iov_گنتی;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
ساكن صحيح
ring_بفر_get_write_iov (بنية ring_بفر *rb, بنية iovec *iov)
{
  صحيح iov_گنتی = 0;

  /* Buffer is full.  */
  إذا ((rb->پوچھ + 1) % rb->ناپ == rb->سر)
    أرجع 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  إذا (rb->سر > rb->پوچھ + 1)
    {
      iov[iov_گنتی].iov_base = rb->بفر + rb->پوچھ;
      iov[iov_گنتی].iov_len = rb->سر - rb->پوچھ - 1;
      iov_گنتی++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  وإلا
    {
      iov[iov_گنتی].iov_base = rb->بفر + rb->پوچھ;
      iov[iov_گنتی].iov_len = rb->سر == 0 ? rb->ناپ - rb->پوچھ - 1 : rb->ناپ - rb->پوچھ;
      iov_گنتی++;

      إذا (rb->سر > 1)
        {
          iov[iov_گنتی].iov_base = rb->بفر;
          iov[iov_گنتی].iov_len = rb->سر - 1;
          iov_گنتی++;
        }
    }
  أرجع iov_گنتی;
}

/* manually advance the head after a successful read.  */
ساكن فراغ
ring_بفر_advance_nocheck_سر (بنية ring_بفر *rb, size_t amount)
{
  rb->سر = (rb->سر + amount) % rb->ناپ;
}

/* manually advance the tail after a successful write.  */
ساكن فراغ
ring_بفر_advance_nocheck_پوچھ (بنية ring_بفر *rb, size_t amount)
{
  rb->پوچھ = (rb->پوچھ + amount) % rb->ناپ;
}

size_t
ring_بفر_get_انکڑے_دستیاب (بنية ring_بفر *rb)
{
  إذا (rb->سر <= rb->پوچھ)
    أرجع rb->پوچھ - rb->سر;

  أرجع rb->ناپ - rb->سر + rb->پوچھ;
}

size_t
ring_بفر_get_ناپ (بنية ring_بفر *rb)
{
  أرجع rb->ناپ - 1;
}

size_t
ring_بفر_get_space_دستیاب (بنية ring_بفر *rb)
{
  أرجع rb->ناپ - ring_بفر_get_انکڑے_دستیاب (rb) - 1;
}

صحيح
ring_بفر_read (بنية ring_بفر *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err)
{
  بنية iovec iov[2];
  صحيح iov_گنتی = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_گنتی = ring_بفر_get_write_iov (rb, iov);
  إذا (iov_گنتی == 0)
    {
      *is_eagain = true;
      أرجع 0;
    }

  ret = readv (fd, iov, iov_گنتی);
  إذا (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      إذا (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          أرجع 0;
        }
      أرجع crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  إذا (ret == 0)
    *is_eagain = true;
  ring_بفر_advance_nocheck_پوچھ (rb, ret);
  أرجع ret;
}

صحيح
ring_بفر_write (بنية ring_بفر *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  بنية iovec iov[2];
  صحيح iov_گنتی = 0;

  *is_eagain = false;

  iov_گنتی = ring_بفر_get_read_iov (rb, iov);
  إذا (iov_گنتی == 0)
    {
      *is_eagain = true;
      أرجع 0;
    }

  ret = writev (fd, iov, iov_گنتی);
  إذا (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      إذا (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          أرجع 0;
        }
      أرجع crun_make_error (err, errno, "writev");
    }
  ring_بفر_advance_nocheck_سر (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  إذا (rb->سر == rb->پوچھ)
    {
      rb->سر = 0;
      rb->پوچھ = 0;
    }
  أرجع ret;
}

بنية ring_بفر *
ring_بفر_make (size_t ناپ)
{
  بنية ring_بفر *rb = xmalloc (حجم (بنية ring_بفر));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->ناپ = ناپ + 1;
  rb->بفر = xmalloc (rb->ناپ);
  rb->سر = 0;
  rb->پوچھ = 0;

  أرجع rb;
}

فراغ
ring_بفر_free (بنية ring_بفر *rb)
{
  إذا (rb == عدم)
    أرجع;
  حرر (rb->بفر);
  حرر (rb);
}
