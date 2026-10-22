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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-internal.h"
#ಅನ್ತರ್ಭಾವ "cgroup-systemd.h"
#ಅನ್ತರ್ಭಾವ "cgroup-setup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"
#ಅನ್ತರ್ಭಾವ "cgroup-cgroupfs.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>

ಸ್ಥಿರ ವರ್ಣ *
make_cgroup_path (ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *id)
{
  ವರ್ಣ *ret;

  ಯದಿ (path == ರಿಕ್ತ)
    xasprintf (&ret, "/%s", id);
  ಅನ್ಯಥಾ ಯದಿ (path[0] == '/')
    ret = xstrdup (path);
  ಅನ್ಯಥಾ
    xasprintf (&ret, "/%s", path);

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_precreate_cgroup_cgroupfs (ಸಂರಚನಾ libcrun_cgroup_args *args, ಪೂರ್ಣಾಙ್ಕ *dirfd, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *parent_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, sub_path, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  ಯದಿ (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  ಅನ್ಯಥಾ ಯದಿ (errno != EEXIST)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = enable_controllers (sub_path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  ಯದಿ (UNLIKELY (*dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
make_new_sibling_cgroup (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *current_cgroup = ರಿಕ್ತ;
  ವರ್ಣ *dir;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, ರಿಕ್ತ);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_enter_cgroupfs (ಸಂರಚನಾ libcrun_cgroup_args *args, ಸಂರಚನಾ libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  ಯದಿ (args->joined)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          ಯದಿ (args->cgroup_path == ರಿಕ್ತ)
            {
              libcrun_error_t tmp_err = ರಿಕ್ತ;
              ಪೂರ್ಣಾಙ್ಕ tmp_ret;

              ಮುಕ್ತ (out->path);
              out->path = ರಿಕ್ತ;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              ಯದಿ (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  ಪ್ರತ್ಯಾಗಮ ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_destroy_cgroup_cgroupfs (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (mode < 0))
    ಪ್ರತ್ಯಾಗಮ mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  ಯದಿ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ಪ್ರತ್ಯಾಗಮ destroy_cgroup_path (cgroup_status->path, mode, err);
}

ಸಂರಚನಾ libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
