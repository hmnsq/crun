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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "cgroup.h"
#அந்தர்பாவ "cgroup-internal.h"
#அந்தர்பாவ "cgroup-systemd.h"
#அந்தர்பாவ "cgroup-setup.h"
#அந்தர்பாவ "cgroup-utils.h"
#அந்தர்பாவ "cgroup-cgroupfs.h"
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <sys/vfs.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ <time.h>

#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <libgen.h>

நிலையான எழுத்து *
make_cgroup_path (மாறிலி எழுத்து *path, மாறிலி எழுத்து *id)
{
  எழுத்து *ret;

  எனில் (path == வெற்று)
    xasprintf (&ret, "/%s", id);
  இல்லையெனில் எனில் (path[0] == '/')
    ret = xstrdup (path);
  இல்லையெனில்
    xasprintf (&ret, "/%s", path);

  திரும்பு ret;
}

நிலையான முழு
libcrun_precreate_cgroup_cgroupfs (கட்டமைப்பு libcrun_cgroup_args *args, முழு *dirfd, libcrun_error_t *err)
{
  cleanup_free எழுத்து *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free எழுத்து *cgroup_path = வெற்று;
  cleanup_free எழுத்து *parent_path = வெற்று;
  முழு ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_paths (&cgroup_path, err, CGROUP_வேர், sub_path, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  எனில் (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      திரும்பு 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  எனில் (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  இல்லையெனில் எனில் (errno != EEXIST)
    திரும்பு 0;

  ret = enable_controllers (sub_path, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  எனில் (UNLIKELY (*dirfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", cgroup_path);

  திரும்பு 0;
}

நிலையான முழு
make_new_sibling_cgroup (எழுத்து **out, மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  cleanup_free எழுத்து *current_cgroup = வெற்று;
  எழுத்து *dir;
  முழு ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, வெற்று);
  திரும்பு 0;
}

நிலையான முழு
libcrun_cgroup_enter_cgroupfs (கட்டமைப்பு libcrun_cgroup_args *args, கட்டமைப்பு libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  முழு cgroup_mode;
  முழு ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  எனில் (args->joined)
    திரும்பு 0;

  எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      எனில் (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          எனில் (args->cgroup_path == வெற்று)
            {
              libcrun_error_t tmp_err = வெற்று;
              முழு tmp_ret;

              விடுவி (out->path);
              out->path = வெற்று;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              எனில் (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  திரும்பு ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

நிலையான முழு
libcrun_destroy_cgroup_cgroupfs (கட்டமைப்பு libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  முழு mode;
  முழு ret;

  mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (mode < 0))
    திரும்பு mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  எனில் (UNLIKELY (ret < 0))
    crun_error_release (err);

  திரும்பு destroy_cgroup_path (cgroup_status->path, mode, err);
}

கட்டமைப்பு libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
