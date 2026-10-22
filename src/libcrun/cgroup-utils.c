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

整数
libcrun_cgroups_create_symlinks (整数 dirfd, libcrun_error_t *err)
{
  整数 i;

  为 (i = 0; cgroup_symlinks[i].name; i++)
    {
      整数 ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      如果 (UNLIKELY (ret < 0))
        {
          如果 (errno == ENOENT || errno == EEXIST)
            continue;
          返回 crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  返回 0;
}

整数
maybe_make_cgroup_threaded (const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_type = NULL;
  const char *const threaded = "threaded";
  cleanup_free char *content = NULL;
  cleanup_free char *buffer = NULL;
  const char *parent;
  size_t size;
  char *it;
  整数 ret;

  如果 (path == NULL)
    返回 0;

  path = consume_slashes (path);

  如果 (path[0] == '\0')
    返回 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ROOT, path, "cgroup.type", NULL);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  如果 (size > 0)
    {
      it = content + size - 1;
      当 (*it == '\n' && it > content)
        *it-- = '\0';
    }

  如果 (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    返回 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  如果 (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      如果 (ret < 0)
        返回 ret;
    }
  返回 write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

整数
move_process_to_cgroup (pid_t pid, const char *subsystem, const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_procs = NULL;
  char pid_str[16];
  整数 ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ROOT,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", NULL);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  ret = snprintf (pid_str, sizeof (pid_str), "%d", pid);
  如果 (UNLIKELY (ret >= (整数) sizeof (pid_str)))
    返回 crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  如果 (UNLIKELY (ret < 0))
    {
      如果 (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = NULL;
          整数 mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          如果 (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              返回 ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;

          返回 write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      返回 ret;
    }
  返回 ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

static 整数
detect_cgroup_mode (libcrun_error_t *err)
{
  struct statfs stat;
  整数 ret;

  ret = statfs (CGROUP_ROOT, &stat);
  如果 (ret < 0)
    返回 crun_make_error (err, errno, "statfs `" CGROUP_ROOT "`");
  如果 (stat.f_type == CGROUP2_SUPER_MAGIC)
    返回 CGROUP_MODE_UNIFIED;
  如果 (stat.f_type != TMPFS_MAGIC)
    返回 crun_make_error (err, 0, "invalid file system type on `" CGROUP_ROOT "`");
  ret = statfs (CGROUP_ROOT "/unified", &stat);
  如果 (ret < 0 && errno != ENOENT)
    返回 crun_make_error (err, errno, "statfs `" CGROUP_ROOT "/unified`");
  如果 (ret < 0)
    返回 CGROUP_MODE_LEGACY;
  返回 stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

整数
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  整数 tmp;
  static 整数 cgroup_mode = 0;

  如果 (cgroup_mode)
    返回 cgroup_mode;

  tmp = detect_cgroup_mode (err);
  如果 (UNLIKELY (tmp < 0))
    返回 tmp;

  cgroup_mode = tmp;

  返回 cgroup_mode;
}

static 整数
get_cgroup_process_from_content (char *content, 整数 cgroup_mode, char **path, bool absolute, libcrun_error_t *err)
{
  char *cg_path = NULL;
  char *controller;
  char *saveptr;
  bool has_data;

  为 (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &cg_path))
    {
      如果 (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          如果 (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            goto found;
        }
      否则
        {
          如果 (strcmp (controller, "memory"))
            goto found;
        }
    }

  返回 crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  如果 (absolute)
    返回 append_paths (path, err, CGROUP_ROOT, cg_path, NULL);

  *path = xstrdup (cg_path);
  返回 0;
}

整数
libcrun_get_cgroup_process_at (整数 dirfd, char **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  size_t content_size;
  整数 cgroup_mode;
  整数 ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  如果 (UNLIKELY (cgroup_mode < 0))
    返回 cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  返回 get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

整数
libcrun_get_cgroup_process (pid_t pid, char **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  char proc_cgroup_file[64];
  size_t content_size;
  整数 cgroup_mode;
  整数 ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  如果 (UNLIKELY (cgroup_mode < 0))
    返回 cgroup_mode;

  如果 (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  否则
    {
      整数 len = snprintf (proc_cgroup_file, sizeof (proc_cgroup_file), "/proc/%d/cgroup", pid);
      如果 (UNLIKELY (len >= (整数) sizeof (proc_cgroup_file)))
        返回 crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  返回 get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

static 整数
read_pids_file (整数 dfd, const char *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close 整数 tasksfd = -1;
  cleanup_free char *buffer = NULL;
  char *saveptr = NULL;
  size_t n_new_pids;
  size_t len;
  char *it;
  整数 ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  如果 (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      如果 (errno == ENOENT || errno == ENODEV)
        返回 0;
      返回 crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  如果 (len > 0)
    {
      为 (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      如果 (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, sizeof (pid_t) * *allocated);
        }

      为 (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (NULL, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, NULL, 10);

          如果 (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  返回 0;
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
  整数 dfd;
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
  整数 saved_errno = errno;

  当 (walk->n_dirs > 0)
    cgroup_walk_pop (walk);
  free (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
static 整数
cgroup_walk_enter (struct cgroup_walk_s *walk, 整数 dfd)
{
  cleanup_close 整数 dfd_clone = -1;
  cleanup_dir DIR *dir = NULL;
  struct dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  如果 (UNLIKELY (dfd_clone < 0))
    返回 -1;

  dir = fdopendir (dfd_clone);
  如果 (UNLIKELY (dir == NULL))
    返回 -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  为 (de = readdir (dir); de; de = readdir (dir))
    {
      struct cgroup_walk_dir_s *d;

      如果 (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      如果 (de->d_type != DT_DIR)
        continue;

      如果 (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, sizeof (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  返回 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
static 整数
cgroup_walk_next (struct cgroup_walk_s *walk, struct cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : NULL;

  当 (*d && walk->depth >= (*d)->depth)
    {
      整数 parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      如果 (UNLIKELY (parent < 0))
        返回 -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  返回 0;
}

static 整数
read_pids_cgroup_file (整数 dfd, const char *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk struct cgroup_walk_s walk = { .dfd = -1 };
  整数 dfd_clone;
  整数 ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  如果 (! recurse)
    返回 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  如果 (UNLIKELY (dfd_clone < 0))
    返回 crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "open cgroup sub-directory");

  为 (;;)
    {
      struct cgroup_walk_dir_s *d;
      cleanup_close 整数 nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      如果 (UNLIKELY (ret < 0))
        返回 crun_make_error (err, errno, "open parent cgroup directory");
      如果 (d == NULL)
        break;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      如果 (UNLIKELY (nfd < 0))
        {
          整数 saved_errno = errno;

          如果 (saved_errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          返回 crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_pop (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      如果 (UNLIKELY (ret < 0))
        {
          整数 e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          如果 (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            返回 ret;
          crun_error_release (err);
          continue;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      如果 (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        返回 crun_make_error (err, errno, "open cgroup sub-directory");
    }

  返回 0;
}

static 整数
read_pids_cgroup (整数 dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  整数 ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  如果 (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  返回 ret;
}

static 整数
rmdir_all_fd (整数 dfd)
{
  cleanup_cgroup_walk struct cgroup_walk_s walk = { .dfd = -1 };
  整数 ret;

  ret = cgroup_walk_enter (&walk, dfd);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  为 (;;)
    {
      struct cgroup_walk_dir_s *d;
      整数 child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      如果 (UNLIKELY (ret < 0))
        返回 ret;
      如果 (d == NULL)
        break;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      如果 (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_pop (&walk);
          continue;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      如果 (UNLIKELY (child_dfd < 0))
        {
          如果 (errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          返回 child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      如果 (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        返回 ret;
    }

  返回 0;
}

static 整数
rmdir_all (const char *path)
{
  整数 ret;
  整数 dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  如果 (UNLIKELY (dfd < 0))
    返回 dfd;

  ret = rmdir_all_fd (dfd);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  返回 rmdir (path);
}

static 整数
read_pids_from_path (const char *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_close 整数 dirfd = -1;
  size_t n_pids, allocated;
  整数 mode;
  整数 ret;

  如果 (path == NULL || *path == '\0')
    返回 0;

  mode = libcrun_get_cgroup_mode (err);
  如果 (UNLIKELY (mode < 0))
    返回 mode;

  switch (mode)
    {
    case CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
      如果 (UNLIKELY (ret < 0))
        返回 ret;
      break;

    case CGROUP_MODE_HYBRID:
    case CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT "/memory", path, NULL);
      如果 (UNLIKELY (ret < 0))
        返回 ret;
      break;

    default:
      返回 crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  如果 (dirfd < 0)
    返回 crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  如果 (threads && mode == CGROUP_MODE_UNIFIED)
    返回 read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  返回 read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

整数
libcrun_cgroup_read_pids_from_path (const char *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  返回 read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (char *content, char **saveptr, char **id, char **controller_list, char **path)
{
  char *it;

  it = strtok_r (content, "\n", saveptr);
  如果 (it == NULL)
    返回 false;

  如果 (id)
    *id = it;

  it = strchr (it, ':');
  如果 (it == NULL)
    返回 false;
  *it++ = '\0';

  如果 (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  如果 (it == NULL)
    返回 false;
  *it++ = '\0';

  如果 (path)
    *path = it;

  返回 true;
}

整数
destroy_cgroup_path (const char *path, 整数 mode, libcrun_error_t *err)
{
  bool repeat = true;
  整数 retry_count = 0;
  const 整数 max_attempts = 500;
  整数 ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  如果 (is_empty_string (path))
    返回 0;

  do
    {
      repeat = false;

      如果 (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free char *cgroup_path = NULL;

          ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
          如果 (UNLIKELY (ret < 0))
            返回 ret;
          ret = rmdir (cgroup_path);
          如果 (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              如果 (ret < 0)
                {
                  如果 (retry_count >= max_attempts)
                    返回 crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_count++;
                  repeat = true;
                }
            }
        }
      否则
        {
          cleanup_free char *content = NULL;
          size_t content_size;
          char *controller;
          char *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          如果 (UNLIKELY (ret < 0))
            {
              如果 (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  返回 0;
                }
              返回 ret;
            }

          为 (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
               has_data;
               has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
            {
              cleanup_free char *cgroup_path = NULL;
              char *subsystem;
              如果 (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              如果 (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                continue;

              ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path, NULL);
              如果 (UNLIKELY (ret < 0))
                返回 ret;

              ret = rmdir (cgroup_path);
              如果 (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  如果 (ret < 0)
                    {
                      如果 (retry_count >= max_attempts)
                        返回 crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_count++;
                      repeat = true;
                    }
                }
            }
        }

      如果 (repeat)
        {
          struct timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, NULL);

          ret = cgroup_killall_path (path, SIGKILL, err);
          如果 (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } 当 (repeat);

  返回 0;
}

整数
chown_cgroups (const char *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_free char *delegate = NULL;
  cleanup_close 整数 dfd = -1;
  size_t delegate_size;
  char *saveptr = NULL;
  char *name;
  整数 ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  如果 (UNLIKELY (dfd < 0))
    返回 crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  如果 (UNLIKELY (ret < 0))
    {
      如果 (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          返回 0;
        }
      返回 ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  为 (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (NULL, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      如果 (UNLIKELY (ret < 0))
        {
          如果 (errno == ENOENT)
            continue;

          返回 crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  返回 0;
}

static 整数
libcrun_cgroup_pause_unpause_with_mode (const char *cgroup_path, 整数 cgroup_mode, const bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free char *path = NULL;
  const char *state = "";
  整数 ret;

  如果 (cgroup_path == NULL || cgroup_path[0] == '\0')
    返回 crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  如果 (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      如果 (UNLIKELY (ret < 0))
        返回 ret;
    }
  否则
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      如果 (UNLIKELY (ret < 0))
        返回 ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  如果 (ret >= 0)
    返回 0;
  返回 ret;
}

整数
libcrun_cgroup_pause_unpause_path (const char *cgroup_path, const bool pause, libcrun_error_t *err)
{
  整数 cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  如果 (UNLIKELY (cgroup_mode < 0))
    返回 cgroup_mode;

  返回 libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
整数
libcrun_cgroup_ensure_not_frozen (const char *cgroup_path, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  cleanup_free char *path = NULL;
  const char *frozen;
  整数 cgroup_mode;
  整数 ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  如果 (UNLIKELY (cgroup_mode < 0))
    返回 cgroup_mode;

  如果 (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
    }
  否则
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
    }
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  ret = read_all_file (path, &content, NULL, err);
  如果 (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      如果 (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          返回 0;
        }
      返回 ret;
    }

  如果 (strstr (content, frozen))
    返回 crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  返回 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
static 整数
watch_cgroup_events (const char *path)
{
  cleanup_free char *events_file = NULL;
  cleanup_close 整数 fd = -1;
  libcrun_error_t tmp_err = NULL;
  整数 ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  如果 (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      返回 -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  如果 (UNLIKELY (fd < 0))
    返回 -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  如果 (UNLIKELY (ret < 0))
    返回 -1;

  ret = fd;
  fd = -1;
  返回 ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
static 整数
wait_cgroup_unpopulated (const char *path, 整数 inotify_fd)
{
  cleanup_free char *events_file = NULL;
  libcrun_error_t tmp_err = NULL;
  struct timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  整数 ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  如果 (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      返回 0;
    }

  为 (;;)
    {
      cleanup_free char *content = NULL;
      char buf[4096];
      struct pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, NULL, &tmp_err);
      如果 (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          返回 0;
        }

      如果 (strstr (content, "populated 0"))
        返回 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, NULL));
      如果 (ret <= 0)
        返回 0;

      当 (read (inotify_fd, buf, sizeof (buf)) > 0)
        ;
    }
}

整数
cgroup_killall_path (const char *path, 整数 signal, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close 整数 inotify_fd = -1;
  整数 ret;
  size_t i;
  cleanup_free pid_t *pids = NULL;

  如果 (path == NULL || *path == '\0')
    返回 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  如果 (signal == SIGKILL)
    {
      cleanup_free char *kill_file = NULL;

      ret = append_paths (&kill_file, err, CGROUP_ROOT, path, "cgroup.kill", NULL);
      如果 (UNLIKELY (ret < 0))
        返回 ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      如果 (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        返回 0;

      如果 (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  如果 (UNLIKELY (ret < 0))
    crun_error_release (err);
  否则
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, signal == SIGKILL, &pids, err);
  如果 (UNLIKELY (ret < 0))
    {
      如果 (crun_error_get_errno (err) != ENOENT)
        goto thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  为 (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], signal);
      如果 (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          goto thaw;
        }
    }

  ret = 0;

thaw:
  如果 (frozen)
    {
      libcrun_error_t thaw_err = NULL;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      如果 (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  返回 ret;
}

static 整数
read_available_controllers (const char *path, libcrun_error_t *err)
{
  cleanup_free char *controllers = NULL;
  cleanup_free char *buf = NULL;
  char *saveptr = NULL;
  const char *token;
  整数 available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ROOT, path, "cgroup.controllers", NULL);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  ret = read_all_file (controllers, &buf, NULL, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  为 (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
    {
      如果 (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      否则 如果 (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      否则 如果 (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      否则 如果 (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      否则 如果 (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      否则 如果 (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      否则 如果 (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  返回 available;
}

static 整数
write_controller_file (const char *path, 整数 controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free char *subtree_control = NULL;
  cleanup_free char *controllers = NULL;
  size_t controllers_len = 0;
  整数 ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ROOT, path, "cgroup.subtree_control", NULL);
  如果 (UNLIKELY (ret < 0))
    返回 ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  如果 (UNLIKELY (ret < 0))
    {
      cleanup_free char *controllers_copy = xmalloc (controllers_len + 1);
      整数 attempts_left;
      char *saveptr = NULL;
      const char *token;
      整数 e;

      e = crun_error_get_errno (err);
      如果 (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        返回 crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      如果 (e == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;
          整数 exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          如果 (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              返回 ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          如果 (exists == 0)
            返回 ret;
        }

      crun_error_release (err);

      如果 (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          如果 (UNLIKELY (ret < 0))
            返回 crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      为 (attempts_left = 1000; attempts_left >= 0; attempts_left--)
        {
          整数 controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          为 (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (NULL, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              如果 (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  如果 (e == EBUSY)
                    repeat = true;

                  continue;
                }

              controllers_written++;
            }

          如果 (! repeat)
            break;

          /* If there was any controller written, just try once more without any delay.  */
          如果 (controllers_written > 0 && attempts_left > 2)
            {
              attempts_left = 1;
              continue;
            }

          如果 (attempts_left > 0)
            {
              struct timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, NULL);
            }
        }

      /* Refresh what controllers are available.  */
      返回 read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  返回 controllers_to_enable;
}

整数
enable_controllers (const char *path, libcrun_error_t *err)
{
  cleanup_free char *tmp_path = NULL;
  char *it;
  整数 ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  如果 (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      如果 ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      如果 (UNLIKELY (ret < 0))
        返回 ret;
    }

  为 (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free char *cgroup_path = NULL;
      char *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, tmp_path, NULL);
      如果 (UNLIKELY (ret < 0))
        返回 ret;

      ret = mkdir (cgroup_path, 0755);
      如果 (UNLIKELY (ret < 0 && errno != EEXIST))
        返回 crun_make_error (err, errno, "create `%s`", cgroup_path);

      如果 (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;
        }

      *it = '/';
      it = next_slash;
    }
  返回 0;
}

整数
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, const char *path, bool create_if_missing, libcrun_error_t *err)
{
  整数 cgroup_mode = libcrun_get_cgroup_mode (err);
  如果 (UNLIKELY (cgroup_mode < 0))
    返回 cgroup_mode;

  如果 (path == NULL || *path == '\0')
    返回 0;

  返回 enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
整数
libcrun_move_self_to_cgroups (const char *content, libcrun_error_t *err)
{
  cleanup_free char *buf = xstrdup (content);
  char *saveptr = NULL;
  char *controller;
  char *path;
  整数 cgroup_mode;
  bool has_data;
  整数 ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  如果 (UNLIKELY (cgroup_mode < 0))
    返回 cgroup_mode;

  为 (has_data = read_proc_cgroup (buf, &saveptr, NULL, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &path))
    {
      const char *subsystem = NULL;

      如果 (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          如果 (controller[0] != '\0')
            continue;
        }
      否则
        {
          cleanup_free char *subsystem_path = NULL;

          /* Use the same names as enter_cgroup_v1 does.  */
          如果 (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          如果 (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          如果 (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ROOT, subsystem, NULL);
          如果 (UNLIKELY (ret < 0))
            返回 ret;
          ret = crun_path_exists (subsystem_path, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;
          如果 (ret == 0)
            continue;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      如果 (UNLIKELY (ret < 0))
        返回 ret;
    }

  返回 0;
}

整数
libcrun_get_cgroup_dirfd (struct libcrun_cgroup_status *status, const char *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free char *path_to_cgroup = NULL;
  整数 cgroup_mode;
  整数 cgroupdirfd;
  整数 ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  如果 (cgroup_mode < 0)
    返回 cgroup_mode;

  如果 (cgroup_mode != CGROUP_MODE_UNIFIED)
    返回 crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  如果 (status == NULL)
    返回 crun_make_error (err, 0, "internal error");

  如果 (is_empty_string (status->path))
    返回 crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ROOT, status->path, sub_cgroup, NULL);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  如果 (UNLIKELY (cgroupdirfd < 0))
    返回 crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  返回 cgroupdirfd;
}

整数
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, char *from, char *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = NULL;
  cleanup_close 整数 child_dfd = -1;
  整数 cgroup_mode;
  size_t from_len;
  size_t i;
  整数 ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  如果 (cgroup_mode < 0)
    返回 cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  如果 (UNLIKELY (ret < 0))
    goto thaw;

  from_len = strlen (from);

  为 (i = 0; pids && pids[i]; i++)
    {
      cleanup_free char *pid_path = NULL;
      cleanup_free char *dest_cgroup = NULL;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      如果 (UNLIKELY (ret < 0))
        goto thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      如果 (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          goto thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      如果 (UNLIKELY (ret < 0))
        goto thaw;
    }

  ret = 0;

thaw:
  如果 (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = NULL;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      如果 (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      返回 ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  返回 destroy_cgroup_path (from, cgroup_mode, err);
}

整数
get_cgroup_dirfd_path (整数 dirfd, char **path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  如果 (UNLIKELY (len < 0))
    返回 len;

  如果 (has_prefix (cgroup_path, CGROUP_ROOT))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ROOT));
      返回 0;
    }
  返回 crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
整数
parse_cpu_max (const char *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  const char *it = value;
  char *endptr = NULL;

  当 (*it == ' ')
    it++;

  errno = 0;
  如果 (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (char *) it + 3;
    }
  否则
    {
      *quota = strtoll (it, &endptr, 10);
      如果 (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        返回 crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
    }

  it = endptr;
  当 (*it == ' ')
    it++;

  *has_period = false;
  如果 (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      如果 (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        返回 crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
      *has_period = true;
    }

  返回 0;
}
