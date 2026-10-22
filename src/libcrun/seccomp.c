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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "seccomp.h"
#அந்தர்பாவ "linux.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/mount.h>
#அந்தர்பாவ <sys/syscall.h>
#அந்தர்பாவ <sys/prctl.h>
#அந்தர்பாவ <sys/time.h>
#அந்தர்பாவ <sys/resource.h>
#அந்தர்பாவ <sys/sysmacros.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/utsname.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/mman.h>

#ifdef HAVE_BLAKE3
#  include <blake3.h>
#இல்லையெனில்
#  include "blake3/blake3.h"
#endif

#எனில் HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int நிலையற்ற முழு
#  endif
#endif

#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#அந்தர்பாவ <linux/seccomp.h>
#அந்தர்பாவ <linux/filter.h>
#அந்தர்பாவ <sys/prctl.h>
#அந்தர்பாவ <sys/syscall.h>

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
நிலையான உள்வரி off_t
get_cache_dir_inode_max_size (off_t size_rundir)
{
  திரும்பு size_rundir * 3 + MIN_CACHE_DIR_INODE_SIZE;
}

நிலையான முழு
syscall_seccomp (unsigned முழு operation, unsigned முழு flags, வெறுமை *args)
{
  திரும்பு (முழு) syscall (__NR_seccomp, operation, flags, args);
}

