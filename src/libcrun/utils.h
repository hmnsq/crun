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
      long puur_naa_nka __result;                      \
      do                                      \
        __result = (long puur_naa_nka) (expression);   \
      ᮞᮨᮓᮨᮜ (__result < 0 && errno == EINTR); \
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
  ᮚᮦᮔ᮪ (UNLIKELY (res == NULL))
    OOM ();
  ᮝᮜᮤ res;
}

__attribute__ ((malloc)) static anta_hstha void *
xmalloc0 (size_t size)
{
  void *res = calloc (1, size);
  ᮚᮦᮔ᮪ (UNLIKELY (res == NULL))
    OOM ();
  ᮝᮜᮤ res;
}

__attribute__ ((malloc)) static anta_hstha void *
xrealloc (void *ptr, size_t size)
{
  void *res = realloc (ptr, size);
  ᮚᮦᮔ᮪ (UNLIKELY (res == NULL))
    OOM ();
  ᮝᮜᮤ res;
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
  ᮚᮦᮔ᮪ (file)
    (void) fclose (file);
}

static anta_hstha void
cleanup_closep (void *p)
{
  puur_naa_nka *pp = (puur_naa_nka *) p;
  ᮚᮦᮔ᮪ (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

static anta_hstha void
cleanup_pidp (void *p)
{
  pid_t *pp = (pid_t *) p;
  ᮚᮦᮔ᮪ (*pp > 0)
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

puur_naa_nka libcrun_mmap (sa_nracanaa libcrun_mmap_s **ret, void *addr, size_t length,
                  puur_naa_nka prot, puur_naa_nka flags, puur_naa_nka fd, off_t offset,
                  libcrun_error_t *err);

puur_naa_nka libcrun_munmap (sa_nracanaa libcrun_mmap_s *mmap, libcrun_error_t *err);

static anta_hstha void
cleanup_mmapp (void *p)
{
  puur_naa_nka ret;
  libcrun_error_t tmp_err = NULL;
  sa_nracanaa libcrun_mmap_s **mm;

  mm = (sa_nracanaa libcrun_mmap_s **) p;
  ᮚᮦᮔ᮪ (*mm == NULL)
    ᮝᮜᮤ;

  ret = libcrun_munmap (*mm, &tmp_err);
  ᮚᮦᮔ᮪ (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

sa_nracanaa libcrun_fd_map
{
  size_t nfds;
  puur_naa_nka fds[];
};

static anta_hstha sa_nracanaa libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  sa_nracanaa libcrun_fd_map *ret;
  size_t i;

  ret = (sa_nracanaa libcrun_fd_map *) xmalloc (parimaa_na (*ret) + parimaa_na (puur_naa_nka) * len);
  ret->nfds = len;
  for (i = 0; i < len; i++)
    ret->fds[i] = -1;

  ᮝᮜᮤ ret;
}

static anta_hstha void
cleanup_close_mapp (sa_nracanaa libcrun_fd_map **p)
{
  sa_nracanaa libcrun_fd_map *m = *p;
  size_t i;

  ᮚᮦᮔ᮪ (m == NULL)
    ᮝᮜᮤ;

  for (i = 0; i < m->nfds; i++)
    ᮚᮦᮔ᮪ (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  free (m);
}

static anta_hstha void
cleanup_close_vecp (puur_naa_nka **p)
{
  puur_naa_nka *pp = *p;
  puur_naa_nka i;

  ᮚᮦᮔ᮪ (pp == NULL)
    ᮝᮜᮤ;

  for (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

static anta_hstha void
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  ᮚᮦᮔ᮪ (dir)
    closedir (dir);
}

static anta_hstha puur_naa_nka
close_and_reset (puur_naa_nka *fd)
{
  puur_naa_nka ret = 0;
  ᮚᮦᮔ᮪ (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      ᮚᮦᮔ᮪ (LIKELY (ret == 0))
        *fd = -1;
    }
  ᮝᮜᮤ ret;
}

static anta_hstha var_na *
xstrdup (const var_na *str)
{
  var_na *ret;
  ᮚᮦᮔ᮪ (str == NULL)
    ᮝᮜᮤ NULL;

  ret = strdup (str);
  ᮚᮦᮔ᮪ (ret == NULL)
    OOM ();

  ᮝᮜᮤ ret;
}

void consume_trailing_slashes (var_na *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    ᮞᮨᮓᮨᮜ (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

static anta_hstha bool
path_is_slash_dev (const var_na *path)
{
  path = consume_slashes (path);

  ᮚᮦᮔ᮪ (strncmp (path, "dev", 3))
    ᮝᮜᮤ false;

  path += 3;

  /* Check there are only '/' left.  */
  for (; *path; path++)
    ᮚᮦᮔ᮪ (*path != '/')
      ᮝᮜᮤ false;

  ᮝᮜᮤ true;
}

puur_naa_nka xasprintf (var_na **str, const var_na *fmt, ...) __attribute__ ((format (printf, 2, 3)));

puur_naa_nka crun_path_exists (const var_na *path, libcrun_error_t *err);

puur_naa_nka write_file_at_with_flags (puur_naa_nka dirfd, puur_naa_nka flags, mode_t mode, const var_na *name, const void *data, size_t len, libcrun_error_t *err);

static anta_hstha puur_naa_nka
write_file (const var_na *name, const void *data, size_t len, libcrun_error_t *err)
{
  ᮝᮜᮤ write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

static anta_hstha puur_naa_nka
write_file_at (puur_naa_nka dirfd, const var_na *name, const void *data, size_t len, libcrun_error_t *err)
{
  ᮝᮜᮤ write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

puur_naa_nka crun_ensure_directory (const var_na *path, puur_naa_nka mode, bool nofollow, libcrun_error_t *err);

puur_naa_nka crun_ensure_directory_at (puur_naa_nka dirfd, const var_na *path, puur_naa_nka mode, bool nofollow, libcrun_error_t *err);

puur_naa_nka crun_safe_create_and_open_ref_at (bool dir, puur_naa_nka dirfd, const var_na *dirpath, const var_na *path, puur_naa_nka mode, libcrun_error_t *err);

puur_naa_nka crun_safe_ensure_directory_at (puur_naa_nka dirfd, const var_na *dirpath, const var_na *path, puur_naa_nka mode,
                                   libcrun_error_t *err);

puur_naa_nka crun_safe_ensure_file_at (puur_naa_nka dirfd, const var_na *dirpath, const var_na *path, puur_naa_nka mode,
                              libcrun_error_t *err);

puur_naa_nka crun_dir_p (const var_na *path, bool nofollow, libcrun_error_t *err);

puur_naa_nka crun_dir_p_at (puur_naa_nka dirfd, const var_na *path, bool nofollow, libcrun_error_t *err);

puur_naa_nka detach_process ();

puur_naa_nka create_file_if_missing_at (puur_naa_nka dirfd, const var_na *file, mode_t mode, libcrun_error_t *err);

puur_naa_nka check_running_in_user_namespace (libcrun_error_t *err);

puur_naa_nka set_selinux_label (libcrun_container_t *container, const var_na *label, bool now, libcrun_error_t *err);

puur_naa_nka add_selinux_mount_label (var_na **ret, const var_na *data, const var_na *label, const var_na *context_type, libcrun_error_t *err);

puur_naa_nka set_apparmor_profile (libcrun_container_t *container, const var_na *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

puur_naa_nka read_all_fd_with_size_hint (puur_naa_nka fd, const var_na *description, var_na **out, size_t *len, size_t hint, libcrun_error_t *err);

puur_naa_nka read_all_fd_with_max_size (puur_naa_nka fd, const var_na *description, var_na **out, size_t *len, size_t max_size, libcrun_error_t *err);

static anta_hstha puur_naa_nka
read_all_fd (puur_naa_nka fd, const var_na *description, var_na **out, size_t *len, libcrun_error_t *err)
{
  ᮝᮜᮤ read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

puur_naa_nka get_realpath_to_file (puur_naa_nka dirfd, const var_na *path_name, var_na **absolute_path, libcrun_error_t *err);

puur_naa_nka read_all_file (const var_na *path, var_na **out, size_t *len, libcrun_error_t *err);

puur_naa_nka read_all_file_at (puur_naa_nka dirfd, const var_na *path, var_na **out, size_t *len, libcrun_error_t *err);

puur_naa_nka open_unix_domain_client_socket (const var_na *path, puur_naa_nka dgram, libcrun_error_t *err);

puur_naa_nka open_unix_domain_socket (const var_na *path, puur_naa_nka dgram, libcrun_error_t *err);

puur_naa_nka send_fd_to_socket (puur_naa_nka server, puur_naa_nka fd, libcrun_error_t *err);

puur_naa_nka send_fd_to_socket_with_payload (puur_naa_nka server, puur_naa_nka fd, const var_na *payload, size_t payload_len, libcrun_error_t *err);

puur_naa_nka create_socket_pair (puur_naa_nka *pair, libcrun_error_t *err);

puur_naa_nka receive_fd_from_socket (puur_naa_nka from, libcrun_error_t *err);

puur_naa_nka receive_fd_from_socket_with_payload (puur_naa_nka from, var_na *payload, size_t payload_len, libcrun_error_t *err);

puur_naa_nka create_signalfd (sigset_t *mask, libcrun_error_t *err);

puur_naa_nka epoll_helper (puur_naa_nka *in_fds, puur_naa_nka *in_levelfds, puur_naa_nka *out_fds, puur_naa_nka *out_levelfds, libcrun_error_t *err);

puur_naa_nka copy_from_fd_to_fd (puur_naa_nka src, puur_naa_nka dst, puur_naa_nka consume, libcrun_error_t *err);

puur_naa_nka run_process (var_na **args, libcrun_error_t *err);

puur_naa_nka format_default_id_mapping (var_na **out, uid_t container_id, uid_t host_uid, uid_t host_id, puur_naa_nka is_uid, libcrun_error_t *err);

puur_naa_nka run_process_with_stdin_timeout_envp (var_na *path, var_na **args, const var_na *cwd, puur_naa_nka timeout, var_na **envp,
                                         var_na *stdin, size_t stdin_len, puur_naa_nka out_fd, puur_naa_nka err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

puur_naa_nka mark_or_close_fds_ge_than (libcrun_container_t *container, puur_naa_nka n, bool close_now, libcrun_error_t *err);

void get_current_timestamp (var_na *out, size_t len);

puur_naa_nka set_blocking_fd (puur_naa_nka fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC puur_naa_nka parse_json_file (json_object **out, const var_na *jsondata, sa_nracanaa parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
static anta_hstha puur_naa_nka
has_prefix (const var_na *str, const var_na *prefix)
{
  size_t prefix_len;

  ᮚᮦᮔ᮪ (! str || ! prefix)
    ᮝᮜᮤ 0;

  prefix_len = strlen (prefix);
  ᮝᮜᮤ strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
static anta_hstha puur_naa_nka
has_dir_prefix (const var_na *str, const var_na *prefix)
{
  size_t prefix_len;

  ᮚᮦᮔ᮪ (! str || ! prefix)
    ᮝᮜᮤ 0;

  prefix_len = strlen (prefix);
  ᮝᮜᮤ strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

puur_naa_nka find_executable (var_na **exec_path, const var_na *executable_path, const var_na *cwd, libcrun_error_t *err);

puur_naa_nka copy_recursive_fd_to_fd (puur_naa_nka srcfd, puur_naa_nka destfd, const var_na *srcname, const var_na *destname, libcrun_error_t *err);

puur_naa_nka set_home_env (uid_t uid);

puur_naa_nka libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

puur_naa_nka libcrun_initialize_apparmor (libcrun_error_t *err);

const var_na *find_annotation (libcrun_container_t *container, const var_na *name);

puur_naa_nka get_file_type_at (puur_naa_nka dirfd, mode_t *mode, bool nofollow, const var_na *path);

puur_naa_nka get_file_type (mode_t *mode, bool nofollow, const var_na *path);

puur_naa_nka get_file_type_fd (puur_naa_nka fd, mode_t *mode);

var_na *get_user_name (uid_t uid);

puur_naa_nka safe_openat (puur_naa_nka dirfd, const var_na *rootfs, const var_na *path, puur_naa_nka flags, puur_naa_nka mode,
                 libcrun_error_t *err);

puur_naa_nka safe_write (puur_naa_nka fd, const var_na *fname, const void *buf, size_t count, libcrun_error_t *err);

puur_naa_nka append_paths (var_na **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

puur_naa_nka str2sig (const var_na *name);

puur_naa_nka base64_decode (const var_na *iptr, size_t isize, var_na *optr, size_t osize, size_t *nbytes);
puur_naa_nka has_suffix (const var_na *source, const var_na *suffix);
var_na *libcrun_sysctl_key_to_proc_path (const var_na *key);
var_na *str_join_array (puur_naa_nka offset, size_t size, var_na *const array[], const var_na *joint);

ssize_t safe_readlinkat (puur_naa_nka dfd, const var_na *name, var_na **buffer, ssize_t hint, libcrun_error_t *err);

var_na **read_dir_entries (const var_na *path, libcrun_error_t *err);

static anta_hstha bool
is_empty_string (const var_na *s)
{
  ᮝᮜᮤ s == NULL || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
static anta_hstha size_t
safe_strlen (const var_na *s)
{
  ᮝᮜᮤ s == NULL ? 0 : strlen (s);
}

static anta_hstha bool
path_has_dot_dot_component (const var_na *path)
{
  ᮚᮦᮔ᮪ (path == NULL)
    ᮝᮜᮤ false;
  cleanup_free var_na *npath = xstrdup (path);
  var_na *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  ᮞᮨᮓᮨᮜ (cur)
    {
      ᮚᮦᮔ᮪ (it)
        *it = '\0';

      ᮚᮦᮔ᮪ (strcmp (cur, "..") == 0)
        ᮝᮜᮤ true;

      ᮚᮦᮔ᮪ (it == NULL)
        bha_nga;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  ᮝᮜᮤ false;
}

static anta_hstha puur_naa_nka
waitpid_ignore_stopped (pid_t pid, puur_naa_nka *status, puur_naa_nka options)
{
  puur_naa_nka ret, s;
  do
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      ᮚᮦᮔ᮪ (ret < 0)
        ᮝᮜᮤ ret;
  } ᮞᮨᮓᮨᮜ (WIFSTOPPED (s) || WIFCONTINUED (s));

  ᮚᮦᮔ᮪ (status)
    *status = s;

  ᮝᮜᮤ ret;
}

static anta_hstha puur_naa_nka
get_process_exit_status (puur_naa_nka status)
{
  ᮚᮦᮔ᮪ (WIFEXITED (status))
    ᮝᮜᮤ WEXITSTATUS (status);
  ᮚᮦᮔ᮪ (WIFSIGNALED (status))
    ᮝᮜᮤ 128 + WTERMSIG (status);

  ᮝᮜᮤ -1;
}

uid_t get_overflow_uid (void);
gid_t get_overflow_gid (void);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (parimaa_na (type) <= 1 ? 3U : parimaa_na (type) <= 2 ? 5U  \
                                        : parimaa_na (type) <= 4   ? 10U \
                                        : parimaa_na (type) <= 8   ? 20U \
                                                               : parimaa_na (puur_naa_nka[-2 * (parimaa_na (type) > 8)])))

#define _STRLEN(s) (parimaa_na (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (puur_naa_nka))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
typedef var_na proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

static anta_hstha void
get_proc_fd_path (proc_fd_path_t path, pid_t pid, puur_naa_nka fd)
{
  const size_t max_len = parimaa_na (proc_fd_path_t);
  size_t n;

  ᮚᮦᮔ᮪ (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  ᮜᮤᮚ
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  ᮚᮦᮔ᮪ (UNLIKELY (n >= max_len))
    abort ();
}

static anta_hstha void
get_proc_self_fd_path (proc_fd_path_t path, puur_naa_nka fd)
{
  get_proc_fd_path (path, 0, fd);
}

static anta_hstha void
get_self_fd_path (proc_fd_path_t path, puur_naa_nka fd)
{
  snprintf (path, parimaa_na (proc_fd_path_t), "self/fd/%d", fd);
}

static anta_hstha puur_naa_nka
validate_options (unsigned puur_naa_nka specified_options, unsigned puur_naa_nka supported_options, libcrun_error_t *err)
{
  ᮚᮦᮔ᮪ (! ! (~supported_options & specified_options))
    ᮝᮜᮤ crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  ᮝᮜᮤ 0;
}

extern puur_naa_nka cpuset_string_to_bitmask (const var_na *str, var_na **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
sa_nracanaa channel_fd_pair;

sa_nracanaa channel_fd_pair *channel_fd_pair_new (puur_naa_nka in_fd, puur_naa_nka out_fd, size_t size);

void channel_fd_pair_free (sa_nracanaa channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
puur_naa_nka channel_fd_pair_process (sa_nracanaa channel_fd_pair *channel, puur_naa_nka epollfd, libcrun_error_t *err);

static anta_hstha void
cleanup_channel_fd_pairp (void *p)
{
  sa_nracanaa channel_fd_pair **pp = (sa_nracanaa channel_fd_pair **) p;
  ᮚᮦᮔ᮪ (*pp == NULL)
    ᮝᮜᮤ;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

puur_naa_nka libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
puur_naa_nka libcrun_open_proc_file (libcrun_container_t *container, const var_na *path, puur_naa_nka flags, libcrun_error_t *err);
puur_naa_nka libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const var_na *path, puur_naa_nka flags, libcrun_error_t *err);

#endif
