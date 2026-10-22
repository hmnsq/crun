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
#만약 HAVE_STDATOMIC_H
#  include <stdatomic.h>
#아니면
#  define atomic_long volatile long
#  define atomic_int volatile 정수
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

정수
crun_path_exists (const char *path, libcrun_error_t *err)
{
  정수 ret = access (path, F_OK);
  만약 (ret < 0)
    {
      만약 (errno == ENOENT)
        반환 0;
      반환 crun_make_error (err, errno, "access `%s`", path);
    }
  반환 1;
}

정수
xasprintf (char **str, const char *fmt, ...)
{
  정수 ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  만약 (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  반환 ret;
}

정수
write_file_at_with_flags (정수 dirfd, 정수 flags, mode_t mode, const char *name, const void *data, size_t len, libcrun_error_t *err)
{
  cleanup_close 정수 fd = -1;
  정수 ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  만약 (UNLIKELY (fd < 0))
    반환 crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  반환 (len > INT_MAX) ? INT_MAX : (정수) len;
}

정수
detach_process ()
{
  pid_t pid;
  만약 (setsid () < 0)
    반환 -1;
  pid = fork ();
  만약 (pid < 0)
    반환 -1;
  만약 (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  반환 0;
}

정수
get_file_type_fd (정수 fd, mode_t *mode)
{
  struct stat st;
  정수 ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  만약 (UNLIKELY (ret < 0))
    {
      만약 (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      반환 ret;
    }
  *mode = stx.stx_mode;
  반환 ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  반환 ret;
}

정수
get_file_type_at (정수 dirfd, mode_t *mode, bool nofollow, const char *path)
{
  정수 empty_path = path == NULL ? AT_EMPTY_PATH : 0;
  struct stat st;
  정수 ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  만약 (UNLIKELY (ret < 0))
    {
      만약 (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      반환 ret;
    }
  *mode = stx.stx_mode;
  반환 ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  반환 ret;
}

정수
get_file_type (mode_t *mode, bool nofollow, const char *path)
{
  반환 get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

정수
create_file_if_missing_at (정수 dirfd, const char *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close 정수 fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  만약 (fd_write < 0)
    {
      정수 saved_errno = errno;
      mode_t tmp_mode;
      정수 ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      만약 (ret == 0 && S_ISREG (tmp_mode))
        반환 0;

      반환 crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  반환 0;
}

static 정수
ensure_directory_internal_at (정수 dirfd, char *path, size_t len, 정수 mode, libcrun_error_t *err)
{
  char *it = path + len;
  정수 ret = 0;
  bool parent_created = false;

  위해 (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      만약 (ret == 0 || errno == EEXIST)
        반환 0;

      정수 saved_errno = errno;
      만약 (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = NULL;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          만약 (ret > 0)
            반환 0;
          만약 (ret < 0)
            crun_error_release (&tmp_err);

          반환 crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      동안 (it > path && *it != '/')
        {
          it--;
          len--;
        }
      만약 (it == path)
        반환 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      만약 (UNLIKELY (ret < 0))
        반환 ret;

      parent_created = true;
    }
  반환 ret;
}

정수
crun_ensure_directory_at (정수 dirfd, const char *path, 정수 mode, bool nofollow, libcrun_error_t *err)
{
  정수 ret;
  cleanup_free char *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  만약 (ret == 0)
    반환 crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  반환 0;
}

static 정수
check_fd_is_path (const char *path, 정수 fd, const char *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  char link[PATH_MAX];
  정수 ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, sizeof (link)));
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "readlink `%s`", fdname);

  만약 (((size_t) ret) != path_len || memcmp (link, path, path_len))
    반환 crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  반환 0;
}

static 정수
check_fd_under_path (const char *rootfs, size_t rootfslen, 정수 fd, const char *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  char link[PATH_MAX];
  정수 ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  만약 (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    반환 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, sizeof (link)));
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "readlink `%s`", fdname);

  만약 (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    반환 crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  반환 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
static void
close_and_replace (정수 *oldfd, 정수 newfd)
{
  만약 (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
char *chroot_realpath (const char *chroot, const char *path, char resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
static 정수
safe_openat_fallback (정수 dirfd, const char *rootfs, const char *path, 정수 flags,
                      정수 mode, libcrun_error_t *err)
{
  cleanup_free char *parent_path = NULL;
  const char *last_component = NULL;
  const char *orig_path = path;
  const char *path_in_chroot;
  cleanup_close 정수 fd = -1;
  char resolved[PATH_MAX];
  char buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  정수 ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  만약 (flags & O_NOFOLLOW)
    {
      char *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      만약 (sep == NULL)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      아니면 만약 (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      만약 (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  만약 (path_in_chroot == NULL)
    반환 crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  만약 (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  만약 (last_component)
    {
      ret = snprintf (resolved, sizeof (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      만약 (UNLIKELY (ret >= (정수) sizeof (resolved)))
        반환 crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  만약 (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      만약 (UNLIKELY (ret < 0))
        반환 crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      반환 ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = fd;
  fd = -1;
  반환 ret;
}

정수
safe_openat (정수 dirfd, const char *rootfs, const char *path, 정수 flags, 정수 mode,
             libcrun_error_t *err)
{
  static bool openat2_supported = true;
  정수 retries = MAX_OPENAT2_EAGAIN_RETRIES;
  정수 ret;

  만약 (is_empty_string (path))
    {
      cleanup_close 정수 fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      만약 (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      만약 (UNLIKELY (fd < 0))
        반환 crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      만약 (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
        }

      ret = fd;
      fd = -1;
      반환 ret;
    }

  만약 (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_ROOT);
      만약 (UNLIKELY (ret < 0))
        {
          만약 (errno == EINTR)
            goto repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          만약 (errno == EAGAIN && retries-- > 0)
            goto repeat;
          만약 (errno == ENOSYS)
            openat2_supported = false;
          만약 (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            반환 safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          반환 crun_make_error (err, errno, "openat2 `%s`", path);
        }

      반환 ret;
    }

  반환 safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (정수 dfd, const char *name, char **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free char *tmp_buf = NULL;
  ssize_t size;

  do
    {
      만약 (tmp_buf != NULL)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      만약 (UNLIKELY (size < 0))
        반환 crun_make_error (err, errno, "readlink `%s`", name);
  } 동안 (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = NULL;

  반환 size;
}

static 정수 crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, 정수 dirfd, const char *dirpath,
                                const char *path, 정수 mode, 정수 max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
static 정수
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, 정수 dirfd, const char *dirpath, 정수 cwd,
                                 const char *npath, const char *cur, const char *rest,
                                 정수 mode, 정수 max_readlinks, libcrun_error_t *err)
{
  cleanup_free char *target = NULL;
  cleanup_free char *new_path = NULL;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (정수) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  반환 crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

static 정수
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, 정수 dirfd, const char *dirpath,
                     const char *path, 정수 mode, 정수 max_readlinks, libcrun_error_t *err)
{
  cleanup_close 정수 wd_cleanup = -1;
  cleanup_free char *npath = NULL;
  bool last_component = false;
  size_t depth = 0;
  char *cur;
  char *it;
  정수 cwd;
  정수 ret;

  만약 (max_readlinks <= 0)
    반환 crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  만약 (*path == '\0')
    {
      만약 (do_open)
        반환 open (dirpath, O_CLOEXEC | O_PATH, 0);
      반환 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  동안 (cur)
    {
      만약 (it)
        *it = '\0';
      아니면
        last_component = true;

      만약 (cur[0] == '\0')
        break;

      만약 (strcmp (cur, ".") == 0)
        goto next;
      아니면 만약 (strcmp (cur, ".."))
        depth++;
      아니면
        {
          만약 (depth)
            depth--;
          아니면
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              goto next;
            }
        }

      만약 (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          만약 (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              만약 (errno == ELOOP)
                반환 crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, NULL,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              만약 (ret < 0)
                반환 crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          만약 (do_open)
            반환 ret;

          close_and_replace (&wd_cleanup, ret);
          반환 0;
        }

      ret = mkdirat (cwd, cur, mode);
      만약 (ret < 0)
        {
          mode_t st_mode;

          만약 (errno != EEXIST)
            반환 crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          만약 (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            반환 crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : NULL, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      만약 (UNLIKELY (cwd < 0))
        반환 crun_error_wrap (err, "creating `/%s`", path);

      만약 (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, NULL);
          만약 (UNLIKELY (ret < 0))
            {
              정수 saved_errno = errno;

              close (cwd);
              반환 crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          만약 ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              반환 crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      만약 (it == NULL)
        break;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  만약 (do_open)
    {
      만약 (cwd == dirfd)
        {
          ret = dup (dirfd);
          만약 (UNLIKELY (ret < 0))
            반환 crun_make_error (err, errno, "dup `%s`", dirpath);
          반환 ret;
        }

      wd_cleanup = -1;
      반환 cwd;
    }

  반환 0;
}

정수
crun_safe_create_and_open_ref_at (bool dir, 정수 dirfd, const char *dirpath, const char *path, 정수 mode, libcrun_error_t *err)
{
  정수 ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  만약 (LIKELY (ret >= 0))
    반환 ret;

  crun_error_release (err);
  반환 crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

정수
crun_safe_ensure_directory_at (정수 dirfd, const char *dirpath, const char *path, 정수 mode,
                               libcrun_error_t *err)
{
  반환 crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

정수
crun_safe_ensure_file_at (정수 dirfd, const char *dirpath, const char *path, 정수 mode,
                          libcrun_error_t *err)
{
  반환 crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

정수
crun_ensure_directory (const char *path, 정수 mode, bool nofollow, libcrun_error_t *err)
{
  반환 crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

static 정수
get_file_size (정수 fd, off_t *size)
{
  struct stat st;
  정수 ret;
#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  만약 (UNLIKELY (ret < 0))
    {
      만약 (errno == ENOSYS || errno == EINVAL)
        goto fallback;
      반환 ret;
    }
  *size = stx.stx_size;

  반환 ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  반환 ret;
}

정수
crun_dir_p_at (정수 dirfd, const char *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  정수 ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "stat `%s`", path);

  반환 S_ISDIR (mode) ? 1 : 0;
}

정수
crun_dir_p (const char *path, bool nofollow, libcrun_error_t *err)
{
  반환 crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
static 정수
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  size_t len;
  정수 ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  만약 (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      만약 (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          반환 0;
        }
      반환 ret;
    }

  반환 strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
static bool
running_in_openvz ()
{
  만약 (access ("/proc/vz", F_OK) < 0)
    반환 false;

  반환 access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

정수
check_running_in_user_namespace (libcrun_error_t *err)
{
  static atomic_int run_in_userns = -1;
  struct stat st;
  정수 ret;

  ret = run_in_userns;
  만약 (ret >= 0)
    반환 ret;

  ret = stat ("/proc/self/ns/user", &st);
  만약 (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      만약 (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          반환 run_in_userns;
        }

      만약 (! running_in_openvz ())
        {
          run_in_userns = 1;
          반환 run_in_userns;
        }
    }
  아니면 만약 (UNLIKELY (errno != ENOENT))
    반환 crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  run_in_userns = ret;
  반환 run_in_userns;
}

static size_t
get_page_size ()
{
  static atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  만약 (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  반환 (size_t) pagesize;
}

static 정수 selinux_enabled = -1;
static 정수 apparmor_enabled = -1;

정수
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free char *out = NULL;
  cleanup_close 정수 fd = -1;
  size_t len;
  정수 ret;

  만약 (selinux_enabled >= 0)
    반환 selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  만약 (UNLIKELY (fd < 0))
    반환 fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  반환 selinux_enabled;
}

정수
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close 정수 fd = -1;
  정수 size;
  char buf[1];

  만약 (apparmor_enabled >= 0)
    반환 apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  만약 (fd == -1)
    {
      만약 (errno == ENOENT)
        {
          apparmor_enabled = 0;
          반환 0;
        }

      반환 crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (read (fd, buf, sizeof (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  반환 apparmor_enabled;
}

static 정수
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  만약 (selinux_enabled < 0)
    반환 crun_make_error (err, 0, "SELinux is not initialized correctly");
  반환 selinux_enabled;
}

정수
add_selinux_mount_label (char **retlabel, const char *data, const char *label, const char *context_type, libcrun_error_t *err)
{
  정수 ret;

  ret = libcrun_is_selinux_enabled (err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  만약 (label && ret)
    {
      만약 (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      아니면
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      반환 0;
    }
  *retlabel = xstrdup (data);
  반환 0;
}

static const char *
lsm_attr_path (libcrun_container_t *container, const char *lsm, const char *fname, libcrun_error_t *err)
{
  cleanup_close 정수 attr_dirfd = -1;
  cleanup_close 정수 lsm_dirfd = -1;
  char *attr_path = NULL;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  만약 (UNLIKELY (attr_dirfd < 0))
    반환 NULL;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  만약 (lsm != NULL)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      만약 (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          반환 NULL;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  반환 attr_path;
}

static 정수
check_proc_super_magic (정수 fd, const char *path, libcrun_error_t *err)
{
  struct statfs sfs;

  정수 ret = fstatfs (fd, &sfs);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "statfs `%s`", path);

  만약 (sfs.f_type != PROC_SUPER_MAGIC)
    반환 crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  반환 0;
}

static 정수
set_security_attr (libcrun_container_t *container, const char *lsm, const char *fname, const char *data, libcrun_error_t *err)
{
  정수 ret;

  cleanup_free const char *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close 정수 fd = -1;

  만약 (UNLIKELY (attr_path == NULL))
    반환 -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  만약 (UNLIKELY (fd < 0))
    반환 fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (write (fd, data, strlen (data)));
  만약 (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      만약 (errno == ENOENT && lsm != NULL && strcmp (lsm, "apparmor") == 0)
        {
          const char *profile = strchr (data, ' ');

          반환 crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      반환 crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  반환 0;
}

정수
set_selinux_label (libcrun_container_t *container, const char *label, bool now, libcrun_error_t *err)
{
  정수 ret;

  ret = libcrun_is_selinux_enabled (err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  만약 (ret)
    반환 set_security_attr (container, NULL, now ? "current" : "exec", label, err);
  반환 0;
}

static 정수
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  만약 (apparmor_enabled < 0)
    반환 crun_make_error (err, 0, "AppArmor is not initialized correctly");
  반환 apparmor_enabled;
}

static 정수
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (sizeof (UNCONFINED) - 1)
  cleanup_free const char *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close 정수 fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  char buf[UNCONFINED_LEN + 1];

  만약 (UNLIKELY (attr_path == NULL))
    반환 -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  만약 (UNLIKELY (fd < 0))
    반환 fd;

  만약 (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    반환 -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (read (fd, buf, sizeof (buf)));
  만약 (UNLIKELY (bytes_read < 0))
    반환 crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  만약 (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    반환 1;
  만약 (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    반환 0;
  반환 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

정수
set_apparmor_profile (libcrun_container_t *container, const char *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  정수 ret;

  ret = libcrun_is_apparmor_enabled (err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  만약 (ret)
    {
      cleanup_free char *buf = NULL;
      ret = is_current_process_confined (container, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      반환 set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  반환 0;
}

static 정수
read_all_fd_internal (정수 fd, const char *description, char **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free char *buf = NULL;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  정수 ret;

  만약 (size_hint)
    allocated = size_hint;
  아니면
    {
      ret = get_file_size (fd, &size);
      만약 (UNLIKELY (ret < 0))
        반환 crun_make_error (err, errno, "stat `%s`", description);

      만약 (max_size && (size_t) size > max_size)
        반환 crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  동안 ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (read (fd, buf + nread, allocated - nread));
      만약 (UNLIKELY (ret < 0))
        반환 crun_make_error (err, errno, "read from file `%s`", description);

      만약 (ret == 0)
        break;

      nread += ret;

      만약 (max_size && nread > max_size)
        반환 crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      만약 (nread == allocated)
        {
          만약 (size)
            break;

          만약 (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  만약 (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      char *tmp = realloc (buf, nread + 1);
      만약 (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = NULL;
  만약 (len)
    *len = nread;
  반환 0;
}

정수
read_all_fd_with_size_hint (정수 fd, const char *description, char **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  반환 read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

정수
read_all_fd_with_max_size (정수 fd, const char *description, char **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  반환 read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

정수
read_all_file_at (정수 dirfd, const char *path, char **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close 정수 fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  만약 (UNLIKELY (fd < 0))
    반환 crun_make_error (err, errno, "open `%s`", path);

  반환 read_all_fd (fd, path, out, len, err);
}

정수
read_all_file (const char *path, char **out, size_t *len, libcrun_error_t *err)
{
  만약 (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  반환 read_all_file_at (AT_FDCWD, path, out, len, err);
}

정수
get_realpath_to_file (정수 dirfd, const char *path_name, char **absolute_path, libcrun_error_t *err)
{
  cleanup_close 정수 targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  만약 (UNLIKELY (targetfd < 0))
    반환 crun_make_error (err, errno, "open `%s`", path_name);
  아니면
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      만약 (UNLIKELY (len < 0))
        반환 crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  반환 0;
}

정수
open_unix_domain_client_socket (const char *path, 정수 dgram, libcrun_error_t *err)
{
  struct sockaddr_un addr = {};
  정수 ret;
  proc_fd_path_t name_buf;
  cleanup_close 정수 destfd = -1;
  cleanup_close 정수 fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  만약 (UNLIKELY (fd < 0))
    반환 crun_make_error (err, errno, "create UNIX socket");

  만약 (strlen (path) >= sizeof (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      만약 (UNLIKELY (destfd < 0))
        반환 crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  만약 (path_len >= sizeof (addr.sun_path))
    반환 crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (struct sockaddr *) &addr, sizeof (addr));
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  반환 ret;
}

정수
open_unix_domain_socket (const char *path, 정수 dgram, libcrun_error_t *err)
{
  struct sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  정수 ret;
  cleanup_close 정수 dirfd = -1;
  cleanup_close 정수 fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  만약 (UNLIKELY (fd < 0))
    반환 crun_make_error (err, errno, "create UNIX socket");

  만약 (strlen (path) >= sizeof (addr.sun_path))
    {
      cleanup_free char *dpath = xstrdup (path);
      cleanup_free char *bpath = xstrdup (path);
      const char *parent_dir = dirname (dpath);
      const char *base = basename (bpath);
      정수 n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      만약 (UNLIKELY (dirfd < 0))
        반환 crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, sizeof (addr.sun_path), "%s/%s", name_buf, base);
      만약 (n < 0 || (size_t) n >= sizeof (addr.sun_path))
        반환 crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  아니면
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (struct sockaddr *) &addr, sizeof (addr));
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "bind socket to `%s`", path);

  만약 (! dgram)
    {
      ret = listen (fd, 1);
      만약 (UNLIKELY (ret < 0))
        반환 crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  반환 ret;
}

정수
send_fd_to_socket (정수 server, 정수 fd, libcrun_error_t *err)
{
  반환 send_fd_to_socket_with_payload (server, fd, NULL, 0, err);
}

정수
send_fd_to_socket_with_payload (정수 server, 정수 fd, const char *payload, size_t payload_len, libcrun_error_t *err)
{
  정수 ret;
  struct cmsghdr *cmsg = NULL;
  struct iovec iov[2];
  struct msghdr msg = {};
  char ctrl_buf[CMSG_SPACE (1 + sizeof (정수))] = {};
  char data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = sizeof (data);

  만약 (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (sizeof (정수));
  msg.msg_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (sizeof (정수));

  *((정수 *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &msg, 0));
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "sendmsg");
  반환 0;
}

정수
receive_fd_from_socket_with_payload (정수 from, char *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close 정수 fd = -1;
  정수 ret;
  struct iovec iov[1];
  struct msghdr msg = {};
  char ctrl_buf[CMSG_SPACE (sizeof (정수))] = {};
  char data[1];
  struct cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = sizeof (data);

  만약 (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (sizeof (정수));
  msg.msg_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &msg, 0));
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "recvmsg");
  만약 (UNLIKELY (ret == 0))
    반환 crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&msg);
  만약 (cmsg == NULL)
    반환 crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), sizeof (fd));

  ret = fd;
  fd = -1;
  반환 ret;
}

정수
receive_fd_from_socket (정수 from, libcrun_error_t *err)
{
  반환 receive_fd_from_socket_with_payload (from, NULL, 0, err);
}

정수
create_socket_pair (정수 *pair, libcrun_error_t *err)
{
  정수 ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "socketpair");
  반환 0;
}

정수
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  정수 ret = signalfd (-1, mask, 0);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "signalfd");
  반환 ret;
}

static 정수
epoll_helper_toggle (정수 epollfd, 정수 fd, 정수 events, libcrun_error_t *err)
{
  struct epoll_event ev = {};
  bool add = events != 0;
  정수 ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  만약 (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  만약 (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      만약 (errno == ENOENT || errno == EPERM)
        반환 0;
      반환 crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  반환 0;
}

정수
epoll_helper (정수 *in_fds, 정수 *in_levelfds, 정수 *out_fds, 정수 *out_levelfds, libcrun_error_t *err)
{
  struct epoll_event ev;
  cleanup_close 정수 epollfd = -1;
  정수 ret;

  정수 *it;
  epollfd = epoll_create1 (0);
  만약 (UNLIKELY (epollfd < 0))
    반환 crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  위해 (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      만약 (UNLIKELY (ret < 0))                                           \
        반환 crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  만약 (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  만약 (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  만약 (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  만약 (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  반환 ret;
}

정수
copy_from_fd_to_fd (정수 src, 정수 dst, 정수 consume, libcrun_error_t *err)
{
  정수 ret;
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
      만약 (can_copy_file_range)
        {
          nread = copy_file_range (src, NULL, dst, NULL, pagesize, 0);
          만약 (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              goto fallback;
            }
          만약 (consume && nread < 0 && errno == EAGAIN)
            반환 0;
          만약 (nread < 0 && errno == EIO)
            반환 0;
          만약 (UNLIKELY (nread < 0))
            반환 crun_make_error (err, errno, "copy_file_range");
          continue;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (read (src, buffer, pagesize));
      만약 (consume && nread < 0 && errno == EAGAIN)
        반환 0;
      만약 (nread < 0 && errno == EIO)
        반환 0;
      만약 (UNLIKELY (nread < 0))
        반환 crun_make_error (err, errno, "read");

      remaining = nread;
      동안 (remaining)
        {
          ret = TEMP_FAILURE_RETRY (write (dst, buffer + nread - remaining, remaining));
          만약 (UNLIKELY (ret < 0))
            반환 crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } 동안 (consume && nread);

  반환 0;
}

정수
run_process (char **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  만약 (UNLIKELY (pid < 0))
    반환 crun_make_error (err, errno, "fork");
  만약 (pid)
    {
      정수 r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      만약 (r < 0)
        반환 crun_make_error (err, errno, "waitpid");
      만약 (WIFEXITED (status) || WIFSIGNALED (status))
        반환 WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
static unsigned
atou (char **s)
{
  unsigned x;
  위해 (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  반환 x;
}

정수
fgetpwent_r (FILE *f, struct passwd *pw, char *line, size_t size, struct passwd **res)
{
  char *s;
  정수 rv = 0;
  위해 (;;)
    {
      line[size - 1] = '\xff';
      만약 ((fgets (line, size, f) == NULL) || ferror (f) || line[size - 1] != '\xff')
        {
          rv = (line[size - 1] != '\xff') ? ERANGE : ENOENT;
          line = 0;
          pw = 0;
          break;
        }
      line[strcspn (line, "\n")] = 0;

      s = line;
      pw->pw_name = s++;
      만약 (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_passwd = s;
      만약 (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      만약 (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      만약 (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gecos = s;
      만약 (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_dir = s;
      만약 (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_shell = s;
      break;
    }
  *res = pw;
  만약 (rv)
    errno = rv;
  반환 rv;
}
#endif

정수
set_home_env (uid_t id)
{
  struct passwd pwd;
  cleanup_free char *buf = NULL;
  long buf_size;
  cleanup_file FILE *stream = NULL;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  만약 (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  만약 (stream == NULL)
    반환 -1;

  위해 (;;)
    {
      struct passwd *ret_pw = NULL;

      정수 ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      만약 (UNLIKELY (ret != 0))
        {
          만약 (ret == EINTR)
            continue;

          만약 (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              반환 -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          continue;
        }

      만약 (ret_pw && ret_pw->pw_uid == id)
        {
          만약 (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          아니면
            반환 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
static 정수
getsubidrange (uid_t id, 정수 is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *input = NULL;
  cleanup_free char *lineptr = NULL;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free char *buf = NULL;
  const char *name;
  struct passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  만약 (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  위해 (;;)
    {
      정수 ret;
      struct passwd *ret_pw = NULL;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      만약 (LIKELY (ret == 0))
        {
          만약 (ret_pw)
            {
              name = ret_pw->pw_name;
              break;
            }
          반환 -1;
        }

      만약 (ret != ERANGE)
        반환 -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  input = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  만약 (input == NULL)
    반환 -1;

  위해 (;;)
    {
      char *endptr;
      ssize_t read = getline (&lineptr, &lenlineptr, input);
      만약 (read < 0)
        반환 -1;

      만약 (read < (ssize_t) (len_name + 2))
        continue;

      만약 (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        continue;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      만약 (endptr >= &lineptr[read])
        반환 -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      반환 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

정수
format_default_id_mapping (char **out, uid_t container_id, uid_t host_uid, uid_t host_id, 정수 is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free char *buffer = NULL;
  정수 written = 0;
  정수 ret, remaining;

  *out = NULL;

  만약 (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    반환 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  만약 (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      만약 (UNLIKELY (ret >= remaining))
        반환 crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  만약 (UNLIKELY (ret >= remaining))
    반환 crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  만약 (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      만약 (UNLIKELY (ret >= remaining))
        반환 crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = NULL;
  반환 written;
}

static 정수
unset_cloexec_flag (정수 fd)
{
  정수 flags = fcntl (fd, F_GETFD);
  만약 (flags == -1)
    반환 -1;

  flags &= ~FD_CLOEXEC;

  반환 fcntl (fd, F_SETFD, flags);
}

static void __attribute__ ((__noreturn__))
run_process_child (char *path, char **args, const char *cwd, char **envp, 정수 pipe_r,
                   정수 pipe_w, 정수 out_fd, 정수 err_fd, bool can_ignore_chdir_errors)
{
  char *tmp_args[] = { path, NULL };
  libcrun_error_t err = NULL;
  정수 dev_null_fd = -1;
  정수 ret;

  ret = mark_or_close_fds_ge_than (NULL, 3, false, &err);
  만약 (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->msg);

  만약 (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      만약 (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  만약 (out_fd >= 0)
    unset_cloexec_flag (1);
  만약 (err_fd >= 0)
    unset_cloexec_flag (2);

  만약 (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  만약 (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  만약 (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  만약 (args == NULL)
    args = tmp_args;

  만약 (cwd && chdir (cwd) < 0)
    {
      만약 (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
정수
run_process_with_stdin_timeout_envp (char *path, char **args, const char *cwd, 정수 timeout,
                                     char **envp, char *stdin, size_t stdin_len, 정수 out_fd,
                                     정수 err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  정수 stdin_pipe[2];
  pid_t pid;
  정수 ret;
  cleanup_close 정수 pipe_r = -1;
  cleanup_close 정수 pipe_w = -1;
  sigset_t oldmask, mask;
  정수 r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  만약 (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      만약 (UNLIKELY (ret < 0))
        반환 crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  만약 (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      goto restore_sig_mask_and_exit;
    }

  만약 (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (write (pipe_w, stdin, stdin_len));
  만약 (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      만약 (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          goto restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  만약 (timeout)
    {
      time_t start = time (NULL);
      time_t now;
      위해 (now = start; now - start < timeout; now = time (NULL))
        {
          siginfo_t info;
          정수 elapsed = now - start;
          struct timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          만약 (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              goto restore_sig_mask_and_exit;
            }

          만약 (info.si_signo == SIGCHLD && info.si_pid == pid)
            goto read_waitpid;

          만약 (ret < 0 && errno == EAGAIN)
            goto timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      goto restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  만약 (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  아니면
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  만약 (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      만약 (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, NULL);
      만약 (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  반환 ret;
}

정수
mark_or_close_fds_ge_than (libcrun_container_t *container, 정수 n, bool close_now, libcrun_error_t *err)
{
  cleanup_close 정수 cfd = -1;
  cleanup_dir DIR *dir = NULL;
  정수 ret;
  정수 fd;
  struct dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  만약 (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  만약 (ret == 0)
    {
      만약 (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      반환 0;
    }
  만약 (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    반환 crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  만약 (UNLIKELY (cfd < 0))
    반환 cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  dir = fdopendir (cfd);
  만약 (UNLIKELY (dir == NULL))
    반환 crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  위해 (next = readdir (dir); next; next = readdir (dir))
    {
      정수 val;
      const char *name = next->d_name;
      만약 (name[0] == '.')
        continue;

      val = strtoll (name, NULL, 10);
      만약 (val < n || val == fd)
        continue;

      만약 (close_now)
        {
          ret = close (val);
          만약 (UNLIKELY (ret < 0))
            반환 crun_make_error (err, errno, "close fd `%d`", val);

          만약 (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      아니면
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          만약 (UNLIKELY (ret < 0))
            반환 crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  반환 0;
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

  (void) snprintf (out, len, "%s.%06lldZ", timestamp, (long long 정수) tv.tv_usec);
  out[len - 1] = '\0';
}

정수
set_blocking_fd (정수 fd, bool blocking, libcrun_error_t *err)
{
  정수 ret, flags = fcntl (fd, F_GETFL, 0);
  만약 (UNLIKELY (flags < 0))
    반환 crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "fcntl");
  반환 0;
}

정수
parse_json_file (json_object **out, const char *jsondata, struct parser_context *ctx arg_unused, libcrun_error_t *err)
{
  struct json_tokener *tok;
  enum json_tokener_error jerr;

  *err = NULL;

  만약 (UNLIKELY (jsondata == NULL))
    반환 crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  만약 (UNLIKELY (tok == NULL))
    반환 crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (정수) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  만약 (*out == NULL || jerr != json_tokener_success)
    {
      정수 saved_pos = (정수) tok->char_offset;

      json_tokener_free (tok);
      만약 (*out)
        {
          json_object_put (*out);
          *out = NULL;
        }
      반환 crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  반환 0;
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
static 정수
check_access (const char *path)
{
  정수 ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#아니면
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  만약 (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      만약 (errno == EACCES)
        {
          정수 saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          만약 (ret == 0)
            반환 CHECK_ACCESS_NOT_EXECUTABLE;
        }
      반환 -errno;
    }

  ret = get_file_type (&mode, false, path);
  만약 (UNLIKELY (ret < 0))
    반환 -errno;

  만약 (! S_ISREG (mode))
    반환 CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  반환 0;
}

정수
find_executable (char **out, const char *executable_path, const char *cwd, libcrun_error_t *err)
{
  cleanup_free char *cwd_executable_path = NULL;
  cleanup_free char *tmp = NULL;
  정수 last_error = -ENOENT;
  char *it, *end;
  정수 ret;

  *out = NULL;

  만약 (executable_path == NULL || executable_path[0] == '\0')
    반환 crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  만약 (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free char *cwd_allocated = NULL;

      만약 (cwd == NULL)
        {
          cwd_allocated = getcwd (NULL, 0);
          만약 (cwd_allocated == NULL)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, NULL);
      만약 (UNLIKELY (ret < 0))
        반환 ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  만약 (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      만약 (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          반환 0;
        }
      last_error = ret;
      goto fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  동안 ((it = strsep (&end, ":")))
    {
      cleanup_free char *path = NULL;

      만약 (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, NULL);
      만약 (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          continue;
        }

      ret = check_access (path);
      만약 (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = NULL;
          반환 0;
        }

      만약 (ret == -ENOENT)
        continue;

      last_error = ret;
    }

fail:
  switch (last_error)
    {
    case CHECK_ACCESS_NOT_EXECUTABLE:
      반환 crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    case CHECK_ACCESS_NOT_REGULAR:
      반환 crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    default:
      errno = -last_error;
      만약 (errno == ENOENT)
        반환 crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      반환 crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

static ssize_t
safe_read_xattr (char **ret, 정수 sfd, const char *srcname, const char *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  동안 (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      만약 (UNLIKELY (s < 0))
        반환 crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      만약 (s < current_size)
        break;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  만약 (s <= 0)
    반환 s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = NULL;

  반환 s;
}

static ssize_t
copy_xattr (정수 sfd, 정수 dfd, const char *srcname, const char *destname, libcrun_error_t *err)
{
  cleanup_free char *buf = NULL;
  ssize_t xattr_len;
  char *it;

  xattr_len = flistxattr (sfd, NULL, 0);
  만약 (UNLIKELY (xattr_len < 0))
    {
      만약 (errno == ENOTSUP)
        반환 0;

      반환 crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  만약 (xattr_len == 0)
    반환 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  만약 (UNLIKELY (xattr_len < 0))
    반환 crun_make_error (err, errno, "flistxattr `%s`", srcname);

  위해 (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free char *v = NULL;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      만약 (UNLIKELY (s < 0))
        반환 s;

      s = fsetxattr (dfd, it, v, s, 0);
      만약 (UNLIKELY (s < 0))
        {
          만약 (errno == EINVAL || errno == EOPNOTSUPP)
            continue;

          반환 crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  반환 0;
}

#endif

static 정수
copy_rec_stat_file_at (정수 dfd, const char *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  struct stat st;
  정수 ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  만약 (UNLIKELY (ret < 0))
    {
      만약 (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      반환 ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  반환 ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  반환 ret;
}

정수
copy_recursive_fd_to_fd (정수 srcdirfd, 정수 dfd, const char *srcname, const char *destname, libcrun_error_t *err)
{
  cleanup_close 정수 destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = NULL;
  struct dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  만약 (UNLIKELY (dsrcfd == NULL))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      반환 crun_make_error (err, errno, "open directory `%s`", destname);
    }

  위해 (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close 정수 srcfd = -1;
      cleanup_close 정수 destfd = -1;
      cleanup_free char *target_buf = NULL;
      정수 ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      만약 (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      만약 (UNLIKELY (ret < 0))
        반환 crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      switch (mode & S_IFMT)
        {
        case S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          만약 (UNLIKELY (srcfd < 0))
            반환 crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          만약 (UNLIKELY (destfd < 0))
            반환 crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;

#ifdef HAVE_FGETXATTR
          ret = (정수) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          break;

        case S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          만약 (UNLIKELY (ret < 0))
            반환 crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          만약 (UNLIKELY (srcfd < 0))
            반환 crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          만약 (UNLIKELY (destfd < 0))
            반환 crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (정수) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          만약 (UNLIKELY (ret < 0))
            반환 ret;
          break;

        case S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          만약 (UNLIKELY (ret < 0))
            반환 crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          break;

        case S_IFBLK:
        case S_IFCHR:
        case S_IFIFO:
        case S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          만약 (UNLIKELY (ret < 0))
            반환 crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          break;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      만약 (UNLIKELY (ret < 0))
        반환 crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      만약 (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          만약 (errno == ENOTSUP)
            continue;

          반환 crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  반환 0;
}

const char *
find_annotation (libcrun_container_t *container, const char *name)
{
  만약 (container->container_def->annotations == NULL)
    반환 NULL;

  반환 find_string_map_value (container->annotations, name);
}

정수
safe_write (정수 fd, const char *fname, const void *buf, size_t count, libcrun_error_t *err)
{
  size_t written = 0;
  동안 (written < count)
    {
      ssize_t w = write (fd, (const char *) buf + written, count - written);
      만약 (UNLIKELY (w < 0))
        {
          만약 (errno == EINTR || errno == EAGAIN)
            continue;
          반환 crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  반환 0;
}

#define MAX_PARTS 32

정수
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
  위해 (;;)
    {
      const char *part;
      size_t size;

      part = va_arg (ap, const char *);
      만약 (part == NULL)
        break;

      만약 (n_parts == MAX_PARTS)
        {
          va_end (ap);
          반환 crun_make_error (err, EINVAL, "too many paths specified");
        }

      만약 (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          동안 (part[0] == '/' && part[1] == '/')
            part++;
        }
      아니면
        {
          /* And drop any initial '/' for other components.  */
          동안 (part[0] == '/')
            part++;
        }

      size = strlen (part);
      만약 (size == 0)
        continue;

      동안 (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  위해 (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  위해 (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      만약 (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  반환 0;
}

#만약 __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#아니면
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
static 정수
b64_input (char c)
{
  const char table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  정수 i;

  위해 (i = 0; i < 64; i++)
    {
      만약 (table[i] == c)
        반환 i;
    }
  반환 -1;
}

정수
base64_decode (const char *iptr, size_t isize, char *optr, size_t osize, size_t *nbytes)
{
  정수 i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned char data[4];

  *nbytes = 0;
  동안 (consumed < isize && (*nbytes) + 3 < osize)
    {
      동안 ((i < 4) && (consumed < isize))
        {
          tmp = b64_input (*iptr++);
          consumed++;
          만약 (tmp != -1)
            {
              만약 (UNLIKELY (pad > 0))
                반환 -1;
              data[i++] = tmp;
            }
          아니면 만약 (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      만약 (i == 4)
        {
          만약 (UNLIKELY (pad > 2))
            반환 -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      아니면
        {
          /* I did not get all the data.  */
          consumed -= i;
          반환 consumed;
        }
      i = 0;
    }
  반환 consumed;
}

char *
get_user_name (uid_t uid)
{
  struct passwd pd;
  struct passwd *temp_result_ptr;
  char pwdbuffer[200];
  만약 (! getpwuid_r (uid, &pd, pwdbuffer, sizeof (pwdbuffer), &temp_result_ptr))
    반환 xstrdup (pd.pw_name);
  반환 xstrdup ("");
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
  만약 (sep == NULL || *sep == '/')
    반환 xstrdup (key);

  ret = xstrdup (key);
  위해 (it = ret; *it; it++)
    {
      만약 (*it == '.')
        *it = '/';
      아니면 만약 (*it == '/')
        *it = '.';
    }

  반환 ret;
}

정수
has_suffix (const char *str, const char *suffix)
{
  만약 (! str || ! suffix)
    반환 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  만약 (lensuffix > lenstr)
    반환 0;
  반환 memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

char *
str_join_array (정수 offset, size_t size, char *const array[], const char *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = NULL;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  char *result, *p;

  lens = xmalloc (size * sizeof (*lens));

  위해 (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  위해 (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      만약 (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  반환 result;
}

정수
libcrun_mmap (struct libcrun_mmap_s **ret, void *addr, size_t length,
              정수 prot, 정수 flags, 정수 fd, off_t offset,
              libcrun_error_t *err)
{
  struct libcrun_mmap_s *mmap_s = NULL;

  void *mapped = mmap (addr, length, prot, flags, fd, offset);
  만약 (mapped == MAP_FAILED)
    반환 crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (sizeof (struct libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  반환 0;
}

정수
libcrun_munmap (struct libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  정수 ret;

  ret = munmap (mmap->addr, mmap->length);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "munmap");

  free (mmap);

  반환 0;
}

static long
read_file_as_long_or_default (const char *path, long def_value)
{
  cleanup_free char *content = NULL;
  libcrun_error_t tmp_err = NULL;
  char *endptr;
  long val;
  정수 ret;

  ret = read_all_file (path, &content, NULL, &tmp_err);
  만약 (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      반환 def_value;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  만약 (UNLIKELY (errno))
    반환 def_value;
  만약 (endptr == content || (*endptr && *endptr != '\n'))
    반환 def_value;
  반환 val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (void)
{
  static atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  만약 (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  반환 uid;
}

gid_t
get_overflow_gid (void)
{
  static atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  만약 (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  반환 gid;
}

void
consume_trailing_slashes (char *path)
{
  만약 (! path || path[0] == '\0')
    반환;

  char *last = path + strlen (path);

  동안 (last > path && *(last - 1) == '/')
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
  만약 (UNLIKELY (dir == NULL))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      반환 NULL;
    }

  entries = xmalloc (entries_size * sizeof (char *));
  동안 ((de = readdir (dir)))
    {
      만약 (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;
      만약 (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * sizeof (char *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * sizeof (char *));
  entries[n_entries] = NULL;

  반환 entries;
}

정수
cpuset_string_to_bitmask (const char *str, char **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free char *mask = NULL;
  size_t mask_size = 0;
  const char *p = str;
  char *endptr;

  동안 (*p)
    {
      long long start_range, end_range;

      만약 (*p < '0' || *p > '9')
        goto invalid_input;

      start_range = strtoll (p, &endptr, 10);
      만약 (start_range < 0)
        goto invalid_input;

      p = endptr;

      만약 (*p != '-')
        end_range = start_range;
      아니면
        {
          p++;

          만약 (*p < '0' || *p > '9')
            goto invalid_input;

          end_range = strtoll (p, &endptr, 10);

          만약 (end_range < start_range)
            goto invalid_input;

          p = endptr;
        }

      /* Just set some limit.  */
      만약 (end_range > (1 << 20))
        goto invalid_input;

      만약 (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      위해 (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      만약 (*p == ',')
        p++;
      아니면 만약 (*p)
        goto invalid_input;
    }

  *out = mask;
  mask = NULL;
  *out_size = mask_size;

  반환 0;

invalid_input:
  반환 crun_make_error (err, 0, "cannot parse input `%s`", str);
}

struct channel_fd_pair
{
  struct ring_buffer *rb;

  정수 in_fd;
  정수 out_fd;

  정수 infd_epoll_events;
  정수 outfd_epoll_events;
};

struct channel_fd_pair *
channel_fd_pair_new (정수 in_fd, 정수 out_fd, size_t size)
{
  struct channel_fd_pair *channel = xmalloc (sizeof (struct channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  반환 channel;
}

void
channel_fd_pair_free (struct channel_fd_pair *channel)
{
  만약 (channel == NULL)
    반환;

  ring_buffer_free (channel->rb);
  free (channel);
}

정수
channel_fd_pair_process (struct channel_fd_pair *channel, 정수 epollfd, libcrun_error_t *err)
{
  bool is_input_eagain = false, is_output_eagain = false, repeat;
  정수 ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  위해 (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      만약 (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_input_eagain, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
          만약 (ret > 0)
            repeat = true;
        }
      만약 (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
          만약 (ret > 0)
            repeat = true;
        }
    }

  만약 (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      정수 events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_input_eagain ? EPOLLET : 0)) : 0;
      만약 (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      만약 (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
          channel->outfd_epoll_events = events;
        }
    }
  반환 0;
}

static 정수
open_proc (libcrun_error_t *err)
{
  정수 fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close 정수 fsfd = -1;
  정수 ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  만약 (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
      만약 (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          만약 (fd >= 0)
            반환 fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  만약 (fd < 0)
    반환 crun_make_error (err, errno, "open `/proc`");
  반환 fd;
}

정수
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  만약 (! container)
    반환 open_proc (err);

  만약 (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  반환 container->proc_fd;
}

정수
libcrun_open_proc_file (libcrun_container_t *container, const char *path, 정수 flags, libcrun_error_t *err)
{
  정수 proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  만약 (proc_fd < 0)
    반환 proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  만약 (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  만약 (UNLIKELY (fd < 0))
    반환 crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  반환 fd;
}

정수
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const char *path, 정수 flags, libcrun_error_t *err)
{
  cleanup_free char *full_path = NULL;
  xasprintf (&full_path, "%d/%s", pid, path);
  반환 libcrun_open_proc_file (container, full_path, flags, err);
}
