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
#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "ring_buffer.h"
#અન્તર્ભાવ <stdarg.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <libgen.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/un.h>
#અન્તર્ભાવ <sys/socket.h>
#અન્તર્ભાવ <sys/signalfd.h>
#અન્તર્ભાવ <sys/epoll.h>
#અન્તર્ભાવ <sys/syscall.h>
#અન્તર્ભાવ <sys/wait.h>
#અન્તર્ભાવ <pwd.h>
#અન્તર્ભાવ <grp.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <sys/time.h>
#અન્તર્ભાવ <sys/xattr.h>
#અન્તર્ભાવ <sys/sysmacros.h>
#અન્તર્ભાવ <sys/vfs.h>
#અન્તર્ભાવ <linux/magic.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <sys/mman.h>
#ifdef HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#endif
#જો HAVE_STDATOMIC_H
#  include <stdatomic.h>
#નહીંતર
#  define atomic_long અસ્થિર long
#  define atomic_int અસ્થિર પૂર્ણાંક
#endif
#અન્તર્ભાવ "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_મૂળ
#  define RESOLVE_IN_મૂળ 0x10
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

પૂર્ણાંક
crun_path_exists (અચલ અક્ષર *path, libcrun_error_t *err)
{
  પૂર્ણાંક ret = access (path, F_OK);
  જો (ret < 0)
    {
      જો (errno == ENOENT)
        પાછા 0;
      પાછા crun_make_error (err, errno, "access `%s`", path);
    }
  પાછા 1;
}

પૂર્ણાંક
xasprintf (અક્ષર **str, અચલ અક્ષર *fmt, ...)
{
  પૂર્ણાંક ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  જો (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  પાછા ret;
}

પૂર્ણાંક
write_file_at_with_flags (પૂર્ણાંક dirfd, પૂર્ણાંક flags, mode_t mode, અચલ અક્ષર *name, અચલ શૂન્ય *data, size_t len, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક fd = -1;
  પૂર્ણાંક ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા (len > INT_MAX) ? INT_MAX : (પૂર્ણાંક) len;
}

પૂર્ણાંક
detach_process ()
{
  pid_t pid;
  જો (setsid () < 0)
    પાછા -1;
  pid = fork ();
  જો (pid < 0)
    પાછા -1;
  જો (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  પાછા 0;
}

પૂર્ણાંક
get_file_type_fd (પૂર્ણાંક fd, mode_t *mode)
{
  સંરચના stat st;
  પૂર્ણાંક ret;

#ifdef HAVE_STATX
  સંરચના statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno == ENOSYS || errno == EINVAL)
        જાઓ fallback;

      પાછા ret;
    }
  *mode = stx.stx_mode;
  પાછા ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  પાછા ret;
}

પૂર્ણાંક
get_file_type_at (પૂર્ણાંક dirfd, mode_t *mode, bool nofollow, અચલ અક્ષર *path)
{
  પૂર્ણાંક empty_path = path == ખાલી ? AT_EMPTY_PATH : 0;
  સંરચના stat st;
  પૂર્ણાંક ret;

#ifdef HAVE_STATX
  સંરચના statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno == ENOSYS || errno == EINVAL)
        જાઓ fallback;

      પાછા ret;
    }
  *mode = stx.stx_mode;
  પાછા ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  પાછા ret;
}

