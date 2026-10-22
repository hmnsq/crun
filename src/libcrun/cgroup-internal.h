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
#నిర్వచితం_కాకపోతే CGROUP_అంతర్గత_H
#నిర్వచించు CGROUP_అంతర్గత_H

#అన్తర్భావ "container.h"
#అన్తర్భావ "utils.h"

#అన్తర్భావ <stdint.h>
#అన్తర్భావ <math.h>

గణన
{
  CGROUP_జ్ఞాపకం = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

నిర్మాణం libcrun_cgroup_స్థితి
{
  అక్షరం *మార్గం;
  అక్షరం *scope;

  పూర్ణాంకం manager;

  bool bpf_dev_సమితి;
};

/* Forward declaration for function pointers below.  */
నిర్మాణం libcrun_cgroup_వాదనలు;

నిర్మాణం libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  పూర్ణాంకం (*create_cgroup) (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, నిర్మాణం libcrun_cgroup_స్థితి *బహిర్గతం, libcrun_error_t *err);
  పూర్ణాంకం (*precreate_cgroup) (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, పూర్ణాంకం *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  పూర్ణాంకం (*destroy_cgroup) (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  పూర్ణాంకం (*update_resources) (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి, స్థిరరాశి అక్షరం *state_మూలం, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
  /* Move PID to the cgroup PATH (the container cgroup or its sub-cgroup) by
     other means than writing to cgroup.procs.  Optional.  */
  పూర్ణాంకం (*attach_process) (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి, స్థిరరాశి అక్షరం *మార్గం, pid_t pid, libcrun_error_t *err);
};

పూర్ణాంకం move_process_గమ్యం_cgroup (pid_t pid, స్థిరరాశి అక్షరం *subsystem, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err);
పూర్ణాంకం enter_cgroup_v1_subsystem (pid_t pid, స్థిరరాశి అక్షరం *subsystem, స్థిరరాశి అక్షరం *మార్గం, bool create_if_missing,
                               libcrun_error_t *err);
పూర్ణాంకం enable_controllers (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err);
పూర్ణాంకం chown_cgroups (స్థిరరాశి అక్షరం *మార్గం, uid_t uid, gid_t gid, libcrun_error_t *err);
పూర్ణాంకం cgroup_killall_మార్గం (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం సంకేతం, libcrun_error_t *err);

పూర్ణాంకం libcrun_cgroup_read_pids_from_మార్గం (స్థిరరాశి అక్షరం *మార్గం, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (అక్షరం *content, అక్షరం **saveptr, అక్షరం **id, అక్షరం **controller_list, అక్షరం **మార్గం);

స్థిర అంతఃవరుస పూర్ణాంకం
is_rootless (libcrun_error_t *err)
{
  ఒకవేళ (geteuid ())
    తిరిగి 1;

  తిరిగి check_running_లోపల_user_namespace (err);
}

స్థిర అంతఃవరుస పూర్ణాంకం
write_cgroup_file (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *name, స్థిరరాశి శూన్య *దత్తాంశం, size_t len, libcrun_error_t *err)
{
  తిరిగి write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, name, దత్తాంశం, len, err);
}

పూర్ణాంకం libcrun_cgroup_pause_unpause_మార్గం (స్థిరరాశి అక్షరం *cgroup_మార్గం, స్థిరరాశి bool pause, libcrun_error_t *err);

స్థిర అంతఃవరుస uint64_t
convert_shares_గమ్యం_బరువు (uint64_t shares)
{
  ద్విగుణ l, exponent;

  /* The value of 0 means "unset".  */
  ఒకవేళ (shares == 0)
    తిరిగి 0;
  ఒకవేళ (shares <= 2)
    తిరిగి 1;
  ఒకవేళ (shares >= 262144)
    తిరిగి 10000;

  l = log2 ((ద్విగుణ) shares);

  /* Quadratic function which fits min, max, and default.  */
  exponent = (l * l + 125 * l) / 612.0 - 7.0 / 34.0;

  తిరిగి (uint64_t) ceil (ఘాతం (10, exponent));
}

పూర్ణాంకం initialize_cpuset_subsystem (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err);
పూర్ణాంకం initialize_cpuset_subsystem_resources (స్థిరరాశి అక్షరం *మార్గం, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

పూర్ణాంకం write_cpuset_resources (పూర్ణాంకం dirfd_cpuset, పూర్ణాంకం cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

పూర్ణాంకం write_cpu_idle (పూర్ణాంకం cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);
పూర్ణాంకం write_cpu_వ్యవధి (పూర్ణాంకం cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#అయితే_ముగింపు
