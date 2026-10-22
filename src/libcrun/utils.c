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

#घोषणा _GNU_SOURCE
#समावेश <config.h>
#समावेश "utils.h"
#समावेश "ring_buffer.h"
#समावेश <stdarg.h>
#समावेश <inttypes.h>
#समावेश <unistd.h>
#समावेश <string.h>
#समावेश <libgen.h>
#समावेश <fcntl.h>
#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#समावेश <sys/un.h>
#समावेश <sys/socket.h>
#समावेश <sys/signalfd.h>
#समावेश <sys/epoll.h>
#समावेश <sys/syscall.h>
#समावेश <sys/wait.h>
#समावेश <pwd.h>
#समावेश <grp.h>
#समावेश <time.h>
#समावेश <sys/time.h>
#समावेश <sys/xattr.h>
#समावेश <sys/sysmacros.h>
#समावेश <sys/vfs.h>
#समावेश <linux/magic.h>
#समावेश <limits.h>
#समावेश <sys/mman.h>
#अगर_घोषित HAVE_LINUX_OPENAT2_H
#  include <linux/openat2.h>
#पूर्ण_अगर
#अगर HAVE_STDATOMIC_H
#  include <stdatomic.h>
#अन्यथा
#  define atomic_long अस्थिर दीर्घ
#  define atomic_int अस्थिर पूर्णांक
#पूर्ण_अगर
#समावेश "syscalls.h"

#अगर_अघोषित CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#पूर्ण_अगर
#अगर_अघोषित RESOLVE_IN_मूल
#  define RESOLVE_IN_मूल 0x10
#पूर्ण_अगर
#अगर_अघोषित __NR_close_range
#  define __NR_close_range 436
#पूर्ण_अगर
#अगर_अघोषित __NR_openat2
#  define __NR_openat2 437
#पूर्ण_अगर
#अगर_अघोषित PROC_USER_INIT_INO
#  define PROC_USER_INIT_INO ((ino_t) 0xEFFFFFFD)
#पूर्ण_अगर

#घोषणा MAX_READLINKS 32
#घोषणा MAX_OPENAT2_EAGAIN_RETRIES 128

पूर्णांक
crun_path_exists (स्थिर अक्षर *path, libcrun_error_t *err)
{
  पूर्णांक ret = access (path, F_OK);
  अगर (ret < 0)
    {
      अगर (त्रुटि_सं == ENOENT)
        वापस 0;
      वापस crun_make_error (err, त्रुटि_सं, "access `%s`", path);
    }
  वापस 1;
}

पूर्णांक
xasprintf (अक्षर **str, स्थिर अक्षर *fmt, ...)
{
  पूर्णांक ret;
  बहु_सूची args_list;

  बहु_शुरू (args_list, fmt);

  ret = vasprintf (str, fmt, args_list);
  अगर (UNLIKELY (ret < 0))
    OOM ();

  बहु_पूर्ण (args_list);
  वापस ret;
}

