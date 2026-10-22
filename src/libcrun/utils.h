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
#ifndef UTILS_H
#define UTILS_H

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव "error.h"
#अन्तर्भाव <dirent.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव <sys/wait.h>
#अन्तर्भाव "container.h"

#ifndef TEMP_FAILURE_RETRY
#  define TEMP_FAILURE_RETRY(expression)      \
    (__extension__ ({                         \
      दीर्घ पूर्णांक __result;                      \
      करा                                      \
        __result = (दीर्घ पूर्णांक) (expression);   \
      पर्यंत (__result < 0 && त्रुटि_सं == EINTR); \
      __result;                               \
    }))
#endif

#define cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#define cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#define cleanup_close __attribute__ ((cleanup (cleanup_closep)))
#define cleanup_close_vec __attribute__ ((cleanup (cleanup_close_vecp)))
#define cleanup_close_map __attribute__ ((cleanup (cleanup_close_mapp)))
#define cleanup_dir __attribute__ ((cleanup (cleanup_dirp)))
#define arg_unused __attribute__ ((unused))
#define cleanup_pid __attribute__ ((cleanup (cleanup_pidp)))
#define cleanup_mmap __attribute__ ((cleanup (cleanup_mmapp)))

#define LIKELY(x) __builtin_expect ((x), 1)
#define UNLIKELY(x) __builtin_expect ((x), 0)

#define WRITE_FILE_DEFAULT_FLAGS (O_CLOEXEC | O_CREAT | O_TRUNC | O_WRONLY)

__attribute__ ((दो_स्मृति)) स्थिर ओळीत शून्य *
xmalloc (माप_प्रकार size)
{
  शून्य *res = दो_स्मृति (size);
  जर (UNLIKELY (res == रिक्त))
    OOM ();
  परत res;
}

__attribute__ ((दो_स्मृति)) स्थिर ओळीत शून्य *
xmalloc0 (माप_प्रकार size)
{
  शून्य *res = सुस्मृति (1, size);
  जर (UNLIKELY (res == रिक्त))
    OOM ();
  परत res;
}

__attribute__ ((दो_स्मृति)) स्थिर ओळीत शून्य *
xrealloc (शून्य *ptr, माप_प्रकार size)
{
  शून्य *res = पुनः_स्मृति (ptr, size);
  जर (UNLIKELY (res == रिक्त))
    OOM ();
  परत res;
}

स्थिर ओळीत शून्य
cleanup_freep (शून्य *p)
{
  शून्य **pp = (शून्य **) p;
  मुक्त (*pp);
}

स्थिर ओळीत शून्य
cleanup_filep (खाता **f)
{
  खाता *file = *f;
  जर (file)
    (शून्य) ख_बंद (file);
}

स्थिर ओळीत शून्य
cleanup_closep (शून्य *p)
{
  पूर्णांक *pp = (पूर्णांक *) p;
  जर (*pp >= 0)
    TEMP_FAILURE_RETRY (बंद (*pp));
}

स्थिर ओळीत शून्य
cleanup_pidp (शून्य *p)
{
  pid_t *pp = (pid_t *) p;
  जर (*pp > 0)
    {
      TEMP_FAILURE_RETRY (समाप्त (*pp, SIGKILL));
      TEMP_FAILURE_RETRY (waitpid (*pp, रिक्त, 0));
    }
}

संरचना libcrun_mmap_s
{
  शून्य *addr;
  माप_प्रकार length;
};

पूर्णांक libcrun_mmap (संरचना libcrun_mmap_s **ret, शून्य *addr, माप_प्रकार length,
                  पूर्णांक prot, पूर्णांक flags, पूर्णांक fd, off_t offset,
                  libcrun_error_t *err);

पूर्णांक libcrun_munmap (संरचना libcrun_mmap_s *mmap, libcrun_error_t *err);