#ifdef HAVE_SECCOMP
நிலையான முழு
get_seccomp_operator (மாறிலி எழுத்து *name, எண்ணிக்கை scmp_compare *op, libcrun_error_t *err)
{
  நிலையான மாறிலி கட்டமைப்பு
  {
    மாறிலி எழுத்து *name;
    எண்ணிக்கை scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  மாறிலி எழுத்து *p = name;
  size_t i;

  எனில் (! has_prefix (p, "SCMP_CMP_"))
    செல் fail;

  p += அளவு ("SCMP_CMP_") - 1;

  க்கு (i = 0; i < அளவு (operators) / அளவு (operators[0]); i++)
    எனில் (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        திரும்பு 0;
      }

fail:
  திரும்பு crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

நிலையான முழு
get_seccomp_action (மாறிலி எழுத்து *name, முழு errno_ret, uint32_t *action, libcrun_error_t *err)
{
  நிலையான மாறிலி கட்டமைப்பு
  {
    மாறிலி எழுத்து *name;
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
  மாறிலி எழுத்து *p = name;
  size_t i;

  எனில் (! has_prefix (p, "SCMP_ACT_"))
    செல் fail;

  p += அளவு ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  எனில் (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      திரும்பு 0;
    }
  எனில் (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      திரும்பு 0;
    }

  க்கு (i = 0; i < அளவு (actions) / அளவு (actions[0]); i++)
    எனில் (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        திரும்பு 0;
      }

fail:
  திரும்பு crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
நிலையான முழு
export_bpf (scmp_filter_ctx ctx, முழு fd, libcrun_error_t *err)
{
  cleanup_free எழுத்து *bpf = வெற்று;
  size_t len = 0;
  முழு ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, வெற்று, &len);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  இல்லையெனில்
  cleanup_close முழு pipe_r = -1;
  cleanup_close முழு pipe_w = -1;
  முழு p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  எனில் (UNLIKELY (len >= (size_t) ret))
    திரும்பு crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  திரும்பு safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#endif /* HAVE_SECCOMP */

நிலையான வெறுமை
make_lowercase (எழுத்து *str)
{
  போது (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

நிலையான வெறுமை
cleanup_seccompp (வெறுமை *p)
{
#ifdef HAVE_SECCOMP
  scmp_filter_ctx *ctx = (வெறுமை **) p;
  எனில் (*ctx)
    seccomp_release (*ctx);
#endif
}
#define cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

முழு
libcrun_apply_seccomp (முழு infd, முழு listener_receiver_fd, மாறிலி எழுத்து *receiver_fd_payload,
                       size_t receiver_fd_payload_len, எழுத்து **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  cleanup_mmap கட்டமைப்பு libcrun_mmap_s *mmap_region = வெற்று;
  cleanup_close முழு listener_fd = -1;
  cleanup_pid முழு helper_proc = -1;
  கட்டமைப்பு sock_fprog seccomp_filter;
  cleanup_free எழுத்து *bpf = வெற்று;
  unsigned முழு flags = 0;
  size_t len;
  முழு ret;

  எனில் (infd < 0)
    திரும்பு 0;

  எனில் (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    திரும்பு crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  எனில் (seccomp_flags == வெற்று)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  இல்லையெனில்
    {
      size_t i = 0;
      க்கு (i = 0; i < seccomp_flags_len; i++)
        {
          எனில் (seccomp_flags[i] == வெற்று)
            திரும்பு crun_make_error (err, EINVAL, "seccomp flag is not specified");

          எனில் (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          இல்லையெனில் எனில் (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          இல்லையெனில் எனில் (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          இல்லையெனில் எனில் (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          இல்லையெனில்
            திரும்பு crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (கட்டமைப்பு sock_filter *) bpf;

  எனில் (listener_receiver_fd >= 0)
    {
      cleanup_close முழு memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  இல்லையெனில்
      திரும்பு crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      எனில் (UNLIKELY (memfd < 0))
        திரும்பு crun_make_error (err, errno, "memfd_create");
#  இல்லையெனில்
      திரும்பு crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, அளவு (atomic_int));
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, வெற்று, அளவு (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, வெற்று);
      எனில் (UNLIKELY (helper_proc < 0))
        திரும்பு crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      எனில் (helper_proc == 0)
        {
          முழு fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          க்கு (;;)
            {
              fd = *fd_received;
              எனில் (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  எனில் (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  தொடர்;
                }
#  எனில் ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              நிறுத்து;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          எனில் (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  எனில் (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      எனில் (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  எனில் (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      முழு status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      எனில் (ret != 0)
        திரும்பு crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  திரும்பு 0;
#இல்லையெனில்
  திரும்பு 0;
#endif
}

நிலையான bool
seccomp_action_supports_errno (மாறிலி எழுத்து *action)
{
  திரும்பு strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
நிலையான முழு
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, unsigned முழு seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  unsigned எழுத்து hash[32];
  கட்டமைப்பு utsname utsbuf;
  size_t i;
  முழு ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#define PROCESS_STRING(X)                                           \
  செய்                                                                \
    {                                                               \
      மாறிலி எழுத்து *str_ = (X);                                       \
      unsigned எழுத்து present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? strlen (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, அளவு (present_)); \
      blake3_hasher_update (&hasher, &len_, அளவு (len_));         \
      எனில் (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } போது (0)
#define PROCESS_DATA(X)                                   \
  செய்                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), அளவு ((X))); \
  } போது (0)
#define PROCESS_OPTIONAL_DATA(PRESENT, X)                           \
  செய்                                                                \
    {                                                               \
      unsigned எழுத்து present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, அளவு (present_)); \
      எனில் (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), அளவு ((X)));         \
  } போது (0)
#define PROCESS_மொத்தம்(X)                                        \
  செய்                                                            \
    {                                                           \
      size_t மொத்தம்_2 = (X);                                      \
      blake3_hasher_update (&hasher, &மொத்தம்_2, அளவு (மொத்தம்_2)); \
  } போது (0)

  PROCESS_STRING (PACKAGE_VERSION);

#ifdef HAVE_SECCOMP
  {
    மாறிலி கட்டமைப்பு scmp_version *version = seccomp_version ();

    PROCESS_DATA (version->major);
    PROCESS_DATA (version->minor);
    PROCESS_DATA (version->micro);
  }
#endif

  memset (&utsbuf, 0, அளவு (utsbuf));
  ret = uname (&utsbuf);
  எனில் (UNLIKELY (ret != 0))
    திரும்பு crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_DATA (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_DATA (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_மொத்தம் (seccomp->flags_len);
  க்கு (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_மொத்தம் (seccomp->architectures_len);
  க்கு (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_மொத்தம் (seccomp->syscalls_len);
  க்கு (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_DATA (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_மொத்தம் (seccomp->syscalls[i]->names_len);
      க்கு (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_மொத்தம் (seccomp->syscalls[i]->args_len);
      க்கு (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->index);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->மதிப்பு);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->மதிப்பு_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, அளவு (hash));

  க்கு (i = 0; i < 32; i++)
    {
      size_t remaining = அளவு (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      எனில் (UNLIKELY (ret != 2))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#undef PROCESS_STRING
#undef PROCESS_DATA
#undef PROCESS_OPTIONAL_DATA
#undef PROCESS_மொத்தம்
  திரும்பு 1;
}

நிலையான முழு
open_rundir_dirfd (மாறிலி எழுத்து *state_வேர், libcrun_error_t *err)
{
  cleanup_free எழுத்து *dir = வெற்று;
  முழு dirfd;
  முழு ret;

  ret = libcrun_get_state_directory (&dir, state_வேர், வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  dirfd = TEMP_FAILURE_RETRY (open (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  எனில் (UNLIKELY (dirfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", dir);

  திரும்பு dirfd;
}

கட்டமைப்பு cache_entry
{
  seccomp_checksum_t checksum;
  கட்டமைப்பு timespec atime;
};

நிலையான முழு
compare_entries_by_atime (மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
  மாறிலி கட்டமைப்பு cache_entry *entry_a = a;
  மாறிலி கட்டமைப்பு cache_entry *entry_b = b;
  முழு ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  எனில் (ret)
    திரும்பு ret;

  திரும்பு entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

நிலையான முழு
evict_cache (முழு வேர்_dfd, libcrun_error_t *err)
{
  cleanup_close முழு cache_dir_fd = -1;
  off_t cache_dir_inode_max_size;
  கட்டமைப்பு stat st;
  முழு ret;

  ret = TEMP_FAILURE_RETRY (fstat (வேர்_dfd, &st));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_size = get_cache_dir_inode_max_size (st.st_size);

  cache_dir_fd = TEMP_FAILURE_RETRY (openat (வேர்_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  எனில் (UNLIKELY (cache_dir_fd < 0))
    {
      எனில் (errno == ENOENT)
        திரும்பு 0;
      திரும்பு crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = TEMP_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  எனில் (ret == 0 && st.st_size > cache_dir_inode_max_size)
    {
      cleanup_free கட்டமைப்பு cache_entry *entries = வெற்று;
      cleanup_dir DIR *d = வெற்று;
      size_t i, n_entries = 0;
      கட்டமைப்பு dirent *de;
      முழு dfd;

      d = fdopendir (cache_dir_fd);
      எனில் (d == வெற்று)
        திரும்பு crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      போது ((de = readdir (d)))
        {
          எனில் (de->d_name[0] == '.')
            தொடர்;

          ret = TEMP_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          எனில் (UNLIKELY (ret < 0))
            தொடர்;

          /* The cache file is already used, it is pointless to free it.  */
          எனில் (st.st_nlink > 1)
            தொடர்;

          /* If the sticky bit is set, then ignore it.  */
          எனில் (st.st_mode & S_ISVTX)
            தொடர்;

          எனில் (strlen (de->d_name) != அளவு (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              தொடர்;
            }
          இல்லையெனில்
            {
              entries = xrealloc (entries, அளவு (கட்டமைப்பு cache_entry) * (n_entries + 1));

              memcpy (entries[n_entries].checksum, de->d_name, அளவு (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, அளவு (கட்டமைப்பு cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      க்கு (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  திரும்பு 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
நிலையான முழு
open_seccomp_bpf_dirfd (libcrun_container_t *container, எழுத்து **rel_path, libcrun_error_t *err)
{
  முழு ret;
  முழு dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_வேர் : வெற்று, err);
  எனில் (UNLIKELY (dirfd < 0))
    திரும்பு dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (close (dirfd));
      திரும்பு ret;
    }

  திரும்பு dirfd;
}

நிலையான முழு
store_seccomp_cache (கட்டமைப்பு libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free எழுத்து *src_path = வெற்று;
  cleanup_free எழுத்து *dest_path = வெற்று;
  cleanup_close முழு dirfd = -1;
  முழு ret;

  எனில் (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    திரும்பு 0;

  எனில் (is_empty_string (ctx->checksum))
    திரும்பு 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  எனில் (UNLIKELY (dirfd < 0))
    திரும்பு dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = evict_cache (dirfd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  எனில் (UNLIKELY (ret < 0 && errno != EEXIST))
    திரும்பு crun_make_error (err, errno, "link `%s` to `%s`", src_path, dest_path);
  திரும்பு 0;
}

நிலையான உள்வரி runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (கட்டமைப்பு libcrun_seccomp_gen_ctx_s *ctx)
{
  எனில் (ctx->container == வெற்று || ctx->container->container_def == வெற்று
      || ctx->container->container_def->linux == வெற்று)
    திரும்பு 0;

  திரும்பு ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
நிலையான உள்வரி bool
has_seccomp_configuration (கட்டமைப்பு libcrun_seccomp_gen_ctx_s *ctx)
{
  எனில் (ctx->container == வெற்று || ctx->container->container_def == வெற்று
      || ctx->container->container_def->linux == வெற்று)
    திரும்பு false;

  திரும்பு ctx->container->container_def->linux->seccomp != வெற்று
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != வெற்று;
}

நிலையான முழு
find_in_cache (கட்டமைப்பு libcrun_seccomp_gen_ctx_s *ctx, முழு dirfd, மாறிலி எழுத்து *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free எழுத்து *cache_file_path = வெற்று;
  முழு ret;

  எனில் (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    திரும்பு 0;

  seccomp = get_seccomp_configuration (ctx);
  எனில் (seccomp == வெற்று)
    திரும்பு 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = TEMP_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  எனில் (UNLIKELY (ret < 0 && errno != ENOENT))
    திரும்பு crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  திரும்பு 0;
}

முழு
libcrun_generate_seccomp (கட்டமைப்பு libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  முழு ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = வெற்று;
  முழு default_errno_மதிப்பு = EPERM;
  uint32_t action, default_action;
  மாறிலி எழுத்து *def_action = வெற்று;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  எனில் (gen_ctx->from_cache)
    திரும்பு 0;

  எனில் (gen_ctx->container == வெற்று || gen_ctx->container->container_def == வெற்று || gen_ctx->container->container_def->linux == வெற்று)
    திரும்பு 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  எனில் (seccomp == வெற்று)
    திரும்பு 0;

  /* seccomp not available.  */
  எனில் (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    திரும்பு crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  எனில் (def_action == வெற்று)
    திரும்பு crun_make_error (err, 0, "seccomp misses the default action");

  எனில் (seccomp->default_errno_ret_present)
    {
      எனில் (! seccomp_action_supports_errno (def_action))
        திரும்பு crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_மதிப்பு = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_மதிப்பு, &default_action, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ctx = seccomp_init (default_action);
  எனில் (ctx == வெற்று)
    திரும்பு crun_make_error (err, 0, "error seccomp_init");

  க்கு (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      மாறிலி எழுத்து *arch = seccomp->architectures[i];
      எழுத்து *end, lowercase_arch[32] = {
        0,
      };

      எனில் (arch == வெற்று)
        திரும்பு crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      எனில் (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, அளவு (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      எனில் (arch_token == 0)
        திரும்பு crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  இல்லையெனில்
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      எனில் (ret < 0 && ret != -EEXIST)
        திரும்பு crun_make_error (err, -ret, "seccomp adding architecture");
    }

  க்கு (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool multiple_args = false;
      முழு errno_ret = EPERM;

      எனில் (seccomp->syscalls[i]->errno_ret_present)
        {
          எனில் (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            திரும்பு crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      எனில் (action == default_action)
        தொடர்;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      எனில் (seccomp->syscalls[i]->args != வெற்று)
        {
          uint32_t மொத்தம்[6] = {};
          size_t k;

          க்கு (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t index = seccomp->syscalls[i]->args[k]->index;

              எனில் (index >= 6)
                திரும்பு crun_make_error (err, 0, "invalid seccomp index `%u`", index);

              மொத்தம்[index]++;
              எனில் (மொத்தம்[index] > 1)
                multiple_args = true;
            }
        }

      க்கு (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          முழு syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          எனில் (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              எனில் (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                திரும்பு crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              தொடர்;
            }

          எனில் (seccomp->syscalls[i]->args == வெற்று)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
            }
          இல்லையெனில்
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              எனில் (multiple_args)
                {
                  size_t r;

                  க்கு (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      கட்டமைப்பு scmp_arg_cmp arg_cmp;
                      எழுத்து *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->index;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      எனில் (UNLIKELY (ret < 0))
                        திரும்பு ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->மதிப்பு;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->மதிப்பு_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      எனில் (UNLIKELY (ret < 0))
                        திரும்பு crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              இல்லையெனில்
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  மாறிலி size_t args_len = seccomp->syscalls[i]->args_len;
                  கட்டமைப்பு scmp_arg_cmp arg_cmp[6];

                  க்கு (k = 0; k < args_len; k++)
                    {
                      எழுத்து *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->index;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      எனில் (UNLIKELY (ret < 0))
                        திரும்பு ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->மதிப்பு;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->மதிப்பு_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : வெற்று);
                  எனில் (UNLIKELY (ret < 0))
                    திரும்பு crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  எனில் (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      திரும்பு store_seccomp_cache (gen_ctx, err);
    }

  திரும்பு 0;
#இல்லையெனில்
  திரும்பு 0;
#endif
}

முழு
libcrun_copy_seccomp (கட்டமைப்பு libcrun_seccomp_gen_ctx_s *gen_ctx, மாறிலி எழுத்து *b64_bpf, libcrun_error_t *err)
{
  cleanup_free எழுத்து *bpf_data = வெற்று;
  size_t size = 0;
  size_t in_size;
  முழு consumed;
  முழு ret;

  in_size = strlen (b64_bpf);
  bpf_data = xmalloc (in_size + 1);

  consumed = base64_decode (b64_bpf, in_size, bpf_data, in_size, &size);
  எனில் (UNLIKELY (consumed != (முழு) in_size))
    திரும்பு crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_data, size, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

முழு
libcrun_open_seccomp_bpf (கட்டமைப்பு libcrun_seccomp_gen_ctx_s *ctx, முழு *fd, libcrun_error_t *err)
{
  cleanup_close முழு dirfd = -1;
  cleanup_free எழுத்து *dest_path = வெற்று;
  libcrun_container_t *container = ctx->container;
  முழு ret;

  ctx->fd = *fd = -1;

  எனில் (container == வெற்று || container->context == வெற்று)
    திரும்பு crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  எனில் (UNLIKELY (dirfd < 0))
    திரும்பு dirfd;

  எனில் (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      எனில் (created)
        {
          ctx->from_cache = true;
          செல் open_existing_file;
        }

      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  இல்லையெனில்
    {
    open_existing_file:
      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      எனில் (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          எனில் (errno == ENOENT && ! has_seccomp_configuration (ctx))
            திரும்பு 0;
          திரும்பு crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  திரும்பு 0;
}
