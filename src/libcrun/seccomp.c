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

#شامل <config.h>
#شامل "seccomp.h"
#شامل "linux.h"
#شامل "utils.h"
#شامل <string.h>
#شامل <sched.h>
#شامل <fcntl.h>
#شامل <unistd.h>
#شامل <sys/mount.h>
#شامل <sys/syscall.h>
#شامل <sys/prctl.h>
#شامل <sys/time.h>
#شامل <sys/resource.h>
#شامل <sys/sysmacros.h>
#شامل <sys/types.h>
#شامل <sys/utsname.h>
#شامل <sys/stat.h>
#شامل <sys/mman.h>

#ifdef HAVE_BLAKE3
#  include <blake3.h>
#وگرنه
#  include "blake3/blake3.h"
#endif

#اگر HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int فرار صحیح
#  endif
#endif

#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#شامل <linux/seccomp.h>
#شامل <linux/filter.h>
#شامل <sys/prctl.h>
#شامل <sys/syscall.h>

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
ایستا درونخطی off_t
get_cache_dir_inode_max_size (off_t size_rundir)
{
  بازگشت size_rundir * 3 + MIN_CACHE_DIR_INODE_SIZE;
}

ایستا صحیح
syscall_seccomp (unsigned صحیح operation, unsigned صحیح flags, تهی *args)
{
  بازگشت (صحیح) syscall (__NR_seccomp, operation, flags, args);
}

