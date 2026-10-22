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

#ضمن <config.h>
#ضمن "cgroup.h"
#ضمن "cgroup-internal.h"
#ضمن "cgroup-systemd.h"
#ضمن "cgroup-utils.h"
#ضمن "ebpf.h"
#ضمن "utils.h"
#ضمن "status.h"
#ضمن <string.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <sys/vfs.h>
#ضمن <inttypes.h>
#ضمن <time.h>

#ضمن <sched.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <libgen.h>

ساكن صحيح
initialize_cpuset_subsystem_rec (حرف *path, size_t path_len, حرف *cpus, حرف *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free حرف *allocated_cpus = عدم;
  cleanup_free حرف *allocated_mems = عدم;
  cleanup_close صحيح dirfd = -1;
  cleanup_close صحيح mems_fd = -1;
  cleanup_close صحيح cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  صحيح b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ܐܢ (UNLIKELY (dirfd < 0))
    ܦܢܐ crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  ܐܢ (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      ܐܢ (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      ܐܢ (UNLIKELY (cpus_fd < 0))
        ܦܢܐ crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (اقرأ (cpus_fd, cpus, 256));
      ܐܢ (UNLIKELY (b_len < 0))
        ܦܢܐ crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      ܐܢ (cpus[0] == '\n')
        cpus[0] = '\0';
      ܐܢ (cpus[0] != '\0')
        has_cpus = true;
    }

  ܐܢ (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      ܐܢ (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      ܐܢ (UNLIKELY (mems_fd < 0))
        ܦܢܐ crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (اقرأ (mems_fd, mems, 256));
      ܐܢ (UNLIKELY (b_len < 0))
        ܦܢܐ crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      ܐܢ (mems[0] == '\n')
        mems[0] = '\0';
      ܐܢ (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  ܐܢ (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      صحيح ret;

      لكل (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      ܐܢ (parent_path_len == 1)
        ܦܢܐ 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      ܐܢ (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  ܐܢ (resources && resources->cpu)
    {
      ܐܢ (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      ܐܢ (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  ܐܢ (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (اكتب (cpus_fd, cpus, strlen (cpus)));
      ܐܢ (UNLIKELY (b_len < 0))
        ܦܢܐ crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  ܐܢ (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (اكتب (mems_fd, mems, strlen (mems)));
      ܐܢ (UNLIKELY (b_len < 0))
        ܦܢܐ crun_make_error (err, errno, "write `cpuset.mems`");
    }

  ܦܢܐ 0;
}

صحيح
initialize_cpuset_subsystem (ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *tmp_path = xstrdup (path);
  حرف cpus_buf[257];
  حرف mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ܦܢܐ initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, عدم, err);
}

صحيح
initialize_cpuset_subsystem_resources (ثابت حرف *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free حرف *tmp_path = xstrdup (path);
  حرف cpus_buf[257];
  حرف mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ܦܢܐ initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

ساكن صحيح
initialize_memory_subsystem (ثابت حرف *path, libcrun_error_t *err)
{
  ثابت حرف *ثابت files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", عدم };
  cleanup_close صحيح dirfd = -1;
  صحيح i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ܐܢ (UNLIKELY (dirfd < 0))
    ܦܢܐ crun_make_error (err, errno, "open `%s`", path);

  لكل (i = 0; files[i]; i++)
    {
      صحيح ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      ܐܢ (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  ܦܢܐ 0;
}

صحيح
enter_cgroup_v1_subsystem (pid_t pid, ثابت حرف *subsystem, ثابت حرف *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path = عدم;
  صحيح ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path ? path : "", عدم);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ܐܢ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      ܐܢ (UNLIKELY (ret < 0))
        {
          ܐܢ (crun_error_get_errno (err) != EROFS)
            ܦܢܐ crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          ܦܢܐ 0;
        }

      ܐܢ (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          ܐܢ (UNLIKELY (ret < 0))
            ܦܢܐ ret;
        }
      ܐܢ (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          ܐܢ (UNLIKELY (ret < 0))
            ܦܢܐ ret;
        }
    }
  ܐܚܪܢܐ
    {
      ret = crun_path_exists (cgroup_path, err);
      ܐܢ (UNLIKELY (ret < 0))
        ܦܢܐ ret;
      ܐܢ (ret == 0)
        ܦܢܐ 0;
    }

  ܦܢܐ move_process_to_cgroup (pid, subsystem, path, err);
}

ساكن صحيح
get_file_owner (ثابت حرف *path, uid_t *uid, gid_t *gid)
{
  بنية stat st;
  صحيح ret;

#ifdef HAVE_STATX
  بنية statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  ܐܢ (UNLIKELY (ret < 0))
    {
      ܐܢ (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;

      ܦܢܐ ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  ܦܢܐ ret;

fallback:
#endif
  ret = stat (path, &st);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  ܦܢܐ ret;
}

ساكن صحيح
copy_owner (ثابت حرف *from, ثابت حرف *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  صحيح ret;

  ret = get_file_owner (from, &uid, &gid);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  ܐܢ (uid == 0 && gid == 0)
    ܦܢܐ 0;

  ܦܢܐ chown_cgroups (to, uid, gid, err);
}

ساكن صحيح
read_unified_cgroup_pid (pid_t pid, حرف **path, libcrun_error_t *err)
{
  صحيح ret;
  حرف cgroup_path[32];
  حرف *from, *to;
  cleanup_free حرف *content = عدم;
  صحيح len;

  len = snprintf (cgroup_path, حجم (cgroup_path), "/proc/%d/cgroup", pid);
  ܐܢ (UNLIKELY (len >= (صحيح) حجم (cgroup_path)))
    ܦܢܐ crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, عدم, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  from = strstr (content, "0::");
  ܐܢ (UNLIKELY (from == عدم))
    ܦܢܐ crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  ܐܢ (UNLIKELY (to == عدم))
    ܦܢܐ crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  ܦܢܐ 0;
}

ساكن صحيح
enter_cgroup_v1 (pid_t pid, ثابت حرف *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  bool entered_any = false;
  size_t content_size;
  حرف *controller;
  حرف *saveptr;
  bool has_data;
  صحيح rootless;
  صحيح ret;

  rootless = is_rootless (err);
  ܐܢ (UNLIKELY (rootless < 0))
    ܦܢܐ rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  ܐܢ (UNLIKELY (ret < 0))
    {
      ܐܢ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ܦܢܐ 0;
        }
      ܦܢܐ ret;
    }

  لكل (has_data = read_proc_cgroup (content, &saveptr, عدم, &controller, عدم);
       has_data;
       has_data = read_proc_cgroup (عدم, &saveptr, عدم, &controller, عدم))
    {
      حرف subsystem_path[64];
      حرف *subsystem;
      size_t len;

      ܐܢ (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      ܐܢ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ܐܢ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, حجم (subsystem_path), CGROUP_ROOT "/%s", subsystem);
      ܐܢ (UNLIKELY (len >= (صحيح) حجم (subsystem_path)))
        ܦܢܐ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      ܐܢ (UNLIKELY (ret < 0))
        ܦܢܐ ret;
      ܐܢ (ret == 0)
        استمر;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      ܐܢ (UNLIKELY (ret < 0))
        {
          صحيح errcode = crun_error_get_errno (err);
          ܐܢ (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              استمر;
            }
          ܦܢܐ ret;
        }
    }

  ܐܢ (entered_any)
    ܦܢܐ 0;

  ܦܢܐ crun_make_error (err, 0, "could not join cgroup");
}

ساكن صحيح
enter_cgroup_v2 (pid_t pid, pid_t init_pid, ثابت حرف *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path_procs = عدم;
  cleanup_free حرف *cgroup_path = عدم;
  حرف pid_str[16];
  صحيح repeat;
  صحيح len;
  صحيح ret;

  len = snprintf (pid_str, حجم (pid_str), "%d", pid);
  ܐܢ (UNLIKELY (len >= (صحيح) حجم (pid_str)))
    ܦܢܐ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, عدم);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ܐܢ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      ܐܢ (UNLIKELY (ret < 0))
        ܦܢܐ ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", عدم);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  ܐܢ (LIKELY (ret >= 0))
    ܦܢܐ ret;

  ܐܢ (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      ܐܢ (UNLIKELY (ret < 0))
        ܦܢܐ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      ܐܢ (LIKELY (ret >= 0))
        ܦܢܐ ret;
    }

  ܐܢ (create_if_missing || crun_error_get_errno (err) != EBUSY)
    ܦܢܐ ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  لكل (repeat = 0;; repeat++)
    {
      cleanup_free حرف *cgroup_crun_exec_path = عدم;
      cleanup_free حرف *cgroup_sub_path_procs = عدم;

      /* There is an init pid, try to join its cgroup.  */
      ܐܢ (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          ܐܢ (UNLIKELY (ret < 0))
            ܦܢܐ ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          ܐܢ (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              حرر (cgroup_crun_exec_path);
              cgroup_crun_exec_path = عدم;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      ܐܢ (cgroup_crun_exec_path == عدم)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      ܐܢ (UNLIKELY (ret < 0))
        ܦܢܐ ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_ROOT, cgroup_crun_exec_path, "cgroup.procs", عدم);
      ܐܢ (UNLIKELY (ret < 0))
        ܦܢܐ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      ܐܢ (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          ܐܢ (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              استمر;
            }
          ܦܢܐ ret;
        }
      ܦܢܐ copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  ܦܢܐ ret;
}

صحيح
enter_cgroup (صحيح cgroup_mode, pid_t pid, pid_t init_pid, ثابت حرف *path,
              bool create_if_missing, libcrun_error_t *err)
{
  صحيح ret;
  ܐܢ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      ܐܢ (UNLIKELY (ret < 0))
        ܦܢܐ ret;
    }
  ܐܚܪܢܐ
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      ܐܢ (UNLIKELY (ret < 0))
        ܦܢܐ ret;
    }
  ܦܢܐ 0;
}
