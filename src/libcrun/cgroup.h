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

#അന്തര്ഭാവ "container.h"
#അന്തര്ഭാവ "string_map.h"
#അന്തര്ഭാവ <unistd.h>

#ifndef CGROUP_വേര്
#  define CGROUP_വേര് "/sys/fs/cgroup"
#endif

#ifndef SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#endif

#ifndef PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#endif

എണ്ണൽ
{
  CGROUP_MODE_UNIFIED = 1,
  CGROUP_MODE_LEGACY,
  CGROUP_MODE_HYBRID
};

എണ്ണൽ
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

ഘടന libcrun_cgroup_status;

ഘടന libcrun_cgroup_args
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  സ്ഥിരം അക്ഷരം *cgroup_path;
  പൂർണ്ണസംഖ്യ manager;
  pid_t pid;
  uid_t വേര്_uid;
  gid_t വേര്_gid;
  സ്ഥിരം അക്ഷരം *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  അക്ഷരം *precreated_path;

  സ്ഥിരം അക്ഷരം *state_വേര്;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
പൂർണ്ണസംഖ്യ libcrun_cgroup_preenter (ഘടന libcrun_cgroup_args *args, പൂർണ്ണസംഖ്യ *dirfd, libcrun_error_t *err);
ശൂന്യം libcrun_cgroup_preenter_cleanup (ഘടന libcrun_cgroup_args *args);
പൂർണ്ണസംഖ്യ libcrun_cgroup_enter (ഘടന libcrun_cgroup_args *args, ഘടന libcrun_cgroup_status **out, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_cgroup_enter_finalize (ഘടന libcrun_cgroup_args *args, ഘടന libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_cgroup_destroy (ഘടന libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);

/* Handle the cgroup status.  */
പൂർണ്ണസംഖ്യ libcrun_cgroup_get_status (ഘടന libcrun_cgroup_status *cgroup_status, libcrun_container_status_t *status,
                               libcrun_error_t *err);
ശൂന്യം libcrun_cgroup_status_free (ഘടന libcrun_cgroup_status *cgroup_status);

ഘടന libcrun_cgroup_status *libcrun_cgroup_make_status (libcrun_container_status_t *status);

സ്ഥിതം വരിയിൽ ശൂന്യം
cgroup_status_freep (ഘടന libcrun_cgroup_status **p)
{
  ഘടന libcrun_cgroup_status *s = *p;
  എങ്കിൽ (s)
    libcrun_cgroup_status_free (s);
}
#define cleanup_cgroup_status __attribute__ ((cleanup (cgroup_status_freep)))
#define cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

പൂർണ്ണസംഖ്യ libcrun_cgroup_killall (ഘടന libcrun_cgroup_status *cgroup_status, പൂർണ്ണസംഖ്യ സൂചന, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_cgroup_has_oom (ഘടന libcrun_cgroup_status *status, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_cgroup_read_pids (ഘടന libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_update_cgroup_resources (ഘടന libcrun_cgroup_status *status,
                                     സ്ഥിരം അക്ഷരം *state_വേര്,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_cgroup_is_container_paused (ഘടന libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
പൂർണ്ണസംഖ്യ libcrun_cgroup_join_process (ഘടന libcrun_cgroup_status *status, സ്ഥിരം അക്ഷരം *path, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_cgroup_pause_unpause (ഘടന libcrun_cgroup_status *status, സ്ഥിരം bool pause, libcrun_error_t *err);

#endif
