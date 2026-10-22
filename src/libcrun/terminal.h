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
#اگر_غیر_تعریف_شدہ TERMINAL_H
#تعریف_کرو TERMINAL_H
#شامل <config.h>

#شامل "container.h"
#شامل <termios.h>

خالی cleanup_terminalp (خالی *p);
#تعریف_کرو cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

صحیح libcrun_new_terminal (حرف **pty, libcrun_error_t *err);

صحیح libcrun_سیٹ_stdio (صحیح ptmx_fd, حرف *pty, libcrun_error_t *err);

صحیح libcrun_سیٹ_raw (صحیح fd, خالی **current_حالت, libcrun_error_t *err);

صحیح libcrun_clear_onlcr (صحیح fd, libcrun_error_t *err);

صحیح libcrun_terminal_تیاری_سائز (صحیح fd, غیرعلامتی مختصر صفیں, غیرعلامتی مختصر ستون_تعداد, libcrun_error_t *err);

#اگر_ختم
