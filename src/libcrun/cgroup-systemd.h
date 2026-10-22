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
#నిర్వచితం_కాకపోతే CGROUP_SYSTEMD_H
#నిర్వచించు CGROUP_SYSTEMD_H

#అన్తర్భావ "container.h"
#అన్తర్భావ <unistd.h>

#నిర్వచితమైతే HAVE_SYSTEMD
బాహ్య పూర్ణాంకం parse_sd_array (అక్షరం *s, అక్షరం **బహిర్గతం, అక్షరం **తదుపరి, libcrun_error_t *err);

బాహ్య అక్షరం *get_cgroup_scope_మార్గం (స్థిరరాశి అక్షరం *cgroup_మార్గం, స్థిరరాశి అక్షరం *scope);
#అయితే_ముగింపు

బాహ్య నిర్మాణం libcrun_cgroup_manager cgroup_manager_systemd;

#అయితే_ముగింపు
