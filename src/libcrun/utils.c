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
#अन्तर्भाव <config.h>
#अन्तर्भाव "utils.h"
#अन्तर्भाव "ring_buffer.h"
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <libgen.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/un.h>
#अन्तर्भाव <sys/socket.h>
#अन्तर्भाव <sys/signalfd.h>
#अन्तर्भाव <sys/epoll.h>
#अन्तर्भाव <sys/syscall.h>
#अन्तर्भाव <sys/wait.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <grp.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <sys/xattr.h>
#अन्तर्भाव <sys/sysmacros.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <linux/magic.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <sys/mman.h>
#ifdef HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#endif
#सचे HAVE_STDATOMIC_H
#  include <stdatomic.h>
#अञ्ञथा
#  define atomic_long चञ्चल long
#  define atomic_int चञ्चल पूर्णाङ्क
#endif
#अन्तर्भाव "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_मूलम्
#  define RESOLVE_IN_मूलम् 0x10
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

पूर्णाङ्क
crun_path_exists (ध्रुव वर्ण *path, libcrun_error_t *err)
{
  पूर्णाङ्क ret = access (path, F_OK);
  सचे (ret < 0)
    {
      सचे (errno == ENOENT)
        पटिनिवत्त 0;
      पटिनिवत्त crun_make_error (err, errno, "access `%s`", path);
    }
  पटिनिवत्त 1;
}

