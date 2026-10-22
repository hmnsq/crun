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
#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "ring_buffer.h"
#അന്തര്ഭാവ <stdarg.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <libgen.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/un.h>
#അന്തര്ഭാവ <sys/socket.h>
#അന്തര്ഭാവ <sys/signalfd.h>
#അന്തര്ഭാവ <sys/epoll.h>
#അന്തര്ഭാവ <sys/syscall.h>
#അന്തര്ഭാവ <sys/wait.h>
#അന്തര്ഭാവ <pwd.h>
#അന്തര്ഭാവ <grp.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <sys/time.h>
#അന്തര്ഭാവ <sys/xattr.h>
#അന്തര്ഭാവ <sys/sysmacros.h>
#അന്തര്ഭാവ <sys/vfs.h>
#അന്തര്ഭാവ <linux/magic.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <sys/mman.h>
#ifdef HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#endif
#എങ്കിൽ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#അല്ലെങ്കിൽ
#  define atomic_long അസ്ഥിര long
#  define atomic_int അസ്ഥിര പൂർണ്ണസംഖ്യ
#endif
#അന്തര്ഭാവ "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_വേര്
#  define RESOLVE_IN_വേര് 0x10
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

പൂർണ്ണസംഖ്യ
crun_path_exists (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret = access (path, F_OK);
  എങ്കിൽ (ret < 0)
    {
      എങ്കിൽ (errno == ENOENT)
        തിരികെ 0;
      തിരികെ crun_make_error (err, errno, "access `%s`", path);
    }
  തിരികെ 1;
}

