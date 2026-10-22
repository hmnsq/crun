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

#include <config.h>
#include "cgroup.h"
#include "cgroup-internal.h"
#include "cgroup-systemd.h"
#include "cgroup-utils.h"
#include "cgroup-setup.h"
#include "ebpf.h"
#include "utils.h"
#include "status.h"
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/vfs.h>
#include <inttypes.h>
#include <time.h>
#include <poll.h>
#include <sys/inotify.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <libgen.h>

sa_nracanaa symlink_s
{
  const var_na *name;
  const var_na *target;
};

static sa_nracanaa symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { NULL, NULL } };

ຈຳນວນເຕັມ
libcrun_cgroups_create_symlinks (ຈຳນວນເຕັມ dirfd, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ i;

  ສຳລັບ (i = 0; cgroup_symlinks[i].name; i++)
    {
      ຈຳນວນເຕັມ ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          ຖ້າ (errno == ENOENT || errno == EEXIST)
            continue;
          ສົ່ງຄືນ crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
maybe_make_cgroup_threaded (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path_type = NULL;
  const var_na *const threaded = "threaded";
  cleanup_free var_na *content = NULL;
  cleanup_free var_na *buffer = NULL;
  const var_na *parent;
  size_t size;
  var_na *it;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (path == NULL)
    ສົ່ງຄືນ 0;

  path = consume_slashes (path);

  ຖ້າ (path[0] == '\0')
    ສົ່ງຄືນ 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ROOT, path, "cgroup.type", NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (size > 0)
    {
      it = content + size - 1;
      ໃນຂະນະທີ່ (*it == '\n' && it > content)
        *it-- = '\0';
    }

  ຖ້າ (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    ສົ່ງຄືນ 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  ຖ້າ (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      ຖ້າ (ret < 0)
        ສົ່ງຄືນ ret;
    }
  ສົ່ງຄືນ write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

ຈຳນວນເຕັມ
move_process_to_cgroup (pid_t pid, const var_na *subsystem, const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path_procs = NULL;
  var_na pid_str[16];
  ຈຳນວນເຕັມ ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ROOT,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = snprintf (pid_str, parimaa_na (pid_str), "%d", pid);
  ຖ້າ (UNLIKELY (ret >= (ຈຳນວນເຕັມ) parimaa_na (pid_str)))
    ສົ່ງຄືນ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      ຖ້າ (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = NULL;
          ຈຳນວນເຕັມ mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          ຖ້າ (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              ສົ່ງຄືນ ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          ສົ່ງຄືນ write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      ສົ່ງຄືນ ret;
    }
  ສົ່ງຄືນ ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

static ຈຳນວນເຕັມ
detect_cgroup_mode (libcrun_error_t *err)
{
  sa_nracanaa statfs stat;
  ຈຳນວນເຕັມ ret;

  ret = statfs (CGROUP_ROOT, &stat);
  ຖ້າ (ret < 0)
    ສົ່ງຄືນ crun_make_error (err, errno, "statfs `" CGROUP_ROOT "`");
  ຖ້າ (stat.f_type == CGROUP2_SUPER_MAGIC)
    ສົ່ງຄືນ CGROUP_MODE_UNIFIED;
  ຖ້າ (stat.f_type != TMPFS_MAGIC)
    ສົ່ງຄືນ crun_make_error (err, 0, "invalid file system type on `" CGROUP_ROOT "`");
  ret = statfs (CGROUP_ROOT "/unified", &stat);
  ຖ້າ (ret < 0 && errno != ENOENT)
    ສົ່ງຄືນ crun_make_error (err, errno, "statfs `" CGROUP_ROOT "/unified`");
  ຖ້າ (ret < 0)
    ສົ່ງຄືນ CGROUP_MODE_LEGACY;
  ສົ່ງຄືນ stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

ຈຳນວນເຕັມ
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  ຈຳນວນເຕັມ tmp;
  static ຈຳນວນເຕັມ cgroup_mode = 0;

  ຖ້າ (cgroup_mode)
    ສົ່ງຄືນ cgroup_mode;

  tmp = detect_cgroup_mode (err);
  ຖ້າ (UNLIKELY (tmp < 0))
    ສົ່ງຄືນ tmp;

  cgroup_mode = tmp;

  ສົ່ງຄືນ cgroup_mode;
}

static ຈຳນວນເຕັມ
get_cgroup_process_from_content (var_na *content, ຈຳນວນເຕັມ cgroup_mode, var_na **path, bool absolute, libcrun_error_t *err)
{
  var_na *cg_path = NULL;
  var_na *controller;
  var_na *saveptr;
  bool has_data;

  ສຳລັບ (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &cg_path))
    {
      ຖ້າ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          ຖ້າ (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            goto found;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          ຖ້າ (strcmp (controller, "memory"))
            goto found;
        }
    }

  ສົ່ງຄືນ crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  ຖ້າ (absolute)
    ສົ່ງຄືນ append_paths (path, err, CGROUP_ROOT, cg_path, NULL);

  *path = xstrdup (cg_path);
  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_get_cgroup_process_at (ຈຳນວນເຕັມ dirfd, var_na **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  size_t content_size;
  ຈຳນວນເຕັມ cgroup_mode;
  ຈຳນວນເຕັມ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (UNLIKELY (cgroup_mode < 0))
    ສົ່ງຄືນ cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

ຈຳນວນເຕັມ
libcrun_get_cgroup_process (pid_t pid, var_na **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  var_na proc_cgroup_file[64];
  size_t content_size;
  ຈຳນວນເຕັມ cgroup_mode;
  ຈຳນວນເຕັມ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (UNLIKELY (cgroup_mode < 0))
    ສົ່ງຄືນ cgroup_mode;

  ຖ້າ (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  ບໍ່ດັ່ງນັ້ນ
    {
      ຈຳນວນເຕັມ len = snprintf (proc_cgroup_file, parimaa_na (proc_cgroup_file), "/proc/%d/cgroup", pid);
      ຖ້າ (UNLIKELY (len >= (ຈຳນວນເຕັມ) parimaa_na (proc_cgroup_file)))
        ສົ່ງຄືນ crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

static ຈຳນວນເຕັມ
read_pids_file (ຈຳນວນເຕັມ dfd, const var_na *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ tasksfd = -1;
  cleanup_free var_na *buffer = NULL;
  var_na *saveptr = NULL;
  size_t n_new_pids;
  size_t len;
  var_na *it;
  ຈຳນວນເຕັມ ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  ຖ້າ (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      ຖ້າ (errno == ENOENT || errno == ENODEV)
        ສົ່ງຄືນ 0;
      ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (len > 0)
    {
      ສຳລັບ (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      ຖ້າ (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, parimaa_na (pid_t) * *allocated);
        }

      ສຳລັບ (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (NULL, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, NULL, 10);

          ຖ້າ (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  ສົ່ງຄືນ 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
sa_nracanaa cgroup_walk_dir_s
{
  var_na *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
sa_nracanaa cgroup_walk_s
{
  ຈຳນວນເຕັມ dfd;
  size_t depth;
  sa_nracanaa cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

static void
cgroup_walk_pop (sa_nracanaa cgroup_walk_s *walk)
{
  free (walk->dirs[--walk->n_dirs].name);
}

static void
cleanup_cgroup_walkp (sa_nracanaa cgroup_walk_s *walk)
{
  ຈຳນວນເຕັມ saved_errno = errno;

  ໃນຂະນະທີ່ (walk->n_dirs > 0)
    cgroup_walk_pop (walk);
  free (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
static ຈຳນວນເຕັມ
cgroup_walk_enter (sa_nracanaa cgroup_walk_s *walk, ຈຳນວນເຕັມ dfd)
{
  cleanup_close ຈຳນວນເຕັມ dfd_clone = -1;
  cleanup_dir DIR *dir = NULL;
  sa_nracanaa dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  ຖ້າ (UNLIKELY (dfd_clone < 0))
    ສົ່ງຄືນ -1;

  dir = fdopendir (dfd_clone);
  ຖ້າ (UNLIKELY (dir == NULL))
    ສົ່ງຄືນ -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  ສຳລັບ (de = readdir (dir); de; de = readdir (dir))
    {
      sa_nracanaa cgroup_walk_dir_s *d;

      ຖ້າ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      ຖ້າ (de->d_type != DT_DIR)
        continue;

      ຖ້າ (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, parimaa_na (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  ສົ່ງຄືນ 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
static ຈຳນວນເຕັມ
cgroup_walk_next (sa_nracanaa cgroup_walk_s *walk, sa_nracanaa cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : NULL;

  ໃນຂະນະທີ່ (*d && walk->depth >= (*d)->depth)
    {
      ຈຳນວນເຕັມ parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      ຖ້າ (UNLIKELY (parent < 0))
        ສົ່ງຄືນ -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
read_pids_cgroup_file (ຈຳນວນເຕັມ dfd, const var_na *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk sa_nracanaa cgroup_walk_s walk = { .dfd = -1 };
  ຈຳນວນເຕັມ dfd_clone;
  ຈຳນວນເຕັມ ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (! recurse)
    ສົ່ງຄືນ 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ຖ້າ (UNLIKELY (dfd_clone < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open cgroup sub-directory");

  ສຳລັບ (;;)
    {
      sa_nracanaa cgroup_walk_dir_s *d;
      cleanup_close ຈຳນວນເຕັມ nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "open parent cgroup directory");
      ຖ້າ (d == NULL)
        bha_nga;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ຖ້າ (UNLIKELY (nfd < 0))
        {
          ຈຳນວນເຕັມ saved_errno = errno;

          ຖ້າ (saved_errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          ສົ່ງຄືນ crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_pop (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          ຈຳນວນເຕັມ e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          ຖ້າ (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            ສົ່ງຄືນ ret;
          crun_error_release (err);
          continue;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      ຖ້າ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ສົ່ງຄືນ crun_make_error (err, errno, "open cgroup sub-directory");
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
read_pids_cgroup (ຈຳນວນເຕັມ dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  ຖ້າ (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ
rmdir_all_fd (ຈຳນວນເຕັມ dfd)
{
  cleanup_cgroup_walk sa_nracanaa cgroup_walk_s walk = { .dfd = -1 };
  ຈຳນວນເຕັມ ret;

  ret = cgroup_walk_enter (&walk, dfd);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສຳລັບ (;;)
    {
      sa_nracanaa cgroup_walk_dir_s *d;
      ຈຳນວນເຕັມ child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
      ຖ້າ (d == NULL)
        bha_nga;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      ຖ້າ (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_pop (&walk);
          continue;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      ຖ້າ (UNLIKELY (child_dfd < 0))
        {
          ຖ້າ (errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          ສົ່ງຄືນ child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      ຖ້າ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
rmdir_all (const var_na *path)
{
  ຈຳນວນເຕັມ ret;
  ຈຳນວນເຕັມ dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  ຖ້າ (UNLIKELY (dfd < 0))
    ສົ່ງຄືນ dfd;

  ret = rmdir_all_fd (dfd);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ rmdir (path);
}

static ຈຳນວນເຕັມ
read_pids_from_path (const var_na *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_close ຈຳນວນເຕັມ dirfd = -1;
  size_t n_pids, allocated;
  ຈຳນວນເຕັມ mode;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (path == NULL || *path == '\0')
    ສົ່ງຄືນ 0;

  mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (UNLIKELY (mode < 0))
    ສົ່ງຄືນ mode;

  switch (mode)
    {
    pak_sa CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
      bha_nga;

    pak_sa CGROUP_MODE_HYBRID:
    pak_sa CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT "/memory", path, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
      bha_nga;

    default:
      ສົ່ງຄືນ crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ຖ້າ (dirfd < 0)
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  ຖ້າ (threads && mode == CGROUP_MODE_UNIFIED)
    ສົ່ງຄືນ read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  ສົ່ງຄືນ read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

ຈຳນວນເຕັມ
libcrun_cgroup_read_pids_from_path (const var_na *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ສົ່ງຄືນ read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (var_na *content, var_na **saveptr, var_na **id, var_na **controller_list, var_na **path)
{
  var_na *it;

  it = strtok_r (content, "\n", saveptr);
  ຖ້າ (it == NULL)
    ສົ່ງຄືນ false;

  ຖ້າ (id)
    *id = it;

  it = strchr (it, ':');
  ຖ້າ (it == NULL)
    ສົ່ງຄືນ false;
  *it++ = '\0';

  ຖ້າ (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  ຖ້າ (it == NULL)
    ສົ່ງຄືນ false;
  *it++ = '\0';

  ຖ້າ (path)
    *path = it;

  ສົ່ງຄືນ true;
}

ຈຳນວນເຕັມ
destroy_cgroup_path (const var_na *path, ຈຳນວນເຕັມ mode, libcrun_error_t *err)
{
  bool repeat = true;
  ຈຳນວນເຕັມ retry_count = 0;
  const ຈຳນວນເຕັມ max_attempts = 500;
  ຈຳນວນເຕັມ ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  ຖ້າ (is_empty_string (path))
    ສົ່ງຄືນ 0;

  do
    {
      repeat = false;

      ຖ້າ (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free var_na *cgroup_path = NULL;

          ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
          ret = rmdir (cgroup_path);
          ຖ້າ (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              ຖ້າ (ret < 0)
                {
                  ຖ້າ (retry_count >= max_attempts)
                    ສົ່ງຄືນ crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_count++;
                  repeat = true;
                }
            }
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          cleanup_free var_na *content = NULL;
          size_t content_size;
          var_na *controller;
          var_na *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          ຖ້າ (UNLIKELY (ret < 0))
            {
              ຖ້າ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  ສົ່ງຄືນ 0;
                }
              ສົ່ງຄືນ ret;
            }

          ສຳລັບ (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
               has_data;
               has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
            {
              cleanup_free var_na *cgroup_path = NULL;
              var_na *subsystem;
              ຖ້າ (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              ຖ້າ (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                continue;

              ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path, NULL);
              ຖ້າ (UNLIKELY (ret < 0))
                ສົ່ງຄືນ ret;

              ret = rmdir (cgroup_path);
              ຖ້າ (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  ຖ້າ (ret < 0)
                    {
                      ຖ້າ (retry_count >= max_attempts)
                        ສົ່ງຄືນ crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_count++;
                      repeat = true;
                    }
                }
            }
        }

      ຖ້າ (repeat)
        {
          sa_nracanaa timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, NULL);

          ret = cgroup_killall_path (path, SIGKILL, err);
          ຖ້າ (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } ໃນຂະນະທີ່ (repeat);

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
chown_cgroups (const var_na *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_free var_na *delegate = NULL;
  cleanup_close ຈຳນວນເຕັມ dfd = -1;
  size_t delegate_size;
  var_na *saveptr = NULL;
  var_na *name;
  ຈຳນວນເຕັມ ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  ຖ້າ (UNLIKELY (dfd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      ຖ້າ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ສົ່ງຄືນ 0;
        }
      ສົ່ງຄືນ ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  ສຳລັບ (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (NULL, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          ຖ້າ (errno == ENOENT)
            continue;

          ສົ່ງຄືນ crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
libcrun_cgroup_pause_unpause_with_mode (const var_na *cgroup_path, ຈຳນວນເຕັມ cgroup_mode, const bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free var_na *path = NULL;
  const var_na *state = "";
  ຈຳນວນເຕັມ ret;

  ຖ້າ (cgroup_path == NULL || cgroup_path[0] == '\0')
    ສົ່ງຄືນ crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  ຖ້າ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  ຖ້າ (ret >= 0)
    ສົ່ງຄືນ 0;
  ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ
libcrun_cgroup_pause_unpause_path (const var_na *cgroup_path, const bool pause, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (UNLIKELY (cgroup_mode < 0))
    ສົ່ງຄືນ cgroup_mode;

  ສົ່ງຄືນ libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
ຈຳນວນເຕັມ
libcrun_cgroup_ensure_not_frozen (const var_na *cgroup_path, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  cleanup_free var_na *path = NULL;
  const var_na *frozen;
  ຈຳນວນເຕັມ cgroup_mode;
  ຈຳນວນເຕັມ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (UNLIKELY (cgroup_mode < 0))
    ສົ່ງຄືນ cgroup_mode;

  ຖ້າ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
    }
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = read_all_file (path, &content, NULL, err);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      ຖ້າ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ສົ່ງຄືນ 0;
        }
      ສົ່ງຄືນ ret;
    }

  ຖ້າ (strstr (content, frozen))
    ສົ່ງຄືນ crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  ສົ່ງຄືນ 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
static ຈຳນວນເຕັມ
watch_cgroup_events (const var_na *path)
{
  cleanup_free var_na *events_file = NULL;
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  libcrun_error_t tmp_err = NULL;
  ຈຳນວນເຕັມ ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ສົ່ງຄືນ -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ -1;

  ret = fd;
  fd = -1;
  ສົ່ງຄືນ ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
static ຈຳນວນເຕັມ
wait_cgroup_unpopulated (const var_na *path, ຈຳນວນເຕັມ inotify_fd)
{
  cleanup_free var_na *events_file = NULL;
  libcrun_error_t tmp_err = NULL;
  sa_nracanaa timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  ຈຳນວນເຕັມ ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ສົ່ງຄືນ 0;
    }

  ສຳລັບ (;;)
    {
      cleanup_free var_na *content = NULL;
      var_na buf[4096];
      sa_nracanaa pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, NULL, &tmp_err);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          ສົ່ງຄືນ 0;
        }

      ຖ້າ (strstr (content, "populated 0"))
        ສົ່ງຄືນ 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, NULL));
      ຖ້າ (ret <= 0)
        ສົ່ງຄືນ 0;

      ໃນຂະນະທີ່ (read (inotify_fd, buf, parimaa_na (buf)) > 0)
        ;
    }
}

ຈຳນວນເຕັມ
cgroup_killall_path (const var_na *path, ຈຳນວນເຕັມ sa_nkeeta, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close ຈຳນວນເຕັມ inotify_fd = -1;
  ຈຳນວນເຕັມ ret;
  size_t i;
  cleanup_free pid_t *pids = NULL;

  ຖ້າ (path == NULL || *path == '\0')
    ສົ່ງຄືນ 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  ຖ້າ (sa_nkeeta == SIGKILL)
    {
      cleanup_free var_na *kill_file = NULL;

      ret = append_paths (&kill_file, err, CGROUP_ROOT, path, "cgroup.kill", NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      ຖ້າ (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        ສົ່ງຄືນ 0;

      ຖ້າ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  ຖ້າ (UNLIKELY (ret < 0))
    crun_error_release (err);
  ບໍ່ດັ່ງນັ້ນ
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, sa_nkeeta == SIGKILL, &pids, err);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      ຖ້າ (crun_error_get_errno (err) != ENOENT)
        goto thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  ສຳລັບ (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], sa_nkeeta);
      ຖ້າ (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          goto thaw;
        }
    }

  ret = 0;

thaw:
  ຖ້າ (frozen)
    {
      libcrun_error_t thaw_err = NULL;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      ຖ້າ (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ
read_available_controllers (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *controllers = NULL;
  cleanup_free var_na *buf = NULL;
  var_na *saveptr = NULL;
  const var_na *token;
  ຈຳນວນເຕັມ available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ROOT, path, "cgroup.controllers", NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = read_all_file (controllers, &buf, NULL, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສຳລັບ (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
    {
      ຖ້າ (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  ສົ່ງຄືນ available;
}

static ຈຳນວນເຕັມ
write_controller_file (const var_na *path, ຈຳນວນເຕັມ controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free var_na *subtree_control = NULL;
  cleanup_free var_na *controllers = NULL;
  size_t controllers_len = 0;
  ຈຳນວນເຕັມ ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ROOT, path, "cgroup.subtree_control", NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      cleanup_free var_na *controllers_copy = xmalloc (controllers_len + 1);
      ຈຳນວນເຕັມ attempts_left;
      var_na *saveptr = NULL;
      const var_na *token;
      ຈຳນວນເຕັມ e;

      e = crun_error_get_errno (err);
      ຖ້າ (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        ສົ່ງຄືນ crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      ຖ້າ (e == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;
          ຈຳນວນເຕັມ exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          ຖ້າ (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              ສົ່ງຄືນ ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          ຖ້າ (exists == 0)
            ສົ່ງຄືນ ret;
        }

      crun_error_release (err);

      ຖ້າ (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      ສຳລັບ (attempts_left = 1000; attempts_left >= 0; attempts_left--)
        {
          ຈຳນວນເຕັມ controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          ສຳລັບ (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (NULL, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              ຖ້າ (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  ຖ້າ (e == EBUSY)
                    repeat = true;

                  continue;
                }

              controllers_written++;
            }

          ຖ້າ (! repeat)
            bha_nga;

          /* If there was any controller written, just try once more without any delay.  */
          ຖ້າ (controllers_written > 0 && attempts_left > 2)
            {
              attempts_left = 1;
              continue;
            }

          ຖ້າ (attempts_left > 0)
            {
              sa_nracanaa timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, NULL);
            }
        }

      /* Refresh what controllers are available.  */
      ສົ່ງຄືນ read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  ສົ່ງຄືນ controllers_to_enable;
}

ຈຳນວນເຕັມ
enable_controllers (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *tmp_path = NULL;
  var_na *it;
  ຈຳນວນເຕັມ ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      ຖ້າ ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສຳລັບ (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free var_na *cgroup_path = NULL;
      var_na *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, tmp_path, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = mkdir (cgroup_path, 0755);
      ຖ້າ (UNLIKELY (ret < 0 && errno != EEXIST))
        ສົ່ງຄືນ crun_make_error (err, errno, "create `%s`", cgroup_path);

      ຖ້າ (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }

      *it = '/';
      it = next_slash;
    }
  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, const var_na *path, bool create_if_missing, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ cgroup_mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (UNLIKELY (cgroup_mode < 0))
    ສົ່ງຄືນ cgroup_mode;

  ຖ້າ (path == NULL || *path == '\0')
    ສົ່ງຄືນ 0;

  ສົ່ງຄືນ enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
ຈຳນວນເຕັມ
libcrun_move_self_to_cgroups (const var_na *content, libcrun_error_t *err)
{
  cleanup_free var_na *buf = xstrdup (content);
  var_na *saveptr = NULL;
  var_na *controller;
  var_na *path;
  ຈຳນວນເຕັມ cgroup_mode;
  bool has_data;
  ຈຳນວນເຕັມ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (UNLIKELY (cgroup_mode < 0))
    ສົ່ງຄືນ cgroup_mode;

  ສຳລັບ (has_data = read_proc_cgroup (buf, &saveptr, NULL, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &path))
    {
      const var_na *subsystem = NULL;

      ຖ້າ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          ຖ້າ (controller[0] != '\0')
            continue;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          cleanup_free var_na *subsystem_path = NULL;

          /* Use the same names as enter_cgroup_v1 does.  */
          ຖ້າ (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          ຖ້າ (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          ຖ້າ (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ROOT, subsystem, NULL);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
          ret = crun_path_exists (subsystem_path, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
          ຖ້າ (ret == 0)
            continue;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_get_cgroup_dirfd (sa_nracanaa libcrun_cgroup_status *status, const var_na *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free var_na *path_to_cgroup = NULL;
  ຈຳນວນເຕັມ cgroup_mode;
  ຈຳນວນເຕັມ cgroupdirfd;
  ຈຳນວນເຕັມ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (cgroup_mode < 0)
    ສົ່ງຄືນ cgroup_mode;

  ຖ້າ (cgroup_mode != CGROUP_MODE_UNIFIED)
    ສົ່ງຄືນ crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  ຖ້າ (status == NULL)
    ສົ່ງຄືນ crun_make_error (err, 0, "internal error");

  ຖ້າ (is_empty_string (status->path))
    ສົ່ງຄືນ crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ROOT, status->path, sub_cgroup, NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  ຖ້າ (UNLIKELY (cgroupdirfd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  ສົ່ງຄືນ cgroupdirfd;
}

ຈຳນວນເຕັມ
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, var_na *from, var_na *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = NULL;
  cleanup_close ຈຳນວນເຕັມ child_dfd = -1;
  ຈຳນວນເຕັມ cgroup_mode;
  size_t from_len;
  size_t i;
  ຈຳນວນເຕັມ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (cgroup_mode < 0)
    ສົ່ງຄືນ cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  ຖ້າ (UNLIKELY (ret < 0))
    goto thaw;

  from_len = strlen (from);

  ສຳລັບ (i = 0; pids && pids[i]; i++)
    {
      cleanup_free var_na *pid_path = NULL;
      cleanup_free var_na *dest_cgroup = NULL;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      ຖ້າ (UNLIKELY (ret < 0))
        goto thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      ຖ້າ (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          goto thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      ຖ້າ (UNLIKELY (ret < 0))
        goto thaw;
    }

  ret = 0;

thaw:
  ຖ້າ (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = NULL;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      ຖ້າ (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      ສົ່ງຄືນ ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ destroy_cgroup_path (from, cgroup_mode, err);
}

ຈຳນວນເຕັມ
get_cgroup_dirfd_path (ຈຳນວນເຕັມ dirfd, var_na **path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  ຖ້າ (UNLIKELY (len < 0))
    ສົ່ງຄືນ len;

  ຖ້າ (has_prefix (cgroup_path, CGROUP_ROOT))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ROOT));
      ສົ່ງຄືນ 0;
    }
  ສົ່ງຄືນ crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
ຈຳນວນເຕັມ
parse_cpu_max (const var_na *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  const var_na *it = value;
  var_na *endptr = NULL;

  ໃນຂະນະທີ່ (*it == ' ')
    it++;

  errno = 0;
  ຖ້າ (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (var_na *) it + 3;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      *quota = strtoll (it, &endptr, 10);
      ຖ້າ (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        ສົ່ງຄືນ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
    }

  it = endptr;
  ໃນຂະນະທີ່ (*it == ' ')
    it++;

  *has_period = false;
  ຖ້າ (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      ຖ້າ (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        ສົ່ງຄືນ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
      *has_period = true;
    }

  ສົ່ງຄືນ 0;
}
