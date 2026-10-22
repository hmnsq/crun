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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "seccomp.h"
#અન્તર્ભાવ "linux.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sched.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <sys/mount.h>
#અન્તર્ભાવ <sys/syscall.h>
#અન્તર્ભાવ <sys/prctl.h>
#અન્તર્ભાવ <sys/time.h>
#અન્તર્ભાવ <sys/resource.h>
#અન્તર્ભાવ <sys/sysmacros.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/utsname.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/mman.h>

#ifdef HAVE_BLAKE3
#  include <blake3.h>
#નહીંતર
#  include "blake3/blake3.h"
#endif

#જો HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int અસ્થિર પૂર્ણાંક
#  endif
#endif

#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#અન્તર્ભાવ <linux/seccomp.h>
#અન્તર્ભાવ <linux/filter.h>
#અન્તર્ભાવ <sys/prctl.h>
#અન્તર્ભાવ <sys/syscall.h>

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
સ્થિર અંતઃરેખ off_t
get_cache_dir_inode_max_size (off_t size_rundir)
{
  પાછા size_rundir * 3 + MIN_CACHE_DIR_INODE_SIZE;
}

સ્થિર પૂર્ણાંક
syscall_seccomp (unsigned પૂર્ણાંક operation, unsigned પૂર્ણાંક flags, શૂન્ય *args)
{
  પાછા (પૂર્ણાંક) syscall (__NR_seccomp, operation, flags, args);
}

