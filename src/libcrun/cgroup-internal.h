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

#شامل "container.h"
#شامل "utils.h"

#شامل <stdint.h>
#شامل <math.h>

شمارش
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

ساختار libcrun_cgroup_status
{
  نویسه *path;
  نویسه *scope;

  صحیح manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
ساختار libcrun_cgroup_args;

ساختار libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  صحیح (*create_cgroup) (ساختار libcrun_cgroup_args *args, ساختار libcrun_cgroup_status *out, libcrun_error_t *err);
  صحیح (*precreate_cgroup) (ساختار libcrun_cgroup_args *args, صحیح *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  صحیح (*destroy_cgroup) (ساختار libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  صحیح (*update_resources) (ساختار libcrun_cgroup_status *cgroup_status, ثابت نویسه *state_ریشه, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  صحیح (*attach_process) (ساختار libcrun_cgroup_status *cgroup_status, ثابت نویسه *path, pid_t pid, libcrun_error_t *err);
};

صحیح move_process_to_cgroup (pid_t pid, ثابت نویسه *subsystem, ثابت نویسه *path, libcrun_error_t *err);
صحیح enter_cgroup_v1_subsystem (pid_t pid, ثابت نویسه *subsystem, ثابت نویسه *path, bool create_if_missing,
                               libcrun_error_t *err);
صحیح enable_controllers (ثابت نویسه *path, libcrun_error_t *err);
صحیح chown_cgroups (ثابت نویسه *path, uid_t uid, gid_t gid, libcrun_error_t *err);
صحیح cgroup_killall_path (ثابت نویسه *path, صحیح سیگنال, libcrun_error_t *err);

صحیح libcrun_cgroup_read_pids_from_path (ثابت نویسه *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (نویسه *content, نویسه **saveptr, نویسه **id, نویسه **controller_list, نویسه **path);

ایستا درونخطی صحیح
is_rootless (libcrun_error_t *err)
{
  اگر (geteuid ())
    بازگشت 1;

  بازگشت check_running_in_user_namespace (err);
}

ایستا درونخطی صحیح
write_cgroup_file (صحیح dirfd, ثابت نویسه *name, ثابت تهی *data, size_t len, libcrun_error_t *err)
{
  بازگشت write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, data, len, err);
}

صحیح libcrun_cgroup_pause_unpause_path (ثابت نویسه *cgroup_path, ثابت bool pause, libcrun_error_t *err);

ایستا درونخطی uint64_t
convert_shares_to_weight (uint64_t shares)
{
  دوگانه l, exponent;

  /* The value of 0 means "unset".  */
  اگر (shares == 0)
    بازگشت 0;
  اگر (shares <= 2)
    بازگشت 1;
  اگر (shares >= 262144)
    بازگشت 10000;

  l = log2 ((دوگانه) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  بازگشت (uint64_t) ceil (pow (10, exponent));
}

صحیح initialize_cpuset_subsystem (ثابت نویسه *path, libcrun_error_t *err);
صحیح initialize_cpuset_subsystem_resources (ثابت نویسه *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

صحیح write_cpuset_resources (صحیح dirfd_cpuset, صحیح cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

صحیح write_cpu_idle (صحیح cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
صحیح write_cpu_burst (صحیح cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
