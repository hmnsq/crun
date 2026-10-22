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
#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <sys/uio.h>

#അന്തര്ഭാവ "ring_buffer.h"
#അന്തര്ഭാവ "utils.h"

ഘടന ring_buffer
{
  അക്ഷരം *buffer;
  size_t size;
  size_t head;
  size_t tail;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ
ring_buffer_get_read_iov (ഘടന ring_buffer *rb, ഘടന iovec *iov)
{
  പൂർണ്ണസംഖ്യ iov_എണ്ണം = 0;

  /* Buffer is empty.  */
  എങ്കിൽ (rb->head == rb->tail)
    തിരികെ 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  എങ്കിൽ (rb->tail > rb->head)
    {
      iov[iov_എണ്ണം].iov_base = rb->buffer + rb->head;
      iov[iov_എണ്ണം].iov_len = rb->tail - rb->head;
      iov_എണ്ണം++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  അല്ലെങ്കിൽ
    {
      iov[iov_എണ്ണം].iov_base = rb->buffer + rb->head;
      iov[iov_എണ്ണം].iov_len = rb->size - rb->head;
      iov_എണ്ണം++;

      എങ്കിൽ (rb->tail > 0)
        {
          iov[iov_എണ്ണം].iov_base = rb->buffer;
          iov[iov_എണ്ണം].iov_len = rb->tail;
          iov_എണ്ണം++;
        }
    }
  തിരികെ iov_എണ്ണം;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ
ring_buffer_get_write_iov (ഘടന ring_buffer *rb, ഘടന iovec *iov)
{
  പൂർണ്ണസംഖ്യ iov_എണ്ണം = 0;

  /* Buffer is full.  */
  എങ്കിൽ ((rb->tail + 1) % rb->size == rb->head)
    തിരികെ 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  എങ്കിൽ (rb->head > rb->tail + 1)
    {
      iov[iov_എണ്ണം].iov_base = rb->buffer + rb->tail;
      iov[iov_എണ്ണം].iov_len = rb->head - rb->tail - 1;
      iov_എണ്ണം++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  അല്ലെങ്കിൽ
    {
      iov[iov_എണ്ണം].iov_base = rb->buffer + rb->tail;
      iov[iov_എണ്ണം].iov_len = rb->head == 0 ? rb->size - rb->tail - 1 : rb->size - rb->tail;
      iov_എണ്ണം++;

      എങ്കിൽ (rb->head > 1)
        {
          iov[iov_എണ്ണം].iov_base = rb->buffer;
          iov[iov_എണ്ണം].iov_len = rb->head - 1;
          iov_എണ്ണം++;
        }
    }
  തിരികെ iov_എണ്ണം;
}

/* manually advance the head after a successful read.  */
സ്ഥിതം ശൂന്യം
ring_buffer_advance_nocheck_head (ഘടന ring_buffer *rb, size_t amount)
{
  rb->head = (rb->head + amount) % rb->size;
}

/* manually advance the tail after a successful write.  */
സ്ഥിതം ശൂന്യം
ring_buffer_advance_nocheck_tail (ഘടന ring_buffer *rb, size_t amount)
{
  rb->tail = (rb->tail + amount) % rb->size;
}

size_t
ring_buffer_get_data_available (ഘടന ring_buffer *rb)
{
  എങ്കിൽ (rb->head <= rb->tail)
    തിരികെ rb->tail - rb->head;

  തിരികെ rb->size - rb->head + rb->tail;
}

size_t
ring_buffer_get_size (ഘടന ring_buffer *rb)
{
  തിരികെ rb->size - 1;
}

size_t
ring_buffer_get_space_available (ഘടന ring_buffer *rb)
{
  തിരികെ rb->size - ring_buffer_get_data_available (rb) - 1;
}

പൂർണ്ണസംഖ്യ
ring_buffer_read (ഘടന ring_buffer *rb, പൂർണ്ണസംഖ്യ fd, bool *is_eagain, libcrun_error_t *err)
{
  ഘടന iovec iov[2];
  പൂർണ്ണസംഖ്യ iov_എണ്ണം = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_എണ്ണം = ring_buffer_get_write_iov (rb, iov);
  എങ്കിൽ (iov_എണ്ണം == 0)
    {
      *is_eagain = true;
      തിരികെ 0;
    }

  ret = readv (fd, iov, iov_എണ്ണം);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      എങ്കിൽ (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          തിരികെ 0;
        }
      തിരികെ crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  എങ്കിൽ (ret == 0)
    *is_eagain = true;
  ring_buffer_advance_nocheck_tail (rb, ret);
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
ring_buffer_write (ഘടന ring_buffer *rb, പൂർണ്ണസംഖ്യ fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  ഘടന iovec iov[2];
  പൂർണ്ണസംഖ്യ iov_എണ്ണം = 0;

  *is_eagain = false;

  iov_എണ്ണം = ring_buffer_get_read_iov (rb, iov);
  എങ്കിൽ (iov_എണ്ണം == 0)
    {
      *is_eagain = true;
      തിരികെ 0;
    }

  ret = writev (fd, iov, iov_എണ്ണം);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      എങ്കിൽ (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          തിരികെ 0;
        }
      തിരികെ crun_make_error (err, errno, "writev");
    }
  ring_buffer_advance_nocheck_head (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  എങ്കിൽ (rb->head == rb->tail)
    {
      rb->head = 0;
      rb->tail = 0;
    }
  തിരികെ ret;
}

ഘടന ring_buffer *
ring_buffer_make (size_t size)
{
  ഘടന ring_buffer *rb = xmalloc (വലുപ്പം (ഘടന ring_buffer));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->size = size + 1;
  rb->buffer = xmalloc (rb->size);
  rb->head = 0;
  rb->tail = 0;

  തിരികെ rb;
}

ശൂന്യം
ring_buffer_free (ഘടന ring_buffer *rb)
{
  എങ്കിൽ (rb == ശൂന്യ)
    തിരികെ;
  സ്വതന്ത്രം (rb->buffer);
  സ്വതന്ത്രം (rb);
}