पूर्णाङ्क
xasprintf (वर्ण **str, ध्रुव वर्ण *fmt, ...)
{
  पूर्णाङ्क ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  सचे (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  पटिनिवत्त ret;
}

पूर्णाङ्क
write_file_at_with_flags (पूर्णाङ्क dirfd, पूर्णाङ्क flags, mode_t mode, ध्रुव वर्ण *name, ध्रुव शून्य *data, size_t len, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त (len > INT_MAX) ? INT_MAX : (पूर्णाङ्क) len;
}

पूर्णाङ्क
detach_process ()
{
  pid_t pid;
  सचे (setsid () < 0)
    पटिनिवत्त -1;
  pid = fork ();
  सचे (pid < 0)
    पटिनिवत्त -1;
  सचे (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  पटिनिवत्त 0;
}

पूर्णाङ्क
get_file_type_fd (पूर्णाङ्क fd, mode_t *mode)
{
  संरचना stat st;
  पूर्णाङ्क ret;

#ifdef HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  सचे (UNLIKELY (ret < 0))
    {
      सचे (errno == ENOSYS || errno == EINVAL)
        गच्छ fallback;

      पटिनिवत्त ret;
    }
  *mode = stx.stx_mode;
  पटिनिवत्त ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  पटिनिवत्त ret;
}

पूर्णाङ्क
get_file_type_at (पूर्णाङ्क dirfd, mode_t *mode, bool nofollow, ध्रुव वर्ण *path)
{
  पूर्णाङ्क empty_path = path == रिक्त ? AT_EMPTY_PATH : 0;
  संरचना stat st;
  पूर्णाङ्क ret;

#ifdef HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  सचे (UNLIKELY (ret < 0))
    {
      सचे (errno == ENOSYS || errno == EINVAL)
        गच्छ fallback;

      पटिनिवत्त ret;
    }
  *mode = stx.stx_mode;
  पटिनिवत्त ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  पटिनिवत्त ret;
}

पूर्णाङ्क
get_file_type (mode_t *mode, bool nofollow, ध्रुव वर्ण *path)
{
  पटिनिवत्त get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

पूर्णाङ्क
create_file_if_missing_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  सचे (fd_write < 0)
    {
      पूर्णाङ्क saved_errno = errno;
      mode_t tmp_mode;
      पूर्णाङ्क ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      सचे (ret == 0 && S_ISREG (tmp_mode))
        पटिनिवत्त 0;

      पटिनिवत्त crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
ensure_directory_internal_at (पूर्णाङ्क dirfd, वर्ण *path, size_t len, पूर्णाङ्क mode, libcrun_error_t *err)
{
  वर्ण *it = path + len;
  पूर्णाङ्क ret = 0;
  bool parent_created = false;

  अत्थाय (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      सचे (ret == 0 || errno == EEXIST)
        पटिनिवत्त 0;

      पूर्णाङ्क saved_errno = errno;
      सचे (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = रिक्त;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          सचे (ret > 0)
            पटिनिवत्त 0;
          सचे (ret < 0)
            crun_error_release (&tmp_err);

          पटिनिवत्त crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      याव (it > path && *it != '/')
        {
          it--;
          len--;
        }
      सचे (it == path)
        पटिनिवत्त 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      parent_created = true;
    }
  पटिनिवत्त ret;
}

पूर्णाङ्क
crun_ensure_directory_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *path, पूर्णाङ्क mode, bool nofollow, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (ret == 0)
    पटिनिवत्त crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
check_fd_is_path (ध्रुव वर्ण *path, पूर्णाङ्क fd, ध्रुव वर्ण *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  वर्ण link[PATH_MAX];
  पूर्णाङ्क ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, परिमाण (link)));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "readlink `%s`", fdname);

  सचे (((size_t) ret) != path_len || memcmp (link, path, path_len))
    पटिनिवत्त crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
check_fd_under_path (ध्रुव वर्ण *rootfs, size_t rootfslen, पूर्णाङ्क fd, ध्रुव वर्ण *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  वर्ण link[PATH_MAX];
  पूर्णाङ्क ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  सचे (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    पटिनिवत्त 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, परिमाण (link)));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "readlink `%s`", fdname);

  सचे (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    पटिनिवत्त crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  पटिनिवत्त 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
स्थिर शून्य
close_and_replace (पूर्णाङ्क *oldfd, पूर्णाङ्क newfd)
{
  सचे (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
वर्ण *chroot_realpath (ध्रुव वर्ण *chroot, ध्रुव वर्ण *path, वर्ण resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
स्थिर पूर्णाङ्क
safe_openat_fallback (पूर्णाङ्क dirfd, ध्रुव वर्ण *rootfs, ध्रुव वर्ण *path, पूर्णाङ्क flags,
                      पूर्णाङ्क mode, libcrun_error_t *err)
{
  cleanup_free वर्ण *parent_path = रिक्त;
  ध्रुव वर्ण *last_component = रिक्त;
  ध्रुव वर्ण *orig_path = path;
  ध्रुव वर्ण *path_in_chroot;
  cleanup_close पूर्णाङ्क fd = -1;
  वर्ण resolved[PATH_MAX];
  वर्ण buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  पूर्णाङ्क ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  सचे (flags & O_NOFOLLOW)
    {
      वर्ण *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      सचे (sep == रिक्त)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      अञ्ञथा सचे (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      सचे (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  सचे (path_in_chroot == रिक्त)
    पटिनिवत्त crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  सचे (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  सचे (last_component)
    {
      ret = snprintf (resolved, परिमाण (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      सचे (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (resolved)))
        पटिनिवत्त crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  सचे (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      पटिनिवत्त ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = fd;
  fd = -1;
  पटिनिवत्त ret;
}

पूर्णाङ्क
safe_openat (पूर्णाङ्क dirfd, ध्रुव वर्ण *rootfs, ध्रुव वर्ण *path, पूर्णाङ्क flags, पूर्णाङ्क mode,
             libcrun_error_t *err)
{
  स्थिर bool openat2_supported = true;
  पूर्णाङ्क retries = MAX_OPENAT2_EAGAIN_RETRIES;
  पूर्णाङ्क ret;

  सचे (is_empty_string (path))
    {
      cleanup_close पूर्णाङ्क fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      सचे (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      सचे (UNLIKELY (fd < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      सचे (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      ret = fd;
      fd = -1;
      पटिनिवत्त ret;
    }

  सचे (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_मूलम्);
      सचे (UNLIKELY (ret < 0))
        {
          सचे (errno == EINTR)
            गच्छ repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          सचे (errno == EAGAIN && retries-- > 0)
            गच्छ repeat;
          सचे (errno == ENOSYS)
            openat2_supported = false;
          सचे (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            पटिनिवत्त safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          पटिनिवत्त crun_make_error (err, errno, "openat2 `%s`", path);
        }

      पटिनिवत्त ret;
    }

  पटिनिवत्त safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (पूर्णाङ्क dfd, ध्रुव वर्ण *name, वर्ण **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free वर्ण *tmp_buf = रिक्त;
  ssize_t size;

  करोहि
    {
      सचे (tmp_buf != रिक्त)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      सचे (UNLIKELY (size < 0))
        पटिनिवत्त crun_make_error (err, errno, "readlink `%s`", name);
  } याव (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = रिक्त;

  पटिनिवत्त size;
}

स्थिर पूर्णाङ्क crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath,
                                ध्रुव वर्ण *path, पूर्णाङ्क mode, पूर्णाङ्क max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
स्थिर पूर्णाङ्क
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath, पूर्णाङ्क cwd,
                                 ध्रुव वर्ण *npath, ध्रुव वर्ण *cur, ध्रुव वर्ण *rest,
                                 पूर्णाङ्क mode, पूर्णाङ्क max_readlinks, libcrun_error_t *err)
{
  cleanup_free वर्ण *target = रिक्त;
  cleanup_free वर्ण *new_path = रिक्त;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (पूर्णाङ्क) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  पटिनिवत्त crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

स्थिर पूर्णाङ्क
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath,
                     ध्रुव वर्ण *path, पूर्णाङ्क mode, पूर्णाङ्क max_readlinks, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क wd_cleanup = -1;
  cleanup_free वर्ण *npath = रिक्त;
  bool last_component = false;
  size_t depth = 0;
  वर्ण *cur;
  वर्ण *it;
  पूर्णाङ्क cwd;
  पूर्णाङ्क ret;

  सचे (max_readlinks <= 0)
    पटिनिवत्त crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  सचे (*path == '\0')
    {
      सचे (do_open)
        पटिनिवत्त open (dirpath, O_CLOEXEC | O_PATH, 0);
      पटिनिवत्त 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  याव (cur)
    {
      सचे (it)
        *it = '\0';
      अञ्ञथा
        last_component = true;

      सचे (cur[0] == '\0')
        भङ्ग;

      सचे (strcmp (cur, ".") == 0)
        गच्छ next;
      अञ्ञथा सचे (strcmp (cur, ".."))
        depth++;
      अञ्ञथा
        {
          सचे (depth)
            depth--;
          अञ्ञथा
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              गच्छ next;
            }
        }

      सचे (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          सचे (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              सचे (errno == ELOOP)
                पटिनिवत्त crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, रिक्त,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              सचे (ret < 0)
                पटिनिवत्त crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          सचे (do_open)
            पटिनिवत्त ret;

          close_and_replace (&wd_cleanup, ret);
          पटिनिवत्त 0;
        }

      ret = mkdirat (cwd, cur, mode);
      सचे (ret < 0)
        {
          mode_t st_mode;

          सचे (errno != EEXIST)
            पटिनिवत्त crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          सचे (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            पटिनिवत्त crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : रिक्त, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      सचे (UNLIKELY (cwd < 0))
        पटिनिवत्त crun_error_wrap (err, "creating `/%s`", path);

      सचे (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, रिक्त);
          सचे (UNLIKELY (ret < 0))
            {
              पूर्णाङ्क saved_errno = errno;

              close (cwd);
              पटिनिवत्त crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          सचे ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              पटिनिवत्त crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      सचे (it == रिक्त)
        भङ्ग;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  सचे (do_open)
    {
      सचे (cwd == dirfd)
        {
          ret = dup (dirfd);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "dup `%s`", dirpath);
          पटिनिवत्त ret;
        }

      wd_cleanup = -1;
      पटिनिवत्त cwd;
    }

  पटिनिवत्त 0;
}

पूर्णाङ्क
crun_safe_create_and_open_ref_at (bool dir, पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath, ध्रुव वर्ण *path, पूर्णाङ्क mode, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  सचे (LIKELY (ret >= 0))
    पटिनिवत्त ret;

  crun_error_release (err);
  पटिनिवत्त crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णाङ्क
crun_safe_ensure_directory_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath, ध्रुव वर्ण *path, पूर्णाङ्क mode,
                               libcrun_error_t *err)
{
  पटिनिवत्त crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णाङ्क
crun_safe_ensure_file_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath, ध्रुव वर्ण *path, पूर्णाङ्क mode,
                          libcrun_error_t *err)
{
  पटिनिवत्त crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णाङ्क
crun_ensure_directory (ध्रुव वर्ण *path, पूर्णाङ्क mode, bool nofollow, libcrun_error_t *err)
{
  पटिनिवत्त crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

स्थिर पूर्णाङ्क
get_file_size (पूर्णाङ्क fd, off_t *size)
{
  संरचना stat st;
  पूर्णाङ्क ret;
#ifdef HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  सचे (UNLIKELY (ret < 0))
    {
      सचे (errno == ENOSYS || errno == EINVAL)
        गच्छ fallback;
      पटिनिवत्त ret;
    }
  *size = stx.stx_size;

  पटिनिवत्त ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  पटिनिवत्त ret;
}

पूर्णाङ्क
crun_dir_p_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  पूर्णाङ्क ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "stat `%s`", path);

  पटिनिवत्त S_ISDIR (mode) ? 1 : 0;
}

पूर्णाङ्क
crun_dir_p (ध्रुव वर्ण *path, bool nofollow, libcrun_error_t *err)
{
  पटिनिवत्त crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
स्थिर पूर्णाङ्क
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free वर्ण *buffer = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  सचे (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      सचे (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          पटिनिवत्त 0;
        }
      पटिनिवत्त ret;
    }

  पटिनिवत्त strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
स्थिर bool
running_in_openvz ()
{
  सचे (access ("/proc/vz", F_OK) < 0)
    पटिनिवत्त false;

  पटिनिवत्त access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

पूर्णाङ्क
check_running_in_user_namespace (libcrun_error_t *err)
{
  स्थिर atomic_int run_in_userns = -1;
  संरचना stat st;
  पूर्णाङ्क ret;

  ret = run_in_userns;
  सचे (ret >= 0)
    पटिनिवत्त ret;

  ret = stat ("/proc/self/ns/user", &st);
  सचे (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      सचे (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          पटिनिवत्त run_in_userns;
        }

      सचे (! running_in_openvz ())
        {
          run_in_userns = 1;
          पटिनिवत्त run_in_userns;
        }
    }
  अञ्ञथा सचे (UNLIKELY (errno != ENOENT))
    पटिनिवत्त crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  run_in_userns = ret;
  पटिनिवत्त run_in_userns;
}

स्थिर size_t
get_page_size ()
{
  स्थिर atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  सचे (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  पटिनिवत्त (size_t) pagesize;
}

स्थिर पूर्णाङ्क selinux_enabled = -1;
स्थिर पूर्णाङ्क apparmor_enabled = -1;

पूर्णाङ्क
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free वर्ण *out = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  size_t len;
  पूर्णाङ्क ret;

  सचे (selinux_enabled >= 0)
    पटिनिवत्त selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  पटिनिवत्त selinux_enabled;
}

पूर्णाङ्क
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क size;
  वर्ण buf[1];

  सचे (apparmor_enabled >= 0)
    पटिनिवत्त apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  सचे (fd == -1)
    {
      सचे (errno == ENOENT)
        {
          apparmor_enabled = 0;
          पटिनिवत्त 0;
        }

      पटिनिवत्त crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (पठ (fd, buf, परिमाण (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  पटिनिवत्त apparmor_enabled;
}

स्थिर पूर्णाङ्क
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  सचे (selinux_enabled < 0)
    पटिनिवत्त crun_make_error (err, 0, "SELinux is not initialized correctly");
  पटिनिवत्त selinux_enabled;
}

पूर्णाङ्क
add_selinux_mount_label (वर्ण **retlabel, ध्रुव वर्ण *data, ध्रुव वर्ण *label, ध्रुव वर्ण *context_type, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_is_selinux_enabled (err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (label && ret)
    {
      सचे (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      अञ्ञथा
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      पटिनिवत्त 0;
    }
  *retlabel = xstrdup (data);
  पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण *
lsm_attr_path (libcrun_container_t *container, ध्रुव वर्ण *lsm, ध्रुव वर्ण *fname, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क attr_dirfd = -1;
  cleanup_close पूर्णाङ्क lsm_dirfd = -1;
  वर्ण *attr_path = रिक्त;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  सचे (UNLIKELY (attr_dirfd < 0))
    पटिनिवत्त रिक्त;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  सचे (lsm != रिक्त)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      सचे (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          पटिनिवत्त रिक्त;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  पटिनिवत्त attr_path;
}

स्थिर पूर्णाङ्क
check_proc_super_magic (पूर्णाङ्क fd, ध्रुव वर्ण *path, libcrun_error_t *err)
{
  संरचना statfs sfs;

  पूर्णाङ्क ret = fstatfs (fd, &sfs);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "statfs `%s`", path);

  सचे (sfs.f_type != PROC_SUPER_MAGIC)
    पटिनिवत्त crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
set_security_attr (libcrun_container_t *container, ध्रुव वर्ण *lsm, ध्रुव वर्ण *fname, ध्रुव वर्ण *data, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  cleanup_free ध्रुव वर्ण *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close पूर्णाङ्क fd = -1;

  सचे (UNLIKELY (attr_path == रिक्त))
    पटिनिवत्त -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (लिख (fd, data, strlen (data)));
  सचे (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      सचे (errno == ENOENT && lsm != रिक्त && strcmp (lsm, "apparmor") == 0)
        {
          ध्रुव वर्ण *profile = strchr (data, ' ');

          पटिनिवत्त crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      पटिनिवत्त crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  पटिनिवत्त 0;
}

पूर्णाङ्क
set_selinux_label (libcrun_container_t *container, ध्रुव वर्ण *label, bool now, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_is_selinux_enabled (err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (ret)
    पटिनिवत्त set_security_attr (container, रिक्त, now ? "current" : "exec", label, err);
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  सचे (apparmor_enabled < 0)
    पटिनिवत्त crun_make_error (err, 0, "AppArmor is not initialized correctly");
  पटिनिवत्त apparmor_enabled;
}

स्थिर पूर्णाङ्क
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (परिमाण (UNCONFINED) - 1)
  cleanup_free ध्रुव वर्ण *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close पूर्णाङ्क fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  वर्ण buf[UNCONFINED_LEN + 1];

  सचे (UNLIKELY (attr_path == रिक्त))
    पटिनिवत्त -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त fd;

  सचे (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    पटिनिवत्त -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (पठ (fd, buf, परिमाण (buf)));
  सचे (UNLIKELY (bytes_read < 0))
    पटिनिवत्त crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  सचे (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    पटिनिवत्त 1;
  सचे (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    पटिनिवत्त 0;
  पटिनिवत्त 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

पूर्णाङ्क
set_apparmor_profile (libcrun_container_t *container, ध्रुव वर्ण *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_is_apparmor_enabled (err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (ret)
    {
      cleanup_free वर्ण *buf = रिक्त;
      ret = is_current_process_confined (container, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      पटिनिवत्त set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
read_all_fd_internal (पूर्णाङ्क fd, ध्रुव वर्ण *description, वर्ण **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free वर्ण *buf = रिक्त;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  पूर्णाङ्क ret;

  सचे (size_hint)
    allocated = size_hint;
  अञ्ञथा
    {
      ret = get_file_size (fd, &size);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "stat `%s`", description);

      सचे (max_size && (size_t) size > max_size)
        पटिनिवत्त crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  याव ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (पठ (fd, buf + nread, allocated - nread));
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "read from file `%s`", description);

      सचे (ret == 0)
        भङ्ग;

      nread += ret;

      सचे (max_size && nread > max_size)
        पटिनिवत्त crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      सचे (nread == allocated)
        {
          सचे (size)
            भङ्ग;

          सचे (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  सचे (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      वर्ण *tmp = realloc (buf, nread + 1);
      सचे (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = रिक्त;
  सचे (len)
    *len = nread;
  पटिनिवत्त 0;
}

पूर्णाङ्क
read_all_fd_with_size_hint (पूर्णाङ्क fd, ध्रुव वर्ण *description, वर्ण **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  पटिनिवत्त read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

पूर्णाङ्क
read_all_fd_with_max_size (पूर्णाङ्क fd, ध्रुव वर्ण *description, वर्ण **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  पटिनिवत्त read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

पूर्णाङ्क
read_all_file_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *path, वर्ण **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", path);

  पटिनिवत्त read_all_fd (fd, path, out, len, err);
}

पूर्णाङ्क
read_all_file (ध्रुव वर्ण *path, वर्ण **out, size_t *len, libcrun_error_t *err)
{
  सचे (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  पटिनिवत्त read_all_file_at (AT_FDCWD, path, out, len, err);
}

पूर्णाङ्क
get_realpath_to_file (पूर्णाङ्क dirfd, ध्रुव वर्ण *path_name, वर्ण **absolute_path, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  सचे (UNLIKELY (targetfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", path_name);
  अञ्ञथा
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      सचे (UNLIKELY (len < 0))
        पटिनिवत्त crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  पटिनिवत्त 0;
}

पूर्णाङ्क
open_unix_domain_client_socket (ध्रुव वर्ण *path, पूर्णाङ्क dgram, libcrun_error_t *err)
{
  संरचना sockaddr_un addr = {};
  पूर्णाङ्क ret;
  proc_fd_path_t name_buf;
  cleanup_close पूर्णाङ्क destfd = -1;
  cleanup_close पूर्णाङ्क fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "create UNIX socket");

  सचे (strlen (path) >= परिमाण (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      सचे (UNLIKELY (destfd < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  सचे (path_len >= परिमाण (addr.sun_path))
    पटिनिवत्त crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (संरचना sockaddr *) &addr, परिमाण (addr));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  पटिनिवत्त ret;
}

पूर्णाङ्क
open_unix_domain_socket (ध्रुव वर्ण *path, पूर्णाङ्क dgram, libcrun_error_t *err)
{
  संरचना sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क dirfd = -1;
  cleanup_close पूर्णाङ्क fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "create UNIX socket");

  सचे (strlen (path) >= परिमाण (addr.sun_path))
    {
      cleanup_free वर्ण *dpath = xstrdup (path);
      cleanup_free वर्ण *bpath = xstrdup (path);
      ध्रुव वर्ण *parent_dir = dirname (dpath);
      ध्रुव वर्ण *base = basename (bpath);
      पूर्णाङ्क n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      सचे (UNLIKELY (dirfd < 0))
        पटिनिवत्त crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, परिमाण (addr.sun_path), "%s/%s", name_buf, base);
      सचे (n < 0 || (size_t) n >= परिमाण (addr.sun_path))
        पटिनिवत्त crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  अञ्ञथा
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (संरचना sockaddr *) &addr, परिमाण (addr));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "bind socket to `%s`", path);

  सचे (! dgram)
    {
      ret = listen (fd, 1);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  पटिनिवत्त ret;
}

पूर्णाङ्क
send_fd_to_socket (पूर्णाङ्क server, पूर्णाङ्क fd, libcrun_error_t *err)
{
  पटिनिवत्त send_fd_to_socket_with_payload (server, fd, रिक्त, 0, err);
}

पूर्णाङ्क
send_fd_to_socket_with_payload (पूर्णाङ्क server, पूर्णाङ्क fd, ध्रुव वर्ण *payload, size_t payload_len, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  संरचना cmsghdr *cmsg = रिक्त;
  संरचना iovec iov[2];
  संरचना msghdr सन्देशः = {};
  वर्ण ctrl_buf[CMSG_SPACE (1 + परिमाण (पूर्णाङ्क))] = {};
  वर्ण data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = परिमाण (data);

  सचे (payload_len > 0)
    {
      iov[0].iov_base = (शून्य *) payload;
      iov[0].iov_len = payload_len;
    }

  सन्देशः.सन्देशः_name = रिक्त;
  सन्देशः.सन्देशः_namelen = 0;
  सन्देशः.सन्देशः_iov = iov;
  सन्देशः.सन्देशः_iovlen = 1;
  सन्देशः.सन्देशः_controllen = CMSG_SPACE (परिमाण (पूर्णाङ्क));
  सन्देशः.सन्देशः_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&सन्देशः);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (परिमाण (पूर्णाङ्क));

  *((पूर्णाङ्क *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &सन्देशः, 0));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "sendmsg");
  पटिनिवत्त 0;
}

पूर्णाङ्क
receive_fd_from_socket_with_payload (पूर्णाङ्क from, वर्ण *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;
  संरचना iovec iov[1];
  संरचना msghdr सन्देशः = {};
  वर्ण ctrl_buf[CMSG_SPACE (परिमाण (पूर्णाङ्क))] = {};
  वर्ण data[1];
  संरचना cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = परिमाण (data);

  सचे (payload_len > 0)
    {
      iov[0].iov_base = (शून्य *) payload;
      iov[0].iov_len = payload_len;
    }

  सन्देशः.सन्देशः_name = रिक्त;
  सन्देशः.सन्देशः_namelen = 0;
  सन्देशः.सन्देशः_iov = iov;
  सन्देशः.सन्देशः_iovlen = 1;
  सन्देशः.सन्देशः_controllen = CMSG_SPACE (परिमाण (पूर्णाङ्क));
  सन्देशः.सन्देशः_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &सन्देशः, 0));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "recvmsg");
  सचे (UNLIKELY (ret == 0))
    पटिनिवत्त crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&सन्देशः);
  सचे (cmsg == रिक्त)
    पटिनिवत्त crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), परिमाण (fd));

  ret = fd;
  fd = -1;
  पटिनिवत्त ret;
}

पूर्णाङ्क
receive_fd_from_socket (पूर्णाङ्क from, libcrun_error_t *err)
{
  पटिनिवत्त receive_fd_from_socket_with_payload (from, रिक्त, 0, err);
}

पूर्णाङ्क
create_socket_pair (पूर्णाङ्क *pair, libcrun_error_t *err)
{
  पूर्णाङ्क ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "socketpair");
  पटिनिवत्त 0;
}

पूर्णाङ्क
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  पूर्णाङ्क ret = signalfd (-1, mask, 0);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "signalfd");
  पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क
epoll_helper_toggle (पूर्णाङ्क epollfd, पूर्णाङ्क fd, पूर्णाङ्क events, libcrun_error_t *err)
{
  संरचना epoll_event ev = {};
  bool add = events != 0;
  पूर्णाङ्क ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  सचे (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  सचे (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      सचे (errno == ENOENT || errno == EPERM)
        पटिनिवत्त 0;
      पटिनिवत्त crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  पटिनिवत्त 0;
}

पूर्णाङ्क
epoll_helper (पूर्णाङ्क *in_fds, पूर्णाङ्क *in_levelfds, पूर्णाङ्क *out_fds, पूर्णाङ्क *out_levelfds, libcrun_error_t *err)
{
  संरचना epoll_event ev;
  cleanup_close पूर्णाङ्क epollfd = -1;
  पूर्णाङ्क ret;

  पूर्णाङ्क *it;
  epollfd = epoll_create1 (0);
  सचे (UNLIKELY (epollfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  अत्थाय (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      सचे (UNLIKELY (ret < 0))                                           \
        पटिनिवत्त crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  सचे (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  सचे (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  सचे (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  सचे (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  पटिनिवत्त ret;
}

पूर्णाङ्क
copy_from_fd_to_fd (पूर्णाङ्क src, पूर्णाङ्क dst, पूर्णाङ्क consume, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  ssize_t nread;
  size_t pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  करोहि
    {
      cleanup_free वर्ण *buffer = रिक्त;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      सचे (can_copy_file_range)
        {
          nread = copy_file_range (src, रिक्त, dst, रिक्त, pagesize, 0);
          सचे (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              गच्छ fallback;
            }
          सचे (consume && nread < 0 && errno == EAGAIN)
            पटिनिवत्त 0;
          सचे (nread < 0 && errno == EIO)
            पटिनिवत्त 0;
          सचे (UNLIKELY (nread < 0))
            पटिनिवत्त crun_make_error (err, errno, "copy_file_range");
          अनुवर्त;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (पठ (src, buffer, pagesize));
      सचे (consume && nread < 0 && errno == EAGAIN)
        पटिनिवत्त 0;
      सचे (nread < 0 && errno == EIO)
        पटिनिवत्त 0;
      सचे (UNLIKELY (nread < 0))
        पटिनिवत्त crun_make_error (err, errno, "read");

      remaining = nread;
      याव (remaining)
        {
          ret = TEMP_FAILURE_RETRY (लिख (dst, buffer + nread - remaining, remaining));
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } याव (consume && nread);

  पटिनिवत्त 0;
}

पूर्णाङ्क
run_process (वर्ण **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  सचे (UNLIKELY (pid < 0))
    पटिनिवत्त crun_make_error (err, errno, "fork");
  सचे (pid)
    {
      पूर्णाङ्क r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      सचे (r < 0)
        पटिनिवत्त crun_make_error (err, errno, "waitpid");
      सचे (WIFEXITED (status) || WIFSIGNALED (status))
        पटिनिवत्त WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
स्थिर unsigned
atou (वर्ण **s)
{
  unsigned x;
  अत्थाय (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  पटिनिवत्त x;
}

पूर्णाङ्क
fgetpwent_r (FILE *f, संरचना passwd *pw, वर्ण *पङ्क्तिः, size_t size, संरचना passwd **res)
{
  वर्ण *s;
  पूर्णाङ्क rv = 0;
  अत्थाय (;;)
    {
      पङ्क्तिः[size - 1] = '\xff';
      सचे ((fgets (पङ्क्तिः, size, f) == रिक्त) || ferror (f) || पङ्क्तिः[size - 1] != '\xff')
        {
          rv = (पङ्क्तिः[size - 1] != '\xff') ? ERANGE : ENOENT;
          पङ्क्तिः = 0;
          pw = 0;
          भङ्ग;
        }
      पङ्क्तिः[strcspn (पङ्क्तिः, "\n")] = 0;

      s = पङ्क्तिः;
      pw->pw_name = s++;
      सचे (! (s = strchr (s, ':')))
        अनुवर्त;

      *s++ = 0;
      pw->pw_passwd = s;
      सचे (! (s = strchr (s, ':')))
        अनुवर्त;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      सचे (*s != ':')
        अनुवर्त;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      सचे (*s != ':')
        अनुवर्त;

      *s++ = 0;
      pw->pw_gecos = s;
      सचे (! (s = strchr (s, ':')))
        अनुवर्त;

      *s++ = 0;
      pw->pw_dir = s;
      सचे (! (s = strchr (s, ':')))
        अनुवर्त;

      *s++ = 0;
      pw->pw_shell = s;
      भङ्ग;
    }
  *res = pw;
  सचे (rv)
    errno = rv;
  पटिनिवत्त rv;
}
#endif

पूर्णाङ्क
set_home_env (uid_t id)
{
  संरचना passwd pwd;
  cleanup_free वर्ण *buf = रिक्त;
  long buf_size;
  cleanup_file FILE *stream = रिक्त;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  सचे (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  सचे (stream == रिक्त)
    पटिनिवत्त -1;

  अत्थाय (;;)
    {
      संरचना passwd *ret_pw = रिक्त;

      पूर्णाङ्क ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      सचे (UNLIKELY (ret != 0))
        {
          सचे (ret == EINTR)
            अनुवर्त;

          सचे (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              पटिनिवत्त -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          अनुवर्त;
        }

      सचे (ret_pw && ret_pw->pw_uid == id)
        {
          सचे (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          अञ्ञथा
            पटिनिवत्त 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
स्थिर पूर्णाङ्क
getsubidrange (uid_t id, पूर्णाङ्क is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *निवेशः = रिक्त;
  cleanup_free वर्ण *lineptr = रिक्त;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free वर्ण *buf = रिक्त;
  ध्रुव वर्ण *name;
  संरचना passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  सचे (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  अत्थाय (;;)
    {
      पूर्णाङ्क ret;
      संरचना passwd *ret_pw = रिक्त;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      सचे (LIKELY (ret == 0))
        {
          सचे (ret_pw)
            {
              name = ret_pw->pw_name;
              भङ्ग;
            }
          पटिनिवत्त -1;
        }

      सचे (ret != ERANGE)
        पटिनिवत्त -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  निवेशः = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  सचे (निवेशः == रिक्त)
    पटिनिवत्त -1;

  अत्थाय (;;)
    {
      वर्ण *endptr;
      ssize_t पठ = getline (&lineptr, &lenlineptr, निवेशः);
      सचे (पठ < 0)
        पटिनिवत्त -1;

      सचे (पठ < (ssize_t) (len_name + 2))
        अनुवर्त;

      सचे (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        अनुवर्त;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      सचे (endptr >= &lineptr[पठ])
        पटिनिवत्त -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      पटिनिवत्त 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

पूर्णाङ्क
format_default_id_mapping (वर्ण **out, uid_t container_id, uid_t host_uid, uid_t host_id, पूर्णाङ्क is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free वर्ण *buffer = रिक्त;
  पूर्णाङ्क written = 0;
  पूर्णाङ्क ret, remaining;

  *out = रिक्त;

  सचे (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    पटिनिवत्त 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  सचे (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      सचे (UNLIKELY (ret >= remaining))
        पटिनिवत्त crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  सचे (UNLIKELY (ret >= remaining))
    पटिनिवत्त crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  सचे (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      सचे (UNLIKELY (ret >= remaining))
        पटिनिवत्त crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = रिक्त;
  पटिनिवत्त written;
}

स्थिर पूर्णाङ्क
unset_cloexec_flag (पूर्णाङ्क fd)
{
  पूर्णाङ्क flags = fcntl (fd, F_GETFD);
  सचे (flags == -1)
    पटिनिवत्त -1;

  flags &= ~FD_CLOEXEC;

  पटिनिवत्त fcntl (fd, F_SETFD, flags);
}

स्थिर शून्य __attribute__ ((__noreturn__))
run_process_child (वर्ण *path, वर्ण **args, ध्रुव वर्ण *cwd, वर्ण **envp, पूर्णाङ्क pipe_r,
                   पूर्णाङ्क pipe_w, पूर्णाङ्क out_fd, पूर्णाङ्क err_fd, bool can_ignore_chdir_errors)
{
  वर्ण *tmp_args[] = { path, रिक्त };
  libcrun_error_t err = रिक्त;
  पूर्णाङ्क dev_null_fd = -1;
  पूर्णाङ्क ret;

  ret = mark_or_close_fds_ge_than (रिक्त, 3, false, &err);
  सचे (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->सन्देशः);

  सचे (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      सचे (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  सचे (out_fd >= 0)
    unset_cloexec_flag (1);
  सचे (err_fd >= 0)
    unset_cloexec_flag (2);

  सचे (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  सचे (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  सचे (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  सचे (args == रिक्त)
    args = tmp_args;

  सचे (cwd && chdir (cwd) < 0)
    {
      सचे (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
पूर्णाङ्क
run_process_with_stdin_timeout_envp (वर्ण *path, वर्ण **args, ध्रुव वर्ण *cwd, पूर्णाङ्क timeout,
                                     वर्ण **envp, वर्ण *stdin, size_t stdin_len, पूर्णाङ्क out_fd,
                                     पूर्णाङ्क err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  पूर्णाङ्क stdin_pipe[2];
  pid_t pid;
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क pipe_r = -1;
  cleanup_close पूर्णाङ्क pipe_w = -1;
  sigset_t oldmask, mask;
  पूर्णाङ्क r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  सचे (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  सचे (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      गच्छ restore_sig_mask_and_exit;
    }

  सचे (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (लिख (pipe_w, stdin, stdin_len));
  सचे (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      सचे (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          गच्छ restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  सचे (timeout)
    {
      time_t start = time (रिक्त);
      time_t now;
      अत्थाय (now = start; now - start < timeout; now = time (रिक्त))
        {
          siginfo_t info;
          पूर्णाङ्क elapsed = now - start;
          संरचना timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          सचे (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              गच्छ restore_sig_mask_and_exit;
            }

          सचे (info.si_signo == SIGCHLD && info.si_pid == pid)
            गच्छ read_waitpid;

          सचे (ret < 0 && errno == EAGAIN)
            गच्छ timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      गच्छ restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  सचे (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  अञ्ञथा
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  सचे (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      सचे (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, रिक्त);
      सचे (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  पटिनिवत्त ret;
}

पूर्णाङ्क
mark_or_close_fds_ge_than (libcrun_container_t *container, पूर्णाङ्क n, bool close_now, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क cfd = -1;
  cleanup_dir DIR *dir = रिक्त;
  पूर्णाङ्क ret;
  पूर्णाङ्क fd;
  संरचना dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  सचे (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  सचे (ret == 0)
    {
      सचे (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      पटिनिवत्त 0;
    }
  सचे (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    पटिनिवत्त crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  सचे (UNLIKELY (cfd < 0))
    पटिनिवत्त cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  dir = fdopendir (cfd);
  सचे (UNLIKELY (dir == रिक्त))
    पटिनिवत्त crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  अत्थाय (next = readdir (dir); next; next = readdir (dir))
    {
      पूर्णाङ्क val;
      ध्रुव वर्ण *name = next->d_name;
      सचे (name[0] == '.')
        अनुवर्त;

      val = strtoll (name, रिक्त, 10);
      सचे (val < n || val == fd)
        अनुवर्त;

      सचे (close_now)
        {
          ret = close (val);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "close fd `%d`", val);

          सचे (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      अञ्ञथा
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  पटिनिवत्त 0;
}

शून्य
get_current_timestamp (वर्ण *out, size_t len)
{
  संरचना timeval tv;
  संरचना tm now;
  वर्ण timestamp[64];

  gettimeofday (&tv, रिक्त);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, परिमाण (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (शून्य) snprintf (out, len, "%s.%06lldZ", timestamp, (long long पूर्णाङ्क) tv.tv_usec);
  out[len - 1] = '\0';
}

पूर्णाङ्क
set_blocking_fd (पूर्णाङ्क fd, bool blocking, libcrun_error_t *err)
{
  पूर्णाङ्क ret, flags = fcntl (fd, F_GETFL, 0);
  सचे (UNLIKELY (flags < 0))
    पटिनिवत्त crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fcntl");
  पटिनिवत्त 0;
}

पूर्णाङ्क
parse_json_file (json_object **out, ध्रुव वर्ण *jsondata, संरचना parser_context *ctx arg_unused, libcrun_error_t *err)
{
  संरचना json_tokener *tok;
  गणना json_tokener_error jerr;

  *err = रिक्त;

  सचे (UNLIKELY (jsondata == रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  सचे (UNLIKELY (tok == रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (पूर्णाङ्क) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  सचे (*out == रिक्त || jerr != json_tokener_success)
    {
      पूर्णाङ्क saved_pos = (पूर्णाङ्क) tok->char_offset;

      json_tokener_free (tok);
      सचे (*out)
        {
          json_object_put (*out);
          *out = रिक्त;
        }
      पटिनिवत्त crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  पटिनिवत्त 0;
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
स्थिर पूर्णाङ्क
check_access (ध्रुव वर्ण *path)
{
  पूर्णाङ्क ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#अञ्ञथा
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  सचे (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      सचे (errno == EACCES)
        {
          पूर्णाङ्क saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          सचे (ret == 0)
            पटिनिवत्त CHECK_ACCESS_NOT_EXECUTABLE;
        }
      पटिनिवत्त -errno;
    }

  ret = get_file_type (&mode, false, path);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त -errno;

  सचे (! S_ISREG (mode))
    पटिनिवत्त CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  पटिनिवत्त 0;
}

पूर्णाङ्क
find_executable (वर्ण **out, ध्रुव वर्ण *executable_path, ध्रुव वर्ण *cwd, libcrun_error_t *err)
{
  cleanup_free वर्ण *cwd_executable_path = रिक्त;
  cleanup_free वर्ण *tmp = रिक्त;
  पूर्णाङ्क last_error = -ENOENT;
  वर्ण *it, *end;
  पूर्णाङ्क ret;

  *out = रिक्त;

  सचे (executable_path == रिक्त || executable_path[0] == '\0')
    पटिनिवत्त crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  सचे (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free वर्ण *cwd_allocated = रिक्त;

      सचे (cwd == रिक्त)
        {
          cwd_allocated = getcwd (रिक्त, 0);
          सचे (cwd_allocated == रिक्त)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  सचे (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      सचे (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          पटिनिवत्त 0;
        }
      last_error = ret;
      गच्छ fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  याव ((it = strsep (&end, ":")))
    {
      cleanup_free वर्ण *path = रिक्त;

      सचे (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, रिक्त);
      सचे (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          अनुवर्त;
        }

      ret = check_access (path);
      सचे (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = रिक्त;
          पटिनिवत्त 0;
        }

      सचे (ret == -ENOENT)
        अनुवर्त;

      last_error = ret;
    }

fail:
  विकल्प (last_error)
    {
    पक्ष CHECK_ACCESS_NOT_EXECUTABLE:
      पटिनिवत्त crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    पक्ष CHECK_ACCESS_NOT_REGULAR:
      पटिनिवत्त crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    स्वभाव:
      errno = -last_error;
      सचे (errno == ENOENT)
        पटिनिवत्त crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      पटिनिवत्त crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

स्थिर ssize_t
safe_read_xattr (वर्ण **ret, पूर्णाङ्क sfd, ध्रुव वर्ण *srcname, ध्रुव वर्ण *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free वर्ण *buffer = रिक्त;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  याव (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      सचे (UNLIKELY (s < 0))
        पटिनिवत्त crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      सचे (s < current_size)
        भङ्ग;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  सचे (s <= 0)
    पटिनिवत्त s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = रिक्त;

  पटिनिवत्त s;
}

स्थिर ssize_t
copy_xattr (पूर्णाङ्क sfd, पूर्णाङ्क dfd, ध्रुव वर्ण *srcname, ध्रुव वर्ण *destname, libcrun_error_t *err)
{
  cleanup_free वर्ण *buf = रिक्त;
  ssize_t xattr_len;
  वर्ण *it;

  xattr_len = flistxattr (sfd, रिक्त, 0);
  सचे (UNLIKELY (xattr_len < 0))
    {
      सचे (errno == ENOTSUP)
        पटिनिवत्त 0;

      पटिनिवत्त crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  सचे (xattr_len == 0)
    पटिनिवत्त 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  सचे (UNLIKELY (xattr_len < 0))
    पटिनिवत्त crun_make_error (err, errno, "flistxattr `%s`", srcname);

  अत्थाय (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free वर्ण *v = रिक्त;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      सचे (UNLIKELY (s < 0))
        पटिनिवत्त s;

      s = fsetxattr (dfd, it, v, s, 0);
      सचे (UNLIKELY (s < 0))
        {
          सचे (errno == EINVAL || errno == EOPNOTSUPP)
            अनुवर्त;

          पटिनिवत्त crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  पटिनिवत्त 0;
}

#endif

स्थिर पूर्णाङ्क
copy_rec_stat_file_at (पूर्णाङ्क dfd, ध्रुव वर्ण *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  संरचना stat st;
  पूर्णाङ्क ret;

#ifdef HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  सचे (UNLIKELY (ret < 0))
    {
      सचे (errno == ENOSYS || errno == EINVAL)
        गच्छ fallback;

      पटिनिवत्त ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  पटिनिवत्त ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  पटिनिवत्त ret;
}

पूर्णाङ्क
copy_recursive_fd_to_fd (पूर्णाङ्क srcdirfd, पूर्णाङ्क dfd, ध्रुव वर्ण *srcname, ध्रुव वर्ण *destname, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = रिक्त;
  संरचना dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  सचे (UNLIKELY (dsrcfd == रिक्त))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      पटिनिवत्त crun_make_error (err, errno, "open directory `%s`", destname);
    }

  अत्थाय (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close पूर्णाङ्क srcfd = -1;
      cleanup_close पूर्णाङ्क destfd = -1;
      cleanup_free वर्ण *target_buf = रिक्त;
      पूर्णाङ्क ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      सचे (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        अनुवर्त;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      विकल्प (mode & S_IFMT)
        {
        पक्ष S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          सचे (UNLIKELY (srcfd < 0))
            पटिनिवत्त crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          सचे (UNLIKELY (destfd < 0))
            पटिनिवत्त crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

#ifdef HAVE_FGETXATTR
          ret = (पूर्णाङ्क) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          भङ्ग;

        पक्ष S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          सचे (UNLIKELY (srcfd < 0))
            पटिनिवत्त crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          सचे (UNLIKELY (destfd < 0))
            पटिनिवत्त crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (पूर्णाङ्क) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          भङ्ग;

        पक्ष S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          भङ्ग;

        पक्ष S_IFBLK:
        पक्ष S_IFCHR:
        पक्ष S_IFIFO:
        पक्ष S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          भङ्ग;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      सचे (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          सचे (errno == ENOTSUP)
            अनुवर्त;

          पटिनिवत्त crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  पटिनिवत्त 0;
}

ध्रुव वर्ण *
find_annotation (libcrun_container_t *container, ध्रुव वर्ण *name)
{
  सचे (container->container_def->annotations == रिक्त)
    पटिनिवत्त रिक्त;

  पटिनिवत्त find_string_map_मानम् (container->annotations, name);
}

पूर्णाङ्क
safe_write (पूर्णाङ्क fd, ध्रुव वर्ण *fname, ध्रुव शून्य *buf, size_t गणनम्, libcrun_error_t *err)
{
  size_t written = 0;
  याव (written < गणनम्)
    {
      ssize_t w = लिख (fd, (ध्रुव वर्ण *) buf + written, गणनम् - written);
      सचे (UNLIKELY (w < 0))
        {
          सचे (errno == EINTR || errno == EAGAIN)
            अनुवर्त;
          पटिनिवत्त crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  पटिनिवत्त 0;
}

#define MAX_PARTS 32

पूर्णाङ्क
append_paths (वर्ण **out, libcrun_error_t *err, ...)
{
  ध्रुव वर्ण *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t total_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  अत्थाय (;;)
    {
      ध्रुव वर्ण *part;
      size_t size;

      part = va_arg (ap, ध्रुव वर्ण *);
      सचे (part == रिक्त)
        भङ्ग;

      सचे (n_parts == MAX_PARTS)
        {
          va_end (ap);
          पटिनिवत्त crun_make_error (err, EINVAL, "too many paths specified");
        }

      सचे (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          याव (part[0] == '/' && part[1] == '/')
            part++;
        }
      अञ्ञथा
        {
          /* And drop any initial '/' for other components.  */
          याव (part[0] == '/')
            part++;
        }

      size = strlen (part);
      सचे (size == 0)
        अनुवर्त;

      याव (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  अत्थाय (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  अत्थाय (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      सचे (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  पटिनिवत्त 0;
}

#सचे __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#अञ्ञथा
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
स्थिर पूर्णाङ्क
b64_निवेशः (वर्ण c)
{
  ध्रुव वर्ण table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  पूर्णाङ्क i;

  अत्थाय (i = 0; i < 64; i++)
    {
      सचे (table[i] == c)
        पटिनिवत्त i;
    }
  पटिनिवत्त -1;
}

पूर्णाङ्क
base64_decode (ध्रुव वर्ण *iptr, size_t isize, वर्ण *optr, size_t osize, size_t *nbytes)
{
  पूर्णाङ्क i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned वर्ण data[4];

  *nbytes = 0;
  याव (consumed < isize && (*nbytes) + 3 < osize)
    {
      याव ((i < 4) && (consumed < isize))
        {
          tmp = b64_निवेशः (*iptr++);
          consumed++;
          सचे (tmp != -1)
            {
              सचे (UNLIKELY (pad > 0))
                पटिनिवत्त -1;
              data[i++] = tmp;
            }
          अञ्ञथा सचे (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      सचे (i == 4)
        {
          सचे (UNLIKELY (pad > 2))
            पटिनिवत्त -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      अञ्ञथा
        {
          /* I did not get all the data.  */
          consumed -= i;
          पटिनिवत्त consumed;
        }
      i = 0;
    }
  पटिनिवत्त consumed;
}

वर्ण *
get_user_name (uid_t uid)
{
  संरचना passwd pd;
  संरचना passwd *temp_result_ptr;
  वर्ण pwdbuffer[200];
  सचे (! getpwuid_r (uid, &pd, pwdbuffer, परिमाण (pwdbuffer), &temp_result_ptr))
    पटिनिवत्त xstrdup (pd.pw_name);
  पटिनिवत्त xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
वर्ण *
libcrun_sysctl_कुञ्चिका_to_proc_path (ध्रुव वर्ण *कुञ्चिका)
{
  ध्रुव वर्ण *sep;
  वर्ण *ret, *it;

  sep = strpbrk (कुञ्चिका, "./");

  /* No separator at all, or the key is already in the path form.  */
  सचे (sep == रिक्त || *sep == '/')
    पटिनिवत्त xstrdup (कुञ्चिका);

  ret = xstrdup (कुञ्चिका);
  अत्थाय (it = ret; *it; it++)
    {
      सचे (*it == '.')
        *it = '/';
      अञ्ञथा सचे (*it == '/')
        *it = '.';
    }

  पटिनिवत्त ret;
}

पूर्णाङ्क
has_suffix (ध्रुव वर्ण *str, ध्रुव वर्ण *suffix)
{
  सचे (! str || ! suffix)
    पटिनिवत्त 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  सचे (lensuffix > lenstr)
    पटिनिवत्त 0;
  पटिनिवत्त memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

वर्ण *
str_join_array (पूर्णाङ्क offset, size_t size, वर्ण *ध्रुव array[], ध्रुव वर्ण *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = रिक्त;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  वर्ण *result, *p;

  lens = xmalloc (size * परिमाण (*lens));

  अत्थाय (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  अत्थाय (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      सचे (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  पटिनिवत्त result;
}

पूर्णाङ्क
libcrun_mmap (संरचना libcrun_mmap_s **ret, शून्य *addr, size_t length,
              पूर्णाङ्क prot, पूर्णाङ्क flags, पूर्णाङ्क fd, off_t offset,
              libcrun_error_t *err)
{
  संरचना libcrun_mmap_s *mmap_s = रिक्त;

  शून्य *mapped = mmap (addr, length, prot, flags, fd, offset);
  सचे (mapped == MAP_FAILED)
    पटिनिवत्त crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (परिमाण (संरचना libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_munmap (संरचना libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = munmap (mmap->addr, mmap->length);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "munmap");

  मुक्त (mmap);

  पटिनिवत्त 0;
}

स्थिर long
read_file_as_long_or_default (ध्रुव वर्ण *path, long def_मानम्)
{
  cleanup_free वर्ण *content = रिक्त;
  libcrun_error_t tmp_err = रिक्त;
  वर्ण *endptr;
  long val;
  पूर्णाङ्क ret;

  ret = read_all_file (path, &content, रिक्त, &tmp_err);
  सचे (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      पटिनिवत्त def_मानम्;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  सचे (UNLIKELY (errno))
    पटिनिवत्त def_मानम्;
  सचे (endptr == content || (*endptr && *endptr != '\n'))
    पटिनिवत्त def_मानम्;
  पटिनिवत्त val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (शून्य)
{
  स्थिर atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  सचे (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  पटिनिवत्त uid;
}

gid_t
get_overflow_gid (शून्य)
{
  स्थिर atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  सचे (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  पटिनिवत्त gid;
}

शून्य
consume_trailing_slashes (वर्ण *path)
{
  सचे (! path || path[0] == '\0')
    पटिनिवत्त;

  वर्ण *last = path + strlen (path);

  याव (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

वर्ण **
read_dir_entries (ध्रुव वर्ण *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = रिक्त;
  size_t n_entries = 0;
  size_t entries_size = 16;
  वर्ण **entries = रिक्त;
  संरचना dirent *de;

  dir = opendir (path);
  सचे (UNLIKELY (dir == रिक्त))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      पटिनिवत्त रिक्त;
    }

  entries = xmalloc (entries_size * परिमाण (वर्ण *));
  याव ((de = readdir (dir)))
    {
      सचे (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        अनुवर्त;
      सचे (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * परिमाण (वर्ण *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * परिमाण (वर्ण *));
  entries[n_entries] = रिक्त;

  पटिनिवत्त entries;
}

पूर्णाङ्क
cpuset_string_to_bitmask (ध्रुव वर्ण *str, वर्ण **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free वर्ण *mask = रिक्त;
  size_t mask_size = 0;
  ध्रुव वर्ण *p = str;
  वर्ण *endptr;

  याव (*p)
    {
      long long start_range, end_range;

      सचे (*p < '0' || *p > '9')
        गच्छ invalid_निवेशः;

      start_range = strtoll (p, &endptr, 10);
      सचे (start_range < 0)
        गच्छ invalid_निवेशः;

      p = endptr;

      सचे (*p != '-')
        end_range = start_range;
      अञ्ञथा
        {
          p++;

          सचे (*p < '0' || *p > '9')
            गच्छ invalid_निवेशः;

          end_range = strtoll (p, &endptr, 10);

          सचे (end_range < start_range)
            गच्छ invalid_निवेशः;

          p = endptr;
        }

      /* Just set some limit.  */
      सचे (end_range > (1 << 20))
        गच्छ invalid_निवेशः;

      सचे (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      अत्थाय (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      सचे (*p == ',')
        p++;
      अञ्ञथा सचे (*p)
        गच्छ invalid_निवेशः;
    }

  *out = mask;
  mask = रिक्त;
  *out_size = mask_size;

  पटिनिवत्त 0;

invalid_निवेशः:
  पटिनिवत्त crun_make_error (err, 0, "cannot parse input `%s`", str);
}

संरचना channel_fd_pair
{
  संरचना ring_buffer *rb;

  पूर्णाङ्क in_fd;
  पूर्णाङ्क out_fd;

  पूर्णाङ्क infd_epoll_events;
  पूर्णाङ्क outfd_epoll_events;
};

संरचना channel_fd_pair *
channel_fd_pair_new (पूर्णाङ्क in_fd, पूर्णाङ्क out_fd, size_t size)
{
  संरचना channel_fd_pair *channel = xmalloc (परिमाण (संरचना channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  पटिनिवत्त channel;
}

शून्य
channel_fd_pair_free (संरचना channel_fd_pair *channel)
{
  सचे (channel == रिक्त)
    पटिनिवत्त;

  ring_buffer_free (channel->rb);
  मुक्त (channel);
}

पूर्णाङ्क
channel_fd_pair_process (संरचना channel_fd_pair *channel, पूर्णाङ्क epollfd, libcrun_error_t *err)
{
  bool is_निवेशः_eagain = false, is_output_eagain = false, repeat;
  पूर्णाङ्क ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  अत्थाय (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      सचे (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_निवेशः_eagain, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          सचे (ret > 0)
            repeat = true;
        }
      सचे (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          सचे (ret > 0)
            repeat = true;
        }
    }

  सचे (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      पूर्णाङ्क events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_निवेशः_eagain ? EPOLLET : 0)) : 0;
      सचे (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      सचे (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          channel->outfd_epoll_events = events;
        }
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
open_proc (libcrun_error_t *err)
{
  पूर्णाङ्क fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close पूर्णाङ्क fsfd = -1;
  पूर्णाङ्क ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  सचे (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, रिक्त, रिक्त, 0);
      सचे (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          सचे (fd >= 0)
            पटिनिवत्त fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  सचे (fd < 0)
    पटिनिवत्त crun_make_error (err, errno, "open `/proc`");
  पटिनिवत्त fd;
}

पूर्णाङ्क
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  सचे (! container)
    पटिनिवत्त open_proc (err);

  सचे (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  पटिनिवत्त container->proc_fd;
}

पूर्णाङ्क
libcrun_open_proc_file (libcrun_container_t *container, ध्रुव वर्ण *path, पूर्णाङ्क flags, libcrun_error_t *err)
{
  पूर्णाङ्क proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  सचे (proc_fd < 0)
    पटिनिवत्त proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  सचे (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  पटिनिवत्त fd;
}

पूर्णाङ्क
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ध्रुव वर्ण *path, पूर्णाङ्क flags, libcrun_error_t *err)
{
  cleanup_free वर्ण *full_path = रिक्त;
  xasprintf (&full_path, "%d/%s", pid, path);
  पटिनिवत्त libcrun_open_proc_file (container, full_path, flags, err);
}
