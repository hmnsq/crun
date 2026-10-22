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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CGROUP_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CGROUP_H

#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string_map.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CGROUP_ਜੜ੍ਹ
#  define CGROUP_ਜੜ੍ਹ "/sys/fs/cgroup"
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ
{
  CGROUP_ਬਹੁਲਕ_UNIFIED = 1,
  CGROUP_ਬਹੁਲਕ_LEGACY,
  CGROUP_ਬਹੁਲਕ_HYBRID
};

ਗਿਣਤੀ
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ;

ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ;
  ਪੂਰਨ manager;
  pid_t pid;
  uid_t ਜੜ੍ਹ_uid;
  gid_t ਜੜ੍ਹ_gid;
  ਅਚਲ ਅੱਖਰ *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  ਅੱਖਰ *precreated_ਰਸਤਾ;

  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
ਪੂਰਨ libcrun_cgroup_preenter (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ, ਪੂਰਨ *dirfd, libcrun_error_t *err);
ਖਾਲੀ libcrun_cgroup_preenter_cleanup (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ);
ਪੂਰਨ libcrun_cgroup_enter (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ, ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ **ਬਾਹਰਲਾ, libcrun_error_t *err);
ਪੂਰਨ libcrun_cgroup_enter_finalize (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ, ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ, libcrun_error_t *err);
ਪੂਰਨ libcrun_cgroup_destroy (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ, libcrun_error_t *err);

/* Handle the cgroup status.  */
ਪੂਰਨ libcrun_cgroup_get_ਸਥਿਤੀ (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ,
                               libcrun_error_t *err);
ਖਾਲੀ libcrun_cgroup_ਸਥਿਤੀ_free (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ);

ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *libcrun_cgroup_make_ਸਥਿਤੀ (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cgroup_ਸਥਿਤੀ_freep (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ **p)
{
  ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *s = *p;
  ਜੇ (s)
    libcrun_cgroup_ਸਥਿਤੀ_free (s);
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_cgroup_ਸਥਿਤੀ __attribute__ ((cleanup (cgroup_ਸਥਿਤੀ_freep)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

ਪੂਰਨ libcrun_cgroup_killall (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ, ਪੂਰਨ ਸੰਕੇਤ, libcrun_error_t *err);

ਪੂਰਨ libcrun_cgroup_has_oom (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, libcrun_error_t *err);

ਪੂਰਨ libcrun_cgroup_read_pids (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, bool recurse, pid_t **pids, libcrun_error_t *err);

ਪੂਰਨ libcrun_update_cgroup_resources (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ,
                                     ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

ਪੂਰਨ libcrun_cgroup_is_container_paused (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
ਪੂਰਨ libcrun_cgroup_join_process (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

ਪੂਰਨ libcrun_cgroup_pause_unpause (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, ਅਚਲ bool pause, libcrun_error_t *err);

#ਜੇ_ਅੰਤ
