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
#अगर_अघोषित CGROUP_UTILS_H
#घोषणा CGROUP_UTILS_H

#समावेश "container.h"
#समावेश "cgroup.h"
#समावेश <unistd.h>

पूर्णांक libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, स्थिर अक्षर *path, bool create_if_missing, libcrun_error_t *err);

पूर्णांक libcrun_move_self_to_cgroups (स्थिर अक्षर *content, libcrun_error_t *err);

पूर्णांक libcrun_cgroups_create_symlinks (पूर्णांक dirfd, libcrun_error_t *err);

पूर्णांक libcrun_get_cgroup_process (pid_t pid, अक्षर **path, bool absolute, libcrun_error_t *err);

पूर्णांक libcrun_get_cgroup_process_at (पूर्णांक dirfd, अक्षर **path, bool absolute, libcrun_error_t *err);

पूर्णांक libcrun_get_cgroup_mode (libcrun_error_t *err);

पूर्णांक libcrun_get_cgroup_dirfd (काष्ठा libcrun_cgroup_status *status, स्थिर अक्षर *sub_cgroup, libcrun_error_t *err);

पूर्णांक maybe_make_cgroup_threaded (स्थिर अक्षर *path, libcrun_error_t *err);

पूर्णांक libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, अक्षर *from, अक्षर *to, libcrun_error_t *err);

पूर्णांक destroy_cgroup_path (स्थिर अक्षर *path, पूर्णांक mode, libcrun_error_t *err);

पूर्णांक get_cgroup_dirfd_path (पूर्णांक dirfd, अक्षर **path, libcrun_error_t *err);

पूर्णांक libcrun_cgroup_ensure_not_frozen (स्थिर अक्षर *cgroup_path, libcrun_error_t *err);

पूर्णांक parse_cpu_max (स्थिर अक्षर *मूल्य, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#पूर्ण_अगर
