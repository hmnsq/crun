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

#وصف_ڪريو GNU_ذريعو

#ضمن <config.h>
#ضمن "cgroup.h"
#ضمن "cgroup-internal.h"
#ضمن "cgroup-systemd.h"
#ضمن "cgroup-setup.h"
#ضمن "cgroup-utils.h"
#ضمن "cgroup-cgroupfs.h"
#ضمن <string.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <sys/vfs.h>
#ضمن <inttypes.h>
#ضمن <time.h>

#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <libgen.h>

ساكن حرف *
make_cgroup_رستو (ثابت حرف *رستو, ثابت حرف *id)
{
  حرف *ret;

  إذا (رستو == عدم)
    xasprintf (&ret, "/%s", id);
  وإلا إذا (رستو[0] == '/')
    ret = xstrdup (رستو);
  وإلا
    xasprintf (&ret, "/%s", رستو);

  أرجع ret;
}

ساكن صحيح
libcrun_precreate_cgroup_cgroupfs (بنية libcrun_cgroup_دليلون *دليلون, صحيح *dirfd, libcrun_error_t *err)
{
  cleanup_free حرف *sub_رستو = make_cgroup_رستو (دليلون->cgroup_رستو, دليلون->id);
  cleanup_free حرف *cgroup_رستو = عدم;
  cleanup_free حرف *والد_رستو = عدم;
  صحيح ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_رستو, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&cgroup_رستو, err, CGROUP_پاڙ, sub_رستو, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  والد_رستو = xstrdup (cgroup_رستو);
  ret = crun_ensure_directory (dirname (والد_رستو), 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      أرجع 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  إذا (mkdir (cgroup_رستو, 0755) == 0)
    دليلون->precreated_رستو = xstrdup (cgroup_رستو);
  وإلا إذا (errno != EEXIST)
    أرجع 0;

  ret = enable_controllers (sub_رستو, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  *dirfd = open (cgroup_رستو, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_رستو);
  إذا (UNLIKELY (*dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_رستو);

  أرجع 0;
}

ساكن صحيح
make_new_sibling_cgroup (حرف **ٻاهريون, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *current_cgroup = عدم;
  حرف *dir;
  صحيح ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dir = dirname (current_cgroup);

  append_paths (ٻاهريون, err, dir, id, عدم);
  أرجع 0;
}

ساكن صحيح
libcrun_cgroup_enter_cgroupfs (بنية libcrun_cgroup_دليلون *دليلون, بنية libcrun_cgroup_حالت *ٻاهريون, libcrun_error_t *err)
{
  pid_t pid = دليلون->pid;
  صحيح cgroup_گهڻائي;
  صحيح ret;

  cgroup_گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (cgroup_گهڻائي < 0))
    أرجع cgroup_گهڻائي;

  ٻاهريون->رستو = make_cgroup_رستو (دليلون->cgroup_رستو, دليلون->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  إذا (دليلون->joined)
    أرجع 0;

  إذا (cgroup_گهڻائي == CGROUP_گهڻائي_UNIFIED)
    {
      ret = enable_controllers (ٻاهريون->رستو, err);
      إذا (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          إذا (دليلون->cgroup_رستو == عدم)
            {
              libcrun_error_t tmp_err = عدم;
              صحيح tmp_ret;

              حرر (ٻاهريون->رستو);
              ٻاهريون->رستو = عدم;

              tmp_ret = make_new_sibling_cgroup (&ٻاهريون->رستو, دليلون->id, &tmp_err);
              إذا (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  أرجع ret;
                }

              crun_error_release (err);
              ret = enable_controllers (ٻاهريون->رستو, err);
            }
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (ٻاهريون->رستو, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع enter_cgroup (cgroup_گهڻائي, pid, 0, ٻاهريون->رستو, true, err);
}

ساكن صحيح
libcrun_destroy_cgroup_cgroupfs (بنية libcrun_cgroup_حالت *cgroup_حالت,
                                 libcrun_error_t *err)
{
  صحيح گهڻائي;
  صحيح ret;

  گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (گهڻائي < 0))
    أرجع گهڻائي;

  ret = cgroup_killall_رستو (cgroup_حالت->رستو, SIGKILL, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  أرجع destroy_cgroup_رستو (cgroup_حالت->رستو, گهڻائي, err);
}

بنية libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
