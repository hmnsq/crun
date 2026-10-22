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

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <argp.h>
#include "error.h"
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <ocispec/runtime_spec_schema_config_schema.h>
#include <sys/wait.h>
#include "container.h"

#ifndef TEMP_FAILURE_RETRY
#  define TEMP_FAILURE_RETRY(expression)      \
    (__extension__ ({                         \
      long පූර්ණ_සංඛ්‍යා __result;                      \
      do                                      \
        __result = (long පූර්ණ_සංඛ්‍යා) (expression);   \
      අතරතුර (__result < 0 && errno == EINTR); \
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

__attribute__ ((malloc)) static anta_hstha void *
xmalloc (size_t size)
{
  void *res = malloc (size);
  නම් (UNLIKELY (res == NULL))
    OOM ();
  ආපසු res;
}

__attribute__ ((malloc)) static anta_hstha void *
xmalloc0 (size_t size)
{
  void *res = calloc (1, size);
  නම් (UNLIKELY (res == NULL))
    OOM ();
  ආපසු res;
}

__attribute__ ((malloc)) static anta_hstha void *
xrealloc (void *ptr, size_t size)
{
  void *res = realloc (ptr, size);
  නම් (UNLIKELY (res == NULL))
    OOM ();
  ආපසු res;
}

static anta_hstha void
cleanup_freep (void *p)
{
  void **pp = (void **) p;
  free (*pp);
}

static anta_hstha void
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  නම් (file)
    (void) fclose (file);
}

static anta_hstha void
cleanup_closep (void *p)
{
  පූර්ණ_සංඛ්‍යා *pp = (පූර්ණ_සංඛ්‍යා *) p;
  නම් (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

static anta_hstha void
cleanup_pidp (void *p)
{
  pid_t *pp = (pid_t *) p;
  නම් (*pp > 0)
    {
      TEMP_FAILURE_RETRY (kill (*pp, SIGKILL));
      TEMP_FAILURE_RETRY (waitpid (*pp, NULL, 0));
    }
}

sa_nracanaa libcrun_mmap_s
{
  void *addr;
  size_t length;
};

පූර්ණ_සංඛ්‍යා libcrun_mmap (sa_nracanaa libcrun_mmap_s **ret, void *addr, size_t length,
                  පූර්ණ_සංඛ්‍යා prot, පූර්ණ_සංඛ්‍යා flags, පූර්ණ_සංඛ්‍යා fd, off_t offset,
                  libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා libcrun_munmap (sa_nracanaa libcrun_mmap_s *mmap, libcrun_error_t *err);

static anta_hstha void
cleanup_mmapp (void *p)
{
  පූර්ණ_සංඛ්‍යා ret;
  libcrun_error_t tmp_err = NULL;
  sa_nracanaa libcrun_mmap_s **mm;

  mm = (sa_nracanaa libcrun_mmap_s **) p;
  නම් (*mm == NULL)
    ආපසු;

  ret = libcrun_munmap (*mm, &tmp_err);
  නම් (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

sa_nracanaa libcrun_fd_map
{
  size_t nfds;
  පූර්ණ_සංඛ්‍යා fds[];
};

static anta_hstha sa_nracanaa libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  sa_nracanaa libcrun_fd_map *ret;
  size_t i;

  ret = (sa_nracanaa libcrun_fd_map *) xmalloc (parimaa_na (*ret) + parimaa_na (පූර්ණ_සංඛ්‍යා) * len);
  ret->nfds = len;
  සඳහා (i = 0; i < len; i++)
    ret->fds[i] = -1;

  ආපසු ret;
}

static anta_hstha void
cleanup_close_mapp (sa_nracanaa libcrun_fd_map **p)
{
  sa_nracanaa libcrun_fd_map *m = *p;
  size_t i;

  නම් (m == NULL)
    ආපසු;

  සඳහා (i = 0; i < m->nfds; i++)
    නම් (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  free (m);
}

static anta_hstha void
cleanup_close_vecp (පූර්ණ_සංඛ්‍යා **p)
{
  පූර්ණ_සංඛ්‍යා *pp = *p;
  පූර්ණ_සංඛ්‍යා i;

  නම් (pp == NULL)
    ආපසු;

  සඳහා (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

static anta_hstha void
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  නම් (dir)
    closedir (dir);
}

static anta_hstha පූර්ණ_සංඛ්‍යා
close_and_reset (පූර්ණ_සංඛ්‍යා *fd)
{
  පූර්ණ_සංඛ්‍යා ret = 0;
  නම් (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      නම් (LIKELY (ret == 0))
        *fd = -1;
    }
  ආපසු ret;
}

static anta_hstha var_na *
xstrdup (const var_na *str)
{
  var_na *ret;
  නම් (str == NULL)
    ආපසු NULL;

  ret = strdup (str);
  නම් (ret == NULL)
    OOM ();

  ආපසු ret;
}

void consume_trailing_slashes (var_na *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    අතරතුර (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

static anta_hstha bool
path_is_slash_dev (const var_na *path)
{
  path = consume_slashes (path);

  නම් (strncmp (path, "dev", 3))
    ආපසු false;

  path += 3;

  /* Check there are only '/' left.  */
  සඳහා (; *path; path++)
    නම් (*path != '/')
      ආපසු false;

  ආපසු true;
}

පූර්ණ_සංඛ්‍යා xasprintf (var_na **str, const var_na *fmt, ...) __attribute__ ((format (printf, 2, 3)));

පූර්ණ_සංඛ්‍යා crun_path_exists (const var_na *path, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා write_file_at_with_flags (පූර්ණ_සංඛ්‍යා dirfd, පූර්ණ_සංඛ්‍යා flags, mode_t mode, const var_na *name, const void *data, size_t len, libcrun_error_t *err);

static anta_hstha පූර්ණ_සංඛ්‍යා
write_file (const var_na *name, const void *data, size_t len, libcrun_error_t *err)
{
  ආපසු write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

static anta_hstha පූර්ණ_සංඛ්‍යා
write_file_at (පූර්ණ_සංඛ්‍යා dirfd, const var_na *name, const void *data, size_t len, libcrun_error_t *err)
{
  ආපසු write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

පූර්ණ_සංඛ්‍යා crun_ensure_directory (const var_na *path, පූර්ණ_සංඛ්‍යා mode, bool nofollow, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා crun_ensure_directory_at (පූර්ණ_සංඛ්‍යා dirfd, const var_na *path, පූර්ණ_සංඛ්‍යා mode, bool nofollow, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා crun_safe_create_and_open_ref_at (bool dir, පූර්ණ_සංඛ්‍යා dirfd, const var_na *dirpath, const var_na *path, පූර්ණ_සංඛ්‍යා mode, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා crun_safe_ensure_directory_at (පූර්ණ_සංඛ්‍යා dirfd, const var_na *dirpath, const var_na *path, පූර්ණ_සංඛ්‍යා mode,
                                   libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා crun_safe_ensure_file_at (පූර්ණ_සංඛ්‍යා dirfd, const var_na *dirpath, const var_na *path, පූර්ණ_සංඛ්‍යා mode,
                              libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා crun_dir_p (const var_na *path, bool nofollow, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා crun_dir_p_at (පූර්ණ_සංඛ්‍යා dirfd, const var_na *path, bool nofollow, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා detach_process ();

පූර්ණ_සංඛ්‍යා create_file_if_missing_at (පූර්ණ_සංඛ්‍යා dirfd, const var_na *file, mode_t mode, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා check_running_in_user_namespace (libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා set_selinux_label (libcrun_container_t *container, const var_na *label, bool now, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා add_selinux_mount_label (var_na **ret, const var_na *data, const var_na *label, const var_na *context_type, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා set_apparmor_profile (libcrun_container_t *container, const var_na *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා read_all_fd_with_size_hint (පූර්ණ_සංඛ්‍යා fd, const var_na *description, var_na **out, size_t *len, size_t hint, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා read_all_fd_with_max_size (පූර්ණ_සංඛ්‍යා fd, const var_na *description, var_na **out, size_t *len, size_t max_size, libcrun_error_t *err);

static anta_hstha පූර්ණ_සංඛ්‍යා
read_all_fd (පූර්ණ_සංඛ්‍යා fd, const var_na *description, var_na **out, size_t *len, libcrun_error_t *err)
{
  ආපසු read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

පූර්ණ_සංඛ්‍යා get_realpath_to_file (පූර්ණ_සංඛ්‍යා dirfd, const var_na *path_name, var_na **absolute_path, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා read_all_file (const var_na *path, var_na **out, size_t *len, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා read_all_file_at (පූර්ණ_සංඛ්‍යා dirfd, const var_na *path, var_na **out, size_t *len, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා open_unix_domain_client_socket (const var_na *path, පූර්ණ_සංඛ්‍යා dgram, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා open_unix_domain_socket (const var_na *path, පූර්ණ_සංඛ්‍යා dgram, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා send_fd_to_socket (පූර්ණ_සංඛ්‍යා server, පූර්ණ_සංඛ්‍යා fd, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා send_fd_to_socket_with_payload (පූර්ණ_සංඛ්‍යා server, පූර්ණ_සංඛ්‍යා fd, const var_na *payload, size_t payload_len, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා create_socket_pair (පූර්ණ_සංඛ්‍යා *pair, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා receive_fd_from_socket (පූර්ණ_සංඛ්‍යා from, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා receive_fd_from_socket_with_payload (පූර්ණ_සංඛ්‍යා from, var_na *payload, size_t payload_len, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා create_signalfd (sigset_t *mask, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා epoll_helper (පූර්ණ_සංඛ්‍යා *in_fds, පූර්ණ_සංඛ්‍යා *in_levelfds, පූර්ණ_සංඛ්‍යා *out_fds, පූර්ණ_සංඛ්‍යා *out_levelfds, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා copy_from_fd_to_fd (පූර්ණ_සංඛ්‍යා src, පූර්ණ_සංඛ්‍යා dst, පූර්ණ_සංඛ්‍යා consume, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා run_process (var_na **args, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා format_default_id_mapping (var_na **out, uid_t container_id, uid_t host_uid, uid_t host_id, පූර්ණ_සංඛ්‍යා is_uid, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා run_process_with_stdin_timeout_envp (var_na *path, var_na **args, const var_na *cwd, පූර්ණ_සංඛ්‍යා timeout, var_na **envp,
                                         var_na *stdin, size_t stdin_len, පූර්ණ_සංඛ්‍යා out_fd, පූර්ණ_සංඛ්‍යා err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා mark_or_close_fds_ge_than (libcrun_container_t *container, පූර්ණ_සංඛ්‍යා n, bool close_now, libcrun_error_t *err);

void get_current_timestamp (var_na *out, size_t len);

පූර්ණ_සංඛ්‍යා set_blocking_fd (පූර්ණ_සංඛ්‍යා fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC පූර්ණ_සංඛ්‍යා parse_json_file (json_object **out, const var_na *jsondata, sa_nracanaa parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
static anta_hstha පූර්ණ_සංඛ්‍යා
has_prefix (const var_na *str, const var_na *prefix)
{
  size_t prefix_len;

  නම් (! str || ! prefix)
    ආපසු 0;

  prefix_len = strlen (prefix);
  ආපසු strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
static anta_hstha පූර්ණ_සංඛ්‍යා
has_dir_prefix (const var_na *str, const var_na *prefix)
{
  size_t prefix_len;

  නම් (! str || ! prefix)
    ආපසු 0;

  prefix_len = strlen (prefix);
  ආපසු strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

පූර්ණ_සංඛ්‍යා find_executable (var_na **exec_path, const var_na *executable_path, const var_na *cwd, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා copy_recursive_fd_to_fd (පූර්ණ_සංඛ්‍යා srcfd, පූර්ණ_සංඛ්‍යා destfd, const var_na *srcname, const var_na *destname, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා set_home_env (uid_t uid);

පූර්ණ_සංඛ්‍යා libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා libcrun_initialize_apparmor (libcrun_error_t *err);

const var_na *find_annotation (libcrun_container_t *container, const var_na *name);

පූර්ණ_සංඛ්‍යා get_file_type_at (පූර්ණ_සංඛ්‍යා dirfd, mode_t *mode, bool nofollow, const var_na *path);

පූර්ණ_සංඛ්‍යා get_file_type (mode_t *mode, bool nofollow, const var_na *path);

පූර්ණ_සංඛ්‍යා get_file_type_fd (පූර්ණ_සංඛ්‍යා fd, mode_t *mode);

var_na *get_user_name (uid_t uid);

පූර්ණ_සංඛ්‍යා safe_openat (පූර්ණ_සංඛ්‍යා dirfd, const var_na *rootfs, const var_na *path, පූර්ණ_සංඛ්‍යා flags, පූර්ණ_සංඛ්‍යා mode,
                 libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා safe_write (පූර්ණ_සංඛ්‍යා fd, const var_na *fname, const void *buf, size_t count, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා append_paths (var_na **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

පූර්ණ_සංඛ්‍යා str2sig (const var_na *name);

පූර්ණ_සංඛ්‍යා base64_decode (const var_na *iptr, size_t isize, var_na *optr, size_t osize, size_t *nbytes);
පූර්ණ_සංඛ්‍යා has_suffix (const var_na *source, const var_na *suffix);
var_na *libcrun_sysctl_key_to_proc_path (const var_na *key);
var_na *str_join_array (පූර්ණ_සංඛ්‍යා offset, size_t size, var_na *const array[], const var_na *joint);

ssize_t safe_readlinkat (පූර්ණ_සංඛ්‍යා dfd, const var_na *name, var_na **buffer, ssize_t hint, libcrun_error_t *err);

var_na **read_dir_entries (const var_na *path, libcrun_error_t *err);

static anta_hstha bool
is_empty_string (const var_na *s)
{
  ආපසු s == NULL || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
static anta_hstha size_t
safe_strlen (const var_na *s)
{
  ආපසු s == NULL ? 0 : strlen (s);
}

static anta_hstha bool
path_has_dot_dot_component (const var_na *path)
{
  නම් (path == NULL)
    ආපසු false;
  cleanup_free var_na *npath = xstrdup (path);
  var_na *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  අතරතුර (cur)
    {
      නම් (it)
        *it = '\0';

      නම් (strcmp (cur, "..") == 0)
        ආපසු true;

      නම් (it == NULL)
        bha_nga;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  ආපසු false;
}

static anta_hstha පූර්ණ_සංඛ්‍යා
waitpid_ignore_stopped (pid_t pid, පූර්ණ_සංඛ්‍යා *status, පූර්ණ_සංඛ්‍යා options)
{
  පූර්ණ_සංඛ්‍යා ret, s;
  do
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      නම් (ret < 0)
        ආපසු ret;
  } අතරතුර (WIFSTOPPED (s) || WIFCONTINUED (s));

  නම් (status)
    *status = s;

  ආපසු ret;
}

static anta_hstha පූර්ණ_සංඛ්‍යා
get_process_exit_status (පූර්ණ_සංඛ්‍යා status)
{
  නම් (WIFEXITED (status))
    ආපසු WEXITSTATUS (status);
  නම් (WIFSIGNALED (status))
    ආපසු 128 + WTERMSIG (status);

  ආපසු -1;
}

uid_t get_overflow_uid (void);
gid_t get_overflow_gid (void);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (parimaa_na (type) <= 1 ? 3U : parimaa_na (type) <= 2 ? 5U  \
                                        : parimaa_na (type) <= 4   ? 10U \
                                        : parimaa_na (type) <= 8   ? 20U \
                                                               : parimaa_na (පූර්ණ_සංඛ්‍යා[-2 * (parimaa_na (type) > 8)])))

#define _STRLEN(s) (parimaa_na (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (පූර්ණ_සංඛ්‍යා))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
typedef var_na proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

static anta_hstha void
get_proc_fd_path (proc_fd_path_t path, pid_t pid, පූර්ණ_සංඛ්‍යා fd)
{
  const size_t max_len = parimaa_na (proc_fd_path_t);
  size_t n;

  නම් (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  නැතහොත්
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  නම් (UNLIKELY (n >= max_len))
    abort ();
}

static anta_hstha void
get_proc_self_fd_path (proc_fd_path_t path, පූර්ණ_සංඛ්‍යා fd)
{
  get_proc_fd_path (path, 0, fd);
}

static anta_hstha void
get_self_fd_path (proc_fd_path_t path, පූර්ණ_සංඛ්‍යා fd)
{
  snprintf (path, parimaa_na (proc_fd_path_t), "self/fd/%d", fd);
}

static anta_hstha පූර්ණ_සංඛ්‍යා
validate_options (unsigned පූර්ණ_සංඛ්‍යා specified_options, unsigned පූර්ණ_සංඛ්‍යා supported_options, libcrun_error_t *err)
{
  නම් (! ! (~supported_options & specified_options))
    ආපසු crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  ආපසු 0;
}

extern පූර්ණ_සංඛ්‍යා cpuset_string_to_bitmask (const var_na *str, var_na **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
sa_nracanaa channel_fd_pair;

sa_nracanaa channel_fd_pair *channel_fd_pair_new (පූර්ණ_සංඛ්‍යා in_fd, පූර්ණ_සංඛ්‍යා out_fd, size_t size);

void channel_fd_pair_free (sa_nracanaa channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
පූර්ණ_සංඛ්‍යා channel_fd_pair_process (sa_nracanaa channel_fd_pair *channel, පූර්ණ_සංඛ්‍යා epollfd, libcrun_error_t *err);

static anta_hstha void
cleanup_channel_fd_pairp (void *p)
{
  sa_nracanaa channel_fd_pair **pp = (sa_nracanaa channel_fd_pair **) p;
  නම් (*pp == NULL)
    ආපසු;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

පූර්ණ_සංඛ්‍යා libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
පූර්ණ_සංඛ්‍යා libcrun_open_proc_file (libcrun_container_t *container, const var_na *path, පූර්ණ_සංඛ්‍යා flags, libcrun_error_t *err);
පූර්ණ_සංඛ්‍යා libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const var_na *path, පූර්ණ_සංඛ්‍යා flags, libcrun_error_t *err);

#endif
