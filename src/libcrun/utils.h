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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ <dirent.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ <sys/wait.h>
#ಅನ್ತರ್ಭಾವ "container.h"

#ifndef TEMP_FAILURE_RETRY
#  define TEMP_FAILURE_RETRY(expression)      \
    (__extension__ ({                         \
      long ಪೂರ್ಣಾಙ್ಕ __result;                      \
      ಕುರು                                      \
        __result = (long ಪೂರ್ಣಾಙ್ಕ) (expression);   \
      ಯಾವತ್ (__result < 0 && errno == EINTR); \
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

__attribute__ ((malloc)) ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ *
xmalloc (size_t size)
{
  ಶೂನ್ಯ *res = malloc (size);
  ಯದಿ (UNLIKELY (res == ರಿಕ್ತ))
    OOM ();
  ಪ್ರತ್ಯಾಗಮ res;
}

__attribute__ ((malloc)) ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ *
xmalloc0 (size_t size)
{
  ಶೂನ್ಯ *res = calloc (1, size);
  ಯದಿ (UNLIKELY (res == ರಿಕ್ತ))
    OOM ();
  ಪ್ರತ್ಯಾಗಮ res;
}

__attribute__ ((malloc)) ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ *
xrealloc (ಶೂನ್ಯ *ptr, size_t size)
{
  ಶೂನ್ಯ *res = realloc (ptr, size);
  ಯದಿ (UNLIKELY (res == ರಿಕ್ತ))
    OOM ();
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_freep (ಶೂನ್ಯ *p)
{
  ಶೂನ್ಯ **pp = (ಶೂನ್ಯ **) p;
  ಮುಕ್ತ (*pp);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  ಯದಿ (file)
    (ಶೂನ್ಯ) fclose (file);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_closep (ಶೂನ್ಯ *p)
{
  ಪೂರ್ಣಾಙ್ಕ *pp = (ಪೂರ್ಣಾಙ್ಕ *) p;
  ಯದಿ (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_pidp (ಶೂನ್ಯ *p)
{
  pid_t *pp = (pid_t *) p;
  ಯದಿ (*pp > 0)
    {
      TEMP_FAILURE_RETRY (kill (*pp, SIGKILL));
      TEMP_FAILURE_RETRY (waitpid (*pp, ರಿಕ್ತ, 0));
    }
}

ಸಂರಚನಾ libcrun_mmap_s
{
  ಶೂನ್ಯ *addr;
  size_t length;
};

ಪೂರ್ಣಾಙ್ಕ libcrun_mmap (ಸಂರಚನಾ libcrun_mmap_s **ret, ಶೂನ್ಯ *addr, size_t length,
                  ಪೂರ್ಣಾಙ್ಕ prot, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ fd, off_t offset,
                  libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_munmap (ಸಂರಚನಾ libcrun_mmap_s *mmap, libcrun_error_t *err);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_mmapp (ಶೂನ್ಯ *p)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  ಸಂರಚನಾ libcrun_mmap_s **mm;

  mm = (ಸಂರಚನಾ libcrun_mmap_s **) p;
  ಯದಿ (*mm == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ret = libcrun_munmap (*mm, &tmp_err);
  ಯದಿ (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

ಸಂರಚನಾ libcrun_fd_map
{
  size_t nfds;
  ಪೂರ್ಣಾಙ್ಕ fds[];
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  ಸಂರಚನಾ libcrun_fd_map *ret;
  size_t i;

  ret = (ಸಂರಚನಾ libcrun_fd_map *) xmalloc (ಪರಿಮಾಣ (*ret) + ಪರಿಮಾಣ (ಪೂರ್ಣಾಙ್ಕ) * len);
  ret->nfds = len;
  ಪ್ರತಿ (i = 0; i < len; i++)
    ret->fds[i] = -1;

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_close_mapp (ಸಂರಚನಾ libcrun_fd_map **p)
{
  ಸಂರಚನಾ libcrun_fd_map *m = *p;
  size_t i;

  ಯದಿ (m == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ಪ್ರತಿ (i = 0; i < m->nfds; i++)
    ಯದಿ (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  ಮುಕ್ತ (m);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_close_vecp (ಪೂರ್ಣಾಙ್ಕ **p)
{
  ಪೂರ್ಣಾಙ್ಕ *pp = *p;
  ಪೂರ್ಣಾಙ್ಕ i;

  ಯದಿ (pp == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ಪ್ರತಿ (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  ಯದಿ (dir)
    closedir (dir);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
close_and_reset (ಪೂರ್ಣಾಙ್ಕ *fd)
{
  ಪೂರ್ಣಾಙ್ಕ ret = 0;
  ಯದಿ (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      ಯದಿ (LIKELY (ret == 0))
        *fd = -1;
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ವರ್ಣ *
xstrdup (ಧ್ರುವ ವರ್ಣ *str)
{
  ವರ್ಣ *ret;
  ಯದಿ (str == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ret = strdup (str);
  ಯದಿ (ret == ರಿಕ್ತ)
    OOM ();

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಶೂನ್ಯ consume_trailing_slashes (ವರ್ಣ *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    ಯಾವತ್ (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
path_is_slash_dev (ಧ್ರುವ ವರ್ಣ *path)
{
  path = consume_slashes (path);

  ಯದಿ (strncmp (path, "dev", 3))
    ಪ್ರತ್ಯಾಗಮ false;

  path += 3;

  /* Check there are only '/' left.  */
  ಪ್ರತಿ (; *path; path++)
    ಯದಿ (*path != '/')
      ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತ್ಯಾಗಮ true;
}

ಪೂರ್ಣಾಙ್ಕ xasprintf (ವರ್ಣ **str, ಧ್ರುವ ವರ್ಣ *fmt, ...) __attribute__ ((format (printf, 2, 3)));

ಪೂರ್ಣಾಙ್ಕ crun_path_exists (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ write_file_at_with_flags (ಪೂರ್ಣಾಙ್ಕ dirfd, ಪೂರ್ಣಾಙ್ಕ flags, mode_t mode, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ಶೂನ್ಯ *data, size_t len, libcrun_error_t *err);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
write_file (ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ಶೂನ್ಯ *data, size_t len, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
write_file_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ಶೂನ್ಯ *data, size_t len, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

ಪೂರ್ಣಾಙ್ಕ crun_ensure_directory (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode, bool nofollow, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ crun_ensure_directory_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode, bool nofollow, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ crun_safe_create_and_open_ref_at (bool dir, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *dirpath, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ crun_safe_ensure_directory_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *dirpath, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode,
                                   libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ crun_safe_ensure_file_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *dirpath, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode,
                              libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ crun_dir_p (ಧ್ರುವ ವರ್ಣ *path, bool nofollow, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ crun_dir_p_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *path, bool nofollow, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ detach_process ();

ಪೂರ್ಣಾಙ್ಕ create_file_if_missing_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *file, mode_t mode, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ check_running_in_user_namespace (libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ set_selinux_label (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *label, bool now, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ add_selinux_mount_label (ವರ್ಣ **ret, ಧ್ರುವ ವರ್ಣ *data, ಧ್ರುವ ವರ್ಣ *label, ಧ್ರುವ ವರ್ಣ *context_type, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ set_apparmor_profile (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ read_all_fd_with_size_hint (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *description, ವರ್ಣ **out, size_t *len, size_t hint, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ read_all_fd_with_max_size (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *description, ವರ್ಣ **out, size_t *len, size_t max_size, libcrun_error_t *err);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
read_all_fd (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *description, ವರ್ಣ **out, size_t *len, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

ಪೂರ್ಣಾಙ್ಕ get_realpath_to_file (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *path_name, ವರ್ಣ **absolute_path, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ read_all_file (ಧ್ರುವ ವರ್ಣ *path, ವರ್ಣ **out, size_t *len, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ read_all_file_at (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *path, ವರ್ಣ **out, size_t *len, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ open_unix_domain_client_socket (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ dgram, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ open_unix_domain_socket (ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ dgram, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ send_fd_to_socket (ಪೂರ್ಣಾಙ್ಕ server, ಪೂರ್ಣಾಙ್ಕ fd, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ send_fd_to_socket_with_payload (ಪೂರ್ಣಾಙ್ಕ server, ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *payload, size_t payload_len, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ create_socket_pair (ಪೂರ್ಣಾಙ್ಕ *pair, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ receive_fd_from_socket (ಪೂರ್ಣಾಙ್ಕ from, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ receive_fd_from_socket_with_payload (ಪೂರ್ಣಾಙ್ಕ from, ವರ್ಣ *payload, size_t payload_len, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ create_signalfd (sigset_t *mask, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ epoll_helper (ಪೂರ್ಣಾಙ್ಕ *in_fds, ಪೂರ್ಣಾಙ್ಕ *in_levelfds, ಪೂರ್ಣಾಙ್ಕ *out_fds, ಪೂರ್ಣಾಙ್ಕ *out_levelfds, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ copy_from_fd_to_fd (ಪೂರ್ಣಾಙ್ಕ src, ಪೂರ್ಣಾಙ್ಕ dst, ಪೂರ್ಣಾಙ್ಕ consume, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ run_process (ವರ್ಣ **args, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ format_default_id_mapping (ವರ್ಣ **out, uid_t container_id, uid_t host_uid, uid_t host_id, ಪೂರ್ಣಾಙ್ಕ is_uid, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ run_process_with_stdin_timeout_envp (ವರ್ಣ *path, ವರ್ಣ **args, ಧ್ರುವ ವರ್ಣ *cwd, ಪೂರ್ಣಾಙ್ಕ timeout, ವರ್ಣ **envp,
                                         ವರ್ಣ *stdin, size_t stdin_len, ಪೂರ್ಣಾಙ್ಕ out_fd, ಪೂರ್ಣಾಙ್ಕ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ mark_or_close_fds_ge_than (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ n, bool close_now, libcrun_error_t *err);

ಶೂನ್ಯ get_current_timestamp (ವರ್ಣ *out, size_t len);

ಪೂರ್ಣಾಙ್ಕ set_blocking_fd (ಪೂರ್ಣಾಙ್ಕ fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ parse_json_file (json_object **out, ಧ್ರುವ ವರ್ಣ *jsondata, ಸಂರಚನಾ parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
has_prefix (ಧ್ರುವ ವರ್ಣ *str, ಧ್ರುವ ವರ್ಣ *prefix)
{
  size_t prefix_len;

  ಯದಿ (! str || ! prefix)
    ಪ್ರತ್ಯಾಗಮ 0;

  prefix_len = strlen (prefix);
  ಪ್ರತ್ಯಾಗಮ strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
has_dir_prefix (ಧ್ರುವ ವರ್ಣ *str, ಧ್ರುವ ವರ್ಣ *prefix)
{
  size_t prefix_len;

  ಯದಿ (! str || ! prefix)
    ಪ್ರತ್ಯಾಗಮ 0;

  prefix_len = strlen (prefix);
  ಪ್ರತ್ಯಾಗಮ strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

ಪೂರ್ಣಾಙ್ಕ find_executable (ವರ್ಣ **exec_path, ಧ್ರುವ ವರ್ಣ *executable_path, ಧ್ರುವ ವರ್ಣ *cwd, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ copy_recursive_fd_to_fd (ಪೂರ್ಣಾಙ್ಕ srcfd, ಪೂರ್ಣಾಙ್ಕ destfd, ಧ್ರುವ ವರ್ಣ *srcname, ಧ್ರುವ ವರ್ಣ *destname, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ set_home_env (uid_t uid);

ಪೂರ್ಣಾಙ್ಕ libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_initialize_apparmor (libcrun_error_t *err);

ಧ್ರುವ ವರ್ಣ *find_annotation (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *name);

ಪೂರ್ಣಾಙ್ಕ get_file_type_at (ಪೂರ್ಣಾಙ್ಕ dirfd, mode_t *mode, bool nofollow, ಧ್ರುವ ವರ್ಣ *path);

ಪೂರ್ಣಾಙ್ಕ get_file_type (mode_t *mode, bool nofollow, ಧ್ರುವ ವರ್ಣ *path);

ಪೂರ್ಣಾಙ್ಕ get_file_type_fd (ಪೂರ್ಣಾಙ್ಕ fd, mode_t *mode);

ವರ್ಣ *get_user_name (uid_t uid);

ಪೂರ್ಣಾಙ್ಕ safe_openat (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *rootfs, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ mode,
                 libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ safe_write (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *fname, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ append_paths (ವರ್ಣ **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

ಪೂರ್ಣಾಙ್ಕ str2sig (ಧ್ರುವ ವರ್ಣ *name);

ಪೂರ್ಣಾಙ್ಕ base64_decode (ಧ್ರುವ ವರ್ಣ *iptr, size_t isize, ವರ್ಣ *optr, size_t osize, size_t *nbytes);
ಪೂರ್ಣಾಙ್ಕ has_suffix (ಧ್ರುವ ವರ್ಣ *source, ಧ್ರುವ ವರ್ಣ *suffix);
ವರ್ಣ *libcrun_sysctl_ಕೀಲಿ_to_proc_path (ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ);
ವರ್ಣ *str_join_array (ಪೂರ್ಣಾಙ್ಕ offset, size_t size, ವರ್ಣ *ಧ್ರುವ array[], ಧ್ರುವ ವರ್ಣ *joint);

ssize_t safe_readlinkat (ಪೂರ್ಣಾಙ್ಕ dfd, ಧ್ರುವ ವರ್ಣ *name, ವರ್ಣ **buffer, ssize_t hint, libcrun_error_t *err);

ವರ್ಣ **read_dir_entries (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_empty_string (ಧ್ರುವ ವರ್ಣ *s)
{
  ಪ್ರತ್ಯಾಗಮ s == ರಿಕ್ತ || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t
safe_strlen (ಧ್ರುವ ವರ್ಣ *s)
{
  ಪ್ರತ್ಯಾಗಮ s == ರಿಕ್ತ ? 0 : strlen (s);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
path_has_dot_dot_component (ಧ್ರುವ ವರ್ಣ *path)
{
  ಯದಿ (path == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;
  cleanup_free ವರ್ಣ *npath = xstrdup (path);
  ವರ್ಣ *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  ಯಾವತ್ (cur)
    {
      ಯದಿ (it)
        *it = '\0';

      ಯದಿ (strcmp (cur, "..") == 0)
        ಪ್ರತ್ಯಾಗಮ true;

      ಯದಿ (it == ರಿಕ್ತ)
        ಭಙ್ಗ;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
waitpid_ignore_stopped (pid_t pid, ಪೂರ್ಣಾಙ್ಕ *status, ಪೂರ್ಣಾಙ್ಕ options)
{
  ಪೂರ್ಣಾಙ್ಕ ret, s;
  ಕುರು
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ ret;
  } ಯಾವತ್ (WIFSTOPPED (s) || WIFCONTINUED (s));

  ಯದಿ (status)
    *status = s;

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
get_process_exit_status (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (WIFEXITED (status))
    ಪ್ರತ್ಯಾಗಮ WEXITSTATUS (status);
  ಯದಿ (WIFSIGNALED (status))
    ಪ್ರತ್ಯಾಗಮ 128 + WTERMSIG (status);

  ಪ್ರತ್ಯಾಗಮ -1;
}

uid_t get_overflow_uid (ಶೂನ್ಯ);
gid_t get_overflow_gid (ಶೂನ್ಯ);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (ಪರಿಮಾಣ (type) <= 1 ? 3U : ಪರಿಮಾಣ (type) <= 2 ? 5U  \
                                        : ಪರಿಮಾಣ (type) <= 4   ? 10U \
                                        : ಪರಿಮಾಣ (type) <= 8   ? 20U \
                                                               : ಪರಿಮಾಣ (ಪೂರ್ಣಾಙ್ಕ[-2 * (ಪರಿಮಾಣ (type) > 8)])))

#define _STRLEN(s) (ಪರಿಮಾಣ (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (ಪೂರ್ಣಾಙ್ಕ))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ವರ್ಣ proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
get_proc_fd_path (proc_fd_path_t path, pid_t pid, ಪೂರ್ಣಾಙ್ಕ fd)
{
  ಧ್ರುವ size_t max_len = ಪರಿಮಾಣ (proc_fd_path_t);
  size_t n;

  ಯದಿ (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  ಅನ್ಯಥಾ
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  ಯದಿ (UNLIKELY (n >= max_len))
    abort ();
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
get_proc_self_fd_path (proc_fd_path_t path, ಪೂರ್ಣಾಙ್ಕ fd)
{
  get_proc_fd_path (path, 0, fd);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
get_self_fd_path (proc_fd_path_t path, ಪೂರ್ಣಾಙ್ಕ fd)
{
  snprintf (path, ಪರಿಮಾಣ (proc_fd_path_t), "self/fd/%d", fd);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
validate_options (unsigned ಪೂರ್ಣಾಙ್ಕ specified_options, unsigned ಪೂರ್ಣಾಙ್ಕ supported_options, libcrun_error_t *err)
{
  ಯದಿ (! ! (~supported_options & specified_options))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ cpuset_string_to_bitmask (ಧ್ರುವ ವರ್ಣ *str, ವರ್ಣ **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
ಸಂರಚನಾ channel_fd_pair;

ಸಂರಚನಾ channel_fd_pair *channel_fd_pair_new (ಪೂರ್ಣಾಙ್ಕ in_fd, ಪೂರ್ಣಾಙ್ಕ out_fd, size_t size);

ಶೂನ್ಯ channel_fd_pair_free (ಸಂರಚನಾ channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
ಪೂರ್ಣಾಙ್ಕ channel_fd_pair_process (ಸಂರಚನಾ channel_fd_pair *channel, ಪೂರ್ಣಾಙ್ಕ epollfd, libcrun_error_t *err);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_channel_fd_pairp (ಶೂನ್ಯ *p)
{
  ಸಂರಚನಾ channel_fd_pair **pp = (ಸಂರಚನಾ channel_fd_pair **) p;
  ಯದಿ (*pp == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

ಪೂರ್ಣಾಙ್ಕ libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_open_proc_file (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ flags, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ flags, libcrun_error_t *err);

#endif
