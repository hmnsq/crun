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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "cgroup.h"
#অন্তর্ভাব "cgroup-internal.h"
#অন্তর্ভাব "cgroup-systemd.h"
#অন্তর্ভাব "cgroup-utils.h"
#অন্তর্ভাব "cgroup-setup.h"
#অন্তর্ভাব "ebpf.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "status.h"
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <sys/vfs.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <poll.h>
#অন্তর্ভাব <sys/inotify.h>

#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <libgen.h>

কাঠামো symlink_s
{
  ধ্রুবক অক্ষর *name;
  ধ্রুবক অক্ষর *target;
};

স্থির কাঠামো symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { ফাঁকা, ফাঁকা } };

পূর্ণসংখ্যা
libcrun_cgroups_create_symlinks (পূর্ণসংখ্যা dirfd, libcrun_error_t *err)
{
  পূর্ণসংখ্যা i;

  জন্য (i = 0; cgroup_symlinks[i].name; i++)
    {
      পূর্ণসংখ্যা ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (errno == ENOENT || errno == EEXIST)
            চালু;
          ফেরত crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  ফেরত 0;
}

পূর্ণসংখ্যা
maybe_make_cgroup_threaded (ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
  cleanup_free অক্ষর *cgroup_path_type = ফাঁকা;
  ধ্রুবক অক্ষর *ধ্রুবক threaded = "threaded";
  cleanup_free অক্ষর *content = ফাঁকা;
  cleanup_free অক্ষর *বাফার = ফাঁকা;
  ধ্রুবক অক্ষর *parent;
  size_t আকার_2;
  অক্ষর *it;
  পূর্ণসংখ্যা ret;

  যদি (path == ফাঁকা)
    ফেরত 0;

  path = consume_slashes (path);

  যদি (path[0] == '\0')
    ফেরত 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_মূল, path, "cgroup.type", ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = read_all_file (cgroup_path_type, &content, &আকার_2, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (আকার_2 > 0)
    {
      it = content + আকার_2 - 1;
      যখন (*it == '\n' && it > content)
        *it-- = '\0';
    }

  যদি (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    ফেরত 0;

  বাফার = xstrdup (path);
  parent = consume_slashes (dirname (বাফার));
  যদি (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      যদি (ret < 0)
        ফেরত ret;
    }
  ফেরত write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

পূর্ণসংখ্যা
move_process_to_cgroup (pid_t pid, ধ্রুবক অক্ষর *subsystem, ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
  cleanup_free অক্ষর *cgroup_path_procs = ফাঁকা;
  অক্ষর pid_str[16];
  পূর্ণসংখ্যা ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_মূল,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = snprintf (pid_str, আকার (pid_str), "%d", pid);
  যদি (UNLIKELY (ret >= (পূর্ণসংখ্যা) আকার (pid_str)))
    ফেরত crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = ফাঁকা;
          পূর্ণসংখ্যা mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          যদি (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              ফেরত ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;

          ফেরত write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      ফেরত ret;
    }
  ফেরত ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

স্থির পূর্ণসংখ্যা
detect_cgroup_mode (libcrun_error_t *err)
{
  কাঠামো statfs stat;
  পূর্ণসংখ্যা ret;

  ret = statfs (CGROUP_মূল, &stat);
  যদি (ret < 0)
    ফেরত crun_make_error (err, errno, "statfs `" CGROUP_মূল "`");
  যদি (stat.f_type == CGROUP2_SUPER_MAGIC)
    ফেরত CGROUP_MODE_UNIFIED;
  যদি (stat.f_type != TMPFS_MAGIC)
    ফেরত crun_make_error (err, 0, "invalid file system type on `" CGROUP_মূল "`");
  ret = statfs (CGROUP_মূল "/unified", &stat);
  যদি (ret < 0 && errno != ENOENT)
    ফেরত crun_make_error (err, errno, "statfs `" CGROUP_মূল "/unified`");
  যদি (ret < 0)
    ফেরত CGROUP_MODE_LEGACY;
  ফেরত stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

পূর্ণসংখ্যা
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  পূর্ণসংখ্যা tmp;
  স্থির পূর্ণসংখ্যা cgroup_mode = 0;

  যদি (cgroup_mode)
    ফেরত cgroup_mode;

  tmp = detect_cgroup_mode (err);
  যদি (UNLIKELY (tmp < 0))
    ফেরত tmp;

  cgroup_mode = tmp;

  ফেরত cgroup_mode;
}

স্থির পূর্ণসংখ্যা
get_cgroup_process_from_content (অক্ষর *content, পূর্ণসংখ্যা cgroup_mode, অক্ষর **path, bool absolute, libcrun_error_t *err)
{
  অক্ষর *cg_path = ফাঁকা;
  অক্ষর *controller;
  অক্ষর *saveptr;
  bool has_তথ্য;

  জন্য (has_তথ্য = read_proc_cgroup (content, &saveptr, ফাঁকা, &controller, &cg_path);
       has_তথ্য;
       has_তথ্য = read_proc_cgroup (ফাঁকা, &saveptr, ফাঁকা, &controller, &cg_path))
    {
      যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          যদি (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            যাও found;
        }
      অন্যথা
        {
          যদি (strcmp (controller, "memory"))
            যাও found;
        }
    }

  ফেরত crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  যদি (absolute)
    ফেরত append_paths (path, err, CGROUP_মূল, cg_path, ফাঁকা);

  *path = xstrdup (cg_path);
  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_get_cgroup_process_at (পূর্ণসংখ্যা dirfd, অক্ষর **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free অক্ষর *content = ফাঁকা;
  size_t content_আকার;
  পূর্ণসংখ্যা cgroup_mode;
  পূর্ণসংখ্যা ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_আকার, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

পূর্ণসংখ্যা
libcrun_get_cgroup_process (pid_t pid, অক্ষর **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free অক্ষর *content = ফাঁকা;
  অক্ষর proc_cgroup_file[64];
  size_t content_আকার;
  পূর্ণসংখ্যা cgroup_mode;
  পূর্ণসংখ্যা ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  যদি (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  অন্যথা
    {
      পূর্ণসংখ্যা len = snprintf (proc_cgroup_file, আকার (proc_cgroup_file), "/proc/%d/cgroup", pid);
      যদি (UNLIKELY (len >= (পূর্ণসংখ্যা) আকার (proc_cgroup_file)))
        ফেরত crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_আকার, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

স্থির পূর্ণসংখ্যা
read_pids_file (পূর্ণসংখ্যা dfd, ধ্রুবক অক্ষর *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা tasksfd = -1;
  cleanup_free অক্ষর *বাফার = ফাঁকা;
  অক্ষর *saveptr = ফাঁকা;
  size_t n_new_pids;
  size_t len;
  অক্ষর *it;
  পূর্ণসংখ্যা ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  যদি (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      যদি (errno == ENOENT || errno == ENODEV)
        ফেরত 0;
      ফেরত crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &বাফার, &len, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (len > 0)
    {
      জন্য (n_new_pids = 0, it = বাফার; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      যদি (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, আকার (pid_t) * *allocated);
        }

      জন্য (it = strtok_r (বাফার, "\n", &saveptr); it; it = strtok_r (ফাঁকা, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, ফাঁকা, 10);

          যদি (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  ফেরত 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
কাঠামো cgroup_walk_dir_s
{
  অক্ষর *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
কাঠামো cgroup_walk_s
{
  পূর্ণসংখ্যা dfd;
  size_t depth;
  কাঠামো cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

স্থির শূন্য
cgroup_walk_বের_করো (কাঠামো cgroup_walk_s *walk)
{
  মুক্ত (walk->dirs[--walk->n_dirs].name);
}

স্থির শূন্য
cleanup_cgroup_walkp (কাঠামো cgroup_walk_s *walk)
{
  পূর্ণসংখ্যা saved_errno = errno;

  যখন (walk->n_dirs > 0)
    cgroup_walk_বের_করো (walk);
  মুক্ত (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
স্থির পূর্ণসংখ্যা
cgroup_walk_enter (কাঠামো cgroup_walk_s *walk, পূর্ণসংখ্যা dfd)
{
  cleanup_close পূর্ণসংখ্যা dfd_clone = -1;
  cleanup_dir DIR *dir = ফাঁকা;
  কাঠামো dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  যদি (UNLIKELY (dfd_clone < 0))
    ফেরত -1;

  dir = fdopendir (dfd_clone);
  যদি (UNLIKELY (dir == ফাঁকা))
    ফেরত -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  জন্য (de = readdir (dir); de; de = readdir (dir))
    {
      কাঠামো cgroup_walk_dir_s *d;

      যদি (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        চালু;

      যদি (de->d_type != DT_DIR)
        চালু;

      যদি (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, আকার (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  ফেরত 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
স্থির পূর্ণসংখ্যা
cgroup_walk_পরবর্তী (কাঠামো cgroup_walk_s *walk, কাঠামো cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : ফাঁকা;

  যখন (*d && walk->depth >= (*d)->depth)
    {
      পূর্ণসংখ্যা parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (parent < 0))
        ফেরত -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
read_pids_cgroup_file (পূর্ণসংখ্যা dfd, ধ্রুবক অক্ষর *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk কাঠামো cgroup_walk_s walk = { .dfd = -1 };
  পূর্ণসংখ্যা dfd_clone;
  পূর্ণসংখ্যা ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (! recurse)
    ফেরত 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (dfd_clone < 0))
    ফেরত crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "open cgroup sub-directory");

  জন্য (;;)
    {
      কাঠামো cgroup_walk_dir_s *d;
      cleanup_close পূর্ণসংখ্যা nfd = -1;

      ret = cgroup_walk_পরবর্তী (&walk, &d);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "open parent cgroup directory");
      যদি (d == ফাঁকা)
        বিরতি;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (nfd < 0))
        {
          পূর্ণসংখ্যা saved_errno = errno;

          যদি (saved_errno == ENOENT)
            {
              cgroup_walk_বের_করো (&walk);
              চালু;
            }
          ফেরত crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_বের_করো (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      যদি (UNLIKELY (ret < 0))
        {
          পূর্ণসংখ্যা e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          যদি (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            ফেরত ret;
          crun_error_release (err);
          চালু;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      যদি (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ফেরত crun_make_error (err, errno, "open cgroup sub-directory");
    }

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
read_pids_cgroup (পূর্ণসংখ্যা dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  যদি (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  ফেরত ret;
}

স্থির পূর্ণসংখ্যা
rmdir_all_fd (পূর্ণসংখ্যা dfd)
{
  cleanup_cgroup_walk কাঠামো cgroup_walk_s walk = { .dfd = -1 };
  পূর্ণসংখ্যা ret;

  ret = cgroup_walk_enter (&walk, dfd);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  জন্য (;;)
    {
      কাঠামো cgroup_walk_dir_s *d;
      পূর্ণসংখ্যা child_dfd;

      ret = cgroup_walk_পরবর্তী (&walk, &d);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
      যদি (d == ফাঁকা)
        বিরতি;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      যদি (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_বের_করো (&walk);
          চালু;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      যদি (UNLIKELY (child_dfd < 0))
        {
          যদি (errno == ENOENT)
            {
              cgroup_walk_বের_করো (&walk);
              চালু;
            }
          ফেরত child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      যদি (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ফেরত ret;
    }

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
rmdir_all (ধ্রুবক অক্ষর *path)
{
  পূর্ণসংখ্যা ret;
  পূর্ণসংখ্যা dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  যদি (UNLIKELY (dfd < 0))
    ফেরত dfd;

  ret = rmdir_all_fd (dfd);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত rmdir (path);
}

স্থির পূর্ণসংখ্যা
read_pids_from_path (ধ্রুবক অক্ষর *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free অক্ষর *cgroup_path = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা dirfd = -1;
  size_t n_pids, allocated;
  পূর্ণসংখ্যা mode;
  পূর্ণসংখ্যা ret;

  যদি (path == ফাঁকা || *path == '\0')
    ফেরত 0;

  mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (mode < 0))
    ফেরত mode;

  নির্বাচন (mode)
    {
    ক্ষেত্র CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_মূল, path, ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
      বিরতি;

    ক্ষেত্র CGROUP_MODE_HYBRID:
    ক্ষেত্র CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_মূল "/memory", path, ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
      বিরতি;

    পূর্বনির্ধারিত:
      ফেরত crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (dirfd < 0)
    ফেরত crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  যদি (threads && mode == CGROUP_MODE_UNIFIED)
    ফেরত read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  ফেরত read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

পূর্ণসংখ্যা
libcrun_cgroup_read_pids_from_path (ধ্রুবক অক্ষর *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ফেরত read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (অক্ষর *content, অক্ষর **saveptr, অক্ষর **id, অক্ষর **controller_list, অক্ষর **path)
{
  অক্ষর *it;

  it = strtok_r (content, "\n", saveptr);
  যদি (it == ফাঁকা)
    ফেরত false;

  যদি (id)
    *id = it;

  it = strchr (it, ':');
  যদি (it == ফাঁকা)
    ফেরত false;
  *it++ = '\0';

  যদি (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  যদি (it == ফাঁকা)
    ফেরত false;
  *it++ = '\0';

  যদি (path)
    *path = it;

  ফেরত true;
}

পূর্ণসংখ্যা
destroy_cgroup_path (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode, libcrun_error_t *err)
{
  bool repeat = true;
  পূর্ণসংখ্যা retry_গুনতি = 0;
  ধ্রুবক পূর্ণসংখ্যা max_attempts = 500;
  পূর্ণসংখ্যা ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  যদি (is_empty_string (path))
    ফেরত 0;

  করো
    {
      repeat = false;

      যদি (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free অক্ষর *cgroup_path = ফাঁকা;

          ret = append_paths (&cgroup_path, err, CGROUP_মূল, path, ফাঁকা);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          ret = rmdir (cgroup_path);
          যদি (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              যদি (ret < 0)
                {
                  যদি (retry_গুনতি >= max_attempts)
                    ফেরত crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_গুনতি++;
                  repeat = true;
                }
            }
        }
      অন্যথা
        {
          cleanup_free অক্ষর *content = ফাঁকা;
          size_t content_আকার;
          অক্ষর *controller;
          অক্ষর *saveptr;
          bool has_তথ্য;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_আকার, err);
          যদি (UNLIKELY (ret < 0))
            {
              যদি (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  ফেরত 0;
                }
              ফেরত ret;
            }

          জন্য (has_তথ্য = read_proc_cgroup (content, &saveptr, ফাঁকা, &controller, ফাঁকা);
               has_তথ্য;
               has_তথ্য = read_proc_cgroup (ফাঁকা, &saveptr, ফাঁকা, &controller, ফাঁকা))
            {
              cleanup_free অক্ষর *cgroup_path = ফাঁকা;
              অক্ষর *subsystem;
              যদি (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              যদি (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                চালু;

              ret = append_paths (&cgroup_path, err, CGROUP_মূল, subsystem, path, ফাঁকা);
              যদি (UNLIKELY (ret < 0))
                ফেরত ret;

              ret = rmdir (cgroup_path);
              যদি (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  যদি (ret < 0)
                    {
                      যদি (retry_গুনতি >= max_attempts)
                        ফেরত crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_গুনতি++;
                      repeat = true;
                    }
                }
            }
        }

      যদি (repeat)
        {
          কাঠামো timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, ফাঁকা);

          ret = cgroup_killall_path (path, SIGKILL, err);
          যদি (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } যখন (repeat);

  ফেরত 0;
}

পূর্ণসংখ্যা
chown_cgroups (ধ্রুবক অক্ষর *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free অক্ষর *cgroup_path = ফাঁকা;
  cleanup_free অক্ষর *delegate = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা dfd = -1;
  size_t delegate_আকার;
  অক্ষর *saveptr = ফাঁকা;
  অক্ষর *name;
  পূর্ণসংখ্যা ret;

  ret = append_paths (&cgroup_path, err, CGROUP_মূল, path, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  যদি (UNLIKELY (dfd < 0))
    ফেরত crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_আকার, err);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ফেরত 0;
        }
      ফেরত ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  জন্য (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (ফাঁকা, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (errno == ENOENT)
            চালু;

          ফেরত crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libcrun_cgroup_pause_unpause_with_mode (ধ্রুবক অক্ষর *cgroup_path, পূর্ণসংখ্যা cgroup_mode, ধ্রুবক bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free অক্ষর *path = ফাঁকা;
  ধ্রুবক অক্ষর *state = "";
  পূর্ণসংখ্যা ret;

  যদি (cgroup_path == ফাঁকা || cgroup_path[0] == '\0')
    ফেরত crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_মূল, cgroup_path, "cgroup.freeze", ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }
  অন্যথা
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_মূল "/freezer", cgroup_path, "freezer.state", ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  যদি (ret >= 0)
    ফেরত 0;
  ফেরত ret;
}

পূর্ণসংখ্যা
libcrun_cgroup_pause_unpause_path (ধ্রুবক অক্ষর *cgroup_path, ধ্রুবক bool pause, libcrun_error_t *err)
{
  পূর্ণসংখ্যা cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  ফেরত libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
পূর্ণসংখ্যা
libcrun_cgroup_ensure_not_frozen (ধ্রুবক অক্ষর *cgroup_path, libcrun_error_t *err)
{
  cleanup_free অক্ষর *content = ফাঁকা;
  cleanup_free অক্ষর *path = ফাঁকা;
  ধ্রুবক অক্ষর *frozen;
  পূর্ণসংখ্যা cgroup_mode;
  পূর্ণসংখ্যা ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_মূল, cgroup_path, "cgroup.freeze", ফাঁকা);
    }
  অন্যথা
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_মূল "/freezer", cgroup_path, "freezer.state", ফাঁকা);
    }
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = read_all_file (path, &content, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      যদি (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ফেরত 0;
        }
      ফেরত ret;
    }

  যদি (strstr (content, frozen))
    ফেরত crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  ফেরত 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
স্থির পূর্ণসংখ্যা
watch_cgroup_events (ধ্রুবক অক্ষর *path)
{
  cleanup_free অক্ষর *events_file = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা fd = -1;
  libcrun_error_t tmp_err = ফাঁকা;
  পূর্ণসংখ্যা ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_মূল, path, "cgroup.events", ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ফেরত -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  যদি (UNLIKELY (fd < 0))
    ফেরত -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  যদি (UNLIKELY (ret < 0))
    ফেরত -1;

  ret = fd;
  fd = -1;
  ফেরত ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
স্থির পূর্ণসংখ্যা
wait_cgroup_unpopulated (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা inotify_fd)
{
  cleanup_free অক্ষর *events_file = ফাঁকা;
  libcrun_error_t tmp_err = ফাঁকা;
  কাঠামো timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  পূর্ণসংখ্যা ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_মূল, path, "cgroup.events", ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ফেরত 0;
    }

  জন্য (;;)
    {
      cleanup_free অক্ষর *content = ফাঁকা;
      অক্ষর buf[4096];
      কাঠামো pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, ফাঁকা, &tmp_err);
      যদি (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          ফেরত 0;
        }

      যদি (strstr (content, "populated 0"))
        ফেরত 1;

      ret = অস্থায়ী_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, ফাঁকা));
      যদি (ret <= 0)
        ফেরত 0;

      যখন (পঠ (inotify_fd, buf, আকার (buf)) > 0)
        ;
    }
}

পূর্ণসংখ্যা
cgroup_killall_path (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা সংকেত, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close পূর্ণসংখ্যা inotify_fd = -1;
  পূর্ণসংখ্যা ret;
  size_t i;
  cleanup_free pid_t *pids = ফাঁকা;

  যদি (path == ফাঁকা || *path == '\0')
    ফেরত 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  যদি (সংকেত == SIGKILL)
    {
      cleanup_free অক্ষর *kill_file = ফাঁকা;

      ret = append_paths (&kill_file, err, CGROUP_মূল, path, "cgroup.kill", ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      যদি (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        ফেরত 0;

      যদি (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  যদি (UNLIKELY (ret < 0))
    crun_error_release (err);
  অন্যথা
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, সংকেত == SIGKILL, &pids, err);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (crun_error_get_errno (err) != ENOENT)
        যাও thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  জন্য (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], সংকেত);
      যদি (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          যাও thaw;
        }
    }

  ret = 0;

thaw:
  যদি (frozen)
    {
      libcrun_error_t thaw_err = ফাঁকা;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      যদি (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  ফেরত ret;
}

স্থির পূর্ণসংখ্যা
read_available_controllers (ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
  cleanup_free অক্ষর *controllers = ফাঁকা;
  cleanup_free অক্ষর *buf = ফাঁকা;
  অক্ষর *saveptr = ফাঁকা;
  ধ্রুবক অক্ষর *token;
  পূর্ণসংখ্যা available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_মূল, path, "cgroup.controllers", ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = read_all_file (controllers, &buf, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  জন্য (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (ফাঁকা, " \n", &saveptr))
    {
      যদি (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      অন্যথা যদি (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      অন্যথা যদি (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      অন্যথা যদি (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      অন্যথা যদি (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      অন্যথা যদি (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      অন্যথা যদি (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  ফেরত available;
}

স্থির পূর্ণসংখ্যা
write_controller_file (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free অক্ষর *subtree_control = ফাঁকা;
  cleanup_free অক্ষর *controllers = ফাঁকা;
  size_t controllers_len = 0;
  পূর্ণসংখ্যা ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_মূল, path, "cgroup.subtree_control", ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  যদি (UNLIKELY (ret < 0))
    {
      cleanup_free অক্ষর *controllers_copy = xmalloc (controllers_len + 1);
      পূর্ণসংখ্যা attempts_বাম;
      অক্ষর *saveptr = ফাঁকা;
      ধ্রুবক অক্ষর *token;
      পূর্ণসংখ্যা e;

      e = crun_error_get_errno (err);
      যদি (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        ফেরত crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      যদি (e == ENOENT)
        {
          libcrun_error_t tmp_err = ফাঁকা;
          পূর্ণসংখ্যা exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          যদি (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              ফেরত ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          যদি (exists == 0)
            ফেরত ret;
        }

      crun_error_release (err);

      যদি (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      জন্য (attempts_বাম = 1000; attempts_বাম >= 0; attempts_বাম--)
        {
          পূর্ণসংখ্যা controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          জন্য (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (ফাঁকা, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              যদি (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  যদি (e == EBUSY)
                    repeat = true;

                  চালু;
                }

              controllers_written++;
            }

          যদি (! repeat)
            বিরতি;

          /* If there was any controller written, just try once more without any delay.  */
          যদি (controllers_written > 0 && attempts_বাম > 2)
            {
              attempts_বাম = 1;
              চালু;
            }

          যদি (attempts_বাম > 0)
            {
              কাঠামো timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, ফাঁকা);
            }
        }

      /* Refresh what controllers are available.  */
      ফেরত read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  ফেরত controllers_to_enable;
}

পূর্ণসংখ্যা
enable_controllers (ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
  cleanup_free অক্ষর *tmp_path = ফাঁকা;
  অক্ষর *it;
  পূর্ণসংখ্যা ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  যদি (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      যদি ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  জন্য (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free অক্ষর *cgroup_path = ফাঁকা;
      অক্ষর *পরবর্তী_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_মূল, tmp_path, ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      ret = mkdir (cgroup_path, 0755);
      যদি (UNLIKELY (ret < 0 && errno != EEXIST))
        ফেরত crun_make_error (err, errno, "create `%s`", cgroup_path);

      যদি (পরবর্তী_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
        }

      *it = '/';
      it = পরবর্তী_slash;
    }
  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ধ্রুবক অক্ষর *path, bool create_if_missing, libcrun_error_t *err)
{
  পূর্ণসংখ্যা cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  যদি (path == ফাঁকা || *path == '\0')
    ফেরত 0;

  ফেরত enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
পূর্ণসংখ্যা
libcrun_move_self_to_cgroups (ধ্রুবক অক্ষর *content, libcrun_error_t *err)
{
  cleanup_free অক্ষর *buf = xstrdup (content);
  অক্ষর *saveptr = ফাঁকা;
  অক্ষর *controller;
  অক্ষর *path;
  পূর্ণসংখ্যা cgroup_mode;
  bool has_তথ্য;
  পূর্ণসংখ্যা ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  জন্য (has_তথ্য = read_proc_cgroup (buf, &saveptr, ফাঁকা, &controller, &path);
       has_তথ্য;
       has_তথ্য = read_proc_cgroup (ফাঁকা, &saveptr, ফাঁকা, &controller, &path))
    {
      ধ্রুবক অক্ষর *subsystem = ফাঁকা;

      যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          যদি (controller[0] != '\0')
            চালু;
        }
      অন্যথা
        {
          cleanup_free অক্ষর *subsystem_path = ফাঁকা;

          /* Use the same names as enter_cgroup_v1 does.  */
          যদি (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          যদি (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          যদি (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_মূল, subsystem, ফাঁকা);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          ret = crun_path_exists (subsystem_path, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          যদি (ret == 0)
            চালু;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_get_cgroup_dirfd (কাঠামো libcrun_cgroup_status *status, ধ্রুবক অক্ষর *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free অক্ষর *path_to_cgroup = ফাঁকা;
  পূর্ণসংখ্যা cgroup_mode;
  পূর্ণসংখ্যা cgroupdirfd;
  পূর্ণসংখ্যা ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (cgroup_mode < 0)
    ফেরত cgroup_mode;

  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    ফেরত crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  যদি (status == ফাঁকা)
    ফেরত crun_make_error (err, 0, "internal error");

  যদি (is_empty_string (status->path))
    ফেরত crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_মূল, status->path, sub_cgroup, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  যদি (UNLIKELY (cgroupdirfd < 0))
    ফেরত crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  ফেরত cgroupdirfd;
}

পূর্ণসংখ্যা
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, অক্ষর *from, অক্ষর *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা child_dfd = -1;
  পূর্ণসংখ্যা cgroup_mode;
  size_t from_len;
  size_t i;
  পূর্ণসংখ্যা ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (cgroup_mode < 0)
    ফেরত cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  যদি (UNLIKELY (ret < 0))
    যাও thaw;

  from_len = strlen (from);

  জন্য (i = 0; pids && pids[i]; i++)
    {
      cleanup_free অক্ষর *pid_path = ফাঁকা;
      cleanup_free অক্ষর *dest_cgroup = ফাঁকা;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      যদি (UNLIKELY (ret < 0))
        যাও thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      যদি (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          যাও thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      যদি (UNLIKELY (ret < 0))
        যাও thaw;
    }

  ret = 0;

thaw:
  যদি (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = ফাঁকা;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      যদি (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      ফেরত ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত destroy_cgroup_path (from, cgroup_mode, err);
}

পূর্ণসংখ্যা
get_cgroup_dirfd_path (পূর্ণসংখ্যা dirfd, অক্ষর **path, libcrun_error_t *err)
{
  cleanup_free অক্ষর *cgroup_path = ফাঁকা;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  যদি (UNLIKELY (len < 0))
    ফেরত len;

  যদি (has_prefix (cgroup_path, CGROUP_মূল))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_মূল));
      ফেরত 0;
    }
  ফেরত crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
পূর্ণসংখ্যা
parse_cpu_max (ধ্রুবক অক্ষর *মান, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ধ্রুবক অক্ষর *it = মান;
  অক্ষর *endptr = ফাঁকা;

  যখন (*it == ' ')
    it++;

  errno = 0;
  যদি (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (অক্ষর *) it + 3;
    }
  অন্যথা
    {
      *quota = strtoll (it, &endptr, 10);
      যদি (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        ফেরত crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", মান);
    }

  it = endptr;
  যখন (*it == ' ')
    it++;

  *has_period = false;
  যদি (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      যদি (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        ফেরত crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", মান);
      *has_period = true;
    }

  ফেরত 0;
}
