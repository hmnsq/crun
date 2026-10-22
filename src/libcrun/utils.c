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
#якщо HAVE_STDATOMIC_H
#  include <stdatomic.h>
#інакше
#  define atomic_long volatile long
#  define atomic_int volatile ціле
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

ціле
crun_path_exists (const char *path, libcrun_error_t *err)
{
  ціле ret = access (path, F_OK);
  якщо (ret < 0)
    {
      якщо (errno == ENOENT)
        повернути 0;
      повернути crun_make_error (err, errno, "access `%s`", path);
    }
  повернути 1;
}

ціле
xasprintf (char **str, const char *fmt, ...)
{
  ціле ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  якщо (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  повернути ret;
}

ціле
write_file_at_with_flags (ціле dirfd, ціле flags, mode_t mode, const char *name, const void *data, size_t len, libcrun_error_t *err)
{
  cleanup_close ціле fd = -1;
  ціле ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  якщо (UNLIKELY (fd < 0))
    повернути crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути (len > INT_MAX) ? INT_MAX : (ціле) len;
}

ціле
detach_process ()
{
  pid_t pid;
  якщо (setsid () < 0)
    повернути -1;
  pid = fork ();
  якщо (pid < 0)
    повернути -1;
  якщо (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  повернути 0;
}

ціле
get_file_type_fd (ціле fd, mode_t *mode)
{
  struct stat st;
  ціле ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  якщо (UNLIKELY (ret < 0))
    {
      якщо (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      повернути ret;
    }
  *mode = stx.stx_mode;
  повернути ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  повернути ret;
}

ціле
get_file_type_at (ціле dirfd, mode_t *mode, bool nofollow, const char *path)
{
  ціле empty_path = path == NULL ? AT_EMPTY_PATH : 0;
  struct stat st;
  ціле ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  якщо (UNLIKELY (ret < 0))
    {
      якщо (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      повернути ret;
    }
  *mode = stx.stx_mode;
  повернути ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  повернути ret;
}

ціле
get_file_type (mode_t *mode, bool nofollow, const char *path)
{
  повернути get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

ціле
create_file_if_missing_at (ціле dirfd, const char *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close ціле fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  якщо (fd_write < 0)
    {
      ціле saved_errno = errno;
      mode_t tmp_mode;
      ціле ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      якщо (ret == 0 && S_ISREG (tmp_mode))
        повернути 0;

      повернути crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  повернути 0;
}

static ціле
ensure_directory_internal_at (ціле dirfd, char *path, size_t len, ціле mode, libcrun_error_t *err)
{
  char *it = path + len;
  ціле ret = 0;
  bool parent_created = false;

  для (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      якщо (ret == 0 || errno == EEXIST)
        повернути 0;

      ціле saved_errno = errno;
      якщо (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = NULL;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          якщо (ret > 0)
            повернути 0;
          якщо (ret < 0)
            crun_error_release (&tmp_err);

          повернути crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      поки (it > path && *it != '/')
        {
          it--;
          len--;
        }
      якщо (it == path)
        повернути 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      parent_created = true;
    }
  повернути ret;
}

ціле
crun_ensure_directory_at (ціле dirfd, const char *path, ціле mode, bool nofollow, libcrun_error_t *err)
{
  ціле ret;
  cleanup_free char *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (ret == 0)
    повернути crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  повернути 0;
}

static ціле
check_fd_is_path (const char *path, ціле fd, const char *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  char link[PATH_MAX];
  ціле ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, sizeof (link)));
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "readlink `%s`", fdname);

  якщо (((size_t) ret) != path_len || memcmp (link, path, path_len))
    повернути crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  повернути 0;
}

static ціле
check_fd_under_path (const char *rootfs, size_t rootfslen, ціле fd, const char *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  char link[PATH_MAX];
  ціле ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  якщо (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    повернути 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, sizeof (link)));
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "readlink `%s`", fdname);

  якщо (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    повернути crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  повернути 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
static void
close_and_replace (ціле *oldfd, ціле newfd)
{
  якщо (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
char *chroot_realpath (const char *chroot, const char *path, char resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
static ціле
safe_openat_fallback (ціле dirfd, const char *rootfs, const char *path, ціле flags,
                      ціле mode, libcrun_error_t *err)
{
  cleanup_free char *parent_path = NULL;
  const char *last_component = NULL;
  const char *orig_path = path;
  const char *path_in_chroot;
  cleanup_close ціле fd = -1;
  char resolved[PATH_MAX];
  char buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  ціле ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  якщо (flags & O_NOFOLLOW)
    {
      char *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      якщо (sep == NULL)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      інакше якщо (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      якщо (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  якщо (path_in_chroot == NULL)
    повернути crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  якщо (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  якщо (last_component)
    {
      ret = snprintf (resolved, sizeof (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      якщо (UNLIKELY (ret >= (ціле) sizeof (resolved)))
        повернути crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  якщо (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      повернути ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = fd;
  fd = -1;
  повернути ret;
}

ціле
safe_openat (ціле dirfd, const char *rootfs, const char *path, ціле flags, ціле mode,
             libcrun_error_t *err)
{
  static bool openat2_supported = true;
  ціле retries = MAX_OPENAT2_EAGAIN_RETRIES;
  ціле ret;

  якщо (is_empty_string (path))
    {
      cleanup_close ціле fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      якщо (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      якщо (UNLIKELY (fd < 0))
        повернути crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      якщо (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
        }

      ret = fd;
      fd = -1;
      повернути ret;
    }

  якщо (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_ROOT);
      якщо (UNLIKELY (ret < 0))
        {
          якщо (errno == EINTR)
            goto repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          якщо (errno == EAGAIN && retries-- > 0)
            goto repeat;
          якщо (errno == ENOSYS)
            openat2_supported = false;
          якщо (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            повернути safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          повернути crun_make_error (err, errno, "openat2 `%s`", path);
        }

      повернути ret;
    }

  повернути safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (ціле dfd, const char *name, char **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free char *tmp_buf = NULL;
  ssize_t size;

  do
    {
      якщо (tmp_buf != NULL)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      якщо (UNLIKELY (size < 0))
        повернути crun_make_error (err, errno, "readlink `%s`", name);
  } поки (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = NULL;

  повернути size;
}

static ціле crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, ціле dirfd, const char *dirpath,
                                const char *path, ціле mode, ціле max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
static ціле
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, ціле dirfd, const char *dirpath, ціле cwd,
                                 const char *npath, const char *cur, const char *rest,
                                 ціле mode, ціле max_readlinks, libcrun_error_t *err)
{
  cleanup_free char *target = NULL;
  cleanup_free char *new_path = NULL;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (ціле) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  повернути crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

static ціле
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, ціле dirfd, const char *dirpath,
                     const char *path, ціле mode, ціле max_readlinks, libcrun_error_t *err)
{
  cleanup_close ціле wd_cleanup = -1;
  cleanup_free char *npath = NULL;
  bool last_component = false;
  size_t depth = 0;
  char *cur;
  char *it;
  ціле cwd;
  ціле ret;

  якщо (max_readlinks <= 0)
    повернути crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  якщо (*path == '\0')
    {
      якщо (do_open)
        повернути open (dirpath, O_CLOEXEC | O_PATH, 0);
      повернути 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  поки (cur)
    {
      якщо (it)
        *it = '\0';
      інакше
        last_component = true;

      якщо (cur[0] == '\0')
        break;

      якщо (strcmp (cur, ".") == 0)
        goto next;
      інакше якщо (strcmp (cur, ".."))
        depth++;
      інакше
        {
          якщо (depth)
            depth--;
          інакше
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              goto next;
            }
        }

      якщо (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          якщо (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              якщо (errno == ELOOP)
                повернути crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, NULL,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              якщо (ret < 0)
                повернути crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          якщо (do_open)
            повернути ret;

          close_and_replace (&wd_cleanup, ret);
          повернути 0;
        }

      ret = mkdirat (cwd, cur, mode);
      якщо (ret < 0)
        {
          mode_t st_mode;

          якщо (errno != EEXIST)
            повернути crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          якщо (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            повернути crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : NULL, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      якщо (UNLIKELY (cwd < 0))
        повернути crun_error_wrap (err, "creating `/%s`", path);

      якщо (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, NULL);
          якщо (UNLIKELY (ret < 0))
            {
              ціле saved_errno = errno;

              close (cwd);
              повернути crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          якщо ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              повернути crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      якщо (it == NULL)
        break;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  якщо (do_open)
    {
      якщо (cwd == dirfd)
        {
          ret = dup (dirfd);
          якщо (UNLIKELY (ret < 0))
            повернути crun_make_error (err, errno, "dup `%s`", dirpath);
          повернути ret;
        }

      wd_cleanup = -1;
      повернути cwd;
    }

  повернути 0;
}

ціле
crun_safe_create_and_open_ref_at (bool dir, ціле dirfd, const char *dirpath, const char *path, ціле mode, libcrun_error_t *err)
{
  ціле ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  якщо (LIKELY (ret >= 0))
    повернути ret;

  crun_error_release (err);
  повернути crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ціле
crun_safe_ensure_directory_at (ціле dirfd, const char *dirpath, const char *path, ціле mode,
                               libcrun_error_t *err)
{
  повернути crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ціле
crun_safe_ensure_file_at (ціле dirfd, const char *dirpath, const char *path, ціле mode,
                          libcrun_error_t *err)
{
  повернути crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ціле
crun_ensure_directory (const char *path, ціле mode, bool nofollow, libcrun_error_t *err)
{
  повернути crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

static ціле
get_file_size (ціле fd, off_t *size)
{
  struct stat st;
  ціле ret;
#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  якщо (UNLIKELY (ret < 0))
    {
      якщо (errno == ENOSYS || errno == EINVAL)
        goto fallback;
      повернути ret;
    }
  *size = stx.stx_size;

  повернути ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  повернути ret;
}

ціле
crun_dir_p_at (ціле dirfd, const char *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  ціле ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "stat `%s`", path);

  повернути S_ISDIR (mode) ? 1 : 0;
}

ціле
crun_dir_p (const char *path, bool nofollow, libcrun_error_t *err)
{
  повернути crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
static ціле
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  size_t len;
  ціле ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  якщо (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      якщо (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          повернути 0;
        }
      повернути ret;
    }

  повернути strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
static bool
running_in_openvz ()
{
  якщо (access ("/proc/vz", F_OK) < 0)
    повернути false;

  повернути access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

ціле
check_running_in_user_namespace (libcrun_error_t *err)
{
  static atomic_int run_in_userns = -1;
  struct stat st;
  ціле ret;

  ret = run_in_userns;
  якщо (ret >= 0)
    повернути ret;

  ret = stat ("/proc/self/ns/user", &st);
  якщо (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      якщо (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          повернути run_in_userns;
        }

      якщо (! running_in_openvz ())
        {
          run_in_userns = 1;
          повернути run_in_userns;
        }
    }
  інакше якщо (UNLIKELY (errno != ENOENT))
    повернути crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  run_in_userns = ret;
  повернути run_in_userns;
}

static size_t
get_page_size ()
{
  static atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  якщо (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  повернути (size_t) pagesize;
}

static ціле selinux_enabled = -1;
static ціле apparmor_enabled = -1;

ціле
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free char *out = NULL;
  cleanup_close ціле fd = -1;
  size_t len;
  ціле ret;

  якщо (selinux_enabled >= 0)
    повернути selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  якщо (UNLIKELY (fd < 0))
    повернути fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  повернути selinux_enabled;
}

ціле
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close ціле fd = -1;
  ціле size;
  char buf[1];

  якщо (apparmor_enabled >= 0)
    повернути apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  якщо (fd == -1)
    {
      якщо (errno == ENOENT)
        {
          apparmor_enabled = 0;
          повернути 0;
        }

      повернути crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (read (fd, buf, sizeof (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  повернути apparmor_enabled;
}

static ціле
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  якщо (selinux_enabled < 0)
    повернути crun_make_error (err, 0, "SELinux is not initialized correctly");
  повернути selinux_enabled;
}

ціле
add_selinux_mount_label (char **retlabel, const char *data, const char *label, const char *context_type, libcrun_error_t *err)
{
  ціле ret;

  ret = libcrun_is_selinux_enabled (err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (label && ret)
    {
      якщо (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      інакше
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      повернути 0;
    }
  *retlabel = xstrdup (data);
  повернути 0;
}

static const char *
lsm_attr_path (libcrun_container_t *container, const char *lsm, const char *fname, libcrun_error_t *err)
{
  cleanup_close ціле attr_dirfd = -1;
  cleanup_close ціле lsm_dirfd = -1;
  char *attr_path = NULL;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  якщо (UNLIKELY (attr_dirfd < 0))
    повернути NULL;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  якщо (lsm != NULL)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      якщо (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          повернути NULL;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  повернути attr_path;
}

static ціле
check_proc_super_magic (ціле fd, const char *path, libcrun_error_t *err)
{
  struct statfs sfs;

  ціле ret = fstatfs (fd, &sfs);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "statfs `%s`", path);

  якщо (sfs.f_type != PROC_SUPER_MAGIC)
    повернути crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  повернути 0;
}

static ціле
set_security_attr (libcrun_container_t *container, const char *lsm, const char *fname, const char *data, libcrun_error_t *err)
{
  ціле ret;

  cleanup_free const char *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close ціле fd = -1;

  якщо (UNLIKELY (attr_path == NULL))
    повернути -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  якщо (UNLIKELY (fd < 0))
    повернути fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (write (fd, data, strlen (data)));
  якщо (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      якщо (errno == ENOENT && lsm != NULL && strcmp (lsm, "apparmor") == 0)
        {
          const char *profile = strchr (data, ' ');

          повернути crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      повернути crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  повернути 0;
}

ціле
set_selinux_label (libcrun_container_t *container, const char *label, bool now, libcrun_error_t *err)
{
  ціле ret;

  ret = libcrun_is_selinux_enabled (err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (ret)
    повернути set_security_attr (container, NULL, now ? "current" : "exec", label, err);
  повернути 0;
}

static ціле
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  якщо (apparmor_enabled < 0)
    повернути crun_make_error (err, 0, "AppArmor is not initialized correctly");
  повернути apparmor_enabled;
}

static ціле
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (sizeof (UNCONFINED) - 1)
  cleanup_free const char *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close ціле fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  char buf[UNCONFINED_LEN + 1];

  якщо (UNLIKELY (attr_path == NULL))
    повернути -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  якщо (UNLIKELY (fd < 0))
    повернути fd;

  якщо (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    повернути -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (read (fd, buf, sizeof (buf)));
  якщо (UNLIKELY (bytes_read < 0))
    повернути crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  якщо (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    повернути 1;
  якщо (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    повернути 0;
  повернути 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

ціле
set_apparmor_profile (libcrun_container_t *container, const char *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  ціле ret;

  ret = libcrun_is_apparmor_enabled (err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (ret)
    {
      cleanup_free char *buf = NULL;
      ret = is_current_process_confined (container, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      повернути set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  повернути 0;
}

static ціле
read_all_fd_internal (ціле fd, const char *description, char **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free char *buf = NULL;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  ціле ret;

  якщо (size_hint)
    allocated = size_hint;
  інакше
    {
      ret = get_file_size (fd, &size);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "stat `%s`", description);

      якщо (max_size && (size_t) size > max_size)
        повернути crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  поки ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (read (fd, buf + nread, allocated - nread));
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "read from file `%s`", description);

      якщо (ret == 0)
        break;

      nread += ret;

      якщо (max_size && nread > max_size)
        повернути crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      якщо (nread == allocated)
        {
          якщо (size)
            break;

          якщо (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  якщо (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      char *tmp = realloc (buf, nread + 1);
      якщо (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = NULL;
  якщо (len)
    *len = nread;
  повернути 0;
}

ціле
read_all_fd_with_size_hint (ціле fd, const char *description, char **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  повернути read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

ціле
read_all_fd_with_max_size (ціле fd, const char *description, char **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  повернути read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

ціле
read_all_file_at (ціле dirfd, const char *path, char **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close ціле fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  якщо (UNLIKELY (fd < 0))
    повернути crun_make_error (err, errno, "open `%s`", path);

  повернути read_all_fd (fd, path, out, len, err);
}

ціле
read_all_file (const char *path, char **out, size_t *len, libcrun_error_t *err)
{
  якщо (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  повернути read_all_file_at (AT_FDCWD, path, out, len, err);
}

ціле
get_realpath_to_file (ціле dirfd, const char *path_name, char **absolute_path, libcrun_error_t *err)
{
  cleanup_close ціле targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  якщо (UNLIKELY (targetfd < 0))
    повернути crun_make_error (err, errno, "open `%s`", path_name);
  інакше
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      якщо (UNLIKELY (len < 0))
        повернути crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  повернути 0;
}

ціле
open_unix_domain_client_socket (const char *path, ціле dgram, libcrun_error_t *err)
{
  struct sockaddr_un addr = {};
  ціле ret;
  proc_fd_path_t name_buf;
  cleanup_close ціле destfd = -1;
  cleanup_close ціле fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  якщо (UNLIKELY (fd < 0))
    повернути crun_make_error (err, errno, "create UNIX socket");

  якщо (strlen (path) >= sizeof (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      якщо (UNLIKELY (destfd < 0))
        повернути crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  якщо (path_len >= sizeof (addr.sun_path))
    повернути crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (struct sockaddr *) &addr, sizeof (addr));
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  повернути ret;
}

ціле
open_unix_domain_socket (const char *path, ціле dgram, libcrun_error_t *err)
{
  struct sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  ціле ret;
  cleanup_close ціле dirfd = -1;
  cleanup_close ціле fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  якщо (UNLIKELY (fd < 0))
    повернути crun_make_error (err, errno, "create UNIX socket");

  якщо (strlen (path) >= sizeof (addr.sun_path))
    {
      cleanup_free char *dpath = xstrdup (path);
      cleanup_free char *bpath = xstrdup (path);
      const char *parent_dir = dirname (dpath);
      const char *base = basename (bpath);
      ціле n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      якщо (UNLIKELY (dirfd < 0))
        повернути crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, sizeof (addr.sun_path), "%s/%s", name_buf, base);
      якщо (n < 0 || (size_t) n >= sizeof (addr.sun_path))
        повернути crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  інакше
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (struct sockaddr *) &addr, sizeof (addr));
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "bind socket to `%s`", path);

  якщо (! dgram)
    {
      ret = listen (fd, 1);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  повернути ret;
}

ціле
send_fd_to_socket (ціле server, ціле fd, libcrun_error_t *err)
{
  повернути send_fd_to_socket_with_payload (server, fd, NULL, 0, err);
}

ціле
send_fd_to_socket_with_payload (ціле server, ціле fd, const char *payload, size_t payload_len, libcrun_error_t *err)
{
  ціле ret;
  struct cmsghdr *cmsg = NULL;
  struct iovec iov[2];
  struct msghdr msg = {};
  char ctrl_buf[CMSG_SPACE (1 + sizeof (ціле))] = {};
  char data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = sizeof (data);

  якщо (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (sizeof (ціле));
  msg.msg_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (sizeof (ціле));

  *((ціле *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &msg, 0));
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "sendmsg");
  повернути 0;
}

ціле
receive_fd_from_socket_with_payload (ціле from, char *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close ціле fd = -1;
  ціле ret;
  struct iovec iov[1];
  struct msghdr msg = {};
  char ctrl_buf[CMSG_SPACE (sizeof (ціле))] = {};
  char data[1];
  struct cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = sizeof (data);

  якщо (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (sizeof (ціле));
  msg.msg_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &msg, 0));
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "recvmsg");
  якщо (UNLIKELY (ret == 0))
    повернути crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&msg);
  якщо (cmsg == NULL)
    повернути crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), sizeof (fd));

  ret = fd;
  fd = -1;
  повернути ret;
}

ціле
receive_fd_from_socket (ціле from, libcrun_error_t *err)
{
  повернути receive_fd_from_socket_with_payload (from, NULL, 0, err);
}

ціле
create_socket_pair (ціле *pair, libcrun_error_t *err)
{
  ціле ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "socketpair");
  повернути 0;
}

ціле
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  ціле ret = signalfd (-1, mask, 0);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "signalfd");
  повернути ret;
}

static ціле
epoll_helper_toggle (ціле epollfd, ціле fd, ціле events, libcrun_error_t *err)
{
  struct epoll_event ev = {};
  bool add = events != 0;
  ціле ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  якщо (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  якщо (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      якщо (errno == ENOENT || errno == EPERM)
        повернути 0;
      повернути crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  повернути 0;
}

ціле
epoll_helper (ціле *in_fds, ціле *in_levelfds, ціле *out_fds, ціле *out_levelfds, libcrun_error_t *err)
{
  struct epoll_event ev;
  cleanup_close ціле epollfd = -1;
  ціле ret;

  ціле *it;
  epollfd = epoll_create1 (0);
  якщо (UNLIKELY (epollfd < 0))
    повернути crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  для (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      якщо (UNLIKELY (ret < 0))                                           \
        повернути crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  якщо (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  якщо (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  якщо (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  якщо (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  повернути ret;
}

ціле
copy_from_fd_to_fd (ціле src, ціле dst, ціле consume, libcrun_error_t *err)
{
  ціле ret;
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
      якщо (can_copy_file_range)
        {
          nread = copy_file_range (src, NULL, dst, NULL, pagesize, 0);
          якщо (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              goto fallback;
            }
          якщо (consume && nread < 0 && errno == EAGAIN)
            повернути 0;
          якщо (nread < 0 && errno == EIO)
            повернути 0;
          якщо (UNLIKELY (nread < 0))
            повернути crun_make_error (err, errno, "copy_file_range");
          continue;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (read (src, buffer, pagesize));
      якщо (consume && nread < 0 && errno == EAGAIN)
        повернути 0;
      якщо (nread < 0 && errno == EIO)
        повернути 0;
      якщо (UNLIKELY (nread < 0))
        повернути crun_make_error (err, errno, "read");

      remaining = nread;
      поки (remaining)
        {
          ret = TEMP_FAILURE_RETRY (write (dst, buffer + nread - remaining, remaining));
          якщо (UNLIKELY (ret < 0))
            повернути crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } поки (consume && nread);

  повернути 0;
}

ціле
run_process (char **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  якщо (UNLIKELY (pid < 0))
    повернути crun_make_error (err, errno, "fork");
  якщо (pid)
    {
      ціле r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      якщо (r < 0)
        повернути crun_make_error (err, errno, "waitpid");
      якщо (WIFEXITED (status) || WIFSIGNALED (status))
        повернути WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
static unsigned
atou (char **s)
{
  unsigned x;
  для (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  повернути x;
}

ціле
fgetpwent_r (FILE *f, struct passwd *pw, char *line, size_t size, struct passwd **res)
{
  char *s;
  ціле rv = 0;
  для (;;)
    {
      line[size - 1] = '\xff';
      якщо ((fgets (line, size, f) == NULL) || ferror (f) || line[size - 1] != '\xff')
        {
          rv = (line[size - 1] != '\xff') ? ERANGE : ENOENT;
          line = 0;
          pw = 0;
          break;
        }
      line[strcspn (line, "\n")] = 0;

      s = line;
      pw->pw_name = s++;
      якщо (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_passwd = s;
      якщо (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      якщо (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      якщо (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gecos = s;
      якщо (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_dir = s;
      якщо (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_shell = s;
      break;
    }
  *res = pw;
  якщо (rv)
    errno = rv;
  повернути rv;
}
#endif

ціле
set_home_env (uid_t id)
{
  struct passwd pwd;
  cleanup_free char *buf = NULL;
  long buf_size;
  cleanup_file FILE *stream = NULL;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  якщо (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  якщо (stream == NULL)
    повернути -1;

  для (;;)
    {
      struct passwd *ret_pw = NULL;

      ціле ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      якщо (UNLIKELY (ret != 0))
        {
          якщо (ret == EINTR)
            continue;

          якщо (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              повернути -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          continue;
        }

      якщо (ret_pw && ret_pw->pw_uid == id)
        {
          якщо (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          інакше
            повернути 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
static ціле
getsubidrange (uid_t id, ціле is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *input = NULL;
  cleanup_free char *lineptr = NULL;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free char *buf = NULL;
  const char *name;
  struct passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  якщо (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  для (;;)
    {
      ціле ret;
      struct passwd *ret_pw = NULL;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      якщо (LIKELY (ret == 0))
        {
          якщо (ret_pw)
            {
              name = ret_pw->pw_name;
              break;
            }
          повернути -1;
        }

      якщо (ret != ERANGE)
        повернути -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  input = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  якщо (input == NULL)
    повернути -1;

  для (;;)
    {
      char *endptr;
      ssize_t read = getline (&lineptr, &lenlineptr, input);
      якщо (read < 0)
        повернути -1;

      якщо (read < (ssize_t) (len_name + 2))
        continue;

      якщо (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        continue;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      якщо (endptr >= &lineptr[read])
        повернути -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      повернути 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

ціле
format_default_id_mapping (char **out, uid_t container_id, uid_t host_uid, uid_t host_id, ціле is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free char *buffer = NULL;
  ціле written = 0;
  ціле ret, remaining;

  *out = NULL;

  якщо (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    повернути 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  якщо (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      якщо (UNLIKELY (ret >= remaining))
        повернути crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  якщо (UNLIKELY (ret >= remaining))
    повернути crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  якщо (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      якщо (UNLIKELY (ret >= remaining))
        повернути crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = NULL;
  повернути written;
}

static ціле
unset_cloexec_flag (ціле fd)
{
  ціле flags = fcntl (fd, F_GETFD);
  якщо (flags == -1)
    повернути -1;

  flags &= ~FD_CLOEXEC;

  повернути fcntl (fd, F_SETFD, flags);
}

static void __attribute__ ((__noreturn__))
run_process_child (char *path, char **args, const char *cwd, char **envp, ціле pipe_r,
                   ціле pipe_w, ціле out_fd, ціле err_fd, bool can_ignore_chdir_errors)
{
  char *tmp_args[] = { path, NULL };
  libcrun_error_t err = NULL;
  ціле dev_null_fd = -1;
  ціле ret;

  ret = mark_or_close_fds_ge_than (NULL, 3, false, &err);
  якщо (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->msg);

  якщо (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      якщо (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  якщо (out_fd >= 0)
    unset_cloexec_flag (1);
  якщо (err_fd >= 0)
    unset_cloexec_flag (2);

  якщо (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  якщо (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  якщо (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  якщо (args == NULL)
    args = tmp_args;

  якщо (cwd && chdir (cwd) < 0)
    {
      якщо (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
ціле
run_process_with_stdin_timeout_envp (char *path, char **args, const char *cwd, ціле timeout,
                                     char **envp, char *stdin, size_t stdin_len, ціле out_fd,
                                     ціле err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  ціле stdin_pipe[2];
  pid_t pid;
  ціле ret;
  cleanup_close ціле pipe_r = -1;
  cleanup_close ціле pipe_w = -1;
  sigset_t oldmask, mask;
  ціле r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  якщо (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  якщо (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      goto restore_sig_mask_and_exit;
    }

  якщо (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (write (pipe_w, stdin, stdin_len));
  якщо (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      якщо (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          goto restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  якщо (timeout)
    {
      time_t start = time (NULL);
      time_t now;
      для (now = start; now - start < timeout; now = time (NULL))
        {
          siginfo_t info;
          ціле elapsed = now - start;
          struct timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          якщо (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              goto restore_sig_mask_and_exit;
            }

          якщо (info.si_signo == SIGCHLD && info.si_pid == pid)
            goto read_waitpid;

          якщо (ret < 0 && errno == EAGAIN)
            goto timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      goto restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  якщо (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  інакше
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  якщо (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      якщо (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, NULL);
      якщо (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  повернути ret;
}

ціле
mark_or_close_fds_ge_than (libcrun_container_t *container, ціле n, bool close_now, libcrun_error_t *err)
{
  cleanup_close ціле cfd = -1;
  cleanup_dir DIR *dir = NULL;
  ціле ret;
  ціле fd;
  struct dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  якщо (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  якщо (ret == 0)
    {
      якщо (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      повернути 0;
    }
  якщо (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    повернути crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  якщо (UNLIKELY (cfd < 0))
    повернути cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  dir = fdopendir (cfd);
  якщо (UNLIKELY (dir == NULL))
    повернути crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  для (next = readdir (dir); next; next = readdir (dir))
    {
      ціле val;
      const char *name = next->d_name;
      якщо (name[0] == '.')
        continue;

      val = strtoll (name, NULL, 10);
      якщо (val < n || val == fd)
        continue;

      якщо (close_now)
        {
          ret = close (val);
          якщо (UNLIKELY (ret < 0))
            повернути crun_make_error (err, errno, "close fd `%d`", val);

          якщо (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      інакше
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          якщо (UNLIKELY (ret < 0))
            повернути crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  повернути 0;
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

  (void) snprintf (out, len, "%s.%06lldZ", timestamp, (long long ціле) tv.tv_usec);
  out[len - 1] = '\0';
}

ціле
set_blocking_fd (ціле fd, bool blocking, libcrun_error_t *err)
{
  ціле ret, flags = fcntl (fd, F_GETFL, 0);
  якщо (UNLIKELY (flags < 0))
    повернути crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "fcntl");
  повернути 0;
}

ціле
parse_json_file (json_object **out, const char *jsondata, struct parser_context *ctx arg_unused, libcrun_error_t *err)
{
  struct json_tokener *tok;
  enum json_tokener_error jerr;

  *err = NULL;

  якщо (UNLIKELY (jsondata == NULL))
    повернути crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  якщо (UNLIKELY (tok == NULL))
    повернути crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (ціле) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  якщо (*out == NULL || jerr != json_tokener_success)
    {
      ціле saved_pos = (ціле) tok->char_offset;

      json_tokener_free (tok);
      якщо (*out)
        {
          json_object_put (*out);
          *out = NULL;
        }
      повернути crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  повернути 0;
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
static ціле
check_access (const char *path)
{
  ціле ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#інакше
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  якщо (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      якщо (errno == EACCES)
        {
          ціле saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          якщо (ret == 0)
            повернути CHECK_ACCESS_NOT_EXECUTABLE;
        }
      повернути -errno;
    }

  ret = get_file_type (&mode, false, path);
  якщо (UNLIKELY (ret < 0))
    повернути -errno;

  якщо (! S_ISREG (mode))
    повернути CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  повернути 0;
}

ціле
find_executable (char **out, const char *executable_path, const char *cwd, libcrun_error_t *err)
{
  cleanup_free char *cwd_executable_path = NULL;
  cleanup_free char *tmp = NULL;
  ціле last_error = -ENOENT;
  char *it, *end;
  ціле ret;

  *out = NULL;

  якщо (executable_path == NULL || executable_path[0] == '\0')
    повернути crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  якщо (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free char *cwd_allocated = NULL;

      якщо (cwd == NULL)
        {
          cwd_allocated = getcwd (NULL, 0);
          якщо (cwd_allocated == NULL)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, NULL);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  якщо (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      якщо (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          повернути 0;
        }
      last_error = ret;
      goto fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  поки ((it = strsep (&end, ":")))
    {
      cleanup_free char *path = NULL;

      якщо (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, NULL);
      якщо (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          continue;
        }

      ret = check_access (path);
      якщо (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = NULL;
          повернути 0;
        }

      якщо (ret == -ENOENT)
        continue;

      last_error = ret;
    }

fail:
  switch (last_error)
    {
    case CHECK_ACCESS_NOT_EXECUTABLE:
      повернути crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    case CHECK_ACCESS_NOT_REGULAR:
      повернути crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    default:
      errno = -last_error;
      якщо (errno == ENOENT)
        повернути crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      повернути crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

static ssize_t
safe_read_xattr (char **ret, ціле sfd, const char *srcname, const char *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  поки (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      якщо (UNLIKELY (s < 0))
        повернути crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      якщо (s < current_size)
        break;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  якщо (s <= 0)
    повернути s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = NULL;

  повернути s;
}

static ssize_t
copy_xattr (ціле sfd, ціле dfd, const char *srcname, const char *destname, libcrun_error_t *err)
{
  cleanup_free char *buf = NULL;
  ssize_t xattr_len;
  char *it;

  xattr_len = flistxattr (sfd, NULL, 0);
  якщо (UNLIKELY (xattr_len < 0))
    {
      якщо (errno == ENOTSUP)
        повернути 0;

      повернути crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  якщо (xattr_len == 0)
    повернути 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  якщо (UNLIKELY (xattr_len < 0))
    повернути crun_make_error (err, errno, "flistxattr `%s`", srcname);

  для (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free char *v = NULL;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      якщо (UNLIKELY (s < 0))
        повернути s;

      s = fsetxattr (dfd, it, v, s, 0);
      якщо (UNLIKELY (s < 0))
        {
          якщо (errno == EINVAL || errno == EOPNOTSUPP)
            continue;

          повернути crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  повернути 0;
}

#endif

static ціле
copy_rec_stat_file_at (ціле dfd, const char *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  struct stat st;
  ціле ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  якщо (UNLIKELY (ret < 0))
    {
      якщо (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      повернути ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  повернути ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  повернути ret;
}

ціле
copy_recursive_fd_to_fd (ціле srcdirfd, ціле dfd, const char *srcname, const char *destname, libcrun_error_t *err)
{
  cleanup_close ціле destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = NULL;
  struct dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  якщо (UNLIKELY (dsrcfd == NULL))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      повернути crun_make_error (err, errno, "open directory `%s`", destname);
    }

  для (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close ціле srcfd = -1;
      cleanup_close ціле destfd = -1;
      cleanup_free char *target_buf = NULL;
      ціле ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      якщо (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      switch (mode & S_IFMT)
        {
        case S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          якщо (UNLIKELY (srcfd < 0))
            повернути crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          якщо (UNLIKELY (destfd < 0))
            повернути crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;

#ifdef HAVE_FGETXATTR
          ret = (ціле) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          break;

        case S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          якщо (UNLIKELY (ret < 0))
            повернути crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          якщо (UNLIKELY (srcfd < 0))
            повернути crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          якщо (UNLIKELY (destfd < 0))
            повернути crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (ціле) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          break;

        case S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          якщо (UNLIKELY (ret < 0))
            повернути crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          break;

        case S_IFBLK:
        case S_IFCHR:
        case S_IFIFO:
        case S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          якщо (UNLIKELY (ret < 0))
            повернути crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          break;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      якщо (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          якщо (errno == ENOTSUP)
            continue;

          повернути crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  повернути 0;
}

const char *
find_annotation (libcrun_container_t *container, const char *name)
{
  якщо (container->container_def->annotations == NULL)
    повернути NULL;

  повернути find_string_map_value (container->annotations, name);
}

ціле
safe_write (ціле fd, const char *fname, const void *buf, size_t count, libcrun_error_t *err)
{
  size_t written = 0;
  поки (written < count)
    {
      ssize_t w = write (fd, (const char *) buf + written, count - written);
      якщо (UNLIKELY (w < 0))
        {
          якщо (errno == EINTR || errno == EAGAIN)
            continue;
          повернути crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  повернути 0;
}

#define MAX_PARTS 32

ціле
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
  для (;;)
    {
      const char *part;
      size_t size;

      part = va_arg (ap, const char *);
      якщо (part == NULL)
        break;

      якщо (n_parts == MAX_PARTS)
        {
          va_end (ap);
          повернути crun_make_error (err, EINVAL, "too many paths specified");
        }

      якщо (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          поки (part[0] == '/' && part[1] == '/')
            part++;
        }
      інакше
        {
          /* And drop any initial '/' for other components.  */
          поки (part[0] == '/')
            part++;
        }

      size = strlen (part);
      якщо (size == 0)
        continue;

      поки (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  для (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  для (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      якщо (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  повернути 0;
}

#якщо __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#інакше
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
static ціле
b64_input (char c)
{
  const char table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  ціле i;

  для (i = 0; i < 64; i++)
    {
      якщо (table[i] == c)
        повернути i;
    }
  повернути -1;
}

ціле
base64_decode (const char *iptr, size_t isize, char *optr, size_t osize, size_t *nbytes)
{
  ціле i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned char data[4];

  *nbytes = 0;
  поки (consumed < isize && (*nbytes) + 3 < osize)
    {
      поки ((i < 4) && (consumed < isize))
        {
          tmp = b64_input (*iptr++);
          consumed++;
          якщо (tmp != -1)
            {
              якщо (UNLIKELY (pad > 0))
                повернути -1;
              data[i++] = tmp;
            }
          інакше якщо (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      якщо (i == 4)
        {
          якщо (UNLIKELY (pad > 2))
            повернути -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      інакше
        {
          /* I did not get all the data.  */
          consumed -= i;
          повернути consumed;
        }
      i = 0;
    }
  повернути consumed;
}

char *
get_user_name (uid_t uid)
{
  struct passwd pd;
  struct passwd *temp_result_ptr;
  char pwdbuffer[200];
  якщо (! getpwuid_r (uid, &pd, pwdbuffer, sizeof (pwdbuffer), &temp_result_ptr))
    повернути xstrdup (pd.pw_name);
  повернути xstrdup ("");
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
  якщо (sep == NULL || *sep == '/')
    повернути xstrdup (key);

  ret = xstrdup (key);
  для (it = ret; *it; it++)
    {
      якщо (*it == '.')
        *it = '/';
      інакше якщо (*it == '/')
        *it = '.';
    }

  повернути ret;
}

ціле
has_suffix (const char *str, const char *suffix)
{
  якщо (! str || ! suffix)
    повернути 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  якщо (lensuffix > lenstr)
    повернути 0;
  повернути memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

char *
str_join_array (ціле offset, size_t size, char *const array[], const char *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = NULL;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  char *result, *p;

  lens = xmalloc (size * sizeof (*lens));

  для (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  для (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      якщо (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  повернути result;
}

ціле
libcrun_mmap (struct libcrun_mmap_s **ret, void *addr, size_t length,
              ціле prot, ціле flags, ціле fd, off_t offset,
              libcrun_error_t *err)
{
  struct libcrun_mmap_s *mmap_s = NULL;

  void *mapped = mmap (addr, length, prot, flags, fd, offset);
  якщо (mapped == MAP_FAILED)
    повернути crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (sizeof (struct libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  повернути 0;
}

ціле
libcrun_munmap (struct libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  ціле ret;

  ret = munmap (mmap->addr, mmap->length);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "munmap");

  free (mmap);

  повернути 0;
}

static long
read_file_as_long_or_default (const char *path, long def_value)
{
  cleanup_free char *content = NULL;
  libcrun_error_t tmp_err = NULL;
  char *endptr;
  long val;
  ціле ret;

  ret = read_all_file (path, &content, NULL, &tmp_err);
  якщо (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      повернути def_value;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  якщо (UNLIKELY (errno))
    повернути def_value;
  якщо (endptr == content || (*endptr && *endptr != '\n'))
    повернути def_value;
  повернути val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (void)
{
  static atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  якщо (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  повернути uid;
}

gid_t
get_overflow_gid (void)
{
  static atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  якщо (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  повернути gid;
}

void
consume_trailing_slashes (char *path)
{
  якщо (! path || path[0] == '\0')
    повернути;

  char *last = path + strlen (path);

  поки (last > path && *(last - 1) == '/')
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
  якщо (UNLIKELY (dir == NULL))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      повернути NULL;
    }

  entries = xmalloc (entries_size * sizeof (char *));
  поки ((de = readdir (dir)))
    {
      якщо (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;
      якщо (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * sizeof (char *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * sizeof (char *));
  entries[n_entries] = NULL;

  повернути entries;
}

ціле
cpuset_string_to_bitmask (const char *str, char **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free char *mask = NULL;
  size_t mask_size = 0;
  const char *p = str;
  char *endptr;

  поки (*p)
    {
      long long start_range, end_range;

      якщо (*p < '0' || *p > '9')
        goto invalid_input;

      start_range = strtoll (p, &endptr, 10);
      якщо (start_range < 0)
        goto invalid_input;

      p = endptr;

      якщо (*p != '-')
        end_range = start_range;
      інакше
        {
          p++;

          якщо (*p < '0' || *p > '9')
            goto invalid_input;

          end_range = strtoll (p, &endptr, 10);

          якщо (end_range < start_range)
            goto invalid_input;

          p = endptr;
        }

      /* Just set some limit.  */
      якщо (end_range > (1 << 20))
        goto invalid_input;

      якщо (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      для (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      якщо (*p == ',')
        p++;
      інакше якщо (*p)
        goto invalid_input;
    }

  *out = mask;
  mask = NULL;
  *out_size = mask_size;

  повернути 0;

invalid_input:
  повернути crun_make_error (err, 0, "cannot parse input `%s`", str);
}

struct channel_fd_pair
{
  struct ring_buffer *rb;

  ціле in_fd;
  ціле out_fd;

  ціле infd_epoll_events;
  ціле outfd_epoll_events;
};

struct channel_fd_pair *
channel_fd_pair_new (ціле in_fd, ціле out_fd, size_t size)
{
  struct channel_fd_pair *channel = xmalloc (sizeof (struct channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  повернути channel;
}

void
channel_fd_pair_free (struct channel_fd_pair *channel)
{
  якщо (channel == NULL)
    повернути;

  ring_buffer_free (channel->rb);
  free (channel);
}

ціле
channel_fd_pair_process (struct channel_fd_pair *channel, ціле epollfd, libcrun_error_t *err)
{
  bool is_input_eagain = false, is_output_eagain = false, repeat;
  ціле ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  для (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      якщо (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_input_eagain, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          якщо (ret > 0)
            repeat = true;
        }
      якщо (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          якщо (ret > 0)
            repeat = true;
        }
    }

  якщо (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      ціле events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_input_eagain ? EPOLLET : 0)) : 0;
      якщо (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      якщо (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          channel->outfd_epoll_events = events;
        }
    }
  повернути 0;
}

static ціле
open_proc (libcrun_error_t *err)
{
  ціле fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close ціле fsfd = -1;
  ціле ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  якщо (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
      якщо (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          якщо (fd >= 0)
            повернути fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  якщо (fd < 0)
    повернути crun_make_error (err, errno, "open `/proc`");
  повернути fd;
}

ціле
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  якщо (! container)
    повернути open_proc (err);

  якщо (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  повернути container->proc_fd;
}

ціле
libcrun_open_proc_file (libcrun_container_t *container, const char *path, ціле flags, libcrun_error_t *err)
{
  ціле proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  якщо (proc_fd < 0)
    повернути proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  якщо (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  якщо (UNLIKELY (fd < 0))
    повернути crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  повернути fd;
}

ціле
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const char *path, ціле flags, libcrun_error_t *err)
{
  cleanup_free char *full_path = NULL;
  xasprintf (&full_path, "%d/%s", pid, path);
  повернути libcrun_open_proc_file (container, full_path, flags, err);
}
