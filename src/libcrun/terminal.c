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

#تعریف_کرو XOPEN_ماخذ

#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل "linux.h"
#شامل "utils.h"

#شامل <stdlib.h>
#شامل <unistd.h>
#شامل <fcntl.h>
#شامل <sys/ioctl.h>
#شامل <termios.h>

#اگر_غیر_تعریف_شدہ TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#اگر_ختم

ساخت terminal_حالت_s
{
  صحیح fd;
  ساخت termios termios;
};

صحیح
libcrun_new_terminal (حرف **pty, libcrun_error_t *err)
{
  حرف buf[64];
  صحیح ret;
  cleanup_close صحیح fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, حجم (buf));
  اگر (UNLIKELY (ret != 0))
    واپس crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  واپس ret;
}

صحیح
libcrun_سیٹ_raw (صحیح fd, خالی **current_حالت, libcrun_error_t *err)
{
  صحیح ret;
  ساخت termios termios;

  ret = tcgetattr (fd, &termios);
  اگر (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      اگر (errno == ENOTTY)
        واپس 0;
      واپس crun_make_error (err, errno, "tcgetattr");
    }

  اگر (current_حالت)
    {
      ساخت terminal_حالت_s *s = xmalloc (حجم (*s));
      s->fd = fd;
      حافظہ_نقل (&(s->termios), &termios, حجم (termios));
      *current_حالت = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "tcsetattr");

  واپس 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
صحیح
libcrun_clear_onlcr (صحیح fd, libcrun_error_t *err)
{
  صحیح ret;
  ساخت termios termios;

  ret = tcgetattr (fd, &termios);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "tcsetattr");

  واپس 0;
}

صحیح
libcrun_سیٹ_stdio (صحیح ptmx_fd, حرف *pty, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  صحیح ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  اگر (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "open `%s`", pty);

  برائے (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "ioctl TIOCSCTTY");

  واپس 0;
}

خالی
cleanup_terminalp (خالی *p)
{
  ساخت terminal_حالت_s **s = (ساخت terminal_حالت_s **) p;
  اگر (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      آزاد (*s);
    }
}

صحیح
libcrun_terminal_تیاری_سائز (صحیح fd, غیرعلامتی مختصر صفیں, غیرعلامتی مختصر ستون_تعداد, libcrun_error_t *err)
{
  ساخت winsize ws = { .ws_row = صفیں, .ws_col = ستون_تعداد };
  صحیح ret;

  اگر (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (errno == ENOTTY)
            واپس 0;
          واپس crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  واپس 0;
}
