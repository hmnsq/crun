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

#تعریف_کن GNU_منبع

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
  ثابت حرف *هدف;
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

  لكل (i = 0; cgroup_symlinks[i].name; i++)
    {
      صحيح ret;

      ret = symlinkat (cgroup_symlinks[i].هدف, dirfd, cgroup_symlinks[i].name);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (errno == ENOENT || errno == EEXIST)
            استمر;
          أرجع crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  أرجع 0;
}

صحيح
maybe_make_cgroup_threaded (ثابت حرف *مسیر, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_مسیر_type = عدم;
  ثابت حرف *ثابت threaded = "threaded";
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *میانگیر = عدم;
  ثابت حرف *والد_2;
  size_t اندازه;
  حرف *it;
  صحيح ret;

  إذا (مسیر == عدم)
    أرجع 0;

  مسیر = consume_slashes (مسیر);

  إذا (مسیر[0] == '\0')
    أرجع 0;

  ret = append_paths (&cgroup_مسیر_type, err, CGROUP_ریشه, مسیر, "cgroup.type", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_all_file (cgroup_مسیر_type, &content, &اندازه, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (اندازه > 0)
    {
      it = content + اندازه - 1;
      طالما (*it == '\n' && it > content)
        *it-- = '\0';
    }

  إذا (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    أرجع 0;

  میانگیر = xstrdup (مسیر);
  والد_2 = consume_slashes (dirname (میانگیر));
  إذا (والد_2[0] && strcmp (والد_2, "."))
    {
      ret = maybe_make_cgroup_threaded (والد_2, err);
      إذا (ret < 0)
        أرجع ret;
    }
  أرجع write_cgroup_file (AT_FDCWD, cgroup_مسیر_type, threaded, طول_رشته (threaded), err);
}

صحيح
move_process_مقصد_cgroup (pid_t pid, ثابت حرف *subsystem, ثابت حرف *مسیر, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_مسیر_procs = عدم;
  حرف pid_str[16];
  صحيح ret;

  ret = append_paths (&cgroup_مسیر_procs, err, CGROUP_ریشه,
                      subsystem ? subsystem : "", مسیر ? مسیر : "",
                      "cgroup.procs", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = snprintf (pid_str, حجم (pid_str), "%d", pid);
  إذا (UNLIKELY (ret >= (صحيح) حجم (pid_str)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_مسیر_procs, pid_str, طول_رشته (pid_str), err);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = عدم;
          صحيح نما;

          نما = libcrun_get_cgroup_نما (&tmp_err);
          إذا (UNLIKELY (نما < 0 || نما != CGROUP_نما_UNIFIED))
            {
              crun_error_release (&tmp_err);
              أرجع ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (مسیر, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          أرجع write_cgroup_file (AT_FDCWD, cgroup_مسیر_procs, pid_str, طول_رشته (pid_str), err);
        }

      أرجع ret;
    }
  أرجع ret;
}

#اگر_تعریف_نشده CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#پایان_اگر

#اگر_تعریف_نشده TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#پایان_اگر

ساكن صحيح
detect_cgroup_نما (libcrun_error_t *err)
{
  بنية statfs stat;
  صحيح ret;

  ret = statfs (CGROUP_ریشه, &stat);
  إذا (ret < 0)
    أرجع crun_make_error (err, errno, "statfs `" CGROUP_ریشه "`");
  إذا (stat.f_type == CGROUP2_SUPER_MAGIC)
    أرجع CGROUP_نما_UNIFIED;
  إذا (stat.f_type != TMPFS_MAGIC)
    أرجع crun_make_error (err, 0, "invalid file system type on `" CGROUP_ریشه "`");
  ret = statfs (CGROUP_ریشه "/unified", &stat);
  إذا (ret < 0 && errno != ENOENT)
    أرجع crun_make_error (err, errno, "statfs `" CGROUP_ریشه "/unified`");
  إذا (ret < 0)
    أرجع CGROUP_نما_LEGACY;
  أرجع stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_نما_HYBRID : CGROUP_نما_LEGACY;
}

صحيح
libcrun_get_cgroup_نما (libcrun_error_t *err)
{
  صحيح tmp;
  ساكن صحيح cgroup_نما = 0;

  إذا (cgroup_نما)
    أرجع cgroup_نما;

  tmp = detect_cgroup_نما (err);
  إذا (UNLIKELY (tmp < 0))
    أرجع tmp;

  cgroup_نما = tmp;

  أرجع cgroup_نما;
}

ساكن صحيح
get_cgroup_process_from_content (حرف *content, صحيح cgroup_نما, حرف **مسیر, bool absolute, libcrun_error_t *err)
{
  حرف *cg_مسیر = عدم;
  حرف *controller;
  حرف *saveptr;
  bool has_داده;

  لكل (has_داده = read_proc_cgroup (content, &saveptr, عدم, &controller, &cg_مسیر);
       has_داده;
       has_داده = read_proc_cgroup (عدم, &saveptr, عدم, &controller, &cg_مسیر))
    {
      إذا (cgroup_نما == CGROUP_نما_UNIFIED)
        {
          إذا (strcmp (controller, "") == 0 && طول_رشته (cg_مسیر) > 0)
            اذهب یافت;
        }
      وإلا
        {
          إذا (strcmp (controller, "memory"))
            اذهب یافت;
        }
    }

  أرجع crun_make_error (err, 0, "cannot find cgroup for the process");

یافت:
  إذا (absolute)
    أرجع append_paths (مسیر, err, CGROUP_ریشه, cg_مسیر, عدم);

  *مسیر = xstrdup (cg_مسیر);
  أرجع 0;
}

صحيح
libcrun_get_cgroup_process_at (صحيح dirfd, حرف **مسیر, bool absolute, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  size_t content_اندازه;
  صحيح cgroup_نما;
  صحيح ret;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_اندازه, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع get_cgroup_process_from_content (content, cgroup_نما, مسیر, absolute, err);
}

صحيح
libcrun_get_cgroup_process (pid_t pid, حرف **مسیر, bool absolute, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  حرف proc_cgroup_file[64];
  size_t content_اندازه;
  صحيح cgroup_نما;
  صحيح ret;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  إذا (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  وإلا
    {
      صحيح len = snprintf (proc_cgroup_file, حجم (proc_cgroup_file), "/proc/%d/cgroup", pid);
      إذا (UNLIKELY (len >= (صحيح) حجم (proc_cgroup_file)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_اندازه, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع get_cgroup_process_from_content (content, cgroup_نما, مسیر, absolute, err);
}

ساكن صحيح
read_pids_file (صحيح dfd, ثابت حرف *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close صحيح tasksfd = -1;
  cleanup_free حرف *میانگیر = عدم;
  حرف *saveptr = عدم;
  size_t n_new_pids;
  size_t len;
  حرف *it;
  صحيح ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  إذا (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      إذا (errno == ENOENT || errno == ENODEV)
        أرجع 0;
      أرجع crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &میانگیر, &len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (len > 0)
    {
      لكل (n_new_pids = 0, it = میانگیر; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      إذا (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, حجم (pid_t) * *allocated);
        }

      لكل (it = strtok_r (میانگیر, "\n", &saveptr); it; it = strtok_r (عدم, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, عدم, 10);

          إذا (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  أرجع 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
بنية cgroup_walk_dir_s
{
  حرف *name;
  size_t عمق;
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
  size_t عمق;
  بنية cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

ساكن فراغ
cgroup_walk_برداشت (بنية cgroup_walk_s *walk)
{
  حرر (walk->dirs[--walk->n_dirs].name);
}

ساكن فراغ
cleanup_cgroup_walkp (بنية cgroup_walk_s *walk)
{
  صحيح saved_errno = errno;

  طالما (walk->n_dirs > 0)
    cgroup_walk_برداشت (walk);
  حرر (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#تعریف_کن cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

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
  walk->عمق++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  إذا (UNLIKELY (dfd_clone < 0))
    أرجع -1;

  dir = fdopendir (dfd_clone);
  إذا (UNLIKELY (dir == عدم))
    أرجع -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  لكل (de = readdir (dir); de; de = readdir (dir))
    {
      بنية cgroup_walk_dir_s *d;

      إذا (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        استمر;

      إذا (de->d_type != DT_DIR)
        استمر;

      إذا (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, حجم (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->عمق = walk->عمق + 1;
      d->entered = false;
    }

  أرجع 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
ساكن صحيح
cgroup_walk_بعدی (بنية cgroup_walk_s *walk, بنية cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : عدم;

  طالما (*d && walk->عمق >= (*d)->عمق)
    {
      صحيح والد_2;

      والد_2 = openat (walk->dfd, "..", O_DIRECTORY | O_مسیر | O_CLOEXEC);
      إذا (UNLIKELY (والد_2 < 0))
        أرجع -1;

      close_and_reset (&walk->dfd);
      walk->dfd = والد_2;
      walk->عمق--;
    }

  أرجع 0;
}

ساكن صحيح
read_pids_cgroup_file (صحيح dfd, ثابت حرف *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk بنية cgroup_walk_s walk = { .dfd = -1 };
  صحيح dfd_clone;
  صحيح ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! recurse)
    أرجع 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_مسیر | O_CLOEXEC);
  إذا (UNLIKELY (dfd_clone < 0))
    أرجع crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "open cgroup sub-directory");

  لكل (;;)
    {
      بنية cgroup_walk_dir_s *d;
      cleanup_close صحيح nfd = -1;

      ret = cgroup_walk_بعدی (&walk, &d);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "open parent cgroup directory");
      إذا (d == عدم)
        اكسر;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_مسیر | O_CLOEXEC);
      إذا (UNLIKELY (nfd < 0))
        {
          صحيح saved_errno = errno;

          إذا (saved_errno == ENOENT)
            {
              cgroup_walk_برداشت (&walk);
              استمر;
            }
          أرجع crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_برداشت (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      إذا (UNLIKELY (ret < 0))
        {
          صحيح e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          إذا (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            أرجع ret;
          crun_error_release (err);
          استمر;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      إذا (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        أرجع crun_make_error (err, errno, "open cgroup sub-directory");
    }

  أرجع 0;
}

ساكن صحيح
read_pids_cgroup (صحيح dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  صحيح ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  إذا (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  أرجع ret;
}

ساكن صحيح
rmdir_all_fd (صحيح dfd)
{
  cleanup_cgroup_walk بنية cgroup_walk_s walk = { .dfd = -1 };
  صحيح ret;

  ret = cgroup_walk_enter (&walk, dfd);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  لكل (;;)
    {
      بنية cgroup_walk_dir_s *d;
      صحيح فرزند_dfd;

      ret = cgroup_walk_بعدی (&walk, &d);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (d == عدم)
        اكسر;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      إذا (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_برداشت (&walk);
          استمر;
        }

      فرزند_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      إذا (UNLIKELY (فرزند_dfd < 0))
        {
          إذا (errno == ENOENT)
            {
              cgroup_walk_برداشت (&walk);
              استمر;
            }
          أرجع فرزند_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, فرزند_dfd);
      إذا (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        أرجع ret;
    }

  أرجع 0;
}

ساكن صحيح
rmdir_all (ثابت حرف *مسیر)
{
  صحيح ret;
  صحيح dfd = open (مسیر, O_DIRECTORY | O_CLOEXEC);
  إذا (UNLIKELY (dfd < 0))
    أرجع dfd;

  ret = rmdir_all_fd (dfd);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع rmdir (مسیر);
}

ساكن صحيح
read_pids_from_مسیر (ثابت حرف *مسیر, bool recurse, bool نخها, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_مسیر = عدم;
  cleanup_close صحيح dirfd = -1;
  size_t n_pids, allocated;
  صحيح نما;
  صحيح ret;

  إذا (مسیر == عدم || *مسیر == '\0')
    أرجع 0;

  نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (نما < 0))
    أرجع نما;

  بدل (نما)
    {
    حالة CGROUP_نما_UNIFIED:
      ret = append_paths (&cgroup_مسیر, err, CGROUP_ریشه, مسیر, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      اكسر;

    حالة CGROUP_نما_HYBRID:
    حالة CGROUP_نما_LEGACY:
      ret = append_paths (&cgroup_مسیر, err, CGROUP_ریشه "/memory", مسیر, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      اكسر;

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", نما);
    }

  dirfd = open (cgroup_مسیر, O_DIRECTORY | O_مسیر | O_CLOEXEC);
  إذا (dirfd < 0)
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_مسیر);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  إذا (نخها && نما == CGROUP_نما_UNIFIED)
    أرجع read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  أرجع read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

صحيح
libcrun_cgroup_read_pids_from_مسیر (ثابت حرف *مسیر, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  أرجع read_pids_from_مسیر (مسیر, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (حرف *content, حرف **saveptr, حرف **id, حرف **controller_list, حرف **مسیر)
{
  حرف *it;

  it = strtok_r (content, "\n", saveptr);
  إذا (it == عدم)
    أرجع false;

  إذا (id)
    *id = it;

  it = strchr (it, ':');
  إذا (it == عدم)
    أرجع false;
  *it++ = '\0';

  إذا (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  إذا (it == عدم)
    أرجع false;
  *it++ = '\0';

  إذا (مسیر)
    *مسیر = it;

  أرجع true;
}

صحيح
destroy_cgroup_مسیر (ثابت حرف *مسیر, صحيح نما, libcrun_error_t *err)
{
  bool repeat = true;
  صحيح retry_شمار = 0;
  ثابت صحيح max_attempts = 500;
  صحيح ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  إذا (is_خالی_string (مسیر))
    أرجع 0;

  افعل
    {
      repeat = false;

      إذا (نما == CGROUP_نما_UNIFIED)
        {
          cleanup_free حرف *cgroup_مسیر = عدم;

          ret = append_paths (&cgroup_مسیر, err, CGROUP_ریشه, مسیر, عدم);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          ret = rmdir (cgroup_مسیر);
          إذا (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_مسیر);
              إذا (ret < 0)
                {
                  إذا (retry_شمار >= max_attempts)
                    أرجع crun_make_error (err, errno, "cannot delete path `%s`", cgroup_مسیر);

                  retry_شمار++;
                  repeat = true;
                }
            }
        }
      وإلا
        {
          cleanup_free حرف *content = عدم;
          size_t content_اندازه;
          حرف *controller;
          حرف *saveptr;
          bool has_داده;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_اندازه, err);
          إذا (UNLIKELY (ret < 0))
            {
              إذا (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  أرجع 0;
                }
              أرجع ret;
            }

          لكل (has_داده = read_proc_cgroup (content, &saveptr, عدم, &controller, عدم);
               has_داده;
               has_داده = read_proc_cgroup (عدم, &saveptr, عدم, &controller, عدم))
            {
              cleanup_free حرف *cgroup_مسیر = عدم;
              حرف *subsystem;
              إذا (has_پیشوند (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              إذا (نما == CGROUP_نما_LEGACY && strcmp (subsystem, "unified") == 0)
                استمر;

              ret = append_paths (&cgroup_مسیر, err, CGROUP_ریشه, subsystem, مسیر, عدم);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;

              ret = rmdir (cgroup_مسیر);
              إذا (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_مسیر);
                  إذا (ret < 0)
                    {
                      إذا (retry_شمار >= max_attempts)
                        أرجع crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_مسیر);
                      retry_شمار++;
                      repeat = true;
                    }
                }
            }
        }

      إذا (repeat)
        {
          بنية timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, عدم);

          ret = cgroup_killall_مسیر (مسیر, SIGKILL, err);
          إذا (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } طالما (repeat);

  أرجع 0;
}

صحيح
chown_cgroups (ثابت حرف *مسیر, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_مسیر = عدم;
  cleanup_free حرف *delegate = عدم;
  cleanup_close صحيح dfd = -1;
  size_t delegate_اندازه;
  حرف *saveptr = عدم;
  حرف *name;
  صحيح ret;

  ret = append_paths (&cgroup_مسیر, err, CGROUP_ریشه, مسیر, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dfd = open (cgroup_مسیر, O_CLOEXEC | O_مسیر);
  إذا (UNLIKELY (dfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_مسیر);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_اندازه, err);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          أرجع 0;
        }
      أرجع ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_خالی_مسیر | AT_SYMLINK_NOFOLLOW);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "cannot chown `%s`", cgroup_مسیر);

  لكل (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (عدم, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (errno == ENOENT)
            استمر;

          أرجع crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_مسیر, name);
        }
    }

  أرجع 0;
}

ساكن صحيح
libcrun_cgroup_pause_unpause_with_نما (ثابت حرف *cgroup_مسیر, صحيح cgroup_نما, ثابت bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free حرف *مسیر = عدم;
  ثابت حرف *state = "";
  صحيح ret;

  إذا (cgroup_مسیر == عدم || cgroup_مسیر[0] == '\0')
    أرجع crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  إذا (cgroup_نما == CGROUP_نما_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&مسیر, err, CGROUP_ریشه, cgroup_مسیر, "cgroup.freeze", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&مسیر, err, CGROUP_ریشه "/freezer", cgroup_مسیر, "freezer.state", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = write_cgroup_file (AT_FDCWD, مسیر, state, طول_رشته (state), err);
  إذا (ret >= 0)
    أرجع 0;
  أرجع ret;
}

صحيح
libcrun_cgroup_pause_unpause_مسیر (ثابت حرف *cgroup_مسیر, ثابت bool pause, libcrun_error_t *err)
{
  صحيح cgroup_نما;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  أرجع libcrun_cgroup_pause_unpause_with_نما (cgroup_مسیر, cgroup_نما, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
صحيح
libcrun_cgroup_ensure_not_frozen (ثابت حرف *cgroup_مسیر, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *مسیر = عدم;
  ثابت حرف *frozen;
  صحيح cgroup_نما;
  صحيح ret;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  إذا (cgroup_نما == CGROUP_نما_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&مسیر, err, CGROUP_ریشه, cgroup_مسیر, "cgroup.freeze", عدم);
    }
  وإلا
    {
      frozen = "FROZEN";
      ret = append_paths (&مسیر, err, CGROUP_ریشه "/freezer", cgroup_مسیر, "freezer.state", عدم);
    }
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_all_file (مسیر, &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      إذا (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          أرجع 0;
        }
      أرجع ret;
    }

  إذا (جستجوی_رشته (content, frozen))
    أرجع crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  أرجع 0;
}

#تعریف_کن CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
ساكن صحيح
watch_cgroup_events (ثابت حرف *مسیر)
{
  cleanup_free حرف *events_file = عدم;
  cleanup_close صحيح fd = -1;
  libcrun_error_t tmp_err = عدم;
  صحيح ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ریشه, مسیر, "cgroup.events", عدم);
  إذا (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      أرجع -1;
    }

  fd = inotify_init1 (درون_CLOEXEC | درون_NONBLOCK);
  إذا (UNLIKELY (fd < 0))
    أرجع -1;

  ret = inotify_جمع_کن_watch (fd, events_file, درون_MODIFY);
  إذا (UNLIKELY (ret < 0))
    أرجع -1;

  ret = fd;
  fd = -1;
  أرجع ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
ساكن صحيح
wait_cgroup_unpopulated (ثابت حرف *مسیر, صحيح inotify_fd)
{
  cleanup_free حرف *events_file = عدم;
  libcrun_error_t tmp_err = عدم;
  بنية timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  صحيح ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ریشه, مسیر, "cgroup.events", عدم);
  إذا (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      أرجع 0;
    }

  لكل (;;)
    {
      cleanup_free حرف *content = عدم;
      حرف buf[4096];
      بنية pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, عدم, &tmp_err);
      إذا (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          أرجع 0;
        }

      إذا (جستجوی_رشته (content, "populated 0"))
        أرجع 1;

      ret = موقت_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, عدم));
      إذا (ret <= 0)
        أرجع 0;

      طالما (اقرأ (inotify_fd, buf, حجم (buf)) > 0)
        ;
    }
}

صحيح
cgroup_killall_مسیر (ثابت حرف *مسیر, صحيح إشارة, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close صحيح inotify_fd = -1;
  صحيح ret;
  size_t i;
  cleanup_free pid_t *pids = عدم;

  إذا (مسیر == عدم || *مسیر == '\0')
    أرجع 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  إذا (إشارة == SIGKILL)
    {
      cleanup_free حرف *kill_file = عدم;

      ret = append_paths (&kill_file, err, CGROUP_ریشه, مسیر, "cgroup.kill", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      inotify_fd = watch_cgroup_events (مسیر);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      إذا (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (مسیر, inotify_fd))
        أرجع 0;

      إذا (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_مسیر (مسیر, true, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);
  وإلا
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_مسیر (مسیر, true, إشارة == SIGKILL, &pids, err);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (crun_error_get_errno (err) != ENOENT)
        اذهب thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  لكل (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], إشارة);
      إذا (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          اذهب thaw;
        }
    }

  ret = 0;

thaw:
  إذا (frozen)
    {
      libcrun_error_t thaw_err = عدم;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      إذا (libcrun_cgroup_pause_unpause_مسیر (مسیر, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  أرجع ret;
}

ساكن صحيح
read_موجود_controllers (ثابت حرف *مسیر, libcrun_error_t *err)
{
  cleanup_free حرف *controllers = عدم;
  cleanup_free حرف *buf = عدم;
  حرف *saveptr = عدم;
  ثابت حرف *token;
  صحيح موجود = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ریشه, مسیر, "cgroup.controllers", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_all_file (controllers, &buf, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  لكل (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (عدم, " \n", &saveptr))
    {
      إذا (strcmp (token, "memory") == 0)
        موجود |= CGROUP_حافظه;
      وإلا إذا (strcmp (token, "cpu") == 0)
        موجود |= CGROUP_CPU;
      وإلا إذا (strcmp (token, "cpuset") == 0)
        موجود |= CGROUP_CPUSET;
      وإلا إذا (strcmp (token, "hugetlb") == 0)
        موجود |= CGROUP_HUGETLB;
      وإلا إذا (strcmp (token, "pids") == 0)
        موجود |= CGROUP_PIDS;
      وإلا إذا (strcmp (token, "io") == 0)
        موجود |= CGROUP_IO;
      وإلا إذا (strcmp (token, "misc") == 0)
        موجود |= CGROUP_MISC;
    }
  أرجع موجود;
}

ساكن صحيح
write_controller_file (ثابت حرف *مسیر, صحيح controllers_مقصد_enable, libcrun_error_t *err)
{
  cleanup_free حرف *subtree_control = عدم;
  cleanup_free حرف *controllers = عدم;
  size_t controllers_len = 0;
  صحيح ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_مقصد_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_مقصد_enable & CGROUP_IO) ? "+io" : "", (controllers_مقصد_enable & CGROUP_حافظه) ? "+memory" : "",
      (controllers_مقصد_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_مقصد_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_مقصد_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_مقصد_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ریشه, مسیر, "cgroup.subtree_control", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  إذا (UNLIKELY (ret < 0))
    {
      cleanup_free حرف *controllers_copy = xmalloc (controllers_len + 1);
      صحيح attempts_چپ;
      حرف *saveptr = عدم;
      ثابت حرف *token;
      صحيح e;

      e = crun_error_get_errno (err);
      إذا (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        أرجع crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      إذا (e == ENOENT)
        {
          libcrun_error_t tmp_err = عدم;
          صحيح exists;

          exists = crun_مسیر_exists (subtree_control, &tmp_err);
          إذا (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              أرجع ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          إذا (exists == 0)
            أرجع ret;
        }

      crun_error_release (err);

      إذا (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (مسیر, err);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      لكل (attempts_چپ = 1000; attempts_چپ >= 0; attempts_چپ--)
        {
          صحيح controllers_written;
          bool repeat = false;

          کپی_حافظه (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          لكل (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (عدم, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, طول_رشته (token), err);
              إذا (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  إذا (e == EBUSY)
                    repeat = true;

                  استمر;
                }

              controllers_written++;
            }

          إذا (! repeat)
            اكسر;

          /* If there was any controller written, just try once more without any delay.  */
          إذا (controllers_written > 0 && attempts_چپ > 2)
            {
              attempts_چپ = 1;
              استمر;
            }

          إذا (attempts_چپ > 0)
            {
              بنية timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, عدم);
            }
        }

      /* Refresh what controllers are available.  */
      أرجع read_موجود_controllers (مسیر, err);
    }

  /* All controllers were enabled successfully.  */
  أرجع controllers_مقصد_enable;
}

صحيح
enable_controllers (ثابت حرف *مسیر, libcrun_error_t *err)
{
  cleanup_free حرف *tmp_مسیر = عدم;
  حرف *it;
  صحيح ret, controllers_مقصد_enable;

  xasprintf (&tmp_مسیر, "%s/", مسیر);

  ret = read_موجود_controllers ("", err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  controllers_مقصد_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_مقصد_enable, err);
  إذا (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      إذا ((controllers_مقصد_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_مقصد_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_مقصد_enable, err);
        }
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  لكل (it = strchr (tmp_مسیر + 1, '/'); it;)
    {
      cleanup_free حرف *cgroup_مسیر = عدم;
      حرف *بعدی_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_مسیر, err, CGROUP_ریشه, tmp_مسیر, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = mkdir (cgroup_مسیر, 0755);
      إذا (UNLIKELY (ret < 0 && errno != EEXIST))
        أرجع crun_make_error (err, errno, "create `%s`", cgroup_مسیر);

      إذا (بعدی_slash)
        {
          ret = write_controller_file (tmp_مسیر, controllers_مقصد_enable, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      *it = '/';
      it = بعدی_slash;
    }
  أرجع 0;
}

صحيح
libcrun_move_process_مقصد_cgroup (pid_t pid, pid_t init_pid, ثابت حرف *مسیر, bool create_if_missing, libcrun_error_t *err)
{
  صحيح cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  إذا (مسیر == عدم || *مسیر == '\0')
    أرجع 0;

  أرجع enter_cgroup (cgroup_نما, pid, init_pid, مسیر, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
صحيح
libcrun_move_self_مقصد_cgroups (ثابت حرف *content, libcrun_error_t *err)
{
  cleanup_free حرف *buf = xstrdup (content);
  حرف *saveptr = عدم;
  حرف *controller;
  حرف *مسیر;
  صحيح cgroup_نما;
  bool has_داده;
  صحيح ret;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  لكل (has_داده = read_proc_cgroup (buf, &saveptr, عدم, &controller, &مسیر);
       has_داده;
       has_داده = read_proc_cgroup (عدم, &saveptr, عدم, &controller, &مسیر))
    {
      ثابت حرف *subsystem = عدم;

      إذا (cgroup_نما == CGROUP_نما_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          إذا (controller[0] != '\0')
            استمر;
        }
      وإلا
        {
          cleanup_free حرف *subsystem_مسیر = عدم;

          /* Use the same names as enter_cgroup_v1 does.  */
          إذا (has_پیشوند (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          إذا (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          إذا (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_مسیر, err, CGROUP_ریشه, subsystem, عدم);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          ret = crun_مسیر_exists (subsystem_مسیر, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          إذا (ret == 0)
            استمر;
        }

      ret = move_process_مقصد_cgroup (0, subsystem, مسیر, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

صحيح
libcrun_get_cgroup_dirfd (بنية libcrun_cgroup_وضعیت *وضعیت, ثابت حرف *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free حرف *مسیر_مقصد_cgroup = عدم;
  صحيح cgroup_نما;
  صحيح cgroupdirfd;
  صحيح ret;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (cgroup_نما < 0)
    أرجع cgroup_نما;

  إذا (cgroup_نما != CGROUP_نما_UNIFIED)
    أرجع crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  إذا (وضعیت == عدم)
    أرجع crun_make_error (err, 0, "internal error");

  إذا (is_خالی_string (وضعیت->مسیر))
    أرجع crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&مسیر_مقصد_cgroup, err, CGROUP_ریشه, وضعیت->مسیر, sub_cgroup, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  cgroupdirfd = open (مسیر_مقصد_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_مسیر);
  إذا (UNLIKELY (cgroupdirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", مسیر_مقصد_cgroup);

  أرجع cgroupdirfd;
}

صحيح
libcrun_migrate_all_pids_مقصد_cgroup (pid_t init_pid, حرف *from, حرف *مقصد, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = عدم;
  cleanup_close صحيح فرزند_dfd = -1;
  صحيح cgroup_نما;
  size_t from_len;
  size_t i;
  صحيح ret;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (cgroup_نما < 0)
    أرجع cgroup_نما;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_مسیر (from, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_cgroup_read_pids_from_مسیر (from, true, &pids, err);
  إذا (UNLIKELY (ret < 0))
    اذهب thaw;

  from_len = طول_رشته (from);

  لكل (i = 0; pids && pids[i]; i++)
    {
      cleanup_free حرف *pid_مسیر = عدم;
      cleanup_free حرف *dest_cgroup = عدم;

      ret = libcrun_get_cgroup_process (pids[i], &pid_مسیر, false, err);
      إذا (UNLIKELY (ret < 0))
        اذهب thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      إذا (! has_پیشوند (pid_مسیر, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          اذهب thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", مقصد, pid_مسیر + from_len);

      ret = enter_cgroup (cgroup_نما, pids[i], init_pid, dest_cgroup, false, err);
      إذا (UNLIKELY (ret < 0))
        اذهب thaw;
    }

  ret = 0;

thaw:
  إذا (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = عدم;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      إذا (libcrun_cgroup_pause_unpause_مسیر (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      أرجع ret;
    }

  ret = libcrun_cgroup_pause_unpause_مسیر (from, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع destroy_cgroup_مسیر (from, cgroup_نما, err);
}

صحيح
get_cgroup_dirfd_مسیر (صحيح dirfd, حرف **مسیر, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_مسیر = عدم;
  proc_fd_مسیر_t fd_مسیر;
  ssize_t len;

  get_proc_self_fd_مسیر (fd_مسیر, dirfd);

  len = امن_readlinkat (AT_FDCWD, fd_مسیر, &cgroup_مسیر, 0, err);
  إذا (UNLIKELY (len < 0))
    أرجع len;

  إذا (has_پیشوند (cgroup_مسیر, CGROUP_ریشه))
    {
      *مسیر = xstrdup (cgroup_مسیر + طول_رشته (CGROUP_ریشه));
      أرجع 0;
    }
  أرجع crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_مسیر);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
صحيح
parse_cpu_max (ثابت حرف *مقدار, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ثابت حرف *it = مقدار;
  حرف *endptr = عدم;

  طالما (*it == ' ')
    it++;

  errno = 0;
  إذا (has_پیشوند (it, "max"))
    {
      *quota = -1;
      endptr = (حرف *) it + 3;
    }
  وإلا
    {
      *quota = strtoll (it, &endptr, 10);
      إذا (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        أرجع crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", مقدار);
    }

  it = endptr;
  طالما (*it == ' ')
    it++;

  *has_period = false;
  إذا (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      إذا (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        أرجع crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", مقدار);
      *has_period = true;
    }

  أرجع 0;
}
