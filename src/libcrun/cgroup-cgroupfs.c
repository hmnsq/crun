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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "cgroup.h"
#અન્તર્ભાવ "cgroup-internal.h"
#અન્તર્ભાવ "cgroup-systemd.h"
#અન્તર્ભાવ "cgroup-setup.h"
#અન્તર્ભાવ "cgroup-utils.h"
#અન્તર્ભાવ "cgroup-cgroupfs.h"
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <sys/vfs.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <libgen.h>

સ્થિર અક્ષર *
make_cgroup_path (અચલ અક્ષર *path, અચલ અક્ષર *id)
{
  અક્ષર *ret;

  જો (path == ખાલી)
    xasprintf (&ret, "/%s", id);
  નહીંતર જો (path[0] == '/')
    ret = xstrdup (path);
  નહીંતર
    xasprintf (&ret, "/%s", path);

  પાછા ret;
}

સ્થિર પૂર્ણાંક
libcrun_precreate_cgroup_cgroupfs (સંરચના libcrun_cgroup_args *args, પૂર્ણાંક *dirfd, libcrun_error_t *err)
{
  cleanup_free અક્ષર *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free અક્ષર *cgroup_path = ખાલી;
  cleanup_free અક્ષર *parent_path = ખાલી;
  પૂર્ણાંક ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, sub_path, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  જો (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      પાછા 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  જો (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  નહીંતર જો (errno != EEXIST)
    પાછા 0;

  ret = enable_controllers (sub_path, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  જો (UNLIKELY (*dirfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", cgroup_path);

  પાછા 0;
}

સ્થિર પૂર્ણાંક
make_new_sibling_cgroup (અક્ષર **out, અચલ અક્ષર *id, libcrun_error_t *err)
{
  cleanup_free અક્ષર *current_cgroup = ખાલી;
  અક્ષર *dir;
  પૂર્ણાંક ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, ખાલી);
  પાછા 0;
}

સ્થિર પૂર્ણાંક
libcrun_cgroup_enter_cgroupfs (સંરચના libcrun_cgroup_args *args, સંરચના libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  જો (args->joined)
    પાછા 0;

  જો (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      જો (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          જો (args->cgroup_path == ખાલી)
            {
              libcrun_error_t tmp_err = ખાલી;
              પૂર્ણાંક tmp_ret;

              મુક્ત (out->path);
              out->path = ખાલી;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              જો (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  પાછા ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

સ્થિર પૂર્ણાંક
libcrun_destroy_cgroup_cgroupfs (સંરચના libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  પૂર્ણાંક mode;
  પૂર્ણાંક ret;

  mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (mode < 0))
    પાછા mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  જો (UNLIKELY (ret < 0))
    crun_error_release (err);

  પાછા destroy_cgroup_path (cgroup_status->path, mode, err);
}

સંરચના libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
