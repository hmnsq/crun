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

#ifndef অস্থায়ী_FAILURE_RETRY
#  define অস্থায়ী_FAILURE_RETRY(expression)      \
    (__extension__ ({                         \
      long পূর্ণসংখ্যা __result;                      \
      করো                                      \
        __result = (long পূর্ণসংখ্যা) (expression);   \
      যখন (__result < 0 && errno == EINTR); \
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

__attribute__ ((malloc)) স্থির অন্তঃরেখ শূন্য *
xmalloc (size_t আকার_2)
{
  শূন্য *res = malloc (আকার_2);
  যদি (UNLIKELY (res == ফাঁকা))
    OOM ();
  ফেরত res;
}

__attribute__ ((malloc)) স্থির অন্তঃরেখ শূন্য *
xmalloc0 (size_t আকার_2)
{
  শূন্য *res = calloc (1, আকার_2);
  যদি (UNLIKELY (res == ফাঁকা))
    OOM ();
  ফেরত res;
}

__attribute__ ((malloc)) স্থির অন্তঃরেখ শূন্য *
xrealloc (শূন্য *ptr, size_t আকার_2)
{
  শূন্য *res = realloc (ptr, আকার_2);
  যদি (UNLIKELY (res == ফাঁকা))
    OOM ();
  ফেরত res;
}

স্থির অন্তঃরেখ শূন্য
cleanup_freep (শূন্য *p)
{
  শূন্য **pp = (শূন্য **) p;
  মুক্ত (*pp);
}

স্থির অন্তঃরেখ শূন্য
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  যদি (file)
    (শূন্য) fclose (file);
}

স্থির অন্তঃরেখ শূন্য
cleanup_closep (শূন্য *p)
{
  পূর্ণসংখ্যা *pp = (পূর্ণসংখ্যা *) p;
  যদি (*pp >= 0)
    অস্থায়ী_FAILURE_RETRY (close (*pp));
}

স্থির অন্তঃরেখ শূন্য
cleanup_pidp (শূন্য *p)
{
  pid_t *pp = (pid_t *) p;
  যদি (*pp > 0)
    {
      অস্থায়ী_FAILURE_RETRY (kill (*pp, SIGKILL));
      অস্থায়ী_FAILURE_RETRY (waitpid (*pp, ফাঁকা, 0));
    }
}

কাঠামো libcrun_mmap_s
{
  শূন্য *addr;
  size_t দৈর্ঘ্য;
};

পূর্ণসংখ্যা libcrun_mmap (কাঠামো libcrun_mmap_s **ret, শূন্য *addr, size_t দৈর্ঘ্য,
                  পূর্ণসংখ্যা prot, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা fd, off_t offset,
                  libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_munmap (কাঠামো libcrun_mmap_s *mmap, libcrun_error_t *err);

স্থির অন্তঃরেখ শূন্য
cleanup_mmapp (শূন্য *p)
{
  পূর্ণসংখ্যা ret;
  libcrun_error_t tmp_err = ফাঁকা;
  কাঠামো libcrun_mmap_s **mm;

  mm = (কাঠামো libcrun_mmap_s **) p;
  যদি (*mm == ফাঁকা)
    ফেরত;

  ret = libcrun_munmap (*mm, &tmp_err);
  যদি (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

কাঠামো libcrun_fd_map
{
  size_t nfds;
  পূর্ণসংখ্যা fds[];
};

স্থির অন্তঃরেখ কাঠামো libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  কাঠামো libcrun_fd_map *ret;
  size_t i;

  ret = (কাঠামো libcrun_fd_map *) xmalloc (আকার (*ret) + আকার (পূর্ণসংখ্যা) * len);
  ret->nfds = len;
  জন্য (i = 0; i < len; i++)
    ret->fds[i] = -1;

  ফেরত ret;
}

স্থির অন্তঃরেখ শূন্য
cleanup_close_mapp (কাঠামো libcrun_fd_map **p)
{
  কাঠামো libcrun_fd_map *m = *p;
  size_t i;

  যদি (m == ফাঁকা)
    ফেরত;

  জন্য (i = 0; i < m->nfds; i++)
    যদি (m->fds[i] >= 0)
      অস্থায়ী_FAILURE_RETRY (close (m->fds[i]));

  মুক্ত (m);
}

স্থির অন্তঃরেখ শূন্য
cleanup_close_vecp (পূর্ণসংখ্যা **p)
{
  পূর্ণসংখ্যা *pp = *p;
  পূর্ণসংখ্যা i;

  যদি (pp == ফাঁকা)
    ফেরত;

  জন্য (i = 0; pp[i] >= 0; i++)
    অস্থায়ী_FAILURE_RETRY (close (pp[i]));
}

স্থির অন্তঃরেখ শূন্য
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  যদি (dir)
    closedir (dir);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
close_and_reset (পূর্ণসংখ্যা *fd)
{
  পূর্ণসংখ্যা ret = 0;
  যদি (*fd >= 0)
    {
      ret = অস্থায়ী_FAILURE_RETRY (close (*fd));
      যদি (LIKELY (ret == 0))
        *fd = -1;
    }
  ফেরত ret;
}

স্থির অন্তঃরেখ অক্ষর *
xstrdup (ধ্রুবক অক্ষর *str)
{
  অক্ষর *ret;
  যদি (str == ফাঁকা)
    ফেরত ফাঁকা;

  ret = strdup (str);
  যদি (ret == ফাঁকা)
    OOM ();

  ফেরত ret;
}

শূন্য consume_trailing_slashes (অক্ষর *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    যখন (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

স্থির অন্তঃরেখ bool
path_is_slash_dev (ধ্রুবক অক্ষর *path)
{
  path = consume_slashes (path);

  যদি (strncmp (path, "dev", 3))
    ফেরত false;

  path += 3;

  /* Check there are only '/' left.  */
  জন্য (; *path; path++)
    যদি (*path != '/')
      ফেরত false;

  ফেরত true;
}

পূর্ণসংখ্যা xasprintf (অক্ষর **str, ধ্রুবক অক্ষর *fmt, ...) __attribute__ ((format (printf, 2, 3)));

পূর্ণসংখ্যা crun_path_exists (ধ্রুবক অক্ষর *path, libcrun_error_t *err);

পূর্ণসংখ্যা write_file_at_with_flags (পূর্ণসংখ্যা dirfd, পূর্ণসংখ্যা flags, mode_t mode, ধ্রুবক অক্ষর *name, ধ্রুবক শূন্য *তথ্য, size_t len, libcrun_error_t *err);

স্থির অন্তঃরেখ পূর্ণসংখ্যা
write_file (ধ্রুবক অক্ষর *name, ধ্রুবক শূন্য *তথ্য, size_t len, libcrun_error_t *err)
{
  ফেরত write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, তথ্য, len, err);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
write_file_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *name, ধ্রুবক শূন্য *তথ্য, size_t len, libcrun_error_t *err)
{
  ফেরত write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, তথ্য, len, err);
}

পূর্ণসংখ্যা crun_ensure_directory (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode, bool nofollow, libcrun_error_t *err);

পূর্ণসংখ্যা crun_ensure_directory_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode, bool nofollow, libcrun_error_t *err);

পূর্ণসংখ্যা crun_safe_create_and_open_ref_at (bool dir, পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *dirpath, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode, libcrun_error_t *err);

পূর্ণসংখ্যা crun_safe_ensure_directory_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *dirpath, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode,
                                   libcrun_error_t *err);

পূর্ণসংখ্যা crun_safe_ensure_file_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *dirpath, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode,
                              libcrun_error_t *err);

পূর্ণসংখ্যা crun_dir_p (ধ্রুবক অক্ষর *path, bool nofollow, libcrun_error_t *err);

পূর্ণসংখ্যা crun_dir_p_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *path, bool nofollow, libcrun_error_t *err);

