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

#परिभाषय _GNU_SOURCE
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
#यदि_परिभाषितम् HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#यदि_अन्तः
#यदि HAVE_STDATOMIC_H
#  include <stdatomic.h>
#अन्यथा
#  define atomic_long चञ्चल दीर्घ
#  define atomic_int चञ्चल पूर्णाङ्क
#यदि_अन्तः
#अन्तर्भाव "syscalls.h"

#यदि_अपरिभाषितम् CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#यदि_अन्तः
#यदि_अपरिभाषितम् RESOLVE_अन्तर्_मूलम्
#  define RESOLVE_अन्तर्_मूलम् 0x10
#यदि_अन्तः
#यदि_अपरिभाषितम् __NR_close_range
#  define __NR_close_range 436
#यदि_अन्तः
#यदि_अपरिभाषितम् __NR_openat2
#  define __NR_openat2 437
#यदि_अन्तः
#यदि_अपरिभाषितम् PROC_USER_INIT_INO
#  define PROC_USER_INIT_INO ((ino_t) 0xEFFFFFFD)
#यदि_अन्तः

#परिभाषय MAX_READLINKS 32
#परिभाषय MAX_OPENAT2_EAGAIN_RETRIES 128

पूर्णाङ्क
crun_मार्गः_exists (ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  पूर्णाङ्क ret = access (मार्गः, F_OK);
  यदि (ret < 0)
    {
      यदि (errno == ENOENT)
        प्रत्यागम 0;
      प्रत्यागम crun_make_error (err, errno, "access `%s`", मार्गः);
    }
  प्रत्यागम 1;
}

पूर्णाङ्क
xasprintf (वर्ण **str, ध्रुव वर्ण *fmt, ...)
{
  पूर्णाङ्क ret;
  va_list तर्काः_list;

  va_start (तर्काः_list, fmt);

  ret = vasprintf (str, fmt, तर्काः_list);
  यदि (UNLIKELY (ret < 0))
    OOM ();

  va_अन्तः (तर्काः_list);
  प्रत्यागम ret;
}

पूर्णाङ्क
write_file_at_with_flags (पूर्णाङ्क dirfd, पूर्णाङ्क flags, mode_t बहुलकम्, ध्रुव वर्ण *name, ध्रुव शून्य *दत्त, size_t len, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, बहुलकम्);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, दत्त, len, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम (len > अधिकतम_पूर्णाङ्कः) ? अधिकतम_पूर्णाङ्कः : (पूर्णाङ्क) len;
}

पूर्णाङ्क
detach_process ()
{
  pid_t pid;
  यदि (setsid () < 0)
    प्रत्यागम -1;
  pid = fork ();
  यदि (pid < 0)
    प्रत्यागम -1;
  यदि (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  प्रत्यागम 0;
}

पूर्णाङ्क
get_file_type_fd (पूर्णाङ्क fd, mode_t *बहुलकम्)
{
  संरचना stat st;
  पूर्णाङ्क ret;

#यदि_परिभाषितम् HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (fd, "", AT_रिक्तानि_मार्गः | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (errno == ENOSYS || errno == EINVAL)
        गच्छ fallback;

      प्रत्यागम ret;
    }
  *बहुलकम् = stx.stx_mode;
  प्रत्यागम ret;

fallback:
#यदि_अन्तः
  ret = fstat (fd, &st);
  *बहुलकम् = st.st_mode;
  प्रत्यागम ret;
}

पूर्णाङ्क
get_file_type_at (पूर्णाङ्क dirfd, mode_t *बहुलकम्, bool nofollow, ध्रुव वर्ण *मार्गः)
{
  पूर्णाङ्क रिक्तानि_मार्गः = मार्गः == रिक्त ? AT_रिक्तानि_मार्गः : 0;
  संरचना stat st;
  पूर्णाङ्क ret;

#यदि_परिभाषितम् HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (dirfd, मार्गः ?: "", रिक्तानि_मार्गः | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (errno == ENOSYS || errno == EINVAL)
        गच्छ fallback;

      प्रत्यागम ret;
    }
  *बहुलकम् = stx.stx_mode;
  प्रत्यागम ret;

fallback:
#यदि_अन्तः
  ret = fstatat (dirfd, मार्गः ?: "", &st, रिक्तानि_मार्गः | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *बहुलकम् = st.st_mode;
  प्रत्यागम ret;
}

पूर्णाङ्क
get_file_type (mode_t *बहुलकम्, bool nofollow, ध्रुव वर्ण *मार्गः)
{
  प्रत्यागम get_file_type_at (AT_FDCWD, बहुलकम्, nofollow, मार्गः);
}

पूर्णाङ्क
create_file_if_missing_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *file, mode_t बहुलकम्, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, बहुलकम्);
  यदि (fd_write < 0)
    {
      पूर्णाङ्क saved_errno = errno;
      mode_t tmp_बहुलकम्;
      पूर्णाङ्क ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_बहुलकम्, true, file);
      यदि (ret == 0 && S_ISREG (tmp_बहुलकम्))
        प्रत्यागम 0;

      प्रत्यागम crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
ensure_directory_आन्तरिकम्_at (पूर्णाङ्क dirfd, वर्ण *मार्गः, size_t len, पूर्णाङ्क बहुलकम्, libcrun_error_t *err)
{
  वर्ण *it = मार्गः + len;
  पूर्णाङ्क ret = 0;
  bool parent_created = false;

  प्रति (;;)
    {
      ret = mkdirat (dirfd, मार्गः, बहुलकम्);
      यदि (ret == 0 || errno == EEXIST)
        प्रत्यागम 0;

      पूर्णाङ्क saved_errno = errno;
      यदि (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = रिक्त;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (मार्गः, false, &tmp_err);
          यदि (ret > 0)
            प्रत्यागम 0;
          यदि (ret < 0)
            crun_error_release (&tmp_err);

          प्रत्यागम crun_make_error (err, saved_errno, "create directory `%s`", मार्गः);
        }

      यावत् (it > मार्गः && *it != '/')
        {
          it--;
          len--;
        }
      यदि (it == मार्गः)
        प्रत्यागम 0;

      *it = '\0';
      ret = ensure_directory_आन्तरिकम्_at (dirfd, मार्गः, len - 1, बहुलकम्, err);
      *it = '/';
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      parent_created = true;
    }
  प्रत्यागम ret;
}

