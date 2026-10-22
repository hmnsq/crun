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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "linux.h"
#அந்தர்பாவ "utils.h"

#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <sys/ioctl.h>
#அந்தர்பாவ <termios.h>

#ifndef TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#endif

கட்டமைப்பு terminal_status_s
{
  முழு fd;
  கட்டமைப்பு termios termios;
};

முழு
libcrun_new_terminal (எழுத்து **pty, libcrun_error_t *err)
{
  எழுத்து buf[64];
  முழு ret;
  cleanup_close முழு fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, அளவு (buf));
  எனில் (UNLIKELY (ret != 0))
    திரும்பு crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  திரும்பு ret;
}

முழு
libcrun_set_raw (முழு fd, வெறுமை **current_status, libcrun_error_t *err)
{
  முழு ret;
  கட்டமைப்பு termios termios;

  ret = tcgetattr (fd, &termios);
  எனில் (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      எனில் (errno == ENOTTY)
        திரும்பு 0;
      திரும்பு crun_make_error (err, errno, "tcgetattr");
    }

  எனில் (current_status)
    {
      கட்டமைப்பு terminal_status_s *s = xmalloc (அளவு (*s));
      s->fd = fd;
      memcpy (&(s->termios), &termios, அளவு (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "tcsetattr");

  திரும்பு 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
முழு
libcrun_clear_onlcr (முழு fd, libcrun_error_t *err)
{
  முழு ret;
  கட்டமைப்பு termios termios;

  ret = tcgetattr (fd, &termios);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "tcsetattr");

  திரும்பு 0;
}

முழு
libcrun_set_stdio (முழு ptmx_fd, எழுத்து *pty, libcrun_error_t *err)
{
  cleanup_close முழு fd = -1;
  முழு ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  எனில் (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", pty);

  க்கு (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "ioctl TIOCSCTTY");

  திரும்பு 0;
}

வெறுமை
cleanup_terminalp (வெறுமை *p)
{
  கட்டமைப்பு terminal_status_s **s = (கட்டமைப்பு terminal_status_s **) p;
  எனில் (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      விடுவி (*s);
    }
}

முழு
libcrun_terminal_setup_size (முழு fd, unsigned short rows, unsigned short cols, libcrun_error_t *err)
{
  கட்டமைப்பு winsize ws = { .ws_row = rows, .ws_col = cols };
  முழு ret;

  எனில் (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (errno == ENOTTY)
            திரும்பு 0;
          திரும்பு crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  திரும்பு 0;
}
