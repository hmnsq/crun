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
#define _GNU_SOURCE
#ضمن <config.h>
#ضمن <sys/uio.h>

#ضمن "ring_buffer.h"
#ضمن "utils.h"

بنية ring_buffer
{
  حرف *buffer;
  size_t size;
  size_t head;
  size_t tail;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
ساكن صحيح
ring_buffer_get_read_iov (بنية ring_buffer *rb, بنية iovec *iov)
{
  صحيح iov_ژماردن = 0;

  /* Buffer is empty.  */
  إذا (rb->head == rb->tail)
    أرجع 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  إذا (rb->tail > rb->head)
    {
      iov[iov_ژماردن].iov_base = rb->buffer + rb->head;
      iov[iov_ژماردن].iov_len = rb->tail - rb->head;
      iov_ژماردن++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  وإلا
    {
      iov[iov_ژماردن].iov_base = rb->buffer + rb->head;
      iov[iov_ژماردن].iov_len = rb->size - rb->head;
      iov_ژماردن++;

      إذا (rb->tail > 0)
        {
          iov[iov_ژماردن].iov_base = rb->buffer;
          iov[iov_ژماردن].iov_len = rb->tail;
          iov_ژماردن++;
        }
    }
  أرجع iov_ژماردن;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
ساكن صحيح
ring_buffer_get_write_iov (بنية ring_buffer *rb, بنية iovec *iov)
{
  صحيح iov_ژماردن = 0;

  /* Buffer is full.  */
  إذا ((rb->tail + 1) % rb->size == rb->head)
    أرجع 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  إذا (rb->head > rb->tail + 1)
    {
      iov[iov_ژماردن].iov_base = rb->buffer + rb->tail;
      iov[iov_ژماردن].iov_len = rb->head - rb->tail - 1;
      iov_ژماردن++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  وإلا
    {
      iov[iov_ژماردن].iov_base = rb->buffer + rb->tail;
      iov[iov_ژماردن].iov_len = rb->head == 0 ? rb->size - rb->tail - 1 : rb->size - rb->tail;
      iov_ژماردن++;

      إذا (rb->head > 1)
        {
          iov[iov_ژماردن].iov_base = rb->buffer;
          iov[iov_ژماردن].iov_len = rb->head - 1;
          iov_ژماردن++;
        }
    }
  أرجع iov_ژماردن;
}

/* manually advance the head after a successful read.  */
ساكن فراغ
ring_buffer_advance_nocheck_head (بنية ring_buffer *rb, size_t amount)
{
  rb->head = (rb->head + amount) % rb->size;
}

/* manually advance the tail after a successful write.  */
ساكن فراغ
ring_buffer_advance_nocheck_tail (بنية ring_buffer *rb, size_t amount)
{
  rb->tail = (rb->tail + amount) % rb->size;
}

size_t
ring_buffer_get_data_available (بنية ring_buffer *rb)
{
  إذا (rb->head <= rb->tail)
    أرجع rb->tail - rb->head;

  أرجع rb->size - rb->head + rb->tail;
}

size_t
ring_buffer_get_size (بنية ring_buffer *rb)
{
  أرجع rb->size - 1;
}

size_t
ring_buffer_get_space_available (بنية ring_buffer *rb)
{
  أرجع rb->size - ring_buffer_get_data_available (rb) - 1;
}

صحيح
ring_buffer_read (بنية ring_buffer *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err)
{
  بنية iovec iov[2];
  صحيح iov_ژماردن = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_ژماردن = ring_buffer_get_write_iov (rb, iov);
  إذا (iov_ژماردن == 0)
    {
      *is_eagain = true;
      أرجع 0;
    }

  ret = readv (fd, iov, iov_ژماردن);
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
  ring_buffer_advance_nocheck_tail (rb, ret);
  أرجع ret;
}

صحيح
ring_buffer_write (بنية ring_buffer *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  بنية iovec iov[2];
  صحيح iov_ژماردن = 0;

  *is_eagain = false;

  iov_ژماردن = ring_buffer_get_read_iov (rb, iov);
  إذا (iov_ژماردن == 0)
    {
      *is_eagain = true;
      أرجع 0;
    }

  ret = writev (fd, iov, iov_ژماردن);
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
  ring_buffer_advance_nocheck_head (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  إذا (rb->head == rb->tail)
    {
      rb->head = 0;
      rb->tail = 0;
    }
  أرجع ret;
}

بنية ring_buffer *
ring_buffer_make (size_t size)
{
  بنية ring_buffer *rb = xmalloc (حجم (بنية ring_buffer));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->size = size + 1;
  rb->buffer = xmalloc (rb->size);
  rb->head = 0;
  rb->tail = 0;

  أرجع rb;
}

فراغ
ring_buffer_free (بنية ring_buffer *rb)
{
  إذا (rb == عدم)
    أرجع;
  حرر (rb->buffer);
  حرر (rb);
}
