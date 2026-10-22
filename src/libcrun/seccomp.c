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

#تعریف_کرو GNU_ماخذ

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

#اگر_تعریف_شدہ HAVE_BLAKE3
#  include <blake3.h>
#ورنہ
#  include "blake3/blake3.h"
#اگر_ختم

#اگر HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int متلون صحیح
#  endif
#اگر_ختم

#اگر_تعریف_شدہ HAVE_SECCOMP
#  include <seccomp.h>
#اگر_ختم
#شامل <linux/seccomp.h>
#شامل <linux/filter.h>
#شامل <sys/prctl.h>
#شامل <sys/syscall.h>

#اگر_غیر_تعریف_شدہ __NR_seccomp
#  define __NR_seccomp 0xffff // seccomp syscall number unknown for this architecture
#اگر_ختم

#اگر_غیر_تعریف_شدہ SECCOMP_SET_MODE_STRICT
#  define SECCOMP_SET_MODE_STRICT 0
#اگر_ختم

#اگر_غیر_تعریف_شدہ SECCOMP_SET_MODE_FILTER
#  define SECCOMP_SET_MODE_FILTER 1
#اگر_ختم

#اگر_غیر_تعریف_شدہ SECCOMP_FILTER_FLAG_TSYNC
#  define SECCOMP_FILTER_FLAG_TSYNC (1UL << 0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ SECCOMP_FILTER_FLAG_LOG
#  define SECCOMP_FILTER_FLAG_LOG (1UL << 1)
#اگر_ختم

#اگر_غیر_تعریف_شدہ SECCOMP_FILTER_FLAG_SPEC_ALLOW
#  define SECCOMP_FILTER_FLAG_SPEC_ALLOW (1UL << 2)
#اگر_ختم

#اگر_غیر_تعریف_شدہ SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV
#  define SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV (1UL << 5)
#اگر_ختم

#تعریف_کرو SECCOMP_CACHE_DIR ".cache/seccomp"

/* On Linux 5.19.15 x86_64, tmpfs reports 20 bytes + 20 bytes for each entry, so allow
   at least 32 entries.  */
#تعریف_کرو MIN_CACHE_DIR_INODE_سائز (20 + 32 * 20)

/* Heuristic to avoid the cache directory grows indefinitely.  The inode size for
   the directory is proportional to the number of entries it contains and it is
   cheaper to compute than iterating the cache directory.  */
ساکن اندرونی off_t
get_cache_dir_inode_max_سائز (off_t سائز_rundir)
{
  واپس سائز_rundir * 3 + MIN_CACHE_DIR_INODE_سائز;
}

ساکن صحیح
syscall_seccomp (غیرعلامتی صحیح operation, غیرعلامتی صحیح flags, خالی *دلائل)
{
  واپس (صحیح) syscall (__NR_seccomp, operation, flags, دلائل);
}

