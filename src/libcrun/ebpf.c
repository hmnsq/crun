/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "ebpf.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <sys/syscall.h>
#অন্তর্ভাব <sys/resource.h>

#ifdef HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
স্থির পূর্ণসংখ্যা
syscall_bpf (পূর্ণসংখ্যা cmd, সংঘ bpf_attr *attr, unsigned পূর্ণসংখ্যা আকার_2)
{
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_bpf, cmd, attr, আকার_2);
}
#    define bpf syscall_bpf
#  endif

#endif

স্থির অন্তঃরেখ uint64_t
ptr_to_u64 (ধ্রুবক শূন্য *ptr)
{
  ফেরত (uint64_t) (unsigned long) ptr;
}

গণনা
{
  HAS_WILDCARD = 1
};

কাঠামো bpf_program
{
  size_t allocated;
  size_t used;
  unsigned পূর্ণসংখ্যা ব্যক্তিগত;
  অক্ষর program[];
};

#ifdef HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((কাঠামো bpf_insn) { .code = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(আকার_3, DST, SRC, OFF) \
    ((কাঠামো bpf_insn) {                   \
        .code = BPF_LDX | BPF_আকার (আকার_3) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((কাঠামো bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((কাঠামো bpf_insn) { .code = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((কাঠামো bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((কাঠামো bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((কাঠামো bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((কাঠামো bpf_insn) { .code = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((কাঠামো bpf_insn) { .code = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#endif

#ifdef HAVE_EBPF
স্থির size_t
bpf_program_instructions (কাঠামো bpf_program *program)
{
  ফেরত program->used / (আকার (কাঠামো bpf_insn));
}
#endif

কাঠামো bpf_program *
bpf_program_new (size_t আকার_2)
{
  কাঠামো bpf_program *p = xmalloc (আকার_2 + আকার (কাঠামো bpf_program));

  p->used = 0;
  p->ব্যক্তিগত = 0;
  p->allocated = আকার_2;

  ফেরত p;
}

কাঠামো bpf_program *
bpf_program_append (কাঠামো bpf_program *p, শূন্য *তথ্য, size_t আকার_2)
{
  যদি (p->allocated <= p->used + আকার_2)
    {
      p->allocated += আকার_2 * 2;
      p = xrealloc (p, p->allocated + আকার (কাঠামো bpf_program));
    }
  memcpy (p->program + p->used, তথ্য, আকার_2);
  p->used += আকার_2;
  ফেরত p;
}

কাঠামো bpf_program *
bpf_program_init_dev (কাঠামো bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  /* taken from systemd.  */
  কাঠামো bpf_insn pre_insn[] = {
    /* type -> R2.  */
    BPF_LDX_MEM (BPF_W, BPF_REG_2, BPF_REG_1, 0),
    BPF_ALU32_IMM (BPF_AND, BPF_REG_2, 0xFFFF),

    /* access -> R3.  */
    BPF_LDX_MEM (BPF_W, BPF_REG_3, BPF_REG_1, 0),
    BPF_ALU32_IMM (BPF_RSH, BPF_REG_3, 16),

    /* major -> R4.  */
    BPF_LDX_MEM (BPF_W, BPF_REG_4, BPF_REG_1, 4),

    /* minor -> R5.  */
    BPF_LDX_MEM (BPF_W, BPF_REG_5, BPF_REG_1, 8),
  };
  program = bpf_program_append (program, pre_insn, আকার (pre_insn));
#endif
  ফেরত program;
}

কাঠামো bpf_program *
bpf_program_append_dev (কাঠামো bpf_program *program, ধ্রুবক অক্ষর *access, অক্ষর type, পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor, bool accept,
                        libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  পূর্ণসংখ্যা i;
  পূর্ণসংখ্যা bpf_access = 0;
  পূর্ণসংখ্যা bpf_type = type == 'b' ? BPF_DEVCG_DEV_BLOCK : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  পূর্ণসংখ্যা সংখ্যা_instructions = 0;
  কাঠামো bpf_insn accept_block[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  যদি (access == ফাঁকা)
    access = "";

  যদি (program->ব্যক্তিগত & HAS_WILDCARD)
    ফেরত program;

  জন্য (i = 0; access[i]; i++)
    {
      নির্বাচন (access[i])
        {
        ক্ষেত্র 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          বিরতি;

        ক্ষেত্র 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          বিরতি;

        ক্ষেত্র 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          বিরতি;
        }
    }

  /*
    if (request.type != device.type)
      goto next_block:
    if (accept && (request.access & device.access) != request.access)
      goto next_block:
    if (! accept && (request.access & device.access) == 0)
      goto next_block:
    if (device.major != '*' && request.major != device.major) == 0)
      goto next_block:
    if (device.minor != '*' && request.minor != device.minor) == 0)
      goto next_block:
    return accept_or_reject;
  next_block:
  */

  /* If the access is rwm, skip the check.  */
  has_access = bpf_access != (BPF_DEVCG_ACC_READ | BPF_DEVCG_ACC_WRITE | BPF_DEVCG_ACC_MKNOD);

  /* Number of instructions to skip the ACCEPT BLOCK.  */
  সংখ্যা_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  যদি (has_type)
    {
      কাঠামো bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, সংখ্যা_instructions) };
      সংখ্যা_instructions--;
      program = bpf_program_append (program, i, আকার (i));
    }
  যদি (has_access)
    {
      কাঠামো bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, সংখ্যা_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, সংখ্যা_instructions - 2),
      };
      সংখ্যা_instructions -= 3;
      program = bpf_program_append (program, i, আকার (i));
    }
  যদি (has_major)
    {
      কাঠামো bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, সংখ্যা_instructions) };
      সংখ্যা_instructions--;
      program = bpf_program_append (program, i, আকার (i));
    }
  যদি (has_minor)
    {
      কাঠামো bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, সংখ্যা_instructions) };
      সংখ্যা_instructions--;
      program = bpf_program_append (program, i, আকার (i));
    }

  যদি (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    program->ব্যক্তিগত |= HAS_WILDCARD;

  program = bpf_program_append (program, accept_block, আকার (accept_block));
#endif
  (শূন্য) access;
  (শূন্য) type;
  (শূন্য) major;
  (শূন্য) minor;
  (শূন্য) accept;

  ফেরত program;
}

কাঠামো bpf_program *
bpf_program_complete_dev (কাঠামো bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  কাঠামো bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  যদি (program->ব্যক্তিগত & HAS_WILDCARD)
    ফেরত program;

  program = bpf_program_append (program, &i, আকার (i));
#endif
  ফেরত program;
}

স্থির পূর্ণসংখ্যা
read_all_progs (পূর্ণসংখ্যা dirfd, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  cleanup_free uint32_t *progs = ফাঁকা;
  সংঘ bpf_attr attr;
  পূর্ণসংখ্যা ret;
  size_t cur_আকার;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  জন্য (cur_আকার = 64; cur_আকার <= 4096; cur_আকার *= 2)
    {
      progs = xrealloc (progs, আকার (uint32_t) * cur_আকার);

      memset (&attr, 0, আকার (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_আকার;
      attr.query.prog_ids = ptr_to_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, আকার (attr));
    }
  যখন (ret < 0 && errno == ENOSPC)
    ;

  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "bpf query");

  *progs_out = progs;
  progs = ফাঁকা;
  *n_progs_out = attr.query.prog_cnt;
  ফেরত 0;
#অন্যথা
  (শূন্য) dirfd;

  *progs_out = ফাঁকা;
  *n_progs_out = 0;

  ফেরত crun_make_error (err, 0, "eBPF not supported");
#endif
}

স্থির পূর্ণসংখ্যা
remove_all_progs (পূর্ণসংখ্যা dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  সংঘ bpf_attr attr;
  size_t i;

  জন্য (i = 0; i < n_progs; i++)
    {
      cleanup_close পূর্ণসংখ্যা fd = -1;
      পূর্ণসংখ্যা ret;

      memset (&attr, 0, আকার (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, আকার (attr));
      যদি (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          যদি (errno == ENOENT)
            চালু;

          ফেরত crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, আকার (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, আকার (attr));
      যদি (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          যদি (errno == ENOENT)
            চালু;

          ফেরত crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  ফেরত 0;
#অন্যথা
  (শূন্য) dirfd;
  (শূন্য) progs;
  (শূন্য) n_progs;
  ফেরত crun_make_error (err, 0, "eBPF not supported");
#endif
}

স্থির পূর্ণসংখ্যা
ebpf_attach_program (পূর্ণসংখ্যা fd, পূর্ণসংখ্যা dirfd, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (শূন্য) fd;
  (শূন্য) dirfd;
  (শূন্য) read_all_progs;
  (শূন্য) remove_all_progs;

  ফেরত crun_make_error (err, 0, "eBPF not supported");
#অন্যথা
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  ধ্রুবক পূর্ণসংখ্যা MAX_ATTEMPTS = 20;
  পূর্ণসংখ্যা attempt;

  জন্য (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = ফাঁকা;
      cleanup_close পূর্ণসংখ্যা replacefd = -1;
      সংঘ bpf_attr attr;
      size_t n_progs = 0;
      পূর্ণসংখ্যা ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      যদি (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, আকার (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, আকার (attr));
          যদি (UNLIKELY (replacefd < 0))
            {
              যদি (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  চালু;
                }
              ফেরত crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, আকার (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      যদি (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, আকার (attr));
      যদি (UNLIKELY (ret < 0))
        {
          যদি (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              চালু;
            }
#  ifdef BPF_F_REPLACE
          যদি (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              চালু;
            }
#  endif
          ফেরত crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      যদি (replacefd < 0 && n_progs)
        ফেরত remove_all_progs (dirfd, progs, n_progs, err);

      ফেরত 0;
    }
#endif
}

স্থির শূন্য
bump_memlock ()
{
  কাঠামো rlimit limit;
  পূর্ণসংখ্যা ret;

  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &limit);
  যদি (ret == 0)
    ফেরত;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &limit);
  যদি (ret < 0)
    ফেরত;

  limit.rlim_cur = limit.rlim_max;
  /* Best effort, ignore errors.  */
  (শূন্য) setrlimit (RLIMIT_MEMLOCK, &limit);
}

পূর্ণসংখ্যা
libcrun_ebpf_load (কাঠামো bpf_program *program, পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *pin, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (শূন্য) dirfd;
  (শূন্য) program;
  (শূন্য) pin;
  (শূন্য) ebpf_attach_program;

  ফেরত crun_make_error (err, 0, "eBPF not supported");
#অন্যথা
  cleanup_close পূর্ণসংখ্যা fd = -1;
  সংঘ bpf_attr attr;
  পূর্ণসংখ্যা ret;

  memset (&attr, 0, আকার (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_to_u64 (program->program);
  attr.insn_cnt = bpf_program_instructions (program);
  attr.license = ptr_to_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_LOAD, &attr, আকার (attr));
  যদি (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_LOAD, &attr, আকার (attr));
    }
  যদি (fd < 0)
    {
      ধ্রুবক size_t max_log_আকার = 1 << 20;
      cleanup_free অক্ষর *log = ফাঁকা;
      size_t log_আকার = 8192;

    retry:
      log = xrealloc (log, log_আকার);
      log[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_to_u64 (log);
      attr.log_আকার = log_আকার;

      fd = bpf (BPF_PROG_LOAD, &attr, আকার (attr));
      যদি (fd < 0)
        {
          যদি (errno == ENOSPC && log_আকার < max_log_আকার)
            {
              /* The provided buffer was not big enough.  */
              log_আকার *= 2;
              যাও retry;
            }
          ফেরত crun_make_error (err, errno, "bpf create `%s`", log);
        }
    }

  যদি (dirfd >= 0)
    {
      ret = ebpf_attach_program (fd, dirfd, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  /* Optionally pin the program to the specified path.  */
  যদি (pin)
    {
      unlink (pin);

      memset (&attr, 0, আকার (attr));
      attr.pathname = ptr_to_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, আকার (attr));
      যদি (ret < 0)
        ফেরত crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  ফেরত 0;
#endif
}

পূর্ণসংখ্যা
libcrun_ebpf_read_program (কাঠামো bpf_program **program_ret, ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (শূন্য) program_ret;
  (শূন্য) path;

  ফেরত crun_make_error (err, 0, "eBPF not supported");
#অন্যথা
  cleanup_free কাঠামো bpf_program *program = ফাঁকা;
  cleanup_free অক্ষর *বাফার = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা prog_fd = -1;
  size_t বাফার_আকার = 0;
  কাঠামো bpf_prog_info info;
  সংঘ bpf_attr attr;
  পূর্ণসংখ্যা ret;

  memset (&attr, 0, আকার (attr));
  attr.pathname = ptr_to_u64 (path);

  prog_fd = bpf (BPF_OBJ_GET, &attr, আকার (attr));
  যদি (UNLIKELY (prog_fd < 0))
    ফেরত crun_make_error (err, errno, "bpf get `%s`", path);

  memset (&info, 0, আকার (info));

  memset (&attr, 0, আকার (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_to_u64 (&info);
  attr.info.info_len = আকার (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, আকার (attr));
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "bpf get info `%s`", path);

  বাফার_আকার = info.xlated_prog_len;
  বাফার = xmalloc (বাফার_আকার);

  memset (&info, 0, আকার (info));
  info.xlated_prog_insns = ptr_to_u64 (বাফার);
  info.xlated_prog_len = বাফার_আকার;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, আকার (attr));
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "bpf get info `%s`", path);

  program = bpf_program_new (বাফার_আকার);
  program = bpf_program_append (program, বাফার, বাফার_আকার);

  *program_ret = program;
  program = ফাঁকা;

  ফেরত 0;
#endif
}

পূর্ণসংখ্যা
libcrun_ebpf_query_cgroup_progs (ধ্রুবক অক্ষর *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (শূন্য) cgroup_path;
  *progs_out = ফাঁকা;
  *n_progs_out = 0;
  ফেরত crun_make_error (err, 0, "eBPF not supported");
#অন্যথা
  cleanup_close পূর্ণসংখ্যা cgroup_fd = -1;

  cgroup_fd = open (cgroup_path, O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (cgroup_fd < 0))
    ফেরত crun_make_error (err, errno, "open cgroup path `%s`", cgroup_path);

  ফেরত read_all_progs (cgroup_fd, progs_out, n_progs_out, err);
#endif
}

bool
libcrun_ebpf_cmp_programs (কাঠামো bpf_program *program1, কাঠামো bpf_program *program2)
{
  যদি (program1->used != program2->used)
    ফেরত false;

  ফেরত memcmp (program1->program, program2->program, program1->used) == 0;
}
