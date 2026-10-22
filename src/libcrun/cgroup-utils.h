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

#அந்தர்பாவ "container.h"
#அந்தர்பாவ "cgroup.h"
#அந்தர்பாவ <unistd.h>

முழு libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, மாறிலி எழுத்து *path, bool create_if_missing, libcrun_error_t *err);

முழு libcrun_move_self_to_cgroups (மாறிலி எழுத்து *content, libcrun_error_t *err);

முழு libcrun_cgroups_create_symlinks (முழு dirfd, libcrun_error_t *err);

முழு libcrun_get_cgroup_process (pid_t pid, எழுத்து **path, bool absolute, libcrun_error_t *err);

முழு libcrun_get_cgroup_process_at (முழு dirfd, எழுத்து **path, bool absolute, libcrun_error_t *err);

முழு libcrun_get_cgroup_mode (libcrun_error_t *err);

முழு libcrun_get_cgroup_dirfd (கட்டமைப்பு libcrun_cgroup_status *status, மாறிலி எழுத்து *sub_cgroup, libcrun_error_t *err);

முழு maybe_make_cgroup_threaded (மாறிலி எழுத்து *path, libcrun_error_t *err);

முழு libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, எழுத்து *from, எழுத்து *to, libcrun_error_t *err);

முழு destroy_cgroup_path (மாறிலி எழுத்து *path, முழு mode, libcrun_error_t *err);

முழு get_cgroup_dirfd_path (முழு dirfd, எழுத்து **path, libcrun_error_t *err);

முழு libcrun_cgroup_ensure_not_frozen (மாறிலி எழுத்து *cgroup_path, libcrun_error_t *err);

முழு parse_cpu_max (மாறிலி எழுத்து *மதிப்பு, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
