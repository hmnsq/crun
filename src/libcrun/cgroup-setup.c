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

static පූර්ණ_සංඛ්‍යා
initialize_cpuset_subsystem_rec (var_na *path, size_t path_len, var_na *cpus, var_na *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free var_na *allocated_cpus = NULL;
  cleanup_free var_na *allocated_mems = NULL;
  cleanup_close පූර්ණ_සංඛ්‍යා dirfd = -1;
  cleanup_close පූර්ණ_සංඛ්‍යා mems_fd = -1;
  cleanup_close පූර්ණ_සංඛ්‍යා cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  පූර්ණ_සංඛ්‍යා b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  නම් (UNLIKELY (dirfd < 0))
    ආපසු crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  නම් (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      නම් (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      නම් (UNLIKELY (cpus_fd < 0))
        ආපසු crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (read (cpus_fd, cpus, 256));
      නම් (UNLIKELY (b_len < 0))
        ආපසු crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      නම් (cpus[0] == '\n')
        cpus[0] = '\0';
      නම් (cpus[0] != '\0')
        has_cpus = true;
    }

  නම් (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      නම් (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      නම් (UNLIKELY (mems_fd < 0))
        ආපසු crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (read (mems_fd, mems, 256));
      නම් (UNLIKELY (b_len < 0))
        ආපසු crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      නම් (mems[0] == '\n')
        mems[0] = '\0';
      නම් (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  නම් (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      පූර්ණ_සංඛ්‍යා ret;

      සඳහා (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      නම් (parent_path_len == 1)
        ආපසු 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      නම් (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  නම් (resources && resources->cpu)
    {
      නම් (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      නම් (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  නම් (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (write (cpus_fd, cpus, strlen (cpus)));
      නම් (UNLIKELY (b_len < 0))
        ආපසු crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  නම් (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (write (mems_fd, mems, strlen (mems)));
      නම් (UNLIKELY (b_len < 0))
        ආපසු crun_make_error (err, errno, "write `cpuset.mems`");
    }

  ආපසු 0;
}

පූර්ණ_සංඛ්‍යා
initialize_cpuset_subsystem (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *tmp_path = xstrdup (path);
  var_na cpus_buf[257];
  var_na mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ආපසු initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, NULL, err);
}

පූර්ණ_සංඛ්‍යා
initialize_cpuset_subsystem_resources (const var_na *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free var_na *tmp_path = xstrdup (path);
  var_na cpus_buf[257];
  var_na mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ආපසු initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

static පූර්ණ_සංඛ්‍යා
initialize_memory_subsystem (const var_na *path, libcrun_error_t *err)
{
  const var_na *const files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", NULL };
  cleanup_close පූර්ණ_සංඛ්‍යා dirfd = -1;
  පූර්ණ_සංඛ්‍යා i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  නම් (UNLIKELY (dirfd < 0))
    ආපසු crun_make_error (err, errno, "open `%s`", path);

  සඳහා (i = 0; files[i]; i++)
    {
      පූර්ණ_සංඛ්‍යා ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      නම් (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  ආපසු 0;
}

පූර්ණ_සංඛ්‍යා
enter_cgroup_v1_subsystem (pid_t pid, const var_na *subsystem, const var_na *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  පූර්ණ_සංඛ්‍යා ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path ? path : "", NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      නම් (UNLIKELY (ret < 0))
        {
          නම් (crun_error_get_errno (err) != EROFS)
            ආපසු crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          ආපසු 0;
        }

      නම් (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          නම් (UNLIKELY (ret < 0))
            ආපසු ret;
        }
      නම් (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          නම් (UNLIKELY (ret < 0))
            ආපසු ret;
        }
    }
  නැතහොත්
    {
      ret = crun_path_exists (cgroup_path, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
      නම් (ret == 0)
        ආපසු 0;
    }

  ආපසු move_process_to_cgroup (pid, subsystem, path, err);
}

static පූර්ණ_සංඛ්‍යා
get_file_owner (const var_na *path, uid_t *uid, gid_t *gid)
{
  sa_nracanaa stat st;
  පූර්ණ_සංඛ්‍යා ret;

#ifdef HAVE_STATX
  sa_nracanaa statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  නම් (UNLIKELY (ret < 0))
    {
      නම් (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      ආපසු ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  ආපසු ret;

fallback:
#endif
  ret = stat (path, &st);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා
copy_owner (const var_na *from, const var_na *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  පූර්ණ_සංඛ්‍යා ret;

  ret = get_file_owner (from, &uid, &gid);
  නම් (UNLIKELY (ret < 0))
    ආපසු crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  නම් (uid == 0 && gid == 0)
    ආපසු 0;

  ආපසු chown_cgroups (to, uid, gid, err);
}

static පූර්ණ_සංඛ්‍යා
read_unified_cgroup_pid (pid_t pid, var_na **path, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;
  var_na cgroup_path[32];
  var_na *from, *to;
  cleanup_free var_na *content = NULL;
  පූර්ණ_සංඛ්‍යා len;

  len = snprintf (cgroup_path, parimaa_na (cgroup_path), "/proc/%d/cgroup", pid);
  නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (cgroup_path)))
    ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, NULL, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  from = strstr (content, "0::");
  නම් (UNLIKELY (from == NULL))
    ආපසු crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  නම් (UNLIKELY (to == NULL))
    ආපසු crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
enter_cgroup_v1 (pid_t pid, const var_na *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  bool entered_any = false;
  size_t content_size;
  var_na *controller;
  var_na *saveptr;
  bool has_data;
  පූර්ණ_සංඛ්‍යා rootless;
  පූර්ණ_සංඛ්‍යා ret;

  rootless = is_rootless (err);
  නම් (UNLIKELY (rootless < 0))
    ආපසු rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  නම් (UNLIKELY (ret < 0))
    {
      නම් (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ආපසු 0;
        }
      ආපසු ret;
    }

  සඳහා (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
    {
      var_na subsystem_path[64];
      var_na *subsystem;
      size_t len;

      නම් (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      නම් (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      නම් (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, parimaa_na (subsystem_path), CGROUP_ROOT "/%s", subsystem);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (subsystem_path)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
      නම් (ret == 0)
        continue;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      නම් (UNLIKELY (ret < 0))
        {
          පූර්ණ_සංඛ්‍යා errcode = crun_error_get_errno (err);
          නම් (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              continue;
            }
          ආපසු ret;
        }
    }

  නම් (entered_any)
    ආපසු 0;

  ආපසු crun_make_error (err, 0, "could not join cgroup");
}

static පූර්ණ_සංඛ්‍යා
enter_cgroup_v2 (pid_t pid, pid_t init_pid, const var_na *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path_procs = NULL;
  cleanup_free var_na *cgroup_path = NULL;
  var_na pid_str[16];
  පූර්ණ_සංඛ්‍යා repeat;
  පූර්ණ_සංඛ්‍යා len;
  පූර්ණ_සංඛ්‍යා ret;

  len = snprintf (pid_str, parimaa_na (pid_str), "%d", pid);
  නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (pid_str)))
    ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  නම් (LIKELY (ret >= 0))
    ආපසු ret;

  නම් (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      නම් (LIKELY (ret >= 0))
        ආපසු ret;
    }

  නම් (create_if_missing || crun_error_get_errno (err) != EBUSY)
    ආපසු ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  සඳහා (repeat = 0;; repeat++)
    {
      cleanup_free var_na *cgroup_crun_exec_path = NULL;
      cleanup_free var_na *cgroup_sub_path_procs = NULL;

      /* There is an init pid, try to join its cgroup.  */
      නම් (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          නම් (UNLIKELY (ret < 0))
            ආපසු ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          නම් (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              free (cgroup_crun_exec_path);
              cgroup_crun_exec_path = NULL;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      නම් (cgroup_crun_exec_path == NULL)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_ROOT, cgroup_crun_exec_path, "cgroup.procs", NULL);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      නම් (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          නම් (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              continue;
            }
          ආපසු ret;
        }
      ආපසු copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  ආපසු ret;
}

පූර්ණ_සංඛ්‍යා
enter_cgroup (පූර්ණ_සංඛ්‍යා cgroup_mode, pid_t pid, pid_t init_pid, const var_na *path,
              bool create_if_missing, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;
  නම් (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නැතහොත්
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  ආපසු 0;
}
