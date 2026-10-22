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
#اگر_تعریف_نشده CGROUP_UTILS_H
#تعریف_کن CGROUP_UTILS_H

#ضمن "container.h"
#ضمن "cgroup.h"
#ضمن <unistd.h>

صحيح libcrun_move_process_مقصد_cgroup (pid_t pid, pid_t init_pid, ثابت حرف *مسیر, bool create_if_missing, libcrun_error_t *err);

صحيح libcrun_move_self_مقصد_cgroups (ثابت حرف *content, libcrun_error_t *err);

صحيح libcrun_cgroups_create_symlinks (صحيح dirfd, libcrun_error_t *err);

صحيح libcrun_get_cgroup_process (pid_t pid, حرف **مسیر, bool absolute, libcrun_error_t *err);

صحيح libcrun_get_cgroup_process_at (صحيح dirfd, حرف **مسیر, bool absolute, libcrun_error_t *err);

صحيح libcrun_get_cgroup_نما (libcrun_error_t *err);

صحيح libcrun_get_cgroup_dirfd (بنية libcrun_cgroup_وضعیت *وضعیت, ثابت حرف *sub_cgroup, libcrun_error_t *err);

صحيح maybe_make_cgroup_threaded (ثابت حرف *مسیر, libcrun_error_t *err);

صحيح libcrun_migrate_all_pids_مقصد_cgroup (pid_t init_pid, حرف *from, حرف *مقصد, libcrun_error_t *err);

صحيح destroy_cgroup_مسیر (ثابت حرف *مسیر, صحيح نما, libcrun_error_t *err);

صحيح get_cgroup_dirfd_مسیر (صحيح dirfd, حرف **مسیر, libcrun_error_t *err);

صحيح libcrun_cgroup_ensure_not_frozen (ثابت حرف *cgroup_مسیر, libcrun_error_t *err);

صحيح parse_cpu_max (ثابت حرف *مقدار, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#پایان_اگر
