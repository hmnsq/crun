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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "cgroup.h"
#അന്തര്ഭാവ "cgroup-internal.h"
#അന്തര്ഭാവ "cgroup-systemd.h"
#അന്തര്ഭാവ "cgroup-setup.h"
#അന്തര്ഭാവ "cgroup-utils.h"
#അന്തര്ഭാവ "cgroup-cgroupfs.h"
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <sys/vfs.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <libgen.h>

സ്ഥിതം അക്ഷരം *
make_cgroup_path (സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *id)
{
  അക്ഷരം *ret;

  എങ്കിൽ (path == ശൂന്യ)
    xasprintf (&ret, "/%s", id);
  അല്ലെങ്കിൽ എങ്കിൽ (path[0] == '/')
    ret = xstrdup (path);
  അല്ലെങ്കിൽ
    xasprintf (&ret, "/%s", path);

  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_precreate_cgroup_cgroupfs (ഘടന libcrun_cgroup_args *args, പൂർണ്ണസംഖ്യ *dirfd, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
  cleanup_free അക്ഷരം *parent_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_paths (&cgroup_path, err, CGROUP_വേര്, sub_path, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      തിരികെ 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  എങ്കിൽ (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  അല്ലെങ്കിൽ എങ്കിൽ (errno != EEXIST)
    തിരികെ 0;

  ret = enable_controllers (sub_path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  എങ്കിൽ (UNLIKELY (*dirfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", cgroup_path);

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
make_new_sibling_cgroup (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *current_cgroup = ശൂന്യ;
  അക്ഷരം *dir;
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, ശൂന്യ);
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_cgroup_enter_cgroupfs (ഘടന libcrun_cgroup_args *args, ഘടന libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  എങ്കിൽ (args->joined)
    തിരികെ 0;

  എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          എങ്കിൽ (args->cgroup_path == ശൂന്യ)
            {
              libcrun_error_t tmp_err = ശൂന്യ;
              പൂർണ്ണസംഖ്യ tmp_ret;

              സ്വതന്ത്രം (out->path);
              out->path = ശൂന്യ;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              എങ്കിൽ (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  തിരികെ ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_destroy_cgroup_cgroupfs (ഘടന libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ mode;
  പൂർണ്ണസംഖ്യ ret;

  mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (mode < 0))
    തിരികെ mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    crun_error_release (err);

  തിരികെ destroy_cgroup_path (cgroup_status->path, mode, err);
}

ഘടന libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
