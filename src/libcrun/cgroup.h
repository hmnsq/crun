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

#شامل "container.h"
#شامل "string_map.h"
#شامل <unistd.h>

#ifndef CGROUP_ریشه
#  define CGROUP_ریشه "/sys/fs/cgroup"
#endif

#ifndef SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#endif

#ifndef PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#endif

شمارش
{
  CGROUP_MODE_UNIFIED = 1,
  CGROUP_MODE_LEGACY,
  CGROUP_MODE_HYBRID
};

شمارش
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

ساختار libcrun_cgroup_status;

ساختار libcrun_cgroup_args
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  ثابت نویسه *cgroup_path;
  صحیح manager;
  pid_t pid;
  uid_t ریشه_uid;
  gid_t ریشه_gid;
  ثابت نویسه *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  نویسه *precreated_path;

  ثابت نویسه *state_ریشه;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
صحیح libcrun_cgroup_preenter (ساختار libcrun_cgroup_args *args, صحیح *dirfd, libcrun_error_t *err);
تهی libcrun_cgroup_preenter_cleanup (ساختار libcrun_cgroup_args *args);
صحیح libcrun_cgroup_enter (ساختار libcrun_cgroup_args *args, ساختار libcrun_cgroup_status **out, libcrun_error_t *err);
صحیح libcrun_cgroup_enter_finalize (ساختار libcrun_cgroup_args *args, ساختار libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
صحیح libcrun_cgroup_destroy (ساختار libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);

/* Handle the cgroup status.  */
صحیح libcrun_cgroup_get_status (ساختار libcrun_cgroup_status *cgroup_status, libcrun_container_status_t *status,
                               libcrun_error_t *err);
تهی libcrun_cgroup_status_free (ساختار libcrun_cgroup_status *cgroup_status);

ساختار libcrun_cgroup_status *libcrun_cgroup_make_status (libcrun_container_status_t *status);

ایستا درونخطی تهی
cgroup_status_freep (ساختار libcrun_cgroup_status **p)
{
  ساختار libcrun_cgroup_status *s = *p;
  اگر (s)
    libcrun_cgroup_status_free (s);
}
#define cleanup_cgroup_status __attribute__ ((cleanup (cgroup_status_freep)))
#define cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

صحیح libcrun_cgroup_killall (ساختار libcrun_cgroup_status *cgroup_status, صحیح سیگنال, libcrun_error_t *err);

صحیح libcrun_cgroup_has_oom (ساختار libcrun_cgroup_status *status, libcrun_error_t *err);

صحیح libcrun_cgroup_read_pids (ساختار libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err);

صحیح libcrun_update_cgroup_resources (ساختار libcrun_cgroup_status *status,
                                     ثابت نویسه *state_ریشه,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

صحیح libcrun_cgroup_is_container_paused (ساختار libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
صحیح libcrun_cgroup_join_process (ساختار libcrun_cgroup_status *status, ثابت نویسه *path, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

صحیح libcrun_cgroup_pause_unpause (ساختار libcrun_cgroup_status *status, ثابت bool pause, libcrun_error_t *err);

#endif
