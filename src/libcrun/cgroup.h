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
#अगर_अघोषित CGROUP_H
#घोषणा CGROUP_H

#समावेश "container.h"
#समावेश "string_map.h"
#समावेश <unistd.h>

#अगर_अघोषित CGROUP_मूल
#  define CGROUP_मूल "/sys/fs/cgroup"
#पूर्ण_अगर

#अगर_अघोषित SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#पूर्ण_अगर

#अगर_अघोषित PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#पूर्ण_अगर

क्रमागत
{
  CGROUP_MODE_UNIFIED = 1,
  CGROUP_MODE_LEGACY,
  CGROUP_MODE_HYBRID
};

क्रमागत
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

काष्ठा libcrun_cgroup_status;

काष्ठा libcrun_cgroup_args
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  स्थिर अक्षर *cgroup_path;
  पूर्णांक manager;
  pid_t pid;
  uid_t मूल_uid;
  gid_t मूल_gid;
  स्थिर अक्षर *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  अक्षर *precreated_path;

  स्थिर अक्षर *state_मूल;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
पूर्णांक libcrun_cgroup_preenter (काष्ठा libcrun_cgroup_args *args, पूर्णांक *dirfd, libcrun_error_t *err);
व्योम libcrun_cgroup_preenter_cleanup (काष्ठा libcrun_cgroup_args *args);
पूर्णांक libcrun_cgroup_enter (काष्ठा libcrun_cgroup_args *args, काष्ठा libcrun_cgroup_status **out, libcrun_error_t *err);
पूर्णांक libcrun_cgroup_enter_finalize (काष्ठा libcrun_cgroup_args *args, काष्ठा libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
पूर्णांक libcrun_cgroup_destroy (काष्ठा libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);

/* Handle the cgroup status.  */
पूर्णांक libcrun_cgroup_get_status (काष्ठा libcrun_cgroup_status *cgroup_status, libcrun_container_status_t *status,
                               libcrun_error_t *err);
व्योम libcrun_cgroup_status_free (काष्ठा libcrun_cgroup_status *cgroup_status);

काष्ठा libcrun_cgroup_status *libcrun_cgroup_make_status (libcrun_container_status_t *status);

अटल अंतरभूत व्योम
cgroup_status_freep (काष्ठा libcrun_cgroup_status **p)
{
  काष्ठा libcrun_cgroup_status *s = *p;
  अगर (s)
    libcrun_cgroup_status_free (s);
}
#घोषणा cleanup_cgroup_status __attribute__ ((cleanup (cgroup_status_freep)))
#घोषणा cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

पूर्णांक libcrun_cgroup_killall (काष्ठा libcrun_cgroup_status *cgroup_status, पूर्णांक संकेत, libcrun_error_t *err);

पूर्णांक libcrun_cgroup_has_oom (काष्ठा libcrun_cgroup_status *status, libcrun_error_t *err);

पूर्णांक libcrun_cgroup_read_pids (काष्ठा libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err);

पूर्णांक libcrun_update_cgroup_resources (काष्ठा libcrun_cgroup_status *status,
                                     स्थिर अक्षर *state_मूल,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

पूर्णांक libcrun_cgroup_is_container_paused (काष्ठा libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
पूर्णांक libcrun_cgroup_join_process (काष्ठा libcrun_cgroup_status *status, स्थिर अक्षर *path, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

पूर्णांक libcrun_cgroup_pause_unpause (काष्ठा libcrun_cgroup_status *status, स्थिर bool छोड़ो, libcrun_error_t *err);

#पूर्ण_अगर
