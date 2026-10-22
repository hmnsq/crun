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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ring_buffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdarg.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <libgen.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/un.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/socket.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/signalfd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/epoll.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/wait.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pwd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <grp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/xattr.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysmacros.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/vfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/magic.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mman.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#ਨਹੀਂਤਾਂ
#  define atomic_long ਅਸਥਿਰ ਲੰਮਾ
#  define atomic_int ਅਸਥਿਰ ਪੂਰਨ
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ "syscalls.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ RESOLVE_ਵਿੱਚ_ਜੜ੍ਹ
#  define RESOLVE_ਵਿੱਚ_ਜੜ੍ਹ 0x10
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __NR_close_range
#  define __NR_close_range 436
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __NR_openat2
#  define __NR_openat2 437
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PROC_USER_INIT_INO
#  define PROC_USER_INIT_INO ((ino_t) 0xEFFFFFFD)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_READLINKS 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_OPENAT2_EAGAIN_RETRIES 128

ਪੂਰਨ
crun_ਰਸਤਾ_exists (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  ਪੂਰਨ ret = access (ਰਸਤਾ, F_OK);
  ਜੇ (ret < 0)
    {
      ਜੇ (errno == ENOENT)
        ਵਾਪਸ 0;
      ਵਾਪਸ crun_make_error (err, errno, "access `%s`", ਰਸਤਾ);
    }
  ਵਾਪਸ 1;
}

ਪੂਰਨ
xasprintf (ਅੱਖਰ **str, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
  ਪੂਰਨ ret;
  va_list ਦਲੀਲਾਂ_list;

  va_ਸ਼ੁਰੂ (ਦਲੀਲਾਂ_list, fmt);

  ret = vasprintf (str, fmt, ਦਲੀਲਾਂ_list);
  ਜੇ (UNLIKELY (ret < 0))
    OOM ();

  va_ਸਿਰਾ (ਦਲੀਲਾਂ_list);
  ਵਾਪਸ ret;
}

