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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CGROUP_UTILS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CGROUP_UTILS_H

#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>

ਪੂਰਨ libcrun_move_process_ਮੰਜ਼ਿਲ_cgroup (pid_t pid, pid_t init_pid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool create_if_missing, libcrun_error_t *err);

ਪੂਰਨ libcrun_move_self_ਮੰਜ਼ਿਲ_cgroups (ਅਚਲ ਅੱਖਰ *content, libcrun_error_t *err);

ਪੂਰਨ libcrun_cgroups_create_symlinks (ਪੂਰਨ dirfd, libcrun_error_t *err);

ਪੂਰਨ libcrun_get_cgroup_process (pid_t pid, ਅੱਖਰ **ਰਸਤਾ, bool absolute, libcrun_error_t *err);

ਪੂਰਨ libcrun_get_cgroup_process_at (ਪੂਰਨ dirfd, ਅੱਖਰ **ਰਸਤਾ, bool absolute, libcrun_error_t *err);

ਪੂਰਨ libcrun_get_cgroup_ਬਹੁਲਕ (libcrun_error_t *err);

ਪੂਰਨ libcrun_get_cgroup_dirfd (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *sub_cgroup, libcrun_error_t *err);

ਪੂਰਨ maybe_make_cgroup_threaded (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err);

ਪੂਰਨ libcrun_migrate_all_pids_ਮੰਜ਼ਿਲ_cgroup (pid_t init_pid, ਅੱਖਰ *from, ਅੱਖਰ *ਮੰਜ਼ਿਲ, libcrun_error_t *err);

ਪੂਰਨ destroy_cgroup_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ, libcrun_error_t *err);

ਪੂਰਨ get_cgroup_dirfd_ਰਸਤਾ (ਪੂਰਨ dirfd, ਅੱਖਰ **ਰਸਤਾ, libcrun_error_t *err);

ਪੂਰਨ libcrun_cgroup_ensure_not_frozen (ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ, libcrun_error_t *err);

ਪੂਰਨ parse_cpu_max (ਅਚਲ ਅੱਖਰ *ਮੁੱਲ, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#ਜੇ_ਅੰਤ
