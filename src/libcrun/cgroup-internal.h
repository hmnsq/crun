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

#அந்தர்பாவ "container.h"
#அந்தர்பாவ "utils.h"

#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <math.h>

எண்ணிக்கை
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

கட்டமைப்பு libcrun_cgroup_status
{
  எழுத்து *path;
  எழுத்து *scope;

  முழு manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
கட்டமைப்பு libcrun_cgroup_args;

கட்டமைப்பு libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  முழு (*create_cgroup) (கட்டமைப்பு libcrun_cgroup_args *args, கட்டமைப்பு libcrun_cgroup_status *out, libcrun_error_t *err);
  முழு (*precreate_cgroup) (கட்டமைப்பு libcrun_cgroup_args *args, முழு *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  முழு (*destroy_cgroup) (கட்டமைப்பு libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  முழு (*update_resources) (கட்டமைப்பு libcrun_cgroup_status *cgroup_status, மாறிலி எழுத்து *state_வேர், runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  முழு (*attach_process) (கட்டமைப்பு libcrun_cgroup_status *cgroup_status, மாறிலி எழுத்து *path, pid_t pid, libcrun_error_t *err);
};

முழு move_process_to_cgroup (pid_t pid, மாறிலி எழுத்து *subsystem, மாறிலி எழுத்து *path, libcrun_error_t *err);
முழு enter_cgroup_v1_subsystem (pid_t pid, மாறிலி எழுத்து *subsystem, மாறிலி எழுத்து *path, bool create_if_missing,
                               libcrun_error_t *err);
முழு enable_controllers (மாறிலி எழுத்து *path, libcrun_error_t *err);
முழு chown_cgroups (மாறிலி எழுத்து *path, uid_t uid, gid_t gid, libcrun_error_t *err);
முழு cgroup_killall_path (மாறிலி எழுத்து *path, முழு சமிக்ஞை, libcrun_error_t *err);

முழு libcrun_cgroup_read_pids_from_path (மாறிலி எழுத்து *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (எழுத்து *content, எழுத்து **saveptr, எழுத்து **id, எழுத்து **controller_list, எழுத்து **path);

நிலையான உள்வரி முழு
is_rootless (libcrun_error_t *err)
{
  எனில் (geteuid ())
    திரும்பு 1;

  திரும்பு check_running_in_user_namespace (err);
}

நிலையான உள்வரி முழு
write_cgroup_file (முழு dirfd, மாறிலி எழுத்து *name, மாறிலி வெறுமை *data, size_t len, libcrun_error_t *err)
{
  திரும்பு write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, data, len, err);
}

முழு libcrun_cgroup_pause_unpause_path (மாறிலி எழுத்து *cgroup_path, மாறிலி bool pause, libcrun_error_t *err);

நிலையான உள்வரி uint64_t
convert_shares_to_weight (uint64_t shares)
{
  இரட்டை l, exponent;

  /* The value of 0 means "unset".  */
  எனில் (shares == 0)
    திரும்பு 0;
  எனில் (shares <= 2)
    திரும்பு 1;
  எனில் (shares >= 262144)
    திரும்பு 10000;

  l = log2 ((இரட்டை) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  திரும்பு (uint64_t) ceil (pow (10, exponent));
}

முழு initialize_cpuset_subsystem (மாறிலி எழுத்து *path, libcrun_error_t *err);
முழு initialize_cpuset_subsystem_resources (மாறிலி எழுத்து *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

முழு write_cpuset_resources (முழு dirfd_cpuset, முழு cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

முழு write_cpu_idle (முழு cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
முழு write_cpu_burst (முழு cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
