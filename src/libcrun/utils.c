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
#ប្រសិនបើ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#បើមិនដូច្នេះ
#  define atomic_long ca_ncala long
#  define atomic_int ca_ncala ចំនួនគត់
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

ចំនួនគត់
crun_path_exists (const var_na *path, libcrun_error_t *err)
{
  ចំនួនគត់ ret = access (path, F_OK);
  ប្រសិនបើ (ret < 0)
    {
      ប្រសិនបើ (errno == ENOENT)
        ត្រឡប់ 0;
      ត្រឡប់ crun_make_error (err, errno, "access `%s`", path);
    }
  ត្រឡប់ 1;
}

ចំនួនគត់
xasprintf (var_na **str, const var_na *fmt, ...)
{
  ចំនួនគត់ ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  ត្រឡប់ ret;
}

ចំនួនគត់
write_file_at_with_flags (ចំនួនគត់ dirfd, ចំនួនគត់ flags, mode_t mode, const var_na *name, const void *data, size_t len, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ fd = -1;
  ចំនួនគត់ ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ (len > INT_MAX) ? INT_MAX : (ចំនួនគត់) len;
}

ចំនួនគត់
detach_process ()
{
  pid_t pid;
  ប្រសិនបើ (setsid () < 0)
    ត្រឡប់ -1;
  pid = fork ();
  ប្រសិនបើ (pid < 0)
    ត្រឡប់ -1;
  ប្រសិនបើ (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  ត្រឡប់ 0;
}

ចំនួនគត់
get_file_type_fd (ចំនួនគត់ fd, mode_t *mode)
{
  sa_nracanaa stat st;
  ចំនួនគត់ ret;

#ifdef HAVE_STATX
  sa_nracanaa statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      ប្រសិនបើ (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      ត្រឡប់ ret;
    }
  *mode = stx.stx_mode;
  ត្រឡប់ ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  ត្រឡប់ ret;
}

ចំនួនគត់
get_file_type_at (ចំនួនគត់ dirfd, mode_t *mode, bool nofollow, const var_na *path)
{
  ចំនួនគត់ empty_path = path == NULL ? AT_EMPTY_PATH : 0;
  sa_nracanaa stat st;
  ចំនួនគត់ ret;

#ifdef HAVE_STATX
  sa_nracanaa statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      ប្រសិនបើ (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      ត្រឡប់ ret;
    }
  *mode = stx.stx_mode;
  ត្រឡប់ ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  ត្រឡប់ ret;
}

ចំនួនគត់
get_file_type (mode_t *mode, bool nofollow, const var_na *path)
{
  ត្រឡប់ get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

ចំនួនគត់
create_file_if_missing_at (ចំនួនគត់ dirfd, const var_na *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  ប្រសិនបើ (fd_write < 0)
    {
      ចំនួនគត់ saved_errno = errno;
      mode_t tmp_mode;
      ចំនួនគត់ ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      ប្រសិនបើ (ret == 0 && S_ISREG (tmp_mode))
        ត្រឡប់ 0;

      ត្រឡប់ crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
ensure_directory_internal_at (ចំនួនគត់ dirfd, var_na *path, size_t len, ចំនួនគត់ mode, libcrun_error_t *err)
{
  var_na *it = path + len;
  ចំនួនគត់ ret = 0;
  bool parent_created = false;

  សម្រាប់ (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      ប្រសិនបើ (ret == 0 || errno == EEXIST)
        ត្រឡប់ 0;

      ចំនួនគត់ saved_errno = errno;
      ប្រសិនបើ (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = NULL;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          ប្រសិនបើ (ret > 0)
            ត្រឡប់ 0;
          ប្រសិនបើ (ret < 0)
            crun_error_release (&tmp_err);

          ត្រឡប់ crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      ខណៈពេល (it > path && *it != '/')
        {
          it--;
          len--;
        }
      ប្រសិនបើ (it == path)
        ត្រឡប់ 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      parent_created = true;
    }
  ត្រឡប់ ret;
}

ចំនួនគត់
crun_ensure_directory_at (ចំនួនគត់ dirfd, const var_na *path, ចំនួនគត់ mode, bool nofollow, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  cleanup_free var_na *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (ret == 0)
    ត្រឡប់ crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  ត្រឡប់ 0;
}

static ចំនួនគត់
check_fd_is_path (const var_na *path, ចំនួនគត់ fd, const var_na *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  var_na link[PATH_MAX];
  ចំនួនគត់ ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, parimaa_na (link)));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "readlink `%s`", fdname);

  ប្រសិនបើ (((size_t) ret) != path_len || memcmp (link, path, path_len))
    ត្រឡប់ crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  ត្រឡប់ 0;
}

static ចំនួនគត់
check_fd_under_path (const var_na *rootfs, size_t rootfslen, ចំនួនគត់ fd, const var_na *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  var_na link[PATH_MAX];
  ចំនួនគត់ ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  ប្រសិនបើ (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    ត្រឡប់ 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, parimaa_na (link)));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "readlink `%s`", fdname);

  ប្រសិនបើ (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    ត្រឡប់ crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  ត្រឡប់ 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
static void
close_and_replace (ចំនួនគត់ *oldfd, ចំនួនគត់ newfd)
{
  ប្រសិនបើ (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
var_na *chroot_realpath (const var_na *chroot, const var_na *path, var_na resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
static ចំនួនគត់
safe_openat_fallback (ចំនួនគត់ dirfd, const var_na *rootfs, const var_na *path, ចំនួនគត់ flags,
                      ចំនួនគត់ mode, libcrun_error_t *err)
{
  cleanup_free var_na *parent_path = NULL;
  const var_na *last_component = NULL;
  const var_na *orig_path = path;
  const var_na *path_in_chroot;
  cleanup_close ចំនួនគត់ fd = -1;
  var_na resolved[PATH_MAX];
  var_na buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  ចំនួនគត់ ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  ប្រសិនបើ (flags & O_NOFOLLOW)
    {
      var_na *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      ប្រសិនបើ (sep == NULL)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      បើមិនដូច្នេះ ប្រសិនបើ (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      ប្រសិនបើ (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  ប្រសិនបើ (path_in_chroot == NULL)
    ត្រឡប់ crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  ប្រសិនបើ (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  ប្រសិនបើ (last_component)
    {
      ret = snprintf (resolved, parimaa_na (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      ប្រសិនបើ (UNLIKELY (ret >= (ចំនួនគត់) parimaa_na (resolved)))
        ត្រឡប់ crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  ប្រសិនបើ (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      ត្រឡប់ ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = fd;
  fd = -1;
  ត្រឡប់ ret;
}

ចំនួនគត់
safe_openat (ចំនួនគត់ dirfd, const var_na *rootfs, const var_na *path, ចំនួនគត់ flags, ចំនួនគត់ mode,
             libcrun_error_t *err)
{
  static bool openat2_supported = true;
  ចំនួនគត់ retries = MAX_OPENAT2_EAGAIN_RETRIES;
  ចំនួនគត់ ret;

  ប្រសិនបើ (is_empty_string (path))
    {
      cleanup_close ចំនួនគត់ fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      ប្រសិនបើ (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      ប្រសិនបើ (UNLIKELY (fd < 0))
        ត្រឡប់ crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      ប្រសិនបើ (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
        }

      ret = fd;
      fd = -1;
      ត្រឡប់ ret;
    }

  ប្រសិនបើ (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_ROOT);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          ប្រសិនបើ (errno == EINTR)
            goto repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          ប្រសិនបើ (errno == EAGAIN && retries-- > 0)
            goto repeat;
          ប្រសិនបើ (errno == ENOSYS)
            openat2_supported = false;
          ប្រសិនបើ (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            ត្រឡប់ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          ត្រឡប់ crun_make_error (err, errno, "openat2 `%s`", path);
        }

      ត្រឡប់ ret;
    }

  ត្រឡប់ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (ចំនួនគត់ dfd, const var_na *name, var_na **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free var_na *tmp_buf = NULL;
  ssize_t size;

  do
    {
      ប្រសិនបើ (tmp_buf != NULL)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      ប្រសិនបើ (UNLIKELY (size < 0))
        ត្រឡប់ crun_make_error (err, errno, "readlink `%s`", name);
  } ខណៈពេល (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = NULL;

  ត្រឡប់ size;
}

static ចំនួនគត់ crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, ចំនួនគត់ dirfd, const var_na *dirpath,
                                const var_na *path, ចំនួនគត់ mode, ចំនួនគត់ max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
static ចំនួនគត់
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, ចំនួនគត់ dirfd, const var_na *dirpath, ចំនួនគត់ cwd,
                                 const var_na *npath, const var_na *cur, const var_na *rest,
                                 ចំនួនគត់ mode, ចំនួនគត់ max_readlinks, libcrun_error_t *err)
{
  cleanup_free var_na *target = NULL;
  cleanup_free var_na *new_path = NULL;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (ចំនួនគត់) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  ត្រឡប់ crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

static ចំនួនគត់
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, ចំនួនគត់ dirfd, const var_na *dirpath,
                     const var_na *path, ចំនួនគត់ mode, ចំនួនគត់ max_readlinks, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ wd_cleanup = -1;
  cleanup_free var_na *npath = NULL;
  bool last_component = false;
  size_t depth = 0;
  var_na *cur;
  var_na *it;
  ចំនួនគត់ cwd;
  ចំនួនគត់ ret;

  ប្រសិនបើ (max_readlinks <= 0)
    ត្រឡប់ crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  ប្រសិនបើ (*path == '\0')
    {
      ប្រសិនបើ (do_open)
        ត្រឡប់ open (dirpath, O_CLOEXEC | O_PATH, 0);
      ត្រឡប់ 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  ខណៈពេល (cur)
    {
      ប្រសិនបើ (it)
        *it = '\0';
      បើមិនដូច្នេះ
        last_component = true;

      ប្រសិនបើ (cur[0] == '\0')
        bha_nga;

      ប្រសិនបើ (strcmp (cur, ".") == 0)
        goto next;
      បើមិនដូច្នេះ ប្រសិនបើ (strcmp (cur, ".."))
        depth++;
      បើមិនដូច្នេះ
        {
          ប្រសិនបើ (depth)
            depth--;
          បើមិនដូច្នេះ
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              goto next;
            }
        }

      ប្រសិនបើ (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              ប្រសិនបើ (errno == ELOOP)
                ត្រឡប់ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, NULL,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              ប្រសិនបើ (ret < 0)
                ត្រឡប់ crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          ប្រសិនបើ (do_open)
            ត្រឡប់ ret;

          close_and_replace (&wd_cleanup, ret);
          ត្រឡប់ 0;
        }

      ret = mkdirat (cwd, cur, mode);
      ប្រសិនបើ (ret < 0)
        {
          mode_t st_mode;

          ប្រសិនបើ (errno != EEXIST)
            ត្រឡប់ crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          ប្រសិនបើ (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            ត្រឡប់ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : NULL, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      ប្រសិនបើ (UNLIKELY (cwd < 0))
        ត្រឡប់ crun_error_wrap (err, "creating `/%s`", path);

      ប្រសិនបើ (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, NULL);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            {
              ចំនួនគត់ saved_errno = errno;

              close (cwd);
              ត្រឡប់ crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          ប្រសិនបើ ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              ត្រឡប់ crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      ប្រសិនបើ (it == NULL)
        bha_nga;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  ប្រសិនបើ (do_open)
    {
      ប្រសិនបើ (cwd == dirfd)
        {
          ret = dup (dirfd);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "dup `%s`", dirpath);
          ត្រឡប់ ret;
        }

      wd_cleanup = -1;
      ត្រឡប់ cwd;
    }

  ត្រឡប់ 0;
}

ចំនួនគត់
crun_safe_create_and_open_ref_at (bool dir, ចំនួនគត់ dirfd, const var_na *dirpath, const var_na *path, ចំនួនគត់ mode, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  ប្រសិនបើ (LIKELY (ret >= 0))
    ត្រឡប់ ret;

  crun_error_release (err);
  ត្រឡប់ crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ចំនួនគត់
crun_safe_ensure_directory_at (ចំនួនគត់ dirfd, const var_na *dirpath, const var_na *path, ចំនួនគត់ mode,
                               libcrun_error_t *err)
{
  ត្រឡប់ crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ចំនួនគត់
crun_safe_ensure_file_at (ចំនួនគត់ dirfd, const var_na *dirpath, const var_na *path, ចំនួនគត់ mode,
                          libcrun_error_t *err)
{
  ត្រឡប់ crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ចំនួនគត់
crun_ensure_directory (const var_na *path, ចំនួនគត់ mode, bool nofollow, libcrun_error_t *err)
{
  ត្រឡប់ crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

static ចំនួនគត់
get_file_size (ចំនួនគត់ fd, off_t *size)
{
  sa_nracanaa stat st;
  ចំនួនគត់ ret;
#ifdef HAVE_STATX
  sa_nracanaa statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      ប្រសិនបើ (errno == ENOSYS || errno == EINVAL)
        goto fallback;
      ត្រឡប់ ret;
    }
  *size = stx.stx_size;

  ត្រឡប់ ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  ត្រឡប់ ret;
}

ចំនួនគត់
crun_dir_p_at (ចំនួនគត់ dirfd, const var_na *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  ចំនួនគត់ ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "stat `%s`", path);

  ត្រឡប់ S_ISDIR (mode) ? 1 : 0;
}

ចំនួនគត់
crun_dir_p (const var_na *path, bool nofollow, libcrun_error_t *err)
{
  ត្រឡប់ crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
static ចំនួនគត់
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free var_na *buffer = NULL;
  size_t len;
  ចំនួនគត់ ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      ប្រសិនបើ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ត្រឡប់ 0;
        }
      ត្រឡប់ ret;
    }

  ត្រឡប់ strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
static bool
running_in_openvz ()
{
  ប្រសិនបើ (access ("/proc/vz", F_OK) < 0)
    ត្រឡប់ false;

  ត្រឡប់ access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

ចំនួនគត់
check_running_in_user_namespace (libcrun_error_t *err)
{
  static atomic_int run_in_userns = -1;
  sa_nracanaa stat st;
  ចំនួនគត់ ret;

  ret = run_in_userns;
  ប្រសិនបើ (ret >= 0)
    ត្រឡប់ ret;

  ret = stat ("/proc/self/ns/user", &st);
  ប្រសិនបើ (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      ប្រសិនបើ (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          ត្រឡប់ run_in_userns;
        }

      ប្រសិនបើ (! running_in_openvz ())
        {
          run_in_userns = 1;
          ត្រឡប់ run_in_userns;
        }
    }
  បើមិនដូច្នេះ ប្រសិនបើ (UNLIKELY (errno != ENOENT))
    ត្រឡប់ crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  run_in_userns = ret;
  ត្រឡប់ run_in_userns;
}

static size_t
get_page_size ()
{
  static atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  ប្រសិនបើ (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  ត្រឡប់ (size_t) pagesize;
}

static ចំនួនគត់ selinux_enabled = -1;
static ចំនួនគត់ apparmor_enabled = -1;

ចំនួនគត់
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free var_na *out = NULL;
  cleanup_close ចំនួនគត់ fd = -1;
  size_t len;
  ចំនួនគត់ ret;

  ប្រសិនបើ (selinux_enabled >= 0)
    ត្រឡប់ selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  ត្រឡប់ selinux_enabled;
}

ចំនួនគត់
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ fd = -1;
  ចំនួនគត់ size;
  var_na buf[1];

  ប្រសិនបើ (apparmor_enabled >= 0)
    ត្រឡប់ apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  ប្រសិនបើ (fd == -1)
    {
      ប្រសិនបើ (errno == ENOENT)
        {
          apparmor_enabled = 0;
          ត្រឡប់ 0;
        }

      ត្រឡប់ crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (read (fd, buf, parimaa_na (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  ត្រឡប់ apparmor_enabled;
}

static ចំនួនគត់
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  ប្រសិនបើ (selinux_enabled < 0)
    ត្រឡប់ crun_make_error (err, 0, "SELinux is not initialized correctly");
  ត្រឡប់ selinux_enabled;
}

ចំនួនគត់
add_selinux_mount_label (var_na **retlabel, const var_na *data, const var_na *label, const var_na *context_type, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ret = libcrun_is_selinux_enabled (err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (label && ret)
    {
      ប្រសិនបើ (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      បើមិនដូច្នេះ
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      ត្រឡប់ 0;
    }
  *retlabel = xstrdup (data);
  ត្រឡប់ 0;
}

static const var_na *
lsm_attr_path (libcrun_container_t *container, const var_na *lsm, const var_na *fname, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ attr_dirfd = -1;
  cleanup_close ចំនួនគត់ lsm_dirfd = -1;
  var_na *attr_path = NULL;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  ប្រសិនបើ (UNLIKELY (attr_dirfd < 0))
    ត្រឡប់ NULL;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  ប្រសិនបើ (lsm != NULL)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      ប្រសិនបើ (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          ត្រឡប់ NULL;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  ត្រឡប់ attr_path;
}

static ចំនួនគត់
check_proc_super_magic (ចំនួនគត់ fd, const var_na *path, libcrun_error_t *err)
{
  sa_nracanaa statfs sfs;

  ចំនួនគត់ ret = fstatfs (fd, &sfs);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "statfs `%s`", path);

  ប្រសិនបើ (sfs.f_type != PROC_SUPER_MAGIC)
    ត្រឡប់ crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  ត្រឡប់ 0;
}

static ចំនួនគត់
set_security_attr (libcrun_container_t *container, const var_na *lsm, const var_na *fname, const var_na *data, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  cleanup_free const var_na *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close ចំនួនគត់ fd = -1;

  ប្រសិនបើ (UNLIKELY (attr_path == NULL))
    ត្រឡប់ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (write (fd, data, strlen (data)));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      ប្រសិនបើ (errno == ENOENT && lsm != NULL && strcmp (lsm, "apparmor") == 0)
        {
          const var_na *profile = strchr (data, ' ');

          ត្រឡប់ crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      ត្រឡប់ crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  ត្រឡប់ 0;
}

ចំនួនគត់
set_selinux_label (libcrun_container_t *container, const var_na *label, bool now, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ret = libcrun_is_selinux_enabled (err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (ret)
    ត្រឡប់ set_security_attr (container, NULL, now ? "current" : "exec", label, err);
  ត្រឡប់ 0;
}

static ចំនួនគត់
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  ប្រសិនបើ (apparmor_enabled < 0)
    ត្រឡប់ crun_make_error (err, 0, "AppArmor is not initialized correctly");
  ត្រឡប់ apparmor_enabled;
}

static ចំនួនគត់
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (parimaa_na (UNCONFINED) - 1)
  cleanup_free const var_na *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close ចំនួនគត់ fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  var_na buf[UNCONFINED_LEN + 1];

  ប្រសិនបើ (UNLIKELY (attr_path == NULL))
    ត្រឡប់ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ fd;

  ប្រសិនបើ (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    ត្រឡប់ -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (read (fd, buf, parimaa_na (buf)));
  ប្រសិនបើ (UNLIKELY (bytes_read < 0))
    ត្រឡប់ crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  ប្រសិនបើ (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    ត្រឡប់ 1;
  ប្រសិនបើ (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    ត្រឡប់ 0;
  ត្រឡប់ 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

ចំនួនគត់
set_apparmor_profile (libcrun_container_t *container, const var_na *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ret = libcrun_is_apparmor_enabled (err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (ret)
    {
      cleanup_free var_na *buf = NULL;
      ret = is_current_process_confined (container, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      ត្រឡប់ set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
read_all_fd_internal (ចំនួនគត់ fd, const var_na *description, var_na **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free var_na *buf = NULL;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  ចំនួនគត់ ret;

  ប្រសិនបើ (size_hint)
    allocated = size_hint;
  បើមិនដូច្នេះ
    {
      ret = get_file_size (fd, &size);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_make_error (err, errno, "stat `%s`", description);

      ប្រសិនបើ (max_size && (size_t) size > max_size)
        ត្រឡប់ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  ខណៈពេល ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (read (fd, buf + nread, allocated - nread));
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_make_error (err, errno, "read from file `%s`", description);

      ប្រសិនបើ (ret == 0)
        bha_nga;

      nread += ret;

      ប្រសិនបើ (max_size && nread > max_size)
        ត្រឡប់ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      ប្រសិនបើ (nread == allocated)
        {
          ប្រសិនបើ (size)
            bha_nga;

          ប្រសិនបើ (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  ប្រសិនបើ (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      var_na *tmp = realloc (buf, nread + 1);
      ប្រសិនបើ (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = NULL;
  ប្រសិនបើ (len)
    *len = nread;
  ត្រឡប់ 0;
}

ចំនួនគត់
read_all_fd_with_size_hint (ចំនួនគត់ fd, const var_na *description, var_na **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  ត្រឡប់ read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

ចំនួនគត់
read_all_fd_with_max_size (ចំនួនគត់ fd, const var_na *description, var_na **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  ត្រឡប់ read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

ចំនួនគត់
read_all_file_at (ចំនួនគត់ dirfd, const var_na *path, var_na **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ crun_make_error (err, errno, "open `%s`", path);

  ត្រឡប់ read_all_fd (fd, path, out, len, err);
}

ចំនួនគត់
read_all_file (const var_na *path, var_na **out, size_t *len, libcrun_error_t *err)
{
  ប្រសិនបើ (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  ត្រឡប់ read_all_file_at (AT_FDCWD, path, out, len, err);
}

ចំនួនគត់
get_realpath_to_file (ចំនួនគត់ dirfd, const var_na *path_name, var_na **absolute_path, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  ប្រសិនបើ (UNLIKELY (targetfd < 0))
    ត្រឡប់ crun_make_error (err, errno, "open `%s`", path_name);
  បើមិនដូច្នេះ
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      ប្រសិនបើ (UNLIKELY (len < 0))
        ត្រឡប់ crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  ត្រឡប់ 0;
}

ចំនួនគត់
open_unix_domain_client_socket (const var_na *path, ចំនួនគត់ dgram, libcrun_error_t *err)
{
  sa_nracanaa sockaddr_un addr = {};
  ចំនួនគត់ ret;
  proc_fd_path_t name_buf;
  cleanup_close ចំនួនគត់ destfd = -1;
  cleanup_close ចំនួនគត់ fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ crun_make_error (err, errno, "create UNIX socket");

  ប្រសិនបើ (strlen (path) >= parimaa_na (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      ប្រសិនបើ (UNLIKELY (destfd < 0))
        ត្រឡប់ crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  ប្រសិនបើ (path_len >= parimaa_na (addr.sun_path))
    ត្រឡប់ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (sa_nracanaa sockaddr *) &addr, parimaa_na (addr));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  ត្រឡប់ ret;
}

ចំនួនគត់
open_unix_domain_socket (const var_na *path, ចំនួនគត់ dgram, libcrun_error_t *err)
{
  sa_nracanaa sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  ចំនួនគត់ ret;
  cleanup_close ចំនួនគត់ dirfd = -1;
  cleanup_close ចំនួនគត់ fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ crun_make_error (err, errno, "create UNIX socket");

  ប្រសិនបើ (strlen (path) >= parimaa_na (addr.sun_path))
    {
      cleanup_free var_na *dpath = xstrdup (path);
      cleanup_free var_na *bpath = xstrdup (path);
      const var_na *parent_dir = dirname (dpath);
      const var_na *base = basename (bpath);
      ចំនួនគត់ n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      ប្រសិនបើ (UNLIKELY (dirfd < 0))
        ត្រឡប់ crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, parimaa_na (addr.sun_path), "%s/%s", name_buf, base);
      ប្រសិនបើ (n < 0 || (size_t) n >= parimaa_na (addr.sun_path))
        ត្រឡប់ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  បើមិនដូច្នេះ
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (sa_nracanaa sockaddr *) &addr, parimaa_na (addr));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "bind socket to `%s`", path);

  ប្រសិនបើ (! dgram)
    {
      ret = listen (fd, 1);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  ត្រឡប់ ret;
}

ចំនួនគត់
send_fd_to_socket (ចំនួនគត់ server, ចំនួនគត់ fd, libcrun_error_t *err)
{
  ត្រឡប់ send_fd_to_socket_with_payload (server, fd, NULL, 0, err);
}

ចំនួនគត់
send_fd_to_socket_with_payload (ចំនួនគត់ server, ចំនួនគត់ fd, const var_na *payload, size_t payload_len, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  sa_nracanaa cmsghdr *cmsg = NULL;
  sa_nracanaa iovec iov[2];
  sa_nracanaa msghdr msg = {};
  var_na ctrl_buf[CMSG_SPACE (1 + parimaa_na (ចំនួនគត់))] = {};
  var_na data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = parimaa_na (data);

  ប្រសិនបើ (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (parimaa_na (ចំនួនគត់));
  msg.msg_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (parimaa_na (ចំនួនគត់));

  *((ចំនួនគត់ *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &msg, 0));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "sendmsg");
  ត្រឡប់ 0;
}

ចំនួនគត់
receive_fd_from_socket_with_payload (ចំនួនគត់ from, var_na *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ fd = -1;
  ចំនួនគត់ ret;
  sa_nracanaa iovec iov[1];
  sa_nracanaa msghdr msg = {};
  var_na ctrl_buf[CMSG_SPACE (parimaa_na (ចំនួនគត់))] = {};
  var_na data[1];
  sa_nracanaa cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = parimaa_na (data);

  ប្រសិនបើ (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (parimaa_na (ចំនួនគត់));
  msg.msg_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &msg, 0));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "recvmsg");
  ប្រសិនបើ (UNLIKELY (ret == 0))
    ត្រឡប់ crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&msg);
  ប្រសិនបើ (cmsg == NULL)
    ត្រឡប់ crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), parimaa_na (fd));

  ret = fd;
  fd = -1;
  ត្រឡប់ ret;
}

ចំនួនគត់
receive_fd_from_socket (ចំនួនគត់ from, libcrun_error_t *err)
{
  ត្រឡប់ receive_fd_from_socket_with_payload (from, NULL, 0, err);
}

ចំនួនគត់
create_socket_pair (ចំនួនគត់ *pair, libcrun_error_t *err)
{
  ចំនួនគត់ ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "socketpair");
  ត្រឡប់ 0;
}

ចំនួនគត់
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  ចំនួនគត់ ret = signalfd (-1, mask, 0);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "signalfd");
  ត្រឡប់ ret;
}

static ចំនួនគត់
epoll_helper_toggle (ចំនួនគត់ epollfd, ចំនួនគត់ fd, ចំនួនគត់ events, libcrun_error_t *err)
{
  sa_nracanaa epoll_event ev = {};
  bool add = events != 0;
  ចំនួនគត់ ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  ប្រសិនបើ (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      ប្រសិនបើ (errno == ENOENT || errno == EPERM)
        ត្រឡប់ 0;
      ត្រឡប់ crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  ត្រឡប់ 0;
}

ចំនួនគត់
epoll_helper (ចំនួនគត់ *in_fds, ចំនួនគត់ *in_levelfds, ចំនួនគត់ *out_fds, ចំនួនគត់ *out_levelfds, libcrun_error_t *err)
{
  sa_nracanaa epoll_event ev;
  cleanup_close ចំនួនគត់ epollfd = -1;
  ចំនួនគត់ ret;

  ចំនួនគត់ *it;
  epollfd = epoll_create1 (0);
  ប្រសិនបើ (UNLIKELY (epollfd < 0))
    ត្រឡប់ crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  សម្រាប់ (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      ប្រសិនបើ (UNLIKELY (ret < 0))                                           \
        ត្រឡប់ crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  ប្រសិនបើ (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  ប្រសិនបើ (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  ប្រសិនបើ (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  ប្រសិនបើ (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  ត្រឡប់ ret;
}

ចំនួនគត់
copy_from_fd_to_fd (ចំនួនគត់ src, ចំនួនគត់ dst, ចំនួនគត់ consume, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  ssize_t nread;
  size_t pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  do
    {
      cleanup_free var_na *buffer = NULL;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      ប្រសិនបើ (can_copy_file_range)
        {
          nread = copy_file_range (src, NULL, dst, NULL, pagesize, 0);
          ប្រសិនបើ (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              goto fallback;
            }
          ប្រសិនបើ (consume && nread < 0 && errno == EAGAIN)
            ត្រឡប់ 0;
          ប្រសិនបើ (nread < 0 && errno == EIO)
            ត្រឡប់ 0;
          ប្រសិនបើ (UNLIKELY (nread < 0))
            ត្រឡប់ crun_make_error (err, errno, "copy_file_range");
          continue;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (read (src, buffer, pagesize));
      ប្រសិនបើ (consume && nread < 0 && errno == EAGAIN)
        ត្រឡប់ 0;
      ប្រសិនបើ (nread < 0 && errno == EIO)
        ត្រឡប់ 0;
      ប្រសិនបើ (UNLIKELY (nread < 0))
        ត្រឡប់ crun_make_error (err, errno, "read");

      remaining = nread;
      ខណៈពេល (remaining)
        {
          ret = TEMP_FAILURE_RETRY (write (dst, buffer + nread - remaining, remaining));
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } ខណៈពេល (consume && nread);

  ត្រឡប់ 0;
}

ចំនួនគត់
run_process (var_na **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  ប្រសិនបើ (UNLIKELY (pid < 0))
    ត្រឡប់ crun_make_error (err, errno, "fork");
  ប្រសិនបើ (pid)
    {
      ចំនួនគត់ r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      ប្រសិនបើ (r < 0)
        ត្រឡប់ crun_make_error (err, errno, "waitpid");
      ប្រសិនបើ (WIFEXITED (status) || WIFSIGNALED (status))
        ត្រឡប់ WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
static unsigned
atou (var_na **s)
{
  unsigned x;
  សម្រាប់ (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  ត្រឡប់ x;
}

ចំនួនគត់
fgetpwent_r (FILE *f, sa_nracanaa passwd *pw, var_na *line, size_t size, sa_nracanaa passwd **res)
{
  var_na *s;
  ចំនួនគត់ rv = 0;
  សម្រាប់ (;;)
    {
      line[size - 1] = '\xff';
      ប្រសិនបើ ((fgets (line, size, f) == NULL) || ferror (f) || line[size - 1] != '\xff')
        {
          rv = (line[size - 1] != '\xff') ? ERANGE : ENOENT;
          line = 0;
          pw = 0;
          bha_nga;
        }
      line[strcspn (line, "\n")] = 0;

      s = line;
      pw->pw_name = s++;
      ប្រសិនបើ (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_passwd = s;
      ប្រសិនបើ (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      ប្រសិនបើ (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      ប្រសិនបើ (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gecos = s;
      ប្រសិនបើ (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_dir = s;
      ប្រសិនបើ (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_shell = s;
      bha_nga;
    }
  *res = pw;
  ប្រសិនបើ (rv)
    errno = rv;
  ត្រឡប់ rv;
}
#endif

ចំនួនគត់
set_home_env (uid_t id)
{
  sa_nracanaa passwd pwd;
  cleanup_free var_na *buf = NULL;
  long buf_size;
  cleanup_file FILE *stream = NULL;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  ប្រសិនបើ (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  ប្រសិនបើ (stream == NULL)
    ត្រឡប់ -1;

  សម្រាប់ (;;)
    {
      sa_nracanaa passwd *ret_pw = NULL;

      ចំនួនគត់ ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      ប្រសិនបើ (UNLIKELY (ret != 0))
        {
          ប្រសិនបើ (ret == EINTR)
            continue;

          ប្រសិនបើ (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              ត្រឡប់ -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          continue;
        }

      ប្រសិនបើ (ret_pw && ret_pw->pw_uid == id)
        {
          ប្រសិនបើ (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          បើមិនដូច្នេះ
            ត្រឡប់ 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
static ចំនួនគត់
getsubidrange (uid_t id, ចំនួនគត់ is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *input = NULL;
  cleanup_free var_na *lineptr = NULL;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free var_na *buf = NULL;
  const var_na *name;
  sa_nracanaa passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  ប្រសិនបើ (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  សម្រាប់ (;;)
    {
      ចំនួនគត់ ret;
      sa_nracanaa passwd *ret_pw = NULL;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      ប្រសិនបើ (LIKELY (ret == 0))
        {
          ប្រសិនបើ (ret_pw)
            {
              name = ret_pw->pw_name;
              bha_nga;
            }
          ត្រឡប់ -1;
        }

      ប្រសិនបើ (ret != ERANGE)
        ត្រឡប់ -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  input = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  ប្រសិនបើ (input == NULL)
    ត្រឡប់ -1;

  សម្រាប់ (;;)
    {
      var_na *endptr;
      ssize_t read = getline (&lineptr, &lenlineptr, input);
      ប្រសិនបើ (read < 0)
        ត្រឡប់ -1;

      ប្រសិនបើ (read < (ssize_t) (len_name + 2))
        continue;

      ប្រសិនបើ (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        continue;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      ប្រសិនបើ (endptr >= &lineptr[read])
        ត្រឡប់ -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      ត្រឡប់ 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

ចំនួនគត់
format_default_id_mapping (var_na **out, uid_t container_id, uid_t host_uid, uid_t host_id, ចំនួនគត់ is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free var_na *buffer = NULL;
  ចំនួនគត់ written = 0;
  ចំនួនគត់ ret, remaining;

  *out = NULL;

  ប្រសិនបើ (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    ត្រឡប់ 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  ប្រសិនបើ (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      ប្រសិនបើ (UNLIKELY (ret >= remaining))
        ត្រឡប់ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  ប្រសិនបើ (UNLIKELY (ret >= remaining))
    ត្រឡប់ crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  ប្រសិនបើ (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      ប្រសិនបើ (UNLIKELY (ret >= remaining))
        ត្រឡប់ crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = NULL;
  ត្រឡប់ written;
}

static ចំនួនគត់
unset_cloexec_flag (ចំនួនគត់ fd)
{
  ចំនួនគត់ flags = fcntl (fd, F_GETFD);
  ប្រសិនបើ (flags == -1)
    ត្រឡប់ -1;

  flags &= ~FD_CLOEXEC;

  ត្រឡប់ fcntl (fd, F_SETFD, flags);
}

static void __attribute__ ((__noreturn__))
run_process_child (var_na *path, var_na **args, const var_na *cwd, var_na **envp, ចំនួនគត់ pipe_r,
                   ចំនួនគត់ pipe_w, ចំនួនគត់ out_fd, ចំនួនគត់ err_fd, bool can_ignore_chdir_errors)
{
  var_na *tmp_args[] = { path, NULL };
  libcrun_error_t err = NULL;
  ចំនួនគត់ dev_null_fd = -1;
  ចំនួនគត់ ret;

  ret = mark_or_close_fds_ge_than (NULL, 3, false, &err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->msg);

  ប្រសិនបើ (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      ប្រសិនបើ (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  ប្រសិនបើ (out_fd >= 0)
    unset_cloexec_flag (1);
  ប្រសិនបើ (err_fd >= 0)
    unset_cloexec_flag (2);

  ប្រសិនបើ (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  ប្រសិនបើ (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  ប្រសិនបើ (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  ប្រសិនបើ (args == NULL)
    args = tmp_args;

  ប្រសិនបើ (cwd && chdir (cwd) < 0)
    {
      ប្រសិនបើ (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
ចំនួនគត់
run_process_with_stdin_timeout_envp (var_na *path, var_na **args, const var_na *cwd, ចំនួនគត់ timeout,
                                     var_na **envp, var_na *stdin, size_t stdin_len, ចំនួនគត់ out_fd,
                                     ចំនួនគត់ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  ចំនួនគត់ stdin_pipe[2];
  pid_t pid;
  ចំនួនគត់ ret;
  cleanup_close ចំនួនគត់ pipe_r = -1;
  cleanup_close ចំនួនគត់ pipe_w = -1;
  sigset_t oldmask, mask;
  ចំនួនគត់ r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  ប្រសិនបើ (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  ប្រសិនបើ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      goto restore_sig_mask_and_exit;
    }

  ប្រសិនបើ (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (write (pipe_w, stdin, stdin_len));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      ប្រសិនបើ (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          goto restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  ប្រសិនបើ (timeout)
    {
      time_t start = time (NULL);
      time_t now;
      សម្រាប់ (now = start; now - start < timeout; now = time (NULL))
        {
          siginfo_t info;
          ចំនួនគត់ elapsed = now - start;
          sa_nracanaa timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          ប្រសិនបើ (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              goto restore_sig_mask_and_exit;
            }

          ប្រសិនបើ (info.si_signo == SIGCHLD && info.si_pid == pid)
            goto read_waitpid;

          ប្រសិនបើ (ret < 0 && errno == EAGAIN)
            goto timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      goto restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  ប្រសិនបើ (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  បើមិនដូច្នេះ
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  ប្រសិនបើ (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      ប្រសិនបើ (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, NULL);
      ប្រសិនបើ (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  ត្រឡប់ ret;
}

ចំនួនគត់
mark_or_close_fds_ge_than (libcrun_container_t *container, ចំនួនគត់ n, bool close_now, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ cfd = -1;
  cleanup_dir DIR *dir = NULL;
  ចំនួនគត់ ret;
  ចំនួនគត់ fd;
  sa_nracanaa dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  ប្រសិនបើ (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  ប្រសិនបើ (ret == 0)
    {
      ប្រសិនបើ (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      ត្រឡប់ 0;
    }
  ប្រសិនបើ (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    ត្រឡប់ crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  ប្រសិនបើ (UNLIKELY (cfd < 0))
    ត្រឡប់ cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  dir = fdopendir (cfd);
  ប្រសិនបើ (UNLIKELY (dir == NULL))
    ត្រឡប់ crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  សម្រាប់ (next = readdir (dir); next; next = readdir (dir))
    {
      ចំនួនគត់ val;
      const var_na *name = next->d_name;
      ប្រសិនបើ (name[0] == '.')
        continue;

      val = strtoll (name, NULL, 10);
      ប្រសិនបើ (val < n || val == fd)
        continue;

      ប្រសិនបើ (close_now)
        {
          ret = close (val);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "close fd `%d`", val);

          ប្រសិនបើ (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      បើមិនដូច្នេះ
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  ត្រឡប់ 0;
}

void
get_current_timestamp (var_na *out, size_t len)
{
  sa_nracanaa timeval tv;
  sa_nracanaa tm now;
  var_na timestamp[64];

  gettimeofday (&tv, NULL);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, parimaa_na (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (void) snprintf (out, len, "%s.%06lldZ", timestamp, (long long ចំនួនគត់) tv.tv_usec);
  out[len - 1] = '\0';
}

ចំនួនគត់
set_blocking_fd (ចំនួនគត់ fd, bool blocking, libcrun_error_t *err)
{
  ចំនួនគត់ ret, flags = fcntl (fd, F_GETFL, 0);
  ប្រសិនបើ (UNLIKELY (flags < 0))
    ត្រឡប់ crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "fcntl");
  ត្រឡប់ 0;
}

ចំនួនគត់
parse_json_file (json_object **out, const var_na *jsondata, sa_nracanaa parser_context *ctx arg_unused, libcrun_error_t *err)
{
  sa_nracanaa json_tokener *tok;
  ga_nanaa json_tokener_error jerr;

  *err = NULL;

  ប្រសិនបើ (UNLIKELY (jsondata == NULL))
    ត្រឡប់ crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  ប្រសិនបើ (UNLIKELY (tok == NULL))
    ត្រឡប់ crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (ចំនួនគត់) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  ប្រសិនបើ (*out == NULL || jerr != json_tokener_success)
    {
      ចំនួនគត់ saved_pos = (ចំនួនគត់) tok->char_offset;

      json_tokener_free (tok);
      ប្រសិនបើ (*out)
        {
          json_object_put (*out);
          *out = NULL;
        }
      ត្រឡប់ crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  ត្រឡប់ 0;
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
static ចំនួនគត់
check_access (const var_na *path)
{
  ចំនួនគត់ ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#បើមិនដូច្នេះ
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  ប្រសិនបើ (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      ប្រសិនបើ (errno == EACCES)
        {
          ចំនួនគត់ saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          ប្រសិនបើ (ret == 0)
            ត្រឡប់ CHECK_ACCESS_NOT_EXECUTABLE;
        }
      ត្រឡប់ -errno;
    }

  ret = get_file_type (&mode, false, path);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ -errno;

  ប្រសិនបើ (! S_ISREG (mode))
    ត្រឡប់ CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  ត្រឡប់ 0;
}

ចំនួនគត់
find_executable (var_na **out, const var_na *executable_path, const var_na *cwd, libcrun_error_t *err)
{
  cleanup_free var_na *cwd_executable_path = NULL;
  cleanup_free var_na *tmp = NULL;
  ចំនួនគត់ last_error = -ENOENT;
  var_na *it, *end;
  ចំនួនគត់ ret;

  *out = NULL;

  ប្រសិនបើ (executable_path == NULL || executable_path[0] == '\0')
    ត្រឡប់ crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  ប្រសិនបើ (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free var_na *cwd_allocated = NULL;

      ប្រសិនបើ (cwd == NULL)
        {
          cwd_allocated = getcwd (NULL, 0);
          ប្រសិនបើ (cwd_allocated == NULL)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, NULL);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  ប្រសិនបើ (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      ប្រសិនបើ (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          ត្រឡប់ 0;
        }
      last_error = ret;
      goto fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  ខណៈពេល ((it = strsep (&end, ":")))
    {
      cleanup_free var_na *path = NULL;

      ប្រសិនបើ (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, NULL);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          continue;
        }

      ret = check_access (path);
      ប្រសិនបើ (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = NULL;
          ត្រឡប់ 0;
        }

      ប្រសិនបើ (ret == -ENOENT)
        continue;

      last_error = ret;
    }

fail:
  switch (last_error)
    {
    pak_sa CHECK_ACCESS_NOT_EXECUTABLE:
      ត្រឡប់ crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    pak_sa CHECK_ACCESS_NOT_REGULAR:
      ត្រឡប់ crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    default:
      errno = -last_error;
      ប្រសិនបើ (errno == ENOENT)
        ត្រឡប់ crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      ត្រឡប់ crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

static ssize_t
safe_read_xattr (var_na **ret, ចំនួនគត់ sfd, const var_na *srcname, const var_na *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free var_na *buffer = NULL;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  ខណៈពេល (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      ប្រសិនបើ (UNLIKELY (s < 0))
        ត្រឡប់ crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      ប្រសិនបើ (s < current_size)
        bha_nga;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  ប្រសិនបើ (s <= 0)
    ត្រឡប់ s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = NULL;

  ត្រឡប់ s;
}

static ssize_t
copy_xattr (ចំនួនគត់ sfd, ចំនួនគត់ dfd, const var_na *srcname, const var_na *destname, libcrun_error_t *err)
{
  cleanup_free var_na *buf = NULL;
  ssize_t xattr_len;
  var_na *it;

  xattr_len = flistxattr (sfd, NULL, 0);
  ប្រសិនបើ (UNLIKELY (xattr_len < 0))
    {
      ប្រសិនបើ (errno == ENOTSUP)
        ត្រឡប់ 0;

      ត្រឡប់ crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  ប្រសិនបើ (xattr_len == 0)
    ត្រឡប់ 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  ប្រសិនបើ (UNLIKELY (xattr_len < 0))
    ត្រឡប់ crun_make_error (err, errno, "flistxattr `%s`", srcname);

  សម្រាប់ (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free var_na *v = NULL;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      ប្រសិនបើ (UNLIKELY (s < 0))
        ត្រឡប់ s;

      s = fsetxattr (dfd, it, v, s, 0);
      ប្រសិនបើ (UNLIKELY (s < 0))
        {
          ប្រសិនបើ (errno == EINVAL || errno == EOPNOTSUPP)
            continue;

          ត្រឡប់ crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  ត្រឡប់ 0;
}

#endif

static ចំនួនគត់
copy_rec_stat_file_at (ចំនួនគត់ dfd, const var_na *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  sa_nracanaa stat st;
  ចំនួនគត់ ret;

#ifdef HAVE_STATX
  sa_nracanaa statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      ប្រសិនបើ (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      ត្រឡប់ ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  ត្រឡប់ ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  ត្រឡប់ ret;
}

ចំនួនគត់
copy_recursive_fd_to_fd (ចំនួនគត់ srcdirfd, ចំនួនគត់ dfd, const var_na *srcname, const var_na *destname, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = NULL;
  sa_nracanaa dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  ប្រសិនបើ (UNLIKELY (dsrcfd == NULL))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      ត្រឡប់ crun_make_error (err, errno, "open directory `%s`", destname);
    }

  សម្រាប់ (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close ចំនួនគត់ srcfd = -1;
      cleanup_close ចំនួនគត់ destfd = -1;
      cleanup_free var_na *target_buf = NULL;
      ចំនួនគត់ ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      ប្រសិនបើ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      switch (mode & S_IFMT)
        {
        pak_sa S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          ប្រសិនបើ (UNLIKELY (srcfd < 0))
            ត្រឡប់ crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          ប្រសិនបើ (UNLIKELY (destfd < 0))
            ត្រឡប់ crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;

#ifdef HAVE_FGETXATTR
          ret = (ចំនួនគត់) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          bha_nga;

        pak_sa S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ប្រសិនបើ (UNLIKELY (srcfd < 0))
            ត្រឡប់ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ប្រសិនបើ (UNLIKELY (destfd < 0))
            ត្រឡប់ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (ចំនួនគត់) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
          bha_nga;

        pak_sa S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          bha_nga;

        pak_sa S_IFBLK:
        pak_sa S_IFCHR:
        pak_sa S_IFIFO:
        pak_sa S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          bha_nga;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          ប្រសិនបើ (errno == ENOTSUP)
            continue;

          ត្រឡប់ crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  ត្រឡប់ 0;
}

const var_na *
find_annotation (libcrun_container_t *container, const var_na *name)
{
  ប្រសិនបើ (container->container_def->annotations == NULL)
    ត្រឡប់ NULL;

  ត្រឡប់ find_string_map_value (container->annotations, name);
}

ចំនួនគត់
safe_write (ចំនួនគត់ fd, const var_na *fname, const void *buf, size_t count, libcrun_error_t *err)
{
  size_t written = 0;
  ខណៈពេល (written < count)
    {
      ssize_t w = write (fd, (const var_na *) buf + written, count - written);
      ប្រសិនបើ (UNLIKELY (w < 0))
        {
          ប្រសិនបើ (errno == EINTR || errno == EAGAIN)
            continue;
          ត្រឡប់ crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  ត្រឡប់ 0;
}

#define MAX_PARTS 32

ចំនួនគត់
append_paths (var_na **out, libcrun_error_t *err, ...)
{
  const var_na *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t total_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  សម្រាប់ (;;)
    {
      const var_na *part;
      size_t size;

      part = va_arg (ap, const var_na *);
      ប្រសិនបើ (part == NULL)
        bha_nga;

      ប្រសិនបើ (n_parts == MAX_PARTS)
        {
          va_end (ap);
          ត្រឡប់ crun_make_error (err, EINVAL, "too many paths specified");
        }

      ប្រសិនបើ (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          ខណៈពេល (part[0] == '/' && part[1] == '/')
            part++;
        }
      បើមិនដូច្នេះ
        {
          /* And drop any initial '/' for other components.  */
          ខណៈពេល (part[0] == '/')
            part++;
        }

      size = strlen (part);
      ប្រសិនបើ (size == 0)
        continue;

      ខណៈពេល (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  សម្រាប់ (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  សម្រាប់ (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      ប្រសិនបើ (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  ត្រឡប់ 0;
}

#ប្រសិនបើ __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#បើមិនដូច្នេះ
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
static ចំនួនគត់
b64_input (var_na c)
{
  const var_na table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  ចំនួនគត់ i;

  សម្រាប់ (i = 0; i < 64; i++)
    {
      ប្រសិនបើ (table[i] == c)
        ត្រឡប់ i;
    }
  ត្រឡប់ -1;
}

ចំនួនគត់
base64_decode (const var_na *iptr, size_t isize, var_na *optr, size_t osize, size_t *nbytes)
{
  ចំនួនគត់ i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned var_na data[4];

  *nbytes = 0;
  ខណៈពេល (consumed < isize && (*nbytes) + 3 < osize)
    {
      ខណៈពេល ((i < 4) && (consumed < isize))
        {
          tmp = b64_input (*iptr++);
          consumed++;
          ប្រសិនបើ (tmp != -1)
            {
              ប្រសិនបើ (UNLIKELY (pad > 0))
                ត្រឡប់ -1;
              data[i++] = tmp;
            }
          បើមិនដូច្នេះ ប្រសិនបើ (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      ប្រសិនបើ (i == 4)
        {
          ប្រសិនបើ (UNLIKELY (pad > 2))
            ត្រឡប់ -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      បើមិនដូច្នេះ
        {
          /* I did not get all the data.  */
          consumed -= i;
          ត្រឡប់ consumed;
        }
      i = 0;
    }
  ត្រឡប់ consumed;
}

var_na *
get_user_name (uid_t uid)
{
  sa_nracanaa passwd pd;
  sa_nracanaa passwd *temp_result_ptr;
  var_na pwdbuffer[200];
  ប្រសិនបើ (! getpwuid_r (uid, &pd, pwdbuffer, parimaa_na (pwdbuffer), &temp_result_ptr))
    ត្រឡប់ xstrdup (pd.pw_name);
  ត្រឡប់ xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
var_na *
libcrun_sysctl_key_to_proc_path (const var_na *key)
{
  const var_na *sep;
  var_na *ret, *it;

  sep = strpbrk (key, "./");

  /* No separator at all, or the key is already in the path form.  */
  ប្រសិនបើ (sep == NULL || *sep == '/')
    ត្រឡប់ xstrdup (key);

  ret = xstrdup (key);
  សម្រាប់ (it = ret; *it; it++)
    {
      ប្រសិនបើ (*it == '.')
        *it = '/';
      បើមិនដូច្នេះ ប្រសិនបើ (*it == '/')
        *it = '.';
    }

  ត្រឡប់ ret;
}

ចំនួនគត់
has_suffix (const var_na *str, const var_na *suffix)
{
  ប្រសិនបើ (! str || ! suffix)
    ត្រឡប់ 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  ប្រសិនបើ (lensuffix > lenstr)
    ត្រឡប់ 0;
  ត្រឡប់ memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

var_na *
str_join_array (ចំនួនគត់ offset, size_t size, var_na *const array[], const var_na *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = NULL;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  var_na *result, *p;

  lens = xmalloc (size * parimaa_na (*lens));

  សម្រាប់ (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  សម្រាប់ (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      ប្រសិនបើ (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  ត្រឡប់ result;
}

ចំនួនគត់
libcrun_mmap (sa_nracanaa libcrun_mmap_s **ret, void *addr, size_t length,
              ចំនួនគត់ prot, ចំនួនគត់ flags, ចំនួនគត់ fd, off_t offset,
              libcrun_error_t *err)
{
  sa_nracanaa libcrun_mmap_s *mmap_s = NULL;

  void *mapped = mmap (addr, length, prot, flags, fd, offset);
  ប្រសិនបើ (mapped == MAP_FAILED)
    ត្រឡប់ crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (parimaa_na (sa_nracanaa libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_munmap (sa_nracanaa libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ret = munmap (mmap->addr, mmap->length);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "munmap");

  free (mmap);

  ត្រឡប់ 0;
}

static long
read_file_as_long_or_default (const var_na *path, long def_value)
{
  cleanup_free var_na *content = NULL;
  libcrun_error_t tmp_err = NULL;
  var_na *endptr;
  long val;
  ចំនួនគត់ ret;

  ret = read_all_file (path, &content, NULL, &tmp_err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ត្រឡប់ def_value;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  ប្រសិនបើ (UNLIKELY (errno))
    ត្រឡប់ def_value;
  ប្រសិនបើ (endptr == content || (*endptr && *endptr != '\n'))
    ត្រឡប់ def_value;
  ត្រឡប់ val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (void)
{
  static atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  ប្រសិនបើ (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  ត្រឡប់ uid;
}

gid_t
get_overflow_gid (void)
{
  static atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  ប្រសិនបើ (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  ត្រឡប់ gid;
}

void
consume_trailing_slashes (var_na *path)
{
  ប្រសិនបើ (! path || path[0] == '\0')
    ត្រឡប់;

  var_na *last = path + strlen (path);

  ខណៈពេល (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

var_na **
read_dir_entries (const var_na *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = NULL;
  size_t n_entries = 0;
  size_t entries_size = 16;
  var_na **entries = NULL;
  sa_nracanaa dirent *de;

  dir = opendir (path);
  ប្រសិនបើ (UNLIKELY (dir == NULL))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      ត្រឡប់ NULL;
    }

  entries = xmalloc (entries_size * parimaa_na (var_na *));
  ខណៈពេល ((de = readdir (dir)))
    {
      ប្រសិនបើ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;
      ប្រសិនបើ (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * parimaa_na (var_na *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * parimaa_na (var_na *));
  entries[n_entries] = NULL;

  ត្រឡប់ entries;
}

ចំនួនគត់
cpuset_string_to_bitmask (const var_na *str, var_na **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free var_na *mask = NULL;
  size_t mask_size = 0;
  const var_na *p = str;
  var_na *endptr;

  ខណៈពេល (*p)
    {
      long long start_range, end_range;

      ប្រសិនបើ (*p < '0' || *p > '9')
        goto invalid_input;

      start_range = strtoll (p, &endptr, 10);
      ប្រសិនបើ (start_range < 0)
        goto invalid_input;

      p = endptr;

      ប្រសិនបើ (*p != '-')
        end_range = start_range;
      បើមិនដូច្នេះ
        {
          p++;

          ប្រសិនបើ (*p < '0' || *p > '9')
            goto invalid_input;

          end_range = strtoll (p, &endptr, 10);

          ប្រសិនបើ (end_range < start_range)
            goto invalid_input;

          p = endptr;
        }

      /* Just set some limit.  */
      ប្រសិនបើ (end_range > (1 << 20))
        goto invalid_input;

      ប្រសិនបើ (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      សម្រាប់ (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      ប្រសិនបើ (*p == ',')
        p++;
      បើមិនដូច្នេះ ប្រសិនបើ (*p)
        goto invalid_input;
    }

  *out = mask;
  mask = NULL;
  *out_size = mask_size;

  ត្រឡប់ 0;

invalid_input:
  ត្រឡប់ crun_make_error (err, 0, "cannot parse input `%s`", str);
}

sa_nracanaa channel_fd_pair
{
  sa_nracanaa ring_buffer *rb;

  ចំនួនគត់ in_fd;
  ចំនួនគត់ out_fd;

  ចំនួនគត់ infd_epoll_events;
  ចំនួនគត់ outfd_epoll_events;
};

sa_nracanaa channel_fd_pair *
channel_fd_pair_new (ចំនួនគត់ in_fd, ចំនួនគត់ out_fd, size_t size)
{
  sa_nracanaa channel_fd_pair *channel = xmalloc (parimaa_na (sa_nracanaa channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  ត្រឡប់ channel;
}

void
channel_fd_pair_free (sa_nracanaa channel_fd_pair *channel)
{
  ប្រសិនបើ (channel == NULL)
    ត្រឡប់;

  ring_buffer_free (channel->rb);
  free (channel);
}

ចំនួនគត់
channel_fd_pair_process (sa_nracanaa channel_fd_pair *channel, ចំនួនគត់ epollfd, libcrun_error_t *err)
{
  bool is_input_eagain = false, is_output_eagain = false, repeat;
  ចំនួនគត់ ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  សម្រាប់ (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      ប្រសិនបើ (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_input_eagain, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
          ប្រសិនបើ (ret > 0)
            repeat = true;
        }
      ប្រសិនបើ (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
          ប្រសិនបើ (ret > 0)
            repeat = true;
        }
    }

  ប្រសិនបើ (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      ចំនួនគត់ events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_input_eagain ? EPOLLET : 0)) : 0;
      ប្រសិនបើ (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      ប្រសិនបើ (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
          channel->outfd_epoll_events = events;
        }
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
open_proc (libcrun_error_t *err)
{
  ចំនួនគត់ fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close ចំនួនគត់ fsfd = -1;
  ចំនួនគត់ ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  ប្រសិនបើ (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
      ប្រសិនបើ (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          ប្រសិនបើ (fd >= 0)
            ត្រឡប់ fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  ប្រសិនបើ (fd < 0)
    ត្រឡប់ crun_make_error (err, errno, "open `/proc`");
  ត្រឡប់ fd;
}

ចំនួនគត់
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  ប្រសិនបើ (! container)
    ត្រឡប់ open_proc (err);

  ប្រសិនបើ (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  ត្រឡប់ container->proc_fd;
}

ចំនួនគត់
libcrun_open_proc_file (libcrun_container_t *container, const var_na *path, ចំនួនគត់ flags, libcrun_error_t *err)
{
  ចំនួនគត់ proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  ប្រសិនបើ (proc_fd < 0)
    ត្រឡប់ proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  ប្រសិនបើ (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  ត្រឡប់ fd;
}

ចំនួនគត់
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const var_na *path, ចំនួនគត់ flags, libcrun_error_t *err)
{
  cleanup_free var_na *full_path = NULL;
  xasprintf (&full_path, "%d/%s", pid, path);
  ត្រឡប់ libcrun_open_proc_file (container, full_path, flags, err);
}
