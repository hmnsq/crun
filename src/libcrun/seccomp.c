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

#تعریف_کن GNU_منبع

#ضمن <config.h>
#ضمن "seccomp.h"
#ضمن "linux.h"
#ضمن "utils.h"
#ضمن <string.h>
#ضمن <sched.h>
#ضمن <fcntl.h>
#ضمن <unistd.h>
#ضمن <sys/mount.h>
#ضمن <sys/syscall.h>
#ضمن <sys/prctl.h>
#ضمن <sys/time.h>
#ضمن <sys/resource.h>
#ضمن <sys/sysmacros.h>
#ضمن <sys/types.h>
#ضمن <sys/utsname.h>
#ضمن <sys/stat.h>
#ضمن <sys/mman.h>

#اگر_تعریف_شده HAVE_BLAKE3
#  include <blake3.h>
#وإلا
#  include "blake3/blake3.h"
#پایان_اگر

#إذا HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int متطاير صحيح
#  endif
#پایان_اگر

#اگر_تعریف_شده HAVE_SECCOMP
#  include <seccomp.h>
#پایان_اگر
#ضمن <linux/seccomp.h>
#ضمن <linux/filter.h>
#ضمن <sys/prctl.h>
#ضمن <sys/syscall.h>

#اگر_تعریف_نشده __NR_seccomp
#  define __NR_seccomp 0xffff // seccomp syscall number unknown for this architecture
#پایان_اگر

#اگر_تعریف_نشده SECCOMP_SET_MODE_STRICT
#  define SECCOMP_SET_MODE_STRICT 0
#پایان_اگر

#اگر_تعریف_نشده SECCOMP_SET_MODE_FILTER
#  define SECCOMP_SET_MODE_FILTER 1
#پایان_اگر

#اگر_تعریف_نشده SECCOMP_FILTER_FLAG_TSYNC
#  define SECCOMP_FILTER_FLAG_TSYNC (1UL << 0)
#پایان_اگر

#اگر_تعریف_نشده SECCOMP_FILTER_FLAG_LOG
#  define SECCOMP_FILTER_FLAG_LOG (1UL << 1)
#پایان_اگر

#اگر_تعریف_نشده SECCOMP_FILTER_FLAG_SPEC_ALLOW
#  define SECCOMP_FILTER_FLAG_SPEC_ALLOW (1UL << 2)
#پایان_اگر

#اگر_تعریف_نشده SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV
#  define SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV (1UL << 5)
#پایان_اگر

#تعریف_کن SECCOMP_CACHE_DIR ".cache/seccomp"

/* On Linux 5.19.15 x86_64, tmpfs reports 20 bytes + 20 bytes for each entry, so allow
   at least 32 entries.  */
#تعریف_کن MIN_CACHE_DIR_INODE_اندازه (20 + 32 * 20)

/* Heuristic to avoid the cache directory grows indefinitely.  The inode size for
   the directory is proportional to the number of entries it contains and it is
   cheaper to compute than iterating the cache directory.  */
ساكن مضمن off_t
get_cache_dir_inode_max_اندازه (off_t اندازه_rundir)
{
  أرجع اندازه_rundir * 3 + MIN_CACHE_DIR_INODE_اندازه;
}

ساكن صحيح
syscall_seccomp (بیعلامت صحيح operation, بیعلامت صحيح flags, فراغ *آرگومانها)
{
  أرجع (صحيح) syscall (__NR_seccomp, operation, flags, آرگومانها);
}

