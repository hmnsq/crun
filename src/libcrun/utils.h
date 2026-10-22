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
#अगर_अघोषित UTILS_H
#घोषणा UTILS_H

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>
#समावेश <errno.h>
#समावेश <argp.h>
#समावेश "error.h"
#समावेश <dirent.h>
#समावेश <unistd.h>
#समावेश <signal.h>
#समावेश <fcntl.h>
#समावेश <ocispec/runtime_spec_schema_config_schema.h>
#समावेश <sys/wait.h>
#समावेश "container.h"

#अगर_अघोषित अस्थायी_FAILURE_RETRY
#  define अस्थायी_FAILURE_RETRY(expression)      \
    (__extension__ ({                         \
      दीर्घ पूर्णांक __result;                      \
      करो                                      \
        __result = (दीर्घ पूर्णांक) (expression);   \
      जबतक (__result < 0 && त्रुटि_सं == EINTR); \
      __result;                               \
    }))
#पूर्ण_अगर

#घोषणा cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#घोषणा cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#घोषणा cleanup_close __attribute__ ((cleanup (cleanup_closep)))
#घोषणा cleanup_close_vec __attribute__ ((cleanup (cleanup_close_vecp)))
#घोषणा cleanup_close_map __attribute__ ((cleanup (cleanup_close_mapp)))
#घोषणा cleanup_dir __attribute__ ((cleanup (cleanup_dirp)))
#घोषणा arg_unused __attribute__ ((unused))
#घोषणा cleanup_pid __attribute__ ((cleanup (cleanup_pidp)))
#घोषणा cleanup_mmap __attribute__ ((cleanup (cleanup_mmapp)))

#घोषणा LIKELY(x) __builtin_expect ((x), 1)
#घोषणा UNLIKELY(x) __builtin_expect ((x), 0)

#घोषणा WRITE_FILE_DEFAULT_FLAGS (O_CLOEXEC | O_CREAT | O_TRUNC | O_WRONLY)

__attribute__ ((दो_स्मृति)) अटल अंतरभूत व्योम *
xmalloc (माप_प्रकार आकार)
{
  व्योम *res = दो_स्मृति (आकार);
  अगर (UNLIKELY (res == शून्य))
    OOM ();
  वापस res;
}

__attribute__ ((दो_स्मृति)) अटल अंतरभूत व्योम *
xmalloc0 (माप_प्रकार आकार)
{
  व्योम *res = सुस्मृति (1, आकार);
  अगर (UNLIKELY (res == शून्य))
    OOM ();
  वापस res;
}

__attribute__ ((दो_स्मृति)) अटल अंतरभूत व्योम *
xrealloc (व्योम *ptr, माप_प्रकार आकार)
{
  व्योम *res = पुनः_स्मृति (ptr, आकार);
  अगर (UNLIKELY (res == शून्य))
    OOM ();
  वापस res;
}

अटल अंतरभूत व्योम
cleanup_freep (व्योम *p)
{
  व्योम **pp = (व्योम **) p;
  मुक्त (*pp);
}

अटल अंतरभूत व्योम
cleanup_filep (खाता **f)
{
  खाता *file = *f;
  अगर (file)
    (व्योम) ख_बंद (file);
}

अटल अंतरभूत व्योम
cleanup_closep (व्योम *p)
{
  पूर्णांक *pp = (पूर्णांक *) p;
  अगर (*pp >= 0)
    अस्थायी_FAILURE_RETRY (बंद (*pp));
}

अटल अंतरभूत व्योम
cleanup_pidp (व्योम *p)
{
  pid_t *pp = (pid_t *) p;
  अगर (*pp > 0)
    {
      अस्थायी_FAILURE_RETRY (समाप्त (*pp, SIGKILL));
      अस्थायी_FAILURE_RETRY (waitpid (*pp, शून्य, 0));
    }
}

काष्ठा libcrun_mmap_s
{
  व्योम *addr;
  माप_प्रकार लंबाई;
};

पूर्णांक libcrun_mmap (काष्ठा libcrun_mmap_s **ret, व्योम *addr, माप_प्रकार लंबाई,
                  पूर्णांक prot, पूर्णांक flags, पूर्णांक fd, off_t offset,
                  libcrun_error_t *err);

पूर्णांक libcrun_munmap (काष्ठा libcrun_mmap_s *mmap, libcrun_error_t *err);

