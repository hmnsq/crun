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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "linux.h"
#അന്തര്ഭാവ "utils.h"

#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <sys/ioctl.h>
#അന്തര്ഭാവ <termios.h>

#ifndef TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#endif

ഘടന terminal_status_s
{
  പൂർണ്ണസംഖ്യ fd;
  ഘടന termios termios;
};

പൂർണ്ണസംഖ്യ
libcrun_new_terminal (അക്ഷരം **pty, libcrun_error_t *err)
{
  അക്ഷരം buf[64];
  പൂർണ്ണസംഖ്യ ret;
  cleanup_close പൂർണ്ണസംഖ്യ fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, വലുപ്പം (buf));
  എങ്കിൽ (UNLIKELY (ret != 0))
    തിരികെ crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_set_raw (പൂർണ്ണസംഖ്യ fd, ശൂന്യം **current_status, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  ഘടന termios termios;

  ret = tcgetattr (fd, &termios);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      എങ്കിൽ (errno == ENOTTY)
        തിരികെ 0;
      തിരികെ crun_make_error (err, errno, "tcgetattr");
    }

  എങ്കിൽ (current_status)
    {
      ഘടന terminal_status_s *s = xmalloc (വലുപ്പം (*s));
      s->fd = fd;
      memcpy (&(s->termios), &termios, വലുപ്പം (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "tcsetattr");

  തിരികെ 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
പൂർണ്ണസംഖ്യ
libcrun_clear_onlcr (പൂർണ്ണസംഖ്യ fd, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  ഘടന termios termios;

  ret = tcgetattr (fd, &termios);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "tcsetattr");

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_set_stdio (പൂർണ്ണസംഖ്യ ptmx_fd, അക്ഷരം *pty, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  പൂർണ്ണസംഖ്യ ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  എങ്കിൽ (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", pty);

  വേണ്ടി (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "ioctl TIOCSCTTY");

  തിരികെ 0;
}

ശൂന്യം
cleanup_terminalp (ശൂന്യം *p)
{
  ഘടന terminal_status_s **s = (ഘടന terminal_status_s **) p;
  എങ്കിൽ (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      സ്വതന്ത്രം (*s);
    }
}

പൂർണ്ണസംഖ്യ
libcrun_terminal_setup_size (പൂർണ്ണസംഖ്യ fd, unsigned short rows, unsigned short cols, libcrun_error_t *err)
{
  ഘടന winsize ws = { .ws_row = rows, .ws_col = cols };
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (errno == ENOTTY)
            തിരികെ 0;
          തിരികെ crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  തിരികെ 0;
}
