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
      long ຈຳນວນເຕັມ __result;                      \
      do                                      \
        __result = (long ຈຳນວນເຕັມ) (expression);   \
      ໃນຂະນະທີ່ (__result < 0 && errno == EINTR); \
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
  ຖ້າ (UNLIKELY (res == NULL))
    OOM ();
  ສົ່ງຄືນ res;
}

__attribute__ ((malloc)) static anta_hstha void *
xmalloc0 (size_t size)
{
  void *res = calloc (1, size);
  ຖ້າ (UNLIKELY (res == NULL))
    OOM ();
  ສົ່ງຄືນ res;
}

__attribute__ ((malloc)) static anta_hstha void *
xrealloc (void *ptr, size_t size)
{
  void *res = realloc (ptr, size);
  ຖ້າ (UNLIKELY (res == NULL))
    OOM ();
  ສົ່ງຄືນ res;
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
  ຖ້າ (file)
    (void) fclose (file);
}

static anta_hstha void
cleanup_closep (void *p)
{
  ຈຳນວນເຕັມ *pp = (ຈຳນວນເຕັມ *) p;
  ຖ້າ (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

static anta_hstha void
cleanup_pidp (void *p)
{
  pid_t *pp = (pid_t *) p;
  ຖ້າ (*pp > 0)
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

ຈຳນວນເຕັມ libcrun_mmap (sa_nracanaa libcrun_mmap_s **ret, void *addr, size_t length,
                  ຈຳນວນເຕັມ prot, ຈຳນວນເຕັມ flags, ຈຳນວນເຕັມ fd, off_t offset,
                  libcrun_error_t *err);

ຈຳນວນເຕັມ libcrun_munmap (sa_nracanaa libcrun_mmap_s *mmap, libcrun_error_t *err);

static anta_hstha void
cleanup_mmapp (void *p)
{
  ຈຳນວນເຕັມ ret;
  libcrun_error_t tmp_err = NULL;
  sa_nracanaa libcrun_mmap_s **mm;

  mm = (sa_nracanaa libcrun_mmap_s **) p;
  ຖ້າ (*mm == NULL)
    ສົ່ງຄືນ;

  ret = libcrun_munmap (*mm, &tmp_err);
  ຖ້າ (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

sa_nracanaa libcrun_fd_map
{
  size_t nfds;
  ຈຳນວນເຕັມ fds[];
};

static anta_hstha sa_nracanaa libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  sa_nracanaa libcrun_fd_map *ret;
  size_t i;

  ret = (sa_nracanaa libcrun_fd_map *) xmalloc (parimaa_na (*ret) + parimaa_na (ຈຳນວນເຕັມ) * len);
  ret->nfds = len;
  ສຳລັບ (i = 0; i < len; i++)
    ret->fds[i] = -1;

  ສົ່ງຄືນ ret;
}

static anta_hstha void
cleanup_close_mapp (sa_nracanaa libcrun_fd_map **p)
{
  sa_nracanaa libcrun_fd_map *m = *p;
  size_t i;

  ຖ້າ (m == NULL)
    ສົ່ງຄືນ;

  ສຳລັບ (i = 0; i < m->nfds; i++)
    ຖ້າ (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  free (m);
}

static anta_hstha void
cleanup_close_vecp (ຈຳນວນເຕັມ **p)
{
  ຈຳນວນເຕັມ *pp = *p;
  ຈຳນວນເຕັມ i;

  ຖ້າ (pp == NULL)
    ສົ່ງຄືນ;

  ສຳລັບ (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

static anta_hstha void
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  ຖ້າ (dir)
    closedir (dir);
}

static anta_hstha ຈຳນວນເຕັມ
close_and_reset (ຈຳນວນເຕັມ *fd)
{
  ຈຳນວນເຕັມ ret = 0;
  ຖ້າ (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      ຖ້າ (LIKELY (ret == 0))
        *fd = -1;
    }
  ສົ່ງຄືນ ret;
}

static anta_hstha var_na *
xstrdup (const var_na *str)
{
  var_na *ret;
  ຖ້າ (str == NULL)
    ສົ່ງຄືນ NULL;

  ret = strdup (str);
  ຖ້າ (ret == NULL)
    OOM ();

  ສົ່ງຄືນ ret;
}

void consume_trailing_slashes (var_na *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    ໃນຂະນະທີ່ (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

static anta_hstha bool
path_is_slash_dev (const var_na *path)
{
  path = consume_slashes (path);

  ຖ້າ (strncmp (path, "dev", 3))
    ສົ່ງຄືນ false;

  path += 3;

  /* Check there are only '/' left.  */
  ສຳລັບ (; *path; path++)
    ຖ້າ (*path != '/')
      ສົ່ງຄືນ false;

  ສົ່ງຄືນ true;
}

ຈຳນວນເຕັມ xasprintf (var_na **str, const var_na *fmt, ...) __attribute__ ((format (printf, 2, 3)));

ຈຳນວນເຕັມ crun_path_exists (const var_na *path, libcrun_error_t *err);

ຈຳນວນເຕັມ write_file_at_with_flags (ຈຳນວນເຕັມ dirfd, ຈຳນວນເຕັມ flags, mode_t mode, const var_na *name, const void *data, size_t len, libcrun_error_t *err);

static anta_hstha ຈຳນວນເຕັມ
write_file (const var_na *name, const void *data, size_t len, libcrun_error_t *err)
{
  ສົ່ງຄືນ write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

static anta_hstha ຈຳນວນເຕັມ
write_file_at (ຈຳນວນເຕັມ dirfd, const var_na *name, const void *data, size_t len, libcrun_error_t *err)
{
  ສົ່ງຄືນ write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

ຈຳນວນເຕັມ crun_ensure_directory (const var_na *path, ຈຳນວນເຕັມ mode, bool nofollow, libcrun_error_t *err);

ຈຳນວນເຕັມ crun_ensure_directory_at (ຈຳນວນເຕັມ dirfd, const var_na *path, ຈຳນວນເຕັມ mode, bool nofollow, libcrun_error_t *err);

ຈຳນວນເຕັມ crun_safe_create_and_open_ref_at (bool dir, ຈຳນວນເຕັມ dirfd, const var_na *dirpath, const var_na *path, ຈຳນວນເຕັມ mode, libcrun_error_t *err);

ຈຳນວນເຕັມ crun_safe_ensure_directory_at (ຈຳນວນເຕັມ dirfd, const var_na *dirpath, const var_na *path, ຈຳນວນເຕັມ mode,
                                   libcrun_error_t *err);

ຈຳນວນເຕັມ crun_safe_ensure_file_at (ຈຳນວນເຕັມ dirfd, const var_na *dirpath, const var_na *path, ຈຳນວນເຕັມ mode,
                              libcrun_error_t *err);

ຈຳນວນເຕັມ crun_dir_p (const var_na *path, bool nofollow, libcrun_error_t *err);

ຈຳນວນເຕັມ crun_dir_p_at (ຈຳນວນເຕັມ dirfd, const var_na *path, bool nofollow, libcrun_error_t *err);

ຈຳນວນເຕັມ detach_process ();

ຈຳນວນເຕັມ create_file_if_missing_at (ຈຳນວນເຕັມ dirfd, const var_na *file, mode_t mode, libcrun_error_t *err);

ຈຳນວນເຕັມ check_running_in_user_namespace (libcrun_error_t *err);

ຈຳນວນເຕັມ set_selinux_label (libcrun_container_t *container, const var_na *label, bool now, libcrun_error_t *err);

ຈຳນວນເຕັມ add_selinux_mount_label (var_na **ret, const var_na *data, const var_na *label, const var_na *context_type, libcrun_error_t *err);

ຈຳນວນເຕັມ set_apparmor_profile (libcrun_container_t *container, const var_na *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

ຈຳນວນເຕັມ read_all_fd_with_size_hint (ຈຳນວນເຕັມ fd, const var_na *description, var_na **out, size_t *len, size_t hint, libcrun_error_t *err);

ຈຳນວນເຕັມ read_all_fd_with_max_size (ຈຳນວນເຕັມ fd, const var_na *description, var_na **out, size_t *len, size_t max_size, libcrun_error_t *err);

static anta_hstha ຈຳນວນເຕັມ
read_all_fd (ຈຳນວນເຕັມ fd, const var_na *description, var_na **out, size_t *len, libcrun_error_t *err)
{
  ສົ່ງຄືນ read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

ຈຳນວນເຕັມ get_realpath_to_file (ຈຳນວນເຕັມ dirfd, const var_na *path_name, var_na **absolute_path, libcrun_error_t *err);

ຈຳນວນເຕັມ read_all_file (const var_na *path, var_na **out, size_t *len, libcrun_error_t *err);

ຈຳນວນເຕັມ read_all_file_at (ຈຳນວນເຕັມ dirfd, const var_na *path, var_na **out, size_t *len, libcrun_error_t *err);

ຈຳນວນເຕັມ open_unix_domain_client_socket (const var_na *path, ຈຳນວນເຕັມ dgram, libcrun_error_t *err);

ຈຳນວນເຕັມ open_unix_domain_socket (const var_na *path, ຈຳນວນເຕັມ dgram, libcrun_error_t *err);

ຈຳນວນເຕັມ send_fd_to_socket (ຈຳນວນເຕັມ server, ຈຳນວນເຕັມ fd, libcrun_error_t *err);

ຈຳນວນເຕັມ send_fd_to_socket_with_payload (ຈຳນວນເຕັມ server, ຈຳນວນເຕັມ fd, const var_na *payload, size_t payload_len, libcrun_error_t *err);

ຈຳນວນເຕັມ create_socket_pair (ຈຳນວນເຕັມ *pair, libcrun_error_t *err);

ຈຳນວນເຕັມ receive_fd_from_socket (ຈຳນວນເຕັມ from, libcrun_error_t *err);

ຈຳນວນເຕັມ receive_fd_from_socket_with_payload (ຈຳນວນເຕັມ from, var_na *payload, size_t payload_len, libcrun_error_t *err);

ຈຳນວນເຕັມ create_signalfd (sigset_t *mask, libcrun_error_t *err);

ຈຳນວນເຕັມ epoll_helper (ຈຳນວນເຕັມ *in_fds, ຈຳນວນເຕັມ *in_levelfds, ຈຳນວນເຕັມ *out_fds, ຈຳນວນເຕັມ *out_levelfds, libcrun_error_t *err);

ຈຳນວນເຕັມ copy_from_fd_to_fd (ຈຳນວນເຕັມ src, ຈຳນວນເຕັມ dst, ຈຳນວນເຕັມ consume, libcrun_error_t *err);

ຈຳນວນເຕັມ run_process (var_na **args, libcrun_error_t *err);

ຈຳນວນເຕັມ format_default_id_mapping (var_na **out, uid_t container_id, uid_t host_uid, uid_t host_id, ຈຳນວນເຕັມ is_uid, libcrun_error_t *err);

ຈຳນວນເຕັມ run_process_with_stdin_timeout_envp (var_na *path, var_na **args, const var_na *cwd, ຈຳນວນເຕັມ timeout, var_na **envp,
                                         var_na *stdin, size_t stdin_len, ຈຳນວນເຕັມ out_fd, ຈຳນວນເຕັມ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

ຈຳນວນເຕັມ mark_or_close_fds_ge_than (libcrun_container_t *container, ຈຳນວນເຕັມ n, bool close_now, libcrun_error_t *err);

void get_current_timestamp (var_na *out, size_t len);

ຈຳນວນເຕັມ set_blocking_fd (ຈຳນວນເຕັມ fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC ຈຳນວນເຕັມ parse_json_file (json_object **out, const var_na *jsondata, sa_nracanaa parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
static anta_hstha ຈຳນວນເຕັມ
has_prefix (const var_na *str, const var_na *prefix)
{
  size_t prefix_len;

  ຖ້າ (! str || ! prefix)
    ສົ່ງຄືນ 0;

  prefix_len = strlen (prefix);
  ສົ່ງຄືນ strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
static anta_hstha ຈຳນວນເຕັມ
has_dir_prefix (const var_na *str, const var_na *prefix)
{
  size_t prefix_len;

  ຖ້າ (! str || ! prefix)
    ສົ່ງຄືນ 0;

  prefix_len = strlen (prefix);
  ສົ່ງຄືນ strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

ຈຳນວນເຕັມ find_executable (var_na **exec_path, const var_na *executable_path, const var_na *cwd, libcrun_error_t *err);

ຈຳນວນເຕັມ copy_recursive_fd_to_fd (ຈຳນວນເຕັມ srcfd, ຈຳນວນເຕັມ destfd, const var_na *srcname, const var_na *destname, libcrun_error_t *err);

ຈຳນວນເຕັມ set_home_env (uid_t uid);

ຈຳນວນເຕັມ libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

ຈຳນວນເຕັມ libcrun_initialize_apparmor (libcrun_error_t *err);

const var_na *find_annotation (libcrun_container_t *container, const var_na *name);

ຈຳນວນເຕັມ get_file_type_at (ຈຳນວນເຕັມ dirfd, mode_t *mode, bool nofollow, const var_na *path);

ຈຳນວນເຕັມ get_file_type (mode_t *mode, bool nofollow, const var_na *path);

ຈຳນວນເຕັມ get_file_type_fd (ຈຳນວນເຕັມ fd, mode_t *mode);

var_na *get_user_name (uid_t uid);

ຈຳນວນເຕັມ safe_openat (ຈຳນວນເຕັມ dirfd, const var_na *rootfs, const var_na *path, ຈຳນວນເຕັມ flags, ຈຳນວນເຕັມ mode,
                 libcrun_error_t *err);

ຈຳນວນເຕັມ safe_write (ຈຳນວນເຕັມ fd, const var_na *fname, const void *buf, size_t count, libcrun_error_t *err);

ຈຳນວນເຕັມ append_paths (var_na **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

ຈຳນວນເຕັມ str2sig (const var_na *name);

ຈຳນວນເຕັມ base64_decode (const var_na *iptr, size_t isize, var_na *optr, size_t osize, size_t *nbytes);
ຈຳນວນເຕັມ has_suffix (const var_na *source, const var_na *suffix);
var_na *libcrun_sysctl_key_to_proc_path (const var_na *key);
var_na *str_join_array (ຈຳນວນເຕັມ offset, size_t size, var_na *const array[], const var_na *joint);

ssize_t safe_readlinkat (ຈຳນວນເຕັມ dfd, const var_na *name, var_na **buffer, ssize_t hint, libcrun_error_t *err);

var_na **read_dir_entries (const var_na *path, libcrun_error_t *err);

static anta_hstha bool
is_empty_string (const var_na *s)
{
  ສົ່ງຄືນ s == NULL || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
static anta_hstha size_t
safe_strlen (const var_na *s)
{
  ສົ່ງຄືນ s == NULL ? 0 : strlen (s);
}

static anta_hstha bool
path_has_dot_dot_component (const var_na *path)
{
  ຖ້າ (path == NULL)
    ສົ່ງຄືນ false;
  cleanup_free var_na *npath = xstrdup (path);
  var_na *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  ໃນຂະນະທີ່ (cur)
    {
      ຖ້າ (it)
        *it = '\0';

      ຖ້າ (strcmp (cur, "..") == 0)
        ສົ່ງຄືນ true;

      ຖ້າ (it == NULL)
        bha_nga;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  ສົ່ງຄືນ false;
}

static anta_hstha ຈຳນວນເຕັມ
waitpid_ignore_stopped (pid_t pid, ຈຳນວນເຕັມ *status, ຈຳນວນເຕັມ options)
{
  ຈຳນວນເຕັມ ret, s;
  do
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      ຖ້າ (ret < 0)
        ສົ່ງຄືນ ret;
  } ໃນຂະນະທີ່ (WIFSTOPPED (s) || WIFCONTINUED (s));

  ຖ້າ (status)
    *status = s;

  ສົ່ງຄືນ ret;
}

static anta_hstha ຈຳນວນເຕັມ
get_process_exit_status (ຈຳນວນເຕັມ status)
{
  ຖ້າ (WIFEXITED (status))
    ສົ່ງຄືນ WEXITSTATUS (status);
  ຖ້າ (WIFSIGNALED (status))
    ສົ່ງຄືນ 128 + WTERMSIG (status);

  ສົ່ງຄືນ -1;
}

uid_t get_overflow_uid (void);
gid_t get_overflow_gid (void);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (parimaa_na (type) <= 1 ? 3U : parimaa_na (type) <= 2 ? 5U  \
                                        : parimaa_na (type) <= 4   ? 10U \
                                        : parimaa_na (type) <= 8   ? 20U \
                                                               : parimaa_na (ຈຳນວນເຕັມ[-2 * (parimaa_na (type) > 8)])))

#define _STRLEN(s) (parimaa_na (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (ຈຳນວນເຕັມ))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
typedef var_na proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

static anta_hstha void
get_proc_fd_path (proc_fd_path_t path, pid_t pid, ຈຳນວນເຕັມ fd)
{
  const size_t max_len = parimaa_na (proc_fd_path_t);
  size_t n;

  ຖ້າ (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  ບໍ່ດັ່ງນັ້ນ
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  ຖ້າ (UNLIKELY (n >= max_len))
    abort ();
}

static anta_hstha void
get_proc_self_fd_path (proc_fd_path_t path, ຈຳນວນເຕັມ fd)
{
  get_proc_fd_path (path, 0, fd);
}

static anta_hstha void
get_self_fd_path (proc_fd_path_t path, ຈຳນວນເຕັມ fd)
{
  snprintf (path, parimaa_na (proc_fd_path_t), "self/fd/%d", fd);
}

static anta_hstha ຈຳນວນເຕັມ
validate_options (unsigned ຈຳນວນເຕັມ specified_options, unsigned ຈຳນວນເຕັມ supported_options, libcrun_error_t *err)
{
  ຖ້າ (! ! (~supported_options & specified_options))
    ສົ່ງຄືນ crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  ສົ່ງຄືນ 0;
}

extern ຈຳນວນເຕັມ cpuset_string_to_bitmask (const var_na *str, var_na **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
sa_nracanaa channel_fd_pair;

sa_nracanaa channel_fd_pair *channel_fd_pair_new (ຈຳນວນເຕັມ in_fd, ຈຳນວນເຕັມ out_fd, size_t size);

void channel_fd_pair_free (sa_nracanaa channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
ຈຳນວນເຕັມ channel_fd_pair_process (sa_nracanaa channel_fd_pair *channel, ຈຳນວນເຕັມ epollfd, libcrun_error_t *err);

static anta_hstha void
cleanup_channel_fd_pairp (void *p)
{
  sa_nracanaa channel_fd_pair **pp = (sa_nracanaa channel_fd_pair **) p;
  ຖ້າ (*pp == NULL)
    ສົ່ງຄືນ;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

ຈຳນວນເຕັມ libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
ຈຳນວນເຕັມ libcrun_open_proc_file (libcrun_container_t *container, const var_na *path, ຈຳນວນເຕັມ flags, libcrun_error_t *err);
ຈຳນວນເຕັມ libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, const var_na *path, ຈຳນວນເຕັມ flags, libcrun_error_t *err);

#endif
