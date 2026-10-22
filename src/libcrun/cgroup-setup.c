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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "cgroup.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-internal.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-systemd.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-utils.h"
#ଅନ୍ତର୍ଭାଵ "ebpf.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "status.h"
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <sys/vfs.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ <sched.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <libgen.h>

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
initialize_cpuset_subsystem_rec (ଅକ୍ଷର *path, size_t path_len, ଅକ୍ଷର *cpus, ଅକ୍ଷର *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *allocated_cpus = ଖାଲି;
  cleanup_free ଅକ୍ଷର *allocated_mems = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା mems_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (dirfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  ଯଦି (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      ଯଦି (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      ଯଦି (UNLIKELY (cpus_fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (ପଠ (cpus_fd, cpus, 256));
      ଯଦି (UNLIKELY (b_len < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      ଯଦି (cpus[0] == '\n')
        cpus[0] = '\0';
      ଯଦି (cpus[0] != '\0')
        has_cpus = true;
    }

  ଯଦି (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      ଯଦି (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      ଯଦି (UNLIKELY (mems_fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (ପଠ (mems_fd, mems, 256));
      ଯଦି (UNLIKELY (b_len < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      ଯଦି (mems[0] == '\n')
        mems[0] = '\0';
      ଯଦି (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  ଯଦି (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ପାଇଁ (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      ଯଦି (parent_path_len == 1)
        ଫେରନ୍ତୁ 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      ଯଦି (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  ଯଦି (resources && resources->cpu)
    {
      ଯଦି (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      ଯଦି (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  ଯଦି (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (ଲିଖ (cpus_fd, cpus, strlen (cpus)));
      ଯଦି (UNLIKELY (b_len < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  ଯଦି (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (ଲିଖ (mems_fd, mems, strlen (mems)));
      ଯଦି (UNLIKELY (b_len < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "write `cpuset.mems`");
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
initialize_cpuset_subsystem (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *tmp_path = xstrdup (path);
  ଅକ୍ଷର cpus_buf[257];
  ଅକ୍ଷର mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ଫେରନ୍ତୁ initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, ଖାଲି, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
initialize_cpuset_subsystem_resources (ଅଚଳ ଅକ୍ଷର *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *tmp_path = xstrdup (path);
  ଅକ୍ଷର cpus_buf[257];
  ଅକ୍ଷର mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ଫେରନ୍ତୁ initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
initialize_memory_subsystem (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *ଅଚଳ files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", ଖାଲି };
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (dirfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", path);

  ପାଇଁ (i = 0; files[i]; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
enter_cgroup_v1_subsystem (pid_t pid, ଅଚଳ ଅକ୍ଷର *subsystem, ଅଚଳ ଅକ୍ଷର *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, subsystem, path ? path : "", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (crun_error_get_errno (err) != EROFS)
            ଫେରନ୍ତୁ crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          ଫେରନ୍ତୁ 0;
        }

      ଯଦି (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
      ଯଦି (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }
  ଅନ୍ୟଥା
    {
      ret = crun_path_exists (cgroup_path, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      ଯଦି (ret == 0)
        ଫେରନ୍ତୁ 0;
    }

  ଫେରନ୍ତୁ move_process_to_cgroup (pid, subsystem, path, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_file_owner (ଅଚଳ ଅକ୍ଷର *path, uid_t *uid, gid_t *gid)
{
  ସଂରଚନା stat st;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

#ifdef HAVE_STATX
  ସଂରଚନା statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (errno == ENOSYS || errno == EINVAL)
        ଯାଅ fallback;

      ଫେରନ୍ତୁ ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  ଫେରନ୍ତୁ ret;

fallback:
#endif
  ret = stat (path, &st);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
copy_owner (ଅଚଳ ଅକ୍ଷର *from, ଅଚଳ ଅକ୍ଷର *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = get_file_owner (from, &uid, &gid);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  ଯଦି (uid == 0 && gid == 0)
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ chown_cgroups (to, uid, gid, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_unified_cgroup_pid (pid_t pid, ଅକ୍ଷର **path, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅକ୍ଷର cgroup_path[32];
  ଅକ୍ଷର *from, *to;
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

  len = snprintf (cgroup_path, ଆକାର (cgroup_path), "/proc/%d/cgroup", pid);
  ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (cgroup_path)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  from = strstr (content, "0::");
  ଯଦି (UNLIKELY (from == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  ଯଦି (UNLIKELY (to == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
enter_cgroup_v1 (pid_t pid, ଅଚଳ ଅକ୍ଷର *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  bool entered_any = false;
  size_t content_size;
  ଅକ୍ଷର *controller;
  ଅକ୍ଷର *saveptr;
  bool has_data;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootless;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  rootless = is_rootless (err);
  ଯଦି (UNLIKELY (rootless < 0))
    ଫେରନ୍ତୁ rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ଫେରନ୍ତୁ 0;
        }
      ଫେରନ୍ତୁ ret;
    }

  ପାଇଁ (has_data = read_proc_cgroup (content, &saveptr, ଖାଲି, &controller, ଖାଲି);
       has_data;
       has_data = read_proc_cgroup (ଖାଲି, &saveptr, ଖାଲି, &controller, ଖାଲି))
    {
      ଅକ୍ଷର subsystem_path[64];
      ଅକ୍ଷର *subsystem;
      size_t len;

      ଯଦି (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      ଯଦି (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ଯଦି (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, ଆକାର (subsystem_path), CGROUP_ମୂଳ "/%s", subsystem);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (subsystem_path)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      ଯଦି (ret == 0)
        ଜାରି;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା errcode = crun_error_get_errno (err);
          ଯଦି (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              ଜାରି;
            }
          ଫେରନ୍ତୁ ret;
        }
    }

  ଯଦି (entered_any)
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ crun_make_error (err, 0, "could not join cgroup");
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
enter_cgroup_v2 (pid_t pid, pid_t init_pid, ଅଚଳ ଅକ୍ଷର *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cgroup_path_procs = ଖାଲି;
  cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
  ଅକ୍ଷର pid_str[16];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା repeat;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  len = snprintf (pid_str, ଆକାର (pid_str), "%d", pid);
  ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (pid_str)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, path, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  ଯଦି (LIKELY (ret >= 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      ଯଦି (LIKELY (ret >= 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (create_if_missing || crun_error_get_errno (err) != EBUSY)
    ଫେରନ୍ତୁ ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  ପାଇଁ (repeat = 0;; repeat++)
    {
      cleanup_free ଅକ୍ଷର *cgroup_crun_exec_path = ଖାଲି;
      cleanup_free ଅକ୍ଷର *cgroup_sub_path_procs = ଖାଲି;

      /* There is an init pid, try to join its cgroup.  */
      ଯଦି (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          ଯଦି (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              ମୁକ୍ତ (cgroup_crun_exec_path);
              cgroup_crun_exec_path = ଖାଲି;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      ଯଦି (cgroup_crun_exec_path == ଖାଲି)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_ମୂଳ, cgroup_crun_exec_path, "cgroup.procs", ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          ଯଦି (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              ଜାରି;
            }
          ଫେରନ୍ତୁ ret;
        }
      ଫେରନ୍ତୁ copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
enter_cgroup (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode, pid_t pid, pid_t init_pid, ଅଚଳ ଅକ୍ଷର *path,
              bool create_if_missing, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ 0;
}
