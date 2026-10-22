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
#अगर_अघोषित CGROUP_INTERNAL_H
#घोषणा CGROUP_INTERNAL_H

#समावेश "container.h"
#समावेश "utils.h"

#समावेश <stdint.h>
#समावेश <math.h>

क्रमागत
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

काष्ठा libcrun_cgroup_status
{
  अक्षर *path;
  अक्षर *scope;

  पूर्णांक manager;

  bool bpf_dev_set;
};

/* Forward declaration for function pointers below.  */
काष्ठा libcrun_cgroup_args;

काष्ठा libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  पूर्णांक (*create_cgroup) (काष्ठा libcrun_cgroup_args *args, काष्ठा libcrun_cgroup_status *out, libcrun_error_t *err);
  पूर्णांक (*precreate_cgroup) (काष्ठा libcrun_cgroup_args *args, पूर्णांक *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  पूर्णांक (*destroy_cgroup) (काष्ठा libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  पूर्णांक (*update_resources) (काष्ठा libcrun_cgroup_status *cgroup_status, स्थिर अक्षर *state_मूल, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  पूर्णांक (*attach_process) (काष्ठा libcrun_cgroup_status *cgroup_status, स्थिर अक्षर *path, pid_t pid, libcrun_error_t *err);
};

पूर्णांक move_process_to_cgroup (pid_t pid, स्थिर अक्षर *subsystem, स्थिर अक्षर *path, libcrun_error_t *err);
पूर्णांक enter_cgroup_v1_subsystem (pid_t pid, स्थिर अक्षर *subsystem, स्थिर अक्षर *path, bool create_if_missing,
                               libcrun_error_t *err);
पूर्णांक enable_controllers (स्थिर अक्षर *path, libcrun_error_t *err);
पूर्णांक chown_cgroups (स्थिर अक्षर *path, uid_t uid, gid_t gid, libcrun_error_t *err);
पूर्णांक cgroup_killall_path (स्थिर अक्षर *path, पूर्णांक संकेत, libcrun_error_t *err);

पूर्णांक libcrun_cgroup_read_pids_from_path (स्थिर अक्षर *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (अक्षर *content, अक्षर **saveptr, अक्षर **id, अक्षर **controller_list, अक्षर **path);

अटल अंतरभूत पूर्णांक
is_rootless (libcrun_error_t *err)
{
  अगर (geteuid ())
    वापस 1;

  वापस check_running_in_user_namespace (err);
}

अटल अंतरभूत पूर्णांक
write_cgroup_file (पूर्णांक dirfd, स्थिर अक्षर *name, स्थिर व्योम *तथ्य, माप_प्रकार len, libcrun_error_t *err)
{
  वापस write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, तथ्य, len, err);
}

पूर्णांक libcrun_cgroup_pause_unpause_path (स्थिर अक्षर *cgroup_path, स्थिर bool छोड़ो, libcrun_error_t *err);

अटल अंतरभूत uint64_t
convert_shares_to_weight (uint64_t shares)
{
  द्विगुन l, exponent;

  /* The value of 0 means "unset".  */
  अगर (shares == 0)
    वापस 0;
  अगर (shares <= 2)
    वापस 1;
  अगर (shares >= 262144)
    वापस 10000;

  l = log2 ((द्विगुन) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  वापस (uint64_t) उच्चमान (घात (10, exponent));
}

पूर्णांक initialize_cpuset_subsystem (स्थिर अक्षर *path, libcrun_error_t *err);
पूर्णांक initialize_cpuset_subsystem_resources (स्थिर अक्षर *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

पूर्णांक write_cpuset_resources (पूर्णांक dirfd_cpuset, पूर्णांक cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

पूर्णांक write_cpu_idle (पूर्णांक cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
पूर्णांक write_cpu_burst (पूर्णांक cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#पूर्ण_अगर
