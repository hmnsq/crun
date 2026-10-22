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

static char *
make_cgroup_path (const char *path, const char *id)
{
  char *ret;

  만약 (path == NULL)
    xasprintf (&ret, "/%s", id);
  아니면 만약 (path[0] == '/')
    ret = xstrdup (path);
  아니면
    xasprintf (&ret, "/%s", path);

  반환 ret;
}

static 정수
libcrun_precreate_cgroup_cgroupfs (struct libcrun_cgroup_args *args, 정수 *dirfd, libcrun_error_t *err)
{
  cleanup_free char *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free char *cgroup_path = NULL;
  cleanup_free char *parent_path = NULL;
  정수 ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, sub_path, NULL);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  만약 (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      반환 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  만약 (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  아니면 만약 (errno != EEXIST)
    반환 0;

  ret = enable_controllers (sub_path, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  만약 (UNLIKELY (*dirfd < 0))
    반환 crun_make_error (err, errno, "open `%s`", cgroup_path);

  반환 0;
}

static 정수
make_new_sibling_cgroup (char **out, const char *id, libcrun_error_t *err)
{
  cleanup_free char *current_cgroup = NULL;
  char *dir;
  정수 ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, NULL);
  반환 0;
}

static 정수
libcrun_cgroup_enter_cgroupfs (struct libcrun_cgroup_args *args, struct libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  정수 cgroup_mode;
  정수 ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  만약 (UNLIKELY (cgroup_mode < 0))
    반환 cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  만약 (args->joined)
    반환 0;

  만약 (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      만약 (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          만약 (args->cgroup_path == NULL)
            {
              libcrun_error_t tmp_err = NULL;
              정수 tmp_ret;

              free (out->path);
              out->path = NULL;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              만약 (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  반환 ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          만약 (UNLIKELY (ret < 0))
            반환 ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  반환 enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

static 정수
libcrun_destroy_cgroup_cgroupfs (struct libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  정수 mode;
  정수 ret;

  mode = libcrun_get_cgroup_mode (err);
  만약 (UNLIKELY (mode < 0))
    반환 mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  만약 (UNLIKELY (ret < 0))
    crun_error_release (err);

  반환 destroy_cgroup_path (cgroup_status->path, mode, err);
}

struct libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
