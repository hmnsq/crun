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
#جے_بنا_تعریف TERMINAL_H
#تعریف_کرو TERMINAL_H
#ضمن <config.h>

#ضمن "container.h"
#ضمن <termios.h>

فراغ cleanup_terminalp (فراغ *p);
#تعریف_کرو cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

صحيح libcrun_new_terminal (حرف **pty, libcrun_error_t *err);

صحيح libcrun_سیٹ_stdio (صحيح ptmx_fd, حرف *pty, libcrun_error_t *err);

صحيح libcrun_سیٹ_raw (صحيح fd, فراغ **current_حالت, libcrun_error_t *err);

صحيح libcrun_clear_onlcr (صحيح fd, libcrun_error_t *err);

صحيح libcrun_terminal_تیاری_ناپ (صحيح fd, بنانشانی چھوٹا صفاں, بنانشانی چھوٹا خانے_گنتی, libcrun_error_t *err);

#جے_انت
