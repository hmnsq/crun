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
#অন্তর্ভাব <config.h>
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "ring_buffer.h"
#অন্তর্ভাব <stdarg.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <libgen.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/un.h>
#অন্তর্ভাব <sys/socket.h>
#অন্তর্ভাব <sys/signalfd.h>
#অন্তর্ভাব <sys/epoll.h>
#অন্তর্ভাব <sys/syscall.h>
#অন্তর্ভাব <sys/wait.h>
#অন্তর্ভাব <pwd.h>
#অন্তর্ভাব <grp.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <sys/time.h>
#অন্তর্ভাব <sys/xattr.h>
#অন্তর্ভাব <sys/sysmacros.h>
#অন্তর্ভাব <sys/vfs.h>
#অন্তর্ভাব <linux/magic.h>
#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <sys/mman.h>
#ifdef HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#endif
#যদি HAVE_STDATOMIC_H
#  include <stdatomic.h>
#অন্যথা
#  define atomic_long অস্থির long
#  define atomic_int অস্থির পূর্ণসংখ্যা
#endif
#অন্তর্ভাব "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_মূল
#  define RESOLVE_IN_মূল 0x10
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

পূর্ণসংখ্যা
crun_path_exists (ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret = access (path, F_OK);
  যদি (ret < 0)
    {
      যদি (errno == ENOENT)
        ফেরত 0;
      ফেরত crun_make_error (err, errno, "access `%s`", path);
    }
  ফেরত 1;
}

পূর্ণসংখ্যা
xasprintf (অক্ষর **str, ধ্রুবক অক্ষর *fmt, ...)
{
  পূর্ণসংখ্যা ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  যদি (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  ফেরত ret;
}

পূর্ণসংখ্যা
write_file_at_with_flags (পূর্ণসংখ্যা dirfd, পূর্ণসংখ্যা flags, mode_t mode, ধ্রুবক অক্ষর *name, ধ্রুবক শূন্য *তথ্য, size_t len, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা fd = -1;
  পূর্ণসংখ্যা ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, তথ্য, len, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত (len > INT_MAX) ? INT_MAX : (পূর্ণসংখ্যা) len;
}

পূর্ণসংখ্যা
detach_process ()
{
  pid_t pid;
  যদি (setsid () < 0)
    ফেরত -1;
  pid = fork ();
  যদি (pid < 0)
    ফেরত -1;
  যদি (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  ফেরত 0;
}

পূর্ণসংখ্যা
get_file_type_fd (পূর্ণসংখ্যা fd, mode_t *mode)
{
  কাঠামো stat st;
  পূর্ণসংখ্যা ret;

#ifdef HAVE_STATX
  কাঠামো statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOSYS || errno == EINVAL)
        যাও fallback;

      ফেরত ret;
    }
  *mode = stx.stx_mode;
  ফেরত ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  ফেরত ret;
}

পূর্ণসংখ্যা
get_file_type_at (পূর্ণসংখ্যা dirfd, mode_t *mode, bool nofollow, ধ্রুবক অক্ষর *path)
{
  পূর্ণসংখ্যা empty_path = path == ফাঁকা ? AT_EMPTY_PATH : 0;
  কাঠামো stat st;
  পূর্ণসংখ্যা ret;

#ifdef HAVE_STATX
  কাঠামো statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOSYS || errno == EINVAL)
        যাও fallback;

      ফেরত ret;
    }
  *mode = stx.stx_mode;
  ফেরত ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  ফেরত ret;
}