পূর্ণসংখ্যা detach_process ();

পূর্ণসংখ্যা create_file_if_missing_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *file, mode_t mode, libcrun_error_t *err);

পূর্ণসংখ্যা check_running_in_user_namespace (libcrun_error_t *err);

পূর্ণসংখ্যা set_selinux_label (libcrun_container_t *container, ধ্রুবক অক্ষর *label, bool now, libcrun_error_t *err);

পূর্ণসংখ্যা add_selinux_mount_label (অক্ষর **ret, ধ্রুবক অক্ষর *তথ্য, ধ্রুবক অক্ষর *label, ধ্রুবক অক্ষর *context_type, libcrun_error_t *err);

পূর্ণসংখ্যা set_apparmor_profile (libcrun_container_t *container, ধ্রুবক অক্ষর *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

পূর্ণসংখ্যা read_all_fd_with_আকার_hint (পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *description, অক্ষর **out, size_t *len, size_t hint, libcrun_error_t *err);

পূর্ণসংখ্যা read_all_fd_with_max_আকার (পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *description, অক্ষর **out, size_t *len, size_t max_আকার, libcrun_error_t *err);

স্থির অন্তঃরেখ পূর্ণসংখ্যা
read_all_fd (পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *description, অক্ষর **out, size_t *len, libcrun_error_t *err)
{
  ফেরত read_all_fd_with_আকার_hint (fd, description, out, len, 0, err);
}

পূর্ণসংখ্যা get_realpath_to_file (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *path_name, অক্ষর **absolute_path, libcrun_error_t *err);

পূর্ণসংখ্যা read_all_file (ধ্রুবক অক্ষর *path, অক্ষর **out, size_t *len, libcrun_error_t *err);

পূর্ণসংখ্যা read_all_file_at (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *path, অক্ষর **out, size_t *len, libcrun_error_t *err);

পূর্ণসংখ্যা open_unix_domain_client_socket (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা dgram, libcrun_error_t *err);

পূর্ণসংখ্যা open_unix_domain_socket (ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা dgram, libcrun_error_t *err);

পূর্ণসংখ্যা send_fd_to_socket (পূর্ণসংখ্যা server, পূর্ণসংখ্যা fd, libcrun_error_t *err);

পূর্ণসংখ্যা send_fd_to_socket_with_payload (পূর্ণসংখ্যা server, পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *payload, size_t payload_len, libcrun_error_t *err);

পূর্ণসংখ্যা create_socket_pair (পূর্ণসংখ্যা *pair, libcrun_error_t *err);

পূর্ণসংখ্যা receive_fd_from_socket (পূর্ণসংখ্যা from, libcrun_error_t *err);

পূর্ণসংখ্যা receive_fd_from_socket_with_payload (পূর্ণসংখ্যা from, অক্ষর *payload, size_t payload_len, libcrun_error_t *err);

পূর্ণসংখ্যা create_signalfd (sigset_t *mask, libcrun_error_t *err);

পূর্ণসংখ্যা epoll_helper (পূর্ণসংখ্যা *in_fds, পূর্ণসংখ্যা *in_levelfds, পূর্ণসংখ্যা *out_fds, পূর্ণসংখ্যা *out_levelfds, libcrun_error_t *err);

পূর্ণসংখ্যা copy_from_fd_to_fd (পূর্ণসংখ্যা src, পূর্ণসংখ্যা dst, পূর্ণসংখ্যা consume, libcrun_error_t *err);

পূর্ণসংখ্যা run_process (অক্ষর **args, libcrun_error_t *err);

পূর্ণসংখ্যা format_default_id_mapping (অক্ষর **out, uid_t container_id, uid_t host_uid, uid_t host_id, পূর্ণসংখ্যা is_uid, libcrun_error_t *err);

পূর্ণসংখ্যা run_process_with_stdin_timeout_envp (অক্ষর *path, অক্ষর **args, ধ্রুবক অক্ষর *cwd, পূর্ণসংখ্যা timeout, অক্ষর **envp,
                                         অক্ষর *stdin, size_t stdin_len, পূর্ণসংখ্যা out_fd, পূর্ণসংখ্যা err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

পূর্ণসংখ্যা mark_or_close_fds_ge_than (libcrun_container_t *container, পূর্ণসংখ্যা n, bool close_now, libcrun_error_t *err);

শূন্য get_current_timestamp (অক্ষর *out, size_t len);

পূর্ণসংখ্যা set_blocking_fd (পূর্ণসংখ্যা fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC পূর্ণসংখ্যা parse_json_file (json_object **out, ধ্রুবক অক্ষর *jsondata, কাঠামো parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
স্থির অন্তঃরেখ পূর্ণসংখ্যা
has_prefix (ধ্রুবক অক্ষর *str, ধ্রুবক অক্ষর *prefix)
{
  size_t prefix_len;

  যদি (! str || ! prefix)
    ফেরত 0;

  prefix_len = strlen (prefix);
  ফেরত strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
স্থির অন্তঃরেখ পূর্ণসংখ্যা
has_dir_prefix (ধ্রুবক অক্ষর *str, ধ্রুবক অক্ষর *prefix)
{
  size_t prefix_len;

  যদি (! str || ! prefix)
    ফেরত 0;

  prefix_len = strlen (prefix);
  ফেরত strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

পূর্ণসংখ্যা find_executable (অক্ষর **exec_path, ধ্রুবক অক্ষর *executable_path, ধ্রুবক অক্ষর *cwd, libcrun_error_t *err);

পূর্ণসংখ্যা copy_recursive_fd_to_fd (পূর্ণসংখ্যা srcfd, পূর্ণসংখ্যা destfd, ধ্রুবক অক্ষর *srcname, ধ্রুবক অক্ষর *destname, libcrun_error_t *err);

পূর্ণসংখ্যা set_home_env (uid_t uid);

পূর্ণসংখ্যা libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_initialize_apparmor (libcrun_error_t *err);

ধ্রুবক অক্ষর *find_annotation (libcrun_container_t *container, ধ্রুবক অক্ষর *name);

পূর্ণসংখ্যা get_file_type_at (পূর্ণসংখ্যা dirfd, mode_t *mode, bool nofollow, ধ্রুবক অক্ষর *path);

পূর্ণসংখ্যা get_file_type (mode_t *mode, bool nofollow, ধ্রুবক অক্ষর *path);

পূর্ণসংখ্যা get_file_type_fd (পূর্ণসংখ্যা fd, mode_t *mode);

অক্ষর *get_user_name (uid_t uid);

পূর্ণসংখ্যা safe_openat (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *rootfs, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা mode,
                 libcrun_error_t *err);

পূর্ণসংখ্যা safe_write (পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *fname, ধ্রুবক শূন্য *buf, size_t গুনতি, libcrun_error_t *err);

পূর্ণসংখ্যা append_paths (অক্ষর **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

পূর্ণসংখ্যা str2sig (ধ্রুবক অক্ষর *name);

পূর্ণসংখ্যা base64_decode (ধ্রুবক অক্ষর *iptr, size_t isize, অক্ষর *optr, size_t osize, size_t *nbytes);
পূর্ণসংখ্যা has_suffix (ধ্রুবক অক্ষর *source, ধ্রুবক অক্ষর *suffix);
অক্ষর *libcrun_sysctl_চাবি_to_proc_path (ধ্রুবক অক্ষর *চাবি);
অক্ষর *str_join_array (পূর্ণসংখ্যা offset, size_t আকার_2, অক্ষর *ধ্রুবক array[], ধ্রুবক অক্ষর *joint);

ssize_t safe_readlinkat (পূর্ণসংখ্যা dfd, ধ্রুবক অক্ষর *name, অক্ষর **বাফার, ssize_t hint, libcrun_error_t *err);

অক্ষর **read_dir_entries (ধ্রুবক অক্ষর *path, libcrun_error_t *err);

স্থির অন্তঃরেখ bool
is_empty_string (ধ্রুবক অক্ষর *s)
{
  ফেরত s == ফাঁকা || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
স্থির অন্তঃরেখ size_t
safe_strlen (ধ্রুবক অক্ষর *s)
{
  ফেরত s == ফাঁকা ? 0 : strlen (s);
}

স্থির অন্তঃরেখ bool
path_has_dot_dot_component (ধ্রুবক অক্ষর *path)
{
  যদি (path == ফাঁকা)
    ফেরত false;
  cleanup_free অক্ষর *npath = xstrdup (path);
  অক্ষর *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  যখন (cur)
    {
      যদি (it)
        *it = '\0';

      যদি (strcmp (cur, "..") == 0)
        ফেরত true;

      যদি (it == ফাঁকা)
        বিরতি;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  ফেরত false;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
waitpid_ignore_stopped (pid_t pid, পূর্ণসংখ্যা *status, পূর্ণসংখ্যা options)
{
  পূর্ণসংখ্যা ret, s;
  করো
    {
      s = 0;
      ret = অস্থায়ী_FAILURE_RETRY (waitpid (pid, &s, options));
      যদি (ret < 0)
        ফেরত ret;
  } যখন (WIFSTOPPED (s) || WIFCONTINUED (s));

  যদি (status)
    *status = s;

  ফেরত ret;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
get_process_exit_status (পূর্ণসংখ্যা status)
{
  যদি (WIFEXITED (status))
    ফেরত WEXITSTATUS (status);
  যদি (WIFSIGNALED (status))
    ফেরত 128 + WTERMSIG (status);

  ফেরত -1;
}

uid_t get_overflow_uid (শূন্য);
gid_t get_overflow_gid (শূন্য);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (আকার (type) <= 1 ? 3U : আকার (type) <= 2 ? 5U  \
                                        : আকার (type) <= 4   ? 10U \
                                        : আকার (type) <= 8   ? 20U \
                                                               : আকার (পূর্ণসংখ্যা[-2 * (আকার (type) > 8)])))

#define _STRLEN(s) (আকার (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (পূর্ণসংখ্যা))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
প্রকারলেখ অক্ষর proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

স্থির অন্তঃরেখ শূন্য
get_proc_fd_path (proc_fd_path_t path, pid_t pid, পূর্ণসংখ্যা fd)
{
  ধ্রুবক size_t max_len = আকার (proc_fd_path_t);
  size_t n;

  যদি (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  অন্যথা
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  যদি (UNLIKELY (n >= max_len))
    abort ();
}

স্থির অন্তঃরেখ শূন্য
get_proc_self_fd_path (proc_fd_path_t path, পূর্ণসংখ্যা fd)
{
  get_proc_fd_path (path, 0, fd);
}

স্থির অন্তঃরেখ শূন্য
get_self_fd_path (proc_fd_path_t path, পূর্ণসংখ্যা fd)
{
  snprintf (path, আকার (proc_fd_path_t), "self/fd/%d", fd);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
validate_options (unsigned পূর্ণসংখ্যা specified_options, unsigned পূর্ণসংখ্যা supported_options, libcrun_error_t *err)
{
  যদি (! ! (~supported_options & specified_options))
    ফেরত crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  ফেরত 0;
}

বাহ্যিক পূর্ণসংখ্যা cpuset_string_to_bitmask (ধ্রুবক অক্ষর *str, অক্ষর **out, size_t *out_আকার, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
কাঠামো channel_fd_pair;

কাঠামো channel_fd_pair *channel_fd_pair_new (পূর্ণসংখ্যা in_fd, পূর্ণসংখ্যা out_fd, size_t আকার_2);

শূন্য channel_fd_pair_free (কাঠামো channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
পূর্ণসংখ্যা channel_fd_pair_process (কাঠামো channel_fd_pair *channel, পূর্ণসংখ্যা epollfd, libcrun_error_t *err);

স্থির অন্তঃরেখ শূন্য
cleanup_channel_fd_pairp (শূন্য *p)
{
  কাঠামো channel_fd_pair **pp = (কাঠামো channel_fd_pair **) p;
  যদি (*pp == ফাঁকা)
    ফেরত;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

পূর্ণসংখ্যা libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_open_proc_file (libcrun_container_t *container, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা flags, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা flags, libcrun_error_t *err);

#endif
