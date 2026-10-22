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
#অন্তর্ভাব <config.h>
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "ring_buffer.h"
#অন্তর্ভাব <stdarg.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <libgen.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/un.h>
#অন্তর্ভাব <sys/socket.h>
#অন্তর্ভাব <sys/signalfd.h>
#অন্তর্ভাব <sys/epoll.h>
#অন্তর্ভাব <sys/syscall.h>
#অন্তর্ভাব <sys/wait.h>
#অন্তর্ভাব <pwd.h>
#অন্তর্ভাব <grp.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <sys/time.h>
#অন্তর্ভাব <sys/xattr.h>
#অন্তর্ভাব <sys/sysmacros.h>
#অন্তর্ভাব <sys/vfs.h>
#অন্তর্ভাব <linux/magic.h>
#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <sys/mman.h>
#ifdef HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#endif
#যদি HAVE_STDATOMIC_H
#  include <stdatomic.h>
#অন্যথা
#  define atomic_long চঞ্চল long
#  define atomic_int চঞ্চল পূর্ণাঙ্ক
#endif
#অন্তর্ভাব "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_মূল
#  define RESOLVE_IN_মূল 0x10
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

পূর্ণাঙ্ক
crun_path_exists (ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret = access (path, F_OK);
  যদি (ret < 0)
    {
      যদি (errno == ENOENT)
        প্রত্যাগম 0;
      প্রত্যাগম crun_make_error (err, errno, "access `%s`", path);
    }
  প্রত্যাগম 1;
}

