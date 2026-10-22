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

#شامل <config.h>
#شامل "cgroup.h"
#شامل "cgroup-internal.h"
#شامل "cgroup-systemd.h"
#شامل "cgroup-utils.h"
#شامل "ebpf.h"
#شامل "utils.h"
#شامل "status.h"
#شامل <string.h>
#شامل <sys/types.h>
#شامل <signal.h>
#شامل <sys/vfs.h>
#شامل <inttypes.h>
#شامل <time.h>

#شامل <sched.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <libgen.h>

ایستا صحیح
initialize_cpuset_subsystem_rec (نویسه *path, size_t path_len, نویسه *cpus, نویسه *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free نویسه *allocated_cpus = پوچ;
  cleanup_free نویسه *allocated_mems = پوچ;
  cleanup_close صحیح dirfd = -1;
  cleanup_close صحیح mems_fd = -1;
  cleanup_close صحیح cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  صحیح b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (dirfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  اگر (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      اگر (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      اگر (UNLIKELY (cpus_fd < 0))
        بازگشت crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (اقرأ (cpus_fd, cpus, 256));
      اگر (UNLIKELY (b_len < 0))
        بازگشت crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      اگر (cpus[0] == '\n')
        cpus[0] = '\0';
      اگر (cpus[0] != '\0')
        has_cpus = true;
    }

  اگر (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      اگر (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      اگر (UNLIKELY (mems_fd < 0))
        بازگشت crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (اقرأ (mems_fd, mems, 256));
      اگر (UNLIKELY (b_len < 0))
        بازگشت crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      اگر (mems[0] == '\n')
        mems[0] = '\0';
      اگر (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  اگر (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      صحیح ret;

      برای (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      اگر (parent_path_len == 1)
        بازگشت 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      اگر (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  اگر (resources && resources->cpu)
    {
      اگر (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      اگر (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  اگر (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (اكتب (cpus_fd, cpus, strlen (cpus)));
      اگر (UNLIKELY (b_len < 0))
        بازگشت crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  اگر (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (اكتب (mems_fd, mems, strlen (mems)));
      اگر (UNLIKELY (b_len < 0))
        بازگشت crun_make_error (err, errno, "write `cpuset.mems`");
    }

  بازگشت 0;
}

صحیح
initialize_cpuset_subsystem (ثابت نویسه *path, libcrun_error_t *err)
{
  cleanup_free نویسه *tmp_path = xstrdup (path);
  نویسه cpus_buf[257];
  نویسه mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  بازگشت initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, پوچ, err);
}

صحیح
initialize_cpuset_subsystem_resources (ثابت نویسه *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free نویسه *tmp_path = xstrdup (path);
  نویسه cpus_buf[257];
  نویسه mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  بازگشت initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

ایستا صحیح
initialize_memory_subsystem (ثابت نویسه *path, libcrun_error_t *err)
{
  ثابت نویسه *ثابت files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", پوچ };
  cleanup_close صحیح dirfd = -1;
  صحیح i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (dirfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", path);

  برای (i = 0; files[i]; i++)
    {
      صحیح ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      اگر (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  بازگشت 0;
}

صحیح
enter_cgroup_v1_subsystem (pid_t pid, ثابت نویسه *subsystem, ثابت نویسه *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free نویسه *cgroup_path = پوچ;
  صحیح ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ریشه, subsystem, path ? path : "", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (crun_error_get_errno (err) != EROFS)
            بازگشت crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          بازگشت 0;
        }

      اگر (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
      اگر (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }
  وگرنه
    {
      ret = crun_path_exists (cgroup_path, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      اگر (ret == 0)
        بازگشت 0;
    }

  بازگشت move_process_to_cgroup (pid, subsystem, path, err);
}

ایستا صحیح
get_file_owner (ثابت نویسه *path, uid_t *uid, gid_t *gid)
{
  ساختار stat st;
  صحیح ret;

#ifdef HAVE_STATX
  ساختار statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS || errno == EINVAL)
        برو fallback;

      بازگشت ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  بازگشت ret;

fallback:
#endif
  ret = stat (path, &st);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  بازگشت ret;
}

ایستا صحیح
copy_owner (ثابت نویسه *from, ثابت نویسه *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  صحیح ret;

  ret = get_file_owner (from, &uid, &gid);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  اگر (uid == 0 && gid == 0)
    بازگشت 0;

  بازگشت chown_cgroups (to, uid, gid, err);
}

ایستا صحیح
read_unified_cgroup_pid (pid_t pid, نویسه **path, libcrun_error_t *err)
{
  صحیح ret;
  نویسه cgroup_path[32];
  نویسه *from, *to;
  cleanup_free نویسه *content = پوچ;
  صحیح len;

  len = snprintf (cgroup_path, اندازه (cgroup_path), "/proc/%d/cgroup", pid);
  اگر (UNLIKELY (len >= (صحیح) اندازه (cgroup_path)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  from = strstr (content, "0::");
  اگر (UNLIKELY (from == پوچ))
    بازگشت crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  اگر (UNLIKELY (to == پوچ))
    بازگشت crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  بازگشت 0;
}

ایستا صحیح
enter_cgroup_v1 (pid_t pid, ثابت نویسه *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free نویسه *content = پوچ;
  bool entered_any = false;
  size_t content_size;
  نویسه *controller;
  نویسه *saveptr;
  bool has_data;
  صحیح rootless;
  صحیح ret;

  rootless = is_rootless (err);
  اگر (UNLIKELY (rootless < 0))
    بازگشت rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          بازگشت 0;
        }
      بازگشت ret;
    }

  برای (has_data = read_proc_cgroup (content, &saveptr, پوچ, &controller, پوچ);
       has_data;
       has_data = read_proc_cgroup (پوچ, &saveptr, پوچ, &controller, پوچ))
    {
      نویسه subsystem_path[64];
      نویسه *subsystem;
      size_t len;

      اگر (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      اگر (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      اگر (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, اندازه (subsystem_path), CGROUP_ریشه "/%s", subsystem);
      اگر (UNLIKELY (len >= (صحیح) اندازه (subsystem_path)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      اگر (ret == 0)
        ادامه;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      اگر (UNLIKELY (ret < 0))
        {
          صحیح errcode = crun_error_get_errno (err);
          اگر (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              ادامه;
            }
          بازگشت ret;
        }
    }

  اگر (entered_any)
    بازگشت 0;

  بازگشت crun_make_error (err, 0, "could not join cgroup");
}

ایستا صحیح
enter_cgroup_v2 (pid_t pid, pid_t init_pid, ثابت نویسه *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free نویسه *cgroup_path_procs = پوچ;
  cleanup_free نویسه *cgroup_path = پوچ;
  نویسه pid_str[16];
  صحیح repeat;
  صحیح len;
  صحیح ret;

  len = snprintf (pid_str, اندازه (pid_str), "%d", pid);
  اگر (UNLIKELY (len >= (صحیح) اندازه (pid_str)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_ریشه, path, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  اگر (LIKELY (ret >= 0))
    بازگشت ret;

  اگر (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      اگر (LIKELY (ret >= 0))
        بازگشت ret;
    }

  اگر (create_if_missing || crun_error_get_errno (err) != EBUSY)
    بازگشت ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  برای (repeat = 0;; repeat++)
    {
      cleanup_free نویسه *cgroup_crun_exec_path = پوچ;
      cleanup_free نویسه *cgroup_sub_path_procs = پوچ;

      /* There is an init pid, try to join its cgroup.  */
      اگر (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          اگر (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              آزاد (cgroup_crun_exec_path);
              cgroup_crun_exec_path = پوچ;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      اگر (cgroup_crun_exec_path == پوچ)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_ریشه, cgroup_crun_exec_path, "cgroup.procs", پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      اگر (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          اگر (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              ادامه;
            }
          بازگشت ret;
        }
      بازگشت copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  بازگشت ret;
}

صحیح
enter_cgroup (صحیح cgroup_mode, pid_t pid, pid_t init_pid, ثابت نویسه *path,
              bool create_if_missing, libcrun_error_t *err)
{
  صحیح ret;
  اگر (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت 0;
}
