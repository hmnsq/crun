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

#অন্তর্ভাব "container.h"
#অন্তর্ভাব "cgroup.h"
#অন্তর্ভাব <unistd.h>

পূর্ণসংখ্যা libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ধ্রুবক অক্ষর *path, bool create_if_missing, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_move_self_to_cgroups (ধ্রুবক অক্ষর *content, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_cgroups_create_symlinks (পূর্ণসংখ্যা dirfd, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_get_cgroup_process (pid_t pid, অক্ষর **path, bool absolute, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_get_cgroup_process_at (পূর্ণসংখ্যা dirfd, অক্ষর **path, bool absolute, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_get_cgroup_mode (libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_get_cgroup_dirfd (কাঠামো libcrun_cgroup_status *status, ধ্রুবক অক্ষর *sub_cgroup, libcrun_error_t *err);

পূর্ণসংখ্যা maybe_make_cgroup_threaded (ধ্রুবক অক্ষর *path, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, অক্ষর *from, অক্ষর *to, libcrun_error_t *err);

পূর্ণসংখ্যা destroy_cgroup_path (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode, libcrun_error_t *err);

পূর্ণসংখ্যা get_cgroup_dirfd_path (পূর্ণসংখ্যা dirfd, অক্ষর **path, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_cgroup_ensure_not_frozen (ধ্রুবক অক্ষর *cgroup_path, libcrun_error_t *err);

পূর্ণসংখ্যা parse_cpu_max (ধ্রুবক অক্ষর *মান, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
