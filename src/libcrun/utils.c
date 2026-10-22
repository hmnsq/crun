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

#నిర్వచించు GNU_ఆధారం
#అన్తర్భావ <config.h>
#అన్తర్భావ "utils.h"
#అన్తర్భావ "ring_buffer.h"
#అన్తర్భావ <stdarg.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <libgen.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/un.h>
#అన్తర్భావ <sys/socket.h>
#అన్తర్భావ <sys/signalfd.h>
#అన్తర్భావ <sys/epoll.h>
#అన్తర్భావ <sys/syscall.h>
#అన్తర్భావ <sys/wait.h>
#అన్తర్భావ <pwd.h>
#అన్తర్భావ <grp.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <sys/time.h>
#అన్తర్భావ <sys/xattr.h>
#అన్తర్భావ <sys/sysmacros.h>
#అన్తర్భావ <sys/vfs.h>
#అన్తర్భావ <linux/magic.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <sys/mman.h>
#నిర్వచితమైతే HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#లేకపోతే
#  define atomic_long అస్థిర దీర్ఘ
#  define atomic_int అస్థిర పూర్ణాంకం
#అయితే_ముగింపు
#అన్తర్భావ "syscalls.h"

#నిర్వచితం_కాకపోతే CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే RESOLVE_లోపల_మూలం
#  define RESOLVE_లోపల_మూలం 0x10
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే __NR_close_range
#  define __NR_close_range 436
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే __NR_openat2
#  define __NR_openat2 437
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే PROC_USER_INIT_INO
#  define PROC_USER_INIT_INO ((ino_t) 0xEFFFFFFD)
#అయితే_ముగింపు

#నిర్వచించు MAX_READLINKS 32
#నిర్వచించు MAX_OPENAT2_EAGAIN_RETRIES 128

పూర్ణాంకం
crun_మార్గం_exists (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  పూర్ణాంకం ret = access (మార్గం, F_OK);
  ఒకవేళ (ret < 0)
    {
      ఒకవేళ (errno == ENOENT)
        తిరిగి 0;
      తిరిగి crun_make_error (err, errno, "access `%s`", మార్గం);
    }
  తిరిగి 1;
}

పూర్ణాంకం
xasprintf (అక్షరం **str, స్థిరరాశి అక్షరం *fmt, ...)
{
  పూర్ణాంకం ret;
  va_list వాదనలు_list;

  va_ప్రారంభం (వాదనలు_list, fmt);

  ret = vasprintf (str, fmt, వాదనలు_list);
  ఒకవేళ (UNLIKELY (ret < 0))
    OOM ();

  va_చివర (వాదనలు_list);
  తిరిగి ret;
}