પૂર્ણાંક
get_file_type (mode_t *mode, bool nofollow, અચલ અક્ષર *path)
{
  પાછા get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

પૂર્ણાંક
create_file_if_missing_at (પૂર્ણાંક dirfd, અચલ અક્ષર *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  જો (fd_write < 0)
    {
      પૂર્ણાંક saved_errno = errno;
      mode_t tmp_mode;
      પૂર્ણાંક ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      જો (ret == 0 && S_ISREG (tmp_mode))
        પાછા 0;

      પાછા crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
ensure_directory_internal_at (પૂર્ણાંક dirfd, અક્ષર *path, size_t len, પૂર્ણાંક mode, libcrun_error_t *err)
{
  અક્ષર *it = path + len;
  પૂર્ણાંક ret = 0;
  bool parent_created = false;

  માટે (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      જો (ret == 0 || errno == EEXIST)
        પાછા 0;

      પૂર્ણાંક saved_errno = errno;
      જો (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = ખાલી;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          જો (ret > 0)
            પાછા 0;
          જો (ret < 0)
            crun_error_release (&tmp_err);

          પાછા crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      જ્યારે (it > path && *it != '/')
        {
          it--;
          len--;
        }
      જો (it == path)
        પાછા 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      parent_created = true;
    }
  પાછા ret;
}

પૂર્ણાંક
crun_ensure_directory_at (પૂર્ણાંક dirfd, અચલ અક્ષર *path, પૂર્ણાંક mode, bool nofollow, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_free અક્ષર *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (ret == 0)
    પાછા crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  પાછા 0;
}

સ્થિર પૂર્ણાંક
check_fd_is_path (અચલ અક્ષર *path, પૂર્ણાંક fd, અચલ અક્ષર *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  અક્ષર link[PATH_MAX];
  પૂર્ણાંક ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, કદ (link)));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "readlink `%s`", fdname);

  જો (((size_t) ret) != path_len || memcmp (link, path, path_len))
    પાછા crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  પાછા 0;
}

સ્થિર પૂર્ણાંક
check_fd_under_path (અચલ અક્ષર *rootfs, size_t rootfslen, પૂર્ણાંક fd, અચલ અક્ષર *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  અક્ષર link[PATH_MAX];
  પૂર્ણાંક ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  જો (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    પાછા 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, કદ (link)));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "readlink `%s`", fdname);

  જો (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    પાછા crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  પાછા 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
સ્થિર શૂન્ય
close_and_replace (પૂર્ણાંક *oldfd, પૂર્ણાંક newfd)
{
  જો (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
અક્ષર *chroot_realpath (અચલ અક્ષર *chroot, અચલ અક્ષર *path, અક્ષર resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
સ્થિર પૂર્ણાંક
safe_openat_fallback (પૂર્ણાંક dirfd, અચલ અક્ષર *rootfs, અચલ અક્ષર *path, પૂર્ણાંક flags,
                      પૂર્ણાંક mode, libcrun_error_t *err)
{
  cleanup_free અક્ષર *parent_path = ખાલી;
  અચલ અક્ષર *last_component = ખાલી;
  અચલ અક્ષર *orig_path = path;
  અચલ અક્ષર *path_in_chroot;
  cleanup_close પૂર્ણાંક fd = -1;
  અક્ષર resolved[PATH_MAX];
  અક્ષર buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  પૂર્ણાંક ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  જો (flags & O_NOFOLLOW)
    {
      અક્ષર *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      જો (sep == ખાલી)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      નહીંતર જો (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      જો (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  જો (path_in_chroot == ખાલી)
    પાછા crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  જો (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  જો (last_component)
    {
      ret = snprintf (resolved, કદ (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      જો (UNLIKELY (ret >= (પૂર્ણાંક) કદ (resolved)))
        પાછા crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  જો (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      પાછા ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = fd;
  fd = -1;
  પાછા ret;
}

પૂર્ણાંક
safe_openat (પૂર્ણાંક dirfd, અચલ અક્ષર *rootfs, અચલ અક્ષર *path, પૂર્ણાંક flags, પૂર્ણાંક mode,
             libcrun_error_t *err)
{
  સ્થિર bool openat2_supported = true;
  પૂર્ણાંક retries = MAX_OPENAT2_EAGAIN_RETRIES;
  પૂર્ણાંક ret;

  જો (is_empty_string (path))
    {
      cleanup_close પૂર્ણાંક fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      જો (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      જો (UNLIKELY (fd < 0))
        પાછા crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      જો (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      ret = fd;
      fd = -1;
      પાછા ret;
    }

  જો (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_મૂળ);
      જો (UNLIKELY (ret < 0))
        {
          જો (errno == EINTR)
            જાઓ repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          જો (errno == EAGAIN && retries-- > 0)
            જાઓ repeat;
          જો (errno == ENOSYS)
            openat2_supported = false;
          જો (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            પાછા safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          પાછા crun_make_error (err, errno, "openat2 `%s`", path);
        }

      પાછા ret;
    }

  પાછા safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (પૂર્ણાંક dfd, અચલ અક્ષર *name, અક્ષર **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free અક્ષર *tmp_buf = ખાલી;
  ssize_t size;

  કરો
    {
      જો (tmp_buf != ખાલી)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      જો (UNLIKELY (size < 0))
        પાછા crun_make_error (err, errno, "readlink `%s`", name);
  } જ્યારે (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = ખાલી;

  પાછા size;
}

સ્થિર પૂર્ણાંક crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, પૂર્ણાંક dirfd, અચલ અક્ષર *dirpath,
                                અચલ અક્ષર *path, પૂર્ણાંક mode, પૂર્ણાંક max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
સ્થિર પૂર્ણાંક
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, પૂર્ણાંક dirfd, અચલ અક્ષર *dirpath, પૂર્ણાંક cwd,
                                 અચલ અક્ષર *npath, અચલ અક્ષર *cur, અચલ અક્ષર *rest,
                                 પૂર્ણાંક mode, પૂર્ણાંક max_readlinks, libcrun_error_t *err)
{
  cleanup_free અક્ષર *target = ખાલી;
  cleanup_free અક્ષર *new_path = ખાલી;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (પૂર્ણાંક) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  પાછા crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

સ્થિર પૂર્ણાંક
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, પૂર્ણાંક dirfd, અચલ અક્ષર *dirpath,
                     અચલ અક્ષર *path, પૂર્ણાંક mode, પૂર્ણાંક max_readlinks, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક wd_cleanup = -1;
  cleanup_free અક્ષર *npath = ખાલી;
  bool last_component = false;
  size_t depth = 0;
  અક્ષર *cur;
  અક્ષર *it;
  પૂર્ણાંક cwd;
  પૂર્ણાંક ret;

  જો (max_readlinks <= 0)
    પાછા crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  જો (*path == '\0')
    {
      જો (do_open)
        પાછા open (dirpath, O_CLOEXEC | O_PATH, 0);
      પાછા 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  જ્યારે (cur)
    {
      જો (it)
        *it = '\0';
      નહીંતર
        last_component = true;

      જો (cur[0] == '\0')
        તોડો;

      જો (strcmp (cur, ".") == 0)
        જાઓ next;
      નહીંતર જો (strcmp (cur, ".."))
        depth++;
      નહીંતર
        {
          જો (depth)
            depth--;
          નહીંતર
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              જાઓ next;
            }
        }

      જો (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          જો (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              જો (errno == ELOOP)
                પાછા crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, ખાલી,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              જો (ret < 0)
                પાછા crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          જો (do_open)
            પાછા ret;

          close_and_replace (&wd_cleanup, ret);
          પાછા 0;
        }

      ret = mkdirat (cwd, cur, mode);
      જો (ret < 0)
        {
          mode_t st_mode;

          જો (errno != EEXIST)
            પાછા crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          જો (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            પાછા crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : ખાલી, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      જો (UNLIKELY (cwd < 0))
        પાછા crun_error_wrap (err, "creating `/%s`", path);

      જો (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, ખાલી);
          જો (UNLIKELY (ret < 0))
            {
              પૂર્ણાંક saved_errno = errno;

              close (cwd);
              પાછા crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          જો ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              પાછા crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      જો (it == ખાલી)
        તોડો;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  જો (do_open)
    {
      જો (cwd == dirfd)
        {
          ret = dup (dirfd);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "dup `%s`", dirpath);
          પાછા ret;
        }

      wd_cleanup = -1;
      પાછા cwd;
    }

  પાછા 0;
}

પૂર્ણાંક
crun_safe_create_and_open_ref_at (bool dir, પૂર્ણાંક dirfd, અચલ અક્ષર *dirpath, અચલ અક્ષર *path, પૂર્ણાંક mode, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  જો (LIKELY (ret >= 0))
    પાછા ret;

  crun_error_release (err);
  પાછા crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

પૂર્ણાંક
crun_safe_ensure_directory_at (પૂર્ણાંક dirfd, અચલ અક્ષર *dirpath, અચલ અક્ષર *path, પૂર્ણાંક mode,
                               libcrun_error_t *err)
{
  પાછા crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

પૂર્ણાંક
crun_safe_ensure_file_at (પૂર્ણાંક dirfd, અચલ અક્ષર *dirpath, અચલ અક્ષર *path, પૂર્ણાંક mode,
                          libcrun_error_t *err)
{
  પાછા crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

પૂર્ણાંક
crun_ensure_directory (અચલ અક્ષર *path, પૂર્ણાંક mode, bool nofollow, libcrun_error_t *err)
{
  પાછા crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

સ્થિર પૂર્ણાંક
get_file_size (પૂર્ણાંક fd, off_t *size)
{
  સંરચના stat st;
  પૂર્ણાંક ret;
#ifdef HAVE_STATX
  સંરચના statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno == ENOSYS || errno == EINVAL)
        જાઓ fallback;
      પાછા ret;
    }
  *size = stx.stx_size;

  પાછા ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  પાછા ret;
}

પૂર્ણાંક
crun_dir_p_at (પૂર્ણાંક dirfd, અચલ અક્ષર *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  પૂર્ણાંક ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "stat `%s`", path);

  પાછા S_ISDIR (mode) ? 1 : 0;
}

પૂર્ણાંક
crun_dir_p (અચલ અક્ષર *path, bool nofollow, libcrun_error_t *err)
{
  પાછા crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
સ્થિર પૂર્ણાંક
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free અક્ષર *buffer = ખાલી;
  size_t len;
  પૂર્ણાંક ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  જો (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      જો (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          પાછા 0;
        }
      પાછા ret;
    }

  પાછા strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
સ્થિર bool
running_in_openvz ()
{
  જો (access ("/proc/vz", F_OK) < 0)
    પાછા false;

  પાછા access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

પૂર્ણાંક
check_running_in_user_namespace (libcrun_error_t *err)
{
  સ્થિર atomic_int run_in_userns = -1;
  સંરચના stat st;
  પૂર્ણાંક ret;

  ret = run_in_userns;
  જો (ret >= 0)
    પાછા ret;

  ret = stat ("/proc/self/ns/user", &st);
  જો (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      જો (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          પાછા run_in_userns;
        }

      જો (! running_in_openvz ())
        {
          run_in_userns = 1;
          પાછા run_in_userns;
        }
    }
  નહીંતર જો (UNLIKELY (errno != ENOENT))
    પાછા crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  run_in_userns = ret;
  પાછા run_in_userns;
}

સ્થિર size_t
get_page_size ()
{
  સ્થિર atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  જો (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  પાછા (size_t) pagesize;
}

સ્થિર પૂર્ણાંક selinux_enabled = -1;
સ્થિર પૂર્ણાંક apparmor_enabled = -1;

પૂર્ણાંક
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free અક્ષર *out = ખાલી;
  cleanup_close પૂર્ણાંક fd = -1;
  size_t len;
  પૂર્ણાંક ret;

  જો (selinux_enabled >= 0)
    પાછા selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  જો (UNLIKELY (fd < 0))
    પાછા fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  પાછા selinux_enabled;
}

પૂર્ણાંક
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક fd = -1;
  પૂર્ણાંક size;
  અક્ષર buf[1];

  જો (apparmor_enabled >= 0)
    પાછા apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  જો (fd == -1)
    {
      જો (errno == ENOENT)
        {
          apparmor_enabled = 0;
          પાછા 0;
        }

      પાછા crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (પઠ (fd, buf, કદ (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  પાછા apparmor_enabled;
}

સ્થિર પૂર્ણાંક
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  જો (selinux_enabled < 0)
    પાછા crun_make_error (err, 0, "SELinux is not initialized correctly");
  પાછા selinux_enabled;
}

પૂર્ણાંક
add_selinux_mount_label (અક્ષર **retlabel, અચલ અક્ષર *data, અચલ અક્ષર *label, અચલ અક્ષર *context_type, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = libcrun_is_selinux_enabled (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (label && ret)
    {
      જો (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      નહીંતર
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      પાછા 0;
    }
  *retlabel = xstrdup (data);
  પાછા 0;
}

સ્થિર અચલ અક્ષર *
lsm_attr_path (libcrun_container_t *container, અચલ અક્ષર *lsm, અચલ અક્ષર *fname, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક attr_dirfd = -1;
  cleanup_close પૂર્ણાંક lsm_dirfd = -1;
  અક્ષર *attr_path = ખાલી;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  જો (UNLIKELY (attr_dirfd < 0))
    પાછા ખાલી;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  જો (lsm != ખાલી)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      જો (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          પાછા ખાલી;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  પાછા attr_path;
}

સ્થિર પૂર્ણાંક
check_proc_super_magic (પૂર્ણાંક fd, અચલ અક્ષર *path, libcrun_error_t *err)
{
  સંરચના statfs sfs;

  પૂર્ણાંક ret = fstatfs (fd, &sfs);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "statfs `%s`", path);

  જો (sfs.f_type != PROC_SUPER_MAGIC)
    પાછા crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  પાછા 0;
}

સ્થિર પૂર્ણાંક
set_security_attr (libcrun_container_t *container, અચલ અક્ષર *lsm, અચલ અક્ષર *fname, અચલ અક્ષર *data, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  cleanup_free અચલ અક્ષર *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close પૂર્ણાંક fd = -1;

  જો (UNLIKELY (attr_path == ખાલી))
    પાછા -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  જો (UNLIKELY (fd < 0))
    પાછા fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (લિખ (fd, data, strlen (data)));
  જો (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      જો (errno == ENOENT && lsm != ખાલી && strcmp (lsm, "apparmor") == 0)
        {
          અચલ અક્ષર *profile = strchr (data, ' ');

          પાછા crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      પાછા crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  પાછા 0;
}

પૂર્ણાંક
set_selinux_label (libcrun_container_t *container, અચલ અક્ષર *label, bool now, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = libcrun_is_selinux_enabled (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (ret)
    પાછા set_security_attr (container, ખાલી, now ? "current" : "exec", label, err);
  પાછા 0;
}

સ્થિર પૂર્ણાંક
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  જો (apparmor_enabled < 0)
    પાછા crun_make_error (err, 0, "AppArmor is not initialized correctly");
  પાછા apparmor_enabled;
}

સ્થિર પૂર્ણાંક
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (કદ (UNCONFINED) - 1)
  cleanup_free અચલ અક્ષર *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close પૂર્ણાંક fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  અક્ષર buf[UNCONFINED_LEN + 1];

  જો (UNLIKELY (attr_path == ખાલી))
    પાછા -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  જો (UNLIKELY (fd < 0))
    પાછા fd;

  જો (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    પાછા -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (પઠ (fd, buf, કદ (buf)));
  જો (UNLIKELY (bytes_read < 0))
    પાછા crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  જો (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    પાછા 1;
  જો (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    પાછા 0;
  પાછા 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

પૂર્ણાંક
set_apparmor_profile (libcrun_container_t *container, અચલ અક્ષર *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = libcrun_is_apparmor_enabled (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (ret)
    {
      cleanup_free અક્ષર *buf = ખાલી;
      ret = is_current_process_confined (container, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      પાછા set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
read_all_fd_internal (પૂર્ણાંક fd, અચલ અક્ષર *description, અક્ષર **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free અક્ષર *buf = ખાલી;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  પૂર્ણાંક ret;

  જો (size_hint)
    allocated = size_hint;
  નહીંતર
    {
      ret = get_file_size (fd, &size);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "stat `%s`", description);

      જો (max_size && (size_t) size > max_size)
        પાછા crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  જ્યારે ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (પઠ (fd, buf + nread, allocated - nread));
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "read from file `%s`", description);

      જો (ret == 0)
        તોડો;

      nread += ret;

      જો (max_size && nread > max_size)
        પાછા crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      જો (nread == allocated)
        {
          જો (size)
            તોડો;

          જો (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  જો (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      અક્ષર *tmp = realloc (buf, nread + 1);
      જો (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = ખાલી;
  જો (len)
    *len = nread;
  પાછા 0;
}

પૂર્ણાંક
read_all_fd_with_size_hint (પૂર્ણાંક fd, અચલ અક્ષર *description, અક્ષર **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  પાછા read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

પૂર્ણાંક
read_all_fd_with_max_size (પૂર્ણાંક fd, અચલ અક્ષર *description, અક્ષર **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  પાછા read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

પૂર્ણાંક
read_all_file_at (પૂર્ણાંક dirfd, અચલ અક્ષર *path, અક્ષર **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", path);

  પાછા read_all_fd (fd, path, out, len, err);
}

પૂર્ણાંક
read_all_file (અચલ અક્ષર *path, અક્ષર **out, size_t *len, libcrun_error_t *err)
{
  જો (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  પાછા read_all_file_at (AT_FDCWD, path, out, len, err);
}

પૂર્ણાંક
get_realpath_to_file (પૂર્ણાંક dirfd, અચલ અક્ષર *path_name, અક્ષર **absolute_path, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  જો (UNLIKELY (targetfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", path_name);
  નહીંતર
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      જો (UNLIKELY (len < 0))
        પાછા crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  પાછા 0;
}

પૂર્ણાંક
open_unix_domain_client_socket (અચલ અક્ષર *path, પૂર્ણાંક dgram, libcrun_error_t *err)
{
  સંરચના sockaddr_un addr = {};
  પૂર્ણાંક ret;
  proc_fd_path_t name_buf;
  cleanup_close પૂર્ણાંક destfd = -1;
  cleanup_close પૂર્ણાંક fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "create UNIX socket");

  જો (strlen (path) >= કદ (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      જો (UNLIKELY (destfd < 0))
        પાછા crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  જો (path_len >= કદ (addr.sun_path))
    પાછા crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (સંરચના sockaddr *) &addr, કદ (addr));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  પાછા ret;
}

પૂર્ણાંક
open_unix_domain_socket (અચલ અક્ષર *path, પૂર્ણાંક dgram, libcrun_error_t *err)
{
  સંરચના sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  પૂર્ણાંક ret;
  cleanup_close પૂર્ણાંક dirfd = -1;
  cleanup_close પૂર્ણાંક fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "create UNIX socket");

  જો (strlen (path) >= કદ (addr.sun_path))
    {
      cleanup_free અક્ષર *dpath = xstrdup (path);
      cleanup_free અક્ષર *bpath = xstrdup (path);
      અચલ અક્ષર *parent_dir = dirname (dpath);
      અચલ અક્ષર *base = basename (bpath);
      પૂર્ણાંક n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      જો (UNLIKELY (dirfd < 0))
        પાછા crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, કદ (addr.sun_path), "%s/%s", name_buf, base);
      જો (n < 0 || (size_t) n >= કદ (addr.sun_path))
        પાછા crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  નહીંતર
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (સંરચના sockaddr *) &addr, કદ (addr));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "bind socket to `%s`", path);

  જો (! dgram)
    {
      ret = listen (fd, 1);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  પાછા ret;
}

પૂર્ણાંક
send_fd_to_socket (પૂર્ણાંક server, પૂર્ણાંક fd, libcrun_error_t *err)
{
  પાછા send_fd_to_socket_with_payload (server, fd, ખાલી, 0, err);
}

પૂર્ણાંક
send_fd_to_socket_with_payload (પૂર્ણાંક server, પૂર્ણાંક fd, અચલ અક્ષર *payload, size_t payload_len, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  સંરચના cmsghdr *cmsg = ખાલી;
  સંરચના iovec iov[2];
  સંરચના msghdr સંદેશ = {};
  અક્ષર ctrl_buf[CMSG_SPACE (1 + કદ (પૂર્ણાંક))] = {};
  અક્ષર data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = કદ (data);

  જો (payload_len > 0)
    {
      iov[0].iov_base = (શૂન્ય *) payload;
      iov[0].iov_len = payload_len;
    }

  સંદેશ.સંદેશ_name = ખાલી;
  સંદેશ.સંદેશ_namelen = 0;
  સંદેશ.સંદેશ_iov = iov;
  સંદેશ.સંદેશ_iovlen = 1;
  સંદેશ.સંદેશ_controllen = CMSG_SPACE (કદ (પૂર્ણાંક));
  સંદેશ.સંદેશ_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&સંદેશ);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (કદ (પૂર્ણાંક));

  *((પૂર્ણાંક *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &સંદેશ, 0));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "sendmsg");
  પાછા 0;
}

પૂર્ણાંક
receive_fd_from_socket_with_payload (પૂર્ણાંક from, અક્ષર *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક fd = -1;
  પૂર્ણાંક ret;
  સંરચના iovec iov[1];
  સંરચના msghdr સંદેશ = {};
  અક્ષર ctrl_buf[CMSG_SPACE (કદ (પૂર્ણાંક))] = {};
  અક્ષર data[1];
  સંરચના cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = કદ (data);

  જો (payload_len > 0)
    {
      iov[0].iov_base = (શૂન્ય *) payload;
      iov[0].iov_len = payload_len;
    }

  સંદેશ.સંદેશ_name = ખાલી;
  સંદેશ.સંદેશ_namelen = 0;
  સંદેશ.સંદેશ_iov = iov;
  સંદેશ.સંદેશ_iovlen = 1;
  સંદેશ.સંદેશ_controllen = CMSG_SPACE (કદ (પૂર્ણાંક));
  સંદેશ.સંદેશ_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &સંદેશ, 0));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "recvmsg");
  જો (UNLIKELY (ret == 0))
    પાછા crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&સંદેશ);
  જો (cmsg == ખાલી)
    પાછા crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), કદ (fd));

  ret = fd;
  fd = -1;
  પાછા ret;
}

પૂર્ણાંક
receive_fd_from_socket (પૂર્ણાંક from, libcrun_error_t *err)
{
  પાછા receive_fd_from_socket_with_payload (from, ખાલી, 0, err);
}

પૂર્ણાંક
create_socket_pair (પૂર્ણાંક *pair, libcrun_error_t *err)
{
  પૂર્ણાંક ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "socketpair");
  પાછા 0;
}

પૂર્ણાંક
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  પૂર્ણાંક ret = signalfd (-1, mask, 0);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "signalfd");
  પાછા ret;
}

સ્થિર પૂર્ણાંક
epoll_helper_toggle (પૂર્ણાંક epollfd, પૂર્ણાંક fd, પૂર્ણાંક events, libcrun_error_t *err)
{
  સંરચના epoll_event ev = {};
  bool add = events != 0;
  પૂર્ણાંક ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  જો (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  જો (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      જો (errno == ENOENT || errno == EPERM)
        પાછા 0;
      પાછા crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  પાછા 0;
}

પૂર્ણાંક
epoll_helper (પૂર્ણાંક *in_fds, પૂર્ણાંક *in_levelfds, પૂર્ણાંક *out_fds, પૂર્ણાંક *out_levelfds, libcrun_error_t *err)
{
  સંરચના epoll_event ev;
  cleanup_close પૂર્ણાંક epollfd = -1;
  પૂર્ણાંક ret;

  પૂર્ણાંક *it;
  epollfd = epoll_create1 (0);
  જો (UNLIKELY (epollfd < 0))
    પાછા crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  માટે (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      જો (UNLIKELY (ret < 0))                                           \
        પાછા crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  જો (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  જો (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  જો (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  જો (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  પાછા ret;
}

પૂર્ણાંક
copy_from_fd_to_fd (પૂર્ણાંક src, પૂર્ણાંક dst, પૂર્ણાંક consume, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  ssize_t nread;
  size_t pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  કરો
    {
      cleanup_free અક્ષર *buffer = ખાલી;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      જો (can_copy_file_range)
        {
          nread = copy_file_range (src, ખાલી, dst, ખાલી, pagesize, 0);
          જો (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              જાઓ fallback;
            }
          જો (consume && nread < 0 && errno == EAGAIN)
            પાછા 0;
          જો (nread < 0 && errno == EIO)
            પાછા 0;
          જો (UNLIKELY (nread < 0))
            પાછા crun_make_error (err, errno, "copy_file_range");
          ચાલુ;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (પઠ (src, buffer, pagesize));
      જો (consume && nread < 0 && errno == EAGAIN)
        પાછા 0;
      જો (nread < 0 && errno == EIO)
        પાછા 0;
      જો (UNLIKELY (nread < 0))
        પાછા crun_make_error (err, errno, "read");

      remaining = nread;
      જ્યારે (remaining)
        {
          ret = TEMP_FAILURE_RETRY (લિખ (dst, buffer + nread - remaining, remaining));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } જ્યારે (consume && nread);

  પાછા 0;
}

પૂર્ણાંક
run_process (અક્ષર **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  જો (UNLIKELY (pid < 0))
    પાછા crun_make_error (err, errno, "fork");
  જો (pid)
    {
      પૂર્ણાંક r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      જો (r < 0)
        પાછા crun_make_error (err, errno, "waitpid");
      જો (WIFEXITED (status) || WIFSIGNALED (status))
        પાછા WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
સ્થિર unsigned
atou (અક્ષર **s)
{
  unsigned x;
  માટે (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  પાછા x;
}

પૂર્ણાંક
fgetpwent_r (FILE *f, સંરચના passwd *pw, અક્ષર *લીટી, size_t size, સંરચના passwd **res)
{
  અક્ષર *s;
  પૂર્ણાંક rv = 0;
  માટે (;;)
    {
      લીટી[size - 1] = '\xff';
      જો ((fgets (લીટી, size, f) == ખાલી) || ferror (f) || લીટી[size - 1] != '\xff')
        {
          rv = (લીટી[size - 1] != '\xff') ? ERANGE : ENOENT;
          લીટી = 0;
          pw = 0;
          તોડો;
        }
      લીટી[strcspn (લીટી, "\n")] = 0;

      s = લીટી;
      pw->pw_name = s++;
      જો (! (s = strchr (s, ':')))
        ચાલુ;

      *s++ = 0;
      pw->pw_passwd = s;
      જો (! (s = strchr (s, ':')))
        ચાલુ;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      જો (*s != ':')
        ચાલુ;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      જો (*s != ':')
        ચાલુ;

      *s++ = 0;
      pw->pw_gecos = s;
      જો (! (s = strchr (s, ':')))
        ચાલુ;

      *s++ = 0;
      pw->pw_dir = s;
      જો (! (s = strchr (s, ':')))
        ચાલુ;

      *s++ = 0;
      pw->pw_shell = s;
      તોડો;
    }
  *res = pw;
  જો (rv)
    errno = rv;
  પાછા rv;
}
#endif

પૂર્ણાંક
set_home_env (uid_t id)
{
  સંરચના passwd pwd;
  cleanup_free અક્ષર *buf = ખાલી;
  long buf_size;
  cleanup_file FILE *stream = ખાલી;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  જો (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  જો (stream == ખાલી)
    પાછા -1;

  માટે (;;)
    {
      સંરચના passwd *ret_pw = ખાલી;

      પૂર્ણાંક ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      જો (UNLIKELY (ret != 0))
        {
          જો (ret == EINTR)
            ચાલુ;

          જો (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              પાછા -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          ચાલુ;
        }

      જો (ret_pw && ret_pw->pw_uid == id)
        {
          જો (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          નહીંતર
            પાછા 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
સ્થિર પૂર્ણાંક
getsubidrange (uid_t id, પૂર્ણાંક is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *આગત = ખાલી;
  cleanup_free અક્ષર *lineptr = ખાલી;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free અક્ષર *buf = ખાલી;
  અચલ અક્ષર *name;
  સંરચના passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  જો (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  માટે (;;)
    {
      પૂર્ણાંક ret;
      સંરચના passwd *ret_pw = ખાલી;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      જો (LIKELY (ret == 0))
        {
          જો (ret_pw)
            {
              name = ret_pw->pw_name;
              તોડો;
            }
          પાછા -1;
        }

      જો (ret != ERANGE)
        પાછા -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  આગત = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  જો (આગત == ખાલી)
    પાછા -1;

  માટે (;;)
    {
      અક્ષર *endptr;
      ssize_t પઠ = getline (&lineptr, &lenlineptr, આગત);
      જો (પઠ < 0)
        પાછા -1;

      જો (પઠ < (ssize_t) (len_name + 2))
        ચાલુ;

      જો (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        ચાલુ;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      જો (endptr >= &lineptr[પઠ])
        પાછા -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      પાછા 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

પૂર્ણાંક
format_default_id_mapping (અક્ષર **out, uid_t container_id, uid_t host_uid, uid_t host_id, પૂર્ણાંક is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free અક્ષર *buffer = ખાલી;
  પૂર્ણાંક written = 0;
  પૂર્ણાંક ret, remaining;

  *out = ખાલી;

  જો (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    પાછા 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  જો (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      જો (UNLIKELY (ret >= remaining))
        પાછા crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  જો (UNLIKELY (ret >= remaining))
    પાછા crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  જો (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      જો (UNLIKELY (ret >= remaining))
        પાછા crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = ખાલી;
  પાછા written;
}

સ્થિર પૂર્ણાંક
unset_cloexec_flag (પૂર્ણાંક fd)
{
  પૂર્ણાંક flags = fcntl (fd, F_GETFD);
  જો (flags == -1)
    પાછા -1;

  flags &= ~FD_CLOEXEC;

  પાછા fcntl (fd, F_SETFD, flags);
}

સ્થિર શૂન્ય __attribute__ ((__noreturn__))
run_process_child (અક્ષર *path, અક્ષર **args, અચલ અક્ષર *cwd, અક્ષર **envp, પૂર્ણાંક pipe_r,
                   પૂર્ણાંક pipe_w, પૂર્ણાંક out_fd, પૂર્ણાંક err_fd, bool can_ignore_chdir_errors)
{
  અક્ષર *tmp_args[] = { path, ખાલી };
  libcrun_error_t err = ખાલી;
  પૂર્ણાંક dev_null_fd = -1;
  પૂર્ણાંક ret;

  ret = mark_or_close_fds_ge_than (ખાલી, 3, false, &err);
  જો (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->સંદેશ);

  જો (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      જો (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  જો (out_fd >= 0)
    unset_cloexec_flag (1);
  જો (err_fd >= 0)
    unset_cloexec_flag (2);

  જો (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  જો (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  જો (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  જો (args == ખાલી)
    args = tmp_args;

  જો (cwd && chdir (cwd) < 0)
    {
      જો (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
પૂર્ણાંક
run_process_with_stdin_timeout_envp (અક્ષર *path, અક્ષર **args, અચલ અક્ષર *cwd, પૂર્ણાંક timeout,
                                     અક્ષર **envp, અક્ષર *stdin, size_t stdin_len, પૂર્ણાંક out_fd,
                                     પૂર્ણાંક err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  પૂર્ણાંક stdin_pipe[2];
  pid_t pid;
  પૂર્ણાંક ret;
  cleanup_close પૂર્ણાંક pipe_r = -1;
  cleanup_close પૂર્ણાંક pipe_w = -1;
  sigset_t oldmask, mask;
  પૂર્ણાંક r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  જો (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  જો (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      જાઓ restore_sig_mask_and_exit;
    }

  જો (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (લિખ (pipe_w, stdin, stdin_len));
  જો (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      જો (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          જાઓ restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  જો (timeout)
    {
      time_t start = time (ખાલી);
      time_t now;
      માટે (now = start; now - start < timeout; now = time (ખાલી))
        {
          siginfo_t info;
          પૂર્ણાંક elapsed = now - start;
          સંરચના timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          જો (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              જાઓ restore_sig_mask_and_exit;
            }

          જો (info.si_signo == SIGCHLD && info.si_pid == pid)
            જાઓ read_waitpid;

          જો (ret < 0 && errno == EAGAIN)
            જાઓ timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      જાઓ restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  જો (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  નહીંતર
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  જો (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      જો (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, ખાલી);
      જો (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  પાછા ret;
}

પૂર્ણાંક
mark_or_close_fds_ge_than (libcrun_container_t *container, પૂર્ણાંક n, bool close_now, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક cfd = -1;
  cleanup_dir DIR *dir = ખાલી;
  પૂર્ણાંક ret;
  પૂર્ણાંક fd;
  સંરચના dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  જો (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  જો (ret == 0)
    {
      જો (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      પાછા 0;
    }
  જો (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    પાછા crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  જો (UNLIKELY (cfd < 0))
    પાછા cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  dir = fdopendir (cfd);
  જો (UNLIKELY (dir == ખાલી))
    પાછા crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  માટે (next = readdir (dir); next; next = readdir (dir))
    {
      પૂર્ણાંક val;
      અચલ અક્ષર *name = next->d_name;
      જો (name[0] == '.')
        ચાલુ;

      val = strtoll (name, ખાલી, 10);
      જો (val < n || val == fd)
        ચાલુ;

      જો (close_now)
        {
          ret = close (val);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "close fd `%d`", val);

          જો (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      નહીંતર
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  પાછા 0;
}

શૂન્ય
get_current_timestamp (અક્ષર *out, size_t len)
{
  સંરચના timeval tv;
  સંરચના tm now;
  અક્ષર timestamp[64];

  gettimeofday (&tv, ખાલી);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, કદ (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (શૂન્ય) snprintf (out, len, "%s.%06lldZ", timestamp, (long long પૂર્ણાંક) tv.tv_usec);
  out[len - 1] = '\0';
}

પૂર્ણાંક
set_blocking_fd (પૂર્ણાંક fd, bool blocking, libcrun_error_t *err)
{
  પૂર્ણાંક ret, flags = fcntl (fd, F_GETFL, 0);
  જો (UNLIKELY (flags < 0))
    પાછા crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fcntl");
  પાછા 0;
}

પૂર્ણાંક
parse_json_file (json_object **out, અચલ અક્ષર *jsondata, સંરચના parser_context *ctx arg_unused, libcrun_error_t *err)
{
  સંરચના json_tokener *tok;
  ગણના json_tokener_error jerr;

  *err = ખાલી;

  જો (UNLIKELY (jsondata == ખાલી))
    પાછા crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  જો (UNLIKELY (tok == ખાલી))
    પાછા crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (પૂર્ણાંક) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  જો (*out == ખાલી || jerr != json_tokener_success)
    {
      પૂર્ણાંક saved_pos = (પૂર્ણાંક) tok->char_offset;

      json_tokener_free (tok);
      જો (*out)
        {
          json_object_put (*out);
          *out = ખાલી;
        }
      પાછા crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  પાછા 0;
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
સ્થિર પૂર્ણાંક
check_access (અચલ અક્ષર *path)
{
  પૂર્ણાંક ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#નહીંતર
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  જો (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      જો (errno == EACCES)
        {
          પૂર્ણાંક saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          જો (ret == 0)
            પાછા CHECK_ACCESS_NOT_EXECUTABLE;
        }
      પાછા -errno;
    }

  ret = get_file_type (&mode, false, path);
  જો (UNLIKELY (ret < 0))
    પાછા -errno;

  જો (! S_ISREG (mode))
    પાછા CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  પાછા 0;
}

પૂર્ણાંક
find_executable (અક્ષર **out, અચલ અક્ષર *executable_path, અચલ અક્ષર *cwd, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cwd_executable_path = ખાલી;
  cleanup_free અક્ષર *tmp = ખાલી;
  પૂર્ણાંક last_error = -ENOENT;
  અક્ષર *it, *end;
  પૂર્ણાંક ret;

  *out = ખાલી;

  જો (executable_path == ખાલી || executable_path[0] == '\0')
    પાછા crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  જો (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free અક્ષર *cwd_allocated = ખાલી;

      જો (cwd == ખાલી)
        {
          cwd_allocated = getcwd (ખાલી, 0);
          જો (cwd_allocated == ખાલી)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  જો (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      જો (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          પાછા 0;
        }
      last_error = ret;
      જાઓ fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  જ્યારે ((it = strsep (&end, ":")))
    {
      cleanup_free અક્ષર *path = ખાલી;

      જો (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, ખાલી);
      જો (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ચાલુ;
        }

      ret = check_access (path);
      જો (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = ખાલી;
          પાછા 0;
        }

      જો (ret == -ENOENT)
        ચાલુ;

      last_error = ret;
    }

fail:
  પસંદગી (last_error)
    {
    સ્થિતિ CHECK_ACCESS_NOT_EXECUTABLE:
      પાછા crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    સ્થિતિ CHECK_ACCESS_NOT_REGULAR:
      પાછા crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    મૂળભૂત:
      errno = -last_error;
      જો (errno == ENOENT)
        પાછા crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      પાછા crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

સ્થિર ssize_t
safe_read_xattr (અક્ષર **ret, પૂર્ણાંક sfd, અચલ અક્ષર *srcname, અચલ અક્ષર *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free અક્ષર *buffer = ખાલી;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  જ્યારે (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      જો (UNLIKELY (s < 0))
        પાછા crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      જો (s < current_size)
        તોડો;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  જો (s <= 0)
    પાછા s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = ખાલી;

  પાછા s;
}

સ્થિર ssize_t
copy_xattr (પૂર્ણાંક sfd, પૂર્ણાંક dfd, અચલ અક્ષર *srcname, અચલ અક્ષર *destname, libcrun_error_t *err)
{
  cleanup_free અક્ષર *buf = ખાલી;
  ssize_t xattr_len;
  અક્ષર *it;

  xattr_len = flistxattr (sfd, ખાલી, 0);
  જો (UNLIKELY (xattr_len < 0))
    {
      જો (errno == ENOTSUP)
        પાછા 0;

      પાછા crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  જો (xattr_len == 0)
    પાછા 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  જો (UNLIKELY (xattr_len < 0))
    પાછા crun_make_error (err, errno, "flistxattr `%s`", srcname);

  માટે (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free અક્ષર *v = ખાલી;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      જો (UNLIKELY (s < 0))
        પાછા s;

      s = fsetxattr (dfd, it, v, s, 0);
      જો (UNLIKELY (s < 0))
        {
          જો (errno == EINVAL || errno == EOPNOTSUPP)
            ચાલુ;

          પાછા crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  પાછા 0;
}

#endif

સ્થિર પૂર્ણાંક
copy_rec_stat_file_at (પૂર્ણાંક dfd, અચલ અક્ષર *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  સંરચના stat st;
  પૂર્ણાંક ret;

#ifdef HAVE_STATX
  સંરચના statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno == ENOSYS || errno == EINVAL)
        જાઓ fallback;

      પાછા ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  પાછા ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  પાછા ret;
}

પૂર્ણાંક
copy_recursive_fd_to_fd (પૂર્ણાંક srcdirfd, પૂર્ણાંક dfd, અચલ અક્ષર *srcname, અચલ અક્ષર *destname, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = ખાલી;
  સંરચના dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  જો (UNLIKELY (dsrcfd == ખાલી))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      પાછા crun_make_error (err, errno, "open directory `%s`", destname);
    }

  માટે (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close પૂર્ણાંક srcfd = -1;
      cleanup_close પૂર્ણાંક destfd = -1;
      cleanup_free અક્ષર *target_buf = ખાલી;
      પૂર્ણાંક ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      જો (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ચાલુ;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      પસંદગી (mode & S_IFMT)
        {
        સ્થિતિ S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          જો (UNLIKELY (srcfd < 0))
            પાછા crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          જો (UNLIKELY (destfd < 0))
            પાછા crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

#ifdef HAVE_FGETXATTR
          ret = (પૂર્ણાંક) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          તોડો;

        સ્થિતિ S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          જો (UNLIKELY (srcfd < 0))
            પાછા crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          જો (UNLIKELY (destfd < 0))
            પાછા crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (પૂર્ણાંક) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          તોડો;

        સ્થિતિ S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          તોડો;

        સ્થિતિ S_IFBLK:
        સ્થિતિ S_IFCHR:
        સ્થિતિ S_IFIFO:
        સ્થિતિ S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          તોડો;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      જો (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          જો (errno == ENOTSUP)
            ચાલુ;

          પાછા crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  પાછા 0;
}

અચલ અક્ષર *
find_annotation (libcrun_container_t *container, અચલ અક્ષર *name)
{
  જો (container->container_def->annotations == ખાલી)
    પાછા ખાલી;

  પાછા find_string_map_કિંમત (container->annotations, name);
}

પૂર્ણાંક
safe_write (પૂર્ણાંક fd, અચલ અક્ષર *fname, અચલ શૂન્ય *buf, size_t ગણતરી, libcrun_error_t *err)
{
  size_t written = 0;
  જ્યારે (written < ગણતરી)
    {
      ssize_t w = લિખ (fd, (અચલ અક્ષર *) buf + written, ગણતરી - written);
      જો (UNLIKELY (w < 0))
        {
          જો (errno == EINTR || errno == EAGAIN)
            ચાલુ;
          પાછા crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  પાછા 0;
}

#define MAX_PARTS 32

પૂર્ણાંક
append_paths (અક્ષર **out, libcrun_error_t *err, ...)
{
  અચલ અક્ષર *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t total_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  માટે (;;)
    {
      અચલ અક્ષર *part;
      size_t size;

      part = va_arg (ap, અચલ અક્ષર *);
      જો (part == ખાલી)
        તોડો;

      જો (n_parts == MAX_PARTS)
        {
          va_end (ap);
          પાછા crun_make_error (err, EINVAL, "too many paths specified");
        }

      જો (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          જ્યારે (part[0] == '/' && part[1] == '/')
            part++;
        }
      નહીંતર
        {
          /* And drop any initial '/' for other components.  */
          જ્યારે (part[0] == '/')
            part++;
        }

      size = strlen (part);
      જો (size == 0)
        ચાલુ;

      જ્યારે (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  માટે (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  માટે (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      જો (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  પાછા 0;
}

#જો __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#નહીંતર
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
સ્થિર પૂર્ણાંક
b64_આગત (અક્ષર c)
{
  અચલ અક્ષર table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  પૂર્ણાંક i;

  માટે (i = 0; i < 64; i++)
    {
      જો (table[i] == c)
        પાછા i;
    }
  પાછા -1;
}

પૂર્ણાંક
base64_decode (અચલ અક્ષર *iptr, size_t isize, અક્ષર *optr, size_t osize, size_t *nbytes)
{
  પૂર્ણાંક i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned અક્ષર data[4];

  *nbytes = 0;
  જ્યારે (consumed < isize && (*nbytes) + 3 < osize)
    {
      જ્યારે ((i < 4) && (consumed < isize))
        {
          tmp = b64_આગત (*iptr++);
          consumed++;
          જો (tmp != -1)
            {
              જો (UNLIKELY (pad > 0))
                પાછા -1;
              data[i++] = tmp;
            }
          નહીંતર જો (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      જો (i == 4)
        {
          જો (UNLIKELY (pad > 2))
            પાછા -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      નહીંતર
        {
          /* I did not get all the data.  */
          consumed -= i;
          પાછા consumed;
        }
      i = 0;
    }
  પાછા consumed;
}

અક્ષર *
get_user_name (uid_t uid)
{
  સંરચના passwd pd;
  સંરચના passwd *temp_result_ptr;
  અક્ષર pwdbuffer[200];
  જો (! getpwuid_r (uid, &pd, pwdbuffer, કદ (pwdbuffer), &temp_result_ptr))
    પાછા xstrdup (pd.pw_name);
  પાછા xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
અક્ષર *
libcrun_sysctl_ચાવી_to_proc_path (અચલ અક્ષર *ચાવી)
{
  અચલ અક્ષર *sep;
  અક્ષર *ret, *it;

  sep = strpbrk (ચાવી, "./");

  /* No separator at all, or the key is already in the path form.  */
  જો (sep == ખાલી || *sep == '/')
    પાછા xstrdup (ચાવી);

  ret = xstrdup (ચાવી);
  માટે (it = ret; *it; it++)
    {
      જો (*it == '.')
        *it = '/';
      નહીંતર જો (*it == '/')
        *it = '.';
    }

  પાછા ret;
}

પૂર્ણાંક
has_suffix (અચલ અક્ષર *str, અચલ અક્ષર *suffix)
{
  જો (! str || ! suffix)
    પાછા 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  જો (lensuffix > lenstr)
    પાછા 0;
  પાછા memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

અક્ષર *
str_join_array (પૂર્ણાંક offset, size_t size, અક્ષર *અચલ array[], અચલ અક્ષર *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = ખાલી;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  અક્ષર *result, *p;

  lens = xmalloc (size * કદ (*lens));

  માટે (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  માટે (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      જો (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  પાછા result;
}

પૂર્ણાંક
libcrun_mmap (સંરચના libcrun_mmap_s **ret, શૂન્ય *addr, size_t length,
              પૂર્ણાંક prot, પૂર્ણાંક flags, પૂર્ણાંક fd, off_t offset,
              libcrun_error_t *err)
{
  સંરચના libcrun_mmap_s *mmap_s = ખાલી;

  શૂન્ય *mapped = mmap (addr, length, prot, flags, fd, offset);
  જો (mapped == MAP_FAILED)
    પાછા crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (કદ (સંરચના libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  પાછા 0;
}

પૂર્ણાંક
libcrun_munmap (સંરચના libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = munmap (mmap->addr, mmap->length);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "munmap");

  મુક્ત (mmap);

  પાછા 0;
}

સ્થિર long
read_file_as_long_or_default (અચલ અક્ષર *path, long def_કિંમત)
{
  cleanup_free અક્ષર *content = ખાલી;
  libcrun_error_t tmp_err = ખાલી;
  અક્ષર *endptr;
  long val;
  પૂર્ણાંક ret;

  ret = read_all_file (path, &content, ખાલી, &tmp_err);
  જો (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      પાછા def_કિંમત;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  જો (UNLIKELY (errno))
    પાછા def_કિંમત;
  જો (endptr == content || (*endptr && *endptr != '\n'))
    પાછા def_કિંમત;
  પાછા val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (શૂન્ય)
{
  સ્થિર atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  જો (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  પાછા uid;
}

gid_t
get_overflow_gid (શૂન્ય)
{
  સ્થિર atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  જો (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  પાછા gid;
}

શૂન્ય
consume_trailing_slashes (અક્ષર *path)
{
  જો (! path || path[0] == '\0')
    પાછા;

  અક્ષર *last = path + strlen (path);

  જ્યારે (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

અક્ષર **
read_dir_entries (અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = ખાલી;
  size_t n_entries = 0;
  size_t entries_size = 16;
  અક્ષર **entries = ખાલી;
  સંરચના dirent *de;

  dir = opendir (path);
  જો (UNLIKELY (dir == ખાલી))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      પાછા ખાલી;
    }

  entries = xmalloc (entries_size * કદ (અક્ષર *));
  જ્યારે ((de = readdir (dir)))
    {
      જો (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ચાલુ;
      જો (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * કદ (અક્ષર *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * કદ (અક્ષર *));
  entries[n_entries] = ખાલી;

  પાછા entries;
}

પૂર્ણાંક
cpuset_string_to_bitmask (અચલ અક્ષર *str, અક્ષર **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free અક્ષર *mask = ખાલી;
  size_t mask_size = 0;
  અચલ અક્ષર *p = str;
  અક્ષર *endptr;

  જ્યારે (*p)
    {
      long long start_range, end_range;

      જો (*p < '0' || *p > '9')
        જાઓ invalid_આગત;

      start_range = strtoll (p, &endptr, 10);
      જો (start_range < 0)
        જાઓ invalid_આગત;

      p = endptr;

      જો (*p != '-')
        end_range = start_range;
      નહીંતર
        {
          p++;

          જો (*p < '0' || *p > '9')
            જાઓ invalid_આગત;

          end_range = strtoll (p, &endptr, 10);

          જો (end_range < start_range)
            જાઓ invalid_આગત;

          p = endptr;
        }

      /* Just set some limit.  */
      જો (end_range > (1 << 20))
        જાઓ invalid_આગત;

      જો (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      માટે (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      જો (*p == ',')
        p++;
      નહીંતર જો (*p)
        જાઓ invalid_આગત;
    }

  *out = mask;
  mask = ખાલી;
  *out_size = mask_size;

  પાછા 0;

invalid_આગત:
  પાછા crun_make_error (err, 0, "cannot parse input `%s`", str);
}

સંરચના channel_fd_pair
{
  સંરચના ring_buffer *rb;

  પૂર્ણાંક in_fd;
  પૂર્ણાંક out_fd;

  પૂર્ણાંક infd_epoll_events;
  પૂર્ણાંક outfd_epoll_events;
};

સંરચના channel_fd_pair *
channel_fd_pair_new (પૂર્ણાંક in_fd, પૂર્ણાંક out_fd, size_t size)
{
  સંરચના channel_fd_pair *channel = xmalloc (કદ (સંરચના channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  પાછા channel;
}

શૂન્ય
channel_fd_pair_free (સંરચના channel_fd_pair *channel)
{
  જો (channel == ખાલી)
    પાછા;

  ring_buffer_free (channel->rb);
  મુક્ત (channel);
}

પૂર્ણાંક
channel_fd_pair_process (સંરચના channel_fd_pair *channel, પૂર્ણાંક epollfd, libcrun_error_t *err)
{
  bool is_આગત_eagain = false, is_output_eagain = false, repeat;
  પૂર્ણાંક ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  માટે (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      જો (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_આગત_eagain, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          જો (ret > 0)
            repeat = true;
        }
      જો (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          જો (ret > 0)
            repeat = true;
        }
    }

  જો (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      પૂર્ણાંક events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_આગત_eagain ? EPOLLET : 0)) : 0;
      જો (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      જો (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          channel->outfd_epoll_events = events;
        }
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
open_proc (libcrun_error_t *err)
{
  પૂર્ણાંક fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close પૂર્ણાંક fsfd = -1;
  પૂર્ણાંક ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  જો (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ખાલી, ખાલી, 0);
      જો (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          જો (fd >= 0)
            પાછા fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  જો (fd < 0)
    પાછા crun_make_error (err, errno, "open `/proc`");
  પાછા fd;
}

પૂર્ણાંક
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  જો (! container)
    પાછા open_proc (err);

  જો (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  પાછા container->proc_fd;
}

પૂર્ણાંક
libcrun_open_proc_file (libcrun_container_t *container, અચલ અક્ષર *path, પૂર્ણાંક flags, libcrun_error_t *err)
{
  પૂર્ણાંક proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  જો (proc_fd < 0)
    પાછા proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  જો (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  પાછા fd;
}

પૂર્ણાંક
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, અચલ અક્ષર *path, પૂર્ણાંક flags, libcrun_error_t *err)
{
  cleanup_free અક્ષર *full_path = ખાલી;
  xasprintf (&full_path, "%d/%s", pid, path);
  પાછા libcrun_open_proc_file (container, full_path, flags, err);
}
