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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "cgroup.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-internal.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-systemd.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-utils.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-setup.h"
#ଅନ୍ତର୍ଭାଵ "ebpf.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "status.h"
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <sys/vfs.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <poll.h>
#ଅନ୍ତର୍ଭାଵ <sys/inotify.h>

#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <libgen.h>

ସଂରଚନା symlink_s
{
  ଅଚଳ ଅକ୍ଷର *name;
  ଅଚଳ ଅକ୍ଷର *target;
};

ସ୍ଥିର ସଂରଚନା symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { ଖାଲି, ଖାଲି } };

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroups_create_symlinks (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ପାଇଁ (i = 0; cgroup_symlinks[i].name; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (errno == ENOENT || errno == EEXIST)
            ଜାରି;
          ଫେରନ୍ତୁ crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
maybe_make_cgroup_threaded (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cgroup_path_type = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *ଅଚଳ threaded = "threaded";
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *parent;
  size_t size;
  ଅକ୍ଷର *it;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (path == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  path = consume_slashes (path);

  ଯଦି (path[0] == '\0')
    ଫେରନ୍ତୁ 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ମୂଳ, path, "cgroup.type", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (size > 0)
    {
      it = content + size - 1;
      ଯେତେବେଳେ (*it == '\n' && it > content)
        *it-- = '\0';
    }

  ଯଦି (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    ଫେରନ୍ତୁ 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  ଯଦି (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      ଯଦି (ret < 0)
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
move_process_to_cgroup (pid_t pid, ଅଚଳ ଅକ୍ଷର *subsystem, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cgroup_path_procs = ଖାଲି;
  ଅକ୍ଷର pid_str[16];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ମୂଳ,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = snprintf (pid_str, ଆକାର (pid_str), "%d", pid);
  ଯଦି (UNLIKELY (ret >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (pid_str)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = ଖାଲି;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          ଯଦି (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              ଫେରନ୍ତୁ ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          ଫେରନ୍ତୁ write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
detect_cgroup_mode (libcrun_error_t *err)
{
  ସଂରଚନା statfs stat;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = statfs (CGROUP_ମୂଳ, &stat);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "statfs `" CGROUP_ମୂଳ "`");
  ଯଦି (stat.f_type == CGROUP2_SUPER_MAGIC)
    ଫେରନ୍ତୁ CGROUP_MODE_UNIFIED;
  ଯଦି (stat.f_type != TMPFS_MAGIC)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid file system type on `" CGROUP_ମୂଳ "`");
  ret = statfs (CGROUP_ମୂଳ "/unified", &stat);
  ଯଦି (ret < 0 && errno != ENOENT)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "statfs `" CGROUP_ମୂଳ "/unified`");
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ CGROUP_MODE_LEGACY;
  ଫେରନ୍ତୁ stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmp;
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode = 0;

  ଯଦି (cgroup_mode)
    ଫେରନ୍ତୁ cgroup_mode;

  tmp = detect_cgroup_mode (err);
  ଯଦି (UNLIKELY (tmp < 0))
    ଫେରନ୍ତୁ tmp;

  cgroup_mode = tmp;

  ଫେରନ୍ତୁ cgroup_mode;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_cgroup_process_from_content (ଅକ୍ଷର *content, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode, ଅକ୍ଷର **path, bool absolute, libcrun_error_t *err)
{
  ଅକ୍ଷର *cg_path = ଖାଲି;
  ଅକ୍ଷର *controller;
  ଅକ୍ଷର *saveptr;
  bool has_data;

  ପାଇଁ (has_data = read_proc_cgroup (content, &saveptr, ଖାଲି, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (ଖାଲି, &saveptr, ଖାଲି, &controller, &cg_path))
    {
      ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          ଯଦି (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            ଯାଅ found;
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (strcmp (controller, "memory"))
            ଯାଅ found;
        }
    }

  ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  ଯଦି (absolute)
    ଫେରନ୍ତୁ append_paths (path, err, CGROUP_ମୂଳ, cg_path, ଖାଲି);

  *path = xstrdup (cg_path);
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_get_cgroup_process_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅକ୍ଷର **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  size_t content_size;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_get_cgroup_process (pid_t pid, ଅକ୍ଷର **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  ଅକ୍ଷର proc_cgroup_file[64];
  size_t content_size;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  ଅନ୍ୟଥା
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = snprintf (proc_cgroup_file, ଆକାର (proc_cgroup_file), "/proc/%d/cgroup", pid);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (proc_cgroup_file)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_pids_file (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd, ଅଚଳ ଅକ୍ଷର *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା tasksfd = -1;
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  ଅକ୍ଷର *saveptr = ଖାଲି;
  size_t n_new_pids;
  size_t len;
  ଅକ୍ଷର *it;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  ଯଦି (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      ଯଦି (errno == ENOENT || errno == ENODEV)
        ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (len > 0)
    {
      ପାଇଁ (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      ଯଦି (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, ଆକାର (pid_t) * *allocated);
        }

      ପାଇଁ (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (ଖାଲି, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, ଖାଲି, 10);

          ଯଦି (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  ଫେରନ୍ତୁ 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
ସଂରଚନା cgroup_walk_dir_s
{
  ଅକ୍ଷର *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
ସଂରଚନା cgroup_walk_s
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd;
  size_t depth;
  ସଂରଚନା cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

ସ୍ଥିର ଶୂନ୍ୟ
cgroup_walk_କାଢ଼ (ସଂରଚନା cgroup_walk_s *walk)
{
  ମୁକ୍ତ (walk->dirs[--walk->n_dirs].name);
}

ସ୍ଥିର ଶୂନ୍ୟ
cleanup_cgroup_walkp (ସଂରଚନା cgroup_walk_s *walk)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;

  ଯେତେବେଳେ (walk->n_dirs > 0)
    cgroup_walk_କାଢ଼ (walk);
  ମୁକ୍ତ (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cgroup_walk_enter (ସଂରଚନା cgroup_walk_s *walk, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd_clone = -1;
  cleanup_dir DIR *dir = ଖାଲି;
  ସଂରଚନା dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  ଯଦି (UNLIKELY (dfd_clone < 0))
    ଫେରନ୍ତୁ -1;

  dir = fdopendir (dfd_clone);
  ଯଦି (UNLIKELY (dir == ଖାଲି))
    ଫେରନ୍ତୁ -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  ପାଇଁ (de = readdir (dir); de; de = readdir (dir))
    {
      ସଂରଚନା cgroup_walk_dir_s *d;

      ଯଦି (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ଜାରି;

      ଯଦି (de->d_type != DT_DIR)
        ଜାରି;

      ଯଦି (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, ଆକାର (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  ଫେରନ୍ତୁ 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cgroup_walk_next (ସଂରଚନା cgroup_walk_s *walk, ସଂରଚନା cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : ଖାଲି;

  ଯେତେବେଳେ (*d && walk->depth >= (*d)->depth)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      ଯଦି (UNLIKELY (parent < 0))
        ଫେରନ୍ତୁ -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_pids_cgroup_file (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd, ଅଚଳ ଅକ୍ଷର *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk ସଂରଚନା cgroup_walk_s walk = { .dfd = -1 };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd_clone;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (! recurse)
    ଫେରନ୍ତୁ 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (dfd_clone < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open cgroup sub-directory");

  ପାଇଁ (;;)
    {
      ସଂରଚନା cgroup_walk_dir_s *d;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open parent cgroup directory");
      ଯଦି (d == ଖାଲି)
        ଭାଙ୍ଗ;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ଯଦି (UNLIKELY (nfd < 0))
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;

          ଯଦି (saved_errno == ENOENT)
            {
              cgroup_walk_କାଢ଼ (&walk);
              ଜାରି;
            }
          ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_କାଢ଼ (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          ଯଦି (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            ଫେରନ୍ତୁ ret;
          crun_error_release (err);
          ଜାରି;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      ଯଦି (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open cgroup sub-directory");
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_pids_cgroup (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  ଯଦି (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
rmdir_all_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd)
{
  cleanup_cgroup_walk ସଂରଚନା cgroup_walk_s walk = { .dfd = -1 };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = cgroup_walk_enter (&walk, dfd);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ପାଇଁ (;;)
    {
      ସଂରଚନା cgroup_walk_dir_s *d;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      ଯଦି (d == ଖାଲି)
        ଭାଙ୍ଗ;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      ଯଦି (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_କାଢ଼ (&walk);
          ଜାରି;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      ଯଦି (UNLIKELY (child_dfd < 0))
        {
          ଯଦି (errno == ENOENT)
            {
              cgroup_walk_କାଢ଼ (&walk);
              ଜାରି;
            }
          ଫେରନ୍ତୁ child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      ଯଦି (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
rmdir_all (ଅଚଳ ଅକ୍ଷର *path)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  ଯଦି (UNLIKELY (dfd < 0))
    ଫେରନ୍ତୁ dfd;

  ret = rmdir_all_fd (dfd);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ rmdir (path);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_pids_from_path (ଅଚଳ ଅକ୍ଷର *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd = -1;
  size_t n_pids, allocated;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (path == ଖାଲି || *path == '\0')
    ଫେରନ୍ତୁ 0;

  mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (mode < 0))
    ଫେରନ୍ତୁ mode;

  ବାଛ (mode)
    {
    ସ୍ଥିତି CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, path, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି CGROUP_MODE_HYBRID:
    ସ୍ଥିତି CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ "/memory", path, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (dirfd < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  ଯଦି (threads && mode == CGROUP_MODE_UNIFIED)
    ଫେରନ୍ତୁ read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  ଫେରନ୍ତୁ read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_read_pids_from_path (ଅଚଳ ଅକ୍ଷର *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (ଅକ୍ଷର *content, ଅକ୍ଷର **saveptr, ଅକ୍ଷର **id, ଅକ୍ଷର **controller_list, ଅକ୍ଷର **path)
{
  ଅକ୍ଷର *it;

  it = strtok_r (content, "\n", saveptr);
  ଯଦି (it == ଖାଲି)
    ଫେରନ୍ତୁ false;

  ଯଦି (id)
    *id = it;

  it = strchr (it, ':');
  ଯଦି (it == ଖାଲି)
    ଫେରନ୍ତୁ false;
  *it++ = '\0';

  ଯଦି (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  ଯଦି (it == ଖାଲି)
    ଫେରନ୍ତୁ false;
  *it++ = '\0';

  ଯଦି (path)
    *path = it;

  ଫେରନ୍ତୁ true;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
destroy_cgroup_path (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, libcrun_error_t *err)
{
  bool repeat = true;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା retry_ଗଣନା = 0;
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_attempts = 500;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  ଯଦି (is_empty_string (path))
    ଫେରନ୍ତୁ 0;

  କର
    {
      repeat = false;

      ଯଦି (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;

          ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, path, ଖାଲି);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          ret = rmdir (cgroup_path);
          ଯଦି (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              ଯଦି (ret < 0)
                {
                  ଯଦି (retry_ଗଣନା >= max_attempts)
                    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_ଗଣନା++;
                  repeat = true;
                }
            }
        }
      ଅନ୍ୟଥା
        {
          cleanup_free ଅକ୍ଷର *content = ଖାଲି;
          size_t content_size;
          ଅକ୍ଷର *controller;
          ଅକ୍ଷର *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ଯଦି (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  ଫେରନ୍ତୁ 0;
                }
              ଫେରନ୍ତୁ ret;
            }

          ପାଇଁ (has_data = read_proc_cgroup (content, &saveptr, ଖାଲି, &controller, ଖାଲି);
               has_data;
               has_data = read_proc_cgroup (ଖାଲି, &saveptr, ଖାଲି, &controller, ଖାଲି))
            {
              cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
              ଅକ୍ଷର *subsystem;
              ଯଦି (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              ଯଦି (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                ଜାରି;

              ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, subsystem, path, ଖାଲି);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;

              ret = rmdir (cgroup_path);
              ଯଦି (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  ଯଦି (ret < 0)
                    {
                      ଯଦି (retry_ଗଣନା >= max_attempts)
                        ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_ଗଣନା++;
                      repeat = true;
                    }
                }
            }
        }

      ଯଦି (repeat)
        {
          ସଂରଚନା timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, ଖାଲି);

          ret = cgroup_killall_path (path, SIGKILL, err);
          ଯଦି (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } ଯେତେବେଳେ (repeat);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
chown_cgroups (ଅଚଳ ଅକ୍ଷର *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *delegate = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd = -1;
  size_t delegate_size;
  ଅକ୍ଷର *saveptr = ଖାଲି;
  ଅକ୍ଷର *name;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, path, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  ଯଦି (UNLIKELY (dfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ଫେରନ୍ତୁ 0;
        }
      ଫେରନ୍ତୁ ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  ପାଇଁ (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (ଖାଲି, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (errno == ENOENT)
            ଜାରି;

          ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_pause_unpause_with_mode (ଅଚଳ ଅକ୍ଷର *cgroup_path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode, ଅଚଳ bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *state = "";
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (cgroup_path == ଖାଲି || cgroup_path[0] == '\0')
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ମୂଳ, cgroup_path, "cgroup.freeze", ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ମୂଳ "/freezer", cgroup_path, "freezer.state", ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  ଯଦି (ret >= 0)
    ଫେରନ୍ତୁ 0;
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_pause_unpause_path (ଅଚଳ ଅକ୍ଷର *cgroup_path, ଅଚଳ bool pause, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଫେରନ୍ତୁ libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_ensure_not_frozen (ଅଚଳ ଅକ୍ଷର *cgroup_path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *frozen;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ମୂଳ, cgroup_path, "cgroup.freeze", ଖାଲି);
    }
  ଅନ୍ୟଥା
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ମୂଳ "/freezer", cgroup_path, "freezer.state", ଖାଲି);
    }
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = read_all_file (path, &content, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      ଯଦି (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ଫେରନ୍ତୁ 0;
        }
      ଫେରନ୍ତୁ ret;
    }

  ଯଦି (strstr (content, frozen))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  ଫେରନ୍ତୁ 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
watch_cgroup_events (ଅଚଳ ଅକ୍ଷର *path)
{
  cleanup_free ଅକ୍ଷର *events_file = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  libcrun_error_t tmp_err = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ମୂଳ, path, "cgroup.events", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ଫେରନ୍ତୁ -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ -1;

  ret = fd;
  fd = -1;
  ଫେରନ୍ତୁ ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
wait_cgroup_unpopulated (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inotify_fd)
{
  cleanup_free ଅକ୍ଷର *events_file = ଖାଲି;
  libcrun_error_t tmp_err = ଖାଲି;
  ସଂରଚନା timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ମୂଳ, path, "cgroup.events", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ଫେରନ୍ତୁ 0;
    }

  ପାଇଁ (;;)
    {
      cleanup_free ଅକ୍ଷର *content = ଖାଲି;
      ଅକ୍ଷର buf[4096];
      ସଂରଚନା pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, ଖାଲି, &tmp_err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          ଫେରନ୍ତୁ 0;
        }

      ଯଦି (strstr (content, "populated 0"))
        ଫେରନ୍ତୁ 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, ଖାଲି));
      ଯଦି (ret <= 0)
        ଫେରନ୍ତୁ 0;

      ଯେତେବେଳେ (ପଠ (inotify_fd, buf, ଆକାର (buf)) > 0)
        ;
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cgroup_killall_path (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଙ୍କେତ, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା inotify_fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  size_t i;
  cleanup_free pid_t *pids = ଖାଲି;

  ଯଦି (path == ଖାଲି || *path == '\0')
    ଫେରନ୍ତୁ 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  ଯଦି (ସଙ୍କେତ == SIGKILL)
    {
      cleanup_free ଅକ୍ଷର *kill_file = ଖାଲି;

      ret = append_paths (&kill_file, err, CGROUP_ମୂଳ, path, "cgroup.kill", ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      ଯଦି (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        ଫେରନ୍ତୁ 0;

      ଯଦି (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    crun_error_release (err);
  ଅନ୍ୟଥା
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, ସଙ୍କେତ == SIGKILL, &pids, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (crun_error_get_errno (err) != ENOENT)
        ଯାଅ thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  ପାଇଁ (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], ସଙ୍କେତ);
      ଯଦି (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          ଯାଅ thaw;
        }
    }

  ret = 0;

thaw:
  ଯଦି (frozen)
    {
      libcrun_error_t thaw_err = ଖାଲି;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      ଯଦି (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_available_controllers (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *controllers = ଖାଲି;
  cleanup_free ଅକ୍ଷର *buf = ଖାଲି;
  ଅକ୍ଷର *saveptr = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *token;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ମୂଳ, path, "cgroup.controllers", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = read_all_file (controllers, &buf, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ପାଇଁ (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (ଖାଲି, " \n", &saveptr))
    {
      ଯଦି (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      ଅନ୍ୟଥା ଯଦି (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      ଅନ୍ୟଥା ଯଦି (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      ଅନ୍ୟଥା ଯଦି (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      ଅନ୍ୟଥା ଯଦି (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      ଅନ୍ୟଥା ଯଦି (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      ଅନ୍ୟଥା ଯଦି (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  ଫେରନ୍ତୁ available;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_controller_file (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *subtree_control = ଖାଲି;
  cleanup_free ଅକ୍ଷର *controllers = ଖାଲି;
  size_t controllers_len = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ମୂଳ, path, "cgroup.subtree_control", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      cleanup_free ଅକ୍ଷର *controllers_copy = xmalloc (controllers_len + 1);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା attempts_ବାମ;
      ଅକ୍ଷର *saveptr = ଖାଲି;
      ଅଚଳ ଅକ୍ଷର *token;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା e;

      e = crun_error_get_errno (err);
      ଯଦି (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        ଫେରନ୍ତୁ crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      ଯଦି (e == ENOENT)
        {
          libcrun_error_t tmp_err = ଖାଲି;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          ଯଦି (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              ଫେରନ୍ତୁ ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          ଯଦି (exists == 0)
            ଫେରନ୍ତୁ ret;
        }

      crun_error_release (err);

      ଯଦି (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      ପାଇଁ (attempts_ବାମ = 1000; attempts_ବାମ >= 0; attempts_ବାମ--)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          ପାଇଁ (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (ଖାଲି, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              ଯଦି (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  ଯଦି (e == EBUSY)
                    repeat = true;

                  ଜାରି;
                }

              controllers_written++;
            }

          ଯଦି (! repeat)
            ଭାଙ୍ଗ;

          /* If there was any controller written, just try once more without any delay.  */
          ଯଦି (controllers_written > 0 && attempts_ବାମ > 2)
            {
              attempts_ବାମ = 1;
              ଜାରି;
            }

          ଯଦି (attempts_ବାମ > 0)
            {
              ସଂରଚନା timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, ଖାଲି);
            }
        }

      /* Refresh what controllers are available.  */
      ଫେରନ୍ତୁ read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  ଫେରନ୍ତୁ controllers_to_enable;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
enable_controllers (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *tmp_path = ଖାଲି;
  ଅକ୍ଷର *it;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      ଯଦି ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ପାଇଁ (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
      ଅକ୍ଷର *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, tmp_path, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = mkdir (cgroup_path, 0755);
      ଯଦି (UNLIKELY (ret < 0 && errno != EEXIST))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "create `%s`", cgroup_path);

      ଯଦି (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      *it = '/';
      it = next_slash;
    }
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ଅଚଳ ଅକ୍ଷର *path, bool create_if_missing, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (path == ଖାଲି || *path == '\0')
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_move_self_to_cgroups (ଅଚଳ ଅକ୍ଷର *content, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *buf = xstrdup (content);
  ଅକ୍ଷର *saveptr = ଖାଲି;
  ଅକ୍ଷର *controller;
  ଅକ୍ଷର *path;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  bool has_data;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ପାଇଁ (has_data = read_proc_cgroup (buf, &saveptr, ଖାଲି, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (ଖାଲି, &saveptr, ଖାଲି, &controller, &path))
    {
      ଅଚଳ ଅକ୍ଷର *subsystem = ଖାଲି;

      ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          ଯଦି (controller[0] != '\0')
            ଜାରି;
        }
      ଅନ୍ୟଥା
        {
          cleanup_free ଅକ୍ଷର *subsystem_path = ଖାଲି;

          /* Use the same names as enter_cgroup_v1 does.  */
          ଯଦି (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          ଯଦି (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          ଯଦି (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ମୂଳ, subsystem, ଖାଲି);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          ret = crun_path_exists (subsystem_path, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          ଯଦି (ret == 0)
            ଜାରି;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_get_cgroup_dirfd (ସଂରଚନା libcrun_cgroup_status *status, ଅଚଳ ଅକ୍ଷର *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *path_to_cgroup = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroupdirfd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (cgroup_mode < 0)
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (cgroup_mode != CGROUP_MODE_UNIFIED)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  ଯଦି (status == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error");

  ଯଦି (is_empty_string (status->path))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ମୂଳ, status->path, sub_cgroup, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  ଯଦି (UNLIKELY (cgroupdirfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  ଫେରନ୍ତୁ cgroupdirfd;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, ଅକ୍ଷର *from, ଅକ୍ଷର *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା child_dfd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  size_t from_len;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (cgroup_mode < 0)
    ଫେରନ୍ତୁ cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ thaw;

  from_len = strlen (from);

  ପାଇଁ (i = 0; pids && pids[i]; i++)
    {
      cleanup_free ଅକ୍ଷର *pid_path = ଖାଲି;
      cleanup_free ଅକ୍ଷର *dest_cgroup = ଖାଲି;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      ଯଦି (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          ଯାଅ thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ thaw;
    }

  ret = 0;

thaw:
  ଯଦି (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = ଖାଲି;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      ଯଦି (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      ଫେରନ୍ତୁ ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ destroy_cgroup_path (from, cgroup_mode, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_cgroup_dirfd_path (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅକ୍ଷର **path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  ଯଦି (UNLIKELY (len < 0))
    ଫେରନ୍ତୁ len;

  ଯଦି (has_prefix (cgroup_path, CGROUP_ମୂଳ))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ମୂଳ));
      ଫେରନ୍ତୁ 0;
    }
  ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
parse_cpu_max (ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *it = ମୂଲ୍ୟ;
  ଅକ୍ଷର *endptr = ଖାଲି;

  ଯେତେବେଳେ (*it == ' ')
    it++;

  errno = 0;
  ଯଦି (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (ଅକ୍ଷର *) it + 3;
    }
  ଅନ୍ୟଥା
    {
      *quota = strtoll (it, &endptr, 10);
      ଯଦି (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", ମୂଲ୍ୟ);
    }

  it = endptr;
  ଯେତେବେଳେ (*it == ' ')
    it++;

  *has_period = false;
  ଯଦି (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      ଯଦି (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", ମୂଲ୍ୟ);
      *has_period = true;
    }

  ଫେରନ୍ତୁ 0;
}
