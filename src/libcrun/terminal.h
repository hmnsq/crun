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
#ifndef TERMINAL_H
#define TERMINAL_H
#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "container.h"
#ଅନ୍ତର୍ଭାଵ <termios.h>

ଶୂନ୍ୟ cleanup_terminalp (ଶୂନ୍ୟ *p);
#define cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_new_terminal (ଅକ୍ଷର **pty, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_stdio (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ptmx_fd, ଅକ୍ଷର *pty, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_raw (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଶୂନ୍ୟ **current_status, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_clear_onlcr (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_terminal_setup_size (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, unsigned short rows, unsigned short cols, libcrun_error_t *err);

#endif
