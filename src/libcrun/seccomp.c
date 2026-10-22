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

#अन्तर्भाव <config.h>
#अन्तर्भाव "seccomp.h"
#अन्तर्भाव "linux.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/mount.h>
#अन्तर्भाव <sys/syscall.h>
#अन्तर्भाव <sys/prctl.h>
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <sys/resource.h>
#अन्तर्भाव <sys/sysmacros.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/utsname.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/mman.h>

#ifdef HAVE_BLAKE3
#  include <blake3.h>
#अञ्ञथा
#  include "blake3/blake3.h"
#endif

#सचे HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int चञ्चल पूर्णाङ्क
#  endif
#endif

#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#अन्तर्भाव <linux/seccomp.h>
#अन्तर्भाव <linux/filter.h>
#अन्तर्भाव <sys/prctl.h>
#अन्तर्भाव <sys/syscall.h>

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
स्थिर अन्तःस्थ off_t
get_cache_dir_inode_max_size (off_t size_rundir)
{
  पटिनिवत्त size_rundir * 3 + MIN_CACHE_DIR_INODE_SIZE;
}

स्थिर पूर्णाङ्क
syscall_seccomp (unsigned पूर्णाङ्क operation, unsigned पूर्णाङ्क flags, शून्य *args)
{
  पटिनिवत्त (पूर्णाङ्क) syscall (__NR_seccomp, operation, flags, args);
}

