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
  puur_naa_nka fd;
  sa_nracanaa termios termios;
};

puur_naa_nka
libcrun_new_terminal (var_na **pty, libcrun_error_t *err)
{
  var_na buf[64];
  puur_naa_nka ret;
  cleanup_close puur_naa_nka fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  ꦪꦼꦤ꧀ (UNLIKELY (fd < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, parimaa_na (buf));
  ꦪꦼꦤ꧀ (UNLIKELY (ret != 0))
    ꦮꦭꦶ crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  ꦮꦭꦶ ret;
}

puur_naa_nka
libcrun_set_raw (puur_naa_nka fd, void **current_status, libcrun_error_t *err)
{
  puur_naa_nka ret;
  sa_nracanaa termios termios;

  ret = tcgetattr (fd, &termios);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      ꦪꦼꦤ꧀ (errno == ENOTTY)
        ꦮꦭꦶ 0;
      ꦮꦭꦶ crun_make_error (err, errno, "tcgetattr");
    }

  ꦪꦼꦤ꧀ (current_status)
    {
      sa_nracanaa terminal_status_s *s = xmalloc (parimaa_na (*s));
      s->fd = fd;
      memcpy (&(s->termios), &termios, parimaa_na (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "tcsetattr");

  ꦮꦭꦶ 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
puur_naa_nka
libcrun_clear_onlcr (puur_naa_nka fd, libcrun_error_t *err)
{
  puur_naa_nka ret;
  sa_nracanaa termios termios;

  ret = tcgetattr (fd, &termios);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "tcsetattr");

  ꦮꦭꦶ 0;
}

puur_naa_nka
libcrun_set_stdio (puur_naa_nka ptmx_fd, var_na *pty, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka fd = -1;
  puur_naa_nka ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  ꦪꦼꦤ꧀ (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  ꦪꦼꦤ꧀ (UNLIKELY (fd < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "open `%s`", pty);

  ꦏꦁꦒꦺꦴ (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
        ꦮꦭꦶ crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "ioctl TIOCSCTTY");

  ꦮꦭꦶ 0;
}

void
cleanup_terminalp (void *p)
{
  sa_nracanaa terminal_status_s **s = (sa_nracanaa terminal_status_s **) p;
  ꦪꦼꦤ꧀ (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      free (*s);
    }
}

puur_naa_nka
libcrun_terminal_setup_size (puur_naa_nka fd, unsigned short rows, unsigned short cols, libcrun_error_t *err)
{
  sa_nracanaa winsize ws = { .ws_row = rows, .ws_col = cols };
  puur_naa_nka ret;

  ꦪꦼꦤ꧀ (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
        {
          ꦪꦼꦤ꧀ (errno == ENOTTY)
            ꦮꦭꦶ 0;
          ꦮꦭꦶ crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  ꦮꦭꦶ 0;
}
