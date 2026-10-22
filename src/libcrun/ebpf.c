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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "ebpf.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#ಅನ್ತರ್ಭಾವ <sys/resource.h>

#ifdef HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
syscall_bpf (ಪೂರ್ಣಾಙ್ಕ cmd, ಸಙ್ಘ bpf_attr *attr, unsigned ಪೂರ್ಣಾಙ್ಕ size)
{
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_bpf, cmd, attr, size);
}
#    define bpf syscall_bpf
#  endif

#endif

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint64_t
ptr_to_u64 (ಧ್ರುವ ಶೂನ್ಯ *ptr)
{
  ಪ್ರತ್ಯಾಗಮ (uint64_t) (unsigned long) ptr;
}

ಗಣನಾ
{
  HAS_WILDCARD = 1
};

ಸಂರಚನಾ bpf_program
{
  size_t allocated;
  size_t used;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಗುಪ್ತ;
  ವರ್ಣ program[];
};

#ifdef HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((ಸಂರಚನಾ bpf_insn) { .code = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(SIZE, DST, SRC, OFF) \
    ((ಸಂರಚನಾ bpf_insn) {                   \
        .code = BPF_LDX | BPF_SIZE (SIZE) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((ಸಂರಚನಾ bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((ಸಂರಚನಾ bpf_insn) { .code = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((ಸಂರಚನಾ bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((ಸಂರಚನಾ bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((ಸಂರಚನಾ bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((ಸಂರಚನಾ bpf_insn) { .code = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((ಸಂರಚನಾ bpf_insn) { .code = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#endif

#ifdef HAVE_EBPF
ಸ್ಥಿರ size_t
bpf_program_instructions (ಸಂರಚನಾ bpf_program *program)
{
  ಪ್ರತ್ಯಾಗಮ program->used / (ಪರಿಮಾಣ (ಸಂರಚನಾ bpf_insn));
}
#endif

ಸಂರಚನಾ bpf_program *
bpf_program_new (size_t size)
{
  ಸಂರಚನಾ bpf_program *p = xmalloc (size + ಪರಿಮಾಣ (ಸಂರಚನಾ bpf_program));

  p->used = 0;
  p->ಗುಪ್ತ = 0;
  p->allocated = size;

  ಪ್ರತ್ಯಾಗಮ p;
}

ಸಂರಚನಾ bpf_program *
bpf_program_append (ಸಂರಚನಾ bpf_program *p, ಶೂನ್ಯ *data, size_t size)
{
  ಯದಿ (p->allocated <= p->used + size)
    {
      p->allocated += size * 2;
      p = xrealloc (p, p->allocated + ಪರಿಮಾಣ (ಸಂರಚನಾ bpf_program));
    }
  memcpy (p->program + p->used, data, size);
  p->used += size;
  ಪ್ರತ್ಯಾಗಮ p;
}

ಸಂರಚನಾ bpf_program *
bpf_program_init_dev (ಸಂರಚನಾ bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  /* taken from systemd.  */
  ಸಂರಚನಾ bpf_insn pre_insn[] = {
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
  program = bpf_program_append (program, pre_insn, ಪರಿಮಾಣ (pre_insn));
#endif
  ಪ್ರತ್ಯಾಗಮ program;
}

ಸಂರಚನಾ bpf_program *
bpf_program_append_dev (ಸಂರಚನಾ bpf_program *program, ಧ್ರುವ ವರ್ಣ *access, ವರ್ಣ type, ಪೂರ್ಣಾಙ್ಕ major, ಪೂರ್ಣಾಙ್ಕ minor, bool accept,
                        libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  ಪೂರ್ಣಾಙ್ಕ i;
  ಪೂರ್ಣಾಙ್ಕ bpf_access = 0;
  ಪೂರ್ಣಾಙ್ಕ bpf_type = type == 'b' ? BPF_DEVCG_DEV_BLOCK : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  ಪೂರ್ಣಾಙ್ಕ ಸಂಖ್ಯೆ_instructions = 0;
  ಸಂರಚನಾ bpf_insn accept_block[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  ಯದಿ (access == ರಿಕ್ತ)
    access = "";

  ಯದಿ (program->ಗುಪ್ತ & HAS_WILDCARD)
    ಪ್ರತ್ಯಾಗಮ program;

  ಪ್ರತಿ (i = 0; access[i]; i++)
    {
      ವಿಕಲ್ಪ (access[i])
        {
        ಪಕ್ಷ 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          ಭಙ್ಗ;
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
  ಸಂಖ್ಯೆ_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  ಯದಿ (has_type)
    {
      ಸಂರಚನಾ bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, ಸಂಖ್ಯೆ_instructions) };
      ಸಂಖ್ಯೆ_instructions--;
      program = bpf_program_append (program, i, ಪರಿಮಾಣ (i));
    }
  ಯದಿ (has_access)
    {
      ಸಂರಚನಾ bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, ಸಂಖ್ಯೆ_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, ಸಂಖ್ಯೆ_instructions - 2),
      };
      ಸಂಖ್ಯೆ_instructions -= 3;
      program = bpf_program_append (program, i, ಪರಿಮಾಣ (i));
    }
  ಯದಿ (has_major)
    {
      ಸಂರಚನಾ bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, ಸಂಖ್ಯೆ_instructions) };
      ಸಂಖ್ಯೆ_instructions--;
      program = bpf_program_append (program, i, ಪರಿಮಾಣ (i));
    }
  ಯದಿ (has_minor)
    {
      ಸಂರಚನಾ bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, ಸಂಖ್ಯೆ_instructions) };
      ಸಂಖ್ಯೆ_instructions--;
      program = bpf_program_append (program, i, ಪರಿಮಾಣ (i));
    }

  ಯದಿ (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    program->ಗುಪ್ತ |= HAS_WILDCARD;

  program = bpf_program_append (program, accept_block, ಪರಿಮಾಣ (accept_block));
#endif
  (ಶೂನ್ಯ) access;
  (ಶೂನ್ಯ) type;
  (ಶೂನ್ಯ) major;
  (ಶೂನ್ಯ) minor;
  (ಶೂನ್ಯ) accept;

  ಪ್ರತ್ಯಾಗಮ program;
}

ಸಂರಚನಾ bpf_program *
bpf_program_complete_dev (ಸಂರಚನಾ bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  ಸಂರಚನಾ bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  ಯದಿ (program->ಗುಪ್ತ & HAS_WILDCARD)
    ಪ್ರತ್ಯಾಗಮ program;

  program = bpf_program_append (program, &i, ಪರಿಮಾಣ (i));
#endif
  ಪ್ರತ್ಯಾಗಮ program;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_all_progs (ಪೂರ್ಣಾಙ್ಕ dirfd, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  cleanup_free uint32_t *progs = ರಿಕ್ತ;
  ಸಙ್ಘ bpf_attr attr;
  ಪೂರ್ಣಾಙ್ಕ ret;
  size_t cur_size;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  ಪ್ರತಿ (cur_size = 64; cur_size <= 4096; cur_size *= 2)
    {
      progs = xrealloc (progs, ಪರಿಮಾಣ (uint32_t) * cur_size);

      memset (&attr, 0, ಪರಿಮಾಣ (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_size;
      attr.query.prog_ids = ptr_to_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, ಪರಿಮಾಣ (attr));
    }
  ಯಾವತ್ (ret < 0 && errno == ENOSPC)
    ;

  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bpf query");

  *progs_out = progs;
  progs = ರಿಕ್ತ;
  *n_progs_out = attr.query.prog_cnt;
  ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) dirfd;

  *progs_out = ರಿಕ್ತ;
  *n_progs_out = 0;

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "eBPF not supported");
#endif
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
remove_all_progs (ಪೂರ್ಣಾಙ್ಕ dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  ಸಙ್ಘ bpf_attr attr;
  size_t i;

  ಪ್ರತಿ (i = 0; i < n_progs; i++)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
      ಪೂರ್ಣಾಙ್ಕ ret;

      memset (&attr, 0, ಪರಿಮಾಣ (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, ಪರಿಮಾಣ (attr));
      ಯದಿ (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          ಯದಿ (errno == ENOENT)
            ಅನುವರ್ತ;

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, ಪರಿಮಾಣ (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, ಪರಿಮಾಣ (attr));
      ಯದಿ (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          ಯದಿ (errno == ENOENT)
            ಅನುವರ್ತ;

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) dirfd;
  (ಶೂನ್ಯ) progs;
  (ಶೂನ್ಯ) n_progs;
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "eBPF not supported");
#endif
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
ebpf_attach_program (ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ dirfd, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ಶೂನ್ಯ) fd;
  (ಶೂನ್ಯ) dirfd;
  (ಶೂನ್ಯ) read_all_progs;
  (ಶೂನ್ಯ) remove_all_progs;

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "eBPF not supported");
#ಅನ್ಯಥಾ
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ MAX_ATTEMPTS = 20;
  ಪೂರ್ಣಾಙ್ಕ attempt;

  ಪ್ರತಿ (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = ರಿಕ್ತ;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ replacefd = -1;
      ಸಙ್ಘ bpf_attr attr;
      size_t n_progs = 0;
      ಪೂರ್ಣಾಙ್ಕ ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      ಯದಿ (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, ಪರಿಮಾಣ (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, ಪರಿಮಾಣ (attr));
          ಯದಿ (UNLIKELY (replacefd < 0))
            {
              ಯದಿ (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  ಅನುವರ್ತ;
                }
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, ಪರಿಮಾಣ (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      ಯದಿ (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, ಪರಿಮಾಣ (attr));
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              ಅನುವರ್ತ;
            }
#  ifdef BPF_F_REPLACE
          ಯದಿ (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              ಅನುವರ್ತ;
            }
#  endif
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      ಯದಿ (replacefd < 0 && n_progs)
        ಪ್ರತ್ಯಾಗಮ remove_all_progs (dirfd, progs, n_progs, err);

      ಪ್ರತ್ಯಾಗಮ 0;
    }
#endif
}

ಸ್ಥಿರ ಶೂನ್ಯ
bump_memlock ()
{
  ಸಂರಚನಾ rlimit limit;
  ಪೂರ್ಣಾಙ್ಕ ret;

  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &limit);
  ಯದಿ (ret == 0)
    ಪ್ರತ್ಯಾಗಮ;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &limit);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ;

  limit.rlim_cur = limit.rlim_max;
  /* Best effort, ignore errors.  */
  (ಶೂನ್ಯ) setrlimit (RLIMIT_MEMLOCK, &limit);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_ebpf_load (ಸಂರಚನಾ bpf_program *program, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *pin, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ಶೂನ್ಯ) dirfd;
  (ಶೂನ್ಯ) program;
  (ಶೂನ್ಯ) pin;
  (ಶೂನ್ಯ) ebpf_attach_program;

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "eBPF not supported");
#ಅನ್ಯಥಾ
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಸಙ್ಘ bpf_attr attr;
  ಪೂರ್ಣಾಙ್ಕ ret;

  memset (&attr, 0, ಪರಿಮಾಣ (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_to_u64 (program->program);
  attr.insn_cnt = bpf_program_instructions (program);
  attr.license = ptr_to_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_LOAD, &attr, ಪರಿಮಾಣ (attr));
  ಯದಿ (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_LOAD, &attr, ಪರಿಮಾಣ (attr));
    }
  ಯದಿ (fd < 0)
    {
      ಧ್ರುವ size_t max_log_size = 1 << 20;
      cleanup_free ವರ್ಣ *log = ರಿಕ್ತ;
      size_t log_size = 8192;

    retry:
      log = xrealloc (log, log_size);
      log[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_to_u64 (log);
      attr.log_size = log_size;

      fd = bpf (BPF_PROG_LOAD, &attr, ಪರಿಮಾಣ (attr));
      ಯದಿ (fd < 0)
        {
          ಯದಿ (errno == ENOSPC && log_size < max_log_size)
            {
              /* The provided buffer was not big enough.  */
              log_size *= 2;
              ಗಚ್ಛ retry;
            }
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bpf create `%s`", log);
        }
    }

  ಯದಿ (dirfd >= 0)
    {
      ret = ebpf_attach_program (fd, dirfd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* Optionally pin the program to the specified path.  */
  ಯದಿ (pin)
    {
      unlink (pin);

      memset (&attr, 0, ಪರಿಮಾಣ (attr));
      attr.pathname = ptr_to_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, ಪರಿಮಾಣ (attr));
      ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_ebpf_read_program (ಸಂರಚನಾ bpf_program **program_ret, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ಶೂನ್ಯ) program_ret;
  (ಶೂನ್ಯ) path;

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "eBPF not supported");
#ಅನ್ಯಥಾ
  cleanup_free ಸಂರಚನಾ bpf_program *program = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ prog_fd = -1;
  size_t buffer_size = 0;
  ಸಂರಚನಾ bpf_prog_info info;
  ಸಙ್ಘ bpf_attr attr;
  ಪೂರ್ಣಾಙ್ಕ ret;

  memset (&attr, 0, ಪರಿಮಾಣ (attr));
  attr.pathname = ptr_to_u64 (path);

  prog_fd = bpf (BPF_OBJ_GET, &attr, ಪರಿಮಾಣ (attr));
  ಯದಿ (UNLIKELY (prog_fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bpf get `%s`", path);

  memset (&info, 0, ಪರಿಮಾಣ (info));

  memset (&attr, 0, ಪರಿಮಾಣ (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_to_u64 (&info);
  attr.info.info_len = ಪರಿಮಾಣ (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, ಪರಿಮಾಣ (attr));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bpf get info `%s`", path);

  buffer_size = info.xlated_prog_len;
  buffer = xmalloc (buffer_size);

  memset (&info, 0, ಪರಿಮಾಣ (info));
  info.xlated_prog_insns = ptr_to_u64 (buffer);
  info.xlated_prog_len = buffer_size;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, ಪರಿಮಾಣ (attr));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bpf get info `%s`", path);

  program = bpf_program_new (buffer_size);
  program = bpf_program_append (program, buffer, buffer_size);

  *program_ret = program;
  program = ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_ebpf_query_cgroup_progs (ಧ್ರುವ ವರ್ಣ *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ಶೂನ್ಯ) cgroup_path;
  *progs_out = ರಿಕ್ತ;
  *n_progs_out = 0;
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "eBPF not supported");
#ಅನ್ಯಥಾ
  cleanup_close ಪೂರ್ಣಾಙ್ಕ cgroup_fd = -1;

  cgroup_fd = open (cgroup_path, O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (cgroup_fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open cgroup path `%s`", cgroup_path);

  ಪ್ರತ್ಯಾಗಮ read_all_progs (cgroup_fd, progs_out, n_progs_out, err);
#endif
}

bool
libcrun_ebpf_cmp_programs (ಸಂರಚನಾ bpf_program *program1, ಸಂರಚನಾ bpf_program *program2)
{
  ಯದಿ (program1->used != program2->used)
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತ್ಯಾಗಮ memcmp (program1->program, program2->program, program1->used) == 0;
}