#ifdef HAVE_SECCOMP
स्थिर पूर्णाङ्क
get_seccomp_operator (ध्रुव वर्ण *name, गणना scmp_compare *op, libcrun_error_t *err)
{
  स्थिर ध्रुव संरचना
  {
    ध्रुव वर्ण *name;
    गणना scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  ध्रुव वर्ण *p = name;
  size_t i;

  सचे (! has_prefix (p, "SCMP_CMP_"))
    गच्छ fail;

  p += परिमाण ("SCMP_CMP_") - 1;

  अत्थाय (i = 0; i < परिमाण (operators) / परिमाण (operators[0]); i++)
    सचे (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        पटिनिवत्त 0;
      }

fail:
  पटिनिवत्त crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

स्थिर पूर्णाङ्क
get_seccomp_action (ध्रुव वर्ण *name, पूर्णाङ्क errno_ret, uint32_t *action, libcrun_error_t *err)
{
  स्थिर ध्रुव संरचना
  {
    ध्रुव वर्ण *name;
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
  ध्रुव वर्ण *p = name;
  size_t i;

  सचे (! has_prefix (p, "SCMP_ACT_"))
    गच्छ fail;

  p += परिमाण ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  सचे (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      पटिनिवत्त 0;
    }
  सचे (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      पटिनिवत्त 0;
    }

  अत्थाय (i = 0; i < परिमाण (actions) / परिमाण (actions[0]); i++)
    सचे (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        पटिनिवत्त 0;
      }

fail:
  पटिनिवत्त crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
स्थिर पूर्णाङ्क
export_bpf (scmp_filter_ctx ctx, पूर्णाङ्क fd, libcrun_error_t *err)
{
  cleanup_free वर्ण *bpf = रिक्त;
  size_t len = 0;
  पूर्णाङ्क ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, रिक्त, &len);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  अञ्ञथा
  cleanup_close पूर्णाङ्क pipe_r = -1;
  cleanup_close पूर्णाङ्क pipe_w = -1;
  पूर्णाङ्क p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  सचे (UNLIKELY (len >= (size_t) ret))
    पटिनिवत्त crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  पटिनिवत्त safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#endif /* HAVE_SECCOMP */

स्थिर शून्य
make_lowercase (वर्ण *str)
{
  याव (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

स्थिर शून्य
cleanup_seccompp (शून्य *p)
{
#ifdef HAVE_SECCOMP
  scmp_filter_ctx *ctx = (शून्य **) p;
  सचे (*ctx)
    seccomp_release (*ctx);
#endif
}
#define cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

पूर्णाङ्क
libcrun_apply_seccomp (पूर्णाङ्क infd, पूर्णाङ्क listener_receiver_fd, ध्रुव वर्ण *receiver_fd_payload,
                       size_t receiver_fd_payload_len, वर्ण **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  cleanup_mmap संरचना libcrun_mmap_s *mmap_region = रिक्त;
  cleanup_close पूर्णाङ्क listener_fd = -1;
  cleanup_pid पूर्णाङ्क helper_proc = -1;
  संरचना sock_fprog seccomp_filter;
  cleanup_free वर्ण *bpf = रिक्त;
  unsigned पूर्णाङ्क flags = 0;
  size_t len;
  पूर्णाङ्क ret;

  सचे (infd < 0)
    पटिनिवत्त 0;

  सचे (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    पटिनिवत्त crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  सचे (seccomp_flags == रिक्त)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  अञ्ञथा
    {
      size_t i = 0;
      अत्थाय (i = 0; i < seccomp_flags_len; i++)
        {
          सचे (seccomp_flags[i] == रिक्त)
            पटिनिवत्त crun_make_error (err, EINVAL, "seccomp flag is not specified");

          सचे (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          अञ्ञथा सचे (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          अञ्ञथा सचे (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          अञ्ञथा सचे (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          अञ्ञथा
            पटिनिवत्त crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (संरचना sock_filter *) bpf;

  सचे (listener_receiver_fd >= 0)
    {
      cleanup_close पूर्णाङ्क memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  अञ्ञथा
      पटिनिवत्त crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      सचे (UNLIKELY (memfd < 0))
        पटिनिवत्त crun_make_error (err, errno, "memfd_create");
#  अञ्ञथा
      पटिनिवत्त crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, परिमाण (atomic_int));
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, रिक्त, परिमाण (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, रिक्त);
      सचे (UNLIKELY (helper_proc < 0))
        पटिनिवत्त crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      सचे (helper_proc == 0)
        {
          पूर्णाङ्क fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          अत्थाय (;;)
            {
              fd = *fd_received;
              सचे (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  सचे (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  अनुवर्त;
                }
#  सचे ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              भङ्ग;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          सचे (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  सचे (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      सचे (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  सचे (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      पूर्णाङ्क status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      सचे (ret != 0)
        पटिनिवत्त crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  पटिनिवत्त 0;
#अञ्ञथा
  पटिनिवत्त 0;
#endif
}

स्थिर bool
seccomp_action_supports_errno (ध्रुव वर्ण *action)
{
  पटिनिवत्त strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
स्थिर पूर्णाङ्क
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, unsigned पूर्णाङ्क seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  unsigned वर्ण hash[32];
  संरचना utsname utsbuf;
  size_t i;
  पूर्णाङ्क ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#define PROCESS_STRING(X)                                           \
  करोहि                                                                \
    {                                                               \
      ध्रुव वर्ण *str_ = (X);                                       \
      unsigned वर्ण present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? strlen (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, परिमाण (present_)); \
      blake3_hasher_update (&hasher, &len_, परिमाण (len_));         \
      सचे (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } याव (0)
#define PROCESS_DATA(X)                                   \
  करोहि                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), परिमाण ((X))); \
  } याव (0)
#define PROCESS_OPTIONAL_DATA(PRESENT, X)                           \
  करोहि                                                                \
    {                                                               \
      unsigned वर्ण present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, परिमाण (present_)); \
      सचे (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), परिमाण ((X)));         \
  } याव (0)
#define PROCESS_गणनम्(X)                                        \
  करोहि                                                            \
    {                                                           \
      size_t गणनम्_2 = (X);                                      \
      blake3_hasher_update (&hasher, &गणनम्_2, परिमाण (गणनम्_2)); \
  } याव (0)

  PROCESS_STRING (PACKAGE_VERSION);

#ifdef HAVE_SECCOMP
  {
    ध्रुव संरचना scmp_version *version = seccomp_version ();

    PROCESS_DATA (version->major);
    PROCESS_DATA (version->minor);
    PROCESS_DATA (version->micro);
  }
#endif

  memset (&utsbuf, 0, परिमाण (utsbuf));
  ret = uname (&utsbuf);
  सचे (UNLIKELY (ret != 0))
    पटिनिवत्त crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_DATA (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_DATA (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_गणनम् (seccomp->flags_len);
  अत्थाय (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_गणनम् (seccomp->architectures_len);
  अत्थाय (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_गणनम् (seccomp->syscalls_len);
  अत्थाय (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_DATA (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_गणनम् (seccomp->syscalls[i]->names_len);
      अत्थाय (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_गणनम् (seccomp->syscalls[i]->args_len);
      अत्थाय (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->index);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->मानम्);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->मानम्_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, परिमाण (hash));

  अत्थाय (i = 0; i < 32; i++)
    {
      size_t remaining = परिमाण (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      सचे (UNLIKELY (ret != 2))
        पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#undef PROCESS_STRING
#undef PROCESS_DATA
#undef PROCESS_OPTIONAL_DATA
#undef PROCESS_गणनम्
  पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क
open_rundir_dirfd (ध्रुव वर्ण *state_मूलम्, libcrun_error_t *err)
{
  cleanup_free वर्ण *dir = रिक्त;
  पूर्णाङ्क dirfd;
  पूर्णाङ्क ret;

  ret = libcrun_get_state_directory (&dir, state_मूलम्, रिक्त, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  dirfd = TEMP_FAILURE_RETRY (open (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  सचे (UNLIKELY (dirfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", dir);

  पटिनिवत्त dirfd;
}

संरचना cache_entry
{
  seccomp_checksum_t checksum;
  संरचना timespec atime;
};

स्थिर पूर्णाङ्क
compare_entries_by_atime (ध्रुव शून्य *a, ध्रुव शून्य *b)
{
  ध्रुव संरचना cache_entry *entry_a = a;
  ध्रुव संरचना cache_entry *entry_b = b;
  पूर्णाङ्क ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  सचे (ret)
    पटिनिवत्त ret;

  पटिनिवत्त entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

स्थिर पूर्णाङ्क
evict_cache (पूर्णाङ्क मूलम्_dfd, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क cache_dir_fd = -1;
  off_t cache_dir_inode_max_size;
  संरचना stat st;
  पूर्णाङ्क ret;

  ret = TEMP_FAILURE_RETRY (fstat (मूलम्_dfd, &st));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_size = get_cache_dir_inode_max_size (st.st_size);

  cache_dir_fd = TEMP_FAILURE_RETRY (openat (मूलम्_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  सचे (UNLIKELY (cache_dir_fd < 0))
    {
      सचे (errno == ENOENT)
        पटिनिवत्त 0;
      पटिनिवत्त crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = TEMP_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  सचे (ret == 0 && st.st_size > cache_dir_inode_max_size)
    {
      cleanup_free संरचना cache_entry *entries = रिक्त;
      cleanup_dir DIR *d = रिक्त;
      size_t i, n_entries = 0;
      संरचना dirent *de;
      पूर्णाङ्क dfd;

      d = fdopendir (cache_dir_fd);
      सचे (d == रिक्त)
        पटिनिवत्त crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      याव ((de = readdir (d)))
        {
          सचे (de->d_name[0] == '.')
            अनुवर्त;

          ret = TEMP_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          सचे (UNLIKELY (ret < 0))
            अनुवर्त;

          /* The cache file is already used, it is pointless to free it.  */
          सचे (st.st_nlink > 1)
            अनुवर्त;

          /* If the sticky bit is set, then ignore it.  */
          सचे (st.st_mode & S_ISVTX)
            अनुवर्त;

          सचे (strlen (de->d_name) != परिमाण (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              अनुवर्त;
            }
          अञ्ञथा
            {
              entries = xrealloc (entries, परिमाण (संरचना cache_entry) * (n_entries + 1));

              memcpy (entries[n_entries].checksum, de->d_name, परिमाण (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, परिमाण (संरचना cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      अत्थाय (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  पटिनिवत्त 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
स्थिर पूर्णाङ्क
open_seccomp_bpf_dirfd (libcrun_container_t *container, वर्ण **rel_path, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  पूर्णाङ्क dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_मूलम् : रिक्त, err);
  सचे (UNLIKELY (dirfd < 0))
    पटिनिवत्त dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", रिक्त);
  सचे (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (close (dirfd));
      पटिनिवत्त ret;
    }

  पटिनिवत्त dirfd;
}

स्थिर पूर्णाङ्क
store_seccomp_cache (संरचना libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free वर्ण *src_path = रिक्त;
  cleanup_free वर्ण *dest_path = रिक्त;
  cleanup_close पूर्णाङ्क dirfd = -1;
  पूर्णाङ्क ret;

  सचे (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    पटिनिवत्त 0;

  सचे (is_empty_string (ctx->checksum))
    पटिनिवत्त 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  सचे (UNLIKELY (dirfd < 0))
    पटिनिवत्त dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = evict_cache (dirfd, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  सचे (UNLIKELY (ret < 0 && errno != EEXIST))
    पटिनिवत्त crun_make_error (err, errno, "link `%s` to `%s`", src_path, dest_path);
  पटिनिवत्त 0;
}

स्थिर अन्तःस्थ runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (संरचना libcrun_seccomp_gen_ctx_s *ctx)
{
  सचे (ctx->container == रिक्त || ctx->container->container_def == रिक्त
      || ctx->container->container_def->linux == रिक्त)
    पटिनिवत्त 0;

  पटिनिवत्त ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
स्थिर अन्तःस्थ bool
has_seccomp_configuration (संरचना libcrun_seccomp_gen_ctx_s *ctx)
{
  सचे (ctx->container == रिक्त || ctx->container->container_def == रिक्त
      || ctx->container->container_def->linux == रिक्त)
    पटिनिवत्त false;

  पटिनिवत्त ctx->container->container_def->linux->seccomp != रिक्त
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != रिक्त;
}

स्थिर पूर्णाङ्क
find_in_cache (संरचना libcrun_seccomp_gen_ctx_s *ctx, पूर्णाङ्क dirfd, ध्रुव वर्ण *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free वर्ण *cache_file_path = रिक्त;
  पूर्णाङ्क ret;

  सचे (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    पटिनिवत्त 0;

  seccomp = get_seccomp_configuration (ctx);
  सचे (seccomp == रिक्त)
    पटिनिवत्त 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = TEMP_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  सचे (UNLIKELY (ret < 0 && errno != ENOENT))
    पटिनिवत्त crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_generate_seccomp (संरचना libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  पूर्णाङ्क ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = रिक्त;
  पूर्णाङ्क default_errno_मानम् = EPERM;
  uint32_t action, default_action;
  ध्रुव वर्ण *def_action = रिक्त;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  सचे (gen_ctx->from_cache)
    पटिनिवत्त 0;

  सचे (gen_ctx->container == रिक्त || gen_ctx->container->container_def == रिक्त || gen_ctx->container->container_def->linux == रिक्त)
    पटिनिवत्त 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  सचे (seccomp == रिक्त)
    पटिनिवत्त 0;

  /* seccomp not available.  */
  सचे (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    पटिनिवत्त crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  सचे (def_action == रिक्त)
    पटिनिवत्त crun_make_error (err, 0, "seccomp misses the default action");

  सचे (seccomp->default_errno_ret_present)
    {
      सचे (! seccomp_action_supports_errno (def_action))
        पटिनिवत्त crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_मानम् = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_मानम्, &default_action, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ctx = seccomp_init (default_action);
  सचे (ctx == रिक्त)
    पटिनिवत्त crun_make_error (err, 0, "error seccomp_init");

  अत्थाय (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      ध्रुव वर्ण *arch = seccomp->architectures[i];
      वर्ण *end, lowercase_arch[32] = {
        0,
      };

      सचे (arch == रिक्त)
        पटिनिवत्त crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      सचे (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, परिमाण (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      सचे (arch_token == 0)
        पटिनिवत्त crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  अञ्ञथा
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      सचे (ret < 0 && ret != -EEXIST)
        पटिनिवत्त crun_make_error (err, -ret, "seccomp adding architecture");
    }

  अत्थाय (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool multiple_args = false;
      पूर्णाङ्क errno_ret = EPERM;

      सचे (seccomp->syscalls[i]->errno_ret_present)
        {
          सचे (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            पटिनिवत्त crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      सचे (action == default_action)
        अनुवर्त;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      सचे (seccomp->syscalls[i]->args != रिक्त)
        {
          uint32_t गणनम्[6] = {};
          size_t k;

          अत्थाय (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t index = seccomp->syscalls[i]->args[k]->index;

              सचे (index >= 6)
                पटिनिवत्त crun_make_error (err, 0, "invalid seccomp index `%u`", index);

              गणनम्[index]++;
              सचे (गणनम्[index] > 1)
                multiple_args = true;
            }
        }

      अत्थाय (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          पूर्णाङ्क syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          सचे (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              सचे (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                पटिनिवत्त crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              अनुवर्त;
            }

          सचे (seccomp->syscalls[i]->args == रिक्त)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
            }
          अञ्ञथा
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              सचे (multiple_args)
                {
                  size_t r;

                  अत्थाय (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      संरचना scmp_arg_cmp arg_cmp;
                      वर्ण *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->index;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      सचे (UNLIKELY (ret < 0))
                        पटिनिवत्त ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->मानम्;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->मानम्_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      सचे (UNLIKELY (ret < 0))
                        पटिनिवत्त crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              अञ्ञथा
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  ध्रुव size_t args_len = seccomp->syscalls[i]->args_len;
                  संरचना scmp_arg_cmp arg_cmp[6];

                  अत्थाय (k = 0; k < args_len; k++)
                    {
                      वर्ण *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->index;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      सचे (UNLIKELY (ret < 0))
                        पटिनिवत्त ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->मानम्;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->मानम्_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : रिक्त);
                  सचे (UNLIKELY (ret < 0))
                    पटिनिवत्त crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  सचे (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      पटिनिवत्त store_seccomp_cache (gen_ctx, err);
    }

  पटिनिवत्त 0;
#अञ्ञथा
  पटिनिवत्त 0;
#endif
}

पूर्णाङ्क
libcrun_copy_seccomp (संरचना libcrun_seccomp_gen_ctx_s *gen_ctx, ध्रुव वर्ण *b64_bpf, libcrun_error_t *err)
{
  cleanup_free वर्ण *bpf_data = रिक्त;
  size_t size = 0;
  size_t in_size;
  पूर्णाङ्क consumed;
  पूर्णाङ्क ret;

  in_size = strlen (b64_bpf);
  bpf_data = xmalloc (in_size + 1);

  consumed = base64_decode (b64_bpf, in_size, bpf_data, in_size, &size);
  सचे (UNLIKELY (consumed != (पूर्णाङ्क) in_size))
    पटिनिवत्त crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_data, size, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_open_seccomp_bpf (संरचना libcrun_seccomp_gen_ctx_s *ctx, पूर्णाङ्क *fd, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क dirfd = -1;
  cleanup_free वर्ण *dest_path = रिक्त;
  libcrun_container_t *container = ctx->container;
  पूर्णाङ्क ret;

  ctx->fd = *fd = -1;

  सचे (container == रिक्त || container->context == रिक्त)
    पटिनिवत्त crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  सचे (UNLIKELY (dirfd < 0))
    पटिनिवत्त dirfd;

  सचे (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      सचे (created)
        {
          ctx->from_cache = true;
          गच्छ open_existing_file;
        }

      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  अञ्ञथा
    {
    open_existing_file:
      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      सचे (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          सचे (errno == ENOENT && ! has_seccomp_configuration (ctx))
            पटिनिवत्त 0;
          पटिनिवत्त crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  पटिनिवत्त 0;
}
