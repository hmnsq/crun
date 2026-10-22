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
#اگر_غیر_تعریف_شدہ CGROUP_UTILS_H
#تعریف_کرو CGROUP_UTILS_H

#شامل "container.h"
#شامل "cgroup.h"
#شامل <unistd.h>

صحیح libcrun_move_process_مقصد_cgroup (pid_t pid, pid_t init_pid, ثابت حرف *راستہ, bool create_if_missing, libcrun_error_t *err);

صحیح libcrun_move_self_مقصد_cgroups (ثابت حرف *content, libcrun_error_t *err);

صحیح libcrun_cgroups_create_symlinks (صحیح dirfd, libcrun_error_t *err);

صحیح libcrun_get_cgroup_process (pid_t pid, حرف **راستہ, bool absolute, libcrun_error_t *err);

صحیح libcrun_get_cgroup_process_at (صحیح dirfd, حرف **راستہ, bool absolute, libcrun_error_t *err);

صحیح libcrun_get_cgroup_منوال (libcrun_error_t *err);

صحیح libcrun_get_cgroup_dirfd (ساخت libcrun_cgroup_حالت *حالت_2, ثابت حرف *sub_cgroup, libcrun_error_t *err);

صحیح maybe_make_cgroup_threaded (ثابت حرف *راستہ, libcrun_error_t *err);

صحیح libcrun_migrate_all_pids_مقصد_cgroup (pid_t init_pid, حرف *from, حرف *مقصد, libcrun_error_t *err);

صحیح destroy_cgroup_راستہ (ثابت حرف *راستہ, صحیح منوال, libcrun_error_t *err);

صحیح get_cgroup_dirfd_راستہ (صحیح dirfd, حرف **راستہ, libcrun_error_t *err);

صحیح libcrun_cgroup_ensure_not_frozen (ثابت حرف *cgroup_راستہ, libcrun_error_t *err);

صحیح parse_cpu_max (ثابت حرف *قدر, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#اگر_ختم