अटल अंतरभूत व्योम
cleanup_mmapp (व्योम *p)
{
  पूर्णांक ret;
  libcrun_error_t tmp_err = शून्य;
  काष्ठा libcrun_mmap_s **mm;

  mm = (काष्ठा libcrun_mmap_s **) p;
  अगर (*mm == शून्य)
    वापस;

  ret = libcrun_munmap (*mm, &tmp_err);
  अगर (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

काष्ठा libcrun_fd_map
{
  माप_प्रकार nfds;
  पूर्णांक fds[];
};

अटल अंतरभूत काष्ठा libcrun_fd_map *
make_libcrun_fd_map (माप_प्रकार len)
{
  काष्ठा libcrun_fd_map *ret;
  माप_प्रकार i;

  ret = (काष्ठा libcrun_fd_map *) xmalloc (माप (*ret) + माप (पूर्णांक) * len);
  ret->nfds = len;
  क्रम (i = 0; i < len; i++)
    ret->fds[i] = -1;

  वापस ret;
}

अटल अंतरभूत व्योम
cleanup_close_mapp (काष्ठा libcrun_fd_map **p)
{
  काष्ठा libcrun_fd_map *m = *p;
  माप_प्रकार i;

  अगर (m == शून्य)
    वापस;

  क्रम (i = 0; i < m->nfds; i++)
    अगर (m->fds[i] >= 0)
      अस्थायी_FAILURE_RETRY (बंद (m->fds[i]));

  मुक्त (m);
}

अटल अंतरभूत व्योम
cleanup_close_vecp (पूर्णांक **p)
{
  पूर्णांक *pp = *p;
  पूर्णांक i;

  अगर (pp == शून्य)
    वापस;

  क्रम (i = 0; pp[i] >= 0; i++)
    अस्थायी_FAILURE_RETRY (बंद (pp[i]));
}

अटल अंतरभूत व्योम
cleanup_dirp (सूची **p)
{
  सूची *dir = *p;
  अगर (dir)
    बंद_सूची (dir);
}

अटल अंतरभूत पूर्णांक
close_and_reset (पूर्णांक *fd)
{
  पूर्णांक ret = 0;
  अगर (*fd >= 0)
    {
      ret = अस्थायी_FAILURE_RETRY (बंद (*fd));
      अगर (LIKELY (ret == 0))
        *fd = -1;
    }
  वापस ret;
}

अटल अंतरभूत अक्षर *
xstrdup (स्थिर अक्षर *str)
{
  अक्षर *ret;
  अगर (str == शून्य)
    वापस शून्य;

  ret = strdup (str);
  अगर (ret == शून्य)
    OOM ();

  वापस ret;
}

व्योम consume_trailing_slashes (अक्षर *path);

#घोषणा consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    जबतक (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

अटल अंतरभूत bool
path_is_slash_dev (स्थिर अक्षर *path)
{
  path = consume_slashes (path);

  अगर (म_भेदन (path, "dev", 3))
    वापस false;

  path += 3;

  /* Check there are only '/' left.  */
  क्रम (; *path; path++)
    अगर (*path != '/')
      वापस false;

  वापस true;
}

पूर्णांक xasprintf (अक्षर **str, स्थिर अक्षर *fmt, ...) __attribute__ ((format (म_लिखो, 2, 3)));

पूर्णांक crun_path_exists (स्थिर अक्षर *path, libcrun_error_t *err);

पूर्णांक write_file_at_with_flags (पूर्णांक dirfd, पूर्णांक flags, mode_t mode, स्थिर अक्षर *name, स्थिर व्योम *तथ्य, माप_प्रकार len, libcrun_error_t *err);

अटल अंतरभूत पूर्णांक
write_file (स्थिर अक्षर *name, स्थिर व्योम *तथ्य, माप_प्रकार len, libcrun_error_t *err)
{
  वापस write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, तथ्य, len, err);
}

अटल अंतरभूत पूर्णांक
write_file_at (पूर्णांक dirfd, स्थिर अक्षर *name, स्थिर व्योम *तथ्य, माप_प्रकार len, libcrun_error_t *err)
{
  वापस write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, तथ्य, len, err);
}

पूर्णांक crun_ensure_directory (स्थिर अक्षर *path, पूर्णांक mode, bool nofollow, libcrun_error_t *err);

पूर्णांक crun_ensure_directory_at (पूर्णांक dirfd, स्थिर अक्षर *path, पूर्णांक mode, bool nofollow, libcrun_error_t *err);

