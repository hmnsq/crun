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
#جے_بنا_تعریف CGROUP_اندرونی_H
#تعریف_کرو CGROUP_اندرونی_H

#ضمن "container.h"
#ضمن "utils.h"

#ضمن <stdint.h>
#ضمن <math.h>

تعداد
{
  CGROUP_حافظہ = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

بنية libcrun_cgroup_حالت
{
  حرف *رستہ;
  حرف *scope;

  صحيح manager;

  bool bpf_dev_سیٹ;
};

/* Forward declaration for function pointers below.  */
بنية libcrun_cgroup_دلیلاں;

بنية libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  صحيح (*create_cgroup) (بنية libcrun_cgroup_دلیلاں *دلیلاں, بنية libcrun_cgroup_حالت *باہرلا, libcrun_error_t *err);
  صحيح (*precreate_cgroup) (بنية libcrun_cgroup_دلیلاں *دلیلاں, صحيح *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  صحيح (*destroy_cgroup) (بنية libcrun_cgroup_حالت *cgroup_حالت, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  صحيح (*update_resources) (بنية libcrun_cgroup_حالت *cgroup_حالت, ثابت حرف *state_جڑ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  صحيح (*attach_process) (بنية libcrun_cgroup_حالت *cgroup_حالت, ثابت حرف *رستہ, pid_t pid, libcrun_error_t *err);
};

صحيح move_process_منزل_cgroup (pid_t pid, ثابت حرف *subsystem, ثابت حرف *رستہ, libcrun_error_t *err);
صحيح enter_cgroup_v1_subsystem (pid_t pid, ثابت حرف *subsystem, ثابت حرف *رستہ, bool create_if_missing,
                               libcrun_error_t *err);
صحيح enable_controllers (ثابت حرف *رستہ, libcrun_error_t *err);
صحيح chown_cgroups (ثابت حرف *رستہ, uid_t uid, gid_t gid, libcrun_error_t *err);
صحيح cgroup_killall_رستہ (ثابت حرف *رستہ, صحيح إشارة, libcrun_error_t *err);

صحيح libcrun_cgroup_read_pids_from_رستہ (ثابت حرف *رستہ, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (حرف *content, حرف **saveptr, حرف **id, حرف **controller_list, حرف **رستہ);

ساكن مضمن صحيح
is_rootless (libcrun_error_t *err)
{
  إذا (geteuid ())
    أرجع 1;

  أرجع check_running_وچ_user_namespace (err);
}

ساكن مضمن صحيح
write_cgroup_file (صحيح dirfd, ثابت حرف *name, ثابت فراغ *انکڑے, size_t len, libcrun_error_t *err)
{
  أرجع write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, انکڑے, len, err);
}

صحيح libcrun_cgroup_pause_unpause_رستہ (ثابت حرف *cgroup_رستہ, ثابت bool pause, libcrun_error_t *err);

ساكن مضمن uint64_t
convert_shares_منزل_بھار (uint64_t shares)
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

  أرجع (uint64_t) ceil (قوت (10, exponent));
}

صحيح initialize_cpuset_subsystem (ثابت حرف *رستہ, libcrun_error_t *err);
صحيح initialize_cpuset_subsystem_resources (ثابت حرف *رستہ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

صحيح write_cpuset_resources (صحيح dirfd_cpuset, صحيح cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

صحيح write_cpu_idle (صحيح cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
صحيح write_cpu_مدت (صحيح cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#جے_انت
