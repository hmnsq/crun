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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "seccomp.h"
#అన్తర్భావ "linux.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ <sched.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/mount.h>
#అన్తర్భావ <sys/syscall.h>
#అన్తర్భావ <sys/prctl.h>
#అన్తర్భావ <sys/time.h>
#అన్తర్భావ <sys/resource.h>
#అన్తర్భావ <sys/sysmacros.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/utsname.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/mman.h>

#నిర్వచితమైతే HAVE_BLAKE3
#  include <blake3.h>
#లేకపోతే
#  include "blake3/blake3.h"
#అయితే_ముగింపు

#ఒకవేళ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int అస్థిర పూర్ణాంకం
#  endif
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_SECCOMP
#  include <seccomp.h>
#అయితే_ముగింపు
#అన్తర్భావ <linux/seccomp.h>
#అన్తర్భావ <linux/filter.h>
#అన్తర్భావ <sys/prctl.h>
#అన్తర్భావ <sys/syscall.h>

#నిర్వచితం_కాకపోతే __NR_seccomp
#  define __NR_seccomp 0xffff // seccomp syscall number unknown for this architecture
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే SECCOMP_SET_MODE_STRICT
#  define SECCOMP_SET_MODE_STRICT 0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే SECCOMP_SET_MODE_FILTER
#  define SECCOMP_SET_MODE_FILTER 1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే SECCOMP_FILTER_FLAG_TSYNC
#  define SECCOMP_FILTER_FLAG_TSYNC (1UL << 0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే SECCOMP_FILTER_FLAG_LOG
#  define SECCOMP_FILTER_FLAG_LOG (1UL << 1)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే SECCOMP_FILTER_FLAG_SPEC_ALLOW
#  define SECCOMP_FILTER_FLAG_SPEC_ALLOW (1UL << 2)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV
#  define SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV (1UL << 5)
#అయితే_ముగింపు

#నిర్వచించు SECCOMP_CACHE_DIR ".cache/seccomp"

/* On Linux 5.19.15 x86_64, tmpfs reports 20 bytes + 20 bytes for each entry, so allow
   at least 32 entries.  */
#నిర్వచించు MIN_CACHE_DIR_INODE_కొలత (20 + 32 * 20)

/* Heuristic to avoid the cache directory grows indefinitely.  The inode size for
   the directory is proportional to the number of entries it contains and it is
   cheaper to compute than iterating the cache directory.  */
స్థిర అంతఃవరుస off_t
get_cache_dir_inode_max_కొలత (off_t కొలత_rundir)
{
  తిరిగి కొలత_rundir * 3 + MIN_CACHE_DIR_INODE_కొలత;
}

స్థిర పూర్ణాంకం
syscall_seccomp (గుర్తులేని పూర్ణాంకం operation, గుర్తులేని పూర్ణాంకం flags, శూన్య *వాదనలు)
{
  తిరిగి (పూర్ణాంకం) syscall (__NR_seccomp, operation, flags, వాదనలు);
}

