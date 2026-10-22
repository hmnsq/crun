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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "cgroup.h"
#અન્તર્ભાવ "cgroup-cgroupfs.h"
#અન્તર્ભાવ "cgroup-internal.h"
#અન્તર્ભાવ "cgroup-systemd.h"
#અન્તર્ભાવ "cgroup-utils.h"
#અન્તર્ભાવ "cgroup-setup.h"
#અન્તર્ભાવ "cgroup-resources.h"
#અન્તર્ભાવ "ebpf.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "status.h"
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sched.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <sys/vfs.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ <linux/magic.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <libgen.h>

સ્થિર પૂર્ણાંક
libcrun_cgroup_enter_disabled (સંરચના libcrun_cgroup_args *args arg_unused, સંરચના libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = ખાલી;
  પાછા 0;
}

સ્થિર પૂર્ણાંક
libcrun_destroy_cgroup_disabled (સંરચના libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  પાછા 0;
}

સંરચના libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = ખાલી,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

સ્થિર પૂર્ણાંક
get_cgroup_manager (પૂર્ણાંક manager, સંરચના libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  પસંદગી (manager)
    {
    સ્થિતિ CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      પાછા 0;

    સ્થિતિ CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      પાછા 0;

    સ્થિતિ CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      પાછા 0;
    }

  *out = ખાલી;
  પાછા crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

સ્થિર અચલ અક્ષર *
find_delegate_cgroup (string_map *annotations)
{
  અચલ અક્ષર *annotation;

  annotation = find_string_map_કિંમત (annotations, "run.oci.delegate-cgroup");
  જો (annotation)
    {
      જો (annotation[0] == '\0')
        પાછા ખાલી;
      પાછા annotation;
    }

  પાછા ખાલી;
}

પૂર્ણાંક
libcrun_cgroup_pause_unpause (સંરચના libcrun_cgroup_status *status, અચલ bool pause, libcrun_error_t *err)
{
  પાછા libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

પૂર્ણાંક
libcrun_cgroup_is_container_paused (સંરચના libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  અચલ અક્ષર *cgroup_path = status->path;
  cleanup_free અક્ષર *content = ખાલી;
  cleanup_free અક્ષર *path = ખાલી;
  અચલ અક્ષર *state;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક ret;

  જો (cgroup_path == ખાલી)
    પાછા 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  જો (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_મૂળ, cgroup_path, "cgroup.freeze", ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_મૂળ "/freezer", cgroup_path, "freezer.state", ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = read_all_file (path, &content, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      જો (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        પાછા ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      સંરચના statfs freezer_stat;
      જો (statfs (CGROUP_મૂળ "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          પાછા crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_મૂળ "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      જો (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        પાછા ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      પાછા 0;
    }

  *paused = strstr (content, state) != ખાલી;
  પાછા 0;
}

પૂર્ણાંક
libcrun_cgroup_join_process (સંરચના libcrun_cgroup_status *status, અચલ અક્ષર *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  સંરચના libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = ખાલી;
  પૂર્ણાંક errno_;
  પૂર્ણાંક ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  જો (LIKELY (ret >= 0))
    પાછા ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  જો (errno_ != EACCES && errno_ != EPERM)
    પાછા ret;

  જો (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      પાછા ret;
    }
  જો (cgroup_manager->attach_process == ખાલી)
    પાછા ret;

  જો (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->સંદેશ);
      crun_error_release (&tmp_err);
      પાછા ret;
    }

  crun_error_release (err);
  પાછા 0;
}

પૂર્ણાંક
libcrun_cgroup_read_pids (સંરચના libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  પાછા libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

પૂર્ણાંક
libcrun_cgroup_killall (સંરચના libcrun_cgroup_status *cgroup_status, પૂર્ણાંક સંકેત, libcrun_error_t *err)
{
  પાછા cgroup_killall_path (cgroup_status->path, સંકેત, err);
}

પૂર્ણાંક
libcrun_cgroup_destroy (સંરચના libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  સંરચના libcrun_cgroup_manager *cgroup_manager = ખાલી;
  પૂર્ણાંક ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
સ્થિર પૂર્ણાંક
complete_cpu_max (અચલ અક્ષર *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free અક્ષર *cpu_max_path = ખાલી;
  cleanup_free અક્ષર *content = ખાલી;
  bool has_period;
  uint64_t period;
  int64_t quota;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક ret;

  જો (resources == ખાલી || resources->cpu == ખાલી)
    પાછા 0;

  cpu = resources->cpu;
  જો (cpu->quota_present == cpu->period_present)
    પાછા 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;
  જો (cgroup_mode != CGROUP_MODE_UNIFIED)
    પાછા 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_મૂળ, path, "cpu.max", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = read_all_file (cpu_max_path, &content, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      પાછા 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  નહીંતર જો (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  પાછા 0;
}

પૂર્ણાંક
libcrun_update_cgroup_resources (સંરચના libcrun_cgroup_status *cgroup_status,
                                 અચલ અક્ષર *state_મૂળ,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  સંરચના libcrun_cgroup_manager *cgroup_manager = ખાલી;
  પૂર્ણાંક ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_મૂળ, resources, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા update_cgroup_resources (cgroup_status->path, state_મૂળ, resources, ! cgroup_status->bpf_dev_set, err);
}

સ્થિર પૂર્ણાંક
can_ignore_cgroup_enter_errors (સંરચના libcrun_cgroup_args *args, પૂર્ણાંક cgroup_mode,
                                libcrun_error_t *err)
{
  પૂર્ણાંક manager = args->manager;
  પૂર્ણાંક rootless;

  rootless = is_rootless (err);
  જો (UNLIKELY (rootless < 0))
    પાછા rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  જો (! rootless)
    પાછા 0;

  જો (! is_empty_string (args->cgroup_path))
    પાછા 0;

  જો (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    પાછા 0;

  પાછા 1;
}

પૂર્ણાંક
libcrun_cgroup_preenter (સંરચના libcrun_cgroup_args *args, પૂર્ણાંક *dirfd, libcrun_error_t *err)
{
  સંરચના libcrun_cgroup_manager *cgroup_manager;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  જો (cgroup_mode != CGROUP_MODE_UNIFIED)
    પાછા 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (cgroup_manager->precreate_cgroup == ખાલી)
    પાછા 0;

  પાછા cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
શૂન્ય
libcrun_cgroup_preenter_cleanup (સંરચના libcrun_cgroup_args *args)
{
  જો (args->precreated_path == ખાલી)
    પાછા;

  (શૂન્ય) rmdir (args->precreated_path);
  મુક્ત (args->precreated_path);
  args->precreated_path = ખાલી;
}

પૂર્ણાંક
libcrun_cgroup_enter (સંરચના libcrun_cgroup_args *args, સંરચના libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status સંરચના libcrun_cgroup_status *status = xmalloc0 (કદ *status);
  સંરચના libcrun_cgroup_manager *cgroup_manager;
  uid_t મૂળ_uid = args->મૂળ_uid;
  uid_t મૂળ_gid = args->મૂળ_gid;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  જો (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free અક્ષર *buffer = ખાલી;

      ret = read_all_file (CGROUP_મૂળ "/unified/cgroup.controllers", &buffer, &len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      જો (len > 0)
        પાછા crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  જો (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ખાલી;
      પૂર્ણાંક ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      જો (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          પાછા ignore_cgroup_errors;
        }

      જો (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          મુક્ત (status->path);
          મુક્ત (status->scope);
          status->path = ખાલી;
          status->scope = ખાલી;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          જાઓ success;
        }

      પાછા ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  મુક્ત (args->precreated_path);
  args->precreated_path = ખાલી;

  જો (status->path)
    {
      bool need_chown;

      need_chown = મૂળ_uid != (uid_t) -1 || મૂળ_gid != (gid_t) -1;
      જો (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, મૂળ_uid, મૂળ_gid, err);
          જો (UNLIKELY (ret < 0))
            જાઓ fail_destroy;
        }

      જો (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_મૂળ, args->resources, ! status->bpf_dev_set, err);
          જો (UNLIKELY (ret < 0))
            જાઓ fail_destroy;
        }
    }
success:
  *out = status;
  status = ખાલી;
  પાછા 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = ખાલી;

    જો (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  પાછા ret;
}

પૂર્ણાંક
libcrun_cgroup_enter_finalize (સંરચના libcrun_cgroup_args *args, સંરચના libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free અક્ષર *target_cgroup = ખાલી;
  cleanup_free અક્ષર *content = ખાલી;
  અચલ અક્ષર *delegate_cgroup;
  cleanup_free અક્ષર *dir = ખાલી;
  અક્ષર *current_cgroup, *to;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  જો (delegate_cgroup == ખાલી)
    પાછા 0;

  જો (path_has_dot_dot_component (delegate_cgroup))
    પાછા crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  જો (cgroup_mode != CGROUP_MODE_UNIFIED)
    પાછા crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free અક્ષર *proc_path = ખાલી;
  cleanup_close પૂર્ણાંક fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  જો (UNLIKELY (fd < 0))
    પાછા fd;

  ret = read_all_fd (fd, "cgroup path", &content, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  current_cgroup = strstr (content, "0::");
  જો (UNLIKELY (current_cgroup == ખાલી))
    પાછા crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  જો (UNLIKELY (to == ખાલી))
    પાછા crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_paths (&dir, err, CGROUP_મૂળ, target_cgroup, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = move_process_to_cgroup (args->pid, ખાલી, target_cgroup, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = enable_controllers (target_cgroup, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = chown_cgroups (target_cgroup, args->મૂળ_uid, args->મૂળ_gid, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

પૂર્ણાંક
libcrun_cgroup_has_oom (સંરચના libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free અક્ષર *content = ખાલી;
  અચલ અક્ષર *path = status->path;
  અચલ અક્ષર *prefix = ખાલી;
  size_t content_size = 0;
  પૂર્ણાંક cgroup_mode;
  અક્ષર *it;

  જો (UNLIKELY (path == ખાલી || path[0] == '\0'))
    પાછા 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  પસંદગી (cgroup_mode)
    {
    સ્થિતિ CGROUP_MODE_UNIFIED:
      {
        cleanup_free અક્ષર *events_path = ખાલી;
        પૂર્ણાંક ret;

        ret = append_paths (&events_path, err, CGROUP_મૂળ, path, "memory.events", ખાલી);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        prefix = "oom ";
        તોડો;
      }
    સ્થિતિ CGROUP_MODE_LEGACY:
    સ્થિતિ CGROUP_MODE_HYBRID:
      {
        cleanup_free અક્ષર *oom_control_path = ખાલી;
        પૂર્ણાંક ret;

        ret = append_paths (&oom_control_path, err, CGROUP_મૂળ, "memory", path, "memory.oom_control", ખાલી);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        prefix = "oom_kill ";
        તોડો;
      }

    મૂળભૂત:
      પાછા crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  જ્યારે (it && *it)
    {
      જો (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          જ્યારે (*it == ' ')
            it++;

          પાછા *it != '0';
        }
      નહીંતર
        {
          it = strchr (it, '\n');
          જો (it == ખાલી)
            પાછા 0;
          it++;
        }
    }

  પાછા 0;
}

પૂર્ણાંક
libcrun_cgroup_get_status (સંરચના libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  પાછા 0;
}

શૂન્ય
libcrun_cgroup_status_free (સંરચના libcrun_cgroup_status *cgroup_status)
{
  જો (cgroup_status == ખાલી)
    પાછા;

  મુક્ત (cgroup_status->path);
  મુક્ત (cgroup_status->scope);
  મુક્ત (cgroup_status);
}

સંરચના libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  સંરચના libcrun_cgroup_status *ret;

  ret = xmalloc0 (કદ *ret);

  જો (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  જો (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  જો (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  નહીંતર
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  પાછા ret;
}
