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

#include <config.h>
#include "cgroup.h"
#include "cgroup-cgroupfs.h"
#include "cgroup-internal.h"
#include "cgroup-systemd.h"
#include "cgroup-utils.h"
#include "cgroup-setup.h"
#include "cgroup-resources.h"
#include "ebpf.h"
#include "utils.h"
#include "status.h"
#include <string.h>
#include <sched.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/vfs.h>
#include <inttypes.h>
#include <time.h>

#include <linux/magic.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <libgen.h>

static පූර්ණ_සංඛ්‍යා
libcrun_cgroup_enter_disabled (sa_nracanaa libcrun_cgroup_args *args arg_unused, sa_nracanaa libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = NULL;
  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
libcrun_destroy_cgroup_disabled (sa_nracanaa libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  ආපසු 0;
}

sa_nracanaa libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = NULL,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

static පූර්ණ_සංඛ්‍යා
get_cgroup_manager (පූර්ණ_සංඛ්‍යා manager, sa_nracanaa libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  switch (manager)
    {
    pak_sa CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      ආපසු 0;

    pak_sa CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      ආපසු 0;

    pak_sa CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      ආපසු 0;
    }

  *out = NULL;
  ආපසු crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

static const var_na *
find_delegate_cgroup (string_map *annotations)
{
  const var_na *annotation;

  annotation = find_string_map_value (annotations, "run.oci.delegate-cgroup");
  නම් (annotation)
    {
      නම් (annotation[0] == '\0')
        ආපසු NULL;
      ආපසු annotation;
    }

  ආපසු NULL;
}

පූර්ණ_සංඛ්‍යා
libcrun_cgroup_pause_unpause (sa_nracanaa libcrun_cgroup_status *status, const bool pause, libcrun_error_t *err)
{
  ආපසු libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

පූර්ණ_සංඛ්‍යා
libcrun_cgroup_is_container_paused (sa_nracanaa libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  const var_na *cgroup_path = status->path;
  cleanup_free var_na *content = NULL;
  cleanup_free var_na *path = NULL;
  const var_na *state;
  පූර්ණ_සංඛ්‍යා cgroup_mode;
  පූර්ණ_සංඛ්‍යා ret;

  නම් (cgroup_path == NULL)
    ආපසු 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (cgroup_mode < 0))
    ආපසු cgroup_mode;

  නම් (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නැතහොත්
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  ret = read_all_file (path, &content, NULL, err);
  නම් (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      නම් (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        ආපසු ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      sa_nracanaa statfs freezer_stat;
      නම් (statfs (CGROUP_ROOT "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          ආපසු crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ROOT "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      නම් (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        ආපසු ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      ආපසු 0;
    }

  *paused = strstr (content, state) != NULL;
  ආපසු 0;
}

පූර්ණ_සංඛ්‍යා
libcrun_cgroup_join_process (sa_nracanaa libcrun_cgroup_status *status, const var_na *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  sa_nracanaa libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = NULL;
  පූර්ණ_සංඛ්‍යා errno_;
  පූර්ණ_සංඛ්‍යා ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  නම් (LIKELY (ret >= 0))
    ආපසු ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  නම් (errno_ != EACCES && errno_ != EPERM)
    ආපසු ret;

  නම් (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      ආපසු ret;
    }
  නම් (cgroup_manager->attach_process == NULL)
    ආපසු ret;

  නම් (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->msg);
      crun_error_release (&tmp_err);
      ආපසු ret;
    }

  crun_error_release (err);
  ආපසු 0;
}

පූර්ණ_සංඛ්‍යා
libcrun_cgroup_read_pids (sa_nracanaa libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ආපසු libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

පූර්ණ_සංඛ්‍යා
libcrun_cgroup_killall (sa_nracanaa libcrun_cgroup_status *cgroup_status, පූර්ණ_සංඛ්‍යා sa_nkeeta, libcrun_error_t *err)
{
  ආපසු cgroup_killall_path (cgroup_status->path, sa_nkeeta, err);
}

පූර්ණ_සංඛ්‍යා
libcrun_cgroup_destroy (sa_nracanaa libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  sa_nracanaa libcrun_cgroup_manager *cgroup_manager = NULL;
  පූර්ණ_සංඛ්‍යා ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ආපසු cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
static පූර්ණ_සංඛ්‍යා
complete_cpu_max (const var_na *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free var_na *cpu_max_path = NULL;
  cleanup_free var_na *content = NULL;
  bool has_period;
  uint64_t period;
  int64_t quota;
  පූර්ණ_සංඛ්‍යා cgroup_mode;
  පූර්ණ_සංඛ්‍යා ret;

  නම් (resources == NULL || resources->cpu == NULL)
    ආපසු 0;

  cpu = resources->cpu;
  නම් (cpu->quota_present == cpu->period_present)
    ආපසු 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (cgroup_mode < 0))
    ආපසු cgroup_mode;
  නම් (cgroup_mode != CGROUP_MODE_UNIFIED)
    ආපසු 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_ROOT, path, "cpu.max", NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = read_all_file (cpu_max_path, &content, NULL, err);
  නම් (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      ආපසු 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  නැතහොත් නම් (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  ආපසු 0;
}

පූර්ණ_සංඛ්‍යා
libcrun_update_cgroup_resources (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                                 const var_na *state_root,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  sa_nracanaa libcrun_cgroup_manager *cgroup_manager = NULL;
  පූර්ණ_සංඛ්‍යා ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_root, resources, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  ආපසු update_cgroup_resources (cgroup_status->path, state_root, resources, ! cgroup_status->bpf_dev_set, err);
}

static පූර්ණ_සංඛ්‍යා
can_ignore_cgroup_enter_errors (sa_nracanaa libcrun_cgroup_args *args, පූර්ණ_සංඛ්‍යා cgroup_mode,
                                libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා manager = args->manager;
  පූර්ණ_සංඛ්‍යා rootless;

  rootless = is_rootless (err);
  නම් (UNLIKELY (rootless < 0))
    ආපසු rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  නම් (! rootless)
    ආපසු 0;

  නම් (! is_empty_string (args->cgroup_path))
    ආපසු 0;

  නම් (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    ආපසු 0;

  ආපසු 1;
}

පූර්ණ_සංඛ්‍යා
libcrun_cgroup_preenter (sa_nracanaa libcrun_cgroup_args *args, පූර්ණ_සංඛ්‍යා *dirfd, libcrun_error_t *err)
{
  sa_nracanaa libcrun_cgroup_manager *cgroup_manager;
  පූර්ණ_සංඛ්‍යා cgroup_mode;
  පූර්ණ_සංඛ්‍යා ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (cgroup_mode < 0))
    ආපසු cgroup_mode;

  නම් (cgroup_mode != CGROUP_MODE_UNIFIED)
    ආපසු 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (cgroup_manager->precreate_cgroup == NULL)
    ආපසු 0;

  ආපසු cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
void
libcrun_cgroup_preenter_cleanup (sa_nracanaa libcrun_cgroup_args *args)
{
  නම් (args->precreated_path == NULL)
    ආපසු;

  (void) rmdir (args->precreated_path);
  free (args->precreated_path);
  args->precreated_path = NULL;
}

පූර්ණ_සංඛ්‍යා
libcrun_cgroup_enter (sa_nracanaa libcrun_cgroup_args *args, sa_nracanaa libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *status = xmalloc0 (parimaa_na *status);
  sa_nracanaa libcrun_cgroup_manager *cgroup_manager;
  uid_t root_uid = args->root_uid;
  uid_t root_gid = args->root_gid;
  පූර්ණ_සංඛ්‍යා cgroup_mode;
  පූර්ණ_සංඛ්‍යා ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (cgroup_mode < 0))
    ආපසු cgroup_mode;

  නම් (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free var_na *buffer = NULL;

      ret = read_all_file (CGROUP_ROOT "/unified/cgroup.controllers", &buffer, &len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
      නම් (len > 0)
        ආපසු crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  නම් (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = NULL;
      පූර්ණ_සංඛ්‍යා ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      නම් (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ආපසු ignore_cgroup_errors;
        }

      නම් (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          free (status->path);
          free (status->scope);
          status->path = NULL;
          status->scope = NULL;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          goto success;
        }

      ආපසු ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  free (args->precreated_path);
  args->precreated_path = NULL;

  නම් (status->path)
    {
      bool need_chown;

      need_chown = root_uid != (uid_t) -1 || root_gid != (gid_t) -1;
      නම් (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, root_uid, root_gid, err);
          නම් (UNLIKELY (ret < 0))
            goto fail_destroy;
        }

      නම් (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_root, args->resources, ! status->bpf_dev_set, err);
          නම් (UNLIKELY (ret < 0))
            goto fail_destroy;
        }
    }
success:
  *out = status;
  status = NULL;
  ආපසු 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = NULL;

    නම් (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  ආපසු ret;
}

පූර්ණ_සංඛ්‍යා
libcrun_cgroup_enter_finalize (sa_nracanaa libcrun_cgroup_args *args, sa_nracanaa libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free var_na *target_cgroup = NULL;
  cleanup_free var_na *content = NULL;
  const var_na *delegate_cgroup;
  cleanup_free var_na *dir = NULL;
  var_na *current_cgroup, *to;
  පූර්ණ_සංඛ්‍යා cgroup_mode;
  පූර්ණ_සංඛ්‍යා ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  නම් (delegate_cgroup == NULL)
    ආපසු 0;

  නම් (path_has_dot_dot_component (delegate_cgroup))
    ආපසු crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (cgroup_mode < 0))
    ආපසු cgroup_mode;

  නම් (cgroup_mode != CGROUP_MODE_UNIFIED)
    ආපසු crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free var_na *proc_path = NULL;
  cleanup_close පූර්ණ_සංඛ්‍යා fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  නම් (UNLIKELY (fd < 0))
    ආපසු fd;

  ret = read_all_fd (fd, "cgroup path", &content, NULL, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  current_cgroup = strstr (content, "0::");
  නම් (UNLIKELY (current_cgroup == NULL))
    ආපසු crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  නම් (UNLIKELY (to == NULL))
    ආපසු crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = append_paths (&dir, err, CGROUP_ROOT, target_cgroup, NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = move_process_to_cgroup (args->pid, NULL, target_cgroup, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = enable_controllers (target_cgroup, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = chown_cgroups (target_cgroup, args->root_uid, args->root_gid, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ආපසු 0;
}

පූර්ණ_සංඛ්‍යා
libcrun_cgroup_has_oom (sa_nracanaa libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  const var_na *path = status->path;
  const var_na *prefix = NULL;
  size_t content_size = 0;
  පූර්ණ_සංඛ්‍යා cgroup_mode;
  var_na *it;

  නම් (UNLIKELY (path == NULL || path[0] == '\0'))
    ආපසු 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (cgroup_mode < 0))
    ආපසු cgroup_mode;

  switch (cgroup_mode)
    {
    pak_sa CGROUP_MODE_UNIFIED:
      {
        cleanup_free var_na *events_path = NULL;
        පූර්ණ_සංඛ්‍යා ret;

        ret = append_paths (&events_path, err, CGROUP_ROOT, path, "memory.events", NULL);
        නම් (UNLIKELY (ret < 0))
          ආපසු ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        නම් (UNLIKELY (ret < 0))
          ආපසු ret;

        prefix = "oom ";
        bha_nga;
      }
    pak_sa CGROUP_MODE_LEGACY:
    pak_sa CGROUP_MODE_HYBRID:
      {
        cleanup_free var_na *oom_control_path = NULL;
        පූර්ණ_සංඛ්‍යා ret;

        ret = append_paths (&oom_control_path, err, CGROUP_ROOT, "memory", path, "memory.oom_control", NULL);
        නම් (UNLIKELY (ret < 0))
          ආපසු ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        නම් (UNLIKELY (ret < 0))
          ආපසු ret;

        prefix = "oom_kill ";
        bha_nga;
      }

    default:
      ආපසු crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  අතරතුර (it && *it)
    {
      නම් (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          අතරතුර (*it == ' ')
            it++;

          ආපසු *it != '0';
        }
      නැතහොත්
        {
          it = strchr (it, '\n');
          නම් (it == NULL)
            ආපසු 0;
          it++;
        }
    }

  ආපසු 0;
}

පූර්ණ_සංඛ්‍යා
libcrun_cgroup_get_status (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  ආපසු 0;
}

void
libcrun_cgroup_status_free (sa_nracanaa libcrun_cgroup_status *cgroup_status)
{
  නම් (cgroup_status == NULL)
    ආපසු;

  free (cgroup_status->path);
  free (cgroup_status->scope);
  free (cgroup_status);
}

sa_nracanaa libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  sa_nracanaa libcrun_cgroup_status *ret;

  ret = xmalloc0 (parimaa_na *ret);

  නම් (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  නම් (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  නම් (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  නැතහොත්
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  ආපසු ret;
}
