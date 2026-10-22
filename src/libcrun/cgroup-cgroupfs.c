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

#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل "cgroup.h"
#شامل "cgroup-internal.h"
#شامل "cgroup-systemd.h"
#شامل "cgroup-setup.h"
#شامل "cgroup-utils.h"
#شامل "cgroup-cgroupfs.h"
#شامل <string.h>
#شامل <sys/types.h>
#شامل <signal.h>
#شامل <sys/vfs.h>
#شامل <inttypes.h>
#شامل <time.h>

#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <libgen.h>

ساکن حرف *
make_cgroup_راستہ (ثابت حرف *راستہ, ثابت حرف *id)
{
  حرف *ret;

  اگر (راستہ == تہی)
    xasprintf (&ret, "/%s", id);
  ورنہ اگر (راستہ[0] == '/')
    ret = xstrdup (راستہ);
  ورنہ
    xasprintf (&ret, "/%s", راستہ);

  واپس ret;
}

ساکن صحیح
libcrun_precreate_cgroup_cgroupfs (ساخت libcrun_cgroup_دلائل *دلائل, صحیح *dirfd, libcrun_error_t *err)
{
  cleanup_free حرف *sub_راستہ = make_cgroup_راستہ (دلائل->cgroup_راستہ, دلائل->id);
  cleanup_free حرف *cgroup_راستہ = تہی;
  cleanup_free حرف *والد_راستہ = تہی;
  صحیح ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_راستہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, sub_راستہ, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  والد_راستہ = xstrdup (cgroup_راستہ);
  ret = crun_ensure_directory (dirname (والد_راستہ), 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      واپس 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  اگر (mkdir (cgroup_راستہ, 0755) == 0)
    دلائل->precreated_راستہ = xstrdup (cgroup_راستہ);
  ورنہ اگر (errno != EEXIST)
    واپس 0;

  ret = enable_controllers (sub_راستہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  *dirfd = open (cgroup_راستہ, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_راستہ);
  اگر (UNLIKELY (*dirfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", cgroup_راستہ);

  واپس 0;
}

ساکن صحیح
make_new_sibling_cgroup (حرف **برآمد, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *current_cgroup = تہی;
  حرف *dir;
  صحیح ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  dir = dirname (current_cgroup);

  append_paths (برآمد, err, dir, id, تہی);
  واپس 0;
}

ساکن صحیح
libcrun_cgroup_enter_cgroupfs (ساخت libcrun_cgroup_دلائل *دلائل, ساخت libcrun_cgroup_حالت *برآمد, libcrun_error_t *err)
{
  pid_t pid = دلائل->pid;
  صحیح cgroup_منوال;
  صحیح ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  برآمد->راستہ = make_cgroup_راستہ (دلائل->cgroup_راستہ, دلائل->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  اگر (دلائل->joined)
    واپس 0;

  اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
    {
      ret = enable_controllers (برآمد->راستہ, err);
      اگر (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          اگر (دلائل->cgroup_راستہ == تہی)
            {
              libcrun_error_t tmp_err = تہی;
              صحیح tmp_ret;

              آزاد (برآمد->راستہ);
              برآمد->راستہ = تہی;

              tmp_ret = make_new_sibling_cgroup (&برآمد->راستہ, دلائل->id, &tmp_err);
              اگر (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  واپس ret;
                }

              crun_error_release (err);
              ret = enable_controllers (برآمد->راستہ, err);
            }
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (برآمد->راستہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس enter_cgroup (cgroup_منوال, pid, 0, برآمد->راستہ, true, err);
}

ساکن صحیح
libcrun_destroy_cgroup_cgroupfs (ساخت libcrun_cgroup_حالت *cgroup_حالت,
                                 libcrun_error_t *err)
{
  صحیح منوال;
  صحیح ret;

  منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (منوال < 0))
    واپس منوال;

  ret = cgroup_killall_راستہ (cgroup_حالت->راستہ, SIGKILL, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);

  واپس destroy_cgroup_راستہ (cgroup_حالت->راستہ, منوال, err);
}

ساخت libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
