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

#परिभाषय _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव "cgroup-cgroupfs.h"
#अन्तर्भाव "cgroup-internal.h"
#अन्तर्भाव "cgroup-systemd.h"
#अन्तर्भाव "cgroup-utils.h"
#अन्तर्भाव "cgroup-setup.h"
#अन्तर्भाव "cgroup-resources.h"
#अन्तर्भाव "ebpf.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "status.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव <linux/magic.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <libgen.h>

स्थिर पूर्णाङ्क
libcrun_cgroup_enter_disabled (संरचना libcrun_cgroup_तर्काः *तर्काः arg_अप्रयुक्तम्, संरचना libcrun_cgroup_स्थितिः *बहिर्गमनम्, libcrun_error_t *err arg_अप्रयुक्तम्)
{
  बहिर्गमनम्->मार्गः = रिक्त;
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libcrun_destroy_cgroup_disabled (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः arg_अप्रयुक्तम्,
                                 libcrun_error_t *err arg_अप्रयुक्तम्)
{
  प्रत्यागम 0;
}

संरचना libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = रिक्त,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

स्थिर पूर्णाङ्क
get_cgroup_manager (पूर्णाङ्क manager, संरचना libcrun_cgroup_manager **बहिर्गमनम्, libcrun_error_t *err)
{
  विकल्प (manager)
    {
    पक्ष CGROUP_MANAGER_DISABLED:
      *बहिर्गमनम् = &cgroup_manager_disabled;
      प्रत्यागम 0;

    पक्ष CGROUP_MANAGER_SYSTEMD:
      *बहिर्गमनम् = &cgroup_manager_systemd;
      प्रत्यागम 0;

    पक्ष CGROUP_MANAGER_CGROUPFS:
      *बहिर्गमनम् = &cgroup_manager_cgroupfs;
      प्रत्यागम 0;
    }

  *बहिर्गमनम् = रिक्त;
  प्रत्यागम crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

स्थिर ध्रुव वर्ण *
find_delegate_cgroup (string_map *annotations)
{
  ध्रुव वर्ण *annotation;

  annotation = find_string_map_मानम् (annotations, "run.oci.delegate-cgroup");
  यदि (annotation)
    {
      यदि (annotation[0] == '\0')
        प्रत्यागम रिक्त;
      प्रत्यागम annotation;
    }

  प्रत्यागम रिक्त;
}

पूर्णाङ्क
libcrun_cgroup_pause_unpause (संरचना libcrun_cgroup_स्थितिः *स्थितिः, ध्रुव bool pause, libcrun_error_t *err)
{
  प्रत्यागम libcrun_cgroup_pause_unpause_मार्गः (स्थितिः->मार्गः, pause, err);
}

पूर्णाङ्क
libcrun_cgroup_is_container_paused (संरचना libcrun_cgroup_स्थितिः *स्थितिः, bool *paused, libcrun_error_t *err)
{
  ध्रुव वर्ण *cgroup_मार्गः = स्थितिः->मार्गः;
  cleanup_free वर्ण *content = रिक्त;
  cleanup_free वर्ण *मार्गः = रिक्त;
  ध्रुव वर्ण *state;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क ret;

  यदि (cgroup_मार्गः == रिक्त)
    प्रत्यागम 0;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
    {
      state = "1";

      ret = append_paths (&मार्गः, err, CGROUP_मूलम्, cgroup_मार्गः, "cgroup.freeze", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अन्यथा
    {
      state = "FROZEN";

      ret = append_paths (&मार्गः, err, CGROUP_मूलम् "/freezer", cgroup_मार्गः, "freezer.state", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = read_all_file (मार्गः, &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      यदि (errno != ENOENT || cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
        प्रत्यागम ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      संरचना statfs freezer_stat;
      यदि (statfs (CGROUP_मूलम् "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          प्रत्यागम crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_मूलम् "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      यदि (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        प्रत्यागम ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      प्रत्यागम 0;
    }

  *paused = वाक्ये_अन्विष (content, state) != रिक्त;
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_cgroup_join_process (संरचना libcrun_cgroup_स्थितिः *स्थितिः, ध्रुव वर्ण *मार्गः, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  संरचना libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क errno_;
  पूर्णाङ्क ret;

  ret = libcrun_move_process_गन्तव्यम्_cgroup (pid, init_pid, मार्गः, false, err);
  यदि (LIKELY (ret >= 0))
    प्रत्यागम ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  यदि (errno_ != EACCES && errno_ != EPERM)
    प्रत्यागम ret;

  यदि (get_cgroup_manager (स्थितिः->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      प्रत्यागम ret;
    }
  यदि (cgroup_manager->attach_process == रिक्त)
    प्रत्यागम ret;

  यदि (UNLIKELY (cgroup_manager->attach_process (स्थितिः, मार्गः, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, मार्गः, tmp_err->सन्देशः);
      crun_error_release (&tmp_err);
      प्रत्यागम ret;
    }

  crun_error_release (err);
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_cgroup_read_pids (संरचना libcrun_cgroup_स्थितिः *स्थितिः, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  प्रत्यागम libcrun_cgroup_read_pids_from_मार्गः (स्थितिः->मार्गः, recurse, pids, err);
}

पूर्णाङ्क
libcrun_cgroup_killall (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः, पूर्णाङ्क सङ्केत, libcrun_error_t *err)
{
  प्रत्यागम cgroup_killall_मार्गः (cgroup_स्थितिः->मार्गः, सङ्केत, err);
}

पूर्णाङ्क
libcrun_cgroup_destroy (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः, libcrun_error_t *err)
{
  संरचना libcrun_cgroup_manager *cgroup_manager = रिक्त;
  पूर्णाङ्क ret;

  ret = get_cgroup_manager (cgroup_स्थितिः->manager, &cgroup_manager, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम cgroup_manager->destroy_cgroup (cgroup_स्थितिः, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
स्थिर पूर्णाङ्क
complete_cpu_max (ध्रुव वर्ण *मार्गः, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free वर्ण *cpu_max_मार्गः = रिक्त;
  cleanup_free वर्ण *content = रिक्त;
  bool has_period;
  uint64_t period;
  int64_t quota;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क ret;

  यदि (resources == रिक्त || resources->cpu == रिक्त)
    प्रत्यागम 0;

  cpu = resources->cpu;
  यदि (cpu->quota_present == cpu->period_present)
    प्रत्यागम 0;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;
  यदि (cgroup_बहुलकम् != CGROUP_बहुलकम्_UNIFIED)
    प्रत्यागम 0;

  ret = append_paths (&cpu_max_मार्गः, err, CGROUP_मूलम्, मार्गः, "cpu.max", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file (cpu_max_मार्गः, &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      प्रत्यागम 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  अन्यथा यदि (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_update_cgroup_resources (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः,
                                 ध्रुव वर्ण *state_मूलम्,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  संरचना libcrun_cgroup_manager *cgroup_manager = रिक्त;
  पूर्णाङ्क ret;

  ret = get_cgroup_manager (cgroup_स्थितिः->manager, &cgroup_manager, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = check_स्मृतिः_before_update (cgroup_स्थितिः->मार्गः, resources, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = complete_cpu_max (cgroup_स्थितिः->मार्गः, resources, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_स्थितिः, state_मूलम्, resources, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम update_cgroup_resources (cgroup_स्थितिः->मार्गः, state_मूलम्, resources, ! cgroup_स्थितिः->bpf_dev_समुच्चयः, err);
}

स्थिर पूर्णाङ्क
can_ignore_cgroup_enter_errors (संरचना libcrun_cgroup_तर्काः *तर्काः, पूर्णाङ्क cgroup_बहुलकम्,
                                libcrun_error_t *err)
{
  पूर्णाङ्क manager = तर्काः->manager;
  पूर्णाङ्क rootless;

  rootless = is_rootless (err);
  यदि (UNLIKELY (rootless < 0))
    प्रत्यागम rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  यदि (! rootless)
    प्रत्यागम 0;

  यदि (! is_रिक्तानि_string (तर्काः->cgroup_मार्गः))
    प्रत्यागम 0;

  यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    प्रत्यागम 0;

  प्रत्यागम 1;
}

पूर्णाङ्क
libcrun_cgroup_preenter (संरचना libcrun_cgroup_तर्काः *तर्काः, पूर्णाङ्क *dirfd, libcrun_error_t *err)
{
  संरचना libcrun_cgroup_manager *cgroup_manager;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क ret;

  *dirfd = -1;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  यदि (cgroup_बहुलकम् != CGROUP_बहुलकम्_UNIFIED)
    प्रत्यागम 0;

  ret = get_cgroup_manager (तर्काः->manager, &cgroup_manager, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (cgroup_manager->precreate_cgroup == रिक्त)
    प्रत्यागम 0;

  प्रत्यागम cgroup_manager->precreate_cgroup (तर्काः, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
शून्य
libcrun_cgroup_preenter_cleanup (संरचना libcrun_cgroup_तर्काः *तर्काः)
{
  यदि (तर्काः->precreated_मार्गः == रिक्त)
    प्रत्यागम;

  (शून्य) rmdir (तर्काः->precreated_मार्गः);
  मुक्त (तर्काः->precreated_मार्गः);
  तर्काः->precreated_मार्गः = रिक्त;
}

पूर्णाङ्क
libcrun_cgroup_enter (संरचना libcrun_cgroup_तर्काः *तर्काः, संरचना libcrun_cgroup_स्थितिः **बहिर्गमनम्, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_स्थितिः संरचना libcrun_cgroup_स्थितिः *स्थितिः = xmalloc0 (परिमाण *स्थितिः);
  संरचना libcrun_cgroup_manager *cgroup_manager;
  uid_t मूलम्_uid = तर्काः->मूलम्_uid;
  uid_t मूलम्_gid = तर्काः->मूलम्_gid;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क ret;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free वर्ण *बफर = रिक्त;

      ret = read_all_file (CGROUP_मूलम् "/unified/cgroup.controllers", &बफर, &len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      यदि (len > 0)
        प्रत्यागम crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (तर्काः->manager, &cgroup_manager, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  स्थितिः->manager = तर्काः->manager;

  ret = cgroup_manager->create_cgroup (तर्काः, स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (तर्काः, cgroup_बहुलकम्, &tmp_err);
      यदि (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          प्रत्यागम ignore_cgroup_errors;
        }

      यदि (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          मुक्त (स्थितिः->मार्गः);
          मुक्त (स्थितिः->scope);
          स्थितिः->मार्गः = रिक्त;
          स्थितिः->scope = रिक्त;
          स्थितिः->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          गच्छ success;
        }

      प्रत्यागम ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  मुक्त (तर्काः->precreated_मार्गः);
  तर्काः->precreated_मार्गः = रिक्त;

  यदि (स्थितिः->मार्गः)
    {
      bool need_chown;

      need_chown = मूलम्_uid != (uid_t) -1 || मूलम्_gid != (gid_t) -1;
      यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED && need_chown)
        {
          ret = chown_cgroups (स्थितिः->मार्गः, मूलम्_uid, मूलम्_gid, err);
          यदि (UNLIKELY (ret < 0))
            गच्छ fail_destroy;
        }

      यदि (तर्काः->resources)
        {
          ret = update_cgroup_resources (स्थितिः->मार्गः, तर्काः->state_मूलम्, तर्काः->resources, ! स्थितिः->bpf_dev_समुच्चयः, err);
          यदि (UNLIKELY (ret < 0))
            गच्छ fail_destroy;
        }
    }
success:
  *बहिर्गमनम् = स्थितिः;
  स्थितिः = रिक्त;
  प्रत्यागम 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = रिक्त;

    यदि (UNLIKELY (cgroup_manager->destroy_cgroup (स्थितिः, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_cgroup_enter_finalize (संरचना libcrun_cgroup_तर्काः *तर्काः, संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः arg_अप्रयुक्तम्, libcrun_error_t *err)
{
  cleanup_free वर्ण *target_cgroup = रिक्त;
  cleanup_free वर्ण *content = रिक्त;
  ध्रुव वर्ण *delegate_cgroup;
  cleanup_free वर्ण *dir = रिक्त;
  वर्ण *current_cgroup, *गन्तव्यम्;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क ret;

  delegate_cgroup = find_delegate_cgroup (तर्काः->annotations);
  यदि (delegate_cgroup == रिक्त)
    प्रत्यागम 0;

  यदि (मार्गः_has_dot_dot_component (delegate_cgroup))
    प्रत्यागम crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  यदि (cgroup_बहुलकम् != CGROUP_बहुलकम्_UNIFIED)
    प्रत्यागम crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free वर्ण *proc_मार्गः = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;

  xasprintf (&proc_मार्गः, "%d/cgroup", तर्काः->pid);
  fd = libcrun_open_proc_file (तर्काः->container, proc_मार्गः, O_RDONLY, err);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम fd;

  ret = read_all_fd (fd, "cgroup path", &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  current_cgroup = वाक्ये_अन्विष (content, "0::");
  यदि (UNLIKELY (current_cgroup == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  गन्तव्यम् = strchr (current_cgroup, '\n');
  यदि (UNLIKELY (गन्तव्यम् == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *गन्तव्यम् = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (&dir, err, CGROUP_मूलम्, target_cgroup, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = move_process_गन्तव्यम्_cgroup (तर्काः->pid, रिक्त, target_cgroup, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = enable_controllers (target_cgroup, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = chown_cgroups (target_cgroup, तर्काः->मूलम्_uid, तर्काः->मूलम्_gid, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_cgroup_has_oom (संरचना libcrun_cgroup_स्थितिः *स्थितिः, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  ध्रुव वर्ण *मार्गः = स्थितिः->मार्गः;
  ध्रुव वर्ण *उपसर्गः = रिक्त;
  size_t content_आकार = 0;
  पूर्णाङ्क cgroup_बहुलकम्;
  वर्ण *it;

  यदि (UNLIKELY (मार्गः == रिक्त || मार्गः[0] == '\0'))
    प्रत्यागम 0;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  विकल्प (cgroup_बहुलकम्)
    {
    पक्ष CGROUP_बहुलकम्_UNIFIED:
      {
        cleanup_free वर्ण *events_मार्गः = रिक्त;
        पूर्णाङ्क ret;

        ret = append_paths (&events_मार्गः, err, CGROUP_मूलम्, मार्गः, "memory.events", रिक्त);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_मार्गः, &content, &content_आकार, err);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        उपसर्गः = "oom ";
        भङ्ग;
      }
    पक्ष CGROUP_बहुलकम्_LEGACY:
    पक्ष CGROUP_बहुलकम्_HYBRID:
      {
        cleanup_free वर्ण *oom_control_मार्गः = रिक्त;
        पूर्णाङ्क ret;

        ret = append_paths (&oom_control_मार्गः, err, CGROUP_मूलम्, "memory", मार्गः, "memory.oom_control", रिक्त);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_मार्गः, &content, &content_आकार, err);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        उपसर्गः = "oom_kill ";
        भङ्ग;
      }

    स्वभाव:
      प्रत्यागम crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_बहुलकम्);
    }

  it = content;
  यावत् (it && *it)
    {
      यदि (has_उपसर्गः (it, उपसर्गः))
        {
          it += वाक्यदैर्घ्यम् (उपसर्गः);
          यावत् (*it == ' ')
            it++;

          प्रत्यागम *it != '0';
        }
      अन्यथा
        {
          it = strchr (it, '\n');
          यदि (it == रिक्त)
            प्रत्यागम 0;
          it++;
        }
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_cgroup_get_स्थितिः (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः,
                           libcrun_container_स्थितिः_t *स्थितिः,
                           libcrun_error_t *err arg_अप्रयुक्तम्)
{
  स्थितिः->cgroup_मार्गः = cgroup_स्थितिः->मार्गः;
  स्थितिः->scope = cgroup_स्थितिः->scope;
  प्रत्यागम 0;
}

शून्य
libcrun_cgroup_स्थितिः_free (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः)
{
  यदि (cgroup_स्थितिः == रिक्त)
    प्रत्यागम;

  मुक्त (cgroup_स्थितिः->मार्गः);
  मुक्त (cgroup_स्थितिः->scope);
  मुक्त (cgroup_स्थितिः);
}

संरचना libcrun_cgroup_स्थितिः *
libcrun_cgroup_make_स्थितिः (libcrun_container_स्थितिः_t *स्थितिः)
{
  संरचना libcrun_cgroup_स्थितिः *ret;

  ret = xmalloc0 (परिमाण *ret);

  यदि (! is_रिक्तानि_string (स्थितिः->cgroup_मार्गः))
    ret->मार्गः = xstrdup (स्थितिः->cgroup_मार्गः);

  यदि (! is_रिक्तानि_string (स्थितिः->scope))
    ret->scope = xstrdup (स्थितिः->scope);

  यदि (is_रिक्तानि_string (स्थितिः->cgroup_मार्गः) && is_रिक्तानि_string (स्थितिः->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  अन्यथा
    ret->manager = स्थितिः->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  प्रत्यागम ret;
}
