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

#تعریف_کړه GNU_سرچینه
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
#که_تعریف_شوی HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#د_که_پای
#إذا HAVE_STDATOMIC_H
#  include <stdatomic.h>
#وإلا
#  define atomic_long متطاير اوږد
#  define atomic_int متطاير صحيح
#د_که_پای
#ضمن "syscalls.h"

#که_نه_تعریف_شوی CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#د_که_پای
#که_نه_تعریف_شوی RESOLVE_دننه_ریښه
#  define RESOLVE_دننه_ریښه 0x10
#د_که_پای
#که_نه_تعریف_شوی __NR_close_range
#  define __NR_close_range 436
#د_که_پای
#که_نه_تعریف_شوی __NR_openat2
#  define __NR_openat2 437
#د_که_پای
#که_نه_تعریف_شوی PROC_USER_INIT_INO
#  define PROC_USER_INIT_INO ((ino_t) 0xEFFFFFFD)
#د_که_پای

#تعریف_کړه MAX_READLINKS 32
#تعریف_کړه MAX_OPENAT2_EAGAIN_RETRIES 128

صحيح
crun_لار_exists (ثابت حرف *لار, libcrun_error_t *err)
{
  صحيح ret = access (لار, F_OK);
  إذا (ret < 0)
    {
      إذا (errno == ENOENT)
        أرجع 0;
      أرجع crun_make_error (err, errno, "access `%s`", لار);
    }
  أرجع 1;
}

صحيح
xasprintf (حرف **str, ثابت حرف *fmt, ...)
{
  صحيح ret;
  va_list دلیلونه_list;

  va_پیل (دلیلونه_list, fmt);

  ret = vasprintf (str, fmt, دلیلونه_list);
  إذا (UNLIKELY (ret < 0))
    OOM ();

  va_پای_ټکی (دلیلونه_list);
  أرجع ret;
}

صحيح
write_file_at_with_flags (صحيح dirfd, صحيح flags, mode_t ډېرپېښ, ثابت حرف *name, ثابت فراغ *معلومات, size_t len, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  صحيح ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, ډېرپېښ);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `%s` for writing", name);

  ret = خوندي_write (fd, name, معلومات, len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع (len > لوی_عدد) ? لوی_عدد : (صحيح) len;
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
    خوندي_exit (EXIT_SUCCESS);
  أرجع 0;
}

صحيح
get_file_type_fd (صحيح fd, mode_t *ډېرپېښ)
{
  بنية stat st;
  صحيح ret;

#که_تعریف_شوی HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (fd, "", AT_تشه_لار | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;

      أرجع ret;
    }
  *ډېرپېښ = stx.stx_mode;
  أرجع ret;

fallback:
#د_که_پای
  ret = fstat (fd, &st);
  *ډېرپېښ = st.st_mode;
  أرجع ret;
}

صحيح
get_file_type_at (صحيح dirfd, mode_t *ډېرپېښ, bool nofollow, ثابت حرف *لار)
{
  صحيح تشه_لار = لار == عدم ? AT_تشه_لار : 0;
  بنية stat st;
  صحيح ret;

#که_تعریف_شوی HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (dirfd, لار ?: "", تشه_لار | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;

      أرجع ret;
    }
  *ډېرپېښ = stx.stx_mode;
  أرجع ret;

fallback:
#د_که_پای
  ret = fstatat (dirfd, لار ?: "", &st, تشه_لار | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *ډېرپېښ = st.st_mode;
  أرجع ret;
}

صحيح
get_file_type (mode_t *ډېرپېښ, bool nofollow, ثابت حرف *لار)
{
  أرجع get_file_type_at (AT_FDCWD, ډېرپېښ, nofollow, لار);
}

صحيح
create_file_if_missing_at (صحيح dirfd, ثابت حرف *file, mode_t ډېرپېښ, libcrun_error_t *err)
{
  cleanup_close صحيح fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, ډېرپېښ);
  إذا (fd_write < 0)
    {
      صحيح saved_errno = errno;
      mode_t tmp_ډېرپېښ;
      صحيح ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_ډېرپېښ, true, file);
      إذا (ret == 0 && S_ISREG (tmp_ډېرپېښ))
        أرجع 0;

      أرجع crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  أرجع 0;
}

ساكن صحيح
ensure_directory_دننی_at (صحيح dirfd, حرف *لار, size_t len, صحيح ډېرپېښ, libcrun_error_t *err)
{
  حرف *it = لار + len;
  صحيح ret = 0;
  bool پلار_created = false;

  لكل (;;)
    {
      ret = mkdirat (dirfd, لار, ډېرپېښ);
      إذا (ret == 0 || errno == EEXIST)
        أرجع 0;

      صحيح saved_errno = errno;
      إذا (پلار_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = عدم;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (لار, false, &tmp_err);
          إذا (ret > 0)
            أرجع 0;
          إذا (ret < 0)
            crun_error_release (&tmp_err);

          أرجع crun_make_error (err, saved_errno, "create directory `%s`", لار);
        }

      طالما (it > لار && *it != '/')
        {
          it--;
          len--;
        }
      إذا (it == لار)
        أرجع 0;

      *it = '\0';
      ret = ensure_directory_دننی_at (dirfd, لار, len - 1, ډېرپېښ, err);
      *it = '/';
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      پلار_created = true;
    }
  أرجع ret;
}

صحيح
crun_ensure_directory_at (صحيح dirfd, ثابت حرف *لار, صحيح ډېرپېښ, bool nofollow, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *tmp = xstrdup (لار);
  ret = ensure_directory_دننی_at (dirfd, tmp, مزي_اوږدوالی (tmp), ډېرپېښ, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_dir_p_at (dirfd, لار, nofollow, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret == 0)
    أرجع crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", لار);

  أرجع 0;
}

ساكن صحيح
check_fd_is_لار (ثابت حرف *لار, صحيح fd, ثابت حرف *fdname, libcrun_error_t *err)
{
  proc_fd_لار_t fdpath;
  size_t لار_len = مزي_اوږدوالی (لار);
  حرف link[PATH_MAX];
  صحيح ret;

  get_proc_self_fd_لار (fdpath, fd);
  ret = لنډمهاله_FAILURE_RETRY (readlink (fdpath, link, حجم (link)));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "readlink `%s`", fdname);

  إذا (((size_t) ret) != لار_len || memcmp (link, لار, لار_len))
    أرجع crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, لار);

  أرجع 0;
}

