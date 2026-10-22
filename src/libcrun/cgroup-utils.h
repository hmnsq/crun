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

#अन्तर्भाव "container.h"
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव <unistd.h>

पूर्णाङ्क libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, अचर वर्ण *path, bool create_if_missing, libcrun_error_t *err);

पूर्णाङ्क libcrun_move_self_to_cgroups (अचर वर्ण *content, libcrun_error_t *err);

पूर्णाङ्क libcrun_cgroups_create_symlinks (पूर्णाङ्क dirfd, libcrun_error_t *err);

पूर्णाङ्क libcrun_get_cgroup_process (pid_t pid, वर्ण **path, bool absolute, libcrun_error_t *err);

पूर्णाङ्क libcrun_get_cgroup_process_at (पूर्णाङ्क dirfd, वर्ण **path, bool absolute, libcrun_error_t *err);

पूर्णाङ्क libcrun_get_cgroup_mode (libcrun_error_t *err);

पूर्णाङ्क libcrun_get_cgroup_dirfd (संरचना libcrun_cgroup_status *status, अचर वर्ण *sub_cgroup, libcrun_error_t *err);

पूर्णाङ्क maybe_make_cgroup_threaded (अचर वर्ण *path, libcrun_error_t *err);

पूर्णाङ्क libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, वर्ण *from, वर्ण *to, libcrun_error_t *err);

पूर्णाङ्क destroy_cgroup_path (अचर वर्ण *path, पूर्णाङ्क mode, libcrun_error_t *err);

पूर्णाङ्क get_cgroup_dirfd_path (पूर्णाङ्क dirfd, वर्ण **path, libcrun_error_t *err);

पूर्णाङ्क libcrun_cgroup_ensure_not_frozen (अचर वर्ण *cgroup_path, libcrun_error_t *err);

पूर्णाङ्क parse_cpu_max (अचर वर्ण *मान, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
