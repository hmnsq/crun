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

enum
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

struct libcrun_cgroup_status
{
  char *path;
  char *scope;

  цяло manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
struct libcrun_cgroup_args;

struct libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  цяло (*create_cgroup) (struct libcrun_cgroup_args *args, struct libcrun_cgroup_status *out, libcrun_error_t *err);
  цяло (*precreate_cgroup) (struct libcrun_cgroup_args *args, цяло *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  цяло (*destroy_cgroup) (struct libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  цяло (*update_resources) (struct libcrun_cgroup_status *cgroup_status, const char *state_root, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  цяло (*attach_process) (struct libcrun_cgroup_status *cgroup_status, const char *path, pid_t pid, libcrun_error_t *err);
};

цяло move_process_to_cgroup (pid_t pid, const char *subsystem, const char *path, libcrun_error_t *err);
цяло enter_cgroup_v1_subsystem (pid_t pid, const char *subsystem, const char *path, bool create_if_missing,
                               libcrun_error_t *err);
цяло enable_controllers (const char *path, libcrun_error_t *err);
цяло chown_cgroups (const char *path, uid_t uid, gid_t gid, libcrun_error_t *err);
цяло cgroup_killall_path (const char *path, цяло signal, libcrun_error_t *err);

цяло libcrun_cgroup_read_pids_from_path (const char *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (char *content, char **saveptr, char **id, char **controller_list, char **path);

static inline цяло
is_rootless (libcrun_error_t *err)
{
  ако (geteuid ())
    върни 1;

  върни check_running_in_user_namespace (err);
}

static inline цяло
write_cgroup_file (цяло dirfd, const char *name, const void *data, size_t len, libcrun_error_t *err)
{
  върни write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, data, len, err);
}

цяло libcrun_cgroup_pause_unpause_path (const char *cgroup_path, const bool pause, libcrun_error_t *err);

static inline uint64_t
convert_shares_to_weight (uint64_t shares)
{
  double l, exponent;

  /* The value of 0 means "unset".  */
  ако (shares == 0)
    върни 0;
  ако (shares <= 2)
    върни 1;
  ако (shares >= 262144)
    върни 10000;

  l = log2 ((double) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  върни (uint64_t) ceil (pow (10, exponent));
}

цяло initialize_cpuset_subsystem (const char *path, libcrun_error_t *err);
цяло initialize_cpuset_subsystem_resources (const char *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

цяло write_cpuset_resources (цяло dirfd_cpuset, цяло cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

цяло write_cpu_idle (цяло cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
цяло write_cpu_burst (цяло cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
