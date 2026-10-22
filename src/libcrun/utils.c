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
#شامل <config.h>
#شامل "utils.h"
#شامل "ring_buffer.h"
#شامل <stdarg.h>
#شامل <inttypes.h>
#شامل <unistd.h>
#شامل <string.h>
#شامل <libgen.h>
#شامل <fcntl.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <sys/un.h>
#شامل <sys/socket.h>
#شامل <sys/signalfd.h>
#شامل <sys/epoll.h>
#شامل <sys/syscall.h>
#شامل <sys/wait.h>
#شامل <pwd.h>
#شامل <grp.h>
#شامل <time.h>
#شامل <sys/time.h>
#شامل <sys/xattr.h>
#شامل <sys/sysmacros.h>
#شامل <sys/vfs.h>
#شامل <linux/magic.h>
#شامل <limits.h>
#شامل <sys/mman.h>
#ifdef HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#endif
#اگر HAVE_STDATOMIC_H
#  include <stdatomic.h>
#وگرنه
#  define atomic_long فرار long
#  define atomic_int فرار صحیح
#endif
#شامل "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_ریشه
#  define RESOLVE_IN_ریشه 0x10
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

صحیح
crun_path_exists (ثابت نویسه *path, libcrun_error_t *err)
{
  صحیح ret = access (path, F_OK);
  اگر (ret < 0)
    {
      اگر (errno == ENOENT)
        بازگشت 0;
      بازگشت crun_make_error (err, errno, "access `%s`", path);
    }
  بازگشت 1;
}

صحیح
xasprintf (نویسه **str, ثابت نویسه *fmt, ...)
{
  صحیح ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  اگر (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  بازگشت ret;
}

صحیح
write_file_at_with_flags (صحیح dirfd, صحیح flags, mode_t mode, ثابت نویسه *name, ثابت تهی *data, size_t len, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  صحیح ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت (len > INT_MAX) ? INT_MAX : (صحیح) len;
}

صحیح
detach_process ()
{
  pid_t pid;
  اگر (setsid () < 0)
    بازگشت -1;
  pid = fork ();
  اگر (pid < 0)
    بازگشت -1;
  اگر (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  بازگشت 0;
}

صحیح
get_file_type_fd (صحیح fd, mode_t *mode)
{
  ساختار stat st;
  صحیح ret;

#ifdef HAVE_STATX
  ساختار statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS || errno == EINVAL)
        برو fallback;

      بازگشت ret;
    }
  *mode = stx.stx_mode;
  بازگشت ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  بازگشت ret;
}

صحیح
get_file_type_at (صحیح dirfd, mode_t *mode, bool nofollow, ثابت نویسه *path)
{
  صحیح empty_path = path == پوچ ? AT_EMPTY_PATH : 0;
  ساختار stat st;
  صحیح ret;

#ifdef HAVE_STATX
  ساختار statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS || errno == EINVAL)
        برو fallback;

      بازگشت ret;
    }
  *mode = stx.stx_mode;
  بازگشت ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  بازگشت ret;
}

صحیح
get_file_type (mode_t *mode, bool nofollow, ثابت نویسه *path)
{
  بازگشت get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

صحیح
create_file_if_missing_at (صحیح dirfd, ثابت نویسه *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close صحیح fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  اگر (fd_write < 0)
    {
      صحیح saved_errno = errno;
      mode_t tmp_mode;
      صحیح ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      اگر (ret == 0 && S_ISREG (tmp_mode))
        بازگشت 0;

      بازگشت crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  بازگشت 0;
}

ایستا صحیح
ensure_directory_internal_at (صحیح dirfd, نویسه *path, size_t len, صحیح mode, libcrun_error_t *err)
{
  نویسه *it = path + len;
  صحیح ret = 0;
  bool parent_created = false;

  برای (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      اگر (ret == 0 || errno == EEXIST)
        بازگشت 0;

      صحیح saved_errno = errno;
      اگر (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = پوچ;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          اگر (ret > 0)
            بازگشت 0;
          اگر (ret < 0)
            crun_error_release (&tmp_err);

          بازگشت crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      وقتی (it > path && *it != '/')
        {
          it--;
          len--;
        }
      اگر (it == path)
        بازگشت 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      parent_created = true;
    }
  بازگشت ret;
}

صحیح
crun_ensure_directory_at (صحیح dirfd, ثابت نویسه *path, صحیح mode, bool nofollow, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free نویسه *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (ret == 0)
    بازگشت crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  بازگشت 0;
}

ایستا صحیح
check_fd_is_path (ثابت نویسه *path, صحیح fd, ثابت نویسه *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  نویسه link[PATH_MAX];
  صحیح ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, اندازه (link)));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "readlink `%s`", fdname);

  اگر (((size_t) ret) != path_len || memcmp (link, path, path_len))
    بازگشت crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  بازگشت 0;
}

