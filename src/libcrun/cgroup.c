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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "cgroup.h"
#அந்தர்பாவ "cgroup-cgroupfs.h"
#அந்தர்பாவ "cgroup-internal.h"
#அந்தர்பாவ "cgroup-systemd.h"
#அந்தர்பாவ "cgroup-utils.h"
#அந்தர்பாவ "cgroup-setup.h"
#அந்தர்பாவ "cgroup-resources.h"
#அந்தர்பாவ "ebpf.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "status.h"
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <sys/vfs.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ <time.h>

#அந்தர்பாவ <linux/magic.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <libgen.h>

நிலையான முழு
libcrun_cgroup_enter_disabled (கட்டமைப்பு libcrun_cgroup_args *args arg_unused, கட்டமைப்பு libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = வெற்று;
  திரும்பு 0;
}

நிலையான முழு
libcrun_destroy_cgroup_disabled (கட்டமைப்பு libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  திரும்பு 0;
}

கட்டமைப்பு libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = வெற்று,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

நிலையான முழு
get_cgroup_manager (முழு manager, கட்டமைப்பு libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  தேர்வு (manager)
    {
    நிலை CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      திரும்பு 0;

    நிலை CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      திரும்பு 0;

    நிலை CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      திரும்பு 0;
    }

  *out = வெற்று;
  திரும்பு crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

நிலையான மாறிலி எழுத்து *
find_delegate_cgroup (string_map *annotations)
{
  மாறிலி எழுத்து *annotation;

  annotation = find_string_map_மதிப்பு (annotations, "run.oci.delegate-cgroup");
  எனில் (annotation)
    {
      எனில் (annotation[0] == '\0')
        திரும்பு வெற்று;
      திரும்பு annotation;
    }

  திரும்பு வெற்று;
}

முழு
libcrun_cgroup_pause_unpause (கட்டமைப்பு libcrun_cgroup_status *status, மாறிலி bool pause, libcrun_error_t *err)
{
  திரும்பு libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

முழு
libcrun_cgroup_is_container_paused (கட்டமைப்பு libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  மாறிலி எழுத்து *cgroup_path = status->path;
  cleanup_free எழுத்து *content = வெற்று;
  cleanup_free எழுத்து *path = வெற்று;
  மாறிலி எழுத்து *state;
  முழு cgroup_mode;
  முழு ret;

  எனில் (cgroup_path == வெற்று)
    திரும்பு 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_வேர், cgroup_path, "cgroup.freeze", வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_வேர் "/freezer", cgroup_path, "freezer.state", வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = read_all_file (path, &content, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      எனில் (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        திரும்பு ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      கட்டமைப்பு statfs freezer_stat;
      எனில் (statfs (CGROUP_வேர் "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          திரும்பு crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_வேர் "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      எனில் (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        திரும்பு ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      திரும்பு 0;
    }

  *paused = strstr (content, state) != வெற்று;
  திரும்பு 0;
}

முழு
libcrun_cgroup_join_process (கட்டமைப்பு libcrun_cgroup_status *status, மாறிலி எழுத்து *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  கட்டமைப்பு libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = வெற்று;
  முழு errno_;
  முழு ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  எனில் (LIKELY (ret >= 0))
    திரும்பு ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  எனில் (errno_ != EACCES && errno_ != EPERM)
    திரும்பு ret;

  எனில் (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      திரும்பு ret;
    }
  எனில் (cgroup_manager->attach_process == வெற்று)
    திரும்பு ret;

  எனில் (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->செய்தி);
      crun_error_release (&tmp_err);
      திரும்பு ret;
    }

  crun_error_release (err);
  திரும்பு 0;
}

முழு
libcrun_cgroup_read_pids (கட்டமைப்பு libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  திரும்பு libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

முழு
libcrun_cgroup_killall (கட்டமைப்பு libcrun_cgroup_status *cgroup_status, முழு சமிக்ஞை, libcrun_error_t *err)
{
  திரும்பு cgroup_killall_path (cgroup_status->path, சமிக்ஞை, err);
}

முழு
libcrun_cgroup_destroy (கட்டமைப்பு libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  கட்டமைப்பு libcrun_cgroup_manager *cgroup_manager = வெற்று;
  முழு ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
நிலையான முழு
complete_cpu_max (மாறிலி எழுத்து *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free எழுத்து *cpu_max_path = வெற்று;
  cleanup_free எழுத்து *content = வெற்று;
  bool has_period;
  uint64_t period;
  int64_t quota;
  முழு cgroup_mode;
  முழு ret;

  எனில் (resources == வெற்று || resources->cpu == வெற்று)
    திரும்பு 0;

  cpu = resources->cpu;
  எனில் (cpu->quota_present == cpu->period_present)
    திரும்பு 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;
  எனில் (cgroup_mode != CGROUP_MODE_UNIFIED)
    திரும்பு 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_வேர், path, "cpu.max", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = read_all_file (cpu_max_path, &content, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      திரும்பு 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  இல்லையெனில் எனில் (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  திரும்பு 0;
}

முழு
libcrun_update_cgroup_resources (கட்டமைப்பு libcrun_cgroup_status *cgroup_status,
                                 மாறிலி எழுத்து *state_வேர்,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  கட்டமைப்பு libcrun_cgroup_manager *cgroup_manager = வெற்று;
  முழு ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_வேர், resources, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு update_cgroup_resources (cgroup_status->path, state_வேர், resources, ! cgroup_status->bpf_dev_set, err);
}

நிலையான முழு
can_ignore_cgroup_enter_errors (கட்டமைப்பு libcrun_cgroup_args *args, முழு cgroup_mode,
                                libcrun_error_t *err)
{
  முழு manager = args->manager;
  முழு rootless;

  rootless = is_rootless (err);
  எனில் (UNLIKELY (rootless < 0))
    திரும்பு rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  எனில் (! rootless)
    திரும்பு 0;

  எனில் (! is_empty_string (args->cgroup_path))
    திரும்பு 0;

  எனில் (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    திரும்பு 0;

  திரும்பு 1;
}

முழு
libcrun_cgroup_preenter (கட்டமைப்பு libcrun_cgroup_args *args, முழு *dirfd, libcrun_error_t *err)
{
  கட்டமைப்பு libcrun_cgroup_manager *cgroup_manager;
  முழு cgroup_mode;
  முழு ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  எனில் (cgroup_mode != CGROUP_MODE_UNIFIED)
    திரும்பு 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (cgroup_manager->precreate_cgroup == வெற்று)
    திரும்பு 0;

  திரும்பு cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
வெறுமை
libcrun_cgroup_preenter_cleanup (கட்டமைப்பு libcrun_cgroup_args *args)
{
  எனில் (args->precreated_path == வெற்று)
    திரும்பு;

  (வெறுமை) rmdir (args->precreated_path);
  விடுவி (args->precreated_path);
  args->precreated_path = வெற்று;
}

முழு
libcrun_cgroup_enter (கட்டமைப்பு libcrun_cgroup_args *args, கட்டமைப்பு libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *status = xmalloc0 (அளவு *status);
  கட்டமைப்பு libcrun_cgroup_manager *cgroup_manager;
  uid_t வேர்_uid = args->வேர்_uid;
  uid_t வேர்_gid = args->வேர்_gid;
  முழு cgroup_mode;
  முழு ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  எனில் (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free எழுத்து *buffer = வெற்று;

      ret = read_all_file (CGROUP_வேர் "/unified/cgroup.controllers", &buffer, &len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      எனில் (len > 0)
        திரும்பு crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  எனில் (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = வெற்று;
      முழு ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      எனில் (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          திரும்பு ignore_cgroup_errors;
        }

      எனில் (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          விடுவி (status->path);
          விடுவி (status->scope);
          status->path = வெற்று;
          status->scope = வெற்று;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          செல் success;
        }

      திரும்பு ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  விடுவி (args->precreated_path);
  args->precreated_path = வெற்று;

  எனில் (status->path)
    {
      bool need_chown;

      need_chown = வேர்_uid != (uid_t) -1 || வேர்_gid != (gid_t) -1;
      எனில் (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, வேர்_uid, வேர்_gid, err);
          எனில் (UNLIKELY (ret < 0))
            செல் fail_destroy;
        }

      எனில் (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_வேர், args->resources, ! status->bpf_dev_set, err);
          எனில் (UNLIKELY (ret < 0))
            செல் fail_destroy;
        }
    }
success:
  *out = status;
  status = வெற்று;
  திரும்பு 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = வெற்று;

    எனில் (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  திரும்பு ret;
}

முழு
libcrun_cgroup_enter_finalize (கட்டமைப்பு libcrun_cgroup_args *args, கட்டமைப்பு libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free எழுத்து *target_cgroup = வெற்று;
  cleanup_free எழுத்து *content = வெற்று;
  மாறிலி எழுத்து *delegate_cgroup;
  cleanup_free எழுத்து *dir = வெற்று;
  எழுத்து *current_cgroup, *to;
  முழு cgroup_mode;
  முழு ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  எனில் (delegate_cgroup == வெற்று)
    திரும்பு 0;

  எனில் (path_has_dot_dot_component (delegate_cgroup))
    திரும்பு crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  எனில் (cgroup_mode != CGROUP_MODE_UNIFIED)
    திரும்பு crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free எழுத்து *proc_path = வெற்று;
  cleanup_close முழு fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு fd;

  ret = read_all_fd (fd, "cgroup path", &content, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  current_cgroup = strstr (content, "0::");
  எனில் (UNLIKELY (current_cgroup == வெற்று))
    திரும்பு crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  எனில் (UNLIKELY (to == வெற்று))
    திரும்பு crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_paths (&dir, err, CGROUP_வேர், target_cgroup, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = move_process_to_cgroup (args->pid, வெற்று, target_cgroup, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = enable_controllers (target_cgroup, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = chown_cgroups (target_cgroup, args->வேர்_uid, args->வேர்_gid, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

முழு
libcrun_cgroup_has_oom (கட்டமைப்பு libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free எழுத்து *content = வெற்று;
  மாறிலி எழுத்து *path = status->path;
  மாறிலி எழுத்து *prefix = வெற்று;
  size_t content_size = 0;
  முழு cgroup_mode;
  எழுத்து *it;

  எனில் (UNLIKELY (path == வெற்று || path[0] == '\0'))
    திரும்பு 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  தேர்வு (cgroup_mode)
    {
    நிலை CGROUP_MODE_UNIFIED:
      {
        cleanup_free எழுத்து *events_path = வெற்று;
        முழு ret;

        ret = append_paths (&events_path, err, CGROUP_வேர், path, "memory.events", வெற்று);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        prefix = "oom ";
        நிறுத்து;
      }
    நிலை CGROUP_MODE_LEGACY:
    நிலை CGROUP_MODE_HYBRID:
      {
        cleanup_free எழுத்து *oom_control_path = வெற்று;
        முழு ret;

        ret = append_paths (&oom_control_path, err, CGROUP_வேர், "memory", path, "memory.oom_control", வெற்று);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        prefix = "oom_kill ";
        நிறுத்து;
      }

    இயல்பு:
      திரும்பு crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  போது (it && *it)
    {
      எனில் (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          போது (*it == ' ')
            it++;

          திரும்பு *it != '0';
        }
      இல்லையெனில்
        {
          it = strchr (it, '\n');
          எனில் (it == வெற்று)
            திரும்பு 0;
          it++;
        }
    }

  திரும்பு 0;
}

முழு
libcrun_cgroup_get_status (கட்டமைப்பு libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  திரும்பு 0;
}

வெறுமை
libcrun_cgroup_status_free (கட்டமைப்பு libcrun_cgroup_status *cgroup_status)
{
  எனில் (cgroup_status == வெற்று)
    திரும்பு;

  விடுவி (cgroup_status->path);
  விடுவி (cgroup_status->scope);
  விடுவி (cgroup_status);
}

கட்டமைப்பு libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  கட்டமைப்பு libcrun_cgroup_status *ret;

  ret = xmalloc0 (அளவு *ret);

  எனில் (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  எனில் (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  எனில் (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  இல்லையெனில்
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  திரும்பு ret;
}
