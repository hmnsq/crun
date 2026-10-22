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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-systemd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ebpf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/vfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <libgen.h>

ਸਥਿਰ ਪੂਰਨ
initialize_cpuset_subsystem_rec (ਅੱਖਰ *ਰਸਤਾ, size_t ਰਸਤਾ_len, ਅੱਖਰ *cpus, ਅੱਖਰ *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *allocated_cpus = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *allocated_mems = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ dirfd = -1;
  cleanup_close ਪੂਰਨ mems_fd = -1;
  cleanup_close ਪੂਰਨ cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  ਪੂਰਨ b_len;

  dirfd = open (ਰਸਤਾ, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (dirfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਰਸਤਾ);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  ਜੇ (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      ਜੇ (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      ਜੇ (UNLIKELY (cpus_fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s/%s`", ਰਸਤਾ, "cpuset.cpus");

      b_len = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (cpus_fd, cpus, 256));
      ਜੇ (UNLIKELY (b_len < 0))
        ਵਾਪਸ crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      ਜੇ (cpus[0] == '\n')
        cpus[0] = '\0';
      ਜੇ (cpus[0] != '\0')
        has_cpus = true;
    }

  ਜੇ (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      ਜੇ (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      ਜੇ (UNLIKELY (mems_fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s/%s`", ਰਸਤਾ, "cpuset.mems");

      b_len = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (mems_fd, mems, 256));
      ਜੇ (UNLIKELY (b_len < 0))
        ਵਾਪਸ crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      ਜੇ (mems[0] == '\n')
        mems[0] = '\0';
      ਜੇ (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  ਜੇ (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t ਮਾਪਾ_ਰਸਤਾ_len;
      ਪੂਰਨ ret;

      ਲਈ (ਮਾਪਾ_ਰਸਤਾ_len = ਰਸਤਾ_len - 1; ਮਾਪਾ_ਰਸਤਾ_len > 1 && ਰਸਤਾ[ਮਾਪਾ_ਰਸਤਾ_len] != '/'; ਮਾਪਾ_ਰਸਤਾ_len--)
        ;
      ਜੇ (ਮਾਪਾ_ਰਸਤਾ_len == 1)
        ਵਾਪਸ 0;

      ਰਸਤਾ[ਮਾਪਾ_ਰਸਤਾ_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (ਰਸਤਾ, ਮਾਪਾ_ਰਸਤਾ_len, cpus, mems, resources, err);
      ਰਸਤਾ[ਮਾਪਾ_ਰਸਤਾ_len] = '/';
      ਜੇ (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  ਜੇ (resources && resources->cpu)
    {
      ਜੇ (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      ਜੇ (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  ਜੇ (cpus_fd >= 0)
    {
      b_len = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (cpus_fd, cpus, ਲੜੀ_ਲੰਬਾਈ (cpus)));
      ਜੇ (UNLIKELY (b_len < 0))
        ਵਾਪਸ crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  ਜੇ (mems_fd >= 0)
    {
      b_len = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (mems_fd, mems, ਲੜੀ_ਲੰਬਾਈ (mems)));
      ਜੇ (UNLIKELY (b_len < 0))
        ਵਾਪਸ crun_make_error (err, errno, "write `cpuset.mems`");
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
initialize_cpuset_subsystem (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *tmp_ਰਸਤਾ = xstrdup (ਰਸਤਾ);
  ਅੱਖਰ cpus_buf[257];
  ਅੱਖਰ mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ਵਾਪਸ initialize_cpuset_subsystem_rec (tmp_ਰਸਤਾ, ਲੜੀ_ਲੰਬਾਈ (tmp_ਰਸਤਾ), cpus_buf, mems_buf, ਨਿਰਾ, err);
}

ਪੂਰਨ
initialize_cpuset_subsystem_resources (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *tmp_ਰਸਤਾ = xstrdup (ਰਸਤਾ);
  ਅੱਖਰ cpus_buf[257];
  ਅੱਖਰ mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ਵਾਪਸ initialize_cpuset_subsystem_rec (tmp_ਰਸਤਾ, ਲੜੀ_ਲੰਬਾਈ (tmp_ਰਸਤਾ), cpus_buf, mems_buf, resources, err);
}

ਸਥਿਰ ਪੂਰਨ
initialize_ਸਮ੍ਰਿਤੀ_subsystem (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *ਅਚਲ files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", ਨਿਰਾ };
  cleanup_close ਪੂਰਨ dirfd = -1;
  ਪੂਰਨ i;

  dirfd = open (ਰਸਤਾ, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (dirfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਰਸਤਾ);

  ਲਈ (i = 0; files[i]; i++)
    {
      ਪੂਰਨ ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
enter_cgroup_v1_subsystem (pid_t pid, ਅਚਲ ਅੱਖਰ *subsystem, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, subsystem, ਰਸਤਾ ? ਰਸਤਾ : "", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_ਰਸਤਾ, 0755, false, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (crun_error_get_errno (err) != EROFS)
            ਵਾਪਸ crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_ਰਸਤਾ);

          crun_error_release (err);
          ਵਾਪਸ 0;
        }

      ਜੇ (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_ਰਸਤਾ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
      ਜੇ (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_ਸਮ੍ਰਿਤੀ_subsystem (cgroup_ਰਸਤਾ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ret = crun_ਰਸਤਾ_exists (cgroup_ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      ਜੇ (ret == 0)
        ਵਾਪਸ 0;
    }

  ਵਾਪਸ move_process_ਮੰਜ਼ਿਲ_cgroup (pid, subsystem, ਰਸਤਾ, err);
}

ਸਥਿਰ ਪੂਰਨ
get_file_owner (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, uid_t *uid, gid_t *gid)
{
  ਬਣਤਰ stat st;
  ਪੂਰਨ ret;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_STATX
  ਬਣਤਰ statx stx;

  ret = statx (AT_FDCWD, ਰਸਤਾ, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno == ENOSYS || errno == EINVAL)
        ਜਾਓ fallback;

      ਵਾਪਸ ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  ਵਾਪਸ ret;

fallback:
#ਜੇ_ਅੰਤ
  ret = stat (ਰਸਤਾ, &st);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
copy_owner (ਅਚਲ ਅੱਖਰ *from, ਅਚਲ ਅੱਖਰ *ਮੰਜ਼ਿਲ, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  ਪੂਰਨ ret;

  ret = get_file_owner (from, &uid, &gid);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  ਜੇ (uid == 0 && gid == 0)
    ਵਾਪਸ 0;

  ਵਾਪਸ chown_cgroups (ਮੰਜ਼ਿਲ, uid, gid, err);
}

ਸਥਿਰ ਪੂਰਨ
read_unified_cgroup_pid (pid_t pid, ਅੱਖਰ **ਰਸਤਾ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਅੱਖਰ cgroup_ਰਸਤਾ[32];
  ਅੱਖਰ *from, *ਮੰਜ਼ਿਲ;
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  ਪੂਰਨ len;

  len = snprintf (cgroup_ਰਸਤਾ, ਆਕਾਰ (cgroup_ਰਸਤਾ), "/proc/%d/cgroup", pid);
  ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (cgroup_ਰਸਤਾ)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_ਰਸਤਾ[len] = '\0';

  ret = read_all_file (cgroup_ਰਸਤਾ, &content, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  from = ਲੜੀ_ਖੋਜ (content, "0::");
  ਜੇ (UNLIKELY (from == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  ਮੰਜ਼ਿਲ = strchr (from, '\n');
  ਜੇ (UNLIKELY (ਮੰਜ਼ਿਲ == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "cannot parse `%s`", cgroup_ਰਸਤਾ);
  *ਮੰਜ਼ਿਲ = '\0';

  *ਰਸਤਾ = xstrdup (from);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
enter_cgroup_v1 (pid_t pid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  bool entered_any = false;
  size_t content_ਮਾਪ;
  ਅੱਖਰ *controller;
  ਅੱਖਰ *saveptr;
  bool has_ਅੰਕੜੇ;
  ਪੂਰਨ rootless;
  ਪੂਰਨ ret;

  rootless = is_rootless (err);
  ਜੇ (UNLIKELY (rootless < 0))
    ਵਾਪਸ rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_ਮਾਪ, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ਵਾਪਸ 0;
        }
      ਵਾਪਸ ret;
    }

  ਲਈ (has_ਅੰਕੜੇ = read_proc_cgroup (content, &saveptr, ਨਿਰਾ, &controller, ਨਿਰਾ);
       has_ਅੰਕੜੇ;
       has_ਅੰਕੜੇ = read_proc_cgroup (ਨਿਰਾ, &saveptr, ਨਿਰਾ, &controller, ਨਿਰਾ))
    {
      ਅੱਖਰ subsystem_ਰਸਤਾ[64];
      ਅੱਖਰ *subsystem;
      size_t len;

      ਜੇ (has_ਅਗੇਤਰ (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      ਜੇ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ਜੇ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_ਰਸਤਾ, ਆਕਾਰ (subsystem_ਰਸਤਾ), CGROUP_ਜੜ੍ਹ "/%s", subsystem);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (subsystem_ਰਸਤਾ)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_ਰਸਤਾ_exists (subsystem_ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      ਜੇ (ret == 0)
        ਜਾਰੀ;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, ਰਸਤਾ, create_if_missing, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਪੂਰਨ errcode = crun_error_get_errno (err);
          ਜੇ (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              ਜਾਰੀ;
            }
          ਵਾਪਸ ret;
        }
    }

  ਜੇ (entered_any)
    ਵਾਪਸ 0;

  ਵਾਪਸ crun_make_error (err, 0, "could not join cgroup");
}

ਸਥਿਰ ਪੂਰਨ
enter_cgroup_v2 (pid_t pid, pid_t init_pid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ_procs = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
  ਅੱਖਰ pid_str[16];
  ਪੂਰਨ repeat;
  ਪੂਰਨ len;
  ਪੂਰਨ ret;

  len = snprintf (pid_str, ਆਕਾਰ (pid_str), "%d", pid);
  ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (pid_str)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_ਰਸਤਾ, 0755, false, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = append_paths (&cgroup_ਰਸਤਾ_procs, err, cgroup_ਰਸਤਾ, "cgroup.procs", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_ਰਸਤਾ_procs, pid_str, ਲੜੀ_ਲੰਬਾਈ (pid_str), err);
  ਜੇ (LIKELY (ret >= 0))
    ਵਾਪਸ ret;

  ਜੇ (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_ਰਸਤਾ_procs, pid_str, ਲੜੀ_ਲੰਬਾਈ (pid_str), err);
      ਜੇ (LIKELY (ret >= 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (create_if_missing || crun_error_get_errno (err) != EBUSY)
    ਵਾਪਸ ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  ਲਈ (repeat = 0;; repeat++)
    {
      cleanup_free ਅੱਖਰ *cgroup_crun_exec_ਰਸਤਾ = ਨਿਰਾ;
      cleanup_free ਅੱਖਰ *cgroup_sub_ਰਸਤਾ_procs = ਨਿਰਾ;

      /* There is an init pid, try to join its cgroup.  */
      ਜੇ (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_ਰਸਤਾ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          ਜੇ (ਲੜੀ_ਤੁਲਨਾ (ਰਸਤਾ, cgroup_crun_exec_ਰਸਤਾ, ਲੜੀ_ਲੰਬਾਈ (ਰਸਤਾ)))
            {
              ਮੁਕਤ (cgroup_crun_exec_ਰਸਤਾ);
              cgroup_crun_exec_ਰਸਤਾ = ਨਿਰਾ;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      ਜੇ (cgroup_crun_exec_ਰਸਤਾ == ਨਿਰਾ)
        xasprintf (&cgroup_crun_exec_ਰਸਤਾ, "%s/crun-exec", ਰਸਤਾ);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = append_paths (&cgroup_sub_ਰਸਤਾ_procs, err, CGROUP_ਜੜ੍ਹ, cgroup_crun_exec_ਰਸਤਾ, "cgroup.procs", ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_ਰਸਤਾ_procs, pid_str, ਲੜੀ_ਲੰਬਾਈ (pid_str), err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          ਜੇ (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              ਜਾਰੀ;
            }
          ਵਾਪਸ ret;
        }
      ਵਾਪਸ copy_owner (cgroup_ਰਸਤਾ_procs, cgroup_crun_exec_ਰਸਤਾ, err);
    }
  ਵਾਪਸ ret;
}

ਪੂਰਨ
enter_cgroup (ਪੂਰਨ cgroup_ਬਹੁਲਕ, pid_t pid, pid_t init_pid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
              bool create_if_missing, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, ਰਸਤਾ, create_if_missing, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      ret = enter_cgroup_v1 (pid, ਰਸਤਾ, create_if_missing, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ 0;
}