ਪੂਰਨ
write_file_at_with_flags (ਪੂਰਨ dirfd, ਪੂਰਨ flags, mode_t ਬਹੁਲਕ, ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, size_t len, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ fd = -1;
  ਪੂਰਨ ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, ਬਹੁਲਕ);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s` for writing", name);

  ret = ਸੁਰੱਖਿਅਤ_write (fd, name, ਅੰਕੜੇ, len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ (len > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ) ? ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ : (ਪੂਰਨ) len;
}

ਪੂਰਨ
detach_process ()
{
  pid_t pid;
  ਜੇ (setsid () < 0)
    ਵਾਪਸ -1;
  pid = fork ();
  ਜੇ (pid < 0)
    ਵਾਪਸ -1;
  ਜੇ (pid != 0)
    ਸੁਰੱਖਿਅਤ_exit (EXIT_SUCCESS);
  ਵਾਪਸ 0;
}

ਪੂਰਨ
get_file_type_fd (ਪੂਰਨ fd, mode_t *ਬਹੁਲਕ)
{
  ਬਣਤਰ stat st;
  ਪੂਰਨ ret;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_STATX
  ਬਣਤਰ statx stx = {
    0,
  };

  ret = statx (fd, "", AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno == ENOSYS || errno == EINVAL)
        ਜਾਓ fallback;

      ਵਾਪਸ ret;
    }
  *ਬਹੁਲਕ = stx.stx_mode;
  ਵਾਪਸ ret;

fallback:
#ਜੇ_ਅੰਤ
  ret = fstat (fd, &st);
  *ਬਹੁਲਕ = st.st_mode;
  ਵਾਪਸ ret;
}

ਪੂਰਨ
get_file_type_at (ਪੂਰਨ dirfd, mode_t *ਬਹੁਲਕ, bool nofollow, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
  ਪੂਰਨ ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ = ਰਸਤਾ == ਨਿਰਾ ? AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ : 0;
  ਬਣਤਰ stat st;
  ਪੂਰਨ ret;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_STATX
  ਬਣਤਰ statx stx = {
    0,
  };

  ret = statx (dirfd, ਰਸਤਾ ?: "", ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno == ENOSYS || errno == EINVAL)
        ਜਾਓ fallback;

      ਵਾਪਸ ret;
    }
  *ਬਹੁਲਕ = stx.stx_mode;
  ਵਾਪਸ ret;

fallback:
#ਜੇ_ਅੰਤ
  ret = fstatat (dirfd, ਰਸਤਾ ?: "", &st, ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *ਬਹੁਲਕ = st.st_mode;
  ਵਾਪਸ ret;
}

ਪੂਰਨ
get_file_type (mode_t *ਬਹੁਲਕ, bool nofollow, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
  ਵਾਪਸ get_file_type_at (AT_FDCWD, ਬਹੁਲਕ, nofollow, ਰਸਤਾ);
}

ਪੂਰਨ
create_file_if_missing_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *file, mode_t ਬਹੁਲਕ, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, ਬਹੁਲਕ);
  ਜੇ (fd_write < 0)
    {
      ਪੂਰਨ saved_errno = errno;
      mode_t tmp_ਬਹੁਲਕ;
      ਪੂਰਨ ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_ਬਹੁਲਕ, true, file);
      ਜੇ (ret == 0 && S_ISREG (tmp_ਬਹੁਲਕ))
        ਵਾਪਸ 0;

      ਵਾਪਸ crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ensure_directory_ਅੰਦਰੂਨੀ_at (ਪੂਰਨ dirfd, ਅੱਖਰ *ਰਸਤਾ, size_t len, ਪੂਰਨ ਬਹੁਲਕ, libcrun_error_t *err)
{
  ਅੱਖਰ *it = ਰਸਤਾ + len;
  ਪੂਰਨ ret = 0;
  bool ਮਾਪਾ_created = false;

  ਲਈ (;;)
    {
      ret = mkdirat (dirfd, ਰਸਤਾ, ਬਹੁਲਕ);
      ਜੇ (ret == 0 || errno == EEXIST)
        ਵਾਪਸ 0;

      ਪੂਰਨ saved_errno = errno;
      ਜੇ (ਮਾਪਾ_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = ਨਿਰਾ;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (ਰਸਤਾ, false, &tmp_err);
          ਜੇ (ret > 0)
            ਵਾਪਸ 0;
          ਜੇ (ret < 0)
            crun_error_release (&tmp_err);

          ਵਾਪਸ crun_make_error (err, saved_errno, "create directory `%s`", ਰਸਤਾ);
        }

      ਜਦੋਂ (it > ਰਸਤਾ && *it != '/')
        {
          it--;
          len--;
        }
      ਜੇ (it == ਰਸਤਾ)
        ਵਾਪਸ 0;

      *it = '\0';
      ret = ensure_directory_ਅੰਦਰੂਨੀ_at (dirfd, ਰਸਤਾ, len - 1, ਬਹੁਲਕ, err);
      *it = '/';
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਮਾਪਾ_created = true;
    }
  ਵਾਪਸ ret;
}

ਪੂਰਨ
crun_ensure_directory_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ, bool nofollow, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_free ਅੱਖਰ *tmp = xstrdup (ਰਸਤਾ);
  ret = ensure_directory_ਅੰਦਰੂਨੀ_at (dirfd, tmp, ਲੜੀ_ਲੰਬਾਈ (tmp), ਬਹੁਲਕ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = crun_dir_p_at (dirfd, ਰਸਤਾ, nofollow, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ret == 0)
    ਵਾਪਸ crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", ਰਸਤਾ);

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
check_fd_is_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *fdname, libcrun_error_t *err)
{
  proc_fd_ਰਸਤਾ_t fdpath;
  size_t ਰਸਤਾ_len = ਲੜੀ_ਲੰਬਾਈ (ਰਸਤਾ);
  ਅੱਖਰ link[PATH_MAX];
  ਪੂਰਨ ret;

  get_proc_self_fd_ਰਸਤਾ (fdpath, fd);
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (readlink (fdpath, link, ਆਕਾਰ (link)));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "readlink `%s`", fdname);

  ਜੇ (((size_t) ret) != ਰਸਤਾ_len || memcmp (link, ਰਸਤਾ, ਰਸਤਾ_len))
    ਵਾਪਸ crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, ਰਸਤਾ);

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
check_fd_under_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *rootfs, size_t rootfslen, ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *fdname, libcrun_error_t *err)
{
  proc_fd_ਰਸਤਾ_t fdpath;
  ਅੱਖਰ link[PATH_MAX];
  ਪੂਰਨ ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  ਜੇ (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    ਵਾਪਸ 0;

  get_proc_self_fd_ਰਸਤਾ (fdpath, fd);
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (readlink (fdpath, link, ਆਕਾਰ (link)));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "readlink `%s`", fdname);

  ਜੇ (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    ਵਾਪਸ crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  ਵਾਪਸ 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
ਸਥਿਰ ਖਾਲੀ
close_and_replace (ਪੂਰਨ *oldfd, ਪੂਰਨ newfd)
{
  ਜੇ (*oldfd >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
ਅੱਖਰ *chroot_realpath (ਅਚਲ ਅੱਖਰ *chroot, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ resolved_ਰਸਤਾ[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
ਸਥਿਰ ਪੂਰਨ
ਸੁਰੱਖਿਅਤ_openat_fallback (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *rootfs, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ flags,
                      ਪੂਰਨ ਬਹੁਲਕ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਮਾਪਾ_ਰਸਤਾ = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *ਆਖਰੀ_component = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *orig_ਰਸਤਾ = ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *ਰਸਤਾ_ਵਿੱਚ_chroot;
  cleanup_close ਪੂਰਨ fd = -1;
  ਅੱਖਰ resolved[PATH_MAX];
  ਅੱਖਰ ਬਫਰ[PATH_MAX];
  size_t rootfs_len = ਸੁਰੱਖਿਅਤ_strlen (rootfs);
  ਪੂਰਨ ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  ਜੇ (flags & O_NOFOLLOW)
    {
      ਅੱਖਰ *sep;

      ਮਾਪਾ_ਰਸਤਾ = xstrdup (ਰਸਤਾ);
      sep = strrchr (ਮਾਪਾ_ਰਸਤਾ, '/');
      ਜੇ (sep == ਨਿਰਾ)
        {
          /* No parent directory, the entire path is the last component.  */
          ਆਖਰੀ_component = ਰਸਤਾ;
          ਮਾਪਾ_ਰਸਤਾ[0] = '\0';
        }
      ਨਹੀਂਤਾਂ ਜੇ (sep[1] != '\0')
        {
          *sep = '\0';
          ਆਖਰੀ_component = ਰਸਤਾ + (sep - ਮਾਪਾ_ਰਸਤਾ) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      ਜੇ (ਆਖਰੀ_component)
        ਰਸਤਾ = ਮਾਪਾ_ਰਸਤਾ;
    }

  ਰਸਤਾ_ਵਿੱਚ_chroot = chroot_realpath (rootfs, ਰਸਤਾ, ਬਫਰ);
  ਜੇ (ਰਸਤਾ_ਵਿੱਚ_chroot == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_ਰਸਤਾ);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  ਜੇ (rootfs_len > 0 && ਲੜੀ_ਤੁਲਨਾ (ਰਸਤਾ_ਵਿੱਚ_chroot, rootfs, rootfs_len) == 0)
    ਰਸਤਾ_ਵਿੱਚ_chroot += rootfs_len;
  ਰਸਤਾ_ਵਿੱਚ_chroot = consume_slashes (ਰਸਤਾ_ਵਿੱਚ_chroot);

  ਜੇ (ਆਖਰੀ_component)
    {
      ret = snprintf (resolved, ਆਕਾਰ (resolved), "%s%s%s", ਰਸਤਾ_ਵਿੱਚ_chroot,
                      ਰਸਤਾ_ਵਿੱਚ_chroot[0] == '\0' ? "" : "/", ਆਖਰੀ_component);
      ਜੇ (UNLIKELY (ret >= (ਪੂਰਨ) ਆਕਾਰ (resolved)))
        ਵਾਪਸ crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_ਰਸਤਾ);

      ਰਸਤਾ_ਵਿੱਚ_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  ਜੇ (ਰਸਤਾ_ਵਿੱਚ_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      ਵਾਪਸ ret;
    }

  ret = openat (dirfd, ਰਸਤਾ_ਵਿੱਚ_chroot, flags, ਬਹੁਲਕ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", orig_ਰਸਤਾ);

  fd = ret;

  ret = check_fd_under_ਰਸਤਾ (rootfs, rootfs_len, fd, orig_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = fd;
  fd = -1;
  ਵਾਪਸ ret;
}

ਪੂਰਨ
ਸੁਰੱਖਿਅਤ_openat (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *rootfs, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ flags, ਪੂਰਨ ਬਹੁਲਕ,
             libcrun_error_t *err)
{
  ਸਥਿਰ bool openat2_supported = true;
  ਪੂਰਨ retries = MAX_OPENAT2_EAGAIN_RETRIES;
  ਪੂਰਨ ret;

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਰਸਤਾ))
    {
      cleanup_close ਪੂਰਨ fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, ਬਹੁਲਕ);
      ਜੇ (UNLIKELY (fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      ਜੇ (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_ਰਸਤਾ (rootfs, fd, ਰਸਤਾ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ret = fd;
      fd = -1;
      ਵਾਪਸ ret;
    }

  ਜੇ (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, ਰਸਤਾ, flags, ਬਹੁਲਕ, RESOLVE_ਵਿੱਚ_ਜੜ੍ਹ);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (errno == EINTR)
            ਜਾਓ repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          ਜੇ (errno == EAGAIN && retries-- > 0)
            ਜਾਓ repeat;
          ਜੇ (errno == ENOSYS)
            openat2_supported = false;
          ਜੇ (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            ਵਾਪਸ ਸੁਰੱਖਿਅਤ_openat_fallback (dirfd, rootfs, ਰਸਤਾ, flags, ਬਹੁਲਕ, err);

          ਵਾਪਸ crun_make_error (err, errno, "openat2 `%s`", ਰਸਤਾ);
        }

      ਵਾਪਸ ret;
    }

  ਵਾਪਸ ਸੁਰੱਖਿਅਤ_openat_fallback (dirfd, rootfs, ਰਸਤਾ, flags, ਬਹੁਲਕ, err);
}

ssize_t
ਸੁਰੱਖਿਅਤ_readlinkat (ਪੂਰਨ dfd, ਅਚਲ ਅੱਖਰ *name, ਅੱਖਰ **ਬਫਰ, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_ਮਾਪ = hint > 0 ? hint + 1 : 512;
  cleanup_free ਅੱਖਰ *tmp_buf = ਨਿਰਾ;
  ssize_t ਮਾਪ;

  ਕਰੋ
    {
      ਜੇ (tmp_buf != ਨਿਰਾ)
        buf_ਮਾਪ += 256;

      tmp_buf = xrealloc (tmp_buf, buf_ਮਾਪ);

      ਮਾਪ = readlinkat (dfd, name, tmp_buf, buf_ਮਾਪ);
      ਜੇ (UNLIKELY (ਮਾਪ < 0))
        ਵਾਪਸ crun_make_error (err, errno, "readlink `%s`", name);
  } ਜਦੋਂ (ਮਾਪ == buf_ਮਾਪ);

  /* Always NUL terminate the buffer.  */
  tmp_buf[ਮਾਪ] = '\0';

  /* Move ownership to BUFFER.  */
  *ਬਫਰ = tmp_buf;
  tmp_buf = ਨਿਰਾ;

  ਵਾਪਸ ਮਾਪ;
}

ਸਥਿਰ ਪੂਰਨ crun_ਸੁਰੱਖਿਅਤ_ensure_at (bool do_open, bool dir, bool follow_symlinks, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *dirpath,
                                ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ, ਪੂਰਨ max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
ਸਥਿਰ ਪੂਰਨ
crun_ਸੁਰੱਖਿਅਤ_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *dirpath, ਪੂਰਨ cwd,
                                 ਅਚਲ ਅੱਖਰ *npath, ਅਚਲ ਅੱਖਰ *cur, ਅਚਲ ਅੱਖਰ *rest,
                                 ਪੂਰਨ ਬਹੁਲਕ, ਪੂਰਨ max_readlinks, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਨਿਸ਼ਾਨਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *new_ਰਸਤਾ = ਨਿਰਾ;
  ssize_t ret;

  ret = ਸੁਰੱਖਿਅਤ_readlinkat (cwd, cur, &ਨਿਸ਼ਾਨਾ, 0, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_ਰਸਤਾ, "%.*s%s%s%s", ਨਿਸ਼ਾਨਾ[0] == '/' ? 0 : (ਪੂਰਨ) (cur - npath), npath, ਨਿਸ਼ਾਨਾ,
             rest ? "/" : "", rest ? rest : "");

  ਵਾਪਸ crun_ਸੁਰੱਖਿਅਤ_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_ਰਸਤਾ, ਬਹੁਲਕ, max_readlinks - 1, err);
}

ਸਥਿਰ ਪੂਰਨ
crun_ਸੁਰੱਖਿਅਤ_ensure_at (bool do_open, bool dir, bool follow_symlinks, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *dirpath,
                     ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ, ਪੂਰਨ max_readlinks, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ wd_cleanup = -1;
  cleanup_free ਅੱਖਰ *npath = ਨਿਰਾ;
  bool ਆਖਰੀ_component = false;
  size_t ਡੂੰਘਾਈ = 0;
  ਅੱਖਰ *cur;
  ਅੱਖਰ *it;
  ਪੂਰਨ cwd;
  ਪੂਰਨ ret;

  ਜੇ (max_readlinks <= 0)
    ਵਾਪਸ crun_make_error (err, ELOOP, "resolve path `%s`", ਰਸਤਾ);

  ਰਸਤਾ = consume_slashes (ਰਸਤਾ);

  /* Empty path, nothing to do.  */
  ਜੇ (*ਰਸਤਾ == '\0')
    {
      ਜੇ (do_open)
        ਵਾਪਸ open (dirpath, O_CLOEXEC | O_ਰਸਤਾ, 0);
      ਵਾਪਸ 0;
    }

  npath = xstrdup (ਰਸਤਾ);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  ਜਦੋਂ (cur)
    {
      ਜੇ (it)
        *it = '\0';
      ਨਹੀਂਤਾਂ
        ਆਖਰੀ_component = true;

      ਜੇ (cur[0] == '\0')
        ਰੋਕੋ;

      ਜੇ (strcmp (cur, ".") == 0)
        ਜਾਓ ਅਗਲਾ;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (cur, ".."))
        ਡੂੰਘਾਈ++;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (ਡੂੰਘਾਈ)
            ਡੂੰਘਾਈ--;
          ਨਹੀਂਤਾਂ
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              ਜਾਓ ਅਗਲਾ;
            }
        }

      ਜੇ (ਆਖਰੀ_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          ਜੇ (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              ਜੇ (errno == ELOOP)
                ਵਾਪਸ crun_ਸੁਰੱਖਿਅਤ_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, ਨਿਰਾ,
                                                        ਬਹੁਲਕ, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_ਰਸਤਾ | O_NOFOLLOW, 0);
              ਜੇ (ret < 0)
                ਵਾਪਸ crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          ਜੇ (do_open)
            ਵਾਪਸ ret;

          close_and_replace (&wd_cleanup, ret);
          ਵਾਪਸ 0;
        }

      ret = mkdirat (cwd, cur, ਬਹੁਲਕ);
      ਜੇ (ret < 0)
        {
          mode_t st_mode;

          ਜੇ (errno != EEXIST)
            ਵਾਪਸ crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          ਜੇ (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            ਵਾਪਸ crun_ਸੁਰੱਖਿਅਤ_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : ਨਿਰਾ, ਬਹੁਲਕ, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = ਸੁਰੱਖਿਅਤ_openat (dirfd, dirpath, npath, O_ਰਸਤਾ | O_CLOEXEC, 0, err);
      ਜੇ (UNLIKELY (cwd < 0))
        ਵਾਪਸ crun_error_wrap (err, "creating `/%s`", ਰਸਤਾ);

      ਜੇ (! ਆਖਰੀ_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, ਨਿਰਾ);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ਪੂਰਨ saved_errno = errno;

              close (cwd);
              ਵਾਪਸ crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          ਜੇ ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              ਵਾਪਸ crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", ਰਸਤਾ, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    ਅਗਲਾ:
      ਜੇ (it == ਨਿਰਾ)
        ਰੋਕੋ;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  ਜੇ (do_open)
    {
      ਜੇ (cwd == dirfd)
        {
          ret = dup (dirfd);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "dup `%s`", dirpath);
          ਵਾਪਸ ret;
        }

      wd_cleanup = -1;
      ਵਾਪਸ cwd;
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
crun_ਸੁਰੱਖਿਅਤ_create_and_open_ref_at (bool dir, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *dirpath, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  /* If the file/dir already exists, just open it.  */
  ret = ਸੁਰੱਖਿਅਤ_openat (dirfd, dirpath, ਰਸਤਾ, O_ਰਸਤਾ | O_CLOEXEC, 0, err);
  ਜੇ (LIKELY (ret >= 0))
    ਵਾਪਸ ret;

  crun_error_release (err);
  ਵਾਪਸ crun_ਸੁਰੱਖਿਅਤ_ensure_at (true, dir, true, dirfd, dirpath, ਰਸਤਾ, ਬਹੁਲਕ, MAX_READLINKS, err);
}

ਪੂਰਨ
crun_ਸੁਰੱਖਿਅਤ_ensure_directory_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *dirpath, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ,
                               libcrun_error_t *err)
{
  ਵਾਪਸ crun_ਸੁਰੱਖਿਅਤ_ensure_at (false, true, false, dirfd, dirpath, ਰਸਤਾ, ਬਹੁਲਕ, MAX_READLINKS, err);
}

ਪੂਰਨ
crun_ਸੁਰੱਖਿਅਤ_ensure_file_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *dirpath, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ,
                          libcrun_error_t *err)
{
  ਵਾਪਸ crun_ਸੁਰੱਖਿਅਤ_ensure_at (false, false, false, dirfd, dirpath, ਰਸਤਾ, ਬਹੁਲਕ, MAX_READLINKS, err);
}

ਪੂਰਨ
crun_ensure_directory (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ, bool nofollow, libcrun_error_t *err)
{
  ਵਾਪਸ crun_ensure_directory_at (AT_FDCWD, ਰਸਤਾ, ਬਹੁਲਕ, nofollow, err);
}

ਸਥਿਰ ਪੂਰਨ
get_file_ਮਾਪ (ਪੂਰਨ fd, off_t *ਮਾਪ)
{
  ਬਣਤਰ stat st;
  ਪੂਰਨ ret;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_STATX
  ਬਣਤਰ statx stx = {
    0,
  };

  ret = statx (fd, "", AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_ਮਾਪ, &stx);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno == ENOSYS || errno == EINVAL)
        ਜਾਓ fallback;
      ਵਾਪਸ ret;
    }
  *ਮਾਪ = stx.stx_size;

  ਵਾਪਸ ret;

fallback:
#ਜੇ_ਅੰਤ
  ret = fstat (fd, &st);
  *ਮਾਪ = st.st_size;
  ਵਾਪਸ ret;
}

ਪੂਰਨ
crun_dir_p_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool nofollow, libcrun_error_t *err)
{
  mode_t ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  ret = get_file_type_at (dirfd, &ਬਹੁਲਕ, nofollow, ਰਸਤਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "stat `%s`", ਰਸਤਾ);

  ਵਾਪਸ S_ISDIR (ਬਹੁਲਕ) ? 1 : 0;
}

ਪੂਰਨ
crun_dir_p (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool nofollow, libcrun_error_t *err)
{
  ਵਾਪਸ crun_dir_p_at (AT_FDCWD, ਰਸਤਾ, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
ਸਥਿਰ ਪੂਰਨ
check_running_ਵਿੱਚ_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  size_t len;
  ਪੂਰਨ ret;

  ret = read_all_file ("/proc/self/uid_map", &ਬਫਰ, &len, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      ਜੇ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ਵਾਪਸ 0;
        }
      ਵਾਪਸ ret;
    }

  ਵਾਪਸ ਲੜੀ_ਖੋਜ (ਬਫਰ, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
ਸਥਿਰ bool
running_ਵਿੱਚ_openvz ()
{
  ਜੇ (access ("/proc/vz", F_OK) < 0)
    ਵਾਪਸ false;

  ਵਾਪਸ access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

ਪੂਰਨ
check_running_ਵਿੱਚ_user_namespace (libcrun_error_t *err)
{
  ਸਥਿਰ atomic_int ਲੜੀਵਾਰ_ਵਿੱਚ_userns = -1;
  ਬਣਤਰ stat st;
  ਪੂਰਨ ret;

  ret = ਲੜੀਵਾਰ_ਵਿੱਚ_userns;
  ਜੇ (ret >= 0)
    ਵਾਪਸ ret;

  ret = stat ("/proc/self/ns/user", &st);
  ਜੇ (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      ਜੇ (st.st_ino == PROC_USER_INIT_INO)
        {
          ਲੜੀਵਾਰ_ਵਿੱਚ_userns = 0;
          ਵਾਪਸ ਲੜੀਵਾਰ_ਵਿੱਚ_userns;
        }

      ਜੇ (! running_ਵਿੱਚ_openvz ())
        {
          ਲੜੀਵਾਰ_ਵਿੱਚ_userns = 1;
          ਵਾਪਸ ਲੜੀਵਾਰ_ਵਿੱਚ_userns;
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (UNLIKELY (errno != ENOENT))
    ਵਾਪਸ crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_ਵਿੱਚ_user_namespace_uid_map (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਲੜੀਵਾਰ_ਵਿੱਚ_userns = ret;
  ਵਾਪਸ ਲੜੀਵਾਰ_ਵਿੱਚ_userns;
}

ਸਥਿਰ size_t
get_ਪੰਨਾ_ਮਾਪ ()
{
  ਸਥਿਰ atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  ਜੇ (pagesize == 0)
    {
      pagesize = (ਲੰਮਾ) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  ਵਾਪਸ (size_t) pagesize;
}

ਸਥਿਰ ਪੂਰਨ selinux_enabled = -1;
ਸਥਿਰ ਪੂਰਨ apparmor_enabled = -1;

ਪੂਰਨ
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਬਾਹਰਲਾ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ fd = -1;
  size_t len;
  ਪੂਰਨ ret;

  ਜੇ (selinux_enabled >= 0)
    ਵਾਪਸ selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ fd;

  ret = read_all_fd_with_ਮਾਪ_hint (fd, "mounts", &ਬਾਹਰਲਾ, &len, get_ਪੰਨਾ_ਮਾਪ (), err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  selinux_enabled = ਲੜੀ_ਖੋਜ (ਬਾਹਰਲਾ, "selinux") ? 1 : 0;

  ਵਾਪਸ selinux_enabled;
}

ਪੂਰਨ
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ fd = -1;
  ਪੂਰਨ ਮਾਪ;
  ਅੱਖਰ buf[1];

  ਜੇ (apparmor_enabled >= 0)
    ਵਾਪਸ apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  ਜੇ (fd == -1)
    {
      ਜੇ (errno == ENOENT)
        {
          apparmor_enabled = 0;
          ਵਾਪਸ 0;
        }

      ਵਾਪਸ crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  ਮਾਪ = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (fd, buf, ਆਕਾਰ (buf)));

  apparmor_enabled = ਮਾਪ > 0 && buf[0] == 'Y' ? 1 : 0;

  ਵਾਪਸ apparmor_enabled;
}

ਸਥਿਰ ਪੂਰਨ
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  ਜੇ (selinux_enabled < 0)
    ਵਾਪਸ crun_make_error (err, 0, "SELinux is not initialized correctly");
  ਵਾਪਸ selinux_enabled;
}

ਪੂਰਨ
ਜੋੜੋ_selinux_mount_label (ਅੱਖਰ **retlabel, ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *label, ਅਚਲ ਅੱਖਰ *context_type, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = libcrun_is_selinux_enabled (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (label && ret)
    {
      ਜੇ (ਅੰਕੜੇ && *ਅੰਕੜੇ)
        xasprintf (retlabel, "%s,%s=\"%s\"", ਅੰਕੜੇ, context_type, label);
      ਨਹੀਂਤਾਂ
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      ਵਾਪਸ 0;
    }
  *retlabel = xstrdup (ਅੰਕੜੇ);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
lsm_attr_ਰਸਤਾ (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *lsm, ਅਚਲ ਅੱਖਰ *fname, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ attr_dirfd = -1;
  cleanup_close ਪੂਰਨ lsm_dirfd = -1;
  ਅੱਖਰ *attr_ਰਸਤਾ = ਨਿਰਾ;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (attr_dirfd < 0))
    ਵਾਪਸ ਨਿਰਾ;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  ਜੇ (lsm != ਨਿਰਾ)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);

      ਜੇ (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          ਵਾਪਸ ਨਿਰਾ;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_ਰਸਤਾ, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  ਵਾਪਸ attr_ਰਸਤਾ;
}

ਸਥਿਰ ਪੂਰਨ
check_proc_super_magic (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  ਬਣਤਰ statfs sfs;

  ਪੂਰਨ ret = fstatfs (fd, &sfs);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "statfs `%s`", ਰਸਤਾ);

  ਜੇ (sfs.f_type != PROC_SUPER_MAGIC)
    ਵਾਪਸ crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", ਰਸਤਾ);

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਸੈੱਟ_security_attr (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *lsm, ਅਚਲ ਅੱਖਰ *fname, ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  cleanup_free ਅਚਲ ਅੱਖਰ *attr_ਰਸਤਾ = lsm_attr_ਰਸਤਾ (container, lsm, fname, err);
  cleanup_close ਪੂਰਨ fd = -1;

  ਜੇ (UNLIKELY (attr_ਰਸਤਾ == ਨਿਰਾ))
    ਵਾਪਸ -1;

  fd = libcrun_open_proc_file (container, attr_ਰਸਤਾ, O_WRONLY | O_CLOEXEC, err);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ fd;

  ret = check_proc_super_magic (fd, attr_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  // Write out data
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, ਅੰਕੜੇ, ਲੜੀ_ਲੰਬਾਈ (ਅੰਕੜੇ)));
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      ਜੇ (errno == ENOENT && lsm != ਨਿਰਾ && strcmp (lsm, "apparmor") == 0)
        {
          ਅਚਲ ਅੱਖਰ *profile = strchr (ਅੰਕੜੇ, ' ');

          ਵਾਪਸ crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : ਅੰਕੜੇ);
        }
      ਵਾਪਸ crun_make_error (err, errno, "write to file `%s`", attr_ਰਸਤਾ);
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
ਸੈੱਟ_selinux_label (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *label, bool now, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = libcrun_is_selinux_enabled (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ret)
    ਵਾਪਸ ਸੈੱਟ_security_attr (container, ਨਿਰਾ, now ? "current" : "exec", label, err);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  ਜੇ (apparmor_enabled < 0)
    ਵਾਪਸ crun_make_error (err, 0, "AppArmor is not initialized correctly");
  ਵਾਪਸ apparmor_enabled;
}

ਸਥਿਰ ਪੂਰਨ
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNCONFINED "unconfined"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNCONFINED_LEN (ssize_t) (ਆਕਾਰ (UNCONFINED) - 1)
  cleanup_free ਅਚਲ ਅੱਖਰ *attr_ਰਸਤਾ = lsm_attr_ਰਸਤਾ (container, "apparmor", "current", err);
  cleanup_close ਪੂਰਨ fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  ਅੱਖਰ buf[UNCONFINED_LEN + 1];

  ਜੇ (UNLIKELY (attr_ਰਸਤਾ == ਨਿਰਾ))
    ਵਾਪਸ -1;

  fd = libcrun_open_proc_file (container, attr_ਰਸਤਾ, O_RDONLY | O_CLOEXEC, err);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ fd;

  ਜੇ (UNLIKELY (check_proc_super_magic (fd, attr_ਰਸਤਾ, err)))
    ਵਾਪਸ -1;

  ssize_t ਬਾਈਟਾਂ_read = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (fd, buf, ਆਕਾਰ (buf)));
  ਜੇ (UNLIKELY (ਬਾਈਟਾਂ_read < 0))
    ਵਾਪਸ crun_make_error (err, errno, "read from `%s`", attr_ਰਸਤਾ);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  ਜੇ (ਬਾਈਟਾਂ_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    ਵਾਪਸ 1;
  ਜੇ (ਬਾਈਟਾਂ_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    ਵਾਪਸ 0;
  ਵਾਪਸ 1;
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ UNCONFINED
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ UNCONFINED_LEN
}

ਪੂਰਨ
ਸੈੱਟ_apparmor_profile (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = libcrun_is_apparmor_enabled (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ret)
    {
      cleanup_free ਅੱਖਰ *buf = ਨਿਰਾ;
      ret = is_current_process_confined (container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      ਵਾਪਸ ਸੈੱਟ_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
read_all_fd_ਅੰਦਰੂਨੀ (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *description, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *len, size_t ਮਾਪ_hint, size_t max_ਮਾਪ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *buf = ਨਿਰਾ;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t ਮਾਪ = 0;
  ਪੂਰਨ ret;

  ਜੇ (ਮਾਪ_hint)
    allocated = ਮਾਪ_hint;
  ਨਹੀਂਤਾਂ
    {
      ret = get_file_ਮਾਪ (fd, &ਮਾਪ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "stat `%s`", description);

      ਜੇ (max_ਮਾਪ && (size_t) ਮਾਪ > max_ਮਾਪ)
        ਵਾਪਸ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_ਮਾਪ);

      allocated = ਮਾਪ == 0 ? 1023 : ਮਾਪ;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  ਜਦੋਂ ((ਮਾਪ && nread < (size_t) ਮਾਪ) || ਮਾਪ == 0)
    {
      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (fd, buf + nread, allocated - nread));
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "read from file `%s`", description);

      ਜੇ (ret == 0)
        ਰੋਕੋ;

      nread += ret;

      ਜੇ (max_ਮਾਪ && nread > max_ਮਾਪ)
        ਵਾਪਸ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_ਮਾਪ);

      ਜੇ (nread == allocated)
        {
          ਜੇ (ਮਾਪ)
            ਰੋਕੋ;

          ਜੇ (pagesize == 0)
            pagesize = get_ਪੰਨਾ_ਮਾਪ ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  ਜੇ (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      ਅੱਖਰ *tmp = realloc (buf, nread + 1);
      ਜੇ (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *ਬਾਹਰਲਾ = buf;
  buf = ਨਿਰਾ;
  ਜੇ (len)
    *len = nread;
  ਵਾਪਸ 0;
}

ਪੂਰਨ
read_all_fd_with_ਮਾਪ_hint (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *description, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *len, size_t ਮਾਪ_hint, libcrun_error_t *err)
{
  ਵਾਪਸ read_all_fd_ਅੰਦਰੂਨੀ (fd, description, ਬਾਹਰਲਾ, len, ਮਾਪ_hint, 0, err);
}

ਪੂਰਨ
read_all_fd_with_max_ਮਾਪ (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *description, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *len, size_t max_ਮਾਪ, libcrun_error_t *err)
{
  ਵਾਪਸ read_all_fd_ਅੰਦਰੂਨੀ (fd, description, ਬਾਹਰਲਾ, len, 0, max_ਮਾਪ, err);
}

ਪੂਰਨ
read_all_file_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *len, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ fd = -1;

  fd = ਆਰਜ਼ੀ_FAILURE_RETRY (openat (dirfd, ਰਸਤਾ, O_RDONLY | O_CLOEXEC));
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਰਸਤਾ);

  ਵਾਪਸ read_all_fd (fd, ਰਸਤਾ, ਬਾਹਰਲਾ, len, err);
}

ਪੂਰਨ
read_all_file (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *len, libcrun_error_t *err)
{
  ਜੇ (strcmp (ਰਸਤਾ, "-") == 0)
    ਰਸਤਾ = "/dev/stdin";

  ਵਾਪਸ read_all_file_at (AT_FDCWD, ਰਸਤਾ, ਬਾਹਰਲਾ, len, err);
}

ਪੂਰਨ
get_realpath_ਮੰਜ਼ਿਲ_file (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ_name, ਅੱਖਰ **absolute_ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ targetfd = -1;

  targetfd = ਆਰਜ਼ੀ_FAILURE_RETRY (openat (dirfd, ਰਸਤਾ_name, O_RDONLY | O_CLOEXEC));
  ਜੇ (UNLIKELY (targetfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਰਸਤਾ_name);
  ਨਹੀਂਤਾਂ
    {
      ssize_t len;
      proc_fd_ਰਸਤਾ_t ਨਿਸ਼ਾਨਾ_fd_ਰਸਤਾ;

      get_proc_self_fd_ਰਸਤਾ (ਨਿਸ਼ਾਨਾ_fd_ਰਸਤਾ, targetfd);
      len = ਸੁਰੱਖਿਅਤ_readlinkat (AT_FDCWD, ਨਿਸ਼ਾਨਾ_fd_ਰਸਤਾ, absolute_ਰਸਤਾ, 0, err);
      ਜੇ (UNLIKELY (len < 0))
        ਵਾਪਸ crun_error_wrap (err, "error unable to provide absolute path to file `%s`", ਰਸਤਾ_name);
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
open_unix_domain_client_socket (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ dgram, libcrun_error_t *err)
{
  ਬਣਤਰ sockaddr_un addr = {};
  ਪੂਰਨ ret;
  proc_fd_ਰਸਤਾ_t name_buf;
  cleanup_close ਪੂਰਨ destfd = -1;
  cleanup_close ਪੂਰਨ fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", ਰਸਤਾ);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_ਧਾਰਾ, 0);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "create UNIX socket");

  ਜੇ (ਲੜੀ_ਲੰਬਾਈ (ਰਸਤਾ) >= ਆਕਾਰ (addr.sun_ਰਸਤਾ))
    {
      destfd = open (ਰਸਤਾ, O_ਰਸਤਾ | O_CLOEXEC);
      ਜੇ (UNLIKELY (destfd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਰਸਤਾ);

      get_proc_self_fd_ਰਸਤਾ (name_buf, destfd);

      ਰਸਤਾ = name_buf;
    }

  size_t ਰਸਤਾ_len = ਲੜੀ_ਲੰਬਾਈ (ਰਸਤਾ);
  ਜੇ (ਰਸਤਾ_len >= ਆਕਾਰ (addr.sun_ਰਸਤਾ))
    ਵਾਪਸ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", ਰਸਤਾ);

  ਸਮ੍ਰਿਤੀ_ਨਕਲ (addr.sun_ਰਸਤਾ, ਰਸਤਾ, ਰਸਤਾ_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (ਬਣਤਰ sockaddr *) &addr, ਆਕਾਰ (addr));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "connect socket to `%s`", ਰਸਤਾ);

  ret = fd;
  fd = -1;

  ਵਾਪਸ ret;
}

ਪੂਰਨ
open_unix_domain_socket (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ dgram, libcrun_error_t *err)
{
  ਬਣਤਰ sockaddr_un addr = {};
  proc_fd_ਰਸਤਾ_t name_buf;
  ਪੂਰਨ ret;
  cleanup_close ਪੂਰਨ dirfd = -1;
  cleanup_close ਪੂਰਨ fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_ਧਾਰਾ, 0);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "create UNIX socket");

  ਜੇ (ਲੜੀ_ਲੰਬਾਈ (ਰਸਤਾ) >= ਆਕਾਰ (addr.sun_ਰਸਤਾ))
    {
      cleanup_free ਅੱਖਰ *dpath = xstrdup (ਰਸਤਾ);
      cleanup_free ਅੱਖਰ *bpath = xstrdup (ਰਸਤਾ);
      ਅਚਲ ਅੱਖਰ *ਮਾਪਾ_dir = dirname (dpath);
      ਅਚਲ ਅੱਖਰ *ਅਧਾਰ = basename (bpath);
      ਪੂਰਨ n;

      dirfd = open (ਮਾਪਾ_dir, O_ਰਸਤਾ | O_DIRECTORY | O_CLOEXEC);
      ਜੇ (UNLIKELY (dirfd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open directory `%s`", ਮਾਪਾ_dir);

      get_proc_self_fd_ਰਸਤਾ (name_buf, dirfd);

      n = snprintf (addr.sun_ਰਸਤਾ, ਆਕਾਰ (addr.sun_ਰਸਤਾ), "%s/%s", name_buf, ਅਧਾਰ);
      ਜੇ (n < 0 || (size_t) n >= ਆਕਾਰ (addr.sun_ਰਸਤਾ))
        ਵਾਪਸ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", ਰਸਤਾ);
    }
  ਨਹੀਂਤਾਂ
    {
      size_t ਰਸਤਾ_len = ਲੜੀ_ਲੰਬਾਈ (ਰਸਤਾ);
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (addr.sun_ਰਸਤਾ, ਰਸਤਾ, ਰਸਤਾ_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (ਬਣਤਰ sockaddr *) &addr, ਆਕਾਰ (addr));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "bind socket to `%s`", ਰਸਤਾ);

  ਜੇ (! dgram)
    {
      ret = listen (fd, 1);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  ਵਾਪਸ ret;
}

ਪੂਰਨ
send_fd_ਮੰਜ਼ਿਲ_socket (ਪੂਰਨ server, ਪੂਰਨ fd, libcrun_error_t *err)
{
  ਵਾਪਸ send_fd_ਮੰਜ਼ਿਲ_socket_with_payload (server, fd, ਨਿਰਾ, 0, err);
}

ਪੂਰਨ
send_fd_ਮੰਜ਼ਿਲ_socket_with_payload (ਪੂਰਨ server, ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *payload, size_t payload_len, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਬਣਤਰ cmsghdr *cmsg = ਨਿਰਾ;
  ਬਣਤਰ iovec iov[2];
  ਬਣਤਰ msghdr ਸੁਨੇਹਾ = {};
  ਅੱਖਰ ctrl_buf[CMSG_SPACE (1 + ਆਕਾਰ (ਪੂਰਨ))] = {};
  ਅੱਖਰ ਅੰਕੜੇ[1];

  ਅੰਕੜੇ[0] = ' ';
  iov[0].iov_base = ਅੰਕੜੇ;
  iov[0].iov_len = ਆਕਾਰ (ਅੰਕੜੇ);

  ਜੇ (payload_len > 0)
    {
      iov[0].iov_base = (ਖਾਲੀ *) payload;
      iov[0].iov_len = payload_len;
    }

  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_name = ਨਿਰਾ;
  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_namelen = 0;
  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_iov = iov;
  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_iovlen = 1;
  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_controllen = CMSG_SPACE (ਆਕਾਰ (ਪੂਰਨ));
  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&ਸੁਨੇਹਾ);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (ਆਕਾਰ (ਪੂਰਨ));

  *((ਪੂਰਨ *) CMSG_ਅੰਕੜੇ (cmsg)) = fd;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (sendmsg (server, &ਸੁਨੇਹਾ, 0));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "sendmsg");
  ਵਾਪਸ 0;
}

ਪੂਰਨ
receive_fd_from_socket_with_payload (ਪੂਰਨ from, ਅੱਖਰ *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ fd = -1;
  ਪੂਰਨ ret;
  ਬਣਤਰ iovec iov[1];
  ਬਣਤਰ msghdr ਸੁਨੇਹਾ = {};
  ਅੱਖਰ ctrl_buf[CMSG_SPACE (ਆਕਾਰ (ਪੂਰਨ))] = {};
  ਅੱਖਰ ਅੰਕੜੇ[1];
  ਬਣਤਰ cmsghdr *cmsg;

  ਅੰਕੜੇ[0] = ' ';
  iov[0].iov_base = ਅੰਕੜੇ;
  iov[0].iov_len = ਆਕਾਰ (ਅੰਕੜੇ);

  ਜੇ (payload_len > 0)
    {
      iov[0].iov_base = (ਖਾਲੀ *) payload;
      iov[0].iov_len = payload_len;
    }

  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_name = ਨਿਰਾ;
  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_namelen = 0;
  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_iov = iov;
  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_iovlen = 1;
  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_controllen = CMSG_SPACE (ਆਕਾਰ (ਪੂਰਨ));
  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_control = ctrl_buf;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (recvmsg (from, &ਸੁਨੇਹਾ, 0));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "recvmsg");
  ਜੇ (UNLIKELY (ret == 0))
    ਵਾਪਸ crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&ਸੁਨੇਹਾ);
  ਜੇ (cmsg == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "no msg received");
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (&fd, CMSG_ਅੰਕੜੇ (cmsg), ਆਕਾਰ (fd));

  ret = fd;
  fd = -1;
  ਵਾਪਸ ret;
}

ਪੂਰਨ
receive_fd_from_socket (ਪੂਰਨ from, libcrun_error_t *err)
{
  ਵਾਪਸ receive_fd_from_socket_with_payload (from, ਨਿਰਾ, 0, err);
}

ਪੂਰਨ
create_socket_pair (ਪੂਰਨ *pair, libcrun_error_t *err)
{
  ਪੂਰਨ ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "socketpair");
  ਵਾਪਸ 0;
}

ਪੂਰਨ
create_signalfd (sigset_t *ਨਕਾਬ, libcrun_error_t *err)
{
  ਪੂਰਨ ret = signalfd (-1, ਨਕਾਬ, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "signalfd");
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
epoll_helper_toggle (ਪੂਰਨ epollfd, ਪੂਰਨ fd, ਪੂਰਨ events, libcrun_error_t *err)
{
  ਬਣਤਰ epoll_event ev = {};
  bool ਜੋੜੋ = events != 0;
  ਪੂਰਨ ret;

  ev.events = events;
  ev.ਅੰਕੜੇ.fd = fd;
  ret = epoll_ctl (epollfd, ਜੋੜੋ ? EPOLL_CTL_ਜੋੜੋ : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  ਜੇ (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      ਜੇ (errno == ENOENT || errno == EPERM)
        ਵਾਪਸ 0;
      ਵਾਪਸ crun_make_error (err, errno, "epoll_ctl `%s` `%d`", ਜੋੜੋ ? "add" : "del", fd);
    }
  ਵਾਪਸ 0;
}

ਪੂਰਨ
epoll_helper (ਪੂਰਨ *ਵਿੱਚ_fds, ਪੂਰਨ *ਵਿੱਚ_levelfds, ਪੂਰਨ *ਬਾਹਰਲਾ_fds, ਪੂਰਨ *ਬਾਹਰਲਾ_levelfds, libcrun_error_t *err)
{
  ਬਣਤਰ epoll_event ev;
  cleanup_close ਪੂਰਨ epollfd = -1;
  ਪੂਰਨ ret;

  ਪੂਰਨ *it;
  epollfd = epoll_create1 (0);
  ਜੇ (UNLIKELY (epollfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "epoll_create1");

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_FDS(FDS, EVENTS)                                            \
  ਲਈ (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.ਅੰਕੜੇ.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ਜੋੜੋ, *it, &ev);               \
      ਜੇ (UNLIKELY (ret < 0))                                           \
        ਵਾਪਸ crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  ਜੇ (ਵਿੱਚ_fds)
    ਜੋੜੋ_FDS (ਵਿੱਚ_fds, EPOLLIN);
  ਜੇ (ਵਿੱਚ_levelfds)
    ਜੋੜੋ_FDS (ਵਿੱਚ_levelfds, EPOLLIN | EPOLLET);
  ਜੇ (ਬਾਹਰਲਾ_fds)
    ਜੋੜੋ_FDS (ਬਾਹਰਲਾ_fds, EPOLLOUT);
  ਜੇ (ਬਾਹਰਲਾ_levelfds)
    ਜੋੜੋ_FDS (ਬਾਹਰਲਾ_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  ਵਾਪਸ ret;
}

ਪੂਰਨ
copy_from_fd_ਮੰਜ਼ਿਲ_fd (ਪੂਰਨ src, ਪੂਰਨ dst, ਪੂਰਨ consume, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ssize_t nread;
  size_t pagesize = get_ਪੰਨਾ_ਮਾਪ ();
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#ਜੇ_ਅੰਤ
  ਕਰੋ
    {
      cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
      ssize_t ਬਚਿਆ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_COPY_FILE_RANGE
      ਜੇ (can_copy_file_range)
        {
          nread = copy_file_range (src, ਨਿਰਾ, dst, ਨਿਰਾ, pagesize, 0);
          ਜੇ (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              ਜਾਓ fallback;
            }
          ਜੇ (consume && nread < 0 && errno == EAGAIN)
            ਵਾਪਸ 0;
          ਜੇ (nread < 0 && errno == EIO)
            ਵਾਪਸ 0;
          ਜੇ (UNLIKELY (nread < 0))
            ਵਾਪਸ crun_make_error (err, errno, "copy_file_range");
          ਜਾਰੀ;
        }
    fallback:
#ਜੇ_ਅੰਤ

      ਬਫਰ = xmalloc (pagesize);
      nread = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (src, ਬਫਰ, pagesize));
      ਜੇ (consume && nread < 0 && errno == EAGAIN)
        ਵਾਪਸ 0;
      ਜੇ (nread < 0 && errno == EIO)
        ਵਾਪਸ 0;
      ਜੇ (UNLIKELY (nread < 0))
        ਵਾਪਸ crun_make_error (err, errno, "read");

      ਬਚਿਆ = nread;
      ਜਦੋਂ (ਬਚਿਆ)
        {
          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (dst, ਬਫਰ + nread - ਬਚਿਆ, ਬਚਿਆ));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "write");
          ਬਚਿਆ -= ret;
        }
  } ਜਦੋਂ (consume && nread);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
ਲੜੀਵਾਰ_process (ਅੱਖਰ **ਦਲੀਲਾਂ, libcrun_error_t *err)
{
  pid_t pid = fork ();
  ਜੇ (UNLIKELY (pid < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fork");
  ਜੇ (pid)
    {
      ਪੂਰਨ r, ਸਥਿਤੀ;
      r = ਆਰਜ਼ੀ_FAILURE_RETRY (waitpid (pid, &ਸਥਿਤੀ, 0));
      ਜੇ (r < 0)
        ਵਾਪਸ crun_make_error (err, errno, "waitpid");
      ਜੇ (WIFEXITED (ਸਥਿਤੀ) || WIFSIGNALED (ਸਥਿਤੀ))
        ਵਾਪਸ WEXITSTATUS (ਸਥਿਤੀ);
    }

  execvp (ਦਲੀਲਾਂ[0], ਦਲੀਲਾਂ);
  ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_FGETPWENT_R
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
atou (ਅੱਖਰ **s)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ x;
  ਲਈ (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  ਵਾਪਸ x;
}

ਪੂਰਨ
fgetpwent_r (FILE *f, ਬਣਤਰ passwd *pw, ਅੱਖਰ *ਸਤਰ, size_t ਮਾਪ, ਬਣਤਰ passwd **res)
{
  ਅੱਖਰ *s;
  ਪੂਰਨ rv = 0;
  ਲਈ (;;)
    {
      ਸਤਰ[ਮਾਪ - 1] = '\xff';
      ਜੇ ((fgets (ਸਤਰ, ਮਾਪ, f) == ਨਿਰਾ) || ferror (f) || ਸਤਰ[ਮਾਪ - 1] != '\xff')
        {
          rv = (ਸਤਰ[ਮਾਪ - 1] != '\xff') ? ERANGE : ENOENT;
          ਸਤਰ = 0;
          pw = 0;
          ਰੋਕੋ;
        }
      ਸਤਰ[strcspn (ਸਤਰ, "\n")] = 0;

      s = ਸਤਰ;
      pw->pw_name = s++;
      ਜੇ (! (s = strchr (s, ':')))
        ਜਾਰੀ;

      *s++ = 0;
      pw->pw_passwd = s;
      ਜੇ (! (s = strchr (s, ':')))
        ਜਾਰੀ;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      ਜੇ (*s != ':')
        ਜਾਰੀ;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      ਜੇ (*s != ':')
        ਜਾਰੀ;

      *s++ = 0;
      pw->pw_gecos = s;
      ਜੇ (! (s = strchr (s, ':')))
        ਜਾਰੀ;

      *s++ = 0;
      pw->pw_dir = s;
      ਜੇ (! (s = strchr (s, ':')))
        ਜਾਰੀ;

      *s++ = 0;
      pw->pw_shell = s;
      ਰੋਕੋ;
    }
  *res = pw;
  ਜੇ (rv)
    errno = rv;
  ਵਾਪਸ rv;
}
#ਜੇ_ਅੰਤ

ਪੂਰਨ
ਸੈੱਟ_home_env (uid_t id)
{
  ਬਣਤਰ passwd pwd;
  cleanup_free ਅੱਖਰ *buf = ਨਿਰਾ;
  ਲੰਮਾ buf_ਮਾਪ;
  cleanup_file FILE *ਧਾਰਾ = ਨਿਰਾ;

  buf_ਮਾਪ = sysconf (_SC_GETPW_R_SIZE_MAX);
  ਜੇ (buf_ਮਾਪ < 0)
    buf_ਮਾਪ = 1024;

  buf = xmalloc (buf_ਮਾਪ);

  ਧਾਰਾ = fopen ("/etc/passwd", "re");
  ਜੇ (ਧਾਰਾ == ਨਿਰਾ)
    ਵਾਪਸ -1;

  ਲਈ (;;)
    {
      ਬਣਤਰ passwd *ret_pw = ਨਿਰਾ;

      ਪੂਰਨ ret = fgetpwent_r (ਧਾਰਾ, &pwd, buf, buf_ਮਾਪ, &ret_pw);
      ਜੇ (UNLIKELY (ret != 0))
        {
          ਜੇ (ret == EINTR)
            ਜਾਰੀ;

          ਜੇ (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              ਵਾਪਸ -1;
            }

          buf_ਮਾਪ *= 2;
          buf = xrealloc (buf, buf_ਮਾਪ);
          ਜਾਰੀ;
        }

      ਜੇ (ret_pw && ret_pw->pw_uid == id)
        {
          ਜੇ (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          ਨਹੀਂਤਾਂ
            ਵਾਪਸ 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
ਸਥਿਰ ਪੂਰਨ
getsubidrange (uid_t id, ਪੂਰਨ is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *ਨਿਵੇਸ਼ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *lineptr = ਨਿਰਾ;
  size_t lenlineptr = 0, len_name;
  ਲੰਮਾ buf_ਮਾਪ;
  cleanup_free ਅੱਖਰ *buf = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *name;
  ਬਣਤਰ passwd pwd;

  buf_ਮਾਪ = sysconf (_SC_GETPW_R_SIZE_MAX);
  ਜੇ (buf_ਮਾਪ < 0)
    buf_ਮਾਪ = 1024;

  buf = xmalloc (buf_ਮਾਪ);
  ਲਈ (;;)
    {
      ਪੂਰਨ ret;
      ਬਣਤਰ passwd *ret_pw = ਨਿਰਾ;

      ret = getpwuid_r (id, &pwd, buf, buf_ਮਾਪ, &ret_pw);
      ਜੇ (LIKELY (ret == 0))
        {
          ਜੇ (ret_pw)
            {
              name = ret_pw->pw_name;
              ਰੋਕੋ;
            }
          ਵਾਪਸ -1;
        }

      ਜੇ (ret != ERANGE)
        ਵਾਪਸ -1;

      buf_ਮਾਪ *= 2;
      buf = xrealloc (buf, buf_ਮਾਪ);
    }

  len_name = ਲੜੀ_ਲੰਬਾਈ (name);

  ਨਿਵੇਸ਼ = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  ਜੇ (ਨਿਵੇਸ਼ == ਨਿਰਾ)
    ਵਾਪਸ -1;

  ਲਈ (;;)
    {
      ਅੱਖਰ *endptr;
      ssize_t ਪਠ = getline (&lineptr, &lenlineptr, ਨਿਵੇਸ਼);
      ਜੇ (ਪਠ < 0)
        ਵਾਪਸ -1;

      ਜੇ (ਪਠ < (ssize_t) (len_name + 2))
        ਜਾਰੀ;

      ਜੇ (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        ਜਾਰੀ;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      ਜੇ (endptr >= &lineptr[ਪਠ])
        ਵਾਪਸ -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      ਵਾਪਸ 0;
    }
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIN(x, y) ((x) < (y) ? (x) : (y))

ਪੂਰਨ
format_default_id_mapping (ਅੱਖਰ **ਬਾਹਰਲਾ, uid_t container_id, uid_t host_uid, uid_t host_id, ਪੂਰਨ is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, ਉਪਲਬਧ = 0;
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  ਪੂਰਨ written = 0;
  ਪੂਰਨ ret, ਬਚਿਆ;

  *ਬਾਹਰਲਾ = ਨਿਰਾ;

  ਜੇ (getsubidrange (host_uid, is_uid, &from, &ਉਪਲਬਧ) < 0)
    ਵਾਪਸ 0;

  /* More than enough space for all the mappings.  */
  ਬਚਿਆ = 15 * 5 * 3;
  ਬਫਰ = xmalloc (ਬਚਿਆ + 1);

  ਜੇ (container_id > 0)
    {
      uint32_t ਵਰਤਿਆ = MIN (container_id, ਉਪਲਬਧ);
      ret = snprintf (ਬਫਰ + written, ਬਚਿਆ, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, ਵਰਤਿਆ);
      ਜੇ (UNLIKELY (ret >= ਬਚਿਆ))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      ਬਚਿਆ -= ret;

      from += ਵਰਤਿਆ;
      ਉਪਲਬਧ -= ਵਰਤਿਆ;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (ਬਫਰ + written, ਬਚਿਆ, "%d %d 1\n", container_id, host_id);
  ਜੇ (UNLIKELY (ret >= ਬਚਿਆ))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  ਬਚਿਆ -= ret;

  /* Last mapping: use any id that is left.  */
  ਜੇ (ਉਪਲਬਧ)
    {
      ret = snprintf (ਬਫਰ + written, ਬਚਿਆ, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, ਉਪਲਬਧ);
      ਜੇ (UNLIKELY (ret >= ਬਚਿਆ))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      ਬਚਿਆ -= ret;
    }

  *ਬਾਹਰਲਾ = ਬਫਰ;
  ਬਫਰ = ਨਿਰਾ;
  ਵਾਪਸ written;
}

ਸਥਿਰ ਪੂਰਨ
unset_cloexec_flag (ਪੂਰਨ fd)
{
  ਪੂਰਨ flags = fcntl (fd, F_GETFD);
  ਜੇ (flags == -1)
    ਵਾਪਸ -1;

  flags &= ~FD_CLOEXEC;

  ਵਾਪਸ fcntl (fd, F_SETFD, flags);
}

ਸਥਿਰ ਖਾਲੀ __attribute__ ((__noreturn__))
ਲੜੀਵਾਰ_process_ਬੱਚਾ (ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ **ਦਲੀਲਾਂ, ਅਚਲ ਅੱਖਰ *cwd, ਅੱਖਰ **envp, ਪੂਰਨ pipe_r,
                   ਪੂਰਨ pipe_w, ਪੂਰਨ ਬਾਹਰਲਾ_fd, ਪੂਰਨ err_fd, bool can_ignore_chdir_errors)
{
  ਅੱਖਰ *tmp_ਦਲੀਲਾਂ[] = { ਰਸਤਾ, ਨਿਰਾ };
  libcrun_error_t err = ਨਿਰਾ;
  ਪੂਰਨ dev_null_fd = -1;
  ਪੂਰਨ ret;

  ret = mark_or_close_fds_ge_than (ਨਿਰਾ, 3, false, &err);
  ਜੇ (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->ਸਥਿਤੀ, "%s", (err)->ਸੁਨੇਹਾ);

  ਜੇ (ਬਾਹਰਲਾ_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      ਜੇ (UNLIKELY (dev_null_fd < 0))
        ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
    }

  ਆਰਜ਼ੀ_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  ਆਰਜ਼ੀ_FAILURE_RETRY (close (pipe_r));

  dup2 (ਬਾਹਰਲਾ_fd >= 0 ? ਬਾਹਰਲਾ_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  ਜੇ (ਬਾਹਰਲਾ_fd >= 0)
    unset_cloexec_flag (1);
  ਜੇ (err_fd >= 0)
    unset_cloexec_flag (2);

  ਜੇ (dev_null_fd >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (dev_null_fd));
  ਜੇ (ਬਾਹਰਲਾ_fd >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (ਬਾਹਰਲਾ_fd));
  ਜੇ (err_fd >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (err_fd));

  ਜੇ (ਦਲੀਲਾਂ == ਨਿਰਾ)
    ਦਲੀਲਾਂ = tmp_ਦਲੀਲਾਂ;

  ਜੇ (cwd && chdir (cwd) < 0)
    {
      ਜੇ (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
    }

  execvpe (ਰਸਤਾ, ਦਲੀਲਾਂ, envp);
  ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
ਪੂਰਨ
ਲੜੀਵਾਰ_process_with_stdin_timeout_envp (ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ **ਦਲੀਲਾਂ, ਅਚਲ ਅੱਖਰ *cwd, ਪੂਰਨ timeout,
                                     ਅੱਖਰ **envp, ਅੱਖਰ *stdin, size_t stdin_len, ਪੂਰਨ ਬਾਹਰਲਾ_fd,
                                     ਪੂਰਨ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  ਪੂਰਨ stdin_pipe[2];
  pid_t pid;
  ਪੂਰਨ ret;
  cleanup_close ਪੂਰਨ pipe_r = -1;
  cleanup_close ਪੂਰਨ pipe_w = -1;
  sigset_t oldmask, ਨਕਾਬ;
  ਪੂਰਨ r, ਸਥਿਤੀ;

  sigemptyset (&ਨਕਾਬ);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  ਜੇ (timeout > 0)
    {
      sigaddset (&ਨਕਾਬ, SIGCHLD);
      ret = sigprocmask (SIG_ਖੰਡ, &ਨਕਾਬ, &oldmask);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  ਜੇ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      ਜਾਓ restore_sig_ਨਕਾਬ_and_exit;
    }

  ਜੇ (pid == 0)
    {
      /* run_process_child doesn't return.  */
      ਲੜੀਵਾਰ_process_ਬੱਚਾ (ਰਸਤਾ, ਦਲੀਲਾਂ, cwd, envp, pipe_r, pipe_w, ਬਾਹਰਲਾ_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (pipe_w, stdin, stdin_len));
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      ਜੇ (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          ਜਾਓ restore_sig_ਨਕਾਬ_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  ਜੇ (timeout)
    {
      time_t ਸ਼ੁਰੂ = ਸਮਾਂ (ਨਿਰਾ);
      time_t now;
      ਲਈ (now = ਸ਼ੁਰੂ; now - ਸ਼ੁਰੂ < timeout; now = ਸਮਾਂ (ਨਿਰਾ))
        {
          siginfo_t info;
          ਪੂਰਨ elapsed = now - ਸ਼ੁਰੂ;
          ਬਣਤਰ timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&ਨਕਾਬ, &info, &ts_timeout);
          ਜੇ (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              ਜਾਓ restore_sig_ਨਕਾਬ_and_exit;
            }

          ਜੇ (info.si_signo == SIGCHLD && info.si_pid == pid)
            ਜਾਓ read_waitpid;

          ਜੇ (ret < 0 && errno == EAGAIN)
            ਜਾਓ timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", ਰਸਤਾ);
      ਜਾਓ restore_sig_ਨਕਾਬ_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &ਸਥਿਤੀ, 0);
  ਜੇ (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  ਨਹੀਂਤਾਂ
    ret = get_process_exit_ਸਥਿਤੀ (ਸਥਿਤੀ);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_ਨਕਾਬ_and_exit:
  ਜੇ (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      ਜੇ (pid > 0)
        {
          kill (pid, SIGKILL);
          ਆਰਜ਼ੀ_FAILURE_RETRY (waitpid (pid, &ਸਥਿਤੀ, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, ਨਿਰਾ);
      ਜੇ (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  ਵਾਪਸ ret;
}

ਪੂਰਨ
mark_or_close_fds_ge_than (libcrun_container_t *container, ਪੂਰਨ n, bool close_now, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ cfd = -1;
  cleanup_dir DIR *dir = ਨਿਰਾ;
  ਪੂਰਨ ret;
  ਪੂਰਨ fd;
  ਬਣਤਰ dirent *ਅਗਲਾ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  ਜੇ (close_now)
    GCOV_DUMP ();
#ਜੇ_ਅੰਤ

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  ਜੇ (ret == 0)
    {
      ਜੇ (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      ਵਾਪਸ 0;
    }
  ਜੇ (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    ਵਾਪਸ crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  ਜੇ (UNLIKELY (cfd < 0))
    ਵਾਪਸ cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  dir = fdopendir (cfd);
  ਜੇ (UNLIKELY (dir == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  ਲਈ (ਅਗਲਾ = readdir (dir); ਅਗਲਾ; ਅਗਲਾ = readdir (dir))
    {
      ਪੂਰਨ val;
      ਅਚਲ ਅੱਖਰ *name = ਅਗਲਾ->d_name;
      ਜੇ (name[0] == '.')
        ਜਾਰੀ;

      val = strtoll (name, ਨਿਰਾ, 10);
      ਜੇ (val < n || val == fd)
        ਜਾਰੀ;

      ਜੇ (close_now)
        {
          ret = close (val);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "close fd `%d`", val);

          ਜੇ (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      ਨਹੀਂਤਾਂ
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  ਵਾਪਸ 0;
}

ਖਾਲੀ
get_current_timestamp (ਅੱਖਰ *ਬਾਹਰਲਾ, size_t len)
{
  ਬਣਤਰ timeval tv;
  ਬਣਤਰ tm now;
  ਅੱਖਰ timestamp[64];

  gettimeofday (&tv, ਨਿਰਾ);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, ਆਕਾਰ (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (ਖਾਲੀ) snprintf (ਬਾਹਰਲਾ, len, "%s.%06lldZ", timestamp, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ) tv.tv_usec);
  ਬਾਹਰਲਾ[len - 1] = '\0';
}

ਪੂਰਨ
ਸੈੱਟ_blocking_fd (ਪੂਰਨ fd, bool blocking, libcrun_error_t *err)
{
  ਪੂਰਨ ret, flags = fcntl (fd, F_GETFL, 0);
  ਜੇ (UNLIKELY (flags < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fcntl");
  ਵਾਪਸ 0;
}

ਪੂਰਨ
parse_json_file (json_object **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *jsondata, ਬਣਤਰ parser_context *ctx arg_ਅਣਵਰਤਿਆ, libcrun_error_t *err)
{
  ਬਣਤਰ json_tokener *tok;
  ਗਿਣਤੀ json_tokener_error jerr;

  *err = ਨਿਰਾ;

  ਜੇ (UNLIKELY (jsondata == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  ਜੇ (UNLIKELY (tok == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_ਸੈੱਟ_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *ਬਾਹਰਲਾ = json_tokener_parse_ex (tok, jsondata, (ਪੂਰਨ) ਲੜੀ_ਲੰਬਾਈ (jsondata));
  jerr = json_tokener_get_error (tok);
  ਜੇ (*ਬਾਹਰਲਾ == ਨਿਰਾ || jerr != json_tokener_success)
    {
      ਪੂਰਨ saved_pos = (ਪੂਰਨ) tok->char_ਵਿੱਥ;

      json_tokener_free (tok);
      ਜੇ (*ਬਾਹਰਲਾ)
        {
          json_object_put (*ਬਾਹਰਲਾ);
          *ਬਾਹਰਲਾ = ਨਿਰਾ;
        }
      ਵਾਪਸ crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  ਵਾਪਸ 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHECK_ACCESS_NOT_EXECUTABLE 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHECK_ACCESS_NOT_REGULAR 2

/* check that the specified path exists and it is executable.
   Return
   - 0 if the file is executable
   - CHECK_ACCESS_NOT_EXECUTABLE if it exists but it is not executable
   - CHECK_ACCESS_NOT_REGULAR if it not a regular file
   - -errno for any other generic error
 */
ਸਥਿਰ ਪੂਰਨ
check_access (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
  ਪੂਰਨ ret;
  mode_t ਬਹੁਲਕ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EACCESS
#  define ACCESS(ਰਸਤਾ, ਬਹੁਲਕ) (eaccess (ਰਸਤਾ, ਬਹੁਲਕ))
#ਨਹੀਂਤਾਂ
#  define ACCESS(ਰਸਤਾ, ਬਹੁਲਕ) (access (ਰਸਤਾ, ਬਹੁਲਕ))
#ਜੇ_ਅੰਤ

  ret = ACCESS (ਰਸਤਾ, X_OK);
  ਜੇ (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      ਜੇ (errno == EACCES)
        {
          ਪੂਰਨ saved_errno = errno;
          ret = ACCESS (ਰਸਤਾ, F_OK);
          errno = saved_errno;

          ਜੇ (ret == 0)
            ਵਾਪਸ CHECK_ACCESS_NOT_EXECUTABLE;
        }
      ਵਾਪਸ -errno;
    }

  ret = get_file_type (&ਬਹੁਲਕ, false, ਰਸਤਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ -errno;

  ਜੇ (! S_ISREG (ਬਹੁਲਕ))
    ਵਾਪਸ CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  ਵਾਪਸ 0;
}

ਪੂਰਨ
ਲੱਭੋ_executable (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *executable_ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *cwd, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cwd_executable_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *tmp = ਨਿਰਾ;
  ਪੂਰਨ ਆਖਰੀ_error = -ENOENT;
  ਅੱਖਰ *it, *ਸਿਰਾ;
  ਪੂਰਨ ret;

  *ਬਾਹਰਲਾ = ਨਿਰਾ;

  ਜੇ (executable_ਰਸਤਾ == ਨਿਰਾ || executable_ਰਸਤਾ[0] == '\0')
    ਵਾਪਸ crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  ਜੇ (executable_ਰਸਤਾ[0] != '/' && strchr (executable_ਰਸਤਾ, '/'))
    {
      cleanup_free ਅੱਖਰ *cwd_allocated = ਨਿਰਾ;

      ਜੇ (cwd == ਨਿਰਾ)
        {
          cwd_allocated = getcwd (ਨਿਰਾ, 0);
          ਜੇ (cwd_allocated == ਨਿਰਾ)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_ਰਸਤਾ, err, "/", cwd, executable_ਰਸਤਾ, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      executable_ਰਸਤਾ = cwd_executable_ਰਸਤਾ;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  ਜੇ (executable_ਰਸਤਾ[0] == '/')
    {
      ret = check_access (executable_ਰਸਤਾ);
      ਜੇ (LIKELY (ret == 0))
        {
          *ਬਾਹਰਲਾ = xstrdup (executable_ਰਸਤਾ);
          ਵਾਪਸ 0;
        }
      ਆਖਰੀ_error = ret;
      ਜਾਓ fail;
    }

  ਸਿਰਾ = tmp = xstrdup (getenv ("PATH"));

  ਜਦੋਂ ((it = strsep (&ਸਿਰਾ, ":")))
    {
      cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;

      ਜੇ (it == ਸਿਰਾ)
        it = ".";

      ret = append_paths (&ਰਸਤਾ, err, it, executable_ਰਸਤਾ, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ਜਾਰੀ;
        }

      ret = check_access (ਰਸਤਾ);
      ਜੇ (ret == 0)
        {
          /* Change owner.  */
          *ਬਾਹਰਲਾ = ਰਸਤਾ;
          ਰਸਤਾ = ਨਿਰਾ;
          ਵਾਪਸ 0;
        }

      ਜੇ (ret == -ENOENT)
        ਜਾਰੀ;

      ਆਖਰੀ_error = ret;
    }

fail:
  ਚੋਣ (ਆਖਰੀ_error)
    {
    ਹਾਲਤ CHECK_ACCESS_NOT_EXECUTABLE:
      ਵਾਪਸ crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_ਰਸਤਾ);

    ਹਾਲਤ CHECK_ACCESS_NOT_REGULAR:
      ਵਾਪਸ crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_ਰਸਤਾ);

    ਮੂਲ:
      errno = -ਆਖਰੀ_error;
      ਜੇ (errno == ENOENT)
        ਵਾਪਸ crun_make_error (err, errno, "executable file `%s` not found%s", executable_ਰਸਤਾ, executable_ਰਸਤਾ[0] == '/' ? "" : " in $PATH");
      ਵਾਪਸ crun_make_error (err, errno, "open `%s`", executable_ਰਸਤਾ);
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FGETXATTR

ਸਥਿਰ ssize_t
ਸੁਰੱਖਿਅਤ_read_xattr (ਅੱਖਰ **ret, ਪੂਰਨ sfd, ਅਚਲ ਅੱਖਰ *srcname, ਅਚਲ ਅੱਖਰ *name, size_t initial_ਮਾਪ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  ssize_t current_ਮਾਪ;
  ssize_t s;

  current_ਮਾਪ = (ssize_t) initial_ਮਾਪ;
  ਬਫਰ = xmalloc (current_ਮਾਪ + 1);

  ਜਦੋਂ (1)
    {
      s = fgetxattr (sfd, name, ਬਫਰ, current_ਮਾਪ);
      ਜੇ (UNLIKELY (s < 0))
        ਵਾਪਸ crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      ਜੇ (s < current_ਮਾਪ)
        ਰੋਕੋ;

      current_ਮਾਪ *= 2;
      ਬਫਰ = xrealloc (ਬਫਰ, current_ਮਾਪ + 1);
    }

  ਜੇ (s <= 0)
    ਵਾਪਸ s;

  ਬਫਰ[s] = '\0';

  /* Change owner.  */
  *ret = ਬਫਰ;
  ਬਫਰ = ਨਿਰਾ;

  ਵਾਪਸ s;
}

ਸਥਿਰ ssize_t
copy_xattr (ਪੂਰਨ sfd, ਪੂਰਨ dfd, ਅਚਲ ਅੱਖਰ *srcname, ਅਚਲ ਅੱਖਰ *destname, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *buf = ਨਿਰਾ;
  ssize_t xattr_len;
  ਅੱਖਰ *it;

  xattr_len = flistxattr (sfd, ਨਿਰਾ, 0);
  ਜੇ (UNLIKELY (xattr_len < 0))
    {
      ਜੇ (errno == ENOTSUP)
        ਵਾਪਸ 0;

      ਵਾਪਸ crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  ਜੇ (xattr_len == 0)
    ਵਾਪਸ 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  ਜੇ (UNLIKELY (xattr_len < 0))
    ਵਾਪਸ crun_make_error (err, errno, "flistxattr `%s`", srcname);

  ਲਈ (it = buf; it - buf < xattr_len; it += ਲੜੀ_ਲੰਬਾਈ (it) + 1)
    {
      cleanup_free ਅੱਖਰ *v = ਨਿਰਾ;
      ssize_t s;

      s = ਸੁਰੱਖਿਅਤ_read_xattr (&v, sfd, srcname, it, 256, err);
      ਜੇ (UNLIKELY (s < 0))
        ਵਾਪਸ s;

      s = fsetxattr (dfd, it, v, s, 0);
      ਜੇ (UNLIKELY (s < 0))
        {
          ਜੇ (errno == EINVAL || errno == EOPNOTSUPP)
            ਜਾਰੀ;

          ਵਾਪਸ crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  ਵਾਪਸ 0;
}

#ਜੇ_ਅੰਤ

ਸਥਿਰ ਪੂਰਨ
copy_rec_stat_file_at (ਪੂਰਨ dfd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, mode_t *ਬਹੁਲਕ, off_t *ਮਾਪ, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  ਬਣਤਰ stat st;
  ਪੂਰਨ ret;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_STATX
  ਬਣਤਰ statx stx = {
    0,
  };

  ret = statx (dfd, ਰਸਤਾ, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_ਬਹੁਲਕ | STATX_ਮਾਪ | STATX_UID | STATX_GID, &stx);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno == ENOSYS || errno == EINVAL)
        ਜਾਓ fallback;

      ਵਾਪਸ ret;
    }

  *ਬਹੁਲਕ = stx.stx_mode;
  *ਮਾਪ = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  ਵਾਪਸ ret;

fallback:
#ਜੇ_ਅੰਤ
  ret = fstatat (dfd, ਰਸਤਾ, &st, AT_SYMLINK_NOFOLLOW);

  *ਬਹੁਲਕ = st.st_mode;
  *ਮਾਪ = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  ਵਾਪਸ ret;
}

ਪੂਰਨ
copy_recursive_fd_ਮੰਜ਼ਿਲ_fd (ਪੂਰਨ srcdirfd, ਪੂਰਨ dfd, ਅਚਲ ਅੱਖਰ *srcname, ਅਚਲ ਅੱਖਰ *destname, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = ਨਿਰਾ;
  ਬਣਤਰ dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  ਜੇ (UNLIKELY (dsrcfd == ਨਿਰਾ))
    {
      ਆਰਜ਼ੀ_FAILURE_RETRY (close (srcdirfd));
      ਵਾਪਸ crun_make_error (err, errno, "open directory `%s`", destname);
    }

  ਲਈ (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close ਪੂਰਨ srcfd = -1;
      cleanup_close ਪੂਰਨ destfd = -1;
      cleanup_free ਅੱਖਰ *ਨਿਸ਼ਾਨਾ_buf = ਨਿਰਾ;
      ਪੂਰਨ ret;
      mode_t ਬਹੁਲਕ;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      ਜੇ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ਜਾਰੀ;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &ਬਹੁਲਕ, &st_size, &rdev, &uid, &gid);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      ਚੋਣ (ਬਹੁਲਕ & S_IFMT)
        {
        ਹਾਲਤ S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          ਜੇ (UNLIKELY (srcfd < 0))
            ਵਾਪਸ crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          ਜੇ (UNLIKELY (destfd < 0))
            ਵਾਪਸ crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_ਮੰਜ਼ਿਲ_fd (srcfd, destfd, 1, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FGETXATTR
          ret = (ਪੂਰਨ) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
#ਜੇ_ਅੰਤ

          ਆਰਜ਼ੀ_FAILURE_RETRY (close (destfd));
          destfd = -1;
          ਰੋਕੋ;

        ਹਾਲਤ S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, ਬਹੁਲਕ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ਜੇ (UNLIKELY (srcfd < 0))
            ਵਾਪਸ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ਜੇ (UNLIKELY (destfd < 0))
            ਵਾਪਸ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FGETXATTR
          ret = (ਪੂਰਨ) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
#ਜੇ_ਅੰਤ

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_ਮੰਜ਼ਿਲ_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          ਰੋਕੋ;

        ਹਾਲਤ S_IFLNK:
          ret = ਸੁਰੱਖਿਅਤ_readlinkat (dirfd (dsrcfd), de->d_name, &ਨਿਸ਼ਾਨਾ_buf, st_size, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          ret = symlinkat (ਨਿਸ਼ਾਨਾ_buf, destdirfd, de->d_name);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          ਰੋਕੋ;

        ਹਾਲਤ S_IFBLK:
        ਹਾਲਤ S_IFCHR:
        ਹਾਲਤ S_IFIFO:
        ਹਾਲਤ S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, ਬਹੁਲਕ, rdev);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          ਰੋਕੋ;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#ਜੇ_ਅੰਤ

      ret = fchmodat (destdirfd, de->d_name, ਬਹੁਲਕ & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      ਜੇ (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          ਜੇ (errno == ENOTSUP)
            ਜਾਰੀ;

          ਵਾਪਸ crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  ਵਾਪਸ 0;
}

ਅਚਲ ਅੱਖਰ *
ਲੱਭੋ_annotation (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *name)
{
  ਜੇ (container->container_def->annotations == ਨਿਰਾ)
    ਵਾਪਸ ਨਿਰਾ;

  ਵਾਪਸ ਲੱਭੋ_string_map_ਮੁੱਲ (container->annotations, name);
}

ਪੂਰਨ
ਸੁਰੱਖਿਅਤ_write (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *fname, ਅਚਲ ਖਾਲੀ *buf, size_t ਗਿਣਤੀ_2, libcrun_error_t *err)
{
  size_t written = 0;
  ਜਦੋਂ (written < ਗਿਣਤੀ_2)
    {
      ssize_t w = ਲਿਖ (fd, (ਅਚਲ ਅੱਖਰ *) buf + written, ਗਿਣਤੀ_2 - written);
      ਜੇ (UNLIKELY (w < 0))
        {
          ਜੇ (errno == EINTR || errno == EAGAIN)
            ਜਾਰੀ;
          ਵਾਪਸ crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  ਵਾਪਸ 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_PARTS 32

ਪੂਰਨ
append_paths (ਅੱਖਰ **ਬਾਹਰਲਾ, libcrun_error_t *err, ...)
{
  ਅਚਲ ਅੱਖਰ *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t ਕੁੱਲ_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_ਸ਼ੁਰੂ (ap, err);
  ਲਈ (;;)
    {
      ਅਚਲ ਅੱਖਰ *part;
      size_t ਮਾਪ;

      part = va_arg (ap, ਅਚਲ ਅੱਖਰ *);
      ਜੇ (part == ਨਿਰਾ)
        ਰੋਕੋ;

      ਜੇ (n_parts == MAX_PARTS)
        {
          va_ਸਿਰਾ (ap);
          ਵਾਪਸ crun_make_error (err, EINVAL, "too many paths specified");
        }

      ਜੇ (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          ਜਦੋਂ (part[0] == '/' && part[1] == '/')
            part++;
        }
      ਨਹੀਂਤਾਂ
        {
          /* And drop any initial '/' for other components.  */
          ਜਦੋਂ (part[0] == '/')
            part++;
        }

      ਮਾਪ = ਲੜੀ_ਲੰਬਾਈ (part);
      ਜੇ (ਮਾਪ == 0)
        ਜਾਰੀ;

      ਜਦੋਂ (ਮਾਪ > 1 && part[ਮਾਪ - 1] == '/')
        ਮਾਪ--;

      parts[n_parts] = part;
      sizes[n_parts] = ਮਾਪ;

      n_parts++;
    }
  va_ਸਿਰਾ (ap);

  ਕੁੱਲ_len = n_parts + 1;
  ਲਈ (i = 0; i < n_parts; i++)
    ਕੁੱਲ_len += sizes[i];

  *ਬਾਹਰਲਾ = xmalloc (ਕੁੱਲ_len);

  copied = 0;
  ਲਈ (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*ਬਾਹਰਲਾ)[copied - 1] == '/';
      ਜੇ (i > 0 && ! has_trailing_slash)
        {
          (*ਬਾਹਰਲਾ)[copied] = '/';
          copied += 1;
        }

      ਸਮ੍ਰਿਤੀ_ਨਕਲ (*ਬਾਹਰਲਾ + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*ਬਾਹਰਲਾ)[copied] = '\0';
  ਵਾਪਸ 0;
}

#ਜੇ __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#ਨਹੀਂਤਾਂ
#  define __nonstring
#ਜੇ_ਅੰਤ

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
ਸਥਿਰ ਪੂਰਨ
b64_ਨਿਵੇਸ਼ (ਅੱਖਰ c)
{
  ਅਚਲ ਅੱਖਰ ਸਾਰਣੀ[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  ਪੂਰਨ i;

  ਲਈ (i = 0; i < 64; i++)
    {
      ਜੇ (ਸਾਰਣੀ[i] == c)
        ਵਾਪਸ i;
    }
  ਵਾਪਸ -1;
}

ਪੂਰਨ
base64_decode (ਅਚਲ ਅੱਖਰ *iptr, size_t isize, ਅੱਖਰ *optr, size_t osize, size_t *nbytes)
{
  ਪੂਰਨ i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅੰਕੜੇ[4];

  *nbytes = 0;
  ਜਦੋਂ (consumed < isize && (*nbytes) + 3 < osize)
    {
      ਜਦੋਂ ((i < 4) && (consumed < isize))
        {
          tmp = b64_ਨਿਵੇਸ਼ (*iptr++);
          consumed++;
          ਜੇ (tmp != -1)
            {
              ਜੇ (UNLIKELY (pad > 0))
                ਵਾਪਸ -1;
              ਅੰਕੜੇ[i++] = tmp;
            }
          ਨਹੀਂਤਾਂ ਜੇ (*(iptr - 1) == '=')
            {
              ਅੰਕੜੇ[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      ਜੇ (i == 4)
        {
          ਜੇ (UNLIKELY (pad > 2))
            ਵਾਪਸ -1;
          *optr++ = (ਅੰਕੜੇ[0] << 2) | ((ਅੰਕੜੇ[1] & 0x30) >> 4);
          *optr++ = ((ਅੰਕੜੇ[1] & 0xf) << 4) | ((ਅੰਕੜੇ[2] & 0x3c) >> 2);
          *optr++ = ((ਅੰਕੜੇ[2] & 0x3) << 6) | ਅੰਕੜੇ[3];
          (*nbytes) += 3 - pad;
        }
      ਨਹੀਂਤਾਂ
        {
          /* I did not get all the data.  */
          consumed -= i;
          ਵਾਪਸ consumed;
        }
      i = 0;
    }
  ਵਾਪਸ consumed;
}

ਅੱਖਰ *
get_user_name (uid_t uid)
{
  ਬਣਤਰ passwd pd;
  ਬਣਤਰ passwd *ਆਰਜ਼ੀ_ਨਤੀਜਾ_ptr;
  ਅੱਖਰ pwdbuffer[200];
  ਜੇ (! getpwuid_r (uid, &pd, pwdbuffer, ਆਕਾਰ (pwdbuffer), &ਆਰਜ਼ੀ_ਨਤੀਜਾ_ptr))
    ਵਾਪਸ xstrdup (pd.pw_name);
  ਵਾਪਸ xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
ਅੱਖਰ *
libcrun_sysctl_ਕੁੰਜੀ_ਮੰਜ਼ਿਲ_proc_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ)
{
  ਅਚਲ ਅੱਖਰ *sep;
  ਅੱਖਰ *ret, *it;

  sep = strpbrk (ਕੁੰਜੀ, "./");

  /* No separator at all, or the key is already in the path form.  */
  ਜੇ (sep == ਨਿਰਾ || *sep == '/')
    ਵਾਪਸ xstrdup (ਕੁੰਜੀ);

  ret = xstrdup (ਕੁੰਜੀ);
  ਲਈ (it = ret; *it; it++)
    {
      ਜੇ (*it == '.')
        *it = '/';
      ਨਹੀਂਤਾਂ ਜੇ (*it == '/')
        *it = '.';
    }

  ਵਾਪਸ ret;
}

ਪੂਰਨ
has_suffix (ਅਚਲ ਅੱਖਰ *str, ਅਚਲ ਅੱਖਰ *suffix)
{
  ਜੇ (! str || ! suffix)
    ਵਾਪਸ 0;
  size_t lenstr = ਲੜੀ_ਲੰਬਾਈ (str);
  size_t lensuffix = ਲੜੀ_ਲੰਬਾਈ (suffix);
  ਜੇ (lensuffix > lenstr)
    ਵਾਪਸ 0;
  ਵਾਪਸ memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

ਅੱਖਰ *
str_join_array (ਪੂਰਨ ਵਿੱਥ, size_t ਮਾਪ, ਅੱਖਰ *ਅਚਲ array[], ਅਚਲ ਅੱਖਰ *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = ਨਿਰਾ;
  size_t i, ਕੁੱਲ_ਮਾਪ = (ਮਾਪ - 1) * (jlen = ਲੜੀ_ਲੰਬਾਈ (joint)) + 1;
  ਅੱਖਰ *ਨਤੀਜਾ, *p;

  lens = xmalloc (ਮਾਪ * ਆਕਾਰ (*lens));

  ਲਈ (i = 0; i < ਮਾਪ; ++i)
    {
      ਕੁੱਲ_ਮਾਪ += (lens[i] = ਲੜੀ_ਲੰਬਾਈ (array[i]));
    }
  p = ਨਤੀਜਾ = xmalloc (ਕੁੱਲ_ਮਾਪ);
  ਲਈ (i = ਵਿੱਥ; i < ਮਾਪ; ++i)
    {
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (p, array[i], lens[i]);
      p += lens[i];
      ਜੇ (i < ਮਾਪ - 1)
        {
          ਸਮ੍ਰਿਤੀ_ਨਕਲ (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  ਵਾਪਸ ਨਤੀਜਾ;
}

ਪੂਰਨ
libcrun_mmap (ਬਣਤਰ libcrun_mmap_s **ret, ਖਾਲੀ *addr, size_t ਲੰਮਾਈ,
              ਪੂਰਨ prot, ਪੂਰਨ flags, ਪੂਰਨ fd, off_t ਵਿੱਥ,
              libcrun_error_t *err)
{
  ਬਣਤਰ libcrun_mmap_s *mmap_s = ਨਿਰਾ;

  ਖਾਲੀ *mapped = mmap (addr, ਲੰਮਾਈ, prot, flags, fd, ਵਿੱਥ);
  ਜੇ (mapped == MAP_FAILED)
    ਵਾਪਸ crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (ਆਕਾਰ (ਬਣਤਰ libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->ਲੰਮਾਈ = ਲੰਮਾਈ;

  *ret = mmap_s;

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_munmap (ਬਣਤਰ libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = munmap (mmap->addr, mmap->ਲੰਮਾਈ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "munmap");

  ਮੁਕਤ (mmap);

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਲੰਮਾ
read_file_as_long_or_default (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਲੰਮਾ def_ਮੁੱਲ)
{
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  libcrun_error_t tmp_err = ਨਿਰਾ;
  ਅੱਖਰ *endptr;
  ਲੰਮਾ val;
  ਪੂਰਨ ret;

  ret = read_all_file (ਰਸਤਾ, &content, ਨਿਰਾ, &tmp_err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ਵਾਪਸ def_ਮੁੱਲ;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  ਜੇ (UNLIKELY (errno))
    ਵਾਪਸ def_ਮੁੱਲ;
  ਜੇ (endptr == content || (*endptr && *endptr != '\n'))
    ਵਾਪਸ def_ਮੁੱਲ;
  ਵਾਪਸ val;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_ਉਛਾਲ_ID 65534

uid_t
get_ਉਛਾਲ_uid (ਖਾਲੀ)
{
  ਸਥਿਰ atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  ਜੇ (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_ਉਛਾਲ_ID);
      cached_uid = uid;
    }
  ਵਾਪਸ uid;
}

gid_t
get_ਉਛਾਲ_gid (ਖਾਲੀ)
{
  ਸਥਿਰ atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  ਜੇ (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_ਉਛਾਲ_ID);
      cached_gid = gid;
    }
  ਵਾਪਸ gid;
}

ਖਾਲੀ
consume_trailing_slashes (ਅੱਖਰ *ਰਸਤਾ)
{
  ਜੇ (! ਰਸਤਾ || ਰਸਤਾ[0] == '\0')
    ਵਾਪਸ;

  ਅੱਖਰ *ਆਖਰੀ = ਰਸਤਾ + ਲੜੀ_ਲੰਬਾਈ (ਰਸਤਾ);

  ਜਦੋਂ (ਆਖਰੀ > ਰਸਤਾ && *(ਆਖਰੀ - 1) == '/')
    ਆਖਰੀ--;

  *ਆਖਰੀ = '\0';
}

ਅੱਖਰ **
read_dir_entries (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = ਨਿਰਾ;
  size_t n_entries = 0;
  size_t entries_ਮਾਪ = 16;
  ਅੱਖਰ **entries = ਨਿਰਾ;
  ਬਣਤਰ dirent *de;

  dir = opendir (ਰਸਤਾ);
  ਜੇ (UNLIKELY (dir == ਨਿਰਾ))
    {
      crun_make_error (err, errno, "opendir `%s`", ਰਸਤਾ);
      ਵਾਪਸ ਨਿਰਾ;
    }

  entries = xmalloc (entries_ਮਾਪ * ਆਕਾਰ (ਅੱਖਰ *));
  ਜਦੋਂ ((de = readdir (dir)))
    {
      ਜੇ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ਜਾਰੀ;
      ਜੇ (n_entries == entries_ਮਾਪ)
        {
          entries_ਮਾਪ *= 2;
          entries = xrealloc (entries, entries_ਮਾਪ * ਆਕਾਰ (ਅੱਖਰ *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * ਆਕਾਰ (ਅੱਖਰ *));
  entries[n_entries] = ਨਿਰਾ;

  ਵਾਪਸ entries;
}

ਪੂਰਨ
cpuset_string_ਮੰਜ਼ਿਲ_bitmask (ਅਚਲ ਅੱਖਰ *str, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *ਬਾਹਰਲਾ_ਮਾਪ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਨਕਾਬ = ਨਿਰਾ;
  size_t ਨਕਾਬ_ਮਾਪ = 0;
  ਅਚਲ ਅੱਖਰ *p = str;
  ਅੱਖਰ *endptr;

  ਜਦੋਂ (*p)
    {
      ਲੰਮਾ ਲੰਮਾ ਸ਼ੁਰੂ_range, ਸਿਰਾ_range;

      ਜੇ (*p < '0' || *p > '9')
        ਜਾਓ invalid_ਨਿਵੇਸ਼;

      ਸ਼ੁਰੂ_range = strtoll (p, &endptr, 10);
      ਜੇ (ਸ਼ੁਰੂ_range < 0)
        ਜਾਓ invalid_ਨਿਵੇਸ਼;

      p = endptr;

      ਜੇ (*p != '-')
        ਸਿਰਾ_range = ਸ਼ੁਰੂ_range;
      ਨਹੀਂਤਾਂ
        {
          p++;

          ਜੇ (*p < '0' || *p > '9')
            ਜਾਓ invalid_ਨਿਵੇਸ਼;

          ਸਿਰਾ_range = strtoll (p, &endptr, 10);

          ਜੇ (ਸਿਰਾ_range < ਸ਼ੁਰੂ_range)
            ਜਾਓ invalid_ਨਿਵੇਸ਼;

          p = endptr;
        }

      /* Just set some limit.  */
      ਜੇ (ਸਿਰਾ_range > (1 << 20))
        ਜਾਓ invalid_ਨਿਵੇਸ਼;

      ਜੇ (ਸਿਰਾ_range >= (ਲੰਮਾ ਲੰਮਾ) (ਨਕਾਬ_ਮਾਪ * CHAR_BIT))
        {
          size_t new_ਨਕਾਬ_ਮਾਪ = (ਸਿਰਾ_range / CHAR_BIT) + 1;
          ਨਕਾਬ = xrealloc (ਨਕਾਬ, new_ਨਕਾਬ_ਮਾਪ);
          memset (ਨਕਾਬ + ਨਕਾਬ_ਮਾਪ, 0, new_ਨਕਾਬ_ਮਾਪ - ਨਕਾਬ_ਮਾਪ);
          ਨਕਾਬ_ਮਾਪ = new_ਨਕਾਬ_ਮਾਪ;
        }

      ਲਈ (ਲੰਮਾ ਲੰਮਾ i = ਸ਼ੁਰੂ_range; i <= ਸਿਰਾ_range; i++)
        ਨਕਾਬ[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      ਜੇ (*p == ',')
        p++;
      ਨਹੀਂਤਾਂ ਜੇ (*p)
        ਜਾਓ invalid_ਨਿਵੇਸ਼;
    }

  *ਬਾਹਰਲਾ = ਨਕਾਬ;
  ਨਕਾਬ = ਨਿਰਾ;
  *ਬਾਹਰਲਾ_ਮਾਪ = ਨਕਾਬ_ਮਾਪ;

  ਵਾਪਸ 0;

invalid_ਨਿਵੇਸ਼:
  ਵਾਪਸ crun_make_error (err, 0, "cannot parse input `%s`", str);
}

ਬਣਤਰ channel_fd_pair
{
  ਬਣਤਰ ring_ਬਫਰ *rb;

  ਪੂਰਨ ਵਿੱਚ_fd;
  ਪੂਰਨ ਬਾਹਰਲਾ_fd;

  ਪੂਰਨ infd_epoll_events;
  ਪੂਰਨ outfd_epoll_events;
};

ਬਣਤਰ channel_fd_pair *
channel_fd_pair_new (ਪੂਰਨ ਵਿੱਚ_fd, ਪੂਰਨ ਬਾਹਰਲਾ_fd, size_t ਮਾਪ)
{
  ਬਣਤਰ channel_fd_pair *channel = xmalloc (ਆਕਾਰ (ਬਣਤਰ channel_fd_pair));
  channel->ਵਿੱਚ_fd = ਵਿੱਚ_fd;
  channel->ਬਾਹਰਲਾ_fd = ਬਾਹਰਲਾ_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_ਬਫਰ_make (ਮਾਪ);
  ਵਾਪਸ channel;
}

ਖਾਲੀ
channel_fd_pair_free (ਬਣਤਰ channel_fd_pair *channel)
{
  ਜੇ (channel == ਨਿਰਾ)
    ਵਾਪਸ;

  ring_ਬਫਰ_free (channel->rb);
  ਮੁਕਤ (channel);
}

ਪੂਰਨ
channel_fd_pair_process (ਬਣਤਰ channel_fd_pair *channel, ਪੂਰਨ epollfd, libcrun_error_t *err)
{
  bool is_ਨਿਵੇਸ਼_eagain = false, is_output_eagain = false, repeat;
  ਪੂਰਨ ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  ਲਈ (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      ਜੇ (ring_ਬਫਰ_get_space_ਉਪਲਬਧ (channel->rb) > 0)
        {
          ret = ring_ਬਫਰ_read (channel->rb, channel->ਵਿੱਚ_fd, &is_ਨਿਵੇਸ਼_eagain, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          ਜੇ (ret > 0)
            repeat = true;
        }
      ਜੇ (ring_ਬਫਰ_get_ਅੰਕੜੇ_ਉਪਲਬਧ (channel->rb) > 0)
        {
          ret = ring_ਬਫਰ_write (channel->rb, channel->ਬਾਹਰਲਾ_fd, &is_output_eagain, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          ਜੇ (ret > 0)
            repeat = true;
        }
    }

  ਜੇ (epollfd >= 0)
    {
      size_t ਉਪਲਬਧ = ring_ਬਫਰ_get_space_ਉਪਲਬਧ (channel->rb);
      size_t ਵਰਤਿਆ = ring_ਬਫਰ_get_ਅੰਕੜੇ_ਉਪਲਬਧ (channel->rb);
      ਪੂਰਨ events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (ਉਪਲਬਧ > 0 && ! is_output_eagain) ? (EPOLLIN | (is_ਨਿਵੇਸ਼_eagain ? EPOLLET : 0)) : 0;
      ਜੇ (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->ਵਿੱਚ_fd, events, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (ਵਰਤਿਆ > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      ਜੇ (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->ਬਾਹਰਲਾ_fd, events, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          channel->outfd_epoll_events = events;
        }
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
open_proc (libcrun_error_t *err)
{
  ਪੂਰਨ fd;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_NEW_MOUNT_API
  cleanup_close ਪੂਰਨ fsfd = -1;
  ਪੂਰਨ ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  ਜੇ (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ਨਿਰਾ, ਨਿਰਾ, 0);
      ਜੇ (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          ਜੇ (fd >= 0)
            ਵਾਪਸ fd;
        }
    }
#ਜੇ_ਅੰਤ

  fd = open ("/proc", O_ਰਸਤਾ | O_DIRECTORY | O_CLOEXEC);
  ਜੇ (fd < 0)
    ਵਾਪਸ crun_make_error (err, errno, "open `/proc`");
  ਵਾਪਸ fd;
}

ਪੂਰਨ
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  ਜੇ (! container)
    ਵਾਪਸ open_proc (err);

  ਜੇ (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  ਵਾਪਸ container->proc_fd;
}

ਪੂਰਨ
libcrun_open_proc_file (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ flags, libcrun_error_t *err)
{
  ਪੂਰਨ proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  ਜੇ (proc_fd < 0)
    ਵਾਪਸ proc_fd;

  fd = ਆਰਜ਼ੀ_FAILURE_RETRY (openat (proc_fd, ਰਸਤਾ, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  ਜੇ (! container)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (proc_fd));

  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, saved_errno, "openat `/proc/%s`", ਰਸਤਾ);

  ਵਾਪਸ fd;
}

ਪੂਰਨ
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ flags, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਭਰਿਆ_ਰਸਤਾ = ਨਿਰਾ;
  xasprintf (&ਭਰਿਆ_ਰਸਤਾ, "%d/%s", pid, ਰਸਤਾ);
  ਵਾਪਸ libcrun_open_proc_file (container, ਭਰਿਆ_ਰਸਤਾ, flags, err);
}
