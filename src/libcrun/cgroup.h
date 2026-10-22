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

#அந்தர்பாவ "container.h"
#அந்தர்பாவ "string_map.h"
#அந்தர்பாவ <unistd.h>

#ifndef CGROUP_வேர்
#  define CGROUP_வேர் "/sys/fs/cgroup"
#endif

#ifndef SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#endif

#ifndef PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#endif

எண்ணிக்கை
{
  CGROUP_MODE_UNIFIED = 1,
  CGROUP_MODE_LEGACY,
  CGROUP_MODE_HYBRID
};

எண்ணிக்கை
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

கட்டமைப்பு libcrun_cgroup_status;

கட்டமைப்பு libcrun_cgroup_args
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  மாறிலி எழுத்து *cgroup_path;
  முழு manager;
  pid_t pid;
  uid_t வேர்_uid;
  gid_t வேர்_gid;
  மாறிலி எழுத்து *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  எழுத்து *precreated_path;

  மாறிலி எழுத்து *state_வேர்;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
முழு libcrun_cgroup_preenter (கட்டமைப்பு libcrun_cgroup_args *args, முழு *dirfd, libcrun_error_t *err);
வெறுமை libcrun_cgroup_preenter_cleanup (கட்டமைப்பு libcrun_cgroup_args *args);
முழு libcrun_cgroup_enter (கட்டமைப்பு libcrun_cgroup_args *args, கட்டமைப்பு libcrun_cgroup_status **out, libcrun_error_t *err);
முழு libcrun_cgroup_enter_finalize (கட்டமைப்பு libcrun_cgroup_args *args, கட்டமைப்பு libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
முழு libcrun_cgroup_destroy (கட்டமைப்பு libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);

/* Handle the cgroup status.  */
முழு libcrun_cgroup_get_status (கட்டமைப்பு libcrun_cgroup_status *cgroup_status, libcrun_container_status_t *status,
                               libcrun_error_t *err);
வெறுமை libcrun_cgroup_status_free (கட்டமைப்பு libcrun_cgroup_status *cgroup_status);

கட்டமைப்பு libcrun_cgroup_status *libcrun_cgroup_make_status (libcrun_container_status_t *status);

நிலையான உள்வரி வெறுமை
cgroup_status_freep (கட்டமைப்பு libcrun_cgroup_status **p)
{
  கட்டமைப்பு libcrun_cgroup_status *s = *p;
  எனில் (s)
    libcrun_cgroup_status_free (s);
}
#define cleanup_cgroup_status __attribute__ ((cleanup (cgroup_status_freep)))
#define cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

முழு libcrun_cgroup_killall (கட்டமைப்பு libcrun_cgroup_status *cgroup_status, முழு சமிக்ஞை, libcrun_error_t *err);

முழு libcrun_cgroup_has_oom (கட்டமைப்பு libcrun_cgroup_status *status, libcrun_error_t *err);

முழு libcrun_cgroup_read_pids (கட்டமைப்பு libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err);

முழு libcrun_update_cgroup_resources (கட்டமைப்பு libcrun_cgroup_status *status,
                                     மாறிலி எழுத்து *state_வேர்,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

முழு libcrun_cgroup_is_container_paused (கட்டமைப்பு libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
முழு libcrun_cgroup_join_process (கட்டமைப்பு libcrun_cgroup_status *status, மாறிலி எழுத்து *path, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

முழு libcrun_cgroup_pause_unpause (கட்டமைப்பு libcrun_cgroup_status *status, மாறிலி bool pause, libcrun_error_t *err);

#endif
