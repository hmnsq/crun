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

সংরচনা symlink_s
{
  ধ্রুব বর্ণ *name;
  ধ্রুব বর্ণ *target;
};

স্থির সংরচনা symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { রিক্ত, রিক্ত } };

পূর্ণাঙ্ক
libcrun_cgroups_create_symlinks (পূর্ণাঙ্ক dirfd, libcrun_error_t *err)
{
  পূর্ণাঙ্ক i;

  প্রতি (i = 0; cgroup_symlinks[i].name; i++)
    {
      পূর্ণাঙ্ক ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (errno == ENOENT || errno == EEXIST)
            অনুবর্ত;
          প্রত্যাগম crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
maybe_make_cgroup_threaded (ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  cleanup_free বর্ণ *cgroup_path_type = রিক্ত;
  ধ্রুব বর্ণ *ধ্রুব threaded = "threaded";
  cleanup_free বর্ণ *content = রিক্ত;
  cleanup_free বর্ণ *buffer = রিক্ত;
  ধ্রুব বর্ণ *parent;
  size_t size;
  বর্ণ *it;
  পূর্ণাঙ্ক ret;

  যদি (path == রিক্ত)
    প্রত্যাগম 0;

  path = consume_slashes (path);

  যদি (path[0] == '\0')
    প্রত্যাগম 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_মূল, path, "cgroup.type", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (size > 0)
    {
      it = content + size - 1;
      যাবত্ (*it == '\n' && it > content)
        *it-- = '\0';
    }

  যদি (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    প্রত্যাগম 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  যদি (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      যদি (ret < 0)
        প্রত্যাগম ret;
    }
  প্রত্যাগম write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

পূর্ণাঙ্ক
move_process_to_cgroup (pid_t pid, ধ্রুব বর্ণ *subsystem, ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  cleanup_free বর্ণ *cgroup_path_procs = রিক্ত;
  বর্ণ pid_str[16];
  পূর্ণাঙ্ক ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_মূল,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = snprintf (pid_str, পরিমাণ (pid_str), "%d", pid);
  যদি (UNLIKELY (ret >= (পূর্ণাঙ্ক) পরিমাণ (pid_str)))
    প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = রিক্ত;
          পূর্ণাঙ্ক mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          যদি (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              প্রত্যাগম ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;

          প্রত্যাগম write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      প্রত্যাগম ret;
    }
  প্রত্যাগম ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

স্থির পূর্ণাঙ্ক
detect_cgroup_mode (libcrun_error_t *err)
{
  সংরচনা statfs stat;
  পূর্ণাঙ্ক ret;

  ret = statfs (CGROUP_মূল, &stat);
  যদি (ret < 0)
    প্রত্যাগম crun_make_error (err, errno, "statfs `" CGROUP_মূল "`");
  যদি (stat.f_type == CGROUP2_SUPER_MAGIC)
    প্রত্যাগম CGROUP_MODE_UNIFIED;
  যদি (stat.f_type != TMPFS_MAGIC)
    প্রত্যাগম crun_make_error (err, 0, "invalid file system type on `" CGROUP_মূল "`");
  ret = statfs (CGROUP_মূল "/unified", &stat);
  যদি (ret < 0 && errno != ENOENT)
    প্রত্যাগম crun_make_error (err, errno, "statfs `" CGROUP_মূল "/unified`");
  যদি (ret < 0)
    প্রত্যাগম CGROUP_MODE_LEGACY;
  প্রত্যাগম stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

পূর্ণাঙ্ক
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  পূর্ণাঙ্ক tmp;
  স্থির পূর্ণাঙ্ক cgroup_mode = 0;

  যদি (cgroup_mode)
    প্রত্যাগম cgroup_mode;

  tmp = detect_cgroup_mode (err);
  যদি (UNLIKELY (tmp < 0))
    প্রত্যাগম tmp;

  cgroup_mode = tmp;

  প্রত্যাগম cgroup_mode;
}

স্থির পূর্ণাঙ্ক
get_cgroup_process_from_content (বর্ণ *content, পূর্ণাঙ্ক cgroup_mode, বর্ণ **path, bool absolute, libcrun_error_t *err)
{
  বর্ণ *cg_path = রিক্ত;
  বর্ণ *controller;
  বর্ণ *saveptr;
  bool has_data;

  প্রতি (has_data = read_proc_cgroup (content, &saveptr, রিক্ত, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (রিক্ত, &saveptr, রিক্ত, &controller, &cg_path))
    {
      যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          যদি (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            গচ্ছ found;
        }
      অন্যথা
        {
          যদি (strcmp (controller, "memory"))
            গচ্ছ found;
        }
    }

  প্রত্যাগম crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  যদি (absolute)
    প্রত্যাগম append_paths (path, err, CGROUP_মূল, cg_path, রিক্ত);

  *path = xstrdup (cg_path);
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_get_cgroup_process_at (পূর্ণাঙ্ক dirfd, বর্ণ **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free বর্ণ *content = রিক্ত;
  size_t content_size;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

পূর্ণাঙ্ক
libcrun_get_cgroup_process (pid_t pid, বর্ণ **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free বর্ণ *content = রিক্ত;
  বর্ণ proc_cgroup_file[64];
  size_t content_size;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  যদি (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  অন্যথা
    {
      পূর্ণাঙ্ক len = snprintf (proc_cgroup_file, পরিমাণ (proc_cgroup_file), "/proc/%d/cgroup", pid);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (proc_cgroup_file)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

স্থির পূর্ণাঙ্ক
read_pids_file (পূর্ণাঙ্ক dfd, ধ্রুব বর্ণ *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক tasksfd = -1;
  cleanup_free বর্ণ *buffer = রিক্ত;
  বর্ণ *saveptr = রিক্ত;
  size_t n_new_pids;
  size_t len;
  বর্ণ *it;
  পূর্ণাঙ্ক ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  যদি (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      যদি (errno == ENOENT || errno == ENODEV)
        প্রত্যাগম 0;
      প্রত্যাগম crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (len > 0)
    {
      প্রতি (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      যদি (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, পরিমাণ (pid_t) * *allocated);
        }

      প্রতি (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (রিক্ত, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, রিক্ত, 10);

          যদি (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  প্রত্যাগম 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
সংরচনা cgroup_walk_dir_s
{
  বর্ণ *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
সংরচনা cgroup_walk_s
{
  পূর্ণাঙ্ক dfd;
  size_t depth;
  সংরচনা cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

স্থির শূন্য
cgroup_walk_বের_করো (সংরচনা cgroup_walk_s *walk)
{
  মুক্ত (walk->dirs[--walk->n_dirs].name);
}

স্থির শূন্য
cleanup_cgroup_walkp (সংরচনা cgroup_walk_s *walk)
{
  পূর্ণাঙ্ক saved_errno = errno;

  যাবত্ (walk->n_dirs > 0)
    cgroup_walk_বের_করো (walk);
  মুক্ত (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
স্থির পূর্ণাঙ্ক
cgroup_walk_enter (সংরচনা cgroup_walk_s *walk, পূর্ণাঙ্ক dfd)
{
  cleanup_close পূর্ণাঙ্ক dfd_clone = -1;
  cleanup_dir DIR *dir = রিক্ত;
  সংরচনা dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  যদি (UNLIKELY (dfd_clone < 0))
    প্রত্যাগম -1;

  dir = fdopendir (dfd_clone);
  যদি (UNLIKELY (dir == রিক্ত))
    প্রত্যাগম -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  প্রতি (de = readdir (dir); de; de = readdir (dir))
    {
      সংরচনা cgroup_walk_dir_s *d;

      যদি (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        অনুবর্ত;

      যদি (de->d_type != DT_DIR)
        অনুবর্ত;

      যদি (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, পরিমাণ (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  প্রত্যাগম 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
স্থির পূর্ণাঙ্ক
cgroup_walk_next (সংরচনা cgroup_walk_s *walk, সংরচনা cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : রিক্ত;

  যাবত্ (*d && walk->depth >= (*d)->depth)
    {
      পূর্ণাঙ্ক parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (parent < 0))
        প্রত্যাগম -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
read_pids_cgroup_file (পূর্ণাঙ্ক dfd, ধ্রুব বর্ণ *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk সংরচনা cgroup_walk_s walk = { .dfd = -1 };
  পূর্ণাঙ্ক dfd_clone;
  পূর্ণাঙ্ক ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (! recurse)
    প্রত্যাগম 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (dfd_clone < 0))
    প্রত্যাগম crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "open cgroup sub-directory");

  প্রতি (;;)
    {
      সংরচনা cgroup_walk_dir_s *d;
      cleanup_close পূর্ণাঙ্ক nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "open parent cgroup directory");
      যদি (d == রিক্ত)
        ভঙ্গ;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (nfd < 0))
        {
          পূর্ণাঙ্ক saved_errno = errno;

          যদি (saved_errno == ENOENT)
            {
              cgroup_walk_বের_করো (&walk);
              অনুবর্ত;
            }
          প্রত্যাগম crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_বের_করো (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      যদি (UNLIKELY (ret < 0))
        {
          পূর্ণাঙ্ক e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          যদি (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            প্রত্যাগম ret;
          crun_error_release (err);
          অনুবর্ত;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      যদি (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        প্রত্যাগম crun_make_error (err, errno, "open cgroup sub-directory");
    }

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
read_pids_cgroup (পূর্ণাঙ্ক dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  যদি (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক
rmdir_all_fd (পূর্ণাঙ্ক dfd)
{
  cleanup_cgroup_walk সংরচনা cgroup_walk_s walk = { .dfd = -1 };
  পূর্ণাঙ্ক ret;

  ret = cgroup_walk_enter (&walk, dfd);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রতি (;;)
    {
      সংরচনা cgroup_walk_dir_s *d;
      পূর্ণাঙ্ক child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      যদি (d == রিক্ত)
        ভঙ্গ;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      যদি (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_বের_করো (&walk);
          অনুবর্ত;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      যদি (UNLIKELY (child_dfd < 0))
        {
          যদি (errno == ENOENT)
            {
              cgroup_walk_বের_করো (&walk);
              অনুবর্ত;
            }
          প্রত্যাগম child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      যদি (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        প্রত্যাগম ret;
    }

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
rmdir_all (ধ্রুব বর্ণ *path)
{
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  যদি (UNLIKELY (dfd < 0))
    প্রত্যাগম dfd;

  ret = rmdir_all_fd (dfd);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম rmdir (path);
}

স্থির পূর্ণাঙ্ক
read_pids_from_path (ধ্রুব বর্ণ *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free বর্ণ *cgroup_path = রিক্ত;
  cleanup_close পূর্ণাঙ্ক dirfd = -1;
  size_t n_pids, allocated;
  পূর্ণাঙ্ক mode;
  পূর্ণাঙ্ক ret;

  যদি (path == রিক্ত || *path == '\0')
    প্রত্যাগম 0;

  mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (mode < 0))
    প্রত্যাগম mode;

  বিকল্প (mode)
    {
    পক্ষ CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_মূল, path, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      ভঙ্গ;

    পক্ষ CGROUP_MODE_HYBRID:
    পক্ষ CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_মূল "/memory", path, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      ভঙ্গ;

    স্বভাব:
      প্রত্যাগম crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (dirfd < 0)
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  যদি (threads && mode == CGROUP_MODE_UNIFIED)
    প্রত্যাগম read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  প্রত্যাগম read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

পূর্ণাঙ্ক
libcrun_cgroup_read_pids_from_path (ধ্রুব বর্ণ *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  প্রত্যাগম read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (বর্ণ *content, বর্ণ **saveptr, বর্ণ **id, বর্ণ **controller_list, বর্ণ **path)
{
  বর্ণ *it;

  it = strtok_r (content, "\n", saveptr);
  যদি (it == রিক্ত)
    প্রত্যাগম false;

  যদি (id)
    *id = it;

  it = strchr (it, ':');
  যদি (it == রিক্ত)
    প্রত্যাগম false;
  *it++ = '\0';

  যদি (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  যদি (it == রিক্ত)
    প্রত্যাগম false;
  *it++ = '\0';

  যদি (path)
    *path = it;

  প্রত্যাগম true;
}

পূর্ণাঙ্ক
destroy_cgroup_path (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode, libcrun_error_t *err)
{
  bool repeat = true;
  পূর্ণাঙ্ক retry_গুনতি = 0;
  ধ্রুব পূর্ণাঙ্ক max_attempts = 500;
  পূর্ণাঙ্ক ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  যদি (is_empty_string (path))
    প্রত্যাগম 0;

  কুরু
    {
      repeat = false;

      যদি (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free বর্ণ *cgroup_path = রিক্ত;

          ret = append_paths (&cgroup_path, err, CGROUP_মূল, path, রিক্ত);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          ret = rmdir (cgroup_path);
          যদি (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              যদি (ret < 0)
                {
                  যদি (retry_গুনতি >= max_attempts)
                    প্রত্যাগম crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_গুনতি++;
                  repeat = true;
                }
            }
        }
      অন্যথা
        {
          cleanup_free বর্ণ *content = রিক্ত;
          size_t content_size;
          বর্ণ *controller;
          বর্ণ *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          যদি (UNLIKELY (ret < 0))
            {
              যদি (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  প্রত্যাগম 0;
                }
              প্রত্যাগম ret;
            }

          প্রতি (has_data = read_proc_cgroup (content, &saveptr, রিক্ত, &controller, রিক্ত);
               has_data;
               has_data = read_proc_cgroup (রিক্ত, &saveptr, রিক্ত, &controller, রিক্ত))
            {
              cleanup_free বর্ণ *cgroup_path = রিক্ত;
              বর্ণ *subsystem;
              যদি (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              যদি (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                অনুবর্ত;

              ret = append_paths (&cgroup_path, err, CGROUP_মূল, subsystem, path, রিক্ত);
              যদি (UNLIKELY (ret < 0))
                প্রত্যাগম ret;

              ret = rmdir (cgroup_path);
              যদি (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  যদি (ret < 0)
                    {
                      যদি (retry_গুনতি >= max_attempts)
                        প্রত্যাগম crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_গুনতি++;
                      repeat = true;
                    }
                }
            }
        }

      যদি (repeat)
        {
          সংরচনা timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, রিক্ত);

          ret = cgroup_killall_path (path, SIGKILL, err);
          যদি (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } যাবত্ (repeat);

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
chown_cgroups (ধ্রুব বর্ণ *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free বর্ণ *cgroup_path = রিক্ত;
  cleanup_free বর্ণ *delegate = রিক্ত;
  cleanup_close পূর্ণাঙ্ক dfd = -1;
  size_t delegate_size;
  বর্ণ *saveptr = রিক্ত;
  বর্ণ *name;
  পূর্ণাঙ্ক ret;

  ret = append_paths (&cgroup_path, err, CGROUP_মূল, path, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  যদি (UNLIKELY (dfd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          প্রত্যাগম 0;
        }
      প্রত্যাগম ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  প্রতি (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (রিক্ত, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (errno == ENOENT)
            অনুবর্ত;

          প্রত্যাগম crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
libcrun_cgroup_pause_unpause_with_mode (ধ্রুব বর্ণ *cgroup_path, পূর্ণাঙ্ক cgroup_mode, ধ্রুব bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free বর্ণ *path = রিক্ত;
  ধ্রুব বর্ণ *state = "";
  পূর্ণাঙ্ক ret;

  যদি (cgroup_path == রিক্ত || cgroup_path[0] == '\0')
    প্রত্যাগম crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_মূল, cgroup_path, "cgroup.freeze", রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  অন্যথা
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_মূল "/freezer", cgroup_path, "freezer.state", রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  যদি (ret >= 0)
    প্রত্যাগম 0;
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
libcrun_cgroup_pause_unpause_path (ধ্রুব বর্ণ *cgroup_path, ধ্রুব bool pause, libcrun_error_t *err)
{
  পূর্ণাঙ্ক cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  প্রত্যাগম libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
পূর্ণাঙ্ক
libcrun_cgroup_ensure_not_frozen (ধ্রুব বর্ণ *cgroup_path, libcrun_error_t *err)
{
  cleanup_free বর্ণ *content = রিক্ত;
  cleanup_free বর্ণ *path = রিক্ত;
  ধ্রুব বর্ণ *frozen;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_মূল, cgroup_path, "cgroup.freeze", রিক্ত);
    }
  অন্যথা
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_মূল "/freezer", cgroup_path, "freezer.state", রিক্ত);
    }
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = read_all_file (path, &content, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      যদি (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          প্রত্যাগম 0;
        }
      প্রত্যাগম ret;
    }

  যদি (strstr (content, frozen))
    প্রত্যাগম crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  প্রত্যাগম 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
স্থির পূর্ণাঙ্ক
watch_cgroup_events (ধ্রুব বর্ণ *path)
{
  cleanup_free বর্ণ *events_file = রিক্ত;
  cleanup_close পূর্ণাঙ্ক fd = -1;
  libcrun_error_t tmp_err = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_মূল, path, "cgroup.events", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      প্রত্যাগম -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম -1;

  ret = fd;
  fd = -1;
  প্রত্যাগম ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
স্থির পূর্ণাঙ্ক
wait_cgroup_unpopulated (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক inotify_fd)
{
  cleanup_free বর্ণ *events_file = রিক্ত;
  libcrun_error_t tmp_err = রিক্ত;
  সংরচনা timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  পূর্ণাঙ্ক ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_মূল, path, "cgroup.events", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      প্রত্যাগম 0;
    }

  প্রতি (;;)
    {
      cleanup_free বর্ণ *content = রিক্ত;
      বর্ণ buf[4096];
      সংরচনা pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, রিক্ত, &tmp_err);
      যদি (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          প্রত্যাগম 0;
        }

      যদি (strstr (content, "populated 0"))
        প্রত্যাগম 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, রিক্ত));
      যদি (ret <= 0)
        প্রত্যাগম 0;

      যাবত্ (পঠ (inotify_fd, buf, পরিমাণ (buf)) > 0)
        ;
    }
}

পূর্ণাঙ্ক
cgroup_killall_path (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক সঙ্কেত, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close পূর্ণাঙ্ক inotify_fd = -1;
  পূর্ণাঙ্ক ret;
  size_t i;
  cleanup_free pid_t *pids = রিক্ত;

  যদি (path == রিক্ত || *path == '\0')
    প্রত্যাগম 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  যদি (সঙ্কেত == SIGKILL)
    {
      cleanup_free বর্ণ *kill_file = রিক্ত;

      ret = append_paths (&kill_file, err, CGROUP_মূল, path, "cgroup.kill", রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      যদি (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        প্রত্যাগম 0;

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
  ret = read_pids_from_path (path, true, সঙ্কেত == SIGKILL, &pids, err);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (crun_error_get_errno (err) != ENOENT)
        গচ্ছ thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  প্রতি (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], সঙ্কেত);
      যদি (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          গচ্ছ thaw;
        }
    }

  ret = 0;

thaw:
  যদি (frozen)
    {
      libcrun_error_t thaw_err = রিক্ত;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      যদি (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক
read_available_controllers (ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  cleanup_free বর্ণ *controllers = রিক্ত;
  cleanup_free বর্ণ *buf = রিক্ত;
  বর্ণ *saveptr = রিক্ত;
  ধ্রুব বর্ণ *token;
  পূর্ণাঙ্ক available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_মূল, path, "cgroup.controllers", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = read_all_file (controllers, &buf, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রতি (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (রিক্ত, " \n", &saveptr))
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
  প্রত্যাগম available;
}

স্থির পূর্ণাঙ্ক
write_controller_file (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free বর্ণ *subtree_control = রিক্ত;
  cleanup_free বর্ণ *controllers = রিক্ত;
  size_t controllers_len = 0;
  পূর্ণাঙ্ক ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_মূল, path, "cgroup.subtree_control", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  যদি (UNLIKELY (ret < 0))
    {
      cleanup_free বর্ণ *controllers_copy = xmalloc (controllers_len + 1);
      পূর্ণাঙ্ক attempts_বাম;
      বর্ণ *saveptr = রিক্ত;
      ধ্রুব বর্ণ *token;
      পূর্ণাঙ্ক e;

      e = crun_error_get_errno (err);
      যদি (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        প্রত্যাগম crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      যদি (e == ENOENT)
        {
          libcrun_error_t tmp_err = রিক্ত;
          পূর্ণাঙ্ক exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          যদি (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              প্রত্যাগম ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          যদি (exists == 0)
            প্রত্যাগম ret;
        }

      crun_error_release (err);

      যদি (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      প্রতি (attempts_বাম = 1000; attempts_বাম >= 0; attempts_বাম--)
        {
          পূর্ণাঙ্ক controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          প্রতি (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (রিক্ত, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              যদি (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  যদি (e == EBUSY)
                    repeat = true;

                  অনুবর্ত;
                }

              controllers_written++;
            }

          যদি (! repeat)
            ভঙ্গ;

          /* If there was any controller written, just try once more without any delay.  */
          যদি (controllers_written > 0 && attempts_বাম > 2)
            {
              attempts_বাম = 1;
              অনুবর্ত;
            }

          যদি (attempts_বাম > 0)
            {
              সংরচনা timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, রিক্ত);
            }
        }

      /* Refresh what controllers are available.  */
      প্রত্যাগম read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  প্রত্যাগম controllers_to_enable;
}

পূর্ণাঙ্ক
enable_controllers (ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  cleanup_free বর্ণ *tmp_path = রিক্ত;
  বর্ণ *it;
  পূর্ণাঙ্ক ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

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
        প্রত্যাগম ret;
    }

  প্রতি (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free বর্ণ *cgroup_path = রিক্ত;
      বর্ণ *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_মূল, tmp_path, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = mkdir (cgroup_path, 0755);
      যদি (UNLIKELY (ret < 0 && errno != EEXIST))
        প্রত্যাগম crun_make_error (err, errno, "create `%s`", cgroup_path);

      যদি (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }

      *it = '/';
      it = next_slash;
    }
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ধ্রুব বর্ণ *path, bool create_if_missing, libcrun_error_t *err)
{
  পূর্ণাঙ্ক cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  যদি (path == রিক্ত || *path == '\0')
    প্রত্যাগম 0;

  প্রত্যাগম enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
পূর্ণাঙ্ক
libcrun_move_self_to_cgroups (ধ্রুব বর্ণ *content, libcrun_error_t *err)
{
  cleanup_free বর্ণ *buf = xstrdup (content);
  বর্ণ *saveptr = রিক্ত;
  বর্ণ *controller;
  বর্ণ *path;
  পূর্ণাঙ্ক cgroup_mode;
  bool has_data;
  পূর্ণাঙ্ক ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  প্রতি (has_data = read_proc_cgroup (buf, &saveptr, রিক্ত, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (রিক্ত, &saveptr, রিক্ত, &controller, &path))
    {
      ধ্রুব বর্ণ *subsystem = রিক্ত;

      যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          যদি (controller[0] != '\0')
            অনুবর্ত;
        }
      অন্যথা
        {
          cleanup_free বর্ণ *subsystem_path = রিক্ত;

          /* Use the same names as enter_cgroup_v1 does.  */
          যদি (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          যদি (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          যদি (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_মূল, subsystem, রিক্ত);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          ret = crun_path_exists (subsystem_path, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          যদি (ret == 0)
            অনুবর্ত;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_get_cgroup_dirfd (সংরচনা libcrun_cgroup_status *status, ধ্রুব বর্ণ *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free বর্ণ *path_to_cgroup = রিক্ত;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক cgroupdirfd;
  পূর্ণাঙ্ক ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (cgroup_mode < 0)
    প্রত্যাগম cgroup_mode;

  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    প্রত্যাগম crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  যদি (status == রিক্ত)
    প্রত্যাগম crun_make_error (err, 0, "internal error");

  যদি (is_empty_string (status->path))
    প্রত্যাগম crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_মূল, status->path, sub_cgroup, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  যদি (UNLIKELY (cgroupdirfd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  প্রত্যাগম cgroupdirfd;
}

পূর্ণাঙ্ক
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, বর্ণ *from, বর্ণ *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = রিক্ত;
  cleanup_close পূর্ণাঙ্ক child_dfd = -1;
  পূর্ণাঙ্ক cgroup_mode;
  size_t from_len;
  size_t i;
  পূর্ণাঙ্ক ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (cgroup_mode < 0)
    প্রত্যাগম cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ thaw;

  from_len = strlen (from);

  প্রতি (i = 0; pids && pids[i]; i++)
    {
      cleanup_free বর্ণ *pid_path = রিক্ত;
      cleanup_free বর্ণ *dest_cgroup = রিক্ত;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      যদি (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          গচ্ছ thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ thaw;
    }

  ret = 0;

thaw:
  যদি (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = রিক্ত;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      যদি (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      প্রত্যাগম ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম destroy_cgroup_path (from, cgroup_mode, err);
}

পূর্ণাঙ্ক
get_cgroup_dirfd_path (পূর্ণাঙ্ক dirfd, বর্ণ **path, libcrun_error_t *err)
{
  cleanup_free বর্ণ *cgroup_path = রিক্ত;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  যদি (UNLIKELY (len < 0))
    প্রত্যাগম len;

  যদি (has_prefix (cgroup_path, CGROUP_মূল))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_মূল));
      প্রত্যাগম 0;
    }
  প্রত্যাগম crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
পূর্ণাঙ্ক
parse_cpu_max (ধ্রুব বর্ণ *মান, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ধ্রুব বর্ণ *it = মান;
  বর্ণ *endptr = রিক্ত;

  যাবত্ (*it == ' ')
    it++;

  errno = 0;
  যদি (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (বর্ণ *) it + 3;
    }
  অন্যথা
    {
      *quota = strtoll (it, &endptr, 10);
      যদি (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        প্রত্যাগম crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", মান);
    }

  it = endptr;
  যাবত্ (*it == ' ')
    it++;

  *has_period = false;
  যদি (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      যদি (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        প্রত্যাগম crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", মান);
      *has_period = true;
    }

  প্রত্যাগম 0;
}
