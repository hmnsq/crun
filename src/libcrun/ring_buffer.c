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
#अन्तर्भाव <config.h>
#अन्तर्भाव <sys/uio.h>

#अन्तर्भाव "ring_buffer.h"
#अन्तर्भाव "utils.h"

संरचना ring_buffer
{
  अक्षर *buffer;
  माप_प्रकार size;
  माप_प्रकार head;
  माप_प्रकार tail;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
स्थिर पूर्णांक
ring_buffer_get_read_iov (संरचना ring_buffer *rb, संरचना iovec *iov)
{
  पूर्णांक iov_मोजणी = 0;

  /* Buffer is empty.  */
  जर (rb->head == rb->tail)
    परत 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  जर (rb->tail > rb->head)
    {
      iov[iov_मोजणी].iov_base = rb->buffer + rb->head;
      iov[iov_मोजणी].iov_len = rb->tail - rb->head;
      iov_मोजणी++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  अन्यथा
    {
      iov[iov_मोजणी].iov_base = rb->buffer + rb->head;
      iov[iov_मोजणी].iov_len = rb->size - rb->head;
      iov_मोजणी++;

      जर (rb->tail > 0)
        {
          iov[iov_मोजणी].iov_base = rb->buffer;
          iov[iov_मोजणी].iov_len = rb->tail;
          iov_मोजणी++;
        }
    }
  परत iov_मोजणी;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
स्थिर पूर्णांक
ring_buffer_get_write_iov (संरचना ring_buffer *rb, संरचना iovec *iov)
{
  पूर्णांक iov_मोजणी = 0;

  /* Buffer is full.  */
  जर ((rb->tail + 1) % rb->size == rb->head)
    परत 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  जर (rb->head > rb->tail + 1)
    {
      iov[iov_मोजणी].iov_base = rb->buffer + rb->tail;
      iov[iov_मोजणी].iov_len = rb->head - rb->tail - 1;
      iov_मोजणी++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  अन्यथा
    {
      iov[iov_मोजणी].iov_base = rb->buffer + rb->tail;
      iov[iov_मोजणी].iov_len = rb->head == 0 ? rb->size - rb->tail - 1 : rb->size - rb->tail;
      iov_मोजणी++;

      जर (rb->head > 1)
        {
          iov[iov_मोजणी].iov_base = rb->buffer;
          iov[iov_मोजणी].iov_len = rb->head - 1;
          iov_मोजणी++;
        }
    }
  परत iov_मोजणी;
}

/* manually advance the head after a successful read.  */
स्थिर शून्य
ring_buffer_advance_nocheck_head (संरचना ring_buffer *rb, माप_प्रकार amount)
{
  rb->head = (rb->head + amount) % rb->size;
}

/* manually advance the tail after a successful write.  */
स्थिर शून्य
ring_buffer_advance_nocheck_tail (संरचना ring_buffer *rb, माप_प्रकार amount)
{
  rb->tail = (rb->tail + amount) % rb->size;
}

माप_प्रकार
ring_buffer_get_data_available (संरचना ring_buffer *rb)
{
  जर (rb->head <= rb->tail)
    परत rb->tail - rb->head;

  परत rb->size - rb->head + rb->tail;
}

माप_प्रकार
ring_buffer_get_size (संरचना ring_buffer *rb)
{
  परत rb->size - 1;
}

माप_प्रकार
ring_buffer_get_space_available (संरचना ring_buffer *rb)
{
  परत rb->size - ring_buffer_get_data_available (rb) - 1;
}

पूर्णांक
ring_buffer_read (संरचना ring_buffer *rb, पूर्णांक fd, bool *is_eagain, libcrun_error_t *err)
{
  संरचना iovec iov[2];
  पूर्णांक iov_मोजणी = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_मोजणी = ring_buffer_get_write_iov (rb, iov);
  जर (iov_मोजणी == 0)
    {
      *is_eagain = true;
      परत 0;
    }

  ret = readv (fd, iov, iov_मोजणी);
  जर (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      जर (त्रुटि_सं == EIO || त्रुटि_सं == EAGAIN || त्रुटि_सं == EWOULDBLOCK)
        {
          *is_eagain = true;
          परत 0;
        }
      परत crun_make_error (err, त्रुटि_सं, "readv");
    }
  /* The same holds at EOF.  */
  जर (ret == 0)
    *is_eagain = true;
  ring_buffer_advance_nocheck_tail (rb, ret);
  परत ret;
}

पूर्णांक
ring_buffer_write (संरचना ring_buffer *rb, पूर्णांक fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  संरचना iovec iov[2];
  पूर्णांक iov_मोजणी = 0;

  *is_eagain = false;

  iov_मोजणी = ring_buffer_get_read_iov (rb, iov);
  जर (iov_मोजणी == 0)
    {
      *is_eagain = true;
      परत 0;
    }

  ret = writev (fd, iov, iov_मोजणी);
  जर (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      जर (त्रुटि_सं == EIO || त्रुटि_सं == EAGAIN || त्रुटि_सं == EWOULDBLOCK)
        {
          *is_eagain = true;
          परत 0;
        }
      परत crun_make_error (err, त्रुटि_सं, "writev");
    }
  ring_buffer_advance_nocheck_head (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  जर (rb->head == rb->tail)
    {
      rb->head = 0;
      rb->tail = 0;
    }
  परत ret;
}

संरचना ring_buffer *
ring_buffer_make (माप_प्रकार size)
{
  संरचना ring_buffer *rb = xmalloc (आकार (संरचना ring_buffer));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->size = size + 1;
  rb->buffer = xmalloc (rb->size);
  rb->head = 0;
  rb->tail = 0;

  परत rb;
}

शून्य
ring_buffer_free (संरचना ring_buffer *rb)
{
  जर (rb == रिक्त)
    परत;
  मुक्त (rb->buffer);
  मुक्त (rb);
}
