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

#অন্তর্ভাৱ "container.h"
#অন্তর্ভাৱ "cgroup.h"
#অন্তর্ভাৱ <unistd.h>

পূর্ণাঙ্ক libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ধ্রুৱ ৱর্ণ *path, bool create_if_missing, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_move_self_to_cgroups (ধ্রুৱ ৱর্ণ *content, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_cgroups_create_symlinks (পূর্ণাঙ্ক dirfd, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_get_cgroup_process (pid_t pid, ৱর্ণ **path, bool absolute, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_get_cgroup_process_at (পূর্ণাঙ্ক dirfd, ৱর্ণ **path, bool absolute, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_get_cgroup_mode (libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_get_cgroup_dirfd (সংরচনা libcrun_cgroup_status *status, ধ্রুৱ ৱর্ণ *sub_cgroup, libcrun_error_t *err);

পূর্ণাঙ্ক maybe_make_cgroup_threaded (ধ্রুৱ ৱর্ণ *path, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, ৱর্ণ *from, ৱর্ণ *to, libcrun_error_t *err);

পূর্ণাঙ্ক destroy_cgroup_path (ধ্রুৱ ৱর্ণ *path, পূর্ণাঙ্ক mode, libcrun_error_t *err);

পূর্ণাঙ্ক get_cgroup_dirfd_path (পূর্ণাঙ্ক dirfd, ৱর্ণ **path, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_cgroup_ensure_not_frozen (ধ্রুৱ ৱর্ণ *cgroup_path, libcrun_error_t *err);

পূর্ণাঙ্ক parse_cpu_max (ধ্রুৱ ৱর্ণ *মান, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
