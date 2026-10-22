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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XOPEN_ਸਰੋਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/ioctl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <termios.h>

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#ਜੇ_ਅੰਤ

ਬਣਤਰ terminal_ਸਥਿਤੀ_s
{
  ਪੂਰਨ fd;
  ਬਣਤਰ termios termios;
};

ਪੂਰਨ
libcrun_new_terminal (ਅੱਖਰ **pty, libcrun_error_t *err)
{
  ਅੱਖਰ buf[64];
  ਪੂਰਨ ret;
  cleanup_close ਪੂਰਨ fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, ਆਕਾਰ (buf));
  ਜੇ (UNLIKELY (ret != 0))
    ਵਾਪਸ crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_raw (ਪੂਰਨ fd, ਖਾਲੀ **current_ਸਥਿਤੀ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਬਣਤਰ termios termios;

  ret = tcgetattr (fd, &termios);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      ਜੇ (errno == ENOTTY)
        ਵਾਪਸ 0;
      ਵਾਪਸ crun_make_error (err, errno, "tcgetattr");
    }

  ਜੇ (current_ਸਥਿਤੀ)
    {
      ਬਣਤਰ terminal_ਸਥਿਤੀ_s *s = xmalloc (ਆਕਾਰ (*s));
      s->fd = fd;
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (&(s->termios), &termios, ਆਕਾਰ (termios));
      *current_ਸਥਿਤੀ = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "tcsetattr");

  ਵਾਪਸ 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
ਪੂਰਨ
libcrun_clear_onlcr (ਪੂਰਨ fd, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਬਣਤਰ termios termios;

  ret = tcgetattr (fd, &termios);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "tcsetattr");

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_stdio (ਪੂਰਨ ptmx_fd, ਅੱਖਰ *pty, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ fd = -1;
  ਪੂਰਨ ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  ਜੇ (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", pty);

  ਲਈ (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "ioctl TIOCSCTTY");

  ਵਾਪਸ 0;
}

ਖਾਲੀ
cleanup_terminalp (ਖਾਲੀ *p)
{
  ਬਣਤਰ terminal_ਸਥਿਤੀ_s **s = (ਬਣਤਰ terminal_ਸਥਿਤੀ_s **) p;
  ਜੇ (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      ਮੁਕਤ (*s);
    }
}

ਪੂਰਨ
libcrun_terminal_ਤਿਆਰੀ_ਮਾਪ (ਪੂਰਨ fd, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਪੰਕਤੀਆਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਖਾਨੇ_ਗਿਣਤੀ, libcrun_error_t *err)
{
  ਬਣਤਰ winsize ws = { .ws_row = ਪੰਕਤੀਆਂ, .ws_col = ਖਾਨੇ_ਗਿਣਤੀ };
  ਪੂਰਨ ret;

  ਜੇ (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (errno == ENOTTY)
            ਵਾਪਸ 0;
          ਵਾਪਸ crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  ਵਾਪਸ 0;
}
