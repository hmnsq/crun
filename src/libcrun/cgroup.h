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

#अन्तर्भाव "container.h"
#अन्तर्भाव "string_map.h"
#अन्तर्भाव <unistd.h>

#ifndef CGROUP_मूळ
#  define CGROUP_मूळ "/sys/fs/cgroup"
#endif

#ifndef SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#endif

#ifndef PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#endif

गणना
{
  CGROUP_MODE_UNIFIED = 1,
  CGROUP_MODE_LEGACY,
  CGROUP_MODE_HYBRID
};

गणना
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

संरचना libcrun_cgroup_status;

संरचना libcrun_cgroup_args
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  ध्रुव वर्ण *cgroup_path;
  पूर्णाङ्क manager;
  pid_t pid;
  uid_t मूळ_uid;
  gid_t मूळ_gid;
  ध्रुव वर्ण *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  वर्ण *precreated_path;

  ध्रुव वर्ण *state_मूळ;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
पूर्णाङ्क libcrun_cgroup_preenter (संरचना libcrun_cgroup_args *args, पूर्णाङ्क *dirfd, libcrun_error_t *err);
शून्य libcrun_cgroup_preenter_cleanup (संरचना libcrun_cgroup_args *args);
पूर्णाङ्क libcrun_cgroup_enter (संरचना libcrun_cgroup_args *args, संरचना libcrun_cgroup_status **out, libcrun_error_t *err);
पूर्णाङ्क libcrun_cgroup_enter_finalize (संरचना libcrun_cgroup_args *args, संरचना libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
पूर्णाङ्क libcrun_cgroup_destroy (संरचना libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);

/* Handle the cgroup status.  */
पूर्णाङ्क libcrun_cgroup_get_status (संरचना libcrun_cgroup_status *cgroup_status, libcrun_container_status_t *status,
                               libcrun_error_t *err);
शून्य libcrun_cgroup_status_free (संरचना libcrun_cgroup_status *cgroup_status);

संरचना libcrun_cgroup_status *libcrun_cgroup_make_status (libcrun_container_status_t *status);

स्थिर अन्तःस्थ शून्य
cgroup_status_freep (संरचना libcrun_cgroup_status **p)
{
  संरचना libcrun_cgroup_status *s = *p;
  यदि (s)
    libcrun_cgroup_status_free (s);
}
#define cleanup_cgroup_status __attribute__ ((cleanup (cgroup_status_freep)))
#define cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

पूर्णाङ्क libcrun_cgroup_killall (संरचना libcrun_cgroup_status *cgroup_status, पूर्णाङ्क सङ्केत, libcrun_error_t *err);

पूर्णाङ्क libcrun_cgroup_has_oom (संरचना libcrun_cgroup_status *status, libcrun_error_t *err);

पूर्णाङ्क libcrun_cgroup_read_pids (संरचना libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err);

पूर्णाङ्क libcrun_update_cgroup_resources (संरचना libcrun_cgroup_status *status,
                                     ध्रुव वर्ण *state_मूळ,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

पूर्णाङ्क libcrun_cgroup_is_container_paused (संरचना libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
पूर्णाङ्क libcrun_cgroup_join_process (संरचना libcrun_cgroup_status *status, ध्रुव वर्ण *path, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

पूर्णाङ्क libcrun_cgroup_pause_unpause (संरचना libcrun_cgroup_status *status, ध्रुव bool pause, libcrun_error_t *err);

#endif
