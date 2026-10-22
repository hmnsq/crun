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
#ifndef CGROUP_H
#define CGROUP_H

#અન્તર્ભાવ "container.h"
#અન્તર્ભાવ "string_map.h"
#અન્તર્ભાવ <unistd.h>

#ifndef CGROUP_મૂળ
#  define CGROUP_મૂળ "/sys/fs/cgroup"
#endif

#ifndef SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#endif

#ifndef PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#endif

ગણના
{
  CGROUP_MODE_UNIFIED = 1,
  CGROUP_MODE_LEGACY,
  CGROUP_MODE_HYBRID
};

ગણના
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

સંરચના libcrun_cgroup_status;

સંરચના libcrun_cgroup_args
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  અચલ અક્ષર *cgroup_path;
  પૂર્ણાંક manager;
  pid_t pid;
  uid_t મૂળ_uid;
  gid_t મૂળ_gid;
  અચલ અક્ષર *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  અક્ષર *precreated_path;

  અચલ અક્ષર *state_મૂળ;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
પૂર્ણાંક libcrun_cgroup_preenter (સંરચના libcrun_cgroup_args *args, પૂર્ણાંક *dirfd, libcrun_error_t *err);
શૂન્ય libcrun_cgroup_preenter_cleanup (સંરચના libcrun_cgroup_args *args);
પૂર્ણાંક libcrun_cgroup_enter (સંરચના libcrun_cgroup_args *args, સંરચના libcrun_cgroup_status **out, libcrun_error_t *err);
પૂર્ણાંક libcrun_cgroup_enter_finalize (સંરચના libcrun_cgroup_args *args, સંરચના libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
પૂર્ણાંક libcrun_cgroup_destroy (સંરચના libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);

/* Handle the cgroup status.  */
પૂર્ણાંક libcrun_cgroup_get_status (સંરચના libcrun_cgroup_status *cgroup_status, libcrun_container_status_t *status,
                               libcrun_error_t *err);
શૂન્ય libcrun_cgroup_status_free (સંરચના libcrun_cgroup_status *cgroup_status);

સંરચના libcrun_cgroup_status *libcrun_cgroup_make_status (libcrun_container_status_t *status);

સ્થિર અંતઃરેખ શૂન્ય
cgroup_status_freep (સંરચના libcrun_cgroup_status **p)
{
  સંરચના libcrun_cgroup_status *s = *p;
  જો (s)
    libcrun_cgroup_status_free (s);
}
#define cleanup_cgroup_status __attribute__ ((cleanup (cgroup_status_freep)))
#define cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

પૂર્ણાંક libcrun_cgroup_killall (સંરચના libcrun_cgroup_status *cgroup_status, પૂર્ણાંક સંકેત, libcrun_error_t *err);

પૂર્ણાંક libcrun_cgroup_has_oom (સંરચના libcrun_cgroup_status *status, libcrun_error_t *err);

પૂર્ણાંક libcrun_cgroup_read_pids (સંરચના libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err);

પૂર્ણાંક libcrun_update_cgroup_resources (સંરચના libcrun_cgroup_status *status,
                                     અચલ અક્ષર *state_મૂળ,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

પૂર્ણાંક libcrun_cgroup_is_container_paused (સંરચના libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
પૂર્ણાંક libcrun_cgroup_join_process (સંરચના libcrun_cgroup_status *status, અચલ અક્ષર *path, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

પૂર્ણાંક libcrun_cgroup_pause_unpause (સંરચના libcrun_cgroup_status *status, અચલ bool pause, libcrun_error_t *err);

#endif
