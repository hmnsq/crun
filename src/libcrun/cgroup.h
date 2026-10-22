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

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "string_map.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>

#ifndef CGROUP_ಬೇರು
#  define CGROUP_ಬೇರು "/sys/fs/cgroup"
#endif

#ifndef SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#endif

#ifndef PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#endif

ಗಣನಾ
{
  CGROUP_MODE_UNIFIED = 1,
  CGROUP_MODE_LEGACY,
  CGROUP_MODE_HYBRID
};

ಗಣನಾ
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

ಸಂರಚನಾ libcrun_cgroup_status;

ಸಂರಚನಾ libcrun_cgroup_args
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  ಧ್ರುವ ವರ್ಣ *cgroup_path;
  ಪೂರ್ಣಾಙ್ಕ manager;
  pid_t pid;
  uid_t ಬೇರು_uid;
  gid_t ಬೇರು_gid;
  ಧ್ರುವ ವರ್ಣ *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  ವರ್ಣ *precreated_path;

  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_preenter (ಸಂರಚನಾ libcrun_cgroup_args *args, ಪೂರ್ಣಾಙ್ಕ *dirfd, libcrun_error_t *err);
ಶೂನ್ಯ libcrun_cgroup_preenter_cleanup (ಸಂರಚನಾ libcrun_cgroup_args *args);
ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_enter (ಸಂರಚನಾ libcrun_cgroup_args *args, ಸಂರಚನಾ libcrun_cgroup_status **out, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_enter_finalize (ಸಂರಚನಾ libcrun_cgroup_args *args, ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_destroy (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);

/* Handle the cgroup status.  */
ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_get_status (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status, libcrun_container_status_t *status,
                               libcrun_error_t *err);
ಶೂನ್ಯ libcrun_cgroup_status_free (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status);

ಸಂರಚನಾ libcrun_cgroup_status *libcrun_cgroup_make_status (libcrun_container_status_t *status);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cgroup_status_freep (ಸಂರಚನಾ libcrun_cgroup_status **p)
{
  ಸಂರಚನಾ libcrun_cgroup_status *s = *p;
  ಯದಿ (s)
    libcrun_cgroup_status_free (s);
}
#define cleanup_cgroup_status __attribute__ ((cleanup (cgroup_status_freep)))
#define cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_killall (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status, ಪೂರ್ಣಾಙ್ಕ ಸಙ್ಕೇತ, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_has_oom (ಸಂರಚನಾ libcrun_cgroup_status *status, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_read_pids (ಸಂರಚನಾ libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_update_cgroup_resources (ಸಂರಚನಾ libcrun_cgroup_status *status,
                                     ಧ್ರುವ ವರ್ಣ *state_ಬೇರು,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_is_container_paused (ಸಂರಚನಾ libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_join_process (ಸಂರಚನಾ libcrun_cgroup_status *status, ಧ್ರುವ ವರ್ಣ *path, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_cgroup_pause_unpause (ಸಂರಚನಾ libcrun_cgroup_status *status, ಧ್ರುವ bool pause, libcrun_error_t *err);

#endif
