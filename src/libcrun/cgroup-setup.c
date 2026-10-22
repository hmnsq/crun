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

#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل "cgroup.h"
#شامل "cgroup-internal.h"
#شامل "cgroup-systemd.h"
#شامل "cgroup-utils.h"
#شامل "ebpf.h"
#شامل "utils.h"
#شامل "status.h"
#شامل <string.h>
#شامل <sys/types.h>
#شامل <signal.h>
#شامل <sys/vfs.h>
#شامل <inttypes.h>
#شامل <time.h>

#شامل <sched.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <libgen.h>

ساکن صحیح
initialize_cpuset_subsystem_rec (حرف *راستہ, size_t راستہ_len, حرف *cpus, حرف *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free حرف *allocated_cpus = تہی;
  cleanup_free حرف *allocated_mems = تہی;
  cleanup_close صحیح dirfd = -1;
  cleanup_close صحیح mems_fd = -1;
  cleanup_close صحیح cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  صحیح b_len;

  dirfd = open (راستہ, O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (dirfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", راستہ);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  اگر (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      اگر (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      اگر (UNLIKELY (cpus_fd < 0))
        واپس crun_make_error (err, errno, "open `%s/%s`", راستہ, "cpuset.cpus");

      b_len = عارضی_FAILURE_RETRY (اقرأ (cpus_fd, cpus, 256));
      اگر (UNLIKELY (b_len < 0))
        واپس crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      اگر (cpus[0] == '\n')
        cpus[0] = '\0';
      اگر (cpus[0] != '\0')
        has_cpus = true;
    }

  اگر (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      اگر (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      اگر (UNLIKELY (mems_fd < 0))
        واپس crun_make_error (err, errno, "open `%s/%s`", راستہ, "cpuset.mems");

      b_len = عارضی_FAILURE_RETRY (اقرأ (mems_fd, mems, 256));
      اگر (UNLIKELY (b_len < 0))
        واپس crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      اگر (mems[0] == '\n')
        mems[0] = '\0';
      اگر (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  اگر (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t والد_راستہ_len;
      صحیح ret;

      برائے (والد_راستہ_len = راستہ_len - 1; والد_راستہ_len > 1 && راستہ[والد_راستہ_len] != '/'; والد_راستہ_len--)
        ;
      اگر (والد_راستہ_len == 1)
        واپس 0;

      راستہ[والد_راستہ_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (راستہ, والد_راستہ_len, cpus, mems, resources, err);
      راستہ[والد_راستہ_len] = '/';
      اگر (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  اگر (resources && resources->cpu)
    {
      اگر (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      اگر (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  اگر (cpus_fd >= 0)
    {
      b_len = عارضی_FAILURE_RETRY (اكتب (cpus_fd, cpus, لڑی_لمبائی (cpus)));
      اگر (UNLIKELY (b_len < 0))
        واپس crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  اگر (mems_fd >= 0)
    {
      b_len = عارضی_FAILURE_RETRY (اكتب (mems_fd, mems, لڑی_لمبائی (mems)));
      اگر (UNLIKELY (b_len < 0))
        واپس crun_make_error (err, errno, "write `cpuset.mems`");
    }

  واپس 0;
}

صحیح
initialize_cpuset_subsystem (ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *tmp_راستہ = xstrdup (راستہ);
  حرف cpus_buf[257];
  حرف mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  واپس initialize_cpuset_subsystem_rec (tmp_راستہ, لڑی_لمبائی (tmp_راستہ), cpus_buf, mems_buf, تہی, err);
}

صحیح
initialize_cpuset_subsystem_resources (ثابت حرف *راستہ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free حرف *tmp_راستہ = xstrdup (راستہ);
  حرف cpus_buf[257];
  حرف mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  واپس initialize_cpuset_subsystem_rec (tmp_راستہ, لڑی_لمبائی (tmp_راستہ), cpus_buf, mems_buf, resources, err);
}

ساکن صحیح
initialize_حافظہ_subsystem (ثابت حرف *راستہ, libcrun_error_t *err)
{
  ثابت حرف *ثابت files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", تہی };
  cleanup_close صحیح dirfd = -1;
  صحیح i;

  dirfd = open (راستہ, O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (dirfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", راستہ);

  برائے (i = 0; files[i]; i++)
    {
      صحیح ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      اگر (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  واپس 0;
}

صحیح
enter_cgroup_v1_subsystem (pid_t pid, ثابت حرف *subsystem, ثابت حرف *راستہ, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_راستہ = تہی;
  صحیح ret;

  ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, subsystem, راستہ ? راستہ : "", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_راستہ, 0755, false, err);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (crun_error_get_errno (err) != EROFS)
            واپس crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_راستہ);

          crun_error_release (err);
          واپس 0;
        }

      اگر (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_راستہ, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
      اگر (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_حافظہ_subsystem (cgroup_راستہ, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }
  ورنہ
    {
      ret = crun_راستہ_exists (cgroup_راستہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      اگر (ret == 0)
        واپس 0;
    }

  واپس move_process_مقصد_cgroup (pid, subsystem, راستہ, err);
}

ساکن صحیح
get_file_owner (ثابت حرف *راستہ, uid_t *uid, gid_t *gid)
{
  ساخت stat st;
  صحیح ret;

#اگر_تعریف_شدہ HAVE_STATX
  ساخت statx stx;

  ret = statx (AT_FDCWD, راستہ, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS || errno == EINVAL)
        جاؤ fallback;

      واپس ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  واپس ret;

fallback:
#اگر_ختم
  ret = stat (راستہ, &st);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  واپس ret;
}

ساکن صحیح
copy_owner (ثابت حرف *from, ثابت حرف *مقصد, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  صحیح ret;

  ret = get_file_owner (from, &uid, &gid);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  اگر (uid == 0 && gid == 0)
    واپس 0;

  واپس chown_cgroups (مقصد, uid, gid, err);
}

ساکن صحیح
read_unified_cgroup_pid (pid_t pid, حرف **راستہ, libcrun_error_t *err)
{
  صحیح ret;
  حرف cgroup_راستہ[32];
  حرف *from, *مقصد;
  cleanup_free حرف *content = تہی;
  صحیح len;

  len = snprintf (cgroup_راستہ, حجم (cgroup_راستہ), "/proc/%d/cgroup", pid);
  اگر (UNLIKELY (len >= (صحیح) حجم (cgroup_راستہ)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_راستہ[len] = '\0';

  ret = read_all_file (cgroup_راستہ, &content, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  from = لڑی_تلاش (content, "0::");
  اگر (UNLIKELY (from == تہی))
    واپس crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  مقصد = strchr (from, '\n');
  اگر (UNLIKELY (مقصد == تہی))
    واپس crun_make_error (err, 0, "cannot parse `%s`", cgroup_راستہ);
  *مقصد = '\0';

  *راستہ = xstrdup (from);
  واپس 0;
}

ساکن صحیح
enter_cgroup_v1 (pid_t pid, ثابت حرف *راستہ, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free حرف *content = تہی;
  bool entered_any = false;
  size_t content_سائز;
  حرف *controller;
  حرف *saveptr;
  bool has_ڈیٹا;
  صحیح rootless;
  صحیح ret;

  rootless = is_rootless (err);
  اگر (UNLIKELY (rootless < 0))
    واپس rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_سائز, err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          واپس 0;
        }
      واپس ret;
    }

  برائے (has_ڈیٹا = read_proc_cgroup (content, &saveptr, تہی, &controller, تہی);
       has_ڈیٹا;
       has_ڈیٹا = read_proc_cgroup (تہی, &saveptr, تہی, &controller, تہی))
    {
      حرف subsystem_راستہ[64];
      حرف *subsystem;
      size_t len;

      اگر (has_سابقہ (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      اگر (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      اگر (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_راستہ, حجم (subsystem_راستہ), CGROUP_جڑ "/%s", subsystem);
      اگر (UNLIKELY (len >= (صحیح) حجم (subsystem_راستہ)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_راستہ_exists (subsystem_راستہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      اگر (ret == 0)
        جاری;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, راستہ, create_if_missing, err);
      اگر (UNLIKELY (ret < 0))
        {
          صحیح errcode = crun_error_get_errno (err);
          اگر (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              جاری;
            }
          واپس ret;
        }
    }

  اگر (entered_any)
    واپس 0;

  واپس crun_make_error (err, 0, "could not join cgroup");
}

ساکن صحیح
enter_cgroup_v2 (pid_t pid, pid_t init_pid, ثابت حرف *راستہ, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_راستہ_procs = تہی;
  cleanup_free حرف *cgroup_راستہ = تہی;
  حرف pid_str[16];
  صحیح repeat;
  صحیح len;
  صحیح ret;

  len = snprintf (pid_str, حجم (pid_str), "%d", pid);
  اگر (UNLIKELY (len >= (صحیح) حجم (pid_str)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, راستہ, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_راستہ, 0755, false, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = append_paths (&cgroup_راستہ_procs, err, cgroup_راستہ, "cgroup.procs", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_راستہ_procs, pid_str, لڑی_لمبائی (pid_str), err);
  اگر (LIKELY (ret >= 0))
    واپس ret;

  اگر (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (راستہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_راستہ_procs, pid_str, لڑی_لمبائی (pid_str), err);
      اگر (LIKELY (ret >= 0))
        واپس ret;
    }

  اگر (create_if_missing || crun_error_get_errno (err) != EBUSY)
    واپس ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  برائے (repeat = 0;; repeat++)
    {
      cleanup_free حرف *cgroup_crun_exec_راستہ = تہی;
      cleanup_free حرف *cgroup_sub_راستہ_procs = تہی;

      /* There is an init pid, try to join its cgroup.  */
      اگر (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_راستہ, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          اگر (لڑی_موازنہ (راستہ, cgroup_crun_exec_راستہ, لڑی_لمبائی (راستہ)))
            {
              آزاد (cgroup_crun_exec_راستہ);
              cgroup_crun_exec_راستہ = تہی;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      اگر (cgroup_crun_exec_راستہ == تہی)
        xasprintf (&cgroup_crun_exec_راستہ, "%s/crun-exec", راستہ);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_راستہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = append_paths (&cgroup_sub_راستہ_procs, err, CGROUP_جڑ, cgroup_crun_exec_راستہ, "cgroup.procs", تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_راستہ_procs, pid_str, لڑی_لمبائی (pid_str), err);
      اگر (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          اگر (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              جاری;
            }
          واپس ret;
        }
      واپس copy_owner (cgroup_راستہ_procs, cgroup_crun_exec_راستہ, err);
    }
  واپس ret;
}

صحیح
enter_cgroup (صحیح cgroup_منوال, pid_t pid, pid_t init_pid, ثابت حرف *راستہ,
              bool create_if_missing, libcrun_error_t *err)
{
  صحیح ret;
  اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, راستہ, create_if_missing, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      ret = enter_cgroup_v1 (pid, راستہ, create_if_missing, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس 0;
}
