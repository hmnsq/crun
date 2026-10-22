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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "cgroup.h"
#অন্তর্ভাব "cgroup-cgroupfs.h"
#অন্তর্ভাব "cgroup-internal.h"
#অন্তর্ভাব "cgroup-systemd.h"
#অন্তর্ভাব "cgroup-utils.h"
#অন্তর্ভাব "cgroup-setup.h"
#অন্তর্ভাব "cgroup-resources.h"
#অন্তর্ভাব "ebpf.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "status.h"
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <sched.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <sys/vfs.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব <time.h>

#অন্তর্ভাব <linux/magic.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <libgen.h>

স্থির পূর্ণসংখ্যা
libcrun_cgroup_enter_disabled (কাঠামো libcrun_cgroup_args *args arg_unused, কাঠামো libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = ফাঁকা;
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libcrun_destroy_cgroup_disabled (কাঠামো libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  ফেরত 0;
}

কাঠামো libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = ফাঁকা,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

স্থির পূর্ণসংখ্যা
get_cgroup_manager (পূর্ণসংখ্যা manager, কাঠামো libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  নির্বাচন (manager)
    {
    ক্ষেত্র CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      ফেরত 0;

    ক্ষেত্র CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      ফেরত 0;

    ক্ষেত্র CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      ফেরত 0;
    }

  *out = ফাঁকা;
  ফেরত crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

স্থির ধ্রুবক অক্ষর *
find_delegate_cgroup (string_map *annotations)
{
  ধ্রুবক অক্ষর *annotation;

  annotation = find_string_map_মান (annotations, "run.oci.delegate-cgroup");
  যদি (annotation)
    {
      যদি (annotation[0] == '\0')
        ফেরত ফাঁকা;
      ফেরত annotation;
    }

  ফেরত ফাঁকা;
}

পূর্ণসংখ্যা
libcrun_cgroup_pause_unpause (কাঠামো libcrun_cgroup_status *status, ধ্রুবক bool pause, libcrun_error_t *err)
{
  ফেরত libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

পূর্ণসংখ্যা
libcrun_cgroup_is_container_paused (কাঠামো libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  ধ্রুবক অক্ষর *cgroup_path = status->path;
  cleanup_free অক্ষর *content = ফাঁকা;
  cleanup_free অক্ষর *path = ফাঁকা;
  ধ্রুবক অক্ষর *state;
  পূর্ণসংখ্যা cgroup_mode;
  পূর্ণসংখ্যা ret;

  যদি (cgroup_path == ফাঁকা)
    ফেরত 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_মূল, cgroup_path, "cgroup.freeze", ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }
  অন্যথা
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_মূল "/freezer", cgroup_path, "freezer.state", ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  ret = read_all_file (path, &content, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      যদি (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        ফেরত ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      কাঠামো statfs freezer_stat;
      যদি (statfs (CGROUP_মূল "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          ফেরত crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_মূল "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      যদি (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        ফেরত ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      ফেরত 0;
    }

  *paused = strstr (content, state) != ফাঁকা;
  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_cgroup_join_process (কাঠামো libcrun_cgroup_status *status, ধ্রুবক অক্ষর *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  কাঠামো libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = ফাঁকা;
  পূর্ণসংখ্যা errno_;
  পূর্ণসংখ্যা ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  যদি (LIKELY (ret >= 0))
    ফেরত ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  যদি (errno_ != EACCES && errno_ != EPERM)
    ফেরত ret;

  যদি (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      ফেরত ret;
    }
  যদি (cgroup_manager->attach_process == ফাঁকা)
    ফেরত ret;

  যদি (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->বার্তা);
      crun_error_release (&tmp_err);
      ফেরত ret;
    }

  crun_error_release (err);
  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_cgroup_read_pids (কাঠামো libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ফেরত libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

পূর্ণসংখ্যা
libcrun_cgroup_killall (কাঠামো libcrun_cgroup_status *cgroup_status, পূর্ণসংখ্যা সংকেত, libcrun_error_t *err)
{
  ফেরত cgroup_killall_path (cgroup_status->path, সংকেত, err);
}

পূর্ণসংখ্যা
libcrun_cgroup_destroy (কাঠামো libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  কাঠামো libcrun_cgroup_manager *cgroup_manager = ফাঁকা;
  পূর্ণসংখ্যা ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
স্থির পূর্ণসংখ্যা
complete_cpu_max (ধ্রুবক অক্ষর *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free অক্ষর *cpu_max_path = ফাঁকা;
  cleanup_free অক্ষর *content = ফাঁকা;
  bool has_period;
  uint64_t period;
  int64_t quota;
  পূর্ণসংখ্যা cgroup_mode;
  পূর্ণসংখ্যা ret;

  যদি (resources == ফাঁকা || resources->cpu == ফাঁকা)
    ফেরত 0;

  cpu = resources->cpu;
  যদি (cpu->quota_present == cpu->period_present)
    ফেরত 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;
  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    ফেরত 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_মূল, path, "cpu.max", ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = read_all_file (cpu_max_path, &content, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      ফেরত 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  অন্যথা যদি (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_update_cgroup_resources (কাঠামো libcrun_cgroup_status *cgroup_status,
                                 ধ্রুবক অক্ষর *state_মূল,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  কাঠামো libcrun_cgroup_manager *cgroup_manager = ফাঁকা;
  পূর্ণসংখ্যা ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_মূল, resources, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }
  ফেরত update_cgroup_resources (cgroup_status->path, state_মূল, resources, ! cgroup_status->bpf_dev_set, err);
}

স্থির পূর্ণসংখ্যা
can_ignore_cgroup_enter_errors (কাঠামো libcrun_cgroup_args *args, পূর্ণসংখ্যা cgroup_mode,
                                libcrun_error_t *err)
{
  পূর্ণসংখ্যা manager = args->manager;
  পূর্ণসংখ্যা rootless;

  rootless = is_rootless (err);
  যদি (UNLIKELY (rootless < 0))
    ফেরত rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  যদি (! rootless)
    ফেরত 0;

  যদি (! is_empty_string (args->cgroup_path))
    ফেরত 0;

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    ফেরত 0;

  ফেরত 1;
}

পূর্ণসংখ্যা
libcrun_cgroup_preenter (কাঠামো libcrun_cgroup_args *args, পূর্ণসংখ্যা *dirfd, libcrun_error_t *err)
{
  কাঠামো libcrun_cgroup_manager *cgroup_manager;
  পূর্ণসংখ্যা cgroup_mode;
  পূর্ণসংখ্যা ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    ফেরত 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (cgroup_manager->precreate_cgroup == ফাঁকা)
    ফেরত 0;

  ফেরত cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
শূন্য
libcrun_cgroup_preenter_cleanup (কাঠামো libcrun_cgroup_args *args)
{
  যদি (args->precreated_path == ফাঁকা)
    ফেরত;

  (শূন্য) rmdir (args->precreated_path);
  মুক্ত (args->precreated_path);
  args->precreated_path = ফাঁকা;
}

পূর্ণসংখ্যা
libcrun_cgroup_enter (কাঠামো libcrun_cgroup_args *args, কাঠামো libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status কাঠামো libcrun_cgroup_status *status = xmalloc0 (আকার *status);
  কাঠামো libcrun_cgroup_manager *cgroup_manager;
  uid_t মূল_uid = args->মূল_uid;
  uid_t মূল_gid = args->মূল_gid;
  পূর্ণসংখ্যা cgroup_mode;
  পূর্ণসংখ্যা ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  যদি (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free অক্ষর *বাফার = ফাঁকা;

      ret = read_all_file (CGROUP_মূল "/unified/cgroup.controllers", &বাফার, &len, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
      যদি (len > 0)
        ফেরত crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  যদি (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ফাঁকা;
      পূর্ণসংখ্যা ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      যদি (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ফেরত ignore_cgroup_errors;
        }

      যদি (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          মুক্ত (status->path);
          মুক্ত (status->scope);
          status->path = ফাঁকা;
          status->scope = ফাঁকা;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          যাও success;
        }

      ফেরত ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  মুক্ত (args->precreated_path);
  args->precreated_path = ফাঁকা;

  যদি (status->path)
    {
      bool need_chown;

      need_chown = মূল_uid != (uid_t) -1 || মূল_gid != (gid_t) -1;
      যদি (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, মূল_uid, মূল_gid, err);
          যদি (UNLIKELY (ret < 0))
            যাও fail_destroy;
        }

      যদি (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_মূল, args->resources, ! status->bpf_dev_set, err);
          যদি (UNLIKELY (ret < 0))
            যাও fail_destroy;
        }
    }
success:
  *out = status;
  status = ফাঁকা;
  ফেরত 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = ফাঁকা;

    যদি (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  ফেরত ret;
}

পূর্ণসংখ্যা
libcrun_cgroup_enter_finalize (কাঠামো libcrun_cgroup_args *args, কাঠামো libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free অক্ষর *target_cgroup = ফাঁকা;
  cleanup_free অক্ষর *content = ফাঁকা;
  ধ্রুবক অক্ষর *delegate_cgroup;
  cleanup_free অক্ষর *dir = ফাঁকা;
  অক্ষর *current_cgroup, *to;
  পূর্ণসংখ্যা cgroup_mode;
  পূর্ণসংখ্যা ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  যদি (delegate_cgroup == ফাঁকা)
    ফেরত 0;

  যদি (path_has_dot_dot_component (delegate_cgroup))
    ফেরত crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    ফেরত crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free অক্ষর *proc_path = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  যদি (UNLIKELY (fd < 0))
    ফেরত fd;

  ret = read_all_fd (fd, "cgroup path", &content, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  current_cgroup = strstr (content, "0::");
  যদি (UNLIKELY (current_cgroup == ফাঁকা))
    ফেরত crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  যদি (UNLIKELY (to == ফাঁকা))
    ফেরত crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = append_paths (&dir, err, CGROUP_মূল, target_cgroup, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = move_process_to_cgroup (args->pid, ফাঁকা, target_cgroup, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = enable_controllers (target_cgroup, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = chown_cgroups (target_cgroup, args->মূল_uid, args->মূল_gid, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_cgroup_has_oom (কাঠামো libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free অক্ষর *content = ফাঁকা;
  ধ্রুবক অক্ষর *path = status->path;
  ধ্রুবক অক্ষর *prefix = ফাঁকা;
  size_t content_আকার = 0;
  পূর্ণসংখ্যা cgroup_mode;
  অক্ষর *it;

  যদি (UNLIKELY (path == ফাঁকা || path[0] == '\0'))
    ফেরত 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  নির্বাচন (cgroup_mode)
    {
    ক্ষেত্র CGROUP_MODE_UNIFIED:
      {
        cleanup_free অক্ষর *events_path = ফাঁকা;
        পূর্ণসংখ্যা ret;

        ret = append_paths (&events_path, err, CGROUP_মূল, path, "memory.events", ফাঁকা);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_আকার, err);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        prefix = "oom ";
        বিরতি;
      }
    ক্ষেত্র CGROUP_MODE_LEGACY:
    ক্ষেত্র CGROUP_MODE_HYBRID:
      {
        cleanup_free অক্ষর *oom_control_path = ফাঁকা;
        পূর্ণসংখ্যা ret;

        ret = append_paths (&oom_control_path, err, CGROUP_মূল, "memory", path, "memory.oom_control", ফাঁকা);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_আকার, err);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        prefix = "oom_kill ";
        বিরতি;
      }

    পূর্বনির্ধারিত:
      ফেরত crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  যখন (it && *it)
    {
      যদি (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          যখন (*it == ' ')
            it++;

          ফেরত *it != '0';
        }
      অন্যথা
        {
          it = strchr (it, '\n');
          যদি (it == ফাঁকা)
            ফেরত 0;
          it++;
        }
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_cgroup_get_status (কাঠামো libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  ফেরত 0;
}

শূন্য
libcrun_cgroup_status_free (কাঠামো libcrun_cgroup_status *cgroup_status)
{
  যদি (cgroup_status == ফাঁকা)
    ফেরত;

  মুক্ত (cgroup_status->path);
  মুক্ত (cgroup_status->scope);
  মুক্ত (cgroup_status);
}

কাঠামো libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  কাঠামো libcrun_cgroup_status *ret;

  ret = xmalloc0 (আকার *ret);

  যদি (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  যদি (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  যদি (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  অন্যথা
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  ফেরত ret;
}
