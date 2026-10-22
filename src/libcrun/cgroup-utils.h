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

#അന്തര്ഭാവ "container.h"
#അന്തര്ഭാവ "cgroup.h"
#അന്തര്ഭാവ <unistd.h>

പൂർണ്ണസംഖ്യ libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, സ്ഥിരം അക്ഷരം *path, bool create_if_missing, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_move_self_to_cgroups (സ്ഥിരം അക്ഷരം *content, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_cgroups_create_symlinks (പൂർണ്ണസംഖ്യ dirfd, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_get_cgroup_process (pid_t pid, അക്ഷരം **path, bool absolute, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_get_cgroup_process_at (പൂർണ്ണസംഖ്യ dirfd, അക്ഷരം **path, bool absolute, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_get_cgroup_mode (libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_get_cgroup_dirfd (ഘടന libcrun_cgroup_status *status, സ്ഥിരം അക്ഷരം *sub_cgroup, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ maybe_make_cgroup_threaded (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, അക്ഷരം *from, അക്ഷരം *to, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ destroy_cgroup_path (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ get_cgroup_dirfd_path (പൂർണ്ണസംഖ്യ dirfd, അക്ഷരം **path, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_cgroup_ensure_not_frozen (സ്ഥിരം അക്ഷരം *cgroup_path, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ parse_cpu_max (സ്ഥിരം അക്ഷരം *മൂല്യം, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
