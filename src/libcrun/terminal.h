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
#अन्तर्भाव <config.h>

#अन्तर्भाव "container.h"
#अन्तर्भाव <termios.h>

शून्य cleanup_terminalp (शून्य *p);
#define cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

पूर्णांक libcrun_new_terminal (अक्षर **pty, libcrun_error_t *err);

पूर्णांक libcrun_set_stdio (पूर्णांक ptmx_fd, अक्षर *pty, libcrun_error_t *err);

पूर्णांक libcrun_set_raw (पूर्णांक fd, शून्य **current_status, libcrun_error_t *err);

पूर्णांक libcrun_clear_onlcr (पूर्णांक fd, libcrun_error_t *err);

पूर्णांक libcrun_terminal_setup_size (पूर्णांक fd, अचिन्हित लघु rows, अचिन्हित लघु cols, libcrun_error_t *err);

#endif
