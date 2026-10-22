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

#నిర్వచించు XOPEN_ఆధారం

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "linux.h"
#అన్తర్భావ "utils.h"

#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <sys/ioctl.h>
#అన్తర్భావ <termios.h>

#నిర్వచితం_కాకపోతే TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#అయితే_ముగింపు

నిర్మాణం terminal_స్థితి_s
{
  పూర్ణాంకం fd;
  నిర్మాణం termios termios;
};

పూర్ణాంకం
libcrun_new_terminal (అక్షరం **pty, libcrun_error_t *err)
{
  అక్షరం buf[64];
  పూర్ణాంకం ret;
  cleanup_close పూర్ణాంకం fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, పరిమాణం (buf));
  ఒకవేళ (UNLIKELY (ret != 0))
    తిరిగి crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  తిరిగి ret;
}

పూర్ణాంకం
libcrun_సమితి_raw (పూర్ణాంకం fd, శూన్య **current_స్థితి, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  నిర్మాణం termios termios;

  ret = tcgetattr (fd, &termios);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      ఒకవేళ (errno == ENOTTY)
        తిరిగి 0;
      తిరిగి crun_make_error (err, errno, "tcgetattr");
    }

  ఒకవేళ (current_స్థితి)
    {
      నిర్మాణం terminal_స్థితి_s *s = xmalloc (పరిమాణం (*s));
      s->fd = fd;
      జ్ఞాపకం_నకలు (&(s->termios), &termios, పరిమాణం (termios));
      *current_స్థితి = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "tcsetattr");

  తిరిగి 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
పూర్ణాంకం
libcrun_clear_onlcr (పూర్ణాంకం fd, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  నిర్మాణం termios termios;

  ret = tcgetattr (fd, &termios);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "tcsetattr");

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_సమితి_stdio (పూర్ణాంకం ptmx_fd, అక్షరం *pty, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం fd = -1;
  పూర్ణాంకం ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  ఒకవేళ (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", pty);

  కొరకు (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "ioctl TIOCSCTTY");

  తిరిగి 0;
}

శూన్య
cleanup_terminalp (శూన్య *p)
{
  నిర్మాణం terminal_స్థితి_s **s = (నిర్మాణం terminal_స్థితి_s **) p;
  ఒకవేళ (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      విడుదల (*s);
    }
}

పూర్ణాంకం
libcrun_terminal_అమరిక_కొలత (పూర్ణాంకం fd, గుర్తులేని పొట్టి అడ్డువరుసలు, గుర్తులేని పొట్టి నిలువుల_సంఖ్య, libcrun_error_t *err)
{
  నిర్మాణం winsize ws = { .ws_row = అడ్డువరుసలు, .ws_col = నిలువుల_సంఖ్య };
  పూర్ణాంకం ret;

  ఒకవేళ (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (errno == ENOTTY)
            తిరిగి 0;
          తిరిగి crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  తిరిగి 0;
}
