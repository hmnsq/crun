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

số_nguyên
libcrun_cgroups_create_symlinks (số_nguyên dirfd, libcrun_error_t *err)
{
  số_nguyên i;

  cho (i = 0; cgroup_symlinks[i].name; i++)
    {
      số_nguyên ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      nếu (UNLIKELY (ret < 0))
        {
          nếu (errno == ENOENT || errno == EEXIST)
            continue;
          trả_về crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  trả_về 0;
}

số_nguyên
maybe_make_cgroup_threaded (const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_type = NULL;
  const char *const threaded = "threaded";
  cleanup_free char *content = NULL;
  cleanup_free char *buffer = NULL;
  const char *parent;
  size_t size;
  char *it;
  số_nguyên ret;

  nếu (path == NULL)
    trả_về 0;

  path = consume_slashes (path);

  nếu (path[0] == '\0')
    trả_về 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ROOT, path, "cgroup.type", NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (size > 0)
    {
      it = content + size - 1;
      trong_khi (*it == '\n' && it > content)
        *it-- = '\0';
    }

  nếu (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    trả_về 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  nếu (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      nếu (ret < 0)
        trả_về ret;
    }
  trả_về write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

số_nguyên
move_process_to_cgroup (pid_t pid, const char *subsystem, const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_procs = NULL;
  char pid_str[16];
  số_nguyên ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ROOT,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = snprintf (pid_str, sizeof (pid_str), "%d", pid);
  nếu (UNLIKELY (ret >= (số_nguyên) sizeof (pid_str)))
    trả_về crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  nếu (UNLIKELY (ret < 0))
    {
      nếu (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = NULL;
          số_nguyên mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          nếu (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              trả_về ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;

          trả_về write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      trả_về ret;
    }
  trả_về ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

static số_nguyên
detect_cgroup_mode (libcrun_error_t *err)
{
  struct statfs stat;
  số_nguyên ret;

  ret = statfs (CGROUP_ROOT, &stat);
  nếu (ret < 0)
    trả_về crun_make_error (err, errno, "statfs `" CGROUP_ROOT "`");
  nếu (stat.f_type == CGROUP2_SUPER_MAGIC)
    trả_về CGROUP_MODE_UNIFIED;
  nếu (stat.f_type != TMPFS_MAGIC)
    trả_về crun_make_error (err, 0, "invalid file system type on `" CGROUP_ROOT "`");
  ret = statfs (CGROUP_ROOT "/unified", &stat);
  nếu (ret < 0 && errno != ENOENT)
    trả_về crun_make_error (err, errno, "statfs `" CGROUP_ROOT "/unified`");
  nếu (ret < 0)
    trả_về CGROUP_MODE_LEGACY;
  trả_về stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

số_nguyên
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  số_nguyên tmp;
  static số_nguyên cgroup_mode = 0;

  nếu (cgroup_mode)
    trả_về cgroup_mode;

  tmp = detect_cgroup_mode (err);
  nếu (UNLIKELY (tmp < 0))
    trả_về tmp;

  cgroup_mode = tmp;

  trả_về cgroup_mode;
}

static số_nguyên
get_cgroup_process_from_content (char *content, số_nguyên cgroup_mode, char **path, bool absolute, libcrun_error_t *err)
{
  char *cg_path = NULL;
  char *controller;
  char *saveptr;
  bool has_data;

  cho (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &cg_path))
    {
      nếu (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          nếu (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            goto found;
        }
      không_thì
        {
          nếu (strcmp (controller, "memory"))
            goto found;
        }
    }

  trả_về crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  nếu (absolute)
    trả_về append_paths (path, err, CGROUP_ROOT, cg_path, NULL);

  *path = xstrdup (cg_path);
  trả_về 0;
}

số_nguyên
libcrun_get_cgroup_process_at (số_nguyên dirfd, char **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  size_t content_size;
  số_nguyên cgroup_mode;
  số_nguyên ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  trả_về get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

số_nguyên
libcrun_get_cgroup_process (pid_t pid, char **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  char proc_cgroup_file[64];
  size_t content_size;
  số_nguyên cgroup_mode;
  số_nguyên ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;

  nếu (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  không_thì
    {
      số_nguyên len = snprintf (proc_cgroup_file, sizeof (proc_cgroup_file), "/proc/%d/cgroup", pid);
      nếu (UNLIKELY (len >= (số_nguyên) sizeof (proc_cgroup_file)))
        trả_về crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  trả_về get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

static số_nguyên
read_pids_file (số_nguyên dfd, const char *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close số_nguyên tasksfd = -1;
  cleanup_free char *buffer = NULL;
  char *saveptr = NULL;
  size_t n_new_pids;
  size_t len;
  char *it;
  số_nguyên ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  nếu (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      nếu (errno == ENOENT || errno == ENODEV)
        trả_về 0;
      trả_về crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (len > 0)
    {
      cho (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      nếu (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, sizeof (pid_t) * *allocated);
        }

      cho (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (NULL, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, NULL, 10);

          nếu (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  trả_về 0;
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
  số_nguyên dfd;
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
  số_nguyên saved_errno = errno;

  trong_khi (walk->n_dirs > 0)
    cgroup_walk_pop (walk);
  free (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
static số_nguyên
cgroup_walk_enter (struct cgroup_walk_s *walk, số_nguyên dfd)
{
  cleanup_close số_nguyên dfd_clone = -1;
  cleanup_dir DIR *dir = NULL;
  struct dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  nếu (UNLIKELY (dfd_clone < 0))
    trả_về -1;

  dir = fdopendir (dfd_clone);
  nếu (UNLIKELY (dir == NULL))
    trả_về -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  cho (de = readdir (dir); de; de = readdir (dir))
    {
      struct cgroup_walk_dir_s *d;

      nếu (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      nếu (de->d_type != DT_DIR)
        continue;

      nếu (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, sizeof (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  trả_về 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
static số_nguyên
cgroup_walk_next (struct cgroup_walk_s *walk, struct cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : NULL;

  trong_khi (*d && walk->depth >= (*d)->depth)
    {
      số_nguyên parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      nếu (UNLIKELY (parent < 0))
        trả_về -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  trả_về 0;
}

static số_nguyên
read_pids_cgroup_file (số_nguyên dfd, const char *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk struct cgroup_walk_s walk = { .dfd = -1 };
  số_nguyên dfd_clone;
  số_nguyên ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (! recurse)
    trả_về 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  nếu (UNLIKELY (dfd_clone < 0))
    trả_về crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "open cgroup sub-directory");

  cho (;;)
    {
      struct cgroup_walk_dir_s *d;
      cleanup_close số_nguyên nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      nếu (UNLIKELY (ret < 0))
        trả_về crun_make_error (err, errno, "open parent cgroup directory");
      nếu (d == NULL)
        break;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      nếu (UNLIKELY (nfd < 0))
        {
          số_nguyên saved_errno = errno;

          nếu (saved_errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          trả_về crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_pop (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      nếu (UNLIKELY (ret < 0))
        {
          số_nguyên e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          nếu (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            trả_về ret;
          crun_error_release (err);
          continue;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      nếu (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        trả_về crun_make_error (err, errno, "open cgroup sub-directory");
    }

  trả_về 0;
}

static số_nguyên
read_pids_cgroup (số_nguyên dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  số_nguyên ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  nếu (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  trả_về ret;
}

static số_nguyên
rmdir_all_fd (số_nguyên dfd)
{
  cleanup_cgroup_walk struct cgroup_walk_s walk = { .dfd = -1 };
  số_nguyên ret;

  ret = cgroup_walk_enter (&walk, dfd);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  cho (;;)
    {
      struct cgroup_walk_dir_s *d;
      số_nguyên child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
      nếu (d == NULL)
        break;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      nếu (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_pop (&walk);
          continue;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      nếu (UNLIKELY (child_dfd < 0))
        {
          nếu (errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          trả_về child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      nếu (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        trả_về ret;
    }

  trả_về 0;
}

static số_nguyên
rmdir_all (const char *path)
{
  số_nguyên ret;
  số_nguyên dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  nếu (UNLIKELY (dfd < 0))
    trả_về dfd;

  ret = rmdir_all_fd (dfd);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  trả_về rmdir (path);
}

static số_nguyên
read_pids_from_path (const char *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_close số_nguyên dirfd = -1;
  size_t n_pids, allocated;
  số_nguyên mode;
  số_nguyên ret;

  nếu (path == NULL || *path == '\0')
    trả_về 0;

  mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (mode < 0))
    trả_về mode;

  switch (mode)
    {
    case CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
      break;

    case CGROUP_MODE_HYBRID:
    case CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT "/memory", path, NULL);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
      break;

    default:
      trả_về crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  nếu (dirfd < 0)
    trả_về crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  nếu (threads && mode == CGROUP_MODE_UNIFIED)
    trả_về read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  trả_về read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

số_nguyên
libcrun_cgroup_read_pids_from_path (const char *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  trả_về read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (char *content, char **saveptr, char **id, char **controller_list, char **path)
{
  char *it;

  it = strtok_r (content, "\n", saveptr);
  nếu (it == NULL)
    trả_về false;

  nếu (id)
    *id = it;

  it = strchr (it, ':');
  nếu (it == NULL)
    trả_về false;
  *it++ = '\0';

  nếu (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  nếu (it == NULL)
    trả_về false;
  *it++ = '\0';

  nếu (path)
    *path = it;

  trả_về true;
}

số_nguyên
destroy_cgroup_path (const char *path, số_nguyên mode, libcrun_error_t *err)
{
  bool repeat = true;
  số_nguyên retry_count = 0;
  const số_nguyên max_attempts = 500;
  số_nguyên ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  nếu (is_empty_string (path))
    trả_về 0;

  do
    {
      repeat = false;

      nếu (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free char *cgroup_path = NULL;

          ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
          ret = rmdir (cgroup_path);
          nếu (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              nếu (ret < 0)
                {
                  nếu (retry_count >= max_attempts)
                    trả_về crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_count++;
                  repeat = true;
                }
            }
        }
      không_thì
        {
          cleanup_free char *content = NULL;
          size_t content_size;
          char *controller;
          char *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          nếu (UNLIKELY (ret < 0))
            {
              nếu (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  trả_về 0;
                }
              trả_về ret;
            }

          cho (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
               has_data;
               has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
            {
              cleanup_free char *cgroup_path = NULL;
              char *subsystem;
              nếu (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              nếu (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                continue;

              ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path, NULL);
              nếu (UNLIKELY (ret < 0))
                trả_về ret;

              ret = rmdir (cgroup_path);
              nếu (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  nếu (ret < 0)
                    {
                      nếu (retry_count >= max_attempts)
                        trả_về crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_count++;
                      repeat = true;
                    }
                }
            }
        }

      nếu (repeat)
        {
          struct timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, NULL);

          ret = cgroup_killall_path (path, SIGKILL, err);
          nếu (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } trong_khi (repeat);

  trả_về 0;
}

số_nguyên
chown_cgroups (const char *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_free char *delegate = NULL;
  cleanup_close số_nguyên dfd = -1;
  size_t delegate_size;
  char *saveptr = NULL;
  char *name;
  số_nguyên ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  nếu (UNLIKELY (dfd < 0))
    trả_về crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  nếu (UNLIKELY (ret < 0))
    {
      nếu (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          trả_về 0;
        }
      trả_về ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  cho (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (NULL, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      nếu (UNLIKELY (ret < 0))
        {
          nếu (errno == ENOENT)
            continue;

          trả_về crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  trả_về 0;
}

static số_nguyên
libcrun_cgroup_pause_unpause_with_mode (const char *cgroup_path, số_nguyên cgroup_mode, const bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free char *path = NULL;
  const char *state = "";
  số_nguyên ret;

  nếu (cgroup_path == NULL || cgroup_path[0] == '\0')
    trả_về crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  nếu (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
    }
  không_thì
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  nếu (ret >= 0)
    trả_về 0;
  trả_về ret;
}

số_nguyên
libcrun_cgroup_pause_unpause_path (const char *cgroup_path, const bool pause, libcrun_error_t *err)
{
  số_nguyên cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;

  trả_về libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
số_nguyên
libcrun_cgroup_ensure_not_frozen (const char *cgroup_path, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  cleanup_free char *path = NULL;
  const char *frozen;
  số_nguyên cgroup_mode;
  số_nguyên ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;

  nếu (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
    }
  không_thì
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
    }
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = read_all_file (path, &content, NULL, err);
  nếu (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      nếu (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          trả_về 0;
        }
      trả_về ret;
    }

  nếu (strstr (content, frozen))
    trả_về crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  trả_về 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
static số_nguyên
watch_cgroup_events (const char *path)
{
  cleanup_free char *events_file = NULL;
  cleanup_close số_nguyên fd = -1;
  libcrun_error_t tmp_err = NULL;
  số_nguyên ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  nếu (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      trả_về -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  nếu (UNLIKELY (fd < 0))
    trả_về -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  nếu (UNLIKELY (ret < 0))
    trả_về -1;

  ret = fd;
  fd = -1;
  trả_về ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
static số_nguyên
wait_cgroup_unpopulated (const char *path, số_nguyên inotify_fd)
{
  cleanup_free char *events_file = NULL;
  libcrun_error_t tmp_err = NULL;
  struct timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  số_nguyên ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  nếu (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      trả_về 0;
    }

  cho (;;)
    {
      cleanup_free char *content = NULL;
      char buf[4096];
      struct pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, NULL, &tmp_err);
      nếu (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          trả_về 0;
        }

      nếu (strstr (content, "populated 0"))
        trả_về 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, NULL));
      nếu (ret <= 0)
        trả_về 0;

      trong_khi (read (inotify_fd, buf, sizeof (buf)) > 0)
        ;
    }
}

số_nguyên
cgroup_killall_path (const char *path, số_nguyên signal, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close số_nguyên inotify_fd = -1;
  số_nguyên ret;
  size_t i;
  cleanup_free pid_t *pids = NULL;

  nếu (path == NULL || *path == '\0')
    trả_về 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  nếu (signal == SIGKILL)
    {
      cleanup_free char *kill_file = NULL;

      ret = append_paths (&kill_file, err, CGROUP_ROOT, path, "cgroup.kill", NULL);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      nếu (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        trả_về 0;

      nếu (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  nếu (UNLIKELY (ret < 0))
    crun_error_release (err);
  không_thì
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, signal == SIGKILL, &pids, err);
  nếu (UNLIKELY (ret < 0))
    {
      nếu (crun_error_get_errno (err) != ENOENT)
        goto thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  cho (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], signal);
      nếu (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          goto thaw;
        }
    }

  ret = 0;

thaw:
  nếu (frozen)
    {
      libcrun_error_t thaw_err = NULL;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      nếu (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  trả_về ret;
}

static số_nguyên
read_available_controllers (const char *path, libcrun_error_t *err)
{
  cleanup_free char *controllers = NULL;
  cleanup_free char *buf = NULL;
  char *saveptr = NULL;
  const char *token;
  số_nguyên available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ROOT, path, "cgroup.controllers", NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = read_all_file (controllers, &buf, NULL, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  cho (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
    {
      nếu (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      không_thì nếu (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      không_thì nếu (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      không_thì nếu (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      không_thì nếu (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      không_thì nếu (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      không_thì nếu (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  trả_về available;
}

static số_nguyên
write_controller_file (const char *path, số_nguyên controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free char *subtree_control = NULL;
  cleanup_free char *controllers = NULL;
  size_t controllers_len = 0;
  số_nguyên ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ROOT, path, "cgroup.subtree_control", NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  nếu (UNLIKELY (ret < 0))
    {
      cleanup_free char *controllers_copy = xmalloc (controllers_len + 1);
      số_nguyên attempts_left;
      char *saveptr = NULL;
      const char *token;
      số_nguyên e;

      e = crun_error_get_errno (err);
      nếu (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        trả_về crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      nếu (e == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;
          số_nguyên exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          nếu (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              trả_về ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          nếu (exists == 0)
            trả_về ret;
        }

      crun_error_release (err);

      nếu (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          nếu (UNLIKELY (ret < 0))
            trả_về crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      cho (attempts_left = 1000; attempts_left >= 0; attempts_left--)
        {
          số_nguyên controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          cho (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (NULL, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              nếu (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  nếu (e == EBUSY)
                    repeat = true;

                  continue;
                }

              controllers_written++;
            }

          nếu (! repeat)
            break;

          /* If there was any controller written, just try once more without any delay.  */
          nếu (controllers_written > 0 && attempts_left > 2)
            {
              attempts_left = 1;
              continue;
            }

          nếu (attempts_left > 0)
            {
              struct timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, NULL);
            }
        }

      /* Refresh what controllers are available.  */
      trả_về read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  trả_về controllers_to_enable;
}

số_nguyên
enable_controllers (const char *path, libcrun_error_t *err)
{
  cleanup_free char *tmp_path = NULL;
  char *it;
  số_nguyên ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  nếu (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      nếu ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
    }

  cho (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free char *cgroup_path = NULL;
      char *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, tmp_path, NULL);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;

      ret = mkdir (cgroup_path, 0755);
      nếu (UNLIKELY (ret < 0 && errno != EEXIST))
        trả_về crun_make_error (err, errno, "create `%s`", cgroup_path);

      nếu (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
        }

      *it = '/';
      it = next_slash;
    }
  trả_về 0;
}

số_nguyên
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, const char *path, bool create_if_missing, libcrun_error_t *err)
{
  số_nguyên cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;

  nếu (path == NULL || *path == '\0')
    trả_về 0;

  trả_về enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
số_nguyên
libcrun_move_self_to_cgroups (const char *content, libcrun_error_t *err)
{
  cleanup_free char *buf = xstrdup (content);
  char *saveptr = NULL;
  char *controller;
  char *path;
  số_nguyên cgroup_mode;
  bool has_data;
  số_nguyên ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (UNLIKELY (cgroup_mode < 0))
    trả_về cgroup_mode;

  cho (has_data = read_proc_cgroup (buf, &saveptr, NULL, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &path))
    {
      const char *subsystem = NULL;

      nếu (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          nếu (controller[0] != '\0')
            continue;
        }
      không_thì
        {
          cleanup_free char *subsystem_path = NULL;

          /* Use the same names as enter_cgroup_v1 does.  */
          nếu (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          nếu (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          nếu (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ROOT, subsystem, NULL);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
          ret = crun_path_exists (subsystem_path, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
          nếu (ret == 0)
            continue;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
    }

  trả_về 0;
}

số_nguyên
libcrun_get_cgroup_dirfd (struct libcrun_cgroup_status *status, const char *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free char *path_to_cgroup = NULL;
  số_nguyên cgroup_mode;
  số_nguyên cgroupdirfd;
  số_nguyên ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (cgroup_mode < 0)
    trả_về cgroup_mode;

  nếu (cgroup_mode != CGROUP_MODE_UNIFIED)
    trả_về crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  nếu (status == NULL)
    trả_về crun_make_error (err, 0, "internal error");

  nếu (is_empty_string (status->path))
    trả_về crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ROOT, status->path, sub_cgroup, NULL);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  nếu (UNLIKELY (cgroupdirfd < 0))
    trả_về crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  trả_về cgroupdirfd;
}

số_nguyên
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, char *from, char *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = NULL;
  cleanup_close số_nguyên child_dfd = -1;
  số_nguyên cgroup_mode;
  size_t from_len;
  size_t i;
  số_nguyên ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  nếu (cgroup_mode < 0)
    trả_về cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  nếu (UNLIKELY (ret < 0))
    goto thaw;

  from_len = strlen (from);

  cho (i = 0; pids && pids[i]; i++)
    {
      cleanup_free char *pid_path = NULL;
      cleanup_free char *dest_cgroup = NULL;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      nếu (UNLIKELY (ret < 0))
        goto thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      nếu (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          goto thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      nếu (UNLIKELY (ret < 0))
        goto thaw;
    }

  ret = 0;

thaw:
  nếu (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = NULL;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      nếu (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      trả_về ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  trả_về destroy_cgroup_path (from, cgroup_mode, err);
}

số_nguyên
get_cgroup_dirfd_path (số_nguyên dirfd, char **path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  nếu (UNLIKELY (len < 0))
    trả_về len;

  nếu (has_prefix (cgroup_path, CGROUP_ROOT))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ROOT));
      trả_về 0;
    }
  trả_về crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
số_nguyên
parse_cpu_max (const char *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  const char *it = value;
  char *endptr = NULL;

  trong_khi (*it == ' ')
    it++;

  errno = 0;
  nếu (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (char *) it + 3;
    }
  không_thì
    {
      *quota = strtoll (it, &endptr, 10);
      nếu (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        trả_về crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
    }

  it = endptr;
  trong_khi (*it == ' ')
    it++;

  *has_period = false;
  nếu (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      nếu (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        trả_về crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
      *has_period = true;
    }

  trả_về 0;
}
