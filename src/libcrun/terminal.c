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

struct terminal_status_s
{
  số_nguyên fd;
  struct termios termios;
};

số_nguyên
libcrun_new_terminal (char **pty, libcrun_error_t *err)
{
  char buf[64];
  số_nguyên ret;
  cleanup_close số_nguyên fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  nếu (UNLIKELY (fd < 0))
    trả_về crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, sizeof (buf));
  nếu (UNLIKELY (ret != 0))
    trả_về crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  trả_về ret;
}

số_nguyên
libcrun_set_raw (số_nguyên fd, void **current_status, libcrun_error_t *err)
{
  số_nguyên ret;
  struct termios termios;

  ret = tcgetattr (fd, &termios);
  nếu (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      nếu (errno == ENOTTY)
        trả_về 0;
      trả_về crun_make_error (err, errno, "tcgetattr");
    }

  nếu (current_status)
    {
      struct terminal_status_s *s = xmalloc (sizeof (*s));
      s->fd = fd;
      memcpy (&(s->termios), &termios, sizeof (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "tcsetattr");

  trả_về 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
số_nguyên
libcrun_clear_onlcr (số_nguyên fd, libcrun_error_t *err)
{
  số_nguyên ret;
  struct termios termios;

  ret = tcgetattr (fd, &termios);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "tcsetattr");

  trả_về 0;
}

số_nguyên
libcrun_set_stdio (số_nguyên ptmx_fd, char *pty, libcrun_error_t *err)
{
  cleanup_close số_nguyên fd = -1;
  số_nguyên ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  nếu (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  nếu (UNLIKELY (fd < 0))
    trả_về crun_make_error (err, errno, "open `%s`", pty);

  cho (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      nếu (UNLIKELY (ret < 0))
        trả_về crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "ioctl TIOCSCTTY");

  trả_về 0;
}

void
cleanup_terminalp (void *p)
{
  struct terminal_status_s **s = (struct terminal_status_s **) p;
  nếu (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      free (*s);
    }
}

số_nguyên
libcrun_terminal_setup_size (số_nguyên fd, unsigned short rows, unsigned short cols, libcrun_error_t *err)
{
  struct winsize ws = { .ws_row = rows, .ws_col = cols };
  số_nguyên ret;

  nếu (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      nếu (UNLIKELY (ret < 0))
        {
          nếu (errno == ENOTTY)
            trả_về 0;
          trả_về crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  trả_về 0;
}
