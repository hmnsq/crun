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

цяло
libcrun_cgroups_create_symlinks (цяло dirfd, libcrun_error_t *err)
{
  цяло i;

  за (i = 0; cgroup_symlinks[i].name; i++)
    {
      цяло ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      ако (UNLIKELY (ret < 0))
        {
          ако (errno == ENOENT || errno == EEXIST)
            continue;
          върни crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  върни 0;
}

цяло
maybe_make_cgroup_threaded (const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_type = NULL;
  const char *const threaded = "threaded";
  cleanup_free char *content = NULL;
  cleanup_free char *buffer = NULL;
  const char *parent;
  size_t size;
  char *it;
  цяло ret;

  ако (path == NULL)
    върни 0;

  path = consume_slashes (path);

  ако (path[0] == '\0')
    върни 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ROOT, path, "cgroup.type", NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ако (size > 0)
    {
      it = content + size - 1;
      докато (*it == '\n' && it > content)
        *it-- = '\0';
    }

  ако (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    върни 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  ако (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      ако (ret < 0)
        върни ret;
    }
  върни write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

цяло
move_process_to_cgroup (pid_t pid, const char *subsystem, const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path_procs = NULL;
  char pid_str[16];
  цяло ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ROOT,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = snprintf (pid_str, sizeof (pid_str), "%d", pid);
  ако (UNLIKELY (ret >= (цяло) sizeof (pid_str)))
    върни crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  ако (UNLIKELY (ret < 0))
    {
      ако (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = NULL;
          цяло mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          ако (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              върни ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          ако (UNLIKELY (ret < 0))
            върни ret;

          върни write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      върни ret;
    }
  върни ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

static цяло
detect_cgroup_mode (libcrun_error_t *err)
{
  struct statfs stat;
  цяло ret;

  ret = statfs (CGROUP_ROOT, &stat);
  ако (ret < 0)
    върни crun_make_error (err, errno, "statfs `" CGROUP_ROOT "`");
  ако (stat.f_type == CGROUP2_SUPER_MAGIC)
    върни CGROUP_MODE_UNIFIED;
  ако (stat.f_type != TMPFS_MAGIC)
    върни crun_make_error (err, 0, "invalid file system type on `" CGROUP_ROOT "`");
  ret = statfs (CGROUP_ROOT "/unified", &stat);
  ако (ret < 0 && errno != ENOENT)
    върни crun_make_error (err, errno, "statfs `" CGROUP_ROOT "/unified`");
  ако (ret < 0)
    върни CGROUP_MODE_LEGACY;
  върни stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

цяло
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  цяло tmp;
  static цяло cgroup_mode = 0;

  ако (cgroup_mode)
    върни cgroup_mode;

  tmp = detect_cgroup_mode (err);
  ако (UNLIKELY (tmp < 0))
    върни tmp;

  cgroup_mode = tmp;

  върни cgroup_mode;
}

static цяло
get_cgroup_process_from_content (char *content, цяло cgroup_mode, char **path, bool absolute, libcrun_error_t *err)
{
  char *cg_path = NULL;
  char *controller;
  char *saveptr;
  bool has_data;

  за (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &cg_path))
    {
      ако (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          ако (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            goto found;
        }
      иначе
        {
          ако (strcmp (controller, "memory"))
            goto found;
        }
    }

  върни crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  ако (absolute)
    върни append_paths (path, err, CGROUP_ROOT, cg_path, NULL);

  *path = xstrdup (cg_path);
  върни 0;
}

цяло
libcrun_get_cgroup_process_at (цяло dirfd, char **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  size_t content_size;
  цяло cgroup_mode;
  цяло ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  върни get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

цяло
libcrun_get_cgroup_process (pid_t pid, char **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  char proc_cgroup_file[64];
  size_t content_size;
  цяло cgroup_mode;
  цяло ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  ако (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  иначе
    {
      цяло len = snprintf (proc_cgroup_file, sizeof (proc_cgroup_file), "/proc/%d/cgroup", pid);
      ако (UNLIKELY (len >= (цяло) sizeof (proc_cgroup_file)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  върни get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

static цяло
read_pids_file (цяло dfd, const char *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close цяло tasksfd = -1;
  cleanup_free char *buffer = NULL;
  char *saveptr = NULL;
  size_t n_new_pids;
  size_t len;
  char *it;
  цяло ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  ако (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      ако (errno == ENOENT || errno == ENODEV)
        върни 0;
      върни crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ако (len > 0)
    {
      за (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      ако (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, sizeof (pid_t) * *allocated);
        }

      за (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (NULL, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, NULL, 10);

          ако (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  върни 0;
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
  цяло dfd;
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
  цяло saved_errno = errno;

  докато (walk->n_dirs > 0)
    cgroup_walk_pop (walk);
  free (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
static цяло
cgroup_walk_enter (struct cgroup_walk_s *walk, цяло dfd)
{
  cleanup_close цяло dfd_clone = -1;
  cleanup_dir DIR *dir = NULL;
  struct dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  ако (UNLIKELY (dfd_clone < 0))
    върни -1;

  dir = fdopendir (dfd_clone);
  ако (UNLIKELY (dir == NULL))
    върни -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  за (de = readdir (dir); de; de = readdir (dir))
    {
      struct cgroup_walk_dir_s *d;

      ако (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      ако (de->d_type != DT_DIR)
        continue;

      ако (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, sizeof (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  върни 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
static цяло
cgroup_walk_next (struct cgroup_walk_s *walk, struct cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : NULL;

  докато (*d && walk->depth >= (*d)->depth)
    {
      цяло parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      ако (UNLIKELY (parent < 0))
        върни -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  върни 0;
}

static цяло
read_pids_cgroup_file (цяло dfd, const char *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk struct cgroup_walk_s walk = { .dfd = -1 };
  цяло dfd_clone;
  цяло ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ако (! recurse)
    върни 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ако (UNLIKELY (dfd_clone < 0))
    върни crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  ако (UNLIKELY (ret < 0))
    върни crun_make_error (err, errno, "open cgroup sub-directory");

  за (;;)
    {
      struct cgroup_walk_dir_s *d;
      cleanup_close цяло nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      ако (UNLIKELY (ret < 0))
        върни crun_make_error (err, errno, "open parent cgroup directory");
      ако (d == NULL)
        break;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ако (UNLIKELY (nfd < 0))
        {
          цяло saved_errno = errno;

          ако (saved_errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          върни crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_pop (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      ако (UNLIKELY (ret < 0))
        {
          цяло e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          ако (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            върни ret;
          crun_error_release (err);
          continue;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      ако (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        върни crun_make_error (err, errno, "open cgroup sub-directory");
    }

  върни 0;
}

static цяло
read_pids_cgroup (цяло dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  цяло ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  ако (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  върни ret;
}

static цяло
rmdir_all_fd (цяло dfd)
{
  cleanup_cgroup_walk struct cgroup_walk_s walk = { .dfd = -1 };
  цяло ret;

  ret = cgroup_walk_enter (&walk, dfd);
  ако (UNLIKELY (ret < 0))
    върни ret;

  за (;;)
    {
      struct cgroup_walk_dir_s *d;
      цяло child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      ако (UNLIKELY (ret < 0))
        върни ret;
      ако (d == NULL)
        break;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      ако (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_pop (&walk);
          continue;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      ако (UNLIKELY (child_dfd < 0))
        {
          ако (errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              continue;
            }
          върни child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      ако (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        върни ret;
    }

  върни 0;
}

static цяло
rmdir_all (const char *path)
{
  цяло ret;
  цяло dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  ако (UNLIKELY (dfd < 0))
    върни dfd;

  ret = rmdir_all_fd (dfd);
  ако (UNLIKELY (ret < 0))
    върни ret;

  върни rmdir (path);
}

static цяло
read_pids_from_path (const char *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_close цяло dirfd = -1;
  size_t n_pids, allocated;
  цяло mode;
  цяло ret;

  ако (path == NULL || *path == '\0')
    върни 0;

  mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (mode < 0))
    върни mode;

  switch (mode)
    {
    case CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
      ако (UNLIKELY (ret < 0))
        върни ret;
      break;

    case CGROUP_MODE_HYBRID:
    case CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT "/memory", path, NULL);
      ако (UNLIKELY (ret < 0))
        върни ret;
      break;

    default:
      върни crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ако (dirfd < 0)
    върни crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  ако (threads && mode == CGROUP_MODE_UNIFIED)
    върни read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  върни read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

цяло
libcrun_cgroup_read_pids_from_path (const char *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  върни read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (char *content, char **saveptr, char **id, char **controller_list, char **path)
{
  char *it;

  it = strtok_r (content, "\n", saveptr);
  ако (it == NULL)
    върни false;

  ако (id)
    *id = it;

  it = strchr (it, ':');
  ако (it == NULL)
    върни false;
  *it++ = '\0';

  ако (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  ако (it == NULL)
    върни false;
  *it++ = '\0';

  ако (path)
    *path = it;

  върни true;
}

цяло
destroy_cgroup_path (const char *path, цяло mode, libcrun_error_t *err)
{
  bool repeat = true;
  цяло retry_count = 0;
  const цяло max_attempts = 500;
  цяло ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  ако (is_empty_string (path))
    върни 0;

  do
    {
      repeat = false;

      ако (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free char *cgroup_path = NULL;

          ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
          ако (UNLIKELY (ret < 0))
            върни ret;
          ret = rmdir (cgroup_path);
          ако (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              ако (ret < 0)
                {
                  ако (retry_count >= max_attempts)
                    върни crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

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
          ако (UNLIKELY (ret < 0))
            {
              ако (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  върни 0;
                }
              върни ret;
            }

          за (has_data = read_proc_cgroup (content, &saveptr, NULL, &controller, NULL);
               has_data;
               has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, NULL))
            {
              cleanup_free char *cgroup_path = NULL;
              char *subsystem;
              ако (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              ако (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                continue;

              ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path, NULL);
              ако (UNLIKELY (ret < 0))
                върни ret;

              ret = rmdir (cgroup_path);
              ако (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  ако (ret < 0)
                    {
                      ако (retry_count >= max_attempts)
                        върни crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_count++;
                      repeat = true;
                    }
                }
            }
        }

      ако (repeat)
        {
          struct timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, NULL);

          ret = cgroup_killall_path (path, SIGKILL, err);
          ако (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } докато (repeat);

  върни 0;
}

цяло
chown_cgroups (const char *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_free char *delegate = NULL;
  cleanup_close цяло dfd = -1;
  size_t delegate_size;
  char *saveptr = NULL;
  char *name;
  цяло ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  ако (UNLIKELY (dfd < 0))
    върни crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  ако (UNLIKELY (ret < 0))
    {
      ако (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          върни 0;
        }
      върни ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  ако (UNLIKELY (ret < 0))
    върни crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  за (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (NULL, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ако (UNLIKELY (ret < 0))
        {
          ако (errno == ENOENT)
            continue;

          върни crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  върни 0;
}

static цяло
libcrun_cgroup_pause_unpause_with_mode (const char *cgroup_path, цяло cgroup_mode, const bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free char *path = NULL;
  const char *state = "";
  цяло ret;

  ако (cgroup_path == NULL || cgroup_path[0] == '\0')
    върни crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  ако (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  иначе
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  ако (ret >= 0)
    върни 0;
  върни ret;
}

цяло
libcrun_cgroup_pause_unpause_path (const char *cgroup_path, const bool pause, libcrun_error_t *err)
{
  цяло cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  върни libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
цяло
libcrun_cgroup_ensure_not_frozen (const char *cgroup_path, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  cleanup_free char *path = NULL;
  const char *frozen;
  цяло cgroup_mode;
  цяло ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  ако (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", NULL);
    }
  иначе
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", NULL);
    }
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = read_all_file (path, &content, NULL, err);
  ако (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      ако (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          върни 0;
        }
      върни ret;
    }

  ако (strstr (content, frozen))
    върни crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  върни 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
static цяло
watch_cgroup_events (const char *path)
{
  cleanup_free char *events_file = NULL;
  cleanup_close цяло fd = -1;
  libcrun_error_t tmp_err = NULL;
  цяло ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  ако (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      върни -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  ако (UNLIKELY (fd < 0))
    върни -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  ако (UNLIKELY (ret < 0))
    върни -1;

  ret = fd;
  fd = -1;
  върни ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
static цяло
wait_cgroup_unpopulated (const char *path, цяло inotify_fd)
{
  cleanup_free char *events_file = NULL;
  libcrun_error_t tmp_err = NULL;
  struct timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  цяло ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", NULL);
  ако (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      върни 0;
    }

  за (;;)
    {
      cleanup_free char *content = NULL;
      char buf[4096];
      struct pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, NULL, &tmp_err);
      ако (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          върни 0;
        }

      ако (strstr (content, "populated 0"))
        върни 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, NULL));
      ако (ret <= 0)
        върни 0;

      докато (read (inotify_fd, buf, sizeof (buf)) > 0)
        ;
    }
}

цяло
cgroup_killall_path (const char *path, цяло signal, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close цяло inotify_fd = -1;
  цяло ret;
  size_t i;
  cleanup_free pid_t *pids = NULL;

  ако (path == NULL || *path == '\0')
    върни 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  ако (signal == SIGKILL)
    {
      cleanup_free char *kill_file = NULL;

      ret = append_paths (&kill_file, err, CGROUP_ROOT, path, "cgroup.kill", NULL);
      ако (UNLIKELY (ret < 0))
        върни ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      ако (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        върни 0;

      ако (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  ако (UNLIKELY (ret < 0))
    crun_error_release (err);
  иначе
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, signal == SIGKILL, &pids, err);
  ако (UNLIKELY (ret < 0))
    {
      ако (crun_error_get_errno (err) != ENOENT)
        goto thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  за (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], signal);
      ако (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          goto thaw;
        }
    }

  ret = 0;

thaw:
  ако (frozen)
    {
      libcrun_error_t thaw_err = NULL;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      ако (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  върни ret;
}

static цяло
read_available_controllers (const char *path, libcrun_error_t *err)
{
  cleanup_free char *controllers = NULL;
  cleanup_free char *buf = NULL;
  char *saveptr = NULL;
  const char *token;
  цяло available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ROOT, path, "cgroup.controllers", NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = read_all_file (controllers, &buf, NULL, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  за (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
    {
      ако (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      иначе ако (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      иначе ако (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      иначе ако (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      иначе ако (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      иначе ако (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      иначе ако (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  върни available;
}

static цяло
write_controller_file (const char *path, цяло controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free char *subtree_control = NULL;
  cleanup_free char *controllers = NULL;
  size_t controllers_len = 0;
  цяло ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ROOT, path, "cgroup.subtree_control", NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  ако (UNLIKELY (ret < 0))
    {
      cleanup_free char *controllers_copy = xmalloc (controllers_len + 1);
      цяло attempts_left;
      char *saveptr = NULL;
      const char *token;
      цяло e;

      e = crun_error_get_errno (err);
      ако (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        върни crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      ако (e == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;
          цяло exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          ако (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              върни ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          ако (exists == 0)
            върни ret;
        }

      crun_error_release (err);

      ако (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          ако (UNLIKELY (ret < 0))
            върни crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      за (attempts_left = 1000; attempts_left >= 0; attempts_left--)
        {
          цяло controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          за (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (NULL, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              ако (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  ако (e == EBUSY)
                    repeat = true;

                  continue;
                }

              controllers_written++;
            }

          ако (! repeat)
            break;

          /* If there was any controller written, just try once more without any delay.  */
          ако (controllers_written > 0 && attempts_left > 2)
            {
              attempts_left = 1;
              continue;
            }

          ако (attempts_left > 0)
            {
              struct timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, NULL);
            }
        }

      /* Refresh what controllers are available.  */
      върни read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  върни controllers_to_enable;
}

цяло
enable_controllers (const char *path, libcrun_error_t *err)
{
  cleanup_free char *tmp_path = NULL;
  char *it;
  цяло ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  ако (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      ако ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  за (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free char *cgroup_path = NULL;
      char *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, tmp_path, NULL);
      ако (UNLIKELY (ret < 0))
        върни ret;

      ret = mkdir (cgroup_path, 0755);
      ако (UNLIKELY (ret < 0 && errno != EEXIST))
        върни crun_make_error (err, errno, "create `%s`", cgroup_path);

      ако (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          ако (UNLIKELY (ret < 0))
            върни ret;
        }

      *it = '/';
      it = next_slash;
    }
  върни 0;
}

цяло
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, const char *path, bool create_if_missing, libcrun_error_t *err)
{
  цяло cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  ако (path == NULL || *path == '\0')
    върни 0;

  върни enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
цяло
libcrun_move_self_to_cgroups (const char *content, libcrun_error_t *err)
{
  cleanup_free char *buf = xstrdup (content);
  char *saveptr = NULL;
  char *controller;
  char *path;
  цяло cgroup_mode;
  bool has_data;
  цяло ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  за (has_data = read_proc_cgroup (buf, &saveptr, NULL, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (NULL, &saveptr, NULL, &controller, &path))
    {
      const char *subsystem = NULL;

      ако (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          ако (controller[0] != '\0')
            continue;
        }
      иначе
        {
          cleanup_free char *subsystem_path = NULL;

          /* Use the same names as enter_cgroup_v1 does.  */
          ако (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          ако (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          ако (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ROOT, subsystem, NULL);
          ако (UNLIKELY (ret < 0))
            върни ret;
          ret = crun_path_exists (subsystem_path, err);
          ако (UNLIKELY (ret < 0))
            върни ret;
          ако (ret == 0)
            continue;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  върни 0;
}

цяло
libcrun_get_cgroup_dirfd (struct libcrun_cgroup_status *status, const char *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free char *path_to_cgroup = NULL;
  цяло cgroup_mode;
  цяло cgroupdirfd;
  цяло ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (cgroup_mode < 0)
    върни cgroup_mode;

  ако (cgroup_mode != CGROUP_MODE_UNIFIED)
    върни crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  ако (status == NULL)
    върни crun_make_error (err, 0, "internal error");

  ако (is_empty_string (status->path))
    върни crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ROOT, status->path, sub_cgroup, NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  ако (UNLIKELY (cgroupdirfd < 0))
    върни crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  върни cgroupdirfd;
}

цяло
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, char *from, char *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = NULL;
  cleanup_close цяло child_dfd = -1;
  цяло cgroup_mode;
  size_t from_len;
  size_t i;
  цяло ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (cgroup_mode < 0)
    върни cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  ако (UNLIKELY (ret < 0))
    goto thaw;

  from_len = strlen (from);

  за (i = 0; pids && pids[i]; i++)
    {
      cleanup_free char *pid_path = NULL;
      cleanup_free char *dest_cgroup = NULL;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      ако (UNLIKELY (ret < 0))
        goto thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      ако (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          goto thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      ако (UNLIKELY (ret < 0))
        goto thaw;
    }

  ret = 0;

thaw:
  ако (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = NULL;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      ако (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      върни ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  върни destroy_cgroup_path (from, cgroup_mode, err);
}

цяло
get_cgroup_dirfd_path (цяло dirfd, char **path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  ако (UNLIKELY (len < 0))
    върни len;

  ако (has_prefix (cgroup_path, CGROUP_ROOT))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ROOT));
      върни 0;
    }
  върни crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
цяло
parse_cpu_max (const char *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  const char *it = value;
  char *endptr = NULL;

  докато (*it == ' ')
    it++;

  errno = 0;
  ако (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (char *) it + 3;
    }
  иначе
    {
      *quota = strtoll (it, &endptr, 10);
      ако (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        върни crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
    }

  it = endptr;
  докато (*it == ' ')
    it++;

  *has_period = false;
  ако (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      ако (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        върни crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
      *has_period = true;
    }

  върни 0;
}
