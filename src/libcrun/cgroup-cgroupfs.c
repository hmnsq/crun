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

#شامل <config.h>
#شامل "cgroup.h"
#شامل "cgroup-internal.h"
#شامل "cgroup-systemd.h"
#شامل "cgroup-setup.h"
#شامل "cgroup-utils.h"
#شامل "cgroup-cgroupfs.h"
#شامل <string.h>
#شامل <sys/types.h>
#شامل <signal.h>
#شامل <sys/vfs.h>
#شامل <inttypes.h>
#شامل <time.h>

#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <libgen.h>

ایستا نویسه *
make_cgroup_path (ثابت نویسه *path, ثابت نویسه *id)
{
  نویسه *ret;

  اگر (path == پوچ)
    xasprintf (&ret, "/%s", id);
  وگرنه اگر (path[0] == '/')
    ret = xstrdup (path);
  وگرنه
    xasprintf (&ret, "/%s", path);

  بازگشت ret;
}

ایستا صحیح
libcrun_precreate_cgroup_cgroupfs (ساختار libcrun_cgroup_args *args, صحیح *dirfd, libcrun_error_t *err)
{
  cleanup_free نویسه *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free نویسه *cgroup_path = پوچ;
  cleanup_free نویسه *parent_path = پوچ;
  صحیح ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ریشه, sub_path, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      بازگشت 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  اگر (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  وگرنه اگر (errno != EEXIST)
    بازگشت 0;

  ret = enable_controllers (sub_path, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  اگر (UNLIKELY (*dirfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", cgroup_path);

  بازگشت 0;
}

ایستا صحیح
make_new_sibling_cgroup (نویسه **out, ثابت نویسه *id, libcrun_error_t *err)
{
  cleanup_free نویسه *current_cgroup = پوچ;
  نویسه *dir;
  صحیح ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, پوچ);
  بازگشت 0;
}

ایستا صحیح
libcrun_cgroup_enter_cgroupfs (ساختار libcrun_cgroup_args *args, ساختار libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  صحیح cgroup_mode;
  صحیح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  اگر (args->joined)
    بازگشت 0;

  اگر (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      اگر (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          اگر (args->cgroup_path == پوچ)
            {
              libcrun_error_t tmp_err = پوچ;
              صحیح tmp_ret;

              آزاد (out->path);
              out->path = پوچ;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              اگر (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  بازگشت ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

ایستا صحیح
libcrun_destroy_cgroup_cgroupfs (ساختار libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  صحیح mode;
  صحیح ret;

  mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (mode < 0))
    بازگشت mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);

  بازگشت destroy_cgroup_path (cgroup_status->path, mode, err);
}

ساختار libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
