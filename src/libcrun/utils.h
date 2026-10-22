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
#اگر_غیر_تعریف_شدہ UTILS_H
#تعریف_کرو UTILS_H

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
#شامل <errno.h>
#شامل <argp.h>
#شامل "error.h"
#شامل <dirent.h>
#شامل <unistd.h>
#شامل <signal.h>
#شامل <fcntl.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل <sys/wait.h>
#شامل "container.h"

#اگر_غیر_تعریف_شدہ عارضی_FAILURE_RETRY
#  define عارضی_FAILURE_RETRY(عبارت)      \
    (__extension__ ({                         \
      طویل صحیح __result;                      \
      کرو                                      \
        __result = (طویل صحیح) (عبارت);   \
      جب (__result < 0 && errno == EINTR); \
      __result;                               \
    }))
#اگر_ختم

#تعریف_کرو cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#تعریف_کرو cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#تعریف_کرو cleanup_close __attribute__ ((cleanup (cleanup_closep)))
#تعریف_کرو cleanup_close_vec __attribute__ ((cleanup (cleanup_close_vecp)))
#تعریف_کرو cleanup_close_map __attribute__ ((cleanup (cleanup_close_mapp)))
#تعریف_کرو cleanup_dir __attribute__ ((cleanup (cleanup_dirp)))
#تعریف_کرو arg_غیر_مستعمل __attribute__ ((غیر_مستعمل))
#تعریف_کرو cleanup_pid __attribute__ ((cleanup (cleanup_pidp)))
#تعریف_کرو cleanup_mmap __attribute__ ((cleanup (cleanup_mmapp)))

#تعریف_کرو LIKELY(x) __builtin_expect ((x), 1)
#تعریف_کرو UNLIKELY(x) __builtin_expect ((x), 0)

#تعریف_کرو WRITE_FILE_DEFAULT_FLAGS (O_CLOEXEC | O_CREAT | O_TRUNC | O_WRONLY)

__attribute__ ((حافظہ_دو)) ساکن اندرونی خالی *
xmalloc (size_t سائز)
{
  خالی *res = حافظہ_دو (سائز);
  اگر (UNLIKELY (res == تہی))
    OOM ();
  واپس res;
}

__attribute__ ((حافظہ_دو)) ساکن اندرونی خالی *
xmalloc0 (size_t سائز)
{
  خالی *res = صفر_حافظہ_دو (1, سائز);
  اگر (UNLIKELY (res == تہی))
    OOM ();
  واپس res;
}

__attribute__ ((حافظہ_دو)) ساکن اندرونی خالی *
xrealloc (خالی *ptr, size_t سائز)
{
  خالی *res = realloc (ptr, سائز);
  اگر (UNLIKELY (res == تہی))
    OOM ();
  واپس res;
}

ساکن اندرونی خالی
cleanup_freep (خالی *p)
{
  خالی **pp = (خالی **) p;
  آزاد (*pp);
}

ساکن اندرونی خالی
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  اگر (file)
    (خالی) fclose (file);
}

ساکن اندرونی خالی
cleanup_closep (خالی *p)
{
  صحیح *pp = (صحیح *) p;
  اگر (*pp >= 0)
    عارضی_FAILURE_RETRY (close (*pp));
}

ساکن اندرونی خالی
cleanup_pidp (خالی *p)
{
  pid_t *pp = (pid_t *) p;
  اگر (*pp > 0)
    {
      عارضی_FAILURE_RETRY (kill (*pp, SIGKILL));
      عارضی_FAILURE_RETRY (waitpid (*pp, تہی, 0));
    }
}

ساخت libcrun_mmap_s
{
  خالی *addr;
  size_t لمبائی;
};

صحیح libcrun_mmap (ساخت libcrun_mmap_s **ret, خالی *addr, size_t لمبائی,
                  صحیح prot, صحیح flags, صحیح fd, off_t انحراف,
                  libcrun_error_t *err);

صحیح libcrun_munmap (ساخت libcrun_mmap_s *mmap, libcrun_error_t *err);

