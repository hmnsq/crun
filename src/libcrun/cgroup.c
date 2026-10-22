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

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_cgroup_enter_disabled (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು arg_ಬಳಸದ, ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಹೊರಹೋಗು, libcrun_error_t *err arg_ಬಳಸದ)
{
  ಹೊರಹೋಗು->ಮಾರ್ಗ = ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_destroy_cgroup_disabled (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ arg_ಬಳಸದ,
                                 libcrun_error_t *err arg_ಬಳಸದ)
{
  ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = ಖಾಲಿ,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_cgroup_manager (ಪೂರ್ಣಾಂಕ manager, ರಚನೆ libcrun_cgroup_manager **ಹೊರಹೋಗು, libcrun_error_t *err)
{
  ಆಯ್ಕೆ (manager)
    {
    ಸ್ಥಿತಿ CGROUP_MANAGER_DISABLED:
      *ಹೊರಹೋಗು = &cgroup_manager_disabled;
      ಹಿಂತಿರುಗಿ 0;

    ಸ್ಥಿತಿ CGROUP_MANAGER_SYSTEMD:
      *ಹೊರಹೋಗು = &cgroup_manager_systemd;
      ಹಿಂತಿರುಗಿ 0;

    ಸ್ಥಿತಿ CGROUP_MANAGER_CGROUPFS:
      *ಹೊರಹೋಗು = &cgroup_manager_cgroupfs;
      ಹಿಂತಿರುಗಿ 0;
    }

  *ಹೊರಹೋಗು = ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
ಕಂಡುಹಿಡಿ_delegate_cgroup (string_map *annotations)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *annotation;

  annotation = ಕಂಡುಹಿಡಿ_string_map_ಮೌಲ್ಯ (annotations, "run.oci.delegate-cgroup");
  ಆದರೆ (annotation)
    {
      ಆದರೆ (annotation[0] == '\0')
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
      ಹಿಂತಿರುಗಿ annotation;
    }

  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_pause_unpause (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ bool pause, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ libcrun_cgroup_pause_unpause_ಮಾರ್ಗ (ಸ್ಥಿತಿ_2->ಮಾರ್ಗ, pause, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_is_container_paused (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, bool *paused, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಸ್ಥಿತಿ_2->ಮಾರ್ಗ;
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (cgroup_ಮಾರ್ಗ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
    {
      state = "1";

      ret = append_paths (&ಮಾರ್ಗ, err, CGROUP_ಬೇರು, cgroup_ಮಾರ್ಗ, "cgroup.freeze", ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      state = "FROZEN";

      ret = append_paths (&ಮಾರ್ಗ, err, CGROUP_ಬೇರು "/freezer", cgroup_ಮಾರ್ಗ, "freezer.state", ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = read_all_file (ಮಾರ್ಗ, &content, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      ಆದರೆ (errno != ENOENT || cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
        ಹಿಂತಿರುಗಿ ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      ರಚನೆ statfs freezer_stat;
      ಆದರೆ (statfs (CGROUP_ಬೇರು "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ಬೇರು "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      ಆದರೆ (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        ಹಿಂತಿರುಗಿ ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      ಹಿಂತಿರುಗಿ 0;
    }

  *paused = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (content, state) != ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_join_process (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  ರಚನೆ libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ errno_;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_move_process_ಗಮ್ಯ_cgroup (pid, init_pid, ಮಾರ್ಗ, false, err);
  ಆದರೆ (LIKELY (ret >= 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  ಆದರೆ (errno_ != EACCES && errno_ != EPERM)
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (get_cgroup_manager (ಸ್ಥಿತಿ_2->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (cgroup_manager->attach_process == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (UNLIKELY (cgroup_manager->attach_process (ಸ್ಥಿತಿ_2, ಮಾರ್ಗ, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, ಮಾರ್ಗ, tmp_err->ಸಂದೇಶ);
      crun_error_release (&tmp_err);
      ಹಿಂತಿರುಗಿ ret;
    }

  crun_error_release (err);
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_read_pids (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ libcrun_cgroup_read_pids_from_ಮಾರ್ಗ (ಸ್ಥಿತಿ_2->ಮಾರ್ಗ, recurse, pids, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_killall (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ, ಪೂರ್ಣಾಂಕ ಸಂಕೇತ, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ cgroup_killall_ಮಾರ್ಗ (cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ, ಸಂಕೇತ, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_destroy (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ, libcrun_error_t *err)
{
  ರಚನೆ libcrun_cgroup_manager *cgroup_manager = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = get_cgroup_manager (cgroup_ಸ್ಥಿತಿ->manager, &cgroup_manager, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ cgroup_manager->destroy_cgroup (cgroup_ಸ್ಥಿತಿ, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
complete_cpu_max (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free ಅಕ್ಷರ *cpu_max_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  bool has_period;
  uint64_t period;
  int64_t quota;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (resources == ಖಾಲಿ || resources->cpu == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  cpu = resources->cpu;
  ಆದರೆ (cpu->quota_present == cpu->period_present)
    ಹಿಂತಿರುಗಿ 0;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;
  ಆದರೆ (cgroup_ಬಹುಲಕ != CGROUP_ಬಹುಲಕ_UNIFIED)
    ಹಿಂತಿರುಗಿ 0;

  ret = append_paths (&cpu_max_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, "cpu.max", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = read_all_file (cpu_max_ಮಾರ್ಗ, &content, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      ಹಿಂತಿರುಗಿ 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_update_cgroup_resources (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ,
                                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  ರಚನೆ libcrun_cgroup_manager *cgroup_manager = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = get_cgroup_manager (cgroup_ಸ್ಥಿತಿ->manager, &cgroup_manager, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = check_ಸ್ಮರಣೆ_before_update (cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ, resources, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = complete_cpu_max (cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ, resources, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_ಸ್ಥಿತಿ, state_ಬೇರು, resources, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ update_cgroup_resources (cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ, state_ಬೇರು, resources, ! cgroup_ಸ್ಥಿತಿ->bpf_dev_ಗಣ, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
can_ignore_cgroup_enter_errors (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ,
                                libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ manager = ವಾದಗಳು->manager;
  ಪೂರ್ಣಾಂಕ rootless;

  rootless = is_rootless (err);
  ಆದರೆ (UNLIKELY (rootless < 0))
    ಹಿಂತಿರುಗಿ rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  ಆದರೆ (! rootless)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (! is_ಖಾಲಿಗಳು_string (ವಾದಗಳು->cgroup_ಮಾರ್ಗ))
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ 1;
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_preenter (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ಪೂರ್ಣಾಂಕ *dirfd, libcrun_error_t *err)
{
  ರಚನೆ libcrun_cgroup_manager *cgroup_manager;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  *dirfd = -1;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (cgroup_ಬಹುಲಕ != CGROUP_ಬಹುಲಕ_UNIFIED)
    ಹಿಂತಿರುಗಿ 0;

  ret = get_cgroup_manager (ವಾದಗಳು->manager, &cgroup_manager, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (cgroup_manager->precreate_cgroup == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ cgroup_manager->precreate_cgroup (ವಾದಗಳು, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
ಶೂನ್ಯ
libcrun_cgroup_preenter_cleanup (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು)
{
  ಆದರೆ (ವಾದಗಳು->precreated_ಮಾರ್ಗ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  (ಶೂನ್ಯ) rmdir (ವಾದಗಳು->precreated_ಮಾರ್ಗ);
  ಬಿಡುಗಡೆ (ವಾದಗಳು->precreated_ಮಾರ್ಗ);
  ವಾದಗಳು->precreated_ಮಾರ್ಗ = ಖಾಲಿ;
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_enter (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ **ಹೊರಹೋಗು, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2 = xmalloc0 (ಗಾತ್ರ *ಸ್ಥಿತಿ_2);
  ರಚನೆ libcrun_cgroup_manager *cgroup_manager;
  uid_t ಬೇರು_uid = ವಾದಗಳು->ಬೇರು_uid;
  uid_t ಬೇರು_gid = ವಾದಗಳು->ಬೇರು_gid;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;

      ret = read_all_file (CGROUP_ಬೇರು "/unified/cgroup.controllers", &ಬಫರ್, &len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      ಆದರೆ (len > 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (ವಾದಗಳು->manager, &cgroup_manager, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಸ್ಥಿತಿ_2->manager = ವಾದಗಳು->manager;

  ret = cgroup_manager->create_cgroup (ವಾದಗಳು, ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (ವಾದಗಳು, cgroup_ಬಹುಲಕ, &tmp_err);
      ಆದರೆ (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ಹಿಂತಿರುಗಿ ignore_cgroup_errors;
        }

      ಆದರೆ (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          ಬಿಡುಗಡೆ (ಸ್ಥಿತಿ_2->ಮಾರ್ಗ);
          ಬಿಡುಗಡೆ (ಸ್ಥಿತಿ_2->scope);
          ಸ್ಥಿತಿ_2->ಮಾರ್ಗ = ಖಾಲಿ;
          ಸ್ಥಿತಿ_2->scope = ಖಾಲಿ;
          ಸ್ಥಿತಿ_2->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          ಹೋಗು success;
        }

      ಹಿಂತಿರುಗಿ ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  ಬಿಡುಗಡೆ (ವಾದಗಳು->precreated_ಮಾರ್ಗ);
  ವಾದಗಳು->precreated_ಮಾರ್ಗ = ಖಾಲಿ;

  ಆದರೆ (ಸ್ಥಿತಿ_2->ಮಾರ್ಗ)
    {
      bool need_chown;

      need_chown = ಬೇರು_uid != (uid_t) -1 || ಬೇರು_gid != (gid_t) -1;
      ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED && need_chown)
        {
          ret = chown_cgroups (ಸ್ಥಿತಿ_2->ಮಾರ್ಗ, ಬೇರು_uid, ಬೇರು_gid, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹೋಗು fail_destroy;
        }

      ಆದರೆ (ವಾದಗಳು->resources)
        {
          ret = update_cgroup_resources (ಸ್ಥಿತಿ_2->ಮಾರ್ಗ, ವಾದಗಳು->state_ಬೇರು, ವಾದಗಳು->resources, ! ಸ್ಥಿತಿ_2->bpf_dev_ಗಣ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹೋಗು fail_destroy;
        }
    }
success:
  *ಹೊರಹೋಗು = ಸ್ಥಿತಿ_2;
  ಸ್ಥಿತಿ_2 = ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = ಖಾಲಿ;

    ಆದರೆ (UNLIKELY (cgroup_manager->destroy_cgroup (ಸ್ಥಿತಿ_2, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_enter_finalize (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ arg_ಬಳಸದ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಗುರಿ_cgroup = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *delegate_cgroup;
  cleanup_free ಅಕ್ಷರ *dir = ಖಾಲಿ;
  ಅಕ್ಷರ *current_cgroup, *ಗಮ್ಯ;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  delegate_cgroup = ಕಂಡುಹಿಡಿ_delegate_cgroup (ವಾದಗಳು->annotations);
  ಆದರೆ (delegate_cgroup == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (ಮಾರ್ಗ_has_dot_dot_component (delegate_cgroup))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (cgroup_ಬಹುಲಕ != CGROUP_ಬಹುಲಕ_UNIFIED)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free ಅಕ್ಷರ *proc_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

  xasprintf (&proc_ಮಾರ್ಗ, "%d/cgroup", ವಾದಗಳು->pid);
  fd = libcrun_open_proc_file (ವಾದಗಳು->container, proc_ಮಾರ್ಗ, O_RDONLY, err);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ fd;

  ret = read_all_fd (fd, "cgroup path", &content, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  current_cgroup = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (content, "0::");
  ಆದರೆ (UNLIKELY (current_cgroup == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  ಗಮ್ಯ = strchr (current_cgroup, '\n');
  ಆದರೆ (UNLIKELY (ಗಮ್ಯ == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *ಗಮ್ಯ = '\0';

  ret = append_paths (&ಗುರಿ_cgroup, err, current_cgroup, delegate_cgroup, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_paths (&dir, err, CGROUP_ಬೇರು, ಗುರಿ_cgroup, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = move_process_ಗಮ್ಯ_cgroup (ವಾದಗಳು->pid, ಖಾಲಿ, ಗುರಿ_cgroup, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = enable_controllers (ಗುರಿ_cgroup, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = chown_cgroups (ಗುರಿ_cgroup, ವಾದಗಳು->ಬೇರು_uid, ವಾದಗಳು->ಬೇರು_gid, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_has_oom (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = ಸ್ಥಿತಿ_2->ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = ಖಾಲಿ;
  size_t content_ಗಾತ್ರ = 0;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಅಕ್ಷರ *it;

  ಆದರೆ (UNLIKELY (ಮಾರ್ಗ == ಖಾಲಿ || ಮಾರ್ಗ[0] == '\0'))
    ಹಿಂತಿರುಗಿ 0;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆಯ್ಕೆ (cgroup_ಬಹುಲಕ)
    {
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_UNIFIED:
      {
        cleanup_free ಅಕ್ಷರ *events_ಮಾರ್ಗ = ಖಾಲಿ;
        ಪೂರ್ಣಾಂಕ ret;

        ret = append_paths (&events_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, "memory.events", ಖಾಲಿ);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_ಮಾರ್ಗ, &content, &content_ಗಾತ್ರ, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        ಪೂರ್ವಪ್ರತ್ಯಯ = "oom ";
        ನಿಲ್ಲಿಸು;
      }
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_LEGACY:
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_HYBRID:
      {
        cleanup_free ಅಕ್ಷರ *oom_control_ಮಾರ್ಗ = ಖಾಲಿ;
        ಪೂರ್ಣಾಂಕ ret;

        ret = append_paths (&oom_control_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, "memory", ಮಾರ್ಗ, "memory.oom_control", ಖಾಲಿ);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_ಮಾರ್ಗ, &content, &content_ಗಾತ್ರ, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        ಪೂರ್ವಪ್ರತ್ಯಯ = "oom_kill ";
        ನಿಲ್ಲಿಸು;
      }

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_ಬಹುಲಕ);
    }

  it = content;
  ಆಗ (it && *it)
    {
      ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (it, ಪೂರ್ವಪ್ರತ್ಯಯ))
        {
          it += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಪೂರ್ವಪ್ರತ್ಯಯ);
          ಆಗ (*it == ' ')
            it++;

          ಹಿಂತಿರುಗಿ *it != '0';
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          it = strchr (it, '\n');
          ಆದರೆ (it == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ 0;
          it++;
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_get_ಸ್ಥಿತಿ (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ,
                           libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2,
                           libcrun_error_t *err arg_ಬಳಸದ)
{
  ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ = cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ;
  ಸ್ಥಿತಿ_2->scope = cgroup_ಸ್ಥಿತಿ->scope;
  ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ
libcrun_cgroup_ಸ್ಥಿತಿ_free (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ)
{
  ಆದರೆ (cgroup_ಸ್ಥಿತಿ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ಬಿಡುಗಡೆ (cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ);
  ಬಿಡುಗಡೆ (cgroup_ಸ್ಥಿತಿ->scope);
  ಬಿಡುಗಡೆ (cgroup_ಸ್ಥಿತಿ);
}

ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *
libcrun_cgroup_make_ಸ್ಥಿತಿ (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2)
{
  ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ret;

  ret = xmalloc0 (ಗಾತ್ರ *ret);

  ಆದರೆ (! is_ಖಾಲಿಗಳು_string (ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ))
    ret->ಮಾರ್ಗ = xstrdup (ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ);

  ಆದರೆ (! is_ಖಾಲಿಗಳು_string (ಸ್ಥಿತಿ_2->scope))
    ret->scope = xstrdup (ಸ್ಥಿತಿ_2->scope);

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ) && is_ಖಾಲಿಗಳು_string (ಸ್ಥಿತಿ_2->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  ಇಲ್ಲದಿದ್ದರೆ
    ret->manager = ಸ್ಥಿತಿ_2->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  ಹಿಂತಿರುಗಿ ret;
}
