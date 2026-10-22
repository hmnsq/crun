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
#ᬬᬾᬦ᭄ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#ᬮᬶᬬ
#  define atomic_long ca_ncala long
#  define atomic_int ca_ncala puur_naa_nka
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

puur_naa_nka
crun_path_exists (const var_na *path, libcrun_error_t *err)
{
  puur_naa_nka ret = access (path, F_OK);
  ᬬᬾᬦ᭄ (ret < 0)
    {
      ᬬᬾᬦ᭄ (errno == ENOENT)
        ᬯᬮᬶ 0;
      ᬯᬮᬶ crun_make_error (err, errno, "access `%s`", path);
    }
  ᬯᬮᬶ 1;
}

puur_naa_nka
xasprintf (var_na **str, const var_na *fmt, ...)
{
  puur_naa_nka ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  ᬯᬮᬶ ret;
}

puur_naa_nka
write_file_at_with_flags (puur_naa_nka dirfd, puur_naa_nka flags, mode_t mode, const var_na *name, const void *data, size_t len, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka fd = -1;
  puur_naa_nka ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ᬯᬮᬶ (len > INT_MAX) ? INT_MAX : (puur_naa_nka) len;
}

puur_naa_nka
detach_process ()
{
  pid_t pid;
  ᬬᬾᬦ᭄ (setsid () < 0)
    ᬯᬮᬶ -1;
  pid = fork ();
  ᬬᬾᬦ᭄ (pid < 0)
    ᬯᬮᬶ -1;
  ᬬᬾᬦ᭄ (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  ᬯᬮᬶ 0;
}

puur_naa_nka
get_file_type_fd (puur_naa_nka fd, mode_t *mode)
{
  sa_nracanaa stat st;
  puur_naa_nka ret;

#ifdef HAVE_STATX
  sa_nracanaa statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    {
      ᬬᬾᬦ᭄ (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      ᬯᬮᬶ ret;
    }
  *mode = stx.stx_mode;
  ᬯᬮᬶ ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  ᬯᬮᬶ ret;
}

puur_naa_nka
get_file_type_at (puur_naa_nka dirfd, mode_t *mode, bool nofollow, const var_na *path)
{
  puur_naa_nka empty_path = path == NULL ? AT_EMPTY_PATH : 0;
  sa_nracanaa stat st;
  puur_naa_nka ret;

#ifdef HAVE_STATX
  sa_nracanaa statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    {
      ᬬᬾᬦ᭄ (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      ᬯᬮᬶ ret;
    }
  *mode = stx.stx_mode;
  ᬯᬮᬶ ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  ᬯᬮᬶ ret;
}

puur_naa_nka
get_file_type (mode_t *mode, bool nofollow, const var_na *path)
{
  ᬯᬮᬶ get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

puur_naa_nka
create_file_if_missing_at (puur_naa_nka dirfd, const var_na *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  ᬬᬾᬦ᭄ (fd_write < 0)
    {
      puur_naa_nka saved_errno = errno;
      mode_t tmp_mode;
      puur_naa_nka ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      ᬬᬾᬦ᭄ (ret == 0 && S_ISREG (tmp_mode))
        ᬯᬮᬶ 0;

      ᬯᬮᬶ crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  ᬯᬮᬶ 0;
}

static puur_naa_nka
ensure_directory_internal_at (puur_naa_nka dirfd, var_na *path, size_t len, puur_naa_nka mode, libcrun_error_t *err)
{
  var_na *it = path + len;
  puur_naa_nka ret = 0;
  bool parent_created = false;

  for (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      ᬬᬾᬦ᭄ (ret == 0 || errno == EEXIST)
        ᬯᬮᬶ 0;

      puur_naa_nka saved_errno = errno;
      ᬬᬾᬦ᭄ (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = NULL;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          ᬬᬾᬦ᭄ (ret > 0)
            ᬯᬮᬶ 0;
          ᬬᬾᬦ᭄ (ret < 0)
            crun_error_release (&tmp_err);

          ᬯᬮᬶ crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      ᬲᭂᬤᭂᬮ (it > path && *it != '/')
        {
          it--;
          len--;
        }
      ᬬᬾᬦ᭄ (it == path)
        ᬯᬮᬶ 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;

      parent_created = true;
    }
  ᬯᬮᬶ ret;
}

puur_naa_nka
crun_ensure_directory_at (puur_naa_nka dirfd, const var_na *path, puur_naa_nka mode, bool nofollow, libcrun_error_t *err)
{
  puur_naa_nka ret;
  cleanup_free var_na *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ᬬᬾᬦ᭄ (ret == 0)
    ᬯᬮᬶ crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  ᬯᬮᬶ 0;
}

static puur_naa_nka
check_fd_is_path (const var_na *path, puur_naa_nka fd, const var_na *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  var_na link[PATH_MAX];
  puur_naa_nka ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, parimaa_na (link)));
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "readlink `%s`", fdname);

  ᬬᬾᬦ᭄ (((size_t) ret) != path_len || memcmp (link, path, path_len))
    ᬯᬮᬶ crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  ᬯᬮᬶ 0;
}

static puur_naa_nka
check_fd_under_path (const var_na *rootfs, size_t rootfslen, puur_naa_nka fd, const var_na *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  var_na link[PATH_MAX];
  puur_naa_nka ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  ᬬᬾᬦ᭄ (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    ᬯᬮᬶ 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, parimaa_na (link)));
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "readlink `%s`", fdname);

  ᬬᬾᬦ᭄ (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    ᬯᬮᬶ crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  ᬯᬮᬶ 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
static void
close_and_replace (puur_naa_nka *oldfd, puur_naa_nka newfd)
{
  ᬬᬾᬦ᭄ (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
var_na *chroot_realpath (const var_na *chroot, const var_na *path, var_na resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
static puur_naa_nka
safe_openat_fallback (puur_naa_nka dirfd, const var_na *rootfs, const var_na *path, puur_naa_nka flags,
                      puur_naa_nka mode, libcrun_error_t *err)
{
  cleanup_free var_na *parent_path = NULL;
  const var_na *last_component = NULL;
  const var_na *orig_path = path;
  const var_na *path_in_chroot;
  cleanup_close puur_naa_nka fd = -1;
  var_na resolved[PATH_MAX];
  var_na buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  puur_naa_nka ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  ᬬᬾᬦ᭄ (flags & O_NOFOLLOW)
    {
      var_na *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      ᬬᬾᬦ᭄ (sep == NULL)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      ᬮᬶᬬ ᬬᬾᬦ᭄ (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      ᬬᬾᬦ᭄ (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  ᬬᬾᬦ᭄ (path_in_chroot == NULL)
    ᬯᬮᬶ crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  ᬬᬾᬦ᭄ (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  ᬬᬾᬦ᭄ (last_component)
    {
      ret = snprintf (resolved, parimaa_na (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      ᬬᬾᬦ᭄ (UNLIKELY (ret >= (puur_naa_nka) parimaa_na (resolved)))
        ᬯᬮᬶ crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  ᬬᬾᬦ᭄ (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      ᬯᬮᬶ ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ret = fd;
  fd = -1;
  ᬯᬮᬶ ret;
}

puur_naa_nka
safe_openat (puur_naa_nka dirfd, const var_na *rootfs, const var_na *path, puur_naa_nka flags, puur_naa_nka mode,
             libcrun_error_t *err)
{
  static bool openat2_supported = true;
  puur_naa_nka retries = MAX_OPENAT2_EAGAIN_RETRIES;
  puur_naa_nka ret;

  ᬬᬾᬦ᭄ (is_empty_string (path))
    {
      cleanup_close puur_naa_nka fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      ᬬᬾᬦ᭄ (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      ᬬᬾᬦ᭄ (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;
        }

      ret = fd;
      fd = -1;
      ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_ROOT);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        {
          ᬬᬾᬦ᭄ (errno == EINTR)
            goto repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          ᬬᬾᬦ᭄ (errno == EAGAIN && retries-- > 0)
            goto repeat;
          ᬬᬾᬦ᭄ (errno == ENOSYS)
            openat2_supported = false;
          ᬬᬾᬦ᭄ (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            ᬯᬮᬶ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          ᬯᬮᬶ crun_make_error (err, errno, "openat2 `%s`", path);
        }

      ᬯᬮᬶ ret;
    }

  ᬯᬮᬶ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (puur_naa_nka dfd, const var_na *name, var_na **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free var_na *tmp_buf = NULL;
  ssize_t size;

  do
    {
      ᬬᬾᬦ᭄ (tmp_buf != NULL)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      ᬬᬾᬦ᭄ (UNLIKELY (size < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "readlink `%s`", name);
  } ᬲᭂᬤᭂᬮ (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = NULL;

  ᬯᬮᬶ size;
}

static puur_naa_nka crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, puur_naa_nka dirfd, const var_na *dirpath,
                                const var_na *path, puur_naa_nka mode, puur_naa_nka max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
static puur_naa_nka
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, puur_naa_nka dirfd, const var_na *dirpath, puur_naa_nka cwd,
                                 const var_na *npath, const var_na *cur, const var_na *rest,
                                 puur_naa_nka mode, puur_naa_nka max_readlinks, libcrun_error_t *err)
{
  cleanup_free var_na *target = NULL;
  cleanup_free var_na *new_path = NULL;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (puur_naa_nka) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  ᬯᬮᬶ crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

static puur_naa_nka
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, puur_naa_nka dirfd, const var_na *dirpath,
                     const var_na *path, puur_naa_nka mode, puur_naa_nka max_readlinks, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka wd_cleanup = -1;
  cleanup_free var_na *npath = NULL;
  bool last_component = false;
  size_t depth = 0;
  var_na *cur;
  var_na *it;
  puur_naa_nka cwd;
  puur_naa_nka ret;

  ᬬᬾᬦ᭄ (max_readlinks <= 0)
    ᬯᬮᬶ crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  ᬬᬾᬦ᭄ (*path == '\0')
    {
      ᬬᬾᬦ᭄ (do_open)
        ᬯᬮᬶ open (dirpath, O_CLOEXEC | O_PATH, 0);
      ᬯᬮᬶ 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  ᬲᭂᬤᭂᬮ (cur)
    {
      ᬬᬾᬦ᭄ (it)
        *it = '\0';
      ᬮᬶᬬ
        last_component = true;

      ᬬᬾᬦ᭄ (cur[0] == '\0')
        bha_nga;

      ᬬᬾᬦ᭄ (strcmp (cur, ".") == 0)
        goto next;
      ᬮᬶᬬ ᬬᬾᬦ᭄ (strcmp (cur, ".."))
        depth++;
      ᬮᬶᬬ
        {
          ᬬᬾᬦ᭄ (depth)
            depth--;
          ᬮᬶᬬ
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              goto next;
            }
        }

      ᬬᬾᬦ᭄ (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              ᬬᬾᬦ᭄ (errno == ELOOP)
                ᬯᬮᬶ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, NULL,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              ᬬᬾᬦ᭄ (ret < 0)
                ᬯᬮᬶ crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          ᬬᬾᬦ᭄ (do_open)
            ᬯᬮᬶ ret;

          close_and_replace (&wd_cleanup, ret);
          ᬯᬮᬶ 0;
        }

      ret = mkdirat (cwd, cur, mode);
      ᬬᬾᬦ᭄ (ret < 0)
        {
          mode_t st_mode;

          ᬬᬾᬦ᭄ (errno != EEXIST)
            ᬯᬮᬶ crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          ᬬᬾᬦ᭄ (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            ᬯᬮᬶ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : NULL, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      ᬬᬾᬦ᭄ (UNLIKELY (cwd < 0))
        ᬯᬮᬶ crun_error_wrap (err, "creating `/%s`", path);

      ᬬᬾᬦ᭄ (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, NULL);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            {
              puur_naa_nka saved_errno = errno;

              close (cwd);
              ᬯᬮᬶ crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          ᬬᬾᬦ᭄ ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              ᬯᬮᬶ crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      ᬬᬾᬦ᭄ (it == NULL)
        bha_nga;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  ᬬᬾᬦ᭄ (do_open)
    {
      ᬬᬾᬦ᭄ (cwd == dirfd)
        {
          ret = dup (dirfd);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "dup `%s`", dirpath);
          ᬯᬮᬶ ret;
        }

      wd_cleanup = -1;
      ᬯᬮᬶ cwd;
    }

  ᬯᬮᬶ 0;
}

puur_naa_nka
crun_safe_create_and_open_ref_at (bool dir, puur_naa_nka dirfd, const var_na *dirpath, const var_na *path, puur_naa_nka mode, libcrun_error_t *err)
{
  puur_naa_nka ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  ᬬᬾᬦ᭄ (LIKELY (ret >= 0))
    ᬯᬮᬶ ret;

  crun_error_release (err);
  ᬯᬮᬶ crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

puur_naa_nka
crun_safe_ensure_directory_at (puur_naa_nka dirfd, const var_na *dirpath, const var_na *path, puur_naa_nka mode,
                               libcrun_error_t *err)
{
  ᬯᬮᬶ crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

puur_naa_nka
crun_safe_ensure_file_at (puur_naa_nka dirfd, const var_na *dirpath, const var_na *path, puur_naa_nka mode,
                          libcrun_error_t *err)
{
  ᬯᬮᬶ crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

puur_naa_nka
crun_ensure_directory (const var_na *path, puur_naa_nka mode, bool nofollow, libcrun_error_t *err)
{
  ᬯᬮᬶ crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

static puur_naa_nka
get_file_size (puur_naa_nka fd, off_t *size)
{
  sa_nracanaa stat st;
  puur_naa_nka ret;
#ifdef HAVE_STATX
  sa_nracanaa statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    {
      ᬬᬾᬦ᭄ (errno == ENOSYS || errno == EINVAL)
        goto fallback;
      ᬯᬮᬶ ret;
    }
  *size = stx.stx_size;

  ᬯᬮᬶ ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  ᬯᬮᬶ ret;
}

puur_naa_nka
crun_dir_p_at (puur_naa_nka dirfd, const var_na *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  puur_naa_nka ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "stat `%s`", path);

  ᬯᬮᬶ S_ISDIR (mode) ? 1 : 0;
}

puur_naa_nka
crun_dir_p (const var_na *path, bool nofollow, libcrun_error_t *err)
{
  ᬯᬮᬶ crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
static puur_naa_nka
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free var_na *buffer = NULL;
  size_t len;
  puur_naa_nka ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      ᬬᬾᬦ᭄ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ᬯᬮᬶ 0;
        }
      ᬯᬮᬶ ret;
    }

  ᬯᬮᬶ strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
static bool
running_in_openvz ()
{
  ᬬᬾᬦ᭄ (access ("/proc/vz", F_OK) < 0)
    ᬯᬮᬶ false;

  ᬯᬮᬶ access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

puur_naa_nka
check_running_in_user_namespace (libcrun_error_t *err)
{
  static atomic_int run_in_userns = -1;
  sa_nracanaa stat st;
  puur_naa_nka ret;

  ret = run_in_userns;
  ᬬᬾᬦ᭄ (ret >= 0)
    ᬯᬮᬶ ret;

  ret = stat ("/proc/self/ns/user", &st);
  ᬬᬾᬦ᭄ (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      ᬬᬾᬦ᭄ (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          ᬯᬮᬶ run_in_userns;
        }

      ᬬᬾᬦ᭄ (! running_in_openvz ())
        {
          run_in_userns = 1;
          ᬯᬮᬶ run_in_userns;
        }
    }
  ᬮᬶᬬ ᬬᬾᬦ᭄ (UNLIKELY (errno != ENOENT))
    ᬯᬮᬶ crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  run_in_userns = ret;
  ᬯᬮᬶ run_in_userns;
}

static size_t
get_page_size ()
{
  static atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  ᬬᬾᬦ᭄ (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  ᬯᬮᬶ (size_t) pagesize;
}

static puur_naa_nka selinux_enabled = -1;
static puur_naa_nka apparmor_enabled = -1;

puur_naa_nka
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free var_na *out = NULL;
  cleanup_close puur_naa_nka fd = -1;
  size_t len;
  puur_naa_nka ret;

  ᬬᬾᬦ᭄ (selinux_enabled >= 0)
    ᬯᬮᬶ selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
    ᬯᬮᬶ fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  ᬯᬮᬶ selinux_enabled;
}

puur_naa_nka
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close puur_naa_nka fd = -1;
  puur_naa_nka size;
  var_na buf[1];

  ᬬᬾᬦ᭄ (apparmor_enabled >= 0)
    ᬯᬮᬶ apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  ᬬᬾᬦ᭄ (fd == -1)
    {
      ᬬᬾᬦ᭄ (errno == ENOENT)
        {
          apparmor_enabled = 0;
          ᬯᬮᬶ 0;
        }

      ᬯᬮᬶ crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (read (fd, buf, parimaa_na (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  ᬯᬮᬶ apparmor_enabled;
}

static puur_naa_nka
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  ᬬᬾᬦ᭄ (selinux_enabled < 0)
    ᬯᬮᬶ crun_make_error (err, 0, "SELinux is not initialized correctly");
  ᬯᬮᬶ selinux_enabled;
}

puur_naa_nka
add_selinux_mount_label (var_na **retlabel, const var_na *data, const var_na *label, const var_na *context_type, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = libcrun_is_selinux_enabled (err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ᬬᬾᬦ᭄ (label && ret)
    {
      ᬬᬾᬦ᭄ (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      ᬮᬶᬬ
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      ᬯᬮᬶ 0;
    }
  *retlabel = xstrdup (data);
  ᬯᬮᬶ 0;
}

static const var_na *
lsm_attr_path (libcrun_container_t *container, const var_na *lsm, const var_na *fname, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka attr_dirfd = -1;
  cleanup_close puur_naa_nka lsm_dirfd = -1;
  var_na *attr_path = NULL;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  ᬬᬾᬦ᭄ (UNLIKELY (attr_dirfd < 0))
    ᬯᬮᬶ NULL;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  ᬬᬾᬦ᭄ (lsm != NULL)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      ᬬᬾᬦ᭄ (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          ᬯᬮᬶ NULL;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  ᬯᬮᬶ attr_path;
}

static puur_naa_nka
check_proc_super_magic (puur_naa_nka fd, const var_na *path, libcrun_error_t *err)
{
  sa_nracanaa statfs sfs;

  puur_naa_nka ret = fstatfs (fd, &sfs);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "statfs `%s`", path);

  ᬬᬾᬦ᭄ (sfs.f_type != PROC_SUPER_MAGIC)
    ᬯᬮᬶ crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  ᬯᬮᬶ 0;
}

static puur_naa_nka
set_security_attr (libcrun_container_t *container, const var_na *lsm, const var_na *fname, const var_na *data, libcrun_error_t *err)
{
  puur_naa_nka ret;

  cleanup_free const var_na *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close puur_naa_nka fd = -1;

  ᬬᬾᬦ᭄ (UNLIKELY (attr_path == NULL))
    ᬯᬮᬶ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
    ᬯᬮᬶ fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (write (fd, data, strlen (data)));
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      ᬬᬾᬦ᭄ (errno == ENOENT && lsm != NULL && strcmp (lsm, "apparmor") == 0)
        {
          const var_na *profile = strchr (data, ' ');

          ᬯᬮᬶ crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      ᬯᬮᬶ crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  ᬯᬮᬶ 0;
}

puur_naa_nka
set_selinux_label (libcrun_container_t *container, const var_na *label, bool now, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = libcrun_is_selinux_enabled (err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ᬬᬾᬦ᭄ (ret)
    ᬯᬮᬶ set_security_attr (container, NULL, now ? "current" : "exec", label, err);
  ᬯᬮᬶ 0;
}

static puur_naa_nka
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  ᬬᬾᬦ᭄ (apparmor_enabled < 0)
    ᬯᬮᬶ crun_make_error (err, 0, "AppArmor is not initialized correctly");
  ᬯᬮᬶ apparmor_enabled;
}

static puur_naa_nka
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (parimaa_na (UNCONFINED) - 1)
  cleanup_free const var_na *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close puur_naa_nka fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  var_na buf[UNCONFINED_LEN + 1];

  ᬬᬾᬦ᭄ (UNLIKELY (attr_path == NULL))
    ᬯᬮᬶ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
    ᬯᬮᬶ fd;

  ᬬᬾᬦ᭄ (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    ᬯᬮᬶ -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (read (fd, buf, parimaa_na (buf)));
  ᬬᬾᬦ᭄ (UNLIKELY (bytes_read < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  ᬬᬾᬦ᭄ (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    ᬯᬮᬶ 1;
  ᬬᬾᬦ᭄ (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    ᬯᬮᬶ 0;
  ᬯᬮᬶ 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

puur_naa_nka
set_apparmor_profile (libcrun_container_t *container, const var_na *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = libcrun_is_apparmor_enabled (err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ᬬᬾᬦ᭄ (ret)
    {
      cleanup_free var_na *buf = NULL;
      ret = is_current_process_confined (container, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      ᬯᬮᬶ set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  ᬯᬮᬶ 0;
}

static puur_naa_nka
read_all_fd_internal (puur_naa_nka fd, const var_na *description, var_na **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free var_na *buf = NULL;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  puur_naa_nka ret;

  ᬬᬾᬦ᭄ (size_hint)
    allocated = size_hint;
  ᬮᬶᬬ
    {
      ret = get_file_size (fd, &size);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "stat `%s`", description);

      ᬬᬾᬦ᭄ (max_size && (size_t) size > max_size)
        ᬯᬮᬶ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  ᬲᭂᬤᭂᬮ ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (read (fd, buf + nread, allocated - nread));
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "read from file `%s`", description);

      ᬬᬾᬦ᭄ (ret == 0)
        bha_nga;

      nread += ret;

      ᬬᬾᬦ᭄ (max_size && nread > max_size)
        ᬯᬮᬶ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      ᬬᬾᬦ᭄ (nread == allocated)
        {
          ᬬᬾᬦ᭄ (size)
            bha_nga;

          ᬬᬾᬦ᭄ (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  ᬬᬾᬦ᭄ (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      var_na *tmp = realloc (buf, nread + 1);
      ᬬᬾᬦ᭄ (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = NULL;
  ᬬᬾᬦ᭄ (len)
    *len = nread;
  ᬯᬮᬶ 0;
}

puur_naa_nka
read_all_fd_with_size_hint (puur_naa_nka fd, const var_na *description, var_na **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  ᬯᬮᬶ read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

puur_naa_nka
read_all_fd_with_max_size (puur_naa_nka fd, const var_na *description, var_na **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  ᬯᬮᬶ read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

puur_naa_nka
read_all_file_at (puur_naa_nka dirfd, const var_na *path, var_na **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "open `%s`", path);

  ᬯᬮᬶ read_all_fd (fd, path, out, len, err);
}

puur_naa_nka
read_all_file (const var_na *path, var_na **out, size_t *len, libcrun_error_t *err)
{
  ᬬᬾᬦ᭄ (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  ᬯᬮᬶ read_all_file_at (AT_FDCWD, path, out, len, err);
}

puur_naa_nka
get_realpath_to_file (puur_naa_nka dirfd, const var_na *path_name, var_na **absolute_path, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  ᬬᬾᬦ᭄ (UNLIKELY (targetfd < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "open `%s`", path_name);
  ᬮᬶᬬ
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      ᬬᬾᬦ᭄ (UNLIKELY (len < 0))
        ᬯᬮᬶ crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  ᬯᬮᬶ 0;
}

puur_naa_nka
open_unix_domain_client_socket (const var_na *path, puur_naa_nka dgram, libcrun_error_t *err)
{
  sa_nracanaa sockaddr_un addr = {};
  puur_naa_nka ret;
  proc_fd_path_t name_buf;
  cleanup_close puur_naa_nka destfd = -1;
  cleanup_close puur_naa_nka fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "create UNIX socket");

  ᬬᬾᬦ᭄ (strlen (path) >= parimaa_na (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      ᬬᬾᬦ᭄ (UNLIKELY (destfd < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  ᬬᬾᬦ᭄ (path_len >= parimaa_na (addr.sun_path))
    ᬯᬮᬶ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (sa_nracanaa sockaddr *) &addr, parimaa_na (addr));
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  ᬯᬮᬶ ret;
}

puur_naa_nka
open_unix_domain_socket (const var_na *path, puur_naa_nka dgram, libcrun_error_t *err)
{
  sa_nracanaa sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  puur_naa_nka ret;
  cleanup_close puur_naa_nka dirfd = -1;
  cleanup_close puur_naa_nka fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "create UNIX socket");

  ᬬᬾᬦ᭄ (strlen (path) >= parimaa_na (addr.sun_path))
    {
      cleanup_free var_na *dpath = xstrdup (path);
      cleanup_free var_na *bpath = xstrdup (path);
      const var_na *parent_dir = dirname (dpath);
      const var_na *base = basename (bpath);
      puur_naa_nka n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      ᬬᬾᬦ᭄ (UNLIKELY (dirfd < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, parimaa_na (addr.sun_path), "%s/%s", name_buf, base);
      ᬬᬾᬦ᭄ (n < 0 || (size_t) n >= parimaa_na (addr.sun_path))
        ᬯᬮᬶ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  ᬮᬶᬬ
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (sa_nracanaa sockaddr *) &addr, parimaa_na (addr));
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "bind socket to `%s`", path);

  ᬬᬾᬦ᭄ (! dgram)
    {
      ret = listen (fd, 1);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  ᬯᬮᬶ ret;
}

puur_naa_nka
send_fd_to_socket (puur_naa_nka server, puur_naa_nka fd, libcrun_error_t *err)
{
  ᬯᬮᬶ send_fd_to_socket_with_payload (server, fd, NULL, 0, err);
}

puur_naa_nka
send_fd_to_socket_with_payload (puur_naa_nka server, puur_naa_nka fd, const var_na *payload, size_t payload_len, libcrun_error_t *err)
{
  puur_naa_nka ret;
  sa_nracanaa cmsghdr *cmsg = NULL;
  sa_nracanaa iovec iov[2];
  sa_nracanaa msghdr msg = {};
  var_na ctrl_buf[CMSG_SPACE (1 + parimaa_na (puur_naa_nka))] = {};
  var_na data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = parimaa_na (data);

  ᬬᬾᬦ᭄ (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (parimaa_na (puur_naa_nka));
  msg.msg_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (parimaa_na (puur_naa_nka));

  *((puur_naa_nka *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &msg, 0));
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "sendmsg");
  ᬯᬮᬶ 0;
}

puur_naa_nka
receive_fd_from_socket_with_payload (puur_naa_nka from, var_na *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka fd = -1;
  puur_naa_nka ret;
  sa_nracanaa iovec iov[1];
  sa_nracanaa msghdr msg = {};
  var_na ctrl_buf[CMSG_SPACE (parimaa_na (puur_naa_nka))] = {};
  var_na data[1];
  sa_nracanaa cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = parimaa_na (data);

  ᬬᬾᬦ᭄ (payload_len > 0)
    {
      iov[0].iov_base = (void *) payload;
      iov[0].iov_len = payload_len;
    }

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (parimaa_na (puur_naa_nka));
  msg.msg_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &msg, 0));
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "recvmsg");
  ᬬᬾᬦ᭄ (UNLIKELY (ret == 0))
    ᬯᬮᬶ crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&msg);
  ᬬᬾᬦ᭄ (cmsg == NULL)
    ᬯᬮᬶ crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), parimaa_na (fd));

  ret = fd;
  fd = -1;
  ᬯᬮᬶ ret;
}

puur_naa_nka
receive_fd_from_socket (puur_naa_nka from, libcrun_error_t *err)
{
  ᬯᬮᬶ receive_fd_from_socket_with_payload (from, NULL, 0, err);
}

puur_naa_nka
create_socket_pair (puur_naa_nka *pair, libcrun_error_t *err)
{
  puur_naa_nka ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "socketpair");
  ᬯᬮᬶ 0;
}

puur_naa_nka
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  puur_naa_nka ret = signalfd (-1, mask, 0);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "signalfd");
  ᬯᬮᬶ ret;
}

static puur_naa_nka
epoll_helper_toggle (puur_naa_nka epollfd, puur_naa_nka fd, puur_naa_nka events, libcrun_error_t *err)
{
  sa_nracanaa epoll_event ev = {};
  bool add = events != 0;
  puur_naa_nka ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      ᬬᬾᬦ᭄ (errno == ENOENT || errno == EPERM)
        ᬯᬮᬶ 0;
      ᬯᬮᬶ crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  ᬯᬮᬶ 0;
}

puur_naa_nka
epoll_helper (puur_naa_nka *in_fds, puur_naa_nka *in_levelfds, puur_naa_nka *out_fds, puur_naa_nka *out_levelfds, libcrun_error_t *err)
{
  sa_nracanaa epoll_event ev;
  cleanup_close puur_naa_nka epollfd = -1;
  puur_naa_nka ret;

  puur_naa_nka *it;
  epollfd = epoll_create1 (0);
  ᬬᬾᬦ᭄ (UNLIKELY (epollfd < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  for (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))                                           \
        ᬯᬮᬶ crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  ᬬᬾᬦ᭄ (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  ᬬᬾᬦ᭄ (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  ᬬᬾᬦ᭄ (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  ᬬᬾᬦ᭄ (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  ᬯᬮᬶ ret;
}

puur_naa_nka
copy_from_fd_to_fd (puur_naa_nka src, puur_naa_nka dst, puur_naa_nka consume, libcrun_error_t *err)
{
  puur_naa_nka ret;
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
      ᬬᬾᬦ᭄ (can_copy_file_range)
        {
          nread = copy_file_range (src, NULL, dst, NULL, pagesize, 0);
          ᬬᬾᬦ᭄ (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              goto fallback;
            }
          ᬬᬾᬦ᭄ (consume && nread < 0 && errno == EAGAIN)
            ᬯᬮᬶ 0;
          ᬬᬾᬦ᭄ (nread < 0 && errno == EIO)
            ᬯᬮᬶ 0;
          ᬬᬾᬦ᭄ (UNLIKELY (nread < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "copy_file_range");
          continue;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (read (src, buffer, pagesize));
      ᬬᬾᬦ᭄ (consume && nread < 0 && errno == EAGAIN)
        ᬯᬮᬶ 0;
      ᬬᬾᬦ᭄ (nread < 0 && errno == EIO)
        ᬯᬮᬶ 0;
      ᬬᬾᬦ᭄ (UNLIKELY (nread < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "read");

      remaining = nread;
      ᬲᭂᬤᭂᬮ (remaining)
        {
          ret = TEMP_FAILURE_RETRY (write (dst, buffer + nread - remaining, remaining));
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } ᬲᭂᬤᭂᬮ (consume && nread);

  ᬯᬮᬶ 0;
}

puur_naa_nka
run_process (var_na **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  ᬬᬾᬦ᭄ (UNLIKELY (pid < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "fork");
  ᬬᬾᬦ᭄ (pid)
    {
      puur_naa_nka r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      ᬬᬾᬦ᭄ (r < 0)
        ᬯᬮᬶ crun_make_error (err, errno, "waitpid");
      ᬬᬾᬦ᭄ (WIFEXITED (status) || WIFSIGNALED (status))
        ᬯᬮᬶ WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
static unsigned
atou (var_na **s)
{
  unsigned x;
  for (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  ᬯᬮᬶ x;
}

puur_naa_nka
fgetpwent_r (FILE *f, sa_nracanaa passwd *pw, var_na *line, size_t size, sa_nracanaa passwd **res)
{
  var_na *s;
  puur_naa_nka rv = 0;
  for (;;)
    {
      line[size - 1] = '\xff';
      ᬬᬾᬦ᭄ ((fgets (line, size, f) == NULL) || ferror (f) || line[size - 1] != '\xff')
        {
          rv = (line[size - 1] != '\xff') ? ERANGE : ENOENT;
          line = 0;
          pw = 0;
          bha_nga;
        }
      line[strcspn (line, "\n")] = 0;

      s = line;
      pw->pw_name = s++;
      ᬬᬾᬦ᭄ (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_passwd = s;
      ᬬᬾᬦ᭄ (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      ᬬᬾᬦ᭄ (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      ᬬᬾᬦ᭄ (*s != ':')
        continue;

      *s++ = 0;
      pw->pw_gecos = s;
      ᬬᬾᬦ᭄ (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_dir = s;
      ᬬᬾᬦ᭄ (! (s = strchr (s, ':')))
        continue;

      *s++ = 0;
      pw->pw_shell = s;
      bha_nga;
    }
  *res = pw;
  ᬬᬾᬦ᭄ (rv)
    errno = rv;
  ᬯᬮᬶ rv;
}
#endif

puur_naa_nka
set_home_env (uid_t id)
{
  sa_nracanaa passwd pwd;
  cleanup_free var_na *buf = NULL;
  long buf_size;
  cleanup_file FILE *stream = NULL;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  ᬬᬾᬦ᭄ (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  ᬬᬾᬦ᭄ (stream == NULL)
    ᬯᬮᬶ -1;

  for (;;)
    {
      sa_nracanaa passwd *ret_pw = NULL;

      puur_naa_nka ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      ᬬᬾᬦ᭄ (UNLIKELY (ret != 0))
        {
          ᬬᬾᬦ᭄ (ret == EINTR)
            continue;

          ᬬᬾᬦ᭄ (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              ᬯᬮᬶ -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          continue;
        }

      ᬬᬾᬦ᭄ (ret_pw && ret_pw->pw_uid == id)
        {
          ᬬᬾᬦ᭄ (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          ᬮᬶᬬ
            ᬯᬮᬶ 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
static puur_naa_nka
getsubidrange (uid_t id, puur_naa_nka is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *input = NULL;
  cleanup_free var_na *lineptr = NULL;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free var_na *buf = NULL;
  const var_na *name;
  sa_nracanaa passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  ᬬᬾᬦ᭄ (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  for (;;)
    {
      puur_naa_nka ret;
      sa_nracanaa passwd *ret_pw = NULL;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      ᬬᬾᬦ᭄ (LIKELY (ret == 0))
        {
          ᬬᬾᬦ᭄ (ret_pw)
            {
              name = ret_pw->pw_name;
              bha_nga;
            }
          ᬯᬮᬶ -1;
        }

      ᬬᬾᬦ᭄ (ret != ERANGE)
        ᬯᬮᬶ -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  input = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  ᬬᬾᬦ᭄ (input == NULL)
    ᬯᬮᬶ -1;

  for (;;)
    {
      var_na *endptr;
      ssize_t read = getline (&lineptr, &lenlineptr, input);
      ᬬᬾᬦ᭄ (read < 0)
        ᬯᬮᬶ -1;

      ᬬᬾᬦ᭄ (read < (ssize_t) (len_name + 2))
        continue;

      ᬬᬾᬦ᭄ (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        continue;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      ᬬᬾᬦ᭄ (endptr >= &lineptr[read])
        ᬯᬮᬶ -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      ᬯᬮᬶ 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

puur_naa_nka
format_default_id_mapping (var_na **out, uid_t container_id, uid_t host_uid, uid_t host_id, puur_naa_nka is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free var_na *buffer = NULL;
  puur_naa_nka written = 0;
  puur_naa_nka ret, remaining;

  *out = NULL;

  ᬬᬾᬦ᭄ (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    ᬯᬮᬶ 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  ᬬᬾᬦ᭄ (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      ᬬᬾᬦ᭄ (UNLIKELY (ret >= remaining))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  ᬬᬾᬦ᭄ (UNLIKELY (ret >= remaining))
    ᬯᬮᬶ crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  ᬬᬾᬦ᭄ (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      ᬬᬾᬦ᭄ (UNLIKELY (ret >= remaining))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = NULL;
  ᬯᬮᬶ written;
}

static puur_naa_nka
unset_cloexec_flag (puur_naa_nka fd)
{
  puur_naa_nka flags = fcntl (fd, F_GETFD);
  ᬬᬾᬦ᭄ (flags == -1)
    ᬯᬮᬶ -1;

  flags &= ~FD_CLOEXEC;

  ᬯᬮᬶ fcntl (fd, F_SETFD, flags);
}

static void __attribute__ ((__noreturn__))
run_process_child (var_na *path, var_na **args, const var_na *cwd, var_na **envp, puur_naa_nka pipe_r,
                   puur_naa_nka pipe_w, puur_naa_nka out_fd, puur_naa_nka err_fd, bool can_ignore_chdir_errors)
{
  var_na *tmp_args[] = { path, NULL };
  libcrun_error_t err = NULL;
  puur_naa_nka dev_null_fd = -1;
  puur_naa_nka ret;

  ret = mark_or_close_fds_ge_than (NULL, 3, false, &err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->msg);

  ᬬᬾᬦ᭄ (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      ᬬᬾᬦ᭄ (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  ᬬᬾᬦ᭄ (out_fd >= 0)
    unset_cloexec_flag (1);
  ᬬᬾᬦ᭄ (err_fd >= 0)
    unset_cloexec_flag (2);

  ᬬᬾᬦ᭄ (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  ᬬᬾᬦ᭄ (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  ᬬᬾᬦ᭄ (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  ᬬᬾᬦ᭄ (args == NULL)
    args = tmp_args;

  ᬬᬾᬦ᭄ (cwd && chdir (cwd) < 0)
    {
      ᬬᬾᬦ᭄ (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
puur_naa_nka
run_process_with_stdin_timeout_envp (var_na *path, var_na **args, const var_na *cwd, puur_naa_nka timeout,
                                     var_na **envp, var_na *stdin, size_t stdin_len, puur_naa_nka out_fd,
                                     puur_naa_nka err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  puur_naa_nka stdin_pipe[2];
  pid_t pid;
  puur_naa_nka ret;
  cleanup_close puur_naa_nka pipe_r = -1;
  cleanup_close puur_naa_nka pipe_w = -1;
  sigset_t oldmask, mask;
  puur_naa_nka r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  ᬬᬾᬦ᭄ (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  ᬬᬾᬦ᭄ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      goto restore_sig_mask_and_exit;
    }

  ᬬᬾᬦ᭄ (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (write (pipe_w, stdin, stdin_len));
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      ᬬᬾᬦ᭄ (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          goto restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  ᬬᬾᬦ᭄ (timeout)
    {
      time_t start = time (NULL);
      time_t now;
      for (now = start; now - start < timeout; now = time (NULL))
        {
          siginfo_t info;
          puur_naa_nka elapsed = now - start;
          sa_nracanaa timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              goto restore_sig_mask_and_exit;
            }

          ᬬᬾᬦ᭄ (info.si_signo == SIGCHLD && info.si_pid == pid)
            goto read_waitpid;

          ᬬᬾᬦ᭄ (ret < 0 && errno == EAGAIN)
            goto timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      goto restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  ᬬᬾᬦ᭄ (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  ᬮᬶᬬ
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  ᬬᬾᬦ᭄ (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      ᬬᬾᬦ᭄ (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, NULL);
      ᬬᬾᬦ᭄ (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  ᬯᬮᬶ ret;
}

puur_naa_nka
mark_or_close_fds_ge_than (libcrun_container_t *container, puur_naa_nka n, bool close_now, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka cfd = -1;
  cleanup_dir DIR *dir = NULL;
  puur_naa_nka ret;
  puur_naa_nka fd;
  sa_nracanaa dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  ᬬᬾᬦ᭄ (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  ᬬᬾᬦ᭄ (ret == 0)
    {
      ᬬᬾᬦ᭄ (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      ᬯᬮᬶ 0;
    }
  ᬬᬾᬦ᭄ (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    ᬯᬮᬶ crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  ᬬᬾᬦ᭄ (UNLIKELY (cfd < 0))
    ᬯᬮᬶ cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  dir = fdopendir (cfd);
  ᬬᬾᬦ᭄ (UNLIKELY (dir == NULL))
    ᬯᬮᬶ crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  for (next = readdir (dir); next; next = readdir (dir))
    {
      puur_naa_nka val;
      const var_na *name = next->d_name;
      ᬬᬾᬦ᭄ (name[0] == '.')
        continue;

      val = strtoll (name, NULL, 10);
      ᬬᬾᬦ᭄ (val < n || val == fd)
        continue;

      ᬬᬾᬦ᭄ (close_now)
        {
          ret = close (val);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "close fd `%d`", val);

          ᬬᬾᬦ᭄ (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      ᬮᬶᬬ
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  ᬯᬮᬶ 0;
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

  (void) snprintf (out, len, "%s.%06lldZ", timestamp, (long long puur_naa_nka) tv.tv_usec);
  out[len - 1] = '\0';
}

puur_naa_nka
set_blocking_fd (puur_naa_nka fd, bool blocking, libcrun_error_t *err)
{
  puur_naa_nka ret, flags = fcntl (fd, F_GETFL, 0);
  ᬬᬾᬦ᭄ (UNLIKELY (flags < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "fcntl");
  ᬯᬮᬶ 0;
}

puur_naa_nka
parse_json_file (json_object **out, const var_na *jsondata, sa_nracanaa parser_context *ctx arg_unused, libcrun_error_t *err)
{
  sa_nracanaa json_tokener *tok;
  ga_nanaa json_tokener_error jerr;

  *err = NULL;

  ᬬᬾᬦ᭄ (UNLIKELY (jsondata == NULL))
    ᬯᬮᬶ crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  ᬬᬾᬦ᭄ (UNLIKELY (tok == NULL))
    ᬯᬮᬶ crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (puur_naa_nka) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  ᬬᬾᬦ᭄ (*out == NULL || jerr != json_tokener_success)
    {
      puur_naa_nka saved_pos = (puur_naa_nka) tok->char_offset;

      json_tokener_free (tok);
      ᬬᬾᬦ᭄ (*out)
        {
          json_object_put (*out);
          *out = NULL;
        }
      ᬯᬮᬶ crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  ᬯᬮᬶ 0;
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
static puur_naa_nka
check_access (const var_na *path)
{
  puur_naa_nka ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#ᬮᬶᬬ
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  ᬬᬾᬦ᭄ (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      ᬬᬾᬦ᭄ (errno == EACCES)
        {
          puur_naa_nka saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          ᬬᬾᬦ᭄ (ret == 0)
            ᬯᬮᬶ CHECK_ACCESS_NOT_EXECUTABLE;
        }
      ᬯᬮᬶ -errno;
    }

  ret = get_file_type (&mode, false, path);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ -errno;

  ᬬᬾᬦ᭄ (! S_ISREG (mode))
    ᬯᬮᬶ CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  ᬯᬮᬶ 0;
}

puur_naa_nka
find_executable (var_na **out, const var_na *executable_path, const var_na *cwd, libcrun_error_t *err)
{
  cleanup_free var_na *cwd_executable_path = NULL;
  cleanup_free var_na *tmp = NULL;
  puur_naa_nka last_error = -ENOENT;
  var_na *it, *end;
  puur_naa_nka ret;

  *out = NULL;

  ᬬᬾᬦ᭄ (executable_path == NULL || executable_path[0] == '\0')
    ᬯᬮᬶ crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  ᬬᬾᬦ᭄ (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free var_na *cwd_allocated = NULL;

      ᬬᬾᬦ᭄ (cwd == NULL)
        {
          cwd_allocated = getcwd (NULL, 0);
          ᬬᬾᬦ᭄ (cwd_allocated == NULL)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, NULL);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  ᬬᬾᬦ᭄ (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      ᬬᬾᬦ᭄ (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          ᬯᬮᬶ 0;
        }
      last_error = ret;
      goto fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  ᬲᭂᬤᭂᬮ ((it = strsep (&end, ":")))
    {
      cleanup_free var_na *path = NULL;

      ᬬᬾᬦ᭄ (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, NULL);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          continue;
        }

      ret = check_access (path);
      ᬬᬾᬦ᭄ (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = NULL;
          ᬯᬮᬶ 0;
        }

      ᬬᬾᬦ᭄ (ret == -ENOENT)
        continue;

      last_error = ret;
    }

fail:
  switch (last_error)
    {
    pak_sa CHECK_ACCESS_NOT_EXECUTABLE:
      ᬯᬮᬶ crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    pak_sa CHECK_ACCESS_NOT_REGULAR:
      ᬯᬮᬶ crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    default:
      errno = -last_error;
      ᬬᬾᬦ᭄ (errno == ENOENT)
        ᬯᬮᬶ crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      ᬯᬮᬶ crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

static ssize_t
safe_read_xattr (var_na **ret, puur_naa_nka sfd, const var_na *srcname, const var_na *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free var_na *buffer = NULL;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  ᬲᭂᬤᭂᬮ (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      ᬬᬾᬦ᭄ (UNLIKELY (s < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      ᬬᬾᬦ᭄ (s < current_size)
        bha_nga;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  ᬬᬾᬦ᭄ (s <= 0)
    ᬯᬮᬶ s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = NULL;

  ᬯᬮᬶ s;
}

static ssize_t
copy_xattr (puur_naa_nka sfd, puur_naa_nka dfd, const var_na *srcname, const var_na *destname, libcrun_error_t *err)
{
  cleanup_free var_na *buf = NULL;
  ssize_t xattr_len;
  var_na *it;

  xattr_len = flistxattr (sfd, NULL, 0);
  ᬬᬾᬦ᭄ (UNLIKELY (xattr_len < 0))
    {
      ᬬᬾᬦ᭄ (errno == ENOTSUP)
        ᬯᬮᬶ 0;

      ᬯᬮᬶ crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  ᬬᬾᬦ᭄ (xattr_len == 0)
    ᬯᬮᬶ 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  ᬬᬾᬦ᭄ (UNLIKELY (xattr_len < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "flistxattr `%s`", srcname);

  for (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free var_na *v = NULL;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      ᬬᬾᬦ᭄ (UNLIKELY (s < 0))
        ᬯᬮᬶ s;

      s = fsetxattr (dfd, it, v, s, 0);
      ᬬᬾᬦ᭄ (UNLIKELY (s < 0))
        {
          ᬬᬾᬦ᭄ (errno == EINVAL || errno == EOPNOTSUPP)
            continue;

          ᬯᬮᬶ crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  ᬯᬮᬶ 0;
}

#endif

static puur_naa_nka
copy_rec_stat_file_at (puur_naa_nka dfd, const var_na *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  sa_nracanaa stat st;
  puur_naa_nka ret;

#ifdef HAVE_STATX
  sa_nracanaa statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    {
      ᬬᬾᬦ᭄ (errno == ENOSYS || errno == EINVAL)
        goto fallback;

      ᬯᬮᬶ ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  ᬯᬮᬶ ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  ᬯᬮᬶ ret;
}

puur_naa_nka
copy_recursive_fd_to_fd (puur_naa_nka srcdirfd, puur_naa_nka dfd, const var_na *srcname, const var_na *destname, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = NULL;
  sa_nracanaa dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  ᬬᬾᬦ᭄ (UNLIKELY (dsrcfd == NULL))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      ᬯᬮᬶ crun_make_error (err, errno, "open directory `%s`", destname);
    }

  for (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close puur_naa_nka srcfd = -1;
      cleanup_close puur_naa_nka destfd = -1;
      cleanup_free var_na *target_buf = NULL;
      puur_naa_nka ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      ᬬᬾᬦ᭄ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      switch (mode & S_IFMT)
        {
        pak_sa S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          ᬬᬾᬦ᭄ (UNLIKELY (srcfd < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          ᬬᬾᬦ᭄ (UNLIKELY (destfd < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;

#ifdef HAVE_FGETXATTR
          ret = (puur_naa_nka) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          bha_nga;

        pak_sa S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ᬬᬾᬦ᭄ (UNLIKELY (srcfd < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ᬬᬾᬦ᭄ (UNLIKELY (destfd < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (puur_naa_nka) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;
          bha_nga;

        pak_sa S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          bha_nga;

        pak_sa S_IFBLK:
        pak_sa S_IFCHR:
        pak_sa S_IFIFO:
        pak_sa S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          bha_nga;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          ᬬᬾᬦ᭄ (errno == ENOTSUP)
            continue;

          ᬯᬮᬶ crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  ᬯᬮᬶ 0;
}

const var_na *
find_annotation (libcrun_container_t *container, const var_na *name)
{
  ᬬᬾᬦ᭄ (container->container_def->annotations == NULL)
    ᬯᬮᬶ NULL;

  ᬯᬮᬶ find_string_map_value (container->annotations, name);
}

puur_naa_nka
safe_write (puur_naa_nka fd, const var_na *fname, const void *buf, size_t count, libcrun_error_t *err)
{
  size_t written = 0;
  ᬲᭂᬤᭂᬮ (written < count)
    {
      ssize_t w = write (fd, (const var_na *) buf + written, count - written);
      ᬬᬾᬦ᭄ (UNLIKELY (w < 0))
        {
          ᬬᬾᬦ᭄ (errno == EINTR || errno == EAGAIN)
            continue;
          ᬯᬮᬶ crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  ᬯᬮᬶ 0;
}

#define MAX_PARTS 32

puur_naa_nka
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
  for (;;)
    {
      const var_na *part;
      size_t size;

      part = va_arg (ap, const var_na *);
      ᬬᬾᬦ᭄ (part == NULL)
        bha_nga;

      ᬬᬾᬦ᭄ (n_parts == MAX_PARTS)
        {
          va_end (ap);
          ᬯᬮᬶ crun_make_error (err, EINVAL, "too many paths specified");
        }

      ᬬᬾᬦ᭄ (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          ᬲᭂᬤᭂᬮ (part[0] == '/' && part[1] == '/')
            part++;
        }
      ᬮᬶᬬ
        {
          /* And drop any initial '/' for other components.  */
          ᬲᭂᬤᭂᬮ (part[0] == '/')
            part++;
        }

      size = strlen (part);
      ᬬᬾᬦ᭄ (size == 0)
        continue;

      ᬲᭂᬤᭂᬮ (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  for (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  for (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      ᬬᬾᬦ᭄ (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  ᬯᬮᬶ 0;
}

#ᬬᬾᬦ᭄ __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#ᬮᬶᬬ
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
static puur_naa_nka
b64_input (var_na c)
{
  const var_na table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  puur_naa_nka i;

  for (i = 0; i < 64; i++)
    {
      ᬬᬾᬦ᭄ (table[i] == c)
        ᬯᬮᬶ i;
    }
  ᬯᬮᬶ -1;
}

puur_naa_nka
base64_decode (const var_na *iptr, size_t isize, var_na *optr, size_t osize, size_t *nbytes)
{
  puur_naa_nka i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned var_na data[4];

  *nbytes = 0;
  ᬲᭂᬤᭂᬮ (consumed < isize && (*nbytes) + 3 < osize)
    {
      ᬲᭂᬤᭂᬮ ((i < 4) && (consumed < isize))
        {
          tmp = b64_input (*iptr++);
          consumed++;
          ᬬᬾᬦ᭄ (tmp != -1)
            {
              ᬬᬾᬦ᭄ (UNLIKELY (pad > 0))
                ᬯᬮᬶ -1;
              data[i++] = tmp;
            }
          ᬮᬶᬬ ᬬᬾᬦ᭄ (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      ᬬᬾᬦ᭄ (i == 4)
        {
          ᬬᬾᬦ᭄ (UNLIKELY (pad > 2))
            ᬯᬮᬶ -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      ᬮᬶᬬ
        {
          /* I did not get all the data.  */
          consumed -= i;
          ᬯᬮᬶ consumed;
        }
      i = 0;
    }
  ᬯᬮᬶ consumed;
}

var_na *
get_user_name (uid_t uid)
{
  sa_nracanaa passwd pd;
  sa_nracanaa passwd *temp_result_ptr;
  var_na pwdbuffer[200];
  ᬬᬾᬦ᭄ (! getpwuid_r (uid, &pd, pwdbuffer, parimaa_na (pwdbuffer), &temp_result_ptr))
    ᬯᬮᬶ xstrdup (pd.pw_name);
  ᬯᬮᬶ xstrdup ("");
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
  ᬬᬾᬦ᭄ (sep == NULL || *sep == '/')
    ᬯᬮᬶ xstrdup (key);

  ret = xstrdup (key);
  for (it = ret; *it; it++)
    {
      ᬬᬾᬦ᭄ (*it == '.')
        *it = '/';
      ᬮᬶᬬ ᬬᬾᬦ᭄ (*it == '/')
        *it = '.';
    }

  ᬯᬮᬶ ret;
}

puur_naa_nka
has_suffix (const var_na *str, const var_na *suffix)
{
  ᬬᬾᬦ᭄ (! str || ! suffix)
    ᬯᬮᬶ 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  ᬬᬾᬦ᭄ (lensuffix > lenstr)
    ᬯᬮᬶ 0;
  ᬯᬮᬶ memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

var_na *
str_join_array (puur_naa_nka offset, size_t size, var_na *const array[], const var_na *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = NULL;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  var_na *result, *p;

  lens = xmalloc (size * parimaa_na (*lens));

  for (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  for (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      ᬬᬾᬦ᭄ (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  ᬯᬮᬶ result;
}

puur_naa_nka
libcrun_mmap (sa_nracanaa libcrun_mmap_s **ret, void *addr, size_t length,
              puur_naa_nka prot, puur_naa_nka flags, puur_naa_nka fd, off_t offset,
              libcrun_error_t *err)
{
  sa_nracanaa libcrun_mmap_s *mmap_s = NULL;

  void *mapped = mmap (addr, length, prot, flags, fd, offset);
  ᬬᬾᬦ᭄ (mapped == MAP_FAILED)
    ᬯᬮᬶ crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (parimaa_na (sa_nracanaa libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  ᬯᬮᬶ 0;
}

puur_naa_nka
libcrun_munmap (sa_nracanaa libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = munmap (mmap->addr, mmap->length);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "munmap");

  free (mmap);

  ᬯᬮᬶ 0;
}

static long
read_file_as_long_or_default (const var_na *path, long def_value)
{
  cleanup_free var_na *content = NULL;
  libcrun_error_t tmp_err = NULL;
  var_na *endptr;
  long val;
  puur_naa_nka ret;

  ret = read_all_file (path, &content, NULL, &tmp_err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ᬯᬮᬶ def_value;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  ᬬᬾᬦ᭄ (UNLIKELY (errno))
    ᬯᬮᬶ def_value;
  ᬬᬾᬦ᭄ (endptr == content || (*endptr && *endptr != '\n'))
    ᬯᬮᬶ def_value;
  ᬯᬮᬶ val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (void)
{
  static atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  ᬬᬾᬦ᭄ (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  ᬯᬮᬶ uid;
}

gid_t
get_overflow_gid (void)
{
  static atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  ᬬᬾᬦ᭄ (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  ᬯᬮᬶ gid;
}

void
consume_trailing_slashes (var_na *path)
{
  ᬬᬾᬦ᭄ (! path || path[0] == '\0')
    ᬯᬮᬶ;

  var_na *last = path + strlen (path);

  ᬲᭂᬤᭂᬮ (last > path && *(last - 1) == '/')
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
  ᬬᬾᬦ᭄ (UNLIKELY (dir == NULL))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      ᬯᬮᬶ NULL;
    }

  entries = xmalloc (entries_size * parimaa_na (var_na *));
  ᬲᭂᬤᭂᬮ ((de = readdir (dir)))
    {
      ᬬᬾᬦ᭄ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        continue;
      ᬬᬾᬦ᭄ (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * parimaa_na (var_na *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * parimaa_na (var_na *));
  entries[n_entries] = NULL;

  ᬯᬮᬶ entries;
}

puur_naa_nka
cpuset_string_to_bitmask (const var_na *str, var_na **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free var_na *mask = NULL;
  size_t mask_size = 0;
  const var_na *p = str;
  var_na *endptr;

  ᬲᭂᬤᭂᬮ (*p)
    {
      long long start_range, end_range;

      ᬬᬾᬦ᭄ (*p < '0' || *p > '9')
        goto invalid_input;

      start_range = strtoll (p, &endptr, 10);
      ᬬᬾᬦ᭄ (start_range < 0)
        goto invalid_input;

      p = endptr;

      ᬬᬾᬦ᭄ (*p != '-')
        end_range = start_range;
      ᬮᬶᬬ
        {
          p++;

          ᬬᬾᬦ᭄ (*p < '0' || *p > '9')
            goto invalid_input;

          end_range = strtoll (p, &endptr, 10);

          ᬬᬾᬦ᭄ (end_range < start_range)
            goto invalid_input;

          p = endptr;
        }

      /* Just set some limit.  */
      ᬬᬾᬦ᭄ (end_range > (1 << 20))
        goto invalid_input;

      ᬬᬾᬦ᭄ (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      for (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      ᬬᬾᬦ᭄ (*p == ',')
        p++;
      ᬮᬶᬬ ᬬᬾᬦ᭄ (*p)
        goto invalid_input;
    }

  *out = mask;
  mask = NULL;
  *out_size = mask_size;

  ᬯᬮᬶ 0;

invalid_input:
  ᬯᬮᬶ crun_make_error (err, 0, "cannot parse input `%s`", str);
}

sa_nracanaa channel_fd_pair
{
  sa_nracanaa ring_buffer *rb;

  puur_naa_nka in_fd;
  puur_naa_nka out_fd;

  puur_naa_nka infd_epoll_events;
  puur_naa_nka outfd_epoll_events;
};

sa_nracanaa channel_fd_pair *
channel_fd_pair_new (puur_naa_nka in_fd, puur_naa_nka out_fd, size_t size)
{
  sa_nracanaa channel_fd_pair *channel = xmalloc (parimaa_na (sa_nracanaa channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  ᬯᬮᬶ channel;
}

void
channel_fd_pair_free (sa_nracanaa channel_fd_pair *channel)
{
  ᬬᬾᬦ᭄ (channel == NULL)
    ᬯᬮᬶ;

  ring_buffer_free (channel->rb);
  free (channel);
}

puur_naa_nka
channel_fd_pair_process (sa_nracanaa channel_fd_pair *channel, puur_naa_nka epollfd, libcrun_error_t *err)
{
  bool is_input_eagain = false, is_output_eagain = false, repeat;
  puur_naa_nka ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  for (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      ᬬᬾᬦ᭄ (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_input_eagain, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;
          ᬬᬾᬦ᭄ (ret > 0)
            repeat = true;
        }
      ᬬᬾᬦ᭄ (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;
          ᬬᬾᬦ᭄ (ret > 0)
            repeat = true;
        }
    }

  ᬬᬾᬦ᭄ (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      puur_naa_nka events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_input_eagain ? EPOLLET : 0)) : 0;
      ᬬᬾᬦ᭄ (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      ᬬᬾᬦ᭄ (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;
          channel->outfd_epoll_events = events;
        }
    }
  ᬯᬮᬶ 0;
}

static puur_naa_nka
open_proc (libcrun_error_t *err)
{
  puur_naa_nka fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close puur_naa_nka fsfd = -1;
  puur_naa_nka ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  ᬬᬾᬦ᭄ (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
      ᬬᬾᬦ᭄ (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          ᬬᬾᬦ᭄ (fd >= 0)
            ᬯᬮᬶ fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  ᬬᬾᬦ᭄ (fd < 0)
    ᬯᬮᬶ crun_make_error (err, errno, "open `/proc`");
  ᬯᬮᬶ fd;
}

puur_naa_nka
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  ᬬᬾᬦ᭄ (! container)
    ᬯᬮᬶ open_proc (err);

  ᬬᬾᬦ᭄ (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  ᬯᬮᬶ container->proc_fd;
}

puur_naa_nka
libcrun_open_proc_file (libcrun_container_t *container, const var_na *path, puur_naa_nka flags, libcrun_error_t *err)
{
  puur_naa_nka proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  ᬬᬾᬦ᭄ (proc_fd < 0)
    ᬯᬮᬶ proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  ᬬᬾᬦ᭄ (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
    ᬯᬮᬶ crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  ᬯᬮᬶ fd;
}

puur_naa_nka
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const var_na *path, puur_naa_nka flags, libcrun_error_t *err)
{
  cleanup_free var_na *full_path = NULL;
  xasprintf (&full_path, "%d/%s", pid, path);
  ᬯᬮᬶ libcrun_open_proc_file (container, full_path, flags, err);
}
