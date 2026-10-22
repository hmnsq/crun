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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_BLAKE3
#  include <blake3.h>
#ಇಲ್ಲದಿದ್ದರೆ
#  include "blake3/blake3.h"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int ಅಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
#  endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SECCOMP
#  include <seccomp.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <linux/seccomp.h>
#ಅನ್ತರ್ಭಾವ <linux/filter.h>
#ಅನ್ತರ್ಭಾವ <sys/prctl.h>
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __NR_seccomp
#  define __NR_seccomp 0xffff // seccomp syscall number unknown for this architecture
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SECCOMP_SET_MODE_STRICT
#  define SECCOMP_SET_MODE_STRICT 0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SECCOMP_SET_MODE_FILTER
#  define SECCOMP_SET_MODE_FILTER 1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SECCOMP_FILTER_FLAG_TSYNC
#  define SECCOMP_FILTER_FLAG_TSYNC (1UL << 0)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SECCOMP_FILTER_FLAG_LOG
#  define SECCOMP_FILTER_FLAG_LOG (1UL << 1)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SECCOMP_FILTER_FLAG_SPEC_ALLOW
#  define SECCOMP_FILTER_FLAG_SPEC_ALLOW (1UL << 2)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV
#  define SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV (1UL << 5)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು SECCOMP_CACHE_DIR ".cache/seccomp"

/* On Linux 5.19.15 x86_64, tmpfs reports 20 bytes + 20 bytes for each entry, so allow
   at least 32 entries.  */
#ವ್ಯಾಖ್ಯಾನಿಸು MIN_CACHE_DIR_INODE_ಗಾತ್ರ (20 + 32 * 20)

/* Heuristic to avoid the cache directory grows indefinitely.  The inode size for
   the directory is proportional to the number of entries it contains and it is
   cheaper to compute than iterating the cache directory.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ off_t
get_cache_dir_inode_max_ಗಾತ್ರ (off_t ಗಾತ್ರ_rundir)
{
  ಹಿಂತಿರುಗಿ ಗಾತ್ರ_rundir * 3 + MIN_CACHE_DIR_INODE_ಗಾತ್ರ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
syscall_seccomp (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ operation, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags, ಶೂನ್ಯ *ವಾದಗಳು)
{
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_seccomp, operation, flags, ವಾದಗಳು);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SECCOMP
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_seccomp_operator (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಎಣಿಕೆ scmp_compare *op, libcrun_error_t *err)
{
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
    ಎಣಿಕೆ scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p = name;
  size_t i;

  ಆದರೆ (! has_ಪೂರ್ವಪ್ರತ್ಯಯ (p, "SCMP_CMP_"))
    ಹೋಗು fail;

  p += ಗಾತ್ರ ("SCMP_CMP_") - 1;

  ಗಾಗಿ (i = 0; i < ಗಾತ್ರ (operators) / ಗಾತ್ರ (operators[0]); i++)
    ಆದರೆ (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        ಹಿಂತಿರುಗಿ 0;
      }

fail:
  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_seccomp_action (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ errno_ret, uint32_t *action, libcrun_error_t *err)
{
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
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
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p = name;
  size_t i;

  ಆದರೆ (! has_ಪೂರ್ವಪ್ರತ್ಯಯ (p, "SCMP_ACT_"))
    ಹೋಗು fail;

  p += ಗಾತ್ರ ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  ಆದರೆ (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      ಹಿಂತಿರುಗಿ 0;
    }
  ಆದರೆ (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      ಹಿಂತಿರುಗಿ 0;
    }

  ಗಾಗಿ (i = 0; i < ಗಾತ್ರ (actions) / ಗಾತ್ರ (actions[0]); i++)
    ಆದರೆ (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        ಹಿಂತಿರುಗಿ 0;
      }

fail:
  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
export_bpf (scmp_filter_ctx ctx, ಪೂರ್ಣಾಂಕ fd, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *bpf = ಖಾಲಿ;
  size_t len = 0;
  ಪೂರ್ಣಾಂಕ ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, ಖಾಲಿ, &len);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  ಇಲ್ಲದಿದ್ದರೆ
  cleanup_close ಪೂರ್ಣಾಂಕ pipe_r = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ pipe_w = -1;
  ಪೂರ್ಣಾಂಕ p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  ಆದರೆ (UNLIKELY (len >= (size_t) ret))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  ಹಿಂತಿರುಗಿ ಸುರಕ್ಷಿತ_write (fd, "seccomp.bpf", bpf, len, err);
}
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_SECCOMP */

