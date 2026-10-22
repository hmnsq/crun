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
      long цео __result;                      \
      do                                      \
        __result = (long цео) (expression);   \
      док (__result < 0 && errno == EINTR); \
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
  ако (UNLIKELY (res == NULL))
    OOM ();
  врати res;
}

__attribute__ ((malloc)) static inline void *
xmalloc0 (size_t size)
{
  void *res = calloc (1, size);
  ако (UNLIKELY (res == NULL))
    OOM ();
  врати res;
}

__attribute__ ((malloc)) static inline void *
xrealloc (void *ptr, size_t size)
{
  void *res = realloc (ptr, size);
  ако (UNLIKELY (res == NULL))
    OOM ();
  врати res;
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
  ако (file)
    (void) fclose (file);
}

static inline void
cleanup_closep (void *p)
{
  цео *pp = (цео *) p;
  ако (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

static inline void
cleanup_pidp (void *p)
{
  pid_t *pp = (pid_t *) p;
  ако (*pp > 0)
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

цео libcrun_mmap (struct libcrun_mmap_s **ret, void *addr, size_t length,
                  цео prot, цео flags, цео fd, off_t offset,
                  libcrun_error_t *err);

цео libcrun_munmap (struct libcrun_mmap_s *mmap, libcrun_error_t *err);

static inline void
cleanup_mmapp (void *p)
{
  цео ret;
  libcrun_error_t tmp_err = NULL;
  struct libcrun_mmap_s **mm;

  mm = (struct libcrun_mmap_s **) p;
  ако (*mm == NULL)
    врати;

  ret = libcrun_munmap (*mm, &tmp_err);
  ако (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

struct libcrun_fd_map
{
  size_t nfds;
  цео fds[];
};

static inline struct libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  struct libcrun_fd_map *ret;
  size_t i;

  ret = (struct libcrun_fd_map *) xmalloc (sizeof (*ret) + sizeof (цео) * len);
  ret->nfds = len;
  за (i = 0; i < len; i++)
    ret->fds[i] = -1;

  врати ret;
}

static inline void
cleanup_close_mapp (struct libcrun_fd_map **p)
{
  struct libcrun_fd_map *m = *p;
  size_t i;

  ако (m == NULL)
    врати;

  за (i = 0; i < m->nfds; i++)
    ако (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  free (m);
}

static inline void
cleanup_close_vecp (цео **p)
{
  цео *pp = *p;
  цео i;

  ако (pp == NULL)
    врати;

  за (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

static inline void
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  ако (dir)
    closedir (dir);
}

static inline цео
close_and_reset (цео *fd)
{
  цео ret = 0;
  ако (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      ако (LIKELY (ret == 0))
        *fd = -1;
    }
  врати ret;
}

static inline char *
xstrdup (const char *str)
{
  char *ret;
  ако (str == NULL)
    врати NULL;

  ret = strdup (str);
  ако (ret == NULL)
    OOM ();

  врати ret;
}

void consume_trailing_slashes (char *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    док (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

static inline bool
path_is_slash_dev (const char *path)
{
  path = consume_slashes (path);

  ако (strncmp (path, "dev", 3))
    врати false;

  path += 3;

  /* Check there are only '/' left.  */
  за (; *path; path++)
    ако (*path != '/')
      врати false;

  врати true;
}

цео xasprintf (char **str, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));

цео crun_path_exists (const char *path, libcrun_error_t *err);

цео write_file_at_with_flags (цео dirfd, цео flags, mode_t mode, const char *name, const void *data, size_t len, libcrun_error_t *err);

static inline цео
write_file (const char *name, const void *data, size_t len, libcrun_error_t *err)
{
  врати write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

static inline цео
write_file_at (цео dirfd, const char *name, const void *data, size_t len, libcrun_error_t *err)
{
  врати write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

цео crun_ensure_directory (const char *path, цео mode, bool nofollow, libcrun_error_t *err);

цео crun_ensure_directory_at (цео dirfd, const char *path, цео mode, bool nofollow, libcrun_error_t *err);

цео crun_safe_create_and_open_ref_at (bool dir, цео dirfd, const char *dirpath, const char *path, цео mode, libcrun_error_t *err);

цео crun_safe_ensure_directory_at (цео dirfd, const char *dirpath, const char *path, цео mode,
                                   libcrun_error_t *err);

цео crun_safe_ensure_file_at (цео dirfd, const char *dirpath, const char *path, цео mode,
                              libcrun_error_t *err);

цео crun_dir_p (const char *path, bool nofollow, libcrun_error_t *err);

цео crun_dir_p_at (цео dirfd, const char *path, bool nofollow, libcrun_error_t *err);

цео detach_process ();

цео create_file_if_missing_at (цео dirfd, const char *file, mode_t mode, libcrun_error_t *err);

цео check_running_in_user_namespace (libcrun_error_t *err);

цео set_selinux_label (libcrun_container_t *container, const char *label, bool now, libcrun_error_t *err);

цео add_selinux_mount_label (char **ret, const char *data, const char *label, const char *context_type, libcrun_error_t *err);

цео set_apparmor_profile (libcrun_container_t *container, const char *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

цео read_all_fd_with_size_hint (цео fd, const char *description, char **out, size_t *len, size_t hint, libcrun_error_t *err);

цео read_all_fd_with_max_size (цео fd, const char *description, char **out, size_t *len, size_t max_size, libcrun_error_t *err);

static inline цео
read_all_fd (цео fd, const char *description, char **out, size_t *len, libcrun_error_t *err)
{
  врати read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

цео get_realpath_to_file (цео dirfd, const char *path_name, char **absolute_path, libcrun_error_t *err);

цео read_all_file (const char *path, char **out, size_t *len, libcrun_error_t *err);

цео read_all_file_at (цео dirfd, const char *path, char **out, size_t *len, libcrun_error_t *err);

цео open_unix_domain_client_socket (const char *path, цео dgram, libcrun_error_t *err);

цео open_unix_domain_socket (const char *path, цео dgram, libcrun_error_t *err);

цео send_fd_to_socket (цео server, цео fd, libcrun_error_t *err);

цео send_fd_to_socket_with_payload (цео server, цео fd, const char *payload, size_t payload_len, libcrun_error_t *err);

цео create_socket_pair (цео *pair, libcrun_error_t *err);

цео receive_fd_from_socket (цео from, libcrun_error_t *err);

цео receive_fd_from_socket_with_payload (цео from, char *payload, size_t payload_len, libcrun_error_t *err);

цео create_signalfd (sigset_t *mask, libcrun_error_t *err);

цео epoll_helper (цео *in_fds, цео *in_levelfds, цео *out_fds, цео *out_levelfds, libcrun_error_t *err);

цео copy_from_fd_to_fd (цео src, цео dst, цео consume, libcrun_error_t *err);

цео run_process (char **args, libcrun_error_t *err);

цео format_default_id_mapping (char **out, uid_t container_id, uid_t host_uid, uid_t host_id, цео is_uid, libcrun_error_t *err);

цео run_process_with_stdin_timeout_envp (char *path, char **args, const char *cwd, цео timeout, char **envp,
                                         char *stdin, size_t stdin_len, цео out_fd, цео err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

цео mark_or_close_fds_ge_than (libcrun_container_t *container, цео n, bool close_now, libcrun_error_t *err);

void get_current_timestamp (char *out, size_t len);

цео set_blocking_fd (цео fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC цео parse_json_file (json_object **out, const char *jsondata, struct parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
static inline цео
has_prefix (const char *str, const char *prefix)
{
  size_t prefix_len;

  ако (! str || ! prefix)
    врати 0;

  prefix_len = strlen (prefix);
  врати strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
static inline цео
has_dir_prefix (const char *str, const char *prefix)
{
  size_t prefix_len;

  ако (! str || ! prefix)
    врати 0;

  prefix_len = strlen (prefix);
  врати strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

цео find_executable (char **exec_path, const char *executable_path, const char *cwd, libcrun_error_t *err);

цео copy_recursive_fd_to_fd (цео srcfd, цео destfd, const char *srcname, const char *destname, libcrun_error_t *err);

цео set_home_env (uid_t uid);

цео libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

цео libcrun_initialize_apparmor (libcrun_error_t *err);

const char *find_annotation (libcrun_container_t *container, const char *name);

цео get_file_type_at (цео dirfd, mode_t *mode, bool nofollow, const char *path);

цео get_file_type (mode_t *mode, bool nofollow, const char *path);

цео get_file_type_fd (цео fd, mode_t *mode);

char *get_user_name (uid_t uid);

цео safe_openat (цео dirfd, const char *rootfs, const char *path, цео flags, цео mode,
                 libcrun_error_t *err);

цео safe_write (цео fd, const char *fname, const void *buf, size_t count, libcrun_error_t *err);

цео append_paths (char **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

цео str2sig (const char *name);

цео base64_decode (const char *iptr, size_t isize, char *optr, size_t osize, size_t *nbytes);
цео has_suffix (const char *source, const char *suffix);
char *libcrun_sysctl_key_to_proc_path (const char *key);
char *str_join_array (цео offset, size_t size, char *const array[], const char *joint);

ssize_t safe_readlinkat (цео dfd, const char *name, char **buffer, ssize_t hint, libcrun_error_t *err);

char **read_dir_entries (const char *path, libcrun_error_t *err);

static inline bool
is_empty_string (const char *s)
{
  врати s == NULL || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
static inline size_t
safe_strlen (const char *s)
{
  врати s == NULL ? 0 : strlen (s);
}

static inline bool
path_has_dot_dot_component (const char *path)
{
  ако (path == NULL)
    врати false;
  cleanup_free char *npath = xstrdup (path);
  char *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  док (cur)
    {
      ако (it)
        *it = '\0';

      ако (strcmp (cur, "..") == 0)
        врати true;

      ако (it == NULL)
        break;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  врати false;
}

static inline цео
waitpid_ignore_stopped (pid_t pid, цео *status, цео options)
{
  цео ret, s;
  do
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      ако (ret < 0)
        врати ret;
  } док (WIFSTOPPED (s) || WIFCONTINUED (s));

  ако (status)
    *status = s;

  врати ret;
}

static inline цео
get_process_exit_status (цео status)
{
  ако (WIFEXITED (status))
    врати WEXITSTATUS (status);
  ако (WIFSIGNALED (status))
    врати 128 + WTERMSIG (status);

  врати -1;
}

uid_t get_overflow_uid (void);
gid_t get_overflow_gid (void);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (sizeof (type) <= 1 ? 3U : sizeof (type) <= 2 ? 5U  \
                                        : sizeof (type) <= 4   ? 10U \
                                        : sizeof (type) <= 8   ? 20U \
                                                               : sizeof (цео[-2 * (sizeof (type) > 8)])))

#define _STRLEN(s) (sizeof (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (цео))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
typedef char proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

static inline void
get_proc_fd_path (proc_fd_path_t path, pid_t pid, цео fd)
{
  const size_t max_len = sizeof (proc_fd_path_t);
  size_t n;

  ако (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  инако
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  ако (UNLIKELY (n >= max_len))
    abort ();
}

static inline void
get_proc_self_fd_path (proc_fd_path_t path, цео fd)
{
  get_proc_fd_path (path, 0, fd);
}

static inline void
get_self_fd_path (proc_fd_path_t path, цео fd)
{
  snprintf (path, sizeof (proc_fd_path_t), "self/fd/%d", fd);
}

static inline цео
validate_options (unsigned цео specified_options, unsigned цео supported_options, libcrun_error_t *err)
{
  ако (! ! (~supported_options & specified_options))
    врати crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  врати 0;
}

extern цео cpuset_string_to_bitmask (const char *str, char **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
struct channel_fd_pair;

struct channel_fd_pair *channel_fd_pair_new (цео in_fd, цео out_fd, size_t size);

void channel_fd_pair_free (struct channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
цео channel_fd_pair_process (struct channel_fd_pair *channel, цео epollfd, libcrun_error_t *err);

static inline void
cleanup_channel_fd_pairp (void *p)
{
  struct channel_fd_pair **pp = (struct channel_fd_pair **) p;
  ако (*pp == NULL)
    врати;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

цео libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
цео libcrun_open_proc_file (libcrun_container_t *container, const char *path, цео flags, libcrun_error_t *err);
цео libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const char *path, цео flags, libcrun_error_t *err);

#endif
