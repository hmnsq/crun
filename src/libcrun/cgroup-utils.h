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

#ଅନ୍ତର୍ଭାଵ "container.h"
#ଅନ୍ତର୍ଭାଵ "cgroup.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ଅଚଳ ଅକ୍ଷର *path, bool create_if_missing, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_move_self_to_cgroups (ଅଚଳ ଅକ୍ଷର *content, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroups_create_symlinks (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_get_cgroup_process (pid_t pid, ଅକ୍ଷର **path, bool absolute, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_get_cgroup_process_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅକ୍ଷର **path, bool absolute, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_get_cgroup_mode (libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_get_cgroup_dirfd (ସଂରଚନା libcrun_cgroup_status *status, ଅଚଳ ଅକ୍ଷର *sub_cgroup, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା maybe_make_cgroup_threaded (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, ଅକ୍ଷର *from, ଅକ୍ଷର *to, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା destroy_cgroup_path (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_cgroup_dirfd_path (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅକ୍ଷର **path, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_ensure_not_frozen (ଅଚଳ ଅକ୍ଷର *cgroup_path, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_cpu_max (ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#endif
