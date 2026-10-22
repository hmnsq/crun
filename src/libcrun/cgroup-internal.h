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

#അന്തര്ഭാവ "container.h"
#അന്തര്ഭാവ "utils.h"

#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <math.h>

എണ്ണൽ
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

ഘടന libcrun_cgroup_status
{
  അക്ഷരം *path;
  അക്ഷരം *scope;

  പൂർണ്ണസംഖ്യ manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
ഘടന libcrun_cgroup_args;

ഘടന libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  പൂർണ്ണസംഖ്യ (*create_cgroup) (ഘടന libcrun_cgroup_args *args, ഘടന libcrun_cgroup_status *out, libcrun_error_t *err);
  പൂർണ്ണസംഖ്യ (*precreate_cgroup) (ഘടന libcrun_cgroup_args *args, പൂർണ്ണസംഖ്യ *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  പൂർണ്ണസംഖ്യ (*destroy_cgroup) (ഘടന libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  പൂർണ്ണസംഖ്യ (*update_resources) (ഘടന libcrun_cgroup_status *cgroup_status, സ്ഥിരം അക്ഷരം *state_വേര്, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  പൂർണ്ണസംഖ്യ (*attach_process) (ഘടന libcrun_cgroup_status *cgroup_status, സ്ഥിരം അക്ഷരം *path, pid_t pid, libcrun_error_t *err);
};

പൂർണ്ണസംഖ്യ move_process_to_cgroup (pid_t pid, സ്ഥിരം അക്ഷരം *subsystem, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ enter_cgroup_v1_subsystem (pid_t pid, സ്ഥിരം അക്ഷരം *subsystem, സ്ഥിരം അക്ഷരം *path, bool create_if_missing,
                               libcrun_error_t *err);
പൂർണ്ണസംഖ്യ enable_controllers (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ chown_cgroups (സ്ഥിരം അക്ഷരം *path, uid_t uid, gid_t gid, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ cgroup_killall_path (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ സൂചന, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_cgroup_read_pids_from_path (സ്ഥിരം അക്ഷരം *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (അക്ഷരം *content, അക്ഷരം **saveptr, അക്ഷരം **id, അക്ഷരം **controller_list, അക്ഷരം **path);

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
is_rootless (libcrun_error_t *err)
{
  എങ്കിൽ (geteuid ())
    തിരികെ 1;

  തിരികെ check_running_in_user_namespace (err);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
write_cgroup_file (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം ശൂന്യം *data, size_t len, libcrun_error_t *err)
{
  തിരികെ write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, data, len, err);
}

പൂർണ്ണസംഖ്യ libcrun_cgroup_pause_unpause_path (സ്ഥിരം അക്ഷരം *cgroup_path, സ്ഥിരം bool pause, libcrun_error_t *err);

സ്ഥിതം വരിയിൽ uint64_t
convert_shares_to_weight (uint64_t shares)
{
  ഇരട്ട l, exponent;

  /* The value of 0 means "unset".  */
  എങ്കിൽ (shares == 0)
    തിരികെ 0;
  എങ്കിൽ (shares <= 2)
    തിരികെ 1;
  എങ്കിൽ (shares >= 262144)
    തിരികെ 10000;

  l = log2 ((ഇരട്ട) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  തിരികെ (uint64_t) ceil (pow (10, exponent));
}

പൂർണ്ണസംഖ്യ initialize_cpuset_subsystem (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ initialize_cpuset_subsystem_resources (സ്ഥിരം അക്ഷരം *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ write_cpuset_resources (പൂർണ്ണസംഖ്യ dirfd_cpuset, പൂർണ്ണസംഖ്യ cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ write_cpu_idle (പൂർണ്ണസംഖ്യ cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ write_cpu_burst (പൂർണ്ണസംഖ്യ cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif
