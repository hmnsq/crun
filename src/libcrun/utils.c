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
#nếu HAVE_STDATOMIC_H
#  include <stdatomic.h>
#không_thì
#  define atomic_long volatile long
#  define atomic_int volatile số_nguyên
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

số_nguyên
crun_path_exists (const char *path, libcrun_error_t *err)
{
  số_nguyên ret = access (path, F_OK);
  nếu (ret < 0)
    {
      nếu (errno == ENOENT)
        trả_về 0;
      trả_về crun_make_error (err, errno, "access `%s`", path);
    }
  trả_về 1;
}

số_nguyên
xasprintf (char **str, const char *fmt, ...)
{
  số_nguyên ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  nếu (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  trả_về ret;
}

số_nguyên
write_file_at_with_flags (số_nguyên dirfd, số_nguyên flags, mode_t mode, const char *name, const void *data, size_t len, libcrun_error_t *err)
{
  cleanup_close số_nguyên fd = -1;
  số_nguyên ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  nếu (UNLIKELY (fd < 0))
    trả_về crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  trả_về (len > INT_MAX) ? INT_MAX : (số_nguyên) len;
}

số_nguyên
detach_process ()
{
  pid_t pid;
  nếu (setsid () < 0)
    trả_về -1;
  pid = fork ();
  nếu (pid < 0)
    trả_về -1;
  nếu (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  trả_về 0;
}

số_nguyên
get_file_type_fd (số_nguyên fd, mode_t *mode)
{
  struct stat st;
  số_nguyên ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  nếu (UNLIKELY (ret < 0))
    {
      nếu (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      trả_về ret;
    }
  *mode = stx.stx_mode;
  trả_về ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  trả_về ret;
}

số_nguyên
get_file_type_at (số_nguyên dirfd, mode_t *mode, bool nofollow, const char *path)
{
  số_nguyên empty_path = path == NULL ? AT_EMPTY_PATH : 0;
  struct stat st;
  số_nguyên ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  nếu (UNLIKELY (ret < 0))
    {
      nếu (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      trả_về ret;
    }
  *mode = stx.stx_mode;
  trả_về ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  trả_về ret;
}

số_nguyên
get_file_type (mode_t *mode, bool nofollow, const char *path)
{
  trả_về get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

số_nguyên
create_file_if_missing_at (số_nguyên dirfd, const char *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close số_nguyên fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  nếu (fd_write < 0)
    {
      số_nguyên saved_errno = errno;
      mode_t tmp_mode;
      số_nguyên ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      nếu (ret == 0 && S_ISREG (tmp_mode))
        trả_về 0;

      trả_về crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  trả_về 0;
}

static số_nguyên
ensure_directory_internal_at (số_nguyên dirfd, char *path, size_t len, số_nguyên mode, libcrun_error_t *err)
{
  char *it = path + len;
  số_nguyên ret = 0;
  bool parent_created = false;

  cho (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      nếu (ret == 0 || errno == EEXIST)
        trả_về 0;

      số_nguyên saved_errno = errno;
      nếu (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = NULL;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          nếu (ret > 0)
            trả_về 0;
          nếu (ret < 0)
            crun_error_release (&tmp_err);

          trả_về crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      trong_khi (it > path && *it != '/')
        {
          it--;
          len--;
        }
      nếu (it == path)
        trả_về 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      nếu (UNLIKELY (ret < 0))
        trả_về ret;

      parent_created = true;
    }
  trả_về ret;
}

số_nguyên
crun_ensure_directory_at (số_nguyên dirfd, const char *path, số_nguyên mode, bool nofollow, libcrun_error_t *err)
{
  số_nguyên ret;
  cleanup_free char *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (ret == 0)
    trả_về crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  trả_về 0;
}

static số_nguyên
check_fd_is_path (const char *path, số_nguyên fd, const char *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  char link[PATH_MAX];
  số_nguyên ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, sizeof (link)));
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "readlink `%s`", fdname);

  nếu (((size_t) ret) != path_len || memcmp (link, path, path_len))
    trả_về crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  trả_về 0;
}

static số_nguyên
check_fd_under_path (const char *rootfs, size_t rootfslen, số_nguyên fd, const char *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  char link[PATH_MAX];
  số_nguyên ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  nếu (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    trả_về 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, sizeof (link)));
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "readlink `%s`", fdname);

  nếu (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    trả_về crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  trả_về 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
static void
close_and_replace (số_nguyên *oldfd, số_nguyên newfd)
{
  nếu (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
char *chroot_realpath (const char *chroot, const char *path, char resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
static số_nguyên
safe_openat_fallback (số_nguyên dirfd, const char *rootfs, const char *path, số_nguyên flags,
                      số_nguyên mode, libcrun_error_t *err)
{
  cleanup_free char *parent_path = NULL;
  const char *last_component = NULL;
  const char *orig_path = path;
  const char *path_in_chroot;
  cleanup_close số_nguyên fd = -1;
  char resolved[PATH_MAX];
  char buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  số_nguyên ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  nếu (flags & O_NOFOLLOW)
    {
      char *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      nếu (sep == NULL)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      không_thì nếu (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      nếu (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  nếu (path_in_chroot == NULL)
    trả_về crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  nếu (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  nếu (last_component)
    {
      ret = snprintf (resolved, sizeof (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      nếu (UNLIKELY (ret >= (số_nguyên) sizeof (resolved)))
        trả_về crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  nếu (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      nếu (UNLIKELY (ret < 0))
        trả_về crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      trả_về ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = fd;
  fd = -1;
  trả_về ret;
}

số_nguyên
safe_openat (số_nguyên dirfd, const char *rootfs, const char *path, số_nguyên flags, số_nguyên mode,
             libcrun_error_t *err)
{
  static bool openat2_supported = true;
  số_nguyên retries = MAX_OPENAT2_EAGAIN_RETRIES;
  số_nguyên ret;

  nếu (is_empty_string (path))
    {
      cleanup_close số_nguyên fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      nếu (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      nếu (UNLIKELY (fd < 0))
        trả_về crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      nếu (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
        }

      ret = fd;
      fd = -1;
      trả_về ret;
    }

  nếu (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_ROOT);
      nếu (UNLIKELY (ret < 0))
        {
          nếu (errno == EINTR)
            goto repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          nếu (errno == EAGAIN && retries-- > 0)
            goto repeat;
          nếu (errno == ENOSYS)
            openat2_supported = false;
          nếu (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            trả_về safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          trả_về crun_make_error (err, errno, "openat2 `%s`", path);
        }

      trả_về ret;
    }

  trả_về safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (số_nguyên dfd, const char *name, char **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free char *tmp_buf = NULL;
  ssize_t size;

  do
    {
      nếu (tmp_buf != NULL)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      nếu (UNLIKELY (size < 0))
        trả_về crun_make_error (err, errno, "readlink `%s`", name);
  } trong_khi (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = NULL;

  trả_về size;
}

static số_nguyên crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, số_nguyên dirfd, const char *dirpath,
                                const char *path, số_nguyên mode, số_nguyên max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
static số_nguyên
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, số_nguyên dirfd, const char *dirpath, số_nguyên cwd,
                                 const char *npath, const char *cur, const char *rest,
                                 số_nguyên mode, số_nguyên max_readlinks, libcrun_error_t *err)
{
  cleanup_free char *target = NULL;
  cleanup_free char *new_path = NULL;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (số_nguyên) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  trả_về crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

static số_nguyên
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, số_nguyên dirfd, const char *dirpath,
                     const char *path, số_nguyên mode, số_nguyên max_readlinks, libcrun_error_t *err)
{
  cleanup_close số_nguyên wd_cleanup = -1;
  cleanup_free char *npath = NULL;
  bool last_component = false;
  size_t depth = 0;
  char *cur;
  char *it;
  số_nguyên cwd;
  số_nguyên ret;

  nếu (max_readlinks <= 0)
    trả_về crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  nếu (*path == '\0')
    {
      nếu (do_open)
        trả_về open (dirpath, O_CLOEXEC | O_PATH, 0);
      trả_về 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  trong_khi (cur)
    {
      nếu (it)
        *it = '\0';
      không_thì
        last_component = true;

      nếu (cur[0] == '\0')
        break;

      nếu (strcmp (cur, ".") == 0)
        goto next;
      không_thì nếu (strcmp (cur, ".."))
        depth++;
      không_thì
        {
          nếu (depth)
            depth--;
          không_thì
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              goto next;
            }
        }

      nếu (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          nếu (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              nếu (errno == ELOOP)
                trả_về crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, NULL,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              nếu (ret < 0)
                trả_về crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          nếu (do_open)
            trả_về ret;

          close_and_replace (&wd_cleanup, ret);
          trả_về 0;
        }

      ret = mkdirat (cwd, cur, mode);
      nếu (ret < 0)
        {
          mode_t st_mode;

          nếu (errno != EEXIST)
            trả_về crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          nếu (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            trả_về crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : NULL, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      nếu (UNLIKELY (cwd < 0))
        trả_về crun_error_wrap (err, "creating `/%s`", path);

      nếu (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, NULL);
          nếu (UNLIKELY (ret < 0))
            {
              số_nguyên saved_errno = errno;

              close (cwd);
              trả_về crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          nếu ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              trả_về crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      nếu (it == NULL)
        break;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  nếu (do_open)
    {
      nếu (cwd == dirfd)
        {
          ret = dup (dirfd);
          nếu (UNLIKELY (ret < 0))
            trả_về crun_make_error (err, errno, "dup `%s`", dirpath);
          trả_về ret;
        }

      wd_cleanup = -1;
      trả_về cwd;
    }

  trả_về 0;
}

số_nguyên
crun_safe_create_and_open_ref_at (bool dir, số_nguyên dirfd, const char *dirpath, const char *path, số_nguyên mode, libcrun_error_t *err)
{
  số_nguyên ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  nếu (LIKELY (ret >= 0))
    trả_về ret;

  crun_error_release (err);
  trả_về crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

số_nguyên
crun_safe_ensure_directory_at (số_nguyên dirfd, const char *dirpath, const char *path, số_nguyên mode,
                               libcrun_error_t *err)
{
  trả_về crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

số_nguyên
crun_safe_ensure_file_at (số_nguyên dirfd, const char *dirpath, const char *path, số_nguyên mode,
                          libcrun_error_t *err)
{
  trả_về crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

số_nguyên
crun_ensure_directory (const char *path, số_nguyên mode, bool nofollow, libcrun_error_t *err)
{
  trả_về crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

static số_nguyên
get_file_size (số_nguyên fd, off_t *size)
{
  struct stat st;
  số_nguyên ret;
#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  nếu (UNLIKELY (ret < 0))
    {
      nếu (errno == ENOSYS || errno == EINVAL)
        goto fallback;
      trả_về ret;
    }
  *size = stx.stx_size;

  trả_về ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  trả_về ret;
}

số_nguyên
crun_dir_p_at (số_nguyên dirfd, const char *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  số_nguyên ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "stat `%s`", path);

  trả_về S_ISDIR (mode) ? 1 : 0;
}

số_nguyên
crun_dir_p (const char *path, bool nofollow, libcrun_error_t *err)
{
  trả_về crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
static số_nguyên
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  size_t len;
  số_nguyên ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  nếu (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      nếu (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          trả_về 0;
        }
      trả_về ret;
    }

  trả_về strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
static bool
running_in_openvz ()
{
  nếu (access ("/proc/vz", F_OK) < 0)
    trả_về false;

  trả_về access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

số_nguyên
check_running_in_user_namespace (libcrun_error_t *err)
{
  static atomic_int run_in_userns = -1;
  struct stat st;
  số_nguyên ret;

  ret = run_in_userns;
  nếu (ret >= 0)
    trả_về ret;

  ret = stat ("/proc/self/ns/user", &st);
  nếu (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      nếu (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          trả_về run_in_userns;
        }

      nếu (! running_in_openvz ())
        {
          run_in_userns = 1;
          trả_về run_in_userns;
        }
    }
  không_thì nếu (UNLIKELY (errno != ENOENT))
    trả_về crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  run_in_userns = ret;
  trả_về run_in_userns;
}

static size_t
get_page_size ()
{
  static atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  nếu (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  trả_về (size_t) pagesize;
}

static số_nguyên selinux_enabled = -1;
static số_nguyên apparmor_enabled = -1;

số_nguyên
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free char *out = NULL;
  cleanup_close số_nguyên fd = -1;
  size_t len;
  số_nguyên ret;

  nếu (selinux_enabled >= 0)
    trả_về selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  nếu (UNLIKELY (fd < 0))
    trả_về fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  trả_về selinux_enabled;
}

số_nguyên
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close số_nguyên fd = -1;
  số_nguyên size;
  char buf[1];

  nếu (apparmor_enabled >= 0)
    trả_về apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  nếu (fd == -1)
    {
      nếu (errno == ENOENT)
        {
          apparmor_enabled = 0;
          trả_về 0;
        }

      trả_về crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (read (fd, buf, sizeof (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  trả_về apparmor_enabled;
}

static số_nguyên
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  nếu (selinux_enabled < 0)
    trả_về crun_make_error (err, 0, "SELinux is not initialized correctly");
  trả_về selinux_enabled;
}

số_nguyên
add_selinux_mount_label (char **retlabel, const char *data, const char *label, const char *context_type, libcrun_error_t *err)
{
  số_nguyên ret;

  ret = libcrun_is_selinux_enabled (err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (label && ret)
    {
      nếu (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      không_thì
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      trả_về 0;
    }
  *retlabel = xstrdup (data);
  trả_về 0;
}

static const char *
lsm_attr_path (libcrun_container_t *container, const char *lsm, const char *fname, libcrun_error_t *err)
{
  cleanup_close số_nguyên attr_dirfd = -1;
  cleanup_close số_nguyên lsm_dirfd = -1;
  char *attr_path = NULL;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  nếu (UNLIKELY (attr_dirfd < 0))
    trả_về NULL;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  nếu (lsm != NULL)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      nếu (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          trả_về NULL;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  trả_về attr_path;
}

static số_nguyên
check_proc_super_magic (số_nguyên fd, const char *path, libcrun_error_t *err)
{
  struct statfs sfs;

  số_nguyên ret = fstatfs (fd, &sfs);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "statfs `%s`", path);

  nếu (sfs.f_type != PROC_SUPER_MAGIC)
    trả_về crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  trả_về 0;
}

static số_nguyên
set_security_attr (libcrun_container_t *container, const char *lsm, const char *fname, const char *data, libcrun_error_t *err)
{
  số_nguyên ret;

  cleanup_free const char *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close số_nguyên fd = -1;

  nếu (UNLIKELY (attr_path == NULL))
    trả_về -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  nếu (UNLIKELY (fd < 0))
    trả_về fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (write (fd, data, strlen (data)));
  nếu (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      nếu (errno == ENOENT && lsm != NULL && strcmp (lsm, "apparmor") == 0)
        {
          const char *profile = strchr (data, ' ');

          trả_về crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      trả_về crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  trả_về 0;
}

số_nguyên
set_selinux_label (libcrun_container_t *container, const char *label, bool now, libcrun_error_t *err)
{
  số_nguyên ret;

  ret = libcrun_is_selinux_enabled (err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (ret)
    trả_về set_security_attr (container, NULL, now ? "current" : "exec", label, err);
  trả_về 0;
}

static số_nguyên
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  nếu (apparmor_enabled < 0)
    trả_về crun_make_error (err, 0, "AppArmor is not initialized correctly");
  trả_về apparmor_enabled;
}

static số_nguyên
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (sizeof (UNCONFINED) - 1)
  cleanup_free const char *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close số_nguyên fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  char buf[UNCONFINED_LEN + 1];

  nếu (UNLIKELY (attr_path == NULL))
    trả_về -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  nếu (UNLIKELY (fd < 0))
    trả_về fd;

  nếu (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    trả_về -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (read (fd, buf, sizeof (buf)));
  nếu (UNLIKELY (bytes_read < 0))
    trả_về crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  nếu (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    trả_về 1;
  nếu (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    trả_về 0;
  trả_về 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

số_nguyên
set_apparmor_profile (libcrun_container_t *container, const char *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  số_nguyên ret;

  ret = libcrun_is_apparmor_enabled (err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  nếu (ret)
    {
      cleanup_free char *buf = NULL;
      ret = is_current_process_confined (container, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      trả_về set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  trả_về 0;
}

static số_nguyên
read_all_fd_internal (số_nguyên fd, const char *description, char **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free char *buf = NULL;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  số_nguyên ret;

  nếu (size_hint)
    allocated = size_hint;
  không_thì
    {
      ret = get_file_size (fd, &size);
      nếu (UNLIKELY (ret < 0))
        trả_về crun_make_error (err, errno, "stat `%s`", description);

      nếu (max_size && (size_t) size > max_size)
        trả_về crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  trong_khi ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (read (fd, buf + nread, allocated - nread));
      nếu (UNLIKELY (ret < 0))
        trả_về crun_make_error (err, errno, "read from file `%s`", description);

      nếu (ret == 0)
        break;

      nread += ret;

      nếu (max_size && nread > max_size)
        trả_về crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      nếu (nread == allocated)
        {
          nếu (size)
            break;

          nếu (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  nếu (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      char *tmp = realloc (buf, nread + 1);
      nếu (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = NULL;
  nếu (len)
    *len = nread;
  trả_về 0;
}

số_nguyên
read_all_fd_with_size_hint (số_nguyên fd, const char *description, char **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  trả_về read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

số_nguyên
read_all_fd_with_max_size (số_nguyên fd, const char *description, char **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  trả_về read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

số_nguyên
read_all_file_at (số_nguyên dirfd, const char *path, char **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close số_nguyên fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  nếu (UNLIKELY (fd < 0))
    trả_về crun_make_error (err, errno, "open `%s`", path);

  trả_về read_all_fd (fd, path, out, len, err);
}

số_nguyên
read_all_file (const char *path, char **out, size_t *len, libcrun_error_t *err)
{
  nếu (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  trả_về read_all_file_at (AT_FDCWD, path, out, len, err);
}

số_nguyên
get_realpath_to_file (số_nguyên dirfd, const char *path_name, char **absolute_path, libcrun_error_t *err)
{
  cleanup_close số_nguyên targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  nếu (UNLIKELY (targetfd < 0))
    trả_về crun_make_error (err, errno, "open `%s`", path_name);
  không_thì
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      nếu (UNLIKELY (len < 0))
        trả_về crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  trả_về 0;
}

số_nguyên
open_unix_domain_client_socket (const char *path, số_nguyên dgram, libcrun_error_t *err)
{
  struct sockaddr_un addr = {};
  số_nguyên ret;
  proc_fd_path_t name_buf;
  cleanup_close số_nguyên destfd = -1;
  cleanup_close số_nguyên fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  nếu (UNLIKELY (fd < 0))
    trả_về crun_make_error (err, errno, "create UNIX socket");

  nếu (strlen (path) >= sizeof (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      nếu (UNLIKELY (destfd < 0))
        trả_về crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  nếu (path_len >= sizeof (addr.sun_path))
    trả_về crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (struct sockaddr *) &addr, sizeof (addr));
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  trả_về ret;
}

số_nguyên
open_unix_domain_socket (const char *path, số_nguyên dgram, libcrun_error_t *err)
{
  struct sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  số_nguyên ret;
  cleanup_close số_nguyên dirfd = -1;
  cleanup_close số_nguyên fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  nếu (UNLIKELY (fd < 0))
    trả_về crun_make_error (err, errno, "create UNIX socket");

  nếu (strlen (path) >= sizeof (addr.sun_path))
    {
      cleanup_free char *dpath = xstrdup (path);
      cleanup_free char *bpath = xstrdup (path);
      const char *parent_dir = dirname (dpath);
      const char *base = basename (bpath);
      số_nguyên n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      nếu (UNLIKELY (dirfd < 0))
        trả_về crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, sizeof (addr.sun_path), "%s/%s", name_buf, base);
      nếu (n < 0 || (size_t) n >= sizeof (addr.sun_path))
        trả_về crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  không_thì
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (struct sockaddr *) &addr, sizeof (addr));
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "bind socket to `%s`", path);

  nếu (! dgram)
    {
      ret = listen (fd, 1);
      nếu (UNLIKELY (ret < 0))
        trả_về crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  trả_về ret;
}

số_nguyên
send_fd_to_socket (số_nguyên server, số_nguyên fd, libcrun_error_t *err)
{
  trả_về send_fd_to_socket_with_payload (server, fd, NULL, 0, err);
}

số_nguyên
send_fd_to_socket_with_payload (số_nguyên server, số_nguyên fd, const char *payload, size_t payload_len, libcrun_error_t *err)
{
  số_nguyên ret;
  struct cmsghdr *cmsg = NULL;
  struct iovec iov[2];
  struct msghdr msg = {};
  char ctrl_buf[CMSG_SPACE (1 + sizeof (số_nguyên))] = {};
  char data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = sizeof (data);

  nếu (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (sizeof (số_nguyên));
  msg.msg_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (sizeof (số_nguyên));

  *((số_nguyên *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &msg, 0));
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "sendmsg");
  trả_về 0;
}

số_nguyên
receive_fd_from_socket_with_payload (số_nguyên from, char *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close số_nguyên fd = -1;
  số_nguyên ret;
  struct iovec iov[1];
  struct msghdr msg = {};
  char ctrl_buf[CMSG_SPACE (sizeof (số_nguyên))] = {};
  char data[1];
  struct cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = sizeof (data);

  nếu (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (sizeof (số_nguyên));
  msg.msg_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &msg, 0));
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "recvmsg");
  nếu (UNLIKELY (ret == 0))
    trả_về crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&msg);
  nếu (cmsg == NULL)
    trả_về crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), sizeof (fd));

  ret = fd;
  fd = -1;
  trả_về ret;
}

số_nguyên
receive_fd_from_socket (số_nguyên from, libcrun_error_t *err)
{
  trả_về receive_fd_from_socket_with_payload (from, NULL, 0, err);
}

số_nguyên
create_socket_pair (số_nguyên *pair, libcrun_error_t *err)
{
  số_nguyên ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "socketpair");
  trả_về 0;
}

số_nguyên
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  số_nguyên ret = signalfd (-1, mask, 0);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "signalfd");
  trả_về ret;
}

static số_nguyên
epoll_helper_toggle (số_nguyên epollfd, số_nguyên fd, số_nguyên events, libcrun_error_t *err)
{
  struct epoll_event ev = {};
  bool add = events != 0;
  số_nguyên ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  nếu (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  nếu (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      nếu (errno == ENOENT || errno == EPERM)
        trả_về 0;
      trả_về crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  trả_về 0;
}

số_nguyên
epoll_helper (số_nguyên *in_fds, số_nguyên *in_levelfds, số_nguyên *out_fds, số_nguyên *out_levelfds, libcrun_error_t *err)
{
  struct epoll_event ev;
  cleanup_close số_nguyên epollfd = -1;
  số_nguyên ret;

  số_nguyên *it;
  epollfd = epoll_create1 (0);
  nếu (UNLIKELY (epollfd < 0))
    trả_về crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  cho (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      nếu (UNLIKELY (ret < 0))                                           \
        trả_về crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  nếu (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  nếu (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  nếu (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  nếu (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  trả_về ret;
}

số_nguyên
copy_from_fd_to_fd (số_nguyên src, số_nguyên dst, số_nguyên consume, libcrun_error_t *err)
{
  số_nguyên ret;
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
      nếu (can_copy_file_range)
        {
          nread = copy_file_range (src, NULL, dst, NULL, pagesize, 0);
          nếu (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              goto fallback;
            }
          nếu (consume && nread < 0 && errno == EAGAIN)
            trả_về 0;
          nếu (nread < 0 && errno == EIO)
            trả_về 0;
          nếu (UNLIKELY (nread < 0))
            trả_về crun_make_error (err, errno, "copy_file_range");
          continue;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (read (src, buffer, pagesize));
      nếu (consume && nread < 0 && errno == EAGAIN)
        trả_về 0;
      nếu (nread < 0 && errno == EIO)
        trả_về 0;
      nếu (UNLIKELY (nread < 0))
        trả_về crun_make_error (err, errno, "read");

      remaining = nread;
      trong_khi (remaining)
        {
          ret = TEMP_FAILURE_RETRY (write (dst, buffer + nread - remaining, remaining));
          nếu (UNLIKELY (ret < 0))
            trả_về crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } trong_khi (consume && nread);

  trả_về 0;
}

số_nguyên
run_process (char **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  nếu (UNLIKELY (pid < 0))
    trả_về crun_make_error (err, errno, "fork");
  nếu (pid)
    {
      số_nguyên r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      nếu (r < 0)
        trả_về crun_make_error (err, errno, "waitpid");
      nếu (WIFEXITED (status) || WIFSIGNALED (status))
        trả_về WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
static unsigned
atou (char **s)
{
  unsigned x;
  cho (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  trả_về x;
}

số_nguyên
fgetpwent_r (FILE *f, struct passwd *pw, char *line, size_t size, struct passwd **res)
{
  char *s;
  số_nguyên rv = 0;
  cho (;;)
    {
      line[size - 1] = '\xff';
      nếu ((fgets (line, size, f) == NULL) || ferror (f) || line[size - 1] != '\xff')
        {
          rv = (line[size - 1] != '\xff') ? ERANGE : ENOENT;
          line = 0;
          pw = 0;
          break;
        }
      line[strcspn (line, "\n")] = 0;

      s = line;
      pw->pw_name = s++;
      nếu (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_passwd = s;
      nếu (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      nếu (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      nếu (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gecos = s;
      nếu (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_dir = s;
      nếu (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_shell = s;
      break;
    }
  *res = pw;
  nếu (rv)
    errno = rv;
  trả_về rv;
}
#endif

số_nguyên
set_home_env (uid_t id)
{
  struct passwd pwd;
  cleanup_free char *buf = NULL;
  long buf_size;
  cleanup_file FILE *stream = NULL;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  nếu (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  nếu (stream == NULL)
    trả_về -1;

  cho (;;)
    {
      struct passwd *ret_pw = NULL;

      số_nguyên ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      nếu (UNLIKELY (ret != 0))
        {
          nếu (ret == EINTR)
            continue;

          nếu (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              trả_về -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          continue;
        }

      nếu (ret_pw && ret_pw->pw_uid == id)
        {
          nếu (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          không_thì
            trả_về 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
static số_nguyên
getsubidrange (uid_t id, số_nguyên is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *input = NULL;
  cleanup_free char *lineptr = NULL;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free char *buf = NULL;
  const char *name;
  struct passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  nếu (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  cho (;;)
    {
      số_nguyên ret;
      struct passwd *ret_pw = NULL;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      nếu (LIKELY (ret == 0))
        {
          nếu (ret_pw)
            {
              name = ret_pw->pw_name;
              break;
            }
          trả_về -1;
        }

      nếu (ret != ERANGE)
        trả_về -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  input = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  nếu (input == NULL)
    trả_về -1;

  cho (;;)
    {
      char *endptr;
      ssize_t read = getline (&lineptr, &lenlineptr, input);
      nếu (read < 0)
        trả_về -1;

      nếu (read < (ssize_t) (len_name + 2))
        continue;

      nếu (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        continue;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      nếu (endptr >= &lineptr[read])
        trả_về -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      trả_về 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

số_nguyên
format_default_id_mapping (char **out, uid_t container_id, uid_t host_uid, uid_t host_id, số_nguyên is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free char *buffer = NULL;
  số_nguyên written = 0;
  số_nguyên ret, remaining;

  *out = NULL;

  nếu (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    trả_về 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  nếu (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      nếu (UNLIKELY (ret >= remaining))
        trả_về crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  nếu (UNLIKELY (ret >= remaining))
    trả_về crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  nếu (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      nếu (UNLIKELY (ret >= remaining))
        trả_về crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = NULL;
  trả_về written;
}

static số_nguyên
unset_cloexec_flag (số_nguyên fd)
{
  số_nguyên flags = fcntl (fd, F_GETFD);
  nếu (flags == -1)
    trả_về -1;

  flags &= ~FD_CLOEXEC;

  trả_về fcntl (fd, F_SETFD, flags);
}

static void __attribute__ ((__noreturn__))
run_process_child (char *path, char **args, const char *cwd, char **envp, số_nguyên pipe_r,
                   số_nguyên pipe_w, số_nguyên out_fd, số_nguyên err_fd, bool can_ignore_chdir_errors)
{
  char *tmp_args[] = { path, NULL };
  libcrun_error_t err = NULL;
  số_nguyên dev_null_fd = -1;
  số_nguyên ret;

  ret = mark_or_close_fds_ge_than (NULL, 3, false, &err);
  nếu (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->msg);

  nếu (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      nếu (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  nếu (out_fd >= 0)
    unset_cloexec_flag (1);
  nếu (err_fd >= 0)
    unset_cloexec_flag (2);

  nếu (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  nếu (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  nếu (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  nếu (args == NULL)
    args = tmp_args;

  nếu (cwd && chdir (cwd) < 0)
    {
      nếu (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
số_nguyên
run_process_with_stdin_timeout_envp (char *path, char **args, const char *cwd, số_nguyên timeout,
                                     char **envp, char *stdin, size_t stdin_len, số_nguyên out_fd,
                                     số_nguyên err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  số_nguyên stdin_pipe[2];
  pid_t pid;
  số_nguyên ret;
  cleanup_close số_nguyên pipe_r = -1;
  cleanup_close số_nguyên pipe_w = -1;
  sigset_t oldmask, mask;
  số_nguyên r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  nếu (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      nếu (UNLIKELY (ret < 0))
        trả_về crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  nếu (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      goto restore_sig_mask_and_exit;
    }

  nếu (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (write (pipe_w, stdin, stdin_len));
  nếu (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      nếu (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          goto restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  nếu (timeout)
    {
      time_t start = time (NULL);
      time_t now;
      cho (now = start; now - start < timeout; now = time (NULL))
        {
          siginfo_t info;
          số_nguyên elapsed = now - start;
          struct timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          nếu (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              goto restore_sig_mask_and_exit;
            }

          nếu (info.si_signo == SIGCHLD && info.si_pid == pid)
            goto read_waitpid;

          nếu (ret < 0 && errno == EAGAIN)
            goto timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      goto restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  nếu (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  không_thì
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  nếu (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      nếu (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, NULL);
      nếu (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  trả_về ret;
}

số_nguyên
mark_or_close_fds_ge_than (libcrun_container_t *container, số_nguyên n, bool close_now, libcrun_error_t *err)
{
  cleanup_close số_nguyên cfd = -1;
  cleanup_dir DIR *dir = NULL;
  số_nguyên ret;
  số_nguyên fd;
  struct dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  nếu (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  nếu (ret == 0)
    {
      nếu (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      trả_về 0;
    }
  nếu (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    trả_về crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  nếu (UNLIKELY (cfd < 0))
    trả_về cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  dir = fdopendir (cfd);
  nếu (UNLIKELY (dir == NULL))
    trả_về crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  cho (next = readdir (dir); next; next = readdir (dir))
    {
      số_nguyên val;
      const char *name = next->d_name;
      nếu (name[0] == '.')
        continue;

      val = strtoll (name, NULL, 10);
      nếu (val < n || val == fd)
        continue;

      nếu (close_now)
        {
          ret = close (val);
          nếu (UNLIKELY (ret < 0))
            trả_về crun_make_error (err, errno, "close fd `%d`", val);

          nếu (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      không_thì
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          nếu (UNLIKELY (ret < 0))
            trả_về crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  trả_về 0;
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

  (void) snprintf (out, len, "%s.%06lldZ", timestamp, (long long số_nguyên) tv.tv_usec);
  out[len - 1] = '\0';
}

số_nguyên
set_blocking_fd (số_nguyên fd, bool blocking, libcrun_error_t *err)
{
  số_nguyên ret, flags = fcntl (fd, F_GETFL, 0);
  nếu (UNLIKELY (flags < 0))
    trả_về crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "fcntl");
  trả_về 0;
}

số_nguyên
parse_json_file (json_object **out, const char *jsondata, struct parser_context *ctx arg_unused, libcrun_error_t *err)
{
  struct json_tokener *tok;
  enum json_tokener_error jerr;

  *err = NULL;

  nếu (UNLIKELY (jsondata == NULL))
    trả_về crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  nếu (UNLIKELY (tok == NULL))
    trả_về crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (số_nguyên) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  nếu (*out == NULL || jerr != json_tokener_success)
    {
      số_nguyên saved_pos = (số_nguyên) tok->char_offset;

      json_tokener_free (tok);
      nếu (*out)
        {
          json_object_put (*out);
          *out = NULL;
        }
      trả_về crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  trả_về 0;
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
static số_nguyên
check_access (const char *path)
{
  số_nguyên ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#không_thì
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  nếu (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      nếu (errno == EACCES)
        {
          số_nguyên saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          nếu (ret == 0)
            trả_về CHECK_ACCESS_NOT_EXECUTABLE;
        }
      trả_về -errno;
    }

  ret = get_file_type (&mode, false, path);
  nếu (UNLIKELY (ret < 0))
    trả_về -errno;

  nếu (! S_ISREG (mode))
    trả_về CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  trả_về 0;
}

số_nguyên
find_executable (char **out, const char *executable_path, const char *cwd, libcrun_error_t *err)
{
  cleanup_free char *cwd_executable_path = NULL;
  cleanup_free char *tmp = NULL;
  số_nguyên last_error = -ENOENT;
  char *it, *end;
  số_nguyên ret;

  *out = NULL;

  nếu (executable_path == NULL || executable_path[0] == '\0')
    trả_về crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  nếu (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free char *cwd_allocated = NULL;

      nếu (cwd == NULL)
        {
          cwd_allocated = getcwd (NULL, 0);
          nếu (cwd_allocated == NULL)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, NULL);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  nếu (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      nếu (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          trả_về 0;
        }
      last_error = ret;
      goto fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  trong_khi ((it = strsep (&end, ":")))
    {
      cleanup_free char *path = NULL;

      nếu (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, NULL);
      nếu (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          continue;
        }

      ret = check_access (path);
      nếu (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = NULL;
          trả_về 0;
        }

      nếu (ret == -ENOENT)
        continue;

      last_error = ret;
    }

fail:
  switch (last_error)
    {
    case CHECK_ACCESS_NOT_EXECUTABLE:
      trả_về crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    case CHECK_ACCESS_NOT_REGULAR:
      trả_về crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    default:
      errno = -last_error;
      nếu (errno == ENOENT)
        trả_về crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      trả_về crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

static ssize_t
safe_read_xattr (char **ret, số_nguyên sfd, const char *srcname, const char *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  trong_khi (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      nếu (UNLIKELY (s < 0))
        trả_về crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      nếu (s < current_size)
        break;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  nếu (s <= 0)
    trả_về s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = NULL;

  trả_về s;
}

static ssize_t
copy_xattr (số_nguyên sfd, số_nguyên dfd, const char *srcname, const char *destname, libcrun_error_t *err)
{
  cleanup_free char *buf = NULL;
  ssize_t xattr_len;
  char *it;

  xattr_len = flistxattr (sfd, NULL, 0);
  nếu (UNLIKELY (xattr_len < 0))
    {
      nếu (errno == ENOTSUP)
        trả_về 0;

      trả_về crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  nếu (xattr_len == 0)
    trả_về 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  nếu (UNLIKELY (xattr_len < 0))
    trả_về crun_make_error (err, errno, "flistxattr `%s`", srcname);

  cho (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free char *v = NULL;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      nếu (UNLIKELY (s < 0))
        trả_về s;

      s = fsetxattr (dfd, it, v, s, 0);
      nếu (UNLIKELY (s < 0))
        {
          nếu (errno == EINVAL || errno == EOPNOTSUPP)
            continue;

          trả_về crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  trả_về 0;
}

#endif

static số_nguyên
copy_rec_stat_file_at (số_nguyên dfd, const char *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  struct stat st;
  số_nguyên ret;

#ifdef HAVE_STATX
  struct statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  nếu (UNLIKELY (ret < 0))
    {
      nếu (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      trả_về ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  trả_về ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  trả_về ret;
}

số_nguyên
copy_recursive_fd_to_fd (số_nguyên srcdirfd, số_nguyên dfd, const char *srcname, const char *destname, libcrun_error_t *err)
{
  cleanup_close số_nguyên destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = NULL;
  struct dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  nếu (UNLIKELY (dsrcfd == NULL))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      trả_về crun_make_error (err, errno, "open directory `%s`", destname);
    }

  cho (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close số_nguyên srcfd = -1;
      cleanup_close số_nguyên destfd = -1;
      cleanup_free char *target_buf = NULL;
      số_nguyên ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      nếu (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      nếu (UNLIKELY (ret < 0))
        trả_về crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      switch (mode & S_IFMT)
        {
        case S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          nếu (UNLIKELY (srcfd < 0))
            trả_về crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          nếu (UNLIKELY (destfd < 0))
            trả_về crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;

#ifdef HAVE_FGETXATTR
          ret = (số_nguyên) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          break;

        case S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          nếu (UNLIKELY (ret < 0))
            trả_về crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          nếu (UNLIKELY (srcfd < 0))
            trả_về crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          nếu (UNLIKELY (destfd < 0))
            trả_về crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (số_nguyên) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
          break;

        case S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          nếu (UNLIKELY (ret < 0))
            trả_về crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          break;

        case S_IFBLK:
        case S_IFCHR:
        case S_IFIFO:
        case S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          nếu (UNLIKELY (ret < 0))
            trả_về crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          break;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      nếu (UNLIKELY (ret < 0))
        trả_về crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      nếu (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          nếu (errno == ENOTSUP)
            continue;

          trả_về crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  trả_về 0;
}

const char *
find_annotation (libcrun_container_t *container, const char *name)
{
  nếu (container->container_def->annotations == NULL)
    trả_về NULL;

  trả_về find_string_map_value (container->annotations, name);
}

số_nguyên
safe_write (số_nguyên fd, const char *fname, const void *buf, size_t count, libcrun_error_t *err)
{
  size_t written = 0;
  trong_khi (written < count)
    {
      ssize_t w = write (fd, (const char *) buf + written, count - written);
      nếu (UNLIKELY (w < 0))
        {
          nếu (errno == EINTR || errno == EAGAIN)
            continue;
          trả_về crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  trả_về 0;
}

#define MAX_PARTS 32

số_nguyên
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
  cho (;;)
    {
      const char *part;
      size_t size;

      part = va_arg (ap, const char *);
      nếu (part == NULL)
        break;

      nếu (n_parts == MAX_PARTS)
        {
          va_end (ap);
          trả_về crun_make_error (err, EINVAL, "too many paths specified");
        }

      nếu (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          trong_khi (part[0] == '/' && part[1] == '/')
            part++;
        }
      không_thì
        {
          /* And drop any initial '/' for other components.  */
          trong_khi (part[0] == '/')
            part++;
        }

      size = strlen (part);
      nếu (size == 0)
        continue;

      trong_khi (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  cho (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  cho (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      nếu (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  trả_về 0;
}

#nếu __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#không_thì
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
static số_nguyên
b64_input (char c)
{
  const char table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  số_nguyên i;

  cho (i = 0; i < 64; i++)
    {
      nếu (table[i] == c)
        trả_về i;
    }
  trả_về -1;
}

số_nguyên
base64_decode (const char *iptr, size_t isize, char *optr, size_t osize, size_t *nbytes)
{
  số_nguyên i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned char data[4];

  *nbytes = 0;
  trong_khi (consumed < isize && (*nbytes) + 3 < osize)
    {
      trong_khi ((i < 4) && (consumed < isize))
        {
          tmp = b64_input (*iptr++);
          consumed++;
          nếu (tmp != -1)
            {
              nếu (UNLIKELY (pad > 0))
                trả_về -1;
              data[i++] = tmp;
            }
          không_thì nếu (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      nếu (i == 4)
        {
          nếu (UNLIKELY (pad > 2))
            trả_về -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      không_thì
        {
          /* I did not get all the data.  */
          consumed -= i;
          trả_về consumed;
        }
      i = 0;
    }
  trả_về consumed;
}

char *
get_user_name (uid_t uid)
{
  struct passwd pd;
  struct passwd *temp_result_ptr;
  char pwdbuffer[200];
  nếu (! getpwuid_r (uid, &pd, pwdbuffer, sizeof (pwdbuffer), &temp_result_ptr))
    trả_về xstrdup (pd.pw_name);
  trả_về xstrdup ("");
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
  nếu (sep == NULL || *sep == '/')
    trả_về xstrdup (key);

  ret = xstrdup (key);
  cho (it = ret; *it; it++)
    {
      nếu (*it == '.')
        *it = '/';
      không_thì nếu (*it == '/')
        *it = '.';
    }

  trả_về ret;
}

số_nguyên
has_suffix (const char *str, const char *suffix)
{
  nếu (! str || ! suffix)
    trả_về 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  nếu (lensuffix > lenstr)
    trả_về 0;
  trả_về memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

char *
str_join_array (số_nguyên offset, size_t size, char *const array[], const char *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = NULL;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  char *result, *p;

  lens = xmalloc (size * sizeof (*lens));

  cho (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  cho (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      nếu (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  trả_về result;
}

số_nguyên
libcrun_mmap (struct libcrun_mmap_s **ret, void *addr, size_t length,
              số_nguyên prot, số_nguyên flags, số_nguyên fd, off_t offset,
              libcrun_error_t *err)
{
  struct libcrun_mmap_s *mmap_s = NULL;

  void *mapped = mmap (addr, length, prot, flags, fd, offset);
  nếu (mapped == MAP_FAILED)
    trả_về crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (sizeof (struct libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  trả_về 0;
}

số_nguyên
libcrun_munmap (struct libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  số_nguyên ret;

  ret = munmap (mmap->addr, mmap->length);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "munmap");

  free (mmap);

  trả_về 0;
}

static long
read_file_as_long_or_default (const char *path, long def_value)
{
  cleanup_free char *content = NULL;
  libcrun_error_t tmp_err = NULL;
  char *endptr;
  long val;
  số_nguyên ret;

  ret = read_all_file (path, &content, NULL, &tmp_err);
  nếu (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      trả_về def_value;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  nếu (UNLIKELY (errno))
    trả_về def_value;
  nếu (endptr == content || (*endptr && *endptr != '\n'))
    trả_về def_value;
  trả_về val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (void)
{
  static atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  nếu (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  trả_về uid;
}

gid_t
get_overflow_gid (void)
{
  static atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  nếu (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  trả_về gid;
}

void
consume_trailing_slashes (char *path)
{
  nếu (! path || path[0] == '\0')
    trả_về;

  char *last = path + strlen (path);

  trong_khi (last > path && *(last - 1) == '/')
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
  nếu (UNLIKELY (dir == NULL))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      trả_về NULL;
    }

  entries = xmalloc (entries_size * sizeof (char *));
  trong_khi ((de = readdir (dir)))
    {
      nếu (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;
      nếu (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * sizeof (char *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * sizeof (char *));
  entries[n_entries] = NULL;

  trả_về entries;
}

số_nguyên
cpuset_string_to_bitmask (const char *str, char **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free char *mask = NULL;
  size_t mask_size = 0;
  const char *p = str;
  char *endptr;

  trong_khi (*p)
    {
      long long start_range, end_range;

      nếu (*p < '0' || *p > '9')
        goto invalid_input;

      start_range = strtoll (p, &endptr, 10);
      nếu (start_range < 0)
        goto invalid_input;

      p = endptr;

      nếu (*p != '-')
        end_range = start_range;
      không_thì
        {
          p++;

          nếu (*p < '0' || *p > '9')
            goto invalid_input;

          end_range = strtoll (p, &endptr, 10);

          nếu (end_range < start_range)
            goto invalid_input;

          p = endptr;
        }

      /* Just set some limit.  */
      nếu (end_range > (1 << 20))
        goto invalid_input;

      nếu (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      cho (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      nếu (*p == ',')
        p++;
      không_thì nếu (*p)
        goto invalid_input;
    }

  *out = mask;
  mask = NULL;
  *out_size = mask_size;

  trả_về 0;

invalid_input:
  trả_về crun_make_error (err, 0, "cannot parse input `%s`", str);
}

struct channel_fd_pair
{
  struct ring_buffer *rb;

  số_nguyên in_fd;
  số_nguyên out_fd;

  số_nguyên infd_epoll_events;
  số_nguyên outfd_epoll_events;
};

struct channel_fd_pair *
channel_fd_pair_new (số_nguyên in_fd, số_nguyên out_fd, size_t size)
{
  struct channel_fd_pair *channel = xmalloc (sizeof (struct channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  trả_về channel;
}

void
channel_fd_pair_free (struct channel_fd_pair *channel)
{
  nếu (channel == NULL)
    trả_về;

  ring_buffer_free (channel->rb);
  free (channel);
}

số_nguyên
channel_fd_pair_process (struct channel_fd_pair *channel, số_nguyên epollfd, libcrun_error_t *err)
{
  bool is_input_eagain = false, is_output_eagain = false, repeat;
  số_nguyên ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  cho (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      nếu (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_input_eagain, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
          nếu (ret > 0)
            repeat = true;
        }
      nếu (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
          nếu (ret > 0)
            repeat = true;
        }
    }

  nếu (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      số_nguyên events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_input_eagain ? EPOLLET : 0)) : 0;
      nếu (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      nếu (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
          channel->outfd_epoll_events = events;
        }
    }
  trả_về 0;
}

static số_nguyên
open_proc (libcrun_error_t *err)
{
  số_nguyên fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close số_nguyên fsfd = -1;
  số_nguyên ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  nếu (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
      nếu (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          nếu (fd >= 0)
            trả_về fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  nếu (fd < 0)
    trả_về crun_make_error (err, errno, "open `/proc`");
  trả_về fd;
}

số_nguyên
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  nếu (! container)
    trả_về open_proc (err);

  nếu (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  trả_về container->proc_fd;
}

số_nguyên
libcrun_open_proc_file (libcrun_container_t *container, const char *path, số_nguyên flags, libcrun_error_t *err)
{
  số_nguyên proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  nếu (proc_fd < 0)
    trả_về proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  nếu (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  nếu (UNLIKELY (fd < 0))
    trả_về crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  trả_về fd;
}

số_nguyên
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const char *path, số_nguyên flags, libcrun_error_t *err)
{
  cleanup_free char *full_path = NULL;
  xasprintf (&full_path, "%d/%s", pid, path);
  trả_về libcrun_open_proc_file (container, full_path, flags, err);
}
