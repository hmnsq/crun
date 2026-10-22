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
#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ "container.h"
#অন্তর্ভাৱ <termios.h>

শূন্য cleanup_terminalp (শূন্য *p);
#define cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

পূর্ণাঙ্ক libcrun_new_terminal (ৱর্ণ **pty, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_set_stdio (পূর্ণাঙ্ক ptmx_fd, ৱর্ণ *pty, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_set_raw (পূর্ণাঙ্ক fd, শূন্য **current_status, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_clear_onlcr (পূর্ণাঙ্ক fd, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_terminal_setup_size (পূর্ণাঙ্ক fd, unsigned short rows, unsigned short cols, libcrun_error_t *err);

#endif
