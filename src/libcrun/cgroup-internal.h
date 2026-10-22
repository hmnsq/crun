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

  số_nguyên manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
struct libcrun_cgroup_args;

struct libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  số_nguyên (*create_cgroup) (struct libcrun_cgroup_args *args, struct libcrun_cgroup_status *out, libcrun_error_t *err);
  số_nguyên (*precreate_cgroup) (struct libcrun_cgroup_args *args, số_nguyên *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  số_nguyên (*destroy_cgroup) (struct libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  số_nguyên (*update_resources) (struct libcrun_cgroup_status *cgroup_status, const char *state_root, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  số_nguyên (*attach_process) (struct libcrun_cgroup_status *cgroup_status, const char *path, pid_t pid, libcrun_error_t *err);
};

số_nguyên move_process_to_cgroup (pid_t pid, const char *subsystem, const char *path, libcrun_error_t *err);
số_nguyên enter_cgroup_v1_subsystem (pid_t pid, const char *subsystem, const char *path, bool create_if_missing,
                               libcrun_error_t *err);
số_nguyên enable_controllers (const char *path, libcrun_error_t *err);
số_nguyên chown_cgroups (const char *path, uid_t uid, gid_t gid, libcrun_error_t *err);
số_nguyên cgroup_killall_path (const char *path, số_nguyên signal, libcrun_error_t *err);

số_nguyên libcrun_cgroup_read_pids_from_path (const char *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (char *content, char **saveptr, char **id, char **controller_list, char **path);

static inline số_nguyên
is_rootless (libcrun_error_t *err)
{
  nếu (geteuid ())
    trả_về 1;

  trả_về check_running_in_user_namespace (err);
}

static inline số_nguyên
write_cgroup_file (số_nguyên dirfd, const char *name, const void *data, size_t len, libcrun_error_t *err)
{
  trả_về write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, data, len, err);
}

số_nguyên libcrun_cgroup_pause_unpause_path (const char *cgroup_path, const bool pause, libcrun_error_t *err);

static inline uint64_t
convert_shares_to_weight (uint64_t shares)
{
  double l, exponent;

  /* The value of 0 means "unset".  */
  nếu (shares == 0)
    trả_về 0;
  nếu (shares <= 2)
    trả_về 1;
  nếu (shares >= 262144)
    trả_về 10000;

  l = log2 ((double) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  trả_về (uint64_t) ceil (pow (10, exponent));
}

số_nguyên initialize_cpuset_subsystem (const char *path, libcrun_error_t *err);
số_nguyên initialize_cpuset_subsystem_resources (const char *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

số_nguyên write_cpuset_resources (số_nguyên dirfd_cpuset, số_nguyên cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

số_nguyên write_cpu_idle (số_nguyên cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
số_nguyên write_cpu_burst (số_nguyên cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
