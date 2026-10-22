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

#شامل <config.h>
#شامل "ebpf.h"
#شامل "utils.h"
#شامل <unistd.h>
#شامل <sys/syscall.h>
#شامل <sys/resource.h>

#ifdef HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
ایستا صحیح
syscall_bpf (صحیح cmd, اجتماع bpf_attr *attr, unsigned صحیح size)
{
  بازگشت (صحیح) syscall (__NR_bpf, cmd, attr, size);
}
#    define bpf syscall_bpf
#  endif

#endif

ایستا درونخطی uint64_t
ptr_to_u64 (ثابت تهی *ptr)
{
  بازگشت (uint64_t) (unsigned long) ptr;
}

شمارش
{
  HAS_WILDCARD = 1
};

ساختار bpf_program
{
  size_t allocated;
  size_t used;
  unsigned صحیح خصوصی;
  نویسه program[];
};

#ifdef HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((ساختار bpf_insn) { .code = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(SIZE, DST, SRC, OFF) \
    ((ساختار bpf_insn) {                   \
        .code = BPF_LDX | BPF_SIZE (SIZE) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((ساختار bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((ساختار bpf_insn) { .code = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((ساختار bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((ساختار bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((ساختار bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((ساختار bpf_insn) { .code = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((ساختار bpf_insn) { .code = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#endif

#ifdef HAVE_EBPF
ایستا size_t
bpf_program_instructions (ساختار bpf_program *program)
{
  بازگشت program->used / (اندازه (ساختار bpf_insn));
}
#endif

ساختار bpf_program *
bpf_program_new (size_t size)
{
  ساختار bpf_program *p = xmalloc (size + اندازه (ساختار bpf_program));

  p->used = 0;
  p->خصوصی = 0;
  p->allocated = size;

  بازگشت p;
}

ساختار bpf_program *
bpf_program_append (ساختار bpf_program *p, تهی *data, size_t size)
{
  اگر (p->allocated <= p->used + size)
    {
      p->allocated += size * 2;
      p = xrealloc (p, p->allocated + اندازه (ساختار bpf_program));
    }
  memcpy (p->program + p->used, data, size);
  p->used += size;
  بازگشت p;
}

ساختار bpf_program *
bpf_program_init_dev (ساختار bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  /* taken from systemd.  */
  ساختار bpf_insn pre_insn[] = {
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
  program = bpf_program_append (program, pre_insn, اندازه (pre_insn));
#endif
  بازگشت program;
}

ساختار bpf_program *
bpf_program_append_dev (ساختار bpf_program *program, ثابت نویسه *access, نویسه type, صحیح major, صحیح minor, bool accept,
                        libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  صحیح i;
  صحیح bpf_access = 0;
  صحیح bpf_type = type == 'b' ? BPF_DEVCG_DEV_BLOCK : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  صحیح عدد_instructions = 0;
  ساختار bpf_insn accept_block[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  اگر (access == پوچ)
    access = "";

  اگر (program->خصوصی & HAS_WILDCARD)
    بازگشت program;

  برای (i = 0; access[i]; i++)
    {
      انتخاب (access[i])
        {
        حالت 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          بشکن;

        حالت 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          بشکن;

        حالت 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          بشکن;
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
  عدد_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  اگر (has_type)
    {
      ساختار bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, عدد_instructions) };
      عدد_instructions--;
      program = bpf_program_append (program, i, اندازه (i));
    }
  اگر (has_access)
    {
      ساختار bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, عدد_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, عدد_instructions - 2),
      };
      عدد_instructions -= 3;
      program = bpf_program_append (program, i, اندازه (i));
    }
  اگر (has_major)
    {
      ساختار bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, عدد_instructions) };
      عدد_instructions--;
      program = bpf_program_append (program, i, اندازه (i));
    }
  اگر (has_minor)
    {
      ساختار bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, عدد_instructions) };
      عدد_instructions--;
      program = bpf_program_append (program, i, اندازه (i));
    }

  اگر (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    program->خصوصی |= HAS_WILDCARD;

  program = bpf_program_append (program, accept_block, اندازه (accept_block));
#endif
  (تهی) access;
  (تهی) type;
  (تهی) major;
  (تهی) minor;
  (تهی) accept;

  بازگشت program;
}

ساختار bpf_program *
bpf_program_complete_dev (ساختار bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  ساختار bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  اگر (program->خصوصی & HAS_WILDCARD)
    بازگشت program;

  program = bpf_program_append (program, &i, اندازه (i));
#endif
  بازگشت program;
}

ایستا صحیح
read_all_progs (صحیح dirfd, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  cleanup_free uint32_t *progs = پوچ;
  اجتماع bpf_attr attr;
  صحیح ret;
  size_t cur_size;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  برای (cur_size = 64; cur_size <= 4096; cur_size *= 2)
    {
      progs = xrealloc (progs, اندازه (uint32_t) * cur_size);

      memset (&attr, 0, اندازه (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_size;
      attr.query.prog_ids = ptr_to_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, اندازه (attr));
    }
  وقتی (ret < 0 && errno == ENOSPC)
    ;

  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "bpf query");

  *progs_out = progs;
  progs = پوچ;
  *n_progs_out = attr.query.prog_cnt;
  بازگشت 0;
#وگرنه
  (تهی) dirfd;

  *progs_out = پوچ;
  *n_progs_out = 0;

  بازگشت crun_make_error (err, 0, "eBPF not supported");
#endif
}

ایستا صحیح
remove_all_progs (صحیح dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  اجتماع bpf_attr attr;
  size_t i;

  برای (i = 0; i < n_progs; i++)
    {
      cleanup_close صحیح fd = -1;
      صحیح ret;

      memset (&attr, 0, اندازه (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, اندازه (attr));
      اگر (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          اگر (errno == ENOENT)
            ادامه;

          بازگشت crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, اندازه (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, اندازه (attr));
      اگر (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          اگر (errno == ENOENT)
            ادامه;

          بازگشت crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  بازگشت 0;
#وگرنه
  (تهی) dirfd;
  (تهی) progs;
  (تهی) n_progs;
  بازگشت crun_make_error (err, 0, "eBPF not supported");
#endif
}

ایستا صحیح
ebpf_attach_program (صحیح fd, صحیح dirfd, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (تهی) fd;
  (تهی) dirfd;
  (تهی) read_all_progs;
  (تهی) remove_all_progs;

  بازگشت crun_make_error (err, 0, "eBPF not supported");
#وگرنه
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  ثابت صحیح MAX_ATTEMPTS = 20;
  صحیح attempt;

  برای (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = پوچ;
      cleanup_close صحیح replacefd = -1;
      اجتماع bpf_attr attr;
      size_t n_progs = 0;
      صحیح ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      اگر (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, اندازه (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, اندازه (attr));
          اگر (UNLIKELY (replacefd < 0))
            {
              اگر (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  ادامه;
                }
              بازگشت crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, اندازه (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      اگر (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, اندازه (attr));
      اگر (UNLIKELY (ret < 0))
        {
          اگر (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              ادامه;
            }
#  ifdef BPF_F_REPLACE
          اگر (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              ادامه;
            }
#  endif
          بازگشت crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      اگر (replacefd < 0 && n_progs)
        بازگشت remove_all_progs (dirfd, progs, n_progs, err);

      بازگشت 0;
    }
#endif
}

ایستا تهی
bump_memlock ()
{
  ساختار rlimit limit;
  صحیح ret;

  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &limit);
  اگر (ret == 0)
    بازگشت;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &limit);
  اگر (ret < 0)
    بازگشت;

  limit.rlim_cur = limit.rlim_max;
  /* Best effort, ignore errors.  */
  (تهی) setrlimit (RLIMIT_MEMLOCK, &limit);
}

صحیح
libcrun_ebpf_load (ساختار bpf_program *program, صحیح dirfd, ثابت نویسه *pin, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (تهی) dirfd;
  (تهی) program;
  (تهی) pin;
  (تهی) ebpf_attach_program;

  بازگشت crun_make_error (err, 0, "eBPF not supported");
#وگرنه
  cleanup_close صحیح fd = -1;
  اجتماع bpf_attr attr;
  صحیح ret;

  memset (&attr, 0, اندازه (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_to_u64 (program->program);
  attr.insn_cnt = bpf_program_instructions (program);
  attr.license = ptr_to_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_LOAD, &attr, اندازه (attr));
  اگر (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_LOAD, &attr, اندازه (attr));
    }
  اگر (fd < 0)
    {
      ثابت size_t max_log_size = 1 << 20;
      cleanup_free نویسه *log = پوچ;
      size_t log_size = 8192;

    retry:
      log = xrealloc (log, log_size);
      log[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_to_u64 (log);
      attr.log_size = log_size;

      fd = bpf (BPF_PROG_LOAD, &attr, اندازه (attr));
      اگر (fd < 0)
        {
          اگر (errno == ENOSPC && log_size < max_log_size)
            {
              /* The provided buffer was not big enough.  */
              log_size *= 2;
              برو retry;
            }
          بازگشت crun_make_error (err, errno, "bpf create `%s`", log);
        }
    }

  اگر (dirfd >= 0)
    {
      ret = ebpf_attach_program (fd, dirfd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  /* Optionally pin the program to the specified path.  */
  اگر (pin)
    {
      unlink (pin);

      memset (&attr, 0, اندازه (attr));
      attr.pathname = ptr_to_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, اندازه (attr));
      اگر (ret < 0)
        بازگشت crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  بازگشت 0;
#endif
}

صحیح
libcrun_ebpf_read_program (ساختار bpf_program **program_ret, ثابت نویسه *path, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (تهی) program_ret;
  (تهی) path;

  بازگشت crun_make_error (err, 0, "eBPF not supported");
#وگرنه
  cleanup_free ساختار bpf_program *program = پوچ;
  cleanup_free نویسه *buffer = پوچ;
  cleanup_close صحیح prog_fd = -1;
  size_t buffer_size = 0;
  ساختار bpf_prog_info info;
  اجتماع bpf_attr attr;
  صحیح ret;

  memset (&attr, 0, اندازه (attr));
  attr.pathname = ptr_to_u64 (path);

  prog_fd = bpf (BPF_OBJ_GET, &attr, اندازه (attr));
  اگر (UNLIKELY (prog_fd < 0))
    بازگشت crun_make_error (err, errno, "bpf get `%s`", path);

  memset (&info, 0, اندازه (info));

  memset (&attr, 0, اندازه (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_to_u64 (&info);
  attr.info.info_len = اندازه (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, اندازه (attr));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "bpf get info `%s`", path);

  buffer_size = info.xlated_prog_len;
  buffer = xmalloc (buffer_size);

  memset (&info, 0, اندازه (info));
  info.xlated_prog_insns = ptr_to_u64 (buffer);
  info.xlated_prog_len = buffer_size;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, اندازه (attr));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "bpf get info `%s`", path);

  program = bpf_program_new (buffer_size);
  program = bpf_program_append (program, buffer, buffer_size);

  *program_ret = program;
  program = پوچ;

  بازگشت 0;
#endif
}

صحیح
libcrun_ebpf_query_cgroup_progs (ثابت نویسه *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (تهی) cgroup_path;
  *progs_out = پوچ;
  *n_progs_out = 0;
  بازگشت crun_make_error (err, 0, "eBPF not supported");
#وگرنه
  cleanup_close صحیح cgroup_fd = -1;

  cgroup_fd = open (cgroup_path, O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (cgroup_fd < 0))
    بازگشت crun_make_error (err, errno, "open cgroup path `%s`", cgroup_path);

  بازگشت read_all_progs (cgroup_fd, progs_out, n_progs_out, err);
#endif
}

bool
libcrun_ebpf_cmp_programs (ساختار bpf_program *program1, ساختار bpf_program *program2)
{
  اگر (program1->used != program2->used)
    بازگشت false;

  بازگشت memcmp (program1->program, program2->program, program1->used) == 0;
}
