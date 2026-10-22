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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "seccomp.h"
#समावेश "linux.h"
#समावेश "utils.h"
#समावेश <string.h>
#समावेश <sched.h>
#समावेश <fcntl.h>
#समावेश <unistd.h>
#समावेश <sys/mount.h>
#समावेश <sys/syscall.h>
#समावेश <sys/prctl.h>
#समावेश <sys/time.h>
#समावेश <sys/resource.h>
#समावेश <sys/sysmacros.h>
#समावेश <sys/types.h>
#समावेश <sys/utsname.h>
#समावेश <sys/stat.h>
#समावेश <sys/mman.h>

#अगर_घोषित HAVE_BLAKE3
#  include <blake3.h>
#अन्यथा
#  include "blake3/blake3.h"
#पूर्ण_अगर

#अगर HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int अस्थिर पूर्णांक
#  endif
#पूर्ण_अगर

#अगर_घोषित HAVE_SECCOMP
#  include <seccomp.h>
#पूर्ण_अगर
#समावेश <linux/seccomp.h>
#समावेश <linux/filter.h>
#समावेश <sys/prctl.h>
#समावेश <sys/syscall.h>

#अगर_अघोषित __NR_seccomp
#  define __NR_seccomp 0xffff // seccomp syscall number unknown for this architecture
#पूर्ण_अगर

#अगर_अघोषित SECCOMP_SET_MODE_STRICT
#  define SECCOMP_SET_MODE_STRICT 0
#पूर्ण_अगर

#अगर_अघोषित SECCOMP_SET_MODE_FILTER
#  define SECCOMP_SET_MODE_FILTER 1
#पूर्ण_अगर

#अगर_अघोषित SECCOMP_FILTER_FLAG_TSYNC
#  define SECCOMP_FILTER_FLAG_TSYNC (1UL << 0)
#पूर्ण_अगर

#अगर_अघोषित SECCOMP_FILTER_FLAG_LOG
#  define SECCOMP_FILTER_FLAG_LOG (1UL << 1)
#पूर्ण_अगर

#अगर_अघोषित SECCOMP_FILTER_FLAG_SPEC_ALLOW
#  define SECCOMP_FILTER_FLAG_SPEC_ALLOW (1UL << 2)
#पूर्ण_अगर

#अगर_अघोषित SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV
#  define SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV (1UL << 5)
#पूर्ण_अगर

#घोषणा SECCOMP_CACHE_DIR ".cache/seccomp"

/* On Linux 5.19.15 x86_64, tmpfs reports 20 bytes + 20 bytes for each entry, so allow
   at least 32 entries.  */
#घोषणा MIN_CACHE_DIR_INODE_आकार (20 + 32 * 20)

/* Heuristic to avoid the cache directory grows indefinitely.  The inode size for
   the directory is proportional to the number of entries it contains and it is
   cheaper to compute than iterating the cache directory.  */
अटल अंतरभूत off_t
get_cache_dir_inode_max_आकार (off_t आकार_rundir)
{
  वापस आकार_rundir * 3 + MIN_CACHE_DIR_INODE_आकार;
}

अटल पूर्णांक
syscall_seccomp (अचिन्हित पूर्णांक operation, अचिन्हित पूर्णांक flags, व्योम *args)
{
  वापस (पूर्णांक) syscall (__NR_seccomp, operation, flags, args);
}