ಸ್ಥಿರ ಶೂನ್ಯ
make_lowercase (ಅಕ್ಷರ *str)
{
  ಆಗ (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup_seccompp (ಶೂನ್ಯ *p)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SECCOMP
  scmp_filter_ctx *ctx = (ಶೂನ್ಯ **) p;
  ಆದರೆ (*ctx)
    seccomp_release (*ctx);
#ಆದರೆ_ಅಂತ್ಯ
}
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

ಪೂರ್ಣಾಂಕ
libcrun_apply_seccomp (ಪೂರ್ಣಾಂಕ infd, ಪೂರ್ಣಾಂಕ listener_receiver_fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *receiver_fd_payload,
                       size_t receiver_fd_payload_len, ಅಕ್ಷರ **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SECCOMP
  cleanup_mmap ರಚನೆ libcrun_mmap_s *mmap_region = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ listener_fd = -1;
  cleanup_pid ಪೂರ್ಣಾಂಕ helper_proc = -1;
  ರಚನೆ sock_fprog seccomp_filter;
  cleanup_free ಅಕ್ಷರ *bpf = ಖಾಲಿ;
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = 0;
  size_t len;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (infd < 0)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  ಆದರೆ (seccomp_flags == ಖಾಲಿ)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      size_t i = 0;
      ಗಾಗಿ (i = 0; i < seccomp_flags_len; i++)
        {
          ಆದರೆ (seccomp_flags[i] == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "seccomp flag is not specified");

          ಆದರೆ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (ರಚನೆ sock_filter *) bpf;

  ಆದರೆ (listener_receiver_fd >= 0)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      ಆದರೆ (UNLIKELY (memfd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "memfd_create");
#  ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, ಗಾತ್ರ (atomic_int));
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, ಖಾಲಿ, ಗಾತ್ರ (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (helper_proc < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      ಆದರೆ (helper_proc == 0)
        {
          ಪೂರ್ಣಾಂಕ fd, timeout = 0;

          prctl (PR_ಗಣ_PDEATHSIG, SIGKILL);
          ಗಾಗಿ (;;)
            {
              fd = *fd_received;
              ಆದರೆ (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  ಆದರೆ (timeout++ > 5000)
                    ಸುರಕ್ಷಿತ_exit (EINVAL);
                  ಮುಂದುವರಿ;
                }
#  ಆದರೆ ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              ನಿಲ್ಲಿಸು;
            }
          ret = send_fd_ಗಮ್ಯ_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಸುರಕ್ಷಿತ_exit (crun_error_get_errno (err));
          ಸುರಕ್ಷಿತ_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      ಆದರೆ (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  ಆದರೆ (listener_receiver_fd >= 0)
    {
      atomic_int *fd_ಗಮ್ಯ_send = mmap_region->addr;
      ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_ಗಮ್ಯ_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &ಸ್ಥಿತಿ_2, 0);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_ಸ್ಥಿತಿ (ಸ್ಥಿತಿ_2);
      ಆದರೆ (ret != 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
  ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ bool
seccomp_action_supports_errno (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *action)
{
  ಹಿಂತಿರುಗಿ strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ seccomp_gen_options, seccomp_checksum_t ಹೊರಹೋಗು, libcrun_error_t *err)
{
  blake3_hasher hasher;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ hash[32];
  ರಚನೆ utsname utsbuf;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#ವ್ಯಾಖ್ಯಾನಿಸು PROCESS_STRING(X)                                           \
  ಮಾಡು                                                                \
    {                                                               \
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str_ = (X);                                       \
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, ಗಾತ್ರ (present_)); \
      blake3_hasher_update (&hasher, &len_, ಗಾತ್ರ (len_));         \
      ಆದರೆ (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು PROCESS_ದತ್ತಾಂಶ(X)                                   \
  ಮಾಡು                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), ಗಾತ್ರ ((X))); \
  } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು PROCESS_OPTIONAL_ದತ್ತಾಂಶ(PRESENT, X)                           \
  ಮಾಡು                                                                \
    {                                                               \
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, ಗಾತ್ರ (present_)); \
      ಆದರೆ (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), ಗಾತ್ರ ((X)));         \
  } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು PROCESS_ಎಣಿಕೆ(X)                                        \
  ಮಾಡು                                                            \
    {                                                           \
      size_t ಎಣಿಕೆ_3 = (X);                                      \
      blake3_hasher_update (&hasher, &ಎಣಿಕೆ_3, ಗಾತ್ರ (ಎಣಿಕೆ_3)); \
  } ಆಗ (0)

  PROCESS_STRING (PACKAGE_ಆವೃತ್ತಿ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SECCOMP
  {
    ಸ್ಥಿರಾಂಕ ರಚನೆ scmp_ಆವೃತ್ತಿ *ಆವೃತ್ತಿ = seccomp_ಆವೃತ್ತಿ ();

    PROCESS_ದತ್ತಾಂಶ (ಆವೃತ್ತಿ->major);
    PROCESS_ದತ್ತಾಂಶ (ಆವೃತ್ತಿ->minor);
    PROCESS_ದತ್ತಾಂಶ (ಆವೃತ್ತಿ->micro);
  }
#ಆದರೆ_ಅಂತ್ಯ

  memset (&utsbuf, 0, ಗಾತ್ರ (utsbuf));
  ret = uname (&utsbuf);
  ಆದರೆ (UNLIKELY (ret != 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.ಆವೃತ್ತಿ);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_ದತ್ತಾಂಶ (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_ದತ್ತಾಂಶ (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_ಎಣಿಕೆ (seccomp->flags_len);
  ಗಾಗಿ (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_ಎಣಿಕೆ (seccomp->architectures_len);
  ಗಾಗಿ (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_ಎಣಿಕೆ (seccomp->syscalls_len);
  ಗಾಗಿ (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_ದತ್ತಾಂಶ (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_ಎಣಿಕೆ (seccomp->syscalls[i]->ಹೆಸರುಗಳು_len);
      ಗಾಗಿ (j = 0; j < seccomp->syscalls[i]->ಹೆಸರುಗಳು_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->ಹೆಸರುಗಳು[j]);

      PROCESS_ಎಣಿಕೆ (seccomp->syscalls[i]->ವಾದಗಳು_len);
      ಗಾಗಿ (j = 0; j < seccomp->syscalls[i]->ವಾದಗಳು_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_ದತ್ತಾಂಶ (seccomp->syscalls[i]->ವಾದಗಳು[j]->ಸೂಚ್ಯಂಕ);
          PROCESS_ದತ್ತಾಂಶ (seccomp->syscalls[i]->ವಾದಗಳು[j]->ಮೌಲ್ಯ);
          PROCESS_ದತ್ತಾಂಶ (seccomp->syscalls[i]->ವಾದಗಳು[j]->ಮೌಲ್ಯ_two);
          PROCESS_STRING (seccomp->syscalls[i]->ವಾದಗಳು[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, ಗಾತ್ರ (hash));

  ಗಾಗಿ (i = 0; i < 32; i++)
    {
      size_t ಉಳಿದ = ಗಾತ್ರ (seccomp_checksum_t) - i * 2;
      ret = snprintf (&ಹೊರಹೋಗು[i * 2], ಉಳಿದ, "%02x", hash[i]);
      ಆದರೆ (UNLIKELY (ret != 2))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  ಹೊರಹೋಗು[64] = 0;

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PROCESS_STRING
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PROCESS_ದತ್ತಾಂಶ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PROCESS_OPTIONAL_ದತ್ತಾಂಶ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PROCESS_ಎಣಿಕೆ
  ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_rundir_dirfd (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *dir = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ dirfd;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_get_state_directory (&dir, state_ಬೇರು, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  dirfd = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (open (dir, O_ಮಾರ್ಗ | O_DIRECTORY | O_CLOEXEC));
  ಆದರೆ (UNLIKELY (dirfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", dir);

  ಹಿಂತಿರುಗಿ dirfd;
}

ರಚನೆ cache_ನಮೂದು
{
  seccomp_checksum_t checksum;
  ರಚನೆ timespec atime;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compare_entries_by_atime (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ನಮೂದು_a = a;
  ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ನಮೂದು_b = b;
  ಪೂರ್ಣಾಂಕ ret;

  ret = ನಮೂದು_a->atime.tv_sec - ನಮೂದು_b->atime.tv_sec;
  ಆದರೆ (ret)
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ ನಮೂದು_a->atime.tv_nsec - ನಮೂದು_b->atime.tv_nsec;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
evict_cache (ಪೂರ್ಣಾಂಕ ಬೇರು_dfd, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ cache_dir_fd = -1;
  off_t cache_dir_inode_max_ಗಾತ್ರ;
  ರಚನೆ stat st;
  ಪೂರ್ಣಾಂಕ ret;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (fstat (ಬೇರು_dfd, &st));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_ಗಾತ್ರ = get_cache_dir_inode_max_ಗಾತ್ರ (st.st_size);

  cache_dir_fd = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (openat (ಬೇರು_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  ಆದರೆ (UNLIKELY (cache_dir_fd < 0))
    {
      ಆದರೆ (errno == ENOENT)
        ಹಿಂತಿರುಗಿ 0;
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  ಆದರೆ (ret == 0 && st.st_size > cache_dir_inode_max_ಗಾತ್ರ)
    {
      cleanup_free ರಚನೆ cache_ನಮೂದು *entries = ಖಾಲಿ;
      cleanup_dir DIR *d = ಖಾಲಿ;
      size_t i, n_entries = 0;
      ರಚನೆ dirent *de;
      ಪೂರ್ಣಾಂಕ dfd;

      d = fdopendir (cache_dir_fd);
      ಆದರೆ (d == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      ಆಗ ((de = readdir (d)))
        {
          ಆದರೆ (de->d_name[0] == '.')
            ಮುಂದುವರಿ;

          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಮುಂದುವರಿ;

          /* The cache file is already used, it is pointless to free it.  */
          ಆದರೆ (st.st_nlink > 1)
            ಮುಂದುವರಿ;

          /* If the sticky bit is set, then ignore it.  */
          ಆದರೆ (st.st_mode & S_ISVTX)
            ಮುಂದುವರಿ;

          ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (de->d_name) != ಗಾತ್ರ (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              ಮುಂದುವರಿ;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              entries = xrealloc (entries, ಗಾತ್ರ (ರಚನೆ cache_ನಮೂದು) * (n_entries + 1));

              ಸ್ಮರಣೆ_ನಕಲು (entries[n_entries].checksum, de->d_name, ಗಾತ್ರ (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, ಗಾತ್ರ (ರಚನೆ cache_ನಮೂದು), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      ಗಾಗಿ (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  ಹಿಂತಿರುಗಿ 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_seccomp_bpf_dirfd (libcrun_container_t *container, ಅಕ್ಷರ **rel_ಮಾರ್ಗ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_ಬೇರು : ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (dirfd < 0))
    ಹಿಂತಿರುಗಿ dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_ಮಾರ್ಗ, err, container->context->id, "seccomp.bpf", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (dirfd));
      ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ dirfd;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
store_seccomp_cache (ರಚನೆ libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free ಅಕ್ಷರ *src_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *dest_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ dirfd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (ctx->checksum))
    ಹಿಂತಿರುಗಿ 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (dirfd < 0))
    ಹಿಂತಿರುಗಿ dirfd;

  ret = append_paths (&dest_ಮಾರ್ಗ, err, SECCOMP_CACHE_DIR, ctx->checksum, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = evict_cache (dirfd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = linkat (dirfd, src_ಮಾರ್ಗ, dirfd, dest_ಮಾರ್ಗ, 0);
  ಆದರೆ (UNLIKELY (ret < 0 && errno != EEXIST))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "link `%s` to `%s`", src_ಮಾರ್ಗ, dest_ಮಾರ್ಗ);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (ರಚನೆ libcrun_seccomp_gen_ctx_s *ctx)
{
  ಆದರೆ (ctx->container == ಖಾಲಿ || ctx->container->container_def == ಖಾಲಿ
      || ctx->container->container_def->linux == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
has_seccomp_configuration (ರಚನೆ libcrun_seccomp_gen_ctx_s *ctx)
{
  ಆದರೆ (ctx->container == ಖಾಲಿ || ctx->container->container_def == ಖಾಲಿ
      || ctx->container->container_def->linux == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ ctx->container->container_def->linux->seccomp != ಖಾಲಿ
         || ಕಂಡುಹಿಡಿ_annotation (ctx->container, "run.oci.seccomp_bpf_data") != ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಕಂಡುಹಿಡಿ_ಒಳ_cache (ರಚನೆ libcrun_seccomp_gen_ctx_s *ctx, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dest_ಮಾರ್ಗ, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free ಅಕ್ಷರ *cache_file_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    ಹಿಂತಿರುಗಿ 0;

  seccomp = get_seccomp_configuration (ctx);
  ಆದರೆ (seccomp == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_paths (&cache_file_ಮಾರ್ಗ, err, SECCOMP_CACHE_DIR, ctx->checksum, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (linkat (dirfd, cache_file_ಮಾರ್ಗ, dirfd, dest_ಮಾರ್ಗ, 0));
  ಆದರೆ (UNLIKELY (ret < 0 && errno != ENOENT))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_ಮಾರ್ಗ, dest_ಮಾರ್ಗ);

  *created = ret == 0;

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_generate_seccomp (ರಚನೆ libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  ಪೂರ್ಣಾಂಕ ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ default_errno_ಮೌಲ್ಯ = EPERM;
  uint32_t action, default_action;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *def_action = ಖಾಲಿ;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  ಆದರೆ (gen_ctx->from_cache)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (gen_ctx->container == ಖಾಲಿ || gen_ctx->container->container_def == ಖಾಲಿ || gen_ctx->container->container_def->linux == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  ಆದರೆ (seccomp == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  /* seccomp not available.  */
  ಆದರೆ (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  ಆದರೆ (def_action == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "seccomp misses the default action");

  ಆದರೆ (seccomp->default_errno_ret_present)
    {
      ಆದರೆ (! seccomp_action_supports_errno (def_action))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_ಮೌಲ್ಯ = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_ಮೌಲ್ಯ, &default_action, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ctx = seccomp_init (default_action);
  ಆದರೆ (ctx == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "error seccomp_init");

  ಗಾಗಿ (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arch = seccomp->architectures[i];
      ಅಕ್ಷರ *ಕೊನೆ, lowercase_arch[32] = {
        0,
      };

      ಆದರೆ (arch == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (arch, "SCMP_ARCH_"))
        arch += 10;
      ಕೊನೆ = stpncpy (lowercase_arch, arch, ಗಾತ್ರ (lowercase_arch) - 1);
      *ಕೊನೆ = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      ಆದರೆ (arch_token == 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  ಇಲ್ಲದಿದ್ದರೆ
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_ಕೂಡಿಸು (ctx, arch_token);
      ಆದರೆ (ret < 0 && ret != -EEXIST)
        ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "seccomp adding architecture");
    }

  ಗಾಗಿ (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool ಅಪವರ್ತ್ಯ_ವಾದಗಳು = false;
      ಪೂರ್ಣಾಂಕ errno_ret = EPERM;

      ಆದರೆ (seccomp->syscalls[i]->errno_ret_present)
        {
          ಆದರೆ (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (action == default_action)
        ಮುಂದುವರಿ;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      ಆದರೆ (seccomp->syscalls[i]->ವಾದಗಳು != ಖಾಲಿ)
        {
          uint32_t ಎಣಿಕೆ_2[6] = {};
          size_t k;

          ಗಾಗಿ (k = 0; k < seccomp->syscalls[i]->ವಾದಗಳು_len; k++)
            {
              uint32_t ಸೂಚ್ಯಂಕ = seccomp->syscalls[i]->ವಾದಗಳು[k]->ಸೂಚ್ಯಂಕ;

              ಆದರೆ (ಸೂಚ್ಯಂಕ >= 6)
                ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid seccomp index `%u`", ಸೂಚ್ಯಂಕ);

              ಎಣಿಕೆ_2[ಸೂಚ್ಯಂಕ]++;
              ಆದರೆ (ಎಣಿಕೆ_2[ಸೂಚ್ಯಂಕ] > 1)
                ಅಪವರ್ತ್ಯ_ವಾದಗಳು = true;
            }
        }

      ಗಾಗಿ (j = 0; j < seccomp->syscalls[i]->ಹೆಸರುಗಳು_len; j++)
        {
          ಪೂರ್ಣಾಂಕ syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->ಹೆಸರುಗಳು[j]);

          ಆದರೆ (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              ಆದರೆ (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->ಹೆಸರುಗಳು[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->ಹೆಸರುಗಳು[j]);
              ಮುಂದುವರಿ;
            }

          ಆದರೆ (seccomp->syscalls[i]->ವಾದಗಳು == ಖಾಲಿ)
            {
              ret = seccomp_ನಿಯಮ_ಕೂಡಿಸು (ctx, action, syscall, 0);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->ಹೆಸರುಗಳು[j]);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              ಆದರೆ (ಅಪವರ್ತ್ಯ_ವಾದಗಳು)
                {
                  size_t r;

                  ಗಾಗಿ (r = 0; r < seccomp->syscalls[i]->ವಾದಗಳು_len; r++)
                    {
                      ರಚನೆ scmp_arg_cmp arg_cmp;
                      ಅಕ್ಷರ *op = seccomp->syscalls[i]->ವಾದಗಳು[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->ವಾದಗಳು[r]->ಸೂಚ್ಯಂಕ;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      ಆದರೆ (UNLIKELY (ret < 0))
                        ಹಿಂತಿರುಗಿ ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->ವಾದಗಳು[r]->ಮೌಲ್ಯ;
                      arg_cmp.datum_b = seccomp->syscalls[i]->ವಾದಗಳು[r]->ಮೌಲ್ಯ_two;

                      ret = seccomp_ನಿಯಮ_ಕೂಡಿಸು_array (ctx, action, syscall, 1, &arg_cmp);
                      ಆದರೆ (UNLIKELY (ret < 0))
                        ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  ಸ್ಥಿರಾಂಕ size_t ವಾದಗಳು_len = seccomp->syscalls[i]->ವಾದಗಳು_len;
                  ರಚನೆ scmp_arg_cmp arg_cmp[6];

                  ಗಾಗಿ (k = 0; k < ವಾದಗಳು_len; k++)
                    {
                      ಅಕ್ಷರ *op = seccomp->syscalls[i]->ವಾದಗಳು[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->ವಾದಗಳು[k]->ಸೂಚ್ಯಂಕ;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      ಆದರೆ (UNLIKELY (ret < 0))
                        ಹಿಂತಿರುಗಿ ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->ವಾದಗಳು[k]->ಮೌಲ್ಯ;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->ವಾದಗಳು[k]->ಮೌಲ್ಯ_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_ನಿಯಮ_ಕೂಡಿಸು_array (ctx, action, syscall, ವಾದಗಳು_len, ವಾದಗಳು_len ? arg_cmp : ಖಾಲಿ);
                  ಆದರೆ (UNLIKELY (ret < 0))
                    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  ಆದರೆ (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಹಿಂತಿರುಗಿ store_seccomp_cache (gen_ctx, err);
    }

  ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
  ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ
libcrun_copy_seccomp (ರಚನೆ libcrun_seccomp_gen_ctx_s *gen_ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *b64_bpf, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *bpf_ದತ್ತಾಂಶ = ಖಾಲಿ;
  size_t ಗಾತ್ರ_2 = 0;
  size_t ಒಳ_ಗಾತ್ರ;
  ಪೂರ್ಣಾಂಕ consumed;
  ಪೂರ್ಣಾಂಕ ret;

  ಒಳ_ಗಾತ್ರ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (b64_bpf);
  bpf_ದತ್ತಾಂಶ = xmalloc (ಒಳ_ಗಾತ್ರ + 1);

  consumed = base64_decode (b64_bpf, ಒಳ_ಗಾತ್ರ, bpf_ದತ್ತಾಂಶ, ಒಳ_ಗಾತ್ರ, &ಗಾತ್ರ_2);
  ಆದರೆ (UNLIKELY (consumed != (ಪೂರ್ಣಾಂಕ) ಒಳ_ಗಾತ್ರ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = ಸುರಕ್ಷಿತ_write (gen_ctx->fd, "seccomp fd", bpf_ದತ್ತಾಂಶ, ಗಾತ್ರ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_open_seccomp_bpf (ರಚನೆ libcrun_seccomp_gen_ctx_s *ctx, ಪೂರ್ಣಾಂಕ *fd, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ dirfd = -1;
  cleanup_free ಅಕ್ಷರ *dest_ಮಾರ್ಗ = ಖಾಲಿ;
  libcrun_container_t *container = ctx->container;
  ಪೂರ್ಣಾಂಕ ret;

  ctx->fd = *fd = -1;

  ಆದರೆ (container == ಖಾಲಿ || container->context == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (dirfd < 0))
    ಹಿಂತಿರುಗಿ dirfd;

  ಆದರೆ (ctx->create)
    {
      bool created = false;

      ret = ಕಂಡುಹಿಡಿ_ಒಳ_cache (ctx, dirfd, dest_ಮಾರ್ಗ, &created, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (created)
        {
          ctx->from_cache = true;
          ಹೋಗು open_existing_file;
        }

      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (openat (dirfd, dest_ಮಾರ್ಗ, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
    open_existing_file:
      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (openat (dirfd, dest_ಮಾರ್ಗ, O_CLOEXEC | O_RDONLY));
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          ಆದರೆ (errno == ENOENT && ! has_seccomp_configuration (ctx))
            ಹಿಂತಿರುಗಿ 0;
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}
