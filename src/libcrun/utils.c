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

#وصف_ڪريو GNU_ذريعو
#ضمن <config.h>
#ضمن "utils.h"
#ضمن "ring_buffer.h"
#ضمن <stdarg.h>
#ضمن <inttypes.h>
#ضمن <unistd.h>
#ضمن <string.h>
#ضمن <libgen.h>
#ضمن <fcntl.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <sys/un.h>
#ضمن <sys/socket.h>
#ضمن <sys/signalfd.h>
#ضمن <sys/epoll.h>
#ضمن <sys/syscall.h>
#ضمن <sys/wait.h>
#ضمن <pwd.h>
#ضمن <grp.h>
#ضمن <time.h>
#ضمن <sys/time.h>
#ضمن <sys/xattr.h>
#ضمن <sys/sysmacros.h>
#ضمن <sys/vfs.h>
#ضمن <linux/magic.h>
#ضمن <limits.h>
#ضمن <sys/mman.h>
#جيڪڏهن_وصف_ٿيل HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#جيڪڏهن_پورو
#إذا HAVE_STDATOMIC_H
#  include <stdatomic.h>
#وإلا
#  define atomic_long متطاير ڊگهو
#  define atomic_int متطاير صحيح
#جيڪڏهن_پورو
#ضمن "syscalls.h"

#جيڪڏهن_وصف_نه_ٿيل CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل RESOLVE_اندريون_پاڙ
#  define RESOLVE_اندريون_پاڙ 0x10
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل __NR_close_range
#  define __NR_close_range 436
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل __NR_openat2
#  define __NR_openat2 437
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل PROC_USER_INIT_INO
#  define PROC_USER_INIT_INO ((ino_t) 0xEFFFFFFD)
#جيڪڏهن_پورو

#وصف_ڪريو MAX_READLINKS 32
#وصف_ڪريو MAX_OPENAT2_EAGAIN_RETRIES 128

صحيح
crun_رستو_exists (ثابت حرف *رستو, libcrun_error_t *err)
{
  صحيح ret = access (رستو, F_OK);
  إذا (ret < 0)
    {
      إذا (errno == ENOENT)
        أرجع 0;
      أرجع crun_make_error (err, errno, "access `%s`", رستو);
    }
  أرجع 1;
}

صحيح
xasprintf (حرف **str, ثابت حرف *fmt, ...)
{
  صحيح ret;
  va_list دليلون_list;

  va_شروعات (دليلون_list, fmt);

  ret = vasprintf (str, fmt, دليلون_list);
  إذا (UNLIKELY (ret < 0))
    OOM ();

  va_پڇاڙي (دليلون_list);
  أرجع ret;
}

صحيح
write_file_at_with_flags (صحيح dirfd, صحيح flags, mode_t گهڻائي, ثابت حرف *name, ثابت فراغ *معلومات, size_t len, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  صحيح ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, گهڻائي);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `%s` for writing", name);

  ret = محفوظ_write (fd, name, معلومات, len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع (len > وڌ_کان_وڌ_عدد) ? وڌ_کان_وڌ_عدد : (صحيح) len;
}

صحيح
detach_process ()
{
  pid_t pid;
  إذا (setsid () < 0)
    أرجع -1;
  pid = fork ();
  إذا (pid < 0)
    أرجع -1;
  إذا (pid != 0)
    محفوظ_exit (EXIT_SUCCESS);
  أرجع 0;
}

صحيح
get_file_type_fd (صحيح fd, mode_t *گهڻائي)
{
  بنية stat st;
  صحيح ret;

#جيڪڏهن_وصف_ٿيل HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (fd, "", AT_خالي_جايون_رستو | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;

      أرجع ret;
    }
  *گهڻائي = stx.stx_mode;
  أرجع ret;

fallback:
#جيڪڏهن_پورو
  ret = fstat (fd, &st);
  *گهڻائي = st.st_mode;
  أرجع ret;
}

صحيح
get_file_type_at (صحيح dirfd, mode_t *گهڻائي, bool nofollow, ثابت حرف *رستو)
{
  صحيح خالي_جايون_رستو = رستو == عدم ? AT_خالي_جايون_رستو : 0;
  بنية stat st;
  صحيح ret;

#جيڪڏهن_وصف_ٿيل HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (dirfd, رستو ?: "", خالي_جايون_رستو | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;

      أرجع ret;
    }
  *گهڻائي = stx.stx_mode;
  أرجع ret;

fallback:
#جيڪڏهن_پورو
  ret = fstatat (dirfd, رستو ?: "", &st, خالي_جايون_رستو | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *گهڻائي = st.st_mode;
  أرجع ret;
}

صحيح
get_file_type (mode_t *گهڻائي, bool nofollow, ثابت حرف *رستو)
{
  أرجع get_file_type_at (AT_FDCWD, گهڻائي, nofollow, رستو);
}

صحيح
create_file_if_missing_at (صحيح dirfd, ثابت حرف *file, mode_t گهڻائي, libcrun_error_t *err)
{
  cleanup_close صحيح fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, گهڻائي);
  إذا (fd_write < 0)
    {
      صحيح saved_errno = errno;
      mode_t tmp_گهڻائي;
      صحيح ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_گهڻائي, true, file);
      إذا (ret == 0 && S_ISREG (tmp_گهڻائي))
        أرجع 0;

      أرجع crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  أرجع 0;
}

ساكن صحيح
ensure_directory_اندروني_at (صحيح dirfd, حرف *رستو, size_t len, صحيح گهڻائي, libcrun_error_t *err)
{
  حرف *it = رستو + len;
  صحيح ret = 0;
  bool والد_created = false;

  لكل (;;)
    {
      ret = mkdirat (dirfd, رستو, گهڻائي);
      إذا (ret == 0 || errno == EEXIST)
        أرجع 0;

      صحيح saved_errno = errno;
      إذا (والد_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = عدم;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (رستو, false, &tmp_err);
          إذا (ret > 0)
            أرجع 0;
          إذا (ret < 0)
            crun_error_release (&tmp_err);

          أرجع crun_make_error (err, saved_errno, "create directory `%s`", رستو);
        }

      طالما (it > رستو && *it != '/')
        {
          it--;
          len--;
        }
      إذا (it == رستو)
        أرجع 0;

      *it = '\0';
      ret = ensure_directory_اندروني_at (dirfd, رستو, len - 1, گهڻائي, err);
      *it = '/';
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      والد_created = true;
    }
  أرجع ret;
}

صحيح
crun_ensure_directory_at (صحيح dirfd, ثابت حرف *رستو, صحيح گهڻائي, bool nofollow, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *tmp = xstrdup (رستو);
  ret = ensure_directory_اندروني_at (dirfd, tmp, لڙي_ڊيگهه (tmp), گهڻائي, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_dir_p_at (dirfd, رستو, nofollow, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret == 0)
    أرجع crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", رستو);

  أرجع 0;
}

ساكن صحيح
check_fd_is_رستو (ثابت حرف *رستو, صحيح fd, ثابت حرف *fdname, libcrun_error_t *err)
{
  proc_fd_رستو_t fdpath;
  size_t رستو_len = لڙي_ڊيگهه (رستو);
  حرف link[PATH_MAX];
  صحيح ret;

  get_proc_self_fd_رستو (fdpath, fd);
  ret = عارضي_FAILURE_RETRY (readlink (fdpath, link, حجم (link)));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "readlink `%s`", fdname);

  إذا (((size_t) ret) != رستو_len || memcmp (link, رستو, رستو_len))
    أرجع crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, رستو);

  أرجع 0;
}

