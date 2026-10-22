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

#ضمن <config.h>
#ضمن "linux.h"
#ضمن "utils.h"

#ضمن <stdlib.h>
#ضمن <unistd.h>
#ضمن <fcntl.h>
#ضمن <sys/ioctl.h>
#ضمن <termios.h>

#ifndef TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#endif

بنية terminal_status_s
{
  صحيح fd;
  بنية termios termios;
};

صحيح
libcrun_new_terminal (حرف **pty, libcrun_error_t *err)
{
  حرف buf[64];
  صحيح ret;
  cleanup_close صحيح fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  𐤀𐤌 (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, حجم (buf));
  𐤀𐤌 (UNLIKELY (ret != 0))
    أرجع crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  𐤀𐤌 (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  أرجع ret;
}

صحيح
libcrun_set_raw (صحيح fd, فراغ **current_status, libcrun_error_t *err)
{
  صحيح ret;
  بنية termios termios;

  ret = tcgetattr (fd, &termios);
  𐤀𐤌 (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      𐤀𐤌 (errno == ENOTTY)
        أرجع 0;
      أرجع crun_make_error (err, errno, "tcgetattr");
    }

  𐤀𐤌 (current_status)
    {
      بنية terminal_status_s *s = xmalloc (حجم (*s));
      s->fd = fd;
      memcpy (&(s->termios), &termios, حجم (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  𐤀𐤌 (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "tcsetattr");

  أرجع 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
صحيح
libcrun_clear_onlcr (صحيح fd, libcrun_error_t *err)
{
  صحيح ret;
  بنية termios termios;

  ret = tcgetattr (fd, &termios);
  𐤀𐤌 (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  𐤀𐤌 (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "tcsetattr");

  أرجع 0;
}

صحيح
libcrun_set_stdio (صحيح ptmx_fd, حرف *pty, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  صحيح ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  𐤀𐤌 (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  𐤀𐤌 (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", pty);

  لكل (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      𐤀𐤌 (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  𐤀𐤌 (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "ioctl TIOCSCTTY");

  أرجع 0;
}

فراغ
cleanup_terminalp (فراغ *p)
{
  بنية terminal_status_s **s = (بنية terminal_status_s **) p;
  𐤀𐤌 (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      حرر (*s);
    }
}

صحيح
libcrun_terminal_setup_size (صحيح fd, unsigned short rows, unsigned short cols, libcrun_error_t *err)
{
  بنية winsize ws = { .ws_row = rows, .ws_col = cols };
  صحيح ret;

  𐤀𐤌 (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      𐤀𐤌 (UNLIKELY (ret < 0))
        {
          𐤀𐤌 (errno == ENOTTY)
            أرجع 0;
          أرجع crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  𐤀𐤌 (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  أرجع 0;
}
