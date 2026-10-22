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

static ចំនួនគត់
initialize_cpuset_subsystem_rec (var_na *path, size_t path_len, var_na *cpus, var_na *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free var_na *allocated_cpus = NULL;
  cleanup_free var_na *allocated_mems = NULL;
  cleanup_close ចំនួនគត់ dirfd = -1;
  cleanup_close ចំនួនគត់ mems_fd = -1;
  cleanup_close ចំនួនគត់ cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  ចំនួនគត់ b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ប្រសិនបើ (UNLIKELY (dirfd < 0))
    ត្រឡប់ crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  ប្រសិនបើ (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      ប្រសិនបើ (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      ប្រសិនបើ (UNLIKELY (cpus_fd < 0))
        ត្រឡប់ crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (read (cpus_fd, cpus, 256));
      ប្រសិនបើ (UNLIKELY (b_len < 0))
        ត្រឡប់ crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      ប្រសិនបើ (cpus[0] == '\n')
        cpus[0] = '\0';
      ប្រសិនបើ (cpus[0] != '\0')
        has_cpus = true;
    }

  ប្រសិនបើ (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      ប្រសិនបើ (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      ប្រសិនបើ (UNLIKELY (mems_fd < 0))
        ត្រឡប់ crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (read (mems_fd, mems, 256));
      ប្រសិនបើ (UNLIKELY (b_len < 0))
        ត្រឡប់ crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      ប្រសិនបើ (mems[0] == '\n')
        mems[0] = '\0';
      ប្រសិនបើ (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  ប្រសិនបើ (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      ចំនួនគត់ ret;

      សម្រាប់ (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      ប្រសិនបើ (parent_path_len == 1)
        ត្រឡប់ 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  ប្រសិនបើ (resources && resources->cpu)
    {
      ប្រសិនបើ (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      ប្រសិនបើ (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  ប្រសិនបើ (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (write (cpus_fd, cpus, strlen (cpus)));
      ប្រសិនបើ (UNLIKELY (b_len < 0))
        ត្រឡប់ crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  ប្រសិនបើ (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (write (mems_fd, mems, strlen (mems)));
      ប្រសិនបើ (UNLIKELY (b_len < 0))
        ត្រឡប់ crun_make_error (err, errno, "write `cpuset.mems`");
    }

  ត្រឡប់ 0;
}

ចំនួនគត់
initialize_cpuset_subsystem (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *tmp_path = xstrdup (path);
  var_na cpus_buf[257];
  var_na mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ត្រឡប់ initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, NULL, err);
}

ចំនួនគត់
initialize_cpuset_subsystem_resources (const var_na *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free var_na *tmp_path = xstrdup (path);
  var_na cpus_buf[257];
  var_na mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ត្រឡប់ initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

static ចំនួនគត់
initialize_memory_subsystem (const var_na *path, libcrun_error_t *err)
{
  const var_na *const files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", NULL };
  cleanup_close ចំនួនគត់ dirfd = -1;
  ចំនួនគត់ i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ប្រសិនបើ (UNLIKELY (dirfd < 0))
    ត្រឡប់ crun_make_error (err, errno, "open `%s`", path);

  សម្រាប់ (i = 0; files[i]; i++)
    {
      ចំនួនគត់ ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  ត្រឡប់ 0;
}

ចំនួនគត់
enter_cgroup_v1_subsystem (pid_t pid, const var_na *subsystem, const var_na *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  ចំនួនគត់ ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path ? path : "", NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          ប្រសិនបើ (crun_error_get_errno (err) != EROFS)
            ត្រឡប់ crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          ត្រឡប់ 0;
        }

      ប្រសិនបើ (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
        }
      ប្រសិនបើ (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
        }
    }
  បើមិនដូច្នេះ
    {
      ret = crun_path_exists (cgroup_path, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
      ប្រសិនបើ (ret == 0)
        ត្រឡប់ 0;
    }

  ត្រឡប់ move_process_to_cgroup (pid, subsystem, path, err);
}

static ចំនួនគត់
get_file_owner (const var_na *path, uid_t *uid, gid_t *gid)
{
  sa_nracanaa stat st;
  ចំនួនគត់ ret;

#ifdef HAVE_STATX
  sa_nracanaa statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      ប្រសិនបើ (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      ត្រឡប់ ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  ត្រឡប់ ret;

fallback:
#endif
  ret = stat (path, &st);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  ត្រឡប់ ret;
}

static ចំនួនគត់
copy_owner (const var_na *from, const var_na *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  ចំនួនគត់ ret;

  ret = get_file_owner (from, &uid, &gid);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  ប្រសិនបើ (uid == 0 && gid == 0)
    ត្រឡប់ 0;

  ត្រឡប់ chown_cgroups (to, uid, gid, err);
}

static ចំនួនគត់
read_unified_cgroup_pid (pid_t pid, var_na **path, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  var_na cgroup_path[32];
  var_na *from, *to;
  cleanup_free var_na *content = NULL;
  ចំនួនគត់ len;

  len = snprintf (cgroup_path, parimaa_na (cgroup_path), "/proc/%d/cgroup", pid);
  ប្រសិនបើ (UNLIKELY (len >= (ចំនួនគត់) parimaa_na (cgroup_path)))
    ត្រឡប់ crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, NULL, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  from = strstr (content, "0::");
  ប្រសិនបើ (UNLIKELY (from == NULL))
    ត្រឡប់ crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  ប្រសិនបើ (UNLIKELY (to == NULL))
    ត្រឡប់ crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  ត្រឡប់ 0;
}

static ចំនួនគត់
enter_cgroup_v1 (pid_t pid, const var_na *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  bool entered_any = false;
  size_t content_size;
  var_na *controller;
  var_na *saveptr;
  bool has_data;
  ចំនួនគត់ rootless;
  ចំនួនគត់ ret;

  rootless = is_rootless (err);
  ប្រសិនបើ (UNLIKELY (rootless < 0))
    ត្រឡប់ rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      ប្រសិនបើ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ត្រឡប់ 0;
        }
      ត្រឡប់ ret;
    }

  សម្រាប់ (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
    {
      var_na subsystem_path[64];
      var_na *subsystem;
      size_t len;

      ប្រសិនបើ (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      ប្រសិនបើ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ប្រសិនបើ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, parimaa_na (subsystem_path), CGROUP_ROOT "/%s", subsystem);
      ប្រសិនបើ (UNLIKELY (len >= (ចំនួនគត់) parimaa_na (subsystem_path)))
        ត្រឡប់ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
      ប្រសិនបើ (ret == 0)
        continue;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          ចំនួនគត់ errcode = crun_error_get_errno (err);
          ប្រសិនបើ (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              continue;
            }
          ត្រឡប់ ret;
        }
    }

  ប្រសិនបើ (entered_any)
    ត្រឡប់ 0;

  ត្រឡប់ crun_make_error (err, 0, "could not join cgroup");
}

static ចំនួនគត់
enter_cgroup_v2 (pid_t pid, pid_t init_pid, const var_na *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path_procs = NULL;
  cleanup_free var_na *cgroup_path = NULL;
  var_na pid_str[16];
  ចំនួនគត់ repeat;
  ចំនួនគត់ len;
  ចំនួនគត់ ret;

  len = snprintf (pid_str, parimaa_na (pid_str), "%d", pid);
  ប្រសិនបើ (UNLIKELY (len >= (ចំនួនគត់) parimaa_na (pid_str)))
    ត្រឡប់ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  ប្រសិនបើ (LIKELY (ret >= 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      ប្រសិនបើ (LIKELY (ret >= 0))
        ត្រឡប់ ret;
    }

  ប្រសិនបើ (create_if_missing || crun_error_get_errno (err) != EBUSY)
    ត្រឡប់ ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  សម្រាប់ (repeat = 0;; repeat++)
    {
      cleanup_free var_na *cgroup_crun_exec_path = NULL;
      cleanup_free var_na *cgroup_sub_path_procs = NULL;

      /* There is an init pid, try to join its cgroup.  */
      ប្រសិនបើ (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          ប្រសិនបើ (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              free (cgroup_crun_exec_path);
              cgroup_crun_exec_path = NULL;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      ប្រសិនបើ (cgroup_crun_exec_path == NULL)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_ROOT, cgroup_crun_exec_path, "cgroup.procs", NULL);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          ប្រសិនបើ (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              continue;
            }
          ត្រឡប់ ret;
        }
      ត្រឡប់ copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  ត្រឡប់ ret;
}

ចំនួនគត់
enter_cgroup (ចំនួនគត់ cgroup_mode, pid_t pid, pid_t init_pid, const var_na *path,
              bool create_if_missing, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  ប្រសិនបើ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }
  បើមិនដូច្នេះ
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }
  ត្រឡប់ 0;
}
