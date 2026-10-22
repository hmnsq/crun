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
#అన్తర్భావ "cgroup-setup.h"
#అన్తర్భావ "ebpf.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "status.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <sys/vfs.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <poll.h>
#అన్తర్భావ <sys/inotify.h>

#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <libgen.h>

నిర్మాణం symlink_s
{
  స్థిరరాశి అక్షరం *name;
  స్థిరరాశి అక్షరం *లక్ష్యం;
};

స్థిర నిర్మాణం symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { ఖాళీ, ఖాళీ } };

పూర్ణాంకం
libcrun_cgroups_create_symlinks (పూర్ణాంకం dirfd, libcrun_error_t *err)
{
  పూర్ణాంకం i;

  కొరకు (i = 0; cgroup_symlinks[i].name; i++)
    {
      పూర్ణాంకం ret;

      ret = symlinkat (cgroup_symlinks[i].లక్ష్యం, dirfd, cgroup_symlinks[i].name);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (errno == ENOENT || errno == EEXIST)
            కొనసాగు;
          తిరిగి crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  తిరిగి 0;
}

పూర్ణాంకం
maybe_make_cgroup_threaded (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cgroup_మార్గం_type = ఖాళీ;
  స్థిరరాశి అక్షరం *స్థిరరాశి threaded = "threaded";
  cleanup_free అక్షరం *content = ఖాళీ;
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  స్థిరరాశి అక్షరం *మాతృ_2;
  size_t కొలత;
  అక్షరం *it;
  పూర్ణాంకం ret;

  ఒకవేళ (మార్గం == ఖాళీ)
    తిరిగి 0;

  మార్గం = consume_slashes (మార్గం);

  ఒకవేళ (మార్గం[0] == '\0')
    తిరిగి 0;

  ret = append_paths (&cgroup_మార్గం_type, err, CGROUP_మూలం, మార్గం, "cgroup.type", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = read_all_file (cgroup_మార్గం_type, &content, &కొలత, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (కొలత > 0)
    {
      it = content + కొలత - 1;
      అయితే (*it == '\n' && it > content)
        *it-- = '\0';
    }

  ఒకవేళ (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    తిరిగి 0;

  బఫరు = xstrdup (మార్గం);
  మాతృ_2 = consume_slashes (dirname (బఫరు));
  ఒకవేళ (మాతృ_2[0] && strcmp (మాతృ_2, "."))
    {
      ret = maybe_make_cgroup_threaded (మాతృ_2, err);
      ఒకవేళ (ret < 0)
        తిరిగి ret;
    }
  తిరిగి write_cgroup_file (AT_FDCWD, cgroup_మార్గం_type, threaded, అక్షరశ్రేణి_పొడవు (threaded), err);
}

పూర్ణాంకం
move_process_గమ్యం_cgroup (pid_t pid, స్థిరరాశి అక్షరం *subsystem, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cgroup_మార్గం_procs = ఖాళీ;
  అక్షరం pid_str[16];
  పూర్ణాంకం ret;

  ret = append_paths (&cgroup_మార్గం_procs, err, CGROUP_మూలం,
                      subsystem ? subsystem : "", మార్గం ? మార్గం : "",
                      "cgroup.procs", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = snprintf (pid_str, పరిమాణం (pid_str), "%d", pid);
  ఒకవేళ (UNLIKELY (ret >= (పూర్ణాంకం) పరిమాణం (pid_str)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_మార్గం_procs, pid_str, అక్షరశ్రేణి_పొడవు (pid_str), err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = ఖాళీ;
          పూర్ణాంకం బహుళకం;

          బహుళకం = libcrun_get_cgroup_బహుళకం (&tmp_err);
          ఒకవేళ (UNLIKELY (బహుళకం < 0 || బహుళకం != CGROUP_బహుళకం_UNIFIED))
            {
              crun_error_release (&tmp_err);
              తిరిగి ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (మార్గం, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          తిరిగి write_cgroup_file (AT_FDCWD, cgroup_మార్గం_procs, pid_str, అక్షరశ్రేణి_పొడవు (pid_str), err);
        }

      తిరిగి ret;
    }
  తిరిగి ret;
}

#నిర్వచితం_కాకపోతే CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#అయితే_ముగింపు

స్థిర పూర్ణాంకం
detect_cgroup_బహుళకం (libcrun_error_t *err)
{
  నిర్మాణం statfs stat;
  పూర్ణాంకం ret;

  ret = statfs (CGROUP_మూలం, &stat);
  ఒకవేళ (ret < 0)
    తిరిగి crun_make_error (err, errno, "statfs `" CGROUP_మూలం "`");
  ఒకవేళ (stat.f_type == CGROUP2_SUPER_MAGIC)
    తిరిగి CGROUP_బహుళకం_UNIFIED;
  ఒకవేళ (stat.f_type != TMPFS_MAGIC)
    తిరిగి crun_make_error (err, 0, "invalid file system type on `" CGROUP_మూలం "`");
  ret = statfs (CGROUP_మూలం "/unified", &stat);
  ఒకవేళ (ret < 0 && errno != ENOENT)
    తిరిగి crun_make_error (err, errno, "statfs `" CGROUP_మూలం "/unified`");
  ఒకవేళ (ret < 0)
    తిరిగి CGROUP_బహుళకం_LEGACY;
  తిరిగి stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_బహుళకం_HYBRID : CGROUP_బహుళకం_LEGACY;
}

పూర్ణాంకం
libcrun_get_cgroup_బహుళకం (libcrun_error_t *err)
{
  పూర్ణాంకం tmp;
  స్థిర పూర్ణాంకం cgroup_బహుళకం = 0;

  ఒకవేళ (cgroup_బహుళకం)
    తిరిగి cgroup_బహుళకం;

  tmp = detect_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (tmp < 0))
    తిరిగి tmp;

  cgroup_బహుళకం = tmp;

  తిరిగి cgroup_బహుళకం;
}

స్థిర పూర్ణాంకం
get_cgroup_process_from_content (అక్షరం *content, పూర్ణాంకం cgroup_బహుళకం, అక్షరం **మార్గం, bool absolute, libcrun_error_t *err)
{
  అక్షరం *cg_మార్గం = ఖాళీ;
  అక్షరం *controller;
  అక్షరం *saveptr;
  bool has_దత్తాంశం;

  కొరకు (has_దత్తాంశం = read_proc_cgroup (content, &saveptr, ఖాళీ, &controller, &cg_మార్గం);
       has_దత్తాంశం;
       has_దత్తాంశం = read_proc_cgroup (ఖాళీ, &saveptr, ఖాళీ, &controller, &cg_మార్గం))
    {
      ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
        {
          ఒకవేళ (strcmp (controller, "") == 0 && అక్షరశ్రేణి_పొడవు (cg_మార్గం) > 0)
            వెళ్ళు దొరికింది;
        }
      లేకపోతే
        {
          ఒకవేళ (strcmp (controller, "memory"))
            వెళ్ళు దొరికింది;
        }
    }

  తిరిగి crun_make_error (err, 0, "cannot find cgroup for the process");

దొరికింది:
  ఒకవేళ (absolute)
    తిరిగి append_paths (మార్గం, err, CGROUP_మూలం, cg_మార్గం, ఖాళీ);

  *మార్గం = xstrdup (cg_మార్గం);
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_get_cgroup_process_at (పూర్ణాంకం dirfd, అక్షరం **మార్గం, bool absolute, libcrun_error_t *err)
{
  cleanup_free అక్షరం *content = ఖాళీ;
  size_t content_కొలత;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం ret;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_కొలత, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి get_cgroup_process_from_content (content, cgroup_బహుళకం, మార్గం, absolute, err);
}

పూర్ణాంకం
libcrun_get_cgroup_process (pid_t pid, అక్షరం **మార్గం, bool absolute, libcrun_error_t *err)
{
  cleanup_free అక్షరం *content = ఖాళీ;
  అక్షరం proc_cgroup_file[64];
  size_t content_కొలత;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం ret;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  లేకపోతే
    {
      పూర్ణాంకం len = snprintf (proc_cgroup_file, పరిమాణం (proc_cgroup_file), "/proc/%d/cgroup", pid);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (proc_cgroup_file)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_కొలత, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి get_cgroup_process_from_content (content, cgroup_బహుళకం, మార్గం, absolute, err);
}

స్థిర పూర్ణాంకం
read_pids_file (పూర్ణాంకం dfd, స్థిరరాశి అక్షరం *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం tasksfd = -1;
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  అక్షరం *saveptr = ఖాళీ;
  size_t n_new_pids;
  size_t len;
  అక్షరం *it;
  పూర్ణాంకం ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  ఒకవేళ (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      ఒకవేళ (errno == ENOENT || errno == ENODEV)
        తిరిగి 0;
      తిరిగి crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &బఫరు, &len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (len > 0)
    {
      కొరకు (n_new_pids = 0, it = బఫరు; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      ఒకవేళ (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, పరిమాణం (pid_t) * *allocated);
        }

      కొరకు (it = strtok_r (బఫరు, "\n", &saveptr); it; it = strtok_r (ఖాళీ, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, ఖాళీ, 10);

          ఒకవేళ (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  తిరిగి 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
నిర్మాణం cgroup_walk_dir_s
{
  అక్షరం *name;
  size_t లోతు;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
నిర్మాణం cgroup_walk_s
{
  పూర్ణాంకం dfd;
  size_t లోతు;
  నిర్మాణం cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

స్థిర శూన్య
cgroup_walk_తీయి (నిర్మాణం cgroup_walk_s *walk)
{
  విడుదల (walk->dirs[--walk->n_dirs].name);
}

స్థిర శూన్య
cleanup_cgroup_walkp (నిర్మాణం cgroup_walk_s *walk)
{
  పూర్ణాంకం saved_errno = errno;

  అయితే (walk->n_dirs > 0)
    cgroup_walk_తీయి (walk);
  విడుదల (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#నిర్వచించు cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
స్థిర పూర్ణాంకం
cgroup_walk_enter (నిర్మాణం cgroup_walk_s *walk, పూర్ణాంకం dfd)
{
  cleanup_close పూర్ణాంకం dfd_clone = -1;
  cleanup_dir DIR *dir = ఖాళీ;
  నిర్మాణం dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->లోతు++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (dfd_clone < 0))
    తిరిగి -1;

  dir = fdopendir (dfd_clone);
  ఒకవేళ (UNLIKELY (dir == ఖాళీ))
    తిరిగి -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  కొరకు (de = readdir (dir); de; de = readdir (dir))
    {
      నిర్మాణం cgroup_walk_dir_s *d;

      ఒకవేళ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        కొనసాగు;

      ఒకవేళ (de->d_type != DT_DIR)
        కొనసాగు;

      ఒకవేళ (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, పరిమాణం (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->లోతు = walk->లోతు + 1;
      d->entered = false;
    }

  తిరిగి 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
స్థిర పూర్ణాంకం
cgroup_walk_తదుపరి (నిర్మాణం cgroup_walk_s *walk, నిర్మాణం cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : ఖాళీ;

  అయితే (*d && walk->లోతు >= (*d)->లోతు)
    {
      పూర్ణాంకం మాతృ_2;

      మాతృ_2 = openat (walk->dfd, "..", O_DIRECTORY | O_మార్గం | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (మాతృ_2 < 0))
        తిరిగి -1;

      close_and_reset (&walk->dfd);
      walk->dfd = మాతృ_2;
      walk->లోతు--;
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
read_pids_cgroup_file (పూర్ణాంకం dfd, స్థిరరాశి అక్షరం *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk నిర్మాణం cgroup_walk_s walk = { .dfd = -1 };
  పూర్ణాంకం dfd_clone;
  పూర్ణాంకం ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (! recurse)
    తిరిగి 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (dfd_clone < 0))
    తిరిగి crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "open cgroup sub-directory");

  కొరకు (;;)
    {
      నిర్మాణం cgroup_walk_dir_s *d;
      cleanup_close పూర్ణాంకం nfd = -1;

      ret = cgroup_walk_తదుపరి (&walk, &d);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "open parent cgroup directory");
      ఒకవేళ (d == ఖాళీ)
        ఆపు;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (nfd < 0))
        {
          పూర్ణాంకం saved_errno = errno;

          ఒకవేళ (saved_errno == ENOENT)
            {
              cgroup_walk_తీయి (&walk);
              కొనసాగు;
            }
          తిరిగి crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_తీయి (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          పూర్ణాంకం e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          ఒకవేళ (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            తిరిగి ret;
          crun_error_release (err);
          కొనసాగు;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      ఒకవేళ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        తిరిగి crun_make_error (err, errno, "open cgroup sub-directory");
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
read_pids_cgroup (పూర్ణాంకం dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  ఒకవేళ (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
rmdir_all_fd (పూర్ణాంకం dfd)
{
  cleanup_cgroup_walk నిర్మాణం cgroup_walk_s walk = { .dfd = -1 };
  పూర్ణాంకం ret;

  ret = cgroup_walk_enter (&walk, dfd);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  కొరకు (;;)
    {
      నిర్మాణం cgroup_walk_dir_s *d;
      పూర్ణాంకం బిడ్డ_dfd;

      ret = cgroup_walk_తదుపరి (&walk, &d);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      ఒకవేళ (d == ఖాళీ)
        ఆపు;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      ఒకవేళ (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_తీయి (&walk);
          కొనసాగు;
        }

      బిడ్డ_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (బిడ్డ_dfd < 0))
        {
          ఒకవేళ (errno == ENOENT)
            {
              cgroup_walk_తీయి (&walk);
              కొనసాగు;
            }
          తిరిగి బిడ్డ_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, బిడ్డ_dfd);
      ఒకవేళ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        తిరిగి ret;
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
rmdir_all (స్థిరరాశి అక్షరం *మార్గం)
{
  పూర్ణాంకం ret;
  పూర్ణాంకం dfd = open (మార్గం, O_DIRECTORY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (dfd < 0))
    తిరిగి dfd;

  ret = rmdir_all_fd (dfd);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి rmdir (మార్గం);
}

స్థిర పూర్ణాంకం
read_pids_from_మార్గం (స్థిరరాశి అక్షరం *మార్గం, bool recurse, bool దారాలు, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
  cleanup_close పూర్ణాంకం dirfd = -1;
  size_t n_pids, allocated;
  పూర్ణాంకం బహుళకం;
  పూర్ణాంకం ret;

  ఒకవేళ (మార్గం == ఖాళీ || *మార్గం == '\0')
    తిరిగి 0;

  బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (బహుళకం < 0))
    తిరిగి బహుళకం;

  ఎంపిక (బహుళకం)
    {
    స్థితి CGROUP_బహుళకం_UNIFIED:
      ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, మార్గం, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      ఆపు;

    స్థితి CGROUP_బహుళకం_HYBRID:
    స్థితి CGROUP_బహుళకం_LEGACY:
      ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం "/memory", మార్గం, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      ఆపు;

    అప్రమేయ:
      తిరిగి crun_make_error (err, 0, "invalid cgroup mode `%d`", బహుళకం);
    }

  dirfd = open (cgroup_మార్గం, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (dirfd < 0)
    తిరిగి crun_make_error (err, errno, "open `%s`", cgroup_మార్గం);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  ఒకవేళ (దారాలు && బహుళకం == CGROUP_బహుళకం_UNIFIED)
    తిరిగి read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  తిరిగి read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

పూర్ణాంకం
libcrun_cgroup_read_pids_from_మార్గం (స్థిరరాశి అక్షరం *మార్గం, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  తిరిగి read_pids_from_మార్గం (మార్గం, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (అక్షరం *content, అక్షరం **saveptr, అక్షరం **id, అక్షరం **controller_list, అక్షరం **మార్గం)
{
  అక్షరం *it;

  it = strtok_r (content, "\n", saveptr);
  ఒకవేళ (it == ఖాళీ)
    తిరిగి false;

  ఒకవేళ (id)
    *id = it;

  it = strchr (it, ':');
  ఒకవేళ (it == ఖాళీ)
    తిరిగి false;
  *it++ = '\0';

  ఒకవేళ (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  ఒకవేళ (it == ఖాళీ)
    తిరిగి false;
  *it++ = '\0';

  ఒకవేళ (మార్గం)
    *మార్గం = it;

  తిరిగి true;
}

పూర్ణాంకం
destroy_cgroup_మార్గం (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం, libcrun_error_t *err)
{
  bool repeat = true;
  పూర్ణాంకం retry_లెక్క = 0;
  స్థిరరాశి పూర్ణాంకం max_attempts = 500;
  పూర్ణాంకం ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  ఒకవేళ (is_ఖాళీలు_string (మార్గం))
    తిరిగి 0;

  చేయి
    {
      repeat = false;

      ఒకవేళ (బహుళకం == CGROUP_బహుళకం_UNIFIED)
        {
          cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;

          ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, మార్గం, ఖాళీ);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          ret = rmdir (cgroup_మార్గం);
          ఒకవేళ (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_మార్గం);
              ఒకవేళ (ret < 0)
                {
                  ఒకవేళ (retry_లెక్క >= max_attempts)
                    తిరిగి crun_make_error (err, errno, "cannot delete path `%s`", cgroup_మార్గం);

                  retry_లెక్క++;
                  repeat = true;
                }
            }
        }
      లేకపోతే
        {
          cleanup_free అక్షరం *content = ఖాళీ;
          size_t content_కొలత;
          అక్షరం *controller;
          అక్షరం *saveptr;
          bool has_దత్తాంశం;

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
              cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
              అక్షరం *subsystem;
              ఒకవేళ (has_ఉపసర్గ (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              ఒకవేళ (బహుళకం == CGROUP_బహుళకం_LEGACY && strcmp (subsystem, "unified") == 0)
                కొనసాగు;

              ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, subsystem, మార్గం, ఖాళీ);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;

              ret = rmdir (cgroup_మార్గం);
              ఒకవేళ (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_మార్గం);
                  ఒకవేళ (ret < 0)
                    {
                      ఒకవేళ (retry_లెక్క >= max_attempts)
                        తిరిగి crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_మార్గం);
                      retry_లెక్క++;
                      repeat = true;
                    }
                }
            }
        }

      ఒకవేళ (repeat)
        {
          నిర్మాణం timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, ఖాళీ);

          ret = cgroup_killall_మార్గం (మార్గం, SIGKILL, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } అయితే (repeat);

  తిరిగి 0;
}

పూర్ణాంకం
chown_cgroups (స్థిరరాశి అక్షరం *మార్గం, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *delegate = ఖాళీ;
  cleanup_close పూర్ణాంకం dfd = -1;
  size_t delegate_కొలత;
  అక్షరం *saveptr = ఖాళీ;
  అక్షరం *name;
  పూర్ణాంకం ret;

  ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, మార్గం, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  dfd = open (cgroup_మార్గం, O_CLOEXEC | O_మార్గం);
  ఒకవేళ (UNLIKELY (dfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", cgroup_మార్గం);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_కొలత, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          తిరిగి 0;
        }
      తిరిగి ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_ఖాళీలు_మార్గం | AT_SYMLINK_NOFOLLOW);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "cannot chown `%s`", cgroup_మార్గం);

  కొరకు (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (ఖాళీ, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (errno == ENOENT)
            కొనసాగు;

          తిరిగి crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_మార్గం, name);
        }
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libcrun_cgroup_pause_unpause_with_బహుళకం (స్థిరరాశి అక్షరం *cgroup_మార్గం, పూర్ణాంకం cgroup_బహుళకం, స్థిరరాశి bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  స్థిరరాశి అక్షరం *state = "";
  పూర్ణాంకం ret;

  ఒకవేళ (cgroup_మార్గం == ఖాళీ || cgroup_మార్గం[0] == '\0')
    తిరిగి crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&మార్గం, err, CGROUP_మూలం, cgroup_మార్గం, "cgroup.freeze", ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&మార్గం, err, CGROUP_మూలం "/freezer", cgroup_మార్గం, "freezer.state", ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = write_cgroup_file (AT_FDCWD, మార్గం, state, అక్షరశ్రేణి_పొడవు (state), err);
  ఒకవేళ (ret >= 0)
    తిరిగి 0;
  తిరిగి ret;
}

పూర్ణాంకం
libcrun_cgroup_pause_unpause_మార్గం (స్థిరరాశి అక్షరం *cgroup_మార్గం, స్థిరరాశి bool pause, libcrun_error_t *err)
{
  పూర్ణాంకం cgroup_బహుళకం;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  తిరిగి libcrun_cgroup_pause_unpause_with_బహుళకం (cgroup_మార్గం, cgroup_బహుళకం, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
పూర్ణాంకం
libcrun_cgroup_ensure_not_frozen (స్థిరరాశి అక్షరం *cgroup_మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *content = ఖాళీ;
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  స్థిరరాశి అక్షరం *frozen;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం ret;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&మార్గం, err, CGROUP_మూలం, cgroup_మార్గం, "cgroup.freeze", ఖాళీ);
    }
  లేకపోతే
    {
      frozen = "FROZEN";
      ret = append_paths (&మార్గం, err, CGROUP_మూలం "/freezer", cgroup_మార్గం, "freezer.state", ఖాళీ);
    }
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = read_all_file (మార్గం, &content, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      ఒకవేళ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          తిరిగి 0;
        }
      తిరిగి ret;
    }

  ఒకవేళ (అక్షరశ్రేణి_శోధన (content, frozen))
    తిరిగి crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  తిరిగి 0;
}

#నిర్వచించు CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
స్థిర పూర్ణాంకం
watch_cgroup_events (స్థిరరాశి అక్షరం *మార్గం)
{
  cleanup_free అక్షరం *events_file = ఖాళీ;
  cleanup_close పూర్ణాంకం fd = -1;
  libcrun_error_t tmp_err = ఖాళీ;
  పూర్ణాంకం ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_మూలం, మార్గం, "cgroup.events", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      తిరిగి -1;
    }

  fd = inotify_init1 (లోపల_CLOEXEC | లోపల_NONBLOCK);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి -1;

  ret = inotify_కూడు_watch (fd, events_file, లోపల_MODIFY);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి -1;

  ret = fd;
  fd = -1;
  తిరిగి ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
స్థిర పూర్ణాంకం
wait_cgroup_unpopulated (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం inotify_fd)
{
  cleanup_free అక్షరం *events_file = ఖాళీ;
  libcrun_error_t tmp_err = ఖాళీ;
  నిర్మాణం timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  పూర్ణాంకం ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_మూలం, మార్గం, "cgroup.events", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      తిరిగి 0;
    }

  కొరకు (;;)
    {
      cleanup_free అక్షరం *content = ఖాళీ;
      అక్షరం buf[4096];
      నిర్మాణం pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, ఖాళీ, &tmp_err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          తిరిగి 0;
        }

      ఒకవేళ (అక్షరశ్రేణి_శోధన (content, "populated 0"))
        తిరిగి 1;

      ret = తాత్కాలిక_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, ఖాళీ));
      ఒకవేళ (ret <= 0)
        తిరిగి 0;

      అయితే (పఠ (inotify_fd, buf, పరిమాణం (buf)) > 0)
        ;
    }
}

పూర్ణాంకం
cgroup_killall_మార్గం (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం సంకేతం, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close పూర్ణాంకం inotify_fd = -1;
  పూర్ణాంకం ret;
  size_t i;
  cleanup_free pid_t *pids = ఖాళీ;

  ఒకవేళ (మార్గం == ఖాళీ || *మార్గం == '\0')
    తిరిగి 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  ఒకవేళ (సంకేతం == SIGKILL)
    {
      cleanup_free అక్షరం *kill_file = ఖాళీ;

      ret = append_paths (&kill_file, err, CGROUP_మూలం, మార్గం, "cgroup.kill", ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      inotify_fd = watch_cgroup_events (మార్గం);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      ఒకవేళ (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (మార్గం, inotify_fd))
        తిరిగి 0;

      ఒకవేళ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_మార్గం (మార్గం, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    crun_error_release (err);
  లేకపోతే
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_మార్గం (మార్గం, true, సంకేతం == SIGKILL, &pids, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (crun_error_get_errno (err) != ENOENT)
        వెళ్ళు thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  కొరకు (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], సంకేతం);
      ఒకవేళ (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          వెళ్ళు thaw;
        }
    }

  ret = 0;

thaw:
  ఒకవేళ (frozen)
    {
      libcrun_error_t thaw_err = ఖాళీ;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      ఒకవేళ (libcrun_cgroup_pause_unpause_మార్గం (మార్గం, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  తిరిగి ret;
}

స్థిర పూర్ణాంకం
read_అందుబాటు_controllers (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *controllers = ఖాళీ;
  cleanup_free అక్షరం *buf = ఖాళీ;
  అక్షరం *saveptr = ఖాళీ;
  స్థిరరాశి అక్షరం *token;
  పూర్ణాంకం అందుబాటు = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_మూలం, మార్గం, "cgroup.controllers", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = read_all_file (controllers, &buf, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  కొరకు (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (ఖాళీ, " \n", &saveptr))
    {
      ఒకవేళ (strcmp (token, "memory") == 0)
        అందుబాటు |= CGROUP_జ్ఞాపకం;
      లేకపోతే ఒకవేళ (strcmp (token, "cpu") == 0)
        అందుబాటు |= CGROUP_CPU;
      లేకపోతే ఒకవేళ (strcmp (token, "cpuset") == 0)
        అందుబాటు |= CGROUP_CPUSET;
      లేకపోతే ఒకవేళ (strcmp (token, "hugetlb") == 0)
        అందుబాటు |= CGROUP_HUGETLB;
      లేకపోతే ఒకవేళ (strcmp (token, "pids") == 0)
        అందుబాటు |= CGROUP_PIDS;
      లేకపోతే ఒకవేళ (strcmp (token, "io") == 0)
        అందుబాటు |= CGROUP_IO;
      లేకపోతే ఒకవేళ (strcmp (token, "misc") == 0)
        అందుబాటు |= CGROUP_MISC;
    }
  తిరిగి అందుబాటు;
}

స్థిర పూర్ణాంకం
write_controller_file (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం controllers_గమ్యం_enable, libcrun_error_t *err)
{
  cleanup_free అక్షరం *subtree_control = ఖాళీ;
  cleanup_free అక్షరం *controllers = ఖాళీ;
  size_t controllers_len = 0;
  పూర్ణాంకం ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_గమ్యం_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_గమ్యం_enable & CGROUP_IO) ? "+io" : "", (controllers_గమ్యం_enable & CGROUP_జ్ఞాపకం) ? "+memory" : "",
      (controllers_గమ్యం_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_గమ్యం_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_గమ్యం_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_గమ్యం_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_మూలం, మార్గం, "cgroup.subtree_control", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      cleanup_free అక్షరం *controllers_copy = xmalloc (controllers_len + 1);
      పూర్ణాంకం attempts_ఎడమ;
      అక్షరం *saveptr = ఖాళీ;
      స్థిరరాశి అక్షరం *token;
      పూర్ణాంకం e;

      e = crun_error_get_errno (err);
      ఒకవేళ (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        తిరిగి crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      ఒకవేళ (e == ENOENT)
        {
          libcrun_error_t tmp_err = ఖాళీ;
          పూర్ణాంకం exists;

          exists = crun_మార్గం_exists (subtree_control, &tmp_err);
          ఒకవేళ (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              తిరిగి ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          ఒకవేళ (exists == 0)
            తిరిగి ret;
        }

      crun_error_release (err);

      ఒకవేళ (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (మార్గం, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      కొరకు (attempts_ఎడమ = 1000; attempts_ఎడమ >= 0; attempts_ఎడమ--)
        {
          పూర్ణాంకం controllers_written;
          bool repeat = false;

          జ్ఞాపకం_నకలు (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          కొరకు (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (ఖాళీ, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, అక్షరశ్రేణి_పొడవు (token), err);
              ఒకవేళ (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  ఒకవేళ (e == EBUSY)
                    repeat = true;

                  కొనసాగు;
                }

              controllers_written++;
            }

          ఒకవేళ (! repeat)
            ఆపు;

          /* If there was any controller written, just try once more without any delay.  */
          ఒకవేళ (controllers_written > 0 && attempts_ఎడమ > 2)
            {
              attempts_ఎడమ = 1;
              కొనసాగు;
            }

          ఒకవేళ (attempts_ఎడమ > 0)
            {
              నిర్మాణం timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, ఖాళీ);
            }
        }

      /* Refresh what controllers are available.  */
      తిరిగి read_అందుబాటు_controllers (మార్గం, err);
    }

  /* All controllers were enabled successfully.  */
  తిరిగి controllers_గమ్యం_enable;
}

పూర్ణాంకం
enable_controllers (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *tmp_మార్గం = ఖాళీ;
  అక్షరం *it;
  పూర్ణాంకం ret, controllers_గమ్యం_enable;

  xasprintf (&tmp_మార్గం, "%s/", మార్గం);

  ret = read_అందుబాటు_controllers ("", err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  controllers_గమ్యం_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_గమ్యం_enable, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      ఒకవేళ ((controllers_గమ్యం_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_గమ్యం_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_గమ్యం_enable, err);
        }
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  కొరకు (it = strchr (tmp_మార్గం + 1, '/'); it;)
    {
      cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
      అక్షరం *తదుపరి_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, tmp_మార్గం, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = mkdir (cgroup_మార్గం, 0755);
      ఒకవేళ (UNLIKELY (ret < 0 && errno != EEXIST))
        తిరిగి crun_make_error (err, errno, "create `%s`", cgroup_మార్గం);

      ఒకవేళ (తదుపరి_slash)
        {
          ret = write_controller_file (tmp_మార్గం, controllers_గమ్యం_enable, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      *it = '/';
      it = తదుపరి_slash;
    }
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_move_process_గమ్యం_cgroup (pid_t pid, pid_t init_pid, స్థిరరాశి అక్షరం *మార్గం, bool create_if_missing, libcrun_error_t *err)
{
  పూర్ణాంకం cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (మార్గం == ఖాళీ || *మార్గం == '\0')
    తిరిగి 0;

  తిరిగి enter_cgroup (cgroup_బహుళకం, pid, init_pid, మార్గం, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
పూర్ణాంకం
libcrun_move_self_గమ్యం_cgroups (స్థిరరాశి అక్షరం *content, libcrun_error_t *err)
{
  cleanup_free అక్షరం *buf = xstrdup (content);
  అక్షరం *saveptr = ఖాళీ;
  అక్షరం *controller;
  అక్షరం *మార్గం;
  పూర్ణాంకం cgroup_బహుళకం;
  bool has_దత్తాంశం;
  పూర్ణాంకం ret;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  కొరకు (has_దత్తాంశం = read_proc_cgroup (buf, &saveptr, ఖాళీ, &controller, &మార్గం);
       has_దత్తాంశం;
       has_దత్తాంశం = read_proc_cgroup (ఖాళీ, &saveptr, ఖాళీ, &controller, &మార్గం))
    {
      స్థిరరాశి అక్షరం *subsystem = ఖాళీ;

      ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          ఒకవేళ (controller[0] != '\0')
            కొనసాగు;
        }
      లేకపోతే
        {
          cleanup_free అక్షరం *subsystem_మార్గం = ఖాళీ;

          /* Use the same names as enter_cgroup_v1 does.  */
          ఒకవేళ (has_ఉపసర్గ (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          ఒకవేళ (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          ఒకవేళ (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_మార్గం, err, CGROUP_మూలం, subsystem, ఖాళీ);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          ret = crun_మార్గం_exists (subsystem_మార్గం, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          ఒకవేళ (ret == 0)
            కొనసాగు;
        }

      ret = move_process_గమ్యం_cgroup (0, subsystem, మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_get_cgroup_dirfd (నిర్మాణం libcrun_cgroup_స్థితి *స్థితి_2, స్థిరరాశి అక్షరం *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free అక్షరం *మార్గం_గమ్యం_cgroup = ఖాళీ;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం cgroupdirfd;
  పూర్ణాంకం ret;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (cgroup_బహుళకం < 0)
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (cgroup_బహుళకం != CGROUP_బహుళకం_UNIFIED)
    తిరిగి crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  ఒకవేళ (స్థితి_2 == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "internal error");

  ఒకవేళ (is_ఖాళీలు_string (స్థితి_2->మార్గం))
    తిరిగి crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&మార్గం_గమ్యం_cgroup, err, CGROUP_మూలం, స్థితి_2->మార్గం, sub_cgroup, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  cgroupdirfd = open (మార్గం_గమ్యం_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_మార్గం);
  ఒకవేళ (UNLIKELY (cgroupdirfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", మార్గం_గమ్యం_cgroup);

  తిరిగి cgroupdirfd;
}

పూర్ణాంకం
libcrun_migrate_all_pids_గమ్యం_cgroup (pid_t init_pid, అక్షరం *from, అక్షరం *గమ్యం, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = ఖాళీ;
  cleanup_close పూర్ణాంకం బిడ్డ_dfd = -1;
  పూర్ణాంకం cgroup_బహుళకం;
  size_t from_len;
  size_t i;
  పూర్ణాంకం ret;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (cgroup_బహుళకం < 0)
    తిరిగి cgroup_బహుళకం;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_మార్గం (from, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_cgroup_read_pids_from_మార్గం (from, true, &pids, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు thaw;

  from_len = అక్షరశ్రేణి_పొడవు (from);

  కొరకు (i = 0; pids && pids[i]; i++)
    {
      cleanup_free అక్షరం *pid_మార్గం = ఖాళీ;
      cleanup_free అక్షరం *dest_cgroup = ఖాళీ;

      ret = libcrun_get_cgroup_process (pids[i], &pid_మార్గం, false, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      ఒకవేళ (! has_ఉపసర్గ (pid_మార్గం, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          వెళ్ళు thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", గమ్యం, pid_మార్గం + from_len);

      ret = enter_cgroup (cgroup_బహుళకం, pids[i], init_pid, dest_cgroup, false, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు thaw;
    }

  ret = 0;

thaw:
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = ఖాళీ;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      ఒకవేళ (libcrun_cgroup_pause_unpause_మార్గం (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      తిరిగి ret;
    }

  ret = libcrun_cgroup_pause_unpause_మార్గం (from, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి destroy_cgroup_మార్గం (from, cgroup_బహుళకం, err);
}

పూర్ణాంకం
get_cgroup_dirfd_మార్గం (పూర్ణాంకం dirfd, అక్షరం **మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
  proc_fd_మార్గం_t fd_మార్గం;
  ssize_t len;

  get_proc_self_fd_మార్గం (fd_మార్గం, dirfd);

  len = సురక్షిత_readlinkat (AT_FDCWD, fd_మార్గం, &cgroup_మార్గం, 0, err);
  ఒకవేళ (UNLIKELY (len < 0))
    తిరిగి len;

  ఒకవేళ (has_ఉపసర్గ (cgroup_మార్గం, CGROUP_మూలం))
    {
      *మార్గం = xstrdup (cgroup_మార్గం + అక్షరశ్రేణి_పొడవు (CGROUP_మూలం));
      తిరిగి 0;
    }
  తిరిగి crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_మార్గం);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
పూర్ణాంకం
parse_cpu_max (స్థిరరాశి అక్షరం *విలువ, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *it = విలువ;
  అక్షరం *endptr = ఖాళీ;

  అయితే (*it == ' ')
    it++;

  errno = 0;
  ఒకవేళ (has_ఉపసర్గ (it, "max"))
    {
      *quota = -1;
      endptr = (అక్షరం *) it + 3;
    }
  లేకపోతే
    {
      *quota = strtoll (it, &endptr, 10);
      ఒకవేళ (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        తిరిగి crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", విలువ);
    }

  it = endptr;
  అయితే (*it == ' ')
    it++;

  *has_period = false;
  ఒకవేళ (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      ఒకవేళ (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        తిరిగి crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", విలువ);
      *has_period = true;
    }

  తిరిగి 0;
}
