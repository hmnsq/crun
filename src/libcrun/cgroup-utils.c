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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-internal.h"
#ಅನ್ತರ್ಭಾವ "cgroup-systemd.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"
#ಅನ್ತರ್ಭಾವ "cgroup-setup.h"
#ಅನ್ತರ್ಭಾವ "ebpf.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <poll.h>
#ಅನ್ತರ್ಭಾವ <sys/inotify.h>

#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>

ರಚನೆ symlink_s
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ;
};

ಸ್ಥಿರ ರಚನೆ symlink_s cgroup_symlinks[] = { { "cpu", "cpu,cpuacct" },
                                              { "cpuacct", "cpu,cpuacct" },
                                              { "net_cls", "net_cls,net_prio" },
                                              { "net_prio", "net_cls,net_prio" },
                                              { ಖಾಲಿ, ಖಾಲಿ } };

ಪೂರ್ಣಾಂಕ
libcrun_cgroups_create_symlinks (ಪೂರ್ಣಾಂಕ dirfd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ i;

  ಗಾಗಿ (i = 0; cgroup_symlinks[i].name; i++)
    {
      ಪೂರ್ಣಾಂಕ ret;

      ret = symlinkat (cgroup_symlinks[i].ಗುರಿ, dirfd, cgroup_symlinks[i].name);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (errno == ENOENT || errno == EEXIST)
            ಮುಂದುವರಿ;
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "symlinkat `%s`", cgroup_symlinks[i].name);
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
maybe_make_cgroup_threaded (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ_type = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ threaded = "threaded";
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೋಷಕ_2;
  size_t ಗಾತ್ರ_2;
  ಅಕ್ಷರ *it;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (ಮಾರ್ಗ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಮಾರ್ಗ = consume_slashes (ಮಾರ್ಗ);

  ಆದರೆ (ಮಾರ್ಗ[0] == '\0')
    ಹಿಂತಿರುಗಿ 0;

  ret = append_paths (&cgroup_ಮಾರ್ಗ_type, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, "cgroup.type", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = read_all_file (cgroup_ಮಾರ್ಗ_type, &content, &ಗಾತ್ರ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ಗಾತ್ರ_2 > 0)
    {
      it = content + ಗಾತ್ರ_2 - 1;
      ಆಗ (*it == '\n' && it > content)
        *it-- = '\0';
    }

  ಆದರೆ (strcmp (content, "domain") == 0 || strcmp (content, "domain threaded") == 0)
    ಹಿಂತಿರುಗಿ 0;

  ಬಫರ್ = xstrdup (ಮಾರ್ಗ);
  ಪೋಷಕ_2 = consume_slashes (dirname (ಬಫರ್));
  ಆದರೆ (ಪೋಷಕ_2[0] && strcmp (ಪೋಷಕ_2, "."))
    {
      ret = maybe_make_cgroup_threaded (ಪೋಷಕ_2, err);
      ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ write_cgroup_file (AT_FDCWD, cgroup_ಮಾರ್ಗ_type, threaded, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (threaded), err);
}

ಪೂರ್ಣಾಂಕ
move_process_ಗಮ್ಯ_cgroup (pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subsystem, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ_procs = ಖಾಲಿ;
  ಅಕ್ಷರ pid_str[16];
  ಪೂರ್ಣಾಂಕ ret;

  ret = append_paths (&cgroup_ಮಾರ್ಗ_procs, err, CGROUP_ಬೇರು,
                      subsystem ? subsystem : "", ಮಾರ್ಗ ? ಮಾರ್ಗ : "",
                      "cgroup.procs", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = snprintf (pid_str, ಗಾತ್ರ (pid_str), "%d", pid);
  ಆದರೆ (UNLIKELY (ret >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (pid_str)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_cgroup_file (AT_FDCWD, cgroup_ಮಾರ್ಗ_procs, pid_str, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (pid_str), err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (crun_error_get_errno (err) == EOPNOTSUPP)
        {
          libcrun_error_t tmp_err = ಖಾಲಿ;
          ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;

          ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (&tmp_err);
          ಆದರೆ (UNLIKELY (ಬಹುಲಕ < 0 || ಬಹುಲಕ != CGROUP_ಬಹುಲಕ_UNIFIED))
            {
              crun_error_release (&tmp_err);
              ಹಿಂತಿರುಗಿ ret;
            }

          crun_error_release (err);

          ret = maybe_make_cgroup_threaded (ಮಾರ್ಗ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          ಹಿಂತಿರುಗಿ write_cgroup_file (AT_FDCWD, cgroup_ಮಾರ್ಗ_procs, pid_str, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (pid_str), err);
        }

      ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ ret;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CGROUP2_SUPER_MAGIC
#  define CGROUP2_SUPER_MAGIC 0x63677270
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ TMPFS_MAGIC
#  define TMPFS_MAGIC 0x01021994
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
detect_cgroup_ಬಹುಲಕ (libcrun_error_t *err)
{
  ರಚನೆ statfs stat;
  ಪೂರ್ಣಾಂಕ ret;

  ret = statfs (CGROUP_ಬೇರು, &stat);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "statfs `" CGROUP_ಬೇರು "`");
  ಆದರೆ (stat.f_type == CGROUP2_SUPER_MAGIC)
    ಹಿಂತಿರುಗಿ CGROUP_ಬಹುಲಕ_UNIFIED;
  ಆದರೆ (stat.f_type != TMPFS_MAGIC)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid file system type on `" CGROUP_ಬೇರು "`");
  ret = statfs (CGROUP_ಬೇರು "/unified", &stat);
  ಆದರೆ (ret < 0 && errno != ENOENT)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "statfs `" CGROUP_ಬೇರು "/unified`");
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ CGROUP_ಬಹುಲಕ_LEGACY;
  ಹಿಂತಿರುಗಿ stat.f_type == CGROUP2_SUPER_MAGIC ? CGROUP_ಬಹುಲಕ_HYBRID : CGROUP_ಬಹುಲಕ_LEGACY;
}

ಪೂರ್ಣಾಂಕ
libcrun_get_cgroup_ಬಹುಲಕ (libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ tmp;
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ = 0;

  ಆದರೆ (cgroup_ಬಹುಲಕ)
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  tmp = detect_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (tmp < 0))
    ಹಿಂತಿರುಗಿ tmp;

  cgroup_ಬಹುಲಕ = tmp;

  ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_cgroup_process_from_content (ಅಕ್ಷರ *content, ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ, ಅಕ್ಷರ **ಮಾರ್ಗ, bool absolute, libcrun_error_t *err)
{
  ಅಕ್ಷರ *cg_ಮಾರ್ಗ = ಖಾಲಿ;
  ಅಕ್ಷರ *controller;
  ಅಕ್ಷರ *saveptr;
  bool has_ದತ್ತಾಂಶ;

  ಗಾಗಿ (has_ದತ್ತಾಂಶ = read_proc_cgroup (content, &saveptr, ಖಾಲಿ, &controller, &cg_ಮಾರ್ಗ);
       has_ದತ್ತಾಂಶ;
       has_ದತ್ತಾಂಶ = read_proc_cgroup (ಖಾಲಿ, &saveptr, ಖಾಲಿ, &controller, &cg_ಮಾರ್ಗ))
    {
      ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
        {
          ಆದರೆ (strcmp (controller, "") == 0 && ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (cg_ಮಾರ್ಗ) > 0)
            ಹೋಗು ಸಿಕ್ಕಿತು;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (strcmp (controller, "memory"))
            ಹೋಗು ಸಿಕ್ಕಿತು;
        }
    }

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot find cgroup for the process");

ಸಿಕ್ಕಿತು:
  ಆದರೆ (absolute)
    ಹಿಂತಿರುಗಿ append_paths (ಮಾರ್ಗ, err, CGROUP_ಬೇರು, cg_ಮಾರ್ಗ, ಖಾಲಿ);

  *ಮಾರ್ಗ = xstrdup (cg_ಮಾರ್ಗ);
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_get_cgroup_process_at (ಪೂರ್ಣಾಂಕ dirfd, ಅಕ್ಷರ **ಮಾರ್ಗ, bool absolute, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  size_t content_ಗಾತ್ರ;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ret = read_all_file_at (dirfd, SELF_CGROUP, &content, &content_ಗಾತ್ರ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ get_cgroup_process_from_content (content, cgroup_ಬಹುಲಕ, ಮಾರ್ಗ, absolute, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_get_cgroup_process (pid_t pid, ಅಕ್ಷರ **ಮಾರ್ಗ, bool absolute, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  ಅಕ್ಷರ proc_cgroup_file[64];
  size_t content_ಗಾತ್ರ;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (pid == 0)
    strcpy (proc_cgroup_file, PROC_SELF_CGROUP);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಪೂರ್ಣಾಂಕ len = snprintf (proc_cgroup_file, ಗಾತ್ರ (proc_cgroup_file), "/proc/%d/cgroup", pid);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (proc_cgroup_file)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
    }

  ret = read_all_file (proc_cgroup_file, &content, &content_ಗಾತ್ರ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ get_cgroup_process_from_content (content, cgroup_ಬಹುಲಕ, ಮಾರ್ಗ, absolute, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_pids_file (ಪೂರ್ಣಾಂಕ dfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ tasksfd = -1;
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  ಅಕ್ಷರ *saveptr = ಖಾಲಿ;
  size_t n_new_pids;
  size_t len;
  ಅಕ್ಷರ *it;
  ಪೂರ್ಣಾಂಕ ret;

  tasksfd = openat (dfd, file, O_RDONLY | O_CLOEXEC);
  ಆದರೆ (tasksfd < 0)
    {
      /* The cgroup was removed concurrently (e.g. while killing the
         container), there are no processes left to account for.  */
      ಆದರೆ (errno == ENOENT || errno == ENODEV)
        ಹಿಂತಿರುಗಿ 0;
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", file);
    }

  ret = read_all_fd (tasksfd, file, &ಬಫರ್, &len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (len > 0)
    {
      ಗಾಗಿ (n_new_pids = 0, it = ಬಫರ್; it; it = strchr (it + 1, '\n'))
        n_new_pids++;

      ಆದರೆ (*allocated < *n_pids + n_new_pids + 1)
        {
          *allocated = *n_pids + n_new_pids + 1;
          *pids = xrealloc (*pids, ಗಾತ್ರ (pid_t) * *allocated);
        }

      ಗಾಗಿ (it = strtok_r (ಬಫರ್, "\n", &saveptr); it; it = strtok_r (ಖಾಲಿ, "\n", &saveptr))
        {
          pid_t pid = strtoul (it, ಖಾಲಿ, 10);

          ಆದರೆ (pid > 0)
            (*pids)[(*n_pids)++] = pid;
        }
      (*pids)[*n_pids] = 0;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* A sub-cgroup still to be visited by a walk of the cgroup tree.  */
ರಚನೆ cgroup_walk_dir_s
{
  ಅಕ್ಷರ *name;
  size_t ಆಳ;
  bool entered;
};

/* Depth-first walk of a cgroup tree.  The shape of the tree is chosen by the
   container, so the walk must not recurse: one open directory per level runs
   out of file descriptors on a deep chain of sub-cgroups.  Only the current
   directory is kept open, the sub-cgroups still to visit are queued by name
   and the walk goes back to a parent through "..".  */
ರಚನೆ cgroup_walk_s
{
  ಪೂರ್ಣಾಂಕ dfd;
  size_t ಆಳ;
  ರಚನೆ cgroup_walk_dir_s *dirs;
  size_t n_dirs;
  size_t allocated;
};

ಸ್ಥಿರ ಶೂನ್ಯ
cgroup_walk_ತೆಗೆ (ರಚನೆ cgroup_walk_s *walk)
{
  ಬಿಡುಗಡೆ (walk->dirs[--walk->n_dirs].name);
}

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup_cgroup_walkp (ರಚನೆ cgroup_walk_s *walk)
{
  ಪೂರ್ಣಾಂಕ saved_errno = errno;

  ಆಗ (walk->n_dirs > 0)
    cgroup_walk_ತೆಗೆ (walk);
  ಬಿಡುಗಡೆ (walk->dirs);
  close_and_reset (&walk->dfd);
  errno = saved_errno;
}

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_cgroup_walk __attribute__ ((cleanup (cleanup_cgroup_walkp)))

/* Make DFD the current directory of the walk and queue its sub-directories.
   DFD must be the root of the walk or a child of the current directory, and
   it is owned by the walk afterwards.  On error -1 is returned with errno set.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cgroup_walk_enter (ರಚನೆ cgroup_walk_s *walk, ಪೂರ್ಣಾಂಕ dfd)
{
  cleanup_close ಪೂರ್ಣಾಂಕ dfd_clone = -1;
  cleanup_dir DIR *dir = ಖಾಲಿ;
  ರಚನೆ dirent *de;

  close_and_reset (&walk->dfd);
  walk->dfd = dfd;
  walk->ಆಳ++;

  dfd_clone = openat (dfd, ".", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (dfd_clone < 0))
    ಹಿಂತಿರುಗಿ -1;

  dir = fdopendir (dfd_clone);
  ಆದರೆ (UNLIKELY (dir == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ -1;
  /* fdopendir takes ownership of dfd_clone.  */
  dfd_clone = -1;

  ಗಾಗಿ (de = readdir (dir); de; de = readdir (dir))
    {
      ರಚನೆ cgroup_walk_dir_s *d;

      ಆದರೆ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ಮುಂದುವರಿ;

      ಆದರೆ (de->d_type != DT_DIR)
        ಮುಂದುವರಿ;

      ಆದರೆ (walk->n_dirs == walk->allocated)
        {
          walk->allocated = walk->allocated * 2 + 32;
          walk->dirs = xrealloc (walk->dirs, ಗಾತ್ರ (*walk->dirs) * walk->allocated);
        }

      d = &walk->dirs[walk->n_dirs++];
      d->name = xstrdup (de->d_name);
      d->ಆಳ = walk->ಆಳ + 1;
      d->entered = false;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Store in *D the next queued sub-directory, or NULL once there are no more,
   and make its parent the current directory of the walk.  On error -1 is
   returned with errno set.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cgroup_walk_ಮುಂದಿನ (ರಚನೆ cgroup_walk_s *walk, ರಚನೆ cgroup_walk_dir_s **d)
{
  *d = walk->n_dirs > 0 ? &walk->dirs[walk->n_dirs - 1] : ಖಾಲಿ;

  ಆಗ (*d && walk->ಆಳ >= (*d)->ಆಳ)
    {
      ಪೂರ್ಣಾಂಕ ಪೋಷಕ_2;

      ಪೋಷಕ_2 = openat (walk->dfd, "..", O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (ಪೋಷಕ_2 < 0))
        ಹಿಂತಿರುಗಿ -1;

      close_and_reset (&walk->dfd);
      walk->dfd = ಪೋಷಕ_2;
      walk->ಆಳ--;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_pids_cgroup_file (ಪೂರ್ಣಾಂಕ dfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  cleanup_cgroup_walk ರಚನೆ cgroup_walk_s walk = { .dfd = -1 };
  ಪೂರ್ಣಾಂಕ dfd_clone;
  ಪೂರ್ಣಾಂಕ ret;

  ret = read_pids_file (dfd, file, pids, n_pids, allocated, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (! recurse)
    ಹಿಂತಿರುಗಿ 0;

  dfd_clone = openat (dfd, ".", O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (dfd_clone < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open cgroup sub-directory");

  ret = cgroup_walk_enter (&walk, dfd_clone);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open cgroup sub-directory");

  ಗಾಗಿ (;;)
    {
      ರಚನೆ cgroup_walk_dir_s *d;
      cleanup_close ಪೂರ್ಣಾಂಕ nfd = -1;

      ret = cgroup_walk_ಮುಂದಿನ (&walk, &d);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open parent cgroup directory");
      ಆದರೆ (d == ಖಾಲಿ)
        ನಿಲ್ಲಿಸು;

      nfd = openat (walk.dfd, d->name, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (nfd < 0))
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno;

          ಆದರೆ (saved_errno == ENOENT)
            {
              cgroup_walk_ತೆಗೆ (&walk);
              ಮುಂದುವರಿ;
            }
          ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "open cgroup directory `%s`", d->name);
        }

      cgroup_walk_ತೆಗೆ (&walk);

      ret = read_pids_file (nfd, file, pids, n_pids, allocated, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಂಕ e = crun_error_get_errno (err);

          /* Reading `cgroup.procs` of a threaded cgroup fails with
             EOPNOTSUPP.  Its processes are already listed by its
             threaded domain root, which was read before.  A cgroup
             removed meanwhile has nothing left to list.  */
          ಆದರೆ (e != EOPNOTSUPP && e != ENOENT && e != ENODEV)
            ಹಿಂತಿರುಗಿ ret;
          crun_error_release (err);
          ಮುಂದುವರಿ;
        }

      ret = cgroup_walk_enter (&walk, nfd);
      nfd = -1;
      ಆದರೆ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open cgroup sub-directory");
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_pids_cgroup (ಪೂರ್ಣಾಂಕ dfd, bool recurse, pid_t **pids, size_t *n_pids, size_t *allocated, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = read_pids_cgroup_file (dfd, "cgroup.procs", recurse, pids, n_pids, allocated, err);
  ಆದರೆ (ret < 0 && crun_error_get_errno (err) == EOPNOTSUPP)
    {
      /* A threaded cgroup has no `cgroup.procs`, list its threads instead.  */
      crun_error_release (err);
      ret = read_pids_cgroup_file (dfd, "cgroup.threads", recurse, pids, n_pids, allocated, err);
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
rmdir_all_fd (ಪೂರ್ಣಾಂಕ dfd)
{
  cleanup_cgroup_walk ರಚನೆ cgroup_walk_s walk = { .dfd = -1 };
  ಪೂರ್ಣಾಂಕ ret;

  ret = cgroup_walk_enter (&walk, dfd);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಗಾಗಿ (;;)
    {
      ರಚನೆ cgroup_walk_dir_s *d;
      ಪೂರ್ಣಾಂಕ ಮಗು_dfd;

      ret = cgroup_walk_ಮುಂದಿನ (&walk, &d);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      ಆದರೆ (d == ಖಾಲಿ)
        ನಿಲ್ಲಿಸು;

      ret = unlinkat (walk.dfd, d->name, AT_REMOVEDIR);
      ಆದರೆ (ret == 0 || errno != EBUSY || d->entered)
        {
          cgroup_walk_ತೆಗೆ (&walk);
          ಮುಂದುವರಿ;
        }

      ಮಗು_dfd = openat (walk.dfd, d->name, O_DIRECTORY | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (ಮಗು_dfd < 0))
        {
          ಆದರೆ (errno == ENOENT)
            {
              cgroup_walk_ತೆಗೆ (&walk);
              ಮುಂದುವರಿ;
            }
          ಹಿಂತಿರುಗಿ ಮಗು_dfd;
        }

      /* Keep the directory queued, so that removing it is retried once its
         children are gone.  */
      d->entered = true;

      ret = cgroup_walk_enter (&walk, ಮಗು_dfd);
      ಆದರೆ (UNLIKELY (ret < 0) && errno != ENOENT && errno != ENODEV)
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
rmdir_all (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ dfd = open (ಮಾರ್ಗ, O_DIRECTORY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (dfd < 0))
    ಹಿಂತಿರುಗಿ dfd;

  ret = rmdir_all_fd (dfd);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ rmdir (ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_pids_from_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool recurse, bool ಎಳೆಗಳು, pid_t **pids, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ dirfd = -1;
  size_t n_pids, allocated;
  ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (ಮಾರ್ಗ == ಖಾಲಿ || *ಮಾರ್ಗ == '\0')
    ಹಿಂತಿರುಗಿ 0;

  ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ ಬಹುಲಕ;

  ಆಯ್ಕೆ (ಬಹುಲಕ)
    {
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_UNIFIED:
      ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_HYBRID:
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_LEGACY:
      ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು "/memory", ಮಾರ್ಗ, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid cgroup mode `%d`", ಬಹುಲಕ);
    }

  dirfd = open (cgroup_ಮಾರ್ಗ, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (dirfd < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", cgroup_ಮಾರ್ಗ);

  n_pids = 0;
  allocated = 0;

  /* On cgroup v1 `cgroup.procs` lists the thread group of every task in the
     cgroup, so there is no need to read `tasks` there.  */
  ಆದರೆ (ಎಳೆಗಳು && ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
    ಹಿಂತಿರುಗಿ read_pids_cgroup_file (dirfd, "cgroup.threads", recurse, pids, &n_pids, &allocated, err);

  ಹಿಂತಿರುಗಿ read_pids_cgroup (dirfd, recurse, pids, &n_pids, &allocated, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_read_pids_from_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ read_pids_from_ಮಾರ್ಗ (ಮಾರ್ಗ, recurse, false, pids, err);
}

/* same semantic as strtok_r.  */
bool
read_proc_cgroup (ಅಕ್ಷರ *content, ಅಕ್ಷರ **saveptr, ಅಕ್ಷರ **id, ಅಕ್ಷರ **controller_list, ಅಕ್ಷರ **ಮಾರ್ಗ)
{
  ಅಕ್ಷರ *it;

  it = strtok_r (content, "\n", saveptr);
  ಆದರೆ (it == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (id)
    *id = it;

  it = strchr (it, ':');
  ಆದರೆ (it == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;
  *it++ = '\0';

  ಆದರೆ (controller_list)
    *controller_list = it;

  it = strchr (it, ':');
  ಆದರೆ (it == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;
  *it++ = '\0';

  ಆದರೆ (ಮಾರ್ಗ)
    *ಮಾರ್ಗ = it;

  ಹಿಂತಿರುಗಿ true;
}

ಪೂರ್ಣಾಂಕ
destroy_cgroup_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, libcrun_error_t *err)
{
  bool repeat = true;
  ಪೂರ್ಣಾಂಕ retry_ಎಣಿಕೆ = 0;
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ max_attempts = 500;
  ಪೂರ್ಣಾಂಕ ret;

  /* Without a path there is nothing to destroy.  Bail out early, since
     append_paths() stops at the first NULL argument and would otherwise
     be left pointing at the cgroup root.  */
  ಆದರೆ (is_ಖಾಲಿಗಳು_string (ಮಾರ್ಗ))
    ಹಿಂತಿರುಗಿ 0;

  ಮಾಡು
    {
      repeat = false;

      ಆದರೆ (ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
        {
          cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;

          ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, ಖಾಲಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          ret = rmdir (cgroup_ಮಾರ್ಗ);
          ಆದರೆ (ret < 0 && errno == EBUSY)
            {
              ret = rmdir_all (cgroup_ಮಾರ್ಗ);
              ಆದರೆ (ret < 0)
                {
                  ಆದರೆ (retry_ಎಣಿಕೆ >= max_attempts)
                    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot delete path `%s`", cgroup_ಮಾರ್ಗ);

                  retry_ಎಣಿಕೆ++;
                  repeat = true;
                }
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
          size_t content_ಗಾತ್ರ;
          ಅಕ್ಷರ *controller;
          ಅಕ್ಷರ *saveptr;
          bool has_ದತ್ತಾಂಶ;

          ret = read_all_file (PROC_SELF_CGROUP, &content, &content_ಗಾತ್ರ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ಆದರೆ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);
                  ಹಿಂತಿರುಗಿ 0;
                }
              ಹಿಂತಿರುಗಿ ret;
            }

          ಗಾಗಿ (has_ದತ್ತಾಂಶ = read_proc_cgroup (content, &saveptr, ಖಾಲಿ, &controller, ಖಾಲಿ);
               has_ದತ್ತಾಂಶ;
               has_ದತ್ತಾಂಶ = read_proc_cgroup (ಖಾಲಿ, &saveptr, ಖಾಲಿ, &controller, ಖಾಲಿ))
            {
              cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
              ಅಕ್ಷರ *subsystem;
              ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (controller, "name="))
                controller += 5;

              subsystem = controller[0] == '\0' ? "unified" : controller;
              ಆದರೆ (ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_LEGACY && strcmp (subsystem, "unified") == 0)
                ಮುಂದುವರಿ;

              ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, subsystem, ಮಾರ್ಗ, ಖಾಲಿ);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;

              ret = rmdir (cgroup_ಮಾರ್ಗ);
              ಆದರೆ (ret < 0 && errno == EBUSY)
                {
                  ret = rmdir_all (cgroup_ಮಾರ್ಗ);
                  ಆದರೆ (ret < 0)
                    {
                      ಆದರೆ (retry_ಎಣಿಕೆ >= max_attempts)
                        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot destroy subsystem `%s` at path `%s`", subsystem, cgroup_ಮಾರ್ಗ);
                      retry_ಎಣಿಕೆ++;
                      repeat = true;
                    }
                }
            }
        }

      ಆದರೆ (repeat)
        {
          ರಚನೆ timespec req = {
            .tv_sec = 0,
            .tv_nsec = 10000000,
          };

          nanosleep (&req, ಖಾಲಿ);

          ret = cgroup_killall_ಮಾರ್ಗ (ಮಾರ್ಗ, SIGKILL, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            crun_error_release (err);
        }
  } ಆಗ (repeat);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
chown_cgroups (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, uid_t uid, gid_t gid, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *delegate = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ dfd = -1;
  size_t delegate_ಗಾತ್ರ;
  ಅಕ್ಷರ *saveptr = ಖಾಲಿ;
  ಅಕ್ಷರ *name;
  ಪೂರ್ಣಾಂಕ ret;

  ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  dfd = open (cgroup_ಮಾರ್ಗ, O_CLOEXEC | O_ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (dfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", cgroup_ಮಾರ್ಗ);

  ret = read_all_file ("/sys/kernel/cgroup/delegate", &delegate, &delegate_ಗಾತ್ರ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಹಿಂತಿರುಗಿ 0;
        }
      ಹಿಂತಿರುಗಿ ret;
    }

  ret = fchownat (dfd, "", uid, gid, AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ | AT_SYMLINK_NOFOLLOW);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot chown `%s`", cgroup_ಮಾರ್ಗ);

  ಗಾಗಿ (name = strtok_r (delegate, "\n", &saveptr); name; name = strtok_r (ಖಾಲಿ, "\n", &saveptr))
    {
      ret = fchownat (dfd, name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (errno == ENOENT)
            ಮುಂದುವರಿ;

          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot chown `%s/%s`", cgroup_ಮಾರ್ಗ, name);
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_cgroup_pause_unpause_with_ಬಹುಲಕ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ bool pause,
                                        libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state = "";
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (cgroup_ಮಾರ್ಗ == ಖಾಲಿ || cgroup_ಮಾರ್ಗ[0] == '\0')
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot %s the container without a cgroup", pause ? "pause" : "resume");

  ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
    {
      state = pause ? "1" : "0";
      ret = append_paths (&ಮಾರ್ಗ, err, CGROUP_ಬೇರು, cgroup_ಮಾರ್ಗ, "cgroup.freeze", ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      state = pause ? "FROZEN" : "THAWED";
      ret = append_paths (&ಮಾರ್ಗ, err, CGROUP_ಬೇರು "/freezer", cgroup_ಮಾರ್ಗ, "freezer.state", ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = write_cgroup_file (AT_FDCWD, ಮಾರ್ಗ, state, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (state), err);
  ಆದರೆ (ret >= 0)
    ಹಿಂತಿರುಗಿ 0;
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_cgroup_pause_unpause_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ bool pause, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಹಿಂತಿರುಗಿ libcrun_cgroup_pause_unpause_with_ಬಹುಲಕ (cgroup_ಮಾರ್ಗ, cgroup_ಬಹುಲಕ, pause, err);
}

/* Fail if the cgroup at CGROUP_PATH exists and is frozen: a process in a
   frozen cgroup cannot make any progress, so the container creation would
   hang forever.  */
ಪೂರ್ಣಾಂಕ
libcrun_cgroup_ensure_not_frozen (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *frozen;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
    {
      frozen = "1";
      ret = append_paths (&ಮಾರ್ಗ, err, CGROUP_ಬೇರು, cgroup_ಮಾರ್ಗ, "cgroup.freeze", ಖಾಲಿ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      frozen = "FROZEN";
      ret = append_paths (&ಮಾರ್ಗ, err, CGROUP_ಬೇರು "/freezer", cgroup_ಮಾರ್ಗ, "freezer.state", ಖಾಲಿ);
    }
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = read_all_file (ಮಾರ್ಗ, &content, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* Either the cgroup does not exist yet, or there is no freezer.  */
      ಆದರೆ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಹಿಂತಿರುಗಿ 0;
        }
      ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (content, frozen))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "container's cgroup unexpectedly frozen");

  ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು CGROUP_KILL_WAIT_SEC 1

/* Watch cgroup.events of PATH, so that a later wait_cgroup_unpopulated does
   not miss a change.  Return -1 if the watch cannot be set up.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
watch_cgroup_events (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
  cleanup_free ಅಕ್ಷರ *events_file = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  libcrun_error_t tmp_err = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ಬೇರು, ಮಾರ್ಗ, "cgroup.events", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ಹಿಂತಿರುಗಿ -1;
    }

  fd = inotify_init1 (ಒಳ_CLOEXEC | ಒಳ_NONBLOCK);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ -1;

  ret = inotify_ಕೂಡಿಸು_watch (fd, events_file, ಒಳ_MODIFY);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ -1;

  ret = fd;
  fd = -1;
  ಹಿಂತಿರುಗಿ ret;
}

/* Wait up to a short timeout for the cgroup to have no processes left.
   Return 1 if the cgroup is not populated anymore, 0 otherwise.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
wait_cgroup_unpopulated (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ inotify_fd)
{
  cleanup_free ಅಕ್ಷರ *events_file = ಖಾಲಿ;
  libcrun_error_t tmp_err = ಖಾಲಿ;
  ರಚನೆ timespec timeout = {
    .tv_sec = CGROUP_KILL_WAIT_SEC,
    .tv_nsec = 0,
  };
  ಪೂರ್ಣಾಂಕ ret;

  ret = append_paths (&events_file, &tmp_err, CGROUP_ಬೇರು, ಮಾರ್ಗ, "cgroup.events", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ಹಿಂತಿರುಗಿ 0;
    }

  ಗಾಗಿ (;;)
    {
      cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
      ಅಕ್ಷರ buf[4096];
      ರಚನೆ pollfd pfd = { .fd = inotify_fd, .events = POLLIN };

      ret = read_all_file (events_file, &content, ಖಾಲಿ, &tmp_err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          crun_error_release (&tmp_err);
          ಹಿಂತಿರುಗಿ 0;
        }

      ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (content, "populated 0"))
        ಹಿಂತಿರುಗಿ 1;

      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ppoll (&pfd, 1, &timeout, ಖಾಲಿ));
      ಆದರೆ (ret <= 0)
        ಹಿಂತಿರುಗಿ 0;

      ಆಗ (ಪಠ (inotify_fd, buf, ಗಾತ್ರ (buf)) > 0)
        ;
    }
}

ಪೂರ್ಣಾಂಕ
cgroup_killall_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಸಂಕೇತ, libcrun_error_t *err)
{
  bool frozen = false;
  cleanup_close ಪೂರ್ಣಾಂಕ inotify_fd = -1;
  ಪೂರ್ಣಾಂಕ ret;
  size_t i;
  cleanup_free pid_t *pids = ಖಾಲಿ;

  ಆದರೆ (ಮಾರ್ಗ == ಖಾಲಿ || *ಮಾರ್ಗ == '\0')
    ಹಿಂತಿರುಗಿ 0;

  /* If the signal is SIGKILL, try to kill each process using the `cgroup.kill` file.
     The kernel signals the thread group leaders, and that has no effect on a
     process whose leader already exited while other threads are still running.
     If the cgroup is not empty shortly after the write, go on and signal
     whatever is still listed in the cgroup.  */
  ಆದರೆ (ಸಂಕೇತ == SIGKILL)
    {
      cleanup_free ಅಕ್ಷರ *kill_file = ಖಾಲಿ;

      ret = append_paths (&kill_file, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, "cgroup.kill", ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      inotify_fd = watch_cgroup_events (ಮಾರ್ಗ);

      ret = write_cgroup_file (AT_FDCWD, kill_file, "1", 1, err);
      ಆದರೆ (ret >= 0 && inotify_fd >= 0 && wait_cgroup_unpopulated (ಮಾರ್ಗ, inotify_fd))
        ಹಿಂತಿರುಗಿ 0;

      ಆದರೆ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  /* Freeze the cgroup so that the processes cannot fork while they are being
     killed.  Once frozen, make sure the cgroup is always thawed again on any
     exit path, otherwise the container would be left frozen and unresponsive.  */
  ret = libcrun_cgroup_pause_unpause_ಮಾರ್ಗ (ಮಾರ್ಗ, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    crun_error_release (err);
  ಇಲ್ಲದಿದ್ದರೆ
    frozen = true;

  /* On cgroup v2 a process is listed in `cgroup.procs` of the cgroup of its
     leader, and a process whose leader exited is not listed anywhere once it
     is moved to another cgroup.  For SIGKILL enumerate the threads instead,
     kill() accepts any thread ID and always targets the whole process.  */
  ret = read_pids_from_ಮಾರ್ಗ (ಮಾರ್ಗ, true, ಸಂಕೇತ == SIGKILL, &pids, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (crun_error_get_errno (err) != ENOENT)
        ಹೋಗು thaw;

      /* If the file doesn't exist then the container was already killed.  */
      crun_error_release (err);
    }

  ಗಾಗಿ (i = 0; pids && pids[i]; i++)
    {
      ret = kill (pids[i], ಸಂಕೇತ);
      ಆದರೆ (UNLIKELY (ret < 0 && errno != ESRCH))
        {
          ret = crun_make_error (err, errno, "kill process `%d`", pids[i]);
          ಹೋಗು thaw;
        }
    }

  ret = 0;

thaw:
  ಆದರೆ (frozen)
    {
      libcrun_error_t thaw_err = ಖಾಲಿ;

      /* Thawing is best-effort: do not clobber a pending error with a
         thaw failure, but never leave the cgroup frozen.  */
      ಆದರೆ (libcrun_cgroup_pause_unpause_ಮಾರ್ಗ (ಮಾರ್ಗ, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_ಲಭ್ಯ_controllers (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *controllers = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *buf = ಖಾಲಿ;
  ಅಕ್ಷರ *saveptr = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *token;
  ಪೂರ್ಣಾಂಕ ಲಭ್ಯ = 0;
  ssize_t ret;

  ret = append_paths (&controllers, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, "cgroup.controllers", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = read_all_file (controllers, &buf, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಗಾಗಿ (token = strtok_r (buf, " \n", &saveptr); token; token = strtok_r (ಖಾಲಿ, " \n", &saveptr))
    {
      ಆದರೆ (strcmp (token, "memory") == 0)
        ಲಭ್ಯ |= CGROUP_ಸ್ಮರಣೆ;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (token, "cpu") == 0)
        ಲಭ್ಯ |= CGROUP_CPU;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (token, "cpuset") == 0)
        ಲಭ್ಯ |= CGROUP_CPUSET;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (token, "hugetlb") == 0)
        ಲಭ್ಯ |= CGROUP_HUGETLB;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (token, "pids") == 0)
        ಲಭ್ಯ |= CGROUP_PIDS;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (token, "io") == 0)
        ಲಭ್ಯ |= CGROUP_IO;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (token, "misc") == 0)
        ಲಭ್ಯ |= CGROUP_MISC;
    }
  ಹಿಂತಿರುಗಿ ಲಭ್ಯ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_controller_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ controllers_ಗಮ್ಯ_enable, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *subtree_control = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *controllers = ಖಾಲಿ;
  size_t controllers_len = 0;
  ಪೂರ್ಣಾಂಕ ret;

  controllers_len = xasprintf (
      &controllers, "%s %s %s %s %s %s %s", (controllers_ಗಮ್ಯ_enable & CGROUP_CPU) ? "+cpu" : "",
      (controllers_ಗಮ್ಯ_enable & CGROUP_IO) ? "+io" : "", (controllers_ಗಮ್ಯ_enable & CGROUP_ಸ್ಮರಣೆ) ? "+memory" : "",
      (controllers_ಗಮ್ಯ_enable & CGROUP_PIDS) ? "+pids" : "", (controllers_ಗಮ್ಯ_enable & CGROUP_CPUSET) ? "+cpuset" : "",
      (controllers_ಗಮ್ಯ_enable & CGROUP_HUGETLB) ? "+hugetlb" : "",
      (controllers_ಗಮ್ಯ_enable & CGROUP_MISC) ? "+misc" : "");

  ret = append_paths (&subtree_control, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, "cgroup.subtree_control", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  ret = write_cgroup_file (AT_FDCWD, subtree_control, controllers, controllers_len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      cleanup_free ಅಕ್ಷರ *controllers_copy = xmalloc (controllers_len + 1);
      ಪೂರ್ಣಾಂಕ attempts_ಎಡ;
      ಅಕ್ಷರ *saveptr = ಖಾಲಿ;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *token;
      ಪೂರ್ಣಾಂಕ e;

      e = crun_error_get_errno (err);
      ಆದರೆ (e != EPERM && e != EACCES && e != EBUSY && e != ENOENT && e != EOPNOTSUPP)
        ಹಿಂತಿರುಗಿ crun_error_wrap (err, "enable controllers `%s`", controllers);

      /* ENOENT can mean both that the file doesn't exist or the controller is not present.  */
      ಆದರೆ (e == ENOENT)
        {
          libcrun_error_t tmp_err = ಖಾಲಿ;
          ಪೂರ್ಣಾಂಕ exists;

          exists = crun_ಮಾರ್ಗ_exists (subtree_control, &tmp_err);
          ಆದರೆ (UNLIKELY (exists < 0))
            {
              crun_error_release (&tmp_err);
              ಹಿಂತಿರುಗಿ ret;
            }
          /* If the file doesn't exist, then return the original ENOENT.  */
          ಆದರೆ (exists == 0)
            ಹಿಂತಿರುಗಿ ret;
        }

      crun_error_release (err);

      ಆದರೆ (e == EOPNOTSUPP)
        {
          ret = maybe_make_cgroup_threaded (ಮಾರ್ಗ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_error_wrap (err, "make cgroup threaded");
        }

      /* It seems the kernel can return EBUSY when a process was moved to a sub-cgroup
         and the controllers are enabled in its parent cgroup.  Retry a few times when
         it happens.  */
      ಗಾಗಿ (attempts_ಎಡ = 1000; attempts_ಎಡ >= 0; attempts_ಎಡ--)
        {
          ಪೂರ್ಣಾಂಕ controllers_written;
          bool repeat = false;

          ಸ್ಮರಣೆ_ನಕಲು (controllers_copy, controllers, controllers_len);
          controllers_copy[controllers_len] = '\0';
          controllers_written = 0;

          /* Fallback to write each one individually.  */
          ಗಾಗಿ (token = strtok_r (controllers_copy, " ", &saveptr); token; token = strtok_r (ಖಾಲಿ, " ", &saveptr))
            {
              ret = write_cgroup_file (AT_FDCWD, subtree_control, token, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (token), err);
              ಆದರೆ (ret < 0)
                {
                  e = crun_error_get_errno (err);
                  crun_error_release (err);

                  ಆದರೆ (e == EBUSY)
                    repeat = true;

                  ಮುಂದುವರಿ;
                }

              controllers_written++;
            }

          ಆದರೆ (! repeat)
            ನಿಲ್ಲಿಸು;

          /* If there was any controller written, just try once more without any delay.  */
          ಆದರೆ (controllers_written > 0 && attempts_ಎಡ > 2)
            {
              attempts_ಎಡ = 1;
              ಮುಂದುವರಿ;
            }

          ಆದರೆ (attempts_ಎಡ > 0)
            {
              ರಚನೆ timespec delay = {
                .tv_sec = 0,
                .tv_nsec = 1000000,
              };
              nanosleep (&delay, ಖಾಲಿ);
            }
        }

      /* Refresh what controllers are available.  */
      ಹಿಂತಿರುಗಿ read_ಲಭ್ಯ_controllers (ಮಾರ್ಗ, err);
    }

  /* All controllers were enabled successfully.  */
  ಹಿಂತಿರುಗಿ controllers_ಗಮ್ಯ_enable;
}

ಪೂರ್ಣಾಂಕ
enable_controllers (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *tmp_ಮಾರ್ಗ = ಖಾಲಿ;
  ಅಕ್ಷರ *it;
  ಪೂರ್ಣಾಂಕ ret, controllers_ಗಮ್ಯ_enable;

  xasprintf (&tmp_ಮಾರ್ಗ, "%s/", ಮಾರ್ಗ);

  ret = read_ಲಭ್ಯ_controllers ("", err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  controllers_ಗಮ್ಯ_enable = ret;

  /* Enable all possible controllers in the root cgroup.  */
  ret = write_controller_file ("", controllers_ಗಮ್ಯ_enable, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* Enabling +cpu when there are realtime processes fail with EINVAL.  */
      ಆದರೆ ((controllers_ಗಮ್ಯ_enable & CGROUP_CPU) && (crun_error_get_errno (err) == EINVAL))
        {
          crun_error_release (err);
          controllers_ಗಮ್ಯ_enable &= ~CGROUP_CPU;
          ret = write_controller_file ("", controllers_ಗಮ್ಯ_enable, err);
        }
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಗಾಗಿ (it = strchr (tmp_ಮಾರ್ಗ + 1, '/'); it;)
    {
      cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
      ಅಕ್ಷರ *ಮುಂದಿನ_slash = strchr (it + 1, '/');

      *it = '\0';

      ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, tmp_ಮಾರ್ಗ, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = mkdir (cgroup_ಮಾರ್ಗ, 0755);
      ಆದರೆ (UNLIKELY (ret < 0 && errno != EEXIST))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "create `%s`", cgroup_ಮಾರ್ಗ);

      ಆದರೆ (ಮುಂದಿನ_slash)
        {
          ret = write_controller_file (tmp_ಮಾರ್ಗ, controllers_ಗಮ್ಯ_enable, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      *it = '/';
      it = ಮುಂದಿನ_slash;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_move_process_ಗಮ್ಯ_cgroup (pid_t pid, pid_t init_pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool create_if_missing, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (ಮಾರ್ಗ == ಖಾಲಿ || *ಮಾರ್ಗ == '\0')
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ enter_cgroup (cgroup_ಬಹುಲಕ, pid, init_pid, ಮಾರ್ಗ, create_if_missing, err);
}

/* Move the current process to the cgroups listed in CONTENT, as read from
   /proc/self/cgroup earlier.  Used to undo libcrun_move_process_to_cgroup.  */
ಪೂರ್ಣಾಂಕ
libcrun_move_self_ಗಮ್ಯ_cgroups (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *content, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *buf = xstrdup (content);
  ಅಕ್ಷರ *saveptr = ಖಾಲಿ;
  ಅಕ್ಷರ *controller;
  ಅಕ್ಷರ *ಮಾರ್ಗ;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  bool has_ದತ್ತಾಂಶ;
  ಪೂರ್ಣಾಂಕ ret;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಗಾಗಿ (has_ದತ್ತಾಂಶ = read_proc_cgroup (buf, &saveptr, ಖಾಲಿ, &controller, &ಮಾರ್ಗ);
       has_ದತ್ತಾಂಶ;
       has_ದತ್ತಾಂಶ = read_proc_cgroup (ಖಾಲಿ, &saveptr, ಖಾಲಿ, &controller, &ಮಾರ್ಗ))
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subsystem = ಖಾಲಿ;

      ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
        {
          /* Ignore named v1 hierarchies, if any.  */
          ಆದರೆ (controller[0] != '\0')
            ಮುಂದುವರಿ;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          cleanup_free ಅಕ್ಷರ *subsystem_ಮಾರ್ಗ = ಖಾಲಿ;

          /* Use the same names as enter_cgroup_v1 does.  */
          ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (controller, "name="))
            controller += 5;
          subsystem = controller[0] == '\0' ? "unified" : controller;
          ಆದರೆ (strcmp (subsystem, "net_prio,net_cls") == 0)
            subsystem = "net_cls,net_prio";
          ಆದರೆ (strcmp (subsystem, "cpuacct,cpu") == 0)
            subsystem = "cpu,cpuacct";

          ret = append_paths (&subsystem_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, subsystem, ಖಾಲಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          ret = crun_ಮಾರ್ಗ_exists (subsystem_ಮಾರ್ಗ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          ಆದರೆ (ret == 0)
            ಮುಂದುವರಿ;
        }

      ret = move_process_ಗಮ್ಯ_cgroup (0, subsystem, ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_get_cgroup_dirfd (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sub_cgroup, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ_ಗಮ್ಯ_cgroup = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ cgroupdirfd;
  ಪೂರ್ಣಾಂಕ ret;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (cgroup_ಬಹುಲಕ < 0)
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (cgroup_ಬಹುಲಕ != CGROUP_ಬಹುಲಕ_UNIFIED)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cgroup dirfd supported only on cgroup v2");

  ಆದರೆ (ಸ್ಥಿತಿ_2 == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error");

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (ಸ್ಥಿತಿ_2->ಮಾರ್ಗ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "no cgroup path specified");

  ret = append_paths (&ಮಾರ್ಗ_ಗಮ್ಯ_cgroup, err, CGROUP_ಬೇರು, ಸ್ಥಿತಿ_2->ಮಾರ್ಗ, sub_cgroup, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  cgroupdirfd = open (ಮಾರ್ಗ_ಗಮ್ಯ_cgroup, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (cgroupdirfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ಮಾರ್ಗ_ಗಮ್ಯ_cgroup);

  ಹಿಂತಿರುಗಿ cgroupdirfd;
}

ಪೂರ್ಣಾಂಕ
libcrun_migrate_all_pids_ಗಮ್ಯ_cgroup (pid_t init_pid, ಅಕ್ಷರ *from, ಅಕ್ಷರ *ಗಮ್ಯ, libcrun_error_t *err)
{
  cleanup_free pid_t *pids = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ ಮಗು_dfd = -1;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  size_t from_len;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (cgroup_ಬಹುಲಕ < 0)
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  /* Freeze the source cgroup for a consistent view of its processes while they
     are migrated, but make sure it is always thawed again on error, otherwise
     it would be left frozen.  */
  ret = libcrun_cgroup_pause_unpause_ಮಾರ್ಗ (from, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_cgroup_read_pids_from_ಮಾರ್ಗ (from, true, &pids, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು thaw;

  from_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (from);

  ಗಾಗಿ (i = 0; pids && pids[i]; i++)
    {
      cleanup_free ಅಕ್ಷರ *pid_ಮಾರ್ಗ = ಖಾಲಿ;
      cleanup_free ಅಕ್ಷರ *dest_cgroup = ಖಾಲಿ;

      ret = libcrun_get_cgroup_process (pids[i], &pid_ಮಾರ್ಗ, false, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು thaw;

      /* Make sure the pid is in the cgroup we are migrating from.  */
      ಆದರೆ (! has_ಪೂರ್ವಪ್ರತ್ಯಯ (pid_ಮಾರ್ಗ, from))
        {
          ret = crun_make_error (err, 0, "error migrating pid %d.  It is not in the cgroup `%s`", pids[i], from);
          ಹೋಗು thaw;
        }

      /* Build the destination cgroup path, keeping the same hierarchy.  */
      xasprintf (&dest_cgroup, "%s%s", ಗಮ್ಯ, pid_ಮಾರ್ಗ + from_len);

      ret = enter_cgroup (cgroup_ಬಹುಲಕ, pids[i], init_pid, dest_cgroup, false, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು thaw;
    }

  ret = 0;

thaw:
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      libcrun_error_t thaw_err = ಖಾಲಿ;

      /* Best-effort thaw: keep the original error but never leave the cgroup frozen.  */
      ಆದರೆ (libcrun_cgroup_pause_unpause_ಮಾರ್ಗ (from, false, &thaw_err) < 0)
        crun_error_release (&thaw_err);
      ಹಿಂತಿರುಗಿ ret;
    }

  ret = libcrun_cgroup_pause_unpause_ಮಾರ್ಗ (from, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ destroy_cgroup_ಮಾರ್ಗ (from, cgroup_ಬಹುಲಕ, err);
}

ಪೂರ್ಣಾಂಕ
get_cgroup_dirfd_ಮಾರ್ಗ (ಪೂರ್ಣಾಂಕ dirfd, ಅಕ್ಷರ **ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
  proc_fd_ಮಾರ್ಗ_t fd_ಮಾರ್ಗ;
  ssize_t len;

  get_proc_self_fd_ಮಾರ್ಗ (fd_ಮಾರ್ಗ, dirfd);

  len = ಸುರಕ್ಷಿತ_readlinkat (AT_FDCWD, fd_ಮಾರ್ಗ, &cgroup_ಮಾರ್ಗ, 0, err);
  ಆದರೆ (UNLIKELY (len < 0))
    ಹಿಂತಿರುಗಿ len;

  ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (cgroup_ಮಾರ್ಗ, CGROUP_ಬೇರು))
    {
      *ಮಾರ್ಗ = xstrdup (cgroup_ಮಾರ್ಗ + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (CGROUP_ಬೇರು));
      ಹಿಂತಿರುಗಿ 0;
    }
  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid cgroup path `%s`", cgroup_ಮಾರ್ಗ);
}

/* Parse a cpu.max value: "QUOTA [PERIOD]", where QUOTA is either a
   number or "max" (returned as -1).  */
ಪೂರ್ಣಾಂಕ
parse_cpu_max (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ, int64_t *quota, uint64_t *period, bool *has_period, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *it = ಮೌಲ್ಯ;
  ಅಕ್ಷರ *endptr = ಖಾಲಿ;

  ಆಗ (*it == ' ')
    it++;

  errno = 0;
  ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (it, "max"))
    {
      *quota = -1;
      endptr = (ಅಕ್ಷರ *) it + 3;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      *quota = strtoll (it, &endptr, 10);
      ಆದರೆ (UNLIKELY (errno != 0 || endptr == it || *quota <= 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", ಮೌಲ್ಯ);
    }

  it = endptr;
  ಆಗ (*it == ' ')
    it++;

  *has_period = false;
  ಆದರೆ (*it && *it != '\n')
    {
      errno = 0;
      *period = strtoull (it, &endptr, 10);
      ಆದರೆ (UNLIKELY (errno != 0 || endptr == it || (*endptr && *endptr != '\n')))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid value for `cpu.max`: `%s`", ಮೌಲ್ಯ);
      *has_period = true;
    }

  ಹಿಂತಿರುಗಿ 0;
}
