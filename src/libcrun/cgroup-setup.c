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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "cgroup.h"
#समावेश "cgroup-internal.h"
#समावेश "cgroup-systemd.h"
#समावेश "cgroup-utils.h"
#समावेश "ebpf.h"
#समावेश "utils.h"
#समावेश "status.h"
#समावेश <string.h>
#समावेश <sys/types.h>
#समावेश <signal.h>
#समावेश <sys/vfs.h>
#समावेश <inttypes.h>
#समावेश <time.h>

#समावेश <sched.h>
#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#समावेश <fcntl.h>
#समावेश <libgen.h>

अटल पूर्णांक
initialize_cpuset_subsystem_rec (अक्षर *path, माप_प्रकार path_len, अक्षर *cpus, अक्षर *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free अक्षर *allocated_cpus = शून्य;
  cleanup_free अक्षर *allocated_mems = शून्य;
  cleanup_close पूर्णांक dirfd = -1;
  cleanup_close पूर्णांक mems_fd = -1;
  cleanup_close पूर्णांक cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  पूर्णांक b_len;

  dirfd = खोलो (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (dirfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  अगर (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      अगर (UNLIKELY (cpus_fd < 0 && त्रुटि_सं == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      अगर (UNLIKELY (cpus_fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s/%s`", path, "cpuset.cpus");

      b_len = अस्थायी_FAILURE_RETRY (पढ़ो (cpus_fd, cpus, 256));
      अगर (UNLIKELY (b_len < 0))
        वापस crun_make_error (err, त्रुटि_सं, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      अगर (cpus[0] == '\n')
        cpus[0] = '\0';
      अगर (cpus[0] != '\0')
        has_cpus = true;
    }

  अगर (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      अगर (UNLIKELY (mems_fd < 0 && त्रुटि_सं == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      अगर (UNLIKELY (mems_fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s/%s`", path, "cpuset.mems");

      b_len = अस्थायी_FAILURE_RETRY (पढ़ो (mems_fd, mems, 256));
      अगर (UNLIKELY (b_len < 0))
        वापस crun_make_error (err, त्रुटि_सं, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      अगर (mems[0] == '\n')
        mems[0] = '\0';
      अगर (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  अगर (cpus[0] == '\0' || mems[0] == '\0')
    {
      माप_प्रकार parent_path_len;
      पूर्णांक ret;

      क्रम (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      अगर (parent_path_len == 1)
        वापस 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      अगर (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  अगर (resources && resources->cpu)
    {
      अगर (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      अगर (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  अगर (cpus_fd >= 0)
    {
      b_len = अस्थायी_FAILURE_RETRY (ग_लिखो (cpus_fd, cpus, म_माप (cpus)));
      अगर (UNLIKELY (b_len < 0))
        वापस crun_make_error (err, त्रुटि_सं, "write `cpuset.cpus`");
    }

  अगर (mems_fd >= 0)
    {
      b_len = अस्थायी_FAILURE_RETRY (ग_लिखो (mems_fd, mems, म_माप (mems)));
      अगर (UNLIKELY (b_len < 0))
        वापस crun_make_error (err, त्रुटि_सं, "write `cpuset.mems`");
    }

  वापस 0;
}

पूर्णांक
initialize_cpuset_subsystem (स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *tmp_path = xstrdup (path);
  अक्षर cpus_buf[257];
  अक्षर mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  वापस initialize_cpuset_subsystem_rec (tmp_path, म_माप (tmp_path), cpus_buf, mems_buf, शून्य, err);
}

पूर्णांक
initialize_cpuset_subsystem_resources (स्थिर अक्षर *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free अक्षर *tmp_path = xstrdup (path);
  अक्षर cpus_buf[257];
  अक्षर mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  वापस initialize_cpuset_subsystem_rec (tmp_path, म_माप (tmp_path), cpus_buf, mems_buf, resources, err);
}

अटल पूर्णांक
initialize_memory_subsystem (स्थिर अक्षर *path, libcrun_error_t *err)
{
  स्थिर अक्षर *स्थिर files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", शून्य };
  cleanup_close पूर्णांक dirfd = -1;
  पूर्णांक i;

  dirfd = खोलो (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (dirfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", path);

  क्रम (i = 0; files[i]; i++)
    {
      पूर्णांक ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      अगर (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  वापस 0;
}

पूर्णांक
enter_cgroup_v1_subsystem (pid_t pid, स्थिर अक्षर *subsystem, स्थिर अक्षर *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = शून्य;
  पूर्णांक ret;

  ret = append_paths (&cgroup_path, err, CGROUP_मूल, subsystem, path ? path : "", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      अगर (UNLIKELY (ret < 0))
        {
          अगर (crun_error_get_errno (err) != EROFS)
            वापस crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          वापस 0;
        }

      अगर (म_भेद (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
      अगर (म_भेद (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }
  अन्यथा
    {
      ret = crun_path_exists (cgroup_path, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      अगर (ret == 0)
        वापस 0;
    }

  वापस move_process_to_cgroup (pid, subsystem, path, err);
}

अटल पूर्णांक
get_file_owner (स्थिर अक्षर *path, uid_t *uid, gid_t *gid)
{
  काष्ठा stat st;
  पूर्णांक ret;

#अगर_घोषित HAVE_STATX
  काष्ठा statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        जाओ fallback;

      वापस ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  वापस ret;

fallback:
#पूर्ण_अगर
  ret = stat (path, &st);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  वापस ret;
}

अटल पूर्णांक
copy_owner (स्थिर अक्षर *from, स्थिर अक्षर *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  पूर्णांक ret;

  ret = get_file_owner (from, &uid, &gid);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "cannot get file owner for `%s`", from);

  अगर (uid == 0 && gid == 0)
    वापस 0;

  वापस chown_cgroups (to, uid, gid, err);
}

अटल पूर्णांक
read_unified_cgroup_pid (pid_t pid, अक्षर **path, libcrun_error_t *err)
{
  पूर्णांक ret;
  अक्षर cgroup_path[32];
  अक्षर *from, *to;
  cleanup_free अक्षर *content = शून्य;
  पूर्णांक len;

  len = snprintf (cgroup_path, माप (cgroup_path), "/proc/%d/cgroup", pid);
  अगर (UNLIKELY (len >= (पूर्णांक) माप (cgroup_path)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  from = म_माला (content, "0::");
  अगर (UNLIKELY (from == शून्य))
    वापस crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = म_अक्षर (from, '\n');
  अगर (UNLIKELY (to == शून्य))
    वापस crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  वापस 0;
}

अटल पूर्णांक
enter_cgroup_v1 (pid_t pid, स्थिर अक्षर *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = शून्य;
  bool entered_any = false;
  माप_प्रकार content_आकार;
  अक्षर *controller;
  अक्षर *saveptr;
  bool has_तथ्य;
  पूर्णांक rootless;
  पूर्णांक ret;

  rootless = is_rootless (err);
  अगर (UNLIKELY (rootless < 0))
    वापस rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_आकार, err);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          वापस 0;
        }
      वापस ret;
    }

  क्रम (has_तथ्य = read_proc_cgroup (content, &saveptr, शून्य, &controller, शून्य);
       has_तथ्य;
       has_तथ्य = read_proc_cgroup (शून्य, &saveptr, शून्य, &controller, शून्य))
    {
      अक्षर subsystem_path[64];
      अक्षर *subsystem;
      माप_प्रकार len;

      अगर (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      अगर (म_भेद (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      अगर (म_भेद (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, माप (subsystem_path), CGROUP_मूल "/%s", subsystem);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (subsystem_path)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      अगर (ret == 0)
        जारी;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      अगर (UNLIKELY (ret < 0))
        {
          पूर्णांक errcode = crun_error_get_errno (err);
          अगर (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              जारी;
            }
          वापस ret;
        }
    }

  अगर (entered_any)
    वापस 0;

  वापस crun_make_error (err, 0, "could not join cgroup");
}

अटल पूर्णांक
enter_cgroup_v2 (pid_t pid, pid_t init_pid, स्थिर अक्षर *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path_procs = शून्य;
  cleanup_free अक्षर *cgroup_path = शून्य;
  अक्षर pid_str[16];
  पूर्णांक repeat;
  पूर्णांक len;
  पूर्णांक ret;

  len = snprintf (pid_str, माप (pid_str), "%d", pid);
  अगर (UNLIKELY (len >= (पूर्णांक) माप (pid_str)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_मूल, path, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, म_माप (pid_str), err);
  अगर (LIKELY (ret >= 0))
    वापस ret;

  अगर (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, म_माप (pid_str), err);
      अगर (LIKELY (ret >= 0))
        वापस ret;
    }

  अगर (create_if_missing || crun_error_get_errno (err) != EBUSY)
    वापस ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  क्रम (repeat = 0;; repeat++)
    {
      cleanup_free अक्षर *cgroup_crun_exec_path = शून्य;
      cleanup_free अक्षर *cgroup_sub_path_procs = शून्य;

      /* There is an init pid, try to join its cgroup.  */
      अगर (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          अगर (म_भेदन (path, cgroup_crun_exec_path, म_माप (path)))
            {
              मुक्त (cgroup_crun_exec_path);
              cgroup_crun_exec_path = शून्य;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      अगर (cgroup_crun_exec_path == शून्य)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_मूल, cgroup_crun_exec_path, "cgroup.procs", शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, म_माप (pid_str), err);
      अगर (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          अगर (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              जारी;
            }
          वापस ret;
        }
      वापस copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  वापस ret;
}

पूर्णांक
enter_cgroup (पूर्णांक cgroup_mode, pid_t pid, pid_t init_pid, स्थिर अक्षर *path,
              bool create_if_missing, libcrun_error_t *err)
{
  पूर्णांक ret;
  अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस 0;
}