पूर्णांक crun_safe_create_and_open_ref_at (bool dir, पूर्णांक dirfd, स्थिर अक्षर *dirpath, स्थिर अक्षर *path, पूर्णांक mode, libcrun_error_t *err);

पूर्णांक crun_safe_ensure_directory_at (पूर्णांक dirfd, स्थिर अक्षर *dirpath, स्थिर अक्षर *path, पूर्णांक mode,
                                   libcrun_error_t *err);

पूर्णांक crun_safe_ensure_file_at (पूर्णांक dirfd, स्थिर अक्षर *dirpath, स्थिर अक्षर *path, पूर्णांक mode,
                              libcrun_error_t *err);

पूर्णांक crun_dir_p (स्थिर अक्षर *path, bool nofollow, libcrun_error_t *err);

पूर्णांक crun_dir_p_at (पूर्णांक dirfd, स्थिर अक्षर *path, bool nofollow, libcrun_error_t *err);

पूर्णांक detach_process ();

पूर्णांक create_file_if_missing_at (पूर्णांक dirfd, स्थिर अक्षर *file, mode_t mode, libcrun_error_t *err);

पूर्णांक check_running_in_user_namespace (libcrun_error_t *err);

पूर्णांक set_selinux_label (libcrun_container_t *container, स्थिर अक्षर *label, bool now, libcrun_error_t *err);

पूर्णांक add_selinux_mount_label (अक्षर **ret, स्थिर अक्षर *तथ्य, स्थिर अक्षर *label, स्थिर अक्षर *context_type, libcrun_error_t *err);

