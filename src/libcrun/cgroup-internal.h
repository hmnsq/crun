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
#اگر_غیر_تعریف_شدہ CGROUP_اندرونی_H
#تعریف_کرو CGROUP_اندرونی_H

#شامل "container.h"
#شامل "utils.h"

#شامل <stdint.h>
#شامل <math.h>

شمار
{
  CGROUP_حافظہ = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

ساخت libcrun_cgroup_حالت
{
  حرف *راستہ;
  حرف *scope;

  صحیح manager;

  bool bpf_dev_سیٹ;
};

/* Forward declaration for function pointers below.  */
ساخت libcrun_cgroup_دلائل;

ساخت libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  صحیح (*create_cgroup) (ساخت libcrun_cgroup_دلائل *دلائل, ساخت libcrun_cgroup_حالت *برآمد, libcrun_error_t *err);
  صحیح (*precreate_cgroup) (ساخت libcrun_cgroup_دلائل *دلائل, صحیح *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  صحیح (*destroy_cgroup) (ساخت libcrun_cgroup_حالت *cgroup_حالت, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  صحیح (*update_resources) (ساخت libcrun_cgroup_حالت *cgroup_حالت, ثابت حرف *state_جڑ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  صحیح (*attach_process) (ساخت libcrun_cgroup_حالت *cgroup_حالت, ثابت حرف *راستہ, pid_t pid, libcrun_error_t *err);
};

صحیح move_process_مقصد_cgroup (pid_t pid, ثابت حرف *subsystem, ثابت حرف *راستہ, libcrun_error_t *err);
صحیح enter_cgroup_v1_subsystem (pid_t pid, ثابت حرف *subsystem, ثابت حرف *راستہ, bool create_if_missing,
                               libcrun_error_t *err);
صحیح enable_controllers (ثابت حرف *راستہ, libcrun_error_t *err);
صحیح chown_cgroups (ثابت حرف *راستہ, uid_t uid, gid_t gid, libcrun_error_t *err);
صحیح cgroup_killall_راستہ (ثابت حرف *راستہ, صحیح اشارہ, libcrun_error_t *err);

صحیح libcrun_cgroup_read_pids_from_راستہ (ثابت حرف *راستہ, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (حرف *content, حرف **saveptr, حرف **id, حرف **controller_list, حرف **راستہ);

ساکن اندرونی صحیح
is_rootless (libcrun_error_t *err)
{
  اگر (geteuid ())
    واپس 1;

  واپس check_running_اندر_user_namespace (err);
}

ساکن اندرونی صحیح
write_cgroup_file (صحیح dirfd, ثابت حرف *name, ثابت خالی *ڈیٹا, size_t len, libcrun_error_t *err)
{
  واپس write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, ڈیٹا, len, err);
}

صحیح libcrun_cgroup_pause_unpause_راستہ (ثابت حرف *cgroup_راستہ, ثابت bool pause, libcrun_error_t *err);

ساکن اندرونی uint64_t
convert_shares_مقصد_وزن (uint64_t shares)
{
  دوہرا l, exponent;

  /* The value of 0 means "unset".  */
  اگر (shares == 0)
    واپس 0;
  اگر (shares <= 2)
    واپس 1;
  اگر (shares >= 262144)
    واپس 10000;

  l = log2 ((دوہرا) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  واپس (uint64_t) ceil (قوت (10, exponent));
}

صحیح initialize_cpuset_subsystem (ثابت حرف *راستہ, libcrun_error_t *err);
صحیح initialize_cpuset_subsystem_resources (ثابت حرف *راستہ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

صحیح write_cpuset_resources (صحیح dirfd_cpuset, صحیح cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

صحیح write_cpu_idle (صحیح cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
صحیح write_cpu_مدت (صحیح cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#اگر_ختم
