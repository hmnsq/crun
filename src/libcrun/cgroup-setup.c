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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "cgroup.h"
#அந்தர்பாவ "cgroup-internal.h"
#அந்தர்பாவ "cgroup-systemd.h"
#அந்தர்பாவ "cgroup-utils.h"
#அந்தர்பாவ "ebpf.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "status.h"
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <sys/vfs.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ <time.h>

#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <libgen.h>

நிலையான முழு
initialize_cpuset_subsystem_rec (எழுத்து *path, size_t path_len, எழுத்து *cpus, எழுத்து *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free எழுத்து *allocated_cpus = வெற்று;
  cleanup_free எழுத்து *allocated_mems = வெற்று;
  cleanup_close முழு dirfd = -1;
  cleanup_close முழு mems_fd = -1;
  cleanup_close முழு cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  முழு b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (dirfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  எனில் (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      எனில் (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      எனில் (UNLIKELY (cpus_fd < 0))
        திரும்பு crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (பட (cpus_fd, cpus, 256));
      எனில் (UNLIKELY (b_len < 0))
        திரும்பு crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      எனில் (cpus[0] == '\n')
        cpus[0] = '\0';
      எனில் (cpus[0] != '\0')
        has_cpus = true;
    }

  எனில் (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      எனில் (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      எனில் (UNLIKELY (mems_fd < 0))
        திரும்பு crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (பட (mems_fd, mems, 256));
      எனில் (UNLIKELY (b_len < 0))
        திரும்பு crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      எனில் (mems[0] == '\n')
        mems[0] = '\0';
      எனில் (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  எனில் (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      முழு ret;

      க்கு (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      எனில் (parent_path_len == 1)
        திரும்பு 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      எனில் (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  எனில் (resources && resources->cpu)
    {
      எனில் (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      எனில் (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  எனில் (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (லிக (cpus_fd, cpus, strlen (cpus)));
      எனில் (UNLIKELY (b_len < 0))
        திரும்பு crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  எனில் (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (லிக (mems_fd, mems, strlen (mems)));
      எனில் (UNLIKELY (b_len < 0))
        திரும்பு crun_make_error (err, errno, "write `cpuset.mems`");
    }

  திரும்பு 0;
}

முழு
initialize_cpuset_subsystem (மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *tmp_path = xstrdup (path);
  எழுத்து cpus_buf[257];
  எழுத்து mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  திரும்பு initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, வெற்று, err);
}

முழு
initialize_cpuset_subsystem_resources (மாறிலி எழுத்து *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free எழுத்து *tmp_path = xstrdup (path);
  எழுத்து cpus_buf[257];
  எழுத்து mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  திரும்பு initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

நிலையான முழு
initialize_memory_subsystem (மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  மாறிலி எழுத்து *மாறிலி files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", வெற்று };
  cleanup_close முழு dirfd = -1;
  முழு i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (dirfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", path);

  க்கு (i = 0; files[i]; i++)
    {
      முழு ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      எனில் (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  திரும்பு 0;
}

முழு
enter_cgroup_v1_subsystem (pid_t pid, மாறிலி எழுத்து *subsystem, மாறிலி எழுத்து *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free எழுத்து *cgroup_path = வெற்று;
  முழு ret;

  ret = append_paths (&cgroup_path, err, CGROUP_வேர், subsystem, path ? path : "", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (crun_error_get_errno (err) != EROFS)
            திரும்பு crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          திரும்பு 0;
        }

      எனில் (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
      எனில் (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }
  இல்லையெனில்
    {
      ret = crun_path_exists (cgroup_path, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      எனில் (ret == 0)
        திரும்பு 0;
    }

  திரும்பு move_process_to_cgroup (pid, subsystem, path, err);
}

நிலையான முழு
get_file_owner (மாறிலி எழுத்து *path, uid_t *uid, gid_t *gid)
{
  கட்டமைப்பு stat st;
  முழு ret;

#ifdef HAVE_STATX
  கட்டமைப்பு statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno == ENOSYS || errno == EINVAL)
        செல் fallback;

      திரும்பு ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  திரும்பு ret;

fallback:
#endif
  ret = stat (path, &st);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  திரும்பு ret;
}

நிலையான முழு
copy_owner (மாறிலி எழுத்து *from, மாறிலி எழுத்து *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  முழு ret;

  ret = get_file_owner (from, &uid, &gid);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  எனில் (uid == 0 && gid == 0)
    திரும்பு 0;

  திரும்பு chown_cgroups (to, uid, gid, err);
}

நிலையான முழு
read_unified_cgroup_pid (pid_t pid, எழுத்து **path, libcrun_error_t *err)
{
  முழு ret;
  எழுத்து cgroup_path[32];
  எழுத்து *from, *to;
  cleanup_free எழுத்து *content = வெற்று;
  முழு len;

  len = snprintf (cgroup_path, அளவு (cgroup_path), "/proc/%d/cgroup", pid);
  எனில் (UNLIKELY (len >= (முழு) அளவு (cgroup_path)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  from = strstr (content, "0::");
  எனில் (UNLIKELY (from == வெற்று))
    திரும்பு crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  எனில் (UNLIKELY (to == வெற்று))
    திரும்பு crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  திரும்பு 0;
}

நிலையான முழு
enter_cgroup_v1 (pid_t pid, மாறிலி எழுத்து *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free எழுத்து *content = வெற்று;
  bool entered_any = false;
  size_t content_size;
  எழுத்து *controller;
  எழுத்து *saveptr;
  bool has_data;
  முழு rootless;
  முழு ret;

  rootless = is_rootless (err);
  எனில் (UNLIKELY (rootless < 0))
    திரும்பு rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          திரும்பு 0;
        }
      திரும்பு ret;
    }

  க்கு (has_data = read_proc_cgroup (content, &saveptr, வெற்று, &controller, வெற்று);
       has_data;
       has_data = read_proc_cgroup (வெற்று, &saveptr, வெற்று, &controller, வெற்று))
    {
      எழுத்து subsystem_path[64];
      எழுத்து *subsystem;
      size_t len;

      எனில் (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      எனில் (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      எனில் (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, அளவு (subsystem_path), CGROUP_வேர் "/%s", subsystem);
      எனில் (UNLIKELY (len >= (முழு) அளவு (subsystem_path)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      எனில் (ret == 0)
        தொடர்;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      எனில் (UNLIKELY (ret < 0))
        {
          முழு errcode = crun_error_get_errno (err);
          எனில் (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              தொடர்;
            }
          திரும்பு ret;
        }
    }

  எனில் (entered_any)
    திரும்பு 0;

  திரும்பு crun_make_error (err, 0, "could not join cgroup");
}

நிலையான முழு
enter_cgroup_v2 (pid_t pid, pid_t init_pid, மாறிலி எழுத்து *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cgroup_path_procs = வெற்று;
  cleanup_free எழுத்து *cgroup_path = வெற்று;
  எழுத்து pid_str[16];
  முழு repeat;
  முழு len;
  முழு ret;

  len = snprintf (pid_str, அளவு (pid_str), "%d", pid);
  எனில் (UNLIKELY (len >= (முழு) அளவு (pid_str)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_வேர், path, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  எனில் (LIKELY (ret >= 0))
    திரும்பு ret;

  எனில் (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      எனில் (LIKELY (ret >= 0))
        திரும்பு ret;
    }

  எனில் (create_if_missing || crun_error_get_errno (err) != EBUSY)
    திரும்பு ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  க்கு (repeat = 0;; repeat++)
    {
      cleanup_free எழுத்து *cgroup_crun_exec_path = வெற்று;
      cleanup_free எழுத்து *cgroup_sub_path_procs = வெற்று;

      /* There is an init pid, try to join its cgroup.  */
      எனில் (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          எனில் (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              விடுவி (cgroup_crun_exec_path);
              cgroup_crun_exec_path = வெற்று;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      எனில் (cgroup_crun_exec_path == வெற்று)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_வேர், cgroup_crun_exec_path, "cgroup.procs", வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      எனில் (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          எனில் (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              தொடர்;
            }
          திரும்பு ret;
        }
      திரும்பு copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  திரும்பு ret;
}

முழு
enter_cgroup (முழு cgroup_mode, pid_t pid, pid_t init_pid, மாறிலி எழுத்து *path,
              bool create_if_missing, libcrun_error_t *err)
{
  முழு ret;
  எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு 0;
}
