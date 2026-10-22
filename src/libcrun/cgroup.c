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

#شامل <config.h>
#شامل "cgroup.h"
#شامل "cgroup-cgroupfs.h"
#شامل "cgroup-internal.h"
#شامل "cgroup-systemd.h"
#شامل "cgroup-utils.h"
#شامل "cgroup-setup.h"
#شامل "cgroup-resources.h"
#شامل "ebpf.h"
#شامل "utils.h"
#شامل "status.h"
#شامل <string.h>
#شامل <sched.h>
#شامل <sys/types.h>
#شامل <signal.h>
#شامل <sys/vfs.h>
#شامل <inttypes.h>
#شامل <time.h>

#شامل <linux/magic.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <libgen.h>

ایستا صحیح
libcrun_cgroup_enter_disabled (ساختار libcrun_cgroup_args *args arg_unused, ساختار libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = پوچ;
  بازگشت 0;
}

ایستا صحیح
libcrun_destroy_cgroup_disabled (ساختار libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  بازگشت 0;
}

ساختار libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = پوچ,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

ایستا صحیح
get_cgroup_manager (صحیح manager, ساختار libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  انتخاب (manager)
    {
    حالت CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      بازگشت 0;

    حالت CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      بازگشت 0;

    حالت CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      بازگشت 0;
    }

  *out = پوچ;
  بازگشت crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

ایستا ثابت نویسه *
find_delegate_cgroup (string_map *annotations)
{
  ثابت نویسه *annotation;

  annotation = find_string_map_مقدار (annotations, "run.oci.delegate-cgroup");
  اگر (annotation)
    {
      اگر (annotation[0] == '\0')
        بازگشت پوچ;
      بازگشت annotation;
    }

  بازگشت پوچ;
}

صحیح
libcrun_cgroup_pause_unpause (ساختار libcrun_cgroup_status *status, ثابت bool pause, libcrun_error_t *err)
{
  بازگشت libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

صحیح
libcrun_cgroup_is_container_paused (ساختار libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  ثابت نویسه *cgroup_path = status->path;
  cleanup_free نویسه *content = پوچ;
  cleanup_free نویسه *path = پوچ;
  ثابت نویسه *state;
  صحیح cgroup_mode;
  صحیح ret;

  اگر (cgroup_path == پوچ)
    بازگشت 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  اگر (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_ریشه, cgroup_path, "cgroup.freeze", پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_ریشه "/freezer", cgroup_path, "freezer.state", پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = read_all_file (path, &content, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      اگر (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        بازگشت ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      ساختار statfs freezer_stat;
      اگر (statfs (CGROUP_ریشه "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          بازگشت crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ریشه "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      اگر (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        بازگشت ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      بازگشت 0;
    }

  *paused = strstr (content, state) != پوچ;
  بازگشت 0;
}

صحیح
libcrun_cgroup_join_process (ساختار libcrun_cgroup_status *status, ثابت نویسه *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  ساختار libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = پوچ;
  صحیح errno_;
  صحیح ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  اگر (LIKELY (ret >= 0))
    بازگشت ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  اگر (errno_ != EACCES && errno_ != EPERM)
    بازگشت ret;

  اگر (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      بازگشت ret;
    }
  اگر (cgroup_manager->attach_process == پوچ)
    بازگشت ret;

  اگر (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->پیام);
      crun_error_release (&tmp_err);
      بازگشت ret;
    }

  crun_error_release (err);
  بازگشت 0;
}

صحیح
libcrun_cgroup_read_pids (ساختار libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  بازگشت libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

صحیح
libcrun_cgroup_killall (ساختار libcrun_cgroup_status *cgroup_status, صحیح سیگنال, libcrun_error_t *err)
{
  بازگشت cgroup_killall_path (cgroup_status->path, سیگنال, err);
}

صحیح
libcrun_cgroup_destroy (ساختار libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  ساختار libcrun_cgroup_manager *cgroup_manager = پوچ;
  صحیح ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
ایستا صحیح
complete_cpu_max (ثابت نویسه *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free نویسه *cpu_max_path = پوچ;
  cleanup_free نویسه *content = پوچ;
  bool has_period;
  uint64_t period;
  int64_t quota;
  صحیح cgroup_mode;
  صحیح ret;

  اگر (resources == پوچ || resources->cpu == پوچ)
    بازگشت 0;

  cpu = resources->cpu;
  اگر (cpu->quota_present == cpu->period_present)
    بازگشت 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;
  اگر (cgroup_mode != CGROUP_MODE_UNIFIED)
    بازگشت 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_ریشه, path, "cpu.max", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = read_all_file (cpu_max_path, &content, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      بازگشت 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  وگرنه اگر (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  بازگشت 0;
}

صحیح
libcrun_update_cgroup_resources (ساختار libcrun_cgroup_status *cgroup_status,
                                 ثابت نویسه *state_ریشه,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  ساختار libcrun_cgroup_manager *cgroup_manager = پوچ;
  صحیح ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_ریشه, resources, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت update_cgroup_resources (cgroup_status->path, state_ریشه, resources, ! cgroup_status->bpf_dev_set, err);
}

ایستا صحیح
can_ignore_cgroup_enter_errors (ساختار libcrun_cgroup_args *args, صحیح cgroup_mode,
                                libcrun_error_t *err)
{
  صحیح manager = args->manager;
  صحیح rootless;

  rootless = is_rootless (err);
  اگر (UNLIKELY (rootless < 0))
    بازگشت rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  اگر (! rootless)
    بازگشت 0;

  اگر (! is_empty_string (args->cgroup_path))
    بازگشت 0;

  اگر (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    بازگشت 0;

  بازگشت 1;
}

صحیح
libcrun_cgroup_preenter (ساختار libcrun_cgroup_args *args, صحیح *dirfd, libcrun_error_t *err)
{
  ساختار libcrun_cgroup_manager *cgroup_manager;
  صحیح cgroup_mode;
  صحیح ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  اگر (cgroup_mode != CGROUP_MODE_UNIFIED)
    بازگشت 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (cgroup_manager->precreate_cgroup == پوچ)
    بازگشت 0;

  بازگشت cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
تهی
libcrun_cgroup_preenter_cleanup (ساختار libcrun_cgroup_args *args)
{
  اگر (args->precreated_path == پوچ)
    بازگشت;

  (تهی) rmdir (args->precreated_path);
  آزاد (args->precreated_path);
  args->precreated_path = پوچ;
}

صحیح
libcrun_cgroup_enter (ساختار libcrun_cgroup_args *args, ساختار libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status ساختار libcrun_cgroup_status *status = xmalloc0 (اندازه *status);
  ساختار libcrun_cgroup_manager *cgroup_manager;
  uid_t ریشه_uid = args->ریشه_uid;
  uid_t ریشه_gid = args->ریشه_gid;
  صحیح cgroup_mode;
  صحیح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  اگر (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free نویسه *buffer = پوچ;

      ret = read_all_file (CGROUP_ریشه "/unified/cgroup.controllers", &buffer, &len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      اگر (len > 0)
        بازگشت crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  اگر (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = پوچ;
      صحیح ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      اگر (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          بازگشت ignore_cgroup_errors;
        }

      اگر (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          آزاد (status->path);
          آزاد (status->scope);
          status->path = پوچ;
          status->scope = پوچ;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          برو success;
        }

      بازگشت ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  آزاد (args->precreated_path);
  args->precreated_path = پوچ;

  اگر (status->path)
    {
      bool need_chown;

      need_chown = ریشه_uid != (uid_t) -1 || ریشه_gid != (gid_t) -1;
      اگر (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, ریشه_uid, ریشه_gid, err);
          اگر (UNLIKELY (ret < 0))
            برو fail_destroy;
        }

      اگر (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_ریشه, args->resources, ! status->bpf_dev_set, err);
          اگر (UNLIKELY (ret < 0))
            برو fail_destroy;
        }
    }
success:
  *out = status;
  status = پوچ;
  بازگشت 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = پوچ;

    اگر (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  بازگشت ret;
}

صحیح
libcrun_cgroup_enter_finalize (ساختار libcrun_cgroup_args *args, ساختار libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free نویسه *target_cgroup = پوچ;
  cleanup_free نویسه *content = پوچ;
  ثابت نویسه *delegate_cgroup;
  cleanup_free نویسه *dir = پوچ;
  نویسه *current_cgroup, *to;
  صحیح cgroup_mode;
  صحیح ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  اگر (delegate_cgroup == پوچ)
    بازگشت 0;

  اگر (path_has_dot_dot_component (delegate_cgroup))
    بازگشت crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  اگر (cgroup_mode != CGROUP_MODE_UNIFIED)
    بازگشت crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free نویسه *proc_path = پوچ;
  cleanup_close صحیح fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  اگر (UNLIKELY (fd < 0))
    بازگشت fd;

  ret = read_all_fd (fd, "cgroup path", &content, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  current_cgroup = strstr (content, "0::");
  اگر (UNLIKELY (current_cgroup == پوچ))
    بازگشت crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  اگر (UNLIKELY (to == پوچ))
    بازگشت crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_paths (&dir, err, CGROUP_ریشه, target_cgroup, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = move_process_to_cgroup (args->pid, پوچ, target_cgroup, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = enable_controllers (target_cgroup, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = chown_cgroups (target_cgroup, args->ریشه_uid, args->ریشه_gid, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

صحیح
libcrun_cgroup_has_oom (ساختار libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free نویسه *content = پوچ;
  ثابت نویسه *path = status->path;
  ثابت نویسه *prefix = پوچ;
  size_t content_size = 0;
  صحیح cgroup_mode;
  نویسه *it;

  اگر (UNLIKELY (path == پوچ || path[0] == '\0'))
    بازگشت 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  انتخاب (cgroup_mode)
    {
    حالت CGROUP_MODE_UNIFIED:
      {
        cleanup_free نویسه *events_path = پوچ;
        صحیح ret;

        ret = append_paths (&events_path, err, CGROUP_ریشه, path, "memory.events", پوچ);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        prefix = "oom ";
        بشکن;
      }
    حالت CGROUP_MODE_LEGACY:
    حالت CGROUP_MODE_HYBRID:
      {
        cleanup_free نویسه *oom_control_path = پوچ;
        صحیح ret;

        ret = append_paths (&oom_control_path, err, CGROUP_ریشه, "memory", path, "memory.oom_control", پوچ);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        prefix = "oom_kill ";
        بشکن;
      }

    پیشفرض:
      بازگشت crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  وقتی (it && *it)
    {
      اگر (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          وقتی (*it == ' ')
            it++;

          بازگشت *it != '0';
        }
      وگرنه
        {
          it = strchr (it, '\n');
          اگر (it == پوچ)
            بازگشت 0;
          it++;
        }
    }

  بازگشت 0;
}

صحیح
libcrun_cgroup_get_status (ساختار libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  بازگشت 0;
}

تهی
libcrun_cgroup_status_free (ساختار libcrun_cgroup_status *cgroup_status)
{
  اگر (cgroup_status == پوچ)
    بازگشت;

  آزاد (cgroup_status->path);
  آزاد (cgroup_status->scope);
  آزاد (cgroup_status);
}

ساختار libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  ساختار libcrun_cgroup_status *ret;

  ret = xmalloc0 (اندازه *ret);

  اگر (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  اگر (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  اگر (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  وگرنه
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  بازگشت ret;
}
