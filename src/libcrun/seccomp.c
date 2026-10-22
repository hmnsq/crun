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

#ifdef HAVE_BLAKE3
#  include <blake3.h>
#אחרת
#  include "blake3/blake3.h"
#endif

#אם HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_int متطاير صحيح
#  endif
#endif

#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#ضمن <linux/seccomp.h>
#ضمن <linux/filter.h>
#ضمن <sys/prctl.h>
#ضمن <sys/syscall.h>

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
ساكن مضمن off_t
get_cache_dir_inode_max_size (off_t size_rundir)
{
  החזר size_rundir * 3 + MIN_CACHE_DIR_INODE_SIZE;
}

ساكن صحيح
syscall_seccomp (unsigned صحيح operation, unsigned صحيح flags, فراغ *args)
{
  החזר (صحيح) syscall (__NR_seccomp, operation, flags, args);
}

#ifdef HAVE_SECCOMP
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

  אם (! has_prefix (p, "SCMP_CMP_"))
    اذهب fail;

  p += حجم ("SCMP_CMP_") - 1;

  עבור (i = 0; i < حجم (operators) / حجم (operators[0]); i++)
    אם (strcmp (p, operators[i].name) == 0)
      {
        *op = operators[i].op;
        החזר 0;
      }

fail:
  החזר crun_make_error (err, 0, "seccomp get operator `%s`", name);
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

  אם (! has_prefix (p, "SCMP_ACT_"))
    اذهب fail;

  p += حجم ("SCMP_ACT_") - 1;

  /* These two embed errno_ret, so they cannot be looked up in the table.  */
  אם (strcmp (p, "ERRNO") == 0)
    {
      *action = SCMP_ACT_ERRNO (errno_ret);
      החזר 0;
    }
  אם (strcmp (p, "TRACE") == 0)
    {
      *action = SCMP_ACT_TRACE (errno_ret);
      החזר 0;
    }

  עבור (i = 0; i < حجم (actions) / حجم (actions[0]); i++)
    אם (strcmp (p, actions[i].name) == 0)
      {
        *action = actions[i].action;
        החזר 0;
      }

fail:
  החזר crun_make_error (err, 0, "seccomp get action `%s`", name);
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
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, -ret, "seccomp_export_bpf_mem");

  bpf = xmalloc (len);

  ret = seccomp_export_bpf_mem (ctx, bpf, &len);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, -ret, "seccomp_export_bpf_mem");
#  אחרת
  cleanup_close صحيح pipe_r = -1;
  cleanup_close صحيح pipe_w = -1;
  صحيح p[2];

  /* A pipe is not affected by the free space or by RLIMIT_FSIZE, the write
     is short only when it fills the pipe.  */
  ret = pipe2 (p, O_CLOEXEC | O_NONBLOCK);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "pipe");
  pipe_r = p[0];
  pipe_w = p[1];

  ret = seccomp_export_bpf (ctx, pipe_w);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, -ret, "seccomp_export_bpf");

  close_and_reset (&pipe_w);

  ret = read_all_fd (pipe_r, "seccomp pipe", &bpf, &len, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = fcntl (pipe_r, F_GETPIPE_SZ);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "fcntl (F_GETPIPE_SZ)");
  אם (UNLIKELY (len >= (size_t) ret))
    החזר crun_make_error (err, 0, "the seccomp filter does not fit in a pipe");
#  endif

  החזר safe_write (fd, "seccomp.bpf", bpf, len, err);
}
#endif /* HAVE_SECCOMP */

ساكن فراغ
make_lowercase (حرف *str)
{
  כלעוד (*str)
    {
      *str = tolower (*str);
      str++;
    }
}

ساكن فراغ
cleanup_seccompp (فراغ *p)
{
#ifdef HAVE_SECCOMP
  scmp_filter_ctx *ctx = (فراغ **) p;
  אם (*ctx)
    seccomp_release (*ctx);
#endif
}
#define cleanup_seccomp __attribute__ ((cleanup (cleanup_seccompp)))

