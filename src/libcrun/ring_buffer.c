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
#घोषणा _GNU_SOURCE
#समावेश <config.h>
#समावेश <sys/uio.h>

#समावेश "ring_buffer.h"
#समावेश "utils.h"

काष्ठा ring_बफर
{
  अक्षर *बफर;
  माप_प्रकार आकार;
  माप_प्रकार शीर्ष;
  माप_प्रकार पुच्छ;
};

/*
 * It returns up to two regions in `iov` that can be read from.
 */
अटल पूर्णांक
ring_बफर_get_read_iov (काष्ठा ring_बफर *rb, काष्ठा iovec *iov)
{
  पूर्णांक iov_गिनती = 0;

  /* Buffer is empty.  */
  अगर (rb->शीर्ष == rb->पुच्छ)
    वापस 0;

  /* Head before tail.  There is only one region to read from, up to tail.  */
  अगर (rb->पुच्छ > rb->शीर्ष)
    {
      iov[iov_गिनती].iov_base = rb->बफर + rb->शीर्ष;
      iov[iov_गिनती].iov_len = rb->पुच्छ - rb->शीर्ष;
      iov_गिनती++;
    }
  /* Head after tail.  There are two regions to read from, up to the
   * end of the buffer and from the beginning of the buffer to tail.  */
  अन्यथा
    {
      iov[iov_गिनती].iov_base = rb->बफर + rb->शीर्ष;
      iov[iov_गिनती].iov_len = rb->आकार - rb->शीर्ष;
      iov_गिनती++;

      अगर (rb->पुच्छ > 0)
        {
          iov[iov_गिनती].iov_base = rb->बफर;
          iov[iov_गिनती].iov_len = rb->पुच्छ;
          iov_गिनती++;
        }
    }
  वापस iov_गिनती;
}

/*
 * It returns up to two regions in `iov` that can be written to without overwriting
 * existing data.
 */
अटल पूर्णांक
ring_बफर_get_write_iov (काष्ठा ring_बफर *rb, काष्ठा iovec *iov)
{
  पूर्णांक iov_गिनती = 0;

  /* Buffer is full.  */
  अगर ((rb->पुच्छ + 1) % rb->आकार == rb->शीर्ष)
    वापस 0;

  /* Tail before head.  There is only one region to write to, up to head.  */
  अगर (rb->शीर्ष > rb->पुच्छ + 1)
    {
      iov[iov_गिनती].iov_base = rb->बफर + rb->पुच्छ;
      iov[iov_गिनती].iov_len = rb->शीर्ष - rb->पुच्छ - 1;
      iov_गिनती++;
    }
  /* Tail after or equal to head.  There are two regions to write to, up to the
   * end of the buffer and from the beginning of the buffer to head.  */
  अन्यथा
    {
      iov[iov_गिनती].iov_base = rb->बफर + rb->पुच्छ;
      iov[iov_गिनती].iov_len = rb->शीर्ष == 0 ? rb->आकार - rb->पुच्छ - 1 : rb->आकार - rb->पुच्छ;
      iov_गिनती++;

      अगर (rb->शीर्ष > 1)
        {
          iov[iov_गिनती].iov_base = rb->बफर;
          iov[iov_गिनती].iov_len = rb->शीर्ष - 1;
          iov_गिनती++;
        }
    }
  वापस iov_गिनती;
}

/* manually advance the head after a successful read.  */
अटल व्योम
ring_बफर_advance_nocheck_शीर्ष (काष्ठा ring_बफर *rb, माप_प्रकार amount)
{
  rb->शीर्ष = (rb->शीर्ष + amount) % rb->आकार;
}

/* manually advance the tail after a successful write.  */
अटल व्योम
ring_बफर_advance_nocheck_पुच्छ (काष्ठा ring_बफर *rb, माप_प्रकार amount)
{
  rb->पुच्छ = (rb->पुच्छ + amount) % rb->आकार;
}

माप_प्रकार
ring_बफर_get_तथ्य_available (काष्ठा ring_बफर *rb)
{
  अगर (rb->शीर्ष <= rb->पुच्छ)
    वापस rb->पुच्छ - rb->शीर्ष;

  वापस rb->आकार - rb->शीर्ष + rb->पुच्छ;
}

माप_प्रकार
ring_बफर_get_आकार (काष्ठा ring_बफर *rb)
{
  वापस rb->आकार - 1;
}

माप_प्रकार
ring_बफर_get_space_available (काष्ठा ring_बफर *rb)
{
  वापस rb->आकार - ring_बफर_get_तथ्य_available (rb) - 1;
}

पूर्णांक
ring_बफर_read (काष्ठा ring_बफर *rb, पूर्णांक fd, bool *is_eagain, libcrun_error_t *err)
{
  काष्ठा iovec iov[2];
  पूर्णांक iov_गिनती = 0;
  ssize_t ret;

  *is_eagain = false;

  iov_गिनती = ring_बफर_get_write_iov (rb, iov);
  अगर (iov_गिनती == 0)
    {
      *is_eagain = true;
      वापस 0;
    }

  ret = readv (fd, iov, iov_गिनती);
  अगर (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to read until the state of the fd changes.  */
      अगर (त्रुटि_सं == EIO || त्रुटि_सं == EAGAIN || त्रुटि_सं == EWOULDBLOCK)
        {
          *is_eagain = true;
          वापस 0;
        }
      वापस crun_make_error (err, त्रुटि_सं, "readv");
    }
  /* The same holds at EOF.  */
  अगर (ret == 0)
    *is_eagain = true;
  ring_बफर_advance_nocheck_पुच्छ (rb, ret);
  वापस ret;
}

पूर्णांक
ring_बफर_write (काष्ठा ring_बफर *rb, पूर्णांक fd, bool *is_eagain, libcrun_error_t *err)
{
  ssize_t ret;
  काष्ठा iovec iov[2];
  पूर्णांक iov_गिनती = 0;

  *is_eagain = false;

  iov_गिनती = ring_बफर_get_read_iov (rb, iov);
  अगर (iov_गिनती == 0)
    {
      *is_eagain = true;
      वापस 0;
    }

  ret = writev (fd, iov, iov_गिनती);
  अगर (UNLIKELY (ret < 0))
    {
      /* A terminal that was hung up fails with EIO.  As with EAGAIN, there
         is nothing to write until the state of the fd changes.  */
      अगर (त्रुटि_सं == EIO || त्रुटि_सं == EAGAIN || त्रुटि_सं == EWOULDBLOCK)
        {
          *is_eagain = true;
          वापस 0;
        }
      वापस crun_make_error (err, त्रुटि_सं, "writev");
    }
  ring_बफर_advance_nocheck_शीर्ष (rb, ret);
  /* If the buffer is empty, reset the head and tail.  */
  अगर (rb->शीर्ष == rb->पुच्छ)
    {
      rb->शीर्ष = 0;
      rb->पुच्छ = 0;
    }
  वापस ret;
}

काष्ठा ring_बफर *
ring_बफर_make (माप_प्रकार आकार)
{
  काष्ठा ring_बफर *rb = xmalloc (माप (काष्ठा ring_बफर));

  /* The extra byte is used to distinguish between full and empty buffer.  */
  rb->आकार = आकार + 1;
  rb->बफर = xmalloc (rb->आकार);
  rb->शीर्ष = 0;
  rb->पुच्छ = 0;

  वापस rb;
}

व्योम
ring_बफर_free (काष्ठा ring_बफर *rb)
{
  अगर (rb == शून्य)
    वापस;
  मुक्त (rb->बफर);
  मुक्त (rb);
}
