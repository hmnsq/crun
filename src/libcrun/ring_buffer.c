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
  صحيح iov_count = 0;

  /* Buffer is empty.  */
  אם (rb->head == rb->tail)
    החזר 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  אם (rb->tail > rb->head)
    {
      iov[iov_count].iov_base = rb->buffer + rb->head;
      iov[iov_count].iov_len = rb->tail - rb->head;
      iov_count++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  אחרת
    {
      iov[iov_count].iov_base = rb->buffer + rb->head;
      iov[iov_count].iov_len = rb->size - rb->head;
      iov_count++;

      אם (rb->tail > 0)
        {
          iov[iov_count].iov_base = rb->buffer;
          iov[iov_count].iov_len = rb->tail;
          iov_count++;
        }
    }
  החזר iov_count;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
ساكن صحيح
ring_buffer_get_write_iov (بنية ring_buffer *rb, بنية iovec *iov)
{
  صحيح iov_count = 0;

  /* Buffer is full.  */
  אם ((rb->tail + 1) % rb->size == rb->head)
    החזר 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  אם (rb->head > rb->tail + 1)
    {
      iov[iov_count].iov_base = rb->buffer + rb->tail;
      iov[iov_count].iov_len = rb->head - rb->tail - 1;
      iov_count++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  אחרת
    {
      iov[iov_count].iov_base = rb->buffer + rb->tail;
      iov[iov_count].iov_len = rb->head == 0 ? rb->size - rb->tail - 1 : rb->size - rb->tail;
      iov_count++;

      אם (rb->head > 1)
        {
          iov[iov_count].iov_base = rb->buffer;
          iov[iov_count].iov_len = rb->head - 1;
          iov_count++;
        }
    }
  החזר iov_count;
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
  אם (rb->head <= rb->tail)
    החזר rb->tail - rb->head;

  החזר rb->size - rb->head + rb->tail;
}

size_t
ring_buffer_get_size (بنية ring_buffer *rb)
{
  החזר rb->size - 1;
}

size_t
ring_buffer_get_space_available (بنية ring_buffer *rb)
{
  החזר rb->size - ring_buffer_get_data_available (rb) - 1;
}

صحيح
ring_buffer_read (بنية ring_buffer *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err)
{
  بنية iovec iov[2];
  صحيح iov_count = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_count = ring_buffer_get_write_iov (rb, iov);
  אם (iov_count == 0)
    {
      *is_eagain = true;
      החזר 0;
    }

  ret = readv (fd, iov, iov_count);
  אם (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      אם (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          החזר 0;
        }
      החזר crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  אם (ret == 0)
    *is_eagain = true;
  ring_buffer_advance_nocheck_tail (rb, ret);
  החזר ret;
}

صحيح
ring_buffer_write (بنية ring_buffer *rb, صحيح fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  بنية iovec iov[2];
  صحيح iov_count = 0;

  *is_eagain = false;

  iov_count = ring_buffer_get_read_iov (rb, iov);
  אם (iov_count == 0)
    {
      *is_eagain = true;
      החזר 0;
    }

  ret = writev (fd, iov, iov_count);
  אם (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      אם (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          החזר 0;
        }
      החזר crun_make_error (err, errno, "writev");
    }
  ring_buffer_advance_nocheck_head (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  אם (rb->head == rb->tail)
    {
      rb->head = 0;
      rb->tail = 0;
    }
  החזר ret;
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

  החזר rb;
}

فراغ
ring_buffer_free (بنية ring_buffer *rb)
{
  אם (rb == عدم)
    החזר;
  حرر (rb->buffer);
  حرر (rb);
}