#ifdef HAVE_SECCOMP
સ્થિર પૂર્ણાંક
get_seccomp_operator (અચલ અક્ષર *name, ગણના scmp_compare *op, libcrun_error_t *err)
{
  સ્થિર અચલ સંરચના
  {
    અચલ અક્ષર *name;
    ગણના scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  અચલ અક્ષર *p = name;
  size_t i;

  જો (! has_prefix (p, "SCMP_CMP_"))
    જાઓ fail;

  p += કદ ("SCMP_CMP_") - 1;

  માટે (i = 0; i < કદ (operators) / કદ (operators[0]); i++)
    જો (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        પાછા 0;
      }

fail:
  પાછા crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

સ્થિર પૂર્ણાંક
get_seccomp_action (અચલ અક્ષર *name, પૂર્ણાંક errno_ret, uint32_t *action, libcrun_error_t *err)
{
  સ્થિર અચલ સંરચના
  {
    અચલ અક્ષર *name;
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
  અચલ અક્ષર *p = name;
  size_t i;

  જો (! has_prefix (p, "SCMP_ACT_"))
    જાઓ fail;

  p += કદ ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  જો (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      પાછા 0;
    }
  જો (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      પાછા 0;
    }

  માટે (i = 0; i < કદ (actions) / કદ (actions[0]); i++)
    જો (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        પાછા 0;
      }

fail:
  પાછા crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
સ્થિર પૂર્ણાંક
export_bpf (scmp_filter_ctx ctx, પૂર્ણાંક fd, libcrun_error_t *err)
{
  cleanup_free અક્ષર *bpf = ખાલી;
  size_t len = 0;
  પૂર્ણાંક ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, ખાલી, &len);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  નહીંતર
  cleanup_close પૂર્ણાંક pipe_r = -1;
  cleanup_close પૂર્ણાંક pipe_w = -1;
  પૂર્ણાંક p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  જો (UNLIKELY (len >= (size_t) ret))
    પાછા crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  પાછા safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#endif /* HAVE_SECCOMP */

સ્થિર શૂન્ય
make_lowercase (અક્ષર *str)
{
  જ્યારે (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

સ્થિર શૂન્ય
cleanup_seccompp (શૂન્ય *p)
{
#ifdef HAVE_SECCOMP
  scmp_filter_ctx *ctx = (શૂન્ય **) p;
  જો (*ctx)
    seccomp_release (*ctx);
#endif
}
#define cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

પૂર્ણાંક
libcrun_apply_seccomp (પૂર્ણાંક infd, પૂર્ણાંક listener_receiver_fd, અચલ અક્ષર *receiver_fd_payload,
                       size_t receiver_fd_payload_len, અક્ષર **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  cleanup_mmap સંરચના libcrun_mmap_s *mmap_region = ખાલી;
  cleanup_close પૂર્ણાંક listener_fd = -1;
  cleanup_pid પૂર્ણાંક helper_proc = -1;
  સંરચના sock_fprog seccomp_filter;
  cleanup_free અક્ષર *bpf = ખાલી;
  unsigned પૂર્ણાંક flags = 0;
  size_t len;
  પૂર્ણાંક ret;

  જો (infd < 0)
    પાછા 0;

  જો (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    પાછા crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  જો (seccomp_flags == ખાલી)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  નહીંતર
    {
      size_t i = 0;
      માટે (i = 0; i < seccomp_flags_len; i++)
        {
          જો (seccomp_flags[i] == ખાલી)
            પાછા crun_make_error (err, EINVAL, "seccomp flag is not specified");

          જો (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          નહીંતર જો (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          નહીંતર જો (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          નહીંતર જો (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          નહીંતર
            પાછા crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (સંરચના sock_filter *) bpf;

  જો (listener_receiver_fd >= 0)
    {
      cleanup_close પૂર્ણાંક memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  નહીંતર
      પાછા crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      જો (UNLIKELY (memfd < 0))
        પાછા crun_make_error (err, errno, "memfd_create");
#  નહીંતર
      પાછા crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, કદ (atomic_int));
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, ખાલી, કદ (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, ખાલી);
      જો (UNLIKELY (helper_proc < 0))
        પાછા crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      જો (helper_proc == 0)
        {
          પૂર્ણાંક fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          માટે (;;)
            {
              fd = *fd_received;
              જો (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  જો (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  ચાલુ;
                }
#  જો ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              તોડો;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          જો (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  જો (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      જો (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  જો (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      પૂર્ણાંક status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      જો (ret != 0)
        પાછા crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  પાછા 0;
#નહીંતર
  પાછા 0;
#endif
}

સ્થિર bool
seccomp_action_supports_errno (અચલ અક્ષર *action)
{
  પાછા strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
સ્થિર પૂર્ણાંક
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, unsigned પૂર્ણાંક seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  unsigned અક્ષર hash[32];
  સંરચના utsname utsbuf;
  size_t i;
  પૂર્ણાંક ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#define PROCESS_STRING(X)                                           \
  કરો                                                                \
    {                                                               \
      અચલ અક્ષર *str_ = (X);                                       \
      unsigned અક્ષર present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? strlen (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, કદ (present_)); \
      blake3_hasher_update (&hasher, &len_, કદ (len_));         \
      જો (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } જ્યારે (0)
#define PROCESS_DATA(X)                                   \
  કરો                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), કદ ((X))); \
  } જ્યારે (0)
#define PROCESS_OPTIONAL_DATA(PRESENT, X)                           \
  કરો                                                                \
    {                                                               \
      unsigned અક્ષર present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, કદ (present_)); \
      જો (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), કદ ((X)));         \
  } જ્યારે (0)
#define PROCESS_ગણતરી(X)                                        \
  કરો                                                            \
    {                                                           \
      size_t ગણતરી_2 = (X);                                      \
      blake3_hasher_update (&hasher, &ગણતરી_2, કદ (ગણતરી_2)); \
  } જ્યારે (0)

  PROCESS_STRING (PACKAGE_VERSION);

#ifdef HAVE_SECCOMP
  {
    અચલ સંરચના scmp_version *version = seccomp_version ();

    PROCESS_DATA (version->major);
    PROCESS_DATA (version->minor);
    PROCESS_DATA (version->micro);
  }
#endif

  memset (&utsbuf, 0, કદ (utsbuf));
  ret = uname (&utsbuf);
  જો (UNLIKELY (ret != 0))
    પાછા crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_DATA (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_DATA (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_ગણતરી (seccomp->flags_len);
  માટે (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_ગણતરી (seccomp->architectures_len);
  માટે (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_ગણતરી (seccomp->syscalls_len);
  માટે (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_DATA (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_ગણતરી (seccomp->syscalls[i]->names_len);
      માટે (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_ગણતરી (seccomp->syscalls[i]->args_len);
      માટે (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->index);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->કિંમત);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->કિંમત_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, કદ (hash));

  માટે (i = 0; i < 32; i++)
    {
      size_t remaining = કદ (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      જો (UNLIKELY (ret != 2))
        પાછા crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#undef PROCESS_STRING
#undef PROCESS_DATA
#undef PROCESS_OPTIONAL_DATA
#undef PROCESS_ગણતરી
  પાછા 1;
}

સ્થિર પૂર્ણાંક
open_rundir_dirfd (અચલ અક્ષર *state_મૂળ, libcrun_error_t *err)
{
  cleanup_free અક્ષર *dir = ખાલી;
  પૂર્ણાંક dirfd;
  પૂર્ણાંક ret;

  ret = libcrun_get_state_directory (&dir, state_મૂળ, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  dirfd = TEMP_FAILURE_RETRY (open (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  જો (UNLIKELY (dirfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", dir);

  પાછા dirfd;
}

સંરચના cache_entry
{
  seccomp_checksum_t checksum;
  સંરચના timespec atime;
};

સ્થિર પૂર્ણાંક
compare_entries_by_atime (અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
  અચલ સંરચના cache_entry *entry_a = a;
  અચલ સંરચના cache_entry *entry_b = b;
  પૂર્ણાંક ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  જો (ret)
    પાછા ret;

  પાછા entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

સ્થિર પૂર્ણાંક
evict_cache (પૂર્ણાંક મૂળ_dfd, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક cache_dir_fd = -1;
  off_t cache_dir_inode_max_size;
  સંરચના stat st;
  પૂર્ણાંક ret;

  ret = TEMP_FAILURE_RETRY (fstat (મૂળ_dfd, &st));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_size = get_cache_dir_inode_max_size (st.st_size);

  cache_dir_fd = TEMP_FAILURE_RETRY (openat (મૂળ_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  જો (UNLIKELY (cache_dir_fd < 0))
    {
      જો (errno == ENOENT)
        પાછા 0;
      પાછા crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = TEMP_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  જો (ret == 0 && st.st_size > cache_dir_inode_max_size)
    {
      cleanup_free સંરચના cache_entry *entries = ખાલી;
      cleanup_dir DIR *d = ખાલી;
      size_t i, n_entries = 0;
      સંરચના dirent *de;
      પૂર્ણાંક dfd;

      d = fdopendir (cache_dir_fd);
      જો (d == ખાલી)
        પાછા crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      જ્યારે ((de = readdir (d)))
        {
          જો (de->d_name[0] == '.')
            ચાલુ;

          ret = TEMP_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          જો (UNLIKELY (ret < 0))
            ચાલુ;

          /* The cache file is already used, it is pointless to free it.  */
          જો (st.st_nlink > 1)
            ચાલુ;

          /* If the sticky bit is set, then ignore it.  */
          જો (st.st_mode & S_ISVTX)
            ચાલુ;

          જો (strlen (de->d_name) != કદ (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              ચાલુ;
            }
          નહીંતર
            {
              entries = xrealloc (entries, કદ (સંરચના cache_entry) * (n_entries + 1));

              memcpy (entries[n_entries].checksum, de->d_name, કદ (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, કદ (સંરચના cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      માટે (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  પાછા 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
સ્થિર પૂર્ણાંક
open_seccomp_bpf_dirfd (libcrun_container_t *container, અક્ષર **rel_path, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  પૂર્ણાંક dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_મૂળ : ખાલી, err);
  જો (UNLIKELY (dirfd < 0))
    પાછા dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", ખાલી);
  જો (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (close (dirfd));
      પાછા ret;
    }

  પાછા dirfd;
}

સ્થિર પૂર્ણાંક
store_seccomp_cache (સંરચના libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free અક્ષર *src_path = ખાલી;
  cleanup_free અક્ષર *dest_path = ખાલી;
  cleanup_close પૂર્ણાંક dirfd = -1;
  પૂર્ણાંક ret;

  જો (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    પાછા 0;

  જો (is_empty_string (ctx->checksum))
    પાછા 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  જો (UNLIKELY (dirfd < 0))
    પાછા dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = evict_cache (dirfd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  જો (UNLIKELY (ret < 0 && errno != EEXIST))
    પાછા crun_make_error (err, errno, "link `%s` to `%s`", src_path, dest_path);
  પાછા 0;
}

સ્થિર અંતઃરેખ runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (સંરચના libcrun_seccomp_gen_ctx_s *ctx)
{
  જો (ctx->container == ખાલી || ctx->container->container_def == ખાલી
      || ctx->container->container_def->linux == ખાલી)
    પાછા 0;

  પાછા ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
સ્થિર અંતઃરેખ bool
has_seccomp_configuration (સંરચના libcrun_seccomp_gen_ctx_s *ctx)
{
  જો (ctx->container == ખાલી || ctx->container->container_def == ખાલી
      || ctx->container->container_def->linux == ખાલી)
    પાછા false;

  પાછા ctx->container->container_def->linux->seccomp != ખાલી
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != ખાલી;
}

સ્થિર પૂર્ણાંક
find_in_cache (સંરચના libcrun_seccomp_gen_ctx_s *ctx, પૂર્ણાંક dirfd, અચલ અક્ષર *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free અક્ષર *cache_file_path = ખાલી;
  પૂર્ણાંક ret;

  જો (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    પાછા 0;

  seccomp = get_seccomp_configuration (ctx);
  જો (seccomp == ખાલી)
    પાછા 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = TEMP_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  જો (UNLIKELY (ret < 0 && errno != ENOENT))
    પાછા crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  પાછા 0;
}

પૂર્ણાંક
libcrun_generate_seccomp (સંરચના libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  પૂર્ણાંક ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = ખાલી;
  પૂર્ણાંક default_errno_કિંમત = EPERM;
  uint32_t action, default_action;
  અચલ અક્ષર *def_action = ખાલી;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  જો (gen_ctx->from_cache)
    પાછા 0;

  જો (gen_ctx->container == ખાલી || gen_ctx->container->container_def == ખાલી || gen_ctx->container->container_def->linux == ખાલી)
    પાછા 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  જો (seccomp == ખાલી)
    પાછા 0;

  /* seccomp not available.  */
  જો (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    પાછા crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  જો (def_action == ખાલી)
    પાછા crun_make_error (err, 0, "seccomp misses the default action");

  જો (seccomp->default_errno_ret_present)
    {
      જો (! seccomp_action_supports_errno (def_action))
        પાછા crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_કિંમત = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_કિંમત, &default_action, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ctx = seccomp_init (default_action);
  જો (ctx == ખાલી)
    પાછા crun_make_error (err, 0, "error seccomp_init");

  માટે (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      અચલ અક્ષર *arch = seccomp->architectures[i];
      અક્ષર *end, lowercase_arch[32] = {
        0,
      };

      જો (arch == ખાલી)
        પાછા crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      જો (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, કદ (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      જો (arch_token == 0)
        પાછા crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  નહીંતર
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      જો (ret < 0 && ret != -EEXIST)
        પાછા crun_make_error (err, -ret, "seccomp adding architecture");
    }

  માટે (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool multiple_args = false;
      પૂર્ણાંક errno_ret = EPERM;

      જો (seccomp->syscalls[i]->errno_ret_present)
        {
          જો (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            પાછા crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      જો (action == default_action)
        ચાલુ;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      જો (seccomp->syscalls[i]->args != ખાલી)
        {
          uint32_t ગણતરી[6] = {};
          size_t k;

          માટે (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t index = seccomp->syscalls[i]->args[k]->index;

              જો (index >= 6)
                પાછા crun_make_error (err, 0, "invalid seccomp index `%u`", index);

              ગણતરી[index]++;
              જો (ગણતરી[index] > 1)
                multiple_args = true;
            }
        }

      માટે (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          પૂર્ણાંક syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          જો (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              જો (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                પાછા crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              ચાલુ;
            }

          જો (seccomp->syscalls[i]->args == ખાલી)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              જો (UNLIKELY (ret < 0))
                પાછા crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
            }
          નહીંતર
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              જો (multiple_args)
                {
                  size_t r;

                  માટે (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      સંરચના scmp_arg_cmp arg_cmp;
                      અક્ષર *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->index;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      જો (UNLIKELY (ret < 0))
                        પાછા ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->કિંમત;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->કિંમત_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      જો (UNLIKELY (ret < 0))
                        પાછા crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              નહીંતર
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  અચલ size_t args_len = seccomp->syscalls[i]->args_len;
                  સંરચના scmp_arg_cmp arg_cmp[6];

                  માટે (k = 0; k < args_len; k++)
                    {
                      અક્ષર *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->index;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      જો (UNLIKELY (ret < 0))
                        પાછા ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->કિંમત;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->કિંમત_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : ખાલી);
                  જો (UNLIKELY (ret < 0))
                    પાછા crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  જો (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      પાછા store_seccomp_cache (gen_ctx, err);
    }

  પાછા 0;
#નહીંતર
  પાછા 0;
#endif
}

પૂર્ણાંક
libcrun_copy_seccomp (સંરચના libcrun_seccomp_gen_ctx_s *gen_ctx, અચલ અક્ષર *b64_bpf, libcrun_error_t *err)
{
  cleanup_free અક્ષર *bpf_data = ખાલી;
  size_t size = 0;
  size_t in_size;
  પૂર્ણાંક consumed;
  પૂર્ણાંક ret;

  in_size = strlen (b64_bpf);
  bpf_data = xmalloc (in_size + 1);

  consumed = base64_decode (b64_bpf, in_size, bpf_data, in_size, &size);
  જો (UNLIKELY (consumed != (પૂર્ણાંક) in_size))
    પાછા crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_data, size, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

પૂર્ણાંક
libcrun_open_seccomp_bpf (સંરચના libcrun_seccomp_gen_ctx_s *ctx, પૂર્ણાંક *fd, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક dirfd = -1;
  cleanup_free અક્ષર *dest_path = ખાલી;
  libcrun_container_t *container = ctx->container;
  પૂર્ણાંક ret;

  ctx->fd = *fd = -1;

  જો (container == ખાલી || container->context == ખાલી)
    પાછા crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  જો (UNLIKELY (dirfd < 0))
    પાછા dirfd;

  જો (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      જો (created)
        {
          ctx->from_cache = true;
          જાઓ open_existing_file;
        }

      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  નહીંતર
    {
    open_existing_file:
      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      જો (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          જો (errno == ENOENT && ! has_seccomp_configuration (ctx))
            પાછા 0;
          પાછા crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  પાછા 0;
}