पूर्णाङ्क
crun_ensure_directory_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *मार्गः, पूर्णाङ्क बहुलकम्, bool nofollow, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *tmp = xstrdup (मार्गः);
  ret = ensure_directory_आन्तरिकम्_at (dirfd, tmp, वाक्यदैर्घ्यम् (tmp), बहुलकम्, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = crun_dir_p_at (dirfd, मार्गः, nofollow, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (ret == 0)
    प्रत्यागम crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", मार्गः);

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
check_fd_is_मार्गः (ध्रुव वर्ण *मार्गः, पूर्णाङ्क fd, ध्रुव वर्ण *fdname, libcrun_error_t *err)
{
  proc_fd_मार्गः_t fdpath;
  size_t मार्गः_len = वाक्यदैर्घ्यम् (मार्गः);
  वर्ण link[PATH_MAX];
  पूर्णाङ्क ret;

  get_proc_self_fd_मार्गः (fdpath, fd);
  ret = अस्थायी_FAILURE_RETRY (readlink (fdpath, link, परिमाण (link)));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "readlink `%s`", fdname);

  यदि (((size_t) ret) != मार्गः_len || memcmp (link, मार्गः, मार्गः_len))
    प्रत्यागम crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, मार्गः);

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
check_fd_under_मार्गः (ध्रुव वर्ण *rootfs, size_t rootfslen, पूर्णाङ्क fd, ध्रुव वर्ण *fdname, libcrun_error_t *err)
{
  proc_fd_मार्गः_t fdpath;
  वर्ण link[PATH_MAX];
  पूर्णाङ्क ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  यदि (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    प्रत्यागम 0;

  get_proc_self_fd_मार्गः (fdpath, fd);
  ret = अस्थायी_FAILURE_RETRY (readlink (fdpath, link, परिमाण (link)));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "readlink `%s`", fdname);

  यदि (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    प्रत्यागम crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  प्रत्यागम 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
स्थिर शून्य
close_and_replace (पूर्णाङ्क *oldfd, पूर्णाङ्क newfd)
{
  यदि (*oldfd >= 0)
    अस्थायी_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
वर्ण *chroot_realpath (ध्रुव वर्ण *chroot, ध्रुव वर्ण *मार्गः, वर्ण resolved_मार्गः[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
स्थिर पूर्णाङ्क
safe_openat_fallback (पूर्णाङ्क dirfd, ध्रुव वर्ण *rootfs, ध्रुव वर्ण *मार्गः, पूर्णाङ्क flags,
                      पूर्णाङ्क बहुलकम्, libcrun_error_t *err)
{
  cleanup_free वर्ण *parent_मार्गः = रिक्त;
  ध्रुव वर्ण *चरमः_component = रिक्त;
  ध्रुव वर्ण *orig_मार्गः = मार्गः;
  ध्रुव वर्ण *मार्गः_अन्तर्_chroot;
  cleanup_close पूर्णाङ्क fd = -1;
  वर्ण resolved[PATH_MAX];
  वर्ण बफर[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  पूर्णाङ्क ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  यदि (flags & O_NOFOLLOW)
    {
      वर्ण *sep;

      parent_मार्गः = xstrdup (मार्गः);
      sep = strrchr (parent_मार्गः, '/');
      यदि (sep == रिक्त)
        {
          /* No parent directory, the entire path is the last component.  */
          चरमः_component = मार्गः;
          parent_मार्गः[0] = '\0';
        }
      अन्यथा यदि (sep[1] != '\0')
        {
          *sep = '\0';
          चरमः_component = मार्गः + (sep - parent_मार्गः) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      यदि (चरमः_component)
        मार्गः = parent_मार्गः;
    }

  मार्गः_अन्तर्_chroot = chroot_realpath (rootfs, मार्गः, बफर);
  यदि (मार्गः_अन्तर्_chroot == रिक्त)
    प्रत्यागम crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_मार्गः);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  यदि (rootfs_len > 0 && वाक्ये_तुलय (मार्गः_अन्तर्_chroot, rootfs, rootfs_len) == 0)
    मार्गः_अन्तर्_chroot += rootfs_len;
  मार्गः_अन्तर्_chroot = consume_slashes (मार्गः_अन्तर्_chroot);

  यदि (चरमः_component)
    {
      ret = snprintf (resolved, परिमाण (resolved), "%s%s%s", मार्गः_अन्तर्_chroot,
                      मार्गः_अन्तर्_chroot[0] == '\0' ? "" : "/", चरमः_component);
      यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (resolved)))
        प्रत्यागम crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_मार्गः);

      मार्गः_अन्तर्_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  यदि (मार्गः_अन्तर्_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      प्रत्यागम ret;
    }

  ret = openat (dirfd, मार्गः_अन्तर्_chroot, flags, बहुलकम्);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", orig_मार्गः);

  fd = ret;

  ret = check_fd_under_मार्गः (rootfs, rootfs_len, fd, orig_मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = fd;
  fd = -1;
  प्रत्यागम ret;
}

पूर्णाङ्क
safe_openat (पूर्णाङ्क dirfd, ध्रुव वर्ण *rootfs, ध्रुव वर्ण *मार्गः, पूर्णाङ्क flags, पूर्णाङ्क बहुलकम्,
             libcrun_error_t *err)
{
  स्थिर bool openat2_supported = true;
  पूर्णाङ्क retries = MAX_OPENAT2_EAGAIN_RETRIES;
  पूर्णाङ्क ret;

  यदि (is_रिक्तानि_string (मार्गः))
    {
      cleanup_close पूर्णाङ्क fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      यदि (is_रिक्तानि_string (rootfs))
        rootfs = "/";

      fd = उद्घाटय (rootfs, flags, बहुलकम्);
      यदि (UNLIKELY (fd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      यदि (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_मार्गः (rootfs, fd, मार्गः, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      ret = fd;
      fd = -1;
      प्रत्यागम ret;
    }

  यदि (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, मार्गः, flags, बहुलकम्, RESOLVE_अन्तर्_मूलम्);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (errno == EINTR)
            गच्छ repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          यदि (errno == EAGAIN && retries-- > 0)
            गच्छ repeat;
          यदि (errno == ENOSYS)
            openat2_supported = false;
          यदि (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            प्रत्यागम safe_openat_fallback (dirfd, rootfs, मार्गः, flags, बहुलकम्, err);

          प्रत्यागम crun_make_error (err, errno, "openat2 `%s`", मार्गः);
        }

      प्रत्यागम ret;
    }

  प्रत्यागम safe_openat_fallback (dirfd, rootfs, मार्गः, flags, बहुलकम्, err);
}

ssize_t
safe_readlinkat (पूर्णाङ्क dfd, ध्रुव वर्ण *name, वर्ण **बफर, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_आकार = hint > 0 ? hint + 1 : 512;
  cleanup_free वर्ण *tmp_buf = रिक्त;
  ssize_t आकार;

  कुरु
    {
      यदि (tmp_buf != रिक्त)
        buf_आकार += 256;

      tmp_buf = xrealloc (tmp_buf, buf_आकार);

      आकार = readlinkat (dfd, name, tmp_buf, buf_आकार);
      यदि (UNLIKELY (आकार < 0))
        प्रत्यागम crun_make_error (err, errno, "readlink `%s`", name);
  } यावत् (आकार == buf_आकार);

  /* Always NUL terminate the buffer.  */
  tmp_buf[आकार] = '\0';

  /* Move ownership to BUFFER.  */
  *बफर = tmp_buf;
  tmp_buf = रिक्त;

  प्रत्यागम आकार;
}

स्थिर पूर्णाङ्क crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath,
                                ध्रुव वर्ण *मार्गः, पूर्णाङ्क बहुलकम्, पूर्णाङ्क max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
स्थिर पूर्णाङ्क
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath, पूर्णाङ्क cwd,
                                 ध्रुव वर्ण *npath, ध्रुव वर्ण *cur, ध्रुव वर्ण *rest,
                                 पूर्णाङ्क बहुलकम्, पूर्णाङ्क max_readlinks, libcrun_error_t *err)
{
  cleanup_free वर्ण *target = रिक्त;
  cleanup_free वर्ण *new_मार्गः = रिक्त;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_मार्गः, "%.*s%s%s%s", target[0] == '/' ? 0 : (पूर्णाङ्क) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  प्रत्यागम crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_मार्गः, बहुलकम्, max_readlinks - 1, err);
}

स्थिर पूर्णाङ्क
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath,
                     ध्रुव वर्ण *मार्गः, पूर्णाङ्क बहुलकम्, पूर्णाङ्क max_readlinks, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क wd_cleanup = -1;
  cleanup_free वर्ण *npath = रिक्त;
  bool चरमः_component = false;
  size_t गभीरता = 0;
  वर्ण *cur;
  वर्ण *it;
  पूर्णाङ्क cwd;
  पूर्णाङ्क ret;

  यदि (max_readlinks <= 0)
    प्रत्यागम crun_make_error (err, ELOOP, "resolve path `%s`", मार्गः);

  मार्गः = consume_slashes (मार्गः);

  /* Empty path, nothing to do.  */
  यदि (*मार्गः == '\0')
    {
      यदि (do_open)
        प्रत्यागम उद्घाटय (dirpath, O_CLOEXEC | O_मार्गः, 0);
      प्रत्यागम 0;
    }

  npath = xstrdup (मार्गः);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  यावत् (cur)
    {
      यदि (it)
        *it = '\0';
      अन्यथा
        चरमः_component = true;

      यदि (cur[0] == '\0')
        भङ्ग;

      यदि (वाक्यं_तुलय (cur, ".") == 0)
        गच्छ अग्र;
      अन्यथा यदि (वाक्यं_तुलय (cur, ".."))
        गभीरता++;
      अन्यथा
        {
          यदि (गभीरता)
            गभीरता--;
          अन्यथा
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              गच्छ अग्र;
            }
        }

      यदि (चरमः_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          यदि (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              यदि (errno == ELOOP)
                प्रत्यागम crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, रिक्त,
                                                        बहुलकम्, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_मार्गः | O_NOFOLLOW, 0);
              यदि (ret < 0)
                प्रत्यागम crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          यदि (do_open)
            प्रत्यागम ret;

          close_and_replace (&wd_cleanup, ret);
          प्रत्यागम 0;
        }

      ret = mkdirat (cwd, cur, बहुलकम्);
      यदि (ret < 0)
        {
          mode_t st_mode;

          यदि (errno != EEXIST)
            प्रत्यागम crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          यदि (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            प्रत्यागम crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : रिक्त, बहुलकम्, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_मार्गः | O_CLOEXEC, 0, err);
      यदि (UNLIKELY (cwd < 0))
        प्रत्यागम crun_error_wrap (err, "creating `/%s`", मार्गः);

      यदि (! चरमः_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, रिक्त);
          यदि (UNLIKELY (ret < 0))
            {
              पूर्णाङ्क saved_errno = errno;

              close (cwd);
              प्रत्यागम crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          यदि ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              प्रत्यागम crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", मार्गः, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    अग्र:
      यदि (it == रिक्त)
        भङ्ग;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  यदि (do_open)
    {
      यदि (cwd == dirfd)
        {
          ret = dup (dirfd);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "dup `%s`", dirpath);
          प्रत्यागम ret;
        }

      wd_cleanup = -1;
      प्रत्यागम cwd;
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
crun_safe_create_and_open_ref_at (bool dir, पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath, ध्रुव वर्ण *मार्गः, पूर्णाङ्क बहुलकम्, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, मार्गः, O_मार्गः | O_CLOEXEC, 0, err);
  यदि (LIKELY (ret >= 0))
    प्रत्यागम ret;

  crun_error_release (err);
  प्रत्यागम crun_safe_ensure_at (true, dir, true, dirfd, dirpath, मार्गः, बहुलकम्, MAX_READLINKS, err);
}

पूर्णाङ्क
crun_safe_ensure_directory_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath, ध्रुव वर्ण *मार्गः, पूर्णाङ्क बहुलकम्,
                               libcrun_error_t *err)
{
  प्रत्यागम crun_safe_ensure_at (false, true, false, dirfd, dirpath, मार्गः, बहुलकम्, MAX_READLINKS, err);
}

पूर्णाङ्क
crun_safe_ensure_file_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath, ध्रुव वर्ण *मार्गः, पूर्णाङ्क बहुलकम्,
                          libcrun_error_t *err)
{
  प्रत्यागम crun_safe_ensure_at (false, false, false, dirfd, dirpath, मार्गः, बहुलकम्, MAX_READLINKS, err);
}

पूर्णाङ्क
crun_ensure_directory (ध्रुव वर्ण *मार्गः, पूर्णाङ्क बहुलकम्, bool nofollow, libcrun_error_t *err)
{
  प्रत्यागम crun_ensure_directory_at (AT_FDCWD, मार्गः, बहुलकम्, nofollow, err);
}

स्थिर पूर्णाङ्क
get_file_आकार (पूर्णाङ्क fd, off_t *आकार)
{
  संरचना stat st;
  पूर्णाङ्क ret;
#यदि_परिभाषितम् HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (fd, "", AT_रिक्तानि_मार्गः | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_आकार, &stx);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (errno == ENOSYS || errno == EINVAL)
        गच्छ fallback;
      प्रत्यागम ret;
    }
  *आकार = stx.stx_size;

  प्रत्यागम ret;

fallback:
#यदि_अन्तः
  ret = fstat (fd, &st);
  *आकार = st.st_size;
  प्रत्यागम ret;
}

पूर्णाङ्क
crun_dir_p_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *मार्गः, bool nofollow, libcrun_error_t *err)
{
  mode_t बहुलकम्;
  पूर्णाङ्क ret;

  ret = get_file_type_at (dirfd, &बहुलकम्, nofollow, मार्गः);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "stat `%s`", मार्गः);

  प्रत्यागम S_ISDIR (बहुलकम्) ? 1 : 0;
}

पूर्णाङ्क
crun_dir_p (ध्रुव वर्ण *मार्गः, bool nofollow, libcrun_error_t *err)
{
  प्रत्यागम crun_dir_p_at (AT_FDCWD, मार्गः, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
स्थिर पूर्णाङ्क
check_running_अन्तर्_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free वर्ण *बफर = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  ret = read_all_file ("/proc/self/uid_map", &बफर, &len, err);
  यदि (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      यदि (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          प्रत्यागम 0;
        }
      प्रत्यागम ret;
    }

  प्रत्यागम वाक्ये_अन्विष (बफर, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
स्थिर bool
running_अन्तर्_openvz ()
{
  यदि (access ("/proc/vz", F_OK) < 0)
    प्रत्यागम false;

  प्रत्यागम access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

पूर्णाङ्क
check_running_अन्तर्_user_namespace (libcrun_error_t *err)
{
  स्थिर atomic_int सततम्_अन्तर्_userns = -1;
  संरचना stat st;
  पूर्णाङ्क ret;

  ret = सततम्_अन्तर्_userns;
  यदि (ret >= 0)
    प्रत्यागम ret;

  ret = stat ("/proc/self/ns/user", &st);
  यदि (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      यदि (st.st_ino == PROC_USER_INIT_INO)
        {
          सततम्_अन्तर्_userns = 0;
          प्रत्यागम सततम्_अन्तर्_userns;
        }

      यदि (! running_अन्तर्_openvz ())
        {
          सततम्_अन्तर्_userns = 1;
          प्रत्यागम सततम्_अन्तर्_userns;
        }
    }
  अन्यथा यदि (UNLIKELY (errno != ENOENT))
    प्रत्यागम crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_अन्तर्_user_namespace_uid_map (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  सततम्_अन्तर्_userns = ret;
  प्रत्यागम सततम्_अन्तर्_userns;
}

स्थिर size_t
get_page_आकार ()
{
  स्थिर atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  यदि (pagesize == 0)
    {
      pagesize = (दीर्घ) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  प्रत्यागम (size_t) pagesize;
}

स्थिर पूर्णाङ्क selinux_enabled = -1;
स्थिर पूर्णाङ्क apparmor_enabled = -1;

पूर्णाङ्क
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free वर्ण *बहिर्गमनम् = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  size_t len;
  पूर्णाङ्क ret;

  यदि (selinux_enabled >= 0)
    प्रत्यागम selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम fd;

  ret = read_all_fd_with_आकार_hint (fd, "mounts", &बहिर्गमनम्, &len, get_page_आकार (), err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  selinux_enabled = वाक्ये_अन्विष (बहिर्गमनम्, "selinux") ? 1 : 0;

  प्रत्यागम selinux_enabled;
}

पूर्णाङ्क
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क आकार;
  वर्ण buf[1];

  यदि (apparmor_enabled >= 0)
    प्रत्यागम apparmor_enabled;

  fd = उद्घाटय ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  यदि (fd == -1)
    {
      यदि (errno == ENOENT)
        {
          apparmor_enabled = 0;
          प्रत्यागम 0;
        }

      प्रत्यागम crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  आकार = अस्थायी_FAILURE_RETRY (पठ (fd, buf, परिमाण (buf)));

  apparmor_enabled = आकार > 0 && buf[0] == 'Y' ? 1 : 0;

  प्रत्यागम apparmor_enabled;
}

स्थिर पूर्णाङ्क
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  यदि (selinux_enabled < 0)
    प्रत्यागम crun_make_error (err, 0, "SELinux is not initialized correctly");
  प्रत्यागम selinux_enabled;
}

पूर्णाङ्क
सङ्कलय_selinux_mount_label (वर्ण **retlabel, ध्रुव वर्ण *दत्त, ध्रुव वर्ण *label, ध्रुव वर्ण *context_type, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_is_selinux_enabled (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (label && ret)
    {
      यदि (दत्त && *दत्त)
        xasprintf (retlabel, "%s,%s=\"%s\"", दत्त, context_type, label);
      अन्यथा
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      प्रत्यागम 0;
    }
  *retlabel = xstrdup (दत्त);
  प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *
lsm_attr_मार्गः (libcrun_container_t *container, ध्रुव वर्ण *lsm, ध्रुव वर्ण *fname, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क attr_dirfd = -1;
  cleanup_close पूर्णाङ्क lsm_dirfd = -1;
  वर्ण *attr_मार्गः = रिक्त;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_मार्गः, err);
  यदि (UNLIKELY (attr_dirfd < 0))
    प्रत्यागम रिक्त;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  यदि (lsm != रिक्त)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_मार्गः | O_CLOEXEC);

      यदि (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          प्रत्यागम रिक्त;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_मार्गः, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  प्रत्यागम attr_मार्गः;
}

स्थिर पूर्णाङ्क
check_proc_super_magic (पूर्णाङ्क fd, ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  संरचना statfs sfs;

  पूर्णाङ्क ret = fstatfs (fd, &sfs);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "statfs `%s`", मार्गः);

  यदि (sfs.f_type != PROC_SUPER_MAGIC)
    प्रत्यागम crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", मार्गः);

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
समुच्चयः_security_attr (libcrun_container_t *container, ध्रुव वर्ण *lsm, ध्रुव वर्ण *fname, ध्रुव वर्ण *दत्त, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  cleanup_free ध्रुव वर्ण *attr_मार्गः = lsm_attr_मार्गः (container, lsm, fname, err);
  cleanup_close पूर्णाङ्क fd = -1;

  यदि (UNLIKELY (attr_मार्गः == रिक्त))
    प्रत्यागम -1;

  fd = libcrun_open_proc_file (container, attr_मार्गः, O_WRONLY | O_CLOEXEC, err);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम fd;

  ret = check_proc_super_magic (fd, attr_मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  // Write out data
  ret = अस्थायी_FAILURE_RETRY (लिख (fd, दत्त, वाक्यदैर्घ्यम् (दत्त)));
  यदि (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      यदि (errno == ENOENT && lsm != रिक्त && वाक्यं_तुलय (lsm, "apparmor") == 0)
        {
          ध्रुव वर्ण *profile = strchr (दत्त, ' ');

          प्रत्यागम crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : दत्त);
        }
      प्रत्यागम crun_make_error (err, errno, "write to file `%s`", attr_मार्गः);
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
समुच्चयः_selinux_label (libcrun_container_t *container, ध्रुव वर्ण *label, bool now, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_is_selinux_enabled (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (ret)
    प्रत्यागम समुच्चयः_security_attr (container, रिक्त, now ? "current" : "exec", label, err);
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  यदि (apparmor_enabled < 0)
    प्रत्यागम crun_make_error (err, 0, "AppArmor is not initialized correctly");
  प्रत्यागम apparmor_enabled;
}

स्थिर पूर्णाङ्क
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#परिभाषय UNCONFINED "unconfined"
#परिभाषय UNCONFINED_LEN (ssize_t) (परिमाण (UNCONFINED) - 1)
  cleanup_free ध्रुव वर्ण *attr_मार्गः = lsm_attr_मार्गः (container, "apparmor", "current", err);
  cleanup_close पूर्णाङ्क fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  वर्ण buf[UNCONFINED_LEN + 1];

  यदि (UNLIKELY (attr_मार्गः == रिक्त))
    प्रत्यागम -1;

  fd = libcrun_open_proc_file (container, attr_मार्गः, O_RDONLY | O_CLOEXEC, err);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम fd;

  यदि (UNLIKELY (check_proc_super_magic (fd, attr_मार्गः, err)))
    प्रत्यागम -1;

  ssize_t अष्टकानि_read = अस्थायी_FAILURE_RETRY (पठ (fd, buf, परिमाण (buf)));
  यदि (UNLIKELY (अष्टकानि_read < 0))
    प्रत्यागम crun_make_error (err, errno, "read from `%s`", attr_मार्गः);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  यदि (अष्टकानि_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    प्रत्यागम 1;
  यदि (अष्टकानि_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    प्रत्यागम 0;
  प्रत्यागम 1;
#अपरिभाषय UNCONFINED
#अपरिभाषय UNCONFINED_LEN
}

पूर्णाङ्क
समुच्चयः_apparmor_profile (libcrun_container_t *container, ध्रुव वर्ण *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_is_apparmor_enabled (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (ret)
    {
      cleanup_free वर्ण *buf = रिक्त;
      ret = is_current_process_confined (container, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      प्रत्यागम समुच्चयः_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
read_all_fd_आन्तरिकम् (पूर्णाङ्क fd, ध्रुव वर्ण *description, वर्ण **बहिर्गमनम्, size_t *len, size_t आकार_hint, size_t max_आकार, libcrun_error_t *err)
{
  cleanup_free वर्ण *buf = रिक्त;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t आकार = 0;
  पूर्णाङ्क ret;

  यदि (आकार_hint)
    allocated = आकार_hint;
  अन्यथा
    {
      ret = get_file_आकार (fd, &आकार);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "stat `%s`", description);

      यदि (max_आकार && (size_t) आकार > max_आकार)
        प्रत्यागम crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_आकार);

      allocated = आकार == 0 ? 1023 : आकार;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  यावत् ((आकार && nread < (size_t) आकार) || आकार == 0)
    {
      ret = अस्थायी_FAILURE_RETRY (पठ (fd, buf + nread, allocated - nread));
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "read from file `%s`", description);

      यदि (ret == 0)
        भङ्ग;

      nread += ret;

      यदि (max_आकार && nread > max_आकार)
        प्रत्यागम crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_आकार);

      यदि (nread == allocated)
        {
          यदि (आकार)
            भङ्ग;

          यदि (pagesize == 0)
            pagesize = get_page_आकार ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  यदि (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      वर्ण *tmp = realloc (buf, nread + 1);
      यदि (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *बहिर्गमनम् = buf;
  buf = रिक्त;
  यदि (len)
    *len = nread;
  प्रत्यागम 0;
}

पूर्णाङ्क
read_all_fd_with_आकार_hint (पूर्णाङ्क fd, ध्रुव वर्ण *description, वर्ण **बहिर्गमनम्, size_t *len, size_t आकार_hint, libcrun_error_t *err)
{
  प्रत्यागम read_all_fd_आन्तरिकम् (fd, description, बहिर्गमनम्, len, आकार_hint, 0, err);
}

पूर्णाङ्क
read_all_fd_with_max_आकार (पूर्णाङ्क fd, ध्रुव वर्ण *description, वर्ण **बहिर्गमनम्, size_t *len, size_t max_आकार, libcrun_error_t *err)
{
  प्रत्यागम read_all_fd_आन्तरिकम् (fd, description, बहिर्गमनम्, len, 0, max_आकार, err);
}

पूर्णाङ्क
read_all_file_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *मार्गः, वर्ण **बहिर्गमनम्, size_t *len, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;

  fd = अस्थायी_FAILURE_RETRY (openat (dirfd, मार्गः, O_RDONLY | O_CLOEXEC));
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", मार्गः);

  प्रत्यागम read_all_fd (fd, मार्गः, बहिर्गमनम्, len, err);
}

पूर्णाङ्क
read_all_file (ध्रुव वर्ण *मार्गः, वर्ण **बहिर्गमनम्, size_t *len, libcrun_error_t *err)
{
  यदि (वाक्यं_तुलय (मार्गः, "-") == 0)
    मार्गः = "/dev/stdin";

  प्रत्यागम read_all_file_at (AT_FDCWD, मार्गः, बहिर्गमनम्, len, err);
}

पूर्णाङ्क
get_realpath_गन्तव्यम्_file (पूर्णाङ्क dirfd, ध्रुव वर्ण *मार्गः_name, वर्ण **absolute_मार्गः, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क targetfd = -1;

  targetfd = अस्थायी_FAILURE_RETRY (openat (dirfd, मार्गः_name, O_RDONLY | O_CLOEXEC));
  यदि (UNLIKELY (targetfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", मार्गः_name);
  अन्यथा
    {
      ssize_t len;
      proc_fd_मार्गः_t target_fd_मार्गः;

      get_proc_self_fd_मार्गः (target_fd_मार्गः, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_मार्गः, absolute_मार्गः, 0, err);
      यदि (UNLIKELY (len < 0))
        प्रत्यागम crun_error_wrap (err, "error unable to provide absolute path to file `%s`", मार्गः_name);
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
open_unix_domain_client_socket (ध्रुव वर्ण *मार्गः, पूर्णाङ्क dgram, libcrun_error_t *err)
{
  संरचना sockaddr_un addr = {};
  पूर्णाङ्क ret;
  proc_fd_मार्गः_t name_buf;
  cleanup_close पूर्णाङ्क destfd = -1;
  cleanup_close पूर्णाङ्क fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", मार्गः);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_प्रवाहः, 0);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "create UNIX socket");

  यदि (वाक्यदैर्घ्यम् (मार्गः) >= परिमाण (addr.sun_मार्गः))
    {
      destfd = उद्घाटय (मार्गः, O_मार्गः | O_CLOEXEC);
      यदि (UNLIKELY (destfd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s`", मार्गः);

      get_proc_self_fd_मार्गः (name_buf, destfd);

      मार्गः = name_buf;
    }

  size_t मार्गः_len = वाक्यदैर्घ्यम् (मार्गः);
  यदि (मार्गः_len >= परिमाण (addr.sun_मार्गः))
    प्रत्यागम crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", मार्गः);

  स्मृतिं_प्रतिलिख (addr.sun_मार्गः, मार्गः, मार्गः_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (संरचना sockaddr *) &addr, परिमाण (addr));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "connect socket to `%s`", मार्गः);

  ret = fd;
  fd = -1;

  प्रत्यागम ret;
}

पूर्णाङ्क
open_unix_domain_socket (ध्रुव वर्ण *मार्गः, पूर्णाङ्क dgram, libcrun_error_t *err)
{
  संरचना sockaddr_un addr = {};
  proc_fd_मार्गः_t name_buf;
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क dirfd = -1;
  cleanup_close पूर्णाङ्क fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_प्रवाहः, 0);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "create UNIX socket");

  यदि (वाक्यदैर्घ्यम् (मार्गः) >= परिमाण (addr.sun_मार्गः))
    {
      cleanup_free वर्ण *dpath = xstrdup (मार्गः);
      cleanup_free वर्ण *bpath = xstrdup (मार्गः);
      ध्रुव वर्ण *parent_dir = dirname (dpath);
      ध्रुव वर्ण *आधारः = basename (bpath);
      पूर्णाङ्क n;

      dirfd = उद्घाटय (parent_dir, O_मार्गः | O_DIRECTORY | O_CLOEXEC);
      यदि (UNLIKELY (dirfd < 0))
        प्रत्यागम crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_मार्गः (name_buf, dirfd);

      n = snprintf (addr.sun_मार्गः, परिमाण (addr.sun_मार्गः), "%s/%s", name_buf, आधारः);
      यदि (n < 0 || (size_t) n >= परिमाण (addr.sun_मार्गः))
        प्रत्यागम crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", मार्गः);
    }
  अन्यथा
    {
      size_t मार्गः_len = वाक्यदैर्घ्यम् (मार्गः);
      स्मृतिं_प्रतिलिख (addr.sun_मार्गः, मार्गः, मार्गः_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (संरचना sockaddr *) &addr, परिमाण (addr));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "bind socket to `%s`", मार्गः);

  यदि (! dgram)
    {
      ret = listen (fd, 1);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  प्रत्यागम ret;
}

पूर्णाङ्क
send_fd_गन्तव्यम्_socket (पूर्णाङ्क server, पूर्णाङ्क fd, libcrun_error_t *err)
{
  प्रत्यागम send_fd_गन्तव्यम्_socket_with_payload (server, fd, रिक्त, 0, err);
}

पूर्णाङ्क
send_fd_गन्तव्यम्_socket_with_payload (पूर्णाङ्क server, पूर्णाङ्क fd, ध्रुव वर्ण *payload, size_t payload_len, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  संरचना cmsghdr *cmsg = रिक्त;
  संरचना iovec iov[2];
  संरचना msghdr सन्देशः = {};
  वर्ण ctrl_buf[CMSG_SPACE (1 + परिमाण (पूर्णाङ्क))] = {};
  वर्ण दत्त[1];

  दत्त[0] = ' ';
  iov[0].iov_base = दत्त;
  iov[0].iov_len = परिमाण (दत्त);

  यदि (payload_len > 0)
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

  *((पूर्णाङ्क *) CMSG_दत्त (cmsg)) = fd;

  ret = अस्थायी_FAILURE_RETRY (sendmsg (server, &सन्देशः, 0));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "sendmsg");
  प्रत्यागम 0;
}

पूर्णाङ्क
receive_fd_from_socket_with_payload (पूर्णाङ्क from, वर्ण *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;
  संरचना iovec iov[1];
  संरचना msghdr सन्देशः = {};
  वर्ण ctrl_buf[CMSG_SPACE (परिमाण (पूर्णाङ्क))] = {};
  वर्ण दत्त[1];
  संरचना cmsghdr *cmsg;

  दत्त[0] = ' ';
  iov[0].iov_base = दत्त;
  iov[0].iov_len = परिमाण (दत्त);

  यदि (payload_len > 0)
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

  ret = अस्थायी_FAILURE_RETRY (recvmsg (from, &सन्देशः, 0));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "recvmsg");
  यदि (UNLIKELY (ret == 0))
    प्रत्यागम crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&सन्देशः);
  यदि (cmsg == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "no msg received");
  स्मृतिं_प्रतिलिख (&fd, CMSG_दत्त (cmsg), परिमाण (fd));

  ret = fd;
  fd = -1;
  प्रत्यागम ret;
}

पूर्णाङ्क
receive_fd_from_socket (पूर्णाङ्क from, libcrun_error_t *err)
{
  प्रत्यागम receive_fd_from_socket_with_payload (from, रिक्त, 0, err);
}

पूर्णाङ्क
create_socket_pair (पूर्णाङ्क *pair, libcrun_error_t *err)
{
  पूर्णाङ्क ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "socketpair");
  प्रत्यागम 0;
}

पूर्णाङ्क
create_signalfd (sigset_t *आवरणम्, libcrun_error_t *err)
{
  पूर्णाङ्क ret = signalfd (-1, आवरणम्, 0);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "signalfd");
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
epoll_helper_toggle (पूर्णाङ्क epollfd, पूर्णाङ्क fd, पूर्णाङ्क events, libcrun_error_t *err)
{
  संरचना epoll_event ev = {};
  bool सङ्कलय = events != 0;
  पूर्णाङ्क ret;

  ev.events = events;
  ev.दत्त.fd = fd;
  ret = epoll_ctl (epollfd, सङ्कलय ? EPOLL_CTL_सङ्कलय : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  यदि (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  यदि (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      यदि (errno == ENOENT || errno == EPERM)
        प्रत्यागम 0;
      प्रत्यागम crun_make_error (err, errno, "epoll_ctl `%s` `%d`", सङ्कलय ? "add" : "del", fd);
    }
  प्रत्यागम 0;
}

पूर्णाङ्क
epoll_helper (पूर्णाङ्क *अन्तर्_fds, पूर्णाङ्क *अन्तर्_levelfds, पूर्णाङ्क *बहिर्गमनम्_fds, पूर्णाङ्क *बहिर्गमनम्_levelfds, libcrun_error_t *err)
{
  संरचना epoll_event ev;
  cleanup_close पूर्णाङ्क epollfd = -1;
  पूर्णाङ्क ret;

  पूर्णाङ्क *it;
  epollfd = epoll_create1 (0);
  यदि (UNLIKELY (epollfd < 0))
    प्रत्यागम crun_make_error (err, errno, "epoll_create1");

#परिभाषय सङ्कलय_FDS(FDS, EVENTS)                                            \
  प्रति (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.दत्त.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_सङ्कलय, *it, &ev);               \
      यदि (UNLIKELY (ret < 0))                                           \
        प्रत्यागम crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  यदि (अन्तर्_fds)
    सङ्कलय_FDS (अन्तर्_fds, EPOLLIN);
  यदि (अन्तर्_levelfds)
    सङ्कलय_FDS (अन्तर्_levelfds, EPOLLIN | EPOLLET);
  यदि (बहिर्गमनम्_fds)
    सङ्कलय_FDS (बहिर्गमनम्_fds, EPOLLOUT);
  यदि (बहिर्गमनम्_levelfds)
    सङ्कलय_FDS (बहिर्गमनम्_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  प्रत्यागम ret;
}

पूर्णाङ्क
copy_from_fd_गन्तव्यम्_fd (पूर्णाङ्क src, पूर्णाङ्क dst, पूर्णाङ्क consume, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  ssize_t nread;
  size_t pagesize = get_page_आकार ();
#यदि_परिभाषितम् HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#यदि_अन्तः
  कुरु
    {
      cleanup_free वर्ण *बफर = रिक्त;
      ssize_t remaining;

#यदि_परिभाषितम् HAVE_COPY_FILE_RANGE
      यदि (can_copy_file_range)
        {
          nread = copy_file_range (src, रिक्त, dst, रिक्त, pagesize, 0);
          यदि (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              गच्छ fallback;
            }
          यदि (consume && nread < 0 && errno == EAGAIN)
            प्रत्यागम 0;
          यदि (nread < 0 && errno == EIO)
            प्रत्यागम 0;
          यदि (UNLIKELY (nread < 0))
            प्रत्यागम crun_make_error (err, errno, "copy_file_range");
          अनुवर्त;
        }
    fallback:
#यदि_अन्तः

      बफर = xmalloc (pagesize);
      nread = अस्थायी_FAILURE_RETRY (पठ (src, बफर, pagesize));
      यदि (consume && nread < 0 && errno == EAGAIN)
        प्रत्यागम 0;
      यदि (nread < 0 && errno == EIO)
        प्रत्यागम 0;
      यदि (UNLIKELY (nread < 0))
        प्रत्यागम crun_make_error (err, errno, "read");

      remaining = nread;
      यावत् (remaining)
        {
          ret = अस्थायी_FAILURE_RETRY (लिख (dst, बफर + nread - remaining, remaining));
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } यावत् (consume && nread);

  प्रत्यागम 0;
}

पूर्णाङ्क
सततम्_process (वर्ण **तर्काः, libcrun_error_t *err)
{
  pid_t pid = fork ();
  यदि (UNLIKELY (pid < 0))
    प्रत्यागम crun_make_error (err, errno, "fork");
  यदि (pid)
    {
      पूर्णाङ्क r, स्थितिः;
      r = अस्थायी_FAILURE_RETRY (waitpid (pid, &स्थितिः, 0));
      यदि (r < 0)
        प्रत्यागम crun_make_error (err, errno, "waitpid");
      यदि (WIFEXITED (स्थितिः) || WIFSIGNALED (स्थितिः))
        प्रत्यागम WEXITSTATUS (स्थितिः);
    }

  execvp (तर्काः[0], तर्काः);
  _safe_exit (EXIT_FAILURE);
}

#यदि_अपरिभाषितम् HAVE_FGETPWENT_R
स्थिर अचिह्नित
atou (वर्ण **s)
{
  अचिह्नित x;
  प्रति (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  प्रत्यागम x;
}

पूर्णाङ्क
fgetpwent_r (FILE *f, संरचना passwd *pw, वर्ण *पङ्क्तिः, size_t आकार, संरचना passwd **res)
{
  वर्ण *s;
  पूर्णाङ्क rv = 0;
  प्रति (;;)
    {
      पङ्क्तिः[आकार - 1] = '\xff';
      यदि ((fgets (पङ्क्तिः, आकार, f) == रिक्त) || ferror (f) || पङ्क्तिः[आकार - 1] != '\xff')
        {
          rv = (पङ्क्तिः[आकार - 1] != '\xff') ? ERANGE : ENOENT;
          पङ्क्तिः = 0;
          pw = 0;
          भङ्ग;
        }
      पङ्क्तिः[strcspn (पङ्क्तिः, "\n")] = 0;

      s = पङ्क्तिः;
      pw->pw_name = s++;
      यदि (! (s = strchr (s, ':')))
        अनुवर्त;

      *s++ = 0;
      pw->pw_passwd = s;
      यदि (! (s = strchr (s, ':')))
        अनुवर्त;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      यदि (*s != ':')
        अनुवर्त;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      यदि (*s != ':')
        अनुवर्त;

      *s++ = 0;
      pw->pw_gecos = s;
      यदि (! (s = strchr (s, ':')))
        अनुवर्त;

      *s++ = 0;
      pw->pw_dir = s;
      यदि (! (s = strchr (s, ':')))
        अनुवर्त;

      *s++ = 0;
      pw->pw_shell = s;
      भङ्ग;
    }
  *res = pw;
  यदि (rv)
    errno = rv;
  प्रत्यागम rv;
}
#यदि_अन्तः

पूर्णाङ्क
समुच्चयः_home_env (uid_t id)
{
  संरचना passwd pwd;
  cleanup_free वर्ण *buf = रिक्त;
  दीर्घ buf_आकार;
  cleanup_file FILE *प्रवाहः = रिक्त;

  buf_आकार = sysconf (_SC_GETPW_R_SIZE_MAX);
  यदि (buf_आकार < 0)
    buf_आकार = 1024;

  buf = xmalloc (buf_आकार);

  प्रवाहः = fopen ("/etc/passwd", "re");
  यदि (प्रवाहः == रिक्त)
    प्रत्यागम -1;

  प्रति (;;)
    {
      संरचना passwd *ret_pw = रिक्त;

      पूर्णाङ्क ret = fgetpwent_r (प्रवाहः, &pwd, buf, buf_आकार, &ret_pw);
      यदि (UNLIKELY (ret != 0))
        {
          यदि (ret == EINTR)
            अनुवर्त;

          यदि (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              प्रत्यागम -1;
            }

          buf_आकार *= 2;
          buf = xrealloc (buf, buf_आकार);
          अनुवर्त;
        }

      यदि (ret_pw && ret_pw->pw_uid == id)
        {
          यदि (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          अन्यथा
            प्रत्यागम 0;
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
  दीर्घ buf_आकार;
  cleanup_free वर्ण *buf = रिक्त;
  ध्रुव वर्ण *name;
  संरचना passwd pwd;

  buf_आकार = sysconf (_SC_GETPW_R_SIZE_MAX);
  यदि (buf_आकार < 0)
    buf_आकार = 1024;

  buf = xmalloc (buf_आकार);
  प्रति (;;)
    {
      पूर्णाङ्क ret;
      संरचना passwd *ret_pw = रिक्त;

      ret = getpwuid_r (id, &pwd, buf, buf_आकार, &ret_pw);
      यदि (LIKELY (ret == 0))
        {
          यदि (ret_pw)
            {
              name = ret_pw->pw_name;
              भङ्ग;
            }
          प्रत्यागम -1;
        }

      यदि (ret != ERANGE)
        प्रत्यागम -1;

      buf_आकार *= 2;
      buf = xrealloc (buf, buf_आकार);
    }

  len_name = वाक्यदैर्घ्यम् (name);

  निवेशः = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  यदि (निवेशः == रिक्त)
    प्रत्यागम -1;

  प्रति (;;)
    {
      वर्ण *endptr;
      ssize_t पठ = getline (&lineptr, &lenlineptr, निवेशः);
      यदि (पठ < 0)
        प्रत्यागम -1;

      यदि (पठ < (ssize_t) (len_name + 2))
        अनुवर्त;

      यदि (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        अनुवर्त;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      यदि (endptr >= &lineptr[पठ])
        प्रत्यागम -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      प्रत्यागम 0;
    }
}

#परिभाषय MIN(x, y) ((x) < (y) ? (x) : (y))

पूर्णाङ्क
format_default_id_mapping (वर्ण **बहिर्गमनम्, uid_t container_id, uid_t host_uid, uid_t host_id, पूर्णाङ्क is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, उपलब्धम् = 0;
  cleanup_free वर्ण *बफर = रिक्त;
  पूर्णाङ्क written = 0;
  पूर्णाङ्क ret, remaining;

  *बहिर्गमनम् = रिक्त;

  यदि (getsubidrange (host_uid, is_uid, &from, &उपलब्धम्) < 0)
    प्रत्यागम 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  बफर = xmalloc (remaining + 1);

  यदि (container_id > 0)
    {
      uint32_t प्रयुक्तम् = MIN (container_id, उपलब्धम्);
      ret = snprintf (बफर + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, प्रयुक्तम्);
      यदि (UNLIKELY (ret >= remaining))
        प्रत्यागम crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += प्रयुक्तम्;
      उपलब्धम् -= प्रयुक्तम्;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (बफर + written, remaining, "%d %d 1\n", container_id, host_id);
  यदि (UNLIKELY (ret >= remaining))
    प्रत्यागम crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  यदि (उपलब्धम्)
    {
      ret = snprintf (बफर + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, उपलब्धम्);
      यदि (UNLIKELY (ret >= remaining))
        प्रत्यागम crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *बहिर्गमनम् = बफर;
  बफर = रिक्त;
  प्रत्यागम written;
}

स्थिर पूर्णाङ्क
unset_cloexec_flag (पूर्णाङ्क fd)
{
  पूर्णाङ्क flags = fcntl (fd, F_GETFD);
  यदि (flags == -1)
    प्रत्यागम -1;

  flags &= ~FD_CLOEXEC;

  प्रत्यागम fcntl (fd, F_SETFD, flags);
}

स्थिर शून्य __attribute__ ((__noreturn__))
सततम्_process_child (वर्ण *मार्गः, वर्ण **तर्काः, ध्रुव वर्ण *cwd, वर्ण **envp, पूर्णाङ्क pipe_r,
                   पूर्णाङ्क pipe_w, पूर्णाङ्क बहिर्गमनम्_fd, पूर्णाङ्क err_fd, bool can_ignore_chdir_errors)
{
  वर्ण *tmp_तर्काः[] = { मार्गः, रिक्त };
  libcrun_error_t err = रिक्त;
  पूर्णाङ्क dev_null_fd = -1;
  पूर्णाङ्क ret;

  ret = mark_or_close_fds_ge_than (रिक्त, 3, false, &err);
  यदि (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->स्थितिः, "%s", (err)->सन्देशः);

  यदि (बहिर्गमनम्_fd < 0 || err_fd < 0)
    {
      dev_null_fd = उद्घाटय ("/dev/null", O_WRONLY | O_CLOEXEC);
      यदि (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  अस्थायी_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  अस्थायी_FAILURE_RETRY (close (pipe_r));

  dup2 (बहिर्गमनम्_fd >= 0 ? बहिर्गमनम्_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  यदि (बहिर्गमनम्_fd >= 0)
    unset_cloexec_flag (1);
  यदि (err_fd >= 0)
    unset_cloexec_flag (2);

  यदि (dev_null_fd >= 0)
    अस्थायी_FAILURE_RETRY (close (dev_null_fd));
  यदि (बहिर्गमनम्_fd >= 0)
    अस्थायी_FAILURE_RETRY (close (बहिर्गमनम्_fd));
  यदि (err_fd >= 0)
    अस्थायी_FAILURE_RETRY (close (err_fd));

  यदि (तर्काः == रिक्त)
    तर्काः = tmp_तर्काः;

  यदि (cwd && chdir (cwd) < 0)
    {
      यदि (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (मार्गः, तर्काः, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
पूर्णाङ्क
सततम्_process_with_stdin_timeout_envp (वर्ण *मार्गः, वर्ण **तर्काः, ध्रुव वर्ण *cwd, पूर्णाङ्क timeout,
                                     वर्ण **envp, वर्ण *stdin, size_t stdin_len, पूर्णाङ्क बहिर्गमनम्_fd,
                                     पूर्णाङ्क err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  पूर्णाङ्क stdin_pipe[2];
  pid_t pid;
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क pipe_r = -1;
  cleanup_close पूर्णाङ्क pipe_w = -1;
  sigset_t oldmask, आवरणम्;
  पूर्णाङ्क r, स्थितिः;

  sigemptyset (&आवरणम्);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  यदि (timeout > 0)
    {
      sigaddset (&आवरणम्, SIGCHLD);
      ret = sigprocmask (SIG_खण्डम्, &आवरणम्, &oldmask);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  यदि (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      गच्छ restore_sig_आवरणम्_and_exit;
    }

  यदि (pid == 0)
    {
      /* run_process_child doesn't return.  */
      सततम्_process_child (मार्गः, तर्काः, cwd, envp, pipe_r, pipe_w, बहिर्गमनम्_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = अस्थायी_FAILURE_RETRY (लिख (pipe_w, stdin, stdin_len));
  यदि (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      यदि (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          गच्छ restore_sig_आवरणम्_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  यदि (timeout)
    {
      time_t start = time (रिक्त);
      time_t now;
      प्रति (now = start; now - start < timeout; now = time (रिक्त))
        {
          siginfo_t info;
          पूर्णाङ्क elapsed = now - start;
          संरचना timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&आवरणम्, &info, &ts_timeout);
          यदि (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              गच्छ restore_sig_आवरणम्_and_exit;
            }

          यदि (info.si_signo == SIGCHLD && info.si_pid == pid)
            गच्छ read_waitpid;

          यदि (ret < 0 && errno == EAGAIN)
            गच्छ timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", मार्गः);
      गच्छ restore_sig_आवरणम्_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &स्थितिः, 0);
  यदि (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  अन्यथा
    ret = get_process_exit_स्थितिः (स्थितिः);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_आवरणम्_and_exit:
  यदि (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      यदि (pid > 0)
        {
          kill (pid, SIGKILL);
          अस्थायी_FAILURE_RETRY (waitpid (pid, &स्थितिः, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, रिक्त);
      यदि (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  प्रत्यागम ret;
}

पूर्णाङ्क
mark_or_close_fds_ge_than (libcrun_container_t *container, पूर्णाङ्क n, bool close_now, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क cfd = -1;
  cleanup_dir DIR *dir = रिक्त;
  पूर्णाङ्क ret;
  पूर्णाङ्क fd;
  संरचना dirent *अग्र;

#यदि_परिभाषितम् ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  यदि (close_now)
    GCOV_DUMP ();
#यदि_अन्तः

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  यदि (ret == 0)
    {
      यदि (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      प्रत्यागम 0;
    }
  यदि (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    प्रत्यागम crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  यदि (UNLIKELY (cfd < 0))
    प्रत्यागम cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dir = fdopendir (cfd);
  यदि (UNLIKELY (dir == रिक्त))
    प्रत्यागम crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  प्रति (अग्र = readdir (dir); अग्र; अग्र = readdir (dir))
    {
      पूर्णाङ्क val;
      ध्रुव वर्ण *name = अग्र->d_name;
      यदि (name[0] == '.')
        अनुवर्त;

      val = strtoll (name, रिक्त, 10);
      यदि (val < n || val == fd)
        अनुवर्त;

      यदि (close_now)
        {
          ret = close (val);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "close fd `%d`", val);

          यदि (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      अन्यथा
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  प्रत्यागम 0;
}

शून्य
get_current_timestamp (वर्ण *बहिर्गमनम्, size_t len)
{
  संरचना timeval tv;
  संरचना tm now;
  वर्ण timestamp[64];

  gettimeofday (&tv, रिक्त);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, परिमाण (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (शून्य) snprintf (बहिर्गमनम्, len, "%s.%06lldZ", timestamp, (दीर्घ दीर्घ पूर्णाङ्क) tv.tv_usec);
  बहिर्गमनम्[len - 1] = '\0';
}

पूर्णाङ्क
समुच्चयः_blocking_fd (पूर्णाङ्क fd, bool blocking, libcrun_error_t *err)
{
  पूर्णाङ्क ret, flags = fcntl (fd, F_GETFL, 0);
  यदि (UNLIKELY (flags < 0))
    प्रत्यागम crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fcntl");
  प्रत्यागम 0;
}

पूर्णाङ्क
parse_json_file (json_object **बहिर्गमनम्, ध्रुव वर्ण *jsondata, संरचना parser_context *ctx arg_अप्रयुक्तम्, libcrun_error_t *err)
{
  संरचना json_tokener *tok;
  गणना json_tokener_error jerr;

  *err = रिक्त;

  यदि (UNLIKELY (jsondata == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  यदि (UNLIKELY (tok == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_समुच्चयः_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *बहिर्गमनम् = json_tokener_parse_ex (tok, jsondata, (पूर्णाङ्क) वाक्यदैर्घ्यम् (jsondata));
  jerr = json_tokener_get_error (tok);
  यदि (*बहिर्गमनम् == रिक्त || jerr != json_tokener_success)
    {
      पूर्णाङ्क saved_pos = (पूर्णाङ्क) tok->char_विस्थापनम्;

      json_tokener_free (tok);
      यदि (*बहिर्गमनम्)
        {
          json_object_put (*बहिर्गमनम्);
          *बहिर्गमनम् = रिक्त;
        }
      प्रत्यागम crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  प्रत्यागम 0;
}

#परिभाषय CHECK_ACCESS_NOT_EXECUTABLE 1
#परिभाषय CHECK_ACCESS_NOT_REGULAR 2

/* check that the specified path exists and it is executable.
   Return
   - 0 if the file is executable
   - CHECK_ACCESS_NOT_EXECUTABLE if it exists but it is not executable
   - CHECK_ACCESS_NOT_REGULAR if it not a regular file
   - -errno for any other generic error
 */
स्थिर पूर्णाङ्क
check_access (ध्रुव वर्ण *मार्गः)
{
  पूर्णाङ्क ret;
  mode_t बहुलकम्;

#यदि_परिभाषितम् HAVE_EACCESS
#  define ACCESS(मार्गः, बहुलकम्) (eaccess (मार्गः, बहुलकम्))
#अन्यथा
#  define ACCESS(मार्गः, बहुलकम्) (access (मार्गः, बहुलकम्))
#यदि_अन्तः

  ret = ACCESS (मार्गः, X_OK);
  यदि (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      यदि (errno == EACCES)
        {
          पूर्णाङ्क saved_errno = errno;
          ret = ACCESS (मार्गः, F_OK);
          errno = saved_errno;

          यदि (ret == 0)
            प्रत्यागम CHECK_ACCESS_NOT_EXECUTABLE;
        }
      प्रत्यागम -errno;
    }

  ret = get_file_type (&बहुलकम्, false, मार्गः);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम -errno;

  यदि (! S_ISREG (बहुलकम्))
    प्रत्यागम CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  प्रत्यागम 0;
}

पूर्णाङ्क
find_executable (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *executable_मार्गः, ध्रुव वर्ण *cwd, libcrun_error_t *err)
{
  cleanup_free वर्ण *cwd_executable_मार्गः = रिक्त;
  cleanup_free वर्ण *tmp = रिक्त;
  पूर्णाङ्क चरमः_error = -ENOENT;
  वर्ण *it, *अन्तः;
  पूर्णाङ्क ret;

  *बहिर्गमनम् = रिक्त;

  यदि (executable_मार्गः == रिक्त || executable_मार्गः[0] == '\0')
    प्रत्यागम crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  यदि (executable_मार्गः[0] != '/' && strchr (executable_मार्गः, '/'))
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
      ret = append_paths (&cwd_executable_मार्गः, err, "/", cwd, executable_मार्गः, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      executable_मार्गः = cwd_executable_मार्गः;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  यदि (executable_मार्गः[0] == '/')
    {
      ret = check_access (executable_मार्गः);
      यदि (LIKELY (ret == 0))
        {
          *बहिर्गमनम् = xstrdup (executable_मार्गः);
          प्रत्यागम 0;
        }
      चरमः_error = ret;
      गच्छ fail;
    }

  अन्तः = tmp = xstrdup (getenv ("PATH"));

  यावत् ((it = strsep (&अन्तः, ":")))
    {
      cleanup_free वर्ण *मार्गः = रिक्त;

      यदि (it == अन्तः)
        it = ".";

      ret = append_paths (&मार्गः, err, it, executable_मार्गः, रिक्त);
      यदि (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          अनुवर्त;
        }

      ret = check_access (मार्गः);
      यदि (ret == 0)
        {
          /* Change owner.  */
          *बहिर्गमनम् = मार्गः;
          मार्गः = रिक्त;
          प्रत्यागम 0;
        }

      यदि (ret == -ENOENT)
        अनुवर्त;

      चरमः_error = ret;
    }

fail:
  विकल्प (चरमः_error)
    {
    पक्ष CHECK_ACCESS_NOT_EXECUTABLE:
      प्रत्यागम crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_मार्गः);

    पक्ष CHECK_ACCESS_NOT_REGULAR:
      प्रत्यागम crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_मार्गः);

    स्वभाव:
      errno = -चरमः_error;
      यदि (errno == ENOENT)
        प्रत्यागम crun_make_error (err, errno, "executable file `%s` not found%s", executable_मार्गः, executable_मार्गः[0] == '/' ? "" : " in $PATH");
      प्रत्यागम crun_make_error (err, errno, "open `%s`", executable_मार्गः);
    }
}

#यदि_परिभाषितम् HAVE_FGETXATTR

स्थिर ssize_t
safe_read_xattr (वर्ण **ret, पूर्णाङ्क sfd, ध्रुव वर्ण *srcname, ध्रुव वर्ण *name, size_t initial_आकार, libcrun_error_t *err)
{
  cleanup_free वर्ण *बफर = रिक्त;
  ssize_t current_आकार;
  ssize_t s;

  current_आकार = (ssize_t) initial_आकार;
  बफर = xmalloc (current_आकार + 1);

  यावत् (1)
    {
      s = fgetxattr (sfd, name, बफर, current_आकार);
      यदि (UNLIKELY (s < 0))
        प्रत्यागम crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      यदि (s < current_आकार)
        भङ्ग;

      current_आकार *= 2;
      बफर = xrealloc (बफर, current_आकार + 1);
    }

  यदि (s <= 0)
    प्रत्यागम s;

  बफर[s] = '\0';

  /* Change owner.  */
  *ret = बफर;
  बफर = रिक्त;

  प्रत्यागम s;
}

स्थिर ssize_t
copy_xattr (पूर्णाङ्क sfd, पूर्णाङ्क dfd, ध्रुव वर्ण *srcname, ध्रुव वर्ण *destname, libcrun_error_t *err)
{
  cleanup_free वर्ण *buf = रिक्त;
  ssize_t xattr_len;
  वर्ण *it;

  xattr_len = flistxattr (sfd, रिक्त, 0);
  यदि (UNLIKELY (xattr_len < 0))
    {
      यदि (errno == ENOTSUP)
        प्रत्यागम 0;

      प्रत्यागम crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  यदि (xattr_len == 0)
    प्रत्यागम 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  यदि (UNLIKELY (xattr_len < 0))
    प्रत्यागम crun_make_error (err, errno, "flistxattr `%s`", srcname);

  प्रति (it = buf; it - buf < xattr_len; it += वाक्यदैर्घ्यम् (it) + 1)
    {
      cleanup_free वर्ण *v = रिक्त;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      यदि (UNLIKELY (s < 0))
        प्रत्यागम s;

      s = fsetxattr (dfd, it, v, s, 0);
      यदि (UNLIKELY (s < 0))
        {
          यदि (errno == EINVAL || errno == EOPNOTSUPP)
            अनुवर्त;

          प्रत्यागम crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  प्रत्यागम 0;
}

#यदि_अन्तः

स्थिर पूर्णाङ्क
copy_rec_stat_file_at (पूर्णाङ्क dfd, ध्रुव वर्ण *मार्गः, mode_t *बहुलकम्, off_t *आकार, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  संरचना stat st;
  पूर्णाङ्क ret;

#यदि_परिभाषितम् HAVE_STATX
  संरचना statx stx = {
    0,
  };

  ret = statx (dfd, मार्गः, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_बहुलकम् | STATX_आकार | STATX_UID | STATX_GID, &stx);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (errno == ENOSYS || errno == EINVAL)
        गच्छ fallback;

      प्रत्यागम ret;
    }

  *बहुलकम् = stx.stx_mode;
  *आकार = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  प्रत्यागम ret;

fallback:
#यदि_अन्तः
  ret = fstatat (dfd, मार्गः, &st, AT_SYMLINK_NOFOLLOW);

  *बहुलकम् = st.st_mode;
  *आकार = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  प्रत्यागम ret;
}

पूर्णाङ्क
copy_recursive_fd_गन्तव्यम्_fd (पूर्णाङ्क srcdirfd, पूर्णाङ्क dfd, ध्रुव वर्ण *srcname, ध्रुव वर्ण *destname, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = रिक्त;
  संरचना dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  यदि (UNLIKELY (dsrcfd == रिक्त))
    {
      अस्थायी_FAILURE_RETRY (close (srcdirfd));
      प्रत्यागम crun_make_error (err, errno, "open directory `%s`", destname);
    }

  प्रति (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close पूर्णाङ्क srcfd = -1;
      cleanup_close पूर्णाङ्क destfd = -1;
      cleanup_free वर्ण *target_buf = रिक्त;
      पूर्णाङ्क ret;
      mode_t बहुलकम्;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      यदि (वाक्यं_तुलय (de->d_name, ".") == 0 || वाक्यं_तुलय (de->d_name, "..") == 0)
        अनुवर्त;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &बहुलकम्, &st_size, &rdev, &uid, &gid);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      विकल्प (बहुलकम् & S_IFMT)
        {
        पक्ष S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          यदि (UNLIKELY (srcfd < 0))
            प्रत्यागम crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          यदि (UNLIKELY (destfd < 0))
            प्रत्यागम crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_गन्तव्यम्_fd (srcfd, destfd, 1, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;

#यदि_परिभाषितम् HAVE_FGETXATTR
          ret = (पूर्णाङ्क) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
#यदि_अन्तः

          अस्थायी_FAILURE_RETRY (close (destfd));
          destfd = -1;
          भङ्ग;

        पक्ष S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, बहुलकम्);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          यदि (UNLIKELY (srcfd < 0))
            प्रत्यागम crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          यदि (UNLIKELY (destfd < 0))
            प्रत्यागम crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#यदि_परिभाषितम् HAVE_FGETXATTR
          ret = (पूर्णाङ्क) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
#यदि_अन्तः

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_गन्तव्यम्_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          भङ्ग;

        पक्ष S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          भङ्ग;

        पक्ष S_IFBLK:
        पक्ष S_IFCHR:
        पक्ष S_IFIFO:
        पक्ष S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, बहुलकम्, rdev);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          भङ्ग;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#यदि_अपरिभाषितम् ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#यदि_अन्तः

      ret = fchmodat (destdirfd, de->d_name, बहुलकम् & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      यदि (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          यदि (errno == ENOTSUP)
            अनुवर्त;

          प्रत्यागम crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  प्रत्यागम 0;
}

ध्रुव वर्ण *
find_annotation (libcrun_container_t *container, ध्रुव वर्ण *name)
{
  यदि (container->container_def->annotations == रिक्त)
    प्रत्यागम रिक्त;

  प्रत्यागम find_string_map_मानम् (container->annotations, name);
}

पूर्णाङ्क
safe_write (पूर्णाङ्क fd, ध्रुव वर्ण *fname, ध्रुव शून्य *buf, size_t गणनम्, libcrun_error_t *err)
{
  size_t written = 0;
  यावत् (written < गणनम्)
    {
      ssize_t w = लिख (fd, (ध्रुव वर्ण *) buf + written, गणनम् - written);
      यदि (UNLIKELY (w < 0))
        {
          यदि (errno == EINTR || errno == EAGAIN)
            अनुवर्त;
          प्रत्यागम crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  प्रत्यागम 0;
}

#परिभाषय MAX_PARTS 32

पूर्णाङ्क
append_paths (वर्ण **बहिर्गमनम्, libcrun_error_t *err, ...)
{
  ध्रुव वर्ण *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t योग_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  प्रति (;;)
    {
      ध्रुव वर्ण *part;
      size_t आकार;

      part = va_arg (ap, ध्रुव वर्ण *);
      यदि (part == रिक्त)
        भङ्ग;

      यदि (n_parts == MAX_PARTS)
        {
          va_अन्तः (ap);
          प्रत्यागम crun_make_error (err, EINVAL, "too many paths specified");
        }

      यदि (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          यावत् (part[0] == '/' && part[1] == '/')
            part++;
        }
      अन्यथा
        {
          /* And drop any initial '/' for other components.  */
          यावत् (part[0] == '/')
            part++;
        }

      आकार = वाक्यदैर्घ्यम् (part);
      यदि (आकार == 0)
        अनुवर्त;

      यावत् (आकार > 1 && part[आकार - 1] == '/')
        आकार--;

      parts[n_parts] = part;
      sizes[n_parts] = आकार;

      n_parts++;
    }
  va_अन्तः (ap);

  योग_len = n_parts + 1;
  प्रति (i = 0; i < n_parts; i++)
    योग_len += sizes[i];

  *बहिर्गमनम् = xmalloc (योग_len);

  copied = 0;
  प्रति (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*बहिर्गमनम्)[copied - 1] == '/';
      यदि (i > 0 && ! has_trailing_slash)
        {
          (*बहिर्गमनम्)[copied] = '/';
          copied += 1;
        }

      स्मृतिं_प्रतिलिख (*बहिर्गमनम् + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*बहिर्गमनम्)[copied] = '\0';
  प्रत्यागम 0;
}

#यदि __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#अन्यथा
#  define __nonstring
#यदि_अन्तः

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
स्थिर पूर्णाङ्क
b64_निवेशः (वर्ण c)
{
  ध्रुव वर्ण सारणी[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  पूर्णाङ्क i;

  प्रति (i = 0; i < 64; i++)
    {
      यदि (सारणी[i] == c)
        प्रत्यागम i;
    }
  प्रत्यागम -1;
}

पूर्णाङ्क
base64_decode (ध्रुव वर्ण *iptr, size_t isize, वर्ण *optr, size_t osize, size_t *nbytes)
{
  पूर्णाङ्क i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  अचिह्नित वर्ण दत्त[4];

  *nbytes = 0;
  यावत् (consumed < isize && (*nbytes) + 3 < osize)
    {
      यावत् ((i < 4) && (consumed < isize))
        {
          tmp = b64_निवेशः (*iptr++);
          consumed++;
          यदि (tmp != -1)
            {
              यदि (UNLIKELY (pad > 0))
                प्रत्यागम -1;
              दत्त[i++] = tmp;
            }
          अन्यथा यदि (*(iptr - 1) == '=')
            {
              दत्त[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      यदि (i == 4)
        {
          यदि (UNLIKELY (pad > 2))
            प्रत्यागम -1;
          *optr++ = (दत्त[0] << 2) | ((दत्त[1] & 0x30) >> 4);
          *optr++ = ((दत्त[1] & 0xf) << 4) | ((दत्त[2] & 0x3c) >> 2);
          *optr++ = ((दत्त[2] & 0x3) << 6) | दत्त[3];
          (*nbytes) += 3 - pad;
        }
      अन्यथा
        {
          /* I did not get all the data.  */
          consumed -= i;
          प्रत्यागम consumed;
        }
      i = 0;
    }
  प्रत्यागम consumed;
}

वर्ण *
get_user_name (uid_t uid)
{
  संरचना passwd pd;
  संरचना passwd *अस्थायी_परिणाम_ptr;
  वर्ण pwdbuffer[200];
  यदि (! getpwuid_r (uid, &pd, pwdbuffer, परिमाण (pwdbuffer), &अस्थायी_परिणाम_ptr))
    प्रत्यागम xstrdup (pd.pw_name);
  प्रत्यागम xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
वर्ण *
libcrun_sysctl_कुञ्चिका_गन्तव्यम्_proc_मार्गः (ध्रुव वर्ण *कुञ्चिका)
{
  ध्रुव वर्ण *sep;
  वर्ण *ret, *it;

  sep = strpbrk (कुञ्चिका, "./");

  /* No separator at all, or the key is already in the path form.  */
  यदि (sep == रिक्त || *sep == '/')
    प्रत्यागम xstrdup (कुञ्चिका);

  ret = xstrdup (कुञ्चिका);
  प्रति (it = ret; *it; it++)
    {
      यदि (*it == '.')
        *it = '/';
      अन्यथा यदि (*it == '/')
        *it = '.';
    }

  प्रत्यागम ret;
}

पूर्णाङ्क
has_suffix (ध्रुव वर्ण *str, ध्रुव वर्ण *suffix)
{
  यदि (! str || ! suffix)
    प्रत्यागम 0;
  size_t lenstr = वाक्यदैर्घ्यम् (str);
  size_t lensuffix = वाक्यदैर्घ्यम् (suffix);
  यदि (lensuffix > lenstr)
    प्रत्यागम 0;
  प्रत्यागम memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

वर्ण *
str_join_array (पूर्णाङ्क विस्थापनम्, size_t आकार, वर्ण *ध्रुव array[], ध्रुव वर्ण *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = रिक्त;
  size_t i, योग_आकार = (आकार - 1) * (jlen = वाक्यदैर्घ्यम् (joint)) + 1;
  वर्ण *परिणाम, *p;

  lens = xmalloc (आकार * परिमाण (*lens));

  प्रति (i = 0; i < आकार; ++i)
    {
      योग_आकार += (lens[i] = वाक्यदैर्घ्यम् (array[i]));
    }
  p = परिणाम = xmalloc (योग_आकार);
  प्रति (i = विस्थापनम्; i < आकार; ++i)
    {
      स्मृतिं_प्रतिलिख (p, array[i], lens[i]);
      p += lens[i];
      यदि (i < आकार - 1)
        {
          स्मृतिं_प्रतिलिख (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  प्रत्यागम परिणाम;
}

पूर्णाङ्क
libcrun_mmap (संरचना libcrun_mmap_s **ret, शून्य *addr, size_t दीर्घता,
              पूर्णाङ्क prot, पूर्णाङ्क flags, पूर्णाङ्क fd, off_t विस्थापनम्,
              libcrun_error_t *err)
{
  संरचना libcrun_mmap_s *mmap_s = रिक्त;

  शून्य *mapped = mmap (addr, दीर्घता, prot, flags, fd, विस्थापनम्);
  यदि (mapped == MAP_FAILED)
    प्रत्यागम crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (परिमाण (संरचना libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->दीर्घता = दीर्घता;

  *ret = mmap_s;

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_munmap (संरचना libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = munmap (mmap->addr, mmap->दीर्घता);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "munmap");

  मुक्त (mmap);

  प्रत्यागम 0;
}

स्थिर दीर्घ
read_file_as_long_or_default (ध्रुव वर्ण *मार्गः, दीर्घ def_मानम्)
{
  cleanup_free वर्ण *content = रिक्त;
  libcrun_error_t tmp_err = रिक्त;
  वर्ण *endptr;
  दीर्घ val;
  पूर्णाङ्क ret;

  ret = read_all_file (मार्गः, &content, रिक्त, &tmp_err);
  यदि (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      प्रत्यागम def_मानम्;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  यदि (UNLIKELY (errno))
    प्रत्यागम def_मानम्;
  यदि (endptr == content || (*endptr && *endptr != '\n'))
    प्रत्यागम def_मानम्;
  प्रत्यागम val;
}

#परिभाषय DEFAULT_अतिप्रवाहः_ID 65534

uid_t
get_अतिप्रवाहः_uid (शून्य)
{
  स्थिर atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  यदि (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_अतिप्रवाहः_ID);
      cached_uid = uid;
    }
  प्रत्यागम uid;
}

gid_t
get_अतिप्रवाहः_gid (शून्य)
{
  स्थिर atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  यदि (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_अतिप्रवाहः_ID);
      cached_gid = gid;
    }
  प्रत्यागम gid;
}

शून्य
consume_trailing_slashes (वर्ण *मार्गः)
{
  यदि (! मार्गः || मार्गः[0] == '\0')
    प्रत्यागम;

  वर्ण *चरमः = मार्गः + वाक्यदैर्घ्यम् (मार्गः);

  यावत् (चरमः > मार्गः && *(चरमः - 1) == '/')
    चरमः--;

  *चरमः = '\0';
}

वर्ण **
read_dir_entries (ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = रिक्त;
  size_t n_entries = 0;
  size_t entries_आकार = 16;
  वर्ण **entries = रिक्त;
  संरचना dirent *de;

  dir = opendir (मार्गः);
  यदि (UNLIKELY (dir == रिक्त))
    {
      crun_make_error (err, errno, "opendir `%s`", मार्गः);
      प्रत्यागम रिक्त;
    }

  entries = xmalloc (entries_आकार * परिमाण (वर्ण *));
  यावत् ((de = readdir (dir)))
    {
      यदि (वाक्यं_तुलय (de->d_name, ".") == 0 || वाक्यं_तुलय (de->d_name, "..") == 0)
        अनुवर्त;
      यदि (n_entries == entries_आकार)
        {
          entries_आकार *= 2;
          entries = xrealloc (entries, entries_आकार * परिमाण (वर्ण *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * परिमाण (वर्ण *));
  entries[n_entries] = रिक्त;

  प्रत्यागम entries;
}

पूर्णाङ्क
cpuset_string_गन्तव्यम्_bitmask (ध्रुव वर्ण *str, वर्ण **बहिर्गमनम्, size_t *बहिर्गमनम्_आकार, libcrun_error_t *err)
{
  cleanup_free वर्ण *आवरणम् = रिक्त;
  size_t आवरणम्_आकार = 0;
  ध्रुव वर्ण *p = str;
  वर्ण *endptr;

  यावत् (*p)
    {
      दीर्घ दीर्घ start_range, अन्तः_range;

      यदि (*p < '0' || *p > '9')
        गच्छ invalid_निवेशः;

      start_range = strtoll (p, &endptr, 10);
      यदि (start_range < 0)
        गच्छ invalid_निवेशः;

      p = endptr;

      यदि (*p != '-')
        अन्तः_range = start_range;
      अन्यथा
        {
          p++;

          यदि (*p < '0' || *p > '9')
            गच्छ invalid_निवेशः;

          अन्तः_range = strtoll (p, &endptr, 10);

          यदि (अन्तः_range < start_range)
            गच्छ invalid_निवेशः;

          p = endptr;
        }

      /* Just set some limit.  */
      यदि (अन्तः_range > (1 << 20))
        गच्छ invalid_निवेशः;

      यदि (अन्तः_range >= (दीर्घ दीर्घ) (आवरणम्_आकार * CHAR_BIT))
        {
          size_t new_आवरणम्_आकार = (अन्तः_range / CHAR_BIT) + 1;
          आवरणम् = xrealloc (आवरणम्, new_आवरणम्_आकार);
          स्मृतिं_पूरय (आवरणम् + आवरणम्_आकार, 0, new_आवरणम्_आकार - आवरणम्_आकार);
          आवरणम्_आकार = new_आवरणम्_आकार;
        }

      प्रति (दीर्घ दीर्घ i = start_range; i <= अन्तः_range; i++)
        आवरणम्[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      यदि (*p == ',')
        p++;
      अन्यथा यदि (*p)
        गच्छ invalid_निवेशः;
    }

  *बहिर्गमनम् = आवरणम्;
  आवरणम् = रिक्त;
  *बहिर्गमनम्_आकार = आवरणम्_आकार;

  प्रत्यागम 0;

invalid_निवेशः:
  प्रत्यागम crun_make_error (err, 0, "cannot parse input `%s`", str);
}

संरचना channel_fd_pair
{
  संरचना ring_बफर *rb;

  पूर्णाङ्क अन्तर्_fd;
  पूर्णाङ्क बहिर्गमनम्_fd;

  पूर्णाङ्क infd_epoll_events;
  पूर्णाङ्क outfd_epoll_events;
};

संरचना channel_fd_pair *
channel_fd_pair_new (पूर्णाङ्क अन्तर्_fd, पूर्णाङ्क बहिर्गमनम्_fd, size_t आकार)
{
  संरचना channel_fd_pair *channel = xmalloc (परिमाण (संरचना channel_fd_pair));
  channel->अन्तर्_fd = अन्तर्_fd;
  channel->बहिर्गमनम्_fd = बहिर्गमनम्_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_बफर_make (आकार);
  प्रत्यागम channel;
}

शून्य
channel_fd_pair_free (संरचना channel_fd_pair *channel)
{
  यदि (channel == रिक्त)
    प्रत्यागम;

  ring_बफर_free (channel->rb);
  मुक्त (channel);
}

पूर्णाङ्क
channel_fd_pair_process (संरचना channel_fd_pair *channel, पूर्णाङ्क epollfd, libcrun_error_t *err)
{
  bool is_निवेशः_eagain = false, is_output_eagain = false, repeat;
  पूर्णाङ्क ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  प्रति (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      यदि (ring_बफर_get_space_उपलब्धम् (channel->rb) > 0)
        {
          ret = ring_बफर_read (channel->rb, channel->अन्तर्_fd, &is_निवेशः_eagain, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          यदि (ret > 0)
            repeat = true;
        }
      यदि (ring_बफर_get_दत्त_उपलब्धम् (channel->rb) > 0)
        {
          ret = ring_बफर_write (channel->rb, channel->बहिर्गमनम्_fd, &is_output_eagain, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          यदि (ret > 0)
            repeat = true;
        }
    }

  यदि (epollfd >= 0)
    {
      size_t उपलब्धम् = ring_बफर_get_space_उपलब्धम् (channel->rb);
      size_t प्रयुक्तम् = ring_बफर_get_दत्त_उपलब्धम् (channel->rb);
      पूर्णाङ्क events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (उपलब्धम् > 0 && ! is_output_eagain) ? (EPOLLIN | (is_निवेशः_eagain ? EPOLLET : 0)) : 0;
      यदि (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->अन्तर्_fd, events, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (प्रयुक्तम् > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      यदि (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->बहिर्गमनम्_fd, events, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          channel->outfd_epoll_events = events;
        }
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
open_proc (libcrun_error_t *err)
{
  पूर्णाङ्क fd;

#यदि_परिभाषितम् HAVE_NEW_MOUNT_API
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
            प्रत्यागम fd;
        }
    }
#यदि_अन्तः

  fd = उद्घाटय ("/proc", O_मार्गः | O_DIRECTORY | O_CLOEXEC);
  यदि (fd < 0)
    प्रत्यागम crun_make_error (err, errno, "open `/proc`");
  प्रत्यागम fd;
}

पूर्णाङ्क
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  यदि (! container)
    प्रत्यागम open_proc (err);

  यदि (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  प्रत्यागम container->proc_fd;
}

पूर्णाङ्क
libcrun_open_proc_file (libcrun_container_t *container, ध्रुव वर्ण *मार्गः, पूर्णाङ्क flags, libcrun_error_t *err)
{
  पूर्णाङ्क proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  यदि (proc_fd < 0)
    प्रत्यागम proc_fd;

  fd = अस्थायी_FAILURE_RETRY (openat (proc_fd, मार्गः, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  यदि (! container)
    अस्थायी_FAILURE_RETRY (close (proc_fd));

  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, saved_errno, "openat `/proc/%s`", मार्गः);

  प्रत्यागम fd;
}

पूर्णाङ्क
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ध्रुव वर्ण *मार्गः, पूर्णाङ्क flags, libcrun_error_t *err)
{
  cleanup_free वर्ण *पूर्णम्_मार्गः = रिक्त;
  xasprintf (&पूर्णम्_मार्गः, "%d/%s", pid, मार्गः);
  प्रत्यागम libcrun_open_proc_file (container, पूर्णम्_मार्गः, flags, err);
}
