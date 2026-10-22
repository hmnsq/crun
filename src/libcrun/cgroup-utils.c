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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-systemd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ebpf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/vfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <poll.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/inotify.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <libgen.h>

ਬਣਤਰ symlink_s
{
  ਅਚਲ ਅੱਖਰ *name;
  ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ;
};

ਸਥਿਰ ਬਣਤਰ symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { ਨਿਰਾ, ਨਿਰਾ } };

ਪੂਰਨ
libcrun_cgroups_create_symlinks (ਪੂਰਨ dirfd, libcrun_error_t *err)
{
  ਪੂਰਨ i;

  ਲਈ (i = 0; cgroup_symlinks[i].name; i++)
    {
      ਪੂਰਨ ret;

      ret = symlinkat (cgroup_symlinks[i].ਨਿਸ਼ਾਨਾ, dirfd, cgroup_symlinks[i].name);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (errno == ENOENT || errno == EEXIST)
            ਜਾਰੀ;
          ਵਾਪਸ crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  ਵਾਪਸ 0;
}

ਪੂਰਨ
maybe_make_cgroup_threaded (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ_type = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *ਅਚਲ threaded = "threaded";
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *ਮਾਪਾ_2;
  size_t ਮਾਪ;
  ਅੱਖਰ *it;
  ਪੂਰਨ ret;

  ਜੇ (ਰਸਤਾ == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਰਸਤਾ = consume_slashes (ਰਸਤਾ);

  ਜੇ (ਰਸਤਾ[0] == '\0')
    ਵਾਪਸ 0;

  ret = append_paths (&cgroup_ਰਸਤਾ_type, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, "cgroup.type", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = read_all_file (cgroup_ਰਸਤਾ_type, &content, &ਮਾਪ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ਮਾਪ > 0)
    {
      it = content + ਮਾਪ - 1;
      ਜਦੋਂ (*it == '\n' && it > content)
        *it-- = '\0';
    }

  ਜੇ (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    ਵਾਪਸ 0;

  ਬਫਰ = xstrdup (ਰਸਤਾ);
  ਮਾਪਾ_2 = consume_slashes (dirname (ਬਫਰ));
  ਜੇ (ਮਾਪਾ_2[0] && strcmp (ਮਾਪਾ_2, "."))
    {
      ret = maybe_make_cgroup_threaded (ਮਾਪਾ_2, err);
      ਜੇ (ret < 0)
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ write_cgroup_file (AT_FDCWD, cgroup_ਰਸਤਾ_type, threaded, ਲੜੀ_ਲੰਬਾਈ (threaded), err);
}

ਪੂਰਨ
move_process_ਮੰਜ਼ਿਲ_cgroup (pid_t pid, ਅਚਲ ਅੱਖਰ *subsystem, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ_procs = ਨਿਰਾ;
  ਅੱਖਰ pid_str[16];
  ਪੂਰਨ ret;

  ret = append_paths (&cgroup_ਰਸਤਾ_procs, err, CGROUP_ਜੜ੍ਹ,
                      subsystem ? subsystem : "", ਰਸਤਾ ? ਰਸਤਾ : "",
                      "cgroup.procs", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = snprintf (pid_str, ਆਕਾਰ (pid_str), "%d", pid);
  ਜੇ (UNLIKELY (ret >= (ਪੂਰਨ) ਆਕਾਰ (pid_str)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_ਰਸਤਾ_procs, pid_str, ਲੜੀ_ਲੰਬਾਈ (pid_str), err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = ਨਿਰਾ;
          ਪੂਰਨ ਬਹੁਲਕ;

          ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (&tmp_err);
          ਜੇ (UNLIKELY (ਬਹੁਲਕ < 0 || ਬਹੁਲਕ != CGROUP_ਬਹੁਲਕ_UNIFIED))
            {
              crun_error_release (&tmp_err);
              ਵਾਪਸ ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (ਰਸਤਾ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          ਵਾਪਸ write_cgroup_file (AT_FDCWD, cgroup_ਰਸਤਾ_procs, pid_str, ਲੜੀ_ਲੰਬਾਈ (pid_str), err);
        }

      ਵਾਪਸ ret;
    }
  ਵਾਪਸ ret;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਪੂਰਨ
detect_cgroup_ਬਹੁਲਕ (libcrun_error_t *err)
{
  ਬਣਤਰ statfs stat;
  ਪੂਰਨ ret;

  ret = statfs (CGROUP_ਜੜ੍ਹ, &stat);
  ਜੇ (ret < 0)
    ਵਾਪਸ crun_make_error (err, errno, "statfs `" CGROUP_ਜੜ੍ਹ "`");
  ਜੇ (stat.f_type == CGROUP2_SUPER_MAGIC)
    ਵਾਪਸ CGROUP_ਬਹੁਲਕ_UNIFIED;
  ਜੇ (stat.f_type != TMPFS_MAGIC)
    ਵਾਪਸ crun_make_error (err, 0, "invalid file system type on `" CGROUP_ਜੜ੍ਹ "`");
  ret = statfs (CGROUP_ਜੜ੍ਹ "/unified", &stat);
  ਜੇ (ret < 0 && errno != ENOENT)
    ਵਾਪਸ crun_make_error (err, errno, "statfs `" CGROUP_ਜੜ੍ਹ "/unified`");
  ਜੇ (ret < 0)
    ਵਾਪਸ CGROUP_ਬਹੁਲਕ_LEGACY;
  ਵਾਪਸ stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_ਬਹੁਲਕ_HYBRID : CGROUP_ਬਹੁਲਕ_LEGACY;
}

ਪੂਰਨ
libcrun_get_cgroup_ਬਹੁਲਕ (libcrun_error_t *err)
{
  ਪੂਰਨ tmp;
  ਸਥਿਰ ਪੂਰਨ cgroup_ਬਹੁਲਕ = 0;

  ਜੇ (cgroup_ਬਹੁਲਕ)
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  tmp = detect_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (tmp < 0))
    ਵਾਪਸ tmp;

  cgroup_ਬਹੁਲਕ = tmp;

  ਵਾਪਸ cgroup_ਬਹੁਲਕ;
}

ਸਥਿਰ ਪੂਰਨ
get_cgroup_process_from_content (ਅੱਖਰ *content, ਪੂਰਨ cgroup_ਬਹੁਲਕ, ਅੱਖਰ **ਰਸਤਾ, bool absolute, libcrun_error_t *err)
{
  ਅੱਖਰ *cg_ਰਸਤਾ = ਨਿਰਾ;
  ਅੱਖਰ *controller;
  ਅੱਖਰ *saveptr;
  bool has_ਅੰਕੜੇ;

  ਲਈ (has_ਅੰਕੜੇ = read_proc_cgroup (content, &saveptr, ਨਿਰਾ, &controller, &cg_ਰਸਤਾ);
       has_ਅੰਕੜੇ;
       has_ਅੰਕੜੇ = read_proc_cgroup (ਨਿਰਾ, &saveptr, ਨਿਰਾ, &controller, &cg_ਰਸਤਾ))
    {
      ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
        {
          ਜੇ (strcmp (controller, "") == 0 && ਲੜੀ_ਲੰਬਾਈ (cg_ਰਸਤਾ) > 0)
            ਜਾਓ ਮਿਲਿਆ;
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (strcmp (controller, "memory"))
            ਜਾਓ ਮਿਲਿਆ;
        }
    }

  ਵਾਪਸ crun_make_error (err, 0, "cannot find cgroup for the process");

ਮਿਲਿਆ:
  ਜੇ (absolute)
    ਵਾਪਸ append_paths (ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, cg_ਰਸਤਾ, ਨਿਰਾ);

  *ਰਸਤਾ = xstrdup (cg_ਰਸਤਾ);
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_get_cgroup_process_at (ਪੂਰਨ dirfd, ਅੱਖਰ **ਰਸਤਾ, bool absolute, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  size_t content_ਮਾਪ;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_ਮਾਪ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ get_cgroup_process_from_content (content, cgroup_ਬਹੁਲਕ, ਰਸਤਾ, absolute, err);
}

ਪੂਰਨ
libcrun_get_cgroup_process (pid_t pid, ਅੱਖਰ **ਰਸਤਾ, bool absolute, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  ਅੱਖਰ proc_cgroup_file[64];
  size_t content_ਮਾਪ;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  ਨਹੀਂਤਾਂ
    {
      ਪੂਰਨ len = snprintf (proc_cgroup_file, ਆਕਾਰ (proc_cgroup_file), "/proc/%d/cgroup", pid);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (proc_cgroup_file)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_ਮਾਪ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ get_cgroup_process_from_content (content, cgroup_ਬਹੁਲਕ, ਰਸਤਾ, absolute, err);
}

ਸਥਿਰ ਪੂਰਨ
read_pids_file (ਪੂਰਨ dfd, ਅਚਲ ਅੱਖਰ *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ tasksfd = -1;
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  ਅੱਖਰ *saveptr = ਨਿਰਾ;
  size_t n_new_pids;
  size_t len;
  ਅੱਖਰ *it;
  ਪੂਰਨ ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  ਜੇ (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      ਜੇ (errno == ENOENT || errno == ENODEV)
        ਵਾਪਸ 0;
      ਵਾਪਸ crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &ਬਫਰ, &len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (len > 0)
    {
      ਲਈ (n_new_pids = 0, it = ਬਫਰ; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      ਜੇ (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, ਆਕਾਰ (pid_t) * *allocated);
        }

      ਲਈ (it = strtok_r (ਬਫਰ, "\n", &saveptr); it; it = strtok_r (ਨਿਰਾ, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, ਨਿਰਾ, 10);

          ਜੇ (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  ਵਾਪਸ 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
ਬਣਤਰ cgroup_walk_dir_s
{
  ਅੱਖਰ *name;
  size_t ਡੂੰਘਾਈ;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
ਬਣਤਰ cgroup_walk_s
{
  ਪੂਰਨ dfd;
  size_t ਡੂੰਘਾਈ;
  ਬਣਤਰ cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

ਸਥਿਰ ਖਾਲੀ
cgroup_walk_ਕੱਢੋ (ਬਣਤਰ cgroup_walk_s *walk)
{
  ਮੁਕਤ (walk->dirs[--walk->n_dirs].name);
}

ਸਥਿਰ ਖਾਲੀ
cleanup_cgroup_walkp (ਬਣਤਰ cgroup_walk_s *walk)
{
  ਪੂਰਨ saved_errno = errno;

  ਜਦੋਂ (walk->n_dirs > 0)
    cgroup_walk_ਕੱਢੋ (walk);
  ਮੁਕਤ (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
ਸਥਿਰ ਪੂਰਨ
cgroup_walk_enter (ਬਣਤਰ cgroup_walk_s *walk, ਪੂਰਨ dfd)
{
  cleanup_close ਪੂਰਨ dfd_clone = -1;
  cleanup_dir DIR *dir = ਨਿਰਾ;
  ਬਣਤਰ dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->ਡੂੰਘਾਈ++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  ਜੇ (UNLIKELY (dfd_clone < 0))
    ਵਾਪਸ -1;

  dir = fdopendir (dfd_clone);
  ਜੇ (UNLIKELY (dir == ਨਿਰਾ))
    ਵਾਪਸ -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  ਲਈ (de = readdir (dir); de; de = readdir (dir))
    {
      ਬਣਤਰ cgroup_walk_dir_s *d;

      ਜੇ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ਜਾਰੀ;

      ਜੇ (de->d_type != DT_DIR)
        ਜਾਰੀ;

      ਜੇ (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, ਆਕਾਰ (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->ਡੂੰਘਾਈ = walk->ਡੂੰਘਾਈ + 1;
      d->entered = false;
    }

  ਵਾਪਸ 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
ਸਥਿਰ ਪੂਰਨ
cgroup_walk_ਅਗਲਾ (ਬਣਤਰ cgroup_walk_s *walk, ਬਣਤਰ cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : ਨਿਰਾ;

  ਜਦੋਂ (*d && walk->ਡੂੰਘਾਈ >= (*d)->ਡੂੰਘਾਈ)
    {
      ਪੂਰਨ ਮਾਪਾ_2;

      ਮਾਪਾ_2 = openat (walk->dfd, "..", O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
      ਜੇ (UNLIKELY (ਮਾਪਾ_2 < 0))
        ਵਾਪਸ -1;

      close_and_reset (&walk->dfd);
      walk->dfd = ਮਾਪਾ_2;
      walk->ਡੂੰਘਾਈ--;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
read_pids_cgroup_file (ਪੂਰਨ dfd, ਅਚਲ ਅੱਖਰ *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk ਬਣਤਰ cgroup_walk_s walk = { .dfd = -1 };
  ਪੂਰਨ dfd_clone;
  ਪੂਰਨ ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (! recurse)
    ਵਾਪਸ 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (dfd_clone < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open cgroup sub-directory");

  ਲਈ (;;)
    {
      ਬਣਤਰ cgroup_walk_dir_s *d;
      cleanup_close ਪੂਰਨ nfd = -1;

      ret = cgroup_walk_ਅਗਲਾ (&walk, &d);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open parent cgroup directory");
      ਜੇ (d == ਨਿਰਾ)
        ਰੋਕੋ;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
      ਜੇ (UNLIKELY (nfd < 0))
        {
          ਪੂਰਨ saved_errno = errno;

          ਜੇ (saved_errno == ENOENT)
            {
              cgroup_walk_ਕੱਢੋ (&walk);
              ਜਾਰੀ;
            }
          ਵਾਪਸ crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_ਕੱਢੋ (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਪੂਰਨ e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          ਜੇ (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            ਵਾਪਸ ret;
          crun_error_release (err);
          ਜਾਰੀ;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      ਜੇ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ਵਾਪਸ crun_make_error (err, errno, "open cgroup sub-directory");
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
read_pids_cgroup (ਪੂਰਨ dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  ਜੇ (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
rmdir_all_fd (ਪੂਰਨ dfd)
{
  cleanup_cgroup_walk ਬਣਤਰ cgroup_walk_s walk = { .dfd = -1 };
  ਪੂਰਨ ret;

  ret = cgroup_walk_enter (&walk, dfd);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਲਈ (;;)
    {
      ਬਣਤਰ cgroup_walk_dir_s *d;
      ਪੂਰਨ ਬੱਚਾ_dfd;

      ret = cgroup_walk_ਅਗਲਾ (&walk, &d);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      ਜੇ (d == ਨਿਰਾ)
        ਰੋਕੋ;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      ਜੇ (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_ਕੱਢੋ (&walk);
          ਜਾਰੀ;
        }

      ਬੱਚਾ_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      ਜੇ (UNLIKELY (ਬੱਚਾ_dfd < 0))
        {
          ਜੇ (errno == ENOENT)
            {
              cgroup_walk_ਕੱਢੋ (&walk);
              ਜਾਰੀ;
            }
          ਵਾਪਸ ਬੱਚਾ_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, ਬੱਚਾ_dfd);
      ਜੇ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
rmdir_all (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
  ਪੂਰਨ ret;
  ਪੂਰਨ dfd = open (ਰਸਤਾ, O_DIRECTORY | O_CLOEXEC);
  ਜੇ (UNLIKELY (dfd < 0))
    ਵਾਪਸ dfd;

  ret = rmdir_all_fd (dfd);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ rmdir (ਰਸਤਾ);
}

ਸਥਿਰ ਪੂਰਨ
read_pids_from_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool recurse, bool ਧਾਗੇ, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ dirfd = -1;
  size_t n_pids, allocated;
  ਪੂਰਨ ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  ਜੇ (ਰਸਤਾ == ਨਿਰਾ || *ਰਸਤਾ == '\0')
    ਵਾਪਸ 0;

  ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (ਬਹੁਲਕ < 0))
    ਵਾਪਸ ਬਹੁਲਕ;

  ਚੋਣ (ਬਹੁਲਕ)
    {
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_UNIFIED:
      ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      ਰੋਕੋ;

    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_HYBRID:
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_LEGACY:
      ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ "/memory", ਰਸਤਾ, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      ਰੋਕੋ;

    ਮੂਲ:
      ਵਾਪਸ crun_make_error (err, 0, "invalid cgroup mode `%d`", ਬਹੁਲਕ);
    }

  dirfd = open (cgroup_ਰਸਤਾ, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (dirfd < 0)
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", cgroup_ਰਸਤਾ);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  ਜੇ (ਧਾਗੇ && ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
    ਵਾਪਸ read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  ਵਾਪਸ read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

ਪੂਰਨ
libcrun_cgroup_read_pids_from_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ਵਾਪਸ read_pids_from_ਰਸਤਾ (ਰਸਤਾ, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (ਅੱਖਰ *content, ਅੱਖਰ **saveptr, ਅੱਖਰ **id, ਅੱਖਰ **controller_list, ਅੱਖਰ **ਰਸਤਾ)
{
  ਅੱਖਰ *it;

  it = strtok_r (content, "\n", saveptr);
  ਜੇ (it == ਨਿਰਾ)
    ਵਾਪਸ false;

  ਜੇ (id)
    *id = it;

  it = strchr (it, ':');
  ਜੇ (it == ਨਿਰਾ)
    ਵਾਪਸ false;
  *it++ = '\0';

  ਜੇ (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  ਜੇ (it == ਨਿਰਾ)
    ਵਾਪਸ false;
  *it++ = '\0';

  ਜੇ (ਰਸਤਾ)
    *ਰਸਤਾ = it;

  ਵਾਪਸ true;
}

ਪੂਰਨ
destroy_cgroup_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ, libcrun_error_t *err)
{
  bool repeat = true;
  ਪੂਰਨ retry_ਗਿਣਤੀ = 0;
  ਅਚਲ ਪੂਰਨ max_attempts = 500;
  ਪੂਰਨ ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਰਸਤਾ))
    ਵਾਪਸ 0;

  ਕਰੋ
    {
      repeat = false;

      ਜੇ (ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
        {
          cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;

          ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, ਨਿਰਾ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          ret = rmdir (cgroup_ਰਸਤਾ);
          ਜੇ (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_ਰਸਤਾ);
              ਜੇ (ret < 0)
                {
                  ਜੇ (retry_ਗਿਣਤੀ >= max_attempts)
                    ਵਾਪਸ crun_make_error (err, errno, "cannot delete path `%s`", cgroup_ਰਸਤਾ);

                  retry_ਗਿਣਤੀ++;
                  repeat = true;
                }
            }
        }
      ਨਹੀਂਤਾਂ
        {
          cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
          size_t content_ਮਾਪ;
          ਅੱਖਰ *controller;
          ਅੱਖਰ *saveptr;
          bool has_ਅੰਕੜੇ;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_ਮਾਪ, err);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ਜੇ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  ਵਾਪਸ 0;
                }
              ਵਾਪਸ ret;
            }

          ਲਈ (has_ਅੰਕੜੇ = read_proc_cgroup (content, &saveptr, ਨਿਰਾ, &controller, ਨਿਰਾ);
               has_ਅੰਕੜੇ;
               has_ਅੰਕੜੇ = read_proc_cgroup (ਨਿਰਾ, &saveptr, ਨਿਰਾ, &controller, ਨਿਰਾ))
            {
              cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
              ਅੱਖਰ *subsystem;
              ਜੇ (has_ਅਗੇਤਰ (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              ਜੇ (ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_LEGACY && strcmp (subsystem, "unified") == 0)
                ਜਾਰੀ;

              ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, subsystem, ਰਸਤਾ, ਨਿਰਾ);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;

              ret = rmdir (cgroup_ਰਸਤਾ);
              ਜੇ (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_ਰਸਤਾ);
                  ਜੇ (ret < 0)
                    {
                      ਜੇ (retry_ਗਿਣਤੀ >= max_attempts)
                        ਵਾਪਸ crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_ਰਸਤਾ);
                      retry_ਗਿਣਤੀ++;
                      repeat = true;
                    }
                }
            }
        }

      ਜੇ (repeat)
        {
          ਬਣਤਰ timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, ਨਿਰਾ);

          ret = cgroup_killall_ਰਸਤਾ (ਰਸਤਾ, SIGKILL, err);
          ਜੇ (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } ਜਦੋਂ (repeat);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
chown_cgroups (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *delegate = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ dfd = -1;
  size_t delegate_ਮਾਪ;
  ਅੱਖਰ *saveptr = ਨਿਰਾ;
  ਅੱਖਰ *name;
  ਪੂਰਨ ret;

  ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  dfd = open (cgroup_ਰਸਤਾ, O_CLOEXEC | O_ਰਸਤਾ);
  ਜੇ (UNLIKELY (dfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", cgroup_ਰਸਤਾ);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_ਮਾਪ, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ਵਾਪਸ 0;
        }
      ਵਾਪਸ ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ | AT_SYMLINK_NOFOLLOW);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "cannot chown `%s`", cgroup_ਰਸਤਾ);

  ਲਈ (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (ਨਿਰਾ, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (errno == ENOENT)
            ਜਾਰੀ;

          ਵਾਪਸ crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_ਰਸਤਾ, name);
        }
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libcrun_cgroup_pause_unpause_with_ਬਹੁਲਕ (ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ, ਪੂਰਨ cgroup_ਬਹੁਲਕ, ਅਚਲ bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *state = "";
  ਪੂਰਨ ret;

  ਜੇ (cgroup_ਰਸਤਾ == ਨਿਰਾ || cgroup_ਰਸਤਾ[0] == '\0')
    ਵਾਪਸ crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, cgroup_ਰਸਤਾ, "cgroup.freeze", ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ "/freezer", cgroup_ਰਸਤਾ, "freezer.state", ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = write_cgroup_file (AT_FDCWD, ਰਸਤਾ, state, ਲੜੀ_ਲੰਬਾਈ (state), err);
  ਜੇ (ret >= 0)
    ਵਾਪਸ 0;
  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_cgroup_pause_unpause_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ, ਅਚਲ bool pause, libcrun_error_t *err)
{
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਵਾਪਸ libcrun_cgroup_pause_unpause_with_ਬਹੁਲਕ (cgroup_ਰਸਤਾ, cgroup_ਬਹੁਲਕ, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
ਪੂਰਨ
libcrun_cgroup_ensure_not_frozen (ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *frozen;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, cgroup_ਰਸਤਾ, "cgroup.freeze", ਨਿਰਾ);
    }
  ਨਹੀਂਤਾਂ
    {
      frozen = "FROZEN";
      ret = append_paths (&ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ "/freezer", cgroup_ਰਸਤਾ, "freezer.state", ਨਿਰਾ);
    }
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = read_all_file (ਰਸਤਾ, &content, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      ਜੇ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ਵਾਪਸ 0;
        }
      ਵਾਪਸ ret;
    }

  ਜੇ (ਲੜੀ_ਖੋਜ (content, frozen))
    ਵਾਪਸ crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  ਵਾਪਸ 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
ਸਥਿਰ ਪੂਰਨ
watch_cgroup_events (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
  cleanup_free ਅੱਖਰ *events_file = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ fd = -1;
  libcrun_error_t tmp_err = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, "cgroup.events", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ਵਾਪਸ -1;
    }

  fd = inotify_init1 (ਵਿੱਚ_CLOEXEC | ਵਿੱਚ_NONBLOCK);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ -1;

  ret = inotify_ਜੋੜੋ_watch (fd, events_file, ਵਿੱਚ_MODIFY);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ -1;

  ret = fd;
  fd = -1;
  ਵਾਪਸ ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
ਸਥਿਰ ਪੂਰਨ
wait_cgroup_unpopulated (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ inotify_fd)
{
  cleanup_free ਅੱਖਰ *events_file = ਨਿਰਾ;
  libcrun_error_t tmp_err = ਨਿਰਾ;
  ਬਣਤਰ timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  ਪੂਰਨ ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, "cgroup.events", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ਵਾਪਸ 0;
    }

  ਲਈ (;;)
    {
      cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
      ਅੱਖਰ buf[4096];
      ਬਣਤਰ pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, ਨਿਰਾ, &tmp_err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          ਵਾਪਸ 0;
        }

      ਜੇ (ਲੜੀ_ਖੋਜ (content, "populated 0"))
        ਵਾਪਸ 1;

      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, ਨਿਰਾ));
      ਜੇ (ret <= 0)
        ਵਾਪਸ 0;

      ਜਦੋਂ (ਪਠ (inotify_fd, buf, ਆਕਾਰ (buf)) > 0)
        ;
    }
}

ਪੂਰਨ
cgroup_killall_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਸੰਕੇਤ, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close ਪੂਰਨ inotify_fd = -1;
  ਪੂਰਨ ret;
  size_t i;
  cleanup_free pid_t *pids = ਨਿਰਾ;

  ਜੇ (ਰਸਤਾ == ਨਿਰਾ || *ਰਸਤਾ == '\0')
    ਵਾਪਸ 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  ਜੇ (ਸੰਕੇਤ == SIGKILL)
    {
      cleanup_free ਅੱਖਰ *kill_file = ਨਿਰਾ;

      ret = append_paths (&kill_file, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, "cgroup.kill", ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      inotify_fd = watch_cgroup_events (ਰਸਤਾ);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      ਜੇ (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (ਰਸਤਾ, inotify_fd))
        ਵਾਪਸ 0;

      ਜੇ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_ਰਸਤਾ (ਰਸਤਾ, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    crun_error_release (err);
  ਨਹੀਂਤਾਂ
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_ਰਸਤਾ (ਰਸਤਾ, true, ਸੰਕੇਤ == SIGKILL, &pids, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (crun_error_get_errno (err) != ENOENT)
        ਜਾਓ thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  ਲਈ (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], ਸੰਕੇਤ);
      ਜੇ (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          ਜਾਓ thaw;
        }
    }

  ret = 0;

thaw:
  ਜੇ (frozen)
    {
      libcrun_error_t thaw_err = ਨਿਰਾ;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      ਜੇ (libcrun_cgroup_pause_unpause_ਰਸਤਾ (ਰਸਤਾ, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
read_ਉਪਲਬਧ_controllers (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *controllers = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *buf = ਨਿਰਾ;
  ਅੱਖਰ *saveptr = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *token;
  ਪੂਰਨ ਉਪਲਬਧ = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, "cgroup.controllers", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = read_all_file (controllers, &buf, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਲਈ (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (ਨਿਰਾ, " \n", &saveptr))
    {
      ਜੇ (strcmp (token, "memory") == 0)
        ਉਪਲਬਧ |= CGROUP_ਸਮ੍ਰਿਤੀ;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (token, "cpu") == 0)
        ਉਪਲਬਧ |= CGROUP_CPU;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (token, "cpuset") == 0)
        ਉਪਲਬਧ |= CGROUP_CPUSET;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (token, "hugetlb") == 0)
        ਉਪਲਬਧ |= CGROUP_HUGETLB;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (token, "pids") == 0)
        ਉਪਲਬਧ |= CGROUP_PIDS;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (token, "io") == 0)
        ਉਪਲਬਧ |= CGROUP_IO;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (token, "misc") == 0)
        ਉਪਲਬਧ |= CGROUP_MISC;
    }
  ਵਾਪਸ ਉਪਲਬਧ;
}

ਸਥਿਰ ਪੂਰਨ
write_controller_file (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ controllers_ਮੰਜ਼ਿਲ_enable, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *subtree_control = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *controllers = ਨਿਰਾ;
  size_t controllers_len = 0;
  ਪੂਰਨ ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_ਮੰਜ਼ਿਲ_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_ਮੰਜ਼ਿਲ_enable & CGROUP_IO) ? "+io" : "", (controllers_ਮੰਜ਼ਿਲ_enable & CGROUP_ਸਮ੍ਰਿਤੀ) ? "+memory" : "",
      (controllers_ਮੰਜ਼ਿਲ_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_ਮੰਜ਼ਿਲ_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_ਮੰਜ਼ਿਲ_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_ਮੰਜ਼ਿਲ_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, "cgroup.subtree_control", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      cleanup_free ਅੱਖਰ *controllers_copy = xmalloc (controllers_len + 1);
      ਪੂਰਨ attempts_ਖੱਬਾ;
      ਅੱਖਰ *saveptr = ਨਿਰਾ;
      ਅਚਲ ਅੱਖਰ *token;
      ਪੂਰਨ e;

      e = crun_error_get_errno (err);
      ਜੇ (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        ਵਾਪਸ crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      ਜੇ (e == ENOENT)
        {
          libcrun_error_t tmp_err = ਨਿਰਾ;
          ਪੂਰਨ exists;

          exists = crun_ਰਸਤਾ_exists (subtree_control, &tmp_err);
          ਜੇ (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              ਵਾਪਸ ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          ਜੇ (exists == 0)
            ਵਾਪਸ ret;
        }

      crun_error_release (err);

      ਜੇ (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (ਰਸਤਾ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      ਲਈ (attempts_ਖੱਬਾ = 1000; attempts_ਖੱਬਾ >= 0; attempts_ਖੱਬਾ--)
        {
          ਪੂਰਨ controllers_written;
          bool repeat = false;

          ਸਮ੍ਰਿਤੀ_ਨਕਲ (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          ਲਈ (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (ਨਿਰਾ, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, ਲੜੀ_ਲੰਬਾਈ (token), err);
              ਜੇ (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  ਜੇ (e == EBUSY)
                    repeat = true;

                  ਜਾਰੀ;
                }

              controllers_written++;
            }

          ਜੇ (! repeat)
            ਰੋਕੋ;

          /* If there was any controller written, just try once more without any delay.  */
          ਜੇ (controllers_written > 0 && attempts_ਖੱਬਾ > 2)
            {
              attempts_ਖੱਬਾ = 1;
              ਜਾਰੀ;
            }

          ਜੇ (attempts_ਖੱਬਾ > 0)
            {
              ਬਣਤਰ timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, ਨਿਰਾ);
            }
        }

      /* Refresh what controllers are available.  */
      ਵਾਪਸ read_ਉਪਲਬਧ_controllers (ਰਸਤਾ, err);
    }

  /* All controllers were enabled successfully.  */
  ਵਾਪਸ controllers_ਮੰਜ਼ਿਲ_enable;
}

ਪੂਰਨ
enable_controllers (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *tmp_ਰਸਤਾ = ਨਿਰਾ;
  ਅੱਖਰ *it;
  ਪੂਰਨ ret, controllers_ਮੰਜ਼ਿਲ_enable;

  xasprintf (&tmp_ਰਸਤਾ, "%s/", ਰਸਤਾ);

  ret = read_ਉਪਲਬਧ_controllers ("", err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  controllers_ਮੰਜ਼ਿਲ_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_ਮੰਜ਼ਿਲ_enable, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      ਜੇ ((controllers_ਮੰਜ਼ਿਲ_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_ਮੰਜ਼ਿਲ_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_ਮੰਜ਼ਿਲ_enable, err);
        }
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਲਈ (it = strchr (tmp_ਰਸਤਾ + 1, '/'); it;)
    {
      cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
      ਅੱਖਰ *ਅਗਲਾ_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, tmp_ਰਸਤਾ, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = mkdir (cgroup_ਰਸਤਾ, 0755);
      ਜੇ (UNLIKELY (ret < 0 && errno != EEXIST))
        ਵਾਪਸ crun_make_error (err, errno, "create `%s`", cgroup_ਰਸਤਾ);

      ਜੇ (ਅਗਲਾ_slash)
        {
          ret = write_controller_file (tmp_ਰਸਤਾ, controllers_ਮੰਜ਼ਿਲ_enable, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      *it = '/';
      it = ਅਗਲਾ_slash;
    }
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_move_process_ਮੰਜ਼ਿਲ_cgroup (pid_t pid, pid_t init_pid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool create_if_missing, libcrun_error_t *err)
{
  ਪੂਰਨ cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (ਰਸਤਾ == ਨਿਰਾ || *ਰਸਤਾ == '\0')
    ਵਾਪਸ 0;

  ਵਾਪਸ enter_cgroup (cgroup_ਬਹੁਲਕ, pid, init_pid, ਰਸਤਾ, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
ਪੂਰਨ
libcrun_move_self_ਮੰਜ਼ਿਲ_cgroups (ਅਚਲ ਅੱਖਰ *content, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *buf = xstrdup (content);
  ਅੱਖਰ *saveptr = ਨਿਰਾ;
  ਅੱਖਰ *controller;
  ਅੱਖਰ *ਰਸਤਾ;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  bool has_ਅੰਕੜੇ;
  ਪੂਰਨ ret;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਲਈ (has_ਅੰਕੜੇ = read_proc_cgroup (buf, &saveptr, ਨਿਰਾ, &controller, &ਰਸਤਾ);
       has_ਅੰਕੜੇ;
       has_ਅੰਕੜੇ = read_proc_cgroup (ਨਿਰਾ, &saveptr, ਨਿਰਾ, &controller, &ਰਸਤਾ))
    {
      ਅਚਲ ਅੱਖਰ *subsystem = ਨਿਰਾ;

      ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          ਜੇ (controller[0] != '\0')
            ਜਾਰੀ;
        }
      ਨਹੀਂਤਾਂ
        {
          cleanup_free ਅੱਖਰ *subsystem_ਰਸਤਾ = ਨਿਰਾ;

          /* Use the same names as enter_cgroup_v1 does.  */
          ਜੇ (has_ਅਗੇਤਰ (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          ਜੇ (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          ਜੇ (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, subsystem, ਨਿਰਾ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          ret = crun_ਰਸਤਾ_exists (subsystem_ਰਸਤਾ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          ਜੇ (ret == 0)
            ਜਾਰੀ;
        }

      ret = move_process_ਮੰਜ਼ਿਲ_cgroup (0, subsystem, ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_get_cgroup_dirfd (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਰਸਤਾ_ਮੰਜ਼ਿਲ_cgroup = ਨਿਰਾ;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ cgroupdirfd;
  ਪੂਰਨ ret;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (cgroup_ਬਹੁਲਕ < 0)
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (cgroup_ਬਹੁਲਕ != CGROUP_ਬਹੁਲਕ_UNIFIED)
    ਵਾਪਸ crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  ਜੇ (ਸਥਿਤੀ == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "internal error");

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਸਥਿਤੀ->ਰਸਤਾ))
    ਵਾਪਸ crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&ਰਸਤਾ_ਮੰਜ਼ਿਲ_cgroup, err, CGROUP_ਜੜ੍ਹ, ਸਥਿਤੀ->ਰਸਤਾ, sub_cgroup, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  cgroupdirfd = open (ਰਸਤਾ_ਮੰਜ਼ਿਲ_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_ਰਸਤਾ);
  ਜੇ (UNLIKELY (cgroupdirfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਰਸਤਾ_ਮੰਜ਼ਿਲ_cgroup);

  ਵਾਪਸ cgroupdirfd;
}

ਪੂਰਨ
libcrun_migrate_all_pids_ਮੰਜ਼ਿਲ_cgroup (pid_t init_pid, ਅੱਖਰ *from, ਅੱਖਰ *ਮੰਜ਼ਿਲ, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ ਬੱਚਾ_dfd = -1;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  size_t from_len;
  size_t i;
  ਪੂਰਨ ret;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (cgroup_ਬਹੁਲਕ < 0)
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_ਰਸਤਾ (from, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_cgroup_read_pids_from_ਰਸਤਾ (from, true, &pids, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ thaw;

  from_len = ਲੜੀ_ਲੰਬਾਈ (from);

  ਲਈ (i = 0; pids && pids[i]; i++)
    {
      cleanup_free ਅੱਖਰ *pid_ਰਸਤਾ = ਨਿਰਾ;
      cleanup_free ਅੱਖਰ *dest_cgroup = ਨਿਰਾ;

      ret = libcrun_get_cgroup_process (pids[i], &pid_ਰਸਤਾ, false, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      ਜੇ (! has_ਅਗੇਤਰ (pid_ਰਸਤਾ, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          ਜਾਓ thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", ਮੰਜ਼ਿਲ, pid_ਰਸਤਾ + from_len);

      ret = enter_cgroup (cgroup_ਬਹੁਲਕ, pids[i], init_pid, dest_cgroup, false, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ thaw;
    }

  ret = 0;

thaw:
  ਜੇ (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = ਨਿਰਾ;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      ਜੇ (libcrun_cgroup_pause_unpause_ਰਸਤਾ (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      ਵਾਪਸ ret;
    }

  ret = libcrun_cgroup_pause_unpause_ਰਸਤਾ (from, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ destroy_cgroup_ਰਸਤਾ (from, cgroup_ਬਹੁਲਕ, err);
}

ਪੂਰਨ
get_cgroup_dirfd_ਰਸਤਾ (ਪੂਰਨ dirfd, ਅੱਖਰ **ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
  proc_fd_ਰਸਤਾ_t fd_ਰਸਤਾ;
  ssize_t len;

  get_proc_self_fd_ਰਸਤਾ (fd_ਰਸਤਾ, dirfd);

  len = ਸੁਰੱਖਿਅਤ_readlinkat (AT_FDCWD, fd_ਰਸਤਾ, &cgroup_ਰਸਤਾ, 0, err);
  ਜੇ (UNLIKELY (len < 0))
    ਵਾਪਸ len;

  ਜੇ (has_ਅਗੇਤਰ (cgroup_ਰਸਤਾ, CGROUP_ਜੜ੍ਹ))
    {
      *ਰਸਤਾ = xstrdup (cgroup_ਰਸਤਾ + ਲੜੀ_ਲੰਬਾਈ (CGROUP_ਜੜ੍ਹ));
      ਵਾਪਸ 0;
    }
  ਵਾਪਸ crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_ਰਸਤਾ);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
ਪੂਰਨ
parse_cpu_max (ਅਚਲ ਅੱਖਰ *ਮੁੱਲ, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *it = ਮੁੱਲ;
  ਅੱਖਰ *endptr = ਨਿਰਾ;

  ਜਦੋਂ (*it == ' ')
    it++;

  errno = 0;
  ਜੇ (has_ਅਗੇਤਰ (it, "max"))
    {
      *quota = -1;
      endptr = (ਅੱਖਰ *) it + 3;
    }
  ਨਹੀਂਤਾਂ
    {
      *quota = strtoll (it, &endptr, 10);
      ਜੇ (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        ਵਾਪਸ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", ਮੁੱਲ);
    }

  it = endptr;
  ਜਦੋਂ (*it == ' ')
    it++;

  *has_period = false;
  ਜੇ (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      ਜੇ (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        ਵਾਪਸ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", ਮੁੱਲ);
      *has_period = true;
    }

  ਵਾਪਸ 0;
}
