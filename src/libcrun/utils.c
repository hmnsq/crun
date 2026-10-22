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
#ضمن <config.h>
#ضمن "utils.h"
#ضمن "ring_buffer.h"
#ضمن <stdarg.h>
#ضمن <inttypes.h>
#ضمن <unistd.h>
#ضمن <string.h>
#ضمن <libgen.h>
#ضمن <fcntl.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <sys/un.h>
#ضمن <sys/socket.h>
#ضمن <sys/signalfd.h>
#ضمن <sys/epoll.h>
#ضمن <sys/syscall.h>
#ضمن <sys/wait.h>
#ضمن <pwd.h>
#ضمن <grp.h>
#ضمن <time.h>
#ضمن <sys/time.h>
#ضمن <sys/xattr.h>
#ضمن <sys/sysmacros.h>
#ضمن <sys/vfs.h>
#ضمن <linux/magic.h>
#ضمن <limits.h>
#ضمن <sys/mman.h>
#ifdef HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#endif
#إذا HAVE_STDATOMIC_H
#  include <stdatomic.h>
#وإلا
#  define atomic_long متطاير long
#  define atomic_int متطاير صحيح
#endif
#ضمن "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_جذر
#  define RESOLVE_IN_جذر 0x10
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

صحيح
crun_path_exists (ثابت حرف *path, libcrun_error_t *err)
{
  صحيح ret = access (path, F_OK);
  إذا (ret < 0)
    {
      إذا (errno == ENOENT)
        أرجع 0;
      أرجع crun_make_error (err, errno, "access `%s`", path);
    }
  أرجع 1;
}

صحيح
xasprintf (حرف **str, ثابت حرف *fmt, ...)
{
  صحيح ret;
  va_list وسائط_list;

  va_start (وسائط_list, fmt);

  ret = vasprintf (str, fmt, وسائط_list);
  إذا (UNLIKELY (ret < 0))
    OOM ();

  va_end (وسائط_list);
  أرجع ret;
}

صحيح
write_file_at_with_flags (صحيح dirfd, صحيح flags, mode_t mode, ثابت حرف *name, ثابت فراغ *data, size_t len, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  صحيح ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع (len > INT_MAX) ? INT_MAX : (صحيح) len;
}

صحيح
detach_process ()
{
  pid_t pid;
  إذا (setsid () < 0)
    أرجع -1;
  pid = fork ();
  إذا (pid < 0)
    أرجع -1;
  إذا (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  أرجع 0;
}

صحيح
get_file_type_fd (صحيح fd, mode_t *mode)
{
  بنية stat st;
  صحيح ret;

#ifdef HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;

      أرجع ret;
    }
  *mode = stx.stx_mode;
  أرجع ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  أرجع ret;
}

صحيح
get_file_type_at (صحيح dirfd, mode_t *mode, bool nofollow, ثابت حرف *path)
{
  صحيح empty_path = path == عدم ? AT_EMPTY_PATH : 0;
  بنية stat st;
  صحيح ret;

#ifdef HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;

      أرجع ret;
    }
  *mode = stx.stx_mode;
  أرجع ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  أرجع ret;
}

صحيح
get_file_type (mode_t *mode, bool nofollow, ثابت حرف *path)
{
  أرجع get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

صحيح
create_file_if_missing_at (صحيح dirfd, ثابت حرف *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close صحيح fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  إذا (fd_write < 0)
    {
      صحيح saved_errno = errno;
      mode_t tmp_mode;
      صحيح ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      إذا (ret == 0 && S_ISREG (tmp_mode))
        أرجع 0;

      أرجع crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  أرجع 0;
}

ساكن صحيح
ensure_directory_internal_at (صحيح dirfd, حرف *path, size_t len, صحيح mode, libcrun_error_t *err)
{
  حرف *it = path + len;
  صحيح ret = 0;
  bool parent_created = false;

  لكل (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      إذا (ret == 0 || errno == EEXIST)
        أرجع 0;

      صحيح saved_errno = errno;
      إذا (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = عدم;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          إذا (ret > 0)
            أرجع 0;
          إذا (ret < 0)
            crun_error_release (&tmp_err);

          أرجع crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      طالما (it > path && *it != '/')
        {
          it--;
          len--;
        }
      إذا (it == path)
        أرجع 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      parent_created = true;
    }
  أرجع ret;
}

صحيح
crun_ensure_directory_at (صحيح dirfd, ثابت حرف *path, صحيح mode, bool nofollow, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret == 0)
    أرجع crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  أرجع 0;
}

ساكن صحيح
check_fd_is_path (ثابت حرف *path, صحيح fd, ثابت حرف *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  حرف link[PATH_MAX];
  صحيح ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, حجم (link)));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "readlink `%s`", fdname);

  إذا (((size_t) ret) != path_len || memcmp (link, path, path_len))
    أرجع crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  أرجع 0;
}

