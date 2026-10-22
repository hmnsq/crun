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

#अन्तर्भाव <config.h>
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव "cgroup-internal.h"
#अन्तर्भाव "cgroup-systemd.h"
#अन्तर्भाव "cgroup-setup.h"
#अन्तर्भाव "cgroup-utils.h"
#अन्तर्भाव "cgroup-cgroupfs.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <libgen.h>

स्थिर वर्ण *
make_cgroup_path (ध्रुव वर्ण *path, ध्रुव वर्ण *id)
{
  वर्ण *ret;

  जइ (path == रिक्त)
    xasprintf (&ret, "/%s", id);
  अण्णहा जइ (path[0] == '/')
    ret = xstrdup (path);
  अण्णहा
    xasprintf (&ret, "/%s", path);

  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
libcrun_precreate_cgroup_cgroupfs (संरचना libcrun_cgroup_args *args, पूर्णाङ्क *dirfd, libcrun_error_t *err)
{
  cleanup_free वर्ण *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free वर्ण *cgroup_path = रिक्त;
  cleanup_free वर्ण *parent_path = रिक्त;
  पूर्णाङ्क ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (&cgroup_path, err, CGROUP_मूलम्, sub_path, रिक्त);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  जइ (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      प्रत्यागम 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  जइ (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  अण्णहा जइ (errno != EEXIST)
    प्रत्यागम 0;

  ret = enable_controllers (sub_path, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  जइ (UNLIKELY (*dirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_path);

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
make_new_sibling_cgroup (वर्ण **out, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *current_cgroup = रिक्त;
  वर्ण *dir;
  पूर्णाङ्क ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, रिक्त);
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libcrun_cgroup_enter_cgroupfs (संरचना libcrun_cgroup_args *args, संरचना libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जइ (UNLIKELY (cgroup_mode < 0))
    प्रत्यागम cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  जइ (args->joined)
    प्रत्यागम 0;

  जइ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      जइ (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          जइ (args->cgroup_path == रिक्त)
            {
              libcrun_error_t tmp_err = रिक्त;
              पूर्णाङ्क tmp_ret;

              मुक्त (out->path);
              out->path = रिक्त;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              जइ (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  प्रत्यागम ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

स्थिर पूर्णाङ्क
libcrun_destroy_cgroup_cgroupfs (संरचना libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  पूर्णाङ्क mode;
  पूर्णाङ्क ret;

  mode = libcrun_get_cgroup_mode (err);
  जइ (UNLIKELY (mode < 0))
    प्रत्यागम mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  जइ (UNLIKELY (ret < 0))
    crun_error_release (err);

  प्रत्यागम destroy_cgroup_path (cgroup_status->path, mode, err);
}

संरचना libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
