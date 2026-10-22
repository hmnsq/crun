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
#include "cgroup-internal.h"
#include "cgroup-systemd.h"
#include "cgroup-utils.h"
#include "ebpf.h"
#include "utils.h"
#include "status.h"
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/vfs.h>
#include <inttypes.h>
#include <time.h>

#include <sched.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <libgen.h>

static цео
initialize_cpuset_subsystem_rec (char *path, size_t path_len, char *cpus, char *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free char *allocated_cpus = NULL;
  cleanup_free char *allocated_mems = NULL;
  cleanup_close цео dirfd = -1;
  cleanup_close цео mems_fd = -1;
  cleanup_close цео cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  цео b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ако (UNLIKELY (dirfd < 0))
    врати crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  ако (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      ако (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      ако (UNLIKELY (cpus_fd < 0))
        врати crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (read (cpus_fd, cpus, 256));
      ако (UNLIKELY (b_len < 0))
        врати crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      ако (cpus[0] == '\n')
        cpus[0] = '\0';
      ако (cpus[0] != '\0')
        has_cpus = true;
    }

  ако (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      ако (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      ако (UNLIKELY (mems_fd < 0))
        врати crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (read (mems_fd, mems, 256));
      ако (UNLIKELY (b_len < 0))
        врати crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      ако (mems[0] == '\n')
        mems[0] = '\0';
      ако (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  ако (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      цео ret;

      за (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      ако (parent_path_len == 1)
        врати 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      ако (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  ако (resources && resources->cpu)
    {
      ако (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      ако (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  ако (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (write (cpus_fd, cpus, strlen (cpus)));
      ако (UNLIKELY (b_len < 0))
        врати crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  ако (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (write (mems_fd, mems, strlen (mems)));
      ако (UNLIKELY (b_len < 0))
        врати crun_make_error (err, errno, "write `cpuset.mems`");
    }

  врати 0;
}

цео
initialize_cpuset_subsystem (const char *path, libcrun_error_t *err)
{
  cleanup_free char *tmp_path = xstrdup (path);
  char cpus_buf[257];
  char mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  врати initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, NULL, err);
}

цео
initialize_cpuset_subsystem_resources (const char *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free char *tmp_path = xstrdup (path);
  char cpus_buf[257];
  char mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  врати initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

static цео
initialize_memory_subsystem (const char *path, libcrun_error_t *err)
{
  const char *const files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", NULL };
  cleanup_close цео dirfd = -1;
  цео i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ако (UNLIKELY (dirfd < 0))
    врати crun_make_error (err, errno, "open `%s`", path);

  за (i = 0; files[i]; i++)
    {
      цео ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      ако (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  врати 0;
}

цео
enter_cgroup_v1_subsystem (pid_t pid, const char *subsystem, const char *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  цео ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path ? path : "", NULL);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ако (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      ако (UNLIKELY (ret < 0))
        {
          ако (crun_error_get_errno (err) != EROFS)
            врати crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          врати 0;
        }

      ако (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          ако (UNLIKELY (ret < 0))
            врати ret;
        }
      ако (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          ако (UNLIKELY (ret < 0))
            врати ret;
        }
    }
  инако
    {
      ret = crun_path_exists (cgroup_path, err);
      ако (UNLIKELY (ret < 0))
        врати ret;
      ако (ret == 0)
        врати 0;
    }

  врати move_process_to_cgroup (pid, subsystem, path, err);
}

static цео
get_file_owner (const char *path, uid_t *uid, gid_t *gid)
{
  struct stat st;
  цео ret;

#ifdef HAVE_STATX
  struct statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  ако (UNLIKELY (ret < 0))
    {
      ако (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      врати ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  врати ret;

fallback:
#endif
  ret = stat (path, &st);
  ако (UNLIKELY (ret < 0))
    врати ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  врати ret;
}

static цео
copy_owner (const char *from, const char *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  цео ret;

  ret = get_file_owner (from, &uid, &gid);
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  ако (uid == 0 && gid == 0)
    врати 0;

  врати chown_cgroups (to, uid, gid, err);
}

static цео
read_unified_cgroup_pid (pid_t pid, char **path, libcrun_error_t *err)
{
  цео ret;
  char cgroup_path[32];
  char *from, *to;
  cleanup_free char *content = NULL;
  цео len;

  len = snprintf (cgroup_path, sizeof (cgroup_path), "/proc/%d/cgroup", pid);
  ако (UNLIKELY (len >= (цео) sizeof (cgroup_path)))
    врати crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, NULL, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  from = strstr (content, "0::");
  ако (UNLIKELY (from == NULL))
    врати crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  ако (UNLIKELY (to == NULL))
    врати crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  врати 0;
}

static цео
enter_cgroup_v1 (pid_t pid, const char *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  bool entered_any = false;
  size_t content_size;
  char *controller;
  char *saveptr;
  bool has_data;
  цео rootless;
  цео ret;

  rootless = is_rootless (err);
  ако (UNLIKELY (rootless < 0))
    врати rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  ако (UNLIKELY (ret < 0))
    {
      ако (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          врати 0;
        }
      врати ret;
    }

  за (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
    {
      char subsystem_path[64];
      char *subsystem;
      size_t len;

      ако (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      ако (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ако (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, sizeof (subsystem_path), CGROUP_ROOT "/%s", subsystem);
      ако (UNLIKELY (len >= (цео) sizeof (subsystem_path)))
        врати crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      ако (UNLIKELY (ret < 0))
        врати ret;
      ако (ret == 0)
        continue;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      ако (UNLIKELY (ret < 0))
        {
          цео errcode = crun_error_get_errno (err);
          ако (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              continue;
            }
          врати ret;
        }
    }

  ако (entered_any)
    врати 0;

  врати crun_make_error (err, 0, "could not join cgroup");
}

static цео
enter_cgroup_v2 (pid_t pid, pid_t init_pid, const char *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_procs = NULL;
  cleanup_free char *cgroup_path = NULL;
  char pid_str[16];
  цео repeat;
  цео len;
  цео ret;

  len = snprintf (pid_str, sizeof (pid_str), "%d", pid);
  ако (UNLIKELY (len >= (цео) sizeof (pid_str)))
    врати crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ако (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      ако (UNLIKELY (ret < 0))
        врати ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", NULL);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  ако (LIKELY (ret >= 0))
    врати ret;

  ако (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      ако (UNLIKELY (ret < 0))
        врати ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      ако (LIKELY (ret >= 0))
        врати ret;
    }

  ако (create_if_missing || crun_error_get_errno (err) != EBUSY)
    врати ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  за (repeat = 0;; repeat++)
    {
      cleanup_free char *cgroup_crun_exec_path = NULL;
      cleanup_free char *cgroup_sub_path_procs = NULL;

      /* There is an init pid, try to join its cgroup.  */
      ако (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          ако (UNLIKELY (ret < 0))
            врати ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          ако (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              free (cgroup_crun_exec_path);
              cgroup_crun_exec_path = NULL;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      ако (cgroup_crun_exec_path == NULL)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      ако (UNLIKELY (ret < 0))
        врати ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_ROOT, cgroup_crun_exec_path, "cgroup.procs", NULL);
      ако (UNLIKELY (ret < 0))
        врати ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      ако (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          ако (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              continue;
            }
          врати ret;
        }
      врати copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  врати ret;
}

цео
enter_cgroup (цео cgroup_mode, pid_t pid, pid_t init_pid, const char *path,
              bool create_if_missing, libcrun_error_t *err)
{
  цео ret;
  ако (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      ако (UNLIKELY (ret < 0))
        врати ret;
    }
  инако
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      ако (UNLIKELY (ret < 0))
        врати ret;
    }
  врати 0;
}
