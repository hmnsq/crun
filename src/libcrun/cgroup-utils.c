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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "cgroup.h"
#அந்தர்பாவ "cgroup-internal.h"
#அந்தர்பாவ "cgroup-systemd.h"
#அந்தர்பாவ "cgroup-utils.h"
#அந்தர்பாவ "cgroup-setup.h"
#அந்தர்பாவ "ebpf.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "status.h"
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <sys/vfs.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <poll.h>
#அந்தர்பாவ <sys/inotify.h>

#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <libgen.h>

கட்டமைப்பு symlink_s
{
  மாறிலி எழுத்து *name;
  மாறிலி எழுத்து *target;
};

நிலையான கட்டமைப்பு symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { வெற்று, வெற்று } };

முழு
libcrun_cgroups_create_symlinks (முழு dirfd, libcrun_error_t *err)
{
  முழு i;

  க்கு (i = 0; cgroup_symlinks[i].name; i++)
    {
      முழு ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (errno == ENOENT || errno == EEXIST)
            தொடர்;
          திரும்பு crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  திரும்பு 0;
}

முழு
maybe_make_cgroup_threaded (மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cgroup_path_type = வெற்று;
  மாறிலி எழுத்து *மாறிலி threaded = "threaded";
  cleanup_free எழுத்து *content = வெற்று;
  cleanup_free எழுத்து *buffer = வெற்று;
  மாறிலி எழுத்து *parent;
  size_t size;
  எழுத்து *it;
  முழு ret;

  எனில் (path == வெற்று)
    திரும்பு 0;

  path = consume_slashes (path);

  எனில் (path[0] == '\0')
    திரும்பு 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_வேர், path, "cgroup.type", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (size > 0)
    {
      it = content + size - 1;
      போது (*it == '\n' && it > content)
        *it-- = '\0';
    }

  எனில் (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    திரும்பு 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  எனில் (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      எனில் (ret < 0)
        திரும்பு ret;
    }
  திரும்பு write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

முழு
move_process_to_cgroup (pid_t pid, மாறிலி எழுத்து *subsystem, மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cgroup_path_procs = வெற்று;
  எழுத்து pid_str[16];
  முழு ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_வேர்,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = snprintf (pid_str, அளவு (pid_str), "%d", pid);
  எனில் (UNLIKELY (ret >= (முழு) அளவு (pid_str)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = வெற்று;
          முழு mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          எனில் (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              திரும்பு ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          திரும்பு write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      திரும்பு ret;
    }
  திரும்பு ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

நிலையான முழு
detect_cgroup_mode (libcrun_error_t *err)
{
  கட்டமைப்பு statfs stat;
  முழு ret;

  ret = statfs (CGROUP_வேர், &stat);
  எனில் (ret < 0)
    திரும்பு crun_make_error (err, errno, "statfs `" CGROUP_வேர் "`");
  எனில் (stat.f_type == CGROUP2_SUPER_MAGIC)
    திரும்பு CGROUP_MODE_UNIFIED;
  எனில் (stat.f_type != TMPFS_MAGIC)
    திரும்பு crun_make_error (err, 0, "invalid file system type on `" CGROUP_வேர் "`");
  ret = statfs (CGROUP_வேர் "/unified", &stat);
  எனில் (ret < 0 && errno != ENOENT)
    திரும்பு crun_make_error (err, errno, "statfs `" CGROUP_வேர் "/unified`");
  எனில் (ret < 0)
    திரும்பு CGROUP_MODE_LEGACY;
  திரும்பு stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

முழு
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  முழு tmp;
  நிலையான முழு cgroup_mode = 0;

  எனில் (cgroup_mode)
    திரும்பு cgroup_mode;

  tmp = detect_cgroup_mode (err);
  எனில் (UNLIKELY (tmp < 0))
    திரும்பு tmp;

  cgroup_mode = tmp;

  திரும்பு cgroup_mode;
}

நிலையான முழு
get_cgroup_process_from_content (எழுத்து *content, முழு cgroup_mode, எழுத்து **path, bool absolute, libcrun_error_t *err)
{
  எழுத்து *cg_path = வெற்று;
  எழுத்து *controller;
  எழுத்து *saveptr;
  bool has_data;

  க்கு (has_data = read_proc_cgroup (content, &saveptr, வெற்று, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (வெற்று, &saveptr, வெற்று, &controller, &cg_path))
    {
      எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          எனில் (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            செல் found;
        }
      இல்லையெனில்
        {
          எனில் (strcmp (controller, "memory"))
            செல் found;
        }
    }

  திரும்பு crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  எனில் (absolute)
    திரும்பு append_paths (path, err, CGROUP_வேர், cg_path, வெற்று);

  *path = xstrdup (cg_path);
  திரும்பு 0;
}

முழு
libcrun_get_cgroup_process_at (முழு dirfd, எழுத்து **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free எழுத்து *content = வெற்று;
  size_t content_size;
  முழு cgroup_mode;
  முழு ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

முழு
libcrun_get_cgroup_process (pid_t pid, எழுத்து **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free எழுத்து *content = வெற்று;
  எழுத்து proc_cgroup_file[64];
  size_t content_size;
  முழு cgroup_mode;
  முழு ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  எனில் (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  இல்லையெனில்
    {
      முழு len = snprintf (proc_cgroup_file, அளவு (proc_cgroup_file), "/proc/%d/cgroup", pid);
      எனில் (UNLIKELY (len >= (முழு) அளவு (proc_cgroup_file)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

நிலையான முழு
read_pids_file (முழு dfd, மாறிலி எழுத்து *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close முழு tasksfd = -1;
  cleanup_free எழுத்து *buffer = வெற்று;
  எழுத்து *saveptr = வெற்று;
  size_t n_new_pids;
  size_t len;
  எழுத்து *it;
  முழு ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  எனில் (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      எனில் (errno == ENOENT || errno == ENODEV)
        திரும்பு 0;
      திரும்பு crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (len > 0)
    {
      க்கு (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      எனில் (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, அளவு (pid_t) * *allocated);
        }

      க்கு (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (வெற்று, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, வெற்று, 10);

          எனில் (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  திரும்பு 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
கட்டமைப்பு cgroup_walk_dir_s
{
  எழுத்து *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
கட்டமைப்பு cgroup_walk_s
{
  முழு dfd;
  size_t depth;
  கட்டமைப்பு cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

நிலையான வெறுமை
cgroup_walk_எடு (கட்டமைப்பு cgroup_walk_s *walk)
{
  விடுவி (walk->dirs[--walk->n_dirs].name);
}

நிலையான வெறுமை
cleanup_cgroup_walkp (கட்டமைப்பு cgroup_walk_s *walk)
{
  முழு saved_errno = errno;

  போது (walk->n_dirs > 0)
    cgroup_walk_எடு (walk);
  விடுவி (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
நிலையான முழு
cgroup_walk_enter (கட்டமைப்பு cgroup_walk_s *walk, முழு dfd)
{
  cleanup_close முழு dfd_clone = -1;
  cleanup_dir DIR *dir = வெற்று;
  கட்டமைப்பு dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  எனில் (UNLIKELY (dfd_clone < 0))
    திரும்பு -1;

  dir = fdopendir (dfd_clone);
  எனில் (UNLIKELY (dir == வெற்று))
    திரும்பு -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  க்கு (de = readdir (dir); de; de = readdir (dir))
    {
      கட்டமைப்பு cgroup_walk_dir_s *d;

      எனில் (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        தொடர்;

      எனில் (de->d_type != DT_DIR)
        தொடர்;

      எனில் (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, அளவு (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  திரும்பு 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
நிலையான முழு
cgroup_walk_next (கட்டமைப்பு cgroup_walk_s *walk, கட்டமைப்பு cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : வெற்று;

  போது (*d && walk->depth >= (*d)->depth)
    {
      முழு parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      எனில் (UNLIKELY (parent < 0))
        திரும்பு -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  திரும்பு 0;
}

நிலையான முழு
read_pids_cgroup_file (முழு dfd, மாறிலி எழுத்து *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk கட்டமைப்பு cgroup_walk_s walk = { .dfd = -1 };
  முழு dfd_clone;
  முழு ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (! recurse)
    திரும்பு 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (dfd_clone < 0))
    திரும்பு crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "open cgroup sub-directory");

  க்கு (;;)
    {
      கட்டமைப்பு cgroup_walk_dir_s *d;
      cleanup_close முழு nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "open parent cgroup directory");
      எனில் (d == வெற்று)
        நிறுத்து;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      எனில் (UNLIKELY (nfd < 0))
        {
          முழு saved_errno = errno;

          எனில் (saved_errno == ENOENT)
            {
              cgroup_walk_எடு (&walk);
              தொடர்;
            }
          திரும்பு crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_எடு (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      எனில் (UNLIKELY (ret < 0))
        {
          முழு e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          எனில் (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            திரும்பு ret;
          crun_error_release (err);
          தொடர்;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      எனில் (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        திரும்பு crun_make_error (err, errno, "open cgroup sub-directory");
    }

  திரும்பு 0;
}

நிலையான முழு
read_pids_cgroup (முழு dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  முழு ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  எனில் (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  திரும்பு ret;
}

நிலையான முழு
rmdir_all_fd (முழு dfd)
{
  cleanup_cgroup_walk கட்டமைப்பு cgroup_walk_s walk = { .dfd = -1 };
  முழு ret;

  ret = cgroup_walk_enter (&walk, dfd);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  க்கு (;;)
    {
      கட்டமைப்பு cgroup_walk_dir_s *d;
      முழு child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      எனில் (d == வெற்று)
        நிறுத்து;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      எனில் (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_எடு (&walk);
          தொடர்;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      எனில் (UNLIKELY (child_dfd < 0))
        {
          எனில் (errno == ENOENT)
            {
              cgroup_walk_எடு (&walk);
              தொடர்;
            }
          திரும்பு child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      எனில் (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        திரும்பு ret;
    }

  திரும்பு 0;
}

நிலையான முழு
rmdir_all (மாறிலி எழுத்து *path)
{
  முழு ret;
  முழு dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  எனில் (UNLIKELY (dfd < 0))
    திரும்பு dfd;

  ret = rmdir_all_fd (dfd);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு rmdir (path);
}

நிலையான முழு
read_pids_from_path (மாறிலி எழுத்து *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cgroup_path = வெற்று;
  cleanup_close முழு dirfd = -1;
  size_t n_pids, allocated;
  முழு mode;
  முழு ret;

  எனில் (path == வெற்று || *path == '\0')
    திரும்பு 0;

  mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (mode < 0))
    திரும்பு mode;

  தேர்வு (mode)
    {
    நிலை CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_வேர், path, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      நிறுத்து;

    நிலை CGROUP_MODE_HYBRID:
    நிலை CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_வேர் "/memory", path, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      நிறுத்து;

    இயல்பு:
      திரும்பு crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (dirfd < 0)
    திரும்பு crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  எனில் (threads && mode == CGROUP_MODE_UNIFIED)
    திரும்பு read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  திரும்பு read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

முழு
libcrun_cgroup_read_pids_from_path (மாறிலி எழுத்து *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  திரும்பு read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (எழுத்து *content, எழுத்து **saveptr, எழுத்து **id, எழுத்து **controller_list, எழுத்து **path)
{
  எழுத்து *it;

  it = strtok_r (content, "\n", saveptr);
  எனில் (it == வெற்று)
    திரும்பு false;

  எனில் (id)
    *id = it;

  it = strchr (it, ':');
  எனில் (it == வெற்று)
    திரும்பு false;
  *it++ = '\0';

  எனில் (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  எனில் (it == வெற்று)
    திரும்பு false;
  *it++ = '\0';

  எனில் (path)
    *path = it;

  திரும்பு true;
}

முழு
destroy_cgroup_path (மாறிலி எழுத்து *path, முழு mode, libcrun_error_t *err)
{
  bool repeat = true;
  முழு retry_மொத்தம் = 0;
  மாறிலி முழு max_attempts = 500;
  முழு ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  எனில் (is_empty_string (path))
    திரும்பு 0;

  செய்
    {
      repeat = false;

      எனில் (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free எழுத்து *cgroup_path = வெற்று;

          ret = append_paths (&cgroup_path, err, CGROUP_வேர், path, வெற்று);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          ret = rmdir (cgroup_path);
          எனில் (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              எனில் (ret < 0)
                {
                  எனில் (retry_மொத்தம் >= max_attempts)
                    திரும்பு crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_மொத்தம்++;
                  repeat = true;
                }
            }
        }
      இல்லையெனில்
        {
          cleanup_free எழுத்து *content = வெற்று;
          size_t content_size;
          எழுத்து *controller;
          எழுத்து *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          எனில் (UNLIKELY (ret < 0))
            {
              எனில் (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  திரும்பு 0;
                }
              திரும்பு ret;
            }

          க்கு (has_data = read_proc_cgroup (content, &saveptr, வெற்று, &controller, வெற்று);
               has_data;
               has_data = read_proc_cgroup (வெற்று, &saveptr, வெற்று, &controller, வெற்று))
            {
              cleanup_free எழுத்து *cgroup_path = வெற்று;
              எழுத்து *subsystem;
              எனில் (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              எனில் (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                தொடர்;

              ret = append_paths (&cgroup_path, err, CGROUP_வேர், subsystem, path, வெற்று);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;

              ret = rmdir (cgroup_path);
              எனில் (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  எனில் (ret < 0)
                    {
                      எனில் (retry_மொத்தம் >= max_attempts)
                        திரும்பு crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_மொத்தம்++;
                      repeat = true;
                    }
                }
            }
        }

      எனில் (repeat)
        {
          கட்டமைப்பு timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, வெற்று);

          ret = cgroup_killall_path (path, SIGKILL, err);
          எனில் (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } போது (repeat);

  திரும்பு 0;
}

முழு
chown_cgroups (மாறிலி எழுத்து *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cgroup_path = வெற்று;
  cleanup_free எழுத்து *delegate = வெற்று;
  cleanup_close முழு dfd = -1;
  size_t delegate_size;
  எழுத்து *saveptr = வெற்று;
  எழுத்து *name;
  முழு ret;

  ret = append_paths (&cgroup_path, err, CGROUP_வேர், path, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  எனில் (UNLIKELY (dfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          திரும்பு 0;
        }
      திரும்பு ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  க்கு (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (வெற்று, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (errno == ENOENT)
            தொடர்;

          திரும்பு crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  திரும்பு 0;
}

நிலையான முழு
libcrun_cgroup_pause_unpause_with_mode (மாறிலி எழுத்து *cgroup_path, முழு cgroup_mode, மாறிலி bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free எழுத்து *path = வெற்று;
  மாறிலி எழுத்து *state = "";
  முழு ret;

  எனில் (cgroup_path == வெற்று || cgroup_path[0] == '\0')
    திரும்பு crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_வேர், cgroup_path, "cgroup.freeze", வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_வேர் "/freezer", cgroup_path, "freezer.state", வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  எனில் (ret >= 0)
    திரும்பு 0;
  திரும்பு ret;
}

முழு
libcrun_cgroup_pause_unpause_path (மாறிலி எழுத்து *cgroup_path, மாறிலி bool pause, libcrun_error_t *err)
{
  முழு cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  திரும்பு libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
முழு
libcrun_cgroup_ensure_not_frozen (மாறிலி எழுத்து *cgroup_path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *content = வெற்று;
  cleanup_free எழுத்து *path = வெற்று;
  மாறிலி எழுத்து *frozen;
  முழு cgroup_mode;
  முழு ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_வேர், cgroup_path, "cgroup.freeze", வெற்று);
    }
  இல்லையெனில்
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_வேர் "/freezer", cgroup_path, "freezer.state", வெற்று);
    }
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = read_all_file (path, &content, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      எனில் (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          திரும்பு 0;
        }
      திரும்பு ret;
    }

  எனில் (strstr (content, frozen))
    திரும்பு crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  திரும்பு 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
நிலையான முழு
watch_cgroup_events (மாறிலி எழுத்து *path)
{
  cleanup_free எழுத்து *events_file = வெற்று;
  cleanup_close முழு fd = -1;
  libcrun_error_t tmp_err = வெற்று;
  முழு ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_வேர், path, "cgroup.events", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      திரும்பு -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு -1;

  ret = fd;
  fd = -1;
  திரும்பு ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
நிலையான முழு
wait_cgroup_unpopulated (மாறிலி எழுத்து *path, முழு inotify_fd)
{
  cleanup_free எழுத்து *events_file = வெற்று;
  libcrun_error_t tmp_err = வெற்று;
  கட்டமைப்பு timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  முழு ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_வேர், path, "cgroup.events", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      திரும்பு 0;
    }

  க்கு (;;)
    {
      cleanup_free எழுத்து *content = வெற்று;
      எழுத்து buf[4096];
      கட்டமைப்பு pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, வெற்று, &tmp_err);
      எனில் (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          திரும்பு 0;
        }

      எனில் (strstr (content, "populated 0"))
        திரும்பு 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, வெற்று));
      எனில் (ret <= 0)
        திரும்பு 0;

      போது (பட (inotify_fd, buf, அளவு (buf)) > 0)
        ;
    }
}

முழு
cgroup_killall_path (மாறிலி எழுத்து *path, முழு சமிக்ஞை, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close முழு inotify_fd = -1;
  முழு ret;
  size_t i;
  cleanup_free pid_t *pids = வெற்று;

  எனில் (path == வெற்று || *path == '\0')
    திரும்பு 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  எனில் (சமிக்ஞை == SIGKILL)
    {
      cleanup_free எழுத்து *kill_file = வெற்று;

      ret = append_paths (&kill_file, err, CGROUP_வேர், path, "cgroup.kill", வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      எனில் (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        திரும்பு 0;

      எனில் (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  எனில் (UNLIKELY (ret < 0))
    crun_error_release (err);
  இல்லையெனில்
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, சமிக்ஞை == SIGKILL, &pids, err);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (crun_error_get_errno (err) != ENOENT)
        செல் thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  க்கு (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], சமிக்ஞை);
      எனில் (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          செல் thaw;
        }
    }

  ret = 0;

thaw:
  எனில் (frozen)
    {
      libcrun_error_t thaw_err = வெற்று;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      எனில் (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  திரும்பு ret;
}

நிலையான முழு
read_available_controllers (மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *controllers = வெற்று;
  cleanup_free எழுத்து *buf = வெற்று;
  எழுத்து *saveptr = வெற்று;
  மாறிலி எழுத்து *token;
  முழு available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_வேர், path, "cgroup.controllers", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = read_all_file (controllers, &buf, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  க்கு (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (வெற்று, " \n", &saveptr))
    {
      எனில் (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      இல்லையெனில் எனில் (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      இல்லையெனில் எனில் (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      இல்லையெனில் எனில் (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      இல்லையெனில் எனில் (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      இல்லையெனில் எனில் (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      இல்லையெனில் எனில் (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  திரும்பு available;
}

நிலையான முழு
write_controller_file (மாறிலி எழுத்து *path, முழு controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free எழுத்து *subtree_control = வெற்று;
  cleanup_free எழுத்து *controllers = வெற்று;
  size_t controllers_len = 0;
  முழு ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_வேர், path, "cgroup.subtree_control", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  எனில் (UNLIKELY (ret < 0))
    {
      cleanup_free எழுத்து *controllers_copy = xmalloc (controllers_len + 1);
      முழு attempts_இடது;
      எழுத்து *saveptr = வெற்று;
      மாறிலி எழுத்து *token;
      முழு e;

      e = crun_error_get_errno (err);
      எனில் (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        திரும்பு crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      எனில் (e == ENOENT)
        {
          libcrun_error_t tmp_err = வெற்று;
          முழு exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          எனில் (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              திரும்பு ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          எனில் (exists == 0)
            திரும்பு ret;
        }

      crun_error_release (err);

      எனில் (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      க்கு (attempts_இடது = 1000; attempts_இடது >= 0; attempts_இடது--)
        {
          முழு controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          க்கு (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (வெற்று, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              எனில் (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  எனில் (e == EBUSY)
                    repeat = true;

                  தொடர்;
                }

              controllers_written++;
            }

          எனில் (! repeat)
            நிறுத்து;

          /* If there was any controller written, just try once more without any delay.  */
          எனில் (controllers_written > 0 && attempts_இடது > 2)
            {
              attempts_இடது = 1;
              தொடர்;
            }

          எனில் (attempts_இடது > 0)
            {
              கட்டமைப்பு timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, வெற்று);
            }
        }

      /* Refresh what controllers are available.  */
      திரும்பு read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  திரும்பு controllers_to_enable;
}

முழு
enable_controllers (மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *tmp_path = வெற்று;
  எழுத்து *it;
  முழு ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  எனில் (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      எனில் ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  க்கு (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free எழுத்து *cgroup_path = வெற்று;
      எழுத்து *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_வேர், tmp_path, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = mkdir (cgroup_path, 0755);
      எனில் (UNLIKELY (ret < 0 && errno != EEXIST))
        திரும்பு crun_make_error (err, errno, "create `%s`", cgroup_path);

      எனில் (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      *it = '/';
      it = next_slash;
    }
  திரும்பு 0;
}

முழு
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, மாறிலி எழுத்து *path, bool create_if_missing, libcrun_error_t *err)
{
  முழு cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  எனில் (path == வெற்று || *path == '\0')
    திரும்பு 0;

  திரும்பு enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
முழு
libcrun_move_self_to_cgroups (மாறிலி எழுத்து *content, libcrun_error_t *err)
{
  cleanup_free எழுத்து *buf = xstrdup (content);
  எழுத்து *saveptr = வெற்று;
  எழுத்து *controller;
  எழுத்து *path;
  முழு cgroup_mode;
  bool has_data;
  முழு ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  க்கு (has_data = read_proc_cgroup (buf, &saveptr, வெற்று, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (வெற்று, &saveptr, வெற்று, &controller, &path))
    {
      மாறிலி எழுத்து *subsystem = வெற்று;

      எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          எனில் (controller[0] != '\0')
            தொடர்;
        }
      இல்லையெனில்
        {
          cleanup_free எழுத்து *subsystem_path = வெற்று;

          /* Use the same names as enter_cgroup_v1 does.  */
          எனில் (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          எனில் (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          எனில் (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_வேர், subsystem, வெற்று);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          ret = crun_path_exists (subsystem_path, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          எனில் (ret == 0)
            தொடர்;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

முழு
libcrun_get_cgroup_dirfd (கட்டமைப்பு libcrun_cgroup_status *status, மாறிலி எழுத்து *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free எழுத்து *path_to_cgroup = வெற்று;
  முழு cgroup_mode;
  முழு cgroupdirfd;
  முழு ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (cgroup_mode < 0)
    திரும்பு cgroup_mode;

  எனில் (cgroup_mode != CGROUP_MODE_UNIFIED)
    திரும்பு crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  எனில் (status == வெற்று)
    திரும்பு crun_make_error (err, 0, "internal error");

  எனில் (is_empty_string (status->path))
    திரும்பு crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_வேர், status->path, sub_cgroup, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  எனில் (UNLIKELY (cgroupdirfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  திரும்பு cgroupdirfd;
}

முழு
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, எழுத்து *from, எழுத்து *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = வெற்று;
  cleanup_close முழு child_dfd = -1;
  முழு cgroup_mode;
  size_t from_len;
  size_t i;
  முழு ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (cgroup_mode < 0)
    திரும்பு cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  எனில் (UNLIKELY (ret < 0))
    செல் thaw;

  from_len = strlen (from);

  க்கு (i = 0; pids && pids[i]; i++)
    {
      cleanup_free எழுத்து *pid_path = வெற்று;
      cleanup_free எழுத்து *dest_cgroup = வெற்று;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      எனில் (UNLIKELY (ret < 0))
        செல் thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      எனில் (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          செல் thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      எனில் (UNLIKELY (ret < 0))
        செல் thaw;
    }

  ret = 0;

thaw:
  எனில் (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = வெற்று;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      எனில் (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      திரும்பு ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு destroy_cgroup_path (from, cgroup_mode, err);
}

முழு
get_cgroup_dirfd_path (முழு dirfd, எழுத்து **path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cgroup_path = வெற்று;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  எனில் (UNLIKELY (len < 0))
    திரும்பு len;

  எனில் (has_prefix (cgroup_path, CGROUP_வேர்))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_வேர்));
      திரும்பு 0;
    }
  திரும்பு crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
முழு
parse_cpu_max (மாறிலி எழுத்து *மதிப்பு, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  மாறிலி எழுத்து *it = மதிப்பு;
  எழுத்து *endptr = வெற்று;

  போது (*it == ' ')
    it++;

  errno = 0;
  எனில் (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (எழுத்து *) it + 3;
    }
  இல்லையெனில்
    {
      *quota = strtoll (it, &endptr, 10);
      எனில் (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        திரும்பு crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", மதிப்பு);
    }

  it = endptr;
  போது (*it == ' ')
    it++;

  *has_period = false;
  எனில் (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      எனில் (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        திரும்பு crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", மதிப்பு);
      *has_period = true;
    }

  திரும்பு 0;
}