स्थिर ओळीत शून्य
cleanup_mmapp (शून्य *p)
{
  पूर्णांक ret;
  libcrun_error_t tmp_err = रिक्त;
  संरचना libcrun_mmap_s **mm;

  mm = (संरचना libcrun_mmap_s **) p;
  जर (*mm == रिक्त)
    परत;

  ret = libcrun_munmap (*mm, &tmp_err);
  जर (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

संरचना libcrun_fd_map
{
  माप_प्रकार nfds;
  पूर्णांक fds[];
};

स्थिर ओळीत संरचना libcrun_fd_map *
make_libcrun_fd_map (माप_प्रकार len)
{
  संरचना libcrun_fd_map *ret;
  माप_प्रकार i;

  ret = (संरचना libcrun_fd_map *) xmalloc (आकार (*ret) + आकार (पूर्णांक) * len);
  ret->nfds = len;
  साठी (i = 0; i < len; i++)
    ret->fds[i] = -1;

  परत ret;
}

स्थिर ओळीत शून्य
cleanup_close_mapp (संरचना libcrun_fd_map **p)
{
  संरचना libcrun_fd_map *m = *p;
  माप_प्रकार i;

  जर (m == रिक्त)
    परत;

  साठी (i = 0; i < m->nfds; i++)
    जर (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (बंद (m->fds[i]));

  मुक्त (m);
}

स्थिर ओळीत शून्य
cleanup_close_vecp (पूर्णांक **p)
{
  पूर्णांक *pp = *p;
  पूर्णांक i;

  जर (pp == रिक्त)
    परत;

  साठी (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (बंद (pp[i]));
}

स्थिर ओळीत शून्य
cleanup_dirp (सूची **p)
{
  सूची *dir = *p;
  जर (dir)
    बंद_सूची (dir);
}

स्थिर ओळीत पूर्णांक
close_and_reset (पूर्णांक *fd)
{
  पूर्णांक ret = 0;
  जर (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (बंद (*fd));
      जर (LIKELY (ret == 0))
        *fd = -1;
    }
  परत ret;
}

स्थिर ओळीत अक्षर *
xstrdup (स्थिरांक अक्षर *str)
{
  अक्षर *ret;
  जर (str == रिक्त)
    परत रिक्त;

  ret = strdup (str);
  जर (ret == रिक्त)
    OOM ();

  परत ret;
}

शून्य consume_trailing_slashes (अक्षर *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    पर्यंत (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

स्थिर ओळीत bool
path_is_slash_dev (स्थिरांक अक्षर *path)
{
  path = consume_slashes (path);

  जर (म_भेदन (path, "dev", 3))
    परत false;

  path += 3;

  /* Check there are only '/' left.  */
  साठी (; *path; path++)
    जर (*path != '/')
      परत false;

  परत true;
}

पूर्णांक xasprintf (अक्षर **str, स्थिरांक अक्षर *fmt, ...) __attribute__ ((format (म_लिखो, 2, 3)));

पूर्णांक crun_path_exists (स्थिरांक अक्षर *path, libcrun_error_t *err);

पूर्णांक write_file_at_with_flags (पूर्णांक dirfd, पूर्णांक flags, mode_t mode, स्थिरांक अक्षर *name, स्थिरांक शून्य *data, माप_प्रकार len, libcrun_error_t *err);

स्थिर ओळीत पूर्णांक
write_file (स्थिरांक अक्षर *name, स्थिरांक शून्य *data, माप_प्रकार len, libcrun_error_t *err)
{
  परत write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

स्थिर ओळीत पूर्णांक
write_file_at (पूर्णांक dirfd, स्थिरांक अक्षर *name, स्थिरांक शून्य *data, माप_प्रकार len, libcrun_error_t *err)
{
  परत write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

पूर्णांक crun_ensure_directory (स्थिरांक अक्षर *path, पूर्णांक mode, bool nofollow, libcrun_error_t *err);

पूर्णांक crun_ensure_directory_at (पूर्णांक dirfd, स्थिरांक अक्षर *path, पूर्णांक mode, bool nofollow, libcrun_error_t *err);

पूर्णांक crun_safe_create_and_open_ref_at (bool dir, पूर्णांक dirfd, स्थिरांक अक्षर *dirpath, स्थिरांक अक्षर *path, पूर्णांक mode, libcrun_error_t *err);

पूर्णांक crun_safe_ensure_directory_at (पूर्णांक dirfd, स्थिरांक अक्षर *dirpath, स्थिरांक अक्षर *path, पूर्णांक mode,
                                   libcrun_error_t *err);

पूर्णांक crun_safe_ensure_file_at (पूर्णांक dirfd, स्थिरांक अक्षर *dirpath, स्थिरांक अक्षर *path, पूर्णांक mode,
                              libcrun_error_t *err);

पूर्णांक crun_dir_p (स्थिरांक अक्षर *path, bool nofollow, libcrun_error_t *err);

पूर्णांक crun_dir_p_at (पूर्णांक dirfd, स्थिरांक अक्षर *path, bool nofollow, libcrun_error_t *err);

पूर्णांक detach_process ();

पूर्णांक create_file_if_missing_at (पूर्णांक dirfd, स्थिरांक अक्षर *file, mode_t mode, libcrun_error_t *err);

पूर्णांक check_running_in_user_namespace (libcrun_error_t *err);

पूर्णांक set_selinux_label (libcrun_container_t *container, स्थिरांक अक्षर *label, bool now, libcrun_error_t *err);

पूर्णांक add_selinux_mount_label (अक्षर **ret, स्थिरांक अक्षर *data, स्थिरांक अक्षर *label, स्थिरांक अक्षर *context_type, libcrun_error_t *err);

पूर्णांक set_apparmor_profile (libcrun_container_t *container, स्थिरांक अक्षर *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

पूर्णांक read_all_fd_with_size_hint (पूर्णांक fd, स्थिरांक अक्षर *description, अक्षर **out, माप_प्रकार *len, माप_प्रकार hint, libcrun_error_t *err);

पूर्णांक read_all_fd_with_max_size (पूर्णांक fd, स्थिरांक अक्षर *description, अक्षर **out, माप_प्रकार *len, माप_प्रकार max_size, libcrun_error_t *err);

स्थिर ओळीत पूर्णांक
read_all_fd (पूर्णांक fd, स्थिरांक अक्षर *description, अक्षर **out, माप_प्रकार *len, libcrun_error_t *err)
{
  परत read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

पूर्णांक get_realpath_to_file (पूर्णांक dirfd, स्थिरांक अक्षर *path_name, अक्षर **absolute_path, libcrun_error_t *err);

पूर्णांक read_all_file (स्थिरांक अक्षर *path, अक्षर **out, माप_प्रकार *len, libcrun_error_t *err);

पूर्णांक read_all_file_at (पूर्णांक dirfd, स्थिरांक अक्षर *path, अक्षर **out, माप_प्रकार *len, libcrun_error_t *err);

पूर्णांक open_unix_domain_client_socket (स्थिरांक अक्षर *path, पूर्णांक dgram, libcrun_error_t *err);

पूर्णांक open_unix_domain_socket (स्थिरांक अक्षर *path, पूर्णांक dgram, libcrun_error_t *err);

पूर्णांक send_fd_to_socket (पूर्णांक server, पूर्णांक fd, libcrun_error_t *err);

पूर्णांक send_fd_to_socket_with_payload (पूर्णांक server, पूर्णांक fd, स्थिरांक अक्षर *payload, माप_प्रकार payload_len, libcrun_error_t *err);

पूर्णांक create_socket_pair (पूर्णांक *pair, libcrun_error_t *err);

पूर्णांक receive_fd_from_socket (पूर्णांक from, libcrun_error_t *err);

पूर्णांक receive_fd_from_socket_with_payload (पूर्णांक from, अक्षर *payload, माप_प्रकार payload_len, libcrun_error_t *err);

पूर्णांक create_signalfd (sigset_t *mask, libcrun_error_t *err);

पूर्णांक epoll_helper (पूर्णांक *in_fds, पूर्णांक *in_levelfds, पूर्णांक *out_fds, पूर्णांक *out_levelfds, libcrun_error_t *err);

पूर्णांक copy_from_fd_to_fd (पूर्णांक src, पूर्णांक dst, पूर्णांक consume, libcrun_error_t *err);

पूर्णांक run_process (अक्षर **args, libcrun_error_t *err);

पूर्णांक format_default_id_mapping (अक्षर **out, uid_t container_id, uid_t host_uid, uid_t host_id, पूर्णांक is_uid, libcrun_error_t *err);

पूर्णांक run_process_with_stdin_timeout_envp (अक्षर *path, अक्षर **args, स्थिरांक अक्षर *cwd, पूर्णांक timeout, अक्षर **envp,
                                         अक्षर *मानक_निवेश, माप_प्रकार stdin_len, पूर्णांक out_fd, पूर्णांक err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

पूर्णांक mark_or_close_fds_ge_than (libcrun_container_t *container, पूर्णांक n, bool close_now, libcrun_error_t *err);

शून्य get_current_timestamp (अक्षर *out, माप_प्रकार len);

पूर्णांक set_blocking_fd (पूर्णांक fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC पूर्णांक parse_json_file (json_object **out, स्थिरांक अक्षर *jsondata, संरचना parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
स्थिर ओळीत पूर्णांक
has_prefix (स्थिरांक अक्षर *str, स्थिरांक अक्षर *prefix)
{
  माप_प्रकार prefix_len;

  जर (! str || ! prefix)
    परत 0;

  prefix_len = म_माप (prefix);
  परत म_माप (str) >= prefix_len && स_भेद (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
स्थिर ओळीत पूर्णांक
has_dir_prefix (स्थिरांक अक्षर *str, स्थिरांक अक्षर *prefix)
{
  माप_प्रकार prefix_len;

  जर (! str || ! prefix)
    परत 0;

  prefix_len = म_माप (prefix);
  परत म_माप (str) > prefix_len && स_भेद (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

पूर्णांक find_executable (अक्षर **exec_path, स्थिरांक अक्षर *executable_path, स्थिरांक अक्षर *cwd, libcrun_error_t *err);

पूर्णांक copy_recursive_fd_to_fd (पूर्णांक srcfd, पूर्णांक destfd, स्थिरांक अक्षर *srcname, स्थिरांक अक्षर *destname, libcrun_error_t *err);

पूर्णांक set_home_env (uid_t uid);

पूर्णांक libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

पूर्णांक libcrun_initialize_apparmor (libcrun_error_t *err);

स्थिरांक अक्षर *find_annotation (libcrun_container_t *container, स्थिरांक अक्षर *name);

पूर्णांक get_file_type_at (पूर्णांक dirfd, mode_t *mode, bool nofollow, स्थिरांक अक्षर *path);

पूर्णांक get_file_type (mode_t *mode, bool nofollow, स्थिरांक अक्षर *path);

पूर्णांक get_file_type_fd (पूर्णांक fd, mode_t *mode);

अक्षर *get_user_name (uid_t uid);

पूर्णांक safe_openat (पूर्णांक dirfd, स्थिरांक अक्षर *rootfs, स्थिरांक अक्षर *path, पूर्णांक flags, पूर्णांक mode,
                 libcrun_error_t *err);

पूर्णांक safe_write (पूर्णांक fd, स्थिरांक अक्षर *fname, स्थिरांक शून्य *buf, माप_प्रकार मोजणी, libcrun_error_t *err);

पूर्णांक append_paths (अक्षर **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

पूर्णांक str2sig (स्थिरांक अक्षर *name);

पूर्णांक base64_decode (स्थिरांक अक्षर *iptr, माप_प्रकार isize, अक्षर *optr, माप_प्रकार osize, माप_प्रकार *nbytes);
पूर्णांक has_suffix (स्थिरांक अक्षर *source, स्थिरांक अक्षर *suffix);
अक्षर *libcrun_sysctl_किल्ली_to_proc_path (स्थिरांक अक्षर *किल्ली);
अक्षर *str_join_array (पूर्णांक offset, माप_प्रकार size, अक्षर *स्थिरांक array[], स्थिरांक अक्षर *joint);

ssize_t safe_readlinkat (पूर्णांक dfd, स्थिरांक अक्षर *name, अक्षर **buffer, ssize_t hint, libcrun_error_t *err);

अक्षर **read_dir_entries (स्थिरांक अक्षर *path, libcrun_error_t *err);

स्थिर ओळीत bool
is_empty_string (स्थिरांक अक्षर *s)
{
  परत s == रिक्त || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
स्थिर ओळीत माप_प्रकार
safe_strlen (स्थिरांक अक्षर *s)
{
  परत s == रिक्त ? 0 : म_माप (s);
}

स्थिर ओळीत bool
path_has_dot_dot_component (स्थिरांक अक्षर *path)
{
  जर (path == रिक्त)
    परत false;
  cleanup_free अक्षर *npath = xstrdup (path);
  अक्षर *cur, *it;

  cur = npath;
  it = म_अक्षर (cur, '/');
  पर्यंत (cur)
    {
      जर (it)
        *it = '\0';

      जर (म_भेद (cur, "..") == 0)
        परत true;

      जर (it == रिक्त)
        थांबा;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = म_अक्षर (cur, '/');
    }
  परत false;
}

स्थिर ओळीत पूर्णांक
waitpid_ignore_stopped (pid_t pid, पूर्णांक *status, पूर्णांक options)
{
  पूर्णांक ret, s;
  करा
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      जर (ret < 0)
        परत ret;
  } पर्यंत (WIFSTOPPED (s) || WIFCONTINUED (s));

  जर (status)
    *status = s;

  परत ret;
}

स्थिर ओळीत पूर्णांक
get_process_exit_status (पूर्णांक status)
{
  जर (WIFEXITED (status))
    परत WEXITSTATUS (status);
  जर (WIFSIGNALED (status))
    परत 128 + WTERMSIG (status);

  परत -1;
}

uid_t get_overflow_uid (शून्य);
gid_t get_overflow_gid (शून्य);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((माप_प्रकार) 2U + (आकार (type) <= 1 ? 3U : आकार (type) <= 2 ? 5U  \
                                        : आकार (type) <= 4   ? 10U \
                                        : आकार (type) <= 8   ? 20U \
                                                               : आकार (पूर्णांक[-2 * (आकार (type) > 8)])))

#define _STRLEN(s) (आकार (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (पूर्णांक))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
प्रकारव्याख्या अक्षर proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

स्थिर ओळीत शून्य
get_proc_fd_path (proc_fd_path_t path, pid_t pid, पूर्णांक fd)
{
  स्थिरांक माप_प्रकार max_len = आकार (proc_fd_path_t);
  माप_प्रकार n;

  जर (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  अन्यथा
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  जर (UNLIKELY (n >= max_len))
    पात ();
}

स्थिर ओळीत शून्य
get_proc_self_fd_path (proc_fd_path_t path, पूर्णांक fd)
{
  get_proc_fd_path (path, 0, fd);
}

स्थिर ओळीत शून्य
get_self_fd_path (proc_fd_path_t path, पूर्णांक fd)
{
  snprintf (path, आकार (proc_fd_path_t), "self/fd/%d", fd);
}

स्थिर ओळीत पूर्णांक
validate_options (अचिन्हित पूर्णांक specified_options, अचिन्हित पूर्णांक supported_options, libcrun_error_t *err)
{
  जर (! ! (~supported_options & specified_options))
    परत crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  परत 0;
}

बाह्य पूर्णांक cpuset_string_to_bitmask (स्थिरांक अक्षर *str, अक्षर **out, माप_प्रकार *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
संरचना channel_fd_pair;

संरचना channel_fd_pair *channel_fd_pair_new (पूर्णांक in_fd, पूर्णांक out_fd, माप_प्रकार size);

शून्य channel_fd_pair_free (संरचना channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
पूर्णांक channel_fd_pair_process (संरचना channel_fd_pair *channel, पूर्णांक epollfd, libcrun_error_t *err);

स्थिर ओळीत शून्य
cleanup_channel_fd_pairp (शून्य *p)
{
  संरचना channel_fd_pair **pp = (संरचना channel_fd_pair **) p;
  जर (*pp == रिक्त)
    परत;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

पूर्णांक libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
पूर्णांक libcrun_open_proc_file (libcrun_container_t *container, स्थिरांक अक्षर *path, पूर्णांक flags, libcrun_error_t *err);
पूर्णांक libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, स्थिरांक अक्षर *path, पूर्णांक flags, libcrun_error_t *err);

#endif
