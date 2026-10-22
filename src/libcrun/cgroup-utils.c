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

จำนวนเต็ม
libcrun_cgroups_create_symlinks (จำนวนเต็ม dirfd, libcrun_error_t *err)
{
  จำนวนเต็ม i;

  สำหรับ (i = 0; cgroup_symlinks[i].name; i++)
    {
      จำนวนเต็ม ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      ถ้า (UNLIKELY (ret < 0))
        {
          ถ้า (errno == ENOENT || errno == EEXIST)
            continue;
          คืน crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  คืน 0;
}

จำนวนเต็ม
maybe_make_cgroup_threaded (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path_type = NULL;
  const var_na *const threaded = "threaded";
  cleanup_free var_na *content = NULL;
  cleanup_free var_na *buffer = NULL;
  const var_na *parent;
  size_t size;
  var_na *it;
  จำนวนเต็ม ret;

  ถ้า (path == NULL)
    คืน 0;

  path = consume_slashes (path);

  ถ้า (path[0] == '\0')
    คืน 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ROOT, path, "cgroup.type", NULL);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ถ้า (size > 0)
    {
      it = content + size - 1;
      ขณะ (*it == '\n' && it > content)
        *it-- = '\0';
    }

  ถ้า (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    คืน 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  ถ้า (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      ถ้า (ret < 0)
        คืน ret;
    }
  คืน write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

จำนวนเต็ม
move_process_to_cgroup (pid_t pid, const var_na *subsystem, const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path_procs = NULL;
  var_na pid_str[16];
  จำนวนเต็ม ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ROOT,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", NULL);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = snprintf (pid_str, parimaa_na (pid_str), "%d", pid);
  ถ้า (UNLIKELY (ret >= (จำนวนเต็ม) parimaa_na (pid_str)))
    คืน crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  ถ้า (UNLIKELY (ret < 0))
    {
      ถ้า (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = NULL;
          จำนวนเต็ม mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          ถ้า (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              คืน ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          ถ้า (UNLIKELY (ret < 0))
            คืน ret;

          คืน write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      คืน ret;
    }
  คืน ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

static จำนวนเต็ม
detect_cgroup_mode (libcrun_error_t *err)
{
  sa_nracanaa statfs stat;
  จำนวนเต็ม ret;

  ret = statfs (CGROUP_ROOT, &stat);
  ถ้า (ret < 0)
    คืน crun_make_error (err, errno, "statfs `" CGROUP_ROOT "`");
  ถ้า (stat.f_type == CGROUP2_SUPER_MAGIC)
    คืน CGROUP_MODE_UNIFIED;
  ถ้า (stat.f_type != TMPFS_MAGIC)
    คืน crun_make_error (err, 0, "invalid file system type on `" CGROUP_ROOT "`");
  ret = statfs (CGROUP_ROOT "/unified", &stat);
  ถ้า (ret < 0 && errno != ENOENT)
    คืน crun_make_error (err, errno, "statfs `" CGROUP_ROOT "/unified`");
  ถ้า (ret < 0)
    คืน CGROUP_MODE_LEGACY;
  คืน stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

จำนวนเต็ม
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  จำนวนเต็ม tmp;
  static จำนวนเต็ม cgroup_mode = 0;

  ถ้า (cgroup_mode)
    คืน cgroup_mode;

  tmp = detect_cgroup_mode (err);
  ถ้า (UNLIKELY (tmp < 0))
    คืน tmp;

  cgroup_mode = tmp;

  คืน cgroup_mode;
}

static จำนวนเต็ม
get_cgroup_process_from_content (var_na *content, จำนวนเต็ม cgroup_mode, var_na **path, bool absolute, libcrun_error_t *err)
{
  var_na *cg_path = NULL;
  var_na *controller;
  var_na *saveptr;
  bool has_data;

  สำหรับ (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &cg_path))
    {
      ถ้า (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          ถ้า (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            goto found;
        }
      มิฉะนั้น
        {
          ถ้า (strcmp (controller, "memory"))
            goto found;
        }
    }

  คืน crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  ถ้า (absolute)
    คืน append_paths (path, err, CGROUP_ROOT, cg_path, NULL);

  *path = xstrdup (cg_path);
  คืน 0;
}

จำนวนเต็ม
libcrun_get_cgroup_process_at (จำนวนเต็ม dirfd, var_na **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  size_t content_size;
  จำนวนเต็ม cgroup_mode;
  จำนวนเต็ม ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ถ้า (UNLIKELY (cgroup_mode < 0))
    คืน cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  คืน get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

จำนวนเต็ม
libcrun_get_cgroup_process (pid_t pid, var_na **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  var_na proc_cgroup_file[64];
  size_t content_size;
  จำนวนเต็ม cgroup_mode;
  จำนวนเต็ม ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ถ้า (UNLIKELY (cgroup_mode < 0))
    คืน cgroup_mode;

  ถ้า (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  มิฉะนั้น
    {
      จำนวนเต็ม len = snprintf (proc_cgroup_file, parimaa_na (proc_cgroup_file), "/proc/%d/cgroup", pid);
      ถ้า (UNLIKELY (len >= (จำนวนเต็ม) parimaa_na (proc_cgroup_file)))
        คืน crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  คืน get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

static จำนวนเต็ม
read_pids_file (จำนวนเต็ม dfd, const var_na *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close จำนวนเต็ม tasksfd = -1;
  cleanup_free var_na *buffer = NULL;
  var_na *saveptr = NULL;
  size_t n_new_pids;
  size_t len;
  var_na *it;
  จำนวนเต็ม ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  ถ้า (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      ถ้า (errno == ENOENT || errno == ENODEV)
        คืน 0;
      คืน crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ถ้า (len > 0)
    {
      สำหรับ (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      ถ้า (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, parimaa_na (pid_t) * *allocated);
        }

      สำหรับ (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (NULL, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, NULL, 10);

          ถ้า (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  คืน 0;
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
  จำนวนเต็ม dfd;
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
  จำนวนเต็ม saved_errno = errno;

  ขณะ (walk->n_dirs > 0)
    cgroup_walk_pop (walk);
  free (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
static จำนวนเต็ม
cgroup_walk_enter (sa_nracanaa cgroup_walk_s *walk, จำนวนเต็ม dfd)
{
  cleanup_close จำนวนเต็ม dfd_clone = -1;
  cleanup_dir DIR *dir = NULL;
  sa_nracanaa dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  ถ้า (UNLIKELY (dfd_clone < 0))
    คืน -1;

  dir = fdopendir (dfd_clone);
  ถ้า (UNLIKELY (dir == NULL))
    คืน -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  สำหรับ (de = readdir (dir); de; de = readdir (dir))
    {
      sa_nracanaa cgroup_walk_dir_s *d;

      ถ้า (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      ถ้า (de->d_type != DT_DIR)
        continue;

      ถ้า (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, parimaa_na (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  คืน 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
static จำนวนเต็ม
cgroup_walk_next (sa_nracanaa cgroup_walk_s *walk, sa_nracanaa cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : NULL;

  ขณะ (*d && walk->depth >= (*d)->depth)
    {
      จำนวนเต็ม parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      ถ้า (UNLIKELY (parent < 0))
        คืน -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  คืน 0;
}

static จำนวนเต็ม
read_pids_cgroup_file (จำนวนเต็ม dfd, const var_na *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk sa_nracanaa cgroup_walk_s walk = { .dfd = -1 };
  จำนวนเต็ม dfd_clone;
  จำนวนเต็ม ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ถ้า (! recurse)
    คืน 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ถ้า (UNLIKELY (dfd_clone < 0))
    คืน crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  ถ้า (UNLIKELY (ret < 0))
    คืน crun_make_error (err, errno, "open cgroup sub-directory");

  สำหรับ (;;)
    {
      sa_nracanaa cgroup_walk_dir_s *d;
      cleanup_close จำนวนเต็ม nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      ถ้า (UNLIKELY (ret < 0))
        คืน crun_make_error (err, errno, "open parent cgroup directory");
      ถ้า (d == NULL)
        bha_nga;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ถ้า (UNLIKELY (nfd < 0))
        {
          จำนวนเต็ม saved_errno = errno;

          ถ้า (saved_errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          คืน crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_pop (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      ถ้า (UNLIKELY (ret < 0))
        {
          จำนวนเต็ม e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          ถ้า (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            คืน ret;
          crun_error_release (err);
          continue;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      ถ้า (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        คืน crun_make_error (err, errno, "open cgroup sub-directory");
    }

  คืน 0;
}

static จำนวนเต็ม
read_pids_cgroup (จำนวนเต็ม dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  จำนวนเต็ม ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  ถ้า (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  คืน ret;
}

static จำนวนเต็ม
rmdir_all_fd (จำนวนเต็ม dfd)
{
  cleanup_cgroup_walk sa_nracanaa cgroup_walk_s walk = { .dfd = -1 };
  จำนวนเต็ม ret;

  ret = cgroup_walk_enter (&walk, dfd);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  สำหรับ (;;)
    {
      sa_nracanaa cgroup_walk_dir_s *d;
      จำนวนเต็ม child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;
      ถ้า (d == NULL)
        bha_nga;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      ถ้า (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_pop (&walk);
          continue;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      ถ้า (UNLIKELY (child_dfd < 0))
        {
          ถ้า (errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          คืน child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      ถ้า (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        คืน ret;
    }

  คืน 0;
}

static จำนวนเต็ม
rmdir_all (const var_na *path)
{
  จำนวนเต็ม ret;
  จำนวนเต็ม dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  ถ้า (UNLIKELY (dfd < 0))
    คืน dfd;

  ret = rmdir_all_fd (dfd);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  คืน rmdir (path);
}

static จำนวนเต็ม
read_pids_from_path (const var_na *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_close จำนวนเต็ม dirfd = -1;
  size_t n_pids, allocated;
  จำนวนเต็ม mode;
  จำนวนเต็ม ret;

  ถ้า (path == NULL || *path == '\0')
    คืน 0;

  mode = libcrun_get_cgroup_mode (err);
  ถ้า (UNLIKELY (mode < 0))
    คืน mode;

  switch (mode)
    {
    pak_sa CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;
      bha_nga;

    pak_sa CGROUP_MODE_HYBRID:
    pak_sa CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT "/memory", path, NULL);
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;
      bha_nga;

    default:
      คืน crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ถ้า (dirfd < 0)
    คืน crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  ถ้า (threads && mode == CGROUP_MODE_UNIFIED)
    คืน read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  คืน read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

จำนวนเต็ม
libcrun_cgroup_read_pids_from_path (const var_na *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  คืน read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (var_na *content, var_na **saveptr, var_na **id, var_na **controller_list, var_na **path)
{
  var_na *it;

  it = strtok_r (content, "\n", saveptr);
  ถ้า (it == NULL)
    คืน false;

  ถ้า (id)
    *id = it;

  it = strchr (it, ':');
  ถ้า (it == NULL)
    คืน false;
  *it++ = '\0';

  ถ้า (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  ถ้า (it == NULL)
    คืน false;
  *it++ = '\0';

  ถ้า (path)
    *path = it;

  คืน true;
}

จำนวนเต็ม
destroy_cgroup_path (const var_na *path, จำนวนเต็ม mode, libcrun_error_t *err)
{
  bool repeat = true;
  จำนวนเต็ม retry_count = 0;
  const จำนวนเต็ม max_attempts = 500;
  จำนวนเต็ม ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  ถ้า (is_empty_string (path))
    คืน 0;

  do
    {
      repeat = false;

      ถ้า (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free var_na *cgroup_path = NULL;

          ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
          ถ้า (UNLIKELY (ret < 0))
            คืน ret;
          ret = rmdir (cgroup_path);
          ถ้า (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              ถ้า (ret < 0)
                {
                  ถ้า (retry_count >= max_attempts)
                    คืน crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_count++;
                  repeat = true;
                }
            }
        }
      มิฉะนั้น
        {
          cleanup_free var_na *content = NULL;
          size_t content_size;
          var_na *controller;
          var_na *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          ถ้า (UNLIKELY (ret < 0))
            {
              ถ้า (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  คืน 0;
                }
              คืน ret;
            }

          สำหรับ (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
               has_data;
               has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
            {
              cleanup_free var_na *cgroup_path = NULL;
              var_na *subsystem;
              ถ้า (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              ถ้า (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                continue;

              ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path, NULL);
              ถ้า (UNLIKELY (ret < 0))
                คืน ret;

              ret = rmdir (cgroup_path);
              ถ้า (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  ถ้า (ret < 0)
                    {
                      ถ้า (retry_count >= max_attempts)
                        คืน crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_count++;
                      repeat = true;
                    }
                }
            }
        }

      ถ้า (repeat)
        {
          sa_nracanaa timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, NULL);

          ret = cgroup_killall_path (path, SIGKILL, err);
          ถ้า (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } ขณะ (repeat);

  คืน 0;
}

จำนวนเต็ม
chown_cgroups (const var_na *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_free var_na *delegate = NULL;
  cleanup_close จำนวนเต็ม dfd = -1;
  size_t delegate_size;
  var_na *saveptr = NULL;
  var_na *name;
  จำนวนเต็ม ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  ถ้า (UNLIKELY (dfd < 0))
    คืน crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  ถ้า (UNLIKELY (ret < 0))
    {
      ถ้า (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          คืน 0;
        }
      คืน ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  ถ้า (UNLIKELY (ret < 0))
    คืน crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  สำหรับ (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (NULL, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ถ้า (UNLIKELY (ret < 0))
        {
          ถ้า (errno == ENOENT)
            continue;

          คืน crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  คืน 0;
}

static จำนวนเต็ม
libcrun_cgroup_pause_unpause_with_mode (const var_na *cgroup_path, จำนวนเต็ม cgroup_mode, const bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free var_na *path = NULL;
  const var_na *state = "";
  จำนวนเต็ม ret;

  ถ้า (cgroup_path == NULL || cgroup_path[0] == '\0')
    คืน crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  ถ้า (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;
    }
  มิฉะนั้น
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  ถ้า (ret >= 0)
    คืน 0;
  คืน ret;
}

จำนวนเต็ม
libcrun_cgroup_pause_unpause_path (const var_na *cgroup_path, const bool pause, libcrun_error_t *err)
{
  จำนวนเต็ม cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ถ้า (UNLIKELY (cgroup_mode < 0))
    คืน cgroup_mode;

  คืน libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
จำนวนเต็ม
libcrun_cgroup_ensure_not_frozen (const var_na *cgroup_path, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  cleanup_free var_na *path = NULL;
  const var_na *frozen;
  จำนวนเต็ม cgroup_mode;
  จำนวนเต็ม ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ถ้า (UNLIKELY (cgroup_mode < 0))
    คืน cgroup_mode;

  ถ้า (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
    }
  มิฉะนั้น
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
    }
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = read_all_file (path, &content, NULL, err);
  ถ้า (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      ถ้า (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          คืน 0;
        }
      คืน ret;
    }

  ถ้า (strstr (content, frozen))
    คืน crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  คืน 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
static จำนวนเต็ม
watch_cgroup_events (const var_na *path)
{
  cleanup_free var_na *events_file = NULL;
  cleanup_close จำนวนเต็ม fd = -1;
  libcrun_error_t tmp_err = NULL;
  จำนวนเต็ม ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  ถ้า (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      คืน -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  ถ้า (UNLIKELY (fd < 0))
    คืน -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  ถ้า (UNLIKELY (ret < 0))
    คืน -1;

  ret = fd;
  fd = -1;
  คืน ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
static จำนวนเต็ม
wait_cgroup_unpopulated (const var_na *path, จำนวนเต็ม inotify_fd)
{
  cleanup_free var_na *events_file = NULL;
  libcrun_error_t tmp_err = NULL;
  sa_nracanaa timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  จำนวนเต็ม ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  ถ้า (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      คืน 0;
    }

  สำหรับ (;;)
    {
      cleanup_free var_na *content = NULL;
      var_na buf[4096];
      sa_nracanaa pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, NULL, &tmp_err);
      ถ้า (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          คืน 0;
        }

      ถ้า (strstr (content, "populated 0"))
        คืน 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, NULL));
      ถ้า (ret <= 0)
        คืน 0;

      ขณะ (read (inotify_fd, buf, parimaa_na (buf)) > 0)
        ;
    }
}

จำนวนเต็ม
cgroup_killall_path (const var_na *path, จำนวนเต็ม sa_nkeeta, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close จำนวนเต็ม inotify_fd = -1;
  จำนวนเต็ม ret;
  size_t i;
  cleanup_free pid_t *pids = NULL;

  ถ้า (path == NULL || *path == '\0')
    คืน 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  ถ้า (sa_nkeeta == SIGKILL)
    {
      cleanup_free var_na *kill_file = NULL;

      ret = append_paths (&kill_file, err, CGROUP_ROOT, path, "cgroup.kill", NULL);
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      ถ้า (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        คืน 0;

      ถ้า (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  ถ้า (UNLIKELY (ret < 0))
    crun_error_release (err);
  มิฉะนั้น
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, sa_nkeeta == SIGKILL, &pids, err);
  ถ้า (UNLIKELY (ret < 0))
    {
      ถ้า (crun_error_get_errno (err) != ENOENT)
        goto thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  สำหรับ (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], sa_nkeeta);
      ถ้า (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          goto thaw;
        }
    }

  ret = 0;

thaw:
  ถ้า (frozen)
    {
      libcrun_error_t thaw_err = NULL;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      ถ้า (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  คืน ret;
}

static จำนวนเต็ม
read_available_controllers (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *controllers = NULL;
  cleanup_free var_na *buf = NULL;
  var_na *saveptr = NULL;
  const var_na *token;
  จำนวนเต็ม available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ROOT, path, "cgroup.controllers", NULL);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = read_all_file (controllers, &buf, NULL, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  สำหรับ (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
    {
      ถ้า (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      มิฉะนั้น ถ้า (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      มิฉะนั้น ถ้า (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      มิฉะนั้น ถ้า (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      มิฉะนั้น ถ้า (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      มิฉะนั้น ถ้า (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      มิฉะนั้น ถ้า (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  คืน available;
}

static จำนวนเต็ม
write_controller_file (const var_na *path, จำนวนเต็ม controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free var_na *subtree_control = NULL;
  cleanup_free var_na *controllers = NULL;
  size_t controllers_len = 0;
  จำนวนเต็ม ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ROOT, path, "cgroup.subtree_control", NULL);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  ถ้า (UNLIKELY (ret < 0))
    {
      cleanup_free var_na *controllers_copy = xmalloc (controllers_len + 1);
      จำนวนเต็ม attempts_left;
      var_na *saveptr = NULL;
      const var_na *token;
      จำนวนเต็ม e;

      e = crun_error_get_errno (err);
      ถ้า (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        คืน crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      ถ้า (e == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;
          จำนวนเต็ม exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          ถ้า (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              คืน ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          ถ้า (exists == 0)
            คืน ret;
        }

      crun_error_release (err);

      ถ้า (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          ถ้า (UNLIKELY (ret < 0))
            คืน crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      สำหรับ (attempts_left = 1000; attempts_left >= 0; attempts_left--)
        {
          จำนวนเต็ม controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          สำหรับ (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (NULL, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              ถ้า (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  ถ้า (e == EBUSY)
                    repeat = true;

                  continue;
                }

              controllers_written++;
            }

          ถ้า (! repeat)
            bha_nga;

          /* If there was any controller written, just try once more without any delay.  */
          ถ้า (controllers_written > 0 && attempts_left > 2)
            {
              attempts_left = 1;
              continue;
            }

          ถ้า (attempts_left > 0)
            {
              sa_nracanaa timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, NULL);
            }
        }

      /* Refresh what controllers are available.  */
      คืน read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  คืน controllers_to_enable;
}

จำนวนเต็ม
enable_controllers (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *tmp_path = NULL;
  var_na *it;
  จำนวนเต็ม ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  ถ้า (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      ถ้า ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;
    }

  สำหรับ (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free var_na *cgroup_path = NULL;
      var_na *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, tmp_path, NULL);
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;

      ret = mkdir (cgroup_path, 0755);
      ถ้า (UNLIKELY (ret < 0 && errno != EEXIST))
        คืน crun_make_error (err, errno, "create `%s`", cgroup_path);

      ถ้า (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          ถ้า (UNLIKELY (ret < 0))
            คืน ret;
        }

      *it = '/';
      it = next_slash;
    }
  คืน 0;
}

จำนวนเต็ม
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, const var_na *path, bool create_if_missing, libcrun_error_t *err)
{
  จำนวนเต็ม cgroup_mode = libcrun_get_cgroup_mode (err);
  ถ้า (UNLIKELY (cgroup_mode < 0))
    คืน cgroup_mode;

  ถ้า (path == NULL || *path == '\0')
    คืน 0;

  คืน enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
จำนวนเต็ม
libcrun_move_self_to_cgroups (const var_na *content, libcrun_error_t *err)
{
  cleanup_free var_na *buf = xstrdup (content);
  var_na *saveptr = NULL;
  var_na *controller;
  var_na *path;
  จำนวนเต็ม cgroup_mode;
  bool has_data;
  จำนวนเต็ม ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ถ้า (UNLIKELY (cgroup_mode < 0))
    คืน cgroup_mode;

  สำหรับ (has_data = read_proc_cgroup (buf, &saveptr, NULL, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &path))
    {
      const var_na *subsystem = NULL;

      ถ้า (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          ถ้า (controller[0] != '\0')
            continue;
        }
      มิฉะนั้น
        {
          cleanup_free var_na *subsystem_path = NULL;

          /* Use the same names as enter_cgroup_v1 does.  */
          ถ้า (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          ถ้า (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          ถ้า (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ROOT, subsystem, NULL);
          ถ้า (UNLIKELY (ret < 0))
            คืน ret;
          ret = crun_path_exists (subsystem_path, err);
          ถ้า (UNLIKELY (ret < 0))
            คืน ret;
          ถ้า (ret == 0)
            continue;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;
    }

  คืน 0;
}

จำนวนเต็ม
libcrun_get_cgroup_dirfd (sa_nracanaa libcrun_cgroup_status *status, const var_na *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free var_na *path_to_cgroup = NULL;
  จำนวนเต็ม cgroup_mode;
  จำนวนเต็ม cgroupdirfd;
  จำนวนเต็ม ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ถ้า (cgroup_mode < 0)
    คืน cgroup_mode;

  ถ้า (cgroup_mode != CGROUP_MODE_UNIFIED)
    คืน crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  ถ้า (status == NULL)
    คืน crun_make_error (err, 0, "internal error");

  ถ้า (is_empty_string (status->path))
    คืน crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ROOT, status->path, sub_cgroup, NULL);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  ถ้า (UNLIKELY (cgroupdirfd < 0))
    คืน crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  คืน cgroupdirfd;
}

จำนวนเต็ม
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, var_na *from, var_na *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = NULL;
  cleanup_close จำนวนเต็ม child_dfd = -1;
  จำนวนเต็ม cgroup_mode;
  size_t from_len;
  size_t i;
  จำนวนเต็ม ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ถ้า (cgroup_mode < 0)
    คืน cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  ถ้า (UNLIKELY (ret < 0))
    goto thaw;

  from_len = strlen (from);

  สำหรับ (i = 0; pids && pids[i]; i++)
    {
      cleanup_free var_na *pid_path = NULL;
      cleanup_free var_na *dest_cgroup = NULL;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      ถ้า (UNLIKELY (ret < 0))
        goto thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      ถ้า (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          goto thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      ถ้า (UNLIKELY (ret < 0))
        goto thaw;
    }

  ret = 0;

thaw:
  ถ้า (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = NULL;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      ถ้า (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      คืน ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  คืน destroy_cgroup_path (from, cgroup_mode, err);
}

จำนวนเต็ม
get_cgroup_dirfd_path (จำนวนเต็ม dirfd, var_na **path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  ถ้า (UNLIKELY (len < 0))
    คืน len;

  ถ้า (has_prefix (cgroup_path, CGROUP_ROOT))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ROOT));
      คืน 0;
    }
  คืน crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
จำนวนเต็ม
parse_cpu_max (const var_na *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  const var_na *it = value;
  var_na *endptr = NULL;

  ขณะ (*it == ' ')
    it++;

  errno = 0;
  ถ้า (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (var_na *) it + 3;
    }
  มิฉะนั้น
    {
      *quota = strtoll (it, &endptr, 10);
      ถ้า (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        คืน crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
    }

  it = endptr;
  ขณะ (*it == ' ')
    it++;

  *has_period = false;
  ถ้า (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      ถ้า (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        คืน crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
      *has_period = true;
    }

  คืน 0;
}
