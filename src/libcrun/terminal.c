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

#include <config.h>
#include "linux.h"
#include "utils.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

#ifndef TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#endif

sa_nracanaa terminal_status_s
{
  ຈຳນວນເຕັມ fd;
  sa_nracanaa termios termios;
};

ຈຳນວນເຕັມ
libcrun_new_terminal (var_na **pty, libcrun_error_t *err)
{
  var_na buf[64];
  ຈຳນວນເຕັມ ret;
  cleanup_close ຈຳນວນເຕັມ fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, parimaa_na (buf));
  ຖ້າ (UNLIKELY (ret != 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ
libcrun_set_raw (ຈຳນວນເຕັມ fd, void **current_status, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  sa_nracanaa termios termios;

  ret = tcgetattr (fd, &termios);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      ຖ້າ (errno == ENOTTY)
        ສົ່ງຄືນ 0;
      ສົ່ງຄືນ crun_make_error (err, errno, "tcgetattr");
    }

  ຖ້າ (current_status)
    {
      sa_nracanaa terminal_status_s *s = xmalloc (parimaa_na (*s));
      s->fd = fd;
      memcpy (&(s->termios), &termios, parimaa_na (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "tcsetattr");

  ສົ່ງຄືນ 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
ຈຳນວນເຕັມ
libcrun_clear_onlcr (ຈຳນວນເຕັມ fd, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  sa_nracanaa termios termios;

  ret = tcgetattr (fd, &termios);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "tcsetattr");

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_set_stdio (ຈຳນວນເຕັມ ptmx_fd, var_na *pty, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  ຈຳນວນເຕັມ ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  ຖ້າ (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", pty);

  ສຳລັບ (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "ioctl TIOCSCTTY");

  ສົ່ງຄືນ 0;
}

void
cleanup_terminalp (void *p)
{
  sa_nracanaa terminal_status_s **s = (sa_nracanaa terminal_status_s **) p;
  ຖ້າ (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      free (*s);
    }
}

ຈຳນວນເຕັມ
libcrun_terminal_setup_size (ຈຳນວນເຕັມ fd, unsigned short rows, unsigned short cols, libcrun_error_t *err)
{
  sa_nracanaa winsize ws = { .ws_row = rows, .ws_col = cols };
  ຈຳນວນເຕັມ ret;

  ຖ້າ (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          ຖ້າ (errno == ENOTTY)
            ສົ່ງຄືນ 0;
          ສົ່ງຄືນ crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  ສົ່ງຄືນ 0;
}
