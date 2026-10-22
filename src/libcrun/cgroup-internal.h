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
#यदि_अपरिभाषितम् CGROUP_आन्तरिकम्_H
#परिभाषय CGROUP_आन्तरिकम्_H

#अन्तर्भाव "container.h"
#अन्तर्भाव "utils.h"

#अन्तर्भाव <stdint.h>
#अन्तर्भाव <math.h>

गणना
{
  CGROUP_स्मृतिः = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

संरचना libcrun_cgroup_स्थितिः
{
  वर्ण *मार्गः;
  वर्ण *scope;

  पूर्णाङ्क manager;

  bool bpf_dev_समुच्चयः;
};

/* Forward declaration for function pointers below.  */
संरचना libcrun_cgroup_तर्काः;

संरचना libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  पूर्णाङ्क (*create_cgroup) (संरचना libcrun_cgroup_तर्काः *तर्काः, संरचना libcrun_cgroup_स्थितिः *बहिर्गमनम्, libcrun_error_t *err);
  पूर्णाङ्क (*precreate_cgroup) (संरचना libcrun_cgroup_तर्काः *तर्काः, पूर्णाङ्क *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  पूर्णाङ्क (*destroy_cgroup) (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  पूर्णाङ्क (*update_resources) (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः, ध्रुव वर्ण *state_मूलम्, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  पूर्णाङ्क (*attach_process) (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः, ध्रुव वर्ण *मार्गः, pid_t pid, libcrun_error_t *err);
};

पूर्णाङ्क move_process_गन्तव्यम्_cgroup (pid_t pid, ध्रुव वर्ण *subsystem, ध्रुव वर्ण *मार्गः, libcrun_error_t *err);
पूर्णाङ्क enter_cgroup_v1_subsystem (pid_t pid, ध्रुव वर्ण *subsystem, ध्रुव वर्ण *मार्गः, bool create_if_missing,
                               libcrun_error_t *err);
पूर्णाङ्क enable_controllers (ध्रुव वर्ण *मार्गः, libcrun_error_t *err);
पूर्णाङ्क chown_cgroups (ध्रुव वर्ण *मार्गः, uid_t uid, gid_t gid, libcrun_error_t *err);
पूर्णाङ्क cgroup_killall_मार्गः (ध्रुव वर्ण *मार्गः, पूर्णाङ्क सङ्केत, libcrun_error_t *err);

पूर्णाङ्क libcrun_cgroup_read_pids_from_मार्गः (ध्रुव वर्ण *मार्गः, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (वर्ण *content, वर्ण **saveptr, वर्ण **id, वर्ण **controller_list, वर्ण **मार्गः);

स्थिर अन्तःस्थ पूर्णाङ्क
is_rootless (libcrun_error_t *err)
{
  यदि (geteuid ())
    प्रत्यागम 1;

  प्रत्यागम check_running_अन्तर्_user_namespace (err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
write_cgroup_file (पूर्णाङ्क dirfd, ध्रुव वर्ण *name, ध्रुव शून्य *दत्त, size_t len, libcrun_error_t *err)
{
  प्रत्यागम write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, दत्त, len, err);
}

पूर्णाङ्क libcrun_cgroup_pause_unpause_मार्गः (ध्रुव वर्ण *cgroup_मार्गः, ध्रुव bool pause, libcrun_error_t *err);

स्थिर अन्तःस्थ uint64_t
convert_shares_गन्तव्यम्_weight (uint64_t shares)
{
  द्विगुण l, exponent;

  /* The value of 0 means "unset".  */
  यदि (shares == 0)
    प्रत्यागम 0;
  यदि (shares <= 2)
    प्रत्यागम 1;
  यदि (shares >= 262144)
    प्रत्यागम 10000;

  l = log2 ((द्विगुण) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  प्रत्यागम (uint64_t) ceil (घातः (10, exponent));
}

पूर्णाङ्क initialize_cpuset_subsystem (ध्रुव वर्ण *मार्गः, libcrun_error_t *err);
पूर्णाङ्क initialize_cpuset_subsystem_resources (ध्रुव वर्ण *मार्गः, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

पूर्णाङ्क write_cpuset_resources (पूर्णाङ्क dirfd_cpuset, पूर्णाङ्क cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

पूर्णाङ्क write_cpu_idle (पूर्णाङ्क cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
पूर्णाङ्क write_cpu_burst (पूर्णाङ्क cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#यदि_अन्तः
