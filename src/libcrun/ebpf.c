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
স্থির পূর্ণাঙ্ক
syscall_bpf (পূর্ণাঙ্ক cmd, সঙ্ঘ bpf_attr *attr, unsigned পূর্ণাঙ্ক size)
{
  প্রত্যাগম (পূর্ণাঙ্ক) syscall (__NR_bpf, cmd, attr, size);
}
#    define bpf syscall_bpf
#  endif

#endif

স্থির অন্তঃস্থ uint64_t
ptr_to_u64 (ধ্রুব শূন্য *ptr)
{
  প্রত্যাগম (uint64_t) (unsigned long) ptr;
}

গণনা
{
  HAS_WILDCARD = 1
};

সংরচনা bpf_program
{
  size_t allocated;
  size_t used;
  unsigned পূর্ণাঙ্ক গুপ্ত;
  বর্ণ program[];
};

#ifdef HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((সংরচনা bpf_insn) { .code = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(SIZE, DST, SRC, OFF) \
    ((সংরচনা bpf_insn) {                   \
        .code = BPF_LDX | BPF_SIZE (SIZE) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((সংরচনা bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((সংরচনা bpf_insn) { .code = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((সংরচনা bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((সংরচনা bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((সংরচনা bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((সংরচনা bpf_insn) { .code = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((সংরচনা bpf_insn) { .code = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#endif

#ifdef HAVE_EBPF
স্থির size_t
bpf_program_instructions (সংরচনা bpf_program *program)
{
  প্রত্যাগম program->used / (পরিমাণ (সংরচনা bpf_insn));
}
#endif

সংরচনা bpf_program *
bpf_program_new (size_t size)
{
  সংরচনা bpf_program *p = xmalloc (size + পরিমাণ (সংরচনা bpf_program));

  p->used = 0;
  p->গুপ্ত = 0;
  p->allocated = size;

  প্রত্যাগম p;
}

সংরচনা bpf_program *
bpf_program_append (সংরচনা bpf_program *p, শূন্য *data, size_t size)
{
  যদি (p->allocated <= p->used + size)
    {
      p->allocated += size * 2;
      p = xrealloc (p, p->allocated + পরিমাণ (সংরচনা bpf_program));
    }
  memcpy (p->program + p->used, data, size);
  p->used += size;
  প্রত্যাগম p;
}

সংরচনা bpf_program *
bpf_program_init_dev (সংরচনা bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  /* taken from systemd.  */
  সংরচনা bpf_insn pre_insn[] = {
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
  program = bpf_program_append (program, pre_insn, পরিমাণ (pre_insn));
#endif
  প্রত্যাগম program;
}

সংরচনা bpf_program *
bpf_program_append_dev (সংরচনা bpf_program *program, ধ্রুব বর্ণ *access, বর্ণ type, পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor, bool accept,
                        libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  পূর্ণাঙ্ক i;
  পূর্ণাঙ্ক bpf_access = 0;
  পূর্ণাঙ্ক bpf_type = type == 'b' ? BPF_DEVCG_DEV_BLOCK : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  পূর্ণাঙ্ক সংখ্যা_instructions = 0;
  সংরচনা bpf_insn accept_block[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  যদি (access == রিক্ত)
    access = "";

  যদি (program->গুপ্ত & HAS_WILDCARD)
    প্রত্যাগম program;

  প্রতি (i = 0; access[i]; i++)
    {
      বিকল্প (access[i])
        {
        পক্ষ 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          ভঙ্গ;

        পক্ষ 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          ভঙ্গ;

        পক্ষ 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          ভঙ্গ;
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
      সংরচনা bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, সংখ্যা_instructions) };
      সংখ্যা_instructions--;
      program = bpf_program_append (program, i, পরিমাণ (i));
    }
  যদি (has_access)
    {
      সংরচনা bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, সংখ্যা_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, সংখ্যা_instructions - 2),
      };
      সংখ্যা_instructions -= 3;
      program = bpf_program_append (program, i, পরিমাণ (i));
    }
  যদি (has_major)
    {
      সংরচনা bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, সংখ্যা_instructions) };
      সংখ্যা_instructions--;
      program = bpf_program_append (program, i, পরিমাণ (i));
    }
  যদি (has_minor)
    {
      সংরচনা bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, সংখ্যা_instructions) };
      সংখ্যা_instructions--;
      program = bpf_program_append (program, i, পরিমাণ (i));
    }

  যদি (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    program->গুপ্ত |= HAS_WILDCARD;

  program = bpf_program_append (program, accept_block, পরিমাণ (accept_block));
#endif
  (শূন্য) access;
  (শূন্য) type;
  (শূন্য) major;
  (শূন্য) minor;
  (শূন্য) accept;

  প্রত্যাগম program;
}

সংরচনা bpf_program *
bpf_program_complete_dev (সংরচনা bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  সংরচনা bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  যদি (program->গুপ্ত & HAS_WILDCARD)
    প্রত্যাগম program;

  program = bpf_program_append (program, &i, পরিমাণ (i));
#endif
  প্রত্যাগম program;
}

স্থির পূর্ণাঙ্ক
read_all_progs (পূর্ণাঙ্ক dirfd, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  cleanup_free uint32_t *progs = রিক্ত;
  সঙ্ঘ bpf_attr attr;
  পূর্ণাঙ্ক ret;
  size_t cur_size;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  প্রতি (cur_size = 64; cur_size <= 4096; cur_size *= 2)
    {
      progs = xrealloc (progs, পরিমাণ (uint32_t) * cur_size);

      memset (&attr, 0, পরিমাণ (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_size;
      attr.query.prog_ids = ptr_to_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, পরিমাণ (attr));
    }
  যাবত্ (ret < 0 && errno == ENOSPC)
    ;

  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "bpf query");

  *progs_out = progs;
  progs = রিক্ত;
  *n_progs_out = attr.query.prog_cnt;
  প্রত্যাগম 0;
#অন্যথা
  (শূন্য) dirfd;

  *progs_out = রিক্ত;
  *n_progs_out = 0;

  প্রত্যাগম crun_make_error (err, 0, "eBPF not supported");
#endif
}

স্থির পূর্ণাঙ্ক
remove_all_progs (পূর্ণাঙ্ক dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  সঙ্ঘ bpf_attr attr;
  size_t i;

  প্রতি (i = 0; i < n_progs; i++)
    {
      cleanup_close পূর্ণাঙ্ক fd = -1;
      পূর্ণাঙ্ক ret;

      memset (&attr, 0, পরিমাণ (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, পরিমাণ (attr));
      যদি (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          যদি (errno == ENOENT)
            অনুবর্ত;

          প্রত্যাগম crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, পরিমাণ (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, পরিমাণ (attr));
      যদি (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          যদি (errno == ENOENT)
            অনুবর্ত;

          প্রত্যাগম crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  প্রত্যাগম 0;
#অন্যথা
  (শূন্য) dirfd;
  (শূন্য) progs;
  (শূন্য) n_progs;
  প্রত্যাগম crun_make_error (err, 0, "eBPF not supported");
#endif
}

স্থির পূর্ণাঙ্ক
ebpf_attach_program (পূর্ণাঙ্ক fd, পূর্ণাঙ্ক dirfd, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (শূন্য) fd;
  (শূন্য) dirfd;
  (শূন্য) read_all_progs;
  (শূন্য) remove_all_progs;

  প্রত্যাগম crun_make_error (err, 0, "eBPF not supported");
#অন্যথা
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  ধ্রুব পূর্ণাঙ্ক MAX_ATTEMPTS = 20;
  পূর্ণাঙ্ক attempt;

  প্রতি (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = রিক্ত;
      cleanup_close পূর্ণাঙ্ক replacefd = -1;
      সঙ্ঘ bpf_attr attr;
      size_t n_progs = 0;
      পূর্ণাঙ্ক ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      যদি (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, পরিমাণ (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, পরিমাণ (attr));
          যদি (UNLIKELY (replacefd < 0))
            {
              যদি (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  অনুবর্ত;
                }
              প্রত্যাগম crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, পরিমাণ (attr));
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

      ret = bpf (BPF_PROG_ATTACH, &attr, পরিমাণ (attr));
      যদি (UNLIKELY (ret < 0))
        {
          যদি (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              অনুবর্ত;
            }
#  ifdef BPF_F_REPLACE
          যদি (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              অনুবর্ত;
            }
#  endif
          প্রত্যাগম crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      যদি (replacefd < 0 && n_progs)
        প্রত্যাগম remove_all_progs (dirfd, progs, n_progs, err);

      প্রত্যাগম 0;
    }
#endif
}

স্থির শূন্য
bump_memlock ()
{
  সংরচনা rlimit limit;
  পূর্ণাঙ্ক ret;

  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &limit);
  যদি (ret == 0)
    প্রত্যাগম;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &limit);
  যদি (ret < 0)
    প্রত্যাগম;

  limit.rlim_cur = limit.rlim_max;
  /* Best effort, ignore errors.  */
  (শূন্য) setrlimit (RLIMIT_MEMLOCK, &limit);
}

পূর্ণাঙ্ক
libcrun_ebpf_load (সংরচনা bpf_program *program, পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *pin, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (শূন্য) dirfd;
  (শূন্য) program;
  (শূন্য) pin;
  (শূন্য) ebpf_attach_program;

  প্রত্যাগম crun_make_error (err, 0, "eBPF not supported");
#অন্যথা
  cleanup_close পূর্ণাঙ্ক fd = -1;
  সঙ্ঘ bpf_attr attr;
  পূর্ণাঙ্ক ret;

  memset (&attr, 0, পরিমাণ (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_to_u64 (program->program);
  attr.insn_cnt = bpf_program_instructions (program);
  attr.license = ptr_to_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_LOAD, &attr, পরিমাণ (attr));
  যদি (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_LOAD, &attr, পরিমাণ (attr));
    }
  যদি (fd < 0)
    {
      ধ্রুব size_t max_log_size = 1 << 20;
      cleanup_free বর্ণ *log = রিক্ত;
      size_t log_size = 8192;

    retry:
      log = xrealloc (log, log_size);
      log[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_to_u64 (log);
      attr.log_size = log_size;

      fd = bpf (BPF_PROG_LOAD, &attr, পরিমাণ (attr));
      যদি (fd < 0)
        {
          যদি (errno == ENOSPC && log_size < max_log_size)
            {
              /* The provided buffer was not big enough.  */
              log_size *= 2;
              গচ্ছ retry;
            }
          প্রত্যাগম crun_make_error (err, errno, "bpf create `%s`", log);
        }
    }

  যদি (dirfd >= 0)
    {
      ret = ebpf_attach_program (fd, dirfd, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  /* Optionally pin the program to the specified path.  */
  যদি (pin)
    {
      unlink (pin);

      memset (&attr, 0, পরিমাণ (attr));
      attr.pathname = ptr_to_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, পরিমাণ (attr));
      যদি (ret < 0)
        প্রত্যাগম crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  প্রত্যাগম 0;
#endif
}

পূর্ণাঙ্ক
libcrun_ebpf_read_program (সংরচনা bpf_program **program_ret, ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (শূন্য) program_ret;
  (শূন্য) path;

  প্রত্যাগম crun_make_error (err, 0, "eBPF not supported");
#অন্যথা
  cleanup_free সংরচনা bpf_program *program = রিক্ত;
  cleanup_free বর্ণ *buffer = রিক্ত;
  cleanup_close পূর্ণাঙ্ক prog_fd = -1;
  size_t buffer_size = 0;
  সংরচনা bpf_prog_info info;
  সঙ্ঘ bpf_attr attr;
  পূর্ণাঙ্ক ret;

  memset (&attr, 0, পরিমাণ (attr));
  attr.pathname = ptr_to_u64 (path);

  prog_fd = bpf (BPF_OBJ_GET, &attr, পরিমাণ (attr));
  যদি (UNLIKELY (prog_fd < 0))
    প্রত্যাগম crun_make_error (err, errno, "bpf get `%s`", path);

  memset (&info, 0, পরিমাণ (info));

  memset (&attr, 0, পরিমাণ (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_to_u64 (&info);
  attr.info.info_len = পরিমাণ (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, পরিমাণ (attr));
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "bpf get info `%s`", path);

  buffer_size = info.xlated_prog_len;
  buffer = xmalloc (buffer_size);

  memset (&info, 0, পরিমাণ (info));
  info.xlated_prog_insns = ptr_to_u64 (buffer);
  info.xlated_prog_len = buffer_size;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, পরিমাণ (attr));
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "bpf get info `%s`", path);

  program = bpf_program_new (buffer_size);
  program = bpf_program_append (program, buffer, buffer_size);

  *program_ret = program;
  program = রিক্ত;

  প্রত্যাগম 0;
#endif
}

পূর্ণাঙ্ক
libcrun_ebpf_query_cgroup_progs (ধ্রুব বর্ণ *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (শূন্য) cgroup_path;
  *progs_out = রিক্ত;
  *n_progs_out = 0;
  প্রত্যাগম crun_make_error (err, 0, "eBPF not supported");
#অন্যথা
  cleanup_close পূর্ণাঙ্ক cgroup_fd = -1;

  cgroup_fd = open (cgroup_path, O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (cgroup_fd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open cgroup path `%s`", cgroup_path);

  প্রত্যাগম read_all_progs (cgroup_fd, progs_out, n_progs_out, err);
#endif
}

bool
libcrun_ebpf_cmp_programs (সংরচনা bpf_program *program1, সংরচনা bpf_program *program2)
{
  যদি (program1->used != program2->used)
    প্রত্যাগম false;

  প্রত্যাগম memcmp (program1->program, program2->program, program1->used) == 0;
}
