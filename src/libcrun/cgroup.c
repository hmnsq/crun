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

স্থির পূর্ণাঙ্ক
libcrun_cgroup_enter_disabled (সংরচনা libcrun_cgroup_args *args arg_unused, সংরচনা libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = রিক্ত;
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
libcrun_destroy_cgroup_disabled (সংরচনা libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  প্রত্যাগম 0;
}

সংরচনা libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = রিক্ত,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

স্থির পূর্ণাঙ্ক
get_cgroup_manager (পূর্ণাঙ্ক manager, সংরচনা libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  বিকল্প (manager)
    {
    পক্ষ CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      প্রত্যাগম 0;

    পক্ষ CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      প্রত্যাগম 0;

    পক্ষ CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      প্রত্যাগম 0;
    }

  *out = রিক্ত;
  প্রত্যাগম crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

স্থির ধ্রুব বর্ণ *
find_delegate_cgroup (string_map *annotations)
{
  ধ্রুব বর্ণ *annotation;

  annotation = find_string_map_মান (annotations, "run.oci.delegate-cgroup");
  যদি (annotation)
    {
      যদি (annotation[0] == '\0')
        প্রত্যাগম রিক্ত;
      প্রত্যাগম annotation;
    }

  প্রত্যাগম রিক্ত;
}

পূর্ণাঙ্ক
libcrun_cgroup_pause_unpause (সংরচনা libcrun_cgroup_status *status, ধ্রুব bool pause, libcrun_error_t *err)
{
  প্রত্যাগম libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

পূর্ণাঙ্ক
libcrun_cgroup_is_container_paused (সংরচনা libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  ধ্রুব বর্ণ *cgroup_path = status->path;
  cleanup_free বর্ণ *content = রিক্ত;
  cleanup_free বর্ণ *path = রিক্ত;
  ধ্রুব বর্ণ *state;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক ret;

  যদি (cgroup_path == রিক্ত)
    প্রত্যাগম 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_মূল, cgroup_path, "cgroup.freeze", রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  অন্যথা
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_মূল "/freezer", cgroup_path, "freezer.state", রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  ret = read_all_file (path, &content, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      যদি (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        প্রত্যাগম ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      সংরচনা statfs freezer_stat;
      যদি (statfs (CGROUP_মূল "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          প্রত্যাগম crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_মূল "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      যদি (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        প্রত্যাগম ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      প্রত্যাগম 0;
    }

  *paused = strstr (content, state) != রিক্ত;
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_cgroup_join_process (সংরচনা libcrun_cgroup_status *status, ধ্রুব বর্ণ *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  সংরচনা libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = রিক্ত;
  পূর্ণাঙ্ক errno_;
  পূর্ণাঙ্ক ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  যদি (LIKELY (ret >= 0))
    প্রত্যাগম ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  যদি (errno_ != EACCES && errno_ != EPERM)
    প্রত্যাগম ret;

  যদি (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      প্রত্যাগম ret;
    }
  যদি (cgroup_manager->attach_process == রিক্ত)
    প্রত্যাগম ret;

  যদি (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->বার্তা);
      crun_error_release (&tmp_err);
      প্রত্যাগম ret;
    }

  crun_error_release (err);
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_cgroup_read_pids (সংরচনা libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  প্রত্যাগম libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

পূর্ণাঙ্ক
libcrun_cgroup_killall (সংরচনা libcrun_cgroup_status *cgroup_status, পূর্ণাঙ্ক সঙ্কেত, libcrun_error_t *err)
{
  প্রত্যাগম cgroup_killall_path (cgroup_status->path, সঙ্কেত, err);
}

পূর্ণাঙ্ক
libcrun_cgroup_destroy (সংরচনা libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  সংরচনা libcrun_cgroup_manager *cgroup_manager = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
স্থির পূর্ণাঙ্ক
complete_cpu_max (ধ্রুব বর্ণ *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free বর্ণ *cpu_max_path = রিক্ত;
  cleanup_free বর্ণ *content = রিক্ত;
  bool has_period;
  uint64_t period;
  int64_t quota;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক ret;

  যদি (resources == রিক্ত || resources->cpu == রিক্ত)
    প্রত্যাগম 0;

  cpu = resources->cpu;
  যদি (cpu->quota_present == cpu->period_present)
    প্রত্যাগম 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;
  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    প্রত্যাগম 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_মূল, path, "cpu.max", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = read_all_file (cpu_max_path, &content, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      প্রত্যাগম 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

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

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_update_cgroup_resources (সংরচনা libcrun_cgroup_status *cgroup_status,
                                 ধ্রুব বর্ণ *state_মূল,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  সংরচনা libcrun_cgroup_manager *cgroup_manager = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_মূল, resources, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  প্রত্যাগম update_cgroup_resources (cgroup_status->path, state_মূল, resources, ! cgroup_status->bpf_dev_set, err);
}

স্থির পূর্ণাঙ্ক
can_ignore_cgroup_enter_errors (সংরচনা libcrun_cgroup_args *args, পূর্ণাঙ্ক cgroup_mode,
                                libcrun_error_t *err)
{
  পূর্ণাঙ্ক manager = args->manager;
  পূর্ণাঙ্ক rootless;

  rootless = is_rootless (err);
  যদি (UNLIKELY (rootless < 0))
    প্রত্যাগম rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  যদি (! rootless)
    প্রত্যাগম 0;

  যদি (! is_empty_string (args->cgroup_path))
    প্রত্যাগম 0;

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    প্রত্যাগম 0;

  প্রত্যাগম 1;
}

পূর্ণাঙ্ক
libcrun_cgroup_preenter (সংরচনা libcrun_cgroup_args *args, পূর্ণাঙ্ক *dirfd, libcrun_error_t *err)
{
  সংরচনা libcrun_cgroup_manager *cgroup_manager;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    প্রত্যাগম 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (cgroup_manager->precreate_cgroup == রিক্ত)
    প্রত্যাগম 0;

  প্রত্যাগম cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
শূন্য
libcrun_cgroup_preenter_cleanup (সংরচনা libcrun_cgroup_args *args)
{
  যদি (args->precreated_path == রিক্ত)
    প্রত্যাগম;

  (শূন্য) rmdir (args->precreated_path);
  মুক্ত (args->precreated_path);
  args->precreated_path = রিক্ত;
}

পূর্ণাঙ্ক
libcrun_cgroup_enter (সংরচনা libcrun_cgroup_args *args, সংরচনা libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status সংরচনা libcrun_cgroup_status *status = xmalloc0 (পরিমাণ *status);
  সংরচনা libcrun_cgroup_manager *cgroup_manager;
  uid_t মূল_uid = args->মূল_uid;
  uid_t মূল_gid = args->মূল_gid;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  যদি (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free বর্ণ *buffer = রিক্ত;

      ret = read_all_file (CGROUP_মূল "/unified/cgroup.controllers", &buffer, &len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      যদি (len > 0)
        প্রত্যাগম crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  যদি (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = রিক্ত;
      পূর্ণাঙ্ক ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      যদি (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          প্রত্যাগম ignore_cgroup_errors;
        }

      যদি (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          মুক্ত (status->path);
          মুক্ত (status->scope);
          status->path = রিক্ত;
          status->scope = রিক্ত;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          গচ্ছ success;
        }

      প্রত্যাগম ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  মুক্ত (args->precreated_path);
  args->precreated_path = রিক্ত;

  যদি (status->path)
    {
      bool need_chown;

      need_chown = মূল_uid != (uid_t) -1 || মূল_gid != (gid_t) -1;
      যদি (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, মূল_uid, মূল_gid, err);
          যদি (UNLIKELY (ret < 0))
            গচ্ছ fail_destroy;
        }

      যদি (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_মূল, args->resources, ! status->bpf_dev_set, err);
          যদি (UNLIKELY (ret < 0))
            গচ্ছ fail_destroy;
        }
    }
success:
  *out = status;
  status = রিক্ত;
  প্রত্যাগম 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = রিক্ত;

    যদি (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
libcrun_cgroup_enter_finalize (সংরচনা libcrun_cgroup_args *args, সংরচনা libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free বর্ণ *target_cgroup = রিক্ত;
  cleanup_free বর্ণ *content = রিক্ত;
  ধ্রুব বর্ণ *delegate_cgroup;
  cleanup_free বর্ণ *dir = রিক্ত;
  বর্ণ *current_cgroup, *to;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  যদি (delegate_cgroup == রিক্ত)
    প্রত্যাগম 0;

  যদি (path_has_dot_dot_component (delegate_cgroup))
    প্রত্যাগম crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    প্রত্যাগম crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free বর্ণ *proc_path = রিক্ত;
  cleanup_close পূর্ণাঙ্ক fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম fd;

  ret = read_all_fd (fd, "cgroup path", &content, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  current_cgroup = strstr (content, "0::");
  যদি (UNLIKELY (current_cgroup == রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  যদি (UNLIKELY (to == রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = append_paths (&dir, err, CGROUP_মূল, target_cgroup, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = move_process_to_cgroup (args->pid, রিক্ত, target_cgroup, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = enable_controllers (target_cgroup, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = chown_cgroups (target_cgroup, args->মূল_uid, args->মূল_gid, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_cgroup_has_oom (সংরচনা libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free বর্ণ *content = রিক্ত;
  ধ্রুব বর্ণ *path = status->path;
  ধ্রুব বর্ণ *prefix = রিক্ত;
  size_t content_size = 0;
  পূর্ণাঙ্ক cgroup_mode;
  বর্ণ *it;

  যদি (UNLIKELY (path == রিক্ত || path[0] == '\0'))
    প্রত্যাগম 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  বিকল্প (cgroup_mode)
    {
    পক্ষ CGROUP_MODE_UNIFIED:
      {
        cleanup_free বর্ণ *events_path = রিক্ত;
        পূর্ণাঙ্ক ret;

        ret = append_paths (&events_path, err, CGROUP_মূল, path, "memory.events", রিক্ত);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        prefix = "oom ";
        ভঙ্গ;
      }
    পক্ষ CGROUP_MODE_LEGACY:
    পক্ষ CGROUP_MODE_HYBRID:
      {
        cleanup_free বর্ণ *oom_control_path = রিক্ত;
        পূর্ণাঙ্ক ret;

        ret = append_paths (&oom_control_path, err, CGROUP_মূল, "memory", path, "memory.oom_control", রিক্ত);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        prefix = "oom_kill ";
        ভঙ্গ;
      }

    স্বভাব:
      প্রত্যাগম crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  যাবত্ (it && *it)
    {
      যদি (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          যাবত্ (*it == ' ')
            it++;

          প্রত্যাগম *it != '0';
        }
      অন্যথা
        {
          it = strchr (it, '\n');
          যদি (it == রিক্ত)
            প্রত্যাগম 0;
          it++;
        }
    }

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_cgroup_get_status (সংরচনা libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  প্রত্যাগম 0;
}

শূন্য
libcrun_cgroup_status_free (সংরচনা libcrun_cgroup_status *cgroup_status)
{
  যদি (cgroup_status == রিক্ত)
    প্রত্যাগম;

  মুক্ত (cgroup_status->path);
  মুক্ত (cgroup_status->scope);
  মুক্ত (cgroup_status);
}

সংরচনা libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  সংরচনা libcrun_cgroup_status *ret;

  ret = xmalloc0 (পরিমাণ *ret);

  যদি (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  যদি (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  যদি (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  অন্যথা
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  প্রত্যাগম ret;
}