ساكن صحيح
check_fd_under_رستو (ثابت حرف *rootfs, size_t rootfslen, صحيح fd, ثابت حرف *fdname, libcrun_error_t *err)
{
  proc_fd_رستو_t fdpath;
  حرف link[PATH_MAX];
  صحيح ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  إذا (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    أرجع 0;

  get_proc_self_fd_رستو (fdpath, fd);
  ret = عارضي_FAILURE_RETRY (readlink (fdpath, link, حجم (link)));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "readlink `%s`", fdname);

  إذا (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    أرجع crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  أرجع 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
ساكن فراغ
close_and_replace (صحيح *oldfd, صحيح newfd)
{
  إذا (*oldfd >= 0)
    عارضي_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
حرف *chroot_realpath (ثابت حرف *chroot, ثابت حرف *رستو, حرف resolved_رستو[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
ساكن صحيح
محفوظ_openat_fallback (صحيح dirfd, ثابت حرف *rootfs, ثابت حرف *رستو, صحيح flags,
                      صحيح گهڻائي, libcrun_error_t *err)
{
  cleanup_free حرف *والد_رستو = عدم;
  ثابت حرف *آخري_component = عدم;
  ثابت حرف *orig_رستو = رستو;
  ثابت حرف *رستو_اندريون_chroot;
  cleanup_close صحيح fd = -1;
  حرف resolved[PATH_MAX];
  حرف بفر[PATH_MAX];
  size_t rootfs_len = محفوظ_strlen (rootfs);
  صحيح ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  إذا (flags & O_NOFOLLOW)
    {
      حرف *sep;

      والد_رستو = xstrdup (رستو);
      sep = strrchr (والد_رستو, '/');
      إذا (sep == عدم)
        {
          /* No parent directory, the entire path is the last component.  */
          آخري_component = رستو;
          والد_رستو[0] = '\0';
        }
      وإلا إذا (sep[1] != '\0')
        {
          *sep = '\0';
          آخري_component = رستو + (sep - والد_رستو) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      إذا (آخري_component)
        رستو = والد_رستو;
    }

  رستو_اندريون_chroot = chroot_realpath (rootfs, رستو, بفر);
  إذا (رستو_اندريون_chroot == عدم)
    أرجع crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_رستو);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  إذا (rootfs_len > 0 && لڙي_ڀيٽ (رستو_اندريون_chroot, rootfs, rootfs_len) == 0)
    رستو_اندريون_chroot += rootfs_len;
  رستو_اندريون_chroot = consume_slashes (رستو_اندريون_chroot);

  إذا (آخري_component)
    {
      ret = snprintf (resolved, حجم (resolved), "%s%s%s", رستو_اندريون_chroot,
                      رستو_اندريون_chroot[0] == '\0' ? "" : "/", آخري_component);
      إذا (UNLIKELY (ret >= (صحيح) حجم (resolved)))
        أرجع crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_رستو);

      رستو_اندريون_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  إذا (رستو_اندريون_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      أرجع ret;
    }

  ret = openat (dirfd, رستو_اندريون_chroot, flags, گهڻائي);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "open `%s`", orig_رستو);

  fd = ret;

  ret = check_fd_under_رستو (rootfs, rootfs_len, fd, orig_رستو, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = fd;
  fd = -1;
  أرجع ret;
}

صحيح
محفوظ_openat (صحيح dirfd, ثابت حرف *rootfs, ثابت حرف *رستو, صحيح flags, صحيح گهڻائي,
             libcrun_error_t *err)
{
  ساكن bool openat2_supported = true;
  صحيح retries = MAX_OPENAT2_EAGAIN_RETRIES;
  صحيح ret;

  إذا (is_خالي_جايون_string (رستو))
    {
      cleanup_close صحيح fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      إذا (is_خالي_جايون_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, گهڻائي);
      إذا (UNLIKELY (fd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      إذا (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_رستو (rootfs, fd, رستو, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = fd;
      fd = -1;
      أرجع ret;
    }

  إذا (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, رستو, flags, گهڻائي, RESOLVE_اندريون_پاڙ);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (errno == EINTR)
            اذهب repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          إذا (errno == EAGAIN && retries-- > 0)
            اذهب repeat;
          إذا (errno == ENOSYS)
            openat2_supported = false;
          إذا (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            أرجع محفوظ_openat_fallback (dirfd, rootfs, رستو, flags, گهڻائي, err);

          أرجع crun_make_error (err, errno, "openat2 `%s`", رستو);
        }

      أرجع ret;
    }

  أرجع محفوظ_openat_fallback (dirfd, rootfs, رستو, flags, گهڻائي, err);
}

ssize_t
محفوظ_readlinkat (صحيح dfd, ثابت حرف *name, حرف **بفر, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_سائيز = hint > 0 ? hint + 1 : 512;
  cleanup_free حرف *tmp_buf = عدم;
  ssize_t سائيز;

  افعل
    {
      إذا (tmp_buf != عدم)
        buf_سائيز += 256;

      tmp_buf = xrealloc (tmp_buf, buf_سائيز);

      سائيز = readlinkat (dfd, name, tmp_buf, buf_سائيز);
      إذا (UNLIKELY (سائيز < 0))
        أرجع crun_make_error (err, errno, "readlink `%s`", name);
  } طالما (سائيز == buf_سائيز);

  /* Always NUL terminate the buffer.  */
  tmp_buf[سائيز] = '\0';

  /* Move ownership to BUFFER.  */
  *بفر = tmp_buf;
  tmp_buf = عدم;

  أرجع سائيز;
}

ساكن صحيح crun_محفوظ_ensure_at (bool do_open, bool dir, bool follow_symlinks, صحيح dirfd, ثابت حرف *dirpath,
                                ثابت حرف *رستو, صحيح گهڻائي, صحيح max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
ساكن صحيح
crun_محفوظ_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, صحيح dirfd, ثابت حرف *dirpath, صحيح cwd,
                                 ثابت حرف *npath, ثابت حرف *cur, ثابت حرف *rest,
                                 صحيح گهڻائي, صحيح max_readlinks, libcrun_error_t *err)
{
  cleanup_free حرف *هدف = عدم;
  cleanup_free حرف *new_رستو = عدم;
  ssize_t ret;

  ret = محفوظ_readlinkat (cwd, cur, &هدف, 0, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_رستو, "%.*s%s%s%s", هدف[0] == '/' ? 0 : (صحيح) (cur - npath), npath, هدف,
             rest ? "/" : "", rest ? rest : "");

  أرجع crun_محفوظ_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_رستو, گهڻائي, max_readlinks - 1, err);
}

ساكن صحيح
crun_محفوظ_ensure_at (bool do_open, bool dir, bool follow_symlinks, صحيح dirfd, ثابت حرف *dirpath,
                     ثابت حرف *رستو, صحيح گهڻائي, صحيح max_readlinks, libcrun_error_t *err)
{
  cleanup_close صحيح wd_cleanup = -1;
  cleanup_free حرف *npath = عدم;
  bool آخري_component = false;
  size_t اونهائي = 0;
  حرف *cur;
  حرف *it;
  صحيح cwd;
  صحيح ret;

  إذا (max_readlinks <= 0)
    أرجع crun_make_error (err, ELOOP, "resolve path `%s`", رستو);

  رستو = consume_slashes (رستو);

  /* Empty path, nothing to do.  */
  إذا (*رستو == '\0')
    {
      إذا (do_open)
        أرجع open (dirpath, O_CLOEXEC | O_رستو, 0);
      أرجع 0;
    }

  npath = xstrdup (رستو);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  طالما (cur)
    {
      إذا (it)
        *it = '\0';
      وإلا
        آخري_component = true;

      إذا (cur[0] == '\0')
        اكسر;

      إذا (strcmp (cur, ".") == 0)
        اذهب ايندڙ;
      وإلا إذا (strcmp (cur, ".."))
        اونهائي++;
      وإلا
        {
          إذا (اونهائي)
            اونهائي--;
          وإلا
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              اذهب ايندڙ;
            }
        }

      إذا (آخري_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          إذا (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              إذا (errno == ELOOP)
                أرجع crun_محفوظ_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, عدم,
                                                        گهڻائي, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_رستو | O_NOFOLLOW, 0);
              إذا (ret < 0)
                أرجع crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          إذا (do_open)
            أرجع ret;

          close_and_replace (&wd_cleanup, ret);
          أرجع 0;
        }

      ret = mkdirat (cwd, cur, گهڻائي);
      إذا (ret < 0)
        {
          mode_t st_mode;

          إذا (errno != EEXIST)
            أرجع crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          إذا (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            أرجع crun_محفوظ_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : عدم, گهڻائي, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = محفوظ_openat (dirfd, dirpath, npath, O_رستو | O_CLOEXEC, 0, err);
      إذا (UNLIKELY (cwd < 0))
        أرجع crun_error_wrap (err, "creating `/%s`", رستو);

      إذا (! آخري_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, عدم);
          إذا (UNLIKELY (ret < 0))
            {
              صحيح saved_errno = errno;

              close (cwd);
              أرجع crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          إذا ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              أرجع crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", رستو, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    ايندڙ:
      إذا (it == عدم)
        اكسر;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  إذا (do_open)
    {
      إذا (cwd == dirfd)
        {
          ret = dup (dirfd);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "dup `%s`", dirpath);
          أرجع ret;
        }

      wd_cleanup = -1;
      أرجع cwd;
    }

  أرجع 0;
}

صحيح
crun_محفوظ_create_and_open_ref_at (bool dir, صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *رستو, صحيح گهڻائي, libcrun_error_t *err)
{
  صحيح ret;

  /* If the file/dir already exists, just open it.  */
  ret = محفوظ_openat (dirfd, dirpath, رستو, O_رستو | O_CLOEXEC, 0, err);
  إذا (LIKELY (ret >= 0))
    أرجع ret;

  crun_error_release (err);
  أرجع crun_محفوظ_ensure_at (true, dir, true, dirfd, dirpath, رستو, گهڻائي, MAX_READLINKS, err);
}

صحيح
crun_محفوظ_ensure_directory_at (صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *رستو, صحيح گهڻائي,
                               libcrun_error_t *err)
{
  أرجع crun_محفوظ_ensure_at (false, true, false, dirfd, dirpath, رستو, گهڻائي, MAX_READLINKS, err);
}

صحيح
crun_محفوظ_ensure_file_at (صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *رستو, صحيح گهڻائي,
                          libcrun_error_t *err)
{
  أرجع crun_محفوظ_ensure_at (false, false, false, dirfd, dirpath, رستو, گهڻائي, MAX_READLINKS, err);
}

صحيح
crun_ensure_directory (ثابت حرف *رستو, صحيح گهڻائي, bool nofollow, libcrun_error_t *err)
{
  أرجع crun_ensure_directory_at (AT_FDCWD, رستو, گهڻائي, nofollow, err);
}

ساكن صحيح
get_file_سائيز (صحيح fd, off_t *سائيز)
{
  بنية stat st;
  صحيح ret;
#جيڪڏهن_وصف_ٿيل HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (fd, "", AT_خالي_جايون_رستو | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_سائيز, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;
      أرجع ret;
    }
  *سائيز = stx.stx_size;

  أرجع ret;

fallback:
#جيڪڏهن_پورو
  ret = fstat (fd, &st);
  *سائيز = st.st_size;
  أرجع ret;
}

صحيح
crun_dir_p_at (صحيح dirfd, ثابت حرف *رستو, bool nofollow, libcrun_error_t *err)
{
  mode_t گهڻائي;
  صحيح ret;

  ret = get_file_type_at (dirfd, &گهڻائي, nofollow, رستو);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "stat `%s`", رستو);

  أرجع S_ISDIR (گهڻائي) ? 1 : 0;
}

صحيح
crun_dir_p (ثابت حرف *رستو, bool nofollow, libcrun_error_t *err)
{
  أرجع crun_dir_p_at (AT_FDCWD, رستو, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
ساكن صحيح
check_running_اندريون_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free حرف *بفر = عدم;
  size_t len;
  صحيح ret;

  ret = read_all_file ("/proc/self/uid_map", &بفر, &len, err);
  إذا (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      إذا (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          أرجع 0;
        }
      أرجع ret;
    }

  أرجع لڙي_ڳولا (بفر, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
ساكن bool
running_اندريون_openvz ()
{
  إذا (access ("/proc/vz", F_OK) < 0)
    أرجع false;

  أرجع access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

صحيح
check_running_اندريون_user_namespace (libcrun_error_t *err)
{
  ساكن atomic_int لاڳيتو_اندريون_userns = -1;
  بنية stat st;
  صحيح ret;

  ret = لاڳيتو_اندريون_userns;
  إذا (ret >= 0)
    أرجع ret;

  ret = stat ("/proc/self/ns/user", &st);
  إذا (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      إذا (st.st_ino == PROC_USER_INIT_INO)
        {
          لاڳيتو_اندريون_userns = 0;
          أرجع لاڳيتو_اندريون_userns;
        }

      إذا (! running_اندريون_openvz ())
        {
          لاڳيتو_اندريون_userns = 1;
          أرجع لاڳيتو_اندريون_userns;
        }
    }
  وإلا إذا (UNLIKELY (errno != ENOENT))
    أرجع crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_اندريون_user_namespace_uid_map (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  لاڳيتو_اندريون_userns = ret;
  أرجع لاڳيتو_اندريون_userns;
}

ساكن size_t
get_صفحو_سائيز ()
{
  ساكن atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  إذا (pagesize == 0)
    {
      pagesize = (ڊگهو) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  أرجع (size_t) pagesize;
}

ساكن صحيح selinux_enabled = -1;
ساكن صحيح apparmor_enabled = -1;

صحيح
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free حرف *ٻاهريون = عدم;
  cleanup_close صحيح fd = -1;
  size_t len;
  صحيح ret;

  إذا (selinux_enabled >= 0)
    أرجع selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = read_all_fd_with_سائيز_hint (fd, "mounts", &ٻاهريون, &len, get_صفحو_سائيز (), err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  selinux_enabled = لڙي_ڳولا (ٻاهريون, "selinux") ? 1 : 0;

  أرجع selinux_enabled;
}

صحيح
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  صحيح سائيز;
  حرف buf[1];

  إذا (apparmor_enabled >= 0)
    أرجع apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  إذا (fd == -1)
    {
      إذا (errno == ENOENT)
        {
          apparmor_enabled = 0;
          أرجع 0;
        }

      أرجع crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  سائيز = عارضي_FAILURE_RETRY (اقرأ (fd, buf, حجم (buf)));

  apparmor_enabled = سائيز > 0 && buf[0] == 'Y' ? 1 : 0;

  أرجع apparmor_enabled;
}

ساكن صحيح
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  إذا (selinux_enabled < 0)
    أرجع crun_make_error (err, 0, "SELinux is not initialized correctly");
  أرجع selinux_enabled;
}

صحيح
جوڙيو_selinux_mount_label (حرف **retlabel, ثابت حرف *معلومات, ثابت حرف *label, ثابت حرف *context_type, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_is_selinux_enabled (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (label && ret)
    {
      إذا (معلومات && *معلومات)
        xasprintf (retlabel, "%s,%s=\"%s\"", معلومات, context_type, label);
      وإلا
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      أرجع 0;
    }
  *retlabel = xstrdup (معلومات);
  أرجع 0;
}

ساكن ثابت حرف *
lsm_attr_رستو (libcrun_container_t *container, ثابت حرف *lsm, ثابت حرف *fname, libcrun_error_t *err)
{
  cleanup_close صحيح attr_dirfd = -1;
  cleanup_close صحيح lsm_dirfd = -1;
  حرف *attr_رستو = عدم;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_رستو, err);
  إذا (UNLIKELY (attr_dirfd < 0))
    أرجع عدم;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  إذا (lsm != عدم)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_رستو | O_CLOEXEC);

      إذا (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          أرجع عدم;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_رستو, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  أرجع attr_رستو;
}

ساكن صحيح
check_proc_super_magic (صحيح fd, ثابت حرف *رستو, libcrun_error_t *err)
{
  بنية statfs sfs;

  صحيح ret = fstatfs (fd, &sfs);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "statfs `%s`", رستو);

  إذا (sfs.f_type != PROC_SUPER_MAGIC)
    أرجع crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", رستو);

  أرجع 0;
}

ساكن صحيح
سيٽ_security_attr (libcrun_container_t *container, ثابت حرف *lsm, ثابت حرف *fname, ثابت حرف *معلومات, libcrun_error_t *err)
{
  صحيح ret;

  cleanup_free ثابت حرف *attr_رستو = lsm_attr_رستو (container, lsm, fname, err);
  cleanup_close صحيح fd = -1;

  إذا (UNLIKELY (attr_رستو == عدم))
    أرجع -1;

  fd = libcrun_open_proc_file (container, attr_رستو, O_WRONLY | O_CLOEXEC, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = check_proc_super_magic (fd, attr_رستو, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  // Write out data
  ret = عارضي_FAILURE_RETRY (اكتب (fd, معلومات, لڙي_ڊيگهه (معلومات)));
  إذا (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      إذا (errno == ENOENT && lsm != عدم && strcmp (lsm, "apparmor") == 0)
        {
          ثابت حرف *profile = strchr (معلومات, ' ');

          أرجع crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : معلومات);
        }
      أرجع crun_make_error (err, errno, "write to file `%s`", attr_رستو);
    }

  أرجع 0;
}

صحيح
سيٽ_selinux_label (libcrun_container_t *container, ثابت حرف *label, bool now, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_is_selinux_enabled (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret)
    أرجع سيٽ_security_attr (container, عدم, now ? "current" : "exec", label, err);
  أرجع 0;
}

ساكن صحيح
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  إذا (apparmor_enabled < 0)
    أرجع crun_make_error (err, 0, "AppArmor is not initialized correctly");
  أرجع apparmor_enabled;
}

ساكن صحيح
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#وصف_ڪريو UNCONFINED "unconfined"
#وصف_ڪريو UNCONFINED_LEN (ssize_t) (حجم (UNCONFINED) - 1)
  cleanup_free ثابت حرف *attr_رستو = lsm_attr_رستو (container, "apparmor", "current", err);
  cleanup_close صحيح fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  حرف buf[UNCONFINED_LEN + 1];

  إذا (UNLIKELY (attr_رستو == عدم))
    أرجع -1;

  fd = libcrun_open_proc_file (container, attr_رستو, O_RDONLY | O_CLOEXEC, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  إذا (UNLIKELY (check_proc_super_magic (fd, attr_رستو, err)))
    أرجع -1;

  ssize_t بائيٽون_read = عارضي_FAILURE_RETRY (اقرأ (fd, buf, حجم (buf)));
  إذا (UNLIKELY (بائيٽون_read < 0))
    أرجع crun_make_error (err, errno, "read from `%s`", attr_رستو);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  إذا (بائيٽون_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    أرجع 1;
  إذا (بائيٽون_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    أرجع 0;
  أرجع 1;
#وصف_رد UNCONFINED
#وصف_رد UNCONFINED_LEN
}

صحيح
سيٽ_apparmor_profile (libcrun_container_t *container, ثابت حرف *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_is_apparmor_enabled (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret)
    {
      cleanup_free حرف *buf = عدم;
      ret = is_current_process_confined (container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      أرجع سيٽ_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  أرجع 0;
}

ساكن صحيح
read_all_fd_اندروني (صحيح fd, ثابت حرف *description, حرف **ٻاهريون, size_t *len, size_t سائيز_hint, size_t max_سائيز, libcrun_error_t *err)
{
  cleanup_free حرف *buf = عدم;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t سائيز = 0;
  صحيح ret;

  إذا (سائيز_hint)
    allocated = سائيز_hint;
  وإلا
    {
      ret = get_file_سائيز (fd, &سائيز);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "stat `%s`", description);

      إذا (max_سائيز && (size_t) سائيز > max_سائيز)
        أرجع crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_سائيز);

      allocated = سائيز == 0 ? 1023 : سائيز;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  طالما ((سائيز && nread < (size_t) سائيز) || سائيز == 0)
    {
      ret = عارضي_FAILURE_RETRY (اقرأ (fd, buf + nread, allocated - nread));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "read from file `%s`", description);

      إذا (ret == 0)
        اكسر;

      nread += ret;

      إذا (max_سائيز && nread > max_سائيز)
        أرجع crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_سائيز);

      إذا (nread == allocated)
        {
          إذا (سائيز)
            اكسر;

          إذا (pagesize == 0)
            pagesize = get_صفحو_سائيز ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  إذا (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      حرف *tmp = realloc (buf, nread + 1);
      إذا (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *ٻاهريون = buf;
  buf = عدم;
  إذا (len)
    *len = nread;
  أرجع 0;
}

صحيح
read_all_fd_with_سائيز_hint (صحيح fd, ثابت حرف *description, حرف **ٻاهريون, size_t *len, size_t سائيز_hint, libcrun_error_t *err)
{
  أرجع read_all_fd_اندروني (fd, description, ٻاهريون, len, سائيز_hint, 0, err);
}

صحيح
read_all_fd_with_max_سائيز (صحيح fd, ثابت حرف *description, حرف **ٻاهريون, size_t *len, size_t max_سائيز, libcrun_error_t *err)
{
  أرجع read_all_fd_اندروني (fd, description, ٻاهريون, len, 0, max_سائيز, err);
}

صحيح
read_all_file_at (صحيح dirfd, ثابت حرف *رستو, حرف **ٻاهريون, size_t *len, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;

  fd = عارضي_FAILURE_RETRY (openat (dirfd, رستو, O_RDONLY | O_CLOEXEC));
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", رستو);

  أرجع read_all_fd (fd, رستو, ٻاهريون, len, err);
}

صحيح
read_all_file (ثابت حرف *رستو, حرف **ٻاهريون, size_t *len, libcrun_error_t *err)
{
  إذا (strcmp (رستو, "-") == 0)
    رستو = "/dev/stdin";

  أرجع read_all_file_at (AT_FDCWD, رستو, ٻاهريون, len, err);
}

صحيح
get_realpath_منزل_file (صحيح dirfd, ثابت حرف *رستو_name, حرف **absolute_رستو, libcrun_error_t *err)
{
  cleanup_close صحيح targetfd = -1;

  targetfd = عارضي_FAILURE_RETRY (openat (dirfd, رستو_name, O_RDONLY | O_CLOEXEC));
  إذا (UNLIKELY (targetfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", رستو_name);
  وإلا
    {
      ssize_t len;
      proc_fd_رستو_t هدف_fd_رستو;

      get_proc_self_fd_رستو (هدف_fd_رستو, targetfd);
      len = محفوظ_readlinkat (AT_FDCWD, هدف_fd_رستو, absolute_رستو, 0, err);
      إذا (UNLIKELY (len < 0))
        أرجع crun_error_wrap (err, "error unable to provide absolute path to file `%s`", رستو_name);
    }

  أرجع 0;
}

صحيح
open_unix_domain_client_socket (ثابت حرف *رستو, صحيح dgram, libcrun_error_t *err)
{
  بنية sockaddr_un addr = {};
  صحيح ret;
  proc_fd_رستو_t name_buf;
  cleanup_close صحيح destfd = -1;
  cleanup_close صحيح fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", رستو);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_وهڪرو, 0);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "create UNIX socket");

  إذا (لڙي_ڊيگهه (رستو) >= حجم (addr.sun_رستو))
    {
      destfd = open (رستو, O_رستو | O_CLOEXEC);
      إذا (UNLIKELY (destfd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", رستو);

      get_proc_self_fd_رستو (name_buf, destfd);

      رستو = name_buf;
    }

  size_t رستو_len = لڙي_ڊيگهه (رستو);
  إذا (رستو_len >= حجم (addr.sun_رستو))
    أرجع crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", رستو);

  يادگيري_نقل (addr.sun_رستو, رستو, رستو_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (بنية sockaddr *) &addr, حجم (addr));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "connect socket to `%s`", رستو);

  ret = fd;
  fd = -1;

  أرجع ret;
}

صحيح
open_unix_domain_socket (ثابت حرف *رستو, صحيح dgram, libcrun_error_t *err)
{
  بنية sockaddr_un addr = {};
  proc_fd_رستو_t name_buf;
  صحيح ret;
  cleanup_close صحيح dirfd = -1;
  cleanup_close صحيح fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_وهڪرو, 0);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "create UNIX socket");

  إذا (لڙي_ڊيگهه (رستو) >= حجم (addr.sun_رستو))
    {
      cleanup_free حرف *dpath = xstrdup (رستو);
      cleanup_free حرف *bpath = xstrdup (رستو);
      ثابت حرف *والد_dir = dirname (dpath);
      ثابت حرف *بنياد = basename (bpath);
      صحيح n;

      dirfd = open (والد_dir, O_رستو | O_DIRECTORY | O_CLOEXEC);
      إذا (UNLIKELY (dirfd < 0))
        أرجع crun_make_error (err, errno, "open directory `%s`", والد_dir);

      get_proc_self_fd_رستو (name_buf, dirfd);

      n = snprintf (addr.sun_رستو, حجم (addr.sun_رستو), "%s/%s", name_buf, بنياد);
      إذا (n < 0 || (size_t) n >= حجم (addr.sun_رستو))
        أرجع crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", رستو);
    }
  وإلا
    {
      size_t رستو_len = لڙي_ڊيگهه (رستو);
      يادگيري_نقل (addr.sun_رستو, رستو, رستو_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (بنية sockaddr *) &addr, حجم (addr));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "bind socket to `%s`", رستو);

  إذا (! dgram)
    {
      ret = listen (fd, 1);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  أرجع ret;
}

صحيح
send_fd_منزل_socket (صحيح server, صحيح fd, libcrun_error_t *err)
{
  أرجع send_fd_منزل_socket_with_payload (server, fd, عدم, 0, err);
}

صحيح
send_fd_منزل_socket_with_payload (صحيح server, صحيح fd, ثابت حرف *payload, size_t payload_len, libcrun_error_t *err)
{
  صحيح ret;
  بنية cmsghdr *cmsg = عدم;
  بنية iovec iov[2];
  بنية msghdr نياپو = {};
  حرف ctrl_buf[CMSG_SPACE (1 + حجم (صحيح))] = {};
  حرف معلومات[1];

  معلومات[0] = ' ';
  iov[0].iov_base = معلومات;
  iov[0].iov_len = حجم (معلومات);

  إذا (payload_len > 0)
    {
      iov[0].iov_base = (فراغ *) payload;
      iov[0].iov_len = payload_len;
    }

  نياپو.نياپو_name = عدم;
  نياپو.نياپو_namelen = 0;
  نياپو.نياپو_iov = iov;
  نياپو.نياپو_iovlen = 1;
  نياپو.نياپو_controllen = CMSG_SPACE (حجم (صحيح));
  نياپو.نياپو_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&نياپو);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (حجم (صحيح));

  *((صحيح *) CMSG_معلومات (cmsg)) = fd;

  ret = عارضي_FAILURE_RETRY (sendmsg (server, &نياپو, 0));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "sendmsg");
  أرجع 0;
}

صحيح
receive_fd_from_socket_with_payload (صحيح from, حرف *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  صحيح ret;
  بنية iovec iov[1];
  بنية msghdr نياپو = {};
  حرف ctrl_buf[CMSG_SPACE (حجم (صحيح))] = {};
  حرف معلومات[1];
  بنية cmsghdr *cmsg;

  معلومات[0] = ' ';
  iov[0].iov_base = معلومات;
  iov[0].iov_len = حجم (معلومات);

  إذا (payload_len > 0)
    {
      iov[0].iov_base = (فراغ *) payload;
      iov[0].iov_len = payload_len;
    }

  نياپو.نياپو_name = عدم;
  نياپو.نياپو_namelen = 0;
  نياپو.نياپو_iov = iov;
  نياپو.نياپو_iovlen = 1;
  نياپو.نياپو_controllen = CMSG_SPACE (حجم (صحيح));
  نياپو.نياپو_control = ctrl_buf;

  ret = عارضي_FAILURE_RETRY (recvmsg (from, &نياپو, 0));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "recvmsg");
  إذا (UNLIKELY (ret == 0))
    أرجع crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&نياپو);
  إذا (cmsg == عدم)
    أرجع crun_make_error (err, 0, "no msg received");
  يادگيري_نقل (&fd, CMSG_معلومات (cmsg), حجم (fd));

  ret = fd;
  fd = -1;
  أرجع ret;
}

صحيح
receive_fd_from_socket (صحيح from, libcrun_error_t *err)
{
  أرجع receive_fd_from_socket_with_payload (from, عدم, 0, err);
}

صحيح
create_socket_pair (صحيح *pair, libcrun_error_t *err)
{
  صحيح ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "socketpair");
  أرجع 0;
}

صحيح
create_signalfd (sigset_t *نقاب, libcrun_error_t *err)
{
  صحيح ret = signalfd (-1, نقاب, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "signalfd");
  أرجع ret;
}

ساكن صحيح
epoll_helper_toggle (صحيح epollfd, صحيح fd, صحيح events, libcrun_error_t *err)
{
  بنية epoll_event ev = {};
  bool جوڙيو = events != 0;
  صحيح ret;

  ev.events = events;
  ev.معلومات.fd = fd;
  ret = epoll_ctl (epollfd, جوڙيو ? EPOLL_CTL_جوڙيو : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  إذا (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  إذا (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      إذا (errno == ENOENT || errno == EPERM)
        أرجع 0;
      أرجع crun_make_error (err, errno, "epoll_ctl `%s` `%d`", جوڙيو ? "add" : "del", fd);
    }
  أرجع 0;
}

صحيح
epoll_helper (صحيح *اندريون_fds, صحيح *اندريون_levelfds, صحيح *ٻاهريون_fds, صحيح *ٻاهريون_levelfds, libcrun_error_t *err)
{
  بنية epoll_event ev;
  cleanup_close صحيح epollfd = -1;
  صحيح ret;

  صحيح *it;
  epollfd = epoll_create1 (0);
  إذا (UNLIKELY (epollfd < 0))
    أرجع crun_make_error (err, errno, "epoll_create1");

#وصف_ڪريو جوڙيو_FDS(FDS, EVENTS)                                            \
  لكل (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.معلومات.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_جوڙيو, *it, &ev);               \
      إذا (UNLIKELY (ret < 0))                                           \
        أرجع crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  إذا (اندريون_fds)
    جوڙيو_FDS (اندريون_fds, EPOLLIN);
  إذا (اندريون_levelfds)
    جوڙيو_FDS (اندريون_levelfds, EPOLLIN | EPOLLET);
  إذا (ٻاهريون_fds)
    جوڙيو_FDS (ٻاهريون_fds, EPOLLOUT);
  إذا (ٻاهريون_levelfds)
    جوڙيو_FDS (ٻاهريون_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  أرجع ret;
}

صحيح
copy_from_fd_منزل_fd (صحيح src, صحيح dst, صحيح consume, libcrun_error_t *err)
{
  صحيح ret;
  ssize_t nread;
  size_t pagesize = get_صفحو_سائيز ();
#جيڪڏهن_وصف_ٿيل HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#جيڪڏهن_پورو
  افعل
    {
      cleanup_free حرف *بفر = عدم;
      ssize_t بچيل;

#جيڪڏهن_وصف_ٿيل HAVE_COPY_FILE_RANGE
      إذا (can_copy_file_range)
        {
          nread = copy_file_range (src, عدم, dst, عدم, pagesize, 0);
          إذا (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              اذهب fallback;
            }
          إذا (consume && nread < 0 && errno == EAGAIN)
            أرجع 0;
          إذا (nread < 0 && errno == EIO)
            أرجع 0;
          إذا (UNLIKELY (nread < 0))
            أرجع crun_make_error (err, errno, "copy_file_range");
          استمر;
        }
    fallback:
#جيڪڏهن_پورو

      بفر = xmalloc (pagesize);
      nread = عارضي_FAILURE_RETRY (اقرأ (src, بفر, pagesize));
      إذا (consume && nread < 0 && errno == EAGAIN)
        أرجع 0;
      إذا (nread < 0 && errno == EIO)
        أرجع 0;
      إذا (UNLIKELY (nread < 0))
        أرجع crun_make_error (err, errno, "read");

      بچيل = nread;
      طالما (بچيل)
        {
          ret = عارضي_FAILURE_RETRY (اكتب (dst, بفر + nread - بچيل, بچيل));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "write");
          بچيل -= ret;
        }
  } طالما (consume && nread);

  أرجع 0;
}

صحيح
لاڳيتو_process (حرف **دليلون, libcrun_error_t *err)
{
  pid_t pid = fork ();
  إذا (UNLIKELY (pid < 0))
    أرجع crun_make_error (err, errno, "fork");
  إذا (pid)
    {
      صحيح r, حالت;
      r = عارضي_FAILURE_RETRY (waitpid (pid, &حالت, 0));
      إذا (r < 0)
        أرجع crun_make_error (err, errno, "waitpid");
      إذا (WIFEXITED (حالت) || WIFSIGNALED (حالت))
        أرجع WEXITSTATUS (حالت);
    }

  execvp (دليلون[0], دليلون);
  محفوظ_exit (EXIT_FAILURE);
}

#جيڪڏهن_وصف_نه_ٿيل HAVE_FGETPWENT_R
ساكن بنانشان
atou (حرف **s)
{
  بنانشان x;
  لكل (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  أرجع x;
}

صحيح
fgetpwent_r (FILE *f, بنية passwd *pw, حرف *سٽ, size_t سائيز, بنية passwd **res)
{
  حرف *s;
  صحيح rv = 0;
  لكل (;;)
    {
      سٽ[سائيز - 1] = '\xff';
      إذا ((fgets (سٽ, سائيز, f) == عدم) || ferror (f) || سٽ[سائيز - 1] != '\xff')
        {
          rv = (سٽ[سائيز - 1] != '\xff') ? ERANGE : ENOENT;
          سٽ = 0;
          pw = 0;
          اكسر;
        }
      سٽ[strcspn (سٽ, "\n")] = 0;

      s = سٽ;
      pw->pw_name = s++;
      إذا (! (s = strchr (s, ':')))
        استمر;

      *s++ = 0;
      pw->pw_passwd = s;
      إذا (! (s = strchr (s, ':')))
        استمر;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      إذا (*s != ':')
        استمر;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      إذا (*s != ':')
        استمر;

      *s++ = 0;
      pw->pw_gecos = s;
      إذا (! (s = strchr (s, ':')))
        استمر;

      *s++ = 0;
      pw->pw_dir = s;
      إذا (! (s = strchr (s, ':')))
        استمر;

      *s++ = 0;
      pw->pw_shell = s;
      اكسر;
    }
  *res = pw;
  إذا (rv)
    errno = rv;
  أرجع rv;
}
#جيڪڏهن_پورو

صحيح
سيٽ_home_env (uid_t id)
{
  بنية passwd pwd;
  cleanup_free حرف *buf = عدم;
  ڊگهو buf_سائيز;
  cleanup_file FILE *وهڪرو = عدم;

  buf_سائيز = sysconf (_SC_GETPW_R_SIZE_MAX);
  إذا (buf_سائيز < 0)
    buf_سائيز = 1024;

  buf = xmalloc (buf_سائيز);

  وهڪرو = fopen ("/etc/passwd", "re");
  إذا (وهڪرو == عدم)
    أرجع -1;

  لكل (;;)
    {
      بنية passwd *ret_pw = عدم;

      صحيح ret = fgetpwent_r (وهڪرو, &pwd, buf, buf_سائيز, &ret_pw);
      إذا (UNLIKELY (ret != 0))
        {
          إذا (ret == EINTR)
            استمر;

          إذا (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              أرجع -1;
            }

          buf_سائيز *= 2;
          buf = xrealloc (buf, buf_سائيز);
          استمر;
        }

      إذا (ret_pw && ret_pw->pw_uid == id)
        {
          إذا (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          وإلا
            أرجع 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
ساكن صحيح
getsubidrange (uid_t id, صحيح is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *داخلا = عدم;
  cleanup_free حرف *lineptr = عدم;
  size_t lenlineptr = 0, len_name;
  ڊگهو buf_سائيز;
  cleanup_free حرف *buf = عدم;
  ثابت حرف *name;
  بنية passwd pwd;

  buf_سائيز = sysconf (_SC_GETPW_R_SIZE_MAX);
  إذا (buf_سائيز < 0)
    buf_سائيز = 1024;

  buf = xmalloc (buf_سائيز);
  لكل (;;)
    {
      صحيح ret;
      بنية passwd *ret_pw = عدم;

      ret = getpwuid_r (id, &pwd, buf, buf_سائيز, &ret_pw);
      إذا (LIKELY (ret == 0))
        {
          إذا (ret_pw)
            {
              name = ret_pw->pw_name;
              اكسر;
            }
          أرجع -1;
        }

      إذا (ret != ERANGE)
        أرجع -1;

      buf_سائيز *= 2;
      buf = xrealloc (buf, buf_سائيز);
    }

  len_name = لڙي_ڊيگهه (name);

  داخلا = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  إذا (داخلا == عدم)
    أرجع -1;

  لكل (;;)
    {
      حرف *endptr;
      ssize_t اقرأ = getline (&lineptr, &lenlineptr, داخلا);
      إذا (اقرأ < 0)
        أرجع -1;

      إذا (اقرأ < (ssize_t) (len_name + 2))
        استمر;

      إذا (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        استمر;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      إذا (endptr >= &lineptr[اقرأ])
        أرجع -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      أرجع 0;
    }
}

#وصف_ڪريو MIN(x, y) ((x) < (y) ? (x) : (y))

صحيح
format_default_id_mapping (حرف **ٻاهريون, uid_t container_id, uid_t host_uid, uid_t host_id, صحيح is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, موجود = 0;
  cleanup_free حرف *بفر = عدم;
  صحيح written = 0;
  صحيح ret, بچيل;

  *ٻاهريون = عدم;

  إذا (getsubidrange (host_uid, is_uid, &from, &موجود) < 0)
    أرجع 0;

  /* More than enough space for all the mappings.  */
  بچيل = 15 * 5 * 3;
  بفر = xmalloc (بچيل + 1);

  إذا (container_id > 0)
    {
      uint32_t استعمال_ٿيل = MIN (container_id, موجود);
      ret = snprintf (بفر + written, بچيل, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, استعمال_ٿيل);
      إذا (UNLIKELY (ret >= بچيل))
        أرجع crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      بچيل -= ret;

      from += استعمال_ٿيل;
      موجود -= استعمال_ٿيل;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (بفر + written, بچيل, "%d %d 1\n", container_id, host_id);
  إذا (UNLIKELY (ret >= بچيل))
    أرجع crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  بچيل -= ret;

  /* Last mapping: use any id that is left.  */
  إذا (موجود)
    {
      ret = snprintf (بفر + written, بچيل, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, موجود);
      إذا (UNLIKELY (ret >= بچيل))
        أرجع crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      بچيل -= ret;
    }

  *ٻاهريون = بفر;
  بفر = عدم;
  أرجع written;
}

ساكن صحيح
unset_cloexec_flag (صحيح fd)
{
  صحيح flags = fcntl (fd, F_GETFD);
  إذا (flags == -1)
    أرجع -1;

  flags &= ~FD_CLOEXEC;

  أرجع fcntl (fd, F_SETFD, flags);
}

ساكن فراغ __attribute__ ((__noreturn__))
لاڳيتو_process_ٻار (حرف *رستو, حرف **دليلون, ثابت حرف *cwd, حرف **envp, صحيح pipe_r,
                   صحيح pipe_w, صحيح ٻاهريون_fd, صحيح err_fd, bool can_ignore_chdir_errors)
{
  حرف *tmp_دليلون[] = { رستو, عدم };
  libcrun_error_t err = عدم;
  صحيح dev_null_fd = -1;
  صحيح ret;

  ret = mark_or_close_fds_ge_than (عدم, 3, false, &err);
  إذا (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->حالت, "%s", (err)->نياپو);

  إذا (ٻاهريون_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      إذا (UNLIKELY (dev_null_fd < 0))
        محفوظ_exit (EXIT_FAILURE);
    }

  عارضي_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  عارضي_FAILURE_RETRY (close (pipe_r));

  dup2 (ٻاهريون_fd >= 0 ? ٻاهريون_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  إذا (ٻاهريون_fd >= 0)
    unset_cloexec_flag (1);
  إذا (err_fd >= 0)
    unset_cloexec_flag (2);

  إذا (dev_null_fd >= 0)
    عارضي_FAILURE_RETRY (close (dev_null_fd));
  إذا (ٻاهريون_fd >= 0)
    عارضي_FAILURE_RETRY (close (ٻاهريون_fd));
  إذا (err_fd >= 0)
    عارضي_FAILURE_RETRY (close (err_fd));

  إذا (دليلون == عدم)
    دليلون = tmp_دليلون;

  إذا (cwd && chdir (cwd) < 0)
    {
      إذا (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        محفوظ_exit (EXIT_FAILURE);
    }

  execvpe (رستو, دليلون, envp);
  محفوظ_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
صحيح
لاڳيتو_process_with_stdin_timeout_envp (حرف *رستو, حرف **دليلون, ثابت حرف *cwd, صحيح timeout,
                                     حرف **envp, حرف *stdin, size_t stdin_len, صحيح ٻاهريون_fd,
                                     صحيح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  صحيح stdin_pipe[2];
  pid_t pid;
  صحيح ret;
  cleanup_close صحيح pipe_r = -1;
  cleanup_close صحيح pipe_w = -1;
  sigset_t oldmask, نقاب;
  صحيح r, حالت;

  sigemptyset (&نقاب);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  إذا (timeout > 0)
    {
      sigaddset (&نقاب, SIGCHLD);
      ret = sigprocmask (SIG_بلاڪ, &نقاب, &oldmask);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  إذا (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      اذهب restore_sig_نقاب_and_exit;
    }

  إذا (pid == 0)
    {
      /* run_process_child doesn't return.  */
      لاڳيتو_process_ٻار (رستو, دليلون, cwd, envp, pipe_r, pipe_w, ٻاهريون_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = عارضي_FAILURE_RETRY (اكتب (pipe_w, stdin, stdin_len));
  إذا (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      إذا (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          اذهب restore_sig_نقاب_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  إذا (timeout)
    {
      time_t شروعات = وقت (عدم);
      time_t now;
      لكل (now = شروعات; now - شروعات < timeout; now = وقت (عدم))
        {
          siginfo_t info;
          صحيح elapsed = now - شروعات;
          بنية timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&نقاب, &info, &ts_timeout);
          إذا (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              اذهب restore_sig_نقاب_and_exit;
            }

          إذا (info.si_signo == SIGCHLD && info.si_pid == pid)
            اذهب read_waitpid;

          إذا (ret < 0 && errno == EAGAIN)
            اذهب timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", رستو);
      اذهب restore_sig_نقاب_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &حالت, 0);
  إذا (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  وإلا
    ret = get_process_exit_حالت (حالت);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_نقاب_and_exit:
  إذا (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      إذا (pid > 0)
        {
          kill (pid, SIGKILL);
          عارضي_FAILURE_RETRY (waitpid (pid, &حالت, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, عدم);
      إذا (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  أرجع ret;
}

صحيح
mark_or_close_fds_ge_than (libcrun_container_t *container, صحيح n, bool close_now, libcrun_error_t *err)
{
  cleanup_close صحيح cfd = -1;
  cleanup_dir DIR *dir = عدم;
  صحيح ret;
  صحيح fd;
  بنية dirent *ايندڙ;

#جيڪڏهن_وصف_ٿيل ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  إذا (close_now)
    GCOV_DUMP ();
#جيڪڏهن_پورو

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  إذا (ret == 0)
    {
      إذا (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      أرجع 0;
    }
  إذا (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    أرجع crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  إذا (UNLIKELY (cfd < 0))
    أرجع cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dir = fdopendir (cfd);
  إذا (UNLIKELY (dir == عدم))
    أرجع crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  لكل (ايندڙ = readdir (dir); ايندڙ; ايندڙ = readdir (dir))
    {
      صحيح val;
      ثابت حرف *name = ايندڙ->d_name;
      إذا (name[0] == '.')
        استمر;

      val = strtoll (name, عدم, 10);
      إذا (val < n || val == fd)
        استمر;

      إذا (close_now)
        {
          ret = close (val);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "close fd `%d`", val);

          إذا (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      وإلا
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  أرجع 0;
}

فراغ
get_current_timestamp (حرف *ٻاهريون, size_t len)
{
  بنية timeval tv;
  بنية tm now;
  حرف timestamp[64];

  gettimeofday (&tv, عدم);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, حجم (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (فراغ) snprintf (ٻاهريون, len, "%s.%06lldZ", timestamp, (ڊگهو ڊگهو صحيح) tv.tv_usec);
  ٻاهريون[len - 1] = '\0';
}

صحيح
سيٽ_blocking_fd (صحيح fd, bool blocking, libcrun_error_t *err)
{
  صحيح ret, flags = fcntl (fd, F_GETFL, 0);
  إذا (UNLIKELY (flags < 0))
    أرجع crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fcntl");
  أرجع 0;
}

صحيح
parse_json_file (json_object **ٻاهريون, ثابت حرف *jsondata, بنية parser_context *ctx arg_اڻ_استعمال_ٿيل, libcrun_error_t *err)
{
  بنية json_tokener *tok;
  تعداد json_tokener_error jerr;

  *err = عدم;

  إذا (UNLIKELY (jsondata == عدم))
    أرجع crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  إذا (UNLIKELY (tok == عدم))
    أرجع crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_سيٽ_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *ٻاهريون = json_tokener_parse_ex (tok, jsondata, (صحيح) لڙي_ڊيگهه (jsondata));
  jerr = json_tokener_get_error (tok);
  إذا (*ٻاهريون == عدم || jerr != json_tokener_success)
    {
      صحيح saved_pos = (صحيح) tok->char_وٿي;

      json_tokener_free (tok);
      إذا (*ٻاهريون)
        {
          json_object_put (*ٻاهريون);
          *ٻاهريون = عدم;
        }
      أرجع crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  أرجع 0;
}

#وصف_ڪريو CHECK_ACCESS_NOT_EXECUTABLE 1
#وصف_ڪريو CHECK_ACCESS_NOT_REGULAR 2

/* check that the specified path exists and it is executable.
   Return
   - 0 if the file is executable
   - CHECK_ACCESS_NOT_EXECUTABLE if it exists but it is not executable
   - CHECK_ACCESS_NOT_REGULAR if it not a regular file
   - -errno for any other generic error
 */
ساكن صحيح
check_access (ثابت حرف *رستو)
{
  صحيح ret;
  mode_t گهڻائي;

#جيڪڏهن_وصف_ٿيل HAVE_EACCESS
#  define ACCESS(رستو, گهڻائي) (eaccess (رستو, گهڻائي))
#وإلا
#  define ACCESS(رستو, گهڻائي) (access (رستو, گهڻائي))
#جيڪڏهن_پورو

  ret = ACCESS (رستو, X_OK);
  إذا (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      إذا (errno == EACCES)
        {
          صحيح saved_errno = errno;
          ret = ACCESS (رستو, F_OK);
          errno = saved_errno;

          إذا (ret == 0)
            أرجع CHECK_ACCESS_NOT_EXECUTABLE;
        }
      أرجع -errno;
    }

  ret = get_file_type (&گهڻائي, false, رستو);
  إذا (UNLIKELY (ret < 0))
    أرجع -errno;

  إذا (! S_ISREG (گهڻائي))
    أرجع CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  أرجع 0;
}

صحيح
ڳوليو_executable (حرف **ٻاهريون, ثابت حرف *executable_رستو, ثابت حرف *cwd, libcrun_error_t *err)
{
  cleanup_free حرف *cwd_executable_رستو = عدم;
  cleanup_free حرف *tmp = عدم;
  صحيح آخري_error = -ENOENT;
  حرف *it, *پڇاڙي;
  صحيح ret;

  *ٻاهريون = عدم;

  إذا (executable_رستو == عدم || executable_رستو[0] == '\0')
    أرجع crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  إذا (executable_رستو[0] != '/' && strchr (executable_رستو, '/'))
    {
      cleanup_free حرف *cwd_allocated = عدم;

      إذا (cwd == عدم)
        {
          cwd_allocated = getcwd (عدم, 0);
          إذا (cwd_allocated == عدم)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_رستو, err, "/", cwd, executable_رستو, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      executable_رستو = cwd_executable_رستو;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  إذا (executable_رستو[0] == '/')
    {
      ret = check_access (executable_رستو);
      إذا (LIKELY (ret == 0))
        {
          *ٻاهريون = xstrdup (executable_رستو);
          أرجع 0;
        }
      آخري_error = ret;
      اذهب fail;
    }

  پڇاڙي = tmp = xstrdup (getenv ("PATH"));

  طالما ((it = strsep (&پڇاڙي, ":")))
    {
      cleanup_free حرف *رستو = عدم;

      إذا (it == پڇاڙي)
        it = ".";

      ret = append_paths (&رستو, err, it, executable_رستو, عدم);
      إذا (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          استمر;
        }

      ret = check_access (رستو);
      إذا (ret == 0)
        {
          /* Change owner.  */
          *ٻاهريون = رستو;
          رستو = عدم;
          أرجع 0;
        }

      إذا (ret == -ENOENT)
        استمر;

      آخري_error = ret;
    }

fail:
  بدل (آخري_error)
    {
    حالة CHECK_ACCESS_NOT_EXECUTABLE:
      أرجع crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_رستو);

    حالة CHECK_ACCESS_NOT_REGULAR:
      أرجع crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_رستو);

    افتراضي:
      errno = -آخري_error;
      إذا (errno == ENOENT)
        أرجع crun_make_error (err, errno, "executable file `%s` not found%s", executable_رستو, executable_رستو[0] == '/' ? "" : " in $PATH");
      أرجع crun_make_error (err, errno, "open `%s`", executable_رستو);
    }
}

#جيڪڏهن_وصف_ٿيل HAVE_FGETXATTR

ساكن ssize_t
محفوظ_read_xattr (حرف **ret, صحيح sfd, ثابت حرف *srcname, ثابت حرف *name, size_t initial_سائيز, libcrun_error_t *err)
{
  cleanup_free حرف *بفر = عدم;
  ssize_t current_سائيز;
  ssize_t s;

  current_سائيز = (ssize_t) initial_سائيز;
  بفر = xmalloc (current_سائيز + 1);

  طالما (1)
    {
      s = fgetxattr (sfd, name, بفر, current_سائيز);
      إذا (UNLIKELY (s < 0))
        أرجع crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      إذا (s < current_سائيز)
        اكسر;

      current_سائيز *= 2;
      بفر = xrealloc (بفر, current_سائيز + 1);
    }

  إذا (s <= 0)
    أرجع s;

  بفر[s] = '\0';

  /* Change owner.  */
  *ret = بفر;
  بفر = عدم;

  أرجع s;
}

ساكن ssize_t
copy_xattr (صحيح sfd, صحيح dfd, ثابت حرف *srcname, ثابت حرف *destname, libcrun_error_t *err)
{
  cleanup_free حرف *buf = عدم;
  ssize_t xattr_len;
  حرف *it;

  xattr_len = flistxattr (sfd, عدم, 0);
  إذا (UNLIKELY (xattr_len < 0))
    {
      إذا (errno == ENOTSUP)
        أرجع 0;

      أرجع crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  إذا (xattr_len == 0)
    أرجع 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  إذا (UNLIKELY (xattr_len < 0))
    أرجع crun_make_error (err, errno, "flistxattr `%s`", srcname);

  لكل (it = buf; it - buf < xattr_len; it += لڙي_ڊيگهه (it) + 1)
    {
      cleanup_free حرف *v = عدم;
      ssize_t s;

      s = محفوظ_read_xattr (&v, sfd, srcname, it, 256, err);
      إذا (UNLIKELY (s < 0))
        أرجع s;

      s = fsetxattr (dfd, it, v, s, 0);
      إذا (UNLIKELY (s < 0))
        {
          إذا (errno == EINVAL || errno == EOPNOTSUPP)
            استمر;

          أرجع crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  أرجع 0;
}

#جيڪڏهن_پورو

ساكن صحيح
copy_rec_stat_file_at (صحيح dfd, ثابت حرف *رستو, mode_t *گهڻائي, off_t *سائيز, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  بنية stat st;
  صحيح ret;

#جيڪڏهن_وصف_ٿيل HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (dfd, رستو, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_گهڻائي | STATX_سائيز | STATX_UID | STATX_GID, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;

      أرجع ret;
    }

  *گهڻائي = stx.stx_mode;
  *سائيز = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  أرجع ret;

fallback:
#جيڪڏهن_پورو
  ret = fstatat (dfd, رستو, &st, AT_SYMLINK_NOFOLLOW);

  *گهڻائي = st.st_mode;
  *سائيز = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  أرجع ret;
}

صحيح
copy_recursive_fd_منزل_fd (صحيح srcdirfd, صحيح dfd, ثابت حرف *srcname, ثابت حرف *destname, libcrun_error_t *err)
{
  cleanup_close صحيح destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = عدم;
  بنية dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  إذا (UNLIKELY (dsrcfd == عدم))
    {
      عارضي_FAILURE_RETRY (close (srcdirfd));
      أرجع crun_make_error (err, errno, "open directory `%s`", destname);
    }

  لكل (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close صحيح srcfd = -1;
      cleanup_close صحيح destfd = -1;
      cleanup_free حرف *هدف_buf = عدم;
      صحيح ret;
      mode_t گهڻائي;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      إذا (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        استمر;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &گهڻائي, &st_size, &rdev, &uid, &gid);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      بدل (گهڻائي & S_IFMT)
        {
        حالة S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          إذا (UNLIKELY (srcfd < 0))
            أرجع crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          إذا (UNLIKELY (destfd < 0))
            أرجع crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_منزل_fd (srcfd, destfd, 1, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

#جيڪڏهن_وصف_ٿيل HAVE_FGETXATTR
          ret = (صحيح) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
#جيڪڏهن_پورو

          عارضي_FAILURE_RETRY (close (destfd));
          destfd = -1;
          اكسر;

        حالة S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, گهڻائي);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          إذا (UNLIKELY (srcfd < 0))
            أرجع crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          إذا (UNLIKELY (destfd < 0))
            أرجع crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#جيڪڏهن_وصف_ٿيل HAVE_FGETXATTR
          ret = (صحيح) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
#جيڪڏهن_پورو

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_منزل_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          اكسر;

        حالة S_IFLNK:
          ret = محفوظ_readlinkat (dirfd (dsrcfd), de->d_name, &هدف_buf, st_size, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          ret = symlinkat (هدف_buf, destdirfd, de->d_name);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          اكسر;

        حالة S_IFBLK:
        حالة S_IFCHR:
        حالة S_IFIFO:
        حالة S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, گهڻائي, rdev);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          اكسر;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#جيڪڏهن_وصف_نه_ٿيل ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#جيڪڏهن_پورو

      ret = fchmodat (destdirfd, de->d_name, گهڻائي & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      إذا (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          إذا (errno == ENOTSUP)
            استمر;

          أرجع crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  أرجع 0;
}

ثابت حرف *
ڳوليو_annotation (libcrun_container_t *container, ثابت حرف *name)
{
  إذا (container->container_def->annotations == عدم)
    أرجع عدم;

  أرجع ڳوليو_string_map_قدر (container->annotations, name);
}

صحيح
محفوظ_write (صحيح fd, ثابت حرف *fname, ثابت فراغ *buf, size_t ڳڻپ, libcrun_error_t *err)
{
  size_t written = 0;
  طالما (written < ڳڻپ)
    {
      ssize_t w = اكتب (fd, (ثابت حرف *) buf + written, ڳڻپ - written);
      إذا (UNLIKELY (w < 0))
        {
          إذا (errno == EINTR || errno == EAGAIN)
            استمر;
          أرجع crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  أرجع 0;
}

#وصف_ڪريو MAX_PARTS 32

صحيح
append_paths (حرف **ٻاهريون, libcrun_error_t *err, ...)
{
  ثابت حرف *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t ڪل_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_شروعات (ap, err);
  لكل (;;)
    {
      ثابت حرف *part;
      size_t سائيز;

      part = va_arg (ap, ثابت حرف *);
      إذا (part == عدم)
        اكسر;

      إذا (n_parts == MAX_PARTS)
        {
          va_پڇاڙي (ap);
          أرجع crun_make_error (err, EINVAL, "too many paths specified");
        }

      إذا (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          طالما (part[0] == '/' && part[1] == '/')
            part++;
        }
      وإلا
        {
          /* And drop any initial '/' for other components.  */
          طالما (part[0] == '/')
            part++;
        }

      سائيز = لڙي_ڊيگهه (part);
      إذا (سائيز == 0)
        استمر;

      طالما (سائيز > 1 && part[سائيز - 1] == '/')
        سائيز--;

      parts[n_parts] = part;
      sizes[n_parts] = سائيز;

      n_parts++;
    }
  va_پڇاڙي (ap);

  ڪل_len = n_parts + 1;
  لكل (i = 0; i < n_parts; i++)
    ڪل_len += sizes[i];

  *ٻاهريون = xmalloc (ڪل_len);

  copied = 0;
  لكل (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*ٻاهريون)[copied - 1] == '/';
      إذا (i > 0 && ! has_trailing_slash)
        {
          (*ٻاهريون)[copied] = '/';
          copied += 1;
        }

      يادگيري_نقل (*ٻاهريون + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*ٻاهريون)[copied] = '\0';
  أرجع 0;
}

#إذا __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#وإلا
#  define __nonstring
#جيڪڏهن_پورو

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
ساكن صحيح
b64_داخلا (حرف c)
{
  ثابت حرف جدول[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  صحيح i;

  لكل (i = 0; i < 64; i++)
    {
      إذا (جدول[i] == c)
        أرجع i;
    }
  أرجع -1;
}

صحيح
base64_decode (ثابت حرف *iptr, size_t isize, حرف *optr, size_t osize, size_t *nbytes)
{
  صحيح i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  بنانشان حرف معلومات[4];

  *nbytes = 0;
  طالما (consumed < isize && (*nbytes) + 3 < osize)
    {
      طالما ((i < 4) && (consumed < isize))
        {
          tmp = b64_داخلا (*iptr++);
          consumed++;
          إذا (tmp != -1)
            {
              إذا (UNLIKELY (pad > 0))
                أرجع -1;
              معلومات[i++] = tmp;
            }
          وإلا إذا (*(iptr - 1) == '=')
            {
              معلومات[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      إذا (i == 4)
        {
          إذا (UNLIKELY (pad > 2))
            أرجع -1;
          *optr++ = (معلومات[0] << 2) | ((معلومات[1] & 0x30) >> 4);
          *optr++ = ((معلومات[1] & 0xf) << 4) | ((معلومات[2] & 0x3c) >> 2);
          *optr++ = ((معلومات[2] & 0x3) << 6) | معلومات[3];
          (*nbytes) += 3 - pad;
        }
      وإلا
        {
          /* I did not get all the data.  */
          consumed -= i;
          أرجع consumed;
        }
      i = 0;
    }
  أرجع consumed;
}

حرف *
get_user_name (uid_t uid)
{
  بنية passwd pd;
  بنية passwd *عارضي_نتيجو_ptr;
  حرف pwdbuffer[200];
  إذا (! getpwuid_r (uid, &pd, pwdbuffer, حجم (pwdbuffer), &عارضي_نتيجو_ptr))
    أرجع xstrdup (pd.pw_name);
  أرجع xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
حرف *
libcrun_sysctl_ڪنجي_منزل_proc_رستو (ثابت حرف *ڪنجي)
{
  ثابت حرف *sep;
  حرف *ret, *it;

  sep = strpbrk (ڪنجي, "./");

  /* No separator at all, or the key is already in the path form.  */
  إذا (sep == عدم || *sep == '/')
    أرجع xstrdup (ڪنجي);

  ret = xstrdup (ڪنجي);
  لكل (it = ret; *it; it++)
    {
      إذا (*it == '.')
        *it = '/';
      وإلا إذا (*it == '/')
        *it = '.';
    }

  أرجع ret;
}

صحيح
has_suffix (ثابت حرف *str, ثابت حرف *suffix)
{
  إذا (! str || ! suffix)
    أرجع 0;
  size_t lenstr = لڙي_ڊيگهه (str);
  size_t lensuffix = لڙي_ڊيگهه (suffix);
  إذا (lensuffix > lenstr)
    أرجع 0;
  أرجع memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

حرف *
str_join_array (صحيح وٿي, size_t سائيز, حرف *ثابت array[], ثابت حرف *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = عدم;
  size_t i, ڪل_سائيز = (سائيز - 1) * (jlen = لڙي_ڊيگهه (joint)) + 1;
  حرف *نتيجو, *p;

  lens = xmalloc (سائيز * حجم (*lens));

  لكل (i = 0; i < سائيز; ++i)
    {
      ڪل_سائيز += (lens[i] = لڙي_ڊيگهه (array[i]));
    }
  p = نتيجو = xmalloc (ڪل_سائيز);
  لكل (i = وٿي; i < سائيز; ++i)
    {
      يادگيري_نقل (p, array[i], lens[i]);
      p += lens[i];
      إذا (i < سائيز - 1)
        {
          يادگيري_نقل (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  أرجع نتيجو;
}

صحيح
libcrun_mmap (بنية libcrun_mmap_s **ret, فراغ *addr, size_t ڊيگهائي,
              صحيح prot, صحيح flags, صحيح fd, off_t وٿي,
              libcrun_error_t *err)
{
  بنية libcrun_mmap_s *mmap_s = عدم;

  فراغ *mapped = mmap (addr, ڊيگهائي, prot, flags, fd, وٿي);
  إذا (mapped == MAP_FAILED)
    أرجع crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (حجم (بنية libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->ڊيگهائي = ڊيگهائي;

  *ret = mmap_s;

  أرجع 0;
}

صحيح
libcrun_munmap (بنية libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  صحيح ret;

  ret = munmap (mmap->addr, mmap->ڊيگهائي);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "munmap");

  حرر (mmap);

  أرجع 0;
}

ساكن ڊگهو
read_file_as_long_or_default (ثابت حرف *رستو, ڊگهو def_قدر)
{
  cleanup_free حرف *content = عدم;
  libcrun_error_t tmp_err = عدم;
  حرف *endptr;
  ڊگهو val;
  صحيح ret;

  ret = read_all_file (رستو, &content, عدم, &tmp_err);
  إذا (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      أرجع def_قدر;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  إذا (UNLIKELY (errno))
    أرجع def_قدر;
  إذا (endptr == content || (*endptr && *endptr != '\n'))
    أرجع def_قدر;
  أرجع val;
}

#وصف_ڪريو DEFAULT_حد_کان_مٿي_ID 65534

uid_t
get_حد_کان_مٿي_uid (فراغ)
{
  ساكن atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  إذا (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_حد_کان_مٿي_ID);
      cached_uid = uid;
    }
  أرجع uid;
}

gid_t
get_حد_کان_مٿي_gid (فراغ)
{
  ساكن atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  إذا (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_حد_کان_مٿي_ID);
      cached_gid = gid;
    }
  أرجع gid;
}

فراغ
consume_trailing_slashes (حرف *رستو)
{
  إذا (! رستو || رستو[0] == '\0')
    أرجع;

  حرف *آخري = رستو + لڙي_ڊيگهه (رستو);

  طالما (آخري > رستو && *(آخري - 1) == '/')
    آخري--;

  *آخري = '\0';
}

حرف **
read_dir_entries (ثابت حرف *رستو, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = عدم;
  size_t n_entries = 0;
  size_t entries_سائيز = 16;
  حرف **entries = عدم;
  بنية dirent *de;

  dir = opendir (رستو);
  إذا (UNLIKELY (dir == عدم))
    {
      crun_make_error (err, errno, "opendir `%s`", رستو);
      أرجع عدم;
    }

  entries = xmalloc (entries_سائيز * حجم (حرف *));
  طالما ((de = readdir (dir)))
    {
      إذا (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        استمر;
      إذا (n_entries == entries_سائيز)
        {
          entries_سائيز *= 2;
          entries = xrealloc (entries, entries_سائيز * حجم (حرف *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * حجم (حرف *));
  entries[n_entries] = عدم;

  أرجع entries;
}

صحيح
cpuset_string_منزل_bitmask (ثابت حرف *str, حرف **ٻاهريون, size_t *ٻاهريون_سائيز, libcrun_error_t *err)
{
  cleanup_free حرف *نقاب = عدم;
  size_t نقاب_سائيز = 0;
  ثابت حرف *p = str;
  حرف *endptr;

  طالما (*p)
    {
      ڊگهو ڊگهو شروعات_range, پڇاڙي_range;

      إذا (*p < '0' || *p > '9')
        اذهب invalid_داخلا;

      شروعات_range = strtoll (p, &endptr, 10);
      إذا (شروعات_range < 0)
        اذهب invalid_داخلا;

      p = endptr;

      إذا (*p != '-')
        پڇاڙي_range = شروعات_range;
      وإلا
        {
          p++;

          إذا (*p < '0' || *p > '9')
            اذهب invalid_داخلا;

          پڇاڙي_range = strtoll (p, &endptr, 10);

          إذا (پڇاڙي_range < شروعات_range)
            اذهب invalid_داخلا;

          p = endptr;
        }

      /* Just set some limit.  */
      إذا (پڇاڙي_range > (1 << 20))
        اذهب invalid_داخلا;

      إذا (پڇاڙي_range >= (ڊگهو ڊگهو) (نقاب_سائيز * CHAR_BIT))
        {
          size_t new_نقاب_سائيز = (پڇاڙي_range / CHAR_BIT) + 1;
          نقاب = xrealloc (نقاب, new_نقاب_سائيز);
          memset (نقاب + نقاب_سائيز, 0, new_نقاب_سائيز - نقاب_سائيز);
          نقاب_سائيز = new_نقاب_سائيز;
        }

      لكل (ڊگهو ڊگهو i = شروعات_range; i <= پڇاڙي_range; i++)
        نقاب[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      إذا (*p == ',')
        p++;
      وإلا إذا (*p)
        اذهب invalid_داخلا;
    }

  *ٻاهريون = نقاب;
  نقاب = عدم;
  *ٻاهريون_سائيز = نقاب_سائيز;

  أرجع 0;

invalid_داخلا:
  أرجع crun_make_error (err, 0, "cannot parse input `%s`", str);
}

بنية channel_fd_pair
{
  بنية ring_بفر *rb;

  صحيح اندريون_fd;
  صحيح ٻاهريون_fd;

  صحيح infd_epoll_events;
  صحيح outfd_epoll_events;
};

بنية channel_fd_pair *
channel_fd_pair_new (صحيح اندريون_fd, صحيح ٻاهريون_fd, size_t سائيز)
{
  بنية channel_fd_pair *channel = xmalloc (حجم (بنية channel_fd_pair));
  channel->اندريون_fd = اندريون_fd;
  channel->ٻاهريون_fd = ٻاهريون_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_بفر_make (سائيز);
  أرجع channel;
}

فراغ
channel_fd_pair_free (بنية channel_fd_pair *channel)
{
  إذا (channel == عدم)
    أرجع;

  ring_بفر_free (channel->rb);
  حرر (channel);
}

صحيح
channel_fd_pair_process (بنية channel_fd_pair *channel, صحيح epollfd, libcrun_error_t *err)
{
  bool is_داخلا_eagain = false, is_output_eagain = false, repeat;
  صحيح ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  لكل (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      إذا (ring_بفر_get_space_موجود (channel->rb) > 0)
        {
          ret = ring_بفر_read (channel->rb, channel->اندريون_fd, &is_داخلا_eagain, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          إذا (ret > 0)
            repeat = true;
        }
      إذا (ring_بفر_get_معلومات_موجود (channel->rb) > 0)
        {
          ret = ring_بفر_write (channel->rb, channel->ٻاهريون_fd, &is_output_eagain, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          إذا (ret > 0)
            repeat = true;
        }
    }

  إذا (epollfd >= 0)
    {
      size_t موجود = ring_بفر_get_space_موجود (channel->rb);
      size_t استعمال_ٿيل = ring_بفر_get_معلومات_موجود (channel->rb);
      صحيح events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (موجود > 0 && ! is_output_eagain) ? (EPOLLIN | (is_داخلا_eagain ? EPOLLET : 0)) : 0;
      إذا (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->اندريون_fd, events, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (استعمال_ٿيل > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      إذا (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->ٻاهريون_fd, events, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          channel->outfd_epoll_events = events;
        }
    }
  أرجع 0;
}

ساكن صحيح
open_proc (libcrun_error_t *err)
{
  صحيح fd;

#جيڪڏهن_وصف_ٿيل HAVE_NEW_MOUNT_API
  cleanup_close صحيح fsfd = -1;
  صحيح ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  إذا (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, عدم, عدم, 0);
      إذا (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          إذا (fd >= 0)
            أرجع fd;
        }
    }
#جيڪڏهن_پورو

  fd = open ("/proc", O_رستو | O_DIRECTORY | O_CLOEXEC);
  إذا (fd < 0)
    أرجع crun_make_error (err, errno, "open `/proc`");
  أرجع fd;
}

صحيح
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  إذا (! container)
    أرجع open_proc (err);

  إذا (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  أرجع container->proc_fd;
}

صحيح
libcrun_open_proc_file (libcrun_container_t *container, ثابت حرف *رستو, صحيح flags, libcrun_error_t *err)
{
  صحيح proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  إذا (proc_fd < 0)
    أرجع proc_fd;

  fd = عارضي_FAILURE_RETRY (openat (proc_fd, رستو, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  إذا (! container)
    عارضي_FAILURE_RETRY (close (proc_fd));

  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, saved_errno, "openat `/proc/%s`", رستو);

  أرجع fd;
}

صحيح
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ثابت حرف *رستو, صحيح flags, libcrun_error_t *err)
{
  cleanup_free حرف *ڀريل_رستو = عدم;
  xasprintf (&ڀريل_رستو, "%d/%s", pid, رستو);
  أرجع libcrun_open_proc_file (container, ڀريل_رستو, flags, err);
}
