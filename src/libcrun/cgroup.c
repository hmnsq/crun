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

static целое
libcrun_cgroup_enter_disabled (struct libcrun_cgroup_args *args arg_unused, struct libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = NULL;
  вернуть 0;
}

static целое
libcrun_destroy_cgroup_disabled (struct libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  вернуть 0;
}

struct libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = NULL,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

static целое
get_cgroup_manager (целое manager, struct libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  switch (manager)
    {
    case CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      вернуть 0;

    case CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      вернуть 0;

    case CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      вернуть 0;
    }

  *out = NULL;
  вернуть crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

static const char *
find_delegate_cgroup (string_map *annotations)
{
  const char *annotation;

  annotation = find_string_map_value (annotations, "run.oci.delegate-cgroup");
  если (annotation)
    {
      если (annotation[0] == '\0')
        вернуть NULL;
      вернуть annotation;
    }

  вернуть NULL;
}

целое
libcrun_cgroup_pause_unpause (struct libcrun_cgroup_status *status, const bool pause, libcrun_error_t *err)
{
  вернуть libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

целое
libcrun_cgroup_is_container_paused (struct libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  const char *cgroup_path = status->path;
  cleanup_free char *content = NULL;
  cleanup_free char *path = NULL;
  const char *state;
  целое cgroup_mode;
  целое ret;

  если (cgroup_path == NULL)
    вернуть 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  если (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  иначе
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  ret = read_all_file (path, &content, NULL, err);
  если (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      если (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        вернуть ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      struct statfs freezer_stat;
      если (statfs (CGROUP_ROOT "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          вернуть crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ROOT "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      если (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        вернуть ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      вернуть 0;
    }

  *paused = strstr (content, state) != NULL;
  вернуть 0;
}

целое
libcrun_cgroup_join_process (struct libcrun_cgroup_status *status, const char *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = NULL;
  целое errno_;
  целое ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  если (LIKELY (ret >= 0))
    вернуть ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  если (errno_ != EACCES && errno_ != EPERM)
    вернуть ret;

  если (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      вернуть ret;
    }
  если (cgroup_manager->attach_process == NULL)
    вернуть ret;

  если (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->msg);
      crun_error_release (&tmp_err);
      вернуть ret;
    }

  crun_error_release (err);
  вернуть 0;
}

целое
libcrun_cgroup_read_pids (struct libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  вернуть libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

целое
libcrun_cgroup_killall (struct libcrun_cgroup_status *cgroup_status, целое signal, libcrun_error_t *err)
{
  вернуть cgroup_killall_path (cgroup_status->path, signal, err);
}

целое
libcrun_cgroup_destroy (struct libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager = NULL;
  целое ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
static целое
complete_cpu_max (const char *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free char *cpu_max_path = NULL;
  cleanup_free char *content = NULL;
  bool has_period;
  uint64_t period;
  int64_t quota;
  целое cgroup_mode;
  целое ret;

  если (resources == NULL || resources->cpu == NULL)
    вернуть 0;

  cpu = resources->cpu;
  если (cpu->quota_present == cpu->period_present)
    вернуть 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;
  если (cgroup_mode != CGROUP_MODE_UNIFIED)
    вернуть 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_ROOT, path, "cpu.max", NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = read_all_file (cpu_max_path, &content, NULL, err);
  если (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      вернуть 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  иначе если (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  вернуть 0;
}

целое
libcrun_update_cgroup_resources (struct libcrun_cgroup_status *cgroup_status,
                                 const char *state_root,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager = NULL;
  целое ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_root, resources, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  вернуть update_cgroup_resources (cgroup_status->path, state_root, resources, ! cgroup_status->bpf_dev_set, err);
}

static целое
can_ignore_cgroup_enter_errors (struct libcrun_cgroup_args *args, целое cgroup_mode,
                                libcrun_error_t *err)
{
  целое manager = args->manager;
  целое rootless;

  rootless = is_rootless (err);
  если (UNLIKELY (rootless < 0))
    вернуть rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  если (! rootless)
    вернуть 0;

  если (! is_empty_string (args->cgroup_path))
    вернуть 0;

  если (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    вернуть 0;

  вернуть 1;
}

целое
libcrun_cgroup_preenter (struct libcrun_cgroup_args *args, целое *dirfd, libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager;
  целое cgroup_mode;
  целое ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  если (cgroup_mode != CGROUP_MODE_UNIFIED)
    вернуть 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (cgroup_manager->precreate_cgroup == NULL)
    вернуть 0;

  вернуть cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
void
libcrun_cgroup_preenter_cleanup (struct libcrun_cgroup_args *args)
{
  если (args->precreated_path == NULL)
    вернуть;

  (void) rmdir (args->precreated_path);
  free (args->precreated_path);
  args->precreated_path = NULL;
}

целое
libcrun_cgroup_enter (struct libcrun_cgroup_args *args, struct libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status struct libcrun_cgroup_status *status = xmalloc0 (sizeof *status);
  struct libcrun_cgroup_manager *cgroup_manager;
  uid_t root_uid = args->root_uid;
  uid_t root_gid = args->root_gid;
  целое cgroup_mode;
  целое ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  если (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free char *buffer = NULL;

      ret = read_all_file (CGROUP_ROOT "/unified/cgroup.controllers", &buffer, &len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
      если (len > 0)
        вернуть crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  если (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = NULL;
      целое ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      если (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          вернуть ignore_cgroup_errors;
        }

      если (ignore_cgroup_errors)
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

      вернуть ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  free (args->precreated_path);
  args->precreated_path = NULL;

  если (status->path)
    {
      bool need_chown;

      need_chown = root_uid != (uid_t) -1 || root_gid != (gid_t) -1;
      если (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, root_uid, root_gid, err);
          если (UNLIKELY (ret < 0))
            goto fail_destroy;
        }

      если (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_root, args->resources, ! status->bpf_dev_set, err);
          если (UNLIKELY (ret < 0))
            goto fail_destroy;
        }
    }
success:
  *out = status;
  status = NULL;
  вернуть 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = NULL;

    если (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  вернуть ret;
}

целое
libcrun_cgroup_enter_finalize (struct libcrun_cgroup_args *args, struct libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free char *target_cgroup = NULL;
  cleanup_free char *content = NULL;
  const char *delegate_cgroup;
  cleanup_free char *dir = NULL;
  char *current_cgroup, *to;
  целое cgroup_mode;
  целое ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  если (delegate_cgroup == NULL)
    вернуть 0;

  если (path_has_dot_dot_component (delegate_cgroup))
    вернуть crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  если (cgroup_mode != CGROUP_MODE_UNIFIED)
    вернуть crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free char *proc_path = NULL;
  cleanup_close целое fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  если (UNLIKELY (fd < 0))
    вернуть fd;

  ret = read_all_fd (fd, "cgroup path", &content, NULL, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  current_cgroup = strstr (content, "0::");
  если (UNLIKELY (current_cgroup == NULL))
    вернуть crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  если (UNLIKELY (to == NULL))
    вернуть crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = append_paths (&dir, err, CGROUP_ROOT, target_cgroup, NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = move_process_to_cgroup (args->pid, NULL, target_cgroup, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = enable_controllers (target_cgroup, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = chown_cgroups (target_cgroup, args->root_uid, args->root_gid, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть 0;
}

целое
libcrun_cgroup_has_oom (struct libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  const char *path = status->path;
  const char *prefix = NULL;
  size_t content_size = 0;
  целое cgroup_mode;
  char *it;

  если (UNLIKELY (path == NULL || path[0] == '\0'))
    вернуть 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  switch (cgroup_mode)
    {
    case CGROUP_MODE_UNIFIED:
      {
        cleanup_free char *events_path = NULL;
        целое ret;

        ret = append_paths (&events_path, err, CGROUP_ROOT, path, "memory.events", NULL);
        если (UNLIKELY (ret < 0))
          вернуть ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        если (UNLIKELY (ret < 0))
          вернуть ret;

        prefix = "oom ";
        break;
      }
    case CGROUP_MODE_LEGACY:
    case CGROUP_MODE_HYBRID:
      {
        cleanup_free char *oom_control_path = NULL;
        целое ret;

        ret = append_paths (&oom_control_path, err, CGROUP_ROOT, "memory", path, "memory.oom_control", NULL);
        если (UNLIKELY (ret < 0))
          вернуть ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        если (UNLIKELY (ret < 0))
          вернуть ret;

        prefix = "oom_kill ";
        break;
      }

    default:
      вернуть crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  пока (it && *it)
    {
      если (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          пока (*it == ' ')
            it++;

          вернуть *it != '0';
        }
      иначе
        {
          it = strchr (it, '\n');
          если (it == NULL)
            вернуть 0;
          it++;
        }
    }

  вернуть 0;
}

целое
libcrun_cgroup_get_status (struct libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  вернуть 0;
}

void
libcrun_cgroup_status_free (struct libcrun_cgroup_status *cgroup_status)
{
  если (cgroup_status == NULL)
    вернуть;

  free (cgroup_status->path);
  free (cgroup_status->scope);
  free (cgroup_status);
}

struct libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  struct libcrun_cgroup_status *ret;

  ret = xmalloc0 (sizeof *ret);

  если (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  если (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  если (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  иначе
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  вернуть ret;
}
