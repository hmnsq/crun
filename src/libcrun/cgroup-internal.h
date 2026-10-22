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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CGROUP_ಆಂತರಿಕ_H
#ವ್ಯಾಖ್ಯಾನಿಸು CGROUP_ಆಂತರಿಕ_H

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <math.h>

ಎಣಿಕೆ
{
  CGROUP_ಸ್ಮರಣೆ = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ
{
  ಅಕ್ಷರ *ಮಾರ್ಗ;
  ಅಕ್ಷರ *scope;

  ಪೂರ್ಣಾಂಕ manager;

  bool bpf_dev_ಗಣ;
};

/* Forward declaration for function pointers below.  */
ರಚನೆ libcrun_cgroup_ವಾದಗಳು;

ರಚನೆ libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  ಪೂರ್ಣಾಂಕ (*create_cgroup) (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಹೊರಹೋಗು, libcrun_error_t *err);
  ಪೂರ್ಣಾಂಕ (*precreate_cgroup) (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ಪೂರ್ಣಾಂಕ *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  ಪೂರ್ಣಾಂಕ (*destroy_cgroup) (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  ಪೂರ್ಣಾಂಕ (*update_resources) (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  ಪೂರ್ಣಾಂಕ (*attach_process) (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, pid_t pid, libcrun_error_t *err);
};

ಪೂರ್ಣಾಂಕ move_process_ಗಮ್ಯ_cgroup (pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subsystem, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ enter_cgroup_v1_subsystem (pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subsystem, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool create_if_missing,
                               libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ enable_controllers (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ chown_cgroups (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, uid_t uid, gid_t gid, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ cgroup_killall_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಸಂಕೇತ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_cgroup_read_pids_from_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (ಅಕ್ಷರ *content, ಅಕ್ಷರ **saveptr, ಅಕ್ಷರ **id, ಅಕ್ಷರ **controller_list, ಅಕ್ಷರ **ಮಾರ್ಗ);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
is_rootless (libcrun_error_t *err)
{
  ಆದರೆ (geteuid ())
    ಹಿಂತಿರುಗಿ 1;

  ಹಿಂತಿರುಗಿ check_running_ಒಳ_user_namespace (err);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
write_cgroup_file (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, size_t len, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, ದತ್ತಾಂಶ, len, err);
}

ಪೂರ್ಣಾಂಕ libcrun_cgroup_pause_unpause_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ bool pause, libcrun_error_t *err);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ uint64_t
convert_shares_ಗಮ್ಯ_ತೂಕ (uint64_t shares)
{
  ದ್ವಿಗುಣ l, exponent;

  /* The value of 0 means "unset".  */
  ಆದರೆ (shares == 0)
    ಹಿಂತಿರುಗಿ 0;
  ಆದರೆ (shares <= 2)
    ಹಿಂತಿರುಗಿ 1;
  ಆದರೆ (shares >= 262144)
    ಹಿಂತಿರುಗಿ 10000;

  l = log2 ((ದ್ವಿಗುಣ) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  ಹಿಂತಿರುಗಿ (uint64_t) ceil (ಘಾತ (10, exponent));
}

ಪೂರ್ಣಾಂಕ initialize_cpuset_subsystem (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ initialize_cpuset_subsystem_resources (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ write_cpuset_resources (ಪೂರ್ಣಾಂಕ dirfd_cpuset, ಪೂರ್ಣಾಂಕ cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ write_cpu_idle (ಪೂರ್ಣಾಂಕ cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ write_cpu_ಅವಧಿ (ಪೂರ್ಣಾಂಕ cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#ಆದರೆ_ಅಂತ್ಯ
