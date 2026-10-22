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
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-cgroupfs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-systemd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-resources.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ebpf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/vfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/magic.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <libgen.h>

ਸਥਿਰ ਪੂਰਨ
libcrun_cgroup_enter_disabled (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ arg_ਅਣਵਰਤਿਆ, ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਬਾਹਰਲਾ, libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
  ਬਾਹਰਲਾ->ਰਸਤਾ = ਨਿਰਾ;
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libcrun_destroy_cgroup_disabled (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ arg_ਅਣਵਰਤਿਆ,
                                 libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ 0;
}

ਬਣਤਰ libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = ਨਿਰਾ,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

ਸਥਿਰ ਪੂਰਨ
get_cgroup_manager (ਪੂਰਨ manager, ਬਣਤਰ libcrun_cgroup_manager **ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  ਚੋਣ (manager)
    {
    ਹਾਲਤ CGROUP_MANAGER_DISABLED:
      *ਬਾਹਰਲਾ = &cgroup_manager_disabled;
      ਵਾਪਸ 0;

    ਹਾਲਤ CGROUP_MANAGER_SYSTEMD:
      *ਬਾਹਰਲਾ = &cgroup_manager_systemd;
      ਵਾਪਸ 0;

    ਹਾਲਤ CGROUP_MANAGER_CGROUPFS:
      *ਬਾਹਰਲਾ = &cgroup_manager_cgroupfs;
      ਵਾਪਸ 0;
    }

  *ਬਾਹਰਲਾ = ਨਿਰਾ;
  ਵਾਪਸ crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
ਲੱਭੋ_delegate_cgroup (string_map *annotations)
{
  ਅਚਲ ਅੱਖਰ *annotation;

  annotation = ਲੱਭੋ_string_map_ਮੁੱਲ (annotations, "run.oci.delegate-cgroup");
  ਜੇ (annotation)
    {
      ਜੇ (annotation[0] == '\0')
        ਵਾਪਸ ਨਿਰਾ;
      ਵਾਪਸ annotation;
    }

  ਵਾਪਸ ਨਿਰਾ;
}

ਪੂਰਨ
libcrun_cgroup_pause_unpause (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, ਅਚਲ bool pause, libcrun_error_t *err)
{
  ਵਾਪਸ libcrun_cgroup_pause_unpause_ਰਸਤਾ (ਸਥਿਤੀ->ਰਸਤਾ, pause, err);
}

ਪੂਰਨ
libcrun_cgroup_is_container_paused (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, bool *paused, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ = ਸਥਿਤੀ->ਰਸਤਾ;
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *state;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  ਜੇ (cgroup_ਰਸਤਾ == ਨਿਰਾ)
    ਵਾਪਸ 0;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
    {
      state = "1";

      ret = append_paths (&ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, cgroup_ਰਸਤਾ, "cgroup.freeze", ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      state = "FROZEN";

      ret = append_paths (&ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ "/freezer", cgroup_ਰਸਤਾ, "freezer.state", ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = read_all_file (ਰਸਤਾ, &content, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      ਜੇ (errno != ENOENT || cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
        ਵਾਪਸ ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      ਬਣਤਰ statfs freezer_stat;
      ਜੇ (statfs (CGROUP_ਜੜ੍ਹ "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          ਵਾਪਸ crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ਜੜ੍ਹ "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      ਜੇ (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        ਵਾਪਸ ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      ਵਾਪਸ 0;
    }

  *paused = ਲੜੀ_ਖੋਜ (content, state) != ਨਿਰਾ;
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_cgroup_join_process (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  ਬਣਤਰ libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = ਨਿਰਾ;
  ਪੂਰਨ errno_;
  ਪੂਰਨ ret;

  ret = libcrun_move_process_ਮੰਜ਼ਿਲ_cgroup (pid, init_pid, ਰਸਤਾ, false, err);
  ਜੇ (LIKELY (ret >= 0))
    ਵਾਪਸ ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  ਜੇ (errno_ != EACCES && errno_ != EPERM)
    ਵਾਪਸ ret;

  ਜੇ (get_cgroup_manager (ਸਥਿਤੀ->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      ਵਾਪਸ ret;
    }
  ਜੇ (cgroup_manager->attach_process == ਨਿਰਾ)
    ਵਾਪਸ ret;

  ਜੇ (UNLIKELY (cgroup_manager->attach_process (ਸਥਿਤੀ, ਰਸਤਾ, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, ਰਸਤਾ, tmp_err->ਸੁਨੇਹਾ);
      crun_error_release (&tmp_err);
      ਵਾਪਸ ret;
    }

  crun_error_release (err);
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_cgroup_read_pids (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ਵਾਪਸ libcrun_cgroup_read_pids_from_ਰਸਤਾ (ਸਥਿਤੀ->ਰਸਤਾ, recurse, pids, err);
}

ਪੂਰਨ
libcrun_cgroup_killall (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ, ਪੂਰਨ ਸੰਕੇਤ, libcrun_error_t *err)
{
  ਵਾਪਸ cgroup_killall_ਰਸਤਾ (cgroup_ਸਥਿਤੀ->ਰਸਤਾ, ਸੰਕੇਤ, err);
}

ਪੂਰਨ
libcrun_cgroup_destroy (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ, libcrun_error_t *err)
{
  ਬਣਤਰ libcrun_cgroup_manager *cgroup_manager = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = get_cgroup_manager (cgroup_ਸਥਿਤੀ->manager, &cgroup_manager, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ cgroup_manager->destroy_cgroup (cgroup_ਸਥਿਤੀ, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
ਸਥਿਰ ਪੂਰਨ
complete_cpu_max (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free ਅੱਖਰ *cpu_max_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  bool has_period;
  uint64_t period;
  int64_t quota;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  ਜੇ (resources == ਨਿਰਾ || resources->cpu == ਨਿਰਾ)
    ਵਾਪਸ 0;

  cpu = resources->cpu;
  ਜੇ (cpu->quota_present == cpu->period_present)
    ਵਾਪਸ 0;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;
  ਜੇ (cgroup_ਬਹੁਲਕ != CGROUP_ਬਹੁਲਕ_UNIFIED)
    ਵਾਪਸ 0;

  ret = append_paths (&cpu_max_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, "cpu.max", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = read_all_file (cpu_max_ਰਸਤਾ, &content, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      ਵਾਪਸ 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  ਨਹੀਂਤਾਂ ਜੇ (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_update_cgroup_resources (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ,
                                 ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  ਬਣਤਰ libcrun_cgroup_manager *cgroup_manager = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = get_cgroup_manager (cgroup_ਸਥਿਤੀ->manager, &cgroup_manager, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = check_ਸਮ੍ਰਿਤੀ_before_update (cgroup_ਸਥਿਤੀ->ਰਸਤਾ, resources, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = complete_cpu_max (cgroup_ਸਥਿਤੀ->ਰਸਤਾ, resources, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_ਸਥਿਤੀ, state_ਜੜ੍ਹ, resources, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ update_cgroup_resources (cgroup_ਸਥਿਤੀ->ਰਸਤਾ, state_ਜੜ੍ਹ, resources, ! cgroup_ਸਥਿਤੀ->bpf_dev_ਸੈੱਟ, err);
}

ਸਥਿਰ ਪੂਰਨ
can_ignore_cgroup_enter_errors (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ, ਪੂਰਨ cgroup_ਬਹੁਲਕ,
                                libcrun_error_t *err)
{
  ਪੂਰਨ manager = ਦਲੀਲਾਂ->manager;
  ਪੂਰਨ rootless;

  rootless = is_rootless (err);
  ਜੇ (UNLIKELY (rootless < 0))
    ਵਾਪਸ rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  ਜੇ (! rootless)
    ਵਾਪਸ 0;

  ਜੇ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਦਲੀਲਾਂ->cgroup_ਰਸਤਾ))
    ਵਾਪਸ 0;

  ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    ਵਾਪਸ 0;

  ਵਾਪਸ 1;
}

ਪੂਰਨ
libcrun_cgroup_preenter (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ, ਪੂਰਨ *dirfd, libcrun_error_t *err)
{
  ਬਣਤਰ libcrun_cgroup_manager *cgroup_manager;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  *dirfd = -1;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (cgroup_ਬਹੁਲਕ != CGROUP_ਬਹੁਲਕ_UNIFIED)
    ਵਾਪਸ 0;

  ret = get_cgroup_manager (ਦਲੀਲਾਂ->manager, &cgroup_manager, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (cgroup_manager->precreate_cgroup == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਵਾਪਸ cgroup_manager->precreate_cgroup (ਦਲੀਲਾਂ, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
ਖਾਲੀ
libcrun_cgroup_preenter_cleanup (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ)
{
  ਜੇ (ਦਲੀਲਾਂ->precreated_ਰਸਤਾ == ਨਿਰਾ)
    ਵਾਪਸ;

  (ਖਾਲੀ) rmdir (ਦਲੀਲਾਂ->precreated_ਰਸਤਾ);
  ਮੁਕਤ (ਦਲੀਲਾਂ->precreated_ਰਸਤਾ);
  ਦਲੀਲਾਂ->precreated_ਰਸਤਾ = ਨਿਰਾ;
}

ਪੂਰਨ
libcrun_cgroup_enter (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ, ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ **ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ = xmalloc0 (ਆਕਾਰ *ਸਥਿਤੀ);
  ਬਣਤਰ libcrun_cgroup_manager *cgroup_manager;
  uid_t ਜੜ੍ਹ_uid = ਦਲੀਲਾਂ->ਜੜ੍ਹ_uid;
  uid_t ਜੜ੍ਹ_gid = ਦਲੀਲਾਂ->ਜੜ੍ਹ_gid;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;

      ret = read_all_file (CGROUP_ਜੜ੍ਹ "/unified/cgroup.controllers", &ਬਫਰ, &len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      ਜੇ (len > 0)
        ਵਾਪਸ crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (ਦਲੀਲਾਂ->manager, &cgroup_manager, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਸਥਿਤੀ->manager = ਦਲੀਲਾਂ->manager;

  ret = cgroup_manager->create_cgroup (ਦਲੀਲਾਂ, ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਪੂਰਨ ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (ਦਲੀਲਾਂ, cgroup_ਬਹੁਲਕ, &tmp_err);
      ਜੇ (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ਵਾਪਸ ignore_cgroup_errors;
        }

      ਜੇ (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          ਮੁਕਤ (ਸਥਿਤੀ->ਰਸਤਾ);
          ਮੁਕਤ (ਸਥਿਤੀ->scope);
          ਸਥਿਤੀ->ਰਸਤਾ = ਨਿਰਾ;
          ਸਥਿਤੀ->scope = ਨਿਰਾ;
          ਸਥਿਤੀ->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          ਜਾਓ success;
        }

      ਵਾਪਸ ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  ਮੁਕਤ (ਦਲੀਲਾਂ->precreated_ਰਸਤਾ);
  ਦਲੀਲਾਂ->precreated_ਰਸਤਾ = ਨਿਰਾ;

  ਜੇ (ਸਥਿਤੀ->ਰਸਤਾ)
    {
      bool need_chown;

      need_chown = ਜੜ੍ਹ_uid != (uid_t) -1 || ਜੜ੍ਹ_gid != (gid_t) -1;
      ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED && need_chown)
        {
          ret = chown_cgroups (ਸਥਿਤੀ->ਰਸਤਾ, ਜੜ੍ਹ_uid, ਜੜ੍ਹ_gid, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਜਾਓ fail_destroy;
        }

      ਜੇ (ਦਲੀਲਾਂ->resources)
        {
          ret = update_cgroup_resources (ਸਥਿਤੀ->ਰਸਤਾ, ਦਲੀਲਾਂ->state_ਜੜ੍ਹ, ਦਲੀਲਾਂ->resources, ! ਸਥਿਤੀ->bpf_dev_ਸੈੱਟ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਜਾਓ fail_destroy;
        }
    }
success:
  *ਬਾਹਰਲਾ = ਸਥਿਤੀ;
  ਸਥਿਤੀ = ਨਿਰਾ;
  ਵਾਪਸ 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = ਨਿਰਾ;

    ਜੇ (UNLIKELY (cgroup_manager->destroy_cgroup (ਸਥਿਤੀ, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_cgroup_enter_finalize (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ, ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ arg_ਅਣਵਰਤਿਆ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਨਿਸ਼ਾਨਾ_cgroup = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *delegate_cgroup;
  cleanup_free ਅੱਖਰ *dir = ਨਿਰਾ;
  ਅੱਖਰ *current_cgroup, *ਮੰਜ਼ਿਲ;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  delegate_cgroup = ਲੱਭੋ_delegate_cgroup (ਦਲੀਲਾਂ->annotations);
  ਜੇ (delegate_cgroup == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਜੇ (ਰਸਤਾ_has_dot_dot_component (delegate_cgroup))
    ਵਾਪਸ crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (cgroup_ਬਹੁਲਕ != CGROUP_ਬਹੁਲਕ_UNIFIED)
    ਵਾਪਸ crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free ਅੱਖਰ *proc_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ fd = -1;

  xasprintf (&proc_ਰਸਤਾ, "%d/cgroup", ਦਲੀਲਾਂ->pid);
  fd = libcrun_open_proc_file (ਦਲੀਲਾਂ->container, proc_ਰਸਤਾ, O_RDONLY, err);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ fd;

  ret = read_all_fd (fd, "cgroup path", &content, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  current_cgroup = ਲੜੀ_ਖੋਜ (content, "0::");
  ਜੇ (UNLIKELY (current_cgroup == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  ਮੰਜ਼ਿਲ = strchr (current_cgroup, '\n');
  ਜੇ (UNLIKELY (ਮੰਜ਼ਿਲ == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *ਮੰਜ਼ਿਲ = '\0';

  ret = append_paths (&ਨਿਸ਼ਾਨਾ_cgroup, err, current_cgroup, delegate_cgroup, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_paths (&dir, err, CGROUP_ਜੜ੍ਹ, ਨਿਸ਼ਾਨਾ_cgroup, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = move_process_ਮੰਜ਼ਿਲ_cgroup (ਦਲੀਲਾਂ->pid, ਨਿਰਾ, ਨਿਸ਼ਾਨਾ_cgroup, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = enable_controllers (ਨਿਸ਼ਾਨਾ_cgroup, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = chown_cgroups (ਨਿਸ਼ਾਨਾ_cgroup, ਦਲੀਲਾਂ->ਜੜ੍ਹ_uid, ਦਲੀਲਾਂ->ਜੜ੍ਹ_gid, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_cgroup_has_oom (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = ਸਥਿਤੀ->ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ = ਨਿਰਾ;
  size_t content_ਮਾਪ = 0;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਅੱਖਰ *it;

  ਜੇ (UNLIKELY (ਰਸਤਾ == ਨਿਰਾ || ਰਸਤਾ[0] == '\0'))
    ਵਾਪਸ 0;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਚੋਣ (cgroup_ਬਹੁਲਕ)
    {
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_UNIFIED:
      {
        cleanup_free ਅੱਖਰ *events_ਰਸਤਾ = ਨਿਰਾ;
        ਪੂਰਨ ret;

        ret = append_paths (&events_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, "memory.events", ਨਿਰਾ);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_ਰਸਤਾ, &content, &content_ਮਾਪ, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        ਅਗੇਤਰ = "oom ";
        ਰੋਕੋ;
      }
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_LEGACY:
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_HYBRID:
      {
        cleanup_free ਅੱਖਰ *oom_control_ਰਸਤਾ = ਨਿਰਾ;
        ਪੂਰਨ ret;

        ret = append_paths (&oom_control_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, "memory", ਰਸਤਾ, "memory.oom_control", ਨਿਰਾ);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_ਰਸਤਾ, &content, &content_ਮਾਪ, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        ਅਗੇਤਰ = "oom_kill ";
        ਰੋਕੋ;
      }

    ਮੂਲ:
      ਵਾਪਸ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_ਬਹੁਲਕ);
    }

  it = content;
  ਜਦੋਂ (it && *it)
    {
      ਜੇ (has_ਅਗੇਤਰ (it, ਅਗੇਤਰ))
        {
          it += ਲੜੀ_ਲੰਬਾਈ (ਅਗੇਤਰ);
          ਜਦੋਂ (*it == ' ')
            it++;

          ਵਾਪਸ *it != '0';
        }
      ਨਹੀਂਤਾਂ
        {
          it = strchr (it, '\n');
          ਜੇ (it == ਨਿਰਾ)
            ਵਾਪਸ 0;
          it++;
        }
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_cgroup_get_ਸਥਿਤੀ (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ,
                           libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ,
                           libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
  ਸਥਿਤੀ->cgroup_ਰਸਤਾ = cgroup_ਸਥਿਤੀ->ਰਸਤਾ;
  ਸਥਿਤੀ->scope = cgroup_ਸਥਿਤੀ->scope;
  ਵਾਪਸ 0;
}

ਖਾਲੀ
libcrun_cgroup_ਸਥਿਤੀ_free (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ)
{
  ਜੇ (cgroup_ਸਥਿਤੀ == ਨਿਰਾ)
    ਵਾਪਸ;

  ਮੁਕਤ (cgroup_ਸਥਿਤੀ->ਰਸਤਾ);
  ਮੁਕਤ (cgroup_ਸਥਿਤੀ->scope);
  ਮੁਕਤ (cgroup_ਸਥਿਤੀ);
}

ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *
libcrun_cgroup_make_ਸਥਿਤੀ (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ)
{
  ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ret;

  ret = xmalloc0 (ਆਕਾਰ *ret);

  ਜੇ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਸਥਿਤੀ->cgroup_ਰਸਤਾ))
    ret->ਰਸਤਾ = xstrdup (ਸਥਿਤੀ->cgroup_ਰਸਤਾ);

  ਜੇ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਸਥਿਤੀ->scope))
    ret->scope = xstrdup (ਸਥਿਤੀ->scope);

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਸਥਿਤੀ->cgroup_ਰਸਤਾ) && is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਸਥਿਤੀ->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  ਨਹੀਂਤਾਂ
    ret->manager = ਸਥਿਤੀ->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  ਵਾਪਸ ret;
}
