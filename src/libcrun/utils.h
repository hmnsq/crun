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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ "error.h"
#અન્તર્ભાવ <dirent.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <ocispec/runtime_spec_schema_config_schema.h>
#અન્તર્ભાવ <sys/wait.h>
#અન્તર્ભાવ "container.h"

#ifndef TEMP_FAILURE_RETRY
#  define TEMP_FAILURE_RETRY(expression)      \
    (__extension__ ({                         \
      long પૂર્ણાંક __result;                      \
      કરો                                      \
        __result = (long પૂર્ણાંક) (expression);   \
      જ્યારે (__result < 0 && errno == EINTR); \
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

__attribute__ ((malloc)) સ્થિર અંતઃરેખ શૂન્ય *
xmalloc (size_t size)
{
  શૂન્ય *res = malloc (size);
  જો (UNLIKELY (res == ખાલી))
    OOM ();
  પાછા res;
}

__attribute__ ((malloc)) સ્થિર અંતઃરેખ શૂન્ય *
xmalloc0 (size_t size)
{
  શૂન્ય *res = calloc (1, size);
  જો (UNLIKELY (res == ખાલી))
    OOM ();
  પાછા res;
}

__attribute__ ((malloc)) સ્થિર અંતઃરેખ શૂન્ય *
xrealloc (શૂન્ય *ptr, size_t size)
{
  શૂન્ય *res = realloc (ptr, size);
  જો (UNLIKELY (res == ખાલી))
    OOM ();
  પાછા res;
}

સ્થિર અંતઃરેખ શૂન્ય
cleanup_freep (શૂન્ય *p)
{
  શૂન્ય **pp = (શૂન્ય **) p;
  મુક્ત (*pp);
}

સ્થિર અંતઃરેખ શૂન્ય
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  જો (file)
    (શૂન્ય) fclose (file);
}

સ્થિર અંતઃરેખ શૂન્ય
cleanup_closep (શૂન્ય *p)
{
  પૂર્ણાંક *pp = (પૂર્ણાંક *) p;
  જો (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

સ્થિર અંતઃરેખ શૂન્ય
cleanup_pidp (શૂન્ય *p)
{
  pid_t *pp = (pid_t *) p;
  જો (*pp > 0)
    {
      TEMP_FAILURE_RETRY (kill (*pp, SIGKILL));
      TEMP_FAILURE_RETRY (waitpid (*pp, ખાલી, 0));
    }
}

સંરચના libcrun_mmap_s
{
  શૂન્ય *addr;
  size_t length;
};

પૂર્ણાંક libcrun_mmap (સંરચના libcrun_mmap_s **ret, શૂન્ય *addr, size_t length,
                  પૂર્ણાંક prot, પૂર્ણાંક flags, પૂર્ણાંક fd, off_t offset,
                  libcrun_error_t *err);

પૂર્ણાંક libcrun_munmap (સંરચના libcrun_mmap_s *mmap, libcrun_error_t *err);

સ્થિર અંતઃરેખ શૂન્ય
cleanup_mmapp (શૂન્ય *p)
{
  પૂર્ણાંક ret;
  libcrun_error_t tmp_err = ખાલી;
  સંરચના libcrun_mmap_s **mm;

  mm = (સંરચના libcrun_mmap_s **) p;
  જો (*mm == ખાલી)
    પાછા;

  ret = libcrun_munmap (*mm, &tmp_err);
  જો (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

સંરચના libcrun_fd_map
{
  size_t nfds;
  પૂર્ણાંક fds[];
};

સ્થિર અંતઃરેખ સંરચના libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  સંરચના libcrun_fd_map *ret;
  size_t i;

  ret = (સંરચના libcrun_fd_map *) xmalloc (કદ (*ret) + કદ (પૂર્ણાંક) * len);
  ret->nfds = len;
  માટે (i = 0; i < len; i++)
    ret->fds[i] = -1;

  પાછા ret;
}

સ્થિર અંતઃરેખ શૂન્ય
cleanup_close_mapp (સંરચના libcrun_fd_map **p)
{
  સંરચના libcrun_fd_map *m = *p;
  size_t i;

  જો (m == ખાલી)
    પાછા;

  માટે (i = 0; i < m->nfds; i++)
    જો (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  મુક્ત (m);
}

સ્થિર અંતઃરેખ શૂન્ય
cleanup_close_vecp (પૂર્ણાંક **p)
{
  પૂર્ણાંક *pp = *p;
  પૂર્ણાંક i;

  જો (pp == ખાલી)
    પાછા;

  માટે (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

સ્થિર અંતઃરેખ શૂન્ય
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  જો (dir)
    closedir (dir);
}

સ્થિર અંતઃરેખ પૂર્ણાંક
close_and_reset (પૂર્ણાંક *fd)
{
  પૂર્ણાંક ret = 0;
  જો (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      જો (LIKELY (ret == 0))
        *fd = -1;
    }
  પાછા ret;
}

સ્થિર અંતઃરેખ અક્ષર *
xstrdup (અચલ અક્ષર *str)
{
  અક્ષર *ret;
  જો (str == ખાલી)
    પાછા ખાલી;

  ret = strdup (str);
  જો (ret == ખાલી)
    OOM ();

  પાછા ret;
}

શૂન્ય consume_trailing_slashes (અક્ષર *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    જ્યારે (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

સ્થિર અંતઃરેખ bool
path_is_slash_dev (અચલ અક્ષર *path)
{
  path = consume_slashes (path);

  જો (strncmp (path, "dev", 3))
    પાછા false;

  path += 3;

  /* Check there are only '/' left.  */
  માટે (; *path; path++)
    જો (*path != '/')
      પાછા false;

  પાછા true;
}

પૂર્ણાંક xasprintf (અક્ષર **str, અચલ અક્ષર *fmt, ...) __attribute__ ((format (printf, 2, 3)));

પૂર્ણાંક crun_path_exists (અચલ અક્ષર *path, libcrun_error_t *err);

પૂર્ણાંક write_file_at_with_flags (પૂર્ણાંક dirfd, પૂર્ણાંક flags, mode_t mode, અચલ અક્ષર *name, અચલ શૂન્ય *data, size_t len, libcrun_error_t *err);

સ્થિર અંતઃરેખ પૂર્ણાંક
write_file (અચલ અક્ષર *name, અચલ શૂન્ય *data, size_t len, libcrun_error_t *err)
{
  પાછા write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

સ્થિર અંતઃરેખ પૂર્ણાંક
write_file_at (પૂર્ણાંક dirfd, અચલ અક્ષર *name, અચલ શૂન્ય *data, size_t len, libcrun_error_t *err)
{
  પાછા write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

પૂર્ણાંક crun_ensure_directory (અચલ અક્ષર *path, પૂર્ણાંક mode, bool nofollow, libcrun_error_t *err);

પૂર્ણાંક crun_ensure_directory_at (પૂર્ણાંક dirfd, અચલ અક્ષર *path, પૂર્ણાંક mode, bool nofollow, libcrun_error_t *err);

પૂર્ણાંક crun_safe_create_and_open_ref_at (bool dir, પૂર્ણાંક dirfd, અચલ અક્ષર *dirpath, અચલ અક્ષર *path, પૂર્ણાંક mode, libcrun_error_t *err);

પૂર્ણાંક crun_safe_ensure_directory_at (પૂર્ણાંક dirfd, અચલ અક્ષર *dirpath, અચલ અક્ષર *path, પૂર્ણાંક mode,
                                   libcrun_error_t *err);

પૂર્ણાંક crun_safe_ensure_file_at (પૂર્ણાંક dirfd, અચલ અક્ષર *dirpath, અચલ અક્ષર *path, પૂર્ણાંક mode,
                              libcrun_error_t *err);

પૂર્ણાંક crun_dir_p (અચલ અક્ષર *path, bool nofollow, libcrun_error_t *err);

પૂર્ણાંક crun_dir_p_at (પૂર્ણાંક dirfd, અચલ અક્ષર *path, bool nofollow, libcrun_error_t *err);

પૂર્ણાંક detach_process ();

પૂર્ણાંક create_file_if_missing_at (પૂર્ણાંક dirfd, અચલ અક્ષર *file, mode_t mode, libcrun_error_t *err);

પૂર્ણાંક check_running_in_user_namespace (libcrun_error_t *err);

પૂર્ણાંક set_selinux_label (libcrun_container_t *container, અચલ અક્ષર *label, bool now, libcrun_error_t *err);

પૂર્ણાંક add_selinux_mount_label (અક્ષર **ret, અચલ અક્ષર *data, અચલ અક્ષર *label, અચલ અક્ષર *context_type, libcrun_error_t *err);

પૂર્ણાંક set_apparmor_profile (libcrun_container_t *container, અચલ અક્ષર *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

પૂર્ણાંક read_all_fd_with_size_hint (પૂર્ણાંક fd, અચલ અક્ષર *description, અક્ષર **out, size_t *len, size_t hint, libcrun_error_t *err);

પૂર્ણાંક read_all_fd_with_max_size (પૂર્ણાંક fd, અચલ અક્ષર *description, અક્ષર **out, size_t *len, size_t max_size, libcrun_error_t *err);

સ્થિર અંતઃરેખ પૂર્ણાંક
read_all_fd (પૂર્ણાંક fd, અચલ અક્ષર *description, અક્ષર **out, size_t *len, libcrun_error_t *err)
{
  પાછા read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

પૂર્ણાંક get_realpath_to_file (પૂર્ણાંક dirfd, અચલ અક્ષર *path_name, અક્ષર **absolute_path, libcrun_error_t *err);

પૂર્ણાંક read_all_file (અચલ અક્ષર *path, અક્ષર **out, size_t *len, libcrun_error_t *err);

પૂર્ણાંક read_all_file_at (પૂર્ણાંક dirfd, અચલ અક્ષર *path, અક્ષર **out, size_t *len, libcrun_error_t *err);

પૂર્ણાંક open_unix_domain_client_socket (અચલ અક્ષર *path, પૂર્ણાંક dgram, libcrun_error_t *err);

પૂર્ણાંક open_unix_domain_socket (અચલ અક્ષર *path, પૂર્ણાંક dgram, libcrun_error_t *err);

પૂર્ણાંક send_fd_to_socket (પૂર્ણાંક server, પૂર્ણાંક fd, libcrun_error_t *err);

પૂર્ણાંક send_fd_to_socket_with_payload (પૂર્ણાંક server, પૂર્ણાંક fd, અચલ અક્ષર *payload, size_t payload_len, libcrun_error_t *err);

પૂર્ણાંક create_socket_pair (પૂર્ણાંક *pair, libcrun_error_t *err);

પૂર્ણાંક receive_fd_from_socket (પૂર્ણાંક from, libcrun_error_t *err);

પૂર્ણાંક receive_fd_from_socket_with_payload (પૂર્ણાંક from, અક્ષર *payload, size_t payload_len, libcrun_error_t *err);

પૂર્ણાંક create_signalfd (sigset_t *mask, libcrun_error_t *err);

પૂર્ણાંક epoll_helper (પૂર્ણાંક *in_fds, પૂર્ણાંક *in_levelfds, પૂર્ણાંક *out_fds, પૂર્ણાંક *out_levelfds, libcrun_error_t *err);

પૂર્ણાંક copy_from_fd_to_fd (પૂર્ણાંક src, પૂર્ણાંક dst, પૂર્ણાંક consume, libcrun_error_t *err);

પૂર્ણાંક run_process (અક્ષર **args, libcrun_error_t *err);

પૂર્ણાંક format_default_id_mapping (અક્ષર **out, uid_t container_id, uid_t host_uid, uid_t host_id, પૂર્ણાંક is_uid, libcrun_error_t *err);

પૂર્ણાંક run_process_with_stdin_timeout_envp (અક્ષર *path, અક્ષર **args, અચલ અક્ષર *cwd, પૂર્ણાંક timeout, અક્ષર **envp,
                                         અક્ષર *stdin, size_t stdin_len, પૂર્ણાંક out_fd, પૂર્ણાંક err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

પૂર્ણાંક mark_or_close_fds_ge_than (libcrun_container_t *container, પૂર્ણાંક n, bool close_now, libcrun_error_t *err);

શૂન્ય get_current_timestamp (અક્ષર *out, size_t len);

પૂર્ણાંક set_blocking_fd (પૂર્ણાંક fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC પૂર્ણાંક parse_json_file (json_object **out, અચલ અક્ષર *jsondata, સંરચના parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
સ્થિર અંતઃરેખ પૂર્ણાંક
has_prefix (અચલ અક્ષર *str, અચલ અક્ષર *prefix)
{
  size_t prefix_len;

  જો (! str || ! prefix)
    પાછા 0;

  prefix_len = strlen (prefix);
  પાછા strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
સ્થિર અંતઃરેખ પૂર્ણાંક
has_dir_prefix (અચલ અક્ષર *str, અચલ અક્ષર *prefix)
{
  size_t prefix_len;

  જો (! str || ! prefix)
    પાછા 0;

  prefix_len = strlen (prefix);
  પાછા strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

પૂર્ણાંક find_executable (અક્ષર **exec_path, અચલ અક્ષર *executable_path, અચલ અક્ષર *cwd, libcrun_error_t *err);

પૂર્ણાંક copy_recursive_fd_to_fd (પૂર્ણાંક srcfd, પૂર્ણાંક destfd, અચલ અક્ષર *srcname, અચલ અક્ષર *destname, libcrun_error_t *err);

પૂર્ણાંક set_home_env (uid_t uid);

પૂર્ણાંક libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

પૂર્ણાંક libcrun_initialize_apparmor (libcrun_error_t *err);

અચલ અક્ષર *find_annotation (libcrun_container_t *container, અચલ અક્ષર *name);

પૂર્ણાંક get_file_type_at (પૂર્ણાંક dirfd, mode_t *mode, bool nofollow, અચલ અક્ષર *path);

પૂર્ણાંક get_file_type (mode_t *mode, bool nofollow, અચલ અક્ષર *path);

પૂર્ણાંક get_file_type_fd (પૂર્ણાંક fd, mode_t *mode);

અક્ષર *get_user_name (uid_t uid);

પૂર્ણાંક safe_openat (પૂર્ણાંક dirfd, અચલ અક્ષર *rootfs, અચલ અક્ષર *path, પૂર્ણાંક flags, પૂર્ણાંક mode,
                 libcrun_error_t *err);

પૂર્ણાંક safe_write (પૂર્ણાંક fd, અચલ અક્ષર *fname, અચલ શૂન્ય *buf, size_t ગણતરી, libcrun_error_t *err);

પૂર્ણાંક append_paths (અક્ષર **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

પૂર્ણાંક str2sig (અચલ અક્ષર *name);

પૂર્ણાંક base64_decode (અચલ અક્ષર *iptr, size_t isize, અક્ષર *optr, size_t osize, size_t *nbytes);
પૂર્ણાંક has_suffix (અચલ અક્ષર *source, અચલ અક્ષર *suffix);
અક્ષર *libcrun_sysctl_ચાવી_to_proc_path (અચલ અક્ષર *ચાવી);
અક્ષર *str_join_array (પૂર્ણાંક offset, size_t size, અક્ષર *અચલ array[], અચલ અક્ષર *joint);

ssize_t safe_readlinkat (પૂર્ણાંક dfd, અચલ અક્ષર *name, અક્ષર **buffer, ssize_t hint, libcrun_error_t *err);

અક્ષર **read_dir_entries (અચલ અક્ષર *path, libcrun_error_t *err);

સ્થિર અંતઃરેખ bool
is_empty_string (અચલ અક્ષર *s)
{
  પાછા s == ખાલી || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
સ્થિર અંતઃરેખ size_t
safe_strlen (અચલ અક્ષર *s)
{
  પાછા s == ખાલી ? 0 : strlen (s);
}

સ્થિર અંતઃરેખ bool
path_has_dot_dot_component (અચલ અક્ષર *path)
{
  જો (path == ખાલી)
    પાછા false;
  cleanup_free અક્ષર *npath = xstrdup (path);
  અક્ષર *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  જ્યારે (cur)
    {
      જો (it)
        *it = '\0';

      જો (strcmp (cur, "..") == 0)
        પાછા true;

      જો (it == ખાલી)
        તોડો;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  પાછા false;
}

સ્થિર અંતઃરેખ પૂર્ણાંક
waitpid_ignore_stopped (pid_t pid, પૂર્ણાંક *status, પૂર્ણાંક options)
{
  પૂર્ણાંક ret, s;
  કરો
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      જો (ret < 0)
        પાછા ret;
  } જ્યારે (WIFSTOPPED (s) || WIFCONTINUED (s));

  જો (status)
    *status = s;

  પાછા ret;
}

સ્થિર અંતઃરેખ પૂર્ણાંક
get_process_exit_status (પૂર્ણાંક status)
{
  જો (WIFEXITED (status))
    પાછા WEXITSTATUS (status);
  જો (WIFSIGNALED (status))
    પાછા 128 + WTERMSIG (status);

  પાછા -1;
}

uid_t get_overflow_uid (શૂન્ય);
gid_t get_overflow_gid (શૂન્ય);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (કદ (type) <= 1 ? 3U : કદ (type) <= 2 ? 5U  \
                                        : કદ (type) <= 4   ? 10U \
                                        : કદ (type) <= 8   ? 20U \
                                                               : કદ (પૂર્ણાંક[-2 * (કદ (type) > 8)])))

#define _STRLEN(s) (કદ (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (પૂર્ણાંક))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
પ્રકારવ્યાખ્યા અક્ષર proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

સ્થિર અંતઃરેખ શૂન્ય
get_proc_fd_path (proc_fd_path_t path, pid_t pid, પૂર્ણાંક fd)
{
  અચલ size_t max_len = કદ (proc_fd_path_t);
  size_t n;

  જો (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  નહીંતર
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  જો (UNLIKELY (n >= max_len))
    abort ();
}

સ્થિર અંતઃરેખ શૂન્ય
get_proc_self_fd_path (proc_fd_path_t path, પૂર્ણાંક fd)
{
  get_proc_fd_path (path, 0, fd);
}

સ્થિર અંતઃરેખ શૂન્ય
get_self_fd_path (proc_fd_path_t path, પૂર્ણાંક fd)
{
  snprintf (path, કદ (proc_fd_path_t), "self/fd/%d", fd);
}

સ્થિર અંતઃરેખ પૂર્ણાંક
validate_options (unsigned પૂર્ણાંક specified_options, unsigned પૂર્ણાંક supported_options, libcrun_error_t *err)
{
  જો (! ! (~supported_options & specified_options))
    પાછા crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  પાછા 0;
}

બાહ્ય પૂર્ણાંક cpuset_string_to_bitmask (અચલ અક્ષર *str, અક્ષર **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
સંરચના channel_fd_pair;

સંરચના channel_fd_pair *channel_fd_pair_new (પૂર્ણાંક in_fd, પૂર્ણાંક out_fd, size_t size);

શૂન્ય channel_fd_pair_free (સંરચના channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
પૂર્ણાંક channel_fd_pair_process (સંરચના channel_fd_pair *channel, પૂર્ણાંક epollfd, libcrun_error_t *err);

સ્થિર અંતઃરેખ શૂન્ય
cleanup_channel_fd_pairp (શૂન્ય *p)
{
  સંરચના channel_fd_pair **pp = (સંરચના channel_fd_pair **) p;
  જો (*pp == ખાલી)
    પાછા;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

પૂર્ણાંક libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
પૂર્ણાંક libcrun_open_proc_file (libcrun_container_t *container, અચલ અક્ષર *path, પૂર્ણાંક flags, libcrun_error_t *err);
પૂર્ણાંક libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, અચલ અક્ષર *path, પૂર્ણાંક flags, libcrun_error_t *err);

#endif
