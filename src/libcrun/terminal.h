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
#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ <termios.h>

ಶೂನ್ಯ cleanup_terminalp (ಶೂನ್ಯ *p);
#define cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

ಪೂರ್ಣಾಙ್ಕ libcrun_new_terminal (ವರ್ಣ **pty, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_set_stdio (ಪೂರ್ಣಾಙ್ಕ ptmx_fd, ವರ್ಣ *pty, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_set_raw (ಪೂರ್ಣಾಙ್ಕ fd, ಶೂನ್ಯ **current_status, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_clear_onlcr (ಪೂರ್ಣಾಙ್ಕ fd, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_terminal_setup_size (ಪೂರ್ಣಾಙ್ಕ fd, unsigned short rows, unsigned short cols, libcrun_error_t *err);

#endif
