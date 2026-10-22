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

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
#شامل <errno.h>
#شامل <argp.h>
#شامل "error.h"
#شامل <dirent.h>
#شامل <unistd.h>
#شامل <signal.h>
#شامل <fcntl.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل <sys/wait.h>
#شامل "container.h"

#ifndef TEMP_FAILURE_RETRY
#  define TEMP_FAILURE_RETRY(expression)      \
    (__extension__ ({                         \
      long صحیح __result;                      \
      انجامبده                                      \
        __result = (long صحیح) (expression);   \
      وقتی (__result < 0 && errno == EINTR); \
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

__attribute__ ((malloc)) ایستا درونخطی تهی *
xmalloc (size_t size)
{
  تهی *res = malloc (size);
  اگر (UNLIKELY (res == پوچ))
    OOM ();
  بازگشت res;
}

__attribute__ ((malloc)) ایستا درونخطی تهی *
xmalloc0 (size_t size)
{
  تهی *res = calloc (1, size);
  اگر (UNLIKELY (res == پوچ))
    OOM ();
  بازگشت res;
}

__attribute__ ((malloc)) ایستا درونخطی تهی *
xrealloc (تهی *ptr, size_t size)
{
  تهی *res = realloc (ptr, size);
  اگر (UNLIKELY (res == پوچ))
    OOM ();
  بازگشت res;
}

ایستا درونخطی تهی
cleanup_freep (تهی *p)
{
  تهی **pp = (تهی **) p;
  آزاد (*pp);
}

ایستا درونخطی تهی
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  اگر (file)
    (تهی) fclose (file);
}

ایستا درونخطی تهی
cleanup_closep (تهی *p)
{
  صحیح *pp = (صحیح *) p;
  اگر (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

ایستا درونخطی تهی
cleanup_pidp (تهی *p)
{
  pid_t *pp = (pid_t *) p;
  اگر (*pp > 0)
    {
      TEMP_FAILURE_RETRY (kill (*pp, SIGKILL));
      TEMP_FAILURE_RETRY (waitpid (*pp, پوچ, 0));
    }
}

ساختار libcrun_mmap_s
{
  تهی *addr;
  size_t length;
};

صحیح libcrun_mmap (ساختار libcrun_mmap_s **ret, تهی *addr, size_t length,
                  صحیح prot, صحیح flags, صحیح fd, off_t offset,
                  libcrun_error_t *err);

صحیح libcrun_munmap (ساختار libcrun_mmap_s *mmap, libcrun_error_t *err);

ایستا درونخطی تهی
cleanup_mmapp (تهی *p)
{
  صحیح ret;
  libcrun_error_t tmp_err = پوچ;
  ساختار libcrun_mmap_s **mm;

  mm = (ساختار libcrun_mmap_s **) p;
  اگر (*mm == پوچ)
    بازگشت;

  ret = libcrun_munmap (*mm, &tmp_err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

ساختار libcrun_fd_map
{
  size_t nfds;
  صحیح fds[];
};

ایستا درونخطی ساختار libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  ساختار libcrun_fd_map *ret;
  size_t i;

  ret = (ساختار libcrun_fd_map *) xmalloc (اندازه (*ret) + اندازه (صحیح) * len);
  ret->nfds = len;
  برای (i = 0; i < len; i++)
    ret->fds[i] = -1;

  بازگشت ret;
}

ایستا درونخطی تهی
cleanup_close_mapp (ساختار libcrun_fd_map **p)
{
  ساختار libcrun_fd_map *m = *p;
  size_t i;

  اگر (m == پوچ)
    بازگشت;

  برای (i = 0; i < m->nfds; i++)
    اگر (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  آزاد (m);
}

ایستا درونخطی تهی
cleanup_close_vecp (صحیح **p)
{
  صحیح *pp = *p;
  صحیح i;

  اگر (pp == پوچ)
    بازگشت;

  برای (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

ایستا درونخطی تهی
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  اگر (dir)
    closedir (dir);
}

ایستا درونخطی صحیح
close_and_reset (صحیح *fd)
{
  صحیح ret = 0;
  اگر (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      اگر (LIKELY (ret == 0))
        *fd = -1;
    }
  بازگشت ret;
}

ایستا درونخطی نویسه *
xstrdup (ثابت نویسه *str)
{
  نویسه *ret;
  اگر (str == پوچ)
    بازگشت پوچ;

  ret = strdup (str);
  اگر (ret == پوچ)
    OOM ();

  بازگشت ret;
}

تهی consume_trailing_slashes (نویسه *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    وقتی (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

ایستا درونخطی bool
path_is_slash_dev (ثابت نویسه *path)
{
  path = consume_slashes (path);

  اگر (strncmp (path, "dev", 3))
    بازگشت false;

  path += 3;

  /* Check there are only '/' left.  */
  برای (; *path; path++)
    اگر (*path != '/')
      بازگشت false;

  بازگشت true;
}

صحیح xasprintf (نویسه **str, ثابت نویسه *fmt, ...) __attribute__ ((format (printf, 2, 3)));

صحیح crun_path_exists (ثابت نویسه *path, libcrun_error_t *err);

صحیح write_file_at_with_flags (صحیح dirfd, صحیح flags, mode_t mode, ثابت نویسه *name, ثابت تهی *data, size_t len, libcrun_error_t *err);

ایستا درونخطی صحیح
write_file (ثابت نویسه *name, ثابت تهی *data, size_t len, libcrun_error_t *err)
{
  بازگشت write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

ایستا درونخطی صحیح
write_file_at (صحیح dirfd, ثابت نویسه *name, ثابت تهی *data, size_t len, libcrun_error_t *err)
{
  بازگشت write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

صحیح crun_ensure_directory (ثابت نویسه *path, صحیح mode, bool nofollow, libcrun_error_t *err);

صحیح crun_ensure_directory_at (صحیح dirfd, ثابت نویسه *path, صحیح mode, bool nofollow, libcrun_error_t *err);

صحیح crun_safe_create_and_open_ref_at (bool dir, صحیح dirfd, ثابت نویسه *dirpath, ثابت نویسه *path, صحیح mode, libcrun_error_t *err);

صحیح crun_safe_ensure_directory_at (صحیح dirfd, ثابت نویسه *dirpath, ثابت نویسه *path, صحیح mode,
                                   libcrun_error_t *err);

صحیح crun_safe_ensure_file_at (صحیح dirfd, ثابت نویسه *dirpath, ثابت نویسه *path, صحیح mode,
                              libcrun_error_t *err);

صحیح crun_dir_p (ثابت نویسه *path, bool nofollow, libcrun_error_t *err);

صحیح crun_dir_p_at (صحیح dirfd, ثابت نویسه *path, bool nofollow, libcrun_error_t *err);

صحیح detach_process ();

صحیح create_file_if_missing_at (صحیح dirfd, ثابت نویسه *file, mode_t mode, libcrun_error_t *err);

صحیح check_running_in_user_namespace (libcrun_error_t *err);

صحیح set_selinux_label (libcrun_container_t *container, ثابت نویسه *label, bool now, libcrun_error_t *err);

صحیح add_selinux_mount_label (نویسه **ret, ثابت نویسه *data, ثابت نویسه *label, ثابت نویسه *context_type, libcrun_error_t *err);

صحیح set_apparmor_profile (libcrun_container_t *container, ثابت نویسه *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

صحیح read_all_fd_with_size_hint (صحیح fd, ثابت نویسه *description, نویسه **out, size_t *len, size_t hint, libcrun_error_t *err);

صحیح read_all_fd_with_max_size (صحیح fd, ثابت نویسه *description, نویسه **out, size_t *len, size_t max_size, libcrun_error_t *err);

ایستا درونخطی صحیح
read_all_fd (صحیح fd, ثابت نویسه *description, نویسه **out, size_t *len, libcrun_error_t *err)
{
  بازگشت read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

صحیح get_realpath_to_file (صحیح dirfd, ثابت نویسه *path_name, نویسه **absolute_path, libcrun_error_t *err);

صحیح read_all_file (ثابت نویسه *path, نویسه **out, size_t *len, libcrun_error_t *err);

صحیح read_all_file_at (صحیح dirfd, ثابت نویسه *path, نویسه **out, size_t *len, libcrun_error_t *err);

صحیح open_unix_domain_client_socket (ثابت نویسه *path, صحیح dgram, libcrun_error_t *err);

صحیح open_unix_domain_socket (ثابت نویسه *path, صحیح dgram, libcrun_error_t *err);

صحیح send_fd_to_socket (صحیح server, صحیح fd, libcrun_error_t *err);

صحیح send_fd_to_socket_with_payload (صحیح server, صحیح fd, ثابت نویسه *payload, size_t payload_len, libcrun_error_t *err);

صحیح create_socket_pair (صحیح *pair, libcrun_error_t *err);

صحیح receive_fd_from_socket (صحیح from, libcrun_error_t *err);

صحیح receive_fd_from_socket_with_payload (صحیح from, نویسه *payload, size_t payload_len, libcrun_error_t *err);

صحیح create_signalfd (sigset_t *mask, libcrun_error_t *err);

صحیح epoll_helper (صحیح *in_fds, صحیح *in_levelfds, صحیح *out_fds, صحیح *out_levelfds, libcrun_error_t *err);

صحیح copy_from_fd_to_fd (صحیح src, صحیح dst, صحیح consume, libcrun_error_t *err);

صحیح run_process (نویسه **args, libcrun_error_t *err);

صحیح format_default_id_mapping (نویسه **out, uid_t container_id, uid_t host_uid, uid_t host_id, صحیح is_uid, libcrun_error_t *err);

صحیح run_process_with_stdin_timeout_envp (نویسه *path, نویسه **args, ثابت نویسه *cwd, صحیح timeout, نویسه **envp,
                                         نویسه *stdin, size_t stdin_len, صحیح out_fd, صحیح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

صحیح mark_or_close_fds_ge_than (libcrun_container_t *container, صحیح n, bool close_now, libcrun_error_t *err);

تهی get_current_timestamp (نویسه *out, size_t len);

صحیح set_blocking_fd (صحیح fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC صحیح parse_json_file (json_object **out, ثابت نویسه *jsondata, ساختار parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
ایستا درونخطی صحیح
has_prefix (ثابت نویسه *str, ثابت نویسه *prefix)
{
  size_t prefix_len;

  اگر (! str || ! prefix)
    بازگشت 0;

  prefix_len = strlen (prefix);
  بازگشت strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
ایستا درونخطی صحیح
has_dir_prefix (ثابت نویسه *str, ثابت نویسه *prefix)
{
  size_t prefix_len;

  اگر (! str || ! prefix)
    بازگشت 0;

  prefix_len = strlen (prefix);
  بازگشت strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

صحیح find_executable (نویسه **exec_path, ثابت نویسه *executable_path, ثابت نویسه *cwd, libcrun_error_t *err);

صحیح copy_recursive_fd_to_fd (صحیح srcfd, صحیح destfd, ثابت نویسه *srcname, ثابت نویسه *destname, libcrun_error_t *err);

صحیح set_home_env (uid_t uid);

صحیح libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

صحیح libcrun_initialize_apparmor (libcrun_error_t *err);

ثابت نویسه *find_annotation (libcrun_container_t *container, ثابت نویسه *name);

صحیح get_file_type_at (صحیح dirfd, mode_t *mode, bool nofollow, ثابت نویسه *path);

صحیح get_file_type (mode_t *mode, bool nofollow, ثابت نویسه *path);

صحیح get_file_type_fd (صحیح fd, mode_t *mode);

نویسه *get_user_name (uid_t uid);

صحیح safe_openat (صحیح dirfd, ثابت نویسه *rootfs, ثابت نویسه *path, صحیح flags, صحیح mode,
                 libcrun_error_t *err);

صحیح safe_write (صحیح fd, ثابت نویسه *fname, ثابت تهی *buf, size_t شمار, libcrun_error_t *err);

صحیح append_paths (نویسه **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

صحیح str2sig (ثابت نویسه *name);

صحیح base64_decode (ثابت نویسه *iptr, size_t isize, نویسه *optr, size_t osize, size_t *nbytes);
صحیح has_suffix (ثابت نویسه *source, ثابت نویسه *suffix);
نویسه *libcrun_sysctl_کلید_to_proc_path (ثابت نویسه *کلید);
نویسه *str_join_array (صحیح offset, size_t size, نویسه *ثابت array[], ثابت نویسه *joint);

ssize_t safe_readlinkat (صحیح dfd, ثابت نویسه *name, نویسه **buffer, ssize_t hint, libcrun_error_t *err);

نویسه **read_dir_entries (ثابت نویسه *path, libcrun_error_t *err);

ایستا درونخطی bool
is_empty_string (ثابت نویسه *s)
{
  بازگشت s == پوچ || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
ایستا درونخطی size_t
safe_strlen (ثابت نویسه *s)
{
  بازگشت s == پوچ ? 0 : strlen (s);
}

ایستا درونخطی bool
path_has_dot_dot_component (ثابت نویسه *path)
{
  اگر (path == پوچ)
    بازگشت false;
  cleanup_free نویسه *npath = xstrdup (path);
  نویسه *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  وقتی (cur)
    {
      اگر (it)
        *it = '\0';

      اگر (strcmp (cur, "..") == 0)
        بازگشت true;

      اگر (it == پوچ)
        بشکن;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  بازگشت false;
}

ایستا درونخطی صحیح
waitpid_ignore_stopped (pid_t pid, صحیح *status, صحیح options)
{
  صحیح ret, s;
  انجامبده
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      اگر (ret < 0)
        بازگشت ret;
  } وقتی (WIFSTOPPED (s) || WIFCONTINUED (s));

  اگر (status)
    *status = s;

  بازگشت ret;
}

ایستا درونخطی صحیح
get_process_exit_status (صحیح status)
{
  اگر (WIFEXITED (status))
    بازگشت WEXITSTATUS (status);
  اگر (WIFSIGNALED (status))
    بازگشت 128 + WTERMSIG (status);

  بازگشت -1;
}

uid_t get_overflow_uid (تهی);
gid_t get_overflow_gid (تهی);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (اندازه (type) <= 1 ? 3U : اندازه (type) <= 2 ? 5U  \
                                        : اندازه (type) <= 4   ? 10U \
                                        : اندازه (type) <= 8   ? 20U \
                                                               : اندازه (صحیح[-2 * (اندازه (type) > 8)])))

#define _STRLEN(s) (اندازه (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (صحیح))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
تعریفنوع نویسه proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

ایستا درونخطی تهی
get_proc_fd_path (proc_fd_path_t path, pid_t pid, صحیح fd)
{
  ثابت size_t max_len = اندازه (proc_fd_path_t);
  size_t n;

  اگر (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  وگرنه
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  اگر (UNLIKELY (n >= max_len))
    abort ();
}

ایستا درونخطی تهی
get_proc_self_fd_path (proc_fd_path_t path, صحیح fd)
{
  get_proc_fd_path (path, 0, fd);
}

ایستا درونخطی تهی
get_self_fd_path (proc_fd_path_t path, صحیح fd)
{
  snprintf (path, اندازه (proc_fd_path_t), "self/fd/%d", fd);
}

ایستا درونخطی صحیح
validate_options (unsigned صحیح specified_options, unsigned صحیح supported_options, libcrun_error_t *err)
{
  اگر (! ! (~supported_options & specified_options))
    بازگشت crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  بازگشت 0;
}

بیرونی صحیح cpuset_string_to_bitmask (ثابت نویسه *str, نویسه **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
ساختار channel_fd_pair;

ساختار channel_fd_pair *channel_fd_pair_new (صحیح in_fd, صحیح out_fd, size_t size);

تهی channel_fd_pair_free (ساختار channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
صحیح channel_fd_pair_process (ساختار channel_fd_pair *channel, صحیح epollfd, libcrun_error_t *err);

ایستا درونخطی تهی
cleanup_channel_fd_pairp (تهی *p)
{
  ساختار channel_fd_pair **pp = (ساختار channel_fd_pair **) p;
  اگر (*pp == پوچ)
    بازگشت;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

صحیح libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_open_proc_file (libcrun_container_t *container, ثابت نویسه *path, صحیح flags, libcrun_error_t *err);
صحیح libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ثابت نویسه *path, صحیح flags, libcrun_error_t *err);

#endif
