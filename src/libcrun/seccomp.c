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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "seccomp.h"
#অন্তর্ভাব "linux.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <sched.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <sys/mount.h>
#অন্তর্ভাব <sys/syscall.h>
#অন্তর্ভাব <sys/prctl.h>
#অন্তর্ভাব <sys/time.h>
#অন্তর্ভাব <sys/resource.h>
#অন্তর্ভাব <sys/sysmacros.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/utsname.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/mman.h>

#ifdef HAVE_BLAKE3
#  include <blake3.h>
#অন্যথা
#  include "blake3/blake3.h"
#endif

#যদি HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int অস্থির পূর্ণসংখ্যা
#  endif
#endif

#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#অন্তর্ভাব <linux/seccomp.h>
#অন্তর্ভাব <linux/filter.h>
#অন্তর্ভাব <sys/prctl.h>
#অন্তর্ভাব <sys/syscall.h>

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
#define MIN_CACHE_DIR_INODE_আকার (20 + 32 * 20)

/* Heuristic to avoid the cache directory grows indefinitely.  The inode size for
   the directory is proportional to the number of entries it contains and it is
   cheaper to compute than iterating the cache directory.  */
স্থির অন্তঃরেখ off_t
get_cache_dir_inode_max_আকার (off_t আকার_rundir)
{
  ফেরত আকার_rundir * 3 + MIN_CACHE_DIR_INODE_আকার;
}

স্থির পূর্ণসংখ্যা
syscall_seccomp (unsigned পূর্ণসংখ্যা operation, unsigned পূর্ণসংখ্যা flags, শূন্য *args)
{
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_seccomp, operation, flags, args);
}