पूर्णांक
write_file_at_with_flags (पूर्णांक dirfd, पूर्णांक flags, mode_t mode, स्थिर अक्षर *name, स्थिर व्योम *तथ्य, माप_प्रकार len, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret = 0;

  fd = openat (dirfd, name, O_CLOEXEC | O_WRONLY | flags, mode);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s` for writing", name);

  ret = safe_write (fd, name, तथ्य, len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस (len > पूर्णांक_उच्च) ? पूर्णांक_उच्च : (पूर्णांक) len;
}

पूर्णांक
detach_process ()
{
  pid_t pid;
  अगर (setsid () < 0)
    वापस -1;
  pid = विभाजन ();
  अगर (pid < 0)
    वापस -1;
  अगर (pid != 0)
    _safe_exit (निकास_सफल);
  वापस 0;
}

पूर्णांक
get_file_type_fd (पूर्णांक fd, mode_t *mode)
{
  काष्ठा stat st;
  पूर्णांक ret;

#अगर_घोषित HAVE_STATX
  काष्ठा statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        जाओ fallback;

      वापस ret;
    }
  *mode = stx.stx_mode;
  वापस ret;

fallback:
#पूर्ण_अगर
  ret = ख_स्थिति (fd, &st);
  *mode = st.st_mode;
  वापस ret;
}

पूर्णांक
get_file_type_at (पूर्णांक dirfd, mode_t *mode, bool nofollow, स्थिर अक्षर *path)
{
  पूर्णांक empty_path = path == शून्य ? AT_EMPTY_PATH : 0;
  काष्ठा stat st;
  पूर्णांक ret;

#अगर_घोषित HAVE_STATX
  काष्ठा statx stx = {
    0,
  };

  ret = statx (dirfd, path ?: "", empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | AT_STATX_DONT_SYNC, STATX_TYPE, &stx);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        जाओ fallback;

      वापस ret;
    }
  *mode = stx.stx_mode;
  वापस ret;

fallback:
#पूर्ण_अगर
  ret = fstatat (dirfd, path ?: "", &st, empty_path | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  *mode = st.st_mode;
  वापस ret;
}

पूर्णांक
get_file_type (mode_t *mode, bool nofollow, स्थिर अक्षर *path)
{
  वापस get_file_type_at (AT_FDCWD, mode, nofollow, path);
}

पूर्णांक
create_file_if_missing_at (पूर्णांक dirfd, स्थिर अक्षर *file, mode_t mode, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd_write = openat (dirfd, file, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, mode);
  अगर (fd_write < 0)
    {
      पूर्णांक saved_errno = त्रुटि_सं;
      mode_t tmp_mode;
      पूर्णांक ret;

      /* On errors, check if the file already exists.  */
      ret = get_file_type_at (dirfd, &tmp_mode, true, file);
      अगर (ret == 0 && S_ISREG (tmp_mode))
        वापस 0;

      वापस crun_make_error (err, saved_errno, "create file `%s`", file);
    }
  वापस 0;
}

अटल पूर्णांक
ensure_directory_internal_at (पूर्णांक dirfd, अक्षर *path, माप_प्रकार len, पूर्णांक mode, libcrun_error_t *err)
{
  अक्षर *it = path + len;
  पूर्णांक ret = 0;
  bool parent_created = false;

  क्रम (;;)
    {
      ret = mkdirat (dirfd, path, mode);
      अगर (ret == 0 || त्रुटि_सं == EEXIST)
        वापस 0;

      पूर्णांक saved_errno = त्रुटि_सं;
      अगर (parent_created || त्रुटि_सं != ENOENT)
        {
          libcrun_error_t tmp_err = शून्य;

          /* On errors check if the directory already exists.  */
          ret = crun_dir_p (path, false, &tmp_err);
          अगर (ret > 0)
            वापस 0;
          अगर (ret < 0)
            crun_error_release (&tmp_err);

          वापस crun_make_error (err, saved_errno, "create directory `%s`", path);
        }

      जबतक (it > path && *it != '/')
        {
          it--;
          len--;
        }
      अगर (it == path)
        वापस 0;

      *it = '\0';
      ret = ensure_directory_internal_at (dirfd, path, len - 1, mode, err);
      *it = '/';
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      parent_created = true;
    }
  वापस ret;
}

पूर्णांक
crun_ensure_directory_at (पूर्णांक dirfd, स्थिर अक्षर *path, पूर्णांक mode, bool nofollow, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *tmp = xstrdup (path);
  ret = ensure_directory_internal_at (dirfd, tmp, म_माप (tmp), mode, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = crun_dir_p_at (dirfd, path, nofollow, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (ret == 0)
    वापस crun_make_error (err, ENOTDIR, "the path `%s` is not a directory", path);

  वापस 0;
}

अटल पूर्णांक
check_fd_is_path (स्थिर अक्षर *path, पूर्णांक fd, स्थिर अक्षर *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  माप_प्रकार path_len = म_माप (path);
  अक्षर link[PATH_MAX];
  पूर्णांक ret;

  get_proc_self_fd_path (fdpath, fd);
  ret = अस्थायी_FAILURE_RETRY (readlink (fdpath, link, माप (link)));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "readlink `%s`", fdname);

  अगर (((माप_प्रकार) ret) != path_len || स_भेद (link, path, path_len))
    वापस crun_make_error (err, 0, "target `%s` does not point to the directory `%s`", fdname, path);

  वापस 0;
}

अटल पूर्णांक
check_fd_under_path (स्थिर अक्षर *rootfs, माप_प्रकार rootfslen, पूर्णांक fd, स्थिर अक्षर *fdname, libcrun_error_t *err)
{
  proc_fd_path_t fdpath;
  अक्षर link[PATH_MAX];
  पूर्णांक ret;

  /* Every path is under "/", there is nothing to verify.  The check below
     would reject any path since it expects a '/' right after the rootfs.  */
  अगर (rootfslen == 0 || (rootfslen == 1 && rootfs[0] == '/'))
    वापस 0;

  get_proc_self_fd_path (fdpath, fd);
  ret = अस्थायी_FAILURE_RETRY (readlink (fdpath, link, माप (link)));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "readlink `%s`", fdname);

  अगर (((माप_प्रकार) ret) <= rootfslen || स_भेद (link, rootfs, rootfslen) != 0 || link[rootfslen] != '/')
    वापस crun_make_error (err, 0, "target `%s` not under the directory `%s`", fdname, rootfs);

  वापस 0;
}

/* Check if *oldfd is a valid fd and close it.  Then store newfd into *oldfd.  */
अटल व्योम
close_and_replace (पूर्णांक *oldfd, पूर्णांक newfd)
{
  अगर (*oldfd >= 0)
    अस्थायी_FAILURE_RETRY (बंद (*oldfd));

  *oldfd = newfd;
}

/* Defined in chroot_realpath.c  */
अक्षर *chroot_realpath (स्थिर अक्षर *chroot, स्थिर अक्षर *path, अक्षर resolved_path[]);

/* DIRFD must be a file descriptor for ROOTFS itself: PATH is resolved
   against ROOTFS and the result is then opened relatively to DIRFD.  */
अटल पूर्णांक
safe_openat_fallback (पूर्णांक dirfd, स्थिर अक्षर *rootfs, स्थिर अक्षर *path, पूर्णांक flags,
                      पूर्णांक mode, libcrun_error_t *err)
{
  cleanup_free अक्षर *parent_path = शून्य;
  स्थिर अक्षर *last_component = शून्य;
  स्थिर अक्षर *orig_path = path;
  स्थिर अक्षर *path_in_chroot;
  cleanup_close पूर्णांक fd = -1;
  अक्षर resolved[PATH_MAX];
  अक्षर बफर[PATH_MAX];
  माप_प्रकार rootfs_len = safe_strlen (rootfs);
  पूर्णांक ret;

  /* chroot_realpath resolves the last component as well, so when O_NOFOLLOW
     is requested resolve only the parent directory and let openat(2) deal
     with the last component, otherwise a symlink would be followed even
     though the caller asked not to.  */
  अगर (flags & O_NOFOLLOW)
    {
      अक्षर *sep;

      parent_path = xstrdup (path);
      sep = म_खोजप (parent_path, '/');
      अगर (sep == शून्य)
        {
          /* No parent directory, the entire path is the last component.  */
          last_component = path;
          parent_path[0] = '\0';
        }
      अन्यथा अगर (sep[1] != '\0')
        {
          *sep = '\0';
          last_component = path + (sep - parent_path) + 1;
        }
      /* A trailing '/' forces the symlink to be resolved anyway, so in that
         case keep resolving the entire path.  */

      अगर (last_component)
        path = parent_path;
    }

  path_in_chroot = chroot_realpath (rootfs, path, बफर);
  अगर (path_in_chroot == शून्य)
    वापस crun_make_error (err, त्रुटि_सं, "cannot resolve `%s` under rootfs", orig_path);

  /* When rootfs is "/" or not set, chroot_realpath returns the path
     unchanged, so drop the prefix only when it is really there.  */
  अगर (rootfs_len > 0 && म_भेदन (path_in_chroot, rootfs, rootfs_len) == 0)
    path_in_chroot += rootfs_len;
  path_in_chroot = consume_slashes (path_in_chroot);

  अगर (last_component)
    {
      ret = snprintf (resolved, माप (resolved), "%s%s%s", path_in_chroot,
                      path_in_chroot[0] == '\0' ? "" : "/", last_component);
      अगर (UNLIKELY (ret >= (पूर्णांक) माप (resolved)))
        वापस crun_make_error (err, ENAMETOOLONG, "resolve `%s` under rootfs", orig_path);

      path_in_chroot = resolved;
    }

  /* If the path is empty we are at the root, dup the dirfd itself.  */
  अगर (path_in_chroot[0] == '\0')
    {
      ret = dup (dirfd);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "dup `%s`", rootfs_len ? rootfs : "/");
      वापस ret;
    }

  ret = openat (dirfd, path_in_chroot, flags, mode);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", orig_path);

  fd = ret;

  ret = check_fd_under_path (rootfs, rootfs_len, fd, orig_path, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = fd;
  fd = -1;
  वापस ret;
}

पूर्णांक
safe_openat (पूर्णांक dirfd, स्थिर अक्षर *rootfs, स्थिर अक्षर *path, पूर्णांक flags, पूर्णांक mode,
             libcrun_error_t *err)
{
  अटल bool openat2_supported = true;
  पूर्णांक retries = MAX_OPENAT2_EAGAIN_RETRIES;
  पूर्णांक ret;

  अगर (is_empty_string (path))
    {
      cleanup_close पूर्णांक fd = -1;

      /* A container without a rootfs of its own uses the host root.  */
      अगर (is_empty_string (rootfs))
        rootfs = "/";

      fd = खोलो (rootfs, flags, mode);
      अगर (UNLIKELY (fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

      /* Skip the readlink-based check when opening the root
         directory itself (rootfs="/", path="").  After pivot_root,
         "/" can only refer to the container root so the readlink
         verification is redundant, and after setns the /proc-based
         readlink may not be reachable by path yet.  */
      अगर (rootfs[0] != '/' || rootfs[1] != '\0')
        {
          ret = check_fd_is_path (rootfs, fd, path, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      ret = fd;
      fd = -1;
      वापस ret;
    }

  अगर (openat2_supported)
    {
    repeat:
      ret = syscall_openat2 (dirfd, path, flags, mode, RESOLVE_IN_मूल);
      अगर (UNLIKELY (ret < 0))
        {
          अगर (त्रुटि_सं == EINTR)
            जाओ repeat;
          /* With RESOLVE_IN_ROOT the kernel returns EAGAIN when a rename or a
             mount happened anywhere on the system while a ".." component was
             being resolved.  Any process can cause that, so give up after a
             bounded number of attempts instead of retrying forever.  */
          अगर (त्रुटि_सं == EAGAIN && retries-- > 0)
            जाओ repeat;
          अगर (त्रुटि_सं == ENOSYS)
            openat2_supported = false;
          अगर (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL || त्रुटि_सं == EPERM)
            वापस safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);

          वापस crun_make_error (err, त्रुटि_सं, "openat2 `%s`", path);
        }

      वापस ret;
    }

  वापस safe_openat_fallback (dirfd, rootfs, path, flags, mode, err);
}

ssize_t
safe_readlinkat (पूर्णांक dfd, स्थिर अक्षर *name, अक्षर **बफर, ssize_t hint, libcrun_error_t *err)
{
  /* Add 1 to make room for the NUL terminator.  */
  ssize_t buf_आकार = hint > 0 ? hint + 1 : 512;
  cleanup_free अक्षर *tmp_buf = शून्य;
  ssize_t आकार;

  करो
    {
      अगर (tmp_buf != शून्य)
        buf_आकार += 256;

      tmp_buf = xrealloc (tmp_buf, buf_आकार);

      आकार = readlinkat (dfd, name, tmp_buf, buf_आकार);
      अगर (UNLIKELY (आकार < 0))
        वापस crun_make_error (err, त्रुटि_सं, "readlink `%s`", name);
  } जबतक (आकार == buf_आकार);

  /* Always NUL terminate the buffer.  */
  tmp_buf[आकार] = '\0';

  /* Move ownership to BUFFER.  */
  *बफर = tmp_buf;
  tmp_buf = शून्य;

  वापस आकार;
}

अटल पूर्णांक crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, पूर्णांक dirfd, स्थिर अक्षर *dirpath,
                                स्थिर अक्षर *path, पूर्णांक mode, पूर्णांक max_readlinks, libcrun_error_t *err);

/* CUR is a symlink in the directory CWD, NPATH is the path up to and
   including CUR, and REST is what follows CUR in the path, or NULL.
   Restart the lookup with the symlink target in place of CUR, so that
   anything missing on the way to the target is created.  */
अटल पूर्णांक
crun_safe_ensure_follow_symlink (bool do_open, bool dir, bool follow_symlinks, पूर्णांक dirfd, स्थिर अक्षर *dirpath, पूर्णांक cwd,
                                 स्थिर अक्षर *npath, स्थिर अक्षर *cur, स्थिर अक्षर *rest,
                                 पूर्णांक mode, पूर्णांक max_readlinks, libcrun_error_t *err)
{
  cleanup_free अक्षर *target = शून्य;
  cleanup_free अक्षर *new_path = शून्य;
  ssize_t ret;

  ret = safe_readlinkat (cwd, cur, &target, 0, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* A relative target is relative to the directory containing the symlink,
     which is the path before CUR.  */
  xasprintf (&new_path, "%.*s%s%s%s", target[0] == '/' ? 0 : (पूर्णांक) (cur - npath), npath, target,
             rest ? "/" : "", rest ? rest : "");

  वापस crun_safe_ensure_at (do_open, dir, follow_symlinks, dirfd, dirpath, new_path, mode, max_readlinks - 1, err);
}

अटल पूर्णांक
crun_safe_ensure_at (bool do_open, bool dir, bool follow_symlinks, पूर्णांक dirfd, स्थिर अक्षर *dirpath,
                     स्थिर अक्षर *path, पूर्णांक mode, पूर्णांक max_readlinks, libcrun_error_t *err)
{
  cleanup_close पूर्णांक wd_cleanup = -1;
  cleanup_free अक्षर *npath = शून्य;
  bool last_component = false;
  माप_प्रकार depth = 0;
  अक्षर *cur;
  अक्षर *it;
  पूर्णांक cwd;
  पूर्णांक ret;

  अगर (max_readlinks <= 0)
    वापस crun_make_error (err, ELOOP, "resolve path `%s`", path);

  path = consume_slashes (path);

  /* Empty path, nothing to do.  */
  अगर (*path == '\0')
    {
      अगर (do_open)
        वापस खोलो (dirpath, O_CLOEXEC | O_PATH, 0);
      वापस 0;
    }

  npath = xstrdup (path);

  cwd = dirfd;
  cur = npath;
  it = म_अक्षर (npath, '/');
  जबतक (cur)
    {
      अगर (it)
        *it = '\0';
      अन्यथा
        last_component = true;

      अगर (cur[0] == '\0')
        अवरोध;

      अगर (म_भेद (cur, ".") == 0)
        जाओ अग्र;
      अन्यथा अगर (म_भेद (cur, ".."))
        depth++;
      अन्यथा
        {
          अगर (depth)
            depth--;
          अन्यथा
            {
              /* Start from the root.  */
              close_and_reset (&wd_cleanup);
              cwd = dirfd;
              जाओ अग्र;
            }
        }

      अगर (last_component && ! dir)
        {
          ret = openat (cwd, cur, O_CLOEXEC | O_CREAT | O_WRONLY | O_NOFOLLOW | O_NONBLOCK, 0700);
          अगर (UNLIKELY (ret < 0))
            {
              /* If the last component is a symlink, repeat the lookup with the resolved path.
                 That lookup is confined to DIRFD, so its result is final: an openat without
                 O_NOFOLLOW would follow the symlink from the process root instead.  */
              अगर (त्रुटि_सं == ELOOP)
                वापस crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur, शून्य,
                                                        mode, max_readlinks, err);

              /* If the previous openat fails, attempt to open the file in O_PATH mode.  */
              ret = openat (cwd, cur, O_CLOEXEC | O_PATH | O_NOFOLLOW, 0);
              अगर (ret < 0)
                वापस crun_make_error (err, त्रुटि_सं, "open `%s/%s`", dirpath, npath);
            }

          अगर (do_open)
            वापस ret;

          close_and_replace (&wd_cleanup, ret);
          वापस 0;
        }

      ret = mkdirat (cwd, cur, mode);
      अगर (ret < 0)
        {
          mode_t st_mode;

          अगर (त्रुटि_सं != EEXIST)
            वापस crun_make_error (err, त्रुटि_सं, "mkdir `/%s`", npath);

          /* If asked to, follow a symlink here, so that a dangling one gets
             its target created, and the lookup of a relative target or a ".."
             after it starts from the right directory.  This is done for mount
             targets, not for the working directory, where both crun and runc
             have always failed with a dangling symlink.  */
          अगर (follow_symlinks && get_file_type_at (cwd, &st_mode, true, cur) == 0 && S_ISLNK (st_mode))
            वापस crun_safe_ensure_follow_symlink (do_open, dir, follow_symlinks, dirfd, dirpath, cwd, npath, cur,
                                                    it ? it + 1 : शून्य, mode, max_readlinks, err);
        }

      /* The type of an existing component is not known yet, so open it with
         O_PATH: a FIFO must not block and a device must not be opened.  */
      cwd = safe_openat (dirfd, dirpath, npath, O_PATH | O_CLOEXEC, 0, err);
      अगर (UNLIKELY (cwd < 0))
        वापस crun_error_wrap (err, "creating `/%s`", path);

      अगर (! last_component)
        {
          mode_t st_mode;

          ret = get_file_type_at (cwd, &st_mode, true, शून्य);
          अगर (UNLIKELY (ret < 0))
            {
              पूर्णांक saved_errno = त्रुटि_सं;

              बंद (cwd);
              वापस crun_make_error (err, saved_errno, "stat `%s`", npath);
            }
          अगर ((st_mode & S_IFMT) != S_IFDIR)
            {
              बंद (cwd);
              वापस crun_make_error (err, ENOTDIR, "error creating directory `%s` since `%s` exists and it is not a directory", path, npath);
            }
        }

      close_and_replace (&wd_cleanup, cwd);

    अग्र:
      अगर (it == शून्य)
        अवरोध;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = म_अक्षर (cur, '/');
    }

  अगर (do_open)
    {
      अगर (cwd == dirfd)
        {
          ret = dup (dirfd);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "dup `%s`", dirpath);
          वापस ret;
        }

      wd_cleanup = -1;
      वापस cwd;
    }

  वापस 0;
}

पूर्णांक
crun_safe_create_and_open_ref_at (bool dir, पूर्णांक dirfd, स्थिर अक्षर *dirpath, स्थिर अक्षर *path, पूर्णांक mode, libcrun_error_t *err)
{
  पूर्णांक ret;

  /* If the file/dir already exists, just open it.  */
  ret = safe_openat (dirfd, dirpath, path, O_PATH | O_CLOEXEC, 0, err);
  अगर (LIKELY (ret >= 0))
    वापस ret;

  crun_error_release (err);
  वापस crun_safe_ensure_at (true, dir, true, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णांक
crun_safe_ensure_directory_at (पूर्णांक dirfd, स्थिर अक्षर *dirpath, स्थिर अक्षर *path, पूर्णांक mode,
                               libcrun_error_t *err)
{
  वापस crun_safe_ensure_at (false, true, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णांक
crun_safe_ensure_file_at (पूर्णांक dirfd, स्थिर अक्षर *dirpath, स्थिर अक्षर *path, पूर्णांक mode,
                          libcrun_error_t *err)
{
  वापस crun_safe_ensure_at (false, false, false, dirfd, dirpath, path, mode, MAX_READLINKS, err);
}

पूर्णांक
crun_ensure_directory (स्थिर अक्षर *path, पूर्णांक mode, bool nofollow, libcrun_error_t *err)
{
  वापस crun_ensure_directory_at (AT_FDCWD, path, mode, nofollow, err);
}

अटल पूर्णांक
get_file_आकार (पूर्णांक fd, off_t *आकार)
{
  काष्ठा stat st;
  पूर्णांक ret;
#अगर_घोषित HAVE_STATX
  काष्ठा statx stx = {
    0,
  };

  ret = statx (fd, "", AT_EMPTY_PATH | AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC, STATX_आकार, &stx);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        जाओ fallback;
      वापस ret;
    }
  *आकार = stx.stx_size;

  वापस ret;

fallback:
#पूर्ण_अगर
  ret = ख_स्थिति (fd, &st);
  *आकार = st.st_size;
  वापस ret;
}

पूर्णांक
crun_dir_p_at (पूर्णांक dirfd, स्थिर अक्षर *path, bool nofollow, libcrun_error_t *err)
{
  mode_t mode;
  पूर्णांक ret;

  ret = get_file_type_at (dirfd, &mode, nofollow, path);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "stat `%s`", path);

  वापस S_ISDIR (mode) ? 1 : 0;
}

पूर्णांक
crun_dir_p (स्थिर अक्षर *path, bool nofollow, libcrun_error_t *err)
{
  वापस crun_dir_p_at (AT_FDCWD, path, nofollow, err);
}

/* Detect the user namespace from the mapping in /proc/self/uid_map.  It cannot
   tell apart the initial user namespace from a user namespace with a full
   identity mapping, so it is used only as a fallback.  */
अटल पूर्णांक
check_running_in_user_namespace_uid_map (libcrun_error_t *err)
{
  cleanup_free अक्षर *बफर = शून्य;
  माप_प्रकार len;
  पूर्णांक ret;

  ret = read_all_file ("/proc/self/uid_map", &बफर, &len, err);
  अगर (UNLIKELY (ret < 0))
    {
      /* If the file does not exist, then the kernel does not support user namespaces and we for sure aren't in one.  */
      अगर (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          वापस 0;
        }
      वापस ret;
    }

  वापस म_माला (बफर, "4294967295") ? 0 : 1;
}

/* OpenVZ exposes /proc/vz both on the host and inside containers, while
   /proc/bc is only exposed on the host.  This follows systemd's OpenVZ
   detection.  */
अटल bool
running_in_openvz ()
{
  अगर (access ("/proc/vz", F_OK) < 0)
    वापस false;

  वापस access ("/proc/bc", F_OK) < 0 && त्रुटि_सं == ENOENT;
}

पूर्णांक
check_running_in_user_namespace (libcrun_error_t *err)
{
  अटल atomic_int run_in_userns = -1;
  काष्ठा stat st;
  पूर्णांक ret;

  ret = run_in_userns;
  अगर (ret >= 0)
    वापस ret;

  ret = stat ("/proc/self/ns/user", &st);
  अगर (ret == 0)
    {
      /* The inode is definitive when it matches.  OpenVZ virtualizes the
         namespace inode numbers exposed through procfs, so a mismatch must fall
         back to the uid_map detection there.  */
      अगर (st.st_ino == PROC_USER_INIT_INO)
        {
          run_in_userns = 0;
          वापस run_in_userns;
        }

      अगर (! running_in_openvz ())
        {
          run_in_userns = 1;
          वापस run_in_userns;
        }
    }
  अन्यथा अगर (UNLIKELY (त्रुटि_सं != ENOENT))
    वापस crun_make_error (err, त्रुटि_सं, "stat `/proc/self/ns/user`");

  /* Kernels older than 3.8 do not expose the user namespace here even though
     they can still support user namespaces, so the uid_map is all we have.  */
  ret = check_running_in_user_namespace_uid_map (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  run_in_userns = ret;
  वापस run_in_userns;
}

अटल माप_प्रकार
get_page_आकार ()
{
  अटल atomic_long cached_pagesize = 0;
  atomic_long pagesize = cached_pagesize;
  अगर (pagesize == 0)
    {
      pagesize = (दीर्घ) sysconf (_SC_PAGESIZE);
      cached_pagesize = pagesize;
    }
  वापस (माप_प्रकार) pagesize;
}

अटल पूर्णांक selinux_enabled = -1;
अटल पूर्णांक apparmor_enabled = -1;

पूर्णांक
libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_free अक्षर *out = शून्य;
  cleanup_close पूर्णांक fd = -1;
  माप_प्रकार len;
  पूर्णांक ret;

  अगर (selinux_enabled >= 0)
    वापस selinux_enabled;

  fd = libcrun_open_proc_file (container, "mounts", O_RDONLY, err);
  अगर (UNLIKELY (fd < 0))
    वापस fd;

  ret = read_all_fd_with_आकार_hint (fd, "mounts", &out, &len, get_page_आकार (), err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  selinux_enabled = म_माला (out, "selinux") ? 1 : 0;

  वापस selinux_enabled;
}

पूर्णांक
libcrun_initialize_apparmor (libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक आकार;
  अक्षर buf[1];

  अगर (apparmor_enabled >= 0)
    वापस apparmor_enabled;

  fd = खोलो ("/sys/module/apparmor/parameters/enabled", O_RDONLY | O_CLOEXEC);
  अगर (fd == -1)
    {
      अगर (त्रुटि_सं == ENOENT)
        {
          apparmor_enabled = 0;
          वापस 0;
        }

      वापस crun_make_error (err, त्रुटि_सं, "open `/sys/module/apparmor/parameters/enabled`");
    }

  आकार = अस्थायी_FAILURE_RETRY (पढ़ो (fd, buf, माप (buf)));

  apparmor_enabled = आकार > 0 && buf[0] == 'Y' ? 1 : 0;

  वापस apparmor_enabled;
}

अटल पूर्णांक
libcrun_is_selinux_enabled (libcrun_error_t *err)
{
  अगर (selinux_enabled < 0)
    वापस crun_make_error (err, 0, "SELinux is not initialized correctly");
  वापस selinux_enabled;
}

पूर्णांक
add_selinux_mount_label (अक्षर **retlabel, स्थिर अक्षर *तथ्य, स्थिर अक्षर *label, स्थिर अक्षर *context_type, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_is_selinux_enabled (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (label && ret)
    {
      अगर (तथ्य && *तथ्य)
        xasprintf (retlabel, "%s,%s=\"%s\"", तथ्य, context_type, label);
      अन्यथा
        xasprintf (retlabel, "%s=\"%s\"", context_type, label);
      वापस 0;
    }
  *retlabel = xstrdup (तथ्य);
  वापस 0;
}

अटल स्थिर अक्षर *
lsm_attr_path (libcrun_container_t *container, स्थिर अक्षर *lsm, स्थिर अक्षर *fname, libcrun_error_t *err)
{
  cleanup_close पूर्णांक attr_dirfd = -1;
  cleanup_close पूर्णांक lsm_dirfd = -1;
  अक्षर *attr_path = शून्य;

  attr_dirfd = libcrun_open_proc_file (container, "thread-self/attr", O_DIRECTORY | O_PATH, err);
  अगर (UNLIKELY (attr_dirfd < 0))
    वापस शून्य;

  // Check for newer scoped interface in /proc/thread-self/attr/<lsm>
  अगर (lsm != शून्य)
    {
      lsm_dirfd = openat (attr_dirfd, lsm, O_DIRECTORY | O_PATH | O_CLOEXEC);

      अगर (UNLIKELY (lsm_dirfd < 0 && त्रुटि_सं != ENOENT))
        {
          crun_make_error (err, त्रुटि_सं, "open `/proc/thread-self/attr/%s`", lsm);
          वापस शून्य;
        }
    }
  // Use scoped interface if available, fall back to unscoped
  xasprintf (&attr_path, "thread-self/attr/%s%s%s", lsm_dirfd >= 0 ? lsm : "", lsm_dirfd >= 0 ? "/" : "", fname);

  वापस attr_path;
}

अटल पूर्णांक
check_proc_super_magic (पूर्णांक fd, स्थिर अक्षर *path, libcrun_error_t *err)
{
  काष्ठा statfs sfs;

  पूर्णांक ret = fstatfs (fd, &sfs);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "statfs `%s`", path);

  अगर (sfs.f_type != PROC_SUPER_MAGIC)
    वापस crun_make_error (err, 0, "the file `%s` is not on a `procfs` file system", path);

  वापस 0;
}

अटल पूर्णांक
set_security_attr (libcrun_container_t *container, स्थिर अक्षर *lsm, स्थिर अक्षर *fname, स्थिर अक्षर *तथ्य, libcrun_error_t *err)
{
  पूर्णांक ret;

  cleanup_free स्थिर अक्षर *attr_path = lsm_attr_path (container, lsm, fname, err);
  cleanup_close पूर्णांक fd = -1;

  अगर (UNLIKELY (attr_path == शून्य))
    वापस -1;

  fd = libcrun_open_proc_file (container, attr_path, O_WRONLY | O_CLOEXEC, err);
  अगर (UNLIKELY (fd < 0))
    वापस fd;

  ret = check_proc_super_magic (fd, attr_path, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  // Write out data
  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (fd, तथ्य, म_माप (तथ्य)));
  अगर (UNLIKELY (ret < 0))
    {
      /* ENOENT from the write is AppArmor telling the profile is unknown.
         The data is "<op> <profile>".  */
      अगर (त्रुटि_सं == ENOENT && lsm != शून्य && म_भेद (lsm, "apparmor") == 0)
        {
          स्थिर अक्षर *profile = म_अक्षर (तथ्य, ' ');

          वापस crun_make_error (err, 0, "apply apparmor profile `%s`: profile not loaded", profile ? profile + 1 : तथ्य);
        }
      वापस crun_make_error (err, त्रुटि_सं, "write to file `%s`", attr_path);
    }

  वापस 0;
}

पूर्णांक
set_selinux_label (libcrun_container_t *container, स्थिर अक्षर *label, bool now, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_is_selinux_enabled (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (ret)
    वापस set_security_attr (container, शून्य, now ? "current" : "exec", label, err);
  वापस 0;
}

अटल पूर्णांक
libcrun_is_apparmor_enabled (libcrun_error_t *err)
{
  अगर (apparmor_enabled < 0)
    वापस crun_make_error (err, 0, "AppArmor is not initialized correctly");
  वापस apparmor_enabled;
}

अटल पूर्णांक
is_current_process_confined (libcrun_container_t *container, libcrun_error_t *err)
{
#घोषणा UNCONFINED "unconfined"
#घोषणा UNCONFINED_LEN (ssize_t) (माप (UNCONFINED) - 1)
  cleanup_free स्थिर अक्षर *attr_path = lsm_attr_path (container, "apparmor", "current", err);
  cleanup_close पूर्णांक fd = -1;
  /* Only the "unconfined" token plus one delimiter byte are inspected.  */
  अक्षर buf[UNCONFINED_LEN + 1];

  अगर (UNLIKELY (attr_path == शून्य))
    वापस -1;

  fd = libcrun_open_proc_file (container, attr_path, O_RDONLY | O_CLOEXEC, err);
  अगर (UNLIKELY (fd < 0))
    वापस fd;

  अगर (UNLIKELY (check_proc_super_magic (fd, attr_path, err)))
    वापस -1;

  ssize_t bytes_read = अस्थायी_FAILURE_RETRY (पढ़ो (fd, buf, माप (buf)));
  अगर (UNLIKELY (bytes_read < 0))
    वापस crun_make_error (err, त्रुटि_सं, "read from `%s`", attr_path);

  /* The process is unconfined only when the attribute is exactly the token
     "unconfined", optionally followed by a delimiter.  Anything shorter (a
     short read), or a longer profile name such as "unconfined_foo", means the
     process is confined.  When in doubt default to confined, which is the safe
     assumption for the caller.  */
  अगर (bytes_read < UNCONFINED_LEN || स_भेद (buf, UNCONFINED, UNCONFINED_LEN) != 0)
    वापस 1;
  अगर (bytes_read == UNCONFINED_LEN || buf[UNCONFINED_LEN] == '\n' || buf[UNCONFINED_LEN] == ' ')
    वापस 0;
  वापस 1;
#अघोषित UNCONFINED
#अघोषित UNCONFINED_LEN
}

पूर्णांक
set_apparmor_profile (libcrun_container_t *container, स्थिर अक्षर *profile, bool no_new_privileges, bool now, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_is_apparmor_enabled (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (ret)
    {
      cleanup_free अक्षर *buf = शून्य;
      ret = is_current_process_confined (container, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      // if confined only way for apparmor to allow change of profile with NNP is with stacking
      xasprintf (&buf, "%s %s", no_new_privileges && ret ? "stack" : now ? "changeprofile"
                                                                         : "exec",
                 profile);

      वापस set_security_attr (container, "apparmor", now ? "current" : "exec", buf, err);
    }
  वापस 0;
}

अटल पूर्णांक
read_all_fd_internal (पूर्णांक fd, स्थिर अक्षर *description, अक्षर **out, माप_प्रकार *len, माप_प्रकार आकार_hint, माप_प्रकार max_आकार, libcrun_error_t *err)
{
  cleanup_free अक्षर *buf = शून्य;
  माप_प्रकार nread, allocated;
  माप_प्रकार pagesize = 0;
  off_t आकार = 0;
  पूर्णांक ret;

  अगर (आकार_hint)
    allocated = आकार_hint;
  अन्यथा
    {
      ret = get_file_आकार (fd, &आकार);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "stat `%s`", description);

      अगर (max_आकार && (माप_प्रकार) आकार > max_आकार)
        वापस crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_आकार);

      allocated = आकार == 0 ? 1023 : आकार;
    }

  /* NUL terminate the buffer.  */
  buf = xmalloc (allocated + 1);
  nread = 0;
  जबतक ((आकार && nread < (माप_प्रकार) आकार) || आकार == 0)
    {
      ret = अस्थायी_FAILURE_RETRY (पढ़ो (fd, buf + nread, allocated - nread));
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "read from file `%s`", description);

      अगर (ret == 0)
        अवरोध;

      nread += ret;

      अगर (max_आकार && nread > max_आकार)
        वापस crun_make_error (err, EFBIG, "`%s` is bigger than %zu bytes", description, max_आकार);

      अगर (nread == allocated)
        {
          अगर (आकार)
            अवरोध;

          अगर (pagesize == 0)
            pagesize = get_page_आकार ();

          allocated += pagesize;

          buf = xrealloc (buf, allocated + 1);
        }
    }
  अगर (nread + 1 < allocated)
    {
      /* shrink the buffer to the used size if it was allocated a bigger block.  */
      अक्षर *tmp = पुनः_स्मृति (buf, nread + 1);
      अगर (tmp)
        buf = tmp;
    }

  buf[nread] = '\0';
  *out = buf;
  buf = शून्य;
  अगर (len)
    *len = nread;
  वापस 0;
}

पूर्णांक
read_all_fd_with_आकार_hint (पूर्णांक fd, स्थिर अक्षर *description, अक्षर **out, माप_प्रकार *len, माप_प्रकार आकार_hint, libcrun_error_t *err)
{
  वापस read_all_fd_internal (fd, description, out, len, आकार_hint, 0, err);
}

पूर्णांक
read_all_fd_with_max_आकार (पूर्णांक fd, स्थिर अक्षर *description, अक्षर **out, माप_प्रकार *len, माप_प्रकार max_आकार, libcrun_error_t *err)
{
  वापस read_all_fd_internal (fd, description, out, len, 0, max_आकार, err);
}

पूर्णांक
read_all_file_at (पूर्णांक dirfd, स्थिर अक्षर *path, अक्षर **out, माप_प्रकार *len, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;

  fd = अस्थायी_FAILURE_RETRY (openat (dirfd, path, O_RDONLY | O_CLOEXEC));
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", path);

  वापस read_all_fd (fd, path, out, len, err);
}

पूर्णांक
read_all_file (स्थिर अक्षर *path, अक्षर **out, माप_प्रकार *len, libcrun_error_t *err)
{
  अगर (म_भेद (path, "-") == 0)
    path = "/dev/stdin";

  वापस read_all_file_at (AT_FDCWD, path, out, len, err);
}

पूर्णांक
get_realpath_to_file (पूर्णांक dirfd, स्थिर अक्षर *path_name, अक्षर **absolute_path, libcrun_error_t *err)
{
  cleanup_close पूर्णांक targetfd = -1;

  targetfd = अस्थायी_FAILURE_RETRY (openat (dirfd, path_name, O_RDONLY | O_CLOEXEC));
  अगर (UNLIKELY (targetfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", path_name);
  अन्यथा
    {
      ssize_t len;
      proc_fd_path_t target_fd_path;

      get_proc_self_fd_path (target_fd_path, targetfd);
      len = safe_readlinkat (AT_FDCWD, target_fd_path, absolute_path, 0, err);
      अगर (UNLIKELY (len < 0))
        वापस crun_error_wrap (err, "error unable to provide absolute path to file `%s`", path_name);
    }

  वापस 0;
}

पूर्णांक
open_unix_domain_client_socket (स्थिर अक्षर *path, पूर्णांक dgram, libcrun_error_t *err)
{
  काष्ठा sockaddr_un addr = {};
  पूर्णांक ret;
  proc_fd_path_t name_buf;
  cleanup_close पूर्णांक destfd = -1;
  cleanup_close पूर्णांक fd = -1;

  libcrun_debug ("Opening UNIX domain socket: %s", path);

  fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "create UNIX socket");

  अगर (म_माप (path) >= माप (addr.sun_path))
    {
      destfd = खोलो (path, O_PATH | O_CLOEXEC);
      अगर (UNLIKELY (destfd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", path);

      get_proc_self_fd_path (name_buf, destfd);

      path = name_buf;
    }

  माप_प्रकार path_len = म_माप (path);
  अगर (path_len >= माप (addr.sun_path))
    वापस crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);

  स_नकल (addr.sun_path, path, path_len + 1);
  addr.sun_family = AF_UNIX;
  ret = connect (fd, (काष्ठा sockaddr *) &addr, माप (addr));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "connect socket to `%s`", path);

  ret = fd;
  fd = -1;

  वापस ret;
}

पूर्णांक
open_unix_domain_socket (स्थिर अक्षर *path, पूर्णांक dgram, libcrun_error_t *err)
{
  काष्ठा sockaddr_un addr = {};
  proc_fd_path_t name_buf;
  पूर्णांक ret;
  cleanup_close पूर्णांक dirfd = -1;
  cleanup_close पूर्णांक fd = socket (AF_UNIX, dgram ? SOCK_DGRAM : SOCK_STREAM, 0);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "create UNIX socket");

  अगर (म_माप (path) >= माप (addr.sun_path))
    {
      cleanup_free अक्षर *dpath = xstrdup (path);
      cleanup_free अक्षर *bpath = xstrdup (path);
      स्थिर अक्षर *parent_dir = स_नाम (dpath);
      स्थिर अक्षर *base = basename (bpath);
      पूर्णांक n;

      dirfd = खोलो (parent_dir, O_PATH | O_DIRECTORY | O_CLOEXEC);
      अगर (UNLIKELY (dirfd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open directory `%s`", parent_dir);

      get_proc_self_fd_path (name_buf, dirfd);

      n = snprintf (addr.sun_path, माप (addr.sun_path), "%s/%s", name_buf, base);
      अगर (n < 0 || (माप_प्रकार) n >= माप (addr.sun_path))
        वापस crun_make_error (err, ENAMETOOLONG, "socket path too long: `%s`", path);
    }
  अन्यथा
    {
      माप_प्रकार path_len = म_माप (path);
      स_नकल (addr.sun_path, path, path_len + 1);
    }

  addr.sun_family = AF_UNIX;
  ret = bind (fd, (काष्ठा sockaddr *) &addr, माप (addr));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "bind socket to `%s`", path);

  अगर (! dgram)
    {
      ret = listen (fd, 1);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "listen on socket");
    }

  ret = fd;
  fd = -1;

  वापस ret;
}

पूर्णांक
send_fd_to_socket (पूर्णांक server, पूर्णांक fd, libcrun_error_t *err)
{
  वापस send_fd_to_socket_with_payload (server, fd, शून्य, 0, err);
}

पूर्णांक
send_fd_to_socket_with_payload (पूर्णांक server, पूर्णांक fd, स्थिर अक्षर *payload, माप_प्रकार payload_len, libcrun_error_t *err)
{
  पूर्णांक ret;
  काष्ठा cmsghdr *cmsg = शून्य;
  काष्ठा iovec iov[2];
  काष्ठा msghdr संदेश = {};
  अक्षर ctrl_buf[CMSG_SPACE (1 + माप (पूर्णांक))] = {};
  अक्षर तथ्य[1];

  तथ्य[0] = ' ';
  iov[0].iov_base = तथ्य;
  iov[0].iov_len = माप (तथ्य);

  अगर (payload_len > 0)
    {
      iov[0].iov_base = (व्योम *) payload;
      iov[0].iov_len = payload_len;
    }

  संदेश.संदेश_name = शून्य;
  संदेश.संदेश_namelen = 0;
  संदेश.संदेश_iov = iov;
  संदेश.संदेश_iovlen = 1;
  संदेश.संदेश_controllen = CMSG_SPACE (माप (पूर्णांक));
  संदेश.संदेश_control = ctrl_buf;

  cmsg = CMSG_FIRSTHDR (&संदेश);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (माप (पूर्णांक));

  *((पूर्णांक *) CMSG_तथ्य (cmsg)) = fd;

  ret = अस्थायी_FAILURE_RETRY (sendmsg (server, &संदेश, 0));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "sendmsg");
  वापस 0;
}

पूर्णांक
receive_fd_from_socket_with_payload (पूर्णांक from, अक्षर *payload, माप_प्रकार payload_len, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret;
  काष्ठा iovec iov[1];
  काष्ठा msghdr संदेश = {};
  अक्षर ctrl_buf[CMSG_SPACE (माप (पूर्णांक))] = {};
  अक्षर तथ्य[1];
  काष्ठा cmsghdr *cmsg;

  तथ्य[0] = ' ';
  iov[0].iov_base = तथ्य;
  iov[0].iov_len = माप (तथ्य);

  अगर (payload_len > 0)
    {
      iov[0].iov_base = (व्योम *) payload;
      iov[0].iov_len = payload_len;
    }

  संदेश.संदेश_name = शून्य;
  संदेश.संदेश_namelen = 0;
  संदेश.संदेश_iov = iov;
  संदेश.संदेश_iovlen = 1;
  संदेश.संदेश_controllen = CMSG_SPACE (माप (पूर्णांक));
  संदेश.संदेश_control = ctrl_buf;

  ret = अस्थायी_FAILURE_RETRY (recvmsg (from, &संदेश, 0));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "recvmsg");
  अगर (UNLIKELY (ret == 0))
    वापस crun_make_error (err, 0, "read FD: connection closed");

  cmsg = CMSG_FIRSTHDR (&संदेश);
  अगर (cmsg == शून्य)
    वापस crun_make_error (err, 0, "no msg received");
  स_नकल (&fd, CMSG_तथ्य (cmsg), माप (fd));

  ret = fd;
  fd = -1;
  वापस ret;
}

पूर्णांक
receive_fd_from_socket (पूर्णांक from, libcrun_error_t *err)
{
  वापस receive_fd_from_socket_with_payload (from, शून्य, 0, err);
}

पूर्णांक
create_socket_pair (पूर्णांक *pair, libcrun_error_t *err)
{
  पूर्णांक ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, pair);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "socketpair");
  वापस 0;
}

पूर्णांक
create_signalfd (sigset_t *mask, libcrun_error_t *err)
{
  पूर्णांक ret = signalfd (-1, mask, 0);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "signalfd");
  वापस ret;
}

अटल पूर्णांक
epoll_helper_toggle (पूर्णांक epollfd, पूर्णांक fd, पूर्णांक events, libcrun_error_t *err)
{
  काष्ठा epoll_event ev = {};
  bool add = events != 0;
  पूर्णांक ret;

  ev.events = events;
  ev.तथ्य.fd = fd;
  ret = epoll_ctl (epollfd, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  /* The fd is already registered, possibly with a different mask.  */
  अगर (UNLIKELY (ret < 0 && त्रुटि_सं == EEXIST))
    ret = epoll_ctl (epollfd, EPOLL_CTL_MOD, fd, &ev);
  अगर (UNLIKELY (ret < 0))
    {
      /* ENOENT: not registered.  EPERM: the fd is not supported by epoll,
         such as a regular file or /dev/null, and is always ready.  */
      अगर (त्रुटि_सं == ENOENT || त्रुटि_सं == EPERM)
        वापस 0;
      वापस crun_make_error (err, त्रुटि_सं, "epoll_ctl `%s` `%d`", add ? "add" : "del", fd);
    }
  वापस 0;
}

पूर्णांक
epoll_helper (पूर्णांक *in_fds, पूर्णांक *in_levelfds, पूर्णांक *out_fds, पूर्णांक *out_levelfds, libcrun_error_t *err)
{
  काष्ठा epoll_event ev;
  cleanup_close पूर्णांक epollfd = -1;
  पूर्णांक ret;

  पूर्णांक *it;
  epollfd = epoll_create1 (0);
  अगर (UNLIKELY (epollfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "epoll_create1");

#घोषणा ADD_FDS(FDS, EVENTS)                                            \
  क्रम (it = FDS; *it >= 0; it++)                                        \
    {                                                                   \
      ev.events = EVENTS;                                               \
      ev.तथ्य.fd = *it;                                                 \
      ret = epoll_ctl (epollfd, EPOLL_CTL_ADD, *it, &ev);               \
      अगर (UNLIKELY (ret < 0))                                           \
        वापस crun_make_error (err, त्रुटि_सं, "epoll_ctl add `%d`", *it); \
    }

  अगर (in_fds)
    ADD_FDS (in_fds, EPOLLIN);
  अगर (in_levelfds)
    ADD_FDS (in_levelfds, EPOLLIN | EPOLLET);
  अगर (out_fds)
    ADD_FDS (out_fds, EPOLLOUT);
  अगर (out_levelfds)
    ADD_FDS (out_levelfds, EPOLLOUT | EPOLLET);

  ret = epollfd;
  epollfd = -1;
  वापस ret;
}

पूर्णांक
copy_from_fd_to_fd (पूर्णांक src, पूर्णांक dst, पूर्णांक consume, libcrun_error_t *err)
{
  पूर्णांक ret;
  ssize_t nread;
  माप_प्रकार pagesize = get_page_आकार ();
#अगर_घोषित HAVE_COPY_FILE_RANGE
  bool can_copy_file_range = true;
#पूर्ण_अगर
  करो
    {
      cleanup_free अक्षर *बफर = शून्य;
      ssize_t remaining;

#अगर_घोषित HAVE_COPY_FILE_RANGE
      अगर (can_copy_file_range)
        {
          nread = copy_file_range (src, शून्य, dst, शून्य, pagesize, 0);
          अगर (nread < 0 && (त्रुटि_सं == EINVAL || त्रुटि_सं == EXDEV))
            {
              can_copy_file_range = false;
              जाओ fallback;
            }
          अगर (consume && nread < 0 && त्रुटि_सं == EAGAIN)
            वापस 0;
          अगर (nread < 0 && त्रुटि_सं == EIO)
            वापस 0;
          अगर (UNLIKELY (nread < 0))
            वापस crun_make_error (err, त्रुटि_सं, "copy_file_range");
          जारी;
        }
    fallback:
#पूर्ण_अगर

      बफर = xmalloc (pagesize);
      nread = अस्थायी_FAILURE_RETRY (पढ़ो (src, बफर, pagesize));
      अगर (consume && nread < 0 && त्रुटि_सं == EAGAIN)
        वापस 0;
      अगर (nread < 0 && त्रुटि_सं == EIO)
        वापस 0;
      अगर (UNLIKELY (nread < 0))
        वापस crun_make_error (err, त्रुटि_सं, "read");

      remaining = nread;
      जबतक (remaining)
        {
          ret = अस्थायी_FAILURE_RETRY (ग_लिखो (dst, बफर + nread - remaining, remaining));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "write");
          remaining -= ret;
        }
  } जबतक (consume && nread);

  वापस 0;
}

पूर्णांक
run_process (अक्षर **args, libcrun_error_t *err)
{
  pid_t pid = विभाजन ();
  अगर (UNLIKELY (pid < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fork");
  अगर (pid)
    {
      पूर्णांक r, status;
      r = अस्थायी_FAILURE_RETRY (waitpid (pid, &status, 0));
      अगर (r < 0)
        वापस crun_make_error (err, त्रुटि_सं, "waitpid");
      अगर (WIFEXITED (status) || WIFSIGNALED (status))
        वापस WEXITSTATUS (status);
    }

  execvp (args[0], args);
  _safe_exit (निकास_त्रुटि);
}

#अगर_अघोषित HAVE_FGETPWENT_R
अटल अचिन्हित
atou (अक्षर **s)
{
  अचिन्हित x;
  क्रम (x = 0; **s >= '0' && **s <= '9'; ++*s)
    x = 10 * x + (**s - '0');
  वापस x;
}

पूर्णांक
fgetpwent_r (खाता *f, काष्ठा passwd *pw, अक्षर *पंक्ति, माप_प्रकार आकार, काष्ठा passwd **res)
{
  अक्षर *s;
  पूर्णांक rv = 0;
  क्रम (;;)
    {
      पंक्ति[आकार - 1] = '\xff';
      अगर ((ख_माला_लो (पंक्ति, आकार, f) == शून्य) || ख_त्रुटि (f) || पंक्ति[आकार - 1] != '\xff')
        {
          rv = (पंक्ति[आकार - 1] != '\xff') ? दुस्फल : ENOENT;
          पंक्ति = 0;
          pw = 0;
          अवरोध;
        }
      पंक्ति[म_खोज (पंक्ति, "\n")] = 0;

      s = पंक्ति;
      pw->pw_name = s++;
      अगर (! (s = म_अक्षर (s, ':')))
        जारी;

      *s++ = 0;
      pw->pw_passwd = s;
      अगर (! (s = म_अक्षर (s, ':')))
        जारी;

      *s++ = 0;
      pw->pw_uid = atou (&s);
      अगर (*s != ':')
        जारी;

      *s++ = 0;
      pw->pw_gid = atou (&s);
      अगर (*s != ':')
        जारी;

      *s++ = 0;
      pw->pw_gecos = s;
      अगर (! (s = म_अक्षर (s, ':')))
        जारी;

      *s++ = 0;
      pw->pw_dir = s;
      अगर (! (s = म_अक्षर (s, ':')))
        जारी;

      *s++ = 0;
      pw->pw_shell = s;
      अवरोध;
    }
  *res = pw;
  अगर (rv)
    त्रुटि_सं = rv;
  वापस rv;
}
#पूर्ण_अगर

पूर्णांक
set_home_env (uid_t id)
{
  काष्ठा passwd pwd;
  cleanup_free अक्षर *buf = शून्य;
  दीर्घ buf_आकार;
  cleanup_file खाता *stream = शून्य;

  buf_आकार = sysconf (_SC_GETPW_R_SIZE_MAX);
  अगर (buf_आकार < 0)
    buf_आकार = 1024;

  buf = xmalloc (buf_आकार);

  stream = ख_खोलो ("/etc/passwd", "re");
  अगर (stream == शून्य)
    वापस -1;

  क्रम (;;)
    {
      काष्ठा passwd *ret_pw = शून्य;

      पूर्णांक ret = fgetpwent_r (stream, &pwd, buf, buf_आकार, &ret_pw);
      अगर (UNLIKELY (ret != 0))
        {
          अगर (ret == EINTR)
            जारी;

          अगर (ret != दुस्फल)
            {
              /* Let callers handle the error if the user was not found. */
              त्रुटि_सं = ret;
              वापस -1;
            }

          buf_आकार *= 2;
          buf = xrealloc (buf, buf_आकार);
          जारी;
        }

      अगर (ret_pw && ret_pw->pw_uid == id)
        {
          अगर (UNLIKELY (setenv ("HOME", ret_pw->pw_dir, 1) < 0))
            OOM ();
          अन्यथा
            वापस 0;
        }
    }
}

/*if subuid or subgid exist, take the first range for the user */
अटल पूर्णांक
getsubidrange (uid_t id, पूर्णांक is_uid, uint32_t *from, uint32_t *len)
{
  cleanup_file खाता *निवेश = शून्य;
  cleanup_free अक्षर *lineptr = शून्य;
  माप_प्रकार lenlineptr = 0, len_name;
  दीर्घ buf_आकार;
  cleanup_free अक्षर *buf = शून्य;
  स्थिर अक्षर *name;
  काष्ठा passwd pwd;

  buf_आकार = sysconf (_SC_GETPW_R_SIZE_MAX);
  अगर (buf_आकार < 0)
    buf_आकार = 1024;

  buf = xmalloc (buf_आकार);
  क्रम (;;)
    {
      पूर्णांक ret;
      काष्ठा passwd *ret_pw = शून्य;

      ret = getpwuid_r (id, &pwd, buf, buf_आकार, &ret_pw);
      अगर (LIKELY (ret == 0))
        {
          अगर (ret_pw)
            {
              name = ret_pw->pw_name;
              अवरोध;
            }
          वापस -1;
        }

      अगर (ret != दुस्फल)
        वापस -1;

      buf_आकार *= 2;
      buf = xrealloc (buf, buf_आकार);
    }

  len_name = म_माप (name);

  निवेश = ख_खोलो (is_uid ? "/etc/subuid" : "/etc/subgid", "re");
  अगर (निवेश == शून्य)
    वापस -1;

  क्रम (;;)
    {
      अक्षर *endptr;
      ssize_t पढ़ो = getline (&lineptr, &lenlineptr, निवेश);
      अगर (पढ़ो < 0)
        वापस -1;

      अगर (पढ़ो < (ssize_t) (len_name + 2))
        जारी;

      अगर (स_भेद (lineptr, name, len_name) || lineptr[len_name] != ':')
        जारी;

      *from = strtoull (&lineptr[len_name + 1], &endptr, 10);

      अगर (endptr >= &lineptr[पढ़ो])
        वापस -1;

      *len = strtoull (&endptr[1], &endptr, 10);

      वापस 0;
    }
}

#घोषणा MIN(x, y) ((x) < (y) ? (x) : (y))

पूर्णांक
format_default_id_mapping (अक्षर **out, uid_t container_id, uid_t host_uid, uid_t host_id, पूर्णांक is_uid, libcrun_error_t *err)
{
  uint32_t from = 0, available = 0;
  cleanup_free अक्षर *बफर = शून्य;
  पूर्णांक written = 0;
  पूर्णांक ret, remaining;

  *out = शून्य;

  अगर (getsubidrange (host_uid, is_uid, &from, &available) < 0)
    वापस 0;

  /* More than enough space for all the mappings.  */
  remaining = 15 * 5 * 3;
  बफर = xmalloc (remaining + 1);

  अगर (container_id > 0)
    {
      uint32_t used = MIN (container_id, available);
      ret = snprintf (बफर + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", 0, from, used);
      अगर (UNLIKELY (ret >= remaining))
        वापस crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
      remaining -= ret;

      from += used;
      available -= used;
    }

  /* Host ID -> Container ID.  */
  ret = snprintf (बफर + written, remaining, "%d %d 1\n", container_id, host_id);
  अगर (UNLIKELY (ret >= remaining))
    वापस crun_make_error (err, 0, "internal error: allocated buffer too small");
  written += ret;
  remaining -= ret;

  /* Last mapping: use any id that is left.  */
  अगर (available)
    {
      ret = snprintf (बफर + written, remaining, "%d %" PRIu32 " %" PRIu32 "\n", container_id + 1, from, available);
      अगर (UNLIKELY (ret >= remaining))
        वापस crun_make_error (err, 0, "internal error: allocated buffer too small");
      written += ret;
      remaining -= ret;
    }

  *out = बफर;
  बफर = शून्य;
  वापस written;
}

अटल पूर्णांक
unset_cloexec_flag (पूर्णांक fd)
{
  पूर्णांक flags = fcntl (fd, F_GETFD);
  अगर (flags == -1)
    वापस -1;

  flags &= ~FD_CLOEXEC;

  वापस fcntl (fd, F_SETFD, flags);
}

अटल व्योम __attribute__ ((__noreturn__))
run_process_child (अक्षर *path, अक्षर **args, स्थिर अक्षर *cwd, अक्षर **envp, पूर्णांक pipe_r,
                   पूर्णांक pipe_w, पूर्णांक out_fd, पूर्णांक err_fd, bool can_ignore_chdir_errors)
{
  अक्षर *tmp_args[] = { path, शून्य };
  libcrun_error_t err = शून्य;
  पूर्णांक dev_null_fd = -1;
  पूर्णांक ret;

  ret = mark_or_close_fds_ge_than (शून्य, 3, false, &err);
  अगर (UNLIKELY (ret < 0))
    libcrun_fail_with_error ((err)->status, "%s", (err)->संदेश);

  अगर (out_fd < 0 || err_fd < 0)
    {
      dev_null_fd = खोलो ("/dev/null", O_WRONLY | O_CLOEXEC);
      अगर (UNLIKELY (dev_null_fd < 0))
        _safe_exit (निकास_त्रुटि);
    }

  अस्थायी_FAILURE_RETRY (बंद (pipe_w));
  dup2 (pipe_r, 0);
  अस्थायी_FAILURE_RETRY (बंद (pipe_r));

  dup2 (out_fd >= 0 ? out_fd : dev_null_fd, 1);
  dup2 (err_fd >= 0 ? err_fd : dev_null_fd, 2);

  अगर (out_fd >= 0)
    unset_cloexec_flag (1);
  अगर (err_fd >= 0)
    unset_cloexec_flag (2);

  अगर (dev_null_fd >= 0)
    अस्थायी_FAILURE_RETRY (बंद (dev_null_fd));
  अगर (out_fd >= 0)
    अस्थायी_FAILURE_RETRY (बंद (out_fd));
  अगर (err_fd >= 0)
    अस्थायी_FAILURE_RETRY (बंद (err_fd));

  अगर (args == शून्य)
    args = tmp_args;

  अगर (cwd && स_बदलो (cwd) < 0)
    {
      अगर (! can_ignore_chdir_errors || (त्रुटि_सं != EACCES && त्रुटि_सं != EPERM))
        _safe_exit (निकास_त्रुटि);
    }

  execvpe (path, args, envp);
  _safe_exit (निकास_त्रुटि);
}

/* It changes the signals mask for the current process.  */
पूर्णांक
run_process_with_stdin_timeout_envp (अक्षर *path, अक्षर **args, स्थिर अक्षर *cwd, पूर्णांक timeout,
                                     अक्षर **envp, अक्षर *मानक_निवेश, माप_प्रकार stdin_len, पूर्णांक out_fd,
                                     पूर्णांक err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  पूर्णांक stdin_pipe[2];
  pid_t pid;
  पूर्णांक ret;
  cleanup_close पूर्णांक pipe_r = -1;
  cleanup_close पूर्णांक pipe_w = -1;
  sigset_t oldmask, mask;
  पूर्णांक r, status;

  sigemptyset (&mask);

  ret = pipe2 (stdin_pipe, O_CLOEXEC);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "pipe");
  pipe_r = stdin_pipe[0];
  pipe_w = stdin_pipe[1];

  अगर (timeout > 0)
    {
      sigaddset (&mask, SIGCHLD);
      ret = sigprocmask (SIG_BLOCK, &mask, &oldmask);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "sigprocmask");
    }

  pid = विभाजन ();
  अगर (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, त्रुटि_सं, "fork");
      जाओ restore_sig_mask_and_exit;
    }

  अगर (pid == 0)
    {
      /* run_process_child doesn't return.  */
      run_process_child (path, args, cwd, envp, pipe_r, pipe_w, out_fd, err_fd, can_ignore_chdir_errors);
    }

  close_and_reset (&pipe_r);

  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (pipe_w, मानक_निवेश, stdin_len));
  अगर (UNLIKELY (ret < 0))
    {
      /* Ignore EPIPE as the container process could have already
         been terminated.  */
      अगर (त्रुटि_सं != EPIPE)
        {
          ret = crun_make_error (err, त्रुटि_सं, "write to pipe");
          जाओ restore_sig_mask_and_exit;
        }
    }

  close_and_reset (&pipe_w);

  अगर (timeout)
    {
      समय_प्रकार start = समय (शून्य);
      समय_प्रकार now;
      क्रम (now = start; now - start < timeout; now = समय (शून्य))
        {
          siginfo_t info;
          पूर्णांक elapsed = now - start;
          काष्ठा timespec ts_timeout = { .tv_sec = timeout - elapsed, .tv_nsec = 0 };

          ret = sigtimedwait (&mask, &info, &ts_timeout);
          अगर (UNLIKELY (ret < 0 && त्रुटि_सं != EAGAIN))
            {
              ret = crun_make_error (err, त्रुटि_सं, "sigtimedwait");
              जाओ restore_sig_mask_and_exit;
            }

          अगर (info.si_signo == SIGCHLD && info.si_pid == pid)
            जाओ read_waitpid;

          अगर (ret < 0 && त्रुटि_सं == EAGAIN)
            जाओ timeout;
        }
    timeout:
      समाप्त (pid, SIGKILL);

      ret = crun_make_error (err, 0, "timeout expired for `%s`", path);
      जाओ restore_sig_mask_and_exit;
    }

read_waitpid:
  r = waitpid_ignore_stopped (pid, &status, 0);
  अगर (r < 0)
    ret = crun_make_error (err, त्रुटि_सं, "waitpid");
  अन्यथा
    ret = get_process_exit_status (status);

  /* Prevent to cleanup the pid again.  */
  pid = 0;

restore_sig_mask_and_exit:
  अगर (timeout > 0)
    {
      /* Cleanup the zombie process.  */
      अगर (pid > 0)
        {
          समाप्त (pid, SIGKILL);
          अस्थायी_FAILURE_RETRY (waitpid (pid, &status, 0));
        }
      r = sigprocmask (SIG_UNBLOCK, &oldmask, शून्य);
      अगर (UNLIKELY (r < 0 && ret >= 0))
        ret = crun_make_error (err, त्रुटि_सं, "restoring signal mask with sigprocmask");
    }
  वापस ret;
}

पूर्णांक
mark_or_close_fds_ge_than (libcrun_container_t *container, पूर्णांक n, bool close_now, libcrun_error_t *err)
{
  cleanup_close पूर्णांक cfd = -1;
  cleanup_dir सूची *dir = शून्य;
  पूर्णांक ret;
  पूर्णांक fd;
  काष्ठा dirent *अग्र;

#अगर_घोषित ENABLE_GCOV
  /* Flush gcov data before closing all fds.  Any code coverage after this point will be lost.  */
  अगर (close_now)
    GCOV_DUMP ();
#पूर्ण_अगर

  ret = syscall_close_range (n, अच_पूर्णांक_उच्च, close_now ? 0 : CLOSE_RANGE_CLOEXEC);
  अगर (ret == 0)
    {
      अगर (close_now && container && container->proc_fd >= n)
        container->proc_fd = -1;
      वापस 0;
    }
  अगर (ret < 0 && त्रुटि_सं != EINVAL && त्रुटि_सं != ENOSYS && त्रुटि_सं != EPERM)
    वापस crun_make_error (err, त्रुटि_सं, "close_range from `%d`", n);

  cfd = libcrun_open_proc_file (container, "self/fd", O_DIRECTORY | O_RDONLY, err);
  अगर (UNLIKELY (cfd < 0))
    वापस cfd;

  ret = check_proc_super_magic (cfd, "self/fd", err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  dir = fdopendir (cfd);
  अगर (UNLIKELY (dir == शून्य))
    वापस crun_make_error (err, त्रुटि_सं, "fdopendir `self/fd`");

  /* Now it is owned by dir.  */
  cfd = -1;

  fd = dirfd (dir);
  क्रम (अग्र = सूची_पढ़ो (dir); अग्र; अग्र = सूची_पढ़ो (dir))
    {
      पूर्णांक val;
      स्थिर अक्षर *name = अग्र->d_name;
      अगर (name[0] == '.')
        जारी;

      val = strtoll (name, शून्य, 10);
      अगर (val < n || val == fd)
        जारी;

      अगर (close_now)
        {
          ret = बंद (val);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "close fd `%d`", val);

          अगर (container && val == container->proc_fd)
            container->proc_fd = -1;
        }
      अन्यथा
        {
          ret = fcntl (val, F_SETFD, FD_CLOEXEC);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "cannot set CLOEXEC for fd `%d`", val);
        }
    }
  वापस 0;
}

व्योम
get_current_timestamp (अक्षर *out, माप_प्रकार len)
{
  काष्ठा timeval tv;
  काष्ठा पंचांग now;
  अक्षर timestamp[64];

  gettimeofday (&tv, शून्य);
  gmtime_r (&tv.tv_sec, &now);
  स_माला (timestamp, माप (timestamp), "%Y-%m-%dT%H:%M:%S", &now);

  (व्योम) snprintf (out, len, "%s.%06lldZ", timestamp, (दीर्घ दीर्घ पूर्णांक) tv.tv_usec);
  out[len - 1] = '\0';
}

पूर्णांक
set_blocking_fd (पूर्णांक fd, bool blocking, libcrun_error_t *err)
{
  पूर्णांक ret, flags = fcntl (fd, F_GETFL, 0);
  अगर (UNLIKELY (flags < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fcntl");

  ret = fcntl (fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fcntl");
  वापस 0;
}

पूर्णांक
parse_json_file (json_object **out, स्थिर अक्षर *jsondata, काष्ठा parser_context *ctx arg_unused, libcrun_error_t *err)
{
  काष्ठा json_tokener *tok;
  क्रमागत json_tokener_error jerr;

  *err = शून्य;

  अगर (UNLIKELY (jsondata == शून्य))
    वापस crun_make_error (err, 0, "JSON data is NULL");

  tok = json_tokener_new ();
  अगर (UNLIKELY (tok == शून्य))
    वापस crun_make_error (err, 0, "cannot allocate JSON tokener");

  json_tokener_set_flags (tok, JSON_TOKENER_VALIDATE_UTF8);

  *out = json_tokener_parse_ex (tok, jsondata, (पूर्णांक) म_माप (jsondata));
  jerr = json_tokener_get_error (tok);
  अगर (*out == शून्य || jerr != json_tokener_success)
    {
      पूर्णांक saved_pos = (पूर्णांक) tok->char_offset;

      json_tokener_free (tok);
      अगर (*out)
        {
          json_object_put (*out);
          *out = शून्य;
        }
      वापस crun_make_error (err, 0, "cannot parse the data: %s at position %d",
                              json_tokener_error_desc (jerr), saved_pos);
    }

  json_tokener_free (tok);
  वापस 0;
}

#घोषणा CHECK_ACCESS_NOT_EXECUTABLE 1
#घोषणा CHECK_ACCESS_NOT_REGULAR 2

/* check that the specified path exists and it is executable.
   Return
   - 0 if the file is executable
   - CHECK_ACCESS_NOT_EXECUTABLE if it exists but it is not executable
   - CHECK_ACCESS_NOT_REGULAR if it not a regular file
   - -errno for any other generic error
 */
अटल पूर्णांक
check_access (स्थिर अक्षर *path)
{
  पूर्णांक ret;
  mode_t mode;

#अगर_घोषित HAVE_EACCESS
#  define ACCESS(path, mode) (eaccess (path, mode))
#अन्यथा
#  define ACCESS(path, mode) (access (path, mode))
#पूर्ण_अगर

  ret = ACCESS (path, X_OK);
  अगर (ret < 0)
    {
      /* If the file is not executable, check if it exists.  */
      अगर (त्रुटि_सं == EACCES)
        {
          पूर्णांक saved_errno = त्रुटि_सं;
          ret = ACCESS (path, F_OK);
          त्रुटि_सं = saved_errno;

          अगर (ret == 0)
            वापस CHECK_ACCESS_NOT_EXECUTABLE;
        }
      वापस -त्रुटि_सं;
    }

  ret = get_file_type (&mode, false, path);
  अगर (UNLIKELY (ret < 0))
    वापस -त्रुटि_सं;

  अगर (! S_ISREG (mode))
    वापस CHECK_ACCESS_NOT_REGULAR;

  /* It exists, is executable and is a regular file.  */
  वापस 0;
}

पूर्णांक
find_executable (अक्षर **out, स्थिर अक्षर *executable_path, स्थिर अक्षर *cwd, libcrun_error_t *err)
{
  cleanup_free अक्षर *cwd_executable_path = शून्य;
  cleanup_free अक्षर *tmp = शून्य;
  पूर्णांक last_error = -ENOENT;
  अक्षर *it, *end;
  पूर्णांक ret;

  *out = शून्य;

  अगर (executable_path == शून्य || executable_path[0] == '\0')
    वापस crun_make_error (err, ENOENT, "cannot find `` in $PATH");

  अगर (executable_path[0] != '/' && म_अक्षर (executable_path, '/'))
    {
      cleanup_free अक्षर *cwd_allocated = शून्य;

      अगर (cwd == शून्य)
        {
          cwd_allocated = getcwd (शून्य, 0);
          अगर (cwd_allocated == शून्य)
            OOM ();

          cwd = cwd_allocated;
        }

      /* Make sure the path starts with a '/' so it will hit the check
         for absolute paths.  */
      ret = append_paths (&cwd_executable_path, err, "/", cwd, executable_path, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      executable_path = cwd_executable_path;
    }

  /* Absolute path.  It doesn't need to lookup $PATH.  */
  अगर (executable_path[0] == '/')
    {
      ret = check_access (executable_path);
      अगर (LIKELY (ret == 0))
        {
          *out = xstrdup (executable_path);
          वापस 0;
        }
      last_error = ret;
      जाओ fail;
    }

  end = tmp = xstrdup (दो_पर्या ("PATH"));

  जबतक ((it = strsep (&end, ":")))
    {
      cleanup_free अक्षर *path = शून्य;

      अगर (it == end)
        it = ".";

      ret = append_paths (&path, err, it, executable_path, शून्य);
      अगर (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          जारी;
        }

      ret = check_access (path);
      अगर (ret == 0)
        {
          /* Change owner.  */
          *out = path;
          path = शून्य;
          वापस 0;
        }

      अगर (ret == -ENOENT)
        जारी;

      last_error = ret;
    }

fail:
  चयन (last_error)
    {
    हाल CHECK_ACCESS_NOT_EXECUTABLE:
      वापस crun_make_error (err, EPERM, "the path `%s` exists but it is not executable", executable_path);

    हाल CHECK_ACCESS_NOT_REGULAR:
      वापस crun_make_error (err, EPERM, "the path `%s` is not a regular file", executable_path);

    शेष:
      त्रुटि_सं = -last_error;
      अगर (त्रुटि_सं == ENOENT)
        वापस crun_make_error (err, त्रुटि_सं, "executable file `%s` not found%s", executable_path, executable_path[0] == '/' ? "" : " in $PATH");
      वापस crun_make_error (err, त्रुटि_सं, "open `%s`", executable_path);
    }
}

#अगर_घोषित HAVE_FGETXATTR

अटल ssize_t
safe_read_xattr (अक्षर **ret, पूर्णांक sfd, स्थिर अक्षर *srcname, स्थिर अक्षर *name, माप_प्रकार initial_आकार, libcrun_error_t *err)
{
  cleanup_free अक्षर *बफर = शून्य;
  ssize_t current_आकार;
  ssize_t s;

  current_आकार = (ssize_t) initial_आकार;
  बफर = xmalloc (current_आकार + 1);

  जबतक (1)
    {
      s = fgetxattr (sfd, name, बफर, current_आकार);
      अगर (UNLIKELY (s < 0))
        वापस crun_make_error (err, त्रुटि_सं, "get xattr `%s` from `%s`", name, srcname);

      अगर (s < current_आकार)
        अवरोध;

      current_आकार *= 2;
      बफर = xrealloc (बफर, current_आकार + 1);
    }

  अगर (s <= 0)
    वापस s;

  बफर[s] = '\0';

  /* Change owner.  */
  *ret = बफर;
  बफर = शून्य;

  वापस s;
}

अटल ssize_t
copy_xattr (पूर्णांक sfd, पूर्णांक dfd, स्थिर अक्षर *srcname, स्थिर अक्षर *destname, libcrun_error_t *err)
{
  cleanup_free अक्षर *buf = शून्य;
  ssize_t xattr_len;
  अक्षर *it;

  xattr_len = flistxattr (sfd, शून्य, 0);
  अगर (UNLIKELY (xattr_len < 0))
    {
      अगर (त्रुटि_सं == ENOTSUP)
        वापस 0;

      वापस crun_make_error (err, त्रुटि_सं, "flistxattr `%s`", srcname);
    }

  अगर (xattr_len == 0)
    वापस 0;

  buf = xmalloc (xattr_len + 1);

  xattr_len = flistxattr (sfd, buf, xattr_len + 1);
  अगर (UNLIKELY (xattr_len < 0))
    वापस crun_make_error (err, त्रुटि_सं, "flistxattr `%s`", srcname);

  क्रम (it = buf; it - buf < xattr_len; it += म_माप (it) + 1)
    {
      cleanup_free अक्षर *v = शून्य;
      ssize_t s;

      s = safe_read_xattr (&v, sfd, srcname, it, 256, err);
      अगर (UNLIKELY (s < 0))
        वापस s;

      s = fsetxattr (dfd, it, v, s, 0);
      अगर (UNLIKELY (s < 0))
        {
          अगर (त्रुटि_सं == EINVAL || त्रुटि_सं == EOPNOTSUPP)
            जारी;

          वापस crun_make_error (err, त्रुटि_सं, "fsetxattr `%s` to `%s`", it, destname);
        }
    }

  वापस 0;
}

#पूर्ण_अगर

अटल पूर्णांक
copy_rec_stat_file_at (पूर्णांक dfd, स्थिर अक्षर *path, mode_t *mode, off_t *आकार, dev_t *rdev, uid_t *uid, gid_t *gid)
{
  काष्ठा stat st;
  पूर्णांक ret;

#अगर_घोषित HAVE_STATX
  काष्ठा statx stx = {
    0,
  };

  ret = statx (dfd, path, AT_SYMLINK_NOFOLLOW | AT_STATX_DONT_SYNC,
               STATX_TYPE | STATX_MODE | STATX_आकार | STATX_UID | STATX_GID, &stx);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं == ENOSYS || त्रुटि_सं == EINVAL)
        जाओ fallback;

      वापस ret;
    }

  *mode = stx.stx_mode;
  *आकार = stx.stx_size;
  *rdev = makedev (stx.stx_rdev_major, stx.stx_rdev_minor);
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;

  वापस ret;

fallback:
#पूर्ण_अगर
  ret = fstatat (dfd, path, &st, AT_SYMLINK_NOFOLLOW);

  *mode = st.st_mode;
  *आकार = st.st_size;
  *rdev = st.st_rdev;
  *uid = st.st_uid;
  *gid = st.st_gid;

  वापस ret;
}

पूर्णांक
copy_recursive_fd_to_fd (पूर्णांक srcdirfd, पूर्णांक dfd, स्थिर अक्षर *srcname, स्थिर अक्षर *destname, libcrun_error_t *err)
{
  cleanup_close पूर्णांक destdirfd = dfd;
  cleanup_dir सूची *dsrcfd = शून्य;
  काष्ठा dirent *de;

  dsrcfd = fdopendir (srcdirfd);
  अगर (UNLIKELY (dsrcfd == शून्य))
    {
      अस्थायी_FAILURE_RETRY (बंद (srcdirfd));
      वापस crun_make_error (err, त्रुटि_सं, "open directory `%s`", destname);
    }

  क्रम (de = सूची_पढ़ो (dsrcfd); de; de = सूची_पढ़ो (dsrcfd))
    {
      cleanup_close पूर्णांक srcfd = -1;
      cleanup_close पूर्णांक destfd = -1;
      cleanup_free अक्षर *target_buf = शून्य;
      पूर्णांक ret;
      mode_t mode;
      off_t st_size;
      dev_t rdev;
      uid_t uid;
      gid_t gid;

      अगर (म_भेद (de->d_name, ".") == 0 || म_भेद (de->d_name, "..") == 0)
        जारी;

      ret = copy_rec_stat_file_at (dirfd (dsrcfd), de->d_name, &mode, &st_size, &rdev, &uid, &gid);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "stat `%s/%s`", srcname, de->d_name);

      चयन (mode & S_IFMT)
        {
        हाल S_IFREG:
          srcfd = openat (dirfd (dsrcfd), de->d_name, O_NONBLOCK | O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
          अगर (UNLIKELY (srcfd < 0))
            वापस crun_make_error (err, त्रुटि_सं, "open `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0777);
          अगर (UNLIKELY (destfd < 0))
            वापस crun_make_error (err, त्रुटि_सं, "open `%s/%s`", destname, de->d_name);

          ret = copy_from_fd_to_fd (srcfd, destfd, 1, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

#अगर_घोषित HAVE_FGETXATTR
          ret = (पूर्णांक) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
#पूर्ण_अगर

          अस्थायी_FAILURE_RETRY (बंद (destfd));
          destfd = -1;
          अवरोध;

        हाल S_IFDIR:
          ret = mkdirat (destdirfd, de->d_name, mode);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "mkdir `%s/%s`", destname, de->d_name);

          srcfd = openat (dirfd (dsrcfd), de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          अगर (UNLIKELY (srcfd < 0))
            वापस crun_make_error (err, त्रुटि_सं, "open directory `%s/%s`", srcname, de->d_name);

          destfd = openat (destdirfd, de->d_name, O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
          अगर (UNLIKELY (destfd < 0))
            वापस crun_make_error (err, त्रुटि_सं, "open directory `%s/%s`", srcname, de->d_name);

#अगर_घोषित HAVE_FGETXATTR
          ret = (पूर्णांक) copy_xattr (srcfd, destfd, de->d_name, de->d_name, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
#पूर्ण_अगर

          // copy_recursive_fd_to_fd closes srcfd and destfd
          ret = copy_recursive_fd_to_fd (srcfd, destfd, de->d_name, de->d_name, err);
          srcfd = destfd = -1;
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          अवरोध;

        हाल S_IFLNK:
          ret = safe_readlinkat (dirfd (dsrcfd), de->d_name, &target_buf, st_size, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          ret = symlinkat (target_buf, destdirfd, de->d_name);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "symlinkat `%s/%s`", destname, de->d_name);
          अवरोध;

        हाल S_IFBLK:
        हाल S_IFCHR:
        हाल S_IFIFO:
        हाल S_IFSOCK:
          ret = mknodat (destdirfd, de->d_name, mode, rdev);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "mknodat `%s/%s`", destname, de->d_name);
          अवरोध;
        }

      ret = fchownat (destdirfd, de->d_name, uid, gid, AT_SYMLINK_NOFOLLOW);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "fchownat `%s/%s`", destname, de->d_name);

      /*
       * ALLPERMS is not defined by POSIX
       */
