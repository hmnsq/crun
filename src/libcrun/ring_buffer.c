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
#அந்தர்பாவ <config.h>
#அந்தர்பாவ <sys/uio.h>

#அந்தர்பாவ "ring_buffer.h"
#அந்தர்பாவ "utils.h"

கட்டமைப்பு ring_buffer
{
  எழுத்து *buffer;
  size_t size;
  size_t head;
  size_t tail;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
நிலையான முழு
ring_buffer_get_read_iov (கட்டமைப்பு ring_buffer *rb, கட்டமைப்பு iovec *iov)
{
  முழு iov_மொத்தம் = 0;

  /* Buffer is empty.  */
  எனில் (rb->head == rb->tail)
    திரும்பு 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  எனில் (rb->tail > rb->head)
    {
      iov[iov_மொத்தம்].iov_base = rb->buffer + rb->head;
      iov[iov_மொத்தம்].iov_len = rb->tail - rb->head;
      iov_மொத்தம்++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  இல்லையெனில்
    {
      iov[iov_மொத்தம்].iov_base = rb->buffer + rb->head;
      iov[iov_மொத்தம்].iov_len = rb->size - rb->head;
      iov_மொத்தம்++;

      எனில் (rb->tail > 0)
        {
          iov[iov_மொத்தம்].iov_base = rb->buffer;
          iov[iov_மொத்தம்].iov_len = rb->tail;
          iov_மொத்தம்++;
        }
    }
  திரும்பு iov_மொத்தம்;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
நிலையான முழு
ring_buffer_get_write_iov (கட்டமைப்பு ring_buffer *rb, கட்டமைப்பு iovec *iov)
{
  முழு iov_மொத்தம் = 0;

  /* Buffer is full.  */
  எனில் ((rb->tail + 1) % rb->size == rb->head)
    திரும்பு 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  எனில் (rb->head > rb->tail + 1)
    {
      iov[iov_மொத்தம்].iov_base = rb->buffer + rb->tail;
      iov[iov_மொத்தம்].iov_len = rb->head - rb->tail - 1;
      iov_மொத்தம்++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  இல்லையெனில்
    {
      iov[iov_மொத்தம்].iov_base = rb->buffer + rb->tail;
      iov[iov_மொத்தம்].iov_len = rb->head == 0 ? rb->size - rb->tail - 1 : rb->size - rb->tail;
      iov_மொத்தம்++;

      எனில் (rb->head > 1)
        {
          iov[iov_மொத்தம்].iov_base = rb->buffer;
          iov[iov_மொத்தம்].iov_len = rb->head - 1;
          iov_மொத்தம்++;
        }
    }
  திரும்பு iov_மொத்தம்;
}

/* manually advance the head after a successful read.  */
நிலையான வெறுமை
ring_buffer_advance_nocheck_head (கட்டமைப்பு ring_buffer *rb, size_t amount)
{
  rb->head = (rb->head + amount) % rb->size;
}

/* manually advance the tail after a successful write.  */
நிலையான வெறுமை
ring_buffer_advance_nocheck_tail (கட்டமைப்பு ring_buffer *rb, size_t amount)
{
  rb->tail = (rb->tail + amount) % rb->size;
}

size_t
ring_buffer_get_data_available (கட்டமைப்பு ring_buffer *rb)
{
  எனில் (rb->head <= rb->tail)
    திரும்பு rb->tail - rb->head;

  திரும்பு rb->size - rb->head + rb->tail;
}

size_t
ring_buffer_get_size (கட்டமைப்பு ring_buffer *rb)
{
  திரும்பு rb->size - 1;
}

size_t
ring_buffer_get_space_available (கட்டமைப்பு ring_buffer *rb)
{
  திரும்பு rb->size - ring_buffer_get_data_available (rb) - 1;
}

முழு
ring_buffer_read (கட்டமைப்பு ring_buffer *rb, முழு fd, bool *is_eagain, libcrun_error_t *err)
{
  கட்டமைப்பு iovec iov[2];
  முழு iov_மொத்தம் = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_மொத்தம் = ring_buffer_get_write_iov (rb, iov);
  எனில் (iov_மொத்தம் == 0)
    {
      *is_eagain = true;
      திரும்பு 0;
    }

  ret = readv (fd, iov, iov_மொத்தம்);
  எனில் (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      எனில் (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          திரும்பு 0;
        }
      திரும்பு crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  எனில் (ret == 0)
    *is_eagain = true;
  ring_buffer_advance_nocheck_tail (rb, ret);
  திரும்பு ret;
}

முழு
ring_buffer_write (கட்டமைப்பு ring_buffer *rb, முழு fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  கட்டமைப்பு iovec iov[2];
  முழு iov_மொத்தம் = 0;

  *is_eagain = false;

  iov_மொத்தம் = ring_buffer_get_read_iov (rb, iov);
  எனில் (iov_மொத்தம் == 0)
    {
      *is_eagain = true;
      திரும்பு 0;
    }

  ret = writev (fd, iov, iov_மொத்தம்);
  எனில் (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      எனில் (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          திரும்பு 0;
        }
      திரும்பு crun_make_error (err, errno, "writev");
    }
  ring_buffer_advance_nocheck_head (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  எனில் (rb->head == rb->tail)
    {
      rb->head = 0;
      rb->tail = 0;
    }
  திரும்பு ret;
}

கட்டமைப்பு ring_buffer *
ring_buffer_make (size_t size)
{
  கட்டமைப்பு ring_buffer *rb = xmalloc (அளவு (கட்டமைப்பு ring_buffer));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->size = size + 1;
  rb->buffer = xmalloc (rb->size);
  rb->head = 0;
  rb->tail = 0;

  திரும்பு rb;
}

வெறுமை
ring_buffer_free (கட்டமைப்பு ring_buffer *rb)
{
  எனில் (rb == வெற்று)
    திரும்பு;
  விடுவி (rb->buffer);
  விடுவி (rb);
}
