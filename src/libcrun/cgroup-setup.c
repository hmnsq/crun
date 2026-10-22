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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-internal.h"
#ಅನ್ತರ್ಭಾವ "cgroup-systemd.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"
#ಅನ್ತರ್ಭಾವ "ebpf.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
initialize_cpuset_subsystem_rec (ವರ್ಣ *path, size_t path_len, ವರ್ಣ *cpus, ವರ್ಣ *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *allocated_cpus = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *allocated_mems = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ mems_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  ಪೂರ್ಣಾಙ್ಕ b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  ಯದಿ (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      ಯದಿ (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      ಯದಿ (UNLIKELY (cpus_fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (ಪಠ (cpus_fd, cpus, 256));
      ಯದಿ (UNLIKELY (b_len < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      ಯದಿ (cpus[0] == '\n')
        cpus[0] = '\0';
      ಯದಿ (cpus[0] != '\0')
        has_cpus = true;
    }

  ಯದಿ (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      ಯದಿ (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      ಯದಿ (UNLIKELY (mems_fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (ಪಠ (mems_fd, mems, 256));
      ಯದಿ (UNLIKELY (b_len < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      ಯದಿ (mems[0] == '\n')
        mems[0] = '\0';
      ಯದಿ (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  ಯದಿ (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      ಪೂರ್ಣಾಙ್ಕ ret;

      ಪ್ರತಿ (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      ಯದಿ (parent_path_len == 1)
        ಪ್ರತ್ಯಾಗಮ 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      ಯದಿ (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  ಯದಿ (resources && resources->cpu)
    {
      ಯದಿ (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      ಯದಿ (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  ಯದಿ (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (ಲಿಖ (cpus_fd, cpus, strlen (cpus)));
      ಯದಿ (UNLIKELY (b_len < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  ಯದಿ (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (ಲಿಖ (mems_fd, mems, strlen (mems)));
      ಯದಿ (UNLIKELY (b_len < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write `cpuset.mems`");
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
initialize_cpuset_subsystem (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *tmp_path = xstrdup (path);
  ವರ್ಣ cpus_buf[257];
  ವರ್ಣ mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ಪ್ರತ್ಯಾಗಮ initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, ರಿಕ್ತ, err);
}

ಪೂರ್ಣಾಙ್ಕ
initialize_cpuset_subsystem_resources (ಧ್ರುವ ವರ್ಣ *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *tmp_path = xstrdup (path);
  ವರ್ಣ cpus_buf[257];
  ವರ್ಣ mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ಪ್ರತ್ಯಾಗಮ initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
initialize_memory_subsystem (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", ರಿಕ್ತ };
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd = -1;
  ಪೂರ್ಣಾಙ್ಕ i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", path);

  ಪ್ರತಿ (i = 0; files[i]; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
enter_cgroup_v1_subsystem (pid_t pid, ಧ್ರುವ ವರ್ಣ *subsystem, ಧ್ರುವ ವರ್ಣ *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, subsystem, path ? path : "", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (crun_error_get_errno (err) != EROFS)
            ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          ಪ್ರತ್ಯಾಗಮ 0;
        }

      ಯದಿ (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಯದಿ (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }
  ಅನ್ಯಥಾ
    {
      ret = crun_path_exists (cgroup_path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      ಯದಿ (ret == 0)
        ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಪ್ರತ್ಯಾಗಮ move_process_to_cgroup (pid, subsystem, path, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_file_owner (ಧ್ರುವ ವರ್ಣ *path, uid_t *uid, gid_t *gid)
{
  ಸಂರಚನಾ stat st;
  ಪೂರ್ಣಾಙ್ಕ ret;

#ifdef HAVE_STATX
  ಸಂರಚನಾ statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno == ENOSYS || errno == EINVAL)
        ಗಚ್ಛ fallback;

      ಪ್ರತ್ಯಾಗಮ ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  ಪ್ರತ್ಯಾಗಮ ret;

fallback:
#endif
  ret = stat (path, &st);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
copy_owner (ಧ್ರುವ ವರ್ಣ *from, ಧ್ರುವ ವರ್ಣ *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = get_file_owner (from, &uid, &gid);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  ಯದಿ (uid == 0 && gid == 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ chown_cgroups (to, uid, gid, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_unified_cgroup_pid (pid_t pid, ವರ್ಣ **path, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ವರ್ಣ cgroup_path[32];
  ವರ್ಣ *from, *to;
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ len;

  len = snprintf (cgroup_path, ಪರಿಮಾಣ (cgroup_path), "/proc/%d/cgroup", pid);
  ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (cgroup_path)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  from = strstr (content, "0::");
  ಯದಿ (UNLIKELY (from == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  ಯದಿ (UNLIKELY (to == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
enter_cgroup_v1 (pid_t pid, ಧ್ರುವ ವರ್ಣ *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  bool entered_any = false;
  size_t content_size;
  ವರ್ಣ *controller;
  ವರ್ಣ *saveptr;
  bool has_data;
  ಪೂರ್ಣಾಙ್ಕ rootless;
  ಪೂರ್ಣಾಙ್ಕ ret;

  rootless = is_rootless (err);
  ಯದಿ (UNLIKELY (rootless < 0))
    ಪ್ರತ್ಯಾಗಮ rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಪ್ರತ್ಯಾಗಮ 0;
        }
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತಿ (has_data = read_proc_cgroup (content, &saveptr, ರಿಕ್ತ, &controller, ರಿಕ್ತ);
       has_data;
       has_data = read_proc_cgroup (ರಿಕ್ತ, &saveptr, ರಿಕ್ತ, &controller, ರಿಕ್ತ))
    {
      ವರ್ಣ subsystem_path[64];
      ವರ್ಣ *subsystem;
      size_t len;

      ಯದಿ (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      ಯದಿ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ಯದಿ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, ಪರಿಮಾಣ (subsystem_path), CGROUP_ಬೇರು "/%s", subsystem);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (subsystem_path)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      ಯದಿ (ret == 0)
        ಅನುವರ್ತ;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಙ್ಕ errcode = crun_error_get_errno (err);
          ಯದಿ (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              ಅನುವರ್ತ;
            }
          ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ಯದಿ (entered_any)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not join cgroup");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
enter_cgroup_v2 (pid_t pid, pid_t init_pid, ಧ್ರುವ ವರ್ಣ *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cgroup_path_procs = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
  ವರ್ಣ pid_str[16];
  ಪೂರ್ಣಾಙ್ಕ repeat;
  ಪೂರ್ಣಾಙ್ಕ len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  len = snprintf (pid_str, ಪರಿಮಾಣ (pid_str), "%d", pid);
  ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (pid_str)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, path, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  ಯದಿ (LIKELY (ret >= 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      ಯದಿ (LIKELY (ret >= 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (create_if_missing || crun_error_get_errno (err) != EBUSY)
    ಪ್ರತ್ಯಾಗಮ ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  ಪ್ರತಿ (repeat = 0;; repeat++)
    {
      cleanup_free ವರ್ಣ *cgroup_crun_exec_path = ರಿಕ್ತ;
      cleanup_free ವರ್ಣ *cgroup_sub_path_procs = ರಿಕ್ತ;

      /* There is an init pid, try to join its cgroup.  */
      ಯದಿ (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          ಯದಿ (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              ಮುಕ್ತ (cgroup_crun_exec_path);
              cgroup_crun_exec_path = ರಿಕ್ತ;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      ಯದಿ (cgroup_crun_exec_path == ರಿಕ್ತ)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_ಬೇರು, cgroup_crun_exec_path, "cgroup.procs", ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          ಯದಿ (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              ಅನುವರ್ತ;
            }
          ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಪ್ರತ್ಯಾಗಮ copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
enter_cgroup (ಪೂರ್ಣಾಙ್ಕ cgroup_mode, pid_t pid, pid_t init_pid, ಧ್ರುವ ವರ್ಣ *path,
              bool create_if_missing, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}
