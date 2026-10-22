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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "cgroup.h"
#समावेश "cgroup-internal.h"
#समावेश "cgroup-systemd.h"
#समावेश "cgroup-utils.h"
#समावेश "cgroup-setup.h"
#समावेश "ebpf.h"
#समावेश "utils.h"
#समावेश "status.h"
#समावेश <string.h>
#समावेश <sys/types.h>
#समावेश <signal.h>
#समावेश <sys/vfs.h>
#समावेश <inttypes.h>
#समावेश <time.h>
#समावेश <poll.h>
#समावेश <sys/inotify.h>

#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#समावेश <fcntl.h>
#समावेश <libgen.h>

काष्ठा symlink_s
{
  स्थिर अक्षर *name;
  स्थिर अक्षर *target;
};

अटल काष्ठा symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { शून्य, शून्य } };

पूर्णांक
libcrun_cgroups_create_symlinks (पूर्णांक dirfd, libcrun_error_t *err)
{
  पूर्णांक i;

  क्रम (i = 0; cgroup_symlinks[i].name; i++)
    {
      पूर्णांक ret;

      ret = symlinkat (cgroup_symlinks[i].target, dirfd, cgroup_symlinks[i].name);
      अगर (UNLIKELY (ret < 0))
        {
          अगर (त्रुटि_सं == ENOENT || त्रुटि_सं == EEXIST)
            जारी;
          वापस crun_make_error (err, त्रुटि_सं, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  वापस 0;
}

पूर्णांक
maybe_make_cgroup_threaded (स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path_type = शून्य;
  स्थिर अक्षर *स्थिर threaded = "threaded";
  cleanup_free अक्षर *content = शून्य;
  cleanup_free अक्षर *बफर = शून्य;
  स्थिर अक्षर *parent;
  माप_प्रकार आकार;
  अक्षर *it;
  पूर्णांक ret;

  अगर (path == शून्य)
    वापस 0;

  path = consume_slashes (path);

  अगर (path[0] == '\0')
    वापस 0;

  ret = append_paths (&cgroup_path_type, err, CGROUP_मूल, path, "cgroup.type", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = read_all_file (cgroup_path_type, &content, &आकार, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (आकार > 0)
    {
      it = content + आकार - 1;
      जबतक (*it == '\n' && it > content)
        *it-- = '\0';
    }

  अगर (म_भेद (content, "domain") == 0 || म_भेद (content, "domain threaded") == 0)
    वापस 0;

  बफर = xstrdup (path);
  parent = consume_slashes (स_नाम (बफर));
  अगर (parent[0] && म_भेद (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      अगर (ret < 0)
        वापस ret;
    }
  वापस write_cgroup_file (AT_FDCWD, cgroup_path_type, threaded, म_माप (threaded), err);
}

पूर्णांक
move_process_to_cgroup (pid_t pid, स्थिर अक्षर *subsystem, स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path_procs = शून्य;
  अक्षर pid_str[16];
  पूर्णांक ret;

  ret = append_paths (&cgroup_path_procs, err, CGROUP_मूल,
                      subsystem ? subsystem : "", path ? path : "",
                      "cgroup.procs", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = snprintf (pid_str, माप (pid_str), "%d", pid);
  अगर (UNLIKELY (ret >= (पूर्णांक) माप (pid_str)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, म_माप (pid_str), err);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = शून्य;
          पूर्णांक mode;

          mode = libcrun_get_cgroup_mode (&tmp_err);
          अगर (UNLIKELY (mode < 0 || mode != CGROUP_MODE_UNIFIED))
            {
              crun_error_release (&tmp_err);
              वापस ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (path, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          वापस write_cgroup_file (AT_FDCWD, cgroup_path_procs, pid_str, म_माप (pid_str), err);
        }

      वापस ret;
    }
  वापस ret;
}

#अगर_अघोषित CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#पूर्ण_अगर

#अगर_अघोषित TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#पूर्ण_अगर

अटल पूर्णांक
detect_cgroup_mode (libcrun_error_t *err)
{
  काष्ठा statfs stat;
  पूर्णांक ret;

  ret = statfs (CGROUP_मूल, &stat);
  अगर (ret < 0)
    वापस crun_make_error (err, त्रुटि_सं, "statfs `" CGROUP_मूल "`");
  अगर (stat.f_type == CGROUP2_SUPER_MAGIC)
    वापस CGROUP_MODE_UNIFIED;
  अगर (stat.f_type != TMPFS_MAGIC)
    वापस crun_make_error (err, 0, "invalid file system type on `" CGROUP_मूल "`");
  ret = statfs (CGROUP_मूल "/unified", &stat);
  अगर (ret < 0 && त्रुटि_सं != ENOENT)
    वापस crun_make_error (err, त्रुटि_सं, "statfs `" CGROUP_मूल "/unified`");
  अगर (ret < 0)
    वापस CGROUP_MODE_LEGACY;
  वापस stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_MODE_HYBRID : CGROUP_MODE_LEGACY;
}

पूर्णांक
libcrun_get_cgroup_mode (libcrun_error_t *err)
{
  पूर्णांक tmp;
  अटल पूर्णांक cgroup_mode = 0;

  अगर (cgroup_mode)
    वापस cgroup_mode;

  tmp = detect_cgroup_mode (err);
  अगर (UNLIKELY (tmp < 0))
    वापस tmp;

  cgroup_mode = tmp;

  वापस cgroup_mode;
}

अटल पूर्णांक
get_cgroup_process_from_content (अक्षर *content, पूर्णांक cgroup_mode, अक्षर **path, bool absolute, libcrun_error_t *err)
{
  अक्षर *cg_path = शून्य;
  अक्षर *controller;
  अक्षर *saveptr;
  bool has_तथ्य;

  क्रम (has_तथ्य = read_proc_cgroup (content, &saveptr, शून्य, &controller, &cg_path);
       has_तथ्य;
       has_तथ्य = read_proc_cgroup (शून्य, &saveptr, शून्य, &controller, &cg_path))
    {
      अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          अगर (म_भेद (controller, "") == 0 && म_माप (cg_path) > 0)
            जाओ found;
        }
      अन्यथा
        {
          अगर (म_भेद (controller, "memory"))
            जाओ found;
        }
    }

  वापस crun_make_error (err, 0, "cannot find cgroup for the process");

found:
  अगर (absolute)
    वापस append_paths (path, err, CGROUP_मूल, cg_path, शून्य);

  *path = xstrdup (cg_path);
  वापस 0;
}

पूर्णांक
libcrun_get_cgroup_process_at (पूर्णांक dirfd, अक्षर **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = शून्य;
  माप_प्रकार content_आकार;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_आकार, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

पूर्णांक
libcrun_get_cgroup_process (pid_t pid, अक्षर **path, bool absolute, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = शून्य;
  अक्षर proc_cgroup_file[64];
  माप_प्रकार content_आकार;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  अगर (pid == 0)
    म_नकल (proc_cgroup_file, PROC_SELF_CGROUP);
  अन्यथा
    {
      पूर्णांक len = snprintf (proc_cgroup_file, माप (proc_cgroup_file), "/proc/%d/cgroup", pid);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (proc_cgroup_file)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_आकार, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस get_cgroup_process_from_content (content, cgroup_mode, path, absolute, err);
}

अटल पूर्णांक
read_pids_file (पूर्णांक dfd, स्थिर अक्षर *file, pid_t **pids, माप_प्रकार *n_pids, माप_प्रकार *allocated, libcrun_error_t *err)
{
  cleanup_close पूर्णांक tasksfd = -1;
  cleanup_free अक्षर *बफर = शून्य;
  अक्षर *saveptr = शून्य;
  माप_प्रकार n_new_pids;
  माप_प्रकार len;
  अक्षर *it;
  पूर्णांक ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  अगर (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      अगर (त्रुटि_सं == ENOENT || त्रुटि_सं == ENODEV)
        वापस 0;
      वापस crun_make_error (err, त्रुटि_सं, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &बफर, &len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (len > 0)
    {
      क्रम (n_new_pids = 0, it = बफर; it; it = म_अक्षर (it + 1, '\n'))
        n_new_pids++;

      अगर (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, माप (pid_t) * *allocated);
        }

      क्रम (it = strtok_r (बफर, "\n", &saveptr); it; it = strtok_r (शून्य, "\n", &saveptr))
        {
          pid_t pid = म_से_अदीर्घ (it, शून्य, 10);

          अगर (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  वापस 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
काष्ठा cgroup_walk_dir_s
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
काष्ठा cgroup_walk_s
{
  पूर्णांक dfd;
  माप_प्रकार depth;
  काष्ठा cgroup_walk_dir_s *dirs;
  माप_प्रकार n_dirs;
  माप_प्रकार allocated;
};

अटल व्योम
cgroup_walk_निकालो (काष्ठा cgroup_walk_s *walk)
{
  मुक्त (walk->dirs[--walk->n_dirs].name);
}

अटल व्योम
cleanup_cgroup_walkp (काष्ठा cgroup_walk_s *walk)
{
  पूर्णांक saved_errno = त्रुटि_सं;

  जबतक (walk->n_dirs > 0)
    cgroup_walk_निकालो (walk);
  मुक्त (walk->dirs);
  close_and_reset (&walk->dfd);
  त्रुटि_सं = saved_errno;
}

#घोषणा cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
अटल पूर्णांक
cgroup_walk_enter (काष्ठा cgroup_walk_s *walk, पूर्णांक dfd)
{
  cleanup_close पूर्णांक dfd_clone = -1;
  cleanup_dir सूची *dir = शून्य;
  काष्ठा dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->depth++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  अगर (UNLIKELY (dfd_clone < 0))
    वापस -1;

  dir = fdopendir (dfd_clone);
  अगर (UNLIKELY (dir == शून्य))
    वापस -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  क्रम (de = सूची_पढ़ो (dir); de; de = सूची_पढ़ो (dir))
    {
      काष्ठा cgroup_walk_dir_s *d;

      अगर (म_भेद (de->d_name, ".") == 0 || म_भेद (de->d_name, "..") == 0)
        जारी;

      अगर (de->d_type != DT_DIR)
        जारी;

      अगर (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, माप (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->depth = walk->depth + 1;
      d->entered = false;
    }

  वापस 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
अटल पूर्णांक
cgroup_walk_अग्र (काष्ठा cgroup_walk_s *walk, काष्ठा cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : शून्य;

  जबतक (*d && walk->depth >= (*d)->depth)
    {
      पूर्णांक parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_PATH | O_CLOEXEC);
      अगर (UNLIKELY (parent < 0))
        वापस -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->depth--;
    }

  वापस 0;
}

अटल पूर्णांक
read_pids_cgroup_file (पूर्णांक dfd, स्थिर अक्षर *file, bool recurse, pid_t **pids, माप_प्रकार *n_pids, माप_प्रकार *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk काष्ठा cgroup_walk_s walk = { .dfd = -1 };
  पूर्णांक dfd_clone;
  पूर्णांक ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (! recurse)
    वापस 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (dfd_clone < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open cgroup sub-directory");

  क्रम (;;)
    {
      काष्ठा cgroup_walk_dir_s *d;
      cleanup_close पूर्णांक nfd = -1;

      ret = cgroup_walk_अग्र (&walk, &d);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open parent cgroup directory");
      अगर (d == शून्य)
        अवरोध;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_PATH | O_CLOEXEC);
      अगर (UNLIKELY (nfd < 0))
        {
          पूर्णांक saved_errno = त्रुटि_सं;

          अगर (saved_errno == ENOENT)
            {
              cgroup_walk_निकालो (&walk);
              जारी;
            }
          वापस crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_निकालो (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      अगर (UNLIKELY (ret < 0))
        {
          पूर्णांक e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          अगर (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            वापस ret;
          crun_error_release (err);
          जारी;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      अगर (UNLIKELY (ret < 0) && त्रुटि_सं != ENOENT && त्रुटि_सं != ENODEV)
        वापस crun_make_error (err, त्रुटि_सं, "open cgroup sub-directory");
    }

  वापस 0;
}

अटल पूर्णांक
read_pids_cgroup (पूर्णांक dfd, bool recurse, pid_t **pids, माप_प्रकार *n_pids, माप_प्रकार *allocated, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  अगर (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  वापस ret;
}

अटल पूर्णांक
rmdir_all_fd (पूर्णांक dfd)
{
  cleanup_cgroup_walk काष्ठा cgroup_walk_s walk = { .dfd = -1 };
  पूर्णांक ret;

  ret = cgroup_walk_enter (&walk, dfd);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  क्रम (;;)
    {
      काष्ठा cgroup_walk_dir_s *d;
      पूर्णांक child_dfd;

      ret = cgroup_walk_अग्र (&walk, &d);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      अगर (d == शून्य)
        अवरोध;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      अगर (ret == 0 || त्रुटि_सं != EBUSY || d->entered)
        {
          cgroup_walk_निकालो (&walk);
          जारी;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      अगर (UNLIKELY (child_dfd < 0))
        {
          अगर (त्रुटि_सं == ENOENT)
            {
              cgroup_walk_निकालो (&walk);
              जारी;
            }
          वापस child_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, child_dfd);
      अगर (UNLIKELY (ret < 0) && त्रुटि_सं != ENOENT && त्रुटि_सं != ENODEV)
        वापस ret;
    }

  वापस 0;
}

अटल पूर्णांक
rmdir_all (स्थिर अक्षर *path)
{
  पूर्णांक ret;
  पूर्णांक dfd = खोलो (path, O_DIRECTORY | O_CLOEXEC);
  अगर (UNLIKELY (dfd < 0))
    वापस dfd;

  ret = rmdir_all_fd (dfd);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस सूची_हटाओ (path);
}

अटल पूर्णांक
read_pids_from_path (स्थिर अक्षर *path, bool recurse, bool threads, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = शून्य;
  cleanup_close पूर्णांक dirfd = -1;
  माप_प्रकार n_pids, allocated;
  पूर्णांक mode;
  पूर्णांक ret;

  अगर (path == शून्य || *path == '\0')
    वापस 0;

  mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (mode < 0))
    वापस mode;

  चयन (mode)
    {
    हाल CGROUP_MODE_UNIFIED:
      ret = append_paths (&cgroup_path, err, CGROUP_मूल, path, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      अवरोध;

    हाल CGROUP_MODE_HYBRID:
    हाल CGROUP_MODE_LEGACY:
      ret = append_paths (&cgroup_path, err, CGROUP_मूल "/memory", path, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      अवरोध;

    शेष:
      वापस crun_make_error (err, 0, "invalid cgroup mode `%d`", mode);
    }

  dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (dirfd < 0)
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  अगर (threads && mode == CGROUP_MODE_UNIFIED)
    वापस read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  वापस read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

पूर्णांक
libcrun_cgroup_read_pids_from_path (स्थिर अक्षर *path, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  वापस read_pids_from_path (path, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (अक्षर *content, अक्षर **saveptr, अक्षर **id, अक्षर **controller_list, अक्षर **path)
{
  अक्षर *it;

  it = strtok_r (content, "\n", saveptr);
  अगर (it == शून्य)
    वापस false;

  अगर (id)
    *id = it;

  it = म_अक्षर (it, ':');
  अगर (it == शून्य)
    वापस false;
  *it++ = '\0';

  अगर (controller_list)
    *controller_list = it;

  it = म_अक्षर (it, ':');
  अगर (it == शून्य)
    वापस false;
  *it++ = '\0';

  अगर (path)
    *path = it;

  वापस true;
}

पूर्णांक
destroy_cgroup_path (स्थिर अक्षर *path, पूर्णांक mode, libcrun_error_t *err)
{
  bool repeat = true;
  पूर्णांक retry_गिनती = 0;
  स्थिर पूर्णांक max_attempts = 500;
  पूर्णांक ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  अगर (is_empty_string (path))
    वापस 0;

  करो
    {
      repeat = false;

      अगर (mode == CGROUP_MODE_UNIFIED)
        {
          cleanup_free अक्षर *cgroup_path = शून्य;

          ret = append_paths (&cgroup_path, err, CGROUP_मूल, path, शून्य);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          ret = सूची_हटाओ (cgroup_path);
          अगर (ret < 0 && त्रुटि_सं == EBUSY)
            {
              ret = rmdir_all (cgroup_path);
              अगर (ret < 0)
                {
                  अगर (retry_गिनती >= max_attempts)
                    वापस crun_make_error (err, त्रुटि_सं, "cannot delete path `%s`", cgroup_path);

                  retry_गिनती++;
                  repeat = true;
                }
            }
        }
      अन्यथा
        {
          cleanup_free अक्षर *content = शून्य;
          माप_प्रकार content_आकार;
          अक्षर *controller;
          अक्षर *saveptr;
          bool has_तथ्य;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_आकार, err);
          अगर (UNLIKELY (ret < 0))
            {
              अगर (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  वापस 0;
                }
              वापस ret;
            }

          क्रम (has_तथ्य = read_proc_cgroup (content, &saveptr, शून्य, &controller, शून्य);
               has_तथ्य;
               has_तथ्य = read_proc_cgroup (शून्य, &saveptr, शून्य, &controller, शून्य))
            {
              cleanup_free अक्षर *cgroup_path = शून्य;
              अक्षर *subsystem;
              अगर (has_prefix (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              अगर (mode == CGROUP_MODE_LEGACY && म_भेद (subsystem, "unified") == 0)
                जारी;

              ret = append_paths (&cgroup_path, err, CGROUP_मूल, subsystem, path, शून्य);
              अगर (UNLIKELY (ret < 0))
                वापस ret;

              ret = सूची_हटाओ (cgroup_path);
              अगर (ret < 0 && त्रुटि_सं == EBUSY)
                {
                  ret = rmdir_all (cgroup_path);
                  अगर (ret < 0)
                    {
                      अगर (retry_गिनती >= max_attempts)
                        वापस crun_make_error (err, त्रुटि_सं, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_path);
                      retry_गिनती++;
                      repeat = true;
                    }
                }
            }
        }

      अगर (repeat)
        {
          काष्ठा timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, शून्य);

          ret = cgroup_killall_path (path, SIGKILL, err);
          अगर (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } जबतक (repeat);

  वापस 0;
}

पूर्णांक
chown_cgroups (स्थिर अक्षर *path, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = शून्य;
  cleanup_free अक्षर *delegate = शून्य;
  cleanup_close पूर्णांक dfd = -1;
  माप_प्रकार delegate_आकार;
  अक्षर *saveptr = शून्य;
  अक्षर *name;
  पूर्णांक ret;

  ret = append_paths (&cgroup_path, err, CGROUP_मूल, path, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  dfd = खोलो (cgroup_path, O_CLOEXEC | O_PATH);
  अगर (UNLIKELY (dfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_आकार, err);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          वापस 0;
        }
      वापस ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "cannot chown `%s`", cgroup_path);

  क्रम (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (शून्य, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      अगर (UNLIKELY (ret < 0))
        {
          अगर (त्रुटि_सं == ENOENT)
            जारी;

          वापस crun_make_error (err, त्रुटि_सं, "cannot chown `%s/%s`", cgroup_path, name);
        }
    }

  वापस 0;
}

अटल पूर्णांक
libcrun_cgroup_pause_unpause_with_mode (स्थिर अक्षर *cgroup_path, पूर्णांक cgroup_mode, स्थिर bool छोड़ो,
                                        libcrun_error_t *err)
{
  cleanup_free अक्षर *path = शून्य;
  स्थिर अक्षर *state = "";
  पूर्णांक ret;

  अगर (cgroup_path == शून्य || cgroup_path[0] == '\0')
    वापस crun_make_error (err, 0, "cannot %s the container without a cgroup", छोड़ो ? "pause" : "resume");

  अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = छोड़ो ? "1" : "0";
      ret = append_paths (&path, err, CGROUP_मूल, cgroup_path, "cgroup.freeze", शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      state = छोड़ो ? "FROZEN" : "THAWED";
      ret = append_paths (&path, err, CGROUP_मूल "/freezer", cgroup_path, "freezer.state", शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = write_cgroup_file (AT_FDCWD, path, state, म_माप (state), err);
  अगर (ret >= 0)
    वापस 0;
  वापस ret;
}

पूर्णांक
libcrun_cgroup_pause_unpause_path (स्थिर अक्षर *cgroup_path, स्थिर bool छोड़ो, libcrun_error_t *err)
{
  पूर्णांक cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  वापस libcrun_cgroup_pause_unpause_with_mode (cgroup_path, cgroup_mode, छोड़ो, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
पूर्णांक
libcrun_cgroup_ensure_not_frozen (स्थिर अक्षर *cgroup_path, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = शून्य;
  cleanup_free अक्षर *path = शून्य;
  स्थिर अक्षर *frozen;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&path, err, CGROUP_मूल, cgroup_path, "cgroup.freeze", शून्य);
    }
  अन्यथा
    {
      frozen = "FROZEN";
      ret = append_paths (&path, err, CGROUP_मूल "/freezer", cgroup_path, "freezer.state", शून्य);
    }
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = read_all_file (path, &content, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      अगर (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          वापस 0;
        }
      वापस ret;
    }

  अगर (म_माला (content, frozen))
    वापस crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  वापस 0;
}

#घोषणा CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
अटल पूर्णांक
watch_cgroup_events (स्थिर अक्षर *path)
{
  cleanup_free अक्षर *events_file = शून्य;
  cleanup_close पूर्णांक fd = -1;
  libcrun_error_t tmp_err = शून्य;
  पूर्णांक ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_मूल, path, "cgroup.events", शून्य);
  अगर (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      वापस -1;
    }

  fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  अगर (UNLIKELY (fd < 0))
    वापस -1;

  ret = inotify_add_watch (fd, events_file, IN_MODIFY);
  अगर (UNLIKELY (ret < 0))
    वापस -1;

  ret = fd;
  fd = -1;
  वापस ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
अटल पूर्णांक
wait_cgroup_unpopulated (स्थिर अक्षर *path, पूर्णांक inotify_fd)
{
  cleanup_free अक्षर *events_file = शून्य;
  libcrun_error_t tmp_err = शून्य;
  काष्ठा timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  पूर्णांक ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_मूल, path, "cgroup.events", शून्य);
  अगर (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      वापस 0;
    }

  क्रम (;;)
    {
      cleanup_free अक्षर *content = शून्य;
      अक्षर buf[4096];
      काष्ठा pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, शून्य, &tmp_err);
      अगर (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          वापस 0;
        }

      अगर (म_माला (content, "populated 0"))
        वापस 1;

      ret = अस्थायी_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, शून्य));
      अगर (ret <= 0)
        वापस 0;

      जबतक (पढ़ो (inotify_fd, buf, माप (buf)) > 0)
        ;
    }
}

पूर्णांक
cgroup_killall_path (स्थिर अक्षर *path, पूर्णांक संकेत, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close पूर्णांक inotify_fd = -1;
  पूर्णांक ret;
  माप_प्रकार i;
  cleanup_free pid_t *pids = शून्य;

  अगर (path == शून्य || *path == '\0')
    वापस 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  अगर (संकेत == SIGKILL)
    {
      cleanup_free अक्षर *kill_file = शून्य;

      ret = append_paths (&kill_file, err, CGROUP_मूल, path, "cgroup.kill", शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      inotify_fd = watch_cgroup_events (path);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      अगर (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (path, inotify_fd))
        वापस 0;

      अगर (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_path (path, true, err);
  अगर (UNLIKELY (ret < 0))
    crun_error_release (err);
  अन्यथा
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_path (path, true, संकेत == SIGKILL, &pids, err);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (crun_error_get_errno (err) != ENOENT)
        जाओ thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  क्रम (i = 0; pids && pids[i]; i++)
    {
      ret = समाप्त (pids[i], संकेत);
      अगर (UNLIKELY (ret < 0 && त्रुटि_सं != ESRCH))
        {
          ret = crun_make_error (err, त्रुटि_सं, "kill process `%d`", pids[i]);
          जाओ thaw;
        }
    }

  ret = 0;

thaw:
  अगर (frozen)
    {
      libcrun_error_t thaw_err = शून्य;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      अगर (libcrun_cgroup_pause_unpause_path (path, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  वापस ret;
}

अटल पूर्णांक
read_available_controllers (स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *controllers = शून्य;
  cleanup_free अक्षर *buf = शून्य;
  अक्षर *saveptr = शून्य;
  स्थिर अक्षर *token;
  पूर्णांक available = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_मूल, path, "cgroup.controllers", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = read_all_file (controllers, &buf, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  क्रम (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (शून्य, " \n", &saveptr))
    {
      अगर (म_भेद (token, "memory") == 0)
        available |= CGROUP_MEMORY;
      अन्यथा अगर (म_भेद (token, "cpu") == 0)
        available |= CGROUP_CPU;
      अन्यथा अगर (म_भेद (token, "cpuset") == 0)
        available |= CGROUP_CPUSET;
      अन्यथा अगर (म_भेद (token, "hugetlb") == 0)
        available |= CGROUP_HUGETLB;
      अन्यथा अगर (म_भेद (token, "pids") == 0)
        available |= CGROUP_PIDS;
      अन्यथा अगर (म_भेद (token, "io") == 0)
        available |= CGROUP_IO;
      अन्यथा अगर (म_भेद (token, "misc") == 0)
        available |= CGROUP_MISC;
    }
  वापस available;
}

अटल पूर्णांक
write_controller_file (स्थिर अक्षर *path, पूर्णांक controllers_to_enable, libcrun_error_t *err)
{
  cleanup_free अक्षर *subtree_control = शून्य;
  cleanup_free अक्षर *controllers = शून्य;
  माप_प्रकार controllers_len = 0;
  पूर्णांक ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_to_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_to_enable & CGROUP_IO) ? "+io" : "", (controllers_to_enable & CGROUP_MEMORY) ? "+memory" : "",
      (controllers_to_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_to_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_to_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_to_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_मूल, path, "cgroup.subtree_control", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  अगर (UNLIKELY (ret < 0))
    {
      cleanup_free अक्षर *controllers_copy = xmalloc (controllers_len + 1);
      पूर्णांक attempts_बायाँ;
      अक्षर *saveptr = शून्य;
      स्थिर अक्षर *token;
      पूर्णांक e;

      e = crun_error_get_errno (err);
      अगर (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        वापस crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      अगर (e == ENOENT)
        {
          libcrun_error_t tmp_err = शून्य;
          पूर्णांक exists;

          exists = crun_path_exists (subtree_control, &tmp_err);
          अगर (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              वापस ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          अगर (exists == 0)
            वापस ret;
        }

      crun_error_release (err);

      अगर (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (path, err);
          अगर (UNLIKELY (ret < 0))
            वापस crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      क्रम (attempts_बायाँ = 1000; attempts_बायाँ >= 0; attempts_बायाँ--)
        {
          पूर्णांक controllers_written;
          bool repeat = false;

          स_नकल (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          क्रम (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (शून्य, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, म_माप (token), err);
              अगर (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  अगर (e == EBUSY)
                    repeat = true;

                  जारी;
                }

              controllers_written++;
            }

          अगर (! repeat)
            अवरोध;

          /* If there was any controller written, just try once more without any delay.  */
          अगर (controllers_written > 0 && attempts_बायाँ > 2)
            {
              attempts_बायाँ = 1;
              जारी;
            }

          अगर (attempts_बायाँ > 0)
            {
              काष्ठा timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, शून्य);
            }
        }

      /* Refresh what controllers are available.  */
      वापस read_available_controllers (path, err);
    }

  /* All controllers were enabled successfully.  */
  वापस controllers_to_enable;
}

पूर्णांक
enable_controllers (स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *tmp_path = शून्य;
  अक्षर *it;
  पूर्णांक ret, controllers_to_enable;

  xasprintf (&tmp_path, "%s/", path);

  ret = read_available_controllers ("", err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  controllers_to_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_to_enable, err);
  अगर (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      अगर ((controllers_to_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_to_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_to_enable, err);
        }
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  क्रम (it = म_अक्षर (tmp_path + 1, '/'); it;)
    {
      cleanup_free अक्षर *cgroup_path = शून्य;
      अक्षर *अग्र_slash = म_अक्षर (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_path, err, CGROUP_मूल, tmp_path, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = सूची_गढ़ो (cgroup_path, 0755);
      अगर (UNLIKELY (ret < 0 && त्रुटि_सं != EEXIST))
        वापस crun_make_error (err, त्रुटि_सं, "create `%s`", cgroup_path);

      अगर (अग्र_slash)
        {
          ret = write_controller_file (tmp_path, controllers_to_enable, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      *it = '/';
      it = अग्र_slash;
    }
  वापस 0;
}

पूर्णांक
libcrun_move_process_to_cgroup (pid_t pid, pid_t init_pid, स्थिर अक्षर *path, bool create_if_missing, libcrun_error_t *err)
{
  पूर्णांक cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  अगर (path == शून्य || *path == '\0')
    वापस 0;

  वापस enter_cgroup (cgroup_mode, pid, init_pid, path, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
पूर्णांक
libcrun_move_self_to_cgroups (स्थिर अक्षर *content, libcrun_error_t *err)
{
  cleanup_free अक्षर *buf = xstrdup (content);
  अक्षर *saveptr = शून्य;
  अक्षर *controller;
  अक्षर *path;
  पूर्णांक cgroup_mode;
  bool has_तथ्य;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  क्रम (has_तथ्य = read_proc_cgroup (buf, &saveptr, शून्य, &controller, &path);
       has_तथ्य;
       has_तथ्य = read_proc_cgroup (शून्य, &saveptr, शून्य, &controller, &path))
    {
      स्थिर अक्षर *subsystem = शून्य;

      अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          अगर (controller[0] != '\0')
            जारी;
        }
      अन्यथा
        {
          cleanup_free अक्षर *subsystem_path = शून्य;

          /* Use the same names as enter_cgroup_v1 does.  */
          अगर (has_prefix (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          अगर (म_भेद (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          अगर (म_भेद (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_path, err, CGROUP_मूल, subsystem, शून्य);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          ret = crun_path_exists (subsystem_path, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          अगर (ret == 0)
            जारी;
        }

      ret = move_process_to_cgroup (0, subsystem, path, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

पूर्णांक
libcrun_get_cgroup_dirfd (काष्ठा libcrun_cgroup_status *status, स्थिर अक्षर *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free अक्षर *path_to_cgroup = शून्य;
  पूर्णांक cgroup_mode;
  पूर्णांक cgroupdirfd;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (cgroup_mode < 0)
    वापस cgroup_mode;

  अगर (cgroup_mode != CGROUP_MODE_UNIFIED)
    वापस crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  अगर (status == शून्य)
    वापस crun_make_error (err, 0, "internal error");

  अगर (is_empty_string (status->path))
    वापस crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&path_to_cgroup, err, CGROUP_मूल, status->path, sub_cgroup, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  cgroupdirfd = खोलो (path_to_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  अगर (UNLIKELY (cgroupdirfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", path_to_cgroup);

  वापस cgroupdirfd;
}

पूर्णांक
libcrun_migrate_all_pids_to_cgroup (pid_t init_pid, अक्षर *from, अक्षर *to, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = शून्य;
  cleanup_close पूर्णांक child_dfd = -1;
  पूर्णांक cgroup_mode;
  माप_प्रकार from_len;
  माप_प्रकार i;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (cgroup_mode < 0)
    वापस cgroup_mode;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_path (from, true, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_cgroup_read_pids_from_path (from, true, &pids, err);
  अगर (UNLIKELY (ret < 0))
    जाओ thaw;

  from_len = म_माप (from);

  क्रम (i = 0; pids && pids[i]; i++)
    {
      cleanup_free अक्षर *pid_path = शून्य;
      cleanup_free अक्षर *dest_cgroup = शून्य;

      ret = libcrun_get_cgroup_process (pids[i], &pid_path, false, err);
      अगर (UNLIKELY (ret < 0))
        जाओ thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      अगर (! has_prefix (pid_path, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          जाओ thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", to, pid_path + from_len);

      ret = enter_cgroup (cgroup_mode, pids[i], init_pid, dest_cgroup, false, err);
      अगर (UNLIKELY (ret < 0))
        जाओ thaw;
    }

  ret = 0;

thaw:
  अगर (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = शून्य;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      अगर (libcrun_cgroup_pause_unpause_path (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      वापस ret;
    }

  ret = libcrun_cgroup_pause_unpause_path (from, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस destroy_cgroup_path (from, cgroup_mode, err);
}

पूर्णांक
get_cgroup_dirfd_path (पूर्णांक dirfd, अक्षर **path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = शून्य;
  proc_fd_path_t fd_path;
  ssize_t len;

  get_proc_self_fd_path (fd_path, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_path, &cgroup_path, 0, err);
  अगर (UNLIKELY (len < 0))
    वापस len;

  अगर (has_prefix (cgroup_path, CGROUP_मूल))
    {
      *path = xstrdup (cgroup_path + म_माप (CGROUP_मूल));
      वापस 0;
    }
  वापस crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_path);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
पूर्णांक
parse_cpu_max (स्थिर अक्षर *मूल्य, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  स्थिर अक्षर *it = मूल्य;
  अक्षर *endptr = शून्य;

  जबतक (*it == ' ')
    it++;

  त्रुटि_सं = 0;
  अगर (has_prefix (it, "max"))
    {
      *quota = -1;
      endptr = (अक्षर *) it + 3;
    }
  अन्यथा
    {
      *quota = strtoll (it, &endptr, 10);
      अगर (UNLIKELY (त्रुटि_सं != 0 || endptr == it || *quota <= 0))
        वापस crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", मूल्य);
    }

  it = endptr;
  जबतक (*it == ' ')
    it++;

  *has_period = false;
  अगर (*it && *it != '\n')
    {
      त्रुटि_सं = 0;
      *period = strtoull (it, &endptr, 10);
      अगर (UNLIKELY (त्रुटि_सं != 0 || endptr == it || (*endptr && *endptr != '\n')))
        वापस crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", मूल्य);
      *has_period = true;
    }

  वापस 0;
}
