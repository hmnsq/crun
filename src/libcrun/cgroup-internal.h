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

#ଅନ୍ତର୍ଭାଵ "container.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <math.h>

ଗଣନା
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

ସଂରଚନା libcrun_cgroup_status
{
  ଅକ୍ଷର *path;
  ଅକ୍ଷର *scope;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
ସଂରଚନା libcrun_cgroup_args;

ସଂରଚନା libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*create_cgroup) (ସଂରଚନା libcrun_cgroup_args *args, ସଂରଚନା libcrun_cgroup_status *out, libcrun_error_t *err);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*precreate_cgroup) (ସଂରଚନା libcrun_cgroup_args *args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*destroy_cgroup) (ସଂରଚନା libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*update_resources) (ସଂରଚନା libcrun_cgroup_status *cgroup_status, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*attach_process) (ସଂରଚନା libcrun_cgroup_status *cgroup_status, ଅଚଳ ଅକ୍ଷର *path, pid_t pid, libcrun_error_t *err);
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା move_process_to_cgroup (pid_t pid, ଅଚଳ ଅକ୍ଷର *subsystem, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା enter_cgroup_v1_subsystem (pid_t pid, ଅଚଳ ଅକ୍ଷର *subsystem, ଅଚଳ ଅକ୍ଷର *path, bool create_if_missing,
                               libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା enable_controllers (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା chown_cgroups (ଅଚଳ ଅକ୍ଷର *path, uid_t uid, gid_t gid, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_killall_path (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଙ୍କେତ, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_read_pids_from_path (ଅଚଳ ଅକ୍ଷର *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (ଅକ୍ଷର *content, ଅକ୍ଷର **saveptr, ଅକ୍ଷର **id, ଅକ୍ଷର **controller_list, ଅକ୍ଷର **path);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
is_rootless (libcrun_error_t *err)
{
  ଯଦି (geteuid ())
    ଫେରନ୍ତୁ 1;

  ଫେରନ୍ତୁ check_running_in_user_namespace (err);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_cgroup_file (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଶୂନ୍ୟ *data, size_t len, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, data, len, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_pause_unpause_path (ଅଚଳ ଅକ୍ଷର *cgroup_path, ଅଚଳ bool pause, libcrun_error_t *err);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint64_t
convert_shares_to_weight (uint64_t shares)
{
  ଦ୍ୱିଗୁଣ l, exponent;

  /* The value of 0 means "unset".  */
  ଯଦି (shares == 0)
    ଫେରନ୍ତୁ 0;
  ଯଦି (shares <= 2)
    ଫେରନ୍ତୁ 1;
  ଯଦି (shares >= 262144)
    ଫେରନ୍ତୁ 10000;

  l = log2 ((ଦ୍ୱିଗୁଣ) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  ଫେରନ୍ତୁ (uint64_t) ceil (pow (10, exponent));
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା initialize_cpuset_subsystem (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା initialize_cpuset_subsystem_resources (ଅଚଳ ଅକ୍ଷର *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_cpuset_resources (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_cpuset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_cpu_idle (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_cpu_burst (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
