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

#define _XOPEN_SOURCE

#define _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव "linux.h"
#अन्तर्भाव "utils.h"

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <sys/ioctl.h>
#अन्तर्भाव <termios.h>

#ifndef TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#endif

संरचना terminal_status_s
{
  पूर्णाङ्क fd;
  संरचना termios termios;
};

पूर्णाङ्क
libcrun_new_terminal (वर्ण **pty, libcrun_error_t *err)
{
  वर्ण buf[64];
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क fd = खोलो ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, त्रुटि_सं, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, परिमाण (buf));
  यदि (UNLIKELY (ret != 0))
    प्रत्यागम crun_make_error (err, त्रुटि_सं, "ptsname");

  ret = unlockpt (fd);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, त्रुटि_सं, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_set_raw (पूर्णाङ्क fd, शून्य **current_status, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  संरचना termios termios;

  ret = tcgetattr (fd, &termios);
  यदि (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      यदि (त्रुटि_सं == ENOTTY)
        प्रत्यागम 0;
      प्रत्यागम crun_make_error (err, त्रुटि_सं, "tcgetattr");
    }

  यदि (current_status)
    {
      संरचना terminal_status_s *s = xmalloc (परिमाण (*s));
      s->fd = fd;
      स_नकल (&(s->termios), &termios, परिमाण (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, त्रुटि_सं, "tcsetattr");

  प्रत्यागम 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
पूर्णाङ्क
libcrun_clear_onlcr (पूर्णाङ्क fd, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  संरचना termios termios;

  ret = tcgetattr (fd, &termios);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, त्रुटि_सं, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, त्रुटि_सं, "tcsetattr");

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_set_stdio (पूर्णाङ्क ptmx_fd, वर्ण *pty, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  यदि (fd < 0 && (त्रुटि_सं == EINVAL || त्रुटि_सं == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = खोलो (pty, O_RDWR | O_CLOEXEC);
    }
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, त्रुटि_सं, "open `%s`", pty);

  प्रति (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, त्रुटि_सं, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, त्रुटि_सं, "ioctl TIOCSCTTY");

  प्रत्यागम 0;
}

शून्य
cleanup_terminalp (शून्य *p)
{
  संरचना terminal_status_s **s = (संरचना terminal_status_s **) p;
  यदि (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      मुक्त (*s);
    }
}

पूर्णाङ्क
libcrun_terminal_setup_size (पूर्णाङ्क fd, अचिन्हित लघु rows, अचिन्हित लघु cols, libcrun_error_t *err)
{
  संरचना winsize ws = { .ws_row = rows, .ws_col = cols };
  पूर्णाङ्क ret;

  यदि (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (त्रुटि_सं == ENOTTY)
            प्रत्यागम 0;
          प्रत्यागम crun_make_error (err, त्रुटि_सं, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, त्रुटि_सं, "ioctl TIOCSWINSZ");
  प्रत्यागम 0;
}
