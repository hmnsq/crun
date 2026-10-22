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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ UTILS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UTILS_H

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <dirent.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/wait.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਆਰਜ਼ੀ_FAILURE_RETRY
#  define ਆਰਜ਼ੀ_FAILURE_RETRY(ਵਿਅੰਜਕ_2)      \
    (__extension__ ({                         \
      ਲੰਮਾ ਪੂਰਨ __result;                      \
      ਕਰੋ                                      \
        __result = (ਲੰਮਾ ਪੂਰਨ) (ਵਿਅੰਜਕ_2);   \
      ਜਦੋਂ (__result < 0 && errno == EINTR); \
      __result;                               \
    }))
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_close __attribute__ ((cleanup (cleanup_closep)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_close_vec __attribute__ ((cleanup (cleanup_close_vecp)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_close_map __attribute__ ((cleanup (cleanup_close_mapp)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_dir __attribute__ ((cleanup (cleanup_dirp)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arg_ਅਣਵਰਤਿਆ __attribute__ ((ਅਣਵਰਤਿਆ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_pid __attribute__ ((cleanup (cleanup_pidp)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_mmap __attribute__ ((cleanup (cleanup_mmapp)))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LIKELY(x) __builtin_expect ((x), 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNLIKELY(x) __builtin_expect ((x), 0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WRITE_FILE_DEFAULT_FLAGS (O_CLOEXEC | O_CREAT | O_TRUNC | O_WRONLY)

__attribute__ ((ਸਮ੍ਰਿਤੀ_ਦਿਓ)) ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *
xmalloc (size_t ਮਾਪ)
{
  ਖਾਲੀ *res = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਮਾਪ);
  ਜੇ (UNLIKELY (res == ਨਿਰਾ))
    OOM ();
  ਵਾਪਸ res;
}

__attribute__ ((ਸਮ੍ਰਿਤੀ_ਦਿਓ)) ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *
xmalloc0 (size_t ਮਾਪ)
{
  ਖਾਲੀ *res = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ (1, ਮਾਪ);
  ਜੇ (UNLIKELY (res == ਨਿਰਾ))
    OOM ();
  ਵਾਪਸ res;
}

__attribute__ ((ਸਮ੍ਰਿਤੀ_ਦਿਓ)) ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *
xrealloc (ਖਾਲੀ *ptr, size_t ਮਾਪ)
{
  ਖਾਲੀ *res = realloc (ptr, ਮਾਪ);
  ਜੇ (UNLIKELY (res == ਨਿਰਾ))
    OOM ();
  ਵਾਪਸ res;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_freep (ਖਾਲੀ *p)
{
  ਖਾਲੀ **pp = (ਖਾਲੀ **) p;
  ਮੁਕਤ (*pp);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  ਜੇ (file)
    (ਖਾਲੀ) fclose (file);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_closep (ਖਾਲੀ *p)
{
  ਪੂਰਨ *pp = (ਪੂਰਨ *) p;
  ਜੇ (*pp >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (*pp));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_pidp (ਖਾਲੀ *p)
{
  pid_t *pp = (pid_t *) p;
  ਜੇ (*pp > 0)
    {
      ਆਰਜ਼ੀ_FAILURE_RETRY (kill (*pp, SIGKILL));
      ਆਰਜ਼ੀ_FAILURE_RETRY (waitpid (*pp, ਨਿਰਾ, 0));
    }
}

ਬਣਤਰ libcrun_mmap_s
{
  ਖਾਲੀ *addr;
  size_t ਲੰਮਾਈ;
};

ਪੂਰਨ libcrun_mmap (ਬਣਤਰ libcrun_mmap_s **ret, ਖਾਲੀ *addr, size_t ਲੰਮਾਈ,
                  ਪੂਰਨ prot, ਪੂਰਨ flags, ਪੂਰਨ fd, off_t ਵਿੱਥ,
                  libcrun_error_t *err);

ਪੂਰਨ libcrun_munmap (ਬਣਤਰ libcrun_mmap_s *mmap, libcrun_error_t *err);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_mmapp (ਖਾਲੀ *p)
{
  ਪੂਰਨ ret;
  libcrun_error_t tmp_err = ਨਿਰਾ;
  ਬਣਤਰ libcrun_mmap_s **mm;

  mm = (ਬਣਤਰ libcrun_mmap_s **) p;
  ਜੇ (*mm == ਨਿਰਾ)
    ਵਾਪਸ;

  ret = libcrun_munmap (*mm, &tmp_err);
  ਜੇ (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

ਬਣਤਰ libcrun_fd_map
{
  size_t nfds;
  ਪੂਰਨ fds[];
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  ਬਣਤਰ libcrun_fd_map *ret;
  size_t i;

  ret = (ਬਣਤਰ libcrun_fd_map *) xmalloc (ਆਕਾਰ (*ret) + ਆਕਾਰ (ਪੂਰਨ) * len);
  ret->nfds = len;
  ਲਈ (i = 0; i < len; i++)
    ret->fds[i] = -1;

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_close_mapp (ਬਣਤਰ libcrun_fd_map **p)
{
  ਬਣਤਰ libcrun_fd_map *m = *p;
  size_t i;

  ਜੇ (m == ਨਿਰਾ)
    ਵਾਪਸ;

  ਲਈ (i = 0; i < m->nfds; i++)
    ਜੇ (m->fds[i] >= 0)
      ਆਰਜ਼ੀ_FAILURE_RETRY (close (m->fds[i]));

  ਮੁਕਤ (m);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_close_vecp (ਪੂਰਨ **p)
{
  ਪੂਰਨ *pp = *p;
  ਪੂਰਨ i;

  ਜੇ (pp == ਨਿਰਾ)
    ਵਾਪਸ;

  ਲਈ (i = 0; pp[i] >= 0; i++)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (pp[i]));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  ਜੇ (dir)
    closedir (dir);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
close_and_reset (ਪੂਰਨ *fd)
{
  ਪੂਰਨ ret = 0;
  ਜੇ (*fd >= 0)
    {
      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (close (*fd));
      ਜੇ (LIKELY (ret == 0))
        *fd = -1;
    }
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ *
xstrdup (ਅਚਲ ਅੱਖਰ *str)
{
  ਅੱਖਰ *ret;
  ਜੇ (str == ਨਿਰਾ)
    ਵਾਪਸ ਨਿਰਾ;

  ret = strdup (str);
  ਜੇ (ret == ਨਿਰਾ)
    OOM ();

  ਵਾਪਸ ret;
}

ਖਾਲੀ consume_trailing_slashes (ਅੱਖਰ *ਰਸਤਾ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    ਜਦੋਂ (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ਰਸਤਾ_is_slash_dev (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
  ਰਸਤਾ = consume_slashes (ਰਸਤਾ);

  ਜੇ (ਲੜੀ_ਤੁਲਨਾ (ਰਸਤਾ, "dev", 3))
    ਵਾਪਸ false;

  ਰਸਤਾ += 3;

  /* Check there are only '/' left.  */
  ਲਈ (; *ਰਸਤਾ; ਰਸਤਾ++)
    ਜੇ (*ਰਸਤਾ != '/')
      ਵਾਪਸ false;

  ਵਾਪਸ true;
}

ਪੂਰਨ xasprintf (ਅੱਖਰ **str, ਅਚਲ ਅੱਖਰ *fmt, ...) __attribute__ ((format (ਲਿਖੋ, 2, 3)));

ਪੂਰਨ crun_ਰਸਤਾ_exists (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err);

ਪੂਰਨ write_file_at_with_flags (ਪੂਰਨ dirfd, ਪੂਰਨ flags, mode_t ਬਹੁਲਕ, ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, size_t len, libcrun_error_t *err);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
write_file (ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, size_t len, libcrun_error_t *err)
{
  ਵਾਪਸ write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, ਅੰਕੜੇ, len, err);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
write_file_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, size_t len, libcrun_error_t *err)
{
  ਵਾਪਸ write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, ਅੰਕੜੇ, len, err);
}

ਪੂਰਨ crun_ensure_directory (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ, bool nofollow, libcrun_error_t *err);

ਪੂਰਨ crun_ensure_directory_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ, bool nofollow, libcrun_error_t *err);

ਪੂਰਨ crun_ਸੁਰੱਖਿਅਤ_create_and_open_ref_at (bool dir, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *dirpath, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ, libcrun_error_t *err);

ਪੂਰਨ crun_ਸੁਰੱਖਿਅਤ_ensure_directory_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *dirpath, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ,
                                   libcrun_error_t *err);

ਪੂਰਨ crun_ਸੁਰੱਖਿਅਤ_ensure_file_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *dirpath, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ,
                              libcrun_error_t *err);

ਪੂਰਨ crun_dir_p (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool nofollow, libcrun_error_t *err);

ਪੂਰਨ crun_dir_p_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool nofollow, libcrun_error_t *err);

ਪੂਰਨ detach_process ();

ਪੂਰਨ create_file_if_missing_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *file, mode_t ਬਹੁਲਕ, libcrun_error_t *err);

ਪੂਰਨ check_running_ਵਿੱਚ_user_namespace (libcrun_error_t *err);

ਪੂਰਨ ਸੈੱਟ_selinux_label (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *label, bool now, libcrun_error_t *err);

ਪੂਰਨ ਜੋੜੋ_selinux_mount_label (ਅੱਖਰ **ret, ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *label, ਅਚਲ ਅੱਖਰ *context_type, libcrun_error_t *err);

ਪੂਰਨ ਸੈੱਟ_apparmor_profile (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

ਪੂਰਨ read_all_fd_with_ਮਾਪ_hint (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *description, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *len, size_t hint, libcrun_error_t *err);

ਪੂਰਨ read_all_fd_with_max_ਮਾਪ (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *description, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *len, size_t max_ਮਾਪ, libcrun_error_t *err);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
read_all_fd (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *description, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *len, libcrun_error_t *err)
{
  ਵਾਪਸ read_all_fd_with_ਮਾਪ_hint (fd, description, ਬਾਹਰਲਾ, len, 0, err);
}

ਪੂਰਨ get_realpath_ਮੰਜ਼ਿਲ_file (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ_name, ਅੱਖਰ **absolute_ਰਸਤਾ, libcrun_error_t *err);

ਪੂਰਨ read_all_file (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *len, libcrun_error_t *err);

ਪੂਰਨ read_all_file_at (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *len, libcrun_error_t *err);

ਪੂਰਨ open_unix_domain_client_socket (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ dgram, libcrun_error_t *err);

ਪੂਰਨ open_unix_domain_socket (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ dgram, libcrun_error_t *err);

ਪੂਰਨ send_fd_ਮੰਜ਼ਿਲ_socket (ਪੂਰਨ server, ਪੂਰਨ fd, libcrun_error_t *err);

ਪੂਰਨ send_fd_ਮੰਜ਼ਿਲ_socket_with_payload (ਪੂਰਨ server, ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *payload, size_t payload_len, libcrun_error_t *err);

ਪੂਰਨ create_socket_pair (ਪੂਰਨ *pair, libcrun_error_t *err);

ਪੂਰਨ receive_fd_from_socket (ਪੂਰਨ from, libcrun_error_t *err);

ਪੂਰਨ receive_fd_from_socket_with_payload (ਪੂਰਨ from, ਅੱਖਰ *payload, size_t payload_len, libcrun_error_t *err);

ਪੂਰਨ create_signalfd (sigset_t *ਨਕਾਬ, libcrun_error_t *err);

ਪੂਰਨ epoll_helper (ਪੂਰਨ *ਵਿੱਚ_fds, ਪੂਰਨ *ਵਿੱਚ_levelfds, ਪੂਰਨ *ਬਾਹਰਲਾ_fds, ਪੂਰਨ *ਬਾਹਰਲਾ_levelfds, libcrun_error_t *err);

ਪੂਰਨ copy_from_fd_ਮੰਜ਼ਿਲ_fd (ਪੂਰਨ src, ਪੂਰਨ dst, ਪੂਰਨ consume, libcrun_error_t *err);

ਪੂਰਨ ਲੜੀਵਾਰ_process (ਅੱਖਰ **ਦਲੀਲਾਂ, libcrun_error_t *err);

ਪੂਰਨ format_default_id_mapping (ਅੱਖਰ **ਬਾਹਰਲਾ, uid_t container_id, uid_t host_uid, uid_t host_id, ਪੂਰਨ is_uid, libcrun_error_t *err);

ਪੂਰਨ ਲੜੀਵਾਰ_process_with_stdin_timeout_envp (ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ **ਦਲੀਲਾਂ, ਅਚਲ ਅੱਖਰ *cwd, ਪੂਰਨ timeout, ਅੱਖਰ **envp,
                                         ਅੱਖਰ *stdin, size_t stdin_len, ਪੂਰਨ ਬਾਹਰਲਾ_fd, ਪੂਰਨ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

ਪੂਰਨ mark_or_close_fds_ge_than (libcrun_container_t *container, ਪੂਰਨ n, bool close_now, libcrun_error_t *err);

ਖਾਲੀ get_current_timestamp (ਅੱਖਰ *ਬਾਹਰਲਾ, size_t len);

ਪੂਰਨ ਸੈੱਟ_blocking_fd (ਪੂਰਨ fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_ਜਨਤਕ ਪੂਰਨ parse_json_file (json_object **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *jsondata, ਬਣਤਰ parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
has_ਅਗੇਤਰ (ਅਚਲ ਅੱਖਰ *str, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
  size_t ਅਗੇਤਰ_len;

  ਜੇ (! str || ! ਅਗੇਤਰ)
    ਵਾਪਸ 0;

  ਅਗੇਤਰ_len = ਲੜੀ_ਲੰਬਾਈ (ਅਗੇਤਰ);
  ਵਾਪਸ ਲੜੀ_ਲੰਬਾਈ (str) >= ਅਗੇਤਰ_len && memcmp (str, ਅਗੇਤਰ, ਅਗੇਤਰ_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
has_dir_ਅਗੇਤਰ (ਅਚਲ ਅੱਖਰ *str, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
  size_t ਅਗੇਤਰ_len;

  ਜੇ (! str || ! ਅਗੇਤਰ)
    ਵਾਪਸ 0;

  ਅਗੇਤਰ_len = ਲੜੀ_ਲੰਬਾਈ (ਅਗੇਤਰ);
  ਵਾਪਸ ਲੜੀ_ਲੰਬਾਈ (str) > ਅਗੇਤਰ_len && memcmp (str, ਅਗੇਤਰ, ਅਗੇਤਰ_len) == 0 && str[ਅਗੇਤਰ_len] == '/';
}

ਪੂਰਨ ਲੱਭੋ_executable (ਅੱਖਰ **exec_ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *executable_ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *cwd, libcrun_error_t *err);

ਪੂਰਨ copy_recursive_fd_ਮੰਜ਼ਿਲ_fd (ਪੂਰਨ srcfd, ਪੂਰਨ destfd, ਅਚਲ ਅੱਖਰ *srcname, ਅਚਲ ਅੱਖਰ *destname, libcrun_error_t *err);

ਪੂਰਨ ਸੈੱਟ_home_env (uid_t uid);

ਪੂਰਨ libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

ਪੂਰਨ libcrun_initialize_apparmor (libcrun_error_t *err);

ਅਚਲ ਅੱਖਰ *ਲੱਭੋ_annotation (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *name);

ਪੂਰਨ get_file_type_at (ਪੂਰਨ dirfd, mode_t *ਬਹੁਲਕ, bool nofollow, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ);

ਪੂਰਨ get_file_type (mode_t *ਬਹੁਲਕ, bool nofollow, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ);

ਪੂਰਨ get_file_type_fd (ਪੂਰਨ fd, mode_t *ਬਹੁਲਕ);

ਅੱਖਰ *get_user_name (uid_t uid);

ਪੂਰਨ ਸੁਰੱਖਿਅਤ_openat (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *rootfs, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ flags, ਪੂਰਨ ਬਹੁਲਕ,
                 libcrun_error_t *err);

ਪੂਰਨ ਸੁਰੱਖਿਅਤ_write (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *fname, ਅਚਲ ਖਾਲੀ *buf, size_t ਗਿਣਤੀ_2, libcrun_error_t *err);

ਪੂਰਨ append_paths (ਅੱਖਰ **ਬਾਹਰਲਾ, libcrun_error_t *err, ...) __attribute__ ((sentinel));

ਪੂਰਨ str2sig (ਅਚਲ ਅੱਖਰ *name);

ਪੂਰਨ base64_decode (ਅਚਲ ਅੱਖਰ *iptr, size_t isize, ਅੱਖਰ *optr, size_t osize, size_t *nbytes);
ਪੂਰਨ has_suffix (ਅਚਲ ਅੱਖਰ *ਸਰੋਤ, ਅਚਲ ਅੱਖਰ *suffix);
ਅੱਖਰ *libcrun_sysctl_ਕੁੰਜੀ_ਮੰਜ਼ਿਲ_proc_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ);
ਅੱਖਰ *str_join_array (ਪੂਰਨ ਵਿੱਥ, size_t ਮਾਪ, ਅੱਖਰ *ਅਚਲ array[], ਅਚਲ ਅੱਖਰ *joint);

ssize_t ਸੁਰੱਖਿਅਤ_readlinkat (ਪੂਰਨ dfd, ਅਚਲ ਅੱਖਰ *name, ਅੱਖਰ **ਬਫਰ, ssize_t hint, libcrun_error_t *err);

ਅੱਖਰ **read_dir_entries (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err);

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਅਚਲ ਅੱਖਰ *s)
{
  ਵਾਪਸ s == ਨਿਰਾ || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ size_t
ਸੁਰੱਖਿਅਤ_strlen (ਅਚਲ ਅੱਖਰ *s)
{
  ਵਾਪਸ s == ਨਿਰਾ ? 0 : ਲੜੀ_ਲੰਬਾਈ (s);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ਰਸਤਾ_has_dot_dot_component (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
  ਜੇ (ਰਸਤਾ == ਨਿਰਾ)
    ਵਾਪਸ false;
  cleanup_free ਅੱਖਰ *npath = xstrdup (ਰਸਤਾ);
  ਅੱਖਰ *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  ਜਦੋਂ (cur)
    {
      ਜੇ (it)
        *it = '\0';

      ਜੇ (strcmp (cur, "..") == 0)
        ਵਾਪਸ true;

      ਜੇ (it == ਨਿਰਾ)
        ਰੋਕੋ;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
waitpid_ignore_stopped (pid_t pid, ਪੂਰਨ *ਸਥਿਤੀ, ਪੂਰਨ options)
{
  ਪੂਰਨ ret, s;
  ਕਰੋ
    {
      s = 0;
      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (waitpid (pid, &s, options));
      ਜੇ (ret < 0)
        ਵਾਪਸ ret;
  } ਜਦੋਂ (WIFSTOPPED (s) || WIFCONTINUED (s));

  ਜੇ (ਸਥਿਤੀ)
    *ਸਥਿਤੀ = s;

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
get_process_exit_ਸਥਿਤੀ (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (WIFEXITED (ਸਥਿਤੀ))
    ਵਾਪਸ WEXITSTATUS (ਸਥਿਤੀ);
  ਜੇ (WIFSIGNALED (ਸਥਿਤੀ))
    ਵਾਪਸ 128 + WTERMSIG (ਸਥਿਤੀ);

  ਵਾਪਸ -1;
}

uid_t get_ਉਛਾਲ_uid (ਖਾਲੀ);
gid_t get_ਉਛਾਲ_gid (ਖਾਲੀ);

/* Adapted from systemd.  Include space for the NUL byte.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (ਆਕਾਰ (type) <= 1 ? 3U : ਆਕਾਰ (type) <= 2 ? 5U  \
                                        : ਆਕਾਰ (type) <= 4   ? 10U \
                                        : ਆਕਾਰ (type) <= 8   ? 20U \
                                                               : ਆਕਾਰ (ਪੂਰਨ[-2 * (ਆਕਾਰ (type) > 8)])))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _STRLEN(s) (ਆਕਾਰ (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (ਪੂਰਨ))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੱਖਰ proc_fd_ਰਸਤਾ_t[PROC_PID_FD_STRLEN];

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ _STRLEN

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
get_proc_fd_ਰਸਤਾ (proc_fd_ਰਸਤਾ_t ਰਸਤਾ, pid_t pid, ਪੂਰਨ fd)
{
  ਅਚਲ size_t max_len = ਆਕਾਰ (proc_fd_ਰਸਤਾ_t);
  size_t n;

  ਜੇ (pid)
    n = snprintf (ਰਸਤਾ, max_len, "/proc/%d/fd/%d", pid, fd);
  ਨਹੀਂਤਾਂ
    n = snprintf (ਰਸਤਾ, max_len, "/proc/self/fd/%d", fd);

  ਜੇ (UNLIKELY (n >= max_len))
    abort ();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
get_proc_self_fd_ਰਸਤਾ (proc_fd_ਰਸਤਾ_t ਰਸਤਾ, ਪੂਰਨ fd)
{
  get_proc_fd_ਰਸਤਾ (ਰਸਤਾ, 0, fd);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
get_self_fd_ਰਸਤਾ (proc_fd_ਰਸਤਾ_t ਰਸਤਾ, ਪੂਰਨ fd)
{
  snprintf (ਰਸਤਾ, ਆਕਾਰ (proc_fd_ਰਸਤਾ_t), "self/fd/%d", fd);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
validate_options (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ specified_options, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ supported_options, libcrun_error_t *err)
{
  ਜੇ (! ! (~supported_options & specified_options))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  ਵਾਪਸ 0;
}

ਬਾਹਰੀ ਪੂਰਨ cpuset_string_ਮੰਜ਼ਿਲ_bitmask (ਅਚਲ ਅੱਖਰ *str, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *ਬਾਹਰਲਾ_ਮਾਪ, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
ਬਣਤਰ channel_fd_pair;

ਬਣਤਰ channel_fd_pair *channel_fd_pair_new (ਪੂਰਨ ਵਿੱਚ_fd, ਪੂਰਨ ਬਾਹਰਲਾ_fd, size_t ਮਾਪ);

ਖਾਲੀ channel_fd_pair_free (ਬਣਤਰ channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
ਪੂਰਨ channel_fd_pair_process (ਬਣਤਰ channel_fd_pair *channel, ਪੂਰਨ epollfd, libcrun_error_t *err);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_channel_fd_pairp (ਖਾਲੀ *p)
{
  ਬਣਤਰ channel_fd_pair **pp = (ਬਣਤਰ channel_fd_pair **) p;
  ਜੇ (*pp == ਨਿਰਾ)
    ਵਾਪਸ;

  channel_fd_pair_free (*pp);
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

ਪੂਰਨ libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
ਪੂਰਨ libcrun_open_proc_file (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ flags, libcrun_error_t *err);
ਪੂਰਨ libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ flags, libcrun_error_t *err);

#ਜੇ_ਅੰਤ
