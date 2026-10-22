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

#define _GNU_SOURCE

#include <config.h>
#include "seccomp.h"
#include "linux.h"
#include "utils.h"
#include <string.h>
#include <sched.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <sys/mman.h>

#ifdef HAVE_BLAKE3
#  include <blake3.h>
#інакше
#  include "blake3/blake3.h"
#endif

#якщо HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int volatile ціле
#  endif
#endif

#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#include <linux/seccomp.h>
#include <linux/filter.h>
#include <sys/prctl.h>
#include <sys/syscall.h>

#ifndef __NR_seccomp
#  define __NR_seccomp 0xffff // seccomp syscall number unknown for this architecture
#endif

#ifndef SECCOMP_SET_MODE_STRICT
#  define SECCOMP_SET_MODE_STRICT 0
#endif

#ifndef SECCOMP_SET_MODE_FILTER
#  define SECCOMP_SET_MODE_FILTER 1
#endif

#ifndef SECCOMP_FILTER_FLAG_TSYNC
#  define SECCOMP_FILTER_FLAG_TSYNC (1UL << 0)
#endif

#ifndef SECCOMP_FILTER_FLAG_LOG
#  define SECCOMP_FILTER_FLAG_LOG (1UL << 1)
#endif

#ifndef SECCOMP_FILTER_FLAG_SPEC_ALLOW
#  define SECCOMP_FILTER_FLAG_SPEC_ALLOW (1UL << 2)
#endif

#ifndef SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV
#  define SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV (1UL << 5)
#endif

#define SECCOMP_CACHE_DIR ".cache/seccomp"

/* On Linux 5.19.15 x86_64, tmpfs reports 20 bytes + 20 bytes for each entry, so allow
   at least 32 entries.  */
#define MIN_CACHE_DIR_INODE_SIZE (20 + 32 * 20)

/* Heuristic to avoid the cache directory grows indefinitely.  The inode size for
   the directory is proportional to the number of entries it contains and it is
   cheaper to compute than iterating the cache directory.  */
static inline off_t
get_cache_dir_inode_max_size (off_t size_rundir)
{
  повернути size_rundir * 3 + MIN_CACHE_DIR_INODE_SIZE;
}

static ціле
syscall_seccomp (unsigned ціле operation, unsigned ціле flags, void *args)
{
  повернути (ціле) syscall (__NR_seccomp, operation, flags, args);
}

