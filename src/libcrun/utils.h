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
#که_نه_تعریف_شوی UTILS_H
#تعریف_کړه UTILS_H

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <errno.h>
#ضمن <argp.h>
#ضمن "error.h"
#ضمن <dirent.h>
#ضمن <unistd.h>
#ضمن <signal.h>
#ضمن <fcntl.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن <sys/wait.h>
#ضمن "container.h"

#که_نه_تعریف_شوی لنډمهاله_FAILURE_RETRY
#  define لنډمهاله_FAILURE_RETRY(افاده)      \
    (__extension__ ({                         \
      اوږد صحيح __result;                      \
      افعل                                      \
        __result = (اوږد صحيح) (افاده);   \
      طالما (__result < 0 && errno == EINTR); \
      __result;                               \
    }))
#د_که_پای

#تعریف_کړه cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#تعریف_کړه cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#تعریف_کړه cleanup_close __attribute__ ((cleanup (cleanup_closep)))
#تعریف_کړه cleanup_close_vec __attribute__ ((cleanup (cleanup_close_vecp)))
#تعریف_کړه cleanup_close_map __attribute__ ((cleanup (cleanup_close_mapp)))
#تعریف_کړه cleanup_dir __attribute__ ((cleanup (cleanup_dirp)))
#تعریف_کړه arg_ناکارول_شوی __attribute__ ((ناکارول_شوی))
#تعریف_کړه cleanup_pid __attribute__ ((cleanup (cleanup_pidp)))
#تعریف_کړه cleanup_mmap __attribute__ ((cleanup (cleanup_mmapp)))

#تعریف_کړه LIKELY(x) __builtin_expect ((x), 1)
#تعریف_کړه UNLIKELY(x) __builtin_expect ((x), 0)

#تعریف_کړه WRITE_FILE_DEFAULT_FLAGS (O_CLOEXEC | O_CREAT | O_TRUNC | O_WRONLY)

__attribute__ ((حافظه_ورکړه)) ساكن مضمن فراغ *
xmalloc (size_t اندازه)
{
  فراغ *res = حافظه_ورکړه (اندازه);
  إذا (UNLIKELY (res == عدم))
    OOM ();
  أرجع res;
}

__attribute__ ((حافظه_ورکړه)) ساكن مضمن فراغ *
xmalloc0 (size_t اندازه)
{
  فراغ *res = حافظه_صفر_ورکړه (1, اندازه);
  إذا (UNLIKELY (res == عدم))
    OOM ();
  أرجع res;
}

__attribute__ ((حافظه_ورکړه)) ساكن مضمن فراغ *
xrealloc (فراغ *ptr, size_t اندازه)
{
  فراغ *res = realloc (ptr, اندازه);
  إذا (UNLIKELY (res == عدم))
    OOM ();
  أرجع res;
}

ساكن مضمن فراغ
cleanup_freep (فراغ *p)
{
  فراغ **pp = (فراغ **) p;
  حرر (*pp);
}

ساكن مضمن فراغ
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  إذا (file)
    (فراغ) fclose (file);
}

ساكن مضمن فراغ
cleanup_closep (فراغ *p)
{
  صحيح *pp = (صحيح *) p;
  إذا (*pp >= 0)
    لنډمهاله_FAILURE_RETRY (close (*pp));
}

ساكن مضمن فراغ
cleanup_pidp (فراغ *p)
{
  pid_t *pp = (pid_t *) p;
  إذا (*pp > 0)
    {
      لنډمهاله_FAILURE_RETRY (kill (*pp, SIGKILL));
      لنډمهاله_FAILURE_RETRY (waitpid (*pp, عدم, 0));
    }
}

بنية libcrun_mmap_s
{
  فراغ *addr;
  size_t اوږدتیا;
};

صحيح libcrun_mmap (بنية libcrun_mmap_s **ret, فراغ *addr, size_t اوږدتیا,
                  صحيح prot, صحيح flags, صحيح fd, off_t دننه_واټن,
                  libcrun_error_t *err);

