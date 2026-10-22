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
#అన్తర్భావ "cgroup-cgroupfs.h"
#అన్తర్భావ "cgroup-internal.h"
#అన్తర్భావ "cgroup-systemd.h"
#అన్తర్భావ "cgroup-utils.h"
#అన్తర్భావ "cgroup-setup.h"
#అన్తర్భావ "cgroup-resources.h"
#అన్తర్భావ "ebpf.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "status.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ <sched.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <sys/vfs.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ <time.h>

#అన్తర్భావ <linux/magic.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <libgen.h>

స్థిర పూర్ణాంకం
libcrun_cgroup_enter_disabled (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు arg_వాడని, నిర్మాణం libcrun_cgroup_స్థితి *బహిర్గతం, libcrun_error_t *err arg_వాడని)
{
  బహిర్గతం->మార్గం = ఖాళీ;
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libcrun_destroy_cgroup_disabled (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి arg_వాడని,
                                 libcrun_error_t *err arg_వాడని)
{
  తిరిగి 0;
}

నిర్మాణం libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = ఖాళీ,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

స్థిర పూర్ణాంకం
get_cgroup_manager (పూర్ణాంకం manager, నిర్మాణం libcrun_cgroup_manager **బహిర్గతం, libcrun_error_t *err)
{
  ఎంపిక (manager)
    {
    స్థితి CGROUP_MANAGER_DISABLED:
      *బహిర్గతం = &cgroup_manager_disabled;
      తిరిగి 0;

    స్థితి CGROUP_MANAGER_SYSTEMD:
      *బహిర్గతం = &cgroup_manager_systemd;
      తిరిగి 0;

    స్థితి CGROUP_MANAGER_CGROUPFS:
      *బహిర్గతం = &cgroup_manager_cgroupfs;
      తిరిగి 0;
    }

  *బహిర్గతం = ఖాళీ;
  తిరిగి crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

స్థిర స్థిరరాశి అక్షరం *
కనుగొను_delegate_cgroup (string_map *annotations)
{
  స్థిరరాశి అక్షరం *annotation;

  annotation = కనుగొను_string_map_విలువ (annotations, "run.oci.delegate-cgroup");
  ఒకవేళ (annotation)
    {
      ఒకవేళ (annotation[0] == '\0')
        తిరిగి ఖాళీ;
      తిరిగి annotation;
    }

  తిరిగి ఖాళీ;
}

పూర్ణాంకం
libcrun_cgroup_pause_unpause (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, స్థిరరాశి bool pause, libcrun_error_t *err)
{
  తిరిగి libcrun_cgroup_pause_unpause_మార్గం (స్థితి_2->మార్గం, pause, err);
}

పూర్ణాంకం
libcrun_cgroup_is_container_paused (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, bool *paused, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *cgroup_మార్గం = స్థితి_2->మార్గం;
  cleanup_free అక్షరం *content = ఖాళీ;
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  స్థిరరాశి అక్షరం *state;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం ret;

  ఒకవేళ (cgroup_మార్గం == ఖాళీ)
    తిరిగి 0;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
    {
      state = "1";

      ret = append_paths (&మార్గం, err, CGROUP_మూలం, cgroup_మార్గం, "cgroup.freeze", ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      state = "FROZEN";

      ret = append_paths (&మార్గం, err, CGROUP_మూలం "/freezer", cgroup_మార్గం, "freezer.state", ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = read_all_file (మార్గం, &content, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      ఒకవేళ (errno != ENOENT || cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
        తిరిగి ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      నిర్మాణం statfs freezer_stat;
      ఒకవేళ (statfs (CGROUP_మూలం "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          తిరిగి crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_మూలం "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      ఒకవేళ (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        తిరిగి ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      తిరిగి 0;
    }

  *paused = అక్షరశ్రేణి_శోధన (content, state) != ఖాళీ;
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_cgroup_join_process (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, స్థిరరాశి అక్షరం *మార్గం, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  నిర్మాణం libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = ఖాళీ;
  పూర్ణాంకం errno_;
  పూర్ణాంకం ret;

  ret = libcrun_move_process_గమ్యం_cgroup (pid, init_pid, మార్గం, false, err);
  ఒకవేళ (LIKELY (ret >= 0))
    తిరిగి ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  ఒకవేళ (errno_ != EACCES && errno_ != EPERM)
    తిరిగి ret;

  ఒకవేళ (get_cgroup_manager (స్థితి_2->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      తిరిగి ret;
    }
  ఒకవేళ (cgroup_manager->attach_process == ఖాళీ)
    తిరిగి ret;

  ఒకవేళ (UNLIKELY (cgroup_manager->attach_process (స్థితి_2, మార్గం, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, మార్గం, tmp_err->సందేశం);
      crun_error_release (&tmp_err);
      తిరిగి ret;
    }

  crun_error_release (err);
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_cgroup_read_pids (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  తిరిగి libcrun_cgroup_read_pids_from_మార్గం (స్థితి_2->మార్గం, recurse, pids, err);
}

పూర్ణాంకం
libcrun_cgroup_killall (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి, పూర్ణాంకం సంకేతం, libcrun_error_t *err)
{
  తిరిగి cgroup_killall_మార్గం (cgroup_స్థితి->మార్గం, సంకేతం, err);
}

పూర్ణాంకం
libcrun_cgroup_destroy (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి, libcrun_error_t *err)
{
  నిర్మాణం libcrun_cgroup_manager *cgroup_manager = ఖాళీ;
  పూర్ణాంకం ret;

  ret = get_cgroup_manager (cgroup_స్థితి->manager, &cgroup_manager, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి cgroup_manager->destroy_cgroup (cgroup_స్థితి, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
స్థిర పూర్ణాంకం
complete_cpu_max (స్థిరరాశి అక్షరం *మార్గం, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free అక్షరం *cpu_max_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *content = ఖాళీ;
  bool has_period;
  uint64_t period;
  int64_t quota;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం ret;

  ఒకవేళ (resources == ఖాళీ || resources->cpu == ఖాళీ)
    తిరిగి 0;

  cpu = resources->cpu;
  ఒకవేళ (cpu->quota_present == cpu->period_present)
    తిరిగి 0;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;
  ఒకవేళ (cgroup_బహుళకం != CGROUP_బహుళకం_UNIFIED)
    తిరిగి 0;

  ret = append_paths (&cpu_max_మార్గం, err, CGROUP_మూలం, మార్గం, "cpu.max", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = read_all_file (cpu_max_మార్గం, &content, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      తిరిగి 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  లేకపోతే ఒకవేళ (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_update_cgroup_resources (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి,
                                 స్థిరరాశి అక్షరం *state_మూలం,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  నిర్మాణం libcrun_cgroup_manager *cgroup_manager = ఖాళీ;
  పూర్ణాంకం ret;

  ret = get_cgroup_manager (cgroup_స్థితి->manager, &cgroup_manager, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = check_జ్ఞాపకం_before_update (cgroup_స్థితి->మార్గం, resources, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = complete_cpu_max (cgroup_స్థితి->మార్గం, resources, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_స్థితి, state_మూలం, resources, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి update_cgroup_resources (cgroup_స్థితి->మార్గం, state_మూలం, resources, ! cgroup_స్థితి->bpf_dev_సమితి, err);
}

స్థిర పూర్ణాంకం
can_ignore_cgroup_enter_errors (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, పూర్ణాంకం cgroup_బహుళకం,
                                libcrun_error_t *err)
{
  పూర్ణాంకం manager = వాదనలు->manager;
  పూర్ణాంకం rootless;

  rootless = is_rootless (err);
  ఒకవేళ (UNLIKELY (rootless < 0))
    తిరిగి rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  ఒకవేళ (! rootless)
    తిరిగి 0;

  ఒకవేళ (! is_ఖాళీలు_string (వాదనలు->cgroup_మార్గం))
    తిరిగి 0;

  ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    తిరిగి 0;

  తిరిగి 1;
}

పూర్ణాంకం
libcrun_cgroup_preenter (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, పూర్ణాంకం *dirfd, libcrun_error_t *err)
{
  నిర్మాణం libcrun_cgroup_manager *cgroup_manager;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం ret;

  *dirfd = -1;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (cgroup_బహుళకం != CGROUP_బహుళకం_UNIFIED)
    తిరిగి 0;

  ret = get_cgroup_manager (వాదనలు->manager, &cgroup_manager, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (cgroup_manager->precreate_cgroup == ఖాళీ)
    తిరిగి 0;

  తిరిగి cgroup_manager->precreate_cgroup (వాదనలు, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
శూన్య
libcrun_cgroup_preenter_cleanup (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు)
{
  ఒకవేళ (వాదనలు->precreated_మార్గం == ఖాళీ)
    తిరిగి;

  (శూన్య) rmdir (వాదనలు->precreated_మార్గం);
  విడుదల (వాదనలు->precreated_మార్గం);
  వాదనలు->precreated_మార్గం = ఖాళీ;
}

పూర్ణాంకం
libcrun_cgroup_enter (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, నిర్మాణం libcrun_cgroup_స్థితి **బహిర్గతం, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2 = xmalloc0 (పరిమాణం *స్థితి_2);
  నిర్మాణం libcrun_cgroup_manager *cgroup_manager;
  uid_t మూలం_uid = వాదనలు->మూలం_uid;
  uid_t మూలం_gid = వాదనలు->మూలం_gid;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం ret;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free అక్షరం *బఫరు = ఖాళీ;

      ret = read_all_file (CGROUP_మూలం "/unified/cgroup.controllers", &బఫరు, &len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      ఒకవేళ (len > 0)
        తిరిగి crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (వాదనలు->manager, &cgroup_manager, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  స్థితి_2->manager = వాదనలు->manager;

  ret = cgroup_manager->create_cgroup (వాదనలు, స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ఖాళీ;
      పూర్ణాంకం ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (వాదనలు, cgroup_బహుళకం, &tmp_err);
      ఒకవేళ (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          తిరిగి ignore_cgroup_errors;
        }

      ఒకవేళ (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          విడుదల (స్థితి_2->మార్గం);
          విడుదల (స్థితి_2->scope);
          స్థితి_2->మార్గం = ఖాళీ;
          స్థితి_2->scope = ఖాళీ;
          స్థితి_2->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          వెళ్ళు success;
        }

      తిరిగి ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  విడుదల (వాదనలు->precreated_మార్గం);
  వాదనలు->precreated_మార్గం = ఖాళీ;

  ఒకవేళ (స్థితి_2->మార్గం)
    {
      bool need_chown;

      need_chown = మూలం_uid != (uid_t) -1 || మూలం_gid != (gid_t) -1;
      ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED && need_chown)
        {
          ret = chown_cgroups (స్థితి_2->మార్గం, మూలం_uid, మూలం_gid, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            వెళ్ళు fail_destroy;
        }

      ఒకవేళ (వాదనలు->resources)
        {
          ret = update_cgroup_resources (స్థితి_2->మార్గం, వాదనలు->state_మూలం, వాదనలు->resources, ! స్థితి_2->bpf_dev_సమితి, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            వెళ్ళు fail_destroy;
        }
    }
success:
  *బహిర్గతం = స్థితి_2;
  స్థితి_2 = ఖాళీ;
  తిరిగి 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = ఖాళీ;

    ఒకవేళ (UNLIKELY (cgroup_manager->destroy_cgroup (స్థితి_2, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  తిరిగి ret;
}

పూర్ణాంకం
libcrun_cgroup_enter_finalize (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి arg_వాడని, libcrun_error_t *err)
{
  cleanup_free అక్షరం *లక్ష్యం_cgroup = ఖాళీ;
  cleanup_free అక్షరం *content = ఖాళీ;
  స్థిరరాశి అక్షరం *delegate_cgroup;
  cleanup_free అక్షరం *dir = ఖాళీ;
  అక్షరం *current_cgroup, *గమ్యం;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం ret;

  delegate_cgroup = కనుగొను_delegate_cgroup (వాదనలు->annotations);
  ఒకవేళ (delegate_cgroup == ఖాళీ)
    తిరిగి 0;

  ఒకవేళ (మార్గం_has_dot_dot_component (delegate_cgroup))
    తిరిగి crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (cgroup_బహుళకం != CGROUP_బహుళకం_UNIFIED)
    తిరిగి crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free అక్షరం *proc_మార్గం = ఖాళీ;
  cleanup_close పూర్ణాంకం fd = -1;

  xasprintf (&proc_మార్గం, "%d/cgroup", వాదనలు->pid);
  fd = libcrun_open_proc_file (వాదనలు->container, proc_మార్గం, O_RDONLY, err);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి fd;

  ret = read_all_fd (fd, "cgroup path", &content, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  current_cgroup = అక్షరశ్రేణి_శోధన (content, "0::");
  ఒకవేళ (UNLIKELY (current_cgroup == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  గమ్యం = strchr (current_cgroup, '\n');
  ఒకవేళ (UNLIKELY (గమ్యం == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *గమ్యం = '\0';

  ret = append_paths (&లక్ష్యం_cgroup, err, current_cgroup, delegate_cgroup, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_paths (&dir, err, CGROUP_మూలం, లక్ష్యం_cgroup, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = move_process_గమ్యం_cgroup (వాదనలు->pid, ఖాళీ, లక్ష్యం_cgroup, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = enable_controllers (లక్ష్యం_cgroup, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = chown_cgroups (లక్ష్యం_cgroup, వాదనలు->మూలం_uid, వాదనలు->మూలం_gid, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_cgroup_has_oom (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, libcrun_error_t *err)
{
  cleanup_free అక్షరం *content = ఖాళీ;
  స్థిరరాశి అక్షరం *మార్గం = స్థితి_2->మార్గం;
  స్థిరరాశి అక్షరం *ఉపసర్గ = ఖాళీ;
  size_t content_కొలత = 0;
  పూర్ణాంకం cgroup_బహుళకం;
  అక్షరం *it;

  ఒకవేళ (UNLIKELY (మార్గం == ఖాళీ || మార్గం[0] == '\0'))
    తిరిగి 0;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఎంపిక (cgroup_బహుళకం)
    {
    స్థితి CGROUP_బహుళకం_UNIFIED:
      {
        cleanup_free అక్షరం *events_మార్గం = ఖాళీ;
        పూర్ణాంకం ret;

        ret = append_paths (&events_మార్గం, err, CGROUP_మూలం, మార్గం, "memory.events", ఖాళీ);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_మార్గం, &content, &content_కొలత, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        ఉపసర్గ = "oom ";
        ఆపు;
      }
    స్థితి CGROUP_బహుళకం_LEGACY:
    స్థితి CGROUP_బహుళకం_HYBRID:
      {
        cleanup_free అక్షరం *oom_control_మార్గం = ఖాళీ;
        పూర్ణాంకం ret;

        ret = append_paths (&oom_control_మార్గం, err, CGROUP_మూలం, "memory", మార్గం, "memory.oom_control", ఖాళీ);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_మార్గం, &content, &content_కొలత, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        ఉపసర్గ = "oom_kill ";
        ఆపు;
      }

    అప్రమేయ:
      తిరిగి crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_బహుళకం);
    }

  it = content;
  అయితే (it && *it)
    {
      ఒకవేళ (has_ఉపసర్గ (it, ఉపసర్గ))
        {
          it += అక్షరశ్రేణి_పొడవు (ఉపసర్గ);
          అయితే (*it == ' ')
            it++;

          తిరిగి *it != '0';
        }
      లేకపోతే
        {
          it = strchr (it, '\n');
          ఒకవేళ (it == ఖాళీ)
            తిరిగి 0;
          it++;
        }
    }

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_cgroup_get_స్థితి (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి,
                           libcrun_container_స్థితి_t *స్థితి_2,
                           libcrun_error_t *err arg_వాడని)
{
  స్థితి_2->cgroup_మార్గం = cgroup_స్థితి->మార్గం;
  స్థితి_2->scope = cgroup_స్థితి->scope;
  తిరిగి 0;
}

శూన్య
libcrun_cgroup_స్థితి_free (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి)
{
  ఒకవేళ (cgroup_స్థితి == ఖాళీ)
    తిరిగి;

  విడుదల (cgroup_స్థితి->మార్గం);
  విడుదల (cgroup_స్థితి->scope);
  విడుదల (cgroup_స్థితి);
}

నిర్మాణం libcrun_cgroup_స్థితి *
libcrun_cgroup_make_స్థితి (libcrun_container_స్థితి_t *స్థితి_2)
{
  నిర్మాణం libcrun_cgroup_స్థితి *ret;

  ret = xmalloc0 (పరిమాణం *ret);

  ఒకవేళ (! is_ఖాళీలు_string (స్థితి_2->cgroup_మార్గం))
    ret->మార్గం = xstrdup (స్థితి_2->cgroup_మార్గం);

  ఒకవేళ (! is_ఖాళీలు_string (స్థితి_2->scope))
    ret->scope = xstrdup (స్థితి_2->scope);

  ఒకవేళ (is_ఖాళీలు_string (స్థితి_2->cgroup_మార్గం) && is_ఖాళీలు_string (స్థితి_2->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  లేకపోతే
    ret->manager = స్థితి_2->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  తిరిగి ret;
}
