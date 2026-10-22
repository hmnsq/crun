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

#ضمن "container.h"
#ضمن "utils.h"

#ضمن <stdint.h>
#ضمن <math.h>

تعداد
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

بنية libcrun_cgroup_status
{
  حرف *path;
  حرف *scope;

  صحيح manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
بنية libcrun_cgroup_args;

بنية libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  صحيح (*create_cgroup) (بنية libcrun_cgroup_args *args, بنية libcrun_cgroup_status *out, libcrun_error_t *err);
  صحيح (*precreate_cgroup) (بنية libcrun_cgroup_args *args, صحيح *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  صحيح (*destroy_cgroup) (بنية libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  صحيح (*update_resources) (بنية libcrun_cgroup_status *cgroup_status, ثابت حرف *state_ڕەگ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  صحيح (*attach_process) (بنية libcrun_cgroup_status *cgroup_status, ثابت حرف *path, pid_t pid, libcrun_error_t *err);
};

صحيح move_process_to_cgroup (pid_t pid, ثابت حرف *subsystem, ثابت حرف *path, libcrun_error_t *err);
صحيح enter_cgroup_v1_subsystem (pid_t pid, ثابت حرف *subsystem, ثابت حرف *path, bool create_if_missing,
                               libcrun_error_t *err);
صحيح enable_controllers (ثابت حرف *path, libcrun_error_t *err);
صحيح chown_cgroups (ثابت حرف *path, uid_t uid, gid_t gid, libcrun_error_t *err);
صحيح cgroup_killall_path (ثابت حرف *path, صحيح إشارة, libcrun_error_t *err);

صحيح libcrun_cgroup_read_pids_from_path (ثابت حرف *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (حرف *content, حرف **saveptr, حرف **id, حرف **controller_list, حرف **path);

ساكن مضمن صحيح
is_rootless (libcrun_error_t *err)
{
  إذا (geteuid ())
    أرجع 1;

  أرجع check_running_in_user_namespace (err);
}

ساكن مضمن صحيح
write_cgroup_file (صحيح dirfd, ثابت حرف *name, ثابت فراغ *data, size_t len, libcrun_error_t *err)
{
  أرجع write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, data, len, err);
}

صحيح libcrun_cgroup_pause_unpause_path (ثابت حرف *cgroup_path, ثابت bool pause, libcrun_error_t *err);

ساكن مضمن uint64_t
convert_shares_to_weight (uint64_t shares)
{
  مزدوج l, exponent;

  /* The value of 0 means "unset".  */
  إذا (shares == 0)
    أرجع 0;
  إذا (shares <= 2)
    أرجع 1;
  إذا (shares >= 262144)
    أرجع 10000;

  l = log2 ((مزدوج) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  أرجع (uint64_t) ceil (pow (10, exponent));
}

صحيح initialize_cpuset_subsystem (ثابت حرف *path, libcrun_error_t *err);
صحيح initialize_cpuset_subsystem_resources (ثابت حرف *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

صحيح write_cpuset_resources (صحيح dirfd_cpuset, صحيح cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

صحيح write_cpu_idle (صحيح cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
صحيح write_cpu_burst (صحيح cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