صحيح libcrun_munmap (بنية libcrun_mmap_s *mmap, libcrun_error_t *err);

ساكن مضمن فراغ
cleanup_mmapp (فراغ *p)
{
  صحيح ret;
  libcrun_error_t tmp_err = عدم;
  بنية libcrun_mmap_s **mm;

  mm = (بنية libcrun_mmap_s **) p;
  إذا (*mm == عدم)
    أرجع;

  ret = libcrun_munmap (*mm, &tmp_err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

بنية libcrun_fd_map
{
  size_t nfds;
  صحيح fds[];
};

ساكن مضمن بنية libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  بنية libcrun_fd_map *ret;
  size_t i;

  ret = (بنية libcrun_fd_map *) xmalloc (حجم (*ret) + حجم (صحيح) * len);
  ret->nfds = len;
  لكل (i = 0; i < len; i++)
    ret->fds[i] = -1;

  أرجع ret;
}

ساكن مضمن فراغ
cleanup_close_mapp (بنية libcrun_fd_map **p)
{
  بنية libcrun_fd_map *m = *p;
  size_t i;

  إذا (m == عدم)
    أرجع;

  لكل (i = 0; i < m->nfds; i++)
    إذا (m->fds[i] >= 0)
      لنډمهاله_FAILURE_RETRY (close (m->fds[i]));

  حرر (m);
}

ساكن مضمن فراغ
cleanup_close_vecp (صحيح **p)
{
  صحيح *pp = *p;
  صحيح i;

  إذا (pp == عدم)
    أرجع;

  لكل (i = 0; pp[i] >= 0; i++)
    لنډمهاله_FAILURE_RETRY (close (pp[i]));
}

ساكن مضمن فراغ
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  إذا (dir)
    closedir (dir);
}

ساكن مضمن صحيح
close_and_reset (صحيح *fd)
{
  صحيح ret = 0;
  إذا (*fd >= 0)
    {
      ret = لنډمهاله_FAILURE_RETRY (close (*fd));
      إذا (LIKELY (ret == 0))
        *fd = -1;
    }
  أرجع ret;
}

ساكن مضمن حرف *
xstrdup (ثابت حرف *str)
{
  حرف *ret;
  إذا (str == عدم)
    أرجع عدم;

  ret = strdup (str);
  إذا (ret == عدم)
    OOM ();

  أرجع ret;
}

فراغ consume_trailing_slashes (حرف *لار);

#تعریف_کړه consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    طالما (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

ساكن مضمن bool
لار_is_slash_dev (ثابت حرف *لار)
{
  لار = consume_slashes (لار);

  إذا (مزي_پرتله (لار, "dev", 3))
    أرجع false;

  لار += 3;

  /* Check there are only '/' left.  */
  لكل (; *لار; لار++)
    إذا (*لار != '/')
      أرجع false;

  أرجع true;
}

صحيح xasprintf (حرف **str, ثابت حرف *fmt, ...) __attribute__ ((format (چاپ_کړه, 2, 3)));

صحيح crun_لار_exists (ثابت حرف *لار, libcrun_error_t *err);

صحيح write_file_at_with_flags (صحيح dirfd, صحيح flags, mode_t ډېرپېښ, ثابت حرف *name, ثابت فراغ *معلومات, size_t len, libcrun_error_t *err);

ساكن مضمن صحيح
write_file (ثابت حرف *name, ثابت فراغ *معلومات, size_t len, libcrun_error_t *err)
{
  أرجع write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, معلومات, len, err);
}

ساكن مضمن صحيح
write_file_at (صحيح dirfd, ثابت حرف *name, ثابت فراغ *معلومات, size_t len, libcrun_error_t *err)
{
  أرجع write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, معلومات, len, err);
}

صحيح crun_ensure_directory (ثابت حرف *لار, صحيح ډېرپېښ, bool nofollow, libcrun_error_t *err);

صحيح crun_ensure_directory_at (صحيح dirfd, ثابت حرف *لار, صحيح ډېرپېښ, bool nofollow, libcrun_error_t *err);

