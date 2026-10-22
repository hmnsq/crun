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
#اگر_تعریف_نشده CGROUP_H
#تعریف_کن CGROUP_H

#ضمن "container.h"
#ضمن "string_map.h"
#ضمن <unistd.h>

#اگر_تعریف_نشده CGROUP_ریشه
#  define CGROUP_ریشه "/sys/fs/cgroup"
#پایان_اگر

#اگر_تعریف_نشده SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#پایان_اگر

#اگر_تعریف_نشده PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#پایان_اگر

تعداد
{
  CGROUP_نما_UNIFIED = 1,
  CGROUP_نما_LEGACY,
  CGROUP_نما_HYBRID
};

تعداد
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

بنية libcrun_cgroup_وضعیت;

بنية libcrun_cgroup_آرگومانها
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  ثابت حرف *cgroup_مسیر;
  صحيح manager;
  pid_t pid;
  uid_t ریشه_uid;
  gid_t ریشه_gid;
  ثابت حرف *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  حرف *precreated_مسیر;

  ثابت حرف *state_ریشه;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
صحيح libcrun_cgroup_preenter (بنية libcrun_cgroup_آرگومانها *آرگومانها, صحيح *dirfd, libcrun_error_t *err);
فراغ libcrun_cgroup_preenter_cleanup (بنية libcrun_cgroup_آرگومانها *آرگومانها);
صحيح libcrun_cgroup_enter (بنية libcrun_cgroup_آرگومانها *آرگومانها, بنية libcrun_cgroup_وضعیت **بیرون, libcrun_error_t *err);
صحيح libcrun_cgroup_enter_finalize (بنية libcrun_cgroup_آرگومانها *آرگومانها, بنية libcrun_cgroup_وضعیت *cgroup_وضعیت, libcrun_error_t *err);
صحيح libcrun_cgroup_destroy (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت, libcrun_error_t *err);

/* Handle the cgroup status.  */
صحيح libcrun_cgroup_get_وضعیت (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت, libcrun_container_وضعیت_t *وضعیت,
                               libcrun_error_t *err);
فراغ libcrun_cgroup_وضعیت_free (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت);

بنية libcrun_cgroup_وضعیت *libcrun_cgroup_make_وضعیت (libcrun_container_وضعیت_t *وضعیت);

ساكن مضمن فراغ
cgroup_وضعیت_freep (بنية libcrun_cgroup_وضعیت **p)
{
  بنية libcrun_cgroup_وضعیت *s = *p;
  إذا (s)
    libcrun_cgroup_وضعیت_free (s);
}
#تعریف_کن cleanup_cgroup_وضعیت __attribute__ ((cleanup (cgroup_وضعیت_freep)))
#تعریف_کن cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

صحيح libcrun_cgroup_killall (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت, صحيح إشارة, libcrun_error_t *err);

صحيح libcrun_cgroup_has_oom (بنية libcrun_cgroup_وضعیت *وضعیت, libcrun_error_t *err);

صحيح libcrun_cgroup_read_pids (بنية libcrun_cgroup_وضعیت *وضعیت, bool recurse, pid_t **pids, libcrun_error_t *err);

صحيح libcrun_update_cgroup_resources (بنية libcrun_cgroup_وضعیت *وضعیت,
                                     ثابت حرف *state_ریشه,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

صحيح libcrun_cgroup_is_container_paused (بنية libcrun_cgroup_وضعیت *وضعیت, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
صحيح libcrun_cgroup_join_process (بنية libcrun_cgroup_وضعیت *وضعیت, ثابت حرف *مسیر, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

صحيح libcrun_cgroup_pause_unpause (بنية libcrun_cgroup_وضعیت *وضعیت, ثابت bool pause, libcrun_error_t *err);

#پایان_اگر