#اگر_تعریف_شدہ HAVE_SECCOMP
ساکن صحیح
get_seccomp_operator (ثابت حرف *name, شمار scmp_compare *op, libcrun_error_t *err)
{
  ساکن ثابت ساخت
  {
    ثابت حرف *name;
    شمار scmp_compare op;
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

  اگر (! has_سابقہ (p, "SCMP_CMP_"))
    جاؤ fail;

  p += حجم ("SCMP_CMP_") - 1;

  برائے (i = 0; i < حجم (operators) / حجم (operators[0]); i++)
    اگر (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        واپس 0;
      }

fail:
  واپس crun_make_error (err, 0, "seccomp get operator `%s`", name);
}

ساکن صحیح
get_seccomp_action (ثابت حرف *name, صحیح errno_ret, uint32_t *action, libcrun_error_t *err)
{
  ساکن ثابت ساخت
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

  اگر (! has_سابقہ (p, "SCMP_ACT_"))
    جاؤ fail;

  p += حجم ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  اگر (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      واپس 0;
    }
  اگر (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      واپس 0;
    }

  برائے (i = 0; i < حجم (actions) / حجم (actions[0]); i++)
    اگر (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        واپس 0;
      }

fail:
  واپس crun_make_error (err, 0, "seccomp get action `%s`", name);
}

/* seccomp_export_bpf issues a single write and succeeds even when it is
   short, so retrieve the whole program first and write it with safe_write,
   which fails on ENOSPC or EFBIG instead of leaving a truncated filter.  */
ساکن صحیح
export_bpf (scmp_filter_ctx ctx, صحیح fd, libcrun_error_t *err)
{
  cleanup_free حرف *bpf = تہی;
  size_t len = 0;
  صحیح ret;

#  ifdef HAVE_SECCOMP_EXPORT_BPF_MEM
  ret = seccomp_export_bpf_mem (ctx, تہی, &len);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  ورنہ
  cleanup_close صحیح pipe_r = -1;
  cleanup_close صحیح pipe_w = -1;
  صحیح p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  اگر (UNLIKELY (len >= (size_t) ret))
    واپس crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  واپس محفوظ_write (fd, "seccomp.bpf", bpf, len, err);
}
#اگر_ختم /* HAVE_SECCOMP */

ساکن خالی
make_lowercase (حرف *str)
{
  جب (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

ساکن خالی
cleanup_seccompp (خالی *p)
{
#اگر_تعریف_شدہ HAVE_SECCOMP
  scmp_filter_ctx *ctx = (خالی **) p;
  اگر (*ctx)
    seccomp_release (*ctx);
#اگر_ختم
}
#تعریف_کرو cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

صحیح
libcrun_apply_seccomp (صحیح infd, صحیح listener_receiver_fd, ثابت حرف *receiver_fd_payload,
                       size_t receiver_fd_payload_len, حرف **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#اگر_تعریف_شدہ HAVE_SECCOMP
  cleanup_mmap ساخت libcrun_mmap_s *mmap_region = تہی;
  cleanup_close صحیح listener_fd = -1;
  cleanup_pid صحیح helper_proc = -1;
  ساخت sock_fprog seccomp_filter;
  cleanup_free حرف *bpf = تہی;
  غیرعلامتی صحیح flags = 0;
  size_t len;
  صحیح ret;

  اگر (infd < 0)
    واپس 0;

  اگر (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    واپس crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  اگر (seccomp_flags == تہی)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  ورنہ
    {
      size_t i = 0;
      برائے (i = 0; i < seccomp_flags_len; i++)
        {
          اگر (seccomp_flags[i] == تہی)
            واپس crun_make_error (err, EINVAL, "seccomp flag is not specified");

          اگر (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          ورنہ اگر (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          ورنہ اگر (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          ورنہ اگر (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          ورنہ
            واپس crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (ساخت sock_filter *) bpf;

  اگر (listener_receiver_fd >= 0)
    {
      cleanup_close صحیح memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  ورنہ
      واپس crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      اگر (UNLIKELY (memfd < 0))
        واپس crun_make_error (err, errno, "memfd_create");
#  ورنہ
      واپس crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, حجم (atomic_int));
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, تہی, حجم (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, تہی);
      اگر (UNLIKELY (helper_proc < 0))
        واپس crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      اگر (helper_proc == 0)
        {
          صحیح fd, timeout = 0;

          prctl (PR_سیٹ_PDEATHSIG, SIGKILL);
          برائے (;;)
            {
              fd = *fd_received;
              اگر (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  اگر (timeout++ > 5000)
                    محفوظ_exit (EINVAL);
                  جاری;
                }
#  اگر ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              توڑو;
            }
          ret = send_fd_مقصد_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          اگر (UNLIKELY (ret < 0))
            محفوظ_exit (crun_error_get_errno (err));
          محفوظ_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  اگر (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      اگر (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  اگر (listener_receiver_fd >= 0)
    {
      atomic_int *fd_مقصد_send = mmap_region->addr;
      صحیح حالت_2 = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_مقصد_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &حالت_2, 0);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_حالت (حالت_2);
      اگر (ret != 0)
        واپس crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  واپس 0;
#ورنہ
  واپس 0;
#اگر_ختم
}

ساکن bool
seccomp_action_supports_errno (ثابت حرف *action)
{
  واپس strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
ساکن صحیح
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, غیرعلامتی صحیح seccomp_gen_options, seccomp_checksum_t برآمد, libcrun_error_t *err)
{
  blake3_hasher hasher;
  غیرعلامتی حرف hash[32];
  ساخت utsname utsbuf;
  size_t i;
  صحیح ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#تعریف_کرو PROCESS_STRING(X)                                           \
  کرو                                                                \
    {                                                               \
      ثابت حرف *str_ = (X);                                       \
      غیرعلامتی حرف present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? لڑی_لمبائی (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, حجم (present_)); \
      blake3_hasher_update (&hasher, &len_, حجم (len_));         \
      اگر (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } جب (0)
#تعریف_کرو PROCESS_ڈیٹا(X)                                   \
  کرو                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), حجم ((X))); \
  } جب (0)
#تعریف_کرو PROCESS_OPTIONAL_ڈیٹا(PRESENT, X)                           \
  کرو                                                                \
    {                                                               \
      غیرعلامتی حرف present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, حجم (present_)); \
      اگر (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), حجم ((X)));         \
  } جب (0)
#تعریف_کرو PROCESS_گنتی(X)                                        \
  کرو                                                            \
    {                                                           \
      size_t گنتی_2 = (X);                                      \
      blake3_hasher_update (&hasher, &گنتی_2, حجم (گنتی_2)); \
  } جب (0)

  PROCESS_STRING (PACKAGE_نسخہ);

#اگر_تعریف_شدہ HAVE_SECCOMP
  {
    ثابت ساخت scmp_نسخہ *نسخہ = seccomp_نسخہ ();

    PROCESS_ڈیٹا (نسخہ->major);
    PROCESS_ڈیٹا (نسخہ->minor);
    PROCESS_ڈیٹا (نسخہ->micro);
  }
#اگر_ختم

  memset (&utsbuf, 0, حجم (utsbuf));
  ret = uname (&utsbuf);
  اگر (UNLIKELY (ret != 0))
    واپس crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.نسخہ);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_ڈیٹا (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_ڈیٹا (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_گنتی (seccomp->flags_len);
  برائے (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_گنتی (seccomp->architectures_len);
  برائے (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_گنتی (seccomp->syscalls_len);
  برائے (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_ڈیٹا (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_گنتی (seccomp->syscalls[i]->نام_len);
      برائے (j = 0; j < seccomp->syscalls[i]->نام_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->نام[j]);

      PROCESS_گنتی (seccomp->syscalls[i]->دلائل_len);
      برائے (j = 0; j < seccomp->syscalls[i]->دلائل_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_ڈیٹا (seccomp->syscalls[i]->دلائل[j]->اشاریہ);
          PROCESS_ڈیٹا (seccomp->syscalls[i]->دلائل[j]->قدر);
          PROCESS_ڈیٹا (seccomp->syscalls[i]->دلائل[j]->قدر_two);
          PROCESS_STRING (seccomp->syscalls[i]->دلائل[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, حجم (hash));

  برائے (i = 0; i < 32; i++)
    {
      size_t بقایا = حجم (seccomp_checksum_t) - i * 2;
      ret = snprintf (&برآمد[i * 2], بقایا, "%02x", hash[i]);
      اگر (UNLIKELY (ret != 2))
        واپس crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  برآمد[64] = 0;

#تعریف_منسوخ PROCESS_STRING
#تعریف_منسوخ PROCESS_ڈیٹا
#تعریف_منسوخ PROCESS_OPTIONAL_ڈیٹا
#تعریف_منسوخ PROCESS_گنتی
  واپس 1;
}

ساکن صحیح
open_rundir_dirfd (ثابت حرف *state_جڑ, libcrun_error_t *err)
{
  cleanup_free حرف *dir = تہی;
  صحیح dirfd;
  صحیح ret;

  ret = libcrun_get_state_directory (&dir, state_جڑ, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  dirfd = عارضی_FAILURE_RETRY (open (dir, O_راستہ | O_DIRECTORY | O_CLOEXEC));
  اگر (UNLIKELY (dirfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", dir);

  واپس dirfd;
}

ساخت cache_اندراج
{
  seccomp_checksum_t checksum;
  ساخت timespec atime;
};

ساکن صحیح
compare_entries_by_atime (ثابت خالی *a, ثابت خالی *b)
{
  ثابت ساخت cache_اندراج *اندراج_a = a;
  ثابت ساخت cache_اندراج *اندراج_b = b;
  صحیح ret;

  ret = اندراج_a->atime.tv_sec - اندراج_b->atime.tv_sec;
  اگر (ret)
    واپس ret;

  واپس اندراج_a->atime.tv_nsec - اندراج_b->atime.tv_nsec;
}

ساکن صحیح
evict_cache (صحیح جڑ_dfd, libcrun_error_t *err)
{
  cleanup_close صحیح cache_dir_fd = -1;
  off_t cache_dir_inode_max_سائز;
  ساخت stat st;
  صحیح ret;

  ret = عارضی_FAILURE_RETRY (fstat (جڑ_dfd, &st));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_سائز = get_cache_dir_inode_max_سائز (st.st_size);

  cache_dir_fd = عارضی_FAILURE_RETRY (openat (جڑ_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  اگر (UNLIKELY (cache_dir_fd < 0))
    {
      اگر (errno == ENOENT)
        واپس 0;
      واپس crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = عارضی_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  اگر (ret == 0 && st.st_size > cache_dir_inode_max_سائز)
    {
      cleanup_free ساخت cache_اندراج *entries = تہی;
      cleanup_dir DIR *d = تہی;
      size_t i, n_entries = 0;
      ساخت dirent *de;
      صحیح dfd;

      d = fdopendir (cache_dir_fd);
      اگر (d == تہی)
        واپس crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      جب ((de = readdir (d)))
        {
          اگر (de->d_name[0] == '.')
            جاری;

          ret = عارضی_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          اگر (UNLIKELY (ret < 0))
            جاری;

          /* The cache file is already used, it is pointless to free it.  */
          اگر (st.st_nlink > 1)
            جاری;

          /* If the sticky bit is set, then ignore it.  */
          اگر (st.st_mode & S_ISVTX)
            جاری;

          اگر (لڑی_لمبائی (de->d_name) != حجم (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              جاری;
            }
          ورنہ
            {
              entries = xrealloc (entries, حجم (ساخت cache_اندراج) * (n_entries + 1));

              حافظہ_نقل (entries[n_entries].checksum, de->d_name, حجم (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, حجم (ساخت cache_اندراج), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      برائے (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  واپس 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
ساکن صحیح
open_seccomp_bpf_dirfd (libcrun_container_t *container, حرف **rel_راستہ, libcrun_error_t *err)
{
  صحیح ret;
  صحیح dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_جڑ : تہی, err);
  اگر (UNLIKELY (dirfd < 0))
    واپس dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_راستہ, err, container->context->id, "seccomp.bpf", تہی);
  اگر (UNLIKELY (ret < 0))
    {
      عارضی_FAILURE_RETRY (close (dirfd));
      واپس ret;
    }

  واپس dirfd;
}

ساکن صحیح
store_seccomp_cache (ساخت libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free حرف *src_راستہ = تہی;
  cleanup_free حرف *dest_راستہ = تہی;
  cleanup_close صحیح dirfd = -1;
  صحیح ret;

  اگر (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    واپس 0;

  اگر (is_خالی_خانے_string (ctx->checksum))
    واپس 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_راستہ, err);
  اگر (UNLIKELY (dirfd < 0))
    واپس dirfd;

  ret = append_paths (&dest_راستہ, err, SECCOMP_CACHE_DIR, ctx->checksum, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = evict_cache (dirfd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = linkat (dirfd, src_راستہ, dirfd, dest_راستہ, 0);
  اگر (UNLIKELY (ret < 0 && errno != EEXIST))
    واپس crun_make_error (err, errno, "link `%s` to `%s`", src_راستہ, dest_راستہ);
  واپس 0;
}

ساکن اندرونی runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (ساخت libcrun_seccomp_gen_ctx_s *ctx)
{
  اگر (ctx->container == تہی || ctx->container->container_def == تہی
      || ctx->container->container_def->linux == تہی)
    واپس 0;

  واپس ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
ساکن اندرونی bool
has_seccomp_configuration (ساخت libcrun_seccomp_gen_ctx_s *ctx)
{
  اگر (ctx->container == تہی || ctx->container->container_def == تہی
      || ctx->container->container_def->linux == تہی)
    واپس false;

  واپس ctx->container->container_def->linux->seccomp != تہی
         || ڈھونڈو_annotation (ctx->container, "run.oci.seccomp_bpf_data") != تہی;
}

ساکن صحیح
ڈھونڈو_اندر_cache (ساخت libcrun_seccomp_gen_ctx_s *ctx, صحیح dirfd, ثابت حرف *dest_راستہ, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free حرف *cache_file_راستہ = تہی;
  صحیح ret;

  اگر (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    واپس 0;

  seccomp = get_seccomp_configuration (ctx);
  اگر (seccomp == تہی)
    واپس 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_paths (&cache_file_راستہ, err, SECCOMP_CACHE_DIR, ctx->checksum, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = عارضی_FAILURE_RETRY (linkat (dirfd, cache_file_راستہ, dirfd, dest_راستہ, 0));
  اگر (UNLIKELY (ret < 0 && errno != ENOENT))
    واپس crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_راستہ, dest_راستہ);

  *created = ret == 0;

  واپس 0;
}

صحیح
libcrun_generate_seccomp (ساخت libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#اگر_تعریف_شدہ HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  صحیح ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = تہی;
  صحیح default_errno_قدر = EPERM;
  uint32_t action, default_action;
  ثابت حرف *def_action = تہی;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  اگر (gen_ctx->from_cache)
    واپس 0;

  اگر (gen_ctx->container == تہی || gen_ctx->container->container_def == تہی || gen_ctx->container->container_def->linux == تہی)
    واپس 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  اگر (seccomp == تہی)
    واپس 0;

  /* seccomp not available.  */
  اگر (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    واپس crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  اگر (def_action == تہی)
    واپس crun_make_error (err, 0, "seccomp misses the default action");

  اگر (seccomp->default_errno_ret_present)
    {
      اگر (! seccomp_action_supports_errno (def_action))
        واپس crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_قدر = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_قدر, &default_action, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ctx = seccomp_init (default_action);
  اگر (ctx == تہی)
    واپس crun_make_error (err, 0, "error seccomp_init");

  برائے (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      ثابت حرف *arch = seccomp->architectures[i];
      حرف *انتہا, lowercase_arch[32] = {
        0,
      };

      اگر (arch == تہی)
        واپس crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      اگر (has_سابقہ (arch, "SCMP_ARCH_"))
        arch += 10;
      انتہا = stpncpy (lowercase_arch, arch, حجم (lowercase_arch) - 1);
      *انتہا = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      اگر (arch_token == 0)
        واپس crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  ورنہ
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_جوڑو (ctx, arch_token);
      اگر (ret < 0 && ret != -EEXIST)
        واپس crun_make_error (err, -ret, "seccomp adding architecture");
    }

  برائے (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool مضرب_دلائل = false;
      صحیح errno_ret = EPERM;

      اگر (seccomp->syscalls[i]->errno_ret_present)
        {
          اگر (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            واپس crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (action == default_action)
        جاری;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      اگر (seccomp->syscalls[i]->دلائل != تہی)
        {
          uint32_t گنتی[6] = {};
          size_t k;

          برائے (k = 0; k < seccomp->syscalls[i]->دلائل_len; k++)
            {
              uint32_t اشاریہ = seccomp->syscalls[i]->دلائل[k]->اشاریہ;

              اگر (اشاریہ >= 6)
                واپس crun_make_error (err, 0, "invalid seccomp index `%u`", اشاریہ);

              گنتی[اشاریہ]++;
              اگر (گنتی[اشاریہ] > 1)
                مضرب_دلائل = true;
            }
        }

      برائے (j = 0; j < seccomp->syscalls[i]->نام_len; j++)
        {
          صحیح syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->نام[j]);

          اگر (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              اگر (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                واپس crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->نام[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->نام[j]);
              جاری;
            }

          اگر (seccomp->syscalls[i]->دلائل == تہی)
            {
              ret = seccomp_قاعدہ_جوڑو (ctx, action, syscall, 0);
              اگر (UNLIKELY (ret < 0))
                واپس crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->نام[j]);
            }
          ورنہ
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              اگر (مضرب_دلائل)
                {
                  size_t r;

                  برائے (r = 0; r < seccomp->syscalls[i]->دلائل_len; r++)
                    {
                      ساخت scmp_arg_cmp arg_cmp;
                      حرف *op = seccomp->syscalls[i]->دلائل[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->دلائل[r]->اشاریہ;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      اگر (UNLIKELY (ret < 0))
                        واپس ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->دلائل[r]->قدر;
                      arg_cmp.datum_b = seccomp->syscalls[i]->دلائل[r]->قدر_two;

                      ret = seccomp_قاعدہ_جوڑو_array (ctx, action, syscall, 1, &arg_cmp);
                      اگر (UNLIKELY (ret < 0))
                        واپس crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              ورنہ
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  ثابت size_t دلائل_len = seccomp->syscalls[i]->دلائل_len;
                  ساخت scmp_arg_cmp arg_cmp[6];

                  برائے (k = 0; k < دلائل_len; k++)
                    {
                      حرف *op = seccomp->syscalls[i]->دلائل[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->دلائل[k]->اشاریہ;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      اگر (UNLIKELY (ret < 0))
                        واپس ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->دلائل[k]->قدر;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->دلائل[k]->قدر_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_قاعدہ_جوڑو_array (ctx, action, syscall, دلائل_len, دلائل_len ? arg_cmp : تہی);
                  اگر (UNLIKELY (ret < 0))
                    واپس crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  اگر (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      واپس store_seccomp_cache (gen_ctx, err);
    }

  واپس 0;
#ورنہ
  واپس 0;
#اگر_ختم
}

صحیح
libcrun_copy_seccomp (ساخت libcrun_seccomp_gen_ctx_s *gen_ctx, ثابت حرف *b64_bpf, libcrun_error_t *err)
{
  cleanup_free حرف *bpf_ڈیٹا = تہی;
  size_t سائز = 0;
  size_t اندر_سائز;
  صحیح consumed;
  صحیح ret;

  اندر_سائز = لڑی_لمبائی (b64_bpf);
  bpf_ڈیٹا = xmalloc (اندر_سائز + 1);

  consumed = base64_decode (b64_bpf, اندر_سائز, bpf_ڈیٹا, اندر_سائز, &سائز);
  اگر (UNLIKELY (consumed != (صحیح) اندر_سائز))
    واپس crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = محفوظ_write (gen_ctx->fd, "seccomp fd", bpf_ڈیٹا, سائز, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

صحیح
libcrun_open_seccomp_bpf (ساخت libcrun_seccomp_gen_ctx_s *ctx, صحیح *fd, libcrun_error_t *err)
{
  cleanup_close صحیح dirfd = -1;
  cleanup_free حرف *dest_راستہ = تہی;
  libcrun_container_t *container = ctx->container;
  صحیح ret;

  ctx->fd = *fd = -1;

  اگر (container == تہی || container->context == تہی)
    واپس crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_راستہ, err);
  اگر (UNLIKELY (dirfd < 0))
    واپس dirfd;

  اگر (ctx->create)
    {
      bool created = false;

      ret = ڈھونڈو_اندر_cache (ctx, dirfd, dest_راستہ, &created, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (created)
        {
          ctx->from_cache = true;
          جاؤ open_existing_file;
        }

      ret = عارضی_FAILURE_RETRY (openat (dirfd, dest_راستہ, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  ورنہ
    {
    open_existing_file:
      ret = عارضی_FAILURE_RETRY (openat (dirfd, dest_راستہ, O_CLOEXEC | O_RDONLY));
      اگر (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          اگر (errno == ENOENT && ! has_seccomp_configuration (ctx))
            واپس 0;
          واپس crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  واپس 0;
}
