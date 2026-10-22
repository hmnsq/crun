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
      long पूर्णाङ्क __result;                      \
      कुरु                                      \
        __result = (long पूर्णाङ्क) (expression);   \
      यावत् (__result < 0 && errno == EINTR); \
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

__attribute__ ((malloc)) स्थिर अन्तःस्थ शून्य *
xmalloc (size_t size)
{
  शून्य *res = malloc (size);
  यदि (UNLIKELY (res == रिक्त))
    OOM ();
  प्रत्यागम res;
}

__attribute__ ((malloc)) स्थिर अन्तःस्थ शून्य *
xmalloc0 (size_t size)
{
  शून्य *res = calloc (1, size);
  यदि (UNLIKELY (res == रिक्त))
    OOM ();
  प्रत्यागम res;
}

__attribute__ ((malloc)) स्थिर अन्तःस्थ शून्य *
xrealloc (शून्य *ptr, size_t size)
{
  शून्य *res = realloc (ptr, size);
  यदि (UNLIKELY (res == रिक्त))
    OOM ();
  प्रत्यागम res;
}

स्थिर अन्तःस्थ शून्य
cleanup_freep (शून्य *p)
{
  शून्य **pp = (शून्य **) p;
  मुक्त (*pp);
}

स्थिर अन्तःस्थ शून्य
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  यदि (file)
    (शून्य) fclose (file);
}