#اگر_تعریف_شده HAVE_SECCOMP
ساكن صحيح
get_seccomp_operator (ثابت حرف *name, تعداد scmp_compare *op, libcrun_error_t *err)
{
  ساكن ثابت بنية
  {
    ثابت حرف *name;
    تعداد scmp_compare op;
  } operators[] = {
    { "NE", SCMP_CMP_NE },
    { "LT", SCMP_CMP_LT },
    { "LE", SCMP_CMP_LE },
    { "EQ", SCMP_CMP_EQ },
    { "GE", SCMP_CMP_GE },
    { "GT", SCMP_CMP_GT },
    { "MASKED_EQ", SCMP_CMP_MASKED_EQ },
  };
  ثابت حرف *p = name;
  size_t i;

  إذا (! has_پیشوند (p, "SCMP_CMP_"))
    اذهب fail;

  p += حجم ("SCMP_CMP_") - 1;

  لكل (i = 0; i < حجم (operators) / حجم (operators[0]); i++)
    إذا (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        أرجع 0;
      }

fail:
  أرجع crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

ساكن صحيح
get_seccomp_action (ثابت حرف *name, صحيح errno_ret, uint32_t *action, libcrun_error_t *err)
{
  ساكن ثابت بنية
  {
    ثابت حرف *name;
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
  ثابت حرف *p = name;
  size_t i;

  إذا (! has_پیشوند (p, "SCMP_ACT_"))
    اذهب fail;

  p += حجم ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  إذا (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      أرجع 0;
    }
  إذا (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      أرجع 0;
    }

  لكل (i = 0; i < حجم (actions) / حجم (actions[0]); i++)
    إذا (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        أرجع 0;
      }

fail:
  أرجع crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
ساكن صحيح
export_bpf (scmp_filter_ctx ctx, صحيح fd, libcrun_error_t *err)
{
  cleanup_free حرف *bpf = عدم;
  size_t len = 0;
  صحيح ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, عدم, &len);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  وإلا
  cleanup_close صحيح pipe_r = -1;
  cleanup_close صحيح pipe_w = -1;
  صحيح p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  إذا (UNLIKELY (len >= (size_t) ret))
    أرجع crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  أرجع امن_write (fd, "seccomp.bpf", bpf, len, err);
}
#پایان_اگر /* HAVE_SECCOMP */

ساكن فراغ
make_lowercase (حرف *str)
{
  طالما (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

ساكن فراغ
cleanup_seccompp (فراغ *p)
{
#اگر_تعریف_شده HAVE_SECCOMP
  scmp_filter_ctx *ctx = (فراغ **) p;
  إذا (*ctx)
    seccomp_release (*ctx);
#پایان_اگر
}
#تعریف_کن cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

صحيح
libcrun_apply_seccomp (صحيح infd, صحيح listener_receiver_fd, ثابت حرف *receiver_fd_payload,
                       size_t receiver_fd_payload_len, حرف **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#اگر_تعریف_شده HAVE_SECCOMP
  cleanup_mmap بنية libcrun_mmap_s *mmap_region = عدم;
  cleanup_close صحيح listener_fd = -1;
  cleanup_pid صحيح helper_proc = -1;
  بنية sock_fprog seccomp_filter;
  cleanup_free حرف *bpf = عدم;
  بیعلامت صحيح flags = 0;
  size_t len;
  صحيح ret;

  إذا (infd < 0)
    أرجع 0;

  إذا (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    أرجع crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  إذا (seccomp_flags == عدم)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  وإلا
    {
      size_t i = 0;
      لكل (i = 0; i < seccomp_flags_len; i++)
        {
          إذا (seccomp_flags[i] == عدم)
            أرجع crun_make_error (err, EINVAL, "seccomp flag is not specified");

          إذا (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          وإلا إذا (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          وإلا إذا (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          وإلا إذا (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          وإلا
            أرجع crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (بنية sock_filter *) bpf;

  إذا (listener_receiver_fd >= 0)
    {
      cleanup_close صحيح memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  وإلا
      أرجع crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      إذا (UNLIKELY (memfd < 0))
        أرجع crun_make_error (err, errno, "memfd_create");
#  وإلا
      أرجع crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, حجم (atomic_int));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, عدم, حجم (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, عدم);
      إذا (UNLIKELY (helper_proc < 0))
        أرجع crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      إذا (helper_proc == 0)
        {
          صحيح fd, timeout = 0;

          prctl (PR_مجموعه_PDEATHSIG, SIGKILL);
          لكل (;;)
            {
              fd = *fd_received;
              إذا (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  إذا (timeout++ > 5000)
                    امن_exit (EINVAL);
                  استمر;
                }
#  إذا ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              اكسر;
            }
          ret = send_fd_مقصد_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          إذا (UNLIKELY (ret < 0))
            امن_exit (crun_error_get_errno (err));
          امن_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  إذا (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      إذا (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  إذا (listener_receiver_fd >= 0)
    {
      atomic_int *fd_مقصد_send = mmap_region->addr;
      صحيح وضعیت = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_مقصد_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &وضعیت, 0);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_وضعیت (وضعیت);
      إذا (ret != 0)
        أرجع crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  أرجع 0;
#وإلا
  أرجع 0;
#پایان_اگر
}

ساكن bool
seccomp_action_supports_errno (ثابت حرف *action)
{
  أرجع strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
ساكن صحيح
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, بیعلامت صحيح seccomp_gen_options, seccomp_checksum_t بیرون, libcrun_error_t *err)
{
  blake3_hasher hasher;
  بیعلامت حرف hash[32];
  بنية utsname utsbuf;
  size_t i;
  صحيح ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#تعریف_کن PROCESS_STRING(X)                                           \
  افعل                                                                \
    {                                                               \
      ثابت حرف *str_ = (X);                                       \
      بیعلامت حرف present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? طول_رشته (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, حجم (present_)); \
      blake3_hasher_update (&hasher, &len_, حجم (len_));         \
      إذا (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } طالما (0)
#تعریف_کن PROCESS_داده(X)                                   \
  افعل                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), حجم ((X))); \
  } طالما (0)
#تعریف_کن PROCESS_OPTIONAL_داده(PRESENT, X)                           \
  افعل                                                                \
    {                                                               \
      بیعلامت حرف present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, حجم (present_)); \
      إذا (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), حجم ((X)));         \
  } طالما (0)
#تعریف_کن PROCESS_شمار(X)                                        \
  افعل                                                            \
    {                                                           \
      size_t شمار_2 = (X);                                      \
      blake3_hasher_update (&hasher, &شمار_2, حجم (شمار_2)); \
  } طالما (0)

  PROCESS_STRING (PACKAGE_نسخه);

#اگر_تعریف_شده HAVE_SECCOMP
  {
    ثابت بنية scmp_نسخه *نسخه = seccomp_نسخه ();

    PROCESS_داده (نسخه->major);
    PROCESS_داده (نسخه->minor);
    PROCESS_داده (نسخه->micro);
  }
#پایان_اگر

  memset (&utsbuf, 0, حجم (utsbuf));
  ret = uname (&utsbuf);
  إذا (UNLIKELY (ret != 0))
    أرجع crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.نسخه);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_داده (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_داده (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_شمار (seccomp->flags_len);
  لكل (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_شمار (seccomp->architectures_len);
  لكل (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_شمار (seccomp->syscalls_len);
  لكل (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_داده (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_شمار (seccomp->syscalls[i]->نامها_len);
      لكل (j = 0; j < seccomp->syscalls[i]->نامها_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->نامها[j]);

      PROCESS_شمار (seccomp->syscalls[i]->آرگومانها_len);
      لكل (j = 0; j < seccomp->syscalls[i]->آرگومانها_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_داده (seccomp->syscalls[i]->آرگومانها[j]->شاخص);
          PROCESS_داده (seccomp->syscalls[i]->آرگومانها[j]->مقدار);
          PROCESS_داده (seccomp->syscalls[i]->آرگومانها[j]->مقدار_two);
          PROCESS_STRING (seccomp->syscalls[i]->آرگومانها[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, حجم (hash));

  لكل (i = 0; i < 32; i++)
    {
      size_t مانده = حجم (seccomp_checksum_t) - i * 2;
      ret = snprintf (&بیرون[i * 2], مانده, "%02x", hash[i]);
      إذا (UNLIKELY (ret != 2))
        أرجع crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  بیرون[64] = 0;

#لغو_تعریف PROCESS_STRING
#لغو_تعریف PROCESS_داده
#لغو_تعریف PROCESS_OPTIONAL_داده
#لغو_تعریف PROCESS_شمار
  أرجع 1;
}

ساكن صحيح
open_rundir_dirfd (ثابت حرف *state_ریشه, libcrun_error_t *err)
{
  cleanup_free حرف *dir = عدم;
  صحيح dirfd;
  صحيح ret;

  ret = libcrun_get_state_directory (&dir, state_ریشه, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dirfd = موقت_FAILURE_RETRY (open (dir, O_مسیر | O_DIRECTORY | O_CLOEXEC));
  إذا (UNLIKELY (dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", dir);

  أرجع dirfd;
}

بنية cache_مدخل
{
  seccomp_checksum_t checksum;
  بنية timespec atime;
};

ساكن صحيح
compare_entries_by_atime (ثابت فراغ *a, ثابت فراغ *b)
{
  ثابت بنية cache_مدخل *مدخل_a = a;
  ثابت بنية cache_مدخل *مدخل_b = b;
  صحيح ret;

  ret = مدخل_a->atime.tv_sec - مدخل_b->atime.tv_sec;
  إذا (ret)
    أرجع ret;

  أرجع مدخل_a->atime.tv_nsec - مدخل_b->atime.tv_nsec;
}

ساكن صحيح
evict_cache (صحيح ریشه_dfd, libcrun_error_t *err)
{
  cleanup_close صحيح cache_dir_fd = -1;
  off_t cache_dir_inode_max_اندازه;
  بنية stat st;
  صحيح ret;

  ret = موقت_FAILURE_RETRY (fstat (ریشه_dfd, &st));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_اندازه = get_cache_dir_inode_max_اندازه (st.st_size);

  cache_dir_fd = موقت_FAILURE_RETRY (openat (ریشه_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  إذا (UNLIKELY (cache_dir_fd < 0))
    {
      إذا (errno == ENOENT)
        أرجع 0;
      أرجع crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = موقت_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  إذا (ret == 0 && st.st_size > cache_dir_inode_max_اندازه)
    {
      cleanup_free بنية cache_مدخل *entries = عدم;
      cleanup_dir DIR *d = عدم;
      size_t i, n_entries = 0;
      بنية dirent *de;
      صحيح dfd;

      d = fdopendir (cache_dir_fd);
      إذا (d == عدم)
        أرجع crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      طالما ((de = readdir (d)))
        {
          إذا (de->d_name[0] == '.')
            استمر;

          ret = موقت_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          إذا (UNLIKELY (ret < 0))
            استمر;

          /* The cache file is already used, it is pointless to free it.  */
          إذا (st.st_nlink > 1)
            استمر;

          /* If the sticky bit is set, then ignore it.  */
          إذا (st.st_mode & S_ISVTX)
            استمر;

          إذا (طول_رشته (de->d_name) != حجم (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              استمر;
            }
          وإلا
            {
              entries = xrealloc (entries, حجم (بنية cache_مدخل) * (n_entries + 1));

              کپی_حافظه (entries[n_entries].checksum, de->d_name, حجم (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, حجم (بنية cache_مدخل), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      لكل (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  أرجع 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
ساكن صحيح
open_seccomp_bpf_dirfd (libcrun_container_t *container, حرف **rel_مسیر, libcrun_error_t *err)
{
  صحيح ret;
  صحيح dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_ریشه : عدم, err);
  إذا (UNLIKELY (dirfd < 0))
    أرجع dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_مسیر, err, container->context->id, "seccomp.bpf", عدم);
  إذا (UNLIKELY (ret < 0))
    {
      موقت_FAILURE_RETRY (close (dirfd));
      أرجع ret;
    }

  أرجع dirfd;
}

ساكن صحيح
store_seccomp_cache (بنية libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free حرف *src_مسیر = عدم;
  cleanup_free حرف *dest_مسیر = عدم;
  cleanup_close صحيح dirfd = -1;
  صحيح ret;

  إذا (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    أرجع 0;

  إذا (is_خالی_string (ctx->checksum))
    أرجع 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_مسیر, err);
  إذا (UNLIKELY (dirfd < 0))
    أرجع dirfd;

  ret = append_paths (&dest_مسیر, err, SECCOMP_CACHE_DIR, ctx->checksum, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = evict_cache (dirfd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = linkat (dirfd, src_مسیر, dirfd, dest_مسیر, 0);
  إذا (UNLIKELY (ret < 0 && errno != EEXIST))
    أرجع crun_make_error (err, errno, "link `%s` to `%s`", src_مسیر, dest_مسیر);
  أرجع 0;
}

ساكن مضمن runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (بنية libcrun_seccomp_gen_ctx_s *ctx)
{
  إذا (ctx->container == عدم || ctx->container->container_def == عدم
      || ctx->container->container_def->linux == عدم)
    أرجع 0;

  أرجع ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
ساكن مضمن bool
has_seccomp_configuration (بنية libcrun_seccomp_gen_ctx_s *ctx)
{
  إذا (ctx->container == عدم || ctx->container->container_def == عدم
      || ctx->container->container_def->linux == عدم)
    أرجع false;

  أرجع ctx->container->container_def->linux->seccomp != عدم
         || پیدا_کن_annotation (ctx->container, "run.oci.seccomp_bpf_data") != عدم;
}

ساكن صحيح
پیدا_کن_درون_cache (بنية libcrun_seccomp_gen_ctx_s *ctx, صحيح dirfd, ثابت حرف *dest_مسیر, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free حرف *cache_file_مسیر = عدم;
  صحيح ret;

  إذا (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    أرجع 0;

  seccomp = get_seccomp_configuration (ctx);
  إذا (seccomp == عدم)
    أرجع 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&cache_file_مسیر, err, SECCOMP_CACHE_DIR, ctx->checksum, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = موقت_FAILURE_RETRY (linkat (dirfd, cache_file_مسیر, dirfd, dest_مسیر, 0));
  إذا (UNLIKELY (ret < 0 && errno != ENOENT))
    أرجع crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_مسیر, dest_مسیر);

  *created = ret == 0;

  أرجع 0;
}

صحيح
libcrun_generate_seccomp (بنية libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#اگر_تعریف_شده HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  صحيح ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = عدم;
  صحيح default_errno_مقدار = EPERM;
  uint32_t action, default_action;
  ثابت حرف *def_action = عدم;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  إذا (gen_ctx->from_cache)
    أرجع 0;

  إذا (gen_ctx->container == عدم || gen_ctx->container->container_def == عدم || gen_ctx->container->container_def->linux == عدم)
    أرجع 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  إذا (seccomp == عدم)
    أرجع 0;

  /* seccomp not available.  */
  إذا (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    أرجع crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  إذا (def_action == عدم)
    أرجع crun_make_error (err, 0, "seccomp misses the default action");

  إذا (seccomp->default_errno_ret_present)
    {
      إذا (! seccomp_action_supports_errno (def_action))
        أرجع crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_مقدار = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_مقدار, &default_action, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ctx = seccomp_init (default_action);
  إذا (ctx == عدم)
    أرجع crun_make_error (err, 0, "error seccomp_init");

  لكل (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      ثابت حرف *arch = seccomp->architectures[i];
      حرف *انتها, lowercase_arch[32] = {
        0,
      };

      إذا (arch == عدم)
        أرجع crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      إذا (has_پیشوند (arch, "SCMP_ARCH_"))
        arch += 10;
      انتها = stpncpy (lowercase_arch, arch, حجم (lowercase_arch) - 1);
      *انتها = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      إذا (arch_token == 0)
        أرجع crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  وإلا
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_جمع_کن (ctx, arch_token);
      إذا (ret < 0 && ret != -EEXIST)
        أرجع crun_make_error (err, -ret, "seccomp adding architecture");
    }

  لكل (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool مضرب_آرگومانها = false;
      صحيح errno_ret = EPERM;

      إذا (seccomp->syscalls[i]->errno_ret_present)
        {
          إذا (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            أرجع crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (action == default_action)
        استمر;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      إذا (seccomp->syscalls[i]->آرگومانها != عدم)
        {
          uint32_t شمار[6] = {};
          size_t k;

          لكل (k = 0; k < seccomp->syscalls[i]->آرگومانها_len; k++)
            {
              uint32_t شاخص = seccomp->syscalls[i]->آرگومانها[k]->شاخص;

              إذا (شاخص >= 6)
                أرجع crun_make_error (err, 0, "invalid seccomp index `%u`", شاخص);

              شمار[شاخص]++;
              إذا (شمار[شاخص] > 1)
                مضرب_آرگومانها = true;
            }
        }

      لكل (j = 0; j < seccomp->syscalls[i]->نامها_len; j++)
        {
          صحيح syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->نامها[j]);

          إذا (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              إذا (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                أرجع crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->نامها[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->نامها[j]);
              استمر;
            }

          إذا (seccomp->syscalls[i]->آرگومانها == عدم)
            {
              ret = seccomp_قاعده_جمع_کن (ctx, action, syscall, 0);
              إذا (UNLIKELY (ret < 0))
                أرجع crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->نامها[j]);
            }
          وإلا
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              إذا (مضرب_آرگومانها)
                {
                  size_t r;

                  لكل (r = 0; r < seccomp->syscalls[i]->آرگومانها_len; r++)
                    {
                      بنية scmp_arg_cmp arg_cmp;
                      حرف *op = seccomp->syscalls[i]->آرگومانها[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->آرگومانها[r]->شاخص;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      إذا (UNLIKELY (ret < 0))
                        أرجع ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->آرگومانها[r]->مقدار;
                      arg_cmp.datum_b = seccomp->syscalls[i]->آرگومانها[r]->مقدار_two;

                      ret = seccomp_قاعده_جمع_کن_array (ctx, action, syscall, 1, &arg_cmp);
                      إذا (UNLIKELY (ret < 0))
                        أرجع crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              وإلا
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  ثابت size_t آرگومانها_len = seccomp->syscalls[i]->آرگومانها_len;
                  بنية scmp_arg_cmp arg_cmp[6];

                  لكل (k = 0; k < آرگومانها_len; k++)
                    {
                      حرف *op = seccomp->syscalls[i]->آرگومانها[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->آرگومانها[k]->شاخص;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      إذا (UNLIKELY (ret < 0))
                        أرجع ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->آرگومانها[k]->مقدار;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->آرگومانها[k]->مقدار_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_قاعده_جمع_کن_array (ctx, action, syscall, آرگومانها_len, آرگومانها_len ? arg_cmp : عدم);
                  إذا (UNLIKELY (ret < 0))
                    أرجع crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  إذا (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      أرجع store_seccomp_cache (gen_ctx, err);
    }

  أرجع 0;
#وإلا
  أرجع 0;
#پایان_اگر
}

صحيح
libcrun_copy_seccomp (بنية libcrun_seccomp_gen_ctx_s *gen_ctx, ثابت حرف *b64_bpf, libcrun_error_t *err)
{
  cleanup_free حرف *bpf_داده = عدم;
  size_t اندازه = 0;
  size_t درون_اندازه;
  صحيح consumed;
  صحيح ret;

  درون_اندازه = طول_رشته (b64_bpf);
  bpf_داده = xmalloc (درون_اندازه + 1);

  consumed = base64_decode (b64_bpf, درون_اندازه, bpf_داده, درون_اندازه, &اندازه);
  إذا (UNLIKELY (consumed != (صحيح) درون_اندازه))
    أرجع crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = امن_write (gen_ctx->fd, "seccomp fd", bpf_داده, اندازه, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

صحيح
libcrun_open_seccomp_bpf (بنية libcrun_seccomp_gen_ctx_s *ctx, صحيح *fd, libcrun_error_t *err)
{
  cleanup_close صحيح dirfd = -1;
  cleanup_free حرف *dest_مسیر = عدم;
  libcrun_container_t *container = ctx->container;
  صحيح ret;

  ctx->fd = *fd = -1;

  إذا (container == عدم || container->context == عدم)
    أرجع crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_مسیر, err);
  إذا (UNLIKELY (dirfd < 0))
    أرجع dirfd;

  إذا (ctx->create)
    {
      bool created = false;

      ret = پیدا_کن_درون_cache (ctx, dirfd, dest_مسیر, &created, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (created)
        {
          ctx->from_cache = true;
          اذهب open_existing_file;
        }

      ret = موقت_FAILURE_RETRY (openat (dirfd, dest_مسیر, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  وإلا
    {
    open_existing_file:
      ret = موقت_FAILURE_RETRY (openat (dirfd, dest_مسیر, O_CLOEXEC | O_RDONLY));
      إذا (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          إذا (errno == ENOENT && ! has_seccomp_configuration (ctx))
            أرجع 0;
          أرجع crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  أرجع 0;
}
