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
#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <sys/uio.h>

#ଅନ୍ତର୍ଭାଵ "ring_buffer.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

ସଂରଚନା ring_buffer
{
  ଅକ୍ଷର *buffer;
  size_t size;
  size_t head;
  size_t tail;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ring_buffer_get_read_iov (ସଂରଚନା ring_buffer *rb, ସଂରଚନା iovec *iov)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା iov_ଗଣନା = 0;

  /* Buffer is empty.  */
  ଯଦି (rb->head == rb->tail)
    ଫେରନ୍ତୁ 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  ଯଦି (rb->tail > rb->head)
    {
      iov[iov_ଗଣନା].iov_base = rb->buffer + rb->head;
      iov[iov_ଗଣନା].iov_len = rb->tail - rb->head;
      iov_ଗଣନା++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  ଅନ୍ୟଥା
    {
      iov[iov_ଗଣନା].iov_base = rb->buffer + rb->head;
      iov[iov_ଗଣନା].iov_len = rb->size - rb->head;
      iov_ଗଣନା++;

      ଯଦି (rb->tail > 0)
        {
          iov[iov_ଗଣନା].iov_base = rb->buffer;
          iov[iov_ଗଣନା].iov_len = rb->tail;
          iov_ଗଣନା++;
        }
    }
  ଫେରନ୍ତୁ iov_ଗଣନା;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ring_buffer_get_write_iov (ସଂରଚନା ring_buffer *rb, ସଂରଚନା iovec *iov)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା iov_ଗଣନା = 0;

  /* Buffer is full.  */
  ଯଦି ((rb->tail + 1) % rb->size == rb->head)
    ଫେରନ୍ତୁ 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  ଯଦି (rb->head > rb->tail + 1)
    {
      iov[iov_ଗଣନା].iov_base = rb->buffer + rb->tail;
      iov[iov_ଗଣନା].iov_len = rb->head - rb->tail - 1;
      iov_ଗଣନା++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  ଅନ୍ୟଥା
    {
      iov[iov_ଗଣନା].iov_base = rb->buffer + rb->tail;
      iov[iov_ଗଣନା].iov_len = rb->head == 0 ? rb->size - rb->tail - 1 : rb->size - rb->tail;
      iov_ଗଣନା++;

      ଯଦି (rb->head > 1)
        {
          iov[iov_ଗଣନା].iov_base = rb->buffer;
          iov[iov_ଗଣନା].iov_len = rb->head - 1;
          iov_ଗଣନା++;
        }
    }
  ଫେରନ୍ତୁ iov_ଗଣନା;
}

/* manually advance the head after a successful read.  */
ସ୍ଥିର ଶୂନ୍ୟ
ring_buffer_advance_nocheck_head (ସଂରଚନା ring_buffer *rb, size_t amount)
{
  rb->head = (rb->head + amount) % rb->size;
}

/* manually advance the tail after a successful write.  */
ସ୍ଥିର ଶୂନ୍ୟ
ring_buffer_advance_nocheck_tail (ସଂରଚନା ring_buffer *rb, size_t amount)
{
  rb->tail = (rb->tail + amount) % rb->size;
}

size_t
ring_buffer_get_data_available (ସଂରଚନା ring_buffer *rb)
{
  ଯଦି (rb->head <= rb->tail)
    ଫେରନ୍ତୁ rb->tail - rb->head;

  ଫେରନ୍ତୁ rb->size - rb->head + rb->tail;
}

size_t
ring_buffer_get_size (ସଂରଚନା ring_buffer *rb)
{
  ଫେରନ୍ତୁ rb->size - 1;
}

size_t
ring_buffer_get_space_available (ସଂରଚନା ring_buffer *rb)
{
  ଫେରନ୍ତୁ rb->size - ring_buffer_get_data_available (rb) - 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ring_buffer_read (ସଂରଚନା ring_buffer *rb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, bool *is_eagain, libcrun_error_t *err)
{
  ସଂରଚନା iovec iov[2];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା iov_ଗଣନା = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_ଗଣନା = ring_buffer_get_write_iov (rb, iov);
  ଯଦି (iov_ଗଣନା == 0)
    {
      *is_eagain = true;
      ଫେରନ୍ତୁ 0;
    }

  ret = readv (fd, iov, iov_ଗଣନା);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      ଯଦି (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ଫେରନ୍ତୁ 0;
        }
      ଫେରନ୍ତୁ crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  ଯଦି (ret == 0)
    *is_eagain = true;
  ring_buffer_advance_nocheck_tail (rb, ret);
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ring_buffer_write (ସଂରଚନା ring_buffer *rb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  ସଂରଚନା iovec iov[2];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା iov_ଗଣନା = 0;

  *is_eagain = false;

  iov_ଗଣନା = ring_buffer_get_read_iov (rb, iov);
  ଯଦି (iov_ଗଣନା == 0)
    {
      *is_eagain = true;
      ଫେରନ୍ତୁ 0;
    }

  ret = writev (fd, iov, iov_ଗଣନା);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      ଯଦି (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ଫେରନ୍ତୁ 0;
        }
      ଫେରନ୍ତୁ crun_make_error (err, errno, "writev");
    }
  ring_buffer_advance_nocheck_head (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  ଯଦି (rb->head == rb->tail)
    {
      rb->head = 0;
      rb->tail = 0;
    }
  ଫେରନ୍ତୁ ret;
}

ସଂରଚନା ring_buffer *
ring_buffer_make (size_t size)
{
  ସଂରଚନା ring_buffer *rb = xmalloc (ଆକାର (ସଂରଚନା ring_buffer));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->size = size + 1;
  rb->buffer = xmalloc (rb->size);
  rb->head = 0;
  rb->tail = 0;

  ଫେରନ୍ତୁ rb;
}

ଶୂନ୍ୟ
ring_buffer_free (ସଂରଚନା ring_buffer *rb)
{
  ଯଦି (rb == ଖାଲି)
    ଫେରନ୍ତୁ;
  ମୁକ୍ତ (rb->buffer);
  ମୁକ୍ତ (rb);
}
