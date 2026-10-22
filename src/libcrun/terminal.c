/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#घोषणा _XOPEN_SOURCE

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "linux.h"
#समावेश "utils.h"

#समावेश <stdlib.h>
#समावेश <unistd.h>
#समावेश <fcntl.h>
#समावेश <sys/ioctl.h>
#समावेश <termios.h>

#अगर_अघोषित TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#पूर्ण_अगर

काष्ठा terminal_status_s
{
  पूर्णांक fd;
  काष्ठा termios termios;
};

पूर्णांक
libcrun_new_terminal (अक्षर **pty, libcrun_error_t *err)
{
  अक्षर buf[64];
  पूर्णांक ret;
  cleanup_close पूर्णांक fd = खोलो ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, माप (buf));
  अगर (UNLIKELY (ret != 0))
    वापस crun_make_error (err, त्रुटि_सं, "ptsname");

  ret = unlockpt (fd);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  वापस ret;
}

पूर्णांक
libcrun_set_raw (पूर्णांक fd, व्योम **current_status, libcrun_error_t *err)
{
  पूर्णांक ret;
  काष्ठा termios termios;

  ret = tcgetattr (fd, &termios);
  अगर (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      अगर (त्रुटि_सं == ENOTTY)
        वापस 0;
      वापस crun_make_error (err, त्रुटि_सं, "tcgetattr");
    }

  अगर (current_status)
    {
      काष्ठा terminal_status_s *s = xmalloc (माप (*s));
      s->fd = fd;
      स_नकल (&(s->termios), &termios, माप (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "tcsetattr");

  वापस 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
पूर्णांक
libcrun_clear_onlcr (पूर्णांक fd, libcrun_error_t *err)
{
  पूर्णांक ret;
  काष्ठा termios termios;

  ret = tcgetattr (fd, &termios);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "tcsetattr");

  वापस 0;
}

पूर्णांक
libcrun_set_stdio (पूर्णांक ptmx_fd, अक्षर *pty, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  अगर (fd < 0 && (त्रुटि_सं == EINVAL || त्रुटि_सं == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = खोलो (pty, O_RDWR | O_CLOEXEC);
    }
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", pty);

  क्रम (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "ioctl TIOCSCTTY");

  वापस 0;
}

व्योम
cleanup_terminalp (व्योम *p)
{
  काष्ठा terminal_status_s **s = (काष्ठा terminal_status_s **) p;
  अगर (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      मुक्त (*s);
    }
}

पूर्णांक
libcrun_terminal_setup_आकार (पूर्णांक fd, अचिन्हित लघु rows, अचिन्हित लघु cols, libcrun_error_t *err)
{
  काष्ठा winsize ws = { .ws_row = rows, .ws_col = cols };
  पूर्णांक ret;

  अगर (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      अगर (UNLIKELY (ret < 0))
        {
          अगर (त्रुटि_सं == ENOTTY)
            वापस 0;
          वापस crun_make_error (err, त्रुटि_सं, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "ioctl TIOCSWINSZ");
  वापस 0;
}
