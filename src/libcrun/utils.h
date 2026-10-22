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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ "error.h"
#அந்தர்பாவ <dirent.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <ocispec/runtime_spec_schema_config_schema.h>
#அந்தர்பாவ <sys/wait.h>
#அந்தர்பாவ "container.h"

#ifndef TEMP_FAILURE_RETRY
#  define TEMP_FAILURE_RETRY(expression)      \
    (__extension__ ({                         \
      long முழு __result;                      \
      செய்                                      \
        __result = (long முழு) (expression);   \
      போது (__result < 0 && errno == EINTR); \
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

__attribute__ ((malloc)) நிலையான உள்வரி வெறுமை *
xmalloc (size_t size)
{
  வெறுமை *res = malloc (size);
  எனில் (UNLIKELY (res == வெற்று))
    OOM ();
  திரும்பு res;
}

__attribute__ ((malloc)) நிலையான உள்வரி வெறுமை *
xmalloc0 (size_t size)
{
  வெறுமை *res = calloc (1, size);
  எனில் (UNLIKELY (res == வெற்று))
    OOM ();
  திரும்பு res;
}

__attribute__ ((malloc)) நிலையான உள்வரி வெறுமை *
xrealloc (வெறுமை *ptr, size_t size)
{
  வெறுமை *res = realloc (ptr, size);
  எனில் (UNLIKELY (res == வெற்று))
    OOM ();
  திரும்பு res;
}

நிலையான உள்வரி வெறுமை
cleanup_freep (வெறுமை *p)
{
  வெறுமை **pp = (வெறுமை **) p;
  விடுவி (*pp);
}

நிலையான உள்வரி வெறுமை
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  எனில் (file)
    (வெறுமை) fclose (file);
}

நிலையான உள்வரி வெறுமை
cleanup_closep (வெறுமை *p)
{
  முழு *pp = (முழு *) p;
  எனில் (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

நிலையான உள்வரி வெறுமை
cleanup_pidp (வெறுமை *p)
{
  pid_t *pp = (pid_t *) p;
  எனில் (*pp > 0)
    {
      TEMP_FAILURE_RETRY (kill (*pp, SIGKILL));
      TEMP_FAILURE_RETRY (waitpid (*pp, வெற்று, 0));
    }
}

கட்டமைப்பு libcrun_mmap_s
{
  வெறுமை *addr;
  size_t length;
};

முழு libcrun_mmap (கட்டமைப்பு libcrun_mmap_s **ret, வெறுமை *addr, size_t length,
                  முழு prot, முழு flags, முழு fd, off_t offset,
                  libcrun_error_t *err);

முழு libcrun_munmap (கட்டமைப்பு libcrun_mmap_s *mmap, libcrun_error_t *err);

நிலையான உள்வரி வெறுமை
cleanup_mmapp (வெறுமை *p)
{
  முழு ret;
  libcrun_error_t tmp_err = வெற்று;
  கட்டமைப்பு libcrun_mmap_s **mm;

  mm = (கட்டமைப்பு libcrun_mmap_s **) p;
  எனில் (*mm == வெற்று)
    திரும்பு;

  ret = libcrun_munmap (*mm, &tmp_err);
  எனில் (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

கட்டமைப்பு libcrun_fd_map
{
  size_t nfds;
  முழு fds[];
};

நிலையான உள்வரி கட்டமைப்பு libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  கட்டமைப்பு libcrun_fd_map *ret;
  size_t i;

  ret = (கட்டமைப்பு libcrun_fd_map *) xmalloc (அளவு (*ret) + அளவு (முழு) * len);
  ret->nfds = len;
  க்கு (i = 0; i < len; i++)
    ret->fds[i] = -1;

  திரும்பு ret;
}

நிலையான உள்வரி வெறுமை
cleanup_close_mapp (கட்டமைப்பு libcrun_fd_map **p)
{
  கட்டமைப்பு libcrun_fd_map *m = *p;
  size_t i;

  எனில் (m == வெற்று)
    திரும்பு;

  க்கு (i = 0; i < m->nfds; i++)
    எனில் (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  விடுவி (m);
}

நிலையான உள்வரி வெறுமை
cleanup_close_vecp (முழு **p)
{
  முழு *pp = *p;
  முழு i;

  எனில் (pp == வெற்று)
    திரும்பு;

  க்கு (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

நிலையான உள்வரி வெறுமை
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  எனில் (dir)
    closedir (dir);
}

நிலையான உள்வரி முழு
close_and_reset (முழு *fd)
{
  முழு ret = 0;
  எனில் (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      எனில் (LIKELY (ret == 0))
        *fd = -1;
    }
  திரும்பு ret;
}

நிலையான உள்வரி எழுத்து *
xstrdup (மாறிலி எழுத்து *str)
{
  எழுத்து *ret;
  எனில் (str == வெற்று)
    திரும்பு வெற்று;

  ret = strdup (str);
  எனில் (ret == வெற்று)
    OOM ();

  திரும்பு ret;
}

வெறுமை consume_trailing_slashes (எழுத்து *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    போது (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

நிலையான உள்வரி bool
path_is_slash_dev (மாறிலி எழுத்து *path)
{
  path = consume_slashes (path);

  எனில் (strncmp (path, "dev", 3))
    திரும்பு false;

  path += 3;

  /* Check there are only '/' left.  */
  க்கு (; *path; path++)
    எனில் (*path != '/')
      திரும்பு false;

  திரும்பு true;
}

முழு xasprintf (எழுத்து **str, மாறிலி எழுத்து *fmt, ...) __attribute__ ((format (printf, 2, 3)));

முழு crun_path_exists (மாறிலி எழுத்து *path, libcrun_error_t *err);

முழு write_file_at_with_flags (முழு dirfd, முழு flags, mode_t mode, மாறிலி எழுத்து *name, மாறிலி வெறுமை *data, size_t len, libcrun_error_t *err);

நிலையான உள்வரி முழு
write_file (மாறிலி எழுத்து *name, மாறிலி வெறுமை *data, size_t len, libcrun_error_t *err)
{
  திரும்பு write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

நிலையான உள்வரி முழு
write_file_at (முழு dirfd, மாறிலி எழுத்து *name, மாறிலி வெறுமை *data, size_t len, libcrun_error_t *err)
{
  திரும்பு write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

முழு crun_ensure_directory (மாறிலி எழுத்து *path, முழு mode, bool nofollow, libcrun_error_t *err);

முழு crun_ensure_directory_at (முழு dirfd, மாறிலி எழுத்து *path, முழு mode, bool nofollow, libcrun_error_t *err);

முழு crun_safe_create_and_open_ref_at (bool dir, முழு dirfd, மாறிலி எழுத்து *dirpath, மாறிலி எழுத்து *path, முழு mode, libcrun_error_t *err);

முழு crun_safe_ensure_directory_at (முழு dirfd, மாறிலி எழுத்து *dirpath, மாறிலி எழுத்து *path, முழு mode,
                                   libcrun_error_t *err);

முழு crun_safe_ensure_file_at (முழு dirfd, மாறிலி எழுத்து *dirpath, மாறிலி எழுத்து *path, முழு mode,
                              libcrun_error_t *err);

முழு crun_dir_p (மாறிலி எழுத்து *path, bool nofollow, libcrun_error_t *err);

முழு crun_dir_p_at (முழு dirfd, மாறிலி எழுத்து *path, bool nofollow, libcrun_error_t *err);

முழு detach_process ();

முழு create_file_if_missing_at (முழு dirfd, மாறிலி எழுத்து *file, mode_t mode, libcrun_error_t *err);

முழு check_running_in_user_namespace (libcrun_error_t *err);

முழு set_selinux_label (libcrun_container_t *container, மாறிலி எழுத்து *label, bool now, libcrun_error_t *err);

முழு add_selinux_mount_label (எழுத்து **ret, மாறிலி எழுத்து *data, மாறிலி எழுத்து *label, மாறிலி எழுத்து *context_type, libcrun_error_t *err);

முழு set_apparmor_profile (libcrun_container_t *container, மாறிலி எழுத்து *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

முழு read_all_fd_with_size_hint (முழு fd, மாறிலி எழுத்து *description, எழுத்து **out, size_t *len, size_t hint, libcrun_error_t *err);

முழு read_all_fd_with_max_size (முழு fd, மாறிலி எழுத்து *description, எழுத்து **out, size_t *len, size_t max_size, libcrun_error_t *err);

நிலையான உள்வரி முழு
read_all_fd (முழு fd, மாறிலி எழுத்து *description, எழுத்து **out, size_t *len, libcrun_error_t *err)
{
  திரும்பு read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

முழு get_realpath_to_file (முழு dirfd, மாறிலி எழுத்து *path_name, எழுத்து **absolute_path, libcrun_error_t *err);

முழு read_all_file (மாறிலி எழுத்து *path, எழுத்து **out, size_t *len, libcrun_error_t *err);

முழு read_all_file_at (முழு dirfd, மாறிலி எழுத்து *path, எழுத்து **out, size_t *len, libcrun_error_t *err);

முழு open_unix_domain_client_socket (மாறிலி எழுத்து *path, முழு dgram, libcrun_error_t *err);

முழு open_unix_domain_socket (மாறிலி எழுத்து *path, முழு dgram, libcrun_error_t *err);

முழு send_fd_to_socket (முழு server, முழு fd, libcrun_error_t *err);

முழு send_fd_to_socket_with_payload (முழு server, முழு fd, மாறிலி எழுத்து *payload, size_t payload_len, libcrun_error_t *err);

முழு create_socket_pair (முழு *pair, libcrun_error_t *err);

முழு receive_fd_from_socket (முழு from, libcrun_error_t *err);

முழு receive_fd_from_socket_with_payload (முழு from, எழுத்து *payload, size_t payload_len, libcrun_error_t *err);

முழு create_signalfd (sigset_t *mask, libcrun_error_t *err);

முழு epoll_helper (முழு *in_fds, முழு *in_levelfds, முழு *out_fds, முழு *out_levelfds, libcrun_error_t *err);

முழு copy_from_fd_to_fd (முழு src, முழு dst, முழு consume, libcrun_error_t *err);

முழு run_process (எழுத்து **args, libcrun_error_t *err);

முழு format_default_id_mapping (எழுத்து **out, uid_t container_id, uid_t host_uid, uid_t host_id, முழு is_uid, libcrun_error_t *err);

முழு run_process_with_stdin_timeout_envp (எழுத்து *path, எழுத்து **args, மாறிலி எழுத்து *cwd, முழு timeout, எழுத்து **envp,
                                         எழுத்து *stdin, size_t stdin_len, முழு out_fd, முழு err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

முழு mark_or_close_fds_ge_than (libcrun_container_t *container, முழு n, bool close_now, libcrun_error_t *err);

வெறுமை get_current_timestamp (எழுத்து *out, size_t len);

முழு set_blocking_fd (முழு fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC முழு parse_json_file (json_object **out, மாறிலி எழுத்து *jsondata, கட்டமைப்பு parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
நிலையான உள்வரி முழு
has_prefix (மாறிலி எழுத்து *str, மாறிலி எழுத்து *prefix)
{
  size_t prefix_len;

  எனில் (! str || ! prefix)
    திரும்பு 0;

  prefix_len = strlen (prefix);
  திரும்பு strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
நிலையான உள்வரி முழு
has_dir_prefix (மாறிலி எழுத்து *str, மாறிலி எழுத்து *prefix)
{
  size_t prefix_len;

  எனில் (! str || ! prefix)
    திரும்பு 0;

  prefix_len = strlen (prefix);
  திரும்பு strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

முழு find_executable (எழுத்து **exec_path, மாறிலி எழுத்து *executable_path, மாறிலி எழுத்து *cwd, libcrun_error_t *err);

முழு copy_recursive_fd_to_fd (முழு srcfd, முழு destfd, மாறிலி எழுத்து *srcname, மாறிலி எழுத்து *destname, libcrun_error_t *err);

முழு set_home_env (uid_t uid);

முழு libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

முழு libcrun_initialize_apparmor (libcrun_error_t *err);

மாறிலி எழுத்து *find_annotation (libcrun_container_t *container, மாறிலி எழுத்து *name);

முழு get_file_type_at (முழு dirfd, mode_t *mode, bool nofollow, மாறிலி எழுத்து *path);

முழு get_file_type (mode_t *mode, bool nofollow, மாறிலி எழுத்து *path);

முழு get_file_type_fd (முழு fd, mode_t *mode);

எழுத்து *get_user_name (uid_t uid);

முழு safe_openat (முழு dirfd, மாறிலி எழுத்து *rootfs, மாறிலி எழுத்து *path, முழு flags, முழு mode,
                 libcrun_error_t *err);

முழு safe_write (முழு fd, மாறிலி எழுத்து *fname, மாறிலி வெறுமை *buf, size_t மொத்தம், libcrun_error_t *err);

முழு append_paths (எழுத்து **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

முழு str2sig (மாறிலி எழுத்து *name);

முழு base64_decode (மாறிலி எழுத்து *iptr, size_t isize, எழுத்து *optr, size_t osize, size_t *nbytes);
முழு has_suffix (மாறிலி எழுத்து *source, மாறிலி எழுத்து *suffix);
எழுத்து *libcrun_sysctl_சாவி_to_proc_path (மாறிலி எழுத்து *சாவி);
எழுத்து *str_join_array (முழு offset, size_t size, எழுத்து *மாறிலி array[], மாறிலி எழுத்து *joint);

ssize_t safe_readlinkat (முழு dfd, மாறிலி எழுத்து *name, எழுத்து **buffer, ssize_t hint, libcrun_error_t *err);

எழுத்து **read_dir_entries (மாறிலி எழுத்து *path, libcrun_error_t *err);

நிலையான உள்வரி bool
is_empty_string (மாறிலி எழுத்து *s)
{
  திரும்பு s == வெற்று || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
நிலையான உள்வரி size_t
safe_strlen (மாறிலி எழுத்து *s)
{
  திரும்பு s == வெற்று ? 0 : strlen (s);
}

நிலையான உள்வரி bool
path_has_dot_dot_component (மாறிலி எழுத்து *path)
{
  எனில் (path == வெற்று)
    திரும்பு false;
  cleanup_free எழுத்து *npath = xstrdup (path);
  எழுத்து *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  போது (cur)
    {
      எனில் (it)
        *it = '\0';

      எனில் (strcmp (cur, "..") == 0)
        திரும்பு true;

      எனில் (it == வெற்று)
        நிறுத்து;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  திரும்பு false;
}

நிலையான உள்வரி முழு
waitpid_ignore_stopped (pid_t pid, முழு *status, முழு options)
{
  முழு ret, s;
  செய்
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      எனில் (ret < 0)
        திரும்பு ret;
  } போது (WIFSTOPPED (s) || WIFCONTINUED (s));

  எனில் (status)
    *status = s;

  திரும்பு ret;
}

நிலையான உள்வரி முழு
get_process_exit_status (முழு status)
{
  எனில் (WIFEXITED (status))
    திரும்பு WEXITSTATUS (status);
  எனில் (WIFSIGNALED (status))
    திரும்பு 128 + WTERMSIG (status);

  திரும்பு -1;
}

uid_t get_overflow_uid (வெறுமை);
gid_t get_overflow_gid (வெறுமை);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (அளவு (type) <= 1 ? 3U : அளவு (type) <= 2 ? 5U  \
                                        : அளவு (type) <= 4   ? 10U \
                                        : அளவு (type) <= 8   ? 20U \
                                                               : அளவு (முழு[-2 * (அளவு (type) > 8)])))

#define _STRLEN(s) (அளவு (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (முழு))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
வகைவரை எழுத்து proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

நிலையான உள்வரி வெறுமை
get_proc_fd_path (proc_fd_path_t path, pid_t pid, முழு fd)
{
  மாறிலி size_t max_len = அளவு (proc_fd_path_t);
  size_t n;

  எனில் (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  இல்லையெனில்
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  எனில் (UNLIKELY (n >= max_len))
    abort ();
}

நிலையான உள்வரி வெறுமை
get_proc_self_fd_path (proc_fd_path_t path, முழு fd)
{
  get_proc_fd_path (path, 0, fd);
}

நிலையான உள்வரி வெறுமை
get_self_fd_path (proc_fd_path_t path, முழு fd)
{
  snprintf (path, அளவு (proc_fd_path_t), "self/fd/%d", fd);
}

நிலையான உள்வரி முழு
validate_options (unsigned முழு specified_options, unsigned முழு supported_options, libcrun_error_t *err)
{
  எனில் (! ! (~supported_options & specified_options))
    திரும்பு crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  திரும்பு 0;
}

வெளி முழு cpuset_string_to_bitmask (மாறிலி எழுத்து *str, எழுத்து **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
கட்டமைப்பு channel_fd_pair;

கட்டமைப்பு channel_fd_pair *channel_fd_pair_new (முழு in_fd, முழு out_fd, size_t size);

வெறுமை channel_fd_pair_free (கட்டமைப்பு channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
முழு channel_fd_pair_process (கட்டமைப்பு channel_fd_pair *channel, முழு epollfd, libcrun_error_t *err);

நிலையான உள்வரி வெறுமை
cleanup_channel_fd_pairp (வெறுமை *p)
{
  கட்டமைப்பு channel_fd_pair **pp = (கட்டமைப்பு channel_fd_pair **) p;
  எனில் (*pp == வெற்று)
    திரும்பு;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

முழு libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
முழு libcrun_open_proc_file (libcrun_container_t *container, மாறிலி எழுத்து *path, முழு flags, libcrun_error_t *err);
முழு libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, மாறிலி எழுத்து *path, முழு flags, libcrun_error_t *err);

#endif
