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

  ຖ້າ (path == NULL)
    xasprintf (&ret, "/%s", id);
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (path[0] == '/')
    ret = xstrdup (path);
  ບໍ່ດັ່ງນັ້ນ
    xasprintf (&ret, "/%s", path);

  ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ
libcrun_precreate_cgroup_cgroupfs (sa_nracanaa libcrun_cgroup_args *args, ຈຳນວນເຕັມ *dirfd, libcrun_error_t *err)
{
  cleanup_free var_na *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_free var_na *parent_path = NULL;
  ຈຳນວນເຕັມ ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, sub_path, NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      ສົ່ງຄືນ 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  ຖ້າ (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (errno != EEXIST)
    ສົ່ງຄືນ 0;

  ret = enable_controllers (sub_path, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  ຖ້າ (UNLIKELY (*dirfd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
make_new_sibling_cgroup (var_na **out, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *current_cgroup = NULL;
  var_na *dir;
  ຈຳນວນເຕັມ ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, NULL);
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
libcrun_cgroup_enter_cgroupfs (sa_nracanaa libcrun_cgroup_args *args, sa_nracanaa libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  ຈຳນວນເຕັມ cgroup_mode;
  ຈຳນວນເຕັມ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (UNLIKELY (cgroup_mode < 0))
    ສົ່ງຄືນ cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  ຖ້າ (args->joined)
    ສົ່ງຄືນ 0;

  ຖ້າ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          ຖ້າ (args->cgroup_path == NULL)
            {
              libcrun_error_t tmp_err = NULL;
              ຈຳນວນເຕັມ tmp_ret;

              free (out->path);
              out->path = NULL;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              ຖ້າ (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  ສົ່ງຄືນ ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

static ຈຳນວນເຕັມ
libcrun_destroy_cgroup_cgroupfs (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  ຈຳນວນເຕັມ mode;
  ຈຳນວນເຕັມ ret;

  mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (UNLIKELY (mode < 0))
    ສົ່ງຄືນ mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  ຖ້າ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ສົ່ງຄືນ destroy_cgroup_path (cgroup_status->path, mode, err);
}

sa_nracanaa libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
