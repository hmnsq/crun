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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-internal.h"
#ಅನ್ತರ್ಭಾವ "cgroup-systemd.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"
#ಅನ್ತರ್ಭಾವ "cgroup-setup.h"
#ಅನ್ತರ್ಭಾವ "ebpf.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <poll.h>
#ಅನ್ತರ್ಭಾವ <sys/inotify.h>

#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>

ಸಂರಚನಾ symlink_s
{
  ಧ್ರುವ ವರ್ಣ *name;
  ಧ್ರುವ ವರ್ಣ *target;
};

ಸ್ಥಿರ ಸಂರಚನಾ symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { ರಿಕ್ತ, ರಿಕ್ತ } };

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroups_create_symlinks (ಪೂರ್ಣಾಙ್ಕ dirfd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ i;

  ಪ್ರತಿ (i = 0; cgroup_symlinks[i].name; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (errno == ENOENT || errno == EEXIST)
            ಅನುವರ್ತ;
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
maybe_make_cgroup_threaded (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cgroup_path_type = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ threaded = "threaded";
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *parent;
  size_t size;
  ವರ್ಣ *it;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (path == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  path = consume_slashes (path);

  ಯದಿ (path[0] == '\0')
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ಬೇರು, path, "cgroup.type", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (size > 0)
    {
      it = content + size - 1;
      ಯಾವತ್ (*it == '\n' && it > content)
        *it-- = '\0';
    }

  ಯದಿ (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  ಯದಿ (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

ಪೂರ್ಣಾಙ್ಕ
move_process_to_cgroup (pid_t pid, ಧ್ರುವ ವರ್ಣ *subsystem, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cgroup_path_procs = ರಿಕ್ತ;
  ವರ್ಣ pid_str[16];
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ಬೇರು,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = snprintf (pid_str, ಪರಿಮಾಣ (pid_str), "%d", pid);
  ಯದಿ (UNLIKELY (ret >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (pid_str)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = ರಿಕ್ತ;
          ಪೂರ್ಣಾಙ್ಕ mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          ಯದಿ (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              ಪ್ರತ್ಯಾಗಮ ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          ಪ್ರತ್ಯಾಗಮ write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
detect_cgroup_mode (libcrun_error_t *err)
{
  ಸಂರಚನಾ statfs stat;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = statfs (CGROUP_ಬೇರು, &stat);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "statfs `" CGROUP_ಬೇರು "`");
  ಯದಿ (stat.f_type == CGROUP2_SUPER_MAGIC)
    ಪ್ರತ್ಯಾಗಮ CGROUP_MODE_UNIFIED;
  ಯದಿ (stat.f_type != TMPFS_MAGIC)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid file system type on `" CGROUP_ಬೇರು "`");
  ret = statfs (CGROUP_ಬೇರು "/unified", &stat);
  ಯದಿ (ret < 0 && errno != ENOENT)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "statfs `" CGROUP_ಬೇರು "/unified`");
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ CGROUP_MODE_LEGACY;
  ಪ್ರತ್ಯಾಗಮ stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ tmp;
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cgroup_mode = 0;

  ಯದಿ (cgroup_mode)
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  tmp = detect_cgroup_mode (err);
  ಯದಿ (UNLIKELY (tmp < 0))
    ಪ್ರತ್ಯಾಗಮ tmp;

  cgroup_mode = tmp;

  ಪ್ರತ್ಯಾಗಮ cgroup_mode;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_cgroup_process_from_content (ವರ್ಣ *content, ಪೂರ್ಣಾಙ್ಕ cgroup_mode, ವರ್ಣ **path, bool absolute, libcrun_error_t *err)
{
  ವರ್ಣ *cg_path = ರಿಕ್ತ;
  ವರ್ಣ *controller;
  ವರ್ಣ *saveptr;
  bool has_data;

  ಪ್ರತಿ (has_data = read_proc_cgroup (content, &saveptr, ರಿಕ್ತ, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (ರಿಕ್ತ, &saveptr, ರಿಕ್ತ, &controller, &cg_path))
    {
      ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          ಯದಿ (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            ಗಚ್ಛ found;
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (strcmp (controller, "memory"))
            ಗಚ್ಛ found;
        }
    }

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  ಯದಿ (absolute)
    ಪ್ರತ್ಯಾಗಮ append_paths (path, err, CGROUP_ಬೇರು, cg_path, ರಿಕ್ತ);

  *path = xstrdup (cg_path);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_get_cgroup_process_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ವರ್ಣ **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  size_t content_size;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_get_cgroup_process (pid_t pid, ವರ್ಣ **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  ವರ್ಣ proc_cgroup_file[64];
  size_t content_size;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  ಅನ್ಯಥಾ
    {
      ಪೂರ್ಣಾಙ್ಕ len = snprintf (proc_cgroup_file, ಪರಿಮಾಣ (proc_cgroup_file), "/proc/%d/cgroup", pid);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (proc_cgroup_file)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_pids_file (ಪೂರ್ಣಾಙ್ಕ dfd, ಧ್ರುವ ವರ್ಣ *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ tasksfd = -1;
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  ವರ್ಣ *saveptr = ರಿಕ್ತ;
  size_t n_new_pids;
  size_t len;
  ವರ್ಣ *it;
  ಪೂರ್ಣಾಙ್ಕ ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  ಯದಿ (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      ಯದಿ (errno == ENOENT || errno == ENODEV)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (len > 0)
    {
      ಪ್ರತಿ (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      ಯದಿ (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, ಪರಿಮಾಣ (pid_t) * *allocated);
        }

      ಪ್ರತಿ (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (ರಿಕ್ತ, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, ರಿಕ್ತ, 10);

          ಯದಿ (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
ಸಂರಚನಾ cgroup_walk_dir_s
{
  ವರ್ಣ *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
ಸಂರಚನಾ cgroup_walk_s
{
  ಪೂರ್ಣಾಙ್ಕ dfd;
  size_t depth;
  ಸಂರಚನಾ cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

ಸ್ಥಿರ ಶೂನ್ಯ
cgroup_walk_ತೆಗೆ (ಸಂರಚನಾ cgroup_walk_s *walk)
{
  ಮುಕ್ತ (walk->dirs[--walk->n_dirs].name);
}

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup_cgroup_walkp (ಸಂರಚನಾ cgroup_walk_s *walk)
{
  ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;

  ಯಾವತ್ (walk->n_dirs > 0)
    cgroup_walk_ತೆಗೆ (walk);
  ಮುಕ್ತ (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cgroup_walk_enter (ಸಂರಚನಾ cgroup_walk_s *walk, ಪೂರ್ಣಾಙ್ಕ dfd)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dfd_clone = -1;
  cleanup_dir DIR *dir = ರಿಕ್ತ;
  ಸಂರಚನಾ dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (dfd_clone < 0))
    ಪ್ರತ್ಯಾಗಮ -1;

  dir = fdopendir (dfd_clone);
  ಯದಿ (UNLIKELY (dir == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  ಪ್ರತಿ (de = readdir (dir); de; de = readdir (dir))
    {
      ಸಂರಚನಾ cgroup_walk_dir_s *d;

      ಯದಿ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ಅನುವರ್ತ;

      ಯದಿ (de->d_type != DT_DIR)
        ಅನುವರ್ತ;

      ಯದಿ (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, ಪರಿಮಾಣ (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cgroup_walk_next (ಸಂರಚನಾ cgroup_walk_s *walk, ಸಂರಚನಾ cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : ರಿಕ್ತ;

  ಯಾವತ್ (*d && walk->depth >= (*d)->depth)
    {
      ಪೂರ್ಣಾಙ್ಕ parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      ಯದಿ (UNLIKELY (parent < 0))
        ಪ್ರತ್ಯಾಗಮ -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_pids_cgroup_file (ಪೂರ್ಣಾಙ್ಕ dfd, ಧ್ರುವ ವರ್ಣ *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk ಸಂರಚನಾ cgroup_walk_s walk = { .dfd = -1 };
  ಪೂರ್ಣಾಙ್ಕ dfd_clone;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (! recurse)
    ಪ್ರತ್ಯಾಗಮ 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (dfd_clone < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open cgroup sub-directory");

  ಪ್ರತಿ (;;)
    {
      ಸಂರಚನಾ cgroup_walk_dir_s *d;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open parent cgroup directory");
      ಯದಿ (d == ರಿಕ್ತ)
        ಭಙ್ಗ;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ಯದಿ (UNLIKELY (nfd < 0))
        {
          ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;

          ಯದಿ (saved_errno == ENOENT)
            {
              cgroup_walk_ತೆಗೆ (&walk);
              ಅನುವರ್ತ;
            }
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_ತೆಗೆ (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಙ್ಕ e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          ಯದಿ (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            ಪ್ರತ್ಯಾಗಮ ret;
          crun_error_release (err);
          ಅನುವರ್ತ;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      ಯದಿ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open cgroup sub-directory");
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_pids_cgroup (ಪೂರ್ಣಾಙ್ಕ dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  ಯದಿ (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
rmdir_all_fd (ಪೂರ್ಣಾಙ್ಕ dfd)
{
  cleanup_cgroup_walk ಸಂರಚನಾ cgroup_walk_s walk = { .dfd = -1 };
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = cgroup_walk_enter (&walk, dfd);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತಿ (;;)
    {
      ಸಂರಚನಾ cgroup_walk_dir_s *d;
      ಪೂರ್ಣಾಙ್ಕ child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      ಯದಿ (d == ರಿಕ್ತ)
        ಭಙ್ಗ;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      ಯದಿ (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_ತೆಗೆ (&walk);
          ಅನುವರ್ತ;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      ಯದಿ (UNLIKELY (child_dfd < 0))
        {
          ಯದಿ (errno == ENOENT)
            {
              cgroup_walk_ತೆಗೆ (&walk);
              ಅನುವರ್ತ;
            }
          ಪ್ರತ್ಯಾಗಮ child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      ಯದಿ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
rmdir_all (ಧ್ರುವ ವರ್ಣ *path)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (dfd < 0))
    ಪ್ರತ್ಯಾಗಮ dfd;

  ret = rmdir_all_fd (dfd);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ rmdir (path);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_pids_from_path (ಧ್ರುವ ವರ್ಣ *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd = -1;
  size_t n_pids, allocated;
  ಪೂರ್ಣಾಙ್ಕ mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (path == ರಿಕ್ತ || *path == '\0')
    ಪ್ರತ್ಯಾಗಮ 0;

  mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (mode < 0))
    ಪ್ರತ್ಯಾಗಮ mode;

  ವಿಕಲ್ಪ (mode)
    {
    ಪಕ್ಷ CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, path, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      ಭಙ್ಗ;

    ಪಕ್ಷ CGROUP_MODE_HYBRID:
    ಪಕ್ಷ CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು "/memory", path, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (dirfd < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  ಯದಿ (threads && mode == CGROUP_MODE_UNIFIED)
    ಪ್ರತ್ಯಾಗಮ read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  ಪ್ರತ್ಯಾಗಮ read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_read_pids_from_path (ಧ್ರುವ ವರ್ಣ *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (ವರ್ಣ *content, ವರ್ಣ **saveptr, ವರ್ಣ **id, ವರ್ಣ **controller_list, ವರ್ಣ **path)
{
  ವರ್ಣ *it;

  it = strtok_r (content, "\n", saveptr);
  ಯದಿ (it == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;

  ಯದಿ (id)
    *id = it;

  it = strchr (it, ':');
  ಯದಿ (it == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;
  *it++ = '\0';

  ಯದಿ (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  ಯದಿ (it == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;
  *it++ = '\0';

  ಯದಿ (path)
    *path = it;

  ಪ್ರತ್ಯಾಗಮ true;
}

ಪೂರ್ಣಾಙ್ಕ
destroy_cgroup_path (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode, libcrun_error_t *err)
{
  bool repeat = true;
  ಪೂರ್ಣಾಙ್ಕ retry_ಎಣಿಕೆ = 0;
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ max_attempts = 500;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  ಯದಿ (is_empty_string (path))
    ಪ್ರತ್ಯಾಗಮ 0;

  ಕುರು
    {
      repeat = false;

      ಯದಿ (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;

          ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, path, ರಿಕ್ತ);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          ret = rmdir (cgroup_path);
          ಯದಿ (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              ಯದಿ (ret < 0)
                {
                  ಯದಿ (retry_ಎಣಿಕೆ >= max_attempts)
                    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_ಎಣಿಕೆ++;
                  repeat = true;
                }
            }
        }
      ಅನ್ಯಥಾ
        {
          cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
          size_t content_size;
          ವರ್ಣ *controller;
          ವರ್ಣ *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ಯದಿ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  ಪ್ರತ್ಯಾಗಮ 0;
                }
              ಪ್ರತ್ಯಾಗಮ ret;
            }

          ಪ್ರತಿ (has_data = read_proc_cgroup (content, &saveptr, ರಿಕ್ತ, &controller, ರಿಕ್ತ);
               has_data;
               has_data = read_proc_cgroup (ರಿಕ್ತ, &saveptr, ರಿಕ್ತ, &controller, ರಿಕ್ತ))
            {
              cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
              ವರ್ಣ *subsystem;
              ಯದಿ (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              ಯದಿ (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                ಅನುವರ್ತ;

              ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, subsystem, path, ರಿಕ್ತ);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;

              ret = rmdir (cgroup_path);
              ಯದಿ (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  ಯದಿ (ret < 0)
                    {
                      ಯದಿ (retry_ಎಣಿಕೆ >= max_attempts)
                        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_ಎಣಿಕೆ++;
                      repeat = true;
                    }
                }
            }
        }

      ಯದಿ (repeat)
        {
          ಸಂರಚನಾ timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, ರಿಕ್ತ);

          ret = cgroup_killall_path (path, SIGKILL, err);
          ಯದಿ (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } ಯಾವತ್ (repeat);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
chown_cgroups (ಧ್ರುವ ವರ್ಣ *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *delegate = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dfd = -1;
  size_t delegate_size;
  ವರ್ಣ *saveptr = ರಿಕ್ತ;
  ವರ್ಣ *name;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, path, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  ಯದಿ (UNLIKELY (dfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಪ್ರತ್ಯಾಗಮ 0;
        }
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  ಪ್ರತಿ (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (ರಿಕ್ತ, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (errno == ENOENT)
            ಅನುವರ್ತ;

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_pause_unpause_with_mode (ಧ್ರುವ ವರ್ಣ *cgroup_path, ಪೂರ್ಣಾಙ್ಕ cgroup_mode, ಧ್ರುವ bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *state = "";
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (cgroup_path == ರಿಕ್ತ || cgroup_path[0] == '\0')
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ಬೇರು, cgroup_path, "cgroup.freeze", ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ಬೇರು "/freezer", cgroup_path, "freezer.state", ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  ಯದಿ (ret >= 0)
    ಪ್ರತ್ಯಾಗಮ 0;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_pause_unpause_path (ಧ್ರುವ ವರ್ಣ *cgroup_path, ಧ್ರುವ bool pause, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಪ್ರತ್ಯಾಗಮ libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_ensure_not_frozen (ಧ್ರುವ ವರ್ಣ *cgroup_path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *frozen;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ಬೇರು, cgroup_path, "cgroup.freeze", ರಿಕ್ತ);
    }
  ಅನ್ಯಥಾ
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ಬೇರು "/freezer", cgroup_path, "freezer.state", ರಿಕ್ತ);
    }
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = read_all_file (path, &content, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      ಯದಿ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಪ್ರತ್ಯಾಗಮ 0;
        }
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (strstr (content, frozen))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  ಪ್ರತ್ಯಾಗಮ 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
watch_cgroup_events (ಧ್ರುವ ವರ್ಣ *path)
{
  cleanup_free ವರ್ಣ *events_file = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ಬೇರು, path, "cgroup.events", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ಪ್ರತ್ಯಾಗಮ -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ -1;

  ret = fd;
  fd = -1;
  ಪ್ರತ್ಯಾಗಮ ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
wait_cgroup_unpopulated (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ inotify_fd)
{
  cleanup_free ವರ್ಣ *events_file = ರಿಕ್ತ;
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  ಸಂರಚನಾ timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ಬೇರು, path, "cgroup.events", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಪ್ರತಿ (;;)
    {
      cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
      ವರ್ಣ buf[4096];
      ಸಂರಚನಾ pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, ರಿಕ್ತ, &tmp_err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          ಪ್ರತ್ಯಾಗಮ 0;
        }

      ಯದಿ (strstr (content, "populated 0"))
        ಪ್ರತ್ಯಾಗಮ 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, ರಿಕ್ತ));
      ಯದಿ (ret <= 0)
        ಪ್ರತ್ಯಾಗಮ 0;

      ಯಾವತ್ (ಪಠ (inotify_fd, buf, ಪರಿಮಾಣ (buf)) > 0)
        ;
    }
}

ಪೂರ್ಣಾಙ್ಕ
cgroup_killall_path (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ ಸಙ್ಕೇತ, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ inotify_fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;
  size_t i;
  cleanup_free pid_t *pids = ರಿಕ್ತ;

  ಯದಿ (path == ರಿಕ್ತ || *path == '\0')
    ಪ್ರತ್ಯಾಗಮ 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  ಯದಿ (ಸಙ್ಕೇತ == SIGKILL)
    {
      cleanup_free ವರ್ಣ *kill_file = ರಿಕ್ತ;

      ret = append_paths (&kill_file, err, CGROUP_ಬೇರು, path, "cgroup.kill", ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      ಯದಿ (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        ಪ್ರತ್ಯಾಗಮ 0;

      ಯದಿ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    crun_error_release (err);
  ಅನ್ಯಥಾ
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, ಸಙ್ಕೇತ == SIGKILL, &pids, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (crun_error_get_errno (err) != ENOENT)
        ಗಚ್ಛ thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  ಪ್ರತಿ (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], ಸಙ್ಕೇತ);
      ಯದಿ (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          ಗಚ್ಛ thaw;
        }
    }

  ret = 0;

thaw:
  ಯದಿ (frozen)
    {
      libcrun_error_t thaw_err = ರಿಕ್ತ;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      ಯದಿ (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_available_controllers (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *controllers = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *buf = ರಿಕ್ತ;
  ವರ್ಣ *saveptr = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *token;
  ಪೂರ್ಣಾಙ್ಕ available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ಬೇರು, path, "cgroup.controllers", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = read_all_file (controllers, &buf, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತಿ (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (ರಿಕ್ತ, " \n", &saveptr))
    {
      ಯದಿ (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  ಪ್ರತ್ಯಾಗಮ available;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_controller_file (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *subtree_control = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *controllers = ರಿಕ್ತ;
  size_t controllers_len = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ಬೇರು, path, "cgroup.subtree_control", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      cleanup_free ವರ್ಣ *controllers_copy = xmalloc (controllers_len + 1);
      ಪೂರ್ಣಾಙ್ಕ attempts_ಎಡ;
      ವರ್ಣ *saveptr = ರಿಕ್ತ;
      ಧ್ರುವ ವರ್ಣ *token;
      ಪೂರ್ಣಾಙ್ಕ e;

      e = crun_error_get_errno (err);
      ಯದಿ (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      ಯದಿ (e == ENOENT)
        {
          libcrun_error_t tmp_err = ರಿಕ್ತ;
          ಪೂರ್ಣಾಙ್ಕ exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          ಯದಿ (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              ಪ್ರತ್ಯಾಗಮ ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          ಯದಿ (exists == 0)
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      crun_error_release (err);

      ಯದಿ (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      ಪ್ರತಿ (attempts_ಎಡ = 1000; attempts_ಎಡ >= 0; attempts_ಎಡ--)
        {
          ಪೂರ್ಣಾಙ್ಕ controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          ಪ್ರತಿ (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (ರಿಕ್ತ, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              ಯದಿ (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  ಯದಿ (e == EBUSY)
                    repeat = true;

                  ಅನುವರ್ತ;
                }

              controllers_written++;
            }

          ಯದಿ (! repeat)
            ಭಙ್ಗ;

          /* If there was any controller written, just try once more without any delay.  */
          ಯದಿ (controllers_written > 0 && attempts_ಎಡ > 2)
            {
              attempts_ಎಡ = 1;
              ಅನುವರ್ತ;
            }

          ಯದಿ (attempts_ಎಡ > 0)
            {
              ಸಂರಚನಾ timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, ರಿಕ್ತ);
            }
        }

      /* Refresh what controllers are available.  */
      ಪ್ರತ್ಯಾಗಮ read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  ಪ್ರತ್ಯಾಗಮ controllers_to_enable;
}

ಪೂರ್ಣಾಙ್ಕ
enable_controllers (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *tmp_path = ರಿಕ್ತ;
  ವರ್ಣ *it;
  ಪೂರ್ಣಾಙ್ಕ ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      ಯದಿ ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತಿ (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
      ವರ್ಣ *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, tmp_path, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = mkdir (cgroup_path, 0755);
      ಯದಿ (UNLIKELY (ret < 0 && errno != EEXIST))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "create `%s`", cgroup_path);

      ಯದಿ (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      *it = '/';
      it = next_slash;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ಧ್ರುವ ವರ್ಣ *path, bool create_if_missing, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (path == ರಿಕ್ತ || *path == '\0')
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
ಪೂರ್ಣಾಙ್ಕ
libcrun_move_self_to_cgroups (ಧ್ರುವ ವರ್ಣ *content, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *buf = xstrdup (content);
  ವರ್ಣ *saveptr = ರಿಕ್ತ;
  ವರ್ಣ *controller;
  ವರ್ಣ *path;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  bool has_data;
  ಪೂರ್ಣಾಙ್ಕ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಪ್ರತಿ (has_data = read_proc_cgroup (buf, &saveptr, ರಿಕ್ತ, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (ರಿಕ್ತ, &saveptr, ರಿಕ್ತ, &controller, &path))
    {
      ಧ್ರುವ ವರ್ಣ *subsystem = ರಿಕ್ತ;

      ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          ಯದಿ (controller[0] != '\0')
            ಅನುವರ್ತ;
        }
      ಅನ್ಯಥಾ
        {
          cleanup_free ವರ್ಣ *subsystem_path = ರಿಕ್ತ;

          /* Use the same names as enter_cgroup_v1 does.  */
          ಯದಿ (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          ಯದಿ (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          ಯದಿ (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ಬೇರು, subsystem, ರಿಕ್ತ);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          ret = crun_path_exists (subsystem_path, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          ಯದಿ (ret == 0)
            ಅನುವರ್ತ;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_get_cgroup_dirfd (ಸಂರಚನಾ libcrun_cgroup_status *status, ಧ್ರುವ ವರ್ಣ *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *path_to_cgroup = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ cgroupdirfd;
  ಪೂರ್ಣಾಙ್ಕ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (cgroup_mode < 0)
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (cgroup_mode != CGROUP_MODE_UNIFIED)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  ಯದಿ (status == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error");

  ಯದಿ (is_empty_string (status->path))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ಬೇರು, status->path, sub_cgroup, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  ಯದಿ (UNLIKELY (cgroupdirfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  ಪ್ರತ್ಯಾಗಮ cgroupdirfd;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, ವರ್ಣ *from, ವರ್ಣ *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ child_dfd = -1;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  size_t from_len;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (cgroup_mode < 0)
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ thaw;

  from_len = strlen (from);

  ಪ್ರತಿ (i = 0; pids && pids[i]; i++)
    {
      cleanup_free ವರ್ಣ *pid_path = ರಿಕ್ತ;
      cleanup_free ವರ್ಣ *dest_cgroup = ರಿಕ್ತ;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      ಯದಿ (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          ಗಚ್ಛ thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ thaw;
    }

  ret = 0;

thaw:
  ಯದಿ (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = ರಿಕ್ತ;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      ಯದಿ (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ destroy_cgroup_path (from, cgroup_mode, err);
}

ಪೂರ್ಣಾಙ್ಕ
get_cgroup_dirfd_path (ಪೂರ್ಣಾಙ್ಕ dirfd, ವರ್ಣ **path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  ಯದಿ (UNLIKELY (len < 0))
    ಪ್ರತ್ಯಾಗಮ len;

  ಯದಿ (has_prefix (cgroup_path, CGROUP_ಬೇರು))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ಬೇರು));
      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
ಪೂರ್ಣಾಙ್ಕ
parse_cpu_max (ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *it = ಮೌಲ್ಯ;
  ವರ್ಣ *endptr = ರಿಕ್ತ;

  ಯಾವತ್ (*it == ' ')
    it++;

  errno = 0;
  ಯದಿ (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (ವರ್ಣ *) it + 3;
    }
  ಅನ್ಯಥಾ
    {
      *quota = strtoll (it, &endptr, 10);
      ಯದಿ (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", ಮೌಲ್ಯ);
    }

  it = endptr;
  ಯಾವತ್ (*it == ' ')
    it++;

  *has_period = false;
  ಯದಿ (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      ಯದಿ (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", ಮೌಲ್ಯ);
      *has_period = true;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}
