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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "linux.h"
#অন্তর্ভাব "utils.h"

#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <sys/ioctl.h>
#অন্তর্ভাব <termios.h>

#ifndef TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#endif

কাঠামো terminal_status_s
{
  পূর্ণসংখ্যা fd;
  কাঠামো termios termios;
};

পূর্ণসংখ্যা
libcrun_new_terminal (অক্ষর **pty, libcrun_error_t *err)
{
  অক্ষর buf[64];
  পূর্ণসংখ্যা ret;
  cleanup_close পূর্ণসংখ্যা fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, আকার (buf));
  যদি (UNLIKELY (ret != 0))
    ফেরত crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  ফেরত ret;
}

পূর্ণসংখ্যা
libcrun_set_raw (পূর্ণসংখ্যা fd, শূন্য **current_status, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  কাঠামো termios termios;

  ret = tcgetattr (fd, &termios);
  যদি (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      যদি (errno == ENOTTY)
        ফেরত 0;
      ফেরত crun_make_error (err, errno, "tcgetattr");
    }

  যদি (current_status)
    {
      কাঠামো terminal_status_s *s = xmalloc (আকার (*s));
      s->fd = fd;
      memcpy (&(s->termios), &termios, আকার (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "tcsetattr");

  ফেরত 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
পূর্ণসংখ্যা
libcrun_clear_onlcr (পূর্ণসংখ্যা fd, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  কাঠামো termios termios;

  ret = tcgetattr (fd, &termios);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "tcsetattr");

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_set_stdio (পূর্ণসংখ্যা ptmx_fd, অক্ষর *pty, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা fd = -1;
  পূর্ণসংখ্যা ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  যদি (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, errno, "open `%s`", pty);

  জন্য (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "ioctl TIOCSCTTY");

  ফেরত 0;
}

শূন্য
cleanup_terminalp (শূন্য *p)
{
  কাঠামো terminal_status_s **s = (কাঠামো terminal_status_s **) p;
  যদি (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      মুক্ত (*s);
    }
}

পূর্ণসংখ্যা
libcrun_terminal_setup_আকার (পূর্ণসংখ্যা fd, unsigned short rows, unsigned short cols, libcrun_error_t *err)
{
  কাঠামো winsize ws = { .ws_row = rows, .ws_col = cols };
  পূর্ণসংখ্যা ret;

  যদি (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (errno == ENOTTY)
            ফেরত 0;
          ফেরত crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  ফেরত 0;
}
