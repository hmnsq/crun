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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-systemd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-cgroupfs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/vfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <libgen.h>

ਸਥਿਰ ਅੱਖਰ *
make_cgroup_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *id)
{
  ਅੱਖਰ *ret;

  ਜੇ (ਰਸਤਾ == ਨਿਰਾ)
    xasprintf (&ret, "/%s", id);
  ਨਹੀਂਤਾਂ ਜੇ (ਰਸਤਾ[0] == '/')
    ret = xstrdup (ਰਸਤਾ);
  ਨਹੀਂਤਾਂ
    xasprintf (&ret, "/%s", ਰਸਤਾ);

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
libcrun_precreate_cgroup_cgroupfs (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ, ਪੂਰਨ *dirfd, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *sub_ਰਸਤਾ = make_cgroup_ਰਸਤਾ (ਦਲੀਲਾਂ->cgroup_ਰਸਤਾ, ਦਲੀਲਾਂ->id);
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਮਾਪਾ_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, sub_ਰਸਤਾ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਮਾਪਾ_ਰਸਤਾ = xstrdup (cgroup_ਰਸਤਾ);
  ret = crun_ensure_directory (dirname (ਮਾਪਾ_ਰਸਤਾ), 0755, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      ਵਾਪਸ 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  ਜੇ (mkdir (cgroup_ਰਸਤਾ, 0755) == 0)
    ਦਲੀਲਾਂ->precreated_ਰਸਤਾ = xstrdup (cgroup_ਰਸਤਾ);
  ਨਹੀਂਤਾਂ ਜੇ (errno != EEXIST)
    ਵਾਪਸ 0;

  ret = enable_controllers (sub_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  *dirfd = open (cgroup_ਰਸਤਾ, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_ਰਸਤਾ);
  ਜੇ (UNLIKELY (*dirfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", cgroup_ਰਸਤਾ);

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
make_new_sibling_cgroup (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *current_cgroup = ਨਿਰਾ;
  ਅੱਖਰ *dir;
  ਪੂਰਨ ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  dir = dirname (current_cgroup);

  append_paths (ਬਾਹਰਲਾ, err, dir, id, ਨਿਰਾ);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libcrun_cgroup_enter_cgroupfs (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ, ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  pid_t pid = ਦਲੀਲਾਂ->pid;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਬਾਹਰਲਾ->ਰਸਤਾ = make_cgroup_ਰਸਤਾ (ਦਲੀਲਾਂ->cgroup_ਰਸਤਾ, ਦਲੀਲਾਂ->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  ਜੇ (ਦਲੀਲਾਂ->joined)
    ਵਾਪਸ 0;

  ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
    {
      ret = enable_controllers (ਬਾਹਰਲਾ->ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          ਜੇ (ਦਲੀਲਾਂ->cgroup_ਰਸਤਾ == ਨਿਰਾ)
            {
              libcrun_error_t tmp_err = ਨਿਰਾ;
              ਪੂਰਨ tmp_ret;

              ਮੁਕਤ (ਬਾਹਰਲਾ->ਰਸਤਾ);
              ਬਾਹਰਲਾ->ਰਸਤਾ = ਨਿਰਾ;

              tmp_ret = make_new_sibling_cgroup (&ਬਾਹਰਲਾ->ਰਸਤਾ, ਦਲੀਲਾਂ->id, &tmp_err);
              ਜੇ (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  ਵਾਪਸ ret;
                }

              crun_error_release (err);
              ret = enable_controllers (ਬਾਹਰਲਾ->ਰਸਤਾ, err);
            }
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (ਬਾਹਰਲਾ->ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ enter_cgroup (cgroup_ਬਹੁਲਕ, pid, 0, ਬਾਹਰਲਾ->ਰਸਤਾ, true, err);
}

ਸਥਿਰ ਪੂਰਨ
libcrun_destroy_cgroup_cgroupfs (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ,
                                 libcrun_error_t *err)
{
  ਪੂਰਨ ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (ਬਹੁਲਕ < 0))
    ਵਾਪਸ ਬਹੁਲਕ;

  ret = cgroup_killall_ਰਸਤਾ (cgroup_ਸਥਿਤੀ->ਰਸਤਾ, SIGKILL, err);
  ਜੇ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ਵਾਪਸ destroy_cgroup_ਰਸਤਾ (cgroup_ਸਥਿਤੀ->ਰਸਤਾ, ਬਹੁਲਕ, err);
}

ਬਣਤਰ libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