صحيح
libcrun_apply_seccomp (صحيح infd, صحيح listener_receiver_fd, ثابت حرف *receiver_fd_payload,
                       size_t receiver_fd_payload_len, حرف **seccomp_flags, size_t seccomp_flags_len,
                       libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  cleanup_mmap بنية libcrun_mmap_s *mmap_region = عدم;
  cleanup_close صحيح listener_fd = -1;
  cleanup_pid صحيح helper_proc = -1;
  بنية sock_fprog seccomp_filter;
  cleanup_free حرف *bpf = عدم;
  unsigned صحيح flags = 0;
  size_t len;
  صحيح ret;

  אם (infd < 0)
    החזר 0;

  אם (UNLIKELY (lseek (infd, 0, SEEK_SET) == (off_t) -1))
    החזר crun_make_error (err, errno, "lseek");

  /* if no seccomp flag was specified use a sane default.  */
  אם (seccomp_flags == عدم)
    flags = SECCOMP_FILTER_FLAG_SPEC_ALLOW;
  אחרת
    {
      size_t i = 0;
      עבור (i = 0; i < seccomp_flags_len; i++)
        {
          אם (seccomp_flags[i] == عدم)
            החזר crun_make_error (err, EINVAL, "seccomp flag is not specified");

          אם (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_TSYNC") == 0)
            flags |= SECCOMP_FILTER_FLAG_TSYNC;
          אחרת אם (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_SPEC_ALLOW") == 0)
            flags |= SECCOMP_FILTER_FLAG_SPEC_ALLOW;
          אחרת אם (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_LOG") == 0)
            flags |= SECCOMP_FILTER_FLAG_LOG;
          אחרת אם (strcmp (seccomp_flags[i], "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") == 0)
            flags |= SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV;
          אחרת
            החזר crun_make_error (err, 0, "unknown seccomp option `%s`", seccomp_flags[i]);
        }
    }

  ret = read_all_fd (infd, "seccomp.bpf", &bpf, &len, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  seccomp_filter.len = len / 8;
  seccomp_filter.filter = (بنية sock_filter *) bpf;

  אם (listener_receiver_fd >= 0)
    {
      cleanup_close صحيح memfd = -1;
      atomic_int *fd_received;

#  ifdef SECCOMP_FILTER_FLAG_NEW_LISTENER
      flags |= SECCOMP_FILTER_FLAG_NEW_LISTENER;
#  אחרת
      החזר crun_make_error (err, 0, "the `SECCOMP_FILTER_FLAG_NEW_LISTENER` flag is not supported");
#  endif

#  ifdef HAVE_MEMFD_CREATE
      memfd = memfd_create ("seccomp-helper-memfd", O_RDWR | MFD_CLOEXEC);
      אם (UNLIKELY (memfd < 0))
        החזר crun_make_error (err, errno, "memfd_create");
#  אחרת
      החזר crun_make_error (err, ENOSYS, "memfd_create non supported");
#  endif

      ret = ftruncate (memfd, حجم (atomic_int));
      אם (UNLIKELY (ret < 0))
        החזר crun_make_error (err, errno, "ftruncate seccomp memfd");

      ret = libcrun_mmap (&mmap_region, عدم, حجم (atomic_int),
                          PROT_WRITE | PROT_READ, MAP_SHARED, memfd, 0, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      /* memfd is not needed anymore.  */
      close_and_reset (&memfd);

      fd_received = mmap_region->addr;
      *fd_received = -1;

      /* The helper process shares the fd table with the current process.  */
      helper_proc = syscall_clone (CLONE_FILES | SIGCHLD, عدم);
      אם (UNLIKELY (helper_proc < 0))
        החזר crun_make_error (err, errno, "clone seccomp listener helper process");

      /* helper process.  Wait that the seccomp listener fd is created then send it to the
         receiver fd.  We use the helper process since the seccomp profile could block the
         sendmsg syscall.   Its exit status is an errno value.  */
      אם (helper_proc == 0)
        {
          صحيح fd, timeout = 0;

          prctl (PR_SET_PDEATHSIG, SIGKILL);
          עבור (;;)
            {
              fd = *fd_received;
              אם (fd == -1)
                {
                  usleep (1000);

                  /* Do not wait longer than 5 seconds.  */
                  אם (timeout++ > 5000)
                    _safe_exit (EINVAL);
                  استمر;
                }
#  אם ! HAVE_STDATOMIC_H
              /* If stdatomic is not available, force a membarrier and read again.  */
              __sync_synchronize ();
              fd = *fd_received;
#  endif
              اكسر;
            }
          ret = send_fd_to_socket_with_payload (listener_receiver_fd, fd,
                                                receiver_fd_payload,
                                                receiver_fd_payload_len,
                                                err);
          אם (UNLIKELY (ret < 0))
            _safe_exit (crun_error_get_errno (err));
          _safe_exit (0);
        }
    }

  ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, flags, &seccomp_filter);
  אם (UNLIKELY (ret < 0))
    {
      /* If any of the flags is not supported, try again without specifying them:  */
      אם (errno == EINVAL && listener_receiver_fd < 0)
        ret = syscall_seccomp (SECCOMP_SET_MODE_FILTER, 0, &seccomp_filter);
      אם (UNLIKELY (ret < 0))
        החזר crun_make_error (err, errno, "seccomp (SECCOMP_SET_MODE_FILTER)");
    }

  אם (listener_receiver_fd >= 0)
    {
      atomic_int *fd_to_send = mmap_region->addr;
      صحيح status = 0;

      /* Write atomically the listener fd to the shared memory.  No syscalls are used between
         the seccomp listener creation and the write.  */
      *fd_to_send = listener_fd = ret;

      ret = waitpid_ignore_stopped (helper_proc, &status, 0);
      אם (UNLIKELY (ret < 0))
        החזר crun_make_error (err, errno, "waitpid seccomp listener helper process");

      ret = get_process_exit_status (status);
      אם (ret != 0)
        החזר crun_make_error (err, 0, "send listener fd `%d` to receiver", listener_fd);
    }

  החזר 0;
#אחרת
  החזר 0;
#endif
}

ساكن bool
seccomp_action_supports_errno (ثابت حرف *action)
{
  החזר strcmp (action, "SCMP_ACT_ERRNO") == 0
         || strcmp (action, "SCMP_ACT_TRACE") == 0;
}

/* Calculate a checksum for the specified seccomp configuration.

   Returns:
   < 0 in case of errors
   == 0 the checksum is supported and the value is in OUT.
 */
ساكن صحيح
calculate_seccomp_checksum (runtime_spec_schema_config_linux_seccomp *seccomp, unsigned صحيح seccomp_gen_options, seccomp_checksum_t out, libcrun_error_t *err)
{
  blake3_hasher hasher;
  unsigned حرف hash[32];
  بنية utsname utsbuf;
  size_t i;
  صحيح ret;

  blake3_hasher_init (&hasher);

  /* Every field is framed so that distinct configurations cannot hash to the
     same value: strings carry a presence byte and an explicit length, arrays
     carry their element count, and optional scalars carry a presence byte.
     Everything that influences the generated filter MUST be included here,
     otherwise a stale filter could be served from the cache.  */
#define PROCESS_STRING(X)                                           \
  עשה                                                                \
    {                                                               \
      ثابت حرف *str_ = (X);                                       \
      unsigned حرف present_ = str_ ? 1 : 0;                        \
      size_t len_ = str_ ? strlen (str_) : 0;                       \
      blake3_hasher_update (&hasher, &present_, حجم (present_)); \
      blake3_hasher_update (&hasher, &len_, حجم (len_));         \
      אם (str_)                                                     \
        blake3_hasher_update (&hasher, str_, len_);                 \
  } כלעוד (0)
#define PROCESS_DATA(X)                                   \
  עשה                                                      \
    {                                                     \
      blake3_hasher_update (&hasher, &(X), حجم ((X))); \
  } כלעוד (0)
#define PROCESS_OPTIONAL_DATA(PRESENT, X)                           \
  עשה                                                                \
    {                                                               \
      unsigned حرف present_ = (PRESENT) ? 1 : 0;                   \
      blake3_hasher_update (&hasher, &present_, حجم (present_)); \
      אם (PRESENT)                                                  \
        blake3_hasher_update (&hasher, &(X), حجم ((X)));         \
  } כלעוד (0)
#define PROCESS_COUNT(X)                                        \
  עשה                                                            \
    {                                                           \
      size_t count_ = (X);                                      \
      blake3_hasher_update (&hasher, &count_, حجم (count_)); \
  } כלעוד (0)

  PROCESS_STRING (PACKAGE_VERSION);

#ifdef HAVE_SECCOMP
  {
    ثابت بنية scmp_version *version = seccomp_version ();

    PROCESS_DATA (version->major);
    PROCESS_DATA (version->minor);
    PROCESS_DATA (version->micro);
  }
#endif

  memset (&utsbuf, 0, حجم (utsbuf));
  ret = uname (&utsbuf);
  אם (UNLIKELY (ret != 0))
    החזר crun_make_error (err, errno, "uname");

  PROCESS_STRING (utsbuf.release);
  PROCESS_STRING (utsbuf.version);
  PROCESS_STRING (utsbuf.machine);

  PROCESS_DATA (seccomp_gen_options);

  /* default_errno_ret is only honoured when present (absent defaults to EPERM).  */
  PROCESS_OPTIONAL_DATA (seccomp->default_errno_ret_present, seccomp->default_errno_ret);
  PROCESS_STRING (seccomp->default_action);

  PROCESS_COUNT (seccomp->flags_len);
  עבור (i = 0; i < seccomp->flags_len; i++)
    PROCESS_STRING (seccomp->flags[i]);

  PROCESS_COUNT (seccomp->architectures_len);
  עבור (i = 0; i < seccomp->architectures_len; i++)
    PROCESS_STRING (seccomp->architectures[i]);

  PROCESS_COUNT (seccomp->syscalls_len);
  עבור (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;

      PROCESS_STRING (seccomp->syscalls[i]->action);

      /* errno_ret is only honoured when present (absent defaults to EPERM).  */
      PROCESS_OPTIONAL_DATA (seccomp->syscalls[i]->errno_ret_present, seccomp->syscalls[i]->errno_ret);

      PROCESS_COUNT (seccomp->syscalls[i]->names_len);
      עבור (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        PROCESS_STRING (seccomp->syscalls[i]->names[j]);

      PROCESS_COUNT (seccomp->syscalls[i]->args_len);
      עבור (j = 0; j < seccomp->syscalls[i]->args_len; j++)
        {
          /* index, value and value_two are used unconditionally by the
             generator (absent fields are 0), so hash them in fixed positions.  */
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->index);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->value);
          PROCESS_DATA (seccomp->syscalls[i]->args[j]->value_two);
          PROCESS_STRING (seccomp->syscalls[i]->args[j]->op);
        }
    }

  blake3_hasher_finalize (&hasher, hash, حجم (hash));

  עבור (i = 0; i < 32; i++)
    {
      size_t remaining = حجم (seccomp_checksum_t) - i * 2;
      ret = snprintf (&out[i * 2], remaining, "%02x", hash[i]);
      אם (UNLIKELY (ret != 2))
        החזר crun_make_error (err, 0, "internal error: static buffer has wrong size");
    }
  out[64] = 0;

#undef PROCESS_STRING
#undef PROCESS_DATA
#undef PROCESS_OPTIONAL_DATA
#undef PROCESS_COUNT
  החזר 1;
}

ساكن صحيح
open_rundir_dirfd (ثابت حرف *state_root, libcrun_error_t *err)
{
  cleanup_free حرف *dir = عدم;
  صحيح dirfd;
  صحيح ret;

  ret = libcrun_get_state_directory (&dir, state_root, عدم, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  dirfd = TEMP_FAILURE_RETRY (open (dir, O_PATH | O_DIRECTORY | O_CLOEXEC));
  אם (UNLIKELY (dirfd < 0))
    החזר crun_make_error (err, errno, "open `%s`", dir);

  החזר dirfd;
}

بنية cache_entry
{
  seccomp_checksum_t checksum;
  بنية timespec atime;
};

ساكن صحيح
compare_entries_by_atime (ثابت فراغ *a, ثابت فراغ *b)
{
  ثابت بنية cache_entry *entry_a = a;
  ثابت بنية cache_entry *entry_b = b;
  صحيح ret;

  ret = entry_a->atime.tv_sec - entry_b->atime.tv_sec;
  אם (ret)
    החזר ret;

  החזר entry_a->atime.tv_nsec - entry_b->atime.tv_nsec;
}

ساكن صحيح
evict_cache (صحيح root_dfd, libcrun_error_t *err)
{
  cleanup_close صحيح cache_dir_fd = -1;
  off_t cache_dir_inode_max_size;
  بنية stat st;
  صحيح ret;

  ret = TEMP_FAILURE_RETRY (fstat (root_dfd, &st));
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "fstat rundir");

  cache_dir_inode_max_size = get_cache_dir_inode_max_size (st.st_size);

  cache_dir_fd = TEMP_FAILURE_RETRY (openat (root_dfd, SECCOMP_CACHE_DIR, O_DIRECTORY | O_CLOEXEC));
  אם (UNLIKELY (cache_dir_fd < 0))
    {
      אם (errno == ENOENT)
        החזר 0;
      החזר crun_make_error (err, errno, "open `%s`", SECCOMP_CACHE_DIR);
    }

  ret = TEMP_FAILURE_RETRY (fstat (cache_dir_fd, &st));
  אם (ret == 0 && st.st_size > cache_dir_inode_max_size)
    {
      cleanup_free بنية cache_entry *entries = عدم;
      cleanup_dir DIR *d = عدم;
      size_t i, n_entries = 0;
      بنية dirent *de;
      صحيح dfd;

      d = fdopendir (cache_dir_fd);
      אם (d == عدم)
        החזר crun_make_error (err, errno, "cannot open seccomp cache directory");
      /* fdopendir owns the fd now.  */
      cache_dir_fd = -1;

      dfd = dirfd (d);
      כלעוד ((de = readdir (d)))
        {
          אם (de->d_name[0] == '.')
            استمر;

          ret = TEMP_FAILURE_RETRY (fstatat (dfd, de->d_name, &st, AT_SYMLINK_NOFOLLOW));
          אם (UNLIKELY (ret < 0))
            استمر;

          /* The cache file is already used, it is pointless to free it.  */
          אם (st.st_nlink > 1)
            استمر;

          /* If the sticky bit is set, then ignore it.  */
          אם (st.st_mode & S_ISVTX)
            استمر;

          אם (strlen (de->d_name) != حجم (seccomp_checksum_t) - 1)
            {
              /* No mercy for unknown files.  */
              unlinkat (dfd, de->d_name, 0);
              استمر;
            }
          אחרת
            {
              entries = xrealloc (entries, حجم (بنية cache_entry) * (n_entries + 1));

              memcpy (entries[n_entries].checksum, de->d_name, حجم (seccomp_checksum_t));
              entries[n_entries].atime = st.st_atim;
              n_entries++;
            }
        }

      qsort (entries, n_entries, حجم (بنية cache_entry), compare_entries_by_atime);

      /* Attempt to delete half of them.  */
      עבור (i = 0; i < (n_entries == 1 ? 1 : n_entries / 2); i++)
        unlinkat (dfd, entries[i].checksum, 0);
    }
  החזר 0;
}

/* Open the run-directory dirfd and build the "<id>/seccomp.bpf" path
   relative to it.  Returns the (caller-owned) dirfd or a negative error.  */
ساكن صحيح
open_seccomp_bpf_dirfd (libcrun_container_t *container, حرف **rel_path, libcrun_error_t *err)
{
  صحيح ret;
  صحيح dirfd;

  dirfd = open_rundir_dirfd (container->context ? container->context->state_root : عدم, err);
  אם (UNLIKELY (dirfd < 0))
    החזר dirfd;

  /* relative path to dirfd.  */
  ret = append_paths (rel_path, err, container->context->id, "seccomp.bpf", عدم);
  אם (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (close (dirfd));
      החזר ret;
    }

  החזר dirfd;
}

ساكن صحيح
store_seccomp_cache (بنية libcrun_seccomp_gen_ctx_s *ctx, libcrun_error_t *err)
{
  libcrun_container_t *container = ctx->container;
  cleanup_free حرف *src_path = عدم;
  cleanup_free حرف *dest_path = عدم;
  cleanup_close صحيح dirfd = -1;
  صحيح ret;

  אם (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    החזר 0;

  אם (is_empty_string (ctx->checksum))
    החזר 0;

  dirfd = open_seccomp_bpf_dirfd (container, &src_path, err);
  אם (UNLIKELY (dirfd < 0))
    החזר dirfd;

  ret = append_paths (&dest_path, err, SECCOMP_CACHE_DIR, ctx->checksum, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = crun_ensure_directory_at (dirfd, SECCOMP_CACHE_DIR, 0700, true, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = evict_cache (dirfd, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = linkat (dirfd, src_path, dirfd, dest_path, 0);
  אם (UNLIKELY (ret < 0 && errno != EEXIST))
    החזר crun_make_error (err, errno, "link `%s` to `%s`", src_path, dest_path);
  החזר 0;
}

ساكن مضمن runtime_spec_schema_config_linux_seccomp *
get_seccomp_configuration (بنية libcrun_seccomp_gen_ctx_s *ctx)
{
  אם (ctx->container == عدم || ctx->container->container_def == عدم
      || ctx->container->container_def->linux == عدم)
    החזר 0;

  החזר ctx->container->container_def->linux->seccomp;
}

/* Whether a seccomp.bpf file is expected to exist for the container.  */
ساكن مضمن bool
has_seccomp_configuration (بنية libcrun_seccomp_gen_ctx_s *ctx)
{
  אם (ctx->container == عدم || ctx->container->container_def == عدم
      || ctx->container->container_def->linux == عدم)
    החזר false;

  החזר ctx->container->container_def->linux->seccomp != عدم
         || find_annotation (ctx->container, "run.oci.seccomp_bpf_data") != عدم;
}

ساكن صحيح
find_in_cache (بنية libcrun_seccomp_gen_ctx_s *ctx, صحيح dirfd, ثابت حرف *dest_path, bool *created, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_seccomp *seccomp;
  cleanup_free حرف *cache_file_path = عدم;
  صحيح ret;

  אם (ctx->options & LIBCRUN_SECCOMP_SKIP_CACHE)
    החזר 0;

  seccomp = get_seccomp_configuration (ctx);
  אם (seccomp == عدم)
    החזר 0;

  /* if the checksum could not be computed, returns early.  */
  ret = calculate_seccomp_checksum (seccomp, ctx->options, ctx->checksum, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = append_paths (&cache_file_path, err, SECCOMP_CACHE_DIR, ctx->checksum, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = TEMP_FAILURE_RETRY (linkat (dirfd, cache_file_path, dirfd, dest_path, 0));
  אם (UNLIKELY (ret < 0 && errno != ENOENT))
    החזר crun_make_error (err, errno, "linkat `%s` to `%s`", cache_file_path, dest_path);

  *created = ret == 0;

  החזר 0;
}

صحيح
libcrun_generate_seccomp (بنية libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err)
{
#ifdef HAVE_SECCOMP
  runtime_spec_schema_config_linux_seccomp *seccomp;
  صحيح ret;
  size_t i;
  cleanup_seccomp scmp_filter_ctx ctx = عدم;
  صحيح default_errno_value = EPERM;
  uint32_t action, default_action;
  ثابت حرف *def_action = عدم;

  /* The bpf filter was loaded from the cache, nothing to do here.  */
  אם (gen_ctx->from_cache)
    החזר 0;

  אם (gen_ctx->container == عدم || gen_ctx->container->container_def == عدم || gen_ctx->container->container_def->linux == عدم)
    החזר 0;

  seccomp = gen_ctx->container->container_def->linux->seccomp;
  אם (seccomp == عدم)
    החזר 0;

  /* seccomp not available.  */
  אם (prctl (PR_GET_SECCOMP, 0, 0, 0, 0) < 0)
    החזר crun_make_error (err, errno, "prctl");

  def_action = seccomp->default_action;
  אם (def_action == عدم)
    החזר crun_make_error (err, 0, "seccomp misses the default action");

  אם (seccomp->default_errno_ret_present)
    {
      אם (! seccomp_action_supports_errno (def_action))
        החזר crun_make_error (err, 0, "errno value specified for action `%s`", def_action);

      default_errno_value = seccomp->default_errno_ret;
    }

  ret = get_seccomp_action (def_action, default_errno_value, &default_action, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ctx = seccomp_init (default_action);
  אם (ctx == عدم)
    החזר crun_make_error (err, 0, "error seccomp_init");

  עבור (i = 0; i < seccomp->architectures_len; i++)
    {
      uint32_t arch_token;
      ثابت حرف *arch = seccomp->architectures[i];
      حرف *end, lowercase_arch[32] = {
        0,
      };

      אם (arch == عدم)
        החזר crun_make_error (err, EINVAL, "seccomp architecture is not specified");

      אם (has_prefix (arch, "SCMP_ARCH_"))
        arch += 10;
      end = stpncpy (lowercase_arch, arch, حجم (lowercase_arch) - 1);
      *end = '\0';
      make_lowercase (lowercase_arch);
#  ifdef SECCOMP_ARCH_RESOLVE_NAME
      arch_token = seccomp_arch_resolve_name (lowercase_arch);
      אם (arch_token == 0)
        החזר crun_make_error (err, 0, "seccomp unknown architecture `%s`", arch);
#  אחרת
      arch_token = SCMP_ARCH_NATIVE;
#  endif
      ret = seccomp_arch_add (ctx, arch_token);
      אם (ret < 0 && ret != -EEXIST)
        החזר crun_make_error (err, -ret, "seccomp adding architecture");
    }

  עבור (i = 0; i < seccomp->syscalls_len; i++)
    {
      size_t j;
      bool multiple_args = false;
      صحيح errno_ret = EPERM;

      אם (seccomp->syscalls[i]->errno_ret_present)
        {
          אם (! seccomp_action_supports_errno (seccomp->syscalls[i]->action))
            החזר crun_make_error (err, 0, "errno value specified for action `%s`", seccomp->syscalls[i]->action);
          errno_ret = seccomp->syscalls[i]->errno_ret;
        }

      ret = get_seccomp_action (seccomp->syscalls[i]->action, errno_ret, &action, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      אם (action == default_action)
        استمر;

      /* The argument indices do not depend on the syscall name, so validate
         them once per entry.  Doing it here also makes an invalid index an
         error even when every name below is unknown and skipped.  */
      אם (seccomp->syscalls[i]->args != عدم)
        {
          uint32_t count[6] = {};
          size_t k;

          עבור (k = 0; k < seccomp->syscalls[i]->args_len; k++)
            {
              uint32_t index = seccomp->syscalls[i]->args[k]->index;

              אם (index >= 6)
                החזר crun_make_error (err, 0, "invalid seccomp index `%u`", index);

              count[index]++;
              אם (count[index] > 1)
                multiple_args = true;
            }
        }

      עבור (j = 0; j < seccomp->syscalls[i]->names_len; j++)
        {
          صحيح syscall = seccomp_syscall_resolve_name (seccomp->syscalls[i]->names[j]);

          אם (UNLIKELY (syscall == __NR_SCMP_ERROR))
            {
              אם (gen_ctx->options & LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL)
                החזר crun_make_error (err, 0, "invalid seccomp syscall `%s`", seccomp->syscalls[i]->names[j]);

              libcrun_warning ("unknown seccomp syscall `%s` ignored", seccomp->syscalls[i]->names[j]);
              استمر;
            }

          אם (seccomp->syscalls[i]->args == عدم)
            {
              ret = seccomp_rule_add (ctx, action, syscall, 0);
              אם (UNLIKELY (ret < 0))
                החזר crun_make_error (err, -ret, "seccomp_rule_add `%s`", seccomp->syscalls[i]->names[j]);
            }
          אחרת
            {
              size_t k;

              /* If multiple rules refer to the same argument, treat the rules are in OR.

                 Note this is not limited to the repeated argument: once any index
                 is repeated, every condition of the entry becomes a rule of its
                 own, so conditions on distinct indices stop being ANDed together.
                 The OCI runtime specification does not define how conditions
                 combine, and this matches what runc does; see
                 https://github.com/opencontainers/runc/issues/2735.  */
              אם (multiple_args)
                {
                  size_t r;

                  עבור (r = 0; r < seccomp->syscalls[i]->args_len; r++)
                    {
                      بنية scmp_arg_cmp arg_cmp;
                      حرف *op = seccomp->syscalls[i]->args[r]->op;

                      arg_cmp.arg = seccomp->syscalls[i]->args[r]->index;
                      ret = get_seccomp_operator (op, &arg_cmp.op, err);
                      אם (UNLIKELY (ret < 0))
                        החזר ret;
                      arg_cmp.datum_a = seccomp->syscalls[i]->args[r]->value;
                      arg_cmp.datum_b = seccomp->syscalls[i]->args[r]->value_two;

                      ret = seccomp_rule_add_array (ctx, action, syscall, 1, &arg_cmp);
                      אם (UNLIKELY (ret < 0))
                        החזר crun_make_error (err, -ret, "seccomp_rule_add_array");
                    }
                }
              אחרת
                {
                  /* No index is repeated, so there are at most 6 distinct arguments.  */
                  ثابت size_t args_len = seccomp->syscalls[i]->args_len;
                  بنية scmp_arg_cmp arg_cmp[6];

                  עבור (k = 0; k < args_len; k++)
                    {
                      حرف *op = seccomp->syscalls[i]->args[k]->op;

                      arg_cmp[k].arg = seccomp->syscalls[i]->args[k]->index;
                      ret = get_seccomp_operator (op, &(arg_cmp[k].op), err);
                      אם (UNLIKELY (ret < 0))
                        החזר ret;
                      arg_cmp[k].datum_a = seccomp->syscalls[i]->args[k]->value;
                      arg_cmp[k].datum_b = seccomp->syscalls[i]->args[k]->value_two;
                    }

                  /* An empty "args" array means the rule is unconditional.  Do not
                     hand over the uninitialized buffer for it.  */
                  ret = seccomp_rule_add_array (ctx, action, syscall, args_len, args_len ? arg_cmp : عدم);
                  אם (UNLIKELY (ret < 0))
                    החזר crun_make_error (err, -ret, "seccomp_rule_add_array");
                }
            }
        }
    }

  אם (gen_ctx->fd >= 0)
    {
      ret = export_bpf (ctx, gen_ctx->fd, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      החזר store_seccomp_cache (gen_ctx, err);
    }

  החזר 0;
#אחרת
  החזר 0;
#endif
}

صحيح
libcrun_copy_seccomp (بنية libcrun_seccomp_gen_ctx_s *gen_ctx, ثابت حرف *b64_bpf, libcrun_error_t *err)
{
  cleanup_free حرف *bpf_data = عدم;
  size_t size = 0;
  size_t in_size;
  صحيح consumed;
  صحيح ret;

  in_size = strlen (b64_bpf);
  bpf_data = xmalloc (in_size + 1);

  consumed = base64_decode (b64_bpf, in_size, bpf_data, in_size, &size);
  אם (UNLIKELY (consumed != (صحيح) in_size))
    החזר crun_make_error (err, 0, "invalid seccomp BPF data");

  ret = safe_write (gen_ctx->fd, "seccomp fd", bpf_data, size, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר 0;
}

صحيح
libcrun_open_seccomp_bpf (بنية libcrun_seccomp_gen_ctx_s *ctx, صحيح *fd, libcrun_error_t *err)
{
  cleanup_close صحيح dirfd = -1;
  cleanup_free حرف *dest_path = عدم;
  libcrun_container_t *container = ctx->container;
  صحيح ret;

  ctx->fd = *fd = -1;

  אם (container == عدم || container->context == عدم)
    החזר crun_make_error (err, EINVAL, "invalid internal state");

  dirfd = open_seccomp_bpf_dirfd (container, &dest_path, err);
  אם (UNLIKELY (dirfd < 0))
    החזר dirfd;

  אם (ctx->create)
    {
      bool created = false;

      ret = find_in_cache (ctx, dirfd, dest_path, &created, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      אם (created)
        {
          ctx->from_cache = true;
          اذهب open_existing_file;
        }

      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDWR | O_CREAT, 0700));
      אם (UNLIKELY (ret < 0))
        החזר crun_make_error (err, errno, "open `seccomp.bpf`");
      ctx->fd = *fd = ret;
    }
  אחרת
    {
    open_existing_file:
      ret = TEMP_FAILURE_RETRY (openat (dirfd, dest_path, O_CLOEXEC | O_RDONLY));
      אם (UNLIKELY (ret < 0))
        {
          /* The file is missing only when the container has no seccomp
             configuration.  If it is configured, do not run without the filter.  */
          אם (errno == ENOENT && ! has_seccomp_configuration (ctx))
            החזר 0;
          החזר crun_make_error (err, errno, "open `seccomp.bpf`");
        }
      ctx->fd = *fd = ret;
    }

  החזר 0;
}
