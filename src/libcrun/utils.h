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
      long 정수 __result;                      \
      do                                      \
        __result = (long 정수) (expression);   \
      동안 (__result < 0 && errno == EINTR); \
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

__attribute__ ((malloc)) static inline void *
xmalloc (size_t size)
{
  void *res = malloc (size);
  만약 (UNLIKELY (res == NULL))
    OOM ();
  반환 res;
}

__attribute__ ((malloc)) static inline void *
xmalloc0 (size_t size)
{
  void *res = calloc (1, size);
  만약 (UNLIKELY (res == NULL))
    OOM ();
  반환 res;
}

__attribute__ ((malloc)) static inline void *
xrealloc (void *ptr, size_t size)
{
  void *res = realloc (ptr, size);
  만약 (UNLIKELY (res == NULL))
    OOM ();
  반환 res;
}

static inline void
cleanup_freep (void *p)
{
  void **pp = (void **) p;
  free (*pp);
}

static inline void
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  만약 (file)
    (void) fclose (file);
}

static inline void
cleanup_closep (void *p)
{
  정수 *pp = (정수 *) p;
  만약 (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

static inline void
cleanup_pidp (void *p)
{
  pid_t *pp = (pid_t *) p;
  만약 (*pp > 0)
    {
      TEMP_FAILURE_RETRY (kill (*pp, SIGKILL));
      TEMP_FAILURE_RETRY (waitpid (*pp, NULL, 0));
    }
}

struct libcrun_mmap_s
{
  void *addr;
  size_t length;
};

정수 libcrun_mmap (struct libcrun_mmap_s **ret, void *addr, size_t length,
                  정수 prot, 정수 flags, 정수 fd, off_t offset,
                  libcrun_error_t *err);

정수 libcrun_munmap (struct libcrun_mmap_s *mmap, libcrun_error_t *err);

static inline void
cleanup_mmapp (void *p)
{
  정수 ret;
  libcrun_error_t tmp_err = NULL;
  struct libcrun_mmap_s **mm;

  mm = (struct libcrun_mmap_s **) p;
  만약 (*mm == NULL)
    반환;

  ret = libcrun_munmap (*mm, &tmp_err);
  만약 (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

struct libcrun_fd_map
{
  size_t nfds;
  정수 fds[];
};

static inline struct libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  struct libcrun_fd_map *ret;
  size_t i;

  ret = (struct libcrun_fd_map *) xmalloc (sizeof (*ret) + sizeof (정수) * len);
  ret->nfds = len;
  위해 (i = 0; i < len; i++)
    ret->fds[i] = -1;

  반환 ret;
}

static inline void
cleanup_close_mapp (struct libcrun_fd_map **p)
{
  struct libcrun_fd_map *m = *p;
  size_t i;

  만약 (m == NULL)
    반환;

  위해 (i = 0; i < m->nfds; i++)
    만약 (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  free (m);
}

static inline void
cleanup_close_vecp (정수 **p)
{
  정수 *pp = *p;
  정수 i;

  만약 (pp == NULL)
    반환;

  위해 (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

static inline void
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  만약 (dir)
    closedir (dir);
}

static inline 정수
close_and_reset (정수 *fd)
{
  정수 ret = 0;
  만약 (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      만약 (LIKELY (ret == 0))
        *fd = -1;
    }
  반환 ret;
}

static inline char *
xstrdup (const char *str)
{
  char *ret;
  만약 (str == NULL)
    반환 NULL;

  ret = strdup (str);
  만약 (ret == NULL)
    OOM ();

  반환 ret;
}

void consume_trailing_slashes (char *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    동안 (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

static inline bool
path_is_slash_dev (const char *path)
{
  path = consume_slashes (path);

  만약 (strncmp (path, "dev", 3))
    반환 false;

  path += 3;

  /* Check there are only '/' left.  */
  위해 (; *path; path++)
    만약 (*path != '/')
      반환 false;

  반환 true;
}

정수 xasprintf (char **str, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));

정수 crun_path_exists (const char *path, libcrun_error_t *err);

정수 write_file_at_with_flags (정수 dirfd, 정수 flags, mode_t mode, const char *name, const void *data, size_t len, libcrun_error_t *err);

static inline 정수
write_file (const char *name, const void *data, size_t len, libcrun_error_t *err)
{
  반환 write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

static inline 정수
write_file_at (정수 dirfd, const char *name, const void *data, size_t len, libcrun_error_t *err)
{
  반환 write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

정수 crun_ensure_directory (const char *path, 정수 mode, bool nofollow, libcrun_error_t *err);

정수 crun_ensure_directory_at (정수 dirfd, const char *path, 정수 mode, bool nofollow, libcrun_error_t *err);

정수 crun_safe_create_and_open_ref_at (bool dir, 정수 dirfd, const char *dirpath, const char *path, 정수 mode, libcrun_error_t *err);

정수 crun_safe_ensure_directory_at (정수 dirfd, const char *dirpath, const char *path, 정수 mode,
                                   libcrun_error_t *err);

정수 crun_safe_ensure_file_at (정수 dirfd, const char *dirpath, const char *path, 정수 mode,
                              libcrun_error_t *err);

정수 crun_dir_p (const char *path, bool nofollow, libcrun_error_t *err);

정수 crun_dir_p_at (정수 dirfd, const char *path, bool nofollow, libcrun_error_t *err);

정수 detach_process ();

정수 create_file_if_missing_at (정수 dirfd, const char *file, mode_t mode, libcrun_error_t *err);

정수 check_running_in_user_namespace (libcrun_error_t *err);

정수 set_selinux_label (libcrun_container_t *container, const char *label, bool now, libcrun_error_t *err);

정수 add_selinux_mount_label (char **ret, const char *data, const char *label, const char *context_type, libcrun_error_t *err);

정수 set_apparmor_profile (libcrun_container_t *container, const char *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

정수 read_all_fd_with_size_hint (정수 fd, const char *description, char **out, size_t *len, size_t hint, libcrun_error_t *err);

정수 read_all_fd_with_max_size (정수 fd, const char *description, char **out, size_t *len, size_t max_size, libcrun_error_t *err);

static inline 정수
read_all_fd (정수 fd, const char *description, char **out, size_t *len, libcrun_error_t *err)
{
  반환 read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

정수 get_realpath_to_file (정수 dirfd, const char *path_name, char **absolute_path, libcrun_error_t *err);

정수 read_all_file (const char *path, char **out, size_t *len, libcrun_error_t *err);

정수 read_all_file_at (정수 dirfd, const char *path, char **out, size_t *len, libcrun_error_t *err);

정수 open_unix_domain_client_socket (const char *path, 정수 dgram, libcrun_error_t *err);

정수 open_unix_domain_socket (const char *path, 정수 dgram, libcrun_error_t *err);

정수 send_fd_to_socket (정수 server, 정수 fd, libcrun_error_t *err);

정수 send_fd_to_socket_with_payload (정수 server, 정수 fd, const char *payload, size_t payload_len, libcrun_error_t *err);

정수 create_socket_pair (정수 *pair, libcrun_error_t *err);

정수 receive_fd_from_socket (정수 from, libcrun_error_t *err);

정수 receive_fd_from_socket_with_payload (정수 from, char *payload, size_t payload_len, libcrun_error_t *err);

정수 create_signalfd (sigset_t *mask, libcrun_error_t *err);

정수 epoll_helper (정수 *in_fds, 정수 *in_levelfds, 정수 *out_fds, 정수 *out_levelfds, libcrun_error_t *err);

정수 copy_from_fd_to_fd (정수 src, 정수 dst, 정수 consume, libcrun_error_t *err);

정수 run_process (char **args, libcrun_error_t *err);

정수 format_default_id_mapping (char **out, uid_t container_id, uid_t host_uid, uid_t host_id, 정수 is_uid, libcrun_error_t *err);

정수 run_process_with_stdin_timeout_envp (char *path, char **args, const char *cwd, 정수 timeout, char **envp,
                                         char *stdin, size_t stdin_len, 정수 out_fd, 정수 err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

정수 mark_or_close_fds_ge_than (libcrun_container_t *container, 정수 n, bool close_now, libcrun_error_t *err);

void get_current_timestamp (char *out, size_t len);

정수 set_blocking_fd (정수 fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC 정수 parse_json_file (json_object **out, const char *jsondata, struct parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
static inline 정수
has_prefix (const char *str, const char *prefix)
{
  size_t prefix_len;

  만약 (! str || ! prefix)
    반환 0;

  prefix_len = strlen (prefix);
  반환 strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
static inline 정수
has_dir_prefix (const char *str, const char *prefix)
{
  size_t prefix_len;

  만약 (! str || ! prefix)
    반환 0;

  prefix_len = strlen (prefix);
  반환 strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

정수 find_executable (char **exec_path, const char *executable_path, const char *cwd, libcrun_error_t *err);

정수 copy_recursive_fd_to_fd (정수 srcfd, 정수 destfd, const char *srcname, const char *destname, libcrun_error_t *err);

정수 set_home_env (uid_t uid);

정수 libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

정수 libcrun_initialize_apparmor (libcrun_error_t *err);

const char *find_annotation (libcrun_container_t *container, const char *name);

정수 get_file_type_at (정수 dirfd, mode_t *mode, bool nofollow, const char *path);

정수 get_file_type (mode_t *mode, bool nofollow, const char *path);

정수 get_file_type_fd (정수 fd, mode_t *mode);

char *get_user_name (uid_t uid);

정수 safe_openat (정수 dirfd, const char *rootfs, const char *path, 정수 flags, 정수 mode,
                 libcrun_error_t *err);

정수 safe_write (정수 fd, const char *fname, const void *buf, size_t count, libcrun_error_t *err);

정수 append_paths (char **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

정수 str2sig (const char *name);

정수 base64_decode (const char *iptr, size_t isize, char *optr, size_t osize, size_t *nbytes);
정수 has_suffix (const char *source, const char *suffix);
char *libcrun_sysctl_key_to_proc_path (const char *key);
char *str_join_array (정수 offset, size_t size, char *const array[], const char *joint);

ssize_t safe_readlinkat (정수 dfd, const char *name, char **buffer, ssize_t hint, libcrun_error_t *err);

char **read_dir_entries (const char *path, libcrun_error_t *err);

static inline bool
is_empty_string (const char *s)
{
  반환 s == NULL || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
static inline size_t
safe_strlen (const char *s)
{
  반환 s == NULL ? 0 : strlen (s);
}

static inline bool
path_has_dot_dot_component (const char *path)
{
  만약 (path == NULL)
    반환 false;
  cleanup_free char *npath = xstrdup (path);
  char *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  동안 (cur)
    {
      만약 (it)
        *it = '\0';

      만약 (strcmp (cur, "..") == 0)
        반환 true;

      만약 (it == NULL)
        break;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  반환 false;
}

static inline 정수
waitpid_ignore_stopped (pid_t pid, 정수 *status, 정수 options)
{
  정수 ret, s;
  do
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      만약 (ret < 0)
        반환 ret;
  } 동안 (WIFSTOPPED (s) || WIFCONTINUED (s));

  만약 (status)
    *status = s;

  반환 ret;
}

static inline 정수
get_process_exit_status (정수 status)
{
  만약 (WIFEXITED (status))
    반환 WEXITSTATUS (status);
  만약 (WIFSIGNALED (status))
    반환 128 + WTERMSIG (status);

  반환 -1;
}

uid_t get_overflow_uid (void);
gid_t get_overflow_gid (void);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (sizeof (type) <= 1 ? 3U : sizeof (type) <= 2 ? 5U  \
                                        : sizeof (type) <= 4   ? 10U \
                                        : sizeof (type) <= 8   ? 20U \
                                                               : sizeof (정수[-2 * (sizeof (type) > 8)])))

#define _STRLEN(s) (sizeof (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (정수))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
typedef char proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

static inline void
get_proc_fd_path (proc_fd_path_t path, pid_t pid, 정수 fd)
{
  const size_t max_len = sizeof (proc_fd_path_t);
  size_t n;

  만약 (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  아니면
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  만약 (UNLIKELY (n >= max_len))
    abort ();
}

static inline void
get_proc_self_fd_path (proc_fd_path_t path, 정수 fd)
{
  get_proc_fd_path (path, 0, fd);
}

static inline void
get_self_fd_path (proc_fd_path_t path, 정수 fd)
{
  snprintf (path, sizeof (proc_fd_path_t), "self/fd/%d", fd);
}

static inline 정수
validate_options (unsigned 정수 specified_options, unsigned 정수 supported_options, libcrun_error_t *err)
{
  만약 (! ! (~supported_options & specified_options))
    반환 crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  반환 0;
}

extern 정수 cpuset_string_to_bitmask (const char *str, char **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
struct channel_fd_pair;

struct channel_fd_pair *channel_fd_pair_new (정수 in_fd, 정수 out_fd, size_t size);

void channel_fd_pair_free (struct channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
정수 channel_fd_pair_process (struct channel_fd_pair *channel, 정수 epollfd, libcrun_error_t *err);

static inline void
cleanup_channel_fd_pairp (void *p)
{
  struct channel_fd_pair **pp = (struct channel_fd_pair **) p;
  만약 (*pp == NULL)
    반환;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

정수 libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
정수 libcrun_open_proc_file (libcrun_container_t *container, const char *path, 정수 flags, libcrun_error_t *err);
정수 libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const char *path, 정수 flags, libcrun_error_t *err);

#endif