পূর্ণসংখ্যা
get_file_type (mode_t *mode, bool nofollow, ধ্রুবক অক্ষর *path)
{
  ফেরত get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

পূর্ণসংখ্যা
create_file_if_missing_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  যদি (fd_write < 0)
    {
      পূর্ণসংখ্যা saved_errno = errno;
      mode_t tmp_mode;
      পূর্ণসংখ্যা ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      যদি (ret == 0 && S_ISREG (tmp_mode))
        ফেরত 0;

      ফেরত crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
ensure_directory_internal_at (পূর্ণসংখ্যা dirfd, অক্ষর *path, size_t len, পূর্ণসংখ্যা mode, libcrun_error_t *err)
{
  অক্ষর *it = path + len;
  পূর্ণসংখ্যা ret = 0;
  bool parent_created = false;

  জন্য (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      যদি (ret == 0 || errno == EEXIST)
        ফেরত 0;

      পূর্ণসংখ্যা saved_errno = errno;
      যদি (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = ফাঁকা;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          যদি (ret > 0)
            ফেরত 0;
          যদি (ret < 0)
            crun_error_release (&tmp_err);

          ফেরত crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      যখন (it > path && *it != '/')
        {
          it--;
          len--;
        }
      যদি (it == path)
        ফেরত 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      parent_created = true;
    }
  ফেরত ret;
}

পূর্ণসংখ্যা
crun_ensure_directory_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode, bool nofollow, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  cleanup_free অক্ষর *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (ret == 0)
    ফেরত crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
check_fd_is_path (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  অক্ষর link[PATH_MAX];
  পূর্ণসংখ্যা ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = অস্থায়ী_FAILURE_RETRY (readlink (fdpath, link, আকার (link)));
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "readlink `%s`", fdname);

  যদি (((size_t) ret) != path_len || memcmp (link, path, path_len))
    ফেরত crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
check_fd_under_path (ধ্রুবক অক্ষর *rootfs, size_t rootfslen, পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  অক্ষর link[PATH_MAX];
  পূর্ণসংখ্যা ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  যদি (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    ফেরত 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = অস্থায়ী_FAILURE_RETRY (readlink (fdpath, link, আকার (link)));
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "readlink `%s`", fdname);

  যদি (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    ফেরত crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  ফেরত 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
স্থির শূন্য
close_and_replace (পূর্ণসংখ্যা *oldfd, পূর্ণসংখ্যা newfd)
{
  যদি (*oldfd >= 0)
    অস্থায়ী_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
অক্ষর *chroot_realpath (ধ্রুবক অক্ষর *chroot, ধ্রুবক অক্ষর *path, অক্ষর resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
স্থির পূর্ণসংখ্যা
safe_openat_fallback (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *rootfs, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা flags,
                      পূর্ণসংখ্যা mode, libcrun_error_t *err)
{
  cleanup_free অক্ষর *parent_path = ফাঁকা;
  ধ্রুবক অক্ষর *last_component = ফাঁকা;
  ধ্রুবক অক্ষর *orig_path = path;
  ধ্রুবক অক্ষর *path_in_chroot;
  cleanup_close পূর্ণসংখ্যা fd = -1;
  অক্ষর resolved[PATH_MAX];
  অক্ষর বাফার[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  পূর্ণসংখ্যা ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  যদি (flags & O_NOFOLLOW)
    {
      অক্ষর *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      যদি (sep == ফাঁকা)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      অন্যথা যদি (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      যদি (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, বাফার);
  যদি (path_in_chroot == ফাঁকা)
    ফেরত crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  যদি (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  যদি (last_component)
    {
      ret = snprintf (resolved, আকার (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      যদি (UNLIKELY (ret >= (পূর্ণসংখ্যা) আকার (resolved)))
        ফেরত crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  যদি (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      ফেরত ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = fd;
  fd = -1;
  ফেরত ret;
}

পূর্ণসংখ্যা
safe_openat (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *rootfs, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা mode,
             libcrun_error_t *err)
{
  স্থির bool openat2_supported = true;
  পূর্ণসংখ্যা retries = MAX_OPENAT2_EAGAIN_RETRIES;
  পূর্ণসংখ্যা ret;

  যদি (is_empty_string (path))
    {
      cleanup_close পূর্ণসংখ্যা fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      যদি (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      যদি (UNLIKELY (fd < 0))
        ফেরত crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      যদি (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
        }

      ret = fd;
      fd = -1;
      ফেরত ret;
    }

  যদি (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_মূল);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (errno == EINTR)
            যাও repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          যদি (errno == EAGAIN && retries-- > 0)
            যাও repeat;
          যদি (errno == ENOSYS)
            openat2_supported = false;
          যদি (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            ফেরত safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          ফেরত crun_make_error (err, errno, "openat2 `%s`", path);
        }

      ফেরত ret;
    }

  ফেরত safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (পূর্ণসংখ্যা dfd, ধ্রুবক অক্ষর *name, অক্ষর **বাফার, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_আকার = hint > 0 ? hint + 1 : 512;
  cleanup_free অক্ষর *tmp_buf = ফাঁকা;
  ssize_t আকার_2;

  করো
    {
      যদি (tmp_buf != ফাঁকা)
        buf_আকার += 256;

      tmp_buf = xrealloc (tmp_buf, buf_আকার);

      আকার_2 = readlinkat (dfd, name, tmp_buf, buf_আকার);
      যদি (UNLIKELY (আকার_2 < 0))
        ফেরত crun_make_error (err, errno, "readlink `%s`", name);
  } যখন (আকার_2 == buf_আকার);

  /* Always NUL terminate the buffer.  */
  tmp_buf[আকার_2] = '\0';

  /* Move ownership to BUFFER.  */
  *বাফার = tmp_buf;
  tmp_buf = ফাঁকা;

  ফেরত আকার_2;
}

স্থির পূর্ণসংখ্যা crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *dirpath,
                                ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode, পূর্ণসংখ্যা max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
স্থির পূর্ণসংখ্যা
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *dirpath, পূর্ণসংখ্যা cwd,
                                 ধ্রুবক অক্ষর *npath, ধ্রুবক অক্ষর *cur, ধ্রুবক অক্ষর *rest,
                                 পূর্ণসংখ্যা mode, পূর্ণসংখ্যা max_readlinks, libcrun_error_t *err)
{
  cleanup_free অক্ষর *target = ফাঁকা;
  cleanup_free অক্ষর *new_path = ফাঁকা;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (পূর্ণসংখ্যা) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  ফেরত crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

স্থির পূর্ণসংখ্যা
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *dirpath,
                     ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode, পূর্ণসংখ্যা max_readlinks, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা wd_cleanup = -1;
  cleanup_free অক্ষর *npath = ফাঁকা;
  bool last_component = false;
  size_t depth = 0;
  অক্ষর *cur;
  অক্ষর *it;
  পূর্ণসংখ্যা cwd;
  পূর্ণসংখ্যা ret;

  যদি (max_readlinks <= 0)
    ফেরত crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  যদি (*path == '\0')
    {
      যদি (do_open)
        ফেরত open (dirpath, O_CLOEXEC | O_PATH, 0);
      ফেরত 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  যখন (cur)
    {
      যদি (it)
        *it = '\0';
      অন্যথা
        last_component = true;

      যদি (cur[0] == '\0')
        বিরতি;

      যদি (strcmp (cur, ".") == 0)
        যাও পরবর্তী;
      অন্যথা যদি (strcmp (cur, ".."))
        depth++;
      অন্যথা
        {
          যদি (depth)
            depth--;
          অন্যথা
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              যাও পরবর্তী;
            }
        }

      যদি (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          যদি (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              যদি (errno == ELOOP)
                ফেরত crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, ফাঁকা,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              যদি (ret < 0)
                ফেরত crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          যদি (do_open)
            ফেরত ret;

          close_and_replace (&wd_cleanup, ret);
          ফেরত 0;
        }

      ret = mkdirat (cwd, cur, mode);
      যদি (ret < 0)
        {
          mode_t st_mode;

          যদি (errno != EEXIST)
            ফেরত crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          যদি (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            ফেরত crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : ফাঁকা, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      যদি (UNLIKELY (cwd < 0))
        ফেরত crun_error_wrap (err, "creating `/%s`", path);

      যদি (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, ফাঁকা);
          যদি (UNLIKELY (ret < 0))
            {
              পূর্ণসংখ্যা saved_errno = errno;

              close (cwd);
              ফেরত crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          যদি ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              ফেরত crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    পরবর্তী:
      যদি (it == ফাঁকা)
        বিরতি;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  যদি (do_open)
    {
      যদি (cwd == dirfd)
        {
          ret = dup (dirfd);
          যদি (UNLIKELY (ret < 0))
            ফেরত crun_make_error (err, errno, "dup `%s`", dirpath);
          ফেরত ret;
        }

      wd_cleanup = -1;
      ফেরত cwd;
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
crun_safe_create_and_open_ref_at (bool dir, পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *dirpath, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  যদি (LIKELY (ret >= 0))
    ফেরত ret;

  crun_error_release (err);
  ফেরত crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

পূর্ণসংখ্যা
crun_safe_ensure_directory_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *dirpath, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode,
                               libcrun_error_t *err)
{
  ফেরত crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

পূর্ণসংখ্যা
crun_safe_ensure_file_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *dirpath, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode,
                          libcrun_error_t *err)
{
  ফেরত crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

পূর্ণসংখ্যা
crun_ensure_directory (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode, bool nofollow, libcrun_error_t *err)
{
  ফেরত crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

স্থির পূর্ণসংখ্যা
get_file_আকার (পূর্ণসংখ্যা fd, off_t *আকার_2)
{
  কাঠামো stat st;
  পূর্ণসংখ্যা ret;
#ifdef HAVE_STATX
  কাঠামো statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_আকার, &stx);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOSYS || errno == EINVAL)
        যাও fallback;
      ফেরত ret;
    }
  *আকার_2 = stx.stx_size;

  ফেরত ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *আকার_2 = st.st_size;
  ফেরত ret;
}

পূর্ণসংখ্যা
crun_dir_p_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  পূর্ণসংখ্যা ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "stat `%s`", path);

  ফেরত S_ISDIR (mode) ? 1 : 0;
}

পূর্ণসংখ্যা
crun_dir_p (ধ্রুবক অক্ষর *path, bool nofollow, libcrun_error_t *err)
{
  ফেরত crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
স্থির পূর্ণসংখ্যা
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free অক্ষর *বাফার = ফাঁকা;
  size_t len;
  পূর্ণসংখ্যা ret;

  ret = read_all_file ("/proc/self/uid_map", &বাফার, &len, err);
  যদি (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      যদি (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ফেরত 0;
        }
      ফেরত ret;
    }

  ফেরত strstr (বাফার, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
স্থির bool
running_in_openvz ()
{
  যদি (access ("/proc/vz", F_OK) < 0)
    ফেরত false;

  ফেরত access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

পূর্ণসংখ্যা
check_running_in_user_namespace (libcrun_error_t *err)
{
  স্থির atomic_int run_in_userns = -1;
  কাঠামো stat st;
  পূর্ণসংখ্যা ret;

  ret = run_in_userns;
  যদি (ret >= 0)
    ফেরত ret;

  ret = stat ("/proc/self/ns/user", &st);
  যদি (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      যদি (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          ফেরত run_in_userns;
        }

      যদি (! running_in_openvz ())
        {
          run_in_userns = 1;
          ফেরত run_in_userns;
        }
    }
  অন্যথা যদি (UNLIKELY (errno != ENOENT))
    ফেরত crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  run_in_userns = ret;
  ফেরত run_in_userns;
}

স্থির size_t
get_page_আকার ()
{
  স্থির atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  যদি (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  ফেরত (size_t) pagesize;
}

স্থির পূর্ণসংখ্যা selinux_enabled = -1;
স্থির পূর্ণসংখ্যা apparmor_enabled = -1;

পূর্ণসংখ্যা
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free অক্ষর *out = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা fd = -1;
  size_t len;
  পূর্ণসংখ্যা ret;

  যদি (selinux_enabled >= 0)
    ফেরত selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  যদি (UNLIKELY (fd < 0))
    ফেরত fd;

  ret = read_all_fd_with_আকার_hint (fd, "mounts", &out, &len, get_page_আকার (), err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  ফেরত selinux_enabled;
}

পূর্ণসংখ্যা
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা fd = -1;
  পূর্ণসংখ্যা আকার_2;
  অক্ষর buf[1];

  যদি (apparmor_enabled >= 0)
    ফেরত apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  যদি (fd == -1)
    {
      যদি (errno == ENOENT)
        {
          apparmor_enabled = 0;
          ফেরত 0;
        }

      ফেরত crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  আকার_2 = অস্থায়ী_FAILURE_RETRY (পঠ (fd, buf, আকার (buf)));

  apparmor_enabled = আকার_2 > 0 && buf[0] == 'Y' ? 1 : 0;

  ফেরত apparmor_enabled;
}

স্থির পূর্ণসংখ্যা
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  যদি (selinux_enabled < 0)
    ফেরত crun_make_error (err, 0, "SELinux is not initialized correctly");
  ফেরত selinux_enabled;
}

পূর্ণসংখ্যা
add_selinux_mount_label (অক্ষর **retlabel, ধ্রুবক অক্ষর *তথ্য, ধ্রুবক অক্ষর *label, ধ্রুবক অক্ষর *context_type, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  ret = libcrun_is_selinux_enabled (err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (label && ret)
    {
      যদি (তথ্য && *তথ্য)
        xasprintf (retlabel, "%s,%s=\"%s\"", তথ্য, context_type, label);
      অন্যথা
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      ফেরত 0;
    }
  *retlabel = xstrdup (তথ্য);
  ফেরত 0;
}

স্থির ধ্রুবক অক্ষর *
lsm_attr_path (libcrun_container_t *container, ধ্রুবক অক্ষর *lsm, ধ্রুবক অক্ষর *fname, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা attr_dirfd = -1;
  cleanup_close পূর্ণসংখ্যা lsm_dirfd = -1;
  অক্ষর *attr_path = ফাঁকা;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  যদি (UNLIKELY (attr_dirfd < 0))
    ফেরত ফাঁকা;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  যদি (lsm != ফাঁকা)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      যদি (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          ফেরত ফাঁকা;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  ফেরত attr_path;
}

স্থির পূর্ণসংখ্যা
check_proc_super_magic (পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
  কাঠামো statfs sfs;

  পূর্ণসংখ্যা ret = fstatfs (fd, &sfs);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "statfs `%s`", path);

  যদি (sfs.f_type != PROC_SUPER_MAGIC)
    ফেরত crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
set_security_attr (libcrun_container_t *container, ধ্রুবক অক্ষর *lsm, ধ্রুবক অক্ষর *fname, ধ্রুবক অক্ষর *তথ্য, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  cleanup_free ধ্রুবক অক্ষর *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close পূর্ণসংখ্যা fd = -1;

  যদি (UNLIKELY (attr_path == ফাঁকা))
    ফেরত -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  যদি (UNLIKELY (fd < 0))
    ফেরত fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  // Write out data
  ret = অস্থায়ী_FAILURE_RETRY (লিখ (fd, তথ্য, strlen (তথ্য)));
  যদি (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      যদি (errno == ENOENT && lsm != ফাঁকা && strcmp (lsm, "apparmor") == 0)
        {
          ধ্রুবক অক্ষর *profile = strchr (তথ্য, ' ');

          ফেরত crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : তথ্য);
        }
      ফেরত crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
set_selinux_label (libcrun_container_t *container, ধ্রুবক অক্ষর *label, bool now, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  ret = libcrun_is_selinux_enabled (err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (ret)
    ফেরত set_security_attr (container, ফাঁকা, now ? "current" : "exec", label, err);
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  যদি (apparmor_enabled < 0)
    ফেরত crun_make_error (err, 0, "AppArmor is not initialized correctly");
  ফেরত apparmor_enabled;
}

স্থির পূর্ণসংখ্যা
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (আকার (UNCONFINED) - 1)
  cleanup_free ধ্রুবক অক্ষর *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close পূর্ণসংখ্যা fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  অক্ষর buf[UNCONFINED_LEN + 1];

  যদি (UNLIKELY (attr_path == ফাঁকা))
    ফেরত -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  যদি (UNLIKELY (fd < 0))
    ফেরত fd;

  যদি (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    ফেরত -1;

  ssize_t bytes_read = অস্থায়ী_FAILURE_RETRY (পঠ (fd, buf, আকার (buf)));
  যদি (UNLIKELY (bytes_read < 0))
    ফেরত crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  যদি (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    ফেরত 1;
  যদি (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    ফেরত 0;
  ফেরত 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

পূর্ণসংখ্যা
set_apparmor_profile (libcrun_container_t *container, ধ্রুবক অক্ষর *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  ret = libcrun_is_apparmor_enabled (err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (ret)
    {
      cleanup_free অক্ষর *buf = ফাঁকা;
      ret = is_current_process_confined (container, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      ফেরত set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
read_all_fd_internal (পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *description, অক্ষর **out, size_t *len, size_t আকার_hint, size_t max_আকার, libcrun_error_t *err)
{
  cleanup_free অক্ষর *buf = ফাঁকা;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t আকার_2 = 0;
  পূর্ণসংখ্যা ret;

  যদি (আকার_hint)
    allocated = আকার_hint;
  অন্যথা
    {
      ret = get_file_আকার (fd, &আকার_2);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "stat `%s`", description);

      যদি (max_আকার && (size_t) আকার_2 > max_আকার)
        ফেরত crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_আকার);

      allocated = আকার_2 == 0 ? 1023 : আকার_2;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  যখন ((আকার_2 && nread < (size_t) আকার_2) || আকার_2 == 0)
    {
      ret = অস্থায়ী_FAILURE_RETRY (পঠ (fd, buf + nread, allocated - nread));
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "read from file `%s`", description);

      যদি (ret == 0)
        বিরতি;

      nread += ret;

      যদি (max_আকার && nread > max_আকার)
        ফেরত crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_আকার);

      যদি (nread == allocated)
        {
          যদি (আকার_2)
            বিরতি;

          যদি (pagesize == 0)
            pagesize = get_page_আকার ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  যদি (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      অক্ষর *tmp = realloc (buf, nread + 1);
      যদি (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = ফাঁকা;
  যদি (len)
    *len = nread;
  ফেরত 0;
}

পূর্ণসংখ্যা
read_all_fd_with_আকার_hint (পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *description, অক্ষর **out, size_t *len, size_t আকার_hint, libcrun_error_t *err)
{
  ফেরত read_all_fd_internal (fd, description, out, len, আকার_hint, 0, err);
}

পূর্ণসংখ্যা
read_all_fd_with_max_আকার (পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *description, অক্ষর **out, size_t *len, size_t max_আকার, libcrun_error_t *err)
{
  ফেরত read_all_fd_internal (fd, description, out, len, 0, max_আকার, err);
}

পূর্ণসংখ্যা
read_all_file_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *path, অক্ষর **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা fd = -1;

  fd = অস্থায়ী_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, errno, "open `%s`", path);

  ফেরত read_all_fd (fd, path, out, len, err);
}

পূর্ণসংখ্যা
read_all_file (ধ্রুবক অক্ষর *path, অক্ষর **out, size_t *len, libcrun_error_t *err)
{
  যদি (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  ফেরত read_all_file_at (AT_FDCWD, path, out, len, err);
}

পূর্ণসংখ্যা
get_realpath_to_file (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *path_name, অক্ষর **absolute_path, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা targetfd = -1;

  targetfd = অস্থায়ী_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  যদি (UNLIKELY (targetfd < 0))
    ফেরত crun_make_error (err, errno, "open `%s`", path_name);
  অন্যথা
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      যদি (UNLIKELY (len < 0))
        ফেরত crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
open_unix_domain_client_socket (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা dgram, libcrun_error_t *err)
{
  কাঠামো sockaddr_un addr = {};
  পূর্ণসংখ্যা ret;
  proc_fd_path_t name_buf;
  cleanup_close পূর্ণসংখ্যা destfd = -1;
  cleanup_close পূর্ণসংখ্যা fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, errno, "create UNIX socket");

  যদি (strlen (path) >= আকার (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (destfd < 0))
        ফেরত crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  যদি (path_len >= আকার (addr.sun_path))
    ফেরত crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (কাঠামো sockaddr *) &addr, আকার (addr));
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  ফেরত ret;
}

পূর্ণসংখ্যা
open_unix_domain_socket (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা dgram, libcrun_error_t *err)
{
  কাঠামো sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  পূর্ণসংখ্যা ret;
  cleanup_close পূর্ণসংখ্যা dirfd = -1;
  cleanup_close পূর্ণসংখ্যা fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, errno, "create UNIX socket");

  যদি (strlen (path) >= আকার (addr.sun_path))
    {
      cleanup_free অক্ষর *dpath = xstrdup (path);
      cleanup_free অক্ষর *bpath = xstrdup (path);
      ধ্রুবক অক্ষর *parent_dir = dirname (dpath);
      ধ্রুবক অক্ষর *base = basename (bpath);
      পূর্ণসংখ্যা n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      যদি (UNLIKELY (dirfd < 0))
        ফেরত crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, আকার (addr.sun_path), "%s/%s", name_buf, base);
      যদি (n < 0 || (size_t) n >= আকার (addr.sun_path))
        ফেরত crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  অন্যথা
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (কাঠামো sockaddr *) &addr, আকার (addr));
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "bind socket to `%s`", path);

  যদি (! dgram)
    {
      ret = listen (fd, 1);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  ফেরত ret;
}

পূর্ণসংখ্যা
send_fd_to_socket (পূর্ণসংখ্যা server, পূর্ণসংখ্যা fd, libcrun_error_t *err)
{
  ফেরত send_fd_to_socket_with_payload (server, fd, ফাঁকা, 0, err);
}

পূর্ণসংখ্যা
send_fd_to_socket_with_payload (পূর্ণসংখ্যা server, পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *payload, size_t payload_len, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  কাঠামো cmsghdr *cmsg = ফাঁকা;
  কাঠামো iovec iov[2];
  কাঠামো msghdr বার্তা = {};
  অক্ষর ctrl_buf[CMSG_SPACE (1 + আকার (পূর্ণসংখ্যা))] = {};
  অক্ষর তথ্য[1];

  তথ্য[0] = ' ';
  iov[0].iov_base = তথ্য;
  iov[0].iov_len = আকার (তথ্য);

  যদি (payload_len > 0)
    {
      iov[0].iov_base = (শূন্য *) payload;
      iov[0].iov_len = payload_len;
    }

  বার্তা.বার্তা_name = ফাঁকা;
  বার্তা.বার্তা_namelen = 0;
  বার্তা.বার্তা_iov = iov;
  বার্তা.বার্তা_iovlen = 1;
  বার্তা.বার্তা_controllen = CMSG_SPACE (আকার (পূর্ণসংখ্যা));
  বার্তা.বার্তা_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&বার্তা);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (আকার (পূর্ণসংখ্যা));

  *((পূর্ণসংখ্যা *) CMSG_তথ্য (cmsg)) = fd;

  ret = অস্থায়ী_FAILURE_RETRY (sendmsg (server, &বার্তা, 0));
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "sendmsg");
  ফেরত 0;
}

পূর্ণসংখ্যা
receive_fd_from_socket_with_payload (পূর্ণসংখ্যা from, অক্ষর *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা fd = -1;
  পূর্ণসংখ্যা ret;
  কাঠামো iovec iov[1];
  কাঠামো msghdr বার্তা = {};
  অক্ষর ctrl_buf[CMSG_SPACE (আকার (পূর্ণসংখ্যা))] = {};
  অক্ষর তথ্য[1];
  কাঠামো cmsghdr *cmsg;

  তথ্য[0] = ' ';
  iov[0].iov_base = তথ্য;
  iov[0].iov_len = আকার (তথ্য);

  যদি (payload_len > 0)
    {
      iov[0].iov_base = (শূন্য *) payload;
      iov[0].iov_len = payload_len;
    }

  বার্তা.বার্তা_name = ফাঁকা;
  বার্তা.বার্তা_namelen = 0;
  বার্তা.বার্তা_iov = iov;
  বার্তা.বার্তা_iovlen = 1;
  বার্তা.বার্তা_controllen = CMSG_SPACE (আকার (পূর্ণসংখ্যা));
  বার্তা.বার্তা_control = ctrl_buf;

  ret = অস্থায়ী_FAILURE_RETRY (recvmsg (from, &বার্তা, 0));
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "recvmsg");
  যদি (UNLIKELY (ret == 0))
    ফেরত crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&বার্তা);
  যদি (cmsg == ফাঁকা)
    ফেরত crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_তথ্য (cmsg), আকার (fd));

  ret = fd;
  fd = -1;
  ফেরত ret;
}

পূর্ণসংখ্যা
receive_fd_from_socket (পূর্ণসংখ্যা from, libcrun_error_t *err)
{
  ফেরত receive_fd_from_socket_with_payload (from, ফাঁকা, 0, err);
}

পূর্ণসংখ্যা
create_socket_pair (পূর্ণসংখ্যা *pair, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "socketpair");
  ফেরত 0;
}

পূর্ণসংখ্যা
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret = signalfd (-1, mask, 0);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "signalfd");
  ফেরত ret;
}

স্থির পূর্ণসংখ্যা
epoll_helper_toggle (পূর্ণসংখ্যা epollfd, পূর্ণসংখ্যা fd, পূর্ণসংখ্যা events, libcrun_error_t *err)
{
  কাঠামো epoll_event ev = {};
  bool add = events != 0;
  পূর্ণসংখ্যা ret;

  ev.events = events;
  ev.তথ্য.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  যদি (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  যদি (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      যদি (errno == ENOENT || errno == EPERM)
        ফেরত 0;
      ফেরত crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  ফেরত 0;
}

পূর্ণসংখ্যা
epoll_helper (পূর্ণসংখ্যা *in_fds, পূর্ণসংখ্যা *in_levelfds, পূর্ণসংখ্যা *out_fds, পূর্ণসংখ্যা *out_levelfds, libcrun_error_t *err)
{
  কাঠামো epoll_event ev;
  cleanup_close পূর্ণসংখ্যা epollfd = -1;
  পূর্ণসংখ্যা ret;

  পূর্ণসংখ্যা *it;
  epollfd = epoll_create1 (0);
  যদি (UNLIKELY (epollfd < 0))
    ফেরত crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  জন্য (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.তথ্য.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      যদি (UNLIKELY (ret < 0))                                           \
        ফেরত crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  যদি (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  যদি (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  যদি (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  যদি (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  ফেরত ret;
}

পূর্ণসংখ্যা
copy_from_fd_to_fd (পূর্ণসংখ্যা src, পূর্ণসংখ্যা dst, পূর্ণসংখ্যা consume, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  ssize_t nread;
  size_t pagesize = get_page_আকার ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  করো
    {
      cleanup_free অক্ষর *বাফার = ফাঁকা;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      যদি (can_copy_file_range)
        {
          nread = copy_file_range (src, ফাঁকা, dst, ফাঁকা, pagesize, 0);
          যদি (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              যাও fallback;
            }
          যদি (consume && nread < 0 && errno == EAGAIN)
            ফেরত 0;
          যদি (nread < 0 && errno == EIO)
            ফেরত 0;
          যদি (UNLIKELY (nread < 0))
            ফেরত crun_make_error (err, errno, "copy_file_range");
          চালু;
        }
    fallback:
#endif

      বাফার = xmalloc (pagesize);
      nread = অস্থায়ী_FAILURE_RETRY (পঠ (src, বাফার, pagesize));
      যদি (consume && nread < 0 && errno == EAGAIN)
        ফেরত 0;
      যদি (nread < 0 && errno == EIO)
        ফেরত 0;
      যদি (UNLIKELY (nread < 0))
        ফেরত crun_make_error (err, errno, "read");

      remaining = nread;
      যখন (remaining)
        {
          ret = অস্থায়ী_FAILURE_RETRY (লিখ (dst, বাফার + nread - remaining, remaining));
          যদি (UNLIKELY (ret < 0))
            ফেরত crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } যখন (consume && nread);

  ফেরত 0;
}

পূর্ণসংখ্যা
run_process (অক্ষর **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  যদি (UNLIKELY (pid < 0))
    ফেরত crun_make_error (err, errno, "fork");
  যদি (pid)
    {
      পূর্ণসংখ্যা r, status;
      r = অস্থায়ী_FAILURE_RETRY (waitpid (pid, &status, 0));
      যদি (r < 0)
        ফেরত crun_make_error (err, errno, "waitpid");
      যদি (WIFEXITED (status) || WIFSIGNALED (status))
        ফেরত WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
স্থির unsigned
atou (অক্ষর **s)
{
  unsigned x;
  জন্য (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  ফেরত x;
}

পূর্ণসংখ্যা
fgetpwent_r (FILE *f, কাঠামো passwd *pw, অক্ষর *সারি, size_t আকার_2, কাঠামো passwd **res)
{
  অক্ষর *s;
  পূর্ণসংখ্যা rv = 0;
  জন্য (;;)
    {
      সারি[আকার_2 - 1] = '\xff';
      যদি ((fgets (সারি, আকার_2, f) == ফাঁকা) || ferror (f) || সারি[আকার_2 - 1] != '\xff')
        {
          rv = (সারি[আকার_2 - 1] != '\xff') ? ERANGE : ENOENT;
          সারি = 0;
          pw = 0;
          বিরতি;
        }
      সারি[strcspn (সারি, "\n")] = 0;

      s = সারি;
      pw->pw_name = s++;
      যদি (! (s = strchr (s, ':')))
        চালু;

      *s++ = 0;
      pw->pw_passwd = s;
      যদি (! (s = strchr (s, ':')))
        চালু;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      যদি (*s != ':')
        চালু;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      যদি (*s != ':')
        চালু;

      *s++ = 0;
      pw->pw_gecos = s;
      যদি (! (s = strchr (s, ':')))
        চালু;

      *s++ = 0;
      pw->pw_dir = s;
      যদি (! (s = strchr (s, ':')))
        চালু;

      *s++ = 0;
      pw->pw_shell = s;
      বিরতি;
    }
  *res = pw;
  যদি (rv)
    errno = rv;
  ফেরত rv;
}
#endif

পূর্ণসংখ্যা
set_home_env (uid_t id)
{
  কাঠামো passwd pwd;
  cleanup_free অক্ষর *buf = ফাঁকা;
  long buf_আকার;
  cleanup_file FILE *stream = ফাঁকা;

  buf_আকার = sysconf (_SC_GETPW_R_SIZE_MAX);
  যদি (buf_আকার < 0)
    buf_আকার = 1024;

  buf = xmalloc (buf_আকার);

  stream = fopen ("/etc/passwd", "re");
  যদি (stream == ফাঁকা)
    ফেরত -1;

  জন্য (;;)
    {
      কাঠামো passwd *ret_pw = ফাঁকা;

      পূর্ণসংখ্যা ret = fgetpwent_r (stream, &pwd, buf, buf_আকার, &ret_pw);
      যদি (UNLIKELY (ret != 0))
        {
          যদি (ret == EINTR)
            চালু;

          যদি (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              ফেরত -1;
            }

          buf_আকার *= 2;
          buf = xrealloc (buf, buf_আকার);
          চালু;
        }

      যদি (ret_pw && ret_pw->pw_uid == id)
        {
          যদি (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          অন্যথা
            ফেরত 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
স্থির পূর্ণসংখ্যা
getsubidrange (uid_t id, পূর্ণসংখ্যা is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *প্রবেশ = ফাঁকা;
  cleanup_free অক্ষর *lineptr = ফাঁকা;
  size_t lenlineptr = 0, len_name;
  long buf_আকার;
  cleanup_free অক্ষর *buf = ফাঁকা;
  ধ্রুবক অক্ষর *name;
  কাঠামো passwd pwd;

  buf_আকার = sysconf (_SC_GETPW_R_SIZE_MAX);
  যদি (buf_আকার < 0)
    buf_আকার = 1024;

  buf = xmalloc (buf_আকার);
  জন্য (;;)
    {
      পূর্ণসংখ্যা ret;
      কাঠামো passwd *ret_pw = ফাঁকা;

      ret = getpwuid_r (id, &pwd, buf, buf_আকার, &ret_pw);
      যদি (LIKELY (ret == 0))
        {
          যদি (ret_pw)
            {
              name = ret_pw->pw_name;
              বিরতি;
            }
          ফেরত -1;
        }

      যদি (ret != ERANGE)
        ফেরত -1;

      buf_আকার *= 2;
      buf = xrealloc (buf, buf_আকার);
    }

  len_name = strlen (name);

  প্রবেশ = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  যদি (প্রবেশ == ফাঁকা)
    ফেরত -1;

  জন্য (;;)
    {
      অক্ষর *endptr;
      ssize_t পঠ = getline (&lineptr, &lenlineptr, প্রবেশ);
      যদি (পঠ < 0)
        ফেরত -1;

      যদি (পঠ < (ssize_t) (len_name + 2))
        চালু;

      যদি (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        চালু;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      যদি (endptr >= &lineptr[পঠ])
        ফেরত -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      ফেরত 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

পূর্ণসংখ্যা
format_default_id_mapping (অক্ষর **out, uid_t container_id, uid_t host_uid, uid_t host_id, পূর্ণসংখ্যা is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free অক্ষর *বাফার = ফাঁকা;
  পূর্ণসংখ্যা written = 0;
  পূর্ণসংখ্যা ret, remaining;

  *out = ফাঁকা;

  যদি (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    ফেরত 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  বাফার = xmalloc (remaining + 1);

  যদি (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (বাফার + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      যদি (UNLIKELY (ret >= remaining))
        ফেরত crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (বাফার + written, remaining, "%d %d 1\n", container_id, host_id);
  যদি (UNLIKELY (ret >= remaining))
    ফেরত crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  যদি (available)
    {
      ret = snprintf (বাফার + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      যদি (UNLIKELY (ret >= remaining))
        ফেরত crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = বাফার;
  বাফার = ফাঁকা;
  ফেরত written;
}

স্থির পূর্ণসংখ্যা
unset_cloexec_flag (পূর্ণসংখ্যা fd)
{
  পূর্ণসংখ্যা flags = fcntl (fd, F_GETFD);
  যদি (flags == -1)
    ফেরত -1;

  flags &= ~FD_CLOEXEC;

  ফেরত fcntl (fd, F_SETFD, flags);
}

স্থির শূন্য __attribute__ ((__noreturn__))
run_process_child (অক্ষর *path, অক্ষর **args, ধ্রুবক অক্ষর *cwd, অক্ষর **envp, পূর্ণসংখ্যা pipe_r,
                   পূর্ণসংখ্যা pipe_w, পূর্ণসংখ্যা out_fd, পূর্ণসংখ্যা err_fd, bool can_ignore_chdir_errors)
{
  অক্ষর *tmp_args[] = { path, ফাঁকা };
  libcrun_error_t err = ফাঁকা;
  পূর্ণসংখ্যা dev_null_fd = -1;
  পূর্ণসংখ্যা ret;

  ret = mark_or_close_fds_ge_than (ফাঁকা, 3, false, &err);
  যদি (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->বার্তা);

  যদি (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      যদি (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  অস্থায়ী_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  অস্থায়ী_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  যদি (out_fd >= 0)
    unset_cloexec_flag (1);
  যদি (err_fd >= 0)
    unset_cloexec_flag (2);

  যদি (dev_null_fd >= 0)
    অস্থায়ী_FAILURE_RETRY (close (dev_null_fd));
  যদি (out_fd >= 0)
    অস্থায়ী_FAILURE_RETRY (close (out_fd));
  যদি (err_fd >= 0)
    অস্থায়ী_FAILURE_RETRY (close (err_fd));

  যদি (args == ফাঁকা)
    args = tmp_args;

  যদি (cwd && chdir (cwd) < 0)
    {
      যদি (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
পূর্ণসংখ্যা
run_process_with_stdin_timeout_envp (অক্ষর *path, অক্ষর **args, ধ্রুবক অক্ষর *cwd, পূর্ণসংখ্যা timeout,
                                     অক্ষর **envp, অক্ষর *stdin, size_t stdin_len, পূর্ণসংখ্যা out_fd,
                                     পূর্ণসংখ্যা err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  পূর্ণসংখ্যা stdin_pipe[2];
  pid_t pid;
  পূর্ণসংখ্যা ret;
  cleanup_close পূর্ণসংখ্যা pipe_r = -1;
  cleanup_close পূর্ণসংখ্যা pipe_w = -1;
  sigset_t oldmask, mask;
  পূর্ণসংখ্যা r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  যদি (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  যদি (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      যাও restore_sig_mask_and_exit;
    }

  যদি (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = অস্থায়ী_FAILURE_RETRY (লিখ (pipe_w, stdin, stdin_len));
  যদি (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      যদি (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          যাও restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  যদি (timeout)
    {
      time_t start = time (ফাঁকা);
      time_t now;
      জন্য (now = start; now - start < timeout; now = time (ফাঁকা))
        {
          siginfo_t info;
          পূর্ণসংখ্যা elapsed = now - start;
          কাঠামো timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          যদি (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              যাও restore_sig_mask_and_exit;
            }

          যদি (info.si_signo == SIGCHLD && info.si_pid == pid)
            যাও read_waitpid;

          যদি (ret < 0 && errno == EAGAIN)
            যাও timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      যাও restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  যদি (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  অন্যথা
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  যদি (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      যদি (pid > 0)
        {
          kill (pid, SIGKILL);
          অস্থায়ী_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, ফাঁকা);
      যদি (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  ফেরত ret;
}

পূর্ণসংখ্যা
mark_or_close_fds_ge_than (libcrun_container_t *container, পূর্ণসংখ্যা n, bool close_now, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা cfd = -1;
  cleanup_dir DIR *dir = ফাঁকা;
  পূর্ণসংখ্যা ret;
  পূর্ণসংখ্যা fd;
  কাঠামো dirent *পরবর্তী;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  যদি (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  যদি (ret == 0)
    {
      যদি (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      ফেরত 0;
    }
  যদি (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    ফেরত crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  যদি (UNLIKELY (cfd < 0))
    ফেরত cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  dir = fdopendir (cfd);
  যদি (UNLIKELY (dir == ফাঁকা))
    ফেরত crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  জন্য (পরবর্তী = readdir (dir); পরবর্তী; পরবর্তী = readdir (dir))
    {
      পূর্ণসংখ্যা val;
      ধ্রুবক অক্ষর *name = পরবর্তী->d_name;
      যদি (name[0] == '.')
        চালু;

      val = strtoll (name, ফাঁকা, 10);
      যদি (val < n || val == fd)
        চালু;

      যদি (close_now)
        {
          ret = close (val);
          যদি (UNLIKELY (ret < 0))
            ফেরত crun_make_error (err, errno, "close fd `%d`", val);

          যদি (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      অন্যথা
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          যদি (UNLIKELY (ret < 0))
            ফেরত crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  ফেরত 0;
}

শূন্য
get_current_timestamp (অক্ষর *out, size_t len)
{
  কাঠামো timeval tv;
  কাঠামো tm now;
  অক্ষর timestamp[64];

  gettimeofday (&tv, ফাঁকা);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, আকার (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (শূন্য) snprintf (out, len, "%s.%06lldZ", timestamp, (long long পূর্ণসংখ্যা) tv.tv_usec);
  out[len - 1] = '\0';
}

পূর্ণসংখ্যা
set_blocking_fd (পূর্ণসংখ্যা fd, bool blocking, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret, flags = fcntl (fd, F_GETFL, 0);
  যদি (UNLIKELY (flags < 0))
    ফেরত crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "fcntl");
  ফেরত 0;
}

পূর্ণসংখ্যা
parse_json_file (json_object **out, ধ্রুবক অক্ষর *jsondata, কাঠামো parser_context *ctx arg_unused, libcrun_error_t *err)
{
  কাঠামো json_tokener *tok;
  গণনা json_tokener_error jerr;

  *err = ফাঁকা;

  যদি (UNLIKELY (jsondata == ফাঁকা))
    ফেরত crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  যদি (UNLIKELY (tok == ফাঁকা))
    ফেরত crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (পূর্ণসংখ্যা) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  যদি (*out == ফাঁকা || jerr != json_tokener_success)
    {
      পূর্ণসংখ্যা saved_pos = (পূর্ণসংখ্যা) tok->char_offset;

      json_tokener_free (tok);
      যদি (*out)
        {
          json_object_put (*out);
          *out = ফাঁকা;
        }
      ফেরত crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  ফেরত 0;
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
স্থির পূর্ণসংখ্যা
check_access (ধ্রুবক অক্ষর *path)
{
  পূর্ণসংখ্যা ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#অন্যথা
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  যদি (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      যদি (errno == EACCES)
        {
          পূর্ণসংখ্যা saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          যদি (ret == 0)
            ফেরত CHECK_ACCESS_NOT_EXECUTABLE;
        }
      ফেরত -errno;
    }

  ret = get_file_type (&mode, false, path);
  যদি (UNLIKELY (ret < 0))
    ফেরত -errno;

  যদি (! S_ISREG (mode))
    ফেরত CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  ফেরত 0;
}

পূর্ণসংখ্যা
find_executable (অক্ষর **out, ধ্রুবক অক্ষর *executable_path, ধ্রুবক অক্ষর *cwd, libcrun_error_t *err)
{
  cleanup_free অক্ষর *cwd_executable_path = ফাঁকা;
  cleanup_free অক্ষর *tmp = ফাঁকা;
  পূর্ণসংখ্যা last_error = -ENOENT;
  অক্ষর *it, *end;
  পূর্ণসংখ্যা ret;

  *out = ফাঁকা;

  যদি (executable_path == ফাঁকা || executable_path[0] == '\0')
    ফেরত crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  যদি (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free অক্ষর *cwd_allocated = ফাঁকা;

      যদি (cwd == ফাঁকা)
        {
          cwd_allocated = getcwd (ফাঁকা, 0);
          যদি (cwd_allocated == ফাঁকা)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  যদি (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      যদি (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          ফেরত 0;
        }
      last_error = ret;
      যাও fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  যখন ((it = strsep (&end, ":")))
    {
      cleanup_free অক্ষর *path = ফাঁকা;

      যদি (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          চালু;
        }

      ret = check_access (path);
      যদি (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = ফাঁকা;
          ফেরত 0;
        }

      যদি (ret == -ENOENT)
        চালু;

      last_error = ret;
    }

fail:
  নির্বাচন (last_error)
    {
    ক্ষেত্র CHECK_ACCESS_NOT_EXECUTABLE:
      ফেরত crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    ক্ষেত্র CHECK_ACCESS_NOT_REGULAR:
      ফেরত crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    পূর্বনির্ধারিত:
      errno = -last_error;
      যদি (errno == ENOENT)
        ফেরত crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      ফেরত crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

স্থির ssize_t
safe_read_xattr (অক্ষর **ret, পূর্ণসংখ্যা sfd, ধ্রুবক অক্ষর *srcname, ধ্রুবক অক্ষর *name, size_t initial_আকার, libcrun_error_t *err)
{
  cleanup_free অক্ষর *বাফার = ফাঁকা;
  ssize_t current_আকার;
  ssize_t s;

  current_আকার = (ssize_t) initial_আকার;
  বাফার = xmalloc (current_আকার + 1);

  যখন (1)
    {
      s = fgetxattr (sfd, name, বাফার, current_আকার);
      যদি (UNLIKELY (s < 0))
        ফেরত crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      যদি (s < current_আকার)
        বিরতি;

      current_আকার *= 2;
      বাফার = xrealloc (বাফার, current_আকার + 1);
    }

  যদি (s <= 0)
    ফেরত s;

  বাফার[s] = '\0';

  /* Change owner.  */
  *ret = বাফার;
  বাফার = ফাঁকা;

  ফেরত s;
}

স্থির ssize_t
copy_xattr (পূর্ণসংখ্যা sfd, পূর্ণসংখ্যা dfd, ধ্রুবক অক্ষর *srcname, ধ্রুবক অক্ষর *destname, libcrun_error_t *err)
{
  cleanup_free অক্ষর *buf = ফাঁকা;
  ssize_t xattr_len;
  অক্ষর *it;

  xattr_len = flistxattr (sfd, ফাঁকা, 0);
  যদি (UNLIKELY (xattr_len < 0))
    {
      যদি (errno == ENOTSUP)
        ফেরত 0;

      ফেরত crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  যদি (xattr_len == 0)
    ফেরত 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  যদি (UNLIKELY (xattr_len < 0))
    ফেরত crun_make_error (err, errno, "flistxattr `%s`", srcname);

  জন্য (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free অক্ষর *v = ফাঁকা;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      যদি (UNLIKELY (s < 0))
        ফেরত s;

      s = fsetxattr (dfd, it, v, s, 0);
      যদি (UNLIKELY (s < 0))
        {
          যদি (errno == EINVAL || errno == EOPNOTSUPP)
            চালু;

          ফেরত crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  ফেরত 0;
}

#endif

স্থির পূর্ণসংখ্যা
copy_rec_stat_file_at (পূর্ণসংখ্যা dfd, ধ্রুবক অক্ষর *path, mode_t *mode, off_t *আকার_2, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  কাঠামো stat st;
  পূর্ণসংখ্যা ret;

#ifdef HAVE_STATX
  কাঠামো statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_আকার | STATX_UID | STATX_GID, &stx);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOSYS || errno == EINVAL)
        যাও fallback;

      ফেরত ret;
    }

  *mode = stx.stx_mode;
  *আকার_2 = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  ফেরত ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *আকার_2 = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  ফেরত ret;
}

পূর্ণসংখ্যা
copy_recursive_fd_to_fd (পূর্ণসংখ্যা srcdirfd, পূর্ণসংখ্যা dfd, ধ্রুবক অক্ষর *srcname, ধ্রুবক অক্ষর *destname, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = ফাঁকা;
  কাঠামো dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  যদি (UNLIKELY (dsrcfd == ফাঁকা))
    {
      অস্থায়ী_FAILURE_RETRY (close (srcdirfd));
      ফেরত crun_make_error (err, errno, "open directory `%s`", destname);
    }

  জন্য (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close পূর্ণসংখ্যা srcfd = -1;
      cleanup_close পূর্ণসংখ্যা destfd = -1;
      cleanup_free অক্ষর *target_buf = ফাঁকা;
      পূর্ণসংখ্যা ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      যদি (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        চালু;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      নির্বাচন (mode & S_IFMT)
        {
        ক্ষেত্র S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          যদি (UNLIKELY (srcfd < 0))
            ফেরত crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          যদি (UNLIKELY (destfd < 0))
            ফেরত crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;

#ifdef HAVE_FGETXATTR
          ret = (পূর্ণসংখ্যা) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
#endif

          অস্থায়ী_FAILURE_RETRY (close (destfd));
          destfd = -1;
          বিরতি;

        ক্ষেত্র S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          যদি (UNLIKELY (ret < 0))
            ফেরত crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          যদি (UNLIKELY (srcfd < 0))
            ফেরত crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          যদি (UNLIKELY (destfd < 0))
            ফেরত crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (পূর্ণসংখ্যা) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          বিরতি;

        ক্ষেত্র S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          যদি (UNLIKELY (ret < 0))
            ফেরত crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          বিরতি;

        ক্ষেত্র S_IFBLK:
        ক্ষেত্র S_IFCHR:
        ক্ষেত্র S_IFIFO:
        ক্ষেত্র S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          যদি (UNLIKELY (ret < 0))
            ফেরত crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          বিরতি;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      যদি (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          যদি (errno == ENOTSUP)
            চালু;

          ফেরত crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  ফেরত 0;
}

ধ্রুবক অক্ষর *
find_annotation (libcrun_container_t *container, ধ্রুবক অক্ষর *name)
{
  যদি (container->container_def->annotations == ফাঁকা)
    ফেরত ফাঁকা;

  ফেরত find_string_map_মান (container->annotations, name);
}

পূর্ণসংখ্যা
safe_write (পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *fname, ধ্রুবক শূন্য *buf, size_t গুনতি, libcrun_error_t *err)
{
  size_t written = 0;
  যখন (written < গুনতি)
    {
      ssize_t w = লিখ (fd, (ধ্রুবক অক্ষর *) buf + written, গুনতি - written);
      যদি (UNLIKELY (w < 0))
        {
          যদি (errno == EINTR || errno == EAGAIN)
            চালু;
          ফেরত crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  ফেরত 0;
}

#define MAX_PARTS 32

পূর্ণসংখ্যা
append_paths (অক্ষর **out, libcrun_error_t *err, ...)
{
  ধ্রুবক অক্ষর *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t যোগ_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  জন্য (;;)
    {
      ধ্রুবক অক্ষর *part;
      size_t আকার_2;

      part = va_arg (ap, ধ্রুবক অক্ষর *);
      যদি (part == ফাঁকা)
        বিরতি;

      যদি (n_parts == MAX_PARTS)
        {
          va_end (ap);
          ফেরত crun_make_error (err, EINVAL, "too many paths specified");
        }

      যদি (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          যখন (part[0] == '/' && part[1] == '/')
            part++;
        }
      অন্যথা
        {
          /* And drop any initial '/' for other components.  */
          যখন (part[0] == '/')
            part++;
        }

      আকার_2 = strlen (part);
      যদি (আকার_2 == 0)
        চালু;

      যখন (আকার_2 > 1 && part[আকার_2 - 1] == '/')
        আকার_2--;

      parts[n_parts] = part;
      sizes[n_parts] = আকার_2;

      n_parts++;
    }
  va_end (ap);

  যোগ_len = n_parts + 1;
  জন্য (i = 0; i < n_parts; i++)
    যোগ_len += sizes[i];

  *out = xmalloc (যোগ_len);

  copied = 0;
  জন্য (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      যদি (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  ফেরত 0;
}

#যদি __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#অন্যথা
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
স্থির পূর্ণসংখ্যা
b64_প্রবেশ (অক্ষর c)
{
  ধ্রুবক অক্ষর table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  পূর্ণসংখ্যা i;

  জন্য (i = 0; i < 64; i++)
    {
      যদি (table[i] == c)
        ফেরত i;
    }
  ফেরত -1;
}

পূর্ণসংখ্যা
base64_decode (ধ্রুবক অক্ষর *iptr, size_t isize, অক্ষর *optr, size_t osize, size_t *nbytes)
{
  পূর্ণসংখ্যা i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned অক্ষর তথ্য[4];

  *nbytes = 0;
  যখন (consumed < isize && (*nbytes) + 3 < osize)
    {
      যখন ((i < 4) && (consumed < isize))
        {
          tmp = b64_প্রবেশ (*iptr++);
          consumed++;
          যদি (tmp != -1)
            {
              যদি (UNLIKELY (pad > 0))
                ফেরত -1;
              তথ্য[i++] = tmp;
            }
          অন্যথা যদি (*(iptr - 1) == '=')
            {
              তথ্য[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      যদি (i == 4)
        {
          যদি (UNLIKELY (pad > 2))
            ফেরত -1;
          *optr++ = (তথ্য[0] << 2) | ((তথ্য[1] & 0x30) >> 4);
          *optr++ = ((তথ্য[1] & 0xf) << 4) | ((তথ্য[2] & 0x3c) >> 2);
          *optr++ = ((তথ্য[2] & 0x3) << 6) | তথ্য[3];
          (*nbytes) += 3 - pad;
        }
      অন্যথা
        {
          /* I did not get all the data.  */
          consumed -= i;
          ফেরত consumed;
        }
      i = 0;
    }
  ফেরত consumed;
}

অক্ষর *
get_user_name (uid_t uid)
{
  কাঠামো passwd pd;
  কাঠামো passwd *অস্থায়ী_ফলাফল_ptr;
  অক্ষর pwdbuffer[200];
  যদি (! getpwuid_r (uid, &pd, pwdbuffer, আকার (pwdbuffer), &অস্থায়ী_ফলাফল_ptr))
    ফেরত xstrdup (pd.pw_name);
  ফেরত xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
অক্ষর *
libcrun_sysctl_চাবি_to_proc_path (ধ্রুবক অক্ষর *চাবি)
{
  ধ্রুবক অক্ষর *sep;
  অক্ষর *ret, *it;

  sep = strpbrk (চাবি, "./");

  /* No separator at all, or the key is already in the path form.  */
  যদি (sep == ফাঁকা || *sep == '/')
    ফেরত xstrdup (চাবি);

  ret = xstrdup (চাবি);
  জন্য (it = ret; *it; it++)
    {
      যদি (*it == '.')
        *it = '/';
      অন্যথা যদি (*it == '/')
        *it = '.';
    }

  ফেরত ret;
}

পূর্ণসংখ্যা
has_suffix (ধ্রুবক অক্ষর *str, ধ্রুবক অক্ষর *suffix)
{
  যদি (! str || ! suffix)
    ফেরত 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  যদি (lensuffix > lenstr)
    ফেরত 0;
  ফেরত memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

অক্ষর *
str_join_array (পূর্ণসংখ্যা offset, size_t আকার_2, অক্ষর *ধ্রুবক array[], ধ্রুবক অক্ষর *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = ফাঁকা;
  size_t i, যোগ_আকার = (আকার_2 - 1) * (jlen = strlen (joint)) + 1;
  অক্ষর *ফলাফল, *p;

  lens = xmalloc (আকার_2 * আকার (*lens));

  জন্য (i = 0; i < আকার_2; ++i)
    {
      যোগ_আকার += (lens[i] = strlen (array[i]));
    }
  p = ফলাফল = xmalloc (যোগ_আকার);
  জন্য (i = offset; i < আকার_2; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      যদি (i < আকার_2 - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  ফেরত ফলাফল;
}

পূর্ণসংখ্যা
libcrun_mmap (কাঠামো libcrun_mmap_s **ret, শূন্য *addr, size_t দৈর্ঘ্য,
              পূর্ণসংখ্যা prot, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা fd, off_t offset,
              libcrun_error_t *err)
{
  কাঠামো libcrun_mmap_s *mmap_s = ফাঁকা;

  শূন্য *mapped = mmap (addr, দৈর্ঘ্য, prot, flags, fd, offset);
  যদি (mapped == MAP_FAILED)
    ফেরত crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (আকার (কাঠামো libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->দৈর্ঘ্য = দৈর্ঘ্য;

  *ret = mmap_s;

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_munmap (কাঠামো libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  ret = munmap (mmap->addr, mmap->দৈর্ঘ্য);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "munmap");

  মুক্ত (mmap);

  ফেরত 0;
}

স্থির long
read_file_as_long_or_default (ধ্রুবক অক্ষর *path, long def_মান)
{
  cleanup_free অক্ষর *content = ফাঁকা;
  libcrun_error_t tmp_err = ফাঁকা;
  অক্ষর *endptr;
  long val;
  পূর্ণসংখ্যা ret;

  ret = read_all_file (path, &content, ফাঁকা, &tmp_err);
  যদি (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ফেরত def_মান;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  যদি (UNLIKELY (errno))
    ফেরত def_মান;
  যদি (endptr == content || (*endptr && *endptr != '\n'))
    ফেরত def_মান;
  ফেরত val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (শূন্য)
{
  স্থির atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  যদি (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  ফেরত uid;
}

gid_t
get_overflow_gid (শূন্য)
{
  স্থির atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  যদি (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  ফেরত gid;
}

শূন্য
consume_trailing_slashes (অক্ষর *path)
{
  যদি (! path || path[0] == '\0')
    ফেরত;

  অক্ষর *last = path + strlen (path);

  যখন (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

অক্ষর **
read_dir_entries (ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = ফাঁকা;
  size_t n_entries = 0;
  size_t entries_আকার = 16;
  অক্ষর **entries = ফাঁকা;
  কাঠামো dirent *de;

  dir = opendir (path);
  যদি (UNLIKELY (dir == ফাঁকা))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      ফেরত ফাঁকা;
    }

  entries = xmalloc (entries_আকার * আকার (অক্ষর *));
  যখন ((de = readdir (dir)))
    {
      যদি (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        চালু;
      যদি (n_entries == entries_আকার)
        {
          entries_আকার *= 2;
          entries = xrealloc (entries, entries_আকার * আকার (অক্ষর *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * আকার (অক্ষর *));
  entries[n_entries] = ফাঁকা;

  ফেরত entries;
}

পূর্ণসংখ্যা
cpuset_string_to_bitmask (ধ্রুবক অক্ষর *str, অক্ষর **out, size_t *out_আকার, libcrun_error_t *err)
{
  cleanup_free অক্ষর *mask = ফাঁকা;
  size_t mask_আকার = 0;
  ধ্রুবক অক্ষর *p = str;
  অক্ষর *endptr;

  যখন (*p)
    {
      long long start_range, end_range;

      যদি (*p < '0' || *p > '9')
        যাও invalid_প্রবেশ;

      start_range = strtoll (p, &endptr, 10);
      যদি (start_range < 0)
        যাও invalid_প্রবেশ;

      p = endptr;

      যদি (*p != '-')
        end_range = start_range;
      অন্যথা
        {
          p++;

          যদি (*p < '0' || *p > '9')
            যাও invalid_প্রবেশ;

          end_range = strtoll (p, &endptr, 10);

          যদি (end_range < start_range)
            যাও invalid_প্রবেশ;

          p = endptr;
        }

      /* Just set some limit.  */
      যদি (end_range > (1 << 20))
        যাও invalid_প্রবেশ;

      যদি (end_range >= (long long) (mask_আকার * CHAR_BIT))
        {
          size_t new_mask_আকার = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_আকার);
          memset (mask + mask_আকার, 0, new_mask_আকার - mask_আকার);
          mask_আকার = new_mask_আকার;
        }

      জন্য (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      যদি (*p == ',')
        p++;
      অন্যথা যদি (*p)
        যাও invalid_প্রবেশ;
    }

  *out = mask;
  mask = ফাঁকা;
  *out_আকার = mask_আকার;

  ফেরত 0;

invalid_প্রবেশ:
  ফেরত crun_make_error (err, 0, "cannot parse input `%s`", str);
}

কাঠামো channel_fd_pair
{
  কাঠামো ring_বাফার *rb;

  পূর্ণসংখ্যা in_fd;
  পূর্ণসংখ্যা out_fd;

  পূর্ণসংখ্যা infd_epoll_events;
  পূর্ণসংখ্যা outfd_epoll_events;
};

কাঠামো channel_fd_pair *
channel_fd_pair_new (পূর্ণসংখ্যা in_fd, পূর্ণসংখ্যা out_fd, size_t আকার_2)
{
  কাঠামো channel_fd_pair *channel = xmalloc (আকার (কাঠামো channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_বাফার_make (আকার_2);
  ফেরত channel;
}

শূন্য
channel_fd_pair_free (কাঠামো channel_fd_pair *channel)
{
  যদি (channel == ফাঁকা)
    ফেরত;

  ring_বাফার_free (channel->rb);
  মুক্ত (channel);
}

পূর্ণসংখ্যা
channel_fd_pair_process (কাঠামো channel_fd_pair *channel, পূর্ণসংখ্যা epollfd, libcrun_error_t *err)
{
  bool is_প্রবেশ_eagain = false, is_output_eagain = false, repeat;
  পূর্ণসংখ্যা ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  জন্য (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      যদি (ring_বাফার_get_space_available (channel->rb) > 0)
        {
          ret = ring_বাফার_read (channel->rb, channel->in_fd, &is_প্রবেশ_eagain, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          যদি (ret > 0)
            repeat = true;
        }
      যদি (ring_বাফার_get_তথ্য_available (channel->rb) > 0)
        {
          ret = ring_বাফার_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          যদি (ret > 0)
            repeat = true;
        }
    }

  যদি (epollfd >= 0)
    {
      size_t available = ring_বাফার_get_space_available (channel->rb);
      size_t used = ring_বাফার_get_তথ্য_available (channel->rb);
      পূর্ণসংখ্যা events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_প্রবেশ_eagain ? EPOLLET : 0)) : 0;
      যদি (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      যদি (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          channel->outfd_epoll_events = events;
        }
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
open_proc (libcrun_error_t *err)
{
  পূর্ণসংখ্যা fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close পূর্ণসংখ্যা fsfd = -1;
  পূর্ণসংখ্যা ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  যদি (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ফাঁকা, ফাঁকা, 0);
      যদি (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          যদি (fd >= 0)
            ফেরত fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  যদি (fd < 0)
    ফেরত crun_make_error (err, errno, "open `/proc`");
  ফেরত fd;
}

পূর্ণসংখ্যা
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  যদি (! container)
    ফেরত open_proc (err);

  যদি (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  ফেরত container->proc_fd;
}

পূর্ণসংখ্যা
libcrun_open_proc_file (libcrun_container_t *container, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা flags, libcrun_error_t *err)
{
  পূর্ণসংখ্যা proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  যদি (proc_fd < 0)
    ফেরত proc_fd;

  fd = অস্থায়ী_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  যদি (! container)
    অস্থায়ী_FAILURE_RETRY (close (proc_fd));

  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  ফেরত fd;
}

পূর্ণসংখ্যা
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা flags, libcrun_error_t *err)
{
  cleanup_free অক্ষর *full_path = ফাঁকা;
  xasprintf (&full_path, "%d/%s", pid, path);
  ফেরত libcrun_open_proc_file (container, full_path, flags, err);
}
