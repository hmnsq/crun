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

#تعریف_کرو GNU_ماخذ
#شامل <config.h>
#شامل "utils.h"
#شامل "ring_buffer.h"
#شامل <stdarg.h>
#شامل <inttypes.h>
#شامل <unistd.h>
#شامل <string.h>
#شامل <libgen.h>
#شامل <fcntl.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <sys/un.h>
#شامل <sys/socket.h>
#شامل <sys/signalfd.h>
#شامل <sys/epoll.h>
#شامل <sys/syscall.h>
#شامل <sys/wait.h>
#شامل <pwd.h>
#شامل <grp.h>
#شامل <time.h>
#شامل <sys/time.h>
#شامل <sys/xattr.h>
#شامل <sys/sysmacros.h>
#شامل <sys/vfs.h>
#شامل <linux/magic.h>
#شامل <limits.h>
#شامل <sys/mman.h>
#اگر_تعریف_شدہ HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#اگر_ختم
#اگر HAVE_STDATOMIC_H
#  include <stdatomic.h>
#ورنہ
#  define atomic_long متلون طویل
#  define atomic_int متلون صحیح
#اگر_ختم
#شامل "syscalls.h"

#اگر_غیر_تعریف_شدہ CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#اگر_ختم
#اگر_غیر_تعریف_شدہ RESOLVE_اندر_جڑ
#  define RESOLVE_اندر_جڑ 0x10
#اگر_ختم
#اگر_غیر_تعریف_شدہ __NR_close_range
#  define __NR_close_range 436
#اگر_ختم
#اگر_غیر_تعریف_شدہ __NR_openat2
#  define __NR_openat2 437
#اگر_ختم
#اگر_غیر_تعریف_شدہ PROC_USER_INIT_INO
#  define PROC_USER_INIT_INO ((ino_t) 0xEFFFFFFD)
#اگر_ختم

#تعریف_کرو MAX_READLINKS 32
#تعریف_کرو MAX_OPENAT2_EAGAIN_RETRIES 128

صحیح
crun_راستہ_exists (ثابت حرف *راستہ, libcrun_error_t *err)
{
  صحیح ret = access (راستہ, F_OK);
  اگر (ret < 0)
    {
      اگر (errno == ENOENT)
        واپس 0;
      واپس crun_make_error (err, errno, "access `%s`", راستہ);
    }
  واپس 1;
}

صحیح
xasprintf (حرف **str, ثابت حرف *fmt, ...)
{
  صحیح ret;
  va_list دلائل_list;

  va_آغاز (دلائل_list, fmt);

  ret = vasprintf (str, fmt, دلائل_list);
  اگر (UNLIKELY (ret < 0))
    OOM ();

  va_انتہا (دلائل_list);
  واپس ret;
}

صحیح
write_file_at_with_flags (صحیح dirfd, صحیح flags, mode_t منوال, ثابت حرف *name, ثابت خالی *ڈیٹا, size_t len, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  صحیح ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, منوال);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "open `%s` for writing", name);

  ret = محفوظ_write (fd, name, ڈیٹا, len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس (len > زیادہ_سے_زیادہ_عدد) ? زیادہ_سے_زیادہ_عدد : (صحیح) len;
}

صحیح
detach_process ()
{
  pid_t pid;
  اگر (setsid () < 0)
    واپس -1;
  pid = fork ();
  اگر (pid < 0)
    واپس -1;
  اگر (pid != 0)
    محفوظ_exit (EXIT_SUCCESS);
  واپس 0;
}

صحیح
get_file_type_fd (صحیح fd, mode_t *منوال)
{
  ساخت stat st;
  صحیح ret;

#اگر_تعریف_شدہ HAVE_STATX
  ساخت statx stx = {
    0,
  };

  ret = statx (fd, "", AT_خالی_خانے_راستہ | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS || errno == EINVAL)
        جاؤ fallback;

      واپس ret;
    }
  *منوال = stx.stx_mode;
  واپس ret;

fallback:
#اگر_ختم
  ret = fstat (fd, &st);
  *منوال = st.st_mode;
  واپس ret;
}

صحیح
get_file_type_at (صحیح dirfd, mode_t *منوال, bool nofollow, ثابت حرف *راستہ)
{
  صحیح خالی_خانے_راستہ = راستہ == تہی ? AT_خالی_خانے_راستہ : 0;
  ساخت stat st;
  صحیح ret;

#اگر_تعریف_شدہ HAVE_STATX
  ساخت statx stx = {
    0,
  };

  ret = statx (dirfd, راستہ ?: "", خالی_خانے_راستہ | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS || errno == EINVAL)
        جاؤ fallback;

      واپس ret;
    }
  *منوال = stx.stx_mode;
  واپس ret;

fallback:
#اگر_ختم
  ret = fstatat (dirfd, راستہ ?: "", &st, خالی_خانے_راستہ | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *منوال = st.st_mode;
  واپس ret;
}

صحیح
get_file_type (mode_t *منوال, bool nofollow, ثابت حرف *راستہ)
{
  واپس get_file_type_at (AT_FDCWD, منوال, nofollow, راستہ);
}

صحیح
create_file_if_missing_at (صحیح dirfd, ثابت حرف *file, mode_t منوال, libcrun_error_t *err)
{
  cleanup_close صحیح fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, منوال);
  اگر (fd_write < 0)
    {
      صحیح saved_errno = errno;
      mode_t tmp_منوال;
      صحیح ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_منوال, true, file);
      اگر (ret == 0 && S_ISREG (tmp_منوال))
        واپس 0;

      واپس crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  واپس 0;
}

ساکن صحیح
ensure_directory_اندرونی_at (صحیح dirfd, حرف *راستہ, size_t len, صحیح منوال, libcrun_error_t *err)
{
  حرف *it = راستہ + len;
  صحیح ret = 0;
  bool والد_created = false;

  برائے (;;)
    {
      ret = mkdirat (dirfd, راستہ, منوال);
      اگر (ret == 0 || errno == EEXIST)
        واپس 0;

      صحیح saved_errno = errno;
      اگر (والد_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = تہی;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (راستہ, false, &tmp_err);
          اگر (ret > 0)
            واپس 0;
          اگر (ret < 0)
            crun_error_release (&tmp_err);

          واپس crun_make_error (err, saved_errno, "create directory `%s`", راستہ);
        }

      جب (it > راستہ && *it != '/')
        {
          it--;
          len--;
        }
      اگر (it == راستہ)
        واپس 0;

      *it = '\0';
      ret = ensure_directory_اندرونی_at (dirfd, راستہ, len - 1, منوال, err);
      *it = '/';
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      والد_created = true;
    }
  واپس ret;
}

صحیح
crun_ensure_directory_at (صحیح dirfd, ثابت حرف *راستہ, صحیح منوال, bool nofollow, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free حرف *tmp = xstrdup (راستہ);
  ret = ensure_directory_اندرونی_at (dirfd, tmp, لڑی_لمبائی (tmp), منوال, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = crun_dir_p_at (dirfd, راستہ, nofollow, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (ret == 0)
    واپس crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", راستہ);

  واپس 0;
}

ساکن صحیح
check_fd_is_راستہ (ثابت حرف *راستہ, صحیح fd, ثابت حرف *fdname, libcrun_error_t *err)
{
  proc_fd_راستہ_t fdpath;
  size_t راستہ_len = لڑی_لمبائی (راستہ);
  حرف link[PATH_MAX];
  صحیح ret;

  get_proc_self_fd_راستہ (fdpath, fd);
  ret = عارضی_FAILURE_RETRY (readlink (fdpath, link, حجم (link)));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "readlink `%s`", fdname);

  اگر (((size_t) ret) != راستہ_len || memcmp (link, راستہ, راستہ_len))
    واپس crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, راستہ);

  واپس 0;
}

