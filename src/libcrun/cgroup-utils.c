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

целое
libcrun_cgroups_create_symlinks (целое dirfd, libcrun_error_t *err)
{
  целое i;

  для (i = 0; cgroup_symlinks[i].name; i++)
    {
      целое ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      если (UNLIKELY (ret < 0))
        {
          если (errno == ENOENT || errno == EEXIST)
            continue;
          вернуть crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  вернуть 0;
}

целое
maybe_make_cgroup_threaded (const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_type = NULL;
  const char *const threaded = "threaded";
  cleanup_free char *content = NULL;
  cleanup_free char *buffer = NULL;
  const char *parent;
  size_t size;
  char *it;
  целое ret;

  если (path == NULL)
    вернуть 0;

  path = consume_slashes (path);

  если (path[0] == '\0')
    вернуть 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ROOT, path, "cgroup.type", NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (size > 0)
    {
      it = content + size - 1;
      пока (*it == '\n' && it > content)
        *it-- = '\0';
    }

  если (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    вернуть 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  если (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      если (ret < 0)
        вернуть ret;
    }
  вернуть write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

целое
move_process_to_cgroup (pid_t pid, const char *subsystem, const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_procs = NULL;
  char pid_str[16];
  целое ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ROOT,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = snprintf (pid_str, sizeof (pid_str), "%d", pid);
  если (UNLIKELY (ret >= (целое) sizeof (pid_str)))
    вернуть crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  если (UNLIKELY (ret < 0))
    {
      если (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = NULL;
          целое mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          если (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              вернуть ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;

          вернуть write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      вернуть ret;
    }
  вернуть ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

static целое
detect_cgroup_mode (libcrun_error_t *err)
{
  struct statfs stat;
  целое ret;

  ret = statfs (CGROUP_ROOT, &stat);
  если (ret < 0)
    вернуть crun_make_error (err, errno, "statfs `" CGROUP_ROOT "`");
  если (stat.f_type == CGROUP2_SUPER_MAGIC)
    вернуть CGROUP_MODE_UNIFIED;
  если (stat.f_type != TMPFS_MAGIC)
    вернуть crun_make_error (err, 0, "invalid file system type on `" CGROUP_ROOT "`");
  ret = statfs (CGROUP_ROOT "/unified", &stat);
  если (ret < 0 && errno != ENOENT)
    вернуть crun_make_error (err, errno, "statfs `" CGROUP_ROOT "/unified`");
  если (ret < 0)
    вернуть CGROUP_MODE_LEGACY;
  вернуть stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

целое
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  целое tmp;
  static целое cgroup_mode = 0;

  если (cgroup_mode)
    вернуть cgroup_mode;

  tmp = detect_cgroup_mode (err);
  если (UNLIKELY (tmp < 0))
    вернуть tmp;

  cgroup_mode = tmp;

  вернуть cgroup_mode;
}

static целое
get_cgroup_process_from_content (char *content, целое cgroup_mode, char **path, bool absolute, libcrun_error_t *err)
{
  char *cg_path = NULL;
  char *controller;
  char *saveptr;
  bool has_data;

  для (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &cg_path))
    {
      если (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          если (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            goto found;
        }
      иначе
        {
          если (strcmp (controller, "memory"))
            goto found;
        }
    }

  вернуть crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  если (absolute)
    вернуть append_paths (path, err, CGROUP_ROOT, cg_path, NULL);

  *path = xstrdup (cg_path);
  вернуть 0;
}

целое
libcrun_get_cgroup_process_at (целое dirfd, char **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  size_t content_size;
  целое cgroup_mode;
  целое ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

целое
libcrun_get_cgroup_process (pid_t pid, char **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  char proc_cgroup_file[64];
  size_t content_size;
  целое cgroup_mode;
  целое ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  если (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  иначе
    {
      целое len = snprintf (proc_cgroup_file, sizeof (proc_cgroup_file), "/proc/%d/cgroup", pid);
      если (UNLIKELY (len >= (целое) sizeof (proc_cgroup_file)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

static целое
read_pids_file (целое dfd, const char *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close целое tasksfd = -1;
  cleanup_free char *buffer = NULL;
  char *saveptr = NULL;
  size_t n_new_pids;
  size_t len;
  char *it;
  целое ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  если (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      если (errno == ENOENT || errno == ENODEV)
        вернуть 0;
      вернуть crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (len > 0)
    {
      для (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      если (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, sizeof (pid_t) * *allocated);
        }

      для (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (NULL, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, NULL, 10);

          если (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  вернуть 0;
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
  целое dfd;
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
  целое saved_errno = errno;

  пока (walk->n_dirs > 0)
    cgroup_walk_pop (walk);
  free (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
static целое
cgroup_walk_enter (struct cgroup_walk_s *walk, целое dfd)
{
  cleanup_close целое dfd_clone = -1;
  cleanup_dir DIR *dir = NULL;
  struct dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  если (UNLIKELY (dfd_clone < 0))
    вернуть -1;

  dir = fdopendir (dfd_clone);
  если (UNLIKELY (dir == NULL))
    вернуть -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  для (de = readdir (dir); de; de = readdir (dir))
    {
      struct cgroup_walk_dir_s *d;

      если (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      если (de->d_type != DT_DIR)
        continue;

      если (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, sizeof (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  вернуть 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
static целое
cgroup_walk_next (struct cgroup_walk_s *walk, struct cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : NULL;

  пока (*d && walk->depth >= (*d)->depth)
    {
      целое parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      если (UNLIKELY (parent < 0))
        вернуть -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  вернуть 0;
}

static целое
read_pids_cgroup_file (целое dfd, const char *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk struct cgroup_walk_s walk = { .dfd = -1 };
  целое dfd_clone;
  целое ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (! recurse)
    вернуть 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  если (UNLIKELY (dfd_clone < 0))
    вернуть crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "open cgroup sub-directory");

  для (;;)
    {
      struct cgroup_walk_dir_s *d;
      cleanup_close целое nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      если (UNLIKELY (ret < 0))
        вернуть crun_make_error (err, errno, "open parent cgroup directory");
      если (d == NULL)
        break;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      если (UNLIKELY (nfd < 0))
        {
          целое saved_errno = errno;

          если (saved_errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          вернуть crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_pop (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      если (UNLIKELY (ret < 0))
        {
          целое e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          если (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            вернуть ret;
          crun_error_release (err);
          continue;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      если (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        вернуть crun_make_error (err, errno, "open cgroup sub-directory");
    }

  вернуть 0;
}

static целое
read_pids_cgroup (целое dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  целое ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  если (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  вернуть ret;
}

static целое
rmdir_all_fd (целое dfd)
{
  cleanup_cgroup_walk struct cgroup_walk_s walk = { .dfd = -1 };
  целое ret;

  ret = cgroup_walk_enter (&walk, dfd);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  для (;;)
    {
      struct cgroup_walk_dir_s *d;
      целое child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      если (UNLIKELY (ret < 0))
        вернуть ret;
      если (d == NULL)
        break;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      если (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_pop (&walk);
          continue;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      если (UNLIKELY (child_dfd < 0))
        {
          если (errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          вернуть child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      если (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        вернуть ret;
    }

  вернуть 0;
}

static целое
rmdir_all (const char *path)
{
  целое ret;
  целое dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  если (UNLIKELY (dfd < 0))
    вернуть dfd;

  ret = rmdir_all_fd (dfd);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть rmdir (path);
}

static целое
read_pids_from_path (const char *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_close целое dirfd = -1;
  size_t n_pids, allocated;
  целое mode;
  целое ret;

  если (path == NULL || *path == '\0')
    вернуть 0;

  mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (mode < 0))
    вернуть mode;

  switch (mode)
    {
    case CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
      если (UNLIKELY (ret < 0))
        вернуть ret;
      break;

    case CGROUP_MODE_HYBRID:
    case CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT "/memory", path, NULL);
      если (UNLIKELY (ret < 0))
        вернуть ret;
      break;

    default:
      вернуть crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  если (dirfd < 0)
    вернуть crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  если (threads && mode == CGROUP_MODE_UNIFIED)
    вернуть read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  вернуть read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

целое
libcrun_cgroup_read_pids_from_path (const char *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  вернуть read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (char *content, char **saveptr, char **id, char **controller_list, char **path)
{
  char *it;

  it = strtok_r (content, "\n", saveptr);
  если (it == NULL)
    вернуть false;

  если (id)
    *id = it;

  it = strchr (it, ':');
  если (it == NULL)
    вернуть false;
  *it++ = '\0';

  если (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  если (it == NULL)
    вернуть false;
  *it++ = '\0';

  если (path)
    *path = it;

  вернуть true;
}

целое
destroy_cgroup_path (const char *path, целое mode, libcrun_error_t *err)
{
  bool repeat = true;
  целое retry_count = 0;
  const целое max_attempts = 500;
  целое ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  если (is_empty_string (path))
    вернуть 0;

  do
    {
      repeat = false;

      если (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free char *cgroup_path = NULL;

          ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
          если (UNLIKELY (ret < 0))
            вернуть ret;
          ret = rmdir (cgroup_path);
          если (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              если (ret < 0)
                {
                  если (retry_count >= max_attempts)
                    вернуть crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_count++;
                  repeat = true;
                }
            }
        }
      иначе
        {
          cleanup_free char *content = NULL;
          size_t content_size;
          char *controller;
          char *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          если (UNLIKELY (ret < 0))
            {
              если (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  вернуть 0;
                }
              вернуть ret;
            }

          для (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
               has_data;
               has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
            {
              cleanup_free char *cgroup_path = NULL;
              char *subsystem;
              если (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              если (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                continue;

              ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path, NULL);
              если (UNLIKELY (ret < 0))
                вернуть ret;

              ret = rmdir (cgroup_path);
              если (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  если (ret < 0)
                    {
                      если (retry_count >= max_attempts)
                        вернуть crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_count++;
                      repeat = true;
                    }
                }
            }
        }

      если (repeat)
        {
          struct timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, NULL);

          ret = cgroup_killall_path (path, SIGKILL, err);
          если (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } пока (repeat);

  вернуть 0;
}

целое
chown_cgroups (const char *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_free char *delegate = NULL;
  cleanup_close целое dfd = -1;
  size_t delegate_size;
  char *saveptr = NULL;
  char *name;
  целое ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  если (UNLIKELY (dfd < 0))
    вернуть crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  если (UNLIKELY (ret < 0))
    {
      если (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          вернуть 0;
        }
      вернуть ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  для (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (NULL, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      если (UNLIKELY (ret < 0))
        {
          если (errno == ENOENT)
            continue;

          вернуть crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  вернуть 0;
}

static целое
libcrun_cgroup_pause_unpause_with_mode (const char *cgroup_path, целое cgroup_mode, const bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free char *path = NULL;
  const char *state = "";
  целое ret;

  если (cgroup_path == NULL || cgroup_path[0] == '\0')
    вернуть crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  если (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  иначе
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  если (ret >= 0)
    вернуть 0;
  вернуть ret;
}

целое
libcrun_cgroup_pause_unpause_path (const char *cgroup_path, const bool pause, libcrun_error_t *err)
{
  целое cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  вернуть libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
целое
libcrun_cgroup_ensure_not_frozen (const char *cgroup_path, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  cleanup_free char *path = NULL;
  const char *frozen;
  целое cgroup_mode;
  целое ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  если (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
    }
  иначе
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
    }
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = read_all_file (path, &content, NULL, err);
  если (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      если (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          вернуть 0;
        }
      вернуть ret;
    }

  если (strstr (content, frozen))
    вернуть crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  вернуть 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
static целое
watch_cgroup_events (const char *path)
{
  cleanup_free char *events_file = NULL;
  cleanup_close целое fd = -1;
  libcrun_error_t tmp_err = NULL;
  целое ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  если (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      вернуть -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  если (UNLIKELY (fd < 0))
    вернуть -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  если (UNLIKELY (ret < 0))
    вернуть -1;

  ret = fd;
  fd = -1;
  вернуть ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
static целое
wait_cgroup_unpopulated (const char *path, целое inotify_fd)
{
  cleanup_free char *events_file = NULL;
  libcrun_error_t tmp_err = NULL;
  struct timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  целое ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  если (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      вернуть 0;
    }

  для (;;)
    {
      cleanup_free char *content = NULL;
      char buf[4096];
      struct pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, NULL, &tmp_err);
      если (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          вернуть 0;
        }

      если (strstr (content, "populated 0"))
        вернуть 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, NULL));
      если (ret <= 0)
        вернуть 0;

      пока (read (inotify_fd, buf, sizeof (buf)) > 0)
        ;
    }
}

целое
cgroup_killall_path (const char *path, целое signal, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close целое inotify_fd = -1;
  целое ret;
  size_t i;
  cleanup_free pid_t *pids = NULL;

  если (path == NULL || *path == '\0')
    вернуть 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  если (signal == SIGKILL)
    {
      cleanup_free char *kill_file = NULL;

      ret = append_paths (&kill_file, err, CGROUP_ROOT, path, "cgroup.kill", NULL);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      если (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        вернуть 0;

      если (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  если (UNLIKELY (ret < 0))
    crun_error_release (err);
  иначе
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, signal == SIGKILL, &pids, err);
  если (UNLIKELY (ret < 0))
    {
      если (crun_error_get_errno (err) != ENOENT)
        goto thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  для (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], signal);
      если (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          goto thaw;
        }
    }

  ret = 0;

thaw:
  если (frozen)
    {
      libcrun_error_t thaw_err = NULL;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      если (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  вернуть ret;
}

static целое
read_available_controllers (const char *path, libcrun_error_t *err)
{
  cleanup_free char *controllers = NULL;
  cleanup_free char *buf = NULL;
  char *saveptr = NULL;
  const char *token;
  целое available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ROOT, path, "cgroup.controllers", NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = read_all_file (controllers, &buf, NULL, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  для (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
    {
      если (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      иначе если (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      иначе если (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      иначе если (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      иначе если (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      иначе если (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      иначе если (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  вернуть available;
}

static целое
write_controller_file (const char *path, целое controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free char *subtree_control = NULL;
  cleanup_free char *controllers = NULL;
  size_t controllers_len = 0;
  целое ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ROOT, path, "cgroup.subtree_control", NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  если (UNLIKELY (ret < 0))
    {
      cleanup_free char *controllers_copy = xmalloc (controllers_len + 1);
      целое attempts_left;
      char *saveptr = NULL;
      const char *token;
      целое e;

      e = crun_error_get_errno (err);
      если (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        вернуть crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      если (e == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;
          целое exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          если (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              вернуть ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          если (exists == 0)
            вернуть ret;
        }

      crun_error_release (err);

      если (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          если (UNLIKELY (ret < 0))
            вернуть crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      для (attempts_left = 1000; attempts_left >= 0; attempts_left--)
        {
          целое controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          для (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (NULL, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              если (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  если (e == EBUSY)
                    repeat = true;

                  continue;
                }

              controllers_written++;
            }

          если (! repeat)
            break;

          /* If there was any controller written, just try once more without any delay.  */
          если (controllers_written > 0 && attempts_left > 2)
            {
              attempts_left = 1;
              continue;
            }

          если (attempts_left > 0)
            {
              struct timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, NULL);
            }
        }

      /* Refresh what controllers are available.  */
      вернуть read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  вернуть controllers_to_enable;
}

целое
enable_controllers (const char *path, libcrun_error_t *err)
{
  cleanup_free char *tmp_path = NULL;
  char *it;
  целое ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  если (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      если ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  для (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free char *cgroup_path = NULL;
      char *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, tmp_path, NULL);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = mkdir (cgroup_path, 0755);
      если (UNLIKELY (ret < 0 && errno != EEXIST))
        вернуть crun_make_error (err, errno, "create `%s`", cgroup_path);

      если (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
        }

      *it = '/';
      it = next_slash;
    }
  вернуть 0;
}

целое
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, const char *path, bool create_if_missing, libcrun_error_t *err)
{
  целое cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  если (path == NULL || *path == '\0')
    вернуть 0;

  вернуть enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
целое
libcrun_move_self_to_cgroups (const char *content, libcrun_error_t *err)
{
  cleanup_free char *buf = xstrdup (content);
  char *saveptr = NULL;
  char *controller;
  char *path;
  целое cgroup_mode;
  bool has_data;
  целое ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  для (has_data = read_proc_cgroup (buf, &saveptr, NULL, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &path))
    {
      const char *subsystem = NULL;

      если (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          если (controller[0] != '\0')
            continue;
        }
      иначе
        {
          cleanup_free char *subsystem_path = NULL;

          /* Use the same names as enter_cgroup_v1 does.  */
          если (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          если (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          если (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ROOT, subsystem, NULL);
          если (UNLIKELY (ret < 0))
            вернуть ret;
          ret = crun_path_exists (subsystem_path, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
          если (ret == 0)
            continue;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  вернуть 0;
}

целое
libcrun_get_cgroup_dirfd (struct libcrun_cgroup_status *status, const char *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free char *path_to_cgroup = NULL;
  целое cgroup_mode;
  целое cgroupdirfd;
  целое ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (cgroup_mode < 0)
    вернуть cgroup_mode;

  если (cgroup_mode != CGROUP_MODE_UNIFIED)
    вернуть crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  если (status == NULL)
    вернуть crun_make_error (err, 0, "internal error");

  если (is_empty_string (status->path))
    вернуть crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ROOT, status->path, sub_cgroup, NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  если (UNLIKELY (cgroupdirfd < 0))
    вернуть crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  вернуть cgroupdirfd;
}

целое
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, char *from, char *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = NULL;
  cleanup_close целое child_dfd = -1;
  целое cgroup_mode;
  size_t from_len;
  size_t i;
  целое ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (cgroup_mode < 0)
    вернуть cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  если (UNLIKELY (ret < 0))
    goto thaw;

  from_len = strlen (from);

  для (i = 0; pids && pids[i]; i++)
    {
      cleanup_free char *pid_path = NULL;
      cleanup_free char *dest_cgroup = NULL;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      если (UNLIKELY (ret < 0))
        goto thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      если (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          goto thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      если (UNLIKELY (ret < 0))
        goto thaw;
    }

  ret = 0;

thaw:
  если (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = NULL;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      если (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      вернуть ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть destroy_cgroup_path (from, cgroup_mode, err);
}

целое
get_cgroup_dirfd_path (целое dirfd, char **path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  если (UNLIKELY (len < 0))
    вернуть len;

  если (has_prefix (cgroup_path, CGROUP_ROOT))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ROOT));
      вернуть 0;
    }
  вернуть crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
целое
parse_cpu_max (const char *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  const char *it = value;
  char *endptr = NULL;

  пока (*it == ' ')
    it++;

  errno = 0;
  если (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (char *) it + 3;
    }
  иначе
    {
      *quota = strtoll (it, &endptr, 10);
      если (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        вернуть crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
    }

  it = endptr;
  пока (*it == ' ')
    it++;

  *has_period = false;
  если (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      если (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        вернуть crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
      *has_period = true;
    }

  вернуть 0;
}
