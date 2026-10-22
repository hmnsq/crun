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
#அந்தர்பாவ <config.h>

#அந்தர்பாவ "container.h"
#அந்தர்பாவ <termios.h>

வெறுமை cleanup_terminalp (வெறுமை *p);
#define cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

முழு libcrun_new_terminal (எழுத்து **pty, libcrun_error_t *err);

முழு libcrun_set_stdio (முழு ptmx_fd, எழுத்து *pty, libcrun_error_t *err);

முழு libcrun_set_raw (முழு fd, வெறுமை **current_status, libcrun_error_t *err);

முழு libcrun_clear_onlcr (முழு fd, libcrun_error_t *err);

முழு libcrun_terminal_setup_size (முழு fd, unsigned short rows, unsigned short cols, libcrun_error_t *err);

#endif
