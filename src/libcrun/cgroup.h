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
#اگر_غیر_تعریف_شدہ CGROUP_H
#تعریف_کرو CGROUP_H

#شامل "container.h"
#شامل "string_map.h"
#شامل <unistd.h>

#اگر_غیر_تعریف_شدہ CGROUP_جڑ
#  define CGROUP_جڑ "/sys/fs/cgroup"
#اگر_ختم

#اگر_غیر_تعریف_شدہ SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#اگر_ختم

#اگر_غیر_تعریف_شدہ PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#اگر_ختم

شمار
{
  CGROUP_منوال_UNIFIED = 1,
  CGROUP_منوال_LEGACY,
  CGROUP_منوال_HYBRID
};

شمار
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

ساخت libcrun_cgroup_حالت;

ساخت libcrun_cgroup_دلائل
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  ثابت حرف *cgroup_راستہ;
  صحیح manager;
  pid_t pid;
  uid_t جڑ_uid;
  gid_t جڑ_gid;
  ثابت حرف *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  حرف *precreated_راستہ;

  ثابت حرف *state_جڑ;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
صحیح libcrun_cgroup_preenter (ساخت libcrun_cgroup_دلائل *دلائل, صحیح *dirfd, libcrun_error_t *err);
خالی libcrun_cgroup_preenter_cleanup (ساخت libcrun_cgroup_دلائل *دلائل);
صحیح libcrun_cgroup_enter (ساخت libcrun_cgroup_دلائل *دلائل, ساخت libcrun_cgroup_حالت **برآمد, libcrun_error_t *err);
صحیح libcrun_cgroup_enter_finalize (ساخت libcrun_cgroup_دلائل *دلائل, ساخت libcrun_cgroup_حالت *cgroup_حالت, libcrun_error_t *err);
صحیح libcrun_cgroup_destroy (ساخت libcrun_cgroup_حالت *cgroup_حالت, libcrun_error_t *err);

/* Handle the cgroup status.  */
صحیح libcrun_cgroup_get_حالت (ساخت libcrun_cgroup_حالت *cgroup_حالت, libcrun_container_حالت_t *حالت_2,
                               libcrun_error_t *err);
خالی libcrun_cgroup_حالت_free (ساخت libcrun_cgroup_حالت *cgroup_حالت);

ساخت libcrun_cgroup_حالت *libcrun_cgroup_make_حالت (libcrun_container_حالت_t *حالت_2);

ساکن اندرونی خالی
cgroup_حالت_freep (ساخت libcrun_cgroup_حالت **p)
{
  ساخت libcrun_cgroup_حالت *s = *p;
  اگر (s)
    libcrun_cgroup_حالت_free (s);
}
#تعریف_کرو cleanup_cgroup_حالت __attribute__ ((cleanup (cgroup_حالت_freep)))
#تعریف_کرو cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

صحیح libcrun_cgroup_killall (ساخت libcrun_cgroup_حالت *cgroup_حالت, صحیح اشارہ, libcrun_error_t *err);

صحیح libcrun_cgroup_has_oom (ساخت libcrun_cgroup_حالت *حالت_2, libcrun_error_t *err);

صحیح libcrun_cgroup_read_pids (ساخت libcrun_cgroup_حالت *حالت_2, bool recurse, pid_t **pids, libcrun_error_t *err);

صحیح libcrun_update_cgroup_resources (ساخت libcrun_cgroup_حالت *حالت_2,
                                     ثابت حرف *state_جڑ,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

صحیح libcrun_cgroup_is_container_paused (ساخت libcrun_cgroup_حالت *حالت_2, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
صحیح libcrun_cgroup_join_process (ساخت libcrun_cgroup_حالت *حالت_2, ثابت حرف *راستہ, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

صحیح libcrun_cgroup_pause_unpause (ساخت libcrun_cgroup_حالت *حالت_2, ثابت bool pause, libcrun_error_t *err);

#اگر_ختم
