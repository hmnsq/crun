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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "seccomp.h"
#അന്തര്ഭാവ "linux.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sched.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/mount.h>
#അന്തര്ഭാവ <sys/syscall.h>
#അന്തര്ഭാവ <sys/prctl.h>
#അന്തര്ഭാവ <sys/time.h>
#അന്തര്ഭാവ <sys/resource.h>
#അന്തര്ഭാവ <sys/sysmacros.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/utsname.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/mman.h>

#ifdef HAVE_BLAKE3
#  include <blake3.h>
#അല്ലെങ്കിൽ
#  include "blake3/blake3.h"
#endif

#എങ്കിൽ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int അസ്ഥിര പൂർണ്ണസംഖ്യ
#  endif
#endif

#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#അന്തര്ഭാവ <linux/seccomp.h>
#അന്തര്ഭാവ <linux/filter.h>
#അന്തര്ഭാവ <sys/prctl.h>
#അന്തര്ഭാവ <sys/syscall.h>

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
സ്ഥിതം വരിയിൽ off_t
get_cache_dir_inode_max_size (off_t size_rundir)
{
  തിരികെ size_rundir * 3 + MIN_CACHE_DIR_INODE_SIZE;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
syscall_seccomp (unsigned പൂർണ്ണസംഖ്യ operation, unsigned പൂർണ്ണസംഖ്യ flags, ശൂന്യം *args)
{
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_seccomp, operation, flags, args);
}