صحيح crun_خوندي_create_and_open_ref_at (bool dir, صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *لار, صحيح ډېرپېښ, libcrun_error_t *err);

صحيح crun_خوندي_ensure_directory_at (صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *لار, صحيح ډېرپېښ,
                                   libcrun_error_t *err);

صحيح crun_خوندي_ensure_file_at (صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *لار, صحيح ډېرپېښ,
                              libcrun_error_t *err);

صحيح crun_dir_p (ثابت حرف *لار, bool nofollow, libcrun_error_t *err);

صحيح crun_dir_p_at (صحيح dirfd, ثابت حرف *لار, bool nofollow, libcrun_error_t *err);

صحيح detach_process ();

صحيح create_file_if_missing_at (صحيح dirfd, ثابت حرف *file, mode_t ډېرپېښ, libcrun_error_t *err);

صحيح check_running_دننه_user_namespace (libcrun_error_t *err);

صحيح سېټ_selinux_label (libcrun_container_t *container, ثابت حرف *label, bool now, libcrun_error_t *err);

صحيح جمع_کړه_selinux_mount_label (حرف **ret, ثابت حرف *معلومات, ثابت حرف *label, ثابت حرف *context_type, libcrun_error_t *err);

صحيح سېټ_apparmor_profile (libcrun_container_t *container, ثابت حرف *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

صحيح read_all_fd_with_اندازه_hint (صحيح fd, ثابت حرف *description, حرف **بهر, size_t *len, size_t hint, libcrun_error_t *err);

صحيح read_all_fd_with_max_اندازه (صحيح fd, ثابت حرف *description, حرف **بهر, size_t *len, size_t max_اندازه, libcrun_error_t *err);

ساكن مضمن صحيح
read_all_fd (صحيح fd, ثابت حرف *description, حرف **بهر, size_t *len, libcrun_error_t *err)
{
  أرجع read_all_fd_with_اندازه_hint (fd, description, بهر, len, 0, err);
}

صحيح get_realpath_منزل_file (صحيح dirfd, ثابت حرف *لار_name, حرف **absolute_لار, libcrun_error_t *err);

صحيح read_all_file (ثابت حرف *لار, حرف **بهر, size_t *len, libcrun_error_t *err);

صحيح read_all_file_at (صحيح dirfd, ثابت حرف *لار, حرف **بهر, size_t *len, libcrun_error_t *err);

صحيح open_unix_domain_client_socket (ثابت حرف *لار, صحيح dgram, libcrun_error_t *err);

صحيح open_unix_domain_socket (ثابت حرف *لار, صحيح dgram, libcrun_error_t *err);

صحيح send_fd_منزل_socket (صحيح server, صحيح fd, libcrun_error_t *err);

صحيح send_fd_منزل_socket_with_payload (صحيح server, صحيح fd, ثابت حرف *payload, size_t payload_len, libcrun_error_t *err);

صحيح create_socket_pair (صحيح *pair, libcrun_error_t *err);

صحيح receive_fd_from_socket (صحيح from, libcrun_error_t *err);

صحيح receive_fd_from_socket_with_payload (صحيح from, حرف *payload, size_t payload_len, libcrun_error_t *err);

صحيح create_signalfd (sigset_t *ماسک, libcrun_error_t *err);

صحيح epoll_helper (صحيح *دننه_fds, صحيح *دننه_levelfds, صحيح *بهر_fds, صحيح *بهر_levelfds, libcrun_error_t *err);

صحيح copy_from_fd_منزل_fd (صحيح src, صحيح dst, صحيح consume, libcrun_error_t *err);

صحيح پرلپسې_process (حرف **دلیلونه, libcrun_error_t *err);

صحيح format_default_id_mapping (حرف **بهر, uid_t container_id, uid_t host_uid, uid_t host_id, صحيح is_uid, libcrun_error_t *err);

صحيح پرلپسې_process_with_stdin_timeout_envp (حرف *لار, حرف **دلیلونه, ثابت حرف *cwd, صحيح timeout, حرف **envp,
                                         حرف *stdin, size_t stdin_len, صحيح بهر_fd, صحيح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

صحيح mark_or_close_fds_ge_than (libcrun_container_t *container, صحيح n, bool close_now, libcrun_error_t *err);

فراغ get_current_timestamp (حرف *بهر, size_t len);

صحيح سېټ_blocking_fd (صحيح fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_عامه صحيح parse_json_file (json_object **بهر, ثابت حرف *jsondata, بنية parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
ساكن مضمن صحيح
has_مختاړی (ثابت حرف *str, ثابت حرف *مختاړی)
{
  size_t مختاړی_len;

  إذا (! str || ! مختاړی)
    أرجع 0;

  مختاړی_len = مزي_اوږدوالی (مختاړی);
  أرجع مزي_اوږدوالی (str) >= مختاړی_len && memcmp (str, مختاړی, مختاړی_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
ساكن مضمن صحيح
has_dir_مختاړی (ثابت حرف *str, ثابت حرف *مختاړی)
{
  size_t مختاړی_len;

  إذا (! str || ! مختاړی)
    أرجع 0;

  مختاړی_len = مزي_اوږدوالی (مختاړی);
  أرجع مزي_اوږدوالی (str) > مختاړی_len && memcmp (str, مختاړی, مختاړی_len) == 0 && str[مختاړی_len] == '/';
}

صحيح پیدا_کړه_executable (حرف **exec_لار, ثابت حرف *executable_لار, ثابت حرف *cwd, libcrun_error_t *err);

صحيح copy_recursive_fd_منزل_fd (صحيح srcfd, صحيح destfd, ثابت حرف *srcname, ثابت حرف *destname, libcrun_error_t *err);

صحيح سېټ_home_env (uid_t uid);

صحيح libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

صحيح libcrun_initialize_apparmor (libcrun_error_t *err);

ثابت حرف *پیدا_کړه_annotation (libcrun_container_t *container, ثابت حرف *name);

صحيح get_file_type_at (صحيح dirfd, mode_t *ډېرپېښ, bool nofollow, ثابت حرف *لار);

صحيح get_file_type (mode_t *ډېرپېښ, bool nofollow, ثابت حرف *لار);

صحيح get_file_type_fd (صحيح fd, mode_t *ډېرپېښ);

حرف *get_user_name (uid_t uid);

صحيح خوندي_openat (صحيح dirfd, ثابت حرف *rootfs, ثابت حرف *لار, صحيح flags, صحيح ډېرپېښ,
                 libcrun_error_t *err);

صحيح خوندي_write (صحيح fd, ثابت حرف *fname, ثابت فراغ *buf, size_t شمېر, libcrun_error_t *err);

صحيح append_paths (حرف **بهر, libcrun_error_t *err, ...) __attribute__ ((sentinel));

صحيح str2sig (ثابت حرف *name);

صحيح base64_decode (ثابت حرف *iptr, size_t isize, حرف *optr, size_t osize, size_t *nbytes);
صحيح has_suffix (ثابت حرف *سرچینه, ثابت حرف *suffix);
حرف *libcrun_sysctl_کلی_منزل_proc_لار (ثابت حرف *کلی);
حرف *str_join_array (صحيح دننه_واټن, size_t اندازه, حرف *ثابت array[], ثابت حرف *joint);

ssize_t خوندي_readlinkat (صحيح dfd, ثابت حرف *name, حرف **بفر, ssize_t hint, libcrun_error_t *err);

حرف **read_dir_entries (ثابت حرف *لار, libcrun_error_t *err);

ساكن مضمن bool
is_تشه_string (ثابت حرف *s)
{
  أرجع s == عدم || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
ساكن مضمن size_t
خوندي_strlen (ثابت حرف *s)
{
  أرجع s == عدم ? 0 : مزي_اوږدوالی (s);
}

ساكن مضمن bool
لار_has_dot_dot_component (ثابت حرف *لار)
{
  إذا (لار == عدم)
    أرجع false;
  cleanup_free حرف *npath = xstrdup (لار);
  حرف *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  طالما (cur)
    {
      إذا (it)
        *it = '\0';

      إذا (strcmp (cur, "..") == 0)
        أرجع true;

      إذا (it == عدم)
        اكسر;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  أرجع false;
}

ساكن مضمن صحيح
waitpid_ignore_stopped (pid_t pid, صحيح *حالت, صحيح options)
{
  صحيح ret, s;
  افعل
    {
      s = 0;
      ret = لنډمهاله_FAILURE_RETRY (waitpid (pid, &s, options));
      إذا (ret < 0)
        أرجع ret;
  } طالما (WIFSTOPPED (s) || WIFCONTINUED (s));

  إذا (حالت)
    *حالت = s;

  أرجع ret;
}

ساكن مضمن صحيح
get_process_exit_حالت (صحيح حالت)
{
  إذا (WIFEXITED (حالت))
    أرجع WEXITSTATUS (حالت);
  إذا (WIFSIGNALED (حالت))
    أرجع 128 + WTERMSIG (حالت);

  أرجع -1;
}

uid_t get_ډکېدنه_uid (فراغ);
gid_t get_ډکېدنه_gid (فراغ);

/* Adapted from systemd.  Include space for the NUL byte.  */
#تعریف_کړه DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (حجم (type) <= 1 ? 3U : حجم (type) <= 2 ? 5U  \
                                        : حجم (type) <= 4   ? 10U \
                                        : حجم (type) <= 8   ? 20U \
                                                               : حجم (صحيح[-2 * (حجم (type) > 8)])))

#تعریف_کړه _STRLEN(s) (حجم (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#تعریف_کړه PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (صحيح))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
تعريفنوع حرف proc_fd_لار_t[PROC_PID_FD_STRLEN];

#تعریف_لغوه _STRLEN

ساكن مضمن فراغ
get_proc_fd_لار (proc_fd_لار_t لار, pid_t pid, صحيح fd)
{
  ثابت size_t max_len = حجم (proc_fd_لار_t);
  size_t n;

  إذا (pid)
    n = snprintf (لار, max_len, "/proc/%d/fd/%d", pid, fd);
  وإلا
    n = snprintf (لار, max_len, "/proc/self/fd/%d", fd);

  إذا (UNLIKELY (n >= max_len))
    abort ();
}

ساكن مضمن فراغ
get_proc_self_fd_لار (proc_fd_لار_t لار, صحيح fd)
{
  get_proc_fd_لار (لار, 0, fd);
}

ساكن مضمن فراغ
get_self_fd_لار (proc_fd_لار_t لار, صحيح fd)
{
  snprintf (لار, حجم (proc_fd_لار_t), "self/fd/%d", fd);
}

ساكن مضمن صحيح
validate_options (بینښه صحيح specified_options, بینښه صحيح supported_options, libcrun_error_t *err)
{
  إذا (! ! (~supported_options & specified_options))
    أرجع crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  أرجع 0;
}

خارجي صحيح cpuset_string_منزل_bitmask (ثابت حرف *str, حرف **بهر, size_t *بهر_اندازه, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
بنية channel_fd_pair;

بنية channel_fd_pair *channel_fd_pair_new (صحيح دننه_fd, صحيح بهر_fd, size_t اندازه);

فراغ channel_fd_pair_free (بنية channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
صحيح channel_fd_pair_process (بنية channel_fd_pair *channel, صحيح epollfd, libcrun_error_t *err);

ساكن مضمن فراغ
cleanup_channel_fd_pairp (فراغ *p)
{
  بنية channel_fd_pair **pp = (بنية channel_fd_pair **) p;
  إذا (*pp == عدم)
    أرجع;

  channel_fd_pair_free (*pp);
}
#تعریف_کړه cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

صحيح libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
صحيح libcrun_open_proc_file (libcrun_container_t *container, ثابت حرف *لار, صحيح flags, libcrun_error_t *err);
صحيح libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ثابت حرف *لار, صحيح flags, libcrun_error_t *err);

#د_که_پای
