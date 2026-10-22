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

#अन्तर्भाव <config.h>
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव "cgroup-cgroupfs.h"
#अन्तर्भाव "cgroup-internal.h"
#अन्तर्भाव "cgroup-systemd.h"
#अन्तर्भाव "cgroup-utils.h"
#अन्तर्भाव "cgroup-setup.h"
#अन्तर्भाव "cgroup-resources.h"
#अन्तर्भाव "ebpf.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "status.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव <linux/magic.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <libgen.h>

स्थिर पूर्णाङ्क
libcrun_cgroup_enter_disabled (संरचना libcrun_cgroup_args *args arg_unused, संरचना libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = रिक्त;
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
libcrun_destroy_cgroup_disabled (संरचना libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  फर्काऊ 0;
}

संरचना libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = रिक्त,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

स्थिर पूर्णाङ्क
get_cgroup_manager (पूर्णाङ्क manager, संरचना libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  विकल्प (manager)
    {
    पक्ष CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      फर्काऊ 0;

    पक्ष CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      फर्काऊ 0;

    पक्ष CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      फर्काऊ 0;
    }

  *out = रिक्त;
  फर्काऊ crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

स्थिर अचर वर्ण *
find_delegate_cgroup (string_map *annotations)
{
  अचर वर्ण *annotation;

  annotation = find_string_map_मान (annotations, "run.oci.delegate-cgroup");
  यदि (annotation)
    {
      यदि (annotation[0] == '\0')
        फर्काऊ रिक्त;
      फर्काऊ annotation;
    }

  फर्काऊ रिक्त;
}

पूर्णाङ्क
libcrun_cgroup_pause_unpause (संरचना libcrun_cgroup_status *status, अचर bool छोड़ो, libcrun_error_t *err)
{
  फर्काऊ libcrun_cgroup_pause_unpause_path (status->path, छोड़ो, err);
}

पूर्णाङ्क
libcrun_cgroup_is_container_paused (संरचना libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  अचर वर्ण *cgroup_path = status->path;
  cleanup_free वर्ण *content = रिक्त;
  cleanup_free वर्ण *path = रिक्त;
  अचर वर्ण *state;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क ret;

  यदि (cgroup_path == रिक्त)
    फर्काऊ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    फर्काऊ cgroup_mode;

  यदि (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_जरा, cgroup_path, "cgroup.freeze", रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  अन्यथा
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_जरा "/freezer", cgroup_path, "freezer.state", रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  ret = read_all_file (path, &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    {
      त्रुटि_सं = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      यदि (त्रुटि_सं != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        फर्काऊ ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      संरचना statfs freezer_stat;
      यदि (statfs (CGROUP_जरा "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          फर्काऊ crun_make_error (err, त्रुटि_सं, "error when using statfs on `%s`", CGROUP_जरा "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      यदि (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        फर्काऊ ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      फर्काऊ 0;
    }

  *paused = म_माला (content, state) != रिक्त;
  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_cgroup_join_process (संरचना libcrun_cgroup_status *status, अचर वर्ण *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  संरचना libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क errno_;
  पूर्णाङ्क ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  यदि (LIKELY (ret >= 0))
    फर्काऊ ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  यदि (errno_ != EACCES && errno_ != EPERM)
    फर्काऊ ret;

  यदि (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      फर्काऊ ret;
    }
  यदि (cgroup_manager->attach_process == रिक्त)
    फर्काऊ ret;

  यदि (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->सन्देश);
      crun_error_release (&tmp_err);
      फर्काऊ ret;
    }

  crun_error_release (err);
  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_cgroup_read_pids (संरचना libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  फर्काऊ libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

पूर्णाङ्क
libcrun_cgroup_killall (संरचना libcrun_cgroup_status *cgroup_status, पूर्णाङ्क सङ्केत, libcrun_error_t *err)
{
  फर्काऊ cgroup_killall_path (cgroup_status->path, सङ्केत, err);
}

पूर्णाङ्क
libcrun_cgroup_destroy (संरचना libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  संरचना libcrun_cgroup_manager *cgroup_manager = रिक्त;
  पूर्णाङ्क ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
स्थिर पूर्णाङ्क
complete_cpu_max (अचर वर्ण *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free वर्ण *cpu_max_path = रिक्त;
  cleanup_free वर्ण *content = रिक्त;
  bool has_period;
  uint64_t period;
  int64_t quota;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क ret;

  यदि (resources == रिक्त || resources->cpu == रिक्त)
    फर्काऊ 0;

  cpu = resources->cpu;
  यदि (cpu->quota_present == cpu->period_present)
    फर्काऊ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    फर्काऊ cgroup_mode;
  यदि (cgroup_mode != CGROUP_MODE_UNIFIED)
    फर्काऊ 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_जरा, path, "cpu.max", रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = read_all_file (cpu_max_path, &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      फर्काऊ 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  अन्यथा यदि (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_update_cgroup_resources (संरचना libcrun_cgroup_status *cgroup_status,
                                 अचर वर्ण *state_जरा,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  संरचना libcrun_cgroup_manager *cgroup_manager = रिक्त;
  पूर्णाङ्क ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_जरा, resources, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  फर्काऊ update_cgroup_resources (cgroup_status->path, state_जरा, resources, ! cgroup_status->bpf_dev_set, err);
}

स्थिर पूर्णाङ्क
can_ignore_cgroup_enter_errors (संरचना libcrun_cgroup_args *args, पूर्णाङ्क cgroup_mode,
                                libcrun_error_t *err)
{
  पूर्णाङ्क manager = args->manager;
  पूर्णाङ्क rootless;

  rootless = is_rootless (err);
  यदि (UNLIKELY (rootless < 0))
    फर्काऊ rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  यदि (! rootless)
    फर्काऊ 0;

  यदि (! is_empty_string (args->cgroup_path))
    फर्काऊ 0;

  यदि (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    फर्काऊ 0;

  फर्काऊ 1;
}

पूर्णाङ्क
libcrun_cgroup_preenter (संरचना libcrun_cgroup_args *args, पूर्णाङ्क *dirfd, libcrun_error_t *err)
{
  संरचना libcrun_cgroup_manager *cgroup_manager;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    फर्काऊ cgroup_mode;

  यदि (cgroup_mode != CGROUP_MODE_UNIFIED)
    फर्काऊ 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (cgroup_manager->precreate_cgroup == रिक्त)
    फर्काऊ 0;

  फर्काऊ cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
शून्य
libcrun_cgroup_preenter_cleanup (संरचना libcrun_cgroup_args *args)
{
  यदि (args->precreated_path == रिक्त)
    फर्काऊ;

  (शून्य) सूची_हटाओ (args->precreated_path);
  मुक्त (args->precreated_path);
  args->precreated_path = रिक्त;
}

पूर्णाङ्क
libcrun_cgroup_enter (संरचना libcrun_cgroup_args *args, संरचना libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status संरचना libcrun_cgroup_status *status = xmalloc0 (परिमाण *status);
  संरचना libcrun_cgroup_manager *cgroup_manager;
  uid_t जरा_uid = args->जरा_uid;
  uid_t जरा_gid = args->जरा_gid;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    फर्काऊ cgroup_mode;

  यदि (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      माप_प्रकार len;
      cleanup_free वर्ण *buffer = रिक्त;

      ret = read_all_file (CGROUP_जरा "/unified/cgroup.controllers", &buffer, &len, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
      यदि (len > 0)
        फर्काऊ crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  यदि (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      यदि (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          फर्काऊ ignore_cgroup_errors;
        }

      यदि (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          मुक्त (status->path);
          मुक्त (status->scope);
          status->path = रिक्त;
          status->scope = रिक्त;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          गच्छ success;
        }

      फर्काऊ ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  मुक्त (args->precreated_path);
  args->precreated_path = रिक्त;

  यदि (status->path)
    {
      bool need_chown;

      need_chown = जरा_uid != (uid_t) -1 || जरा_gid != (gid_t) -1;
      यदि (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, जरा_uid, जरा_gid, err);
          यदि (UNLIKELY (ret < 0))
            गच्छ fail_destroy;
        }

      यदि (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_जरा, args->resources, ! status->bpf_dev_set, err);
          यदि (UNLIKELY (ret < 0))
            गच्छ fail_destroy;
        }
    }
success:
  *out = status;
  status = रिक्त;
  फर्काऊ 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = रिक्त;

    यदि (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  फर्काऊ ret;
}

पूर्णाङ्क
libcrun_cgroup_enter_finalize (संरचना libcrun_cgroup_args *args, संरचना libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free वर्ण *target_cgroup = रिक्त;
  cleanup_free वर्ण *content = रिक्त;
  अचर वर्ण *delegate_cgroup;
  cleanup_free वर्ण *dir = रिक्त;
  वर्ण *current_cgroup, *to;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  यदि (delegate_cgroup == रिक्त)
    फर्काऊ 0;

  यदि (path_has_dot_dot_component (delegate_cgroup))
    फर्काऊ crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    फर्काऊ cgroup_mode;

  यदि (cgroup_mode != CGROUP_MODE_UNIFIED)
    फर्काऊ crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free वर्ण *proc_path = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ fd;

  ret = read_all_fd (fd, "cgroup path", &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  current_cgroup = म_माला (content, "0::");
  यदि (UNLIKELY (current_cgroup == रिक्त))
    फर्काऊ crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = म_अक्षर (current_cgroup, '\n');
  यदि (UNLIKELY (to == रिक्त))
    फर्काऊ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = append_paths (&dir, err, CGROUP_जरा, target_cgroup, रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = move_process_to_cgroup (args->pid, रिक्त, target_cgroup, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = enable_controllers (target_cgroup, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = chown_cgroups (target_cgroup, args->जरा_uid, args->जरा_gid, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_cgroup_has_oom (संरचना libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  अचर वर्ण *path = status->path;
  अचर वर्ण *prefix = रिक्त;
  माप_प्रकार content_size = 0;
  पूर्णाङ्क cgroup_mode;
  वर्ण *it;

  यदि (UNLIKELY (path == रिक्त || path[0] == '\0'))
    फर्काऊ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    फर्काऊ cgroup_mode;

  विकल्प (cgroup_mode)
    {
    पक्ष CGROUP_MODE_UNIFIED:
      {
        cleanup_free वर्ण *events_path = रिक्त;
        पूर्णाङ्क ret;

        ret = append_paths (&events_path, err, CGROUP_जरा, path, "memory.events", रिक्त);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        prefix = "oom ";
        रोक;
      }
    पक्ष CGROUP_MODE_LEGACY:
    पक्ष CGROUP_MODE_HYBRID:
      {
        cleanup_free वर्ण *oom_control_path = रिक्त;
        पूर्णाङ्क ret;

        ret = append_paths (&oom_control_path, err, CGROUP_जरा, "memory", path, "memory.oom_control", रिक्त);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        prefix = "oom_kill ";
        रोक;
      }

    स्वभाव:
      फर्काऊ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  जबसम्म (it && *it)
    {
      यदि (has_prefix (it, prefix))
        {
          it += म_माप (prefix);
          जबसम्म (*it == ' ')
            it++;

          फर्काऊ *it != '0';
        }
      अन्यथा
        {
          it = म_अक्षर (it, '\n');
          यदि (it == रिक्त)
            फर्काऊ 0;
          it++;
        }
    }

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_cgroup_get_status (संरचना libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  फर्काऊ 0;
}

शून्य
libcrun_cgroup_status_free (संरचना libcrun_cgroup_status *cgroup_status)
{
  यदि (cgroup_status == रिक्त)
    फर्काऊ;

  मुक्त (cgroup_status->path);
  मुक्त (cgroup_status->scope);
  मुक्त (cgroup_status);
}

संरचना libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  संरचना libcrun_cgroup_status *ret;

  ret = xmalloc0 (परिमाण *ret);

  यदि (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  यदि (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  यदि (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  अन्यथा
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  फर्काऊ ret;
}