#अगर_अघोषित ALLPERMS
#  define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#पूर्ण_अगर

      ret = fchmodat (destdirfd, de->d_name, mode & ALLPERMS, AT_SYMLINK_NOFOLLOW);
      अगर (UNLIKELY (ret < 0))
        {
          /* If the operation fails with ENOTSUP we are dealing with a symlink, so ignore it.  */
          अगर (त्रुटि_सं == ENOTSUP)
            जारी;

          वापस crun_make_error (err, त्रुटि_सं, "fchmodat `%s/%s`", destname, de->d_name);
        }
    }

  वापस 0;
}

स्थिर अक्षर *
find_annotation (libcrun_container_t *container, स्थिर अक्षर *name)
{
  अगर (container->container_def->annotations == शून्य)
    वापस शून्य;

  वापस find_string_map_मूल्य (container->annotations, name);
}

पूर्णांक
safe_write (पूर्णांक fd, स्थिर अक्षर *fname, स्थिर व्योम *buf, माप_प्रकार गिनती, libcrun_error_t *err)
{
  माप_प्रकार written = 0;
  जबतक (written < गिनती)
    {
      ssize_t w = ग_लिखो (fd, (स्थिर अक्षर *) buf + written, गिनती - written);
      अगर (UNLIKELY (w < 0))
        {
          अगर (त्रुटि_सं == EINTR || त्रुटि_सं == EAGAIN)
            जारी;
          वापस crun_make_error (err, त्रुटि_सं, "write to `%s`", fname);
        }
      written += w;
    }
  वापस 0;
}

