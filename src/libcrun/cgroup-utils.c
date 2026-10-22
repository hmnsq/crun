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

#अन्तर्भाव <config.h>
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव "cgroup-internal.h"
#अन्तर्भाव "cgroup-systemd.h"
#अन्तर्भाव "cgroup-utils.h"
#अन्तर्भाव "cgroup-setup.h"
#अन्तर्भाव "ebpf.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "status.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <poll.h>
#अन्तर्भाव <sys/inotify.h>

#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <libgen.h>

संरचना symlink_s
{
  ध्रुव वर्ण *name;
  ध्रुव वर्ण *target;
};

स्थिर संरचना symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { रिक्त, रिक्त } };

पूर्णाङ्क
libcrun_cgroups_create_symlinks (पूर्णाङ्क dirfd, libcrun_error_t *err)
{
  पूर्णाङ्क i;

  प्रति (i = 0; cgroup_symlinks[i].name; i++)
    {
      पूर्णाङ्क ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (errno == ENOENT || errno == EEXIST)
            अनुवर्त;
          प्रत्यागम crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  प्रत्यागम 0;
}

पूर्णाङ्क
maybe_make_cgroup_threaded (ध्रुव वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path_type = रिक्त;
  ध्रुव वर्ण *ध्रुव threaded = "threaded";
  cleanup_free वर्ण *content = रिक्त;
  cleanup_free वर्ण *buffer = रिक्त;
  ध्रुव वर्ण *parent;
  size_t size;
  वर्ण *it;
  पूर्णाङ्क ret;

  यदि (path == रिक्त)
    प्रत्यागम 0;

  path = consume_slashes (path);

  यदि (path[0] == '\0')
    प्रत्यागम 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_मूल, path, "cgroup.type", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (size > 0)
    {
      it = content + size - 1;
      यावत् (*it == '\n' && it > content)
        *it-- = '\0';
    }

  यदि (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    प्रत्यागम 0;

  buffer = xstrdup (path);
  parent = consume_slashes (dirname (buffer));
  यदि (parent[0] && strcmp (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      यदि (ret < 0)
        प्रत्यागम ret;
    }
  प्रत्यागम write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, strlen (threaded), err);
}

पूर्णाङ्क
move_process_to_cgroup (pid_t pid, ध्रुव वर्ण *subsystem, ध्रुव वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path_procs = रिक्त;
  वर्ण pid_str[16];
  पूर्णाङ्क ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_मूल,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = snprintf (pid_str, परिमाण (pid_str), "%d", pid);
  यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (pid_str)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = रिक्त;
          पूर्णाङ्क mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          यदि (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              प्रत्यागम ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          प्रत्यागम write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, strlen (pid_str), err);
        }

      प्रत्यागम ret;
    }
  प्रत्यागम ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

स्थिर पूर्णाङ्क
detect_cgroup_mode (libcrun_error_t *err)
{
  संरचना statfs stat;
  पूर्णाङ्क ret;

  ret = statfs (CGROUP_मूल, &stat);
  यदि (ret < 0)
    प्रत्यागम crun_make_error (err, errno, "statfs `" CGROUP_मूल "`");
  यदि (stat.f_type == CGROUP2_SUPER_MAGIC)
    प्रत्यागम CGROUP_MODE_UNIFIED;
  यदि (stat.f_type != TMPFS_MAGIC)
    प्रत्यागम crun_make_error (err, 0, "invalid file system type on `" CGROUP_मूल "`");
  ret = statfs (CGROUP_मूल "/unified", &stat);
  यदि (ret < 0 && errno != ENOENT)
    प्रत्यागम crun_make_error (err, errno, "statfs `" CGROUP_मूल "/unified`");
  यदि (ret < 0)
    प्रत्यागम CGROUP_MODE_LEGACY;
  प्रत्यागम stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

पूर्णाङ्क
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  पूर्णाङ्क tmp;
  स्थिर पूर्णाङ्क cgroup_mode = 0;

  यदि (cgroup_mode)
    प्रत्यागम cgroup_mode;

  tmp = detect_cgroup_mode (err);
  यदि (UNLIKELY (tmp < 0))
    प्रत्यागम tmp;

  cgroup_mode = tmp;

  प्रत्यागम cgroup_mode;
}

स्थिर पूर्णाङ्क
get_cgroup_process_from_content (वर्ण *content, पूर्णाङ्क cgroup_mode, वर्ण **path, bool absolute, libcrun_error_t *err)
{
  वर्ण *cg_path = रिक्त;
  वर्ण *controller;
  वर्ण *saveptr;
  bool has_data;

  प्रति (has_data = read_proc_cgroup (content, &saveptr, रिक्त, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (रिक्त, &saveptr, रिक्त, &controller, &cg_path))
    {
      यदि (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          यदि (strcmp (controller, "") == 0 && strlen (cg_path) > 0)
            गच्छ found;
        }
      अन्यथा
        {
          यदि (strcmp (controller, "memory"))
            गच्छ found;
        }
    }

  प्रत्यागम crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  यदि (absolute)
    प्रत्यागम append_paths (path, err, CGROUP_मूल, cg_path, रिक्त);

  *path = xstrdup (cg_path);
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_get_cgroup_process_at (पूर्णाङ्क dirfd, वर्ण **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  size_t content_size;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    प्रत्यागम cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

पूर्णाङ्क
libcrun_get_cgroup_process (pid_t pid, वर्ण **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  वर्ण proc_cgroup_file[64];
  size_t content_size;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    प्रत्यागम cgroup_mode;

  यदि (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  अन्यथा
    {
      पूर्णाङ्क len = snprintf (proc_cgroup_file, परिमाण (proc_cgroup_file), "/proc/%d/cgroup", pid);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (proc_cgroup_file)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

स्थिर पूर्णाङ्क
read_pids_file (पूर्णाङ्क dfd, ध्रुव वर्ण *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क tasksfd = -1;
  cleanup_free वर्ण *buffer = रिक्त;
  वर्ण *saveptr = रिक्त;
  size_t n_new_pids;
  size_t len;
  वर्ण *it;
  पूर्णाङ्क ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  यदि (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      यदि (errno == ENOENT || errno == ENODEV)
        प्रत्यागम 0;
      प्रत्यागम crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (len > 0)
    {
      प्रति (n_new_pids = 0, it = buffer; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      यदि (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, परिमाण (pid_t) * *allocated);
        }

      प्रति (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (रिक्त, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, रिक्त, 10);

          यदि (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  प्रत्यागम 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
संरचना cgroup_walk_dir_s
{
  वर्ण *name;
  size_t depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
संरचना cgroup_walk_s
{
  पूर्णाङ्क dfd;
  size_t depth;
  संरचना cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

स्थिर शून्य
cgroup_walk_निकालो (संरचना cgroup_walk_s *walk)
{
  मुक्त (walk->dirs[--walk->n_dirs].name);
}

स्थिर शून्य
cleanup_cgroup_walkp (संरचना cgroup_walk_s *walk)
{
  पूर्णाङ्क saved_errno = errno;

  यावत् (walk->n_dirs > 0)
    cgroup_walk_निकालो (walk);
  मुक्त (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
स्थिर पूर्णाङ्क
cgroup_walk_enter (संरचना cgroup_walk_s *walk, पूर्णाङ्क dfd)
{
  cleanup_close पूर्णाङ्क dfd_clone = -1;
  cleanup_dir DIR *dir = रिक्त;
  संरचना dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  यदि (UNLIKELY (dfd_clone < 0))
    प्रत्यागम -1;

  dir = fdopendir (dfd_clone);
  यदि (UNLIKELY (dir == रिक्त))
    प्रत्यागम -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  प्रति (de = readdir (dir); de; de = readdir (dir))
    {
      संरचना cgroup_walk_dir_s *d;

      यदि (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        अनुवर्त;

      यदि (de->d_type != DT_DIR)
        अनुवर्त;

      यदि (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, परिमाण (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  प्रत्यागम 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
स्थिर पूर्णाङ्क
cgroup_walk_next (संरचना cgroup_walk_s *walk, संरचना cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : रिक्त;

  यावत् (*d && walk->depth >= (*d)->depth)
    {
      पूर्णाङ्क parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      यदि (UNLIKELY (parent < 0))
        प्रत्यागम -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
read_pids_cgroup_file (पूर्णाङ्क dfd, ध्रुव वर्ण *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk संरचना cgroup_walk_s walk = { .dfd = -1 };
  पूर्णाङ्क dfd_clone;
  पूर्णाङ्क ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (! recurse)
    प्रत्यागम 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (dfd_clone < 0))
    प्रत्यागम crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "open cgroup sub-directory");

  प्रति (;;)
    {
      संरचना cgroup_walk_dir_s *d;
      cleanup_close पूर्णाङ्क nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "open parent cgroup directory");
      यदि (d == रिक्त)
        भङ्ग;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      यदि (UNLIKELY (nfd < 0))
        {
          पूर्णाङ्क saved_errno = errno;

          यदि (saved_errno == ENOENT)
            {
              cgroup_walk_निकालो (&walk);
              अनुवर्त;
            }
          प्रत्यागम crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_निकालो (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      यदि (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          यदि (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            प्रत्यागम ret;
          crun_error_release (err);
          अनुवर्त;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      यदि (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        प्रत्यागम crun_make_error (err, errno, "open cgroup sub-directory");
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
read_pids_cgroup (पूर्णाङ्क dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  यदि (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
rmdir_all_fd (पूर्णाङ्क dfd)
{
  cleanup_cgroup_walk संरचना cgroup_walk_s walk = { .dfd = -1 };
  पूर्णाङ्क ret;

  ret = cgroup_walk_enter (&walk, dfd);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रति (;;)
    {
      संरचना cgroup_walk_dir_s *d;
      पूर्णाङ्क child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      यदि (d == रिक्त)
        भङ्ग;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      यदि (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_निकालो (&walk);
          अनुवर्त;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      यदि (UNLIKELY (child_dfd < 0))
        {
          यदि (errno == ENOENT)
            {
              cgroup_walk_निकालो (&walk);
              अनुवर्त;
            }
          प्रत्यागम child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      यदि (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
rmdir_all (ध्रुव वर्ण *path)
{
  पूर्णाङ्क ret;
  पूर्णाङ्क dfd = open (path, O_DIRECTORY | O_CLOEXEC);
  यदि (UNLIKELY (dfd < 0))
    प्रत्यागम dfd;

  ret = rmdir_all_fd (dfd);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम rmdir (path);
}

स्थिर पूर्णाङ्क
read_pids_from_path (ध्रुव वर्ण *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path = रिक्त;
  cleanup_close पूर्णाङ्क dirfd = -1;
  size_t n_pids, allocated;
  पूर्णाङ्क mode;
  पूर्णाङ्क ret;

  यदि (path == रिक्त || *path == '\0')
    प्रत्यागम 0;

  mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (mode < 0))
    प्रत्यागम mode;

  विकल्प (mode)
    {
    पक्ष CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_मूल, path, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      भङ्ग;

    पक्ष CGROUP_MODE_HYBRID:
    पक्ष CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_मूल "/memory", path, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      भङ्ग;

    स्वभाव:
      प्रत्यागम crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (dirfd < 0)
    प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  यदि (threads && mode == CGROUP_MODE_UNIFIED)
    प्रत्यागम read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  प्रत्यागम read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

पूर्णाङ्क
libcrun_cgroup_read_pids_from_path (ध्रुव वर्ण *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  प्रत्यागम read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (वर्ण *content, वर्ण **saveptr, वर्ण **id, वर्ण **controller_list, वर्ण **path)
{
  वर्ण *it;

  it = strtok_r (content, "\n", saveptr);
  यदि (it == रिक्त)
    प्रत्यागम false;

  यदि (id)
    *id = it;

  it = strchr (it, ':');
  यदि (it == रिक्त)
    प्रत्यागम false;
  *it++ = '\0';

  यदि (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  यदि (it == रिक्त)
    प्रत्यागम false;
  *it++ = '\0';

  यदि (path)
    *path = it;

  प्रत्यागम true;
}

पूर्णाङ्क
destroy_cgroup_path (ध्रुव वर्ण *path, पूर्णाङ्क mode, libcrun_error_t *err)
{
  bool repeat = true;
  पूर्णाङ्क retry_गिनती = 0;
  ध्रुव पूर्णाङ्क max_attempts = 500;
  पूर्णाङ्क ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  यदि (is_empty_string (path))
    प्रत्यागम 0;

  कुरु
    {
      repeat = false;

      यदि (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free वर्ण *cgroup_path = रिक्त;

          ret = append_paths (&cgroup_path, err, CGROUP_मूल, path, रिक्त);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          ret = rmdir (cgroup_path);
          यदि (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              यदि (ret < 0)
                {
                  यदि (retry_गिनती >= max_attempts)
                    प्रत्यागम crun_make_error (err, errno, "cannot delete path `%s`", cgroup_path);

                  retry_गिनती++;
                  repeat = true;
                }
            }
        }
      अन्यथा
        {
          cleanup_free वर्ण *content = रिक्त;
          size_t content_size;
          वर्ण *controller;
          वर्ण *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          यदि (UNLIKELY (ret < 0))
            {
              यदि (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  प्रत्यागम 0;
                }
              प्रत्यागम ret;
            }

          प्रति (has_data = read_proc_cgroup (content, &saveptr, रिक्त, &controller, रिक्त);
               has_data;
               has_data = read_proc_cgroup (रिक्त, &saveptr, रिक्त, &controller, रिक्त))
            {
              cleanup_free वर्ण *cgroup_path = रिक्त;
              वर्ण *subsystem;
              यदि (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              यदि (mode == CGROUP_MODE_LEGACY && strcmp (subsystem, "unified") == 0)
                अनुवर्त;

              ret = append_paths (&cgroup_path, err, CGROUP_मूल, subsystem, path, रिक्त);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम ret;

              ret = rmdir (cgroup_path);
              यदि (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  यदि (ret < 0)
                    {
                      यदि (retry_गिनती >= max_attempts)
                        प्रत्यागम crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_गिनती++;
                      repeat = true;
                    }
                }
            }
        }

      यदि (repeat)
        {
          संरचना timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, रिक्त);

          ret = cgroup_killall_path (path, SIGKILL, err);
          यदि (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } यावत् (repeat);

  प्रत्यागम 0;
}

पूर्णाङ्क
chown_cgroups (ध्रुव वर्ण *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path = रिक्त;
  cleanup_free वर्ण *delegate = रिक्त;
  cleanup_close पूर्णाङ्क dfd = -1;
  size_t delegate_size;
  वर्ण *saveptr = रिक्त;
  वर्ण *name;
  पूर्णाङ्क ret;

  ret = append_paths (&cgroup_path, err, CGROUP_मूल, path, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dfd = open (cgroup_path, O_CLOEXEC | O_PATH);
  यदि (UNLIKELY (dfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          प्रत्यागम 0;
        }
      प्रत्यागम ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot chown `%s`", cgroup_path);

  प्रति (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (रिक्त, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (errno == ENOENT)
            अनुवर्त;

          प्रत्यागम crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libcrun_cgroup_pause_unpause_with_mode (ध्रुव वर्ण *cgroup_path, पूर्णाङ्क cgroup_mode, ध्रुव bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free वर्ण *path = रिक्त;
  ध्रुव वर्ण *state = "";
  पूर्णाङ्क ret;

  यदि (cgroup_path == रिक्त || cgroup_path[0] == '\0')
    प्रत्यागम crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  यदि (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_मूल, cgroup_path, "cgroup.freeze", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अन्यथा
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_मूल "/freezer", cgroup_path, "freezer.state", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, strlen (state), err);
  यदि (ret >= 0)
    प्रत्यागम 0;
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_cgroup_pause_unpause_path (ध्रुव वर्ण *cgroup_path, ध्रुव bool pause, libcrun_error_t *err)
{
  पूर्णाङ्क cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    प्रत्यागम cgroup_mode;

  प्रत्यागम libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
पूर्णाङ्क
libcrun_cgroup_ensure_not_frozen (ध्रुव वर्ण *cgroup_path, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  cleanup_free वर्ण *path = रिक्त;
  ध्रुव वर्ण *frozen;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    प्रत्यागम cgroup_mode;

  यदि (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_मूल, cgroup_path, "cgroup.freeze", रिक्त);
    }
  अन्यथा
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_मूल "/freezer", cgroup_path, "freezer.state", रिक्त);
    }
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file (path, &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      यदि (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          प्रत्यागम 0;
        }
      प्रत्यागम ret;
    }

  यदि (strstr (content, frozen))
    प्रत्यागम crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  प्रत्यागम 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
स्थिर पूर्णाङ्क
watch_cgroup_events (ध्रुव वर्ण *path)
{
  cleanup_free वर्ण *events_file = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_मूल, path, "cgroup.events", रिक्त);
  यदि (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      प्रत्यागम -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम -1;

  ret = fd;
  fd = -1;
  प्रत्यागम ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
स्थिर पूर्णाङ्क
wait_cgroup_unpopulated (ध्रुव वर्ण *path, पूर्णाङ्क inotify_fd)
{
  cleanup_free वर्ण *events_file = रिक्त;
  libcrun_error_t tmp_err = रिक्त;
  संरचना timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  पूर्णाङ्क ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_मूल, path, "cgroup.events", रिक्त);
  यदि (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      प्रत्यागम 0;
    }

  प्रति (;;)
    {
      cleanup_free वर्ण *content = रिक्त;
      वर्ण buf[4096];
      संरचना pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, रिक्त, &tmp_err);
      यदि (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          प्रत्यागम 0;
        }

      यदि (strstr (content, "populated 0"))
        प्रत्यागम 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, रिक्त));
      यदि (ret <= 0)
        प्रत्यागम 0;

      यावत् (पठ (inotify_fd, buf, परिमाण (buf)) > 0)
        ;
    }
}

पूर्णाङ्क
cgroup_killall_path (ध्रुव वर्ण *path, पूर्णाङ्क सङ्केत, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close पूर्णाङ्क inotify_fd = -1;
  पूर्णाङ्क ret;
  size_t i;
  cleanup_free pid_t *pids = रिक्त;

  यदि (path == रिक्त || *path == '\0')
    प्रत्यागम 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  यदि (सङ्केत == SIGKILL)
    {
      cleanup_free वर्ण *kill_file = रिक्त;

      ret = append_paths (&kill_file, err, CGROUP_मूल, path, "cgroup.kill", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      यदि (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        प्रत्यागम 0;

      यदि (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (err);
  अन्यथा
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, सङ्केत == SIGKILL, &pids, err);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (crun_error_get_errno (err) != ENOENT)
        गच्छ thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  प्रति (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], सङ्केत);
      यदि (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          गच्छ thaw;
        }
    }

  ret = 0;

thaw:
  यदि (frozen)
    {
      libcrun_error_t thaw_err = रिक्त;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      यदि (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
read_available_controllers (ध्रुव वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *controllers = रिक्त;
  cleanup_free वर्ण *buf = रिक्त;
  वर्ण *saveptr = रिक्त;
  ध्रुव वर्ण *token;
  पूर्णाङ्क available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_मूल, path, "cgroup.controllers", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file (controllers, &buf, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रति (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (रिक्त, " \n", &saveptr))
    {
      यदि (strcmp (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      अन्यथा यदि (strcmp (token, "cpu") == 0)
        available |= CGROUP_CPU;
      अन्यथा यदि (strcmp (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      अन्यथा यदि (strcmp (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      अन्यथा यदि (strcmp (token, "pids") == 0)
        available |= CGROUP_PIDS;
      अन्यथा यदि (strcmp (token, "io") == 0)
        available |= CGROUP_IO;
      अन्यथा यदि (strcmp (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  प्रत्यागम available;
}

स्थिर पूर्णाङ्क
write_controller_file (ध्रुव वर्ण *path, पूर्णाङ्क controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free वर्ण *subtree_control = रिक्त;
  cleanup_free वर्ण *controllers = रिक्त;
  size_t controllers_len = 0;
  पूर्णाङ्क ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_मूल, path, "cgroup.subtree_control", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  यदि (UNLIKELY (ret < 0))
    {
      cleanup_free वर्ण *controllers_copy = xmalloc (controllers_len + 1);
      पूर्णाङ्क attempts_बायाँ;
      वर्ण *saveptr = रिक्त;
      ध्रुव वर्ण *token;
      पूर्णाङ्क e;

      e = crun_error_get_errno (err);
      यदि (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        प्रत्यागम crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      यदि (e == ENOENT)
        {
          libcrun_error_t tmp_err = रिक्त;
          पूर्णाङ्क exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          यदि (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              प्रत्यागम ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          यदि (exists == 0)
            प्रत्यागम ret;
        }

      crun_error_release (err);

      यदि (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      प्रति (attempts_बायाँ = 1000; attempts_बायाँ >= 0; attempts_बायाँ--)
        {
          पूर्णाङ्क controllers_written;
          bool repeat = false;

          memcpy (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          प्रति (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (रिक्त, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, strlen (token), err);
              यदि (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  यदि (e == EBUSY)
                    repeat = true;

                  अनुवर्त;
                }

              controllers_written++;
            }

          यदि (! repeat)
            भङ्ग;

          /* If there was any controller written, just try once more without any delay.  */
          यदि (controllers_written > 0 && attempts_बायाँ > 2)
            {
              attempts_बायाँ = 1;
              अनुवर्त;
            }

          यदि (attempts_बायाँ > 0)
            {
              संरचना timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, रिक्त);
            }
        }

      /* Refresh what controllers are available.  */
      प्रत्यागम read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  प्रत्यागम controllers_to_enable;
}

पूर्णाङ्क
enable_controllers (ध्रुव वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *tmp_path = रिक्त;
  वर्ण *it;
  पूर्णाङ्क ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  यदि (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      यदि ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रति (it = strchr (tmp_path + 1, '/'); it;)
    {
      cleanup_free वर्ण *cgroup_path = रिक्त;
      वर्ण *next_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_मूल, tmp_path, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = mkdir (cgroup_path, 0755);
      यदि (UNLIKELY (ret < 0 && errno != EEXIST))
        प्रत्यागम crun_make_error (err, errno, "create `%s`", cgroup_path);

      यदि (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      *it = '/';
      it = next_slash;
    }
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, ध्रुव वर्ण *path, bool create_if_missing, libcrun_error_t *err)
{
  पूर्णाङ्क cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    प्रत्यागम cgroup_mode;

  यदि (path == रिक्त || *path == '\0')
    प्रत्यागम 0;

  प्रत्यागम enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
पूर्णाङ्क
libcrun_move_self_to_cgroups (ध्रुव वर्ण *content, libcrun_error_t *err)
{
  cleanup_free वर्ण *buf = xstrdup (content);
  वर्ण *saveptr = रिक्त;
  वर्ण *controller;
  वर्ण *path;
  पूर्णाङ्क cgroup_mode;
  bool has_data;
  पूर्णाङ्क ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    प्रत्यागम cgroup_mode;

  प्रति (has_data = read_proc_cgroup (buf, &saveptr, रिक्त, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (रिक्त, &saveptr, रिक्त, &controller, &path))
    {
      ध्रुव वर्ण *subsystem = रिक्त;

      यदि (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          यदि (controller[0] != '\0')
            अनुवर्त;
        }
      अन्यथा
        {
          cleanup_free वर्ण *subsystem_path = रिक्त;

          /* Use the same names as enter_cgroup_v1 does.  */
          यदि (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          यदि (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          यदि (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_मूल, subsystem, रिक्त);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          ret = crun_path_exists (subsystem_path, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          यदि (ret == 0)
            अनुवर्त;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_get_cgroup_dirfd (संरचना libcrun_cgroup_status *status, ध्रुव वर्ण *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free वर्ण *path_to_cgroup = रिक्त;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क cgroupdirfd;
  पूर्णाङ्क ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (cgroup_mode < 0)
    प्रत्यागम cgroup_mode;

  यदि (cgroup_mode != CGROUP_MODE_UNIFIED)
    प्रत्यागम crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  यदि (status == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "internal error");

  यदि (is_empty_string (status->path))
    प्रत्यागम crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_मूल, status->path, sub_cgroup, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  cgroupdirfd = open (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  यदि (UNLIKELY (cgroupdirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", path_to_cgroup);

  प्रत्यागम cgroupdirfd;
}

पूर्णाङ्क
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, वर्ण *from, वर्ण *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = रिक्त;
  cleanup_close पूर्णाङ्क child_dfd = -1;
  पूर्णाङ्क cgroup_mode;
  size_t from_len;
  size_t i;
  पूर्णाङ्क ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (cgroup_mode < 0)
    प्रत्यागम cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ thaw;

  from_len = strlen (from);

  प्रति (i = 0; pids && pids[i]; i++)
    {
      cleanup_free वर्ण *pid_path = रिक्त;
      cleanup_free वर्ण *dest_cgroup = रिक्त;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      यदि (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          गच्छ thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ thaw;
    }

  ret = 0;

thaw:
  यदि (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = रिक्त;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      यदि (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      प्रत्यागम ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम destroy_cgroup_path (from, cgroup_mode, err);
}

पूर्णाङ्क
get_cgroup_dirfd_path (पूर्णाङ्क dirfd, वर्ण **path, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path = रिक्त;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  यदि (UNLIKELY (len < 0))
    प्रत्यागम len;

  यदि (has_prefix (cgroup_path, CGROUP_मूल))
    {
      *path = xstrdup (cgroup_path + strlen (CGROUP_मूल));
      प्रत्यागम 0;
    }
  प्रत्यागम crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
पूर्णाङ्क
parse_cpu_max (ध्रुव वर्ण *मूल्य, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ध्रुव वर्ण *it = मूल्य;
  वर्ण *endptr = रिक्त;

  यावत् (*it == ' ')
    it++;

  errno = 0;
  यदि (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (वर्ण *) it + 3;
    }
  अन्यथा
    {
      *quota = strtoll (it, &endptr, 10);
      यदि (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        प्रत्यागम crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", मूल्य);
    }

  it = endptr;
  यावत् (*it == ' ')
    it++;

  *has_period = false;
  यदि (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      यदि (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        प्रत्यागम crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", मूल्य);
      *has_period = true;
    }

  प्रत्यागम 0;
}