#నిర్వచితమైతే HAVE_SECCOMP
స్థిర పూర్ణాంకం
get_seccomp_operator (స్థిరరాశి అక్షరం *name, గణన scmp_compare *op, libcrun_error_t *err)
{
  స్థిర స్థిరరాశి నిర్మాణం
  {
    స్థిరరాశి అక్షరం *name;
    గణన scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  స్థిరరాశి అక్షరం *p = name;
  size_t i;

  ఒకవేళ (! has_ఉపసర్గ (p, "SCMP_CMP_"))
    వెళ్ళు fail;

  p += పరిమాణం ("SCMP_CMP_") - 1;

  కొరకు (i = 0; i < పరిమాణం (operators) / పరిమాణం (operators[0]); i++)
    ఒకవేళ (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        తిరిగి 0;
      }

fail:
  తిరిగి crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

స్థిర పూర్ణాంకం
get_seccomp_action (స్థిరరాశి అక్షరం *name, పూర్ణాంకం errno_ret, uint32_t *action, libcrun_error_t *err)
{
  స్థిర స్థిరరాశి నిర్మాణం
  {
    స్థిరరాశి అక్షరం *name;
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
  స్థిరరాశి అక్షరం *p = name;
  size_t i;

  ఒకవేళ (! has_ఉపసర్గ (p, "SCMP_ACT_"))
    వెళ్ళు fail;

  p += పరిమాణం ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  ఒకవేళ (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      తిరిగి 0;
    }
  ఒకవేళ (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      తిరిగి 0;
    }

  కొరకు (i = 0; i < పరిమాణం (actions) / పరిమాణం (actions[0]); i++)
    ఒకవేళ (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        తిరిగి 0;
      }

fail:
  తిరిగి crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
స్థిర పూర్ణాంకం
export_bpf (scmp_filter_ctx ctx, పూర్ణాంకం fd, libcrun_error_t *err)
{
  cleanup_free అక్షరం *bpf = ఖాళీ;
  size_t len = 0;
  పూర్ణాంకం ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, ఖాళీ, &len);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  లేకపోతే
  cleanup_close పూర్ణాంకం pipe_r = -1;
  cleanup_close పూర్ణాంకం pipe_w = -1;
  పూర్ణాంకం p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  ఒకవేళ (UNLIKELY (len >= (size_t) ret))
    తిరిగి crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  తిరిగి సురక్షిత_write (fd, "seccomp.bpf", bpf, len, err);
}
#అయితే_ముగింపు /* HAVE_SECCOMP */

స్థిర శూన్య
make_lowercase (అక్షరం *str)
{
  అయితే (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

స్థిర శూన్య
cleanup_seccompp (శూన్య *p)
{
#నిర్వచితమైతే HAVE_SECCOMP
  scmp_filter_ctx *ctx = (శూన్య **) p;
  ఒకవేళ (*ctx)
    seccomp_release (*ctx);
#అయితే_ముగింపు
}
#నిర్వచించు cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

పూర్ణాంకం
libcrun_apply_seccomp (పూర్ణాంకం infd, పూర్ణాంకం listener_receiver_fd, స్థిరరాశి అక్షరం *receiver_fd_payload,
                       size_t receiver_fd_payload_len, అక్షరం **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#నిర్వచితమైతే HAVE_SECCOMP
  cleanup_mmap నిర్మాణం libcrun_mmap_s *mmap_region = ఖాళీ;
  cleanup_close పూర్ణాంకం listener_fd = -1;
  cleanup_pid పూర్ణాంకం helper_proc = -1;
  నిర్మాణం sock_fprog seccomp_filter;
  cleanup_free అక్షరం *bpf = ఖాళీ;
  గుర్తులేని పూర్ణాంకం flags = 0;
  size_t len;
  పూర్ణాంకం ret;

  ఒకవేళ (infd < 0)
    తిరిగి 0;

  ఒకవేళ (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    తిరిగి crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  ఒకవేళ (seccomp_flags == ఖాళీ)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  లేకపోతే
    {
      size_t i = 0;
      కొరకు (i = 0; i < seccomp_flags_len; i++)
        {
          ఒకవేళ (seccomp_flags[i] == ఖాళీ)
            తిరిగి crun_make_error (err, EINVAL, "seccomp flag is not specified");

          ఒకవేళ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          లేకపోతే ఒకవేళ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          లేకపోతే ఒకవేళ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          లేకపోతే ఒకవేళ (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          లేకపోతే
            తిరిగి crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (నిర్మాణం sock_filter *) bpf;

  ఒకవేళ (listener_receiver_fd >= 0)
    {
      cleanup_close పూర్ణాంకం memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  లేకపోతే
      తిరిగి crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      ఒకవేళ (UNLIKELY (memfd < 0))
        తిరిగి crun_make_error (err, errno, "memfd_create");
#  లేకపోతే
      తిరిగి crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, పరిమాణం (atomic_int));
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, ఖాళీ, పరిమాణం (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, ఖాళీ);
      ఒకవేళ (UNLIKELY (helper_proc < 0))
        తిరిగి crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      ఒకవేళ (helper_proc == 0)
        {
          పూర్ణాంకం fd, timeout = 0;

          prctl (PR_సమితి_PDEATHSIG, SIGKILL);
          కొరకు (;;)
            {
              fd = *fd_received;
              ఒకవేళ (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  ఒకవేళ (timeout++ > 5000)
                    సురక్షిత_exit (EINVAL);
                  కొనసాగు;
                }
#  ఒకవేళ ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              ఆపు;
            }
          ret = send_fd_గమ్యం_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          ఒకవేళ (UNLIKELY (ret < 0))
            సురక్షిత_exit (crun_error_get_errno (err));
          సురక్షిత_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      ఒకవేళ (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  ఒకవేళ (listener_receiver_fd >= 0)
    {
      atomic_int *fd_గమ్యం_send = mmap_region->addr;
      పూర్ణాంకం స్థితి_2 = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_గమ్యం_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &స్థితి_2, 0);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_స్థితి (స్థితి_2);
      ఒకవేళ (ret != 0)
        తిరిగి crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  తిరిగి 0;
#లేకపోతే
  తిరిగి 0;
#అయితే_ముగింపు
}

స్థిర bool
seccomp_action_supports_errno (స్థిరరాశి అక్షరం *action)
{
  తిరిగి strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
స్థిర పూర్ణాంకం
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, గుర్తులేని పూర్ణాంకం seccomp_gen_options, seccomp_checksum_t బహిర్గతం, libcrun_error_t *err)
{
  blake3_hasher hasher;
  గుర్తులేని అక్షరం hash[32];
  నిర్మాణం utsname utsbuf;
  size_t i;
  పూర్ణాంకం ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#నిర్వచించు PROCESS_STRING(X)                                           \
  చేయి                                                                \
    {                                                               \
      స్థిరరాశి అక్షరం *str_ = (X);                                       \
      గుర్తులేని అక్షరం present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? అక్షరశ్రేణి_పొడవు (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, పరిమాణం (present_)); \
      blake3_hasher_update (&hasher, &len_, పరిమాణం (len_));         \
      ఒకవేళ (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } అయితే (0)
#నిర్వచించు PROCESS_దత్తాంశం(X)                                   \
  చేయి                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), పరిమాణం ((X))); \
  } అయితే (0)
#నిర్వచించు PROCESS_OPTIONAL_దత్తాంశం(PRESENT, X)                           \
  చేయి                                                                \
    {                                                               \
      గుర్తులేని అక్షరం present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, పరిమాణం (present_)); \
      ఒకవేళ (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), పరిమాణం ((X)));         \
  } అయితే (0)
#నిర్వచించు PROCESS_లెక్క(X)                                        \
  చేయి                                                            \
    {                                                           \
      size_t లెక్క_2 = (X);                                      \
      blake3_hasher_update (&hasher, &లెక్క_2, పరిమాణం (లెక్క_2)); \
  } అయితే (0)

  PROCESS_STRING (PACKAGE_సంస్కరణ);

#నిర్వచితమైతే HAVE_SECCOMP
  {
    స్థిరరాశి నిర్మాణం scmp_సంస్కరణ *సంస్కరణ = seccomp_సంస్కరణ ();

    PROCESS_దత్తాంశం (సంస్కరణ->major);
    PROCESS_దత్తాంశం (సంస్కరణ->minor);
    PROCESS_దత్తాంశం (సంస్కరణ->micro);
  }
#అయితే_ముగింపు

  memset (&utsbuf, 0, పరిమాణం (utsbuf));
  ret = uname (&utsbuf);
  ఒకవేళ (UNLIKELY (ret != 0))
    తిరిగి crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.సంస్కరణ);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_దత్తాంశం (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_దత్తాంశం (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_లెక్క (seccomp->flags_len);
  కొరకు (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_లెక్క (seccomp->architectures_len);
  కొరకు (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_లెక్క (seccomp->syscalls_len);
  కొరకు (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_దత్తాంశం (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_లెక్క (seccomp->syscalls[i]->పేర్లు_len);
      కొరకు (j = 0; j < seccomp->syscalls[i]->పేర్లు_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->పేర్లు[j]);

      PROCESS_లెక్క (seccomp->syscalls[i]->వాదనలు_len);
      కొరకు (j = 0; j < seccomp->syscalls[i]->వాదనలు_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_దత్తాంశం (seccomp->syscalls[i]->వాదనలు[j]->సూచీ);
          PROCESS_దత్తాంశం (seccomp->syscalls[i]->వాదనలు[j]->విలువ);
          PROCESS_దత్తాంశం (seccomp->syscalls[i]->వాదనలు[j]->విలువ_two);
          PROCESS_STRING (seccomp->syscalls[i]->వాదనలు[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, పరిమాణం (hash));

  కొరకు (i = 0; i < 32; i++)
    {
      size_t మిగిలిన = పరిమాణం (seccomp_checksum_t) - i * 2;
      ret = snprintf (&బహిర్గతం[i * 2], మిగిలిన, "%02x", hash[i]);
      ఒకవేళ (UNLIKELY (ret != 2))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  బహిర్గతం[64] = 0;

#నిర్వచనం_రద్దు PROCESS_STRING
#నిర్వచనం_రద్దు PROCESS_దత్తాంశం
#నిర్వచనం_రద్దు PROCESS_OPTIONAL_దత్తాంశం
#నిర్వచనం_రద్దు PROCESS_లెక్క
  తిరిగి 1;
}

స్థిర పూర్ణాంకం
open_rundir_dirfd (స్థిరరాశి అక్షరం *state_మూలం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *dir = ఖాళీ;
  పూర్ణాంకం dirfd;
  పూర్ణాంకం ret;

  ret = libcrun_get_state_directory (&dir, state_మూలం, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  dirfd = తాత్కాలిక_FAILURE_RETRY (open (dir, O_మార్గం | O_DIRECTORY | O_CLOEXEC));
  ఒకవేళ (UNLIKELY (dirfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", dir);

  తిరిగి dirfd;
}

నిర్మాణం cache_నమోదు
{
  seccomp_checksum_t checksum;
  నిర్మాణం timespec atime;
};

స్థిర పూర్ణాంకం
compare_entries_by_atime (స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
  స్థిరరాశి నిర్మాణం cache_నమోదు *నమోదు_a = a;
  స్థిరరాశి నిర్మాణం cache_నమోదు *నమోదు_b = b;
  పూర్ణాంకం ret;

  ret = నమోదు_a->atime.tv_sec - నమోదు_b->atime.tv_sec;
  ఒకవేళ (ret)
    తిరిగి ret;

  తిరిగి నమోదు_a->atime.tv_nsec - నమోదు_b->atime.tv_nsec;
}

స్థిర పూర్ణాంకం
evict_cache (పూర్ణాంకం మూలం_dfd, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం cache_dir_fd = -1;
  off_t cache_dir_inode_max_కొలత;
  నిర్మాణం stat st;
  పూర్ణాంకం ret;

  ret = తాత్కాలిక_FAILURE_RETRY (fstat (మూలం_dfd, &st));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_కొలత = get_cache_dir_inode_max_కొలత (st.st_size);

  cache_dir_fd = తాత్కాలిక_FAILURE_RETRY (openat (మూలం_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  ఒకవేళ (UNLIKELY (cache_dir_fd < 0))
    {
      ఒకవేళ (errno == ENOENT)
        తిరిగి 0;
      తిరిగి crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = తాత్కాలిక_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  ఒకవేళ (ret == 0 && st.st_size > cache_dir_inode_max_కొలత)
    {
      cleanup_free నిర్మాణం cache_నమోదు *entries = ఖాళీ;
      cleanup_dir DIR *d = ఖాళీ;
      size_t i, n_entries = 0;
      నిర్మాణం dirent *de;
      పూర్ణాంకం dfd;

      d = fdopendir (cache_dir_fd);
      ఒకవేళ (d == ఖాళీ)
        తిరిగి crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      అయితే ((de = readdir (d)))
        {
          ఒకవేళ (de->d_name[0] == '.')
            కొనసాగు;

          ret = తాత్కాలిక_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          ఒకవేళ (UNLIKELY (ret < 0))
            కొనసాగు;

          /* The cache file is already used, it is pointless to free it.  */
          ఒకవేళ (st.st_nlink > 1)
            కొనసాగు;

          /* If the sticky bit is set, then ignore it.  */
          ఒకవేళ (st.st_mode & S_ISVTX)
            కొనసాగు;

          ఒకవేళ (అక్షరశ్రేణి_పొడవు (de->d_name) != పరిమాణం (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              కొనసాగు;
            }
          లేకపోతే
            {
              entries = xrealloc (entries, పరిమాణం (నిర్మాణం cache_నమోదు) * (n_entries + 1));

              జ్ఞాపకం_నకలు (entries[n_entries].checksum, de->d_name, పరిమాణం (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, పరిమాణం (నిర్మాణం cache_నమోదు), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      కొరకు (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  తిరిగి 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
స్థిర పూర్ణాంకం
open_seccomp_bpf_dirfd (libcrun_container_t *container, అక్షరం **rel_మార్గం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  పూర్ణాంకం dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_మూలం : ఖాళీ, err);
  ఒకవేళ (UNLIKELY (dirfd < 0))
    తిరిగి dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_మార్గం, err, container->context->id, "seccomp.bpf", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      తాత్కాలిక_FAILURE_RETRY (close (dirfd));
      తిరిగి ret;
    }

  తిరిగి dirfd;
}

స్థిర పూర్ణాంకం
store_seccomp_cache (నిర్మాణం libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free అక్షరం *src_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *dest_మార్గం = ఖాళీ;
  cleanup_close పూర్ణాంకం dirfd = -1;
  పూర్ణాంకం ret;

  ఒకవేళ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    తిరిగి 0;

  ఒకవేళ (is_ఖాళీలు_string (ctx->checksum))
    తిరిగి 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_మార్గం, err);
  ఒకవేళ (UNLIKELY (dirfd < 0))
    తిరిగి dirfd;

  ret = append_paths (&dest_మార్గం, err, SECCOMP_CACHE_DIR, ctx->checksum, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = evict_cache (dirfd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = linkat (dirfd, src_మార్గం, dirfd, dest_మార్గం, 0);
  ఒకవేళ (UNLIKELY (ret < 0 && errno != EEXIST))
    తిరిగి crun_make_error (err, errno, "link `%s` to `%s`", src_మార్గం, dest_మార్గం);
  తిరిగి 0;
}

స్థిర అంతఃవరుస runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (నిర్మాణం libcrun_seccomp_gen_ctx_s *ctx)
{
  ఒకవేళ (ctx->container == ఖాళీ || ctx->container->container_def == ఖాళీ
      || ctx->container->container_def->linux == ఖాళీ)
    తిరిగి 0;

  తిరిగి ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
స్థిర అంతఃవరుస bool
has_seccomp_configuration (నిర్మాణం libcrun_seccomp_gen_ctx_s *ctx)
{
  ఒకవేళ (ctx->container == ఖాళీ || ctx->container->container_def == ఖాళీ
      || ctx->container->container_def->linux == ఖాళీ)
    తిరిగి false;

  తిరిగి ctx->container->container_def->linux->seccomp != ఖాళీ
         || కనుగొను_annotation (ctx->container, "run.oci.seccomp_bpf_data") != ఖాళీ;
}

స్థిర పూర్ణాంకం
కనుగొను_లోపల_cache (నిర్మాణం libcrun_seccomp_gen_ctx_s *ctx, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *dest_మార్గం, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free అక్షరం *cache_file_మార్గం = ఖాళీ;
  పూర్ణాంకం ret;

  ఒకవేళ (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    తిరిగి 0;

  seccomp = get_seccomp_configuration (ctx);
  ఒకవేళ (seccomp == ఖాళీ)
    తిరిగి 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_paths (&cache_file_మార్గం, err, SECCOMP_CACHE_DIR, ctx->checksum, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = తాత్కాలిక_FAILURE_RETRY (linkat (dirfd, cache_file_మార్గం, dirfd, dest_మార్గం, 0));
  ఒకవేళ (UNLIKELY (ret < 0 && errno != ENOENT))
    తిరిగి crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_మార్గం, dest_మార్గం);

  *created = ret == 0;

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_generate_seccomp (నిర్మాణం libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#నిర్వచితమైతే HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  పూర్ణాంకం ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = ఖాళీ;
  పూర్ణాంకం default_errno_విలువ = EPERM;
  uint32_t action, default_action;
  స్థిరరాశి అక్షరం *def_action = ఖాళీ;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  ఒకవేళ (gen_ctx->from_cache)
    తిరిగి 0;

  ఒకవేళ (gen_ctx->container == ఖాళీ || gen_ctx->container->container_def == ఖాళీ || gen_ctx->container->container_def->linux == ఖాళీ)
    తిరిగి 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  ఒకవేళ (seccomp == ఖాళీ)
    తిరిగి 0;

  /* seccomp not available.  */
  ఒకవేళ (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    తిరిగి crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  ఒకవేళ (def_action == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "seccomp misses the default action");

  ఒకవేళ (seccomp->default_errno_ret_present)
    {
      ఒకవేళ (! seccomp_action_supports_errno (def_action))
        తిరిగి crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_విలువ = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_విలువ, &default_action, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ctx = seccomp_init (default_action);
  ఒకవేళ (ctx == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "error seccomp_init");

  కొరకు (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      స్థిరరాశి అక్షరం *arch = seccomp->architectures[i];
      అక్షరం *చివర, lowercase_arch[32] = {
        0,
      };

      ఒకవేళ (arch == ఖాళీ)
        తిరిగి crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      ఒకవేళ (has_ఉపసర్గ (arch, "SCMP_ARCH_"))
        arch += 10;
      చివర = stpncpy (lowercase_arch, arch, పరిమాణం (lowercase_arch) - 1);
      *చివర = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      ఒకవేళ (arch_token == 0)
        తిరిగి crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  లేకపోతే
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_కూడు (ctx, arch_token);
      ఒకవేళ (ret < 0 && ret != -EEXIST)
        తిరిగి crun_make_error (err, -ret, "seccomp adding architecture");
    }

  కొరకు (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool గుణిజం_వాదనలు = false;
      పూర్ణాంకం errno_ret = EPERM;

      ఒకవేళ (seccomp->syscalls[i]->errno_ret_present)
        {
          ఒకవేళ (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            తిరిగి crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (action == default_action)
        కొనసాగు;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      ఒకవేళ (seccomp->syscalls[i]->వాదనలు != ఖాళీ)
        {
          uint32_t లెక్క[6] = {};
          size_t k;

          కొరకు (k = 0; k < seccomp->syscalls[i]->వాదనలు_len; k++)
            {
              uint32_t సూచీ = seccomp->syscalls[i]->వాదనలు[k]->సూచీ;

              ఒకవేళ (సూచీ >= 6)
                తిరిగి crun_make_error (err, 0, "invalid seccomp index `%u`", సూచీ);

              లెక్క[సూచీ]++;
              ఒకవేళ (లెక్క[సూచీ] > 1)
                గుణిజం_వాదనలు = true;
            }
        }

      కొరకు (j = 0; j < seccomp->syscalls[i]->పేర్లు_len; j++)
        {
          పూర్ణాంకం syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->పేర్లు[j]);

          ఒకవేళ (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              ఒకవేళ (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                తిరిగి crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->పేర్లు[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->పేర్లు[j]);
              కొనసాగు;
            }

          ఒకవేళ (seccomp->syscalls[i]->వాదనలు == ఖాళీ)
            {
              ret = seccomp_నియమం_కూడు (ctx, action, syscall, 0);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->పేర్లు[j]);
            }
          లేకపోతే
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              ఒకవేళ (గుణిజం_వాదనలు)
                {
                  size_t r;

                  కొరకు (r = 0; r < seccomp->syscalls[i]->వాదనలు_len; r++)
                    {
                      నిర్మాణం scmp_arg_cmp arg_cmp;
                      అక్షరం *op = seccomp->syscalls[i]->వాదనలు[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->వాదనలు[r]->సూచీ;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      ఒకవేళ (UNLIKELY (ret < 0))
                        తిరిగి ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->వాదనలు[r]->విలువ;
                      arg_cmp.datum_b = seccomp->syscalls[i]->వాదనలు[r]->విలువ_two;

                      ret = seccomp_నియమం_కూడు_array (ctx, action, syscall, 1, &arg_cmp);
                      ఒకవేళ (UNLIKELY (ret < 0))
                        తిరిగి crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              లేకపోతే
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  స్థిరరాశి size_t వాదనలు_len = seccomp->syscalls[i]->వాదనలు_len;
                  నిర్మాణం scmp_arg_cmp arg_cmp[6];

                  కొరకు (k = 0; k < వాదనలు_len; k++)
                    {
                      అక్షరం *op = seccomp->syscalls[i]->వాదనలు[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->వాదనలు[k]->సూచీ;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      ఒకవేళ (UNLIKELY (ret < 0))
                        తిరిగి ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->వాదనలు[k]->విలువ;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->వాదనలు[k]->విలువ_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_నియమం_కూడు_array (ctx, action, syscall, వాదనలు_len, వాదనలు_len ? arg_cmp : ఖాళీ);
                  ఒకవేళ (UNLIKELY (ret < 0))
                    తిరిగి crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  ఒకవేళ (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      తిరిగి store_seccomp_cache (gen_ctx, err);
    }

  తిరిగి 0;
#లేకపోతే
  తిరిగి 0;
#అయితే_ముగింపు
}

పూర్ణాంకం
libcrun_copy_seccomp (నిర్మాణం libcrun_seccomp_gen_ctx_s *gen_ctx, స్థిరరాశి అక్షరం *b64_bpf, libcrun_error_t *err)
{
  cleanup_free అక్షరం *bpf_దత్తాంశం = ఖాళీ;
  size_t కొలత = 0;
  size_t లోపల_కొలత;
  పూర్ణాంకం consumed;
  పూర్ణాంకం ret;

  లోపల_కొలత = అక్షరశ్రేణి_పొడవు (b64_bpf);
  bpf_దత్తాంశం = xmalloc (లోపల_కొలత + 1);

  consumed = base64_decode (b64_bpf, లోపల_కొలత, bpf_దత్తాంశం, లోపల_కొలత, &కొలత);
  ఒకవేళ (UNLIKELY (consumed != (పూర్ణాంకం) లోపల_కొలత))
    తిరిగి crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = సురక్షిత_write (gen_ctx->fd, "seccomp fd", bpf_దత్తాంశం, కొలత, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_open_seccomp_bpf (నిర్మాణం libcrun_seccomp_gen_ctx_s *ctx, పూర్ణాంకం *fd, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం dirfd = -1;
  cleanup_free అక్షరం *dest_మార్గం = ఖాళీ;
  libcrun_container_t *container = ctx->container;
  పూర్ణాంకం ret;

  ctx->fd = *fd = -1;

  ఒకవేళ (container == ఖాళీ || container->context == ఖాళీ)
    తిరిగి crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_మార్గం, err);
  ఒకవేళ (UNLIKELY (dirfd < 0))
    తిరిగి dirfd;

  ఒకవేళ (ctx->create)
    {
      bool created = false;

      ret = కనుగొను_లోపల_cache (ctx, dirfd, dest_మార్గం, &created, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (created)
        {
          ctx->from_cache = true;
          వెళ్ళు open_existing_file;
        }

      ret = తాత్కాలిక_FAILURE_RETRY (openat (dirfd, dest_మార్గం, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  లేకపోతే
    {
    open_existing_file:
      ret = తాత్కాలిక_FAILURE_RETRY (openat (dirfd, dest_మార్గం, O_CLOEXEC | O_RDONLY));
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          ఒకవేళ (errno == ENOENT && ! has_seccomp_configuration (ctx))
            తిరిగి 0;
          తిరిగి crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  తిరిగి 0;
}