#ifdef HAVE_SECCOMP
സ്ഥിതം പൂർണ്ണസംഖ്യ
get_seccomp_operator (സ്ഥിരം അക്ഷരം *name, എണ്ണൽ scmp_compare *op, libcrun_error_t *err)
{
  സ്ഥിതം സ്ഥിരം ഘടന
  {
    സ്ഥിരം അക്ഷരം *name;
    എണ്ണൽ scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  സ്ഥിരം അക്ഷരം *p = name;
  size_t i;

  എങ്കിൽ (! has_prefix (p, "SCMP_CMP_"))
    പോകുക fail;

  p += വലുപ്പം ("SCMP_CMP_") - 1;

  വേണ്ടി (i = 0; i < വലുപ്പം (operators) / വലുപ്പം (operators[0]); i++)
    എങ്കിൽ (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        തിരികെ 0;
      }

fail:
  തിരികെ crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_seccomp_action (സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ errno_ret, uint32_t *action, libcrun_error_t *err)
{
  സ്ഥിതം സ്ഥിരം ഘടന
  {
    സ്ഥിരം അക്ഷരം *name;
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
  സ്ഥിരം അക്ഷരം *p = name;
  size_t i;

  എങ്കിൽ (! has_prefix (p, "SCMP_ACT_"))
    പോകുക fail;

  p += വലുപ്പം ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  എങ്കിൽ (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      തിരികെ 0;
    }
  എങ്കിൽ (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      തിരികെ 0;
    }

  വേണ്ടി (i = 0; i < വലുപ്പം (actions) / വലുപ്പം (actions[0]); i++)
    എങ്കിൽ (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        തിരികെ 0;
      }

fail:
  തിരികെ crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
export_bpf (scmp_filter_ctx ctx, പൂർണ്ണസംഖ്യ fd, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *bpf = ശൂന്യ;
  size_t len = 0;
  പൂർണ്ണസംഖ്യ ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, ശൂന്യ, &len);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  അല്ലെങ്കിൽ
  cleanup_close പൂർണ്ണസംഖ്യ pipe_r = -1;
  cleanup_close പൂർണ്ണസംഖ്യ pipe_w = -1;
  പൂർണ്ണസംഖ്യ p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  എങ്കിൽ (UNLIKELY (len >= (size_t) ret))
    തിരികെ crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  തിരികെ safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#endif /* HAVE_SECCOMP */

സ്ഥിതം ശൂന്യം
make_lowercase (അക്ഷരം *str)
{
  ആകുമ്പോൾ (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

സ്ഥിതം ശൂന്യം
cleanup_seccompp (ശൂന്യം *p)
{
#ifdef HAVE_SECCOMP
  scmp_filter_ctx *ctx = (ശൂന്യം **) p;
  എങ്കിൽ (*ctx)
    seccomp_release (*ctx);
#endif
}
#define cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

പൂർണ്ണസംഖ്യ
libcrun_apply_seccomp (പൂർണ്ണസംഖ്യ infd, പൂർണ്ണസംഖ്യ listener_receiver_fd, സ്ഥിരം അക്ഷരം *receiver_fd_payload,
                       size_t receiver_fd_payload_len, അക്ഷരം **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  cleanup_mmap ഘടന libcrun_mmap_s *mmap_region = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ listener_fd = -1;
  cleanup_pid പൂർണ്ണസംഖ്യ helper_proc = -1;
  ഘടന sock_fprog seccomp_filter;
  cleanup_free അക്ഷരം *bpf = ശൂന്യ;
  unsigned പൂർണ്ണസംഖ്യ flags = 0;
  size_t len;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (infd < 0)
    തിരികെ 0;

  എങ്കിൽ (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    തിരികെ crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  എങ്കിൽ (seccomp_flags == ശൂന്യ)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  അല്ലെങ്കിൽ
    {
      size_t i = 0;
      വേണ്ടി (i = 0; i < seccomp_flags_len; i++)
        {
          എങ്കിൽ (seccomp_flags[i] == ശൂന്യ)
            തിരികെ crun_make_error (err, EINVAL, "seccomp flag is not specified");

          എങ്കിൽ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          അല്ലെങ്കിൽ എങ്കിൽ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          അല്ലെങ്കിൽ എങ്കിൽ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          അല്ലെങ്കിൽ എങ്കിൽ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          അല്ലെങ്കിൽ
            തിരികെ crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (ഘടന sock_filter *) bpf;

  എങ്കിൽ (listener_receiver_fd >= 0)
    {
      cleanup_close പൂർണ്ണസംഖ്യ memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  അല്ലെങ്കിൽ
      തിരികെ crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      എങ്കിൽ (UNLIKELY (memfd < 0))
        തിരികെ crun_make_error (err, errno, "memfd_create");
#  അല്ലെങ്കിൽ
      തിരികെ crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, വലുപ്പം (atomic_int));
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, ശൂന്യ, വലുപ്പം (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (helper_proc < 0))
        തിരികെ crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      എങ്കിൽ (helper_proc == 0)
        {
          പൂർണ്ണസംഖ്യ fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          വേണ്ടി (;;)
            {
              fd = *fd_received;
              എങ്കിൽ (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  എങ്കിൽ (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  തുടരുക;
                }
#  എങ്കിൽ ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              നിർത്തുക;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      എങ്കിൽ (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  എങ്കിൽ (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      പൂർണ്ണസംഖ്യ status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      എങ്കിൽ (ret != 0)
        തിരികെ crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  തിരികെ 0;
#അല്ലെങ്കിൽ
  തിരികെ 0;
#endif
}

സ്ഥിതം bool
seccomp_action_supports_errno (സ്ഥിരം അക്ഷരം *action)
{
  തിരികെ strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, unsigned പൂർണ്ണസംഖ്യ seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  unsigned അക്ഷരം hash[32];
  ഘടന utsname utsbuf;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#define PROCESS_STRING(X)                                           \
  ചെയ്യുക                                                                \
    {                                                               \
      സ്ഥിരം അക്ഷരം *str_ = (X);                                       \
      unsigned അക്ഷരം present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? strlen (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, വലുപ്പം (present_)); \
      blake3_hasher_update (&hasher, &len_, വലുപ്പം (len_));         \
      എങ്കിൽ (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } ആകുമ്പോൾ (0)
#define PROCESS_DATA(X)                                   \
  ചെയ്യുക                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), വലുപ്പം ((X))); \
  } ആകുമ്പോൾ (0)
#define PROCESS_OPTIONAL_DATA(PRESENT, X)                           \
  ചെയ്യുക                                                                \
    {                                                               \
      unsigned അക്ഷരം present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, വലുപ്പം (present_)); \
      എങ്കിൽ (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), വലുപ്പം ((X)));         \
  } ആകുമ്പോൾ (0)
#define PROCESS_എണ്ണം(X)                                        \
  ചെയ്യുക                                                            \
    {                                                           \
      size_t എണ്ണം_2 = (X);                                      \
      blake3_hasher_update (&hasher, &എണ്ണം_2, വലുപ്പം (എണ്ണം_2)); \
  } ആകുമ്പോൾ (0)

  PROCESS_STRING (PACKAGE_VERSION);

#ifdef HAVE_SECCOMP
  {
    സ്ഥിരം ഘടന scmp_version *version = seccomp_version ();

    PROCESS_DATA (version->major);
    PROCESS_DATA (version->minor);
    PROCESS_DATA (version->micro);
  }
#endif

  memset (&utsbuf, 0, വലുപ്പം (utsbuf));
  ret = uname (&utsbuf);
  എങ്കിൽ (UNLIKELY (ret != 0))
    തിരികെ crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_DATA (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_DATA (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_എണ്ണം (seccomp->flags_len);
  വേണ്ടി (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_എണ്ണം (seccomp->architectures_len);
  വേണ്ടി (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_എണ്ണം (seccomp->syscalls_len);
  വേണ്ടി (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_DATA (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_എണ്ണം (seccomp->syscalls[i]->names_len);
      വേണ്ടി (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_എണ്ണം (seccomp->syscalls[i]->args_len);
      വേണ്ടി (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->index);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->മൂല്യം);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->മൂല്യം_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, വലുപ്പം (hash));

  വേണ്ടി (i = 0; i < 32; i++)
    {
      size_t remaining = വലുപ്പം (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      എങ്കിൽ (UNLIKELY (ret != 2))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#undef PROCESS_STRING
#undef PROCESS_DATA
#undef PROCESS_OPTIONAL_DATA
#undef PROCESS_എണ്ണം
  തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
open_rundir_dirfd (സ്ഥിരം അക്ഷരം *state_വേര്, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *dir = ശൂന്യ;
  പൂർണ്ണസംഖ്യ dirfd;
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_get_state_directory (&dir, state_വേര്, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  dirfd = TEMP_FAILURE_RETRY (open (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  എങ്കിൽ (UNLIKELY (dirfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", dir);

  തിരികെ dirfd;
}

ഘടന cache_entry
{
  seccomp_checksum_t checksum;
  ഘടന timespec atime;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
compare_entries_by_atime (സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
  സ്ഥിരം ഘടന cache_entry *entry_a = a;
  സ്ഥിരം ഘടന cache_entry *entry_b = b;
  പൂർണ്ണസംഖ്യ ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  എങ്കിൽ (ret)
    തിരികെ ret;

  തിരികെ entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
evict_cache (പൂർണ്ണസംഖ്യ വേര്_dfd, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ cache_dir_fd = -1;
  off_t cache_dir_inode_max_size;
  ഘടന stat st;
  പൂർണ്ണസംഖ്യ ret;

  ret = TEMP_FAILURE_RETRY (fstat (വേര്_dfd, &st));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_size = get_cache_dir_inode_max_size (st.st_size);

  cache_dir_fd = TEMP_FAILURE_RETRY (openat (വേര്_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  എങ്കിൽ (UNLIKELY (cache_dir_fd < 0))
    {
      എങ്കിൽ (errno == ENOENT)
        തിരികെ 0;
      തിരികെ crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = TEMP_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  എങ്കിൽ (ret == 0 && st.st_size > cache_dir_inode_max_size)
    {
      cleanup_free ഘടന cache_entry *entries = ശൂന്യ;
      cleanup_dir DIR *d = ശൂന്യ;
      size_t i, n_entries = 0;
      ഘടന dirent *de;
      പൂർണ്ണസംഖ്യ dfd;

      d = fdopendir (cache_dir_fd);
      എങ്കിൽ (d == ശൂന്യ)
        തിരികെ crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      ആകുമ്പോൾ ((de = readdir (d)))
        {
          എങ്കിൽ (de->d_name[0] == '.')
            തുടരുക;

          ret = TEMP_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തുടരുക;

          /* The cache file is already used, it is pointless to free it.  */
          എങ്കിൽ (st.st_nlink > 1)
            തുടരുക;

          /* If the sticky bit is set, then ignore it.  */
          എങ്കിൽ (st.st_mode & S_ISVTX)
            തുടരുക;

          എങ്കിൽ (strlen (de->d_name) != വലുപ്പം (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              തുടരുക;
            }
          അല്ലെങ്കിൽ
            {
              entries = xrealloc (entries, വലുപ്പം (ഘടന cache_entry) * (n_entries + 1));

              memcpy (entries[n_entries].checksum, de->d_name, വലുപ്പം (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, വലുപ്പം (ഘടന cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      വേണ്ടി (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  തിരികെ 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
open_seccomp_bpf_dirfd (libcrun_container_t *container, അക്ഷരം **rel_path, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_വേര് : ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (dirfd < 0))
    തിരികെ dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (close (dirfd));
      തിരികെ ret;
    }

  തിരികെ dirfd;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
store_seccomp_cache (ഘടന libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free അക്ഷരം *src_path = ശൂന്യ;
  cleanup_free അക്ഷരം *dest_path = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ dirfd = -1;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    തിരികെ 0;

  എങ്കിൽ (is_empty_string (ctx->checksum))
    തിരികെ 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  എങ്കിൽ (UNLIKELY (dirfd < 0))
    തിരികെ dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = evict_cache (dirfd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  എങ്കിൽ (UNLIKELY (ret < 0 && errno != EEXIST))
    തിരികെ crun_make_error (err, errno, "link `%s` to `%s`", src_path, dest_path);
  തിരികെ 0;
}

സ്ഥിതം വരിയിൽ runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (ഘടന libcrun_seccomp_gen_ctx_s *ctx)
{
  എങ്കിൽ (ctx->container == ശൂന്യ || ctx->container->container_def == ശൂന്യ
      || ctx->container->container_def->linux == ശൂന്യ)
    തിരികെ 0;

  തിരികെ ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
സ്ഥിതം വരിയിൽ bool
has_seccomp_configuration (ഘടന libcrun_seccomp_gen_ctx_s *ctx)
{
  എങ്കിൽ (ctx->container == ശൂന്യ || ctx->container->container_def == ശൂന്യ
      || ctx->container->container_def->linux == ശൂന്യ)
    തിരികെ false;

  തിരികെ ctx->container->container_def->linux->seccomp != ശൂന്യ
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
find_in_cache (ഘടന libcrun_seccomp_gen_ctx_s *ctx, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free അക്ഷരം *cache_file_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    തിരികെ 0;

  seccomp = get_seccomp_configuration (ctx);
  എങ്കിൽ (seccomp == ശൂന്യ)
    തിരികെ 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = TEMP_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  എങ്കിൽ (UNLIKELY (ret < 0 && errno != ENOENT))
    തിരികെ crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_generate_seccomp (ഘടന libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  പൂർണ്ണസംഖ്യ ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = ശൂന്യ;
  പൂർണ്ണസംഖ്യ default_errno_മൂല്യം = EPERM;
  uint32_t action, default_action;
  സ്ഥിരം അക്ഷരം *def_action = ശൂന്യ;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  എങ്കിൽ (gen_ctx->from_cache)
    തിരികെ 0;

  എങ്കിൽ (gen_ctx->container == ശൂന്യ || gen_ctx->container->container_def == ശൂന്യ || gen_ctx->container->container_def->linux == ശൂന്യ)
    തിരികെ 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  എങ്കിൽ (seccomp == ശൂന്യ)
    തിരികെ 0;

  /* seccomp not available.  */
  എങ്കിൽ (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    തിരികെ crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  എങ്കിൽ (def_action == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "seccomp misses the default action");

  എങ്കിൽ (seccomp->default_errno_ret_present)
    {
      എങ്കിൽ (! seccomp_action_supports_errno (def_action))
        തിരികെ crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_മൂല്യം = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_മൂല്യം, &default_action, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ctx = seccomp_init (default_action);
  എങ്കിൽ (ctx == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "error seccomp_init");

  വേണ്ടി (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      സ്ഥിരം അക്ഷരം *arch = seccomp->architectures[i];
      അക്ഷരം *end, lowercase_arch[32] = {
        0,
      };

      എങ്കിൽ (arch == ശൂന്യ)
        തിരികെ crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      എങ്കിൽ (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, വലുപ്പം (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      എങ്കിൽ (arch_token == 0)
        തിരികെ crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  അല്ലെങ്കിൽ
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      എങ്കിൽ (ret < 0 && ret != -EEXIST)
        തിരികെ crun_make_error (err, -ret, "seccomp adding architecture");
    }

  വേണ്ടി (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool multiple_args = false;
      പൂർണ്ണസംഖ്യ errno_ret = EPERM;

      എങ്കിൽ (seccomp->syscalls[i]->errno_ret_present)
        {
          എങ്കിൽ (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            തിരികെ crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      എങ്കിൽ (action == default_action)
        തുടരുക;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      എങ്കിൽ (seccomp->syscalls[i]->args != ശൂന്യ)
        {
          uint32_t എണ്ണം[6] = {};
          size_t k;

          വേണ്ടി (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t index = seccomp->syscalls[i]->args[k]->index;

              എങ്കിൽ (index >= 6)
                തിരികെ crun_make_error (err, 0, "invalid seccomp index `%u`", index);

              എണ്ണം[index]++;
              എങ്കിൽ (എണ്ണം[index] > 1)
                multiple_args = true;
            }
        }

      വേണ്ടി (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          പൂർണ്ണസംഖ്യ syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          എങ്കിൽ (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              എങ്കിൽ (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                തിരികെ crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              തുടരുക;
            }

          എങ്കിൽ (seccomp->syscalls[i]->args == ശൂന്യ)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
            }
          അല്ലെങ്കിൽ
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              എങ്കിൽ (multiple_args)
                {
                  size_t r;

                  വേണ്ടി (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      ഘടന scmp_arg_cmp arg_cmp;
                      അക്ഷരം *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->index;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      എങ്കിൽ (UNLIKELY (ret < 0))
                        തിരികെ ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->മൂല്യം;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->മൂല്യം_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      എങ്കിൽ (UNLIKELY (ret < 0))
                        തിരികെ crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              അല്ലെങ്കിൽ
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  സ്ഥിരം size_t args_len = seccomp->syscalls[i]->args_len;
                  ഘടന scmp_arg_cmp arg_cmp[6];

                  വേണ്ടി (k = 0; k < args_len; k++)
                    {
                      അക്ഷരം *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->index;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      എങ്കിൽ (UNLIKELY (ret < 0))
                        തിരികെ ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->മൂല്യം;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->മൂല്യം_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : ശൂന്യ);
                  എങ്കിൽ (UNLIKELY (ret < 0))
                    തിരികെ crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  എങ്കിൽ (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      തിരികെ store_seccomp_cache (gen_ctx, err);
    }

  തിരികെ 0;
#അല്ലെങ്കിൽ
  തിരികെ 0;
#endif
}

പൂർണ്ണസംഖ്യ
libcrun_copy_seccomp (ഘടന libcrun_seccomp_gen_ctx_s *gen_ctx, സ്ഥിരം അക്ഷരം *b64_bpf, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *bpf_data = ശൂന്യ;
  size_t size = 0;
  size_t in_size;
  പൂർണ്ണസംഖ്യ consumed;
  പൂർണ്ണസംഖ്യ ret;

  in_size = strlen (b64_bpf);
  bpf_data = xmalloc (in_size + 1);

  consumed = base64_decode (b64_bpf, in_size, bpf_data, in_size, &size);
  എങ്കിൽ (UNLIKELY (consumed != (പൂർണ്ണസംഖ്യ) in_size))
    തിരികെ crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_data, size, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_open_seccomp_bpf (ഘടന libcrun_seccomp_gen_ctx_s *ctx, പൂർണ്ണസംഖ്യ *fd, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ dirfd = -1;
  cleanup_free അക്ഷരം *dest_path = ശൂന്യ;
  libcrun_container_t *container = ctx->container;
  പൂർണ്ണസംഖ്യ ret;

  ctx->fd = *fd = -1;

  എങ്കിൽ (container == ശൂന്യ || container->context == ശൂന്യ)
    തിരികെ crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  എങ്കിൽ (UNLIKELY (dirfd < 0))
    തിരികെ dirfd;

  എങ്കിൽ (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      എങ്കിൽ (created)
        {
          ctx->from_cache = true;
          പോകുക open_existing_file;
        }

      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  അല്ലെങ്കിൽ
    {
    open_existing_file:
      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          എങ്കിൽ (errno == ENOENT && ! has_seccomp_configuration (ctx))
            തിരികെ 0;
          തിരികെ crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  തിരികെ 0;
}
