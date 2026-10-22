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
#அந்தர்பாவ <config.h>
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "ring_buffer.h"
#அந்தர்பாவ <stdarg.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <libgen.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/un.h>
#அந்தர்பாவ <sys/socket.h>
#அந்தர்பாவ <sys/signalfd.h>
#அந்தர்பாவ <sys/epoll.h>
#அந்தர்பாவ <sys/syscall.h>
#அந்தர்பாவ <sys/wait.h>
#அந்தர்பாவ <pwd.h>
#அந்தர்பாவ <grp.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <sys/time.h>
#அந்தர்பாவ <sys/xattr.h>
#அந்தர்பாவ <sys/sysmacros.h>
#அந்தர்பாவ <sys/vfs.h>
#அந்தர்பாவ <linux/magic.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <sys/mman.h>
#ifdef HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#endif
#எனில் HAVE_STDATOMIC_H
#  include <stdatomic.h>
#இல்லையெனில்
#  define atomic_long நிலையற்ற long
#  define atomic_int நிலையற்ற முழு
#endif
#அந்தர்பாவ "syscalls.h"

#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif
#ifndef RESOLVE_IN_வேர்
#  define RESOLVE_IN_வேர் 0x10
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

முழு
crun_path_exists (மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  முழு ret = access (path, F_OK);
  எனில் (ret < 0)
    {
      எனில் (errno == ENOENT)
        திரும்பு 0;
      திரும்பு crun_make_error (err, errno, "access `%s`", path);
    }
  திரும்பு 1;
}

