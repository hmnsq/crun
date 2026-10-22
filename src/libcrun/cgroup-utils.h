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

#include "container.h"
#include "cgroup.h"
#include <unistd.h>

số_nguyên libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, const char *path, bool create_if_missing, libcrun_error_t *err);

số_nguyên libcrun_move_self_to_cgroups (const char *content, libcrun_error_t *err);

số_nguyên libcrun_cgroups_create_symlinks (số_nguyên dirfd, libcrun_error_t *err);

số_nguyên libcrun_get_cgroup_process (pid_t pid, char **path, bool absolute, libcrun_error_t *err);

số_nguyên libcrun_get_cgroup_process_at (số_nguyên dirfd, char **path, bool absolute, libcrun_error_t *err);

số_nguyên libcrun_get_cgroup_mode (libcrun_error_t *err);

số_nguyên libcrun_get_cgroup_dirfd (struct libcrun_cgroup_status *status, const char *sub_cgroup, libcrun_error_t *err);

số_nguyên maybe_make_cgroup_threaded (const char *path, libcrun_error_t *err);

số_nguyên libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, char *from, char *to, libcrun_error_t *err);

số_nguyên destroy_cgroup_path (const char *path, số_nguyên mode, libcrun_error_t *err);

số_nguyên get_cgroup_dirfd_path (số_nguyên dirfd, char **path, libcrun_error_t *err);

số_nguyên libcrun_cgroup_ensure_not_frozen (const char *cgroup_path, libcrun_error_t *err);

số_nguyên parse_cpu_max (const char *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
