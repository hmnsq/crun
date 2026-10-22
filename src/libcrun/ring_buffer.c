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
#অন্তর্ভাব <config.h>
#অন্তর্ভাব <sys/uio.h>

#অন্তর্ভাব "ring_buffer.h"
#অন্তর্ভাব "utils.h"

কাঠামো ring_বাফার
{
  অক্ষর *বাফার;
  size_t আকার_2;
  size_t শীর্ষ;
  size_t লেজ;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
স্থির পূর্ণসংখ্যা
ring_বাফার_get_read_iov (কাঠামো ring_বাফার *rb, কাঠামো iovec *iov)
{
  পূর্ণসংখ্যা iov_গুনতি = 0;

  /* Buffer is empty.  */
  যদি (rb->শীর্ষ == rb->লেজ)
    ফেরত 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  যদি (rb->লেজ > rb->শীর্ষ)
    {
      iov[iov_গুনতি].iov_base = rb->বাফার + rb->শীর্ষ;
      iov[iov_গুনতি].iov_len = rb->লেজ - rb->শীর্ষ;
      iov_গুনতি++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  অন্যথা
    {
      iov[iov_গুনতি].iov_base = rb->বাফার + rb->শীর্ষ;
      iov[iov_গুনতি].iov_len = rb->আকার_2 - rb->শীর্ষ;
      iov_গুনতি++;

      যদি (rb->লেজ > 0)
        {
          iov[iov_গুনতি].iov_base = rb->বাফার;
          iov[iov_গুনতি].iov_len = rb->লেজ;
          iov_গুনতি++;
        }
    }
  ফেরত iov_গুনতি;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
স্থির পূর্ণসংখ্যা
ring_বাফার_get_write_iov (কাঠামো ring_বাফার *rb, কাঠামো iovec *iov)
{
  পূর্ণসংখ্যা iov_গুনতি = 0;

  /* Buffer is full.  */
  যদি ((rb->লেজ + 1) % rb->আকার_2 == rb->শীর্ষ)
    ফেরত 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  যদি (rb->শীর্ষ > rb->লেজ + 1)
    {
      iov[iov_গুনতি].iov_base = rb->বাফার + rb->লেজ;
      iov[iov_গুনতি].iov_len = rb->শীর্ষ - rb->লেজ - 1;
      iov_গুনতি++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  অন্যথা
    {
      iov[iov_গুনতি].iov_base = rb->বাফার + rb->লেজ;
      iov[iov_গুনতি].iov_len = rb->শীর্ষ == 0 ? rb->আকার_2 - rb->লেজ - 1 : rb->আকার_2 - rb->লেজ;
      iov_গুনতি++;

      যদি (rb->শীর্ষ > 1)
        {
          iov[iov_গুনতি].iov_base = rb->বাফার;
          iov[iov_গুনতি].iov_len = rb->শীর্ষ - 1;
          iov_গুনতি++;
        }
    }
  ফেরত iov_গুনতি;
}

/* manually advance the head after a successful read.  */
স্থির শূন্য
ring_বাফার_advance_nocheck_শীর্ষ (কাঠামো ring_বাফার *rb, size_t amount)
{
  rb->শীর্ষ = (rb->শীর্ষ + amount) % rb->আকার_2;
}

/* manually advance the tail after a successful write.  */
স্থির শূন্য
ring_বাফার_advance_nocheck_লেজ (কাঠামো ring_বাফার *rb, size_t amount)
{
  rb->লেজ = (rb->লেজ + amount) % rb->আকার_2;
}

size_t
ring_বাফার_get_তথ্য_available (কাঠামো ring_বাফার *rb)
{
  যদি (rb->শীর্ষ <= rb->লেজ)
    ফেরত rb->লেজ - rb->শীর্ষ;

  ফেরত rb->আকার_2 - rb->শীর্ষ + rb->লেজ;
}

size_t
ring_বাফার_get_আকার (কাঠামো ring_বাফার *rb)
{
  ফেরত rb->আকার_2 - 1;
}

size_t
ring_বাফার_get_space_available (কাঠামো ring_বাফার *rb)
{
  ফেরত rb->আকার_2 - ring_বাফার_get_তথ্য_available (rb) - 1;
}

পূর্ণসংখ্যা
ring_বাফার_read (কাঠামো ring_বাফার *rb, পূর্ণসংখ্যা fd, bool *is_eagain, libcrun_error_t *err)
{
  কাঠামো iovec iov[2];
  পূর্ণসংখ্যা iov_গুনতি = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_গুনতি = ring_বাফার_get_write_iov (rb, iov);
  যদি (iov_গুনতি == 0)
    {
      *is_eagain = true;
      ফেরত 0;
    }

  ret = readv (fd, iov, iov_গুনতি);
  যদি (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      যদি (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ফেরত 0;
        }
      ফেরত crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  যদি (ret == 0)
    *is_eagain = true;
  ring_বাফার_advance_nocheck_লেজ (rb, ret);
  ফেরত ret;
}

পূর্ণসংখ্যা
ring_বাফার_write (কাঠামো ring_বাফার *rb, পূর্ণসংখ্যা fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  কাঠামো iovec iov[2];
  পূর্ণসংখ্যা iov_গুনতি = 0;

  *is_eagain = false;

  iov_গুনতি = ring_বাফার_get_read_iov (rb, iov);
  যদি (iov_গুনতি == 0)
    {
      *is_eagain = true;
      ফেরত 0;
    }

  ret = writev (fd, iov, iov_গুনতি);
  যদি (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      যদি (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          ফেরত 0;
        }
      ফেরত crun_make_error (err, errno, "writev");
    }
  ring_বাফার_advance_nocheck_শীর্ষ (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  যদি (rb->শীর্ষ == rb->লেজ)
    {
      rb->শীর্ষ = 0;
      rb->লেজ = 0;
    }
  ফেরত ret;
}

কাঠামো ring_বাফার *
ring_বাফার_make (size_t আকার_2)
{
  কাঠামো ring_বাফার *rb = xmalloc (আকার (কাঠামো ring_বাফার));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->আকার_2 = আকার_2 + 1;
  rb->বাফার = xmalloc (rb->আকার_2);
  rb->শীর্ষ = 0;
  rb->লেজ = 0;

  ফেরত rb;
}

শূন্য
ring_বাফার_free (কাঠামো ring_বাফার *rb)
{
  যদি (rb == ফাঁকা)
    ফেরত;
  মুক্ত (rb->বাফার);
  মুক্ত (rb);
}
