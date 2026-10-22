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
#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "container.h"
#અન્તર્ભાવ <termios.h>

શૂન્ય cleanup_terminalp (શૂન્ય *p);
#define cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

પૂર્ણાંક libcrun_new_terminal (અક્ષર **pty, libcrun_error_t *err);

પૂર્ણાંક libcrun_set_stdio (પૂર્ણાંક ptmx_fd, અક્ષર *pty, libcrun_error_t *err);

પૂર્ણાંક libcrun_set_raw (પૂર્ણાંક fd, શૂન્ય **current_status, libcrun_error_t *err);

પૂર્ણાંક libcrun_clear_onlcr (પૂર્ણાંક fd, libcrun_error_t *err);

પૂર્ણાંક libcrun_terminal_setup_size (પૂર્ણાંક fd, unsigned short rows, unsigned short cols, libcrun_error_t *err);

#endif