#ifdef HAVE_SECCOMP
ایستا صحیح
get_seccomp_operator (ثابت نویسه *name, شمارش scmp_compare *op, libcrun_error_t *err)
{
  ایستا ثابت ساختار
  {
    ثابت نویسه *name;
    شمارش scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  ثابت نویسه *p = name;
  size_t i;

  اگر (! has_prefix (p, "SCMP_CMP_"))
    برو fail;

  p += اندازه ("SCMP_CMP_") - 1;

  برای (i = 0; i < اندازه (operators) / اندازه (operators[0]); i++)
    اگر (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        بازگشت 0;
      }

fail:
  بازگشت crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

ایستا صحیح
get_seccomp_action (ثابت نویسه *name, صحیح errno_ret, uint32_t *action, libcrun_error_t *err)
{
  ایستا ثابت ساختار
  {
    ثابت نویسه *name;
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
  ثابت نویسه *p = name;
  size_t i;

  اگر (! has_prefix (p, "SCMP_ACT_"))
    برو fail;

  p += اندازه ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  اگر (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      بازگشت 0;
    }
  اگر (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      بازگشت 0;
    }

  برای (i = 0; i < اندازه (actions) / اندازه (actions[0]); i++)
    اگر (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        بازگشت 0;
      }

fail:
  بازگشت crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
ایستا صحیح
export_bpf (scmp_filter_ctx ctx, صحیح fd, libcrun_error_t *err)
{
  cleanup_free نویسه *bpf = پوچ;
  size_t len = 0;
  صحیح ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, پوچ, &len);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  وگرنه
  cleanup_close صحیح pipe_r = -1;
  cleanup_close صحیح pipe_w = -1;
  صحیح p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  اگر (UNLIKELY (len >= (size_t) ret))
    بازگشت crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  بازگشت safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#endif /* HAVE_SECCOMP */

ایستا تهی
make_lowercase (نویسه *str)
{
  وقتی (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

ایستا تهی
cleanup_seccompp (تهی *p)
{
#ifdef HAVE_SECCOMP
  scmp_filter_ctx *ctx = (تهی **) p;
  اگر (*ctx)
    seccomp_release (*ctx);
#endif
}
#define cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

صحیح
libcrun_apply_seccomp (صحیح infd, صحیح listener_receiver_fd, ثابت نویسه *receiver_fd_payload,
                       size_t receiver_fd_payload_len, نویسه **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  cleanup_mmap ساختار libcrun_mmap_s *mmap_region = پوچ;
  cleanup_close صحیح listener_fd = -1;
  cleanup_pid صحیح helper_proc = -1;
  ساختار sock_fprog seccomp_filter;
  cleanup_free نویسه *bpf = پوچ;
  unsigned صحیح flags = 0;
  size_t len;
  صحیح ret;

  اگر (infd < 0)
    بازگشت 0;

  اگر (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    بازگشت crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  اگر (seccomp_flags == پوچ)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  وگرنه
    {
      size_t i = 0;
      برای (i = 0; i < seccomp_flags_len; i++)
        {
          اگر (seccomp_flags[i] == پوچ)
            بازگشت crun_make_error (err, EINVAL, "seccomp flag is not specified");

          اگر (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          وگرنه اگر (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          وگرنه اگر (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          وگرنه اگر (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          وگرنه
            بازگشت crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (ساختار sock_filter *) bpf;

  اگر (listener_receiver_fd >= 0)
    {
      cleanup_close صحیح memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  وگرنه
      بازگشت crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      اگر (UNLIKELY (memfd < 0))
        بازگشت crun_make_error (err, errno, "memfd_create");
#  وگرنه
      بازگشت crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, اندازه (atomic_int));
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, پوچ, اندازه (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, پوچ);
      اگر (UNLIKELY (helper_proc < 0))
        بازگشت crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      اگر (helper_proc == 0)
        {
          صحیح fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          برای (;;)
            {
              fd = *fd_received;
              اگر (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  اگر (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  ادامه;
                }
#  اگر ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              بشکن;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          اگر (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  اگر (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      اگر (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  اگر (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      صحیح status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      اگر (ret != 0)
        بازگشت crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  بازگشت 0;
#وگرنه
  بازگشت 0;
#endif
}

ایستا bool
seccomp_action_supports_errno (ثابت نویسه *action)
{
  بازگشت strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
ایستا صحیح
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, unsigned صحیح seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  unsigned نویسه hash[32];
  ساختار utsname utsbuf;
  size_t i;
  صحیح ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#define PROCESS_STRING(X)                                           \
  انجامبده                                                                \
    {                                                               \
      ثابت نویسه *str_ = (X);                                       \
      unsigned نویسه present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? strlen (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, اندازه (present_)); \
      blake3_hasher_update (&hasher, &len_, اندازه (len_));         \
      اگر (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } وقتی (0)
#define PROCESS_DATA(X)                                   \
  انجامبده                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), اندازه ((X))); \
  } وقتی (0)
#define PROCESS_OPTIONAL_DATA(PRESENT, X)                           \
  انجامبده                                                                \
    {                                                               \
      unsigned نویسه present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, اندازه (present_)); \
      اگر (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), اندازه ((X)));         \
  } وقتی (0)
#define PROCESS_شمار(X)                                        \
  انجامبده                                                            \
    {                                                           \
      size_t شمار_2 = (X);                                      \
      blake3_hasher_update (&hasher, &شمار_2, اندازه (شمار_2)); \
  } وقتی (0)

  PROCESS_STRING (PACKAGE_VERSION);

#ifdef HAVE_SECCOMP
  {
    ثابت ساختار scmp_version *version = seccomp_version ();

    PROCESS_DATA (version->major);
    PROCESS_DATA (version->minor);
    PROCESS_DATA (version->micro);
  }
#endif

  memset (&utsbuf, 0, اندازه (utsbuf));
  ret = uname (&utsbuf);
  اگر (UNLIKELY (ret != 0))
    بازگشت crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_DATA (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_DATA (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_شمار (seccomp->flags_len);
  برای (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_شمار (seccomp->architectures_len);
  برای (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_شمار (seccomp->syscalls_len);
  برای (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_DATA (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_شمار (seccomp->syscalls[i]->names_len);
      برای (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_شمار (seccomp->syscalls[i]->args_len);
      برای (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->index);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->مقدار);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->مقدار_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, اندازه (hash));

  برای (i = 0; i < 32; i++)
    {
      size_t remaining = اندازه (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      اگر (UNLIKELY (ret != 2))
        بازگشت crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#undef PROCESS_STRING
#undef PROCESS_DATA
#undef PROCESS_OPTIONAL_DATA
#undef PROCESS_شمار
  بازگشت 1;
}

ایستا صحیح
open_rundir_dirfd (ثابت نویسه *state_ریشه, libcrun_error_t *err)
{
  cleanup_free نویسه *dir = پوچ;
  صحیح dirfd;
  صحیح ret;

  ret = libcrun_get_state_directory (&dir, state_ریشه, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  dirfd = TEMP_FAILURE_RETRY (open (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  اگر (UNLIKELY (dirfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", dir);

  بازگشت dirfd;
}

ساختار cache_entry
{
  seccomp_checksum_t checksum;
  ساختار timespec atime;
};

ایستا صحیح
compare_entries_by_atime (ثابت تهی *a, ثابت تهی *b)
{
  ثابت ساختار cache_entry *entry_a = a;
  ثابت ساختار cache_entry *entry_b = b;
  صحیح ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  اگر (ret)
    بازگشت ret;

  بازگشت entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

ایستا صحیح
evict_cache (صحیح ریشه_dfd, libcrun_error_t *err)
{
  cleanup_close صحیح cache_dir_fd = -1;
  off_t cache_dir_inode_max_size;
  ساختار stat st;
  صحیح ret;

  ret = TEMP_FAILURE_RETRY (fstat (ریشه_dfd, &st));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_size = get_cache_dir_inode_max_size (st.st_size);

  cache_dir_fd = TEMP_FAILURE_RETRY (openat (ریشه_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  اگر (UNLIKELY (cache_dir_fd < 0))
    {
      اگر (errno == ENOENT)
        بازگشت 0;
      بازگشت crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = TEMP_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  اگر (ret == 0 && st.st_size > cache_dir_inode_max_size)
    {
      cleanup_free ساختار cache_entry *entries = پوچ;
      cleanup_dir DIR *d = پوچ;
      size_t i, n_entries = 0;
      ساختار dirent *de;
      صحیح dfd;

      d = fdopendir (cache_dir_fd);
      اگر (d == پوچ)
        بازگشت crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      وقتی ((de = readdir (d)))
        {
          اگر (de->d_name[0] == '.')
            ادامه;

          ret = TEMP_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          اگر (UNLIKELY (ret < 0))
            ادامه;

          /* The cache file is already used, it is pointless to free it.  */
          اگر (st.st_nlink > 1)
            ادامه;

          /* If the sticky bit is set, then ignore it.  */
          اگر (st.st_mode & S_ISVTX)
            ادامه;

          اگر (strlen (de->d_name) != اندازه (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              ادامه;
            }
          وگرنه
            {
              entries = xrealloc (entries, اندازه (ساختار cache_entry) * (n_entries + 1));

              memcpy (entries[n_entries].checksum, de->d_name, اندازه (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, اندازه (ساختار cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      برای (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  بازگشت 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
ایستا صحیح
open_seccomp_bpf_dirfd (libcrun_container_t *container, نویسه **rel_path, libcrun_error_t *err)
{
  صحیح ret;
  صحیح dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_ریشه : پوچ, err);
  اگر (UNLIKELY (dirfd < 0))
    بازگشت dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", پوچ);
  اگر (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (close (dirfd));
      بازگشت ret;
    }

  بازگشت dirfd;
}

ایستا صحیح
store_seccomp_cache (ساختار libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free نویسه *src_path = پوچ;
  cleanup_free نویسه *dest_path = پوچ;
  cleanup_close صحیح dirfd = -1;
  صحیح ret;

  اگر (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    بازگشت 0;

  اگر (is_empty_string (ctx->checksum))
    بازگشت 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  اگر (UNLIKELY (dirfd < 0))
    بازگشت dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = evict_cache (dirfd, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  اگر (UNLIKELY (ret < 0 && errno != EEXIST))
    بازگشت crun_make_error (err, errno, "link `%s` to `%s`", src_path, dest_path);
  بازگشت 0;
}

ایستا درونخطی runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (ساختار libcrun_seccomp_gen_ctx_s *ctx)
{
  اگر (ctx->container == پوچ || ctx->container->container_def == پوچ
      || ctx->container->container_def->linux == پوچ)
    بازگشت 0;

  بازگشت ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
ایستا درونخطی bool
has_seccomp_configuration (ساختار libcrun_seccomp_gen_ctx_s *ctx)
{
  اگر (ctx->container == پوچ || ctx->container->container_def == پوچ
      || ctx->container->container_def->linux == پوچ)
    بازگشت false;

  بازگشت ctx->container->container_def->linux->seccomp != پوچ
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != پوچ;
}

ایستا صحیح
find_in_cache (ساختار libcrun_seccomp_gen_ctx_s *ctx, صحیح dirfd, ثابت نویسه *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free نویسه *cache_file_path = پوچ;
  صحیح ret;

  اگر (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    بازگشت 0;

  seccomp = get_seccomp_configuration (ctx);
  اگر (seccomp == پوچ)
    بازگشت 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = TEMP_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  اگر (UNLIKELY (ret < 0 && errno != ENOENT))
    بازگشت crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  بازگشت 0;
}

صحیح
libcrun_generate_seccomp (ساختار libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  صحیح ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = پوچ;
  صحیح default_errno_مقدار = EPERM;
  uint32_t action, default_action;
  ثابت نویسه *def_action = پوچ;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  اگر (gen_ctx->from_cache)
    بازگشت 0;

  اگر (gen_ctx->container == پوچ || gen_ctx->container->container_def == پوچ || gen_ctx->container->container_def->linux == پوچ)
    بازگشت 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  اگر (seccomp == پوچ)
    بازگشت 0;

  /* seccomp not available.  */
  اگر (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    بازگشت crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  اگر (def_action == پوچ)
    بازگشت crun_make_error (err, 0, "seccomp misses the default action");

  اگر (seccomp->default_errno_ret_present)
    {
      اگر (! seccomp_action_supports_errno (def_action))
        بازگشت crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_مقدار = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_مقدار, &default_action, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ctx = seccomp_init (default_action);
  اگر (ctx == پوچ)
    بازگشت crun_make_error (err, 0, "error seccomp_init");

  برای (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      ثابت نویسه *arch = seccomp->architectures[i];
      نویسه *end, lowercase_arch[32] = {
        0,
      };

      اگر (arch == پوچ)
        بازگشت crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      اگر (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, اندازه (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      اگر (arch_token == 0)
        بازگشت crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  وگرنه
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      اگر (ret < 0 && ret != -EEXIST)
        بازگشت crun_make_error (err, -ret, "seccomp adding architecture");
    }

  برای (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool multiple_args = false;
      صحیح errno_ret = EPERM;

      اگر (seccomp->syscalls[i]->errno_ret_present)
        {
          اگر (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            بازگشت crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      اگر (action == default_action)
        ادامه;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      اگر (seccomp->syscalls[i]->args != پوچ)
        {
          uint32_t شمار[6] = {};
          size_t k;

          برای (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t index = seccomp->syscalls[i]->args[k]->index;

              اگر (index >= 6)
                بازگشت crun_make_error (err, 0, "invalid seccomp index `%u`", index);

              شمار[index]++;
              اگر (شمار[index] > 1)
                multiple_args = true;
            }
        }

      برای (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          صحیح syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          اگر (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              اگر (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                بازگشت crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              ادامه;
            }

          اگر (seccomp->syscalls[i]->args == پوچ)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              اگر (UNLIKELY (ret < 0))
                بازگشت crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
            }
          وگرنه
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              اگر (multiple_args)
                {
                  size_t r;

                  برای (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      ساختار scmp_arg_cmp arg_cmp;
                      نویسه *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->index;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      اگر (UNLIKELY (ret < 0))
                        بازگشت ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->مقدار;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->مقدار_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      اگر (UNLIKELY (ret < 0))
                        بازگشت crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              وگرنه
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  ثابت size_t args_len = seccomp->syscalls[i]->args_len;
                  ساختار scmp_arg_cmp arg_cmp[6];

                  برای (k = 0; k < args_len; k++)
                    {
                      نویسه *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->index;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      اگر (UNLIKELY (ret < 0))
                        بازگشت ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->مقدار;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->مقدار_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : پوچ);
                  اگر (UNLIKELY (ret < 0))
                    بازگشت crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  اگر (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      بازگشت store_seccomp_cache (gen_ctx, err);
    }

  بازگشت 0;
#وگرنه
  بازگشت 0;
#endif
}

صحیح
libcrun_copy_seccomp (ساختار libcrun_seccomp_gen_ctx_s *gen_ctx, ثابت نویسه *b64_bpf, libcrun_error_t *err)
{
  cleanup_free نویسه *bpf_data = پوچ;
  size_t size = 0;
  size_t in_size;
  صحیح consumed;
  صحیح ret;

  in_size = strlen (b64_bpf);
  bpf_data = xmalloc (in_size + 1);

  consumed = base64_decode (b64_bpf, in_size, bpf_data, in_size, &size);
  اگر (UNLIKELY (consumed != (صحیح) in_size))
    بازگشت crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_data, size, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

صحیح
libcrun_open_seccomp_bpf (ساختار libcrun_seccomp_gen_ctx_s *ctx, صحیح *fd, libcrun_error_t *err)
{
  cleanup_close صحیح dirfd = -1;
  cleanup_free نویسه *dest_path = پوچ;
  libcrun_container_t *container = ctx->container;
  صحیح ret;

  ctx->fd = *fd = -1;

  اگر (container == پوچ || container->context == پوچ)
    بازگشت crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  اگر (UNLIKELY (dirfd < 0))
    بازگشت dirfd;

  اگر (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      اگر (created)
        {
          ctx->from_cache = true;
          برو open_existing_file;
        }

      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  وگرنه
    {
    open_existing_file:
      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      اگر (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          اگر (errno == ENOENT && ! has_seccomp_configuration (ctx))
            بازگشت 0;
          بازگشت crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  بازگشت 0;
}
