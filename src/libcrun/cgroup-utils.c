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

#ضمن <config.h>
#ضمن "cgroup.h"
#ضمن "cgroup-internal.h"
#ضمن "cgroup-systemd.h"
#ضمن "cgroup-utils.h"
#ضمن "cgroup-setup.h"
#ضمن "ebpf.h"
#ضمن "utils.h"
#ضمن "status.h"
#ضمن <string.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <sys/vfs.h>
#ضمن <inttypes.h>
#ضمن <time.h>
#ضمن <poll.h>
#ضمن <sys/inotify.h>

#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <libgen.h>

بنية symlink_s
{
  ثابت حرف *name;
  ثابت حرف *target;
};

ساكن بنية symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { عدم, عدم } };

صحيح
libcrun_cgroups_create_symlinks (صحيح dirfd, libcrun_error_t *err)
{
  صحيح i;

  עבור (i = 0; cgroup_symlinks[i].name; i++)
    {
      صحيح ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      אם (UNLIKELY (ret < 0))
        {
          אם (errno == ENOENT || errno == EEXIST)
            استمر;
          החזר crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  החזר 0;
}

صحيح
maybe_make_cgroup_threaded (ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path_type = عدم;
  ثابت حرف *ثابت threaded = "threaded";
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *buffer = عدم;
  ثابت حرف *parent;
  size_t size;
  حرف *it;
  صحيح ret;

  אם (path == عدم)
    החזר 0;

  path = consume_slashes (path);

  אם (path[0] == '\0')
    החזר 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ROOT, path, "cgroup.type", عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (size > 0)
    {
      it = content + size - 1;
      כלעוד (*it == '\n' && it > content)
        *it-- = '\0';
    }

  אם (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    החזר 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  אם (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      אם (ret < 0)
        החזר ret;
    }
  החזר write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

صحيح
move_process_to_cgroup (pid_t pid, ثابت حرف *subsystem, ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path_procs = عدم;
  حرف pid_str[16];
  صحيح ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ROOT,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = snprintf (pid_str, حجم (pid_str), "%d", pid);
  אם (UNLIKELY (ret >= (صحيح) حجم (pid_str)))
    החזר crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  אם (UNLIKELY (ret < 0))
    {
      אם (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = عدم;
          صحيح mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          אם (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              החזר ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;

          החזר write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      החזר ret;
    }
  החזר ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

ساكن صحيح
detect_cgroup_mode (libcrun_error_t *err)
{
  بنية statfs stat;
  صحيح ret;

  ret = statfs (CGROUP_ROOT, &stat);
  אם (ret < 0)
    החזר crun_make_error (err, errno, "statfs `" CGROUP_ROOT "`");
  אם (stat.f_type == CGROUP2_SUPER_MAGIC)
    החזר CGROUP_MODE_UNIFIED;
  אם (stat.f_type != TMPFS_MAGIC)
    החזר crun_make_error (err, 0, "invalid file system type on `" CGROUP_ROOT "`");
  ret = statfs (CGROUP_ROOT "/unified", &stat);
  אם (ret < 0 && errno != ENOENT)
    החזר crun_make_error (err, errno, "statfs `" CGROUP_ROOT "/unified`");
  אם (ret < 0)
    החזר CGROUP_MODE_LEGACY;
  החזר stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

صحيح
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  صحيح tmp;
  ساكن صحيح cgroup_mode = 0;

  אם (cgroup_mode)
    החזר cgroup_mode;

  tmp = detect_cgroup_mode (err);
  אם (UNLIKELY (tmp < 0))
    החזר tmp;

  cgroup_mode = tmp;

  החזר cgroup_mode;
}

ساكن صحيح
get_cgroup_process_from_content (حرف *content, صحيح cgroup_mode, حرف **path, bool absolute, libcrun_error_t *err)
{
  حرف *cg_path = عدم;
  حرف *controller;
  حرف *saveptr;
  bool has_data;

  עבור (has_data = read_proc_cgroup (content, &saveptr, عدم, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (عدم, &saveptr, عدم, &controller, &cg_path))
    {
      אם (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          אם (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            اذهب found;
        }
      אחרת
        {
          אם (strcmp (controller, "memory"))
            اذهب found;
        }
    }

  החזר crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  אם (absolute)
    החזר append_paths (path, err, CGROUP_ROOT, cg_path, عدم);

  *path = xstrdup (cg_path);
  החזר 0;
}

صحيح
libcrun_get_cgroup_process_at (صحيح dirfd, حرف **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  size_t content_size;
  صحيح cgroup_mode;
  صحيح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

صحيح
libcrun_get_cgroup_process (pid_t pid, حرف **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  حرف proc_cgroup_file[64];
  size_t content_size;
  صحيح cgroup_mode;
  صحيح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  אם (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  אחרת
    {
      صحيح len = snprintf (proc_cgroup_file, حجم (proc_cgroup_file), "/proc/%d/cgroup", pid);
      אם (UNLIKELY (len >= (صحيح) حجم (proc_cgroup_file)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

ساكن صحيح
read_pids_file (صحيح dfd, ثابت حرف *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close صحيح tasksfd = -1;
  cleanup_free حرف *buffer = عدم;
  حرف *saveptr = عدم;
  size_t n_new_pids;
  size_t len;
  حرف *it;
  صحيح ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  אם (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      אם (errno == ENOENT || errno == ENODEV)
        החזר 0;
      החזר crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (len > 0)
    {
      עבור (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      אם (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, حجم (pid_t) * *allocated);
        }

      עבור (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (عدم, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, عدم, 10);

          אם (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  החזר 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
بنية cgroup_walk_dir_s
{
  حرف *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
بنية cgroup_walk_s
{
  صحيح dfd;
  size_t depth;
  بنية cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

ساكن فراغ
cgroup_walk_pop (بنية cgroup_walk_s *walk)
{
  حرر (walk->dirs[--walk->n_dirs].name);
}

ساكن فراغ
cleanup_cgroup_walkp (بنية cgroup_walk_s *walk)
{
  صحيح saved_errno = errno;

  כלעוד (walk->n_dirs > 0)
    cgroup_walk_pop (walk);
  حرر (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
ساكن صحيح
cgroup_walk_enter (بنية cgroup_walk_s *walk, صحيح dfd)
{
  cleanup_close صحيح dfd_clone = -1;
  cleanup_dir DIR *dir = عدم;
  بنية dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  אם (UNLIKELY (dfd_clone < 0))
    החזר -1;

  dir = fdopendir (dfd_clone);
  אם (UNLIKELY (dir == عدم))
    החזר -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  עבור (de = readdir (dir); de; de = readdir (dir))
    {
      بنية cgroup_walk_dir_s *d;

      אם (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        استمر;

      אם (de->d_type != DT_DIR)
        استمر;

      אם (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, حجم (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  החזר 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
ساكن صحيح
cgroup_walk_next (بنية cgroup_walk_s *walk, بنية cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : عدم;

  כלעוד (*d && walk->depth >= (*d)->depth)
    {
      صحيح parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      אם (UNLIKELY (parent < 0))
        החזר -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  החזר 0;
}

ساكن صحيح
read_pids_cgroup_file (صحيح dfd, ثابت حرف *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk بنية cgroup_walk_s walk = { .dfd = -1 };
  صحيح dfd_clone;
  صحيح ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (! recurse)
    החזר 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  אם (UNLIKELY (dfd_clone < 0))
    החזר crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "open cgroup sub-directory");

  עבור (;;)
    {
      بنية cgroup_walk_dir_s *d;
      cleanup_close صحيح nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      אם (UNLIKELY (ret < 0))
        החזר crun_make_error (err, errno, "open parent cgroup directory");
      אם (d == عدم)
        اكسر;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      אם (UNLIKELY (nfd < 0))
        {
          صحيح saved_errno = errno;

          אם (saved_errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              استمر;
            }
          החזר crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_pop (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      אם (UNLIKELY (ret < 0))
        {
          صحيح e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          אם (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            החזר ret;
          crun_error_release (err);
          استمر;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      אם (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        החזר crun_make_error (err, errno, "open cgroup sub-directory");
    }

  החזר 0;
}

ساكن صحيح
read_pids_cgroup (صحيح dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  صحيح ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  אם (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  החזר ret;
}

ساكن صحيح
rmdir_all_fd (صحيح dfd)
{
  cleanup_cgroup_walk بنية cgroup_walk_s walk = { .dfd = -1 };
  صحيح ret;

  ret = cgroup_walk_enter (&walk, dfd);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  עבור (;;)
    {
      بنية cgroup_walk_dir_s *d;
      صحيح child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      אם (UNLIKELY (ret < 0))
        החזר ret;
      אם (d == عدم)
        اكسر;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      אם (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_pop (&walk);
          استمر;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      אם (UNLIKELY (child_dfd < 0))
        {
          אם (errno == ENOENT)
            {
              cgroup_walk_pop (&walk);
              استمر;
            }
          החזר child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      אם (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        החזר ret;
    }

  החזר 0;
}

ساكن صحيح
rmdir_all (ثابت حرف *path)
{
  صحيح ret;
  صحيح dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  אם (UNLIKELY (dfd < 0))
    החזר dfd;

  ret = rmdir_all_fd (dfd);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר rmdir (path);
}

ساكن صحيح
read_pids_from_path (ثابت حرف *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path = عدم;
  cleanup_close صحيح dirfd = -1;
  size_t n_pids, allocated;
  صحيح mode;
  صحيح ret;

  אם (path == عدم || *path == '\0')
    החזר 0;

  mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (mode < 0))
    החזר mode;

  بدل (mode)
    {
    حالة CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;
      اكسر;

    حالة CGROUP_MODE_HYBRID:
    حالة CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ROOT "/memory", path, عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;
      اكسر;

    افتراضي:
      החזר crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  אם (dirfd < 0)
    החזר crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  אם (threads && mode == CGROUP_MODE_UNIFIED)
    החזר read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  החזר read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

صحيح
libcrun_cgroup_read_pids_from_path (ثابت حرف *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  החזר read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (حرف *content, حرف **saveptr, حرف **id, حرف **controller_list, حرف **path)
{
  حرف *it;

  it = strtok_r (content, "\n", saveptr);
  אם (it == عدم)
    החזר false;

  אם (id)
    *id = it;

  it = strchr (it, ':');
  אם (it == عدم)
    החזר false;
  *it++ = '\0';

  אם (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  אם (it == عدم)
    החזר false;
  *it++ = '\0';

  אם (path)
    *path = it;

  החזר true;
}

صحيح
destroy_cgroup_path (ثابت حرف *path, صحيح mode, libcrun_error_t *err)
{
  bool repeat = true;
  صحيح retry_count = 0;
  ثابت صحيح max_attempts = 500;
  صحيح ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  אם (is_empty_string (path))
    החזר 0;

  עשה
    {
      repeat = false;

      אם (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free حرف *cgroup_path = عدم;

          ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, عدم);
          אם (UNLIKELY (ret < 0))
            החזר ret;
          ret = rmdir (cgroup_path);
          אם (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              אם (ret < 0)
                {
                  אם (retry_count >= max_attempts)
                    החזר crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_count++;
                  repeat = true;
                }
            }
        }
      אחרת
        {
          cleanup_free حرف *content = عدم;
          size_t content_size;
          حرف *controller;
          حرف *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          אם (UNLIKELY (ret < 0))
            {
              אם (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  החזר 0;
                }
              החזר ret;
            }

          עבור (has_data = read_proc_cgroup (content, &saveptr, عدم, &controller, عدم);
               has_data;
               has_data = read_proc_cgroup (عدم, &saveptr, عدم, &controller, عدم))
            {
              cleanup_free حرف *cgroup_path = عدم;
              حرف *subsystem;
              אם (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              אם (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                استمر;

              ret = append_paths (&cgroup_path, err, CGROUP_ROOT, subsystem, path, عدم);
              אם (UNLIKELY (ret < 0))
                החזר ret;

              ret = rmdir (cgroup_path);
              אם (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  אם (ret < 0)
                    {
                      אם (retry_count >= max_attempts)
                        החזר crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_count++;
                      repeat = true;
                    }
                }
            }
        }

      אם (repeat)
        {
          بنية timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, عدم);

          ret = cgroup_killall_path (path, SIGKILL, err);
          אם (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } כלעוד (repeat);

  החזר 0;
}

صحيح
chown_cgroups (ثابت حرف *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path = عدم;
  cleanup_free حرف *delegate = عدم;
  cleanup_close صحيح dfd = -1;
  size_t delegate_size;
  حرف *saveptr = عدم;
  حرف *name;
  صحيح ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  אם (UNLIKELY (dfd < 0))
    החזר crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  אם (UNLIKELY (ret < 0))
    {
      אם (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          החזר 0;
        }
      החזר ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  עבור (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (عدم, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      אם (UNLIKELY (ret < 0))
        {
          אם (errno == ENOENT)
            استمر;

          החזר crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  החזר 0;
}

ساكن صحيح
libcrun_cgroup_pause_unpause_with_mode (ثابت حرف *cgroup_path, صحيح cgroup_mode, ثابت bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free حرف *path = عدم;
  ثابت حرف *state = "";
  صحيح ret;

  אם (cgroup_path == عدم || cgroup_path[0] == '\0')
    החזר crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  אם (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אחרת
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  אם (ret >= 0)
    החזר 0;
  החזר ret;
}

صحيح
libcrun_cgroup_pause_unpause_path (ثابت حرف *cgroup_path, ثابت bool pause, libcrun_error_t *err)
{
  صحيح cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  החזר libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
صحيح
libcrun_cgroup_ensure_not_frozen (ثابت حرف *cgroup_path, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *path = عدم;
  ثابت حرف *frozen;
  صحيح cgroup_mode;
  صحيح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  אם (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ROOT, cgroup_path, "cgroup.freeze", عدم);
    }
  אחרת
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ROOT "/freezer", cgroup_path, "freezer.state", عدم);
    }
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = read_all_file (path, &content, عدم, err);
  אם (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      אם (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          החזר 0;
        }
      החזר ret;
    }

  אם (strstr (content, frozen))
    החזר crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  החזר 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
ساكن صحيح
watch_cgroup_events (ثابت حرف *path)
{
  cleanup_free حرف *events_file = عدم;
  cleanup_close صحيح fd = -1;
  libcrun_error_t tmp_err = عدم;
  صحيح ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", عدم);
  אם (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      החזר -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  אם (UNLIKELY (fd < 0))
    החזר -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  אם (UNLIKELY (ret < 0))
    החזר -1;

  ret = fd;
  fd = -1;
  החזר ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
ساكن صحيح
wait_cgroup_unpopulated (ثابت حرف *path, صحيح inotify_fd)
{
  cleanup_free حرف *events_file = عدم;
  libcrun_error_t tmp_err = عدم;
  بنية timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  صحيح ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ROOT, path, "cgroup.events", عدم);
  אם (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      החזר 0;
    }

  עבור (;;)
    {
      cleanup_free حرف *content = عدم;
      حرف buf[4096];
      بنية pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, عدم, &tmp_err);
      אם (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          החזר 0;
        }

      אם (strstr (content, "populated 0"))
        החזר 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, عدم));
      אם (ret <= 0)
        החזר 0;

      כלעוד (اقرأ (inotify_fd, buf, حجم (buf)) > 0)
        ;
    }
}

صحيح
cgroup_killall_path (ثابت حرف *path, صحيح إشارة, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close صحيح inotify_fd = -1;
  صحيح ret;
  size_t i;
  cleanup_free pid_t *pids = عدم;

  אם (path == عدم || *path == '\0')
    החזר 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  אם (إشارة == SIGKILL)
    {
      cleanup_free حرف *kill_file = عدم;

      ret = append_paths (&kill_file, err, CGROUP_ROOT, path, "cgroup.kill", عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      אם (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        החזר 0;

      אם (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  אם (UNLIKELY (ret < 0))
    crun_error_release (err);
  אחרת
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, إشارة == SIGKILL, &pids, err);
  אם (UNLIKELY (ret < 0))
    {
      אם (crun_error_get_errno (err) != ENOENT)
        اذهب thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  עבור (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], إشارة);
      אם (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          اذهب thaw;
        }
    }

  ret = 0;

thaw:
  אם (frozen)
    {
      libcrun_error_t thaw_err = عدم;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      אם (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  החזר ret;
}

ساكن صحيح
read_available_controllers (ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *controllers = عدم;
  cleanup_free حرف *buf = عدم;
  حرف *saveptr = عدم;
  ثابت حرف *token;
  صحيح available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ROOT, path, "cgroup.controllers", عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = read_all_file (controllers, &buf, عدم, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  עבור (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (عدم, " \n", &saveptr))
    {
      אם (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      אחרת אם (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      אחרת אם (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      אחרת אם (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      אחרת אם (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      אחרת אם (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      אחרת אם (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  החזר available;
}

ساكن صحيح
write_controller_file (ثابت حرف *path, صحيح controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free حرف *subtree_control = عدم;
  cleanup_free حرف *controllers = عدم;
  size_t controllers_len = 0;
  صحيح ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ROOT, path, "cgroup.subtree_control", عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  אם (UNLIKELY (ret < 0))
    {
      cleanup_free حرف *controllers_copy = xmalloc (controllers_len + 1);
      صحيح attempts_left;
      حرف *saveptr = عدم;
      ثابت حرف *token;
      صحيح e;

      e = crun_error_get_errno (err);
      אם (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        החזר crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      אם (e == ENOENT)
        {
          libcrun_error_t tmp_err = عدم;
          صحيح exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          אם (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              החזר ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          אם (exists == 0)
            החזר ret;
        }

      crun_error_release (err);

      אם (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          אם (UNLIKELY (ret < 0))
            החזר crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      עבור (attempts_left = 1000; attempts_left >= 0; attempts_left--)
        {
          صحيح controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          עבור (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (عدم, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              אם (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  אם (e == EBUSY)
                    repeat = true;

                  استمر;
                }

              controllers_written++;
            }

          אם (! repeat)
            اكسر;

          /* If there was any controller written, just try once more without any delay.  */
          אם (controllers_written > 0 && attempts_left > 2)
            {
              attempts_left = 1;
              استمر;
            }

          אם (attempts_left > 0)
            {
              بنية timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, عدم);
            }
        }

      /* Refresh what controllers are available.  */
      החזר read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  החזר controllers_to_enable;
}

صحيح
enable_controllers (ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *tmp_path = عدم;
  حرف *it;
  صحيح ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  אם (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      אם ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  עבור (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free حرف *cgroup_path = عدم;
      حرف *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ROOT, tmp_path, عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = mkdir (cgroup_path, 0755);
      אם (UNLIKELY (ret < 0 && errno != EEXIST))
        החזר crun_make_error (err, errno, "create `%s`", cgroup_path);

      אם (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }

      *it = '/';
      it = next_slash;
    }
  החזר 0;
}

صحيح
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ثابت حرف *path, bool create_if_missing, libcrun_error_t *err)
{
  صحيح cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  אם (path == عدم || *path == '\0')
    החזר 0;

  החזר enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
صحيح
libcrun_move_self_to_cgroups (ثابت حرف *content, libcrun_error_t *err)
{
  cleanup_free حرف *buf = xstrdup (content);
  حرف *saveptr = عدم;
  حرف *controller;
  حرف *path;
  صحيح cgroup_mode;
  bool has_data;
  صحيح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  עבור (has_data = read_proc_cgroup (buf, &saveptr, عدم, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (عدم, &saveptr, عدم, &controller, &path))
    {
      ثابت حرف *subsystem = عدم;

      אם (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          אם (controller[0] != '\0')
            استمر;
        }
      אחרת
        {
          cleanup_free حرف *subsystem_path = عدم;

          /* Use the same names as enter_cgroup_v1 does.  */
          אם (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          אם (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          אם (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ROOT, subsystem, عدم);
          אם (UNLIKELY (ret < 0))
            החזר ret;
          ret = crun_path_exists (subsystem_path, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
          אם (ret == 0)
            استمر;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  החזר 0;
}

صحيح
libcrun_get_cgroup_dirfd (بنية libcrun_cgroup_status *status, ثابت حرف *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free حرف *path_to_cgroup = عدم;
  صحيح cgroup_mode;
  صحيح cgroupdirfd;
  صحيح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (cgroup_mode < 0)
    החזר cgroup_mode;

  אם (cgroup_mode != CGROUP_MODE_UNIFIED)
    החזר crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  אם (status == عدم)
    החזר crun_make_error (err, 0, "internal error");

  אם (is_empty_string (status->path))
    החזר crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ROOT, status->path, sub_cgroup, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  אם (UNLIKELY (cgroupdirfd < 0))
    החזר crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  החזר cgroupdirfd;
}

صحيح
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, حرف *from, حرف *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = عدم;
  cleanup_close صحيح child_dfd = -1;
  صحيح cgroup_mode;
  size_t from_len;
  size_t i;
  صحيح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (cgroup_mode < 0)
    החזר cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  אם (UNLIKELY (ret < 0))
    اذهب thaw;

  from_len = strlen (from);

  עבור (i = 0; pids && pids[i]; i++)
    {
      cleanup_free حرف *pid_path = عدم;
      cleanup_free حرف *dest_cgroup = عدم;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      אם (UNLIKELY (ret < 0))
        اذهب thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      אם (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          اذهب thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      אם (UNLIKELY (ret < 0))
        اذهب thaw;
    }

  ret = 0;

thaw:
  אם (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = عدم;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      אם (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      החזר ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר destroy_cgroup_path (from, cgroup_mode, err);
}

صحيح
get_cgroup_dirfd_path (صحيح dirfd, حرف **path, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path = عدم;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  אם (UNLIKELY (len < 0))
    החזר len;

  אם (has_prefix (cgroup_path, CGROUP_ROOT))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ROOT));
      החזר 0;
    }
  החזר crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
صحيح
parse_cpu_max (ثابت حرف *value, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ثابت حرف *it = value;
  حرف *endptr = عدم;

  כלעוד (*it == ' ')
    it++;

  errno = 0;
  אם (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (حرف *) it + 3;
    }
  אחרת
    {
      *quota = strtoll (it, &endptr, 10);
      אם (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        החזר crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
    }

  it = endptr;
  כלעוד (*it == ' ')
    it++;

  *has_period = false;
  אם (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      אם (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        החזר crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", value);
      *has_period = true;
    }

  החזר 0;
}
