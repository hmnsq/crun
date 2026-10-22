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

#define _GNU_SOURCE

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
make_cgroup_path (ثابت حرف *path, ثابت حرف *id)
{
  حرف *ret;

  𐡄𐡍 (path == عدم)
    xasprintf (&ret, "/%s", id);
  وإلا 𐡄𐡍 (path[0] == '/')
    ret = xstrdup (path);
  وإلا
    xasprintf (&ret, "/%s", path);

  أرجع ret;
}

ساكن صحيح
libcrun_precreate_cgroup_cgroupfs (بنية libcrun_cgroup_args *args, صحيح *dirfd, libcrun_error_t *err)
{
  cleanup_free حرف *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free حرف *cgroup_path = عدم;
  cleanup_free حرف *parent_path = عدم;
  صحيح ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, sub_path, عدم);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      أرجع 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  𐡄𐡍 (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  وإلا 𐡄𐡍 (errno != EEXIST)
    أرجع 0;

  ret = enable_controllers (sub_path, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  𐡄𐡍 (UNLIKELY (*dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_path);

  أرجع 0;
}

ساكن صحيح
make_new_sibling_cgroup (حرف **out, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *current_cgroup = عدم;
  حرف *dir;
  صحيح ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, عدم);
  أرجع 0;
}

ساكن صحيح
libcrun_cgroup_enter_cgroupfs (بنية libcrun_cgroup_args *args, بنية libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  صحيح cgroup_mode;
  صحيح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  𐡄𐡍 (UNLIKELY (cgroup_mode < 0))
    أرجع cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  𐡄𐡍 (args->joined)
    أرجع 0;

  𐡄𐡍 (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          𐡄𐡍 (args->cgroup_path == عدم)
            {
              libcrun_error_t tmp_err = عدم;
              صحيح tmp_ret;

              حرر (out->path);
              out->path = عدم;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              𐡄𐡍 (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  أرجع ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

ساكن صحيح
libcrun_destroy_cgroup_cgroupfs (بنية libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  صحيح mode;
  صحيح ret;

  mode = libcrun_get_cgroup_mode (err);
  𐡄𐡍 (UNLIKELY (mode < 0))
    أرجع mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    crun_error_release (err);

  أرجع destroy_cgroup_path (cgroup_status->path, mode, err);
}

بنية libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
