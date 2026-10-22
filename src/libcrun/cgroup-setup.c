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

#अन्तर्भाव <config.h>
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव "cgroup-internal.h"
#अन्तर्भाव "cgroup-systemd.h"
#अन्तर्भाव "cgroup-utils.h"
#अन्तर्भाव "ebpf.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "status.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव <sched.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <libgen.h>

स्थिर पूर्णाङ्क
initialize_cpuset_subsystem_rec (वर्ण *path, माप_प्रकार path_len, वर्ण *cpus, वर्ण *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free वर्ण *allocated_cpus = रिक्त;
  cleanup_free वर्ण *allocated_mems = रिक्त;
  cleanup_close पूर्णाङ्क dirfd = -1;
  cleanup_close पूर्णाङ्क mems_fd = -1;
  cleanup_close पूर्णाङ्क cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  पूर्णाङ्क b_len;

  dirfd = खोलो (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (dirfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  यदि (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      यदि (UNLIKELY (cpus_fd < 0 && त्रुटि_सं == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      यदि (UNLIKELY (cpus_fd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (पठ (cpus_fd, cpus, 256));
      यदि (UNLIKELY (b_len < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      यदि (cpus[0] == '\n')
        cpus[0] = '\0';
      यदि (cpus[0] != '\0')
        has_cpus = true;
    }

  यदि (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      यदि (UNLIKELY (mems_fd < 0 && त्रुटि_सं == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      यदि (UNLIKELY (mems_fd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (पठ (mems_fd, mems, 256));
      यदि (UNLIKELY (b_len < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      यदि (mems[0] == '\n')
        mems[0] = '\0';
      यदि (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  यदि (cpus[0] == '\0' || mems[0] == '\0')
    {
      माप_प्रकार parent_path_len;
      पूर्णाङ्क ret;

      लागि (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      यदि (parent_path_len == 1)
        फर्काऊ 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      यदि (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  यदि (resources && resources->cpu)
    {
      यदि (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      यदि (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  यदि (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (लिख (cpus_fd, cpus, म_माप (cpus)));
      यदि (UNLIKELY (b_len < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "write `cpuset.cpus`");
    }

  यदि (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (लिख (mems_fd, mems, म_माप (mems)));
      यदि (UNLIKELY (b_len < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "write `cpuset.mems`");
    }

  फर्काऊ 0;
}

पूर्णाङ्क
initialize_cpuset_subsystem (अचर वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *tmp_path = xstrdup (path);
  वर्ण cpus_buf[257];
  वर्ण mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  फर्काऊ initialize_cpuset_subsystem_rec (tmp_path, म_माप (tmp_path), cpus_buf, mems_buf, रिक्त, err);
}

पूर्णाङ्क
initialize_cpuset_subsystem_resources (अचर वर्ण *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free वर्ण *tmp_path = xstrdup (path);
  वर्ण cpus_buf[257];
  वर्ण mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  फर्काऊ initialize_cpuset_subsystem_rec (tmp_path, म_माप (tmp_path), cpus_buf, mems_buf, resources, err);
}

स्थिर पूर्णाङ्क
initialize_memory_subsystem (अचर वर्ण *path, libcrun_error_t *err)
{
  अचर वर्ण *अचर files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", रिक्त };
  cleanup_close पूर्णाङ्क dirfd = -1;
  पूर्णाङ्क i;

  dirfd = खोलो (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (dirfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", path);

  लागि (i = 0; files[i]; i++)
    {
      पूर्णाङ्क ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      यदि (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  फर्काऊ 0;
}

पूर्णाङ्क
enter_cgroup_v1_subsystem (pid_t pid, अचर वर्ण *subsystem, अचर वर्ण *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path = रिक्त;
  पूर्णाङ्क ret;

  ret = append_paths (&cgroup_path, err, CGROUP_जरा, subsystem, path ? path : "", रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (crun_error_get_errno (err) != EROFS)
            फर्काऊ crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          फर्काऊ 0;
        }

      यदि (म_भेद (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
      यदि (म_भेद (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
    }
  अन्यथा
    {
      ret = crun_path_exists (cgroup_path, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
      यदि (ret == 0)
        फर्काऊ 0;
    }

  फर्काऊ move_process_to_cgroup (pid, subsystem, path, err);
}

स्थिर पूर्णाङ्क
get_file_owner (अचर वर्ण *path, uid_t *uid, gid_t *gid)
{
  संरचना stat st;
  पूर्णाङ्क ret;

#ifdef HAVE_STATX
  संरचना statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        गच्छ fallback;

      फर्काऊ ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  फर्काऊ ret;

fallback:
#endif
  ret = stat (path, &st);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  फर्काऊ ret;
}

स्थिर पूर्णाङ्क
copy_owner (अचर वर्ण *from, अचर वर्ण *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  पूर्णाङ्क ret;

  ret = get_file_owner (from, &uid, &gid);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot get file owner for `%s`", from);

  यदि (uid == 0 && gid == 0)
    फर्काऊ 0;

  फर्काऊ chown_cgroups (to, uid, gid, err);
}

स्थिर पूर्णाङ्क
read_unified_cgroup_pid (pid_t pid, वर्ण **path, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  वर्ण cgroup_path[32];
  वर्ण *from, *to;
  cleanup_free वर्ण *content = रिक्त;
  पूर्णाङ्क len;

  len = snprintf (cgroup_path, परिमाण (cgroup_path), "/proc/%d/cgroup", pid);
  यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (cgroup_path)))
    फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  from = म_माला (content, "0::");
  यदि (UNLIKELY (from == रिक्त))
    फर्काऊ crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = म_अक्षर (from, '\n');
  यदि (UNLIKELY (to == रिक्त))
    फर्काऊ crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
enter_cgroup_v1 (pid_t pid, अचर वर्ण *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  bool entered_any = false;
  माप_प्रकार content_size;
  वर्ण *controller;
  वर्ण *saveptr;
  bool has_data;
  पूर्णाङ्क rootless;
  पूर्णाङ्क ret;

  rootless = is_rootless (err);
  यदि (UNLIKELY (rootless < 0))
    फर्काऊ rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          फर्काऊ 0;
        }
      फर्काऊ ret;
    }

  लागि (has_data = read_proc_cgroup (content, &saveptr, रिक्त, &controller, रिक्त);
       has_data;
       has_data = read_proc_cgroup (रिक्त, &saveptr, रिक्त, &controller, रिक्त))
    {
      वर्ण subsystem_path[64];
      वर्ण *subsystem;
      माप_प्रकार len;

      यदि (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      यदि (म_भेद (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      यदि (म_भेद (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, परिमाण (subsystem_path), CGROUP_जरा "/%s", subsystem);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (subsystem_path)))
        फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
      यदि (ret == 0)
        जारी;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      यदि (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क errcode = crun_error_get_errno (err);
          यदि (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              जारी;
            }
          फर्काऊ ret;
        }
    }

  यदि (entered_any)
    फर्काऊ 0;

  फर्काऊ crun_make_error (err, 0, "could not join cgroup");
}

स्थिर पूर्णाङ्क
enter_cgroup_v2 (pid_t pid, pid_t init_pid, अचर वर्ण *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path_procs = रिक्त;
  cleanup_free वर्ण *cgroup_path = रिक्त;
  वर्ण pid_str[16];
  पूर्णाङ्क repeat;
  पूर्णाङ्क len;
  पूर्णाङ्क ret;

  len = snprintf (pid_str, परिमाण (pid_str), "%d", pid);
  यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (pid_str)))
    फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_जरा, path, रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, म_माप (pid_str), err);
  यदि (LIKELY (ret >= 0))
    फर्काऊ ret;

  यदि (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, म_माप (pid_str), err);
      यदि (LIKELY (ret >= 0))
        फर्काऊ ret;
    }

  यदि (create_if_missing || crun_error_get_errno (err) != EBUSY)
    फर्काऊ ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  लागि (repeat = 0;; repeat++)
    {
      cleanup_free वर्ण *cgroup_crun_exec_path = रिक्त;
      cleanup_free वर्ण *cgroup_sub_path_procs = रिक्त;

      /* There is an init pid, try to join its cgroup.  */
      यदि (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          यदि (म_भेदन (path, cgroup_crun_exec_path, म_माप (path)))
            {
              मुक्त (cgroup_crun_exec_path);
              cgroup_crun_exec_path = रिक्त;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      यदि (cgroup_crun_exec_path == रिक्त)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_जरा, cgroup_crun_exec_path, "cgroup.procs", रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, म_माप (pid_str), err);
      यदि (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          यदि (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              जारी;
            }
          फर्काऊ ret;
        }
      फर्काऊ copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  फर्काऊ ret;
}

पूर्णाङ्क
enter_cgroup (पूर्णाङ्क cgroup_mode, pid_t pid, pid_t init_pid, अचर वर्ण *path,
              bool create_if_missing, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  यदि (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  अन्यथा
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  फर्काऊ 0;
}
