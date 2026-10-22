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
#ᱵᱟᱝ
#  include "blake3/blake3.h"
#endif

#ᱡᱩᱫᱤ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int ca_ncala puur_naa_nka
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
static anta_hstha off_t
get_cache_dir_inode_max_size (off_t size_rundir)
{
  ᱨᱩᱣᱟᱹᱲ size_rundir * 3 + MIN_CACHE_DIR_INODE_SIZE;
}

static puur_naa_nka
syscall_seccomp (unsigned puur_naa_nka operation, unsigned puur_naa_nka flags, void *args)
{
  ᱨᱩᱣᱟᱹᱲ (puur_naa_nka) syscall (__NR_seccomp, operation, flags, args);
}

#ifdef HAVE_SECCOMP
static puur_naa_nka
get_seccomp_operator (const var_na *name, ga_nanaa scmp_compare *op, libcrun_error_t *err)
{
  static const sa_nracanaa
  {
    const var_na *name;
    ga_nanaa scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  const var_na *p = name;
  size_t i;

  ᱡᱩᱫᱤ (! has_prefix (p, "SCMP_CMP_"))
    goto fail;

  p += parimaa_na ("SCMP_CMP_") - 1;

  for (i = 0; i < parimaa_na (operators) / parimaa_na (operators[0]); i++)
    ᱡᱩᱫᱤ (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        ᱨᱩᱣᱟᱹᱲ 0;
      }

fail:
  ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

static puur_naa_nka
get_seccomp_action (const var_na *name, puur_naa_nka errno_ret, uint32_t *action, libcrun_error_t *err)
{
  static const sa_nracanaa
  {
    const var_na *name;
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
  const var_na *p = name;
  size_t i;

  ᱡᱩᱫᱤ (! has_prefix (p, "SCMP_ACT_"))
    goto fail;

  p += parimaa_na ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  ᱡᱩᱫᱤ (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      ᱨᱩᱣᱟᱹᱲ 0;
    }
  ᱡᱩᱫᱤ (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      ᱨᱩᱣᱟᱹᱲ 0;
    }

  for (i = 0; i < parimaa_na (actions) / parimaa_na (actions[0]); i++)
    ᱡᱩᱫᱤ (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        ᱨᱩᱣᱟᱹᱲ 0;
      }

fail:
  ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
static puur_naa_nka
export_bpf (scmp_filter_ctx ctx, puur_naa_nka fd, libcrun_error_t *err)
{
  cleanup_free var_na *bpf = NULL;
  size_t len = 0;
  puur_naa_nka ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, NULL, &len);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  ᱵᱟᱝ
  cleanup_close puur_naa_nka pipe_r = -1;
  cleanup_close puur_naa_nka pipe_w = -1;
  puur_naa_nka p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  ᱡᱩᱫᱤ (UNLIKELY (len >= (size_t) ret))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  ᱨᱩᱣᱟᱹᱲ safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#endif /* HAVE_SECCOMP */

static void
make_lowercase (var_na *str)
{
  ᱡᱚᱠᱷᱚᱱ (*str)
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
  ᱡᱩᱫᱤ (*ctx)
    seccomp_release (*ctx);
#endif
}
#define cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

puur_naa_nka
libcrun_apply_seccomp (puur_naa_nka infd, puur_naa_nka listener_receiver_fd, const var_na *receiver_fd_payload,
                       size_t receiver_fd_payload_len, var_na **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  cleanup_mmap sa_nracanaa libcrun_mmap_s *mmap_region = NULL;
  cleanup_close puur_naa_nka listener_fd = -1;
  cleanup_pid puur_naa_nka helper_proc = -1;
  sa_nracanaa sock_fprog seccomp_filter;
  cleanup_free var_na *bpf = NULL;
  unsigned puur_naa_nka flags = 0;
  size_t len;
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (infd < 0)
    ᱨᱩᱣᱟᱹᱲ 0;

  ᱡᱩᱫᱤ (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  ᱡᱩᱫᱤ (seccomp_flags == NULL)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  ᱵᱟᱝ
    {
      size_t i = 0;
      for (i = 0; i < seccomp_flags_len; i++)
        {
          ᱡᱩᱫᱤ (seccomp_flags[i] == NULL)
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, EINVAL, "seccomp flag is not specified");

          ᱡᱩᱫᱤ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          ᱵᱟᱝ ᱡᱩᱫᱤ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          ᱵᱟᱝ ᱡᱩᱫᱤ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          ᱵᱟᱝ ᱡᱩᱫᱤ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          ᱵᱟᱝ
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (sa_nracanaa sock_filter *) bpf;

  ᱡᱩᱫᱤ (listener_receiver_fd >= 0)
    {
      cleanup_close puur_naa_nka memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  ᱵᱟᱝ
      ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      ᱡᱩᱫᱤ (UNLIKELY (memfd < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "memfd_create");
#  ᱵᱟᱝ
      ᱨᱩᱣᱟᱹᱲ crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, parimaa_na (atomic_int));
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, NULL, parimaa_na (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, NULL);
      ᱡᱩᱫᱤ (UNLIKELY (helper_proc < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      ᱡᱩᱫᱤ (helper_proc == 0)
        {
          puur_naa_nka fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          for (;;)
            {
              fd = *fd_received;
              ᱡᱩᱫᱤ (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  ᱡᱩᱫᱤ (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  continue;
                }
#  ᱡᱩᱫᱤ ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              bha_nga;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      ᱡᱩᱫᱤ (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  ᱡᱩᱫᱤ (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      puur_naa_nka status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      ᱡᱩᱫᱤ (ret != 0)
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  ᱨᱩᱣᱟᱹᱲ 0;
#ᱵᱟᱝ
  ᱨᱩᱣᱟᱹᱲ 0;
#endif
}

static bool
seccomp_action_supports_errno (const var_na *action)
{
  ᱨᱩᱣᱟᱹᱲ strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
static puur_naa_nka
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, unsigned puur_naa_nka seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  unsigned var_na hash[32];
  sa_nracanaa utsname utsbuf;
  size_t i;
  puur_naa_nka ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#define PROCESS_STRING(X)                                           \
  do                                                                \
    {                                                               \
      const var_na *str_ = (X);                                       \
      unsigned var_na present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? strlen (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, parimaa_na (present_)); \
      blake3_hasher_update (&hasher, &len_, parimaa_na (len_));         \
      ᱡᱩᱫᱤ (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } ᱡᱚᱠᱷᱚᱱ (0)
#define PROCESS_DATA(X)                                   \
  do                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), parimaa_na ((X))); \
  } ᱡᱚᱠᱷᱚᱱ (0)
#define PROCESS_OPTIONAL_DATA(PRESENT, X)                           \
  do                                                                \
    {                                                               \
      unsigned var_na present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, parimaa_na (present_)); \
      ᱡᱩᱫᱤ (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), parimaa_na ((X)));         \
  } ᱡᱚᱠᱷᱚᱱ (0)
#define PROCESS_COUNT(X)                                        \
  do                                                            \
    {                                                           \
      size_t count_ = (X);                                      \
      blake3_hasher_update (&hasher, &count_, parimaa_na (count_)); \
  } ᱡᱚᱠᱷᱚᱱ (0)

  PROCESS_STRING (PACKAGE_VERSION);

#ifdef HAVE_SECCOMP
  {
    const sa_nracanaa scmp_version *version = seccomp_version ();

    PROCESS_DATA (version->major);
    PROCESS_DATA (version->minor);
    PROCESS_DATA (version->micro);
  }
#endif

  memset (&utsbuf, 0, parimaa_na (utsbuf));
  ret = uname (&utsbuf);
  ᱡᱩᱫᱤ (UNLIKELY (ret != 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_DATA (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_DATA (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_COUNT (seccomp->flags_len);
  for (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_COUNT (seccomp->architectures_len);
  for (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_COUNT (seccomp->syscalls_len);
  for (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_DATA (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_COUNT (seccomp->syscalls[i]->names_len);
      for (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_COUNT (seccomp->syscalls[i]->args_len);
      for (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->index);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->value);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->value_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, parimaa_na (hash));

  for (i = 0; i < 32; i++)
    {
      size_t remaining = parimaa_na (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      ᱡᱩᱫᱤ (UNLIKELY (ret != 2))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#undef PROCESS_STRING
#undef PROCESS_DATA
#undef PROCESS_OPTIONAL_DATA
#undef PROCESS_COUNT
  ᱨᱩᱣᱟᱹᱲ 1;
}

static puur_naa_nka
open_rundir_dirfd (const var_na *state_root, libcrun_error_t *err)
{
  cleanup_free var_na *dir = NULL;
  puur_naa_nka dirfd;
  puur_naa_nka ret;

  ret = libcrun_get_state_directory (&dir, state_root, NULL, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  dirfd = TEMP_FAILURE_RETRY (open (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  ᱡᱩᱫᱤ (UNLIKELY (dirfd < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "open `%s`", dir);

  ᱨᱩᱣᱟᱹᱲ dirfd;
}

sa_nracanaa cache_entry
{
  seccomp_checksum_t checksum;
  sa_nracanaa timespec atime;
};

static puur_naa_nka
compare_entries_by_atime (const void *a, const void *b)
{
  const sa_nracanaa cache_entry *entry_a = a;
  const sa_nracanaa cache_entry *entry_b = b;
  puur_naa_nka ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  ᱡᱩᱫᱤ (ret)
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱨᱩᱣᱟᱹᱲ entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

static puur_naa_nka
evict_cache (puur_naa_nka root_dfd, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka cache_dir_fd = -1;
  off_t cache_dir_inode_max_size;
  sa_nracanaa stat st;
  puur_naa_nka ret;

  ret = TEMP_FAILURE_RETRY (fstat (root_dfd, &st));
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_size = get_cache_dir_inode_max_size (st.st_size);

  cache_dir_fd = TEMP_FAILURE_RETRY (openat (root_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  ᱡᱩᱫᱤ (UNLIKELY (cache_dir_fd < 0))
    {
      ᱡᱩᱫᱤ (errno == ENOENT)
        ᱨᱩᱣᱟᱹᱲ 0;
      ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = TEMP_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  ᱡᱩᱫᱤ (ret == 0 && st.st_size > cache_dir_inode_max_size)
    {
      cleanup_free sa_nracanaa cache_entry *entries = NULL;
      cleanup_dir DIR *d = NULL;
      size_t i, n_entries = 0;
      sa_nracanaa dirent *de;
      puur_naa_nka dfd;

      d = fdopendir (cache_dir_fd);
      ᱡᱩᱫᱤ (d == NULL)
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      ᱡᱚᱠᱷᱚᱱ ((de = readdir (d)))
        {
          ᱡᱩᱫᱤ (de->d_name[0] == '.')
            continue;

          ret = TEMP_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            continue;

          /* The cache file is already used, it is pointless to free it.  */
          ᱡᱩᱫᱤ (st.st_nlink > 1)
            continue;

          /* If the sticky bit is set, then ignore it.  */
          ᱡᱩᱫᱤ (st.st_mode & S_ISVTX)
            continue;

          ᱡᱩᱫᱤ (strlen (de->d_name) != parimaa_na (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              continue;
            }
          ᱵᱟᱝ
            {
              entries = xrealloc (entries, parimaa_na (sa_nracanaa cache_entry) * (n_entries + 1));

              memcpy (entries[n_entries].checksum, de->d_name, parimaa_na (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, parimaa_na (sa_nracanaa cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      for (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
static puur_naa_nka
open_seccomp_bpf_dirfd (libcrun_container_t *container, var_na **rel_path, libcrun_error_t *err)
{
  puur_naa_nka ret;
  puur_naa_nka dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_root : NULL, err);
  ᱡᱩᱫᱤ (UNLIKELY (dirfd < 0))
    ᱨᱩᱣᱟᱹᱲ dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", NULL);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (close (dirfd));
      ᱨᱩᱣᱟᱹᱲ ret;
    }

  ᱨᱩᱣᱟᱹᱲ dirfd;
}

static puur_naa_nka
store_seccomp_cache (sa_nracanaa libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free var_na *src_path = NULL;
  cleanup_free var_na *dest_path = NULL;
  cleanup_close puur_naa_nka dirfd = -1;
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    ᱨᱩᱣᱟᱹᱲ 0;

  ᱡᱩᱫᱤ (is_empty_string (ctx->checksum))
    ᱨᱩᱣᱟᱹᱲ 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  ᱡᱩᱫᱤ (UNLIKELY (dirfd < 0))
    ᱨᱩᱣᱟᱹᱲ dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, NULL);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = evict_cache (dirfd, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0 && errno != EEXIST))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "link `%s` to `%s`", src_path, dest_path);
  ᱨᱩᱣᱟᱹᱲ 0;
}

static anta_hstha runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (sa_nracanaa libcrun_seccomp_gen_ctx_s *ctx)
{
  ᱡᱩᱫᱤ (ctx->container == NULL || ctx->container->container_def == NULL
      || ctx->container->container_def->linux == NULL)
    ᱨᱩᱣᱟᱹᱲ 0;

  ᱨᱩᱣᱟᱹᱲ ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
static anta_hstha bool
has_seccomp_configuration (sa_nracanaa libcrun_seccomp_gen_ctx_s *ctx)
{
  ᱡᱩᱫᱤ (ctx->container == NULL || ctx->container->container_def == NULL
      || ctx->container->container_def->linux == NULL)
    ᱨᱩᱣᱟᱹᱲ false;

  ᱨᱩᱣᱟᱹᱲ ctx->container->container_def->linux->seccomp != NULL
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != NULL;
}

static puur_naa_nka
find_in_cache (sa_nracanaa libcrun_seccomp_gen_ctx_s *ctx, puur_naa_nka dirfd, const var_na *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free var_na *cache_file_path = NULL;
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    ᱨᱩᱣᱟᱹᱲ 0;

  seccomp = get_seccomp_configuration (ctx);
  ᱡᱩᱫᱤ (seccomp == NULL)
    ᱨᱩᱣᱟᱹᱲ 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, NULL);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = TEMP_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0 && errno != ENOENT))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka
libcrun_generate_seccomp (sa_nracanaa libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  puur_naa_nka ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = NULL;
  puur_naa_nka default_errno_value = EPERM;
  uint32_t action, default_action;
  const var_na *def_action = NULL;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  ᱡᱩᱫᱤ (gen_ctx->from_cache)
    ᱨᱩᱣᱟᱹᱲ 0;

  ᱡᱩᱫᱤ (gen_ctx->container == NULL || gen_ctx->container->container_def == NULL || gen_ctx->container->container_def->linux == NULL)
    ᱨᱩᱣᱟᱹᱲ 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  ᱡᱩᱫᱤ (seccomp == NULL)
    ᱨᱩᱣᱟᱹᱲ 0;

  /* seccomp not available.  */
  ᱡᱩᱫᱤ (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  ᱡᱩᱫᱤ (def_action == NULL)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "seccomp misses the default action");

  ᱡᱩᱫᱤ (seccomp->default_errno_ret_present)
    {
      ᱡᱩᱫᱤ (! seccomp_action_supports_errno (def_action))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_value = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_value, &default_action, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ctx = seccomp_init (default_action);
  ᱡᱩᱫᱤ (ctx == NULL)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "error seccomp_init");

  for (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      const var_na *arch = seccomp->architectures[i];
      var_na *end, lowercase_arch[32] = {
        0,
      };

      ᱡᱩᱫᱤ (arch == NULL)
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      ᱡᱩᱫᱤ (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, parimaa_na (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      ᱡᱩᱫᱤ (arch_token == 0)
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  ᱵᱟᱝ
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      ᱡᱩᱫᱤ (ret < 0 && ret != -EEXIST)
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, -ret, "seccomp adding architecture");
    }

  for (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool multiple_args = false;
      puur_naa_nka errno_ret = EPERM;

      ᱡᱩᱫᱤ (seccomp->syscalls[i]->errno_ret_present)
        {
          ᱡᱩᱫᱤ (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ᱡᱩᱫᱤ (action == default_action)
        continue;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      ᱡᱩᱫᱤ (seccomp->syscalls[i]->args != NULL)
        {
          uint32_t count[6] = {};
          size_t k;

          for (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t index = seccomp->syscalls[i]->args[k]->index;

              ᱡᱩᱫᱤ (index >= 6)
                ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "invalid seccomp index `%u`", index);

              count[index]++;
              ᱡᱩᱫᱤ (count[index] > 1)
                multiple_args = true;
            }
        }

      for (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          puur_naa_nka syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          ᱡᱩᱫᱤ (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              ᱡᱩᱫᱤ (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              continue;
            }

          ᱡᱩᱫᱤ (seccomp->syscalls[i]->args == NULL)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                ᱨᱩᱣᱟᱹᱲ crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
            }
          ᱵᱟᱝ
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              ᱡᱩᱫᱤ (multiple_args)
                {
                  size_t r;

                  for (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      sa_nracanaa scmp_arg_cmp arg_cmp;
                      var_na *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->index;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                        ᱨᱩᱣᱟᱹᱲ ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->value;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->value_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              ᱵᱟᱝ
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  const size_t args_len = seccomp->syscalls[i]->args_len;
                  sa_nracanaa scmp_arg_cmp arg_cmp[6];

                  for (k = 0; k < args_len; k++)
                    {
                      var_na *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->index;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                        ᱨᱩᱣᱟᱹᱲ ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->value;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->value_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : NULL);
                  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  ᱡᱩᱫᱤ (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ᱨᱩᱣᱟᱹᱲ store_seccomp_cache (gen_ctx, err);
    }

  ᱨᱩᱣᱟᱹᱲ 0;
#ᱵᱟᱝ
  ᱨᱩᱣᱟᱹᱲ 0;
#endif
}

puur_naa_nka
libcrun_copy_seccomp (sa_nracanaa libcrun_seccomp_gen_ctx_s *gen_ctx, const var_na *b64_bpf, libcrun_error_t *err)
{
  cleanup_free var_na *bpf_data = NULL;
  size_t size = 0;
  size_t in_size;
  puur_naa_nka consumed;
  puur_naa_nka ret;

  in_size = strlen (b64_bpf);
  bpf_data = xmalloc (in_size + 1);

  consumed = base64_decode (b64_bpf, in_size, bpf_data, in_size, &size);
  ᱡᱩᱫᱤ (UNLIKELY (consumed != (puur_naa_nka) in_size))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_data, size, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka
libcrun_open_seccomp_bpf (sa_nracanaa libcrun_seccomp_gen_ctx_s *ctx, puur_naa_nka *fd, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka dirfd = -1;
  cleanup_free var_na *dest_path = NULL;
  libcrun_container_t *container = ctx->container;
  puur_naa_nka ret;

  ctx->fd = *fd = -1;

  ᱡᱩᱫᱤ (container == NULL || container->context == NULL)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  ᱡᱩᱫᱤ (UNLIKELY (dirfd < 0))
    ᱨᱩᱣᱟᱹᱲ dirfd;

  ᱡᱩᱫᱤ (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ᱡᱩᱫᱤ (created)
        {
          ctx->from_cache = true;
          goto open_existing_file;
        }

      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  ᱵᱟᱝ
    {
    open_existing_file:
      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          ᱡᱩᱫᱤ (errno == ENOENT && ! has_seccomp_configuration (ctx))
            ᱨᱩᱣᱟᱹᱲ 0;
          ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}
