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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ "error.h"
#അന്തര്ഭാവ <dirent.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <ocispec/runtime_spec_schema_config_schema.h>
#അന്തര്ഭാവ <sys/wait.h>
#അന്തര്ഭാവ "container.h"

#ifndef TEMP_FAILURE_RETRY
#  define TEMP_FAILURE_RETRY(expression)      \
    (__extension__ ({                         \
      long പൂർണ്ണസംഖ്യ __result;                      \
      ചെയ്യുക                                      \
        __result = (long പൂർണ്ണസംഖ്യ) (expression);   \
      ആകുമ്പോൾ (__result < 0 && errno == EINTR); \
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

__attribute__ ((malloc)) സ്ഥിതം വരിയിൽ ശൂന്യം *
xmalloc (size_t size)
{
  ശൂന്യം *res = malloc (size);
  എങ്കിൽ (UNLIKELY (res == ശൂന്യ))
    OOM ();
  തിരികെ res;
}

__attribute__ ((malloc)) സ്ഥിതം വരിയിൽ ശൂന്യം *
xmalloc0 (size_t size)
{
  ശൂന്യം *res = calloc (1, size);
  എങ്കിൽ (UNLIKELY (res == ശൂന്യ))
    OOM ();
  തിരികെ res;
}

__attribute__ ((malloc)) സ്ഥിതം വരിയിൽ ശൂന്യം *
xrealloc (ശൂന്യം *ptr, size_t size)
{
  ശൂന്യം *res = realloc (ptr, size);
  എങ്കിൽ (UNLIKELY (res == ശൂന്യ))
    OOM ();
  തിരികെ res;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_freep (ശൂന്യം *p)
{
  ശൂന്യം **pp = (ശൂന്യം **) p;
  സ്വതന്ത്രം (*pp);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  എങ്കിൽ (file)
    (ശൂന്യം) fclose (file);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_closep (ശൂന്യം *p)
{
  പൂർണ്ണസംഖ്യ *pp = (പൂർണ്ണസംഖ്യ *) p;
  എങ്കിൽ (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_pidp (ശൂന്യം *p)
{
  pid_t *pp = (pid_t *) p;
  എങ്കിൽ (*pp > 0)
    {
      TEMP_FAILURE_RETRY (kill (*pp, SIGKILL));
      TEMP_FAILURE_RETRY (waitpid (*pp, ശൂന്യ, 0));
    }
}

ഘടന libcrun_mmap_s
{
  ശൂന്യം *addr;
  size_t length;
};

പൂർണ്ണസംഖ്യ libcrun_mmap (ഘടന libcrun_mmap_s **ret, ശൂന്യം *addr, size_t length,
                  പൂർണ്ണസംഖ്യ prot, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ fd, off_t offset,
                  libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_munmap (ഘടന libcrun_mmap_s *mmap, libcrun_error_t *err);

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_mmapp (ശൂന്യം *p)
{
  പൂർണ്ണസംഖ്യ ret;
  libcrun_error_t tmp_err = ശൂന്യ;
  ഘടന libcrun_mmap_s **mm;

  mm = (ഘടന libcrun_mmap_s **) p;
  എങ്കിൽ (*mm == ശൂന്യ)
    തിരികെ;

  ret = libcrun_munmap (*mm, &tmp_err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

ഘടന libcrun_fd_map
{
  size_t nfds;
  പൂർണ്ണസംഖ്യ fds[];
};

സ്ഥിതം വരിയിൽ ഘടന libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  ഘടന libcrun_fd_map *ret;
  size_t i;

  ret = (ഘടന libcrun_fd_map *) xmalloc (വലുപ്പം (*ret) + വലുപ്പം (പൂർണ്ണസംഖ്യ) * len);
  ret->nfds = len;
  വേണ്ടി (i = 0; i < len; i++)
    ret->fds[i] = -1;

  തിരികെ ret;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_close_mapp (ഘടന libcrun_fd_map **p)
{
  ഘടന libcrun_fd_map *m = *p;
  size_t i;

  എങ്കിൽ (m == ശൂന്യ)
    തിരികെ;

  വേണ്ടി (i = 0; i < m->nfds; i++)
    എങ്കിൽ (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  സ്വതന്ത്രം (m);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_close_vecp (പൂർണ്ണസംഖ്യ **p)
{
  പൂർണ്ണസംഖ്യ *pp = *p;
  പൂർണ്ണസംഖ്യ i;

  എങ്കിൽ (pp == ശൂന്യ)
    തിരികെ;

  വേണ്ടി (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  എങ്കിൽ (dir)
    closedir (dir);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
close_and_reset (പൂർണ്ണസംഖ്യ *fd)
{
  പൂർണ്ണസംഖ്യ ret = 0;
  എങ്കിൽ (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      എങ്കിൽ (LIKELY (ret == 0))
        *fd = -1;
    }
  തിരികെ ret;
}

സ്ഥിതം വരിയിൽ അക്ഷരം *
xstrdup (സ്ഥിരം അക്ഷരം *str)
{
  അക്ഷരം *ret;
  എങ്കിൽ (str == ശൂന്യ)
    തിരികെ ശൂന്യ;

  ret = strdup (str);
  എങ്കിൽ (ret == ശൂന്യ)
    OOM ();

  തിരികെ ret;
}

ശൂന്യം consume_trailing_slashes (അക്ഷരം *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    ആകുമ്പോൾ (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

സ്ഥിതം വരിയിൽ bool
path_is_slash_dev (സ്ഥിരം അക്ഷരം *path)
{
  path = consume_slashes (path);

  എങ്കിൽ (strncmp (path, "dev", 3))
    തിരികെ false;

  path += 3;

  /* Check there are only '/' left.  */
  വേണ്ടി (; *path; path++)
    എങ്കിൽ (*path != '/')
      തിരികെ false;

  തിരികെ true;
}

പൂർണ്ണസംഖ്യ xasprintf (അക്ഷരം **str, സ്ഥിരം അക്ഷരം *fmt, ...) __attribute__ ((format (printf, 2, 3)));

പൂർണ്ണസംഖ്യ crun_path_exists (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ write_file_at_with_flags (പൂർണ്ണസംഖ്യ dirfd, പൂർണ്ണസംഖ്യ flags, mode_t mode, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം ശൂന്യം *data, size_t len, libcrun_error_t *err);

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
write_file (സ്ഥിരം അക്ഷരം *name, സ്ഥിരം ശൂന്യം *data, size_t len, libcrun_error_t *err)
{
  തിരികെ write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
write_file_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം ശൂന്യം *data, size_t len, libcrun_error_t *err)
{
  തിരികെ write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

പൂർണ്ണസംഖ്യ crun_ensure_directory (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode, bool nofollow, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ crun_ensure_directory_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode, bool nofollow, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ crun_safe_create_and_open_ref_at (bool dir, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *dirpath, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ crun_safe_ensure_directory_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *dirpath, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode,
                                   libcrun_error_t *err);

പൂർണ്ണസംഖ്യ crun_safe_ensure_file_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *dirpath, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode,
                              libcrun_error_t *err);

പൂർണ്ണസംഖ്യ crun_dir_p (സ്ഥിരം അക്ഷരം *path, bool nofollow, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ crun_dir_p_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *path, bool nofollow, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ detach_process ();

പൂർണ്ണസംഖ്യ create_file_if_missing_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *file, mode_t mode, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ check_running_in_user_namespace (libcrun_error_t *err);

പൂർണ്ണസംഖ്യ set_selinux_label (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *label, bool now, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ add_selinux_mount_label (അക്ഷരം **ret, സ്ഥിരം അക്ഷരം *data, സ്ഥിരം അക്ഷരം *label, സ്ഥിരം അക്ഷരം *context_type, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ set_apparmor_profile (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ read_all_fd_with_size_hint (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *description, അക്ഷരം **out, size_t *len, size_t hint, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ read_all_fd_with_max_size (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *description, അക്ഷരം **out, size_t *len, size_t max_size, libcrun_error_t *err);

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
read_all_fd (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *description, അക്ഷരം **out, size_t *len, libcrun_error_t *err)
{
  തിരികെ read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

പൂർണ്ണസംഖ്യ get_realpath_to_file (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *path_name, അക്ഷരം **absolute_path, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ read_all_file (സ്ഥിരം അക്ഷരം *path, അക്ഷരം **out, size_t *len, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ read_all_file_at (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *path, അക്ഷരം **out, size_t *len, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ open_unix_domain_client_socket (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ dgram, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ open_unix_domain_socket (സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ dgram, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ send_fd_to_socket (പൂർണ്ണസംഖ്യ server, പൂർണ്ണസംഖ്യ fd, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ send_fd_to_socket_with_payload (പൂർണ്ണസംഖ്യ server, പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *payload, size_t payload_len, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ create_socket_pair (പൂർണ്ണസംഖ്യ *pair, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ receive_fd_from_socket (പൂർണ്ണസംഖ്യ from, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ receive_fd_from_socket_with_payload (പൂർണ്ണസംഖ്യ from, അക്ഷരം *payload, size_t payload_len, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ create_signalfd (sigset_t *mask, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ epoll_helper (പൂർണ്ണസംഖ്യ *in_fds, പൂർണ്ണസംഖ്യ *in_levelfds, പൂർണ്ണസംഖ്യ *out_fds, പൂർണ്ണസംഖ്യ *out_levelfds, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ copy_from_fd_to_fd (പൂർണ്ണസംഖ്യ src, പൂർണ്ണസംഖ്യ dst, പൂർണ്ണസംഖ്യ consume, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ run_process (അക്ഷരം **args, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ format_default_id_mapping (അക്ഷരം **out, uid_t container_id, uid_t host_uid, uid_t host_id, പൂർണ്ണസംഖ്യ is_uid, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ run_process_with_stdin_timeout_envp (അക്ഷരം *path, അക്ഷരം **args, സ്ഥിരം അക്ഷരം *cwd, പൂർണ്ണസംഖ്യ timeout, അക്ഷരം **envp,
                                         അക്ഷരം *stdin, size_t stdin_len, പൂർണ്ണസംഖ്യ out_fd, പൂർണ്ണസംഖ്യ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ mark_or_close_fds_ge_than (libcrun_container_t *container, പൂർണ്ണസംഖ്യ n, bool close_now, libcrun_error_t *err);

ശൂന്യം get_current_timestamp (അക്ഷരം *out, size_t len);

പൂർണ്ണസംഖ്യ set_blocking_fd (പൂർണ്ണസംഖ്യ fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ parse_json_file (json_object **out, സ്ഥിരം അക്ഷരം *jsondata, ഘടന parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
has_prefix (സ്ഥിരം അക്ഷരം *str, സ്ഥിരം അക്ഷരം *prefix)
{
  size_t prefix_len;

  എങ്കിൽ (! str || ! prefix)
    തിരികെ 0;

  prefix_len = strlen (prefix);
  തിരികെ strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
has_dir_prefix (സ്ഥിരം അക്ഷരം *str, സ്ഥിരം അക്ഷരം *prefix)
{
  size_t prefix_len;

  എങ്കിൽ (! str || ! prefix)
    തിരികെ 0;

  prefix_len = strlen (prefix);
  തിരികെ strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

പൂർണ്ണസംഖ്യ find_executable (അക്ഷരം **exec_path, സ്ഥിരം അക്ഷരം *executable_path, സ്ഥിരം അക്ഷരം *cwd, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ copy_recursive_fd_to_fd (പൂർണ്ണസംഖ്യ srcfd, പൂർണ്ണസംഖ്യ destfd, സ്ഥിരം അക്ഷരം *srcname, സ്ഥിരം അക്ഷരം *destname, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ set_home_env (uid_t uid);

പൂർണ്ണസംഖ്യ libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_initialize_apparmor (libcrun_error_t *err);

സ്ഥിരം അക്ഷരം *find_annotation (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *name);

പൂർണ്ണസംഖ്യ get_file_type_at (പൂർണ്ണസംഖ്യ dirfd, mode_t *mode, bool nofollow, സ്ഥിരം അക്ഷരം *path);

പൂർണ്ണസംഖ്യ get_file_type (mode_t *mode, bool nofollow, സ്ഥിരം അക്ഷരം *path);

പൂർണ്ണസംഖ്യ get_file_type_fd (പൂർണ്ണസംഖ്യ fd, mode_t *mode);

അക്ഷരം *get_user_name (uid_t uid);

പൂർണ്ണസംഖ്യ safe_openat (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *rootfs, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ mode,
                 libcrun_error_t *err);

പൂർണ്ണസംഖ്യ safe_write (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *fname, സ്ഥിരം ശൂന്യം *buf, size_t എണ്ണം, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ append_paths (അക്ഷരം **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

പൂർണ്ണസംഖ്യ str2sig (സ്ഥിരം അക്ഷരം *name);

പൂർണ്ണസംഖ്യ base64_decode (സ്ഥിരം അക്ഷരം *iptr, size_t isize, അക്ഷരം *optr, size_t osize, size_t *nbytes);
പൂർണ്ണസംഖ്യ has_suffix (സ്ഥിരം അക്ഷരം *source, സ്ഥിരം അക്ഷരം *suffix);
അക്ഷരം *libcrun_sysctl_താക്കോൽ_to_proc_path (സ്ഥിരം അക്ഷരം *താക്കോൽ);
അക്ഷരം *str_join_array (പൂർണ്ണസംഖ്യ offset, size_t size, അക്ഷരം *സ്ഥിരം array[], സ്ഥിരം അക്ഷരം *joint);

ssize_t safe_readlinkat (പൂർണ്ണസംഖ്യ dfd, സ്ഥിരം അക്ഷരം *name, അക്ഷരം **buffer, ssize_t hint, libcrun_error_t *err);

അക്ഷരം **read_dir_entries (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err);

സ്ഥിതം വരിയിൽ bool
is_empty_string (സ്ഥിരം അക്ഷരം *s)
{
  തിരികെ s == ശൂന്യ || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
സ്ഥിതം വരിയിൽ size_t
safe_strlen (സ്ഥിരം അക്ഷരം *s)
{
  തിരികെ s == ശൂന്യ ? 0 : strlen (s);
}

സ്ഥിതം വരിയിൽ bool
path_has_dot_dot_component (സ്ഥിരം അക്ഷരം *path)
{
  എങ്കിൽ (path == ശൂന്യ)
    തിരികെ false;
  cleanup_free അക്ഷരം *npath = xstrdup (path);
  അക്ഷരം *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  ആകുമ്പോൾ (cur)
    {
      എങ്കിൽ (it)
        *it = '\0';

      എങ്കിൽ (strcmp (cur, "..") == 0)
        തിരികെ true;

      എങ്കിൽ (it == ശൂന്യ)
        നിർത്തുക;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  തിരികെ false;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
waitpid_ignore_stopped (pid_t pid, പൂർണ്ണസംഖ്യ *status, പൂർണ്ണസംഖ്യ options)
{
  പൂർണ്ണസംഖ്യ ret, s;
  ചെയ്യുക
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      എങ്കിൽ (ret < 0)
        തിരികെ ret;
  } ആകുമ്പോൾ (WIFSTOPPED (s) || WIFCONTINUED (s));

  എങ്കിൽ (status)
    *status = s;

  തിരികെ ret;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
get_process_exit_status (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (WIFEXITED (status))
    തിരികെ WEXITSTATUS (status);
  എങ്കിൽ (WIFSIGNALED (status))
    തിരികെ 128 + WTERMSIG (status);

  തിരികെ -1;
}

uid_t get_overflow_uid (ശൂന്യം);
gid_t get_overflow_gid (ശൂന്യം);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (വലുപ്പം (type) <= 1 ? 3U : വലുപ്പം (type) <= 2 ? 5U  \
                                        : വലുപ്പം (type) <= 4   ? 10U \
                                        : വലുപ്പം (type) <= 8   ? 20U \
                                                               : വലുപ്പം (പൂർണ്ണസംഖ്യ[-2 * (വലുപ്പം (type) > 8)])))

#define _STRLEN(s) (വലുപ്പം (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (പൂർണ്ണസംഖ്യ))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
തരനിർവചനം അക്ഷരം proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

സ്ഥിതം വരിയിൽ ശൂന്യം
get_proc_fd_path (proc_fd_path_t path, pid_t pid, പൂർണ്ണസംഖ്യ fd)
{
  സ്ഥിരം size_t max_len = വലുപ്പം (proc_fd_path_t);
  size_t n;

  എങ്കിൽ (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  അല്ലെങ്കിൽ
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  എങ്കിൽ (UNLIKELY (n >= max_len))
    abort ();
}

സ്ഥിതം വരിയിൽ ശൂന്യം
get_proc_self_fd_path (proc_fd_path_t path, പൂർണ്ണസംഖ്യ fd)
{
  get_proc_fd_path (path, 0, fd);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
get_self_fd_path (proc_fd_path_t path, പൂർണ്ണസംഖ്യ fd)
{
  snprintf (path, വലുപ്പം (proc_fd_path_t), "self/fd/%d", fd);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
validate_options (unsigned പൂർണ്ണസംഖ്യ specified_options, unsigned പൂർണ്ണസംഖ്യ supported_options, libcrun_error_t *err)
{
  എങ്കിൽ (! ! (~supported_options & specified_options))
    തിരികെ crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  തിരികെ 0;
}

ബാഹ്യ പൂർണ്ണസംഖ്യ cpuset_string_to_bitmask (സ്ഥിരം അക്ഷരം *str, അക്ഷരം **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
ഘടന channel_fd_pair;

ഘടന channel_fd_pair *channel_fd_pair_new (പൂർണ്ണസംഖ്യ in_fd, പൂർണ്ണസംഖ്യ out_fd, size_t size);

ശൂന്യം channel_fd_pair_free (ഘടന channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
പൂർണ്ണസംഖ്യ channel_fd_pair_process (ഘടന channel_fd_pair *channel, പൂർണ്ണസംഖ്യ epollfd, libcrun_error_t *err);

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_channel_fd_pairp (ശൂന്യം *p)
{
  ഘടന channel_fd_pair **pp = (ഘടന channel_fd_pair **) p;
  എങ്കിൽ (*pp == ശൂന്യ)
    തിരികെ;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

പൂർണ്ണസംഖ്യ libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_open_proc_file (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ flags, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ flags, libcrun_error_t *err);

#endif
