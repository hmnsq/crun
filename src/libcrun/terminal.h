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
#నిర్వచితం_కాకపోతే TERMINAL_H
#నిర్వచించు TERMINAL_H
#అన్తర్భావ <config.h>

#అన్తర్భావ "container.h"
#అన్తర్భావ <termios.h>

శూన్య cleanup_terminalp (శూన్య *p);
#నిర్వచించు cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

పూర్ణాంకం libcrun_new_terminal (అక్షరం **pty, libcrun_error_t *err);

పూర్ణాంకం libcrun_సమితి_stdio (పూర్ణాంకం ptmx_fd, అక్షరం *pty, libcrun_error_t *err);

పూర్ణాంకం libcrun_సమితి_raw (పూర్ణాంకం fd, శూన్య **current_స్థితి, libcrun_error_t *err);

పూర్ణాంకం libcrun_clear_onlcr (పూర్ణాంకం fd, libcrun_error_t *err);

పూర్ణాంకం libcrun_terminal_అమరిక_కొలత (పూర్ణాంకం fd, గుర్తులేని పొట్టి అడ్డువరుసలు, గుర్తులేని పొట్టి నిలువుల_సంఖ్య, libcrun_error_t *err);

#అయితే_ముగింపు