#ifdef HAVE_SECCOMP
স্থির পূর্ণসংখ্যা
get_seccomp_operator (ধ্রুবক অক্ষর *name, গণনা scmp_compare *op, libcrun_error_t *err)
{
  স্থির ধ্রুবক কাঠামো
  {
    ধ্রুবক অক্ষর *name;
    গণনা scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  ধ্রুবক অক্ষর *p = name;
  size_t i;

  যদি (! has_prefix (p, "SCMP_CMP_"))
    যাও fail;

  p += আকার ("SCMP_CMP_") - 1;

  জন্য (i = 0; i < আকার (operators) / আকার (operators[0]); i++)
    যদি (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        ফেরত 0;
      }

fail:
  ফেরত crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

স্থির পূর্ণসংখ্যা
get_seccomp_action (ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা errno_ret, uint32_t *action, libcrun_error_t *err)
{
  স্থির ধ্রুবক কাঠামো
  {
    ধ্রুবক অক্ষর *name;
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
  ধ্রুবক অক্ষর *p = name;
  size_t i;

  যদি (! has_prefix (p, "SCMP_ACT_"))
    যাও fail;

  p += আকার ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  যদি (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      ফেরত 0;
    }
  যদি (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      ফেরত 0;
    }

  জন্য (i = 0; i < আকার (actions) / আকার (actions[0]); i++)
    যদি (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        ফেরত 0;
      }

fail:
  ফেরত crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
স্থির পূর্ণসংখ্যা
export_bpf (scmp_filter_ctx ctx, পূর্ণসংখ্যা fd, libcrun_error_t *err)
{
  cleanup_free অক্ষর *bpf = ফাঁকা;
  size_t len = 0;
  পূর্ণসংখ্যা ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, ফাঁকা, &len);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  অন্যথা
  cleanup_close পূর্ণসংখ্যা pipe_r = -1;
  cleanup_close পূর্ণসংখ্যা pipe_w = -1;
  পূর্ণসংখ্যা p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  যদি (UNLIKELY (len >= (size_t) ret))
    ফেরত crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  ফেরত safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#endif /* HAVE_SECCOMP */

স্থির শূন্য
make_lowercase (অক্ষর *str)
{
  যখন (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

স্থির শূন্য
cleanup_seccompp (শূন্য *p)
{
#ifdef HAVE_SECCOMP
  scmp_filter_ctx *ctx = (শূন্য **) p;
  যদি (*ctx)
    seccomp_release (*ctx);
#endif
}
#define cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

পূর্ণসংখ্যা
libcrun_apply_seccomp (পূর্ণসংখ্যা infd, পূর্ণসংখ্যা listener_receiver_fd, ধ্রুবক অক্ষর *receiver_fd_payload,
                       size_t receiver_fd_payload_len, অক্ষর **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  cleanup_mmap কাঠামো libcrun_mmap_s *mmap_region = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা listener_fd = -1;
  cleanup_pid পূর্ণসংখ্যা helper_proc = -1;
  কাঠামো sock_fprog seccomp_filter;
  cleanup_free অক্ষর *bpf = ফাঁকা;
  unsigned পূর্ণসংখ্যা flags = 0;
  size_t len;
  পূর্ণসংখ্যা ret;

  যদি (infd < 0)
    ফেরত 0;

  যদি (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    ফেরত crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  যদি (seccomp_flags == ফাঁকা)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  অন্যথা
    {
      size_t i = 0;
      জন্য (i = 0; i < seccomp_flags_len; i++)
        {
          যদি (seccomp_flags[i] == ফাঁকা)
            ফেরত crun_make_error (err, EINVAL, "seccomp flag is not specified");

          যদি (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          অন্যথা যদি (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          অন্যথা যদি (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          অন্যথা যদি (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          অন্যথা
            ফেরত crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (কাঠামো sock_filter *) bpf;

  যদি (listener_receiver_fd >= 0)
    {
      cleanup_close পূর্ণসংখ্যা memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  অন্যথা
      ফেরত crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      যদি (UNLIKELY (memfd < 0))
        ফেরত crun_make_error (err, errno, "memfd_create");
#  অন্যথা
      ফেরত crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, আকার (atomic_int));
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, ফাঁকা, আকার (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, ফাঁকা);
      যদি (UNLIKELY (helper_proc < 0))
        ফেরত crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      যদি (helper_proc == 0)
        {
          পূর্ণসংখ্যা fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          জন্য (;;)
            {
              fd = *fd_received;
              যদি (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  যদি (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  চালু;
                }
#  যদি ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              বিরতি;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          যদি (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  যদি (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      যদি (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  যদি (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      পূর্ণসংখ্যা status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      যদি (ret != 0)
        ফেরত crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  ফেরত 0;
#অন্যথা
  ফেরত 0;
#endif
}

স্থির bool
seccomp_action_supports_errno (ধ্রুবক অক্ষর *action)
{
  ফেরত strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
স্থির পূর্ণসংখ্যা
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, unsigned পূর্ণসংখ্যা seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  unsigned অক্ষর hash[32];
  কাঠামো utsname utsbuf;
  size_t i;
  পূর্ণসংখ্যা ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#define PROCESS_STRING(X)                                           \
  করো                                                                \
    {                                                               \
      ধ্রুবক অক্ষর *str_ = (X);                                       \
      unsigned অক্ষর present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? strlen (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, আকার (present_)); \
      blake3_hasher_update (&hasher, &len_, আকার (len_));         \
      যদি (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } যখন (0)
#define PROCESS_তথ্য(X)                                   \
  করো                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), আকার ((X))); \
  } যখন (0)
#define PROCESS_OPTIONAL_তথ্য(PRESENT, X)                           \
  করো                                                                \
    {                                                               \
      unsigned অক্ষর present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, আকার (present_)); \
      যদি (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), আকার ((X)));         \
  } যখন (0)
#define PROCESS_গুনতি(X)                                        \
  করো                                                            \
    {                                                           \
      size_t গুনতি_2 = (X);                                      \
      blake3_hasher_update (&hasher, &গুনতি_2, আকার (গুনতি_2)); \
  } যখন (0)

  PROCESS_STRING (PACKAGE_VERSION);

#ifdef HAVE_SECCOMP
  {
    ধ্রুবক কাঠামো scmp_version *version = seccomp_version ();

    PROCESS_তথ্য (version->major);
    PROCESS_তথ্য (version->minor);
    PROCESS_তথ্য (version->micro);
  }
#endif

  memset (&utsbuf, 0, আকার (utsbuf));
  ret = uname (&utsbuf);
  যদি (UNLIKELY (ret != 0))
    ফেরত crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_তথ্য (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_তথ্য (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_গুনতি (seccomp->flags_len);
  জন্য (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_গুনতি (seccomp->architectures_len);
  জন্য (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_গুনতি (seccomp->syscalls_len);
  জন্য (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_তথ্য (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_গুনতি (seccomp->syscalls[i]->names_len);
      জন্য (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_গুনতি (seccomp->syscalls[i]->args_len);
      জন্য (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_তথ্য (seccomp->syscalls[i]->args[j]->সূচক);
          PROCESS_তথ্য (seccomp->syscalls[i]->args[j]->মান);
          PROCESS_তথ্য (seccomp->syscalls[i]->args[j]->মান_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, আকার (hash));

  জন্য (i = 0; i < 32; i++)
    {
      size_t remaining = আকার (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      যদি (UNLIKELY (ret != 2))
        ফেরত crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#undef PROCESS_STRING
#undef PROCESS_তথ্য
#undef PROCESS_OPTIONAL_তথ্য
#undef PROCESS_গুনতি
  ফেরত 1;
}

স্থির পূর্ণসংখ্যা
open_rundir_dirfd (ধ্রুবক অক্ষর *state_মূল, libcrun_error_t *err)
{
  cleanup_free অক্ষর *dir = ফাঁকা;
  পূর্ণসংখ্যা dirfd;
  পূর্ণসংখ্যা ret;

  ret = libcrun_get_state_directory (&dir, state_মূল, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  dirfd = অস্থায়ী_FAILURE_RETRY (open (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  যদি (UNLIKELY (dirfd < 0))
    ফেরত crun_make_error (err, errno, "open `%s`", dir);

  ফেরত dirfd;
}

কাঠামো cache_entry
{
  seccomp_checksum_t checksum;
  কাঠামো timespec atime;
};

স্থির পূর্ণসংখ্যা
compare_entries_by_atime (ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
  ধ্রুবক কাঠামো cache_entry *entry_a = a;
  ধ্রুবক কাঠামো cache_entry *entry_b = b;
  পূর্ণসংখ্যা ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  যদি (ret)
    ফেরত ret;

  ফেরত entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

স্থির পূর্ণসংখ্যা
evict_cache (পূর্ণসংখ্যা মূল_dfd, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা cache_dir_fd = -1;
  off_t cache_dir_inode_max_আকার;
  কাঠামো stat st;
  পূর্ণসংখ্যা ret;

  ret = অস্থায়ী_FAILURE_RETRY (fstat (মূল_dfd, &st));
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_আকার = get_cache_dir_inode_max_আকার (st.st_size);

  cache_dir_fd = অস্থায়ী_FAILURE_RETRY (openat (মূল_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  যদি (UNLIKELY (cache_dir_fd < 0))
    {
      যদি (errno == ENOENT)
        ফেরত 0;
      ফেরত crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = অস্থায়ী_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  যদি (ret == 0 && st.st_size > cache_dir_inode_max_আকার)
    {
      cleanup_free কাঠামো cache_entry *entries = ফাঁকা;
      cleanup_dir DIR *d = ফাঁকা;
      size_t i, n_entries = 0;
      কাঠামো dirent *de;
      পূর্ণসংখ্যা dfd;

      d = fdopendir (cache_dir_fd);
      যদি (d == ফাঁকা)
        ফেরত crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      যখন ((de = readdir (d)))
        {
          যদি (de->d_name[0] == '.')
            চালু;

          ret = অস্থায়ী_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          যদি (UNLIKELY (ret < 0))
            চালু;

          /* The cache file is already used, it is pointless to free it.  */
          যদি (st.st_nlink > 1)
            চালু;

          /* If the sticky bit is set, then ignore it.  */
          যদি (st.st_mode & S_ISVTX)
            চালু;

          যদি (strlen (de->d_name) != আকার (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              চালু;
            }
          অন্যথা
            {
              entries = xrealloc (entries, আকার (কাঠামো cache_entry) * (n_entries + 1));

              memcpy (entries[n_entries].checksum, de->d_name, আকার (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, আকার (কাঠামো cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      জন্য (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  ফেরত 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
স্থির পূর্ণসংখ্যা
open_seccomp_bpf_dirfd (libcrun_container_t *container, অক্ষর **rel_path, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  পূর্ণসংখ্যা dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_মূল : ফাঁকা, err);
  যদি (UNLIKELY (dirfd < 0))
    ফেরত dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    {
      অস্থায়ী_FAILURE_RETRY (close (dirfd));
      ফেরত ret;
    }

  ফেরত dirfd;
}

স্থির পূর্ণসংখ্যা
store_seccomp_cache (কাঠামো libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free অক্ষর *src_path = ফাঁকা;
  cleanup_free অক্ষর *dest_path = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা dirfd = -1;
  পূর্ণসংখ্যা ret;

  যদি (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    ফেরত 0;

  যদি (is_empty_string (ctx->checksum))
    ফেরত 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  যদি (UNLIKELY (dirfd < 0))
    ফেরত dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = evict_cache (dirfd, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  যদি (UNLIKELY (ret < 0 && errno != EEXIST))
    ফেরত crun_make_error (err, errno, "link `%s` to `%s`", src_path, dest_path);
  ফেরত 0;
}

স্থির অন্তঃরেখ runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (কাঠামো libcrun_seccomp_gen_ctx_s *ctx)
{
  যদি (ctx->container == ফাঁকা || ctx->container->container_def == ফাঁকা
      || ctx->container->container_def->linux == ফাঁকা)
    ফেরত 0;

  ফেরত ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
স্থির অন্তঃরেখ bool
has_seccomp_configuration (কাঠামো libcrun_seccomp_gen_ctx_s *ctx)
{
  যদি (ctx->container == ফাঁকা || ctx->container->container_def == ফাঁকা
      || ctx->container->container_def->linux == ফাঁকা)
    ফেরত false;

  ফেরত ctx->container->container_def->linux->seccomp != ফাঁকা
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != ফাঁকা;
}

স্থির পূর্ণসংখ্যা
find_in_cache (কাঠামো libcrun_seccomp_gen_ctx_s *ctx, পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free অক্ষর *cache_file_path = ফাঁকা;
  পূর্ণসংখ্যা ret;

  যদি (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    ফেরত 0;

  seccomp = get_seccomp_configuration (ctx);
  যদি (seccomp == ফাঁকা)
    ফেরত 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = অস্থায়ী_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  যদি (UNLIKELY (ret < 0 && errno != ENOENT))
    ফেরত crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_generate_seccomp (কাঠামো libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  পূর্ণসংখ্যা ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = ফাঁকা;
  পূর্ণসংখ্যা default_errno_মান = EPERM;
  uint32_t action, default_action;
  ধ্রুবক অক্ষর *def_action = ফাঁকা;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  যদি (gen_ctx->from_cache)
    ফেরত 0;

  যদি (gen_ctx->container == ফাঁকা || gen_ctx->container->container_def == ফাঁকা || gen_ctx->container->container_def->linux == ফাঁকা)
    ফেরত 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  যদি (seccomp == ফাঁকা)
    ফেরত 0;

  /* seccomp not available.  */
  যদি (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    ফেরত crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  যদি (def_action == ফাঁকা)
    ফেরত crun_make_error (err, 0, "seccomp misses the default action");

  যদি (seccomp->default_errno_ret_present)
    {
      যদি (! seccomp_action_supports_errno (def_action))
        ফেরত crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_মান = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_মান, &default_action, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ctx = seccomp_init (default_action);
  যদি (ctx == ফাঁকা)
    ফেরত crun_make_error (err, 0, "error seccomp_init");

  জন্য (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      ধ্রুবক অক্ষর *arch = seccomp->architectures[i];
      অক্ষর *end, lowercase_arch[32] = {
        0,
      };

      যদি (arch == ফাঁকা)
        ফেরত crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      যদি (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, আকার (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      যদি (arch_token == 0)
        ফেরত crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  অন্যথা
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      যদি (ret < 0 && ret != -EEXIST)
        ফেরত crun_make_error (err, -ret, "seccomp adding architecture");
    }

  জন্য (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool multiple_args = false;
      পূর্ণসংখ্যা errno_ret = EPERM;

      যদি (seccomp->syscalls[i]->errno_ret_present)
        {
          যদি (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            ফেরত crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      যদি (action == default_action)
        চালু;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      যদি (seccomp->syscalls[i]->args != ফাঁকা)
        {
          uint32_t গুনতি[6] = {};
          size_t k;

          জন্য (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t সূচক = seccomp->syscalls[i]->args[k]->সূচক;

              যদি (সূচক >= 6)
                ফেরত crun_make_error (err, 0, "invalid seccomp index `%u`", সূচক);

              গুনতি[সূচক]++;
              যদি (গুনতি[সূচক] > 1)
                multiple_args = true;
            }
        }

      জন্য (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          পূর্ণসংখ্যা syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          যদি (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              যদি (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                ফেরত crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              চালু;
            }

          যদি (seccomp->syscalls[i]->args == ফাঁকা)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              যদি (UNLIKELY (ret < 0))
                ফেরত crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
            }
          অন্যথা
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              যদি (multiple_args)
                {
                  size_t r;

                  জন্য (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      কাঠামো scmp_arg_cmp arg_cmp;
                      অক্ষর *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->সূচক;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      যদি (UNLIKELY (ret < 0))
                        ফেরত ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->মান;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->মান_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      যদি (UNLIKELY (ret < 0))
                        ফেরত crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              অন্যথা
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  ধ্রুবক size_t args_len = seccomp->syscalls[i]->args_len;
                  কাঠামো scmp_arg_cmp arg_cmp[6];

                  জন্য (k = 0; k < args_len; k++)
                    {
                      অক্ষর *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->সূচক;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      যদি (UNLIKELY (ret < 0))
                        ফেরত ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->মান;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->মান_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : ফাঁকা);
                  যদি (UNLIKELY (ret < 0))
                    ফেরত crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  যদি (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      ফেরত store_seccomp_cache (gen_ctx, err);
    }

  ফেরত 0;
#অন্যথা
  ফেরত 0;
#endif
}

পূর্ণসংখ্যা
libcrun_copy_seccomp (কাঠামো libcrun_seccomp_gen_ctx_s *gen_ctx, ধ্রুবক অক্ষর *b64_bpf, libcrun_error_t *err)
{
  cleanup_free অক্ষর *bpf_তথ্য = ফাঁকা;
  size_t আকার_2 = 0;
  size_t in_আকার;
  পূর্ণসংখ্যা consumed;
  পূর্ণসংখ্যা ret;

  in_আকার = strlen (b64_bpf);
  bpf_তথ্য = xmalloc (in_আকার + 1);

  consumed = base64_decode (b64_bpf, in_আকার, bpf_তথ্য, in_আকার, &আকার_2);
  যদি (UNLIKELY (consumed != (পূর্ণসংখ্যা) in_আকার))
    ফেরত crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_তথ্য, আকার_2, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_open_seccomp_bpf (কাঠামো libcrun_seccomp_gen_ctx_s *ctx, পূর্ণসংখ্যা *fd, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা dirfd = -1;
  cleanup_free অক্ষর *dest_path = ফাঁকা;
  libcrun_container_t *container = ctx->container;
  পূর্ণসংখ্যা ret;

  ctx->fd = *fd = -1;

  যদি (container == ফাঁকা || container->context == ফাঁকা)
    ফেরত crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  যদি (UNLIKELY (dirfd < 0))
    ফেরত dirfd;

  যদি (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      যদি (created)
        {
          ctx->from_cache = true;
          যাও open_existing_file;
        }

      ret = অস্থায়ী_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  অন্যথা
    {
    open_existing_file:
      ret = অস্থায়ী_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      যদি (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          যদি (errno == ENOENT && ! has_seccomp_configuration (ctx))
            ফেরত 0;
          ফেরত crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  ফেরত 0;
}
