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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "cgroup.h"
#অন্তর্ভাৱ "cgroup-internal.h"
#অন্তর্ভাৱ "cgroup-systemd.h"
#অন্তর্ভাৱ "cgroup-utils.h"
#অন্তর্ভাৱ "ebpf.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "status.h"
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <signal.h>
#অন্তর্ভাৱ <sys/vfs.h>
#অন্তর্ভাৱ <inttypes.h>
#অন্তর্ভাৱ <time.h>

#অন্তর্ভাৱ <sched.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <libgen.h>

স্থির পূর্ণাঙ্ক
initialize_cpuset_subsystem_rec (ৱর্ণ *path, size_t path_len, ৱর্ণ *cpus, ৱর্ণ *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *allocated_cpus = রিক্ত;
  cleanup_free ৱর্ণ *allocated_mems = রিক্ত;
  cleanup_close পূর্ণাঙ্ক dirfd = -1;
  cleanup_close পূর্ণাঙ্ক mems_fd = -1;
  cleanup_close পূর্ণাঙ্ক cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  পূর্ণাঙ্ক b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (dirfd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  যদি (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      যদি (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      যদি (UNLIKELY (cpus_fd < 0))
        ঘূৰাই crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (পঠ (cpus_fd, cpus, 256));
      যদি (UNLIKELY (b_len < 0))
        ঘূৰাই crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      যদি (cpus[0] == '\n')
        cpus[0] = '\0';
      যদি (cpus[0] != '\0')
        has_cpus = true;
    }

  যদি (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      যদি (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      যদি (UNLIKELY (mems_fd < 0))
        ঘূৰাই crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (পঠ (mems_fd, mems, 256));
      যদি (UNLIKELY (b_len < 0))
        ঘূৰাই crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      যদি (mems[0] == '\n')
        mems[0] = '\0';
      যদি (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  যদি (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      পূর্ণাঙ্ক ret;

      বাবে (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      যদি (parent_path_len == 1)
        ঘূৰাই 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      যদি (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  যদি (resources && resources->cpu)
    {
      যদি (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      যদি (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  যদি (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (লিখ (cpus_fd, cpus, strlen (cpus)));
      যদি (UNLIKELY (b_len < 0))
        ঘূৰাই crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  যদি (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (লিখ (mems_fd, mems, strlen (mems)));
      যদি (UNLIKELY (b_len < 0))
        ঘূৰাই crun_make_error (err, errno, "write `cpuset.mems`");
    }

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
initialize_cpuset_subsystem (ধ্রুৱ ৱর্ণ *path, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *tmp_path = xstrdup (path);
  ৱর্ণ cpus_buf[257];
  ৱর্ণ mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ঘূৰাই initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, রিক্ত, err);
}

পূর্ণাঙ্ক
initialize_cpuset_subsystem_resources (ধ্রুৱ ৱর্ণ *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *tmp_path = xstrdup (path);
  ৱর্ণ cpus_buf[257];
  ৱর্ণ mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ঘূৰাই initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

স্থির পূর্ণাঙ্ক
initialize_memory_subsystem (ধ্রুৱ ৱর্ণ *path, libcrun_error_t *err)
{
  ধ্রুৱ ৱর্ণ *ধ্রুৱ files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", রিক্ত };
  cleanup_close পূর্ণাঙ্ক dirfd = -1;
  পূর্ণাঙ্ক i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (dirfd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s`", path);

  বাবে (i = 0; files[i]; i++)
    {
      পূর্ণাঙ্ক ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      যদি (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
enter_cgroup_v1_subsystem (pid_t pid, ধ্রুৱ ৱর্ণ *subsystem, ধ্রুৱ ৱর্ণ *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *cgroup_path = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = append_paths (&cgroup_path, err, CGROUP_মূল, subsystem, path ? path : "", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (crun_error_get_errno (err) != EROFS)
            ঘূৰাই crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          ঘূৰাই 0;
        }

      যদি (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
      যদি (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
    }
  অন্যথা
    {
      ret = crun_path_exists (cgroup_path, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
      যদি (ret == 0)
        ঘূৰাই 0;
    }

  ঘূৰাই move_process_to_cgroup (pid, subsystem, path, err);
}

স্থির পূর্ণাঙ্ক
get_file_owner (ধ্রুৱ ৱর্ণ *path, uid_t *uid, gid_t *gid)
{
  সংরচনা stat st;
  পূর্ণাঙ্ক ret;

#ifdef HAVE_STATX
  সংরচনা statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOSYS || errno == EINVAL)
        গচ্ছ fallback;

      ঘূৰাই ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  ঘূৰাই ret;

fallback:
#endif
  ret = stat (path, &st);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক
copy_owner (ধ্রুৱ ৱর্ণ *from, ধ্রুৱ ৱর্ণ *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  পূর্ণাঙ্ক ret;

  ret = get_file_owner (from, &uid, &gid);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  যদি (uid == 0 && gid == 0)
    ঘূৰাই 0;

  ঘূৰাই chown_cgroups (to, uid, gid, err);
}

স্থির পূর্ণাঙ্ক
read_unified_cgroup_pid (pid_t pid, ৱর্ণ **path, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  ৱর্ণ cgroup_path[32];
  ৱর্ণ *from, *to;
  cleanup_free ৱর্ণ *content = রিক্ত;
  পূর্ণাঙ্ক len;

  len = snprintf (cgroup_path, পরিমাণ (cgroup_path), "/proc/%d/cgroup", pid);
  যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (cgroup_path)))
    ঘূৰাই crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  from = strstr (content, "0::");
  যদি (UNLIKELY (from == রিক্ত))
    ঘূৰাই crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  যদি (UNLIKELY (to == রিক্ত))
    ঘূৰাই crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
enter_cgroup_v1 (pid_t pid, ধ্রুৱ ৱর্ণ *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *content = রিক্ত;
  bool entered_any = false;
  size_t content_size;
  ৱর্ণ *controller;
  ৱর্ণ *saveptr;
  bool has_data;
  পূর্ণাঙ্ক rootless;
  পূর্ণাঙ্ক ret;

  rootless = is_rootless (err);
  যদি (UNLIKELY (rootless < 0))
    ঘূৰাই rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ঘূৰাই 0;
        }
      ঘূৰাই ret;
    }

  বাবে (has_data = read_proc_cgroup (content, &saveptr, রিক্ত, &controller, রিক্ত);
       has_data;
       has_data = read_proc_cgroup (রিক্ত, &saveptr, রিক্ত, &controller, রিক্ত))
    {
      ৱর্ণ subsystem_path[64];
      ৱর্ণ *subsystem;
      size_t len;

      যদি (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      যদি (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      যদি (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, পরিমাণ (subsystem_path), CGROUP_মূল "/%s", subsystem);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (subsystem_path)))
        ঘূৰাই crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
      যদি (ret == 0)
        অনুৱর্ত;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      যদি (UNLIKELY (ret < 0))
        {
          পূর্ণাঙ্ক errcode = crun_error_get_errno (err);
          যদি (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              অনুৱর্ত;
            }
          ঘূৰাই ret;
        }
    }

  যদি (entered_any)
    ঘূৰাই 0;

  ঘূৰাই crun_make_error (err, 0, "could not join cgroup");
}

স্থির পূর্ণাঙ্ক
enter_cgroup_v2 (pid_t pid, pid_t init_pid, ধ্রুৱ ৱর্ণ *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *cgroup_path_procs = রিক্ত;
  cleanup_free ৱর্ণ *cgroup_path = রিক্ত;
  ৱর্ণ pid_str[16];
  পূর্ণাঙ্ক repeat;
  পূর্ণাঙ্ক len;
  পূর্ণাঙ্ক ret;

  len = snprintf (pid_str, পরিমাণ (pid_str), "%d", pid);
  যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (pid_str)))
    ঘূৰাই crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_মূল, path, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  যদি (LIKELY (ret >= 0))
    ঘূৰাই ret;

  যদি (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      যদি (LIKELY (ret >= 0))
        ঘূৰাই ret;
    }

  যদি (create_if_missing || crun_error_get_errno (err) != EBUSY)
    ঘূৰাই ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  বাবে (repeat = 0;; repeat++)
    {
      cleanup_free ৱর্ণ *cgroup_crun_exec_path = রিক্ত;
      cleanup_free ৱর্ণ *cgroup_sub_path_procs = রিক্ত;

      /* There is an init pid, try to join its cgroup.  */
      যদি (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          যদি (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              মুক্ত (cgroup_crun_exec_path);
              cgroup_crun_exec_path = রিক্ত;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      যদি (cgroup_crun_exec_path == রিক্ত)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_মূল, cgroup_crun_exec_path, "cgroup.procs", রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      যদি (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          যদি (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              অনুৱর্ত;
            }
          ঘূৰাই ret;
        }
      ঘূৰাই copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  ঘূৰাই ret;
}

পূর্ণাঙ্ক
enter_cgroup (পূর্ণাঙ্ক cgroup_mode, pid_t pid, pid_t init_pid, ধ্রুৱ ৱর্ণ *path,
              bool create_if_missing, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  অন্যথা
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  ঘূৰাই 0;
}
