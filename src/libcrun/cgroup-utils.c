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
  स्थिरांक अक्षर *name;
  स्थिरांक अक्षर *target;
};

स्थिर संरचना symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { रिक्त, रिक्त } };

पूर्णांक
libcrun_cgroups_create_symlinks (पूर्णांक dirfd, libcrun_error_t *err)
{
  पूर्णांक i;

  साठी (i = 0; cgroup_symlinks[i].name; i++)
    {
      पूर्णांक ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      जर (UNLIKELY (ret < 0))
        {
          जर (त्रुटि_सं == ENOENT || त्रुटि_सं == EEXIST)
            सुरूठेवा;
          परत crun_make_error (err, त्रुटि_सं, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  परत 0;
}

पूर्णांक
maybe_make_cgroup_threaded (स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path_type = रिक्त;
  स्थिरांक अक्षर *स्थिरांक threaded = "threaded";
  cleanup_free अक्षर *content = रिक्त;
  cleanup_free अक्षर *buffer = रिक्त;
  स्थिरांक अक्षर *parent;
  माप_प्रकार size;
  अक्षर *it;
  पूर्णांक ret;

  जर (path == रिक्त)
    परत 0;

  path = consume_slashes (path);

  जर (path[0] == '\0')
    परत 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_मूळ, path, "cgroup.type", रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = read_all_file (cgroup_path_type, &content, &size, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (size > 0)
    {
      it = content + size - 1;
      पर्यंत (*it == '\n' && it > content)
        *it-- = '\0';
    }

  जर (म_भेद (content, "domain") == 0 || म_भेद (content, "domain threaded") == 0)
    परत 0;

  buffer = xstrdup (path);
  parent = consume_slashes (स_नाम (buffer));
  जर (parent[0] && म_भेद (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      जर (ret < 0)
        परत ret;
    }
  परत write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, म_माप (threaded), err);
}

पूर्णांक
move_process_to_cgroup (pid_t pid, स्थिरांक अक्षर *subsystem, स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path_procs = रिक्त;
  अक्षर pid_str[16];
  पूर्णांक ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_मूळ,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = snprintf (pid_str, आकार (pid_str), "%d", pid);
  जर (UNLIKELY (ret >= (पूर्णांक) आकार (pid_str)))
    परत crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, म_माप (pid_str), err);
  जर (UNLIKELY (ret < 0))
    {
      जर (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = रिक्त;
          पूर्णांक mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          जर (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              परत ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          जर (UNLIKELY (ret < 0))
            परत ret;

          परत write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, म_माप (pid_str), err);
        }

      परत ret;
    }
  परत ret;
}

#ifndef CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#endif

#ifndef TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#endif

स्थिर पूर्णांक
detect_cgroup_mode (libcrun_error_t *err)
{
  संरचना statfs stat;
  पूर्णांक ret;

  ret = statfs (CGROUP_मूळ, &stat);
  जर (ret < 0)
    परत crun_make_error (err, त्रुटि_सं, "statfs `" CGROUP_मूळ "`");
  जर (stat.f_type == CGROUP2_SUPER_MAGIC)
    परत CGROUP_MODE_UNIFIED;
  जर (stat.f_type != TMPFS_MAGIC)
    परत crun_make_error (err, 0, "invalid file system type on `" CGROUP_मूळ "`");
  ret = statfs (CGROUP_मूळ "/unified", &stat);
  जर (ret < 0 && त्रुटि_सं != ENOENT)
    परत crun_make_error (err, त्रुटि_सं, "statfs `" CGROUP_मूळ "/unified`");
  जर (ret < 0)
    परत CGROUP_MODE_LEGACY;
  परत stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

पूर्णांक
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  पूर्णांक tmp;
  स्थिर पूर्णांक cgroup_mode = 0;

  जर (cgroup_mode)
    परत cgroup_mode;

  tmp = detect_cgroup_mode (err);
  जर (UNLIKELY (tmp < 0))
    परत tmp;

  cgroup_mode = tmp;

  परत cgroup_mode;
}

स्थिर पूर्णांक
get_cgroup_process_from_content (अक्षर *content, पूर्णांक cgroup_mode, अक्षर **path, bool absolute, libcrun_error_t *err)
{
  अक्षर *cg_path = रिक्त;
  अक्षर *controller;
  अक्षर *saveptr;
  bool has_data;

  साठी (has_data = read_proc_cgroup (content, &saveptr, रिक्त, &controller, &cg_path);
       has_data;
       has_data = read_proc_cgroup (रिक्त, &saveptr, रिक्त, &controller, &cg_path))
    {
      जर (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          जर (म_भेद (controller, "") == 0 && म_माप (cg_path) > 0)
            जा found;
        }
      अन्यथा
        {
          जर (म_भेद (controller, "memory"))
            जा found;
        }
    }

  परत crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  जर (absolute)
    परत append_paths (path, err, CGROUP_मूळ, cg_path, रिक्त);

  *path = xstrdup (cg_path);
  परत 0;
}

पूर्णांक
libcrun_get_cgroup_process_at (पूर्णांक dirfd, अक्षर **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = रिक्त;
  माप_प्रकार content_size;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (cgroup_mode < 0))
    परत cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_size, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

पूर्णांक
libcrun_get_cgroup_process (pid_t pid, अक्षर **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = रिक्त;
  अक्षर proc_cgroup_file[64];
  माप_प्रकार content_size;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (cgroup_mode < 0))
    परत cgroup_mode;

  जर (pid == 0)
    म_नकल (proc_cgroup_file, PROC_SELF_CGROUP);
  अन्यथा
    {
      पूर्णांक len = snprintf (proc_cgroup_file, आकार (proc_cgroup_file), "/proc/%d/cgroup", pid);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (proc_cgroup_file)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_size, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

स्थिर पूर्णांक
read_pids_file (पूर्णांक dfd, स्थिरांक अक्षर *file, pid_t **pids, माप_प्रकार *n_pids, माप_प्रकार *allocated, libcrun_error_t *err)
{
  cleanup_close पूर्णांक tasksfd = -1;
  cleanup_free अक्षर *buffer = रिक्त;
  अक्षर *saveptr = रिक्त;
  माप_प्रकार n_new_pids;
  माप_प्रकार len;
  अक्षर *it;
  पूर्णांक ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  जर (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      जर (त्रुटि_सं == ENOENT || त्रुटि_सं == ENODEV)
        परत 0;
      परत crun_make_error (err, त्रुटि_सं, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &buffer, &len, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (len > 0)
    {
      साठी (n_new_pids = 0, it = buffer; it; it = म_अक्षर (it + 1, '\n'))
        n_new_pids++;

      जर (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, आकार (pid_t) * *allocated);
        }

      साठी (it = strtok_r (buffer, "\n", &saveptr); it; it = strtok_r (रिक्त, "\n", &saveptr))
        {
          pid_t pid = म_से_अदीर्घ (it, रिक्त, 10);

          जर (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  परत 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
संरचना cgroup_walk_dir_s
{
  अक्षर *name;
  माप_प्रकार depth;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
संरचना cgroup_walk_s
{
  पूर्णांक dfd;
  माप_प्रकार depth;
  संरचना cgroup_walk_dir_s *dirs;
  माप_प्रकार n_dirs;
  माप_प्रकार allocated;
};

स्थिर शून्य
cgroup_walk_काढा (संरचना cgroup_walk_s *walk)
{
  मुक्त (walk->dirs[--walk->n_dirs].name);
}

स्थिर शून्य
cleanup_cgroup_walkp (संरचना cgroup_walk_s *walk)
{
  पूर्णांक saved_errno = त्रुटि_सं;

  पर्यंत (walk->n_dirs > 0)
    cgroup_walk_काढा (walk);
  मुक्त (walk->dirs);
  close_and_reset (&walk->dfd);
  त्रुटि_सं = saved_errno;
}

#define cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
स्थिर पूर्णांक
cgroup_walk_enter (संरचना cgroup_walk_s *walk, पूर्णांक dfd)
{
  cleanup_close पूर्णांक dfd_clone = -1;
  cleanup_dir सूची *dir = रिक्त;
  संरचना dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  जर (UNLIKELY (dfd_clone < 0))
    परत -1;

  dir = fdopendir (dfd_clone);
  जर (UNLIKELY (dir == रिक्त))
    परत -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  साठी (de = सूची_पढ़ो (dir); de; de = सूची_पढ़ो (dir))
    {
      संरचना cgroup_walk_dir_s *d;

      जर (म_भेद (de->d_name, ".") == 0 || म_भेद (de->d_name, "..") == 0)
        सुरूठेवा;

      जर (de->d_type != DT_DIR)
        सुरूठेवा;

      जर (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, आकार (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  परत 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
स्थिर पूर्णांक
cgroup_walk_next (संरचना cgroup_walk_s *walk, संरचना cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : रिक्त;

  पर्यंत (*d && walk->depth >= (*d)->depth)
    {
      पूर्णांक parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      जर (UNLIKELY (parent < 0))
        परत -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  परत 0;
}

स्थिर पूर्णांक
read_pids_cgroup_file (पूर्णांक dfd, स्थिरांक अक्षर *file, bool recurse, pid_t **pids, माप_प्रकार *n_pids, माप_प्रकार *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk संरचना cgroup_walk_s walk = { .dfd = -1 };
  पूर्णांक dfd_clone;
  पूर्णांक ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (! recurse)
    परत 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  जर (UNLIKELY (dfd_clone < 0))
    परत crun_make_error (err, त्रुटि_सं, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "open cgroup sub-directory");

  साठी (;;)
    {
      संरचना cgroup_walk_dir_s *d;
      cleanup_close पूर्णांक nfd = -1;

      ret = cgroup_walk_next (&walk, &d);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "open parent cgroup directory");
      जर (d == रिक्त)
        थांबा;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      जर (UNLIKELY (nfd < 0))
        {
          पूर्णांक saved_errno = त्रुटि_सं;

          जर (saved_errno == ENOENT)
            {
              cgroup_walk_काढा (&walk);
              सुरूठेवा;
            }
          परत crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_काढा (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      जर (UNLIKELY (ret < 0))
        {
          पूर्णांक e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          जर (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            परत ret;
          crun_error_release (err);
          सुरूठेवा;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      जर (UNLIKELY (ret < 0) && त्रुटि_सं != ENOENT && त्रुटि_सं != ENODEV)
        परत crun_make_error (err, त्रुटि_सं, "open cgroup sub-directory");
    }

  परत 0;
}

स्थिर पूर्णांक
read_pids_cgroup (पूर्णांक dfd, bool recurse, pid_t **pids, माप_प्रकार *n_pids, माप_प्रकार *allocated, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  जर (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  परत ret;
}

स्थिर पूर्णांक
rmdir_all_fd (पूर्णांक dfd)
{
  cleanup_cgroup_walk संरचना cgroup_walk_s walk = { .dfd = -1 };
  पूर्णांक ret;

  ret = cgroup_walk_enter (&walk, dfd);
  जर (UNLIKELY (ret < 0))
    परत ret;

  साठी (;;)
    {
      संरचना cgroup_walk_dir_s *d;
      पूर्णांक child_dfd;

      ret = cgroup_walk_next (&walk, &d);
      जर (UNLIKELY (ret < 0))
        परत ret;
      जर (d == रिक्त)
        थांबा;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      जर (ret == 0 || त्रुटि_सं != EBUSY || d->entered)
        {
          cgroup_walk_काढा (&walk);
          सुरूठेवा;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      जर (UNLIKELY (child_dfd < 0))
        {
          जर (त्रुटि_सं == ENOENT)
            {
              cgroup_walk_काढा (&walk);
              सुरूठेवा;
            }
          परत child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      जर (UNLIKELY (ret < 0) && त्रुटि_सं != ENOENT && त्रुटि_सं != ENODEV)
        परत ret;
    }

  परत 0;
}

स्थिर पूर्णांक
rmdir_all (स्थिरांक अक्षर *path)
{
  पूर्णांक ret;
  पूर्णांक dfd = खोलो (path, O_DIRECTORY | O_CLOEXEC);
  जर (UNLIKELY (dfd < 0))
    परत dfd;

  ret = rmdir_all_fd (dfd);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत सूची_हटाओ (path);
}

स्थिर पूर्णांक
read_pids_from_path (स्थिरांक अक्षर *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = रिक्त;
  cleanup_close पूर्णांक dirfd = -1;
  माप_प्रकार n_pids, allocated;
  पूर्णांक mode;
  पूर्णांक ret;

  जर (path == रिक्त || *path == '\0')
    परत 0;

  mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (mode < 0))
    परत mode;

  निवड (mode)
    {
    स्थिती CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_मूळ, path, रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;
      थांबा;

    स्थिती CGROUP_MODE_HYBRID:
    स्थिती CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_मूळ "/memory", path, रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;
      थांबा;

    पूर्वनिर्धारित:
      परत crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  जर (dirfd < 0)
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  जर (threads && mode == CGROUP_MODE_UNIFIED)
    परत read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  परत read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

पूर्णांक
libcrun_cgroup_read_pids_from_path (स्थिरांक अक्षर *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  परत read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (अक्षर *content, अक्षर **saveptr, अक्षर **id, अक्षर **controller_list, अक्षर **path)
{
  अक्षर *it;

  it = strtok_r (content, "\n", saveptr);
  जर (it == रिक्त)
    परत false;

  जर (id)
    *id = it;

  it = म_अक्षर (it, ':');
  जर (it == रिक्त)
    परत false;
  *it++ = '\0';

  जर (controller_list)
    *controller_list = it;

  it = म_अक्षर (it, ':');
  जर (it == रिक्त)
    परत false;
  *it++ = '\0';

  जर (path)
    *path = it;

  परत true;
}

पूर्णांक
destroy_cgroup_path (स्थिरांक अक्षर *path, पूर्णांक mode, libcrun_error_t *err)
{
  bool repeat = true;
  पूर्णांक retry_मोजणी = 0;
  स्थिरांक पूर्णांक max_attempts = 500;
  पूर्णांक ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  जर (is_empty_string (path))
    परत 0;

  करा
    {
      repeat = false;

      जर (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free अक्षर *cgroup_path = रिक्त;

          ret = append_paths (&cgroup_path, err, CGROUP_मूळ, path, रिक्त);
          जर (UNLIKELY (ret < 0))
            परत ret;
          ret = सूची_हटाओ (cgroup_path);
          जर (ret < 0 && त्रुटि_सं == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              जर (ret < 0)
                {
                  जर (retry_मोजणी >= max_attempts)
                    परत crun_make_error (err, त्रुटि_सं, "cannot delete path `%s`", cgroup_path);

                  retry_मोजणी++;
                  repeat = true;
                }
            }
        }
      अन्यथा
        {
          cleanup_free अक्षर *content = रिक्त;
          माप_प्रकार content_size;
          अक्षर *controller;
          अक्षर *saveptr;
          bool has_data;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_size, err);
          जर (UNLIKELY (ret < 0))
            {
              जर (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  परत 0;
                }
              परत ret;
            }

          साठी (has_data = read_proc_cgroup (content, &saveptr, रिक्त, &controller, रिक्त);
               has_data;
               has_data = read_proc_cgroup (रिक्त, &saveptr, रिक्त, &controller, रिक्त))
            {
              cleanup_free अक्षर *cgroup_path = रिक्त;
              अक्षर *subsystem;
              जर (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              जर (mode == CGROUP_MODE_LEGACY && म_भेद (subsystem, "unified") == 0)
                सुरूठेवा;

              ret = append_paths (&cgroup_path, err, CGROUP_मूळ, subsystem, path, रिक्त);
              जर (UNLIKELY (ret < 0))
                परत ret;

              ret = सूची_हटाओ (cgroup_path);
              जर (ret < 0 && त्रुटि_सं == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  जर (ret < 0)
                    {
                      जर (retry_मोजणी >= max_attempts)
                        परत crun_make_error (err, त्रुटि_सं, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_मोजणी++;
                      repeat = true;
                    }
                }
            }
        }

      जर (repeat)
        {
          संरचना timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, रिक्त);

          ret = cgroup_killall_path (path, SIGKILL, err);
          जर (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } पर्यंत (repeat);

  परत 0;
}

पूर्णांक
chown_cgroups (स्थिरांक अक्षर *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = रिक्त;
  cleanup_free अक्षर *delegate = रिक्त;
  cleanup_close पूर्णांक dfd = -1;
  माप_प्रकार delegate_size;
  अक्षर *saveptr = रिक्त;
  अक्षर *name;
  पूर्णांक ret;

  ret = append_paths (&cgroup_path, err, CGROUP_मूळ, path, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  dfd = खोलो (cgroup_path, O_CLOEXEC | O_PATH);
  जर (UNLIKELY (dfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_size, err);
  जर (UNLIKELY (ret < 0))
    {
      जर (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          परत 0;
        }
      परत ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "cannot chown `%s`", cgroup_path);

  साठी (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (रिक्त, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      जर (UNLIKELY (ret < 0))
        {
          जर (त्रुटि_सं == ENOENT)
            सुरूठेवा;

          परत crun_make_error (err, त्रुटि_सं, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  परत 0;
}

स्थिर पूर्णांक
libcrun_cgroup_pause_unpause_with_mode (स्थिरांक अक्षर *cgroup_path, पूर्णांक cgroup_mode, स्थिरांक bool छोड़ो,
                                        libcrun_error_t *err)
{
  cleanup_free अक्षर *path = रिक्त;
  स्थिरांक अक्षर *state = "";
  पूर्णांक ret;

  जर (cgroup_path == रिक्त || cgroup_path[0] == '\0')
    परत crun_make_error (err, 0, "cannot %s the container without a cgroup", छोड़ो ? "pause" : "resume");

  जर (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = छोड़ो ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_मूळ, cgroup_path, "cgroup.freeze", रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  अन्यथा
    {
      state = छोड़ो ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_मूळ "/freezer", cgroup_path, "freezer.state", रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, म_माप (state), err);
  जर (ret >= 0)
    परत 0;
  परत ret;
}

पूर्णांक
libcrun_cgroup_pause_unpause_path (स्थिरांक अक्षर *cgroup_path, स्थिरांक bool छोड़ो, libcrun_error_t *err)
{
  पूर्णांक cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (cgroup_mode < 0))
    परत cgroup_mode;

  परत libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, छोड़ो, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
पूर्णांक
libcrun_cgroup_ensure_not_frozen (स्थिरांक अक्षर *cgroup_path, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = रिक्त;
  cleanup_free अक्षर *path = रिक्त;
  स्थिरांक अक्षर *frozen;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (cgroup_mode < 0))
    परत cgroup_mode;

  जर (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_मूळ, cgroup_path, "cgroup.freeze", रिक्त);
    }
  अन्यथा
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_मूळ "/freezer", cgroup_path, "freezer.state", रिक्त);
    }
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = read_all_file (path, &content, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      जर (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          परत 0;
        }
      परत ret;
    }

  जर (म_माला (content, frozen))
    परत crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  परत 0;
}

#define CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
स्थिर पूर्णांक
watch_cgroup_events (स्थिरांक अक्षर *path)
{
  cleanup_free अक्षर *events_file = रिक्त;
  cleanup_close पूर्णांक fd = -1;
  libcrun_error_t tmp_err = रिक्त;
  पूर्णांक ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_मूळ, path, "cgroup.events", रिक्त);
  जर (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      परत -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  जर (UNLIKELY (fd < 0))
    परत -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  जर (UNLIKELY (ret < 0))
    परत -1;

  ret = fd;
  fd = -1;
  परत ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
स्थिर पूर्णांक
wait_cgroup_unpopulated (स्थिरांक अक्षर *path, पूर्णांक inotify_fd)
{
  cleanup_free अक्षर *events_file = रिक्त;
  libcrun_error_t tmp_err = रिक्त;
  संरचना timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  पूर्णांक ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_मूळ, path, "cgroup.events", रिक्त);
  जर (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      परत 0;
    }

  साठी (;;)
    {
      cleanup_free अक्षर *content = रिक्त;
      अक्षर buf[4096];
      संरचना pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, रिक्त, &tmp_err);
      जर (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          परत 0;
        }

      जर (म_माला (content, "populated 0"))
        परत 1;

      ret = TEMP_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, रिक्त));
      जर (ret <= 0)
        परत 0;

      पर्यंत (पठ (inotify_fd, buf, आकार (buf)) > 0)
        ;
    }
}

पूर्णांक
cgroup_killall_path (स्थिरांक अक्षर *path, पूर्णांक संकेत, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close पूर्णांक inotify_fd = -1;
  पूर्णांक ret;
  माप_प्रकार i;
  cleanup_free pid_t *pids = रिक्त;

  जर (path == रिक्त || *path == '\0')
    परत 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  जर (संकेत == SIGKILL)
    {
      cleanup_free अक्षर *kill_file = रिक्त;

      ret = append_paths (&kill_file, err, CGROUP_मूळ, path, "cgroup.kill", रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      जर (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        परत 0;

      जर (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  जर (UNLIKELY (ret < 0))
    crun_error_release (err);
  अन्यथा
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, संकेत == SIGKILL, &pids, err);
  जर (UNLIKELY (ret < 0))
    {
      जर (crun_error_get_errno (err) != ENOENT)
        जा thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  साठी (i = 0; pids && pids[i]; i++)
    {
      ret = समाप्त (pids[i], संकेत);
      जर (UNLIKELY (ret < 0 && त्रुटि_सं != ESRCH))
        {
          ret = crun_make_error (err, त्रुटि_सं, "kill process `%d`", pids[i]);
          जा thaw;
        }
    }

  ret = 0;

thaw:
  जर (frozen)
    {
      libcrun_error_t thaw_err = रिक्त;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      जर (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  परत ret;
}

स्थिर पूर्णांक
read_available_controllers (स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *controllers = रिक्त;
  cleanup_free अक्षर *buf = रिक्त;
  अक्षर *saveptr = रिक्त;
  स्थिरांक अक्षर *token;
  पूर्णांक available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_मूळ, path, "cgroup.controllers", रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = read_all_file (controllers, &buf, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  साठी (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (रिक्त, " \n", &saveptr))
    {
      जर (म_भेद (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      अन्यथा जर (म_भेद (token, "cpu") == 0)
        available |= CGROUP_CPU;
      अन्यथा जर (म_भेद (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      अन्यथा जर (म_भेद (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      अन्यथा जर (म_भेद (token, "pids") == 0)
        available |= CGROUP_PIDS;
      अन्यथा जर (म_भेद (token, "io") == 0)
        available |= CGROUP_IO;
      अन्यथा जर (म_भेद (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  परत available;
}

स्थिर पूर्णांक
write_controller_file (स्थिरांक अक्षर *path, पूर्णांक controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free अक्षर *subtree_control = रिक्त;
  cleanup_free अक्षर *controllers = रिक्त;
  माप_प्रकार controllers_len = 0;
  पूर्णांक ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_मूळ, path, "cgroup.subtree_control", रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  जर (UNLIKELY (ret < 0))
    {
      cleanup_free अक्षर *controllers_copy = xmalloc (controllers_len + 1);
      पूर्णांक attempts_डावा;
      अक्षर *saveptr = रिक्त;
      स्थिरांक अक्षर *token;
      पूर्णांक e;

      e = crun_error_get_errno (err);
      जर (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        परत crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      जर (e == ENOENT)
        {
          libcrun_error_t tmp_err = रिक्त;
          पूर्णांक exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          जर (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              परत ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          जर (exists == 0)
            परत ret;
        }

      crun_error_release (err);

      जर (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          जर (UNLIKELY (ret < 0))
            परत crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      साठी (attempts_डावा = 1000; attempts_डावा >= 0; attempts_डावा--)
        {
          पूर्णांक controllers_written;
          bool repeat = false;

          स_नकल (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          साठी (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (रिक्त, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, म_माप (token), err);
              जर (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  जर (e == EBUSY)
                    repeat = true;

                  सुरूठेवा;
                }

              controllers_written++;
            }

          जर (! repeat)
            थांबा;

          /* If there was any controller written, just try once more without any delay.  */
          जर (controllers_written > 0 && attempts_डावा > 2)
            {
              attempts_डावा = 1;
              सुरूठेवा;
            }

          जर (attempts_डावा > 0)
            {
              संरचना timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, रिक्त);
            }
        }

      /* Refresh what controllers are available.  */
      परत read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  परत controllers_to_enable;
}

पूर्णांक
enable_controllers (स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *tmp_path = रिक्त;
  अक्षर *it;
  पूर्णांक ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  जर (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      जर ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  साठी (it = म_अक्षर (tmp_path + 1, '/'); it;)
    {
      cleanup_free अक्षर *cgroup_path = रिक्त;
      अक्षर *next_slash = म_अक्षर (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_मूळ, tmp_path, रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = सूची_गढ़ो (cgroup_path, 0755);
      जर (UNLIKELY (ret < 0 && त्रुटि_सं != EEXIST))
        परत crun_make_error (err, त्रुटि_सं, "create `%s`", cgroup_path);

      जर (next_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }

      *it = '/';
      it = next_slash;
    }
  परत 0;
}

पूर्णांक
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, स्थिरांक अक्षर *path, bool create_if_missing, libcrun_error_t *err)
{
  पूर्णांक cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (cgroup_mode < 0))
    परत cgroup_mode;

  जर (path == रिक्त || *path == '\0')
    परत 0;

  परत enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
पूर्णांक
libcrun_move_self_to_cgroups (स्थिरांक अक्षर *content, libcrun_error_t *err)
{
  cleanup_free अक्षर *buf = xstrdup (content);
  अक्षर *saveptr = रिक्त;
  अक्षर *controller;
  अक्षर *path;
  पूर्णांक cgroup_mode;
  bool has_data;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (cgroup_mode < 0))
    परत cgroup_mode;

  साठी (has_data = read_proc_cgroup (buf, &saveptr, रिक्त, &controller, &path);
       has_data;
       has_data = read_proc_cgroup (रिक्त, &saveptr, रिक्त, &controller, &path))
    {
      स्थिरांक अक्षर *subsystem = रिक्त;

      जर (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          जर (controller[0] != '\0')
            सुरूठेवा;
        }
      अन्यथा
        {
          cleanup_free अक्षर *subsystem_path = रिक्त;

          /* Use the same names as enter_cgroup_v1 does.  */
          जर (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          जर (म_भेद (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          जर (म_भेद (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_मूळ, subsystem, रिक्त);
          जर (UNLIKELY (ret < 0))
            परत ret;
          ret = crun_path_exists (subsystem_path, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          जर (ret == 0)
            सुरूठेवा;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  परत 0;
}

पूर्णांक
libcrun_get_cgroup_dirfd (संरचना libcrun_cgroup_status *status, स्थिरांक अक्षर *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free अक्षर *path_to_cgroup = रिक्त;
  पूर्णांक cgroup_mode;
  पूर्णांक cgroupdirfd;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (cgroup_mode < 0)
    परत cgroup_mode;

  जर (cgroup_mode != CGROUP_MODE_UNIFIED)
    परत crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  जर (status == रिक्त)
    परत crun_make_error (err, 0, "internal error");

  जर (is_empty_string (status->path))
    परत crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_मूळ, status->path, sub_cgroup, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  cgroupdirfd = खोलो (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  जर (UNLIKELY (cgroupdirfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", path_to_cgroup);

  परत cgroupdirfd;
}

पूर्णांक
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, अक्षर *from, अक्षर *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = रिक्त;
  cleanup_close पूर्णांक child_dfd = -1;
  पूर्णांक cgroup_mode;
  माप_प्रकार from_len;
  माप_प्रकार i;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (cgroup_mode < 0)
    परत cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  जर (UNLIKELY (ret < 0))
    जा thaw;

  from_len = म_माप (from);

  साठी (i = 0; pids && pids[i]; i++)
    {
      cleanup_free अक्षर *pid_path = रिक्त;
      cleanup_free अक्षर *dest_cgroup = रिक्त;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      जर (UNLIKELY (ret < 0))
        जा thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      जर (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          जा thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      जर (UNLIKELY (ret < 0))
        जा thaw;
    }

  ret = 0;

thaw:
  जर (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = रिक्त;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      जर (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      परत ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत destroy_cgroup_path (from, cgroup_mode, err);
}

पूर्णांक
get_cgroup_dirfd_path (पूर्णांक dirfd, अक्षर **path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = रिक्त;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  जर (UNLIKELY (len < 0))
    परत len;

  जर (has_prefix (cgroup_path, CGROUP_मूळ))
    {
      *path = xstrdup (cgroup_path + म_माप (CGROUP_मूळ));
      परत 0;
    }
  परत crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
पूर्णांक
parse_cpu_max (स्थिरांक अक्षर *मूल्य, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  स्थिरांक अक्षर *it = मूल्य;
  अक्षर *endptr = रिक्त;

  पर्यंत (*it == ' ')
    it++;

  त्रुटि_सं = 0;
  जर (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (अक्षर *) it + 3;
    }
  अन्यथा
    {
      *quota = strtoll (it, &endptr, 10);
      जर (UNLIKELY (त्रुटि_सं != 0 || endptr == it || *quota <= 0))
        परत crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", मूल्य);
    }

  it = endptr;
  पर्यंत (*it == ' ')
    it++;

  *has_period = false;
  जर (*it && *it != '\n')
    {
      त्रुटि_सं = 0;
      *period = strtoull (it, &endptr, 10);
      जर (UNLIKELY (त्रुटि_सं != 0 || endptr == it || (*endptr && *endptr != '\n')))
        परत crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", मूल्य);
      *has_period = true;
    }

  परत 0;
}
