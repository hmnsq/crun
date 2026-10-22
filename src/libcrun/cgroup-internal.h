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

#অন্তর্ভাব "container.h"
#অন্তর্ভাব "utils.h"

#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <math.h>

গণনা
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

সংরচনা libcrun_cgroup_status
{
  বর্ণ *path;
  বর্ণ *scope;

  পূর্ণাঙ্ক manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
সংরচনা libcrun_cgroup_args;

সংরচনা libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  পূর্ণাঙ্ক (*create_cgroup) (সংরচনা libcrun_cgroup_args *args, সংরচনা libcrun_cgroup_status *out, libcrun_error_t *err);
  পূর্ণাঙ্ক (*precreate_cgroup) (সংরচনা libcrun_cgroup_args *args, পূর্ণাঙ্ক *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  পূর্ণাঙ্ক (*destroy_cgroup) (সংরচনা libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  পূর্ণাঙ্ক (*update_resources) (সংরচনা libcrun_cgroup_status *cgroup_status, ধ্রুব বর্ণ *state_মূল, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  পূর্ণাঙ্ক (*attach_process) (সংরচনা libcrun_cgroup_status *cgroup_status, ধ্রুব বর্ণ *path, pid_t pid, libcrun_error_t *err);
};

পূর্ণাঙ্ক move_process_to_cgroup (pid_t pid, ধ্রুব বর্ণ *subsystem, ধ্রুব বর্ণ *path, libcrun_error_t *err);
পূর্ণাঙ্ক enter_cgroup_v1_subsystem (pid_t pid, ধ্রুব বর্ণ *subsystem, ধ্রুব বর্ণ *path, bool create_if_missing,
                               libcrun_error_t *err);
পূর্ণাঙ্ক enable_controllers (ধ্রুব বর্ণ *path, libcrun_error_t *err);
পূর্ণাঙ্ক chown_cgroups (ধ্রুব বর্ণ *path, uid_t uid, gid_t gid, libcrun_error_t *err);
পূর্ণাঙ্ক cgroup_killall_path (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক সঙ্কেত, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_cgroup_read_pids_from_path (ধ্রুব বর্ণ *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (বর্ণ *content, বর্ণ **saveptr, বর্ণ **id, বর্ণ **controller_list, বর্ণ **path);

স্থির অন্তঃস্থ পূর্ণাঙ্ক
is_rootless (libcrun_error_t *err)
{
  যদি (geteuid ())
    প্রত্যাগম 1;

  প্রত্যাগম check_running_in_user_namespace (err);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
write_cgroup_file (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *name, ধ্রুব শূন্য *data, size_t len, libcrun_error_t *err)
{
  প্রত্যাগম write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, data, len, err);
}

পূর্ণাঙ্ক libcrun_cgroup_pause_unpause_path (ধ্রুব বর্ণ *cgroup_path, ধ্রুব bool pause, libcrun_error_t *err);

স্থির অন্তঃস্থ uint64_t
convert_shares_to_weight (uint64_t shares)
{
  দ্বিগুণ l, exponent;

  /* The value of 0 means "unset".  */
  যদি (shares == 0)
    প্রত্যাগম 0;
  যদি (shares <= 2)
    প্রত্যাগম 1;
  যদি (shares >= 262144)
    প্রত্যাগম 10000;

  l = log2 ((দ্বিগুণ) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  প্রত্যাগম (uint64_t) ceil (pow (10, exponent));
}

পূর্ণাঙ্ক initialize_cpuset_subsystem (ধ্রুব বর্ণ *path, libcrun_error_t *err);
পূর্ণাঙ্ক initialize_cpuset_subsystem_resources (ধ্রুব বর্ণ *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

পূর্ণাঙ্ক write_cpuset_resources (পূর্ণাঙ্ক dirfd_cpuset, পূর্ণাঙ্ক cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

পূর্ণাঙ্ক write_cpu_idle (পূর্ণাঙ্ক cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
পূর্ণাঙ্ক write_cpu_burst (পূর্ণাঙ্ক cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
