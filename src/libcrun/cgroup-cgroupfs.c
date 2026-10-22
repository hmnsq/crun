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

#تعریف_کړه GNU_سرچینه

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
make_cgroup_لار (ثابت حرف *لار, ثابت حرف *id)
{
  حرف *ret;

  إذا (لار == عدم)
    xasprintf (&ret, "/%s", id);
  وإلا إذا (لار[0] == '/')
    ret = xstrdup (لار);
  وإلا
    xasprintf (&ret, "/%s", لار);

  أرجع ret;
}

ساكن صحيح
libcrun_precreate_cgroup_cgroupfs (بنية libcrun_cgroup_دلیلونه *دلیلونه, صحيح *dirfd, libcrun_error_t *err)
{
  cleanup_free حرف *sub_لار = make_cgroup_لار (دلیلونه->cgroup_لار, دلیلونه->id);
  cleanup_free حرف *cgroup_لار = عدم;
  cleanup_free حرف *پلار_لار = عدم;
  صحيح ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_لار, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&cgroup_لار, err, CGROUP_ریښه, sub_لار, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  پلار_لار = xstrdup (cgroup_لار);
  ret = crun_ensure_directory (dirname (پلار_لار), 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      أرجع 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  إذا (mkdir (cgroup_لار, 0755) == 0)
    دلیلونه->precreated_لار = xstrdup (cgroup_لار);
  وإلا إذا (errno != EEXIST)
    أرجع 0;

  ret = enable_controllers (sub_لار, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  *dirfd = open (cgroup_لار, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_لار);
  إذا (UNLIKELY (*dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_لار);

  أرجع 0;
}

ساكن صحيح
make_new_sibling_cgroup (حرف **بهر, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *current_cgroup = عدم;
  حرف *dir;
  صحيح ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dir = dirname (current_cgroup);

  append_paths (بهر, err, dir, id, عدم);
  أرجع 0;
}

ساكن صحيح
libcrun_cgroup_enter_cgroupfs (بنية libcrun_cgroup_دلیلونه *دلیلونه, بنية libcrun_cgroup_حالت *بهر, libcrun_error_t *err)
{
  pid_t pid = دلیلونه->pid;
  صحيح cgroup_ډېرپېښ;
  صحيح ret;

  cgroup_ډېرپېښ = libcrun_get_cgroup_ډېرپېښ (err);
  إذا (UNLIKELY (cgroup_ډېرپېښ < 0))
    أرجع cgroup_ډېرپېښ;

  بهر->لار = make_cgroup_لار (دلیلونه->cgroup_لار, دلیلونه->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  إذا (دلیلونه->joined)
    أرجع 0;

  إذا (cgroup_ډېرپېښ == CGROUP_ډېرپېښ_UNIFIED)
    {
      ret = enable_controllers (بهر->لار, err);
      إذا (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          إذا (دلیلونه->cgroup_لار == عدم)
            {
              libcrun_error_t tmp_err = عدم;
              صحيح tmp_ret;

              حرر (بهر->لار);
              بهر->لار = عدم;

              tmp_ret = make_new_sibling_cgroup (&بهر->لار, دلیلونه->id, &tmp_err);
              إذا (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  أرجع ret;
                }

              crun_error_release (err);
              ret = enable_controllers (بهر->لار, err);
            }
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (بهر->لار, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع enter_cgroup (cgroup_ډېرپېښ, pid, 0, بهر->لار, true, err);
}

ساكن صحيح
libcrun_destroy_cgroup_cgroupfs (بنية libcrun_cgroup_حالت *cgroup_حالت,
                                 libcrun_error_t *err)
{
  صحيح ډېرپېښ;
  صحيح ret;

  ډېرپېښ = libcrun_get_cgroup_ډېرپېښ (err);
  إذا (UNLIKELY (ډېرپېښ < 0))
    أرجع ډېرپېښ;

  ret = cgroup_killall_لار (cgroup_حالت->لار, SIGKILL, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  أرجع destroy_cgroup_لار (cgroup_حالت->لار, ډېرپېښ, err);
}

بنية libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
