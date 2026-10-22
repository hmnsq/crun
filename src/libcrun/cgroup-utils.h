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
#ifndef CGROUP_UTILS_H
#define CGROUP_UTILS_H

#ضمن "container.h"
#ضمن "cgroup.h"
#ضمن <unistd.h>

صحيح libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ثابت حرف *path, bool create_if_missing, libcrun_error_t *err);

صحيح libcrun_move_self_to_cgroups (ثابت حرف *content, libcrun_error_t *err);

صحيح libcrun_cgroups_create_symlinks (صحيح dirfd, libcrun_error_t *err);

صحيح libcrun_get_cgroup_process (pid_t pid, حرف **path, bool absolute, libcrun_error_t *err);

صحيح libcrun_get_cgroup_process_at (صحيح dirfd, حرف **path, bool absolute, libcrun_error_t *err);

صحيح libcrun_get_cgroup_mode (libcrun_error_t *err);

صحيح libcrun_get_cgroup_dirfd (بنية libcrun_cgroup_status *status, ثابت حرف *sub_cgroup, libcrun_error_t *err);

صحيح maybe_make_cgroup_threaded (ثابت حرف *path, libcrun_error_t *err);

صحيح libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, حرف *from, حرف *to, libcrun_error_t *err);

صحيح destroy_cgroup_path (ثابت حرف *path, صحيح mode, libcrun_error_t *err);

صحيح get_cgroup_dirfd_path (صحيح dirfd, حرف **path, libcrun_error_t *err);

صحيح libcrun_cgroup_ensure_not_frozen (ثابت حرف *cgroup_path, libcrun_error_t *err);

صحيح parse_cpu_max (ثابت حرف *قدر, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
