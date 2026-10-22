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
#جے_بنا_تعریف UTILS_H
#تعریف_کرو UTILS_H

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

#جے_بنا_تعریف عارضی_FAILURE_RETRY
#  define عارضی_FAILURE_RETRY(عبارت)      \
    (__extension__ ({                         \
      لما صحيح __result;                      \
      افعل                                      \
        __result = (لما صحيح) (عبارت);   \
      طالما (__result < 0 && errno == EINTR); \
      __result;                               \
    }))
#جے_انت

#تعریف_کرو cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#تعریف_کرو cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#تعریف_کرو cleanup_close __attribute__ ((cleanup (cleanup_closep)))
#تعریف_کرو cleanup_close_vec __attribute__ ((cleanup (cleanup_close_vecp)))
#تعریف_کرو cleanup_close_map __attribute__ ((cleanup (cleanup_close_mapp)))
#تعریف_کرو cleanup_dir __attribute__ ((cleanup (cleanup_dirp)))
#تعریف_کرو arg_بن_ورتیا __attribute__ ((بن_ورتیا))
#تعریف_کرو cleanup_pid __attribute__ ((cleanup (cleanup_pidp)))
#تعریف_کرو cleanup_mmap __attribute__ ((cleanup (cleanup_mmapp)))

#تعریف_کرو LIKELY(x) __builtin_expect ((x), 1)
#تعریف_کرو UNLIKELY(x) __builtin_expect ((x), 0)

#تعریف_کرو WRITE_FILE_DEFAULT_FLAGS (O_CLOEXEC | O_CREAT | O_TRUNC | O_WRONLY)

__attribute__ ((حافظہ_دیو)) ساكن مضمن فراغ *
xmalloc (size_t ناپ)
{
  فراغ *res = حافظہ_دیو (ناپ);
  إذا (UNLIKELY (res == عدم))
    OOM ();
  أرجع res;
}

__attribute__ ((حافظہ_دیو)) ساكن مضمن فراغ *
xmalloc0 (size_t ناپ)
{
  فراغ *res = صفر_حافظہ_دیو (1, ناپ);
  إذا (UNLIKELY (res == عدم))
    OOM ();
  أرجع res;
}

__attribute__ ((حافظہ_دیو)) ساكن مضمن فراغ *
xrealloc (فراغ *ptr, size_t ناپ)
{
  فراغ *res = realloc (ptr, ناپ);
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
    عارضی_FAILURE_RETRY (close (*pp));
}

ساكن مضمن فراغ
cleanup_pidp (فراغ *p)
{
  pid_t *pp = (pid_t *) p;
  إذا (*pp > 0)
    {
      عارضی_FAILURE_RETRY (kill (*pp, SIGKILL));
      عارضی_FAILURE_RETRY (waitpid (*pp, عدم, 0));
    }
}

بنية libcrun_mmap_s
{
  فراغ *addr;
  size_t لمائی;
};

صحيح libcrun_mmap (بنية libcrun_mmap_s **ret, فراغ *addr, size_t لمائی,
                  صحيح prot, صحيح flags, صحيح fd, off_t وتھ,
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
      عارضی_FAILURE_RETRY (close (m->fds[i]));

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
    عارضی_FAILURE_RETRY (close (pp[i]));
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
      ret = عارضی_FAILURE_RETRY (close (*fd));
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

فراغ consume_trailing_slashes (حرف *رستہ);

#تعریف_کرو consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    طالما (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

ساكن مضمن bool
رستہ_is_slash_dev (ثابت حرف *رستہ)
{
  رستہ = consume_slashes (رستہ);

  إذا (لڑی_مقابلہ (رستہ, "dev", 3))
    أرجع false;

  رستہ += 3;

  /* Check there are only '/' left.  */
  لكل (; *رستہ; رستہ++)
    إذا (*رستہ != '/')
      أرجع false;

  أرجع true;
}

صحيح xasprintf (حرف **str, ثابت حرف *fmt, ...) __attribute__ ((format (لکھو, 2, 3)));

صحيح crun_رستہ_exists (ثابت حرف *رستہ, libcrun_error_t *err);

صحيح write_file_at_with_flags (صحيح dirfd, صحيح flags, mode_t منوال, ثابت حرف *name, ثابت فراغ *انکڑے, size_t len, libcrun_error_t *err);

ساكن مضمن صحيح
write_file (ثابت حرف *name, ثابت فراغ *انکڑے, size_t len, libcrun_error_t *err)
{
  أرجع write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, انکڑے, len, err);
}