ساکن اندرونی خالی
cleanup_mmapp (خالی *p)
{
  صحیح ret;
  libcrun_error_t tmp_err = تہی;
  ساخت libcrun_mmap_s **mm;

  mm = (ساخت libcrun_mmap_s **) p;
  اگر (*mm == تہی)
    واپس;

  ret = libcrun_munmap (*mm, &tmp_err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

ساخت libcrun_fd_map
{
  size_t nfds;
  صحیح fds[];
};

ساکن اندرونی ساخت libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  ساخت libcrun_fd_map *ret;
  size_t i;

  ret = (ساخت libcrun_fd_map *) xmalloc (حجم (*ret) + حجم (صحیح) * len);
  ret->nfds = len;
  برائے (i = 0; i < len; i++)
    ret->fds[i] = -1;

  واپس ret;
}

ساکن اندرونی خالی
cleanup_close_mapp (ساخت libcrun_fd_map **p)
{
  ساخت libcrun_fd_map *m = *p;
  size_t i;

  اگر (m == تہی)
    واپس;

  برائے (i = 0; i < m->nfds; i++)
    اگر (m->fds[i] >= 0)
      عارضی_FAILURE_RETRY (close (m->fds[i]));

  آزاد (m);
}

ساکن اندرونی خالی
cleanup_close_vecp (صحیح **p)
{
  صحیح *pp = *p;
  صحیح i;

  اگر (pp == تہی)
    واپس;

  برائے (i = 0; pp[i] >= 0; i++)
    عارضی_FAILURE_RETRY (close (pp[i]));
}

ساکن اندرونی خالی
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  اگر (dir)
    closedir (dir);
}

ساکن اندرونی صحیح
close_and_reset (صحیح *fd)
{
  صحیح ret = 0;
  اگر (*fd >= 0)
    {
      ret = عارضی_FAILURE_RETRY (close (*fd));
      اگر (LIKELY (ret == 0))
        *fd = -1;
    }
  واپس ret;
}

ساکن اندرونی حرف *
xstrdup (ثابت حرف *str)
{
  حرف *ret;
  اگر (str == تہی)
    واپس تہی;

  ret = strdup (str);
  اگر (ret == تہی)
    OOM ();

  واپس ret;
}

خالی consume_trailing_slashes (حرف *راستہ);

#تعریف_کرو consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    جب (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

ساکن اندرونی bool
راستہ_is_slash_dev (ثابت حرف *راستہ)
{
  راستہ = consume_slashes (راستہ);

  اگر (لڑی_موازنہ (راستہ, "dev", 3))
    واپس false;

  راستہ += 3;

  /* Check there are only '/' left.  */
  برائے (; *راستہ; راستہ++)
    اگر (*راستہ != '/')
      واپس false;

  واپس true;
}

صحیح xasprintf (حرف **str, ثابت حرف *fmt, ...) __attribute__ ((format (لکھو, 2, 3)));

صحیح crun_راستہ_exists (ثابت حرف *راستہ, libcrun_error_t *err);

صحیح write_file_at_with_flags (صحیح dirfd, صحیح flags, mode_t منوال, ثابت حرف *name, ثابت خالی *ڈیٹا, size_t len, libcrun_error_t *err);

ساکن اندرونی صحیح
write_file (ثابت حرف *name, ثابت خالی *ڈیٹا, size_t len, libcrun_error_t *err)
{
  واپس write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, ڈیٹا, len, err);
}

ساکن اندرونی صحیح
write_file_at (صحیح dirfd, ثابت حرف *name, ثابت خالی *ڈیٹا, size_t len, libcrun_error_t *err)
{
  واپس write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, ڈیٹا, len, err);
}

صحیح crun_ensure_directory (ثابت حرف *راستہ, صحیح منوال, bool nofollow, libcrun_error_t *err);

صحیح crun_ensure_directory_at (صحیح dirfd, ثابت حرف *راستہ, صحیح منوال, bool nofollow, libcrun_error_t *err);

صحیح crun_محفوظ_create_and_open_ref_at (bool dir, صحیح dirfd, ثابت حرف *dirpath, ثابت حرف *راستہ, صحیح منوال, libcrun_error_t *err);

