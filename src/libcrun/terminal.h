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
#شامل <config.h>

#شامل "container.h"
#شامل <termios.h>

تهی cleanup_terminalp (تهی *p);
#define cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

صحیح libcrun_new_terminal (نویسه **pty, libcrun_error_t *err);

صحیح libcrun_set_stdio (صحیح ptmx_fd, نویسه *pty, libcrun_error_t *err);

صحیح libcrun_set_raw (صحیح fd, تهی **current_status, libcrun_error_t *err);

صحیح libcrun_clear_onlcr (صحیح fd, libcrun_error_t *err);

صحیح libcrun_terminal_setup_size (صحیح fd, unsigned short rows, unsigned short cols, libcrun_error_t *err);

#endif