ساكن مضمن صحيح
write_file_at (صحيح dirfd, ثابت حرف *name, ثابت فراغ *انکڑے, size_t len, libcrun_error_t *err)
{
  أرجع write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, انکڑے, len, err);
}

صحيح crun_ensure_directory (ثابت حرف *رستہ, صحيح منوال, bool nofollow, libcrun_error_t *err);

صحيح crun_ensure_directory_at (صحيح dirfd, ثابت حرف *رستہ, صحيح منوال, bool nofollow, libcrun_error_t *err);

صحيح crun_محفوظ_create_and_open_ref_at (bool dir, صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *رستہ, صحيح منوال, libcrun_error_t *err);

صحيح crun_محفوظ_ensure_directory_at (صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *رستہ, صحيح منوال,
                                   libcrun_error_t *err);

صحيح crun_محفوظ_ensure_file_at (صحيح dirfd, ثابت حرف *dirpath, ثابت حرف *رستہ, صحيح منوال,
                              libcrun_error_t *err);

صحيح crun_dir_p (ثابت حرف *رستہ, bool nofollow, libcrun_error_t *err);

صحيح crun_dir_p_at (صحيح dirfd, ثابت حرف *رستہ, bool nofollow, libcrun_error_t *err);

صحيح detach_process ();

صحيح create_file_if_missing_at (صحيح dirfd, ثابت حرف *file, mode_t منوال, libcrun_error_t *err);

صحيح check_running_وچ_user_namespace (libcrun_error_t *err);

صحيح سیٹ_selinux_label (libcrun_container_t *container, ثابت حرف *label, bool now, libcrun_error_t *err);

صحيح جوڑو_selinux_mount_label (حرف **ret, ثابت حرف *انکڑے, ثابت حرف *label, ثابت حرف *context_type, libcrun_error_t *err);

