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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "cgroup.h"
#അന്തര്ഭാവ "cgroup-cgroupfs.h"
#അന്തര്ഭാവ "cgroup-internal.h"
#അന്തര്ഭാവ "cgroup-systemd.h"
#അന്തര്ഭാവ "cgroup-utils.h"
#അന്തര്ഭാവ "cgroup-setup.h"
#അന്തര്ഭാവ "cgroup-resources.h"
#അന്തര്ഭാവ "ebpf.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "status.h"
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sched.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <sys/vfs.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ <linux/magic.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <libgen.h>

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_cgroup_enter_disabled (ഘടന libcrun_cgroup_args *args arg_unused, ഘടന libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = ശൂന്യ;
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_destroy_cgroup_disabled (ഘടന libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  തിരികെ 0;
}

ഘടന libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = ശൂന്യ,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_cgroup_manager (പൂർണ്ണസംഖ്യ manager, ഘടന libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  തിരഞ്ഞെടുപ്പ് (manager)
    {
    അവസ്ഥ CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      തിരികെ 0;

    അവസ്ഥ CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      തിരികെ 0;

    അവസ്ഥ CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      തിരികെ 0;
    }

  *out = ശൂന്യ;
  തിരികെ crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
find_delegate_cgroup (string_map *annotations)
{
  സ്ഥിരം അക്ഷരം *annotation;

  annotation = find_string_map_മൂല്യം (annotations, "run.oci.delegate-cgroup");
  എങ്കിൽ (annotation)
    {
      എങ്കിൽ (annotation[0] == '\0')
        തിരികെ ശൂന്യ;
      തിരികെ annotation;
    }

  തിരികെ ശൂന്യ;
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_pause_unpause (ഘടന libcrun_cgroup_status *status, സ്ഥിരം bool pause, libcrun_error_t *err)
{
  തിരികെ libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_is_container_paused (ഘടന libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *cgroup_path = status->path;
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  cleanup_free അക്ഷരം *path = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *state;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (cgroup_path == ശൂന്യ)
    തിരികെ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_വേര്, cgroup_path, "cgroup.freeze", ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_വേര് "/freezer", cgroup_path, "freezer.state", ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = read_all_file (path, &content, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      എങ്കിൽ (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        തിരികെ ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      ഘടന statfs freezer_stat;
      എങ്കിൽ (statfs (CGROUP_വേര് "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          തിരികെ crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_വേര് "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      എങ്കിൽ (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        തിരികെ ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      തിരികെ 0;
    }

  *paused = strstr (content, state) != ശൂന്യ;
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_join_process (ഘടന libcrun_cgroup_status *status, സ്ഥിരം അക്ഷരം *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  ഘടന libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = ശൂന്യ;
  പൂർണ്ണസംഖ്യ errno_;
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  എങ്കിൽ (LIKELY (ret >= 0))
    തിരികെ ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  എങ്കിൽ (errno_ != EACCES && errno_ != EPERM)
    തിരികെ ret;

  എങ്കിൽ (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      തിരികെ ret;
    }
  എങ്കിൽ (cgroup_manager->attach_process == ശൂന്യ)
    തിരികെ ret;

  എങ്കിൽ (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->സന്ദേശം);
      crun_error_release (&tmp_err);
      തിരികെ ret;
    }

  crun_error_release (err);
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_read_pids (ഘടന libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  തിരികെ libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_killall (ഘടന libcrun_cgroup_status *cgroup_status, പൂർണ്ണസംഖ്യ സൂചന, libcrun_error_t *err)
{
  തിരികെ cgroup_killall_path (cgroup_status->path, സൂചന, err);
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_destroy (ഘടന libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  ഘടന libcrun_cgroup_manager *cgroup_manager = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
complete_cpu_max (സ്ഥിരം അക്ഷരം *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free അക്ഷരം *cpu_max_path = ശൂന്യ;
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  bool has_period;
  uint64_t period;
  int64_t quota;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (resources == ശൂന്യ || resources->cpu == ശൂന്യ)
    തിരികെ 0;

  cpu = resources->cpu;
  എങ്കിൽ (cpu->quota_present == cpu->period_present)
    തിരികെ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;
  എങ്കിൽ (cgroup_mode != CGROUP_MODE_UNIFIED)
    തിരികെ 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_വേര്, path, "cpu.max", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = read_all_file (cpu_max_path, &content, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      തിരികെ 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_update_cgroup_resources (ഘടന libcrun_cgroup_status *cgroup_status,
                                 സ്ഥിരം അക്ഷരം *state_വേര്,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  ഘടന libcrun_cgroup_manager *cgroup_manager = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_വേര്, resources, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ update_cgroup_resources (cgroup_status->path, state_വേര്, resources, ! cgroup_status->bpf_dev_set, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
can_ignore_cgroup_enter_errors (ഘടന libcrun_cgroup_args *args, പൂർണ്ണസംഖ്യ cgroup_mode,
                                libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ manager = args->manager;
  പൂർണ്ണസംഖ്യ rootless;

  rootless = is_rootless (err);
  എങ്കിൽ (UNLIKELY (rootless < 0))
    തിരികെ rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  എങ്കിൽ (! rootless)
    തിരികെ 0;

  എങ്കിൽ (! is_empty_string (args->cgroup_path))
    തിരികെ 0;

  എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    തിരികെ 0;

  തിരികെ 1;
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_preenter (ഘടന libcrun_cgroup_args *args, പൂർണ്ണസംഖ്യ *dirfd, libcrun_error_t *err)
{
  ഘടന libcrun_cgroup_manager *cgroup_manager;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  എങ്കിൽ (cgroup_mode != CGROUP_MODE_UNIFIED)
    തിരികെ 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (cgroup_manager->precreate_cgroup == ശൂന്യ)
    തിരികെ 0;

  തിരികെ cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
ശൂന്യം
libcrun_cgroup_preenter_cleanup (ഘടന libcrun_cgroup_args *args)
{
  എങ്കിൽ (args->precreated_path == ശൂന്യ)
    തിരികെ;

  (ശൂന്യം) rmdir (args->precreated_path);
  സ്വതന്ത്രം (args->precreated_path);
  args->precreated_path = ശൂന്യ;
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_enter (ഘടന libcrun_cgroup_args *args, ഘടന libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status ഘടന libcrun_cgroup_status *status = xmalloc0 (വലുപ്പം *status);
  ഘടന libcrun_cgroup_manager *cgroup_manager;
  uid_t വേര്_uid = args->വേര്_uid;
  uid_t വേര്_gid = args->വേര്_gid;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  എങ്കിൽ (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free അക്ഷരം *buffer = ശൂന്യ;

      ret = read_all_file (CGROUP_വേര് "/unified/cgroup.controllers", &buffer, &len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      എങ്കിൽ (len > 0)
        തിരികെ crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      പൂർണ്ണസംഖ്യ ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      എങ്കിൽ (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          തിരികെ ignore_cgroup_errors;
        }

      എങ്കിൽ (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          സ്വതന്ത്രം (status->path);
          സ്വതന്ത്രം (status->scope);
          status->path = ശൂന്യ;
          status->scope = ശൂന്യ;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          പോകുക success;
        }

      തിരികെ ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  സ്വതന്ത്രം (args->precreated_path);
  args->precreated_path = ശൂന്യ;

  എങ്കിൽ (status->path)
    {
      bool need_chown;

      need_chown = വേര്_uid != (uid_t) -1 || വേര്_gid != (gid_t) -1;
      എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, വേര്_uid, വേര്_gid, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            പോകുക fail_destroy;
        }

      എങ്കിൽ (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_വേര്, args->resources, ! status->bpf_dev_set, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            പോകുക fail_destroy;
        }
    }
success:
  *out = status;
  status = ശൂന്യ;
  തിരികെ 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = ശൂന്യ;

    എങ്കിൽ (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_enter_finalize (ഘടന libcrun_cgroup_args *args, ഘടന libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *target_cgroup = ശൂന്യ;
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *delegate_cgroup;
  cleanup_free അക്ഷരം *dir = ശൂന്യ;
  അക്ഷരം *current_cgroup, *to;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  എങ്കിൽ (delegate_cgroup == ശൂന്യ)
    തിരികെ 0;

  എങ്കിൽ (path_has_dot_dot_component (delegate_cgroup))
    തിരികെ crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  എങ്കിൽ (cgroup_mode != CGROUP_MODE_UNIFIED)
    തിരികെ crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free അക്ഷരം *proc_path = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ fd;

  ret = read_all_fd (fd, "cgroup path", &content, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  current_cgroup = strstr (content, "0::");
  എങ്കിൽ (UNLIKELY (current_cgroup == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  എങ്കിൽ (UNLIKELY (to == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_paths (&dir, err, CGROUP_വേര്, target_cgroup, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = move_process_to_cgroup (args->pid, ശൂന്യ, target_cgroup, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = enable_controllers (target_cgroup, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = chown_cgroups (target_cgroup, args->വേര്_uid, args->വേര്_gid, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_has_oom (ഘടന libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *path = status->path;
  സ്ഥിരം അക്ഷരം *prefix = ശൂന്യ;
  size_t content_size = 0;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  അക്ഷരം *it;

  എങ്കിൽ (UNLIKELY (path == ശൂന്യ || path[0] == '\0'))
    തിരികെ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  തിരഞ്ഞെടുപ്പ് (cgroup_mode)
    {
    അവസ്ഥ CGROUP_MODE_UNIFIED:
      {
        cleanup_free അക്ഷരം *events_path = ശൂന്യ;
        പൂർണ്ണസംഖ്യ ret;

        ret = append_paths (&events_path, err, CGROUP_വേര്, path, "memory.events", ശൂന്യ);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        prefix = "oom ";
        നിർത്തുക;
      }
    അവസ്ഥ CGROUP_MODE_LEGACY:
    അവസ്ഥ CGROUP_MODE_HYBRID:
      {
        cleanup_free അക്ഷരം *oom_control_path = ശൂന്യ;
        പൂർണ്ണസംഖ്യ ret;

        ret = append_paths (&oom_control_path, err, CGROUP_വേര്, "memory", path, "memory.oom_control", ശൂന്യ);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        prefix = "oom_kill ";
        നിർത്തുക;
      }

    സ്ഥിരസ്ഥിതി:
      തിരികെ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  ആകുമ്പോൾ (it && *it)
    {
      എങ്കിൽ (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          ആകുമ്പോൾ (*it == ' ')
            it++;

          തിരികെ *it != '0';
        }
      അല്ലെങ്കിൽ
        {
          it = strchr (it, '\n');
          എങ്കിൽ (it == ശൂന്യ)
            തിരികെ 0;
          it++;
        }
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_get_status (ഘടന libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  തിരികെ 0;
}

ശൂന്യം
libcrun_cgroup_status_free (ഘടന libcrun_cgroup_status *cgroup_status)
{
  എങ്കിൽ (cgroup_status == ശൂന്യ)
    തിരികെ;

  സ്വതന്ത്രം (cgroup_status->path);
  സ്വതന്ത്രം (cgroup_status->scope);
  സ്വതന്ത്രം (cgroup_status);
}

ഘടന libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  ഘടന libcrun_cgroup_status *ret;

  ret = xmalloc0 (വലുപ്പം *ret);

  എങ്കിൽ (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  എങ്കിൽ (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  എങ്കിൽ (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  അല്ലെങ്കിൽ
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  തിരികെ ret;
}