#अगर_घोषित HAVE_SECCOMP
अटल पूर्णांक
get_seccomp_operator (स्थिर अक्षर *name, क्रमागत scmp_compare *op, libcrun_error_t *err)
{
  अटल स्थिर काष्ठा
  {
    स्थिर अक्षर *name;
    क्रमागत scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  स्थिर अक्षर *p = name;
  माप_प्रकार i;

  अगर (! has_prefix (p, "SCMP_CMP_"))
    जाओ fail;

  p += माप ("SCMP_CMP_") - 1;

  क्रम (i = 0; i < माप (operators) / माप (operators[0]); i++)
    अगर (म_भेद (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        वापस 0;
      }

fail:
  वापस crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

अटल पूर्णांक
get_seccomp_action (स्थिर अक्षर *name, पूर्णांक errno_ret, uint32_t *action, libcrun_error_t *err)
{
  अटल स्थिर काष्ठा
  {
    स्थिर अक्षर *name;
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
  स्थिर अक्षर *p = name;
  माप_प्रकार i;

  अगर (! has_prefix (p, "SCMP_ACT_"))
    जाओ fail;

  p += माप ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  अगर (म_भेद (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      वापस 0;
    }
  अगर (म_भेद (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      वापस 0;
    }

  क्रम (i = 0; i < माप (actions) / माप (actions[0]); i++)
    अगर (म_भेद (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        वापस 0;
      }

fail:
  वापस crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
अटल पूर्णांक
export_bpf (scmp_filter_ctx ctx, पूर्णांक fd, libcrun_error_t *err)
{
  cleanup_free अक्षर *bpf = शून्य;
  माप_प्रकार len = 0;
  पूर्णांक ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, शून्य, &len);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  अन्यथा
  cleanup_close पूर्णांक pipe_r = -1;
  cleanup_close पूर्णांक pipe_w = -1;
  पूर्णांक p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fcntl (F_GETPIPE_SZ)");
  अगर (UNLIKELY (len >= (माप_प्रकार) ret))
    वापस crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  वापस safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#पूर्ण_अगर /* HAVE_SECCOMP */

अटल व्योम
make_lowercase (अक्षर *str)
{
  जबतक (*str)
    {
      *str = छोटे (*str);
      str++;
    }
}

अटल व्योम
cleanup_seccompp (व्योम *p)
{
#अगर_घोषित HAVE_SECCOMP
  scmp_filter_ctx *ctx = (व्योम **) p;
  अगर (*ctx)
    seccomp_release (*ctx);
#पूर्ण_अगर
}
#घोषणा cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

पूर्णांक
libcrun_apply_seccomp (पूर्णांक infd, पूर्णांक listener_receiver_fd, स्थिर अक्षर *receiver_fd_payload,
                       माप_प्रकार receiver_fd_payload_len, अक्षर **seccomp_flags, माप_प्रकार seccomp_flags_len,
                       libcrun_error_t *err)
{
#अगर_घोषित HAVE_SECCOMP
  cleanup_mmap काष्ठा libcrun_mmap_s *mmap_region = शून्य;
  cleanup_close पूर्णांक listener_fd = -1;
  cleanup_pid पूर्णांक helper_proc = -1;
  काष्ठा sock_fprog seccomp_filter;
  cleanup_free अक्षर *bpf = शून्य;
  अचिन्हित पूर्णांक flags = 0;
  माप_प्रकार len;
  पूर्णांक ret;

  अगर (infd < 0)
    वापस 0;

  अगर (UNLIKELY (lseek (infd, 0, शुरू_से) == (off_t) -1))
    वापस crun_make_error (err, त्रुटि_सं, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  अगर (seccomp_flags == शून्य)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  अन्यथा
    {
      माप_प्रकार i = 0;
      क्रम (i = 0; i < seccomp_flags_len; i++)
        {
          अगर (seccomp_flags[i] == शून्य)
            वापस crun_make_error (err, EINVAL, "seccomp flag is not specified");

          अगर (म_भेद (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          अन्यथा अगर (म_भेद (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          अन्यथा अगर (म_भेद (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          अन्यथा अगर (म_भेद (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          अन्यथा
            वापस crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (काष्ठा sock_filter *) bpf;

  अगर (listener_receiver_fd >= 0)
    {
      cleanup_close पूर्णांक memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  अन्यथा
      वापस crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      अगर (UNLIKELY (memfd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "memfd_create");
#  अन्यथा
      वापस crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, माप (atomic_int));
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, शून्य, माप (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, शून्य);
      अगर (UNLIKELY (helper_proc < 0))
        वापस crun_make_error (err, त्रुटि_सं, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      अगर (helper_proc == 0)
        {
          पूर्णांक fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          क्रम (;;)
            {
              fd = *fd_received;
              अगर (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  अगर (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  जारी;
                }
#  अगर ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              अवरोध;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          अगर (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  अगर (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      अगर (त्रुटि_सं == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  अगर (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      पूर्णांक status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      अगर (ret != 0)
        वापस crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  वापस 0;
#अन्यथा
  वापस 0;
#पूर्ण_अगर
}

अटल bool
seccomp_action_supports_errno (स्थिर अक्षर *action)
{
  वापस म_भेद (action, "SCMP_ACT_ERRNO") == 0
         || म_भेद (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
अटल पूर्णांक
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, अचिन्हित पूर्णांक seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  अचिन्हित अक्षर hash[32];
  काष्ठा utsname utsbuf;
  माप_प्रकार i;
  पूर्णांक ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#घोषणा PROCESS_STRING(X)                                           \
  करो                                                                \
    {                                                               \
      स्थिर अक्षर *str_ = (X);                                       \
      अचिन्हित अक्षर present_ = str_ ? 1 : 0;                        \
      माप_प्रकार len_ = str_ ? म_माप (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, माप (present_)); \
      blake3_hasher_update (&hasher, &len_, माप (len_));         \
      अगर (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } जबतक (0)
#घोषणा PROCESS_तथ्य(X)                                   \
  करो                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), माप ((X))); \
  } जबतक (0)
#घोषणा PROCESS_OPTIONAL_तथ्य(PRESENT, X)                           \
  करो                                                                \
    {                                                               \
      अचिन्हित अक्षर present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, माप (present_)); \
      अगर (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), माप ((X)));         \
  } जबतक (0)
#घोषणा PROCESS_गिनती(X)                                        \
  करो                                                            \
    {                                                           \
      माप_प्रकार गिनती_2 = (X);                                      \
      blake3_hasher_update (&hasher, &गिनती_2, माप (गिनती_2)); \
  } जबतक (0)

  PROCESS_STRING (PACKAGE_VERSION);

#अगर_घोषित HAVE_SECCOMP
  {
    स्थिर काष्ठा scmp_version *version = seccomp_version ();

    PROCESS_तथ्य (version->major);
    PROCESS_तथ्य (version->minor);
    PROCESS_तथ्य (version->micro);
  }
#पूर्ण_अगर

  स_रखो (&utsbuf, 0, माप (utsbuf));
  ret = uname (&utsbuf);
  अगर (UNLIKELY (ret != 0))
    वापस crun_make_error (err, त्रुटि_सं, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_तथ्य (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_तथ्य (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_गिनती (seccomp->flags_len);
  क्रम (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_गिनती (seccomp->architectures_len);
  क्रम (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_गिनती (seccomp->syscalls_len);
  क्रम (i = 0; i < seccomp->syscalls_len; i++)
    {
      माप_प्रकार j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_तथ्य (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_गिनती (seccomp->syscalls[i]->names_len);
      क्रम (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_गिनती (seccomp->syscalls[i]->args_len);
      क्रम (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_तथ्य (seccomp->syscalls[i]->args[j]->सूचकांक);
          PROCESS_तथ्य (seccomp->syscalls[i]->args[j]->मूल्य);
          PROCESS_तथ्य (seccomp->syscalls[i]->args[j]->मूल्य_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, माप (hash));

  क्रम (i = 0; i < 32; i++)
    {
      माप_प्रकार remaining = माप (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      अगर (UNLIKELY (ret != 2))
        वापस crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#अघोषित PROCESS_STRING
#अघोषित PROCESS_तथ्य
#अघोषित PROCESS_OPTIONAL_तथ्य
#अघोषित PROCESS_गिनती
  वापस 1;
}

अटल पूर्णांक
open_rundir_dirfd (स्थिर अक्षर *state_मूल, libcrun_error_t *err)
{
  cleanup_free अक्षर *dir = शून्य;
  पूर्णांक dirfd;
  पूर्णांक ret;

  ret = libcrun_get_state_directory (&dir, state_मूल, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  dirfd = अस्थायी_FAILURE_RETRY (खोलो (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  अगर (UNLIKELY (dirfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", dir);

  वापस dirfd;
}

काष्ठा cache_entry
{
  seccomp_checksum_t checksum;
  काष्ठा timespec atime;
};

अटल पूर्णांक
compare_entries_by_atime (स्थिर व्योम *a, स्थिर व्योम *b)
{
  स्थिर काष्ठा cache_entry *entry_a = a;
  स्थिर काष्ठा cache_entry *entry_b = b;
  पूर्णांक ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  अगर (ret)
    वापस ret;

  वापस entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

अटल पूर्णांक
evict_cache (पूर्णांक मूल_dfd, libcrun_error_t *err)
{
  cleanup_close पूर्णांक cache_dir_fd = -1;
  off_t cache_dir_inode_max_आकार;
  काष्ठा stat st;
  पूर्णांक ret;

  ret = अस्थायी_FAILURE_RETRY (ख_स्थिति (मूल_dfd, &st));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fstat rundir");

  cache_dir_inode_max_आकार = get_cache_dir_inode_max_आकार (st.st_size);

  cache_dir_fd = अस्थायी_FAILURE_RETRY (openat (मूल_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  अगर (UNLIKELY (cache_dir_fd < 0))
    {
      अगर (त्रुटि_सं == ENOENT)
        वापस 0;
      वापस crun_make_error (err, त्रुटि_सं, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = अस्थायी_FAILURE_RETRY (ख_स्थिति (cache_dir_fd, &st));
  अगर (ret == 0 && st.st_size > cache_dir_inode_max_आकार)
    {
      cleanup_free काष्ठा cache_entry *entries = शून्य;
      cleanup_dir सूची *d = शून्य;
      माप_प्रकार i, n_entries = 0;
      काष्ठा dirent *de;
      पूर्णांक dfd;

      d = fdopendir (cache_dir_fd);
      अगर (d == शून्य)
        वापस crun_make_error (err, त्रुटि_सं, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      जबतक ((de = सूची_पढ़ो (d)))
        {
          अगर (de->d_name[0] == '.')
            जारी;

          ret = अस्थायी_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          अगर (UNLIKELY (ret < 0))
            जारी;

          /* The cache file is already used, it is pointless to free it.  */
          अगर (st.st_nlink > 1)
            जारी;

          /* If the sticky bit is set, then ignore it.  */
          अगर (st.st_mode & S_ISVTX)
            जारी;

          अगर (म_माप (de->d_name) != माप (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              जारी;
            }
          अन्यथा
            {
              entries = xrealloc (entries, माप (काष्ठा cache_entry) * (n_entries + 1));

              स_नकल (entries[n_entries].checksum, de->d_name, माप (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      क्विक (entries, n_entries, माप (काष्ठा cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      क्रम (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  वापस 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
अटल पूर्णांक
open_seccomp_bpf_dirfd (libcrun_container_t *container, अक्षर **rel_path, libcrun_error_t *err)
{
  पूर्णांक ret;
  पूर्णांक dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_मूल : शून्य, err);
  अगर (UNLIKELY (dirfd < 0))
    वापस dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", शून्य);
  अगर (UNLIKELY (ret < 0))
    {
      अस्थायी_FAILURE_RETRY (बंद (dirfd));
      वापस ret;
    }

  वापस dirfd;
}

अटल पूर्णांक
store_seccomp_cache (काष्ठा libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free अक्षर *src_path = शून्य;
  cleanup_free अक्षर *dest_path = शून्य;
  cleanup_close पूर्णांक dirfd = -1;
  पूर्णांक ret;

  अगर (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    वापस 0;

  अगर (is_empty_string (ctx->checksum))
    वापस 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  अगर (UNLIKELY (dirfd < 0))
    वापस dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = evict_cache (dirfd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  अगर (UNLIKELY (ret < 0 && त्रुटि_सं != EEXIST))
    वापस crun_make_error (err, त्रुटि_सं, "link `%s` to `%s`", src_path, dest_path);
  वापस 0;
}

अटल अंतरभूत runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (काष्ठा libcrun_seccomp_gen_ctx_s *ctx)
{
  अगर (ctx->container == शून्य || ctx->container->container_def == शून्य
      || ctx->container->container_def->linux == शून्य)
    वापस 0;

  वापस ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
अटल अंतरभूत bool
has_seccomp_configuration (काष्ठा libcrun_seccomp_gen_ctx_s *ctx)
{
  अगर (ctx->container == शून्य || ctx->container->container_def == शून्य
      || ctx->container->container_def->linux == शून्य)
    वापस false;

  वापस ctx->container->container_def->linux->seccomp != शून्य
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != शून्य;
}

अटल पूर्णांक
find_in_cache (काष्ठा libcrun_seccomp_gen_ctx_s *ctx, पूर्णांक dirfd, स्थिर अक्षर *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free अक्षर *cache_file_path = शून्य;
  पूर्णांक ret;

  अगर (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    वापस 0;

  seccomp = get_seccomp_configuration (ctx);
  अगर (seccomp == शून्य)
    वापस 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = अस्थायी_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  अगर (UNLIKELY (ret < 0 && त्रुटि_सं != ENOENT))
    वापस crun_make_error (err, त्रुटि_सं, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  वापस 0;
}

पूर्णांक
libcrun_generate_seccomp (काष्ठा libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#अगर_घोषित HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  पूर्णांक ret;
  माप_प्रकार i;
  cleanup_seccomp scmp_filter_ctx ctx = शून्य;
  पूर्णांक default_errno_मूल्य = EPERM;
  uint32_t action, default_action;
  स्थिर अक्षर *def_action = शून्य;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  अगर (gen_ctx->from_cache)
    वापस 0;

  अगर (gen_ctx->container == शून्य || gen_ctx->container->container_def == शून्य || gen_ctx->container->container_def->linux == शून्य)
    वापस 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  अगर (seccomp == शून्य)
    वापस 0;

  /* seccomp not available.  */
  अगर (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    वापस crun_make_error (err, त्रुटि_सं, "prctl");

  def_action = seccomp->default_action;
  अगर (def_action == शून्य)
    वापस crun_make_error (err, 0, "seccomp misses the default action");

  अगर (seccomp->default_errno_ret_present)
    {
      अगर (! seccomp_action_supports_errno (def_action))
        वापस crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_मूल्य = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_मूल्य, &default_action, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ctx = seccomp_init (default_action);
  अगर (ctx == शून्य)
    वापस crun_make_error (err, 0, "error seccomp_init");

  क्रम (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      स्थिर अक्षर *arch = seccomp->architectures[i];
      अक्षर *end, lowercase_arch[32] = {
        0,
      };

      अगर (arch == शून्य)
        वापस crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      अगर (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, माप (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      अगर (arch_token == 0)
        वापस crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  अन्यथा
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      अगर (ret < 0 && ret != -EEXIST)
        वापस crun_make_error (err, -ret, "seccomp adding architecture");
    }

  क्रम (i = 0; i < seccomp->syscalls_len; i++)
    {
      माप_प्रकार j;
      bool multiple_args = false;
      पूर्णांक errno_ret = EPERM;

      अगर (seccomp->syscalls[i]->errno_ret_present)
        {
          अगर (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            वापस crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      अगर (action == default_action)
        जारी;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      अगर (seccomp->syscalls[i]->args != शून्य)
        {
          uint32_t गिनती[6] = {};
          माप_प्रकार k;

          क्रम (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t सूचकांक = seccomp->syscalls[i]->args[k]->सूचकांक;

              अगर (सूचकांक >= 6)
                वापस crun_make_error (err, 0, "invalid seccomp index `%u`", सूचकांक);

              गिनती[सूचकांक]++;
              अगर (गिनती[सूचकांक] > 1)
                multiple_args = true;
            }
        }

      क्रम (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          पूर्णांक syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          अगर (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              अगर (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                वापस crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              जारी;
            }

          अगर (seccomp->syscalls[i]->args == शून्य)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              अगर (UNLIKELY (ret < 0))
                वापस crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
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
              अगर (multiple_args)
                {
                  माप_प्रकार r;

                  क्रम (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      काष्ठा scmp_arg_cmp arg_cmp;
                      अक्षर *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->सूचकांक;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      अगर (UNLIKELY (ret < 0))
                        वापस ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->मूल्य;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->मूल्य_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      अगर (UNLIKELY (ret < 0))
                        वापस crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              अन्यथा
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  स्थिर माप_प्रकार args_len = seccomp->syscalls[i]->args_len;
                  काष्ठा scmp_arg_cmp arg_cmp[6];

                  क्रम (k = 0; k < args_len; k++)
                    {
                      अक्षर *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->सूचकांक;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      अगर (UNLIKELY (ret < 0))
                        वापस ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->मूल्य;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->मूल्य_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : शून्य);
                  अगर (UNLIKELY (ret < 0))
                    वापस crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  अगर (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      वापस store_seccomp_cache (gen_ctx, err);
    }

  वापस 0;
#अन्यथा
  वापस 0;
#पूर्ण_अगर
}

पूर्णांक
libcrun_copy_seccomp (काष्ठा libcrun_seccomp_gen_ctx_s *gen_ctx, स्थिर अक्षर *b64_bpf, libcrun_error_t *err)
{
  cleanup_free अक्षर *bpf_तथ्य = शून्य;
  माप_प्रकार आकार = 0;
  माप_प्रकार in_आकार;
  पूर्णांक consumed;
  पूर्णांक ret;

  in_आकार = म_माप (b64_bpf);
  bpf_तथ्य = xmalloc (in_आकार + 1);

  consumed = base64_decode (b64_bpf, in_आकार, bpf_तथ्य, in_आकार, &आकार);
  अगर (UNLIKELY (consumed != (पूर्णांक) in_आकार))
    वापस crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_तथ्य, आकार, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

पूर्णांक
libcrun_open_seccomp_bpf (काष्ठा libcrun_seccomp_gen_ctx_s *ctx, पूर्णांक *fd, libcrun_error_t *err)
{
  cleanup_close पूर्णांक dirfd = -1;
  cleanup_free अक्षर *dest_path = शून्य;
  libcrun_container_t *container = ctx->container;
  पूर्णांक ret;

  ctx->fd = *fd = -1;

  अगर (container == शून्य || container->context == शून्य)
    वापस crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  अगर (UNLIKELY (dirfd < 0))
    वापस dirfd;

  अगर (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      अगर (created)
        {
          ctx->from_cache = true;
          जाओ open_existing_file;
        }

      ret = अस्थायी_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  अन्यथा
    {
    open_existing_file:
      ret = अस्थायी_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      अगर (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          अगर (त्रुटि_सं == ENOENT && ! has_seccomp_configuration (ctx))
            वापस 0;
          वापस crun_make_error (err, त्रुटि_सं, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  वापस 0;
}
