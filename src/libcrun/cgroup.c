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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "cgroup.h"
#समावेश "cgroup-cgroupfs.h"
#समावेश "cgroup-internal.h"
#समावेश "cgroup-systemd.h"
#समावेश "cgroup-utils.h"
#समावेश "cgroup-setup.h"
#समावेश "cgroup-resources.h"
#समावेश "ebpf.h"
#समावेश "utils.h"
#समावेश "status.h"
#समावेश <string.h>
#समावेश <sched.h>
#समावेश <sys/types.h>
#समावेश <signal.h>
#समावेश <sys/vfs.h>
#समावेश <inttypes.h>
#समावेश <time.h>

#समावेश <linux/magic.h>
#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#समावेश <fcntl.h>
#समावेश <libgen.h>

अटल पूर्णांक
libcrun_cgroup_enter_disabled (काष्ठा libcrun_cgroup_args *args arg_unused, काष्ठा libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = शून्य;
  वापस 0;
}

अटल पूर्णांक
libcrun_destroy_cgroup_disabled (काष्ठा libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  वापस 0;
}

काष्ठा libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = शून्य,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

अटल पूर्णांक
get_cgroup_manager (पूर्णांक manager, काष्ठा libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  चयन (manager)
    {
    हाल CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      वापस 0;

    हाल CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      वापस 0;

    हाल CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      वापस 0;
    }

  *out = शून्य;
  वापस crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

अटल स्थिर अक्षर *
find_delegate_cgroup (string_map *annotations)
{
  स्थिर अक्षर *annotation;

  annotation = find_string_map_मूल्य (annotations, "run.oci.delegate-cgroup");
  अगर (annotation)
    {
      अगर (annotation[0] == '\0')
        वापस शून्य;
      वापस annotation;
    }

  वापस शून्य;
}

पूर्णांक
libcrun_cgroup_pause_unpause (काष्ठा libcrun_cgroup_status *status, स्थिर bool छोड़ो, libcrun_error_t *err)
{
  वापस libcrun_cgroup_pause_unpause_path (status->path, छोड़ो, err);
}

पूर्णांक
libcrun_cgroup_is_container_paused (काष्ठा libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  स्थिर अक्षर *cgroup_path = status->path;
  cleanup_free अक्षर *content = शून्य;
  cleanup_free अक्षर *path = शून्य;
  स्थिर अक्षर *state;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  अगर (cgroup_path == शून्य)
    वापस 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_मूल, cgroup_path, "cgroup.freeze", शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_मूल "/freezer", cgroup_path, "freezer.state", शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = read_all_file (path, &content, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    {
      त्रुटि_सं = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      अगर (त्रुटि_सं != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        वापस ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      काष्ठा statfs freezer_stat;
      अगर (statfs (CGROUP_मूल "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          वापस crun_make_error (err, त्रुटि_सं, "error when using statfs on `%s`", CGROUP_मूल "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      अगर (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        वापस ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      वापस 0;
    }

  *paused = म_माला (content, state) != शून्य;
  वापस 0;
}

पूर्णांक
libcrun_cgroup_join_process (काष्ठा libcrun_cgroup_status *status, स्थिर अक्षर *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  काष्ठा libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = शून्य;
  पूर्णांक errno_;
  पूर्णांक ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  अगर (LIKELY (ret >= 0))
    वापस ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  अगर (errno_ != EACCES && errno_ != EPERM)
    वापस ret;

  अगर (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      वापस ret;
    }
  अगर (cgroup_manager->attach_process == शून्य)
    वापस ret;

  अगर (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->संदेश);
      crun_error_release (&tmp_err);
      वापस ret;
    }

  crun_error_release (err);
  वापस 0;
}

पूर्णांक
libcrun_cgroup_read_pids (काष्ठा libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  वापस libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

पूर्णांक
libcrun_cgroup_killall (काष्ठा libcrun_cgroup_status *cgroup_status, पूर्णांक संकेत, libcrun_error_t *err)
{
  वापस cgroup_killall_path (cgroup_status->path, संकेत, err);
}

पूर्णांक
libcrun_cgroup_destroy (काष्ठा libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  काष्ठा libcrun_cgroup_manager *cgroup_manager = शून्य;
  पूर्णांक ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
अटल पूर्णांक
complete_cpu_max (स्थिर अक्षर *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free अक्षर *cpu_max_path = शून्य;
  cleanup_free अक्षर *content = शून्य;
  bool has_period;
  uint64_t period;
  int64_t quota;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  अगर (resources == शून्य || resources->cpu == शून्य)
    वापस 0;

  cpu = resources->cpu;
  अगर (cpu->quota_present == cpu->period_present)
    वापस 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;
  अगर (cgroup_mode != CGROUP_MODE_UNIFIED)
    वापस 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_मूल, path, "cpu.max", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = read_all_file (cpu_max_path, &content, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      वापस 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  अन्यथा अगर (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  वापस 0;
}

पूर्णांक
libcrun_update_cgroup_resources (काष्ठा libcrun_cgroup_status *cgroup_status,
                                 स्थिर अक्षर *state_मूल,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  काष्ठा libcrun_cgroup_manager *cgroup_manager = शून्य;
  पूर्णांक ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_मूल, resources, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस update_cgroup_resources (cgroup_status->path, state_मूल, resources, ! cgroup_status->bpf_dev_set, err);
}

अटल पूर्णांक
can_ignore_cgroup_enter_errors (काष्ठा libcrun_cgroup_args *args, पूर्णांक cgroup_mode,
                                libcrun_error_t *err)
{
  पूर्णांक manager = args->manager;
  पूर्णांक rootless;

  rootless = is_rootless (err);
  अगर (UNLIKELY (rootless < 0))
    वापस rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  अगर (! rootless)
    वापस 0;

  अगर (! is_empty_string (args->cgroup_path))
    वापस 0;

  अगर (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    वापस 0;

  वापस 1;
}

पूर्णांक
libcrun_cgroup_preenter (काष्ठा libcrun_cgroup_args *args, पूर्णांक *dirfd, libcrun_error_t *err)
{
  काष्ठा libcrun_cgroup_manager *cgroup_manager;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  अगर (cgroup_mode != CGROUP_MODE_UNIFIED)
    वापस 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (cgroup_manager->precreate_cgroup == शून्य)
    वापस 0;

  वापस cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
व्योम
libcrun_cgroup_preenter_cleanup (काष्ठा libcrun_cgroup_args *args)
{
  अगर (args->precreated_path == शून्य)
    वापस;

  (व्योम) सूची_हटाओ (args->precreated_path);
  मुक्त (args->precreated_path);
  args->precreated_path = शून्य;
}

पूर्णांक
libcrun_cgroup_enter (काष्ठा libcrun_cgroup_args *args, काष्ठा libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status काष्ठा libcrun_cgroup_status *status = xmalloc0 (माप *status);
  काष्ठा libcrun_cgroup_manager *cgroup_manager;
  uid_t मूल_uid = args->मूल_uid;
  uid_t मूल_gid = args->मूल_gid;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  अगर (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      माप_प्रकार len;
      cleanup_free अक्षर *बफर = शून्य;

      ret = read_all_file (CGROUP_मूल "/unified/cgroup.controllers", &बफर, &len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      अगर (len > 0)
        वापस crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  अगर (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = शून्य;
      पूर्णांक ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      अगर (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          वापस ignore_cgroup_errors;
        }

      अगर (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          मुक्त (status->path);
          मुक्त (status->scope);
          status->path = शून्य;
          status->scope = शून्य;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          जाओ success;
        }

      वापस ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  मुक्त (args->precreated_path);
  args->precreated_path = शून्य;

  अगर (status->path)
    {
      bool need_chown;

      need_chown = मूल_uid != (uid_t) -1 || मूल_gid != (gid_t) -1;
      अगर (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, मूल_uid, मूल_gid, err);
          अगर (UNLIKELY (ret < 0))
            जाओ fail_destroy;
        }

      अगर (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_मूल, args->resources, ! status->bpf_dev_set, err);
          अगर (UNLIKELY (ret < 0))
            जाओ fail_destroy;
        }
    }
success:
  *out = status;
  status = शून्य;
  वापस 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = शून्य;

    अगर (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  वापस ret;
}

पूर्णांक
libcrun_cgroup_enter_finalize (काष्ठा libcrun_cgroup_args *args, काष्ठा libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free अक्षर *target_cgroup = शून्य;
  cleanup_free अक्षर *content = शून्य;
  स्थिर अक्षर *delegate_cgroup;
  cleanup_free अक्षर *dir = शून्य;
  अक्षर *current_cgroup, *to;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  अगर (delegate_cgroup == शून्य)
    वापस 0;

  अगर (path_has_dot_dot_component (delegate_cgroup))
    वापस crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  अगर (cgroup_mode != CGROUP_MODE_UNIFIED)
    वापस crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free अक्षर *proc_path = शून्य;
  cleanup_close पूर्णांक fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  अगर (UNLIKELY (fd < 0))
    वापस fd;

  ret = read_all_fd (fd, "cgroup path", &content, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  current_cgroup = म_माला (content, "0::");
  अगर (UNLIKELY (current_cgroup == शून्य))
    वापस crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = म_अक्षर (current_cgroup, '\n');
  अगर (UNLIKELY (to == शून्य))
    वापस crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_paths (&dir, err, CGROUP_मूल, target_cgroup, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = move_process_to_cgroup (args->pid, शून्य, target_cgroup, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = enable_controllers (target_cgroup, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = chown_cgroups (target_cgroup, args->मूल_uid, args->मूल_gid, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

पूर्णांक
libcrun_cgroup_has_oom (काष्ठा libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = शून्य;
  स्थिर अक्षर *path = status->path;
  स्थिर अक्षर *prefix = शून्य;
  माप_प्रकार content_आकार = 0;
  पूर्णांक cgroup_mode;
  अक्षर *it;

  अगर (UNLIKELY (path == शून्य || path[0] == '\0'))
    वापस 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  चयन (cgroup_mode)
    {
    हाल CGROUP_MODE_UNIFIED:
      {
        cleanup_free अक्षर *events_path = शून्य;
        पूर्णांक ret;

        ret = append_paths (&events_path, err, CGROUP_मूल, path, "memory.events", शून्य);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_आकार, err);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        prefix = "oom ";
        अवरोध;
      }
    हाल CGROUP_MODE_LEGACY:
    हाल CGROUP_MODE_HYBRID:
      {
        cleanup_free अक्षर *oom_control_path = शून्य;
        पूर्णांक ret;

        ret = append_paths (&oom_control_path, err, CGROUP_मूल, "memory", path, "memory.oom_control", शून्य);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_आकार, err);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        prefix = "oom_kill ";
        अवरोध;
      }

    शेष:
      वापस crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  जबतक (it && *it)
    {
      अगर (has_prefix (it, prefix))
        {
          it += म_माप (prefix);
          जबतक (*it == ' ')
            it++;

          वापस *it != '0';
        }
      अन्यथा
        {
          it = म_अक्षर (it, '\n');
          अगर (it == शून्य)
            वापस 0;
          it++;
        }
    }

  वापस 0;
}

पूर्णांक
libcrun_cgroup_get_status (काष्ठा libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  वापस 0;
}

व्योम
libcrun_cgroup_status_free (काष्ठा libcrun_cgroup_status *cgroup_status)
{
  अगर (cgroup_status == शून्य)
    वापस;

  मुक्त (cgroup_status->path);
  मुक्त (cgroup_status->scope);
  मुक्त (cgroup_status);
}

काष्ठा libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  काष्ठा libcrun_cgroup_status *ret;

  ret = xmalloc0 (माप *ret);

  अगर (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  अगर (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  अगर (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  अन्यथा
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  वापस ret;
}
