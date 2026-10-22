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
#include <config.h>
#include <sys/uio.h>

#include "ring_buffer.h"
#include "utils.h"

sa_nracanaa ring_buffer
{
  var_na *buffer;
  size_t size;
  size_t head;
  size_t tail;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
static ཧྲིལ
ring_buffer_get_read_iov (sa_nracanaa ring_buffer *rb, sa_nracanaa iovec *iov)
{
  ཧྲིལ iov_count = 0;

  /* Buffer is empty.  */
  གལ་སྲིད (rb->head == rb->tail)
    ལོག 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  གལ་སྲིད (rb->tail > rb->head)
    {
      iov[iov_count].iov_base = rb->buffer + rb->head;
      iov[iov_count].iov_len = rb->tail - rb->head;
      iov_count++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  དེ་མིན
    {
      iov[iov_count].iov_base = rb->buffer + rb->head;
      iov[iov_count].iov_len = rb->size - rb->head;
      iov_count++;

      གལ་སྲིད (rb->tail > 0)
        {
          iov[iov_count].iov_base = rb->buffer;
          iov[iov_count].iov_len = rb->tail;
          iov_count++;
        }
    }
  ལོག iov_count;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
static ཧྲིལ
ring_buffer_get_write_iov (sa_nracanaa ring_buffer *rb, sa_nracanaa iovec *iov)
{
  ཧྲིལ iov_count = 0;

  /* Buffer is full.  */
  གལ་སྲིད ((rb->tail + 1) % rb->size == rb->head)
    ལོག 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  གལ་སྲིད (rb->head > rb->tail + 1)
    {
      iov[iov_count].iov_base = rb->buffer + rb->tail;
      iov[iov_count].iov_len = rb->head - rb->tail - 1;
      iov_count++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  དེ་མིན
    {
      iov[iov_count].iov_base = rb->buffer + rb->tail;
      iov[iov_count].iov_len = rb->head == 0 ? rb->size - rb->tail - 1 : rb->size - rb->tail;
      iov_count++;

      གལ་སྲིད (rb->head > 1)
        {
          iov[iov_count].iov_base = rb->buffer;
          iov[iov_count].iov_len = rb->head - 1;
          iov_count++;
        }
    }
  ལོག iov_count;
}

/* manually advance the head after a successful read.  */
static void
ring_buffer_advance_nocheck_head (sa_nracanaa ring_buffer *rb, size_t amount)
{
  rb->head = (rb->head + amount) % rb->size;
}

/* manually advance the tail after a successful write.  */
static void
ring_buffer_advance_nocheck_tail (sa_nracanaa ring_buffer *rb, size_t amount)
{
  rb->tail = (rb->tail + amount) % rb->size;
}

size_t
ring_buffer_get_data_available (sa_nracanaa ring_buffer *rb)
{
  གལ་སྲིད (rb->head <= rb->tail)
    ལོག rb->tail - rb->head;

  ལོག rb->size - rb->head + rb->tail;
}

size_t
ring_buffer_get_size (sa_nracanaa ring_buffer *rb)
{
  ལོག rb->size - 1;
}

size_t
ring_buffer_get_space_available (sa_nracanaa ring_buffer *rb)
{
  ལོག rb->size - ring_buffer_get_data_available (rb) - 1;
}

ཧྲིལ
ring_buffer_read (sa_nracanaa ring_buffer *rb, ཧྲིལ fd, bool *is_eagain, libcrun_error_t *err)
{
  sa_nracanaa iovec iov[2];
  ཧྲིལ iov_count = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_count = ring_buffer_get_write_iov (rb, iov);
  གལ་སྲིད (iov_count == 0)
    {
      *is_eagain = true;
      ལོག 0;
    }

  ret = readv (fd, iov, iov_count);
  གལ་སྲིད (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      གལ་སྲིད (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ལོག 0;
        }
      ལོག crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  གལ་སྲིད (ret == 0)
    *is_eagain = true;
  ring_buffer_advance_nocheck_tail (rb, ret);
  ལོག ret;
}

ཧྲིལ
ring_buffer_write (sa_nracanaa ring_buffer *rb, ཧྲིལ fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  sa_nracanaa iovec iov[2];
  ཧྲིལ iov_count = 0;

  *is_eagain = false;

  iov_count = ring_buffer_get_read_iov (rb, iov);
  གལ་སྲིད (iov_count == 0)
    {
      *is_eagain = true;
      ལོག 0;
    }

  ret = writev (fd, iov, iov_count);
  གལ་སྲིད (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      གལ་སྲིད (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ལོག 0;
        }
      ལོག crun_make_error (err, errno, "writev");
    }
  ring_buffer_advance_nocheck_head (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  གལ་སྲིད (rb->head == rb->tail)
    {
      rb->head = 0;
      rb->tail = 0;
    }
  ལོག ret;
}

sa_nracanaa ring_buffer *
ring_buffer_make (size_t size)
{
  sa_nracanaa ring_buffer *rb = xmalloc (parimaa_na (sa_nracanaa ring_buffer));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->size = size + 1;
  rb->buffer = xmalloc (rb->size);
  rb->head = 0;
  rb->tail = 0;

  ལོག rb;
}

void
ring_buffer_free (sa_nracanaa ring_buffer *rb)
{
  གལ་སྲིད (rb == NULL)
    ལོག;
  free (rb->buffer);
  free (rb);
}
