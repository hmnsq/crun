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
#ако HAVE_STDATOMIC_H
#  include <stdatomic.h>
#инако
#  define atomic_long volatile long
#  define atomic_int volatile цео
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

цео
crun_path_exists (const char *path, libcrun_error_t *err)
{
  цео ret = access (path, F_OK);
  ако (ret < 0)
    {
      ако (errno == ENOENT)
        врати 0;
      врати crun_make_error (err, errno, "access `%s`", path);
    }
  врати 1;
}

цео
xasprintf (char **str, const char *fmt, ...)
{
  цео ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  ако (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  врати ret;
}

цео
write_file_at_with_flags (цео dirfd, цео flags, mode_t mode, const char *name, const void *data, size_t len, libcrun_error_t *err)
{
  cleanup_close цео fd = -1;
  цео ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  ако (UNLIKELY (fd < 0))
    врати crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  врати (len > INT_MAX) ? INT_MAX : (цео) len;
}

цео
detach_process ()
{
  pid_t pid;
  ако (setsid () < 0)
    врати -1;
  pid = fork ();
  ако (pid < 0)
    врати -1;
  ако (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  врати 0;
}

цео
get_file_type_fd (цео fd, mode_t *mode)
{
  struct stat st;
  цео ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ако (UNLIKELY (ret < 0))
    {
      ако (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      врати ret;
    }
  *mode = stx.stx_mode;
  врати ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  врати ret;
}

цео
get_file_type_at (цео dirfd, mode_t *mode, bool nofollow, const char *path)
{
  цео empty_path = path == NULL ? AT_EMPTY_PATH : 0;
  struct stat st;
  цео ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ако (UNLIKELY (ret < 0))
    {
      ако (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      врати ret;
    }
  *mode = stx.stx_mode;
  врати ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  врати ret;
}

цео
get_file_type (mode_t *mode, bool nofollow, const char *path)
{
  врати get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

цео
create_file_if_missing_at (цео dirfd, const char *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close цео fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  ако (fd_write < 0)
    {
      цео saved_errno = errno;
      mode_t tmp_mode;
      цео ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      ако (ret == 0 && S_ISREG (tmp_mode))
        врати 0;

      врати crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  врати 0;
}

static цео
ensure_directory_internal_at (цео dirfd, char *path, size_t len, цео mode, libcrun_error_t *err)
{
  char *it = path + len;
  цео ret = 0;
  bool parent_created = false;

  за (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      ако (ret == 0 || errno == EEXIST)
        врати 0;

      цео saved_errno = errno;
      ако (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = NULL;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          ако (ret > 0)
            врати 0;
          ако (ret < 0)
            crun_error_release (&tmp_err);

          врати crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      док (it > path && *it != '/')
        {
          it--;
          len--;
        }
      ако (it == path)
        врати 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      ако (UNLIKELY (ret < 0))
        врати ret;

      parent_created = true;
    }
  врати ret;
}

цео
crun_ensure_directory_at (цео dirfd, const char *path, цео mode, bool nofollow, libcrun_error_t *err)
{
  цео ret;
  cleanup_free char *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ако (ret == 0)
    врати crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  врати 0;
}

static цео
check_fd_is_path (const char *path, цео fd, const char *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  char link[PATH_MAX];
  цео ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, sizeof (link)));
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "readlink `%s`", fdname);

  ако (((size_t) ret) != path_len || memcmp (link, path, path_len))
    врати crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  врати 0;
}

static цео
check_fd_under_path (const char *rootfs, size_t rootfslen, цео fd, const char *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  char link[PATH_MAX];
  цео ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  ако (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    врати 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, sizeof (link)));
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "readlink `%s`", fdname);

  ако (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    врати crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  врати 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
static void
close_and_replace (цео *oldfd, цео newfd)
{
  ако (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
char *chroot_realpath (const char *chroot, const char *path, char resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
static цео
safe_openat_fallback (цео dirfd, const char *rootfs, const char *path, цео flags,
                      цео mode, libcrun_error_t *err)
{
  cleanup_free char *parent_path = NULL;
  const char *last_component = NULL;
  const char *orig_path = path;
  const char *path_in_chroot;
  cleanup_close цео fd = -1;
  char resolved[PATH_MAX];
  char buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  цео ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  ако (flags & O_NOFOLLOW)
    {
      char *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      ако (sep == NULL)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      инако ако (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      ако (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  ако (path_in_chroot == NULL)
    врати crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  ако (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  ако (last_component)
    {
      ret = snprintf (resolved, sizeof (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      ако (UNLIKELY (ret >= (цео) sizeof (resolved)))
        врати crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  ако (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      ако (UNLIKELY (ret < 0))
        врати crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      врати ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ret = fd;
  fd = -1;
  врати ret;
}

цео
safe_openat (цео dirfd, const char *rootfs, const char *path, цео flags, цео mode,
             libcrun_error_t *err)
{
  static bool openat2_supported = true;
  цео retries = MAX_OPENAT2_EAGAIN_RETRIES;
  цео ret;

  ако (is_empty_string (path))
    {
      cleanup_close цео fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      ако (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      ако (UNLIKELY (fd < 0))
        врати crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      ако (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          ако (UNLIKELY (ret < 0))
            врати ret;
        }

      ret = fd;
      fd = -1;
      врати ret;
    }

  ако (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_ROOT);
      ако (UNLIKELY (ret < 0))
        {
          ако (errno == EINTR)
            goto repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          ако (errno == EAGAIN && retries-- > 0)
            goto repeat;
          ако (errno == ENOSYS)
            openat2_supported = false;
          ако (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            врати safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          врати crun_make_error (err, errno, "openat2 `%s`", path);
        }

      врати ret;
    }

  врати safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (цео dfd, const char *name, char **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free char *tmp_buf = NULL;
  ssize_t size;

  do
    {
      ако (tmp_buf != NULL)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      ако (UNLIKELY (size < 0))
        врати crun_make_error (err, errno, "readlink `%s`", name);
  } док (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = NULL;

  врати size;
}

static цео crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, цео dirfd, const char *dirpath,
                                const char *path, цео mode, цео max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
static цео
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, цео dirfd, const char *dirpath, цео cwd,
                                 const char *npath, const char *cur, const char *rest,
                                 цео mode, цео max_readlinks, libcrun_error_t *err)
{
  cleanup_free char *target = NULL;
  cleanup_free char *new_path = NULL;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (цео) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  врати crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

static цео
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, цео dirfd, const char *dirpath,
                     const char *path, цео mode, цео max_readlinks, libcrun_error_t *err)
{
  cleanup_close цео wd_cleanup = -1;
  cleanup_free char *npath = NULL;
  bool last_component = false;
  size_t depth = 0;
  char *cur;
  char *it;
  цео cwd;
  цео ret;

  ако (max_readlinks <= 0)
    врати crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  ако (*path == '\0')
    {
      ако (do_open)
        врати open (dirpath, O_CLOEXEC | O_PATH, 0);
      врати 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  док (cur)
    {
      ако (it)
        *it = '\0';
      инако
        last_component = true;

      ако (cur[0] == '\0')
        break;

      ако (strcmp (cur, ".") == 0)
        goto next;
      инако ако (strcmp (cur, ".."))
        depth++;
      инако
        {
          ако (depth)
            depth--;
          инако
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              goto next;
            }
        }

      ако (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          ако (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              ако (errno == ELOOP)
                врати crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, NULL,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              ако (ret < 0)
                врати crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          ако (do_open)
            врати ret;

          close_and_replace (&wd_cleanup, ret);
          врати 0;
        }

      ret = mkdirat (cwd, cur, mode);
      ако (ret < 0)
        {
          mode_t st_mode;

          ако (errno != EEXIST)
            врати crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          ако (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            врати crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : NULL, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      ако (UNLIKELY (cwd < 0))
        врати crun_error_wrap (err, "creating `/%s`", path);

      ако (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, NULL);
          ако (UNLIKELY (ret < 0))
            {
              цео saved_errno = errno;

              close (cwd);
              врати crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          ако ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              врати crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      ако (it == NULL)
        break;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  ако (do_open)
    {
      ако (cwd == dirfd)
        {
          ret = dup (dirfd);
          ако (UNLIKELY (ret < 0))
            врати crun_make_error (err, errno, "dup `%s`", dirpath);
          врати ret;
        }

      wd_cleanup = -1;
      врати cwd;
    }

  врати 0;
}

цео
crun_safe_create_and_open_ref_at (bool dir, цео dirfd, const char *dirpath, const char *path, цео mode, libcrun_error_t *err)
{
  цео ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  ако (LIKELY (ret >= 0))
    врати ret;

  crun_error_release (err);
  врати crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

цео
crun_safe_ensure_directory_at (цео dirfd, const char *dirpath, const char *path, цео mode,
                               libcrun_error_t *err)
{
  врати crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

цео
crun_safe_ensure_file_at (цео dirfd, const char *dirpath, const char *path, цео mode,
                          libcrun_error_t *err)
{
  врати crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

цео
crun_ensure_directory (const char *path, цео mode, bool nofollow, libcrun_error_t *err)
{
  врати crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

static цео
get_file_size (цео fd, off_t *size)
{
  struct stat st;
  цео ret;
#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  ако (UNLIKELY (ret < 0))
    {
      ако (errno == ENOSYS || errno == EINVAL)
        goto fallback;
      врати ret;
    }
  *size = stx.stx_size;

  врати ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  врати ret;
}

цео
crun_dir_p_at (цео dirfd, const char *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  цео ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "stat `%s`", path);

  врати S_ISDIR (mode) ? 1 : 0;
}

цео
crun_dir_p (const char *path, bool nofollow, libcrun_error_t *err)
{
  врати crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
static цео
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  size_t len;
  цео ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  ако (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      ако (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          врати 0;
        }
      врати ret;
    }

  врати strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
static bool
running_in_openvz ()
{
  ако (access ("/proc/vz", F_OK) < 0)
    врати false;

  врати access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

цео
check_running_in_user_namespace (libcrun_error_t *err)
{
  static atomic_int run_in_userns = -1;
  struct stat st;
  цео ret;

  ret = run_in_userns;
  ако (ret >= 0)
    врати ret;

  ret = stat ("/proc/self/ns/user", &st);
  ако (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      ако (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          врати run_in_userns;
        }

      ако (! running_in_openvz ())
        {
          run_in_userns = 1;
          врати run_in_userns;
        }
    }
  инако ако (UNLIKELY (errno != ENOENT))
    врати crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  run_in_userns = ret;
  врати run_in_userns;
}

static size_t
get_page_size ()
{
  static atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  ако (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  врати (size_t) pagesize;
}

static цео selinux_enabled = -1;
static цео apparmor_enabled = -1;

цео
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free char *out = NULL;
  cleanup_close цео fd = -1;
  size_t len;
  цео ret;

  ако (selinux_enabled >= 0)
    врати selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  ако (UNLIKELY (fd < 0))
    врати fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  врати selinux_enabled;
}

цео
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close цео fd = -1;
  цео size;
  char buf[1];

  ако (apparmor_enabled >= 0)
    врати apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  ако (fd == -1)
    {
      ако (errno == ENOENT)
        {
          apparmor_enabled = 0;
          врати 0;
        }

      врати crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (read (fd, buf, sizeof (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  врати apparmor_enabled;
}

static цео
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  ако (selinux_enabled < 0)
    врати crun_make_error (err, 0, "SELinux is not initialized correctly");
  врати selinux_enabled;
}

цео
add_selinux_mount_label (char **retlabel, const char *data, const char *label, const char *context_type, libcrun_error_t *err)
{
  цео ret;

  ret = libcrun_is_selinux_enabled (err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ако (label && ret)
    {
      ако (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      инако
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      врати 0;
    }
  *retlabel = xstrdup (data);
  врати 0;
}

static const char *
lsm_attr_path (libcrun_container_t *container, const char *lsm, const char *fname, libcrun_error_t *err)
{
  cleanup_close цео attr_dirfd = -1;
  cleanup_close цео lsm_dirfd = -1;
  char *attr_path = NULL;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  ако (UNLIKELY (attr_dirfd < 0))
    врати NULL;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  ако (lsm != NULL)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      ако (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          врати NULL;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  врати attr_path;
}

static цео
check_proc_super_magic (цео fd, const char *path, libcrun_error_t *err)
{
  struct statfs sfs;

  цео ret = fstatfs (fd, &sfs);
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "statfs `%s`", path);

  ако (sfs.f_type != PROC_SUPER_MAGIC)
    врати crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  врати 0;
}

static цео
set_security_attr (libcrun_container_t *container, const char *lsm, const char *fname, const char *data, libcrun_error_t *err)
{
  цео ret;

  cleanup_free const char *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close цео fd = -1;

  ако (UNLIKELY (attr_path == NULL))
    врати -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  ако (UNLIKELY (fd < 0))
    врати fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (write (fd, data, strlen (data)));
  ако (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      ако (errno == ENOENT && lsm != NULL && strcmp (lsm, "apparmor") == 0)
        {
          const char *profile = strchr (data, ' ');

          врати crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      врати crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  врати 0;
}

цео
set_selinux_label (libcrun_container_t *container, const char *label, bool now, libcrun_error_t *err)
{
  цео ret;

  ret = libcrun_is_selinux_enabled (err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ако (ret)
    врати set_security_attr (container, NULL, now ? "current" : "exec", label, err);
  врати 0;
}

static цео
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  ако (apparmor_enabled < 0)
    врати crun_make_error (err, 0, "AppArmor is not initialized correctly");
  врати apparmor_enabled;
}

static цео
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (sizeof (UNCONFINED) - 1)
  cleanup_free const char *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close цео fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  char buf[UNCONFINED_LEN + 1];

  ако (UNLIKELY (attr_path == NULL))
    врати -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  ако (UNLIKELY (fd < 0))
    врати fd;

  ако (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    врати -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (read (fd, buf, sizeof (buf)));
  ако (UNLIKELY (bytes_read < 0))
    врати crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  ако (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    врати 1;
  ако (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    врати 0;
  врати 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

цео
set_apparmor_profile (libcrun_container_t *container, const char *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  цео ret;

  ret = libcrun_is_apparmor_enabled (err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ако (ret)
    {
      cleanup_free char *buf = NULL;
      ret = is_current_process_confined (container, err);
      ако (UNLIKELY (ret < 0))
        врати ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      врати set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  врати 0;
}

static цео
read_all_fd_internal (цео fd, const char *description, char **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free char *buf = NULL;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  цео ret;

  ако (size_hint)
    allocated = size_hint;
  инако
    {
      ret = get_file_size (fd, &size);
      ако (UNLIKELY (ret < 0))
        врати crun_make_error (err, errno, "stat `%s`", description);

      ако (max_size && (size_t) size > max_size)
        врати crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  док ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (read (fd, buf + nread, allocated - nread));
      ако (UNLIKELY (ret < 0))
        врати crun_make_error (err, errno, "read from file `%s`", description);

      ако (ret == 0)
        break;

      nread += ret;

      ако (max_size && nread > max_size)
        врати crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      ако (nread == allocated)
        {
          ако (size)
            break;

          ако (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  ако (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      char *tmp = realloc (buf, nread + 1);
      ако (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = NULL;
  ако (len)
    *len = nread;
  врати 0;
}

цео
read_all_fd_with_size_hint (цео fd, const char *description, char **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  врати read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

цео
read_all_fd_with_max_size (цео fd, const char *description, char **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  врати read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

цео
read_all_file_at (цео dirfd, const char *path, char **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close цео fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  ако (UNLIKELY (fd < 0))
    врати crun_make_error (err, errno, "open `%s`", path);

  врати read_all_fd (fd, path, out, len, err);
}

цео
read_all_file (const char *path, char **out, size_t *len, libcrun_error_t *err)
{
  ако (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  врати read_all_file_at (AT_FDCWD, path, out, len, err);
}

цео
get_realpath_to_file (цео dirfd, const char *path_name, char **absolute_path, libcrun_error_t *err)
{
  cleanup_close цео targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  ако (UNLIKELY (targetfd < 0))
    врати crun_make_error (err, errno, "open `%s`", path_name);
  инако
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      ако (UNLIKELY (len < 0))
        врати crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  врати 0;
}

цео
open_unix_domain_client_socket (const char *path, цео dgram, libcrun_error_t *err)
{
  struct sockaddr_un addr = {};
  цео ret;
  proc_fd_path_t name_buf;
  cleanup_close цео destfd = -1;
  cleanup_close цео fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  ако (UNLIKELY (fd < 0))
    врати crun_make_error (err, errno, "create UNIX socket");

  ако (strlen (path) >= sizeof (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      ако (UNLIKELY (destfd < 0))
        врати crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  ако (path_len >= sizeof (addr.sun_path))
    врати crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (struct sockaddr *) &addr, sizeof (addr));
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  врати ret;
}

цео
open_unix_domain_socket (const char *path, цео dgram, libcrun_error_t *err)
{
  struct sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  цео ret;
  cleanup_close цео dirfd = -1;
  cleanup_close цео fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  ако (UNLIKELY (fd < 0))
    врати crun_make_error (err, errno, "create UNIX socket");

  ако (strlen (path) >= sizeof (addr.sun_path))
    {
      cleanup_free char *dpath = xstrdup (path);
      cleanup_free char *bpath = xstrdup (path);
      const char *parent_dir = dirname (dpath);
      const char *base = basename (bpath);
      цео n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      ако (UNLIKELY (dirfd < 0))
        врати crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, sizeof (addr.sun_path), "%s/%s", name_buf, base);
      ако (n < 0 || (size_t) n >= sizeof (addr.sun_path))
        врати crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  инако
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (struct sockaddr *) &addr, sizeof (addr));
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "bind socket to `%s`", path);

  ако (! dgram)
    {
      ret = listen (fd, 1);
      ако (UNLIKELY (ret < 0))
        врати crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  врати ret;
}

цео
send_fd_to_socket (цео server, цео fd, libcrun_error_t *err)
{
  врати send_fd_to_socket_with_payload (server, fd, NULL, 0, err);
}

цео
send_fd_to_socket_with_payload (цео server, цео fd, const char *payload, size_t payload_len, libcrun_error_t *err)
{
  цео ret;
  struct cmsghdr *cmsg = NULL;
  struct iovec iov[2];
  struct msghdr msg = {};
  char ctrl_buf[CMSG_SPACE (1 + sizeof (цео))] = {};
  char data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = sizeof (data);

  ако (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (sizeof (цео));
  msg.msg_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (sizeof (цео));

  *((цео *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &msg, 0));
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "sendmsg");
  врати 0;
}

цео
receive_fd_from_socket_with_payload (цео from, char *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close цео fd = -1;
  цео ret;
  struct iovec iov[1];
  struct msghdr msg = {};
  char ctrl_buf[CMSG_SPACE (sizeof (цео))] = {};
  char data[1];
  struct cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = sizeof (data);

  ако (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (sizeof (цео));
  msg.msg_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &msg, 0));
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "recvmsg");
  ако (UNLIKELY (ret == 0))
    врати crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&msg);
  ако (cmsg == NULL)
    врати crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), sizeof (fd));

  ret = fd;
  fd = -1;
  врати ret;
}

цео
receive_fd_from_socket (цео from, libcrun_error_t *err)
{
  врати receive_fd_from_socket_with_payload (from, NULL, 0, err);
}

цео
create_socket_pair (цео *pair, libcrun_error_t *err)
{
  цео ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "socketpair");
  врати 0;
}

цео
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  цео ret = signalfd (-1, mask, 0);
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "signalfd");
  врати ret;
}

static цео
epoll_helper_toggle (цео epollfd, цео fd, цео events, libcrun_error_t *err)
{
  struct epoll_event ev = {};
  bool add = events != 0;
  цео ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  ако (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  ако (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      ако (errno == ENOENT || errno == EPERM)
        врати 0;
      врати crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  врати 0;
}

цео
epoll_helper (цео *in_fds, цео *in_levelfds, цео *out_fds, цео *out_levelfds, libcrun_error_t *err)
{
  struct epoll_event ev;
  cleanup_close цео epollfd = -1;
  цео ret;

  цео *it;
  epollfd = epoll_create1 (0);
  ако (UNLIKELY (epollfd < 0))
    врати crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  за (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      ако (UNLIKELY (ret < 0))                                           \
        врати crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  ако (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  ако (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  ако (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  ако (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  врати ret;
}

цео
copy_from_fd_to_fd (цео src, цео dst, цео consume, libcrun_error_t *err)
{
  цео ret;
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
      ако (can_copy_file_range)
        {
          nread = copy_file_range (src, NULL, dst, NULL, pagesize, 0);
          ако (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              goto fallback;
            }
          ако (consume && nread < 0 && errno == EAGAIN)
            врати 0;
          ако (nread < 0 && errno == EIO)
            врати 0;
          ако (UNLIKELY (nread < 0))
            врати crun_make_error (err, errno, "copy_file_range");
          continue;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (read (src, buffer, pagesize));
      ако (consume && nread < 0 && errno == EAGAIN)
        врати 0;
      ако (nread < 0 && errno == EIO)
        врати 0;
      ако (UNLIKELY (nread < 0))
        врати crun_make_error (err, errno, "read");

      remaining = nread;
      док (remaining)
        {
          ret = TEMP_FAILURE_RETRY (write (dst, buffer + nread - remaining, remaining));
          ако (UNLIKELY (ret < 0))
            врати crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } док (consume && nread);

  врати 0;
}

цео
run_process (char **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  ако (UNLIKELY (pid < 0))
    врати crun_make_error (err, errno, "fork");
  ако (pid)
    {
      цео r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      ако (r < 0)
        врати crun_make_error (err, errno, "waitpid");
      ако (WIFEXITED (status) || WIFSIGNALED (status))
        врати WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
static unsigned
atou (char **s)
{
  unsigned x;
  за (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  врати x;
}

цео
fgetpwent_r (FILE *f, struct passwd *pw, char *line, size_t size, struct passwd **res)
{
  char *s;
  цео rv = 0;
  за (;;)
    {
      line[size - 1] = '\xff';
      ако ((fgets (line, size, f) == NULL) || ferror (f) || line[size - 1] != '\xff')
        {
          rv = (line[size - 1] != '\xff') ? ERANGE : ENOENT;
          line = 0;
          pw = 0;
          break;
        }
      line[strcspn (line, "\n")] = 0;

      s = line;
      pw->pw_name = s++;
      ако (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_passwd = s;
      ако (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      ако (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      ако (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gecos = s;
      ако (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_dir = s;
      ако (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_shell = s;
      break;
    }
  *res = pw;
  ако (rv)
    errno = rv;
  врати rv;
}
#endif

цео
set_home_env (uid_t id)
{
  struct passwd pwd;
  cleanup_free char *buf = NULL;
  long buf_size;
  cleanup_file FILE *stream = NULL;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  ако (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  ако (stream == NULL)
    врати -1;

  за (;;)
    {
      struct passwd *ret_pw = NULL;

      цео ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      ако (UNLIKELY (ret != 0))
        {
          ако (ret == EINTR)
            continue;

          ако (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              врати -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          continue;
        }

      ако (ret_pw && ret_pw->pw_uid == id)
        {
          ако (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          инако
            врати 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
static цео
getsubidrange (uid_t id, цео is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *input = NULL;
  cleanup_free char *lineptr = NULL;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free char *buf = NULL;
  const char *name;
  struct passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  ако (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  за (;;)
    {
      цео ret;
      struct passwd *ret_pw = NULL;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      ако (LIKELY (ret == 0))
        {
          ако (ret_pw)
            {
              name = ret_pw->pw_name;
              break;
            }
          врати -1;
        }

      ако (ret != ERANGE)
        врати -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  input = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  ако (input == NULL)
    врати -1;

  за (;;)
    {
      char *endptr;
      ssize_t read = getline (&lineptr, &lenlineptr, input);
      ако (read < 0)
        врати -1;

      ако (read < (ssize_t) (len_name + 2))
        continue;

      ако (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        continue;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      ако (endptr >= &lineptr[read])
        врати -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      врати 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

цео
format_default_id_mapping (char **out, uid_t container_id, uid_t host_uid, uid_t host_id, цео is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free char *buffer = NULL;
  цео written = 0;
  цео ret, remaining;

  *out = NULL;

  ако (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    врати 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  ако (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      ако (UNLIKELY (ret >= remaining))
        врати crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  ако (UNLIKELY (ret >= remaining))
    врати crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  ако (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      ако (UNLIKELY (ret >= remaining))
        врати crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = NULL;
  врати written;
}

static цео
unset_cloexec_flag (цео fd)
{
  цео flags = fcntl (fd, F_GETFD);
  ако (flags == -1)
    врати -1;

  flags &= ~FD_CLOEXEC;

  врати fcntl (fd, F_SETFD, flags);
}

static void __attribute__ ((__noreturn__))
run_process_child (char *path, char **args, const char *cwd, char **envp, цео pipe_r,
                   цео pipe_w, цео out_fd, цео err_fd, bool can_ignore_chdir_errors)
{
  char *tmp_args[] = { path, NULL };
  libcrun_error_t err = NULL;
  цео dev_null_fd = -1;
  цео ret;

  ret = mark_or_close_fds_ge_than (NULL, 3, false, &err);
  ако (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->msg);

  ако (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      ако (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  ако (out_fd >= 0)
    unset_cloexec_flag (1);
  ако (err_fd >= 0)
    unset_cloexec_flag (2);

  ако (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  ако (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  ако (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  ако (args == NULL)
    args = tmp_args;

  ако (cwd && chdir (cwd) < 0)
    {
      ако (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
цео
run_process_with_stdin_timeout_envp (char *path, char **args, const char *cwd, цео timeout,
                                     char **envp, char *stdin, size_t stdin_len, цео out_fd,
                                     цео err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  цео stdin_pipe[2];
  pid_t pid;
  цео ret;
  cleanup_close цео pipe_r = -1;
  cleanup_close цео pipe_w = -1;
  sigset_t oldmask, mask;
  цео r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  ако (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      ако (UNLIKELY (ret < 0))
        врати crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  ако (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      goto restore_sig_mask_and_exit;
    }

  ако (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (write (pipe_w, stdin, stdin_len));
  ако (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      ако (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          goto restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  ако (timeout)
    {
      time_t start = time (NULL);
      time_t now;
      за (now = start; now - start < timeout; now = time (NULL))
        {
          siginfo_t info;
          цео elapsed = now - start;
          struct timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          ако (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              goto restore_sig_mask_and_exit;
            }

          ако (info.si_signo == SIGCHLD && info.si_pid == pid)
            goto read_waitpid;

          ако (ret < 0 && errno == EAGAIN)
            goto timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      goto restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  ако (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  инако
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  ако (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      ако (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, NULL);
      ако (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  врати ret;
}

цео
mark_or_close_fds_ge_than (libcrun_container_t *container, цео n, bool close_now, libcrun_error_t *err)
{
  cleanup_close цео cfd = -1;
  cleanup_dir DIR *dir = NULL;
  цео ret;
  цео fd;
  struct dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  ако (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  ако (ret == 0)
    {
      ако (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      врати 0;
    }
  ако (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    врати crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  ако (UNLIKELY (cfd < 0))
    врати cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  dir = fdopendir (cfd);
  ако (UNLIKELY (dir == NULL))
    врати crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  за (next = readdir (dir); next; next = readdir (dir))
    {
      цео val;
      const char *name = next->d_name;
      ако (name[0] == '.')
        continue;

      val = strtoll (name, NULL, 10);
      ако (val < n || val == fd)
        continue;

      ако (close_now)
        {
          ret = close (val);
          ако (UNLIKELY (ret < 0))
            врати crun_make_error (err, errno, "close fd `%d`", val);

          ако (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      инако
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          ако (UNLIKELY (ret < 0))
            врати crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  врати 0;
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

  (void) snprintf (out, len, "%s.%06lldZ", timestamp, (long long цео) tv.tv_usec);
  out[len - 1] = '\0';
}

цео
set_blocking_fd (цео fd, bool blocking, libcrun_error_t *err)
{
  цео ret, flags = fcntl (fd, F_GETFL, 0);
  ако (UNLIKELY (flags < 0))
    врати crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "fcntl");
  врати 0;
}

цео
parse_json_file (json_object **out, const char *jsondata, struct parser_context *ctx arg_unused, libcrun_error_t *err)
{
  struct json_tokener *tok;
  enum json_tokener_error jerr;

  *err = NULL;

  ако (UNLIKELY (jsondata == NULL))
    врати crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  ако (UNLIKELY (tok == NULL))
    врати crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (цео) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  ако (*out == NULL || jerr != json_tokener_success)
    {
      цео saved_pos = (цео) tok->char_offset;

      json_tokener_free (tok);
      ако (*out)
        {
          json_object_put (*out);
          *out = NULL;
        }
      врати crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  врати 0;
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
static цео
check_access (const char *path)
{
  цео ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#инако
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  ако (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      ако (errno == EACCES)
        {
          цео saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          ако (ret == 0)
            врати CHECK_ACCESS_NOT_EXECUTABLE;
        }
      врати -errno;
    }

  ret = get_file_type (&mode, false, path);
  ако (UNLIKELY (ret < 0))
    врати -errno;

  ако (! S_ISREG (mode))
    врати CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  врати 0;
}

цео
find_executable (char **out, const char *executable_path, const char *cwd, libcrun_error_t *err)
{
  cleanup_free char *cwd_executable_path = NULL;
  cleanup_free char *tmp = NULL;
  цео last_error = -ENOENT;
  char *it, *end;
  цео ret;

  *out = NULL;

  ако (executable_path == NULL || executable_path[0] == '\0')
    врати crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  ако (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free char *cwd_allocated = NULL;

      ако (cwd == NULL)
        {
          cwd_allocated = getcwd (NULL, 0);
          ако (cwd_allocated == NULL)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, NULL);
      ако (UNLIKELY (ret < 0))
        врати ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  ако (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      ако (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          врати 0;
        }
      last_error = ret;
      goto fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  док ((it = strsep (&end, ":")))
    {
      cleanup_free char *path = NULL;

      ако (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, NULL);
      ако (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          continue;
        }

      ret = check_access (path);
      ако (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = NULL;
          врати 0;
        }

      ако (ret == -ENOENT)
        continue;

      last_error = ret;
    }

fail:
  switch (last_error)
    {
    case CHECK_ACCESS_NOT_EXECUTABLE:
      врати crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    case CHECK_ACCESS_NOT_REGULAR:
      врати crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    default:
      errno = -last_error;
      ако (errno == ENOENT)
        врати crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      врати crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

static ssize_t
safe_read_xattr (char **ret, цео sfd, const char *srcname, const char *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  док (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      ако (UNLIKELY (s < 0))
        врати crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      ако (s < current_size)
        break;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  ако (s <= 0)
    врати s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = NULL;

  врати s;
}

static ssize_t
copy_xattr (цео sfd, цео dfd, const char *srcname, const char *destname, libcrun_error_t *err)
{
  cleanup_free char *buf = NULL;
  ssize_t xattr_len;
  char *it;

  xattr_len = flistxattr (sfd, NULL, 0);
  ако (UNLIKELY (xattr_len < 0))
    {
      ако (errno == ENOTSUP)
        врати 0;

      врати crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  ако (xattr_len == 0)
    врати 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  ако (UNLIKELY (xattr_len < 0))
    врати crun_make_error (err, errno, "flistxattr `%s`", srcname);

  за (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free char *v = NULL;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      ако (UNLIKELY (s < 0))
        врати s;

      s = fsetxattr (dfd, it, v, s, 0);
      ако (UNLIKELY (s < 0))
        {
          ако (errno == EINVAL || errno == EOPNOTSUPP)
            continue;

          врати crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  врати 0;
}

#endif

static цео
copy_rec_stat_file_at (цео dfd, const char *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  struct stat st;
  цео ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  ако (UNLIKELY (ret < 0))
    {
      ако (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      врати ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  врати ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  врати ret;
}

цео
copy_recursive_fd_to_fd (цео srcdirfd, цео dfd, const char *srcname, const char *destname, libcrun_error_t *err)
{
  cleanup_close цео destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = NULL;
  struct dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  ако (UNLIKELY (dsrcfd == NULL))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      врати crun_make_error (err, errno, "open directory `%s`", destname);
    }

  за (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close цео srcfd = -1;
      cleanup_close цео destfd = -1;
      cleanup_free char *target_buf = NULL;
      цео ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      ако (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      ако (UNLIKELY (ret < 0))
        врати crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      switch (mode & S_IFMT)
        {
        case S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          ако (UNLIKELY (srcfd < 0))
            врати crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          ако (UNLIKELY (destfd < 0))
            врати crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          ако (UNLIKELY (ret < 0))
            врати ret;

#ifdef HAVE_FGETXATTR
          ret = (цео) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ако (UNLIKELY (ret < 0))
            врати ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          break;

        case S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          ако (UNLIKELY (ret < 0))
            врати crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ако (UNLIKELY (srcfd < 0))
            врати crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ако (UNLIKELY (destfd < 0))
            врати crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (цео) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ако (UNLIKELY (ret < 0))
            врати ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          ако (UNLIKELY (ret < 0))
            врати ret;
          break;

        case S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          ако (UNLIKELY (ret < 0))
            врати ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          ако (UNLIKELY (ret < 0))
            врати crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          break;

        case S_IFBLK:
        case S_IFCHR:
        case S_IFIFO:
        case S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          ако (UNLIKELY (ret < 0))
            врати crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          break;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ако (UNLIKELY (ret < 0))
        врати crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      ако (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          ако (errno == ENOTSUP)
            continue;

          врати crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  врати 0;
}

const char *
find_annotation (libcrun_container_t *container, const char *name)
{
  ако (container->container_def->annotations == NULL)
    врати NULL;

  врати find_string_map_value (container->annotations, name);
}

цео
safe_write (цео fd, const char *fname, const void *buf, size_t count, libcrun_error_t *err)
{
  size_t written = 0;
  док (written < count)
    {
      ssize_t w = write (fd, (const char *) buf + written, count - written);
      ако (UNLIKELY (w < 0))
        {
          ако (errno == EINTR || errno == EAGAIN)
            continue;
          врати crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  врати 0;
}

#define MAX_PARTS 32

цео
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
  за (;;)
    {
      const char *part;
      size_t size;

      part = va_arg (ap, const char *);
      ако (part == NULL)
        break;

      ако (n_parts == MAX_PARTS)
        {
          va_end (ap);
          врати crun_make_error (err, EINVAL, "too many paths specified");
        }

      ако (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          док (part[0] == '/' && part[1] == '/')
            part++;
        }
      инако
        {
          /* And drop any initial '/' for other components.  */
          док (part[0] == '/')
            part++;
        }

      size = strlen (part);
      ако (size == 0)
        continue;

      док (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  за (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  за (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      ако (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  врати 0;
}

#ако __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#инако
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
static цео
b64_input (char c)
{
  const char table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  цео i;

  за (i = 0; i < 64; i++)
    {
      ако (table[i] == c)
        врати i;
    }
  врати -1;
}

цео
base64_decode (const char *iptr, size_t isize, char *optr, size_t osize, size_t *nbytes)
{
  цео i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned char data[4];

  *nbytes = 0;
  док (consumed < isize && (*nbytes) + 3 < osize)
    {
      док ((i < 4) && (consumed < isize))
        {
          tmp = b64_input (*iptr++);
          consumed++;
          ако (tmp != -1)
            {
              ако (UNLIKELY (pad > 0))
                врати -1;
              data[i++] = tmp;
            }
          инако ако (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      ако (i == 4)
        {
          ако (UNLIKELY (pad > 2))
            врати -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      инако
        {
          /* I did not get all the data.  */
          consumed -= i;
          врати consumed;
        }
      i = 0;
    }
  врати consumed;
}

char *
get_user_name (uid_t uid)
{
  struct passwd pd;
  struct passwd *temp_result_ptr;
  char pwdbuffer[200];
  ако (! getpwuid_r (uid, &pd, pwdbuffer, sizeof (pwdbuffer), &temp_result_ptr))
    врати xstrdup (pd.pw_name);
  врати xstrdup ("");
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
  ако (sep == NULL || *sep == '/')
    врати xstrdup (key);

  ret = xstrdup (key);
  за (it = ret; *it; it++)
    {
      ако (*it == '.')
        *it = '/';
      инако ако (*it == '/')
        *it = '.';
    }

  врати ret;
}

цео
has_suffix (const char *str, const char *suffix)
{
  ако (! str || ! suffix)
    врати 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  ако (lensuffix > lenstr)
    врати 0;
  врати memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

char *
str_join_array (цео offset, size_t size, char *const array[], const char *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = NULL;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  char *result, *p;

  lens = xmalloc (size * sizeof (*lens));

  за (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  за (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      ако (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  врати result;
}

цео
libcrun_mmap (struct libcrun_mmap_s **ret, void *addr, size_t length,
              цео prot, цео flags, цео fd, off_t offset,
              libcrun_error_t *err)
{
  struct libcrun_mmap_s *mmap_s = NULL;

  void *mapped = mmap (addr, length, prot, flags, fd, offset);
  ако (mapped == MAP_FAILED)
    врати crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (sizeof (struct libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  врати 0;
}

цео
libcrun_munmap (struct libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  цео ret;

  ret = munmap (mmap->addr, mmap->length);
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "munmap");

  free (mmap);

  врати 0;
}

static long
read_file_as_long_or_default (const char *path, long def_value)
{
  cleanup_free char *content = NULL;
  libcrun_error_t tmp_err = NULL;
  char *endptr;
  long val;
  цео ret;

  ret = read_all_file (path, &content, NULL, &tmp_err);
  ако (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      врати def_value;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  ако (UNLIKELY (errno))
    врати def_value;
  ако (endptr == content || (*endptr && *endptr != '\n'))
    врати def_value;
  врати val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (void)
{
  static atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  ако (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  врати uid;
}

gid_t
get_overflow_gid (void)
{
  static atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  ако (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  врати gid;
}

void
consume_trailing_slashes (char *path)
{
  ако (! path || path[0] == '\0')
    врати;

  char *last = path + strlen (path);

  док (last > path && *(last - 1) == '/')
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
  ако (UNLIKELY (dir == NULL))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      врати NULL;
    }

  entries = xmalloc (entries_size * sizeof (char *));
  док ((de = readdir (dir)))
    {
      ако (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;
      ако (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * sizeof (char *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * sizeof (char *));
  entries[n_entries] = NULL;

  врати entries;
}

цео
cpuset_string_to_bitmask (const char *str, char **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free char *mask = NULL;
  size_t mask_size = 0;
  const char *p = str;
  char *endptr;

  док (*p)
    {
      long long start_range, end_range;

      ако (*p < '0' || *p > '9')
        goto invalid_input;

      start_range = strtoll (p, &endptr, 10);
      ако (start_range < 0)
        goto invalid_input;

      p = endptr;

      ако (*p != '-')
        end_range = start_range;
      инако
        {
          p++;

          ако (*p < '0' || *p > '9')
            goto invalid_input;

          end_range = strtoll (p, &endptr, 10);

          ако (end_range < start_range)
            goto invalid_input;

          p = endptr;
        }

      /* Just set some limit.  */
      ако (end_range > (1 << 20))
        goto invalid_input;

      ако (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      за (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      ако (*p == ',')
        p++;
      инако ако (*p)
        goto invalid_input;
    }

  *out = mask;
  mask = NULL;
  *out_size = mask_size;

  врати 0;

invalid_input:
  врати crun_make_error (err, 0, "cannot parse input `%s`", str);
}

struct channel_fd_pair
{
  struct ring_buffer *rb;

  цео in_fd;
  цео out_fd;

  цео infd_epoll_events;
  цео outfd_epoll_events;
};

struct channel_fd_pair *
channel_fd_pair_new (цео in_fd, цео out_fd, size_t size)
{
  struct channel_fd_pair *channel = xmalloc (sizeof (struct channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  врати channel;
}

void
channel_fd_pair_free (struct channel_fd_pair *channel)
{
  ако (channel == NULL)
    врати;

  ring_buffer_free (channel->rb);
  free (channel);
}

цео
channel_fd_pair_process (struct channel_fd_pair *channel, цео epollfd, libcrun_error_t *err)
{
  bool is_input_eagain = false, is_output_eagain = false, repeat;
  цео ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  за (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      ако (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_input_eagain, err);
          ако (UNLIKELY (ret < 0))
            врати ret;
          ако (ret > 0)
            repeat = true;
        }
      ако (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          ако (UNLIKELY (ret < 0))
            врати ret;
          ако (ret > 0)
            repeat = true;
        }
    }

  ако (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      цео events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_input_eagain ? EPOLLET : 0)) : 0;
      ако (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          ако (UNLIKELY (ret < 0))
            врати ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      ако (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          ако (UNLIKELY (ret < 0))
            врати ret;
          channel->outfd_epoll_events = events;
        }
    }
  врати 0;
}

static цео
open_proc (libcrun_error_t *err)
{
  цео fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close цео fsfd = -1;
  цео ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  ако (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
      ако (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          ако (fd >= 0)
            врати fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  ако (fd < 0)
    врати crun_make_error (err, errno, "open `/proc`");
  врати fd;
}

цео
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  ако (! container)
    врати open_proc (err);

  ако (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  врати container->proc_fd;
}

цео
libcrun_open_proc_file (libcrun_container_t *container, const char *path, цео flags, libcrun_error_t *err)
{
  цео proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  ако (proc_fd < 0)
    врати proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  ако (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  ако (UNLIKELY (fd < 0))
    врати crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  врати fd;
}

цео
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const char *path, цео flags, libcrun_error_t *err)
{
  cleanup_free char *full_path = NULL;
  xasprintf (&full_path, "%d/%s", pid, path);
  врати libcrun_open_proc_file (container, full_path, flags, err);
}
