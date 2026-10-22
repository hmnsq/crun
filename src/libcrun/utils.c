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
#जर HAVE_STDATOMIC_H
#  include <stdatomic.h>
#अन्यथा
#  define atomic_long अस्थिर दीर्घ
#  define atomic_int अस्थिर पूर्णांक
#endif
#अन्तर्भाव "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_मूळ
#  define RESOLVE_IN_मूळ 0x10
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

पूर्णांक
crun_path_exists (स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  पूर्णांक ret = access (path, F_OK);
  जर (ret < 0)
    {
      जर (त्रुटि_सं == ENOENT)
        परत 0;
      परत crun_make_error (err, त्रुटि_सं, "access `%s`", path);
    }
  परत 1;
}

पूर्णांक
xasprintf (अक्षर **str, स्थिरांक अक्षर *fmt, ...)
{
  पूर्णांक ret;
  बहु_सूची args_list;

  बहु_शुरू (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  जर (UNLIKELY (ret < 0))
    OOM ();

  बहु_पूर्ण (args_list);
  परत ret;
}

पूर्णांक
write_file_at_with_flags (पूर्णांक dirfd, पूर्णांक flags, mode_t mode, स्थिरांक अक्षर *name, स्थिरांक शून्य *data, माप_प्रकार len, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  जर (UNLIKELY (fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत (len > पूर्णांक_उच्च) ? पूर्णांक_उच्च : (पूर्णांक) len;
}

पूर्णांक
detach_process ()
{
  pid_t pid;
  जर (setsid () < 0)
    परत -1;
  pid = विभाजन ();
  जर (pid < 0)
    परत -1;
  जर (pid != 0)
    _safe_exit (निकास_सफल);
  परत 0;
}

पूर्णांक
get_file_type_fd (पूर्णांक fd, mode_t *mode)
{
  संरचना stat st;
  पूर्णांक ret;

#ifdef HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  जर (UNLIKELY (ret < 0))
    {
      जर (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        जा fallback;

      परत ret;
    }
  *mode = stx.stx_mode;
  परत ret;

fallback:
#endif
  ret = ख_स्थिति (fd, &st);
  *mode = st.st_mode;
  परत ret;
}

पूर्णांक
get_file_type_at (पूर्णांक dirfd, mode_t *mode, bool nofollow, स्थिरांक अक्षर *path)
{
  पूर्णांक empty_path = path == रिक्त ? AT_EMPTY_PATH : 0;
  संरचना stat st;
  पूर्णांक ret;

#ifdef HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  जर (UNLIKELY (ret < 0))
    {
      जर (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        जा fallback;

      परत ret;
    }
  *mode = stx.stx_mode;
  परत ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  परत ret;
}

पूर्णांक
get_file_type (mode_t *mode, bool nofollow, स्थिरांक अक्षर *path)
{
  परत get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

पूर्णांक
create_file_if_missing_at (पूर्णांक dirfd, स्थिरांक अक्षर *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  जर (fd_write < 0)
    {
      पूर्णांक saved_errno = त्रुटि_सं;
      mode_t tmp_mode;
      पूर्णांक ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      जर (ret == 0 && S_ISREG (tmp_mode))
        परत 0;

      परत crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  परत 0;
}

स्थिर पूर्णांक
ensure_directory_internal_at (पूर्णांक dirfd, अक्षर *path, माप_प्रकार len, पूर्णांक mode, libcrun_error_t *err)
{
  अक्षर *it = path + len;
  पूर्णांक ret = 0;
  bool parent_created = false;

  साठी (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      जर (ret == 0 || त्रुटि_सं == EEXIST)
        परत 0;

      पूर्णांक saved_errno = त्रुटि_सं;
      जर (parent_created || त्रुटि_सं != ENOENT)
        {
          libcrun_error_t tmp_err = रिक्त;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          जर (ret > 0)
            परत 0;
          जर (ret < 0)
            crun_error_release (&tmp_err);

          परत crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      पर्यंत (it > path && *it != '/')
        {
          it--;
          len--;
        }
      जर (it == path)
        परत 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      जर (UNLIKELY (ret < 0))
        परत ret;

      parent_created = true;
    }
  परत ret;
}

पूर्णांक
crun_ensure_directory_at (पूर्णांक dirfd, स्थिरांक अक्षर *path, पूर्णांक mode, bool nofollow, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, म_माप (tmp), mode, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (ret == 0)
    परत crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  परत 0;
}

स्थिर पूर्णांक
check_fd_is_path (स्थिरांक अक्षर *path, पूर्णांक fd, स्थिरांक अक्षर *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  माप_प्रकार path_len = म_माप (path);
  अक्षर link[PATH_MAX];
  पूर्णांक ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, आकार (link)));
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "readlink `%s`", fdname);

  जर (((माप_प्रकार) ret) != path_len || स_भेद (link, path, path_len))
    परत crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  परत 0;
}

स्थिर पूर्णांक
check_fd_under_path (स्थिरांक अक्षर *rootfs, माप_प्रकार rootfslen, पूर्णांक fd, स्थिरांक अक्षर *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  अक्षर link[PATH_MAX];
  पूर्णांक ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  जर (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    परत 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, आकार (link)));
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "readlink `%s`", fdname);

  जर (((माप_प्रकार) ret) <= rootfslen || स_भेद (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    परत crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  परत 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
स्थिर शून्य
close_and_replace (पूर्णांक *oldfd, पूर्णांक newfd)
{
  जर (*oldfd >= 0)
    TEMP_FAILURE_RETRY (बंद (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
अक्षर *chroot_realpath (स्थिरांक अक्षर *chroot, स्थिरांक अक्षर *path, अक्षर resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
स्थिर पूर्णांक
safe_openat_fallback (पूर्णांक dirfd, स्थिरांक अक्षर *rootfs, स्थिरांक अक्षर *path, पूर्णांक flags,
                      पूर्णांक mode, libcrun_error_t *err)
{
  cleanup_free अक्षर *parent_path = रिक्त;
  स्थिरांक अक्षर *last_component = रिक्त;
  स्थिरांक अक्षर *orig_path = path;
  स्थिरांक अक्षर *path_in_chroot;
  cleanup_close पूर्णांक fd = -1;
  अक्षर resolved[PATH_MAX];
  अक्षर buffer[PATH_MAX];
  माप_प्रकार rootfs_len = safe_strlen (rootfs);
  पूर्णांक ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  जर (flags & O_NOFOLLOW)
    {
      अक्षर *sep;

      parent_path = xstrdup (path);
      sep = म_खोजप (parent_path, '/');
      जर (sep == रिक्त)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      अन्यथा जर (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      जर (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  जर (path_in_chroot == रिक्त)
    परत crun_make_error (err, त्रुटि_सं, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  जर (rootfs_len > 0 && म_भेदन (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  जर (last_component)
    {
      ret = snprintf (resolved, आकार (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      जर (UNLIKELY (ret >= (पूर्णांक) आकार (resolved)))
        परत crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  जर (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "dup `%s`", rootfs_len ? rootfs : "/");
      परत ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = fd;
  fd = -1;
  परत ret;
}

पूर्णांक
safe_openat (पूर्णांक dirfd, स्थिरांक अक्षर *rootfs, स्थिरांक अक्षर *path, पूर्णांक flags, पूर्णांक mode,
             libcrun_error_t *err)
{
  स्थिर bool openat2_supported = true;
  पूर्णांक retries = MAX_OPENAT2_EAGAIN_RETRIES;
  पूर्णांक ret;

  जर (is_empty_string (path))
    {
      cleanup_close पूर्णांक fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      जर (is_empty_string (rootfs))
        rootfs = "/";

      fd = खोलो (rootfs, flags, mode);
      जर (UNLIKELY (fd < 0))
        परत crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      जर (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }

      ret = fd;
      fd = -1;
      परत ret;
    }

  जर (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_मूळ);
      जर (UNLIKELY (ret < 0))
        {
          जर (त्रुटि_सं == EINTR)
            जा repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          जर (त्रुटि_सं == EAGAIN && retries-- > 0)
            जा repeat;
          जर (त्रुटि_सं == ENOSYS)
            openat2_supported = false;
          जर (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL || त्रुटि_सं == EPERM)
            परत safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          परत crun_make_error (err, त्रुटि_सं, "openat2 `%s`", path);
        }

      परत ret;
    }

  परत safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (पूर्णांक dfd, स्थिरांक अक्षर *name, अक्षर **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free अक्षर *tmp_buf = रिक्त;
  ssize_t size;

  करा
    {
      जर (tmp_buf != रिक्त)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      जर (UNLIKELY (size < 0))
        परत crun_make_error (err, त्रुटि_सं, "readlink `%s`", name);
  } पर्यंत (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = रिक्त;

  परत size;
}

स्थिर पूर्णांक crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, पूर्णांक dirfd, स्थिरांक अक्षर *dirpath,
                                स्थिरांक अक्षर *path, पूर्णांक mode, पूर्णांक max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
स्थिर पूर्णांक
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, पूर्णांक dirfd, स्थिरांक अक्षर *dirpath, पूर्णांक cwd,
                                 स्थिरांक अक्षर *npath, स्थिरांक अक्षर *cur, स्थिरांक अक्षर *rest,
                                 पूर्णांक mode, पूर्णांक max_readlinks, libcrun_error_t *err)
{
  cleanup_free अक्षर *target = रिक्त;
  cleanup_free अक्षर *new_path = रिक्त;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (पूर्णांक) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  परत crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

स्थिर पूर्णांक
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, पूर्णांक dirfd, स्थिरांक अक्षर *dirpath,
                     स्थिरांक अक्षर *path, पूर्णांक mode, पूर्णांक max_readlinks, libcrun_error_t *err)
{
  cleanup_close पूर्णांक wd_cleanup = -1;
  cleanup_free अक्षर *npath = रिक्त;
  bool last_component = false;
  माप_प्रकार depth = 0;
  अक्षर *cur;
  अक्षर *it;
  पूर्णांक cwd;
  पूर्णांक ret;

  जर (max_readlinks <= 0)
    परत crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  जर (*path == '\0')
    {
      जर (do_open)
        परत खोलो (dirpath, O_CLOEXEC | O_PATH, 0);
      परत 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = म_अक्षर (npath, '/');
  पर्यंत (cur)
    {
      जर (it)
        *it = '\0';
      अन्यथा
        last_component = true;

      जर (cur[0] == '\0')
        थांबा;

      जर (म_भेद (cur, ".") == 0)
        जा next;
      अन्यथा जर (म_भेद (cur, ".."))
        depth++;
      अन्यथा
        {
          जर (depth)
            depth--;
          अन्यथा
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              जा next;
            }
        }

      जर (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          जर (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              जर (त्रुटि_सं == ELOOP)
                परत crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, रिक्त,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              जर (ret < 0)
                परत crun_make_error (err, त्रुटि_सं, "open `%s/%s`", dirpath, npath);
            }

          जर (do_open)
            परत ret;

          close_and_replace (&wd_cleanup, ret);
          परत 0;
        }

      ret = mkdirat (cwd, cur, mode);
      जर (ret < 0)
        {
          mode_t st_mode;

          जर (त्रुटि_सं != EEXIST)
            परत crun_make_error (err, त्रुटि_सं, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          जर (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            परत crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : रिक्त, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      जर (UNLIKELY (cwd < 0))
        परत crun_error_wrap (err, "creating `/%s`", path);

      जर (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, रिक्त);
          जर (UNLIKELY (ret < 0))
            {
              पूर्णांक saved_errno = त्रुटि_सं;

              बंद (cwd);
              परत crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          जर ((st_mode & S_IFMT) != S_IFDIR)
            {
              बंद (cwd);
              परत crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      जर (it == रिक्त)
        थांबा;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = म_अक्षर (cur, '/');
    }

  जर (do_open)
    {
      जर (cwd == dirfd)
        {
          ret = dup (dirfd);
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "dup `%s`", dirpath);
          परत ret;
        }

      wd_cleanup = -1;
      परत cwd;
    }

  परत 0;
}

पूर्णांक
crun_safe_create_and_open_ref_at (bool dir, पूर्णांक dirfd, स्थिरांक अक्षर *dirpath, स्थिरांक अक्षर *path, पूर्णांक mode, libcrun_error_t *err)
{
  पूर्णांक ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  जर (LIKELY (ret >= 0))
    परत ret;

  crun_error_release (err);
  परत crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णांक
crun_safe_ensure_directory_at (पूर्णांक dirfd, स्थिरांक अक्षर *dirpath, स्थिरांक अक्षर *path, पूर्णांक mode,
                               libcrun_error_t *err)
{
  परत crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णांक
crun_safe_ensure_file_at (पूर्णांक dirfd, स्थिरांक अक्षर *dirpath, स्थिरांक अक्षर *path, पूर्णांक mode,
                          libcrun_error_t *err)
{
  परत crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णांक
crun_ensure_directory (स्थिरांक अक्षर *path, पूर्णांक mode, bool nofollow, libcrun_error_t *err)
{
  परत crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

स्थिर पूर्णांक
get_file_size (पूर्णांक fd, off_t *size)
{
  संरचना stat st;
  पूर्णांक ret;
#ifdef HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  जर (UNLIKELY (ret < 0))
    {
      जर (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        जा fallback;
      परत ret;
    }
  *size = stx.stx_size;

  परत ret;

fallback:
#endif
  ret = ख_स्थिति (fd, &st);
  *size = st.st_size;
  परत ret;
}

पूर्णांक
crun_dir_p_at (पूर्णांक dirfd, स्थिरांक अक्षर *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  पूर्णांक ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "stat `%s`", path);

  परत S_ISDIR (mode) ? 1 : 0;
}

पूर्णांक
crun_dir_p (स्थिरांक अक्षर *path, bool nofollow, libcrun_error_t *err)
{
  परत crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
स्थिर पूर्णांक
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free अक्षर *buffer = रिक्त;
  माप_प्रकार len;
  पूर्णांक ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  जर (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      जर (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          परत 0;
        }
      परत ret;
    }

  परत म_माला (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
स्थिर bool
running_in_openvz ()
{
  जर (access ("/proc/vz", F_OK) < 0)
    परत false;

  परत access ("/proc/bc", F_OK) < 0 && त्रुटि_सं == ENOENT;
}

पूर्णांक
check_running_in_user_namespace (libcrun_error_t *err)
{
  स्थिर atomic_int run_in_userns = -1;
  संरचना stat st;
  पूर्णांक ret;

  ret = run_in_userns;
  जर (ret >= 0)
    परत ret;

  ret = stat ("/proc/self/ns/user", &st);
  जर (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      जर (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          परत run_in_userns;
        }

      जर (! running_in_openvz ())
        {
          run_in_userns = 1;
          परत run_in_userns;
        }
    }
  अन्यथा जर (UNLIKELY (त्रुटि_सं != ENOENT))
    परत crun_make_error (err, त्रुटि_सं, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  run_in_userns = ret;
  परत run_in_userns;
}

स्थिर माप_प्रकार
get_page_size ()
{
  स्थिर atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  जर (pagesize == 0)
    {
      pagesize = (दीर्घ) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  परत (माप_प्रकार) pagesize;
}

स्थिर पूर्णांक selinux_enabled = -1;
स्थिर पूर्णांक apparmor_enabled = -1;

पूर्णांक
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free अक्षर *out = रिक्त;
  cleanup_close पूर्णांक fd = -1;
  माप_प्रकार len;
  पूर्णांक ret;

  जर (selinux_enabled >= 0)
    परत selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  जर (UNLIKELY (fd < 0))
    परत fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  selinux_enabled = म_माला (out, "selinux") ? 1 : 0;

  परत selinux_enabled;
}

पूर्णांक
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक size;
  अक्षर buf[1];

  जर (apparmor_enabled >= 0)
    परत apparmor_enabled;

  fd = खोलो ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  जर (fd == -1)
    {
      जर (त्रुटि_सं == ENOENT)
        {
          apparmor_enabled = 0;
          परत 0;
        }

      परत crun_make_error (err, त्रुटि_सं, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (पठ (fd, buf, आकार (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  परत apparmor_enabled;
}

स्थिर पूर्णांक
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  जर (selinux_enabled < 0)
    परत crun_make_error (err, 0, "SELinux is not initialized correctly");
  परत selinux_enabled;
}

पूर्णांक
add_selinux_mount_label (अक्षर **retlabel, स्थिरांक अक्षर *data, स्थिरांक अक्षर *label, स्थिरांक अक्षर *context_type, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_is_selinux_enabled (err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (label && ret)
    {
      जर (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      अन्यथा
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      परत 0;
    }
  *retlabel = xstrdup (data);
  परत 0;
}

स्थिर स्थिरांक अक्षर *
lsm_attr_path (libcrun_container_t *container, स्थिरांक अक्षर *lsm, स्थिरांक अक्षर *fname, libcrun_error_t *err)
{
  cleanup_close पूर्णांक attr_dirfd = -1;
  cleanup_close पूर्णांक lsm_dirfd = -1;
  अक्षर *attr_path = रिक्त;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  जर (UNLIKELY (attr_dirfd < 0))
    परत रिक्त;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  जर (lsm != रिक्त)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      जर (UNLIKELY (lsm_dirfd < 0 && त्रुटि_सं != ENOENT))
        {
          crun_make_error (err, त्रुटि_सं, "open `/proc/thread-self/attr/%s`", lsm);
          परत रिक्त;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  परत attr_path;
}

स्थिर पूर्णांक
check_proc_super_magic (पूर्णांक fd, स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  संरचना statfs sfs;

  पूर्णांक ret = fstatfs (fd, &sfs);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "statfs `%s`", path);

  जर (sfs.f_type != PROC_SUPER_MAGIC)
    परत crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  परत 0;
}

स्थिर पूर्णांक
set_security_attr (libcrun_container_t *container, स्थिरांक अक्षर *lsm, स्थिरांक अक्षर *fname, स्थिरांक अक्षर *data, libcrun_error_t *err)
{
  पूर्णांक ret;

  cleanup_free स्थिरांक अक्षर *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close पूर्णांक fd = -1;

  जर (UNLIKELY (attr_path == रिक्त))
    परत -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  जर (UNLIKELY (fd < 0))
    परत fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (लिख (fd, data, म_माप (data)));
  जर (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      जर (त्रुटि_सं == ENOENT && lsm != रिक्त && म_भेद (lsm, "apparmor") == 0)
        {
          स्थिरांक अक्षर *profile = म_अक्षर (data, ' ');

          परत crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      परत crun_make_error (err, त्रुटि_सं, "write to file `%s`", attr_path);
    }

  परत 0;
}

पूर्णांक
set_selinux_label (libcrun_container_t *container, स्थिरांक अक्षर *label, bool now, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_is_selinux_enabled (err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (ret)
    परत set_security_attr (container, रिक्त, now ? "current" : "exec", label, err);
  परत 0;
}

स्थिर पूर्णांक
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  जर (apparmor_enabled < 0)
    परत crun_make_error (err, 0, "AppArmor is not initialized correctly");
  परत apparmor_enabled;
}

स्थिर पूर्णांक
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (आकार (UNCONFINED) - 1)
  cleanup_free स्थिरांक अक्षर *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close पूर्णांक fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  अक्षर buf[UNCONFINED_LEN + 1];

  जर (UNLIKELY (attr_path == रिक्त))
    परत -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  जर (UNLIKELY (fd < 0))
    परत fd;

  जर (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    परत -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (पठ (fd, buf, आकार (buf)));
  जर (UNLIKELY (bytes_read < 0))
    परत crun_make_error (err, त्रुटि_सं, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  जर (bytes_read < UNCONFINED_LEN || स_भेद (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    परत 1;
  जर (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    परत 0;
  परत 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

पूर्णांक
set_apparmor_profile (libcrun_container_t *container, स्थिरांक अक्षर *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_is_apparmor_enabled (err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (ret)
    {
      cleanup_free अक्षर *buf = रिक्त;
      ret = is_current_process_confined (container, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      परत set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  परत 0;
}

स्थिर पूर्णांक
read_all_fd_internal (पूर्णांक fd, स्थिरांक अक्षर *description, अक्षर **out, माप_प्रकार *len, माप_प्रकार size_hint, माप_प्रकार max_size, libcrun_error_t *err)
{
  cleanup_free अक्षर *buf = रिक्त;
  माप_प्रकार nread, allocated;
  माप_प्रकार pagesize = 0;
  off_t size = 0;
  पूर्णांक ret;

  जर (size_hint)
    allocated = size_hint;
  अन्यथा
    {
      ret = get_file_size (fd, &size);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "stat `%s`", description);

      जर (max_size && (माप_प्रकार) size > max_size)
        परत crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  पर्यंत ((size && nread < (माप_प्रकार) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (पठ (fd, buf + nread, allocated - nread));
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "read from file `%s`", description);

      जर (ret == 0)
        थांबा;

      nread += ret;

      जर (max_size && nread > max_size)
        परत crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      जर (nread == allocated)
        {
          जर (size)
            थांबा;

          जर (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  जर (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      अक्षर *tmp = पुनः_स्मृति (buf, nread + 1);
      जर (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = रिक्त;
  जर (len)
    *len = nread;
  परत 0;
}

पूर्णांक
read_all_fd_with_size_hint (पूर्णांक fd, स्थिरांक अक्षर *description, अक्षर **out, माप_प्रकार *len, माप_प्रकार size_hint, libcrun_error_t *err)
{
  परत read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

पूर्णांक
read_all_fd_with_max_size (पूर्णांक fd, स्थिरांक अक्षर *description, अक्षर **out, माप_प्रकार *len, माप_प्रकार max_size, libcrun_error_t *err)
{
  परत read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

पूर्णांक
read_all_file_at (पूर्णांक dirfd, स्थिरांक अक्षर *path, अक्षर **out, माप_प्रकार *len, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  जर (UNLIKELY (fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", path);

  परत read_all_fd (fd, path, out, len, err);
}

पूर्णांक
read_all_file (स्थिरांक अक्षर *path, अक्षर **out, माप_प्रकार *len, libcrun_error_t *err)
{
  जर (म_भेद (path, "-") == 0)
    path = "/dev/stdin";

  परत read_all_file_at (AT_FDCWD, path, out, len, err);
}

पूर्णांक
get_realpath_to_file (पूर्णांक dirfd, स्थिरांक अक्षर *path_name, अक्षर **absolute_path, libcrun_error_t *err)
{
  cleanup_close पूर्णांक targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  जर (UNLIKELY (targetfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", path_name);
  अन्यथा
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      जर (UNLIKELY (len < 0))
        परत crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  परत 0;
}

पूर्णांक
open_unix_domain_client_socket (स्थिरांक अक्षर *path, पूर्णांक dgram, libcrun_error_t *err)
{
  संरचना sockaddr_un addr = {};
  पूर्णांक ret;
  proc_fd_path_t name_buf;
  cleanup_close पूर्णांक destfd = -1;
  cleanup_close पूर्णांक fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  जर (UNLIKELY (fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "create UNIX socket");

  जर (म_माप (path) >= आकार (addr.sun_path))
    {
      destfd = खोलो (path, O_PATH | O_CLOEXEC);
      जर (UNLIKELY (destfd < 0))
        परत crun_make_error (err, त्रुटि_सं, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  माप_प्रकार path_len = म_माप (path);
  जर (path_len >= आकार (addr.sun_path))
    परत crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  स_नकल (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (संरचना sockaddr *) &addr, आकार (addr));
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  परत ret;
}

पूर्णांक
open_unix_domain_socket (स्थिरांक अक्षर *path, पूर्णांक dgram, libcrun_error_t *err)
{
  संरचना sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  पूर्णांक ret;
  cleanup_close पूर्णांक dirfd = -1;
  cleanup_close पूर्णांक fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  जर (UNLIKELY (fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "create UNIX socket");

  जर (म_माप (path) >= आकार (addr.sun_path))
    {
      cleanup_free अक्षर *dpath = xstrdup (path);
      cleanup_free अक्षर *bpath = xstrdup (path);
      स्थिरांक अक्षर *parent_dir = स_नाम (dpath);
      स्थिरांक अक्षर *base = basename (bpath);
      पूर्णांक n;

      dirfd = खोलो (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      जर (UNLIKELY (dirfd < 0))
        परत crun_make_error (err, त्रुटि_सं, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, आकार (addr.sun_path), "%s/%s", name_buf, base);
      जर (n < 0 || (माप_प्रकार) n >= आकार (addr.sun_path))
        परत crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  अन्यथा
    {
      माप_प्रकार path_len = म_माप (path);
      स_नकल (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (संरचना sockaddr *) &addr, आकार (addr));
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "bind socket to `%s`", path);

  जर (! dgram)
    {
      ret = listen (fd, 1);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "listen on socket");
    }

  ret = fd;
  fd = -1;

  परत ret;
}

पूर्णांक
send_fd_to_socket (पूर्णांक server, पूर्णांक fd, libcrun_error_t *err)
{
  परत send_fd_to_socket_with_payload (server, fd, रिक्त, 0, err);
}

पूर्णांक
send_fd_to_socket_with_payload (पूर्णांक server, पूर्णांक fd, स्थिरांक अक्षर *payload, माप_प्रकार payload_len, libcrun_error_t *err)
{
  पूर्णांक ret;
  संरचना cmsghdr *cmsg = रिक्त;
  संरचना iovec iov[2];
  संरचना msghdr संदेश = {};
  अक्षर ctrl_buf[CMSG_SPACE (1 + आकार (पूर्णांक))] = {};
  अक्षर data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = आकार (data);

  जर (payload_len > 0)
    {
      iov[0].iov_base = (शून्य *) payload;
      iov[0].iov_len = payload_len;
    }

  संदेश.संदेश_name = रिक्त;
  संदेश.संदेश_namelen = 0;
  संदेश.संदेश_iov = iov;
  संदेश.संदेश_iovlen = 1;
  संदेश.संदेश_controllen = CMSG_SPACE (आकार (पूर्णांक));
  संदेश.संदेश_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&संदेश);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (आकार (पूर्णांक));

  *((पूर्णांक *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &संदेश, 0));
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "sendmsg");
  परत 0;
}

पूर्णांक
receive_fd_from_socket_with_payload (पूर्णांक from, अक्षर *payload, माप_प्रकार payload_len, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret;
  संरचना iovec iov[1];
  संरचना msghdr संदेश = {};
  अक्षर ctrl_buf[CMSG_SPACE (आकार (पूर्णांक))] = {};
  अक्षर data[1];
  संरचना cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = आकार (data);

  जर (payload_len > 0)
    {
      iov[0].iov_base = (शून्य *) payload;
      iov[0].iov_len = payload_len;
    }

  संदेश.संदेश_name = रिक्त;
  संदेश.संदेश_namelen = 0;
  संदेश.संदेश_iov = iov;
  संदेश.संदेश_iovlen = 1;
  संदेश.संदेश_controllen = CMSG_SPACE (आकार (पूर्णांक));
  संदेश.संदेश_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &संदेश, 0));
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "recvmsg");
  जर (UNLIKELY (ret == 0))
    परत crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&संदेश);
  जर (cmsg == रिक्त)
    परत crun_make_error (err, 0, "no msg received");
  स_नकल (&fd, CMSG_DATA (cmsg), आकार (fd));

  ret = fd;
  fd = -1;
  परत ret;
}

पूर्णांक
receive_fd_from_socket (पूर्णांक from, libcrun_error_t *err)
{
  परत receive_fd_from_socket_with_payload (from, रिक्त, 0, err);
}

पूर्णांक
create_socket_pair (पूर्णांक *pair, libcrun_error_t *err)
{
  पूर्णांक ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "socketpair");
  परत 0;
}

पूर्णांक
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  पूर्णांक ret = signalfd (-1, mask, 0);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "signalfd");
  परत ret;
}

स्थिर पूर्णांक
epoll_helper_toggle (पूर्णांक epollfd, पूर्णांक fd, पूर्णांक events, libcrun_error_t *err)
{
  संरचना epoll_event ev = {};
  bool add = events != 0;
  पूर्णांक ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  जर (UNLIKELY (ret < 0 && त्रुटि_सं == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  जर (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      जर (त्रुटि_सं == ENOENT || त्रुटि_सं == EPERM)
        परत 0;
      परत crun_make_error (err, त्रुटि_सं, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  परत 0;
}

पूर्णांक
epoll_helper (पूर्णांक *in_fds, पूर्णांक *in_levelfds, पूर्णांक *out_fds, पूर्णांक *out_levelfds, libcrun_error_t *err)
{
  संरचना epoll_event ev;
  cleanup_close पूर्णांक epollfd = -1;
  पूर्णांक ret;

  पूर्णांक *it;
  epollfd = epoll_create1 (0);
  जर (UNLIKELY (epollfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  साठी (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      जर (UNLIKELY (ret < 0))                                           \
        परत crun_make_error (err, त्रुटि_सं, "epoll_ctl add `%d`", *it); \
    }

  जर (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  जर (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  जर (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  जर (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  परत ret;
}

पूर्णांक
copy_from_fd_to_fd (पूर्णांक src, पूर्णांक dst, पूर्णांक consume, libcrun_error_t *err)
{
  पूर्णांक ret;
  ssize_t nread;
  माप_प्रकार pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  करा
    {
      cleanup_free अक्षर *buffer = रिक्त;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      जर (can_copy_file_range)
        {
          nread = copy_file_range (src, रिक्त, dst, रिक्त, pagesize, 0);
          जर (nread < 0 && (त्रुटि_सं == EINVAL || त्रुटि_सं == EXDEV))
            {
              can_copy_file_range = false;
              जा fallback;
            }
          जर (consume && nread < 0 && त्रुटि_सं == EAGAIN)
            परत 0;
          जर (nread < 0 && त्रुटि_सं == EIO)
            परत 0;
          जर (UNLIKELY (nread < 0))
            परत crun_make_error (err, त्रुटि_सं, "copy_file_range");
          सुरूठेवा;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (पठ (src, buffer, pagesize));
      जर (consume && nread < 0 && त्रुटि_सं == EAGAIN)
        परत 0;
      जर (nread < 0 && त्रुटि_सं == EIO)
        परत 0;
      जर (UNLIKELY (nread < 0))
        परत crun_make_error (err, त्रुटि_सं, "read");

      remaining = nread;
      पर्यंत (remaining)
        {
          ret = TEMP_FAILURE_RETRY (लिख (dst, buffer + nread - remaining, remaining));
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "write");
          remaining -= ret;
        }
  } पर्यंत (consume && nread);

  परत 0;
}

पूर्णांक
run_process (अक्षर **args, libcrun_error_t *err)
{
  pid_t pid = विभाजन ();
  जर (UNLIKELY (pid < 0))
    परत crun_make_error (err, त्रुटि_सं, "fork");
  जर (pid)
    {
      पूर्णांक r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      जर (r < 0)
        परत crun_make_error (err, त्रुटि_सं, "waitpid");
      जर (WIFEXITED (status) || WIFSIGNALED (status))
        परत WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (निकास_त्रुटि);
}

#ifndef HAVE_FGETPWENT_R
स्थिर अचिन्हित
atou (अक्षर **s)
{
  अचिन्हित x;
  साठी (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  परत x;
}

पूर्णांक
fgetpwent_r (खाता *f, संरचना passwd *pw, अक्षर *ओळ, माप_प्रकार size, संरचना passwd **res)
{
  अक्षर *s;
  पूर्णांक rv = 0;
  साठी (;;)
    {
      ओळ[size - 1] = '\xff';
      जर ((ख_माला_लो (ओळ, size, f) == रिक्त) || ख_त्रुटि (f) || ओळ[size - 1] != '\xff')
        {
          rv = (ओळ[size - 1] != '\xff') ? दुस्फल : ENOENT;
          ओळ = 0;
          pw = 0;
          थांबा;
        }
      ओळ[म_खोज (ओळ, "\n")] = 0;

      s = ओळ;
      pw->pw_name = s++;
      जर (! (s = म_अक्षर (s, ':')))
        सुरूठेवा;

      *s++ = 0;
      pw->pw_passwd = s;
      जर (! (s = म_अक्षर (s, ':')))
        सुरूठेवा;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      जर (*s != ':')
        सुरूठेवा;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      जर (*s != ':')
        सुरूठेवा;

      *s++ = 0;
      pw->pw_gecos = s;
      जर (! (s = म_अक्षर (s, ':')))
        सुरूठेवा;

      *s++ = 0;
      pw->pw_dir = s;
      जर (! (s = म_अक्षर (s, ':')))
        सुरूठेवा;

      *s++ = 0;
      pw->pw_shell = s;
      थांबा;
    }
  *res = pw;
  जर (rv)
    त्रुटि_सं = rv;
  परत rv;
}
#endif

पूर्णांक
set_home_env (uid_t id)
{
  संरचना passwd pwd;
  cleanup_free अक्षर *buf = रिक्त;
  दीर्घ buf_size;
  cleanup_file खाता *stream = रिक्त;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  जर (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = ख_खोलो ("/etc/passwd", "re");
  जर (stream == रिक्त)
    परत -1;

  साठी (;;)
    {
      संरचना passwd *ret_pw = रिक्त;

      पूर्णांक ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      जर (UNLIKELY (ret != 0))
        {
          जर (ret == EINTR)
            सुरूठेवा;

          जर (ret != दुस्फल)
            {
              /* Let callers handle the error if the user was not found. */
              त्रुटि_सं = ret;
              परत -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          सुरूठेवा;
        }

      जर (ret_pw && ret_pw->pw_uid == id)
        {
          जर (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          अन्यथा
            परत 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
स्थिर पूर्णांक
getsubidrange (uid_t id, पूर्णांक is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file खाता *आदान = रिक्त;
  cleanup_free अक्षर *lineptr = रिक्त;
  माप_प्रकार lenlineptr = 0, len_name;
  दीर्घ buf_size;
  cleanup_free अक्षर *buf = रिक्त;
  स्थिरांक अक्षर *name;
  संरचना passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  जर (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  साठी (;;)
    {
      पूर्णांक ret;
      संरचना passwd *ret_pw = रिक्त;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      जर (LIKELY (ret == 0))
        {
          जर (ret_pw)
            {
              name = ret_pw->pw_name;
              थांबा;
            }
          परत -1;
        }

      जर (ret != दुस्फल)
        परत -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = म_माप (name);

  आदान = ख_खोलो (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  जर (आदान == रिक्त)
    परत -1;

  साठी (;;)
    {
      अक्षर *endptr;
      ssize_t पठ = getline (&lineptr, &lenlineptr, आदान);
      जर (पठ < 0)
        परत -1;

      जर (पठ < (ssize_t) (len_name + 2))
        सुरूठेवा;

      जर (स_भेद (lineptr, name, len_name) || lineptr[len_name] != ':')
        सुरूठेवा;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      जर (endptr >= &lineptr[पठ])
        परत -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      परत 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

पूर्णांक
format_default_id_mapping (अक्षर **out, uid_t container_id, uid_t host_uid, uid_t host_id, पूर्णांक is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free अक्षर *buffer = रिक्त;
  पूर्णांक written = 0;
  पूर्णांक ret, remaining;

  *out = रिक्त;

  जर (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    परत 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  जर (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      जर (UNLIKELY (ret >= remaining))
        परत crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  जर (UNLIKELY (ret >= remaining))
    परत crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  जर (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      जर (UNLIKELY (ret >= remaining))
        परत crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = रिक्त;
  परत written;
}

स्थिर पूर्णांक
unset_cloexec_flag (पूर्णांक fd)
{
  पूर्णांक flags = fcntl (fd, F_GETFD);
  जर (flags == -1)
    परत -1;

  flags &= ~FD_CLOEXEC;

  परत fcntl (fd, F_SETFD, flags);
}

स्थिर शून्य __attribute__ ((__noreturn__))
run_process_child (अक्षर *path, अक्षर **args, स्थिरांक अक्षर *cwd, अक्षर **envp, पूर्णांक pipe_r,
                   पूर्णांक pipe_w, पूर्णांक out_fd, पूर्णांक err_fd, bool can_ignore_chdir_errors)
{
  अक्षर *tmp_args[] = { path, रिक्त };
  libcrun_error_t err = रिक्त;
  पूर्णांक dev_null_fd = -1;
  पूर्णांक ret;

  ret = mark_or_close_fds_ge_than (रिक्त, 3, false, &err);
  जर (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->संदेश);

  जर (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = खोलो ("/dev/null", O_WRONLY | O_CLOEXEC);
      जर (UNLIKELY (dev_null_fd < 0))
        _safe_exit (निकास_त्रुटि);
    }

  TEMP_FAILURE_RETRY (बंद (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (बंद (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  जर (out_fd >= 0)
    unset_cloexec_flag (1);
  जर (err_fd >= 0)
    unset_cloexec_flag (2);

  जर (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (बंद (dev_null_fd));
  जर (out_fd >= 0)
    TEMP_FAILURE_RETRY (बंद (out_fd));
  जर (err_fd >= 0)
    TEMP_FAILURE_RETRY (बंद (err_fd));

  जर (args == रिक्त)
    args = tmp_args;

  जर (cwd && स_बदलो (cwd) < 0)
    {
      जर (! can_ignore_chdir_errors || (त्रुटि_सं != EACCES && त्रुटि_सं != EPERM))
        _safe_exit (निकास_त्रुटि);
    }

  execvpe (path, args, envp);
  _safe_exit (निकास_त्रुटि);
}

/* It changes the signals mask for the current process.  */
पूर्णांक
run_process_with_stdin_timeout_envp (अक्षर *path, अक्षर **args, स्थिरांक अक्षर *cwd, पूर्णांक timeout,
                                     अक्षर **envp, अक्षर *मानक_निवेश, माप_प्रकार stdin_len, पूर्णांक out_fd,
                                     पूर्णांक err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  पूर्णांक stdin_pipe[2];
  pid_t pid;
  पूर्णांक ret;
  cleanup_close पूर्णांक pipe_r = -1;
  cleanup_close पूर्णांक pipe_w = -1;
  sigset_t oldmask, mask;
  पूर्णांक r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  जर (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "sigprocmask");
    }

  pid = विभाजन ();
  जर (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, त्रुटि_सं, "fork");
      जा restore_sig_mask_and_exit;
    }

  जर (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (लिख (pipe_w, मानक_निवेश, stdin_len));
  जर (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      जर (त्रुटि_सं != EPIPE)
        {
          ret = crun_make_error (err, त्रुटि_सं, "write to pipe");
          जा restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  जर (timeout)
    {
      समय_प्रकार start = समय (रिक्त);
      समय_प्रकार now;
      साठी (now = start; now - start < timeout; now = समय (रिक्त))
        {
          siginfo_t info;
          पूर्णांक elapsed = now - start;
          संरचना timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          जर (UNLIKELY (ret < 0 && त्रुटि_सं != EAGAIN))
            {
              ret = crun_make_error (err, त्रुटि_सं, "sigtimedwait");
              जा restore_sig_mask_and_exit;
            }

          जर (info.si_signo == SIGCHLD && info.si_pid == pid)
            जा read_waitpid;

          जर (ret < 0 && त्रुटि_सं == EAGAIN)
            जा timeout;
        }
    timeout:
      समाप्त (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      जा restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  जर (r < 0)
    ret = crun_make_error (err, त्रुटि_सं, "waitpid");
  अन्यथा
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  जर (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      जर (pid > 0)
        {
          समाप्त (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, रिक्त);
      जर (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, त्रुटि_सं, "restoring signal mask with sigprocmask");
    }
  परत ret;
}

पूर्णांक
mark_or_close_fds_ge_than (libcrun_container_t *container, पूर्णांक n, bool close_now, libcrun_error_t *err)
{
  cleanup_close पूर्णांक cfd = -1;
  cleanup_dir सूची *dir = रिक्त;
  पूर्णांक ret;
  पूर्णांक fd;
  संरचना dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  जर (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, अच_पूर्णांक_उच्च, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  जर (ret == 0)
    {
      जर (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      परत 0;
    }
  जर (ret < 0 && त्रुटि_सं != EINVAL && त्रुटि_सं != ENOSYS && त्रुटि_सं != EPERM)
    परत crun_make_error (err, त्रुटि_सं, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  जर (UNLIKELY (cfd < 0))
    परत cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  dir = fdopendir (cfd);
  जर (UNLIKELY (dir == रिक्त))
    परत crun_make_error (err, त्रुटि_सं, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  साठी (next = सूची_पढ़ो (dir); next; next = सूची_पढ़ो (dir))
    {
      पूर्णांक val;
      स्थिरांक अक्षर *name = next->d_name;
      जर (name[0] == '.')
        सुरूठेवा;

      val = strtoll (name, रिक्त, 10);
      जर (val < n || val == fd)
        सुरूठेवा;

      जर (close_now)
        {
          ret = बंद (val);
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "close fd `%d`", val);

          जर (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      अन्यथा
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  परत 0;
}

शून्य
get_current_timestamp (अक्षर *out, माप_प्रकार len)
{
  संरचना timeval tv;
  संरचना पंचांग now;
  अक्षर timestamp[64];

  gettimeofday (&tv, रिक्त);
  gmtime_r (&tv.tv_sec, &now);
  स_माला (timestamp, आकार (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (शून्य) snprintf (out, len, "%s.%06lldZ", timestamp, (दीर्घ दीर्घ पूर्णांक) tv.tv_usec);
  out[len - 1] = '\0';
}

पूर्णांक
set_blocking_fd (पूर्णांक fd, bool blocking, libcrun_error_t *err)
{
  पूर्णांक ret, flags = fcntl (fd, F_GETFL, 0);
  जर (UNLIKELY (flags < 0))
    परत crun_make_error (err, त्रुटि_सं, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "fcntl");
  परत 0;
}

पूर्णांक
parse_json_file (json_object **out, स्थिरांक अक्षर *jsondata, संरचना parser_context *ctx arg_unused, libcrun_error_t *err)
{
  संरचना json_tokener *tok;
  गणना json_tokener_error jerr;

  *err = रिक्त;

  जर (UNLIKELY (jsondata == रिक्त))
    परत crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  जर (UNLIKELY (tok == रिक्त))
    परत crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (पूर्णांक) म_माप (jsondata));
  jerr = json_tokener_get_error (tok);
  जर (*out == रिक्त || jerr != json_tokener_success)
    {
      पूर्णांक saved_pos = (पूर्णांक) tok->char_offset;

      json_tokener_free (tok);
      जर (*out)
        {
          json_object_put (*out);
          *out = रिक्त;
        }
      परत crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  परत 0;
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
स्थिर पूर्णांक
check_access (स्थिरांक अक्षर *path)
{
  पूर्णांक ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#अन्यथा
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  जर (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      जर (त्रुटि_सं == EACCES)
        {
          पूर्णांक saved_errno = त्रुटि_सं;
          ret = ACCESS (path, F_OK);
          त्रुटि_सं = saved_errno;

          जर (ret == 0)
            परत CHECK_ACCESS_NOT_EXECUTABLE;
        }
      परत -त्रुटि_सं;
    }

  ret = get_file_type (&mode, false, path);
  जर (UNLIKELY (ret < 0))
    परत -त्रुटि_सं;

  जर (! S_ISREG (mode))
    परत CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  परत 0;
}

पूर्णांक
find_executable (अक्षर **out, स्थिरांक अक्षर *executable_path, स्थिरांक अक्षर *cwd, libcrun_error_t *err)
{
  cleanup_free अक्षर *cwd_executable_path = रिक्त;
  cleanup_free अक्षर *tmp = रिक्त;
  पूर्णांक last_error = -ENOENT;
  अक्षर *it, *end;
  पूर्णांक ret;

  *out = रिक्त;

  जर (executable_path == रिक्त || executable_path[0] == '\0')
    परत crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  जर (executable_path[0] != '/' && म_अक्षर (executable_path, '/'))
    {
      cleanup_free अक्षर *cwd_allocated = रिक्त;

      जर (cwd == रिक्त)
        {
          cwd_allocated = getcwd (रिक्त, 0);
          जर (cwd_allocated == रिक्त)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  जर (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      जर (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          परत 0;
        }
      last_error = ret;
      जा fail;
    }

  end = tmp = xstrdup (दो_पर्या ("PATH"));

  पर्यंत ((it = strsep (&end, ":")))
    {
      cleanup_free अक्षर *path = रिक्त;

      जर (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, रिक्त);
      जर (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          सुरूठेवा;
        }

      ret = check_access (path);
      जर (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = रिक्त;
          परत 0;
        }

      जर (ret == -ENOENT)
        सुरूठेवा;

      last_error = ret;
    }

fail:
  निवड (last_error)
    {
    स्थिती CHECK_ACCESS_NOT_EXECUTABLE:
      परत crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    स्थिती CHECK_ACCESS_NOT_REGULAR:
      परत crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    पूर्वनिर्धारित:
      त्रुटि_सं = -last_error;
      जर (त्रुटि_सं == ENOENT)
        परत crun_make_error (err, त्रुटि_सं, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      परत crun_make_error (err, त्रुटि_सं, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

स्थिर ssize_t
safe_read_xattr (अक्षर **ret, पूर्णांक sfd, स्थिरांक अक्षर *srcname, स्थिरांक अक्षर *name, माप_प्रकार initial_size, libcrun_error_t *err)
{
  cleanup_free अक्षर *buffer = रिक्त;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  पर्यंत (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      जर (UNLIKELY (s < 0))
        परत crun_make_error (err, त्रुटि_सं, "get xattr `%s` from `%s`", name, srcname);

      जर (s < current_size)
        थांबा;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  जर (s <= 0)
    परत s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = रिक्त;

  परत s;
}

स्थिर ssize_t
copy_xattr (पूर्णांक sfd, पूर्णांक dfd, स्थिरांक अक्षर *srcname, स्थिरांक अक्षर *destname, libcrun_error_t *err)
{
  cleanup_free अक्षर *buf = रिक्त;
  ssize_t xattr_len;
  अक्षर *it;

  xattr_len = flistxattr (sfd, रिक्त, 0);
  जर (UNLIKELY (xattr_len < 0))
    {
      जर (त्रुटि_सं == ENOTSUP)
        परत 0;

      परत crun_make_error (err, त्रुटि_सं, "flistxattr `%s`", srcname);
    }

  जर (xattr_len == 0)
    परत 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  जर (UNLIKELY (xattr_len < 0))
    परत crun_make_error (err, त्रुटि_सं, "flistxattr `%s`", srcname);

  साठी (it = buf; it - buf < xattr_len; it += म_माप (it) + 1)
    {
      cleanup_free अक्षर *v = रिक्त;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      जर (UNLIKELY (s < 0))
        परत s;

      s = fsetxattr (dfd, it, v, s, 0);
      जर (UNLIKELY (s < 0))
        {
          जर (त्रुटि_सं == EINVAL || त्रुटि_सं == EOPNOTSUPP)
            सुरूठेवा;

          परत crun_make_error (err, त्रुटि_सं, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  परत 0;
}

#endif

स्थिर पूर्णांक
copy_rec_stat_file_at (पूर्णांक dfd, स्थिरांक अक्षर *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  संरचना stat st;
  पूर्णांक ret;

#ifdef HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  जर (UNLIKELY (ret < 0))
    {
      जर (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        जा fallback;

      परत ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  परत ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  परत ret;
}

पूर्णांक
copy_recursive_fd_to_fd (पूर्णांक srcdirfd, पूर्णांक dfd, स्थिरांक अक्षर *srcname, स्थिरांक अक्षर *destname, libcrun_error_t *err)
{
  cleanup_close पूर्णांक destdirfd = dfd;
  cleanup_dir सूची *dsrcfd = रिक्त;
  संरचना dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  जर (UNLIKELY (dsrcfd == रिक्त))
    {
      TEMP_FAILURE_RETRY (बंद (srcdirfd));
      परत crun_make_error (err, त्रुटि_सं, "open directory `%s`", destname);
    }

  साठी (de = सूची_पढ़ो (dsrcfd); de; de = सूची_पढ़ो (dsrcfd))
    {
      cleanup_close पूर्णांक srcfd = -1;
      cleanup_close पूर्णांक destfd = -1;
      cleanup_free अक्षर *target_buf = रिक्त;
      पूर्णांक ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      जर (म_भेद (de->d_name, ".") == 0 || म_भेद (de->d_name, "..") == 0)
        सुरूठेवा;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "stat `%s/%s`", srcname, de->d_name);

      निवड (mode & S_IFMT)
        {
        स्थिती S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          जर (UNLIKELY (srcfd < 0))
            परत crun_make_error (err, त्रुटि_सं, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          जर (UNLIKELY (destfd < 0))
            परत crun_make_error (err, त्रुटि_सं, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          जर (UNLIKELY (ret < 0))
            परत ret;

#ifdef HAVE_FGETXATTR
          ret = (पूर्णांक) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
#endif

          TEMP_FAILURE_RETRY (बंद (destfd));
          destfd = -1;
          थांबा;

        स्थिती S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          जर (UNLIKELY (srcfd < 0))
            परत crun_make_error (err, त्रुटि_सं, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          जर (UNLIKELY (destfd < 0))
            परत crun_make_error (err, त्रुटि_सं, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (पूर्णांक) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          जर (UNLIKELY (ret < 0))
            परत ret;
          थांबा;

        स्थिती S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          जर (UNLIKELY (ret < 0))
            परत ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "symlinkat `%s/%s`", destname, de->d_name);
          थांबा;

        स्थिती S_IFBLK:
        स्थिती S_IFCHR:
        स्थिती S_IFIFO:
        स्थिती S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "mknodat `%s/%s`", destname, de->d_name);
          थांबा;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      जर (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          जर (त्रुटि_सं == ENOTSUP)
            सुरूठेवा;

          परत crun_make_error (err, त्रुटि_सं, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  परत 0;
}

स्थिरांक अक्षर *
find_annotation (libcrun_container_t *container, स्थिरांक अक्षर *name)
{
  जर (container->container_def->annotations == रिक्त)
    परत रिक्त;

  परत find_string_map_मूल्य (container->annotations, name);
}

पूर्णांक
safe_write (पूर्णांक fd, स्थिरांक अक्षर *fname, स्थिरांक शून्य *buf, माप_प्रकार मोजणी, libcrun_error_t *err)
{
  माप_प्रकार written = 0;
  पर्यंत (written < मोजणी)
    {
      ssize_t w = लिख (fd, (स्थिरांक अक्षर *) buf + written, मोजणी - written);
      जर (UNLIKELY (w < 0))
        {
          जर (त्रुटि_सं == EINTR || त्रुटि_सं == EAGAIN)
            सुरूठेवा;
          परत crun_make_error (err, त्रुटि_सं, "write to `%s`", fname);
        }
      written += w;
    }
  परत 0;
}

#define MAX_PARTS 32

पूर्णांक
append_paths (अक्षर **out, libcrun_error_t *err, ...)
{
  स्थिरांक अक्षर *parts[MAX_PARTS];
  माप_प्रकार sizes[MAX_PARTS];
  माप_प्रकार total_len = 0;
  माप_प्रकार n_parts = 0;
  माप_प्रकार copied = 0;
  बहु_सूची ap;
  माप_प्रकार i;

  बहु_शुरू (ap, err);
  साठी (;;)
    {
      स्थिरांक अक्षर *part;
      माप_प्रकार size;

      part = बहु_तर्क (ap, स्थिरांक अक्षर *);
      जर (part == रिक्त)
        थांबा;

      जर (n_parts == MAX_PARTS)
        {
          बहु_पूर्ण (ap);
          परत crun_make_error (err, EINVAL, "too many paths specified");
        }

      जर (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          पर्यंत (part[0] == '/' && part[1] == '/')
            part++;
        }
      अन्यथा
        {
          /* And drop any initial '/' for other components.  */
          पर्यंत (part[0] == '/')
            part++;
        }

      size = म_माप (part);
      जर (size == 0)
        सुरूठेवा;

      पर्यंत (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  बहु_पूर्ण (ap);

  total_len = n_parts + 1;
  साठी (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  साठी (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      जर (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      स_नकल (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  परत 0;
}

#जर __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#अन्यथा
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
स्थिर पूर्णांक
b64_आदान (अक्षर c)
{
  स्थिरांक अक्षर table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  पूर्णांक i;

  साठी (i = 0; i < 64; i++)
    {
      जर (table[i] == c)
        परत i;
    }
  परत -1;
}

पूर्णांक
base64_decode (स्थिरांक अक्षर *iptr, माप_प्रकार isize, अक्षर *optr, माप_प्रकार osize, माप_प्रकार *nbytes)
{
  पूर्णांक i = 0, tmp = 0, pad = 0;
  माप_प्रकार consumed = 0;
  अचिन्हित अक्षर data[4];

  *nbytes = 0;
  पर्यंत (consumed < isize && (*nbytes) + 3 < osize)
    {
      पर्यंत ((i < 4) && (consumed < isize))
        {
          tmp = b64_आदान (*iptr++);
          consumed++;
          जर (tmp != -1)
            {
              जर (UNLIKELY (pad > 0))
                परत -1;
              data[i++] = tmp;
            }
          अन्यथा जर (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      जर (i == 4)
        {
          जर (UNLIKELY (pad > 2))
            परत -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      अन्यथा
        {
          /* I did not get all the data.  */
          consumed -= i;
          परत consumed;
        }
      i = 0;
    }
  परत consumed;
}

अक्षर *
get_user_name (uid_t uid)
{
  संरचना passwd pd;
  संरचना passwd *temp_result_ptr;
  अक्षर pwdbuffer[200];
  जर (! getpwuid_r (uid, &pd, pwdbuffer, आकार (pwdbuffer), &temp_result_ptr))
    परत xstrdup (pd.pw_name);
  परत xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
अक्षर *
libcrun_sysctl_किल्ली_to_proc_path (स्थिरांक अक्षर *किल्ली)
{
  स्थिरांक अक्षर *sep;
  अक्षर *ret, *it;

  sep = strpbrk (किल्ली, "./");

  /* No separator at all, or the key is already in the path form.  */
  जर (sep == रिक्त || *sep == '/')
    परत xstrdup (किल्ली);

  ret = xstrdup (किल्ली);
  साठी (it = ret; *it; it++)
    {
      जर (*it == '.')
        *it = '/';
      अन्यथा जर (*it == '/')
        *it = '.';
    }

  परत ret;
}

पूर्णांक
has_suffix (स्थिरांक अक्षर *str, स्थिरांक अक्षर *suffix)
{
  जर (! str || ! suffix)
    परत 0;
  माप_प्रकार lenstr = म_माप (str);
  माप_प्रकार lensuffix = म_माप (suffix);
  जर (lensuffix > lenstr)
    परत 0;
  परत स_भेद (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

अक्षर *
str_join_array (पूर्णांक offset, माप_प्रकार size, अक्षर *स्थिरांक array[], स्थिरांक अक्षर *joint)
{
  माप_प्रकार jlen;
  cleanup_free माप_प्रकार *lens = रिक्त;
  माप_प्रकार i, total_size = (size - 1) * (jlen = म_माप (joint)) + 1;
  अक्षर *result, *p;

  lens = xmalloc (size * आकार (*lens));

  साठी (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = म_माप (array[i]));
    }
  p = result = xmalloc (total_size);
  साठी (i = offset; i < size; ++i)
    {
      स_नकल (p, array[i], lens[i]);
      p += lens[i];
      जर (i < size - 1)
        {
          स_नकल (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  परत result;
}

पूर्णांक
libcrun_mmap (संरचना libcrun_mmap_s **ret, शून्य *addr, माप_प्रकार length,
              पूर्णांक prot, पूर्णांक flags, पूर्णांक fd, off_t offset,
              libcrun_error_t *err)
{
  संरचना libcrun_mmap_s *mmap_s = रिक्त;

  शून्य *mapped = mmap (addr, length, prot, flags, fd, offset);
  जर (mapped == MAP_FAILED)
    परत crun_make_error (err, त्रुटि_सं, "mmap");

  mmap_s = xmalloc (आकार (संरचना libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  परत 0;
}

पूर्णांक
libcrun_munmap (संरचना libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = munmap (mmap->addr, mmap->length);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "munmap");

  मुक्त (mmap);

  परत 0;
}

स्थिर दीर्घ
read_file_as_long_or_default (स्थिरांक अक्षर *path, दीर्घ def_मूल्य)
{
  cleanup_free अक्षर *content = रिक्त;
  libcrun_error_t tmp_err = रिक्त;
  अक्षर *endptr;
  दीर्घ val;
  पूर्णांक ret;

  ret = read_all_file (path, &content, रिक्त, &tmp_err);
  जर (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      परत def_मूल्य;
    }

  त्रुटि_सं = 0;
  val = म_से_दीर्घ (content, &endptr, 10);
  जर (UNLIKELY (त्रुटि_सं))
    परत def_मूल्य;
  जर (endptr == content || (*endptr && *endptr != '\n'))
    परत def_मूल्य;
  परत val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (शून्य)
{
  स्थिर atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  जर (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  परत uid;
}

gid_t
get_overflow_gid (शून्य)
{
  स्थिर atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  जर (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  परत gid;
}

शून्य
consume_trailing_slashes (अक्षर *path)
{
  जर (! path || path[0] == '\0')
    परत;

  अक्षर *last = path + म_माप (path);

  पर्यंत (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

अक्षर **
read_dir_entries (स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  cleanup_dir सूची *dir = रिक्त;
  माप_प्रकार n_entries = 0;
  माप_प्रकार entries_size = 16;
  अक्षर **entries = रिक्त;
  संरचना dirent *de;

  dir = सूची_खोलो (path);
  जर (UNLIKELY (dir == रिक्त))
    {
      crun_make_error (err, त्रुटि_सं, "opendir `%s`", path);
      परत रिक्त;
    }

  entries = xmalloc (entries_size * आकार (अक्षर *));
  पर्यंत ((de = सूची_पढ़ो (dir)))
    {
      जर (म_भेद (de->d_name, ".") == 0 || म_भेद (de->d_name, "..") == 0)
        सुरूठेवा;
      जर (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * आकार (अक्षर *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * आकार (अक्षर *));
  entries[n_entries] = रिक्त;

  परत entries;
}

पूर्णांक
cpuset_string_to_bitmask (स्थिरांक अक्षर *str, अक्षर **out, माप_प्रकार *out_size, libcrun_error_t *err)
{
  cleanup_free अक्षर *mask = रिक्त;
  माप_प्रकार mask_size = 0;
  स्थिरांक अक्षर *p = str;
  अक्षर *endptr;

  पर्यंत (*p)
    {
      दीर्घ दीर्घ start_range, end_range;

      जर (*p < '0' || *p > '9')
        जा invalid_आदान;

      start_range = strtoll (p, &endptr, 10);
      जर (start_range < 0)
        जा invalid_आदान;

      p = endptr;

      जर (*p != '-')
        end_range = start_range;
      अन्यथा
        {
          p++;

          जर (*p < '0' || *p > '9')
            जा invalid_आदान;

          end_range = strtoll (p, &endptr, 10);

          जर (end_range < start_range)
            जा invalid_आदान;

          p = endptr;
        }

      /* Just set some limit.  */
      जर (end_range > (1 << 20))
        जा invalid_आदान;

      जर (end_range >= (दीर्घ दीर्घ) (mask_size * अक्षर_बिट))
        {
          माप_प्रकार new_mask_size = (end_range / अक्षर_बिट) + 1;
          mask = xrealloc (mask, new_mask_size);
          स_रखो (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      साठी (दीर्घ दीर्घ i = start_range; i <= end_range; i++)
        mask[i / अक्षर_बिट] |= (1 << (i % अक्षर_बिट));

      जर (*p == ',')
        p++;
      अन्यथा जर (*p)
        जा invalid_आदान;
    }

  *out = mask;
  mask = रिक्त;
  *out_size = mask_size;

  परत 0;

invalid_आदान:
  परत crun_make_error (err, 0, "cannot parse input `%s`", str);
}

संरचना channel_fd_pair
{
  संरचना ring_buffer *rb;

  पूर्णांक in_fd;
  पूर्णांक out_fd;

  पूर्णांक infd_epoll_events;
  पूर्णांक outfd_epoll_events;
};

संरचना channel_fd_pair *
channel_fd_pair_new (पूर्णांक in_fd, पूर्णांक out_fd, माप_प्रकार size)
{
  संरचना channel_fd_pair *channel = xmalloc (आकार (संरचना channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  परत channel;
}

शून्य
channel_fd_pair_free (संरचना channel_fd_pair *channel)
{
  जर (channel == रिक्त)
    परत;

  ring_buffer_free (channel->rb);
  मुक्त (channel);
}

पूर्णांक
channel_fd_pair_process (संरचना channel_fd_pair *channel, पूर्णांक epollfd, libcrun_error_t *err)
{
  bool is_आदान_eagain = false, is_output_eagain = false, repeat;
  पूर्णांक ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  साठी (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      जर (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_आदान_eagain, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          जर (ret > 0)
            repeat = true;
        }
      जर (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          जर (ret > 0)
            repeat = true;
        }
    }

  जर (epollfd >= 0)
    {
      माप_प्रकार available = ring_buffer_get_space_available (channel->rb);
      माप_प्रकार used = ring_buffer_get_data_available (channel->rb);
      पूर्णांक events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_आदान_eagain ? EPOLLET : 0)) : 0;
      जर (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      जर (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          channel->outfd_epoll_events = events;
        }
    }
  परत 0;
}

स्थिर पूर्णांक
open_proc (libcrun_error_t *err)
{
  पूर्णांक fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close पूर्णांक fsfd = -1;
  पूर्णांक ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  जर (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, रिक्त, रिक्त, 0);
      जर (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          जर (fd >= 0)
            परत fd;
        }
    }
#endif

  fd = खोलो ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  जर (fd < 0)
    परत crun_make_error (err, त्रुटि_सं, "open `/proc`");
  परत fd;
}

पूर्णांक
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  जर (! container)
    परत open_proc (err);

  जर (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  परत container->proc_fd;
}

पूर्णांक
libcrun_open_proc_file (libcrun_container_t *container, स्थिरांक अक्षर *path, पूर्णांक flags, libcrun_error_t *err)
{
  पूर्णांक proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  जर (proc_fd < 0)
    परत proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = त्रुटि_सं;
  /* requires closing if not cached */
  जर (! container)
    TEMP_FAILURE_RETRY (बंद (proc_fd));

  जर (UNLIKELY (fd < 0))
    परत crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  परत fd;
}

पूर्णांक
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, स्थिरांक अक्षर *path, पूर्णांक flags, libcrun_error_t *err)
{
  cleanup_free अक्षर *full_path = रिक्त;
  xasprintf (&full_path, "%d/%s", pid, path);
  परत libcrun_open_proc_file (container, full_path, flags, err);
}
