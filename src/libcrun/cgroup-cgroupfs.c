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

#تعریف_کرو GNU_سروت

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
make_cgroup_رستہ (ثابت حرف *رستہ, ثابت حرف *id)
{
  حرف *ret;

  إذا (رستہ == عدم)
    xasprintf (&ret, "/%s", id);
  وإلا إذا (رستہ[0] == '/')
    ret = xstrdup (رستہ);
  وإلا
    xasprintf (&ret, "/%s", رستہ);

  أرجع ret;
}

ساكن صحيح
libcrun_precreate_cgroup_cgroupfs (بنية libcrun_cgroup_دلیلاں *دلیلاں, صحيح *dirfd, libcrun_error_t *err)
{
  cleanup_free حرف *sub_رستہ = make_cgroup_رستہ (دلیلاں->cgroup_رستہ, دلیلاں->id);
  cleanup_free حرف *cgroup_رستہ = عدم;
  cleanup_free حرف *ماپا_رستہ = عدم;
  صحيح ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_رستہ, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&cgroup_رستہ, err, CGROUP_جڑ, sub_رستہ, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ماپا_رستہ = xstrdup (cgroup_رستہ);
  ret = crun_ensure_directory (dirname (ماپا_رستہ), 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      أرجع 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  إذا (mkdir (cgroup_رستہ, 0755) == 0)
    دلیلاں->precreated_رستہ = xstrdup (cgroup_رستہ);
  وإلا إذا (errno != EEXIST)
    أرجع 0;

  ret = enable_controllers (sub_رستہ, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  *dirfd = open (cgroup_رستہ, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_رستہ);
  إذا (UNLIKELY (*dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_رستہ);

  أرجع 0;
}

ساكن صحيح
make_new_sibling_cgroup (حرف **باہرلا, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *current_cgroup = عدم;
  حرف *dir;
  صحيح ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dir = dirname (current_cgroup);

  append_paths (باہرلا, err, dir, id, عدم);
  أرجع 0;
}

ساكن صحيح
libcrun_cgroup_enter_cgroupfs (بنية libcrun_cgroup_دلیلاں *دلیلاں, بنية libcrun_cgroup_حالت *باہرلا, libcrun_error_t *err)
{
  pid_t pid = دلیلاں->pid;
  صحيح cgroup_منوال;
  صحيح ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  إذا (UNLIKELY (cgroup_منوال < 0))
    أرجع cgroup_منوال;

  باہرلا->رستہ = make_cgroup_رستہ (دلیلاں->cgroup_رستہ, دلیلاں->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  إذا (دلیلاں->joined)
    أرجع 0;

  إذا (cgroup_منوال == CGROUP_منوال_UNIFIED)
    {
      ret = enable_controllers (باہرلا->رستہ, err);
      إذا (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          إذا (دلیلاں->cgroup_رستہ == عدم)
            {
              libcrun_error_t tmp_err = عدم;
              صحيح tmp_ret;

              حرر (باہرلا->رستہ);
              باہرلا->رستہ = عدم;

              tmp_ret = make_new_sibling_cgroup (&باہرلا->رستہ, دلیلاں->id, &tmp_err);
              إذا (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  أرجع ret;
                }

              crun_error_release (err);
              ret = enable_controllers (باہرلا->رستہ, err);
            }
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (باہرلا->رستہ, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع enter_cgroup (cgroup_منوال, pid, 0, باہرلا->رستہ, true, err);
}

ساكن صحيح
libcrun_destroy_cgroup_cgroupfs (بنية libcrun_cgroup_حالت *cgroup_حالت,
                                 libcrun_error_t *err)
{
  صحيح منوال;
  صحيح ret;

  منوال = libcrun_get_cgroup_منوال (err);
  إذا (UNLIKELY (منوال < 0))
    أرجع منوال;

  ret = cgroup_killall_رستہ (cgroup_حالت->رستہ, SIGKILL, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  أرجع destroy_cgroup_رستہ (cgroup_حالت->رستہ, منوال, err);
}

بنية libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
