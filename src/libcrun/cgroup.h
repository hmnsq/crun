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
#جيڪڏهن_وصف_نه_ٿيل CGROUP_H
#وصف_ڪريو CGROUP_H

#ضمن "container.h"
#ضمن "string_map.h"
#ضمن <unistd.h>

#جيڪڏهن_وصف_نه_ٿيل CGROUP_پاڙ
#  define CGROUP_پاڙ "/sys/fs/cgroup"
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#جيڪڏهن_پورو

تعداد
{
  CGROUP_گهڻائي_UNIFIED = 1,
  CGROUP_گهڻائي_LEGACY,
  CGROUP_گهڻائي_HYBRID
};

تعداد
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

بنية libcrun_cgroup_حالت;

بنية libcrun_cgroup_دليلون
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  ثابت حرف *cgroup_رستو;
  صحيح manager;
  pid_t pid;
  uid_t پاڙ_uid;
  gid_t پاڙ_gid;
  ثابت حرف *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  حرف *precreated_رستو;

  ثابت حرف *state_پاڙ;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
صحيح libcrun_cgroup_preenter (بنية libcrun_cgroup_دليلون *دليلون, صحيح *dirfd, libcrun_error_t *err);
فراغ libcrun_cgroup_preenter_cleanup (بنية libcrun_cgroup_دليلون *دليلون);
صحيح libcrun_cgroup_enter (بنية libcrun_cgroup_دليلون *دليلون, بنية libcrun_cgroup_حالت **ٻاهريون, libcrun_error_t *err);
صحيح libcrun_cgroup_enter_finalize (بنية libcrun_cgroup_دليلون *دليلون, بنية libcrun_cgroup_حالت *cgroup_حالت, libcrun_error_t *err);
صحيح libcrun_cgroup_destroy (بنية libcrun_cgroup_حالت *cgroup_حالت, libcrun_error_t *err);

/* Handle the cgroup status.  */
صحيح libcrun_cgroup_get_حالت (بنية libcrun_cgroup_حالت *cgroup_حالت, libcrun_container_حالت_t *حالت,
                               libcrun_error_t *err);
فراغ libcrun_cgroup_حالت_free (بنية libcrun_cgroup_حالت *cgroup_حالت);

بنية libcrun_cgroup_حالت *libcrun_cgroup_make_حالت (libcrun_container_حالت_t *حالت);

ساكن مضمن فراغ
cgroup_حالت_freep (بنية libcrun_cgroup_حالت **p)
{
  بنية libcrun_cgroup_حالت *s = *p;
  إذا (s)
    libcrun_cgroup_حالت_free (s);
}
#وصف_ڪريو cleanup_cgroup_حالت __attribute__ ((cleanup (cgroup_حالت_freep)))
#وصف_ڪريو cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

صحيح libcrun_cgroup_killall (بنية libcrun_cgroup_حالت *cgroup_حالت, صحيح إشارة, libcrun_error_t *err);

صحيح libcrun_cgroup_has_oom (بنية libcrun_cgroup_حالت *حالت, libcrun_error_t *err);

صحيح libcrun_cgroup_read_pids (بنية libcrun_cgroup_حالت *حالت, bool recurse, pid_t **pids, libcrun_error_t *err);

صحيح libcrun_update_cgroup_resources (بنية libcrun_cgroup_حالت *حالت,
                                     ثابت حرف *state_پاڙ,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

صحيح libcrun_cgroup_is_container_paused (بنية libcrun_cgroup_حالت *حالت, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
صحيح libcrun_cgroup_join_process (بنية libcrun_cgroup_حالت *حالت, ثابت حرف *رستو, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

صحيح libcrun_cgroup_pause_unpause (بنية libcrun_cgroup_حالت *حالت, ثابت bool pause, libcrun_error_t *err);

#جيڪڏهن_پورو
