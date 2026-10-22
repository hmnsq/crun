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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "cgroup.h"
#અન્તર્ભાવ "cgroup-internal.h"
#અન્તર્ભાવ "cgroup-systemd.h"
#અન્તર્ભાવ "cgroup-utils.h"
#અન્તર્ભાવ "ebpf.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "status.h"
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <sys/vfs.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ <sched.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <libgen.h>

સ્થિર પૂર્ણાંક
initialize_cpuset_subsystem_rec (અક્ષર *path, size_t path_len, અક્ષર *cpus, અક્ષર *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free અક્ષર *allocated_cpus = ખાલી;
  cleanup_free અક્ષર *allocated_mems = ખાલી;
  cleanup_close પૂર્ણાંક dirfd = -1;
  cleanup_close પૂર્ણાંક mems_fd = -1;
  cleanup_close પૂર્ણાંક cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  પૂર્ણાંક b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (dirfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  જો (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      જો (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      જો (UNLIKELY (cpus_fd < 0))
        પાછા crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (પઠ (cpus_fd, cpus, 256));
      જો (UNLIKELY (b_len < 0))
        પાછા crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      જો (cpus[0] == '\n')
        cpus[0] = '\0';
      જો (cpus[0] != '\0')
        has_cpus = true;
    }

  જો (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      જો (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      જો (UNLIKELY (mems_fd < 0))
        પાછા crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (પઠ (mems_fd, mems, 256));
      જો (UNLIKELY (b_len < 0))
        પાછા crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      જો (mems[0] == '\n')
        mems[0] = '\0';
      જો (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  જો (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      પૂર્ણાંક ret;

      માટે (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      જો (parent_path_len == 1)
        પાછા 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      જો (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  જો (resources && resources->cpu)
    {
      જો (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      જો (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  જો (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (લિખ (cpus_fd, cpus, strlen (cpus)));
      જો (UNLIKELY (b_len < 0))
        પાછા crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  જો (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (લિખ (mems_fd, mems, strlen (mems)));
      જો (UNLIKELY (b_len < 0))
        પાછા crun_make_error (err, errno, "write `cpuset.mems`");
    }

  પાછા 0;
}

પૂર્ણાંક
initialize_cpuset_subsystem (અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *tmp_path = xstrdup (path);
  અક્ષર cpus_buf[257];
  અક્ષર mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  પાછા initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, ખાલી, err);
}

પૂર્ણાંક
initialize_cpuset_subsystem_resources (અચલ અક્ષર *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free અક્ષર *tmp_path = xstrdup (path);
  અક્ષર cpus_buf[257];
  અક્ષર mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  પાછા initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

સ્થિર પૂર્ણાંક
initialize_memory_subsystem (અચલ અક્ષર *path, libcrun_error_t *err)
{
  અચલ અક્ષર *અચલ files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", ખાલી };
  cleanup_close પૂર્ણાંક dirfd = -1;
  પૂર્ણાંક i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (dirfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", path);

  માટે (i = 0; files[i]; i++)
    {
      પૂર્ણાંક ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      જો (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  પાછા 0;
}

પૂર્ણાંક
enter_cgroup_v1_subsystem (pid_t pid, અચલ અક્ષર *subsystem, અચલ અક્ષર *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free અક્ષર *cgroup_path = ખાલી;
  પૂર્ણાંક ret;

  ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, subsystem, path ? path : "", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      જો (UNLIKELY (ret < 0))
        {
          જો (crun_error_get_errno (err) != EROFS)
            પાછા crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          પાછા 0;
        }

      જો (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
      જો (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }
  નહીંતર
    {
      ret = crun_path_exists (cgroup_path, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      જો (ret == 0)
        પાછા 0;
    }

  પાછા move_process_to_cgroup (pid, subsystem, path, err);
}

સ્થિર પૂર્ણાંક
get_file_owner (અચલ અક્ષર *path, uid_t *uid, gid_t *gid)
{
  સંરચના stat st;
  પૂર્ણાંક ret;

#ifdef HAVE_STATX
  સંરચના statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno == ENOSYS || errno == EINVAL)
        જાઓ fallback;

      પાછા ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  પાછા ret;

fallback:
#endif
  ret = stat (path, &st);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  પાછા ret;
}

સ્થિર પૂર્ણાંક
copy_owner (અચલ અક્ષર *from, અચલ અક્ષર *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  પૂર્ણાંક ret;

  ret = get_file_owner (from, &uid, &gid);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  જો (uid == 0 && gid == 0)
    પાછા 0;

  પાછા chown_cgroups (to, uid, gid, err);
}

સ્થિર પૂર્ણાંક
read_unified_cgroup_pid (pid_t pid, અક્ષર **path, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  અક્ષર cgroup_path[32];
  અક્ષર *from, *to;
  cleanup_free અક્ષર *content = ખાલી;
  પૂર્ણાંક len;

  len = snprintf (cgroup_path, કદ (cgroup_path), "/proc/%d/cgroup", pid);
  જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (cgroup_path)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  from = strstr (content, "0::");
  જો (UNLIKELY (from == ખાલી))
    પાછા crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  જો (UNLIKELY (to == ખાલી))
    પાછા crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  પાછા 0;
}

સ્થિર પૂર્ણાંક
enter_cgroup_v1 (pid_t pid, અચલ અક્ષર *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free અક્ષર *content = ખાલી;
  bool entered_any = false;
  size_t content_size;
  અક્ષર *controller;
  અક્ષર *saveptr;
  bool has_data;
  પૂર્ણાંક rootless;
  પૂર્ણાંક ret;

  rootless = is_rootless (err);
  જો (UNLIKELY (rootless < 0))
    પાછા rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  જો (UNLIKELY (ret < 0))
    {
      જો (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          પાછા 0;
        }
      પાછા ret;
    }

  માટે (has_data = read_proc_cgroup (content, &saveptr, ખાલી, &controller, ખાલી);
       has_data;
       has_data = read_proc_cgroup (ખાલી, &saveptr, ખાલી, &controller, ખાલી))
    {
      અક્ષર subsystem_path[64];
      અક્ષર *subsystem;
      size_t len;

      જો (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      જો (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      જો (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, કદ (subsystem_path), CGROUP_મૂળ "/%s", subsystem);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (subsystem_path)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      જો (ret == 0)
        ચાલુ;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      જો (UNLIKELY (ret < 0))
        {
          પૂર્ણાંક errcode = crun_error_get_errno (err);
          જો (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              ચાલુ;
            }
          પાછા ret;
        }
    }

  જો (entered_any)
    પાછા 0;

  પાછા crun_make_error (err, 0, "could not join cgroup");
}

સ્થિર પૂર્ણાંક
enter_cgroup_v2 (pid_t pid, pid_t init_pid, અચલ અક્ષર *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cgroup_path_procs = ખાલી;
  cleanup_free અક્ષર *cgroup_path = ખાલી;
  અક્ષર pid_str[16];
  પૂર્ણાંક repeat;
  પૂર્ણાંક len;
  પૂર્ણાંક ret;

  len = snprintf (pid_str, કદ (pid_str), "%d", pid);
  જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (pid_str)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, path, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  જો (LIKELY (ret >= 0))
    પાછા ret;

  જો (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      જો (LIKELY (ret >= 0))
        પાછા ret;
    }

  જો (create_if_missing || crun_error_get_errno (err) != EBUSY)
    પાછા ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  માટે (repeat = 0;; repeat++)
    {
      cleanup_free અક્ષર *cgroup_crun_exec_path = ખાલી;
      cleanup_free અક્ષર *cgroup_sub_path_procs = ખાલી;

      /* There is an init pid, try to join its cgroup.  */
      જો (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          જો (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              મુક્ત (cgroup_crun_exec_path);
              cgroup_crun_exec_path = ખાલી;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      જો (cgroup_crun_exec_path == ખાલી)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_મૂળ, cgroup_crun_exec_path, "cgroup.procs", ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      જો (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          જો (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              ચાલુ;
            }
          પાછા ret;
        }
      પાછા copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  પાછા ret;
}

પૂર્ણાંક
enter_cgroup (પૂર્ણાંક cgroup_mode, pid_t pid, pid_t init_pid, અચલ અક્ષર *path,
              bool create_if_missing, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  જો (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા 0;
}
