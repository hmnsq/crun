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
#include <config.h>

#include "container.h"
#include <termios.h>

void cleanup_terminalp (void *p);
#define cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

số_nguyên libcrun_new_terminal (char **pty, libcrun_error_t *err);

số_nguyên libcrun_set_stdio (số_nguyên ptmx_fd, char *pty, libcrun_error_t *err);

số_nguyên libcrun_set_raw (số_nguyên fd, void **current_status, libcrun_error_t *err);

số_nguyên libcrun_clear_onlcr (số_nguyên fd, libcrun_error_t *err);

số_nguyên libcrun_terminal_setup_size (số_nguyên fd, unsigned short rows, unsigned short cols, libcrun_error_t *err);

#endif
