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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "cgroup.h"
#അന്തര്ഭാവ "cgroup-internal.h"
#അന്തര്ഭാവ "cgroup-systemd.h"
#അന്തര്ഭാവ "cgroup-utils.h"
#അന്തര്ഭാവ "cgroup-setup.h"
#അന്തര്ഭാവ "ebpf.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "status.h"
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <sys/vfs.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <poll.h>
#അന്തര്ഭാവ <sys/inotify.h>

#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <libgen.h>

ഘടന symlink_s
{
  സ്ഥിരം അക്ഷരം *name;
  സ്ഥിരം അക്ഷരം *target;
};

സ്ഥിതം ഘടന symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { ശൂന്യ, ശൂന്യ } };

പൂർണ്ണസംഖ്യ
libcrun_cgroups_create_symlinks (പൂർണ്ണസംഖ്യ dirfd, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ i;

  വേണ്ടി (i = 0; cgroup_symlinks[i].name; i++)
    {
      പൂർണ്ണസംഖ്യ ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (errno == ENOENT || errno == EEXIST)
            തുടരുക;
          തിരികെ crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
maybe_make_cgroup_threaded (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cgroup_path_type = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *സ്ഥിരം threaded = "threaded";
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  cleanup_free അക്ഷരം *buffer = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *parent;
  size_t size;
  അക്ഷരം *it;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (path == ശൂന്യ)
    തിരികെ 0;

  path = consume_slashes (path);

  എങ്കിൽ (path[0] == '\0')
    തിരികെ 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_വേര്, path, "cgroup.type", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (size > 0)
    {
      it = content + size - 1;
      ആകുമ്പോൾ (*it == '\n' && it > content)
        *it-- = '\0';
    }

  എങ്കിൽ (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    തിരികെ 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  എങ്കിൽ (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      എങ്കിൽ (ret < 0)
        തിരികെ ret;
    }
  തിരികെ write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

പൂർണ്ണസംഖ്യ
move_process_to_cgroup (pid_t pid, സ്ഥിരം അക്ഷരം *subsystem, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cgroup_path_procs = ശൂന്യ;
  അക്ഷരം pid_str[16];
  പൂർണ്ണസംഖ്യ ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_വേര്,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = snprintf (pid_str, വലുപ്പം (pid_str), "%d", pid);
  എങ്കിൽ (UNLIKELY (ret >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (pid_str)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = ശൂന്യ;
          പൂർണ്ണസംഖ്യ mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          എങ്കിൽ (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              തിരികെ ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          തിരികെ write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      തിരികെ ret;
    }
  തിരികെ ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

സ്ഥിതം പൂർണ്ണസംഖ്യ
detect_cgroup_mode (libcrun_error_t *err)
{
  ഘടന statfs stat;
  പൂർണ്ണസംഖ്യ ret;

  ret = statfs (CGROUP_വേര്, &stat);
  എങ്കിൽ (ret < 0)
    തിരികെ crun_make_error (err, errno, "statfs `" CGROUP_വേര് "`");
  എങ്കിൽ (stat.f_type == CGROUP2_SUPER_MAGIC)
    തിരികെ CGROUP_MODE_UNIFIED;
  എങ്കിൽ (stat.f_type != TMPFS_MAGIC)
    തിരികെ crun_make_error (err, 0, "invalid file system type on `" CGROUP_വേര് "`");
  ret = statfs (CGROUP_വേര് "/unified", &stat);
  എങ്കിൽ (ret < 0 && errno != ENOENT)
    തിരികെ crun_make_error (err, errno, "statfs `" CGROUP_വേര് "/unified`");
  എങ്കിൽ (ret < 0)
    തിരികെ CGROUP_MODE_LEGACY;
  തിരികെ stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

പൂർണ്ണസംഖ്യ
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ tmp;
  സ്ഥിതം പൂർണ്ണസംഖ്യ cgroup_mode = 0;

  എങ്കിൽ (cgroup_mode)
    തിരികെ cgroup_mode;

  tmp = detect_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (tmp < 0))
    തിരികെ tmp;

  cgroup_mode = tmp;

  തിരികെ cgroup_mode;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_cgroup_process_from_content (അക്ഷരം *content, പൂർണ്ണസംഖ്യ cgroup_mode, അക്ഷരം **path, bool absolute, libcrun_error_t *err)
{
  അക്ഷരം *cg_path = ശൂന്യ;
  അക്ഷരം *controller;
  അക്ഷരം *saveptr;
  bool has_data;

  വേണ്ടി (has_data = read_proc_cgroup (content, &saveptr, ശൂന്യ, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (ശൂന്യ, &saveptr, ശൂന്യ, &controller, &cg_path))
    {
      എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          എങ്കിൽ (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            പോകുക found;
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (strcmp (controller, "memory"))
            പോകുക found;
        }
    }

  തിരികെ crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  എങ്കിൽ (absolute)
    തിരികെ append_paths (path, err, CGROUP_വേര്, cg_path, ശൂന്യ);

  *path = xstrdup (cg_path);
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_get_cgroup_process_at (പൂർണ്ണസംഖ്യ dirfd, അക്ഷരം **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  size_t content_size;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

പൂർണ്ണസംഖ്യ
libcrun_get_cgroup_process (pid_t pid, അക്ഷരം **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  അക്ഷരം proc_cgroup_file[64];
  size_t content_size;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  എങ്കിൽ (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  അല്ലെങ്കിൽ
    {
      പൂർണ്ണസംഖ്യ len = snprintf (proc_cgroup_file, വലുപ്പം (proc_cgroup_file), "/proc/%d/cgroup", pid);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (proc_cgroup_file)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_pids_file (പൂർണ്ണസംഖ്യ dfd, സ്ഥിരം അക്ഷരം *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ tasksfd = -1;
  cleanup_free അക്ഷരം *buffer = ശൂന്യ;
  അക്ഷരം *saveptr = ശൂന്യ;
  size_t n_new_pids;
  size_t len;
  അക്ഷരം *it;
  പൂർണ്ണസംഖ്യ ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  എങ്കിൽ (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      എങ്കിൽ (errno == ENOENT || errno == ENODEV)
        തിരികെ 0;
      തിരികെ crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (len > 0)
    {
      വേണ്ടി (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      എങ്കിൽ (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, വലുപ്പം (pid_t) * *allocated);
        }

      വേണ്ടി (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (ശൂന്യ, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, ശൂന്യ, 10);

          എങ്കിൽ (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  തിരികെ 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
ഘടന cgroup_walk_dir_s
{
  അക്ഷരം *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
ഘടന cgroup_walk_s
{
  പൂർണ്ണസംഖ്യ dfd;
  size_t depth;
  ഘടന cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

സ്ഥിതം ശൂന്യം
cgroup_walk_എടുക്കുക (ഘടന cgroup_walk_s *walk)
{
  സ്വതന്ത്രം (walk->dirs[--walk->n_dirs].name);
}

സ്ഥിതം ശൂന്യം
cleanup_cgroup_walkp (ഘടന cgroup_walk_s *walk)
{
  പൂർണ്ണസംഖ്യ saved_errno = errno;

  ആകുമ്പോൾ (walk->n_dirs > 0)
    cgroup_walk_എടുക്കുക (walk);
  സ്വതന്ത്രം (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
cgroup_walk_enter (ഘടന cgroup_walk_s *walk, പൂർണ്ണസംഖ്യ dfd)
{
  cleanup_close പൂർണ്ണസംഖ്യ dfd_clone = -1;
  cleanup_dir DIR *dir = ശൂന്യ;
  ഘടന dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (dfd_clone < 0))
    തിരികെ -1;

  dir = fdopendir (dfd_clone);
  എങ്കിൽ (UNLIKELY (dir == ശൂന്യ))
    തിരികെ -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  വേണ്ടി (de = readdir (dir); de; de = readdir (dir))
    {
      ഘടന cgroup_walk_dir_s *d;

      എങ്കിൽ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        തുടരുക;

      എങ്കിൽ (de->d_type != DT_DIR)
        തുടരുക;

      എങ്കിൽ (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, വലുപ്പം (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  തിരികെ 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
cgroup_walk_next (ഘടന cgroup_walk_s *walk, ഘടന cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : ശൂന്യ;

  ആകുമ്പോൾ (*d && walk->depth >= (*d)->depth)
    {
      പൂർണ്ണസംഖ്യ parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (parent < 0))
        തിരികെ -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_pids_cgroup_file (പൂർണ്ണസംഖ്യ dfd, സ്ഥിരം അക്ഷരം *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk ഘടന cgroup_walk_s walk = { .dfd = -1 };
  പൂർണ്ണസംഖ്യ dfd_clone;
  പൂർണ്ണസംഖ്യ ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (! recurse)
    തിരികെ 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (dfd_clone < 0))
    തിരികെ crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "open cgroup sub-directory");

  വേണ്ടി (;;)
    {
      ഘടന cgroup_walk_dir_s *d;
      cleanup_close പൂർണ്ണസംഖ്യ nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "open parent cgroup directory");
      എങ്കിൽ (d == ശൂന്യ)
        നിർത്തുക;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (nfd < 0))
        {
          പൂർണ്ണസംഖ്യ saved_errno = errno;

          എങ്കിൽ (saved_errno == ENOENT)
            {
              cgroup_walk_എടുക്കുക (&walk);
              തുടരുക;
            }
          തിരികെ crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_എടുക്കുക (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          പൂർണ്ണസംഖ്യ e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          എങ്കിൽ (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            തിരികെ ret;
          crun_error_release (err);
          തുടരുക;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      എങ്കിൽ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        തിരികെ crun_make_error (err, errno, "open cgroup sub-directory");
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_pids_cgroup (പൂർണ്ണസംഖ്യ dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  എങ്കിൽ (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
rmdir_all_fd (പൂർണ്ണസംഖ്യ dfd)
{
  cleanup_cgroup_walk ഘടന cgroup_walk_s walk = { .dfd = -1 };
  പൂർണ്ണസംഖ്യ ret;

  ret = cgroup_walk_enter (&walk, dfd);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  വേണ്ടി (;;)
    {
      ഘടന cgroup_walk_dir_s *d;
      പൂർണ്ണസംഖ്യ child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      എങ്കിൽ (d == ശൂന്യ)
        നിർത്തുക;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      എങ്കിൽ (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_എടുക്കുക (&walk);
          തുടരുക;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (child_dfd < 0))
        {
          എങ്കിൽ (errno == ENOENT)
            {
              cgroup_walk_എടുക്കുക (&walk);
              തുടരുക;
            }
          തിരികെ child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      എങ്കിൽ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        തിരികെ ret;
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
rmdir_all (സ്ഥിരം അക്ഷരം *path)
{
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (dfd < 0))
    തിരികെ dfd;

  ret = rmdir_all_fd (dfd);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ rmdir (path);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_pids_from_path (സ്ഥിരം അക്ഷരം *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ dirfd = -1;
  size_t n_pids, allocated;
  പൂർണ്ണസംഖ്യ mode;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (path == ശൂന്യ || *path == '\0')
    തിരികെ 0;

  mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (mode < 0))
    തിരികെ mode;

  തിരഞ്ഞെടുപ്പ് (mode)
    {
    അവസ്ഥ CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_വേര്, path, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      നിർത്തുക;

    അവസ്ഥ CGROUP_MODE_HYBRID:
    അവസ്ഥ CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_വേര് "/memory", path, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      തിരികെ crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (dirfd < 0)
    തിരികെ crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  എങ്കിൽ (threads && mode == CGROUP_MODE_UNIFIED)
    തിരികെ read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  തിരികെ read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_read_pids_from_path (സ്ഥിരം അക്ഷരം *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  തിരികെ read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (അക്ഷരം *content, അക്ഷരം **saveptr, അക്ഷരം **id, അക്ഷരം **controller_list, അക്ഷരം **path)
{
  അക്ഷരം *it;

  it = strtok_r (content, "\n", saveptr);
  എങ്കിൽ (it == ശൂന്യ)
    തിരികെ false;

  എങ്കിൽ (id)
    *id = it;

  it = strchr (it, ':');
  എങ്കിൽ (it == ശൂന്യ)
    തിരികെ false;
  *it++ = '\0';

  എങ്കിൽ (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  എങ്കിൽ (it == ശൂന്യ)
    തിരികെ false;
  *it++ = '\0';

  എങ്കിൽ (path)
    *path = it;

  തിരികെ true;
}

പൂർണ്ണസംഖ്യ
destroy_cgroup_path (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode, libcrun_error_t *err)
{
  bool repeat = true;
  പൂർണ്ണസംഖ്യ retry_എണ്ണം = 0;
  സ്ഥിരം പൂർണ്ണസംഖ്യ max_attempts = 500;
  പൂർണ്ണസംഖ്യ ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  എങ്കിൽ (is_empty_string (path))
    തിരികെ 0;

  ചെയ്യുക
    {
      repeat = false;

      എങ്കിൽ (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;

          ret = append_paths (&cgroup_path, err, CGROUP_വേര്, path, ശൂന്യ);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          ret = rmdir (cgroup_path);
          എങ്കിൽ (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              എങ്കിൽ (ret < 0)
                {
                  എങ്കിൽ (retry_എണ്ണം >= max_attempts)
                    തിരികെ crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_എണ്ണം++;
                  repeat = true;
                }
            }
        }
      അല്ലെങ്കിൽ
        {
          cleanup_free അക്ഷരം *content = ശൂന്യ;
          size_t content_size;
          അക്ഷരം *controller;
          അക്ഷരം *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  തിരികെ 0;
                }
              തിരികെ ret;
            }

          വേണ്ടി (has_data = read_proc_cgroup (content, &saveptr, ശൂന്യ, &controller, ശൂന്യ);
               has_data;
               has_data = read_proc_cgroup (ശൂന്യ, &saveptr, ശൂന്യ, &controller, ശൂന്യ))
            {
              cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
              അക്ഷരം *subsystem;
              എങ്കിൽ (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              എങ്കിൽ (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                തുടരുക;

              ret = append_paths (&cgroup_path, err, CGROUP_വേര്, subsystem, path, ശൂന്യ);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;

              ret = rmdir (cgroup_path);
              എങ്കിൽ (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  എങ്കിൽ (ret < 0)
                    {
                      എങ്കിൽ (retry_എണ്ണം >= max_attempts)
                        തിരികെ crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_എണ്ണം++;
                      repeat = true;
                    }
                }
            }
        }

      എങ്കിൽ (repeat)
        {
          ഘടന timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, ശൂന്യ);

          ret = cgroup_killall_path (path, SIGKILL, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } ആകുമ്പോൾ (repeat);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
chown_cgroups (സ്ഥിരം അക്ഷരം *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
  cleanup_free അക്ഷരം *delegate = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ dfd = -1;
  size_t delegate_size;
  അക്ഷരം *saveptr = ശൂന്യ;
  അക്ഷരം *name;
  പൂർണ്ണസംഖ്യ ret;

  ret = append_paths (&cgroup_path, err, CGROUP_വേര്, path, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  എങ്കിൽ (UNLIKELY (dfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          തിരികെ 0;
        }
      തിരികെ ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  വേണ്ടി (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (ശൂന്യ, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (errno == ENOENT)
            തുടരുക;

          തിരികെ crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_cgroup_pause_unpause_with_mode (സ്ഥിരം അക്ഷരം *cgroup_path, പൂർണ്ണസംഖ്യ cgroup_mode, സ്ഥിരം bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *path = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *state = "";
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (cgroup_path == ശൂന്യ || cgroup_path[0] == '\0')
    തിരികെ crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_വേര്, cgroup_path, "cgroup.freeze", ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_വേര് "/freezer", cgroup_path, "freezer.state", ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  എങ്കിൽ (ret >= 0)
    തിരികെ 0;
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_cgroup_pause_unpause_path (സ്ഥിരം അക്ഷരം *cgroup_path, സ്ഥിരം bool pause, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  തിരികെ libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
പൂർണ്ണസംഖ്യ
libcrun_cgroup_ensure_not_frozen (സ്ഥിരം അക്ഷരം *cgroup_path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  cleanup_free അക്ഷരം *path = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *frozen;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_വേര്, cgroup_path, "cgroup.freeze", ശൂന്യ);
    }
  അല്ലെങ്കിൽ
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_വേര് "/freezer", cgroup_path, "freezer.state", ശൂന്യ);
    }
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = read_all_file (path, &content, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          തിരികെ 0;
        }
      തിരികെ ret;
    }

  എങ്കിൽ (strstr (content, frozen))
    തിരികെ crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  തിരികെ 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
watch_cgroup_events (സ്ഥിരം അക്ഷരം *path)
{
  cleanup_free അക്ഷരം *events_file = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  libcrun_error_t tmp_err = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_വേര്, path, "cgroup.events", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      തിരികെ -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ -1;

  ret = fd;
  fd = -1;
  തിരികെ ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
wait_cgroup_unpopulated (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ inotify_fd)
{
  cleanup_free അക്ഷരം *events_file = ശൂന്യ;
  libcrun_error_t tmp_err = ശൂന്യ;
  ഘടന timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  പൂർണ്ണസംഖ്യ ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_വേര്, path, "cgroup.events", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      തിരികെ 0;
    }

  വേണ്ടി (;;)
    {
      cleanup_free അക്ഷരം *content = ശൂന്യ;
      അക്ഷരം buf[4096];
      ഘടന pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, ശൂന്യ, &tmp_err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          തിരികെ 0;
        }

      എങ്കിൽ (strstr (content, "populated 0"))
        തിരികെ 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, ശൂന്യ));
      എങ്കിൽ (ret <= 0)
        തിരികെ 0;

      ആകുമ്പോൾ (പഠ (inotify_fd, buf, വലുപ്പം (buf)) > 0)
        ;
    }
}

പൂർണ്ണസംഖ്യ
cgroup_killall_path (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ സൂചന, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close പൂർണ്ണസംഖ്യ inotify_fd = -1;
  പൂർണ്ണസംഖ്യ ret;
  size_t i;
  cleanup_free pid_t *pids = ശൂന്യ;

  എങ്കിൽ (path == ശൂന്യ || *path == '\0')
    തിരികെ 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  എങ്കിൽ (സൂചന == SIGKILL)
    {
      cleanup_free അക്ഷരം *kill_file = ശൂന്യ;

      ret = append_paths (&kill_file, err, CGROUP_വേര്, path, "cgroup.kill", ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      എങ്കിൽ (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        തിരികെ 0;

      എങ്കിൽ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    crun_error_release (err);
  അല്ലെങ്കിൽ
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, സൂചന == SIGKILL, &pids, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (crun_error_get_errno (err) != ENOENT)
        പോകുക thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  വേണ്ടി (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], സൂചന);
      എങ്കിൽ (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          പോകുക thaw;
        }
    }

  ret = 0;

thaw:
  എങ്കിൽ (frozen)
    {
      libcrun_error_t thaw_err = ശൂന്യ;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      എങ്കിൽ (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_available_controllers (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *controllers = ശൂന്യ;
  cleanup_free അക്ഷരം *buf = ശൂന്യ;
  അക്ഷരം *saveptr = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *token;
  പൂർണ്ണസംഖ്യ available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_വേര്, path, "cgroup.controllers", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = read_all_file (controllers, &buf, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  വേണ്ടി (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (ശൂന്യ, " \n", &saveptr))
    {
      എങ്കിൽ (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  തിരികെ available;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_controller_file (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *subtree_control = ശൂന്യ;
  cleanup_free അക്ഷരം *controllers = ശൂന്യ;
  size_t controllers_len = 0;
  പൂർണ്ണസംഖ്യ ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_വേര്, path, "cgroup.subtree_control", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      cleanup_free അക്ഷരം *controllers_copy = xmalloc (controllers_len + 1);
      പൂർണ്ണസംഖ്യ attempts_ഇടത്;
      അക്ഷരം *saveptr = ശൂന്യ;
      സ്ഥിരം അക്ഷരം *token;
      പൂർണ്ണസംഖ്യ e;

      e = crun_error_get_errno (err);
      എങ്കിൽ (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        തിരികെ crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      എങ്കിൽ (e == ENOENT)
        {
          libcrun_error_t tmp_err = ശൂന്യ;
          പൂർണ്ണസംഖ്യ exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          എങ്കിൽ (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              തിരികെ ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          എങ്കിൽ (exists == 0)
            തിരികെ ret;
        }

      crun_error_release (err);

      എങ്കിൽ (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      വേണ്ടി (attempts_ഇടത് = 1000; attempts_ഇടത് >= 0; attempts_ഇടത്--)
        {
          പൂർണ്ണസംഖ്യ controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          വേണ്ടി (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (ശൂന്യ, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              എങ്കിൽ (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  എങ്കിൽ (e == EBUSY)
                    repeat = true;

                  തുടരുക;
                }

              controllers_written++;
            }

          എങ്കിൽ (! repeat)
            നിർത്തുക;

          /* If there was any controller written, just try once more without any delay.  */
          എങ്കിൽ (controllers_written > 0 && attempts_ഇടത് > 2)
            {
              attempts_ഇടത് = 1;
              തുടരുക;
            }

          എങ്കിൽ (attempts_ഇടത് > 0)
            {
              ഘടന timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, ശൂന്യ);
            }
        }

      /* Refresh what controllers are available.  */
      തിരികെ read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  തിരികെ controllers_to_enable;
}

പൂർണ്ണസംഖ്യ
enable_controllers (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *tmp_path = ശൂന്യ;
  അക്ഷരം *it;
  പൂർണ്ണസംഖ്യ ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      എങ്കിൽ ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  വേണ്ടി (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
      അക്ഷരം *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_വേര്, tmp_path, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = mkdir (cgroup_path, 0755);
      എങ്കിൽ (UNLIKELY (ret < 0 && errno != EEXIST))
        തിരികെ crun_make_error (err, errno, "create `%s`", cgroup_path);

      എങ്കിൽ (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      *it = '/';
      it = next_slash;
    }
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, സ്ഥിരം അക്ഷരം *path, bool create_if_missing, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  എങ്കിൽ (path == ശൂന്യ || *path == '\0')
    തിരികെ 0;

  തിരികെ enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
പൂർണ്ണസംഖ്യ
libcrun_move_self_to_cgroups (സ്ഥിരം അക്ഷരം *content, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *buf = xstrdup (content);
  അക്ഷരം *saveptr = ശൂന്യ;
  അക്ഷരം *controller;
  അക്ഷരം *path;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  bool has_data;
  പൂർണ്ണസംഖ്യ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  വേണ്ടി (has_data = read_proc_cgroup (buf, &saveptr, ശൂന്യ, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (ശൂന്യ, &saveptr, ശൂന്യ, &controller, &path))
    {
      സ്ഥിരം അക്ഷരം *subsystem = ശൂന്യ;

      എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          എങ്കിൽ (controller[0] != '\0')
            തുടരുക;
        }
      അല്ലെങ്കിൽ
        {
          cleanup_free അക്ഷരം *subsystem_path = ശൂന്യ;

          /* Use the same names as enter_cgroup_v1 does.  */
          എങ്കിൽ (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          എങ്കിൽ (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          എങ്കിൽ (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_വേര്, subsystem, ശൂന്യ);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          ret = crun_path_exists (subsystem_path, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          എങ്കിൽ (ret == 0)
            തുടരുക;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_get_cgroup_dirfd (ഘടന libcrun_cgroup_status *status, സ്ഥിരം അക്ഷരം *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *path_to_cgroup = ശൂന്യ;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ cgroupdirfd;
  പൂർണ്ണസംഖ്യ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (cgroup_mode < 0)
    തിരികെ cgroup_mode;

  എങ്കിൽ (cgroup_mode != CGROUP_MODE_UNIFIED)
    തിരികെ crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  എങ്കിൽ (status == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "internal error");

  എങ്കിൽ (is_empty_string (status->path))
    തിരികെ crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_വേര്, status->path, sub_cgroup, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  എങ്കിൽ (UNLIKELY (cgroupdirfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  തിരികെ cgroupdirfd;
}

പൂർണ്ണസംഖ്യ
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, അക്ഷരം *from, അക്ഷരം *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ child_dfd = -1;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  size_t from_len;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (cgroup_mode < 0)
    തിരികെ cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക thaw;

  from_len = strlen (from);

  വേണ്ടി (i = 0; pids && pids[i]; i++)
    {
      cleanup_free അക്ഷരം *pid_path = ശൂന്യ;
      cleanup_free അക്ഷരം *dest_cgroup = ശൂന്യ;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      എങ്കിൽ (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          പോകുക thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക thaw;
    }

  ret = 0;

thaw:
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = ശൂന്യ;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      എങ്കിൽ (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      തിരികെ ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ destroy_cgroup_path (from, cgroup_mode, err);
}

പൂർണ്ണസംഖ്യ
get_cgroup_dirfd_path (പൂർണ്ണസംഖ്യ dirfd, അക്ഷരം **path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  എങ്കിൽ (UNLIKELY (len < 0))
    തിരികെ len;

  എങ്കിൽ (has_prefix (cgroup_path, CGROUP_വേര്))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_വേര്));
      തിരികെ 0;
    }
  തിരികെ crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
പൂർണ്ണസംഖ്യ
parse_cpu_max (സ്ഥിരം അക്ഷരം *മൂല്യം, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *it = മൂല്യം;
  അക്ഷരം *endptr = ശൂന്യ;

  ആകുമ്പോൾ (*it == ' ')
    it++;

  errno = 0;
  എങ്കിൽ (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (അക്ഷരം *) it + 3;
    }
  അല്ലെങ്കിൽ
    {
      *quota = strtoll (it, &endptr, 10);
      എങ്കിൽ (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        തിരികെ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", മൂല്യം);
    }

  it = endptr;
  ആകുമ്പോൾ (*it == ' ')
    it++;

  *has_period = false;
  എങ്കിൽ (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      എങ്കിൽ (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        തിരികെ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", മൂല്യം);
      *has_period = true;
    }

  തിരികെ 0;
}