पूर्णांक set_apparmor_profile (libcrun_container_t *container, स्थिर अक्षर *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

पूर्णांक read_all_fd_with_आकार_hint (पूर्णांक fd, स्थिर अक्षर *description, अक्षर **out, माप_प्रकार *len, माप_प्रकार hint, libcrun_error_t *err);

पूर्णांक read_all_fd_with_max_आकार (पूर्णांक fd, स्थिर अक्षर *description, अक्षर **out, माप_प्रकार *len, माप_प्रकार max_आकार, libcrun_error_t *err);

अटल अंतरभूत पूर्णांक
read_all_fd (पूर्णांक fd, स्थिर अक्षर *description, अक्षर **out, माप_प्रकार *len, libcrun_error_t *err)
{
  वापस read_all_fd_with_आकार_hint (fd, description, out, len, 0, err);
}

पूर्णांक get_realpath_to_file (पूर्णांक dirfd, स्थिर अक्षर *path_name, अक्षर **absolute_path, libcrun_error_t *err);

पूर्णांक read_all_file (स्थिर अक्षर *path, अक्षर **out, माप_प्रकार *len, libcrun_error_t *err);

पूर्णांक read_all_file_at (पूर्णांक dirfd, स्थिर अक्षर *path, अक्षर **out, माप_प्रकार *len, libcrun_error_t *err);

पूर्णांक open_unix_domain_client_socket (स्थिर अक्षर *path, पूर्णांक dgram, libcrun_error_t *err);

पूर्णांक open_unix_domain_socket (स्थिर अक्षर *path, पूर्णांक dgram, libcrun_error_t *err);

पूर्णांक send_fd_to_socket (पूर्णांक server, पूर्णांक fd, libcrun_error_t *err);

पूर्णांक send_fd_to_socket_with_payload (पूर्णांक server, पूर्णांक fd, स्थिर अक्षर *payload, माप_प्रकार payload_len, libcrun_error_t *err);

पूर्णांक create_socket_pair (पूर्णांक *pair, libcrun_error_t *err);

पूर्णांक receive_fd_from_socket (पूर्णांक from, libcrun_error_t *err);

पूर्णांक receive_fd_from_socket_with_payload (पूर्णांक from, अक्षर *payload, माप_प्रकार payload_len, libcrun_error_t *err);

पूर्णांक create_signalfd (sigset_t *mask, libcrun_error_t *err);

पूर्णांक epoll_helper (पूर्णांक *in_fds, पूर्णांक *in_levelfds, पूर्णांक *out_fds, पूर्णांक *out_levelfds, libcrun_error_t *err);

पूर्णांक copy_from_fd_to_fd (पूर्णांक src, पूर्णांक dst, पूर्णांक consume, libcrun_error_t *err);

पूर्णांक run_process (अक्षर **args, libcrun_error_t *err);

पूर्णांक format_default_id_mapping (अक्षर **out, uid_t container_id, uid_t host_uid, uid_t host_id, पूर्णांक is_uid, libcrun_error_t *err);

पूर्णांक run_process_with_stdin_timeout_envp (अक्षर *path, अक्षर **args, स्थिर अक्षर *cwd, पूर्णांक timeout, अक्षर **envp,
                                         अक्षर *मानक_निवेश, माप_प्रकार stdin_len, पूर्णांक out_fd, पूर्णांक err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

पूर्णांक mark_or_close_fds_ge_than (libcrun_container_t *container, पूर्णांक n, bool close_now, libcrun_error_t *err);

व्योम get_current_timestamp (अक्षर *out, माप_प्रकार len);

पूर्णांक set_blocking_fd (पूर्णांक fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC पूर्णांक parse_json_file (json_object **out, स्थिर अक्षर *jsondata, काष्ठा parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
अटल अंतरभूत पूर्णांक
has_prefix (स्थिर अक्षर *str, स्थिर अक्षर *prefix)
{
  माप_प्रकार prefix_len;

  अगर (! str || ! prefix)
    वापस 0;

  prefix_len = म_माप (prefix);
  वापस म_माप (str) >= prefix_len && स_भेद (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
अटल अंतरभूत पूर्णांक
has_dir_prefix (स्थिर अक्षर *str, स्थिर अक्षर *prefix)
{
  माप_प्रकार prefix_len;

  अगर (! str || ! prefix)
    वापस 0;

  prefix_len = म_माप (prefix);
  वापस म_माप (str) > prefix_len && स_भेद (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

पूर्णांक find_executable (अक्षर **exec_path, स्थिर अक्षर *executable_path, स्थिर अक्षर *cwd, libcrun_error_t *err);

पूर्णांक copy_recursive_fd_to_fd (पूर्णांक srcfd, पूर्णांक destfd, स्थिर अक्षर *srcname, स्थिर अक्षर *destname, libcrun_error_t *err);

पूर्णांक set_home_env (uid_t uid);

पूर्णांक libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

पूर्णांक libcrun_initialize_apparmor (libcrun_error_t *err);

स्थिर अक्षर *find_annotation (libcrun_container_t *container, स्थिर अक्षर *name);

पूर्णांक get_file_type_at (पूर्णांक dirfd, mode_t *mode, bool nofollow, स्थिर अक्षर *path);

पूर्णांक get_file_type (mode_t *mode, bool nofollow, स्थिर अक्षर *path);

पूर्णांक get_file_type_fd (पूर्णांक fd, mode_t *mode);

अक्षर *get_user_name (uid_t uid);

पूर्णांक safe_openat (पूर्णांक dirfd, स्थिर अक्षर *rootfs, स्थिर अक्षर *path, पूर्णांक flags, पूर्णांक mode,
                 libcrun_error_t *err);

पूर्णांक safe_write (पूर्णांक fd, स्थिर अक्षर *fname, स्थिर व्योम *buf, माप_प्रकार गिनती, libcrun_error_t *err);

पूर्णांक append_paths (अक्षर **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

पूर्णांक str2sig (स्थिर अक्षर *name);

पूर्णांक base64_decode (स्थिर अक्षर *iptr, माप_प्रकार isize, अक्षर *optr, माप_प्रकार osize, माप_प्रकार *nbytes);
पूर्णांक has_suffix (स्थिर अक्षर *source, स्थिर अक्षर *suffix);
अक्षर *libcrun_sysctl_कुंजी_to_proc_path (स्थिर अक्षर *कुंजी);
अक्षर *str_join_array (पूर्णांक offset, माप_प्रकार आकार, अक्षर *स्थिर array[], स्थिर अक्षर *joint);

ssize_t safe_readlinkat (पूर्णांक dfd, स्थिर अक्षर *name, अक्षर **बफर, ssize_t hint, libcrun_error_t *err);

अक्षर **read_dir_entries (स्थिर अक्षर *path, libcrun_error_t *err);

अटल अंतरभूत bool
is_empty_string (स्थिर अक्षर *s)
{
  वापस s == शून्य || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
अटल अंतरभूत माप_प्रकार
safe_strlen (स्थिर अक्षर *s)
{
  वापस s == शून्य ? 0 : म_माप (s);
}

अटल अंतरभूत bool
path_has_dot_dot_component (स्थिर अक्षर *path)
{
  अगर (path == शून्य)
    वापस false;
  cleanup_free अक्षर *npath = xstrdup (path);
  अक्षर *cur, *it;

  cur = npath;
  it = म_अक्षर (cur, '/');
  जबतक (cur)
    {
      अगर (it)
        *it = '\0';

      अगर (म_भेद (cur, "..") == 0)
        वापस true;

      अगर (it == शून्य)
        अवरोध;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = म_अक्षर (cur, '/');
    }
  वापस false;
}

अटल अंतरभूत पूर्णांक
waitpid_ignore_stopped (pid_t pid, पूर्णांक *status, पूर्णांक options)
{
  पूर्णांक ret, s;
  करो
    {
      s = 0;
      ret = अस्थायी_FAILURE_RETRY (waitpid (pid, &s, options));
      अगर (ret < 0)
        वापस ret;
  } जबतक (WIFSTOPPED (s) || WIFCONTINUED (s));

  अगर (status)
    *status = s;

  वापस ret;
}

अटल अंतरभूत पूर्णांक
get_process_exit_status (पूर्णांक status)
{
  अगर (WIFEXITED (status))
    वापस WEXITSTATUS (status);
  अगर (WIFSIGNALED (status))
    वापस 128 + WTERMSIG (status);

  वापस -1;
}

uid_t get_overflow_uid (व्योम);
gid_t get_overflow_gid (व्योम);

/* Adapted from systemd.  Include space for the NUL byte.  */
#घोषणा DECIMAL_STR_MAX(type)                                        \
  ((माप_प्रकार) 2U + (माप (type) <= 1 ? 3U : माप (type) <= 2 ? 5U  \
                                        : माप (type) <= 4   ? 10U \
                                        : माप (type) <= 8   ? 20U \
                                                               : माप (पूर्णांक[-2 * (माप (type) > 8)])))

#घोषणा _STRLEN(s) (माप (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#घोषणा PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (पूर्णांक))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
प्रकार अक्षर proc_fd_path_t[PROC_PID_FD_STRLEN];

#अघोषित _STRLEN

अटल अंतरभूत व्योम
get_proc_fd_path (proc_fd_path_t path, pid_t pid, पूर्णांक fd)
{
  स्थिर माप_प्रकार max_len = माप (proc_fd_path_t);
  माप_प्रकार n;

  अगर (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  अन्यथा
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  अगर (UNLIKELY (n >= max_len))
    पात ();
}

अटल अंतरभूत व्योम
get_proc_self_fd_path (proc_fd_path_t path, पूर्णांक fd)
{
  get_proc_fd_path (path, 0, fd);
}

अटल अंतरभूत व्योम
get_self_fd_path (proc_fd_path_t path, पूर्णांक fd)
{
  snprintf (path, माप (proc_fd_path_t), "self/fd/%d", fd);
}

अटल अंतरभूत पूर्णांक
validate_options (अचिन्हित पूर्णांक specified_options, अचिन्हित पूर्णांक supported_options, libcrun_error_t *err)
{
  अगर (! ! (~supported_options & specified_options))
    वापस crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  वापस 0;
}

बाह्य पूर्णांक cpuset_string_to_bitmask (स्थिर अक्षर *str, अक्षर **out, माप_प्रकार *out_आकार, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
काष्ठा channel_fd_pair;

काष्ठा channel_fd_pair *channel_fd_pair_new (पूर्णांक in_fd, पूर्णांक out_fd, माप_प्रकार आकार);

व्योम channel_fd_pair_free (काष्ठा channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
पूर्णांक channel_fd_pair_process (काष्ठा channel_fd_pair *channel, पूर्णांक epollfd, libcrun_error_t *err);

अटल अंतरभूत व्योम
cleanup_channel_fd_pairp (व्योम *p)
{
  काष्ठा channel_fd_pair **pp = (काष्ठा channel_fd_pair **) p;
  अगर (*pp == शून्य)
    वापस;

  channel_fd_pair_free (*pp);
}
#घोषणा cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

पूर्णांक libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
पूर्णांक libcrun_open_proc_file (libcrun_container_t *container, स्थिर अक्षर *path, पूर्णांक flags, libcrun_error_t *err);
पूर्णांक libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, स्थिर अक्षर *path, पूर्णांक flags, libcrun_error_t *err);

#पूर्ण_अगर
