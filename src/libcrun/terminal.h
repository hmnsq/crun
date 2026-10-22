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
#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "container.h"
#അന്തര്ഭാവ <termios.h>

ശൂന്യം cleanup_terminalp (ശൂന്യം *p);
#define cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

പൂർണ്ണസംഖ്യ libcrun_new_terminal (അക്ഷരം **pty, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_set_stdio (പൂർണ്ണസംഖ്യ ptmx_fd, അക്ഷരം *pty, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_set_raw (പൂർണ്ണസംഖ്യ fd, ശൂന്യം **current_status, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_clear_onlcr (പൂർണ്ണസംഖ്യ fd, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_terminal_setup_size (പൂർണ്ണസംഖ്യ fd, unsigned short rows, unsigned short cols, libcrun_error_t *err);

#endif
