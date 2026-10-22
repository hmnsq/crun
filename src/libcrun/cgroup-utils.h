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

#شامل "container.h"
#شامل "cgroup.h"
#شامل <unistd.h>

صحیح libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ثابت نویسه *path, bool create_if_missing, libcrun_error_t *err);

صحیح libcrun_move_self_to_cgroups (ثابت نویسه *content, libcrun_error_t *err);

صحیح libcrun_cgroups_create_symlinks (صحیح dirfd, libcrun_error_t *err);

صحیح libcrun_get_cgroup_process (pid_t pid, نویسه **path, bool absolute, libcrun_error_t *err);

صحیح libcrun_get_cgroup_process_at (صحیح dirfd, نویسه **path, bool absolute, libcrun_error_t *err);

صحیح libcrun_get_cgroup_mode (libcrun_error_t *err);

صحیح libcrun_get_cgroup_dirfd (ساختار libcrun_cgroup_status *status, ثابت نویسه *sub_cgroup, libcrun_error_t *err);

صحیح maybe_make_cgroup_threaded (ثابت نویسه *path, libcrun_error_t *err);

صحیح libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, نویسه *from, نویسه *to, libcrun_error_t *err);

صحیح destroy_cgroup_path (ثابت نویسه *path, صحیح mode, libcrun_error_t *err);

صحیح get_cgroup_dirfd_path (صحیح dirfd, نویسه **path, libcrun_error_t *err);

صحیح libcrun_cgroup_ensure_not_frozen (ثابت نویسه *cgroup_path, libcrun_error_t *err);

صحیح parse_cpu_max (ثابت نویسه *مقدار, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
