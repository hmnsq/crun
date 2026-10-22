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

static số_nguyên
libcrun_cgroup_enter_disabled (struct libcrun_cgroup_args *args arg_unused, struct libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = NULL;
  trả_về 0;
}

static số_nguyên
libcrun_destroy_cgroup_disabled (struct libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  trả_về 0;
}

struct libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = NULL,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

static số_nguyên
get_cgroup_manager (số_nguyên manager, struct libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  switch (manager)
    {
    case CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      trả_về 0;

    case CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      trả_về 0;

    case CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      trả_về 0;
    }

  *out = NULL;
  trả_về crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

static const char *
find_delegate_cgroup (string_map *annotations)
{
  const char *annotation;

  annotation = find_string_map_value (annotations, "run.oci.delegate-cgroup");
  nếu (annotation)
    {
      nếu (annotation[0] == '\0')
        trả_về NULL;
      trả_về annotation;
    }

  trả_về NULL;
}

số_nguyên
libcrun_cgroup_pause_unpause (struct libcrun_cgroup_status *status, const bool pause, libcrun_error_t *err)
{
  trả_về libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

số_nguyên
libcrun_cgroup_is_container_paused (struct libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  const char *cgroup_path = status->path;
  cleanup_free char *content = NULL;
  cleanup_free char *path = NULL;
  const char *state;
  số_nguyên cgroup_mode;
  số_nguyên ret;

  nếu (cgroup_path == NULL)
    trả_về 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;

  nếu (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
    }
  không_thì
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
    }

  ret = read_all_file (path, &content, NULL, err);
  nếu (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      nếu (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        trả_về ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      struct statfs freezer_stat;
      nếu (statfs (CGROUP_ROOT "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          trả_về crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ROOT "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      nếu (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        trả_về ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      trả_về 0;
    }

  *paused = strstr (content, state) != NULL;
  trả_về 0;
}

số_nguyên
libcrun_cgroup_join_process (struct libcrun_cgroup_status *status, const char *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = NULL;
  số_nguyên errno_;
  số_nguyên ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  nếu (LIKELY (ret >= 0))
    trả_về ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  nếu (errno_ != EACCES && errno_ != EPERM)
    trả_về ret;

  nếu (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      trả_về ret;
    }
  nếu (cgroup_manager->attach_process == NULL)
    trả_về ret;

  nếu (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->msg);
      crun_error_release (&tmp_err);
      trả_về ret;
    }

  crun_error_release (err);
  trả_về 0;
}

số_nguyên
libcrun_cgroup_read_pids (struct libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  trả_về libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

số_nguyên
libcrun_cgroup_killall (struct libcrun_cgroup_status *cgroup_status, số_nguyên signal, libcrun_error_t *err)
{
  trả_về cgroup_killall_path (cgroup_status->path, signal, err);
}

số_nguyên
libcrun_cgroup_destroy (struct libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager = NULL;
  số_nguyên ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  trả_về cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
static số_nguyên
complete_cpu_max (const char *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free char *cpu_max_path = NULL;
  cleanup_free char *content = NULL;
  bool has_period;
  uint64_t period;
  int64_t quota;
  số_nguyên cgroup_mode;
  số_nguyên ret;

  nếu (resources == NULL || resources->cpu == NULL)
    trả_về 0;

  cpu = resources->cpu;
  nếu (cpu->quota_present == cpu->period_present)
    trả_về 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;
  nếu (cgroup_mode != CGROUP_MODE_UNIFIED)
    trả_về 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_ROOT, path, "cpu.max", NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = read_all_file (cpu_max_path, &content, NULL, err);
  nếu (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      trả_về 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  không_thì nếu (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  trả_về 0;
}

số_nguyên
libcrun_update_cgroup_resources (struct libcrun_cgroup_status *cgroup_status,
                                 const char *state_root,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager = NULL;
  số_nguyên ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_root, resources, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
    }
  trả_về update_cgroup_resources (cgroup_status->path, state_root, resources, ! cgroup_status->bpf_dev_set, err);
}

static số_nguyên
can_ignore_cgroup_enter_errors (struct libcrun_cgroup_args *args, số_nguyên cgroup_mode,
                                libcrun_error_t *err)
{
  số_nguyên manager = args->manager;
  số_nguyên rootless;

  rootless = is_rootless (err);
  nếu (UNLIKELY (rootless < 0))
    trả_về rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  nếu (! rootless)
    trả_về 0;

  nếu (! is_empty_string (args->cgroup_path))
    trả_về 0;

  nếu (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    trả_về 0;

  trả_về 1;
}

số_nguyên
libcrun_cgroup_preenter (struct libcrun_cgroup_args *args, số_nguyên *dirfd, libcrun_error_t *err)
{
  struct libcrun_cgroup_manager *cgroup_manager;
  số_nguyên cgroup_mode;
  số_nguyên ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;

  nếu (cgroup_mode != CGROUP_MODE_UNIFIED)
    trả_về 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (cgroup_manager->precreate_cgroup == NULL)
    trả_về 0;

  trả_về cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
void
libcrun_cgroup_preenter_cleanup (struct libcrun_cgroup_args *args)
{
  nếu (args->precreated_path == NULL)
    trả_về;

  (void) rmdir (args->precreated_path);
  free (args->precreated_path);
  args->precreated_path = NULL;
}

số_nguyên
libcrun_cgroup_enter (struct libcrun_cgroup_args *args, struct libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status struct libcrun_cgroup_status *status = xmalloc0 (sizeof *status);
  struct libcrun_cgroup_manager *cgroup_manager;
  uid_t root_uid = args->root_uid;
  uid_t root_gid = args->root_gid;
  số_nguyên cgroup_mode;
  số_nguyên ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;

  nếu (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free char *buffer = NULL;

      ret = read_all_file (CGROUP_ROOT "/unified/cgroup.controllers", &buffer, &len, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
      nếu (len > 0)
        trả_về crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  nếu (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = NULL;
      số_nguyên ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      nếu (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          trả_về ignore_cgroup_errors;
        }

      nếu (ignore_cgroup_errors)
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

      trả_về ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  free (args->precreated_path);
  args->precreated_path = NULL;

  nếu (status->path)
    {
      bool need_chown;

      need_chown = root_uid != (uid_t) -1 || root_gid != (gid_t) -1;
      nếu (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, root_uid, root_gid, err);
          nếu (UNLIKELY (ret < 0))
            goto fail_destroy;
        }

      nếu (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_root, args->resources, ! status->bpf_dev_set, err);
          nếu (UNLIKELY (ret < 0))
            goto fail_destroy;
        }
    }
success:
  *out = status;
  status = NULL;
  trả_về 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = NULL;

    nếu (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  trả_về ret;
}

số_nguyên
libcrun_cgroup_enter_finalize (struct libcrun_cgroup_args *args, struct libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free char *target_cgroup = NULL;
  cleanup_free char *content = NULL;
  const char *delegate_cgroup;
  cleanup_free char *dir = NULL;
  char *current_cgroup, *to;
  số_nguyên cgroup_mode;
  số_nguyên ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  nếu (delegate_cgroup == NULL)
    trả_về 0;

  nếu (path_has_dot_dot_component (delegate_cgroup))
    trả_về crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;

  nếu (cgroup_mode != CGROUP_MODE_UNIFIED)
    trả_về crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free char *proc_path = NULL;
  cleanup_close số_nguyên fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  nếu (UNLIKELY (fd < 0))
    trả_về fd;

  ret = read_all_fd (fd, "cgroup path", &content, NULL, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  current_cgroup = strstr (content, "0::");
  nếu (UNLIKELY (current_cgroup == NULL))
    trả_về crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  nếu (UNLIKELY (to == NULL))
    trả_về crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = append_paths (&dir, err, CGROUP_ROOT, target_cgroup, NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = move_process_to_cgroup (args->pid, NULL, target_cgroup, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = enable_controllers (target_cgroup, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = chown_cgroups (target_cgroup, args->root_uid, args->root_gid, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  trả_về 0;
}

số_nguyên
libcrun_cgroup_has_oom (struct libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  const char *path = status->path;
  const char *prefix = NULL;
  size_t content_size = 0;
  số_nguyên cgroup_mode;
  char *it;

  nếu (UNLIKELY (path == NULL || path[0] == '\0'))
    trả_về 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;

  switch (cgroup_mode)
    {
    case CGROUP_MODE_UNIFIED:
      {
        cleanup_free char *events_path = NULL;
        số_nguyên ret;

        ret = append_paths (&events_path, err, CGROUP_ROOT, path, "memory.events", NULL);
        nếu (UNLIKELY (ret < 0))
          trả_về ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        nếu (UNLIKELY (ret < 0))
          trả_về ret;

        prefix = "oom ";
        break;
      }
    case CGROUP_MODE_LEGACY:
    case CGROUP_MODE_HYBRID:
      {
        cleanup_free char *oom_control_path = NULL;
        số_nguyên ret;

        ret = append_paths (&oom_control_path, err, CGROUP_ROOT, "memory", path, "memory.oom_control", NULL);
        nếu (UNLIKELY (ret < 0))
          trả_về ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        nếu (UNLIKELY (ret < 0))
          trả_về ret;

        prefix = "oom_kill ";
        break;
      }

    default:
      trả_về crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  trong_khi (it && *it)
    {
      nếu (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          trong_khi (*it == ' ')
            it++;

          trả_về *it != '0';
        }
      không_thì
        {
          it = strchr (it, '\n');
          nếu (it == NULL)
            trả_về 0;
          it++;
        }
    }

  trả_về 0;
}

số_nguyên
libcrun_cgroup_get_status (struct libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  trả_về 0;
}

void
libcrun_cgroup_status_free (struct libcrun_cgroup_status *cgroup_status)
{
  nếu (cgroup_status == NULL)
    trả_về;

  free (cgroup_status->path);
  free (cgroup_status->scope);
  free (cgroup_status);
}

struct libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  struct libcrun_cgroup_status *ret;

  ret = xmalloc0 (sizeof *ret);

  nếu (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  nếu (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  nếu (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  không_thì
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  trả_về ret;
}
