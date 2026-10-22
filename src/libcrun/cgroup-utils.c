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

#شامل <config.h>
#شامل "cgroup.h"
#شامل "cgroup-internal.h"
#شامل "cgroup-systemd.h"
#شامل "cgroup-utils.h"
#شامل "cgroup-setup.h"
#شامل "ebpf.h"
#شامل "utils.h"
#شامل "status.h"
#شامل <string.h>
#شامل <sys/types.h>
#شامل <signal.h>
#شامل <sys/vfs.h>
#شامل <inttypes.h>
#شامل <time.h>
#شامل <poll.h>
#شامل <sys/inotify.h>

#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <libgen.h>

ساختار symlink_s
{
  ثابت نویسه *name;
  ثابت نویسه *target;
};

ایستا ساختار symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { پوچ, پوچ } };

صحیح
libcrun_cgroups_create_symlinks (صحیح dirfd, libcrun_error_t *err)
{
  صحیح i;

  برای (i = 0; cgroup_symlinks[i].name; i++)
    {
      صحیح ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (errno == ENOENT || errno == EEXIST)
            ادامه;
          بازگشت crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  بازگشت 0;
}

صحیح
maybe_make_cgroup_threaded (ثابت نویسه *path, libcrun_error_t *err)
{
  cleanup_free نویسه *cgroup_path_type = پوچ;
  ثابت نویسه *ثابت threaded = "threaded";
  cleanup_free نویسه *content = پوچ;
  cleanup_free نویسه *buffer = پوچ;
  ثابت نویسه *parent;
  size_t size;
  نویسه *it;
  صحیح ret;

  اگر (path == پوچ)
    بازگشت 0;

  path = consume_slashes (path);

  اگر (path[0] == '\0')
    بازگشت 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_ریشه, path, "cgroup.type", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (size > 0)
    {
      it = content + size - 1;
      وقتی (*it == '\n' && it > content)
        *it-- = '\0';
    }

  اگر (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    بازگشت 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  اگر (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      اگر (ret < 0)
        بازگشت ret;
    }
  بازگشت write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

صحیح
move_process_to_cgroup (pid_t pid, ثابت نویسه *subsystem, ثابت نویسه *path, libcrun_error_t *err)
{
  cleanup_free نویسه *cgroup_path_procs = پوچ;
  نویسه pid_str[16];
  صحیح ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_ریشه,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = snprintf (pid_str, اندازه (pid_str), "%d", pid);
  اگر (UNLIKELY (ret >= (صحیح) اندازه (pid_str)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = پوچ;
          صحیح mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          اگر (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              بازگشت ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          بازگشت write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      بازگشت ret;
    }
  بازگشت ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

ایستا صحیح
detect_cgroup_mode (libcrun_error_t *err)
{
  ساختار statfs stat;
  صحیح ret;

  ret = statfs (CGROUP_ریشه, &stat);
  اگر (ret < 0)
    بازگشت crun_make_error (err, errno, "statfs `" CGROUP_ریشه "`");
  اگر (stat.f_type == CGROUP2_SUPER_MAGIC)
    بازگشت CGROUP_MODE_UNIFIED;
  اگر (stat.f_type != TMPFS_MAGIC)
    بازگشت crun_make_error (err, 0, "invalid file system type on `" CGROUP_ریشه "`");
  ret = statfs (CGROUP_ریشه "/unified", &stat);
  اگر (ret < 0 && errno != ENOENT)
    بازگشت crun_make_error (err, errno, "statfs `" CGROUP_ریشه "/unified`");
  اگر (ret < 0)
    بازگشت CGROUP_MODE_LEGACY;
  بازگشت stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

صحیح
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  صحیح tmp;
  ایستا صحیح cgroup_mode = 0;

  اگر (cgroup_mode)
    بازگشت cgroup_mode;

  tmp = detect_cgroup_mode (err);
  اگر (UNLIKELY (tmp < 0))
    بازگشت tmp;

  cgroup_mode = tmp;

  بازگشت cgroup_mode;
}

ایستا صحیح
get_cgroup_process_from_content (نویسه *content, صحیح cgroup_mode, نویسه **path, bool absolute, libcrun_error_t *err)
{
  نویسه *cg_path = پوچ;
  نویسه *controller;
  نویسه *saveptr;
  bool has_data;

  برای (has_data = read_proc_cgroup (content, &saveptr, پوچ, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (پوچ, &saveptr, پوچ, &controller, &cg_path))
    {
      اگر (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          اگر (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            برو found;
        }
      وگرنه
        {
          اگر (strcmp (controller, "memory"))
            برو found;
        }
    }

  بازگشت crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  اگر (absolute)
    بازگشت append_paths (path, err, CGROUP_ریشه, cg_path, پوچ);

  *path = xstrdup (cg_path);
  بازگشت 0;
}

صحیح
libcrun_get_cgroup_process_at (صحیح dirfd, نویسه **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free نویسه *content = پوچ;
  size_t content_size;
  صحیح cgroup_mode;
  صحیح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

صحیح
libcrun_get_cgroup_process (pid_t pid, نویسه **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free نویسه *content = پوچ;
  نویسه proc_cgroup_file[64];
  size_t content_size;
  صحیح cgroup_mode;
  صحیح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  اگر (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  وگرنه
    {
      صحیح len = snprintf (proc_cgroup_file, اندازه (proc_cgroup_file), "/proc/%d/cgroup", pid);
      اگر (UNLIKELY (len >= (صحیح) اندازه (proc_cgroup_file)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

ایستا صحیح
read_pids_file (صحیح dfd, ثابت نویسه *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close صحیح tasksfd = -1;
  cleanup_free نویسه *buffer = پوچ;
  نویسه *saveptr = پوچ;
  size_t n_new_pids;
  size_t len;
  نویسه *it;
  صحیح ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  اگر (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      اگر (errno == ENOENT || errno == ENODEV)
        بازگشت 0;
      بازگشت crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (len > 0)
    {
      برای (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      اگر (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, اندازه (pid_t) * *allocated);
        }

      برای (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (پوچ, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, پوچ, 10);

          اگر (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  بازگشت 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
ساختار cgroup_walk_dir_s
{
  نویسه *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
ساختار cgroup_walk_s
{
  صحیح dfd;
  size_t depth;
  ساختار cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

ایستا تهی
cgroup_walk_برداشت (ساختار cgroup_walk_s *walk)
{
  آزاد (walk->dirs[--walk->n_dirs].name);
}

ایستا تهی
cleanup_cgroup_walkp (ساختار cgroup_walk_s *walk)
{
  صحیح saved_errno = errno;

  وقتی (walk->n_dirs > 0)
    cgroup_walk_برداشت (walk);
  آزاد (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
ایستا صحیح
cgroup_walk_enter (ساختار cgroup_walk_s *walk, صحیح dfd)
{
  cleanup_close صحیح dfd_clone = -1;
  cleanup_dir DIR *dir = پوچ;
  ساختار dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (dfd_clone < 0))
    بازگشت -1;

  dir = fdopendir (dfd_clone);
  اگر (UNLIKELY (dir == پوچ))
    بازگشت -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  برای (de = readdir (dir); de; de = readdir (dir))
    {
      ساختار cgroup_walk_dir_s *d;

      اگر (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ادامه;

      اگر (de->d_type != DT_DIR)
        ادامه;

      اگر (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, اندازه (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  بازگشت 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
ایستا صحیح
cgroup_walk_next (ساختار cgroup_walk_s *walk, ساختار cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : پوچ;

  وقتی (*d && walk->depth >= (*d)->depth)
    {
      صحیح parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      اگر (UNLIKELY (parent < 0))
        بازگشت -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  بازگشت 0;
}

ایستا صحیح
read_pids_cgroup_file (صحیح dfd, ثابت نویسه *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk ساختار cgroup_walk_s walk = { .dfd = -1 };
  صحیح dfd_clone;
  صحیح ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (! recurse)
    بازگشت 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (dfd_clone < 0))
    بازگشت crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "open cgroup sub-directory");

  برای (;;)
    {
      ساختار cgroup_walk_dir_s *d;
      cleanup_close صحیح nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "open parent cgroup directory");
      اگر (d == پوچ)
        بشکن;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      اگر (UNLIKELY (nfd < 0))
        {
          صحیح saved_errno = errno;

          اگر (saved_errno == ENOENT)
            {
              cgroup_walk_برداشت (&walk);
              ادامه;
            }
          بازگشت crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_برداشت (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      اگر (UNLIKELY (ret < 0))
        {
          صحیح e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          اگر (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            بازگشت ret;
          crun_error_release (err);
          ادامه;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      اگر (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        بازگشت crun_make_error (err, errno, "open cgroup sub-directory");
    }

  بازگشت 0;
}

ایستا صحیح
read_pids_cgroup (صحیح dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  صحیح ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  اگر (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  بازگشت ret;
}

ایستا صحیح
rmdir_all_fd (صحیح dfd)
{
  cleanup_cgroup_walk ساختار cgroup_walk_s walk = { .dfd = -1 };
  صحیح ret;

  ret = cgroup_walk_enter (&walk, dfd);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  برای (;;)
    {
      ساختار cgroup_walk_dir_s *d;
      صحیح child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      اگر (d == پوچ)
        بشکن;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      اگر (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_برداشت (&walk);
          ادامه;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      اگر (UNLIKELY (child_dfd < 0))
        {
          اگر (errno == ENOENT)
            {
              cgroup_walk_برداشت (&walk);
              ادامه;
            }
          بازگشت child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      اگر (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        بازگشت ret;
    }

  بازگشت 0;
}

ایستا صحیح
rmdir_all (ثابت نویسه *path)
{
  صحیح ret;
  صحیح dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (dfd < 0))
    بازگشت dfd;

  ret = rmdir_all_fd (dfd);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت rmdir (path);
}

ایستا صحیح
read_pids_from_path (ثابت نویسه *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free نویسه *cgroup_path = پوچ;
  cleanup_close صحیح dirfd = -1;
  size_t n_pids, allocated;
  صحیح mode;
  صحیح ret;

  اگر (path == پوچ || *path == '\0')
    بازگشت 0;

  mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (mode < 0))
    بازگشت mode;

  انتخاب (mode)
    {
    حالت CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_ریشه, path, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      بشکن;

    حالت CGROUP_MODE_HYBRID:
    حالت CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_ریشه "/memory", path, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      بشکن;

    پیشفرض:
      بازگشت crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (dirfd < 0)
    بازگشت crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  اگر (threads && mode == CGROUP_MODE_UNIFIED)
    بازگشت read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  بازگشت read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

صحیح
libcrun_cgroup_read_pids_from_path (ثابت نویسه *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  بازگشت read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (نویسه *content, نویسه **saveptr, نویسه **id, نویسه **controller_list, نویسه **path)
{
  نویسه *it;

  it = strtok_r (content, "\n", saveptr);
  اگر (it == پوچ)
    بازگشت false;

  اگر (id)
    *id = it;

  it = strchr (it, ':');
  اگر (it == پوچ)
    بازگشت false;
  *it++ = '\0';

  اگر (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  اگر (it == پوچ)
    بازگشت false;
  *it++ = '\0';

  اگر (path)
    *path = it;

  بازگشت true;
}

صحیح
destroy_cgroup_path (ثابت نویسه *path, صحیح mode, libcrun_error_t *err)
{
  bool repeat = true;
  صحیح retry_شمار = 0;
  ثابت صحیح max_attempts = 500;
  صحیح ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  اگر (is_empty_string (path))
    بازگشت 0;

  انجامبده
    {
      repeat = false;

      اگر (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free نویسه *cgroup_path = پوچ;

          ret = append_paths (&cgroup_path, err, CGROUP_ریشه, path, پوچ);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          ret = rmdir (cgroup_path);
          اگر (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              اگر (ret < 0)
                {
                  اگر (retry_شمار >= max_attempts)
                    بازگشت crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_شمار++;
                  repeat = true;
                }
            }
        }
      وگرنه
        {
          cleanup_free نویسه *content = پوچ;
          size_t content_size;
          نویسه *controller;
          نویسه *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  بازگشت 0;
                }
              بازگشت ret;
            }

          برای (has_data = read_proc_cgroup (content, &saveptr, پوچ, &controller, پوچ);
               has_data;
               has_data = read_proc_cgroup (پوچ, &saveptr, پوچ, &controller, پوچ))
            {
              cleanup_free نویسه *cgroup_path = پوچ;
              نویسه *subsystem;
              اگر (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              اگر (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                ادامه;

              ret = append_paths (&cgroup_path, err, CGROUP_ریشه, subsystem, path, پوچ);
              اگر (UNLIKELY (ret < 0))
                بازگشت ret;

              ret = rmdir (cgroup_path);
              اگر (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  اگر (ret < 0)
                    {
                      اگر (retry_شمار >= max_attempts)
                        بازگشت crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_شمار++;
                      repeat = true;
                    }
                }
            }
        }

      اگر (repeat)
        {
          ساختار timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, پوچ);

          ret = cgroup_killall_path (path, SIGKILL, err);
          اگر (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } وقتی (repeat);

  بازگشت 0;
}

صحیح
chown_cgroups (ثابت نویسه *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free نویسه *cgroup_path = پوچ;
  cleanup_free نویسه *delegate = پوچ;
  cleanup_close صحیح dfd = -1;
  size_t delegate_size;
  نویسه *saveptr = پوچ;
  نویسه *name;
  صحیح ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ریشه, path, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  اگر (UNLIKELY (dfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          بازگشت 0;
        }
      بازگشت ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  برای (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (پوچ, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (errno == ENOENT)
            ادامه;

          بازگشت crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  بازگشت 0;
}

ایستا صحیح
libcrun_cgroup_pause_unpause_with_mode (ثابت نویسه *cgroup_path, صحیح cgroup_mode, ثابت bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free نویسه *path = پوچ;
  ثابت نویسه *state = "";
  صحیح ret;

  اگر (cgroup_path == پوچ || cgroup_path[0] == '\0')
    بازگشت crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  اگر (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_ریشه, cgroup_path, "cgroup.freeze", پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_ریشه "/freezer", cgroup_path, "freezer.state", پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  اگر (ret >= 0)
    بازگشت 0;
  بازگشت ret;
}

صحیح
libcrun_cgroup_pause_unpause_path (ثابت نویسه *cgroup_path, ثابت bool pause, libcrun_error_t *err)
{
  صحیح cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  بازگشت libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
صحیح
libcrun_cgroup_ensure_not_frozen (ثابت نویسه *cgroup_path, libcrun_error_t *err)
{
  cleanup_free نویسه *content = پوچ;
  cleanup_free نویسه *path = پوچ;
  ثابت نویسه *frozen;
  صحیح cgroup_mode;
  صحیح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  اگر (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_ریشه, cgroup_path, "cgroup.freeze", پوچ);
    }
  وگرنه
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_ریشه "/freezer", cgroup_path, "freezer.state", پوچ);
    }
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = read_all_file (path, &content, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          بازگشت 0;
        }
      بازگشت ret;
    }

  اگر (strstr (content, frozen))
    بازگشت crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  بازگشت 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
ایستا صحیح
watch_cgroup_events (ثابت نویسه *path)
{
  cleanup_free نویسه *events_file = پوچ;
  cleanup_close صحیح fd = -1;
  libcrun_error_t tmp_err = پوچ;
  صحیح ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ریشه, path, "cgroup.events", پوچ);
  اگر (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      بازگشت -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  اگر (UNLIKELY (fd < 0))
    بازگشت -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  اگر (UNLIKELY (ret < 0))
    بازگشت -1;

  ret = fd;
  fd = -1;
  بازگشت ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
ایستا صحیح
wait_cgroup_unpopulated (ثابت نویسه *path, صحیح inotify_fd)
{
  cleanup_free نویسه *events_file = پوچ;
  libcrun_error_t tmp_err = پوچ;
  ساختار timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  صحیح ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ریشه, path, "cgroup.events", پوچ);
  اگر (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      بازگشت 0;
    }

  برای (;;)
    {
      cleanup_free نویسه *content = پوچ;
      نویسه buf[4096];
      ساختار pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, پوچ, &tmp_err);
      اگر (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          بازگشت 0;
        }

      اگر (strstr (content, "populated 0"))
        بازگشت 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, پوچ));
      اگر (ret <= 0)
        بازگشت 0;

      وقتی (اقرأ (inotify_fd, buf, اندازه (buf)) > 0)
        ;
    }
}

صحیح
cgroup_killall_path (ثابت نویسه *path, صحیح سیگنال, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close صحیح inotify_fd = -1;
  صحیح ret;
  size_t i;
  cleanup_free pid_t *pids = پوچ;

  اگر (path == پوچ || *path == '\0')
    بازگشت 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  اگر (سیگنال == SIGKILL)
    {
      cleanup_free نویسه *kill_file = پوچ;

      ret = append_paths (&kill_file, err, CGROUP_ریشه, path, "cgroup.kill", پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      اگر (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        بازگشت 0;

      اگر (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);
  وگرنه
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, سیگنال == SIGKILL, &pids, err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (crun_error_get_errno (err) != ENOENT)
        برو thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  برای (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], سیگنال);
      اگر (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          برو thaw;
        }
    }

  ret = 0;

thaw:
  اگر (frozen)
    {
      libcrun_error_t thaw_err = پوچ;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      اگر (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  بازگشت ret;
}

ایستا صحیح
read_available_controllers (ثابت نویسه *path, libcrun_error_t *err)
{
  cleanup_free نویسه *controllers = پوچ;
  cleanup_free نویسه *buf = پوچ;
  نویسه *saveptr = پوچ;
  ثابت نویسه *token;
  صحیح available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ریشه, path, "cgroup.controllers", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = read_all_file (controllers, &buf, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  برای (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (پوچ, " \n", &saveptr))
    {
      اگر (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      وگرنه اگر (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      وگرنه اگر (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      وگرنه اگر (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      وگرنه اگر (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      وگرنه اگر (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      وگرنه اگر (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  بازگشت available;
}

ایستا صحیح
write_controller_file (ثابت نویسه *path, صحیح controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free نویسه *subtree_control = پوچ;
  cleanup_free نویسه *controllers = پوچ;
  size_t controllers_len = 0;
  صحیح ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ریشه, path, "cgroup.subtree_control", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  اگر (UNLIKELY (ret < 0))
    {
      cleanup_free نویسه *controllers_copy = xmalloc (controllers_len + 1);
      صحیح attempts_چپ;
      نویسه *saveptr = پوچ;
      ثابت نویسه *token;
      صحیح e;

      e = crun_error_get_errno (err);
      اگر (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        بازگشت crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      اگر (e == ENOENT)
        {
          libcrun_error_t tmp_err = پوچ;
          صحیح exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          اگر (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              بازگشت ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          اگر (exists == 0)
            بازگشت ret;
        }

      crun_error_release (err);

      اگر (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      برای (attempts_چپ = 1000; attempts_چپ >= 0; attempts_چپ--)
        {
          صحیح controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          برای (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (پوچ, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              اگر (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  اگر (e == EBUSY)
                    repeat = true;

                  ادامه;
                }

              controllers_written++;
            }

          اگر (! repeat)
            بشکن;

          /* If there was any controller written, just try once more without any delay.  */
          اگر (controllers_written > 0 && attempts_چپ > 2)
            {
              attempts_چپ = 1;
              ادامه;
            }

          اگر (attempts_چپ > 0)
            {
              ساختار timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, پوچ);
            }
        }

      /* Refresh what controllers are available.  */
      بازگشت read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  بازگشت controllers_to_enable;
}

صحیح
enable_controllers (ثابت نویسه *path, libcrun_error_t *err)
{
  cleanup_free نویسه *tmp_path = پوچ;
  نویسه *it;
  صحیح ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  اگر (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      اگر ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  برای (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free نویسه *cgroup_path = پوچ;
      نویسه *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_ریشه, tmp_path, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = mkdir (cgroup_path, 0755);
      اگر (UNLIKELY (ret < 0 && errno != EEXIST))
        بازگشت crun_make_error (err, errno, "create `%s`", cgroup_path);

      اگر (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      *it = '/';
      it = next_slash;
    }
  بازگشت 0;
}

صحیح
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ثابت نویسه *path, bool create_if_missing, libcrun_error_t *err)
{
  صحیح cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  اگر (path == پوچ || *path == '\0')
    بازگشت 0;

  بازگشت enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
صحیح
libcrun_move_self_to_cgroups (ثابت نویسه *content, libcrun_error_t *err)
{
  cleanup_free نویسه *buf = xstrdup (content);
  نویسه *saveptr = پوچ;
  نویسه *controller;
  نویسه *path;
  صحیح cgroup_mode;
  bool has_data;
  صحیح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  برای (has_data = read_proc_cgroup (buf, &saveptr, پوچ, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (پوچ, &saveptr, پوچ, &controller, &path))
    {
      ثابت نویسه *subsystem = پوچ;

      اگر (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          اگر (controller[0] != '\0')
            ادامه;
        }
      وگرنه
        {
          cleanup_free نویسه *subsystem_path = پوچ;

          /* Use the same names as enter_cgroup_v1 does.  */
          اگر (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          اگر (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          اگر (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_ریشه, subsystem, پوچ);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          ret = crun_path_exists (subsystem_path, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          اگر (ret == 0)
            ادامه;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

صحیح
libcrun_get_cgroup_dirfd (ساختار libcrun_cgroup_status *status, ثابت نویسه *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free نویسه *path_to_cgroup = پوچ;
  صحیح cgroup_mode;
  صحیح cgroupdirfd;
  صحیح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (cgroup_mode < 0)
    بازگشت cgroup_mode;

  اگر (cgroup_mode != CGROUP_MODE_UNIFIED)
    بازگشت crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  اگر (status == پوچ)
    بازگشت crun_make_error (err, 0, "internal error");

  اگر (is_empty_string (status->path))
    بازگشت crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_ریشه, status->path, sub_cgroup, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  اگر (UNLIKELY (cgroupdirfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  بازگشت cgroupdirfd;
}

صحیح
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, نویسه *from, نویسه *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = پوچ;
  cleanup_close صحیح child_dfd = -1;
  صحیح cgroup_mode;
  size_t from_len;
  size_t i;
  صحیح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (cgroup_mode < 0)
    بازگشت cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  اگر (UNLIKELY (ret < 0))
    برو thaw;

  from_len = strlen (from);

  برای (i = 0; pids && pids[i]; i++)
    {
      cleanup_free نویسه *pid_path = پوچ;
      cleanup_free نویسه *dest_cgroup = پوچ;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      اگر (UNLIKELY (ret < 0))
        برو thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      اگر (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          برو thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      اگر (UNLIKELY (ret < 0))
        برو thaw;
    }

  ret = 0;

thaw:
  اگر (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = پوچ;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      اگر (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      بازگشت ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت destroy_cgroup_path (from, cgroup_mode, err);
}

صحیح
get_cgroup_dirfd_path (صحیح dirfd, نویسه **path, libcrun_error_t *err)
{
  cleanup_free نویسه *cgroup_path = پوچ;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  اگر (UNLIKELY (len < 0))
    بازگشت len;

  اگر (has_prefix (cgroup_path, CGROUP_ریشه))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_ریشه));
      بازگشت 0;
    }
  بازگشت crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
صحیح
parse_cpu_max (ثابت نویسه *مقدار, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ثابت نویسه *it = مقدار;
  نویسه *endptr = پوچ;

  وقتی (*it == ' ')
    it++;

  errno = 0;
  اگر (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (نویسه *) it + 3;
    }
  وگرنه
    {
      *quota = strtoll (it, &endptr, 10);
      اگر (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        بازگشت crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", مقدار);
    }

  it = endptr;
  وقتی (*it == ' ')
    it++;

  *has_period = false;
  اگر (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      اگر (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        بازگشت crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", مقدار);
      *has_period = true;
    }

  بازگشت 0;
}
