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

ကိန်းပြည့်
libcrun_cgroups_create_symlinks (ကိန်းပြည့် dirfd, libcrun_error_t *err)
{
  ကိန်းပြည့် i;

  အတွက် (i = 0; cgroup_symlinks[i].name; i++)
    {
      ကိန်းပြည့် ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      အကယ်၍ (UNLIKELY (ret < 0))
        {
          အကယ်၍ (errno == ENOENT || errno == EEXIST)
            continue;
          ပြန်ပေး crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  ပြန်ပေး 0;
}

ကိန်းပြည့်
maybe_make_cgroup_threaded (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path_type = NULL;
  const var_na *const threaded = "threaded";
  cleanup_free var_na *content = NULL;
  cleanup_free var_na *buffer = NULL;
  const var_na *parent;
  size_t size;
  var_na *it;
  ကိန်းပြည့် ret;

  အကယ်၍ (path == NULL)
    ပြန်ပေး 0;

  path = consume_slashes (path);

  အကယ်၍ (path[0] == '\0')
    ပြန်ပေး 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ROOT, path, "cgroup.type", NULL);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  အကယ်၍ (size > 0)
    {
      it = content + size - 1;
      နေစဉ် (*it == '\n' && it > content)
        *it-- = '\0';
    }

  အကယ်၍ (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    ပြန်ပေး 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  အကယ်၍ (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      အကယ်၍ (ret < 0)
        ပြန်ပေး ret;
    }
  ပြန်ပေး write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

ကိန်းပြည့်
move_process_to_cgroup (pid_t pid, const var_na *subsystem, const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path_procs = NULL;
  var_na pid_str[16];
  ကိန်းပြည့် ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ROOT,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", NULL);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ret = snprintf (pid_str, parimaa_na (pid_str), "%d", pid);
  အကယ်၍ (UNLIKELY (ret >= (ကိန်းပြည့်) parimaa_na (pid_str)))
    ပြန်ပေး crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  အကယ်၍ (UNLIKELY (ret < 0))
    {
      အကယ်၍ (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = NULL;
          ကိန်းပြည့် mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          အကယ်၍ (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              ပြန်ပေး ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          အကယ်၍ (UNLIKELY (ret < 0))
            ပြန်ပေး ret;

          ပြန်ပေး write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      ပြန်ပေး ret;
    }
  ပြန်ပေး ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

static ကိန်းပြည့်
detect_cgroup_mode (libcrun_error_t *err)
{
  sa_nracanaa statfs stat;
  ကိန်းပြည့် ret;

  ret = statfs (CGROUP_ROOT, &stat);
  အကယ်၍ (ret < 0)
    ပြန်ပေး crun_make_error (err, errno, "statfs `" CGROUP_ROOT "`");
  အကယ်၍ (stat.f_type == CGROUP2_SUPER_MAGIC)
    ပြန်ပေး CGROUP_MODE_UNIFIED;
  အကယ်၍ (stat.f_type != TMPFS_MAGIC)
    ပြန်ပေး crun_make_error (err, 0, "invalid file system type on `" CGROUP_ROOT "`");
  ret = statfs (CGROUP_ROOT "/unified", &stat);
  အကယ်၍ (ret < 0 && errno != ENOENT)
    ပြန်ပေး crun_make_error (err, errno, "statfs `" CGROUP_ROOT "/unified`");
  အကယ်၍ (ret < 0)
    ပြန်ပေး CGROUP_MODE_LEGACY;
  ပြန်ပေး stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

ကိန်းပြည့်
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  ကိန်းပြည့် tmp;
  static ကိန်းပြည့် cgroup_mode = 0;

  အကယ်၍ (cgroup_mode)
    ပြန်ပေး cgroup_mode;

  tmp = detect_cgroup_mode (err);
  အကယ်၍ (UNLIKELY (tmp < 0))
    ပြန်ပေး tmp;

  cgroup_mode = tmp;

  ပြန်ပေး cgroup_mode;
}

static ကိန်းပြည့်
get_cgroup_process_from_content (var_na *content, ကိန်းပြည့် cgroup_mode, var_na **path, bool absolute, libcrun_error_t *err)
{
  var_na *cg_path = NULL;
  var_na *controller;
  var_na *saveptr;
  bool has_data;

  အတွက် (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &cg_path))
    {
      အကယ်၍ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          အကယ်၍ (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            goto found;
        }
      မဟုတ်ပါက
        {
          အကယ်၍ (strcmp (controller, "memory"))
            goto found;
        }
    }

  ပြန်ပေး crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  အကယ်၍ (absolute)
    ပြန်ပေး append_paths (path, err, CGROUP_ROOT, cg_path, NULL);

  *path = xstrdup (cg_path);
  ပြန်ပေး 0;
}

ကိန်းပြည့်
libcrun_get_cgroup_process_at (ကိန်းပြည့် dirfd, var_na **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  size_t content_size;
  ကိန်းပြည့် cgroup_mode;
  ကိန်းပြည့် ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  အကယ်၍ (UNLIKELY (cgroup_mode < 0))
    ပြန်ပေး cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ပြန်ပေး get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

ကိန်းပြည့်
libcrun_get_cgroup_process (pid_t pid, var_na **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  var_na proc_cgroup_file[64];
  size_t content_size;
  ကိန်းပြည့် cgroup_mode;
  ကိန်းပြည့် ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  အကယ်၍ (UNLIKELY (cgroup_mode < 0))
    ပြန်ပေး cgroup_mode;

  အကယ်၍ (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  မဟုတ်ပါက
    {
      ကိန်းပြည့် len = snprintf (proc_cgroup_file, parimaa_na (proc_cgroup_file), "/proc/%d/cgroup", pid);
      အကယ်၍ (UNLIKELY (len >= (ကိန်းပြည့်) parimaa_na (proc_cgroup_file)))
        ပြန်ပေး crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ပြန်ပေး get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

static ကိန်းပြည့်
read_pids_file (ကိန်းပြည့် dfd, const var_na *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close ကိန်းပြည့် tasksfd = -1;
  cleanup_free var_na *buffer = NULL;
  var_na *saveptr = NULL;
  size_t n_new_pids;
  size_t len;
  var_na *it;
  ကိန်းပြည့် ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  အကယ်၍ (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      အကယ်၍ (errno == ENOENT || errno == ENODEV)
        ပြန်ပေး 0;
      ပြန်ပေး crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  အကယ်၍ (len > 0)
    {
      အတွက် (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      အကယ်၍ (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, parimaa_na (pid_t) * *allocated);
        }

      အတွက် (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (NULL, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, NULL, 10);

          အကယ်၍ (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  ပြန်ပေး 0;
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
  ကိန်းပြည့် dfd;
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
  ကိန်းပြည့် saved_errno = errno;

  နေစဉ် (walk->n_dirs > 0)
    cgroup_walk_pop (walk);
  free (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
static ကိန်းပြည့်
cgroup_walk_enter (sa_nracanaa cgroup_walk_s *walk, ကိန်းပြည့် dfd)
{
  cleanup_close ကိန်းပြည့် dfd_clone = -1;
  cleanup_dir DIR *dir = NULL;
  sa_nracanaa dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  အကယ်၍ (UNLIKELY (dfd_clone < 0))
    ပြန်ပေး -1;

  dir = fdopendir (dfd_clone);
  အကယ်၍ (UNLIKELY (dir == NULL))
    ပြန်ပေး -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  အတွက် (de = readdir (dir); de; de = readdir (dir))
    {
      sa_nracanaa cgroup_walk_dir_s *d;

      အကယ်၍ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      အကယ်၍ (de->d_type != DT_DIR)
        continue;

      အကယ်၍ (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, parimaa_na (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  ပြန်ပေး 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
static ကိန်းပြည့်
cgroup_walk_next (sa_nracanaa cgroup_walk_s *walk, sa_nracanaa cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : NULL;

  နေစဉ် (*d && walk->depth >= (*d)->depth)
    {
      ကိန်းပြည့် parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      အကယ်၍ (UNLIKELY (parent < 0))
        ပြန်ပေး -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  ပြန်ပေး 0;
}

static ကိန်းပြည့်
read_pids_cgroup_file (ကိန်းပြည့် dfd, const var_na *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk sa_nracanaa cgroup_walk_s walk = { .dfd = -1 };
  ကိန်းပြည့် dfd_clone;
  ကိန်းပြည့် ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  အကယ်၍ (! recurse)
    ပြန်ပေး 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  အကယ်၍ (UNLIKELY (dfd_clone < 0))
    ပြန်ပေး crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး crun_make_error (err, errno, "open cgroup sub-directory");

  အတွက် (;;)
    {
      sa_nracanaa cgroup_walk_dir_s *d;
      cleanup_close ကိန်းပြည့် nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး crun_make_error (err, errno, "open parent cgroup directory");
      အကယ်၍ (d == NULL)
        bha_nga;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      အကယ်၍ (UNLIKELY (nfd < 0))
        {
          ကိန်းပြည့် saved_errno = errno;

          အကယ်၍ (saved_errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          ပြန်ပေး crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_pop (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      အကယ်၍ (UNLIKELY (ret < 0))
        {
          ကိန်းပြည့် e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          အကယ်၍ (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            ပြန်ပေး ret;
          crun_error_release (err);
          continue;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      အကယ်၍ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ပြန်ပေး crun_make_error (err, errno, "open cgroup sub-directory");
    }

  ပြန်ပေး 0;
}

static ကိန်းပြည့်
read_pids_cgroup (ကိန်းပြည့် dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  ကိန်းပြည့် ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  အကယ်၍ (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  ပြန်ပေး ret;
}

static ကိန်းပြည့်
rmdir_all_fd (ကိန်းပြည့် dfd)
{
  cleanup_cgroup_walk sa_nracanaa cgroup_walk_s walk = { .dfd = -1 };
  ကိန်းပြည့် ret;

  ret = cgroup_walk_enter (&walk, dfd);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  အတွက် (;;)
    {
      sa_nracanaa cgroup_walk_dir_s *d;
      ကိန်းပြည့် child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;
      အကယ်၍ (d == NULL)
        bha_nga;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      အကယ်၍ (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_pop (&walk);
          continue;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      အကယ်၍ (UNLIKELY (child_dfd < 0))
        {
          အကယ်၍ (errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          ပြန်ပေး child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      အကယ်၍ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ပြန်ပေး ret;
    }

  ပြန်ပေး 0;
}

static ကိန်းပြည့်
rmdir_all (const var_na *path)
{
  ကိန်းပြည့် ret;
  ကိန်းပြည့် dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  အကယ်၍ (UNLIKELY (dfd < 0))
    ပြန်ပေး dfd;

  ret = rmdir_all_fd (dfd);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ပြန်ပေး rmdir (path);
}

static ကိန်းပြည့်
read_pids_from_path (const var_na *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_close ကိန်းပြည့် dirfd = -1;
  size_t n_pids, allocated;
  ကိန်းပြည့် mode;
  ကိန်းပြည့် ret;

  အကယ်၍ (path == NULL || *path == '\0')
    ပြန်ပေး 0;

  mode = libcrun_get_cgroup_mode (err);
  အကယ်၍ (UNLIKELY (mode < 0))
    ပြန်ပေး mode;

  switch (mode)
    {
    pak_sa CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;
      bha_nga;

    pak_sa CGROUP_MODE_HYBRID:
    pak_sa CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT "/memory", path, NULL);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;
      bha_nga;

    default:
      ပြန်ပေး crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  အကယ်၍ (dirfd < 0)
    ပြန်ပေး crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  အကယ်၍ (threads && mode == CGROUP_MODE_UNIFIED)
    ပြန်ပေး read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  ပြန်ပေး read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

ကိန်းပြည့်
libcrun_cgroup_read_pids_from_path (const var_na *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ပြန်ပေး read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (var_na *content, var_na **saveptr, var_na **id, var_na **controller_list, var_na **path)
{
  var_na *it;

  it = strtok_r (content, "\n", saveptr);
  အကယ်၍ (it == NULL)
    ပြန်ပေး false;

  အကယ်၍ (id)
    *id = it;

  it = strchr (it, ':');
  အကယ်၍ (it == NULL)
    ပြန်ပေး false;
  *it++ = '\0';

  အကယ်၍ (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  အကယ်၍ (it == NULL)
    ပြန်ပေး false;
  *it++ = '\0';

  အကယ်၍ (path)
    *path = it;

  ပြန်ပေး true;
}

ကိန်းပြည့်
destroy_cgroup_path (const var_na *path, ကိန်းပြည့် mode, libcrun_error_t *err)
{
  bool repeat = true;
  ကိန်းပြည့် retry_count = 0;
  const ကိန်းပြည့် max_attempts = 500;
  ကိန်းပြည့် ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  အကယ်၍ (is_empty_string (path))
    ပြန်ပေး 0;

  do
    {
      repeat = false;

      အကယ်၍ (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free var_na *cgroup_path = NULL;

          ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
          အကယ်၍ (UNLIKELY (ret < 0))
            ပြန်ပေး ret;
          ret = rmdir (cgroup_path);
          အကယ်၍ (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              အကယ်၍ (ret < 0)
                {
                  အကယ်၍ (retry_count >= max_attempts)
                    ပြန်ပေး crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_count++;
                  repeat = true;
                }
            }
        }
      မဟုတ်ပါက
        {
          cleanup_free var_na *content = NULL;
          size_t content_size;
          var_na *controller;
          var_na *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          အကယ်၍ (UNLIKELY (ret < 0))
            {
              အကယ်၍ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  ပြန်ပေး 0;
                }
              ပြန်ပေး ret;
            }

          အတွက် (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
               has_data;
               has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
            {
              cleanup_free var_na *cgroup_path = NULL;
              var_na *subsystem;
              အကယ်၍ (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              အကယ်၍ (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                continue;

              ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path, NULL);
              အကယ်၍ (UNLIKELY (ret < 0))
                ပြန်ပေး ret;

              ret = rmdir (cgroup_path);
              အကယ်၍ (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  အကယ်၍ (ret < 0)
                    {
                      အကယ်၍ (retry_count >= max_attempts)
                        ပြန်ပေး crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_count++;
                      repeat = true;
                    }
                }
            }
        }

      အကယ်၍ (repeat)
        {
          sa_nracanaa timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, NULL);

          ret = cgroup_killall_path (path, SIGKILL, err);
          အကယ်၍ (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } နေစဉ် (repeat);

  ပြန်ပေး 0;
}

ကိန်းပြည့်
chown_cgroups (const var_na *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_free var_na *delegate = NULL;
  cleanup_close ကိန်းပြည့် dfd = -1;
  size_t delegate_size;
  var_na *saveptr = NULL;
  var_na *name;
  ကိန်းပြည့် ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  အကယ်၍ (UNLIKELY (dfd < 0))
    ပြန်ပေး crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    {
      အကယ်၍ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ပြန်ပေး 0;
        }
      ပြန်ပေး ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  အတွက် (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (NULL, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      အကယ်၍ (UNLIKELY (ret < 0))
        {
          အကယ်၍ (errno == ENOENT)
            continue;

          ပြန်ပေး crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  ပြန်ပေး 0;
}

static ကိန်းပြည့်
libcrun_cgroup_pause_unpause_with_mode (const var_na *cgroup_path, ကိန်းပြည့် cgroup_mode, const bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free var_na *path = NULL;
  const var_na *state = "";
  ကိန်းပြည့် ret;

  အကယ်၍ (cgroup_path == NULL || cgroup_path[0] == '\0')
    ပြန်ပေး crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  အကယ်၍ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;
    }
  မဟုတ်ပါက
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  အကယ်၍ (ret >= 0)
    ပြန်ပေး 0;
  ပြန်ပေး ret;
}

ကိန်းပြည့်
libcrun_cgroup_pause_unpause_path (const var_na *cgroup_path, const bool pause, libcrun_error_t *err)
{
  ကိန်းပြည့် cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  အကယ်၍ (UNLIKELY (cgroup_mode < 0))
    ပြန်ပေး cgroup_mode;

  ပြန်ပေး libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
ကိန်းပြည့်
libcrun_cgroup_ensure_not_frozen (const var_na *cgroup_path, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  cleanup_free var_na *path = NULL;
  const var_na *frozen;
  ကိန်းပြည့် cgroup_mode;
  ကိန်းပြည့် ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  အကယ်၍ (UNLIKELY (cgroup_mode < 0))
    ပြန်ပေး cgroup_mode;

  အကယ်၍ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
    }
  မဟုတ်ပါက
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
    }
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ret = read_all_file (path, &content, NULL, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      အကယ်၍ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ပြန်ပေး 0;
        }
      ပြန်ပေး ret;
    }

  အကယ်၍ (strstr (content, frozen))
    ပြန်ပေး crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  ပြန်ပေး 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
static ကိန်းပြည့်
watch_cgroup_events (const var_na *path)
{
  cleanup_free var_na *events_file = NULL;
  cleanup_close ကိန်းပြည့် fd = -1;
  libcrun_error_t tmp_err = NULL;
  ကိန်းပြည့် ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  အကယ်၍ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ပြန်ပေး -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  အကယ်၍ (UNLIKELY (fd < 0))
    ပြန်ပေး -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး -1;

  ret = fd;
  fd = -1;
  ပြန်ပေး ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
static ကိန်းပြည့်
wait_cgroup_unpopulated (const var_na *path, ကိန်းပြည့် inotify_fd)
{
  cleanup_free var_na *events_file = NULL;
  libcrun_error_t tmp_err = NULL;
  sa_nracanaa timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  ကိန်းပြည့် ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  အကယ်၍ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ပြန်ပေး 0;
    }

  အတွက် (;;)
    {
      cleanup_free var_na *content = NULL;
      var_na buf[4096];
      sa_nracanaa pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, NULL, &tmp_err);
      အကယ်၍ (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          ပြန်ပေး 0;
        }

      အကယ်၍ (strstr (content, "populated 0"))
        ပြန်ပေး 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, NULL));
      အကယ်၍ (ret <= 0)
        ပြန်ပေး 0;

      နေစဉ် (read (inotify_fd, buf, parimaa_na (buf)) > 0)
        ;
    }
}

ကိန်းပြည့်
cgroup_killall_path (const var_na *path, ကိန်းပြည့် sa_nkeeta, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close ကိန်းပြည့် inotify_fd = -1;
  ကိန်းပြည့် ret;
  size_t i;
  cleanup_free pid_t *pids = NULL;

  အကယ်၍ (path == NULL || *path == '\0')
    ပြန်ပေး 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  အကယ်၍ (sa_nkeeta == SIGKILL)
    {
      cleanup_free var_na *kill_file = NULL;

      ret = append_paths (&kill_file, err, CGROUP_ROOT, path, "cgroup.kill", NULL);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      အကယ်၍ (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        ပြန်ပေး 0;

      အကယ်၍ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    crun_error_release (err);
  မဟုတ်ပါက
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, sa_nkeeta == SIGKILL, &pids, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    {
      အကယ်၍ (crun_error_get_errno (err) != ENOENT)
        goto thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  အတွက် (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], sa_nkeeta);
      အကယ်၍ (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          goto thaw;
        }
    }

  ret = 0;

thaw:
  အကယ်၍ (frozen)
    {
      libcrun_error_t thaw_err = NULL;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      အကယ်၍ (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  ပြန်ပေး ret;
}

static ကိန်းပြည့်
read_available_controllers (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *controllers = NULL;
  cleanup_free var_na *buf = NULL;
  var_na *saveptr = NULL;
  const var_na *token;
  ကိန်းပြည့် available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ROOT, path, "cgroup.controllers", NULL);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ret = read_all_file (controllers, &buf, NULL, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  အတွက် (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
    {
      အကယ်၍ (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      မဟုတ်ပါက အကယ်၍ (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      မဟုတ်ပါက အကယ်၍ (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      မဟုတ်ပါက အကယ်၍ (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      မဟုတ်ပါက အကယ်၍ (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      မဟုတ်ပါက အကယ်၍ (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      မဟုတ်ပါက အကယ်၍ (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  ပြန်ပေး available;
}

static ကိန်းပြည့်
write_controller_file (const var_na *path, ကိန်းပြည့် controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free var_na *subtree_control = NULL;
  cleanup_free var_na *controllers = NULL;
  size_t controllers_len = 0;
  ကိန်းပြည့် ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ROOT, path, "cgroup.subtree_control", NULL);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    {
      cleanup_free var_na *controllers_copy = xmalloc (controllers_len + 1);
      ကိန်းပြည့် attempts_left;
      var_na *saveptr = NULL;
      const var_na *token;
      ကိန်းပြည့် e;

      e = crun_error_get_errno (err);
      အကယ်၍ (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        ပြန်ပေး crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      အကယ်၍ (e == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;
          ကိန်းပြည့် exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          အကယ်၍ (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              ပြန်ပေး ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          အကယ်၍ (exists == 0)
            ပြန်ပေး ret;
        }

      crun_error_release (err);

      အကယ်၍ (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          အကယ်၍ (UNLIKELY (ret < 0))
            ပြန်ပေး crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      အတွက် (attempts_left = 1000; attempts_left >= 0; attempts_left--)
        {
          ကိန်းပြည့် controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          အတွက် (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (NULL, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              အကယ်၍ (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  အကယ်၍ (e == EBUSY)
                    repeat = true;

                  continue;
                }

              controllers_written++;
            }

          အကယ်၍ (! repeat)
            bha_nga;

          /* If there was any controller written, just try once more without any delay.  */
          အကယ်၍ (controllers_written > 0 && attempts_left > 2)
            {
              attempts_left = 1;
              continue;
            }

          အကယ်၍ (attempts_left > 0)
            {
              sa_nracanaa timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, NULL);
            }
        }

      /* Refresh what controllers are available.  */
      ပြန်ပေး read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  ပြန်ပေး controllers_to_enable;
}

ကိန်းပြည့်
enable_controllers (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *tmp_path = NULL;
  var_na *it;
  ကိန်းပြည့် ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      အကယ်၍ ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;
    }

  အတွက် (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free var_na *cgroup_path = NULL;
      var_na *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, tmp_path, NULL);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;

      ret = mkdir (cgroup_path, 0755);
      အကယ်၍ (UNLIKELY (ret < 0 && errno != EEXIST))
        ပြန်ပေး crun_make_error (err, errno, "create `%s`", cgroup_path);

      အကယ်၍ (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          အကယ်၍ (UNLIKELY (ret < 0))
            ပြန်ပေး ret;
        }

      *it = '/';
      it = next_slash;
    }
  ပြန်ပေး 0;
}

ကိန်းပြည့်
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, const var_na *path, bool create_if_missing, libcrun_error_t *err)
{
  ကိန်းပြည့် cgroup_mode = libcrun_get_cgroup_mode (err);
  အကယ်၍ (UNLIKELY (cgroup_mode < 0))
    ပြန်ပေး cgroup_mode;

  အကယ်၍ (path == NULL || *path == '\0')
    ပြန်ပေး 0;

  ပြန်ပေး enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
ကိန်းပြည့်
libcrun_move_self_to_cgroups (const var_na *content, libcrun_error_t *err)
{
  cleanup_free var_na *buf = xstrdup (content);
  var_na *saveptr = NULL;
  var_na *controller;
  var_na *path;
  ကိန်းပြည့် cgroup_mode;
  bool has_data;
  ကိန်းပြည့် ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  အကယ်၍ (UNLIKELY (cgroup_mode < 0))
    ပြန်ပေး cgroup_mode;

  အတွက် (has_data = read_proc_cgroup (buf, &saveptr, NULL, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &path))
    {
      const var_na *subsystem = NULL;

      အကယ်၍ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          အကယ်၍ (controller[0] != '\0')
            continue;
        }
      မဟုတ်ပါက
        {
          cleanup_free var_na *subsystem_path = NULL;

          /* Use the same names as enter_cgroup_v1 does.  */
          အကယ်၍ (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          အကယ်၍ (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          အကယ်၍ (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ROOT, subsystem, NULL);
          အကယ်၍ (UNLIKELY (ret < 0))
            ပြန်ပေး ret;
          ret = crun_path_exists (subsystem_path, err);
          အကယ်၍ (UNLIKELY (ret < 0))
            ပြန်ပေး ret;
          အကယ်၍ (ret == 0)
            continue;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;
    }

  ပြန်ပေး 0;
}

ကိန်းပြည့်
libcrun_get_cgroup_dirfd (sa_nracanaa libcrun_cgroup_status *status, const var_na *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free var_na *path_to_cgroup = NULL;
  ကိန်းပြည့် cgroup_mode;
  ကိန်းပြည့် cgroupdirfd;
  ကိန်းပြည့် ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  အကယ်၍ (cgroup_mode < 0)
    ပြန်ပေး cgroup_mode;

  အကယ်၍ (cgroup_mode != CGROUP_MODE_UNIFIED)
    ပြန်ပေး crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  အကယ်၍ (status == NULL)
    ပြန်ပေး crun_make_error (err, 0, "internal error");

  အကယ်၍ (is_empty_string (status->path))
    ပြန်ပေး crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ROOT, status->path, sub_cgroup, NULL);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  အကယ်၍ (UNLIKELY (cgroupdirfd < 0))
    ပြန်ပေး crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  ပြန်ပေး cgroupdirfd;
}

ကိန်းပြည့်
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, var_na *from, var_na *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = NULL;
  cleanup_close ကိန်းပြည့် child_dfd = -1;
  ကိန်းပြည့် cgroup_mode;
  size_t from_len;
  size_t i;
  ကိန်းပြည့် ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  အကယ်၍ (cgroup_mode < 0)
    ပြန်ပေး cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    goto thaw;

  from_len = strlen (from);

  အတွက် (i = 0; pids && pids[i]; i++)
    {
      cleanup_free var_na *pid_path = NULL;
      cleanup_free var_na *dest_cgroup = NULL;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      အကယ်၍ (UNLIKELY (ret < 0))
        goto thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      အကယ်၍ (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          goto thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      အကယ်၍ (UNLIKELY (ret < 0))
        goto thaw;
    }

  ret = 0;

thaw:
  အကယ်၍ (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = NULL;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      အကယ်၍ (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      ပြန်ပေး ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ပြန်ပေး destroy_cgroup_path (from, cgroup_mode, err);
}

ကိန်းပြည့်
get_cgroup_dirfd_path (ကိန်းပြည့် dirfd, var_na **path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  အကယ်၍ (UNLIKELY (len < 0))
    ပြန်ပေး len;

  အကယ်၍ (has_prefix (cgroup_path, CGROUP_ROOT))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ROOT));
      ပြန်ပေး 0;
    }
  ပြန်ပေး crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
ကိန်းပြည့်
parse_cpu_max (const var_na *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  const var_na *it = value;
  var_na *endptr = NULL;

  နေစဉ် (*it == ' ')
    it++;

  errno = 0;
  အကယ်၍ (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (var_na *) it + 3;
    }
  မဟုတ်ပါက
    {
      *quota = strtoll (it, &endptr, 10);
      အကယ်၍ (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        ပြန်ပေး crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
    }

  it = endptr;
  နေစဉ် (*it == ' ')
    it++;

  *has_period = false;
  အကယ်၍ (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      အကယ်၍ (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        ပြန်ပေး crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
      *has_period = true;
    }

  ပြန်ပေး 0;
}
