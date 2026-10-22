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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "cgroup.h"
#समावेश "cgroup-internal.h"
#समावेश "cgroup-systemd.h"
#समावेश "cgroup-setup.h"
#समावेश "cgroup-utils.h"
#समावेश "cgroup-cgroupfs.h"
#समावेश <string.h>
#समावेश <sys/types.h>
#समावेश <signal.h>
#समावेश <sys/vfs.h>
#समावेश <inttypes.h>
#समावेश <time.h>

#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#समावेश <fcntl.h>
#समावेश <libgen.h>

अटल अक्षर *
make_cgroup_path (स्थिर अक्षर *path, स्थिर अक्षर *id)
{
  अक्षर *ret;

  अगर (path == शून्य)
    xasprintf (&ret, "/%s", id);
  अन्यथा अगर (path[0] == '/')
    ret = xstrdup (path);
  अन्यथा
    xasprintf (&ret, "/%s", path);

  वापस ret;
}

अटल पूर्णांक
libcrun_precreate_cgroup_cgroupfs (काष्ठा libcrun_cgroup_args *args, पूर्णांक *dirfd, libcrun_error_t *err)
{
  cleanup_free अक्षर *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free अक्षर *cgroup_path = शून्य;
  cleanup_free अक्षर *parent_path = शून्य;
  पूर्णांक ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_paths (&cgroup_path, err, CGROUP_मूल, sub_path, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (स_नाम (parent_path), 0755, true, err);
  अगर (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      वापस 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  अगर (सूची_गढ़ो (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  अन्यथा अगर (त्रुटि_सं != EEXIST)
    वापस 0;

  ret = enable_controllers (sub_path, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  *dirfd = खोलो (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  अगर (UNLIKELY (*dirfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  वापस 0;
}

अटल पूर्णांक
make_new_sibling_cgroup (अक्षर **out, स्थिर अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *current_cgroup = शून्य;
  अक्षर *dir;
  पूर्णांक ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  dir = स_नाम (current_cgroup);

  append_paths (out, err, dir, id, शून्य);
  वापस 0;
}

अटल पूर्णांक
libcrun_cgroup_enter_cgroupfs (काष्ठा libcrun_cgroup_args *args, काष्ठा libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  अगर (args->joined)
    वापस 0;

  अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      अगर (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          अगर (args->cgroup_path == शून्य)
            {
              libcrun_error_t tmp_err = शून्य;
              पूर्णांक tmp_ret;

              मुक्त (out->path);
              out->path = शून्य;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              अगर (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  वापस ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

अटल पूर्णांक
libcrun_destroy_cgroup_cgroupfs (काष्ठा libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  पूर्णांक mode;
  पूर्णांक ret;

  mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (mode < 0))
    वापस mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  अगर (UNLIKELY (ret < 0))
    crun_error_release (err);

  वापस destroy_cgroup_path (cgroup_status->path, mode, err);
}

काष्ठा libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
