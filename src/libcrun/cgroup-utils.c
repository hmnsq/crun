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

#परिभाषय _GNU_SOURCE

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
maybe_make_cgroup_threaded (ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_मार्गः_type = रिक्त;
  ध्रुव वर्ण *ध्रुव threaded = "threaded";
  cleanup_free वर्ण *content = रिक्त;
  cleanup_free वर्ण *बफर = रिक्त;
  ध्रुव वर्ण *parent;
  size_t आकार;
  वर्ण *it;
  पूर्णाङ्क ret;

  यदि (मार्गः == रिक्त)
    प्रत्यागम 0;

  मार्गः = consume_slashes (मार्गः);

  यदि (मार्गः[0] == '\0')
    प्रत्यागम 0;

  ret = append_paths (&cgroup_मार्गः_type, err, CGROUP_मूलम्, मार्गः, "cgroup.type", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file (cgroup_मार्गः_type, &content, &आकार, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (आकार > 0)
    {
      it = content + आकार - 1;
      यावत् (*it == '\n' && it > content)
        *it-- = '\0';
    }

  यदि (वाक्यं_तुलय (content, "domain") == 0 || वाक्यं_तुलय (content, "domain threaded") == 0)
    प्रत्यागम 0;

  बफर = xstrdup (मार्गः);
  parent = consume_slashes (dirname (बफर));
  यदि (parent[0] && वाक्यं_तुलय (parent, "."))
    {
      ret = maybe_make_cgroup_threaded (parent, err);
      यदि (ret < 0)
        प्रत्यागम ret;
    }
  प्रत्यागम write_cgroup_file (AT_FDCWD, cgroup_मार्गः_type, threaded, वाक्यदैर्घ्यम् (threaded), err);
}

पूर्णाङ्क
move_process_गन्तव्यम्_cgroup (pid_t pid, ध्रुव वर्ण *subsystem, ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_मार्गः_procs = रिक्त;
  वर्ण pid_str[16];
  पूर्णाङ्क ret;

  ret = append_paths (&cgroup_मार्गः_procs, err, CGROUP_मूलम्,
                      subsystem ? subsystem : "", मार्गः ? मार्गः : "",
                      "cgroup.procs", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = snprintf (pid_str, परिमाण (pid_str), "%d", pid);
  यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (pid_str)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_मार्गः_procs, pid_str, वाक्यदैर्घ्यम् (pid_str), err);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = रिक्त;
          पूर्णाङ्क बहुलकम्;

          बहुलकम् = libcrun_get_cgroup_बहुलकम् (&tmp_err);
          यदि (UNLIKELY (बहुलकम् < 0 || बहुलकम् != CGROUP_बहुलकम्_UNIFIED))
            {
              crun_error_release (&tmp_err);
              प्रत्यागम ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (मार्गः, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          प्रत्यागम write_cgroup_file (AT_FDCWD, cgroup_मार्गः_procs, pid_str, वाक्यदैर्घ्यम् (pid_str), err);
        }

      प्रत्यागम ret;
    }
  प्रत्यागम ret;
}

#यदि_अपरिभाषितम् CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#यदि_अन्तः

#यदि_अपरिभाषितम् TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#यदि_अन्तः

स्थिर पूर्णाङ्क
detect_cgroup_बहुलकम् (libcrun_error_t *err)
{
  संरचना statfs stat;
  पूर्णाङ्क ret;

  ret = statfs (CGROUP_मूलम्, &stat);
  यदि (ret < 0)
    प्रत्यागम crun_make_error (err, errno, "statfs `" CGROUP_मूलम् "`");
  यदि (stat.f_type == CGROUP2_SUPER_MAGIC)
    प्रत्यागम CGROUP_बहुलकम्_UNIFIED;
  यदि (stat.f_type != TMPFS_MAGIC)
    प्रत्यागम crun_make_error (err, 0, "invalid file system type on `" CGROUP_मूलम् "`");
  ret = statfs (CGROUP_मूलम् "/unified", &stat);
  यदि (ret < 0 && errno != ENOENT)
    प्रत्यागम crun_make_error (err, errno, "statfs `" CGROUP_मूलम् "/unified`");
  यदि (ret < 0)
    प्रत्यागम CGROUP_बहुलकम्_LEGACY;
  प्रत्यागम stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_बहुलकम्_HYBRID : CGROUP_बहुलकम्_LEGACY;
}

पूर्णाङ्क
libcrun_get_cgroup_बहुलकम् (libcrun_error_t *err)
{
  पूर्णाङ्क tmp;
  स्थिर पूर्णाङ्क cgroup_बहुलकम् = 0;

  यदि (cgroup_बहुलकम्)
    प्रत्यागम cgroup_बहुलकम्;

  tmp = detect_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (tmp < 0))
    प्रत्यागम tmp;

  cgroup_बहुलकम् = tmp;

  प्रत्यागम cgroup_बहुलकम्;
}

स्थिर पूर्णाङ्क
get_cgroup_process_from_content (वर्ण *content, पूर्णाङ्क cgroup_बहुलकम्, वर्ण **मार्गः, bool absolute, libcrun_error_t *err)
{
  वर्ण *cg_मार्गः = रिक्त;
  वर्ण *controller;
  वर्ण *saveptr;
  bool has_दत्त;

  प्रति (has_दत्त = read_proc_cgroup (content, &saveptr, रिक्त, &controller, &cg_मार्गः);
       has_दत्त;
       has_दत्त = read_proc_cgroup (रिक्त, &saveptr, रिक्त, &controller, &cg_मार्गः))
    {
      यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
        {
          यदि (वाक्यं_तुलय (controller, "") == 0 && वाक्यदैर्घ्यम् (cg_मार्गः) > 0)
            गच्छ प्राप्तम्;
        }
      अन्यथा
        {
          यदि (वाक्यं_तुलय (controller, "memory"))
            गच्छ प्राप्तम्;
        }
    }

  प्रत्यागम crun_make_error (err, 0, "cannot find cgroup for the process");

प्राप्तम्:
  यदि (absolute)
    प्रत्यागम append_paths (मार्गः, err, CGROUP_मूलम्, cg_मार्गः, रिक्त);

  *मार्गः = xstrdup (cg_मार्गः);
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_get_cgroup_process_at (पूर्णाङ्क dirfd, वर्ण **मार्गः, bool absolute, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  size_t content_आकार;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क ret;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_आकार, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम get_cgroup_process_from_content (content, cgroup_बहुलकम्, मार्गः, absolute, err);
}

पूर्णाङ्क
libcrun_get_cgroup_process (pid_t pid, वर्ण **मार्गः, bool absolute, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  वर्ण proc_cgroup_file[64];
  size_t content_आकार;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क ret;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  यदि (pid == 0)
    वाक्यं_प्रतिलिख (proc_cgroup_file, PROC_SELF_CGROUP);
  अन्यथा
    {
      पूर्णाङ्क len = snprintf (proc_cgroup_file, परिमाण (proc_cgroup_file), "/proc/%d/cgroup", pid);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (proc_cgroup_file)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_आकार, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम get_cgroup_process_from_content (content, cgroup_बहुलकम्, मार्गः, absolute, err);
}

स्थिर पूर्णाङ्क
read_pids_file (पूर्णाङ्क dfd, ध्रुव वर्ण *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क tasksfd = -1;
  cleanup_free वर्ण *बफर = रिक्त;
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

  ret = read_all_fd (tasksfd, file, &बफर, &len, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (len > 0)
    {
      प्रति (n_new_pids = 0, it = बफर; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      यदि (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, परिमाण (pid_t) * *allocated);
        }

      प्रति (it = strtok_r (बफर, "\n", &saveptr); it; it = strtok_r (रिक्त, "\n", &saveptr))
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
  size_t गभीरता;
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
  size_t गभीरता;
  संरचना cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

स्थिर शून्य
cgroup_walk_निष्कासय (संरचना cgroup_walk_s *walk)
{
  मुक्त (walk->dirs[--walk->n_dirs].name);
}

स्थिर शून्य
cleanup_cgroup_walkp (संरचना cgroup_walk_s *walk)
{
  पूर्णाङ्क saved_errno = errno;

  यावत् (walk->n_dirs > 0)
    cgroup_walk_निष्कासय (walk);
  मुक्त (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#परिभाषय cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

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
  walk->गभीरता++;

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

      यदि (वाक्यं_तुलय (de->d_name, ".") == 0 || वाक्यं_तुलय (de->d_name, "..") == 0)
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
      d->गभीरता = walk->गभीरता + 1;
      d->entered = false;
    }

  प्रत्यागम 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
स्थिर पूर्णाङ्क
cgroup_walk_अग्र (संरचना cgroup_walk_s *walk, संरचना cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : रिक्त;

  यावत् (*d && walk->गभीरता >= (*d)->गभीरता)
    {
      पूर्णाङ्क parent;

      parent = openat (walk->dfd, "..", O_DIRECTORY | O_मार्गः | O_CLOEXEC);
      यदि (UNLIKELY (parent < 0))
        प्रत्यागम -1;

      close_and_reset (&walk->dfd);
      walk->dfd = parent;
      walk->गभीरता--;
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

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_मार्गः | O_CLOEXEC);
  यदि (UNLIKELY (dfd_clone < 0))
    प्रत्यागम crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "open cgroup sub-directory");

  प्रति (;;)
    {
      संरचना cgroup_walk_dir_s *d;
      cleanup_close पूर्णाङ्क nfd = -1;

      ret = cgroup_walk_अग्र (&walk, &d);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "open parent cgroup directory");
      यदि (d == रिक्त)
        भङ्ग;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_मार्गः | O_CLOEXEC);
      यदि (UNLIKELY (nfd < 0))
        {
          पूर्णाङ्क saved_errno = errno;

          यदि (saved_errno == ENOENT)
            {
              cgroup_walk_निष्कासय (&walk);
              अनुवर्त;
            }
          प्रत्यागम crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_निष्कासय (&walk);

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

      ret = cgroup_walk_अग्र (&walk, &d);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      यदि (d == रिक्त)
        भङ्ग;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      यदि (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_निष्कासय (&walk);
          अनुवर्त;
        }

      child_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      यदि (UNLIKELY (child_dfd < 0))
        {
          यदि (errno == ENOENT)
            {
              cgroup_walk_निष्कासय (&walk);
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
rmdir_all (ध्रुव वर्ण *मार्गः)
{
  पूर्णाङ्क ret;
  पूर्णाङ्क dfd = उद्घाटय (मार्गः, O_DIRECTORY | O_CLOEXEC);
  यदि (UNLIKELY (dfd < 0))
    प्रत्यागम dfd;

  ret = rmdir_all_fd (dfd);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम rmdir (मार्गः);
}

स्थिर पूर्णाङ्क
read_pids_from_मार्गः (ध्रुव वर्ण *मार्गः, bool recurse, bool तन्तवः, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
  cleanup_close पूर्णाङ्क dirfd = -1;
  size_t n_pids, allocated;
  पूर्णाङ्क बहुलकम्;
  पूर्णाङ्क ret;

  यदि (मार्गः == रिक्त || *मार्गः == '\0')
    प्रत्यागम 0;

  बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (बहुलकम् < 0))
    प्रत्यागम बहुलकम्;

  विकल्प (बहुलकम्)
    {
    पक्ष CGROUP_बहुलकम्_UNIFIED:
      ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, मार्गः, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      भङ्ग;

    पक्ष CGROUP_बहुलकम्_HYBRID:
    पक्ष CGROUP_बहुलकम्_LEGACY:
      ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम् "/memory", मार्गः, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      भङ्ग;

    स्वभाव:
      प्रत्यागम crun_make_error (err, 0, "invalid cgroup mode `%d`", बहुलकम्);
    }

  dirfd = उद्घाटय (cgroup_मार्गः, O_DIRECTORY | O_मार्गः | O_CLOEXEC);
  यदि (dirfd < 0)
    प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_मार्गः);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  यदि (तन्तवः && बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
    प्रत्यागम read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  प्रत्यागम read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

पूर्णाङ्क
libcrun_cgroup_read_pids_from_मार्गः (ध्रुव वर्ण *मार्गः, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  प्रत्यागम read_pids_from_मार्गः (मार्गः, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (वर्ण *content, वर्ण **saveptr, वर्ण **id, वर्ण **controller_list, वर्ण **मार्गः)
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

  यदि (मार्गः)
    *मार्गः = it;

  प्रत्यागम true;
}

पूर्णाङ्क
destroy_cgroup_मार्गः (ध्रुव वर्ण *मार्गः, पूर्णाङ्क बहुलकम्, libcrun_error_t *err)
{
  bool repeat = true;
  पूर्णाङ्क retry_गणनम् = 0;
  ध्रुव पूर्णाङ्क max_attempts = 500;
  पूर्णाङ्क ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  यदि (is_रिक्तानि_string (मार्गः))
    प्रत्यागम 0;

  कुरु
    {
      repeat = false;

      यदि (बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
        {
          cleanup_free वर्ण *cgroup_मार्गः = रिक्त;

          ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, मार्गः, रिक्त);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          ret = rmdir (cgroup_मार्गः);
          यदि (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_मार्गः);
              यदि (ret < 0)
                {
                  यदि (retry_गणनम् >= max_attempts)
                    प्रत्यागम crun_make_error (err, errno, "cannot delete path `%s`", cgroup_मार्गः);

                  retry_गणनम्++;
                  repeat = true;
                }
            }
        }
      अन्यथा
        {
          cleanup_free वर्ण *content = रिक्त;
          size_t content_आकार;
          वर्ण *controller;
          वर्ण *saveptr;
          bool has_दत्त;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_आकार, err);
          यदि (UNLIKELY (ret < 0))
            {
              यदि (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  प्रत्यागम 0;
                }
              प्रत्यागम ret;
            }

          प्रति (has_दत्त = read_proc_cgroup (content, &saveptr, रिक्त, &controller, रिक्त);
               has_दत्त;
               has_दत्त = read_proc_cgroup (रिक्त, &saveptr, रिक्त, &controller, रिक्त))
            {
              cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
              वर्ण *subsystem;
              यदि (has_उपसर्गः (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              यदि (बहुलकम् == CGROUP_बहुलकम्_LEGACY && वाक्यं_तुलय (subsystem, "unified") == 0)
                अनुवर्त;

              ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, subsystem, मार्गः, रिक्त);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम ret;

              ret = rmdir (cgroup_मार्गः);
              यदि (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_मार्गः);
                  यदि (ret < 0)
                    {
                      यदि (retry_गणनम् >= max_attempts)
                        प्रत्यागम crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_मार्गः);
                      retry_गणनम्++;
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

          ret = cgroup_killall_मार्गः (मार्गः, SIGKILL, err);
          यदि (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } यावत् (repeat);

  प्रत्यागम 0;
}

पूर्णाङ्क
chown_cgroups (ध्रुव वर्ण *मार्गः, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
  cleanup_free वर्ण *delegate = रिक्त;
  cleanup_close पूर्णाङ्क dfd = -1;
  size_t delegate_आकार;
  वर्ण *saveptr = रिक्त;
  वर्ण *name;
  पूर्णाङ्क ret;

  ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, मार्गः, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dfd = उद्घाटय (cgroup_मार्गः, O_CLOEXEC | O_मार्गः);
  यदि (UNLIKELY (dfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_मार्गः);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_आकार, err);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          प्रत्यागम 0;
        }
      प्रत्यागम ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_रिक्तानि_मार्गः | AT_SYMLINK_NOFOLLOW);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot chown `%s`", cgroup_मार्गः);

  प्रति (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (रिक्त, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (errno == ENOENT)
            अनुवर्त;

          प्रत्यागम crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_मार्गः, name);
        }
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libcrun_cgroup_pause_unpause_with_बहुलकम् (ध्रुव वर्ण *cgroup_मार्गः, पूर्णाङ्क cgroup_बहुलकम्, ध्रुव bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free वर्ण *मार्गः = रिक्त;
  ध्रुव वर्ण *state = "";
  पूर्णाङ्क ret;

  यदि (cgroup_मार्गः == रिक्त || cgroup_मार्गः[0] == '\0')
    प्रत्यागम crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&मार्गः, err, CGROUP_मूलम्, cgroup_मार्गः, "cgroup.freeze", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अन्यथा
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&मार्गः, err, CGROUP_मूलम् "/freezer", cgroup_मार्गः, "freezer.state", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = write_cgroup_file (AT_FDCWD, मार्गः, state, वाक्यदैर्घ्यम् (state), err);
  यदि (ret >= 0)
    प्रत्यागम 0;
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_cgroup_pause_unpause_मार्गः (ध्रुव वर्ण *cgroup_मार्गः, ध्रुव bool pause, libcrun_error_t *err)
{
  पूर्णाङ्क cgroup_बहुलकम्;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  प्रत्यागम libcrun_cgroup_pause_unpause_with_बहुलकम् (cgroup_मार्गः, cgroup_बहुलकम्, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
पूर्णाङ्क
libcrun_cgroup_ensure_not_frozen (ध्रुव वर्ण *cgroup_मार्गः, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  cleanup_free वर्ण *मार्गः = रिक्त;
  ध्रुव वर्ण *frozen;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क ret;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&मार्गः, err, CGROUP_मूलम्, cgroup_मार्गः, "cgroup.freeze", रिक्त);
    }
  अन्यथा
    {
      frozen = "FROZEN";
      ret = append_paths (&मार्गः, err, CGROUP_मूलम् "/freezer", cgroup_मार्गः, "freezer.state", रिक्त);
    }
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file (मार्गः, &content, रिक्त, err);
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

  यदि (वाक्ये_अन्विष (content, frozen))
    प्रत्यागम crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  प्रत्यागम 0;
}

#परिभाषय CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
स्थिर पूर्णाङ्क
watch_cgroup_events (ध्रुव वर्ण *मार्गः)
{
  cleanup_free वर्ण *events_file = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_मूलम्, मार्गः, "cgroup.events", रिक्त);
  यदि (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      प्रत्यागम -1;
    }

  fd = inotify_init1 (अन्तर्_CLOEXEC | अन्तर्_NONBLOCK);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम -1;

  ret = inotify_सङ्कलय_watch (fd, events_file, अन्तर्_MODIFY);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम -1;

  ret = fd;
  fd = -1;
  प्रत्यागम ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
स्थिर पूर्णाङ्क
wait_cgroup_unpopulated (ध्रुव वर्ण *मार्गः, पूर्णाङ्क inotify_fd)
{
  cleanup_free वर्ण *events_file = रिक्त;
  libcrun_error_t tmp_err = रिक्त;
  संरचना timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  पूर्णाङ्क ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_मूलम्, मार्गः, "cgroup.events", रिक्त);
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

      यदि (वाक्ये_अन्विष (content, "populated 0"))
        प्रत्यागम 1;

      ret = अस्थायी_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, रिक्त));
      यदि (ret <= 0)
        प्रत्यागम 0;

      यावत् (पठ (inotify_fd, buf, परिमाण (buf)) > 0)
        ;
    }
}

पूर्णाङ्क
cgroup_killall_मार्गः (ध्रुव वर्ण *मार्गः, पूर्णाङ्क सङ्केत, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close पूर्णाङ्क inotify_fd = -1;
  पूर्णाङ्क ret;
  size_t i;
  cleanup_free pid_t *pids = रिक्त;

  यदि (मार्गः == रिक्त || *मार्गः == '\0')
    प्रत्यागम 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  यदि (सङ्केत == SIGKILL)
    {
      cleanup_free वर्ण *kill_file = रिक्त;

      ret = append_paths (&kill_file, err, CGROUP_मूलम्, मार्गः, "cgroup.kill", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      inotify_fd = watch_cgroup_events (मार्गः);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      यदि (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (मार्गः, inotify_fd))
        प्रत्यागम 0;

      यदि (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_मार्गः (मार्गः, true, err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (err);
  अन्यथा
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_मार्गः (मार्गः, true, सङ्केत == SIGKILL, &pids, err);
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
      यदि (libcrun_cgroup_pause_unpause_मार्गः (मार्गः, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
read_उपलब्धम्_controllers (ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  cleanup_free वर्ण *controllers = रिक्त;
  cleanup_free वर्ण *buf = रिक्त;
  वर्ण *saveptr = रिक्त;
  ध्रुव वर्ण *token;
  पूर्णाङ्क उपलब्धम् = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_मूलम्, मार्गः, "cgroup.controllers", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file (controllers, &buf, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रति (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (रिक्त, " \n", &saveptr))
    {
      यदि (वाक्यं_तुलय (token, "memory") == 0)
        उपलब्धम् |= CGROUP_स्मृतिः;
      अन्यथा यदि (वाक्यं_तुलय (token, "cpu") == 0)
        उपलब्धम् |= CGROUP_CPU;
      अन्यथा यदि (वाक्यं_तुलय (token, "cpuset") == 0)
        उपलब्धम् |= CGROUP_CPUSET;
      अन्यथा यदि (वाक्यं_तुलय (token, "hugetlb") == 0)
        उपलब्धम् |= CGROUP_HUGETLB;
      अन्यथा यदि (वाक्यं_तुलय (token, "pids") == 0)
        उपलब्धम् |= CGROUP_PIDS;
      अन्यथा यदि (वाक्यं_तुलय (token, "io") == 0)
        उपलब्धम् |= CGROUP_IO;
      अन्यथा यदि (वाक्यं_तुलय (token, "misc") == 0)
        उपलब्धम् |= CGROUP_MISC;
    }
  प्रत्यागम उपलब्धम्;
}

स्थिर पूर्णाङ्क
write_controller_file (ध्रुव वर्ण *मार्गः, पूर्णाङ्क controllers_गन्तव्यम्_enable, libcrun_error_t *err)
{
  cleanup_free वर्ण *subtree_control = रिक्त;
  cleanup_free वर्ण *controllers = रिक्त;
  size_t controllers_len = 0;
  पूर्णाङ्क ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_गन्तव्यम्_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_गन्तव्यम्_enable & CGROUP_IO) ? "+io" : "", (controllers_गन्तव्यम्_enable & CGROUP_स्मृतिः) ? "+memory" : "",
      (controllers_गन्तव्यम्_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_गन्तव्यम्_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_गन्तव्यम्_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_गन्तव्यम्_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_मूलम्, मार्गः, "cgroup.subtree_control", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  यदि (UNLIKELY (ret < 0))
    {
      cleanup_free वर्ण *controllers_copy = xmalloc (controllers_len + 1);
      पूर्णाङ्क attempts_वामः;
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

          exists = crun_मार्गः_exists (subtree_control, &tmp_err);
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
          ret = maybe_make_cgroup_threaded (मार्गः, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      प्रति (attempts_वामः = 1000; attempts_वामः >= 0; attempts_वामः--)
        {
          पूर्णाङ्क controllers_written;
          bool repeat = false;

          स्मृतिं_प्रतिलिख (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          प्रति (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (रिक्त, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, वाक्यदैर्घ्यम् (token), err);
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
          यदि (controllers_written > 0 && attempts_वामः > 2)
            {
              attempts_वामः = 1;
              अनुवर्त;
            }

          यदि (attempts_वामः > 0)
            {
              संरचना timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, रिक्त);
            }
        }

      /* Refresh what controllers are available.  */
      प्रत्यागम read_उपलब्धम्_controllers (मार्गः, err);
    }

  /* All controllers were enabled successfully.  */
  प्रत्यागम controllers_गन्तव्यम्_enable;
}

पूर्णाङ्क
enable_controllers (ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  cleanup_free वर्ण *tmp_मार्गः = रिक्त;
  वर्ण *it;
  पूर्णाङ्क ret, controllers_गन्तव्यम्_enable;

  xasprintf (&tmp_मार्गः, "%s/", मार्गः);

  ret = read_उपलब्धम्_controllers ("", err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  controllers_गन्तव्यम्_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_गन्तव्यम्_enable, err);
  यदि (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      यदि ((controllers_गन्तव्यम्_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_गन्तव्यम्_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_गन्तव्यम्_enable, err);
        }
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रति (it = strchr (tmp_मार्गः + 1, '/'); it;)
    {
      cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
      वर्ण *अग्र_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, tmp_मार्गः, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = mkdir (cgroup_मार्गः, 0755);
      यदि (UNLIKELY (ret < 0 && errno != EEXIST))
        प्रत्यागम crun_make_error (err, errno, "create `%s`", cgroup_मार्गः);

      यदि (अग्र_slash)
        {
          ret = write_controller_file (tmp_मार्गः, controllers_गन्तव्यम्_enable, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      *it = '/';
      it = अग्र_slash;
    }
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_move_process_गन्तव्यम्_cgroup (pid_t pid, pid_t init_pid, ध्रुव वर्ण *मार्गः, bool create_if_missing, libcrun_error_t *err)
{
  पूर्णाङ्क cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  यदि (मार्गः == रिक्त || *मार्गः == '\0')
    प्रत्यागम 0;

  प्रत्यागम enter_cgroup (cgroup_बहुलकम्, pid, init_pid, मार्गः, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
पूर्णाङ्क
libcrun_move_self_गन्तव्यम्_cgroups (ध्रुव वर्ण *content, libcrun_error_t *err)
{
  cleanup_free वर्ण *buf = xstrdup (content);
  वर्ण *saveptr = रिक्त;
  वर्ण *controller;
  वर्ण *मार्गः;
  पूर्णाङ्क cgroup_बहुलकम्;
  bool has_दत्त;
  पूर्णाङ्क ret;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  प्रति (has_दत्त = read_proc_cgroup (buf, &saveptr, रिक्त, &controller, &मार्गः);
       has_दत्त;
       has_दत्त = read_proc_cgroup (रिक्त, &saveptr, रिक्त, &controller, &मार्गः))
    {
      ध्रुव वर्ण *subsystem = रिक्त;

      यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          यदि (controller[0] != '\0')
            अनुवर्त;
        }
      अन्यथा
        {
          cleanup_free वर्ण *subsystem_मार्गः = रिक्त;

          /* Use the same names as enter_cgroup_v1 does.  */
          यदि (has_उपसर्गः (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          यदि (वाक्यं_तुलय (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          यदि (वाक्यं_तुलय (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_मार्गः, err, CGROUP_मूलम्, subsystem, रिक्त);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          ret = crun_मार्गः_exists (subsystem_मार्गः, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          यदि (ret == 0)
            अनुवर्त;
        }

      ret = move_process_गन्तव्यम्_cgroup (0, subsystem, मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_get_cgroup_dirfd (संरचना libcrun_cgroup_स्थितिः *स्थितिः, ध्रुव वर्ण *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free वर्ण *मार्गः_गन्तव्यम्_cgroup = रिक्त;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क cgroupdirfd;
  पूर्णाङ्क ret;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (cgroup_बहुलकम् < 0)
    प्रत्यागम cgroup_बहुलकम्;

  यदि (cgroup_बहुलकम् != CGROUP_बहुलकम्_UNIFIED)
    प्रत्यागम crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  यदि (स्थितिः == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "internal error");

  यदि (is_रिक्तानि_string (स्थितिः->मार्गः))
    प्रत्यागम crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&मार्गः_गन्तव्यम्_cgroup, err, CGROUP_मूलम्, स्थितिः->मार्गः, sub_cgroup, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  cgroupdirfd = उद्घाटय (मार्गः_गन्तव्यम्_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_मार्गः);
  यदि (UNLIKELY (cgroupdirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", मार्गः_गन्तव्यम्_cgroup);

  प्रत्यागम cgroupdirfd;
}

पूर्णाङ्क
libcrun_migrate_all_pids_गन्तव्यम्_cgroup (pid_t init_pid, वर्ण *from, वर्ण *गन्तव्यम्, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = रिक्त;
  cleanup_close पूर्णाङ्क child_dfd = -1;
  पूर्णाङ्क cgroup_बहुलकम्;
  size_t from_len;
  size_t i;
  पूर्णाङ्क ret;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (cgroup_बहुलकम् < 0)
    प्रत्यागम cgroup_बहुलकम्;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_मार्गः (from, true, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_cgroup_read_pids_from_मार्गः (from, true, &pids, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ thaw;

  from_len = वाक्यदैर्घ्यम् (from);

  प्रति (i = 0; pids && pids[i]; i++)
    {
      cleanup_free वर्ण *pid_मार्गः = रिक्त;
      cleanup_free वर्ण *dest_cgroup = रिक्त;

      ret = libcrun_get_cgroup_process (pids[i], &pid_मार्गः, false, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      यदि (! has_उपसर्गः (pid_मार्गः, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          गच्छ thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", गन्तव्यम्, pid_मार्गः + from_len);

      ret = enter_cgroup (cgroup_बहुलकम्, pids[i], init_pid, dest_cgroup, false, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ thaw;
    }

  ret = 0;

thaw:
  यदि (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = रिक्त;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      यदि (libcrun_cgroup_pause_unpause_मार्गः (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      प्रत्यागम ret;
    }

  ret = libcrun_cgroup_pause_unpause_मार्गः (from, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम destroy_cgroup_मार्गः (from, cgroup_बहुलकम्, err);
}

पूर्णाङ्क
get_cgroup_dirfd_मार्गः (पूर्णाङ्क dirfd, वर्ण **मार्गः, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
  proc_fd_मार्गः_t fd_मार्गः;
  ssize_t len;

  get_proc_self_fd_मार्गः (fd_मार्गः, dirfd);

  len = safe_readlinkat (AT_FDCWD, fd_मार्गः, &cgroup_मार्गः, 0, err);
  यदि (UNLIKELY (len < 0))
    प्रत्यागम len;

  यदि (has_उपसर्गः (cgroup_मार्गः, CGROUP_मूलम्))
    {
      *मार्गः = xstrdup (cgroup_मार्गः + वाक्यदैर्घ्यम् (CGROUP_मूलम्));
      प्रत्यागम 0;
    }
  प्रत्यागम crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_मार्गः);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
पूर्णाङ्क
parse_cpu_max (ध्रुव वर्ण *मानम्, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ध्रुव वर्ण *it = मानम्;
  वर्ण *endptr = रिक्त;

  यावत् (*it == ' ')
    it++;

  errno = 0;
  यदि (has_उपसर्गः (it, "max"))
    {
      *quota = -1;
      endptr = (वर्ण *) it + 3;
    }
  अन्यथा
    {
      *quota = strtoll (it, &endptr, 10);
      यदि (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        प्रत्यागम crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", मानम्);
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
        प्रत्यागम crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", मानम्);
      *has_period = true;
    }

  प्रत्यागम 0;
}
