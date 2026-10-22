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

#ଅନ୍ତର୍ଭାଵ "container.h"
#ଅନ୍ତର୍ଭାଵ "string_map.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>

#ifndef CGROUP_ମୂଳ
#  define CGROUP_ମୂଳ "/sys/fs/cgroup"
#endif

#ifndef SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#endif

#ifndef PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#endif

ଗଣନା
{
  CGROUP_MODE_UNIFIED = 1,
  CGROUP_MODE_LEGACY,
  CGROUP_MODE_HYBRID
};

ଗଣନା
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

ସଂରଚନା libcrun_cgroup_status;

ସଂରଚନା libcrun_cgroup_args
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  ଅଚଳ ଅକ୍ଷର *cgroup_path;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା manager;
  pid_t pid;
  uid_t ମୂଳ_uid;
  gid_t ମୂଳ_gid;
  ଅଚଳ ଅକ୍ଷର *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  ଅକ୍ଷର *precreated_path;

  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_preenter (ସଂରଚନା libcrun_cgroup_args *args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *dirfd, libcrun_error_t *err);
ଶୂନ୍ୟ libcrun_cgroup_preenter_cleanup (ସଂରଚନା libcrun_cgroup_args *args);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_enter (ସଂରଚନା libcrun_cgroup_args *args, ସଂରଚନା libcrun_cgroup_status **out, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_enter_finalize (ସଂରଚନା libcrun_cgroup_args *args, ସଂରଚନା libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_destroy (ସଂରଚନା libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);

/* Handle the cgroup status.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_get_status (ସଂରଚନା libcrun_cgroup_status *cgroup_status, libcrun_container_status_t *status,
                               libcrun_error_t *err);
ଶୂନ୍ୟ libcrun_cgroup_status_free (ସଂରଚନା libcrun_cgroup_status *cgroup_status);

ସଂରଚନା libcrun_cgroup_status *libcrun_cgroup_make_status (libcrun_container_status_t *status);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cgroup_status_freep (ସଂରଚନା libcrun_cgroup_status **p)
{
  ସଂରଚନା libcrun_cgroup_status *s = *p;
  ଯଦି (s)
    libcrun_cgroup_status_free (s);
}
#define cleanup_cgroup_status __attribute__ ((cleanup (cgroup_status_freep)))
#define cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_killall (ସଂରଚନା libcrun_cgroup_status *cgroup_status, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଙ୍କେତ, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_has_oom (ସଂରଚନା libcrun_cgroup_status *status, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_read_pids (ସଂରଚନା libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_update_cgroup_resources (ସଂରଚନା libcrun_cgroup_status *status,
                                     ଅଚଳ ଅକ୍ଷର *state_ମୂଳ,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_is_container_paused (ସଂରଚନା libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_join_process (ସଂରଚନା libcrun_cgroup_status *status, ଅଚଳ ଅକ୍ଷର *path, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_cgroup_pause_unpause (ସଂରଚନା libcrun_cgroup_status *status, ଅଚଳ bool pause, libcrun_error_t *err);

#endif
