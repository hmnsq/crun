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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "cgroup.h"
#അന്തര്ഭാവ "cgroup-internal.h"
#അന്തര്ഭാവ "cgroup-systemd.h"
#അന്തര്ഭാവ "cgroup-utils.h"
#അന്തര്ഭാവ "ebpf.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "status.h"
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <sys/vfs.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ <sched.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <libgen.h>

സ്ഥിതം പൂർണ്ണസംഖ്യ
initialize_cpuset_subsystem_rec (അക്ഷരം *path, size_t path_len, അക്ഷരം *cpus, അക്ഷരം *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *allocated_cpus = ശൂന്യ;
  cleanup_free അക്ഷരം *allocated_mems = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ dirfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ mems_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  പൂർണ്ണസംഖ്യ b_len;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (dirfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", path);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  എങ്കിൽ (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (cpus_fd < 0))
        തിരികെ crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.cpus");

      b_len = TEMP_FAILURE_RETRY (പഠ (cpus_fd, cpus, 256));
      എങ്കിൽ (UNLIKELY (b_len < 0))
        തിരികെ crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      എങ്കിൽ (cpus[0] == '\n')
        cpus[0] = '\0';
      എങ്കിൽ (cpus[0] != '\0')
        has_cpus = true;
    }

  എങ്കിൽ (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (mems_fd < 0))
        തിരികെ crun_make_error (err, errno, "open `%s/%s`", path, "cpuset.mems");

      b_len = TEMP_FAILURE_RETRY (പഠ (mems_fd, mems, 256));
      എങ്കിൽ (UNLIKELY (b_len < 0))
        തിരികെ crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      എങ്കിൽ (mems[0] == '\n')
        mems[0] = '\0';
      എങ്കിൽ (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  എങ്കിൽ (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t parent_path_len;
      പൂർണ്ണസംഖ്യ ret;

      വേണ്ടി (parent_path_len = path_len - 1; parent_path_len > 1 && path[parent_path_len] != '/'; parent_path_len--)
        ;
      എങ്കിൽ (parent_path_len == 1)
        തിരികെ 0;

      path[parent_path_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (path, parent_path_len, cpus, mems, resources, err);
      path[parent_path_len] = '/';
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  എങ്കിൽ (resources && resources->cpu)
    {
      എങ്കിൽ (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      എങ്കിൽ (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  എങ്കിൽ (cpus_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (ലിഖ (cpus_fd, cpus, strlen (cpus)));
      എങ്കിൽ (UNLIKELY (b_len < 0))
        തിരികെ crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  എങ്കിൽ (mems_fd >= 0)
    {
      b_len = TEMP_FAILURE_RETRY (ലിഖ (mems_fd, mems, strlen (mems)));
      എങ്കിൽ (UNLIKELY (b_len < 0))
        തിരികെ crun_make_error (err, errno, "write `cpuset.mems`");
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
initialize_cpuset_subsystem (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *tmp_path = xstrdup (path);
  അക്ഷരം cpus_buf[257];
  അക്ഷരം mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  തിരികെ initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, ശൂന്യ, err);
}

പൂർണ്ണസംഖ്യ
initialize_cpuset_subsystem_resources (സ്ഥിരം അക്ഷരം *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *tmp_path = xstrdup (path);
  അക്ഷരം cpus_buf[257];
  അക്ഷരം mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  തിരികെ initialize_cpuset_subsystem_rec (tmp_path, strlen (tmp_path), cpus_buf, mems_buf, resources, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
initialize_memory_subsystem (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *സ്ഥിരം files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", ശൂന്യ };
  cleanup_close പൂർണ്ണസംഖ്യ dirfd = -1;
  പൂർണ്ണസംഖ്യ i;

  dirfd = open (path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (dirfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", path);

  വേണ്ടി (i = 0; files[i]; i++)
    {
      പൂർണ്ണസംഖ്യ ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
enter_cgroup_v1_subsystem (pid_t pid, സ്ഥിരം അക്ഷരം *subsystem, സ്ഥിരം അക്ഷരം *path, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = append_paths (&cgroup_path, err, CGROUP_വേര്, subsystem, path ? path : "", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (crun_error_get_errno (err) != EROFS)
            തിരികെ crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_path);

          crun_error_release (err);
          തിരികെ 0;
        }

      എങ്കിൽ (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_path, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
      എങ്കിൽ (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_memory_subsystem (cgroup_path, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }
  അല്ലെങ്കിൽ
    {
      ret = crun_path_exists (cgroup_path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      എങ്കിൽ (ret == 0)
        തിരികെ 0;
    }

  തിരികെ move_process_to_cgroup (pid, subsystem, path, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_file_owner (സ്ഥിരം അക്ഷരം *path, uid_t *uid, gid_t *gid)
{
  ഘടന stat st;
  പൂർണ്ണസംഖ്യ ret;

#ifdef HAVE_STATX
  ഘടന statx stx;

  ret = statx (AT_FDCWD, path, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno == ENOSYS || errno == EINVAL)
        പോകുക fallback;

      തിരികെ ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  തിരികെ ret;

fallback:
#endif
  ret = stat (path, &st);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
copy_owner (സ്ഥിരം അക്ഷരം *from, സ്ഥിരം അക്ഷരം *to, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  പൂർണ്ണസംഖ്യ ret;

  ret = get_file_owner (from, &uid, &gid);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  എങ്കിൽ (uid == 0 && gid == 0)
    തിരികെ 0;

  തിരികെ chown_cgroups (to, uid, gid, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_unified_cgroup_pid (pid_t pid, അക്ഷരം **path, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  അക്ഷരം cgroup_path[32];
  അക്ഷരം *from, *to;
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  പൂർണ്ണസംഖ്യ len;

  len = snprintf (cgroup_path, വലുപ്പം (cgroup_path), "/proc/%d/cgroup", pid);
  എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (cgroup_path)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_path[len] = '\0';

  ret = read_all_file (cgroup_path, &content, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  from = strstr (content, "0::");
  എങ്കിൽ (UNLIKELY (from == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  to = strchr (from, '\n');
  എങ്കിൽ (UNLIKELY (to == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "cannot parse `%s`", cgroup_path);
  *to = '\0';

  *path = xstrdup (from);
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
enter_cgroup_v1 (pid_t pid, സ്ഥിരം അക്ഷരം *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  bool entered_any = false;
  size_t content_size;
  അക്ഷരം *controller;
  അക്ഷരം *saveptr;
  bool has_data;
  പൂർണ്ണസംഖ്യ rootless;
  പൂർണ്ണസംഖ്യ ret;

  rootless = is_rootless (err);
  എങ്കിൽ (UNLIKELY (rootless < 0))
    തിരികെ rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          തിരികെ 0;
        }
      തിരികെ ret;
    }

  വേണ്ടി (has_data = read_proc_cgroup (content, &saveptr, ശൂന്യ, &controller, ശൂന്യ);
       has_data;
       has_data = read_proc_cgroup (ശൂന്യ, &saveptr, ശൂന്യ, &controller, ശൂന്യ))
    {
      അക്ഷരം subsystem_path[64];
      അക്ഷരം *subsystem;
      size_t len;

      എങ്കിൽ (has_prefix (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      എങ്കിൽ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      എങ്കിൽ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_path, വലുപ്പം (subsystem_path), CGROUP_വേര് "/%s", subsystem);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (subsystem_path)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_path_exists (subsystem_path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      എങ്കിൽ (ret == 0)
        തുടരുക;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, path, create_if_missing, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          പൂർണ്ണസംഖ്യ errcode = crun_error_get_errno (err);
          എങ്കിൽ (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              തുടരുക;
            }
          തിരികെ ret;
        }
    }

  എങ്കിൽ (entered_any)
    തിരികെ 0;

  തിരികെ crun_make_error (err, 0, "could not join cgroup");
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
enter_cgroup_v2 (pid_t pid, pid_t init_pid, സ്ഥിരം അക്ഷരം *path, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cgroup_path_procs = ശൂന്യ;
  cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
  അക്ഷരം pid_str[16];
  പൂർണ്ണസംഖ്യ repeat;
  പൂർണ്ണസംഖ്യ len;
  പൂർണ്ണസംഖ്യ ret;

  len = snprintf (pid_str, വലുപ്പം (pid_str), "%d", pid);
  എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (pid_str)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_path, err, CGROUP_വേര്, path, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_path, 0755, false, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = append_paths (&cgroup_path_procs, err, cgroup_path, "cgroup.procs", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  എങ്കിൽ (LIKELY (ret >= 0))
    തിരികെ ret;

  എങ്കിൽ (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
      എങ്കിൽ (LIKELY (ret >= 0))
        തിരികെ ret;
    }

  എങ്കിൽ (create_if_missing || crun_error_get_errno (err) != EBUSY)
    തിരികെ ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  വേണ്ടി (repeat = 0;; repeat++)
    {
      cleanup_free അക്ഷരം *cgroup_crun_exec_path = ശൂന്യ;
      cleanup_free അക്ഷരം *cgroup_sub_path_procs = ശൂന്യ;

      /* There is an init pid, try to join its cgroup.  */
      എങ്കിൽ (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_path, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          എങ്കിൽ (strncmp (path, cgroup_crun_exec_path, strlen (path)))
            {
              സ്വതന്ത്രം (cgroup_crun_exec_path);
              cgroup_crun_exec_path = ശൂന്യ;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      എങ്കിൽ (cgroup_crun_exec_path == ശൂന്യ)
        xasprintf (&cgroup_crun_exec_path, "%s/crun-exec", path);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = append_paths (&cgroup_sub_path_procs, err, CGROUP_വേര്, cgroup_crun_exec_path, "cgroup.procs", ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_path_procs, pid_str, strlen (pid_str), err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          എങ്കിൽ (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              തുടരുക;
            }
          തിരികെ ret;
        }
      തിരികെ copy_owner (cgroup_path_procs, cgroup_crun_exec_path, err);
    }
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
enter_cgroup (പൂർണ്ണസംഖ്യ cgroup_mode, pid_t pid, pid_t init_pid, സ്ഥിരം അക്ഷരം *path,
              bool create_if_missing, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, path, create_if_missing, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      ret = enter_cgroup_v1 (pid, path, create_if_missing, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ 0;
}
