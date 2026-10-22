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

#परिभाषय _GNU_SOURCE

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
initialize_cpuset_subsystem_rec (वर्ण *मार्गः, size_t मार्गः_len, वर्ण *cpus, वर्ण *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free वर्ण *allocated_cpus = रिक्त;
  cleanup_free वर्ण *allocated_mems = रिक्त;
  cleanup_close पूर्णाङ्क dirfd = -1;
  cleanup_close पूर्णाङ्क mems_fd = -1;
  cleanup_close पूर्णाङ्क cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  पूर्णाङ्क b_len;

  dirfd = उद्घाटय (मार्गः, O_DIRECTORY | O_मार्गः | O_CLOEXEC);
  यदि (UNLIKELY (dirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", मार्गः);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  यदि (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      यदि (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      यदि (UNLIKELY (cpus_fd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s/%s`", मार्गः, "cpuset.cpus");

      b_len = अस्थायी_FAILURE_RETRY (पठ (cpus_fd, cpus, 256));
      यदि (UNLIKELY (b_len < 0))
        प्रत्यागम crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      यदि (cpus[0] == '\n')
        cpus[0] = '\0';
      यदि (cpus[0] != '\0')
        has_cpus = true;
    }

  यदि (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      यदि (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      यदि (UNLIKELY (mems_fd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s/%s`", मार्गः, "cpuset.mems");

      b_len = अस्थायी_FAILURE_RETRY (पठ (mems_fd, mems, 256));
      यदि (UNLIKELY (b_len < 0))
        प्रत्यागम crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      यदि (mems[0] == '\n')
        mems[0] = '\0';
      यदि (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  यदि (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_मार्गः_len;
      पूर्णाङ्क ret;

      प्रति (parent_मार्गः_len = मार्गः_len - 1; parent_मार्गः_len > 1 && मार्गः[parent_मार्गः_len] != '/'; parent_मार्गः_len--)
        ;
      यदि (parent_मार्गः_len == 1)
        प्रत्यागम 0;

      मार्गः[parent_मार्गः_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (मार्गः, parent_मार्गः_len, cpus, mems, resources, err);
      मार्गः[parent_मार्गः_len] = '/';
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
      b_len = अस्थायी_FAILURE_RETRY (लिख (cpus_fd, cpus, वाक्यदैर्घ्यम् (cpus)));
      यदि (UNLIKELY (b_len < 0))
        प्रत्यागम crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  यदि (mems_fd >= 0)
    {
      b_len = अस्थायी_FAILURE_RETRY (लिख (mems_fd, mems, वाक्यदैर्घ्यम् (mems)));
      यदि (UNLIKELY (b_len < 0))
        प्रत्यागम crun_make_error (err, errno, "write `cpuset.mems`");
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
initialize_cpuset_subsystem (ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  cleanup_free वर्ण *tmp_मार्गः = xstrdup (मार्गः);
  वर्ण cpus_buf[257];
  वर्ण mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  प्रत्यागम initialize_cpuset_subsystem_rec (tmp_मार्गः, वाक्यदैर्घ्यम् (tmp_मार्गः), cpus_buf, mems_buf, रिक्त, err);
}

पूर्णाङ्क
initialize_cpuset_subsystem_resources (ध्रुव वर्ण *मार्गः, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free वर्ण *tmp_मार्गः = xstrdup (मार्गः);
  वर्ण cpus_buf[257];
  वर्ण mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  प्रत्यागम initialize_cpuset_subsystem_rec (tmp_मार्गः, वाक्यदैर्घ्यम् (tmp_मार्गः), cpus_buf, mems_buf, resources, err);
}

स्थिर पूर्णाङ्क
initialize_स्मृतिः_subsystem (ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  ध्रुव वर्ण *ध्रुव files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", रिक्त };
  cleanup_close पूर्णाङ्क dirfd = -1;
  पूर्णाङ्क i;

  dirfd = उद्घाटय (मार्गः, O_DIRECTORY | O_मार्गः | O_CLOEXEC);
  यदि (UNLIKELY (dirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", मार्गः);

  प्रति (i = 0; files[i]; i++)
    {
      पूर्णाङ्क ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      यदि (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
enter_cgroup_v1_subsystem (pid_t pid, ध्रुव वर्ण *subsystem, ध्रुव वर्ण *मार्गः, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
  पूर्णाङ्क ret;

  ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, subsystem, मार्गः ? मार्गः : "", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_मार्गः, 0755, false, err);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (crun_error_get_errno (err) != EROFS)
            प्रत्यागम crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_मार्गः);

          crun_error_release (err);
          प्रत्यागम 0;
        }

      यदि (वाक्यं_तुलय (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_मार्गः, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      यदि (वाक्यं_तुलय (subsystem, "memory") == 0)
        {
          ret = initialize_स्मृतिः_subsystem (cgroup_मार्गः, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }
  अन्यथा
    {
      ret = crun_मार्गः_exists (cgroup_मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      यदि (ret == 0)
        प्रत्यागम 0;
    }

  प्रत्यागम move_process_गन्तव्यम्_cgroup (pid, subsystem, मार्गः, err);
}

स्थिर पूर्णाङ्क
get_file_owner (ध्रुव वर्ण *मार्गः, uid_t *uid, gid_t *gid)
{
  संरचना stat st;
  पूर्णाङ्क ret;

#यदि_परिभाषितम् HAVE_STATX
  संरचना statx stx;

  ret = statx (AT_FDCWD, मार्गः, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (errno == ENOSYS || errno == EINVAL)
        गच्छ fallback;

      प्रत्यागम ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  प्रत्यागम ret;

fallback:
#यदि_अन्तः
  ret = stat (मार्गः, &st);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
copy_owner (ध्रुव वर्ण *from, ध्रुव वर्ण *गन्तव्यम्, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  पूर्णाङ्क ret;

  ret = get_file_owner (from, &uid, &gid);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  यदि (uid == 0 && gid == 0)
    प्रत्यागम 0;

  प्रत्यागम chown_cgroups (गन्तव्यम्, uid, gid, err);
}

स्थिर पूर्णाङ्क
read_unified_cgroup_pid (pid_t pid, वर्ण **मार्गः, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  वर्ण cgroup_मार्गः[32];
  वर्ण *from, *गन्तव्यम्;
  cleanup_free वर्ण *content = रिक्त;
  पूर्णाङ्क len;

  len = snprintf (cgroup_मार्गः, परिमाण (cgroup_मार्गः), "/proc/%d/cgroup", pid);
  यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (cgroup_मार्गः)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_मार्गः[len] = '\0';

  ret = read_all_file (cgroup_मार्गः, &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  from = वाक्ये_अन्विष (content, "0::");
  यदि (UNLIKELY (from == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  गन्तव्यम् = strchr (from, '\n');
  यदि (UNLIKELY (गन्तव्यम् == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "cannot parse `%s`", cgroup_मार्गः);
  *गन्तव्यम् = '\0';

  *मार्गः = xstrdup (from);
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
enter_cgroup_v1 (pid_t pid, ध्रुव वर्ण *मार्गः, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  bool entered_any = false;
  size_t content_आकार;
  वर्ण *controller;
  वर्ण *saveptr;
  bool has_दत्त;
  पूर्णाङ्क rootless;
  पूर्णाङ्क ret;

  rootless = is_rootless (err);
  यदि (UNLIKELY (rootless < 0))
    प्रत्यागम rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_आकार, err);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          प्रत्यागम 0;
        }
      प्रत्यागम ret;
    }

  प्रति (has_दत्त = read_proc_cgroup (content, &saveptr, रिक्त, &controller, रिक्त);
       has_दत्त;
       has_दत्त = read_proc_cgroup (रिक्त, &saveptr, रिक्त, &controller, रिक्त))
    {
      वर्ण subsystem_मार्गः[64];
      वर्ण *subsystem;
      size_t len;

      यदि (has_उपसर्गः (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      यदि (वाक्यं_तुलय (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      यदि (वाक्यं_तुलय (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_मार्गः, परिमाण (subsystem_मार्गः), CGROUP_मूलम् "/%s", subsystem);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (subsystem_मार्गः)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_मार्गः_exists (subsystem_मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      यदि (ret == 0)
        अनुवर्त;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, मार्गः, create_if_missing, err);
      यदि (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क errcode = crun_error_get_errno (err);
          यदि (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              अनुवर्त;
            }
          प्रत्यागम ret;
        }
    }

  यदि (entered_any)
    प्रत्यागम 0;

  प्रत्यागम crun_make_error (err, 0, "could not join cgroup");
}

स्थिर पूर्णाङ्क
enter_cgroup_v2 (pid_t pid, pid_t init_pid, ध्रुव वर्ण *मार्गः, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_मार्गः_procs = रिक्त;
  cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
  वर्ण pid_str[16];
  पूर्णाङ्क repeat;
  पूर्णाङ्क len;
  पूर्णाङ्क ret;

  len = snprintf (pid_str, परिमाण (pid_str), "%d", pid);
  यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (pid_str)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, मार्गः, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_मार्गः, 0755, false, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = append_paths (&cgroup_मार्गः_procs, err, cgroup_मार्गः, "cgroup.procs", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_मार्गः_procs, pid_str, वाक्यदैर्घ्यम् (pid_str), err);
  यदि (LIKELY (ret >= 0))
    प्रत्यागम ret;

  यदि (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_मार्गः_procs, pid_str, वाक्यदैर्घ्यम् (pid_str), err);
      यदि (LIKELY (ret >= 0))
        प्रत्यागम ret;
    }

  यदि (create_if_missing || crun_error_get_errno (err) != EBUSY)
    प्रत्यागम ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  प्रति (repeat = 0;; repeat++)
    {
      cleanup_free वर्ण *cgroup_crun_exec_मार्गः = रिक्त;
      cleanup_free वर्ण *cgroup_sub_मार्गः_procs = रिक्त;

      /* There is an init pid, try to join its cgroup.  */
      यदि (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_मार्गः, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          यदि (वाक्ये_तुलय (मार्गः, cgroup_crun_exec_मार्गः, वाक्यदैर्घ्यम् (मार्गः)))
            {
              मुक्त (cgroup_crun_exec_मार्गः);
              cgroup_crun_exec_मार्गः = रिक्त;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      यदि (cgroup_crun_exec_मार्गः == रिक्त)
        xasprintf (&cgroup_crun_exec_मार्गः, "%s/crun-exec", मार्गः);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = append_paths (&cgroup_sub_मार्गः_procs, err, CGROUP_मूलम्, cgroup_crun_exec_मार्गः, "cgroup.procs", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_मार्गः_procs, pid_str, वाक्यदैर्घ्यम् (pid_str), err);
      यदि (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          यदि (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              अनुवर्त;
            }
          प्रत्यागम ret;
        }
      प्रत्यागम copy_owner (cgroup_मार्गः_procs, cgroup_crun_exec_मार्गः, err);
    }
  प्रत्यागम ret;
}

पूर्णाङ्क
enter_cgroup (पूर्णाङ्क cgroup_बहुलकम्, pid_t pid, pid_t init_pid, ध्रुव वर्ण *मार्गः,
              bool create_if_missing, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, मार्गः, create_if_missing, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अन्यथा
    {
      ret = enter_cgroup_v1 (pid, मार्गः, create_if_missing, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}
