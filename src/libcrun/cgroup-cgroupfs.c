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

#تعریف_کن GNU_منبع

#ضمن <config.h>
#ضمن "cgroup.h"
#ضمن "cgroup-internal.h"
#ضمن "cgroup-systemd.h"
#ضمن "cgroup-setup.h"
#ضمن "cgroup-utils.h"
#ضمن "cgroup-cgroupfs.h"
#ضمن <string.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <sys/vfs.h>
#ضمن <inttypes.h>
#ضمن <time.h>

#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <libgen.h>

ساكن حرف *
make_cgroup_مسیر (ثابت حرف *مسیر, ثابت حرف *id)
{
  حرف *ret;

  إذا (مسیر == عدم)
    xasprintf (&ret, "/%s", id);
  وإلا إذا (مسیر[0] == '/')
    ret = xstrdup (مسیر);
  وإلا
    xasprintf (&ret, "/%s", مسیر);

  أرجع ret;
}

ساكن صحيح
libcrun_precreate_cgroup_cgroupfs (بنية libcrun_cgroup_آرگومانها *آرگومانها, صحيح *dirfd, libcrun_error_t *err)
{
  cleanup_free حرف *sub_مسیر = make_cgroup_مسیر (آرگومانها->cgroup_مسیر, آرگومانها->id);
  cleanup_free حرف *cgroup_مسیر = عدم;
  cleanup_free حرف *والد_مسیر = عدم;
  صحيح ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_مسیر, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&cgroup_مسیر, err, CGROUP_ریشه, sub_مسیر, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  والد_مسیر = xstrdup (cgroup_مسیر);
  ret = crun_ensure_directory (dirname (والد_مسیر), 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      أرجع 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  إذا (mkdir (cgroup_مسیر, 0755) == 0)
    آرگومانها->precreated_مسیر = xstrdup (cgroup_مسیر);
  وإلا إذا (errno != EEXIST)
    أرجع 0;

  ret = enable_controllers (sub_مسیر, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  *dirfd = open (cgroup_مسیر, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_مسیر);
  إذا (UNLIKELY (*dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_مسیر);

  أرجع 0;
}

ساكن صحيح
make_new_sibling_cgroup (حرف **بیرون, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *current_cgroup = عدم;
  حرف *dir;
  صحيح ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dir = dirname (current_cgroup);

  append_paths (بیرون, err, dir, id, عدم);
  أرجع 0;
}

ساكن صحيح
libcrun_cgroup_enter_cgroupfs (بنية libcrun_cgroup_آرگومانها *آرگومانها, بنية libcrun_cgroup_وضعیت *بیرون, libcrun_error_t *err)
{
  pid_t pid = آرگومانها->pid;
  صحيح cgroup_نما;
  صحيح ret;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  بیرون->مسیر = make_cgroup_مسیر (آرگومانها->cgroup_مسیر, آرگومانها->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  إذا (آرگومانها->joined)
    أرجع 0;

  إذا (cgroup_نما == CGROUP_نما_UNIFIED)
    {
      ret = enable_controllers (بیرون->مسیر, err);
      إذا (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          إذا (آرگومانها->cgroup_مسیر == عدم)
            {
              libcrun_error_t tmp_err = عدم;
              صحيح tmp_ret;

              حرر (بیرون->مسیر);
              بیرون->مسیر = عدم;

              tmp_ret = make_new_sibling_cgroup (&بیرون->مسیر, آرگومانها->id, &tmp_err);
              إذا (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  أرجع ret;
                }

              crun_error_release (err);
              ret = enable_controllers (بیرون->مسیر, err);
            }
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (بیرون->مسیر, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع enter_cgroup (cgroup_نما, pid, 0, بیرون->مسیر, true, err);
}

ساكن صحيح
libcrun_destroy_cgroup_cgroupfs (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت,
                                 libcrun_error_t *err)
{
  صحيح نما;
  صحيح ret;

  نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (نما < 0))
    أرجع نما;

  ret = cgroup_killall_مسیر (cgroup_وضعیت->مسیر, SIGKILL, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  أرجع destroy_cgroup_مسیر (cgroup_وضعیت->مسیر, نما, err);
}

بنية libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
