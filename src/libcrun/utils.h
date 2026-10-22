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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ "error.h"
#ଅନ୍ତର୍ଭାଵ <dirent.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/runtime_spec_schema_config_schema.h>
#ଅନ୍ତର୍ଭାଵ <sys/wait.h>
#ଅନ୍ତର୍ଭାଵ "container.h"

#ifndef TEMP_FAILURE_RETRY
#  define TEMP_FAILURE_RETRY(expression)      \
    (__extension__ ({                         \
      long ପୂର୍ଣ୍ଣସଂଖ୍ୟା __result;                      \
      କର                                      \
        __result = (long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (expression);   \
      ଯେତେବେଳେ (__result < 0 && errno == EINTR); \
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

__attribute__ ((malloc)) ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ *
xmalloc (size_t size)
{
  ଶୂନ୍ୟ *res = malloc (size);
  ଯଦି (UNLIKELY (res == ଖାଲି))
    OOM ();
  ଫେରନ୍ତୁ res;
}

__attribute__ ((malloc)) ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ *
xmalloc0 (size_t size)
{
  ଶୂନ୍ୟ *res = calloc (1, size);
  ଯଦି (UNLIKELY (res == ଖାଲି))
    OOM ();
  ଫେରନ୍ତୁ res;
}

__attribute__ ((malloc)) ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ *
xrealloc (ଶୂନ୍ୟ *ptr, size_t size)
{
  ଶୂନ୍ୟ *res = realloc (ptr, size);
  ଯଦି (UNLIKELY (res == ଖାଲି))
    OOM ();
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_freep (ଶୂନ୍ୟ *p)
{
  ଶୂନ୍ୟ **pp = (ଶୂନ୍ୟ **) p;
  ମୁକ୍ତ (*pp);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  ଯଦି (file)
    (ଶୂନ୍ୟ) fclose (file);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_closep (ଶୂନ୍ୟ *p)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pp = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *) p;
  ଯଦି (*pp >= 0)
    TEMP_FAILURE_RETRY (close (*pp));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_pidp (ଶୂନ୍ୟ *p)
{
  pid_t *pp = (pid_t *) p;
  ଯଦି (*pp > 0)
    {
      TEMP_FAILURE_RETRY (kill (*pp, SIGKILL));
      TEMP_FAILURE_RETRY (waitpid (*pp, ଖାଲି, 0));
    }
}

ସଂରଚନା libcrun_mmap_s
{
  ଶୂନ୍ୟ *addr;
  size_t length;
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_mmap (ସଂରଚନା libcrun_mmap_s **ret, ଶୂନ୍ୟ *addr, size_t length,
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା prot, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, off_t offset,
                  libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_munmap (ସଂରଚନା libcrun_mmap_s *mmap, libcrun_error_t *err);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_mmapp (ଶୂନ୍ୟ *p)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  libcrun_error_t tmp_err = ଖାଲି;
  ସଂରଚନା libcrun_mmap_s **mm;

  mm = (ସଂରଚନା libcrun_mmap_s **) p;
  ଯଦି (*mm == ଖାଲି)
    ଫେରନ୍ତୁ;

  ret = libcrun_munmap (*mm, &tmp_err);
  ଯଦି (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

ସଂରଚନା libcrun_fd_map
{
  size_t nfds;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fds[];
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  ସଂରଚନା libcrun_fd_map *ret;
  size_t i;

  ret = (ସଂରଚନା libcrun_fd_map *) xmalloc (ଆକାର (*ret) + ଆକାର (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) * len);
  ret->nfds = len;
  ପାଇଁ (i = 0; i < len; i++)
    ret->fds[i] = -1;

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_close_mapp (ସଂରଚନା libcrun_fd_map **p)
{
  ସଂରଚନା libcrun_fd_map *m = *p;
  size_t i;

  ଯଦି (m == ଖାଲି)
    ଫେରନ୍ତୁ;

  ପାଇଁ (i = 0; i < m->nfds; i++)
    ଯଦି (m->fds[i] >= 0)
      TEMP_FAILURE_RETRY (close (m->fds[i]));

  ମୁକ୍ତ (m);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_close_vecp (ପୂର୍ଣ୍ଣସଂଖ୍ୟା **p)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pp = *p;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ଯଦି (pp == ଖାଲି)
    ଫେରନ୍ତୁ;

  ପାଇଁ (i = 0; pp[i] >= 0; i++)
    TEMP_FAILURE_RETRY (close (pp[i]));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  ଯଦି (dir)
    closedir (dir);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
close_and_reset (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *fd)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
  ଯଦି (*fd >= 0)
    {
      ret = TEMP_FAILURE_RETRY (close (*fd));
      ଯଦି (LIKELY (ret == 0))
        *fd = -1;
    }
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅକ୍ଷର *
xstrdup (ଅଚଳ ଅକ୍ଷର *str)
{
  ଅକ୍ଷର *ret;
  ଯଦି (str == ଖାଲି)
    ଫେରନ୍ତୁ ଖାଲି;

  ret = strdup (str);
  ଯଦି (ret == ଖାଲି)
    OOM ();

  ଫେରନ୍ତୁ ret;
}

ଶୂନ୍ୟ consume_trailing_slashes (ଅକ୍ଷର *path);

#define consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    ଯେତେବେଳେ (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
path_is_slash_dev (ଅଚଳ ଅକ୍ଷର *path)
{
  path = consume_slashes (path);

  ଯଦି (strncmp (path, "dev", 3))
    ଫେରନ୍ତୁ false;

  path += 3;

  /* Check there are only '/' left.  */
  ପାଇଁ (; *path; path++)
    ଯଦି (*path != '/')
      ଫେରନ୍ତୁ false;

  ଫେରନ୍ତୁ true;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା xasprintf (ଅକ୍ଷର **str, ଅଚଳ ଅକ୍ଷର *fmt, ...) __attribute__ ((format (printf, 2, 3)));

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_path_exists (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_file_at_with_flags (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, mode_t mode, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଶୂନ୍ୟ *data, size_t len, libcrun_error_t *err);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_file (ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଶୂନ୍ୟ *data, size_t len, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_file_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଶୂନ୍ୟ *data, size_t len, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, data, len, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_ensure_directory (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, bool nofollow, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_ensure_directory_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, bool nofollow, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_safe_create_and_open_ref_at (bool dir, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *dirpath, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_safe_ensure_directory_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *dirpath, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode,
                                   libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_safe_ensure_file_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *dirpath, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode,
                              libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_dir_p (ଅଚଳ ଅକ୍ଷର *path, bool nofollow, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_dir_p_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *path, bool nofollow, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach_process ();

ପୂର୍ଣ୍ଣସଂଖ୍ୟା create_file_if_missing_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *file, mode_t mode, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_running_in_user_namespace (libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_selinux_label (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *label, bool now, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_selinux_mount_label (ଅକ୍ଷର **ret, ଅଚଳ ଅକ୍ଷର *data, ଅଚଳ ଅକ୍ଷର *label, ଅଚଳ ଅକ୍ଷର *context_type, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_apparmor_profile (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_all_fd_with_size_hint (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *description, ଅକ୍ଷର **out, size_t *len, size_t hint, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_all_fd_with_max_size (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *description, ଅକ୍ଷର **out, size_t *len, size_t max_size, libcrun_error_t *err);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_all_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *description, ଅକ୍ଷର **out, size_t *len, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ read_all_fd_with_size_hint (fd, description, out, len, 0, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_realpath_to_file (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *path_name, ଅକ୍ଷର **absolute_path, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_all_file (ଅଚଳ ଅକ୍ଷର *path, ଅକ୍ଷର **out, size_t *len, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_all_file_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *path, ଅକ୍ଷର **out, size_t *len, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_unix_domain_client_socket (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dgram, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_unix_domain_socket (ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dgram, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା send_fd_to_socket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା server, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା send_fd_to_socket_with_payload (ପୂର୍ଣ୍ଣସଂଖ୍ୟା server, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *payload, size_t payload_len, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା create_socket_pair (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pair, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା receive_fd_from_socket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା from, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା receive_fd_from_socket_with_payload (ପୂର୍ଣ୍ଣସଂଖ୍ୟା from, ଅକ୍ଷର *payload, size_t payload_len, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା create_signalfd (sigset_t *mask, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା epoll_helper (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *in_fds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *in_levelfds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out_fds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out_levelfds, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy_from_fd_to_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା consume, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_process (ଅକ୍ଷର **args, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_default_id_mapping (ଅକ୍ଷର **out, uid_t container_id, uid_t host_uid, uid_t host_id, ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_uid, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_process_with_stdin_timeout_envp (ଅକ୍ଷର *path, ଅକ୍ଷର **args, ଅଚଳ ଅକ୍ଷର *cwd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା timeout, ଅକ୍ଷର **envp,
                                         ଅକ୍ଷର *stdin, size_t stdin_len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_or_close_fds_ge_than (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, bool close_now, libcrun_error_t *err);

ଶୂନ୍ୟ get_current_timestamp (ଅକ୍ଷର *out, size_t len);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_blocking_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_json_file (json_object **out, ଅଚଳ ଅକ୍ଷର *jsondata, ସଂରଚନା parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
has_prefix (ଅଚଳ ଅକ୍ଷର *str, ଅଚଳ ଅକ୍ଷର *prefix)
{
  size_t prefix_len;

  ଯଦି (! str || ! prefix)
    ଫେରନ୍ତୁ 0;

  prefix_len = strlen (prefix);
  ଫେରନ୍ତୁ strlen (str) >= prefix_len && memcmp (str, prefix, prefix_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
has_dir_prefix (ଅଚଳ ଅକ୍ଷର *str, ଅଚଳ ଅକ୍ଷର *prefix)
{
  size_t prefix_len;

  ଯଦି (! str || ! prefix)
    ଫେରନ୍ତୁ 0;

  prefix_len = strlen (prefix);
  ଫେରନ୍ତୁ strlen (str) > prefix_len && memcmp (str, prefix, prefix_len) == 0 && str[prefix_len] == '/';
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_executable (ଅକ୍ଷର **exec_path, ଅଚଳ ଅକ୍ଷର *executable_path, ଅଚଳ ଅକ୍ଷର *cwd, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy_recursive_fd_to_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା destfd, ଅଚଳ ଅକ୍ଷର *srcname, ଅଚଳ ଅକ୍ଷର *destname, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_home_env (uid_t uid);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_initialize_apparmor (libcrun_error_t *err);

ଅଚଳ ଅକ୍ଷର *find_annotation (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *name);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_file_type_at (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, mode_t *mode, bool nofollow, ଅଚଳ ଅକ୍ଷର *path);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_file_type (mode_t *mode, bool nofollow, ଅଚଳ ଅକ୍ଷର *path);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_file_type_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, mode_t *mode);

ଅକ୍ଷର *get_user_name (uid_t uid);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା safe_openat (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *rootfs, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode,
                 libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା safe_write (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *fname, ଅଚଳ ଶୂନ୍ୟ *buf, size_t ଗଣନା_2, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା append_paths (ଅକ୍ଷର **out, libcrun_error_t *err, ...) __attribute__ ((sentinel));

ପୂର୍ଣ୍ଣସଂଖ୍ୟା str2sig (ଅଚଳ ଅକ୍ଷର *name);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା base64_decode (ଅଚଳ ଅକ୍ଷର *iptr, size_t isize, ଅକ୍ଷର *optr, size_t osize, size_t *nbytes);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_suffix (ଅଚଳ ଅକ୍ଷର *source, ଅଚଳ ଅକ୍ଷର *suffix);
ଅକ୍ଷର *libcrun_sysctl_ଚାବି_to_proc_path (ଅଚଳ ଅକ୍ଷର *ଚାବି);
ଅକ୍ଷର *str_join_array (ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset, size_t size, ଅକ୍ଷର *ଅଚଳ array[], ଅଚଳ ଅକ୍ଷର *joint);

ssize_t safe_readlinkat (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd, ଅଚଳ ଅକ୍ଷର *name, ଅକ୍ଷର **buffer, ssize_t hint, libcrun_error_t *err);

ଅକ୍ଷର **read_dir_entries (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_empty_string (ଅଚଳ ଅକ୍ଷର *s)
{
  ଫେରନ୍ତୁ s == ଖାଲି || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t
safe_strlen (ଅଚଳ ଅକ୍ଷର *s)
{
  ଫେରନ୍ତୁ s == ଖାଲି ? 0 : strlen (s);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
path_has_dot_dot_component (ଅଚଳ ଅକ୍ଷର *path)
{
  ଯଦି (path == ଖାଲି)
    ଫେରନ୍ତୁ false;
  cleanup_free ଅକ୍ଷର *npath = xstrdup (path);
  ଅକ୍ଷର *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  ଯେତେବେଳେ (cur)
    {
      ଯଦି (it)
        *it = '\0';

      ଯଦି (strcmp (cur, "..") == 0)
        ଫେରନ୍ତୁ true;

      ଯଦି (it == ଖାଲି)
        ଭାଙ୍ଗ;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
waitpid_ignore_stopped (pid_t pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *status, ପୂର୍ଣ୍ଣସଂଖ୍ୟା options)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, s;
  କର
    {
      s = 0;
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &s, options));
      ଯଦି (ret < 0)
        ଫେରନ୍ତୁ ret;
  } ଯେତେବେଳେ (WIFSTOPPED (s) || WIFCONTINUED (s));

  ଯଦି (status)
    *status = s;

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_process_exit_status (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (WIFEXITED (status))
    ଫେରନ୍ତୁ WEXITSTATUS (status);
  ଯଦି (WIFSIGNALED (status))
    ଫେରନ୍ତୁ 128 + WTERMSIG (status);

  ଫେରନ୍ତୁ -1;
}

uid_t get_overflow_uid (ଶୂନ୍ୟ);
gid_t get_overflow_gid (ଶୂନ୍ୟ);

/* Adapted from systemd.  Include space for the NUL byte.  */
#define DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (ଆକାର (type) <= 1 ? 3U : ଆକାର (type) <= 2 ? 5U  \
                                        : ଆକାର (type) <= 4   ? 10U \
                                        : ଆକାର (type) <= 8   ? 20U \
                                                               : ଆକାର (ପୂର୍ଣ୍ଣସଂଖ୍ୟା[-2 * (ଆକାର (type) > 8)])))

#define _STRLEN(s) (ଆକାର (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#define PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (ପୂର୍ଣ୍ଣସଂଖ୍ୟା))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅକ୍ଷର proc_fd_path_t[PROC_PID_FD_STRLEN];

#undef _STRLEN

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
get_proc_fd_path (proc_fd_path_t path, pid_t pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
  ଅଚଳ size_t max_len = ଆକାର (proc_fd_path_t);
  size_t n;

  ଯଦି (pid)
    n = snprintf (path, max_len, "/proc/%d/fd/%d", pid, fd);
  ଅନ୍ୟଥା
    n = snprintf (path, max_len, "/proc/self/fd/%d", fd);

  ଯଦି (UNLIKELY (n >= max_len))
    abort ();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
get_proc_self_fd_path (proc_fd_path_t path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
  get_proc_fd_path (path, 0, fd);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
get_self_fd_path (proc_fd_path_t path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
  snprintf (path, ଆକାର (proc_fd_path_t), "self/fd/%d", fd);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
validate_options (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା specified_options, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା supported_options, libcrun_error_t *err)
{
  ଯଦି (! ! (~supported_options & specified_options))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  ଫେରନ୍ତୁ 0;
}

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpuset_string_to_bitmask (ଅଚଳ ଅକ୍ଷର *str, ଅକ୍ଷର **out, size_t *out_size, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
ସଂରଚନା channel_fd_pair;

ସଂରଚନା channel_fd_pair *channel_fd_pair_new (ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_fd, size_t size);

ଶୂନ୍ୟ channel_fd_pair_free (ସଂରଚନା channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା channel_fd_pair_process (ସଂରଚନା channel_fd_pair *channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା epollfd, libcrun_error_t *err);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_channel_fd_pairp (ଶୂନ୍ୟ *p)
{
  ସଂରଚନା channel_fd_pair **pp = (ସଂରଚନା channel_fd_pair **) p;
  ଯଦି (*pp == ଖାଲି)
    ଫେରନ୍ତୁ;

  channel_fd_pair_free (*pp);
}
#define cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_open_proc_file (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, libcrun_error_t *err);

#endif