صحيح سیٹ_apparmor_profile (libcrun_container_t *container, ثابت حرف *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

صحيح read_all_fd_with_ناپ_hint (صحيح fd, ثابت حرف *description, حرف **باہرلا, size_t *len, size_t hint, libcrun_error_t *err);

صحيح read_all_fd_with_max_ناپ (صحيح fd, ثابت حرف *description, حرف **باہرلا, size_t *len, size_t max_ناپ, libcrun_error_t *err);

ساكن مضمن صحيح
read_all_fd (صحيح fd, ثابت حرف *description, حرف **باہرلا, size_t *len, libcrun_error_t *err)
{
  أرجع read_all_fd_with_ناپ_hint (fd, description, باہرلا, len, 0, err);
}

صحيح get_realpath_منزل_file (صحيح dirfd, ثابت حرف *رستہ_name, حرف **absolute_رستہ, libcrun_error_t *err);

صحيح read_all_file (ثابت حرف *رستہ, حرف **باہرلا, size_t *len, libcrun_error_t *err);

صحيح read_all_file_at (صحيح dirfd, ثابت حرف *رستہ, حرف **باہرلا, size_t *len, libcrun_error_t *err);

صحيح open_unix_domain_client_socket (ثابت حرف *رستہ, صحيح dgram, libcrun_error_t *err);

صحيح open_unix_domain_socket (ثابت حرف *رستہ, صحيح dgram, libcrun_error_t *err);

صحيح send_fd_منزل_socket (صحيح server, صحيح fd, libcrun_error_t *err);

صحيح send_fd_منزل_socket_with_payload (صحيح server, صحيح fd, ثابت حرف *payload, size_t payload_len, libcrun_error_t *err);

صحيح create_socket_pair (صحيح *pair, libcrun_error_t *err);

صحيح receive_fd_from_socket (صحيح from, libcrun_error_t *err);

صحيح receive_fd_from_socket_with_payload (صحيح from, حرف *payload, size_t payload_len, libcrun_error_t *err);

صحيح create_signalfd (sigset_t *نقاب, libcrun_error_t *err);

صحيح epoll_helper (صحيح *وچ_fds, صحيح *وچ_levelfds, صحيح *باہرلا_fds, صحيح *باہرلا_levelfds, libcrun_error_t *err);

صحيح copy_from_fd_منزل_fd (صحيح src, صحيح dst, صحيح consume, libcrun_error_t *err);

صحيح لگاتار_process (حرف **دلیلاں, libcrun_error_t *err);

صحيح format_default_id_mapping (حرف **باہرلا, uid_t container_id, uid_t host_uid, uid_t host_id, صحيح is_uid, libcrun_error_t *err);

صحيح لگاتار_process_with_stdin_timeout_envp (حرف *رستہ, حرف **دلیلاں, ثابت حرف *cwd, صحيح timeout, حرف **envp,
                                         حرف *stdin, size_t stdin_len, صحيح باہرلا_fd, صحيح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

صحيح mark_or_close_fds_ge_than (libcrun_container_t *container, صحيح n, bool close_now, libcrun_error_t *err);

فراغ get_current_timestamp (حرف *باہرلا, size_t len);

صحيح سیٹ_blocking_fd (صحيح fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_عوامی صحيح parse_json_file (json_object **باہرلا, ثابت حرف *jsondata, بنية parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
ساكن مضمن صحيح
has_اگیتر (ثابت حرف *str, ثابت حرف *اگیتر)
{
  size_t اگیتر_len;

  إذا (! str || ! اگیتر)
    أرجع 0;

  اگیتر_len = لڑی_لمبائی (اگیتر);
  أرجع لڑی_لمبائی (str) >= اگیتر_len && memcmp (str, اگیتر, اگیتر_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
ساكن مضمن صحيح
has_dir_اگیتر (ثابت حرف *str, ثابت حرف *اگیتر)
{
  size_t اگیتر_len;

  إذا (! str || ! اگیتر)
    أرجع 0;

  اگیتر_len = لڑی_لمبائی (اگیتر);
  أرجع لڑی_لمبائی (str) > اگیتر_len && memcmp (str, اگیتر, اگیتر_len) == 0 && str[اگیتر_len] == '/';
}

صحيح لبھو_executable (حرف **exec_رستہ, ثابت حرف *executable_رستہ, ثابت حرف *cwd, libcrun_error_t *err);

صحيح copy_recursive_fd_منزل_fd (صحيح srcfd, صحيح destfd, ثابت حرف *srcname, ثابت حرف *destname, libcrun_error_t *err);

صحيح سیٹ_home_env (uid_t uid);

صحيح libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

صحيح libcrun_initialize_apparmor (libcrun_error_t *err);

ثابت حرف *لبھو_annotation (libcrun_container_t *container, ثابت حرف *name);

صحيح get_file_type_at (صحيح dirfd, mode_t *منوال, bool nofollow, ثابت حرف *رستہ);

صحيح get_file_type (mode_t *منوال, bool nofollow, ثابت حرف *رستہ);

صحيح get_file_type_fd (صحيح fd, mode_t *منوال);

حرف *get_user_name (uid_t uid);

صحيح محفوظ_openat (صحيح dirfd, ثابت حرف *rootfs, ثابت حرف *رستہ, صحيح flags, صحيح منوال,
                 libcrun_error_t *err);

صحيح محفوظ_write (صحيح fd, ثابت حرف *fname, ثابت فراغ *buf, size_t گنتی, libcrun_error_t *err);

صحيح append_paths (حرف **باہرلا, libcrun_error_t *err, ...) __attribute__ ((sentinel));

صحيح str2sig (ثابت حرف *name);

صحيح base64_decode (ثابت حرف *iptr, size_t isize, حرف *optr, size_t osize, size_t *nbytes);
صحيح has_suffix (ثابت حرف *سروت, ثابت حرف *suffix);
حرف *libcrun_sysctl_کلید_منزل_proc_رستہ (ثابت حرف *کلید);
حرف *str_join_array (صحيح وتھ, size_t ناپ, حرف *ثابت array[], ثابت حرف *joint);

ssize_t محفوظ_readlinkat (صحيح dfd, ثابت حرف *name, حرف **بفر, ssize_t hint, libcrun_error_t *err);

حرف **read_dir_entries (ثابت حرف *رستہ, libcrun_error_t *err);

ساكن مضمن bool
is_خالی_تھاواں_string (ثابت حرف *s)
{
  أرجع s == عدم || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
ساكن مضمن size_t
محفوظ_strlen (ثابت حرف *s)
{
  أرجع s == عدم ? 0 : لڑی_لمبائی (s);
}

ساكن مضمن bool
رستہ_has_dot_dot_component (ثابت حرف *رستہ)
{
  إذا (رستہ == عدم)
    أرجع false;
  cleanup_free حرف *npath = xstrdup (رستہ);
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
      ret = عارضی_FAILURE_RETRY (waitpid (pid, &s, options));
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

uid_t get_حد_توں_ودھ_uid (فراغ);
gid_t get_حد_توں_ودھ_gid (فراغ);

/* Adapted from systemd.  Include space for the NUL byte.  */
#تعریف_کرو DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (حجم (type) <= 1 ? 3U : حجم (type) <= 2 ? 5U  \
                                        : حجم (type) <= 4   ? 10U \
                                        : حجم (type) <= 8   ? 20U \
                                                               : حجم (صحيح[-2 * (حجم (type) > 8)])))

#تعریف_کرو _STRLEN(s) (حجم (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#تعریف_کرو PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (صحيح))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
تعريفنوع حرف proc_fd_رستہ_t[PROC_PID_FD_STRLEN];

#تعریف_رد _STRLEN

ساكن مضمن فراغ
get_proc_fd_رستہ (proc_fd_رستہ_t رستہ, pid_t pid, صحيح fd)
{
  ثابت size_t max_len = حجم (proc_fd_رستہ_t);
  size_t n;

  إذا (pid)
    n = snprintf (رستہ, max_len, "/proc/%d/fd/%d", pid, fd);
  وإلا
    n = snprintf (رستہ, max_len, "/proc/self/fd/%d", fd);

  إذا (UNLIKELY (n >= max_len))
    abort ();
}

ساكن مضمن فراغ
get_proc_self_fd_رستہ (proc_fd_رستہ_t رستہ, صحيح fd)
{
  get_proc_fd_رستہ (رستہ, 0, fd);
}

ساكن مضمن فراغ
get_self_fd_رستہ (proc_fd_رستہ_t رستہ, صحيح fd)
{
  snprintf (رستہ, حجم (proc_fd_رستہ_t), "self/fd/%d", fd);
}

ساكن مضمن صحيح
validate_options (بنانشانی صحيح specified_options, بنانشانی صحيح supported_options, libcrun_error_t *err)
{
  إذا (! ! (~supported_options & specified_options))
    أرجع crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  أرجع 0;
}

خارجي صحيح cpuset_string_منزل_bitmask (ثابت حرف *str, حرف **باہرلا, size_t *باہرلا_ناپ, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
بنية channel_fd_pair;

بنية channel_fd_pair *channel_fd_pair_new (صحيح وچ_fd, صحيح باہرلا_fd, size_t ناپ);

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
#تعریف_کرو cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

صحيح libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
صحيح libcrun_open_proc_file (libcrun_container_t *container, ثابت حرف *رستہ, صحيح flags, libcrun_error_t *err);
صحيح libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ثابت حرف *رستہ, صحيح flags, libcrun_error_t *err);

#جے_انت
