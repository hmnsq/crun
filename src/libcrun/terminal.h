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
#यदि_अपरिभाषितम् TERMINAL_H
#परिभाषय TERMINAL_H
#अन्तर्भाव <config.h>

#अन्तर्भाव "container.h"
#अन्तर्भाव <termios.h>

शून्य cleanup_terminalp (शून्य *p);
#परिभाषय cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

पूर्णाङ्क libcrun_new_terminal (वर्ण **pty, libcrun_error_t *err);

पूर्णाङ्क libcrun_समुच्चयः_stdio (पूर्णाङ्क ptmx_fd, वर्ण *pty, libcrun_error_t *err);

पूर्णाङ्क libcrun_समुच्चयः_raw (पूर्णाङ्क fd, शून्य **current_स्थितिः, libcrun_error_t *err);

पूर्णाङ्क libcrun_clear_onlcr (पूर्णाङ्क fd, libcrun_error_t *err);

पूर्णाङ्क libcrun_terminal_सज्जता_आकार (पूर्णाङ्क fd, अचिह्नित ह्रस्व पङ्क्तिकाः, अचिह्नित ह्रस्व स्तम्भसङ्ख्या, libcrun_error_t *err);

#यदि_अन्तः
