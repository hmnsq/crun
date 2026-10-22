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
#تعریف_کن GNU_منبع
#ضمن <config.h>
#ضمن <sys/uio.h>

#ضمن "ring_buffer.h"
#ضمن "utils.h"

بنية ring_میانگیر
{
  حرف *میانگیر;
  size_t اندازه;
  size_t سر;
  size_t دم;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
ساكن صحيح
ring_میانگیر_get_read_iov (بنية ring_میانگیر *rb, بنية iovec *iov)
{
  صحيح iov_شمار = 0;

  /* Buffer is empty.  */
  إذا (rb->سر == rb->دم)
    أرجع 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  إذا (rb->دم > rb->سر)
    {
      iov[iov_شمار].iov_base = rb->میانگیر + rb->سر;
      iov[iov_شمار].iov_len = rb->دم - rb->سر;
      iov_شمار++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  وإلا
    {
      iov[iov_شمار].iov_base = rb->میانگیر + rb->سر;
      iov[iov_شمار].iov_len = rb->اندازه - rb->سر;
      iov_شمار++;

      إذا (rb->دم > 0)
        {
          iov[iov_شمار].iov_base = rb->میانگیر;
          iov[iov_شمار].iov_len = rb->دم;
          iov_شمار++;
        }
    }
  أرجع iov_شمار;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
ساكن صحيح
ring_میانگیر_get_write_iov (بنية ring_میانگیر *rb, بنية iovec *iov)
{
  صحيح iov_شمار = 0;

  /* Buffer is full.  */
  إذا ((rb->دم + 1) % rb->اندازه == rb->سر)
    أرجع 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  إذا (rb->سر > rb->دم + 1)
    {
      iov[iov_شمار].iov_base = rb->میانگیر + rb->دم;
      iov[iov_شمار].iov_len = rb->سر - rb->دم - 1;
      iov_شمار++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  وإلا
    {
      iov[iov_شمار].iov_base = rb->میانگیر + rb->دم;
      iov[iov_شمار].iov_len = rb->سر == 0 ? rb->اندازه - rb->دم - 1 : rb->اندازه - rb->دم;
      iov_شمار++;

      إذا (rb->سر > 1)
        {
          iov[iov_شمار].iov_base = rb->میانگیر;
          iov[iov_شمار].iov_len = rb->سر - 1;
          iov_شمار++;
        }
    }
  أرجع iov_شمار;
}

/* manually advance the head after a successful read.  */
ساكن فراغ
ring_میانگیر_advance_nocheck_سر (بنية ring_میانگیر *rb, size_t amount)
{
  rb->سر = (rb->سر + amount) % rb->اندازه;
}

/* manually advance the tail after a successful write.  */
ساكن فراغ
ring_میانگیر_advance_nocheck_دم (بنية ring_میانگیر *rb, size_t amount)
{
  rb->دم = (rb->دم + amount) % rb->اندازه;
}

size_t
ring_میانگیر_get_داده_موجود (بنية ring_میانگیر *rb)
{
  إذا (rb->سر <= rb->دم)
    أرجع rb->دم - rb->سر;

  أرجع rb->اندازه - rb->سر + rb->دم;
}

size_t
ring_میانگیر_get_اندازه (بنية ring_میانگیر *rb)
{
  أرجع rb->اندازه - 1;
}

size_t
ring_میانگیر_get_space_موجود (بنية ring_میانگیر *rb)
{
  أرجع rb->اندازه - ring_میانگیر_get_داده_موجود (rb) - 1;
}

صحيح
ring_میانگیر_read (بنية ring_میانگیر *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err)
{
  بنية iovec iov[2];
  صحيح iov_شمار = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_شمار = ring_میانگیر_get_write_iov (rb, iov);
  إذا (iov_شمار == 0)
    {
      *is_eagain = true;
      أرجع 0;
    }

  ret = readv (fd, iov, iov_شمار);
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
  ring_میانگیر_advance_nocheck_دم (rb, ret);
  أرجع ret;
}

صحيح
ring_میانگیر_write (بنية ring_میانگیر *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  بنية iovec iov[2];
  صحيح iov_شمار = 0;

  *is_eagain = false;

  iov_شمار = ring_میانگیر_get_read_iov (rb, iov);
  إذا (iov_شمار == 0)
    {
      *is_eagain = true;
      أرجع 0;
    }

  ret = writev (fd, iov, iov_شمار);
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
  ring_میانگیر_advance_nocheck_سر (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  إذا (rb->سر == rb->دم)
    {
      rb->سر = 0;
      rb->دم = 0;
    }
  أرجع ret;
}

بنية ring_میانگیر *
ring_میانگیر_make (size_t اندازه)
{
  بنية ring_میانگیر *rb = xmalloc (حجم (بنية ring_میانگیر));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->اندازه = اندازه + 1;
  rb->میانگیر = xmalloc (rb->اندازه);
  rb->سر = 0;
  rb->دم = 0;

  أرجع rb;
}

فراغ
ring_میانگیر_free (بنية ring_میانگیر *rb)
{
  إذا (rb == عدم)
    أرجع;
  حرر (rb->میانگیر);
  حرر (rb);
}
