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

#অন্তর্ভাব "container.h"
#অন্তর্ভাব "string_map.h"
#অন্তর্ভাব <unistd.h>

#ifndef CGROUP_মূল
#  define CGROUP_মূল "/sys/fs/cgroup"
#endif

#ifndef SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#endif

#ifndef PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#endif

গণনা
{
  CGROUP_MODE_UNIFIED = 1,
  CGROUP_MODE_LEGACY,
  CGROUP_MODE_HYBRID
};

গণনা
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

কাঠামো libcrun_cgroup_status;

কাঠামো libcrun_cgroup_args
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  ধ্রুবক অক্ষর *cgroup_path;
  পূর্ণসংখ্যা manager;
  pid_t pid;
  uid_t মূল_uid;
  gid_t মূল_gid;
  ধ্রুবক অক্ষর *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  অক্ষর *precreated_path;

  ধ্রুবক অক্ষর *state_মূল;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
পূর্ণসংখ্যা libcrun_cgroup_preenter (কাঠামো libcrun_cgroup_args *args, পূর্ণসংখ্যা *dirfd, libcrun_error_t *err);
শূন্য libcrun_cgroup_preenter_cleanup (কাঠামো libcrun_cgroup_args *args);
পূর্ণসংখ্যা libcrun_cgroup_enter (কাঠামো libcrun_cgroup_args *args, কাঠামো libcrun_cgroup_status **out, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_cgroup_enter_finalize (কাঠামো libcrun_cgroup_args *args, কাঠামো libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_cgroup_destroy (কাঠামো libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);

/* Handle the cgroup status.  */
পূর্ণসংখ্যা libcrun_cgroup_get_status (কাঠামো libcrun_cgroup_status *cgroup_status, libcrun_container_status_t *status,
                               libcrun_error_t *err);
শূন্য libcrun_cgroup_status_free (কাঠামো libcrun_cgroup_status *cgroup_status);

কাঠামো libcrun_cgroup_status *libcrun_cgroup_make_status (libcrun_container_status_t *status);

স্থির অন্তঃরেখ শূন্য
cgroup_status_freep (কাঠামো libcrun_cgroup_status **p)
{
  কাঠামো libcrun_cgroup_status *s = *p;
  যদি (s)
    libcrun_cgroup_status_free (s);
}
#define cleanup_cgroup_status __attribute__ ((cleanup (cgroup_status_freep)))
#define cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

পূর্ণসংখ্যা libcrun_cgroup_killall (কাঠামো libcrun_cgroup_status *cgroup_status, পূর্ণসংখ্যা সংকেত, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_cgroup_has_oom (কাঠামো libcrun_cgroup_status *status, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_cgroup_read_pids (কাঠামো libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_update_cgroup_resources (কাঠামো libcrun_cgroup_status *status,
                                     ধ্রুবক অক্ষর *state_মূল,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_cgroup_is_container_paused (কাঠামো libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
পূর্ণসংখ্যা libcrun_cgroup_join_process (কাঠামো libcrun_cgroup_status *status, ধ্রুবক অক্ষর *path, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_cgroup_pause_unpause (কাঠামো libcrun_cgroup_status *status, ধ্রুবক bool pause, libcrun_error_t *err);

#endif