#घोषणा MAX_PARTS 32

पूर्णांक
append_paths (अक्षर **out, libcrun_error_t *err, ...)
{
  स्थिर अक्षर *parts[MAX_PARTS];
  माप_प्रकार sizes[MAX_PARTS];
  माप_प्रकार योग_len = 0;
  माप_प्रकार n_parts = 0;
  माप_प्रकार copied = 0;
  बहु_सूची ap;
  माप_प्रकार i;

  बहु_शुरू (ap, err);
  क्रम (;;)
    {
      स्थिर अक्षर *part;
      माप_प्रकार आकार;

      part = बहु_तर्क (ap, स्थिर अक्षर *);
      अगर (part == शून्य)
        अवरोध;

      अगर (n_parts == MAX_PARTS)
        {
          बहु_पूर्ण (ap);
          वापस crun_make_error (err, EINVAL, "too many paths specified");
        }

      अगर (n_parts == 0)
        {
          /* For the first component allow only one '/'.  */
          जबतक (part[0] == '/' && part[1] == '/')
            part++;
        }
      अन्यथा
        {
          /* And drop any initial '/' for other components.  */
          जबतक (part[0] == '/')
            part++;
        }

      आकार = म_माप (part);
      अगर (आकार == 0)
        जारी;

      जबतक (आकार > 1 && part[आकार - 1] == '/')
        आकार--;

      parts[n_parts] = part;
      sizes[n_parts] = आकार;

      n_parts++;
    }
  बहु_पूर्ण (ap);

  योग_len = n_parts + 1;
  क्रम (i = 0; i < n_parts; i++)
    योग_len += sizes[i];

  *out = xmalloc (योग_len);

  copied = 0;
  क्रम (i = 0; i < n_parts; i++)
    {
      bool has_trailing_slash;

      has_trailing_slash = copied > 0 && (*out)[copied - 1] == '/';
      अगर (i > 0 && ! has_trailing_slash)
        {
          (*out)[copied] = '/';
          copied += 1;
        }

      स_नकल (*out + copied, parts[i], sizes[i]);
      copied += sizes[i];
    }
  (*out)[copied] = '\0';
  वापस 0;
}