പൂർണ്ണസംഖ്യ
xasprintf (അക്ഷരം **str, സ്ഥിരം അക്ഷരം *fmt, ...)
{
  പൂർണ്ണസംഖ്യ ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  എങ്കിൽ (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
write_file_at_with_flags (പൂർണ്ണസംഖ്യ dirfd, പൂർണ്ണസംഖ്യ flags, mode_t mode, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം ശൂന്യം *data, size_t len, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  പൂർണ്ണസംഖ്യ ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ (len > INT_MAX) ? INT_MAX : (പൂർണ്ണസംഖ്യ) len;
}

പൂർണ്ണസംഖ്യ
detach_process ()
{
  pid_t pid;
  എങ്കിൽ (setsid () < 0)
    തിരികെ -1;
  pid = fork ();
  എങ്കിൽ (pid < 0)
    തിരികെ -1;
  എങ്കിൽ (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
get_file_type_fd (പൂർണ്ണസംഖ്യ fd, mode_t *mode)
{
  ഘടന stat st;
  പൂർണ്ണസംഖ്യ ret;

#ifdef HAVE_STATX
  ഘടന statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno == ENOSYS || errno == EINVAL)
        പോകുക fallback;

      തിരികെ ret;
    }
  *mode = stx.stx_mode;
  തിരികെ ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
get_file_type_at (പൂർണ്ണസംഖ്യ dirfd, mode_t *mode, bool nofollow, സ്ഥിരം അക്ഷരം *path)
{
  പൂർണ്ണസംഖ്യ empty_path = path == ശൂന്യ ? AT_EMPTY_PATH : 0;
  ഘടന stat st;
  പൂർണ്ണസംഖ്യ ret;

#ifdef HAVE_STATX
  ഘടന statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno == ENOSYS || errno == EINVAL)
        പോകുക fallback;

      തിരികെ ret;
    }
  *mode = stx.stx_mode;
  തിരികെ ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
get_file_type (mode_t *mode, bool nofollow, സ്ഥിരം അക്ഷരം *path)
{
  തിരികെ get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

പൂർണ്ണസംഖ്യ
create_file_if_missing_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  എങ്കിൽ (fd_write < 0)
    {
      പൂർണ്ണസംഖ്യ saved_errno = errno;
      mode_t tmp_mode;
      പൂർണ്ണസംഖ്യ ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      എങ്കിൽ (ret == 0 && S_ISREG (tmp_mode))
        തിരികെ 0;

      തിരികെ crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
ensure_directory_internal_at (പൂർണ്ണസംഖ്യ dirfd, അക്ഷരം *path, size_t len, പൂർണ്ണസംഖ്യ mode, libcrun_error_t *err)
{
  അക്ഷരം *it = path + len;
  പൂർണ്ണസംഖ്യ ret = 0;
  bool parent_created = false;

  വേണ്ടി (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      എങ്കിൽ (ret == 0 || errno == EEXIST)
        തിരികെ 0;

      പൂർണ്ണസംഖ്യ saved_errno = errno;
      എങ്കിൽ (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = ശൂന്യ;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          എങ്കിൽ (ret > 0)
            തിരികെ 0;
          എങ്കിൽ (ret < 0)
            crun_error_release (&tmp_err);

          തിരികെ crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      ആകുമ്പോൾ (it > path && *it != '/')
        {
          it--;
          len--;
        }
      എങ്കിൽ (it == path)
        തിരികെ 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      parent_created = true;
    }
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
crun_ensure_directory_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode, bool nofollow, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free അക്ഷരം *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (ret == 0)
    തിരികെ crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
check_fd_is_path (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  അക്ഷരം link[PATH_MAX];
  പൂർണ്ണസംഖ്യ ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, വലുപ്പം (link)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "readlink `%s`", fdname);

  എങ്കിൽ (((size_t) ret) != path_len || memcmp (link, path, path_len))
    തിരികെ crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
check_fd_under_path (സ്ഥിരം അക്ഷരം *rootfs, size_t rootfslen, പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  അക്ഷരം link[PATH_MAX];
  പൂർണ്ണസംഖ്യ ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  എങ്കിൽ (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    തിരികെ 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, വലുപ്പം (link)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "readlink `%s`", fdname);

  എങ്കിൽ (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    തിരികെ crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  തിരികെ 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
സ്ഥിതം ശൂന്യം
close_and_replace (പൂർണ്ണസംഖ്യ *oldfd, പൂർണ്ണസംഖ്യ newfd)
{
  എങ്കിൽ (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
അക്ഷരം *chroot_realpath (സ്ഥിരം അക്ഷരം *chroot, സ്ഥിരം അക്ഷരം *path, അക്ഷരം resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
safe_openat_fallback (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *rootfs, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ flags,
                      പൂർണ്ണസംഖ്യ mode, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *parent_path = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *last_component = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *orig_path = path;
  സ്ഥിരം അക്ഷരം *path_in_chroot;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  അക്ഷരം resolved[PATH_MAX];
  അക്ഷരം buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  പൂർണ്ണസംഖ്യ ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  എങ്കിൽ (flags & O_NOFOLLOW)
    {
      അക്ഷരം *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      എങ്കിൽ (sep == ശൂന്യ)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      അല്ലെങ്കിൽ എങ്കിൽ (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      എങ്കിൽ (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  എങ്കിൽ (path_in_chroot == ശൂന്യ)
    തിരികെ crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  എങ്കിൽ (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  എങ്കിൽ (last_component)
    {
      ret = snprintf (resolved, വലുപ്പം (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      എങ്കിൽ (UNLIKELY (ret >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (resolved)))
        തിരികെ crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  എങ്കിൽ (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      തിരികെ ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = fd;
  fd = -1;
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
safe_openat (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *rootfs, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ mode,
             libcrun_error_t *err)
{
  സ്ഥിതം bool openat2_supported = true;
  പൂർണ്ണസംഖ്യ retries = MAX_OPENAT2_EAGAIN_RETRIES;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (is_empty_string (path))
    {
      cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      എങ്കിൽ (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      എങ്കിൽ (UNLIKELY (fd < 0))
        തിരികെ crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      എങ്കിൽ (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      ret = fd;
      fd = -1;
      തിരികെ ret;
    }

  എങ്കിൽ (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_വേര്);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (errno == EINTR)
            പോകുക repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          എങ്കിൽ (errno == EAGAIN && retries-- > 0)
            പോകുക repeat;
          എങ്കിൽ (errno == ENOSYS)
            openat2_supported = false;
          എങ്കിൽ (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            തിരികെ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          തിരികെ crun_make_error (err, errno, "openat2 `%s`", path);
        }

      തിരികെ ret;
    }

  തിരികെ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (പൂർണ്ണസംഖ്യ dfd, സ്ഥിരം അക്ഷരം *name, അക്ഷരം **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free അക്ഷരം *tmp_buf = ശൂന്യ;
  ssize_t size;

  ചെയ്യുക
    {
      എങ്കിൽ (tmp_buf != ശൂന്യ)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      എങ്കിൽ (UNLIKELY (size < 0))
        തിരികെ crun_make_error (err, errno, "readlink `%s`", name);
  } ആകുമ്പോൾ (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = ശൂന്യ;

  തിരികെ size;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *dirpath,
                                സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode, പൂർണ്ണസംഖ്യ max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *dirpath, പൂർണ്ണസംഖ്യ cwd,
                                 സ്ഥിരം അക്ഷരം *npath, സ്ഥിരം അക്ഷരം *cur, സ്ഥിരം അക്ഷരം *rest,
                                 പൂർണ്ണസംഖ്യ mode, പൂർണ്ണസംഖ്യ max_readlinks, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *target = ശൂന്യ;
  cleanup_free അക്ഷരം *new_path = ശൂന്യ;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (പൂർണ്ണസംഖ്യ) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  തിരികെ crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *dirpath,
                     സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode, പൂർണ്ണസംഖ്യ max_readlinks, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ wd_cleanup = -1;
  cleanup_free അക്ഷരം *npath = ശൂന്യ;
  bool last_component = false;
  size_t depth = 0;
  അക്ഷരം *cur;
  അക്ഷരം *it;
  പൂർണ്ണസംഖ്യ cwd;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (max_readlinks <= 0)
    തിരികെ crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  എങ്കിൽ (*path == '\0')
    {
      എങ്കിൽ (do_open)
        തിരികെ open (dirpath, O_CLOEXEC | O_PATH, 0);
      തിരികെ 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  ആകുമ്പോൾ (cur)
    {
      എങ്കിൽ (it)
        *it = '\0';
      അല്ലെങ്കിൽ
        last_component = true;

      എങ്കിൽ (cur[0] == '\0')
        നിർത്തുക;

      എങ്കിൽ (strcmp (cur, ".") == 0)
        പോകുക next;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (cur, ".."))
        depth++;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (depth)
            depth--;
          അല്ലെങ്കിൽ
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              പോകുക next;
            }
        }

      എങ്കിൽ (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              എങ്കിൽ (errno == ELOOP)
                തിരികെ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, ശൂന്യ,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              എങ്കിൽ (ret < 0)
                തിരികെ crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          എങ്കിൽ (do_open)
            തിരികെ ret;

          close_and_replace (&wd_cleanup, ret);
          തിരികെ 0;
        }

      ret = mkdirat (cwd, cur, mode);
      എങ്കിൽ (ret < 0)
        {
          mode_t st_mode;

          എങ്കിൽ (errno != EEXIST)
            തിരികെ crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          എങ്കിൽ (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            തിരികെ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : ശൂന്യ, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      എങ്കിൽ (UNLIKELY (cwd < 0))
        തിരികെ crun_error_wrap (err, "creating `/%s`", path);

      എങ്കിൽ (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, ശൂന്യ);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              പൂർണ്ണസംഖ്യ saved_errno = errno;

              close (cwd);
              തിരികെ crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          എങ്കിൽ ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              തിരികെ crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      എങ്കിൽ (it == ശൂന്യ)
        നിർത്തുക;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  എങ്കിൽ (do_open)
    {
      എങ്കിൽ (cwd == dirfd)
        {
          ret = dup (dirfd);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "dup `%s`", dirpath);
          തിരികെ ret;
        }

      wd_cleanup = -1;
      തിരികെ cwd;
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
crun_safe_create_and_open_ref_at (bool dir, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *dirpath, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  എങ്കിൽ (LIKELY (ret >= 0))
    തിരികെ ret;

  crun_error_release (err);
  തിരികെ crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

പൂർണ്ണസംഖ്യ
crun_safe_ensure_directory_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *dirpath, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode,
                               libcrun_error_t *err)
{
  തിരികെ crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

പൂർണ്ണസംഖ്യ
crun_safe_ensure_file_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *dirpath, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode,
                          libcrun_error_t *err)
{
  തിരികെ crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

പൂർണ്ണസംഖ്യ
crun_ensure_directory (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode, bool nofollow, libcrun_error_t *err)
{
  തിരികെ crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_file_size (പൂർണ്ണസംഖ്യ fd, off_t *size)
{
  ഘടന stat st;
  പൂർണ്ണസംഖ്യ ret;
#ifdef HAVE_STATX
  ഘടന statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno == ENOSYS || errno == EINVAL)
        പോകുക fallback;
      തിരികെ ret;
    }
  *size = stx.stx_size;

  തിരികെ ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
crun_dir_p_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  പൂർണ്ണസംഖ്യ ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "stat `%s`", path);

  തിരികെ S_ISDIR (mode) ? 1 : 0;
}

പൂർണ്ണസംഖ്യ
crun_dir_p (സ്ഥിരം അക്ഷരം *path, bool nofollow, libcrun_error_t *err)
{
  തിരികെ crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *buffer = ശൂന്യ;
  size_t len;
  പൂർണ്ണസംഖ്യ ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          തിരികെ 0;
        }
      തിരികെ ret;
    }

  തിരികെ strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
സ്ഥിതം bool
running_in_openvz ()
{
  എങ്കിൽ (access ("/proc/vz", F_OK) < 0)
    തിരികെ false;

  തിരികെ access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

പൂർണ്ണസംഖ്യ
check_running_in_user_namespace (libcrun_error_t *err)
{
  സ്ഥിതം atomic_int run_in_userns = -1;
  ഘടന stat st;
  പൂർണ്ണസംഖ്യ ret;

  ret = run_in_userns;
  എങ്കിൽ (ret >= 0)
    തിരികെ ret;

  ret = stat ("/proc/self/ns/user", &st);
  എങ്കിൽ (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      എങ്കിൽ (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          തിരികെ run_in_userns;
        }

      എങ്കിൽ (! running_in_openvz ())
        {
          run_in_userns = 1;
          തിരികെ run_in_userns;
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (UNLIKELY (errno != ENOENT))
    തിരികെ crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  run_in_userns = ret;
  തിരികെ run_in_userns;
}

സ്ഥിതം size_t
get_page_size ()
{
  സ്ഥിതം atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  എങ്കിൽ (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  തിരികെ (size_t) pagesize;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ selinux_enabled = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ apparmor_enabled = -1;

പൂർണ്ണസംഖ്യ
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *out = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  size_t len;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (selinux_enabled >= 0)
    തിരികെ selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  തിരികെ selinux_enabled;
}

പൂർണ്ണസംഖ്യ
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  പൂർണ്ണസംഖ്യ size;
  അക്ഷരം buf[1];

  എങ്കിൽ (apparmor_enabled >= 0)
    തിരികെ apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  എങ്കിൽ (fd == -1)
    {
      എങ്കിൽ (errno == ENOENT)
        {
          apparmor_enabled = 0;
          തിരികെ 0;
        }

      തിരികെ crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (പഠ (fd, buf, വലുപ്പം (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  തിരികെ apparmor_enabled;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  എങ്കിൽ (selinux_enabled < 0)
    തിരികെ crun_make_error (err, 0, "SELinux is not initialized correctly");
  തിരികെ selinux_enabled;
}

പൂർണ്ണസംഖ്യ
add_selinux_mount_label (അക്ഷരം **retlabel, സ്ഥിരം അക്ഷരം *data, സ്ഥിരം അക്ഷരം *label, സ്ഥിരം അക്ഷരം *context_type, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_is_selinux_enabled (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (label && ret)
    {
      എങ്കിൽ (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      അല്ലെങ്കിൽ
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      തിരികെ 0;
    }
  *retlabel = xstrdup (data);
  തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
lsm_attr_path (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *lsm, സ്ഥിരം അക്ഷരം *fname, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ attr_dirfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ lsm_dirfd = -1;
  അക്ഷരം *attr_path = ശൂന്യ;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  എങ്കിൽ (UNLIKELY (attr_dirfd < 0))
    തിരികെ ശൂന്യ;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  എങ്കിൽ (lsm != ശൂന്യ)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      എങ്കിൽ (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          തിരികെ ശൂന്യ;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  തിരികെ attr_path;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
check_proc_super_magic (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  ഘടന statfs sfs;

  പൂർണ്ണസംഖ്യ ret = fstatfs (fd, &sfs);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "statfs `%s`", path);

  എങ്കിൽ (sfs.f_type != PROC_SUPER_MAGIC)
    തിരികെ crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
set_security_attr (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *lsm, സ്ഥിരം അക്ഷരം *fname, സ്ഥിരം അക്ഷരം *data, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  cleanup_free സ്ഥിരം അക്ഷരം *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

  എങ്കിൽ (UNLIKELY (attr_path == ശൂന്യ))
    തിരികെ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (ലിഖ (fd, data, strlen (data)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      എങ്കിൽ (errno == ENOENT && lsm != ശൂന്യ && strcmp (lsm, "apparmor") == 0)
        {
          സ്ഥിരം അക്ഷരം *profile = strchr (data, ' ');

          തിരികെ crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      തിരികെ crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
set_selinux_label (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *label, bool now, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_is_selinux_enabled (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (ret)
    തിരികെ set_security_attr (container, ശൂന്യ, now ? "current" : "exec", label, err);
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  എങ്കിൽ (apparmor_enabled < 0)
    തിരികെ crun_make_error (err, 0, "AppArmor is not initialized correctly");
  തിരികെ apparmor_enabled;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (വലുപ്പം (UNCONFINED) - 1)
  cleanup_free സ്ഥിരം അക്ഷരം *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  അക്ഷരം buf[UNCONFINED_LEN + 1];

  എങ്കിൽ (UNLIKELY (attr_path == ശൂന്യ))
    തിരികെ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ fd;

  എങ്കിൽ (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    തിരികെ -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (പഠ (fd, buf, വലുപ്പം (buf)));
  എങ്കിൽ (UNLIKELY (bytes_read < 0))
    തിരികെ crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  എങ്കിൽ (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    തിരികെ 1;
  എങ്കിൽ (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    തിരികെ 0;
  തിരികെ 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

പൂർണ്ണസംഖ്യ
set_apparmor_profile (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_is_apparmor_enabled (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (ret)
    {
      cleanup_free അക്ഷരം *buf = ശൂന്യ;
      ret = is_current_process_confined (container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      തിരികെ set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_all_fd_internal (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *description, അക്ഷരം **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *buf = ശൂന്യ;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (size_hint)
    allocated = size_hint;
  അല്ലെങ്കിൽ
    {
      ret = get_file_size (fd, &size);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "stat `%s`", description);

      എങ്കിൽ (max_size && (size_t) size > max_size)
        തിരികെ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  ആകുമ്പോൾ ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (പഠ (fd, buf + nread, allocated - nread));
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "read from file `%s`", description);

      എങ്കിൽ (ret == 0)
        നിർത്തുക;

      nread += ret;

      എങ്കിൽ (max_size && nread > max_size)
        തിരികെ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      എങ്കിൽ (nread == allocated)
        {
          എങ്കിൽ (size)
            നിർത്തുക;

          എങ്കിൽ (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  എങ്കിൽ (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      അക്ഷരം *tmp = realloc (buf, nread + 1);
      എങ്കിൽ (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = ശൂന്യ;
  എങ്കിൽ (len)
    *len = nread;
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
read_all_fd_with_size_hint (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *description, അക്ഷരം **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  തിരികെ read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

പൂർണ്ണസംഖ്യ
read_all_fd_with_max_size (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *description, അക്ഷരം **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  തിരികെ read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

പൂർണ്ണസംഖ്യ
read_all_file_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *path, അക്ഷരം **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", path);

  തിരികെ read_all_fd (fd, path, out, len, err);
}

പൂർണ്ണസംഖ്യ
read_all_file (സ്ഥിരം അക്ഷരം *path, അക്ഷരം **out, size_t *len, libcrun_error_t *err)
{
  എങ്കിൽ (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  തിരികെ read_all_file_at (AT_FDCWD, path, out, len, err);
}

പൂർണ്ണസംഖ്യ
get_realpath_to_file (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *path_name, അക്ഷരം **absolute_path, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  എങ്കിൽ (UNLIKELY (targetfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", path_name);
  അല്ലെങ്കിൽ
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      എങ്കിൽ (UNLIKELY (len < 0))
        തിരികെ crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
open_unix_domain_client_socket (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ dgram, libcrun_error_t *err)
{
  ഘടന sockaddr_un addr = {};
  പൂർണ്ണസംഖ്യ ret;
  proc_fd_path_t name_buf;
  cleanup_close പൂർണ്ണസംഖ്യ destfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "create UNIX socket");

  എങ്കിൽ (strlen (path) >= വലുപ്പം (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (destfd < 0))
        തിരികെ crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  എങ്കിൽ (path_len >= വലുപ്പം (addr.sun_path))
    തിരികെ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (ഘടന sockaddr *) &addr, വലുപ്പം (addr));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
open_unix_domain_socket (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ dgram, libcrun_error_t *err)
{
  ഘടന sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  പൂർണ്ണസംഖ്യ ret;
  cleanup_close പൂർണ്ണസംഖ്യ dirfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "create UNIX socket");

  എങ്കിൽ (strlen (path) >= വലുപ്പം (addr.sun_path))
    {
      cleanup_free അക്ഷരം *dpath = xstrdup (path);
      cleanup_free അക്ഷരം *bpath = xstrdup (path);
      സ്ഥിരം അക്ഷരം *parent_dir = dirname (dpath);
      സ്ഥിരം അക്ഷരം *base = basename (bpath);
      പൂർണ്ണസംഖ്യ n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (dirfd < 0))
        തിരികെ crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, വലുപ്പം (addr.sun_path), "%s/%s", name_buf, base);
      എങ്കിൽ (n < 0 || (size_t) n >= വലുപ്പം (addr.sun_path))
        തിരികെ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  അല്ലെങ്കിൽ
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (ഘടന sockaddr *) &addr, വലുപ്പം (addr));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "bind socket to `%s`", path);

  എങ്കിൽ (! dgram)
    {
      ret = listen (fd, 1);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
send_fd_to_socket (പൂർണ്ണസംഖ്യ server, പൂർണ്ണസംഖ്യ fd, libcrun_error_t *err)
{
  തിരികെ send_fd_to_socket_with_payload (server, fd, ശൂന്യ, 0, err);
}

പൂർണ്ണസംഖ്യ
send_fd_to_socket_with_payload (പൂർണ്ണസംഖ്യ server, പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *payload, size_t payload_len, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  ഘടന cmsghdr *cmsg = ശൂന്യ;
  ഘടന iovec iov[2];
  ഘടന msghdr സന്ദേശം = {};
  അക്ഷരം ctrl_buf[CMSG_SPACE (1 + വലുപ്പം (പൂർണ്ണസംഖ്യ))] = {};
  അക്ഷരം data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = വലുപ്പം (data);

  എങ്കിൽ (payload_len > 0)
    {
      iov[0].iov_base = (ശൂന്യം *) payload;
      iov[0].iov_len = payload_len;
    }

  സന്ദേശം.സന്ദേശം_name = ശൂന്യ;
  സന്ദേശം.സന്ദേശം_namelen = 0;
  സന്ദേശം.സന്ദേശം_iov = iov;
  സന്ദേശം.സന്ദേശം_iovlen = 1;
  സന്ദേശം.സന്ദേശം_controllen = CMSG_SPACE (വലുപ്പം (പൂർണ്ണസംഖ്യ));
  സന്ദേശം.സന്ദേശം_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&സന്ദേശം);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (വലുപ്പം (പൂർണ്ണസംഖ്യ));

  *((പൂർണ്ണസംഖ്യ *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &സന്ദേശം, 0));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "sendmsg");
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
receive_fd_from_socket_with_payload (പൂർണ്ണസംഖ്യ from, അക്ഷരം *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  പൂർണ്ണസംഖ്യ ret;
  ഘടന iovec iov[1];
  ഘടന msghdr സന്ദേശം = {};
  അക്ഷരം ctrl_buf[CMSG_SPACE (വലുപ്പം (പൂർണ്ണസംഖ്യ))] = {};
  അക്ഷരം data[1];
  ഘടന cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = വലുപ്പം (data);

  എങ്കിൽ (payload_len > 0)
    {
      iov[0].iov_base = (ശൂന്യം *) payload;
      iov[0].iov_len = payload_len;
    }

  സന്ദേശം.സന്ദേശം_name = ശൂന്യ;
  സന്ദേശം.സന്ദേശം_namelen = 0;
  സന്ദേശം.സന്ദേശം_iov = iov;
  സന്ദേശം.സന്ദേശം_iovlen = 1;
  സന്ദേശം.സന്ദേശം_controllen = CMSG_SPACE (വലുപ്പം (പൂർണ്ണസംഖ്യ));
  സന്ദേശം.സന്ദേശം_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &സന്ദേശം, 0));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "recvmsg");
  എങ്കിൽ (UNLIKELY (ret == 0))
    തിരികെ crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&സന്ദേശം);
  എങ്കിൽ (cmsg == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), വലുപ്പം (fd));

  ret = fd;
  fd = -1;
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
receive_fd_from_socket (പൂർണ്ണസംഖ്യ from, libcrun_error_t *err)
{
  തിരികെ receive_fd_from_socket_with_payload (from, ശൂന്യ, 0, err);
}

പൂർണ്ണസംഖ്യ
create_socket_pair (പൂർണ്ണസംഖ്യ *pair, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "socketpair");
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret = signalfd (-1, mask, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "signalfd");
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
epoll_helper_toggle (പൂർണ്ണസംഖ്യ epollfd, പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ events, libcrun_error_t *err)
{
  ഘടന epoll_event ev = {};
  bool add = events != 0;
  പൂർണ്ണസംഖ്യ ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  എങ്കിൽ (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      എങ്കിൽ (errno == ENOENT || errno == EPERM)
        തിരികെ 0;
      തിരികെ crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
epoll_helper (പൂർണ്ണസംഖ്യ *in_fds, പൂർണ്ണസംഖ്യ *in_levelfds, പൂർണ്ണസംഖ്യ *out_fds, പൂർണ്ണസംഖ്യ *out_levelfds, libcrun_error_t *err)
{
  ഘടന epoll_event ev;
  cleanup_close പൂർണ്ണസംഖ്യ epollfd = -1;
  പൂർണ്ണസംഖ്യ ret;

  പൂർണ്ണസംഖ്യ *it;
  epollfd = epoll_create1 (0);
  എങ്കിൽ (UNLIKELY (epollfd < 0))
    തിരികെ crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  വേണ്ടി (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      എങ്കിൽ (UNLIKELY (ret < 0))                                           \
        തിരികെ crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  എങ്കിൽ (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  എങ്കിൽ (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  എങ്കിൽ (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  എങ്കിൽ (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
copy_from_fd_to_fd (പൂർണ്ണസംഖ്യ src, പൂർണ്ണസംഖ്യ dst, പൂർണ്ണസംഖ്യ consume, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  ssize_t nread;
  size_t pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  ചെയ്യുക
    {
      cleanup_free അക്ഷരം *buffer = ശൂന്യ;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      എങ്കിൽ (can_copy_file_range)
        {
          nread = copy_file_range (src, ശൂന്യ, dst, ശൂന്യ, pagesize, 0);
          എങ്കിൽ (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              പോകുക fallback;
            }
          എങ്കിൽ (consume && nread < 0 && errno == EAGAIN)
            തിരികെ 0;
          എങ്കിൽ (nread < 0 && errno == EIO)
            തിരികെ 0;
          എങ്കിൽ (UNLIKELY (nread < 0))
            തിരികെ crun_make_error (err, errno, "copy_file_range");
          തുടരുക;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (പഠ (src, buffer, pagesize));
      എങ്കിൽ (consume && nread < 0 && errno == EAGAIN)
        തിരികെ 0;
      എങ്കിൽ (nread < 0 && errno == EIO)
        തിരികെ 0;
      എങ്കിൽ (UNLIKELY (nread < 0))
        തിരികെ crun_make_error (err, errno, "read");

      remaining = nread;
      ആകുമ്പോൾ (remaining)
        {
          ret = TEMP_FAILURE_RETRY (ലിഖ (dst, buffer + nread - remaining, remaining));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } ആകുമ്പോൾ (consume && nread);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
run_process (അക്ഷരം **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  എങ്കിൽ (UNLIKELY (pid < 0))
    തിരികെ crun_make_error (err, errno, "fork");
  എങ്കിൽ (pid)
    {
      പൂർണ്ണസംഖ്യ r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      എങ്കിൽ (r < 0)
        തിരികെ crun_make_error (err, errno, "waitpid");
      എങ്കിൽ (WIFEXITED (status) || WIFSIGNALED (status))
        തിരികെ WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
സ്ഥിതം unsigned
atou (അക്ഷരം **s)
{
  unsigned x;
  വേണ്ടി (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  തിരികെ x;
}

പൂർണ്ണസംഖ്യ
fgetpwent_r (FILE *f, ഘടന passwd *pw, അക്ഷരം *വരി, size_t size, ഘടന passwd **res)
{
  അക്ഷരം *s;
  പൂർണ്ണസംഖ്യ rv = 0;
  വേണ്ടി (;;)
    {
      വരി[size - 1] = '\xff';
      എങ്കിൽ ((fgets (വരി, size, f) == ശൂന്യ) || ferror (f) || വരി[size - 1] != '\xff')
        {
          rv = (വരി[size - 1] != '\xff') ? ERANGE : ENOENT;
          വരി = 0;
          pw = 0;
          നിർത്തുക;
        }
      വരി[strcspn (വരി, "\n")] = 0;

      s = വരി;
      pw->pw_name = s++;
      എങ്കിൽ (! (s = strchr (s, ':')))
        തുടരുക;

      *s++ = 0;
      pw->pw_passwd = s;
      എങ്കിൽ (! (s = strchr (s, ':')))
        തുടരുക;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      എങ്കിൽ (*s != ':')
        തുടരുക;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      എങ്കിൽ (*s != ':')
        തുടരുക;

      *s++ = 0;
      pw->pw_gecos = s;
      എങ്കിൽ (! (s = strchr (s, ':')))
        തുടരുക;

      *s++ = 0;
      pw->pw_dir = s;
      എങ്കിൽ (! (s = strchr (s, ':')))
        തുടരുക;

      *s++ = 0;
      pw->pw_shell = s;
      നിർത്തുക;
    }
  *res = pw;
  എങ്കിൽ (rv)
    errno = rv;
  തിരികെ rv;
}
#endif

പൂർണ്ണസംഖ്യ
set_home_env (uid_t id)
{
  ഘടന passwd pwd;
  cleanup_free അക്ഷരം *buf = ശൂന്യ;
  long buf_size;
  cleanup_file FILE *stream = ശൂന്യ;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  എങ്കിൽ (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  എങ്കിൽ (stream == ശൂന്യ)
    തിരികെ -1;

  വേണ്ടി (;;)
    {
      ഘടന passwd *ret_pw = ശൂന്യ;

      പൂർണ്ണസംഖ്യ ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      എങ്കിൽ (UNLIKELY (ret != 0))
        {
          എങ്കിൽ (ret == EINTR)
            തുടരുക;

          എങ്കിൽ (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              തിരികെ -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          തുടരുക;
        }

      എങ്കിൽ (ret_pw && ret_pw->pw_uid == id)
        {
          എങ്കിൽ (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          അല്ലെങ്കിൽ
            തിരികെ 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
സ്ഥിതം പൂർണ്ണസംഖ്യ
getsubidrange (uid_t id, പൂർണ്ണസംഖ്യ is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *നിവേശം = ശൂന്യ;
  cleanup_free അക്ഷരം *lineptr = ശൂന്യ;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free അക്ഷരം *buf = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *name;
  ഘടന passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  എങ്കിൽ (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  വേണ്ടി (;;)
    {
      പൂർണ്ണസംഖ്യ ret;
      ഘടന passwd *ret_pw = ശൂന്യ;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      എങ്കിൽ (LIKELY (ret == 0))
        {
          എങ്കിൽ (ret_pw)
            {
              name = ret_pw->pw_name;
              നിർത്തുക;
            }
          തിരികെ -1;
        }

      എങ്കിൽ (ret != ERANGE)
        തിരികെ -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  നിവേശം = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  എങ്കിൽ (നിവേശം == ശൂന്യ)
    തിരികെ -1;

  വേണ്ടി (;;)
    {
      അക്ഷരം *endptr;
      ssize_t പഠ = getline (&lineptr, &lenlineptr, നിവേശം);
      എങ്കിൽ (പഠ < 0)
        തിരികെ -1;

      എങ്കിൽ (പഠ < (ssize_t) (len_name + 2))
        തുടരുക;

      എങ്കിൽ (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        തുടരുക;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      എങ്കിൽ (endptr >= &lineptr[പഠ])
        തിരികെ -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      തിരികെ 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

പൂർണ്ണസംഖ്യ
format_default_id_mapping (അക്ഷരം **out, uid_t container_id, uid_t host_uid, uid_t host_id, പൂർണ്ണസംഖ്യ is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free അക്ഷരം *buffer = ശൂന്യ;
  പൂർണ്ണസംഖ്യ written = 0;
  പൂർണ്ണസംഖ്യ ret, remaining;

  *out = ശൂന്യ;

  എങ്കിൽ (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    തിരികെ 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  എങ്കിൽ (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      എങ്കിൽ (UNLIKELY (ret >= remaining))
        തിരികെ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  എങ്കിൽ (UNLIKELY (ret >= remaining))
    തിരികെ crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  എങ്കിൽ (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      എങ്കിൽ (UNLIKELY (ret >= remaining))
        തിരികെ crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = ശൂന്യ;
  തിരികെ written;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
unset_cloexec_flag (പൂർണ്ണസംഖ്യ fd)
{
  പൂർണ്ണസംഖ്യ flags = fcntl (fd, F_GETFD);
  എങ്കിൽ (flags == -1)
    തിരികെ -1;

  flags &= ~FD_CLOEXEC;

  തിരികെ fcntl (fd, F_SETFD, flags);
}

സ്ഥിതം ശൂന്യം __attribute__ ((__noreturn__))
run_process_child (അക്ഷരം *path, അക്ഷരം **args, സ്ഥിരം അക്ഷരം *cwd, അക്ഷരം **envp, പൂർണ്ണസംഖ്യ pipe_r,
                   പൂർണ്ണസംഖ്യ pipe_w, പൂർണ്ണസംഖ്യ out_fd, പൂർണ്ണസംഖ്യ err_fd, bool can_ignore_chdir_errors)
{
  അക്ഷരം *tmp_args[] = { path, ശൂന്യ };
  libcrun_error_t err = ശൂന്യ;
  പൂർണ്ണസംഖ്യ dev_null_fd = -1;
  പൂർണ്ണസംഖ്യ ret;

  ret = mark_or_close_fds_ge_than (ശൂന്യ, 3, false, &err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->സന്ദേശം);

  എങ്കിൽ (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  എങ്കിൽ (out_fd >= 0)
    unset_cloexec_flag (1);
  എങ്കിൽ (err_fd >= 0)
    unset_cloexec_flag (2);

  എങ്കിൽ (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  എങ്കിൽ (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  എങ്കിൽ (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  എങ്കിൽ (args == ശൂന്യ)
    args = tmp_args;

  എങ്കിൽ (cwd && chdir (cwd) < 0)
    {
      എങ്കിൽ (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
പൂർണ്ണസംഖ്യ
run_process_with_stdin_timeout_envp (അക്ഷരം *path, അക്ഷരം **args, സ്ഥിരം അക്ഷരം *cwd, പൂർണ്ണസംഖ്യ timeout,
                                     അക്ഷരം **envp, അക്ഷരം *stdin, size_t stdin_len, പൂർണ്ണസംഖ്യ out_fd,
                                     പൂർണ്ണസംഖ്യ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ stdin_pipe[2];
  pid_t pid;
  പൂർണ്ണസംഖ്യ ret;
  cleanup_close പൂർണ്ണസംഖ്യ pipe_r = -1;
  cleanup_close പൂർണ്ണസംഖ്യ pipe_w = -1;
  sigset_t oldmask, mask;
  പൂർണ്ണസംഖ്യ r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  എങ്കിൽ (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  എങ്കിൽ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      പോകുക restore_sig_mask_and_exit;
    }

  എങ്കിൽ (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (ലിഖ (pipe_w, stdin, stdin_len));
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      എങ്കിൽ (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          പോകുക restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  എങ്കിൽ (timeout)
    {
      time_t start = time (ശൂന്യ);
      time_t now;
      വേണ്ടി (now = start; now - start < timeout; now = time (ശൂന്യ))
        {
          siginfo_t info;
          പൂർണ്ണസംഖ്യ elapsed = now - start;
          ഘടന timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          എങ്കിൽ (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              പോകുക restore_sig_mask_and_exit;
            }

          എങ്കിൽ (info.si_signo == SIGCHLD && info.si_pid == pid)
            പോകുക read_waitpid;

          എങ്കിൽ (ret < 0 && errno == EAGAIN)
            പോകുക timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      പോകുക restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  എങ്കിൽ (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  അല്ലെങ്കിൽ
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  എങ്കിൽ (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      എങ്കിൽ (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
mark_or_close_fds_ge_than (libcrun_container_t *container, പൂർണ്ണസംഖ്യ n, bool close_now, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ cfd = -1;
  cleanup_dir DIR *dir = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ fd;
  ഘടന dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  എങ്കിൽ (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  എങ്കിൽ (ret == 0)
    {
      എങ്കിൽ (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      തിരികെ 0;
    }
  എങ്കിൽ (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    തിരികെ crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  എങ്കിൽ (UNLIKELY (cfd < 0))
    തിരികെ cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  dir = fdopendir (cfd);
  എങ്കിൽ (UNLIKELY (dir == ശൂന്യ))
    തിരികെ crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  വേണ്ടി (next = readdir (dir); next; next = readdir (dir))
    {
      പൂർണ്ണസംഖ്യ val;
      സ്ഥിരം അക്ഷരം *name = next->d_name;
      എങ്കിൽ (name[0] == '.')
        തുടരുക;

      val = strtoll (name, ശൂന്യ, 10);
      എങ്കിൽ (val < n || val == fd)
        തുടരുക;

      എങ്കിൽ (close_now)
        {
          ret = close (val);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "close fd `%d`", val);

          എങ്കിൽ (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      അല്ലെങ്കിൽ
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  തിരികെ 0;
}

ശൂന്യം
get_current_timestamp (അക്ഷരം *out, size_t len)
{
  ഘടന timeval tv;
  ഘടന tm now;
  അക്ഷരം timestamp[64];

  gettimeofday (&tv, ശൂന്യ);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, വലുപ്പം (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (ശൂന്യം) snprintf (out, len, "%s.%06lldZ", timestamp, (long long പൂർണ്ണസംഖ്യ) tv.tv_usec);
  out[len - 1] = '\0';
}

പൂർണ്ണസംഖ്യ
set_blocking_fd (പൂർണ്ണസംഖ്യ fd, bool blocking, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret, flags = fcntl (fd, F_GETFL, 0);
  എങ്കിൽ (UNLIKELY (flags < 0))
    തിരികെ crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fcntl");
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
parse_json_file (json_object **out, സ്ഥിരം അക്ഷരം *jsondata, ഘടന parser_context *ctx arg_unused, libcrun_error_t *err)
{
  ഘടന json_tokener *tok;
  എണ്ണൽ json_tokener_error jerr;

  *err = ശൂന്യ;

  എങ്കിൽ (UNLIKELY (jsondata == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  എങ്കിൽ (UNLIKELY (tok == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (പൂർണ്ണസംഖ്യ) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  എങ്കിൽ (*out == ശൂന്യ || jerr != json_tokener_success)
    {
      പൂർണ്ണസംഖ്യ saved_pos = (പൂർണ്ണസംഖ്യ) tok->char_offset;

      json_tokener_free (tok);
      എങ്കിൽ (*out)
        {
          json_object_put (*out);
          *out = ശൂന്യ;
        }
      തിരികെ crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  തിരികെ 0;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
check_access (സ്ഥിരം അക്ഷരം *path)
{
  പൂർണ്ണസംഖ്യ ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#അല്ലെങ്കിൽ
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  എങ്കിൽ (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      എങ്കിൽ (errno == EACCES)
        {
          പൂർണ്ണസംഖ്യ saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          എങ്കിൽ (ret == 0)
            തിരികെ CHECK_ACCESS_NOT_EXECUTABLE;
        }
      തിരികെ -errno;
    }

  ret = get_file_type (&mode, false, path);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ -errno;

  എങ്കിൽ (! S_ISREG (mode))
    തിരികെ CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
find_executable (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *executable_path, സ്ഥിരം അക്ഷരം *cwd, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cwd_executable_path = ശൂന്യ;
  cleanup_free അക്ഷരം *tmp = ശൂന്യ;
  പൂർണ്ണസംഖ്യ last_error = -ENOENT;
  അക്ഷരം *it, *end;
  പൂർണ്ണസംഖ്യ ret;

  *out = ശൂന്യ;

  എങ്കിൽ (executable_path == ശൂന്യ || executable_path[0] == '\0')
    തിരികെ crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  എങ്കിൽ (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free അക്ഷരം *cwd_allocated = ശൂന്യ;

      എങ്കിൽ (cwd == ശൂന്യ)
        {
          cwd_allocated = getcwd (ശൂന്യ, 0);
          എങ്കിൽ (cwd_allocated == ശൂന്യ)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  എങ്കിൽ (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      എങ്കിൽ (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          തിരികെ 0;
        }
      last_error = ret;
      പോകുക fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  ആകുമ്പോൾ ((it = strsep (&end, ":")))
    {
      cleanup_free അക്ഷരം *path = ശൂന്യ;

      എങ്കിൽ (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          തുടരുക;
        }

      ret = check_access (path);
      എങ്കിൽ (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = ശൂന്യ;
          തിരികെ 0;
        }

      എങ്കിൽ (ret == -ENOENT)
        തുടരുക;

      last_error = ret;
    }

fail:
  തിരഞ്ഞെടുപ്പ് (last_error)
    {
    അവസ്ഥ CHECK_ACCESS_NOT_EXECUTABLE:
      തിരികെ crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    അവസ്ഥ CHECK_ACCESS_NOT_REGULAR:
      തിരികെ crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    സ്ഥിരസ്ഥിതി:
      errno = -last_error;
      എങ്കിൽ (errno == ENOENT)
        തിരികെ crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      തിരികെ crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

സ്ഥിതം ssize_t
safe_read_xattr (അക്ഷരം **ret, പൂർണ്ണസംഖ്യ sfd, സ്ഥിരം അക്ഷരം *srcname, സ്ഥിരം അക്ഷരം *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *buffer = ശൂന്യ;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  ആകുമ്പോൾ (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      എങ്കിൽ (UNLIKELY (s < 0))
        തിരികെ crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      എങ്കിൽ (s < current_size)
        നിർത്തുക;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  എങ്കിൽ (s <= 0)
    തിരികെ s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = ശൂന്യ;

  തിരികെ s;
}

സ്ഥിതം ssize_t
copy_xattr (പൂർണ്ണസംഖ്യ sfd, പൂർണ്ണസംഖ്യ dfd, സ്ഥിരം അക്ഷരം *srcname, സ്ഥിരം അക്ഷരം *destname, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *buf = ശൂന്യ;
  ssize_t xattr_len;
  അക്ഷരം *it;

  xattr_len = flistxattr (sfd, ശൂന്യ, 0);
  എങ്കിൽ (UNLIKELY (xattr_len < 0))
    {
      എങ്കിൽ (errno == ENOTSUP)
        തിരികെ 0;

      തിരികെ crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  എങ്കിൽ (xattr_len == 0)
    തിരികെ 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  എങ്കിൽ (UNLIKELY (xattr_len < 0))
    തിരികെ crun_make_error (err, errno, "flistxattr `%s`", srcname);

  വേണ്ടി (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free അക്ഷരം *v = ശൂന്യ;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      എങ്കിൽ (UNLIKELY (s < 0))
        തിരികെ s;

      s = fsetxattr (dfd, it, v, s, 0);
      എങ്കിൽ (UNLIKELY (s < 0))
        {
          എങ്കിൽ (errno == EINVAL || errno == EOPNOTSUPP)
            തുടരുക;

          തിരികെ crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  തിരികെ 0;
}

#endif

സ്ഥിതം പൂർണ്ണസംഖ്യ
copy_rec_stat_file_at (പൂർണ്ണസംഖ്യ dfd, സ്ഥിരം അക്ഷരം *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  ഘടന stat st;
  പൂർണ്ണസംഖ്യ ret;

#ifdef HAVE_STATX
  ഘടന statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno == ENOSYS || errno == EINVAL)
        പോകുക fallback;

      തിരികെ ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  തിരികെ ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
copy_recursive_fd_to_fd (പൂർണ്ണസംഖ്യ srcdirfd, പൂർണ്ണസംഖ്യ dfd, സ്ഥിരം അക്ഷരം *srcname, സ്ഥിരം അക്ഷരം *destname, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = ശൂന്യ;
  ഘടന dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  എങ്കിൽ (UNLIKELY (dsrcfd == ശൂന്യ))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      തിരികെ crun_make_error (err, errno, "open directory `%s`", destname);
    }

  വേണ്ടി (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close പൂർണ്ണസംഖ്യ srcfd = -1;
      cleanup_close പൂർണ്ണസംഖ്യ destfd = -1;
      cleanup_free അക്ഷരം *target_buf = ശൂന്യ;
      പൂർണ്ണസംഖ്യ ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      എങ്കിൽ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        തുടരുക;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      തിരഞ്ഞെടുപ്പ് (mode & S_IFMT)
        {
        അവസ്ഥ S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          എങ്കിൽ (UNLIKELY (srcfd < 0))
            തിരികെ crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          എങ്കിൽ (UNLIKELY (destfd < 0))
            തിരികെ crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

#ifdef HAVE_FGETXATTR
          ret = (പൂർണ്ണസംഖ്യ) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          നിർത്തുക;

        അവസ്ഥ S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          എങ്കിൽ (UNLIKELY (srcfd < 0))
            തിരികെ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          എങ്കിൽ (UNLIKELY (destfd < 0))
            തിരികെ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (പൂർണ്ണസംഖ്യ) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          നിർത്തുക;

        അവസ്ഥ S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          നിർത്തുക;

        അവസ്ഥ S_IFBLK:
        അവസ്ഥ S_IFCHR:
        അവസ്ഥ S_IFIFO:
        അവസ്ഥ S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          നിർത്തുക;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          എങ്കിൽ (errno == ENOTSUP)
            തുടരുക;

          തിരികെ crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  തിരികെ 0;
}

സ്ഥിരം അക്ഷരം *
find_annotation (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *name)
{
  എങ്കിൽ (container->container_def->annotations == ശൂന്യ)
    തിരികെ ശൂന്യ;

  തിരികെ find_string_map_മൂല്യം (container->annotations, name);
}

പൂർണ്ണസംഖ്യ
safe_write (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *fname, സ്ഥിരം ശൂന്യം *buf, size_t എണ്ണം, libcrun_error_t *err)
{
  size_t written = 0;
  ആകുമ്പോൾ (written < എണ്ണം)
    {
      ssize_t w = ലിഖ (fd, (സ്ഥിരം അക്ഷരം *) buf + written, എണ്ണം - written);
      എങ്കിൽ (UNLIKELY (w < 0))
        {
          എങ്കിൽ (errno == EINTR || errno == EAGAIN)
            തുടരുക;
          തിരികെ crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  തിരികെ 0;
}

#define MAX_PARTS 32

പൂർണ്ണസംഖ്യ
append_paths (അക്ഷരം **out, libcrun_error_t *err, ...)
{
  സ്ഥിരം അക്ഷരം *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t total_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  വേണ്ടി (;;)
    {
      സ്ഥിരം അക്ഷരം *part;
      size_t size;

      part = va_arg (ap, സ്ഥിരം അക്ഷരം *);
      എങ്കിൽ (part == ശൂന്യ)
        നിർത്തുക;

      എങ്കിൽ (n_parts == MAX_PARTS)
        {
          va_end (ap);
          തിരികെ crun_make_error (err, EINVAL, "too many paths specified");
        }

      എങ്കിൽ (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          ആകുമ്പോൾ (part[0] == '/' && part[1] == '/')
            part++;
        }
      അല്ലെങ്കിൽ
        {
          /* And drop any initial '/' for other components.  */
          ആകുമ്പോൾ (part[0] == '/')
            part++;
        }

      size = strlen (part);
      എങ്കിൽ (size == 0)
        തുടരുക;

      ആകുമ്പോൾ (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  വേണ്ടി (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  വേണ്ടി (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      എങ്കിൽ (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  തിരികെ 0;
}

#എങ്കിൽ __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#അല്ലെങ്കിൽ
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
b64_നിവേശം (അക്ഷരം c)
{
  സ്ഥിരം അക്ഷരം table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  പൂർണ്ണസംഖ്യ i;

  വേണ്ടി (i = 0; i < 64; i++)
    {
      എങ്കിൽ (table[i] == c)
        തിരികെ i;
    }
  തിരികെ -1;
}

പൂർണ്ണസംഖ്യ
base64_decode (സ്ഥിരം അക്ഷരം *iptr, size_t isize, അക്ഷരം *optr, size_t osize, size_t *nbytes)
{
  പൂർണ്ണസംഖ്യ i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned അക്ഷരം data[4];

  *nbytes = 0;
  ആകുമ്പോൾ (consumed < isize && (*nbytes) + 3 < osize)
    {
      ആകുമ്പോൾ ((i < 4) && (consumed < isize))
        {
          tmp = b64_നിവേശം (*iptr++);
          consumed++;
          എങ്കിൽ (tmp != -1)
            {
              എങ്കിൽ (UNLIKELY (pad > 0))
                തിരികെ -1;
              data[i++] = tmp;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      എങ്കിൽ (i == 4)
        {
          എങ്കിൽ (UNLIKELY (pad > 2))
            തിരികെ -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      അല്ലെങ്കിൽ
        {
          /* I did not get all the data.  */
          consumed -= i;
          തിരികെ consumed;
        }
      i = 0;
    }
  തിരികെ consumed;
}

അക്ഷരം *
get_user_name (uid_t uid)
{
  ഘടന passwd pd;
  ഘടന passwd *temp_result_ptr;
  അക്ഷരം pwdbuffer[200];
  എങ്കിൽ (! getpwuid_r (uid, &pd, pwdbuffer, വലുപ്പം (pwdbuffer), &temp_result_ptr))
    തിരികെ xstrdup (pd.pw_name);
  തിരികെ xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
അക്ഷരം *
libcrun_sysctl_താക്കോൽ_to_proc_path (സ്ഥിരം അക്ഷരം *താക്കോൽ)
{
  സ്ഥിരം അക്ഷരം *sep;
  അക്ഷരം *ret, *it;

  sep = strpbrk (താക്കോൽ, "./");

  /* No separator at all, or the key is already in the path form.  */
  എങ്കിൽ (sep == ശൂന്യ || *sep == '/')
    തിരികെ xstrdup (താക്കോൽ);

  ret = xstrdup (താക്കോൽ);
  വേണ്ടി (it = ret; *it; it++)
    {
      എങ്കിൽ (*it == '.')
        *it = '/';
      അല്ലെങ്കിൽ എങ്കിൽ (*it == '/')
        *it = '.';
    }

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
has_suffix (സ്ഥിരം അക്ഷരം *str, സ്ഥിരം അക്ഷരം *suffix)
{
  എങ്കിൽ (! str || ! suffix)
    തിരികെ 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  എങ്കിൽ (lensuffix > lenstr)
    തിരികെ 0;
  തിരികെ memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

അക്ഷരം *
str_join_array (പൂർണ്ണസംഖ്യ offset, size_t size, അക്ഷരം *സ്ഥിരം array[], സ്ഥിരം അക്ഷരം *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = ശൂന്യ;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  അക്ഷരം *result, *p;

  lens = xmalloc (size * വലുപ്പം (*lens));

  വേണ്ടി (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  വേണ്ടി (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      എങ്കിൽ (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  തിരികെ result;
}

പൂർണ്ണസംഖ്യ
libcrun_mmap (ഘടന libcrun_mmap_s **ret, ശൂന്യം *addr, size_t length,
              പൂർണ്ണസംഖ്യ prot, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ fd, off_t offset,
              libcrun_error_t *err)
{
  ഘടന libcrun_mmap_s *mmap_s = ശൂന്യ;

  ശൂന്യം *mapped = mmap (addr, length, prot, flags, fd, offset);
  എങ്കിൽ (mapped == MAP_FAILED)
    തിരികെ crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (വലുപ്പം (ഘടന libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_munmap (ഘടന libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = munmap (mmap->addr, mmap->length);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "munmap");

  സ്വതന്ത്രം (mmap);

  തിരികെ 0;
}

സ്ഥിതം long
read_file_as_long_or_default (സ്ഥിരം അക്ഷരം *path, long def_മൂല്യം)
{
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  libcrun_error_t tmp_err = ശൂന്യ;
  അക്ഷരം *endptr;
  long val;
  പൂർണ്ണസംഖ്യ ret;

  ret = read_all_file (path, &content, ശൂന്യ, &tmp_err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      തിരികെ def_മൂല്യം;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  എങ്കിൽ (UNLIKELY (errno))
    തിരികെ def_മൂല്യം;
  എങ്കിൽ (endptr == content || (*endptr && *endptr != '\n'))
    തിരികെ def_മൂല്യം;
  തിരികെ val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (ശൂന്യം)
{
  സ്ഥിതം atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  എങ്കിൽ (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  തിരികെ uid;
}

gid_t
get_overflow_gid (ശൂന്യം)
{
  സ്ഥിതം atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  എങ്കിൽ (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  തിരികെ gid;
}

ശൂന്യം
consume_trailing_slashes (അക്ഷരം *path)
{
  എങ്കിൽ (! path || path[0] == '\0')
    തിരികെ;

  അക്ഷരം *last = path + strlen (path);

  ആകുമ്പോൾ (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

അക്ഷരം **
read_dir_entries (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = ശൂന്യ;
  size_t n_entries = 0;
  size_t entries_size = 16;
  അക്ഷരം **entries = ശൂന്യ;
  ഘടന dirent *de;

  dir = opendir (path);
  എങ്കിൽ (UNLIKELY (dir == ശൂന്യ))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      തിരികെ ശൂന്യ;
    }

  entries = xmalloc (entries_size * വലുപ്പം (അക്ഷരം *));
  ആകുമ്പോൾ ((de = readdir (dir)))
    {
      എങ്കിൽ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        തുടരുക;
      എങ്കിൽ (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * വലുപ്പം (അക്ഷരം *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * വലുപ്പം (അക്ഷരം *));
  entries[n_entries] = ശൂന്യ;

  തിരികെ entries;
}

പൂർണ്ണസംഖ്യ
cpuset_string_to_bitmask (സ്ഥിരം അക്ഷരം *str, അക്ഷരം **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *mask = ശൂന്യ;
  size_t mask_size = 0;
  സ്ഥിരം അക്ഷരം *p = str;
  അക്ഷരം *endptr;

  ആകുമ്പോൾ (*p)
    {
      long long start_range, end_range;

      എങ്കിൽ (*p < '0' || *p > '9')
        പോകുക invalid_നിവേശം;

      start_range = strtoll (p, &endptr, 10);
      എങ്കിൽ (start_range < 0)
        പോകുക invalid_നിവേശം;

      p = endptr;

      എങ്കിൽ (*p != '-')
        end_range = start_range;
      അല്ലെങ്കിൽ
        {
          p++;

          എങ്കിൽ (*p < '0' || *p > '9')
            പോകുക invalid_നിവേശം;

          end_range = strtoll (p, &endptr, 10);

          എങ്കിൽ (end_range < start_range)
            പോകുക invalid_നിവേശം;

          p = endptr;
        }

      /* Just set some limit.  */
      എങ്കിൽ (end_range > (1 << 20))
        പോകുക invalid_നിവേശം;

      എങ്കിൽ (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      വേണ്ടി (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      എങ്കിൽ (*p == ',')
        p++;
      അല്ലെങ്കിൽ എങ്കിൽ (*p)
        പോകുക invalid_നിവേശം;
    }

  *out = mask;
  mask = ശൂന്യ;
  *out_size = mask_size;

  തിരികെ 0;

invalid_നിവേശം:
  തിരികെ crun_make_error (err, 0, "cannot parse input `%s`", str);
}

ഘടന channel_fd_pair
{
  ഘടന ring_buffer *rb;

  പൂർണ്ണസംഖ്യ in_fd;
  പൂർണ്ണസംഖ്യ out_fd;

  പൂർണ്ണസംഖ്യ infd_epoll_events;
  പൂർണ്ണസംഖ്യ outfd_epoll_events;
};

ഘടന channel_fd_pair *
channel_fd_pair_new (പൂർണ്ണസംഖ്യ in_fd, പൂർണ്ണസംഖ്യ out_fd, size_t size)
{
  ഘടന channel_fd_pair *channel = xmalloc (വലുപ്പം (ഘടന channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  തിരികെ channel;
}

ശൂന്യം
channel_fd_pair_free (ഘടന channel_fd_pair *channel)
{
  എങ്കിൽ (channel == ശൂന്യ)
    തിരികെ;

  ring_buffer_free (channel->rb);
  സ്വതന്ത്രം (channel);
}

പൂർണ്ണസംഖ്യ
channel_fd_pair_process (ഘടന channel_fd_pair *channel, പൂർണ്ണസംഖ്യ epollfd, libcrun_error_t *err)
{
  bool is_നിവേശം_eagain = false, is_output_eagain = false, repeat;
  പൂർണ്ണസംഖ്യ ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  വേണ്ടി (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      എങ്കിൽ (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_നിവേശം_eagain, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          എങ്കിൽ (ret > 0)
            repeat = true;
        }
      എങ്കിൽ (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          എങ്കിൽ (ret > 0)
            repeat = true;
        }
    }

  എങ്കിൽ (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      പൂർണ്ണസംഖ്യ events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_നിവേശം_eagain ? EPOLLET : 0)) : 0;
      എങ്കിൽ (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      എങ്കിൽ (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          channel->outfd_epoll_events = events;
        }
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
open_proc (libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close പൂർണ്ണസംഖ്യ fsfd = -1;
  പൂർണ്ണസംഖ്യ ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  എങ്കിൽ (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ശൂന്യ, ശൂന്യ, 0);
      എങ്കിൽ (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          എങ്കിൽ (fd >= 0)
            തിരികെ fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  എങ്കിൽ (fd < 0)
    തിരികെ crun_make_error (err, errno, "open `/proc`");
  തിരികെ fd;
}

പൂർണ്ണസംഖ്യ
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  എങ്കിൽ (! container)
    തിരികെ open_proc (err);

  എങ്കിൽ (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  തിരികെ container->proc_fd;
}

പൂർണ്ണസംഖ്യ
libcrun_open_proc_file (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ flags, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  എങ്കിൽ (proc_fd < 0)
    തിരികെ proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  എങ്കിൽ (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  തിരികെ fd;
}

പൂർണ്ണസംഖ്യ
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ flags, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *full_path = ശൂന്യ;
  xasprintf (&full_path, "%d/%s", pid, path);
  തിരികെ libcrun_open_proc_file (container, full_path, flags, err);
}
