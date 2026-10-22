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

  အကယ်၍ (path == NULL)
    xasprintf (&ret, "/%s", id);
  မဟုတ်ပါက အကယ်၍ (path[0] == '/')
    ret = xstrdup (path);
  မဟုတ်ပါက
    xasprintf (&ret, "/%s", path);

  ပြန်ပေး ret;
}

static ကိန်းပြည့်
libcrun_precreate_cgroup_cgroupfs (sa_nracanaa libcrun_cgroup_args *args, ကိန်းပြည့် *dirfd, libcrun_error_t *err)
{
  cleanup_free var_na *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_free var_na *parent_path = NULL;
  ကိန်းပြည့် ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, sub_path, NULL);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      ပြန်ပေး 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  အကယ်၍ (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  မဟုတ်ပါက အကယ်၍ (errno != EEXIST)
    ပြန်ပေး 0;

  ret = enable_controllers (sub_path, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  အကယ်၍ (UNLIKELY (*dirfd < 0))
    ပြန်ပေး crun_make_error (err, errno, "open `%s`", cgroup_path);

  ပြန်ပေး 0;
}

static ကိန်းပြည့်
make_new_sibling_cgroup (var_na **out, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *current_cgroup = NULL;
  var_na *dir;
  ကိန်းပြည့် ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, NULL);
  ပြန်ပေး 0;
}

static ကိန်းပြည့်
libcrun_cgroup_enter_cgroupfs (sa_nracanaa libcrun_cgroup_args *args, sa_nracanaa libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  ကိန်းပြည့် cgroup_mode;
  ကိန်းပြည့် ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  အကယ်၍ (UNLIKELY (cgroup_mode < 0))
    ပြန်ပေး cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  အကယ်၍ (args->joined)
    ပြန်ပေး 0;

  အကယ်၍ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      အကယ်၍ (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          အကယ်၍ (args->cgroup_path == NULL)
            {
              libcrun_error_t tmp_err = NULL;
              ကိန်းပြည့် tmp_ret;

              free (out->path);
              out->path = NULL;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              အကယ်၍ (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  ပြန်ပေး ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          အကယ်၍ (UNLIKELY (ret < 0))
            ပြန်ပေး ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ပြန်ပေး enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

static ကိန်းပြည့်
libcrun_destroy_cgroup_cgroupfs (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  ကိန်းပြည့် mode;
  ကိန်းပြည့် ret;

  mode = libcrun_get_cgroup_mode (err);
  အကယ်၍ (UNLIKELY (mode < 0))
    ပြန်ပေး mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ပြန်ပေး destroy_cgroup_path (cgroup_status->path, mode, err);
}

sa_nracanaa libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
