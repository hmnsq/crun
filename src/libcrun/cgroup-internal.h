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
#ifndef CGROUP_INTERNAL_H
#define CGROUP_INTERNAL_H

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <math.h>

ಗಣನಾ
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

ಸಂರಚನಾ libcrun_cgroup_status
{
  ವರ್ಣ *path;
  ವರ್ಣ *scope;

  ಪೂರ್ಣಾಙ್ಕ manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
ಸಂರಚನಾ libcrun_cgroup_args;

ಸಂರಚನಾ libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  ಪೂರ್ಣಾಙ್ಕ (*create_cgroup) (ಸಂರಚನಾ libcrun_cgroup_args *args, ಸಂರಚನಾ libcrun_cgroup_status *out, libcrun_error_t *err);
  ಪೂರ್ಣಾಙ್ಕ (*precreate_cgroup) (ಸಂರಚನಾ libcrun_cgroup_args *args, ಪೂರ್ಣಾಙ್ಕ *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  ಪೂರ್ಣಾಙ್ಕ (*destroy_cgroup) (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  ಪೂರ್ಣಾಙ್ಕ (*update_resources) (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  ಪೂರ್ಣಾಙ್ಕ (*attach_process) (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status, ಧ್ರುವ ವರ್ಣ *path, pid_t pid, libcrun_error_t *err);
};

ಪೂರ್ಣಾಙ್ಕ move_process_to_cgroup (pid_t pid, ಧ್ರುವ ವರ್ಣ *subsystem, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ enter_cgroup_v1_subsystem (pid_t pid, ಧ್ರುವ ವರ್ಣ *subsystem, ಧ್ರುವ ವರ್ಣ *path, bool create_if_missing,
                               libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ enable_controllers (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ chown_cgroups (ಧ್ರುವ ವರ್ಣ *path, uid_t uid, gid_t gid, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ cgroup_killall_path (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ ಸಙ್ಕೇತ, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_read_pids_from_path (ಧ್ರುವ ವರ್ಣ *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (ವರ್ಣ *content, ವರ್ಣ **saveptr, ವರ್ಣ **id, ವರ್ಣ **controller_list, ವರ್ಣ **path);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
is_rootless (libcrun_error_t *err)
{
  ಯದಿ (geteuid ())
    ಪ್ರತ್ಯಾಗಮ 1;

  ಪ್ರತ್ಯಾಗಮ check_running_in_user_namespace (err);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
write_cgroup_file (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ಶೂನ್ಯ *data, size_t len, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, data, len, err);
}

ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_pause_unpause_path (ಧ್ರುವ ವರ್ಣ *cgroup_path, ಧ್ರುವ bool pause, libcrun_error_t *err);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint64_t
convert_shares_to_weight (uint64_t shares)
{
  ದ್ವಿಗುಣ l, exponent;

  /* The value of 0 means "unset".  */
  ಯದಿ (shares == 0)
    ಪ್ರತ್ಯಾಗಮ 0;
  ಯದಿ (shares <= 2)
    ಪ್ರತ್ಯಾಗಮ 1;
  ಯದಿ (shares >= 262144)
    ಪ್ರತ್ಯಾಗಮ 10000;

  l = log2 ((ದ್ವಿಗುಣ) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  ಪ್ರತ್ಯಾಗಮ (uint64_t) ceil (pow (10, exponent));
}

ಪೂರ್ಣಾಙ್ಕ initialize_cpuset_subsystem (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ initialize_cpuset_subsystem_resources (ಧ್ರುವ ವರ್ಣ *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ write_cpuset_resources (ಪೂರ್ಣಾಙ್ಕ dirfd_cpuset, ಪೂರ್ಣಾಙ್ಕ cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ write_cpu_idle (ಪೂರ್ಣಾಙ್ಕ cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ write_cpu_burst (ಪೂರ್ಣಾಙ್ಕ cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
