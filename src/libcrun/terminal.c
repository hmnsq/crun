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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "linux.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <sys/ioctl.h>
#ଅନ୍ତର୍ଭାଵ <termios.h>

#ifndef TIOCGPTPEER
#  define TIOCGPTPEER _IO ('T', 0x41)
#endif

ସଂରଚନା terminal_status_s
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
  ସଂରଚନା termios termios;
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_new_terminal (ଅକ୍ଷର **pty, libcrun_error_t *err)
{
  ଅକ୍ଷର buf[64];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open ("/dev/ptmx", O_RDWR | O_NOCTTY | O_CLOEXEC);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `/dev/ptmx`");

  ret = ptsname_r (fd, buf, ଆକାର (buf));
  ଯଦି (UNLIKELY (ret != 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "ptsname");

  ret = unlockpt (fd);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "unlockpt");

  *pty = xstrdup (buf);

  ret = fd;
  fd = -1;

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_raw (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଶୂନ୍ୟ **current_status, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ସଂରଚନା termios termios;

  ret = tcgetattr (fd, &termios);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* Nothing to do if FD is not a terminal.  */
      ଯଦି (errno == ENOTTY)
        ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ crun_make_error (err, errno, "tcgetattr");
    }

  ଯଦି (current_status)
    {
      ସଂରଚନା terminal_status_s *s = xmalloc (ଆକାର (*s));
      s->fd = fd;
      memcpy (&(s->termios), &termios, ଆକାର (termios));
      *current_status = s;
    }

  cfmakeraw (&termios);
  termios.c_iflag = 0;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "tcsetattr");

  ଫେରନ୍ତୁ 0;
}

/* Do not translate "\n" to "\r\n" on output, so that what the container
   writes to the terminal reaches our stdout unchanged.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_clear_onlcr (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ସଂରଚନା termios termios;

  ret = tcgetattr (fd, &termios);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "tcgetattr");

  termios.c_oflag &= ~ONLCR;

  ret = tcsetattr (fd, TCSANOW, &termios);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "tcsetattr");

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_stdio (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ptmx_fd, ଅକ୍ଷର *pty, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, i;

  /* Open the slave directly from the master.  The path is resolved in the
     container mount namespace, and could point anywhere.  */
  fd = ioctl (ptmx_fd, TIOCGPTPEER, O_RDWR | O_CLOEXEC);
  ଯଦି (fd < 0 && (errno == EINVAL || errno == ENOTTY))
    {
      /* TIOCGPTPEER requires Linux 4.13.  */
      fd = open (pty, O_RDWR | O_CLOEXEC);
    }
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", pty);

  ପାଇଁ (i = 0; i < 3; i++)
    {
      ret = dup3 (fd, i, 0);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "dup terminal");
    }

  ret = ioctl (0, TIOCSCTTY, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "ioctl TIOCSCTTY");

  ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ
cleanup_terminalp (ଶୂନ୍ୟ *p)
{
  ସଂରଚନା terminal_status_s **s = (ସଂରଚନା terminal_status_s **) p;
  ଯଦି (*s)
    {
      tcsetattr ((*s)->fd, TCSANOW, &(*s)->termios);
      ମୁକ୍ତ (*s);
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_terminal_setup_size (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, unsigned short rows, unsigned short cols, libcrun_error_t *err)
{
  ସଂରଚନା winsize ws = { .ws_row = rows, .ws_col = cols };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (ws.ws_row == 0 || ws.ws_col == 0)
    {
      ret = ioctl (0, TIOCGWINSZ, &ws);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (errno == ENOTTY)
            ଫେରନ୍ତୁ 0;
          ଫେରନ୍ତୁ crun_make_error (err, errno, "ioctl TIOCGWINSZ");
        }
    }

  ret = ioctl (fd, TIOCSWINSZ, &ws);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "ioctl TIOCSWINSZ");
  ଫେରନ୍ତୁ 0;
}