পূর্ণাঙ্ক
xasprintf (বর্ণ **str, ধ্রুব বর্ণ *fmt, ...)
{
  পূর্ণাঙ্ক ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  যদি (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
write_file_at_with_flags (পূর্ণাঙ্ক dirfd, পূর্ণাঙ্ক flags, mode_t mode, ধ্রুব বর্ণ *name, ধ্রুব শূন্য *data, size_t len, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক fd = -1;
  পূর্ণাঙ্ক ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম (len > INT_MAX) ? INT_MAX : (পূর্ণাঙ্ক) len;
}

পূর্ণাঙ্ক
detach_process ()
{
  pid_t pid;
  যদি (setsid () < 0)
    প্রত্যাগম -1;
  pid = fork ();
  যদি (pid < 0)
    প্রত্যাগম -1;
  যদি (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
get_file_type_fd (পূর্ণাঙ্ক fd, mode_t *mode)
{
  সংরচনা stat st;
  পূর্ণাঙ্ক ret;

#ifdef HAVE_STATX
  সংরচনা statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOSYS || errno == EINVAL)
        গচ্ছ fallback;

      প্রত্যাগম ret;
    }
  *mode = stx.stx_mode;
  প্রত্যাগম ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
get_file_type_at (পূর্ণাঙ্ক dirfd, mode_t *mode, bool nofollow, ধ্রুব বর্ণ *path)
{
  পূর্ণাঙ্ক empty_path = path == রিক্ত ? AT_EMPTY_PATH : 0;
  সংরচনা stat st;
  পূর্ণাঙ্ক ret;

#ifdef HAVE_STATX
  সংরচনা statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOSYS || errno == EINVAL)
        গচ্ছ fallback;

      প্রত্যাগম ret;
    }
  *mode = stx.stx_mode;
  প্রত্যাগম ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
get_file_type (mode_t *mode, bool nofollow, ধ্রুব বর্ণ *path)
{
  প্রত্যাগম get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

পূর্ণাঙ্ক
create_file_if_missing_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  যদি (fd_write < 0)
    {
      পূর্ণাঙ্ক saved_errno = errno;
      mode_t tmp_mode;
      পূর্ণাঙ্ক ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      যদি (ret == 0 && S_ISREG (tmp_mode))
        প্রত্যাগম 0;

      প্রত্যাগম crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
ensure_directory_internal_at (পূর্ণাঙ্ক dirfd, বর্ণ *path, size_t len, পূর্ণাঙ্ক mode, libcrun_error_t *err)
{
  বর্ণ *it = path + len;
  পূর্ণাঙ্ক ret = 0;
  bool parent_created = false;

  প্রতি (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      যদি (ret == 0 || errno == EEXIST)
        প্রত্যাগম 0;

      পূর্ণাঙ্ক saved_errno = errno;
      যদি (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = রিক্ত;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          যদি (ret > 0)
            প্রত্যাগম 0;
          যদি (ret < 0)
            crun_error_release (&tmp_err);

          প্রত্যাগম crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      যাবত্ (it > path && *it != '/')
        {
          it--;
          len--;
        }
      যদি (it == path)
        প্রত্যাগম 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      parent_created = true;
    }
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
crun_ensure_directory_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode, bool nofollow, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_free বর্ণ *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (ret == 0)
    প্রত্যাগম crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
check_fd_is_path (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  বর্ণ link[PATH_MAX];
  পূর্ণাঙ্ক ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, পরিমাণ (link)));
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "readlink `%s`", fdname);

  যদি (((size_t) ret) != path_len || memcmp (link, path, path_len))
    প্রত্যাগম crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
check_fd_under_path (ধ্রুব বর্ণ *rootfs, size_t rootfslen, পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  বর্ণ link[PATH_MAX];
  পূর্ণাঙ্ক ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  যদি (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    প্রত্যাগম 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, পরিমাণ (link)));
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "readlink `%s`", fdname);

  যদি (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    প্রত্যাগম crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  প্রত্যাগম 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
স্থির শূন্য
close_and_replace (পূর্ণাঙ্ক *oldfd, পূর্ণাঙ্ক newfd)
{
  যদি (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
বর্ণ *chroot_realpath (ধ্রুব বর্ণ *chroot, ধ্রুব বর্ণ *path, বর্ণ resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
স্থির পূর্ণাঙ্ক
safe_openat_fallback (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *rootfs, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক flags,
                      পূর্ণাঙ্ক mode, libcrun_error_t *err)
{
  cleanup_free বর্ণ *parent_path = রিক্ত;
  ধ্রুব বর্ণ *last_component = রিক্ত;
  ধ্রুব বর্ণ *orig_path = path;
  ধ্রুব বর্ণ *path_in_chroot;
  cleanup_close পূর্ণাঙ্ক fd = -1;
  বর্ণ resolved[PATH_MAX];
  বর্ণ buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  পূর্ণাঙ্ক ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  যদি (flags & O_NOFOLLOW)
    {
      বর্ণ *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      যদি (sep == রিক্ত)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      অন্যথা যদি (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      যদি (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  যদি (path_in_chroot == রিক্ত)
    প্রত্যাগম crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  যদি (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  যদি (last_component)
    {
      ret = snprintf (resolved, পরিমাণ (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      যদি (UNLIKELY (ret >= (পূর্ণাঙ্ক) পরিমাণ (resolved)))
        প্রত্যাগম crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  যদি (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      প্রত্যাগম ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = fd;
  fd = -1;
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
safe_openat (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *rootfs, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক mode,
             libcrun_error_t *err)
{
  স্থির bool openat2_supported = true;
  পূর্ণাঙ্ক retries = MAX_OPENAT2_EAGAIN_RETRIES;
  পূর্ণাঙ্ক ret;

  যদি (is_empty_string (path))
    {
      cleanup_close পূর্ণাঙ্ক fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      যদি (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      যদি (UNLIKELY (fd < 0))
        প্রত্যাগম crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      যদি (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }

      ret = fd;
      fd = -1;
      প্রত্যাগম ret;
    }

  যদি (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_মূল);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (errno == EINTR)
            গচ্ছ repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          যদি (errno == EAGAIN && retries-- > 0)
            গচ্ছ repeat;
          যদি (errno == ENOSYS)
            openat2_supported = false;
          যদি (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            প্রত্যাগম safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          প্রত্যাগম crun_make_error (err, errno, "openat2 `%s`", path);
        }

      প্রত্যাগম ret;
    }

  প্রত্যাগম safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (পূর্ণাঙ্ক dfd, ধ্রুব বর্ণ *name, বর্ণ **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free বর্ণ *tmp_buf = রিক্ত;
  ssize_t size;

  কুরু
    {
      যদি (tmp_buf != রিক্ত)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      যদি (UNLIKELY (size < 0))
        প্রত্যাগম crun_make_error (err, errno, "readlink `%s`", name);
  } যাবত্ (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = রিক্ত;

  প্রত্যাগম size;
}

স্থির পূর্ণাঙ্ক crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *dirpath,
                                ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode, পূর্ণাঙ্ক max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
স্থির পূর্ণাঙ্ক
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *dirpath, পূর্ণাঙ্ক cwd,
                                 ধ্রুব বর্ণ *npath, ধ্রুব বর্ণ *cur, ধ্রুব বর্ণ *rest,
                                 পূর্ণাঙ্ক mode, পূর্ণাঙ্ক max_readlinks, libcrun_error_t *err)
{
  cleanup_free বর্ণ *target = রিক্ত;
  cleanup_free বর্ণ *new_path = রিক্ত;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (পূর্ণাঙ্ক) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  প্রত্যাগম crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

স্থির পূর্ণাঙ্ক
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *dirpath,
                     ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode, পূর্ণাঙ্ক max_readlinks, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক wd_cleanup = -1;
  cleanup_free বর্ণ *npath = রিক্ত;
  bool last_component = false;
  size_t depth = 0;
  বর্ণ *cur;
  বর্ণ *it;
  পূর্ণাঙ্ক cwd;
  পূর্ণাঙ্ক ret;

  যদি (max_readlinks <= 0)
    প্রত্যাগম crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  যদি (*path == '\0')
    {
      যদি (do_open)
        প্রত্যাগম open (dirpath, O_CLOEXEC | O_PATH, 0);
      প্রত্যাগম 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  যাবত্ (cur)
    {
      যদি (it)
        *it = '\0';
      অন্যথা
        last_component = true;

      যদি (cur[0] == '\0')
        ভঙ্গ;

      যদি (strcmp (cur, ".") == 0)
        গচ্ছ next;
      অন্যথা যদি (strcmp (cur, ".."))
        depth++;
      অন্যথা
        {
          যদি (depth)
            depth--;
          অন্যথা
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              গচ্ছ next;
            }
        }

      যদি (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          যদি (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              যদি (errno == ELOOP)
                প্রত্যাগম crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, রিক্ত,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              যদি (ret < 0)
                প্রত্যাগম crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          যদি (do_open)
            প্রত্যাগম ret;

          close_and_replace (&wd_cleanup, ret);
          প্রত্যাগম 0;
        }

      ret = mkdirat (cwd, cur, mode);
      যদি (ret < 0)
        {
          mode_t st_mode;

          যদি (errno != EEXIST)
            প্রত্যাগম crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          যদি (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            প্রত্যাগম crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : রিক্ত, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      যদি (UNLIKELY (cwd < 0))
        প্রত্যাগম crun_error_wrap (err, "creating `/%s`", path);

      যদি (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, রিক্ত);
          যদি (UNLIKELY (ret < 0))
            {
              পূর্ণাঙ্ক saved_errno = errno;

              close (cwd);
              প্রত্যাগম crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          যদি ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              প্রত্যাগম crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      যদি (it == রিক্ত)
        ভঙ্গ;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  যদি (do_open)
    {
      যদি (cwd == dirfd)
        {
          ret = dup (dirfd);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "dup `%s`", dirpath);
          প্রত্যাগম ret;
        }

      wd_cleanup = -1;
      প্রত্যাগম cwd;
    }

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
crun_safe_create_and_open_ref_at (bool dir, পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *dirpath, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  যদি (LIKELY (ret >= 0))
    প্রত্যাগম ret;

  crun_error_release (err);
  প্রত্যাগম crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

পূর্ণাঙ্ক
crun_safe_ensure_directory_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *dirpath, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode,
                               libcrun_error_t *err)
{
  প্রত্যাগম crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

পূর্ণাঙ্ক
crun_safe_ensure_file_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *dirpath, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode,
                          libcrun_error_t *err)
{
  প্রত্যাগম crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

পূর্ণাঙ্ক
crun_ensure_directory (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode, bool nofollow, libcrun_error_t *err)
{
  প্রত্যাগম crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

স্থির পূর্ণাঙ্ক
get_file_size (পূর্ণাঙ্ক fd, off_t *size)
{
  সংরচনা stat st;
  পূর্ণাঙ্ক ret;
#ifdef HAVE_STATX
  সংরচনা statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOSYS || errno == EINVAL)
        গচ্ছ fallback;
      প্রত্যাগম ret;
    }
  *size = stx.stx_size;

  প্রত্যাগম ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
crun_dir_p_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  পূর্ণাঙ্ক ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "stat `%s`", path);

  প্রত্যাগম S_ISDIR (mode) ? 1 : 0;
}

পূর্ণাঙ্ক
crun_dir_p (ধ্রুব বর্ণ *path, bool nofollow, libcrun_error_t *err)
{
  প্রত্যাগম crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
স্থির পূর্ণাঙ্ক
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free বর্ণ *buffer = রিক্ত;
  size_t len;
  পূর্ণাঙ্ক ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  যদি (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      যদি (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          প্রত্যাগম 0;
        }
      প্রত্যাগম ret;
    }

  প্রত্যাগম strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
স্থির bool
running_in_openvz ()
{
  যদি (access ("/proc/vz", F_OK) < 0)
    প্রত্যাগম false;

  প্রত্যাগম access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

পূর্ণাঙ্ক
check_running_in_user_namespace (libcrun_error_t *err)
{
  স্থির atomic_int run_in_userns = -1;
  সংরচনা stat st;
  পূর্ণাঙ্ক ret;

  ret = run_in_userns;
  যদি (ret >= 0)
    প্রত্যাগম ret;

  ret = stat ("/proc/self/ns/user", &st);
  যদি (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      যদি (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          প্রত্যাগম run_in_userns;
        }

      যদি (! running_in_openvz ())
        {
          run_in_userns = 1;
          প্রত্যাগম run_in_userns;
        }
    }
  অন্যথা যদি (UNLIKELY (errno != ENOENT))
    প্রত্যাগম crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  run_in_userns = ret;
  প্রত্যাগম run_in_userns;
}

স্থির size_t
get_page_size ()
{
  স্থির atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  যদি (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  প্রত্যাগম (size_t) pagesize;
}

স্থির পূর্ণাঙ্ক selinux_enabled = -1;
স্থির পূর্ণাঙ্ক apparmor_enabled = -1;

পূর্ণাঙ্ক
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free বর্ণ *out = রিক্ত;
  cleanup_close পূর্ণাঙ্ক fd = -1;
  size_t len;
  পূর্ণাঙ্ক ret;

  যদি (selinux_enabled >= 0)
    প্রত্যাগম selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  প্রত্যাগম selinux_enabled;
}

পূর্ণাঙ্ক
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক fd = -1;
  পূর্ণাঙ্ক size;
  বর্ণ buf[1];

  যদি (apparmor_enabled >= 0)
    প্রত্যাগম apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  যদি (fd == -1)
    {
      যদি (errno == ENOENT)
        {
          apparmor_enabled = 0;
          প্রত্যাগম 0;
        }

      প্রত্যাগম crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (পঠ (fd, buf, পরিমাণ (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  প্রত্যাগম apparmor_enabled;
}

স্থির পূর্ণাঙ্ক
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  যদি (selinux_enabled < 0)
    প্রত্যাগম crun_make_error (err, 0, "SELinux is not initialized correctly");
  প্রত্যাগম selinux_enabled;
}

পূর্ণাঙ্ক
add_selinux_mount_label (বর্ণ **retlabel, ধ্রুব বর্ণ *data, ধ্রুব বর্ণ *label, ধ্রুব বর্ণ *context_type, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = libcrun_is_selinux_enabled (err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (label && ret)
    {
      যদি (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      অন্যথা
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      প্রত্যাগম 0;
    }
  *retlabel = xstrdup (data);
  প্রত্যাগম 0;
}

স্থির ধ্রুব বর্ণ *
lsm_attr_path (libcrun_container_t *container, ধ্রুব বর্ণ *lsm, ধ্রুব বর্ণ *fname, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক attr_dirfd = -1;
  cleanup_close পূর্ণাঙ্ক lsm_dirfd = -1;
  বর্ণ *attr_path = রিক্ত;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  যদি (UNLIKELY (attr_dirfd < 0))
    প্রত্যাগম রিক্ত;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  যদি (lsm != রিক্ত)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      যদি (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          প্রত্যাগম রিক্ত;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  প্রত্যাগম attr_path;
}

স্থির পূর্ণাঙ্ক
check_proc_super_magic (পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  সংরচনা statfs sfs;

  পূর্ণাঙ্ক ret = fstatfs (fd, &sfs);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "statfs `%s`", path);

  যদি (sfs.f_type != PROC_SUPER_MAGIC)
    প্রত্যাগম crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
set_security_attr (libcrun_container_t *container, ধ্রুব বর্ণ *lsm, ধ্রুব বর্ণ *fname, ধ্রুব বর্ণ *data, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  cleanup_free ধ্রুব বর্ণ *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close পূর্ণাঙ্ক fd = -1;

  যদি (UNLIKELY (attr_path == রিক্ত))
    প্রত্যাগম -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (লিখ (fd, data, strlen (data)));
  যদি (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      যদি (errno == ENOENT && lsm != রিক্ত && strcmp (lsm, "apparmor") == 0)
        {
          ধ্রুব বর্ণ *profile = strchr (data, ' ');

          প্রত্যাগম crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      প্রত্যাগম crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
set_selinux_label (libcrun_container_t *container, ধ্রুব বর্ণ *label, bool now, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = libcrun_is_selinux_enabled (err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (ret)
    প্রত্যাগম set_security_attr (container, রিক্ত, now ? "current" : "exec", label, err);
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  যদি (apparmor_enabled < 0)
    প্রত্যাগম crun_make_error (err, 0, "AppArmor is not initialized correctly");
  প্রত্যাগম apparmor_enabled;
}

স্থির পূর্ণাঙ্ক
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (পরিমাণ (UNCONFINED) - 1)
  cleanup_free ধ্রুব বর্ণ *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close পূর্ণাঙ্ক fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  বর্ণ buf[UNCONFINED_LEN + 1];

  যদি (UNLIKELY (attr_path == রিক্ত))
    প্রত্যাগম -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম fd;

  যদি (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    প্রত্যাগম -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (পঠ (fd, buf, পরিমাণ (buf)));
  যদি (UNLIKELY (bytes_read < 0))
    প্রত্যাগম crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  যদি (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    প্রত্যাগম 1;
  যদি (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    প্রত্যাগম 0;
  প্রত্যাগম 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

পূর্ণাঙ্ক
set_apparmor_profile (libcrun_container_t *container, ধ্রুব বর্ণ *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = libcrun_is_apparmor_enabled (err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (ret)
    {
      cleanup_free বর্ণ *buf = রিক্ত;
      ret = is_current_process_confined (container, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      প্রত্যাগম set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
read_all_fd_internal (পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *description, বর্ণ **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free বর্ণ *buf = রিক্ত;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  পূর্ণাঙ্ক ret;

  যদি (size_hint)
    allocated = size_hint;
  অন্যথা
    {
      ret = get_file_size (fd, &size);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "stat `%s`", description);

      যদি (max_size && (size_t) size > max_size)
        প্রত্যাগম crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  যাবত্ ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (পঠ (fd, buf + nread, allocated - nread));
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "read from file `%s`", description);

      যদি (ret == 0)
        ভঙ্গ;

      nread += ret;

      যদি (max_size && nread > max_size)
        প্রত্যাগম crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      যদি (nread == allocated)
        {
          যদি (size)
            ভঙ্গ;

          যদি (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  যদি (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      বর্ণ *tmp = realloc (buf, nread + 1);
      যদি (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = রিক্ত;
  যদি (len)
    *len = nread;
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
read_all_fd_with_size_hint (পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *description, বর্ণ **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  প্রত্যাগম read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

পূর্ণাঙ্ক
read_all_fd_with_max_size (পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *description, বর্ণ **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  প্রত্যাগম read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

পূর্ণাঙ্ক
read_all_file_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *path, বর্ণ **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", path);

  প্রত্যাগম read_all_fd (fd, path, out, len, err);
}

পূর্ণাঙ্ক
read_all_file (ধ্রুব বর্ণ *path, বর্ণ **out, size_t *len, libcrun_error_t *err)
{
  যদি (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  প্রত্যাগম read_all_file_at (AT_FDCWD, path, out, len, err);
}

পূর্ণাঙ্ক
get_realpath_to_file (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *path_name, বর্ণ **absolute_path, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  যদি (UNLIKELY (targetfd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", path_name);
  অন্যথা
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      যদি (UNLIKELY (len < 0))
        প্রত্যাগম crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
open_unix_domain_client_socket (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক dgram, libcrun_error_t *err)
{
  সংরচনা sockaddr_un addr = {};
  পূর্ণাঙ্ক ret;
  proc_fd_path_t name_buf;
  cleanup_close পূর্ণাঙ্ক destfd = -1;
  cleanup_close পূর্ণাঙ্ক fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম crun_make_error (err, errno, "create UNIX socket");

  যদি (strlen (path) >= পরিমাণ (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (destfd < 0))
        প্রত্যাগম crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  যদি (path_len >= পরিমাণ (addr.sun_path))
    প্রত্যাগম crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (সংরচনা sockaddr *) &addr, পরিমাণ (addr));
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
open_unix_domain_socket (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক dgram, libcrun_error_t *err)
{
  সংরচনা sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  পূর্ণাঙ্ক ret;
  cleanup_close পূর্ণাঙ্ক dirfd = -1;
  cleanup_close পূর্ণাঙ্ক fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম crun_make_error (err, errno, "create UNIX socket");

  যদি (strlen (path) >= পরিমাণ (addr.sun_path))
    {
      cleanup_free বর্ণ *dpath = xstrdup (path);
      cleanup_free বর্ণ *bpath = xstrdup (path);
      ধ্রুব বর্ণ *parent_dir = dirname (dpath);
      ধ্রুব বর্ণ *base = basename (bpath);
      পূর্ণাঙ্ক n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      যদি (UNLIKELY (dirfd < 0))
        প্রত্যাগম crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, পরিমাণ (addr.sun_path), "%s/%s", name_buf, base);
      যদি (n < 0 || (size_t) n >= পরিমাণ (addr.sun_path))
        প্রত্যাগম crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  অন্যথা
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (সংরচনা sockaddr *) &addr, পরিমাণ (addr));
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "bind socket to `%s`", path);

  যদি (! dgram)
    {
      ret = listen (fd, 1);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
send_fd_to_socket (পূর্ণাঙ্ক server, পূর্ণাঙ্ক fd, libcrun_error_t *err)
{
  প্রত্যাগম send_fd_to_socket_with_payload (server, fd, রিক্ত, 0, err);
}

পূর্ণাঙ্ক
send_fd_to_socket_with_payload (পূর্ণাঙ্ক server, পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *payload, size_t payload_len, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  সংরচনা cmsghdr *cmsg = রিক্ত;
  সংরচনা iovec iov[2];
  সংরচনা msghdr বার্তা = {};
  বর্ণ ctrl_buf[CMSG_SPACE (1 + পরিমাণ (পূর্ণাঙ্ক))] = {};
  বর্ণ data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = পরিমাণ (data);

  যদি (payload_len > 0)
    {
      iov[0].iov_base = (শূন্য *) payload;
      iov[0].iov_len = payload_len;
    }

  বার্তা.বার্তা_name = রিক্ত;
  বার্তা.বার্তা_namelen = 0;
  বার্তা.বার্তা_iov = iov;
  বার্তা.বার্তা_iovlen = 1;
  বার্তা.বার্তা_controllen = CMSG_SPACE (পরিমাণ (পূর্ণাঙ্ক));
  বার্তা.বার্তা_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&বার্তা);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (পরিমাণ (পূর্ণাঙ্ক));

  *((পূর্ণাঙ্ক *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &বার্তা, 0));
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "sendmsg");
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
receive_fd_from_socket_with_payload (পূর্ণাঙ্ক from, বর্ণ *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক fd = -1;
  পূর্ণাঙ্ক ret;
  সংরচনা iovec iov[1];
  সংরচনা msghdr বার্তা = {};
  বর্ণ ctrl_buf[CMSG_SPACE (পরিমাণ (পূর্ণাঙ্ক))] = {};
  বর্ণ data[1];
  সংরচনা cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = পরিমাণ (data);

  যদি (payload_len > 0)
    {
      iov[0].iov_base = (শূন্য *) payload;
      iov[0].iov_len = payload_len;
    }

  বার্তা.বার্তা_name = রিক্ত;
  বার্তা.বার্তা_namelen = 0;
  বার্তা.বার্তা_iov = iov;
  বার্তা.বার্তা_iovlen = 1;
  বার্তা.বার্তা_controllen = CMSG_SPACE (পরিমাণ (পূর্ণাঙ্ক));
  বার্তা.বার্তা_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &বার্তা, 0));
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "recvmsg");
  যদি (UNLIKELY (ret == 0))
    প্রত্যাগম crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&বার্তা);
  যদি (cmsg == রিক্ত)
    প্রত্যাগম crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), পরিমাণ (fd));

  ret = fd;
  fd = -1;
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
receive_fd_from_socket (পূর্ণাঙ্ক from, libcrun_error_t *err)
{
  প্রত্যাগম receive_fd_from_socket_with_payload (from, রিক্ত, 0, err);
}

পূর্ণাঙ্ক
create_socket_pair (পূর্ণাঙ্ক *pair, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "socketpair");
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret = signalfd (-1, mask, 0);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "signalfd");
  প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক
epoll_helper_toggle (পূর্ণাঙ্ক epollfd, পূর্ণাঙ্ক fd, পূর্ণাঙ্ক events, libcrun_error_t *err)
{
  সংরচনা epoll_event ev = {};
  bool add = events != 0;
  পূর্ণাঙ্ক ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  যদি (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  যদি (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      যদি (errno == ENOENT || errno == EPERM)
        প্রত্যাগম 0;
      প্রত্যাগম crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
epoll_helper (পূর্ণাঙ্ক *in_fds, পূর্ণাঙ্ক *in_levelfds, পূর্ণাঙ্ক *out_fds, পূর্ণাঙ্ক *out_levelfds, libcrun_error_t *err)
{
  সংরচনা epoll_event ev;
  cleanup_close পূর্ণাঙ্ক epollfd = -1;
  পূর্ণাঙ্ক ret;

  পূর্ণাঙ্ক *it;
  epollfd = epoll_create1 (0);
  যদি (UNLIKELY (epollfd < 0))
    প্রত্যাগম crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  প্রতি (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      যদি (UNLIKELY (ret < 0))                                           \
        প্রত্যাগম crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  যদি (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  যদি (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  যদি (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  যদি (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
copy_from_fd_to_fd (পূর্ণাঙ্ক src, পূর্ণাঙ্ক dst, পূর্ণাঙ্ক consume, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  ssize_t nread;
  size_t pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  কুরু
    {
      cleanup_free বর্ণ *buffer = রিক্ত;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      যদি (can_copy_file_range)
        {
          nread = copy_file_range (src, রিক্ত, dst, রিক্ত, pagesize, 0);
          যদি (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              গচ্ছ fallback;
            }
          যদি (consume && nread < 0 && errno == EAGAIN)
            প্রত্যাগম 0;
          যদি (nread < 0 && errno == EIO)
            প্রত্যাগম 0;
          যদি (UNLIKELY (nread < 0))
            প্রত্যাগম crun_make_error (err, errno, "copy_file_range");
          অনুবর্ত;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (পঠ (src, buffer, pagesize));
      যদি (consume && nread < 0 && errno == EAGAIN)
        প্রত্যাগম 0;
      যদি (nread < 0 && errno == EIO)
        প্রত্যাগম 0;
      যদি (UNLIKELY (nread < 0))
        প্রত্যাগম crun_make_error (err, errno, "read");

      remaining = nread;
      যাবত্ (remaining)
        {
          ret = TEMP_FAILURE_RETRY (লিখ (dst, buffer + nread - remaining, remaining));
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } যাবত্ (consume && nread);

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
run_process (বর্ণ **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  যদি (UNLIKELY (pid < 0))
    প্রত্যাগম crun_make_error (err, errno, "fork");
  যদি (pid)
    {
      পূর্ণাঙ্ক r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      যদি (r < 0)
        প্রত্যাগম crun_make_error (err, errno, "waitpid");
      যদি (WIFEXITED (status) || WIFSIGNALED (status))
        প্রত্যাগম WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
স্থির unsigned
atou (বর্ণ **s)
{
  unsigned x;
  প্রতি (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  প্রত্যাগম x;
}

পূর্ণাঙ্ক
fgetpwent_r (FILE *f, সংরচনা passwd *pw, বর্ণ *সারি, size_t size, সংরচনা passwd **res)
{
  বর্ণ *s;
  পূর্ণাঙ্ক rv = 0;
  প্রতি (;;)
    {
      সারি[size - 1] = '\xff';
      যদি ((fgets (সারি, size, f) == রিক্ত) || ferror (f) || সারি[size - 1] != '\xff')
        {
          rv = (সারি[size - 1] != '\xff') ? ERANGE : ENOENT;
          সারি = 0;
          pw = 0;
          ভঙ্গ;
        }
      সারি[strcspn (সারি, "\n")] = 0;

      s = সারি;
      pw->pw_name = s++;
      যদি (! (s = strchr (s, ':')))
        অনুবর্ত;

      *s++ = 0;
      pw->pw_passwd = s;
      যদি (! (s = strchr (s, ':')))
        অনুবর্ত;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      যদি (*s != ':')
        অনুবর্ত;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      যদি (*s != ':')
        অনুবর্ত;

      *s++ = 0;
      pw->pw_gecos = s;
      যদি (! (s = strchr (s, ':')))
        অনুবর্ত;

      *s++ = 0;
      pw->pw_dir = s;
      যদি (! (s = strchr (s, ':')))
        অনুবর্ত;

      *s++ = 0;
      pw->pw_shell = s;
      ভঙ্গ;
    }
  *res = pw;
  যদি (rv)
    errno = rv;
  প্রত্যাগম rv;
}
#endif

পূর্ণাঙ্ক
set_home_env (uid_t id)
{
  সংরচনা passwd pwd;
  cleanup_free বর্ণ *buf = রিক্ত;
  long buf_size;
  cleanup_file FILE *stream = রিক্ত;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  যদি (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  যদি (stream == রিক্ত)
    প্রত্যাগম -1;

  প্রতি (;;)
    {
      সংরচনা passwd *ret_pw = রিক্ত;

      পূর্ণাঙ্ক ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      যদি (UNLIKELY (ret != 0))
        {
          যদি (ret == EINTR)
            অনুবর্ত;

          যদি (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              প্রত্যাগম -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          অনুবর্ত;
        }

      যদি (ret_pw && ret_pw->pw_uid == id)
        {
          যদি (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          অন্যথা
            প্রত্যাগম 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
স্থির পূর্ণাঙ্ক
getsubidrange (uid_t id, পূর্ণাঙ্ক is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *প্রবেশ = রিক্ত;
  cleanup_free বর্ণ *lineptr = রিক্ত;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free বর্ণ *buf = রিক্ত;
  ধ্রুব বর্ণ *name;
  সংরচনা passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  যদি (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  প্রতি (;;)
    {
      পূর্ণাঙ্ক ret;
      সংরচনা passwd *ret_pw = রিক্ত;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      যদি (LIKELY (ret == 0))
        {
          যদি (ret_pw)
            {
              name = ret_pw->pw_name;
              ভঙ্গ;
            }
          প্রত্যাগম -1;
        }

      যদি (ret != ERANGE)
        প্রত্যাগম -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  প্রবেশ = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  যদি (প্রবেশ == রিক্ত)
    প্রত্যাগম -1;

  প্রতি (;;)
    {
      বর্ণ *endptr;
      ssize_t পঠ = getline (&lineptr, &lenlineptr, প্রবেশ);
      যদি (পঠ < 0)
        প্রত্যাগম -1;

      যদি (পঠ < (ssize_t) (len_name + 2))
        অনুবর্ত;

      যদি (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        অনুবর্ত;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      যদি (endptr >= &lineptr[পঠ])
        প্রত্যাগম -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      প্রত্যাগম 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

পূর্ণাঙ্ক
format_default_id_mapping (বর্ণ **out, uid_t container_id, uid_t host_uid, uid_t host_id, পূর্ণাঙ্ক is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free বর্ণ *buffer = রিক্ত;
  পূর্ণাঙ্ক written = 0;
  পূর্ণাঙ্ক ret, remaining;

  *out = রিক্ত;

  যদি (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    প্রত্যাগম 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  যদি (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      যদি (UNLIKELY (ret >= remaining))
        প্রত্যাগম crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  যদি (UNLIKELY (ret >= remaining))
    প্রত্যাগম crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  যদি (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      যদি (UNLIKELY (ret >= remaining))
        প্রত্যাগম crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = রিক্ত;
  প্রত্যাগম written;
}

স্থির পূর্ণাঙ্ক
unset_cloexec_flag (পূর্ণাঙ্ক fd)
{
  পূর্ণাঙ্ক flags = fcntl (fd, F_GETFD);
  যদি (flags == -1)
    প্রত্যাগম -1;

  flags &= ~FD_CLOEXEC;

  প্রত্যাগম fcntl (fd, F_SETFD, flags);
}

স্থির শূন্য __attribute__ ((__noreturn__))
run_process_child (বর্ণ *path, বর্ণ **args, ধ্রুব বর্ণ *cwd, বর্ণ **envp, পূর্ণাঙ্ক pipe_r,
                   পূর্ণাঙ্ক pipe_w, পূর্ণাঙ্ক out_fd, পূর্ণাঙ্ক err_fd, bool can_ignore_chdir_errors)
{
  বর্ণ *tmp_args[] = { path, রিক্ত };
  libcrun_error_t err = রিক্ত;
  পূর্ণাঙ্ক dev_null_fd = -1;
  পূর্ণাঙ্ক ret;

  ret = mark_or_close_fds_ge_than (রিক্ত, 3, false, &err);
  যদি (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->বার্তা);

  যদি (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      যদি (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  যদি (out_fd >= 0)
    unset_cloexec_flag (1);
  যদি (err_fd >= 0)
    unset_cloexec_flag (2);

  যদি (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  যদি (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  যদি (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  যদি (args == রিক্ত)
    args = tmp_args;

  যদি (cwd && chdir (cwd) < 0)
    {
      যদি (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
পূর্ণাঙ্ক
run_process_with_stdin_timeout_envp (বর্ণ *path, বর্ণ **args, ধ্রুব বর্ণ *cwd, পূর্ণাঙ্ক timeout,
                                     বর্ণ **envp, বর্ণ *stdin, size_t stdin_len, পূর্ণাঙ্ক out_fd,
                                     পূর্ণাঙ্ক err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  পূর্ণাঙ্ক stdin_pipe[2];
  pid_t pid;
  পূর্ণাঙ্ক ret;
  cleanup_close পূর্ণাঙ্ক pipe_r = -1;
  cleanup_close পূর্ণাঙ্ক pipe_w = -1;
  sigset_t oldmask, mask;
  পূর্ণাঙ্ক r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  যদি (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  যদি (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      গচ্ছ restore_sig_mask_and_exit;
    }

  যদি (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (লিখ (pipe_w, stdin, stdin_len));
  যদি (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      যদি (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          গচ্ছ restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  যদি (timeout)
    {
      time_t start = time (রিক্ত);
      time_t now;
      প্রতি (now = start; now - start < timeout; now = time (রিক্ত))
        {
          siginfo_t info;
          পূর্ণাঙ্ক elapsed = now - start;
          সংরচনা timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          যদি (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              গচ্ছ restore_sig_mask_and_exit;
            }

          যদি (info.si_signo == SIGCHLD && info.si_pid == pid)
            গচ্ছ read_waitpid;

          যদি (ret < 0 && errno == EAGAIN)
            গচ্ছ timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      গচ্ছ restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  যদি (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  অন্যথা
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  যদি (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      যদি (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, রিক্ত);
      যদি (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
mark_or_close_fds_ge_than (libcrun_container_t *container, পূর্ণাঙ্ক n, bool close_now, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক cfd = -1;
  cleanup_dir DIR *dir = রিক্ত;
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক fd;
  সংরচনা dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  যদি (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  যদি (ret == 0)
    {
      যদি (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      প্রত্যাগম 0;
    }
  যদি (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    প্রত্যাগম crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  যদি (UNLIKELY (cfd < 0))
    প্রত্যাগম cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  dir = fdopendir (cfd);
  যদি (UNLIKELY (dir == রিক্ত))
    প্রত্যাগম crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  প্রতি (next = readdir (dir); next; next = readdir (dir))
    {
      পূর্ণাঙ্ক val;
      ধ্রুব বর্ণ *name = next->d_name;
      যদি (name[0] == '.')
        অনুবর্ত;

      val = strtoll (name, রিক্ত, 10);
      যদি (val < n || val == fd)
        অনুবর্ত;

      যদি (close_now)
        {
          ret = close (val);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "close fd `%d`", val);

          যদি (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      অন্যথা
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  প্রত্যাগম 0;
}

শূন্য
get_current_timestamp (বর্ণ *out, size_t len)
{
  সংরচনা timeval tv;
  সংরচনা tm now;
  বর্ণ timestamp[64];

  gettimeofday (&tv, রিক্ত);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, পরিমাণ (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (শূন্য) snprintf (out, len, "%s.%06lldZ", timestamp, (long long পূর্ণাঙ্ক) tv.tv_usec);
  out[len - 1] = '\0';
}

পূর্ণাঙ্ক
set_blocking_fd (পূর্ণাঙ্ক fd, bool blocking, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret, flags = fcntl (fd, F_GETFL, 0);
  যদি (UNLIKELY (flags < 0))
    প্রত্যাগম crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "fcntl");
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
parse_json_file (json_object **out, ধ্রুব বর্ণ *jsondata, সংরচনা parser_context *ctx arg_unused, libcrun_error_t *err)
{
  সংরচনা json_tokener *tok;
  গণনা json_tokener_error jerr;

  *err = রিক্ত;

  যদি (UNLIKELY (jsondata == রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  যদি (UNLIKELY (tok == রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (পূর্ণাঙ্ক) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  যদি (*out == রিক্ত || jerr != json_tokener_success)
    {
      পূর্ণাঙ্ক saved_pos = (পূর্ণাঙ্ক) tok->char_offset;

      json_tokener_free (tok);
      যদি (*out)
        {
          json_object_put (*out);
          *out = রিক্ত;
        }
      প্রত্যাগম crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  প্রত্যাগম 0;
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
স্থির পূর্ণাঙ্ক
check_access (ধ্রুব বর্ণ *path)
{
  পূর্ণাঙ্ক ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#অন্যথা
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  যদি (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      যদি (errno == EACCES)
        {
          পূর্ণাঙ্ক saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          যদি (ret == 0)
            প্রত্যাগম CHECK_ACCESS_NOT_EXECUTABLE;
        }
      প্রত্যাগম -errno;
    }

  ret = get_file_type (&mode, false, path);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম -errno;

  যদি (! S_ISREG (mode))
    প্রত্যাগম CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
find_executable (বর্ণ **out, ধ্রুব বর্ণ *executable_path, ধ্রুব বর্ণ *cwd, libcrun_error_t *err)
{
  cleanup_free বর্ণ *cwd_executable_path = রিক্ত;
  cleanup_free বর্ণ *tmp = রিক্ত;
  পূর্ণাঙ্ক last_error = -ENOENT;
  বর্ণ *it, *end;
  পূর্ণাঙ্ক ret;

  *out = রিক্ত;

  যদি (executable_path == রিক্ত || executable_path[0] == '\0')
    প্রত্যাগম crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  যদি (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free বর্ণ *cwd_allocated = রিক্ত;

      যদি (cwd == রিক্ত)
        {
          cwd_allocated = getcwd (রিক্ত, 0);
          যদি (cwd_allocated == রিক্ত)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  যদি (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      যদি (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          প্রত্যাগম 0;
        }
      last_error = ret;
      গচ্ছ fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  যাবত্ ((it = strsep (&end, ":")))
    {
      cleanup_free বর্ণ *path = রিক্ত;

      যদি (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          অনুবর্ত;
        }

      ret = check_access (path);
      যদি (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = রিক্ত;
          প্রত্যাগম 0;
        }

      যদি (ret == -ENOENT)
        অনুবর্ত;

      last_error = ret;
    }

fail:
  বিকল্প (last_error)
    {
    পক্ষ CHECK_ACCESS_NOT_EXECUTABLE:
      প্রত্যাগম crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    পক্ষ CHECK_ACCESS_NOT_REGULAR:
      প্রত্যাগম crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    স্বভাব:
      errno = -last_error;
      যদি (errno == ENOENT)
        প্রত্যাগম crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      প্রত্যাগম crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

স্থির ssize_t
safe_read_xattr (বর্ণ **ret, পূর্ণাঙ্ক sfd, ধ্রুব বর্ণ *srcname, ধ্রুব বর্ণ *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free বর্ণ *buffer = রিক্ত;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  যাবত্ (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      যদি (UNLIKELY (s < 0))
        প্রত্যাগম crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      যদি (s < current_size)
        ভঙ্গ;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  যদি (s <= 0)
    প্রত্যাগম s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = রিক্ত;

  প্রত্যাগম s;
}

স্থির ssize_t
copy_xattr (পূর্ণাঙ্ক sfd, পূর্ণাঙ্ক dfd, ধ্রুব বর্ণ *srcname, ধ্রুব বর্ণ *destname, libcrun_error_t *err)
{
  cleanup_free বর্ণ *buf = রিক্ত;
  ssize_t xattr_len;
  বর্ণ *it;

  xattr_len = flistxattr (sfd, রিক্ত, 0);
  যদি (UNLIKELY (xattr_len < 0))
    {
      যদি (errno == ENOTSUP)
        প্রত্যাগম 0;

      প্রত্যাগম crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  যদি (xattr_len == 0)
    প্রত্যাগম 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  যদি (UNLIKELY (xattr_len < 0))
    প্রত্যাগম crun_make_error (err, errno, "flistxattr `%s`", srcname);

  প্রতি (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free বর্ণ *v = রিক্ত;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      যদি (UNLIKELY (s < 0))
        প্রত্যাগম s;

      s = fsetxattr (dfd, it, v, s, 0);
      যদি (UNLIKELY (s < 0))
        {
          যদি (errno == EINVAL || errno == EOPNOTSUPP)
            অনুবর্ত;

          প্রত্যাগম crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  প্রত্যাগম 0;
}

#endif

স্থির পূর্ণাঙ্ক
copy_rec_stat_file_at (পূর্ণাঙ্ক dfd, ধ্রুব বর্ণ *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  সংরচনা stat st;
  পূর্ণাঙ্ক ret;

#ifdef HAVE_STATX
  সংরচনা statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOSYS || errno == EINVAL)
        গচ্ছ fallback;

      প্রত্যাগম ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  প্রত্যাগম ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
copy_recursive_fd_to_fd (পূর্ণাঙ্ক srcdirfd, পূর্ণাঙ্ক dfd, ধ্রুব বর্ণ *srcname, ধ্রুব বর্ণ *destname, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = রিক্ত;
  সংরচনা dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  যদি (UNLIKELY (dsrcfd == রিক্ত))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      প্রত্যাগম crun_make_error (err, errno, "open directory `%s`", destname);
    }

  প্রতি (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close পূর্ণাঙ্ক srcfd = -1;
      cleanup_close পূর্ণাঙ্ক destfd = -1;
      cleanup_free বর্ণ *target_buf = রিক্ত;
      পূর্ণাঙ্ক ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      যদি (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        অনুবর্ত;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      বিকল্প (mode & S_IFMT)
        {
        পক্ষ S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          যদি (UNLIKELY (srcfd < 0))
            প্রত্যাগম crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          যদি (UNLIKELY (destfd < 0))
            প্রত্যাগম crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;

#ifdef HAVE_FGETXATTR
          ret = (পূর্ণাঙ্ক) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          ভঙ্গ;

        পক্ষ S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          যদি (UNLIKELY (srcfd < 0))
            প্রত্যাগম crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          যদি (UNLIKELY (destfd < 0))
            প্রত্যাগম crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (পূর্ণাঙ্ক) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          ভঙ্গ;

        পক্ষ S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          ভঙ্গ;

        পক্ষ S_IFBLK:
        পক্ষ S_IFCHR:
        পক্ষ S_IFIFO:
        পক্ষ S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          ভঙ্গ;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      যদি (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          যদি (errno == ENOTSUP)
            অনুবর্ত;

          প্রত্যাগম crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  প্রত্যাগম 0;
}

ধ্রুব বর্ণ *
find_annotation (libcrun_container_t *container, ধ্রুব বর্ণ *name)
{
  যদি (container->container_def->annotations == রিক্ত)
    প্রত্যাগম রিক্ত;

  প্রত্যাগম find_string_map_মান (container->annotations, name);
}

পূর্ণাঙ্ক
safe_write (পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *fname, ধ্রুব শূন্য *buf, size_t গুনতি, libcrun_error_t *err)
{
  size_t written = 0;
  যাবত্ (written < গুনতি)
    {
      ssize_t w = লিখ (fd, (ধ্রুব বর্ণ *) buf + written, গুনতি - written);
      যদি (UNLIKELY (w < 0))
        {
          যদি (errno == EINTR || errno == EAGAIN)
            অনুবর্ত;
          প্রত্যাগম crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  প্রত্যাগম 0;
}

#define MAX_PARTS 32

পূর্ণাঙ্ক
append_paths (বর্ণ **out, libcrun_error_t *err, ...)
{
  ধ্রুব বর্ণ *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t total_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  প্রতি (;;)
    {
      ধ্রুব বর্ণ *part;
      size_t size;

      part = va_arg (ap, ধ্রুব বর্ণ *);
      যদি (part == রিক্ত)
        ভঙ্গ;

      যদি (n_parts == MAX_PARTS)
        {
          va_end (ap);
          প্রত্যাগম crun_make_error (err, EINVAL, "too many paths specified");
        }

      যদি (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          যাবত্ (part[0] == '/' && part[1] == '/')
            part++;
        }
      অন্যথা
        {
          /* And drop any initial '/' for other components.  */
          যাবত্ (part[0] == '/')
            part++;
        }

      size = strlen (part);
      যদি (size == 0)
        অনুবর্ত;

      যাবত্ (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  প্রতি (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  প্রতি (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      যদি (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  প্রত্যাগম 0;
}

#যদি __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#অন্যথা
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
স্থির পূর্ণাঙ্ক
b64_প্রবেশ (বর্ণ c)
{
  ধ্রুব বর্ণ table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  পূর্ণাঙ্ক i;

  প্রতি (i = 0; i < 64; i++)
    {
      যদি (table[i] == c)
        প্রত্যাগম i;
    }
  প্রত্যাগম -1;
}

পূর্ণাঙ্ক
base64_decode (ধ্রুব বর্ণ *iptr, size_t isize, বর্ণ *optr, size_t osize, size_t *nbytes)
{
  পূর্ণাঙ্ক i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned বর্ণ data[4];

  *nbytes = 0;
  যাবত্ (consumed < isize && (*nbytes) + 3 < osize)
    {
      যাবত্ ((i < 4) && (consumed < isize))
        {
          tmp = b64_প্রবেশ (*iptr++);
          consumed++;
          যদি (tmp != -1)
            {
              যদি (UNLIKELY (pad > 0))
                প্রত্যাগম -1;
              data[i++] = tmp;
            }
          অন্যথা যদি (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      যদি (i == 4)
        {
          যদি (UNLIKELY (pad > 2))
            প্রত্যাগম -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      অন্যথা
        {
          /* I did not get all the data.  */
          consumed -= i;
          প্রত্যাগম consumed;
        }
      i = 0;
    }
  প্রত্যাগম consumed;
}

বর্ণ *
get_user_name (uid_t uid)
{
  সংরচনা passwd pd;
  সংরচনা passwd *temp_result_ptr;
  বর্ণ pwdbuffer[200];
  যদি (! getpwuid_r (uid, &pd, pwdbuffer, পরিমাণ (pwdbuffer), &temp_result_ptr))
    প্রত্যাগম xstrdup (pd.pw_name);
  প্রত্যাগম xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
বর্ণ *
libcrun_sysctl_চাবি_to_proc_path (ধ্রুব বর্ণ *চাবি)
{
  ধ্রুব বর্ণ *sep;
  বর্ণ *ret, *it;

  sep = strpbrk (চাবি, "./");

  /* No separator at all, or the key is already in the path form.  */
  যদি (sep == রিক্ত || *sep == '/')
    প্রত্যাগম xstrdup (চাবি);

  ret = xstrdup (চাবি);
  প্রতি (it = ret; *it; it++)
    {
      যদি (*it == '.')
        *it = '/';
      অন্যথা যদি (*it == '/')
        *it = '.';
    }

  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
has_suffix (ধ্রুব বর্ণ *str, ধ্রুব বর্ণ *suffix)
{
  যদি (! str || ! suffix)
    প্রত্যাগম 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  যদি (lensuffix > lenstr)
    প্রত্যাগম 0;
  প্রত্যাগম memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

বর্ণ *
str_join_array (পূর্ণাঙ্ক offset, size_t size, বর্ণ *ধ্রুব array[], ধ্রুব বর্ণ *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = রিক্ত;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  বর্ণ *result, *p;

  lens = xmalloc (size * পরিমাণ (*lens));

  প্রতি (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  প্রতি (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      যদি (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  প্রত্যাগম result;
}

পূর্ণাঙ্ক
libcrun_mmap (সংরচনা libcrun_mmap_s **ret, শূন্য *addr, size_t length,
              পূর্ণাঙ্ক prot, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক fd, off_t offset,
              libcrun_error_t *err)
{
  সংরচনা libcrun_mmap_s *mmap_s = রিক্ত;

  শূন্য *mapped = mmap (addr, length, prot, flags, fd, offset);
  যদি (mapped == MAP_FAILED)
    প্রত্যাগম crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (পরিমাণ (সংরচনা libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_munmap (সংরচনা libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = munmap (mmap->addr, mmap->length);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "munmap");

  মুক্ত (mmap);

  প্রত্যাগম 0;
}

স্থির long
read_file_as_long_or_default (ধ্রুব বর্ণ *path, long def_মান)
{
  cleanup_free বর্ণ *content = রিক্ত;
  libcrun_error_t tmp_err = রিক্ত;
  বর্ণ *endptr;
  long val;
  পূর্ণাঙ্ক ret;

  ret = read_all_file (path, &content, রিক্ত, &tmp_err);
  যদি (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      প্রত্যাগম def_মান;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  যদি (UNLIKELY (errno))
    প্রত্যাগম def_মান;
  যদি (endptr == content || (*endptr && *endptr != '\n'))
    প্রত্যাগম def_মান;
  প্রত্যাগম val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (শূন্য)
{
  স্থির atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  যদি (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  প্রত্যাগম uid;
}

gid_t
get_overflow_gid (শূন্য)
{
  স্থির atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  যদি (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  প্রত্যাগম gid;
}

শূন্য
consume_trailing_slashes (বর্ণ *path)
{
  যদি (! path || path[0] == '\0')
    প্রত্যাগম;

  বর্ণ *last = path + strlen (path);

  যাবত্ (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

বর্ণ **
read_dir_entries (ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = রিক্ত;
  size_t n_entries = 0;
  size_t entries_size = 16;
  বর্ণ **entries = রিক্ত;
  সংরচনা dirent *de;

  dir = opendir (path);
  যদি (UNLIKELY (dir == রিক্ত))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      প্রত্যাগম রিক্ত;
    }

  entries = xmalloc (entries_size * পরিমাণ (বর্ণ *));
  যাবত্ ((de = readdir (dir)))
    {
      যদি (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        অনুবর্ত;
      যদি (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * পরিমাণ (বর্ণ *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * পরিমাণ (বর্ণ *));
  entries[n_entries] = রিক্ত;

  প্রত্যাগম entries;
}

পূর্ণাঙ্ক
cpuset_string_to_bitmask (ধ্রুব বর্ণ *str, বর্ণ **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free বর্ণ *mask = রিক্ত;
  size_t mask_size = 0;
  ধ্রুব বর্ণ *p = str;
  বর্ণ *endptr;

  যাবত্ (*p)
    {
      long long start_range, end_range;

      যদি (*p < '0' || *p > '9')
        গচ্ছ invalid_প্রবেশ;

      start_range = strtoll (p, &endptr, 10);
      যদি (start_range < 0)
        গচ্ছ invalid_প্রবেশ;

      p = endptr;

      যদি (*p != '-')
        end_range = start_range;
      অন্যথা
        {
          p++;

          যদি (*p < '0' || *p > '9')
            গচ্ছ invalid_প্রবেশ;

          end_range = strtoll (p, &endptr, 10);

          যদি (end_range < start_range)
            গচ্ছ invalid_প্রবেশ;

          p = endptr;
        }

      /* Just set some limit.  */
      যদি (end_range > (1 << 20))
        গচ্ছ invalid_প্রবেশ;

      যদি (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      প্রতি (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      যদি (*p == ',')
        p++;
      অন্যথা যদি (*p)
        গচ্ছ invalid_প্রবেশ;
    }

  *out = mask;
  mask = রিক্ত;
  *out_size = mask_size;

  প্রত্যাগম 0;

invalid_প্রবেশ:
  প্রত্যাগম crun_make_error (err, 0, "cannot parse input `%s`", str);
}

সংরচনা channel_fd_pair
{
  সংরচনা ring_buffer *rb;

  পূর্ণাঙ্ক in_fd;
  পূর্ণাঙ্ক out_fd;

  পূর্ণাঙ্ক infd_epoll_events;
  পূর্ণাঙ্ক outfd_epoll_events;
};

সংরচনা channel_fd_pair *
channel_fd_pair_new (পূর্ণাঙ্ক in_fd, পূর্ণাঙ্ক out_fd, size_t size)
{
  সংরচনা channel_fd_pair *channel = xmalloc (পরিমাণ (সংরচনা channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  প্রত্যাগম channel;
}

শূন্য
channel_fd_pair_free (সংরচনা channel_fd_pair *channel)
{
  যদি (channel == রিক্ত)
    প্রত্যাগম;

  ring_buffer_free (channel->rb);
  মুক্ত (channel);
}

পূর্ণাঙ্ক
channel_fd_pair_process (সংরচনা channel_fd_pair *channel, পূর্ণাঙ্ক epollfd, libcrun_error_t *err)
{
  bool is_প্রবেশ_eagain = false, is_output_eagain = false, repeat;
  পূর্ণাঙ্ক ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  প্রতি (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      যদি (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_প্রবেশ_eagain, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          যদি (ret > 0)
            repeat = true;
        }
      যদি (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          যদি (ret > 0)
            repeat = true;
        }
    }

  যদি (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      পূর্ণাঙ্ক events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_প্রবেশ_eagain ? EPOLLET : 0)) : 0;
      যদি (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      যদি (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          channel->outfd_epoll_events = events;
        }
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
open_proc (libcrun_error_t *err)
{
  পূর্ণাঙ্ক fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close পূর্ণাঙ্ক fsfd = -1;
  পূর্ণাঙ্ক ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  যদি (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, রিক্ত, রিক্ত, 0);
      যদি (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          যদি (fd >= 0)
            প্রত্যাগম fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  যদি (fd < 0)
    প্রত্যাগম crun_make_error (err, errno, "open `/proc`");
  প্রত্যাগম fd;
}

পূর্ণাঙ্ক
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  যদি (! container)
    প্রত্যাগম open_proc (err);

  যদি (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  প্রত্যাগম container->proc_fd;
}

পূর্ণাঙ্ক
libcrun_open_proc_file (libcrun_container_t *container, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক flags, libcrun_error_t *err)
{
  পূর্ণাঙ্ক proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  যদি (proc_fd < 0)
    প্রত্যাগম proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  যদি (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  প্রত্যাগম fd;
}

পূর্ণাঙ্ক
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক flags, libcrun_error_t *err)
{
  cleanup_free বর্ণ *full_path = রিক্ত;
  xasprintf (&full_path, "%d/%s", pid, path);
  প্রত্যাগম libcrun_open_proc_file (container, full_path, flags, err);
}
