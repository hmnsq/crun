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
#अन्यथा
#  include "blake3/blake3.h"
#endif

#जर HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int अस्थिर पूर्णांक
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
स्थिर ओळीत off_t
get_cache_dir_inode_max_size (off_t size_rundir)
{
  परत size_rundir * 3 + MIN_CACHE_DIR_INODE_SIZE;
}

स्थिर पूर्णांक
syscall_seccomp (अचिन्हित पूर्णांक operation, अचिन्हित पूर्णांक flags, शून्य *args)
{
  परत (पूर्णांक) syscall (__NR_seccomp, operation, flags, args);
}

#ifdef HAVE_SECCOMP
स्थिर पूर्णांक
get_seccomp_operator (स्थिरांक अक्षर *name, गणना scmp_compare *op, libcrun_error_t *err)
{
  स्थिर स्थिरांक संरचना
  {
    स्थिरांक अक्षर *name;
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
  स्थिरांक अक्षर *p = name;
  माप_प्रकार i;

  जर (! has_prefix (p, "SCMP_CMP_"))
    जा fail;

  p += आकार ("SCMP_CMP_") - 1;

  साठी (i = 0; i < आकार (operators) / आकार (operators[0]); i++)
    जर (म_भेद (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        परत 0;
      }

fail:
  परत crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

स्थिर पूर्णांक
get_seccomp_action (स्थिरांक अक्षर *name, पूर्णांक errno_ret, uint32_t *action, libcrun_error_t *err)
{
  स्थिर स्थिरांक संरचना
  {
    स्थिरांक अक्षर *name;
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
  स्थिरांक अक्षर *p = name;
  माप_प्रकार i;

  जर (! has_prefix (p, "SCMP_ACT_"))
    जा fail;

  p += आकार ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  जर (म_भेद (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      परत 0;
    }
  जर (म_भेद (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      परत 0;
    }

  साठी (i = 0; i < आकार (actions) / आकार (actions[0]); i++)
    जर (म_भेद (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        परत 0;
      }

fail:
  परत crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
स्थिर पूर्णांक
export_bpf (scmp_filter_ctx ctx, पूर्णांक fd, libcrun_error_t *err)
{
  cleanup_free अक्षर *bpf = रिक्त;
  माप_प्रकार len = 0;
  पूर्णांक ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, रिक्त, &len);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  अन्यथा
  cleanup_close पूर्णांक pipe_r = -1;
  cleanup_close पूर्णांक pipe_w = -1;
  पूर्णांक p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "fcntl (F_GETPIPE_SZ)");
  जर (UNLIKELY (len >= (माप_प्रकार) ret))
    परत crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  परत safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#endif /* HAVE_SECCOMP */

स्थिर शून्य
make_lowercase (अक्षर *str)
{
  पर्यंत (*str)
    {
      *str = छोटे (*str);
      str++;
    }
}

स्थिर शून्य
cleanup_seccompp (शून्य *p)
{
#ifdef HAVE_SECCOMP
  scmp_filter_ctx *ctx = (शून्य **) p;
  जर (*ctx)
    seccomp_release (*ctx);
#endif
}
#define cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

पूर्णांक
libcrun_apply_seccomp (पूर्णांक infd, पूर्णांक listener_receiver_fd, स्थिरांक अक्षर *receiver_fd_payload,
                       माप_प्रकार receiver_fd_payload_len, अक्षर **seccomp_flags, माप_प्रकार seccomp_flags_len,
                       libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  cleanup_mmap संरचना libcrun_mmap_s *mmap_region = रिक्त;
  cleanup_close पूर्णांक listener_fd = -1;
  cleanup_pid पूर्णांक helper_proc = -1;
  संरचना sock_fprog seccomp_filter;
  cleanup_free अक्षर *bpf = रिक्त;
  अचिन्हित पूर्णांक flags = 0;
  माप_प्रकार len;
  पूर्णांक ret;

  जर (infd < 0)
    परत 0;

  जर (UNLIKELY (lseek (infd, 0, शुरू_से) == (off_t) -1))
    परत crun_make_error (err, त्रुटि_सं, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  जर (seccomp_flags == रिक्त)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  अन्यथा
    {
      माप_प्रकार i = 0;
      साठी (i = 0; i < seccomp_flags_len; i++)
        {
          जर (seccomp_flags[i] == रिक्त)
            परत crun_make_error (err, EINVAL, "seccomp flag is not specified");

          जर (म_भेद (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          अन्यथा जर (म_भेद (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          अन्यथा जर (म_भेद (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          अन्यथा जर (म_भेद (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          अन्यथा
            परत crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (संरचना sock_filter *) bpf;

  जर (listener_receiver_fd >= 0)
    {
      cleanup_close पूर्णांक memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  अन्यथा
      परत crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      जर (UNLIKELY (memfd < 0))
        परत crun_make_error (err, त्रुटि_सं, "memfd_create");
#  अन्यथा
      परत crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, आकार (atomic_int));
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, रिक्त, आकार (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, रिक्त);
      जर (UNLIKELY (helper_proc < 0))
        परत crun_make_error (err, त्रुटि_सं, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      जर (helper_proc == 0)
        {
          पूर्णांक fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          साठी (;;)
            {
              fd = *fd_received;
              जर (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  जर (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  सुरूठेवा;
                }
#  जर ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              थांबा;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          जर (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  जर (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      जर (त्रुटि_सं == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  जर (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      पूर्णांक status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      जर (ret != 0)
        परत crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  परत 0;
#अन्यथा
  परत 0;
#endif
}

स्थिर bool
seccomp_action_supports_errno (स्थिरांक अक्षर *action)
{
  परत म_भेद (action, "SCMP_ACT_ERRNO") == 0
         || म_भेद (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
स्थिर पूर्णांक
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, अचिन्हित पूर्णांक seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  अचिन्हित अक्षर hash[32];
  संरचना utsname utsbuf;
  माप_प्रकार i;
  पूर्णांक ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#define PROCESS_STRING(X)                                           \
  करा                                                                \
    {                                                               \
      स्थिरांक अक्षर *str_ = (X);                                       \
      अचिन्हित अक्षर present_ = str_ ? 1 : 0;                        \
      माप_प्रकार len_ = str_ ? म_माप (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, आकार (present_)); \
      blake3_hasher_update (&hasher, &len_, आकार (len_));         \
      जर (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } पर्यंत (0)
#define PROCESS_DATA(X)                                   \
  करा                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), आकार ((X))); \
  } पर्यंत (0)
#define PROCESS_OPTIONAL_DATA(PRESENT, X)                           \
  करा                                                                \
    {                                                               \
      अचिन्हित अक्षर present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, आकार (present_)); \
      जर (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), आकार ((X)));         \
  } पर्यंत (0)
#define PROCESS_मोजणी(X)                                        \
  करा                                                            \
    {                                                           \
      माप_प्रकार मोजणी_2 = (X);                                      \
      blake3_hasher_update (&hasher, &मोजणी_2, आकार (मोजणी_2)); \
  } पर्यंत (0)

  PROCESS_STRING (PACKAGE_VERSION);

#ifdef HAVE_SECCOMP
  {
    स्थिरांक संरचना scmp_version *version = seccomp_version ();

    PROCESS_DATA (version->major);
    PROCESS_DATA (version->minor);
    PROCESS_DATA (version->micro);
  }
#endif

  स_रखो (&utsbuf, 0, आकार (utsbuf));
  ret = uname (&utsbuf);
  जर (UNLIKELY (ret != 0))
    परत crun_make_error (err, त्रुटि_सं, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_DATA (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_DATA (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_मोजणी (seccomp->flags_len);
  साठी (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_मोजणी (seccomp->architectures_len);
  साठी (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_मोजणी (seccomp->syscalls_len);
  साठी (i = 0; i < seccomp->syscalls_len; i++)
    {
      माप_प्रकार j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_DATA (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_मोजणी (seccomp->syscalls[i]->names_len);
      साठी (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_मोजणी (seccomp->syscalls[i]->args_len);
      साठी (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->index);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->मूल्य);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->मूल्य_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, आकार (hash));

  साठी (i = 0; i < 32; i++)
    {
      माप_प्रकार remaining = आकार (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      जर (UNLIKELY (ret != 2))
        परत crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#undef PROCESS_STRING
#undef PROCESS_DATA
#undef PROCESS_OPTIONAL_DATA
#undef PROCESS_मोजणी
  परत 1;
}

स्थिर पूर्णांक
open_rundir_dirfd (स्थिरांक अक्षर *state_मूळ, libcrun_error_t *err)
{
  cleanup_free अक्षर *dir = रिक्त;
  पूर्णांक dirfd;
  पूर्णांक ret;

  ret = libcrun_get_state_directory (&dir, state_मूळ, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  dirfd = TEMP_FAILURE_RETRY (खोलो (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  जर (UNLIKELY (dirfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", dir);

  परत dirfd;
}

संरचना cache_entry
{
  seccomp_checksum_t checksum;
  संरचना timespec atime;
};

स्थिर पूर्णांक
compare_entries_by_atime (स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
  स्थिरांक संरचना cache_entry *entry_a = a;
  स्थिरांक संरचना cache_entry *entry_b = b;
  पूर्णांक ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  जर (ret)
    परत ret;

  परत entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

स्थिर पूर्णांक
evict_cache (पूर्णांक मूळ_dfd, libcrun_error_t *err)
{
  cleanup_close पूर्णांक cache_dir_fd = -1;
  off_t cache_dir_inode_max_size;
  संरचना stat st;
  पूर्णांक ret;

  ret = TEMP_FAILURE_RETRY (ख_स्थिति (मूळ_dfd, &st));
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "fstat rundir");

  cache_dir_inode_max_size = get_cache_dir_inode_max_size (st.st_size);

  cache_dir_fd = TEMP_FAILURE_RETRY (openat (मूळ_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  जर (UNLIKELY (cache_dir_fd < 0))
    {
      जर (त्रुटि_सं == ENOENT)
        परत 0;
      परत crun_make_error (err, त्रुटि_सं, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = TEMP_FAILURE_RETRY (ख_स्थिति (cache_dir_fd, &st));
  जर (ret == 0 && st.st_size > cache_dir_inode_max_size)
    {
      cleanup_free संरचना cache_entry *entries = रिक्त;
      cleanup_dir सूची *d = रिक्त;
      माप_प्रकार i, n_entries = 0;
      संरचना dirent *de;
      पूर्णांक dfd;

      d = fdopendir (cache_dir_fd);
      जर (d == रिक्त)
        परत crun_make_error (err, त्रुटि_सं, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      पर्यंत ((de = सूची_पढ़ो (d)))
        {
          जर (de->d_name[0] == '.')
            सुरूठेवा;

          ret = TEMP_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          जर (UNLIKELY (ret < 0))
            सुरूठेवा;

          /* The cache file is already used, it is pointless to free it.  */
          जर (st.st_nlink > 1)
            सुरूठेवा;

          /* If the sticky bit is set, then ignore it.  */
          जर (st.st_mode & S_ISVTX)
            सुरूठेवा;

          जर (म_माप (de->d_name) != आकार (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              सुरूठेवा;
            }
          अन्यथा
            {
              entries = xrealloc (entries, आकार (संरचना cache_entry) * (n_entries + 1));

              स_नकल (entries[n_entries].checksum, de->d_name, आकार (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      क्विक (entries, n_entries, आकार (संरचना cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      साठी (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  परत 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
स्थिर पूर्णांक
open_seccomp_bpf_dirfd (libcrun_container_t *container, अक्षर **rel_path, libcrun_error_t *err)
{
  पूर्णांक ret;
  पूर्णांक dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_मूळ : रिक्त, err);
  जर (UNLIKELY (dirfd < 0))
    परत dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", रिक्त);
  जर (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (बंद (dirfd));
      परत ret;
    }

  परत dirfd;
}

स्थिर पूर्णांक
store_seccomp_cache (संरचना libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free अक्षर *src_path = रिक्त;
  cleanup_free अक्षर *dest_path = रिक्त;
  cleanup_close पूर्णांक dirfd = -1;
  पूर्णांक ret;

  जर (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    परत 0;

  जर (is_empty_string (ctx->checksum))
    परत 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  जर (UNLIKELY (dirfd < 0))
    परत dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = evict_cache (dirfd, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  जर (UNLIKELY (ret < 0 && त्रुटि_सं != EEXIST))
    परत crun_make_error (err, त्रुटि_सं, "link `%s` to `%s`", src_path, dest_path);
  परत 0;
}

स्थिर ओळीत runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (संरचना libcrun_seccomp_gen_ctx_s *ctx)
{
  जर (ctx->container == रिक्त || ctx->container->container_def == रिक्त
      || ctx->container->container_def->linux == रिक्त)
    परत 0;

  परत ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
स्थिर ओळीत bool
has_seccomp_configuration (संरचना libcrun_seccomp_gen_ctx_s *ctx)
{
  जर (ctx->container == रिक्त || ctx->container->container_def == रिक्त
      || ctx->container->container_def->linux == रिक्त)
    परत false;

  परत ctx->container->container_def->linux->seccomp != रिक्त
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != रिक्त;
}

स्थिर पूर्णांक
find_in_cache (संरचना libcrun_seccomp_gen_ctx_s *ctx, पूर्णांक dirfd, स्थिरांक अक्षर *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free अक्षर *cache_file_path = रिक्त;
  पूर्णांक ret;

  जर (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    परत 0;

  seccomp = get_seccomp_configuration (ctx);
  जर (seccomp == रिक्त)
    परत 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = TEMP_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  जर (UNLIKELY (ret < 0 && त्रुटि_सं != ENOENT))
    परत crun_make_error (err, त्रुटि_सं, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  परत 0;
}

पूर्णांक
libcrun_generate_seccomp (संरचना libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  पूर्णांक ret;
  माप_प्रकार i;
  cleanup_seccomp scmp_filter_ctx ctx = रिक्त;
  पूर्णांक default_errno_मूल्य = EPERM;
  uint32_t action, default_action;
  स्थिरांक अक्षर *def_action = रिक्त;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  जर (gen_ctx->from_cache)
    परत 0;

  जर (gen_ctx->container == रिक्त || gen_ctx->container->container_def == रिक्त || gen_ctx->container->container_def->linux == रिक्त)
    परत 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  जर (seccomp == रिक्त)
    परत 0;

  /* seccomp not available.  */
  जर (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    परत crun_make_error (err, त्रुटि_सं, "prctl");

  def_action = seccomp->default_action;
  जर (def_action == रिक्त)
    परत crun_make_error (err, 0, "seccomp misses the default action");

  जर (seccomp->default_errno_ret_present)
    {
      जर (! seccomp_action_supports_errno (def_action))
        परत crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_मूल्य = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_मूल्य, &default_action, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ctx = seccomp_init (default_action);
  जर (ctx == रिक्त)
    परत crun_make_error (err, 0, "error seccomp_init");

  साठी (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      स्थिरांक अक्षर *arch = seccomp->architectures[i];
      अक्षर *end, lowercase_arch[32] = {
        0,
      };

      जर (arch == रिक्त)
        परत crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      जर (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, आकार (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      जर (arch_token == 0)
        परत crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  अन्यथा
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      जर (ret < 0 && ret != -EEXIST)
        परत crun_make_error (err, -ret, "seccomp adding architecture");
    }

  साठी (i = 0; i < seccomp->syscalls_len; i++)
    {
      माप_प्रकार j;
      bool multiple_args = false;
      पूर्णांक errno_ret = EPERM;

      जर (seccomp->syscalls[i]->errno_ret_present)
        {
          जर (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            परत crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      जर (action == default_action)
        सुरूठेवा;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      जर (seccomp->syscalls[i]->args != रिक्त)
        {
          uint32_t मोजणी[6] = {};
          माप_प्रकार k;

          साठी (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t index = seccomp->syscalls[i]->args[k]->index;

              जर (index >= 6)
                परत crun_make_error (err, 0, "invalid seccomp index `%u`", index);

              मोजणी[index]++;
              जर (मोजणी[index] > 1)
                multiple_args = true;
            }
        }

      साठी (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          पूर्णांक syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          जर (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              जर (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                परत crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              सुरूठेवा;
            }

          जर (seccomp->syscalls[i]->args == रिक्त)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              जर (UNLIKELY (ret < 0))
                परत crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
            }
          अन्यथा
            {
              माप_प्रकार k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              जर (multiple_args)
                {
                  माप_प्रकार r;

                  साठी (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      संरचना scmp_arg_cmp arg_cmp;
                      अक्षर *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->index;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      जर (UNLIKELY (ret < 0))
                        परत ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->मूल्य;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->मूल्य_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      जर (UNLIKELY (ret < 0))
                        परत crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              अन्यथा
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  स्थिरांक माप_प्रकार args_len = seccomp->syscalls[i]->args_len;
                  संरचना scmp_arg_cmp arg_cmp[6];

                  साठी (k = 0; k < args_len; k++)
                    {
                      अक्षर *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->index;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      जर (UNLIKELY (ret < 0))
                        परत ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->मूल्य;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->मूल्य_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : रिक्त);
                  जर (UNLIKELY (ret < 0))
                    परत crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  जर (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      परत store_seccomp_cache (gen_ctx, err);
    }

  परत 0;
#अन्यथा
  परत 0;
#endif
}

पूर्णांक
libcrun_copy_seccomp (संरचना libcrun_seccomp_gen_ctx_s *gen_ctx, स्थिरांक अक्षर *b64_bpf, libcrun_error_t *err)
{
  cleanup_free अक्षर *bpf_data = रिक्त;
  माप_प्रकार size = 0;
  माप_प्रकार in_size;
  पूर्णांक consumed;
  पूर्णांक ret;

  in_size = म_माप (b64_bpf);
  bpf_data = xmalloc (in_size + 1);

  consumed = base64_decode (b64_bpf, in_size, bpf_data, in_size, &size);
  जर (UNLIKELY (consumed != (पूर्णांक) in_size))
    परत crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_data, size, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत 0;
}

पूर्णांक
libcrun_open_seccomp_bpf (संरचना libcrun_seccomp_gen_ctx_s *ctx, पूर्णांक *fd, libcrun_error_t *err)
{
  cleanup_close पूर्णांक dirfd = -1;
  cleanup_free अक्षर *dest_path = रिक्त;
  libcrun_container_t *container = ctx->container;
  पूर्णांक ret;

  ctx->fd = *fd = -1;

  जर (container == रिक्त || container->context == रिक्त)
    परत crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  जर (UNLIKELY (dirfd < 0))
    परत dirfd;

  जर (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      जर (created)
        {
          ctx->from_cache = true;
          जा open_existing_file;
        }

      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  अन्यथा
    {
    open_existing_file:
      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      जर (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          जर (त्रुटि_सं == ENOENT && ! has_seccomp_configuration (ctx))
            परत 0;
          परत crun_make_error (err, त्रुटि_सं, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  परत 0;
}
