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

struct symlink_s
{
  const char *name;
  const char *target;
};

static struct symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { NULL, NULL } };

ціле
libcrun_cgroups_create_symlinks (ціле dirfd, libcrun_error_t *err)
{
  ціле i;

  для (i = 0; cgroup_symlinks[i].name; i++)
    {
      ціле ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      якщо (UNLIKELY (ret < 0))
        {
          якщо (errno == ENOENT || errno == EEXIST)
            continue;
          повернути crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  повернути 0;
}

ціле
maybe_make_cgroup_threaded (const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_type = NULL;
  const char *const threaded = "threaded";
  cleanup_free char *content = NULL;
  cleanup_free char *buffer = NULL;
  const char *parent;
  size_t size;
  char *it;
  ціле ret;

  якщо (path == NULL)
    повернути 0;

  path = consume_slashes (path);

  якщо (path[0] == '\0')
    повернути 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ROOT, path, "cgroup.type", NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (size > 0)
    {
      it = content + size - 1;
      поки (*it == '\n' && it > content)
        *it-- = '\0';
    }

  якщо (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    повернути 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  якщо (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      якщо (ret < 0)
        повернути ret;
    }
  повернути write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

ціле
move_process_to_cgroup (pid_t pid, const char *subsystem, const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_procs = NULL;
  char pid_str[16];
  ціле ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ROOT,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = snprintf (pid_str, sizeof (pid_str), "%d", pid);
  якщо (UNLIKELY (ret >= (ціле) sizeof (pid_str)))
    повернути crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  якщо (UNLIKELY (ret < 0))
    {
      якщо (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = NULL;
          ціле mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          якщо (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              повернути ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;

          повернути write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      повернути ret;
    }
  повернути ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

static ціле
detect_cgroup_mode (libcrun_error_t *err)
{
  struct statfs stat;
  ціле ret;

  ret = statfs (CGROUP_ROOT, &stat);
  якщо (ret < 0)
    повернути crun_make_error (err, errno, "statfs `" CGROUP_ROOT "`");
  якщо (stat.f_type == CGROUP2_SUPER_MAGIC)
    повернути CGROUP_MODE_UNIFIED;
  якщо (stat.f_type != TMPFS_MAGIC)
    повернути crun_make_error (err, 0, "invalid file system type on `" CGROUP_ROOT "`");
  ret = statfs (CGROUP_ROOT "/unified", &stat);
  якщо (ret < 0 && errno != ENOENT)
    повернути crun_make_error (err, errno, "statfs `" CGROUP_ROOT "/unified`");
  якщо (ret < 0)
    повернути CGROUP_MODE_LEGACY;
  повернути stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

ціле
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  ціле tmp;
  static ціле cgroup_mode = 0;

  якщо (cgroup_mode)
    повернути cgroup_mode;

  tmp = detect_cgroup_mode (err);
  якщо (UNLIKELY (tmp < 0))
    повернути tmp;

  cgroup_mode = tmp;

  повернути cgroup_mode;
}

static ціле
get_cgroup_process_from_content (char *content, ціле cgroup_mode, char **path, bool absolute, libcrun_error_t *err)
{
  char *cg_path = NULL;
  char *controller;
  char *saveptr;
  bool has_data;

  для (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &cg_path))
    {
      якщо (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          якщо (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            goto found;
        }
      інакше
        {
          якщо (strcmp (controller, "memory"))
            goto found;
        }
    }

  повернути crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  якщо (absolute)
    повернути append_paths (path, err, CGROUP_ROOT, cg_path, NULL);

  *path = xstrdup (cg_path);
  повернути 0;
}

ціле
libcrun_get_cgroup_process_at (ціле dirfd, char **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  size_t content_size;
  ціле cgroup_mode;
  ціле ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  якщо (UNLIKELY (cgroup_mode < 0))
    повернути cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

ціле
libcrun_get_cgroup_process (pid_t pid, char **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  char proc_cgroup_file[64];
  size_t content_size;
  ціле cgroup_mode;
  ціле ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  якщо (UNLIKELY (cgroup_mode < 0))
    повернути cgroup_mode;

  якщо (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  інакше
    {
      ціле len = snprintf (proc_cgroup_file, sizeof (proc_cgroup_file), "/proc/%d/cgroup", pid);
      якщо (UNLIKELY (len >= (ціле) sizeof (proc_cgroup_file)))
        повернути crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

static ціле
read_pids_file (ціле dfd, const char *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close ціле tasksfd = -1;
  cleanup_free char *buffer = NULL;
  char *saveptr = NULL;
  size_t n_new_pids;
  size_t len;
  char *it;
  ціле ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  якщо (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      якщо (errno == ENOENT || errno == ENODEV)
        повернути 0;
      повернути crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (len > 0)
    {
      для (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      якщо (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, sizeof (pid_t) * *allocated);
        }

      для (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (NULL, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, NULL, 10);

          якщо (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  повернути 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
struct cgroup_walk_dir_s
{
  char *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
struct cgroup_walk_s
{
  ціле dfd;
  size_t depth;
  struct cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

static void
cgroup_walk_pop (struct cgroup_walk_s *walk)
{
  free (walk->dirs[--walk->n_dirs].name);
}

static void
cleanup_cgroup_walkp (struct cgroup_walk_s *walk)
{
  ціле saved_errno = errno;

  поки (walk->n_dirs > 0)
    cgroup_walk_pop (walk);
  free (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
static ціле
cgroup_walk_enter (struct cgroup_walk_s *walk, ціле dfd)
{
  cleanup_close ціле dfd_clone = -1;
  cleanup_dir DIR *dir = NULL;
  struct dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  якщо (UNLIKELY (dfd_clone < 0))
    повернути -1;

  dir = fdopendir (dfd_clone);
  якщо (UNLIKELY (dir == NULL))
    повернути -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  для (de = readdir (dir); de; de = readdir (dir))
    {
      struct cgroup_walk_dir_s *d;

      якщо (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      якщо (de->d_type != DT_DIR)
        continue;

      якщо (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, sizeof (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  повернути 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
static ціле
cgroup_walk_next (struct cgroup_walk_s *walk, struct cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : NULL;

  поки (*d && walk->depth >= (*d)->depth)
    {
      ціле parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      якщо (UNLIKELY (parent < 0))
        повернути -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  повернути 0;
}

static ціле
read_pids_cgroup_file (ціле dfd, const char *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk struct cgroup_walk_s walk = { .dfd = -1 };
  ціле dfd_clone;
  ціле ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (! recurse)
    повернути 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  якщо (UNLIKELY (dfd_clone < 0))
    повернути crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "open cgroup sub-directory");

  для (;;)
    {
      struct cgroup_walk_dir_s *d;
      cleanup_close ціле nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "open parent cgroup directory");
      якщо (d == NULL)
        break;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      якщо (UNLIKELY (nfd < 0))
        {
          ціле saved_errno = errno;

          якщо (saved_errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          повернути crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_pop (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      якщо (UNLIKELY (ret < 0))
        {
          ціле e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          якщо (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            повернути ret;
          crun_error_release (err);
          continue;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      якщо (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        повернути crun_make_error (err, errno, "open cgroup sub-directory");
    }

  повернути 0;
}

static ціле
read_pids_cgroup (ціле dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  ціле ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  якщо (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  повернути ret;
}

static ціле
rmdir_all_fd (ціле dfd)
{
  cleanup_cgroup_walk struct cgroup_walk_s walk = { .dfd = -1 };
  ціле ret;

  ret = cgroup_walk_enter (&walk, dfd);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  для (;;)
    {
      struct cgroup_walk_dir_s *d;
      ціле child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
      якщо (d == NULL)
        break;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      якщо (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_pop (&walk);
          continue;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      якщо (UNLIKELY (child_dfd < 0))
        {
          якщо (errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          повернути child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      якщо (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        повернути ret;
    }

  повернути 0;
}

static ціле
rmdir_all (const char *path)
{
  ціле ret;
  ціле dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  якщо (UNLIKELY (dfd < 0))
    повернути dfd;

  ret = rmdir_all_fd (dfd);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути rmdir (path);
}

static ціле
read_pids_from_path (const char *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_close ціле dirfd = -1;
  size_t n_pids, allocated;
  ціле mode;
  ціле ret;

  якщо (path == NULL || *path == '\0')
    повернути 0;

  mode = libcrun_get_cgroup_mode (err);
  якщо (UNLIKELY (mode < 0))
    повернути mode;

  switch (mode)
    {
    case CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
      break;

    case CGROUP_MODE_HYBRID:
    case CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT "/memory", path, NULL);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
      break;

    default:
      повернути crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  якщо (dirfd < 0)
    повернути crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  якщо (threads && mode == CGROUP_MODE_UNIFIED)
    повернути read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  повернути read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

ціле
libcrun_cgroup_read_pids_from_path (const char *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  повернути read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (char *content, char **saveptr, char **id, char **controller_list, char **path)
{
  char *it;

  it = strtok_r (content, "\n", saveptr);
  якщо (it == NULL)
    повернути false;

  якщо (id)
    *id = it;

  it = strchr (it, ':');
  якщо (it == NULL)
    повернути false;
  *it++ = '\0';

  якщо (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  якщо (it == NULL)
    повернути false;
  *it++ = '\0';

  якщо (path)
    *path = it;

  повернути true;
}

ціле
destroy_cgroup_path (const char *path, ціле mode, libcrun_error_t *err)
{
  bool repeat = true;
  ціле retry_count = 0;
  const ціле max_attempts = 500;
  ціле ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  якщо (is_empty_string (path))
    повернути 0;

  do
    {
      repeat = false;

      якщо (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free char *cgroup_path = NULL;

          ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          ret = rmdir (cgroup_path);
          якщо (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              якщо (ret < 0)
                {
                  якщо (retry_count >= max_attempts)
                    повернути crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_count++;
                  repeat = true;
                }
            }
        }
      інакше
        {
          cleanup_free char *content = NULL;
          size_t content_size;
          char *controller;
          char *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          якщо (UNLIKELY (ret < 0))
            {
              якщо (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  повернути 0;
                }
              повернути ret;
            }

          для (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
               has_data;
               has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
            {
              cleanup_free char *cgroup_path = NULL;
              char *subsystem;
              якщо (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              якщо (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                continue;

              ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path, NULL);
              якщо (UNLIKELY (ret < 0))
                повернути ret;

              ret = rmdir (cgroup_path);
              якщо (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  якщо (ret < 0)
                    {
                      якщо (retry_count >= max_attempts)
                        повернути crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_count++;
                      repeat = true;
                    }
                }
            }
        }

      якщо (repeat)
        {
          struct timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, NULL);

          ret = cgroup_killall_path (path, SIGKILL, err);
          якщо (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } поки (repeat);

  повернути 0;
}

ціле
chown_cgroups (const char *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_free char *delegate = NULL;
  cleanup_close ціле dfd = -1;
  size_t delegate_size;
  char *saveptr = NULL;
  char *name;
  ціле ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  якщо (UNLIKELY (dfd < 0))
    повернути crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  якщо (UNLIKELY (ret < 0))
    {
      якщо (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          повернути 0;
        }
      повернути ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  для (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (NULL, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      якщо (UNLIKELY (ret < 0))
        {
          якщо (errno == ENOENT)
            continue;

          повернути crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  повернути 0;
}

static ціле
libcrun_cgroup_pause_unpause_with_mode (const char *cgroup_path, ціле cgroup_mode, const bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free char *path = NULL;
  const char *state = "";
  ціле ret;

  якщо (cgroup_path == NULL || cgroup_path[0] == '\0')
    повернути crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  якщо (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }
  інакше
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  якщо (ret >= 0)
    повернути 0;
  повернути ret;
}

ціле
libcrun_cgroup_pause_unpause_path (const char *cgroup_path, const bool pause, libcrun_error_t *err)
{
  ціле cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  якщо (UNLIKELY (cgroup_mode < 0))
    повернути cgroup_mode;

  повернути libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
ціле
libcrun_cgroup_ensure_not_frozen (const char *cgroup_path, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  cleanup_free char *path = NULL;
  const char *frozen;
  ціле cgroup_mode;
  ціле ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  якщо (UNLIKELY (cgroup_mode < 0))
    повернути cgroup_mode;

  якщо (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
    }
  інакше
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
    }
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = read_all_file (path, &content, NULL, err);
  якщо (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      якщо (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          повернути 0;
        }
      повернути ret;
    }

  якщо (strstr (content, frozen))
    повернути crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  повернути 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
static ціле
watch_cgroup_events (const char *path)
{
  cleanup_free char *events_file = NULL;
  cleanup_close ціле fd = -1;
  libcrun_error_t tmp_err = NULL;
  ціле ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  якщо (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      повернути -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  якщо (UNLIKELY (fd < 0))
    повернути -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  якщо (UNLIKELY (ret < 0))
    повернути -1;

  ret = fd;
  fd = -1;
  повернути ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
static ціле
wait_cgroup_unpopulated (const char *path, ціле inotify_fd)
{
  cleanup_free char *events_file = NULL;
  libcrun_error_t tmp_err = NULL;
  struct timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  ціле ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  якщо (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      повернути 0;
    }

  для (;;)
    {
      cleanup_free char *content = NULL;
      char buf[4096];
      struct pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, NULL, &tmp_err);
      якщо (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          повернути 0;
        }

      якщо (strstr (content, "populated 0"))
        повернути 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, NULL));
      якщо (ret <= 0)
        повернути 0;

      поки (read (inotify_fd, buf, sizeof (buf)) > 0)
        ;
    }
}

ціле
cgroup_killall_path (const char *path, ціле signal, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close ціле inotify_fd = -1;
  ціле ret;
  size_t i;
  cleanup_free pid_t *pids = NULL;

  якщо (path == NULL || *path == '\0')
    повернути 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  якщо (signal == SIGKILL)
    {
      cleanup_free char *kill_file = NULL;

      ret = append_paths (&kill_file, err, CGROUP_ROOT, path, "cgroup.kill", NULL);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      якщо (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        повернути 0;

      якщо (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  якщо (UNLIKELY (ret < 0))
    crun_error_release (err);
  інакше
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, signal == SIGKILL, &pids, err);
  якщо (UNLIKELY (ret < 0))
    {
      якщо (crun_error_get_errno (err) != ENOENT)
        goto thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  для (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], signal);
      якщо (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          goto thaw;
        }
    }

  ret = 0;

thaw:
  якщо (frozen)
    {
      libcrun_error_t thaw_err = NULL;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      якщо (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  повернути ret;
}

static ціле
read_available_controllers (const char *path, libcrun_error_t *err)
{
  cleanup_free char *controllers = NULL;
  cleanup_free char *buf = NULL;
  char *saveptr = NULL;
  const char *token;
  ціле available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ROOT, path, "cgroup.controllers", NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = read_all_file (controllers, &buf, NULL, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  для (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
    {
      якщо (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      інакше якщо (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      інакше якщо (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      інакше якщо (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      інакше якщо (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      інакше якщо (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      інакше якщо (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  повернути available;
}

static ціле
write_controller_file (const char *path, ціле controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free char *subtree_control = NULL;
  cleanup_free char *controllers = NULL;
  size_t controllers_len = 0;
  ціле ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ROOT, path, "cgroup.subtree_control", NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  якщо (UNLIKELY (ret < 0))
    {
      cleanup_free char *controllers_copy = xmalloc (controllers_len + 1);
      ціле attempts_left;
      char *saveptr = NULL;
      const char *token;
      ціле e;

      e = crun_error_get_errno (err);
      якщо (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        повернути crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      якщо (e == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;
          ціле exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          якщо (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              повернути ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          якщо (exists == 0)
            повернути ret;
        }

      crun_error_release (err);

      якщо (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          якщо (UNLIKELY (ret < 0))
            повернути crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      для (attempts_left = 1000; attempts_left >= 0; attempts_left--)
        {
          ціле controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          для (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (NULL, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              якщо (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  якщо (e == EBUSY)
                    repeat = true;

                  continue;
                }

              controllers_written++;
            }

          якщо (! repeat)
            break;

          /* If there was any controller written, just try once more without any delay.  */
          якщо (controllers_written > 0 && attempts_left > 2)
            {
              attempts_left = 1;
              continue;
            }

          якщо (attempts_left > 0)
            {
              struct timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, NULL);
            }
        }

      /* Refresh what controllers are available.  */
      повернути read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  повернути controllers_to_enable;
}

ціле
enable_controllers (const char *path, libcrun_error_t *err)
{
  cleanup_free char *tmp_path = NULL;
  char *it;
  ціле ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  якщо (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      якщо ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }

  для (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free char *cgroup_path = NULL;
      char *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, tmp_path, NULL);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      ret = mkdir (cgroup_path, 0755);
      якщо (UNLIKELY (ret < 0 && errno != EEXIST))
        повернути crun_make_error (err, errno, "create `%s`", cgroup_path);

      якщо (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
        }

      *it = '/';
      it = next_slash;
    }
  повернути 0;
}

ціле
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, const char *path, bool create_if_missing, libcrun_error_t *err)
{
  ціле cgroup_mode = libcrun_get_cgroup_mode (err);
  якщо (UNLIKELY (cgroup_mode < 0))
    повернути cgroup_mode;

  якщо (path == NULL || *path == '\0')
    повернути 0;

  повернути enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
ціле
libcrun_move_self_to_cgroups (const char *content, libcrun_error_t *err)
{
  cleanup_free char *buf = xstrdup (content);
  char *saveptr = NULL;
  char *controller;
  char *path;
  ціле cgroup_mode;
  bool has_data;
  ціле ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  якщо (UNLIKELY (cgroup_mode < 0))
    повернути cgroup_mode;

  для (has_data = read_proc_cgroup (buf, &saveptr, NULL, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &path))
    {
      const char *subsystem = NULL;

      якщо (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          якщо (controller[0] != '\0')
            continue;
        }
      інакше
        {
          cleanup_free char *subsystem_path = NULL;

          /* Use the same names as enter_cgroup_v1 does.  */
          якщо (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          якщо (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          якщо (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ROOT, subsystem, NULL);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          ret = crun_path_exists (subsystem_path, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          якщо (ret == 0)
            continue;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }

  повернути 0;
}

ціле
libcrun_get_cgroup_dirfd (struct libcrun_cgroup_status *status, const char *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free char *path_to_cgroup = NULL;
  ціле cgroup_mode;
  ціле cgroupdirfd;
  ціле ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  якщо (cgroup_mode < 0)
    повернути cgroup_mode;

  якщо (cgroup_mode != CGROUP_MODE_UNIFIED)
    повернути crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  якщо (status == NULL)
    повернути crun_make_error (err, 0, "internal error");

  якщо (is_empty_string (status->path))
    повернути crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ROOT, status->path, sub_cgroup, NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  якщо (UNLIKELY (cgroupdirfd < 0))
    повернути crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  повернути cgroupdirfd;
}

ціле
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, char *from, char *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = NULL;
  cleanup_close ціле child_dfd = -1;
  ціле cgroup_mode;
  size_t from_len;
  size_t i;
  ціле ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  якщо (cgroup_mode < 0)
    повернути cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  якщо (UNLIKELY (ret < 0))
    goto thaw;

  from_len = strlen (from);

  для (i = 0; pids && pids[i]; i++)
    {
      cleanup_free char *pid_path = NULL;
      cleanup_free char *dest_cgroup = NULL;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      якщо (UNLIKELY (ret < 0))
        goto thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      якщо (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          goto thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      якщо (UNLIKELY (ret < 0))
        goto thaw;
    }

  ret = 0;

thaw:
  якщо (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = NULL;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      якщо (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      повернути ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути destroy_cgroup_path (from, cgroup_mode, err);
}

ціле
get_cgroup_dirfd_path (ціле dirfd, char **path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  якщо (UNLIKELY (len < 0))
    повернути len;

  якщо (has_prefix (cgroup_path, CGROUP_ROOT))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ROOT));
      повернути 0;
    }
  повернути crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
ціле
parse_cpu_max (const char *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  const char *it = value;
  char *endptr = NULL;

  поки (*it == ' ')
    it++;

  errno = 0;
  якщо (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (char *) it + 3;
    }
  інакше
    {
      *quota = strtoll (it, &endptr, 10);
      якщо (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        повернути crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
    }

  it = endptr;
  поки (*it == ' ')
    it++;

  *has_period = false;
  якщо (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      якщо (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        повернути crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
      *has_period = true;
    }

  повернути 0;
}
