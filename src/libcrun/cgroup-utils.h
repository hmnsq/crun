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
#నిర్వచితం_కాకపోతే CGROUP_UTILS_H
#నిర్వచించు CGROUP_UTILS_H

#అన్తర్భావ "container.h"
#అన్తర్భావ "cgroup.h"
#అన్తర్భావ <unistd.h>

పూర్ణాంకం libcrun_move_process_గమ్యం_cgroup (pid_t pid, pid_t init_pid, స్థిరరాశి అక్షరం *మార్గం, bool create_if_missing, libcrun_error_t *err);

పూర్ణాంకం libcrun_move_self_గమ్యం_cgroups (స్థిరరాశి అక్షరం *content, libcrun_error_t *err);

పూర్ణాంకం libcrun_cgroups_create_symlinks (పూర్ణాంకం dirfd, libcrun_error_t *err);

పూర్ణాంకం libcrun_get_cgroup_process (pid_t pid, అక్షరం **మార్గం, bool absolute, libcrun_error_t *err);

పూర్ణాంకం libcrun_get_cgroup_process_at (పూర్ణాంకం dirfd, అక్షరం **మార్గం, bool absolute, libcrun_error_t *err);

పూర్ణాంకం libcrun_get_cgroup_బహుళకం (libcrun_error_t *err);

పూర్ణాంకం libcrun_get_cgroup_dirfd (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, స్థిరరాశి అక్షరం *sub_cgroup, libcrun_error_t *err);

పూర్ణాంకం maybe_make_cgroup_threaded (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err);

పూర్ణాంకం libcrun_migrate_all_pids_గమ్యం_cgroup (pid_t init_pid, అక్షరం *from, అక్షరం *గమ్యం, libcrun_error_t *err);

పూర్ణాంకం destroy_cgroup_మార్గం (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం, libcrun_error_t *err);

పూర్ణాంకం get_cgroup_dirfd_మార్గం (పూర్ణాంకం dirfd, అక్షరం **మార్గం, libcrun_error_t *err);

పూర్ణాంకం libcrun_cgroup_ensure_not_frozen (స్థిరరాశి అక్షరం *cgroup_మార్గం, libcrun_error_t *err);

పూర్ణాంకం parse_cpu_max (స్థిరరాశి అక్షరం *విలువ, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#అయితే_ముగింపు
