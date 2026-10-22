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

ཧྲིལ libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, const var_na *path, bool create_if_missing, libcrun_error_t *err);

ཧྲིལ libcrun_move_self_to_cgroups (const var_na *content, libcrun_error_t *err);

ཧྲིལ libcrun_cgroups_create_symlinks (ཧྲིལ dirfd, libcrun_error_t *err);

ཧྲིལ libcrun_get_cgroup_process (pid_t pid, var_na **path, bool absolute, libcrun_error_t *err);

ཧྲིལ libcrun_get_cgroup_process_at (ཧྲིལ dirfd, var_na **path, bool absolute, libcrun_error_t *err);

ཧྲིལ libcrun_get_cgroup_mode (libcrun_error_t *err);

ཧྲིལ libcrun_get_cgroup_dirfd (sa_nracanaa libcrun_cgroup_status *status, const var_na *sub_cgroup, libcrun_error_t *err);

ཧྲིལ maybe_make_cgroup_threaded (const var_na *path, libcrun_error_t *err);

ཧྲིལ libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, var_na *from, var_na *to, libcrun_error_t *err);

ཧྲིལ destroy_cgroup_path (const var_na *path, ཧྲིལ mode, libcrun_error_t *err);

ཧྲིལ get_cgroup_dirfd_path (ཧྲིལ dirfd, var_na **path, libcrun_error_t *err);

ཧྲིལ libcrun_cgroup_ensure_not_frozen (const var_na *cgroup_path, libcrun_error_t *err);

ཧྲིལ parse_cpu_max (const var_na *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
