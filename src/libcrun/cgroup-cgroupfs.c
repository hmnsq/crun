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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "cgroup.h"
#అన్తర్భావ "cgroup-internal.h"
#అన్తర్భావ "cgroup-systemd.h"
#అన్తర్భావ "cgroup-setup.h"
#అన్తర్భావ "cgroup-utils.h"
#అన్తర్భావ "cgroup-cgroupfs.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <sys/vfs.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ <time.h>

#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <libgen.h>

స్థిర అక్షరం *
make_cgroup_మార్గం (స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *id)
{
  అక్షరం *ret;

  ఒకవేళ (మార్గం == ఖాళీ)
    xasprintf (&ret, "/%s", id);
  లేకపోతే ఒకవేళ (మార్గం[0] == '/')
    ret = xstrdup (మార్గం);
  లేకపోతే
    xasprintf (&ret, "/%s", మార్గం);

  తిరిగి ret;
}

స్థిర పూర్ణాంకం
libcrun_precreate_cgroup_cgroupfs (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, పూర్ణాంకం *dirfd, libcrun_error_t *err)
{
  cleanup_free అక్షరం *sub_మార్గం = make_cgroup_మార్గం (వాదనలు->cgroup_మార్గం, వాదనలు->id);
  cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *మాతృ_మార్గం = ఖాళీ;
  పూర్ణాంకం ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, sub_మార్గం, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  మాతృ_మార్గం = xstrdup (cgroup_మార్గం);
  ret = crun_ensure_directory (dirname (మాతృ_మార్గం), 0755, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      తిరిగి 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  ఒకవేళ (mkdir (cgroup_మార్గం, 0755) == 0)
    వాదనలు->precreated_మార్గం = xstrdup (cgroup_మార్గం);
  లేకపోతే ఒకవేళ (errno != EEXIST)
    తిరిగి 0;

  ret = enable_controllers (sub_మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  *dirfd = open (cgroup_మార్గం, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_మార్గం);
  ఒకవేళ (UNLIKELY (*dirfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", cgroup_మార్గం);

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
make_new_sibling_cgroup (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  cleanup_free అక్షరం *current_cgroup = ఖాళీ;
  అక్షరం *dir;
  పూర్ణాంకం ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  dir = dirname (current_cgroup);

  append_paths (బహిర్గతం, err, dir, id, ఖాళీ);
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libcrun_cgroup_enter_cgroupfs (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, నిర్మాణం libcrun_cgroup_స్థితి *బహిర్గతం, libcrun_error_t *err)
{
  pid_t pid = వాదనలు->pid;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం ret;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  బహిర్గతం->మార్గం = make_cgroup_మార్గం (వాదనలు->cgroup_మార్గం, వాదనలు->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  ఒకవేళ (వాదనలు->joined)
    తిరిగి 0;

  ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
    {
      ret = enable_controllers (బహిర్గతం->మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          ఒకవేళ (వాదనలు->cgroup_మార్గం == ఖాళీ)
            {
              libcrun_error_t tmp_err = ఖాళీ;
              పూర్ణాంకం tmp_ret;

              విడుదల (బహిర్గతం->మార్గం);
              బహిర్గతం->మార్గం = ఖాళీ;

              tmp_ret = make_new_sibling_cgroup (&బహిర్గతం->మార్గం, వాదనలు->id, &tmp_err);
              ఒకవేళ (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  తిరిగి ret;
                }

              crun_error_release (err);
              ret = enable_controllers (బహిర్గతం->మార్గం, err);
            }
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (బహిర్గతం->మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి enter_cgroup (cgroup_బహుళకం, pid, 0, బహిర్గతం->మార్గం, true, err);
}

స్థిర పూర్ణాంకం
libcrun_destroy_cgroup_cgroupfs (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి,
                                 libcrun_error_t *err)
{
  పూర్ణాంకం బహుళకం;
  పూర్ణాంకం ret;

  బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (బహుళకం < 0))
    తిరిగి బహుళకం;

  ret = cgroup_killall_మార్గం (cgroup_స్థితి->మార్గం, SIGKILL, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    crun_error_release (err);

  తిరిగి destroy_cgroup_మార్గం (cgroup_స్థితి->మార్గం, బహుళకం, err);
}

నిర్మాణం libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
