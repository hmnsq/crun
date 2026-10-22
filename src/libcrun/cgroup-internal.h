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

কাঠামো libcrun_cgroup_status
{
  অক্ষর *path;
  অক্ষর *scope;

  পূর্ণসংখ্যা manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
কাঠামো libcrun_cgroup_args;

কাঠামো libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  পূর্ণসংখ্যা (*create_cgroup) (কাঠামো libcrun_cgroup_args *args, কাঠামো libcrun_cgroup_status *out, libcrun_error_t *err);
  পূর্ণসংখ্যা (*precreate_cgroup) (কাঠামো libcrun_cgroup_args *args, পূর্ণসংখ্যা *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  পূর্ণসংখ্যা (*destroy_cgroup) (কাঠামো libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  পূর্ণসংখ্যা (*update_resources) (কাঠামো libcrun_cgroup_status *cgroup_status, ধ্রুবক অক্ষর *state_মূল, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  পূর্ণসংখ্যা (*attach_process) (কাঠামো libcrun_cgroup_status *cgroup_status, ধ্রুবক অক্ষর *path, pid_t pid, libcrun_error_t *err);
};

পূর্ণসংখ্যা move_process_to_cgroup (pid_t pid, ধ্রুবক অক্ষর *subsystem, ধ্রুবক অক্ষর *path, libcrun_error_t *err);
পূর্ণসংখ্যা enter_cgroup_v1_subsystem (pid_t pid, ধ্রুবক অক্ষর *subsystem, ধ্রুবক অক্ষর *path, bool create_if_missing,
                               libcrun_error_t *err);
পূর্ণসংখ্যা enable_controllers (ধ্রুবক অক্ষর *path, libcrun_error_t *err);
পূর্ণসংখ্যা chown_cgroups (ধ্রুবক অক্ষর *path, uid_t uid, gid_t gid, libcrun_error_t *err);
পূর্ণসংখ্যা cgroup_killall_path (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা সংকেত, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_cgroup_read_pids_from_path (ধ্রুবক অক্ষর *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (অক্ষর *content, অক্ষর **saveptr, অক্ষর **id, অক্ষর **controller_list, অক্ষর **path);

স্থির অন্তঃরেখ পূর্ণসংখ্যা
is_rootless (libcrun_error_t *err)
{
  যদি (geteuid ())
    ফেরত 1;

  ফেরত check_running_in_user_namespace (err);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
write_cgroup_file (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *name, ধ্রুবক শূন্য *তথ্য, size_t len, libcrun_error_t *err)
{
  ফেরত write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, তথ্য, len, err);
}

পূর্ণসংখ্যা libcrun_cgroup_pause_unpause_path (ধ্রুবক অক্ষর *cgroup_path, ধ্রুবক bool pause, libcrun_error_t *err);

স্থির অন্তঃরেখ uint64_t
convert_shares_to_weight (uint64_t shares)
{
  দ্বিগুণ l, exponent;

  /* The value of 0 means "unset".  */
  যদি (shares == 0)
    ফেরত 0;
  যদি (shares <= 2)
    ফেরত 1;
  যদি (shares >= 262144)
    ফেরত 10000;

  l = log2 ((দ্বিগুণ) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  ফেরত (uint64_t) ceil (pow (10, exponent));
}

পূর্ণসংখ্যা initialize_cpuset_subsystem (ধ্রুবক অক্ষর *path, libcrun_error_t *err);
পূর্ণসংখ্যা initialize_cpuset_subsystem_resources (ধ্রুবক অক্ষর *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

পূর্ণসংখ্যা write_cpuset_resources (পূর্ণসংখ্যা dirfd_cpuset, পূর্ণসংখ্যা cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

পূর্ণসংখ্যা write_cpu_idle (পূর্ণসংখ্যা cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
পূর্ণসংখ্যা write_cpu_burst (পূর্ণসংখ্যা cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
