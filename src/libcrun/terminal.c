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

#ವ್ಯಾಖ್ಯಾನಿಸು XOPEN_ಮೂಲ

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಅನ್ತರ್ಭಾವ <termios.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ terminal_ಸ್ಥಿತಿ_s
{
  ಪೂರ್ಣಾಂಕ fd;
  ರಚನೆ termios termios;
};

ಪೂರ್ಣಾಂಕ
libcrun_new_terminal (ಅಕ್ಷರ **pty, libcrun_error_t *err)
{
  ಅಕ್ಷರ buf[64];
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, ಗಾತ್ರ (buf));
  ಆದರೆ (UNLIKELY (ret != 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_raw (ಪೂರ್ಣಾಂಕ fd, ಶೂನ್ಯ **current_ಸ್ಥಿತಿ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ರಚನೆ termios termios;

  ret = tcgetattr (fd, &termios);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      ಆದರೆ (errno == ENOTTY)
        ಹಿಂತಿರುಗಿ 0;
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "tcgetattr");
    }

  ಆದರೆ (current_ಸ್ಥಿತಿ)
    {
      ರಚನೆ terminal_ಸ್ಥಿತಿ_s *s = xmalloc (ಗಾತ್ರ (*s));
      s->fd = fd;
      ಸ್ಮರಣೆ_ನಕಲು (&(s->termios), &termios, ಗಾತ್ರ (termios));
      *current_ಸ್ಥಿತಿ = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "tcsetattr");

  ಹಿಂತಿರುಗಿ 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
ಪೂರ್ಣಾಂಕ
libcrun_clear_onlcr (ಪೂರ್ಣಾಂಕ fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ರಚನೆ termios termios;

  ret = tcgetattr (fd, &termios);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "tcsetattr");

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_stdio (ಪೂರ್ಣಾಂಕ ptmx_fd, ಅಕ್ಷರ *pty, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಪೂರ್ಣಾಂಕ ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  ಆದರೆ (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", pty);

  ಗಾಗಿ (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "ioctl TIOCSCTTY");

  ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ
cleanup_terminalp (ಶೂನ್ಯ *p)
{
  ರಚನೆ terminal_ಸ್ಥಿತಿ_s **s = (ರಚನೆ terminal_ಸ್ಥಿತಿ_s **) p;
  ಆದರೆ (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      ಬಿಡುಗಡೆ (*s);
    }
}

ಪೂರ್ಣಾಂಕ
libcrun_terminal_ಸಿದ್ಧತೆ_ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ fd, ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ಅಡ್ಡಸಾಲುಗಳು, ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, libcrun_error_t *err)
{
  ರಚನೆ winsize ws = { .ws_row = ಅಡ್ಡಸಾಲುಗಳು, .ws_col = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ };
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (errno == ENOTTY)
            ಹಿಂತಿರುಗಿ 0;
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  ಹಿಂತಿರುಗಿ 0;
}
