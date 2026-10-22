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

#تعریف_کرو GNU_سروت

#ضمن <config.h>
#ضمن "cgroup.h"
#ضمن "cgroup-internal.h"
#ضمن "cgroup-systemd.h"
#ضمن "cgroup-utils.h"
#ضمن "ebpf.h"
#ضمن "utils.h"
#ضمن "status.h"
#ضمن <string.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <sys/vfs.h>
#ضمن <inttypes.h>
#ضمن <time.h>

#ضمن <sched.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <libgen.h>

ساكن صحيح
initialize_cpuset_subsystem_rec (حرف *رستہ, size_t رستہ_len, حرف *cpus, حرف *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free حرف *allocated_cpus = عدم;
  cleanup_free حرف *allocated_mems = عدم;
  cleanup_close صحيح dirfd = -1;
  cleanup_close صحيح mems_fd = -1;
  cleanup_close صحيح cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  صحيح b_len;

  dirfd = open (رستہ, O_DIRECTORY | O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", رستہ);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  إذا (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      إذا (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      إذا (UNLIKELY (cpus_fd < 0))
        أرجع crun_make_error (err, errno, "open `%s/%s`", رستہ, "cpuset.cpus");

      b_len = عارضی_FAILURE_RETRY (اقرأ (cpus_fd, cpus, 256));
      إذا (UNLIKELY (b_len < 0))
        أرجع crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      إذا (cpus[0] == '\n')
        cpus[0] = '\0';
      إذا (cpus[0] != '\0')
        has_cpus = true;
    }

  إذا (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      إذا (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      إذا (UNLIKELY (mems_fd < 0))
        أرجع crun_make_error (err, errno, "open `%s/%s`", رستہ, "cpuset.mems");

      b_len = عارضی_FAILURE_RETRY (اقرأ (mems_fd, mems, 256));
      إذا (UNLIKELY (b_len < 0))
        أرجع crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      إذا (mems[0] == '\n')
        mems[0] = '\0';
      إذا (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  إذا (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t ماپا_رستہ_len;
      صحيح ret;

      لكل (ماپا_رستہ_len = رستہ_len - 1; ماپا_رستہ_len > 1 && رستہ[ماپا_رستہ_len] != '/'; ماپا_رستہ_len--)
        ;
      إذا (ماپا_رستہ_len == 1)
        أرجع 0;

      رستہ[ماپا_رستہ_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (رستہ, ماپا_رستہ_len, cpus, mems, resources, err);
      رستہ[ماپا_رستہ_len] = '/';
      إذا (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  إذا (resources && resources->cpu)
    {
      إذا (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      إذا (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  إذا (cpus_fd >= 0)
    {
      b_len = عارضی_FAILURE_RETRY (اكتب (cpus_fd, cpus, لڑی_لمبائی (cpus)));
      إذا (UNLIKELY (b_len < 0))
        أرجع crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  إذا (mems_fd >= 0)
    {
      b_len = عارضی_FAILURE_RETRY (اكتب (mems_fd, mems, لڑی_لمبائی (mems)));
      إذا (UNLIKELY (b_len < 0))
        أرجع crun_make_error (err, errno, "write `cpuset.mems`");
    }

  أرجع 0;
}

صحيح
initialize_cpuset_subsystem (ثابت حرف *رستہ, libcrun_error_t *err)
{
  cleanup_free حرف *tmp_رستہ = xstrdup (رستہ);
  حرف cpus_buf[257];
  حرف mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  أرجع initialize_cpuset_subsystem_rec (tmp_رستہ, لڑی_لمبائی (tmp_رستہ), cpus_buf, mems_buf, عدم, err);
}

صحيح
initialize_cpuset_subsystem_resources (ثابت حرف *رستہ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free حرف *tmp_رستہ = xstrdup (رستہ);
  حرف cpus_buf[257];
  حرف mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  أرجع initialize_cpuset_subsystem_rec (tmp_رستہ, لڑی_لمبائی (tmp_رستہ), cpus_buf, mems_buf, resources, err);
}

ساكن صحيح
initialize_حافظہ_subsystem (ثابت حرف *رستہ, libcrun_error_t *err)
{
  ثابت حرف *ثابت files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", عدم };
  cleanup_close صحيح dirfd = -1;
  صحيح i;

  dirfd = open (رستہ, O_DIRECTORY | O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", رستہ);

  لكل (i = 0; files[i]; i++)
    {
      صحيح ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      إذا (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  أرجع 0;
}

صحيح
enter_cgroup_v1_subsystem (pid_t pid, ثابت حرف *subsystem, ثابت حرف *رستہ, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_رستہ = عدم;
  صحيح ret;

  ret = append_paths (&cgroup_رستہ, err, CGROUP_جڑ, subsystem, رستہ ? رستہ : "", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_رستہ, 0755, false, err);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (crun_error_get_errno (err) != EROFS)
            أرجع crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_رستہ);

          crun_error_release (err);
          أرجع 0;
        }

      إذا (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_رستہ, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      إذا (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_حافظہ_subsystem (cgroup_رستہ, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }
  وإلا
    {
      ret = crun_رستہ_exists (cgroup_رستہ, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (ret == 0)
        أرجع 0;
    }

  أرجع move_process_منزل_cgroup (pid, subsystem, رستہ, err);
}

ساكن صحيح
get_file_owner (ثابت حرف *رستہ, uid_t *uid, gid_t *gid)
{
  بنية stat st;
  صحيح ret;

#جے_تعریف_شدہ HAVE_STATX
  بنية statx stx;

  ret = statx (AT_FDCWD, رستہ, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;

      أرجع ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  أرجع ret;

fallback:
#جے_انت
  ret = stat (رستہ, &st);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  أرجع ret;
}

ساكن صحيح
copy_owner (ثابت حرف *from, ثابت حرف *منزل, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  صحيح ret;

  ret = get_file_owner (from, &uid, &gid);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  إذا (uid == 0 && gid == 0)
    أرجع 0;

  أرجع chown_cgroups (منزل, uid, gid, err);
}

ساكن صحيح
read_unified_cgroup_pid (pid_t pid, حرف **رستہ, libcrun_error_t *err)
{
  صحيح ret;
  حرف cgroup_رستہ[32];
  حرف *from, *منزل;
  cleanup_free حرف *content = عدم;
  صحيح len;

  len = snprintf (cgroup_رستہ, حجم (cgroup_رستہ), "/proc/%d/cgroup", pid);
  إذا (UNLIKELY (len >= (صحيح) حجم (cgroup_رستہ)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_رستہ[len] = '\0';

  ret = read_all_file (cgroup_رستہ, &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  from = لڑی_کھوج (content, "0::");
  إذا (UNLIKELY (from == عدم))
    أرجع crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  منزل = strchr (from, '\n');
  إذا (UNLIKELY (منزل == عدم))
    أرجع crun_make_error (err, 0, "cannot parse `%s`", cgroup_رستہ);
  *منزل = '\0';

  *رستہ = xstrdup (from);
  أرجع 0;
}

ساكن صحيح
enter_cgroup_v1 (pid_t pid, ثابت حرف *رستہ, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  bool entered_any = false;
  size_t content_ناپ;
  حرف *controller;
  حرف *saveptr;
  bool has_انکڑے;
  صحيح rootless;
  صحيح ret;

  rootless = is_rootless (err);
  إذا (UNLIKELY (rootless < 0))
    أرجع rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_ناپ, err);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          أرجع 0;
        }
      أرجع ret;
    }

  لكل (has_انکڑے = read_proc_cgroup (content, &saveptr, عدم, &controller, عدم);
       has_انکڑے;
       has_انکڑے = read_proc_cgroup (عدم, &saveptr, عدم, &controller, عدم))
    {
      حرف subsystem_رستہ[64];
      حرف *subsystem;
      size_t len;

      إذا (has_اگیتر (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      إذا (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      إذا (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_رستہ, حجم (subsystem_رستہ), CGROUP_جڑ "/%s", subsystem);
      إذا (UNLIKELY (len >= (صحيح) حجم (subsystem_رستہ)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_رستہ_exists (subsystem_رستہ, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (ret == 0)
        استمر;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, رستہ, create_if_missing, err);
      إذا (UNLIKELY (ret < 0))
        {
          صحيح errcode = crun_error_get_errno (err);
          إذا (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              استمر;
            }
          أرجع ret;
        }
    }

  إذا (entered_any)
    أرجع 0;

  أرجع crun_make_error (err, 0, "could not join cgroup");
}

ساكن صحيح
enter_cgroup_v2 (pid_t pid, pid_t init_pid, ثابت حرف *رستہ, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_رستہ_procs = عدم;
  cleanup_free حرف *cgroup_رستہ = عدم;
  حرف pid_str[16];
  صحيح repeat;
  صحيح len;
  صحيح ret;

  len = snprintf (pid_str, حجم (pid_str), "%d", pid);
  إذا (UNLIKELY (len >= (صحيح) حجم (pid_str)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_رستہ, err, CGROUP_جڑ, رستہ, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_رستہ, 0755, false, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = append_paths (&cgroup_رستہ_procs, err, cgroup_رستہ, "cgroup.procs", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_رستہ_procs, pid_str, لڑی_لمبائی (pid_str), err);
  إذا (LIKELY (ret >= 0))
    أرجع ret;

  إذا (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (رستہ, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_رستہ_procs, pid_str, لڑی_لمبائی (pid_str), err);
      إذا (LIKELY (ret >= 0))
        أرجع ret;
    }

  إذا (create_if_missing || crun_error_get_errno (err) != EBUSY)
    أرجع ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  لكل (repeat = 0;; repeat++)
    {
      cleanup_free حرف *cgroup_crun_exec_رستہ = عدم;
      cleanup_free حرف *cgroup_sub_رستہ_procs = عدم;

      /* There is an init pid, try to join its cgroup.  */
      إذا (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_رستہ, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          إذا (لڑی_مقابلہ (رستہ, cgroup_crun_exec_رستہ, لڑی_لمبائی (رستہ)))
            {
              حرر (cgroup_crun_exec_رستہ);
              cgroup_crun_exec_رستہ = عدم;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      إذا (cgroup_crun_exec_رستہ == عدم)
        xasprintf (&cgroup_crun_exec_رستہ, "%s/crun-exec", رستہ);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_رستہ, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = append_paths (&cgroup_sub_رستہ_procs, err, CGROUP_جڑ, cgroup_crun_exec_رستہ, "cgroup.procs", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_رستہ_procs, pid_str, لڑی_لمبائی (pid_str), err);
      إذا (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          إذا (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              استمر;
            }
          أرجع ret;
        }
      أرجع copy_owner (cgroup_رستہ_procs, cgroup_crun_exec_رستہ, err);
    }
  أرجع ret;
}

صحيح
enter_cgroup (صحيح cgroup_منوال, pid_t pid, pid_t init_pid, ثابت حرف *رستہ,
              bool create_if_missing, libcrun_error_t *err)
{
  صحيح ret;
  إذا (cgroup_منوال == CGROUP_منوال_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, رستہ, create_if_missing, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      ret = enter_cgroup_v1 (pid, رستہ, create_if_missing, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}
