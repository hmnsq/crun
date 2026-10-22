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
#यदि HAVE_STDATOMIC_H
#  include <stdatomic.h>
#अन्यथा
#  define atomic_long चञ्चल दीर्घ
#  define atomic_int चञ्चल पूर्णाङ्क
#endif
#अन्तर्भाव "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_जरा
#  define RESOLVE_IN_जरा 0x10
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
crun_path_exists (अचर वर्ण *path, libcrun_error_t *err)
{
  पूर्णाङ्क ret = access (path, F_OK);
  यदि (ret < 0)
    {
      यदि (त्रुटि_सं == ENOENT)
        फर्काऊ 0;
      फर्काऊ crun_make_error (err, त्रुटि_सं, "access `%s`", path);
    }
  फर्काऊ 1;
}

पूर्णाङ्क
xasprintf (वर्ण **str, अचर वर्ण *fmt, ...)
{
  पूर्णाङ्क ret;
  बहु_सूची args_list;

  बहु_शुरू (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  यदि (UNLIKELY (ret < 0))
    OOM ();

  बहु_पूर्ण (args_list);
  फर्काऊ ret;
}

पूर्णाङ्क
write_file_at_with_flags (पूर्णाङ्क dirfd, पूर्णाङ्क flags, mode_t mode, अचर वर्ण *name, अचर शून्य *data, माप_प्रकार len, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ (len > पूर्णांक_उच्च) ? पूर्णांक_उच्च : (पूर्णाङ्क) len;
}

पूर्णाङ्क
detach_process ()
{
  pid_t pid;
  यदि (setsid () < 0)
    फर्काऊ -1;
  pid = विभाजन ();
  यदि (pid < 0)
    फर्काऊ -1;
  यदि (pid != 0)
    _safe_exit (निकास_सफल);
  फर्काऊ 0;
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
  यदि (UNLIKELY (ret < 0))
    {
      यदि (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        गच्छ fallback;

      फर्काऊ ret;
    }
  *mode = stx.stx_mode;
  फर्काऊ ret;

fallback:
#endif
  ret = ख_स्थिति (fd, &st);
  *mode = st.st_mode;
  फर्काऊ ret;
}

पूर्णाङ्क
get_file_type_at (पूर्णाङ्क dirfd, mode_t *mode, bool nofollow, अचर वर्ण *path)
{
  पूर्णाङ्क empty_path = path == रिक्त ? AT_EMPTY_PATH : 0;
  संरचना stat st;
  पूर्णाङ्क ret;

#ifdef HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        गच्छ fallback;

      फर्काऊ ret;
    }
  *mode = stx.stx_mode;
  फर्काऊ ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  फर्काऊ ret;
}

पूर्णाङ्क
get_file_type (mode_t *mode, bool nofollow, अचर वर्ण *path)
{
  फर्काऊ get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

पूर्णाङ्क
create_file_if_missing_at (पूर्णाङ्क dirfd, अचर वर्ण *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  यदि (fd_write < 0)
    {
      पूर्णाङ्क saved_errno = त्रुटि_सं;
      mode_t tmp_mode;
      पूर्णाङ्क ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      यदि (ret == 0 && S_ISREG (tmp_mode))
        फर्काऊ 0;

      फर्काऊ crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
ensure_directory_internal_at (पूर्णाङ्क dirfd, वर्ण *path, माप_प्रकार len, पूर्णाङ्क mode, libcrun_error_t *err)
{
  वर्ण *it = path + len;
  पूर्णाङ्क ret = 0;
  bool parent_created = false;

  लागि (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      यदि (ret == 0 || त्रुटि_सं == EEXIST)
        फर्काऊ 0;

      पूर्णाङ्क saved_errno = त्रुटि_सं;
      यदि (parent_created || त्रुटि_सं != ENOENT)
        {
          libcrun_error_t tmp_err = रिक्त;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          यदि (ret > 0)
            फर्काऊ 0;
          यदि (ret < 0)
            crun_error_release (&tmp_err);

          फर्काऊ crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      जबसम्म (it > path && *it != '/')
        {
          it--;
          len--;
        }
      यदि (it == path)
        फर्काऊ 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      parent_created = true;
    }
  फर्काऊ ret;
}

पूर्णाङ्क
crun_ensure_directory_at (पूर्णाङ्क dirfd, अचर वर्ण *path, पूर्णाङ्क mode, bool nofollow, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, म_माप (tmp), mode, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (ret == 0)
    फर्काऊ crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
check_fd_is_path (अचर वर्ण *path, पूर्णाङ्क fd, अचर वर्ण *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  माप_प्रकार path_len = म_माप (path);
  वर्ण link[PATH_MAX];
  पूर्णाङ्क ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, परिमाण (link)));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "readlink `%s`", fdname);

  यदि (((माप_प्रकार) ret) != path_len || स_भेद (link, path, path_len))
    फर्काऊ crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
check_fd_under_path (अचर वर्ण *rootfs, माप_प्रकार rootfslen, पूर्णाङ्क fd, अचर वर्ण *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  वर्ण link[PATH_MAX];
  पूर्णाङ्क ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  यदि (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    फर्काऊ 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, परिमाण (link)));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "readlink `%s`", fdname);

  यदि (((माप_प्रकार) ret) <= rootfslen || स_भेद (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    फर्काऊ crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  फर्काऊ 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
स्थिर शून्य
close_and_replace (पूर्णाङ्क *oldfd, पूर्णाङ्क newfd)
{
  यदि (*oldfd >= 0)
    TEMP_FAILURE_RETRY (बंद (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
वर्ण *chroot_realpath (अचर वर्ण *chroot, अचर वर्ण *path, वर्ण resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
स्थिर पूर्णाङ्क
safe_openat_fallback (पूर्णाङ्क dirfd, अचर वर्ण *rootfs, अचर वर्ण *path, पूर्णाङ्क flags,
                      पूर्णाङ्क mode, libcrun_error_t *err)
{
  cleanup_free वर्ण *parent_path = रिक्त;
  अचर वर्ण *last_component = रिक्त;
  अचर वर्ण *orig_path = path;
  अचर वर्ण *path_in_chroot;
  cleanup_close पूर्णाङ्क fd = -1;
  वर्ण resolved[PATH_MAX];
  वर्ण buffer[PATH_MAX];
  माप_प्रकार rootfs_len = safe_strlen (rootfs);
  पूर्णाङ्क ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  यदि (flags & O_NOFOLLOW)
    {
      वर्ण *sep;

      parent_path = xstrdup (path);
      sep = म_खोजप (parent_path, '/');
      यदि (sep == रिक्त)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      अन्यथा यदि (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      यदि (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  यदि (path_in_chroot == रिक्त)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  यदि (rootfs_len > 0 && म_भेदन (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  यदि (last_component)
    {
      ret = snprintf (resolved, परिमाण (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (resolved)))
        फर्काऊ crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  यदि (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "dup `%s`", rootfs_len ? rootfs : "/");
      फर्काऊ ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = fd;
  fd = -1;
  फर्काऊ ret;
}

पूर्णाङ्क
safe_openat (पूर्णाङ्क dirfd, अचर वर्ण *rootfs, अचर वर्ण *path, पूर्णाङ्क flags, पूर्णाङ्क mode,
             libcrun_error_t *err)
{
  स्थिर bool openat2_supported = true;
  पूर्णाङ्क retries = MAX_OPENAT2_EAGAIN_RETRIES;
  पूर्णाङ्क ret;

  यदि (is_empty_string (path))
    {
      cleanup_close पूर्णाङ्क fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      यदि (is_empty_string (rootfs))
        rootfs = "/";

      fd = खोलो (rootfs, flags, mode);
      यदि (UNLIKELY (fd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      यदि (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }

      ret = fd;
      fd = -1;
      फर्काऊ ret;
    }

  यदि (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_जरा);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (त्रुटि_सं == EINTR)
            गच्छ repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          यदि (त्रुटि_सं == EAGAIN && retries-- > 0)
            गच्छ repeat;
          यदि (त्रुटि_सं == ENOSYS)
            openat2_supported = false;
          यदि (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL || त्रुटि_सं == EPERM)
            फर्काऊ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          फर्काऊ crun_make_error (err, त्रुटि_सं, "openat2 `%s`", path);
        }

      फर्काऊ ret;
    }

  फर्काऊ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (पूर्णाङ्क dfd, अचर वर्ण *name, वर्ण **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free वर्ण *tmp_buf = रिक्त;
  ssize_t size;

  गर
    {
      यदि (tmp_buf != रिक्त)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      यदि (UNLIKELY (size < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "readlink `%s`", name);
  } जबसम्म (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = रिक्त;

  फर्काऊ size;
}

स्थिर पूर्णाङ्क crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, पूर्णाङ्क dirfd, अचर वर्ण *dirpath,
                                अचर वर्ण *path, पूर्णाङ्क mode, पूर्णाङ्क max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
स्थिर पूर्णाङ्क
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, पूर्णाङ्क dirfd, अचर वर्ण *dirpath, पूर्णाङ्क cwd,
                                 अचर वर्ण *npath, अचर वर्ण *cur, अचर वर्ण *rest,
                                 पूर्णाङ्क mode, पूर्णाङ्क max_readlinks, libcrun_error_t *err)
{
  cleanup_free वर्ण *target = रिक्त;
  cleanup_free वर्ण *new_path = रिक्त;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (पूर्णाङ्क) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  फर्काऊ crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

स्थिर पूर्णाङ्क
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, पूर्णाङ्क dirfd, अचर वर्ण *dirpath,
                     अचर वर्ण *path, पूर्णाङ्क mode, पूर्णाङ्क max_readlinks, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क wd_cleanup = -1;
  cleanup_free वर्ण *npath = रिक्त;
  bool last_component = false;
  माप_प्रकार depth = 0;
  वर्ण *cur;
  वर्ण *it;
  पूर्णाङ्क cwd;
  पूर्णाङ्क ret;

  यदि (max_readlinks <= 0)
    फर्काऊ crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  यदि (*path == '\0')
    {
      यदि (do_open)
        फर्काऊ खोलो (dirpath, O_CLOEXEC | O_PATH, 0);
      फर्काऊ 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = म_अक्षर (npath, '/');
  जबसम्म (cur)
    {
      यदि (it)
        *it = '\0';
      अन्यथा
        last_component = true;

      यदि (cur[0] == '\0')
        रोक;

      यदि (म_भेद (cur, ".") == 0)
        गच्छ next;
      अन्यथा यदि (म_भेद (cur, ".."))
        depth++;
      अन्यथा
        {
          यदि (depth)
            depth--;
          अन्यथा
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              गच्छ next;
            }
        }

      यदि (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          यदि (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              यदि (त्रुटि_सं == ELOOP)
                फर्काऊ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, रिक्त,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              यदि (ret < 0)
                फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s/%s`", dirpath, npath);
            }

          यदि (do_open)
            फर्काऊ ret;

          close_and_replace (&wd_cleanup, ret);
          फर्काऊ 0;
        }

      ret = mkdirat (cwd, cur, mode);
      यदि (ret < 0)
        {
          mode_t st_mode;

          यदि (त्रुटि_सं != EEXIST)
            फर्काऊ crun_make_error (err, त्रुटि_सं, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          यदि (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            फर्काऊ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : रिक्त, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      यदि (UNLIKELY (cwd < 0))
        फर्काऊ crun_error_wrap (err, "creating `/%s`", path);

      यदि (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, रिक्त);
          यदि (UNLIKELY (ret < 0))
            {
              पूर्णाङ्क saved_errno = त्रुटि_सं;

              बंद (cwd);
              फर्काऊ crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          यदि ((st_mode & S_IFMT) != S_IFDIR)
            {
              बंद (cwd);
              फर्काऊ crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      यदि (it == रिक्त)
        रोक;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = म_अक्षर (cur, '/');
    }

  यदि (do_open)
    {
      यदि (cwd == dirfd)
        {
          ret = dup (dirfd);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "dup `%s`", dirpath);
          फर्काऊ ret;
        }

      wd_cleanup = -1;
      फर्काऊ cwd;
    }

  फर्काऊ 0;
}

पूर्णाङ्क
crun_safe_create_and_open_ref_at (bool dir, पूर्णाङ्क dirfd, अचर वर्ण *dirpath, अचर वर्ण *path, पूर्णाङ्क mode, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  यदि (LIKELY (ret >= 0))
    फर्काऊ ret;

  crun_error_release (err);
  फर्काऊ crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णाङ्क
crun_safe_ensure_directory_at (पूर्णाङ्क dirfd, अचर वर्ण *dirpath, अचर वर्ण *path, पूर्णाङ्क mode,
                               libcrun_error_t *err)
{
  फर्काऊ crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णाङ्क
crun_safe_ensure_file_at (पूर्णाङ्क dirfd, अचर वर्ण *dirpath, अचर वर्ण *path, पूर्णाङ्क mode,
                          libcrun_error_t *err)
{
  फर्काऊ crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णाङ्क
crun_ensure_directory (अचर वर्ण *path, पूर्णाङ्क mode, bool nofollow, libcrun_error_t *err)
{
  फर्काऊ crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
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
  यदि (UNLIKELY (ret < 0))
    {
      यदि (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        गच्छ fallback;
      फर्काऊ ret;
    }
  *size = stx.stx_size;

  फर्काऊ ret;

fallback:
#endif
  ret = ख_स्थिति (fd, &st);
  *size = st.st_size;
  फर्काऊ ret;
}

पूर्णाङ्क
crun_dir_p_at (पूर्णाङ्क dirfd, अचर वर्ण *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  पूर्णाङ्क ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "stat `%s`", path);

  फर्काऊ S_ISDIR (mode) ? 1 : 0;
}

पूर्णाङ्क
crun_dir_p (अचर वर्ण *path, bool nofollow, libcrun_error_t *err)
{
  फर्काऊ crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
स्थिर पूर्णाङ्क
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free वर्ण *buffer = रिक्त;
  माप_प्रकार len;
  पूर्णाङ्क ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  यदि (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      यदि (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          फर्काऊ 0;
        }
      फर्काऊ ret;
    }

  फर्काऊ म_माला (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
स्थिर bool
running_in_openvz ()
{
  यदि (access ("/proc/vz", F_OK) < 0)
    फर्काऊ false;

  फर्काऊ access ("/proc/bc", F_OK) < 0 && त्रुटि_सं == ENOENT;
}

पूर्णाङ्क
check_running_in_user_namespace (libcrun_error_t *err)
{
  स्थिर atomic_int run_in_userns = -1;
  संरचना stat st;
  पूर्णाङ्क ret;

  ret = run_in_userns;
  यदि (ret >= 0)
    फर्काऊ ret;

  ret = stat ("/proc/self/ns/user", &st);
  यदि (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      यदि (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          फर्काऊ run_in_userns;
        }

      यदि (! running_in_openvz ())
        {
          run_in_userns = 1;
          फर्काऊ run_in_userns;
        }
    }
  अन्यथा यदि (UNLIKELY (त्रुटि_सं != ENOENT))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  run_in_userns = ret;
  फर्काऊ run_in_userns;
}

स्थिर माप_प्रकार
get_page_size ()
{
  स्थिर atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  यदि (pagesize == 0)
    {
      pagesize = (दीर्घ) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  फर्काऊ (माप_प्रकार) pagesize;
}

स्थिर पूर्णाङ्क selinux_enabled = -1;
स्थिर पूर्णाङ्क apparmor_enabled = -1;

पूर्णाङ्क
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free वर्ण *out = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  माप_प्रकार len;
  पूर्णाङ्क ret;

  यदि (selinux_enabled >= 0)
    फर्काऊ selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  selinux_enabled = म_माला (out, "selinux") ? 1 : 0;

  फर्काऊ selinux_enabled;
}

पूर्णाङ्क
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क size;
  वर्ण buf[1];

  यदि (apparmor_enabled >= 0)
    फर्काऊ apparmor_enabled;

  fd = खोलो ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  यदि (fd == -1)
    {
      यदि (त्रुटि_सं == ENOENT)
        {
          apparmor_enabled = 0;
          फर्काऊ 0;
        }

      फर्काऊ crun_make_error (err, त्रुटि_सं, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (पठ (fd, buf, परिमाण (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  फर्काऊ apparmor_enabled;
}

स्थिर पूर्णाङ्क
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  यदि (selinux_enabled < 0)
    फर्काऊ crun_make_error (err, 0, "SELinux is not initialized correctly");
  फर्काऊ selinux_enabled;
}

पूर्णाङ्क
add_selinux_mount_label (वर्ण **retlabel, अचर वर्ण *data, अचर वर्ण *label, अचर वर्ण *context_type, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_is_selinux_enabled (err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (label && ret)
    {
      यदि (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      अन्यथा
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      फर्काऊ 0;
    }
  *retlabel = xstrdup (data);
  फर्काऊ 0;
}

स्थिर अचर वर्ण *
lsm_attr_path (libcrun_container_t *container, अचर वर्ण *lsm, अचर वर्ण *fname, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क attr_dirfd = -1;
  cleanup_close पूर्णाङ्क lsm_dirfd = -1;
  वर्ण *attr_path = रिक्त;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  यदि (UNLIKELY (attr_dirfd < 0))
    फर्काऊ रिक्त;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  यदि (lsm != रिक्त)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      यदि (UNLIKELY (lsm_dirfd < 0 && त्रुटि_सं != ENOENT))
        {
          crun_make_error (err, त्रुटि_सं, "open `/proc/thread-self/attr/%s`", lsm);
          फर्काऊ रिक्त;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  फर्काऊ attr_path;
}

स्थिर पूर्णाङ्क
check_proc_super_magic (पूर्णाङ्क fd, अचर वर्ण *path, libcrun_error_t *err)
{
  संरचना statfs sfs;

  पूर्णाङ्क ret = fstatfs (fd, &sfs);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "statfs `%s`", path);

  यदि (sfs.f_type != PROC_SUPER_MAGIC)
    फर्काऊ crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
set_security_attr (libcrun_container_t *container, अचर वर्ण *lsm, अचर वर्ण *fname, अचर वर्ण *data, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  cleanup_free अचर वर्ण *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close पूर्णाङ्क fd = -1;

  यदि (UNLIKELY (attr_path == रिक्त))
    फर्काऊ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (लिख (fd, data, म_माप (data)));
  यदि (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      यदि (त्रुटि_सं == ENOENT && lsm != रिक्त && म_भेद (lsm, "apparmor") == 0)
        {
          अचर वर्ण *profile = म_अक्षर (data, ' ');

          फर्काऊ crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      फर्काऊ crun_make_error (err, त्रुटि_सं, "write to file `%s`", attr_path);
    }

  फर्काऊ 0;
}

पूर्णाङ्क
set_selinux_label (libcrun_container_t *container, अचर वर्ण *label, bool now, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_is_selinux_enabled (err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (ret)
    फर्काऊ set_security_attr (container, रिक्त, now ? "current" : "exec", label, err);
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  यदि (apparmor_enabled < 0)
    फर्काऊ crun_make_error (err, 0, "AppArmor is not initialized correctly");
  फर्काऊ apparmor_enabled;
}

स्थिर पूर्णाङ्क
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (परिमाण (UNCONFINED) - 1)
  cleanup_free अचर वर्ण *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close पूर्णाङ्क fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  वर्ण buf[UNCONFINED_LEN + 1];

  यदि (UNLIKELY (attr_path == रिक्त))
    फर्काऊ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ fd;

  यदि (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    फर्काऊ -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (पठ (fd, buf, परिमाण (buf)));
  यदि (UNLIKELY (bytes_read < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  यदि (bytes_read < UNCONFINED_LEN || स_भेद (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    फर्काऊ 1;
  यदि (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    फर्काऊ 0;
  फर्काऊ 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

पूर्णाङ्क
set_apparmor_profile (libcrun_container_t *container, अचर वर्ण *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_is_apparmor_enabled (err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (ret)
    {
      cleanup_free वर्ण *buf = रिक्त;
      ret = is_current_process_confined (container, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      फर्काऊ set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
read_all_fd_internal (पूर्णाङ्क fd, अचर वर्ण *description, वर्ण **out, माप_प्रकार *len, माप_प्रकार size_hint, माप_प्रकार max_size, libcrun_error_t *err)
{
  cleanup_free वर्ण *buf = रिक्त;
  माप_प्रकार nread, allocated;
  माप_प्रकार pagesize = 0;
  off_t size = 0;
  पूर्णाङ्क ret;

  यदि (size_hint)
    allocated = size_hint;
  अन्यथा
    {
      ret = get_file_size (fd, &size);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "stat `%s`", description);

      यदि (max_size && (माप_प्रकार) size > max_size)
        फर्काऊ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  जबसम्म ((size && nread < (माप_प्रकार) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (पठ (fd, buf + nread, allocated - nread));
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "read from file `%s`", description);

      यदि (ret == 0)
        रोक;

      nread += ret;

      यदि (max_size && nread > max_size)
        फर्काऊ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      यदि (nread == allocated)
        {
          यदि (size)
            रोक;

          यदि (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  यदि (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      वर्ण *tmp = पुनः_स्मृति (buf, nread + 1);
      यदि (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = रिक्त;
  यदि (len)
    *len = nread;
  फर्काऊ 0;
}

पूर्णाङ्क
read_all_fd_with_size_hint (पूर्णाङ्क fd, अचर वर्ण *description, वर्ण **out, माप_प्रकार *len, माप_प्रकार size_hint, libcrun_error_t *err)
{
  फर्काऊ read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

पूर्णाङ्क
read_all_fd_with_max_size (पूर्णाङ्क fd, अचर वर्ण *description, वर्ण **out, माप_प्रकार *len, माप_प्रकार max_size, libcrun_error_t *err)
{
  फर्काऊ read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

पूर्णाङ्क
read_all_file_at (पूर्णाङ्क dirfd, अचर वर्ण *path, वर्ण **out, माप_प्रकार *len, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", path);

  फर्काऊ read_all_fd (fd, path, out, len, err);
}

पूर्णाङ्क
read_all_file (अचर वर्ण *path, वर्ण **out, माप_प्रकार *len, libcrun_error_t *err)
{
  यदि (म_भेद (path, "-") == 0)
    path = "/dev/stdin";

  फर्काऊ read_all_file_at (AT_FDCWD, path, out, len, err);
}

पूर्णाङ्क
get_realpath_to_file (पूर्णाङ्क dirfd, अचर वर्ण *path_name, वर्ण **absolute_path, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  यदि (UNLIKELY (targetfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", path_name);
  अन्यथा
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      यदि (UNLIKELY (len < 0))
        फर्काऊ crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  फर्काऊ 0;
}

पूर्णाङ्क
open_unix_domain_client_socket (अचर वर्ण *path, पूर्णाङ्क dgram, libcrun_error_t *err)
{
  संरचना sockaddr_un addr = {};
  पूर्णाङ्क ret;
  proc_fd_path_t name_buf;
  cleanup_close पूर्णाङ्क destfd = -1;
  cleanup_close पूर्णाङ्क fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "create UNIX socket");

  यदि (म_माप (path) >= परिमाण (addr.sun_path))
    {
      destfd = खोलो (path, O_PATH | O_CLOEXEC);
      यदि (UNLIKELY (destfd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  माप_प्रकार path_len = म_माप (path);
  यदि (path_len >= परिमाण (addr.sun_path))
    फर्काऊ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  स_नकल (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (संरचना sockaddr *) &addr, परिमाण (addr));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  फर्काऊ ret;
}

पूर्णाङ्क
open_unix_domain_socket (अचर वर्ण *path, पूर्णाङ्क dgram, libcrun_error_t *err)
{
  संरचना sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क dirfd = -1;
  cleanup_close पूर्णाङ्क fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "create UNIX socket");

  यदि (म_माप (path) >= परिमाण (addr.sun_path))
    {
      cleanup_free वर्ण *dpath = xstrdup (path);
      cleanup_free वर्ण *bpath = xstrdup (path);
      अचर वर्ण *parent_dir = स_नाम (dpath);
      अचर वर्ण *base = basename (bpath);
      पूर्णाङ्क n;

      dirfd = खोलो (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      यदि (UNLIKELY (dirfd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, परिमाण (addr.sun_path), "%s/%s", name_buf, base);
      यदि (n < 0 || (माप_प्रकार) n >= परिमाण (addr.sun_path))
        फर्काऊ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  अन्यथा
    {
      माप_प्रकार path_len = म_माप (path);
      स_नकल (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (संरचना sockaddr *) &addr, परिमाण (addr));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "bind socket to `%s`", path);

  यदि (! dgram)
    {
      ret = listen (fd, 1);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "listen on socket");
    }

  ret = fd;
  fd = -1;

  फर्काऊ ret;
}

पूर्णाङ्क
send_fd_to_socket (पूर्णाङ्क server, पूर्णाङ्क fd, libcrun_error_t *err)
{
  फर्काऊ send_fd_to_socket_with_payload (server, fd, रिक्त, 0, err);
}

पूर्णाङ्क
send_fd_to_socket_with_payload (पूर्णाङ्क server, पूर्णाङ्क fd, अचर वर्ण *payload, माप_प्रकार payload_len, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  संरचना cmsghdr *cmsg = रिक्त;
  संरचना iovec iov[2];
  संरचना msghdr सन्देश = {};
  वर्ण ctrl_buf[CMSG_SPACE (1 + परिमाण (पूर्णाङ्क))] = {};
  वर्ण data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = परिमाण (data);

  यदि (payload_len > 0)
    {
      iov[0].iov_base = (शून्य *) payload;
      iov[0].iov_len = payload_len;
    }

  सन्देश.सन्देश_name = रिक्त;
  सन्देश.सन्देश_namelen = 0;
  सन्देश.सन्देश_iov = iov;
  सन्देश.सन्देश_iovlen = 1;
  सन्देश.सन्देश_controllen = CMSG_SPACE (परिमाण (पूर्णाङ्क));
  सन्देश.सन्देश_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&सन्देश);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (परिमाण (पूर्णाङ्क));

  *((पूर्णाङ्क *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &सन्देश, 0));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "sendmsg");
  फर्काऊ 0;
}

पूर्णाङ्क
receive_fd_from_socket_with_payload (पूर्णाङ्क from, वर्ण *payload, माप_प्रकार payload_len, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;
  संरचना iovec iov[1];
  संरचना msghdr सन्देश = {};
  वर्ण ctrl_buf[CMSG_SPACE (परिमाण (पूर्णाङ्क))] = {};
  वर्ण data[1];
  संरचना cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = परिमाण (data);

  यदि (payload_len > 0)
    {
      iov[0].iov_base = (शून्य *) payload;
      iov[0].iov_len = payload_len;
    }

  सन्देश.सन्देश_name = रिक्त;
  सन्देश.सन्देश_namelen = 0;
  सन्देश.सन्देश_iov = iov;
  सन्देश.सन्देश_iovlen = 1;
  सन्देश.सन्देश_controllen = CMSG_SPACE (परिमाण (पूर्णाङ्क));
  सन्देश.सन्देश_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &सन्देश, 0));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "recvmsg");
  यदि (UNLIKELY (ret == 0))
    फर्काऊ crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&सन्देश);
  यदि (cmsg == रिक्त)
    फर्काऊ crun_make_error (err, 0, "no msg received");
  स_नकल (&fd, CMSG_DATA (cmsg), परिमाण (fd));

  ret = fd;
  fd = -1;
  फर्काऊ ret;
}

पूर्णाङ्क
receive_fd_from_socket (पूर्णाङ्क from, libcrun_error_t *err)
{
  फर्काऊ receive_fd_from_socket_with_payload (from, रिक्त, 0, err);
}

पूर्णाङ्क
create_socket_pair (पूर्णाङ्क *pair, libcrun_error_t *err)
{
  पूर्णाङ्क ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "socketpair");
  फर्काऊ 0;
}

पूर्णाङ्क
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  पूर्णाङ्क ret = signalfd (-1, mask, 0);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "signalfd");
  फर्काऊ ret;
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
  यदि (UNLIKELY (ret < 0 && त्रुटि_सं == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  यदि (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      यदि (त्रुटि_सं == ENOENT || त्रुटि_सं == EPERM)
        फर्काऊ 0;
      फर्काऊ crun_make_error (err, त्रुटि_सं, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  फर्काऊ 0;
}

पूर्णाङ्क
epoll_helper (पूर्णाङ्क *in_fds, पूर्णाङ्क *in_levelfds, पूर्णाङ्क *out_fds, पूर्णाङ्क *out_levelfds, libcrun_error_t *err)
{
  संरचना epoll_event ev;
  cleanup_close पूर्णाङ्क epollfd = -1;
  पूर्णाङ्क ret;

  पूर्णाङ्क *it;
  epollfd = epoll_create1 (0);
  यदि (UNLIKELY (epollfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  लागि (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      यदि (UNLIKELY (ret < 0))                                           \
        फर्काऊ crun_make_error (err, त्रुटि_सं, "epoll_ctl add `%d`", *it); \
    }

  यदि (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  यदि (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  यदि (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  यदि (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  फर्काऊ ret;
}

पूर्णाङ्क
copy_from_fd_to_fd (पूर्णाङ्क src, पूर्णाङ्क dst, पूर्णाङ्क consume, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  ssize_t nread;
  माप_प्रकार pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  गर
    {
      cleanup_free वर्ण *buffer = रिक्त;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      यदि (can_copy_file_range)
        {
          nread = copy_file_range (src, रिक्त, dst, रिक्त, pagesize, 0);
          यदि (nread < 0 && (त्रुटि_सं == EINVAL || त्रुटि_सं == EXDEV))
            {
              can_copy_file_range = false;
              गच्छ fallback;
            }
          यदि (consume && nread < 0 && त्रुटि_सं == EAGAIN)
            फर्काऊ 0;
          यदि (nread < 0 && त्रुटि_सं == EIO)
            फर्काऊ 0;
          यदि (UNLIKELY (nread < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "copy_file_range");
          जारी;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (पठ (src, buffer, pagesize));
      यदि (consume && nread < 0 && त्रुटि_सं == EAGAIN)
        फर्काऊ 0;
      यदि (nread < 0 && त्रुटि_सं == EIO)
        फर्काऊ 0;
      यदि (UNLIKELY (nread < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "read");

      remaining = nread;
      जबसम्म (remaining)
        {
          ret = TEMP_FAILURE_RETRY (लिख (dst, buffer + nread - remaining, remaining));
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "write");
          remaining -= ret;
        }
  } जबसम्म (consume && nread);

  फर्काऊ 0;
}

पूर्णाङ्क
run_process (वर्ण **args, libcrun_error_t *err)
{
  pid_t pid = विभाजन ();
  यदि (UNLIKELY (pid < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fork");
  यदि (pid)
    {
      पूर्णाङ्क r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      यदि (r < 0)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "waitpid");
      यदि (WIFEXITED (status) || WIFSIGNALED (status))
        फर्काऊ WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (निकास_त्रुटि);
}

#ifndef HAVE_FGETPWENT_R
स्थिर अचिन्हित
atou (वर्ण **s)
{
  अचिन्हित x;
  लागि (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  फर्काऊ x;
}

पूर्णाङ्क
fgetpwent_r (खाता *f, संरचना passwd *pw, वर्ण *पङ्क्ति, माप_प्रकार size, संरचना passwd **res)
{
  वर्ण *s;
  पूर्णाङ्क rv = 0;
  लागि (;;)
    {
      पङ्क्ति[size - 1] = '\xff';
      यदि ((ख_माला_लो (पङ्क्ति, size, f) == रिक्त) || ख_त्रुटि (f) || पङ्क्ति[size - 1] != '\xff')
        {
          rv = (पङ्क्ति[size - 1] != '\xff') ? दुस्फल : ENOENT;
          पङ्क्ति = 0;
          pw = 0;
          रोक;
        }
      पङ्क्ति[म_खोज (पङ्क्ति, "\n")] = 0;

      s = पङ्क्ति;
      pw->pw_name = s++;
      यदि (! (s = म_अक्षर (s, ':')))
        जारी;

      *s++ = 0;
      pw->pw_passwd = s;
      यदि (! (s = म_अक्षर (s, ':')))
        जारी;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      यदि (*s != ':')
        जारी;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      यदि (*s != ':')
        जारी;

      *s++ = 0;
      pw->pw_gecos = s;
      यदि (! (s = म_अक्षर (s, ':')))
        जारी;

      *s++ = 0;
      pw->pw_dir = s;
      यदि (! (s = म_अक्षर (s, ':')))
        जारी;

      *s++ = 0;
      pw->pw_shell = s;
      रोक;
    }
  *res = pw;
  यदि (rv)
    त्रुटि_सं = rv;
  फर्काऊ rv;
}
#endif

पूर्णाङ्क
set_home_env (uid_t id)
{
  संरचना passwd pwd;
  cleanup_free वर्ण *buf = रिक्त;
  दीर्घ buf_size;
  cleanup_file खाता *stream = रिक्त;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  यदि (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = ख_खोलो ("/etc/passwd", "re");
  यदि (stream == रिक्त)
    फर्काऊ -1;

  लागि (;;)
    {
      संरचना passwd *ret_pw = रिक्त;

      पूर्णाङ्क ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      यदि (UNLIKELY (ret != 0))
        {
          यदि (ret == EINTR)
            जारी;

          यदि (ret != दुस्फल)
            {
              /* Let callers handle the error if the user was not found. */
              त्रुटि_सं = ret;
              फर्काऊ -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          जारी;
        }

      यदि (ret_pw && ret_pw->pw_uid == id)
        {
          यदि (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          अन्यथा
            फर्काऊ 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
स्थिर पूर्णाङ्क
getsubidrange (uid_t id, पूर्णाङ्क is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file खाता *आगत = रिक्त;
  cleanup_free वर्ण *lineptr = रिक्त;
  माप_प्रकार lenlineptr = 0, len_name;
  दीर्घ buf_size;
  cleanup_free वर्ण *buf = रिक्त;
  अचर वर्ण *name;
  संरचना passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  यदि (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  लागि (;;)
    {
      पूर्णाङ्क ret;
      संरचना passwd *ret_pw = रिक्त;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      यदि (LIKELY (ret == 0))
        {
          यदि (ret_pw)
            {
              name = ret_pw->pw_name;
              रोक;
            }
          फर्काऊ -1;
        }

      यदि (ret != दुस्फल)
        फर्काऊ -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = म_माप (name);

  आगत = ख_खोलो (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  यदि (आगत == रिक्त)
    फर्काऊ -1;

  लागि (;;)
    {
      वर्ण *endptr;
      ssize_t पठ = getline (&lineptr, &lenlineptr, आगत);
      यदि (पठ < 0)
        फर्काऊ -1;

      यदि (पठ < (ssize_t) (len_name + 2))
        जारी;

      यदि (स_भेद (lineptr, name, len_name) || lineptr[len_name] != ':')
        जारी;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      यदि (endptr >= &lineptr[पठ])
        फर्काऊ -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      फर्काऊ 0;
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

  यदि (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    फर्काऊ 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  यदि (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      यदि (UNLIKELY (ret >= remaining))
        फर्काऊ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  यदि (UNLIKELY (ret >= remaining))
    फर्काऊ crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  यदि (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      यदि (UNLIKELY (ret >= remaining))
        फर्काऊ crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = रिक्त;
  फर्काऊ written;
}

स्थिर पूर्णाङ्क
unset_cloexec_flag (पूर्णाङ्क fd)
{
  पूर्णाङ्क flags = fcntl (fd, F_GETFD);
  यदि (flags == -1)
    फर्काऊ -1;

  flags &= ~FD_CLOEXEC;

  फर्काऊ fcntl (fd, F_SETFD, flags);
}

स्थिर शून्य __attribute__ ((__noreturn__))
run_process_child (वर्ण *path, वर्ण **args, अचर वर्ण *cwd, वर्ण **envp, पूर्णाङ्क pipe_r,
                   पूर्णाङ्क pipe_w, पूर्णाङ्क out_fd, पूर्णाङ्क err_fd, bool can_ignore_chdir_errors)
{
  वर्ण *tmp_args[] = { path, रिक्त };
  libcrun_error_t err = रिक्त;
  पूर्णाङ्क dev_null_fd = -1;
  पूर्णाङ्क ret;

  ret = mark_or_close_fds_ge_than (रिक्त, 3, false, &err);
  यदि (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->सन्देश);

  यदि (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = खोलो ("/dev/null", O_WRONLY | O_CLOEXEC);
      यदि (UNLIKELY (dev_null_fd < 0))
        _safe_exit (निकास_त्रुटि);
    }

  TEMP_FAILURE_RETRY (बंद (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (बंद (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  यदि (out_fd >= 0)
    unset_cloexec_flag (1);
  यदि (err_fd >= 0)
    unset_cloexec_flag (2);

  यदि (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (बंद (dev_null_fd));
  यदि (out_fd >= 0)
    TEMP_FAILURE_RETRY (बंद (out_fd));
  यदि (err_fd >= 0)
    TEMP_FAILURE_RETRY (बंद (err_fd));

  यदि (args == रिक्त)
    args = tmp_args;

  यदि (cwd && स_बदलो (cwd) < 0)
    {
      यदि (! can_ignore_chdir_errors || (त्रुटि_सं != EACCES && त्रुटि_सं != EPERM))
        _safe_exit (निकास_त्रुटि);
    }

  execvpe (path, args, envp);
  _safe_exit (निकास_त्रुटि);
}

/* It changes the signals mask for the current process.  */
पूर्णाङ्क
run_process_with_stdin_timeout_envp (वर्ण *path, वर्ण **args, अचर वर्ण *cwd, पूर्णाङ्क timeout,
                                     वर्ण **envp, वर्ण *मानक_निवेश, माप_प्रकार stdin_len, पूर्णाङ्क out_fd,
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
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  यदि (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "sigprocmask");
    }

  pid = विभाजन ();
  यदि (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, त्रुटि_सं, "fork");
      गच्छ restore_sig_mask_and_exit;
    }

  यदि (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (लिख (pipe_w, मानक_निवेश, stdin_len));
  यदि (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      यदि (त्रुटि_सं != EPIPE)
        {
          ret = crun_make_error (err, त्रुटि_सं, "write to pipe");
          गच्छ restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  यदि (timeout)
    {
      समय_प्रकार start = समय (रिक्त);
      समय_प्रकार now;
      लागि (now = start; now - start < timeout; now = समय (रिक्त))
        {
          siginfo_t info;
          पूर्णाङ्क elapsed = now - start;
          संरचना timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          यदि (UNLIKELY (ret < 0 && त्रुटि_सं != EAGAIN))
            {
              ret = crun_make_error (err, त्रुटि_सं, "sigtimedwait");
              गच्छ restore_sig_mask_and_exit;
            }

          यदि (info.si_signo == SIGCHLD && info.si_pid == pid)
            गच्छ read_waitpid;

          यदि (ret < 0 && त्रुटि_सं == EAGAIN)
            गच्छ timeout;
        }
    timeout:
      समाप्त (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      गच्छ restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  यदि (r < 0)
    ret = crun_make_error (err, त्रुटि_सं, "waitpid");
  अन्यथा
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  यदि (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      यदि (pid > 0)
        {
          समाप्त (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, रिक्त);
      यदि (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, त्रुटि_सं, "restoring signal mask with sigprocmask");
    }
  फर्काऊ ret;
}

पूर्णाङ्क
mark_or_close_fds_ge_than (libcrun_container_t *container, पूर्णाङ्क n, bool close_now, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क cfd = -1;
  cleanup_dir सूची *dir = रिक्त;
  पूर्णाङ्क ret;
  पूर्णाङ्क fd;
  संरचना dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  यदि (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, अच_पूर्णांक_उच्च, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  यदि (ret == 0)
    {
      यदि (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      फर्काऊ 0;
    }
  यदि (ret < 0 && त्रुटि_सं != EINVAL && त्रुटि_सं != ENOSYS && त्रुटि_सं != EPERM)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  यदि (UNLIKELY (cfd < 0))
    फर्काऊ cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  dir = fdopendir (cfd);
  यदि (UNLIKELY (dir == रिक्त))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  लागि (next = सूची_पढ़ो (dir); next; next = सूची_पढ़ो (dir))
    {
      पूर्णाङ्क val;
      अचर वर्ण *name = next->d_name;
      यदि (name[0] == '.')
        जारी;

      val = strtoll (name, रिक्त, 10);
      यदि (val < n || val == fd)
        जारी;

      यदि (close_now)
        {
          ret = बंद (val);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "close fd `%d`", val);

          यदि (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      अन्यथा
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  फर्काऊ 0;
}

शून्य
get_current_timestamp (वर्ण *out, माप_प्रकार len)
{
  संरचना timeval tv;
  संरचना पंचांग now;
  वर्ण timestamp[64];

  gettimeofday (&tv, रिक्त);
  gmtime_r (&tv.tv_sec, &now);
  स_माला (timestamp, परिमाण (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (शून्य) snprintf (out, len, "%s.%06lldZ", timestamp, (दीर्घ दीर्घ पूर्णाङ्क) tv.tv_usec);
  out[len - 1] = '\0';
}

पूर्णाङ्क
set_blocking_fd (पूर्णाङ्क fd, bool blocking, libcrun_error_t *err)
{
  पूर्णाङ्क ret, flags = fcntl (fd, F_GETFL, 0);
  यदि (UNLIKELY (flags < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fcntl");
  फर्काऊ 0;
}

पूर्णाङ्क
parse_json_file (json_object **out, अचर वर्ण *jsondata, संरचना parser_context *ctx arg_unused, libcrun_error_t *err)
{
  संरचना json_tokener *tok;
  गणना json_tokener_error jerr;

  *err = रिक्त;

  यदि (UNLIKELY (jsondata == रिक्त))
    फर्काऊ crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  यदि (UNLIKELY (tok == रिक्त))
    फर्काऊ crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (पूर्णाङ्क) म_माप (jsondata));
  jerr = json_tokener_get_error (tok);
  यदि (*out == रिक्त || jerr != json_tokener_success)
    {
      पूर्णाङ्क saved_pos = (पूर्णाङ्क) tok->char_offset;

      json_tokener_free (tok);
      यदि (*out)
        {
          json_object_put (*out);
          *out = रिक्त;
        }
      फर्काऊ crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  फर्काऊ 0;
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
check_access (अचर वर्ण *path)
{
  पूर्णाङ्क ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#अन्यथा
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  यदि (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      यदि (त्रुटि_सं == EACCES)
        {
          पूर्णाङ्क saved_errno = त्रुटि_सं;
          ret = ACCESS (path, F_OK);
          त्रुटि_सं = saved_errno;

          यदि (ret == 0)
            फर्काऊ CHECK_ACCESS_NOT_EXECUTABLE;
        }
      फर्काऊ -त्रुटि_सं;
    }

  ret = get_file_type (&mode, false, path);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ -त्रुटि_सं;

  यदि (! S_ISREG (mode))
    फर्काऊ CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  फर्काऊ 0;
}

पूर्णाङ्क
find_executable (वर्ण **out, अचर वर्ण *executable_path, अचर वर्ण *cwd, libcrun_error_t *err)
{
  cleanup_free वर्ण *cwd_executable_path = रिक्त;
  cleanup_free वर्ण *tmp = रिक्त;
  पूर्णाङ्क last_error = -ENOENT;
  वर्ण *it, *end;
  पूर्णाङ्क ret;

  *out = रिक्त;

  यदि (executable_path == रिक्त || executable_path[0] == '\0')
    फर्काऊ crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  यदि (executable_path[0] != '/' && म_अक्षर (executable_path, '/'))
    {
      cleanup_free वर्ण *cwd_allocated = रिक्त;

      यदि (cwd == रिक्त)
        {
          cwd_allocated = getcwd (रिक्त, 0);
          यदि (cwd_allocated == रिक्त)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  यदि (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      यदि (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          फर्काऊ 0;
        }
      last_error = ret;
      गच्छ fail;
    }

  end = tmp = xstrdup (दो_पर्या ("PATH"));

  जबसम्म ((it = strsep (&end, ":")))
    {
      cleanup_free वर्ण *path = रिक्त;

      यदि (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, रिक्त);
      यदि (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          जारी;
        }

      ret = check_access (path);
      यदि (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = रिक्त;
          फर्काऊ 0;
        }

      यदि (ret == -ENOENT)
        जारी;

      last_error = ret;
    }

fail:
  विकल्प (last_error)
    {
    पक्ष CHECK_ACCESS_NOT_EXECUTABLE:
      फर्काऊ crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    पक्ष CHECK_ACCESS_NOT_REGULAR:
      फर्काऊ crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    स्वभाव:
      त्रुटि_सं = -last_error;
      यदि (त्रुटि_सं == ENOENT)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

स्थिर ssize_t
safe_read_xattr (वर्ण **ret, पूर्णाङ्क sfd, अचर वर्ण *srcname, अचर वर्ण *name, माप_प्रकार initial_size, libcrun_error_t *err)
{
  cleanup_free वर्ण *buffer = रिक्त;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  जबसम्म (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      यदि (UNLIKELY (s < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "get xattr `%s` from `%s`", name, srcname);

      यदि (s < current_size)
        रोक;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  यदि (s <= 0)
    फर्काऊ s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = रिक्त;

  फर्काऊ s;
}

स्थिर ssize_t
copy_xattr (पूर्णाङ्क sfd, पूर्णाङ्क dfd, अचर वर्ण *srcname, अचर वर्ण *destname, libcrun_error_t *err)
{
  cleanup_free वर्ण *buf = रिक्त;
  ssize_t xattr_len;
  वर्ण *it;

  xattr_len = flistxattr (sfd, रिक्त, 0);
  यदि (UNLIKELY (xattr_len < 0))
    {
      यदि (त्रुटि_सं == ENOTSUP)
        फर्काऊ 0;

      फर्काऊ crun_make_error (err, त्रुटि_सं, "flistxattr `%s`", srcname);
    }

  यदि (xattr_len == 0)
    फर्काऊ 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  यदि (UNLIKELY (xattr_len < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "flistxattr `%s`", srcname);

  लागि (it = buf; it - buf < xattr_len; it += म_माप (it) + 1)
    {
      cleanup_free वर्ण *v = रिक्त;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      यदि (UNLIKELY (s < 0))
        फर्काऊ s;

      s = fsetxattr (dfd, it, v, s, 0);
      यदि (UNLIKELY (s < 0))
        {
          यदि (त्रुटि_सं == EINVAL || त्रुटि_सं == EOPNOTSUPP)
            जारी;

          फर्काऊ crun_make_error (err, त्रुटि_सं, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  फर्काऊ 0;
}

#endif

स्थिर पूर्णाङ्क
copy_rec_stat_file_at (पूर्णाङ्क dfd, अचर वर्ण *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  संरचना stat st;
  पूर्णाङ्क ret;

#ifdef HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        गच्छ fallback;

      फर्काऊ ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  फर्काऊ ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  फर्काऊ ret;
}

पूर्णाङ्क
copy_recursive_fd_to_fd (पूर्णाङ्क srcdirfd, पूर्णाङ्क dfd, अचर वर्ण *srcname, अचर वर्ण *destname, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क destdirfd = dfd;
  cleanup_dir सूची *dsrcfd = रिक्त;
  संरचना dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  यदि (UNLIKELY (dsrcfd == रिक्त))
    {
      TEMP_FAILURE_RETRY (बंद (srcdirfd));
      फर्काऊ crun_make_error (err, त्रुटि_सं, "open directory `%s`", destname);
    }

  लागि (de = सूची_पढ़ो (dsrcfd); de; de = सूची_पढ़ो (dsrcfd))
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

      यदि (म_भेद (de->d_name, ".") == 0 || म_भेद (de->d_name, "..") == 0)
        जारी;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "stat `%s/%s`", srcname, de->d_name);

      विकल्प (mode & S_IFMT)
        {
        पक्ष S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          यदि (UNLIKELY (srcfd < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          यदि (UNLIKELY (destfd < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

#ifdef HAVE_FGETXATTR
          ret = (पूर्णाङ्क) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
#endif

          TEMP_FAILURE_RETRY (बंद (destfd));
          destfd = -1;
          रोक;

        पक्ष S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          यदि (UNLIKELY (srcfd < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          यदि (UNLIKELY (destfd < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (पूर्णाङ्क) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
          रोक;

        पक्ष S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "symlinkat `%s/%s`", destname, de->d_name);
          रोक;

        पक्ष S_IFBLK:
        पक्ष S_IFCHR:
        पक्ष S_IFIFO:
        पक्ष S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "mknodat `%s/%s`", destname, de->d_name);
          रोक;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      यदि (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          यदि (त्रुटि_सं == ENOTSUP)
            जारी;

          फर्काऊ crun_make_error (err, त्रुटि_सं, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  फर्काऊ 0;
}

अचर वर्ण *
find_annotation (libcrun_container_t *container, अचर वर्ण *name)
{
  यदि (container->container_def->annotations == रिक्त)
    फर्काऊ रिक्त;

  फर्काऊ find_string_map_मान (container->annotations, name);
}

पूर्णाङ्क
safe_write (पूर्णाङ्क fd, अचर वर्ण *fname, अचर शून्य *buf, माप_प्रकार गन्ती, libcrun_error_t *err)
{
  माप_प्रकार written = 0;
  जबसम्म (written < गन्ती)
    {
      ssize_t w = लिख (fd, (अचर वर्ण *) buf + written, गन्ती - written);
      यदि (UNLIKELY (w < 0))
        {
          यदि (त्रुटि_सं == EINTR || त्रुटि_सं == EAGAIN)
            जारी;
          फर्काऊ crun_make_error (err, त्रुटि_सं, "write to `%s`", fname);
        }
      written += w;
    }
  फर्काऊ 0;
}

#define MAX_PARTS 32

पूर्णाङ्क
append_paths (वर्ण **out, libcrun_error_t *err, ...)
{
  अचर वर्ण *parts[MAX_PARTS];
  माप_प्रकार sizes[MAX_PARTS];
  माप_प्रकार total_len = 0;
  माप_प्रकार n_parts = 0;
  माप_प्रकार copied = 0;
  बहु_सूची ap;
  माप_प्रकार i;

  बहु_शुरू (ap, err);
  लागि (;;)
    {
      अचर वर्ण *part;
      माप_प्रकार size;

      part = बहु_तर्क (ap, अचर वर्ण *);
      यदि (part == रिक्त)
        रोक;

      यदि (n_parts == MAX_PARTS)
        {
          बहु_पूर्ण (ap);
          फर्काऊ crun_make_error (err, EINVAL, "too many paths specified");
        }

      यदि (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          जबसम्म (part[0] == '/' && part[1] == '/')
            part++;
        }
      अन्यथा
        {
          /* And drop any initial '/' for other components.  */
          जबसम्म (part[0] == '/')
            part++;
        }

      size = म_माप (part);
      यदि (size == 0)
        जारी;

      जबसम्म (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  बहु_पूर्ण (ap);

  total_len = n_parts + 1;
  लागि (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  लागि (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      यदि (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      स_नकल (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  फर्काऊ 0;
}

#यदि __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#अन्यथा
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
स्थिर पूर्णाङ्क
b64_आगत (वर्ण c)
{
  अचर वर्ण table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  पूर्णाङ्क i;

  लागि (i = 0; i < 64; i++)
    {
      यदि (table[i] == c)
        फर्काऊ i;
    }
  फर्काऊ -1;
}

पूर्णाङ्क
base64_decode (अचर वर्ण *iptr, माप_प्रकार isize, वर्ण *optr, माप_प्रकार osize, माप_प्रकार *nbytes)
{
  पूर्णाङ्क i = 0, tmp = 0, pad = 0;
  माप_प्रकार consumed = 0;
  अचिन्हित वर्ण data[4];

  *nbytes = 0;
  जबसम्म (consumed < isize && (*nbytes) + 3 < osize)
    {
      जबसम्म ((i < 4) && (consumed < isize))
        {
          tmp = b64_आगत (*iptr++);
          consumed++;
          यदि (tmp != -1)
            {
              यदि (UNLIKELY (pad > 0))
                फर्काऊ -1;
              data[i++] = tmp;
            }
          अन्यथा यदि (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      यदि (i == 4)
        {
          यदि (UNLIKELY (pad > 2))
            फर्काऊ -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      अन्यथा
        {
          /* I did not get all the data.  */
          consumed -= i;
          फर्काऊ consumed;
        }
      i = 0;
    }
  फर्काऊ consumed;
}

वर्ण *
get_user_name (uid_t uid)
{
  संरचना passwd pd;
  संरचना passwd *temp_result_ptr;
  वर्ण pwdbuffer[200];
  यदि (! getpwuid_r (uid, &pd, pwdbuffer, परिमाण (pwdbuffer), &temp_result_ptr))
    फर्काऊ xstrdup (pd.pw_name);
  फर्काऊ xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
वर्ण *
libcrun_sysctl_साँचो_to_proc_path (अचर वर्ण *साँचो)
{
  अचर वर्ण *sep;
  वर्ण *ret, *it;

  sep = strpbrk (साँचो, "./");

  /* No separator at all, or the key is already in the path form.  */
  यदि (sep == रिक्त || *sep == '/')
    फर्काऊ xstrdup (साँचो);

  ret = xstrdup (साँचो);
  लागि (it = ret; *it; it++)
    {
      यदि (*it == '.')
        *it = '/';
      अन्यथा यदि (*it == '/')
        *it = '.';
    }

  फर्काऊ ret;
}

पूर्णाङ्क
has_suffix (अचर वर्ण *str, अचर वर्ण *suffix)
{
  यदि (! str || ! suffix)
    फर्काऊ 0;
  माप_प्रकार lenstr = म_माप (str);
  माप_प्रकार lensuffix = म_माप (suffix);
  यदि (lensuffix > lenstr)
    फर्काऊ 0;
  फर्काऊ स_भेद (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

वर्ण *
str_join_array (पूर्णाङ्क offset, माप_प्रकार size, वर्ण *अचर array[], अचर वर्ण *joint)
{
  माप_प्रकार jlen;
  cleanup_free माप_प्रकार *lens = रिक्त;
  माप_प्रकार i, total_size = (size - 1) * (jlen = म_माप (joint)) + 1;
  वर्ण *result, *p;

  lens = xmalloc (size * परिमाण (*lens));

  लागि (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = म_माप (array[i]));
    }
  p = result = xmalloc (total_size);
  लागि (i = offset; i < size; ++i)
    {
      स_नकल (p, array[i], lens[i]);
      p += lens[i];
      यदि (i < size - 1)
        {
          स_नकल (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  फर्काऊ result;
}

पूर्णाङ्क
libcrun_mmap (संरचना libcrun_mmap_s **ret, शून्य *addr, माप_प्रकार length,
              पूर्णाङ्क prot, पूर्णाङ्क flags, पूर्णाङ्क fd, off_t offset,
              libcrun_error_t *err)
{
  संरचना libcrun_mmap_s *mmap_s = रिक्त;

  शून्य *mapped = mmap (addr, length, prot, flags, fd, offset);
  यदि (mapped == MAP_FAILED)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "mmap");

  mmap_s = xmalloc (परिमाण (संरचना libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_munmap (संरचना libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = munmap (mmap->addr, mmap->length);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "munmap");

  मुक्त (mmap);

  फर्काऊ 0;
}

स्थिर दीर्घ
read_file_as_long_or_default (अचर वर्ण *path, दीर्घ def_मान)
{
  cleanup_free वर्ण *content = रिक्त;
  libcrun_error_t tmp_err = रिक्त;
  वर्ण *endptr;
  दीर्घ val;
  पूर्णाङ्क ret;

  ret = read_all_file (path, &content, रिक्त, &tmp_err);
  यदि (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      फर्काऊ def_मान;
    }

  त्रुटि_सं = 0;
  val = म_से_दीर्घ (content, &endptr, 10);
  यदि (UNLIKELY (त्रुटि_सं))
    फर्काऊ def_मान;
  यदि (endptr == content || (*endptr && *endptr != '\n'))
    फर्काऊ def_मान;
  फर्काऊ val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (शून्य)
{
  स्थिर atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  यदि (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  फर्काऊ uid;
}

gid_t
get_overflow_gid (शून्य)
{
  स्थिर atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  यदि (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  फर्काऊ gid;
}

शून्य
consume_trailing_slashes (वर्ण *path)
{
  यदि (! path || path[0] == '\0')
    फर्काऊ;

  वर्ण *last = path + म_माप (path);

  जबसम्म (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

वर्ण **
read_dir_entries (अचर वर्ण *path, libcrun_error_t *err)
{
  cleanup_dir सूची *dir = रिक्त;
  माप_प्रकार n_entries = 0;
  माप_प्रकार entries_size = 16;
  वर्ण **entries = रिक्त;
  संरचना dirent *de;

  dir = सूची_खोलो (path);
  यदि (UNLIKELY (dir == रिक्त))
    {
      crun_make_error (err, त्रुटि_सं, "opendir `%s`", path);
      फर्काऊ रिक्त;
    }

  entries = xmalloc (entries_size * परिमाण (वर्ण *));
  जबसम्म ((de = सूची_पढ़ो (dir)))
    {
      यदि (म_भेद (de->d_name, ".") == 0 || म_भेद (de->d_name, "..") == 0)
        जारी;
      यदि (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * परिमाण (वर्ण *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * परिमाण (वर्ण *));
  entries[n_entries] = रिक्त;

  फर्काऊ entries;
}

पूर्णाङ्क
cpuset_string_to_bitmask (अचर वर्ण *str, वर्ण **out, माप_प्रकार *out_size, libcrun_error_t *err)
{
  cleanup_free वर्ण *mask = रिक्त;
  माप_प्रकार mask_size = 0;
  अचर वर्ण *p = str;
  वर्ण *endptr;

  जबसम्म (*p)
    {
      दीर्घ दीर्घ start_range, end_range;

      यदि (*p < '0' || *p > '9')
        गच्छ invalid_आगत;

      start_range = strtoll (p, &endptr, 10);
      यदि (start_range < 0)
        गच्छ invalid_आगत;

      p = endptr;

      यदि (*p != '-')
        end_range = start_range;
      अन्यथा
        {
          p++;

          यदि (*p < '0' || *p > '9')
            गच्छ invalid_आगत;

          end_range = strtoll (p, &endptr, 10);

          यदि (end_range < start_range)
            गच्छ invalid_आगत;

          p = endptr;
        }

      /* Just set some limit.  */
      यदि (end_range > (1 << 20))
        गच्छ invalid_आगत;

      यदि (end_range >= (दीर्घ दीर्घ) (mask_size * अक्षर_बिट))
        {
          माप_प्रकार new_mask_size = (end_range / अक्षर_बिट) + 1;
          mask = xrealloc (mask, new_mask_size);
          स_रखो (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      लागि (दीर्घ दीर्घ i = start_range; i <= end_range; i++)
        mask[i / अक्षर_बिट] |= (1 << (i % अक्षर_बिट));

      यदि (*p == ',')
        p++;
      अन्यथा यदि (*p)
        गच्छ invalid_आगत;
    }

  *out = mask;
  mask = रिक्त;
  *out_size = mask_size;

  फर्काऊ 0;

invalid_आगत:
  फर्काऊ crun_make_error (err, 0, "cannot parse input `%s`", str);
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
channel_fd_pair_new (पूर्णाङ्क in_fd, पूर्णाङ्क out_fd, माप_प्रकार size)
{
  संरचना channel_fd_pair *channel = xmalloc (परिमाण (संरचना channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  फर्काऊ channel;
}

शून्य
channel_fd_pair_free (संरचना channel_fd_pair *channel)
{
  यदि (channel == रिक्त)
    फर्काऊ;

  ring_buffer_free (channel->rb);
  मुक्त (channel);
}

पूर्णाङ्क
channel_fd_pair_process (संरचना channel_fd_pair *channel, पूर्णाङ्क epollfd, libcrun_error_t *err)
{
  bool is_आगत_eagain = false, is_output_eagain = false, repeat;
  पूर्णाङ्क ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  लागि (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      यदि (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_आगत_eagain, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
          यदि (ret > 0)
            repeat = true;
        }
      यदि (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
          यदि (ret > 0)
            repeat = true;
        }
    }

  यदि (epollfd >= 0)
    {
      माप_प्रकार available = ring_buffer_get_space_available (channel->rb);
      माप_प्रकार used = ring_buffer_get_data_available (channel->rb);
      पूर्णाङ्क events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_आगत_eagain ? EPOLLET : 0)) : 0;
      यदि (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      यदि (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
          channel->outfd_epoll_events = events;
        }
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
open_proc (libcrun_error_t *err)
{
  पूर्णाङ्क fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close पूर्णाङ्क fsfd = -1;
  पूर्णाङ्क ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  यदि (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, रिक्त, रिक्त, 0);
      यदि (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          यदि (fd >= 0)
            फर्काऊ fd;
        }
    }
#endif

  fd = खोलो ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  यदि (fd < 0)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `/proc`");
  फर्काऊ fd;
}

पूर्णाङ्क
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  यदि (! container)
    फर्काऊ open_proc (err);

  यदि (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  फर्काऊ container->proc_fd;
}

पूर्णाङ्क
libcrun_open_proc_file (libcrun_container_t *container, अचर वर्ण *path, पूर्णाङ्क flags, libcrun_error_t *err)
{
  पूर्णाङ्क proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  यदि (proc_fd < 0)
    फर्काऊ proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = त्रुटि_सं;
  /* requires closing if not cached */
  यदि (! container)
    TEMP_FAILURE_RETRY (बंद (proc_fd));

  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  फर्काऊ fd;
}

पूर्णाङ्क
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, अचर वर्ण *path, पूर्णाङ्क flags, libcrun_error_t *err)
{
  cleanup_free वर्ण *full_path = रिक्त;
  xasprintf (&full_path, "%d/%s", pid, path);
  फर्काऊ libcrun_open_proc_file (container, full_path, flags, err);
}
