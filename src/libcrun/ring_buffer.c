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
#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <sys/uio.h>

#অন্তর্ভাৱ "ring_buffer.h"
#অন্তর্ভাৱ "utils.h"

সংরচনা ring_buffer
{
  ৱর্ণ *buffer;
  size_t size;
  size_t head;
  size_t tail;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
স্থির পূর্ণাঙ্ক
ring_buffer_get_read_iov (সংরচনা ring_buffer *rb, সংরচনা iovec *iov)
{
  পূর্ণাঙ্ক iov_গুনতি = 0;

  /* Buffer is empty.  */
  যদি (rb->head == rb->tail)
    ঘূৰাই 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  যদি (rb->tail > rb->head)
    {
      iov[iov_গুনতি].iov_base = rb->buffer + rb->head;
      iov[iov_গুনতি].iov_len = rb->tail - rb->head;
      iov_গুনতি++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  অন্যথা
    {
      iov[iov_গুনতি].iov_base = rb->buffer + rb->head;
      iov[iov_গুনতি].iov_len = rb->size - rb->head;
      iov_গুনতি++;

      যদি (rb->tail > 0)
        {
          iov[iov_গুনতি].iov_base = rb->buffer;
          iov[iov_গুনতি].iov_len = rb->tail;
          iov_গুনতি++;
        }
    }
  ঘূৰাই iov_গুনতি;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
স্থির পূর্ণাঙ্ক
ring_buffer_get_write_iov (সংরচনা ring_buffer *rb, সংরচনা iovec *iov)
{
  পূর্ণাঙ্ক iov_গুনতি = 0;

  /* Buffer is full.  */
  যদি ((rb->tail + 1) % rb->size == rb->head)
    ঘূৰাই 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  যদি (rb->head > rb->tail + 1)
    {
      iov[iov_গুনতি].iov_base = rb->buffer + rb->tail;
      iov[iov_গুনতি].iov_len = rb->head - rb->tail - 1;
      iov_গুনতি++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  অন্যথা
    {
      iov[iov_গুনতি].iov_base = rb->buffer + rb->tail;
      iov[iov_গুনতি].iov_len = rb->head == 0 ? rb->size - rb->tail - 1 : rb->size - rb->tail;
      iov_গুনতি++;

      যদি (rb->head > 1)
        {
          iov[iov_গুনতি].iov_base = rb->buffer;
          iov[iov_গুনতি].iov_len = rb->head - 1;
          iov_গুনতি++;
        }
    }
  ঘূৰাই iov_গুনতি;
}

/* manually advance the head after a successful read.  */
স্থির শূন্য
ring_buffer_advance_nocheck_head (সংরচনা ring_buffer *rb, size_t amount)
{
  rb->head = (rb->head + amount) % rb->size;
}

/* manually advance the tail after a successful write.  */
স্থির শূন্য
ring_buffer_advance_nocheck_tail (সংরচনা ring_buffer *rb, size_t amount)
{
  rb->tail = (rb->tail + amount) % rb->size;
}

size_t
ring_buffer_get_data_available (সংরচনা ring_buffer *rb)
{
  যদি (rb->head <= rb->tail)
    ঘূৰাই rb->tail - rb->head;

  ঘূৰাই rb->size - rb->head + rb->tail;
}

size_t
ring_buffer_get_size (সংরচনা ring_buffer *rb)
{
  ঘূৰাই rb->size - 1;
}

size_t
ring_buffer_get_space_available (সংরচনা ring_buffer *rb)
{
  ঘূৰাই rb->size - ring_buffer_get_data_available (rb) - 1;
}

পূর্ণাঙ্ক
ring_buffer_read (সংরচনা ring_buffer *rb, পূর্ণাঙ্ক fd, bool *is_eagain, libcrun_error_t *err)
{
  সংরচনা iovec iov[2];
  পূর্ণাঙ্ক iov_গুনতি = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_গুনতি = ring_buffer_get_write_iov (rb, iov);
  যদি (iov_গুনতি == 0)
    {
      *is_eagain = true;
      ঘূৰাই 0;
    }

  ret = readv (fd, iov, iov_গুনতি);
  যদি (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      যদি (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ঘূৰাই 0;
        }
      ঘূৰাই crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  যদি (ret == 0)
    *is_eagain = true;
  ring_buffer_advance_nocheck_tail (rb, ret);
  ঘূৰাই ret;
}

পূর্ণাঙ্ক
ring_buffer_write (সংরচনা ring_buffer *rb, পূর্ণাঙ্ক fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  সংরচনা iovec iov[2];
  পূর্ণাঙ্ক iov_গুনতি = 0;

  *is_eagain = false;

  iov_গুনতি = ring_buffer_get_read_iov (rb, iov);
  যদি (iov_গুনতি == 0)
    {
      *is_eagain = true;
      ঘূৰাই 0;
    }

  ret = writev (fd, iov, iov_গুনতি);
  যদি (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      যদি (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ঘূৰাই 0;
        }
      ঘূৰাই crun_make_error (err, errno, "writev");
    }
  ring_buffer_advance_nocheck_head (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  যদি (rb->head == rb->tail)
    {
      rb->head = 0;
      rb->tail = 0;
    }
  ঘূৰাই ret;
}

সংরচনা ring_buffer *
ring_buffer_make (size_t size)
{
  সংরচনা ring_buffer *rb = xmalloc (পরিমাণ (সংরচনা ring_buffer));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->size = size + 1;
  rb->buffer = xmalloc (rb->size);
  rb->head = 0;
  rb->tail = 0;

  ঘূৰাই rb;
}

শূন্য
ring_buffer_free (সংরচনা ring_buffer *rb)
{
  যদি (rb == রিক্ত)
    ঘূৰাই;
  মুক্ত (rb->buffer);
  মুক্ত (rb);
}
