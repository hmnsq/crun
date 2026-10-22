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
  целое fd;
  struct termios termios;
};

целое
libcrun_new_terminal (char **pty, libcrun_error_t *err)
{
  char buf[64];
  целое ret;
  cleanup_close целое fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  если (UNLIKELY (fd < 0))
    вернуть crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, sizeof (buf));
  если (UNLIKELY (ret != 0))
    вернуть crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  вернуть ret;
}

целое
libcrun_set_raw (целое fd, void **current_status, libcrun_error_t *err)
{
  целое ret;
  struct termios termios;

  ret = tcgetattr (fd, &termios);
  если (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      если (errno == ENOTTY)
        вернуть 0;
      вернуть crun_make_error (err, errno, "tcgetattr");
    }

  если (current_status)
    {
      struct terminal_status_s *s = xmalloc (sizeof (*s));
      s->fd = fd;
      memcpy (&(s->termios), &termios, sizeof (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "tcsetattr");

  вернуть 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
целое
libcrun_clear_onlcr (целое fd, libcrun_error_t *err)
{
  целое ret;
  struct termios termios;

  ret = tcgetattr (fd, &termios);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "tcsetattr");

  вернуть 0;
}

целое
libcrun_set_stdio (целое ptmx_fd, char *pty, libcrun_error_t *err)
{
  cleanup_close целое fd = -1;
  целое ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  если (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  если (UNLIKELY (fd < 0))
    вернуть crun_make_error (err, errno, "open `%s`", pty);

  для (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      если (UNLIKELY (ret < 0))
        вернуть crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "ioctl TIOCSCTTY");

  вернуть 0;
}

void
cleanup_terminalp (void *p)
{
  struct terminal_status_s **s = (struct terminal_status_s **) p;
  если (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      free (*s);
    }
}

целое
libcrun_terminal_setup_size (целое fd, unsigned short rows, unsigned short cols, libcrun_error_t *err)
{
  struct winsize ws = { .ws_row = rows, .ws_col = cols };
  целое ret;

  если (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      если (UNLIKELY (ret < 0))
        {
          если (errno == ENOTTY)
            вернуть 0;
          вернуть crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  вернуть 0;
}
