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
#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "ring_buffer.h"
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/un.h>
#ಅನ್ತರ್ಭಾವ <sys/socket.h>
#ಅನ್ತರ್ಭಾವ <sys/signalfd.h>
#ಅನ್ತರ್ಭಾವ <sys/epoll.h>
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#ಅನ್ತರ್ಭಾವ <sys/wait.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <grp.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <sys/xattr.h>
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <linux/magic.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <sys/mman.h>
#ifdef HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#endif
#ಯದಿ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#ಅನ್ಯಥಾ
#  define atomic_long ಚಞ್ಚಲ long
#  define atomic_int ಚಞ್ಚಲ ಪೂರ್ಣಾಙ್ಕ
#endif
#ಅನ್ತರ್ಭಾವ "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_ಬೇರು
#  define RESOLVE_IN_ಬೇರು 0x10
#endif
#ifndef __NR_close_range
#  define __NR_close_range 436
#endif
#ifndef __NR_openat2
#  define __NR_openat2 437
#endif
#ifndef PROC_USER_INIT_INO
#  define PROC_USER_INIT_INO ((ino_t) 0xEFFFFFFD)
#endif

#define MAX_READLINKS 32
#define MAX_OPENAT2_EAGAIN_RETRIES 128

ಪೂರ್ಣಾಙ್ಕ
crun_path_exists (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret = access (path, F_OK);
  ಯದಿ (ret < 0)
    {
      ಯದಿ (errno == ENOENT)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "access `%s`", path);
    }
  ಪ್ರತ್ಯಾಗಮ 1;
}

