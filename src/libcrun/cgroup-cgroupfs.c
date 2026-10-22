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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "cgroup.h"
#অন্তর্ভাব "cgroup-internal.h"
#অন্তর্ভাব "cgroup-systemd.h"
#অন্তর্ভাব "cgroup-setup.h"
#অন্তর্ভাব "cgroup-utils.h"
#অন্তর্ভাব "cgroup-cgroupfs.h"
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <sys/vfs.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব <time.h>

#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <libgen.h>

স্থির বর্ণ *
make_cgroup_path (ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *id)
{
  বর্ণ *ret;

  যদি (path == রিক্ত)
    xasprintf (&ret, "/%s", id);
  অন্যথা যদি (path[0] == '/')
    ret = xstrdup (path);
  অন্যথা
    xasprintf (&ret, "/%s", path);

  প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক
libcrun_precreate_cgroup_cgroupfs (সংরচনা libcrun_cgroup_args *args, পূর্ণাঙ্ক *dirfd, libcrun_error_t *err)
{
  cleanup_free বর্ণ *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free বর্ণ *cgroup_path = রিক্ত;
  cleanup_free বর্ণ *parent_path = রিক্ত;
  পূর্ণাঙ্ক ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = append_paths (&cgroup_path, err, CGROUP_মূল, sub_path, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      প্রত্যাগম 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  যদি (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  অন্যথা যদি (errno != EEXIST)
    প্রত্যাগম 0;

  ret = enable_controllers (sub_path, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  যদি (UNLIKELY (*dirfd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", cgroup_path);

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
make_new_sibling_cgroup (বর্ণ **out, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  cleanup_free বর্ণ *current_cgroup = রিক্ত;
  বর্ণ *dir;
  পূর্ণাঙ্ক ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, রিক্ত);
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
libcrun_cgroup_enter_cgroupfs (সংরচনা libcrun_cgroup_args *args, সংরচনা libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  যদি (args->joined)
    প্রত্যাগম 0;

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      যদি (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          যদি (args->cgroup_path == রিক্ত)
            {
              libcrun_error_t tmp_err = রিক্ত;
              পূর্ণাঙ্ক tmp_ret;

              মুক্ত (out->path);
              out->path = রিক্ত;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              যদি (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  প্রত্যাগম ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

স্থির পূর্ণাঙ্ক
libcrun_destroy_cgroup_cgroupfs (সংরচনা libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  পূর্ণাঙ্ক mode;
  পূর্ণাঙ্ক ret;

  mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (mode < 0))
    প্রত্যাগম mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  যদি (UNLIKELY (ret < 0))
    crun_error_release (err);

  প্রত্যাগম destroy_cgroup_path (cgroup_status->path, mode, err);
}

সংরচনা libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