صحیح crun_محفوظ_ensure_directory_at (صحیح dirfd, ثابت حرف *dirpath, ثابت حرف *راستہ, صحیح منوال,
                                   libcrun_error_t *err);

صحیح crun_محفوظ_ensure_file_at (صحیح dirfd, ثابت حرف *dirpath, ثابت حرف *راستہ, صحیح منوال,
                              libcrun_error_t *err);

صحیح crun_dir_p (ثابت حرف *راستہ, bool nofollow, libcrun_error_t *err);

صحیح crun_dir_p_at (صحیح dirfd, ثابت حرف *راستہ, bool nofollow, libcrun_error_t *err);

صحیح detach_process ();

صحیح create_file_if_missing_at (صحیح dirfd, ثابت حرف *file, mode_t منوال, libcrun_error_t *err);

صحیح check_running_اندر_user_namespace (libcrun_error_t *err);

صحیح سیٹ_selinux_label (libcrun_container_t *container, ثابت حرف *label, bool now, libcrun_error_t *err);

صحیح جوڑو_selinux_mount_label (حرف **ret, ثابت حرف *ڈیٹا, ثابت حرف *label, ثابت حرف *context_type, libcrun_error_t *err);

صحیح سیٹ_apparmor_profile (libcrun_container_t *container, ثابت حرف *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

صحیح read_all_fd_with_سائز_hint (صحیح fd, ثابت حرف *description, حرف **برآمد, size_t *len, size_t hint, libcrun_error_t *err);

صحیح read_all_fd_with_max_سائز (صحیح fd, ثابت حرف *description, حرف **برآمد, size_t *len, size_t max_سائز, libcrun_error_t *err);

ساکن اندرونی صحیح
read_all_fd (صحیح fd, ثابت حرف *description, حرف **برآمد, size_t *len, libcrun_error_t *err)
{
  واپس read_all_fd_with_سائز_hint (fd, description, برآمد, len, 0, err);
}

صحیح get_realpath_مقصد_file (صحیح dirfd, ثابت حرف *راستہ_name, حرف **absolute_راستہ, libcrun_error_t *err);

صحیح read_all_file (ثابت حرف *راستہ, حرف **برآمد, size_t *len, libcrun_error_t *err);

صحیح read_all_file_at (صحیح dirfd, ثابت حرف *راستہ, حرف **برآمد, size_t *len, libcrun_error_t *err);

صحیح open_unix_domain_client_socket (ثابت حرف *راستہ, صحیح dgram, libcrun_error_t *err);

صحیح open_unix_domain_socket (ثابت حرف *راستہ, صحیح dgram, libcrun_error_t *err);

صحیح send_fd_مقصد_socket (صحیح server, صحیح fd, libcrun_error_t *err);

صحیح send_fd_مقصد_socket_with_payload (صحیح server, صحیح fd, ثابت حرف *payload, size_t payload_len, libcrun_error_t *err);

صحیح create_socket_pair (صحیح *pair, libcrun_error_t *err);

صحیح receive_fd_from_socket (صحیح from, libcrun_error_t *err);

صحیح receive_fd_from_socket_with_payload (صحیح from, حرف *payload, size_t payload_len, libcrun_error_t *err);

صحیح create_signalfd (sigset_t *نقاب, libcrun_error_t *err);

صحیح epoll_helper (صحیح *اندر_fds, صحیح *اندر_levelfds, صحیح *برآمد_fds, صحیح *برآمد_levelfds, libcrun_error_t *err);

صحیح copy_from_fd_مقصد_fd (صحیح src, صحیح dst, صحیح consume, libcrun_error_t *err);

صحیح تسلسل_process (حرف **دلائل, libcrun_error_t *err);

صحیح format_default_id_mapping (حرف **برآمد, uid_t container_id, uid_t host_uid, uid_t host_id, صحیح is_uid, libcrun_error_t *err);

صحیح تسلسل_process_with_stdin_timeout_envp (حرف *راستہ, حرف **دلائل, ثابت حرف *cwd, صحیح timeout, حرف **envp,
                                         حرف *stdin, size_t stdin_len, صحیح برآمد_fd, صحیح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

صحیح mark_or_close_fds_ge_than (libcrun_container_t *container, صحیح n, bool close_now, libcrun_error_t *err);

خالی get_current_timestamp (حرف *برآمد, size_t len);

صحیح سیٹ_blocking_fd (صحیح fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_عوامی صحیح parse_json_file (json_object **برآمد, ثابت حرف *jsondata, ساخت parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
ساکن اندرونی صحیح
has_سابقہ (ثابت حرف *str, ثابت حرف *سابقہ)
{
  size_t سابقہ_len;

  اگر (! str || ! سابقہ)
    واپس 0;

  سابقہ_len = لڑی_لمبائی (سابقہ);
  واپس لڑی_لمبائی (str) >= سابقہ_len && memcmp (str, سابقہ, سابقہ_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
ساکن اندرونی صحیح
has_dir_سابقہ (ثابت حرف *str, ثابت حرف *سابقہ)
{
  size_t سابقہ_len;

  اگر (! str || ! سابقہ)
    واپس 0;

  سابقہ_len = لڑی_لمبائی (سابقہ);
  واپس لڑی_لمبائی (str) > سابقہ_len && memcmp (str, سابقہ, سابقہ_len) == 0 && str[سابقہ_len] == '/';
}

صحیح ڈھونڈو_executable (حرف **exec_راستہ, ثابت حرف *executable_راستہ, ثابت حرف *cwd, libcrun_error_t *err);

صحیح copy_recursive_fd_مقصد_fd (صحیح srcfd, صحیح destfd, ثابت حرف *srcname, ثابت حرف *destname, libcrun_error_t *err);

صحیح سیٹ_home_env (uid_t uid);

صحیح libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

صحیح libcrun_initialize_apparmor (libcrun_error_t *err);

ثابت حرف *ڈھونڈو_annotation (libcrun_container_t *container, ثابت حرف *name);

صحیح get_file_type_at (صحیح dirfd, mode_t *منوال, bool nofollow, ثابت حرف *راستہ);

صحیح get_file_type (mode_t *منوال, bool nofollow, ثابت حرف *راستہ);

صحیح get_file_type_fd (صحیح fd, mode_t *منوال);

حرف *get_user_name (uid_t uid);

صحیح محفوظ_openat (صحیح dirfd, ثابت حرف *rootfs, ثابت حرف *راستہ, صحیح flags, صحیح منوال,
                 libcrun_error_t *err);

صحیح محفوظ_write (صحیح fd, ثابت حرف *fname, ثابت خالی *buf, size_t گنتی, libcrun_error_t *err);

صحیح append_paths (حرف **برآمد, libcrun_error_t *err, ...) __attribute__ ((sentinel));

صحیح str2sig (ثابت حرف *name);

صحیح base64_decode (ثابت حرف *iptr, size_t isize, حرف *optr, size_t osize, size_t *nbytes);
صحیح has_suffix (ثابت حرف *ماخذ, ثابت حرف *suffix);
حرف *libcrun_sysctl_کلید_مقصد_proc_راستہ (ثابت حرف *کلید);
حرف *str_join_array (صحیح انحراف, size_t سائز, حرف *ثابت array[], ثابت حرف *joint);

ssize_t محفوظ_readlinkat (صحیح dfd, ثابت حرف *name, حرف **بفر, ssize_t hint, libcrun_error_t *err);

حرف **read_dir_entries (ثابت حرف *راستہ, libcrun_error_t *err);

ساکن اندرونی bool
is_خالی_خانے_string (ثابت حرف *s)
{
  واپس s == تہی || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
ساکن اندرونی size_t
محفوظ_strlen (ثابت حرف *s)
{
  واپس s == تہی ? 0 : لڑی_لمبائی (s);
}

ساکن اندرونی bool
راستہ_has_dot_dot_component (ثابت حرف *راستہ)
{
  اگر (راستہ == تہی)
    واپس false;
  cleanup_free حرف *npath = xstrdup (راستہ);
  حرف *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  جب (cur)
    {
      اگر (it)
        *it = '\0';

      اگر (strcmp (cur, "..") == 0)
        واپس true;

      اگر (it == تہی)
        توڑو;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  واپس false;
}

ساکن اندرونی صحیح
waitpid_ignore_stopped (pid_t pid, صحیح *حالت_2, صحیح options)
{
  صحیح ret, s;
  کرو
    {
      s = 0;
      ret = عارضی_FAILURE_RETRY (waitpid (pid, &s, options));
      اگر (ret < 0)
        واپس ret;
  } جب (WIFSTOPPED (s) || WIFCONTINUED (s));

  اگر (حالت_2)
    *حالت_2 = s;

  واپس ret;
}

ساکن اندرونی صحیح
get_process_exit_حالت (صحیح حالت_2)
{
  اگر (WIFEXITED (حالت_2))
    واپس WEXITSTATUS (حالت_2);
  اگر (WIFSIGNALED (حالت_2))
    واپس 128 + WTERMSIG (حالت_2);

  واپس -1;
}

uid_t get_حد_سے_تجاوز_uid (خالی);
gid_t get_حد_سے_تجاوز_gid (خالی);

/* Adapted from systemd.  Include space for the NUL byte.  */
#تعریف_کرو DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (حجم (type) <= 1 ? 3U : حجم (type) <= 2 ? 5U  \
                                        : حجم (type) <= 4   ? 10U \
                                        : حجم (type) <= 8   ? 20U \
                                                               : حجم (صحیح[-2 * (حجم (type) > 8)])))

#تعریف_کرو _STRLEN(s) (حجم (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#تعریف_کرو PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (صحیح))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
قسمتعریف حرف proc_fd_راستہ_t[PROC_PID_FD_STRLEN];

#تعریف_منسوخ _STRLEN

ساکن اندرونی خالی
get_proc_fd_راستہ (proc_fd_راستہ_t راستہ, pid_t pid, صحیح fd)
{
  ثابت size_t max_len = حجم (proc_fd_راستہ_t);
  size_t n;

  اگر (pid)
    n = snprintf (راستہ, max_len, "/proc/%d/fd/%d", pid, fd);
  ورنہ
    n = snprintf (راستہ, max_len, "/proc/self/fd/%d", fd);

  اگر (UNLIKELY (n >= max_len))
    abort ();
}

ساکن اندرونی خالی
get_proc_self_fd_راستہ (proc_fd_راستہ_t راستہ, صحیح fd)
{
  get_proc_fd_راستہ (راستہ, 0, fd);
}

ساکن اندرونی خالی
get_self_fd_راستہ (proc_fd_راستہ_t راستہ, صحیح fd)
{
  snprintf (راستہ, حجم (proc_fd_راستہ_t), "self/fd/%d", fd);
}

ساکن اندرونی صحیح
validate_options (غیرعلامتی صحیح specified_options, غیرعلامتی صحیح supported_options, libcrun_error_t *err)
{
  اگر (! ! (~supported_options & specified_options))
    واپس crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  واپس 0;
}

خارجی صحیح cpuset_string_مقصد_bitmask (ثابت حرف *str, حرف **برآمد, size_t *برآمد_سائز, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
ساخت channel_fd_pair;

ساخت channel_fd_pair *channel_fd_pair_new (صحیح اندر_fd, صحیح برآمد_fd, size_t سائز);

خالی channel_fd_pair_free (ساخت channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
صحیح channel_fd_pair_process (ساخت channel_fd_pair *channel, صحیح epollfd, libcrun_error_t *err);

ساکن اندرونی خالی
cleanup_channel_fd_pairp (خالی *p)
{
  ساخت channel_fd_pair **pp = (ساخت channel_fd_pair **) p;
  اگر (*pp == تہی)
    واپس;

  channel_fd_pair_free (*pp);
}
#تعریف_کرو cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

صحیح libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_open_proc_file (libcrun_container_t *container, ثابت حرف *راستہ, صحیح flags, libcrun_error_t *err);
صحیح libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ثابت حرف *راستہ, صحیح flags, libcrun_error_t *err);

#اگر_ختم