ساكن صحيح
check_fd_under_path (ثابت حرف *rootfs, size_t rootfslen, صحيح fd, ثابت حرف *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  حرف link[PATH_MAX];
  صحيح ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  إذا (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    أرجع 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, حجم (link)));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "readlink `%s`", fdname);

  إذا (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    أرجع crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  أرجع 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
ساكن فراغ
close_and_replace (صحيح *oldfd, صحيح newfd)
{
  إذا (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
حرف *chroot_realpath (ثابت حرف *chroot, ثابت حرف *path, حرف resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
ساكن صحيح
safe_openat_fallback (صحيح dirfd, ثابت حرف *rootfs, ثابت حرف *path, صحيح flags,
                      صحيح mode, libcrun_error_t *err)
{
  cleanup_free حرف *parent_path = عدم;
  ثابت حرف *last_component = عدم;
  ثابت حرف *orig_path = path;
  ثابت حرف *path_in_chroot;
  cleanup_close صحيح fd = -1;
  حرف resolved[PATH_MAX];
  حرف buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  صحيح ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  إذا (flags & O_NOFOLLOW)
    {
      حرف *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      إذا (sep == عدم)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      وإلا إذا (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      إذا (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  إذا (path_in_chroot == عدم)
    أرجع crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  إذا (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  إذا (last_component)
    {
      ret = snprintf (resolved, حجم (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      إذا (UNLIKELY (ret >= (صحيح) حجم (resolved)))
        أرجع crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  إذا (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      أرجع ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = fd;
  fd = -1;
  أرجع ret;
}

صحيح
safe_openat (صحيح dirfd, ثابت حرف *rootfs, ثابت حرف *path, صحيح flags, صحيح mode,
             libcrun_error_t *err)
{
  ساكن bool openat2_supported = true;
  صحيح retries = MAX_OPENAT2_EAGAIN_RETRIES;
  صحيح ret;

  إذا (is_empty_string (path))
    {
      cleanup_close صحيح fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      إذا (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      إذا (UNLIKELY (fd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      إذا (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = fd;
      fd = -1;
      أرجع ret;
    }

  إذا (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_جذر);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (errno == EINTR)
            اذهب repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          إذا (errno == EAGAIN && retries-- > 0)
            اذهب repeat;
          إذا (errno == ENOSYS)
            openat2_supported = false;
          إذا (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            أرجع safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          أرجع crun_make_error (err, errno, "openat2 `%s`", path);
        }

      أرجع ret;
    }

  أرجع safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (صحيح dfd, ثابت حرف *name, حرف **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free حرف *tmp_buf = عدم;
  ssize_t size;

  افعل
    {
      إذا (tmp_buf != عدم)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      إذا (UNLIKELY (size < 0))
        أرجع crun_make_error (err, errno, "readlink `%s`", name);
  } طالما (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = عدم;

  أرجع size;
}

ساكن صحيح crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, صحيح dirfd, ثابت حرف *dirpath,
                                ثابت حرف *path, صحيح mode, صحيح max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
ساكن صحيح
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, صحيح dirfd, ثابت حرف *dirpath, صحيح cwd,
                                 ثابت حرف *npath, ثابت حرف *cur, ثابت حرف *rest,
                                 صحيح mode, صحيح max_readlinks, libcrun_error_t *err)
{
  cleanup_free حرف *target = عدم;
  cleanup_free حرف *new_path = عدم;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (صحيح) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  أرجع crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

ساكن صحيح
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, صحيح dirfd, ثابت حرف *dirpath,
                     ثابت حرف *path, صحيح mode, صحيح max_readlinks, libcrun_error_t *err)
{
  cleanup_close صحيح wd_cleanup = -1;
  cleanup_free حرف *npath = عدم;
  bool last_component = false;
  size_t depth = 0;
  حرف *cur;
  حرف *it;
  صحيح cwd;
  صحيح ret;

  إذا (max_readlinks <= 0)
    أرجع crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  إذا (*path == '\0')
    {
      إذا (do_open)
        أرجع open (dirpath, O_CLOEXEC | O_PATH, 0);
      أرجع 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  طالما (cur)
    {
      إذا (it)
        *it = '\0';
      وإلا
        last_component = true;

      إذا (cur[0] == '\0')
        اكسر;

      إذا (strcmp (cur, ".") == 0)
        اذهب next;
      وإلا إذا (strcmp (cur, ".."))
        depth++;
      وإلا
        {
          إذا (depth)
            depth--;
          وإلا
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              اذهب next;
            }
        }

      إذا (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          إذا (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              إذا (errno == ELOOP)
                أرجع crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, عدم,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              إذا (ret < 0)
                أرجع crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          إذا (do_open)
            أرجع ret;

          close_and_replace (&wd_cleanup, ret);
          أرجع 0;
        }

      ret = mkdirat (cwd, cur, mode);
      إذا (ret < 0)
        {
          mode_t st_mode;

          إذا (errno != EEXIST)
            أرجع crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          إذا (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            أرجع crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : عدم, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      إذا (UNLIKELY (cwd < 0))
        أرجع crun_error_wrap (err, "creating `/%s`", path);

      إذا (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, عدم);
          إذا (UNLIKELY (ret < 0))
            {
              صحيح saved_errno = errno;

              close (cwd);
              أرجع crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          إذا ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              أرجع crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      إذا (it == عدم)
        اكسر;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  إذا (do_open)
    {
      إذا (cwd == dirfd)
        {
          ret = dup (dirfd);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "dup `%s`", dirpath);
          أرجع ret;
        }

      wd_cleanup = -1;
      أرجع cwd;
    }

  أرجع 0;
}

صحيح
crun_safe_create_and_open_ref_at (bool dir, صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *path, صحيح mode, libcrun_error_t *err)
{
  صحيح ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  إذا (LIKELY (ret >= 0))
    أرجع ret;

  crun_error_release (err);
  أرجع crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

صحيح
crun_safe_ensure_directory_at (صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *path, صحيح mode,
                               libcrun_error_t *err)
{
  أرجع crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

صحيح
crun_safe_ensure_file_at (صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *path, صحيح mode,
                          libcrun_error_t *err)
{
  أرجع crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

صحيح
crun_ensure_directory (ثابت حرف *path, صحيح mode, bool nofollow, libcrun_error_t *err)
{
  أرجع crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

ساكن صحيح
get_file_size (صحيح fd, off_t *size)
{
  بنية stat st;
  صحيح ret;
#ifdef HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;
      أرجع ret;
    }
  *size = stx.stx_size;

  أرجع ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  أرجع ret;
}

صحيح
crun_dir_p_at (صحيح dirfd, ثابت حرف *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  صحيح ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "stat `%s`", path);

  أرجع S_ISDIR (mode) ? 1 : 0;
}

صحيح
crun_dir_p (ثابت حرف *path, bool nofollow, libcrun_error_t *err)
{
  أرجع crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
ساكن صحيح
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free حرف *buffer = عدم;
  size_t len;
  صحيح ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  إذا (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      إذا (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          أرجع 0;
        }
      أرجع ret;
    }

  أرجع strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
ساكن bool
running_in_openvz ()
{
  إذا (access ("/proc/vz", F_OK) < 0)
    أرجع false;

  أرجع access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

صحيح
check_running_in_user_namespace (libcrun_error_t *err)
{
  ساكن atomic_int run_in_userns = -1;
  بنية stat st;
  صحيح ret;

  ret = run_in_userns;
  إذا (ret >= 0)
    أرجع ret;

  ret = stat ("/proc/self/ns/user", &st);
  إذا (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      إذا (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          أرجع run_in_userns;
        }

      إذا (! running_in_openvz ())
        {
          run_in_userns = 1;
          أرجع run_in_userns;
        }
    }
  وإلا إذا (UNLIKELY (errno != ENOENT))
    أرجع crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  run_in_userns = ret;
  أرجع run_in_userns;
}

ساكن size_t
get_page_size ()
{
  ساكن atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  إذا (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  أرجع (size_t) pagesize;
}

ساكن صحيح selinux_enabled = -1;
ساكن صحيح apparmor_enabled = -1;

صحيح
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free حرف *out = عدم;
  cleanup_close صحيح fd = -1;
  size_t len;
  صحيح ret;

  إذا (selinux_enabled >= 0)
    أرجع selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  أرجع selinux_enabled;
}

صحيح
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  صحيح size;
  حرف buf[1];

  إذا (apparmor_enabled >= 0)
    أرجع apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  إذا (fd == -1)
    {
      إذا (errno == ENOENT)
        {
          apparmor_enabled = 0;
          أرجع 0;
        }

      أرجع crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (اقرأ (fd, buf, حجم (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  أرجع apparmor_enabled;
}

ساكن صحيح
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  إذا (selinux_enabled < 0)
    أرجع crun_make_error (err, 0, "SELinux is not initialized correctly");
  أرجع selinux_enabled;
}

صحيح
add_selinux_mount_label (حرف **retlabel, ثابت حرف *data, ثابت حرف *label, ثابت حرف *context_type, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_is_selinux_enabled (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (label && ret)
    {
      إذا (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      وإلا
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      أرجع 0;
    }
  *retlabel = xstrdup (data);
  أرجع 0;
}

ساكن ثابت حرف *
lsm_attr_path (libcrun_container_t *container, ثابت حرف *lsm, ثابت حرف *fname, libcrun_error_t *err)
{
  cleanup_close صحيح attr_dirfd = -1;
  cleanup_close صحيح lsm_dirfd = -1;
  حرف *attr_path = عدم;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  إذا (UNLIKELY (attr_dirfd < 0))
    أرجع عدم;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  إذا (lsm != عدم)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      إذا (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          أرجع عدم;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  أرجع attr_path;
}

ساكن صحيح
check_proc_super_magic (صحيح fd, ثابت حرف *path, libcrun_error_t *err)
{
  بنية statfs sfs;

  صحيح ret = fstatfs (fd, &sfs);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "statfs `%s`", path);

  إذا (sfs.f_type != PROC_SUPER_MAGIC)
    أرجع crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  أرجع 0;
}

ساكن صحيح
set_security_attr (libcrun_container_t *container, ثابت حرف *lsm, ثابت حرف *fname, ثابت حرف *data, libcrun_error_t *err)
{
  صحيح ret;

  cleanup_free ثابت حرف *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close صحيح fd = -1;

  إذا (UNLIKELY (attr_path == عدم))
    أرجع -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (اكتب (fd, data, strlen (data)));
  إذا (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      إذا (errno == ENOENT && lsm != عدم && strcmp (lsm, "apparmor") == 0)
        {
          ثابت حرف *profile = strchr (data, ' ');

          أرجع crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      أرجع crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  أرجع 0;
}

صحيح
set_selinux_label (libcrun_container_t *container, ثابت حرف *label, bool now, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_is_selinux_enabled (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret)
    أرجع set_security_attr (container, عدم, now ? "current" : "exec", label, err);
  أرجع 0;
}

ساكن صحيح
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  إذا (apparmor_enabled < 0)
    أرجع crun_make_error (err, 0, "AppArmor is not initialized correctly");
  أرجع apparmor_enabled;
}

ساكن صحيح
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (حجم (UNCONFINED) - 1)
  cleanup_free ثابت حرف *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close صحيح fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  حرف buf[UNCONFINED_LEN + 1];

  إذا (UNLIKELY (attr_path == عدم))
    أرجع -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  إذا (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    أرجع -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (اقرأ (fd, buf, حجم (buf)));
  إذا (UNLIKELY (bytes_read < 0))
    أرجع crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  إذا (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    أرجع 1;
  إذا (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    أرجع 0;
  أرجع 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

صحيح
set_apparmor_profile (libcrun_container_t *container, ثابت حرف *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_is_apparmor_enabled (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret)
    {
      cleanup_free حرف *buf = عدم;
      ret = is_current_process_confined (container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      أرجع set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  أرجع 0;
}

ساكن صحيح
read_all_fd_internal (صحيح fd, ثابت حرف *description, حرف **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free حرف *buf = عدم;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  صحيح ret;

  إذا (size_hint)
    allocated = size_hint;
  وإلا
    {
      ret = get_file_size (fd, &size);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "stat `%s`", description);

      إذا (max_size && (size_t) size > max_size)
        أرجع crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  طالما ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (اقرأ (fd, buf + nread, allocated - nread));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "read from file `%s`", description);

      إذا (ret == 0)
        اكسر;

      nread += ret;

      إذا (max_size && nread > max_size)
        أرجع crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      إذا (nread == allocated)
        {
          إذا (size)
            اكسر;

          إذا (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  إذا (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      حرف *tmp = realloc (buf, nread + 1);
      إذا (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = عدم;
  إذا (len)
    *len = nread;
  أرجع 0;
}

صحيح
read_all_fd_with_size_hint (صحيح fd, ثابت حرف *description, حرف **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  أرجع read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

صحيح
read_all_fd_with_max_size (صحيح fd, ثابت حرف *description, حرف **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  أرجع read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

صحيح
read_all_file_at (صحيح dirfd, ثابت حرف *path, حرف **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", path);

  أرجع read_all_fd (fd, path, out, len, err);
}

صحيح
read_all_file (ثابت حرف *path, حرف **out, size_t *len, libcrun_error_t *err)
{
  إذا (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  أرجع read_all_file_at (AT_FDCWD, path, out, len, err);
}

صحيح
get_realpath_to_file (صحيح dirfd, ثابت حرف *path_name, حرف **absolute_path, libcrun_error_t *err)
{
  cleanup_close صحيح targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  إذا (UNLIKELY (targetfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", path_name);
  وإلا
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      إذا (UNLIKELY (len < 0))
        أرجع crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  أرجع 0;
}

صحيح
open_unix_domain_client_socket (ثابت حرف *path, صحيح dgram, libcrun_error_t *err)
{
  بنية sockaddr_un addr = {};
  صحيح ret;
  proc_fd_path_t name_buf;
  cleanup_close صحيح destfd = -1;
  cleanup_close صحيح fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "create UNIX socket");

  إذا (strlen (path) >= حجم (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      إذا (UNLIKELY (destfd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  إذا (path_len >= حجم (addr.sun_path))
    أرجع crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (بنية sockaddr *) &addr, حجم (addr));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  أرجع ret;
}

صحيح
open_unix_domain_socket (ثابت حرف *path, صحيح dgram, libcrun_error_t *err)
{
  بنية sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  صحيح ret;
  cleanup_close صحيح dirfd = -1;
  cleanup_close صحيح fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "create UNIX socket");

  إذا (strlen (path) >= حجم (addr.sun_path))
    {
      cleanup_free حرف *dpath = xstrdup (path);
      cleanup_free حرف *bpath = xstrdup (path);
      ثابت حرف *parent_dir = dirname (dpath);
      ثابت حرف *base = basename (bpath);
      صحيح n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      إذا (UNLIKELY (dirfd < 0))
        أرجع crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, حجم (addr.sun_path), "%s/%s", name_buf, base);
      إذا (n < 0 || (size_t) n >= حجم (addr.sun_path))
        أرجع crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  وإلا
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (بنية sockaddr *) &addr, حجم (addr));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "bind socket to `%s`", path);

  إذا (! dgram)
    {
      ret = listen (fd, 1);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  أرجع ret;
}

صحيح
send_fd_to_socket (صحيح server, صحيح fd, libcrun_error_t *err)
{
  أرجع send_fd_to_socket_with_payload (server, fd, عدم, 0, err);
}

صحيح
send_fd_to_socket_with_payload (صحيح server, صحيح fd, ثابت حرف *payload, size_t payload_len, libcrun_error_t *err)
{
  صحيح ret;
  بنية cmsghdr *cmsg = عدم;
  بنية iovec iov[2];
  بنية msghdr رسالة = {};
  حرف ctrl_buf[CMSG_SPACE (1 + حجم (صحيح))] = {};
  حرف data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = حجم (data);

  إذا (payload_len > 0)
    {
      iov[0].iov_base = (فراغ *) payload;
      iov[0].iov_len = payload_len;
    }

  رسالة.رسالة_name = عدم;
  رسالة.رسالة_namelen = 0;
  رسالة.رسالة_iov = iov;
  رسالة.رسالة_iovlen = 1;
  رسالة.رسالة_controllen = CMSG_SPACE (حجم (صحيح));
  رسالة.رسالة_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&رسالة);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (حجم (صحيح));

  *((صحيح *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &رسالة, 0));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "sendmsg");
  أرجع 0;
}

صحيح
receive_fd_from_socket_with_payload (صحيح from, حرف *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  صحيح ret;
  بنية iovec iov[1];
  بنية msghdr رسالة = {};
  حرف ctrl_buf[CMSG_SPACE (حجم (صحيح))] = {};
  حرف data[1];
  بنية cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = حجم (data);

  إذا (payload_len > 0)
    {
      iov[0].iov_base = (فراغ *) payload;
      iov[0].iov_len = payload_len;
    }

  رسالة.رسالة_name = عدم;
  رسالة.رسالة_namelen = 0;
  رسالة.رسالة_iov = iov;
  رسالة.رسالة_iovlen = 1;
  رسالة.رسالة_controllen = CMSG_SPACE (حجم (صحيح));
  رسالة.رسالة_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &رسالة, 0));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "recvmsg");
  إذا (UNLIKELY (ret == 0))
    أرجع crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&رسالة);
  إذا (cmsg == عدم)
    أرجع crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), حجم (fd));

  ret = fd;
  fd = -1;
  أرجع ret;
}

صحيح
receive_fd_from_socket (صحيح from, libcrun_error_t *err)
{
  أرجع receive_fd_from_socket_with_payload (from, عدم, 0, err);
}

صحيح
create_socket_pair (صحيح *pair, libcrun_error_t *err)
{
  صحيح ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "socketpair");
  أرجع 0;
}

صحيح
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  صحيح ret = signalfd (-1, mask, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "signalfd");
  أرجع ret;
}

ساكن صحيح
epoll_helper_toggle (صحيح epollfd, صحيح fd, صحيح events, libcrun_error_t *err)
{
  بنية epoll_event ev = {};
  bool add = events != 0;
  صحيح ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  إذا (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  إذا (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      إذا (errno == ENOENT || errno == EPERM)
        أرجع 0;
      أرجع crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  أرجع 0;
}

صحيح
epoll_helper (صحيح *in_fds, صحيح *in_levelfds, صحيح *out_fds, صحيح *out_levelfds, libcrun_error_t *err)
{
  بنية epoll_event ev;
  cleanup_close صحيح epollfd = -1;
  صحيح ret;

  صحيح *it;
  epollfd = epoll_create1 (0);
  إذا (UNLIKELY (epollfd < 0))
    أرجع crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  لكل (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      إذا (UNLIKELY (ret < 0))                                           \
        أرجع crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  إذا (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  إذا (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  إذا (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  إذا (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  أرجع ret;
}

صحيح
copy_from_fd_to_fd (صحيح src, صحيح dst, صحيح consume, libcrun_error_t *err)
{
  صحيح ret;
  ssize_t nread;
  size_t pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  افعل
    {
      cleanup_free حرف *buffer = عدم;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      إذا (can_copy_file_range)
        {
          nread = copy_file_range (src, عدم, dst, عدم, pagesize, 0);
          إذا (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              اذهب fallback;
            }
          إذا (consume && nread < 0 && errno == EAGAIN)
            أرجع 0;
          إذا (nread < 0 && errno == EIO)
            أرجع 0;
          إذا (UNLIKELY (nread < 0))
            أرجع crun_make_error (err, errno, "copy_file_range");
          استمر;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (اقرأ (src, buffer, pagesize));
      إذا (consume && nread < 0 && errno == EAGAIN)
        أرجع 0;
      إذا (nread < 0 && errno == EIO)
        أرجع 0;
      إذا (UNLIKELY (nread < 0))
        أرجع crun_make_error (err, errno, "read");

      remaining = nread;
      طالما (remaining)
        {
          ret = TEMP_FAILURE_RETRY (اكتب (dst, buffer + nread - remaining, remaining));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } طالما (consume && nread);

  أرجع 0;
}

صحيح
run_process (حرف **وسائط, libcrun_error_t *err)
{
  pid_t pid = fork ();
  إذا (UNLIKELY (pid < 0))
    أرجع crun_make_error (err, errno, "fork");
  إذا (pid)
    {
      صحيح r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      إذا (r < 0)
        أرجع crun_make_error (err, errno, "waitpid");
      إذا (WIFEXITED (status) || WIFSIGNALED (status))
        أرجع WEXITSTATUS (status);
    }

  execvp (وسائط[0], وسائط);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
ساكن unsigned
atou (حرف **s)
{
  unsigned x;
  لكل (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  أرجع x;
}

صحيح
fgetpwent_r (FILE *f, بنية passwd *pw, حرف *سطر, size_t size, بنية passwd **res)
{
  حرف *s;
  صحيح rv = 0;
  لكل (;;)
    {
      سطر[size - 1] = '\xff';
      إذا ((fgets (سطر, size, f) == عدم) || ferror (f) || سطر[size - 1] != '\xff')
        {
          rv = (سطر[size - 1] != '\xff') ? ERANGE : ENOENT;
          سطر = 0;
          pw = 0;
          اكسر;
        }
      سطر[strcspn (سطر, "\n")] = 0;

      s = سطر;
      pw->pw_name = s++;
      إذا (! (s = strchr (s, ':')))
        استمر;

      *s++ = 0;
      pw->pw_passwd = s;
      إذا (! (s = strchr (s, ':')))
        استمر;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      إذا (*s != ':')
        استمر;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      إذا (*s != ':')
        استمر;

      *s++ = 0;
      pw->pw_gecos = s;
      إذا (! (s = strchr (s, ':')))
        استمر;

      *s++ = 0;
      pw->pw_dir = s;
      إذا (! (s = strchr (s, ':')))
        استمر;

      *s++ = 0;
      pw->pw_shell = s;
      اكسر;
    }
  *res = pw;
  إذا (rv)
    errno = rv;
  أرجع rv;
}
#endif

صحيح
set_home_env (uid_t id)
{
  بنية passwd pwd;
  cleanup_free حرف *buf = عدم;
  long buf_size;
  cleanup_file FILE *stream = عدم;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  إذا (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  إذا (stream == عدم)
    أرجع -1;

  لكل (;;)
    {
      بنية passwd *ret_pw = عدم;

      صحيح ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      إذا (UNLIKELY (ret != 0))
        {
          إذا (ret == EINTR)
            استمر;

          إذا (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              أرجع -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          استمر;
        }

      إذا (ret_pw && ret_pw->pw_uid == id)
        {
          إذا (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          وإلا
            أرجع 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
ساكن صحيح
getsubidrange (uid_t id, صحيح is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *مدخل = عدم;
  cleanup_free حرف *lineptr = عدم;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free حرف *buf = عدم;
  ثابت حرف *name;
  بنية passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  إذا (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  لكل (;;)
    {
      صحيح ret;
      بنية passwd *ret_pw = عدم;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      إذا (LIKELY (ret == 0))
        {
          إذا (ret_pw)
            {
              name = ret_pw->pw_name;
              اكسر;
            }
          أرجع -1;
        }

      إذا (ret != ERANGE)
        أرجع -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  مدخل = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  إذا (مدخل == عدم)
    أرجع -1;

  لكل (;;)
    {
      حرف *endptr;
      ssize_t اقرأ = getline (&lineptr, &lenlineptr, مدخل);
      إذا (اقرأ < 0)
        أرجع -1;

      إذا (اقرأ < (ssize_t) (len_name + 2))
        استمر;

      إذا (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        استمر;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      إذا (endptr >= &lineptr[اقرأ])
        أرجع -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      أرجع 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

صحيح
format_default_id_mapping (حرف **out, uid_t container_id, uid_t host_uid, uid_t host_id, صحيح is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free حرف *buffer = عدم;
  صحيح written = 0;
  صحيح ret, remaining;

  *out = عدم;

  إذا (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    أرجع 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  إذا (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      إذا (UNLIKELY (ret >= remaining))
        أرجع crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  إذا (UNLIKELY (ret >= remaining))
    أرجع crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  إذا (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      إذا (UNLIKELY (ret >= remaining))
        أرجع crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = عدم;
  أرجع written;
}

ساكن صحيح
unset_cloexec_flag (صحيح fd)
{
  صحيح flags = fcntl (fd, F_GETFD);
  إذا (flags == -1)
    أرجع -1;

  flags &= ~FD_CLOEXEC;

  أرجع fcntl (fd, F_SETFD, flags);
}

ساكن فراغ __attribute__ ((__noreturn__))
run_process_child (حرف *path, حرف **وسائط, ثابت حرف *cwd, حرف **envp, صحيح pipe_r,
                   صحيح pipe_w, صحيح out_fd, صحيح err_fd, bool can_ignore_chdir_errors)
{
  حرف *tmp_وسائط[] = { path, عدم };
  libcrun_error_t err = عدم;
  صحيح dev_null_fd = -1;
  صحيح ret;

  ret = mark_or_close_fds_ge_than (عدم, 3, false, &err);
  إذا (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->رسالة);

  إذا (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      إذا (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  إذا (out_fd >= 0)
    unset_cloexec_flag (1);
  إذا (err_fd >= 0)
    unset_cloexec_flag (2);

  إذا (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  إذا (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  إذا (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  إذا (وسائط == عدم)
    وسائط = tmp_وسائط;

  إذا (cwd && chdir (cwd) < 0)
    {
      إذا (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, وسائط, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
صحيح
run_process_with_stdin_timeout_envp (حرف *path, حرف **وسائط, ثابت حرف *cwd, صحيح timeout,
                                     حرف **envp, حرف *stdin, size_t stdin_len, صحيح out_fd,
                                     صحيح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  صحيح stdin_pipe[2];
  pid_t pid;
  صحيح ret;
  cleanup_close صحيح pipe_r = -1;
  cleanup_close صحيح pipe_w = -1;
  sigset_t oldmask, mask;
  صحيح r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  إذا (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  إذا (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      اذهب restore_sig_mask_and_exit;
    }

  إذا (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, وسائط, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (اكتب (pipe_w, stdin, stdin_len));
  إذا (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      إذا (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          اذهب restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  إذا (timeout)
    {
      time_t start = time (عدم);
      time_t now;
      لكل (now = start; now - start < timeout; now = time (عدم))
        {
          siginfo_t info;
          صحيح elapsed = now - start;
          بنية timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          إذا (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              اذهب restore_sig_mask_and_exit;
            }

          إذا (info.si_signo == SIGCHLD && info.si_pid == pid)
            اذهب read_waitpid;

          إذا (ret < 0 && errno == EAGAIN)
            اذهب timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      اذهب restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  إذا (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  وإلا
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  إذا (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      إذا (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, عدم);
      إذا (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  أرجع ret;
}

صحيح
mark_or_close_fds_ge_than (libcrun_container_t *container, صحيح n, bool close_now, libcrun_error_t *err)
{
  cleanup_close صحيح cfd = -1;
  cleanup_dir DIR *dir = عدم;
  صحيح ret;
  صحيح fd;
  بنية dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  إذا (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  إذا (ret == 0)
    {
      إذا (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      أرجع 0;
    }
  إذا (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    أرجع crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  إذا (UNLIKELY (cfd < 0))
    أرجع cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dir = fdopendir (cfd);
  إذا (UNLIKELY (dir == عدم))
    أرجع crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  لكل (next = readdir (dir); next; next = readdir (dir))
    {
      صحيح val;
      ثابت حرف *name = next->d_name;
      إذا (name[0] == '.')
        استمر;

      val = strtoll (name, عدم, 10);
      إذا (val < n || val == fd)
        استمر;

      إذا (close_now)
        {
          ret = close (val);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "close fd `%d`", val);

          إذا (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      وإلا
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  أرجع 0;
}

فراغ
get_current_timestamp (حرف *out, size_t len)
{
  بنية timeval tv;
  بنية tm now;
  حرف timestamp[64];

  gettimeofday (&tv, عدم);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, حجم (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (فراغ) snprintf (out, len, "%s.%06lldZ", timestamp, (long long صحيح) tv.tv_usec);
  out[len - 1] = '\0';
}

صحيح
set_blocking_fd (صحيح fd, bool blocking, libcrun_error_t *err)
{
  صحيح ret, flags = fcntl (fd, F_GETFL, 0);
  إذا (UNLIKELY (flags < 0))
    أرجع crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fcntl");
  أرجع 0;
}

صحيح
parse_json_file (json_object **out, ثابت حرف *jsondata, بنية parser_context *ctx arg_unused, libcrun_error_t *err)
{
  بنية json_tokener *tok;
  تعداد json_tokener_error jerr;

  *err = عدم;

  إذا (UNLIKELY (jsondata == عدم))
    أرجع crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  إذا (UNLIKELY (tok == عدم))
    أرجع crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (صحيح) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  إذا (*out == عدم || jerr != json_tokener_success)
    {
      صحيح saved_pos = (صحيح) tok->char_offset;

      json_tokener_free (tok);
      إذا (*out)
        {
          json_object_put (*out);
          *out = عدم;
        }
      أرجع crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  أرجع 0;
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
ساكن صحيح
check_access (ثابت حرف *path)
{
  صحيح ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#وإلا
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  إذا (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      إذا (errno == EACCES)
        {
          صحيح saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          إذا (ret == 0)
            أرجع CHECK_ACCESS_NOT_EXECUTABLE;
        }
      أرجع -errno;
    }

  ret = get_file_type (&mode, false, path);
  إذا (UNLIKELY (ret < 0))
    أرجع -errno;

  إذا (! S_ISREG (mode))
    أرجع CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  أرجع 0;
}

صحيح
find_executable (حرف **out, ثابت حرف *executable_path, ثابت حرف *cwd, libcrun_error_t *err)
{
  cleanup_free حرف *cwd_executable_path = عدم;
  cleanup_free حرف *tmp = عدم;
  صحيح last_error = -ENOENT;
  حرف *it, *end;
  صحيح ret;

  *out = عدم;

  إذا (executable_path == عدم || executable_path[0] == '\0')
    أرجع crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  إذا (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free حرف *cwd_allocated = عدم;

      إذا (cwd == عدم)
        {
          cwd_allocated = getcwd (عدم, 0);
          إذا (cwd_allocated == عدم)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  إذا (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      إذا (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          أرجع 0;
        }
      last_error = ret;
      اذهب fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  طالما ((it = strsep (&end, ":")))
    {
      cleanup_free حرف *path = عدم;

      إذا (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, عدم);
      إذا (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          استمر;
        }

      ret = check_access (path);
      إذا (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = عدم;
          أرجع 0;
        }

      إذا (ret == -ENOENT)
        استمر;

      last_error = ret;
    }

fail:
  بدل (last_error)
    {
    حالة CHECK_ACCESS_NOT_EXECUTABLE:
      أرجع crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    حالة CHECK_ACCESS_NOT_REGULAR:
      أرجع crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    افتراضي:
      errno = -last_error;
      إذا (errno == ENOENT)
        أرجع crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      أرجع crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

ساكن ssize_t
safe_read_xattr (حرف **ret, صحيح sfd, ثابت حرف *srcname, ثابت حرف *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free حرف *buffer = عدم;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  طالما (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      إذا (UNLIKELY (s < 0))
        أرجع crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      إذا (s < current_size)
        اكسر;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  إذا (s <= 0)
    أرجع s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = عدم;

  أرجع s;
}

ساكن ssize_t
copy_xattr (صحيح sfd, صحيح dfd, ثابت حرف *srcname, ثابت حرف *destname, libcrun_error_t *err)
{
  cleanup_free حرف *buf = عدم;
  ssize_t xattr_len;
  حرف *it;

  xattr_len = flistxattr (sfd, عدم, 0);
  إذا (UNLIKELY (xattr_len < 0))
    {
      إذا (errno == ENOTSUP)
        أرجع 0;

      أرجع crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  إذا (xattr_len == 0)
    أرجع 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  إذا (UNLIKELY (xattr_len < 0))
    أرجع crun_make_error (err, errno, "flistxattr `%s`", srcname);

  لكل (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free حرف *v = عدم;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      إذا (UNLIKELY (s < 0))
        أرجع s;

      s = fsetxattr (dfd, it, v, s, 0);
      إذا (UNLIKELY (s < 0))
        {
          إذا (errno == EINVAL || errno == EOPNOTSUPP)
            استمر;

          أرجع crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  أرجع 0;
}

#endif

ساكن صحيح
copy_rec_stat_file_at (صحيح dfd, ثابت حرف *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  بنية stat st;
  صحيح ret;

#ifdef HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;

      أرجع ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  أرجع ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  أرجع ret;
}

صحيح
copy_recursive_fd_to_fd (صحيح srcdirfd, صحيح dfd, ثابت حرف *srcname, ثابت حرف *destname, libcrun_error_t *err)
{
  cleanup_close صحيح destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = عدم;
  بنية dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  إذا (UNLIKELY (dsrcfd == عدم))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      أرجع crun_make_error (err, errno, "open directory `%s`", destname);
    }

  لكل (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close صحيح srcfd = -1;
      cleanup_close صحيح destfd = -1;
      cleanup_free حرف *target_buf = عدم;
      صحيح ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      إذا (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        استمر;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      بدل (mode & S_IFMT)
        {
        حالة S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          إذا (UNLIKELY (srcfd < 0))
            أرجع crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          إذا (UNLIKELY (destfd < 0))
            أرجع crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

#ifdef HAVE_FGETXATTR
          ret = (صحيح) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          اكسر;

        حالة S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          إذا (UNLIKELY (srcfd < 0))
            أرجع crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          إذا (UNLIKELY (destfd < 0))
            أرجع crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (صحيح) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          اكسر;

        حالة S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          اكسر;

        حالة S_IFBLK:
        حالة S_IFCHR:
        حالة S_IFIFO:
        حالة S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          اكسر;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      إذا (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          إذا (errno == ENOTSUP)
            استمر;

          أرجع crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  أرجع 0;
}

ثابت حرف *
find_annotation (libcrun_container_t *container, ثابت حرف *name)
{
  إذا (container->container_def->annotations == عدم)
    أرجع عدم;

  أرجع find_string_map_قيمة (container->annotations, name);
}

صحيح
safe_write (صحيح fd, ثابت حرف *fname, ثابت فراغ *buf, size_t عدد, libcrun_error_t *err)
{
  size_t written = 0;
  طالما (written < عدد)
    {
      ssize_t w = اكتب (fd, (ثابت حرف *) buf + written, عدد - written);
      إذا (UNLIKELY (w < 0))
        {
          إذا (errno == EINTR || errno == EAGAIN)
            استمر;
          أرجع crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  أرجع 0;
}

#define MAX_PARTS 32

صحيح
append_paths (حرف **out, libcrun_error_t *err, ...)
{
  ثابت حرف *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t total_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  لكل (;;)
    {
      ثابت حرف *part;
      size_t size;

      part = va_arg (ap, ثابت حرف *);
      إذا (part == عدم)
        اكسر;

      إذا (n_parts == MAX_PARTS)
        {
          va_end (ap);
          أرجع crun_make_error (err, EINVAL, "too many paths specified");
        }

      إذا (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          طالما (part[0] == '/' && part[1] == '/')
            part++;
        }
      وإلا
        {
          /* And drop any initial '/' for other components.  */
          طالما (part[0] == '/')
            part++;
        }

      size = strlen (part);
      إذا (size == 0)
        استمر;

      طالما (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  لكل (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  لكل (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      إذا (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  أرجع 0;
}

#إذا __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#وإلا
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
ساكن صحيح
b64_مدخل (حرف c)
{
  ثابت حرف table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  صحيح i;

  لكل (i = 0; i < 64; i++)
    {
      إذا (table[i] == c)
        أرجع i;
    }
  أرجع -1;
}

صحيح
base64_decode (ثابت حرف *iptr, size_t isize, حرف *optr, size_t osize, size_t *nbytes)
{
  صحيح i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned حرف data[4];

  *nbytes = 0;
  طالما (consumed < isize && (*nbytes) + 3 < osize)
    {
      طالما ((i < 4) && (consumed < isize))
        {
          tmp = b64_مدخل (*iptr++);
          consumed++;
          إذا (tmp != -1)
            {
              إذا (UNLIKELY (pad > 0))
                أرجع -1;
              data[i++] = tmp;
            }
          وإلا إذا (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      إذا (i == 4)
        {
          إذا (UNLIKELY (pad > 2))
            أرجع -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      وإلا
        {
          /* I did not get all the data.  */
          consumed -= i;
          أرجع consumed;
        }
      i = 0;
    }
  أرجع consumed;
}

حرف *
get_user_name (uid_t uid)
{
  بنية passwd pd;
  بنية passwd *temp_result_ptr;
  حرف pwdbuffer[200];
  إذا (! getpwuid_r (uid, &pd, pwdbuffer, حجم (pwdbuffer), &temp_result_ptr))
    أرجع xstrdup (pd.pw_name);
  أرجع xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
حرف *
libcrun_sysctl_مفتاح_to_proc_path (ثابت حرف *مفتاح)
{
  ثابت حرف *sep;
  حرف *ret, *it;

  sep = strpbrk (مفتاح, "./");

  /* No separator at all, or the key is already in the path form.  */
  إذا (sep == عدم || *sep == '/')
    أرجع xstrdup (مفتاح);

  ret = xstrdup (مفتاح);
  لكل (it = ret; *it; it++)
    {
      إذا (*it == '.')
        *it = '/';
      وإلا إذا (*it == '/')
        *it = '.';
    }

  أرجع ret;
}

صحيح
has_suffix (ثابت حرف *str, ثابت حرف *suffix)
{
  إذا (! str || ! suffix)
    أرجع 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  إذا (lensuffix > lenstr)
    أرجع 0;
  أرجع memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

حرف *
str_join_array (صحيح offset, size_t size, حرف *ثابت array[], ثابت حرف *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = عدم;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  حرف *result, *p;

  lens = xmalloc (size * حجم (*lens));

  لكل (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  لكل (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      إذا (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  أرجع result;
}

صحيح
libcrun_mmap (بنية libcrun_mmap_s **ret, فراغ *addr, size_t length,
              صحيح prot, صحيح flags, صحيح fd, off_t offset,
              libcrun_error_t *err)
{
  بنية libcrun_mmap_s *mmap_s = عدم;

  فراغ *mapped = mmap (addr, length, prot, flags, fd, offset);
  إذا (mapped == MAP_FAILED)
    أرجع crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (حجم (بنية libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  أرجع 0;
}

صحيح
libcrun_munmap (بنية libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  صحيح ret;

  ret = munmap (mmap->addr, mmap->length);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "munmap");

  حرر (mmap);

  أرجع 0;
}

ساكن long
read_file_as_long_or_default (ثابت حرف *path, long def_قيمة)
{
  cleanup_free حرف *content = عدم;
  libcrun_error_t tmp_err = عدم;
  حرف *endptr;
  long val;
  صحيح ret;

  ret = read_all_file (path, &content, عدم, &tmp_err);
  إذا (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      أرجع def_قيمة;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  إذا (UNLIKELY (errno))
    أرجع def_قيمة;
  إذا (endptr == content || (*endptr && *endptr != '\n'))
    أرجع def_قيمة;
  أرجع val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (فراغ)
{
  ساكن atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  إذا (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  أرجع uid;
}

gid_t
get_overflow_gid (فراغ)
{
  ساكن atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  إذا (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  أرجع gid;
}

فراغ
consume_trailing_slashes (حرف *path)
{
  إذا (! path || path[0] == '\0')
    أرجع;

  حرف *last = path + strlen (path);

  طالما (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

حرف **
read_dir_entries (ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = عدم;
  size_t n_entries = 0;
  size_t entries_size = 16;
  حرف **entries = عدم;
  بنية dirent *de;

  dir = opendir (path);
  إذا (UNLIKELY (dir == عدم))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      أرجع عدم;
    }

  entries = xmalloc (entries_size * حجم (حرف *));
  طالما ((de = readdir (dir)))
    {
      إذا (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        استمر;
      إذا (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * حجم (حرف *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * حجم (حرف *));
  entries[n_entries] = عدم;

  أرجع entries;
}

صحيح
cpuset_string_to_bitmask (ثابت حرف *str, حرف **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free حرف *mask = عدم;
  size_t mask_size = 0;
  ثابت حرف *p = str;
  حرف *endptr;

  طالما (*p)
    {
      long long start_range, end_range;

      إذا (*p < '0' || *p > '9')
        اذهب invalid_مدخل;

      start_range = strtoll (p, &endptr, 10);
      إذا (start_range < 0)
        اذهب invalid_مدخل;

      p = endptr;

      إذا (*p != '-')
        end_range = start_range;
      وإلا
        {
          p++;

          إذا (*p < '0' || *p > '9')
            اذهب invalid_مدخل;

          end_range = strtoll (p, &endptr, 10);

          إذا (end_range < start_range)
            اذهب invalid_مدخل;

          p = endptr;
        }

      /* Just set some limit.  */
      إذا (end_range > (1 << 20))
        اذهب invalid_مدخل;

      إذا (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      لكل (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      إذا (*p == ',')
        p++;
      وإلا إذا (*p)
        اذهب invalid_مدخل;
    }

  *out = mask;
  mask = عدم;
  *out_size = mask_size;

  أرجع 0;

invalid_مدخل:
  أرجع crun_make_error (err, 0, "cannot parse input `%s`", str);
}

بنية channel_fd_pair
{
  بنية ring_buffer *rb;

  صحيح in_fd;
  صحيح out_fd;

  صحيح infd_epoll_events;
  صحيح outfd_epoll_events;
};

بنية channel_fd_pair *
channel_fd_pair_new (صحيح in_fd, صحيح out_fd, size_t size)
{
  بنية channel_fd_pair *channel = xmalloc (حجم (بنية channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  أرجع channel;
}

فراغ
channel_fd_pair_free (بنية channel_fd_pair *channel)
{
  إذا (channel == عدم)
    أرجع;

  ring_buffer_free (channel->rb);
  حرر (channel);
}

صحيح
channel_fd_pair_process (بنية channel_fd_pair *channel, صحيح epollfd, libcrun_error_t *err)
{
  bool is_مدخل_eagain = false, is_output_eagain = false, repeat;
  صحيح ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  لكل (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      إذا (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_مدخل_eagain, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          إذا (ret > 0)
            repeat = true;
        }
      إذا (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          إذا (ret > 0)
            repeat = true;
        }
    }

  إذا (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      صحيح events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_مدخل_eagain ? EPOLLET : 0)) : 0;
      إذا (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      إذا (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          channel->outfd_epoll_events = events;
        }
    }
  أرجع 0;
}

ساكن صحيح
open_proc (libcrun_error_t *err)
{
  صحيح fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close صحيح fsfd = -1;
  صحيح ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  إذا (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, عدم, عدم, 0);
      إذا (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          إذا (fd >= 0)
            أرجع fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  إذا (fd < 0)
    أرجع crun_make_error (err, errno, "open `/proc`");
  أرجع fd;
}

صحيح
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  إذا (! container)
    أرجع open_proc (err);

  إذا (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  أرجع container->proc_fd;
}

صحيح
libcrun_open_proc_file (libcrun_container_t *container, ثابت حرف *path, صحيح flags, libcrun_error_t *err)
{
  صحيح proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  إذا (proc_fd < 0)
    أرجع proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  إذا (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  أرجع fd;
}

صحيح
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ثابت حرف *path, صحيح flags, libcrun_error_t *err)
{
  cleanup_free حرف *full_path = عدم;
  xasprintf (&full_path, "%d/%s", pid, path);
  أرجع libcrun_open_proc_file (container, full_path, flags, err);
}
