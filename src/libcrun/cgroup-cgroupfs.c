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

#include <config.h>
#include "cgroup.h"
#include "cgroup-internal.h"
#include "cgroup-systemd.h"
#include "cgroup-setup.h"
#include "cgroup-utils.h"
#include "cgroup-cgroupfs.h"
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/vfs.h>
#include <inttypes.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <libgen.h>

static var_na *
make_cgroup_path (const var_na *path, const var_na *id)
{
  var_na *ret;

  නම් (path == NULL)
    xasprintf (&ret, "/%s", id);
  නැතහොත් නම් (path[0] == '/')
    ret = xstrdup (path);
  නැතහොත්
    xasprintf (&ret, "/%s", path);

  ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා
libcrun_precreate_cgroup_cgroupfs (sa_nracanaa libcrun_cgroup_args *args, පූර්ණ_සංඛ්‍යා *dirfd, libcrun_error_t *err)
{
  cleanup_free var_na *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_free var_na *parent_path = NULL;
  පූර්ණ_සංඛ්‍යා ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, sub_path, NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  නම් (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      ආපසු 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  නම් (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  නැතහොත් නම් (errno != EEXIST)
    ආපසු 0;

  ret = enable_controllers (sub_path, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  නම් (UNLIKELY (*dirfd < 0))
    ආපසු crun_make_error (err, errno, "open `%s`", cgroup_path);

  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
make_new_sibling_cgroup (var_na **out, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *current_cgroup = NULL;
  var_na *dir;
  පූර්ණ_සංඛ්‍යා ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, NULL);
  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
libcrun_cgroup_enter_cgroupfs (sa_nracanaa libcrun_cgroup_args *args, sa_nracanaa libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  පූර්ණ_සංඛ්‍යා cgroup_mode;
  පූර්ණ_සංඛ්‍යා ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (cgroup_mode < 0))
    ආපසු cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  නම් (args->joined)
    ආපසු 0;

  නම් (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      නම් (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          නම් (args->cgroup_path == NULL)
            {
              libcrun_error_t tmp_err = NULL;
              පූර්ණ_සංඛ්‍යා tmp_ret;

              free (out->path);
              out->path = NULL;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              නම් (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  ආපසු ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          නම් (UNLIKELY (ret < 0))
            ආපසු ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ආපසු enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

static පූර්ණ_සංඛ්‍යා
libcrun_destroy_cgroup_cgroupfs (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා mode;
  පූර්ණ_සංඛ්‍යා ret;

  mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (mode < 0))
    ආපසු mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  නම් (UNLIKELY (ret < 0))
    crun_error_release (err);

  ආපසු destroy_cgroup_path (cgroup_status->path, mode, err);
}

sa_nracanaa libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
