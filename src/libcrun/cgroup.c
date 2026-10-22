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

static цяло
libcrun_cgroup_enter_disabled (struct libcrun_cgroup_args *args arg_unused, struct libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = NULL;
  върни 0;
}

static цяло
libcrun_destroy_cgroup_disabled (struct libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  върни 0;
}

struct libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = NULL,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

static цяло
get_cgroup_manager (цяло manager, struct libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  switch (manager)
    {
    case CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      върни 0;

    case CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      върни 0;

    case CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      върни 0;
    }

  *out = NULL;
  върни crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

static const char *
find_delegate_cgroup (string_map *annotations)
{
  const char *annotation;

  annotation = find_string_map_value (annotations, "run.oci.delegate-cgroup");
  ако (annotation)
    {
      ако (annotation[0] == '\0')
        върни NULL;
      върни annotation;
    }

  върни NULL;
}

цяло
libcrun_cgroup_pause_unpause (struct libcrun_cgroup_status *status, const bool pause, libcrun_error_t *err)
{
  върни libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

цяло
libcrun_cgroup_is_container_paused (struct libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  const char *cgroup_path = status->path;
  cleanup_free char *content = NULL;
  cleanup_free char *path = NULL;
  const char *state;
  цяло cgroup_mode;
  цяло ret;

  ако (cgroup_path == NULL)
    върни 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  ако (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  иначе
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ret = read_all_file (path, &content, NULL, err);
  ако (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      ако (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        върни ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      struct statfs freezer_stat;
      ако (statfs (CGROUP_ROOT "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          върни crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ROOT "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      ако (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        върни ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      върни 0;
    }

  *paused = strstr (content, state) != NULL;
  върни 0;
}

цяло
libcrun_cgroup_join_process (struct libcrun_cgroup_status *status, const char *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = NULL;
  цяло errno_;
  цяло ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  ако (LIKELY (ret >= 0))
    върни ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  ако (errno_ != EACCES && errno_ != EPERM)
    върни ret;

  ако (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      върни ret;
    }
  ако (cgroup_manager->attach_process == NULL)
    върни ret;

  ако (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->msg);
      crun_error_release (&tmp_err);
      върни ret;
    }

  crun_error_release (err);
  върни 0;
}

цяло
libcrun_cgroup_read_pids (struct libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  върни libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

цяло
libcrun_cgroup_killall (struct libcrun_cgroup_status *cgroup_status, цяло signal, libcrun_error_t *err)
{
  върни cgroup_killall_path (cgroup_status->path, signal, err);
}

цяло
libcrun_cgroup_destroy (struct libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager = NULL;
  цяло ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  върни cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
static цяло
complete_cpu_max (const char *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free char *cpu_max_path = NULL;
  cleanup_free char *content = NULL;
  bool has_period;
  uint64_t period;
  int64_t quota;
  цяло cgroup_mode;
  цяло ret;

  ако (resources == NULL || resources->cpu == NULL)
    върни 0;

  cpu = resources->cpu;
  ако (cpu->quota_present == cpu->period_present)
    върни 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;
  ако (cgroup_mode != CGROUP_MODE_UNIFIED)
    върни 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_ROOT, path, "cpu.max", NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = read_all_file (cpu_max_path, &content, NULL, err);
  ако (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      върни 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ако (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  иначе ако (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  върни 0;
}

цяло
libcrun_update_cgroup_resources (struct libcrun_cgroup_status *cgroup_status,
                                 const char *state_root,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager = NULL;
  цяло ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ако (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_root, resources, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  върни update_cgroup_resources (cgroup_status->path, state_root, resources, ! cgroup_status->bpf_dev_set, err);
}

static цяло
can_ignore_cgroup_enter_errors (struct libcrun_cgroup_args *args, цяло cgroup_mode,
                                libcrun_error_t *err)
{
  цяло manager = args->manager;
  цяло rootless;

  rootless = is_rootless (err);
  ако (UNLIKELY (rootless < 0))
    върни rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  ако (! rootless)
    върни 0;

  ако (! is_empty_string (args->cgroup_path))
    върни 0;

  ако (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    върни 0;

  върни 1;
}

цяло
libcrun_cgroup_preenter (struct libcrun_cgroup_args *args, цяло *dirfd, libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager;
  цяло cgroup_mode;
  цяло ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  ако (cgroup_mode != CGROUP_MODE_UNIFIED)
    върни 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ако (cgroup_manager->precreate_cgroup == NULL)
    върни 0;

  върни cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
void
libcrun_cgroup_preenter_cleanup (struct libcrun_cgroup_args *args)
{
  ако (args->precreated_path == NULL)
    върни;

  (void) rmdir (args->precreated_path);
  free (args->precreated_path);
  args->precreated_path = NULL;
}

цяло
libcrun_cgroup_enter (struct libcrun_cgroup_args *args, struct libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status struct libcrun_cgroup_status *status = xmalloc0 (sizeof *status);
  struct libcrun_cgroup_manager *cgroup_manager;
  uid_t root_uid = args->root_uid;
  uid_t root_gid = args->root_gid;
  цяло cgroup_mode;
  цяло ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  ако (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free char *buffer = NULL;

      ret = read_all_file (CGROUP_ROOT "/unified/cgroup.controllers", &buffer, &len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
      ако (len > 0)
        върни crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  ако (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = NULL;
      цяло ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      ако (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          върни ignore_cgroup_errors;
        }

      ако (ignore_cgroup_errors)
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

      върни ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  free (args->precreated_path);
  args->precreated_path = NULL;

  ако (status->path)
    {
      bool need_chown;

      need_chown = root_uid != (uid_t) -1 || root_gid != (gid_t) -1;
      ако (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, root_uid, root_gid, err);
          ако (UNLIKELY (ret < 0))
            goto fail_destroy;
        }

      ако (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_root, args->resources, ! status->bpf_dev_set, err);
          ако (UNLIKELY (ret < 0))
            goto fail_destroy;
        }
    }
success:
  *out = status;
  status = NULL;
  върни 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = NULL;

    ако (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  върни ret;
}

цяло
libcrun_cgroup_enter_finalize (struct libcrun_cgroup_args *args, struct libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free char *target_cgroup = NULL;
  cleanup_free char *content = NULL;
  const char *delegate_cgroup;
  cleanup_free char *dir = NULL;
  char *current_cgroup, *to;
  цяло cgroup_mode;
  цяло ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  ако (delegate_cgroup == NULL)
    върни 0;

  ако (path_has_dot_dot_component (delegate_cgroup))
    върни crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  ако (cgroup_mode != CGROUP_MODE_UNIFIED)
    върни crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free char *proc_path = NULL;
  cleanup_close цяло fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  ако (UNLIKELY (fd < 0))
    върни fd;

  ret = read_all_fd (fd, "cgroup path", &content, NULL, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  current_cgroup = strstr (content, "0::");
  ако (UNLIKELY (current_cgroup == NULL))
    върни crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  ако (UNLIKELY (to == NULL))
    върни crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = append_paths (&dir, err, CGROUP_ROOT, target_cgroup, NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = move_process_to_cgroup (args->pid, NULL, target_cgroup, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = enable_controllers (target_cgroup, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = chown_cgroups (target_cgroup, args->root_uid, args->root_gid, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  върни 0;
}

цяло
libcrun_cgroup_has_oom (struct libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  const char *path = status->path;
  const char *prefix = NULL;
  size_t content_size = 0;
  цяло cgroup_mode;
  char *it;

  ако (UNLIKELY (path == NULL || path[0] == '\0'))
    върни 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  switch (cgroup_mode)
    {
    case CGROUP_MODE_UNIFIED:
      {
        cleanup_free char *events_path = NULL;
        цяло ret;

        ret = append_paths (&events_path, err, CGROUP_ROOT, path, "memory.events", NULL);
        ако (UNLIKELY (ret < 0))
          върни ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        ако (UNLIKELY (ret < 0))
          върни ret;

        prefix = "oom ";
        break;
      }
    case CGROUP_MODE_LEGACY:
    case CGROUP_MODE_HYBRID:
      {
        cleanup_free char *oom_control_path = NULL;
        цяло ret;

        ret = append_paths (&oom_control_path, err, CGROUP_ROOT, "memory", path, "memory.oom_control", NULL);
        ако (UNLIKELY (ret < 0))
          върни ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        ако (UNLIKELY (ret < 0))
          върни ret;

        prefix = "oom_kill ";
        break;
      }

    default:
      върни crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  докато (it && *it)
    {
      ако (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          докато (*it == ' ')
            it++;

          върни *it != '0';
        }
      иначе
        {
          it = strchr (it, '\n');
          ако (it == NULL)
            върни 0;
          it++;
        }
    }

  върни 0;
}

цяло
libcrun_cgroup_get_status (struct libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  върни 0;
}

void
libcrun_cgroup_status_free (struct libcrun_cgroup_status *cgroup_status)
{
  ако (cgroup_status == NULL)
    върни;

  free (cgroup_status->path);
  free (cgroup_status->scope);
  free (cgroup_status);
}

struct libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  struct libcrun_cgroup_status *ret;

  ret = xmalloc0 (sizeof *ret);

  ако (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  ако (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  ако (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  иначе
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  върни ret;
}
