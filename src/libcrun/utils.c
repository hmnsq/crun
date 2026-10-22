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
#include <config.h>
#include "utils.h"
#include "ring_buffer.h"
#include <stdarg.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/signalfd.h>
#include <sys/epoll.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/time.h>
#include <sys/xattr.h>
#include <sys/sysmacros.h>
#include <sys/vfs.h>
#include <linux/magic.h>
#include <limits.h>
#include <sys/mman.h>
#ifdef HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#endif
#如果 HAVE_STDATOMIC_H
#  include <stdatomic.h>
#否则
#  define atomic_long volatile long
#  define atomic_int volatile 整数
#endif
#include "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_ROOT
#  define RESOLVE_IN_ROOT 0x10
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

整数
crun_path_exists (const char *path, libcrun_error_t *err)
{
  整数 ret = access (path, F_OK);
  如果 (ret < 0)
    {
      如果 (errno == ENOENT)
        返回 0;
      返回 crun_make_error (err, errno, "access `%s`", path);
    }
  返回 1;
}

整数
xasprintf (char **str, const char *fmt, ...)
{
  整数 ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  如果 (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  返回 ret;
}

整数
write_file_at_with_flags (整数 dirfd, 整数 flags, mode_t mode, const char *name, const void *data, size_t len, libcrun_error_t *err)
{
  cleanup_close 整数 fd = -1;
  整数 ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  如果 (UNLIKELY (fd < 0))
    返回 crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  返回 (len > INT_MAX) ? INT_MAX : (整数) len;
}

整数
detach_process ()
{
  pid_t pid;
  如果 (setsid () < 0)
    返回 -1;
  pid = fork ();
  如果 (pid < 0)
    返回 -1;
  如果 (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  返回 0;
}

整数
get_file_type_fd (整数 fd, mode_t *mode)
{
  struct stat st;
  整数 ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  如果 (UNLIKELY (ret < 0))
    {
      如果 (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      返回 ret;
    }
  *mode = stx.stx_mode;
  返回 ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  返回 ret;
}

整数
get_file_type_at (整数 dirfd, mode_t *mode, bool nofollow, const char *path)
{
  整数 empty_path = path == NULL ? AT_EMPTY_PATH : 0;
  struct stat st;
  整数 ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  如果 (UNLIKELY (ret < 0))
    {
      如果 (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      返回 ret;
    }
  *mode = stx.stx_mode;
  返回 ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  返回 ret;
}

整数
get_file_type (mode_t *mode, bool nofollow, const char *path)
{
  返回 get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

整数
create_file_if_missing_at (整数 dirfd, const char *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close 整数 fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  如果 (fd_write < 0)
    {
      整数 saved_errno = errno;
      mode_t tmp_mode;
      整数 ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      如果 (ret == 0 && S_ISREG (tmp_mode))
        返回 0;

      返回 crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  返回 0;
}

static 整数
ensure_directory_internal_at (整数 dirfd, char *path, size_t len, 整数 mode, libcrun_error_t *err)
{
  char *it = path + len;
  整数 ret = 0;
  bool parent_created = false;

  为 (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      如果 (ret == 0 || errno == EEXIST)
        返回 0;

      整数 saved_errno = errno;
      如果 (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = NULL;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          如果 (ret > 0)
            返回 0;
          如果 (ret < 0)
            crun_error_release (&tmp_err);

          返回 crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      当 (it > path && *it != '/')
        {
          it--;
          len--;
        }
      如果 (it == path)
        返回 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      如果 (UNLIKELY (ret < 0))
        返回 ret;

      parent_created = true;
    }
  返回 ret;
}

整数
crun_ensure_directory_at (整数 dirfd, const char *path, 整数 mode, bool nofollow, libcrun_error_t *err)
{
  整数 ret;
  cleanup_free char *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  如果 (ret == 0)
    返回 crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  返回 0;
}

static 整数
check_fd_is_path (const char *path, 整数 fd, const char *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  char link[PATH_MAX];
  整数 ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, sizeof (link)));
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "readlink `%s`", fdname);

  如果 (((size_t) ret) != path_len || memcmp (link, path, path_len))
    返回 crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  返回 0;
}

static 整数
check_fd_under_path (const char *rootfs, size_t rootfslen, 整数 fd, const char *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  char link[PATH_MAX];
  整数 ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  如果 (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    返回 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, sizeof (link)));
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "readlink `%s`", fdname);

  如果 (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    返回 crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  返回 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
static void
close_and_replace (整数 *oldfd, 整数 newfd)
{
  如果 (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
char *chroot_realpath (const char *chroot, const char *path, char resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
static 整数
safe_openat_fallback (整数 dirfd, const char *rootfs, const char *path, 整数 flags,
                      整数 mode, libcrun_error_t *err)
{
  cleanup_free char *parent_path = NULL;
  const char *last_component = NULL;
  const char *orig_path = path;
  const char *path_in_chroot;
  cleanup_close 整数 fd = -1;
  char resolved[PATH_MAX];
  char buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  整数 ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  如果 (flags & O_NOFOLLOW)
    {
      char *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      如果 (sep == NULL)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      否则 如果 (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      如果 (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  如果 (path_in_chroot == NULL)
    返回 crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  如果 (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  如果 (last_component)
    {
      ret = snprintf (resolved, sizeof (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      如果 (UNLIKELY (ret >= (整数) sizeof (resolved)))
        返回 crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  如果 (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      如果 (UNLIKELY (ret < 0))
        返回 crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      返回 ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  ret = fd;
  fd = -1;
  返回 ret;
}

整数
safe_openat (整数 dirfd, const char *rootfs, const char *path, 整数 flags, 整数 mode,
             libcrun_error_t *err)
{
  static bool openat2_supported = true;
  整数 retries = MAX_OPENAT2_EAGAIN_RETRIES;
  整数 ret;

  如果 (is_empty_string (path))
    {
      cleanup_close 整数 fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      如果 (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      如果 (UNLIKELY (fd < 0))
        返回 crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      如果 (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;
        }

      ret = fd;
      fd = -1;
      返回 ret;
    }

  如果 (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_ROOT);
      如果 (UNLIKELY (ret < 0))
        {
          如果 (errno == EINTR)
            goto repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          如果 (errno == EAGAIN && retries-- > 0)
            goto repeat;
          如果 (errno == ENOSYS)
            openat2_supported = false;
          如果 (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            返回 safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          返回 crun_make_error (err, errno, "openat2 `%s`", path);
        }

      返回 ret;
    }

  返回 safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (整数 dfd, const char *name, char **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free char *tmp_buf = NULL;
  ssize_t size;

  do
    {
      如果 (tmp_buf != NULL)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      如果 (UNLIKELY (size < 0))
        返回 crun_make_error (err, errno, "readlink `%s`", name);
  } 当 (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = NULL;

  返回 size;
}

static 整数 crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, 整数 dirfd, const char *dirpath,
                                const char *path, 整数 mode, 整数 max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
static 整数
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, 整数 dirfd, const char *dirpath, 整数 cwd,
                                 const char *npath, const char *cur, const char *rest,
                                 整数 mode, 整数 max_readlinks, libcrun_error_t *err)
{
  cleanup_free char *target = NULL;
  cleanup_free char *new_path = NULL;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (整数) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  返回 crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

static 整数
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, 整数 dirfd, const char *dirpath,
                     const char *path, 整数 mode, 整数 max_readlinks, libcrun_error_t *err)
{
  cleanup_close 整数 wd_cleanup = -1;
  cleanup_free char *npath = NULL;
  bool last_component = false;
  size_t depth = 0;
  char *cur;
  char *it;
  整数 cwd;
  整数 ret;

  如果 (max_readlinks <= 0)
    返回 crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  如果 (*path == '\0')
    {
      如果 (do_open)
        返回 open (dirpath, O_CLOEXEC | O_PATH, 0);
      返回 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  当 (cur)
    {
      如果 (it)
        *it = '\0';
      否则
        last_component = true;

      如果 (cur[0] == '\0')
        break;

      如果 (strcmp (cur, ".") == 0)
        goto next;
      否则 如果 (strcmp (cur, ".."))
        depth++;
      否则
        {
          如果 (depth)
            depth--;
          否则
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              goto next;
            }
        }

      如果 (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          如果 (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              如果 (errno == ELOOP)
                返回 crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, NULL,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              如果 (ret < 0)
                返回 crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          如果 (do_open)
            返回 ret;

          close_and_replace (&wd_cleanup, ret);
          返回 0;
        }

      ret = mkdirat (cwd, cur, mode);
      如果 (ret < 0)
        {
          mode_t st_mode;

          如果 (errno != EEXIST)
            返回 crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          如果 (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            返回 crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : NULL, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      如果 (UNLIKELY (cwd < 0))
        返回 crun_error_wrap (err, "creating `/%s`", path);

      如果 (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, NULL);
          如果 (UNLIKELY (ret < 0))
            {
              整数 saved_errno = errno;

              close (cwd);
              返回 crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          如果 ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              返回 crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      如果 (it == NULL)
        break;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  如果 (do_open)
    {
      如果 (cwd == dirfd)
        {
          ret = dup (dirfd);
          如果 (UNLIKELY (ret < 0))
            返回 crun_make_error (err, errno, "dup `%s`", dirpath);
          返回 ret;
        }

      wd_cleanup = -1;
      返回 cwd;
    }

  返回 0;
}

整数
crun_safe_create_and_open_ref_at (bool dir, 整数 dirfd, const char *dirpath, const char *path, 整数 mode, libcrun_error_t *err)
{
  整数 ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  如果 (LIKELY (ret >= 0))
    返回 ret;

  crun_error_release (err);
  返回 crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

整数
crun_safe_ensure_directory_at (整数 dirfd, const char *dirpath, const char *path, 整数 mode,
                               libcrun_error_t *err)
{
  返回 crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

整数
crun_safe_ensure_file_at (整数 dirfd, const char *dirpath, const char *path, 整数 mode,
                          libcrun_error_t *err)
{
  返回 crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

整数
crun_ensure_directory (const char *path, 整数 mode, bool nofollow, libcrun_error_t *err)
{
  返回 crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

static 整数
get_file_size (整数 fd, off_t *size)
{
  struct stat st;
  整数 ret;
#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  如果 (UNLIKELY (ret < 0))
    {
      如果 (errno == ENOSYS || errno == EINVAL)
        goto fallback;
      返回 ret;
    }
  *size = stx.stx_size;

  返回 ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  返回 ret;
}

整数
crun_dir_p_at (整数 dirfd, const char *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  整数 ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "stat `%s`", path);

  返回 S_ISDIR (mode) ? 1 : 0;
}

整数
crun_dir_p (const char *path, bool nofollow, libcrun_error_t *err)
{
  返回 crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
static 整数
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  size_t len;
  整数 ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  如果 (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      如果 (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          返回 0;
        }
      返回 ret;
    }

  返回 strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
static bool
running_in_openvz ()
{
  如果 (access ("/proc/vz", F_OK) < 0)
    返回 false;

  返回 access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

整数
check_running_in_user_namespace (libcrun_error_t *err)
{
  static atomic_int run_in_userns = -1;
  struct stat st;
  整数 ret;

  ret = run_in_userns;
  如果 (ret >= 0)
    返回 ret;

  ret = stat ("/proc/self/ns/user", &st);
  如果 (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      如果 (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          返回 run_in_userns;
        }

      如果 (! running_in_openvz ())
        {
          run_in_userns = 1;
          返回 run_in_userns;
        }
    }
  否则 如果 (UNLIKELY (errno != ENOENT))
    返回 crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  run_in_userns = ret;
  返回 run_in_userns;
}

static size_t
get_page_size ()
{
  static atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  如果 (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  返回 (size_t) pagesize;
}

static 整数 selinux_enabled = -1;
static 整数 apparmor_enabled = -1;

整数
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free char *out = NULL;
  cleanup_close 整数 fd = -1;
  size_t len;
  整数 ret;

  如果 (selinux_enabled >= 0)
    返回 selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  如果 (UNLIKELY (fd < 0))
    返回 fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  返回 selinux_enabled;
}

整数
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close 整数 fd = -1;
  整数 size;
  char buf[1];

  如果 (apparmor_enabled >= 0)
    返回 apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  如果 (fd == -1)
    {
      如果 (errno == ENOENT)
        {
          apparmor_enabled = 0;
          返回 0;
        }

      返回 crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (read (fd, buf, sizeof (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  返回 apparmor_enabled;
}

static 整数
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  如果 (selinux_enabled < 0)
    返回 crun_make_error (err, 0, "SELinux is not initialized correctly");
  返回 selinux_enabled;
}

整数
add_selinux_mount_label (char **retlabel, const char *data, const char *label, const char *context_type, libcrun_error_t *err)
{
  整数 ret;

  ret = libcrun_is_selinux_enabled (err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  如果 (label && ret)
    {
      如果 (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      否则
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      返回 0;
    }
  *retlabel = xstrdup (data);
  返回 0;
}

static const char *
lsm_attr_path (libcrun_container_t *container, const char *lsm, const char *fname, libcrun_error_t *err)
{
  cleanup_close 整数 attr_dirfd = -1;
  cleanup_close 整数 lsm_dirfd = -1;
  char *attr_path = NULL;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  如果 (UNLIKELY (attr_dirfd < 0))
    返回 NULL;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  如果 (lsm != NULL)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      如果 (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          返回 NULL;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  返回 attr_path;
}

static 整数
check_proc_super_magic (整数 fd, const char *path, libcrun_error_t *err)
{
  struct statfs sfs;

  整数 ret = fstatfs (fd, &sfs);
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "statfs `%s`", path);

  如果 (sfs.f_type != PROC_SUPER_MAGIC)
    返回 crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  返回 0;
}

static 整数
set_security_attr (libcrun_container_t *container, const char *lsm, const char *fname, const char *data, libcrun_error_t *err)
{
  整数 ret;

  cleanup_free const char *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close 整数 fd = -1;

  如果 (UNLIKELY (attr_path == NULL))
    返回 -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  如果 (UNLIKELY (fd < 0))
    返回 fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (write (fd, data, strlen (data)));
  如果 (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      如果 (errno == ENOENT && lsm != NULL && strcmp (lsm, "apparmor") == 0)
        {
          const char *profile = strchr (data, ' ');

          返回 crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      返回 crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  返回 0;
}

整数
set_selinux_label (libcrun_container_t *container, const char *label, bool now, libcrun_error_t *err)
{
  整数 ret;

  ret = libcrun_is_selinux_enabled (err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  如果 (ret)
    返回 set_security_attr (container, NULL, now ? "current" : "exec", label, err);
  返回 0;
}

static 整数
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  如果 (apparmor_enabled < 0)
    返回 crun_make_error (err, 0, "AppArmor is not initialized correctly");
  返回 apparmor_enabled;
}

static 整数
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (sizeof (UNCONFINED) - 1)
  cleanup_free const char *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close 整数 fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  char buf[UNCONFINED_LEN + 1];

  如果 (UNLIKELY (attr_path == NULL))
    返回 -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  如果 (UNLIKELY (fd < 0))
    返回 fd;

  如果 (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    返回 -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (read (fd, buf, sizeof (buf)));
  如果 (UNLIKELY (bytes_read < 0))
    返回 crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  如果 (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    返回 1;
  如果 (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    返回 0;
  返回 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

整数
set_apparmor_profile (libcrun_container_t *container, const char *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  整数 ret;

  ret = libcrun_is_apparmor_enabled (err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  如果 (ret)
    {
      cleanup_free char *buf = NULL;
      ret = is_current_process_confined (container, err);
      如果 (UNLIKELY (ret < 0))
        返回 ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      返回 set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  返回 0;
}

static 整数
read_all_fd_internal (整数 fd, const char *description, char **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free char *buf = NULL;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  整数 ret;

  如果 (size_hint)
    allocated = size_hint;
  否则
    {
      ret = get_file_size (fd, &size);
      如果 (UNLIKELY (ret < 0))
        返回 crun_make_error (err, errno, "stat `%s`", description);

      如果 (max_size && (size_t) size > max_size)
        返回 crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  当 ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (read (fd, buf + nread, allocated - nread));
      如果 (UNLIKELY (ret < 0))
        返回 crun_make_error (err, errno, "read from file `%s`", description);

      如果 (ret == 0)
        break;

      nread += ret;

      如果 (max_size && nread > max_size)
        返回 crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      如果 (nread == allocated)
        {
          如果 (size)
            break;

          如果 (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  如果 (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      char *tmp = realloc (buf, nread + 1);
      如果 (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = NULL;
  如果 (len)
    *len = nread;
  返回 0;
}

整数
read_all_fd_with_size_hint (整数 fd, const char *description, char **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  返回 read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

整数
read_all_fd_with_max_size (整数 fd, const char *description, char **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  返回 read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

整数
read_all_file_at (整数 dirfd, const char *path, char **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close 整数 fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  如果 (UNLIKELY (fd < 0))
    返回 crun_make_error (err, errno, "open `%s`", path);

  返回 read_all_fd (fd, path, out, len, err);
}

整数
read_all_file (const char *path, char **out, size_t *len, libcrun_error_t *err)
{
  如果 (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  返回 read_all_file_at (AT_FDCWD, path, out, len, err);
}

整数
get_realpath_to_file (整数 dirfd, const char *path_name, char **absolute_path, libcrun_error_t *err)
{
  cleanup_close 整数 targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  如果 (UNLIKELY (targetfd < 0))
    返回 crun_make_error (err, errno, "open `%s`", path_name);
  否则
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      如果 (UNLIKELY (len < 0))
        返回 crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  返回 0;
}

整数
open_unix_domain_client_socket (const char *path, 整数 dgram, libcrun_error_t *err)
{
  struct sockaddr_un addr = {};
  整数 ret;
  proc_fd_path_t name_buf;
  cleanup_close 整数 destfd = -1;
  cleanup_close 整数 fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  如果 (UNLIKELY (fd < 0))
    返回 crun_make_error (err, errno, "create UNIX socket");

  如果 (strlen (path) >= sizeof (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      如果 (UNLIKELY (destfd < 0))
        返回 crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  如果 (path_len >= sizeof (addr.sun_path))
    返回 crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (struct sockaddr *) &addr, sizeof (addr));
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  返回 ret;
}

整数
open_unix_domain_socket (const char *path, 整数 dgram, libcrun_error_t *err)
{
  struct sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  整数 ret;
  cleanup_close 整数 dirfd = -1;
  cleanup_close 整数 fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  如果 (UNLIKELY (fd < 0))
    返回 crun_make_error (err, errno, "create UNIX socket");

  如果 (strlen (path) >= sizeof (addr.sun_path))
    {
      cleanup_free char *dpath = xstrdup (path);
      cleanup_free char *bpath = xstrdup (path);
      const char *parent_dir = dirname (dpath);
      const char *base = basename (bpath);
      整数 n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      如果 (UNLIKELY (dirfd < 0))
        返回 crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, sizeof (addr.sun_path), "%s/%s", name_buf, base);
      如果 (n < 0 || (size_t) n >= sizeof (addr.sun_path))
        返回 crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  否则
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (struct sockaddr *) &addr, sizeof (addr));
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "bind socket to `%s`", path);

  如果 (! dgram)
    {
      ret = listen (fd, 1);
      如果 (UNLIKELY (ret < 0))
        返回 crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  返回 ret;
}

整数
send_fd_to_socket (整数 server, 整数 fd, libcrun_error_t *err)
{
  返回 send_fd_to_socket_with_payload (server, fd, NULL, 0, err);
}

整数
send_fd_to_socket_with_payload (整数 server, 整数 fd, const char *payload, size_t payload_len, libcrun_error_t *err)
{
  整数 ret;
  struct cmsghdr *cmsg = NULL;
  struct iovec iov[2];
  struct msghdr msg = {};
  char ctrl_buf[CMSG_SPACE (1 + sizeof (整数))] = {};
  char data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = sizeof (data);

  如果 (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (sizeof (整数));
  msg.msg_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (sizeof (整数));

  *((整数 *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &msg, 0));
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "sendmsg");
  返回 0;
}

整数
receive_fd_from_socket_with_payload (整数 from, char *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close 整数 fd = -1;
  整数 ret;
  struct iovec iov[1];
  struct msghdr msg = {};
  char ctrl_buf[CMSG_SPACE (sizeof (整数))] = {};
  char data[1];
  struct cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = sizeof (data);

  如果 (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (sizeof (整数));
  msg.msg_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &msg, 0));
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "recvmsg");
  如果 (UNLIKELY (ret == 0))
    返回 crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&msg);
  如果 (cmsg == NULL)
    返回 crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), sizeof (fd));

  ret = fd;
  fd = -1;
  返回 ret;
}

整数
receive_fd_from_socket (整数 from, libcrun_error_t *err)
{
  返回 receive_fd_from_socket_with_payload (from, NULL, 0, err);
}

整数
create_socket_pair (整数 *pair, libcrun_error_t *err)
{
  整数 ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "socketpair");
  返回 0;
}

整数
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  整数 ret = signalfd (-1, mask, 0);
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "signalfd");
  返回 ret;
}

static 整数
epoll_helper_toggle (整数 epollfd, 整数 fd, 整数 events, libcrun_error_t *err)
{
  struct epoll_event ev = {};
  bool add = events != 0;
  整数 ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  如果 (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  如果 (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      如果 (errno == ENOENT || errno == EPERM)
        返回 0;
      返回 crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  返回 0;
}

整数
epoll_helper (整数 *in_fds, 整数 *in_levelfds, 整数 *out_fds, 整数 *out_levelfds, libcrun_error_t *err)
{
  struct epoll_event ev;
  cleanup_close 整数 epollfd = -1;
  整数 ret;

  整数 *it;
  epollfd = epoll_create1 (0);
  如果 (UNLIKELY (epollfd < 0))
    返回 crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  为 (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      如果 (UNLIKELY (ret < 0))                                           \
        返回 crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  如果 (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  如果 (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  如果 (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  如果 (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  返回 ret;
}

整数
copy_from_fd_to_fd (整数 src, 整数 dst, 整数 consume, libcrun_error_t *err)
{
  整数 ret;
  ssize_t nread;
  size_t pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  do
    {
      cleanup_free char *buffer = NULL;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      如果 (can_copy_file_range)
        {
          nread = copy_file_range (src, NULL, dst, NULL, pagesize, 0);
          如果 (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              goto fallback;
            }
          如果 (consume && nread < 0 && errno == EAGAIN)
            返回 0;
          如果 (nread < 0 && errno == EIO)
            返回 0;
          如果 (UNLIKELY (nread < 0))
            返回 crun_make_error (err, errno, "copy_file_range");
          continue;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (read (src, buffer, pagesize));
      如果 (consume && nread < 0 && errno == EAGAIN)
        返回 0;
      如果 (nread < 0 && errno == EIO)
        返回 0;
      如果 (UNLIKELY (nread < 0))
        返回 crun_make_error (err, errno, "read");

      remaining = nread;
      当 (remaining)
        {
          ret = TEMP_FAILURE_RETRY (write (dst, buffer + nread - remaining, remaining));
          如果 (UNLIKELY (ret < 0))
            返回 crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } 当 (consume && nread);

  返回 0;
}

整数
run_process (char **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  如果 (UNLIKELY (pid < 0))
    返回 crun_make_error (err, errno, "fork");
  如果 (pid)
    {
      整数 r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      如果 (r < 0)
        返回 crun_make_error (err, errno, "waitpid");
      如果 (WIFEXITED (status) || WIFSIGNALED (status))
        返回 WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
static unsigned
atou (char **s)
{
  unsigned x;
  为 (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  返回 x;
}

整数
fgetpwent_r (FILE *f, struct passwd *pw, char *line, size_t size, struct passwd **res)
{
  char *s;
  整数 rv = 0;
  为 (;;)
    {
      line[size - 1] = '\xff';
      如果 ((fgets (line, size, f) == NULL) || ferror (f) || line[size - 1] != '\xff')
        {
          rv = (line[size - 1] != '\xff') ? ERANGE : ENOENT;
          line = 0;
          pw = 0;
          break;
        }
      line[strcspn (line, "\n")] = 0;

      s = line;
      pw->pw_name = s++;
      如果 (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_passwd = s;
      如果 (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      如果 (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      如果 (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gecos = s;
      如果 (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_dir = s;
      如果 (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_shell = s;
      break;
    }
  *res = pw;
  如果 (rv)
    errno = rv;
  返回 rv;
}
#endif

整数
set_home_env (uid_t id)
{
  struct passwd pwd;
  cleanup_free char *buf = NULL;
  long buf_size;
  cleanup_file FILE *stream = NULL;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  如果 (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  如果 (stream == NULL)
    返回 -1;

  为 (;;)
    {
      struct passwd *ret_pw = NULL;

      整数 ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      如果 (UNLIKELY (ret != 0))
        {
          如果 (ret == EINTR)
            continue;

          如果 (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              返回 -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          continue;
        }

      如果 (ret_pw && ret_pw->pw_uid == id)
        {
          如果 (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          否则
            返回 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
static 整数
getsubidrange (uid_t id, 整数 is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *input = NULL;
  cleanup_free char *lineptr = NULL;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free char *buf = NULL;
  const char *name;
  struct passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  如果 (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  为 (;;)
    {
      整数 ret;
      struct passwd *ret_pw = NULL;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      如果 (LIKELY (ret == 0))
        {
          如果 (ret_pw)
            {
              name = ret_pw->pw_name;
              break;
            }
          返回 -1;
        }

      如果 (ret != ERANGE)
        返回 -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  input = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  如果 (input == NULL)
    返回 -1;

  为 (;;)
    {
      char *endptr;
      ssize_t read = getline (&lineptr, &lenlineptr, input);
      如果 (read < 0)
        返回 -1;

      如果 (read < (ssize_t) (len_name + 2))
        continue;

      如果 (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        continue;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      如果 (endptr >= &lineptr[read])
        返回 -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      返回 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

整数
format_default_id_mapping (char **out, uid_t container_id, uid_t host_uid, uid_t host_id, 整数 is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free char *buffer = NULL;
  整数 written = 0;
  整数 ret, remaining;

  *out = NULL;

  如果 (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    返回 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  如果 (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      如果 (UNLIKELY (ret >= remaining))
        返回 crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  如果 (UNLIKELY (ret >= remaining))
    返回 crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  如果 (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      如果 (UNLIKELY (ret >= remaining))
        返回 crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = NULL;
  返回 written;
}

static 整数
unset_cloexec_flag (整数 fd)
{
  整数 flags = fcntl (fd, F_GETFD);
  如果 (flags == -1)
    返回 -1;

  flags &= ~FD_CLOEXEC;

  返回 fcntl (fd, F_SETFD, flags);
}

static void __attribute__ ((__noreturn__))
run_process_child (char *path, char **args, const char *cwd, char **envp, 整数 pipe_r,
                   整数 pipe_w, 整数 out_fd, 整数 err_fd, bool can_ignore_chdir_errors)
{
  char *tmp_args[] = { path, NULL };
  libcrun_error_t err = NULL;
  整数 dev_null_fd = -1;
  整数 ret;

  ret = mark_or_close_fds_ge_than (NULL, 3, false, &err);
  如果 (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->msg);

  如果 (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      如果 (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  如果 (out_fd >= 0)
    unset_cloexec_flag (1);
  如果 (err_fd >= 0)
    unset_cloexec_flag (2);

  如果 (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  如果 (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  如果 (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  如果 (args == NULL)
    args = tmp_args;

  如果 (cwd && chdir (cwd) < 0)
    {
      如果 (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
整数
run_process_with_stdin_timeout_envp (char *path, char **args, const char *cwd, 整数 timeout,
                                     char **envp, char *stdin, size_t stdin_len, 整数 out_fd,
                                     整数 err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  整数 stdin_pipe[2];
  pid_t pid;
  整数 ret;
  cleanup_close 整数 pipe_r = -1;
  cleanup_close 整数 pipe_w = -1;
  sigset_t oldmask, mask;
  整数 r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  如果 (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      如果 (UNLIKELY (ret < 0))
        返回 crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  如果 (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      goto restore_sig_mask_and_exit;
    }

  如果 (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (write (pipe_w, stdin, stdin_len));
  如果 (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      如果 (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          goto restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  如果 (timeout)
    {
      time_t start = time (NULL);
      time_t now;
      为 (now = start; now - start < timeout; now = time (NULL))
        {
          siginfo_t info;
          整数 elapsed = now - start;
          struct timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          如果 (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              goto restore_sig_mask_and_exit;
            }

          如果 (info.si_signo == SIGCHLD && info.si_pid == pid)
            goto read_waitpid;

          如果 (ret < 0 && errno == EAGAIN)
            goto timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      goto restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  如果 (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  否则
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  如果 (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      如果 (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, NULL);
      如果 (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  返回 ret;
}

整数
mark_or_close_fds_ge_than (libcrun_container_t *container, 整数 n, bool close_now, libcrun_error_t *err)
{
  cleanup_close 整数 cfd = -1;
  cleanup_dir DIR *dir = NULL;
  整数 ret;
  整数 fd;
  struct dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  如果 (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  如果 (ret == 0)
    {
      如果 (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      返回 0;
    }
  如果 (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    返回 crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  如果 (UNLIKELY (cfd < 0))
    返回 cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  如果 (UNLIKELY (ret < 0))
    返回 ret;

  dir = fdopendir (cfd);
  如果 (UNLIKELY (dir == NULL))
    返回 crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  为 (next = readdir (dir); next; next = readdir (dir))
    {
      整数 val;
      const char *name = next->d_name;
      如果 (name[0] == '.')
        continue;

      val = strtoll (name, NULL, 10);
      如果 (val < n || val == fd)
        continue;

      如果 (close_now)
        {
          ret = close (val);
          如果 (UNLIKELY (ret < 0))
            返回 crun_make_error (err, errno, "close fd `%d`", val);

          如果 (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      否则
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          如果 (UNLIKELY (ret < 0))
            返回 crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  返回 0;
}

void
get_current_timestamp (char *out, size_t len)
{
  struct timeval tv;
  struct tm now;
  char timestamp[64];

  gettimeofday (&tv, NULL);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, sizeof (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (void) snprintf (out, len, "%s.%06lldZ", timestamp, (long long 整数) tv.tv_usec);
  out[len - 1] = '\0';
}

整数
set_blocking_fd (整数 fd, bool blocking, libcrun_error_t *err)
{
  整数 ret, flags = fcntl (fd, F_GETFL, 0);
  如果 (UNLIKELY (flags < 0))
    返回 crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "fcntl");
  返回 0;
}

整数
parse_json_file (json_object **out, const char *jsondata, struct parser_context *ctx arg_unused, libcrun_error_t *err)
{
  struct json_tokener *tok;
  enum json_tokener_error jerr;

  *err = NULL;

  如果 (UNLIKELY (jsondata == NULL))
    返回 crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  如果 (UNLIKELY (tok == NULL))
    返回 crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (整数) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  如果 (*out == NULL || jerr != json_tokener_success)
    {
      整数 saved_pos = (整数) tok->char_offset;

      json_tokener_free (tok);
      如果 (*out)
        {
          json_object_put (*out);
          *out = NULL;
        }
      返回 crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  返回 0;
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
static 整数
check_access (const char *path)
{
  整数 ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#否则
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  如果 (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      如果 (errno == EACCES)
        {
          整数 saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          如果 (ret == 0)
            返回 CHECK_ACCESS_NOT_EXECUTABLE;
        }
      返回 -errno;
    }

  ret = get_file_type (&mode, false, path);
  如果 (UNLIKELY (ret < 0))
    返回 -errno;

  如果 (! S_ISREG (mode))
    返回 CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  返回 0;
}

整数
find_executable (char **out, const char *executable_path, const char *cwd, libcrun_error_t *err)
{
  cleanup_free char *cwd_executable_path = NULL;
  cleanup_free char *tmp = NULL;
  整数 last_error = -ENOENT;
  char *it, *end;
  整数 ret;

  *out = NULL;

  如果 (executable_path == NULL || executable_path[0] == '\0')
    返回 crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  如果 (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free char *cwd_allocated = NULL;

      如果 (cwd == NULL)
        {
          cwd_allocated = getcwd (NULL, 0);
          如果 (cwd_allocated == NULL)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, NULL);
      如果 (UNLIKELY (ret < 0))
        返回 ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  如果 (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      如果 (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          返回 0;
        }
      last_error = ret;
      goto fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  当 ((it = strsep (&end, ":")))
    {
      cleanup_free char *path = NULL;

      如果 (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, NULL);
      如果 (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          continue;
        }

      ret = check_access (path);
      如果 (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = NULL;
          返回 0;
        }

      如果 (ret == -ENOENT)
        continue;

      last_error = ret;
    }

fail:
  switch (last_error)
    {
    case CHECK_ACCESS_NOT_EXECUTABLE:
      返回 crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    case CHECK_ACCESS_NOT_REGULAR:
      返回 crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    default:
      errno = -last_error;
      如果 (errno == ENOENT)
        返回 crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      返回 crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

static ssize_t
safe_read_xattr (char **ret, 整数 sfd, const char *srcname, const char *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  当 (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      如果 (UNLIKELY (s < 0))
        返回 crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      如果 (s < current_size)
        break;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  如果 (s <= 0)
    返回 s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = NULL;

  返回 s;
}

static ssize_t
copy_xattr (整数 sfd, 整数 dfd, const char *srcname, const char *destname, libcrun_error_t *err)
{
  cleanup_free char *buf = NULL;
  ssize_t xattr_len;
  char *it;

  xattr_len = flistxattr (sfd, NULL, 0);
  如果 (UNLIKELY (xattr_len < 0))
    {
      如果 (errno == ENOTSUP)
        返回 0;

      返回 crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  如果 (xattr_len == 0)
    返回 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  如果 (UNLIKELY (xattr_len < 0))
    返回 crun_make_error (err, errno, "flistxattr `%s`", srcname);

  为 (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free char *v = NULL;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      如果 (UNLIKELY (s < 0))
        返回 s;

      s = fsetxattr (dfd, it, v, s, 0);
      如果 (UNLIKELY (s < 0))
        {
          如果 (errno == EINVAL || errno == EOPNOTSUPP)
            continue;

          返回 crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  返回 0;
}

#endif

static 整数
copy_rec_stat_file_at (整数 dfd, const char *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  struct stat st;
  整数 ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  如果 (UNLIKELY (ret < 0))
    {
      如果 (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      返回 ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  返回 ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  返回 ret;
}

整数
copy_recursive_fd_to_fd (整数 srcdirfd, 整数 dfd, const char *srcname, const char *destname, libcrun_error_t *err)
{
  cleanup_close 整数 destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = NULL;
  struct dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  如果 (UNLIKELY (dsrcfd == NULL))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      返回 crun_make_error (err, errno, "open directory `%s`", destname);
    }

  为 (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close 整数 srcfd = -1;
      cleanup_close 整数 destfd = -1;
      cleanup_free char *target_buf = NULL;
      整数 ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      如果 (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      如果 (UNLIKELY (ret < 0))
        返回 crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      switch (mode & S_IFMT)
        {
        case S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          如果 (UNLIKELY (srcfd < 0))
            返回 crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          如果 (UNLIKELY (destfd < 0))
            返回 crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;

#ifdef HAVE_FGETXATTR
          ret = (整数) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          break;

        case S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          如果 (UNLIKELY (ret < 0))
            返回 crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          如果 (UNLIKELY (srcfd < 0))
            返回 crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          如果 (UNLIKELY (destfd < 0))
            返回 crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (整数) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          如果 (UNLIKELY (ret < 0))
            返回 ret;
          break;

        case S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          如果 (UNLIKELY (ret < 0))
            返回 crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          break;

        case S_IFBLK:
        case S_IFCHR:
        case S_IFIFO:
        case S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          如果 (UNLIKELY (ret < 0))
            返回 crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          break;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      如果 (UNLIKELY (ret < 0))
        返回 crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      如果 (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          如果 (errno == ENOTSUP)
            continue;

          返回 crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  返回 0;
}

const char *
find_annotation (libcrun_container_t *container, const char *name)
{
  如果 (container->container_def->annotations == NULL)
    返回 NULL;

  返回 find_string_map_value (container->annotations, name);
}

整数
safe_write (整数 fd, const char *fname, const void *buf, size_t count, libcrun_error_t *err)
{
  size_t written = 0;
  当 (written < count)
    {
      ssize_t w = write (fd, (const char *) buf + written, count - written);
      如果 (UNLIKELY (w < 0))
        {
          如果 (errno == EINTR || errno == EAGAIN)
            continue;
          返回 crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  返回 0;
}

#define MAX_PARTS 32

整数
append_paths (char **out, libcrun_error_t *err, ...)
{
  const char *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t total_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  为 (;;)
    {
      const char *part;
      size_t size;

      part = va_arg (ap, const char *);
      如果 (part == NULL)
        break;

      如果 (n_parts == MAX_PARTS)
        {
          va_end (ap);
          返回 crun_make_error (err, EINVAL, "too many paths specified");
        }

      如果 (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          当 (part[0] == '/' && part[1] == '/')
            part++;
        }
      否则
        {
          /* And drop any initial '/' for other components.  */
          当 (part[0] == '/')
            part++;
        }

      size = strlen (part);
      如果 (size == 0)
        continue;

      当 (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  为 (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  为 (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      如果 (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  返回 0;
}

#如果 __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#否则
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
static 整数
b64_input (char c)
{
  const char table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  整数 i;

  为 (i = 0; i < 64; i++)
    {
      如果 (table[i] == c)
        返回 i;
    }
  返回 -1;
}

整数
base64_decode (const char *iptr, size_t isize, char *optr, size_t osize, size_t *nbytes)
{
  整数 i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned char data[4];

  *nbytes = 0;
  当 (consumed < isize && (*nbytes) + 3 < osize)
    {
      当 ((i < 4) && (consumed < isize))
        {
          tmp = b64_input (*iptr++);
          consumed++;
          如果 (tmp != -1)
            {
              如果 (UNLIKELY (pad > 0))
                返回 -1;
              data[i++] = tmp;
            }
          否则 如果 (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      如果 (i == 4)
        {
          如果 (UNLIKELY (pad > 2))
            返回 -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      否则
        {
          /* I did not get all the data.  */
          consumed -= i;
          返回 consumed;
        }
      i = 0;
    }
  返回 consumed;
}

char *
get_user_name (uid_t uid)
{
  struct passwd pd;
  struct passwd *temp_result_ptr;
  char pwdbuffer[200];
  如果 (! getpwuid_r (uid, &pd, pwdbuffer, sizeof (pwdbuffer), &temp_result_ptr))
    返回 xstrdup (pd.pw_name);
  返回 xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
char *
libcrun_sysctl_key_to_proc_path (const char *key)
{
  const char *sep;
  char *ret, *it;

  sep = strpbrk (key, "./");

  /* No separator at all, or the key is already in the path form.  */
  如果 (sep == NULL || *sep == '/')
    返回 xstrdup (key);

  ret = xstrdup (key);
  为 (it = ret; *it; it++)
    {
      如果 (*it == '.')
        *it = '/';
      否则 如果 (*it == '/')
        *it = '.';
    }

  返回 ret;
}

整数
has_suffix (const char *str, const char *suffix)
{
  如果 (! str || ! suffix)
    返回 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  如果 (lensuffix > lenstr)
    返回 0;
  返回 memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

char *
str_join_array (整数 offset, size_t size, char *const array[], const char *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = NULL;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  char *result, *p;

  lens = xmalloc (size * sizeof (*lens));

  为 (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  为 (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      如果 (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  返回 result;
}

整数
libcrun_mmap (struct libcrun_mmap_s **ret, void *addr, size_t length,
              整数 prot, 整数 flags, 整数 fd, off_t offset,
              libcrun_error_t *err)
{
  struct libcrun_mmap_s *mmap_s = NULL;

  void *mapped = mmap (addr, length, prot, flags, fd, offset);
  如果 (mapped == MAP_FAILED)
    返回 crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (sizeof (struct libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  返回 0;
}

整数
libcrun_munmap (struct libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  整数 ret;

  ret = munmap (mmap->addr, mmap->length);
  如果 (UNLIKELY (ret < 0))
    返回 crun_make_error (err, errno, "munmap");

  free (mmap);

  返回 0;
}

static long
read_file_as_long_or_default (const char *path, long def_value)
{
  cleanup_free char *content = NULL;
  libcrun_error_t tmp_err = NULL;
  char *endptr;
  long val;
  整数 ret;

  ret = read_all_file (path, &content, NULL, &tmp_err);
  如果 (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      返回 def_value;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  如果 (UNLIKELY (errno))
    返回 def_value;
  如果 (endptr == content || (*endptr && *endptr != '\n'))
    返回 def_value;
  返回 val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (void)
{
  static atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  如果 (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  返回 uid;
}

gid_t
get_overflow_gid (void)
{
  static atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  如果 (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  返回 gid;
}

void
consume_trailing_slashes (char *path)
{
  如果 (! path || path[0] == '\0')
    返回;

  char *last = path + strlen (path);

  当 (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

char **
read_dir_entries (const char *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = NULL;
  size_t n_entries = 0;
  size_t entries_size = 16;
  char **entries = NULL;
  struct dirent *de;

  dir = opendir (path);
  如果 (UNLIKELY (dir == NULL))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      返回 NULL;
    }

  entries = xmalloc (entries_size * sizeof (char *));
  当 ((de = readdir (dir)))
    {
      如果 (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;
      如果 (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * sizeof (char *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * sizeof (char *));
  entries[n_entries] = NULL;

  返回 entries;
}

整数
cpuset_string_to_bitmask (const char *str, char **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free char *mask = NULL;
  size_t mask_size = 0;
  const char *p = str;
  char *endptr;

  当 (*p)
    {
      long long start_range, end_range;

      如果 (*p < '0' || *p > '9')
        goto invalid_input;

      start_range = strtoll (p, &endptr, 10);
      如果 (start_range < 0)
        goto invalid_input;

      p = endptr;

      如果 (*p != '-')
        end_range = start_range;
      否则
        {
          p++;

          如果 (*p < '0' || *p > '9')
            goto invalid_input;

          end_range = strtoll (p, &endptr, 10);

          如果 (end_range < start_range)
            goto invalid_input;

          p = endptr;
        }

      /* Just set some limit.  */
      如果 (end_range > (1 << 20))
        goto invalid_input;

      如果 (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      为 (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      如果 (*p == ',')
        p++;
      否则 如果 (*p)
        goto invalid_input;
    }

  *out = mask;
  mask = NULL;
  *out_size = mask_size;

  返回 0;

invalid_input:
  返回 crun_make_error (err, 0, "cannot parse input `%s`", str);
}

struct channel_fd_pair
{
  struct ring_buffer *rb;

  整数 in_fd;
  整数 out_fd;

  整数 infd_epoll_events;
  整数 outfd_epoll_events;
};

struct channel_fd_pair *
channel_fd_pair_new (整数 in_fd, 整数 out_fd, size_t size)
{
  struct channel_fd_pair *channel = xmalloc (sizeof (struct channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  返回 channel;
}

void
channel_fd_pair_free (struct channel_fd_pair *channel)
{
  如果 (channel == NULL)
    返回;

  ring_buffer_free (channel->rb);
  free (channel);
}

整数
channel_fd_pair_process (struct channel_fd_pair *channel, 整数 epollfd, libcrun_error_t *err)
{
  bool is_input_eagain = false, is_output_eagain = false, repeat;
  整数 ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  为 (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      如果 (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_input_eagain, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;
          如果 (ret > 0)
            repeat = true;
        }
      如果 (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;
          如果 (ret > 0)
            repeat = true;
        }
    }

  如果 (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      整数 events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_input_eagain ? EPOLLET : 0)) : 0;
      如果 (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      如果 (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          如果 (UNLIKELY (ret < 0))
            返回 ret;
          channel->outfd_epoll_events = events;
        }
    }
  返回 0;
}

static 整数
open_proc (libcrun_error_t *err)
{
  整数 fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close 整数 fsfd = -1;
  整数 ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  如果 (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
      如果 (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          如果 (fd >= 0)
            返回 fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  如果 (fd < 0)
    返回 crun_make_error (err, errno, "open `/proc`");
  返回 fd;
}

整数
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  如果 (! container)
    返回 open_proc (err);

  如果 (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  返回 container->proc_fd;
}

整数
libcrun_open_proc_file (libcrun_container_t *container, const char *path, 整数 flags, libcrun_error_t *err)
{
  整数 proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  如果 (proc_fd < 0)
    返回 proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  如果 (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  如果 (UNLIKELY (fd < 0))
    返回 crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  返回 fd;
}

整数
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const char *path, 整数 flags, libcrun_error_t *err)
{
  cleanup_free char *full_path = NULL;
  xasprintf (&full_path, "%d/%s", pid, path);
  返回 libcrun_open_proc_file (container, full_path, flags, err);
}
