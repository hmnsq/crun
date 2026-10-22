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

puur_naa_nka
libcrun_cgroups_create_symlinks (puur_naa_nka dirfd, libcrun_error_t *err)
{
  puur_naa_nka i;

  for (i = 0; cgroup_symlinks[i].name; i++)
    {
      puur_naa_nka ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      ᰀᰧ (UNLIKELY (ret < 0))
        {
          ᰀᰧ (errno == ENOENT || errno == EEXIST)
            continue;
          ᰀᰧᰌ crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  ᰀᰧᰌ 0;
}

puur_naa_nka
maybe_make_cgroup_threaded (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path_type = NULL;
  const var_na *const threaded = "threaded";
  cleanup_free var_na *content = NULL;
  cleanup_free var_na *buffer = NULL;
  const var_na *parent;
  size_t size;
  var_na *it;
  puur_naa_nka ret;

  ᰀᰧ (path == NULL)
    ᰀᰧᰌ 0;

  path = consume_slashes (path);

  ᰀᰧ (path[0] == '\0')
    ᰀᰧᰌ 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ROOT, path, "cgroup.type", NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ᰀᰧ (size > 0)
    {
      it = content + size - 1;
      ᰀᰬ (*it == '\n' && it > content)
        *it-- = '\0';
    }

  ᰀᰧ (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    ᰀᰧᰌ 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  ᰀᰧ (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      ᰀᰧ (ret < 0)
        ᰀᰧᰌ ret;
    }
  ᰀᰧᰌ write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

puur_naa_nka
move_process_to_cgroup (pid_t pid, const var_na *subsystem, const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path_procs = NULL;
  var_na pid_str[16];
  puur_naa_nka ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ROOT,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = snprintf (pid_str, parimaa_na (pid_str), "%d", pid);
  ᰀᰧ (UNLIKELY (ret >= (puur_naa_nka) parimaa_na (pid_str)))
    ᰀᰧᰌ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      ᰀᰧ (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = NULL;
          puur_naa_nka mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          ᰀᰧ (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              ᰀᰧᰌ ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;

          ᰀᰧᰌ write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      ᰀᰧᰌ ret;
    }
  ᰀᰧᰌ ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

static puur_naa_nka
detect_cgroup_mode (libcrun_error_t *err)
{
  sa_nracanaa statfs stat;
  puur_naa_nka ret;

  ret = statfs (CGROUP_ROOT, &stat);
  ᰀᰧ (ret < 0)
    ᰀᰧᰌ crun_make_error (err, errno, "statfs `" CGROUP_ROOT "`");
  ᰀᰧ (stat.f_type == CGROUP2_SUPER_MAGIC)
    ᰀᰧᰌ CGROUP_MODE_UNIFIED;
  ᰀᰧ (stat.f_type != TMPFS_MAGIC)
    ᰀᰧᰌ crun_make_error (err, 0, "invalid file system type on `" CGROUP_ROOT "`");
  ret = statfs (CGROUP_ROOT "/unified", &stat);
  ᰀᰧ (ret < 0 && errno != ENOENT)
    ᰀᰧᰌ crun_make_error (err, errno, "statfs `" CGROUP_ROOT "/unified`");
  ᰀᰧ (ret < 0)
    ᰀᰧᰌ CGROUP_MODE_LEGACY;
  ᰀᰧᰌ stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

puur_naa_nka
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  puur_naa_nka tmp;
  static puur_naa_nka cgroup_mode = 0;

  ᰀᰧ (cgroup_mode)
    ᰀᰧᰌ cgroup_mode;

  tmp = detect_cgroup_mode (err);
  ᰀᰧ (UNLIKELY (tmp < 0))
    ᰀᰧᰌ tmp;

  cgroup_mode = tmp;

  ᰀᰧᰌ cgroup_mode;
}

static puur_naa_nka
get_cgroup_process_from_content (var_na *content, puur_naa_nka cgroup_mode, var_na **path, bool absolute, libcrun_error_t *err)
{
  var_na *cg_path = NULL;
  var_na *controller;
  var_na *saveptr;
  bool has_data;

  for (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &cg_path))
    {
      ᰀᰧ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          ᰀᰧ (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            goto found;
        }
      ᰀᰧᰀ
        {
          ᰀᰧ (strcmp (controller, "memory"))
            goto found;
        }
    }

  ᰀᰧᰌ crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  ᰀᰧ (absolute)
    ᰀᰧᰌ append_paths (path, err, CGROUP_ROOT, cg_path, NULL);

  *path = xstrdup (cg_path);
  ᰀᰧᰌ 0;
}

puur_naa_nka
libcrun_get_cgroup_process_at (puur_naa_nka dirfd, var_na **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  size_t content_size;
  puur_naa_nka cgroup_mode;
  puur_naa_nka ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (UNLIKELY (cgroup_mode < 0))
    ᰀᰧᰌ cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ᰀᰧᰌ get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

puur_naa_nka
libcrun_get_cgroup_process (pid_t pid, var_na **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  var_na proc_cgroup_file[64];
  size_t content_size;
  puur_naa_nka cgroup_mode;
  puur_naa_nka ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (UNLIKELY (cgroup_mode < 0))
    ᰀᰧᰌ cgroup_mode;

  ᰀᰧ (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  ᰀᰧᰀ
    {
      puur_naa_nka len = snprintf (proc_cgroup_file, parimaa_na (proc_cgroup_file), "/proc/%d/cgroup", pid);
      ᰀᰧ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (proc_cgroup_file)))
        ᰀᰧᰌ crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ᰀᰧᰌ get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

static puur_naa_nka
read_pids_file (puur_naa_nka dfd, const var_na *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka tasksfd = -1;
  cleanup_free var_na *buffer = NULL;
  var_na *saveptr = NULL;
  size_t n_new_pids;
  size_t len;
  var_na *it;
  puur_naa_nka ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  ᰀᰧ (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      ᰀᰧ (errno == ENOENT || errno == ENODEV)
        ᰀᰧᰌ 0;
      ᰀᰧᰌ crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ᰀᰧ (len > 0)
    {
      for (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      ᰀᰧ (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, parimaa_na (pid_t) * *allocated);
        }

      for (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (NULL, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, NULL, 10);

          ᰀᰧ (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  ᰀᰧᰌ 0;
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
  puur_naa_nka dfd;
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
  puur_naa_nka saved_errno = errno;

  ᰀᰬ (walk->n_dirs > 0)
    cgroup_walk_pop (walk);
  free (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
static puur_naa_nka
cgroup_walk_enter (sa_nracanaa cgroup_walk_s *walk, puur_naa_nka dfd)
{
  cleanup_close puur_naa_nka dfd_clone = -1;
  cleanup_dir DIR *dir = NULL;
  sa_nracanaa dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  ᰀᰧ (UNLIKELY (dfd_clone < 0))
    ᰀᰧᰌ -1;

  dir = fdopendir (dfd_clone);
  ᰀᰧ (UNLIKELY (dir == NULL))
    ᰀᰧᰌ -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  for (de = readdir (dir); de; de = readdir (dir))
    {
      sa_nracanaa cgroup_walk_dir_s *d;

      ᰀᰧ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      ᰀᰧ (de->d_type != DT_DIR)
        continue;

      ᰀᰧ (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, parimaa_na (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  ᰀᰧᰌ 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
static puur_naa_nka
cgroup_walk_next (sa_nracanaa cgroup_walk_s *walk, sa_nracanaa cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : NULL;

  ᰀᰬ (*d && walk->depth >= (*d)->depth)
    {
      puur_naa_nka parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      ᰀᰧ (UNLIKELY (parent < 0))
        ᰀᰧᰌ -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  ᰀᰧᰌ 0;
}

static puur_naa_nka
read_pids_cgroup_file (puur_naa_nka dfd, const var_na *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk sa_nracanaa cgroup_walk_s walk = { .dfd = -1 };
  puur_naa_nka dfd_clone;
  puur_naa_nka ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ᰀᰧ (! recurse)
    ᰀᰧᰌ 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ᰀᰧ (UNLIKELY (dfd_clone < 0))
    ᰀᰧᰌ crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ crun_make_error (err, errno, "open cgroup sub-directory");

  for (;;)
    {
      sa_nracanaa cgroup_walk_dir_s *d;
      cleanup_close puur_naa_nka nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ crun_make_error (err, errno, "open parent cgroup directory");
      ᰀᰧ (d == NULL)
        bha_nga;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ᰀᰧ (UNLIKELY (nfd < 0))
        {
          puur_naa_nka saved_errno = errno;

          ᰀᰧ (saved_errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          ᰀᰧᰌ crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_pop (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        {
          puur_naa_nka e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          ᰀᰧ (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            ᰀᰧᰌ ret;
          crun_error_release (err);
          continue;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      ᰀᰧ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ᰀᰧᰌ crun_make_error (err, errno, "open cgroup sub-directory");
    }

  ᰀᰧᰌ 0;
}

static puur_naa_nka
read_pids_cgroup (puur_naa_nka dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  ᰀᰧ (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  ᰀᰧᰌ ret;
}

static puur_naa_nka
rmdir_all_fd (puur_naa_nka dfd)
{
  cleanup_cgroup_walk sa_nracanaa cgroup_walk_s walk = { .dfd = -1 };
  puur_naa_nka ret;

  ret = cgroup_walk_enter (&walk, dfd);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  for (;;)
    {
      sa_nracanaa cgroup_walk_dir_s *d;
      puur_naa_nka child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
      ᰀᰧ (d == NULL)
        bha_nga;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      ᰀᰧ (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_pop (&walk);
          continue;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      ᰀᰧ (UNLIKELY (child_dfd < 0))
        {
          ᰀᰧ (errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          ᰀᰧᰌ child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      ᰀᰧ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ᰀᰧᰌ ret;
    }

  ᰀᰧᰌ 0;
}

static puur_naa_nka
rmdir_all (const var_na *path)
{
  puur_naa_nka ret;
  puur_naa_nka dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  ᰀᰧ (UNLIKELY (dfd < 0))
    ᰀᰧᰌ dfd;

  ret = rmdir_all_fd (dfd);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ᰀᰧᰌ rmdir (path);
}

static puur_naa_nka
read_pids_from_path (const var_na *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_close puur_naa_nka dirfd = -1;
  size_t n_pids, allocated;
  puur_naa_nka mode;
  puur_naa_nka ret;

  ᰀᰧ (path == NULL || *path == '\0')
    ᰀᰧᰌ 0;

  mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (UNLIKELY (mode < 0))
    ᰀᰧᰌ mode;

  switch (mode)
    {
    pak_sa CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
      bha_nga;

    pak_sa CGROUP_MODE_HYBRID:
    pak_sa CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT "/memory", path, NULL);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
      bha_nga;

    default:
      ᰀᰧᰌ crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ᰀᰧ (dirfd < 0)
    ᰀᰧᰌ crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  ᰀᰧ (threads && mode == CGROUP_MODE_UNIFIED)
    ᰀᰧᰌ read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  ᰀᰧᰌ read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

puur_naa_nka
libcrun_cgroup_read_pids_from_path (const var_na *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ᰀᰧᰌ read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (var_na *content, var_na **saveptr, var_na **id, var_na **controller_list, var_na **path)
{
  var_na *it;

  it = strtok_r (content, "\n", saveptr);
  ᰀᰧ (it == NULL)
    ᰀᰧᰌ false;

  ᰀᰧ (id)
    *id = it;

  it = strchr (it, ':');
  ᰀᰧ (it == NULL)
    ᰀᰧᰌ false;
  *it++ = '\0';

  ᰀᰧ (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  ᰀᰧ (it == NULL)
    ᰀᰧᰌ false;
  *it++ = '\0';

  ᰀᰧ (path)
    *path = it;

  ᰀᰧᰌ true;
}

puur_naa_nka
destroy_cgroup_path (const var_na *path, puur_naa_nka mode, libcrun_error_t *err)
{
  bool repeat = true;
  puur_naa_nka retry_count = 0;
  const puur_naa_nka max_attempts = 500;
  puur_naa_nka ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  ᰀᰧ (is_empty_string (path))
    ᰀᰧᰌ 0;

  do
    {
      repeat = false;

      ᰀᰧ (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free var_na *cgroup_path = NULL;

          ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;
          ret = rmdir (cgroup_path);
          ᰀᰧ (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              ᰀᰧ (ret < 0)
                {
                  ᰀᰧ (retry_count >= max_attempts)
                    ᰀᰧᰌ crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_count++;
                  repeat = true;
                }
            }
        }
      ᰀᰧᰀ
        {
          cleanup_free var_na *content = NULL;
          size_t content_size;
          var_na *controller;
          var_na *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            {
              ᰀᰧ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  ᰀᰧᰌ 0;
                }
              ᰀᰧᰌ ret;
            }

          for (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
               has_data;
               has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
            {
              cleanup_free var_na *cgroup_path = NULL;
              var_na *subsystem;
              ᰀᰧ (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              ᰀᰧ (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                continue;

              ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path, NULL);
              ᰀᰧ (UNLIKELY (ret < 0))
                ᰀᰧᰌ ret;

              ret = rmdir (cgroup_path);
              ᰀᰧ (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  ᰀᰧ (ret < 0)
                    {
                      ᰀᰧ (retry_count >= max_attempts)
                        ᰀᰧᰌ crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_count++;
                      repeat = true;
                    }
                }
            }
        }

      ᰀᰧ (repeat)
        {
          sa_nracanaa timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, NULL);

          ret = cgroup_killall_path (path, SIGKILL, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } ᰀᰬ (repeat);

  ᰀᰧᰌ 0;
}

puur_naa_nka
chown_cgroups (const var_na *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_free var_na *delegate = NULL;
  cleanup_close puur_naa_nka dfd = -1;
  size_t delegate_size;
  var_na *saveptr = NULL;
  var_na *name;
  puur_naa_nka ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  ᰀᰧ (UNLIKELY (dfd < 0))
    ᰀᰧᰌ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      ᰀᰧ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ᰀᰧᰌ 0;
        }
      ᰀᰧᰌ ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  for (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (NULL, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ᰀᰧ (UNLIKELY (ret < 0))
        {
          ᰀᰧ (errno == ENOENT)
            continue;

          ᰀᰧᰌ crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  ᰀᰧᰌ 0;
}

static puur_naa_nka
libcrun_cgroup_pause_unpause_with_mode (const var_na *cgroup_path, puur_naa_nka cgroup_mode, const bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free var_na *path = NULL;
  const var_na *state = "";
  puur_naa_nka ret;

  ᰀᰧ (cgroup_path == NULL || cgroup_path[0] == '\0')
    ᰀᰧᰌ crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  ᰀᰧ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
    }
  ᰀᰧᰀ
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  ᰀᰧ (ret >= 0)
    ᰀᰧᰌ 0;
  ᰀᰧᰌ ret;
}

puur_naa_nka
libcrun_cgroup_pause_unpause_path (const var_na *cgroup_path, const bool pause, libcrun_error_t *err)
{
  puur_naa_nka cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (UNLIKELY (cgroup_mode < 0))
    ᰀᰧᰌ cgroup_mode;

  ᰀᰧᰌ libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
puur_naa_nka
libcrun_cgroup_ensure_not_frozen (const var_na *cgroup_path, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  cleanup_free var_na *path = NULL;
  const var_na *frozen;
  puur_naa_nka cgroup_mode;
  puur_naa_nka ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (UNLIKELY (cgroup_mode < 0))
    ᰀᰧᰌ cgroup_mode;

  ᰀᰧ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
    }
  ᰀᰧᰀ
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
    }
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = read_all_file (path, &content, NULL, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      ᰀᰧ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ᰀᰧᰌ 0;
        }
      ᰀᰧᰌ ret;
    }

  ᰀᰧ (strstr (content, frozen))
    ᰀᰧᰌ crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  ᰀᰧᰌ 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
static puur_naa_nka
watch_cgroup_events (const var_na *path)
{
  cleanup_free var_na *events_file = NULL;
  cleanup_close puur_naa_nka fd = -1;
  libcrun_error_t tmp_err = NULL;
  puur_naa_nka ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ᰀᰧᰌ -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  ᰀᰧ (UNLIKELY (fd < 0))
    ᰀᰧᰌ -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ -1;

  ret = fd;
  fd = -1;
  ᰀᰧᰌ ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
static puur_naa_nka
wait_cgroup_unpopulated (const var_na *path, puur_naa_nka inotify_fd)
{
  cleanup_free var_na *events_file = NULL;
  libcrun_error_t tmp_err = NULL;
  sa_nracanaa timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  puur_naa_nka ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ᰀᰧᰌ 0;
    }

  for (;;)
    {
      cleanup_free var_na *content = NULL;
      var_na buf[4096];
      sa_nracanaa pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, NULL, &tmp_err);
      ᰀᰧ (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          ᰀᰧᰌ 0;
        }

      ᰀᰧ (strstr (content, "populated 0"))
        ᰀᰧᰌ 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, NULL));
      ᰀᰧ (ret <= 0)
        ᰀᰧᰌ 0;

      ᰀᰬ (read (inotify_fd, buf, parimaa_na (buf)) > 0)
        ;
    }
}

puur_naa_nka
cgroup_killall_path (const var_na *path, puur_naa_nka sa_nkeeta, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close puur_naa_nka inotify_fd = -1;
  puur_naa_nka ret;
  size_t i;
  cleanup_free pid_t *pids = NULL;

  ᰀᰧ (path == NULL || *path == '\0')
    ᰀᰧᰌ 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  ᰀᰧ (sa_nkeeta == SIGKILL)
    {
      cleanup_free var_na *kill_file = NULL;

      ret = append_paths (&kill_file, err, CGROUP_ROOT, path, "cgroup.kill", NULL);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      ᰀᰧ (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        ᰀᰧᰌ 0;

      ᰀᰧ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    crun_error_release (err);
  ᰀᰧᰀ
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, sa_nkeeta == SIGKILL, &pids, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      ᰀᰧ (crun_error_get_errno (err) != ENOENT)
        goto thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  for (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], sa_nkeeta);
      ᰀᰧ (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          goto thaw;
        }
    }

  ret = 0;

thaw:
  ᰀᰧ (frozen)
    {
      libcrun_error_t thaw_err = NULL;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      ᰀᰧ (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  ᰀᰧᰌ ret;
}

static puur_naa_nka
read_available_controllers (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *controllers = NULL;
  cleanup_free var_na *buf = NULL;
  var_na *saveptr = NULL;
  const var_na *token;
  puur_naa_nka available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ROOT, path, "cgroup.controllers", NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = read_all_file (controllers, &buf, NULL, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  for (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
    {
      ᰀᰧ (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      ᰀᰧᰀ ᰀᰧ (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      ᰀᰧᰀ ᰀᰧ (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      ᰀᰧᰀ ᰀᰧ (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      ᰀᰧᰀ ᰀᰧ (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      ᰀᰧᰀ ᰀᰧ (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      ᰀᰧᰀ ᰀᰧ (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  ᰀᰧᰌ available;
}

static puur_naa_nka
write_controller_file (const var_na *path, puur_naa_nka controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free var_na *subtree_control = NULL;
  cleanup_free var_na *controllers = NULL;
  size_t controllers_len = 0;
  puur_naa_nka ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ROOT, path, "cgroup.subtree_control", NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      cleanup_free var_na *controllers_copy = xmalloc (controllers_len + 1);
      puur_naa_nka attempts_left;
      var_na *saveptr = NULL;
      const var_na *token;
      puur_naa_nka e;

      e = crun_error_get_errno (err);
      ᰀᰧ (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        ᰀᰧᰌ crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      ᰀᰧ (e == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;
          puur_naa_nka exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          ᰀᰧ (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              ᰀᰧᰌ ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          ᰀᰧ (exists == 0)
            ᰀᰧᰌ ret;
        }

      crun_error_release (err);

      ᰀᰧ (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      for (attempts_left = 1000; attempts_left >= 0; attempts_left--)
        {
          puur_naa_nka controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          for (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (NULL, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              ᰀᰧ (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  ᰀᰧ (e == EBUSY)
                    repeat = true;

                  continue;
                }

              controllers_written++;
            }

          ᰀᰧ (! repeat)
            bha_nga;

          /* If there was any controller written, just try once more without any delay.  */
          ᰀᰧ (controllers_written > 0 && attempts_left > 2)
            {
              attempts_left = 1;
              continue;
            }

          ᰀᰧ (attempts_left > 0)
            {
              sa_nracanaa timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, NULL);
            }
        }

      /* Refresh what controllers are available.  */
      ᰀᰧᰌ read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  ᰀᰧᰌ controllers_to_enable;
}

puur_naa_nka
enable_controllers (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *tmp_path = NULL;
  var_na *it;
  puur_naa_nka ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      ᰀᰧ ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
    }

  for (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free var_na *cgroup_path = NULL;
      var_na *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, tmp_path, NULL);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;

      ret = mkdir (cgroup_path, 0755);
      ᰀᰧ (UNLIKELY (ret < 0 && errno != EEXIST))
        ᰀᰧᰌ crun_make_error (err, errno, "create `%s`", cgroup_path);

      ᰀᰧ (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;
        }

      *it = '/';
      it = next_slash;
    }
  ᰀᰧᰌ 0;
}

puur_naa_nka
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, const var_na *path, bool create_if_missing, libcrun_error_t *err)
{
  puur_naa_nka cgroup_mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (UNLIKELY (cgroup_mode < 0))
    ᰀᰧᰌ cgroup_mode;

  ᰀᰧ (path == NULL || *path == '\0')
    ᰀᰧᰌ 0;

  ᰀᰧᰌ enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
puur_naa_nka
libcrun_move_self_to_cgroups (const var_na *content, libcrun_error_t *err)
{
  cleanup_free var_na *buf = xstrdup (content);
  var_na *saveptr = NULL;
  var_na *controller;
  var_na *path;
  puur_naa_nka cgroup_mode;
  bool has_data;
  puur_naa_nka ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (UNLIKELY (cgroup_mode < 0))
    ᰀᰧᰌ cgroup_mode;

  for (has_data = read_proc_cgroup (buf, &saveptr, NULL, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &path))
    {
      const var_na *subsystem = NULL;

      ᰀᰧ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          ᰀᰧ (controller[0] != '\0')
            continue;
        }
      ᰀᰧᰀ
        {
          cleanup_free var_na *subsystem_path = NULL;

          /* Use the same names as enter_cgroup_v1 does.  */
          ᰀᰧ (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          ᰀᰧ (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          ᰀᰧ (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ROOT, subsystem, NULL);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;
          ret = crun_path_exists (subsystem_path, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;
          ᰀᰧ (ret == 0)
            continue;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
    }

  ᰀᰧᰌ 0;
}

puur_naa_nka
libcrun_get_cgroup_dirfd (sa_nracanaa libcrun_cgroup_status *status, const var_na *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free var_na *path_to_cgroup = NULL;
  puur_naa_nka cgroup_mode;
  puur_naa_nka cgroupdirfd;
  puur_naa_nka ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (cgroup_mode < 0)
    ᰀᰧᰌ cgroup_mode;

  ᰀᰧ (cgroup_mode != CGROUP_MODE_UNIFIED)
    ᰀᰧᰌ crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  ᰀᰧ (status == NULL)
    ᰀᰧᰌ crun_make_error (err, 0, "internal error");

  ᰀᰧ (is_empty_string (status->path))
    ᰀᰧᰌ crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ROOT, status->path, sub_cgroup, NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  ᰀᰧ (UNLIKELY (cgroupdirfd < 0))
    ᰀᰧᰌ crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  ᰀᰧᰌ cgroupdirfd;
}

puur_naa_nka
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, var_na *from, var_na *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = NULL;
  cleanup_close puur_naa_nka child_dfd = -1;
  puur_naa_nka cgroup_mode;
  size_t from_len;
  size_t i;
  puur_naa_nka ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (cgroup_mode < 0)
    ᰀᰧᰌ cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    goto thaw;

  from_len = strlen (from);

  for (i = 0; pids && pids[i]; i++)
    {
      cleanup_free var_na *pid_path = NULL;
      cleanup_free var_na *dest_cgroup = NULL;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        goto thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      ᰀᰧ (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          goto thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        goto thaw;
    }

  ret = 0;

thaw:
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = NULL;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      ᰀᰧ (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      ᰀᰧᰌ ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ᰀᰧᰌ destroy_cgroup_path (from, cgroup_mode, err);
}

puur_naa_nka
get_cgroup_dirfd_path (puur_naa_nka dirfd, var_na **path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  ᰀᰧ (UNLIKELY (len < 0))
    ᰀᰧᰌ len;

  ᰀᰧ (has_prefix (cgroup_path, CGROUP_ROOT))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ROOT));
      ᰀᰧᰌ 0;
    }
  ᰀᰧᰌ crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
puur_naa_nka
parse_cpu_max (const var_na *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  const var_na *it = value;
  var_na *endptr = NULL;

  ᰀᰬ (*it == ' ')
    it++;

  errno = 0;
  ᰀᰧ (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (var_na *) it + 3;
    }
  ᰀᰧᰀ
    {
      *quota = strtoll (it, &endptr, 10);
      ᰀᰧ (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        ᰀᰧᰌ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
    }

  it = endptr;
  ᰀᰬ (*it == ' ')
    it++;

  *has_period = false;
  ᰀᰧ (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      ᰀᰧ (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        ᰀᰧᰌ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
      *has_period = true;
    }

  ᰀᰧᰌ 0;
}