स्थिर अन्तःस्थ शून्य
cleanup_closep (शून्य *p)
{
  पूर्णाङ्क *pp = (पूर्णाङ्क *) p;
  यदि (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

स्थिर अन्तःस्थ शून्य
cleanup_pidp (शून्य *p)
{
  pid_t *pp = (pid_t *) p;
  यदि (*pp > 0)
    {
      TEMP_FAILURE_RETRY (kill (*pp, SIGKILL));
      TEMP_FAILURE_RETRY (waitpid (*pp, रिक्त, 0));
    }
}

संरचना libcrun_mmap_s
{
  शून्य *addr;
  size_t length;
};

पूर्णाङ्क libcrun_mmap (संरचना libcrun_mmap_s **ret, शून्य *addr, size_t length,
                  पूर्णाङ्क prot, पूर्णाङ्क flags, पूर्णाङ्क fd, off_t offset,
                  libcrun_error_t *err);

पूर्णाङ्क libcrun_munmap (संरचना libcrun_mmap_s *mmap, libcrun_error_t *err);

स्थिर अन्तःस्थ शून्य
cleanup_mmapp (शून्य *p)
{
  पूर्णाङ्क ret;
  libcrun_error_t tmp_err = रिक्त;
  संरचना libcrun_mmap_s **mm;

  mm = (संरचना libcrun_mmap_s **) p;
  यदि (*mm == रिक्त)
    प्रत्यागम;

  ret = libcrun_munmap (*mm, &tmp_err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

संरचना libcrun_fd_map
{
  size_t nfds;
  पूर्णाङ्क fds[];
};

स्थिर अन्तःस्थ संरचना libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  संरचना libcrun_fd_map *ret;
  size_t i;

  ret = (संरचना libcrun_fd_map *) xmalloc (परिमाण (*ret) + परिमाण (पूर्णाङ्क) * len);
  ret->nfds = len;
  प्रति (i = 0; i < len; i++)
    ret->fds[i] = -1;

  प्रत्यागम ret;
}

स्थिर अन्तःस्थ शून्य
cleanup_close_mapp (संरचना libcrun_fd_map **p)
{
  संरचना libcrun_fd_map *m = *p;
  size_t i;

  यदि (m == रिक्त)
    प्रत्यागम;

  प्रति (i = 0; i < m->nfds; i++)
    यदि (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  मुक्त (m);
}

स्थिर अन्तःस्थ शून्य
cleanup_close_vecp (पूर्णाङ्क **p)
{
  पूर्णाङ्क *pp = *p;
  पूर्णाङ्क i;

  यदि (pp == रिक्त)
    प्रत्यागम;

  प्रति (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

स्थिर अन्तःस्थ शून्य
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  यदि (dir)
    closedir (dir);
}

स्थिर अन्तःस्थ पूर्णाङ्क
close_and_reset (पूर्णाङ्क *fd)
{
  पूर्णाङ्क ret = 0;
  यदि (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      यदि (LIKELY (ret == 0))
        *fd = -1;
    }
  प्रत्यागम ret;
}

स्थिर अन्तःस्थ वर्ण *
xstrdup (ध्रुव वर्ण *str)
{
  वर्ण *ret;
  यदि (str == रिक्त)
    प्रत्यागम रिक्त;

  ret = strdup (str);
  यदि (ret == रिक्त)
    OOM ();

  प्रत्यागम ret;
}

शून्य consume_trailing_slashes (वर्ण *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    यावत् (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

स्थिर अन्तःस्थ bool
path_is_slash_dev (ध्रुव वर्ण *path)
{
  path = consume_slashes (path);

  यदि (strncmp (path, "dev", 3))
    प्रत्यागम false;

  path += 3;

  /* Check there are only '/' left.  */
  प्रति (; *path; path++)
    यदि (*path != '/')
      प्रत्यागम false;

  प्रत्यागम true;
}

पूर्णाङ्क xasprintf (वर्ण **str, ध्रुव वर्ण *fmt, ...) __attribute__ ((format (printf, 2, 3)));

पूर्णाङ्क crun_path_exists (ध्रुव वर्ण *path, libcrun_error_t *err);

पूर्णाङ्क write_file_at_with_flags (पूर्णाङ्क dirfd, पूर्णाङ्क flags, mode_t mode, ध्रुव वर्ण *name, ध्रुव शून्य *data, size_t len, libcrun_error_t *err);

स्थिर अन्तःस्थ पूर्णाङ्क
write_file (ध्रुव वर्ण *name, ध्रुव शून्य *data, size_t len, libcrun_error_t *err)
{
  प्रत्यागम write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
write_file_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *name, ध्रुव शून्य *data, size_t len, libcrun_error_t *err)
{
  प्रत्यागम write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

पूर्णाङ्क crun_ensure_directory (ध्रुव वर्ण *path, पूर्णाङ्क mode, bool nofollow, libcrun_error_t *err);

पूर्णाङ्क crun_ensure_directory_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *path, पूर्णाङ्क mode, bool nofollow, libcrun_error_t *err);

पूर्णाङ्क crun_safe_create_and_open_ref_at (bool dir, पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath, ध्रुव वर्ण *path, पूर्णाङ्क mode, libcrun_error_t *err);

पूर्णाङ्क crun_safe_ensure_directory_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath, ध्रुव वर्ण *path, पूर्णाङ्क mode,
                                   libcrun_error_t *err);

पूर्णाङ्क crun_safe_ensure_file_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *dirpath, ध्रुव वर्ण *path, पूर्णाङ्क mode,
                              libcrun_error_t *err);

पूर्णाङ्क crun_dir_p (ध्रुव वर्ण *path, bool nofollow, libcrun_error_t *err);

पूर्णाङ्क crun_dir_p_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *path, bool nofollow, libcrun_error_t *err);

पूर्णाङ्क detach_process ();

पूर्णाङ्क create_file_if_missing_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *file, mode_t mode, libcrun_error_t *err);

पूर्णाङ्क check_running_in_user_namespace (libcrun_error_t *err);

पूर्णाङ्क set_selinux_label (libcrun_container_t *container, ध्रुव वर्ण *label, bool now, libcrun_error_t *err);

पूर्णाङ्क add_selinux_mount_label (वर्ण **ret, ध्रुव वर्ण *data, ध्रुव वर्ण *label, ध्रुव वर्ण *context_type, libcrun_error_t *err);

पूर्णाङ्क set_apparmor_profile (libcrun_container_t *container, ध्रुव वर्ण *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

पूर्णाङ्क read_all_fd_with_size_hint (पूर्णाङ्क fd, ध्रुव वर्ण *description, वर्ण **out, size_t *len, size_t hint, libcrun_error_t *err);

पूर्णाङ्क read_all_fd_with_max_size (पूर्णाङ्क fd, ध्रुव वर्ण *description, वर्ण **out, size_t *len, size_t max_size, libcrun_error_t *err);

स्थिर अन्तःस्थ पूर्णाङ्क
read_all_fd (पूर्णाङ्क fd, ध्रुव वर्ण *description, वर्ण **out, size_t *len, libcrun_error_t *err)
{
  प्रत्यागम read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

पूर्णाङ्क get_realpath_to_file (पूर्णाङ्क dirfd, ध्रुव वर्ण *path_name, वर्ण **absolute_path, libcrun_error_t *err);

पूर्णाङ्क read_all_file (ध्रुव वर्ण *path, वर्ण **out, size_t *len, libcrun_error_t *err);

पूर्णाङ्क read_all_file_at (पूर्णाङ्क dirfd, ध्रुव वर्ण *path, वर्ण **out, size_t *len, libcrun_error_t *err);

पूर्णाङ्क open_unix_domain_client_socket (ध्रुव वर्ण *path, पूर्णाङ्क dgram, libcrun_error_t *err);

पूर्णाङ्क open_unix_domain_socket (ध्रुव वर्ण *path, पूर्णाङ्क dgram, libcrun_error_t *err);

पूर्णाङ्क send_fd_to_socket (पूर्णाङ्क server, पूर्णाङ्क fd, libcrun_error_t *err);

पूर्णाङ्क send_fd_to_socket_with_payload (पूर्णाङ्क server, पूर्णाङ्क fd, ध्रुव वर्ण *payload, size_t payload_len, libcrun_error_t *err);

पूर्णाङ्क create_socket_pair (पूर्णाङ्क *pair, libcrun_error_t *err);

पूर्णाङ्क receive_fd_from_socket (पूर्णाङ्क from, libcrun_error_t *err);

पूर्णाङ्क receive_fd_from_socket_with_payload (पूर्णाङ्क from, वर्ण *payload, size_t payload_len, libcrun_error_t *err);

पूर्णाङ्क create_signalfd (sigset_t *mask, libcrun_error_t *err);

पूर्णाङ्क epoll_helper (पूर्णाङ्क *in_fds, पूर्णाङ्क *in_levelfds, पूर्णाङ्क *out_fds, पूर्णाङ्क *out_levelfds, libcrun_error_t *err);

पूर्णाङ्क copy_from_fd_to_fd (पूर्णाङ्क src, पूर्णाङ्क dst, पूर्णाङ्क consume, libcrun_error_t *err);

पूर्णाङ्क run_process (वर्ण **args, libcrun_error_t *err);

पूर्णाङ्क format_default_id_mapping (वर्ण **out, uid_t container_id, uid_t host_uid, uid_t host_id, पूर्णाङ्क is_uid, libcrun_error_t *err);

पूर्णाङ्क run_process_with_stdin_timeout_envp (वर्ण *path, वर्ण **args, ध्रुव वर्ण *cwd, पूर्णाङ्क timeout, वर्ण **envp,
                                         वर्ण *stdin, size_t stdin_len, पूर्णाङ्क out_fd, पूर्णाङ्क err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

पूर्णाङ्क mark_or_close_fds_ge_than (libcrun_container_t *container, पूर्णाङ्क n, bool close_now, libcrun_error_t *err);

शून्य get_current_timestamp (वर्ण *out, size_t len);

पूर्णाङ्क set_blocking_fd (पूर्णाङ्क fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC पूर्णाङ्क parse_json_file (json_object **out, ध्रुव वर्ण *jsondata, संरचना parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
स्थिर अन्तःस्थ पूर्णाङ्क
has_prefix (ध्रुव वर्ण *str, ध्रुव वर्ण *prefix)
{
  size_t prefix_len;

  यदि (! str || ! prefix)
    प्रत्यागम 0;

  prefix_len = strlen (prefix);
  प्रत्यागम strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
स्थिर अन्तःस्थ पूर्णाङ्क
has_dir_prefix (ध्रुव वर्ण *str, ध्रुव वर्ण *prefix)
{
  size_t prefix_len;

  यदि (! str || ! prefix)
    प्रत्यागम 0;

  prefix_len = strlen (prefix);
  प्रत्यागम strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

पूर्णाङ्क find_executable (वर्ण **exec_path, ध्रुव वर्ण *executable_path, ध्रुव वर्ण *cwd, libcrun_error_t *err);

पूर्णाङ्क copy_recursive_fd_to_fd (पूर्णाङ्क srcfd, पूर्णाङ्क destfd, ध्रुव वर्ण *srcname, ध्रुव वर्ण *destname, libcrun_error_t *err);

पूर्णाङ्क set_home_env (uid_t uid);

पूर्णाङ्क libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

पूर्णाङ्क libcrun_initialize_apparmor (libcrun_error_t *err);

ध्रुव वर्ण *find_annotation (libcrun_container_t *container, ध्रुव वर्ण *name);

पूर्णाङ्क get_file_type_at (पूर्णाङ्क dirfd, mode_t *mode, bool nofollow, ध्रुव वर्ण *path);

पूर्णाङ्क get_file_type (mode_t *mode, bool nofollow, ध्रुव वर्ण *path);

पूर्णाङ्क get_file_type_fd (पूर्णाङ्क fd, mode_t *mode);

वर्ण *get_user_name (uid_t uid);

पूर्णाङ्क safe_openat (पूर्णाङ्क dirfd, ध्रुव वर्ण *rootfs, ध्रुव वर्ण *path, पूर्णाङ्क flags, पूर्णाङ्क mode,
                 libcrun_error_t *err);

पूर्णाङ्क safe_write (पूर्णाङ्क fd, ध्रुव वर्ण *fname, ध्रुव शून्य *buf, size_t गिनती, libcrun_error_t *err);

पूर्णाङ्क append_paths (वर्ण **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

पूर्णाङ्क str2sig (ध्रुव वर्ण *name);

पूर्णाङ्क base64_decode (ध्रुव वर्ण *iptr, size_t isize, वर्ण *optr, size_t osize, size_t *nbytes);
पूर्णाङ्क has_suffix (ध्रुव वर्ण *source, ध्रुव वर्ण *suffix);
वर्ण *libcrun_sysctl_कुंजी_to_proc_path (ध्रुव वर्ण *कुंजी);
वर्ण *str_join_array (पूर्णाङ्क offset, size_t size, वर्ण *ध्रुव array[], ध्रुव वर्ण *joint);

ssize_t safe_readlinkat (पूर्णाङ्क dfd, ध्रुव वर्ण *name, वर्ण **buffer, ssize_t hint, libcrun_error_t *err);

वर्ण **read_dir_entries (ध्रुव वर्ण *path, libcrun_error_t *err);

स्थिर अन्तःस्थ bool
is_empty_string (ध्रुव वर्ण *s)
{
  प्रत्यागम s == रिक्त || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
स्थिर अन्तःस्थ size_t
safe_strlen (ध्रुव वर्ण *s)
{
  प्रत्यागम s == रिक्त ? 0 : strlen (s);
}

स्थिर अन्तःस्थ bool
path_has_dot_dot_component (ध्रुव वर्ण *path)
{
  यदि (path == रिक्त)
    प्रत्यागम false;
  cleanup_free वर्ण *npath = xstrdup (path);
  वर्ण *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  यावत् (cur)
    {
      यदि (it)
        *it = '\0';

      यदि (strcmp (cur, "..") == 0)
        प्रत्यागम true;

      यदि (it == रिक्त)
        भङ्ग;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  प्रत्यागम false;
}

स्थिर अन्तःस्थ पूर्णाङ्क
waitpid_ignore_stopped (pid_t pid, पूर्णाङ्क *status, पूर्णाङ्क options)
{
  पूर्णाङ्क ret, s;
  कुरु
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      यदि (ret < 0)
        प्रत्यागम ret;
  } यावत् (WIFSTOPPED (s) || WIFCONTINUED (s));

  यदि (status)
    *status = s;

  प्रत्यागम ret;
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_process_exit_status (पूर्णाङ्क status)
{
  यदि (WIFEXITED (status))
    प्रत्यागम WEXITSTATUS (status);
  यदि (WIFSIGNALED (status))
    प्रत्यागम 128 + WTERMSIG (status);

  प्रत्यागम -1;
}

uid_t get_overflow_uid (शून्य);
gid_t get_overflow_gid (शून्य);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (परिमाण (type) <= 1 ? 3U : परिमाण (type) <= 2 ? 5U  \
                                        : परिमाण (type) <= 4   ? 10U \
                                        : परिमाण (type) <= 8   ? 20U \
                                                               : परिमाण (पूर्णाङ्क[-2 * (परिमाण (type) > 8)])))

#define _STRLEN(s) (परिमाण (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (पूर्णाङ्क))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
प्रकारनिर्देश वर्ण proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

स्थिर अन्तःस्थ शून्य
get_proc_fd_path (proc_fd_path_t path, pid_t pid, पूर्णाङ्क fd)
{
  ध्रुव size_t max_len = परिमाण (proc_fd_path_t);
  size_t n;

  यदि (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  अन्यथा
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  यदि (UNLIKELY (n >= max_len))
    abort ();
}

स्थिर अन्तःस्थ शून्य
get_proc_self_fd_path (proc_fd_path_t path, पूर्णाङ्क fd)
{
  get_proc_fd_path (path, 0, fd);
}

स्थिर अन्तःस्थ शून्य
get_self_fd_path (proc_fd_path_t path, पूर्णाङ्क fd)
{
  snprintf (path, परिमाण (proc_fd_path_t), "self/fd/%d", fd);
}

स्थिर अन्तःस्थ पूर्णाङ्क
validate_options (unsigned पूर्णाङ्क specified_options, unsigned पूर्णाङ्क supported_options, libcrun_error_t *err)
{
  यदि (! ! (~supported_options & specified_options))
    प्रत्यागम crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  प्रत्यागम 0;
}

बाह्य पूर्णाङ्क cpuset_string_to_bitmask (ध्रुव वर्ण *str, वर्ण **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
संरचना channel_fd_pair;

संरचना channel_fd_pair *channel_fd_pair_new (पूर्णाङ्क in_fd, पूर्णाङ्क out_fd, size_t size);

शून्य channel_fd_pair_free (संरचना channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
पूर्णाङ्क channel_fd_pair_process (संरचना channel_fd_pair *channel, पूर्णाङ्क epollfd, libcrun_error_t *err);

स्थिर अन्तःस्थ शून्य
cleanup_channel_fd_pairp (शून्य *p)
{
  संरचना channel_fd_pair **pp = (संरचना channel_fd_pair **) p;
  यदि (*pp == रिक्त)
    प्रत्यागम;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

पूर्णाङ्क libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
पूर्णाङ्क libcrun_open_proc_file (libcrun_container_t *container, ध्रुव वर्ण *path, पूर्णाङ्क flags, libcrun_error_t *err);
पूर्णाङ्क libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ध्रुव वर्ण *path, पूर्णाङ्क flags, libcrun_error_t *err);

#endif
