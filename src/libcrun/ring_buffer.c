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
#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <sys/uio.h>

#અન્તર્ભાવ "ring_buffer.h"
#અન્તર્ભાવ "utils.h"

સંરચના ring_buffer
{
  અક્ષર *buffer;
  size_t size;
  size_t head;
  size_t tail;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
સ્થિર પૂર્ણાંક
ring_buffer_get_read_iov (સંરચના ring_buffer *rb, સંરચના iovec *iov)
{
  પૂર્ણાંક iov_ગણતરી = 0;

  /* Buffer is empty.  */
  જો (rb->head == rb->tail)
    પાછા 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  જો (rb->tail > rb->head)
    {
      iov[iov_ગણતરી].iov_base = rb->buffer + rb->head;
      iov[iov_ગણતરી].iov_len = rb->tail - rb->head;
      iov_ગણતરી++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  નહીંતર
    {
      iov[iov_ગણતરી].iov_base = rb->buffer + rb->head;
      iov[iov_ગણતરી].iov_len = rb->size - rb->head;
      iov_ગણતરી++;

      જો (rb->tail > 0)
        {
          iov[iov_ગણતરી].iov_base = rb->buffer;
          iov[iov_ગણતરી].iov_len = rb->tail;
          iov_ગણતરી++;
        }
    }
  પાછા iov_ગણતરી;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
સ્થિર પૂર્ણાંક
ring_buffer_get_write_iov (સંરચના ring_buffer *rb, સંરચના iovec *iov)
{
  પૂર્ણાંક iov_ગણતરી = 0;

  /* Buffer is full.  */
  જો ((rb->tail + 1) % rb->size == rb->head)
    પાછા 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  જો (rb->head > rb->tail + 1)
    {
      iov[iov_ગણતરી].iov_base = rb->buffer + rb->tail;
      iov[iov_ગણતરી].iov_len = rb->head - rb->tail - 1;
      iov_ગણતરી++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  નહીંતર
    {
      iov[iov_ગણતરી].iov_base = rb->buffer + rb->tail;
      iov[iov_ગણતરી].iov_len = rb->head == 0 ? rb->size - rb->tail - 1 : rb->size - rb->tail;
      iov_ગણતરી++;

      જો (rb->head > 1)
        {
          iov[iov_ગણતરી].iov_base = rb->buffer;
          iov[iov_ગણતરી].iov_len = rb->head - 1;
          iov_ગણતરી++;
        }
    }
  પાછા iov_ગણતરી;
}

/* manually advance the head after a successful read.  */
સ્થિર શૂન્ય
ring_buffer_advance_nocheck_head (સંરચના ring_buffer *rb, size_t amount)
{
  rb->head = (rb->head + amount) % rb->size;
}

/* manually advance the tail after a successful write.  */
સ્થિર શૂન્ય
ring_buffer_advance_nocheck_tail (સંરચના ring_buffer *rb, size_t amount)
{
  rb->tail = (rb->tail + amount) % rb->size;
}

size_t
ring_buffer_get_data_available (સંરચના ring_buffer *rb)
{
  જો (rb->head <= rb->tail)
    પાછા rb->tail - rb->head;

  પાછા rb->size - rb->head + rb->tail;
}

size_t
ring_buffer_get_size (સંરચના ring_buffer *rb)
{
  પાછા rb->size - 1;
}

size_t
ring_buffer_get_space_available (સંરચના ring_buffer *rb)
{
  પાછા rb->size - ring_buffer_get_data_available (rb) - 1;
}

પૂર્ણાંક
ring_buffer_read (સંરચના ring_buffer *rb, પૂર્ણાંક fd, bool *is_eagain, libcrun_error_t *err)
{
  સંરચના iovec iov[2];
  પૂર્ણાંક iov_ગણતરી = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_ગણતરી = ring_buffer_get_write_iov (rb, iov);
  જો (iov_ગણતરી == 0)
    {
      *is_eagain = true;
      પાછા 0;
    }

  ret = readv (fd, iov, iov_ગણતરી);
  જો (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      જો (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          પાછા 0;
        }
      પાછા crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  જો (ret == 0)
    *is_eagain = true;
  ring_buffer_advance_nocheck_tail (rb, ret);
  પાછા ret;
}

પૂર્ણાંક
ring_buffer_write (સંરચના ring_buffer *rb, પૂર્ણાંક fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  સંરચના iovec iov[2];
  પૂર્ણાંક iov_ગણતરી = 0;

  *is_eagain = false;

  iov_ગણતરી = ring_buffer_get_read_iov (rb, iov);
  જો (iov_ગણતરી == 0)
    {
      *is_eagain = true;
      પાછા 0;
    }

  ret = writev (fd, iov, iov_ગણતરી);
  જો (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      જો (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          પાછા 0;
        }
      પાછા crun_make_error (err, errno, "writev");
    }
  ring_buffer_advance_nocheck_head (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  જો (rb->head == rb->tail)
    {
      rb->head = 0;
      rb->tail = 0;
    }
  પાછા ret;
}

સંરચના ring_buffer *
ring_buffer_make (size_t size)
{
  સંરચના ring_buffer *rb = xmalloc (કદ (સંરચના ring_buffer));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->size = size + 1;
  rb->buffer = xmalloc (rb->size);
  rb->head = 0;
  rb->tail = 0;

  પાછા rb;
}

શૂન્ય
ring_buffer_free (સંરચના ring_buffer *rb)
{
  જો (rb == ખાલી)
    પાછા;
  મુક્ત (rb->buffer);
  મુક્ત (rb);
}