#अगर __has_attribute(__nonstring__)
#  define __nonstring __attribute__ ((__nonstring__))
#अन्यथा
#  define __nonstring
#पूर्ण_अगर

/* Adapted from mailutils 0.6.91 (distributed under LGPL 2.0+)  */
अटल पूर्णांक
b64_निवेश (अक्षर c)
{
  स्थिर अक्षर table[64] __nonstring = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  पूर्णांक i;

  क्रम (i = 0; i < 64; i++)
    {
      अगर (table[i] == c)
        वापस i;
    }
  वापस -1;
}

पूर्णांक
base64_decode (स्थिर अक्षर *iptr, माप_प्रकार isize, अक्षर *optr, माप_प्रकार osize, माप_प्रकार *nbytes)
{
  पूर्णांक i = 0, tmp = 0, pad = 0;
  माप_प्रकार consumed = 0;
  अचिन्हित अक्षर तथ्य[4];

  *nbytes = 0;
  जबतक (consumed < isize && (*nbytes) + 3 < osize)
    {
      जबतक ((i < 4) && (consumed < isize))
        {
          tmp = b64_निवेश (*iptr++);
          consumed++;
          अगर (tmp != -1)
            {
              अगर (UNLIKELY (pad > 0))
                वापस -1;
              तथ्य[i++] = tmp;
            }
          अन्यथा अगर (*(iptr - 1) == '=')
            {
              तथ्य[i++] = '\0';
              pad++;
            }
        }

      /* I have an entire block of data 32 bits get the output data.  */
      अगर (i == 4)
        {
          अगर (UNLIKELY (pad > 2))
            वापस -1;
          *optr++ = (तथ्य[0] << 2) | ((तथ्य[1] & 0x30) >> 4);
          *optr++ = ((तथ्य[1] & 0xf) << 4) | ((तथ्य[2] & 0x3c) >> 2);
          *optr++ = ((तथ्य[2] & 0x3) << 6) | तथ्य[3];
          (*nbytes) += 3 - pad;
        }
      अन्यथा
        {
          /* I did not get all the data.  */
          consumed -= i;
          वापस consumed;
        }
      i = 0;
    }
  वापस consumed;
}

