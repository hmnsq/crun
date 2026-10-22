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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-cgroupfs.h"
#ಅನ್ತರ್ಭಾವ "cgroup-internal.h"
#ಅನ್ತರ್ಭಾವ "cgroup-systemd.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"
#ಅನ್ತರ್ಭಾವ "cgroup-setup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-resources.h"
#ಅನ್ತರ್ಭಾವ "ebpf.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ <linux/magic.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_enter_disabled (ಸಂರಚನಾ libcrun_cgroup_args *args arg_unused, ಸಂರಚನಾ libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_destroy_cgroup_disabled (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = ರಿಕ್ತ,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_cgroup_manager (ಪೂರ್ಣಾಙ್ಕ manager, ಸಂರಚನಾ libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  ವಿಕಲ್ಪ (manager)
    {
    ಪಕ್ಷ CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      ಪ್ರತ್ಯಾಗಮ 0;

    ಪಕ್ಷ CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      ಪ್ರತ್ಯಾಗಮ 0;

    ಪಕ್ಷ CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  *out = ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
find_delegate_cgroup (string_map *annotations)
{
  ಧ್ರುವ ವರ್ಣ *annotation;

  annotation = find_string_map_ಮೌಲ್ಯ (annotations, "run.oci.delegate-cgroup");
  ಯದಿ (annotation)
    {
      ಯದಿ (annotation[0] == '\0')
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
      ಪ್ರತ್ಯಾಗಮ annotation;
    }

  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_pause_unpause (ಸಂರಚನಾ libcrun_cgroup_status *status, ಧ್ರುವ bool pause, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_is_container_paused (ಸಂರಚನಾ libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *cgroup_path = status->path;
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *state;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (cgroup_path == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_ಬೇರು, cgroup_path, "cgroup.freeze", ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_ಬೇರು "/freezer", cgroup_path, "freezer.state", ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = read_all_file (path, &content, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      ಯದಿ (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        ಪ್ರತ್ಯಾಗಮ ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      ಸಂರಚನಾ statfs freezer_stat;
      ಯದಿ (statfs (CGROUP_ಬೇರು "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ಬೇರು "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      ಯದಿ (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        ಪ್ರತ್ಯಾಗಮ ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  *paused = strstr (content, state) != ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_join_process (ಸಂರಚನಾ libcrun_cgroup_status *status, ಧ್ರುವ ವರ್ಣ *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  ಸಂರಚನಾ libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ errno_;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  ಯದಿ (LIKELY (ret >= 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  ಯದಿ (errno_ != EACCES && errno_ != EPERM)
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (cgroup_manager->attach_process == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->ಸಂದೇಶ);
      crun_error_release (&tmp_err);
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  crun_error_release (err);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_read_pids (ಸಂರಚನಾ libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_killall (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status, ಪೂರ್ಣಾಙ್ಕ ಸಙ್ಕೇತ, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ cgroup_killall_path (cgroup_status->path, ಸಙ್ಕೇತ, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_destroy (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  ಸಂರಚನಾ libcrun_cgroup_manager *cgroup_manager = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
complete_cpu_max (ಧ್ರುವ ವರ್ಣ *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free ವರ್ಣ *cpu_max_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  bool has_period;
  uint64_t period;
  int64_t quota;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (resources == ರಿಕ್ತ || resources->cpu == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  cpu = resources->cpu;
  ಯದಿ (cpu->quota_present == cpu->period_present)
    ಪ್ರತ್ಯಾಗಮ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;
  ಯದಿ (cgroup_mode != CGROUP_MODE_UNIFIED)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_ಬೇರು, path, "cpu.max", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = read_all_file (cpu_max_path, &content, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  ಅನ್ಯಥಾ ಯದಿ (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_update_cgroup_resources (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status,
                                 ಧ್ರುವ ವರ್ಣ *state_ಬೇರು,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  ಸಂರಚನಾ libcrun_cgroup_manager *cgroup_manager = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_ಬೇರು, resources, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ update_cgroup_resources (cgroup_status->path, state_ಬೇರು, resources, ! cgroup_status->bpf_dev_set, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
can_ignore_cgroup_enter_errors (ಸಂರಚನಾ libcrun_cgroup_args *args, ಪೂರ್ಣಾಙ್ಕ cgroup_mode,
                                libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ manager = args->manager;
  ಪೂರ್ಣಾಙ್ಕ rootless;

  rootless = is_rootless (err);
  ಯದಿ (UNLIKELY (rootless < 0))
    ಪ್ರತ್ಯಾಗಮ rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  ಯದಿ (! rootless)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (! is_empty_string (args->cgroup_path))
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ 1;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_preenter (ಸಂರಚನಾ libcrun_cgroup_args *args, ಪೂರ್ಣಾಙ್ಕ *dirfd, libcrun_error_t *err)
{
  ಸಂರಚನಾ libcrun_cgroup_manager *cgroup_manager;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (cgroup_mode != CGROUP_MODE_UNIFIED)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (cgroup_manager->precreate_cgroup == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
ಶೂನ್ಯ
libcrun_cgroup_preenter_cleanup (ಸಂರಚನಾ libcrun_cgroup_args *args)
{
  ಯದಿ (args->precreated_path == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  (ಶೂನ್ಯ) rmdir (args->precreated_path);
  ಮುಕ್ತ (args->precreated_path);
  args->precreated_path = ರಿಕ್ತ;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_enter (ಸಂರಚನಾ libcrun_cgroup_args *args, ಸಂರಚನಾ libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *status = xmalloc0 (ಪರಿಮಾಣ *status);
  ಸಂರಚನಾ libcrun_cgroup_manager *cgroup_manager;
  uid_t ಬೇರು_uid = args->ಬೇರು_uid;
  uid_t ಬೇರು_gid = args->ಬೇರು_gid;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;

      ret = read_all_file (CGROUP_ಬೇರು "/unified/cgroup.controllers", &buffer, &len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      ಯದಿ (len > 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      ಯದಿ (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ಪ್ರತ್ಯಾಗಮ ignore_cgroup_errors;
        }

      ಯದಿ (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          ಮುಕ್ತ (status->path);
          ಮುಕ್ತ (status->scope);
          status->path = ರಿಕ್ತ;
          status->scope = ರಿಕ್ತ;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          ಗಚ್ಛ success;
        }

      ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  ಮುಕ್ತ (args->precreated_path);
  args->precreated_path = ರಿಕ್ತ;

  ಯದಿ (status->path)
    {
      bool need_chown;

      need_chown = ಬೇರು_uid != (uid_t) -1 || ಬೇರು_gid != (gid_t) -1;
      ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, ಬೇರು_uid, ಬೇರು_gid, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಗಚ್ಛ fail_destroy;
        }

      ಯದಿ (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_ಬೇರು, args->resources, ! status->bpf_dev_set, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಗಚ್ಛ fail_destroy;
        }
    }
success:
  *out = status;
  status = ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = ರಿಕ್ತ;

    ಯದಿ (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_enter_finalize (ಸಂರಚನಾ libcrun_cgroup_args *args, ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *target_cgroup = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *delegate_cgroup;
  cleanup_free ವರ್ಣ *dir = ರಿಕ್ತ;
  ವರ್ಣ *current_cgroup, *to;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  ಯದಿ (delegate_cgroup == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (path_has_dot_dot_component (delegate_cgroup))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (cgroup_mode != CGROUP_MODE_UNIFIED)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free ವರ್ಣ *proc_path = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ fd;

  ret = read_all_fd (fd, "cgroup path", &content, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  current_cgroup = strstr (content, "0::");
  ಯದಿ (UNLIKELY (current_cgroup == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  ಯದಿ (UNLIKELY (to == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_paths (&dir, err, CGROUP_ಬೇರು, target_cgroup, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = move_process_to_cgroup (args->pid, ರಿಕ್ತ, target_cgroup, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = enable_controllers (target_cgroup, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = chown_cgroups (target_cgroup, args->ಬೇರು_uid, args->ಬೇರು_gid, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_has_oom (ಸಂರಚನಾ libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *path = status->path;
  ಧ್ರುವ ವರ್ಣ *prefix = ರಿಕ್ತ;
  size_t content_size = 0;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ವರ್ಣ *it;

  ಯದಿ (UNLIKELY (path == ರಿಕ್ತ || path[0] == '\0'))
    ಪ್ರತ್ಯಾಗಮ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ವಿಕಲ್ಪ (cgroup_mode)
    {
    ಪಕ್ಷ CGROUP_MODE_UNIFIED:
      {
        cleanup_free ವರ್ಣ *events_path = ರಿಕ್ತ;
        ಪೂರ್ಣಾಙ್ಕ ret;

        ret = append_paths (&events_path, err, CGROUP_ಬೇರು, path, "memory.events", ರಿಕ್ತ);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        prefix = "oom ";
        ಭಙ್ಗ;
      }
    ಪಕ್ಷ CGROUP_MODE_LEGACY:
    ಪಕ್ಷ CGROUP_MODE_HYBRID:
      {
        cleanup_free ವರ್ಣ *oom_control_path = ರಿಕ್ತ;
        ಪೂರ್ಣಾಙ್ಕ ret;

        ret = append_paths (&oom_control_path, err, CGROUP_ಬೇರು, "memory", path, "memory.oom_control", ರಿಕ್ತ);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        prefix = "oom_kill ";
        ಭಙ್ಗ;
      }

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  ಯಾವತ್ (it && *it)
    {
      ಯದಿ (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          ಯಾವತ್ (*it == ' ')
            it++;

          ಪ್ರತ್ಯಾಗಮ *it != '0';
        }
      ಅನ್ಯಥಾ
        {
          it = strchr (it, '\n');
          ಯದಿ (it == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ 0;
          it++;
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_get_status (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ
libcrun_cgroup_status_free (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status)
{
  ಯದಿ (cgroup_status == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ಮುಕ್ತ (cgroup_status->path);
  ಮುಕ್ತ (cgroup_status->scope);
  ಮುಕ್ತ (cgroup_status);
}

ಸಂರಚನಾ libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  ಸಂರಚನಾ libcrun_cgroup_status *ret;

  ret = xmalloc0 (ಪರಿಮಾಣ *ret);

  ಯದಿ (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  ಯದಿ (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  ಯದಿ (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  ಅನ್ಯಥಾ
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  ಪ್ರತ್ಯಾಗಮ ret;
}
