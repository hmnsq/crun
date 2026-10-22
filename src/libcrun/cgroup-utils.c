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

#تعریف_کرو GNU_ماخذ

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

ساخت symlink_s
{
  ثابت حرف *name;
  ثابت حرف *ہدف;
};

ساکن ساخت symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { تہی, تہی } };

صحیح
libcrun_cgroups_create_symlinks (صحیح dirfd, libcrun_error_t *err)
{
  صحیح i;

  برائے (i = 0; cgroup_symlinks[i].name; i++)
    {
      صحیح ret;

      ret = symlinkat (cgroup_symlinks[i].ہدف, dirfd, cgroup_symlinks[i].name);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (errno == ENOENT || errno == EEXIST)
            جاری;
          واپس crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  واپس 0;
}

صحیح
maybe_make_cgroup_threaded (ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_راستہ_type = تہی;
  ثابت حرف *ثابت threaded = "threaded";
  cleanup_free حرف *content = تہی;
  cleanup_free حرف *بفر = تہی;
  ثابت حرف *والد_2;
  size_t سائز;
  حرف *it;
  صحیح ret;

  اگر (راستہ == تہی)
    واپس 0;

  راستہ = consume_slashes (راستہ);

  اگر (راستہ[0] == '\0')
    واپس 0;

  ret = append_paths (&cgroup_راستہ_type, err, CGROUP_جڑ, راستہ, "cgroup.type", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = read_all_file (cgroup_راستہ_type, &content, &سائز, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (سائز > 0)
    {
      it = content + سائز - 1;
      جب (*it == '\n' && it > content)
        *it-- = '\0';
    }

  اگر (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    واپس 0;

  بفر = xstrdup (راستہ);
  والد_2 = consume_slashes (dirname (بفر));
  اگر (والد_2[0] && strcmp (والد_2, "."))
    {
      ret = maybe_make_cgroup_threaded (والد_2, err);
      اگر (ret < 0)
        واپس ret;
    }
  واپس write_cgroup_file (AT_FDCWD, cgroup_راستہ_type, threaded, لڑی_لمبائی (threaded), err);
}

صحیح
move_process_مقصد_cgroup (pid_t pid, ثابت حرف *subsystem, ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_راستہ_procs = تہی;
  حرف pid_str[16];
  صحیح ret;

  ret = append_paths (&cgroup_راستہ_procs, err, CGROUP_جڑ,
                      subsystem ? subsystem : "", راستہ ? راستہ : "",
                      "cgroup.procs", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = snprintf (pid_str, حجم (pid_str), "%d", pid);
  اگر (UNLIKELY (ret >= (صحیح) حجم (pid_str)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_راستہ_procs, pid_str, لڑی_لمبائی (pid_str), err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = تہی;
          صحیح منوال;

          منوال = libcrun_get_cgroup_منوال (&tmp_err);
          اگر (UNLIKELY (منوال < 0 || منوال != CGROUP_منوال_UNIFIED))
            {
              crun_error_release (&tmp_err);
              واپس ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (راستہ, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          واپس write_cgroup_file (AT_FDCWD, cgroup_راستہ_procs, pid_str, لڑی_لمبائی (pid_str), err);
        }

      واپس ret;
    }
  واپس ret;
}

#اگر_غیر_تعریف_شدہ CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#اگر_ختم

#اگر_غیر_تعریف_شدہ TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#اگر_ختم

ساکن صحیح
detect_cgroup_منوال (libcrun_error_t *err)
{
  ساخت statfs stat;
  صحیح ret;

  ret = statfs (CGROUP_جڑ, &stat);
  اگر (ret < 0)
    واپس crun_make_error (err, errno, "statfs `" CGROUP_جڑ "`");
  اگر (stat.f_type == CGROUP2_SUPER_MAGIC)
    واپس CGROUP_منوال_UNIFIED;
  اگر (stat.f_type != TMPFS_MAGIC)
    واپس crun_make_error (err, 0, "invalid file system type on `" CGROUP_جڑ "`");
  ret = statfs (CGROUP_جڑ "/unified", &stat);
  اگر (ret < 0 && errno != ENOENT)
    واپس crun_make_error (err, errno, "statfs `" CGROUP_جڑ "/unified`");
  اگر (ret < 0)
    واپس CGROUP_منوال_LEGACY;
  واپس stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_منوال_HYBRID : CGROUP_منوال_LEGACY;
}

صحیح
libcrun_get_cgroup_منوال (libcrun_error_t *err)
{
  صحیح tmp;
  ساکن صحیح cgroup_منوال = 0;

  اگر (cgroup_منوال)
    واپس cgroup_منوال;

  tmp = detect_cgroup_منوال (err);
  اگر (UNLIKELY (tmp < 0))
    واپس tmp;

  cgroup_منوال = tmp;

  واپس cgroup_منوال;
}

ساکن صحیح
get_cgroup_process_from_content (حرف *content, صحیح cgroup_منوال, حرف **راستہ, bool absolute, libcrun_error_t *err)
{
  حرف *cg_راستہ = تہی;
  حرف *controller;
  حرف *saveptr;
  bool has_ڈیٹا;

  برائے (has_ڈیٹا = read_proc_cgroup (content, &saveptr, تہی, &controller, &cg_راستہ);
       has_ڈیٹا;
       has_ڈیٹا = read_proc_cgroup (تہی, &saveptr, تہی, &controller, &cg_راستہ))
    {
      اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
        {
          اگر (strcmp (controller, "") == 0 && لڑی_لمبائی (cg_راستہ) > 0)
            جاؤ ملا;
        }
      ورنہ
        {
          اگر (strcmp (controller, "memory"))
            جاؤ ملا;
        }
    }

  واپس crun_make_error (err, 0, "cannot find cgroup for the process");

ملا:
  اگر (absolute)
    واپس append_paths (راستہ, err, CGROUP_جڑ, cg_راستہ, تہی);

  *راستہ = xstrdup (cg_راستہ);
  واپس 0;
}

صحیح
libcrun_get_cgroup_process_at (صحیح dirfd, حرف **راستہ, bool absolute, libcrun_error_t *err)
{
  cleanup_free حرف *content = تہی;
  size_t content_سائز;
  صحیح cgroup_منوال;
  صحیح ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_سائز, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس get_cgroup_process_from_content (content, cgroup_منوال, راستہ, absolute, err);
}

صحیح
libcrun_get_cgroup_process (pid_t pid, حرف **راستہ, bool absolute, libcrun_error_t *err)
{
  cleanup_free حرف *content = تہی;
  حرف proc_cgroup_file[64];
  size_t content_سائز;
  صحیح cgroup_منوال;
  صحیح ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  اگر (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  ورنہ
    {
      صحیح len = snprintf (proc_cgroup_file, حجم (proc_cgroup_file), "/proc/%d/cgroup", pid);
      اگر (UNLIKELY (len >= (صحیح) حجم (proc_cgroup_file)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_سائز, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس get_cgroup_process_from_content (content, cgroup_منوال, راستہ, absolute, err);
}

ساکن صحیح
read_pids_file (صحیح dfd, ثابت حرف *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close صحیح tasksfd = -1;
  cleanup_free حرف *بفر = تہی;
  حرف *saveptr = تہی;
  size_t n_new_pids;
  size_t len;
  حرف *it;
  صحیح ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  اگر (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      اگر (errno == ENOENT || errno == ENODEV)
        واپس 0;
      واپس crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &بفر, &len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (len > 0)
    {
      برائے (n_new_pids = 0, it = بفر; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      اگر (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, حجم (pid_t) * *allocated);
        }

      برائے (it = strtok_r (بفر, "\n", &saveptr); it; it = strtok_r (تہی, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, تہی, 10);

          اگر (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  واپس 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
ساخت cgroup_walk_dir_s
{
  حرف *name;
  size_t گہرائی;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
ساخت cgroup_walk_s
{
  صحیح dfd;
  size_t گہرائی;
  ساخت cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

ساکن خالی
cgroup_walk_نکالو (ساخت cgroup_walk_s *walk)
{
  آزاد (walk->dirs[--walk->n_dirs].name);
}

ساکن خالی
cleanup_cgroup_walkp (ساخت cgroup_walk_s *walk)
{
  صحیح saved_errno = errno;

  جب (walk->n_dirs > 0)
    cgroup_walk_نکالو (walk);
  آزاد (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#تعریف_کرو cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
ساکن صحیح
cgroup_walk_enter (ساخت cgroup_walk_s *walk, صحیح dfd)
{
  cleanup_close صحیح dfd_clone = -1;
  cleanup_dir DIR *dir = تہی;
  ساخت dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->گہرائی++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (dfd_clone < 0))
    واپس -1;

  dir = fdopendir (dfd_clone);
  اگر (UNLIKELY (dir == تہی))
    واپس -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  برائے (de = readdir (dir); de; de = readdir (dir))
    {
      ساخت cgroup_walk_dir_s *d;

      اگر (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        جاری;

      اگر (de->d_type != DT_DIR)
        جاری;

      اگر (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, حجم (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->گہرائی = walk->گہرائی + 1;
      d->entered = false;
    }

  واپس 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
ساکن صحیح
cgroup_walk_اگلا (ساخت cgroup_walk_s *walk, ساخت cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : تہی;

  جب (*d && walk->گہرائی >= (*d)->گہرائی)
    {
      صحیح والد_2;

      والد_2 = openat (walk->dfd, "..", O_DIRECTORY | O_راستہ | O_CLOEXEC);
      اگر (UNLIKELY (والد_2 < 0))
        واپس -1;

      close_and_reset (&walk->dfd);
      walk->dfd = والد_2;
      walk->گہرائی--;
    }

  واپس 0;
}

ساکن صحیح
read_pids_cgroup_file (صحیح dfd, ثابت حرف *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk ساخت cgroup_walk_s walk = { .dfd = -1 };
  صحیح dfd_clone;
  صحیح ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (! recurse)
    واپس 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (dfd_clone < 0))
    واپس crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "open cgroup sub-directory");

  برائے (;;)
    {
      ساخت cgroup_walk_dir_s *d;
      cleanup_close صحیح nfd = -1;

      ret = cgroup_walk_اگلا (&walk, &d);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "open parent cgroup directory");
      اگر (d == تہی)
        توڑو;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_راستہ | O_CLOEXEC);
      اگر (UNLIKELY (nfd < 0))
        {
          صحیح saved_errno = errno;

          اگر (saved_errno == ENOENT)
            {
              cgroup_walk_نکالو (&walk);
              جاری;
            }
          واپس crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_نکالو (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      اگر (UNLIKELY (ret < 0))
        {
          صحیح e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          اگر (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            واپس ret;
          crun_error_release (err);
          جاری;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      اگر (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        واپس crun_make_error (err, errno, "open cgroup sub-directory");
    }

  واپس 0;
}

ساکن صحیح
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
  واپس ret;
}

ساکن صحیح
rmdir_all_fd (صحیح dfd)
{
  cleanup_cgroup_walk ساخت cgroup_walk_s walk = { .dfd = -1 };
  صحیح ret;

  ret = cgroup_walk_enter (&walk, dfd);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  برائے (;;)
    {
      ساخت cgroup_walk_dir_s *d;
      صحیح بچہ_dfd;

      ret = cgroup_walk_اگلا (&walk, &d);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      اگر (d == تہی)
        توڑو;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      اگر (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_نکالو (&walk);
          جاری;
        }

      بچہ_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      اگر (UNLIKELY (بچہ_dfd < 0))
        {
          اگر (errno == ENOENT)
            {
              cgroup_walk_نکالو (&walk);
              جاری;
            }
          واپس بچہ_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, بچہ_dfd);
      اگر (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        واپس ret;
    }

  واپس 0;
}

ساکن صحیح
rmdir_all (ثابت حرف *راستہ)
{
  صحیح ret;
  صحیح dfd = open (راستہ, O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (dfd < 0))
    واپس dfd;

  ret = rmdir_all_fd (dfd);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس rmdir (راستہ);
}

ساکن صحیح
read_pids_from_راستہ (ثابت حرف *راستہ, bool recurse, bool دھاگے, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_راستہ = تہی;
  cleanup_close صحیح dirfd = -1;
  size_t n_pids, allocated;
  صحیح منوال;
  صحیح ret;

  اگر (راستہ == تہی || *راستہ == '\0')
    واپس 0;

  منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (منوال < 0))
    واپس منوال;

  انتخاب (منوال)
    {
    حالت CGROUP_منوال_UNIFIED:
      ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, راستہ, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      توڑو;

    حالت CGROUP_منوال_HYBRID:
    حالت CGROUP_منوال_LEGACY:
      ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ "/memory", راستہ, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      توڑو;

    طےشدہ:
      واپس crun_make_error (err, 0, "invalid cgroup mode `%d`", منوال);
    }

  dirfd = open (cgroup_راستہ, O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (dirfd < 0)
    واپس crun_make_error (err, errno, "open `%s`", cgroup_راستہ);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  اگر (دھاگے && منوال == CGROUP_منوال_UNIFIED)
    واپس read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  واپس read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

صحیح
libcrun_cgroup_read_pids_from_راستہ (ثابت حرف *راستہ, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  واپس read_pids_from_راستہ (راستہ, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (حرف *content, حرف **saveptr, حرف **id, حرف **controller_list, حرف **راستہ)
{
  حرف *it;

  it = strtok_r (content, "\n", saveptr);
  اگر (it == تہی)
    واپس false;

  اگر (id)
    *id = it;

  it = strchr (it, ':');
  اگر (it == تہی)
    واپس false;
  *it++ = '\0';

  اگر (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  اگر (it == تہی)
    واپس false;
  *it++ = '\0';

  اگر (راستہ)
    *راستہ = it;

  واپس true;
}

صحیح
destroy_cgroup_راستہ (ثابت حرف *راستہ, صحیح منوال, libcrun_error_t *err)
{
  bool repeat = true;
  صحیح retry_گنتی = 0;
  ثابت صحیح max_attempts = 500;
  صحیح ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  اگر (is_خالی_خانے_string (راستہ))
    واپس 0;

  کرو
    {
      repeat = false;

      اگر (منوال == CGROUP_منوال_UNIFIED)
        {
          cleanup_free حرف *cgroup_راستہ = تہی;

          ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, راستہ, تہی);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          ret = rmdir (cgroup_راستہ);
          اگر (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_راستہ);
              اگر (ret < 0)
                {
                  اگر (retry_گنتی >= max_attempts)
                    واپس crun_make_error (err, errno, "cannot delete path `%s`", cgroup_راستہ);

                  retry_گنتی++;
                  repeat = true;
                }
            }
        }
      ورنہ
        {
          cleanup_free حرف *content = تہی;
          size_t content_سائز;
          حرف *controller;
          حرف *saveptr;
          bool has_ڈیٹا;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_سائز, err);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  واپس 0;
                }
              واپس ret;
            }

          برائے (has_ڈیٹا = read_proc_cgroup (content, &saveptr, تہی, &controller, تہی);
               has_ڈیٹا;
               has_ڈیٹا = read_proc_cgroup (تہی, &saveptr, تہی, &controller, تہی))
            {
              cleanup_free حرف *cgroup_راستہ = تہی;
              حرف *subsystem;
              اگر (has_سابقہ (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              اگر (منوال == CGROUP_منوال_LEGACY && strcmp (subsystem, "unified") == 0)
                جاری;

              ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, subsystem, راستہ, تہی);
              اگر (UNLIKELY (ret < 0))
                واپس ret;

              ret = rmdir (cgroup_راستہ);
              اگر (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_راستہ);
                  اگر (ret < 0)
                    {
                      اگر (retry_گنتی >= max_attempts)
                        واپس crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_راستہ);
                      retry_گنتی++;
                      repeat = true;
                    }
                }
            }
        }

      اگر (repeat)
        {
          ساخت timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, تہی);

          ret = cgroup_killall_راستہ (راستہ, SIGKILL, err);
          اگر (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } جب (repeat);

  واپس 0;
}

صحیح
chown_cgroups (ثابت حرف *راستہ, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_راستہ = تہی;
  cleanup_free حرف *delegate = تہی;
  cleanup_close صحیح dfd = -1;
  size_t delegate_سائز;
  حرف *saveptr = تہی;
  حرف *name;
  صحیح ret;

  ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, راستہ, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  dfd = open (cgroup_راستہ, O_CLOEXEC | O_راستہ);
  اگر (UNLIKELY (dfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", cgroup_راستہ);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_سائز, err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          واپس 0;
        }
      واپس ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_خالی_خانے_راستہ | AT_SYMLINK_NOFOLLOW);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "cannot chown `%s`", cgroup_راستہ);

  برائے (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (تہی, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (errno == ENOENT)
            جاری;

          واپس crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_راستہ, name);
        }
    }

  واپس 0;
}

ساکن صحیح
libcrun_cgroup_pause_unpause_with_منوال (ثابت حرف *cgroup_راستہ, صحیح cgroup_منوال, ثابت bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free حرف *راستہ = تہی;
  ثابت حرف *state = "";
  صحیح ret;

  اگر (cgroup_راستہ == تہی || cgroup_راستہ[0] == '\0')
    واپس crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&راستہ, err, CGROUP_جڑ, cgroup_راستہ, "cgroup.freeze", تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&راستہ, err, CGROUP_جڑ "/freezer", cgroup_راستہ, "freezer.state", تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = write_cgroup_file (AT_FDCWD, راستہ, state, لڑی_لمبائی (state), err);
  اگر (ret >= 0)
    واپس 0;
  واپس ret;
}

صحیح
libcrun_cgroup_pause_unpause_راستہ (ثابت حرف *cgroup_راستہ, ثابت bool pause, libcrun_error_t *err)
{
  صحیح cgroup_منوال;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  واپس libcrun_cgroup_pause_unpause_with_منوال (cgroup_راستہ, cgroup_منوال, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
صحیح
libcrun_cgroup_ensure_not_frozen (ثابت حرف *cgroup_راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *content = تہی;
  cleanup_free حرف *راستہ = تہی;
  ثابت حرف *frozen;
  صحیح cgroup_منوال;
  صحیح ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&راستہ, err, CGROUP_جڑ, cgroup_راستہ, "cgroup.freeze", تہی);
    }
  ورنہ
    {
      frozen = "FROZEN";
      ret = append_paths (&راستہ, err, CGROUP_جڑ "/freezer", cgroup_راستہ, "freezer.state", تہی);
    }
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = read_all_file (راستہ, &content, تہی, err);
  اگر (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          واپس 0;
        }
      واپس ret;
    }

  اگر (لڑی_تلاش (content, frozen))
    واپس crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  واپس 0;
}

#تعریف_کرو CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
ساکن صحیح
watch_cgroup_events (ثابت حرف *راستہ)
{
  cleanup_free حرف *events_file = تہی;
  cleanup_close صحیح fd = -1;
  libcrun_error_t tmp_err = تہی;
  صحیح ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_جڑ, راستہ, "cgroup.events", تہی);
  اگر (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      واپس -1;
    }

  fd = inotify_init1 (اندر_CLOEXEC | اندر_NONBLOCK);
  اگر (UNLIKELY (fd < 0))
    واپس -1;

  ret = inotify_جوڑو_watch (fd, events_file, اندر_MODIFY);
  اگر (UNLIKELY (ret < 0))
    واپس -1;

  ret = fd;
  fd = -1;
  واپس ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
ساکن صحیح
wait_cgroup_unpopulated (ثابت حرف *راستہ, صحیح inotify_fd)
{
  cleanup_free حرف *events_file = تہی;
  libcrun_error_t tmp_err = تہی;
  ساخت timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  صحیح ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_جڑ, راستہ, "cgroup.events", تہی);
  اگر (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      واپس 0;
    }

  برائے (;;)
    {
      cleanup_free حرف *content = تہی;
      حرف buf[4096];
      ساخت pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, تہی, &tmp_err);
      اگر (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          واپس 0;
        }

      اگر (لڑی_تلاش (content, "populated 0"))
        واپس 1;

      ret = عارضی_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, تہی));
      اگر (ret <= 0)
        واپس 0;

      جب (اقرأ (inotify_fd, buf, حجم (buf)) > 0)
        ;
    }
}

صحیح
cgroup_killall_راستہ (ثابت حرف *راستہ, صحیح اشارہ, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close صحیح inotify_fd = -1;
  صحیح ret;
  size_t i;
  cleanup_free pid_t *pids = تہی;

  اگر (راستہ == تہی || *راستہ == '\0')
    واپس 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  اگر (اشارہ == SIGKILL)
    {
      cleanup_free حرف *kill_file = تہی;

      ret = append_paths (&kill_file, err, CGROUP_جڑ, راستہ, "cgroup.kill", تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      inotify_fd = watch_cgroup_events (راستہ);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      اگر (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (راستہ, inotify_fd))
        واپس 0;

      اگر (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_راستہ (راستہ, true, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);
  ورنہ
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_راستہ (راستہ, true, اشارہ == SIGKILL, &pids, err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (crun_error_get_errno (err) != ENOENT)
        جاؤ thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  برائے (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], اشارہ);
      اگر (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          جاؤ thaw;
        }
    }

  ret = 0;

thaw:
  اگر (frozen)
    {
      libcrun_error_t thaw_err = تہی;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      اگر (libcrun_cgroup_pause_unpause_راستہ (راستہ, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  واپس ret;
}

ساکن صحیح
read_دستیاب_controllers (ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *controllers = تہی;
  cleanup_free حرف *buf = تہی;
  حرف *saveptr = تہی;
  ثابت حرف *token;
  صحیح دستیاب = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_جڑ, راستہ, "cgroup.controllers", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = read_all_file (controllers, &buf, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  برائے (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (تہی, " \n", &saveptr))
    {
      اگر (strcmp (token, "memory") == 0)
        دستیاب |= CGROUP_حافظہ;
      ورنہ اگر (strcmp (token, "cpu") == 0)
        دستیاب |= CGROUP_CPU;
      ورنہ اگر (strcmp (token, "cpuset") == 0)
        دستیاب |= CGROUP_CPUSET;
      ورنہ اگر (strcmp (token, "hugetlb") == 0)
        دستیاب |= CGROUP_HUGETLB;
      ورنہ اگر (strcmp (token, "pids") == 0)
        دستیاب |= CGROUP_PIDS;
      ورنہ اگر (strcmp (token, "io") == 0)
        دستیاب |= CGROUP_IO;
      ورنہ اگر (strcmp (token, "misc") == 0)
        دستیاب |= CGROUP_MISC;
    }
  واپس دستیاب;
}

ساکن صحیح
write_controller_file (ثابت حرف *راستہ, صحیح controllers_مقصد_enable, libcrun_error_t *err)
{
  cleanup_free حرف *subtree_control = تہی;
  cleanup_free حرف *controllers = تہی;
  size_t controllers_len = 0;
  صحیح ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_مقصد_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_مقصد_enable & CGROUP_IO) ? "+io" : "", (controllers_مقصد_enable & CGROUP_حافظہ) ? "+memory" : "",
      (controllers_مقصد_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_مقصد_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_مقصد_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_مقصد_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_جڑ, راستہ, "cgroup.subtree_control", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  اگر (UNLIKELY (ret < 0))
    {
      cleanup_free حرف *controllers_copy = xmalloc (controllers_len + 1);
      صحیح attempts_بایاں;
      حرف *saveptr = تہی;
      ثابت حرف *token;
      صحیح e;

      e = crun_error_get_errno (err);
      اگر (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        واپس crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      اگر (e == ENOENT)
        {
          libcrun_error_t tmp_err = تہی;
          صحیح exists;

          exists = crun_راستہ_exists (subtree_control, &tmp_err);
          اگر (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              واپس ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          اگر (exists == 0)
            واپس ret;
        }

      crun_error_release (err);

      اگر (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (راستہ, err);
          اگر (UNLIKELY (ret < 0))
            واپس crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      برائے (attempts_بایاں = 1000; attempts_بایاں >= 0; attempts_بایاں--)
        {
          صحیح controllers_written;
          bool repeat = false;

          حافظہ_نقل (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          برائے (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (تہی, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, لڑی_لمبائی (token), err);
              اگر (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  اگر (e == EBUSY)
                    repeat = true;

                  جاری;
                }

              controllers_written++;
            }

          اگر (! repeat)
            توڑو;

          /* If there was any controller written, just try once more without any delay.  */
          اگر (controllers_written > 0 && attempts_بایاں > 2)
            {
              attempts_بایاں = 1;
              جاری;
            }

          اگر (attempts_بایاں > 0)
            {
              ساخت timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, تہی);
            }
        }

      /* Refresh what controllers are available.  */
      واپس read_دستیاب_controllers (راستہ, err);
    }

  /* All controllers were enabled successfully.  */
  واپس controllers_مقصد_enable;
}

صحیح
enable_controllers (ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *tmp_راستہ = تہی;
  حرف *it;
  صحیح ret, controllers_مقصد_enable;

  xasprintf (&tmp_راستہ, "%s/", راستہ);

  ret = read_دستیاب_controllers ("", err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  controllers_مقصد_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_مقصد_enable, err);
  اگر (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      اگر ((controllers_مقصد_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_مقصد_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_مقصد_enable, err);
        }
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  برائے (it = strchr (tmp_راستہ + 1, '/'); it;)
    {
      cleanup_free حرف *cgroup_راستہ = تہی;
      حرف *اگلا_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, tmp_راستہ, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = mkdir (cgroup_راستہ, 0755);
      اگر (UNLIKELY (ret < 0 && errno != EEXIST))
        واپس crun_make_error (err, errno, "create `%s`", cgroup_راستہ);

      اگر (اگلا_slash)
        {
          ret = write_controller_file (tmp_راستہ, controllers_مقصد_enable, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      *it = '/';
      it = اگلا_slash;
    }
  واپس 0;
}

صحیح
libcrun_move_process_مقصد_cgroup (pid_t pid, pid_t init_pid, ثابت حرف *راستہ, bool create_if_missing, libcrun_error_t *err)
{
  صحیح cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  اگر (راستہ == تہی || *راستہ == '\0')
    واپس 0;

  واپس enter_cgroup (cgroup_منوال, pid, init_pid, راستہ, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
صحیح
libcrun_move_self_مقصد_cgroups (ثابت حرف *content, libcrun_error_t *err)
{
  cleanup_free حرف *buf = xstrdup (content);
  حرف *saveptr = تہی;
  حرف *controller;
  حرف *راستہ;
  صحیح cgroup_منوال;
  bool has_ڈیٹا;
  صحیح ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  برائے (has_ڈیٹا = read_proc_cgroup (buf, &saveptr, تہی, &controller, &راستہ);
       has_ڈیٹا;
       has_ڈیٹا = read_proc_cgroup (تہی, &saveptr, تہی, &controller, &راستہ))
    {
      ثابت حرف *subsystem = تہی;

      اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          اگر (controller[0] != '\0')
            جاری;
        }
      ورنہ
        {
          cleanup_free حرف *subsystem_راستہ = تہی;

          /* Use the same names as enter_cgroup_v1 does.  */
          اگر (has_سابقہ (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          اگر (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          اگر (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_راستہ, err, CGROUP_جڑ, subsystem, تہی);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          ret = crun_راستہ_exists (subsystem_راستہ, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          اگر (ret == 0)
            جاری;
        }

      ret = move_process_مقصد_cgroup (0, subsystem, راستہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

صحیح
libcrun_get_cgroup_dirfd (ساخت libcrun_cgroup_حالت *حالت_2, ثابت حرف *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free حرف *راستہ_مقصد_cgroup = تہی;
  صحیح cgroup_منوال;
  صحیح cgroupdirfd;
  صحیح ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (cgroup_منوال < 0)
    واپس cgroup_منوال;

  اگر (cgroup_منوال != CGROUP_منوال_UNIFIED)
    واپس crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  اگر (حالت_2 == تہی)
    واپس crun_make_error (err, 0, "internal error");

  اگر (is_خالی_خانے_string (حالت_2->راستہ))
    واپس crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&راستہ_مقصد_cgroup, err, CGROUP_جڑ, حالت_2->راستہ, sub_cgroup, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  cgroupdirfd = open (راستہ_مقصد_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_راستہ);
  اگر (UNLIKELY (cgroupdirfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", راستہ_مقصد_cgroup);

  واپس cgroupdirfd;
}

صحیح
libcrun_migrate_all_pids_مقصد_cgroup (pid_t init_pid, حرف *from, حرف *مقصد, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = تہی;
  cleanup_close صحیح بچہ_dfd = -1;
  صحیح cgroup_منوال;
  size_t from_len;
  size_t i;
  صحیح ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (cgroup_منوال < 0)
    واپس cgroup_منوال;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_راستہ (from, true, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_cgroup_read_pids_from_راستہ (from, true, &pids, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ thaw;

  from_len = لڑی_لمبائی (from);

  برائے (i = 0; pids && pids[i]; i++)
    {
      cleanup_free حرف *pid_راستہ = تہی;
      cleanup_free حرف *dest_cgroup = تہی;

      ret = libcrun_get_cgroup_process (pids[i], &pid_راستہ, false, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      اگر (! has_سابقہ (pid_راستہ, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          جاؤ thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", مقصد, pid_راستہ + from_len);

      ret = enter_cgroup (cgroup_منوال, pids[i], init_pid, dest_cgroup, false, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ thaw;
    }

  ret = 0;

thaw:
  اگر (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = تہی;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      اگر (libcrun_cgroup_pause_unpause_راستہ (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      واپس ret;
    }

  ret = libcrun_cgroup_pause_unpause_راستہ (from, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس destroy_cgroup_راستہ (from, cgroup_منوال, err);
}

صحیح
get_cgroup_dirfd_راستہ (صحیح dirfd, حرف **راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_راستہ = تہی;
  proc_fd_راستہ_t fd_راستہ;
  ssize_t len;

  get_proc_self_fd_راستہ (fd_راستہ, dirfd);

  len = محفوظ_readlinkat (AT_FDCWD, fd_راستہ, &cgroup_راستہ, 0, err);
  اگر (UNLIKELY (len < 0))
    واپس len;

  اگر (has_سابقہ (cgroup_راستہ, CGROUP_جڑ))
    {
      *راستہ = xstrdup (cgroup_راستہ + لڑی_لمبائی (CGROUP_جڑ));
      واپس 0;
    }
  واپس crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_راستہ);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
صحیح
parse_cpu_max (ثابت حرف *قدر, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ثابت حرف *it = قدر;
  حرف *endptr = تہی;

  جب (*it == ' ')
    it++;

  errno = 0;
  اگر (has_سابقہ (it, "max"))
    {
      *quota = -1;
      endptr = (حرف *) it + 3;
    }
  ورنہ
    {
      *quota = strtoll (it, &endptr, 10);
      اگر (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        واپس crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", قدر);
    }

  it = endptr;
  جب (*it == ' ')
    it++;

  *has_period = false;
  اگر (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      اگر (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        واپس crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", قدر);
      *has_period = true;
    }

  واپس 0;
}
