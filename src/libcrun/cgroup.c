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

#ضمن <config.h>
#ضمن "cgroup.h"
#ضمن "cgroup-cgroupfs.h"
#ضمن "cgroup-internal.h"
#ضمن "cgroup-systemd.h"
#ضمن "cgroup-utils.h"
#ضمن "cgroup-setup.h"
#ضمن "cgroup-resources.h"
#ضمن "ebpf.h"
#ضمن "utils.h"
#ضمن "status.h"
#ضمن <string.h>
#ضمن <sched.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <sys/vfs.h>
#ضمن <inttypes.h>
#ضمن <time.h>

#ضمن <linux/magic.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <libgen.h>

ساكن صحيح
libcrun_cgroup_enter_disabled (بنية libcrun_cgroup_args *args arg_unused, بنية libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = عدم;
  החזר 0;
}

ساكن صحيح
libcrun_destroy_cgroup_disabled (بنية libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  החזר 0;
}

بنية libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = عدم,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

ساكن صحيح
get_cgroup_manager (صحيح manager, بنية libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  بدل (manager)
    {
    حالة CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      החזר 0;

    حالة CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      החזר 0;

    حالة CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      החזר 0;
    }

  *out = عدم;
  החזר crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

ساكن ثابت حرف *
find_delegate_cgroup (string_map *annotations)
{
  ثابت حرف *annotation;

  annotation = find_string_map_value (annotations, "run.oci.delegate-cgroup");
  אם (annotation)
    {
      אם (annotation[0] == '\0')
        החזר عدم;
      החזר annotation;
    }

  החזר عدم;
}

صحيح
libcrun_cgroup_pause_unpause (بنية libcrun_cgroup_status *status, ثابت bool pause, libcrun_error_t *err)
{
  החזר libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

صحيح
libcrun_cgroup_is_container_paused (بنية libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  ثابت حرف *cgroup_path = status->path;
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *path = عدم;
  ثابت حرف *state;
  صحيح cgroup_mode;
  صحيح ret;

  אם (cgroup_path == عدم)
    החזר 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  אם (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אחרת
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  ret = read_all_file (path, &content, عدم, err);
  אם (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      אם (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        החזר ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      بنية statfs freezer_stat;
      אם (statfs (CGROUP_ROOT "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          החזר crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ROOT "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      אם (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        החזר ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      החזר 0;
    }

  *paused = strstr (content, state) != عدم;
  החזר 0;
}

صحيح
libcrun_cgroup_join_process (بنية libcrun_cgroup_status *status, ثابت حرف *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = عدم;
  صحيح errno_;
  صحيح ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  אם (LIKELY (ret >= 0))
    החזר ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  אם (errno_ != EACCES && errno_ != EPERM)
    החזר ret;

  אם (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      החזר ret;
    }
  אם (cgroup_manager->attach_process == عدم)
    החזר ret;

  אם (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->msg);
      crun_error_release (&tmp_err);
      החזר ret;
    }

  crun_error_release (err);
  החזר 0;
}

صحيح
libcrun_cgroup_read_pids (بنية libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  החזר libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

صحيح
libcrun_cgroup_killall (بنية libcrun_cgroup_status *cgroup_status, صحيح إشارة, libcrun_error_t *err)
{
  החזר cgroup_killall_path (cgroup_status->path, إشارة, err);
}

صحيح
libcrun_cgroup_destroy (بنية libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager = عدم;
  صحيح ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
ساكن صحيح
complete_cpu_max (ثابت حرف *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free حرف *cpu_max_path = عدم;
  cleanup_free حرف *content = عدم;
  bool has_period;
  uint64_t period;
  int64_t quota;
  صحيح cgroup_mode;
  صحيح ret;

  אם (resources == عدم || resources->cpu == عدم)
    החזר 0;

  cpu = resources->cpu;
  אם (cpu->quota_present == cpu->period_present)
    החזר 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;
  אם (cgroup_mode != CGROUP_MODE_UNIFIED)
    החזר 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_ROOT, path, "cpu.max", عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = read_all_file (cpu_max_path, &content, عدم, err);
  אם (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      החזר 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  אחרת אם (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  החזר 0;
}

صحيح
libcrun_update_cgroup_resources (بنية libcrun_cgroup_status *cgroup_status,
                                 ثابت حرف *state_root,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager = عدم;
  صحيح ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_root, resources, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  החזר update_cgroup_resources (cgroup_status->path, state_root, resources, ! cgroup_status->bpf_dev_set, err);
}

ساكن صحيح
can_ignore_cgroup_enter_errors (بنية libcrun_cgroup_args *args, صحيح cgroup_mode,
                                libcrun_error_t *err)
{
  صحيح manager = args->manager;
  صحيح rootless;

  rootless = is_rootless (err);
  אם (UNLIKELY (rootless < 0))
    החזר rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  אם (! rootless)
    החזר 0;

  אם (! is_empty_string (args->cgroup_path))
    החזר 0;

  אם (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    החזר 0;

  החזר 1;
}

صحيح
libcrun_cgroup_preenter (بنية libcrun_cgroup_args *args, صحيح *dirfd, libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager;
  صحيح cgroup_mode;
  صحيح ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  אם (cgroup_mode != CGROUP_MODE_UNIFIED)
    החזר 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (cgroup_manager->precreate_cgroup == عدم)
    החזר 0;

  החזר cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
فراغ
libcrun_cgroup_preenter_cleanup (بنية libcrun_cgroup_args *args)
{
  אם (args->precreated_path == عدم)
    החזר;

  (فراغ) rmdir (args->precreated_path);
  حرر (args->precreated_path);
  args->precreated_path = عدم;
}

صحيح
libcrun_cgroup_enter (بنية libcrun_cgroup_args *args, بنية libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status بنية libcrun_cgroup_status *status = xmalloc0 (حجم *status);
  بنية libcrun_cgroup_manager *cgroup_manager;
  uid_t root_uid = args->root_uid;
  uid_t root_gid = args->root_gid;
  صحيح cgroup_mode;
  صحيح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  אם (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free حرف *buffer = عدم;

      ret = read_all_file (CGROUP_ROOT "/unified/cgroup.controllers", &buffer, &len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
      אם (len > 0)
        החזר crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  אם (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = عدم;
      صحيح ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      אם (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          החזר ignore_cgroup_errors;
        }

      אם (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          حرر (status->path);
          حرر (status->scope);
          status->path = عدم;
          status->scope = عدم;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          اذهب success;
        }

      החזר ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  حرر (args->precreated_path);
  args->precreated_path = عدم;

  אם (status->path)
    {
      bool need_chown;

      need_chown = root_uid != (uid_t) -1 || root_gid != (gid_t) -1;
      אם (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, root_uid, root_gid, err);
          אם (UNLIKELY (ret < 0))
            اذهب fail_destroy;
        }

      אם (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_root, args->resources, ! status->bpf_dev_set, err);
          אם (UNLIKELY (ret < 0))
            اذهب fail_destroy;
        }
    }
success:
  *out = status;
  status = عدم;
  החזר 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = عدم;

    אם (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  החזר ret;
}

صحيح
libcrun_cgroup_enter_finalize (بنية libcrun_cgroup_args *args, بنية libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free حرف *target_cgroup = عدم;
  cleanup_free حرف *content = عدم;
  ثابت حرف *delegate_cgroup;
  cleanup_free حرف *dir = عدم;
  حرف *current_cgroup, *to;
  صحيح cgroup_mode;
  صحيح ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  אם (delegate_cgroup == عدم)
    החזר 0;

  אם (path_has_dot_dot_component (delegate_cgroup))
    החזר crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  אם (cgroup_mode != CGROUP_MODE_UNIFIED)
    החזר crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free حرف *proc_path = عدم;
  cleanup_close صحيح fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  אם (UNLIKELY (fd < 0))
    החזר fd;

  ret = read_all_fd (fd, "cgroup path", &content, عدم, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  current_cgroup = strstr (content, "0::");
  אם (UNLIKELY (current_cgroup == عدم))
    החזר crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  אם (UNLIKELY (to == عدم))
    החזר crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = append_paths (&dir, err, CGROUP_ROOT, target_cgroup, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = move_process_to_cgroup (args->pid, عدم, target_cgroup, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = enable_controllers (target_cgroup, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = chown_cgroups (target_cgroup, args->root_uid, args->root_gid, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר 0;
}

صحيح
libcrun_cgroup_has_oom (بنية libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  ثابت حرف *path = status->path;
  ثابت حرف *prefix = عدم;
  size_t content_size = 0;
  صحيح cgroup_mode;
  حرف *it;

  אם (UNLIKELY (path == عدم || path[0] == '\0'))
    החזר 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  بدل (cgroup_mode)
    {
    حالة CGROUP_MODE_UNIFIED:
      {
        cleanup_free حرف *events_path = عدم;
        صحيح ret;

        ret = append_paths (&events_path, err, CGROUP_ROOT, path, "memory.events", عدم);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        prefix = "oom ";
        اكسر;
      }
    حالة CGROUP_MODE_LEGACY:
    حالة CGROUP_MODE_HYBRID:
      {
        cleanup_free حرف *oom_control_path = عدم;
        صحيح ret;

        ret = append_paths (&oom_control_path, err, CGROUP_ROOT, "memory", path, "memory.oom_control", عدم);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        prefix = "oom_kill ";
        اكسر;
      }

    افتراضي:
      החזר crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  כלעוד (it && *it)
    {
      אם (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          כלעוד (*it == ' ')
            it++;

          החזר *it != '0';
        }
      אחרת
        {
          it = strchr (it, '\n');
          אם (it == عدم)
            החזר 0;
          it++;
        }
    }

  החזר 0;
}

صحيح
libcrun_cgroup_get_status (بنية libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  החזר 0;
}

فراغ
libcrun_cgroup_status_free (بنية libcrun_cgroup_status *cgroup_status)
{
  אם (cgroup_status == عدم)
    החזר;

  حرر (cgroup_status->path);
  حرر (cgroup_status->scope);
  حرر (cgroup_status);
}

بنية libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  بنية libcrun_cgroup_status *ret;

  ret = xmalloc0 (حجم *ret);

  אם (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  אם (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  אם (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  אחרת
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  החזר ret;
}