పూర్ణాంకం
write_file_at_with_flags (పూర్ణాంకం dirfd, పూర్ణాంకం flags, mode_t బహుళకం, స్థిరరాశి అక్షరం *name, స్థిరరాశి శూన్య *దత్తాంశం, size_t len, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం fd = -1;
  పూర్ణాంకం ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, బహుళకం);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s` for writing", name);

  ret = సురక్షిత_write (fd, name, దత్తాంశం, len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి (len > గరిష్ఠ_పూర్ణాంకం) ? గరిష్ఠ_పూర్ణాంకం : (పూర్ణాంకం) len;
}

పూర్ణాంకం
detach_process ()
{
  pid_t pid;
  ఒకవేళ (setsid () < 0)
    తిరిగి -1;
  pid = fork ();
  ఒకవేళ (pid < 0)
    తిరిగి -1;
  ఒకవేళ (pid != 0)
    సురక్షిత_exit (EXIT_SUCCESS);
  తిరిగి 0;
}

పూర్ణాంకం
get_file_type_fd (పూర్ణాంకం fd, mode_t *బహుళకం)
{
  నిర్మాణం stat st;
  పూర్ణాంకం ret;

#నిర్వచితమైతే HAVE_STATX
  నిర్మాణం statx stx = {
    0,
  };

  ret = statx (fd, "", AT_ఖాళీలు_మార్గం | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno == ENOSYS || errno == EINVAL)
        వెళ్ళు fallback;

      తిరిగి ret;
    }
  *బహుళకం = stx.stx_mode;
  తిరిగి ret;

fallback:
#అయితే_ముగింపు
  ret = fstat (fd, &st);
  *బహుళకం = st.st_mode;
  తిరిగి ret;
}

పూర్ణాంకం
get_file_type_at (పూర్ణాంకం dirfd, mode_t *బహుళకం, bool nofollow, స్థిరరాశి అక్షరం *మార్గం)
{
  పూర్ణాంకం ఖాళీలు_మార్గం = మార్గం == ఖాళీ ? AT_ఖాళీలు_మార్గం : 0;
  నిర్మాణం stat st;
  పూర్ణాంకం ret;

#నిర్వచితమైతే HAVE_STATX
  నిర్మాణం statx stx = {
    0,
  };

  ret = statx (dirfd, మార్గం ?: "", ఖాళీలు_మార్గం | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno == ENOSYS || errno == EINVAL)
        వెళ్ళు fallback;

      తిరిగి ret;
    }
  *బహుళకం = stx.stx_mode;
  తిరిగి ret;

fallback:
#అయితే_ముగింపు
  ret = fstatat (dirfd, మార్గం ?: "", &st, ఖాళీలు_మార్గం | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *బహుళకం = st.st_mode;
  తిరిగి ret;
}

పూర్ణాంకం
get_file_type (mode_t *బహుళకం, bool nofollow, స్థిరరాశి అక్షరం *మార్గం)
{
  తిరిగి get_file_type_at (AT_FDCWD, బహుళకం, nofollow, మార్గం);
}

పూర్ణాంకం
create_file_if_missing_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *file, mode_t బహుళకం, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, బహుళకం);
  ఒకవేళ (fd_write < 0)
    {
      పూర్ణాంకం saved_errno = errno;
      mode_t tmp_బహుళకం;
      పూర్ణాంకం ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_బహుళకం, true, file);
      ఒకవేళ (ret == 0 && S_ISREG (tmp_బహుళకం))
        తిరిగి 0;

      తిరిగి crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
ensure_directory_అంతర్గత_at (పూర్ణాంకం dirfd, అక్షరం *మార్గం, size_t len, పూర్ణాంకం బహుళకం, libcrun_error_t *err)
{
  అక్షరం *it = మార్గం + len;
  పూర్ణాంకం ret = 0;
  bool మాతృ_created = false;

  కొరకు (;;)
    {
      ret = mkdirat (dirfd, మార్గం, బహుళకం);
      ఒకవేళ (ret == 0 || errno == EEXIST)
        తిరిగి 0;

      పూర్ణాంకం saved_errno = errno;
      ఒకవేళ (మాతృ_created || errno != ENOENT)
        {
          libcrun_error_t tmp_err = ఖాళీ;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (మార్గం, false, &tmp_err);
          ఒకవేళ (ret > 0)
            తిరిగి 0;
          ఒకవేళ (ret < 0)
            crun_error_release (&tmp_err);

          తిరిగి crun_make_error (err, saved_errno, "create directory `%s`", మార్గం);
        }

      అయితే (it > మార్గం && *it != '/')
        {
          it--;
          len--;
        }
      ఒకవేళ (it == మార్గం)
        తిరిగి 0;

      *it = '\0';
      ret = ensure_directory_అంతర్గత_at (dirfd, మార్గం, len - 1, బహుళకం, err);
      *it = '/';
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      మాతృ_created = true;
    }
  తిరిగి ret;
}

పూర్ణాంకం
crun_ensure_directory_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం, bool nofollow, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_free అక్షరం *tmp = xstrdup (మార్గం);
  ret = ensure_directory_అంతర్గత_at (dirfd, tmp, అక్షరశ్రేణి_పొడవు (tmp), బహుళకం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = crun_dir_p_at (dirfd, మార్గం, nofollow, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (ret == 0)
    తిరిగి crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", మార్గం);

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
check_fd_is_మార్గం (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం fd, స్థిరరాశి అక్షరం *fdname, libcrun_error_t *err)
{
  proc_fd_మార్గం_t fdpath;
  size_t మార్గం_len = అక్షరశ్రేణి_పొడవు (మార్గం);
  అక్షరం link[PATH_MAX];
  పూర్ణాంకం ret;

  get_proc_self_fd_మార్గం (fdpath, fd);
  ret = తాత్కాలిక_FAILURE_RETRY (readlink (fdpath, link, పరిమాణం (link)));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "readlink `%s`", fdname);

  ఒకవేళ (((size_t) ret) != మార్గం_len || memcmp (link, మార్గం, మార్గం_len))
    తిరిగి crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, మార్గం);

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
check_fd_under_మార్గం (స్థిరరాశి అక్షరం *rootfs, size_t rootfslen, పూర్ణాంకం fd, స్థిరరాశి అక్షరం *fdname, libcrun_error_t *err)
{
  proc_fd_మార్గం_t fdpath;
  అక్షరం link[PATH_MAX];
  పూర్ణాంకం ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  ఒకవేళ (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    తిరిగి 0;

  get_proc_self_fd_మార్గం (fdpath, fd);
  ret = తాత్కాలిక_FAILURE_RETRY (readlink (fdpath, link, పరిమాణం (link)));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "readlink `%s`", fdname);

  ఒకవేళ (((size_t) ret) <= rootfslen || memcmp (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    తిరిగి crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  తిరిగి 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
స్థిర శూన్య
close_and_replace (పూర్ణాంకం *oldfd, పూర్ణాంకం newfd)
{
  ఒకవేళ (*oldfd >= 0)
    తాత్కాలిక_FAILURE_RETRY (close (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
అక్షరం *chroot_realpath (స్థిరరాశి అక్షరం *chroot, స్థిరరాశి అక్షరం *మార్గం, అక్షరం resolved_మార్గం[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
స్థిర పూర్ణాంకం
సురక్షిత_openat_fallback (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *rootfs, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం flags,
                      పూర్ణాంకం బహుళకం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *మాతృ_మార్గం = ఖాళీ;
  స్థిరరాశి అక్షరం *చివరి_component = ఖాళీ;
  స్థిరరాశి అక్షరం *orig_మార్గం = మార్గం;
  స్థిరరాశి అక్షరం *మార్గం_లోపల_chroot;
  cleanup_close పూర్ణాంకం fd = -1;
  అక్షరం resolved[PATH_MAX];
  అక్షరం బఫరు[PATH_MAX];
  size_t rootfs_len = సురక్షిత_strlen (rootfs);
  పూర్ణాంకం ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  ఒకవేళ (flags & O_NOFOLLOW)
    {
      అక్షరం *sep;

      మాతృ_మార్గం = xstrdup (మార్గం);
      sep = strrchr (మాతృ_మార్గం, '/');
      ఒకవేళ (sep == ఖాళీ)
        {
          /* No parent directory, the entire path is the last component.  */
          చివరి_component = మార్గం;
          మాతృ_మార్గం[0] = '\0';
        }
      లేకపోతే ఒకవేళ (sep[1] != '\0')
        {
          *sep = '\0';
          చివరి_component = మార్గం + (sep - మాతృ_మార్గం) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      ఒకవేళ (చివరి_component)
        మార్గం = మాతృ_మార్గం;
    }

  మార్గం_లోపల_chroot = chroot_realpath (rootfs, మార్గం, బఫరు);
  ఒకవేళ (మార్గం_లోపల_chroot == ఖాళీ)
    తిరిగి crun_make_error (err, errno, "cannot resolve `%s` under rootfs", orig_మార్గం);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  ఒకవేళ (rootfs_len > 0 && అక్షరశ్రేణి_పోలిక (మార్గం_లోపల_chroot, rootfs, rootfs_len) == 0)
    మార్గం_లోపల_chroot += rootfs_len;
  మార్గం_లోపల_chroot = consume_slashes (మార్గం_లోపల_chroot);

  ఒకవేళ (చివరి_component)
    {
      ret = snprintf (resolved, పరిమాణం (resolved), "%s%s%s", మార్గం_లోపల_chroot,
                      మార్గం_లోపల_chroot[0] == '\0' ? "" : "/", చివరి_component);
      ఒకవేళ (UNLIKELY (ret >= (పూర్ణాంకం) పరిమాణం (resolved)))
        తిరిగి crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_మార్గం);

      మార్గం_లోపల_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  ఒకవేళ (మార్గం_లోపల_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "dup `%s`", rootfs_len ? rootfs : "/");
      తిరిగి ret;
    }

  ret = openat (dirfd, మార్గం_లోపల_chroot, flags, బహుళకం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", orig_మార్గం);

  fd = ret;

  ret = check_fd_under_మార్గం (rootfs, rootfs_len, fd, orig_మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = fd;
  fd = -1;
  తిరిగి ret;
}

పూర్ణాంకం
సురక్షిత_openat (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *rootfs, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం flags, పూర్ణాంకం బహుళకం,
             libcrun_error_t *err)
{
  స్థిర bool openat2_supported = true;
  పూర్ణాంకం retries = MAX_OPENAT2_EAGAIN_RETRIES;
  పూర్ణాంకం ret;

  ఒకవేళ (is_ఖాళీలు_string (మార్గం))
    {
      cleanup_close పూర్ణాంకం fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      ఒకవేళ (is_ఖాళీలు_string (rootfs))
        rootfs = "/";

      fd = open (rootfs, flags, బహుళకం);
      ఒకవేళ (UNLIKELY (fd < 0))
        తిరిగి crun_make_error (err, errno, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      ఒకవేళ (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_మార్గం (rootfs, fd, మార్గం, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ret = fd;
      fd = -1;
      తిరిగి ret;
    }

  ఒకవేళ (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, మార్గం, flags, బహుళకం, RESOLVE_లోపల_మూలం);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (errno == EINTR)
            వెళ్ళు repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          ఒకవేళ (errno == EAGAIN && retries-- > 0)
            వెళ్ళు repeat;
          ఒకవేళ (errno == ENOSYS)
            openat2_supported = false;
          ఒకవేళ (errno == ENOSYS || errno == EINVAL || errno == EPERM)
            తిరిగి సురక్షిత_openat_fallback (dirfd, rootfs, మార్గం, flags, బహుళకం, err);

          తిరిగి crun_make_error (err, errno, "openat2 `%s`", మార్గం);
        }

      తిరిగి ret;
    }

  తిరిగి సురక్షిత_openat_fallback (dirfd, rootfs, మార్గం, flags, బహుళకం, err);
}

ssize_t
సురక్షిత_readlinkat (పూర్ణాంకం dfd, స్థిరరాశి అక్షరం *name, అక్షరం **బఫరు, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_కొలత = hint > 0 ? hint + 1 : 512;
  cleanup_free అక్షరం *tmp_buf = ఖాళీ;
  ssize_t కొలత;

  చేయి
    {
      ఒకవేళ (tmp_buf != ఖాళీ)
        buf_కొలత += 256;

      tmp_buf = xrealloc (tmp_buf, buf_కొలత);

      కొలత = readlinkat (dfd, name, tmp_buf, buf_కొలత);
      ఒకవేళ (UNLIKELY (కొలత < 0))
        తిరిగి crun_make_error (err, errno, "readlink `%s`", name);
  } అయితే (కొలత == buf_కొలత);

  /* Always NUL terminate the buffer.  */
  tmp_buf[కొలత] = '\0';

  /* Move ownership to BUFFER.  */
  *బఫరు = tmp_buf;
  tmp_buf = ఖాళీ;

  తిరిగి కొలత;
}

స్థిర పూర్ణాంకం crun_సురక్షిత_ensure_at (bool do_open, bool dir, bool follow_symlinks, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *dirpath,
                                స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం, పూర్ణాంకం max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
స్థిర పూర్ణాంకం
crun_సురక్షిత_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *dirpath, పూర్ణాంకం cwd,
                                 స్థిరరాశి అక్షరం *npath, స్థిరరాశి అక్షరం *cur, స్థిరరాశి అక్షరం *rest,
                                 పూర్ణాంకం బహుళకం, పూర్ణాంకం max_readlinks, libcrun_error_t *err)
{
  cleanup_free అక్షరం *లక్ష్యం = ఖాళీ;
  cleanup_free అక్షరం *new_మార్గం = ఖాళీ;
  ssize_t ret;

  ret = సురక్షిత_readlinkat (cwd, cur, &లక్ష్యం, 0, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_మార్గం, "%.*s%s%s%s", లక్ష్యం[0] == '/' ? 0 : (పూర్ణాంకం) (cur - npath), npath, లక్ష్యం,
             rest ? "/" : "", rest ? rest : "");

  తిరిగి crun_సురక్షిత_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_మార్గం, బహుళకం, max_readlinks - 1, err);
}

స్థిర పూర్ణాంకం
crun_సురక్షిత_ensure_at (bool do_open, bool dir, bool follow_symlinks, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *dirpath,
                     స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం, పూర్ణాంకం max_readlinks, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం wd_cleanup = -1;
  cleanup_free అక్షరం *npath = ఖాళీ;
  bool చివరి_component = false;
  size_t లోతు = 0;
  అక్షరం *cur;
  అక్షరం *it;
  పూర్ణాంకం cwd;
  పూర్ణాంకం ret;

  ఒకవేళ (max_readlinks <= 0)
    తిరిగి crun_make_error (err, ELOOP, "resolve path `%s`", మార్గం);

  మార్గం = consume_slashes (మార్గం);

  /* Empty path, nothing to do.  */
  ఒకవేళ (*మార్గం == '\0')
    {
      ఒకవేళ (do_open)
        తిరిగి open (dirpath, O_CLOEXEC | O_మార్గం, 0);
      తిరిగి 0;
    }

  npath = xstrdup (మార్గం);

  cwd = dirfd;
  cur = npath;
  it = strchr (npath, '/');
  అయితే (cur)
    {
      ఒకవేళ (it)
        *it = '\0';
      లేకపోతే
        చివరి_component = true;

      ఒకవేళ (cur[0] == '\0')
        ఆపు;

      ఒకవేళ (strcmp (cur, ".") == 0)
        వెళ్ళు తదుపరి;
      లేకపోతే ఒకవేళ (strcmp (cur, ".."))
        లోతు++;
      లేకపోతే
        {
          ఒకవేళ (లోతు)
            లోతు--;
          లేకపోతే
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              వెళ్ళు తదుపరి;
            }
        }

      ఒకవేళ (చివరి_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              ఒకవేళ (errno == ELOOP)
                తిరిగి crun_సురక్షిత_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, ఖాళీ,
                                                        బహుళకం, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_మార్గం | O_NOFOLLOW, 0);
              ఒకవేళ (ret < 0)
                తిరిగి crun_make_error (err, errno, "open `%s/%s`", dirpath, npath);
            }

          ఒకవేళ (do_open)
            తిరిగి ret;

          close_and_replace (&wd_cleanup, ret);
          తిరిగి 0;
        }

      ret = mkdirat (cwd, cur, బహుళకం);
      ఒకవేళ (ret < 0)
        {
          mode_t st_mode;

          ఒకవేళ (errno != EEXIST)
            తిరిగి crun_make_error (err, errno, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          ఒకవేళ (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            తిరిగి crun_సురక్షిత_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : ఖాళీ, బహుళకం, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = సురక్షిత_openat (dirfd, dirpath, npath, O_మార్గం | O_CLOEXEC, 0, err);
      ఒకవేళ (UNLIKELY (cwd < 0))
        తిరిగి crun_error_wrap (err, "creating `/%s`", మార్గం);

      ఒకవేళ (! చివరి_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, ఖాళీ);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              పూర్ణాంకం saved_errno = errno;

              close (cwd);
              తిరిగి crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          ఒకవేళ ((st_mode & S_IFMT) != S_IFDIR)
            {
              close (cwd);
              తిరిగి crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", మార్గం, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    తదుపరి:
      ఒకవేళ (it == ఖాళీ)
        ఆపు;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }

  ఒకవేళ (do_open)
    {
      ఒకవేళ (cwd == dirfd)
        {
          ret = dup (dirfd);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "dup `%s`", dirpath);
          తిరిగి ret;
        }

      wd_cleanup = -1;
      తిరిగి cwd;
    }

  తిరిగి 0;
}

పూర్ణాంకం
crun_సురక్షిత_create_and_open_ref_at (bool dir, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *dirpath, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  /* If the file/dir already exists, just open it.  */
  ret = సురక్షిత_openat (dirfd, dirpath, మార్గం, O_మార్గం | O_CLOEXEC, 0, err);
  ఒకవేళ (LIKELY (ret >= 0))
    తిరిగి ret;

  crun_error_release (err);
  తిరిగి crun_సురక్షిత_ensure_at (true, dir, true, dirfd, dirpath, మార్గం, బహుళకం, MAX_READLINKS, err);
}

పూర్ణాంకం
crun_సురక్షిత_ensure_directory_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *dirpath, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం,
                               libcrun_error_t *err)
{
  తిరిగి crun_సురక్షిత_ensure_at (false, true, false, dirfd, dirpath, మార్గం, బహుళకం, MAX_READLINKS, err);
}

పూర్ణాంకం
crun_సురక్షిత_ensure_file_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *dirpath, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం,
                          libcrun_error_t *err)
{
  తిరిగి crun_సురక్షిత_ensure_at (false, false, false, dirfd, dirpath, మార్గం, బహుళకం, MAX_READLINKS, err);
}

పూర్ణాంకం
crun_ensure_directory (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం, bool nofollow, libcrun_error_t *err)
{
  తిరిగి crun_ensure_directory_at (AT_FDCWD, మార్గం, బహుళకం, nofollow, err);
}

స్థిర పూర్ణాంకం
get_file_కొలత (పూర్ణాంకం fd, off_t *కొలత)
{
  నిర్మాణం stat st;
  పూర్ణాంకం ret;
#నిర్వచితమైతే HAVE_STATX
  నిర్మాణం statx stx = {
    0,
  };

  ret = statx (fd, "", AT_ఖాళీలు_మార్గం | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_కొలత, &stx);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno == ENOSYS || errno == EINVAL)
        వెళ్ళు fallback;
      తిరిగి ret;
    }
  *కొలత = stx.stx_size;

  తిరిగి ret;

fallback:
#అయితే_ముగింపు
  ret = fstat (fd, &st);
  *కొలత = st.st_size;
  తిరిగి ret;
}

పూర్ణాంకం
crun_dir_p_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *మార్గం, bool nofollow, libcrun_error_t *err)
{
  mode_t బహుళకం;
  పూర్ణాంకం ret;

  ret = get_file_type_at (dirfd, &బహుళకం, nofollow, మార్గం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "stat `%s`", మార్గం);

  తిరిగి S_ISDIR (బహుళకం) ? 1 : 0;
}

పూర్ణాంకం
crun_dir_p (స్థిరరాశి అక్షరం *మార్గం, bool nofollow, libcrun_error_t *err)
{
  తిరిగి crun_dir_p_at (AT_FDCWD, మార్గం, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
స్థిర పూర్ణాంకం
check_running_లోపల_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  size_t len;
  పూర్ణాంకం ret;

  ret = read_all_file ("/proc/self/uid_map", &బఫరు, &len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      ఒకవేళ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          తిరిగి 0;
        }
      తిరిగి ret;
    }

  తిరిగి అక్షరశ్రేణి_శోధన (బఫరు, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
స్థిర bool
running_లోపల_openvz ()
{
  ఒకవేళ (access ("/proc/vz", F_OK) < 0)
    తిరిగి false;

  తిరిగి access ("/proc/bc", F_OK) < 0 && errno == ENOENT;
}

పూర్ణాంకం
check_running_లోపల_user_namespace (libcrun_error_t *err)
{
  స్థిర atomic_int నిరంతరం_లోపల_userns = -1;
  నిర్మాణం stat st;
  పూర్ణాంకం ret;

  ret = నిరంతరం_లోపల_userns;
  ఒకవేళ (ret >= 0)
    తిరిగి ret;

  ret = stat ("/proc/self/ns/user", &st);
  ఒకవేళ (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      ఒకవేళ (st.st_ino == PROC_USER_INIT_INO)
        {
          నిరంతరం_లోపల_userns = 0;
          తిరిగి నిరంతరం_లోపల_userns;
        }

      ఒకవేళ (! running_లోపల_openvz ())
        {
          నిరంతరం_లోపల_userns = 1;
          తిరిగి నిరంతరం_లోపల_userns;
        }
    }
  లేకపోతే ఒకవేళ (UNLIKELY (errno != ENOENT))
    తిరిగి crun_make_error (err, errno, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_లోపల_user_namespace_uid_map (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  నిరంతరం_లోపల_userns = ret;
  తిరిగి నిరంతరం_లోపల_userns;
}

స్థిర size_t
get_పుట_కొలత ()
{
  స్థిర atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  ఒకవేళ (pagesize == 0)
    {
      pagesize = (దీర్ఘ) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  తిరిగి (size_t) pagesize;
}

స్థిర పూర్ణాంకం selinux_enabled = -1;
స్థిర పూర్ణాంకం apparmor_enabled = -1;

పూర్ణాంకం
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free అక్షరం *బహిర్గతం = ఖాళీ;
  cleanup_close పూర్ణాంకం fd = -1;
  size_t len;
  పూర్ణాంకం ret;

  ఒకవేళ (selinux_enabled >= 0)
    తిరిగి selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి fd;

  ret = read_all_fd_with_కొలత_hint (fd, "mounts", &బహిర్గతం, &len, get_పుట_కొలత (), err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  selinux_enabled = అక్షరశ్రేణి_శోధన (బహిర్గతం, "selinux") ? 1 : 0;

  తిరిగి selinux_enabled;
}

పూర్ణాంకం
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం fd = -1;
  పూర్ణాంకం కొలత;
  అక్షరం buf[1];

  ఒకవేళ (apparmor_enabled >= 0)
    తిరిగి apparmor_enabled;

  fd = open ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  ఒకవేళ (fd == -1)
    {
      ఒకవేళ (errno == ENOENT)
        {
          apparmor_enabled = 0;
          తిరిగి 0;
        }

      తిరిగి crun_make_error (err, errno, "open `/sys/module/apparmor/parameters/enabled`");
    }

  కొలత = తాత్కాలిక_FAILURE_RETRY (పఠ (fd, buf, పరిమాణం (buf)));

  apparmor_enabled = కొలత > 0 && buf[0] == 'Y' ? 1 : 0;

  తిరిగి apparmor_enabled;
}

స్థిర పూర్ణాంకం
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  ఒకవేళ (selinux_enabled < 0)
    తిరిగి crun_make_error (err, 0, "SELinux is not initialized correctly");
  తిరిగి selinux_enabled;
}

పూర్ణాంకం
కూడు_selinux_mount_label (అక్షరం **retlabel, స్థిరరాశి అక్షరం *దత్తాంశం, స్థిరరాశి అక్షరం *label, స్థిరరాశి అక్షరం *context_type, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = libcrun_is_selinux_enabled (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (label && ret)
    {
      ఒకవేళ (దత్తాంశం && *దత్తాంశం)
        xasprintf (retlabel, "%s,%s=\"%s\"", దత్తాంశం, context_type, label);
      లేకపోతే
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      తిరిగి 0;
    }
  *retlabel = xstrdup (దత్తాంశం);
  తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *
lsm_attr_మార్గం (libcrun_container_t *container, స్థిరరాశి అక్షరం *lsm, స్థిరరాశి అక్షరం *fname, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం attr_dirfd = -1;
  cleanup_close పూర్ణాంకం lsm_dirfd = -1;
  అక్షరం *attr_మార్గం = ఖాళీ;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_మార్గం, err);
  ఒకవేళ (UNLIKELY (attr_dirfd < 0))
    తిరిగి ఖాళీ;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  ఒకవేళ (lsm != ఖాళీ)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_మార్గం | O_CLOEXEC);

      ఒకవేళ (UNLIKELY (lsm_dirfd < 0 && errno != ENOENT))
        {
          crun_make_error (err, errno, "open `/proc/thread-self/attr/%s`", lsm);
          తిరిగి ఖాళీ;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_మార్గం, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  తిరిగి attr_మార్గం;
}

స్థిర పూర్ణాంకం
check_proc_super_magic (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  నిర్మాణం statfs sfs;

  పూర్ణాంకం ret = fstatfs (fd, &sfs);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "statfs `%s`", మార్గం);

  ఒకవేళ (sfs.f_type != PROC_SUPER_MAGIC)
    తిరిగి crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", మార్గం);

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
సమితి_security_attr (libcrun_container_t *container, స్థిరరాశి అక్షరం *lsm, స్థిరరాశి అక్షరం *fname, స్థిరరాశి అక్షరం *దత్తాంశం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  cleanup_free స్థిరరాశి అక్షరం *attr_మార్గం = lsm_attr_మార్గం (container, lsm, fname, err);
  cleanup_close పూర్ణాంకం fd = -1;

  ఒకవేళ (UNLIKELY (attr_మార్గం == ఖాళీ))
    తిరిగి -1;

  fd = libcrun_open_proc_file (container, attr_మార్గం, O_WRONLY | O_CLOEXEC, err);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి fd;

  ret = check_proc_super_magic (fd, attr_మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  // Write out data
  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, దత్తాంశం, అక్షరశ్రేణి_పొడవు (దత్తాంశం)));
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      ఒకవేళ (errno == ENOENT && lsm != ఖాళీ && strcmp (lsm, "apparmor") == 0)
        {
          స్థిరరాశి అక్షరం *profile = strchr (దత్తాంశం, ' ');

          తిరిగి crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : దత్తాంశం);
        }
      తిరిగి crun_make_error (err, errno, "write to file `%s`", attr_మార్గం);
    }

  తిరిగి 0;
}

పూర్ణాంకం
సమితి_selinux_label (libcrun_container_t *container, స్థిరరాశి అక్షరం *label, bool now, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = libcrun_is_selinux_enabled (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (ret)
    తిరిగి సమితి_security_attr (container, ఖాళీ, now ? "current" : "exec", label, err);
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  ఒకవేళ (apparmor_enabled < 0)
    తిరిగి crun_make_error (err, 0, "AppArmor is not initialized correctly");
  తిరిగి apparmor_enabled;
}

స్థిర పూర్ణాంకం
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#నిర్వచించు UNCONFINED "unconfined"
#నిర్వచించు UNCONFINED_LEN (ssize_t) (పరిమాణం (UNCONFINED) - 1)
  cleanup_free స్థిరరాశి అక్షరం *attr_మార్గం = lsm_attr_మార్గం (container, "apparmor", "current", err);
  cleanup_close పూర్ణాంకం fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  అక్షరం buf[UNCONFINED_LEN + 1];

  ఒకవేళ (UNLIKELY (attr_మార్గం == ఖాళీ))
    తిరిగి -1;

  fd = libcrun_open_proc_file (container, attr_మార్గం, O_RDONLY | O_CLOEXEC, err);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి fd;

  ఒకవేళ (UNLIKELY (check_proc_super_magic (fd, attr_మార్గం, err)))
    తిరిగి -1;

  ssize_t బైట్లు_read = తాత్కాలిక_FAILURE_RETRY (పఠ (fd, buf, పరిమాణం (buf)));
  ఒకవేళ (UNLIKELY (బైట్లు_read < 0))
    తిరిగి crun_make_error (err, errno, "read from `%s`", attr_మార్గం);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  ఒకవేళ (బైట్లు_read < UNCONFINED_LEN || memcmp (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    తిరిగి 1;
  ఒకవేళ (బైట్లు_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    తిరిగి 0;
  తిరిగి 1;
#నిర్వచనం_రద్దు UNCONFINED
#నిర్వచనం_రద్దు UNCONFINED_LEN
}

పూర్ణాంకం
సమితి_apparmor_profile (libcrun_container_t *container, స్థిరరాశి అక్షరం *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = libcrun_is_apparmor_enabled (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (ret)
    {
      cleanup_free అక్షరం *buf = ఖాళీ;
      ret = is_current_process_confined (container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      తిరిగి సమితి_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
read_all_fd_అంతర్గత (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *description, అక్షరం **బహిర్గతం, size_t *len, size_t కొలత_hint, size_t max_కొలత, libcrun_error_t *err)
{
  cleanup_free అక్షరం *buf = ఖాళీ;
  size_t nread, allocated;
  size_t pagesize = 0;
  off_t కొలత = 0;
  పూర్ణాంకం ret;

  ఒకవేళ (కొలత_hint)
    allocated = కొలత_hint;
  లేకపోతే
    {
      ret = get_file_కొలత (fd, &కొలత);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "stat `%s`", description);

      ఒకవేళ (max_కొలత && (size_t) కొలత > max_కొలత)
        తిరిగి crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_కొలత);

      allocated = కొలత == 0 ? 1023 : కొలత;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  అయితే ((కొలత && nread < (size_t) కొలత) || కొలత == 0)
    {
      ret = తాత్కాలిక_FAILURE_RETRY (పఠ (fd, buf + nread, allocated - nread));
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "read from file `%s`", description);

      ఒకవేళ (ret == 0)
        ఆపు;

      nread += ret;

      ఒకవేళ (max_కొలత && nread > max_కొలత)
        తిరిగి crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_కొలత);

      ఒకవేళ (nread == allocated)
        {
          ఒకవేళ (కొలత)
            ఆపు;

          ఒకవేళ (pagesize == 0)
            pagesize = get_పుట_కొలత ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  ఒకవేళ (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      అక్షరం *tmp = realloc (buf, nread + 1);
      ఒకవేళ (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *బహిర్గతం = buf;
  buf = ఖాళీ;
  ఒకవేళ (len)
    *len = nread;
  తిరిగి 0;
}

పూర్ణాంకం
read_all_fd_with_కొలత_hint (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *description, అక్షరం **బహిర్గతం, size_t *len, size_t కొలత_hint, libcrun_error_t *err)
{
  తిరిగి read_all_fd_అంతర్గత (fd, description, బహిర్గతం, len, కొలత_hint, 0, err);
}

పూర్ణాంకం
read_all_fd_with_max_కొలత (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *description, అక్షరం **బహిర్గతం, size_t *len, size_t max_కొలత, libcrun_error_t *err)
{
  తిరిగి read_all_fd_అంతర్గత (fd, description, బహిర్గతం, len, 0, max_కొలత, err);
}

పూర్ణాంకం
read_all_file_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *మార్గం, అక్షరం **బహిర్గతం, size_t *len, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం fd = -1;

  fd = తాత్కాలిక_FAILURE_RETRY (openat (dirfd, మార్గం, O_RDONLY | O_CLOEXEC));
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", మార్గం);

  తిరిగి read_all_fd (fd, మార్గం, బహిర్గతం, len, err);
}

పూర్ణాంకం
read_all_file (స్థిరరాశి అక్షరం *మార్గం, అక్షరం **బహిర్గతం, size_t *len, libcrun_error_t *err)
{
  ఒకవేళ (strcmp (మార్గం, "-") == 0)
    మార్గం = "/dev/stdin";

  తిరిగి read_all_file_at (AT_FDCWD, మార్గం, బహిర్గతం, len, err);
}

పూర్ణాంకం
get_realpath_గమ్యం_file (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *మార్గం_name, అక్షరం **absolute_మార్గం, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం targetfd = -1;

  targetfd = తాత్కాలిక_FAILURE_RETRY (openat (dirfd, మార్గం_name, O_RDONLY | O_CLOEXEC));
  ఒకవేళ (UNLIKELY (targetfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", మార్గం_name);
  లేకపోతే
    {
      ssize_t len;
      proc_fd_మార్గం_t లక్ష్యం_fd_మార్గం;

      get_proc_self_fd_మార్గం (లక్ష్యం_fd_మార్గం, targetfd);
      len = సురక్షిత_readlinkat (AT_FDCWD, లక్ష్యం_fd_మార్గం, absolute_మార్గం, 0, err);
      ఒకవేళ (UNLIKELY (len < 0))
        తిరిగి crun_error_wrap (err, "error unable to provide absolute path to file `%s`", మార్గం_name);
    }

  తిరిగి 0;
}

పూర్ణాంకం
open_unix_domain_client_socket (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం dgram, libcrun_error_t *err)
{
  నిర్మాణం sockaddr_un addr = {};
  పూర్ణాంకం ret;
  proc_fd_మార్గం_t name_buf;
  cleanup_close పూర్ణాంకం destfd = -1;
  cleanup_close పూర్ణాంకం fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", మార్గం);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_ప్రవాహం, 0);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "create UNIX socket");

  ఒకవేళ (అక్షరశ్రేణి_పొడవు (మార్గం) >= పరిమాణం (addr.sun_మార్గం))
    {
      destfd = open (మార్గం, O_మార్గం | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (destfd < 0))
        తిరిగి crun_make_error (err, errno, "open `%s`", మార్గం);

      get_proc_self_fd_మార్గం (name_buf, destfd);

      మార్గం = name_buf;
    }

  size_t మార్గం_len = అక్షరశ్రేణి_పొడవు (మార్గం);
  ఒకవేళ (మార్గం_len >= పరిమాణం (addr.sun_మార్గం))
    తిరిగి crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", మార్గం);

  జ్ఞాపకం_నకలు (addr.sun_మార్గం, మార్గం, మార్గం_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (నిర్మాణం sockaddr *) &addr, పరిమాణం (addr));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "connect socket to `%s`", మార్గం);

  ret = fd;
  fd = -1;

  తిరిగి ret;
}

పూర్ణాంకం
open_unix_domain_socket (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం dgram, libcrun_error_t *err)
{
  నిర్మాణం sockaddr_un addr = {};
  proc_fd_మార్గం_t name_buf;
  పూర్ణాంకం ret;
  cleanup_close పూర్ణాంకం dirfd = -1;
  cleanup_close పూర్ణాంకం fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_ప్రవాహం, 0);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "create UNIX socket");

  ఒకవేళ (అక్షరశ్రేణి_పొడవు (మార్గం) >= పరిమాణం (addr.sun_మార్గం))
    {
      cleanup_free అక్షరం *dpath = xstrdup (మార్గం);
      cleanup_free అక్షరం *bpath = xstrdup (మార్గం);
      స్థిరరాశి అక్షరం *మాతృ_dir = dirname (dpath);
      స్థిరరాశి అక్షరం *పునాది = basename (bpath);
      పూర్ణాంకం n;

      dirfd = open (మాతృ_dir, O_మార్గం | O_DIRECTORY | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (dirfd < 0))
        తిరిగి crun_make_error (err, errno, "open directory `%s`", మాతృ_dir);

      get_proc_self_fd_మార్గం (name_buf, dirfd);

      n = snprintf (addr.sun_మార్గం, పరిమాణం (addr.sun_మార్గం), "%s/%s", name_buf, పునాది);
      ఒకవేళ (n < 0 || (size_t) n >= పరిమాణం (addr.sun_మార్గం))
        తిరిగి crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", మార్గం);
    }
  లేకపోతే
    {
      size_t మార్గం_len = అక్షరశ్రేణి_పొడవు (మార్గం);
      జ్ఞాపకం_నకలు (addr.sun_మార్గం, మార్గం, మార్గం_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (నిర్మాణం sockaddr *) &addr, పరిమాణం (addr));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "bind socket to `%s`", మార్గం);

  ఒకవేళ (! dgram)
    {
      ret = listen (fd, 1);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "listen on socket");
    }

  ret = fd;
  fd = -1;

  తిరిగి ret;
}

పూర్ణాంకం
send_fd_గమ్యం_socket (పూర్ణాంకం server, పూర్ణాంకం fd, libcrun_error_t *err)
{
  తిరిగి send_fd_గమ్యం_socket_with_payload (server, fd, ఖాళీ, 0, err);
}

పూర్ణాంకం
send_fd_గమ్యం_socket_with_payload (పూర్ణాంకం server, పూర్ణాంకం fd, స్థిరరాశి అక్షరం *payload, size_t payload_len, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  నిర్మాణం cmsghdr *cmsg = ఖాళీ;
  నిర్మాణం iovec iov[2];
  నిర్మాణం msghdr సందేశం = {};
  అక్షరం ctrl_buf[CMSG_SPACE (1 + పరిమాణం (పూర్ణాంకం))] = {};
  అక్షరం దత్తాంశం[1];

  దత్తాంశం[0] = ' ';
  iov[0].iov_base = దత్తాంశం;
  iov[0].iov_len = పరిమాణం (దత్తాంశం);

  ఒకవేళ (payload_len > 0)
    {
      iov[0].iov_base = (శూన్య *) payload;
      iov[0].iov_len = payload_len;
    }

  సందేశం.సందేశం_name = ఖాళీ;
  సందేశం.సందేశం_namelen = 0;
  సందేశం.సందేశం_iov = iov;
  సందేశం.సందేశం_iovlen = 1;
  సందేశం.సందేశం_controllen = CMSG_SPACE (పరిమాణం (పూర్ణాంకం));
  సందేశం.సందేశం_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&సందేశం);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (పరిమాణం (పూర్ణాంకం));

  *((పూర్ణాంకం *) CMSG_దత్తాంశం (cmsg)) = fd;

  ret = తాత్కాలిక_FAILURE_RETRY (sendmsg (server, &సందేశం, 0));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "sendmsg");
  తిరిగి 0;
}

పూర్ణాంకం
receive_fd_from_socket_with_payload (పూర్ణాంకం from, అక్షరం *payload, size_t payload_len, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం fd = -1;
  పూర్ణాంకం ret;
  నిర్మాణం iovec iov[1];
  నిర్మాణం msghdr సందేశం = {};
  అక్షరం ctrl_buf[CMSG_SPACE (పరిమాణం (పూర్ణాంకం))] = {};
  అక్షరం దత్తాంశం[1];
  నిర్మాణం cmsghdr *cmsg;

  దత్తాంశం[0] = ' ';
  iov[0].iov_base = దత్తాంశం;
  iov[0].iov_len = పరిమాణం (దత్తాంశం);

  ఒకవేళ (payload_len > 0)
    {
      iov[0].iov_base = (శూన్య *) payload;
      iov[0].iov_len = payload_len;
    }

  సందేశం.సందేశం_name = ఖాళీ;
  సందేశం.సందేశం_namelen = 0;
  సందేశం.సందేశం_iov = iov;
  సందేశం.సందేశం_iovlen = 1;
  సందేశం.సందేశం_controllen = CMSG_SPACE (పరిమాణం (పూర్ణాంకం));
  సందేశం.సందేశం_control = ctrl_buf;

  ret = తాత్కాలిక_FAILURE_RETRY (recvmsg (from, &సందేశం, 0));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "recvmsg");
  ఒకవేళ (UNLIKELY (ret == 0))
    తిరిగి crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&సందేశం);
  ఒకవేళ (cmsg == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "no msg received");
  జ్ఞాపకం_నకలు (&fd, CMSG_దత్తాంశం (cmsg), పరిమాణం (fd));

  ret = fd;
  fd = -1;
  తిరిగి ret;
}

పూర్ణాంకం
receive_fd_from_socket (పూర్ణాంకం from, libcrun_error_t *err)
{
  తిరిగి receive_fd_from_socket_with_payload (from, ఖాళీ, 0, err);
}

పూర్ణాంకం
create_socket_pair (పూర్ణాంకం *pair, libcrun_error_t *err)
{
  పూర్ణాంకం ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "socketpair");
  తిరిగి 0;
}

పూర్ణాంకం
create_signalfd (sigset_t *ముసుగు, libcrun_error_t *err)
{
  పూర్ణాంకం ret = signalfd (-1, ముసుగు, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "signalfd");
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
epoll_helper_toggle (పూర్ణాంకం epollfd, పూర్ణాంకం fd, పూర్ణాంకం events, libcrun_error_t *err)
{
  నిర్మాణం epoll_event ev = {};
  bool కూడు = events != 0;
  పూర్ణాంకం ret;

  ev.events = events;
  ev.దత్తాంశం.fd = fd;
  ret = epoll_ctl (epollfd, కూడు ? EPOLL_CTL_కూడు : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  ఒకవేళ (UNLIKELY (ret < 0 && errno == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      ఒకవేళ (errno == ENOENT || errno == EPERM)
        తిరిగి 0;
      తిరిగి crun_make_error (err, errno, "epoll_ctl `%s` `%d`", కూడు ? "add" : "del", fd);
    }
  తిరిగి 0;
}

పూర్ణాంకం
epoll_helper (పూర్ణాంకం *లోపల_fds, పూర్ణాంకం *లోపల_levelfds, పూర్ణాంకం *బహిర్గతం_fds, పూర్ణాంకం *బహిర్గతం_levelfds, libcrun_error_t *err)
{
  నిర్మాణం epoll_event ev;
  cleanup_close పూర్ణాంకం epollfd = -1;
  పూర్ణాంకం ret;

  పూర్ణాంకం *it;
  epollfd = epoll_create1 (0);
  ఒకవేళ (UNLIKELY (epollfd < 0))
    తిరిగి crun_make_error (err, errno, "epoll_create1");

#నిర్వచించు కూడు_FDS(FDS, EVENTS)                                            \
  కొరకు (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.దత్తాంశం.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_కూడు, *it, &ev);               \
      ఒకవేళ (UNLIKELY (ret < 0))                                           \
        తిరిగి crun_make_error (err, errno, "epoll_ctl add `%d`", *it); \
    }

  ఒకవేళ (లోపల_fds)
    కూడు_FDS (లోపల_fds, EPOLLIN);
  ఒకవేళ (లోపల_levelfds)
    కూడు_FDS (లోపల_levelfds, EPOLLIN | EPOLLET);
  ఒకవేళ (బహిర్గతం_fds)
    కూడు_FDS (బహిర్గతం_fds, EPOLLOUT);
  ఒకవేళ (బహిర్గతం_levelfds)
    కూడు_FDS (బహిర్గతం_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  తిరిగి ret;
}

పూర్ణాంకం
copy_from_fd_గమ్యం_fd (పూర్ణాంకం src, పూర్ణాంకం dst, పూర్ణాంకం consume, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  ssize_t nread;
  size_t pagesize = get_పుట_కొలత ();
#నిర్వచితమైతే HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#అయితే_ముగింపు
  చేయి
    {
      cleanup_free అక్షరం *బఫరు = ఖాళీ;
      ssize_t మిగిలిన;

#నిర్వచితమైతే HAVE_COPY_FILE_RANGE
      ఒకవేళ (can_copy_file_range)
        {
          nread = copy_file_range (src, ఖాళీ, dst, ఖాళీ, pagesize, 0);
          ఒకవేళ (nread < 0 && (errno == EINVAL || errno == EXDEV))
            {
              can_copy_file_range = false;
              వెళ్ళు fallback;
            }
          ఒకవేళ (consume && nread < 0 && errno == EAGAIN)
            తిరిగి 0;
          ఒకవేళ (nread < 0 && errno == EIO)
            తిరిగి 0;
          ఒకవేళ (UNLIKELY (nread < 0))
            తిరిగి crun_make_error (err, errno, "copy_file_range");
          కొనసాగు;
        }
    fallback:
#అయితే_ముగింపు

      బఫరు = xmalloc (pagesize);
      nread = తాత్కాలిక_FAILURE_RETRY (పఠ (src, బఫరు, pagesize));
      ఒకవేళ (consume && nread < 0 && errno == EAGAIN)
        తిరిగి 0;
      ఒకవేళ (nread < 0 && errno == EIO)
        తిరిగి 0;
      ఒకవేళ (UNLIKELY (nread < 0))
        తిరిగి crun_make_error (err, errno, "read");

      మిగిలిన = nread;
      అయితే (మిగిలిన)
        {
          ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (dst, బఫరు + nread - మిగిలిన, మిగిలిన));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "write");
          మిగిలిన -= ret;
        }
  } అయితే (consume && nread);

  తిరిగి 0;
}

పూర్ణాంకం
నిరంతరం_process (అక్షరం **వాదనలు, libcrun_error_t *err)
{
  pid_t pid = fork ();
  ఒకవేళ (UNLIKELY (pid < 0))
    తిరిగి crun_make_error (err, errno, "fork");
  ఒకవేళ (pid)
    {
      పూర్ణాంకం r, స్థితి_2;
      r = తాత్కాలిక_FAILURE_RETRY (waitpid (pid, &స్థితి_2, 0));
      ఒకవేళ (r < 0)
        తిరిగి crun_make_error (err, errno, "waitpid");
      ఒకవేళ (WIFEXITED (స్థితి_2) || WIFSIGNALED (స్థితి_2))
        తిరిగి WEXITSTATUS (స్థితి_2);
    }

  execvp (వాదనలు[0], వాదనలు);
  సురక్షిత_exit (EXIT_FAILURE);
}

#నిర్వచితం_కాకపోతే HAVE_FGETPWENT_R
స్థిర గుర్తులేని
atou (అక్షరం **s)
{
  గుర్తులేని x;
  కొరకు (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  తిరిగి x;
}

పూర్ణాంకం
fgetpwent_r (FILE *f, నిర్మాణం passwd *pw, అక్షరం *పంక్తి, size_t కొలత, నిర్మాణం passwd **res)
{
  అక్షరం *s;
  పూర్ణాంకం rv = 0;
  కొరకు (;;)
    {
      పంక్తి[కొలత - 1] = '\xff';
      ఒకవేళ ((fgets (పంక్తి, కొలత, f) == ఖాళీ) || ferror (f) || పంక్తి[కొలత - 1] != '\xff')
        {
          rv = (పంక్తి[కొలత - 1] != '\xff') ? ERANGE : ENOENT;
          పంక్తి = 0;
          pw = 0;
          ఆపు;
        }
      పంక్తి[strcspn (పంక్తి, "\n")] = 0;

      s = పంక్తి;
      pw->pw_name = s++;
      ఒకవేళ (! (s = strchr (s, ':')))
        కొనసాగు;

      *s++ = 0;
      pw->pw_passwd = s;
      ఒకవేళ (! (s = strchr (s, ':')))
        కొనసాగు;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      ఒకవేళ (*s != ':')
        కొనసాగు;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      ఒకవేళ (*s != ':')
        కొనసాగు;

      *s++ = 0;
      pw->pw_gecos = s;
      ఒకవేళ (! (s = strchr (s, ':')))
        కొనసాగు;

      *s++ = 0;
      pw->pw_dir = s;
      ఒకవేళ (! (s = strchr (s, ':')))
        కొనసాగు;

      *s++ = 0;
      pw->pw_shell = s;
      ఆపు;
    }
  *res = pw;
  ఒకవేళ (rv)
    errno = rv;
  తిరిగి rv;
}
#అయితే_ముగింపు

పూర్ణాంకం
సమితి_home_env (uid_t id)
{
  నిర్మాణం passwd pwd;
  cleanup_free అక్షరం *buf = ఖాళీ;
  దీర్ఘ buf_కొలత;
  cleanup_file FILE *ప్రవాహం = ఖాళీ;

  buf_కొలత = sysconf (_SC_GETPW_R_SIZE_MAX);
  ఒకవేళ (buf_కొలత < 0)
    buf_కొలత = 1024;

  buf = xmalloc (buf_కొలత);

  ప్రవాహం = fopen ("/etc/passwd", "re");
  ఒకవేళ (ప్రవాహం == ఖాళీ)
    తిరిగి -1;

  కొరకు (;;)
    {
      నిర్మాణం passwd *ret_pw = ఖాళీ;

      పూర్ణాంకం ret = fgetpwent_r (ప్రవాహం, &pwd, buf, buf_కొలత, &ret_pw);
      ఒకవేళ (UNLIKELY (ret != 0))
        {
          ఒకవేళ (ret == EINTR)
            కొనసాగు;

          ఒకవేళ (ret != ERANGE)
            {
              /* Let callers handle the error if the user was not found. */
              errno = ret;
              తిరిగి -1;
            }

          buf_కొలత *= 2;
          buf = xrealloc (buf, buf_కొలత);
          కొనసాగు;
        }

      ఒకవేళ (ret_pw && ret_pw->pw_uid == id)
        {
          ఒకవేళ (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          లేకపోతే
            తిరిగి 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
స్థిర పూర్ణాంకం
getsubidrange (uid_t id, పూర్ణాంకం is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file FILE *ఆదానం = ఖాళీ;
  cleanup_free అక్షరం *lineptr = ఖాళీ;
  size_t lenlineptr = 0, len_name;
  దీర్ఘ buf_కొలత;
  cleanup_free అక్షరం *buf = ఖాళీ;
  స్థిరరాశి అక్షరం *name;
  నిర్మాణం passwd pwd;

  buf_కొలత = sysconf (_SC_GETPW_R_SIZE_MAX);
  ఒకవేళ (buf_కొలత < 0)
    buf_కొలత = 1024;

  buf = xmalloc (buf_కొలత);
  కొరకు (;;)
    {
      పూర్ణాంకం ret;
      నిర్మాణం passwd *ret_pw = ఖాళీ;

      ret = getpwuid_r (id, &pwd, buf, buf_కొలత, &ret_pw);
      ఒకవేళ (LIKELY (ret == 0))
        {
          ఒకవేళ (ret_pw)
            {
              name = ret_pw->pw_name;
              ఆపు;
            }
          తిరిగి -1;
        }

      ఒకవేళ (ret != ERANGE)
        తిరిగి -1;

      buf_కొలత *= 2;
      buf = xrealloc (buf, buf_కొలత);
    }

  len_name = అక్షరశ్రేణి_పొడవు (name);

  ఆదానం = fopen (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  ఒకవేళ (ఆదానం == ఖాళీ)
    తిరిగి -1;

  కొరకు (;;)
    {
      అక్షరం *endptr;
      ssize_t పఠ = getline (&lineptr, &lenlineptr, ఆదానం);
      ఒకవేళ (పఠ < 0)
        తిరిగి -1;

      ఒకవేళ (పఠ < (ssize_t) (len_name + 2))
        కొనసాగు;

      ఒకవేళ (memcmp (lineptr, name, len_name) || lineptr[len_name] != ':')
        కొనసాగు;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      ఒకవేళ (endptr >= &lineptr[పఠ])
        తిరిగి -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      తిరిగి 0;
    }
}

#నిర్వచించు MIN(x, y) ((x) < (y) ? (x) : (y))

పూర్ణాంకం
format_default_id_mapping (అక్షరం **బహిర్గతం, uid_t container_id, uid_t host_uid, uid_t host_id, పూర్ణాంకం is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, అందుబాటు = 0;
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  పూర్ణాంకం written = 0;
  పూర్ణాంకం ret, మిగిలిన;

  *బహిర్గతం = ఖాళీ;

  ఒకవేళ (getsubidrange (host_uid, is_uid, &from, &అందుబాటు) < 0)
    తిరిగి 0;

  /* More than enough space for all the mappings.  */
  మిగిలిన = 15 * 5 * 3;
  బఫరు = xmalloc (మిగిలిన + 1);

  ఒకవేళ (container_id > 0)
    {
      uint32_t వాడిన = MIN (container_id, అందుబాటు);
      ret = snprintf (బఫరు + written, మిగిలిన, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, వాడిన);
      ఒకవేళ (UNLIKELY (ret >= మిగిలిన))
        తిరిగి crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      మిగిలిన -= ret;

      from += వాడిన;
      అందుబాటు -= వాడిన;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (బఫరు + written, మిగిలిన, "%d %d 1\n", container_id, host_id);
  ఒకవేళ (UNLIKELY (ret >= మిగిలిన))
    తిరిగి crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  మిగిలిన -= ret;

  /* Last mapping: use any id that is left.  */
  ఒకవేళ (అందుబాటు)
    {
      ret = snprintf (బఫరు + written, మిగిలిన, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, అందుబాటు);
      ఒకవేళ (UNLIKELY (ret >= మిగిలిన))
        తిరిగి crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      మిగిలిన -= ret;
    }

  *బహిర్గతం = బఫరు;
  బఫరు = ఖాళీ;
  తిరిగి written;
}

స్థిర పూర్ణాంకం
unset_cloexec_flag (పూర్ణాంకం fd)
{
  పూర్ణాంకం flags = fcntl (fd, F_GETFD);
  ఒకవేళ (flags == -1)
    తిరిగి -1;

  flags &= ~FD_CLOEXEC;

  తిరిగి fcntl (fd, F_SETFD, flags);
}

స్థిర శూన్య __attribute__ ((__noreturn__))
నిరంతరం_process_బిడ్డ (అక్షరం *మార్గం, అక్షరం **వాదనలు, స్థిరరాశి అక్షరం *cwd, అక్షరం **envp, పూర్ణాంకం pipe_r,
                   పూర్ణాంకం pipe_w, పూర్ణాంకం బహిర్గతం_fd, పూర్ణాంకం err_fd, bool can_ignore_chdir_errors)
{
  అక్షరం *tmp_వాదనలు[] = { మార్గం, ఖాళీ };
  libcrun_error_t err = ఖాళీ;
  పూర్ణాంకం dev_null_fd = -1;
  పూర్ణాంకం ret;

  ret = mark_or_close_fds_ge_than (ఖాళీ, 3, false, &err);
  ఒకవేళ (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->స్థితి_2, "%s", (err)->సందేశం);

  ఒకవేళ (బహిర్గతం_fd < 0 || err_fd < 0)
    {
      dev_null_fd = open ("/dev/null", O_WRONLY | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (dev_null_fd < 0))
        సురక్షిత_exit (EXIT_FAILURE);
    }

  తాత్కాలిక_FAILURE_RETRY (close (pipe_w));
  dup2 (pipe_r, 0);
  తాత్కాలిక_FAILURE_RETRY (close (pipe_r));

  dup2 (బహిర్గతం_fd >= 0 ? బహిర్గతం_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  ఒకవేళ (బహిర్గతం_fd >= 0)
    unset_cloexec_flag (1);
  ఒకవేళ (err_fd >= 0)
    unset_cloexec_flag (2);

  ఒకవేళ (dev_null_fd >= 0)
    తాత్కాలిక_FAILURE_RETRY (close (dev_null_fd));
  ఒకవేళ (బహిర్గతం_fd >= 0)
    తాత్కాలిక_FAILURE_RETRY (close (బహిర్గతం_fd));
  ఒకవేళ (err_fd >= 0)
    తాత్కాలిక_FAILURE_RETRY (close (err_fd));

  ఒకవేళ (వాదనలు == ఖాళీ)
    వాదనలు = tmp_వాదనలు;

  ఒకవేళ (cwd && chdir (cwd) < 0)
    {
      ఒకవేళ (! can_ignore_chdir_errors || (errno != EACCES && errno != EPERM))
        సురక్షిత_exit (EXIT_FAILURE);
    }

  execvpe (మార్గం, వాదనలు, envp);
  సురక్షిత_exit (EXIT_FAILURE);
}

/* It changes the signals mask for the current process.  */
పూర్ణాంకం
నిరంతరం_process_with_stdin_timeout_envp (అక్షరం *మార్గం, అక్షరం **వాదనలు, స్థిరరాశి అక్షరం *cwd, పూర్ణాంకం timeout,
                                     అక్షరం **envp, అక్షరం *stdin, size_t stdin_len, పూర్ణాంకం బహిర్గతం_fd,
                                     పూర్ణాంకం err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  పూర్ణాంకం stdin_pipe[2];
  pid_t pid;
  పూర్ణాంకం ret;
  cleanup_close పూర్ణాంకం pipe_r = -1;
  cleanup_close పూర్ణాంకం pipe_w = -1;
  sigset_t oldmask, ముసుగు;
  పూర్ణాంకం r, స్థితి_2;

  sigemptyset (&ముసుగు);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  ఒకవేళ (timeout > 0)
    {
      sigaddset (&ముసుగు, SIGCHLD);
      ret = sigprocmask (SIG_ఖండం, &ముసుగు, &oldmask);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "sigprocmask");
    }

  pid = fork ();
  ఒకవేళ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      వెళ్ళు restore_sig_ముసుగు_and_exit;
    }

  ఒకవేళ (pid == 0)
    {
      /* run_process_child doesn't return.  */
      నిరంతరం_process_బిడ్డ (మార్గం, వాదనలు, cwd, envp, pipe_r, pipe_w, బహిర్గతం_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (pipe_w, stdin, stdin_len));
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      ఒకవేళ (errno != EPIPE)
        {
          ret = crun_make_error (err, errno, "write to pipe");
          వెళ్ళు restore_sig_ముసుగు_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  ఒకవేళ (timeout)
    {
      time_t ప్రారంభం = సమయం (ఖాళీ);
      time_t now;
      కొరకు (now = ప్రారంభం; now - ప్రారంభం < timeout; now = సమయం (ఖాళీ))
        {
          siginfo_t info;
          పూర్ణాంకం elapsed = now - ప్రారంభం;
          నిర్మాణం timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&ముసుగు, &info, &ts_timeout);
          ఒకవేళ (UNLIKELY (ret < 0 && errno != EAGAIN))
            {
              ret = crun_make_error (err, errno, "sigtimedwait");
              వెళ్ళు restore_sig_ముసుగు_and_exit;
            }

          ఒకవేళ (info.si_signo == SIGCHLD && info.si_pid == pid)
            వెళ్ళు read_waitpid;

          ఒకవేళ (ret < 0 && errno == EAGAIN)
            వెళ్ళు timeout;
        }
    timeout:
      kill (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", మార్గం);
      వెళ్ళు restore_sig_ముసుగు_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &స్థితి_2, 0);
  ఒకవేళ (r < 0)
    ret = crun_make_error (err, errno, "waitpid");
  లేకపోతే
    ret = get_process_exit_స్థితి (స్థితి_2);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_ముసుగు_and_exit:
  ఒకవేళ (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      ఒకవేళ (pid > 0)
        {
          kill (pid, SIGKILL);
          తాత్కాలిక_FAILURE_RETRY (waitpid (pid, &స్థితి_2, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, ఖాళీ);
      ఒకవేళ (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "restoring signal mask with sigprocmask");
    }
  తిరిగి ret;
}

పూర్ణాంకం
mark_or_close_fds_ge_than (libcrun_container_t *container, పూర్ణాంకం n, bool close_now, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం cfd = -1;
  cleanup_dir DIR *dir = ఖాళీ;
  పూర్ణాంకం ret;
  పూర్ణాంకం fd;
  నిర్మాణం dirent *తదుపరి;

#నిర్వచితమైతే ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  ఒకవేళ (close_now)
    GCOV_DUMP ();
#అయితే_ముగింపు

  ret = syscall_close_range (n, UINT_MAX, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  ఒకవేళ (ret == 0)
    {
      ఒకవేళ (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      తిరిగి 0;
    }
  ఒకవేళ (ret < 0 && errno != EINVAL && errno != ENOSYS && errno != EPERM)
    తిరిగి crun_make_error (err, errno, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  ఒకవేళ (UNLIKELY (cfd < 0))
    తిరిగి cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  dir = fdopendir (cfd);
  ఒకవేళ (UNLIKELY (dir == ఖాళీ))
    తిరిగి crun_make_error (err, errno, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  కొరకు (తదుపరి = readdir (dir); తదుపరి; తదుపరి = readdir (dir))
    {
      పూర్ణాంకం val;
      స్థిరరాశి అక్షరం *name = తదుపరి->d_name;
      ఒకవేళ (name[0] == '.')
        కొనసాగు;

      val = strtoll (name, ఖాళీ, 10);
      ఒకవేళ (val < n || val == fd)
        కొనసాగు;

      ఒకవేళ (close_now)
        {
          ret = close (val);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "close fd `%d`", val);

          ఒకవేళ (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      లేకపోతే
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  తిరిగి 0;
}

శూన్య
get_current_timestamp (అక్షరం *బహిర్గతం, size_t len)
{
  నిర్మాణం timeval tv;
  నిర్మాణం tm now;
  అక్షరం timestamp[64];

  gettimeofday (&tv, ఖాళీ);
  gmtime_r (&tv.tv_sec, &now);
  strftime (timestamp, పరిమాణం (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (శూన్య) snprintf (బహిర్గతం, len, "%s.%06lldZ", timestamp, (దీర్ఘ దీర్ఘ పూర్ణాంకం) tv.tv_usec);
  బహిర్గతం[len - 1] = '\0';
}

పూర్ణాంకం
సమితి_blocking_fd (పూర్ణాంకం fd, bool blocking, libcrun_error_t *err)
{
  పూర్ణాంకం ret, flags = fcntl (fd, F_GETFL, 0);
  ఒకవేళ (UNLIKELY (flags < 0))
    తిరిగి crun_make_error (err, errno, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fcntl");
  తిరిగి 0;
}

పూర్ణాంకం
parse_json_file (json_object **బహిర్గతం, స్థిరరాశి అక్షరం *jsondata, నిర్మాణం parser_context *ctx arg_వాడని, libcrun_error_t *err)
{
  నిర్మాణం json_tokener *tok;
  గణన json_tokener_error jerr;

  *err = ఖాళీ;

  ఒకవేళ (UNLIKELY (jsondata == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  ఒకవేళ (UNLIKELY (tok == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_సమితి_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *బహిర్గతం = json_tokener_parse_ex (tok, jsondata, (పూర్ణాంకం) అక్షరశ్రేణి_పొడవు (jsondata));
  jerr = json_tokener_get_error (tok);
  ఒకవేళ (*బహిర్గతం == ఖాళీ || jerr != json_tokener_success)
    {
      పూర్ణాంకం saved_pos = (పూర్ణాంకం) tok->char_స్థానభ్రంశం;

      json_tokener_free (tok);
      ఒకవేళ (*బహిర్గతం)
        {
          json_object_put (*బహిర్గతం);
          *బహిర్గతం = ఖాళీ;
        }
      తిరిగి crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  తిరిగి 0;
}

#నిర్వచించు CHECK_ACCESS_NOT_EXECUTABLE 1
#నిర్వచించు CHECK_ACCESS_NOT_REGULAR 2

/* check that the specified path exists and it is executable.
   Return
   - 0 if the file is executable
   - CHECK_ACCESS_NOT_EXECUTABLE if it exists but it is not executable
   - CHECK_ACCESS_NOT_REGULAR if it not a regular file
   - -errno for any other generic error
 */
స్థిర పూర్ణాంకం
check_access (స్థిరరాశి అక్షరం *మార్గం)
{
  పూర్ణాంకం ret;
  mode_t బహుళకం;

#నిర్వచితమైతే HAVE_EACCESS
#  define ACCESS(మార్గం, బహుళకం) (eaccess (మార్గం, బహుళకం))
#లేకపోతే
#  define ACCESS(మార్గం, బహుళకం) (access (మార్గం, బహుళకం))
#అయితే_ముగింపు

  ret = ACCESS (మార్గం, X_OK);
  ఒకవేళ (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      ఒకవేళ (errno == EACCES)
        {
          పూర్ణాంకం saved_errno = errno;
          ret = ACCESS (మార్గం, F_OK);
          errno = saved_errno;

          ఒకవేళ (ret == 0)
            తిరిగి CHECK_ACCESS_NOT_EXECUTABLE;
        }
      తిరిగి -errno;
    }

  ret = get_file_type (&బహుళకం, false, మార్గం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి -errno;

  ఒకవేళ (! S_ISREG (బహుళకం))
    తిరిగి CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  తిరిగి 0;
}

పూర్ణాంకం
కనుగొను_executable (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *executable_మార్గం, స్థిరరాశి అక్షరం *cwd, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cwd_executable_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *tmp = ఖాళీ;
  పూర్ణాంకం చివరి_error = -ENOENT;
  అక్షరం *it, *చివర;
  పూర్ణాంకం ret;

  *బహిర్గతం = ఖాళీ;

  ఒకవేళ (executable_మార్గం == ఖాళీ || executable_మార్గం[0] == '\0')
    తిరిగి crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  ఒకవేళ (executable_మార్గం[0] != '/' && strchr (executable_మార్గం, '/'))
    {
      cleanup_free అక్షరం *cwd_allocated = ఖాళీ;

      ఒకవేళ (cwd == ఖాళీ)
        {
          cwd_allocated = getcwd (ఖాళీ, 0);
          ఒకవేళ (cwd_allocated == ఖాళీ)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_మార్గం, err, "/", cwd, executable_మార్గం, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      executable_మార్గం = cwd_executable_మార్గం;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  ఒకవేళ (executable_మార్గం[0] == '/')
    {
      ret = check_access (executable_మార్గం);
      ఒకవేళ (LIKELY (ret == 0))
        {
          *బహిర్గతం = xstrdup (executable_మార్గం);
          తిరిగి 0;
        }
      చివరి_error = ret;
      వెళ్ళు fail;
    }

  చివర = tmp = xstrdup (getenv ("PATH"));

  అయితే ((it = strsep (&చివర, ":")))
    {
      cleanup_free అక్షరం *మార్గం = ఖాళీ;

      ఒకవేళ (it == చివర)
        it = ".";

      ret = append_paths (&మార్గం, err, it, executable_మార్గం, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          కొనసాగు;
        }

      ret = check_access (మార్గం);
      ఒకవేళ (ret == 0)
        {
          /* Change owner.  */
          *బహిర్గతం = మార్గం;
          మార్గం = ఖాళీ;
          తిరిగి 0;
        }

      ఒకవేళ (ret == -ENOENT)
        కొనసాగు;

      చివరి_error = ret;
    }

fail:
  ఎంపిక (చివరి_error)
    {
    స్థితి CHECK_ACCESS_NOT_EXECUTABLE:
      తిరిగి crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_మార్గం);

    స్థితి CHECK_ACCESS_NOT_REGULAR:
      తిరిగి crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_మార్గం);

    అప్రమేయ:
      errno = -చివరి_error;
      ఒకవేళ (errno == ENOENT)
        తిరిగి crun_make_error (err, errno, "executable file `%s` not found%s", executable_మార్గం, executable_మార్గం[0] == '/' ? "" : " in $PATH");
      తిరిగి crun_make_error (err, errno, "open `%s`", executable_మార్గం);
    }
}

#నిర్వచితమైతే HAVE_FGETXATTR

స్థిర ssize_t
సురక్షిత_read_xattr (అక్షరం **ret, పూర్ణాంకం sfd, స్థిరరాశి అక్షరం *srcname, స్థిరరాశి అక్షరం *name, size_t initial_కొలత, libcrun_error_t *err)
{
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  ssize_t current_కొలత;
  ssize_t s;

  current_కొలత = (ssize_t) initial_కొలత;
  బఫరు = xmalloc (current_కొలత + 1);

  అయితే (1)
    {
      s = fgetxattr (sfd, name, బఫరు, current_కొలత);
      ఒకవేళ (UNLIKELY (s < 0))
        తిరిగి crun_make_error (err, errno, "get xattr `%s` from `%s`", name, srcname);

      ఒకవేళ (s < current_కొలత)
        ఆపు;

      current_కొలత *= 2;
      బఫరు = xrealloc (బఫరు, current_కొలత + 1);
    }

  ఒకవేళ (s <= 0)
    తిరిగి s;

  బఫరు[s] = '\0';

  /* Change owner.  */
  *ret = బఫరు;
  బఫరు = ఖాళీ;

  తిరిగి s;
}

స్థిర ssize_t
copy_xattr (పూర్ణాంకం sfd, పూర్ణాంకం dfd, స్థిరరాశి అక్షరం *srcname, స్థిరరాశి అక్షరం *destname, libcrun_error_t *err)
{
  cleanup_free అక్షరం *buf = ఖాళీ;
  ssize_t xattr_len;
  అక్షరం *it;

  xattr_len = flistxattr (sfd, ఖాళీ, 0);
  ఒకవేళ (UNLIKELY (xattr_len < 0))
    {
      ఒకవేళ (errno == ENOTSUP)
        తిరిగి 0;

      తిరిగి crun_make_error (err, errno, "flistxattr `%s`", srcname);
    }

  ఒకవేళ (xattr_len == 0)
    తిరిగి 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  ఒకవేళ (UNLIKELY (xattr_len < 0))
    తిరిగి crun_make_error (err, errno, "flistxattr `%s`", srcname);

  కొరకు (it = buf; it - buf < xattr_len; it += అక్షరశ్రేణి_పొడవు (it) + 1)
    {
      cleanup_free అక్షరం *v = ఖాళీ;
      ssize_t s;

      s = సురక్షిత_read_xattr (&v, sfd, srcname, it, 256, err);
      ఒకవేళ (UNLIKELY (s < 0))
        తిరిగి s;

      s = fsetxattr (dfd, it, v, s, 0);
      ఒకవేళ (UNLIKELY (s < 0))
        {
          ఒకవేళ (errno == EINVAL || errno == EOPNOTSUPP)
            కొనసాగు;

          తిరిగి crun_make_error (err, errno, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  తిరిగి 0;
}

#అయితే_ముగింపు

స్థిర పూర్ణాంకం
copy_rec_stat_file_at (పూర్ణాంకం dfd, స్థిరరాశి అక్షరం *మార్గం, mode_t *బహుళకం, off_t *కొలత, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  నిర్మాణం stat st;
  పూర్ణాంకం ret;

#నిర్వచితమైతే HAVE_STATX
  నిర్మాణం statx stx = {
    0,
  };

  ret = statx (dfd, మార్గం, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_బహుళకం | STATX_కొలత | STATX_UID | STATX_GID, &stx);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno == ENOSYS || errno == EINVAL)
        వెళ్ళు fallback;

      తిరిగి ret;
    }

  *బహుళకం = stx.stx_mode;
  *కొలత = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  తిరిగి ret;

fallback:
#అయితే_ముగింపు
  ret = fstatat (dfd, మార్గం, &st, AT_SYMLINK_NOFOLLOW);

  *బహుళకం = st.st_mode;
  *కొలత = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  తిరిగి ret;
}

పూర్ణాంకం
copy_recursive_fd_గమ్యం_fd (పూర్ణాంకం srcdirfd, పూర్ణాంకం dfd, స్థిరరాశి అక్షరం *srcname, స్థిరరాశి అక్షరం *destname, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం destdirfd = dfd;
  cleanup_dir DIR *dsrcfd = ఖాళీ;
  నిర్మాణం dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  ఒకవేళ (UNLIKELY (dsrcfd == ఖాళీ))
    {
      తాత్కాలిక_FAILURE_RETRY (close (srcdirfd));
      తిరిగి crun_make_error (err, errno, "open directory `%s`", destname);
    }

  కొరకు (de = readdir (dsrcfd); de; de = readdir (dsrcfd))
    {
      cleanup_close పూర్ణాంకం srcfd = -1;
      cleanup_close పూర్ణాంకం destfd = -1;
      cleanup_free అక్షరం *లక్ష్యం_buf = ఖాళీ;
      పూర్ణాంకం ret;
      mode_t బహుళకం;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      ఒకవేళ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        కొనసాగు;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &బహుళకం, &st_size, &rdev, &uid, &gid);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "stat `%s/%s`", srcname, de->d_name);

      ఎంపిక (బహుళకం & S_IFMT)
        {
        స్థితి S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          ఒకవేళ (UNLIKELY (srcfd < 0))
            తిరిగి crun_make_error (err, errno, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          ఒకవేళ (UNLIKELY (destfd < 0))
            తిరిగి crun_make_error (err, errno, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_గమ్యం_fd (srcfd, destfd, 1, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

#నిర్వచితమైతే HAVE_FGETXATTR
          ret = (పూర్ణాంకం) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
#అయితే_ముగింపు

          తాత్కాలిక_FAILURE_RETRY (close (destfd));
          destfd = -1;
          ఆపు;

        స్థితి S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, బహుళకం);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ఒకవేళ (UNLIKELY (srcfd < 0))
            తిరిగి crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          ఒకవేళ (UNLIKELY (destfd < 0))
            తిరిగి crun_make_error (err, errno, "open directory `%s/%s`", srcname, de->d_name);

#నిర్వచితమైతే HAVE_FGETXATTR
          ret = (పూర్ణాంకం) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
#అయితే_ముగింపు

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_గమ్యం_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          ఆపు;

        స్థితి S_IFLNK:
          ret = సురక్షిత_readlinkat (dirfd (dsrcfd), de->d_name, &లక్ష్యం_buf, st_size, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          ret = symlinkat (లక్ష్యం_buf, destdirfd, de->d_name);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "symlinkat `%s/%s`", destname, de->d_name);
          ఆపు;

        స్థితి S_IFBLK:
        స్థితి S_IFCHR:
        స్థితి S_IFIFO:
        స్థితి S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, బహుళకం, rdev);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "mknodat `%s/%s`", destname, de->d_name);
          ఆపు;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#నిర్వచితం_కాకపోతే ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#అయితే_ముగింపు

      ret = fchmodat (destdirfd, de->d_name, బహుళకం & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          ఒకవేళ (errno == ENOTSUP)
            కొనసాగు;

          తిరిగి crun_make_error (err, errno, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  తిరిగి 0;
}

స్థిరరాశి అక్షరం *
కనుగొను_annotation (libcrun_container_t *container, స్థిరరాశి అక్షరం *name)
{
  ఒకవేళ (container->container_def->annotations == ఖాళీ)
    తిరిగి ఖాళీ;

  తిరిగి కనుగొను_string_map_విలువ (container->annotations, name);
}

పూర్ణాంకం
సురక్షిత_write (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *fname, స్థిరరాశి శూన్య *buf, size_t లెక్క, libcrun_error_t *err)
{
  size_t written = 0;
  అయితే (written < లెక్క)
    {
      ssize_t w = లిఖ (fd, (స్థిరరాశి అక్షరం *) buf + written, లెక్క - written);
      ఒకవేళ (UNLIKELY (w < 0))
        {
          ఒకవేళ (errno == EINTR || errno == EAGAIN)
            కొనసాగు;
          తిరిగి crun_make_error (err, errno, "write to `%s`", fname);
        }
      written += w;
    }
  తిరిగి 0;
}

#నిర్వచించు MAX_PARTS 32

పూర్ణాంకం
append_paths (అక్షరం **బహిర్గతం, libcrun_error_t *err, ...)
{
  స్థిరరాశి అక్షరం *parts[MAX_PARTS];
  size_t sizes[MAX_PARTS];
  size_t మొత్తం_len = 0;
  size_t n_parts = 0;
  size_t copied = 0;
  va_list ap;
  size_t i;

  va_ప్రారంభం (ap, err);
  కొరకు (;;)
    {
      స్థిరరాశి అక్షరం *part;
      size_t కొలత;

      part = va_arg (ap, స్థిరరాశి అక్షరం *);
      ఒకవేళ (part == ఖాళీ)
        ఆపు;

      ఒకవేళ (n_parts == MAX_PARTS)
        {
          va_చివర (ap);
          తిరిగి crun_make_error (err, EINVAL, "too many paths specified");
        }

      ఒకవేళ (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          అయితే (part[0] == '/' && part[1] == '/')
            part++;
        }
      లేకపోతే
        {
          /* And drop any initial '/' for other components.  */
          అయితే (part[0] == '/')
            part++;
        }

      కొలత = అక్షరశ్రేణి_పొడవు (part);
      ఒకవేళ (కొలత == 0)
        కొనసాగు;

      అయితే (కొలత > 1 && part[కొలత - 1] == '/')
        కొలత--;

      parts[n_parts] = part;
      sizes[n_parts] = కొలత;

      n_parts++;
    }
  va_చివర (ap);

  మొత్తం_len = n_parts + 1;
  కొరకు (i = 0; i < n_parts; i++)
    మొత్తం_len += sizes[i];

  *బహిర్గతం = xmalloc (మొత్తం_len);

  copied = 0;
  కొరకు (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*బహిర్గతం)[copied - 1] == '/';
      ఒకవేళ (i > 0 && ! has_trailing_slash)
        {
          (*బహిర్గతం)[copied] = '/';
          copied += 1;
        }

      జ్ఞాపకం_నకలు (*బహిర్గతం + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*బహిర్గతం)[copied] = '\0';
  తిరిగి 0;
}

#ఒకవేళ __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#లేకపోతే
#  define __nonstring
#అయితే_ముగింపు

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
స్థిర పూర్ణాంకం
b64_ఆదానం (అక్షరం c)
{
  స్థిరరాశి అక్షరం పట్టిక[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  పూర్ణాంకం i;

  కొరకు (i = 0; i < 64; i++)
    {
      ఒకవేళ (పట్టిక[i] == c)
        తిరిగి i;
    }
  తిరిగి -1;
}

పూర్ణాంకం
base64_decode (స్థిరరాశి అక్షరం *iptr, size_t isize, అక్షరం *optr, size_t osize, size_t *nbytes)
{
  పూర్ణాంకం i = 0, tmp = 0, pad = 0;
  size_t consumed = 0;
  గుర్తులేని అక్షరం దత్తాంశం[4];

  *nbytes = 0;
  అయితే (consumed < isize && (*nbytes) + 3 < osize)
    {
      అయితే ((i < 4) && (consumed < isize))
        {
          tmp = b64_ఆదానం (*iptr++);
          consumed++;
          ఒకవేళ (tmp != -1)
            {
              ఒకవేళ (UNLIKELY (pad > 0))
                తిరిగి -1;
              దత్తాంశం[i++] = tmp;
            }
          లేకపోతే ఒకవేళ (*(iptr - 1) == '=')
            {
              దత్తాంశం[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      ఒకవేళ (i == 4)
        {
          ఒకవేళ (UNLIKELY (pad > 2))
            తిరిగి -1;
          *optr++ = (దత్తాంశం[0] << 2) | ((దత్తాంశం[1] & 0x30) >> 4);
          *optr++ = ((దత్తాంశం[1] & 0xf) << 4) | ((దత్తాంశం[2] & 0x3c) >> 2);
          *optr++ = ((దత్తాంశం[2] & 0x3) << 6) | దత్తాంశం[3];
          (*nbytes) += 3 - pad;
        }
      లేకపోతే
        {
          /* I did not get all the data.  */
          consumed -= i;
          తిరిగి consumed;
        }
      i = 0;
    }
  తిరిగి consumed;
}

అక్షరం *
get_user_name (uid_t uid)
{
  నిర్మాణం passwd pd;
  నిర్మాణం passwd *తాత్కాలిక_ఫలితం_ptr;
  అక్షరం pwdbuffer[200];
  ఒకవేళ (! getpwuid_r (uid, &pd, pwdbuffer, పరిమాణం (pwdbuffer), &తాత్కాలిక_ఫలితం_ptr))
    తిరిగి xstrdup (pd.pw_name);
  తిరిగి xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
అక్షరం *
libcrun_sysctl_కీలకం_గమ్యం_proc_మార్గం (స్థిరరాశి అక్షరం *కీలకం)
{
  స్థిరరాశి అక్షరం *sep;
  అక్షరం *ret, *it;

  sep = strpbrk (కీలకం, "./");

  /* No separator at all, or the key is already in the path form.  */
  ఒకవేళ (sep == ఖాళీ || *sep == '/')
    తిరిగి xstrdup (కీలకం);

  ret = xstrdup (కీలకం);
  కొరకు (it = ret; *it; it++)
    {
      ఒకవేళ (*it == '.')
        *it = '/';
      లేకపోతే ఒకవేళ (*it == '/')
        *it = '.';
    }

  తిరిగి ret;
}

పూర్ణాంకం
has_suffix (స్థిరరాశి అక్షరం *str, స్థిరరాశి అక్షరం *suffix)
{
  ఒకవేళ (! str || ! suffix)
    తిరిగి 0;
  size_t lenstr = అక్షరశ్రేణి_పొడవు (str);
  size_t lensuffix = అక్షరశ్రేణి_పొడవు (suffix);
  ఒకవేళ (lensuffix > lenstr)
    తిరిగి 0;
  తిరిగి memcmp (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

అక్షరం *
str_join_array (పూర్ణాంకం స్థానభ్రంశం, size_t కొలత, అక్షరం *స్థిరరాశి array[], స్థిరరాశి అక్షరం *joint)
{
  size_t jlen;
  cleanup_free size_t *lens = ఖాళీ;
  size_t i, మొత్తం_కొలత = (కొలత - 1) * (jlen = అక్షరశ్రేణి_పొడవు (joint)) + 1;
  అక్షరం *ఫలితం, *p;

  lens = xmalloc (కొలత * పరిమాణం (*lens));

  కొరకు (i = 0; i < కొలత; ++i)
    {
      మొత్తం_కొలత += (lens[i] = అక్షరశ్రేణి_పొడవు (array[i]));
    }
  p = ఫలితం = xmalloc (మొత్తం_కొలత);
  కొరకు (i = స్థానభ్రంశం; i < కొలత; ++i)
    {
      జ్ఞాపకం_నకలు (p, array[i], lens[i]);
      p += lens[i];
      ఒకవేళ (i < కొలత - 1)
        {
          జ్ఞాపకం_నకలు (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  తిరిగి ఫలితం;
}

పూర్ణాంకం
libcrun_mmap (నిర్మాణం libcrun_mmap_s **ret, శూన్య *addr, size_t నిడివి,
              పూర్ణాంకం prot, పూర్ణాంకం flags, పూర్ణాంకం fd, off_t స్థానభ్రంశం,
              libcrun_error_t *err)
{
  నిర్మాణం libcrun_mmap_s *mmap_s = ఖాళీ;

  శూన్య *mapped = mmap (addr, నిడివి, prot, flags, fd, స్థానభ్రంశం);
  ఒకవేళ (mapped == MAP_FAILED)
    తిరిగి crun_make_error (err, errno, "mmap");

  mmap_s = xmalloc (పరిమాణం (నిర్మాణం libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->నిడివి = నిడివి;

  *ret = mmap_s;

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_munmap (నిర్మాణం libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = munmap (mmap->addr, mmap->నిడివి);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "munmap");

  విడుదల (mmap);

  తిరిగి 0;
}

స్థిర దీర్ఘ
read_file_as_long_or_default (స్థిరరాశి అక్షరం *మార్గం, దీర్ఘ def_విలువ)
{
  cleanup_free అక్షరం *content = ఖాళీ;
  libcrun_error_t tmp_err = ఖాళీ;
  అక్షరం *endptr;
  దీర్ఘ val;
  పూర్ణాంకం ret;

  ret = read_all_file (మార్గం, &content, ఖాళీ, &tmp_err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      తిరిగి def_విలువ;
    }

  errno = 0;
  val = strtol (content, &endptr, 10);
  ఒకవేళ (UNLIKELY (errno))
    తిరిగి def_విలువ;
  ఒకవేళ (endptr == content || (*endptr && *endptr != '\n'))
    తిరిగి def_విలువ;
  తిరిగి val;
}

#నిర్వచించు DEFAULT_అధికప్రవాహం_ID 65534

uid_t
get_అధికప్రవాహం_uid (శూన్య)
{
  స్థిర atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  ఒకవేళ (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_అధికప్రవాహం_ID);
      cached_uid = uid;
    }
  తిరిగి uid;
}

gid_t
get_అధికప్రవాహం_gid (శూన్య)
{
  స్థిర atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  ఒకవేళ (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_అధికప్రవాహం_ID);
      cached_gid = gid;
    }
  తిరిగి gid;
}

శూన్య
consume_trailing_slashes (అక్షరం *మార్గం)
{
  ఒకవేళ (! మార్గం || మార్గం[0] == '\0')
    తిరిగి;

  అక్షరం *చివరి = మార్గం + అక్షరశ్రేణి_పొడవు (మార్గం);

  అయితే (చివరి > మార్గం && *(చివరి - 1) == '/')
    చివరి--;

  *చివరి = '\0';
}

అక్షరం **
read_dir_entries (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_dir DIR *dir = ఖాళీ;
  size_t n_entries = 0;
  size_t entries_కొలత = 16;
  అక్షరం **entries = ఖాళీ;
  నిర్మాణం dirent *de;

  dir = opendir (మార్గం);
  ఒకవేళ (UNLIKELY (dir == ఖాళీ))
    {
      crun_make_error (err, errno, "opendir `%s`", మార్గం);
      తిరిగి ఖాళీ;
    }

  entries = xmalloc (entries_కొలత * పరిమాణం (అక్షరం *));
  అయితే ((de = readdir (dir)))
    {
      ఒకవేళ (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
        కొనసాగు;
      ఒకవేళ (n_entries == entries_కొలత)
        {
          entries_కొలత *= 2;
          entries = xrealloc (entries, entries_కొలత * పరిమాణం (అక్షరం *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * పరిమాణం (అక్షరం *));
  entries[n_entries] = ఖాళీ;

  తిరిగి entries;
}

పూర్ణాంకం
cpuset_string_గమ్యం_bitmask (స్థిరరాశి అక్షరం *str, అక్షరం **బహిర్గతం, size_t *బహిర్గతం_కొలత, libcrun_error_t *err)
{
  cleanup_free అక్షరం *ముసుగు = ఖాళీ;
  size_t ముసుగు_కొలత = 0;
  స్థిరరాశి అక్షరం *p = str;
  అక్షరం *endptr;

  అయితే (*p)
    {
      దీర్ఘ దీర్ఘ ప్రారంభం_range, చివర_range;

      ఒకవేళ (*p < '0' || *p > '9')
        వెళ్ళు invalid_ఆదానం;

      ప్రారంభం_range = strtoll (p, &endptr, 10);
      ఒకవేళ (ప్రారంభం_range < 0)
        వెళ్ళు invalid_ఆదానం;

      p = endptr;

      ఒకవేళ (*p != '-')
        చివర_range = ప్రారంభం_range;
      లేకపోతే
        {
          p++;

          ఒకవేళ (*p < '0' || *p > '9')
            వెళ్ళు invalid_ఆదానం;

          చివర_range = strtoll (p, &endptr, 10);

          ఒకవేళ (చివర_range < ప్రారంభం_range)
            వెళ్ళు invalid_ఆదానం;

          p = endptr;
        }

      /* Just set some limit.  */
      ఒకవేళ (చివర_range > (1 << 20))
        వెళ్ళు invalid_ఆదానం;

      ఒకవేళ (చివర_range >= (దీర్ఘ దీర్ఘ) (ముసుగు_కొలత * CHAR_BIT))
        {
          size_t new_ముసుగు_కొలత = (చివర_range / CHAR_BIT) + 1;
          ముసుగు = xrealloc (ముసుగు, new_ముసుగు_కొలత);
          memset (ముసుగు + ముసుగు_కొలత, 0, new_ముసుగు_కొలత - ముసుగు_కొలత);
          ముసుగు_కొలత = new_ముసుగు_కొలత;
        }

      కొరకు (దీర్ఘ దీర్ఘ i = ప్రారంభం_range; i <= చివర_range; i++)
        ముసుగు[i / CHAR_BIT] |= (1 << (i % CHAR_BIT));

      ఒకవేళ (*p == ',')
        p++;
      లేకపోతే ఒకవేళ (*p)
        వెళ్ళు invalid_ఆదానం;
    }

  *బహిర్గతం = ముసుగు;
  ముసుగు = ఖాళీ;
  *బహిర్గతం_కొలత = ముసుగు_కొలత;

  తిరిగి 0;

invalid_ఆదానం:
  తిరిగి crun_make_error (err, 0, "cannot parse input `%s`", str);
}

నిర్మాణం channel_fd_pair
{
  నిర్మాణం ring_బఫరు *rb;

  పూర్ణాంకం లోపల_fd;
  పూర్ణాంకం బహిర్గతం_fd;

  పూర్ణాంకం infd_epoll_events;
  పూర్ణాంకం outfd_epoll_events;
};

నిర్మాణం channel_fd_pair *
channel_fd_pair_new (పూర్ణాంకం లోపల_fd, పూర్ణాంకం బహిర్గతం_fd, size_t కొలత)
{
  నిర్మాణం channel_fd_pair *channel = xmalloc (పరిమాణం (నిర్మాణం channel_fd_pair));
  channel->లోపల_fd = లోపల_fd;
  channel->బహిర్గతం_fd = బహిర్గతం_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_బఫరు_make (కొలత);
  తిరిగి channel;
}

శూన్య
channel_fd_pair_free (నిర్మాణం channel_fd_pair *channel)
{
  ఒకవేళ (channel == ఖాళీ)
    తిరిగి;

  ring_బఫరు_free (channel->rb);
  విడుదల (channel);
}

పూర్ణాంకం
channel_fd_pair_process (నిర్మాణం channel_fd_pair *channel, పూర్ణాంకం epollfd, libcrun_error_t *err)
{
  bool is_ఆదానం_eagain = false, is_output_eagain = false, repeat;
  పూర్ణాంకం ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  కొరకు (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      ఒకవేళ (ring_బఫరు_get_space_అందుబాటు (channel->rb) > 0)
        {
          ret = ring_బఫరు_read (channel->rb, channel->లోపల_fd, &is_ఆదానం_eagain, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          ఒకవేళ (ret > 0)
            repeat = true;
        }
      ఒకవేళ (ring_బఫరు_get_దత్తాంశం_అందుబాటు (channel->rb) > 0)
        {
          ret = ring_బఫరు_write (channel->rb, channel->బహిర్గతం_fd, &is_output_eagain, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          ఒకవేళ (ret > 0)
            repeat = true;
        }
    }

  ఒకవేళ (epollfd >= 0)
    {
      size_t అందుబాటు = ring_బఫరు_get_space_అందుబాటు (channel->rb);
      size_t వాడిన = ring_బఫరు_get_దత్తాంశం_అందుబాటు (channel->rb);
      పూర్ణాంకం events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (అందుబాటు > 0 && ! is_output_eagain) ? (EPOLLIN | (is_ఆదానం_eagain ? EPOLLET : 0)) : 0;
      ఒకవేళ (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->లోపల_fd, events, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (వాడిన > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      ఒకవేళ (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->బహిర్గతం_fd, events, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          channel->outfd_epoll_events = events;
        }
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
open_proc (libcrun_error_t *err)
{
  పూర్ణాంకం fd;

#నిర్వచితమైతే HAVE_NEW_MOUNT_API
  cleanup_close పూర్ణాంకం fsfd = -1;
  పూర్ణాంకం ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  ఒకవేళ (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ఖాళీ, ఖాళీ, 0);
      ఒకవేళ (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          ఒకవేళ (fd >= 0)
            తిరిగి fd;
        }
    }
#అయితే_ముగింపు

  fd = open ("/proc", O_మార్గం | O_DIRECTORY | O_CLOEXEC);
  ఒకవేళ (fd < 0)
    తిరిగి crun_make_error (err, errno, "open `/proc`");
  తిరిగి fd;
}

పూర్ణాంకం
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  ఒకవేళ (! container)
    తిరిగి open_proc (err);

  ఒకవేళ (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  తిరిగి container->proc_fd;
}

పూర్ణాంకం
libcrun_open_proc_file (libcrun_container_t *container, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం flags, libcrun_error_t *err)
{
  పూర్ణాంకం proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  ఒకవేళ (proc_fd < 0)
    తిరిగి proc_fd;

  fd = తాత్కాలిక_FAILURE_RETRY (openat (proc_fd, మార్గం, flags | O_CLOEXEC));
  saved_errno = errno;
  /* requires closing if not cached */
  ఒకవేళ (! container)
    తాత్కాలిక_FAILURE_RETRY (close (proc_fd));

  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, saved_errno, "openat `/proc/%s`", మార్గం);

  తిరిగి fd;
}

పూర్ణాంకం
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం flags, libcrun_error_t *err)
{
  cleanup_free అక్షరం *నిండిన_మార్గం = ఖాళీ;
  xasprintf (&నిండిన_మార్గం, "%d/%s", pid, మార్గం);
  తిరిగి libcrun_open_proc_file (container, నిండిన_మార్గం, flags, err);
}
