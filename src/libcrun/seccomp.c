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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "seccomp.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mount.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/prctl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/resource.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysmacros.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/utsname.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mman.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_BLAKE3
#  include <blake3.h>
#ਨਹੀਂਤਾਂ
#  include "blake3/blake3.h"
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int ਅਸਥਿਰ ਪੂਰਨ
#  endif
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SECCOMP
#  include <seccomp.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/seccomp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/filter.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/prctl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __NR_seccomp
#  define __NR_seccomp 0xffff // seccomp syscall number unknown for this architecture
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SECCOMP_SET_MODE_STRICT
#  define SECCOMP_SET_MODE_STRICT 0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SECCOMP_SET_MODE_FILTER
#  define SECCOMP_SET_MODE_FILTER 1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SECCOMP_FILTER_FLAG_TSYNC
#  define SECCOMP_FILTER_FLAG_TSYNC (1UL << 0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SECCOMP_FILTER_FLAG_LOG
#  define SECCOMP_FILTER_FLAG_LOG (1UL << 1)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SECCOMP_FILTER_FLAG_SPEC_ALLOW
#  define SECCOMP_FILTER_FLAG_SPEC_ALLOW (1UL << 2)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV
#  define SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV (1UL << 5)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SECCOMP_CACHE_DIR ".cache/seccomp"

/* On Linux 5.19.15 x86_64, tmpfs reports 20 bytes + 20 bytes for each entry, so allow
   at least 32 entries.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIN_CACHE_DIR_INODE_ਮਾਪ (20 + 32 * 20)

/* Heuristic to avoid the cache directory grows indefinitely.  The inode size for
   the directory is proportional to the number of entries it contains and it is
   cheaper to compute than iterating the cache directory.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ off_t
get_cache_dir_inode_max_ਮਾਪ (off_t ਮਾਪ_rundir)
{
  ਵਾਪਸ ਮਾਪ_rundir * 3 + MIN_CACHE_DIR_INODE_ਮਾਪ;
}

ਸਥਿਰ ਪੂਰਨ
syscall_seccomp (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ operation, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags, ਖਾਲੀ *ਦਲੀਲਾਂ)
{
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_seccomp, operation, flags, ਦਲੀਲਾਂ);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SECCOMP
ਸਥਿਰ ਪੂਰਨ
get_seccomp_operator (ਅਚਲ ਅੱਖਰ *name, ਗਿਣਤੀ scmp_compare *op, libcrun_error_t *err)
{
  ਸਥਿਰ ਅਚਲ ਬਣਤਰ
  {
    ਅਚਲ ਅੱਖਰ *name;
    ਗਿਣਤੀ scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  ਅਚਲ ਅੱਖਰ *p = name;
  size_t i;

  ਜੇ (! has_ਅਗੇਤਰ (p, "SCMP_CMP_"))
    ਜਾਓ fail;

  p += ਆਕਾਰ ("SCMP_CMP_") - 1;

  ਲਈ (i = 0; i < ਆਕਾਰ (operators) / ਆਕਾਰ (operators[0]); i++)
    ਜੇ (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        ਵਾਪਸ 0;
      }

fail:
  ਵਾਪਸ crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

ਸਥਿਰ ਪੂਰਨ
get_seccomp_action (ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ errno_ret, uint32_t *action, libcrun_error_t *err)
{
  ਸਥਿਰ ਅਚਲ ਬਣਤਰ
  {
    ਅਚਲ ਅੱਖਰ *name;
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
  ਅਚਲ ਅੱਖਰ *p = name;
  size_t i;

  ਜੇ (! has_ਅਗੇਤਰ (p, "SCMP_ACT_"))
    ਜਾਓ fail;

  p += ਆਕਾਰ ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  ਜੇ (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      ਵਾਪਸ 0;
    }
  ਜੇ (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      ਵਾਪਸ 0;
    }

  ਲਈ (i = 0; i < ਆਕਾਰ (actions) / ਆਕਾਰ (actions[0]); i++)
    ਜੇ (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        ਵਾਪਸ 0;
      }

fail:
  ਵਾਪਸ crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
ਸਥਿਰ ਪੂਰਨ
export_bpf (scmp_filter_ctx ctx, ਪੂਰਨ fd, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *bpf = ਨਿਰਾ;
  size_t len = 0;
  ਪੂਰਨ ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, ਨਿਰਾ, &len);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  ਨਹੀਂਤਾਂ
  cleanup_close ਪੂਰਨ pipe_r = -1;
  cleanup_close ਪੂਰਨ pipe_w = -1;
  ਪੂਰਨ p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  ਜੇ (UNLIKELY (len >= (size_t) ret))
    ਵਾਪਸ crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  ਵਾਪਸ ਸੁਰੱਖਿਅਤ_write (fd, "seccomp.bpf", bpf, len, err);
}
#ਜੇ_ਅੰਤ /* HAVE_SECCOMP */