முழு
xasprintf (எழுத்து **str, மாறிலி எழுத்து *fmt, ...)
{
  முழு ret;
  va_list args_list;

  va_start (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  எனில் (UNLIKELY (ret < 0))
    OOM ();

  va_end (args_list);
  திரும்பு ret;
}

முழு
write_file_at_with_flags (முழு dirfd, முழு flags, mode_t mode, மாறிலி எழுத்து *name, மாறிலி வெறுமை *data, size_t len, libcrun_error_t *err)
{
  cleanup_close முழு fd = -1;
  முழு ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s` for writing", name);

  ret = safe_write (fd, name, data, len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு (len > INT_MAX) ? INT_MAX : (முழு) len;
}

முழு
detach_process ()
{
  pid_t pid;
  எனில் (setsid () < 0)
    திரும்பு -1;
  pid = fork ();
  எனில் (pid < 0)
    திரும்பு -1;
  எனில் (pid != 0)
    _safe_exit (EXIT_SUCCESS);
  திரும்பு 0;
}

முழு
get_file_type_fd (முழு fd, mode_t *mode)
{
  கட்டமைப்பு stat st;
  முழு ret;

#ifdef HAVE_STATX
  கட்டமைப்பு statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno == ENOSYS || errno == EINVAL)
        செல் fallback;

      திரும்பு ret;
    }
  *mode = stx.stx_mode;
  திரும்பு ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *mode = st.st_mode;
  திரும்பு ret;
}

முழு
get_file_type_at (முழு dirfd, mode_t *mode, bool nofollow, மாறிலி எழுத்து *path)
{
  முழு empty_path = path == வெற்று ? AT_EMPTY_PATH : 0;
  கட்டமைப்பு stat st;
  முழு ret;

#ifdef HAVE_STATX
  கட்டமைப்பு statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno == ENOSYS || errno == EINVAL)
        செல் fallback;

      திரும்பு ret;
    }
  *mode = stx.stx_mode;
  திரும்பு ret;

fallback:
#endif
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  திரும்பு ret;
}

முழு
get_file_type (mode_t *mode, bool nofollow, மாறிலி எழுத்து *path)
{
  திரும்பு get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

முழு
create_file_if_missing_at (முழு dirfd, மாறிலி எழுத்து *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close முழு fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  எனில் (fd_write < 0)
    {
      முழு saved_errno = errno;
      mode_t tmp_mode;
      முழு ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      எனில் (ret == 0 && S_ISREG (tmp_mode))
        திரும்பு 0;

      திரும்பு crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  திரும்பு 0;
}

நிலையான முழு
ensure_directory_internal_at (முழு dirfd, எழுத்து *path, size_t len, முழு mode, libcrun_error_t *err)
{
  எழுத்து *it = path + len;
  முழு ret = 0;
  bool parent_created = false;

  க்கு (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      எனில் (ret == 0 || errno == EEXIST)
        திரும்பு 0;

      முழு saved_errno = errno;
      எனில் (parent_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = வெற்று;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          எனில் (ret > 0)
            திரும்பு 0;
          எனில் (ret < 0)
            crun_error_release (&tmp_err);

          திரும்பு crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      போது (it > path && *it != '/')
        {
          it--;
          len--;
        }
      எனில் (it == path)
        திரும்பு 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      parent_created = true;
    }
  திரும்பு ret;
}

முழு
crun_ensure_directory_at (முழு dirfd, மாறிலி எழுத்து *path, முழு mode, bool nofollow, libcrun_error_t *err)
{
  முழு ret;
  cleanup_free எழுத்து *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, strlen (tmp), mode, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (ret == 0)
    திரும்பு crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  திரும்பு 0;
}

நிலையான முழு
check_fd_is_path (மாறிலி எழுத்து *path, முழு fd, மாறிலி எழுத்து *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  size_t path_len = strlen (path);
  எழுத்து link[PATH_MAX];
  முழு ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, அளவு (link)));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "readlink `%s`", fdname);

  எனில் (((size_t) ret) != path_len || memcmp (link, path, path_len))
    திரும்பு crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  திரும்பு 0;
}

நிலையான முழு
check_fd_under_path (மாறிலி எழுத்து *rootfs, size_t rootfslen, முழு fd, மாறிலி எழுத்து *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  எழுத்து link[PATH_MAX];
  முழு ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  எனில் (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    திரும்பு 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = TEMP_FAILURE_RETRY (readlink (fdpath, link, அளவு (link)));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "readlink `%s`", fdname);

  எனில் (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    திரும்பு crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  திரும்பு 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
நிலையான வெறுமை
close_and_replace (முழு *oldfd, முழு newfd)
{
  எனில் (*oldfd >= 0)
    TEMP_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
எழுத்து *chroot_realpath (மாறிலி எழுத்து *chroot, மாறிலி எழுத்து *path, எழுத்து resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
நிலையான முழு
safe_openat_fallback (முழு dirfd, மாறிலி எழுத்து *rootfs, மாறிலி எழுத்து *path, முழு flags,
                      முழு mode, libcrun_error_t *err)
{
  cleanup_free எழுத்து *parent_path = வெற்று;
  மாறிலி எழுத்து *last_component = வெற்று;
  மாறிலி எழுத்து *orig_path = path;
  மாறிலி எழுத்து *path_in_chroot;
  cleanup_close முழு fd = -1;
  எழுத்து resolved[PATH_MAX];
  எழுத்து buffer[PATH_MAX];
  size_t rootfs_len = safe_strlen (rootfs);
  முழு ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  எனில் (flags & O_NOFOLLOW)
    {
      எழுத்து *sep;

      parent_path = xstrdup (path);
      sep = strrchr (parent_path, '/');
      எனில் (sep == வெற்று)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      இல்லையெனில் எனில் (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      எனில் (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, buffer);
  எனில் (path_in_chroot == வெற்று)
    திரும்பு crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  எனில் (rootfs_len > 0 && strncmp (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  எனில் (last_component)
    {
      ret = snprintf (resolved, அளவு (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      எனில் (UNLIKELY (ret >= (முழு) அளவு (resolved)))
        திரும்பு crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  எனில் (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      திரும்பு ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = fd;
  fd = -1;
  திரும்பு ret;
}

முழு
safe_openat (முழு dirfd, மாறிலி எழுத்து *rootfs, மாறிலி எழுத்து *path, முழு flags, முழு mode,
             libcrun_error_t *err)
{
  நிலையான bool openat2_supported = true;
  முழு retries = MAX_OPENAT2_EAGAIN_RETRIES;
  முழு ret;

  எனில் (is_empty_string (path))
    {
      cleanup_close முழு fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      எனில் (is_empty_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, mode);
      எனில் (UNLIKELY (fd < 0))
        திரும்பு crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      எனில் (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      ret = fd;
      fd = -1;
      திரும்பு ret;
    }

  எனில் (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_வேர்);
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (errno == EINTR)
            செல் repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          எனில் (errno == EAGAIN && retries-- > 0)
            செல் repeat;
          எனில் (errno == ENOSYS)
            openat2_supported = false;
          எனில் (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            திரும்பு safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          திரும்பு crun_make_error (err, errno, "openat2 `%s`", path);
        }

      திரும்பு ret;
    }

  திரும்பு safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (முழு dfd, மாறிலி எழுத்து *name, எழுத்து **buffer, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_size = hint > 0 ? hint + 1 : 512;
  cleanup_free எழுத்து *tmp_buf = வெற்று;
  ssize_t size;

  செய்
    {
      எனில் (tmp_buf != வெற்று)
        buf_size += 256;

      tmp_buf = xrealloc (tmp_buf, buf_size);

      size = readlinkat (dfd, name, tmp_buf, buf_size);
      எனில் (UNLIKELY (size < 0))
        திரும்பு crun_make_error (err, errno, "readlink `%s`", name);
  } போது (size == buf_size);

  /* Always NUL terminate the buffer.  */
  tmp_buf[size] = '\0';

  /* Move ownership to BUFFER.  */
  *buffer = tmp_buf;
  tmp_buf = வெற்று;

  திரும்பு size;
}

நிலையான முழு crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, முழு dirfd, மாறிலி எழுத்து *dirpath,
                                மாறிலி எழுத்து *path, முழு mode, முழு max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
நிலையான முழு
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, முழு dirfd, மாறிலி எழுத்து *dirpath, முழு cwd,
                                 மாறிலி எழுத்து *npath, மாறிலி எழுத்து *cur, மாறிலி எழுத்து *rest,
                                 முழு mode, முழு max_readlinks, libcrun_error_t *err)
{
  cleanup_free எழுத்து *target = வெற்று;
  cleanup_free எழுத்து *new_path = வெற்று;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (முழு) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  திரும்பு crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

நிலையான முழு
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, முழு dirfd, மாறிலி எழுத்து *dirpath,
                     மாறிலி எழுத்து *path, முழு mode, முழு max_readlinks, libcrun_error_t *err)
{
  cleanup_close முழு wd_cleanup = -1;
  cleanup_free எழுத்து *npath = வெற்று;
  bool last_component = false;
  size_t depth = 0;
  எழுத்து *cur;
  எழுத்து *it;
  முழு cwd;
  முழு ret;

  எனில் (max_readlinks <= 0)
    திரும்பு crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  எனில் (*path == '\0')
    {
      எனில் (do_open)
        திரும்பு open (dirpath, O_CLOEXEC | O_PATH, 0);
      திரும்பு 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  போது (cur)
    {
      எனில் (it)
        *it = '\0';
      இல்லையெனில்
        last_component = true;

      எனில் (cur[0] == '\0')
        நிறுத்து;

      எனில் (strcmp (cur, ".") == 0)
        செல் next;
      இல்லையெனில் எனில் (strcmp (cur, ".."))
        depth++;
      இல்லையெனில்
        {
          எனில் (depth)
            depth--;
          இல்லையெனில்
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              செல் next;
            }
        }

      எனில் (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          எனில் (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              எனில் (errno == ELOOP)
                திரும்பு crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, வெற்று,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              எனில் (ret < 0)
                திரும்பு crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          எனில் (do_open)
            திரும்பு ret;

          close_and_replace (&wd_cleanup, ret);
          திரும்பு 0;
        }

      ret = mkdirat (cwd, cur, mode);
      எனில் (ret < 0)
        {
          mode_t st_mode;

          எனில் (errno != EEXIST)
            திரும்பு crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          எனில் (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            திரும்பு crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : வெற்று, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      எனில் (UNLIKELY (cwd < 0))
        திரும்பு crun_error_wrap (err, "creating `/%s`", path);

      எனில் (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, வெற்று);
          எனில் (UNLIKELY (ret < 0))
            {
              முழு saved_errno = errno;

              close (cwd);
              திரும்பு crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          எனில் ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              திரும்பு crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    next:
      எனில் (it == வெற்று)
        நிறுத்து;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  எனில் (do_open)
    {
      எனில் (cwd == dirfd)
        {
          ret = dup (dirfd);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "dup `%s`", dirpath);
          திரும்பு ret;
        }

      wd_cleanup = -1;
      திரும்பு cwd;
    }

  திரும்பு 0;
}

முழு
crun_safe_create_and_open_ref_at (bool dir, முழு dirfd, மாறிலி எழுத்து *dirpath, மாறிலி எழுத்து *path, முழு mode, libcrun_error_t *err)
{
  முழு ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  எனில் (LIKELY (ret >= 0))
    திரும்பு ret;

  crun_error_release (err);
  திரும்பு crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

முழு
crun_safe_ensure_directory_at (முழு dirfd, மாறிலி எழுத்து *dirpath, மாறிலி எழுத்து *path, முழு mode,
                               libcrun_error_t *err)
{
  திரும்பு crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

முழு
crun_safe_ensure_file_at (முழு dirfd, மாறிலி எழுத்து *dirpath, மாறிலி எழுத்து *path, முழு mode,
                          libcrun_error_t *err)
{
  திரும்பு crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

முழு
crun_ensure_directory (மாறிலி எழுத்து *path, முழு mode, bool nofollow, libcrun_error_t *err)
{
  திரும்பு crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

நிலையான முழு
get_file_size (முழு fd, off_t *size)
{
  கட்டமைப்பு stat st;
  முழு ret;
#ifdef HAVE_STATX
  கட்டமைப்பு statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_SIZE, &stx);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno == ENOSYS || errno == EINVAL)
        செல் fallback;
      திரும்பு ret;
    }
  *size = stx.stx_size;

  திரும்பு ret;

fallback:
#endif
  ret = fstat (fd, &st);
  *size = st.st_size;
  திரும்பு ret;
}

முழு
crun_dir_p_at (முழு dirfd, மாறிலி எழுத்து *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  முழு ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "stat `%s`", path);

  திரும்பு S_ISDIR (mode) ? 1 : 0;
}

முழு
crun_dir_p (மாறிலி எழுத்து *path, bool nofollow, libcrun_error_t *err)
{
  திரும்பு crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
நிலையான முழு
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free எழுத்து *buffer = வெற்று;
  size_t len;
  முழு ret;

  ret = read_all_file ("/proc/self/uid_map", &buffer, &len, err);
  எனில் (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      எனில் (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          திரும்பு 0;
        }
      திரும்பு ret;
    }

  திரும்பு strstr (buffer, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
நிலையான bool
running_in_openvz ()
{
  எனில் (access ("/proc/vz", F_OK) < 0)
    திரும்பு false;

  திரும்பு access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

முழு
check_running_in_user_namespace (libcrun_error_t *err)
{
  நிலையான atomic_int run_in_userns = -1;
  கட்டமைப்பு stat st;
  முழு ret;

  ret = run_in_userns;
  எனில் (ret >= 0)
    திரும்பு ret;

  ret = stat ("/proc/self/ns/user", &st);
  எனில் (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      எனில் (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          திரும்பு run_in_userns;
        }

      எனில் (! running_in_openvz ())
        {
          run_in_userns = 1;
          திரும்பு run_in_userns;
        }
    }
  இல்லையெனில் எனில் (UNLIKELY (errno != ENOENT))
    திரும்பு crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  run_in_userns = ret;
  திரும்பு run_in_userns;
}

நிலையான size_t
get_page_size ()
{
  நிலையான atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  எனில் (pagesize == 0)
    {
      pagesize = (long) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  திரும்பு (size_t) pagesize;
}

நிலையான முழு selinux_enabled = -1;
நிலையான முழு apparmor_enabled = -1;

முழு
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free எழுத்து *out = வெற்று;
  cleanup_close முழு fd = -1;
  size_t len;
  முழு ret;

  எனில் (selinux_enabled >= 0)
    திரும்பு selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு fd;

  ret = read_all_fd_with_size_hint (fd, "mounts", &out, &len, get_page_size (), err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  selinux_enabled = strstr (out, "selinux") ? 1 : 0;

  திரும்பு selinux_enabled;
}

முழு
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close முழு fd = -1;
  முழு size;
  எழுத்து buf[1];

  எனில் (apparmor_enabled >= 0)
    திரும்பு apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  எனில் (fd == -1)
    {
      எனில் (errno == ENOENT)
        {
          apparmor_enabled = 0;
          திரும்பு 0;
        }

      திரும்பு crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  size = TEMP_FAILURE_RETRY (பட (fd, buf, அளவு (buf)));

  apparmor_enabled = size > 0 && buf[0] == 'Y' ? 1 : 0;

  திரும்பு apparmor_enabled;
}

நிலையான முழு
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  எனில் (selinux_enabled < 0)
    திரும்பு crun_make_error (err, 0, "SELinux is not initialized correctly");
  திரும்பு selinux_enabled;
}

முழு
add_selinux_mount_label (எழுத்து **retlabel, மாறிலி எழுத்து *data, மாறிலி எழுத்து *label, மாறிலி எழுத்து *context_type, libcrun_error_t *err)
{
  முழு ret;

  ret = libcrun_is_selinux_enabled (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (label && ret)
    {
      எனில் (data && *data)
        xasprintf (retlabel, "%s,%s=\"%s\"", data, context_type, label);
      இல்லையெனில்
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      திரும்பு 0;
    }
  *retlabel = xstrdup (data);
  திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *
lsm_attr_path (libcrun_container_t *container, மாறிலி எழுத்து *lsm, மாறிலி எழுத்து *fname, libcrun_error_t *err)
{
  cleanup_close முழு attr_dirfd = -1;
  cleanup_close முழு lsm_dirfd = -1;
  எழுத்து *attr_path = வெற்று;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  எனில் (UNLIKELY (attr_dirfd < 0))
    திரும்பு வெற்று;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  எனில் (lsm != வெற்று)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      எனில் (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          திரும்பு வெற்று;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  திரும்பு attr_path;
}

நிலையான முழு
check_proc_super_magic (முழு fd, மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  கட்டமைப்பு statfs sfs;

  முழு ret = fstatfs (fd, &sfs);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "statfs `%s`", path);

  எனில் (sfs.f_type != PROC_SUPER_MAGIC)
    திரும்பு crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  திரும்பு 0;
}

நிலையான முழு
set_security_attr (libcrun_container_t *container, மாறிலி எழுத்து *lsm, மாறிலி எழுத்து *fname, மாறிலி எழுத்து *data, libcrun_error_t *err)
{
  முழு ret;

  cleanup_free மாறிலி எழுத்து *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close முழு fd = -1;

  எனில் (UNLIKELY (attr_path == வெற்று))
    திரும்பு -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  // Write out data
  ret = TEMP_FAILURE_RETRY (லிக (fd, data, strlen (data)));
  எனில் (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      எனில் (errno == ENOENT && lsm != வெற்று && strcmp (lsm, "apparmor") == 0)
        {
          மாறிலி எழுத்து *profile = strchr (data, ' ');

          திரும்பு crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : data);
        }
      திரும்பு crun_make_error (err, errno, "write to file `%s`", attr_path);
    }

  திரும்பு 0;
}

முழு
set_selinux_label (libcrun_container_t *container, மாறிலி எழுத்து *label, bool now, libcrun_error_t *err)
{
  முழு ret;

  ret = libcrun_is_selinux_enabled (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (ret)
    திரும்பு set_security_attr (container, வெற்று, now ? "current" : "exec", label, err);
  திரும்பு 0;
}

நிலையான முழு
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  எனில் (apparmor_enabled < 0)
    திரும்பு crun_make_error (err, 0, "AppArmor is not initialized correctly");
  திரும்பு apparmor_enabled;
}

நிலையான முழு
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#define UNCONFINED "unconfined"
#define UNCONFINED_LEN (ssize_t) (அளவு (UNCONFINED) - 1)
  cleanup_free மாறிலி எழுத்து *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close முழு fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  எழுத்து buf[UNCONFINED_LEN + 1];

  எனில் (UNLIKELY (attr_path == வெற்று))
    திரும்பு -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு fd;

  எனில் (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    திரும்பு -1;

  ssize_t bytes_read = TEMP_FAILURE_RETRY (பட (fd, buf, அளவு (buf)));
  எனில் (UNLIKELY (bytes_read < 0))
    திரும்பு crun_make_error (err, errno, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  எனில் (bytes_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    திரும்பு 1;
  எனில் (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    திரும்பு 0;
  திரும்பு 1;
#undef UNCONFINED
#undef UNCONFINED_LEN
}

முழு
set_apparmor_profile (libcrun_container_t *container, மாறிலி எழுத்து *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  முழு ret;

  ret = libcrun_is_apparmor_enabled (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (ret)
    {
      cleanup_free எழுத்து *buf = வெற்று;
      ret = is_current_process_confined (container, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      திரும்பு set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  திரும்பு 0;
}

நிலையான முழு
read_all_fd_internal (முழு fd, மாறிலி எழுத்து *description, எழுத்து **out, size_t *len, size_t size_hint, size_t max_size, libcrun_error_t *err)
{
  cleanup_free எழுத்து *buf = வெற்று;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t size = 0;
  முழு ret;

  எனில் (size_hint)
    allocated = size_hint;
  இல்லையெனில்
    {
      ret = get_file_size (fd, &size);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "stat `%s`", description);

      எனில் (max_size && (size_t) size > max_size)
        திரும்பு crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      allocated = size == 0 ? 1023 : size;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  போது ((size && nread < (size_t) size) || size == 0)
    {
      ret = TEMP_FAILURE_RETRY (பட (fd, buf + nread, allocated - nread));
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "read from file `%s`", description);

      எனில் (ret == 0)
        நிறுத்து;

      nread += ret;

      எனில் (max_size && nread > max_size)
        திரும்பு crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_size);

      எனில் (nread == allocated)
        {
          எனில் (size)
            நிறுத்து;

          எனில் (pagesize == 0)
            pagesize = get_page_size ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  எனில் (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      எழுத்து *tmp = realloc (buf, nread + 1);
      எனில் (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = வெற்று;
  எனில் (len)
    *len = nread;
  திரும்பு 0;
}

முழு
read_all_fd_with_size_hint (முழு fd, மாறிலி எழுத்து *description, எழுத்து **out, size_t *len, size_t size_hint, libcrun_error_t *err)
{
  திரும்பு read_all_fd_internal (fd, description, out, len, size_hint, 0, err);
}

முழு
read_all_fd_with_max_size (முழு fd, மாறிலி எழுத்து *description, எழுத்து **out, size_t *len, size_t max_size, libcrun_error_t *err)
{
  திரும்பு read_all_fd_internal (fd, description, out, len, 0, max_size, err);
}

முழு
read_all_file_at (முழு dirfd, மாறிலி எழுத்து *path, எழுத்து **out, size_t *len, libcrun_error_t *err)
{
  cleanup_close முழு fd = -1;

  fd = TEMP_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", path);

  திரும்பு read_all_fd (fd, path, out, len, err);
}

முழு
read_all_file (மாறிலி எழுத்து *path, எழுத்து **out, size_t *len, libcrun_error_t *err)
{
  எனில் (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  திரும்பு read_all_file_at (AT_FDCWD, path, out, len, err);
}

முழு
get_realpath_to_file (முழு dirfd, மாறிலி எழுத்து *path_name, எழுத்து **absolute_path, libcrun_error_t *err)
{
  cleanup_close முழு targetfd = -1;

  targetfd = TEMP_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  எனில் (UNLIKELY (targetfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", path_name);
  இல்லையெனில்
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      எனில் (UNLIKELY (len < 0))
        திரும்பு crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  திரும்பு 0;
}

முழு
open_unix_domain_client_socket (மாறிலி எழுத்து *path, முழு dgram, libcrun_error_t *err)
{
  கட்டமைப்பு sockaddr_un addr = {};
  முழு ret;
  proc_fd_path_t name_buf;
  cleanup_close முழு destfd = -1;
  cleanup_close முழு fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "create UNIX socket");

  எனில் (strlen (path) >= அளவு (addr.sun_path))
    {
      destfd = open (path, O_PATH | O_CLOEXEC);
      எனில் (UNLIKELY (destfd < 0))
        திரும்பு crun_make_error (err, errno, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  size_t path_len = strlen (path);
  எனில் (path_len >= அளவு (addr.sun_path))
    திரும்பு crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  memcpy (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (கட்டமைப்பு sockaddr *) &addr, அளவு (addr));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  திரும்பு ret;
}

முழு
open_unix_domain_socket (மாறிலி எழுத்து *path, முழு dgram, libcrun_error_t *err)
{
  கட்டமைப்பு sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  முழு ret;
  cleanup_close முழு dirfd = -1;
  cleanup_close முழு fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "create UNIX socket");

  எனில் (strlen (path) >= அளவு (addr.sun_path))
    {
      cleanup_free எழுத்து *dpath = xstrdup (path);
      cleanup_free எழுத்து *bpath = xstrdup (path);
      மாறிலி எழுத்து *parent_dir = dirname (dpath);
      மாறிலி எழுத்து *base = basename (bpath);
      முழு n;

      dirfd = open (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      எனில் (UNLIKELY (dirfd < 0))
        திரும்பு crun_make_error (err, errno, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, அளவு (addr.sun_path), "%s/%s", name_buf, base);
      எனில் (n < 0 || (size_t) n >= அளவு (addr.sun_path))
        திரும்பு crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  இல்லையெனில்
    {
      size_t path_len = strlen (path);
      memcpy (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (கட்டமைப்பு sockaddr *) &addr, அளவு (addr));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "bind socket to `%s`", path);

  எனில் (! dgram)
    {
      ret = listen (fd, 1);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  திரும்பு ret;
}

முழு
send_fd_to_socket (முழு server, முழு fd, libcrun_error_t *err)
{
  திரும்பு send_fd_to_socket_with_payload (server, fd, வெற்று, 0, err);
}

முழு
send_fd_to_socket_with_payload (முழு server, முழு fd, மாறிலி எழுத்து *payload, size_t payload_len, libcrun_error_t *err)
{
  முழு ret;
  கட்டமைப்பு cmsghdr *cmsg = வெற்று;
  கட்டமைப்பு iovec iov[2];
  கட்டமைப்பு msghdr செய்தி = {};
  எழுத்து ctrl_buf[CMSG_SPACE (1 + அளவு (முழு))] = {};
  எழுத்து data[1];

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = அளவு (data);

  எனில் (payload_len > 0)
    {
      iov[0].iov_base = (வெறுமை *) payload;
      iov[0].iov_len = payload_len;
    }

  செய்தி.செய்தி_name = வெற்று;
  செய்தி.செய்தி_namelen = 0;
  செய்தி.செய்தி_iov = iov;
  செய்தி.செய்தி_iovlen = 1;
  செய்தி.செய்தி_controllen = CMSG_SPACE (அளவு (முழு));
  செய்தி.செய்தி_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&செய்தி);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (அளவு (முழு));

  *((முழு *) CMSG_DATA (cmsg)) = fd;

  ret = TEMP_FAILURE_RETRY (sendmsg (server, &செய்தி, 0));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "sendmsg");
  திரும்பு 0;
}

முழு
receive_fd_from_socket_with_payload (முழு from, எழுத்து *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close முழு fd = -1;
  முழு ret;
  கட்டமைப்பு iovec iov[1];
  கட்டமைப்பு msghdr செய்தி = {};
  எழுத்து ctrl_buf[CMSG_SPACE (அளவு (முழு))] = {};
  எழுத்து data[1];
  கட்டமைப்பு cmsghdr *cmsg;

  data[0] = ' ';
  iov[0].iov_base = data;
  iov[0].iov_len = அளவு (data);

  எனில் (payload_len > 0)
    {
      iov[0].iov_base = (வெறுமை *) payload;
      iov[0].iov_len = payload_len;
    }

  செய்தி.செய்தி_name = வெற்று;
  செய்தி.செய்தி_namelen = 0;
  செய்தி.செய்தி_iov = iov;
  செய்தி.செய்தி_iovlen = 1;
  செய்தி.செய்தி_controllen = CMSG_SPACE (அளவு (முழு));
  செய்தி.செய்தி_control = ctrl_buf;

  ret = TEMP_FAILURE_RETRY (recvmsg (from, &செய்தி, 0));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "recvmsg");
  எனில் (UNLIKELY (ret == 0))
    திரும்பு crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&செய்தி);
  எனில் (cmsg == வெற்று)
    திரும்பு crun_make_error (err, 0, "no msg received");
  memcpy (&fd, CMSG_DATA (cmsg), அளவு (fd));

  ret = fd;
  fd = -1;
  திரும்பு ret;
}

முழு
receive_fd_from_socket (முழு from, libcrun_error_t *err)
{
  திரும்பு receive_fd_from_socket_with_payload (from, வெற்று, 0, err);
}

முழு
create_socket_pair (முழு *pair, libcrun_error_t *err)
{
  முழு ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "socketpair");
  திரும்பு 0;
}

முழு
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  முழு ret = signalfd (-1, mask, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "signalfd");
  திரும்பு ret;
}

நிலையான முழு
epoll_helper_toggle (முழு epollfd, முழு fd, முழு events, libcrun_error_t *err)
{
  கட்டமைப்பு epoll_event ev = {};
  bool add = events != 0;
  முழு ret;

  ev.events = events;
  ev.data.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  எனில் (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  எனில் (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      எனில் (errno == ENOENT || errno == EPERM)
        திரும்பு 0;
      திரும்பு crun_make_error (err, errno, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  திரும்பு 0;
}

முழு
epoll_helper (முழு *in_fds, முழு *in_levelfds, முழு *out_fds, முழு *out_levelfds, libcrun_error_t *err)
{
  கட்டமைப்பு epoll_event ev;
  cleanup_close முழு epollfd = -1;
  முழு ret;

  முழு *it;
  epollfd = epoll_create1 (0);
  எனில் (UNLIKELY (epollfd < 0))
    திரும்பு crun_make_error (err, errno, "epoll_create1");

#define ADD_FDS(FDS, EVENTS)                                            \
  க்கு (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.data.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      எனில் (UNLIKELY (ret < 0))                                           \
        திரும்பு crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  எனில் (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  எனில் (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  எனில் (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  எனில் (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  திரும்பு ret;
}

முழு
copy_from_fd_to_fd (முழு src, முழு dst, முழு consume, libcrun_error_t *err)
{
  முழு ret;
  ssize_t nread;
  size_t pagesize = get_page_size ();
#ifdef HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#endif
  செய்
    {
      cleanup_free எழுத்து *buffer = வெற்று;
      ssize_t remaining;

#ifdef HAVE_COPY_FILE_RANGE
      எனில் (can_copy_file_range)
        {
          nread = copy_file_range (src, வெற்று, dst, வெற்று, pagesize, 0);
          எனில் (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              செல் fallback;
            }
          எனில் (consume && nread < 0 && errno == EAGAIN)
            திரும்பு 0;
          எனில் (nread < 0 && errno == EIO)
            திரும்பு 0;
          எனில் (UNLIKELY (nread < 0))
            திரும்பு crun_make_error (err, errno, "copy_file_range");
          தொடர்;
        }
    fallback:
#endif

      buffer = xmalloc (pagesize);
      nread = TEMP_FAILURE_RETRY (பட (src, buffer, pagesize));
      எனில் (consume && nread < 0 && errno == EAGAIN)
        திரும்பு 0;
      எனில் (nread < 0 && errno == EIO)
        திரும்பு 0;
      எனில் (UNLIKELY (nread < 0))
        திரும்பு crun_make_error (err, errno, "read");

      remaining = nread;
      போது (remaining)
        {
          ret = TEMP_FAILURE_RETRY (லிக (dst, buffer + nread - remaining, remaining));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "write");
          remaining -= ret;
        }
  } போது (consume && nread);

  திரும்பு 0;
}

முழு
run_process (எழுத்து **args, libcrun_error_t *err)
{
  pid_t pid = fork ();
  எனில் (UNLIKELY (pid < 0))
    திரும்பு crun_make_error (err, errno, "fork");
  எனில் (pid)
    {
      முழு r, status;
      r = TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
      எனில் (r < 0)
        திரும்பு crun_make_error (err, errno, "waitpid");
      எனில் (WIFEXITED (status) || WIFSIGNALED (status))
        திரும்பு WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (EXIT_FAILURE);
}

#ifndef HAVE_FGETPWENT_R
நிலையான unsigned
atou (எழுத்து **s)
{
  unsigned x;
  க்கு (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  திரும்பு x;
}

முழு
fgetpwent_r (FILE *f, கட்டமைப்பு passwd *pw, எழுத்து *வரி, size_t size, கட்டமைப்பு passwd **res)
{
  எழுத்து *s;
  முழு rv = 0;
  க்கு (;;)
    {
      வரி[size - 1] = '\xff';
      எனில் ((fgets (வரி, size, f) == வெற்று) || ferror (f) || வரி[size - 1] != '\xff')
        {
          rv = (வரி[size - 1] != '\xff') ? ERANGE : ENOENT;
          வரி = 0;
          pw = 0;
          நிறுத்து;
        }
      வரி[strcspn (வரி, "\n")] = 0;

      s = வரி;
      pw->pw_name = s++;
      எனில் (! (s = strchr (s, ':')))
        தொடர்;

      *s++ = 0;
      pw->pw_passwd = s;
      எனில் (! (s = strchr (s, ':')))
        தொடர்;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      எனில் (*s != ':')
        தொடர்;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      எனில் (*s != ':')
        தொடர்;

      *s++ = 0;
      pw->pw_gecos = s;
      எனில் (! (s = strchr (s, ':')))
        தொடர்;

      *s++ = 0;
      pw->pw_dir = s;
      எனில் (! (s = strchr (s, ':')))
        தொடர்;

      *s++ = 0;
      pw->pw_shell = s;
      நிறுத்து;
    }
  *res = pw;
  எனில் (rv)
    errno = rv;
  திரும்பு rv;
}
#endif

முழு
set_home_env (uid_t id)
{
  கட்டமைப்பு passwd pwd;
  cleanup_free எழுத்து *buf = வெற்று;
  long buf_size;
  cleanup_file FILE *stream = வெற்று;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  எனில் (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);

  stream = fopen ("/etc/passwd", "re");
  எனில் (stream == வெற்று)
    திரும்பு -1;

  க்கு (;;)
    {
      கட்டமைப்பு passwd *ret_pw = வெற்று;

      முழு ret = fgetpwent_r (stream, &pwd, buf, buf_size, &ret_pw);
      எனில் (UNLIKELY (ret != 0))
        {
          எனில் (ret == EINTR)
            தொடர்;

          எனில் (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              திரும்பு -1;
            }

          buf_size *= 2;
          buf = xrealloc (buf, buf_size);
          தொடர்;
        }

      எனில் (ret_pw && ret_pw->pw_uid == id)
        {
          எனில் (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          இல்லையெனில்
            திரும்பு 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
நிலையான முழு
getsubidrange (uid_t id, முழு is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *உள்ளீடு = வெற்று;
  cleanup_free எழுத்து *lineptr = வெற்று;
  size_t lenlineptr = 0, len_name;
  long buf_size;
  cleanup_free எழுத்து *buf = வெற்று;
  மாறிலி எழுத்து *name;
  கட்டமைப்பு passwd pwd;

  buf_size = sysconf (_SC_GETPW_R_SIZE_MAX);
  எனில் (buf_size < 0)
    buf_size = 1024;

  buf = xmalloc (buf_size);
  க்கு (;;)
    {
      முழு ret;
      கட்டமைப்பு passwd *ret_pw = வெற்று;

      ret = getpwuid_r (id, &pwd, buf, buf_size, &ret_pw);
      எனில் (LIKELY (ret == 0))
        {
          எனில் (ret_pw)
            {
              name = ret_pw->pw_name;
              நிறுத்து;
            }
          திரும்பு -1;
        }

      எனில் (ret != ERANGE)
        திரும்பு -1;

      buf_size *= 2;
      buf = xrealloc (buf, buf_size);
    }

  len_name = strlen (name);

  உள்ளீடு = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  எனில் (உள்ளீடு == வெற்று)
    திரும்பு -1;

  க்கு (;;)
    {
      எழுத்து *endptr;
      ssize_t பட = getline (&lineptr, &lenlineptr, உள்ளீடு);
      எனில் (பட < 0)
        திரும்பு -1;

      எனில் (பட < (ssize_t) (len_name + 2))
        தொடர்;

      எனில் (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        தொடர்;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      எனில் (endptr >= &lineptr[பட])
        திரும்பு -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      திரும்பு 0;
    }
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

முழு
format_default_id_mapping (எழுத்து **out, uid_t container_id, uid_t host_uid, uid_t host_id, முழு is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free எழுத்து *buffer = வெற்று;
  முழு written = 0;
  முழு ret, remaining;

  *out = வெற்று;

  எனில் (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    திரும்பு 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  buffer = xmalloc (remaining + 1);

  எனில் (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      எனில் (UNLIKELY (ret >= remaining))
        திரும்பு crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (buffer + written, remaining, "%d %d 1\n", container_id, host_id);
  எனில் (UNLIKELY (ret >= remaining))
    திரும்பு crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  எனில் (available)
    {
      ret = snprintf (buffer + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      எனில் (UNLIKELY (ret >= remaining))
        திரும்பு crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = buffer;
  buffer = வெற்று;
  திரும்பு written;
}

நிலையான முழு
unset_cloexec_flag (முழு fd)
{
  முழு flags = fcntl (fd, F_GETFD);
  எனில் (flags == -1)
    திரும்பு -1;

  flags &= ~FD_CLOEXEC;

  திரும்பு fcntl (fd, F_SETFD, flags);
}

நிலையான வெறுமை __attribute__ ((__noreturn__))
run_process_child (எழுத்து *path, எழுத்து **args, மாறிலி எழுத்து *cwd, எழுத்து **envp, முழு pipe_r,
                   முழு pipe_w, முழு out_fd, முழு err_fd, bool can_ignore_chdir_errors)
{
  எழுத்து *tmp_args[] = { path, வெற்று };
  libcrun_error_t err = வெற்று;
  முழு dev_null_fd = -1;
  முழு ret;

  ret = mark_or_close_fds_ge_than (வெற்று, 3, false, &err);
  எனில் (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->செய்தி);

  எனில் (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      எனில் (UNLIKELY (dev_null_fd < 0))
        _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  TEMP_FAILURE_RETRY (close (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  எனில் (out_fd >= 0)
    unset_cloexec_flag (1);
  எனில் (err_fd >= 0)
    unset_cloexec_flag (2);

  எனில் (dev_null_fd >= 0)
    TEMP_FAILURE_RETRY (close (dev_null_fd));
  எனில் (out_fd >= 0)
    TEMP_FAILURE_RETRY (close (out_fd));
  எனில் (err_fd >= 0)
    TEMP_FAILURE_RETRY (close (err_fd));

  எனில் (args == வெற்று)
    args = tmp_args;

  எனில் (cwd && chdir (cwd) < 0)
    {
      எனில் (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        _safe_exit (EXIT_FAILURE);
    }

  execvpe (path, args, envp);
  _safe_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
முழு
run_process_with_stdin_timeout_envp (எழுத்து *path, எழுத்து **args, மாறிலி எழுத்து *cwd, முழு timeout,
                                     எழுத்து **envp, எழுத்து *stdin, size_t stdin_len, முழு out_fd,
                                     முழு err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  முழு stdin_pipe[2];
  pid_t pid;
  முழு ret;
  cleanup_close முழு pipe_r = -1;
  cleanup_close முழு pipe_w = -1;
  sigset_t oldmask, mask;
  முழு r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  எனில் (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  எனில் (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      செல் restore_sig_mask_and_exit;
    }

  எனில் (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = TEMP_FAILURE_RETRY (லிக (pipe_w, stdin, stdin_len));
  எனில் (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      எனில் (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          செல் restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  எனில் (timeout)
    {
      time_t start = time (வெற்று);
      time_t now;
      க்கு (now = start; now - start < timeout; now = time (வெற்று))
        {
          siginfo_t info;
          முழு elapsed = now - start;
          கட்டமைப்பு timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          எனில் (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              செல் restore_sig_mask_and_exit;
            }

          எனில் (info.si_signo == SIGCHLD && info.si_pid == pid)
            செல் read_waitpid;

          எனில் (ret < 0 && errno == EAGAIN)
            செல் timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      செல் restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  எனில் (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  இல்லையெனில்
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  எனில் (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      எனில் (pid > 0)
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, வெற்று);
      எனில் (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  திரும்பு ret;
}

முழு
mark_or_close_fds_ge_than (libcrun_container_t *container, முழு n, bool close_now, libcrun_error_t *err)
{
  cleanup_close முழு cfd = -1;
  cleanup_dir DIR *dir = வெற்று;
  முழு ret;
  முழு fd;
  கட்டமைப்பு dirent *next;

#ifdef ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  எனில் (close_now)
    GCOV_DUMP ();
#endif

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  எனில் (ret == 0)
    {
      எனில் (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      திரும்பு 0;
    }
  எனில் (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    திரும்பு crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  எனில் (UNLIKELY (cfd < 0))
    திரும்பு cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  dir = fdopendir (cfd);
  எனில் (UNLIKELY (dir == வெற்று))
    திரும்பு crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  க்கு (next = readdir (dir); next; next = readdir (dir))
    {
      முழு val;
      மாறிலி எழுத்து *name = next->d_name;
      எனில் (name[0] == '.')
        தொடர்;

      val = strtoll (name, வெற்று, 10);
      எனில் (val < n || val == fd)
        தொடர்;

      எனில் (close_now)
        {
          ret = close (val);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "close fd `%d`", val);

          எனில் (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      இல்லையெனில்
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  திரும்பு 0;
}

வெறுமை
get_current_timestamp (எழுத்து *out, size_t len)
{
  கட்டமைப்பு timeval tv;
  கட்டமைப்பு tm now;
  எழுத்து timestamp[64];

  gettimeofday (&tv, வெற்று);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, அளவு (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (வெறுமை) snprintf (out, len, "%s.%06lldZ", timestamp, (long long முழு) tv.tv_usec);
  out[len - 1] = '\0';
}

முழு
set_blocking_fd (முழு fd, bool blocking, libcrun_error_t *err)
{
  முழு ret, flags = fcntl (fd, F_GETFL, 0);
  எனில் (UNLIKELY (flags < 0))
    திரும்பு crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fcntl");
  திரும்பு 0;
}

முழு
parse_json_file (json_object **out, மாறிலி எழுத்து *jsondata, கட்டமைப்பு parser_context *ctx arg_unused, libcrun_error_t *err)
{
  கட்டமைப்பு json_tokener *tok;
  எண்ணிக்கை json_tokener_error jerr;

  *err = வெற்று;

  எனில் (UNLIKELY (jsondata == வெற்று))
    திரும்பு crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  எனில் (UNLIKELY (tok == வெற்று))
    திரும்பு crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (முழு) strlen (jsondata));
  jerr = json_tokener_get_error (tok);
  எனில் (*out == வெற்று || jerr != json_tokener_success)
    {
      முழு saved_pos = (முழு) tok->char_offset;

      json_tokener_free (tok);
      எனில் (*out)
        {
          json_object_put (*out);
          *out = வெற்று;
        }
      திரும்பு crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  திரும்பு 0;
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
நிலையான முழு
check_access (மாறிலி எழுத்து *path)
{
  முழு ret;
  mode_t mode;

#ifdef HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#இல்லையெனில்
#  define ACCESS(path, mode) (access (path, mode))
#endif

  ret = ACCESS (path, X_OK);
  எனில் (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      எனில் (errno == EACCES)
        {
          முழு saved_errno = errno;
          ret = ACCESS (path, F_OK);
          errno = saved_errno;

          எனில் (ret == 0)
            திரும்பு CHECK_ACCESS_NOT_EXECUTABLE;
        }
      திரும்பு -errno;
    }

  ret = get_file_type (&mode, false, path);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு -errno;

  எனில் (! S_ISREG (mode))
    திரும்பு CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  திரும்பு 0;
}

முழு
find_executable (எழுத்து **out, மாறிலி எழுத்து *executable_path, மாறிலி எழுத்து *cwd, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cwd_executable_path = வெற்று;
  cleanup_free எழுத்து *tmp = வெற்று;
  முழு last_error = -ENOENT;
  எழுத்து *it, *end;
  முழு ret;

  *out = வெற்று;

  எனில் (executable_path == வெற்று || executable_path[0] == '\0')
    திரும்பு crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  எனில் (executable_path[0] != '/' && strchr (executable_path, '/'))
    {
      cleanup_free எழுத்து *cwd_allocated = வெற்று;

      எனில் (cwd == வெற்று)
        {
          cwd_allocated = getcwd (வெற்று, 0);
          எனில் (cwd_allocated == வெற்று)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  எனில் (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      எனில் (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          திரும்பு 0;
        }
      last_error = ret;
      செல் fail;
    }

  end = tmp = xstrdup (getenv ("PATH"));

  போது ((it = strsep (&end, ":")))
    {
      cleanup_free எழுத்து *path = வெற்று;

      எனில் (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          தொடர்;
        }

      ret = check_access (path);
      எனில் (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = வெற்று;
          திரும்பு 0;
        }

      எனில் (ret == -ENOENT)
        தொடர்;

      last_error = ret;
    }

fail:
  தேர்வு (last_error)
    {
    நிலை CHECK_ACCESS_NOT_EXECUTABLE:
      திரும்பு crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    நிலை CHECK_ACCESS_NOT_REGULAR:
      திரும்பு crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    இயல்பு:
      errno = -last_error;
      எனில் (errno == ENOENT)
        திரும்பு crun_make_error (err, errno, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      திரும்பு crun_make_error (err, errno, "open `%s`", executable_path);
    }
}

#ifdef HAVE_FGETXATTR

நிலையான ssize_t
safe_read_xattr (எழுத்து **ret, முழு sfd, மாறிலி எழுத்து *srcname, மாறிலி எழுத்து *name, size_t initial_size, libcrun_error_t *err)
{
  cleanup_free எழுத்து *buffer = வெற்று;
  ssize_t current_size;
  ssize_t s;

  current_size = (ssize_t) initial_size;
  buffer = xmalloc (current_size + 1);

  போது (1)
    {
      s = fgetxattr (sfd, name, buffer, current_size);
      எனில் (UNLIKELY (s < 0))
        திரும்பு crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      எனில் (s < current_size)
        நிறுத்து;

      current_size *= 2;
      buffer = xrealloc (buffer, current_size + 1);
    }

  எனில் (s <= 0)
    திரும்பு s;

  buffer[s] = '\0';

  /* Change owner.  */
  *ret = buffer;
  buffer = வெற்று;

  திரும்பு s;
}

நிலையான ssize_t
copy_xattr (முழு sfd, முழு dfd, மாறிலி எழுத்து *srcname, மாறிலி எழுத்து *destname, libcrun_error_t *err)
{
  cleanup_free எழுத்து *buf = வெற்று;
  ssize_t xattr_len;
  எழுத்து *it;

  xattr_len = flistxattr (sfd, வெற்று, 0);
  எனில் (UNLIKELY (xattr_len < 0))
    {
      எனில் (errno == ENOTSUP)
        திரும்பு 0;

      திரும்பு crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  எனில் (xattr_len == 0)
    திரும்பு 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  எனில் (UNLIKELY (xattr_len < 0))
    திரும்பு crun_make_error (err, errno, "flistxattr `%s`", srcname);

  க்கு (it = buf; it - buf < xattr_len; it += strlen (it) + 1)
    {
      cleanup_free எழுத்து *v = வெற்று;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      எனில் (UNLIKELY (s < 0))
        திரும்பு s;

      s = fsetxattr (dfd, it, v, s, 0);
      எனில் (UNLIKELY (s < 0))
        {
          எனில் (errno == EINVAL || errno == EOPNOTSUPP)
            தொடர்;

          திரும்பு crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  திரும்பு 0;
}

#endif

நிலையான முழு
copy_rec_stat_file_at (முழு dfd, மாறிலி எழுத்து *path, mode_t *mode, off_t *size, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  கட்டமைப்பு stat st;
  முழு ret;

#ifdef HAVE_STATX
  கட்டமைப்பு statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_SIZE | STATX_UID | STATX_GID, &stx);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno == ENOSYS || errno == EINVAL)
        செல் fallback;

      திரும்பு ret;
    }

  *mode = stx.stx_mode;
  *size = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  திரும்பு ret;

fallback:
#endif
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *size = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  திரும்பு ret;
}

முழு
copy_recursive_fd_to_fd (முழு srcdirfd, முழு dfd, மாறிலி எழுத்து *srcname, மாறிலி எழுத்து *destname, libcrun_error_t *err)
{
  cleanup_close முழு destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = வெற்று;
  கட்டமைப்பு dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  எனில் (UNLIKELY (dsrcfd == வெற்று))
    {
      TEMP_FAILURE_RETRY (close (srcdirfd));
      திரும்பு crun_make_error (err, errno, "open directory `%s`", destname);
    }

  க்கு (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close முழு srcfd = -1;
      cleanup_close முழு destfd = -1;
      cleanup_free எழுத்து *target_buf = வெற்று;
      முழு ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      எனில் (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        தொடர்;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      தேர்வு (mode & S_IFMT)
        {
        நிலை S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          எனில் (UNLIKELY (srcfd < 0))
            திரும்பு crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          எனில் (UNLIKELY (destfd < 0))
            திரும்பு crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

#ifdef HAVE_FGETXATTR
          ret = (முழு) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
#endif

          TEMP_FAILURE_RETRY (close (destfd));
          destfd = -1;
          நிறுத்து;

        நிலை S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          எனில் (UNLIKELY (srcfd < 0))
            திரும்பு crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          எனில் (UNLIKELY (destfd < 0))
            திரும்பு crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ifdef HAVE_FGETXATTR
          ret = (முழு) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
#endif

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          நிறுத்து;

        நிலை S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          நிறுத்து;

        நிலை S_IFBLK:
        நிலை S_IFCHR:
        நிலை S_IFIFO:
        நிலை S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          நிறுத்து;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ifndef ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#endif

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      எனில் (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          எனில் (errno == ENOTSUP)
            தொடர்;

          திரும்பு crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  திரும்பு 0;
}

மாறிலி எழுத்து *
find_annotation (libcrun_container_t *container, மாறிலி எழுத்து *name)
{
  எனில் (container->container_def->annotations == வெற்று)
    திரும்பு வெற்று;

  திரும்பு find_string_map_மதிப்பு (container->annotations, name);
}

முழு
safe_write (முழு fd, மாறிலி எழுத்து *fname, மாறிலி வெறுமை *buf, size_t மொத்தம், libcrun_error_t *err)
{
  size_t written = 0;
  போது (written < மொத்தம்)
    {
      ssize_t w = லிக (fd, (மாறிலி எழுத்து *) buf + written, மொத்தம் - written);
      எனில் (UNLIKELY (w < 0))
        {
          எனில் (errno == EINTR || errno == EAGAIN)
            தொடர்;
          திரும்பு crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  திரும்பு 0;
}

#define MAX_PARTS 32

முழு
append_paths (எழுத்து **out, libcrun_error_t *err, ...)
{
  மாறிலி எழுத்து *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t total_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_start (ap, err);
  க்கு (;;)
    {
      மாறிலி எழுத்து *part;
      size_t size;

      part = va_arg (ap, மாறிலி எழுத்து *);
      எனில் (part == வெற்று)
        நிறுத்து;

      எனில் (n_parts == MAX_PARTS)
        {
          va_end (ap);
          திரும்பு crun_make_error (err, EINVAL, "too many paths specified");
        }

      எனில் (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          போது (part[0] == '/' && part[1] == '/')
            part++;
        }
      இல்லையெனில்
        {
          /* And drop any initial '/' for other components.  */
          போது (part[0] == '/')
            part++;
        }

      size = strlen (part);
      எனில் (size == 0)
        தொடர்;

      போது (size > 1 && part[size - 1] == '/')
        size--;

      parts[n_parts] = part;
      sizes[n_parts] = size;

      n_parts++;
    }
  va_end (ap);

  total_len = n_parts + 1;
  க்கு (i = 0; i < n_parts; i++)
    total_len += sizes[i];

  *out = xmalloc (total_len);

  copied = 0;
  க்கு (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      எனில் (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      memcpy (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  திரும்பு 0;
}

#எனில் __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#இல்லையெனில்
#  define __nonstring
#endif

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
நிலையான முழு
b64_உள்ளீடு (எழுத்து c)
{
  மாறிலி எழுத்து table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  முழு i;

  க்கு (i = 0; i < 64; i++)
    {
      எனில் (table[i] == c)
        திரும்பு i;
    }
  திரும்பு -1;
}

முழு
base64_decode (மாறிலி எழுத்து *iptr, size_t isize, எழுத்து *optr, size_t osize, size_t *nbytes)
{
  முழு i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  unsigned எழுத்து data[4];

  *nbytes = 0;
  போது (consumed < isize && (*nbytes) + 3 < osize)
    {
      போது ((i < 4) && (consumed < isize))
        {
          tmp = b64_உள்ளீடு (*iptr++);
          consumed++;
          எனில் (tmp != -1)
            {
              எனில் (UNLIKELY (pad > 0))
                திரும்பு -1;
              data[i++] = tmp;
            }
          இல்லையெனில் எனில் (*(iptr - 1) == '=')
            {
              data[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      எனில் (i == 4)
        {
          எனில் (UNLIKELY (pad > 2))
            திரும்பு -1;
          *optr++ = (data[0] << 2) | ((data[1] & 0x30) >> 4);
          *optr++ = ((data[1] & 0xf) << 4) | ((data[2] & 0x3c) >> 2);
          *optr++ = ((data[2] & 0x3) << 6) | data[3];
          (*nbytes) += 3 - pad;
        }
      இல்லையெனில்
        {
          /* I did not get all the data.  */
          consumed -= i;
          திரும்பு consumed;
        }
      i = 0;
    }
  திரும்பு consumed;
}

எழுத்து *
get_user_name (uid_t uid)
{
  கட்டமைப்பு passwd pd;
  கட்டமைப்பு passwd *temp_result_ptr;
  எழுத்து pwdbuffer[200];
  எனில் (! getpwuid_r (uid, &pd, pwdbuffer, அளவு (pwdbuffer), &temp_result_ptr))
    திரும்பு xstrdup (pd.pw_name);
  திரும்பு xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
எழுத்து *
libcrun_sysctl_சாவி_to_proc_path (மாறிலி எழுத்து *சாவி)
{
  மாறிலி எழுத்து *sep;
  எழுத்து *ret, *it;

  sep = strpbrk (சாவி, "./");

  /* No separator at all, or the key is already in the path form.  */
  எனில் (sep == வெற்று || *sep == '/')
    திரும்பு xstrdup (சாவி);

  ret = xstrdup (சாவி);
  க்கு (it = ret; *it; it++)
    {
      எனில் (*it == '.')
        *it = '/';
      இல்லையெனில் எனில் (*it == '/')
        *it = '.';
    }

  திரும்பு ret;
}

முழு
has_suffix (மாறிலி எழுத்து *str, மாறிலி எழுத்து *suffix)
{
  எனில் (! str || ! suffix)
    திரும்பு 0;
  size_t lenstr = strlen (str);
  size_t lensuffix = strlen (suffix);
  எனில் (lensuffix > lenstr)
    திரும்பு 0;
  திரும்பு memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

எழுத்து *
str_join_array (முழு offset, size_t size, எழுத்து *மாறிலி array[], மாறிலி எழுத்து *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = வெற்று;
  size_t i, total_size = (size - 1) * (jlen = strlen (joint)) + 1;
  எழுத்து *result, *p;

  lens = xmalloc (size * அளவு (*lens));

  க்கு (i = 0; i < size; ++i)
    {
      total_size += (lens[i] = strlen (array[i]));
    }
  p = result = xmalloc (total_size);
  க்கு (i = offset; i < size; ++i)
    {
      memcpy (p, array[i], lens[i]);
      p += lens[i];
      எனில் (i < size - 1)
        {
          memcpy (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  திரும்பு result;
}

முழு
libcrun_mmap (கட்டமைப்பு libcrun_mmap_s **ret, வெறுமை *addr, size_t length,
              முழு prot, முழு flags, முழு fd, off_t offset,
              libcrun_error_t *err)
{
  கட்டமைப்பு libcrun_mmap_s *mmap_s = வெற்று;

  வெறுமை *mapped = mmap (addr, length, prot, flags, fd, offset);
  எனில் (mapped == MAP_FAILED)
    திரும்பு crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (அளவு (கட்டமைப்பு libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->length = length;

  *ret = mmap_s;

  திரும்பு 0;
}

முழு
libcrun_munmap (கட்டமைப்பு libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  முழு ret;

  ret = munmap (mmap->addr, mmap->length);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "munmap");

  விடுவி (mmap);

  திரும்பு 0;
}

நிலையான long
read_file_as_long_or_default (மாறிலி எழுத்து *path, long def_மதிப்பு)
{
  cleanup_free எழுத்து *content = வெற்று;
  libcrun_error_t tmp_err = வெற்று;
  எழுத்து *endptr;
  long val;
  முழு ret;

  ret = read_all_file (path, &content, வெற்று, &tmp_err);
  எனில் (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      திரும்பு def_மதிப்பு;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  எனில் (UNLIKELY (errno))
    திரும்பு def_மதிப்பு;
  எனில் (endptr == content || (*endptr && *endptr != '\n'))
    திரும்பு def_மதிப்பு;
  திரும்பு val;
}

#define DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (வெறுமை)
{
  நிலையான atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  எனில் (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  திரும்பு uid;
}

gid_t
get_overflow_gid (வெறுமை)
{
  நிலையான atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  எனில் (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  திரும்பு gid;
}

வெறுமை
consume_trailing_slashes (எழுத்து *path)
{
  எனில் (! path || path[0] == '\0')
    திரும்பு;

  எழுத்து *last = path + strlen (path);

  போது (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

எழுத்து **
read_dir_entries (மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = வெற்று;
  size_t n_entries = 0;
  size_t entries_size = 16;
  எழுத்து **entries = வெற்று;
  கட்டமைப்பு dirent *de;

  dir = opendir (path);
  எனில் (UNLIKELY (dir == வெற்று))
    {
      crun_make_error (err, errno, "opendir `%s`", path);
      திரும்பு வெற்று;
    }

  entries = xmalloc (entries_size * அளவு (எழுத்து *));
  போது ((de = readdir (dir)))
    {
      எனில் (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        தொடர்;
      எனில் (n_entries == entries_size)
        {
          entries_size *= 2;
          entries = xrealloc (entries, entries_size * அளவு (எழுத்து *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * அளவு (எழுத்து *));
  entries[n_entries] = வெற்று;

  திரும்பு entries;
}

முழு
cpuset_string_to_bitmask (மாறிலி எழுத்து *str, எழுத்து **out, size_t *out_size, libcrun_error_t *err)
{
  cleanup_free எழுத்து *mask = வெற்று;
  size_t mask_size = 0;
  மாறிலி எழுத்து *p = str;
  எழுத்து *endptr;

  போது (*p)
    {
      long long start_range, end_range;

      எனில் (*p < '0' || *p > '9')
        செல் invalid_உள்ளீடு;

      start_range = strtoll (p, &endptr, 10);
      எனில் (start_range < 0)
        செல் invalid_உள்ளீடு;

      p = endptr;

      எனில் (*p != '-')
        end_range = start_range;
      இல்லையெனில்
        {
          p++;

          எனில் (*p < '0' || *p > '9')
            செல் invalid_உள்ளீடு;

          end_range = strtoll (p, &endptr, 10);

          எனில் (end_range < start_range)
            செல் invalid_உள்ளீடு;

          p = endptr;
        }

      /* Just set some limit.  */
      எனில் (end_range > (1 << 20))
        செல் invalid_உள்ளீடு;

      எனில் (end_range >= (long long) (mask_size * CHAR_BIT))
        {
          size_t new_mask_size = (end_range / CHAR_BIT) + 1;
          mask = xrealloc (mask, new_mask_size);
          memset (mask + mask_size, 0, new_mask_size - mask_size);
          mask_size = new_mask_size;
        }

      க்கு (long long i = start_range; i <= end_range; i++)
        mask[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      எனில் (*p == ',')
        p++;
      இல்லையெனில் எனில் (*p)
        செல் invalid_உள்ளீடு;
    }

  *out = mask;
  mask = வெற்று;
  *out_size = mask_size;

  திரும்பு 0;

invalid_உள்ளீடு:
  திரும்பு crun_make_error (err, 0, "cannot parse input `%s`", str);
}

கட்டமைப்பு channel_fd_pair
{
  கட்டமைப்பு ring_buffer *rb;

  முழு in_fd;
  முழு out_fd;

  முழு infd_epoll_events;
  முழு outfd_epoll_events;
};

கட்டமைப்பு channel_fd_pair *
channel_fd_pair_new (முழு in_fd, முழு out_fd, size_t size)
{
  கட்டமைப்பு channel_fd_pair *channel = xmalloc (அளவு (கட்டமைப்பு channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_buffer_make (size);
  திரும்பு channel;
}

வெறுமை
channel_fd_pair_free (கட்டமைப்பு channel_fd_pair *channel)
{
  எனில் (channel == வெற்று)
    திரும்பு;

  ring_buffer_free (channel->rb);
  விடுவி (channel);
}

முழு
channel_fd_pair_process (கட்டமைப்பு channel_fd_pair *channel, முழு epollfd, libcrun_error_t *err)
{
  bool is_உள்ளீடு_eagain = false, is_output_eagain = false, repeat;
  முழு ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  க்கு (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      எனில் (ring_buffer_get_space_available (channel->rb) > 0)
        {
          ret = ring_buffer_read (channel->rb, channel->in_fd, &is_உள்ளீடு_eagain, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          எனில் (ret > 0)
            repeat = true;
        }
      எனில் (ring_buffer_get_data_available (channel->rb) > 0)
        {
          ret = ring_buffer_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          எனில் (ret > 0)
            repeat = true;
        }
    }

  எனில் (epollfd >= 0)
    {
      size_t available = ring_buffer_get_space_available (channel->rb);
      size_t used = ring_buffer_get_data_available (channel->rb);
      முழு events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_உள்ளீடு_eagain ? EPOLLET : 0)) : 0;
      எனில் (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      எனில் (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          channel->outfd_epoll_events = events;
        }
    }
  திரும்பு 0;
}

நிலையான முழு
open_proc (libcrun_error_t *err)
{
  முழு fd;

#ifdef HAVE_NEW_MOUNT_API
  cleanup_close முழு fsfd = -1;
  முழு ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  எனில் (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, வெற்று, வெற்று, 0);
      எனில் (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          எனில் (fd >= 0)
            திரும்பு fd;
        }
    }
#endif

  fd = open ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  எனில் (fd < 0)
    திரும்பு crun_make_error (err, errno, "open `/proc`");
  திரும்பு fd;
}

முழு
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  எனில் (! container)
    திரும்பு open_proc (err);

  எனில் (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  திரும்பு container->proc_fd;
}

முழு
libcrun_open_proc_file (libcrun_container_t *container, மாறிலி எழுத்து *path, முழு flags, libcrun_error_t *err)
{
  முழு proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  எனில் (proc_fd < 0)
    திரும்பு proc_fd;

  fd = TEMP_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  எனில் (! container)
    TEMP_FAILURE_RETRY (close (proc_fd));

  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  திரும்பு fd;
}

முழு
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, மாறிலி எழுத்து *path, முழு flags, libcrun_error_t *err)
{
  cleanup_free எழுத்து *full_path = வெற்று;
  xasprintf (&full_path, "%d/%s", pid, path);
  திரும்பு libcrun_open_proc_file (container, full_path, flags, err);
}
