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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "linux.h"
#અન્તર્ભાવ "utils.h"

#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <sys/ioctl.h>
#અન્તર્ભાવ <termios.h>

#ifndef TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#endif

સંરચના terminal_status_s
{
  પૂર્ણાંક fd;
  સંરચના termios termios;
};

પૂર્ણાંક
libcrun_new_terminal (અક્ષર **pty, libcrun_error_t *err)
{
  અક્ષર buf[64];
  પૂર્ણાંક ret;
  cleanup_close પૂર્ણાંક fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, કદ (buf));
  જો (UNLIKELY (ret != 0))
    પાછા crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  પાછા ret;
}

પૂર્ણાંક
libcrun_set_raw (પૂર્ણાંક fd, શૂન્ય **current_status, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  સંરચના termios termios;

  ret = tcgetattr (fd, &termios);
  જો (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      જો (errno == ENOTTY)
        પાછા 0;
      પાછા crun_make_error (err, errno, "tcgetattr");
    }

  જો (current_status)
    {
      સંરચના terminal_status_s *s = xmalloc (કદ (*s));
      s->fd = fd;
      memcpy (&(s->termios), &termios, કદ (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "tcsetattr");

  પાછા 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
પૂર્ણાંક
libcrun_clear_onlcr (પૂર્ણાંક fd, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  સંરચના termios termios;

  ret = tcgetattr (fd, &termios);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "tcsetattr");

  પાછા 0;
}

પૂર્ણાંક
libcrun_set_stdio (પૂર્ણાંક ptmx_fd, અક્ષર *pty, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક fd = -1;
  પૂર્ણાંક ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  જો (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", pty);

  માટે (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "ioctl TIOCSCTTY");

  પાછા 0;
}

શૂન્ય
cleanup_terminalp (શૂન્ય *p)
{
  સંરચના terminal_status_s **s = (સંરચના terminal_status_s **) p;
  જો (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      મુક્ત (*s);
    }
}

પૂર્ણાંક
libcrun_terminal_setup_size (પૂર્ણાંક fd, unsigned short rows, unsigned short cols, libcrun_error_t *err)
{
  સંરચના winsize ws = { .ws_row = rows, .ws_col = cols };
  પૂર્ણાંક ret;

  જો (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      જો (UNLIKELY (ret < 0))
        {
          જો (errno == ENOTTY)
            પાછા 0;
          પાછા crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  પાછા 0;
}
