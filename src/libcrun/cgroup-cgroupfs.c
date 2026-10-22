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

स्थिर अक्षर *
make_cgroup_path (स्थिरांक अक्षर *path, स्थिरांक अक्षर *id)
{
  अक्षर *ret;

  जर (path == रिक्त)
    xasprintf (&ret, "/%s", id);
  अन्यथा जर (path[0] == '/')
    ret = xstrdup (path);
  अन्यथा
    xasprintf (&ret, "/%s", path);

  परत ret;
}

स्थिर पूर्णांक
libcrun_precreate_cgroup_cgroupfs (संरचना libcrun_cgroup_args *args, पूर्णांक *dirfd, libcrun_error_t *err)
{
  cleanup_free अक्षर *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free अक्षर *cgroup_path = रिक्त;
  cleanup_free अक्षर *parent_path = रिक्त;
  पूर्णांक ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = append_paths (&cgroup_path, err, CGROUP_मूळ, sub_path, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (स_नाम (parent_path), 0755, true, err);
  जर (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      परत 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  जर (सूची_गढ़ो (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  अन्यथा जर (त्रुटि_सं != EEXIST)
    परत 0;

  ret = enable_controllers (sub_path, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  *dirfd = खोलो (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  जर (UNLIKELY (*dirfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  परत 0;
}

स्थिर पूर्णांक
make_new_sibling_cgroup (अक्षर **out, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *current_cgroup = रिक्त;
  अक्षर *dir;
  पूर्णांक ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  dir = स_नाम (current_cgroup);

  append_paths (out, err, dir, id, रिक्त);
  परत 0;
}

स्थिर पूर्णांक
libcrun_cgroup_enter_cgroupfs (संरचना libcrun_cgroup_args *args, संरचना libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (cgroup_mode < 0))
    परत cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  जर (args->joined)
    परत 0;

  जर (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      जर (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          जर (args->cgroup_path == रिक्त)
            {
              libcrun_error_t tmp_err = रिक्त;
              पूर्णांक tmp_ret;

              मुक्त (out->path);
              out->path = रिक्त;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              जर (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  परत ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          जर (UNLIKELY (ret < 0))
            परत ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

स्थिर पूर्णांक
libcrun_destroy_cgroup_cgroupfs (संरचना libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  पूर्णांक mode;
  पूर्णांक ret;

  mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (mode < 0))
    परत mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  जर (UNLIKELY (ret < 0))
    crun_error_release (err);

  परत destroy_cgroup_path (cgroup_status->path, mode, err);
}

संरचना libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
