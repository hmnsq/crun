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

#include "container.h"
#include "utils.h"

#include <stdint.h>
#include <math.h>

ga_nanaa
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

sa_nracanaa libcrun_cgroup_status
{
  var_na *path;
  var_na *scope;

  puur_naa_nka manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
sa_nracanaa libcrun_cgroup_args;

sa_nracanaa libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  puur_naa_nka (*create_cgroup) (sa_nracanaa libcrun_cgroup_args *args, sa_nracanaa libcrun_cgroup_status *out, libcrun_error_t *err);
  puur_naa_nka (*precreate_cgroup) (sa_nracanaa libcrun_cgroup_args *args, puur_naa_nka *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  puur_naa_nka (*destroy_cgroup) (sa_nracanaa libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  puur_naa_nka (*update_resources) (sa_nracanaa libcrun_cgroup_status *cgroup_status, const var_na *state_root, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  puur_naa_nka (*attach_process) (sa_nracanaa libcrun_cgroup_status *cgroup_status, const var_na *path, pid_t pid, libcrun_error_t *err);
};

puur_naa_nka move_process_to_cgroup (pid_t pid, const var_na *subsystem, const var_na *path, libcrun_error_t *err);
puur_naa_nka enter_cgroup_v1_subsystem (pid_t pid, const var_na *subsystem, const var_na *path, bool create_if_missing,
                               libcrun_error_t *err);
puur_naa_nka enable_controllers (const var_na *path, libcrun_error_t *err);
puur_naa_nka chown_cgroups (const var_na *path, uid_t uid, gid_t gid, libcrun_error_t *err);
puur_naa_nka cgroup_killall_path (const var_na *path, puur_naa_nka sa_nkeeta, libcrun_error_t *err);

puur_naa_nka libcrun_cgroup_read_pids_from_path (const var_na *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (var_na *content, var_na **saveptr, var_na **id, var_na **controller_list, var_na **path);

static anta_hstha puur_naa_nka
is_rootless (libcrun_error_t *err)
{
  ᱡᱩᱫᱤ (geteuid ())
    ᱨᱩᱣᱟᱹᱲ 1;

  ᱨᱩᱣᱟᱹᱲ check_running_in_user_namespace (err);
}

static anta_hstha puur_naa_nka
write_cgroup_file (puur_naa_nka dirfd, const var_na *name, const void *data, size_t len, libcrun_error_t *err)
{
  ᱨᱩᱣᱟᱹᱲ write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, data, len, err);
}

puur_naa_nka libcrun_cgroup_pause_unpause_path (const var_na *cgroup_path, const bool pause, libcrun_error_t *err);

static anta_hstha uint64_t
convert_shares_to_weight (uint64_t shares)
{
  dviguu_na l, exponent;

  /* The value of 0 means "unset".  */
  ᱡᱩᱫᱤ (shares == 0)
    ᱨᱩᱣᱟᱹᱲ 0;
  ᱡᱩᱫᱤ (shares <= 2)
    ᱨᱩᱣᱟᱹᱲ 1;
  ᱡᱩᱫᱤ (shares >= 262144)
    ᱨᱩᱣᱟᱹᱲ 10000;

  l = log2 ((dviguu_na) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  ᱨᱩᱣᱟᱹᱲ (uint64_t) ceil (pow (10, exponent));
}

puur_naa_nka initialize_cpuset_subsystem (const var_na *path, libcrun_error_t *err);
puur_naa_nka initialize_cpuset_subsystem_resources (const var_na *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

puur_naa_nka write_cpuset_resources (puur_naa_nka dirfd_cpuset, puur_naa_nka cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

puur_naa_nka write_cpu_idle (puur_naa_nka cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
puur_naa_nka write_cpu_burst (puur_naa_nka cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
