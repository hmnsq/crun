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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ
#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "ring_buffer.h"
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/un.h>
#ಅನ್ತರ್ಭಾವ <sys/socket.h>
#ಅನ್ತರ್ಭಾವ <sys/signalfd.h>
#ಅನ್ತರ್ಭಾವ <sys/epoll.h>
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#ಅನ್ತರ್ಭಾವ <sys/wait.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <grp.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <sys/xattr.h>
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <linux/magic.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <sys/mman.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#ಇಲ್ಲದಿದ್ದರೆ
#  define atomic_long ಅಸ್ಥಿರ ದೀರ್ಘ
#  define atomic_int ಅಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ "syscalls.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ RESOLVE_ಒಳ_ಬೇರು
#  define RESOLVE_ಒಳ_ಬೇರು 0x10
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __NR_close_range
#  define __NR_close_range 436
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __NR_openat2
#  define __NR_openat2 437
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PROC_USER_INIT_INO
#  define PROC_USER_INIT_INO ((ino_t) 0xEFFFFFFD)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು MAX_READLINKS 32
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_OPENAT2_EAGAIN_RETRIES 128

ಪೂರ್ಣಾಂಕ
crun_ಮಾರ್ಗ_exists (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret = access (ಮಾರ್ಗ, F_OK);
  ಆದರೆ (ret < 0)
    {
      ಆದರೆ (errno == ENOENT)
        ಹಿಂತಿರುಗಿ 0;
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "access `%s`", ಮಾರ್ಗ);
    }
  ಹಿಂತಿರುಗಿ 1;
}

