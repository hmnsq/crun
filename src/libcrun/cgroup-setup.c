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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "cgroup.h"
#అన్తర్భావ "cgroup-internal.h"
#అన్తర్భావ "cgroup-systemd.h"
#అన్తర్భావ "cgroup-utils.h"
#అన్తర్భావ "ebpf.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "status.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <sys/vfs.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ <time.h>

#అన్తర్భావ <sched.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <libgen.h>

స్థిర పూర్ణాంకం
initialize_cpuset_subsystem_rec (అక్షరం *మార్గం, size_t మార్గం_len, అక్షరం *cpus, అక్షరం *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free అక్షరం *allocated_cpus = ఖాళీ;
  cleanup_free అక్షరం *allocated_mems = ఖాళీ;
  cleanup_close పూర్ణాంకం dirfd = -1;
  cleanup_close పూర్ణాంకం mems_fd = -1;
  cleanup_close పూర్ణాంకం cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  పూర్ణాంకం b_len;

  dirfd = open (మార్గం, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (dirfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", మార్గం);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  ఒకవేళ (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (cpus_fd < 0))
        తిరిగి crun_make_error (err, errno, "open `%s/%s`", మార్గం, "cpuset.cpus");

      b_len = తాత్కాలిక_FAILURE_RETRY (పఠ (cpus_fd, cpus, 256));
      ఒకవేళ (UNLIKELY (b_len < 0))
        తిరిగి crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      ఒకవేళ (cpus[0] == '\n')
        cpus[0] = '\0';
      ఒకవేళ (cpus[0] != '\0')
        has_cpus = true;
    }

  ఒకవేళ (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (mems_fd < 0))
        తిరిగి crun_make_error (err, errno, "open `%s/%s`", మార్గం, "cpuset.mems");

      b_len = తాత్కాలిక_FAILURE_RETRY (పఠ (mems_fd, mems, 256));
      ఒకవేళ (UNLIKELY (b_len < 0))
        తిరిగి crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      ఒకవేళ (mems[0] == '\n')
        mems[0] = '\0';
      ఒకవేళ (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  ఒకవేళ (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t మాతృ_మార్గం_len;
      పూర్ణాంకం ret;

      కొరకు (మాతృ_మార్గం_len = మార్గం_len - 1; మాతృ_మార్గం_len > 1 && మార్గం[మాతృ_మార్గం_len] != '/'; మాతృ_మార్గం_len--)
        ;
      ఒకవేళ (మాతృ_మార్గం_len == 1)
        తిరిగి 0;

      మార్గం[మాతృ_మార్గం_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (మార్గం, మాతృ_మార్గం_len, cpus, mems, resources, err);
      మార్గం[మాతృ_మార్గం_len] = '/';
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  ఒకవేళ (resources && resources->cpu)
    {
      ఒకవేళ (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      ఒకవేళ (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  ఒకవేళ (cpus_fd >= 0)
    {
      b_len = తాత్కాలిక_FAILURE_RETRY (లిఖ (cpus_fd, cpus, అక్షరశ్రేణి_పొడవు (cpus)));
      ఒకవేళ (UNLIKELY (b_len < 0))
        తిరిగి crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  ఒకవేళ (mems_fd >= 0)
    {
      b_len = తాత్కాలిక_FAILURE_RETRY (లిఖ (mems_fd, mems, అక్షరశ్రేణి_పొడవు (mems)));
      ఒకవేళ (UNLIKELY (b_len < 0))
        తిరిగి crun_make_error (err, errno, "write `cpuset.mems`");
    }

  తిరిగి 0;
}

పూర్ణాంకం
initialize_cpuset_subsystem (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *tmp_మార్గం = xstrdup (మార్గం);
  అక్షరం cpus_buf[257];
  అక్షరం mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  తిరిగి initialize_cpuset_subsystem_rec (tmp_మార్గం, అక్షరశ్రేణి_పొడవు (tmp_మార్గం), cpus_buf, mems_buf, ఖాళీ, err);
}

పూర్ణాంకం
initialize_cpuset_subsystem_resources (స్థిరరాశి అక్షరం *మార్గం, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free అక్షరం *tmp_మార్గం = xstrdup (మార్గం);
  అక్షరం cpus_buf[257];
  అక్షరం mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  తిరిగి initialize_cpuset_subsystem_rec (tmp_మార్గం, అక్షరశ్రేణి_పొడవు (tmp_మార్గం), cpus_buf, mems_buf, resources, err);
}

స్థిర పూర్ణాంకం
initialize_జ్ఞాపకం_subsystem (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *స్థిరరాశి files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", ఖాళీ };
  cleanup_close పూర్ణాంకం dirfd = -1;
  పూర్ణాంకం i;

  dirfd = open (మార్గం, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (dirfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", మార్గం);

  కొరకు (i = 0; files[i]; i++)
    {
      పూర్ణాంకం ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  తిరిగి 0;
}

పూర్ణాంకం
enter_cgroup_v1_subsystem (pid_t pid, స్థిరరాశి అక్షరం *subsystem, స్థిరరాశి అక్షరం *మార్గం, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
  పూర్ణాంకం ret;

  ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, subsystem, మార్గం ? మార్గం : "", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_మార్గం, 0755, false, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (crun_error_get_errno (err) != EROFS)
            తిరిగి crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_మార్గం);

          crun_error_release (err);
          తిరిగి 0;
        }

      ఒకవేళ (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_మార్గం, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
      ఒకవేళ (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_జ్ఞాపకం_subsystem (cgroup_మార్గం, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }
  లేకపోతే
    {
      ret = crun_మార్గం_exists (cgroup_మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      ఒకవేళ (ret == 0)
        తిరిగి 0;
    }

  తిరిగి move_process_గమ్యం_cgroup (pid, subsystem, మార్గం, err);
}

స్థిర పూర్ణాంకం
get_file_owner (స్థిరరాశి అక్షరం *మార్గం, uid_t *uid, gid_t *gid)
{
  నిర్మాణం stat st;
  పూర్ణాంకం ret;

#నిర్వచితమైతే HAVE_STATX
  నిర్మాణం statx stx;

  ret = statx (AT_FDCWD, మార్గం, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno == ENOSYS || errno == EINVAL)
        వెళ్ళు fallback;

      తిరిగి ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  తిరిగి ret;

fallback:
#అయితే_ముగింపు
  ret = stat (మార్గం, &st);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
copy_owner (స్థిరరాశి అక్షరం *from, స్థిరరాశి అక్షరం *గమ్యం, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  పూర్ణాంకం ret;

  ret = get_file_owner (from, &uid, &gid);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  ఒకవేళ (uid == 0 && gid == 0)
    తిరిగి 0;

  తిరిగి chown_cgroups (గమ్యం, uid, gid, err);
}

స్థిర పూర్ణాంకం
read_unified_cgroup_pid (pid_t pid, అక్షరం **మార్గం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  అక్షరం cgroup_మార్గం[32];
  అక్షరం *from, *గమ్యం;
  cleanup_free అక్షరం *content = ఖాళీ;
  పూర్ణాంకం len;

  len = snprintf (cgroup_మార్గం, పరిమాణం (cgroup_మార్గం), "/proc/%d/cgroup", pid);
  ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (cgroup_మార్గం)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_మార్గం[len] = '\0';

  ret = read_all_file (cgroup_మార్గం, &content, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  from = అక్షరశ్రేణి_శోధన (content, "0::");
  ఒకవేళ (UNLIKELY (from == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  గమ్యం = strchr (from, '\n');
  ఒకవేళ (UNLIKELY (గమ్యం == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "cannot parse `%s`", cgroup_మార్గం);
  *గమ్యం = '\0';

  *మార్గం = xstrdup (from);
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
enter_cgroup_v1 (pid_t pid, స్థిరరాశి అక్షరం *మార్గం, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free అక్షరం *content = ఖాళీ;
  bool entered_any = false;
  size_t content_కొలత;
  అక్షరం *controller;
  అక్షరం *saveptr;
  bool has_దత్తాంశం;
  పూర్ణాంకం rootless;
  పూర్ణాంకం ret;

  rootless = is_rootless (err);
  ఒకవేళ (UNLIKELY (rootless < 0))
    తిరిగి rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_కొలత, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          తిరిగి 0;
        }
      తిరిగి ret;
    }

  కొరకు (has_దత్తాంశం = read_proc_cgroup (content, &saveptr, ఖాళీ, &controller, ఖాళీ);
       has_దత్తాంశం;
       has_దత్తాంశం = read_proc_cgroup (ఖాళీ, &saveptr, ఖాళీ, &controller, ఖాళీ))
    {
      అక్షరం subsystem_మార్గం[64];
      అక్షరం *subsystem;
      size_t len;

      ఒకవేళ (has_ఉపసర్గ (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      ఒకవేళ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ఒకవేళ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_మార్గం, పరిమాణం (subsystem_మార్గం), CGROUP_మూలం "/%s", subsystem);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (subsystem_మార్గం)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_మార్గం_exists (subsystem_మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      ఒకవేళ (ret == 0)
        కొనసాగు;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, మార్గం, create_if_missing, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          పూర్ణాంకం errcode = crun_error_get_errno (err);
          ఒకవేళ (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              కొనసాగు;
            }
          తిరిగి ret;
        }
    }

  ఒకవేళ (entered_any)
    తిరిగి 0;

  తిరిగి crun_make_error (err, 0, "could not join cgroup");
}

స్థిర పూర్ణాంకం
enter_cgroup_v2 (pid_t pid, pid_t init_pid, స్థిరరాశి అక్షరం *మార్గం, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cgroup_మార్గం_procs = ఖాళీ;
  cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
  అక్షరం pid_str[16];
  పూర్ణాంకం repeat;
  పూర్ణాంకం len;
  పూర్ణాంకం ret;

  len = snprintf (pid_str, పరిమాణం (pid_str), "%d", pid);
  ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (pid_str)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, మార్గం, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_మార్గం, 0755, false, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = append_paths (&cgroup_మార్గం_procs, err, cgroup_మార్గం, "cgroup.procs", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_మార్గం_procs, pid_str, అక్షరశ్రేణి_పొడవు (pid_str), err);
  ఒకవేళ (LIKELY (ret >= 0))
    తిరిగి ret;

  ఒకవేళ (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_మార్గం_procs, pid_str, అక్షరశ్రేణి_పొడవు (pid_str), err);
      ఒకవేళ (LIKELY (ret >= 0))
        తిరిగి ret;
    }

  ఒకవేళ (create_if_missing || crun_error_get_errno (err) != EBUSY)
    తిరిగి ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  కొరకు (repeat = 0;; repeat++)
    {
      cleanup_free అక్షరం *cgroup_crun_exec_మార్గం = ఖాళీ;
      cleanup_free అక్షరం *cgroup_sub_మార్గం_procs = ఖాళీ;

      /* There is an init pid, try to join its cgroup.  */
      ఒకవేళ (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_మార్గం, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          ఒకవేళ (అక్షరశ్రేణి_పోలిక (మార్గం, cgroup_crun_exec_మార్గం, అక్షరశ్రేణి_పొడవు (మార్గం)))
            {
              విడుదల (cgroup_crun_exec_మార్గం);
              cgroup_crun_exec_మార్గం = ఖాళీ;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      ఒకవేళ (cgroup_crun_exec_మార్గం == ఖాళీ)
        xasprintf (&cgroup_crun_exec_మార్గం, "%s/crun-exec", మార్గం);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = append_paths (&cgroup_sub_మార్గం_procs, err, CGROUP_మూలం, cgroup_crun_exec_మార్గం, "cgroup.procs", ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_మార్గం_procs, pid_str, అక్షరశ్రేణి_పొడవు (pid_str), err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          ఒకవేళ (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              కొనసాగు;
            }
          తిరిగి ret;
        }
      తిరిగి copy_owner (cgroup_మార్గం_procs, cgroup_crun_exec_మార్గం, err);
    }
  తిరిగి ret;
}

పూర్ణాంకం
enter_cgroup (పూర్ణాంకం cgroup_బహుళకం, pid_t pid, pid_t init_pid, స్థిరరాశి అక్షరం *మార్గం,
              bool create_if_missing, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, మార్గం, create_if_missing, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      ret = enter_cgroup_v1 (pid, మార్గం, create_if_missing, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి 0;
}
