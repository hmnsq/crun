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
#परिभाषय _GNU_SOURCE
#अन्तर्भाव <config.h>
#अन्तर्भाव <sys/uio.h>

#अन्तर्भाव "ring_buffer.h"
#अन्तर्भाव "utils.h"

संरचना ring_बफर
{
  वर्ण *बफर;
  size_t आकार;
  size_t शीर्ष;
  size_t पुच्छ;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
स्थिर पूर्णाङ्क
ring_बफर_get_read_iov (संरचना ring_बफर *rb, संरचना iovec *iov)
{
  पूर्णाङ्क iov_गणनम् = 0;

  /* Buffer is empty.  */
  यदि (rb->शीर्ष == rb->पुच्छ)
    प्रत्यागम 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  यदि (rb->पुच्छ > rb->शीर्ष)
    {
      iov[iov_गणनम्].iov_base = rb->बफर + rb->शीर्ष;
      iov[iov_गणनम्].iov_len = rb->पुच्छ - rb->शीर्ष;
      iov_गणनम्++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  अन्यथा
    {
      iov[iov_गणनम्].iov_base = rb->बफर + rb->शीर्ष;
      iov[iov_गणनम्].iov_len = rb->आकार - rb->शीर्ष;
      iov_गणनम्++;

      यदि (rb->पुच्छ > 0)
        {
          iov[iov_गणनम्].iov_base = rb->बफर;
          iov[iov_गणनम्].iov_len = rb->पुच्छ;
          iov_गणनम्++;
        }
    }
  प्रत्यागम iov_गणनम्;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
स्थिर पूर्णाङ्क
ring_बफर_get_write_iov (संरचना ring_बफर *rb, संरचना iovec *iov)
{
  पूर्णाङ्क iov_गणनम् = 0;

  /* Buffer is full.  */
  यदि ((rb->पुच्छ + 1) % rb->आकार == rb->शीर्ष)
    प्रत्यागम 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  यदि (rb->शीर्ष > rb->पुच्छ + 1)
    {
      iov[iov_गणनम्].iov_base = rb->बफर + rb->पुच्छ;
      iov[iov_गणनम्].iov_len = rb->शीर्ष - rb->पुच्छ - 1;
      iov_गणनम्++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  अन्यथा
    {
      iov[iov_गणनम्].iov_base = rb->बफर + rb->पुच्छ;
      iov[iov_गणनम्].iov_len = rb->शीर्ष == 0 ? rb->आकार - rb->पुच्छ - 1 : rb->आकार - rb->पुच्छ;
      iov_गणनम्++;

      यदि (rb->शीर्ष > 1)
        {
          iov[iov_गणनम्].iov_base = rb->बफर;
          iov[iov_गणनम्].iov_len = rb->शीर्ष - 1;
          iov_गणनम्++;
        }
    }
  प्रत्यागम iov_गणनम्;
}

/* manually advance the head after a successful read.  */
स्थिर शून्य
ring_बफर_advance_nocheck_शीर्ष (संरचना ring_बफर *rb, size_t amount)
{
  rb->शीर्ष = (rb->शीर्ष + amount) % rb->आकार;
}

/* manually advance the tail after a successful write.  */
स्थिर शून्य
ring_बफर_advance_nocheck_पुच्छ (संरचना ring_बफर *rb, size_t amount)
{
  rb->पुच्छ = (rb->पुच्छ + amount) % rb->आकार;
}

size_t
ring_बफर_get_दत्त_उपलब्धम् (संरचना ring_बफर *rb)
{
  यदि (rb->शीर्ष <= rb->पुच्छ)
    प्रत्यागम rb->पुच्छ - rb->शीर्ष;

  प्रत्यागम rb->आकार - rb->शीर्ष + rb->पुच्छ;
}

size_t
ring_बफर_get_आकार (संरचना ring_बफर *rb)
{
  प्रत्यागम rb->आकार - 1;
}

size_t
ring_बफर_get_space_उपलब्धम् (संरचना ring_बफर *rb)
{
  प्रत्यागम rb->आकार - ring_बफर_get_दत्त_उपलब्धम् (rb) - 1;
}

पूर्णाङ्क
ring_बफर_read (संरचना ring_बफर *rb, पूर्णाङ्क fd, bool *is_eagain, libcrun_error_t *err)
{
  संरचना iovec iov[2];
  पूर्णाङ्क iov_गणनम् = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_गणनम् = ring_बफर_get_write_iov (rb, iov);
  यदि (iov_गणनम् == 0)
    {
      *is_eagain = true;
      प्रत्यागम 0;
    }

  ret = readv (fd, iov, iov_गणनम्);
  यदि (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      यदि (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          प्रत्यागम 0;
        }
      प्रत्यागम crun_make_error (err, errno, "readv");
    }
  /* The same holds at EOF.  */
  यदि (ret == 0)
    *is_eagain = true;
  ring_बफर_advance_nocheck_पुच्छ (rb, ret);
  प्रत्यागम ret;
}

पूर्णाङ्क
ring_बफर_write (संरचना ring_बफर *rb, पूर्णाङ्क fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  संरचना iovec iov[2];
  पूर्णाङ्क iov_गणनम् = 0;

  *is_eagain = false;

  iov_गणनम् = ring_बफर_get_read_iov (rb, iov);
  यदि (iov_गणनम् == 0)
    {
      *is_eagain = true;
      प्रत्यागम 0;
    }

  ret = writev (fd, iov, iov_गणनम्);
  यदि (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      यदि (errno == EIO || errno == EAGAIN || errno == EWOULDBLOCK)
        {
          *is_eagain = true;
          प्रत्यागम 0;
        }
      प्रत्यागम crun_make_error (err, errno, "writev");
    }
  ring_बफर_advance_nocheck_शीर्ष (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  यदि (rb->शीर्ष == rb->पुच्छ)
    {
      rb->शीर्ष = 0;
      rb->पुच्छ = 0;
    }
  प्रत्यागम ret;
}

संरचना ring_बफर *
ring_बफर_make (size_t आकार)
{
  संरचना ring_बफर *rb = xmalloc (परिमाण (संरचना ring_बफर));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->आकार = आकार + 1;
  rb->बफर = xmalloc (rb->आकार);
  rb->शीर्ष = 0;
  rb->पुच्छ = 0;

  प्रत्यागम rb;
}

शून्य
ring_बफर_free (संरचना ring_बफर *rb)
{
  यदि (rb == रिक्त)
    प्रत्यागम;
  मुक्त (rb->बफर);
  मुक्त (rb);
}