ಪೂರ್ಣಾಂಕ
xasprintf (ಅಕ್ಷರ **str, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
  ಪೂರ್ಣಾಂಕ ret;
  va_list ವಾದಗಳು_list;

  va_ಆರಂಭ (ವಾದಗಳು_list, fmt);

  ret = vasprintf (str, fmt, ವಾದಗಳು_list);
  ಆದರೆ (UNLIKELY (ret < 0))
    OOM ();

  va_ಕೊನೆ (ವಾದಗಳು_list);
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
write_file_at_with_flags (ಪೂರ್ಣಾಂಕ dirfd, ಪೂರ್ಣಾಂಕ flags, mode_t ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, size_t len, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಪೂರ್ಣಾಂಕ ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, ಬಹುಲಕ);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s` for writing", name);

  ret = ಸುರಕ್ಷಿತ_write (fd, name, ದತ್ತಾಂಶ, len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ (len > ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ) ? ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ : (ಪೂರ್ಣಾಂಕ) len;
}

ಪೂರ್ಣಾಂಕ
detach_process ()
{
  pid_t pid;
  ಆದರೆ (setsid () < 0)
    ಹಿಂತಿರುಗಿ -1;
  pid = fork ();
  ಆದರೆ (pid < 0)
    ಹಿಂತಿರುಗಿ -1;
  ಆದರೆ (pid != 0)
    ಸುರಕ್ಷಿತ_exit (EXIT_SUCCESS);
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
get_file_type_fd (ಪೂರ್ಣಾಂಕ fd, mode_t *ಬಹುಲಕ)
{
  ರಚನೆ stat st;
  ಪೂರ್ಣಾಂಕ ret;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_STATX
  ರಚನೆ statx stx = {
    0,
  };

  ret = statx (fd, "", AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno == ENOSYS || errno == EINVAL)
        ಹೋಗು fallback;

      ಹಿಂತಿರುಗಿ ret;
    }
  *ಬಹುಲಕ = stx.stx_mode;
  ಹಿಂತಿರುಗಿ ret;

fallback:
#ಆದರೆ_ಅಂತ್ಯ
  ret = fstat (fd, &st);
  *ಬಹುಲಕ = st.st_mode;
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
get_file_type_at (ಪೂರ್ಣಾಂಕ dirfd, mode_t *ಬಹುಲಕ, bool nofollow, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
  ಪೂರ್ಣಾಂಕ ಖಾಲಿಗಳು_ಮಾರ್ಗ = ಮಾರ್ಗ == ಖಾಲಿ ? AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ : 0;
  ರಚನೆ stat st;
  ಪೂರ್ಣಾಂಕ ret;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_STATX
  ರಚನೆ statx stx = {
    0,
  };

  ret = statx (dirfd, ಮಾರ್ಗ ?: "", ಖಾಲಿಗಳು_ಮಾರ್ಗ | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno == ENOSYS || errno == EINVAL)
        ಹೋಗು fallback;

      ಹಿಂತಿರುಗಿ ret;
    }
  *ಬಹುಲಕ = stx.stx_mode;
  ಹಿಂತಿರುಗಿ ret;

fallback:
#ಆದರೆ_ಅಂತ್ಯ
  ret = fstatat (dirfd, ಮಾರ್ಗ ?: "", &st, ಖಾಲಿಗಳು_ಮಾರ್ಗ | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *ಬಹುಲಕ = st.st_mode;
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
get_file_type (mode_t *ಬಹುಲಕ, bool nofollow, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
  ಹಿಂತಿರುಗಿ get_file_type_at (AT_FDCWD, ಬಹುಲಕ, nofollow, ಮಾರ್ಗ);
}

ಪೂರ್ಣಾಂಕ
create_file_if_missing_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, mode_t ಬಹುಲಕ, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, ಬಹುಲಕ);
  ಆದರೆ (fd_write < 0)
    {
      ಪೂರ್ಣಾಂಕ saved_errno = errno;
      mode_t tmp_ಬಹುಲಕ;
      ಪೂರ್ಣಾಂಕ ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_ಬಹುಲಕ, true, file);
      ಆದರೆ (ret == 0 && S_ISREG (tmp_ಬಹುಲಕ))
        ಹಿಂತಿರುಗಿ 0;

      ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ensure_directory_ಆಂತರಿಕ_at (ಪೂರ್ಣಾಂಕ dirfd, ಅಕ್ಷರ *ಮಾರ್ಗ, size_t len, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, libcrun_error_t *err)
{
  ಅಕ್ಷರ *it = ಮಾರ್ಗ + len;
  ಪೂರ್ಣಾಂಕ ret = 0;
  bool ಪೋಷಕ_created = false;

  ಗಾಗಿ (;;)
    {
      ret = mkdirat (dirfd, ಮಾರ್ಗ, ಬಹುಲಕ);
      ಆದರೆ (ret == 0 || errno == EEXIST)
        ಹಿಂತಿರುಗಿ 0;

      ಪೂರ್ಣಾಂಕ saved_errno = errno;
      ಆದರೆ (ಪೋಷಕ_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = ಖಾಲಿ;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (ಮಾರ್ಗ, false, &tmp_err);
          ಆದರೆ (ret > 0)
            ಹಿಂತಿರುಗಿ 0;
          ಆದರೆ (ret < 0)
            crun_error_release (&tmp_err);

          ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "create directory `%s`", ಮಾರ್ಗ);
        }

      ಆಗ (it > ಮಾರ್ಗ && *it != '/')
        {
          it--;
          len--;
        }
      ಆದರೆ (it == ಮಾರ್ಗ)
        ಹಿಂತಿರುಗಿ 0;

      *it = '\0';
      ret = ensure_directory_ಆಂತರಿಕ_at (dirfd, ಮಾರ್ಗ, len - 1, ಬಹುಲಕ, err);
      *it = '/';
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಪೋಷಕ_created = true;
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
crun_ensure_directory_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, bool nofollow, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free ಅಕ್ಷರ *tmp = xstrdup (ಮಾರ್ಗ);
  ret = ensure_directory_ಆಂತರಿಕ_at (dirfd, tmp, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (tmp), ಬಹುಲಕ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = crun_dir_p_at (dirfd, ಮಾರ್ಗ, nofollow, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ret == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
check_fd_is_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fdname, libcrun_error_t *err)
{
  proc_fd_ಮಾರ್ಗ_t fdpath;
  size_t ಮಾರ್ಗ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮಾರ್ಗ);
  ಅಕ್ಷರ link[PATH_MAX];
  ಪೂರ್ಣಾಂಕ ret;

  get_proc_self_fd_ಮಾರ್ಗ (fdpath, fd);
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (readlink (fdpath, link, ಗಾತ್ರ (link)));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "readlink `%s`", fdname);

  ಆದರೆ (((size_t) ret) != ಮಾರ್ಗ_len || memcmp (link, ಮಾರ್ಗ, ಮಾರ್ಗ_len))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
check_fd_under_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, size_t rootfslen, ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fdname, libcrun_error_t *err)
{
  proc_fd_ಮಾರ್ಗ_t fdpath;
  ಅಕ್ಷರ link[PATH_MAX];
  ಪೂರ್ಣಾಂಕ ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  ಆದರೆ (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    ಹಿಂತಿರುಗಿ 0;

  get_proc_self_fd_ಮಾರ್ಗ (fdpath, fd);
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (readlink (fdpath, link, ಗಾತ್ರ (link)));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "readlink `%s`", fdname);

  ಆದರೆ (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  ಹಿಂತಿರುಗಿ 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
ಸ್ಥಿರ ಶೂನ್ಯ
close_and_replace (ಪೂರ್ಣಾಂಕ *oldfd, ಪೂರ್ಣಾಂಕ newfd)
{
  ಆದರೆ (*oldfd >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
ಅಕ್ಷರ *chroot_realpath (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *chroot, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ resolved_ಮಾರ್ಗ[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸುರಕ್ಷಿತ_openat_fallback (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ flags,
                      ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಪೋಷಕ_ಮಾರ್ಗ = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆಯ_component = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *orig_ಮಾರ್ಗ = ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ_ಒಳ_chroot;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಅಕ್ಷರ resolved[PATH_MAX];
  ಅಕ್ಷರ ಬಫರ್[PATH_MAX];
  size_t rootfs_len = ಸುರಕ್ಷಿತ_strlen (rootfs);
  ಪೂರ್ಣಾಂಕ ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  ಆದರೆ (flags & O_NOFOLLOW)
    {
      ಅಕ್ಷರ *sep;

      ಪೋಷಕ_ಮಾರ್ಗ = xstrdup (ಮಾರ್ಗ);
      sep = strrchr (ಪೋಷಕ_ಮಾರ್ಗ, '/');
      ಆದರೆ (sep == ಖಾಲಿ)
        {
          /* No parent directory, the entire path is the last component.  */
          ಕೊನೆಯ_component = ಮಾರ್ಗ;
          ಪೋಷಕ_ಮಾರ್ಗ[0] = '\0';
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sep[1] != '\0')
        {
          *sep = '\0';
          ಕೊನೆಯ_component = ಮಾರ್ಗ + (sep - ಪೋಷಕ_ಮಾರ್ಗ) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      ಆದರೆ (ಕೊನೆಯ_component)
        ಮಾರ್ಗ = ಪೋಷಕ_ಮಾರ್ಗ;
    }

  ಮಾರ್ಗ_ಒಳ_chroot = chroot_realpath (rootfs, ಮಾರ್ಗ, ಬಫರ್);
  ಆದರೆ (ಮಾರ್ಗ_ಒಳ_chroot == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_ಮಾರ್ಗ);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  ಆದರೆ (rootfs_len > 0 && ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (ಮಾರ್ಗ_ಒಳ_chroot, rootfs, rootfs_len) == 0)
    ಮಾರ್ಗ_ಒಳ_chroot += rootfs_len;
  ಮಾರ್ಗ_ಒಳ_chroot = consume_slashes (ಮಾರ್ಗ_ಒಳ_chroot);

  ಆದರೆ (ಕೊನೆಯ_component)
    {
      ret = snprintf (resolved, ಗಾತ್ರ (resolved), "%s%s%s", ಮಾರ್ಗ_ಒಳ_chroot,
                      ಮಾರ್ಗ_ಒಳ_chroot[0] == '\0' ? "" : "/", ಕೊನೆಯ_component);
      ಆದರೆ (UNLIKELY (ret >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (resolved)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_ಮಾರ್ಗ);

      ಮಾರ್ಗ_ಒಳ_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  ಆದರೆ (ಮಾರ್ಗ_ಒಳ_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      ಹಿಂತಿರುಗಿ ret;
    }

  ret = openat (dirfd, ಮಾರ್ಗ_ಒಳ_chroot, flags, ಬಹುಲಕ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", orig_ಮಾರ್ಗ);

  fd = ret;

  ret = check_fd_under_ಮಾರ್ಗ (rootfs, rootfs_len, fd, orig_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = fd;
  fd = -1;
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
ಸುರಕ್ಷಿತ_openat (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
             libcrun_error_t *err)
{
  ಸ್ಥಿರ bool openat2_supported = true;
  ಪೂರ್ಣಾಂಕ retries = MAX_OPENAT2_EAGAIN_RETRIES;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (ಮಾರ್ಗ))
    {
      cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      ಆದರೆ (is_ಖಾಲಿಗಳು_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, ಬಹುಲಕ);
      ಆದರೆ (UNLIKELY (fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      ಆದರೆ (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_ಮಾರ್ಗ (rootfs, fd, ಮಾರ್ಗ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ret = fd;
      fd = -1;
      ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, ಮಾರ್ಗ, flags, ಬಹುಲಕ, RESOLVE_ಒಳ_ಬೇರು);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (errno == EINTR)
            ಹೋಗು repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          ಆದರೆ (errno == EAGAIN && retries-- > 0)
            ಹೋಗು repeat;
          ಆದರೆ (errno == ENOSYS)
            openat2_supported = false;
          ಆದರೆ (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            ಹಿಂತಿರುಗಿ ಸುರಕ್ಷಿತ_openat_fallback (dirfd, rootfs, ಮಾರ್ಗ, flags, ಬಹುಲಕ, err);

          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "openat2 `%s`", ಮಾರ್ಗ);
        }

      ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ ಸುರಕ್ಷಿತ_openat_fallback (dirfd, rootfs, ಮಾರ್ಗ, flags, ಬಹುಲಕ, err);
}

ssize_t
ಸುರಕ್ಷಿತ_readlinkat (ಪೂರ್ಣಾಂಕ dfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಅಕ್ಷರ **ಬಫರ್, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_ಗಾತ್ರ = hint > 0 ? hint + 1 : 512;
  cleanup_free ಅಕ್ಷರ *tmp_buf = ಖಾಲಿ;
  ssize_t ಗಾತ್ರ_2;

  ಮಾಡು
    {
      ಆದರೆ (tmp_buf != ಖಾಲಿ)
        buf_ಗಾತ್ರ += 256;

      tmp_buf = xrealloc (tmp_buf, buf_ಗಾತ್ರ);

      ಗಾತ್ರ_2 = readlinkat (dfd, name, tmp_buf, buf_ಗಾತ್ರ);
      ಆದರೆ (UNLIKELY (ಗಾತ್ರ_2 < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "readlink `%s`", name);
  } ಆಗ (ಗಾತ್ರ_2 == buf_ಗಾತ್ರ);

  /* Always NUL terminate the buffer.  */
  tmp_buf[ಗಾತ್ರ_2] = '\0';

  /* Move ownership to BUFFER.  */
  *ಬಫರ್ = tmp_buf;
  tmp_buf = ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ crun_ಸುರಕ್ಷಿತ_ensure_at (bool do_open, bool dir, bool follow_symlinks, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dirpath,
                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
crun_ಸುರಕ್ಷಿತ_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dirpath, ಪೂರ್ಣಾಂಕ cwd,
                                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *npath, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cur, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rest,
                                 ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ max_readlinks, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಗುರಿ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *new_ಮಾರ್ಗ = ಖಾಲಿ;
  ssize_t ret;

  ret = ಸುರಕ್ಷಿತ_readlinkat (cwd, cur, &ಗುರಿ, 0, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_ಮಾರ್ಗ, "%.*s%s%s%s", ಗುರಿ[0] == '/' ? 0 : (ಪೂರ್ಣಾಂಕ) (cur - npath), npath, ಗುರಿ,
             rest ? "/" : "", rest ? rest : "");

  ಹಿಂತಿರುಗಿ crun_ಸುರಕ್ಷಿತ_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_ಮಾರ್ಗ, ಬಹುಲಕ, max_readlinks - 1, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
crun_ಸುರಕ್ಷಿತ_ensure_at (bool do_open, bool dir, bool follow_symlinks, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dirpath,
                     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ max_readlinks, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ wd_cleanup = -1;
  cleanup_free ಅಕ್ಷರ *npath = ಖಾಲಿ;
  bool ಕೊನೆಯ_component = false;
  size_t ಆಳ = 0;
  ಅಕ್ಷರ *cur;
  ಅಕ್ಷರ *it;
  ಪೂರ್ಣಾಂಕ cwd;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (max_readlinks <= 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, ELOOP, "resolve path `%s`", ಮಾರ್ಗ);

  ಮಾರ್ಗ = consume_slashes (ಮಾರ್ಗ);

  /* Empty path, nothing to do.  */
  ಆದರೆ (*ಮಾರ್ಗ == '\0')
    {
      ಆದರೆ (do_open)
        ಹಿಂತಿರುಗಿ open (dirpath, O_CLOEXEC | O_ಮಾರ್ಗ, 0);
      ಹಿಂತಿರುಗಿ 0;
    }

  npath = xstrdup (ಮಾರ್ಗ);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  ಆಗ (cur)
    {
      ಆದರೆ (it)
        *it = '\0';
      ಇಲ್ಲದಿದ್ದರೆ
        ಕೊನೆಯ_component = true;

      ಆದರೆ (cur[0] == '\0')
        ನಿಲ್ಲಿಸು;

      ಆದರೆ (strcmp (cur, ".") == 0)
        ಹೋಗು ಮುಂದಿನ;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (cur, ".."))
        ಆಳ++;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (ಆಳ)
            ಆಳ--;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              ಹೋಗು ಮುಂದಿನ;
            }
        }

      ಆದರೆ (ಕೊನೆಯ_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              ಆದರೆ (errno == ELOOP)
                ಹಿಂತಿರುಗಿ crun_ಸುರಕ್ಷಿತ_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, ಖಾಲಿ,
                                                        ಬಹುಲಕ, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_ಮಾರ್ಗ | O_NOFOLLOW, 0);
              ಆದರೆ (ret < 0)
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          ಆದರೆ (do_open)
            ಹಿಂತಿರುಗಿ ret;

          close_and_replace (&wd_cleanup, ret);
          ಹಿಂತಿರುಗಿ 0;
        }

      ret = mkdirat (cwd, cur, ಬಹುಲಕ);
      ಆದರೆ (ret < 0)
        {
          mode_t st_mode;

          ಆದರೆ (errno != EEXIST)
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          ಆದರೆ (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            ಹಿಂತಿರುಗಿ crun_ಸುರಕ್ಷಿತ_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : ಖಾಲಿ, ಬಹುಲಕ, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = ಸುರಕ್ಷಿತ_openat (dirfd, dirpath, npath, O_ಮಾರ್ಗ | O_CLOEXEC, 0, err);
      ಆದರೆ (UNLIKELY (cwd < 0))
        ಹಿಂತಿರುಗಿ crun_error_wrap (err, "creating `/%s`", ಮಾರ್ಗ);

      ಆದರೆ (! ಕೊನೆಯ_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, ಖಾಲಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ಪೂರ್ಣಾಂಕ saved_errno = errno;

              close (cwd);
              ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          ಆದರೆ ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", ಮಾರ್ಗ, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    ಮುಂದಿನ:
      ಆದರೆ (it == ಖಾಲಿ)
        ನಿಲ್ಲಿಸು;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  ಆದರೆ (do_open)
    {
      ಆದರೆ (cwd == dirfd)
        {
          ret = dup (dirfd);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "dup `%s`", dirpath);
          ಹಿಂತಿರುಗಿ ret;
        }

      wd_cleanup = -1;
      ಹಿಂತಿರುಗಿ cwd;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
crun_ಸುರಕ್ಷಿತ_create_and_open_ref_at (bool dir, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dirpath, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  /* If the file/dir already exists, just open it.  */
  ret = ಸುರಕ್ಷಿತ_openat (dirfd, dirpath, ಮಾರ್ಗ, O_ಮಾರ್ಗ | O_CLOEXEC, 0, err);
  ಆದರೆ (LIKELY (ret >= 0))
    ಹಿಂತಿರುಗಿ ret;

  crun_error_release (err);
  ಹಿಂತಿರುಗಿ crun_ಸುರಕ್ಷಿತ_ensure_at (true, dir, true, dirfd, dirpath, ಮಾರ್ಗ, ಬಹುಲಕ, MAX_READLINKS, err);
}

ಪೂರ್ಣಾಂಕ
crun_ಸುರಕ್ಷಿತ_ensure_directory_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dirpath, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
                               libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ crun_ಸುರಕ್ಷಿತ_ensure_at (false, true, false, dirfd, dirpath, ಮಾರ್ಗ, ಬಹುಲಕ, MAX_READLINKS, err);
}

ಪೂರ್ಣಾಂಕ
crun_ಸುರಕ್ಷಿತ_ensure_file_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dirpath, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
                          libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ crun_ಸುರಕ್ಷಿತ_ensure_at (false, false, false, dirfd, dirpath, ಮಾರ್ಗ, ಬಹುಲಕ, MAX_READLINKS, err);
}

ಪೂರ್ಣಾಂಕ
crun_ensure_directory (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, bool nofollow, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ crun_ensure_directory_at (AT_FDCWD, ಮಾರ್ಗ, ಬಹುಲಕ, nofollow, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_file_ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ fd, off_t *ಗಾತ್ರ_2)
{
  ರಚನೆ stat st;
  ಪೂರ್ಣಾಂಕ ret;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_STATX
  ರಚನೆ statx stx = {
    0,
  };

  ret = statx (fd, "", AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_ಗಾತ್ರ, &stx);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno == ENOSYS || errno == EINVAL)
        ಹೋಗು fallback;
      ಹಿಂತಿರುಗಿ ret;
    }
  *ಗಾತ್ರ_2 = stx.stx_size;

  ಹಿಂತಿರುಗಿ ret;

fallback:
#ಆದರೆ_ಅಂತ್ಯ
  ret = fstat (fd, &st);
  *ಗಾತ್ರ_2 = st.st_size;
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
crun_dir_p_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool nofollow, libcrun_error_t *err)
{
  mode_t ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = get_file_type_at (dirfd, &ಬಹುಲಕ, nofollow, ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "stat `%s`", ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ S_ISDIR (ಬಹುಲಕ) ? 1 : 0;
}

ಪೂರ್ಣಾಂಕ
crun_dir_p (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool nofollow, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ crun_dir_p_at (AT_FDCWD, ಮಾರ್ಗ, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
check_running_ಒಳ_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  size_t len;
  ಪೂರ್ಣಾಂಕ ret;

  ret = read_all_file ("/proc/self/uid_map", &ಬಫರ್, &len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      ಆದರೆ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಹಿಂತಿರುಗಿ 0;
        }
      ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (ಬಫರ್, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
ಸ್ಥಿರ bool
running_ಒಳ_openvz ()
{
  ಆದರೆ (access ("/proc/vz", F_OK) < 0)
    ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

ಪೂರ್ಣಾಂಕ
check_running_ಒಳ_user_namespace (libcrun_error_t *err)
{
  ಸ್ಥಿರ atomic_int ಸತತ_ಒಳ_userns = -1;
  ರಚನೆ stat st;
  ಪೂರ್ಣಾಂಕ ret;

  ret = ಸತತ_ಒಳ_userns;
  ಆದರೆ (ret >= 0)
    ಹಿಂತಿರುಗಿ ret;

  ret = stat ("/proc/self/ns/user", &st);
  ಆದರೆ (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      ಆದರೆ (st.st_ino == PROC_USER_INIT_INO)
        {
          ಸತತ_ಒಳ_userns = 0;
          ಹಿಂತಿರುಗಿ ಸತತ_ಒಳ_userns;
        }

      ಆದರೆ (! running_ಒಳ_openvz ())
        {
          ಸತತ_ಒಳ_userns = 1;
          ಹಿಂತಿರುಗಿ ಸತತ_ಒಳ_userns;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (UNLIKELY (errno != ENOENT))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_ಒಳ_user_namespace_uid_map (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಸತತ_ಒಳ_userns = ret;
  ಹಿಂತಿರುಗಿ ಸತತ_ಒಳ_userns;
}

ಸ್ಥಿರ size_t
get_ಪುಟ_ಗಾತ್ರ ()
{
  ಸ್ಥಿರ atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  ಆದರೆ (pagesize == 0)
    {
      pagesize = (ದೀರ್ಘ) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  ಹಿಂತಿರುಗಿ (size_t) pagesize;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ selinux_enabled = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ apparmor_enabled = -1;

ಪೂರ್ಣಾಂಕ
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಹೊರಹೋಗು = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  size_t len;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (selinux_enabled >= 0)
    ಹಿಂತಿರುಗಿ selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ fd;

  ret = read_all_fd_with_ಗಾತ್ರ_hint (fd, "mounts", &ಹೊರಹೋಗು, &len, get_ಪುಟ_ಗಾತ್ರ (), err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  selinux_enabled = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (ಹೊರಹೋಗು, "selinux") ? 1 : 0;

  ಹಿಂತಿರುಗಿ selinux_enabled;
}

ಪೂರ್ಣಾಂಕ
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2;
  ಅಕ್ಷರ buf[1];

  ಆದರೆ (apparmor_enabled >= 0)
    ಹಿಂತಿರುಗಿ apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  ಆದರೆ (fd == -1)
    {
      ಆದರೆ (errno == ENOENT)
        {
          apparmor_enabled = 0;
          ಹಿಂತಿರುಗಿ 0;
        }

      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  ಗಾತ್ರ_2 = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (fd, buf, ಗಾತ್ರ (buf)));

  apparmor_enabled = ಗಾತ್ರ_2 > 0 && buf[0] == 'Y' ? 1 : 0;

  ಹಿಂತಿರುಗಿ apparmor_enabled;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  ಆದರೆ (selinux_enabled < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "SELinux is not initialized correctly");
  ಹಿಂತಿರುಗಿ selinux_enabled;
}

ಪೂರ್ಣಾಂಕ
ಕೂಡಿಸು_selinux_mount_label (ಅಕ್ಷರ **retlabel, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *context_type, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_is_selinux_enabled (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (label && ret)
    {
      ಆದರೆ (ದತ್ತಾಂಶ && *ದತ್ತಾಂಶ)
        xasprintf (retlabel, "%s,%s=\"%s\"", ದತ್ತಾಂಶ, context_type, label);
      ಇಲ್ಲದಿದ್ದರೆ
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      ಹಿಂತಿರುಗಿ 0;
    }
  *retlabel = xstrdup (ದತ್ತಾಂಶ);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
lsm_attr_ಮಾರ್ಗ (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *lsm, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fname, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ attr_dirfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ lsm_dirfd = -1;
  ಅಕ್ಷರ *attr_ಮಾರ್ಗ = ಖಾಲಿ;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (attr_dirfd < 0))
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  ಆದರೆ (lsm != ಖಾಲಿ)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);

      ಆದರೆ (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_ಮಾರ್ಗ, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  ಹಿಂತಿರುಗಿ attr_ಮಾರ್ಗ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
check_proc_super_magic (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  ರಚನೆ statfs sfs;

  ಪೂರ್ಣಾಂಕ ret = fstatfs (fd, &sfs);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "statfs `%s`", ಮಾರ್ಗ);

  ಆದರೆ (sfs.f_type != PROC_SUPER_MAGIC)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಗಣ_security_attr (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *lsm, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fname, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ದತ್ತಾಂಶ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  cleanup_free ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *attr_ಮಾರ್ಗ = lsm_attr_ಮಾರ್ಗ (container, lsm, fname, err);
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

  ಆದರೆ (UNLIKELY (attr_ಮಾರ್ಗ == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ -1;

  fd = libcrun_open_proc_file (container, attr_ಮಾರ್ಗ, O_WRONLY | O_CLOEXEC, err);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ fd;

  ret = check_proc_super_magic (fd, attr_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  // Write out data
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, ದತ್ತಾಂಶ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ದತ್ತಾಂಶ)));
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      ಆದರೆ (errno == ENOENT && lsm != ಖಾಲಿ && strcmp (lsm, "apparmor") == 0)
        {
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *profile = strchr (ದತ್ತಾಂಶ, ' ');

          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : ದತ್ತಾಂಶ);
        }
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to file `%s`", attr_ಮಾರ್ಗ);
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
ಗಣ_selinux_label (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label, bool now, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_is_selinux_enabled (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ret)
    ಹಿಂತಿರುಗಿ ಗಣ_security_attr (container, ಖಾಲಿ, now ? "current" : "exec", label, err);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  ಆದರೆ (apparmor_enabled < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "AppArmor is not initialized correctly");
  ಹಿಂತಿರುಗಿ apparmor_enabled;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿಸು UNCONFINED "unconfined"
#ವ್ಯಾಖ್ಯಾನಿಸು UNCONFINED_LEN (ssize_t) (ಗಾತ್ರ (UNCONFINED) - 1)
  cleanup_free ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *attr_ಮಾರ್ಗ = lsm_attr_ಮಾರ್ಗ (container, "apparmor", "current", err);
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  ಅಕ್ಷರ buf[UNCONFINED_LEN + 1];

  ಆದರೆ (UNLIKELY (attr_ಮಾರ್ಗ == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ -1;

  fd = libcrun_open_proc_file (container, attr_ಮಾರ್ಗ, O_RDONLY | O_CLOEXEC, err);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ fd;

  ಆದರೆ (UNLIKELY (check_proc_super_magic (fd, attr_ಮಾರ್ಗ, err)))
    ಹಿಂತಿರುಗಿ -1;

  ssize_t ಬೈಟುಗಳು_read = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (fd, buf, ಗಾತ್ರ (buf)));
  ಆದರೆ (UNLIKELY (ಬೈಟುಗಳು_read < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from `%s`", attr_ಮಾರ್ಗ);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  ಆದರೆ (ಬೈಟುಗಳು_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    ಹಿಂತಿರುಗಿ 1;
  ಆದರೆ (ಬೈಟುಗಳು_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    ಹಿಂತಿರುಗಿ 0;
  ಹಿಂತಿರುಗಿ 1;
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು UNCONFINED
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು UNCONFINED_LEN
}

ಪೂರ್ಣಾಂಕ
ಗಣ_apparmor_profile (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_is_apparmor_enabled (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ret)
    {
      cleanup_free ಅಕ್ಷರ *buf = ಖಾಲಿ;
      ret = is_current_process_confined (container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      ಹಿಂತಿರುಗಿ ಗಣ_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_all_fd_ಆಂತರಿಕ (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *description, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *len, size_t ಗಾತ್ರ_hint, size_t max_ಗಾತ್ರ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *buf = ಖಾಲಿ;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t ಗಾತ್ರ_2 = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (ಗಾತ್ರ_hint)
    allocated = ಗಾತ್ರ_hint;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = get_file_ಗಾತ್ರ (fd, &ಗಾತ್ರ_2);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "stat `%s`", description);

      ಆದರೆ (max_ಗಾತ್ರ && (size_t) ಗಾತ್ರ_2 > max_ಗಾತ್ರ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_ಗಾತ್ರ);

      allocated = ಗಾತ್ರ_2 == 0 ? 1023 : ಗಾತ್ರ_2;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  ಆಗ ((ಗಾತ್ರ_2 && nread < (size_t) ಗಾತ್ರ_2) || ಗಾತ್ರ_2 == 0)
    {
      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (fd, buf + nread, allocated - nread));
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from file `%s`", description);

      ಆದರೆ (ret == 0)
        ನಿಲ್ಲಿಸು;

      nread += ret;

      ಆದರೆ (max_ಗಾತ್ರ && nread > max_ಗಾತ್ರ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_ಗಾತ್ರ);

      ಆದರೆ (nread == allocated)
        {
          ಆದರೆ (ಗಾತ್ರ_2)
            ನಿಲ್ಲಿಸು;

          ಆದರೆ (pagesize == 0)
            pagesize = get_ಪುಟ_ಗಾತ್ರ ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  ಆದರೆ (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      ಅಕ್ಷರ *tmp = realloc (buf, nread + 1);
      ಆದರೆ (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *ಹೊರಹೋಗು = buf;
  buf = ಖಾಲಿ;
  ಆದರೆ (len)
    *len = nread;
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
read_all_fd_with_ಗಾತ್ರ_hint (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *description, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *len, size_t ಗಾತ್ರ_hint, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ read_all_fd_ಆಂತರಿಕ (fd, description, ಹೊರಹೋಗು, len, ಗಾತ್ರ_hint, 0, err);
}

ಪೂರ್ಣಾಂಕ
read_all_fd_with_max_ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *description, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *len, size_t max_ಗಾತ್ರ, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ read_all_fd_ಆಂತರಿಕ (fd, description, ಹೊರಹೋಗು, len, 0, max_ಗಾತ್ರ, err);
}

ಪೂರ್ಣಾಂಕ
read_all_file_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *len, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

  fd = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (openat (dirfd, ಮಾರ್ಗ, O_RDONLY | O_CLOEXEC));
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ read_all_fd (fd, ಮಾರ್ಗ, ಹೊರಹೋಗು, len, err);
}

ಪೂರ್ಣಾಂಕ
read_all_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *len, libcrun_error_t *err)
{
  ಆದರೆ (strcmp (ಮಾರ್ಗ, "-") == 0)
    ಮಾರ್ಗ = "/dev/stdin";

  ಹಿಂತಿರುಗಿ read_all_file_at (AT_FDCWD, ಮಾರ್ಗ, ಹೊರಹೋಗು, len, err);
}

ಪೂರ್ಣಾಂಕ
get_realpath_ಗಮ್ಯ_file (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ_name, ಅಕ್ಷರ **absolute_ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ targetfd = -1;

  targetfd = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (openat (dirfd, ಮಾರ್ಗ_name, O_RDONLY | O_CLOEXEC));
  ಆದರೆ (UNLIKELY (targetfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ಮಾರ್ಗ_name);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ssize_t len;
      proc_fd_ಮಾರ್ಗ_t ಗುರಿ_fd_ಮಾರ್ಗ;

      get_proc_self_fd_ಮಾರ್ಗ (ಗುರಿ_fd_ಮಾರ್ಗ, targetfd);
      len = ಸುರಕ್ಷಿತ_readlinkat (AT_FDCWD, ಗುರಿ_fd_ಮಾರ್ಗ, absolute_ಮಾರ್ಗ, 0, err);
      ಆದರೆ (UNLIKELY (len < 0))
        ಹಿಂತಿರುಗಿ crun_error_wrap (err, "error unable to provide absolute path to file `%s`", ಮಾರ್ಗ_name);
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
open_unix_domain_client_socket (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ dgram, libcrun_error_t *err)
{
  ರಚನೆ sockaddr_un addr = {};
  ಪೂರ್ಣಾಂಕ ret;
  proc_fd_ಮಾರ್ಗ_t name_buf;
  cleanup_close ಪೂರ್ಣಾಂಕ destfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", ಮಾರ್ಗ);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_ಹರಿವು, 0);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "create UNIX socket");

  ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮಾರ್ಗ) >= ಗಾತ್ರ (addr.sun_ಮಾರ್ಗ))
    {
      destfd = open (ಮಾರ್ಗ, O_ಮಾರ್ಗ | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (destfd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ಮಾರ್ಗ);

      get_proc_self_fd_ಮಾರ್ಗ (name_buf, destfd);

      ಮಾರ್ಗ = name_buf;
    }

  size_t ಮಾರ್ಗ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮಾರ್ಗ);
  ಆದರೆ (ಮಾರ್ಗ_len >= ಗಾತ್ರ (addr.sun_ಮಾರ್ಗ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", ಮಾರ್ಗ);

  ಸ್ಮರಣೆ_ನಕಲು (addr.sun_ಮಾರ್ಗ, ಮಾರ್ಗ, ಮಾರ್ಗ_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (ರಚನೆ sockaddr *) &addr, ಗಾತ್ರ (addr));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "connect socket to `%s`", ಮಾರ್ಗ);

  ret = fd;
  fd = -1;

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
open_unix_domain_socket (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ dgram, libcrun_error_t *err)
{
  ರಚನೆ sockaddr_un addr = {};
  proc_fd_ಮಾರ್ಗ_t name_buf;
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_close ಪೂರ್ಣಾಂಕ dirfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_ಹರಿವು, 0);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "create UNIX socket");

  ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮಾರ್ಗ) >= ಗಾತ್ರ (addr.sun_ಮಾರ್ಗ))
    {
      cleanup_free ಅಕ್ಷರ *dpath = xstrdup (ಮಾರ್ಗ);
      cleanup_free ಅಕ್ಷರ *bpath = xstrdup (ಮಾರ್ಗ);
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೋಷಕ_dir = dirname (dpath);
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ = basename (bpath);
      ಪೂರ್ಣಾಂಕ n;

      dirfd = open (ಪೋಷಕ_dir, O_ಮಾರ್ಗ | O_DIRECTORY | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (dirfd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open directory `%s`", ಪೋಷಕ_dir);

      get_proc_self_fd_ಮಾರ್ಗ (name_buf, dirfd);

      n = snprintf (addr.sun_ಮಾರ್ಗ, ಗಾತ್ರ (addr.sun_ಮಾರ್ಗ), "%s/%s", name_buf, ತಳ);
      ಆದರೆ (n < 0 || (size_t) n >= ಗಾತ್ರ (addr.sun_ಮಾರ್ಗ))
        ಹಿಂತಿರುಗಿ crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", ಮಾರ್ಗ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      size_t ಮಾರ್ಗ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮಾರ್ಗ);
      ಸ್ಮರಣೆ_ನಕಲು (addr.sun_ಮಾರ್ಗ, ಮಾರ್ಗ, ಮಾರ್ಗ_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (ರಚನೆ sockaddr *) &addr, ಗಾತ್ರ (addr));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bind socket to `%s`", ಮಾರ್ಗ);

  ಆದರೆ (! dgram)
    {
      ret = listen (fd, 1);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
send_fd_ಗಮ್ಯ_socket (ಪೂರ್ಣಾಂಕ server, ಪೂರ್ಣಾಂಕ fd, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ send_fd_ಗಮ್ಯ_socket_with_payload (server, fd, ಖಾಲಿ, 0, err);
}

ಪೂರ್ಣಾಂಕ
send_fd_ಗಮ್ಯ_socket_with_payload (ಪೂರ್ಣಾಂಕ server, ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *payload, size_t payload_len, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ರಚನೆ cmsghdr *cmsg = ಖಾಲಿ;
  ರಚನೆ iovec iov[2];
  ರಚನೆ msghdr ಸಂದೇಶ = {};
  ಅಕ್ಷರ ctrl_buf[CMSG_SPACE (1 + ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ))] = {};
  ಅಕ್ಷರ ದತ್ತಾಂಶ[1];

  ದತ್ತಾಂಶ[0] = ' ';
  iov[0].iov_base = ದತ್ತಾಂಶ;
  iov[0].iov_len = ಗಾತ್ರ (ದತ್ತಾಂಶ);

  ಆದರೆ (payload_len > 0)
    {
      iov[0].iov_base = (ಶೂನ್ಯ *) payload;
      iov[0].iov_len = payload_len;
    }

  ಸಂದೇಶ.ಸಂದೇಶ_name = ಖಾಲಿ;
  ಸಂದೇಶ.ಸಂದೇಶ_namelen = 0;
  ಸಂದೇಶ.ಸಂದೇಶ_iov = iov;
  ಸಂದೇಶ.ಸಂದೇಶ_iovlen = 1;
  ಸಂದೇಶ.ಸಂದೇಶ_controllen = CMSG_SPACE (ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ));
  ಸಂದೇಶ.ಸಂದೇಶ_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&ಸಂದೇಶ);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ));

  *((ಪೂರ್ಣಾಂಕ *) CMSG_ದತ್ತಾಂಶ (cmsg)) = fd;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (sendmsg (server, &ಸಂದೇಶ, 0));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sendmsg");
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
receive_fd_from_socket_with_payload (ಪೂರ್ಣಾಂಕ from, ಅಕ್ಷರ *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಪೂರ್ಣಾಂಕ ret;
  ರಚನೆ iovec iov[1];
  ರಚನೆ msghdr ಸಂದೇಶ = {};
  ಅಕ್ಷರ ctrl_buf[CMSG_SPACE (ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ))] = {};
  ಅಕ್ಷರ ದತ್ತಾಂಶ[1];
  ರಚನೆ cmsghdr *cmsg;

  ದತ್ತಾಂಶ[0] = ' ';
  iov[0].iov_base = ದತ್ತಾಂಶ;
  iov[0].iov_len = ಗಾತ್ರ (ದತ್ತಾಂಶ);

  ಆದರೆ (payload_len > 0)
    {
      iov[0].iov_base = (ಶೂನ್ಯ *) payload;
      iov[0].iov_len = payload_len;
    }

  ಸಂದೇಶ.ಸಂದೇಶ_name = ಖಾಲಿ;
  ಸಂದೇಶ.ಸಂದೇಶ_namelen = 0;
  ಸಂದೇಶ.ಸಂದೇಶ_iov = iov;
  ಸಂದೇಶ.ಸಂದೇಶ_iovlen = 1;
  ಸಂದೇಶ.ಸಂದೇಶ_controllen = CMSG_SPACE (ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ));
  ಸಂದೇಶ.ಸಂದೇಶ_control = ctrl_buf;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (recvmsg (from, &ಸಂದೇಶ, 0));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "recvmsg");
  ಆದರೆ (UNLIKELY (ret == 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&ಸಂದೇಶ);
  ಆದರೆ (cmsg == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "no msg received");
  ಸ್ಮರಣೆ_ನಕಲು (&fd, CMSG_ದತ್ತಾಂಶ (cmsg), ಗಾತ್ರ (fd));

  ret = fd;
  fd = -1;
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
receive_fd_from_socket (ಪೂರ್ಣಾಂಕ from, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ receive_fd_from_socket_with_payload (from, ಖಾಲಿ, 0, err);
}

ಪೂರ್ಣಾಂಕ
create_socket_pair (ಪೂರ್ಣಾಂಕ *pair, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "socketpair");
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
create_signalfd (sigset_t *ಮುಖವಾಡ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret = signalfd (-1, ಮುಖವಾಡ, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "signalfd");
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
epoll_helper_toggle (ಪೂರ್ಣಾಂಕ epollfd, ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ events, libcrun_error_t *err)
{
  ರಚನೆ epoll_event ev = {};
  bool ಕೂಡಿಸು = events != 0;
  ಪೂರ್ಣಾಂಕ ret;

  ev.events = events;
  ev.ದತ್ತಾಂಶ.fd = fd;
  ret = epoll_ctl (epollfd, ಕೂಡಿಸು ? EPOLL_CTL_ಕೂಡಿಸು : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  ಆದರೆ (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      ಆದರೆ (errno == ENOENT || errno == EPERM)
        ಹಿಂತಿರುಗಿ 0;
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "epoll_ctl `%s` `%d`", ಕೂಡಿಸು ? "add" : "del", fd);
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
epoll_helper (ಪೂರ್ಣಾಂಕ *ಒಳ_fds, ಪೂರ್ಣಾಂಕ *ಒಳ_levelfds, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_fds, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_levelfds, libcrun_error_t *err)
{
  ರಚನೆ epoll_event ev;
  cleanup_close ಪೂರ್ಣಾಂಕ epollfd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  ಪೂರ್ಣಾಂಕ *it;
  epollfd = epoll_create1 (0);
  ಆದರೆ (UNLIKELY (epollfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "epoll_create1");

#ವ್ಯಾಖ್ಯಾನಿಸು ಕೂಡಿಸು_FDS(FDS, EVENTS)                                            \
  ಗಾಗಿ (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.ದತ್ತಾಂಶ.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ಕೂಡಿಸು, *it, &ev);               \
      ಆದರೆ (UNLIKELY (ret < 0))                                           \
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  ಆದರೆ (ಒಳ_fds)
    ಕೂಡಿಸು_FDS (ಒಳ_fds, EPOLLIN);
  ಆದರೆ (ಒಳ_levelfds)
    ಕೂಡಿಸು_FDS (ಒಳ_levelfds, EPOLLIN | EPOLLET);
  ಆದರೆ (ಹೊರಹೋಗು_fds)
    ಕೂಡಿಸು_FDS (ಹೊರಹೋಗು_fds, EPOLLOUT);
  ಆದರೆ (ಹೊರಹೋಗು_levelfds)
    ಕೂಡಿಸು_FDS (ಹೊರಹೋಗು_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
copy_from_fd_ಗಮ್ಯ_fd (ಪೂರ್ಣಾಂಕ src, ಪೂರ್ಣಾಂಕ dst, ಪೂರ್ಣಾಂಕ consume, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ssize_t nread;
  size_t pagesize = get_ಪುಟ_ಗಾತ್ರ ();
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#ಆದರೆ_ಅಂತ್ಯ
  ಮಾಡು
    {
      cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
      ssize_t ಉಳಿದ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_COPY_FILE_RANGE
      ಆದರೆ (can_copy_file_range)
        {
          nread = copy_file_range (src, ಖಾಲಿ, dst, ಖಾಲಿ, pagesize, 0);
          ಆದರೆ (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              ಹೋಗು fallback;
            }
          ಆದರೆ (consume && nread < 0 && errno == EAGAIN)
            ಹಿಂತಿರುಗಿ 0;
          ಆದರೆ (nread < 0 && errno == EIO)
            ಹಿಂತಿರುಗಿ 0;
          ಆದರೆ (UNLIKELY (nread < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "copy_file_range");
          ಮುಂದುವರಿ;
        }
    fallback:
#ಆದರೆ_ಅಂತ್ಯ

      ಬಫರ್ = xmalloc (pagesize);
      nread = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (src, ಬಫರ್, pagesize));
      ಆದರೆ (consume && nread < 0 && errno == EAGAIN)
        ಹಿಂತಿರುಗಿ 0;
      ಆದರೆ (nread < 0 && errno == EIO)
        ಹಿಂತಿರುಗಿ 0;
      ಆದರೆ (UNLIKELY (nread < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read");

      ಉಳಿದ = nread;
      ಆಗ (ಉಳಿದ)
        {
          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (dst, ಬಫರ್ + nread - ಉಳಿದ, ಉಳಿದ));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write");
          ಉಳಿದ -= ret;
        }
  } ಆಗ (consume && nread);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
ಸತತ_process (ಅಕ್ಷರ **ವಾದಗಳು, libcrun_error_t *err)
{
  pid_t pid = fork ();
  ಆದರೆ (UNLIKELY (pid < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fork");
  ಆದರೆ (pid)
    {
      ಪೂರ್ಣಾಂಕ r, ಸ್ಥಿತಿ_2;
      r = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (waitpid (pid, &ಸ್ಥಿತಿ_2, 0));
      ಆದರೆ (r < 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "waitpid");
      ಆದರೆ (WIFEXITED (ಸ್ಥಿತಿ_2) || WIFSIGNALED (ಸ್ಥಿತಿ_2))
        ಹಿಂತಿರುಗಿ WEXITSTATUS (ಸ್ಥಿತಿ_2);
    }

  execvp (ವಾದಗಳು[0], ವಾದಗಳು);
  ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_FGETPWENT_R
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ
atou (ಅಕ್ಷರ **s)
{
  ಚಿಹ್ನೆರಹಿತ x;
  ಗಾಗಿ (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  ಹಿಂತಿರುಗಿ x;
}

ಪೂರ್ಣಾಂಕ
fgetpwent_r (FILE *f, ರಚನೆ passwd *pw, ಅಕ್ಷರ *ಸಾಲು, size_t ಗಾತ್ರ_2, ರಚನೆ passwd **res)
{
  ಅಕ್ಷರ *s;
  ಪೂರ್ಣಾಂಕ rv = 0;
  ಗಾಗಿ (;;)
    {
      ಸಾಲು[ಗಾತ್ರ_2 - 1] = '\xff';
      ಆದರೆ ((fgets (ಸಾಲು, ಗಾತ್ರ_2, f) == ಖಾಲಿ) || ferror (f) || ಸಾಲು[ಗಾತ್ರ_2 - 1] != '\xff')
        {
          rv = (ಸಾಲು[ಗಾತ್ರ_2 - 1] != '\xff') ? ERANGE : ENOENT;
          ಸಾಲು = 0;
          pw = 0;
          ನಿಲ್ಲಿಸು;
        }
      ಸಾಲು[strcspn (ಸಾಲು, "\n")] = 0;

      s = ಸಾಲು;
      pw->pw_name = s++;
      ಆದರೆ (! (s = strchr (s, ':')))
        ಮುಂದುವರಿ;

      *s++ = 0;
      pw->pw_passwd = s;
      ಆದರೆ (! (s = strchr (s, ':')))
        ಮುಂದುವರಿ;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      ಆದರೆ (*s != ':')
        ಮುಂದುವರಿ;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      ಆದರೆ (*s != ':')
        ಮುಂದುವರಿ;

      *s++ = 0;
      pw->pw_gecos = s;
      ಆದರೆ (! (s = strchr (s, ':')))
        ಮುಂದುವರಿ;

      *s++ = 0;
      pw->pw_dir = s;
      ಆದರೆ (! (s = strchr (s, ':')))
        ಮುಂದುವರಿ;

      *s++ = 0;
      pw->pw_shell = s;
      ನಿಲ್ಲಿಸು;
    }
  *res = pw;
  ಆದರೆ (rv)
    errno = rv;
  ಹಿಂತಿರುಗಿ rv;
}
#ಆದರೆ_ಅಂತ್ಯ

ಪೂರ್ಣಾಂಕ
ಗಣ_home_env (uid_t id)
{
  ರಚನೆ passwd pwd;
  cleanup_free ಅಕ್ಷರ *buf = ಖಾಲಿ;
  ದೀರ್ಘ buf_ಗಾತ್ರ;
  cleanup_file FILE *ಹರಿವು = ಖಾಲಿ;

  buf_ಗಾತ್ರ = sysconf (_SC_GETPW_R_SIZE_MAX);
  ಆದರೆ (buf_ಗಾತ್ರ < 0)
    buf_ಗಾತ್ರ = 1024;

  buf = xmalloc (buf_ಗಾತ್ರ);

  ಹರಿವು = fopen ("/etc/passwd", "re");
  ಆದರೆ (ಹರಿವು == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ -1;

  ಗಾಗಿ (;;)
    {
      ರಚನೆ passwd *ret_pw = ಖಾಲಿ;

      ಪೂರ್ಣಾಂಕ ret = fgetpwent_r (ಹರಿವು, &pwd, buf, buf_ಗಾತ್ರ, &ret_pw);
      ಆದರೆ (UNLIKELY (ret != 0))
        {
          ಆದರೆ (ret == EINTR)
            ಮುಂದುವರಿ;

          ಆದರೆ (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              ಹಿಂತಿರುಗಿ -1;
            }

          buf_ಗಾತ್ರ *= 2;
          buf = xrealloc (buf, buf_ಗಾತ್ರ);
          ಮುಂದುವರಿ;
        }

      ಆದರೆ (ret_pw && ret_pw->pw_uid == id)
        {
          ಆದರೆ (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
getsubidrange (uid_t id, ಪೂರ್ಣಾಂಕ is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *ಒಳಹರಿವು = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *lineptr = ಖಾಲಿ;
  size_t lenlineptr = 0, len_name;
  ದೀರ್ಘ buf_ಗಾತ್ರ;
  cleanup_free ಅಕ್ಷರ *buf = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  ರಚನೆ passwd pwd;

  buf_ಗಾತ್ರ = sysconf (_SC_GETPW_R_SIZE_MAX);
  ಆದರೆ (buf_ಗಾತ್ರ < 0)
    buf_ಗಾತ್ರ = 1024;

  buf = xmalloc (buf_ಗಾತ್ರ);
  ಗಾಗಿ (;;)
    {
      ಪೂರ್ಣಾಂಕ ret;
      ರಚನೆ passwd *ret_pw = ಖಾಲಿ;

      ret = getpwuid_r (id, &pwd, buf, buf_ಗಾತ್ರ, &ret_pw);
      ಆದರೆ (LIKELY (ret == 0))
        {
          ಆದರೆ (ret_pw)
            {
              name = ret_pw->pw_name;
              ನಿಲ್ಲಿಸು;
            }
          ಹಿಂತಿರುಗಿ -1;
        }

      ಆದರೆ (ret != ERANGE)
        ಹಿಂತಿರುಗಿ -1;

      buf_ಗಾತ್ರ *= 2;
      buf = xrealloc (buf, buf_ಗಾತ್ರ);
    }

  len_name = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (name);

  ಒಳಹರಿವು = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  ಆದರೆ (ಒಳಹರಿವು == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ -1;

  ಗಾಗಿ (;;)
    {
      ಅಕ್ಷರ *endptr;
      ssize_t ಪಠ = getline (&lineptr, &lenlineptr, ಒಳಹರಿವು);
      ಆದರೆ (ಪಠ < 0)
        ಹಿಂತಿರುಗಿ -1;

      ಆದರೆ (ಪಠ < (ssize_t) (len_name + 2))
        ಮುಂದುವರಿ;

      ಆದರೆ (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        ಮುಂದುವರಿ;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      ಆದರೆ (endptr >= &lineptr[ಪಠ])
        ಹಿಂತಿರುಗಿ -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      ಹಿಂತಿರುಗಿ 0;
    }
}

#ವ್ಯಾಖ್ಯಾನಿಸು MIN(x, y) ((x) < (y) ? (x) : (y))

ಪೂರ್ಣಾಂಕ
format_default_id_mapping (ಅಕ್ಷರ **ಹೊರಹೋಗು, uid_t container_id, uid_t host_uid, uid_t host_id, ಪೂರ್ಣಾಂಕ is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, ಲಭ್ಯ = 0;
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ written = 0;
  ಪೂರ್ಣಾಂಕ ret, ಉಳಿದ;

  *ಹೊರಹೋಗು = ಖಾಲಿ;

  ಆದರೆ (getsubidrange (host_uid, is_uid, &from, &ಲಭ್ಯ) < 0)
    ಹಿಂತಿರುಗಿ 0;

  /* More than enough space for all the mappings.  */
  ಉಳಿದ = 15 * 5 * 3;
  ಬಫರ್ = xmalloc (ಉಳಿದ + 1);

  ಆದರೆ (container_id > 0)
    {
      uint32_t ಬಳಸಿದ = MIN (container_id, ಲಭ್ಯ);
      ret = snprintf (ಬಫರ್ + written, ಉಳಿದ, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, ಬಳಸಿದ);
      ಆದರೆ (UNLIKELY (ret >= ಉಳಿದ))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      ಉಳಿದ -= ret;

      from += ಬಳಸಿದ;
      ಲಭ್ಯ -= ಬಳಸಿದ;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (ಬಫರ್ + written, ಉಳಿದ, "%d %d 1\n", container_id, host_id);
  ಆದರೆ (UNLIKELY (ret >= ಉಳಿದ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  ಉಳಿದ -= ret;

  /* Last mapping: use any id that is left.  */
  ಆದರೆ (ಲಭ್ಯ)
    {
      ret = snprintf (ಬಫರ್ + written, ಉಳಿದ, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, ಲಭ್ಯ);
      ಆದರೆ (UNLIKELY (ret >= ಉಳಿದ))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      ಉಳಿದ -= ret;
    }

  *ಹೊರಹೋಗು = ಬಫರ್;
  ಬಫರ್ = ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ written;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
unset_cloexec_flag (ಪೂರ್ಣಾಂಕ fd)
{
  ಪೂರ್ಣಾಂಕ flags = fcntl (fd, F_GETFD);
  ಆದರೆ (flags == -1)
    ಹಿಂತಿರುಗಿ -1;

  flags &= ~FD_CLOEXEC;

  ಹಿಂತಿರುಗಿ fcntl (fd, F_SETFD, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ __attribute__ ((__noreturn__))
ಸತತ_process_ಮಗು (ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ **ವಾದಗಳು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cwd, ಅಕ್ಷರ **envp, ಪೂರ್ಣಾಂಕ pipe_r,
                   ಪೂರ್ಣಾಂಕ pipe_w, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_fd, ಪೂರ್ಣಾಂಕ err_fd, bool can_ignore_chdir_errors)
{
  ಅಕ್ಷರ *tmp_ವಾದಗಳು[] = { ಮಾರ್ಗ, ಖಾಲಿ };
  libcrun_error_t err = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ dev_null_fd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  ret = mark_or_close_fds_ge_than (ಖಾಲಿ, 3, false, &err);
  ಆದರೆ (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->ಸ್ಥಿತಿ_2, "%s", (err)->ಸಂದೇಶ);

  ಆದರೆ (ಹೊರಹೋಗು_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (dev_null_fd < 0))
        ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
    }

  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (pipe_r));

  dup2 (ಹೊರಹೋಗು_fd >= 0 ? ಹೊರಹೋಗು_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  ಆದರೆ (ಹೊರಹೋಗು_fd >= 0)
    unset_cloexec_flag (1);
  ಆದರೆ (err_fd >= 0)
    unset_cloexec_flag (2);

  ಆದರೆ (dev_null_fd >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (dev_null_fd));
  ಆದರೆ (ಹೊರಹೋಗು_fd >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (ಹೊರಹೋಗು_fd));
  ಆದರೆ (err_fd >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (err_fd));

  ಆದರೆ (ವಾದಗಳು == ಖಾಲಿ)
    ವಾದಗಳು = tmp_ವಾದಗಳು;

  ಆದರೆ (cwd && chdir (cwd) < 0)
    {
      ಆದರೆ (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
    }

  execvpe (ಮಾರ್ಗ, ವಾದಗಳು, envp);
  ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
ಪೂರ್ಣಾಂಕ
ಸತತ_process_with_stdin_timeout_envp (ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ **ವಾದಗಳು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cwd, ಪೂರ್ಣಾಂಕ timeout,
                                     ಅಕ್ಷರ **envp, ಅಕ್ಷರ *stdin, size_t stdin_len, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_fd,
                                     ಪೂರ್ಣಾಂಕ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ stdin_pipe[2];
  pid_t pid;
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_close ಪೂರ್ಣಾಂಕ pipe_r = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ pipe_w = -1;
  sigset_t oldmask, ಮುಖವಾಡ;
  ಪೂರ್ಣಾಂಕ r, ಸ್ಥಿತಿ_2;

  sigemptyset (&ಮುಖವಾಡ);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  ಆದರೆ (timeout > 0)
    {
      sigaddset (&ಮುಖವಾಡ, SIGCHLD);
      ret = sigprocmask (SIG_ಖಂಡ, &ಮುಖವಾಡ, &oldmask);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  ಆದರೆ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      ಹೋಗು restore_sig_ಮುಖವಾಡ_and_exit;
    }

  ಆದರೆ (pid == 0)
    {
      /* run_process_child doesn't return.  */
      ಸತತ_process_ಮಗು (ಮಾರ್ಗ, ವಾದಗಳು, cwd, envp, pipe_r, pipe_w, ಹೊರಹೋಗು_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (pipe_w, stdin, stdin_len));
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      ಆದರೆ (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          ಹೋಗು restore_sig_ಮುಖವಾಡ_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  ಆದರೆ (timeout)
    {
      time_t ಆರಂಭ = ಸಮಯ (ಖಾಲಿ);
      time_t now;
      ಗಾಗಿ (now = ಆರಂಭ; now - ಆರಂಭ < timeout; now = ಸಮಯ (ಖಾಲಿ))
        {
          siginfo_t info;
          ಪೂರ್ಣಾಂಕ elapsed = now - ಆರಂಭ;
          ರಚನೆ timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&ಮುಖವಾಡ, &info, &ts_timeout);
          ಆದರೆ (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              ಹೋಗು restore_sig_ಮುಖವಾಡ_and_exit;
            }

          ಆದರೆ (info.si_signo == SIGCHLD && info.si_pid == pid)
            ಹೋಗು read_waitpid;

          ಆದರೆ (ret < 0 && errno == EAGAIN)
            ಹೋಗು timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", ಮಾರ್ಗ);
      ಹೋಗು restore_sig_ಮುಖವಾಡ_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &ಸ್ಥಿತಿ_2, 0);
  ಆದರೆ (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  ಇಲ್ಲದಿದ್ದರೆ
    ret = get_process_exit_ಸ್ಥಿತಿ (ಸ್ಥಿತಿ_2);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_ಮುಖವಾಡ_and_exit:
  ಆದರೆ (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      ಆದರೆ (pid > 0)
        {
          kill (pid, SIGKILL);
          ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (waitpid (pid, &ಸ್ಥಿತಿ_2, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
mark_or_close_fds_ge_than (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ n, bool close_now, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ cfd = -1;
  cleanup_dir DIR *dir = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ fd;
  ರಚನೆ dirent *ಮುಂದಿನ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  ಆದರೆ (close_now)
    GCOV_DUMP ();
#ಆದರೆ_ಅಂತ್ಯ

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  ಆದರೆ (ret == 0)
    {
      ಆದರೆ (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      ಹಿಂತಿರುಗಿ 0;
    }
  ಆದರೆ (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  ಆದರೆ (UNLIKELY (cfd < 0))
    ಹಿಂತಿರುಗಿ cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  dir = fdopendir (cfd);
  ಆದರೆ (UNLIKELY (dir == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  ಗಾಗಿ (ಮುಂದಿನ = readdir (dir); ಮುಂದಿನ; ಮುಂದಿನ = readdir (dir))
    {
      ಪೂರ್ಣಾಂಕ val;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = ಮುಂದಿನ->d_name;
      ಆದರೆ (name[0] == '.')
        ಮುಂದುವರಿ;

      val = strtoll (name, ಖಾಲಿ, 10);
      ಆದರೆ (val < n || val == fd)
        ಮುಂದುವರಿ;

      ಆದರೆ (close_now)
        {
          ret = close (val);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "close fd `%d`", val);

          ಆದರೆ (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ
get_current_timestamp (ಅಕ್ಷರ *ಹೊರಹೋಗು, size_t len)
{
  ರಚನೆ timeval tv;
  ರಚನೆ tm now;
  ಅಕ್ಷರ timestamp[64];

  gettimeofday (&tv, ಖಾಲಿ);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, ಗಾತ್ರ (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (ಶೂನ್ಯ) snprintf (ಹೊರಹೋಗು, len, "%s.%06lldZ", timestamp, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) tv.tv_usec);
  ಹೊರಹೋಗು[len - 1] = '\0';
}

ಪೂರ್ಣಾಂಕ
ಗಣ_blocking_fd (ಪೂರ್ಣಾಂಕ fd, bool blocking, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret, flags = fcntl (fd, F_GETFL, 0);
  ಆದರೆ (UNLIKELY (flags < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fcntl");
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
parse_json_file (json_object **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *jsondata, ರಚನೆ parser_context *ctx arg_ಬಳಸದ, libcrun_error_t *err)
{
  ರಚನೆ json_tokener *tok;
  ಎಣಿಕೆ json_tokener_error jerr;

  *err = ಖಾಲಿ;

  ಆದರೆ (UNLIKELY (jsondata == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  ಆದರೆ (UNLIKELY (tok == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_ಗಣ_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *ಹೊರಹೋಗು = json_tokener_parse_ex (tok, jsondata, (ಪೂರ್ಣಾಂಕ) ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (jsondata));
  jerr = json_tokener_get_error (tok);
  ಆದರೆ (*ಹೊರಹೋಗು == ಖಾಲಿ || jerr != json_tokener_success)
    {
      ಪೂರ್ಣಾಂಕ saved_pos = (ಪೂರ್ಣಾಂಕ) tok->char_ಸ್ಥಾನಾಂತರ;

      json_tokener_free (tok);
      ಆದರೆ (*ಹೊರಹೋಗು)
        {
          json_object_put (*ಹೊರಹೋಗು);
          *ಹೊರಹೋಗು = ಖಾಲಿ;
        }
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು CHECK_ACCESS_NOT_EXECUTABLE 1
#ವ್ಯಾಖ್ಯಾನಿಸು CHECK_ACCESS_NOT_REGULAR 2

/* check that the specified path exists and it is executable.
   Return
   - 0 if the file is executable
   - CHECK_ACCESS_NOT_EXECUTABLE if it exists but it is not executable
   - CHECK_ACCESS_NOT_REGULAR if it not a regular file
   - -errno for any other generic error
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
check_access (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
  ಪೂರ್ಣಾಂಕ ret;
  mode_t ಬಹುಲಕ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EACCESS
#  define ACCESS(ಮಾರ್ಗ, ಬಹುಲಕ) (eaccess (ಮಾರ್ಗ, ಬಹುಲಕ))
#ಇಲ್ಲದಿದ್ದರೆ
#  define ACCESS(ಮಾರ್ಗ, ಬಹುಲಕ) (access (ಮಾರ್ಗ, ಬಹುಲಕ))
#ಆದರೆ_ಅಂತ್ಯ

  ret = ACCESS (ಮಾರ್ಗ, X_OK);
  ಆದರೆ (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      ಆದರೆ (errno == EACCES)
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno;
          ret = ACCESS (ಮಾರ್ಗ, F_OK);
          errno = saved_errno;

          ಆದರೆ (ret == 0)
            ಹಿಂತಿರುಗಿ CHECK_ACCESS_NOT_EXECUTABLE;
        }
      ಹಿಂತಿರುಗಿ -errno;
    }

  ret = get_file_type (&ಬಹುಲಕ, false, ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ -errno;

  ಆದರೆ (! S_ISREG (ಬಹುಲಕ))
    ಹಿಂತಿರುಗಿ CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
ಕಂಡುಹಿಡಿ_executable (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *executable_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cwd, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cwd_executable_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *tmp = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_error = -ENOENT;
  ಅಕ್ಷರ *it, *ಕೊನೆ;
  ಪೂರ್ಣಾಂಕ ret;

  *ಹೊರಹೋಗು = ಖಾಲಿ;

  ಆದರೆ (executable_ಮಾರ್ಗ == ಖಾಲಿ || executable_ಮಾರ್ಗ[0] == '\0')
    ಹಿಂತಿರುಗಿ crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  ಆದರೆ (executable_ಮಾರ್ಗ[0] != '/' && strchr (executable_ಮಾರ್ಗ, '/'))
    {
      cleanup_free ಅಕ್ಷರ *cwd_allocated = ಖಾಲಿ;

      ಆದರೆ (cwd == ಖಾಲಿ)
        {
          cwd_allocated = getcwd (ಖಾಲಿ, 0);
          ಆದರೆ (cwd_allocated == ಖಾಲಿ)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_ಮಾರ್ಗ, err, "/", cwd, executable_ಮಾರ್ಗ, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      executable_ಮಾರ್ಗ = cwd_executable_ಮಾರ್ಗ;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  ಆದರೆ (executable_ಮಾರ್ಗ[0] == '/')
    {
      ret = check_access (executable_ಮಾರ್ಗ);
      ಆದರೆ (LIKELY (ret == 0))
        {
          *ಹೊರಹೋಗು = xstrdup (executable_ಮಾರ್ಗ);
          ಹಿಂತಿರುಗಿ 0;
        }
      ಕೊನೆಯ_error = ret;
      ಹೋಗು fail;
    }

  ಕೊನೆ = tmp = xstrdup (getenv ("PATH"));

  ಆಗ ((it = strsep (&ಕೊನೆ, ":")))
    {
      cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;

      ಆದರೆ (it == ಕೊನೆ)
        it = ".";

      ret = append_paths (&ಮಾರ್ಗ, err, it, executable_ಮಾರ್ಗ, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ಮುಂದುವರಿ;
        }

      ret = check_access (ಮಾರ್ಗ);
      ಆದರೆ (ret == 0)
        {
          /* Change owner.  */
          *ಹೊರಹೋಗು = ಮಾರ್ಗ;
          ಮಾರ್ಗ = ಖಾಲಿ;
          ಹಿಂತಿರುಗಿ 0;
        }

      ಆದರೆ (ret == -ENOENT)
        ಮುಂದುವರಿ;

      ಕೊನೆಯ_error = ret;
    }

fail:
  ಆಯ್ಕೆ (ಕೊನೆಯ_error)
    {
    ಸ್ಥಿತಿ CHECK_ACCESS_NOT_EXECUTABLE:
      ಹಿಂತಿರುಗಿ crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_ಮಾರ್ಗ);

    ಸ್ಥಿತಿ CHECK_ACCESS_NOT_REGULAR:
      ಹಿಂತಿರುಗಿ crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_ಮಾರ್ಗ);

    ಪೂರ್ವನಿಯೋಜಿತ:
      errno = -ಕೊನೆಯ_error;
      ಆದರೆ (errno == ENOENT)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "executable file `%s` not found%s", executable_ಮಾರ್ಗ, executable_ಮಾರ್ಗ[0] == '/' ? "" : " in $PATH");
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", executable_ಮಾರ್ಗ);
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_FGETXATTR

ಸ್ಥಿರ ssize_t
ಸುರಕ್ಷಿತ_read_xattr (ಅಕ್ಷರ **ret, ಪೂರ್ಣಾಂಕ sfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *srcname, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, size_t initial_ಗಾತ್ರ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  ssize_t current_ಗಾತ್ರ;
  ssize_t s;

  current_ಗಾತ್ರ = (ssize_t) initial_ಗಾತ್ರ;
  ಬಫರ್ = xmalloc (current_ಗಾತ್ರ + 1);

  ಆಗ (1)
    {
      s = fgetxattr (sfd, name, ಬಫರ್, current_ಗಾತ್ರ);
      ಆದರೆ (UNLIKELY (s < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      ಆದರೆ (s < current_ಗಾತ್ರ)
        ನಿಲ್ಲಿಸು;

      current_ಗಾತ್ರ *= 2;
      ಬಫರ್ = xrealloc (ಬಫರ್, current_ಗಾತ್ರ + 1);
    }

  ಆದರೆ (s <= 0)
    ಹಿಂತಿರುಗಿ s;

  ಬಫರ್[s] = '\0';

  /* Change owner.  */
  *ret = ಬಫರ್;
  ಬಫರ್ = ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ s;
}

ಸ್ಥಿರ ssize_t
copy_xattr (ಪೂರ್ಣಾಂಕ sfd, ಪೂರ್ಣಾಂಕ dfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *srcname, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *destname, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *buf = ಖಾಲಿ;
  ssize_t xattr_len;
  ಅಕ್ಷರ *it;

  xattr_len = flistxattr (sfd, ಖಾಲಿ, 0);
  ಆದರೆ (UNLIKELY (xattr_len < 0))
    {
      ಆದರೆ (errno == ENOTSUP)
        ಹಿಂತಿರುಗಿ 0;

      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  ಆದರೆ (xattr_len == 0)
    ಹಿಂತಿರುಗಿ 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  ಆದರೆ (UNLIKELY (xattr_len < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "flistxattr `%s`", srcname);

  ಗಾಗಿ (it = buf; it - buf < xattr_len; it += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (it) + 1)
    {
      cleanup_free ಅಕ್ಷರ *v = ಖಾಲಿ;
      ssize_t s;

      s = ಸುರಕ್ಷಿತ_read_xattr (&v, sfd, srcname, it, 256, err);
      ಆದರೆ (UNLIKELY (s < 0))
        ಹಿಂತಿರುಗಿ s;

      s = fsetxattr (dfd, it, v, s, 0);
      ಆದರೆ (UNLIKELY (s < 0))
        {
          ಆದರೆ (errno == EINVAL || errno == EOPNOTSUPP)
            ಮುಂದುವರಿ;

          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
copy_rec_stat_file_at (ಪೂರ್ಣಾಂಕ dfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, mode_t *ಬಹುಲಕ, off_t *ಗಾತ್ರ_2, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  ರಚನೆ stat st;
  ಪೂರ್ಣಾಂಕ ret;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_STATX
  ರಚನೆ statx stx = {
    0,
  };

  ret = statx (dfd, ಮಾರ್ಗ, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_ಬಹುಲಕ | STATX_ಗಾತ್ರ | STATX_UID | STATX_GID, &stx);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno == ENOSYS || errno == EINVAL)
        ಹೋಗು fallback;

      ಹಿಂತಿರುಗಿ ret;
    }

  *ಬಹುಲಕ = stx.stx_mode;
  *ಗಾತ್ರ_2 = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  ಹಿಂತಿರುಗಿ ret;

fallback:
#ಆದರೆ_ಅಂತ್ಯ
  ret = fstatat (dfd, ಮಾರ್ಗ, &st, AT_SYMLINK_NOFOLLOW);

  *ಬಹುಲಕ = st.st_mode;
  *ಗಾತ್ರ_2 = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
copy_recursive_fd_ಗಮ್ಯ_fd (ಪೂರ್ಣಾಂಕ srcdirfd, ಪೂರ್ಣಾಂಕ dfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *srcname, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *destname, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = ಖಾಲಿ;
  ರಚನೆ dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  ಆದರೆ (UNLIKELY (dsrcfd == ಖಾಲಿ))
    {
      ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (srcdirfd));
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open directory `%s`", destname);
    }

  ಗಾಗಿ (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close ಪೂರ್ಣಾಂಕ srcfd = -1;
      cleanup_close ಪೂರ್ಣಾಂಕ destfd = -1;
      cleanup_free ಅಕ್ಷರ *ಗುರಿ_buf = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ ret;
      mode_t ಬಹುಲಕ;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      ಆದರೆ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ಮುಂದುವರಿ;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &ಬಹುಲಕ, &st_size, &rdev, &uid, &gid);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      ಆಯ್ಕೆ (ಬಹುಲಕ & S_IFMT)
        {
        ಸ್ಥಿತಿ S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          ಆದರೆ (UNLIKELY (srcfd < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          ಆದರೆ (UNLIKELY (destfd < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_ಗಮ್ಯ_fd (srcfd, destfd, 1, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_FGETXATTR
          ret = (ಪೂರ್ಣಾಂಕ) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
#ಆದರೆ_ಅಂತ್ಯ

          ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (destfd));
          destfd = -1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, ಬಹುಲಕ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ಆದರೆ (UNLIKELY (srcfd < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ಆದರೆ (UNLIKELY (destfd < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_FGETXATTR
          ret = (ಪೂರ್ಣಾಂಕ) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
#ಆದರೆ_ಅಂತ್ಯ

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_ಗಮ್ಯ_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ S_IFLNK:
          ret = ಸುರಕ್ಷಿತ_readlinkat (dirfd (dsrcfd), de->d_name, &ಗುರಿ_buf, st_size, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          ret = symlinkat (ಗುರಿ_buf, destdirfd, de->d_name);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ S_IFBLK:
        ಸ್ಥಿತಿ S_IFCHR:
        ಸ್ಥಿತಿ S_IFIFO:
        ಸ್ಥಿತಿ S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, ಬಹುಲಕ, rdev);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          ನಿಲ್ಲಿಸು;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#ಆದರೆ_ಅಂತ್ಯ

      ret = fchmodat (destdirfd, de->d_name, ಬಹುಲಕ & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          ಆದರೆ (errno == ENOTSUP)
            ಮುಂದುವರಿ;

          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
ಕಂಡುಹಿಡಿ_annotation (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
  ಆದರೆ (container->container_def->annotations == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ ಕಂಡುಹಿಡಿ_string_map_ಮೌಲ್ಯ (container->annotations, name);
}

ಪೂರ್ಣಾಂಕ
ಸುರಕ್ಷಿತ_write (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fname, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ_2, libcrun_error_t *err)
{
  size_t written = 0;
  ಆಗ (written < ಎಣಿಕೆ_2)
    {
      ssize_t w = ಲಿಖ (fd, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) buf + written, ಎಣಿಕೆ_2 - written);
      ಆದರೆ (UNLIKELY (w < 0))
        {
          ಆದರೆ (errno == EINTR || errno == EAGAIN)
            ಮುಂದುವರಿ;
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು MAX_PARTS 32

ಪೂರ್ಣಾಂಕ
append_paths (ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err, ...)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t ಒಟ್ಟು_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_ಆರಂಭ (ap, err);
  ಗಾಗಿ (;;)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *part;
      size_t ಗಾತ್ರ_2;

      part = va_arg (ap, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *);
      ಆದರೆ (part == ಖಾಲಿ)
        ನಿಲ್ಲಿಸು;

      ಆದರೆ (n_parts == MAX_PARTS)
        {
          va_ಕೊನೆ (ap);
          ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "too many paths specified");
        }

      ಆದರೆ (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          ಆಗ (part[0] == '/' && part[1] == '/')
            part++;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* And drop any initial '/' for other components.  */
          ಆಗ (part[0] == '/')
            part++;
        }

      ಗಾತ್ರ_2 = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (part);
      ಆದರೆ (ಗಾತ್ರ_2 == 0)
        ಮುಂದುವರಿ;

      ಆಗ (ಗಾತ್ರ_2 > 1 && part[ಗಾತ್ರ_2 - 1] == '/')
        ಗಾತ್ರ_2--;

      parts[n_parts] = part;
      sizes[n_parts] = ಗಾತ್ರ_2;

      n_parts++;
    }
  va_ಕೊನೆ (ap);

  ಒಟ್ಟು_len = n_parts + 1;
  ಗಾಗಿ (i = 0; i < n_parts; i++)
    ಒಟ್ಟು_len += sizes[i];

  *ಹೊರಹೋಗು = xmalloc (ಒಟ್ಟು_len);

  copied = 0;
  ಗಾಗಿ (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*ಹೊರಹೋಗು)[copied - 1] == '/';
      ಆದರೆ (i > 0 && ! has_trailing_slash)
        {
          (*ಹೊರಹೋಗು)[copied] = '/';
          copied += 1;
        }

      ಸ್ಮರಣೆ_ನಕಲು (*ಹೊರಹೋಗು + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*ಹೊರಹೋಗು)[copied] = '\0';
  ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#ಇಲ್ಲದಿದ್ದರೆ
#  define __nonstring
#ಆದರೆ_ಅಂತ್ಯ

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
b64_ಒಳಹರಿವು (ಅಕ್ಷರ c)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಕೋಷ್ಟಕ[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  ಪೂರ್ಣಾಂಕ i;

  ಗಾಗಿ (i = 0; i < 64; i++)
    {
      ಆದರೆ (ಕೋಷ್ಟಕ[i] == c)
        ಹಿಂತಿರುಗಿ i;
    }
  ಹಿಂತಿರುಗಿ -1;
}

ಪೂರ್ಣಾಂಕ
base64_decode (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *iptr, size_t isize, ಅಕ್ಷರ *optr, size_t osize, size_t *nbytes)
{
  ಪೂರ್ಣಾಂಕ i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ದತ್ತಾಂಶ[4];

  *nbytes = 0;
  ಆಗ (consumed < isize && (*nbytes) + 3 < osize)
    {
      ಆಗ ((i < 4) && (consumed < isize))
        {
          tmp = b64_ಒಳಹರಿವು (*iptr++);
          consumed++;
          ಆದರೆ (tmp != -1)
            {
              ಆದರೆ (UNLIKELY (pad > 0))
                ಹಿಂತಿರುಗಿ -1;
              ದತ್ತಾಂಶ[i++] = tmp;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*(iptr - 1) == '=')
            {
              ದತ್ತಾಂಶ[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      ಆದರೆ (i == 4)
        {
          ಆದರೆ (UNLIKELY (pad > 2))
            ಹಿಂತಿರುಗಿ -1;
          *optr++ = (ದತ್ತಾಂಶ[0] << 2) | ((ದತ್ತಾಂಶ[1] & 0x30) >> 4);
          *optr++ = ((ದತ್ತಾಂಶ[1] & 0xf) << 4) | ((ದತ್ತಾಂಶ[2] & 0x3c) >> 2);
          *optr++ = ((ದತ್ತಾಂಶ[2] & 0x3) << 6) | ದತ್ತಾಂಶ[3];
          (*nbytes) += 3 - pad;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* I did not get all the data.  */
          consumed -= i;
          ಹಿಂತಿರುಗಿ consumed;
        }
      i = 0;
    }
  ಹಿಂತಿರುಗಿ consumed;
}

ಅಕ್ಷರ *
get_user_name (uid_t uid)
{
  ರಚನೆ passwd pd;
  ರಚನೆ passwd *ತಾತ್ಕಾಲಿಕ_ಫಲಿತಾಂಶ_ptr;
  ಅಕ್ಷರ pwdbuffer[200];
  ಆದರೆ (! getpwuid_r (uid, &pd, pwdbuffer, ಗಾತ್ರ (pwdbuffer), &ತಾತ್ಕಾಲಿಕ_ಫಲಿತಾಂಶ_ptr))
    ಹಿಂತಿರುಗಿ xstrdup (pd.pw_name);
  ಹಿಂತಿರುಗಿ xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
ಅಕ್ಷರ *
libcrun_sysctl_ಕೀಲಿ_ಗಮ್ಯ_proc_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sep;
  ಅಕ್ಷರ *ret, *it;

  sep = strpbrk (ಕೀಲಿ, "./");

  /* No separator at all, or the key is already in the path form.  */
  ಆದರೆ (sep == ಖಾಲಿ || *sep == '/')
    ಹಿಂತಿರುಗಿ xstrdup (ಕೀಲಿ);

  ret = xstrdup (ಕೀಲಿ);
  ಗಾಗಿ (it = ret; *it; it++)
    {
      ಆದರೆ (*it == '.')
        *it = '/';
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*it == '/')
        *it = '.';
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
has_suffix (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix)
{
  ಆದರೆ (! str || ! suffix)
    ಹಿಂತಿರುಗಿ 0;
  size_t lenstr = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (str);
  size_t lensuffix = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (suffix);
  ಆದರೆ (lensuffix > lenstr)
    ಹಿಂತಿರುಗಿ 0;
  ಹಿಂತಿರುಗಿ memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

ಅಕ್ಷರ *
str_join_array (ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ, size_t ಗಾತ್ರ_2, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ array[], ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = ಖಾಲಿ;
  size_t i, ಒಟ್ಟು_ಗಾತ್ರ = (ಗಾತ್ರ_2 - 1) * (jlen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (joint)) + 1;
  ಅಕ್ಷರ *ಫಲಿತಾಂಶ, *p;

  lens = xmalloc (ಗಾತ್ರ_2 * ಗಾತ್ರ (*lens));

  ಗಾಗಿ (i = 0; i < ಗಾತ್ರ_2; ++i)
    {
      ಒಟ್ಟು_ಗಾತ್ರ += (lens[i] = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (array[i]));
    }
  p = ಫಲಿತಾಂಶ = xmalloc (ಒಟ್ಟು_ಗಾತ್ರ);
  ಗಾಗಿ (i = ಸ್ಥಾನಾಂತರ; i < ಗಾತ್ರ_2; ++i)
    {
      ಸ್ಮರಣೆ_ನಕಲು (p, array[i], lens[i]);
      p += lens[i];
      ಆದರೆ (i < ಗಾತ್ರ_2 - 1)
        {
          ಸ್ಮರಣೆ_ನಕಲು (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಪೂರ್ಣಾಂಕ
libcrun_mmap (ರಚನೆ libcrun_mmap_s **ret, ಶೂನ್ಯ *addr, size_t ಉದ್ದಳತೆ,
              ಪೂರ್ಣಾಂಕ prot, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ fd, off_t ಸ್ಥಾನಾಂತರ,
              libcrun_error_t *err)
{
  ರಚನೆ libcrun_mmap_s *mmap_s = ಖಾಲಿ;

  ಶೂನ್ಯ *mapped = mmap (addr, ಉದ್ದಳತೆ, prot, flags, fd, ಸ್ಥಾನಾಂತರ);
  ಆದರೆ (mapped == MAP_FAILED)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (ಗಾತ್ರ (ರಚನೆ libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->ಉದ್ದಳತೆ = ಉದ್ದಳತೆ;

  *ret = mmap_s;

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_munmap (ರಚನೆ libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = munmap (mmap->addr, mmap->ಉದ್ದಳತೆ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "munmap");

  ಬಿಡುಗಡೆ (mmap);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ದೀರ್ಘ
read_file_as_long_or_default (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ದೀರ್ಘ def_ಮೌಲ್ಯ)
{
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  libcrun_error_t tmp_err = ಖಾಲಿ;
  ಅಕ್ಷರ *endptr;
  ದೀರ್ಘ val;
  ಪೂರ್ಣಾಂಕ ret;

  ret = read_all_file (ಮಾರ್ಗ, &content, ಖಾಲಿ, &tmp_err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      ಹಿಂತಿರುಗಿ def_ಮೌಲ್ಯ;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  ಆದರೆ (UNLIKELY (errno))
    ಹಿಂತಿರುಗಿ def_ಮೌಲ್ಯ;
  ಆದರೆ (endptr == content || (*endptr && *endptr != '\n'))
    ಹಿಂತಿರುಗಿ def_ಮೌಲ್ಯ;
  ಹಿಂತಿರುಗಿ val;
}

#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_ಅತಿಪ್ರವಾಹ_ID 65534

uid_t
get_ಅತಿಪ್ರವಾಹ_uid (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  ಆದರೆ (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_ಅತಿಪ್ರವಾಹ_ID);
      cached_uid = uid;
    }
  ಹಿಂತಿರುಗಿ uid;
}

gid_t
get_ಅತಿಪ್ರವಾಹ_gid (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  ಆದರೆ (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_ಅತಿಪ್ರವಾಹ_ID);
      cached_gid = gid;
    }
  ಹಿಂತಿರುಗಿ gid;
}

ಶೂನ್ಯ
consume_trailing_slashes (ಅಕ್ಷರ *ಮಾರ್ಗ)
{
  ಆದರೆ (! ಮಾರ್ಗ || ಮಾರ್ಗ[0] == '\0')
    ಹಿಂತಿರುಗಿ;

  ಅಕ್ಷರ *ಕೊನೆಯ = ಮಾರ್ಗ + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮಾರ್ಗ);

  ಆಗ (ಕೊನೆಯ > ಮಾರ್ಗ && *(ಕೊನೆಯ - 1) == '/')
    ಕೊನೆಯ--;

  *ಕೊನೆಯ = '\0';
}

ಅಕ್ಷರ **
read_dir_entries (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = ಖಾಲಿ;
  size_t n_entries = 0;
  size_t entries_ಗಾತ್ರ = 16;
  ಅಕ್ಷರ **entries = ಖಾಲಿ;
  ರಚನೆ dirent *de;

  dir = opendir (ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (dir == ಖಾಲಿ))
    {
      crun_make_error (err, errno, "opendir `%s`", ಮಾರ್ಗ);
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

  entries = xmalloc (entries_ಗಾತ್ರ * ಗಾತ್ರ (ಅಕ್ಷರ *));
  ಆಗ ((de = readdir (dir)))
    {
      ಆದರೆ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        ಮುಂದುವರಿ;
      ಆದರೆ (n_entries == entries_ಗಾತ್ರ)
        {
          entries_ಗಾತ್ರ *= 2;
          entries = xrealloc (entries, entries_ಗಾತ್ರ * ಗಾತ್ರ (ಅಕ್ಷರ *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * ಗಾತ್ರ (ಅಕ್ಷರ *));
  entries[n_entries] = ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ entries;
}

ಪೂರ್ಣಾಂಕ
cpuset_string_ಗಮ್ಯ_bitmask (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *ಹೊರಹೋಗು_ಗಾತ್ರ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಮುಖವಾಡ = ಖಾಲಿ;
  size_t ಮುಖವಾಡ_ಗಾತ್ರ = 0;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p = str;
  ಅಕ್ಷರ *endptr;

  ಆಗ (*p)
    {
      ದೀರ್ಘ ದೀರ್ಘ ಆರಂಭ_range, ಕೊನೆ_range;

      ಆದರೆ (*p < '0' || *p > '9')
        ಹೋಗು invalid_ಒಳಹರಿವು;

      ಆರಂಭ_range = strtoll (p, &endptr, 10);
      ಆದರೆ (ಆರಂಭ_range < 0)
        ಹೋಗು invalid_ಒಳಹರಿವು;

      p = endptr;

      ಆದರೆ (*p != '-')
        ಕೊನೆ_range = ಆರಂಭ_range;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          p++;

          ಆದರೆ (*p < '0' || *p > '9')
            ಹೋಗು invalid_ಒಳಹರಿವು;

          ಕೊನೆ_range = strtoll (p, &endptr, 10);

          ಆದರೆ (ಕೊನೆ_range < ಆರಂಭ_range)
            ಹೋಗು invalid_ಒಳಹರಿವು;

          p = endptr;
        }

      /* Just set some limit.  */
      ಆದರೆ (ಕೊನೆ_range > (1 << 20))
        ಹೋಗು invalid_ಒಳಹರಿವು;

      ಆದರೆ (ಕೊನೆ_range >= (ದೀರ್ಘ ದೀರ್ಘ) (ಮುಖವಾಡ_ಗಾತ್ರ * CHAR_BIT))
        {
          size_t new_ಮುಖವಾಡ_ಗಾತ್ರ = (ಕೊನೆ_range / CHAR_BIT) + 1;
          ಮುಖವಾಡ = xrealloc (ಮುಖವಾಡ, new_ಮುಖವಾಡ_ಗಾತ್ರ);
          memset (ಮುಖವಾಡ + ಮುಖವಾಡ_ಗಾತ್ರ, 0, new_ಮುಖವಾಡ_ಗಾತ್ರ - ಮುಖವಾಡ_ಗಾತ್ರ);
          ಮುಖವಾಡ_ಗಾತ್ರ = new_ಮುಖವಾಡ_ಗಾತ್ರ;
        }

      ಗಾಗಿ (ದೀರ್ಘ ದೀರ್ಘ i = ಆರಂಭ_range; i <= ಕೊನೆ_range; i++)
        ಮುಖವಾಡ[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      ಆದರೆ (*p == ',')
        p++;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*p)
        ಹೋಗು invalid_ಒಳಹರಿವು;
    }

  *ಹೊರಹೋಗು = ಮುಖವಾಡ;
  ಮುಖವಾಡ = ಖಾಲಿ;
  *ಹೊರಹೋಗು_ಗಾತ್ರ = ಮುಖವಾಡ_ಗಾತ್ರ;

  ಹಿಂತಿರುಗಿ 0;

invalid_ಒಳಹರಿವು:
  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot parse input `%s`", str);
}

ರಚನೆ channel_fd_pair
{
  ರಚನೆ ring_ಬಫರ್ *rb;

  ಪೂರ್ಣಾಂಕ ಒಳ_fd;
  ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_fd;

  ಪೂರ್ಣಾಂಕ infd_epoll_events;
  ಪೂರ್ಣಾಂಕ outfd_epoll_events;
};

ರಚನೆ channel_fd_pair *
channel_fd_pair_new (ಪೂರ್ಣಾಂಕ ಒಳ_fd, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_fd, size_t ಗಾತ್ರ_2)
{
  ರಚನೆ channel_fd_pair *channel = xmalloc (ಗಾತ್ರ (ರಚನೆ channel_fd_pair));
  channel->ಒಳ_fd = ಒಳ_fd;
  channel->ಹೊರಹೋಗು_fd = ಹೊರಹೋಗು_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_ಬಫರ್_make (ಗಾತ್ರ_2);
  ಹಿಂತಿರುಗಿ channel;
}

ಶೂನ್ಯ
channel_fd_pair_free (ರಚನೆ channel_fd_pair *channel)
{
  ಆದರೆ (channel == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ring_ಬಫರ್_free (channel->rb);
  ಬಿಡುಗಡೆ (channel);
}

ಪೂರ್ಣಾಂಕ
channel_fd_pair_process (ರಚನೆ channel_fd_pair *channel, ಪೂರ್ಣಾಂಕ epollfd, libcrun_error_t *err)
{
  bool is_ಒಳಹರಿವು_eagain = false, is_output_eagain = false, repeat;
  ಪೂರ್ಣಾಂಕ ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  ಗಾಗಿ (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      ಆದರೆ (ring_ಬಫರ್_get_space_ಲಭ್ಯ (channel->rb) > 0)
        {
          ret = ring_ಬಫರ್_read (channel->rb, channel->ಒಳ_fd, &is_ಒಳಹರಿವು_eagain, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          ಆದರೆ (ret > 0)
            repeat = true;
        }
      ಆದರೆ (ring_ಬಫರ್_get_ದತ್ತಾಂಶ_ಲಭ್ಯ (channel->rb) > 0)
        {
          ret = ring_ಬಫರ್_write (channel->rb, channel->ಹೊರಹೋಗು_fd, &is_output_eagain, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          ಆದರೆ (ret > 0)
            repeat = true;
        }
    }

  ಆದರೆ (epollfd >= 0)
    {
      size_t ಲಭ್ಯ = ring_ಬಫರ್_get_space_ಲಭ್ಯ (channel->rb);
      size_t ಬಳಸಿದ = ring_ಬಫರ್_get_ದತ್ತಾಂಶ_ಲಭ್ಯ (channel->rb);
      ಪೂರ್ಣಾಂಕ events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (ಲಭ್ಯ > 0 && ! is_output_eagain) ? (EPOLLIN | (is_ಒಳಹರಿವು_eagain ? EPOLLET : 0)) : 0;
      ಆದರೆ (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->ಒಳ_fd, events, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (ಬಳಸಿದ > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      ಆದರೆ (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->ಹೊರಹೋಗು_fd, events, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          channel->outfd_epoll_events = events;
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_proc (libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ fd;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_NEW_MOUNT_API
  cleanup_close ಪೂರ್ಣಾಂಕ fsfd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  ಆದರೆ (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ಖಾಲಿ, ಖಾಲಿ, 0);
      ಆದರೆ (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          ಆದರೆ (fd >= 0)
            ಹಿಂತಿರುಗಿ fd;
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

  fd = open ("/proc", O_ಮಾರ್ಗ | O_DIRECTORY | O_CLOEXEC);
  ಆದರೆ (fd < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `/proc`");
  ಹಿಂತಿರುಗಿ fd;
}

ಪೂರ್ಣಾಂಕ
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  ಆದರೆ (! container)
    ಹಿಂತಿರುಗಿ open_proc (err);

  ಆದರೆ (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  ಹಿಂತಿರುಗಿ container->proc_fd;
}

ಪೂರ್ಣಾಂಕ
libcrun_open_proc_file (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ flags, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  ಆದರೆ (proc_fd < 0)
    ಹಿಂತಿರುಗಿ proc_fd;

  fd = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (openat (proc_fd, ಮಾರ್ಗ, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  ಆದರೆ (! container)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (proc_fd));

  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "openat `/proc/%s`", ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ fd;
}

ಪೂರ್ಣಾಂಕ
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ flags, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ತುಂಬಿದ_ಮಾರ್ಗ = ಖಾಲಿ;
  xasprintf (&ತುಂಬಿದ_ಮಾರ್ಗ, "%d/%s", pid, ಮಾರ್ಗ);
  ಹಿಂತಿರುಗಿ libcrun_open_proc_file (container, ತುಂಬಿದ_ಮಾರ್ಗ, flags, err);
}