ساكن صحيح
check_fd_under_لار (ثابت حرف *rootfs, size_t rootfslen, صحيح fd, ثابت حرف *fdname, libcrun_error_t *err)
{
  proc_fd_لار_t fdpath;
  حرف link[PATH_MAX];
  صحيح ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  إذا (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    أرجع 0;

  get_proc_self_fd_لار (fdpath, fd);
  ret = لنډمهاله_FAILURE_RETRY (readlink (fdpath, link, حجم (link)));
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
    لنډمهاله_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
حرف *chroot_realpath (ثابت حرف *chroot, ثابت حرف *لار, حرف resolved_لار[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
ساكن صحيح
خوندي_openat_fallback (صحيح dirfd, ثابت حرف *rootfs, ثابت حرف *لار, صحيح flags,
                      صحيح ډېرپېښ, libcrun_error_t *err)
{
  cleanup_free حرف *پلار_لار = عدم;
  ثابت حرف *وروستی_component = عدم;
  ثابت حرف *orig_لار = لار;
  ثابت حرف *لار_دننه_chroot;
  cleanup_close صحيح fd = -1;
  حرف resolved[PATH_MAX];
  حرف بفر[PATH_MAX];
  size_t rootfs_len = خوندي_strlen (rootfs);
  صحيح ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  إذا (flags & O_NOFOLLOW)
    {
      حرف *sep;

      پلار_لار = xstrdup (لار);
      sep = strrchr (پلار_لار, '/');
      إذا (sep == عدم)
        {
          /* No parent directory, the entire path is the last component.  */
          وروستی_component = لار;
          پلار_لار[0] = '\0';
        }
      وإلا إذا (sep[1] != '\0')
        {
          *sep = '\0';
          وروستی_component = لار + (sep - پلار_لار) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      إذا (وروستی_component)
        لار = پلار_لار;
    }

  لار_دننه_chroot = chroot_realpath (rootfs, لار, بفر);
  إذا (لار_دننه_chroot == عدم)
    أرجع crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_لار);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  إذا (rootfs_len > 0 && مزي_پرتله (لار_دننه_chroot, rootfs, rootfs_len) == 0)
    لار_دننه_chroot += rootfs_len;
  لار_دننه_chroot = consume_slashes (لار_دننه_chroot);

  إذا (وروستی_component)
    {
      ret = snprintf (resolved, حجم (resolved), "%s%s%s", لار_دننه_chroot,
                      لار_دننه_chroot[0] == '\0' ? "" : "/", وروستی_component);
      إذا (UNLIKELY (ret >= (صحيح) حجم (resolved)))
        أرجع crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_لار);

      لار_دننه_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  إذا (لار_دننه_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      أرجع ret;
    }

  ret = openat (dirfd, لار_دننه_chroot, flags, ډېرپېښ);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "open `%s`", orig_لار);

  fd = ret;

  ret = check_fd_under_لار (rootfs, rootfs_len, fd, orig_لار, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = fd;
  fd = -1;
  أرجع ret;
}

صحيح
خوندي_openat (صحيح dirfd, ثابت حرف *rootfs, ثابت حرف *لار, صحيح flags, صحيح ډېرپېښ,
             libcrun_error_t *err)
{
  ساكن bool openat2_supported = true;
  صحيح retries = MAX_OPENAT2_EAGAIN_RETRIES;
  صحيح ret;

  إذا (is_تشه_string (لار))
    {
      cleanup_close صحيح fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      إذا (is_تشه_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, ډېرپېښ);
      إذا (UNLIKELY (fd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      إذا (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_لار (rootfs, fd, لار, err);
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
      ret = syscall_openat2 (dirfd, لار, flags, ډېرپېښ, RESOLVE_دننه_ریښه);
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
            أرجع خوندي_openat_fallback (dirfd, rootfs, لار, flags, ډېرپېښ, err);

          أرجع crun_make_error (err, errno, "openat2 `%s`", لار);
        }

      أرجع ret;
    }

  أرجع خوندي_openat_fallback (dirfd, rootfs, لار, flags, ډېرپېښ, err);
}

ssize_t
خوندي_readlinkat (صحيح dfd, ثابت حرف *name, حرف **بفر, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_اندازه = hint > 0 ? hint + 1 : 512;
  cleanup_free حرف *tmp_buf = عدم;
  ssize_t اندازه;

  افعل
    {
      إذا (tmp_buf != عدم)
        buf_اندازه += 256;

      tmp_buf = xrealloc (tmp_buf, buf_اندازه);

      اندازه = readlinkat (dfd, name, tmp_buf, buf_اندازه);
      إذا (UNLIKELY (اندازه < 0))
        أرجع crun_make_error (err, errno, "readlink `%s`", name);
  } طالما (اندازه == buf_اندازه);

  /* Always NUL terminate the buffer.  */
  tmp_buf[اندازه] = '\0';

  /* Move ownership to BUFFER.  */
  *بفر = tmp_buf;
  tmp_buf = عدم;

  أرجع اندازه;
}

ساكن صحيح crun_خوندي_ensure_at (bool do_open, bool dir, bool follow_symlinks, صحيح dirfd, ثابت حرف *dirpath,
                                ثابت حرف *لار, صحيح ډېرپېښ, صحيح max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
ساكن صحيح
crun_خوندي_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, صحيح dirfd, ثابت حرف *dirpath, صحيح cwd,
                                 ثابت حرف *npath, ثابت حرف *cur, ثابت حرف *rest,
                                 صحيح ډېرپېښ, صحيح max_readlinks, libcrun_error_t *err)
{
  cleanup_free حرف *موخه = عدم;
  cleanup_free حرف *new_لار = عدم;
  ssize_t ret;

  ret = خوندي_readlinkat (cwd, cur, &موخه, 0, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_لار, "%.*s%s%s%s", موخه[0] == '/' ? 0 : (صحيح) (cur - npath), npath, موخه,
             rest ? "/" : "", rest ? rest : "");

  أرجع crun_خوندي_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_لار, ډېرپېښ, max_readlinks - 1, err);
}

ساكن صحيح
crun_خوندي_ensure_at (bool do_open, bool dir, bool follow_symlinks, صحيح dirfd, ثابت حرف *dirpath,
                     ثابت حرف *لار, صحيح ډېرپېښ, صحيح max_readlinks, libcrun_error_t *err)
{
  cleanup_close صحيح wd_cleanup = -1;
  cleanup_free حرف *npath = عدم;
  bool وروستی_component = false;
  size_t ژوروالی = 0;
  حرف *cur;
  حرف *it;
  صحيح cwd;
  صحيح ret;

  إذا (max_readlinks <= 0)
    أرجع crun_make_error (err, ELOOP, "resolve path `%s`", لار);

  لار = consume_slashes (لار);

  /* Empty path, nothing to do.  */
  إذا (*لار == '\0')
    {
      إذا (do_open)
        أرجع open (dirpath, O_CLOEXEC | O_لار, 0);
      أرجع 0;
    }

  npath = xstrdup (لار);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  طالما (cur)
    {
      إذا (it)
        *it = '\0';
      وإلا
        وروستی_component = true;

      إذا (cur[0] == '\0')
        اكسر;

      إذا (strcmp (cur, ".") == 0)
        اذهب بل;
      وإلا إذا (strcmp (cur, ".."))
        ژوروالی++;
      وإلا
        {
          إذا (ژوروالی)
            ژوروالی--;
          وإلا
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              اذهب بل;
            }
        }

      إذا (وروستی_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          إذا (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              إذا (errno == ELOOP)
                أرجع crun_خوندي_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, عدم,
                                                        ډېرپېښ, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_لار | O_NOFOLLOW, 0);
              إذا (ret < 0)
                أرجع crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          إذا (do_open)
            أرجع ret;

          close_and_replace (&wd_cleanup, ret);
          أرجع 0;
        }

      ret = mkdirat (cwd, cur, ډېرپېښ);
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
            أرجع crun_خوندي_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : عدم, ډېرپېښ, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = خوندي_openat (dirfd, dirpath, npath, O_لار | O_CLOEXEC, 0, err);
      إذا (UNLIKELY (cwd < 0))
        أرجع crun_error_wrap (err, "creating `/%s`", لار);

      إذا (! وروستی_component)
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
              أرجع crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", لار, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    بل:
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
crun_خوندي_create_and_open_ref_at (bool dir, صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *لار, صحيح ډېرپېښ, libcrun_error_t *err)
{
  صحيح ret;

  /* If the file/dir already exists, just open it.  */
  ret = خوندي_openat (dirfd, dirpath, لار, O_لار | O_CLOEXEC, 0, err);
  إذا (LIKELY (ret >= 0))
    أرجع ret;

  crun_error_release (err);
  أرجع crun_خوندي_ensure_at (true, dir, true, dirfd, dirpath, لار, ډېرپېښ, MAX_READLINKS, err);
}

صحيح
crun_خوندي_ensure_directory_at (صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *لار, صحيح ډېرپېښ,
                               libcrun_error_t *err)
{
  أرجع crun_خوندي_ensure_at (false, true, false, dirfd, dirpath, لار, ډېرپېښ, MAX_READLINKS, err);
}

صحيح
crun_خوندي_ensure_file_at (صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *لار, صحيح ډېرپېښ,
                          libcrun_error_t *err)
{
  أرجع crun_خوندي_ensure_at (false, false, false, dirfd, dirpath, لار, ډېرپېښ, MAX_READLINKS, err);
}

صحيح
crun_ensure_directory (ثابت حرف *لار, صحيح ډېرپېښ, bool nofollow, libcrun_error_t *err)
{
  أرجع crun_ensure_directory_at (AT_FDCWD, لار, ډېرپېښ, nofollow, err);
}

ساكن صحيح
get_file_اندازه (صحيح fd, off_t *اندازه)
{
  بنية stat st;
  صحيح ret;
#که_تعریف_شوی HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (fd, "", AT_تشه_لار | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_اندازه, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;
      أرجع ret;
    }
  *اندازه = stx.stx_size;

  أرجع ret;

fallback:
#د_که_پای
  ret = fstat (fd, &st);
  *اندازه = st.st_size;
  أرجع ret;
}

صحيح
crun_dir_p_at (صحيح dirfd, ثابت حرف *لار, bool nofollow, libcrun_error_t *err)
{
  mode_t ډېرپېښ;
  صحيح ret;

  ret = get_file_type_at (dirfd, &ډېرپېښ, nofollow, لار);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "stat `%s`", لار);

  أرجع S_ISDIR (ډېرپېښ) ? 1 : 0;
}

صحيح
crun_dir_p (ثابت حرف *لار, bool nofollow, libcrun_error_t *err)
{
  أرجع crun_dir_p_at (AT_FDCWD, لار, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
ساكن صحيح
check_running_دننه_user_namespace_uid_map (libcrun_error_t *err)
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

  أرجع مزي_پلټنه (بفر, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
ساكن bool
running_دننه_openvz ()
{
  إذا (access ("/proc/vz", F_OK) < 0)
    أرجع false;

  أرجع access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

صحيح
check_running_دننه_user_namespace (libcrun_error_t *err)
{
  ساكن atomic_int پرلپسې_دننه_userns = -1;
  بنية stat st;
  صحيح ret;

  ret = پرلپسې_دننه_userns;
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
          پرلپسې_دننه_userns = 0;
          أرجع پرلپسې_دننه_userns;
        }

      إذا (! running_دننه_openvz ())
        {
          پرلپسې_دننه_userns = 1;
          أرجع پرلپسې_دننه_userns;
        }
    }
  وإلا إذا (UNLIKELY (errno != ENOENT))
    أرجع crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_دننه_user_namespace_uid_map (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  پرلپسې_دننه_userns = ret;
  أرجع پرلپسې_دننه_userns;
}

ساكن size_t
get_پاڼه_اندازه ()
{
  ساكن atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  إذا (pagesize == 0)
    {
      pagesize = (اوږد) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  أرجع (size_t) pagesize;
}

ساكن صحيح selinux_enabled = -1;
ساكن صحيح apparmor_enabled = -1;

صحيح
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free حرف *بهر = عدم;
  cleanup_close صحيح fd = -1;
  size_t len;
  صحيح ret;

  إذا (selinux_enabled >= 0)
    أرجع selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = read_all_fd_with_اندازه_hint (fd, "mounts", &بهر, &len, get_پاڼه_اندازه (), err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  selinux_enabled = مزي_پلټنه (بهر, "selinux") ? 1 : 0;

  أرجع selinux_enabled;
}

صحيح
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  صحيح اندازه;
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

  اندازه = لنډمهاله_FAILURE_RETRY (اقرأ (fd, buf, حجم (buf)));

  apparmor_enabled = اندازه > 0 && buf[0] == 'Y' ? 1 : 0;

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
جمع_کړه_selinux_mount_label (حرف **retlabel, ثابت حرف *معلومات, ثابت حرف *label, ثابت حرف *context_type, libcrun_error_t *err)
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
lsm_attr_لار (libcrun_container_t *container, ثابت حرف *lsm, ثابت حرف *fname, libcrun_error_t *err)
{
  cleanup_close صحيح attr_dirfd = -1;
  cleanup_close صحيح lsm_dirfd = -1;
  حرف *attr_لار = عدم;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_لار, err);
  إذا (UNLIKELY (attr_dirfd < 0))
    أرجع عدم;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  إذا (lsm != عدم)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_لار | O_CLOEXEC);

      إذا (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          أرجع عدم;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_لار, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  أرجع attr_لار;
}

ساكن صحيح
check_proc_super_magic (صحيح fd, ثابت حرف *لار, libcrun_error_t *err)
{
  بنية statfs sfs;

  صحيح ret = fstatfs (fd, &sfs);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "statfs `%s`", لار);

  إذا (sfs.f_type != PROC_SUPER_MAGIC)
    أرجع crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", لار);

  أرجع 0;
}

ساكن صحيح
سېټ_security_attr (libcrun_container_t *container, ثابت حرف *lsm, ثابت حرف *fname, ثابت حرف *معلومات, libcrun_error_t *err)
{
  صحيح ret;

  cleanup_free ثابت حرف *attr_لار = lsm_attr_لار (container, lsm, fname, err);
  cleanup_close صحيح fd = -1;

  إذا (UNLIKELY (attr_لار == عدم))
    أرجع -1;

  fd = libcrun_open_proc_file (container, attr_لار, O_WRONLY | O_CLOEXEC, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = check_proc_super_magic (fd, attr_لار, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  // Write out data
  ret = لنډمهاله_FAILURE_RETRY (اكتب (fd, معلومات, مزي_اوږدوالی (معلومات)));
  إذا (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      إذا (errno == ENOENT && lsm != عدم && strcmp (lsm, "apparmor") == 0)
        {
          ثابت حرف *profile = strchr (معلومات, ' ');

          أرجع crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : معلومات);
        }
      أرجع crun_make_error (err, errno, "write to file `%s`", attr_لار);
    }

  أرجع 0;
}

صحيح
سېټ_selinux_label (libcrun_container_t *container, ثابت حرف *label, bool now, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_is_selinux_enabled (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret)
    أرجع سېټ_security_attr (container, عدم, now ? "current" : "exec", label, err);
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
#تعریف_کړه UNCONFINED "unconfined"
#تعریف_کړه UNCONFINED_LEN (ssize_t) (حجم (UNCONFINED) - 1)
  cleanup_free ثابت حرف *attr_لار = lsm_attr_لار (container, "apparmor", "current", err);
  cleanup_close صحيح fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  حرف buf[UNCONFINED_LEN + 1];

  إذا (UNLIKELY (attr_لار == عدم))
    أرجع -1;

  fd = libcrun_open_proc_file (container, attr_لار, O_RDONLY | O_CLOEXEC, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  إذا (UNLIKELY (check_proc_super_magic (fd, attr_لار, err)))
    أرجع -1;

  ssize_t بایټونه_read = لنډمهاله_FAILURE_RETRY (اقرأ (fd, buf, حجم (buf)));
  إذا (UNLIKELY (بایټونه_read < 0))
    أرجع crun_make_error (err, errno, "read from `%s`", attr_لار);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  إذا (بایټونه_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    أرجع 1;
  إذا (بایټونه_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    أرجع 0;
  أرجع 1;
#تعریف_لغوه UNCONFINED
#تعریف_لغوه UNCONFINED_LEN
}

صحيح
سېټ_apparmor_profile (libcrun_container_t *container, ثابت حرف *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
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

      أرجع سېټ_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  أرجع 0;
}

ساكن صحيح
read_all_fd_دننی (صحيح fd, ثابت حرف *description, حرف **بهر, size_t *len, size_t اندازه_hint, size_t max_اندازه, libcrun_error_t *err)
{
  cleanup_free حرف *buf = عدم;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t اندازه = 0;
  صحيح ret;

  إذا (اندازه_hint)
    allocated = اندازه_hint;
  وإلا
    {
      ret = get_file_اندازه (fd, &اندازه);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "stat `%s`", description);

      إذا (max_اندازه && (size_t) اندازه > max_اندازه)
        أرجع crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_اندازه);

      allocated = اندازه == 0 ? 1023 : اندازه;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  طالما ((اندازه && nread < (size_t) اندازه) || اندازه == 0)
    {
      ret = لنډمهاله_FAILURE_RETRY (اقرأ (fd, buf + nread, allocated - nread));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "read from file `%s`", description);

      إذا (ret == 0)
        اكسر;

      nread += ret;

      إذا (max_اندازه && nread > max_اندازه)
        أرجع crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_اندازه);

      إذا (nread == allocated)
        {
          إذا (اندازه)
            اكسر;

          إذا (pagesize == 0)
            pagesize = get_پاڼه_اندازه ();

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
  *بهر = buf;
  buf = عدم;
  إذا (len)
    *len = nread;
  أرجع 0;
}

صحيح
read_all_fd_with_اندازه_hint (صحيح fd, ثابت حرف *description, حرف **بهر, size_t *len, size_t اندازه_hint, libcrun_error_t *err)
{
  أرجع read_all_fd_دننی (fd, description, بهر, len, اندازه_hint, 0, err);
}

صحيح
read_all_fd_with_max_اندازه (صحيح fd, ثابت حرف *description, حرف **بهر, size_t *len, size_t max_اندازه, libcrun_error_t *err)
{
  أرجع read_all_fd_دننی (fd, description, بهر, len, 0, max_اندازه, err);
}

صحيح
read_all_file_at (صحيح dirfd, ثابت حرف *لار, حرف **بهر, size_t *len, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;

  fd = لنډمهاله_FAILURE_RETRY (openat (dirfd, لار, O_RDONLY | O_CLOEXEC));
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", لار);

  أرجع read_all_fd (fd, لار, بهر, len, err);
}

صحيح
read_all_file (ثابت حرف *لار, حرف **بهر, size_t *len, libcrun_error_t *err)
{
  إذا (strcmp (لار, "-") == 0)
    لار = "/dev/stdin";

  أرجع read_all_file_at (AT_FDCWD, لار, بهر, len, err);
}

صحيح
get_realpath_منزل_file (صحيح dirfd, ثابت حرف *لار_name, حرف **absolute_لار, libcrun_error_t *err)
{
  cleanup_close صحيح targetfd = -1;

  targetfd = لنډمهاله_FAILURE_RETRY (openat (dirfd, لار_name, O_RDONLY | O_CLOEXEC));
  إذا (UNLIKELY (targetfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", لار_name);
  وإلا
    {
      ssize_t len;
      proc_fd_لار_t موخه_fd_لار;

      get_proc_self_fd_لار (موخه_fd_لار, targetfd);
      len = خوندي_readlinkat (AT_FDCWD, موخه_fd_لار, absolute_لار, 0, err);
      إذا (UNLIKELY (len < 0))
        أرجع crun_error_wrap (err, "error unable to provide absolute path to file `%s`", لار_name);
    }

  أرجع 0;
}

صحيح
open_unix_domain_client_socket (ثابت حرف *لار, صحيح dgram, libcrun_error_t *err)
{
  بنية sockaddr_un addr = {};
  صحيح ret;
  proc_fd_لار_t name_buf;
  cleanup_close صحيح destfd = -1;
  cleanup_close صحيح fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", لار);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_بهیر, 0);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "create UNIX socket");

  إذا (مزي_اوږدوالی (لار) >= حجم (addr.sun_لار))
    {
      destfd = open (لار, O_لار | O_CLOEXEC);
      إذا (UNLIKELY (destfd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", لار);

      get_proc_self_fd_لار (name_buf, destfd);

      لار = name_buf;
    }

  size_t لار_len = مزي_اوږدوالی (لار);
  إذا (لار_len >= حجم (addr.sun_لار))
    أرجع crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", لار);

  حافظه_نقل (addr.sun_لار, لار, لار_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (بنية sockaddr *) &addr, حجم (addr));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "connect socket to `%s`", لار);

  ret = fd;
  fd = -1;

  أرجع ret;
}

صحيح
open_unix_domain_socket (ثابت حرف *لار, صحيح dgram, libcrun_error_t *err)
{
  بنية sockaddr_un addr = {};
  proc_fd_لار_t name_buf;
  صحيح ret;
  cleanup_close صحيح dirfd = -1;
  cleanup_close صحيح fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_بهیر, 0);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "create UNIX socket");

  إذا (مزي_اوږدوالی (لار) >= حجم (addr.sun_لار))
    {
      cleanup_free حرف *dpath = xstrdup (لار);
      cleanup_free حرف *bpath = xstrdup (لار);
      ثابت حرف *پلار_dir = dirname (dpath);
      ثابت حرف *بنسټ = basename (bpath);
      صحيح n;

      dirfd = open (پلار_dir, O_لار | O_DIRECTORY | O_CLOEXEC);
      إذا (UNLIKELY (dirfd < 0))
        أرجع crun_make_error (err, errno, "open directory `%s`", پلار_dir);

      get_proc_self_fd_لار (name_buf, dirfd);

      n = snprintf (addr.sun_لار, حجم (addr.sun_لار), "%s/%s", name_buf, بنسټ);
      إذا (n < 0 || (size_t) n >= حجم (addr.sun_لار))
        أرجع crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", لار);
    }
  وإلا
    {
      size_t لار_len = مزي_اوږدوالی (لار);
      حافظه_نقل (addr.sun_لار, لار, لار_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (بنية sockaddr *) &addr, حجم (addr));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "bind socket to `%s`", لار);

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
  بنية msghdr پیغام = {};
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

  پیغام.پیغام_name = عدم;
  پیغام.پیغام_namelen = 0;
  پیغام.پیغام_iov = iov;
  پیغام.پیغام_iovlen = 1;
  پیغام.پیغام_controllen = CMSG_SPACE (حجم (صحيح));
  پیغام.پیغام_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&پیغام);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (حجم (صحيح));

  *((صحيح *) CMSG_معلومات (cmsg)) = fd;

  ret = لنډمهاله_FAILURE_RETRY (sendmsg (server, &پیغام, 0));
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
  بنية msghdr پیغام = {};
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

  پیغام.پیغام_name = عدم;
  پیغام.پیغام_namelen = 0;
  پیغام.پیغام_iov = iov;
  پیغام.پیغام_iovlen = 1;
  پیغام.پیغام_controllen = CMSG_SPACE (حجم (صحيح));
  پیغام.پیغام_control = ctrl_buf;

  ret = لنډمهاله_FAILURE_RETRY (recvmsg (from, &پیغام, 0));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "recvmsg");
  إذا (UNLIKELY (ret == 0))
    أرجع crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&پیغام);
  إذا (cmsg == عدم)
    أرجع crun_make_error (err, 0, "no msg received");
  حافظه_نقل (&fd, CMSG_معلومات (cmsg), حجم (fd));

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
create_signalfd (sigset_t *ماسک, libcrun_error_t *err)
{
  صحيح ret = signalfd (-1, ماسک, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "signalfd");
  أرجع ret;
}

ساكن صحيح
epoll_helper_toggle (صحيح epollfd, صحيح fd, صحيح events, libcrun_error_t *err)
{
  بنية epoll_event ev = {};
  bool جمع_کړه = events != 0;
  صحيح ret;

  ev.events = events;
  ev.معلومات.fd = fd;
  ret = epoll_ctl (epollfd, جمع_کړه ? EPOLL_CTL_جمع_کړه : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  إذا (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  إذا (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      إذا (errno == ENOENT || errno == EPERM)
        أرجع 0;
      أرجع crun_make_error (err, errno, "epoll_ctl `%s` `%d`", جمع_کړه ? "add" : "del", fd);
    }
  أرجع 0;
}

صحيح
epoll_helper (صحيح *دننه_fds, صحيح *دننه_levelfds, صحيح *بهر_fds, صحيح *بهر_levelfds, libcrun_error_t *err)
{
  بنية epoll_event ev;
  cleanup_close صحيح epollfd = -1;
  صحيح ret;

  صحيح *it;
  epollfd = epoll_create1 (0);
  إذا (UNLIKELY (epollfd < 0))
    أرجع crun_make_error (err, errno, "epoll_create1");

#تعریف_کړه جمع_کړه_FDS(FDS, EVENTS)                                            \
  لكل (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.معلومات.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_جمع_کړه, *it, &ev);               \
      إذا (UNLIKELY (ret < 0))                                           \
        أرجع crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  إذا (دننه_fds)
    جمع_کړه_FDS (دننه_fds, EPOLLIN);
  إذا (دننه_levelfds)
    جمع_کړه_FDS (دننه_levelfds, EPOLLIN | EPOLLET);
  إذا (بهر_fds)
    جمع_کړه_FDS (بهر_fds, EPOLLOUT);
  إذا (بهر_levelfds)
    جمع_کړه_FDS (بهر_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  أرجع ret;
}

صحيح
copy_from_fd_منزل_fd (صحيح src, صحيح dst, صحيح consume, libcrun_error_t *err)
{
  صحيح ret;
  ssize_t nread;
  size_t pagesize = get_پاڼه_اندازه ();
#که_تعریف_شوی HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#د_که_پای
  افعل
    {
      cleanup_free حرف *بفر = عدم;
      ssize_t پاتې_شوی;

#که_تعریف_شوی HAVE_COPY_FILE_RANGE
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
#د_که_پای

      بفر = xmalloc (pagesize);
      nread = لنډمهاله_FAILURE_RETRY (اقرأ (src, بفر, pagesize));
      إذا (consume && nread < 0 && errno == EAGAIN)
        أرجع 0;
      إذا (nread < 0 && errno == EIO)
        أرجع 0;
      إذا (UNLIKELY (nread < 0))
        أرجع crun_make_error (err, errno, "read");

      پاتې_شوی = nread;
      طالما (پاتې_شوی)
        {
          ret = لنډمهاله_FAILURE_RETRY (اكتب (dst, بفر + nread - پاتې_شوی, پاتې_شوی));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "write");
          پاتې_شوی -= ret;
        }
  } طالما (consume && nread);

  أرجع 0;
}

صحيح
پرلپسې_process (حرف **دلیلونه, libcrun_error_t *err)
{
  pid_t pid = fork ();
  إذا (UNLIKELY (pid < 0))
    أرجع crun_make_error (err, errno, "fork");
  إذا (pid)
    {
      صحيح r, حالت;
      r = لنډمهاله_FAILURE_RETRY (waitpid (pid, &حالت, 0));
      إذا (r < 0)
        أرجع crun_make_error (err, errno, "waitpid");
      إذا (WIFEXITED (حالت) || WIFSIGNALED (حالت))
        أرجع WEXITSTATUS (حالت);
    }

  execvp (دلیلونه[0], دلیلونه);
  خوندي_exit (EXIT_FAILURE);
}

#که_نه_تعریف_شوی HAVE_FGETPWENT_R
ساكن بینښه
atou (حرف **s)
{
  بینښه x;
  لكل (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  أرجع x;
}

صحيح
fgetpwent_r (FILE *f, بنية passwd *pw, حرف *کرښه, size_t اندازه, بنية passwd **res)
{
  حرف *s;
  صحيح rv = 0;
  لكل (;;)
    {
      کرښه[اندازه - 1] = '\xff';
      إذا ((fgets (کرښه, اندازه, f) == عدم) || ferror (f) || کرښه[اندازه - 1] != '\xff')
        {
          rv = (کرښه[اندازه - 1] != '\xff') ? ERANGE : ENOENT;
          کرښه = 0;
          pw = 0;
          اكسر;
        }
      کرښه[strcspn (کرښه, "\n")] = 0;

      s = کرښه;
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
#د_که_پای

صحيح
سېټ_home_env (uid_t id)
{
  بنية passwd pwd;
  cleanup_free حرف *buf = عدم;
  اوږد buf_اندازه;
  cleanup_file FILE *بهیر = عدم;

  buf_اندازه = sysconf (_SC_GETPW_R_SIZE_MAX);
  إذا (buf_اندازه < 0)
    buf_اندازه = 1024;

  buf = xmalloc (buf_اندازه);

  بهیر = fopen ("/etc/passwd", "re");
  إذا (بهیر == عدم)
    أرجع -1;

  لكل (;;)
    {
      بنية passwd *ret_pw = عدم;

      صحيح ret = fgetpwent_r (بهیر, &pwd, buf, buf_اندازه, &ret_pw);
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

          buf_اندازه *= 2;
          buf = xrealloc (buf, buf_اندازه);
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
  cleanup_file FILE *ننوت = عدم;
  cleanup_free حرف *lineptr = عدم;
  size_t lenlineptr = 0, len_name;
  اوږد buf_اندازه;
  cleanup_free حرف *buf = عدم;
  ثابت حرف *name;
  بنية passwd pwd;

  buf_اندازه = sysconf (_SC_GETPW_R_SIZE_MAX);
  إذا (buf_اندازه < 0)
    buf_اندازه = 1024;

  buf = xmalloc (buf_اندازه);
  لكل (;;)
    {
      صحيح ret;
      بنية passwd *ret_pw = عدم;

      ret = getpwuid_r (id, &pwd, buf, buf_اندازه, &ret_pw);
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

      buf_اندازه *= 2;
      buf = xrealloc (buf, buf_اندازه);
    }

  len_name = مزي_اوږدوالی (name);

  ننوت = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  إذا (ننوت == عدم)
    أرجع -1;

  لكل (;;)
    {
      حرف *endptr;
      ssize_t اقرأ = getline (&lineptr, &lenlineptr, ننوت);
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

#تعریف_کړه MIN(x, y) ((x) < (y) ? (x) : (y))

صحيح
format_default_id_mapping (حرف **بهر, uid_t container_id, uid_t host_uid, uid_t host_id, صحيح is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, شته = 0;
  cleanup_free حرف *بفر = عدم;
  صحيح written = 0;
  صحيح ret, پاتې_شوی;

  *بهر = عدم;

  إذا (getsubidrange (host_uid, is_uid, &from, &شته) < 0)
    أرجع 0;

  /* More than enough space for all the mappings.  */
  پاتې_شوی = 15 * 5 * 3;
  بفر = xmalloc (پاتې_شوی + 1);

  إذا (container_id > 0)
    {
      uint32_t کارول_شوی = MIN (container_id, شته);
      ret = snprintf (بفر + written, پاتې_شوی, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, کارول_شوی);
      إذا (UNLIKELY (ret >= پاتې_شوی))
        أرجع crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      پاتې_شوی -= ret;

      from += کارول_شوی;
      شته -= کارول_شوی;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (بفر + written, پاتې_شوی, "%d %d 1\n", container_id, host_id);
  إذا (UNLIKELY (ret >= پاتې_شوی))
    أرجع crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  پاتې_شوی -= ret;

  /* Last mapping: use any id that is left.  */
  إذا (شته)
    {
      ret = snprintf (بفر + written, پاتې_شوی, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, شته);
      إذا (UNLIKELY (ret >= پاتې_شوی))
        أرجع crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      پاتې_شوی -= ret;
    }

  *بهر = بفر;
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
پرلپسې_process_ماشوم (حرف *لار, حرف **دلیلونه, ثابت حرف *cwd, حرف **envp, صحيح pipe_r,
                   صحيح pipe_w, صحيح بهر_fd, صحيح err_fd, bool can_ignore_chdir_errors)
{
  حرف *tmp_دلیلونه[] = { لار, عدم };
  libcrun_error_t err = عدم;
  صحيح dev_null_fd = -1;
  صحيح ret;

  ret = mark_or_close_fds_ge_than (عدم, 3, false, &err);
  إذا (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->حالت, "%s", (err)->پیغام);

  إذا (بهر_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      إذا (UNLIKELY (dev_null_fd < 0))
        خوندي_exit (EXIT_FAILURE);
    }

  لنډمهاله_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  لنډمهاله_FAILURE_RETRY (close (pipe_r));

  dup2 (بهر_fd >= 0 ? بهر_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  إذا (بهر_fd >= 0)
    unset_cloexec_flag (1);
  إذا (err_fd >= 0)
    unset_cloexec_flag (2);

  إذا (dev_null_fd >= 0)
    لنډمهاله_FAILURE_RETRY (close (dev_null_fd));
  إذا (بهر_fd >= 0)
    لنډمهاله_FAILURE_RETRY (close (بهر_fd));
  إذا (err_fd >= 0)
    لنډمهاله_FAILURE_RETRY (close (err_fd));

  إذا (دلیلونه == عدم)
    دلیلونه = tmp_دلیلونه;

  إذا (cwd && chdir (cwd) < 0)
    {
      إذا (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        خوندي_exit (EXIT_FAILURE);
    }

  execvpe (لار, دلیلونه, envp);
  خوندي_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
صحيح
پرلپسې_process_with_stdin_timeout_envp (حرف *لار, حرف **دلیلونه, ثابت حرف *cwd, صحيح timeout,
                                     حرف **envp, حرف *stdin, size_t stdin_len, صحيح بهر_fd,
                                     صحيح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  صحيح stdin_pipe[2];
  pid_t pid;
  صحيح ret;
  cleanup_close صحيح pipe_r = -1;
  cleanup_close صحيح pipe_w = -1;
  sigset_t oldmask, ماسک;
  صحيح r, حالت;

  sigemptyset (&ماسک);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  إذا (timeout > 0)
    {
      sigaddset (&ماسک, SIGCHLD);
      ret = sigprocmask (SIG_بلاک, &ماسک, &oldmask);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  إذا (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      اذهب restore_sig_ماسک_and_exit;
    }

  إذا (pid == 0)
    {
      /* run_process_child doesn't return.  */
      پرلپسې_process_ماشوم (لار, دلیلونه, cwd, envp, pipe_r, pipe_w, بهر_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = لنډمهاله_FAILURE_RETRY (اكتب (pipe_w, stdin, stdin_len));
  إذا (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      إذا (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          اذهب restore_sig_ماسک_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  إذا (timeout)
    {
      time_t پیل = وخت (عدم);
      time_t now;
      لكل (now = پیل; now - پیل < timeout; now = وخت (عدم))
        {
          siginfo_t info;
          صحيح elapsed = now - پیل;
          بنية timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&ماسک, &info, &ts_timeout);
          إذا (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              اذهب restore_sig_ماسک_and_exit;
            }

          إذا (info.si_signo == SIGCHLD && info.si_pid == pid)
            اذهب read_waitpid;

          إذا (ret < 0 && errno == EAGAIN)
            اذهب timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", لار);
      اذهب restore_sig_ماسک_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &حالت, 0);
  إذا (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  وإلا
    ret = get_process_exit_حالت (حالت);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_ماسک_and_exit:
  إذا (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      إذا (pid > 0)
        {
          kill (pid, SIGKILL);
          لنډمهاله_FAILURE_RETRY (waitpid (pid, &حالت, 0));
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
  بنية dirent *بل;

#که_تعریف_شوی ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  إذا (close_now)
    GCOV_DUMP ();
#د_که_پای

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
  لكل (بل = readdir (dir); بل; بل = readdir (dir))
    {
      صحيح val;
      ثابت حرف *name = بل->d_name;
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
get_current_timestamp (حرف *بهر, size_t len)
{
  بنية timeval tv;
  بنية tm now;
  حرف timestamp[64];

  gettimeofday (&tv, عدم);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, حجم (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (فراغ) snprintf (بهر, len, "%s.%06lldZ", timestamp, (اوږد اوږد صحيح) tv.tv_usec);
  بهر[len - 1] = '\0';
}

صحيح
سېټ_blocking_fd (صحيح fd, bool blocking, libcrun_error_t *err)
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
parse_json_file (json_object **بهر, ثابت حرف *jsondata, بنية parser_context *ctx arg_ناکارول_شوی, libcrun_error_t *err)
{
  بنية json_tokener *tok;
  تعداد json_tokener_error jerr;

  *err = عدم;

  إذا (UNLIKELY (jsondata == عدم))
    أرجع crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  إذا (UNLIKELY (tok == عدم))
    أرجع crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_سېټ_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *بهر = json_tokener_parse_ex (tok, jsondata, (صحيح) مزي_اوږدوالی (jsondata));
  jerr = json_tokener_get_error (tok);
  إذا (*بهر == عدم || jerr != json_tokener_success)
    {
      صحيح saved_pos = (صحيح) tok->char_دننه_واټن;

      json_tokener_free (tok);
      إذا (*بهر)
        {
          json_object_put (*بهر);
          *بهر = عدم;
        }
      أرجع crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  أرجع 0;
}

#تعریف_کړه CHECK_ACCESS_NOT_EXECUTABLE 1
#تعریف_کړه CHECK_ACCESS_NOT_REGULAR 2

/* check that the specified path exists and it is executable.
   Return
   - 0 if the file is executable
   - CHECK_ACCESS_NOT_EXECUTABLE if it exists but it is not executable
   - CHECK_ACCESS_NOT_REGULAR if it not a regular file
   - -errno for any other generic error
 */
ساكن صحيح
check_access (ثابت حرف *لار)
{
  صحيح ret;
  mode_t ډېرپېښ;

#که_تعریف_شوی HAVE_EACCESS
#  define ACCESS(لار, ډېرپېښ) (eaccess (لار, ډېرپېښ))
#وإلا
#  define ACCESS(لار, ډېرپېښ) (access (لار, ډېرپېښ))
#د_که_پای

  ret = ACCESS (لار, X_OK);
  إذا (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      إذا (errno == EACCES)
        {
          صحيح saved_errno = errno;
          ret = ACCESS (لار, F_OK);
          errno = saved_errno;

          إذا (ret == 0)
            أرجع CHECK_ACCESS_NOT_EXECUTABLE;
        }
      أرجع -errno;
    }

  ret = get_file_type (&ډېرپېښ, false, لار);
  إذا (UNLIKELY (ret < 0))
    أرجع -errno;

  إذا (! S_ISREG (ډېرپېښ))
    أرجع CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  أرجع 0;
}

صحيح
پیدا_کړه_executable (حرف **بهر, ثابت حرف *executable_لار, ثابت حرف *cwd, libcrun_error_t *err)
{
  cleanup_free حرف *cwd_executable_لار = عدم;
  cleanup_free حرف *tmp = عدم;
  صحيح وروستی_error = -ENOENT;
  حرف *it, *پای_ټکی;
  صحيح ret;

  *بهر = عدم;

  إذا (executable_لار == عدم || executable_لار[0] == '\0')
    أرجع crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  إذا (executable_لار[0] != '/' && strchr (executable_لار, '/'))
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
      ret = append_paths (&cwd_executable_لار, err, "/", cwd, executable_لار, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      executable_لار = cwd_executable_لار;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  إذا (executable_لار[0] == '/')
    {
      ret = check_access (executable_لار);
      إذا (LIKELY (ret == 0))
        {
          *بهر = xstrdup (executable_لار);
          أرجع 0;
        }
      وروستی_error = ret;
      اذهب fail;
    }

  پای_ټکی = tmp = xstrdup (getenv ("PATH"));

  طالما ((it = strsep (&پای_ټکی, ":")))
    {
      cleanup_free حرف *لار = عدم;

      إذا (it == پای_ټکی)
        it = ".";

      ret = append_paths (&لار, err, it, executable_لار, عدم);
      إذا (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          استمر;
        }

      ret = check_access (لار);
      إذا (ret == 0)
        {
          /* Change owner.  */
          *بهر = لار;
          لار = عدم;
          أرجع 0;
        }

      إذا (ret == -ENOENT)
        استمر;

      وروستی_error = ret;
    }

fail:
  بدل (وروستی_error)
    {
    حالة CHECK_ACCESS_NOT_EXECUTABLE:
      أرجع crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_لار);

    حالة CHECK_ACCESS_NOT_REGULAR:
      أرجع crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_لار);

    افتراضي:
      errno = -وروستی_error;
      إذا (errno == ENOENT)
        أرجع crun_make_error (err, errno, "executable file `%s` not found%s", executable_لار, executable_لار[0] == '/' ? "" : " in $PATH");
      أرجع crun_make_error (err, errno, "open `%s`", executable_لار);
    }
}

#که_تعریف_شوی HAVE_FGETXATTR

ساكن ssize_t
خوندي_read_xattr (حرف **ret, صحيح sfd, ثابت حرف *srcname, ثابت حرف *name, size_t initial_اندازه, libcrun_error_t *err)
{
  cleanup_free حرف *بفر = عدم;
  ssize_t current_اندازه;
  ssize_t s;

  current_اندازه = (ssize_t) initial_اندازه;
  بفر = xmalloc (current_اندازه + 1);

  طالما (1)
    {
      s = fgetxattr (sfd, name, بفر, current_اندازه);
      إذا (UNLIKELY (s < 0))
        أرجع crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      إذا (s < current_اندازه)
        اكسر;

      current_اندازه *= 2;
      بفر = xrealloc (بفر, current_اندازه + 1);
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

  لكل (it = buf; it - buf < xattr_len; it += مزي_اوږدوالی (it) + 1)
    {
      cleanup_free حرف *v = عدم;
      ssize_t s;

      s = خوندي_read_xattr (&v, sfd, srcname, it, 256, err);
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

#د_که_پای

ساكن صحيح
copy_rec_stat_file_at (صحيح dfd, ثابت حرف *لار, mode_t *ډېرپېښ, off_t *اندازه, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  بنية stat st;
  صحيح ret;

#که_تعریف_شوی HAVE_STATX
  بنية statx stx = {
    0,
  };

  ret = statx (dfd, لار, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_ډېرپېښ | STATX_اندازه | STATX_UID | STATX_GID, &stx);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS || errno == EINVAL)
        اذهب fallback;

      أرجع ret;
    }

  *ډېرپېښ = stx.stx_mode;
  *اندازه = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  أرجع ret;

fallback:
#د_که_پای
  ret = fstatat (dfd, لار, &st, AT_SYMLINK_NOFOLLOW);

  *ډېرپېښ = st.st_mode;
  *اندازه = st.st_size;
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
      لنډمهاله_FAILURE_RETRY (close (srcdirfd));
      أرجع crun_make_error (err, errno, "open directory `%s`", destname);
    }

  لكل (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close صحيح srcfd = -1;
      cleanup_close صحيح destfd = -1;
      cleanup_free حرف *موخه_buf = عدم;
      صحيح ret;
      mode_t ډېرپېښ;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      إذا (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        استمر;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &ډېرپېښ, &st_size, &rdev, &uid, &gid);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      بدل (ډېرپېښ & S_IFMT)
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

#که_تعریف_شوی HAVE_FGETXATTR
          ret = (صحيح) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
#د_که_پای

          لنډمهاله_FAILURE_RETRY (close (destfd));
          destfd = -1;
          اكسر;

        حالة S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, ډېرپېښ);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          إذا (UNLIKELY (srcfd < 0))
            أرجع crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          إذا (UNLIKELY (destfd < 0))
            أرجع crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#که_تعریف_شوی HAVE_FGETXATTR
          ret = (صحيح) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
#د_که_پای

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_منزل_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          اكسر;

        حالة S_IFLNK:
          ret = خوندي_readlinkat (dirfd (dsrcfd), de->d_name, &موخه_buf, st_size, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          ret = symlinkat (موخه_buf, destdirfd, de->d_name);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          اكسر;

        حالة S_IFBLK:
        حالة S_IFCHR:
        حالة S_IFIFO:
        حالة S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, ډېرپېښ, rdev);
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
#که_نه_تعریف_شوی ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#د_که_پای

      ret = fchmodat (destdirfd, de->d_name, ډېرپېښ & ALLPERMS, AT_SYMLINK_NOFOLLOW);
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
پیدا_کړه_annotation (libcrun_container_t *container, ثابت حرف *name)
{
  إذا (container->container_def->annotations == عدم)
    أرجع عدم;

  أرجع پیدا_کړه_string_map_ارزښت (container->annotations, name);
}

صحيح
خوندي_write (صحيح fd, ثابت حرف *fname, ثابت فراغ *buf, size_t شمېر, libcrun_error_t *err)
{
  size_t written = 0;
  طالما (written < شمېر)
    {
      ssize_t w = اكتب (fd, (ثابت حرف *) buf + written, شمېر - written);
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

#تعریف_کړه MAX_PARTS 32

صحيح
append_paths (حرف **بهر, libcrun_error_t *err, ...)
{
  ثابت حرف *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t ټول_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_پیل (ap, err);
  لكل (;;)
    {
      ثابت حرف *part;
      size_t اندازه;

      part = va_arg (ap, ثابت حرف *);
      إذا (part == عدم)
        اكسر;

      إذا (n_parts == MAX_PARTS)
        {
          va_پای_ټکی (ap);
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

      اندازه = مزي_اوږدوالی (part);
      إذا (اندازه == 0)
        استمر;

      طالما (اندازه > 1 && part[اندازه - 1] == '/')
        اندازه--;

      parts[n_parts] = part;
      sizes[n_parts] = اندازه;

      n_parts++;
    }
  va_پای_ټکی (ap);

  ټول_len = n_parts + 1;
  لكل (i = 0; i < n_parts; i++)
    ټول_len += sizes[i];

  *بهر = xmalloc (ټول_len);

  copied = 0;
  لكل (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*بهر)[copied - 1] == '/';
      إذا (i > 0 && ! has_trailing_slash)
        {
          (*بهر)[copied] = '/';
          copied += 1;
        }

      حافظه_نقل (*بهر + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*بهر)[copied] = '\0';
  أرجع 0;
}

#إذا __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#وإلا
#  define __nonstring
#د_که_پای

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
ساكن صحيح
b64_ننوت (حرف c)
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
  بینښه حرف معلومات[4];

  *nbytes = 0;
  طالما (consumed < isize && (*nbytes) + 3 < osize)
    {
      طالما ((i < 4) && (consumed < isize))
        {
          tmp = b64_ننوت (*iptr++);
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
  بنية passwd *لنډمهاله_پایله_ptr;
  حرف pwdbuffer[200];
  إذا (! getpwuid_r (uid, &pd, pwdbuffer, حجم (pwdbuffer), &لنډمهاله_پایله_ptr))
    أرجع xstrdup (pd.pw_name);
  أرجع xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
حرف *
libcrun_sysctl_کلی_منزل_proc_لار (ثابت حرف *کلی)
{
  ثابت حرف *sep;
  حرف *ret, *it;

  sep = strpbrk (کلی, "./");

  /* No separator at all, or the key is already in the path form.  */
  إذا (sep == عدم || *sep == '/')
    أرجع xstrdup (کلی);

  ret = xstrdup (کلی);
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
  size_t lenstr = مزي_اوږدوالی (str);
  size_t lensuffix = مزي_اوږدوالی (suffix);
  إذا (lensuffix > lenstr)
    أرجع 0;
  أرجع memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

حرف *
str_join_array (صحيح دننه_واټن, size_t اندازه, حرف *ثابت array[], ثابت حرف *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = عدم;
  size_t i, ټول_اندازه = (اندازه - 1) * (jlen = مزي_اوږدوالی (joint)) + 1;
  حرف *پایله, *p;

  lens = xmalloc (اندازه * حجم (*lens));

  لكل (i = 0; i < اندازه; ++i)
    {
      ټول_اندازه += (lens[i] = مزي_اوږدوالی (array[i]));
    }
  p = پایله = xmalloc (ټول_اندازه);
  لكل (i = دننه_واټن; i < اندازه; ++i)
    {
      حافظه_نقل (p, array[i], lens[i]);
      p += lens[i];
      إذا (i < اندازه - 1)
        {
          حافظه_نقل (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  أرجع پایله;
}

صحيح
libcrun_mmap (بنية libcrun_mmap_s **ret, فراغ *addr, size_t اوږدتیا,
              صحيح prot, صحيح flags, صحيح fd, off_t دننه_واټن,
              libcrun_error_t *err)
{
  بنية libcrun_mmap_s *mmap_s = عدم;

  فراغ *mapped = mmap (addr, اوږدتیا, prot, flags, fd, دننه_واټن);
  إذا (mapped == MAP_FAILED)
    أرجع crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (حجم (بنية libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->اوږدتیا = اوږدتیا;

  *ret = mmap_s;

  أرجع 0;
}

صحيح
libcrun_munmap (بنية libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  صحيح ret;

  ret = munmap (mmap->addr, mmap->اوږدتیا);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "munmap");

  حرر (mmap);

  أرجع 0;
}

ساكن اوږد
read_file_as_long_or_default (ثابت حرف *لار, اوږد def_ارزښت)
{
  cleanup_free حرف *content = عدم;
  libcrun_error_t tmp_err = عدم;
  حرف *endptr;
  اوږد val;
  صحيح ret;

  ret = read_all_file (لار, &content, عدم, &tmp_err);
  إذا (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      أرجع def_ارزښت;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  إذا (UNLIKELY (errno))
    أرجع def_ارزښت;
  إذا (endptr == content || (*endptr && *endptr != '\n'))
    أرجع def_ارزښت;
  أرجع val;
}

#تعریف_کړه DEFAULT_ډکېدنه_ID 65534

uid_t
get_ډکېدنه_uid (فراغ)
{
  ساكن atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  إذا (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_ډکېدنه_ID);
      cached_uid = uid;
    }
  أرجع uid;
}

gid_t
get_ډکېدنه_gid (فراغ)
{
  ساكن atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  إذا (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_ډکېدنه_ID);
      cached_gid = gid;
    }
  أرجع gid;
}

فراغ
consume_trailing_slashes (حرف *لار)
{
  إذا (! لار || لار[0] == '\0')
    أرجع;

  حرف *وروستی = لار + مزي_اوږدوالی (لار);

  طالما (وروستی > لار && *(وروستی - 1) == '/')
    وروستی--;

  *وروستی = '\0';
}

حرف **
read_dir_entries (ثابت حرف *لار, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = عدم;
  size_t n_entries = 0;
  size_t entries_اندازه = 16;
  حرف **entries = عدم;
  بنية dirent *de;

  dir = opendir (لار);
  إذا (UNLIKELY (dir == عدم))
    {
      crun_make_error (err, errno, "opendir `%s`", لار);
      أرجع عدم;
    }

  entries = xmalloc (entries_اندازه * حجم (حرف *));
  طالما ((de = readdir (dir)))
    {
      إذا (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        استمر;
      إذا (n_entries == entries_اندازه)
        {
          entries_اندازه *= 2;
          entries = xrealloc (entries, entries_اندازه * حجم (حرف *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * حجم (حرف *));
  entries[n_entries] = عدم;

  أرجع entries;
}

صحيح
cpuset_string_منزل_bitmask (ثابت حرف *str, حرف **بهر, size_t *بهر_اندازه, libcrun_error_t *err)
{
  cleanup_free حرف *ماسک = عدم;
  size_t ماسک_اندازه = 0;
  ثابت حرف *p = str;
  حرف *endptr;

  طالما (*p)
    {
      اوږد اوږد پیل_range, پای_ټکی_range;

      إذا (*p < '0' || *p > '9')
        اذهب invalid_ننوت;

      پیل_range = strtoll (p, &endptr, 10);
      إذا (پیل_range < 0)
        اذهب invalid_ننوت;

      p = endptr;

      إذا (*p != '-')
        پای_ټکی_range = پیل_range;
      وإلا
        {
          p++;

          إذا (*p < '0' || *p > '9')
            اذهب invalid_ننوت;

          پای_ټکی_range = strtoll (p, &endptr, 10);

          إذا (پای_ټکی_range < پیل_range)
            اذهب invalid_ننوت;

          p = endptr;
        }

      /* Just set some limit.  */
      إذا (پای_ټکی_range > (1 << 20))
        اذهب invalid_ننوت;

      إذا (پای_ټکی_range >= (اوږد اوږد) (ماسک_اندازه * CHAR_BIT))
        {
          size_t new_ماسک_اندازه = (پای_ټکی_range / CHAR_BIT) + 1;
          ماسک = xrealloc (ماسک, new_ماسک_اندازه);
          memset (ماسک + ماسک_اندازه, 0, new_ماسک_اندازه - ماسک_اندازه);
          ماسک_اندازه = new_ماسک_اندازه;
        }

      لكل (اوږد اوږد i = پیل_range; i <= پای_ټکی_range; i++)
        ماسک[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      إذا (*p == ',')
        p++;
      وإلا إذا (*p)
        اذهب invalid_ننوت;
    }

  *بهر = ماسک;
  ماسک = عدم;
  *بهر_اندازه = ماسک_اندازه;

  أرجع 0;

invalid_ننوت:
  أرجع crun_make_error (err, 0, "cannot parse input `%s`", str);
}

بنية channel_fd_pair
{
  بنية ring_بفر *rb;

  صحيح دننه_fd;
  صحيح بهر_fd;

  صحيح infd_epoll_events;
  صحيح outfd_epoll_events;
};

بنية channel_fd_pair *
channel_fd_pair_new (صحيح دننه_fd, صحيح بهر_fd, size_t اندازه)
{
  بنية channel_fd_pair *channel = xmalloc (حجم (بنية channel_fd_pair));
  channel->دننه_fd = دننه_fd;
  channel->بهر_fd = بهر_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_بفر_make (اندازه);
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
  bool is_ننوت_eagain = false, is_output_eagain = false, repeat;
  صحيح ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  لكل (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      إذا (ring_بفر_get_space_شته (channel->rb) > 0)
        {
          ret = ring_بفر_read (channel->rb, channel->دننه_fd, &is_ننوت_eagain, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          إذا (ret > 0)
            repeat = true;
        }
      إذا (ring_بفر_get_معلومات_شته (channel->rb) > 0)
        {
          ret = ring_بفر_write (channel->rb, channel->بهر_fd, &is_output_eagain, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          إذا (ret > 0)
            repeat = true;
        }
    }

  إذا (epollfd >= 0)
    {
      size_t شته = ring_بفر_get_space_شته (channel->rb);
      size_t کارول_شوی = ring_بفر_get_معلومات_شته (channel->rb);
      صحيح events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (شته > 0 && ! is_output_eagain) ? (EPOLLIN | (is_ننوت_eagain ? EPOLLET : 0)) : 0;
      إذا (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->دننه_fd, events, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (کارول_شوی > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      إذا (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->بهر_fd, events, err);
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

#که_تعریف_شوی HAVE_NEW_MOUNT_API
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
#د_که_پای

  fd = open ("/proc", O_لار | O_DIRECTORY | O_CLOEXEC);
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
libcrun_open_proc_file (libcrun_container_t *container, ثابت حرف *لار, صحيح flags, libcrun_error_t *err)
{
  صحيح proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  إذا (proc_fd < 0)
    أرجع proc_fd;

  fd = لنډمهاله_FAILURE_RETRY (openat (proc_fd, لار, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  إذا (! container)
    لنډمهاله_FAILURE_RETRY (close (proc_fd));

  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, saved_errno, "openat `/proc/%s`", لار);

  أرجع fd;
}

صحيح
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ثابت حرف *لار, صحيح flags, libcrun_error_t *err)
{
  cleanup_free حرف *ډک_لار = عدم;
  xasprintf (&ډک_لار, "%d/%s", pid, لار);
  أرجع libcrun_open_proc_file (container, ډک_لار, flags, err);
}
