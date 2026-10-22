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
#यदि_अपरिभाषितम् CGROUP_UTILS_H
#परिभाषय CGROUP_UTILS_H

#अन्तर्भाव "container.h"
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव <unistd.h>

पूर्णाङ्क libcrun_move_process_गन्तव्यम्_cgroup (pid_t pid, pid_t init_pid, ध्रुव वर्ण *मार्गः, bool create_if_missing, libcrun_error_t *err);

पूर्णाङ्क libcrun_move_self_गन्तव्यम्_cgroups (ध्रुव वर्ण *content, libcrun_error_t *err);

पूर्णाङ्क libcrun_cgroups_create_symlinks (पूर्णाङ्क dirfd, libcrun_error_t *err);

पूर्णाङ्क libcrun_get_cgroup_process (pid_t pid, वर्ण **मार्गः, bool absolute, libcrun_error_t *err);

पूर्णाङ्क libcrun_get_cgroup_process_at (पूर्णाङ्क dirfd, वर्ण **मार्गः, bool absolute, libcrun_error_t *err);

पूर्णाङ्क libcrun_get_cgroup_बहुलकम् (libcrun_error_t *err);

पूर्णाङ्क libcrun_get_cgroup_dirfd (संरचना libcrun_cgroup_स्थितिः *स्थितिः, ध्रुव वर्ण *sub_cgroup, libcrun_error_t *err);

पूर्णाङ्क maybe_make_cgroup_threaded (ध्रुव वर्ण *मार्गः, libcrun_error_t *err);

पूर्णाङ्क libcrun_migrate_all_pids_गन्तव्यम्_cgroup (pid_t init_pid, वर्ण *from, वर्ण *गन्तव्यम्, libcrun_error_t *err);

पूर्णाङ्क destroy_cgroup_मार्गः (ध्रुव वर्ण *मार्गः, पूर्णाङ्क बहुलकम्, libcrun_error_t *err);

पूर्णाङ्क get_cgroup_dirfd_मार्गः (पूर्णाङ्क dirfd, वर्ण **मार्गः, libcrun_error_t *err);

पूर्णाङ्क libcrun_cgroup_ensure_not_frozen (ध्रुव वर्ण *cgroup_मार्गः, libcrun_error_t *err);

पूर्णाङ्क parse_cpu_max (ध्रुव वर्ण *मानम्, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#यदि_अन्तः
