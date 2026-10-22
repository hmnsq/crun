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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "cgroup.h"
#અન્તર્ભાવ "cgroup-internal.h"
#અન્તર્ભાવ "cgroup-systemd.h"
#અન્તર્ભાવ "cgroup-utils.h"
#અન્તર્ભાવ "cgroup-setup.h"
#અન્તર્ભાવ "ebpf.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "status.h"
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <sys/vfs.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <poll.h>
#અન્તર્ભાવ <sys/inotify.h>

#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <libgen.h>

સંરચના symlink_s
{
  અચલ અક્ષર *name;
  અચલ અક્ષર *target;
};

સ્થિર સંરચના symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { ખાલી, ખાલી } };

પૂર્ણાંક
libcrun_cgroups_create_symlinks (પૂર્ણાંક dirfd, libcrun_error_t *err)
{
  પૂર્ણાંક i;

  માટે (i = 0; cgroup_symlinks[i].name; i++)
    {
      પૂર્ણાંક ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      જો (UNLIKELY (ret < 0))
        {
          જો (errno == ENOENT || errno == EEXIST)
            ચાલુ;
          પાછા crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  પાછા 0;
}

પૂર્ણાંક
maybe_make_cgroup_threaded (અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cgroup_path_type = ખાલી;
  અચલ અક્ષર *અચલ threaded = "threaded";
  cleanup_free અક્ષર *content = ખાલી;
  cleanup_free અક્ષર *buffer = ખાલી;
  અચલ અક્ષર *parent;
  size_t size;
  અક્ષર *it;
  પૂર્ણાંક ret;

  જો (path == ખાલી)
    પાછા 0;

  path = consume_slashes (path);

  જો (path[0] == '\0')
    પાછા 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_મૂળ, path, "cgroup.type", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (size > 0)
    {
      it = content + size - 1;
      જ્યારે (*it == '\n' && it > content)
        *it-- = '\0';
    }

  જો (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    પાછા 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  જો (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      જો (ret < 0)
        પાછા ret;
    }
  પાછા write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

પૂર્ણાંક
move_process_to_cgroup (pid_t pid, અચલ અક્ષર *subsystem, અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cgroup_path_procs = ખાલી;
  અક્ષર pid_str[16];
  પૂર્ણાંક ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_મૂળ,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = snprintf (pid_str, કદ (pid_str), "%d", pid);
  જો (UNLIKELY (ret >= (પૂર્ણાંક) કદ (pid_str)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  જો (UNLIKELY (ret < 0))
    {
      જો (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = ખાલી;
          પૂર્ણાંક mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          જો (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              પાછા ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          પાછા write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      પાછા ret;
    }
  પાછા ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

સ્થિર પૂર્ણાંક
detect_cgroup_mode (libcrun_error_t *err)
{
  સંરચના statfs stat;
  પૂર્ણાંક ret;

  ret = statfs (CGROUP_મૂળ, &stat);
  જો (ret < 0)
    પાછા crun_make_error (err, errno, "statfs `" CGROUP_મૂળ "`");
  જો (stat.f_type == CGROUP2_SUPER_MAGIC)
    પાછા CGROUP_MODE_UNIFIED;
  જો (stat.f_type != TMPFS_MAGIC)
    પાછા crun_make_error (err, 0, "invalid file system type on `" CGROUP_મૂળ "`");
  ret = statfs (CGROUP_મૂળ "/unified", &stat);
  જો (ret < 0 && errno != ENOENT)
    પાછા crun_make_error (err, errno, "statfs `" CGROUP_મૂળ "/unified`");
  જો (ret < 0)
    પાછા CGROUP_MODE_LEGACY;
  પાછા stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

પૂર્ણાંક
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  પૂર્ણાંક tmp;
  સ્થિર પૂર્ણાંક cgroup_mode = 0;

  જો (cgroup_mode)
    પાછા cgroup_mode;

  tmp = detect_cgroup_mode (err);
  જો (UNLIKELY (tmp < 0))
    પાછા tmp;

  cgroup_mode = tmp;

  પાછા cgroup_mode;
}

સ્થિર પૂર્ણાંક
get_cgroup_process_from_content (અક્ષર *content, પૂર્ણાંક cgroup_mode, અક્ષર **path, bool absolute, libcrun_error_t *err)
{
  અક્ષર *cg_path = ખાલી;
  અક્ષર *controller;
  અક્ષર *saveptr;
  bool has_data;

  માટે (has_data = read_proc_cgroup (content, &saveptr, ખાલી, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (ખાલી, &saveptr, ખાલી, &controller, &cg_path))
    {
      જો (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          જો (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            જાઓ found;
        }
      નહીંતર
        {
          જો (strcmp (controller, "memory"))
            જાઓ found;
        }
    }

  પાછા crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  જો (absolute)
    પાછા append_paths (path, err, CGROUP_મૂળ, cg_path, ખાલી);

  *path = xstrdup (cg_path);
  પાછા 0;
}

પૂર્ણાંક
libcrun_get_cgroup_process_at (પૂર્ણાંક dirfd, અક્ષર **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free અક્ષર *content = ખાલી;
  size_t content_size;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

પૂર્ણાંક
libcrun_get_cgroup_process (pid_t pid, અક્ષર **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free અક્ષર *content = ખાલી;
  અક્ષર proc_cgroup_file[64];
  size_t content_size;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  જો (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  નહીંતર
    {
      પૂર્ણાંક len = snprintf (proc_cgroup_file, કદ (proc_cgroup_file), "/proc/%d/cgroup", pid);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (proc_cgroup_file)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

સ્થિર પૂર્ણાંક
read_pids_file (પૂર્ણાંક dfd, અચલ અક્ષર *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક tasksfd = -1;
  cleanup_free અક્ષર *buffer = ખાલી;
  અક્ષર *saveptr = ખાલી;
  size_t n_new_pids;
  size_t len;
  અક્ષર *it;
  પૂર્ણાંક ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  જો (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      જો (errno == ENOENT || errno == ENODEV)
        પાછા 0;
      પાછા crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (len > 0)
    {
      માટે (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      જો (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, કદ (pid_t) * *allocated);
        }

      માટે (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (ખાલી, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, ખાલી, 10);

          જો (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  પાછા 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
સંરચના cgroup_walk_dir_s
{
  અક્ષર *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
સંરચના cgroup_walk_s
{
  પૂર્ણાંક dfd;
  size_t depth;
  સંરચના cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

સ્થિર શૂન્ય
cgroup_walk_કાઢો (સંરચના cgroup_walk_s *walk)
{
  મુક્ત (walk->dirs[--walk->n_dirs].name);
}

સ્થિર શૂન્ય
cleanup_cgroup_walkp (સંરચના cgroup_walk_s *walk)
{
  પૂર્ણાંક saved_errno = errno;

  જ્યારે (walk->n_dirs > 0)
    cgroup_walk_કાઢો (walk);
  મુક્ત (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
સ્થિર પૂર્ણાંક
cgroup_walk_enter (સંરચના cgroup_walk_s *walk, પૂર્ણાંક dfd)
{
  cleanup_close પૂર્ણાંક dfd_clone = -1;
  cleanup_dir DIR *dir = ખાલી;
  સંરચના dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  જો (UNLIKELY (dfd_clone < 0))
    પાછા -1;

  dir = fdopendir (dfd_clone);
  જો (UNLIKELY (dir == ખાલી))
    પાછા -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  માટે (de = readdir (dir); de; de = readdir (dir))
    {
      સંરચના cgroup_walk_dir_s *d;

      જો (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ચાલુ;

      જો (de->d_type != DT_DIR)
        ચાલુ;

      જો (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, કદ (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  પાછા 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
સ્થિર પૂર્ણાંક
cgroup_walk_next (સંરચના cgroup_walk_s *walk, સંરચના cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : ખાલી;

  જ્યારે (*d && walk->depth >= (*d)->depth)
    {
      પૂર્ણાંક parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      જો (UNLIKELY (parent < 0))
        પાછા -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
read_pids_cgroup_file (પૂર્ણાંક dfd, અચલ અક્ષર *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk સંરચના cgroup_walk_s walk = { .dfd = -1 };
  પૂર્ણાંક dfd_clone;
  પૂર્ણાંક ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (! recurse)
    પાછા 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (dfd_clone < 0))
    પાછા crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "open cgroup sub-directory");

  માટે (;;)
    {
      સંરચના cgroup_walk_dir_s *d;
      cleanup_close પૂર્ણાંક nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "open parent cgroup directory");
      જો (d == ખાલી)
        તોડો;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      જો (UNLIKELY (nfd < 0))
        {
          પૂર્ણાંક saved_errno = errno;

          જો (saved_errno == ENOENT)
            {
              cgroup_walk_કાઢો (&walk);
              ચાલુ;
            }
          પાછા crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_કાઢો (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      જો (UNLIKELY (ret < 0))
        {
          પૂર્ણાંક e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          જો (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            પાછા ret;
          crun_error_release (err);
          ચાલુ;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      જો (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        પાછા crun_make_error (err, errno, "open cgroup sub-directory");
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
read_pids_cgroup (પૂર્ણાંક dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  જો (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  પાછા ret;
}

સ્થિર પૂર્ણાંક
rmdir_all_fd (પૂર્ણાંક dfd)
{
  cleanup_cgroup_walk સંરચના cgroup_walk_s walk = { .dfd = -1 };
  પૂર્ણાંક ret;

  ret = cgroup_walk_enter (&walk, dfd);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  માટે (;;)
    {
      સંરચના cgroup_walk_dir_s *d;
      પૂર્ણાંક child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      જો (d == ખાલી)
        તોડો;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      જો (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_કાઢો (&walk);
          ચાલુ;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      જો (UNLIKELY (child_dfd < 0))
        {
          જો (errno == ENOENT)
            {
              cgroup_walk_કાઢો (&walk);
              ચાલુ;
            }
          પાછા child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      જો (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        પાછા ret;
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
rmdir_all (અચલ અક્ષર *path)
{
  પૂર્ણાંક ret;
  પૂર્ણાંક dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  જો (UNLIKELY (dfd < 0))
    પાછા dfd;

  ret = rmdir_all_fd (dfd);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા rmdir (path);
}

સ્થિર પૂર્ણાંક
read_pids_from_path (અચલ અક્ષર *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cgroup_path = ખાલી;
  cleanup_close પૂર્ણાંક dirfd = -1;
  size_t n_pids, allocated;
  પૂર્ણાંક mode;
  પૂર્ણાંક ret;

  જો (path == ખાલી || *path == '\0')
    પાછા 0;

  mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (mode < 0))
    પાછા mode;

  પસંદગી (mode)
    {
    સ્થિતિ CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, path, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      તોડો;

    સ્થિતિ CGROUP_MODE_HYBRID:
    સ્થિતિ CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_મૂળ "/memory", path, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      તોડો;

    મૂળભૂત:
      પાછા crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (dirfd < 0)
    પાછા crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  જો (threads && mode == CGROUP_MODE_UNIFIED)
    પાછા read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  પાછા read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

પૂર્ણાંક
libcrun_cgroup_read_pids_from_path (અચલ અક્ષર *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  પાછા read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (અક્ષર *content, અક્ષર **saveptr, અક્ષર **id, અક્ષર **controller_list, અક્ષર **path)
{
  અક્ષર *it;

  it = strtok_r (content, "\n", saveptr);
  જો (it == ખાલી)
    પાછા false;

  જો (id)
    *id = it;

  it = strchr (it, ':');
  જો (it == ખાલી)
    પાછા false;
  *it++ = '\0';

  જો (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  જો (it == ખાલી)
    પાછા false;
  *it++ = '\0';

  જો (path)
    *path = it;

  પાછા true;
}

પૂર્ણાંક
destroy_cgroup_path (અચલ અક્ષર *path, પૂર્ણાંક mode, libcrun_error_t *err)
{
  bool repeat = true;
  પૂર્ણાંક retry_ગણતરી = 0;
  અચલ પૂર્ણાંક max_attempts = 500;
  પૂર્ણાંક ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  જો (is_empty_string (path))
    પાછા 0;

  કરો
    {
      repeat = false;

      જો (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free અક્ષર *cgroup_path = ખાલી;

          ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, path, ખાલી);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          ret = rmdir (cgroup_path);
          જો (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              જો (ret < 0)
                {
                  જો (retry_ગણતરી >= max_attempts)
                    પાછા crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_ગણતરી++;
                  repeat = true;
                }
            }
        }
      નહીંતર
        {
          cleanup_free અક્ષર *content = ખાલી;
          size_t content_size;
          અક્ષર *controller;
          અક્ષર *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          જો (UNLIKELY (ret < 0))
            {
              જો (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  પાછા 0;
                }
              પાછા ret;
            }

          માટે (has_data = read_proc_cgroup (content, &saveptr, ખાલી, &controller, ખાલી);
               has_data;
               has_data = read_proc_cgroup (ખાલી, &saveptr, ખાલી, &controller, ખાલી))
            {
              cleanup_free અક્ષર *cgroup_path = ખાલી;
              અક્ષર *subsystem;
              જો (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              જો (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                ચાલુ;

              ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, subsystem, path, ખાલી);
              જો (UNLIKELY (ret < 0))
                પાછા ret;

              ret = rmdir (cgroup_path);
              જો (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  જો (ret < 0)
                    {
                      જો (retry_ગણતરી >= max_attempts)
                        પાછા crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_ગણતરી++;
                      repeat = true;
                    }
                }
            }
        }

      જો (repeat)
        {
          સંરચના timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, ખાલી);

          ret = cgroup_killall_path (path, SIGKILL, err);
          જો (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } જ્યારે (repeat);

  પાછા 0;
}

પૂર્ણાંક
chown_cgroups (અચલ અક્ષર *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cgroup_path = ખાલી;
  cleanup_free અક્ષર *delegate = ખાલી;
  cleanup_close પૂર્ણાંક dfd = -1;
  size_t delegate_size;
  અક્ષર *saveptr = ખાલી;
  અક્ષર *name;
  પૂર્ણાંક ret;

  ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, path, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  જો (UNLIKELY (dfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  જો (UNLIKELY (ret < 0))
    {
      જો (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          પાછા 0;
        }
      પાછા ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  માટે (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (ખાલી, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      જો (UNLIKELY (ret < 0))
        {
          જો (errno == ENOENT)
            ચાલુ;

          પાછા crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
libcrun_cgroup_pause_unpause_with_mode (અચલ અક્ષર *cgroup_path, પૂર્ણાંક cgroup_mode, અચલ bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free અક્ષર *path = ખાલી;
  અચલ અક્ષર *state = "";
  પૂર્ણાંક ret;

  જો (cgroup_path == ખાલી || cgroup_path[0] == '\0')
    પાછા crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  જો (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_મૂળ, cgroup_path, "cgroup.freeze", ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_મૂળ "/freezer", cgroup_path, "freezer.state", ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  જો (ret >= 0)
    પાછા 0;
  પાછા ret;
}

પૂર્ણાંક
libcrun_cgroup_pause_unpause_path (અચલ અક્ષર *cgroup_path, અચલ bool pause, libcrun_error_t *err)
{
  પૂર્ણાંક cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  પાછા libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
પૂર્ણાંક
libcrun_cgroup_ensure_not_frozen (અચલ અક્ષર *cgroup_path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *content = ખાલી;
  cleanup_free અક્ષર *path = ખાલી;
  અચલ અક્ષર *frozen;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  જો (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_મૂળ, cgroup_path, "cgroup.freeze", ખાલી);
    }
  નહીંતર
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_મૂળ "/freezer", cgroup_path, "freezer.state", ખાલી);
    }
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = read_all_file (path, &content, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      જો (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          પાછા 0;
        }
      પાછા ret;
    }

  જો (strstr (content, frozen))
    પાછા crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  પાછા 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
સ્થિર પૂર્ણાંક
watch_cgroup_events (અચલ અક્ષર *path)
{
  cleanup_free અક્ષર *events_file = ખાલી;
  cleanup_close પૂર્ણાંક fd = -1;
  libcrun_error_t tmp_err = ખાલી;
  પૂર્ણાંક ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_મૂળ, path, "cgroup.events", ખાલી);
  જો (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      પાછા -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  જો (UNLIKELY (fd < 0))
    પાછા -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  જો (UNLIKELY (ret < 0))
    પાછા -1;

  ret = fd;
  fd = -1;
  પાછા ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
સ્થિર પૂર્ણાંક
wait_cgroup_unpopulated (અચલ અક્ષર *path, પૂર્ણાંક inotify_fd)
{
  cleanup_free અક્ષર *events_file = ખાલી;
  libcrun_error_t tmp_err = ખાલી;
  સંરચના timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  પૂર્ણાંક ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_મૂળ, path, "cgroup.events", ખાલી);
  જો (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      પાછા 0;
    }

  માટે (;;)
    {
      cleanup_free અક્ષર *content = ખાલી;
      અક્ષર buf[4096];
      સંરચના pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, ખાલી, &tmp_err);
      જો (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          પાછા 0;
        }

      જો (strstr (content, "populated 0"))
        પાછા 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, ખાલી));
      જો (ret <= 0)
        પાછા 0;

      જ્યારે (પઠ (inotify_fd, buf, કદ (buf)) > 0)
        ;
    }
}

પૂર્ણાંક
cgroup_killall_path (અચલ અક્ષર *path, પૂર્ણાંક સંકેત, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close પૂર્ણાંક inotify_fd = -1;
  પૂર્ણાંક ret;
  size_t i;
  cleanup_free pid_t *pids = ખાલી;

  જો (path == ખાલી || *path == '\0')
    પાછા 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  જો (સંકેત == SIGKILL)
    {
      cleanup_free અક્ષર *kill_file = ખાલી;

      ret = append_paths (&kill_file, err, CGROUP_મૂળ, path, "cgroup.kill", ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      જો (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        પાછા 0;

      જો (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  જો (UNLIKELY (ret < 0))
    crun_error_release (err);
  નહીંતર
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, સંકેત == SIGKILL, &pids, err);
  જો (UNLIKELY (ret < 0))
    {
      જો (crun_error_get_errno (err) != ENOENT)
        જાઓ thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  માટે (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], સંકેત);
      જો (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          જાઓ thaw;
        }
    }

  ret = 0;

thaw:
  જો (frozen)
    {
      libcrun_error_t thaw_err = ખાલી;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      જો (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  પાછા ret;
}

સ્થિર પૂર્ણાંક
read_available_controllers (અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *controllers = ખાલી;
  cleanup_free અક્ષર *buf = ખાલી;
  અક્ષર *saveptr = ખાલી;
  અચલ અક્ષર *token;
  પૂર્ણાંક available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_મૂળ, path, "cgroup.controllers", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = read_all_file (controllers, &buf, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  માટે (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (ખાલી, " \n", &saveptr))
    {
      જો (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      નહીંતર જો (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      નહીંતર જો (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      નહીંતર જો (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      નહીંતર જો (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      નહીંતર જો (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      નહીંતર જો (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  પાછા available;
}

સ્થિર પૂર્ણાંક
write_controller_file (અચલ અક્ષર *path, પૂર્ણાંક controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free અક્ષર *subtree_control = ખાલી;
  cleanup_free અક્ષર *controllers = ખાલી;
  size_t controllers_len = 0;
  પૂર્ણાંક ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_મૂળ, path, "cgroup.subtree_control", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  જો (UNLIKELY (ret < 0))
    {
      cleanup_free અક્ષર *controllers_copy = xmalloc (controllers_len + 1);
      પૂર્ણાંક attempts_ડાબું;
      અક્ષર *saveptr = ખાલી;
      અચલ અક્ષર *token;
      પૂર્ણાંક e;

      e = crun_error_get_errno (err);
      જો (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        પાછા crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      જો (e == ENOENT)
        {
          libcrun_error_t tmp_err = ખાલી;
          પૂર્ણાંક exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          જો (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              પાછા ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          જો (exists == 0)
            પાછા ret;
        }

      crun_error_release (err);

      જો (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          જો (UNLIKELY (ret < 0))
            પાછા crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      માટે (attempts_ડાબું = 1000; attempts_ડાબું >= 0; attempts_ડાબું--)
        {
          પૂર્ણાંક controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          માટે (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (ખાલી, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              જો (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  જો (e == EBUSY)
                    repeat = true;

                  ચાલુ;
                }

              controllers_written++;
            }

          જો (! repeat)
            તોડો;

          /* If there was any controller written, just try once more without any delay.  */
          જો (controllers_written > 0 && attempts_ડાબું > 2)
            {
              attempts_ડાબું = 1;
              ચાલુ;
            }

          જો (attempts_ડાબું > 0)
            {
              સંરચના timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, ખાલી);
            }
        }

      /* Refresh what controllers are available.  */
      પાછા read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  પાછા controllers_to_enable;
}

પૂર્ણાંક
enable_controllers (અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *tmp_path = ખાલી;
  અક્ષર *it;
  પૂર્ણાંક ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  જો (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      જો ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  માટે (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free અક્ષર *cgroup_path = ખાલી;
      અક્ષર *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, tmp_path, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = mkdir (cgroup_path, 0755);
      જો (UNLIKELY (ret < 0 && errno != EEXIST))
        પાછા crun_make_error (err, errno, "create `%s`", cgroup_path);

      જો (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      *it = '/';
      it = next_slash;
    }
  પાછા 0;
}

પૂર્ણાંક
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, અચલ અક્ષર *path, bool create_if_missing, libcrun_error_t *err)
{
  પૂર્ણાંક cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  જો (path == ખાલી || *path == '\0')
    પાછા 0;

  પાછા enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
પૂર્ણાંક
libcrun_move_self_to_cgroups (અચલ અક્ષર *content, libcrun_error_t *err)
{
  cleanup_free અક્ષર *buf = xstrdup (content);
  અક્ષર *saveptr = ખાલી;
  અક્ષર *controller;
  અક્ષર *path;
  પૂર્ણાંક cgroup_mode;
  bool has_data;
  પૂર્ણાંક ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  માટે (has_data = read_proc_cgroup (buf, &saveptr, ખાલી, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (ખાલી, &saveptr, ખાલી, &controller, &path))
    {
      અચલ અક્ષર *subsystem = ખાલી;

      જો (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          જો (controller[0] != '\0')
            ચાલુ;
        }
      નહીંતર
        {
          cleanup_free અક્ષર *subsystem_path = ખાલી;

          /* Use the same names as enter_cgroup_v1 does.  */
          જો (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          જો (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          જો (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_મૂળ, subsystem, ખાલી);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          ret = crun_path_exists (subsystem_path, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          જો (ret == 0)
            ચાલુ;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

પૂર્ણાંક
libcrun_get_cgroup_dirfd (સંરચના libcrun_cgroup_status *status, અચલ અક્ષર *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free અક્ષર *path_to_cgroup = ખાલી;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક cgroupdirfd;
  પૂર્ણાંક ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (cgroup_mode < 0)
    પાછા cgroup_mode;

  જો (cgroup_mode != CGROUP_MODE_UNIFIED)
    પાછા crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  જો (status == ખાલી)
    પાછા crun_make_error (err, 0, "internal error");

  જો (is_empty_string (status->path))
    પાછા crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_મૂળ, status->path, sub_cgroup, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  જો (UNLIKELY (cgroupdirfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  પાછા cgroupdirfd;
}

પૂર્ણાંક
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, અક્ષર *from, અક્ષર *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = ખાલી;
  cleanup_close પૂર્ણાંક child_dfd = -1;
  પૂર્ણાંક cgroup_mode;
  size_t from_len;
  size_t i;
  પૂર્ણાંક ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (cgroup_mode < 0)
    પાછા cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  જો (UNLIKELY (ret < 0))
    જાઓ thaw;

  from_len = strlen (from);

  માટે (i = 0; pids && pids[i]; i++)
    {
      cleanup_free અક્ષર *pid_path = ખાલી;
      cleanup_free અક્ષર *dest_cgroup = ખાલી;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      જો (UNLIKELY (ret < 0))
        જાઓ thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      જો (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          જાઓ thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      જો (UNLIKELY (ret < 0))
        જાઓ thaw;
    }

  ret = 0;

thaw:
  જો (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = ખાલી;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      જો (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      પાછા ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા destroy_cgroup_path (from, cgroup_mode, err);
}

પૂર્ણાંક
get_cgroup_dirfd_path (પૂર્ણાંક dirfd, અક્ષર **path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cgroup_path = ખાલી;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  જો (UNLIKELY (len < 0))
    પાછા len;

  જો (has_prefix (cgroup_path, CGROUP_મૂળ))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_મૂળ));
      પાછા 0;
    }
  પાછા crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
પૂર્ણાંક
parse_cpu_max (અચલ અક્ષર *કિંમત, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  અચલ અક્ષર *it = કિંમત;
  અક્ષર *endptr = ખાલી;

  જ્યારે (*it == ' ')
    it++;

  errno = 0;
  જો (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (અક્ષર *) it + 3;
    }
  નહીંતર
    {
      *quota = strtoll (it, &endptr, 10);
      જો (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        પાછા crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", કિંમત);
    }

  it = endptr;
  જ્યારે (*it == ' ')
    it++;

  *has_period = false;
  જો (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      જો (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        પાછા crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", કિંમત);
      *has_period = true;
    }

  પાછા 0;
}