#ifdef HAVE_SECCOMP
static ціле
get_seccomp_operator (const char *name, enum scmp_compare *op, libcrun_error_t *err)
{
  static const struct
  {
    const char *name;
    enum scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  const char *p = name;
  size_t i;

  якщо (! has_prefix (p, "SCMP_CMP_"))
    goto fail;

  p += sizeof ("SCMP_CMP_") - 1;

  для (i = 0; i < sizeof (operators) / sizeof (operators[0]); i++)
    якщо (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        повернути 0;
      }

fail:
  повернути crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

static ціле
get_seccomp_action (const char *name, ціле errno_ret, uint32_t *action, libcrun_error_t *err)
{
  static const struct
  {
    const char *name;
    uint32_t action;
  } actions[] = {
    { "ALLOW", SCMP_ACT_ALLOW },
    { "KILL", SCMP_ACT_KILL },
    { "TRAP", SCMP_ACT_TRAP },
#  ifdef SCMP_ACT_LOG
    { "LOG", SCMP_ACT_LOG },
#  endif
#  ifdef SCMP_ACT_KILL_PROCESS
    { "KILL_PROCESS", SCMP_ACT_KILL_PROCESS },
#  endif
#  ifdef SCMP_ACT_KILL_THREAD
    { "KILL_THREAD", SCMP_ACT_KILL_THREAD },
#  endif
#  ifdef SCMP_ACT_NOTIFY
    { "NOTIFY", SCMP_ACT_NOTIFY },
#  endif
  };
  const char *p = name;
  size_t i;

  якщо (! has_prefix (p, "SCMP_ACT_"))
    goto fail;

  p += sizeof ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  якщо (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      повернути 0;
    }
  якщо (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      повернути 0;
    }

  для (i = 0; i < sizeof (actions) / sizeof (actions[0]); i++)
    якщо (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        повернути 0;
      }

fail:
  повернути crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
static ціле
export_bpf (scmp_filter_ctx ctx, ціле fd, libcrun_error_t *err)
{
  cleanup_free char *bpf = NULL;
  size_t len = 0;
  ціле ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, NULL, &len);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  інакше
  cleanup_close ціле pipe_r = -1;
  cleanup_close ціле pipe_w = -1;
  ціле p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  якщо (UNLIKELY (len >= (size_t) ret))
    повернути crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  повернути safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#endif /* HAVE_SECCOMP */

static void
make_lowercase (char *str)
{
  поки (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

static void
cleanup_seccompp (void *p)
{
#ifdef HAVE_SECCOMP
  scmp_filter_ctx *ctx = (void **) p;
  якщо (*ctx)
    seccomp_release (*ctx);
#endif
}
#define cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

ціле
libcrun_apply_seccomp (ціле infd, ціле listener_receiver_fd, const char *receiver_fd_payload,
                       size_t receiver_fd_payload_len, char **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  cleanup_mmap struct libcrun_mmap_s *mmap_region = NULL;
  cleanup_close ціле listener_fd = -1;
  cleanup_pid ціле helper_proc = -1;
  struct sock_fprog seccomp_filter;
  cleanup_free char *bpf = NULL;
  unsigned ціле flags = 0;
  size_t len;
  ціле ret;

  якщо (infd < 0)
    повернути 0;

  якщо (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    повернути crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  якщо (seccomp_flags == NULL)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  інакше
    {
      size_t i = 0;
      для (i = 0; i < seccomp_flags_len; i++)
        {
          якщо (seccomp_flags[i] == NULL)
            повернути crun_make_error (err, EINVAL, "seccomp flag is not specified");

          якщо (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          інакше якщо (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          інакше якщо (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          інакше якщо (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          інакше
            повернути crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (struct sock_filter *) bpf;

  якщо (listener_receiver_fd >= 0)
    {
      cleanup_close ціле memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  інакше
      повернути crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      якщо (UNLIKELY (memfd < 0))
        повернути crun_make_error (err, errno, "memfd_create");
#  інакше
      повернути crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, sizeof (atomic_int));
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, NULL, sizeof (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, NULL);
      якщо (UNLIKELY (helper_proc < 0))
        повернути crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      якщо (helper_proc == 0)
        {
          ціле fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          для (;;)
            {
              fd = *fd_received;
              якщо (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  якщо (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  continue;
                }
#  якщо ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              break;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          якщо (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  якщо (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      якщо (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  якщо (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      ціле status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      якщо (ret != 0)
        повернути crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  повернути 0;
#інакше
  повернути 0;
#endif
}

static bool
seccomp_action_supports_errno (const char *action)
{
  повернути strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
static ціле
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, unsigned ціле seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  unsigned char hash[32];
  struct utsname utsbuf;
  size_t i;
  ціле ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#define PROCESS_STRING(X)                                           \
  do                                                                \
    {                                                               \
      const char *str_ = (X);                                       \
      unsigned char present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? strlen (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, sizeof (present_)); \
      blake3_hasher_update (&hasher, &len_, sizeof (len_));         \
      якщо (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } поки (0)
#define PROCESS_DATA(X)                                   \
  do                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), sizeof ((X))); \
  } поки (0)
#define PROCESS_OPTIONAL_DATA(PRESENT, X)                           \
  do                                                                \
    {                                                               \
      unsigned char present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, sizeof (present_)); \
      якщо (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), sizeof ((X)));         \
  } поки (0)
#define PROCESS_COUNT(X)                                        \
  do                                                            \
    {                                                           \
      size_t count_ = (X);                                      \
      blake3_hasher_update (&hasher, &count_, sizeof (count_)); \
  } поки (0)

  PROCESS_STRING (PACKAGE_VERSION);

#ifdef HAVE_SECCOMP
  {
    const struct scmp_version *version = seccomp_version ();

    PROCESS_DATA (version->major);
    PROCESS_DATA (version->minor);
    PROCESS_DATA (version->micro);
  }
#endif

  memset (&utsbuf, 0, sizeof (utsbuf));
  ret = uname (&utsbuf);
  якщо (UNLIKELY (ret != 0))
    повернути crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_DATA (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_DATA (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_COUNT (seccomp->flags_len);
  для (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_COUNT (seccomp->architectures_len);
  для (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_COUNT (seccomp->syscalls_len);
  для (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_DATA (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_COUNT (seccomp->syscalls[i]->names_len);
      для (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_COUNT (seccomp->syscalls[i]->args_len);
      для (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->index);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->value);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->value_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, sizeof (hash));

  для (i = 0; i < 32; i++)
    {
      size_t remaining = sizeof (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      якщо (UNLIKELY (ret != 2))
        повернути crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#undef PROCESS_STRING
#undef PROCESS_DATA
#undef PROCESS_OPTIONAL_DATA
#undef PROCESS_COUNT
  повернути 1;
}

static ціле
open_rundir_dirfd (const char *state_root, libcrun_error_t *err)
{
  cleanup_free char *dir = NULL;
  ціле dirfd;
  ціле ret;

  ret = libcrun_get_state_directory (&dir, state_root, NULL, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  dirfd = TEMP_FAILURE_RETRY (open (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  якщо (UNLIKELY (dirfd < 0))
    повернути crun_make_error (err, errno, "open `%s`", dir);

  повернути dirfd;
}

struct cache_entry
{
  seccomp_checksum_t checksum;
  struct timespec atime;
};

static ціле
compare_entries_by_atime (const void *a, const void *b)
{
  const struct cache_entry *entry_a = a;
  const struct cache_entry *entry_b = b;
  ціле ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  якщо (ret)
    повернути ret;

  повернути entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

static ціле
evict_cache (ціле root_dfd, libcrun_error_t *err)
{
  cleanup_close ціле cache_dir_fd = -1;
  off_t cache_dir_inode_max_size;
  struct stat st;
  ціле ret;

  ret = TEMP_FAILURE_RETRY (fstat (root_dfd, &st));
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_size = get_cache_dir_inode_max_size (st.st_size);

  cache_dir_fd = TEMP_FAILURE_RETRY (openat (root_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  якщо (UNLIKELY (cache_dir_fd < 0))
    {
      якщо (errno == ENOENT)
        повернути 0;
      повернути crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = TEMP_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  якщо (ret == 0 && st.st_size > cache_dir_inode_max_size)
    {
      cleanup_free struct cache_entry *entries = NULL;
      cleanup_dir DIR *d = NULL;
      size_t i, n_entries = 0;
      struct dirent *de;
      ціле dfd;

      d = fdopendir (cache_dir_fd);
      якщо (d == NULL)
        повернути crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      поки ((de = readdir (d)))
        {
          якщо (de->d_name[0] == '.')
            continue;

          ret = TEMP_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          якщо (UNLIKELY (ret < 0))
            continue;

          /* The cache file is already used, it is pointless to free it.  */
          якщо (st.st_nlink > 1)
            continue;

          /* If the sticky bit is set, then ignore it.  */
          якщо (st.st_mode & S_ISVTX)
            continue;

          якщо (strlen (de->d_name) != sizeof (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              continue;
            }
          інакше
            {
              entries = xrealloc (entries, sizeof (struct cache_entry) * (n_entries + 1));

              memcpy (entries[n_entries].checksum, de->d_name, sizeof (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, sizeof (struct cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      для (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  повернути 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
static ціле
open_seccomp_bpf_dirfd (libcrun_container_t *container, char **rel_path, libcrun_error_t *err)
{
  ціле ret;
  ціле dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_root : NULL, err);
  якщо (UNLIKELY (dirfd < 0))
    повернути dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", NULL);
  якщо (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (close (dirfd));
      повернути ret;
    }

  повернути dirfd;
}

static ціле
store_seccomp_cache (struct libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free char *src_path = NULL;
  cleanup_free char *dest_path = NULL;
  cleanup_close ціле dirfd = -1;
  ціле ret;

  якщо (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    повернути 0;

  якщо (is_empty_string (ctx->checksum))
    повернути 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  якщо (UNLIKELY (dirfd < 0))
    повернути dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = evict_cache (dirfd, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  якщо (UNLIKELY (ret < 0 && errno != EEXIST))
    повернути crun_make_error (err, errno, "link `%s` to `%s`", src_path, dest_path);
  повернути 0;
}

static inline runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (struct libcrun_seccomp_gen_ctx_s *ctx)
{
  якщо (ctx->container == NULL || ctx->container->container_def == NULL
      || ctx->container->container_def->linux == NULL)
    повернути 0;

  повернути ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
static inline bool
has_seccomp_configuration (struct libcrun_seccomp_gen_ctx_s *ctx)
{
  якщо (ctx->container == NULL || ctx->container->container_def == NULL
      || ctx->container->container_def->linux == NULL)
    повернути false;

  повернути ctx->container->container_def->linux->seccomp != NULL
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != NULL;
}

static ціле
find_in_cache (struct libcrun_seccomp_gen_ctx_s *ctx, ціле dirfd, const char *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free char *cache_file_path = NULL;
  ціле ret;

  якщо (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    повернути 0;

  seccomp = get_seccomp_configuration (ctx);
  якщо (seccomp == NULL)
    повернути 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = TEMP_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  якщо (UNLIKELY (ret < 0 && errno != ENOENT))
    повернути crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  повернути 0;
}

ціле
libcrun_generate_seccomp (struct libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  ціле ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = NULL;
  ціле default_errno_value = EPERM;
  uint32_t action, default_action;
  const char *def_action = NULL;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  якщо (gen_ctx->from_cache)
    повернути 0;

  якщо (gen_ctx->container == NULL || gen_ctx->container->container_def == NULL || gen_ctx->container->container_def->linux == NULL)
    повернути 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  якщо (seccomp == NULL)
    повернути 0;

  /* seccomp not available.  */
  якщо (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    повернути crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  якщо (def_action == NULL)
    повернути crun_make_error (err, 0, "seccomp misses the default action");

  якщо (seccomp->default_errno_ret_present)
    {
      якщо (! seccomp_action_supports_errno (def_action))
        повернути crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_value = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_value, &default_action, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ctx = seccomp_init (default_action);
  якщо (ctx == NULL)
    повернути crun_make_error (err, 0, "error seccomp_init");

  для (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      const char *arch = seccomp->architectures[i];
      char *end, lowercase_arch[32] = {
        0,
      };

      якщо (arch == NULL)
        повернути crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      якщо (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, sizeof (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      якщо (arch_token == 0)
        повернути crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  інакше
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      якщо (ret < 0 && ret != -EEXIST)
        повернути crun_make_error (err, -ret, "seccomp adding architecture");
    }

  для (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool multiple_args = false;
      ціле errno_ret = EPERM;

      якщо (seccomp->syscalls[i]->errno_ret_present)
        {
          якщо (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            повернути crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      якщо (action == default_action)
        continue;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      якщо (seccomp->syscalls[i]->args != NULL)
        {
          uint32_t count[6] = {};
          size_t k;

          для (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t index = seccomp->syscalls[i]->args[k]->index;

              якщо (index >= 6)
                повернути crun_make_error (err, 0, "invalid seccomp index `%u`", index);

              count[index]++;
              якщо (count[index] > 1)
                multiple_args = true;
            }
        }

      для (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          ціле syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          якщо (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              якщо (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                повернути crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              continue;
            }

          якщо (seccomp->syscalls[i]->args == NULL)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              якщо (UNLIKELY (ret < 0))
                повернути crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
            }
          інакше
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              якщо (multiple_args)
                {
                  size_t r;

                  для (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      struct scmp_arg_cmp arg_cmp;
                      char *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->index;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      якщо (UNLIKELY (ret < 0))
                        повернути ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->value;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->value_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      якщо (UNLIKELY (ret < 0))
                        повернути crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              інакше
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  const size_t args_len = seccomp->syscalls[i]->args_len;
                  struct scmp_arg_cmp arg_cmp[6];

                  для (k = 0; k < args_len; k++)
                    {
                      char *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->index;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      якщо (UNLIKELY (ret < 0))
                        повернути ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->value;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->value_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : NULL);
                  якщо (UNLIKELY (ret < 0))
                    повернути crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  якщо (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      повернути store_seccomp_cache (gen_ctx, err);
    }

  повернути 0;
#інакше
  повернути 0;
#endif
}

ціле
libcrun_copy_seccomp (struct libcrun_seccomp_gen_ctx_s *gen_ctx, const char *b64_bpf, libcrun_error_t *err)
{
  cleanup_free char *bpf_data = NULL;
  size_t size = 0;
  size_t in_size;
  ціле consumed;
  ціле ret;

  in_size = strlen (b64_bpf);
  bpf_data = xmalloc (in_size + 1);

  consumed = base64_decode (b64_bpf, in_size, bpf_data, in_size, &size);
  якщо (UNLIKELY (consumed != (ціле) in_size))
    повернути crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_data, size, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути 0;
}

ціле
libcrun_open_seccomp_bpf (struct libcrun_seccomp_gen_ctx_s *ctx, ціле *fd, libcrun_error_t *err)
{
  cleanup_close ціле dirfd = -1;
  cleanup_free char *dest_path = NULL;
  libcrun_container_t *container = ctx->container;
  ціле ret;

  ctx->fd = *fd = -1;

  якщо (container == NULL || container->context == NULL)
    повернути crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  якщо (UNLIKELY (dirfd < 0))
    повернути dirfd;

  якщо (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      якщо (created)
        {
          ctx->from_cache = true;
          goto open_existing_file;
        }

      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  інакше
    {
    open_existing_file:
      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      якщо (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          якщо (errno == ENOENT && ! has_seccomp_configuration (ctx))
            повернути 0;
          повернути crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  повернути 0;
}
