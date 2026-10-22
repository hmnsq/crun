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
#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "ring_buffer.h"
#ଅନ୍ତର୍ଭାଵ <stdarg.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <libgen.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/un.h>
#ଅନ୍ତର୍ଭାଵ <sys/socket.h>
#ଅନ୍ତର୍ଭାଵ <sys/signalfd.h>
#ଅନ୍ତର୍ଭାଵ <sys/epoll.h>
#ଅନ୍ତର୍ଭାଵ <sys/syscall.h>
#ଅନ୍ତର୍ଭାଵ <sys/wait.h>
#ଅନ୍ତର୍ଭାଵ <pwd.h>
#ଅନ୍ତର୍ଭାଵ <grp.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <sys/time.h>
#ଅନ୍ତର୍ଭାଵ <sys/xattr.h>
#ଅନ୍ତର୍ଭାଵ <sys/sysmacros.h>
#ଅନ୍ତର୍ଭାଵ <sys/vfs.h>
#ଅନ୍ତର୍ଭାଵ <linux/magic.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <sys/mman.h>
#ifdef HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#endif
#ଯଦି HAVE_STDATOMIC_H
#  include <stdatomic.h>
#ଅନ୍ୟଥା
#  define atomic_long ଅସ୍ଥିର long
#  define atomic_int ଅସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
#endif
#ଅନ୍ତର୍ଭାଵ "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_ମୂଳ
#  define RESOLVE_IN_ମୂଳ 0x10
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_path_exists (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = access (path, F_OK);
  ଯଦି (ret < 0)
    {
      ଯଦି (errno == ENOENT)
        ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ crun_make_error (err, errno, "access `%s`", path);
    }
  ଫେରନ୍ତୁ 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xasprintf (ଅକ୍ଷର **str, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  ଯଦି (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_file_at_with_flags (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, mode_t mode, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଶୂନ୍ୟ *data, size_t len, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ (len > INT_MAX) ? INT_MAX : (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) len;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
detach_process ()
{
  pid_t pid;
  ଯଦି (setsid () < 0)
    ଫେରନ୍ତୁ -1;
  pid = fork ();
  ଯଦି (pid < 0)
    ଫେରନ୍ତୁ -1;
  ଯଦି (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_file_type_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, mode_t *mode)
{
  ସଂରଚନା stat st;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

#ifdef HAVE_STATX
  ସଂରଚନା statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (errno == ENOSYS || errno == EINVAL)
        ଯାଅ fallback;

      ଫେରନ୍ତୁ ret;
    }
  *mode = stx.stx_mode;
  ଫେରନ୍ତୁ ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_file_type_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, mode_t *mode, bool nofollow, ଅଚଳ ଅକ୍ଷର *path)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା empty_path = path == ଖାଲି ? AT_EMPTY_PATH : 0;
  ସଂରଚନା stat st;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

#ifdef HAVE_STATX
  ସଂରଚନା statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (errno == ENOSYS || errno == EINVAL)
        ଯାଅ fallback;

      ଫେରନ୍ତୁ ret;
    }
  *mode = stx.stx_mode;
  ଫେରନ୍ତୁ ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_file_type (mode_t *mode, bool nofollow, ଅଚଳ ଅକ୍ଷର *path)
{
  ଫେରନ୍ତୁ get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
create_file_if_missing_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  ଯଦି (fd_write < 0)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
      mode_t tmp_mode;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      ଯଦି (ret == 0 && S_ISREG (tmp_mode))
        ଫେରନ୍ତୁ 0;

      ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ensure_directory_internal_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅକ୍ଷର *path, size_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, libcrun_error_t *err)
{
  ଅକ୍ଷର *it = path + len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
  bool parent_created = false;

  ପାଇଁ (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      ଯଦି (ret == 0 || errno == EEXIST)
        ଫେରନ୍ତୁ 0;

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
      ଯଦି (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = ଖାଲି;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          ଯଦି (ret > 0)
            ଫେରନ୍ତୁ 0;
          ଯଦି (ret < 0)
            crun_error_release (&tmp_err);

          ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      ଯେତେବେଳେ (it > path && *it != '/')
        {
          it--;
          len--;
        }
      ଯଦି (it == path)
        ଫେରନ୍ତୁ 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      parent_created = true;
    }
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_ensure_directory_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, bool nofollow, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_free ଅକ୍ଷର *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (ret == 0)
    ଫେରନ୍ତୁ crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
check_fd_is_path (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  ଅକ୍ଷର link[PATH_MAX];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, ଆକାର (link)));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "readlink `%s`", fdname);

  ଯଦି (((size_t) ret) != path_len || memcmp (link, path, path_len))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
check_fd_under_path (ଅଚଳ ଅକ୍ଷର *rootfs, size_t rootfslen, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  ଅକ୍ଷର link[PATH_MAX];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  ଯଦି (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    ଫେରନ୍ତୁ 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, ଆକାର (link)));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "readlink `%s`", fdname);

  ଯଦି (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    ଫେରନ୍ତୁ crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  ଫେରନ୍ତୁ 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
ସ୍ଥିର ଶୂନ୍ୟ
close_and_replace (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *oldfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା newfd)
{
  ଯଦି (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
ଅକ୍ଷର *chroot_realpath (ଅଚଳ ଅକ୍ଷର *chroot, ଅଚଳ ଅକ୍ଷର *path, ଅକ୍ଷର resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
safe_openat_fallback (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *rootfs, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
                      ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *parent_path = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *last_component = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *orig_path = path;
  ଅଚଳ ଅକ୍ଷର *path_in_chroot;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ଅକ୍ଷର resolved[PATH_MAX];
  ଅକ୍ଷର buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  ଯଦି (flags & O_NOFOLLOW)
    {
      ଅକ୍ଷର *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      ଯଦି (sep == ଖାଲି)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      ଅନ୍ୟଥା ଯଦି (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      ଯଦି (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  ଯଦି (path_in_chroot == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  ଯଦି (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  ଯଦି (last_component)
    {
      ret = snprintf (resolved, ଆକାର (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      ଯଦି (UNLIKELY (ret >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (resolved)))
        ଫେରନ୍ତୁ crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  ଯଦି (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      ଫେରନ୍ତୁ ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = fd;
  fd = -1;
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
safe_openat (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *rootfs, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode,
             libcrun_error_t *err)
{
  ସ୍ଥିର bool openat2_supported = true;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା retries = MAX_OPENAT2_EAGAIN_RETRIES;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (is_empty_string (path))
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      ଯଦି (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      ଯଦି (UNLIKELY (fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      ଯଦି (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ret = fd;
      fd = -1;
      ଫେରନ୍ତୁ ret;
    }

  ଯଦି (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_ମୂଳ);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (errno == EINTR)
            ଯାଅ repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          ଯଦି (errno == EAGAIN && retries-- > 0)
            ଯାଅ repeat;
          ଯଦି (errno == ENOSYS)
            openat2_supported = false;
          ଯଦି (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            ଫେରନ୍ତୁ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          ଫେରନ୍ତୁ crun_make_error (err, errno, "openat2 `%s`", path);
        }

      ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd, ଅଚଳ ଅକ୍ଷର *name, ଅକ୍ଷର **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free ଅକ୍ଷର *tmp_buf = ଖାଲି;
  ssize_t size;

  କର
    {
      ଯଦି (tmp_buf != ଖାଲି)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      ଯଦି (UNLIKELY (size < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "readlink `%s`", name);
  } ଯେତେବେଳେ (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = ଖାଲି;

  ଫେରନ୍ତୁ size;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *dirpath,
                                ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *dirpath, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cwd,
                                 ଅଚଳ ଅକ୍ଷର *npath, ଅଚଳ ଅକ୍ଷର *cur, ଅଚଳ ଅକ୍ଷର *rest,
                                 ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_readlinks, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *target = ଖାଲି;
  cleanup_free ଅକ୍ଷର *new_path = ଖାଲି;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  ଫେରନ୍ତୁ crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *dirpath,
                     ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_readlinks, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା wd_cleanup = -1;
  cleanup_free ଅକ୍ଷର *npath = ଖାଲି;
  bool last_component = false;
  size_t depth = 0;
  ଅକ୍ଷର *cur;
  ଅକ୍ଷର *it;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cwd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (max_readlinks <= 0)
    ଫେରନ୍ତୁ crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  ଯଦି (*path == '\0')
    {
      ଯଦି (do_open)
        ଫେରନ୍ତୁ open (dirpath, O_CLOEXEC | O_PATH, 0);
      ଫେରନ୍ତୁ 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  ଯେତେବେଳେ (cur)
    {
      ଯଦି (it)
        *it = '\0';
      ଅନ୍ୟଥା
        last_component = true;

      ଯଦି (cur[0] == '\0')
        ଭାଙ୍ଗ;

      ଯଦି (strcmp (cur, ".") == 0)
        ଯାଅ next;
      ଅନ୍ୟଥା ଯଦି (strcmp (cur, ".."))
        depth++;
      ଅନ୍ୟଥା
        {
          ଯଦି (depth)
            depth--;
          ଅନ୍ୟଥା
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              ଯାଅ next;
            }
        }

      ଯଦି (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          ଯଦି (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              ଯଦି (errno == ELOOP)
                ଫେରନ୍ତୁ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, ଖାଲି,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              ଯଦି (ret < 0)
                ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          ଯଦି (do_open)
            ଫେରନ୍ତୁ ret;

          close_and_replace (&wd_cleanup, ret);
          ଫେରନ୍ତୁ 0;
        }

      ret = mkdirat (cwd, cur, mode);
      ଯଦି (ret < 0)
        {
          mode_t st_mode;

          ଯଦି (errno != EEXIST)
            ଫେରନ୍ତୁ crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          ଯଦି (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            ଫେରନ୍ତୁ crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : ଖାଲି, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      ଯଦି (UNLIKELY (cwd < 0))
        ଫେରନ୍ତୁ crun_error_wrap (err, "creating `/%s`", path);

      ଯଦି (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, ଖାଲି);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;

              close (cwd);
              ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          ଯଦି ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              ଫେରନ୍ତୁ crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      ଯଦି (it == ଖାଲି)
        ଭାଙ୍ଗ;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  ଯଦି (do_open)
    {
      ଯଦି (cwd == dirfd)
        {
          ret = dup (dirfd);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "dup `%s`", dirpath);
          ଫେରନ୍ତୁ ret;
        }

      wd_cleanup = -1;
      ଫେରନ୍ତୁ cwd;
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_safe_create_and_open_ref_at (bool dir, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *dirpath, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  ଯଦି (LIKELY (ret >= 0))
    ଫେରନ୍ତୁ ret;

  crun_error_release (err);
  ଫେରନ୍ତୁ crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_safe_ensure_directory_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *dirpath, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode,
                               libcrun_error_t *err)
{
  ଫେରନ୍ତୁ crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_safe_ensure_file_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *dirpath, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode,
                          libcrun_error_t *err)
{
  ଫେରନ୍ତୁ crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_ensure_directory (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, bool nofollow, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_file_size (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, off_t *size)
{
  ସଂରଚନା stat st;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
#ifdef HAVE_STATX
  ସଂରଚନା statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (errno == ENOSYS || errno == EINVAL)
        ଯାଅ fallback;
      ଫେରନ୍ତୁ ret;
    }
  *size = stx.stx_size;

  ଫେରନ୍ତୁ ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_dir_p_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "stat `%s`", path);

  ଫେରନ୍ତୁ S_ISDIR (mode) ? 1 : 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_dir_p (ଅଚଳ ଅକ୍ଷର *path, bool nofollow, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  size_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      ଯଦି (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ଫେରନ୍ତୁ 0;
        }
      ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
ସ୍ଥିର bool
running_in_openvz ()
{
  ଯଦି (access ("/proc/vz", F_OK) < 0)
    ଫେରନ୍ତୁ false;

  ଫେରନ୍ତୁ access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
check_running_in_user_namespace (libcrun_error_t *err)
{
  ସ୍ଥିର atomic_int run_in_userns = -1;
  ସଂରଚନା stat st;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = run_in_userns;
  ଯଦି (ret >= 0)
    ଫେରନ୍ତୁ ret;

  ret = stat ("/proc/self/ns/user", &st);
  ଯଦି (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      ଯଦି (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          ଫେରନ୍ତୁ run_in_userns;
        }

      ଯଦି (! running_in_openvz ())
        {
          run_in_userns = 1;
          ଫେରନ୍ତୁ run_in_userns;
        }
    }
  ଅନ୍ୟଥା ଯଦି (UNLIKELY (errno != ENOENT))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  run_in_userns = ret;
  ଫେରନ୍ତୁ run_in_userns;
}

ସ୍ଥିର size_t
get_page_size ()
{
  ସ୍ଥିର atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  ଯଦି (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  ଫେରନ୍ତୁ (size_t) pagesize;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା selinux_enabled = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା apparmor_enabled = -1;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *out = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  size_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (selinux_enabled >= 0)
    ଫେରନ୍ତୁ selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  ଫେରନ୍ତୁ selinux_enabled;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା size;
  ଅକ୍ଷର buf[1];

  ଯଦି (apparmor_enabled >= 0)
    ଫେରନ୍ତୁ apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  ଯଦି (fd == -1)
    {
      ଯଦି (errno == ENOENT)
        {
          apparmor_enabled = 0;
          ଫେରନ୍ତୁ 0;
        }

      ଫେରନ୍ତୁ crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (ପଠ (fd, buf, ଆକାର (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  ଫେରନ୍ତୁ apparmor_enabled;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  ଯଦି (selinux_enabled < 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "SELinux is not initialized correctly");
  ଫେରନ୍ତୁ selinux_enabled;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
add_selinux_mount_label (ଅକ୍ଷର **retlabel, ଅଚଳ ଅକ୍ଷର *data, ଅଚଳ ଅକ୍ଷର *label, ଅଚଳ ଅକ୍ଷର *context_type, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_is_selinux_enabled (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (label && ret)
    {
      ଯଦି (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      ଅନ୍ୟଥା
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      ଫେରନ୍ତୁ 0;
    }
  *retlabel = xstrdup (data);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
lsm_attr_path (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *lsm, ଅଚଳ ଅକ୍ଷର *fname, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା attr_dirfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା lsm_dirfd = -1;
  ଅକ୍ଷର *attr_path = ଖାଲି;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  ଯଦି (UNLIKELY (attr_dirfd < 0))
    ଫେରନ୍ତୁ ଖାଲି;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  ଯଦି (lsm != ଖାଲି)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      ଯଦି (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          ଫେରନ୍ତୁ ଖାଲି;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  ଫେରନ୍ତୁ attr_path;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
check_proc_super_magic (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  ସଂରଚନା statfs sfs;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = fstatfs (fd, &sfs);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "statfs `%s`", path);

  ଯଦି (sfs.f_type != PROC_SUPER_MAGIC)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
set_security_attr (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *lsm, ଅଚଳ ଅକ୍ଷର *fname, ଅଚଳ ଅକ୍ଷର *data, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cleanup_free ଅଚଳ ଅକ୍ଷର *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

  ଯଦି (UNLIKELY (attr_path == ଖାଲି))
    ଫେରନ୍ତୁ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (ଲିଖ (fd, data, strlen (data)));
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      ଯଦି (errno == ENOENT && lsm != ଖାଲି && strcmp (lsm, "apparmor") == 0)
        {
          ଅଚଳ ଅକ୍ଷର *profile = strchr (data, ' ');

          ଫେରନ୍ତୁ crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      ଫେରନ୍ତୁ crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
set_selinux_label (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *label, bool now, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_is_selinux_enabled (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (ret)
    ଫେରନ୍ତୁ set_security_attr (container, ଖାଲି, now ? "current" : "exec", label, err);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  ଯଦି (apparmor_enabled < 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "AppArmor is not initialized correctly");
  ଫେରନ୍ତୁ apparmor_enabled;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (ଆକାର (UNCONFINED) - 1)
  cleanup_free ଅଚଳ ଅକ୍ଷର *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  ଅକ୍ଷର buf[UNCONFINED_LEN + 1];

  ଯଦି (UNLIKELY (attr_path == ଖାଲି))
    ଫେରନ୍ତୁ -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ fd;

  ଯଦି (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    ଫେରନ୍ତୁ -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (ପଠ (fd, buf, ଆକାର (buf)));
  ଯଦି (UNLIKELY (bytes_read < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  ଯଦି (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    ଫେରନ୍ତୁ 1;
  ଯଦି (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    ଫେରନ୍ତୁ 0;
  ଫେରନ୍ତୁ 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
set_apparmor_profile (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_is_apparmor_enabled (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (ret)
    {
      cleanup_free ଅକ୍ଷର *buf = ଖାଲି;
      ret = is_current_process_confined (container, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      ଫେରନ୍ତୁ set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_all_fd_internal (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *description, ଅକ୍ଷର **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *buf = ଖାଲି;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (size_hint)
    allocated = size_hint;
  ଅନ୍ୟଥା
    {
      ret = get_file_size (fd, &size);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "stat `%s`", description);

      ଯଦି (max_size && (size_t) size > max_size)
        ଫେରନ୍ତୁ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  ଯେତେବେଳେ ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (ପଠ (fd, buf + nread, allocated - nread));
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "read from file `%s`", description);

      ଯଦି (ret == 0)
        ଭାଙ୍ଗ;

      nread += ret;

      ଯଦି (max_size && nread > max_size)
        ଫେରନ୍ତୁ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      ଯଦି (nread == allocated)
        {
          ଯଦି (size)
            ଭାଙ୍ଗ;

          ଯଦି (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  ଯଦି (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      ଅକ୍ଷର *tmp = realloc (buf, nread + 1);
      ଯଦି (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = ଖାଲି;
  ଯଦି (len)
    *len = nread;
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_all_fd_with_size_hint (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *description, ଅକ୍ଷର **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_all_fd_with_max_size (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *description, ଅକ୍ଷର **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_all_file_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *path, ଅକ୍ଷର **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", path);

  ଫେରନ୍ତୁ read_all_fd (fd, path, out, len, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_all_file (ଅଚଳ ଅକ୍ଷର *path, ଅକ୍ଷର **out, size_t *len, libcrun_error_t *err)
{
  ଯଦି (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  ଫେରନ୍ତୁ read_all_file_at (AT_FDCWD, path, out, len, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_realpath_to_file (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *path_name, ଅକ୍ଷର **absolute_path, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  ଯଦି (UNLIKELY (targetfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", path_name);
  ଅନ୍ୟଥା
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      ଯଦି (UNLIKELY (len < 0))
        ଫେରନ୍ତୁ crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_unix_domain_client_socket (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dgram, libcrun_error_t *err)
{
  ସଂରଚନା sockaddr_un addr = {};
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  proc_fd_path_t name_buf;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା destfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "create UNIX socket");

  ଯଦି (strlen (path) >= ଆକାର (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      ଯଦି (UNLIKELY (destfd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  ଯଦି (path_len >= ଆକାର (addr.sun_path))
    ଫେରନ୍ତୁ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (ସଂରଚନା sockaddr *) &addr, ଆକାର (addr));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_unix_domain_socket (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dgram, libcrun_error_t *err)
{
  ସଂରଚନା sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "create UNIX socket");

  ଯଦି (strlen (path) >= ଆକାର (addr.sun_path))
    {
      cleanup_free ଅକ୍ଷର *dpath = xstrdup (path);
      cleanup_free ଅକ୍ଷର *bpath = xstrdup (path);
      ଅଚଳ ଅକ୍ଷର *parent_dir = dirname (dpath);
      ଅଚଳ ଅକ୍ଷର *base = basename (bpath);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      ଯଦି (UNLIKELY (dirfd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, ଆକାର (addr.sun_path), "%s/%s", name_buf, base);
      ଯଦି (n < 0 || (size_t) n >= ଆକାର (addr.sun_path))
        ଫେରନ୍ତୁ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  ଅନ୍ୟଥା
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (ସଂରଚନା sockaddr *) &addr, ଆକାର (addr));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "bind socket to `%s`", path);

  ଯଦି (! dgram)
    {
      ret = listen (fd, 1);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
send_fd_to_socket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା server, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ send_fd_to_socket_with_payload (server, fd, ଖାଲି, 0, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
send_fd_to_socket_with_payload (ପୂର୍ଣ୍ଣସଂଖ୍ୟା server, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *payload, size_t payload_len, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ସଂରଚନା cmsghdr *cmsg = ଖାଲି;
  ସଂରଚନା iovec iov[2];
  ସଂରଚନା msghdr ସନ୍ଦେଶ = {};
  ଅକ୍ଷର ctrl_buf[CMSG_SPACE (1 + ଆକାର (ପୂର୍ଣ୍ଣସଂଖ୍ୟା))] = {};
  ଅକ୍ଷର data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = ଆକାର (data);

  ଯଦି (payload_len > 0)
    {
      iov[0].iov_base = (ଶୂନ୍ୟ *) payload;
      iov[0].iov_len = payload_len;
    }

  ସନ୍ଦେଶ.ସନ୍ଦେଶ_name = ଖାଲି;
  ସନ୍ଦେଶ.ସନ୍ଦେଶ_namelen = 0;
  ସନ୍ଦେଶ.ସନ୍ଦେଶ_iov = iov;
  ସନ୍ଦେଶ.ସନ୍ଦେଶ_iovlen = 1;
  ସନ୍ଦେଶ.ସନ୍ଦେଶ_controllen = CMSG_SPACE (ଆକାର (ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
  ସନ୍ଦେଶ.ସନ୍ଦେଶ_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&ସନ୍ଦେଶ);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (ଆକାର (ପୂର୍ଣ୍ଣସଂଖ୍ୟା));

  *((ପୂର୍ଣ୍ଣସଂଖ୍ୟା *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &ସନ୍ଦେଶ, 0));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "sendmsg");
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
receive_fd_from_socket_with_payload (ପୂର୍ଣ୍ଣସଂଖ୍ୟା from, ଅକ୍ଷର *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ସଂରଚନା iovec iov[1];
  ସଂରଚନା msghdr ସନ୍ଦେଶ = {};
  ଅକ୍ଷର ctrl_buf[CMSG_SPACE (ଆକାର (ପୂର୍ଣ୍ଣସଂଖ୍ୟା))] = {};
  ଅକ୍ଷର data[1];
  ସଂରଚନା cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = ଆକାର (data);

  ଯଦି (payload_len > 0)
    {
      iov[0].iov_base = (ଶୂନ୍ୟ *) payload;
      iov[0].iov_len = payload_len;
    }

  ସନ୍ଦେଶ.ସନ୍ଦେଶ_name = ଖାଲି;
  ସନ୍ଦେଶ.ସନ୍ଦେଶ_namelen = 0;
  ସନ୍ଦେଶ.ସନ୍ଦେଶ_iov = iov;
  ସନ୍ଦେଶ.ସନ୍ଦେଶ_iovlen = 1;
  ସନ୍ଦେଶ.ସନ୍ଦେଶ_controllen = CMSG_SPACE (ଆକାର (ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
  ସନ୍ଦେଶ.ସନ୍ଦେଶ_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &ସନ୍ଦେଶ, 0));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "recvmsg");
  ଯଦି (UNLIKELY (ret == 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&ସନ୍ଦେଶ);
  ଯଦି (cmsg == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), ଆକାର (fd));

  ret = fd;
  fd = -1;
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
receive_fd_from_socket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା from, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ receive_fd_from_socket_with_payload (from, ଖାଲି, 0, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
create_socket_pair (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pair, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "socketpair");
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = signalfd (-1, mask, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "signalfd");
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
epoll_helper_toggle (ପୂର୍ଣ୍ଣସଂଖ୍ୟା epollfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା events, libcrun_error_t *err)
{
  ସଂରଚନା epoll_event ev = {};
  bool add = events != 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  ଯଦି (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      ଯଦି (errno == ENOENT || errno == EPERM)
        ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
epoll_helper (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *in_fds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *in_levelfds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out_fds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out_levelfds, libcrun_error_t *err)
{
  ସଂରଚନା epoll_event ev;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା epollfd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *it;
  epollfd = epoll_create1 (0);
  ଯଦି (UNLIKELY (epollfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  ପାଇଁ (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      ଯଦି (UNLIKELY (ret < 0))                                           \
        ଫେରନ୍ତୁ crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  ଯଦି (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  ଯଦି (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  ଯଦି (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  ଯଦି (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
copy_from_fd_to_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା consume, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ssize_t nread;
  size_t pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  କର
    {
      cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      ଯଦି (can_copy_file_range)
        {
          nread = copy_file_range (src, ଖାଲି, dst, ଖାଲି, pagesize, 0);
          ଯଦି (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              ଯାଅ fallback;
            }
          ଯଦି (consume && nread < 0 && errno == EAGAIN)
            ଫେରନ୍ତୁ 0;
          ଯଦି (nread < 0 && errno == EIO)
            ଫେରନ୍ତୁ 0;
          ଯଦି (UNLIKELY (nread < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "copy_file_range");
          ଜାରି;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (ପଠ (src, buffer, pagesize));
      ଯଦି (consume && nread < 0 && errno == EAGAIN)
        ଫେରନ୍ତୁ 0;
      ଯଦି (nread < 0 && errno == EIO)
        ଫେରନ୍ତୁ 0;
      ଯଦି (UNLIKELY (nread < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "read");

      remaining = nread;
      ଯେତେବେଳେ (remaining)
        {
          ret = TEMP_FAILURE_RETRY (ଲିଖ (dst, buffer + nread - remaining, remaining));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } ଯେତେବେଳେ (consume && nread);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
run_process (ଅକ୍ଷର **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  ଯଦି (UNLIKELY (pid < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fork");
  ଯଦି (pid)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      ଯଦି (r < 0)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "waitpid");
      ଯଦି (WIFEXITED (status) || WIFSIGNALED (status))
        ଫେରନ୍ତୁ WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
ସ୍ଥିର unsigned
atou (ଅକ୍ଷର **s)
{
  unsigned x;
  ପାଇଁ (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  ଫେରନ୍ତୁ x;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
fgetpwent_r (FILE *f, ସଂରଚନା passwd *pw, ଅକ୍ଷର *ଧାଡ଼ି, size_t size, ସଂରଚନା passwd **res)
{
  ଅକ୍ଷର *s;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rv = 0;
  ପାଇଁ (;;)
    {
      ଧାଡ଼ି[size - 1] = '\xff';
      ଯଦି ((fgets (ଧାଡ଼ି, size, f) == ଖାଲି) || ferror (f) || ଧାଡ଼ି[size - 1] != '\xff')
        {
          rv = (ଧାଡ଼ି[size - 1] != '\xff') ? ERANGE : ENOENT;
          ଧାଡ଼ି = 0;
          pw = 0;
          ଭାଙ୍ଗ;
        }
      ଧାଡ଼ି[strcspn (ଧାଡ଼ି, "\n")] = 0;

      s = ଧାଡ଼ି;
      pw->pw_name = s++;
      ଯଦି (! (s = strchr (s, ':')))
        ଜାରି;

      *s++ = 0;
      pw->pw_passwd = s;
      ଯଦି (! (s = strchr (s, ':')))
        ଜାରି;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      ଯଦି (*s != ':')
        ଜାରି;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      ଯଦି (*s != ':')
        ଜାରି;

      *s++ = 0;
      pw->pw_gecos = s;
      ଯଦି (! (s = strchr (s, ':')))
        ଜାରି;

      *s++ = 0;
      pw->pw_dir = s;
      ଯଦି (! (s = strchr (s, ':')))
        ଜାରି;

      *s++ = 0;
      pw->pw_shell = s;
      ଭାଙ୍ଗ;
    }
  *res = pw;
  ଯଦି (rv)
    errno = rv;
  ଫେରନ୍ତୁ rv;
}
#endif

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
set_home_env (uid_t id)
{
  ସଂରଚନା passwd pwd;
  cleanup_free ଅକ୍ଷର *buf = ଖାଲି;
  long buf_size;
  cleanup_file FILE *stream = ଖାଲି;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  ଯଦି (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  ଯଦି (stream == ଖାଲି)
    ଫେରନ୍ତୁ -1;

  ପାଇଁ (;;)
    {
      ସଂରଚନା passwd *ret_pw = ଖାଲି;

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      ଯଦି (UNLIKELY (ret != 0))
        {
          ଯଦି (ret == EINTR)
            ଜାରି;

          ଯଦି (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              ଫେରନ୍ତୁ -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          ଜାରି;
        }

      ଯଦି (ret_pw && ret_pw->pw_uid == id)
        {
          ଯଦି (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          ଅନ୍ୟଥା
            ଫେରନ୍ତୁ 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
getsubidrange (uid_t id, ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *ନିବେଶ = ଖାଲି;
  cleanup_free ଅକ୍ଷର *lineptr = ଖାଲି;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free ଅକ୍ଷର *buf = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *name;
  ସଂରଚନା passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  ଯଦି (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  ପାଇଁ (;;)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
      ସଂରଚନା passwd *ret_pw = ଖାଲି;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      ଯଦି (LIKELY (ret == 0))
        {
          ଯଦି (ret_pw)
            {
              name = ret_pw->pw_name;
              ଭାଙ୍ଗ;
            }
          ଫେରନ୍ତୁ -1;
        }

      ଯଦି (ret != ERANGE)
        ଫେରନ୍ତୁ -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  ନିବେଶ = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  ଯଦି (ନିବେଶ == ଖାଲି)
    ଫେରନ୍ତୁ -1;

  ପାଇଁ (;;)
    {
      ଅକ୍ଷର *endptr;
      ssize_t ପଠ = getline (&lineptr, &lenlineptr, ନିବେଶ);
      ଯଦି (ପଠ < 0)
        ଫେରନ୍ତୁ -1;

      ଯଦି (ପଠ < (ssize_t) (len_name + 2))
        ଜାରି;

      ଯଦି (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        ଜାରି;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      ଯଦି (endptr >= &lineptr[ପଠ])
        ଫେରନ୍ତୁ -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      ଫେରନ୍ତୁ 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
format_default_id_mapping (ଅକ୍ଷର **out, uid_t container_id, uid_t host_uid, uid_t host_id, ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା written = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, remaining;

  *out = ଖାଲି;

  ଯଦି (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    ଫେରନ୍ତୁ 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  ଯଦି (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      ଯଦି (UNLIKELY (ret >= remaining))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  ଯଦି (UNLIKELY (ret >= remaining))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  ଯଦି (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      ଯଦି (UNLIKELY (ret >= remaining))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = ଖାଲି;
  ଫେରନ୍ତୁ written;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
unset_cloexec_flag (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = fcntl (fd, F_GETFD);
  ଯଦି (flags == -1)
    ଫେରନ୍ତୁ -1;

  flags &= ~FD_CLOEXEC;

  ଫେରନ୍ତୁ fcntl (fd, F_SETFD, flags);
}

ସ୍ଥିର ଶୂନ୍ୟ __attribute__ ((__noreturn__))
run_process_child (ଅକ୍ଷର *path, ଅକ୍ଷର **args, ଅଚଳ ଅକ୍ଷର *cwd, ଅକ୍ଷର **envp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipe_r,
                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipe_w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା err_fd, bool can_ignore_chdir_errors)
{
  ଅକ୍ଷର *tmp_args[] = { path, ଖାଲି };
  libcrun_error_t err = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dev_null_fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = mark_or_close_fds_ge_than (ଖାଲି, 3, false, &err);
  ଯଦି (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->ସନ୍ଦେଶ);

  ଯଦି (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      ଯଦି (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  ଯଦି (out_fd >= 0)
    unset_cloexec_flag (1);
  ଯଦି (err_fd >= 0)
    unset_cloexec_flag (2);

  ଯଦି (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  ଯଦି (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  ଯଦି (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  ଯଦି (args == ଖାଲି)
    args = tmp_args;

  ଯଦି (cwd && chdir (cwd) < 0)
    {
      ଯଦି (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
run_process_with_stdin_timeout_envp (ଅକ୍ଷର *path, ଅକ୍ଷର **args, ଅଚଳ ଅକ୍ଷର *cwd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା timeout,
                                     ଅକ୍ଷର **envp, ଅକ୍ଷର *stdin, size_t stdin_len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_fd,
                                     ପୂର୍ଣ୍ଣସଂଖ୍ୟା err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdin_pipe[2];
  pid_t pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipe_r = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipe_w = -1;
  sigset_t oldmask, mask;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  ଯଦି (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  ଯଦି (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      ଯାଅ restore_sig_mask_and_exit;
    }

  ଯଦି (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (ଲିଖ (pipe_w, stdin, stdin_len));
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      ଯଦି (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          ଯାଅ restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  ଯଦି (timeout)
    {
      time_t start = time (ଖାଲି);
      time_t now;
      ପାଇଁ (now = start; now - start < timeout; now = time (ଖାଲି))
        {
          siginfo_t info;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା elapsed = now - start;
          ସଂରଚନା timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          ଯଦି (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              ଯାଅ restore_sig_mask_and_exit;
            }

          ଯଦି (info.si_signo == SIGCHLD && info.si_pid == pid)
            ଯାଅ read_waitpid;

          ଯଦି (ret < 0 && errno == EAGAIN)
            ଯାଅ timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      ଯାଅ restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  ଯଦି (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  ଅନ୍ୟଥା
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  ଯଦି (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      ଯଦି (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, ଖାଲି);
      ଯଦି (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
mark_or_close_fds_ge_than (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, bool close_now, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା cfd = -1;
  cleanup_dir DIR *dir = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
  ସଂରଚନା dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  ଯଦି (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  ଯଦି (ret == 0)
    {
      ଯଦି (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      ଫେରନ୍ତୁ 0;
    }
  ଯଦି (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  ଯଦି (UNLIKELY (cfd < 0))
    ଫେରନ୍ତୁ cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  dir = fdopendir (cfd);
  ଯଦି (UNLIKELY (dir == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  ପାଇଁ (next = readdir (dir); next; next = readdir (dir))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା val;
      ଅଚଳ ଅକ୍ଷର *name = next->d_name;
      ଯଦି (name[0] == '.')
        ଜାରି;

      val = strtoll (name, ଖାଲି, 10);
      ଯଦି (val < n || val == fd)
        ଜାରି;

      ଯଦି (close_now)
        {
          ret = close (val);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "close fd `%d`", val);

          ଯଦି (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      ଅନ୍ୟଥା
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ
get_current_timestamp (ଅକ୍ଷର *out, size_t len)
{
  ସଂରଚନା timeval tv;
  ସଂରଚନା tm now;
  ଅକ୍ଷର timestamp[64];

  gettimeofday (&tv, ଖାଲି);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, ଆକାର (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (ଶୂନ୍ୟ) snprintf (out, len, "%s.%06lldZ", timestamp, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) tv.tv_usec);
  out[len - 1] = '\0';
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
set_blocking_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, bool blocking, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, flags = fcntl (fd, F_GETFL, 0);
  ଯଦି (UNLIKELY (flags < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fcntl");
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
parse_json_file (json_object **out, ଅଚଳ ଅକ୍ଷର *jsondata, ସଂରଚନା parser_context *ctx arg_unused, libcrun_error_t *err)
{
  ସଂରଚନା json_tokener *tok;
  ଗଣନା json_tokener_error jerr;

  *err = ଖାଲି;

  ଯଦି (UNLIKELY (jsondata == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  ଯଦି (UNLIKELY (tok == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  ଯଦି (*out == ଖାଲି || jerr != json_tokener_success)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_pos = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) tok->char_offset;

      json_tokener_free (tok);
      ଯଦି (*out)
        {
          json_object_put (*out);
          *out = ଖାଲି;
        }
      ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
check_access (ଅଚଳ ଅକ୍ଷର *path)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#ଅନ୍ୟଥା
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  ଯଦି (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      ଯଦି (errno == EACCES)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          ଯଦି (ret == 0)
            ଫେରନ୍ତୁ CHECK_ACCESS_NOT_EXECUTABLE;
        }
      ଫେରନ୍ତୁ -errno;
    }

  ret = get_file_type (&mode, false, path);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ -errno;

  ଯଦି (! S_ISREG (mode))
    ଫେରନ୍ତୁ CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
find_executable (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *executable_path, ଅଚଳ ଅକ୍ଷର *cwd, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cwd_executable_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *tmp = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_error = -ENOENT;
  ଅକ୍ଷର *it, *end;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  *out = ଖାଲି;

  ଯଦି (executable_path == ଖାଲି || executable_path[0] == '\0')
    ଫେରନ୍ତୁ crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  ଯଦି (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free ଅକ୍ଷର *cwd_allocated = ଖାଲି;

      ଯଦି (cwd == ଖାଲି)
        {
          cwd_allocated = getcwd (ଖାଲି, 0);
          ଯଦି (cwd_allocated == ଖାଲି)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  ଯଦି (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      ଯଦି (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          ଫେରନ୍ତୁ 0;
        }
      last_error = ret;
      ଯାଅ fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  ଯେତେବେଳେ ((it = strsep (&end, ":")))
    {
      cleanup_free ଅକ୍ଷର *path = ଖାଲି;

      ଯଦି (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ଜାରି;
        }

      ret = check_access (path);
      ଯଦି (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = ଖାଲି;
          ଫେରନ୍ତୁ 0;
        }

      ଯଦି (ret == -ENOENT)
        ଜାରି;

      last_error = ret;
    }

fail:
  ବାଛ (last_error)
    {
    ସ୍ଥିତି CHECK_ACCESS_NOT_EXECUTABLE:
      ଫେରନ୍ତୁ crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    ସ୍ଥିତି CHECK_ACCESS_NOT_REGULAR:
      ଫେରନ୍ତୁ crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      errno = -last_error;
      ଯଦି (errno == ENOENT)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

ସ୍ଥିର ssize_t
safe_read_xattr (ଅକ୍ଷର **ret, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sfd, ଅଚଳ ଅକ୍ଷର *srcname, ଅଚଳ ଅକ୍ଷର *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  ଯେତେବେଳେ (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      ଯଦି (UNLIKELY (s < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      ଯଦି (s < current_size)
        ଭାଙ୍ଗ;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  ଯଦି (s <= 0)
    ଫେରନ୍ତୁ s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = ଖାଲି;

  ଫେରନ୍ତୁ s;
}

ସ୍ଥିର ssize_t
copy_xattr (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd, ଅଚଳ ଅକ୍ଷର *srcname, ଅଚଳ ଅକ୍ଷର *destname, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *buf = ଖାଲି;
  ssize_t xattr_len;
  ଅକ୍ଷର *it;

  xattr_len = flistxattr (sfd, ଖାଲି, 0);
  ଯଦି (UNLIKELY (xattr_len < 0))
    {
      ଯଦି (errno == ENOTSUP)
        ଫେରନ୍ତୁ 0;

      ଫେରନ୍ତୁ crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  ଯଦି (xattr_len == 0)
    ଫେରନ୍ତୁ 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  ଯଦି (UNLIKELY (xattr_len < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "flistxattr `%s`", srcname);

  ପାଇଁ (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free ଅକ୍ଷର *v = ଖାଲି;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      ଯଦି (UNLIKELY (s < 0))
        ଫେରନ୍ତୁ s;

      s = fsetxattr (dfd, it, v, s, 0);
      ଯଦି (UNLIKELY (s < 0))
        {
          ଯଦି (errno == EINVAL || errno == EOPNOTSUPP)
            ଜାରି;

          ଫେରନ୍ତୁ crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  ଫେରନ୍ତୁ 0;
}

#endif

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
copy_rec_stat_file_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd, ଅଚଳ ଅକ୍ଷର *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  ସଂରଚନା stat st;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

#ifdef HAVE_STATX
  ସଂରଚନା statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (errno == ENOSYS || errno == EINVAL)
        ଯାଅ fallback;

      ଫେରନ୍ତୁ ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  ଫେରନ୍ତୁ ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
copy_recursive_fd_to_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcdirfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd, ଅଚଳ ଅକ୍ଷର *srcname, ଅଚଳ ଅକ୍ଷର *destname, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = ଖାଲି;
  ସଂରଚନା dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  ଯଦି (UNLIKELY (dsrcfd == ଖାଲି))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      ଫେରନ୍ତୁ crun_make_error (err, errno, "open directory `%s`", destname);
    }

  ପାଇଁ (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcfd = -1;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା destfd = -1;
      cleanup_free ଅକ୍ଷର *target_buf = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      ଯଦି (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ଜାରି;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      ବାଛ (mode & S_IFMT)
        {
        ସ୍ଥିତି S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          ଯଦି (UNLIKELY (srcfd < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          ଯଦି (UNLIKELY (destfd < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

#ifdef HAVE_FGETXATTR
          ret = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ଯଦି (UNLIKELY (srcfd < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ଯଦି (UNLIKELY (destfd < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି S_IFBLK:
        ସ୍ଥିତି S_IFCHR:
        ସ୍ଥିତି S_IFIFO:
        ସ୍ଥିତି S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          ଭାଙ୍ଗ;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      ଯଦି (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          ଯଦି (errno == ENOTSUP)
            ଜାରି;

          ଫେରନ୍ତୁ crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  ଫେରନ୍ତୁ 0;
}

ଅଚଳ ଅକ୍ଷର *
find_annotation (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *name)
{
  ଯଦି (container->container_def->annotations == ଖାଲି)
    ଫେରନ୍ତୁ ଖାଲି;

  ଫେରନ୍ତୁ find_string_map_ମୂଲ୍ୟ (container->annotations, name);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
safe_write (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *fname, ଅଚଳ ଶୂନ୍ୟ *buf, size_t ଗଣନା_2, libcrun_error_t *err)
{
  size_t written = 0;
  ଯେତେବେଳେ (written < ଗଣନା_2)
    {
      ssize_t w = ଲିଖ (fd, (ଅଚଳ ଅକ୍ଷର *) buf + written, ଗଣନା_2 - written);
      ଯଦି (UNLIKELY (w < 0))
        {
          ଯଦି (errno == EINTR || errno == EAGAIN)
            ଜାରି;
          ଫେରନ୍ତୁ crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  ଫେରନ୍ତୁ 0;
}

#define MAX_PARTS 32

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
append_paths (ଅକ୍ଷର **out, libcrun_error_t *err, ...)
{
  ଅଚଳ ଅକ୍ଷର *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t total_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  ପାଇଁ (;;)
    {
      ଅଚଳ ଅକ୍ଷର *part;
      size_t size;

      part = va_arg (ap, ଅଚଳ ଅକ୍ଷର *);
      ଯଦି (part == ଖାଲି)
        ଭାଙ୍ଗ;

      ଯଦି (n_parts == MAX_PARTS)
        {
          va_end (ap);
          ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "too many paths specified");
        }

      ଯଦି (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          ଯେତେବେଳେ (part[0] == '/' && part[1] == '/')
            part++;
        }
      ଅନ୍ୟଥା
        {
          /* And drop any initial '/' for other components.  */
          ଯେତେବେଳେ (part[0] == '/')
            part++;
        }

      size = strlen (part);
      ଯଦି (size == 0)
        ଜାରି;

      ଯେତେବେଳେ (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  ପାଇଁ (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  ପାଇଁ (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      ଯଦି (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  ଫେରନ୍ତୁ 0;
}

#ଯଦି __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#ଅନ୍ୟଥା
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
b64_ନିବେଶ (ଅକ୍ଷର c)
{
  ଅଚଳ ଅକ୍ଷର table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ପାଇଁ (i = 0; i < 64; i++)
    {
      ଯଦି (table[i] == c)
        ଫେରନ୍ତୁ i;
    }
  ଫେରନ୍ତୁ -1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
base64_decode (ଅଚଳ ଅକ୍ଷର *iptr, size_t isize, ଅକ୍ଷର *optr, size_t osize, size_t *nbytes)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned ଅକ୍ଷର data[4];

  *nbytes = 0;
  ଯେତେବେଳେ (consumed < isize && (*nbytes) + 3 < osize)
    {
      ଯେତେବେଳେ ((i < 4) && (consumed < isize))
        {
          tmp = b64_ନିବେଶ (*iptr++);
          consumed++;
          ଯଦି (tmp != -1)
            {
              ଯଦି (UNLIKELY (pad > 0))
                ଫେରନ୍ତୁ -1;
              data[i++] = tmp;
            }
          ଅନ୍ୟଥା ଯଦି (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      ଯଦି (i == 4)
        {
          ଯଦି (UNLIKELY (pad > 2))
            ଫେରନ୍ତୁ -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      ଅନ୍ୟଥା
        {
          /* I did not get all the data.  */
          consumed -= i;
          ଫେରନ୍ତୁ consumed;
        }
      i = 0;
    }
  ଫେରନ୍ତୁ consumed;
}

ଅକ୍ଷର *
get_user_name (uid_t uid)
{
  ସଂରଚନା passwd pd;
  ସଂରଚନା passwd *temp_result_ptr;
  ଅକ୍ଷର pwdbuffer[200];
  ଯଦି (! getpwuid_r (uid, &pd, pwdbuffer, ଆକାର (pwdbuffer), &temp_result_ptr))
    ଫେରନ୍ତୁ xstrdup (pd.pw_name);
  ଫେରନ୍ତୁ xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
ଅକ୍ଷର *
libcrun_sysctl_ଚାବି_to_proc_path (ଅଚଳ ଅକ୍ଷର *ଚାବି)
{
  ଅଚଳ ଅକ୍ଷର *sep;
  ଅକ୍ଷର *ret, *it;

  sep = strpbrk (ଚାବି, "./");

  /* No separator at all, or the key is already in the path form.  */
  ଯଦି (sep == ଖାଲି || *sep == '/')
    ଫେରନ୍ତୁ xstrdup (ଚାବି);

  ret = xstrdup (ଚାବି);
  ପାଇଁ (it = ret; *it; it++)
    {
      ଯଦି (*it == '.')
        *it = '/';
      ଅନ୍ୟଥା ଯଦି (*it == '/')
        *it = '.';
    }

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
has_suffix (ଅଚଳ ଅକ୍ଷର *str, ଅଚଳ ଅକ୍ଷର *suffix)
{
  ଯଦି (! str || ! suffix)
    ଫେରନ୍ତୁ 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  ଯଦି (lensuffix > lenstr)
    ଫେରନ୍ତୁ 0;
  ଫେରନ୍ତୁ memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

ଅକ୍ଷର *
str_join_array (ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset, size_t size, ଅକ୍ଷର *ଅଚଳ array[], ଅଚଳ ଅକ୍ଷର *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = ଖାଲି;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  ଅକ୍ଷର *result, *p;

  lens = xmalloc (size * ଆକାର (*lens));

  ପାଇଁ (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  ପାଇଁ (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      ଯଦି (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  ଫେରନ୍ତୁ result;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_mmap (ସଂରଚନା libcrun_mmap_s **ret, ଶୂନ୍ୟ *addr, size_t length,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା prot, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, off_t offset,
              libcrun_error_t *err)
{
  ସଂରଚନା libcrun_mmap_s *mmap_s = ଖାଲି;

  ଶୂନ୍ୟ *mapped = mmap (addr, length, prot, flags, fd, offset);
  ଯଦି (mapped == MAP_FAILED)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (ଆକାର (ସଂରଚନା libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_munmap (ସଂରଚନା libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = munmap (mmap->addr, mmap->length);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "munmap");

  ମୁକ୍ତ (mmap);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର long
read_file_as_long_or_default (ଅଚଳ ଅକ୍ଷର *path, long def_ମୂଲ୍ୟ)
{
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  libcrun_error_t tmp_err = ଖାଲି;
  ଅକ୍ଷର *endptr;
  long val;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = read_all_file (path, &content, ଖାଲି, &tmp_err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ଫେରନ୍ତୁ def_ମୂଲ୍ୟ;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  ଯଦି (UNLIKELY (errno))
    ଫେରନ୍ତୁ def_ମୂଲ୍ୟ;
  ଯଦି (endptr == content || (*endptr && *endptr != '\n'))
    ଫେରନ୍ତୁ def_ମୂଲ୍ୟ;
  ଫେରନ୍ତୁ val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (ଶୂନ୍ୟ)
{
  ସ୍ଥିର atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  ଯଦି (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  ଫେରନ୍ତୁ uid;
}

gid_t
get_overflow_gid (ଶୂନ୍ୟ)
{
  ସ୍ଥିର atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  ଯଦି (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  ଫେରନ୍ତୁ gid;
}

ଶୂନ୍ୟ
consume_trailing_slashes (ଅକ୍ଷର *path)
{
  ଯଦି (! path || path[0] == '\0')
    ଫେରନ୍ତୁ;

  ଅକ୍ଷର *last = path + strlen (path);

  ଯେତେବେଳେ (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

ଅକ୍ଷର **
read_dir_entries (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = ଖାଲି;
  size_t n_entries = 0;
  size_t entries_size = 16;
  ଅକ୍ଷର **entries = ଖାଲି;
  ସଂରଚନା dirent *de;

  dir = opendir (path);
  ଯଦି (UNLIKELY (dir == ଖାଲି))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      ଫେରନ୍ତୁ ଖାଲି;
    }

  entries = xmalloc (entries_size * ଆକାର (ଅକ୍ଷର *));
  ଯେତେବେଳେ ((de = readdir (dir)))
    {
      ଯଦି (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ଜାରି;
      ଯଦି (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * ଆକାର (ଅକ୍ଷର *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * ଆକାର (ଅକ୍ଷର *));
  entries[n_entries] = ଖାଲି;

  ଫେରନ୍ତୁ entries;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cpuset_string_to_bitmask (ଅଚଳ ଅକ୍ଷର *str, ଅକ୍ଷର **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *mask = ଖାଲି;
  size_t mask_size = 0;
  ଅଚଳ ଅକ୍ଷର *p = str;
  ଅକ୍ଷର *endptr;

  ଯେତେବେଳେ (*p)
    {
      long long start_range, end_range;

      ଯଦି (*p < '0' || *p > '9')
        ଯାଅ invalid_ନିବେଶ;

      start_range = strtoll (p, &endptr, 10);
      ଯଦି (start_range < 0)
        ଯାଅ invalid_ନିବେଶ;

      p = endptr;

      ଯଦି (*p != '-')
        end_range = start_range;
      ଅନ୍ୟଥା
        {
          p++;

          ଯଦି (*p < '0' || *p > '9')
            ଯାଅ invalid_ନିବେଶ;

          end_range = strtoll (p, &endptr, 10);

          ଯଦି (end_range < start_range)
            ଯାଅ invalid_ନିବେଶ;

          p = endptr;
        }

      /* Just set some limit.  */
      ଯଦି (end_range > (1 << 20))
        ଯାଅ invalid_ନିବେଶ;

      ଯଦି (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      ପାଇଁ (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      ଯଦି (*p == ',')
        p++;
      ଅନ୍ୟଥା ଯଦି (*p)
        ଯାଅ invalid_ନିବେଶ;
    }

  *out = mask;
  mask = ଖାଲି;
  *out_size = mask_size;

  ଫେରନ୍ତୁ 0;

invalid_ନିବେଶ:
  ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot parse input `%s`", str);
}

ସଂରଚନା channel_fd_pair
{
  ସଂରଚନା ring_buffer *rb;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_fd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_fd;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା infd_epoll_events;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା outfd_epoll_events;
};

ସଂରଚନା channel_fd_pair *
channel_fd_pair_new (ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_fd, size_t size)
{
  ସଂରଚନା channel_fd_pair *channel = xmalloc (ଆକାର (ସଂରଚନା channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  ଫେରନ୍ତୁ channel;
}

ଶୂନ୍ୟ
channel_fd_pair_free (ସଂରଚନା channel_fd_pair *channel)
{
  ଯଦି (channel == ଖାଲି)
    ଫେରନ୍ତୁ;

  ring_buffer_free (channel->rb);
  ମୁକ୍ତ (channel);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
channel_fd_pair_process (ସଂରଚନା channel_fd_pair *channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା epollfd, libcrun_error_t *err)
{
  bool is_ନିବେଶ_eagain = false, is_output_eagain = false, repeat;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  ପାଇଁ (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      ଯଦି (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_ନିବେଶ_eagain, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          ଯଦି (ret > 0)
            repeat = true;
        }
      ଯଦି (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          ଯଦି (ret > 0)
            repeat = true;
        }
    }

  ଯଦି (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_ନିବେଶ_eagain ? EPOLLET : 0)) : 0;
      ଯଦି (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      ଯଦି (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          channel->outfd_epoll_events = events;
        }
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_proc (libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsfd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  ଯଦି (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ଖାଲି, ଖାଲି, 0);
      ଯଦି (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          ଯଦି (fd >= 0)
            ଫେରନ୍ତୁ fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  ଯଦି (fd < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `/proc`");
  ଫେରନ୍ତୁ fd;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  ଯଦି (! container)
    ଫେରନ୍ତୁ open_proc (err);

  ଯଦି (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  ଫେରନ୍ତୁ container->proc_fd;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_open_proc_file (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  ଯଦି (proc_fd < 0)
    ଫେରନ୍ତୁ proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  ଯଦି (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  ଫେରନ୍ତୁ fd;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *full_path = ଖାଲି;
  xasprintf (&full_path, "%d/%s", pid, path);
  ଫେରନ୍ତୁ libcrun_open_proc_file (container, full_path, flags, err);
}