ساکن صحیح
check_fd_under_راستہ (ثابت حرف *rootfs, size_t rootfslen, صحیح fd, ثابت حرف *fdname, libcrun_error_t *err)
{
  proc_fd_راستہ_t fdpath;
  حرف link[PATH_MAX];
  صحیح ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  اگر (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    واپس 0;

  get_proc_self_fd_راستہ (fdpath, fd);
  ret = عارضی_FAILURE_RETRY (readlink (fdpath, link, حجم (link)));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "readlink `%s`", fdname);

  اگر (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    واپس crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  واپس 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
ساکن خالی
close_and_replace (صحیح *oldfd, صحیح newfd)
{
  اگر (*oldfd >= 0)
    عارضی_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
حرف *chroot_realpath (ثابت حرف *chroot, ثابت حرف *راستہ, حرف resolved_راستہ[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
ساکن صحیح
محفوظ_openat_fallback (صحیح dirfd, ثابت حرف *rootfs, ثابت حرف *راستہ, صحیح flags,
                      صحیح منوال, libcrun_error_t *err)
{
  cleanup_free حرف *والد_راستہ = تہی;
  ثابت حرف *آخری_component = تہی;
  ثابت حرف *orig_راستہ = راستہ;
  ثابت حرف *راستہ_اندر_chroot;
  cleanup_close صحیح fd = -1;
  حرف resolved[PATH_MAX];
  حرف بفر[PATH_MAX];
  size_t rootfs_len = محفوظ_strlen (rootfs);
  صحیح ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  اگر (flags & O_NOFOLLOW)
    {
      حرف *sep;

      والد_راستہ = xstrdup (راستہ);
      sep = strrchr (والد_راستہ, '/');
      اگر (sep == تہی)
        {
          /* No parent directory, the entire path is the last component.  */
          آخری_component = راستہ;
          والد_راستہ[0] = '\0';
        }
      ورنہ اگر (sep[1] != '\0')
        {
          *sep = '\0';
          آخری_component = راستہ + (sep - والد_راستہ) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      اگر (آخری_component)
        راستہ = والد_راستہ;
    }

  راستہ_اندر_chroot = chroot_realpath (rootfs, راستہ, بفر);
  اگر (راستہ_اندر_chroot == تہی)
    واپس crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_راستہ);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  اگر (rootfs_len > 0 && لڑی_موازنہ (راستہ_اندر_chroot, rootfs, rootfs_len) == 0)
    راستہ_اندر_chroot += rootfs_len;
  راستہ_اندر_chroot = consume_slashes (راستہ_اندر_chroot);

  اگر (آخری_component)
    {
      ret = snprintf (resolved, حجم (resolved), "%s%s%s", راستہ_اندر_chroot,
                      راستہ_اندر_chroot[0] == '\0' ? "" : "/", آخری_component);
      اگر (UNLIKELY (ret >= (صحیح) حجم (resolved)))
        واپس crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_راستہ);

      راستہ_اندر_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  اگر (راستہ_اندر_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      واپس ret;
    }

  ret = openat (dirfd, راستہ_اندر_chroot, flags, منوال);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "open `%s`", orig_راستہ);

  fd = ret;

  ret = check_fd_under_راستہ (rootfs, rootfs_len, fd, orig_راستہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = fd;
  fd = -1;
  واپس ret;
}

صحیح
محفوظ_openat (صحیح dirfd, ثابت حرف *rootfs, ثابت حرف *راستہ, صحیح flags, صحیح منوال,
             libcrun_error_t *err)
{
  ساکن bool openat2_supported = true;
  صحیح retries = MAX_OPENAT2_EAGAIN_RETRIES;
  صحیح ret;

  اگر (is_خالی_خانے_string (راستہ))
    {
      cleanup_close صحیح fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      اگر (is_خالی_خانے_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, منوال);
      اگر (UNLIKELY (fd < 0))
        واپس crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      اگر (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_راستہ (rootfs, fd, راستہ, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      ret = fd;
      fd = -1;
      واپس ret;
    }

  اگر (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, راستہ, flags, منوال, RESOLVE_اندر_جڑ);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (errno == EINTR)
            جاؤ repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          اگر (errno == EAGAIN && retries-- > 0)
            جاؤ repeat;
          اگر (errno == ENOSYS)
            openat2_supported = false;
          اگر (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            واپس محفوظ_openat_fallback (dirfd, rootfs, راستہ, flags, منوال, err);

          واپس crun_make_error (err, errno, "openat2 `%s`", راستہ);
        }

      واپس ret;
    }

  واپس محفوظ_openat_fallback (dirfd, rootfs, راستہ, flags, منوال, err);
}

ssize_t
محفوظ_readlinkat (صحیح dfd, ثابت حرف *name, حرف **بفر, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_سائز = hint > 0 ? hint + 1 : 512;
  cleanup_free حرف *tmp_buf = تہی;
  ssize_t سائز;

  کرو
    {
      اگر (tmp_buf != تہی)
        buf_سائز += 256;

      tmp_buf = xrealloc (tmp_buf, buf_سائز);

      سائز = readlinkat (dfd, name, tmp_buf, buf_سائز);
      اگر (UNLIKELY (سائز < 0))
        واپس crun_make_error (err, errno, "readlink `%s`", name);
  } جب (سائز == buf_سائز);

  /* Always NUL terminate the buffer.  */
  tmp_buf[سائز] = '\0';

  /* Move ownership to BUFFER.  */
  *بفر = tmp_buf;
  tmp_buf = تہی;

  واپس سائز;
}

ساکن صحیح crun_محفوظ_ensure_at (bool do_open, bool dir, bool follow_symlinks, صحیح dirfd, ثابت حرف *dirpath,
                                ثابت حرف *راستہ, صحیح منوال, صحیح max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
ساکن صحیح
crun_محفوظ_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, صحیح dirfd, ثابت حرف *dirpath, صحیح cwd,
                                 ثابت حرف *npath, ثابت حرف *cur, ثابت حرف *rest,
                                 صحیح منوال, صحیح max_readlinks, libcrun_error_t *err)
{
  cleanup_free حرف *ہدف = تہی;
  cleanup_free حرف *new_راستہ = تہی;
  ssize_t ret;

  ret = محفوظ_readlinkat (cwd, cur, &ہدف, 0, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_راستہ, "%.*s%s%s%s", ہدف[0] == '/' ? 0 : (صحیح) (cur - npath), npath, ہدف,
             rest ? "/" : "", rest ? rest : "");

  واپس crun_محفوظ_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_راستہ, منوال, max_readlinks - 1, err);
}

ساکن صحیح
crun_محفوظ_ensure_at (bool do_open, bool dir, bool follow_symlinks, صحیح dirfd, ثابت حرف *dirpath,
                     ثابت حرف *راستہ, صحیح منوال, صحیح max_readlinks, libcrun_error_t *err)
{
  cleanup_close صحیح wd_cleanup = -1;
  cleanup_free حرف *npath = تہی;
  bool آخری_component = false;
  size_t گہرائی = 0;
  حرف *cur;
  حرف *it;
  صحیح cwd;
  صحیح ret;

  اگر (max_readlinks <= 0)
    واپس crun_make_error (err, ELOOP, "resolve path `%s`", راستہ);

  راستہ = consume_slashes (راستہ);

  /* Empty path, nothing to do.  */
  اگر (*راستہ == '\0')
    {
      اگر (do_open)
        واپس open (dirpath, O_CLOEXEC | O_راستہ, 0);
      واپس 0;
    }

  npath = xstrdup (راستہ);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  جب (cur)
    {
      اگر (it)
        *it = '\0';
      ورنہ
        آخری_component = true;

      اگر (cur[0] == '\0')
        توڑو;

      اگر (strcmp (cur, ".") == 0)
        جاؤ اگلا;
      ورنہ اگر (strcmp (cur, ".."))
        گہرائی++;
      ورنہ
        {
          اگر (گہرائی)
            گہرائی--;
          ورنہ
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              جاؤ اگلا;
            }
        }

      اگر (آخری_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          اگر (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              اگر (errno == ELOOP)
                واپس crun_محفوظ_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, تہی,
                                                        منوال, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_راستہ | O_NOFOLLOW, 0);
              اگر (ret < 0)
                واپس crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          اگر (do_open)
            واپس ret;

          close_and_replace (&wd_cleanup, ret);
          واپس 0;
        }

      ret = mkdirat (cwd, cur, منوال);
      اگر (ret < 0)
        {
          mode_t st_mode;

          اگر (errno != EEXIST)
            واپس crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          اگر (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            واپس crun_محفوظ_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : تہی, منوال, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = محفوظ_openat (dirfd, dirpath, npath, O_راستہ | O_CLOEXEC, 0, err);
      اگر (UNLIKELY (cwd < 0))
        واپس crun_error_wrap (err, "creating `/%s`", راستہ);

      اگر (! آخری_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, تہی);
          اگر (UNLIKELY (ret < 0))
            {
              صحیح saved_errno = errno;

              close (cwd);
              واپس crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          اگر ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              واپس crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", راستہ, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    اگلا:
      اگر (it == تہی)
        توڑو;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  اگر (do_open)
    {
      اگر (cwd == dirfd)
        {
          ret = dup (dirfd);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "dup `%s`", dirpath);
          واپس ret;
        }

      wd_cleanup = -1;
      واپس cwd;
    }

  واپس 0;
}

صحیح
crun_محفوظ_create_and_open_ref_at (bool dir, صحیح dirfd, ثابت حرف *dirpath, ثابت حرف *راستہ, صحیح منوال, libcrun_error_t *err)
{
  صحیح ret;

  /* If the file/dir already exists, just open it.  */
  ret = محفوظ_openat (dirfd, dirpath, راستہ, O_راستہ | O_CLOEXEC, 0, err);
  اگر (LIKELY (ret >= 0))
    واپس ret;

  crun_error_release (err);
  واپس crun_محفوظ_ensure_at (true, dir, true, dirfd, dirpath, راستہ, منوال, MAX_READLINKS, err);
}

صحیح
crun_محفوظ_ensure_directory_at (صحیح dirfd, ثابت حرف *dirpath, ثابت حرف *راستہ, صحیح منوال,
                               libcrun_error_t *err)
{
  واپس crun_محفوظ_ensure_at (false, true, false, dirfd, dirpath, راستہ, منوال, MAX_READLINKS, err);
}

صحیح
crun_محفوظ_ensure_file_at (صحیح dirfd, ثابت حرف *dirpath, ثابت حرف *راستہ, صحیح منوال,
                          libcrun_error_t *err)
{
  واپس crun_محفوظ_ensure_at (false, false, false, dirfd, dirpath, راستہ, منوال, MAX_READLINKS, err);
}

صحیح
crun_ensure_directory (ثابت حرف *راستہ, صحیح منوال, bool nofollow, libcrun_error_t *err)
{
  واپس crun_ensure_directory_at (AT_FDCWD, راستہ, منوال, nofollow, err);
}

ساکن صحیح
get_file_سائز (صحیح fd, off_t *سائز)
{
  ساخت stat st;
  صحیح ret;
#اگر_تعریف_شدہ HAVE_STATX
  ساخت statx stx = {
    0,
  };

  ret = statx (fd, "", AT_خالی_خانے_راستہ | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_سائز, &stx);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS || errno == EINVAL)
        جاؤ fallback;
      واپس ret;
    }
  *سائز = stx.stx_size;

  واپس ret;

fallback:
#اگر_ختم
  ret = fstat (fd, &st);
  *سائز = st.st_size;
  واپس ret;
}

صحیح
crun_dir_p_at (صحیح dirfd, ثابت حرف *راستہ, bool nofollow, libcrun_error_t *err)
{
  mode_t منوال;
  صحیح ret;

  ret = get_file_type_at (dirfd, &منوال, nofollow, راستہ);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "stat `%s`", راستہ);

  واپس S_ISDIR (منوال) ? 1 : 0;
}

صحیح
crun_dir_p (ثابت حرف *راستہ, bool nofollow, libcrun_error_t *err)
{
  واپس crun_dir_p_at (AT_FDCWD, راستہ, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
ساکن صحیح
check_running_اندر_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free حرف *بفر = تہی;
  size_t len;
  صحیح ret;

  ret = read_all_file ("/proc/self/uid_map", &بفر, &len, err);
  اگر (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          واپس 0;
        }
      واپس ret;
    }

  واپس لڑی_تلاش (بفر, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
ساکن bool
running_اندر_openvz ()
{
  اگر (access ("/proc/vz", F_OK) < 0)
    واپس false;

  واپس access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

صحیح
check_running_اندر_user_namespace (libcrun_error_t *err)
{
  ساکن atomic_int تسلسل_اندر_userns = -1;
  ساخت stat st;
  صحیح ret;

  ret = تسلسل_اندر_userns;
  اگر (ret >= 0)
    واپس ret;

  ret = stat ("/proc/self/ns/user", &st);
  اگر (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      اگر (st.st_ino == PROC_USER_INIT_INO)
        {
          تسلسل_اندر_userns = 0;
          واپس تسلسل_اندر_userns;
        }

      اگر (! running_اندر_openvz ())
        {
          تسلسل_اندر_userns = 1;
          واپس تسلسل_اندر_userns;
        }
    }
  ورنہ اگر (UNLIKELY (errno != ENOENT))
    واپس crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_اندر_user_namespace_uid_map (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  تسلسل_اندر_userns = ret;
  واپس تسلسل_اندر_userns;
}

ساکن size_t
get_صفحہ_سائز ()
{
  ساکن atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  اگر (pagesize == 0)
    {
      pagesize = (طویل) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  واپس (size_t) pagesize;
}

ساکن صحیح selinux_enabled = -1;
ساکن صحیح apparmor_enabled = -1;

صحیح
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free حرف *برآمد = تہی;
  cleanup_close صحیح fd = -1;
  size_t len;
  صحیح ret;

  اگر (selinux_enabled >= 0)
    واپس selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  اگر (UNLIKELY (fd < 0))
    واپس fd;

  ret = read_all_fd_with_سائز_hint (fd, "mounts", &برآمد, &len, get_صفحہ_سائز (), err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  selinux_enabled = لڑی_تلاش (برآمد, "selinux") ? 1 : 0;

  واپس selinux_enabled;
}

صحیح
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  صحیح سائز;
  حرف buf[1];

  اگر (apparmor_enabled >= 0)
    واپس apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  اگر (fd == -1)
    {
      اگر (errno == ENOENT)
        {
          apparmor_enabled = 0;
          واپس 0;
        }

      واپس crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  سائز = عارضی_FAILURE_RETRY (اقرأ (fd, buf, حجم (buf)));

  apparmor_enabled = سائز > 0 && buf[0] == 'Y' ? 1 : 0;

  واپس apparmor_enabled;
}

ساکن صحیح
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  اگر (selinux_enabled < 0)
    واپس crun_make_error (err, 0, "SELinux is not initialized correctly");
  واپس selinux_enabled;
}

صحیح
جوڑو_selinux_mount_label (حرف **retlabel, ثابت حرف *ڈیٹا, ثابت حرف *label, ثابت حرف *context_type, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_is_selinux_enabled (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (label && ret)
    {
      اگر (ڈیٹا && *ڈیٹا)
        xasprintf (retlabel, "%s,%s=\"%s\"", ڈیٹا, context_type, label);
      ورنہ
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      واپس 0;
    }
  *retlabel = xstrdup (ڈیٹا);
  واپس 0;
}

ساکن ثابت حرف *
lsm_attr_راستہ (libcrun_container_t *container, ثابت حرف *lsm, ثابت حرف *fname, libcrun_error_t *err)
{
  cleanup_close صحیح attr_dirfd = -1;
  cleanup_close صحیح lsm_dirfd = -1;
  حرف *attr_راستہ = تہی;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_راستہ, err);
  اگر (UNLIKELY (attr_dirfd < 0))
    واپس تہی;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  اگر (lsm != تہی)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_راستہ | O_CLOEXEC);

      اگر (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          واپس تہی;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_راستہ, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  واپس attr_راستہ;
}

ساکن صحیح
check_proc_super_magic (صحیح fd, ثابت حرف *راستہ, libcrun_error_t *err)
{
  ساخت statfs sfs;

  صحیح ret = fstatfs (fd, &sfs);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "statfs `%s`", راستہ);

  اگر (sfs.f_type != PROC_SUPER_MAGIC)
    واپس crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", راستہ);

  واپس 0;
}

ساکن صحیح
سیٹ_security_attr (libcrun_container_t *container, ثابت حرف *lsm, ثابت حرف *fname, ثابت حرف *ڈیٹا, libcrun_error_t *err)
{
  صحیح ret;

  cleanup_free ثابت حرف *attr_راستہ = lsm_attr_راستہ (container, lsm, fname, err);
  cleanup_close صحیح fd = -1;

  اگر (UNLIKELY (attr_راستہ == تہی))
    واپس -1;

  fd = libcrun_open_proc_file (container, attr_راستہ, O_WRONLY | O_CLOEXEC, err);
  اگر (UNLIKELY (fd < 0))
    واپس fd;

  ret = check_proc_super_magic (fd, attr_راستہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  // Write out data
  ret = عارضی_FAILURE_RETRY (اكتب (fd, ڈیٹا, لڑی_لمبائی (ڈیٹا)));
  اگر (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      اگر (errno == ENOENT && lsm != تہی && strcmp (lsm, "apparmor") == 0)
        {
          ثابت حرف *profile = strchr (ڈیٹا, ' ');

          واپس crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : ڈیٹا);
        }
      واپس crun_make_error (err, errno, "write to file `%s`", attr_راستہ);
    }

  واپس 0;
}

صحیح
سیٹ_selinux_label (libcrun_container_t *container, ثابت حرف *label, bool now, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_is_selinux_enabled (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (ret)
    واپس سیٹ_security_attr (container, تہی, now ? "current" : "exec", label, err);
  واپس 0;
}

ساکن صحیح
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  اگر (apparmor_enabled < 0)
    واپس crun_make_error (err, 0, "AppArmor is not initialized correctly");
  واپس apparmor_enabled;
}

ساکن صحیح
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#تعریف_کرو UNCONFINED "unconfined"
#تعریف_کرو UNCONFINED_LEN (ssize_t) (حجم (UNCONFINED) - 1)
  cleanup_free ثابت حرف *attr_راستہ = lsm_attr_راستہ (container, "apparmor", "current", err);
  cleanup_close صحیح fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  حرف buf[UNCONFINED_LEN + 1];

  اگر (UNLIKELY (attr_راستہ == تہی))
    واپس -1;

  fd = libcrun_open_proc_file (container, attr_راستہ, O_RDONLY | O_CLOEXEC, err);
  اگر (UNLIKELY (fd < 0))
    واپس fd;

  اگر (UNLIKELY (check_proc_super_magic (fd, attr_راستہ, err)))
    واپس -1;

  ssize_t بائٹیں_read = عارضی_FAILURE_RETRY (اقرأ (fd, buf, حجم (buf)));
  اگر (UNLIKELY (بائٹیں_read < 0))
    واپس crun_make_error (err, errno, "read from `%s`", attr_راستہ);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  اگر (بائٹیں_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    واپس 1;
  اگر (بائٹیں_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    واپس 0;
  واپس 1;
#تعریف_منسوخ UNCONFINED
#تعریف_منسوخ UNCONFINED_LEN
}

صحیح
سیٹ_apparmor_profile (libcrun_container_t *container, ثابت حرف *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_is_apparmor_enabled (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (ret)
    {
      cleanup_free حرف *buf = تہی;
      ret = is_current_process_confined (container, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      واپس سیٹ_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  واپس 0;
}

ساکن صحیح
read_all_fd_اندرونی (صحیح fd, ثابت حرف *description, حرف **برآمد, size_t *len, size_t سائز_hint, size_t max_سائز, libcrun_error_t *err)
{
  cleanup_free حرف *buf = تہی;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t سائز = 0;
  صحیح ret;

  اگر (سائز_hint)
    allocated = سائز_hint;
  ورنہ
    {
      ret = get_file_سائز (fd, &سائز);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "stat `%s`", description);

      اگر (max_سائز && (size_t) سائز > max_سائز)
        واپس crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_سائز);

      allocated = سائز == 0 ? 1023 : سائز;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  جب ((سائز && nread < (size_t) سائز) || سائز == 0)
    {
      ret = عارضی_FAILURE_RETRY (اقرأ (fd, buf + nread, allocated - nread));
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "read from file `%s`", description);

      اگر (ret == 0)
        توڑو;

      nread += ret;

      اگر (max_سائز && nread > max_سائز)
        واپس crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_سائز);

      اگر (nread == allocated)
        {
          اگر (سائز)
            توڑو;

          اگر (pagesize == 0)
            pagesize = get_صفحہ_سائز ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  اگر (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      حرف *tmp = realloc (buf, nread + 1);
      اگر (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *برآمد = buf;
  buf = تہی;
  اگر (len)
    *len = nread;
  واپس 0;
}

صحیح
read_all_fd_with_سائز_hint (صحیح fd, ثابت حرف *description, حرف **برآمد, size_t *len, size_t سائز_hint, libcrun_error_t *err)
{
  واپس read_all_fd_اندرونی (fd, description, برآمد, len, سائز_hint, 0, err);
}

صحیح
read_all_fd_with_max_سائز (صحیح fd, ثابت حرف *description, حرف **برآمد, size_t *len, size_t max_سائز, libcrun_error_t *err)
{
  واپس read_all_fd_اندرونی (fd, description, برآمد, len, 0, max_سائز, err);
}

صحیح
read_all_file_at (صحیح dirfd, ثابت حرف *راستہ, حرف **برآمد, size_t *len, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;

  fd = عارضی_FAILURE_RETRY (openat (dirfd, راستہ, O_RDONLY | O_CLOEXEC));
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "open `%s`", راستہ);

  واپس read_all_fd (fd, راستہ, برآمد, len, err);
}

صحیح
read_all_file (ثابت حرف *راستہ, حرف **برآمد, size_t *len, libcrun_error_t *err)
{
  اگر (strcmp (راستہ, "-") == 0)
    راستہ = "/dev/stdin";

  واپس read_all_file_at (AT_FDCWD, راستہ, برآمد, len, err);
}

صحیح
get_realpath_مقصد_file (صحیح dirfd, ثابت حرف *راستہ_name, حرف **absolute_راستہ, libcrun_error_t *err)
{
  cleanup_close صحیح targetfd = -1;

  targetfd = عارضی_FAILURE_RETRY (openat (dirfd, راستہ_name, O_RDONLY | O_CLOEXEC));
  اگر (UNLIKELY (targetfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", راستہ_name);
  ورنہ
    {
      ssize_t len;
      proc_fd_راستہ_t ہدف_fd_راستہ;

      get_proc_self_fd_راستہ (ہدف_fd_راستہ, targetfd);
      len = محفوظ_readlinkat (AT_FDCWD, ہدف_fd_راستہ, absolute_راستہ, 0, err);
      اگر (UNLIKELY (len < 0))
        واپس crun_error_wrap (err, "error unable to provide absolute path to file `%s`", راستہ_name);
    }

  واپس 0;
}

صحیح
open_unix_domain_client_socket (ثابت حرف *راستہ, صحیح dgram, libcrun_error_t *err)
{
  ساخت sockaddr_un addr = {};
  صحیح ret;
  proc_fd_راستہ_t name_buf;
  cleanup_close صحیح destfd = -1;
  cleanup_close صحیح fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", راستہ);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_دھارا, 0);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "create UNIX socket");

  اگر (لڑی_لمبائی (راستہ) >= حجم (addr.sun_راستہ))
    {
      destfd = open (راستہ, O_راستہ | O_CLOEXEC);
      اگر (UNLIKELY (destfd < 0))
        واپس crun_make_error (err, errno, "open `%s`", راستہ);

      get_proc_self_fd_راستہ (name_buf, destfd);

      راستہ = name_buf;
    }

  size_t راستہ_len = لڑی_لمبائی (راستہ);
  اگر (راستہ_len >= حجم (addr.sun_راستہ))
    واپس crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", راستہ);

  حافظہ_نقل (addr.sun_راستہ, راستہ, راستہ_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (ساخت sockaddr *) &addr, حجم (addr));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "connect socket to `%s`", راستہ);

  ret = fd;
  fd = -1;

  واپس ret;
}

صحیح
open_unix_domain_socket (ثابت حرف *راستہ, صحیح dgram, libcrun_error_t *err)
{
  ساخت sockaddr_un addr = {};
  proc_fd_راستہ_t name_buf;
  صحیح ret;
  cleanup_close صحیح dirfd = -1;
  cleanup_close صحیح fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_دھارا, 0);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "create UNIX socket");

  اگر (لڑی_لمبائی (راستہ) >= حجم (addr.sun_راستہ))
    {
      cleanup_free حرف *dpath = xstrdup (راستہ);
      cleanup_free حرف *bpath = xstrdup (راستہ);
      ثابت حرف *والد_dir = dirname (dpath);
      ثابت حرف *بنیاد = basename (bpath);
      صحیح n;

      dirfd = open (والد_dir, O_راستہ | O_DIRECTORY | O_CLOEXEC);
      اگر (UNLIKELY (dirfd < 0))
        واپس crun_make_error (err, errno, "open directory `%s`", والد_dir);

      get_proc_self_fd_راستہ (name_buf, dirfd);

      n = snprintf (addr.sun_راستہ, حجم (addr.sun_راستہ), "%s/%s", name_buf, بنیاد);
      اگر (n < 0 || (size_t) n >= حجم (addr.sun_راستہ))
        واپس crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", راستہ);
    }
  ورنہ
    {
      size_t راستہ_len = لڑی_لمبائی (راستہ);
      حافظہ_نقل (addr.sun_راستہ, راستہ, راستہ_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (ساخت sockaddr *) &addr, حجم (addr));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "bind socket to `%s`", راستہ);

  اگر (! dgram)
    {
      ret = listen (fd, 1);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  واپس ret;
}

صحیح
send_fd_مقصد_socket (صحیح server, صحیح fd, libcrun_error_t *err)
{
  واپس send_fd_مقصد_socket_with_payload (server, fd, تہی, 0, err);
}

صحیح
send_fd_مقصد_socket_with_payload (صحیح server, صحیح fd, ثابت حرف *payload, size_t payload_len, libcrun_error_t *err)
{
  صحیح ret;
  ساخت cmsghdr *cmsg = تہی;
  ساخت iovec iov[2];
  ساخت msghdr پیغام = {};
  حرف ctrl_buf[CMSG_SPACE (1 + حجم (صحیح))] = {};
  حرف ڈیٹا[1];

  ڈیٹا[0] = ' ';
  iov[0].iov_base = ڈیٹا;
  iov[0].iov_len = حجم (ڈیٹا);

  اگر (payload_len > 0)
    {
      iov[0].iov_base = (خالی *) payload;
      iov[0].iov_len = payload_len;
    }

  پیغام.پیغام_name = تہی;
  پیغام.پیغام_namelen = 0;
  پیغام.پیغام_iov = iov;
  پیغام.پیغام_iovlen = 1;
  پیغام.پیغام_controllen = CMSG_SPACE (حجم (صحیح));
  پیغام.پیغام_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&پیغام);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (حجم (صحیح));

  *((صحیح *) CMSG_ڈیٹا (cmsg)) = fd;

  ret = عارضی_FAILURE_RETRY (sendmsg (server, &پیغام, 0));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "sendmsg");
  واپس 0;
}

صحیح
receive_fd_from_socket_with_payload (صحیح from, حرف *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  صحیح ret;
  ساخت iovec iov[1];
  ساخت msghdr پیغام = {};
  حرف ctrl_buf[CMSG_SPACE (حجم (صحیح))] = {};
  حرف ڈیٹا[1];
  ساخت cmsghdr *cmsg;

  ڈیٹا[0] = ' ';
  iov[0].iov_base = ڈیٹا;
  iov[0].iov_len = حجم (ڈیٹا);

  اگر (payload_len > 0)
    {
      iov[0].iov_base = (خالی *) payload;
      iov[0].iov_len = payload_len;
    }

  پیغام.پیغام_name = تہی;
  پیغام.پیغام_namelen = 0;
  پیغام.پیغام_iov = iov;
  پیغام.پیغام_iovlen = 1;
  پیغام.پیغام_controllen = CMSG_SPACE (حجم (صحیح));
  پیغام.پیغام_control = ctrl_buf;

  ret = عارضی_FAILURE_RETRY (recvmsg (from, &پیغام, 0));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "recvmsg");
  اگر (UNLIKELY (ret == 0))
    واپس crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&پیغام);
  اگر (cmsg == تہی)
    واپس crun_make_error (err, 0, "no msg received");
  حافظہ_نقل (&fd, CMSG_ڈیٹا (cmsg), حجم (fd));

  ret = fd;
  fd = -1;
  واپس ret;
}

صحیح
receive_fd_from_socket (صحیح from, libcrun_error_t *err)
{
  واپس receive_fd_from_socket_with_payload (from, تہی, 0, err);
}

صحیح
create_socket_pair (صحیح *pair, libcrun_error_t *err)
{
  صحیح ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "socketpair");
  واپس 0;
}

صحیح
create_signalfd (sigset_t *نقاب, libcrun_error_t *err)
{
  صحیح ret = signalfd (-1, نقاب, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "signalfd");
  واپس ret;
}

ساکن صحیح
epoll_helper_toggle (صحیح epollfd, صحیح fd, صحیح events, libcrun_error_t *err)
{
  ساخت epoll_event ev = {};
  bool جوڑو = events != 0;
  صحیح ret;

  ev.events = events;
  ev.ڈیٹا.fd = fd;
  ret = epoll_ctl (epollfd, جوڑو ? EPOLL_CTL_جوڑو : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  اگر (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  اگر (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      اگر (errno == ENOENT || errno == EPERM)
        واپس 0;
      واپس crun_make_error (err, errno, "epoll_ctl `%s` `%d`", جوڑو ? "add" : "del", fd);
    }
  واپس 0;
}

صحیح
epoll_helper (صحیح *اندر_fds, صحیح *اندر_levelfds, صحیح *برآمد_fds, صحیح *برآمد_levelfds, libcrun_error_t *err)
{
  ساخت epoll_event ev;
  cleanup_close صحیح epollfd = -1;
  صحیح ret;

  صحیح *it;
  epollfd = epoll_create1 (0);
  اگر (UNLIKELY (epollfd < 0))
    واپس crun_make_error (err, errno, "epoll_create1");

#تعریف_کرو جوڑو_FDS(FDS, EVENTS)                                            \
  برائے (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.ڈیٹا.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_جوڑو, *it, &ev);               \
      اگر (UNLIKELY (ret < 0))                                           \
        واپس crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  اگر (اندر_fds)
    جوڑو_FDS (اندر_fds, EPOLLIN);
  اگر (اندر_levelfds)
    جوڑو_FDS (اندر_levelfds, EPOLLIN | EPOLLET);
  اگر (برآمد_fds)
    جوڑو_FDS (برآمد_fds, EPOLLOUT);
  اگر (برآمد_levelfds)
    جوڑو_FDS (برآمد_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  واپس ret;
}

صحیح
copy_from_fd_مقصد_fd (صحیح src, صحیح dst, صحیح consume, libcrun_error_t *err)
{
  صحیح ret;
  ssize_t nread;
  size_t pagesize = get_صفحہ_سائز ();
#اگر_تعریف_شدہ HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#اگر_ختم
  کرو
    {
      cleanup_free حرف *بفر = تہی;
      ssize_t بقایا;

#اگر_تعریف_شدہ HAVE_COPY_FILE_RANGE
      اگر (can_copy_file_range)
        {
          nread = copy_file_range (src, تہی, dst, تہی, pagesize, 0);
          اگر (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              جاؤ fallback;
            }
          اگر (consume && nread < 0 && errno == EAGAIN)
            واپس 0;
          اگر (nread < 0 && errno == EIO)
            واپس 0;
          اگر (UNLIKELY (nread < 0))
            واپس crun_make_error (err, errno, "copy_file_range");
          جاری;
        }
    fallback:
#اگر_ختم

      بفر = xmalloc (pagesize);
      nread = عارضی_FAILURE_RETRY (اقرأ (src, بفر, pagesize));
      اگر (consume && nread < 0 && errno == EAGAIN)
        واپس 0;
      اگر (nread < 0 && errno == EIO)
        واپس 0;
      اگر (UNLIKELY (nread < 0))
        واپس crun_make_error (err, errno, "read");

      بقایا = nread;
      جب (بقایا)
        {
          ret = عارضی_FAILURE_RETRY (اكتب (dst, بفر + nread - بقایا, بقایا));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "write");
          بقایا -= ret;
        }
  } جب (consume && nread);

  واپس 0;
}

صحیح
تسلسل_process (حرف **دلائل, libcrun_error_t *err)
{
  pid_t pid = fork ();
  اگر (UNLIKELY (pid < 0))
    واپس crun_make_error (err, errno, "fork");
  اگر (pid)
    {
      صحیح r, حالت_2;
      r = عارضی_FAILURE_RETRY (waitpid (pid, &حالت_2, 0));
      اگر (r < 0)
        واپس crun_make_error (err, errno, "waitpid");
      اگر (WIFEXITED (حالت_2) || WIFSIGNALED (حالت_2))
        واپس WEXITSTATUS (حالت_2);
    }

  execvp (دلائل[0], دلائل);
  محفوظ_exit (EXIT_FAILURE);
}

#اگر_غیر_تعریف_شدہ HAVE_FGETPWENT_R
ساکن غیرعلامتی
atou (حرف **s)
{
  غیرعلامتی x;
  برائے (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  واپس x;
}

صحیح
fgetpwent_r (FILE *f, ساخت passwd *pw, حرف *سطر, size_t سائز, ساخت passwd **res)
{
  حرف *s;
  صحیح rv = 0;
  برائے (;;)
    {
      سطر[سائز - 1] = '\xff';
      اگر ((fgets (سطر, سائز, f) == تہی) || ferror (f) || سطر[سائز - 1] != '\xff')
        {
          rv = (سطر[سائز - 1] != '\xff') ? ERANGE : ENOENT;
          سطر = 0;
          pw = 0;
          توڑو;
        }
      سطر[strcspn (سطر, "\n")] = 0;

      s = سطر;
      pw->pw_name = s++;
      اگر (! (s = strchr (s, ':')))
        جاری;

      *s++ = 0;
      pw->pw_passwd = s;
      اگر (! (s = strchr (s, ':')))
        جاری;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      اگر (*s != ':')
        جاری;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      اگر (*s != ':')
        جاری;

      *s++ = 0;
      pw->pw_gecos = s;
      اگر (! (s = strchr (s, ':')))
        جاری;

      *s++ = 0;
      pw->pw_dir = s;
      اگر (! (s = strchr (s, ':')))
        جاری;

      *s++ = 0;
      pw->pw_shell = s;
      توڑو;
    }
  *res = pw;
  اگر (rv)
    errno = rv;
  واپس rv;
}
#اگر_ختم

صحیح
سیٹ_home_env (uid_t id)
{
  ساخت passwd pwd;
  cleanup_free حرف *buf = تہی;
  طویل buf_سائز;
  cleanup_file FILE *دھارا = تہی;

  buf_سائز = sysconf (_SC_GETPW_R_SIZE_MAX);
  اگر (buf_سائز < 0)
    buf_سائز = 1024;

  buf = xmalloc (buf_سائز);

  دھارا = fopen ("/etc/passwd", "re");
  اگر (دھارا == تہی)
    واپس -1;

  برائے (;;)
    {
      ساخت passwd *ret_pw = تہی;

      صحیح ret = fgetpwent_r (دھارا, &pwd, buf, buf_سائز, &ret_pw);
      اگر (UNLIKELY (ret != 0))
        {
          اگر (ret == EINTR)
            جاری;

          اگر (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              واپس -1;
            }

          buf_سائز *= 2;
          buf = xrealloc (buf, buf_سائز);
          جاری;
        }

      اگر (ret_pw && ret_pw->pw_uid == id)
        {
          اگر (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          ورنہ
            واپس 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
ساکن صحیح
getsubidrange (uid_t id, صحیح is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *داخلہ = تہی;
  cleanup_free حرف *lineptr = تہی;
  size_t lenlineptr = 0, len_name;
  طویل buf_سائز;
  cleanup_free حرف *buf = تہی;
  ثابت حرف *name;
  ساخت passwd pwd;

  buf_سائز = sysconf (_SC_GETPW_R_SIZE_MAX);
  اگر (buf_سائز < 0)
    buf_سائز = 1024;

  buf = xmalloc (buf_سائز);
  برائے (;;)
    {
      صحیح ret;
      ساخت passwd *ret_pw = تہی;

      ret = getpwuid_r (id, &pwd, buf, buf_سائز, &ret_pw);
      اگر (LIKELY (ret == 0))
        {
          اگر (ret_pw)
            {
              name = ret_pw->pw_name;
              توڑو;
            }
          واپس -1;
        }

      اگر (ret != ERANGE)
        واپس -1;

      buf_سائز *= 2;
      buf = xrealloc (buf, buf_سائز);
    }

  len_name = لڑی_لمبائی (name);

  داخلہ = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  اگر (داخلہ == تہی)
    واپس -1;

  برائے (;;)
    {
      حرف *endptr;
      ssize_t اقرأ = getline (&lineptr, &lenlineptr, داخلہ);
      اگر (اقرأ < 0)
        واپس -1;

      اگر (اقرأ < (ssize_t) (len_name + 2))
        جاری;

      اگر (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        جاری;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      اگر (endptr >= &lineptr[اقرأ])
        واپس -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      واپس 0;
    }
}

#تعریف_کرو MIN(x, y) ((x) < (y) ? (x) : (y))

صحیح
format_default_id_mapping (حرف **برآمد, uid_t container_id, uid_t host_uid, uid_t host_id, صحیح is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, دستیاب = 0;
  cleanup_free حرف *بفر = تہی;
  صحیح written = 0;
  صحیح ret, بقایا;

  *برآمد = تہی;

  اگر (getsubidrange (host_uid, is_uid, &from, &دستیاب) < 0)
    واپس 0;

  /* More than enough space for all the mappings.  */
  بقایا = 15 * 5 * 3;
  بفر = xmalloc (بقایا + 1);

  اگر (container_id > 0)
    {
      uint32_t مستعمل = MIN (container_id, دستیاب);
      ret = snprintf (بفر + written, بقایا, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, مستعمل);
      اگر (UNLIKELY (ret >= بقایا))
        واپس crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      بقایا -= ret;

      from += مستعمل;
      دستیاب -= مستعمل;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (بفر + written, بقایا, "%d %d 1\n", container_id, host_id);
  اگر (UNLIKELY (ret >= بقایا))
    واپس crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  بقایا -= ret;

  /* Last mapping: use any id that is left.  */
  اگر (دستیاب)
    {
      ret = snprintf (بفر + written, بقایا, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, دستیاب);
      اگر (UNLIKELY (ret >= بقایا))
        واپس crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      بقایا -= ret;
    }

  *برآمد = بفر;
  بفر = تہی;
  واپس written;
}

ساکن صحیح
unset_cloexec_flag (صحیح fd)
{
  صحیح flags = fcntl (fd, F_GETFD);
  اگر (flags == -1)
    واپس -1;

  flags &= ~FD_CLOEXEC;

  واپس fcntl (fd, F_SETFD, flags);
}

ساکن خالی __attribute__ ((__noreturn__))
تسلسل_process_بچہ (حرف *راستہ, حرف **دلائل, ثابت حرف *cwd, حرف **envp, صحیح pipe_r,
                   صحیح pipe_w, صحیح برآمد_fd, صحیح err_fd, bool can_ignore_chdir_errors)
{
  حرف *tmp_دلائل[] = { راستہ, تہی };
  libcrun_error_t err = تہی;
  صحیح dev_null_fd = -1;
  صحیح ret;

  ret = mark_or_close_fds_ge_than (تہی, 3, false, &err);
  اگر (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->حالت_2, "%s", (err)->پیغام);

  اگر (برآمد_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      اگر (UNLIKELY (dev_null_fd < 0))
        محفوظ_exit (EXIT_FAILURE);
    }

  عارضی_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  عارضی_FAILURE_RETRY (close (pipe_r));

  dup2 (برآمد_fd >= 0 ? برآمد_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  اگر (برآمد_fd >= 0)
    unset_cloexec_flag (1);
  اگر (err_fd >= 0)
    unset_cloexec_flag (2);

  اگر (dev_null_fd >= 0)
    عارضی_FAILURE_RETRY (close (dev_null_fd));
  اگر (برآمد_fd >= 0)
    عارضی_FAILURE_RETRY (close (برآمد_fd));
  اگر (err_fd >= 0)
    عارضی_FAILURE_RETRY (close (err_fd));

  اگر (دلائل == تہی)
    دلائل = tmp_دلائل;

  اگر (cwd && chdir (cwd) < 0)
    {
      اگر (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        محفوظ_exit (EXIT_FAILURE);
    }

  execvpe (راستہ, دلائل, envp);
  محفوظ_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
صحیح
تسلسل_process_with_stdin_timeout_envp (حرف *راستہ, حرف **دلائل, ثابت حرف *cwd, صحیح timeout,
                                     حرف **envp, حرف *stdin, size_t stdin_len, صحیح برآمد_fd,
                                     صحیح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  صحیح stdin_pipe[2];
  pid_t pid;
  صحیح ret;
  cleanup_close صحیح pipe_r = -1;
  cleanup_close صحیح pipe_w = -1;
  sigset_t oldmask, نقاب;
  صحیح r, حالت_2;

  sigemptyset (&نقاب);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  اگر (timeout > 0)
    {
      sigaddset (&نقاب, SIGCHLD);
      ret = sigprocmask (SIG_بلاک, &نقاب, &oldmask);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  اگر (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      جاؤ restore_sig_نقاب_and_exit;
    }

  اگر (pid == 0)
    {
      /* run_process_child doesn't return.  */
      تسلسل_process_بچہ (راستہ, دلائل, cwd, envp, pipe_r, pipe_w, برآمد_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = عارضی_FAILURE_RETRY (اكتب (pipe_w, stdin, stdin_len));
  اگر (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      اگر (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          جاؤ restore_sig_نقاب_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  اگر (timeout)
    {
      time_t آغاز = وقت (تہی);
      time_t now;
      برائے (now = آغاز; now - آغاز < timeout; now = وقت (تہی))
        {
          siginfo_t info;
          صحیح elapsed = now - آغاز;
          ساخت timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&نقاب, &info, &ts_timeout);
          اگر (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              جاؤ restore_sig_نقاب_and_exit;
            }

          اگر (info.si_signo == SIGCHLD && info.si_pid == pid)
            جاؤ read_waitpid;

          اگر (ret < 0 && errno == EAGAIN)
            جاؤ timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", راستہ);
      جاؤ restore_sig_نقاب_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &حالت_2, 0);
  اگر (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  ورنہ
    ret = get_process_exit_حالت (حالت_2);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_نقاب_and_exit:
  اگر (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      اگر (pid > 0)
        {
          kill (pid, SIGKILL);
          عارضی_FAILURE_RETRY (waitpid (pid, &حالت_2, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, تہی);
      اگر (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  واپس ret;
}

صحیح
mark_or_close_fds_ge_than (libcrun_container_t *container, صحیح n, bool close_now, libcrun_error_t *err)
{
  cleanup_close صحیح cfd = -1;
  cleanup_dir DIR *dir = تہی;
  صحیح ret;
  صحیح fd;
  ساخت dirent *اگلا;

#اگر_تعریف_شدہ ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  اگر (close_now)
    GCOV_DUMP ();
#اگر_ختم

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  اگر (ret == 0)
    {
      اگر (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      واپس 0;
    }
  اگر (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    واپس crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  اگر (UNLIKELY (cfd < 0))
    واپس cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  dir = fdopendir (cfd);
  اگر (UNLIKELY (dir == تہی))
    واپس crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  برائے (اگلا = readdir (dir); اگلا; اگلا = readdir (dir))
    {
      صحیح val;
      ثابت حرف *name = اگلا->d_name;
      اگر (name[0] == '.')
        جاری;

      val = strtoll (name, تہی, 10);
      اگر (val < n || val == fd)
        جاری;

      اگر (close_now)
        {
          ret = close (val);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "close fd `%d`", val);

          اگر (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      ورنہ
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  واپس 0;
}

خالی
get_current_timestamp (حرف *برآمد, size_t len)
{
  ساخت timeval tv;
  ساخت tm now;
  حرف timestamp[64];

  gettimeofday (&tv, تہی);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, حجم (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (خالی) snprintf (برآمد, len, "%s.%06lldZ", timestamp, (طویل طویل صحیح) tv.tv_usec);
  برآمد[len - 1] = '\0';
}

صحیح
سیٹ_blocking_fd (صحیح fd, bool blocking, libcrun_error_t *err)
{
  صحیح ret, flags = fcntl (fd, F_GETFL, 0);
  اگر (UNLIKELY (flags < 0))
    واپس crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fcntl");
  واپس 0;
}

صحیح
parse_json_file (json_object **برآمد, ثابت حرف *jsondata, ساخت parser_context *ctx arg_غیر_مستعمل, libcrun_error_t *err)
{
  ساخت json_tokener *tok;
  شمار json_tokener_error jerr;

  *err = تہی;

  اگر (UNLIKELY (jsondata == تہی))
    واپس crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  اگر (UNLIKELY (tok == تہی))
    واپس crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_سیٹ_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *برآمد = json_tokener_parse_ex (tok, jsondata, (صحیح) لڑی_لمبائی (jsondata));
  jerr = json_tokener_get_error (tok);
  اگر (*برآمد == تہی || jerr != json_tokener_success)
    {
      صحیح saved_pos = (صحیح) tok->char_انحراف;

      json_tokener_free (tok);
      اگر (*برآمد)
        {
          json_object_put (*برآمد);
          *برآمد = تہی;
        }
      واپس crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  واپس 0;
}

#تعریف_کرو CHECK_ACCESS_NOT_EXECUTABLE 1
#تعریف_کرو CHECK_ACCESS_NOT_REGULAR 2

/* check that the specified path exists and it is executable.
   Return
   - 0 if the file is executable
   - CHECK_ACCESS_NOT_EXECUTABLE if it exists but it is not executable
   - CHECK_ACCESS_NOT_REGULAR if it not a regular file
   - -errno for any other generic error
 */
ساکن صحیح
check_access (ثابت حرف *راستہ)
{
  صحیح ret;
  mode_t منوال;

#اگر_تعریف_شدہ HAVE_EACCESS
#  define ACCESS(راستہ, منوال) (eaccess (راستہ, منوال))
#ورنہ
#  define ACCESS(راستہ, منوال) (access (راستہ, منوال))
#اگر_ختم

  ret = ACCESS (راستہ, X_OK);
  اگر (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      اگر (errno == EACCES)
        {
          صحیح saved_errno = errno;
          ret = ACCESS (راستہ, F_OK);
          errno = saved_errno;

          اگر (ret == 0)
            واپس CHECK_ACCESS_NOT_EXECUTABLE;
        }
      واپس -errno;
    }

  ret = get_file_type (&منوال, false, راستہ);
  اگر (UNLIKELY (ret < 0))
    واپس -errno;

  اگر (! S_ISREG (منوال))
    واپس CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  واپس 0;
}

صحیح
ڈھونڈو_executable (حرف **برآمد, ثابت حرف *executable_راستہ, ثابت حرف *cwd, libcrun_error_t *err)
{
  cleanup_free حرف *cwd_executable_راستہ = تہی;
  cleanup_free حرف *tmp = تہی;
  صحیح آخری_error = -ENOENT;
  حرف *it, *انتہا;
  صحیح ret;

  *برآمد = تہی;

  اگر (executable_راستہ == تہی || executable_راستہ[0] == '\0')
    واپس crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  اگر (executable_راستہ[0] != '/' && strchr (executable_راستہ, '/'))
    {
      cleanup_free حرف *cwd_allocated = تہی;

      اگر (cwd == تہی)
        {
          cwd_allocated = getcwd (تہی, 0);
          اگر (cwd_allocated == تہی)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_راستہ, err, "/", cwd, executable_راستہ, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      executable_راستہ = cwd_executable_راستہ;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  اگر (executable_راستہ[0] == '/')
    {
      ret = check_access (executable_راستہ);
      اگر (LIKELY (ret == 0))
        {
          *برآمد = xstrdup (executable_راستہ);
          واپس 0;
        }
      آخری_error = ret;
      جاؤ fail;
    }

  انتہا = tmp = xstrdup (getenv ("PATH"));

  جب ((it = strsep (&انتہا, ":")))
    {
      cleanup_free حرف *راستہ = تہی;

      اگر (it == انتہا)
        it = ".";

      ret = append_paths (&راستہ, err, it, executable_راستہ, تہی);
      اگر (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          جاری;
        }

      ret = check_access (راستہ);
      اگر (ret == 0)
        {
          /* Change owner.  */
          *برآمد = راستہ;
          راستہ = تہی;
          واپس 0;
        }

      اگر (ret == -ENOENT)
        جاری;

      آخری_error = ret;
    }

fail:
  انتخاب (آخری_error)
    {
    حالت CHECK_ACCESS_NOT_EXECUTABLE:
      واپس crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_راستہ);

    حالت CHECK_ACCESS_NOT_REGULAR:
      واپس crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_راستہ);

    طےشدہ:
      errno = -آخری_error;
      اگر (errno == ENOENT)
        واپس crun_make_error (err, errno, "executable file `%s` not found%s", executable_راستہ, executable_راستہ[0] == '/' ? "" : " in $PATH");
      واپس crun_make_error (err, errno, "open `%s`", executable_راستہ);
    }
}

#اگر_تعریف_شدہ HAVE_FGETXATTR

ساکن ssize_t
محفوظ_read_xattr (حرف **ret, صحیح sfd, ثابت حرف *srcname, ثابت حرف *name, size_t initial_سائز, libcrun_error_t *err)
{
  cleanup_free حرف *بفر = تہی;
  ssize_t current_سائز;
  ssize_t s;

  current_سائز = (ssize_t) initial_سائز;
  بفر = xmalloc (current_سائز + 1);

  جب (1)
    {
      s = fgetxattr (sfd, name, بفر, current_سائز);
      اگر (UNLIKELY (s < 0))
        واپس crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      اگر (s < current_سائز)
        توڑو;

      current_سائز *= 2;
      بفر = xrealloc (بفر, current_سائز + 1);
    }

  اگر (s <= 0)
    واپس s;

  بفر[s] = '\0';

  /* Change owner.  */
  *ret = بفر;
  بفر = تہی;

  واپس s;
}

ساکن ssize_t
copy_xattr (صحیح sfd, صحیح dfd, ثابت حرف *srcname, ثابت حرف *destname, libcrun_error_t *err)
{
  cleanup_free حرف *buf = تہی;
  ssize_t xattr_len;
  حرف *it;

  xattr_len = flistxattr (sfd, تہی, 0);
  اگر (UNLIKELY (xattr_len < 0))
    {
      اگر (errno == ENOTSUP)
        واپس 0;

      واپس crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  اگر (xattr_len == 0)
    واپس 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  اگر (UNLIKELY (xattr_len < 0))
    واپس crun_make_error (err, errno, "flistxattr `%s`", srcname);

  برائے (it = buf; it - buf < xattr_len; it += لڑی_لمبائی (it) + 1)
    {
      cleanup_free حرف *v = تہی;
      ssize_t s;

      s = محفوظ_read_xattr (&v, sfd, srcname, it, 256, err);
      اگر (UNLIKELY (s < 0))
        واپس s;

      s = fsetxattr (dfd, it, v, s, 0);
      اگر (UNLIKELY (s < 0))
        {
          اگر (errno == EINVAL || errno == EOPNOTSUPP)
            جاری;

          واپس crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  واپس 0;
}

#اگر_ختم

ساکن صحیح
copy_rec_stat_file_at (صحیح dfd, ثابت حرف *راستہ, mode_t *منوال, off_t *سائز, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  ساخت stat st;
  صحیح ret;

#اگر_تعریف_شدہ HAVE_STATX
  ساخت statx stx = {
    0,
  };

  ret = statx (dfd, راستہ, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_منوال | STATX_سائز | STATX_UID | STATX_GID, &stx);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS || errno == EINVAL)
        جاؤ fallback;

      واپس ret;
    }

  *منوال = stx.stx_mode;
  *سائز = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  واپس ret;

fallback:
#اگر_ختم
  ret = fstatat (dfd, راستہ, &st, AT_SYMLINK_NOFOLLOW);

  *منوال = st.st_mode;
  *سائز = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  واپس ret;
}

صحیح
copy_recursive_fd_مقصد_fd (صحیح srcdirfd, صحیح dfd, ثابت حرف *srcname, ثابت حرف *destname, libcrun_error_t *err)
{
  cleanup_close صحیح destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = تہی;
  ساخت dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  اگر (UNLIKELY (dsrcfd == تہی))
    {
      عارضی_FAILURE_RETRY (close (srcdirfd));
      واپس crun_make_error (err, errno, "open directory `%s`", destname);
    }

  برائے (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close صحیح srcfd = -1;
      cleanup_close صحیح destfd = -1;
      cleanup_free حرف *ہدف_buf = تہی;
      صحیح ret;
      mode_t منوال;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      اگر (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        جاری;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &منوال, &st_size, &rdev, &uid, &gid);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      انتخاب (منوال & S_IFMT)
        {
        حالت S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          اگر (UNLIKELY (srcfd < 0))
            واپس crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          اگر (UNLIKELY (destfd < 0))
            واپس crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_مقصد_fd (srcfd, destfd, 1, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

#اگر_تعریف_شدہ HAVE_FGETXATTR
          ret = (صحیح) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
#اگر_ختم

          عارضی_FAILURE_RETRY (close (destfd));
          destfd = -1;
          توڑو;

        حالت S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, منوال);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          اگر (UNLIKELY (srcfd < 0))
            واپس crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          اگر (UNLIKELY (destfd < 0))
            واپس crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#اگر_تعریف_شدہ HAVE_FGETXATTR
          ret = (صحیح) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
#اگر_ختم

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_مقصد_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          توڑو;

        حالت S_IFLNK:
          ret = محفوظ_readlinkat (dirfd (dsrcfd), de->d_name, &ہدف_buf, st_size, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          ret = symlinkat (ہدف_buf, destdirfd, de->d_name);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          توڑو;

        حالت S_IFBLK:
        حالت S_IFCHR:
        حالت S_IFIFO:
        حالت S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, منوال, rdev);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          توڑو;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#اگر_غیر_تعریف_شدہ ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#اگر_ختم

      ret = fchmodat (destdirfd, de->d_name, منوال & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      اگر (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          اگر (errno == ENOTSUP)
            جاری;

          واپس crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  واپس 0;
}

ثابت حرف *
ڈھونڈو_annotation (libcrun_container_t *container, ثابت حرف *name)
{
  اگر (container->container_def->annotations == تہی)
    واپس تہی;

  واپس ڈھونڈو_string_map_قدر (container->annotations, name);
}

صحیح
محفوظ_write (صحیح fd, ثابت حرف *fname, ثابت خالی *buf, size_t گنتی, libcrun_error_t *err)
{
  size_t written = 0;
  جب (written < گنتی)
    {
      ssize_t w = اكتب (fd, (ثابت حرف *) buf + written, گنتی - written);
      اگر (UNLIKELY (w < 0))
        {
          اگر (errno == EINTR || errno == EAGAIN)
            جاری;
          واپس crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  واپس 0;
}

#تعریف_کرو MAX_PARTS 32

صحیح
append_paths (حرف **برآمد, libcrun_error_t *err, ...)
{
  ثابت حرف *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t کل_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_آغاز (ap, err);
  برائے (;;)
    {
      ثابت حرف *part;
      size_t سائز;

      part = va_arg (ap, ثابت حرف *);
      اگر (part == تہی)
        توڑو;

      اگر (n_parts == MAX_PARTS)
        {
          va_انتہا (ap);
          واپس crun_make_error (err, EINVAL, "too many paths specified");
        }

      اگر (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          جب (part[0] == '/' && part[1] == '/')
            part++;
        }
      ورنہ
        {
          /* And drop any initial '/' for other components.  */
          جب (part[0] == '/')
            part++;
        }

      سائز = لڑی_لمبائی (part);
      اگر (سائز == 0)
        جاری;

      جب (سائز > 1 && part[سائز - 1] == '/')
        سائز--;

      parts[n_parts] = part;
      sizes[n_parts] = سائز;

      n_parts++;
    }
  va_انتہا (ap);

  کل_len = n_parts + 1;
  برائے (i = 0; i < n_parts; i++)
    کل_len += sizes[i];

  *برآمد = xmalloc (کل_len);

  copied = 0;
  برائے (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*برآمد)[copied - 1] == '/';
      اگر (i > 0 && ! has_trailing_slash)
        {
          (*برآمد)[copied] = '/';
          copied += 1;
        }

      حافظہ_نقل (*برآمد + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*برآمد)[copied] = '\0';
  واپس 0;
}

#اگر __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#ورنہ
#  define __nonstring
#اگر_ختم

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
ساکن صحیح
b64_داخلہ (حرف c)
{
  ثابت حرف جدول[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  صحیح i;

  برائے (i = 0; i < 64; i++)
    {
      اگر (جدول[i] == c)
        واپس i;
    }
  واپس -1;
}

صحیح
base64_decode (ثابت حرف *iptr, size_t isize, حرف *optr, size_t osize, size_t *nbytes)
{
  صحیح i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  غیرعلامتی حرف ڈیٹا[4];

  *nbytes = 0;
  جب (consumed < isize && (*nbytes) + 3 < osize)
    {
      جب ((i < 4) && (consumed < isize))
        {
          tmp = b64_داخلہ (*iptr++);
          consumed++;
          اگر (tmp != -1)
            {
              اگر (UNLIKELY (pad > 0))
                واپس -1;
              ڈیٹا[i++] = tmp;
            }
          ورنہ اگر (*(iptr - 1) == '=')
            {
              ڈیٹا[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      اگر (i == 4)
        {
          اگر (UNLIKELY (pad > 2))
            واپس -1;
          *optr++ = (ڈیٹا[0] << 2) | ((ڈیٹا[1] & 0x30) >> 4);
          *optr++ = ((ڈیٹا[1] & 0xf) << 4) | ((ڈیٹا[2] & 0x3c) >> 2);
          *optr++ = ((ڈیٹا[2] & 0x3) << 6) | ڈیٹا[3];
          (*nbytes) += 3 - pad;
        }
      ورنہ
        {
          /* I did not get all the data.  */
          consumed -= i;
          واپس consumed;
        }
      i = 0;
    }
  واپس consumed;
}

حرف *
get_user_name (uid_t uid)
{
  ساخت passwd pd;
  ساخت passwd *عارضی_نتیجہ_ptr;
  حرف pwdbuffer[200];
  اگر (! getpwuid_r (uid, &pd, pwdbuffer, حجم (pwdbuffer), &عارضی_نتیجہ_ptr))
    واپس xstrdup (pd.pw_name);
  واپس xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
حرف *
libcrun_sysctl_کلید_مقصد_proc_راستہ (ثابت حرف *کلید)
{
  ثابت حرف *sep;
  حرف *ret, *it;

  sep = strpbrk (کلید, "./");

  /* No separator at all, or the key is already in the path form.  */
  اگر (sep == تہی || *sep == '/')
    واپس xstrdup (کلید);

  ret = xstrdup (کلید);
  برائے (it = ret; *it; it++)
    {
      اگر (*it == '.')
        *it = '/';
      ورنہ اگر (*it == '/')
        *it = '.';
    }

  واپس ret;
}

صحیح
has_suffix (ثابت حرف *str, ثابت حرف *suffix)
{
  اگر (! str || ! suffix)
    واپس 0;
  size_t lenstr = لڑی_لمبائی (str);
  size_t lensuffix = لڑی_لمبائی (suffix);
  اگر (lensuffix > lenstr)
    واپس 0;
  واپس memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

حرف *
str_join_array (صحیح انحراف, size_t سائز, حرف *ثابت array[], ثابت حرف *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = تہی;
  size_t i, کل_سائز = (سائز - 1) * (jlen = لڑی_لمبائی (joint)) + 1;
  حرف *نتیجہ, *p;

  lens = xmalloc (سائز * حجم (*lens));

  برائے (i = 0; i < سائز; ++i)
    {
      کل_سائز += (lens[i] = لڑی_لمبائی (array[i]));
    }
  p = نتیجہ = xmalloc (کل_سائز);
  برائے (i = انحراف; i < سائز; ++i)
    {
      حافظہ_نقل (p, array[i], lens[i]);
      p += lens[i];
      اگر (i < سائز - 1)
        {
          حافظہ_نقل (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  واپس نتیجہ;
}

صحیح
libcrun_mmap (ساخت libcrun_mmap_s **ret, خالی *addr, size_t لمبائی,
              صحیح prot, صحیح flags, صحیح fd, off_t انحراف,
              libcrun_error_t *err)
{
  ساخت libcrun_mmap_s *mmap_s = تہی;

  خالی *mapped = mmap (addr, لمبائی, prot, flags, fd, انحراف);
  اگر (mapped == MAP_FAILED)
    واپس crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (حجم (ساخت libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->لمبائی = لمبائی;

  *ret = mmap_s;

  واپس 0;
}

صحیح
libcrun_munmap (ساخت libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  صحیح ret;

  ret = munmap (mmap->addr, mmap->لمبائی);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "munmap");

  آزاد (mmap);

  واپس 0;
}

ساکن طویل
read_file_as_long_or_default (ثابت حرف *راستہ, طویل def_قدر)
{
  cleanup_free حرف *content = تہی;
  libcrun_error_t tmp_err = تہی;
  حرف *endptr;
  طویل val;
  صحیح ret;

  ret = read_all_file (راستہ, &content, تہی, &tmp_err);
  اگر (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      واپس def_قدر;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  اگر (UNLIKELY (errno))
    واپس def_قدر;
  اگر (endptr == content || (*endptr && *endptr != '\n'))
    واپس def_قدر;
  واپس val;
}

#تعریف_کرو DEFAULT_حد_سے_تجاوز_ID 65534

uid_t
get_حد_سے_تجاوز_uid (خالی)
{
  ساکن atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  اگر (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_حد_سے_تجاوز_ID);
      cached_uid = uid;
    }
  واپس uid;
}

gid_t
get_حد_سے_تجاوز_gid (خالی)
{
  ساکن atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  اگر (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_حد_سے_تجاوز_ID);
      cached_gid = gid;
    }
  واپس gid;
}

خالی
consume_trailing_slashes (حرف *راستہ)
{
  اگر (! راستہ || راستہ[0] == '\0')
    واپس;

  حرف *آخری = راستہ + لڑی_لمبائی (راستہ);

  جب (آخری > راستہ && *(آخری - 1) == '/')
    آخری--;

  *آخری = '\0';
}

حرف **
read_dir_entries (ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = تہی;
  size_t n_entries = 0;
  size_t entries_سائز = 16;
  حرف **entries = تہی;
  ساخت dirent *de;

  dir = opendir (راستہ);
  اگر (UNLIKELY (dir == تہی))
    {
      crun_make_error (err, errno, "opendir `%s`", راستہ);
      واپس تہی;
    }

  entries = xmalloc (entries_سائز * حجم (حرف *));
  جب ((de = readdir (dir)))
    {
      اگر (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        جاری;
      اگر (n_entries == entries_سائز)
        {
          entries_سائز *= 2;
          entries = xrealloc (entries, entries_سائز * حجم (حرف *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * حجم (حرف *));
  entries[n_entries] = تہی;

  واپس entries;
}

صحیح
cpuset_string_مقصد_bitmask (ثابت حرف *str, حرف **برآمد, size_t *برآمد_سائز, libcrun_error_t *err)
{
  cleanup_free حرف *نقاب = تہی;
  size_t نقاب_سائز = 0;
  ثابت حرف *p = str;
  حرف *endptr;

  جب (*p)
    {
      طویل طویل آغاز_range, انتہا_range;

      اگر (*p < '0' || *p > '9')
        جاؤ invalid_داخلہ;

      آغاز_range = strtoll (p, &endptr, 10);
      اگر (آغاز_range < 0)
        جاؤ invalid_داخلہ;

      p = endptr;

      اگر (*p != '-')
        انتہا_range = آغاز_range;
      ورنہ
        {
          p++;

          اگر (*p < '0' || *p > '9')
            جاؤ invalid_داخلہ;

          انتہا_range = strtoll (p, &endptr, 10);

          اگر (انتہا_range < آغاز_range)
            جاؤ invalid_داخلہ;

          p = endptr;
        }

      /* Just set some limit.  */
      اگر (انتہا_range > (1 << 20))
        جاؤ invalid_داخلہ;

      اگر (انتہا_range >= (طویل طویل) (نقاب_سائز * CHAR_BIT))
        {
          size_t new_نقاب_سائز = (انتہا_range / CHAR_BIT) + 1;
          نقاب = xrealloc (نقاب, new_نقاب_سائز);
          memset (نقاب + نقاب_سائز, 0, new_نقاب_سائز - نقاب_سائز);
          نقاب_سائز = new_نقاب_سائز;
        }

      برائے (طویل طویل i = آغاز_range; i <= انتہا_range; i++)
        نقاب[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      اگر (*p == ',')
        p++;
      ورنہ اگر (*p)
        جاؤ invalid_داخلہ;
    }

  *برآمد = نقاب;
  نقاب = تہی;
  *برآمد_سائز = نقاب_سائز;

  واپس 0;

invalid_داخلہ:
  واپس crun_make_error (err, 0, "cannot parse input `%s`", str);
}

ساخت channel_fd_pair
{
  ساخت ring_بفر *rb;

  صحیح اندر_fd;
  صحیح برآمد_fd;

  صحیح infd_epoll_events;
  صحیح outfd_epoll_events;
};

ساخت channel_fd_pair *
channel_fd_pair_new (صحیح اندر_fd, صحیح برآمد_fd, size_t سائز)
{
  ساخت channel_fd_pair *channel = xmalloc (حجم (ساخت channel_fd_pair));
  channel->اندر_fd = اندر_fd;
  channel->برآمد_fd = برآمد_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_بفر_make (سائز);
  واپس channel;
}

خالی
channel_fd_pair_free (ساخت channel_fd_pair *channel)
{
  اگر (channel == تہی)
    واپس;

  ring_بفر_free (channel->rb);
  آزاد (channel);
}

صحیح
channel_fd_pair_process (ساخت channel_fd_pair *channel, صحیح epollfd, libcrun_error_t *err)
{
  bool is_داخلہ_eagain = false, is_output_eagain = false, repeat;
  صحیح ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  برائے (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      اگر (ring_بفر_get_space_دستیاب (channel->rb) > 0)
        {
          ret = ring_بفر_read (channel->rb, channel->اندر_fd, &is_داخلہ_eagain, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          اگر (ret > 0)
            repeat = true;
        }
      اگر (ring_بفر_get_ڈیٹا_دستیاب (channel->rb) > 0)
        {
          ret = ring_بفر_write (channel->rb, channel->برآمد_fd, &is_output_eagain, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          اگر (ret > 0)
            repeat = true;
        }
    }

  اگر (epollfd >= 0)
    {
      size_t دستیاب = ring_بفر_get_space_دستیاب (channel->rb);
      size_t مستعمل = ring_بفر_get_ڈیٹا_دستیاب (channel->rb);
      صحیح events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (دستیاب > 0 && ! is_output_eagain) ? (EPOLLIN | (is_داخلہ_eagain ? EPOLLET : 0)) : 0;
      اگر (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->اندر_fd, events, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (مستعمل > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      اگر (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->برآمد_fd, events, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          channel->outfd_epoll_events = events;
        }
    }
  واپس 0;
}

ساکن صحیح
open_proc (libcrun_error_t *err)
{
  صحیح fd;

#اگر_تعریف_شدہ HAVE_NEW_MOUNT_API
  cleanup_close صحیح fsfd = -1;
  صحیح ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  اگر (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, تہی, تہی, 0);
      اگر (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          اگر (fd >= 0)
            واپس fd;
        }
    }
#اگر_ختم

  fd = open ("/proc", O_راستہ | O_DIRECTORY | O_CLOEXEC);
  اگر (fd < 0)
    واپس crun_make_error (err, errno, "open `/proc`");
  واپس fd;
}

صحیح
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  اگر (! container)
    واپس open_proc (err);

  اگر (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  واپس container->proc_fd;
}

صحیح
libcrun_open_proc_file (libcrun_container_t *container, ثابت حرف *راستہ, صحیح flags, libcrun_error_t *err)
{
  صحیح proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  اگر (proc_fd < 0)
    واپس proc_fd;

  fd = عارضی_FAILURE_RETRY (openat (proc_fd, راستہ, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  اگر (! container)
    عارضی_FAILURE_RETRY (close (proc_fd));

  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, saved_errno, "openat `/proc/%s`", راستہ);

  واپس fd;
}

صحیح
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ثابت حرف *راستہ, صحیح flags, libcrun_error_t *err)
{
  cleanup_free حرف *بھرا_راستہ = تہی;
  xasprintf (&بھرا_راستہ, "%d/%s", pid, راستہ);
  واپس libcrun_open_proc_file (container, بھرا_راستہ, flags, err);
}
