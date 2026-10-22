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

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>

ಪೂರ್ಣಾಙ್ಕ libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ಧ್ರುವ ವರ್ಣ *path, bool create_if_missing, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_move_self_to_cgroups (ಧ್ರುವ ವರ್ಣ *content, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_cgroups_create_symlinks (ಪೂರ್ಣಾಙ್ಕ dirfd, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_get_cgroup_process (pid_t pid, ವರ್ಣ **path, bool absolute, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_get_cgroup_process_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ವರ್ಣ **path, bool absolute, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_get_cgroup_mode (libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_get_cgroup_dirfd (ಸಂರಚನಾ libcrun_cgroup_status *status, ಧ್ರುವ ವರ್ಣ *sub_cgroup, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ maybe_make_cgroup_threaded (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, ವರ್ಣ *from, ವರ್ಣ *to, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ destroy_cgroup_path (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ get_cgroup_dirfd_path (ಪೂರ್ಣಾಙ್ಕ dirfd, ವರ್ಣ **path, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_ensure_not_frozen (ಧ್ರುವ ವರ್ಣ *cgroup_path, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ parse_cpu_max (ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
