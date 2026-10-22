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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব "error.h"
#অন্তর্ভাব <dirent.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <ocispec/runtime_spec_schema_config_schema.h>
#অন্তর্ভাব <sys/wait.h>
#অন্তর্ভাব "container.h"

#ifndef TEMP_FAILURE_RETRY
#  define TEMP_FAILURE_RETRY(expression)      \
    (__extension__ ({                         \
      long পূর্ণাঙ্ক __result;                      \
      কুরু                                      \
        __result = (long পূর্ণাঙ্ক) (expression);   \
      যাবত্ (__result < 0 && errno == EINTR); \
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

__attribute__ ((malloc)) স্থির অন্তঃস্থ শূন্য *
xmalloc (size_t size)
{
  শূন্য *res = malloc (size);
  যদি (UNLIKELY (res == রিক্ত))
    OOM ();
  প্রত্যাগম res;
}

__attribute__ ((malloc)) স্থির অন্তঃস্থ শূন্য *
xmalloc0 (size_t size)
{
  শূন্য *res = calloc (1, size);
  যদি (UNLIKELY (res == রিক্ত))
    OOM ();
  প্রত্যাগম res;
}

__attribute__ ((malloc)) স্থির অন্তঃস্থ শূন্য *
xrealloc (শূন্য *ptr, size_t size)
{
  শূন্য *res = realloc (ptr, size);
  যদি (UNLIKELY (res == রিক্ত))
    OOM ();
  প্রত্যাগম res;
}

স্থির অন্তঃস্থ শূন্য
cleanup_freep (শূন্য *p)
{
  শূন্য **pp = (শূন্য **) p;
  মুক্ত (*pp);
}

স্থির অন্তঃস্থ শূন্য
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  যদি (file)
    (শূন্য) fclose (file);
}

স্থির অন্তঃস্থ শূন্য
cleanup_closep (শূন্য *p)
{
  পূর্ণাঙ্ক *pp = (পূর্ণাঙ্ক *) p;
  যদি (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

স্থির অন্তঃস্থ শূন্য
cleanup_pidp (শূন্য *p)
{
  pid_t *pp = (pid_t *) p;
  যদি (*pp > 0)
    {
      TEMP_FAILURE_RETRY (kill (*pp, SIGKILL));
      TEMP_FAILURE_RETRY (waitpid (*pp, রিক্ত, 0));
    }
}

সংরচনা libcrun_mmap_s
{
  শূন্য *addr;
  size_t length;
};

পূর্ণাঙ্ক libcrun_mmap (সংরচনা libcrun_mmap_s **ret, শূন্য *addr, size_t length,
                  পূর্ণাঙ্ক prot, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক fd, off_t offset,
                  libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_munmap (সংরচনা libcrun_mmap_s *mmap, libcrun_error_t *err);

স্থির অন্তঃস্থ শূন্য
cleanup_mmapp (শূন্য *p)
{
  পূর্ণাঙ্ক ret;
  libcrun_error_t tmp_err = রিক্ত;
  সংরচনা libcrun_mmap_s **mm;

  mm = (সংরচনা libcrun_mmap_s **) p;
  যদি (*mm == রিক্ত)
    প্রত্যাগম;

  ret = libcrun_munmap (*mm, &tmp_err);
  যদি (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

সংরচনা libcrun_fd_map
{
  size_t nfds;
  পূর্ণাঙ্ক fds[];
};

স্থির অন্তঃস্থ সংরচনা libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  সংরচনা libcrun_fd_map *ret;
  size_t i;

  ret = (সংরচনা libcrun_fd_map *) xmalloc (পরিমাণ (*ret) + পরিমাণ (পূর্ণাঙ্ক) * len);
  ret->nfds = len;
  প্রতি (i = 0; i < len; i++)
    ret->fds[i] = -1;

  প্রত্যাগম ret;
}

স্থির অন্তঃস্থ শূন্য
cleanup_close_mapp (সংরচনা libcrun_fd_map **p)
{
  সংরচনা libcrun_fd_map *m = *p;
  size_t i;

  যদি (m == রিক্ত)
    প্রত্যাগম;

  প্রতি (i = 0; i < m->nfds; i++)
    যদি (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  মুক্ত (m);
}

স্থির অন্তঃস্থ শূন্য
cleanup_close_vecp (পূর্ণাঙ্ক **p)
{
  পূর্ণাঙ্ক *pp = *p;
  পূর্ণাঙ্ক i;

  যদি (pp == রিক্ত)
    প্রত্যাগম;

  প্রতি (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

স্থির অন্তঃস্থ শূন্য
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  যদি (dir)
    closedir (dir);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
close_and_reset (পূর্ণাঙ্ক *fd)
{
  পূর্ণাঙ্ক ret = 0;
  যদি (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      যদি (LIKELY (ret == 0))
        *fd = -1;
    }
  প্রত্যাগম ret;
}

স্থির অন্তঃস্থ বর্ণ *
xstrdup (ধ্রুব বর্ণ *str)
{
  বর্ণ *ret;
  যদি (str == রিক্ত)
    প্রত্যাগম রিক্ত;

  ret = strdup (str);
  যদি (ret == রিক্ত)
    OOM ();

  প্রত্যাগম ret;
}

শূন্য consume_trailing_slashes (বর্ণ *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    যাবত্ (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

স্থির অন্তঃস্থ bool
path_is_slash_dev (ধ্রুব বর্ণ *path)
{
  path = consume_slashes (path);

  যদি (strncmp (path, "dev", 3))
    প্রত্যাগম false;

  path += 3;

  /* Check there are only '/' left.  */
  প্রতি (; *path; path++)
    যদি (*path != '/')
      প্রত্যাগম false;

  প্রত্যাগম true;
}

পূর্ণাঙ্ক xasprintf (বর্ণ **str, ধ্রুব বর্ণ *fmt, ...) __attribute__ ((format (printf, 2, 3)));

পূর্ণাঙ্ক crun_path_exists (ধ্রুব বর্ণ *path, libcrun_error_t *err);

পূর্ণাঙ্ক write_file_at_with_flags (পূর্ণাঙ্ক dirfd, পূর্ণাঙ্ক flags, mode_t mode, ধ্রুব বর্ণ *name, ধ্রুব শূন্য *data, size_t len, libcrun_error_t *err);

স্থির অন্তঃস্থ পূর্ণাঙ্ক
write_file (ধ্রুব বর্ণ *name, ধ্রুব শূন্য *data, size_t len, libcrun_error_t *err)
{
  প্রত্যাগম write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
write_file_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *name, ধ্রুব শূন্য *data, size_t len, libcrun_error_t *err)
{
  প্রত্যাগম write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

পূর্ণাঙ্ক crun_ensure_directory (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode, bool nofollow, libcrun_error_t *err);

পূর্ণাঙ্ক crun_ensure_directory_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode, bool nofollow, libcrun_error_t *err);

পূর্ণাঙ্ক crun_safe_create_and_open_ref_at (bool dir, পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *dirpath, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode, libcrun_error_t *err);

পূর্ণাঙ্ক crun_safe_ensure_directory_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *dirpath, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode,
                                   libcrun_error_t *err);

পূর্ণাঙ্ক crun_safe_ensure_file_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *dirpath, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode,
                              libcrun_error_t *err);

পূর্ণাঙ্ক crun_dir_p (ধ্রুব বর্ণ *path, bool nofollow, libcrun_error_t *err);

পূর্ণাঙ্ক crun_dir_p_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *path, bool nofollow, libcrun_error_t *err);

পূর্ণাঙ্ক detach_process ();

পূর্ণাঙ্ক create_file_if_missing_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *file, mode_t mode, libcrun_error_t *err);

পূর্ণাঙ্ক check_running_in_user_namespace (libcrun_error_t *err);

পূর্ণাঙ্ক set_selinux_label (libcrun_container_t *container, ধ্রুব বর্ণ *label, bool now, libcrun_error_t *err);

পূর্ণাঙ্ক add_selinux_mount_label (বর্ণ **ret, ধ্রুব বর্ণ *data, ধ্রুব বর্ণ *label, ধ্রুব বর্ণ *context_type, libcrun_error_t *err);

পূর্ণাঙ্ক set_apparmor_profile (libcrun_container_t *container, ধ্রুব বর্ণ *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

পূর্ণাঙ্ক read_all_fd_with_size_hint (পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *description, বর্ণ **out, size_t *len, size_t hint, libcrun_error_t *err);

পূর্ণাঙ্ক read_all_fd_with_max_size (পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *description, বর্ণ **out, size_t *len, size_t max_size, libcrun_error_t *err);

স্থির অন্তঃস্থ পূর্ণাঙ্ক
read_all_fd (পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *description, বর্ণ **out, size_t *len, libcrun_error_t *err)
{
  প্রত্যাগম read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

পূর্ণাঙ্ক get_realpath_to_file (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *path_name, বর্ণ **absolute_path, libcrun_error_t *err);

পূর্ণাঙ্ক read_all_file (ধ্রুব বর্ণ *path, বর্ণ **out, size_t *len, libcrun_error_t *err);

পূর্ণাঙ্ক read_all_file_at (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *path, বর্ণ **out, size_t *len, libcrun_error_t *err);

পূর্ণাঙ্ক open_unix_domain_client_socket (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক dgram, libcrun_error_t *err);

পূর্ণাঙ্ক open_unix_domain_socket (ধ্রুব বর্ণ *path, পূর্ণাঙ্ক dgram, libcrun_error_t *err);

পূর্ণাঙ্ক send_fd_to_socket (পূর্ণাঙ্ক server, পূর্ণাঙ্ক fd, libcrun_error_t *err);

পূর্ণাঙ্ক send_fd_to_socket_with_payload (পূর্ণাঙ্ক server, পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *payload, size_t payload_len, libcrun_error_t *err);

পূর্ণাঙ্ক create_socket_pair (পূর্ণাঙ্ক *pair, libcrun_error_t *err);

পূর্ণাঙ্ক receive_fd_from_socket (পূর্ণাঙ্ক from, libcrun_error_t *err);

পূর্ণাঙ্ক receive_fd_from_socket_with_payload (পূর্ণাঙ্ক from, বর্ণ *payload, size_t payload_len, libcrun_error_t *err);

পূর্ণাঙ্ক create_signalfd (sigset_t *mask, libcrun_error_t *err);

পূর্ণাঙ্ক epoll_helper (পূর্ণাঙ্ক *in_fds, পূর্ণাঙ্ক *in_levelfds, পূর্ণাঙ্ক *out_fds, পূর্ণাঙ্ক *out_levelfds, libcrun_error_t *err);

পূর্ণাঙ্ক copy_from_fd_to_fd (পূর্ণাঙ্ক src, পূর্ণাঙ্ক dst, পূর্ণাঙ্ক consume, libcrun_error_t *err);

পূর্ণাঙ্ক run_process (বর্ণ **args, libcrun_error_t *err);

পূর্ণাঙ্ক format_default_id_mapping (বর্ণ **out, uid_t container_id, uid_t host_uid, uid_t host_id, পূর্ণাঙ্ক is_uid, libcrun_error_t *err);

পূর্ণাঙ্ক run_process_with_stdin_timeout_envp (বর্ণ *path, বর্ণ **args, ধ্রুব বর্ণ *cwd, পূর্ণাঙ্ক timeout, বর্ণ **envp,
                                         বর্ণ *stdin, size_t stdin_len, পূর্ণাঙ্ক out_fd, পূর্ণাঙ্ক err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

পূর্ণাঙ্ক mark_or_close_fds_ge_than (libcrun_container_t *container, পূর্ণাঙ্ক n, bool close_now, libcrun_error_t *err);

শূন্য get_current_timestamp (বর্ণ *out, size_t len);

পূর্ণাঙ্ক set_blocking_fd (পূর্ণাঙ্ক fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC পূর্ণাঙ্ক parse_json_file (json_object **out, ধ্রুব বর্ণ *jsondata, সংরচনা parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
স্থির অন্তঃস্থ পূর্ণাঙ্ক
has_prefix (ধ্রুব বর্ণ *str, ধ্রুব বর্ণ *prefix)
{
  size_t prefix_len;

  যদি (! str || ! prefix)
    প্রত্যাগম 0;

  prefix_len = strlen (prefix);
  প্রত্যাগম strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
স্থির অন্তঃস্থ পূর্ণাঙ্ক
has_dir_prefix (ধ্রুব বর্ণ *str, ধ্রুব বর্ণ *prefix)
{
  size_t prefix_len;

  যদি (! str || ! prefix)
    প্রত্যাগম 0;

  prefix_len = strlen (prefix);
  প্রত্যাগম strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

পূর্ণাঙ্ক find_executable (বর্ণ **exec_path, ধ্রুব বর্ণ *executable_path, ধ্রুব বর্ণ *cwd, libcrun_error_t *err);

পূর্ণাঙ্ক copy_recursive_fd_to_fd (পূর্ণাঙ্ক srcfd, পূর্ণাঙ্ক destfd, ধ্রুব বর্ণ *srcname, ধ্রুব বর্ণ *destname, libcrun_error_t *err);

পূর্ণাঙ্ক set_home_env (uid_t uid);

পূর্ণাঙ্ক libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_initialize_apparmor (libcrun_error_t *err);

ধ্রুব বর্ণ *find_annotation (libcrun_container_t *container, ধ্রুব বর্ণ *name);

পূর্ণাঙ্ক get_file_type_at (পূর্ণাঙ্ক dirfd, mode_t *mode, bool nofollow, ধ্রুব বর্ণ *path);

পূর্ণাঙ্ক get_file_type (mode_t *mode, bool nofollow, ধ্রুব বর্ণ *path);

পূর্ণাঙ্ক get_file_type_fd (পূর্ণাঙ্ক fd, mode_t *mode);

বর্ণ *get_user_name (uid_t uid);

পূর্ণাঙ্ক safe_openat (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *rootfs, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক mode,
                 libcrun_error_t *err);

পূর্ণাঙ্ক safe_write (পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *fname, ধ্রুব শূন্য *buf, size_t গুনতি, libcrun_error_t *err);

পূর্ণাঙ্ক append_paths (বর্ণ **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

পূর্ণাঙ্ক str2sig (ধ্রুব বর্ণ *name);

পূর্ণাঙ্ক base64_decode (ধ্রুব বর্ণ *iptr, size_t isize, বর্ণ *optr, size_t osize, size_t *nbytes);
পূর্ণাঙ্ক has_suffix (ধ্রুব বর্ণ *source, ধ্রুব বর্ণ *suffix);
বর্ণ *libcrun_sysctl_চাবি_to_proc_path (ধ্রুব বর্ণ *চাবি);
বর্ণ *str_join_array (পূর্ণাঙ্ক offset, size_t size, বর্ণ *ধ্রুব array[], ধ্রুব বর্ণ *joint);

ssize_t safe_readlinkat (পূর্ণাঙ্ক dfd, ধ্রুব বর্ণ *name, বর্ণ **buffer, ssize_t hint, libcrun_error_t *err);

বর্ণ **read_dir_entries (ধ্রুব বর্ণ *path, libcrun_error_t *err);

স্থির অন্তঃস্থ bool
is_empty_string (ধ্রুব বর্ণ *s)
{
  প্রত্যাগম s == রিক্ত || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
স্থির অন্তঃস্থ size_t
safe_strlen (ধ্রুব বর্ণ *s)
{
  প্রত্যাগম s == রিক্ত ? 0 : strlen (s);
}

স্থির অন্তঃস্থ bool
path_has_dot_dot_component (ধ্রুব বর্ণ *path)
{
  যদি (path == রিক্ত)
    প্রত্যাগম false;
  cleanup_free বর্ণ *npath = xstrdup (path);
  বর্ণ *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  যাবত্ (cur)
    {
      যদি (it)
        *it = '\0';

      যদি (strcmp (cur, "..") == 0)
        প্রত্যাগম true;

      যদি (it == রিক্ত)
        ভঙ্গ;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  প্রত্যাগম false;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
waitpid_ignore_stopped (pid_t pid, পূর্ণাঙ্ক *status, পূর্ণাঙ্ক options)
{
  পূর্ণাঙ্ক ret, s;
  কুরু
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      যদি (ret < 0)
        প্রত্যাগম ret;
  } যাবত্ (WIFSTOPPED (s) || WIFCONTINUED (s));

  যদি (status)
    *status = s;

  প্রত্যাগম ret;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
get_process_exit_status (পূর্ণাঙ্ক status)
{
  যদি (WIFEXITED (status))
    প্রত্যাগম WEXITSTATUS (status);
  যদি (WIFSIGNALED (status))
    প্রত্যাগম 128 + WTERMSIG (status);

  প্রত্যাগম -1;
}

uid_t get_overflow_uid (শূন্য);
gid_t get_overflow_gid (শূন্য);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (পরিমাণ (type) <= 1 ? 3U : পরিমাণ (type) <= 2 ? 5U  \
                                        : পরিমাণ (type) <= 4   ? 10U \
                                        : পরিমাণ (type) <= 8   ? 20U \
                                                               : পরিমাণ (পূর্ণাঙ্ক[-2 * (পরিমাণ (type) > 8)])))

#define _STRLEN(s) (পরিমাণ (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (পূর্ণাঙ্ক))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
প্রকারনির্দেশ বর্ণ proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

স্থির অন্তঃস্থ শূন্য
get_proc_fd_path (proc_fd_path_t path, pid_t pid, পূর্ণাঙ্ক fd)
{
  ধ্রুব size_t max_len = পরিমাণ (proc_fd_path_t);
  size_t n;

  যদি (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  অন্যথা
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  যদি (UNLIKELY (n >= max_len))
    abort ();
}

স্থির অন্তঃস্থ শূন্য
get_proc_self_fd_path (proc_fd_path_t path, পূর্ণাঙ্ক fd)
{
  get_proc_fd_path (path, 0, fd);
}

স্থির অন্তঃস্থ শূন্য
get_self_fd_path (proc_fd_path_t path, পূর্ণাঙ্ক fd)
{
  snprintf (path, পরিমাণ (proc_fd_path_t), "self/fd/%d", fd);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
validate_options (unsigned পূর্ণাঙ্ক specified_options, unsigned পূর্ণাঙ্ক supported_options, libcrun_error_t *err)
{
  যদি (! ! (~supported_options & specified_options))
    প্রত্যাগম crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  প্রত্যাগম 0;
}

বাহ্য পূর্ণাঙ্ক cpuset_string_to_bitmask (ধ্রুব বর্ণ *str, বর্ণ **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
সংরচনা channel_fd_pair;

সংরচনা channel_fd_pair *channel_fd_pair_new (পূর্ণাঙ্ক in_fd, পূর্ণাঙ্ক out_fd, size_t size);

শূন্য channel_fd_pair_free (সংরচনা channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
পূর্ণাঙ্ক channel_fd_pair_process (সংরচনা channel_fd_pair *channel, পূর্ণাঙ্ক epollfd, libcrun_error_t *err);

স্থির অন্তঃস্থ শূন্য
cleanup_channel_fd_pairp (শূন্য *p)
{
  সংরচনা channel_fd_pair **pp = (সংরচনা channel_fd_pair **) p;
  যদি (*pp == রিক্ত)
    প্রত্যাগম;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

পূর্ণাঙ্ক libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_open_proc_file (libcrun_container_t *container, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক flags, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ধ্রুব বর্ণ *path, পূর্ণাঙ্ক flags, libcrun_error_t *err);

#endif
