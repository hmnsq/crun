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
#నిర్వచితం_కాకపోతే CGROUP_H
#నిర్వచించు CGROUP_H

#అన్తర్భావ "container.h"
#అన్తర్భావ "string_map.h"
#అన్తర్భావ <unistd.h>

#నిర్వచితం_కాకపోతే CGROUP_మూలం
#  define CGROUP_మూలం "/sys/fs/cgroup"
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#అయితే_ముగింపు

గణన
{
  CGROUP_బహుళకం_UNIFIED = 1,
  CGROUP_బహుళకం_LEGACY,
  CGROUP_బహుళకం_HYBRID
};

గణన
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

నిర్మాణం libcrun_cgroup_స్థితి;

నిర్మాణం libcrun_cgroup_వాదనలు
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  స్థిరరాశి అక్షరం *cgroup_మార్గం;
  పూర్ణాంకం manager;
  pid_t pid;
  uid_t మూలం_uid;
  gid_t మూలం_gid;
  స్థిరరాశి అక్షరం *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  అక్షరం *precreated_మార్గం;

  స్థిరరాశి అక్షరం *state_మూలం;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
పూర్ణాంకం libcrun_cgroup_preenter (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, పూర్ణాంకం *dirfd, libcrun_error_t *err);
శూన్య libcrun_cgroup_preenter_cleanup (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు);
పూర్ణాంకం libcrun_cgroup_enter (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, నిర్మాణం libcrun_cgroup_స్థితి **బహిర్గతం, libcrun_error_t *err);
పూర్ణాంకం libcrun_cgroup_enter_finalize (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి, libcrun_error_t *err);
పూర్ణాంకం libcrun_cgroup_destroy (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి, libcrun_error_t *err);

/* Handle the cgroup status.  */
పూర్ణాంకం libcrun_cgroup_get_స్థితి (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి, libcrun_container_స్థితి_t *స్థితి_2,
                               libcrun_error_t *err);
శూన్య libcrun_cgroup_స్థితి_free (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి);

నిర్మాణం libcrun_cgroup_స్థితి *libcrun_cgroup_make_స్థితి (libcrun_container_స్థితి_t *స్థితి_2);

స్థిర అంతఃవరుస శూన్య
cgroup_స్థితి_freep (నిర్మాణం libcrun_cgroup_స్థితి **p)
{
  నిర్మాణం libcrun_cgroup_స్థితి *s = *p;
  ఒకవేళ (s)
    libcrun_cgroup_స్థితి_free (s);
}
#నిర్వచించు cleanup_cgroup_స్థితి __attribute__ ((cleanup (cgroup_స్థితి_freep)))
#నిర్వచించు cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

పూర్ణాంకం libcrun_cgroup_killall (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి, పూర్ణాంకం సంకేతం, libcrun_error_t *err);

పూర్ణాంకం libcrun_cgroup_has_oom (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, libcrun_error_t *err);

పూర్ణాంకం libcrun_cgroup_read_pids (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, bool recurse, pid_t **pids, libcrun_error_t *err);

పూర్ణాంకం libcrun_update_cgroup_resources (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2,
                                     స్థిరరాశి అక్షరం *state_మూలం,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

పూర్ణాంకం libcrun_cgroup_is_container_paused (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
పూర్ణాంకం libcrun_cgroup_join_process (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, స్థిరరాశి అక్షరం *మార్గం, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

పూర్ణాంకం libcrun_cgroup_pause_unpause (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, స్థిరరాశి bool pause, libcrun_error_t *err);

#అయితే_ముగింపు
