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

static số_nguyên
initialize_cpuset_subsystem_rec (char *path, size_t path_len, char *cpus, char *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free char *allocated_cpus = NULL;
  cleanup_free char *allocated_mems = NULL;
  cleanup_close số_nguyên dirfd = -1;
  cleanup_close số_nguyên mems_fd = -1;
  cleanup_close số_nguyên cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  số_nguyên b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  nếu (UNLIKELY (dirfd < 0))
    trả_về crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  nếu (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      nếu (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      nếu (UNLIKELY (cpus_fd < 0))
        trả_về crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (read (cpus_fd, cpus, 256));
      nếu (UNLIKELY (b_len < 0))
        trả_về crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      nếu (cpus[0] == '\n')
        cpus[0] = '\0';
      nếu (cpus[0] != '\0')
        has_cpus = true;
    }

  nếu (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      nếu (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      nếu (UNLIKELY (mems_fd < 0))
        trả_về crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (read (mems_fd, mems, 256));
      nếu (UNLIKELY (b_len < 0))
        trả_về crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      nếu (mems[0] == '\n')
        mems[0] = '\0';
      nếu (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  nếu (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      số_nguyên ret;

      cho (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      nếu (parent_path_len == 1)
        trả_về 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      nếu (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  nếu (resources && resources->cpu)
    {
      nếu (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      nếu (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  nếu (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (write (cpus_fd, cpus, strlen (cpus)));
      nếu (UNLIKELY (b_len < 0))
        trả_về crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  nếu (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (write (mems_fd, mems, strlen (mems)));
      nếu (UNLIKELY (b_len < 0))
        trả_về crun_make_error (err, errno, "write `cpuset.mems`");
    }

  trả_về 0;
}

số_nguyên
initialize_cpuset_subsystem (const char *path, libcrun_error_t *err)
{
  cleanup_free char *tmp_path = xstrdup (path);
  char cpus_buf[257];
  char mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  trả_về initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, NULL, err);
}

số_nguyên
initialize_cpuset_subsystem_resources (const char *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free char *tmp_path = xstrdup (path);
  char cpus_buf[257];
  char mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  trả_về initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

static số_nguyên
initialize_memory_subsystem (const char *path, libcrun_error_t *err)
{
  const char *const files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", NULL };
  cleanup_close số_nguyên dirfd = -1;
  số_nguyên i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  nếu (UNLIKELY (dirfd < 0))
    trả_về crun_make_error (err, errno, "open `%s`", path);

  cho (i = 0; files[i]; i++)
    {
      số_nguyên ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      nếu (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  trả_về 0;
}

số_nguyên
enter_cgroup_v1_subsystem (pid_t pid, const char *subsystem, const char *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  số_nguyên ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path ? path : "", NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      nếu (UNLIKELY (ret < 0))
        {
          nếu (crun_error_get_errno (err) != EROFS)
            trả_về crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          trả_về 0;
        }

      nếu (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
        }
      nếu (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
        }
    }
  không_thì
    {
      ret = crun_path_exists (cgroup_path, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
      nếu (ret == 0)
        trả_về 0;
    }

  trả_về move_process_to_cgroup (pid, subsystem, path, err);
}

static số_nguyên
get_file_owner (const char *path, uid_t *uid, gid_t *gid)
{
  struct stat st;
  số_nguyên ret;

#ifdef HAVE_STATX
  struct statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  nếu (UNLIKELY (ret < 0))
    {
      nếu (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      trả_về ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  trả_về ret;

fallback:
#endif
  ret = stat (path, &st);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  trả_về ret;
}

static số_nguyên
copy_owner (const char *from, const char *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  số_nguyên ret;

  ret = get_file_owner (from, &uid, &gid);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  nếu (uid == 0 && gid == 0)
    trả_về 0;

  trả_về chown_cgroups (to, uid, gid, err);
}

static số_nguyên
read_unified_cgroup_pid (pid_t pid, char **path, libcrun_error_t *err)
{
  số_nguyên ret;
  char cgroup_path[32];
  char *from, *to;
  cleanup_free char *content = NULL;
  số_nguyên len;

  len = snprintf (cgroup_path, sizeof (cgroup_path), "/proc/%d/cgroup", pid);
  nếu (UNLIKELY (len >= (số_nguyên) sizeof (cgroup_path)))
    trả_về crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, NULL, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  from = strstr (content, "0::");
  nếu (UNLIKELY (from == NULL))
    trả_về crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  nếu (UNLIKELY (to == NULL))
    trả_về crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  trả_về 0;
}

static số_nguyên
enter_cgroup_v1 (pid_t pid, const char *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  bool entered_any = false;
  size_t content_size;
  char *controller;
  char *saveptr;
  bool has_data;
  số_nguyên rootless;
  số_nguyên ret;

  rootless = is_rootless (err);
  nếu (UNLIKELY (rootless < 0))
    trả_về rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  nếu (UNLIKELY (ret < 0))
    {
      nếu (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          trả_về 0;
        }
      trả_về ret;
    }

  cho (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
    {
      char subsystem_path[64];
      char *subsystem;
      size_t len;

      nếu (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      nếu (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      nếu (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, sizeof (subsystem_path), CGROUP_ROOT "/%s", subsystem);
      nếu (UNLIKELY (len >= (số_nguyên) sizeof (subsystem_path)))
        trả_về crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
      nếu (ret == 0)
        continue;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      nếu (UNLIKELY (ret < 0))
        {
          số_nguyên errcode = crun_error_get_errno (err);
          nếu (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              continue;
            }
          trả_về ret;
        }
    }

  nếu (entered_any)
    trả_về 0;

  trả_về crun_make_error (err, 0, "could not join cgroup");
}

static số_nguyên
enter_cgroup_v2 (pid_t pid, pid_t init_pid, const char *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_procs = NULL;
  cleanup_free char *cgroup_path = NULL;
  char pid_str[16];
  số_nguyên repeat;
  số_nguyên len;
  số_nguyên ret;

  len = snprintf (pid_str, sizeof (pid_str), "%d", pid);
  nếu (UNLIKELY (len >= (số_nguyên) sizeof (pid_str)))
    trả_về crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  nếu (LIKELY (ret >= 0))
    trả_về ret;

  nếu (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      nếu (LIKELY (ret >= 0))
        trả_về ret;
    }

  nếu (create_if_missing || crun_error_get_errno (err) != EBUSY)
    trả_về ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  cho (repeat = 0;; repeat++)
    {
      cleanup_free char *cgroup_crun_exec_path = NULL;
      cleanup_free char *cgroup_sub_path_procs = NULL;

      /* There is an init pid, try to join its cgroup.  */
      nếu (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          nếu (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              free (cgroup_crun_exec_path);
              cgroup_crun_exec_path = NULL;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      nếu (cgroup_crun_exec_path == NULL)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_ROOT, cgroup_crun_exec_path, "cgroup.procs", NULL);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      nếu (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          nếu (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              continue;
            }
          trả_về ret;
        }
      trả_về copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  trả_về ret;
}

số_nguyên
enter_cgroup (số_nguyên cgroup_mode, pid_t pid, pid_t init_pid, const char *path,
              bool create_if_missing, libcrun_error_t *err)
{
  số_nguyên ret;
  nếu (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
    }
  không_thì
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
    }
  trả_về 0;
}
