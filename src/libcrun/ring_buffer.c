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
#నిర్వచించు GNU_ఆధారం
#అన్తర్భావ <config.h>
#అన్తర్భావ <sys/uio.h>

#అన్తర్భావ "ring_buffer.h"
#అన్తర్భావ "utils.h"

నిర్మాణం ring_బఫరు
{
  అక్షరం *బఫరు;
  size_t కొలత;
  size_t తల;
  size_t తోక;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
స్థిర పూర్ణాంకం
ring_బఫరు_get_read_iov (నిర్మాణం ring_బఫరు *rb, నిర్మాణం iovec *iov)
{
  పూర్ణాంకం iov_లెక్క = 0;

  /* Buffer is empty.  */
  ఒకవేళ (rb->తల == rb->తోక)
    తిరిగి 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  ఒకవేళ (rb->తోక > rb->తల)
    {
      iov[iov_లెక్క].iov_base = rb->బఫరు + rb->తల;
      iov[iov_లెక్క].iov_len = rb->తోక - rb->తల;
      iov_లెక్క++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  లేకపోతే
    {
      iov[iov_లెక్క].iov_base = rb->బఫరు + rb->తల;
      iov[iov_లెక్క].iov_len = rb->కొలత - rb->తల;
      iov_లెక్క++;

      ఒకవేళ (rb->తోక > 0)
        {
          iov[iov_లెక్క].iov_base = rb->బఫరు;
          iov[iov_లెక్క].iov_len = rb->తోక;
          iov_లెక్క++;
        }
    }
  తిరిగి iov_లెక్క;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
స్థిర పూర్ణాంకం
ring_బఫరు_get_write_iov (నిర్మాణం ring_బఫరు *rb, నిర్మాణం iovec *iov)
{
  పూర్ణాంకం iov_లెక్క = 0;

  /* Buffer is full.  */
  ఒకవేళ ((rb->తోక + 1) % rb->కొలత == rb->తల)
    తిరిగి 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  ఒకవేళ (rb->తల > rb->తోక + 1)
    {
      iov[iov_లెక్క].iov_base = rb->బఫరు + rb->తోక;
      iov[iov_లెక్క].iov_len = rb->తల - rb->తోక - 1;
      iov_లెక్క++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  లేకపోతే
    {
      iov[iov_లెక్క].iov_base = rb->బఫరు + rb->తోక;
      iov[iov_లెక్క].iov_len = rb->తల == 0 ? rb->కొలత - rb->తోక - 1 : rb->కొలత - rb->తోక;
      iov_లెక్క++;

      ఒకవేళ (rb->తల > 1)
        {
          iov[iov_లెక్క].iov_base = rb->బఫరు;
          iov[iov_లెక్క].iov_len = rb->తల - 1;
          iov_లెక్క++;
        }
    }
  తిరిగి iov_లెక్క;
}

/* manually advance the head after a successful read.  */
స్థిర శూన్య
ring_బఫరు_advance_nocheck_తల (నిర్మాణం ring_బఫరు *rb, size_t amount)
{
  rb->తల = (rb->తల + amount) % rb->కొలత;
}

/* manually advance the tail after a successful write.  */
స్థిర శూన్య
ring_బఫరు_advance_nocheck_తోక (నిర్మాణం ring_బఫరు *rb, size_t amount)
{
  rb->తోక = (rb->తోక + amount) % rb->కొలత;
}

size_t
ring_బఫరు_get_దత్తాంశం_అందుబాటు (నిర్మాణం ring_బఫరు *rb)
{
  ఒకవేళ (rb->తల <= rb->తోక)
    తిరిగి rb->తోక - rb->తల;

  తిరిగి rb->కొలత - rb->తల + rb->తోక;
}

size_t
ring_బఫరు_get_కొలత (నిర్మాణం ring_బఫరు *rb)
{
  తిరిగి rb->కొలత - 1;
}

size_t
ring_బఫరు_get_space_అందుబాటు (నిర్మాణం ring_బఫరు *rb)
{
  తిరిగి rb->కొలత - ring_బఫరు_get_దత్తాంశం_అందుబాటు (rb) - 1;
}

పూర్ణాంకం
ring_బఫరు_read (నిర్మాణం ring_బఫరు *rb, పూర్ణాంకం fd, bool *is_eagain, libcrun_error_t *err)
{
  నిర్మాణం iovec iov[2];
  పూర్ణాంకం iov_లెక్క = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_లెక్క = ring_బఫరు_get_write_iov (rb, iov);
  ఒకవేళ (iov_లెక్క == 0)
    {
      *is_eagain = true;
      తిరిగి 0;
    }

  ret = readv (fd, iov, iov_లెక్క);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      ఒకవేళ (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          తిరిగి 0;
        }
      తిరిగి crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  ఒకవేళ (ret == 0)
    *is_eagain = true;
  ring_బఫరు_advance_nocheck_తోక (rb, ret);
  తిరిగి ret;
}

పూర్ణాంకం
ring_బఫరు_write (నిర్మాణం ring_బఫరు *rb, పూర్ణాంకం fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  నిర్మాణం iovec iov[2];
  పూర్ణాంకం iov_లెక్క = 0;

  *is_eagain = false;

  iov_లెక్క = ring_బఫరు_get_read_iov (rb, iov);
  ఒకవేళ (iov_లెక్క == 0)
    {
      *is_eagain = true;
      తిరిగి 0;
    }

  ret = writev (fd, iov, iov_లెక్క);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      ఒకవేళ (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          తిరిగి 0;
        }
      తిరిగి crun_make_error (err, errno, "writev");
    }
  ring_బఫరు_advance_nocheck_తల (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  ఒకవేళ (rb->తల == rb->తోక)
    {
      rb->తల = 0;
      rb->తోక = 0;
    }
  తిరిగి ret;
}

నిర్మాణం ring_బఫరు *
ring_బఫరు_make (size_t కొలత)
{
  నిర్మాణం ring_బఫరు *rb = xmalloc (పరిమాణం (నిర్మాణం ring_బఫరు));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->కొలత = కొలత + 1;
  rb->బఫరు = xmalloc (rb->కొలత);
  rb->తల = 0;
  rb->తోక = 0;

  తిరిగి rb;
}

శూన్య
ring_బఫరు_free (నిర్మాణం ring_బఫరు *rb)
{
  ఒకవేళ (rb == ఖాళీ)
    తిరిగి;
  విడుదల (rb->బఫరు);
  విడుదల (rb);
}