ایستا صحیح
check_fd_under_path (ثابت نویسه *rootfs, size_t rootfslen, صحیح fd, ثابت نویسه *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  نویسه link[PATH_MAX];
  صحیح ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  اگر (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    بازگشت 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, اندازه (link)));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "readlink `%s`", fdname);

  اگر (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    بازگشت crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  بازگشت 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
ایستا تهی
close_and_replace (صحیح *oldfd, صحیح newfd)
{
  اگر (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
نویسه *chroot_realpath (ثابت نویسه *chroot, ثابت نویسه *path, نویسه resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
ایستا صحیح
safe_openat_fallback (صحیح dirfd, ثابت نویسه *rootfs, ثابت نویسه *path, صحیح flags,
                      صحیح mode, libcrun_error_t *err)
{
  cleanup_free نویسه *parent_path = پوچ;
  ثابت نویسه *last_component = پوچ;
  ثابت نویسه *orig_path = path;
  ثابت نویسه *path_in_chroot;
  cleanup_close صحیح fd = -1;
  نویسه resolved[PATH_MAX];
  نویسه buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  صحیح ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  اگر (flags & O_NOFOLLOW)
    {
      نویسه *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      اگر (sep == پوچ)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      وگرنه اگر (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      اگر (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  اگر (path_in_chroot == پوچ)
    بازگشت crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  اگر (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  اگر (last_component)
    {
      ret = snprintf (resolved, اندازه (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      اگر (UNLIKELY (ret >= (صحیح) اندازه (resolved)))
        بازگشت crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  اگر (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      بازگشت ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = fd;
  fd = -1;
  بازگشت ret;
}

صحیح
safe_openat (صحیح dirfd, ثابت نویسه *rootfs, ثابت نویسه *path, صحیح flags, صحیح mode,
             libcrun_error_t *err)
{
  ایستا bool openat2_supported = true;
  صحیح retries = MAX_OPENAT2_EAGAIN_RETRIES;
  صحیح ret;

  اگر (is_empty_string (path))
    {
      cleanup_close صحیح fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      اگر (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      اگر (UNLIKELY (fd < 0))
        بازگشت crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      اگر (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      ret = fd;
      fd = -1;
      بازگشت ret;
    }

  اگر (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_ریشه);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (errno == EINTR)
            برو repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          اگر (errno == EAGAIN && retries-- > 0)
            برو repeat;
          اگر (errno == ENOSYS)
            openat2_supported = false;
          اگر (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            بازگشت safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          بازگشت crun_make_error (err, errno, "openat2 `%s`", path);
        }

      بازگشت ret;
    }

  بازگشت safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (صحیح dfd, ثابت نویسه *name, نویسه **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free نویسه *tmp_buf = پوچ;
  ssize_t size;

  انجامبده
    {
      اگر (tmp_buf != پوچ)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      اگر (UNLIKELY (size < 0))
        بازگشت crun_make_error (err, errno, "readlink `%s`", name);
  } وقتی (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = پوچ;

  بازگشت size;
}

ایستا صحیح crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, صحیح dirfd, ثابت نویسه *dirpath,
                                ثابت نویسه *path, صحیح mode, صحیح max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
ایستا صحیح
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, صحیح dirfd, ثابت نویسه *dirpath, صحیح cwd,
                                 ثابت نویسه *npath, ثابت نویسه *cur, ثابت نویسه *rest,
                                 صحیح mode, صحیح max_readlinks, libcrun_error_t *err)
{
  cleanup_free نویسه *target = پوچ;
  cleanup_free نویسه *new_path = پوچ;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (صحیح) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  بازگشت crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

ایستا صحیح
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, صحیح dirfd, ثابت نویسه *dirpath,
                     ثابت نویسه *path, صحیح mode, صحیح max_readlinks, libcrun_error_t *err)
{
  cleanup_close صحیح wd_cleanup = -1;
  cleanup_free نویسه *npath = پوچ;
  bool last_component = false;
  size_t depth = 0;
  نویسه *cur;
  نویسه *it;
  صحیح cwd;
  صحیح ret;

  اگر (max_readlinks <= 0)
    بازگشت crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  اگر (*path == '\0')
    {
      اگر (do_open)
        بازگشت open (dirpath, O_CLOEXEC | O_PATH, 0);
      بازگشت 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  وقتی (cur)
    {
      اگر (it)
        *it = '\0';
      وگرنه
        last_component = true;

      اگر (cur[0] == '\0')
        بشکن;

      اگر (strcmp (cur, ".") == 0)
        برو next;
      وگرنه اگر (strcmp (cur, ".."))
        depth++;
      وگرنه
        {
          اگر (depth)
            depth--;
          وگرنه
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              برو next;
            }
        }

      اگر (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          اگر (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              اگر (errno == ELOOP)
                بازگشت crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, پوچ,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              اگر (ret < 0)
                بازگشت crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          اگر (do_open)
            بازگشت ret;

          close_and_replace (&wd_cleanup, ret);
          بازگشت 0;
        }

      ret = mkdirat (cwd, cur, mode);
      اگر (ret < 0)
        {
          mode_t st_mode;

          اگر (errno != EEXIST)
            بازگشت crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          اگر (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            بازگشت crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : پوچ, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      اگر (UNLIKELY (cwd < 0))
        بازگشت crun_error_wrap (err, "creating `/%s`", path);

      اگر (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, پوچ);
          اگر (UNLIKELY (ret < 0))
            {
              صحیح saved_errno = errno;

              close (cwd);
              بازگشت crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          اگر ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              بازگشت crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      اگر (it == پوچ)
        بشکن;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  اگر (do_open)
    {
      اگر (cwd == dirfd)
        {
          ret = dup (dirfd);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "dup `%s`", dirpath);
          بازگشت ret;
        }

      wd_cleanup = -1;
      بازگشت cwd;
    }

  بازگشت 0;
}

صحیح
crun_safe_create_and_open_ref_at (bool dir, صحیح dirfd, ثابت نویسه *dirpath, ثابت نویسه *path, صحیح mode, libcrun_error_t *err)
{
  صحیح ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  اگر (LIKELY (ret >= 0))
    بازگشت ret;

  crun_error_release (err);
  بازگشت crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

صحیح
crun_safe_ensure_directory_at (صحیح dirfd, ثابت نویسه *dirpath, ثابت نویسه *path, صحیح mode,
                               libcrun_error_t *err)
{
  بازگشت crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

صحیح
crun_safe_ensure_file_at (صحیح dirfd, ثابت نویسه *dirpath, ثابت نویسه *path, صحیح mode,
                          libcrun_error_t *err)
{
  بازگشت crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

صحیح
crun_ensure_directory (ثابت نویسه *path, صحیح mode, bool nofollow, libcrun_error_t *err)
{
  بازگشت crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

ایستا صحیح
get_file_size (صحیح fd, off_t *size)
{
  ساختار stat st;
  صحیح ret;
#ifdef HAVE_STATX
  ساختار statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS || errno == EINVAL)
        برو fallback;
      بازگشت ret;
    }
  *size = stx.stx_size;

  بازگشت ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  بازگشت ret;
}

صحیح
crun_dir_p_at (صحیح dirfd, ثابت نویسه *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  صحیح ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "stat `%s`", path);

  بازگشت S_ISDIR (mode) ? 1 : 0;
}

صحیح
crun_dir_p (ثابت نویسه *path, bool nofollow, libcrun_error_t *err)
{
  بازگشت crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
ایستا صحیح
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free نویسه *buffer = پوچ;
  size_t len;
  صحیح ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  اگر (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          بازگشت 0;
        }
      بازگشت ret;
    }

  بازگشت strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
ایستا bool
running_in_openvz ()
{
  اگر (access ("/proc/vz", F_OK) < 0)
    بازگشت false;

  بازگشت access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

صحیح
check_running_in_user_namespace (libcrun_error_t *err)
{
  ایستا atomic_int run_in_userns = -1;
  ساختار stat st;
  صحیح ret;

  ret = run_in_userns;
  اگر (ret >= 0)
    بازگشت ret;

  ret = stat ("/proc/self/ns/user", &st);
  اگر (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      اگر (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          بازگشت run_in_userns;
        }

      اگر (! running_in_openvz ())
        {
          run_in_userns = 1;
          بازگشت run_in_userns;
        }
    }
  وگرنه اگر (UNLIKELY (errno != ENOENT))
    بازگشت crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  run_in_userns = ret;
  بازگشت run_in_userns;
}

ایستا size_t
get_page_size ()
{
  ایستا atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  اگر (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  بازگشت (size_t) pagesize;
}

ایستا صحیح selinux_enabled = -1;
ایستا صحیح apparmor_enabled = -1;

صحیح
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free نویسه *out = پوچ;
  cleanup_close صحیح fd = -1;
  size_t len;
  صحیح ret;

  اگر (selinux_enabled >= 0)
    بازگشت selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  اگر (UNLIKELY (fd < 0))
    بازگشت fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  بازگشت selinux_enabled;
}

صحیح
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  صحیح size;
  نویسه buf[1];

  اگر (apparmor_enabled >= 0)
    بازگشت apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  اگر (fd == -1)
    {
      اگر (errno == ENOENT)
        {
          apparmor_enabled = 0;
          بازگشت 0;
        }

      بازگشت crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (اقرأ (fd, buf, اندازه (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  بازگشت apparmor_enabled;
}

ایستا صحیح
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  اگر (selinux_enabled < 0)
    بازگشت crun_make_error (err, 0, "SELinux is not initialized correctly");
  بازگشت selinux_enabled;
}

صحیح
add_selinux_mount_label (نویسه **retlabel, ثابت نویسه *data, ثابت نویسه *label, ثابت نویسه *context_type, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_is_selinux_enabled (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (label && ret)
    {
      اگر (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      وگرنه
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      بازگشت 0;
    }
  *retlabel = xstrdup (data);
  بازگشت 0;
}

ایستا ثابت نویسه *
lsm_attr_path (libcrun_container_t *container, ثابت نویسه *lsm, ثابت نویسه *fname, libcrun_error_t *err)
{
  cleanup_close صحیح attr_dirfd = -1;
  cleanup_close صحیح lsm_dirfd = -1;
  نویسه *attr_path = پوچ;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  اگر (UNLIKELY (attr_dirfd < 0))
    بازگشت پوچ;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  اگر (lsm != پوچ)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      اگر (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          بازگشت پوچ;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  بازگشت attr_path;
}

ایستا صحیح
check_proc_super_magic (صحیح fd, ثابت نویسه *path, libcrun_error_t *err)
{
  ساختار statfs sfs;

  صحیح ret = fstatfs (fd, &sfs);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "statfs `%s`", path);

  اگر (sfs.f_type != PROC_SUPER_MAGIC)
    بازگشت crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  بازگشت 0;
}

ایستا صحیح
set_security_attr (libcrun_container_t *container, ثابت نویسه *lsm, ثابت نویسه *fname, ثابت نویسه *data, libcrun_error_t *err)
{
  صحیح ret;

  cleanup_free ثابت نویسه *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close صحیح fd = -1;

  اگر (UNLIKELY (attr_path == پوچ))
    بازگشت -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  اگر (UNLIKELY (fd < 0))
    بازگشت fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (اكتب (fd, data, strlen (data)));
  اگر (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      اگر (errno == ENOENT && lsm != پوچ && strcmp (lsm, "apparmor") == 0)
        {
          ثابت نویسه *profile = strchr (data, ' ');

          بازگشت crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      بازگشت crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  بازگشت 0;
}

صحیح
set_selinux_label (libcrun_container_t *container, ثابت نویسه *label, bool now, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_is_selinux_enabled (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (ret)
    بازگشت set_security_attr (container, پوچ, now ? "current" : "exec", label, err);
  بازگشت 0;
}

ایستا صحیح
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  اگر (apparmor_enabled < 0)
    بازگشت crun_make_error (err, 0, "AppArmor is not initialized correctly");
  بازگشت apparmor_enabled;
}

ایستا صحیح
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (اندازه (UNCONFINED) - 1)
  cleanup_free ثابت نویسه *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close صحیح fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  نویسه buf[UNCONFINED_LEN + 1];

  اگر (UNLIKELY (attr_path == پوچ))
    بازگشت -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  اگر (UNLIKELY (fd < 0))
    بازگشت fd;

  اگر (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    بازگشت -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (اقرأ (fd, buf, اندازه (buf)));
  اگر (UNLIKELY (bytes_read < 0))
    بازگشت crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  اگر (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    بازگشت 1;
  اگر (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    بازگشت 0;
  بازگشت 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

صحیح
set_apparmor_profile (libcrun_container_t *container, ثابت نویسه *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_is_apparmor_enabled (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (ret)
    {
      cleanup_free نویسه *buf = پوچ;
      ret = is_current_process_confined (container, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      بازگشت set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  بازگشت 0;
}

ایستا صحیح
read_all_fd_internal (صحیح fd, ثابت نویسه *description, نویسه **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free نویسه *buf = پوچ;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  صحیح ret;

  اگر (size_hint)
    allocated = size_hint;
  وگرنه
    {
      ret = get_file_size (fd, &size);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "stat `%s`", description);

      اگر (max_size && (size_t) size > max_size)
        بازگشت crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  وقتی ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (اقرأ (fd, buf + nread, allocated - nread));
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "read from file `%s`", description);

      اگر (ret == 0)
        بشکن;

      nread += ret;

      اگر (max_size && nread > max_size)
        بازگشت crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      اگر (nread == allocated)
        {
          اگر (size)
            بشکن;

          اگر (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  اگر (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      نویسه *tmp = realloc (buf, nread + 1);
      اگر (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = پوچ;
  اگر (len)
    *len = nread;
  بازگشت 0;
}

صحیح
read_all_fd_with_size_hint (صحیح fd, ثابت نویسه *description, نویسه **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  بازگشت read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

صحیح
read_all_fd_with_max_size (صحیح fd, ثابت نویسه *description, نویسه **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  بازگشت read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

صحیح
read_all_file_at (صحیح dirfd, ثابت نویسه *path, نویسه **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", path);

  بازگشت read_all_fd (fd, path, out, len, err);
}

صحیح
read_all_file (ثابت نویسه *path, نویسه **out, size_t *len, libcrun_error_t *err)
{
  اگر (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  بازگشت read_all_file_at (AT_FDCWD, path, out, len, err);
}

صحیح
get_realpath_to_file (صحیح dirfd, ثابت نویسه *path_name, نویسه **absolute_path, libcrun_error_t *err)
{
  cleanup_close صحیح targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  اگر (UNLIKELY (targetfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", path_name);
  وگرنه
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      اگر (UNLIKELY (len < 0))
        بازگشت crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  بازگشت 0;
}

صحیح
open_unix_domain_client_socket (ثابت نویسه *path, صحیح dgram, libcrun_error_t *err)
{
  ساختار sockaddr_un addr = {};
  صحیح ret;
  proc_fd_path_t name_buf;
  cleanup_close صحیح destfd = -1;
  cleanup_close صحیح fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "create UNIX socket");

  اگر (strlen (path) >= اندازه (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      اگر (UNLIKELY (destfd < 0))
        بازگشت crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  اگر (path_len >= اندازه (addr.sun_path))
    بازگشت crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (ساختار sockaddr *) &addr, اندازه (addr));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  بازگشت ret;
}

صحیح
open_unix_domain_socket (ثابت نویسه *path, صحیح dgram, libcrun_error_t *err)
{
  ساختار sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  صحیح ret;
  cleanup_close صحیح dirfd = -1;
  cleanup_close صحیح fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "create UNIX socket");

  اگر (strlen (path) >= اندازه (addr.sun_path))
    {
      cleanup_free نویسه *dpath = xstrdup (path);
      cleanup_free نویسه *bpath = xstrdup (path);
      ثابت نویسه *parent_dir = dirname (dpath);
      ثابت نویسه *base = basename (bpath);
      صحیح n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      اگر (UNLIKELY (dirfd < 0))
        بازگشت crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, اندازه (addr.sun_path), "%s/%s", name_buf, base);
      اگر (n < 0 || (size_t) n >= اندازه (addr.sun_path))
        بازگشت crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  وگرنه
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (ساختار sockaddr *) &addr, اندازه (addr));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "bind socket to `%s`", path);

  اگر (! dgram)
    {
      ret = listen (fd, 1);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  بازگشت ret;
}

صحیح
send_fd_to_socket (صحیح server, صحیح fd, libcrun_error_t *err)
{
  بازگشت send_fd_to_socket_with_payload (server, fd, پوچ, 0, err);
}

صحیح
send_fd_to_socket_with_payload (صحیح server, صحیح fd, ثابت نویسه *payload, size_t payload_len, libcrun_error_t *err)
{
  صحیح ret;
  ساختار cmsghdr *cmsg = پوچ;
  ساختار iovec iov[2];
  ساختار msghdr پیام = {};
  نویسه ctrl_buf[CMSG_SPACE (1 + اندازه (صحیح))] = {};
  نویسه data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = اندازه (data);

  اگر (payload_len > 0)
    {
      iov[0].iov_base = (تهی *) payload;
      iov[0].iov_len = payload_len;
    }

  پیام.پیام_name = پوچ;
  پیام.پیام_namelen = 0;
  پیام.پیام_iov = iov;
  پیام.پیام_iovlen = 1;
  پیام.پیام_controllen = CMSG_SPACE (اندازه (صحیح));
  پیام.پیام_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&پیام);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (اندازه (صحیح));

  *((صحیح *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &پیام, 0));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "sendmsg");
  بازگشت 0;
}

صحیح
receive_fd_from_socket_with_payload (صحیح from, نویسه *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  صحیح ret;
  ساختار iovec iov[1];
  ساختار msghdr پیام = {};
  نویسه ctrl_buf[CMSG_SPACE (اندازه (صحیح))] = {};
  نویسه data[1];
  ساختار cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = اندازه (data);

  اگر (payload_len > 0)
    {
      iov[0].iov_base = (تهی *) payload;
      iov[0].iov_len = payload_len;
    }

  پیام.پیام_name = پوچ;
  پیام.پیام_namelen = 0;
  پیام.پیام_iov = iov;
  پیام.پیام_iovlen = 1;
  پیام.پیام_controllen = CMSG_SPACE (اندازه (صحیح));
  پیام.پیام_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &پیام, 0));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "recvmsg");
  اگر (UNLIKELY (ret == 0))
    بازگشت crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&پیام);
  اگر (cmsg == پوچ)
    بازگشت crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), اندازه (fd));

  ret = fd;
  fd = -1;
  بازگشت ret;
}

صحیح
receive_fd_from_socket (صحیح from, libcrun_error_t *err)
{
  بازگشت receive_fd_from_socket_with_payload (from, پوچ, 0, err);
}

صحیح
create_socket_pair (صحیح *pair, libcrun_error_t *err)
{
  صحیح ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "socketpair");
  بازگشت 0;
}

صحیح
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  صحیح ret = signalfd (-1, mask, 0);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "signalfd");
  بازگشت ret;
}

ایستا صحیح
epoll_helper_toggle (صحیح epollfd, صحیح fd, صحیح events, libcrun_error_t *err)
{
  ساختار epoll_event ev = {};
  bool add = events != 0;
  صحیح ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  اگر (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  اگر (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      اگر (errno == ENOENT || errno == EPERM)
        بازگشت 0;
      بازگشت crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  بازگشت 0;
}

صحیح
epoll_helper (صحیح *in_fds, صحیح *in_levelfds, صحیح *out_fds, صحیح *out_levelfds, libcrun_error_t *err)
{
  ساختار epoll_event ev;
  cleanup_close صحیح epollfd = -1;
  صحیح ret;

  صحیح *it;
  epollfd = epoll_create1 (0);
  اگر (UNLIKELY (epollfd < 0))
    بازگشت crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  برای (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      اگر (UNLIKELY (ret < 0))                                           \
        بازگشت crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  اگر (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  اگر (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  اگر (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  اگر (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  بازگشت ret;
}

صحیح
copy_from_fd_to_fd (صحیح src, صحیح dst, صحیح consume, libcrun_error_t *err)
{
  صحیح ret;
  ssize_t nread;
  size_t pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  انجامبده
    {
      cleanup_free نویسه *buffer = پوچ;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      اگر (can_copy_file_range)
        {
          nread = copy_file_range (src, پوچ, dst, پوچ, pagesize, 0);
          اگر (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              برو fallback;
            }
          اگر (consume && nread < 0 && errno == EAGAIN)
            بازگشت 0;
          اگر (nread < 0 && errno == EIO)
            بازگشت 0;
          اگر (UNLIKELY (nread < 0))
            بازگشت crun_make_error (err, errno, "copy_file_range");
          ادامه;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (اقرأ (src, buffer, pagesize));
      اگر (consume && nread < 0 && errno == EAGAIN)
        بازگشت 0;
      اگر (nread < 0 && errno == EIO)
        بازگشت 0;
      اگر (UNLIKELY (nread < 0))
        بازگشت crun_make_error (err, errno, "read");

      remaining = nread;
      وقتی (remaining)
        {
          ret = TEMP_FAILURE_RETRY (اكتب (dst, buffer + nread - remaining, remaining));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } وقتی (consume && nread);

  بازگشت 0;
}

صحیح
run_process (نویسه **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  اگر (UNLIKELY (pid < 0))
    بازگشت crun_make_error (err, errno, "fork");
  اگر (pid)
    {
      صحیح r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      اگر (r < 0)
        بازگشت crun_make_error (err, errno, "waitpid");
      اگر (WIFEXITED (status) || WIFSIGNALED (status))
        بازگشت WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
ایستا unsigned
atou (نویسه **s)
{
  unsigned x;
  برای (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  بازگشت x;
}

صحیح
fgetpwent_r (FILE *f, ساختار passwd *pw, نویسه *سطر, size_t size, ساختار passwd **res)
{
  نویسه *s;
  صحیح rv = 0;
  برای (;;)
    {
      سطر[size - 1] = '\xff';
      اگر ((fgets (سطر, size, f) == پوچ) || ferror (f) || سطر[size - 1] != '\xff')
        {
          rv = (سطر[size - 1] != '\xff') ? ERANGE : ENOENT;
          سطر = 0;
          pw = 0;
          بشکن;
        }
      سطر[strcspn (سطر, "\n")] = 0;

      s = سطر;
      pw->pw_name = s++;
      اگر (! (s = strchr (s, ':')))
        ادامه;

      *s++ = 0;
      pw->pw_passwd = s;
      اگر (! (s = strchr (s, ':')))
        ادامه;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      اگر (*s != ':')
        ادامه;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      اگر (*s != ':')
        ادامه;

      *s++ = 0;
      pw->pw_gecos = s;
      اگر (! (s = strchr (s, ':')))
        ادامه;

      *s++ = 0;
      pw->pw_dir = s;
      اگر (! (s = strchr (s, ':')))
        ادامه;

      *s++ = 0;
      pw->pw_shell = s;
      بشکن;
    }
  *res = pw;
  اگر (rv)
    errno = rv;
  بازگشت rv;
}
#endif

صحیح
set_home_env (uid_t id)
{
  ساختار passwd pwd;
  cleanup_free نویسه *buf = پوچ;
  long buf_size;
  cleanup_file FILE *stream = پوچ;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  اگر (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  اگر (stream == پوچ)
    بازگشت -1;

  برای (;;)
    {
      ساختار passwd *ret_pw = پوچ;

      صحیح ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      اگر (UNLIKELY (ret != 0))
        {
          اگر (ret == EINTR)
            ادامه;

          اگر (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              بازگشت -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          ادامه;
        }

      اگر (ret_pw && ret_pw->pw_uid == id)
        {
          اگر (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          وگرنه
            بازگشت 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
ایستا صحیح
getsubidrange (uid_t id, صحیح is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *درون_داد = پوچ;
  cleanup_free نویسه *lineptr = پوچ;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free نویسه *buf = پوچ;
  ثابت نویسه *name;
  ساختار passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  اگر (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  برای (;;)
    {
      صحیح ret;
      ساختار passwd *ret_pw = پوچ;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      اگر (LIKELY (ret == 0))
        {
          اگر (ret_pw)
            {
              name = ret_pw->pw_name;
              بشکن;
            }
          بازگشت -1;
        }

      اگر (ret != ERANGE)
        بازگشت -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  درون_داد = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  اگر (درون_داد == پوچ)
    بازگشت -1;

  برای (;;)
    {
      نویسه *endptr;
      ssize_t اقرأ = getline (&lineptr, &lenlineptr, درون_داد);
      اگر (اقرأ < 0)
        بازگشت -1;

      اگر (اقرأ < (ssize_t) (len_name + 2))
        ادامه;

      اگر (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        ادامه;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      اگر (endptr >= &lineptr[اقرأ])
        بازگشت -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      بازگشت 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

صحیح
format_default_id_mapping (نویسه **out, uid_t container_id, uid_t host_uid, uid_t host_id, صحیح is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free نویسه *buffer = پوچ;
  صحیح written = 0;
  صحیح ret, remaining;

  *out = پوچ;

  اگر (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    بازگشت 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  اگر (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      اگر (UNLIKELY (ret >= remaining))
        بازگشت crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  اگر (UNLIKELY (ret >= remaining))
    بازگشت crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  اگر (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      اگر (UNLIKELY (ret >= remaining))
        بازگشت crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = پوچ;
  بازگشت written;
}

ایستا صحیح
unset_cloexec_flag (صحیح fd)
{
  صحیح flags = fcntl (fd, F_GETFD);
  اگر (flags == -1)
    بازگشت -1;

  flags &= ~FD_CLOEXEC;

  بازگشت fcntl (fd, F_SETFD, flags);
}

ایستا تهی __attribute__ ((__noreturn__))
run_process_child (نویسه *path, نویسه **args, ثابت نویسه *cwd, نویسه **envp, صحیح pipe_r,
                   صحیح pipe_w, صحیح out_fd, صحیح err_fd, bool can_ignore_chdir_errors)
{
  نویسه *tmp_args[] = { path, پوچ };
  libcrun_error_t err = پوچ;
  صحیح dev_null_fd = -1;
  صحیح ret;

  ret = mark_or_close_fds_ge_than (پوچ, 3, false, &err);
  اگر (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->پیام);

  اگر (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      اگر (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  اگر (out_fd >= 0)
    unset_cloexec_flag (1);
  اگر (err_fd >= 0)
    unset_cloexec_flag (2);

  اگر (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  اگر (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  اگر (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  اگر (args == پوچ)
    args = tmp_args;

  اگر (cwd && chdir (cwd) < 0)
    {
      اگر (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
صحیح
run_process_with_stdin_timeout_envp (نویسه *path, نویسه **args, ثابت نویسه *cwd, صحیح timeout,
                                     نویسه **envp, نویسه *stdin, size_t stdin_len, صحیح out_fd,
                                     صحیح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  صحیح stdin_pipe[2];
  pid_t pid;
  صحیح ret;
  cleanup_close صحیح pipe_r = -1;
  cleanup_close صحیح pipe_w = -1;
  sigset_t oldmask, mask;
  صحیح r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  اگر (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  اگر (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      برو restore_sig_mask_and_exit;
    }

  اگر (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (اكتب (pipe_w, stdin, stdin_len));
  اگر (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      اگر (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          برو restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  اگر (timeout)
    {
      time_t start = time (پوچ);
      time_t now;
      برای (now = start; now - start < timeout; now = time (پوچ))
        {
          siginfo_t info;
          صحیح elapsed = now - start;
          ساختار timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          اگر (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              برو restore_sig_mask_and_exit;
            }

          اگر (info.si_signo == SIGCHLD && info.si_pid == pid)
            برو read_waitpid;

          اگر (ret < 0 && errno == EAGAIN)
            برو timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      برو restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  اگر (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  وگرنه
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  اگر (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      اگر (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, پوچ);
      اگر (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  بازگشت ret;
}

صحیح
mark_or_close_fds_ge_than (libcrun_container_t *container, صحیح n, bool close_now, libcrun_error_t *err)
{
  cleanup_close صحیح cfd = -1;
  cleanup_dir DIR *dir = پوچ;
  صحیح ret;
  صحیح fd;
  ساختار dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  اگر (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  اگر (ret == 0)
    {
      اگر (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      بازگشت 0;
    }
  اگر (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    بازگشت crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  اگر (UNLIKELY (cfd < 0))
    بازگشت cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  dir = fdopendir (cfd);
  اگر (UNLIKELY (dir == پوچ))
    بازگشت crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  برای (next = readdir (dir); next; next = readdir (dir))
    {
      صحیح val;
      ثابت نویسه *name = next->d_name;
      اگر (name[0] == '.')
        ادامه;

      val = strtoll (name, پوچ, 10);
      اگر (val < n || val == fd)
        ادامه;

      اگر (close_now)
        {
          ret = close (val);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "close fd `%d`", val);

          اگر (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      وگرنه
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  بازگشت 0;
}

تهی
get_current_timestamp (نویسه *out, size_t len)
{
  ساختار timeval tv;
  ساختار tm now;
  نویسه timestamp[64];

  gettimeofday (&tv, پوچ);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, اندازه (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (تهی) snprintf (out, len, "%s.%06lldZ", timestamp, (long long صحیح) tv.tv_usec);
  out[len - 1] = '\0';
}

صحیح
set_blocking_fd (صحیح fd, bool blocking, libcrun_error_t *err)
{
  صحیح ret, flags = fcntl (fd, F_GETFL, 0);
  اگر (UNLIKELY (flags < 0))
    بازگشت crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fcntl");
  بازگشت 0;
}

صحیح
parse_json_file (json_object **out, ثابت نویسه *jsondata, ساختار parser_context *ctx arg_unused, libcrun_error_t *err)
{
  ساختار json_tokener *tok;
  شمارش json_tokener_error jerr;

  *err = پوچ;

  اگر (UNLIKELY (jsondata == پوچ))
    بازگشت crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  اگر (UNLIKELY (tok == پوچ))
    بازگشت crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (صحیح) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  اگر (*out == پوچ || jerr != json_tokener_success)
    {
      صحیح saved_pos = (صحیح) tok->char_offset;

      json_tokener_free (tok);
      اگر (*out)
        {
          json_object_put (*out);
          *out = پوچ;
        }
      بازگشت crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  بازگشت 0;
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
ایستا صحیح
check_access (ثابت نویسه *path)
{
  صحیح ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#وگرنه
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  اگر (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      اگر (errno == EACCES)
        {
          صحیح saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          اگر (ret == 0)
            بازگشت CHECK_ACCESS_NOT_EXECUTABLE;
        }
      بازگشت -errno;
    }

  ret = get_file_type (&mode, false, path);
  اگر (UNLIKELY (ret < 0))
    بازگشت -errno;

  اگر (! S_ISREG (mode))
    بازگشت CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  بازگشت 0;
}

صحیح
find_executable (نویسه **out, ثابت نویسه *executable_path, ثابت نویسه *cwd, libcrun_error_t *err)
{
  cleanup_free نویسه *cwd_executable_path = پوچ;
  cleanup_free نویسه *tmp = پوچ;
  صحیح last_error = -ENOENT;
  نویسه *it, *end;
  صحیح ret;

  *out = پوچ;

  اگر (executable_path == پوچ || executable_path[0] == '\0')
    بازگشت crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  اگر (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free نویسه *cwd_allocated = پوچ;

      اگر (cwd == پوچ)
        {
          cwd_allocated = getcwd (پوچ, 0);
          اگر (cwd_allocated == پوچ)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  اگر (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      اگر (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          بازگشت 0;
        }
      last_error = ret;
      برو fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  وقتی ((it = strsep (&end, ":")))
    {
      cleanup_free نویسه *path = پوچ;

      اگر (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, پوچ);
      اگر (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ادامه;
        }

      ret = check_access (path);
      اگر (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = پوچ;
          بازگشت 0;
        }

      اگر (ret == -ENOENT)
        ادامه;

      last_error = ret;
    }

fail:
  انتخاب (last_error)
    {
    حالت CHECK_ACCESS_NOT_EXECUTABLE:
      بازگشت crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    حالت CHECK_ACCESS_NOT_REGULAR:
      بازگشت crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    پیشفرض:
      errno = -last_error;
      اگر (errno == ENOENT)
        بازگشت crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      بازگشت crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

ایستا ssize_t
safe_read_xattr (نویسه **ret, صحیح sfd, ثابت نویسه *srcname, ثابت نویسه *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free نویسه *buffer = پوچ;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  وقتی (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      اگر (UNLIKELY (s < 0))
        بازگشت crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      اگر (s < current_size)
        بشکن;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  اگر (s <= 0)
    بازگشت s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = پوچ;

  بازگشت s;
}

ایستا ssize_t
copy_xattr (صحیح sfd, صحیح dfd, ثابت نویسه *srcname, ثابت نویسه *destname, libcrun_error_t *err)
{
  cleanup_free نویسه *buf = پوچ;
  ssize_t xattr_len;
  نویسه *it;

  xattr_len = flistxattr (sfd, پوچ, 0);
  اگر (UNLIKELY (xattr_len < 0))
    {
      اگر (errno == ENOTSUP)
        بازگشت 0;

      بازگشت crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  اگر (xattr_len == 0)
    بازگشت 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  اگر (UNLIKELY (xattr_len < 0))
    بازگشت crun_make_error (err, errno, "flistxattr `%s`", srcname);

  برای (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free نویسه *v = پوچ;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      اگر (UNLIKELY (s < 0))
        بازگشت s;

      s = fsetxattr (dfd, it, v, s, 0);
      اگر (UNLIKELY (s < 0))
        {
          اگر (errno == EINVAL || errno == EOPNOTSUPP)
            ادامه;

          بازگشت crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  بازگشت 0;
}

#endif

ایستا صحیح
copy_rec_stat_file_at (صحیح dfd, ثابت نویسه *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  ساختار stat st;
  صحیح ret;

#ifdef HAVE_STATX
  ساختار statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS || errno == EINVAL)
        برو fallback;

      بازگشت ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  بازگشت ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  بازگشت ret;
}

صحیح
copy_recursive_fd_to_fd (صحیح srcdirfd, صحیح dfd, ثابت نویسه *srcname, ثابت نویسه *destname, libcrun_error_t *err)
{
  cleanup_close صحیح destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = پوچ;
  ساختار dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  اگر (UNLIKELY (dsrcfd == پوچ))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      بازگشت crun_make_error (err, errno, "open directory `%s`", destname);
    }

  برای (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close صحیح srcfd = -1;
      cleanup_close صحیح destfd = -1;
      cleanup_free نویسه *target_buf = پوچ;
      صحیح ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      اگر (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ادامه;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      انتخاب (mode & S_IFMT)
        {
        حالت S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          اگر (UNLIKELY (srcfd < 0))
            بازگشت crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          اگر (UNLIKELY (destfd < 0))
            بازگشت crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

#ifdef HAVE_FGETXATTR
          ret = (صحیح) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          بشکن;

        حالت S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          اگر (UNLIKELY (srcfd < 0))
            بازگشت crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          اگر (UNLIKELY (destfd < 0))
            بازگشت crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (صحیح) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          بشکن;

        حالت S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          بشکن;

        حالت S_IFBLK:
        حالت S_IFCHR:
        حالت S_IFIFO:
        حالت S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          بشکن;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      اگر (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          اگر (errno == ENOTSUP)
            ادامه;

          بازگشت crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  بازگشت 0;
}

ثابت نویسه *
find_annotation (libcrun_container_t *container, ثابت نویسه *name)
{
  اگر (container->container_def->annotations == پوچ)
    بازگشت پوچ;

  بازگشت find_string_map_مقدار (container->annotations, name);
}

صحیح
safe_write (صحیح fd, ثابت نویسه *fname, ثابت تهی *buf, size_t شمار, libcrun_error_t *err)
{
  size_t written = 0;
  وقتی (written < شمار)
    {
      ssize_t w = اكتب (fd, (ثابت نویسه *) buf + written, شمار - written);
      اگر (UNLIKELY (w < 0))
        {
          اگر (errno == EINTR || errno == EAGAIN)
            ادامه;
          بازگشت crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  بازگشت 0;
}

#define MAX_PARTS 32

صحیح
append_paths (نویسه **out, libcrun_error_t *err, ...)
{
  ثابت نویسه *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t total_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  برای (;;)
    {
      ثابت نویسه *part;
      size_t size;

      part = va_arg (ap, ثابت نویسه *);
      اگر (part == پوچ)
        بشکن;

      اگر (n_parts == MAX_PARTS)
        {
          va_end (ap);
          بازگشت crun_make_error (err, EINVAL, "too many paths specified");
        }

      اگر (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          وقتی (part[0] == '/' && part[1] == '/')
            part++;
        }
      وگرنه
        {
          /* And drop any initial '/' for other components.  */
          وقتی (part[0] == '/')
            part++;
        }

      size = strlen (part);
      اگر (size == 0)
        ادامه;

      وقتی (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  برای (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  برای (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      اگر (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  بازگشت 0;
}

#اگر __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#وگرنه
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
ایستا صحیح
b64_درون_داد (نویسه c)
{
  ثابت نویسه table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  صحیح i;

  برای (i = 0; i < 64; i++)
    {
      اگر (table[i] == c)
        بازگشت i;
    }
  بازگشت -1;
}

صحیح
base64_decode (ثابت نویسه *iptr, size_t isize, نویسه *optr, size_t osize, size_t *nbytes)
{
  صحیح i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned نویسه data[4];

  *nbytes = 0;
  وقتی (consumed < isize && (*nbytes) + 3 < osize)
    {
      وقتی ((i < 4) && (consumed < isize))
        {
          tmp = b64_درون_داد (*iptr++);
          consumed++;
          اگر (tmp != -1)
            {
              اگر (UNLIKELY (pad > 0))
                بازگشت -1;
              data[i++] = tmp;
            }
          وگرنه اگر (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      اگر (i == 4)
        {
          اگر (UNLIKELY (pad > 2))
            بازگشت -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      وگرنه
        {
          /* I did not get all the data.  */
          consumed -= i;
          بازگشت consumed;
        }
      i = 0;
    }
  بازگشت consumed;
}

نویسه *
get_user_name (uid_t uid)
{
  ساختار passwd pd;
  ساختار passwd *temp_result_ptr;
  نویسه pwdbuffer[200];
  اگر (! getpwuid_r (uid, &pd, pwdbuffer, اندازه (pwdbuffer), &temp_result_ptr))
    بازگشت xstrdup (pd.pw_name);
  بازگشت xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
نویسه *
libcrun_sysctl_کلید_to_proc_path (ثابت نویسه *کلید)
{
  ثابت نویسه *sep;
  نویسه *ret, *it;

  sep = strpbrk (کلید, "./");

  /* No separator at all, or the key is already in the path form.  */
  اگر (sep == پوچ || *sep == '/')
    بازگشت xstrdup (کلید);

  ret = xstrdup (کلید);
  برای (it = ret; *it; it++)
    {
      اگر (*it == '.')
        *it = '/';
      وگرنه اگر (*it == '/')
        *it = '.';
    }

  بازگشت ret;
}

صحیح
has_suffix (ثابت نویسه *str, ثابت نویسه *suffix)
{
  اگر (! str || ! suffix)
    بازگشت 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  اگر (lensuffix > lenstr)
    بازگشت 0;
  بازگشت memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

نویسه *
str_join_array (صحیح offset, size_t size, نویسه *ثابت array[], ثابت نویسه *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = پوچ;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  نویسه *result, *p;

  lens = xmalloc (size * اندازه (*lens));

  برای (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  برای (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      اگر (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  بازگشت result;
}

صحیح
libcrun_mmap (ساختار libcrun_mmap_s **ret, تهی *addr, size_t length,
              صحیح prot, صحیح flags, صحیح fd, off_t offset,
              libcrun_error_t *err)
{
  ساختار libcrun_mmap_s *mmap_s = پوچ;

  تهی *mapped = mmap (addr, length, prot, flags, fd, offset);
  اگر (mapped == MAP_FAILED)
    بازگشت crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (اندازه (ساختار libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  بازگشت 0;
}

صحیح
libcrun_munmap (ساختار libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  صحیح ret;

  ret = munmap (mmap->addr, mmap->length);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "munmap");

  آزاد (mmap);

  بازگشت 0;
}

ایستا long
read_file_as_long_or_default (ثابت نویسه *path, long def_مقدار)
{
  cleanup_free نویسه *content = پوچ;
  libcrun_error_t tmp_err = پوچ;
  نویسه *endptr;
  long val;
  صحیح ret;

  ret = read_all_file (path, &content, پوچ, &tmp_err);
  اگر (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      بازگشت def_مقدار;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  اگر (UNLIKELY (errno))
    بازگشت def_مقدار;
  اگر (endptr == content || (*endptr && *endptr != '\n'))
    بازگشت def_مقدار;
  بازگشت val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (تهی)
{
  ایستا atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  اگر (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  بازگشت uid;
}

gid_t
get_overflow_gid (تهی)
{
  ایستا atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  اگر (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  بازگشت gid;
}

تهی
consume_trailing_slashes (نویسه *path)
{
  اگر (! path || path[0] == '\0')
    بازگشت;

  نویسه *last = path + strlen (path);

  وقتی (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

نویسه **
read_dir_entries (ثابت نویسه *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = پوچ;
  size_t n_entries = 0;
  size_t entries_size = 16;
  نویسه **entries = پوچ;
  ساختار dirent *de;

  dir = opendir (path);
  اگر (UNLIKELY (dir == پوچ))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      بازگشت پوچ;
    }

  entries = xmalloc (entries_size * اندازه (نویسه *));
  وقتی ((de = readdir (dir)))
    {
      اگر (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ادامه;
      اگر (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * اندازه (نویسه *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * اندازه (نویسه *));
  entries[n_entries] = پوچ;

  بازگشت entries;
}

صحیح
cpuset_string_to_bitmask (ثابت نویسه *str, نویسه **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free نویسه *mask = پوچ;
  size_t mask_size = 0;
  ثابت نویسه *p = str;
  نویسه *endptr;

  وقتی (*p)
    {
      long long start_range, end_range;

      اگر (*p < '0' || *p > '9')
        برو invalid_درون_داد;

      start_range = strtoll (p, &endptr, 10);
      اگر (start_range < 0)
        برو invalid_درون_داد;

      p = endptr;

      اگر (*p != '-')
        end_range = start_range;
      وگرنه
        {
          p++;

          اگر (*p < '0' || *p > '9')
            برو invalid_درون_داد;

          end_range = strtoll (p, &endptr, 10);

          اگر (end_range < start_range)
            برو invalid_درون_داد;

          p = endptr;
        }

      /* Just set some limit.  */
      اگر (end_range > (1 << 20))
        برو invalid_درون_داد;

      اگر (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      برای (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      اگر (*p == ',')
        p++;
      وگرنه اگر (*p)
        برو invalid_درون_داد;
    }

  *out = mask;
  mask = پوچ;
  *out_size = mask_size;

  بازگشت 0;

invalid_درون_داد:
  بازگشت crun_make_error (err, 0, "cannot parse input `%s`", str);
}

ساختار channel_fd_pair
{
  ساختار ring_buffer *rb;

  صحیح in_fd;
  صحیح out_fd;

  صحیح infd_epoll_events;
  صحیح outfd_epoll_events;
};

ساختار channel_fd_pair *
channel_fd_pair_new (صحیح in_fd, صحیح out_fd, size_t size)
{
  ساختار channel_fd_pair *channel = xmalloc (اندازه (ساختار channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  بازگشت channel;
}

تهی
channel_fd_pair_free (ساختار channel_fd_pair *channel)
{
  اگر (channel == پوچ)
    بازگشت;

  ring_buffer_free (channel->rb);
  آزاد (channel);
}

صحیح
channel_fd_pair_process (ساختار channel_fd_pair *channel, صحیح epollfd, libcrun_error_t *err)
{
  bool is_درون_داد_eagain = false, is_output_eagain = false, repeat;
  صحیح ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  برای (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      اگر (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_درون_داد_eagain, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          اگر (ret > 0)
            repeat = true;
        }
      اگر (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          اگر (ret > 0)
            repeat = true;
        }
    }

  اگر (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      صحیح events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_درون_داد_eagain ? EPOLLET : 0)) : 0;
      اگر (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      اگر (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          channel->outfd_epoll_events = events;
        }
    }
  بازگشت 0;
}

ایستا صحیح
open_proc (libcrun_error_t *err)
{
  صحیح fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close صحیح fsfd = -1;
  صحیح ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  اگر (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, پوچ, پوچ, 0);
      اگر (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          اگر (fd >= 0)
            بازگشت fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  اگر (fd < 0)
    بازگشت crun_make_error (err, errno, "open `/proc`");
  بازگشت fd;
}

صحیح
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  اگر (! container)
    بازگشت open_proc (err);

  اگر (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  بازگشت container->proc_fd;
}

صحیح
libcrun_open_proc_file (libcrun_container_t *container, ثابت نویسه *path, صحیح flags, libcrun_error_t *err)
{
  صحیح proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  اگر (proc_fd < 0)
    بازگشت proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  اگر (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  بازگشت fd;
}

صحیح
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ثابت نویسه *path, صحیح flags, libcrun_error_t *err)
{
  cleanup_free نویسه *full_path = پوچ;
  xasprintf (&full_path, "%d/%s", pid, path);
  بازگشت libcrun_open_proc_file (container, full_path, flags, err);
}
