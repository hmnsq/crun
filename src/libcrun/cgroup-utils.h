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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CGROUP_UTILS_H
#ವ್ಯಾಖ್ಯಾನಿಸು CGROUP_UTILS_H

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>

ಪೂರ್ಣಾಂಕ libcrun_move_process_ಗಮ್ಯ_cgroup (pid_t pid, pid_t init_pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool create_if_missing, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_move_self_ಗಮ್ಯ_cgroups (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *content, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_cgroups_create_symlinks (ಪೂರ್ಣಾಂಕ dirfd, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_get_cgroup_process (pid_t pid, ಅಕ್ಷರ **ಮಾರ್ಗ, bool absolute, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_get_cgroup_process_at (ಪೂರ್ಣಾಂಕ dirfd, ಅಕ್ಷರ **ಮಾರ್ಗ, bool absolute, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_get_cgroup_ಬಹುಲಕ (libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_get_cgroup_dirfd (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sub_cgroup, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ maybe_make_cgroup_threaded (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_migrate_all_pids_ಗಮ್ಯ_cgroup (pid_t init_pid, ಅಕ್ಷರ *from, ಅಕ್ಷರ *ಗಮ್ಯ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ destroy_cgroup_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ get_cgroup_dirfd_ಮಾರ್ಗ (ಪೂರ್ಣಾಂಕ dirfd, ಅಕ್ಷರ **ಮಾರ್ಗ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_cgroup_ensure_not_frozen (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ parse_cpu_max (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err);

#ಆದರೆ_ಅಂತ್ಯ
