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

#અન્તર્ભાવ "container.h"
#અન્તર્ભાવ "utils.h"

#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <math.h>

ગણના
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

સંરચના libcrun_cgroup_status
{
  અક્ષર *path;
  અક્ષર *scope;

  પૂર્ણાંક manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
સંરચના libcrun_cgroup_args;

સંરચના libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  પૂર્ણાંક (*create_cgroup) (સંરચના libcrun_cgroup_args *args, સંરચના libcrun_cgroup_status *out, libcrun_error_t *err);
  પૂર્ણાંક (*precreate_cgroup) (સંરચના libcrun_cgroup_args *args, પૂર્ણાંક *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  પૂર્ણાંક (*destroy_cgroup) (સંરચના libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  પૂર્ણાંક (*update_resources) (સંરચના libcrun_cgroup_status *cgroup_status, અચલ અક્ષર *state_મૂળ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  પૂર્ણાંક (*attach_process) (સંરચના libcrun_cgroup_status *cgroup_status, અચલ અક્ષર *path, pid_t pid, libcrun_error_t *err);
};

પૂર્ણાંક move_process_to_cgroup (pid_t pid, અચલ અક્ષર *subsystem, અચલ અક્ષર *path, libcrun_error_t *err);
પૂર્ણાંક enter_cgroup_v1_subsystem (pid_t pid, અચલ અક્ષર *subsystem, અચલ અક્ષર *path, bool create_if_missing,
                               libcrun_error_t *err);
પૂર્ણાંક enable_controllers (અચલ અક્ષર *path, libcrun_error_t *err);
પૂર્ણાંક chown_cgroups (અચલ અક્ષર *path, uid_t uid, gid_t gid, libcrun_error_t *err);
પૂર્ણાંક cgroup_killall_path (અચલ અક્ષર *path, પૂર્ણાંક સંકેત, libcrun_error_t *err);

પૂર્ણાંક libcrun_cgroup_read_pids_from_path (અચલ અક્ષર *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (અક્ષર *content, અક્ષર **saveptr, અક્ષર **id, અક્ષર **controller_list, અક્ષર **path);

સ્થિર અંતઃરેખ પૂર્ણાંક
is_rootless (libcrun_error_t *err)
{
  જો (geteuid ())
    પાછા 1;

  પાછા check_running_in_user_namespace (err);
}

સ્થિર અંતઃરેખ પૂર્ણાંક
write_cgroup_file (પૂર્ણાંક dirfd, અચલ અક્ષર *name, અચલ શૂન્ય *data, size_t len, libcrun_error_t *err)
{
  પાછા write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, data, len, err);
}

પૂર્ણાંક libcrun_cgroup_pause_unpause_path (અચલ અક્ષર *cgroup_path, અચલ bool pause, libcrun_error_t *err);

સ્થિર અંતઃરેખ uint64_t
convert_shares_to_weight (uint64_t shares)
{
  દ્વિગુણ l, exponent;

  /* The value of 0 means "unset".  */
  જો (shares == 0)
    પાછા 0;
  જો (shares <= 2)
    પાછા 1;
  જો (shares >= 262144)
    પાછા 10000;

  l = log2 ((દ્વિગુણ) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  પાછા (uint64_t) ceil (pow (10, exponent));
}

પૂર્ણાંક initialize_cpuset_subsystem (અચલ અક્ષર *path, libcrun_error_t *err);
પૂર્ણાંક initialize_cpuset_subsystem_resources (અચલ અક્ષર *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

પૂર્ણાંક write_cpuset_resources (પૂર્ણાંક dirfd_cpuset, પૂર્ણાંક cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

પૂર્ણાંક write_cpu_idle (પૂર્ણાંક cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
પૂર્ણાંક write_cpu_burst (પૂર્ણાંક cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
