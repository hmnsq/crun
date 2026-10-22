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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-internal.h"
#ಅನ್ತರ್ಭಾವ "cgroup-systemd.h"
#ಅನ್ತರ್ಭಾವ "cgroup-setup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"
#ಅನ್ತರ್ಭಾವ "cgroup-cgroupfs.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>

ಸ್ಥಿರ ಅಕ್ಷರ *
make_cgroup_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id)
{
  ಅಕ್ಷರ *ret;

  ಆದರೆ (ಮಾರ್ಗ == ಖಾಲಿ)
    xasprintf (&ret, "/%s", id);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮಾರ್ಗ[0] == '/')
    ret = xstrdup (ಮಾರ್ಗ);
  ಇಲ್ಲದಿದ್ದರೆ
    xasprintf (&ret, "/%s", ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_precreate_cgroup_cgroupfs (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ಪೂರ್ಣಾಂಕ *dirfd, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *sub_ಮಾರ್ಗ = make_cgroup_ಮಾರ್ಗ (ವಾದಗಳು->cgroup_ಮಾರ್ಗ, ವಾದಗಳು->id);
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಪೋಷಕ_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, sub_ಮಾರ್ಗ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಪೋಷಕ_ಮಾರ್ಗ = xstrdup (cgroup_ಮಾರ್ಗ);
  ret = crun_ensure_directory (dirname (ಪೋಷಕ_ಮಾರ್ಗ), 0755, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      ಹಿಂತಿರುಗಿ 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  ಆದರೆ (mkdir (cgroup_ಮಾರ್ಗ, 0755) == 0)
    ವಾದಗಳು->precreated_ಮಾರ್ಗ = xstrdup (cgroup_ಮಾರ್ಗ);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno != EEXIST)
    ಹಿಂತಿರುಗಿ 0;

  ret = enable_controllers (sub_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  *dirfd = open (cgroup_ಮಾರ್ಗ, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (*dirfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", cgroup_ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
make_new_sibling_cgroup (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *current_cgroup = ಖಾಲಿ;
  ಅಕ್ಷರ *dir;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  dir = dirname (current_cgroup);

  append_paths (ಹೊರಹೋಗು, err, dir, id, ಖಾಲಿ);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_cgroup_enter_cgroupfs (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಹೊರಹೋಗು, libcrun_error_t *err)
{
  pid_t pid = ವಾದಗಳು->pid;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಹೊರಹೋಗು->ಮಾರ್ಗ = make_cgroup_ಮಾರ್ಗ (ವಾದಗಳು->cgroup_ಮಾರ್ಗ, ವಾದಗಳು->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  ಆದರೆ (ವಾದಗಳು->joined)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
    {
      ret = enable_controllers (ಹೊರಹೋಗು->ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          ಆದರೆ (ವಾದಗಳು->cgroup_ಮಾರ್ಗ == ಖಾಲಿ)
            {
              libcrun_error_t tmp_err = ಖಾಲಿ;
              ಪೂರ್ಣಾಂಕ tmp_ret;

              ಬಿಡುಗಡೆ (ಹೊರಹೋಗು->ಮಾರ್ಗ);
              ಹೊರಹೋಗು->ಮಾರ್ಗ = ಖಾಲಿ;

              tmp_ret = make_new_sibling_cgroup (&ಹೊರಹೋಗು->ಮಾರ್ಗ, ವಾದಗಳು->id, &tmp_err);
              ಆದರೆ (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  ಹಿಂತಿರುಗಿ ret;
                }

              crun_error_release (err);
              ret = enable_controllers (ಹೊರಹೋಗು->ಮಾರ್ಗ, err);
            }
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (ಹೊರಹೋಗು->ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ enter_cgroup (cgroup_ಬಹುಲಕ, pid, 0, ಹೊರಹೋಗು->ಮಾರ್ಗ, true, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_destroy_cgroup_cgroupfs (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ,
                                 libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ ಬಹುಲಕ;

  ret = cgroup_killall_ಮಾರ್ಗ (cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ, SIGKILL, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ಹಿಂತಿರುಗಿ destroy_cgroup_ಮಾರ್ಗ (cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ, ಬಹುಲಕ, err);
}

ರಚನೆ libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
