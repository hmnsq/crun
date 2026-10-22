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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಅನ್ತರ್ಭಾವ <termios.h>

#ifndef TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#endif

ಸಂರಚನಾ terminal_status_s
{
  ಪೂರ್ಣಾಙ್ಕ fd;
  ಸಂರಚನಾ termios termios;
};

ಪೂರ್ಣಾಙ್ಕ
libcrun_new_terminal (ವರ್ಣ **pty, libcrun_error_t *err)
{
  ವರ್ಣ buf[64];
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, ಪರಿಮಾಣ (buf));
  ಯದಿ (UNLIKELY (ret != 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_raw (ಪೂರ್ಣಾಙ್ಕ fd, ಶೂನ್ಯ **current_status, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಸಂರಚನಾ termios termios;

  ret = tcgetattr (fd, &termios);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      ಯದಿ (errno == ENOTTY)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "tcgetattr");
    }

  ಯದಿ (current_status)
    {
      ಸಂರಚನಾ terminal_status_s *s = xmalloc (ಪರಿಮಾಣ (*s));
      s->fd = fd;
      memcpy (&(s->termios), &termios, ಪರಿಮಾಣ (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "tcsetattr");

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
ಪೂರ್ಣಾಙ್ಕ
libcrun_clear_onlcr (ಪೂರ್ಣಾಙ್ಕ fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಸಂರಚನಾ termios termios;

  ret = tcgetattr (fd, &termios);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "tcsetattr");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_stdio (ಪೂರ್ಣಾಙ್ಕ ptmx_fd, ವರ್ಣ *pty, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  ಯದಿ (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", pty);

  ಪ್ರತಿ (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "ioctl TIOCSCTTY");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ
cleanup_terminalp (ಶೂನ್ಯ *p)
{
  ಸಂರಚನಾ terminal_status_s **s = (ಸಂರಚನಾ terminal_status_s **) p;
  ಯದಿ (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      ಮುಕ್ತ (*s);
    }
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_terminal_setup_size (ಪೂರ್ಣಾಙ್ಕ fd, unsigned short rows, unsigned short cols, libcrun_error_t *err)
{
  ಸಂರಚನಾ winsize ws = { .ws_row = rows, .ws_col = cols };
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (errno == ENOTTY)
            ಪ್ರತ್ಯಾಗಮ 0;
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  ಪ್ರತ್ಯಾಗಮ 0;
}