अक्षर *
get_user_name (uid_t uid)
{
  काष्ठा passwd pd;
  काष्ठा passwd *अस्थायी_परिणाम_ptr;
  अक्षर pwdbuffer[200];
  अगर (! getpwuid_r (uid, &pd, pwdbuffer, माप (pwdbuffer), &अस्थायी_परिणाम_ptr))
    वापस xstrdup (pd.pw_name);
  वापस xstrdup ("");
}

/* Convert a sysctl key to its /proc/sys path according to sysctl.d(5).
   If the first separator is '/', remaining slashes and dots are left intact.
   If the first separator is '.', dots and slashes are interchanged.  */
अक्षर *
libcrun_sysctl_कुंजी_to_proc_path (स्थिर अक्षर *कुंजी)
{
  स्थिर अक्षर *sep;
  अक्षर *ret, *it;

  sep = strpbrk (कुंजी, "./");

  /* No separator at all, or the key is already in the path form.  */
  अगर (sep == शून्य || *sep == '/')
    वापस xstrdup (कुंजी);

  ret = xstrdup (कुंजी);
  क्रम (it = ret; *it; it++)
    {
      अगर (*it == '.')
        *it = '/';
      अन्यथा अगर (*it == '/')
        *it = '.';
    }

  वापस ret;
}

