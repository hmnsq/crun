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

#અન્તર્ભાવ "container.h"
#અન્તર્ભાવ "cgroup.h"
#અન્તર્ભાવ <unistd.h>

પૂર્ણાંક libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, અચલ અક્ષર *path, bool create_if_missing, libcrun_error_t *err);

પૂર્ણાંક libcrun_move_self_to_cgroups (અચલ અક્ષર *content, libcrun_error_t *err);

પૂર્ણાંક libcrun_cgroups_create_symlinks (પૂર્ણાંક dirfd, libcrun_error_t *err);

પૂર્ણાંક libcrun_get_cgroup_process (pid_t pid, અક્ષર **path, bool absolute, libcrun_error_t *err);

પૂર્ણાંક libcrun_get_cgroup_process_at (પૂર્ણાંક dirfd, અક્ષર **path, bool absolute, libcrun_error_t *err);

પૂર્ણાંક libcrun_get_cgroup_mode (libcrun_error_t *err);

પૂર્ણાંક libcrun_get_cgroup_dirfd (સંરચના libcrun_cgroup_status *status, અચલ અક્ષર *sub_cgroup, libcrun_error_t *err);

પૂર્ણાંક maybe_make_cgroup_threaded (અચલ અક્ષર *path, libcrun_error_t *err);

પૂર્ણાંક libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, અક્ષર *from, અક્ષર *to, libcrun_error_t *err);

પૂર્ણાંક destroy_cgroup_path (અચલ અક્ષર *path, પૂર્ણાંક mode, libcrun_error_t *err);

પૂર્ણાંક get_cgroup_dirfd_path (પૂર્ણાંક dirfd, અક્ષર **path, libcrun_error_t *err);

પૂર્ણાંક libcrun_cgroup_ensure_not_frozen (અચલ અક્ષર *cgroup_path, libcrun_error_t *err);

પૂર્ણાંક parse_cpu_max (અચલ અક્ષર *કિંમત, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