ಪೂರ್ಣಾಙ್ಕ
xasprintf (ವರ್ಣ **str, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  ಯದಿ (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
write_file_at_with_flags (ಪೂರ್ಣಾಙ್ಕ dirfd, ಪೂರ್ಣಾಙ್ಕ flags, mode_t mode, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ಶೂನ್ಯ *data, size_t len, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ (len > INT_MAX) ? INT_MAX : (ಪೂರ್ಣಾಙ್ಕ) len;
}

ಪೂರ್ಣಾಙ್ಕ
detach_process ()
{
  pid_t pid;
  ಯದಿ (setsid () < 0)
    ಪ್ರತ್ಯಾಗಮ -1;
  pid = fork ();
  ಯದಿ (pid < 0)
    ಪ್ರತ್ಯಾಗಮ -1;
  ಯದಿ (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
get_file_type_fd (ಪೂರ್ಣಾಙ್ಕ fd, mode_t *mode)
{
  ಸಂರಚನಾ stat st;
  ಪೂರ್ಣಾಙ್ಕ ret;

#ifdef HAVE_STATX
  ಸಂರಚನಾ statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno == ENOSYS || errno == EINVAL)
        ಗಚ್ಛ fallback;

      ಪ್ರತ್ಯಾಗಮ ret;
    }
  *mode = stx.stx_mode;
  ಪ್ರತ್ಯಾಗಮ ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
get_file_type_at (ಪೂರ್ಣಾಙ್ಕ dirfd, mode_t *mode, bool nofollow, ಧ್ರುವ ವರ್ಣ *path)
{
  ಪೂರ್ಣಾಙ್ಕ empty_path = path == ರಿಕ್ತ ? AT_EMPTY_PATH : 0;
  ಸಂರಚನಾ stat st;
  ಪೂರ್ಣಾಙ್ಕ ret;

#ifdef HAVE_STATX
  ಸಂರಚನಾ statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno == ENOSYS || errno == EINVAL)
        ಗಚ್ಛ fallback;

      ಪ್ರತ್ಯಾಗಮ ret;
    }
  *mode = stx.stx_mode;
  ಪ್ರತ್ಯಾಗಮ ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
get_file_type (mode_t *mode, bool nofollow, ಧ್ರುವ ವರ್ಣ *path)
{
  ಪ್ರತ್ಯಾಗಮ get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

ಪೂರ್ಣಾಙ್ಕ
create_file_if_missing_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  ಯದಿ (fd_write < 0)
    {
      ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
      mode_t tmp_mode;
      ಪೂರ್ಣಾಙ್ಕ ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      ಯದಿ (ret == 0 && S_ISREG (tmp_mode))
        ಪ್ರತ್ಯಾಗಮ 0;

      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
ensure_directory_internal_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ವರ್ಣ *path, size_t len, ಪೂರ್ಣಾಙ್ಕ mode, libcrun_error_t *err)
{
  ವರ್ಣ *it = path + len;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;
  bool parent_created = false;

  ಪ್ರತಿ (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      ಯದಿ (ret == 0 || errno == EEXIST)
        ಪ್ರತ್ಯಾಗಮ 0;

      ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
      ಯದಿ (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = ರಿಕ್ತ;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          ಯದಿ (ret > 0)
            ಪ್ರತ್ಯಾಗಮ 0;
          ಯದಿ (ret < 0)
            crun_error_release (&tmp_err);

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      ಯಾವತ್ (it > path && *it != '/')
        {
          it--;
          len--;
        }
      ಯದಿ (it == path)
        ಪ್ರತ್ಯಾಗಮ 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      parent_created = true;
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
crun_ensure_directory_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode, bool nofollow, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free ವರ್ಣ *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (ret == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
check_fd_is_path (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  ವರ್ಣ link[PATH_MAX];
  ಪೂರ್ಣಾಙ್ಕ ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, ಪರಿಮಾಣ (link)));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "readlink `%s`", fdname);

  ಯದಿ (((size_t) ret) != path_len || memcmp (link, path, path_len))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
check_fd_under_path (ಧ್ರುವ ವರ್ಣ *rootfs, size_t rootfslen, ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  ವರ್ಣ link[PATH_MAX];
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  ಯದಿ (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    ಪ್ರತ್ಯಾಗಮ 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, ಪರಿಮಾಣ (link)));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "readlink `%s`", fdname);

  ಯದಿ (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
ಸ್ಥಿರ ಶೂನ್ಯ
close_and_replace (ಪೂರ್ಣಾಙ್ಕ *oldfd, ಪೂರ್ಣಾಙ್ಕ newfd)
{
  ಯದಿ (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
ವರ್ಣ *chroot_realpath (ಧ್ರುವ ವರ್ಣ *chroot, ಧ್ರುವ ವರ್ಣ *path, ವರ್ಣ resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
safe_openat_fallback (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *rootfs, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ flags,
                      ಪೂರ್ಣಾಙ್ಕ mode, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *parent_path = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *last_component = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *orig_path = path;
  ಧ್ರುವ ವರ್ಣ *path_in_chroot;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ವರ್ಣ resolved[PATH_MAX];
  ವರ್ಣ buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  ಯದಿ (flags & O_NOFOLLOW)
    {
      ವರ್ಣ *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      ಯದಿ (sep == ರಿಕ್ತ)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      ಅನ್ಯಥಾ ಯದಿ (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      ಯದಿ (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  ಯದಿ (path_in_chroot == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  ಯದಿ (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  ಯದಿ (last_component)
    {
      ret = snprintf (resolved, ಪರಿಮಾಣ (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      ಯದಿ (UNLIKELY (ret >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (resolved)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  ಯದಿ (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = fd;
  fd = -1;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
safe_openat (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *rootfs, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ mode,
             libcrun_error_t *err)
{
  ಸ್ಥಿರ bool openat2_supported = true;
  ಪೂರ್ಣಾಙ್ಕ retries = MAX_OPENAT2_EAGAIN_RETRIES;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (is_empty_string (path))
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      ಯದಿ (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      ಯದಿ (UNLIKELY (fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      ಯದಿ (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ret = fd;
      fd = -1;
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_ಬೇರು);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (errno == EINTR)
            ಗಚ್ಛ repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          ಯದಿ (errno == EAGAIN && retries-- > 0)
            ಗಚ್ಛ repeat;
          ಯದಿ (errno == ENOSYS)
            openat2_supported = false;
          ಯದಿ (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            ಪ್ರತ್ಯಾಗಮ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "openat2 `%s`", path);
        }

      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (ಪೂರ್ಣಾಙ್ಕ dfd, ಧ್ರುವ ವರ್ಣ *name, ವರ್ಣ **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free ವರ್ಣ *tmp_buf = ರಿಕ್ತ;
  ssize_t size;

  ಕುರು
    {
      ಯದಿ (tmp_buf != ರಿಕ್ತ)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      ಯದಿ (UNLIKELY (size < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "readlink `%s`", name);
  } ಯಾವತ್ (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ size;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *dirpath,
                                ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode, ಪೂರ್ಣಾಙ್ಕ max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *dirpath, ಪೂರ್ಣಾಙ್ಕ cwd,
                                 ಧ್ರುವ ವರ್ಣ *npath, ಧ್ರುವ ವರ್ಣ *cur, ಧ್ರುವ ವರ್ಣ *rest,
                                 ಪೂರ್ಣಾಙ್ಕ mode, ಪೂರ್ಣಾಙ್ಕ max_readlinks, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *target = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *new_path = ರಿಕ್ತ;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (ಪೂರ್ಣಾಙ್ಕ) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  ಪ್ರತ್ಯಾಗಮ crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *dirpath,
                     ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode, ಪೂರ್ಣಾಙ್ಕ max_readlinks, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ wd_cleanup = -1;
  cleanup_free ವರ್ಣ *npath = ರಿಕ್ತ;
  bool last_component = false;
  size_t depth = 0;
  ವರ್ಣ *cur;
  ವರ್ಣ *it;
  ಪೂರ್ಣಾಙ್ಕ cwd;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (max_readlinks <= 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  ಯದಿ (*path == '\0')
    {
      ಯದಿ (do_open)
        ಪ್ರತ್ಯಾಗಮ open (dirpath, O_CLOEXEC | O_PATH, 0);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  ಯಾವತ್ (cur)
    {
      ಯದಿ (it)
        *it = '\0';
      ಅನ್ಯಥಾ
        last_component = true;

      ಯದಿ (cur[0] == '\0')
        ಭಙ್ಗ;

      ಯದಿ (strcmp (cur, ".") == 0)
        ಗಚ್ಛ next;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (cur, ".."))
        depth++;
      ಅನ್ಯಥಾ
        {
          ಯದಿ (depth)
            depth--;
          ಅನ್ಯಥಾ
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              ಗಚ್ಛ next;
            }
        }

      ಯದಿ (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              ಯದಿ (errno == ELOOP)
                ಪ್ರತ್ಯಾಗಮ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, ರಿಕ್ತ,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              ಯದಿ (ret < 0)
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          ಯದಿ (do_open)
            ಪ್ರತ್ಯಾಗಮ ret;

          close_and_replace (&wd_cleanup, ret);
          ಪ್ರತ್ಯಾಗಮ 0;
        }

      ret = mkdirat (cwd, cur, mode);
      ಯದಿ (ret < 0)
        {
          mode_t st_mode;

          ಯದಿ (errno != EEXIST)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          ಯದಿ (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            ಪ್ರತ್ಯಾಗಮ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : ರಿಕ್ತ, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      ಯದಿ (UNLIKELY (cwd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "creating `/%s`", path);

      ಯದಿ (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, ರಿಕ್ತ);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;

              close (cwd);
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          ಯದಿ ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      ಯದಿ (it == ರಿಕ್ತ)
        ಭಙ್ಗ;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  ಯದಿ (do_open)
    {
      ಯದಿ (cwd == dirfd)
        {
          ret = dup (dirfd);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "dup `%s`", dirpath);
          ಪ್ರತ್ಯಾಗಮ ret;
        }

      wd_cleanup = -1;
      ಪ್ರತ್ಯಾಗಮ cwd;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
crun_safe_create_and_open_ref_at (bool dir, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *dirpath, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  ಯದಿ (LIKELY (ret >= 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  crun_error_release (err);
  ಪ್ರತ್ಯಾಗಮ crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ಪೂರ್ಣಾಙ್ಕ
crun_safe_ensure_directory_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *dirpath, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode,
                               libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ಪೂರ್ಣಾಙ್ಕ
crun_safe_ensure_file_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *dirpath, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode,
                          libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ಪೂರ್ಣಾಙ್ಕ
crun_ensure_directory (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode, bool nofollow, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_file_size (ಪೂರ್ಣಾಙ್ಕ fd, off_t *size)
{
  ಸಂರಚನಾ stat st;
  ಪೂರ್ಣಾಙ್ಕ ret;
#ifdef HAVE_STATX
  ಸಂರಚನಾ statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno == ENOSYS || errno == EINVAL)
        ಗಚ್ಛ fallback;
      ಪ್ರತ್ಯಾಗಮ ret;
    }
  *size = stx.stx_size;

  ಪ್ರತ್ಯಾಗಮ ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
crun_dir_p_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "stat `%s`", path);

  ಪ್ರತ್ಯಾಗಮ S_ISDIR (mode) ? 1 : 0;
}

ಪೂರ್ಣಾಙ್ಕ
crun_dir_p (ಧ್ರುವ ವರ್ಣ *path, bool nofollow, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  size_t len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      ಯದಿ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಪ್ರತ್ಯಾಗಮ 0;
        }
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
ಸ್ಥಿರ bool
running_in_openvz ()
{
  ಯದಿ (access ("/proc/vz", F_OK) < 0)
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತ್ಯಾಗಮ access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

ಪೂರ್ಣಾಙ್ಕ
check_running_in_user_namespace (libcrun_error_t *err)
{
  ಸ್ಥಿರ atomic_int run_in_userns = -1;
  ಸಂರಚನಾ stat st;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = run_in_userns;
  ಯದಿ (ret >= 0)
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = stat ("/proc/self/ns/user", &st);
  ಯದಿ (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      ಯದಿ (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          ಪ್ರತ್ಯಾಗಮ run_in_userns;
        }

      ಯದಿ (! running_in_openvz ())
        {
          run_in_userns = 1;
          ಪ್ರತ್ಯಾಗಮ run_in_userns;
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (UNLIKELY (errno != ENOENT))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  run_in_userns = ret;
  ಪ್ರತ್ಯಾಗಮ run_in_userns;
}

ಸ್ಥಿರ size_t
get_page_size ()
{
  ಸ್ಥಿರ atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  ಯದಿ (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  ಪ್ರತ್ಯಾಗಮ (size_t) pagesize;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ selinux_enabled = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ apparmor_enabled = -1;

ಪೂರ್ಣಾಙ್ಕ
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *out = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  size_t len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (selinux_enabled >= 0)
    ಪ್ರತ್ಯಾಗಮ selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  ಪ್ರತ್ಯಾಗಮ selinux_enabled;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಪೂರ್ಣಾಙ್ಕ size;
  ವರ್ಣ buf[1];

  ಯದಿ (apparmor_enabled >= 0)
    ಪ್ರತ್ಯಾಗಮ apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  ಯದಿ (fd == -1)
    {
      ಯದಿ (errno == ENOENT)
        {
          apparmor_enabled = 0;
          ಪ್ರತ್ಯಾಗಮ 0;
        }

      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (ಪಠ (fd, buf, ಪರಿಮಾಣ (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  ಪ್ರತ್ಯಾಗಮ apparmor_enabled;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  ಯದಿ (selinux_enabled < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "SELinux is not initialized correctly");
  ಪ್ರತ್ಯಾಗಮ selinux_enabled;
}

ಪೂರ್ಣಾಙ್ಕ
add_selinux_mount_label (ವರ್ಣ **retlabel, ಧ್ರುವ ವರ್ಣ *data, ಧ್ರುವ ವರ್ಣ *label, ಧ್ರುವ ವರ್ಣ *context_type, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_is_selinux_enabled (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (label && ret)
    {
      ಯದಿ (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      ಅನ್ಯಥಾ
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      ಪ್ರತ್ಯಾಗಮ 0;
    }
  *retlabel = xstrdup (data);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
lsm_attr_path (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *lsm, ಧ್ರುವ ವರ್ಣ *fname, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ attr_dirfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ lsm_dirfd = -1;
  ವರ್ಣ *attr_path = ರಿಕ್ತ;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  ಯದಿ (UNLIKELY (attr_dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  ಯದಿ (lsm != ರಿಕ್ತ)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      ಯದಿ (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  ಪ್ರತ್ಯಾಗಮ attr_path;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
check_proc_super_magic (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  ಸಂರಚನಾ statfs sfs;

  ಪೂರ್ಣಾಙ್ಕ ret = fstatfs (fd, &sfs);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "statfs `%s`", path);

  ಯದಿ (sfs.f_type != PROC_SUPER_MAGIC)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
set_security_attr (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *lsm, ಧ್ರುವ ವರ್ಣ *fname, ಧ್ರುವ ವರ್ಣ *data, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  cleanup_free ಧ್ರುವ ವರ್ಣ *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

  ಯದಿ (UNLIKELY (attr_path == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (ಲಿಖ (fd, data, strlen (data)));
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      ಯದಿ (errno == ENOENT && lsm != ರಿಕ್ತ && strcmp (lsm, "apparmor") == 0)
        {
          ಧ್ರುವ ವರ್ಣ *profile = strchr (data, ' ');

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
set_selinux_label (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *label, bool now, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_is_selinux_enabled (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (ret)
    ಪ್ರತ್ಯಾಗಮ set_security_attr (container, ರಿಕ್ತ, now ? "current" : "exec", label, err);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  ಯದಿ (apparmor_enabled < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "AppArmor is not initialized correctly");
  ಪ್ರತ್ಯಾಗಮ apparmor_enabled;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (ಪರಿಮಾಣ (UNCONFINED) - 1)
  cleanup_free ಧ್ರುವ ವರ್ಣ *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  ವರ್ಣ buf[UNCONFINED_LEN + 1];

  ಯದಿ (UNLIKELY (attr_path == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ fd;

  ಯದಿ (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    ಪ್ರತ್ಯಾಗಮ -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (ಪಠ (fd, buf, ಪರಿಮಾಣ (buf)));
  ಯದಿ (UNLIKELY (bytes_read < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  ಯದಿ (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    ಪ್ರತ್ಯಾಗಮ 1;
  ಯದಿ (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    ಪ್ರತ್ಯಾಗಮ 0;
  ಪ್ರತ್ಯಾಗಮ 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

ಪೂರ್ಣಾಙ್ಕ
set_apparmor_profile (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_is_apparmor_enabled (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (ret)
    {
      cleanup_free ವರ್ಣ *buf = ರಿಕ್ತ;
      ret = is_current_process_confined (container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      ಪ್ರತ್ಯಾಗಮ set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_all_fd_internal (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *description, ವರ್ಣ **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *buf = ರಿಕ್ತ;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (size_hint)
    allocated = size_hint;
  ಅನ್ಯಥಾ
    {
      ret = get_file_size (fd, &size);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "stat `%s`", description);

      ಯದಿ (max_size && (size_t) size > max_size)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  ಯಾವತ್ ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (ಪಠ (fd, buf + nread, allocated - nread));
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from file `%s`", description);

      ಯದಿ (ret == 0)
        ಭಙ್ಗ;

      nread += ret;

      ಯದಿ (max_size && nread > max_size)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      ಯದಿ (nread == allocated)
        {
          ಯದಿ (size)
            ಭಙ್ಗ;

          ಯದಿ (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  ಯದಿ (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      ವರ್ಣ *tmp = realloc (buf, nread + 1);
      ಯದಿ (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = ರಿಕ್ತ;
  ಯದಿ (len)
    *len = nread;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
read_all_fd_with_size_hint (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *description, ವರ್ಣ **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

ಪೂರ್ಣಾಙ್ಕ
read_all_fd_with_max_size (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *description, ವರ್ಣ **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

ಪೂರ್ಣಾಙ್ಕ
read_all_file_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *path, ವರ್ಣ **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", path);

  ಪ್ರತ್ಯಾಗಮ read_all_fd (fd, path, out, len, err);
}

ಪೂರ್ಣಾಙ್ಕ
read_all_file (ಧ್ರುವ ವರ್ಣ *path, ವರ್ಣ **out, size_t *len, libcrun_error_t *err)
{
  ಯದಿ (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  ಪ್ರತ್ಯಾಗಮ read_all_file_at (AT_FDCWD, path, out, len, err);
}

ಪೂರ್ಣಾಙ್ಕ
get_realpath_to_file (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *path_name, ವರ್ಣ **absolute_path, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  ಯದಿ (UNLIKELY (targetfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", path_name);
  ಅನ್ಯಥಾ
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      ಯದಿ (UNLIKELY (len < 0))
        ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
open_unix_domain_client_socket (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ dgram, libcrun_error_t *err)
{
  ಸಂರಚನಾ sockaddr_un addr = {};
  ಪೂರ್ಣಾಙ್ಕ ret;
  proc_fd_path_t name_buf;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ destfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "create UNIX socket");

  ಯದಿ (strlen (path) >= ಪರಿಮಾಣ (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      ಯದಿ (UNLIKELY (destfd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  ಯದಿ (path_len >= ಪರಿಮಾಣ (addr.sun_path))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (ಸಂರಚನಾ sockaddr *) &addr, ಪರಿಮಾಣ (addr));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
open_unix_domain_socket (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ dgram, libcrun_error_t *err)
{
  ಸಂರಚನಾ sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "create UNIX socket");

  ಯದಿ (strlen (path) >= ಪರಿಮಾಣ (addr.sun_path))
    {
      cleanup_free ವರ್ಣ *dpath = xstrdup (path);
      cleanup_free ವರ್ಣ *bpath = xstrdup (path);
      ಧ್ರುವ ವರ್ಣ *parent_dir = dirname (dpath);
      ಧ್ರುವ ವರ್ಣ *base = basename (bpath);
      ಪೂರ್ಣಾಙ್ಕ n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      ಯದಿ (UNLIKELY (dirfd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, ಪರಿಮಾಣ (addr.sun_path), "%s/%s", name_buf, base);
      ಯದಿ (n < 0 || (size_t) n >= ಪರಿಮಾಣ (addr.sun_path))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  ಅನ್ಯಥಾ
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (ಸಂರಚನಾ sockaddr *) &addr, ಪರಿಮಾಣ (addr));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bind socket to `%s`", path);

  ಯದಿ (! dgram)
    {
      ret = listen (fd, 1);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
send_fd_to_socket (ಪೂರ್ಣಾಙ್ಕ server, ಪೂರ್ಣಾಙ್ಕ fd, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ send_fd_to_socket_with_payload (server, fd, ರಿಕ್ತ, 0, err);
}

ಪೂರ್ಣಾಙ್ಕ
send_fd_to_socket_with_payload (ಪೂರ್ಣಾಙ್ಕ server, ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *payload, size_t payload_len, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಸಂರಚನಾ cmsghdr *cmsg = ರಿಕ್ತ;
  ಸಂರಚನಾ iovec iov[2];
  ಸಂರಚನಾ msghdr ಸಂದೇಶ = {};
  ವರ್ಣ ctrl_buf[CMSG_SPACE (1 + ಪರಿಮಾಣ (ಪೂರ್ಣಾಙ್ಕ))] = {};
  ವರ್ಣ data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = ಪರಿಮಾಣ (data);

  ಯದಿ (payload_len > 0)
    {
      iov[0].iov_base = (ಶೂನ್ಯ *) payload;
      iov[0].iov_len = payload_len;
    }

  ಸಂದೇಶ.ಸಂದೇಶ_name = ರಿಕ್ತ;
  ಸಂದೇಶ.ಸಂದೇಶ_namelen = 0;
  ಸಂದೇಶ.ಸಂದೇಶ_iov = iov;
  ಸಂದೇಶ.ಸಂದೇಶ_iovlen = 1;
  ಸಂದೇಶ.ಸಂದೇಶ_controllen = CMSG_SPACE (ಪರಿಮಾಣ (ಪೂರ್ಣಾಙ್ಕ));
  ಸಂದೇಶ.ಸಂದೇಶ_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&ಸಂದೇಶ);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (ಪರಿಮಾಣ (ಪೂರ್ಣಾಙ್ಕ));

  *((ಪೂರ್ಣಾಙ್ಕ *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &ಸಂದೇಶ, 0));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "sendmsg");
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
receive_fd_from_socket_with_payload (ಪೂರ್ಣಾಙ್ಕ from, ವರ್ಣ *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಸಂರಚನಾ iovec iov[1];
  ಸಂರಚನಾ msghdr ಸಂದೇಶ = {};
  ವರ್ಣ ctrl_buf[CMSG_SPACE (ಪರಿಮಾಣ (ಪೂರ್ಣಾಙ್ಕ))] = {};
  ವರ್ಣ data[1];
  ಸಂರಚನಾ cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = ಪರಿಮಾಣ (data);

  ಯದಿ (payload_len > 0)
    {
      iov[0].iov_base = (ಶೂನ್ಯ *) payload;
      iov[0].iov_len = payload_len;
    }

  ಸಂದೇಶ.ಸಂದೇಶ_name = ರಿಕ್ತ;
  ಸಂದೇಶ.ಸಂದೇಶ_namelen = 0;
  ಸಂದೇಶ.ಸಂದೇಶ_iov = iov;
  ಸಂದೇಶ.ಸಂದೇಶ_iovlen = 1;
  ಸಂದೇಶ.ಸಂದೇಶ_controllen = CMSG_SPACE (ಪರಿಮಾಣ (ಪೂರ್ಣಾಙ್ಕ));
  ಸಂದೇಶ.ಸಂದೇಶ_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &ಸಂದೇಶ, 0));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "recvmsg");
  ಯದಿ (UNLIKELY (ret == 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&ಸಂದೇಶ);
  ಯದಿ (cmsg == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), ಪರಿಮಾಣ (fd));

  ret = fd;
  fd = -1;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
receive_fd_from_socket (ಪೂರ್ಣಾಙ್ಕ from, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ receive_fd_from_socket_with_payload (from, ರಿಕ್ತ, 0, err);
}

ಪೂರ್ಣಾಙ್ಕ
create_socket_pair (ಪೂರ್ಣಾಙ್ಕ *pair, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "socketpair");
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret = signalfd (-1, mask, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "signalfd");
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
epoll_helper_toggle (ಪೂರ್ಣಾಙ್ಕ epollfd, ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ events, libcrun_error_t *err)
{
  ಸಂರಚನಾ epoll_event ev = {};
  bool add = events != 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  ಯದಿ (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      ಯದಿ (errno == ENOENT || errno == EPERM)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
epoll_helper (ಪೂರ್ಣಾಙ್ಕ *in_fds, ಪೂರ್ಣಾಙ್ಕ *in_levelfds, ಪೂರ್ಣಾಙ್ಕ *out_fds, ಪೂರ್ಣಾಙ್ಕ *out_levelfds, libcrun_error_t *err)
{
  ಸಂರಚನಾ epoll_event ev;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ epollfd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಪೂರ್ಣಾಙ್ಕ *it;
  epollfd = epoll_create1 (0);
  ಯದಿ (UNLIKELY (epollfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  ಪ್ರತಿ (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      ಯದಿ (UNLIKELY (ret < 0))                                           \
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  ಯದಿ (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  ಯದಿ (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  ಯದಿ (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  ಯದಿ (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
copy_from_fd_to_fd (ಪೂರ್ಣಾಙ್ಕ src, ಪೂರ್ಣಾಙ್ಕ dst, ಪೂರ್ಣಾಙ್ಕ consume, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ssize_t nread;
  size_t pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  ಕುರು
    {
      cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      ಯದಿ (can_copy_file_range)
        {
          nread = copy_file_range (src, ರಿಕ್ತ, dst, ರಿಕ್ತ, pagesize, 0);
          ಯದಿ (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              ಗಚ್ಛ fallback;
            }
          ಯದಿ (consume && nread < 0 && errno == EAGAIN)
            ಪ್ರತ್ಯಾಗಮ 0;
          ಯದಿ (nread < 0 && errno == EIO)
            ಪ್ರತ್ಯಾಗಮ 0;
          ಯದಿ (UNLIKELY (nread < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "copy_file_range");
          ಅನುವರ್ತ;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (ಪಠ (src, buffer, pagesize));
      ಯದಿ (consume && nread < 0 && errno == EAGAIN)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಯದಿ (nread < 0 && errno == EIO)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಯದಿ (UNLIKELY (nread < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read");

      remaining = nread;
      ಯಾವತ್ (remaining)
        {
          ret = TEMP_FAILURE_RETRY (ಲಿಖ (dst, buffer + nread - remaining, remaining));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } ಯಾವತ್ (consume && nread);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
run_process (ವರ್ಣ **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  ಯದಿ (UNLIKELY (pid < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fork");
  ಯದಿ (pid)
    {
      ಪೂರ್ಣಾಙ್ಕ r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      ಯದಿ (r < 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "waitpid");
      ಯದಿ (WIFEXITED (status) || WIFSIGNALED (status))
        ಪ್ರತ್ಯಾಗಮ WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
ಸ್ಥಿರ unsigned
atou (ವರ್ಣ **s)
{
  unsigned x;
  ಪ್ರತಿ (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  ಪ್ರತ್ಯಾಗಮ x;
}

ಪೂರ್ಣಾಙ್ಕ
fgetpwent_r (FILE *f, ಸಂರಚನಾ passwd *pw, ವರ್ಣ *ಸಾಲು, size_t size, ಸಂರಚನಾ passwd **res)
{
  ವರ್ಣ *s;
  ಪೂರ್ಣಾಙ್ಕ rv = 0;
  ಪ್ರತಿ (;;)
    {
      ಸಾಲು[size - 1] = '\xff';
      ಯದಿ ((fgets (ಸಾಲು, size, f) == ರಿಕ್ತ) || ferror (f) || ಸಾಲು[size - 1] != '\xff')
        {
          rv = (ಸಾಲು[size - 1] != '\xff') ? ERANGE : ENOENT;
          ಸಾಲು = 0;
          pw = 0;
          ಭಙ್ಗ;
        }
      ಸಾಲು[strcspn (ಸಾಲು, "\n")] = 0;

      s = ಸಾಲು;
      pw->pw_name = s++;
      ಯದಿ (! (s = strchr (s, ':')))
        ಅನುವರ್ತ;

      *s++ = 0;
      pw->pw_passwd = s;
      ಯದಿ (! (s = strchr (s, ':')))
        ಅನುವರ್ತ;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      ಯದಿ (*s != ':')
        ಅನುವರ್ತ;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      ಯದಿ (*s != ':')
        ಅನುವರ್ತ;

      *s++ = 0;
      pw->pw_gecos = s;
      ಯದಿ (! (s = strchr (s, ':')))
        ಅನುವರ್ತ;

      *s++ = 0;
      pw->pw_dir = s;
      ಯದಿ (! (s = strchr (s, ':')))
        ಅನುವರ್ತ;

      *s++ = 0;
      pw->pw_shell = s;
      ಭಙ್ಗ;
    }
  *res = pw;
  ಯದಿ (rv)
    errno = rv;
  ಪ್ರತ್ಯಾಗಮ rv;
}
#endif

ಪೂರ್ಣಾಙ್ಕ
set_home_env (uid_t id)
{
  ಸಂರಚನಾ passwd pwd;
  cleanup_free ವರ್ಣ *buf = ರಿಕ್ತ;
  long buf_size;
  cleanup_file FILE *stream = ರಿಕ್ತ;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  ಯದಿ (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  ಯದಿ (stream == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ -1;

  ಪ್ರತಿ (;;)
    {
      ಸಂರಚನಾ passwd *ret_pw = ರಿಕ್ತ;

      ಪೂರ್ಣಾಙ್ಕ ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      ಯದಿ (UNLIKELY (ret != 0))
        {
          ಯದಿ (ret == EINTR)
            ಅನುವರ್ತ;

          ಯದಿ (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              ಪ್ರತ್ಯಾಗಮ -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          ಅನುವರ್ತ;
        }

      ಯದಿ (ret_pw && ret_pw->pw_uid == id)
        {
          ಯದಿ (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
getsubidrange (uid_t id, ಪೂರ್ಣಾಙ್ಕ is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *ಒಳಹರಿವು = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *lineptr = ರಿಕ್ತ;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free ವರ್ಣ *buf = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *name;
  ಸಂರಚನಾ passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  ಯದಿ (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  ಪ್ರತಿ (;;)
    {
      ಪೂರ್ಣಾಙ್ಕ ret;
      ಸಂರಚನಾ passwd *ret_pw = ರಿಕ್ತ;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      ಯದಿ (LIKELY (ret == 0))
        {
          ಯದಿ (ret_pw)
            {
              name = ret_pw->pw_name;
              ಭಙ್ಗ;
            }
          ಪ್ರತ್ಯಾಗಮ -1;
        }

      ಯದಿ (ret != ERANGE)
        ಪ್ರತ್ಯಾಗಮ -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  ಒಳಹರಿವು = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  ಯದಿ (ಒಳಹರಿವು == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ -1;

  ಪ್ರತಿ (;;)
    {
      ವರ್ಣ *endptr;
      ssize_t ಪಠ = getline (&lineptr, &lenlineptr, ಒಳಹರಿವು);
      ಯದಿ (ಪಠ < 0)
        ಪ್ರತ್ಯಾಗಮ -1;

      ಯದಿ (ಪಠ < (ssize_t) (len_name + 2))
        ಅನುವರ್ತ;

      ಯದಿ (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        ಅನುವರ್ತ;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      ಯದಿ (endptr >= &lineptr[ಪಠ])
        ಪ್ರತ್ಯಾಗಮ -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      ಪ್ರತ್ಯಾಗಮ 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

ಪೂರ್ಣಾಙ್ಕ
format_default_id_mapping (ವರ್ಣ **out, uid_t container_id, uid_t host_uid, uid_t host_id, ಪೂರ್ಣಾಙ್ಕ is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ written = 0;
  ಪೂರ್ಣಾಙ್ಕ ret, remaining;

  *out = ರಿಕ್ತ;

  ಯದಿ (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  ಯದಿ (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      ಯದಿ (UNLIKELY (ret >= remaining))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  ಯದಿ (UNLIKELY (ret >= remaining))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  ಯದಿ (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      ಯದಿ (UNLIKELY (ret >= remaining))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ written;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
unset_cloexec_flag (ಪೂರ್ಣಾಙ್ಕ fd)
{
  ಪೂರ್ಣಾಙ್ಕ flags = fcntl (fd, F_GETFD);
  ಯದಿ (flags == -1)
    ಪ್ರತ್ಯಾಗಮ -1;

  flags &= ~FD_CLOEXEC;

  ಪ್ರತ್ಯಾಗಮ fcntl (fd, F_SETFD, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ __attribute__ ((__noreturn__))
run_process_child (ವರ್ಣ *path, ವರ್ಣ **args, ಧ್ರುವ ವರ್ಣ *cwd, ವರ್ಣ **envp, ಪೂರ್ಣಾಙ್ಕ pipe_r,
                   ಪೂರ್ಣಾಙ್ಕ pipe_w, ಪೂರ್ಣಾಙ್ಕ out_fd, ಪೂರ್ಣಾಙ್ಕ err_fd, bool can_ignore_chdir_errors)
{
  ವರ್ಣ *tmp_args[] = { path, ರಿಕ್ತ };
  libcrun_error_t err = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ dev_null_fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = mark_or_close_fds_ge_than (ರಿಕ್ತ, 3, false, &err);
  ಯದಿ (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->ಸಂದೇಶ);

  ಯದಿ (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      ಯದಿ (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  ಯದಿ (out_fd >= 0)
    unset_cloexec_flag (1);
  ಯದಿ (err_fd >= 0)
    unset_cloexec_flag (2);

  ಯದಿ (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  ಯದಿ (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  ಯದಿ (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  ಯದಿ (args == ರಿಕ್ತ)
    args = tmp_args;

  ಯದಿ (cwd && chdir (cwd) < 0)
    {
      ಯದಿ (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
ಪೂರ್ಣಾಙ್ಕ
run_process_with_stdin_timeout_envp (ವರ್ಣ *path, ವರ್ಣ **args, ಧ್ರುವ ವರ್ಣ *cwd, ಪೂರ್ಣಾಙ್ಕ timeout,
                                     ವರ್ಣ **envp, ವರ್ಣ *stdin, size_t stdin_len, ಪೂರ್ಣಾಙ್ಕ out_fd,
                                     ಪೂರ್ಣಾಙ್ಕ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ stdin_pipe[2];
  pid_t pid;
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pipe_r = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pipe_w = -1;
  sigset_t oldmask, mask;
  ಪೂರ್ಣಾಙ್ಕ r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  ಯದಿ (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  ಯದಿ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      ಗಚ್ಛ restore_sig_mask_and_exit;
    }

  ಯದಿ (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (ಲಿಖ (pipe_w, stdin, stdin_len));
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      ಯದಿ (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          ಗಚ್ಛ restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  ಯದಿ (timeout)
    {
      time_t start = time (ರಿಕ್ತ);
      time_t now;
      ಪ್ರತಿ (now = start; now - start < timeout; now = time (ರಿಕ್ತ))
        {
          siginfo_t info;
          ಪೂರ್ಣಾಙ್ಕ elapsed = now - start;
          ಸಂರಚನಾ timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          ಯದಿ (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              ಗಚ್ಛ restore_sig_mask_and_exit;
            }

          ಯದಿ (info.si_signo == SIGCHLD && info.si_pid == pid)
            ಗಚ್ಛ read_waitpid;

          ಯದಿ (ret < 0 && errno == EAGAIN)
            ಗಚ್ಛ timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      ಗಚ್ಛ restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  ಯದಿ (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  ಅನ್ಯಥಾ
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  ಯದಿ (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      ಯದಿ (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
mark_or_close_fds_ge_than (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ n, bool close_now, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ cfd = -1;
  cleanup_dir DIR *dir = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ fd;
  ಸಂರಚನಾ dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  ಯದಿ (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  ಯದಿ (ret == 0)
    {
      ಯದಿ (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ಯದಿ (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  ಯದಿ (UNLIKELY (cfd < 0))
    ಪ್ರತ್ಯಾಗಮ cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  dir = fdopendir (cfd);
  ಯದಿ (UNLIKELY (dir == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  ಪ್ರತಿ (next = readdir (dir); next; next = readdir (dir))
    {
      ಪೂರ್ಣಾಙ್ಕ val;
      ಧ್ರುವ ವರ್ಣ *name = next->d_name;
      ಯದಿ (name[0] == '.')
        ಅನುವರ್ತ;

      val = strtoll (name, ರಿಕ್ತ, 10);
      ಯದಿ (val < n || val == fd)
        ಅನುವರ್ತ;

      ಯದಿ (close_now)
        {
          ret = close (val);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "close fd `%d`", val);

          ಯದಿ (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      ಅನ್ಯಥಾ
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ
get_current_timestamp (ವರ್ಣ *out, size_t len)
{
  ಸಂರಚನಾ timeval tv;
  ಸಂರಚನಾ tm now;
  ವರ್ಣ timestamp[64];

  gettimeofday (&tv, ರಿಕ್ತ);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, ಪರಿಮಾಣ (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (ಶೂನ್ಯ) snprintf (out, len, "%s.%06lldZ", timestamp, (long long ಪೂರ್ಣಾಙ್ಕ) tv.tv_usec);
  out[len - 1] = '\0';
}

ಪೂರ್ಣಾಙ್ಕ
set_blocking_fd (ಪೂರ್ಣಾಙ್ಕ fd, bool blocking, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret, flags = fcntl (fd, F_GETFL, 0);
  ಯದಿ (UNLIKELY (flags < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fcntl");
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
parse_json_file (json_object **out, ಧ್ರುವ ವರ್ಣ *jsondata, ಸಂರಚನಾ parser_context *ctx arg_unused, libcrun_error_t *err)
{
  ಸಂರಚನಾ json_tokener *tok;
  ಗಣನಾ json_tokener_error jerr;

  *err = ರಿಕ್ತ;

  ಯದಿ (UNLIKELY (jsondata == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  ಯದಿ (UNLIKELY (tok == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (ಪೂರ್ಣಾಙ್ಕ) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  ಯದಿ (*out == ರಿಕ್ತ || jerr != json_tokener_success)
    {
      ಪೂರ್ಣಾಙ್ಕ saved_pos = (ಪೂರ್ಣಾಙ್ಕ) tok->char_offset;

      json_tokener_free (tok);
      ಯದಿ (*out)
        {
          json_object_put (*out);
          *out = ರಿಕ್ತ;
        }
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  ಪ್ರತ್ಯಾಗಮ 0;
}

#define CHECK_ACCESS_NOT_EXECUTABLE 1
#define CHECK_ACCESS_NOT_REGULAR 2

/* check that the specified path exists and it is executable.
   Return
   - 0 if the file is executable
   - CHECK_ACCESS_NOT_EXECUTABLE if it exists but it is not executable
   - CHECK_ACCESS_NOT_REGULAR if it not a regular file
   - -errno for any other generic error
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
check_access (ಧ್ರುವ ವರ್ಣ *path)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#ಅನ್ಯಥಾ
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  ಯದಿ (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      ಯದಿ (errno == EACCES)
        {
          ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          ಯದಿ (ret == 0)
            ಪ್ರತ್ಯಾಗಮ CHECK_ACCESS_NOT_EXECUTABLE;
        }
      ಪ್ರತ್ಯಾಗಮ -errno;
    }

  ret = get_file_type (&mode, false, path);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ -errno;

  ಯದಿ (! S_ISREG (mode))
    ಪ್ರತ್ಯಾಗಮ CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
find_executable (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *executable_path, ಧ್ರುವ ವರ್ಣ *cwd, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cwd_executable_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *tmp = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ last_error = -ENOENT;
  ವರ್ಣ *it, *end;
  ಪೂರ್ಣಾಙ್ಕ ret;

  *out = ರಿಕ್ತ;

  ಯದಿ (executable_path == ರಿಕ್ತ || executable_path[0] == '\0')
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  ಯದಿ (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free ವರ್ಣ *cwd_allocated = ರಿಕ್ತ;

      ಯದಿ (cwd == ರಿಕ್ತ)
        {
          cwd_allocated = getcwd (ರಿಕ್ತ, 0);
          ಯದಿ (cwd_allocated == ರಿಕ್ತ)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  ಯದಿ (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      ಯದಿ (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          ಪ್ರತ್ಯಾಗಮ 0;
        }
      last_error = ret;
      ಗಚ್ಛ fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  ಯಾವತ್ ((it = strsep (&end, ":")))
    {
      cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;

      ಯದಿ (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ಅನುವರ್ತ;
        }

      ret = check_access (path);
      ಯದಿ (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = ರಿಕ್ತ;
          ಪ್ರತ್ಯಾಗಮ 0;
        }

      ಯದಿ (ret == -ENOENT)
        ಅನುವರ್ತ;

      last_error = ret;
    }

fail:
  ವಿಕಲ್ಪ (last_error)
    {
    ಪಕ್ಷ CHECK_ACCESS_NOT_EXECUTABLE:
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    ಪಕ್ಷ CHECK_ACCESS_NOT_REGULAR:
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    ಸ್ವಭಾವ:
      errno = -last_error;
      ಯದಿ (errno == ENOENT)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

ಸ್ಥಿರ ssize_t
safe_read_xattr (ವರ್ಣ **ret, ಪೂರ್ಣಾಙ್ಕ sfd, ಧ್ರುವ ವರ್ಣ *srcname, ಧ್ರುವ ವರ್ಣ *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  ಯಾವತ್ (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      ಯದಿ (UNLIKELY (s < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      ಯದಿ (s < current_size)
        ಭಙ್ಗ;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  ಯದಿ (s <= 0)
    ಪ್ರತ್ಯಾಗಮ s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ s;
}

ಸ್ಥಿರ ssize_t
copy_xattr (ಪೂರ್ಣಾಙ್ಕ sfd, ಪೂರ್ಣಾಙ್ಕ dfd, ಧ್ರುವ ವರ್ಣ *srcname, ಧ್ರುವ ವರ್ಣ *destname, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *buf = ರಿಕ್ತ;
  ssize_t xattr_len;
  ವರ್ಣ *it;

  xattr_len = flistxattr (sfd, ರಿಕ್ತ, 0);
  ಯದಿ (UNLIKELY (xattr_len < 0))
    {
      ಯದಿ (errno == ENOTSUP)
        ಪ್ರತ್ಯಾಗಮ 0;

      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  ಯದಿ (xattr_len == 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  ಯದಿ (UNLIKELY (xattr_len < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "flistxattr `%s`", srcname);

  ಪ್ರತಿ (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free ವರ್ಣ *v = ರಿಕ್ತ;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      ಯದಿ (UNLIKELY (s < 0))
        ಪ್ರತ್ಯಾಗಮ s;

      s = fsetxattr (dfd, it, v, s, 0);
      ಯದಿ (UNLIKELY (s < 0))
        {
          ಯದಿ (errno == EINVAL || errno == EOPNOTSUPP)
            ಅನುವರ್ತ;

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

#endif

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
copy_rec_stat_file_at (ಪೂರ್ಣಾಙ್ಕ dfd, ಧ್ರುವ ವರ್ಣ *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  ಸಂರಚನಾ stat st;
  ಪೂರ್ಣಾಙ್ಕ ret;

#ifdef HAVE_STATX
  ಸಂರಚನಾ statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno == ENOSYS || errno == EINVAL)
        ಗಚ್ಛ fallback;

      ಪ್ರತ್ಯಾಗಮ ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  ಪ್ರತ್ಯಾಗಮ ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
copy_recursive_fd_to_fd (ಪೂರ್ಣಾಙ್ಕ srcdirfd, ಪೂರ್ಣಾಙ್ಕ dfd, ಧ್ರುವ ವರ್ಣ *srcname, ಧ್ರುವ ವರ್ಣ *destname, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = ರಿಕ್ತ;
  ಸಂರಚನಾ dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  ಯದಿ (UNLIKELY (dsrcfd == ರಿಕ್ತ))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open directory `%s`", destname);
    }

  ಪ್ರತಿ (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ srcfd = -1;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ destfd = -1;
      cleanup_free ವರ್ಣ *target_buf = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      ಯದಿ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ಅನುವರ್ತ;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      ವಿಕಲ್ಪ (mode & S_IFMT)
        {
        ಪಕ್ಷ S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          ಯದಿ (UNLIKELY (srcfd < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          ಯದಿ (UNLIKELY (destfd < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

#ifdef HAVE_FGETXATTR
          ret = (ಪೂರ್ಣಾಙ್ಕ) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          ಭಙ್ಗ;

        ಪಕ್ಷ S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ಯದಿ (UNLIKELY (srcfd < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ಯದಿ (UNLIKELY (destfd < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (ಪೂರ್ಣಾಙ್ಕ) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          ಭಙ್ಗ;

        ಪಕ್ಷ S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          ಭಙ್ಗ;

        ಪಕ್ಷ S_IFBLK:
        ಪಕ್ಷ S_IFCHR:
        ಪಕ್ಷ S_IFIFO:
        ಪಕ್ಷ S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          ಭಙ್ಗ;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          ಯದಿ (errno == ENOTSUP)
            ಅನುವರ್ತ;

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಧ್ರುವ ವರ್ಣ *
find_annotation (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *name)
{
  ಯದಿ (container->container_def->annotations == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ find_string_map_ಮೌಲ್ಯ (container->annotations, name);
}

ಪೂರ್ಣಾಙ್ಕ
safe_write (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *fname, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ, libcrun_error_t *err)
{
  size_t written = 0;
  ಯಾವತ್ (written < ಎಣಿಕೆ)
    {
      ssize_t w = ಲಿಖ (fd, (ಧ್ರುವ ವರ್ಣ *) buf + written, ಎಣಿಕೆ - written);
      ಯದಿ (UNLIKELY (w < 0))
        {
          ಯದಿ (errno == EINTR || errno == EAGAIN)
            ಅನುವರ್ತ;
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

#define MAX_PARTS 32

ಪೂರ್ಣಾಙ್ಕ
append_paths (ವರ್ಣ **out, libcrun_error_t *err, ...)
{
  ಧ್ರುವ ವರ್ಣ *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t total_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  ಪ್ರತಿ (;;)
    {
      ಧ್ರುವ ವರ್ಣ *part;
      size_t size;

      part = va_arg (ap, ಧ್ರುವ ವರ್ಣ *);
      ಯದಿ (part == ರಿಕ್ತ)
        ಭಙ್ಗ;

      ಯದಿ (n_parts == MAX_PARTS)
        {
          va_end (ap);
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "too many paths specified");
        }

      ಯದಿ (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          ಯಾವತ್ (part[0] == '/' && part[1] == '/')
            part++;
        }
      ಅನ್ಯಥಾ
        {
          /* And drop any initial '/' for other components.  */
          ಯಾವತ್ (part[0] == '/')
            part++;
        }

      size = strlen (part);
      ಯದಿ (size == 0)
        ಅನುವರ್ತ;

      ಯಾವತ್ (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  ಪ್ರತಿ (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  ಪ್ರತಿ (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      ಯದಿ (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  ಪ್ರತ್ಯಾಗಮ 0;
}

#ಯದಿ __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#ಅನ್ಯಥಾ
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
b64_ಒಳಹರಿವು (ವರ್ಣ c)
{
  ಧ್ರುವ ವರ್ಣ table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  ಪೂರ್ಣಾಙ್ಕ i;

  ಪ್ರತಿ (i = 0; i < 64; i++)
    {
      ಯದಿ (table[i] == c)
        ಪ್ರತ್ಯಾಗಮ i;
    }
  ಪ್ರತ್ಯಾಗಮ -1;
}

ಪೂರ್ಣಾಙ್ಕ
base64_decode (ಧ್ರುವ ವರ್ಣ *iptr, size_t isize, ವರ್ಣ *optr, size_t osize, size_t *nbytes)
{
  ಪೂರ್ಣಾಙ್ಕ i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned ವರ್ಣ data[4];

  *nbytes = 0;
  ಯಾವತ್ (consumed < isize && (*nbytes) + 3 < osize)
    {
      ಯಾವತ್ ((i < 4) && (consumed < isize))
        {
          tmp = b64_ಒಳಹರಿವು (*iptr++);
          consumed++;
          ಯದಿ (tmp != -1)
            {
              ಯದಿ (UNLIKELY (pad > 0))
                ಪ್ರತ್ಯಾಗಮ -1;
              data[i++] = tmp;
            }
          ಅನ್ಯಥಾ ಯದಿ (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      ಯದಿ (i == 4)
        {
          ಯದಿ (UNLIKELY (pad > 2))
            ಪ್ರತ್ಯಾಗಮ -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      ಅನ್ಯಥಾ
        {
          /* I did not get all the data.  */
          consumed -= i;
          ಪ್ರತ್ಯಾಗಮ consumed;
        }
      i = 0;
    }
  ಪ್ರತ್ಯಾಗಮ consumed;
}

ವರ್ಣ *
get_user_name (uid_t uid)
{
  ಸಂರಚನಾ passwd pd;
  ಸಂರಚನಾ passwd *temp_result_ptr;
  ವರ್ಣ pwdbuffer[200];
  ಯದಿ (! getpwuid_r (uid, &pd, pwdbuffer, ಪರಿಮಾಣ (pwdbuffer), &temp_result_ptr))
    ಪ್ರತ್ಯಾಗಮ xstrdup (pd.pw_name);
  ಪ್ರತ್ಯಾಗಮ xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
ವರ್ಣ *
libcrun_sysctl_ಕೀಲಿ_to_proc_path (ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ)
{
  ಧ್ರುವ ವರ್ಣ *sep;
  ವರ್ಣ *ret, *it;

  sep = strpbrk (ಕೀಲಿ, "./");

  /* No separator at all, or the key is already in the path form.  */
  ಯದಿ (sep == ರಿಕ್ತ || *sep == '/')
    ಪ್ರತ್ಯಾಗಮ xstrdup (ಕೀಲಿ);

  ret = xstrdup (ಕೀಲಿ);
  ಪ್ರತಿ (it = ret; *it; it++)
    {
      ಯದಿ (*it == '.')
        *it = '/';
      ಅನ್ಯಥಾ ಯದಿ (*it == '/')
        *it = '.';
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
has_suffix (ಧ್ರುವ ವರ್ಣ *str, ಧ್ರುವ ವರ್ಣ *suffix)
{
  ಯದಿ (! str || ! suffix)
    ಪ್ರತ್ಯಾಗಮ 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  ಯದಿ (lensuffix > lenstr)
    ಪ್ರತ್ಯಾಗಮ 0;
  ಪ್ರತ್ಯಾಗಮ memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

ವರ್ಣ *
str_join_array (ಪೂರ್ಣಾಙ್ಕ offset, size_t size, ವರ್ಣ *ಧ್ರುವ array[], ಧ್ರುವ ವರ್ಣ *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = ರಿಕ್ತ;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  ವರ್ಣ *result, *p;

  lens = xmalloc (size * ಪರಿಮಾಣ (*lens));

  ಪ್ರತಿ (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  ಪ್ರತಿ (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      ಯದಿ (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  ಪ್ರತ್ಯಾಗಮ result;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_mmap (ಸಂರಚನಾ libcrun_mmap_s **ret, ಶೂನ್ಯ *addr, size_t length,
              ಪೂರ್ಣಾಙ್ಕ prot, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ fd, off_t offset,
              libcrun_error_t *err)
{
  ಸಂರಚನಾ libcrun_mmap_s *mmap_s = ರಿಕ್ತ;

  ಶೂನ್ಯ *mapped = mmap (addr, length, prot, flags, fd, offset);
  ಯದಿ (mapped == MAP_FAILED)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (ಪರಿಮಾಣ (ಸಂರಚನಾ libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_munmap (ಸಂರಚನಾ libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = munmap (mmap->addr, mmap->length);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "munmap");

  ಮುಕ್ತ (mmap);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ long
read_file_as_long_or_default (ಧ್ರುವ ವರ್ಣ *path, long def_ಮೌಲ್ಯ)
{
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  ವರ್ಣ *endptr;
  long val;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = read_all_file (path, &content, ರಿಕ್ತ, &tmp_err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ಪ್ರತ್ಯಾಗಮ def_ಮೌಲ್ಯ;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  ಯದಿ (UNLIKELY (errno))
    ಪ್ರತ್ಯಾಗಮ def_ಮೌಲ್ಯ;
  ಯದಿ (endptr == content || (*endptr && *endptr != '\n'))
    ಪ್ರತ್ಯಾಗಮ def_ಮೌಲ್ಯ;
  ಪ್ರತ್ಯಾಗಮ val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  ಯದಿ (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  ಪ್ರತ್ಯಾಗಮ uid;
}

gid_t
get_overflow_gid (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  ಯದಿ (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  ಪ್ರತ್ಯಾಗಮ gid;
}

ಶೂನ್ಯ
consume_trailing_slashes (ವರ್ಣ *path)
{
  ಯದಿ (! path || path[0] == '\0')
    ಪ್ರತ್ಯಾಗಮ;

  ವರ್ಣ *last = path + strlen (path);

  ಯಾವತ್ (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

ವರ್ಣ **
read_dir_entries (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = ರಿಕ್ತ;
  size_t n_entries = 0;
  size_t entries_size = 16;
  ವರ್ಣ **entries = ರಿಕ್ತ;
  ಸಂರಚನಾ dirent *de;

  dir = opendir (path);
  ಯದಿ (UNLIKELY (dir == ರಿಕ್ತ))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

  entries = xmalloc (entries_size * ಪರಿಮಾಣ (ವರ್ಣ *));
  ಯಾವತ್ ((de = readdir (dir)))
    {
      ಯದಿ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ಅನುವರ್ತ;
      ಯದಿ (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * ಪರಿಮಾಣ (ವರ್ಣ *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * ಪರಿಮಾಣ (ವರ್ಣ *));
  entries[n_entries] = ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ entries;
}

ಪೂರ್ಣಾಙ್ಕ
cpuset_string_to_bitmask (ಧ್ರುವ ವರ್ಣ *str, ವರ್ಣ **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *mask = ರಿಕ್ತ;
  size_t mask_size = 0;
  ಧ್ರುವ ವರ್ಣ *p = str;
  ವರ್ಣ *endptr;

  ಯಾವತ್ (*p)
    {
      long long start_range, end_range;

      ಯದಿ (*p < '0' || *p > '9')
        ಗಚ್ಛ invalid_ಒಳಹರಿವು;

      start_range = strtoll (p, &endptr, 10);
      ಯದಿ (start_range < 0)
        ಗಚ್ಛ invalid_ಒಳಹರಿವು;

      p = endptr;

      ಯದಿ (*p != '-')
        end_range = start_range;
      ಅನ್ಯಥಾ
        {
          p++;

          ಯದಿ (*p < '0' || *p > '9')
            ಗಚ್ಛ invalid_ಒಳಹರಿವು;

          end_range = strtoll (p, &endptr, 10);

          ಯದಿ (end_range < start_range)
            ಗಚ್ಛ invalid_ಒಳಹರಿವು;

          p = endptr;
        }

      /* Just set some limit.  */
      ಯದಿ (end_range > (1 << 20))
        ಗಚ್ಛ invalid_ಒಳಹರಿವು;

      ಯದಿ (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      ಪ್ರತಿ (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      ಯದಿ (*p == ',')
        p++;
      ಅನ್ಯಥಾ ಯದಿ (*p)
        ಗಚ್ಛ invalid_ಒಳಹರಿವು;
    }

  *out = mask;
  mask = ರಿಕ್ತ;
  *out_size = mask_size;

  ಪ್ರತ್ಯಾಗಮ 0;

invalid_ಒಳಹರಿವು:
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot parse input `%s`", str);
}

ಸಂರಚನಾ channel_fd_pair
{
  ಸಂರಚನಾ ring_buffer *rb;

  ಪೂರ್ಣಾಙ್ಕ in_fd;
  ಪೂರ್ಣಾಙ್ಕ out_fd;

  ಪೂರ್ಣಾಙ್ಕ infd_epoll_events;
  ಪೂರ್ಣಾಙ್ಕ outfd_epoll_events;
};

ಸಂರಚನಾ channel_fd_pair *
channel_fd_pair_new (ಪೂರ್ಣಾಙ್ಕ in_fd, ಪೂರ್ಣಾಙ್ಕ out_fd, size_t size)
{
  ಸಂರಚನಾ channel_fd_pair *channel = xmalloc (ಪರಿಮಾಣ (ಸಂರಚನಾ channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  ಪ್ರತ್ಯಾಗಮ channel;
}

ಶೂನ್ಯ
channel_fd_pair_free (ಸಂರಚನಾ channel_fd_pair *channel)
{
  ಯದಿ (channel == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ring_buffer_free (channel->rb);
  ಮುಕ್ತ (channel);
}

ಪೂರ್ಣಾಙ್ಕ
channel_fd_pair_process (ಸಂರಚನಾ channel_fd_pair *channel, ಪೂರ್ಣಾಙ್ಕ epollfd, libcrun_error_t *err)
{
  bool is_ಒಳಹರಿವು_eagain = false, is_output_eagain = false, repeat;
  ಪೂರ್ಣಾಙ್ಕ ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  ಪ್ರತಿ (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      ಯದಿ (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_ಒಳಹರಿವು_eagain, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          ಯದಿ (ret > 0)
            repeat = true;
        }
      ಯದಿ (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          ಯದಿ (ret > 0)
            repeat = true;
        }
    }

  ಯದಿ (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      ಪೂರ್ಣಾಙ್ಕ events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_ಒಳಹರಿವು_eagain ? EPOLLET : 0)) : 0;
      ಯದಿ (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      ಯದಿ (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          channel->outfd_epoll_events = events;
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_proc (libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fsfd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  ಯದಿ (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ರಿಕ್ತ, ರಿಕ್ತ, 0);
      ಯದಿ (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          ಯದಿ (fd >= 0)
            ಪ್ರತ್ಯಾಗಮ fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  ಯದಿ (fd < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `/proc`");
  ಪ್ರತ್ಯಾಗಮ fd;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  ಯದಿ (! container)
    ಪ್ರತ್ಯಾಗಮ open_proc (err);

  ಯದಿ (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  ಪ್ರತ್ಯಾಗಮ container->proc_fd;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_open_proc_file (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ flags, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  ಯದಿ (proc_fd < 0)
    ಪ್ರತ್ಯಾಗಮ proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  ಯದಿ (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  ಪ್ರತ್ಯಾಗಮ fd;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ flags, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *full_path = ರಿಕ್ತ;
  xasprintf (&full_path, "%d/%s", pid, path);
  ಪ್ರತ್ಯಾಗಮ libcrun_open_proc_file (container, full_path, flags, err);
}