ਸਥਿਰ ਖਾਲੀ
make_lowercase (ਅੱਖਰ *str)
{
  ਜਦੋਂ (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

ਸਥਿਰ ਖਾਲੀ
cleanup_seccompp (ਖਾਲੀ *p)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SECCOMP
  scmp_filter_ctx *ctx = (ਖਾਲੀ **) p;
  ਜੇ (*ctx)
    seccomp_release (*ctx);
#ਜੇ_ਅੰਤ
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

ਪੂਰਨ
libcrun_apply_seccomp (ਪੂਰਨ infd, ਪੂਰਨ listener_receiver_fd, ਅਚਲ ਅੱਖਰ *receiver_fd_payload,
                       size_t receiver_fd_payload_len, ਅੱਖਰ **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SECCOMP
  cleanup_mmap ਬਣਤਰ libcrun_mmap_s *mmap_region = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ listener_fd = -1;
  cleanup_pid ਪੂਰਨ helper_proc = -1;
  ਬਣਤਰ sock_fprog seccomp_filter;
  cleanup_free ਅੱਖਰ *bpf = ਨਿਰਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = 0;
  size_t len;
  ਪੂਰਨ ret;

  ਜੇ (infd < 0)
    ਵਾਪਸ 0;

  ਜੇ (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    ਵਾਪਸ crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  ਜੇ (seccomp_flags == ਨਿਰਾ)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  ਨਹੀਂਤਾਂ
    {
      size_t i = 0;
      ਲਈ (i = 0; i < seccomp_flags_len; i++)
        {
          ਜੇ (seccomp_flags[i] == ਨਿਰਾ)
            ਵਾਪਸ crun_make_error (err, EINVAL, "seccomp flag is not specified");

          ਜੇ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          ਨਹੀਂਤਾਂ ਜੇ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          ਨਹੀਂਤਾਂ ਜੇ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          ਨਹੀਂਤਾਂ ਜੇ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          ਨਹੀਂਤਾਂ
            ਵਾਪਸ crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (ਬਣਤਰ sock_filter *) bpf;

  ਜੇ (listener_receiver_fd >= 0)
    {
      cleanup_close ਪੂਰਨ memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  ਨਹੀਂਤਾਂ
      ਵਾਪਸ crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      ਜੇ (UNLIKELY (memfd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "memfd_create");
#  ਨਹੀਂਤਾਂ
      ਵਾਪਸ crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, ਆਕਾਰ (atomic_int));
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, ਨਿਰਾ, ਆਕਾਰ (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, ਨਿਰਾ);
      ਜੇ (UNLIKELY (helper_proc < 0))
        ਵਾਪਸ crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      ਜੇ (helper_proc == 0)
        {
          ਪੂਰਨ fd, timeout = 0;

          prctl (PR_ਸੈੱਟ_PDEATHSIG, SIGKILL);
          ਲਈ (;;)
            {
              fd = *fd_received;
              ਜੇ (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  ਜੇ (timeout++ > 5000)
                    ਸੁਰੱਖਿਅਤ_exit (EINVAL);
                  ਜਾਰੀ;
                }
#  ਜੇ ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              ਰੋਕੋ;
            }
          ret = send_fd_ਮੰਜ਼ਿਲ_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          ਜੇ (UNLIKELY (ret < 0))
            ਸੁਰੱਖਿਅਤ_exit (crun_error_get_errno (err));
          ਸੁਰੱਖਿਅਤ_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      ਜੇ (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  ਜੇ (listener_receiver_fd >= 0)
    {
      atomic_int *fd_ਮੰਜ਼ਿਲ_send = mmap_region->addr;
      ਪੂਰਨ ਸਥਿਤੀ = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_ਮੰਜ਼ਿਲ_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &ਸਥਿਤੀ, 0);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_ਸਥਿਤੀ (ਸਥਿਤੀ);
      ਜੇ (ret != 0)
        ਵਾਪਸ crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ bool
seccomp_action_supports_errno (ਅਚਲ ਅੱਖਰ *action)
{
  ਵਾਪਸ strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
ਸਥਿਰ ਪੂਰਨ
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seccomp_gen_options, seccomp_checksum_t ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  blake3_hasher hasher;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ hash[32];
  ਬਣਤਰ utsname utsbuf;
  size_t i;
  ਪੂਰਨ ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROCESS_STRING(X)                                           \
  ਕਰੋ                                                                \
    {                                                               \
      ਅਚਲ ਅੱਖਰ *str_ = (X);                                       \
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? ਲੜੀ_ਲੰਬਾਈ (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, ਆਕਾਰ (present_)); \
      blake3_hasher_update (&hasher, &len_, ਆਕਾਰ (len_));         \
      ਜੇ (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROCESS_ਅੰਕੜੇ(X)                                   \
  ਕਰੋ                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), ਆਕਾਰ ((X))); \
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROCESS_OPTIONAL_ਅੰਕੜੇ(PRESENT, X)                           \
  ਕਰੋ                                                                \
    {                                                               \
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, ਆਕਾਰ (present_)); \
      ਜੇ (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), ਆਕਾਰ ((X)));         \
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROCESS_ਗਿਣਤੀ(X)                                        \
  ਕਰੋ                                                            \
    {                                                           \
      size_t ਗਿਣਤੀ_3 = (X);                                      \
      blake3_hasher_update (&hasher, &ਗਿਣਤੀ_3, ਆਕਾਰ (ਗਿਣਤੀ_3)); \
  } ਜਦੋਂ (0)

  PROCESS_STRING (PACKAGE_ਸੰਸਕਰਣ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SECCOMP
  {
    ਅਚਲ ਬਣਤਰ scmp_ਸੰਸਕਰਣ *ਸੰਸਕਰਣ = seccomp_ਸੰਸਕਰਣ ();

    PROCESS_ਅੰਕੜੇ (ਸੰਸਕਰਣ->major);
    PROCESS_ਅੰਕੜੇ (ਸੰਸਕਰਣ->minor);
    PROCESS_ਅੰਕੜੇ (ਸੰਸਕਰਣ->micro);
  }
#ਜੇ_ਅੰਤ

  memset (&utsbuf, 0, ਆਕਾਰ (utsbuf));
  ret = uname (&utsbuf);
  ਜੇ (UNLIKELY (ret != 0))
    ਵਾਪਸ crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.ਸੰਸਕਰਣ);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_ਅੰਕੜੇ (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_ਅੰਕੜੇ (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_ਗਿਣਤੀ (seccomp->flags_len);
  ਲਈ (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_ਗਿਣਤੀ (seccomp->architectures_len);
  ਲਈ (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_ਗਿਣਤੀ (seccomp->syscalls_len);
  ਲਈ (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_ਅੰਕੜੇ (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_ਗਿਣਤੀ (seccomp->syscalls[i]->ਨਾਂ_len);
      ਲਈ (j = 0; j < seccomp->syscalls[i]->ਨਾਂ_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->ਨਾਂ[j]);

      PROCESS_ਗਿਣਤੀ (seccomp->syscalls[i]->ਦਲੀਲਾਂ_len);
      ਲਈ (j = 0; j < seccomp->syscalls[i]->ਦਲੀਲਾਂ_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_ਅੰਕੜੇ (seccomp->syscalls[i]->ਦਲੀਲਾਂ[j]->ਸੂਚਕਾਂਕ);
          PROCESS_ਅੰਕੜੇ (seccomp->syscalls[i]->ਦਲੀਲਾਂ[j]->ਮੁੱਲ);
          PROCESS_ਅੰਕੜੇ (seccomp->syscalls[i]->ਦਲੀਲਾਂ[j]->ਮੁੱਲ_two);
          PROCESS_STRING (seccomp->syscalls[i]->ਦਲੀਲਾਂ[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, ਆਕਾਰ (hash));

  ਲਈ (i = 0; i < 32; i++)
    {
      size_t ਬਚਿਆ = ਆਕਾਰ (seccomp_checksum_t) - i * 2;
      ret = snprintf (&ਬਾਹਰਲਾ[i * 2], ਬਚਿਆ, "%02x", hash[i]);
      ਜੇ (UNLIKELY (ret != 2))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  ਬਾਹਰਲਾ[64] = 0;

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PROCESS_STRING
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PROCESS_ਅੰਕੜੇ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PROCESS_OPTIONAL_ਅੰਕੜੇ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PROCESS_ਗਿਣਤੀ
  ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ
open_rundir_dirfd (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *dir = ਨਿਰਾ;
  ਪੂਰਨ dirfd;
  ਪੂਰਨ ret;

  ret = libcrun_get_state_directory (&dir, state_ਜੜ੍ਹ, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  dirfd = ਆਰਜ਼ੀ_FAILURE_RETRY (open (dir, O_ਰਸਤਾ | O_DIRECTORY | O_CLOEXEC));
  ਜੇ (UNLIKELY (dirfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", dir);

  ਵਾਪਸ dirfd;
}

ਬਣਤਰ cache_ਇੰਦਰਾਜ
{
  seccomp_checksum_t checksum;
  ਬਣਤਰ timespec atime;
};

ਸਥਿਰ ਪੂਰਨ
compare_entries_by_atime (ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
  ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ_a = a;
  ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ_b = b;
  ਪੂਰਨ ret;

  ret = ਇੰਦਰਾਜ_a->atime.tv_sec - ਇੰਦਰਾਜ_b->atime.tv_sec;
  ਜੇ (ret)
    ਵਾਪਸ ret;

  ਵਾਪਸ ਇੰਦਰਾਜ_a->atime.tv_nsec - ਇੰਦਰਾਜ_b->atime.tv_nsec;
}

ਸਥਿਰ ਪੂਰਨ
evict_cache (ਪੂਰਨ ਜੜ੍ਹ_dfd, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ cache_dir_fd = -1;
  off_t cache_dir_inode_max_ਮਾਪ;
  ਬਣਤਰ stat st;
  ਪੂਰਨ ret;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (fstat (ਜੜ੍ਹ_dfd, &st));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_ਮਾਪ = get_cache_dir_inode_max_ਮਾਪ (st.st_size);

  cache_dir_fd = ਆਰਜ਼ੀ_FAILURE_RETRY (openat (ਜੜ੍ਹ_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  ਜੇ (UNLIKELY (cache_dir_fd < 0))
    {
      ਜੇ (errno == ENOENT)
        ਵਾਪਸ 0;
      ਵਾਪਸ crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  ਜੇ (ret == 0 && st.st_size > cache_dir_inode_max_ਮਾਪ)
    {
      cleanup_free ਬਣਤਰ cache_ਇੰਦਰਾਜ *entries = ਨਿਰਾ;
      cleanup_dir DIR *d = ਨਿਰਾ;
      size_t i, n_entries = 0;
      ਬਣਤਰ dirent *de;
      ਪੂਰਨ dfd;

      d = fdopendir (cache_dir_fd);
      ਜੇ (d == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      ਜਦੋਂ ((de = readdir (d)))
        {
          ਜੇ (de->d_name[0] == '.')
            ਜਾਰੀ;

          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          ਜੇ (UNLIKELY (ret < 0))
            ਜਾਰੀ;

          /* The cache file is already used, it is pointless to free it.  */
          ਜੇ (st.st_nlink > 1)
            ਜਾਰੀ;

          /* If the sticky bit is set, then ignore it.  */
          ਜੇ (st.st_mode & S_ISVTX)
            ਜਾਰੀ;

          ਜੇ (ਲੜੀ_ਲੰਬਾਈ (de->d_name) != ਆਕਾਰ (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              ਜਾਰੀ;
            }
          ਨਹੀਂਤਾਂ
            {
              entries = xrealloc (entries, ਆਕਾਰ (ਬਣਤਰ cache_ਇੰਦਰਾਜ) * (n_entries + 1));

              ਸਮ੍ਰਿਤੀ_ਨਕਲ (entries[n_entries].checksum, de->d_name, ਆਕਾਰ (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, ਆਕਾਰ (ਬਣਤਰ cache_ਇੰਦਰਾਜ), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      ਲਈ (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  ਵਾਪਸ 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
ਸਥਿਰ ਪੂਰਨ
open_seccomp_bpf_dirfd (libcrun_container_t *container, ਅੱਖਰ **rel_ਰਸਤਾ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਪੂਰਨ dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_ਜੜ੍ਹ : ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (dirfd < 0))
    ਵਾਪਸ dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_ਰਸਤਾ, err, container->context->id, "seccomp.bpf", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਆਰਜ਼ੀ_FAILURE_RETRY (close (dirfd));
      ਵਾਪਸ ret;
    }

  ਵਾਪਸ dirfd;
}

ਸਥਿਰ ਪੂਰਨ
store_seccomp_cache (ਬਣਤਰ libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free ਅੱਖਰ *src_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *dest_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ dirfd = -1;
  ਪੂਰਨ ret;

  ਜੇ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    ਵਾਪਸ 0;

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (ctx->checksum))
    ਵਾਪਸ 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (dirfd < 0))
    ਵਾਪਸ dirfd;

  ret = append_paths (&dest_ਰਸਤਾ, err, SECCOMP_CACHE_DIR, ctx->checksum, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = evict_cache (dirfd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = linkat (dirfd, src_ਰਸਤਾ, dirfd, dest_ਰਸਤਾ, 0);
  ਜੇ (UNLIKELY (ret < 0 && errno != EEXIST))
    ਵਾਪਸ crun_make_error (err, errno, "link `%s` to `%s`", src_ਰਸਤਾ, dest_ਰਸਤਾ);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (ਬਣਤਰ libcrun_seccomp_gen_ctx_s *ctx)
{
  ਜੇ (ctx->container == ਨਿਰਾ || ctx->container->container_def == ਨਿਰਾ
      || ctx->container->container_def->linux == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਵਾਪਸ ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
has_seccomp_configuration (ਬਣਤਰ libcrun_seccomp_gen_ctx_s *ctx)
{
  ਜੇ (ctx->container == ਨਿਰਾ || ctx->container->container_def == ਨਿਰਾ
      || ctx->container->container_def->linux == ਨਿਰਾ)
    ਵਾਪਸ false;

  ਵਾਪਸ ctx->container->container_def->linux->seccomp != ਨਿਰਾ
         || ਲੱਭੋ_annotation (ctx->container, "run.oci.seccomp_bpf_data") != ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ
ਲੱਭੋ_ਵਿੱਚ_cache (ਬਣਤਰ libcrun_seccomp_gen_ctx_s *ctx, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *dest_ਰਸਤਾ, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free ਅੱਖਰ *cache_file_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ਜੇ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    ਵਾਪਸ 0;

  seccomp = get_seccomp_configuration (ctx);
  ਜੇ (seccomp == ਨਿਰਾ)
    ਵਾਪਸ 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_paths (&cache_file_ਰਸਤਾ, err, SECCOMP_CACHE_DIR, ctx->checksum, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (linkat (dirfd, cache_file_ਰਸਤਾ, dirfd, dest_ਰਸਤਾ, 0));
  ਜੇ (UNLIKELY (ret < 0 && errno != ENOENT))
    ਵਾਪਸ crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_ਰਸਤਾ, dest_ਰਸਤਾ);

  *created = ret == 0;

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_generate_seccomp (ਬਣਤਰ libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  ਪੂਰਨ ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = ਨਿਰਾ;
  ਪੂਰਨ default_errno_ਮੁੱਲ = EPERM;
  uint32_t action, default_action;
  ਅਚਲ ਅੱਖਰ *def_action = ਨਿਰਾ;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  ਜੇ (gen_ctx->from_cache)
    ਵਾਪਸ 0;

  ਜੇ (gen_ctx->container == ਨਿਰਾ || gen_ctx->container->container_def == ਨਿਰਾ || gen_ctx->container->container_def->linux == ਨਿਰਾ)
    ਵਾਪਸ 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  ਜੇ (seccomp == ਨਿਰਾ)
    ਵਾਪਸ 0;

  /* seccomp not available.  */
  ਜੇ (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    ਵਾਪਸ crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  ਜੇ (def_action == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "seccomp misses the default action");

  ਜੇ (seccomp->default_errno_ret_present)
    {
      ਜੇ (! seccomp_action_supports_errno (def_action))
        ਵਾਪਸ crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_ਮੁੱਲ = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_ਮੁੱਲ, &default_action, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ctx = seccomp_init (default_action);
  ਜੇ (ctx == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "error seccomp_init");

  ਲਈ (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      ਅਚਲ ਅੱਖਰ *arch = seccomp->architectures[i];
      ਅੱਖਰ *ਸਿਰਾ, lowercase_arch[32] = {
        0,
      };

      ਜੇ (arch == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      ਜੇ (has_ਅਗੇਤਰ (arch, "SCMP_ARCH_"))
        arch += 10;
      ਸਿਰਾ = stpncpy (lowercase_arch, arch, ਆਕਾਰ (lowercase_arch) - 1);
      *ਸਿਰਾ = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      ਜੇ (arch_token == 0)
        ਵਾਪਸ crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  ਨਹੀਂਤਾਂ
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_ਜੋੜੋ (ctx, arch_token);
      ਜੇ (ret < 0 && ret != -EEXIST)
        ਵਾਪਸ crun_make_error (err, -ret, "seccomp adding architecture");
    }

  ਲਈ (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool ਗੁਣਜ_ਦਲੀਲਾਂ = false;
      ਪੂਰਨ errno_ret = EPERM;

      ਜੇ (seccomp->syscalls[i]->errno_ret_present)
        {
          ਜੇ (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            ਵਾਪਸ crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (action == default_action)
        ਜਾਰੀ;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      ਜੇ (seccomp->syscalls[i]->ਦਲੀਲਾਂ != ਨਿਰਾ)
        {
          uint32_t ਗਿਣਤੀ_2[6] = {};
          size_t k;

          ਲਈ (k = 0; k < seccomp->syscalls[i]->ਦਲੀਲਾਂ_len; k++)
            {
              uint32_t ਸੂਚਕਾਂਕ = seccomp->syscalls[i]->ਦਲੀਲਾਂ[k]->ਸੂਚਕਾਂਕ;

              ਜੇ (ਸੂਚਕਾਂਕ >= 6)
                ਵਾਪਸ crun_make_error (err, 0, "invalid seccomp index `%u`", ਸੂਚਕਾਂਕ);

              ਗਿਣਤੀ_2[ਸੂਚਕਾਂਕ]++;
              ਜੇ (ਗਿਣਤੀ_2[ਸੂਚਕਾਂਕ] > 1)
                ਗੁਣਜ_ਦਲੀਲਾਂ = true;
            }
        }

      ਲਈ (j = 0; j < seccomp->syscalls[i]->ਨਾਂ_len; j++)
        {
          ਪੂਰਨ syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->ਨਾਂ[j]);

          ਜੇ (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              ਜੇ (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                ਵਾਪਸ crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->ਨਾਂ[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->ਨਾਂ[j]);
              ਜਾਰੀ;
            }

          ਜੇ (seccomp->syscalls[i]->ਦਲੀਲਾਂ == ਨਿਰਾ)
            {
              ret = seccomp_ਨਿਯਮ_ਜੋੜੋ (ctx, action, syscall, 0);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->ਨਾਂ[j]);
            }
          ਨਹੀਂਤਾਂ
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              ਜੇ (ਗੁਣਜ_ਦਲੀਲਾਂ)
                {
                  size_t r;

                  ਲਈ (r = 0; r < seccomp->syscalls[i]->ਦਲੀਲਾਂ_len; r++)
                    {
                      ਬਣਤਰ scmp_arg_cmp arg_cmp;
                      ਅੱਖਰ *op = seccomp->syscalls[i]->ਦਲੀਲਾਂ[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->ਦਲੀਲਾਂ[r]->ਸੂਚਕਾਂਕ;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      ਜੇ (UNLIKELY (ret < 0))
                        ਵਾਪਸ ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->ਦਲੀਲਾਂ[r]->ਮੁੱਲ;
                      arg_cmp.datum_b = seccomp->syscalls[i]->ਦਲੀਲਾਂ[r]->ਮੁੱਲ_two;

                      ret = seccomp_ਨਿਯਮ_ਜੋੜੋ_array (ctx, action, syscall, 1, &arg_cmp);
                      ਜੇ (UNLIKELY (ret < 0))
                        ਵਾਪਸ crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              ਨਹੀਂਤਾਂ
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  ਅਚਲ size_t ਦਲੀਲਾਂ_len = seccomp->syscalls[i]->ਦਲੀਲਾਂ_len;
                  ਬਣਤਰ scmp_arg_cmp arg_cmp[6];

                  ਲਈ (k = 0; k < ਦਲੀਲਾਂ_len; k++)
                    {
                      ਅੱਖਰ *op = seccomp->syscalls[i]->ਦਲੀਲਾਂ[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->ਦਲੀਲਾਂ[k]->ਸੂਚਕਾਂਕ;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      ਜੇ (UNLIKELY (ret < 0))
                        ਵਾਪਸ ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->ਦਲੀਲਾਂ[k]->ਮੁੱਲ;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->ਦਲੀਲਾਂ[k]->ਮੁੱਲ_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_ਨਿਯਮ_ਜੋੜੋ_array (ctx, action, syscall, ਦਲੀਲਾਂ_len, ਦਲੀਲਾਂ_len ? arg_cmp : ਨਿਰਾ);
                  ਜੇ (UNLIKELY (ret < 0))
                    ਵਾਪਸ crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  ਜੇ (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਵਾਪਸ store_seccomp_cache (gen_ctx, err);
    }

  ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ
libcrun_copy_seccomp (ਬਣਤਰ libcrun_seccomp_gen_ctx_s *gen_ctx, ਅਚਲ ਅੱਖਰ *b64_bpf, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *bpf_ਅੰਕੜੇ = ਨਿਰਾ;
  size_t ਮਾਪ = 0;
  size_t ਵਿੱਚ_ਮਾਪ;
  ਪੂਰਨ consumed;
  ਪੂਰਨ ret;

  ਵਿੱਚ_ਮਾਪ = ਲੜੀ_ਲੰਬਾਈ (b64_bpf);
  bpf_ਅੰਕੜੇ = xmalloc (ਵਿੱਚ_ਮਾਪ + 1);

  consumed = base64_decode (b64_bpf, ਵਿੱਚ_ਮਾਪ, bpf_ਅੰਕੜੇ, ਵਿੱਚ_ਮਾਪ, &ਮਾਪ);
  ਜੇ (UNLIKELY (consumed != (ਪੂਰਨ) ਵਿੱਚ_ਮਾਪ))
    ਵਾਪਸ crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = ਸੁਰੱਖਿਅਤ_write (gen_ctx->fd, "seccomp fd", bpf_ਅੰਕੜੇ, ਮਾਪ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_open_seccomp_bpf (ਬਣਤਰ libcrun_seccomp_gen_ctx_s *ctx, ਪੂਰਨ *fd, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ dirfd = -1;
  cleanup_free ਅੱਖਰ *dest_ਰਸਤਾ = ਨਿਰਾ;
  libcrun_container_t *container = ctx->container;
  ਪੂਰਨ ret;

  ctx->fd = *fd = -1;

  ਜੇ (container == ਨਿਰਾ || container->context == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (dirfd < 0))
    ਵਾਪਸ dirfd;

  ਜੇ (ctx->create)
    {
      bool created = false;

      ret = ਲੱਭੋ_ਵਿੱਚ_cache (ctx, dirfd, dest_ਰਸਤਾ, &created, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (created)
        {
          ctx->from_cache = true;
          ਜਾਓ open_existing_file;
        }

      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (openat (dirfd, dest_ਰਸਤਾ, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  ਨਹੀਂਤਾਂ
    {
    open_existing_file:
      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (openat (dirfd, dest_ਰਸਤਾ, O_CLOEXEC | O_RDONLY));
      ਜੇ (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          ਜੇ (errno == ENOENT && ! has_seccomp_configuration (ctx))
            ਵਾਪਸ 0;
          ਵਾਪਸ crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  ਵਾਪਸ 0;
}