पूर्णांक
has_suffix (स्थिर अक्षर *str, स्थिर अक्षर *suffix)
{
  अगर (! str || ! suffix)
    वापस 0;
  माप_प्रकार lenstr = म_माप (str);
  माप_प्रकार lensuffix = म_माप (suffix);
  अगर (lensuffix > lenstr)
    वापस 0;
  वापस स_भेद (str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

अक्षर *
str_join_array (पूर्णांक offset, माप_प्रकार आकार, अक्षर *स्थिर array[], स्थिर अक्षर *joint)
{
  माप_प्रकार jlen;
  cleanup_free माप_प्रकार *lens = शून्य;
  माप_प्रकार i, योग_आकार = (आकार - 1) * (jlen = म_माप (joint)) + 1;
  अक्षर *परिणाम, *p;

  lens = xmalloc (आकार * माप (*lens));

  क्रम (i = 0; i < आकार; ++i)
    {
      योग_आकार += (lens[i] = म_माप (array[i]));
    }
  p = परिणाम = xmalloc (योग_आकार);
  क्रम (i = offset; i < आकार; ++i)
    {
      स_नकल (p, array[i], lens[i]);
      p += lens[i];
      अगर (i < आकार - 1)
        {
          स_नकल (p, joint, jlen);
          p += jlen;
        }
    }
  *p = '\0';
  वापस परिणाम;
}

पूर्णांक
libcrun_mmap (काष्ठा libcrun_mmap_s **ret, व्योम *addr, माप_प्रकार लंबाई,
              पूर्णांक prot, पूर्णांक flags, पूर्णांक fd, off_t offset,
              libcrun_error_t *err)
{
  काष्ठा libcrun_mmap_s *mmap_s = शून्य;

  व्योम *mapped = mmap (addr, लंबाई, prot, flags, fd, offset);
  अगर (mapped == MAP_FAILED)
    वापस crun_make_error (err, त्रुटि_सं, "mmap");

  mmap_s = xmalloc (माप (काष्ठा libcrun_mmap_s));
  mmap_s->addr = mapped;
  mmap_s->लंबाई = लंबाई;

  *ret = mmap_s;

  वापस 0;
}

पूर्णांक
libcrun_munmap (काष्ठा libcrun_mmap_s *mmap, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = munmap (mmap->addr, mmap->लंबाई);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "munmap");

  मुक्त (mmap);

  वापस 0;
}

अटल दीर्घ
read_file_as_long_or_default (स्थिर अक्षर *path, दीर्घ def_मूल्य)
{
  cleanup_free अक्षर *content = शून्य;
  libcrun_error_t tmp_err = शून्य;
  अक्षर *endptr;
  दीर्घ val;
  पूर्णांक ret;

  ret = read_all_file (path, &content, शून्य, &tmp_err);
  अगर (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      वापस def_मूल्य;
    }

  त्रुटि_सं = 0;
  val = म_से_दीर्घ (content, &endptr, 10);
  अगर (UNLIKELY (त्रुटि_सं))
    वापस def_मूल्य;
  अगर (endptr == content || (*endptr && *endptr != '\n'))
    वापस def_मूल्य;
  वापस val;
}

#घोषणा DEFAULT_OVERFLOW_ID 65534

uid_t
get_overflow_uid (व्योम)
{
  अटल atomic_long cached_uid = -1;
  atomic_long uid = cached_uid;
  अगर (uid == -1)
    {
      uid = read_file_as_long_or_default ("/proc/sys/kernel/overflowuid", DEFAULT_OVERFLOW_ID);
      cached_uid = uid;
    }
  वापस uid;
}

gid_t
get_overflow_gid (व्योम)
{
  अटल atomic_long cached_gid = -1;
  atomic_long gid = cached_gid;
  अगर (gid == -1)
    {
      gid = read_file_as_long_or_default ("/proc/sys/kernel/overflowgid", DEFAULT_OVERFLOW_ID);
      cached_gid = gid;
    }
  वापस gid;
}

व्योम
consume_trailing_slashes (अक्षर *path)
{
  अगर (! path || path[0] == '\0')
    वापस;

  अक्षर *last = path + म_माप (path);

  जबतक (last > path && *(last - 1) == '/')
    last--;

  *last = '\0';
}

अक्षर **
read_dir_entries (स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_dir सूची *dir = शून्य;
  माप_प्रकार n_entries = 0;
  माप_प्रकार entries_आकार = 16;
  अक्षर **entries = शून्य;
  काष्ठा dirent *de;

  dir = सूची_खोलो (path);
  अगर (UNLIKELY (dir == शून्य))
    {
      crun_make_error (err, त्रुटि_सं, "opendir `%s`", path);
      वापस शून्य;
    }

  entries = xmalloc (entries_आकार * माप (अक्षर *));
  जबतक ((de = सूची_पढ़ो (dir)))
    {
      अगर (म_भेद (de->d_name, ".") == 0 || म_भेद (de->d_name, "..") == 0)
        जारी;
      अगर (n_entries == entries_आकार)
        {
          entries_आकार *= 2;
          entries = xrealloc (entries, entries_आकार * माप (अक्षर *));
        }
      entries[n_entries++] = xstrdup (de->d_name);
    }
  entries = xrealloc (entries, (n_entries + 1) * माप (अक्षर *));
  entries[n_entries] = शून्य;

  वापस entries;
}

पूर्णांक
cpuset_string_to_bitmask (स्थिर अक्षर *str, अक्षर **out, माप_प्रकार *out_आकार, libcrun_error_t *err)
{
  cleanup_free अक्षर *mask = शून्य;
  माप_प्रकार mask_आकार = 0;
  स्थिर अक्षर *p = str;
  अक्षर *endptr;

  जबतक (*p)
    {
      दीर्घ दीर्घ start_range, end_range;

      अगर (*p < '0' || *p > '9')
        जाओ invalid_निवेश;

      start_range = strtoll (p, &endptr, 10);
      अगर (start_range < 0)
        जाओ invalid_निवेश;

      p = endptr;

      अगर (*p != '-')
        end_range = start_range;
      अन्यथा
        {
          p++;

          अगर (*p < '0' || *p > '9')
            जाओ invalid_निवेश;

          end_range = strtoll (p, &endptr, 10);

          अगर (end_range < start_range)
            जाओ invalid_निवेश;

          p = endptr;
        }

      /* Just set some limit.  */
      अगर (end_range > (1 << 20))
        जाओ invalid_निवेश;

      अगर (end_range >= (दीर्घ दीर्घ) (mask_आकार * अक्षर_बिट))
        {
          माप_प्रकार new_mask_आकार = (end_range / अक्षर_बिट) + 1;
          mask = xrealloc (mask, new_mask_आकार);
          स_रखो (mask + mask_आकार, 0, new_mask_आकार - mask_आकार);
          mask_आकार = new_mask_आकार;
        }

      क्रम (दीर्घ दीर्घ i = start_range; i <= end_range; i++)
        mask[i / अक्षर_बिट] |= (1 << (i % अक्षर_बिट));

      अगर (*p == ',')
        p++;
      अन्यथा अगर (*p)
        जाओ invalid_निवेश;
    }

  *out = mask;
  mask = शून्य;
  *out_आकार = mask_आकार;

  वापस 0;

invalid_निवेश:
  वापस crun_make_error (err, 0, "cannot parse input `%s`", str);
}

काष्ठा channel_fd_pair
{
  काष्ठा ring_बफर *rb;

  पूर्णांक in_fd;
  पूर्णांक out_fd;

  पूर्णांक infd_epoll_events;
  पूर्णांक outfd_epoll_events;
};

काष्ठा channel_fd_pair *
channel_fd_pair_new (पूर्णांक in_fd, पूर्णांक out_fd, माप_प्रकार आकार)
{
  काष्ठा channel_fd_pair *channel = xmalloc (माप (काष्ठा channel_fd_pair));
  channel->in_fd = in_fd;
  channel->out_fd = out_fd;
  channel->infd_epoll_events = -1;
  channel->outfd_epoll_events = -1;
  channel->rb = ring_बफर_make (आकार);
  वापस channel;
}

व्योम
channel_fd_pair_free (काष्ठा channel_fd_pair *channel)
{
  अगर (channel == शून्य)
    वापस;

  ring_बफर_free (channel->rb);
  मुक्त (channel);
}

पूर्णांक
channel_fd_pair_process (काष्ठा channel_fd_pair *channel, पूर्णांक epollfd, libcrun_error_t *err)
{
  bool is_निवेश_eagain = false, is_output_eagain = false, repeat;
  पूर्णांक ret, i;

  /* This function is called from an epoll loop.  Use a hard limit to avoid infinite loops
     and prevent other events from being processed.  */
  क्रम (i = 0, repeat = true; i < 1000 && repeat; i++)
    {
      repeat = false;
      अगर (ring_बफर_get_space_available (channel->rb) > 0)
        {
          ret = ring_बफर_read (channel->rb, channel->in_fd, &is_निवेश_eagain, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          अगर (ret > 0)
            repeat = true;
        }
      अगर (ring_बफर_get_तथ्य_available (channel->rb) > 0)
        {
          ret = ring_बफर_write (channel->rb, channel->out_fd, &is_output_eagain, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          अगर (ret > 0)
            repeat = true;
        }
    }

  अगर (epollfd >= 0)
    {
      माप_प्रकार available = ring_बफर_get_space_available (channel->rb);
      माप_प्रकार used = ring_बफर_get_तथ्य_available (channel->rb);
      पूर्णांक events;

      /* If there is space available in the buffer and the output is not
         blocked, we want to read more.  When the output got EAGAIN, stop
         reading until the output fd becomes writable again to avoid a
         busy loop when the consumer is not draining the pipe.  */
      events = (available > 0 && ! is_output_eagain) ? (EPOLLIN | (is_निवेश_eagain ? EPOLLET : 0)) : 0;
      अगर (events != channel->infd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->in_fd, events, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          channel->infd_epoll_events = events;
        }

      /* If there is data available in the buffer, we want to write as soon as
         it is possible.  */
      events = (used > 0) ? (EPOLLOUT | (is_output_eagain ? EPOLLET : 0)) : 0;
      अगर (events != channel->outfd_epoll_events)
        {
          ret = epoll_helper_toggle (epollfd, channel->out_fd, events, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          channel->outfd_epoll_events = events;
        }
    }
  वापस 0;
}

अटल पूर्णांक
open_proc (libcrun_error_t *err)
{
  पूर्णांक fd;

#अगर_घोषित HAVE_NEW_MOUNT_API
  cleanup_close पूर्णांक fsfd = -1;
  पूर्णांक ret;

  fsfd = syscall_fsopen ("proc", FSOPEN_CLOEXEC);
  अगर (fsfd >= 0)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, शून्य, शून्य, 0);
      अगर (ret >= 0)
        {
          fd = syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
          अगर (fd >= 0)
            वापस fd;
        }
    }
#पूर्ण_अगर

  fd = खोलो ("/proc", O_PATH | O_DIRECTORY | O_CLOEXEC);
  अगर (fd < 0)
    वापस crun_make_error (err, त्रुटि_सं, "open `/proc`");
  वापस fd;
}

पूर्णांक
libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err)
{
  अगर (! container)
    वापस open_proc (err);

  अगर (container->proc_fd < 0)
    container->proc_fd = open_proc (err);

  वापस container->proc_fd;
}

पूर्णांक
libcrun_open_proc_file (libcrun_container_t *container, स्थिर अक्षर *path, पूर्णांक flags, libcrun_error_t *err)
{
  पूर्णांक proc_fd, fd, saved_errno;

  proc_fd = libcrun_get_cached_proc_fd (container, err);
  अगर (proc_fd < 0)
    वापस proc_fd;

  fd = अस्थायी_FAILURE_RETRY (openat (proc_fd, path, flags | O_CLOEXEC));
  saved_errno = त्रुटि_सं;
  /* requires closing if not cached */
  अगर (! container)
    अस्थायी_FAILURE_RETRY (बंद (proc_fd));

  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, saved_errno, "openat `/proc/%s`", path);

  वापस fd;
}

पूर्णांक
libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, स्थिर अक्षर *path, पूर्णांक flags, libcrun_error_t *err)
{
  cleanup_free अक्षर *full_path = शून्य;
  xasprintf (&full_path, "%d/%s", pid, path);
  वापस libcrun_open_proc_file (container, full_path, flags, err);
}
