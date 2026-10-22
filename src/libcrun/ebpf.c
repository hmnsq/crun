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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "ebpf.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/syscall.h>
#ଅନ୍ତର୍ଭାଵ <sys/resource.h>

#ifdef HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_bpf (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd, ସଂଘ bpf_attr *attr, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_bpf, cmd, attr, size);
}
#    define bpf syscall_bpf
#  endif

#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint64_t
ptr_to_u64 (ଅଚଳ ଶୂନ୍ୟ *ptr)
{
  ଫେରନ୍ତୁ (uint64_t) (unsigned long) ptr;
}

ଗଣନା
{
  HAS_WILDCARD = 1
};

ସଂରଚନା bpf_program
{
  size_t allocated;
  size_t used;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବ୍ୟକ୍ତିଗତ;
  ଅକ୍ଷର program[];
};

#ifdef HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((ସଂରଚନା bpf_insn) { .code = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(SIZE, DST, SRC, OFF) \
    ((ସଂରଚନା bpf_insn) {                   \
        .code = BPF_LDX | BPF_SIZE (SIZE) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((ସଂରଚନା bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((ସଂରଚନା bpf_insn) { .code = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((ସଂରଚନା bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((ସଂରଚନା bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((ସଂରଚନା bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((ସଂରଚନା bpf_insn) { .code = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((ସଂରଚନା bpf_insn) { .code = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#endif

#ifdef HAVE_EBPF
ସ୍ଥିର size_t
bpf_program_instructions (ସଂରଚନା bpf_program *program)
{
  ଫେରନ୍ତୁ program->used / (ଆକାର (ସଂରଚନା bpf_insn));
}
#endif

ସଂରଚନା bpf_program *
bpf_program_new (size_t size)
{
  ସଂରଚନା bpf_program *p = xmalloc (size + ଆକାର (ସଂରଚନା bpf_program));

  p->used = 0;
  p->ବ୍ୟକ୍ତିଗତ = 0;
  p->allocated = size;

  ଫେରନ୍ତୁ p;
}

ସଂରଚନା bpf_program *
bpf_program_append (ସଂରଚନା bpf_program *p, ଶୂନ୍ୟ *data, size_t size)
{
  ଯଦି (p->allocated <= p->used + size)
    {
      p->allocated += size * 2;
      p = xrealloc (p, p->allocated + ଆକାର (ସଂରଚନା bpf_program));
    }
  memcpy (p->program + p->used, data, size);
  p->used += size;
  ଫେରନ୍ତୁ p;
}

ସଂରଚନା bpf_program *
bpf_program_init_dev (ସଂରଚନା bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  /* taken from systemd.  */
  ସଂରଚନା bpf_insn pre_insn[] = {
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
  program = bpf_program_append (program, pre_insn, ଆକାର (pre_insn));
#endif
  ଫେରନ୍ତୁ program;
}

ସଂରଚନା bpf_program *
bpf_program_append_dev (ସଂରଚନା bpf_program *program, ଅଚଳ ଅକ୍ଷର *access, ଅକ୍ଷର type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor, bool accept,
                        libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bpf_access = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bpf_type = type == 'b' ? BPF_DEVCG_DEV_BLOCK : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା_instructions = 0;
  ସଂରଚନା bpf_insn accept_block[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  ଯଦି (access == ଖାଲି)
    access = "";

  ଯଦି (program->ବ୍ୟକ୍ତିଗତ & HAS_WILDCARD)
    ଫେରନ୍ତୁ program;

  ପାଇଁ (i = 0; access[i]; i++)
    {
      ବାଛ (access[i])
        {
        ସ୍ଥିତି 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          ଭାଙ୍ଗ;
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
  ସଂଖ୍ୟା_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  ଯଦି (has_type)
    {
      ସଂରଚନା bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, ସଂଖ୍ୟା_instructions) };
      ସଂଖ୍ୟା_instructions--;
      program = bpf_program_append (program, i, ଆକାର (i));
    }
  ଯଦି (has_access)
    {
      ସଂରଚନା bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, ସଂଖ୍ୟା_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, ସଂଖ୍ୟା_instructions - 2),
      };
      ସଂଖ୍ୟା_instructions -= 3;
      program = bpf_program_append (program, i, ଆକାର (i));
    }
  ଯଦି (has_major)
    {
      ସଂରଚନା bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, ସଂଖ୍ୟା_instructions) };
      ସଂଖ୍ୟା_instructions--;
      program = bpf_program_append (program, i, ଆକାର (i));
    }
  ଯଦି (has_minor)
    {
      ସଂରଚନା bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, ସଂଖ୍ୟା_instructions) };
      ସଂଖ୍ୟା_instructions--;
      program = bpf_program_append (program, i, ଆକାର (i));
    }

  ଯଦି (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    program->ବ୍ୟକ୍ତିଗତ |= HAS_WILDCARD;

  program = bpf_program_append (program, accept_block, ଆକାର (accept_block));
#endif
  (ଶୂନ୍ୟ) access;
  (ଶୂନ୍ୟ) type;
  (ଶୂନ୍ୟ) major;
  (ଶୂନ୍ୟ) minor;
  (ଶୂନ୍ୟ) accept;

  ଫେରନ୍ତୁ program;
}

ସଂରଚନା bpf_program *
bpf_program_complete_dev (ସଂରଚନା bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  ସଂରଚନା bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  ଯଦି (program->ବ୍ୟକ୍ତିଗତ & HAS_WILDCARD)
    ଫେରନ୍ତୁ program;

  program = bpf_program_append (program, &i, ଆକାର (i));
#endif
  ଫେରନ୍ତୁ program;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_all_progs (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  cleanup_free uint32_t *progs = ଖାଲି;
  ସଂଘ bpf_attr attr;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  size_t cur_size;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  ପାଇଁ (cur_size = 64; cur_size <= 4096; cur_size *= 2)
    {
      progs = xrealloc (progs, ଆକାର (uint32_t) * cur_size);

      memset (&attr, 0, ଆକାର (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_size;
      attr.query.prog_ids = ptr_to_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, ଆକାର (attr));
    }
  ଯେତେବେଳେ (ret < 0 && errno == ENOSPC)
    ;

  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "bpf query");

  *progs_out = progs;
  progs = ଖାଲି;
  *n_progs_out = attr.query.prog_cnt;
  ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) dirfd;

  *progs_out = ଖାଲି;
  *n_progs_out = 0;

  ଫେରନ୍ତୁ crun_make_error (err, 0, "eBPF not supported");
#endif
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
remove_all_progs (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  ସଂଘ bpf_attr attr;
  size_t i;

  ପାଇଁ (i = 0; i < n_progs; i++)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      memset (&attr, 0, ଆକାର (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, ଆକାର (attr));
      ଯଦି (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          ଯଦି (errno == ENOENT)
            ଜାରି;

          ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, ଆକାର (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, ଆକାର (attr));
      ଯଦି (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          ଯଦି (errno == ENOENT)
            ଜାରି;

          ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) dirfd;
  (ଶୂନ୍ୟ) progs;
  (ଶୂନ୍ୟ) n_progs;
  ଫେରନ୍ତୁ crun_make_error (err, 0, "eBPF not supported");
#endif
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ebpf_attach_program (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ଶୂନ୍ୟ) fd;
  (ଶୂନ୍ୟ) dirfd;
  (ଶୂନ୍ୟ) read_all_progs;
  (ଶୂନ୍ୟ) remove_all_progs;

  ଫେରନ୍ତୁ crun_make_error (err, 0, "eBPF not supported");
#ଅନ୍ୟଥା
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା MAX_ATTEMPTS = 20;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା attempt;

  ପାଇଁ (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = ଖାଲି;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା replacefd = -1;
      ସଂଘ bpf_attr attr;
      size_t n_progs = 0;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      ଯଦି (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, ଆକାର (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, ଆକାର (attr));
          ଯଦି (UNLIKELY (replacefd < 0))
            {
              ଯଦି (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  ଜାରି;
                }
              ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, ଆକାର (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      ଯଦି (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, ଆକାର (attr));
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              ଜାରି;
            }
#  ifdef BPF_F_REPLACE
          ଯଦି (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              ଜାରି;
            }
#  endif
          ଫେରନ୍ତୁ crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      ଯଦି (replacefd < 0 && n_progs)
        ଫେରନ୍ତୁ remove_all_progs (dirfd, progs, n_progs, err);

      ଫେରନ୍ତୁ 0;
    }
#endif
}

ସ୍ଥିର ଶୂନ୍ୟ
bump_memlock ()
{
  ସଂରଚନା rlimit limit;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &limit);
  ଯଦି (ret == 0)
    ଫେରନ୍ତୁ;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &limit);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ;

  limit.rlim_cur = limit.rlim_max;
  /* Best effort, ignore errors.  */
  (ଶୂନ୍ୟ) setrlimit (RLIMIT_MEMLOCK, &limit);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_ebpf_load (ସଂରଚନା bpf_program *program, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *pin, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ଶୂନ୍ୟ) dirfd;
  (ଶୂନ୍ୟ) program;
  (ଶୂନ୍ୟ) pin;
  (ଶୂନ୍ୟ) ebpf_attach_program;

  ଫେରନ୍ତୁ crun_make_error (err, 0, "eBPF not supported");
#ଅନ୍ୟଥା
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ସଂଘ bpf_attr attr;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  memset (&attr, 0, ଆକାର (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_to_u64 (program->program);
  attr.insn_cnt = bpf_program_instructions (program);
  attr.license = ptr_to_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_LOAD, &attr, ଆକାର (attr));
  ଯଦି (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_LOAD, &attr, ଆକାର (attr));
    }
  ଯଦି (fd < 0)
    {
      ଅଚଳ size_t max_log_size = 1 << 20;
      cleanup_free ଅକ୍ଷର *log = ଖାଲି;
      size_t log_size = 8192;

    retry:
      log = xrealloc (log, log_size);
      log[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_to_u64 (log);
      attr.log_size = log_size;

      fd = bpf (BPF_PROG_LOAD, &attr, ଆକାର (attr));
      ଯଦି (fd < 0)
        {
          ଯଦି (errno == ENOSPC && log_size < max_log_size)
            {
              /* The provided buffer was not big enough.  */
              log_size *= 2;
              ଯାଅ retry;
            }
          ଫେରନ୍ତୁ crun_make_error (err, errno, "bpf create `%s`", log);
        }
    }

  ଯଦି (dirfd >= 0)
    {
      ret = ebpf_attach_program (fd, dirfd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  /* Optionally pin the program to the specified path.  */
  ଯଦି (pin)
    {
      unlink (pin);

      memset (&attr, 0, ଆକାର (attr));
      attr.pathname = ptr_to_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, ଆକାର (attr));
      ଯଦି (ret < 0)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  ଫେରନ୍ତୁ 0;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_ebpf_read_program (ସଂରଚନା bpf_program **program_ret, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ଶୂନ୍ୟ) program_ret;
  (ଶୂନ୍ୟ) path;

  ଫେରନ୍ତୁ crun_make_error (err, 0, "eBPF not supported");
#ଅନ୍ୟଥା
  cleanup_free ସଂରଚନା bpf_program *program = ଖାଲି;
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା prog_fd = -1;
  size_t buffer_size = 0;
  ସଂରଚନା bpf_prog_info info;
  ସଂଘ bpf_attr attr;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  memset (&attr, 0, ଆକାର (attr));
  attr.pathname = ptr_to_u64 (path);

  prog_fd = bpf (BPF_OBJ_GET, &attr, ଆକାର (attr));
  ଯଦି (UNLIKELY (prog_fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "bpf get `%s`", path);

  memset (&info, 0, ଆକାର (info));

  memset (&attr, 0, ଆକାର (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_to_u64 (&info);
  attr.info.info_len = ଆକାର (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, ଆକାର (attr));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "bpf get info `%s`", path);

  buffer_size = info.xlated_prog_len;
  buffer = xmalloc (buffer_size);

  memset (&info, 0, ଆକାର (info));
  info.xlated_prog_insns = ptr_to_u64 (buffer);
  info.xlated_prog_len = buffer_size;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, ଆକାର (attr));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "bpf get info `%s`", path);

  program = bpf_program_new (buffer_size);
  program = bpf_program_append (program, buffer, buffer_size);

  *program_ret = program;
  program = ଖାଲି;

  ଫେରନ୍ତୁ 0;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_ebpf_query_cgroup_progs (ଅଚଳ ଅକ୍ଷର *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ଶୂନ୍ୟ) cgroup_path;
  *progs_out = ଖାଲି;
  *n_progs_out = 0;
  ଫେରନ୍ତୁ crun_make_error (err, 0, "eBPF not supported");
#ଅନ୍ୟଥା
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_fd = -1;

  cgroup_fd = open (cgroup_path, O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (cgroup_fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open cgroup path `%s`", cgroup_path);

  ଫେରନ୍ତୁ read_all_progs (cgroup_fd, progs_out, n_progs_out, err);
#endif
}

bool
libcrun_ebpf_cmp_programs (ସଂରଚନା bpf_program *program1, ସଂରଚନା bpf_program *program2)
{
  ଯଦି (program1->used != program2->used)
    ଫେରନ୍ତୁ false;

  ଫେରନ୍ତୁ memcmp (program1->program, program2->program, program1->used) == 0;
}
