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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "seccomp.h"
#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/mount.h>
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#ಅನ್ತರ್ಭಾವ <sys/prctl.h>
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <sys/resource.h>
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/utsname.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/mman.h>

#ifdef HAVE_BLAKE3
#  include <blake3.h>
#ಅನ್ಯಥಾ
#  include "blake3/blake3.h"
#endif

#ಯದಿ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int ಚಞ್ಚಲ ಪೂರ್ಣಾಙ್ಕ
#  endif
#endif

#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#ಅನ್ತರ್ಭಾವ <linux/seccomp.h>
#ಅನ್ತರ್ಭಾವ <linux/filter.h>
#ಅನ್ತರ್ಭಾವ <sys/prctl.h>
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>

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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ off_t
get_cache_dir_inode_max_size (off_t size_rundir)
{
  ಪ್ರತ್ಯಾಗಮ size_rundir * 3 + MIN_CACHE_DIR_INODE_SIZE;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
syscall_seccomp (unsigned ಪೂರ್ಣಾಙ್ಕ operation, unsigned ಪೂರ್ಣಾಙ್ಕ flags, ಶೂನ್ಯ *args)
{
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_seccomp, operation, flags, args);
}

#ifdef HAVE_SECCOMP
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_seccomp_operator (ಧ್ರುವ ವರ್ಣ *name, ಗಣನಾ scmp_compare *op, libcrun_error_t *err)
{
  ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ
  {
    ಧ್ರುವ ವರ್ಣ *name;
    ಗಣನಾ scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  ಧ್ರುವ ವರ್ಣ *p = name;
  size_t i;

  ಯದಿ (! has_prefix (p, "SCMP_CMP_"))
    ಗಚ್ಛ fail;

  p += ಪರಿಮಾಣ ("SCMP_CMP_") - 1;

  ಪ್ರತಿ (i = 0; i < ಪರಿಮಾಣ (operators) / ಪರಿಮಾಣ (operators[0]); i++)
    ಯದಿ (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        ಪ್ರತ್ಯಾಗಮ 0;
      }

fail:
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_seccomp_action (ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ errno_ret, uint32_t *action, libcrun_error_t *err)
{
  ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ
  {
    ಧ್ರುವ ವರ್ಣ *name;
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
  ಧ್ರುವ ವರ್ಣ *p = name;
  size_t i;

  ಯದಿ (! has_prefix (p, "SCMP_ACT_"))
    ಗಚ್ಛ fail;

  p += ಪರಿಮಾಣ ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  ಯದಿ (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ಯದಿ (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಪ್ರತಿ (i = 0; i < ಪರಿಮಾಣ (actions) / ಪರಿಮಾಣ (actions[0]); i++)
    ಯದಿ (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        ಪ್ರತ್ಯಾಗಮ 0;
      }

fail:
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
export_bpf (scmp_filter_ctx ctx, ಪೂರ್ಣಾಙ್ಕ fd, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *bpf = ರಿಕ್ತ;
  size_t len = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, ರಿಕ್ತ, &len);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  ಅನ್ಯಥಾ
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pipe_r = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pipe_w = -1;
  ಪೂರ್ಣಾಙ್ಕ p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  ಯದಿ (UNLIKELY (len >= (size_t) ret))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  ಪ್ರತ್ಯಾಗಮ safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#endif /* HAVE_SECCOMP */

ಸ್ಥಿರ ಶೂನ್ಯ
make_lowercase (ವರ್ಣ *str)
{
  ಯಾವತ್ (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup_seccompp (ಶೂನ್ಯ *p)
{
#ifdef HAVE_SECCOMP
  scmp_filter_ctx *ctx = (ಶೂನ್ಯ **) p;
  ಯದಿ (*ctx)
    seccomp_release (*ctx);
#endif
}
#define cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

ಪೂರ್ಣಾಙ್ಕ
libcrun_apply_seccomp (ಪೂರ್ಣಾಙ್ಕ infd, ಪೂರ್ಣಾಙ್ಕ listener_receiver_fd, ಧ್ರುವ ವರ್ಣ *receiver_fd_payload,
                       size_t receiver_fd_payload_len, ವರ್ಣ **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  cleanup_mmap ಸಂರಚನಾ libcrun_mmap_s *mmap_region = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ listener_fd = -1;
  cleanup_pid ಪೂರ್ಣಾಙ್ಕ helper_proc = -1;
  ಸಂರಚನಾ sock_fprog seccomp_filter;
  cleanup_free ವರ್ಣ *bpf = ರಿಕ್ತ;
  unsigned ಪೂರ್ಣಾಙ್ಕ flags = 0;
  size_t len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (infd < 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  ಯದಿ (seccomp_flags == ರಿಕ್ತ)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  ಅನ್ಯಥಾ
    {
      size_t i = 0;
      ಪ್ರತಿ (i = 0; i < seccomp_flags_len; i++)
        {
          ಯದಿ (seccomp_flags[i] == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "seccomp flag is not specified");

          ಯದಿ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          ಅನ್ಯಥಾ ಯದಿ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          ಅನ್ಯಥಾ ಯದಿ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          ಅನ್ಯಥಾ ಯದಿ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (ಸಂರಚನಾ sock_filter *) bpf;

  ಯದಿ (listener_receiver_fd >= 0)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      ಯದಿ (UNLIKELY (memfd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "memfd_create");
#  ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, ಪರಿಮಾಣ (atomic_int));
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, ರಿಕ್ತ, ಪರಿಮಾಣ (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (helper_proc < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      ಯದಿ (helper_proc == 0)
        {
          ಪೂರ್ಣಾಙ್ಕ fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          ಪ್ರತಿ (;;)
            {
              fd = *fd_received;
              ಯದಿ (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  ಯದಿ (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  ಅನುವರ್ತ;
                }
#  ಯದಿ ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              ಭಙ್ಗ;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          ಯದಿ (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      ಯದಿ (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  ಯದಿ (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      ಪೂರ್ಣಾಙ್ಕ status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      ಯದಿ (ret != 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

ಸ್ಥಿರ bool
seccomp_action_supports_errno (ಧ್ರುವ ವರ್ಣ *action)
{
  ಪ್ರತ್ಯಾಗಮ strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, unsigned ಪೂರ್ಣಾಙ್ಕ seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  unsigned ವರ್ಣ hash[32];
  ಸಂರಚನಾ utsname utsbuf;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#define PROCESS_STRING(X)                                           \
  ಕುರು                                                                \
    {                                                               \
      ಧ್ರುವ ವರ್ಣ *str_ = (X);                                       \
      unsigned ವರ್ಣ present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? strlen (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, ಪರಿಮಾಣ (present_)); \
      blake3_hasher_update (&hasher, &len_, ಪರಿಮಾಣ (len_));         \
      ಯದಿ (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } ಯಾವತ್ (0)
#define PROCESS_DATA(X)                                   \
  ಕುರು                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), ಪರಿಮಾಣ ((X))); \
  } ಯಾವತ್ (0)
#define PROCESS_OPTIONAL_DATA(PRESENT, X)                           \
  ಕುರು                                                                \
    {                                                               \
      unsigned ವರ್ಣ present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, ಪರಿಮಾಣ (present_)); \
      ಯದಿ (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), ಪರಿಮಾಣ ((X)));         \
  } ಯಾವತ್ (0)
#define PROCESS_ಎಣಿಕೆ(X)                                        \
  ಕುರು                                                            \
    {                                                           \
      size_t ಎಣಿಕೆ_2 = (X);                                      \
      blake3_hasher_update (&hasher, &ಎಣಿಕೆ_2, ಪರಿಮಾಣ (ಎಣಿಕೆ_2)); \
  } ಯಾವತ್ (0)

  PROCESS_STRING (PACKAGE_VERSION);

#ifdef HAVE_SECCOMP
  {
    ಧ್ರುವ ಸಂರಚನಾ scmp_version *version = seccomp_version ();

    PROCESS_DATA (version->major);
    PROCESS_DATA (version->minor);
    PROCESS_DATA (version->micro);
  }
#endif

  memset (&utsbuf, 0, ಪರಿಮಾಣ (utsbuf));
  ret = uname (&utsbuf);
  ಯದಿ (UNLIKELY (ret != 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_DATA (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_DATA (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_ಎಣಿಕೆ (seccomp->flags_len);
  ಪ್ರತಿ (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_ಎಣಿಕೆ (seccomp->architectures_len);
  ಪ್ರತಿ (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_ಎಣಿಕೆ (seccomp->syscalls_len);
  ಪ್ರತಿ (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_DATA (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_ಎಣಿಕೆ (seccomp->syscalls[i]->names_len);
      ಪ್ರತಿ (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_ಎಣಿಕೆ (seccomp->syscalls[i]->args_len);
      ಪ್ರತಿ (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->index);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->ಮೌಲ್ಯ);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->ಮೌಲ್ಯ_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, ಪರಿಮಾಣ (hash));

  ಪ್ರತಿ (i = 0; i < 32; i++)
    {
      size_t remaining = ಪರಿಮಾಣ (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      ಯದಿ (UNLIKELY (ret != 2))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#undef PROCESS_STRING
#undef PROCESS_DATA
#undef PROCESS_OPTIONAL_DATA
#undef PROCESS_ಎಣಿಕೆ
  ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_rundir_dirfd (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *dir = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ dirfd;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_get_state_directory (&dir, state_ಬೇರು, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  dirfd = TEMP_FAILURE_RETRY (open (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  ಯದಿ (UNLIKELY (dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", dir);

  ಪ್ರತ್ಯಾಗಮ dirfd;
}

ಸಂರಚನಾ cache_entry
{
  seccomp_checksum_t checksum;
  ಸಂರಚನಾ timespec atime;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compare_entries_by_atime (ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
  ಧ್ರುವ ಸಂರಚನಾ cache_entry *entry_a = a;
  ಧ್ರುವ ಸಂರಚನಾ cache_entry *entry_b = b;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  ಯದಿ (ret)
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
evict_cache (ಪೂರ್ಣಾಙ್ಕ ಬೇರು_dfd, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ cache_dir_fd = -1;
  off_t cache_dir_inode_max_size;
  ಸಂರಚನಾ stat st;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = TEMP_FAILURE_RETRY (fstat (ಬೇರು_dfd, &st));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_size = get_cache_dir_inode_max_size (st.st_size);

  cache_dir_fd = TEMP_FAILURE_RETRY (openat (ಬೇರು_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  ಯದಿ (UNLIKELY (cache_dir_fd < 0))
    {
      ಯದಿ (errno == ENOENT)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = TEMP_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  ಯದಿ (ret == 0 && st.st_size > cache_dir_inode_max_size)
    {
      cleanup_free ಸಂರಚನಾ cache_entry *entries = ರಿಕ್ತ;
      cleanup_dir DIR *d = ರಿಕ್ತ;
      size_t i, n_entries = 0;
      ಸಂರಚನಾ dirent *de;
      ಪೂರ್ಣಾಙ್ಕ dfd;

      d = fdopendir (cache_dir_fd);
      ಯದಿ (d == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      ಯಾವತ್ ((de = readdir (d)))
        {
          ಯದಿ (de->d_name[0] == '.')
            ಅನುವರ್ತ;

          ret = TEMP_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          ಯದಿ (UNLIKELY (ret < 0))
            ಅನುವರ್ತ;

          /* The cache file is already used, it is pointless to free it.  */
          ಯದಿ (st.st_nlink > 1)
            ಅನುವರ್ತ;

          /* If the sticky bit is set, then ignore it.  */
          ಯದಿ (st.st_mode & S_ISVTX)
            ಅನುವರ್ತ;

          ಯದಿ (strlen (de->d_name) != ಪರಿಮಾಣ (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              ಅನುವರ್ತ;
            }
          ಅನ್ಯಥಾ
            {
              entries = xrealloc (entries, ಪರಿಮಾಣ (ಸಂರಚನಾ cache_entry) * (n_entries + 1));

              memcpy (entries[n_entries].checksum, de->d_name, ಪರಿಮಾಣ (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, ಪರಿಮಾಣ (ಸಂರಚನಾ cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      ಪ್ರತಿ (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_seccomp_bpf_dirfd (libcrun_container_t *container, ವರ್ಣ **rel_path, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_ಬೇರು : ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (close (dirfd));
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ dirfd;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
store_seccomp_cache (ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free ವರ್ಣ *src_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *dest_path = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (is_empty_string (ctx->checksum))
    ಪ್ರತ್ಯಾಗಮ 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  ಯದಿ (UNLIKELY (dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = evict_cache (dirfd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  ಯದಿ (UNLIKELY (ret < 0 && errno != EEXIST))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "link `%s` to `%s`", src_path, dest_path);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *ctx)
{
  ಯದಿ (ctx->container == ರಿಕ್ತ || ctx->container->container_def == ರಿಕ್ತ
      || ctx->container->container_def->linux == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
has_seccomp_configuration (ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *ctx)
{
  ಯದಿ (ctx->container == ರಿಕ್ತ || ctx->container->container_def == ರಿಕ್ತ
      || ctx->container->container_def->linux == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತ್ಯಾಗಮ ctx->container->container_def->linux->seccomp != ರಿಕ್ತ
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
find_in_cache (ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *ctx, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free ವರ್ಣ *cache_file_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    ಪ್ರತ್ಯಾಗಮ 0;

  seccomp = get_seccomp_configuration (ctx);
  ಯದಿ (seccomp == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = TEMP_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  ಯದಿ (UNLIKELY (ret < 0 && errno != ENOENT))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_generate_seccomp (ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  ಪೂರ್ಣಾಙ್ಕ ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ default_errno_ಮೌಲ್ಯ = EPERM;
  uint32_t action, default_action;
  ಧ್ರುವ ವರ್ಣ *def_action = ರಿಕ್ತ;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  ಯದಿ (gen_ctx->from_cache)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (gen_ctx->container == ರಿಕ್ತ || gen_ctx->container->container_def == ರಿಕ್ತ || gen_ctx->container->container_def->linux == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  ಯದಿ (seccomp == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  /* seccomp not available.  */
  ಯದಿ (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  ಯದಿ (def_action == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "seccomp misses the default action");

  ಯದಿ (seccomp->default_errno_ret_present)
    {
      ಯದಿ (! seccomp_action_supports_errno (def_action))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_ಮೌಲ್ಯ = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_ಮೌಲ್ಯ, &default_action, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ctx = seccomp_init (default_action);
  ಯದಿ (ctx == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "error seccomp_init");

  ಪ್ರತಿ (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      ಧ್ರುವ ವರ್ಣ *arch = seccomp->architectures[i];
      ವರ್ಣ *end, lowercase_arch[32] = {
        0,
      };

      ಯದಿ (arch == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      ಯದಿ (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, ಪರಿಮಾಣ (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      ಯದಿ (arch_token == 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  ಅನ್ಯಥಾ
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      ಯದಿ (ret < 0 && ret != -EEXIST)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "seccomp adding architecture");
    }

  ಪ್ರತಿ (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool multiple_args = false;
      ಪೂರ್ಣಾಙ್ಕ errno_ret = EPERM;

      ಯದಿ (seccomp->syscalls[i]->errno_ret_present)
        {
          ಯದಿ (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (action == default_action)
        ಅನುವರ್ತ;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      ಯದಿ (seccomp->syscalls[i]->args != ರಿಕ್ತ)
        {
          uint32_t ಎಣಿಕೆ[6] = {};
          size_t k;

          ಪ್ರತಿ (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t index = seccomp->syscalls[i]->args[k]->index;

              ಯದಿ (index >= 6)
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid seccomp index `%u`", index);

              ಎಣಿಕೆ[index]++;
              ಯದಿ (ಎಣಿಕೆ[index] > 1)
                multiple_args = true;
            }
        }

      ಪ್ರತಿ (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          ಪೂರ್ಣಾಙ್ಕ syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          ಯದಿ (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              ಯದಿ (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              ಅನುವರ್ತ;
            }

          ಯದಿ (seccomp->syscalls[i]->args == ರಿಕ್ತ)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
            }
          ಅನ್ಯಥಾ
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              ಯದಿ (multiple_args)
                {
                  size_t r;

                  ಪ್ರತಿ (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      ಸಂರಚನಾ scmp_arg_cmp arg_cmp;
                      ವರ್ಣ *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->index;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      ಯದಿ (UNLIKELY (ret < 0))
                        ಪ್ರತ್ಯಾಗಮ ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->ಮೌಲ್ಯ;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->ಮೌಲ್ಯ_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      ಯದಿ (UNLIKELY (ret < 0))
                        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              ಅನ್ಯಥಾ
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  ಧ್ರುವ size_t args_len = seccomp->syscalls[i]->args_len;
                  ಸಂರಚನಾ scmp_arg_cmp arg_cmp[6];

                  ಪ್ರತಿ (k = 0; k < args_len; k++)
                    {
                      ವರ್ಣ *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->index;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      ಯದಿ (UNLIKELY (ret < 0))
                        ಪ್ರತ್ಯಾಗಮ ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->ಮೌಲ್ಯ;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->ಮೌಲ್ಯ_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : ರಿಕ್ತ);
                  ಯದಿ (UNLIKELY (ret < 0))
                    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  ಯದಿ (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಪ್ರತ್ಯಾಗಮ store_seccomp_cache (gen_ctx, err);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_copy_seccomp (ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *gen_ctx, ಧ್ರುವ ವರ್ಣ *b64_bpf, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *bpf_data = ರಿಕ್ತ;
  size_t size = 0;
  size_t in_size;
  ಪೂರ್ಣಾಙ್ಕ consumed;
  ಪೂರ್ಣಾಙ್ಕ ret;

  in_size = strlen (b64_bpf);
  bpf_data = xmalloc (in_size + 1);

  consumed = base64_decode (b64_bpf, in_size, bpf_data, in_size, &size);
  ಯದಿ (UNLIKELY (consumed != (ಪೂರ್ಣಾಙ್ಕ) in_size))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_data, size, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_open_seccomp_bpf (ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *ctx, ಪೂರ್ಣಾಙ್ಕ *fd, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd = -1;
  cleanup_free ವರ್ಣ *dest_path = ರಿಕ್ತ;
  libcrun_container_t *container = ctx->container;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ctx->fd = *fd = -1;

  ಯದಿ (container == ರಿಕ್ತ || container->context == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  ಯದಿ (UNLIKELY (dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ dirfd;

  ಯದಿ (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (created)
        {
          ctx->from_cache = true;
          ಗಚ್ಛ open_existing_file;
        }

      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  ಅನ್ಯಥಾ
    {
    open_existing_file:
      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      ಯದಿ (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          ಯದಿ (errno == ENOENT && ! has_seccomp_configuration (ctx))
            ಪ್ರತ್ಯಾಗಮ 0;
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}
