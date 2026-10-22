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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "ebpf.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#ಅನ್ತರ್ಭಾವ <sys/resource.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
syscall_bpf (ಪೂರ್ಣಾಂಕ cmd, ಒಕ್ಕೂಟ bpf_attr *attr, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_bpf, cmd, attr, ಗಾತ್ರ_2);
}
#    define bpf syscall_bpf
#  endif

#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ uint64_t
ptr_ಗಮ್ಯ_u64 (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr)
{
  ಹಿಂತಿರುಗಿ (uint64_t) (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) ptr;
}

ಎಣಿಕೆ
{
  HAS_WILDCARD = 1
};

ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ
{
  size_t allocated;
  size_t ಬಳಸಿದ;
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಖಾಸಗಿ;
  ಅಕ್ಷರ ಕಾರ್ಯಕ್ರಮ[];
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((ರಚನೆ bpf_insn) { .ಸಂಕೇತ_2 = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(ಗಾತ್ರ_3, DST, SRC, OFF) \
    ((ರಚನೆ bpf_insn) {                   \
        .ಸಂಕೇತ_2 = BPF_LDX | BPF_ಗಾತ್ರ (ಗಾತ್ರ_3) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((ರಚನೆ bpf_insn) { .ಸಂಕೇತ_2 = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((ರಚನೆ bpf_insn) { .ಸಂಕೇತ_2 = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((ರಚನೆ bpf_insn) { .ಸಂಕೇತ_2 = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((ರಚನೆ bpf_insn) { .ಸಂಕೇತ_2 = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((ರಚನೆ bpf_insn) { .ಸಂಕೇತ_2 = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((ರಚನೆ bpf_insn) { .ಸಂಕೇತ_2 = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((ರಚನೆ bpf_insn) { .ಸಂಕೇತ_2 = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EBPF
ಸ್ಥಿರ size_t
bpf_ಕಾರ್ಯಕ್ರಮ_instructions (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ)
{
  ಹಿಂತಿರುಗಿ ಕಾರ್ಯಕ್ರಮ->ಬಳಸಿದ / (ಗಾತ್ರ (ರಚನೆ bpf_insn));
}
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *
bpf_ಕಾರ್ಯಕ್ರಮ_new (size_t ಗಾತ್ರ_2)
{
  ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *p = xmalloc (ಗಾತ್ರ_2 + ಗಾತ್ರ (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ));

  p->ಬಳಸಿದ = 0;
  p->ಖಾಸಗಿ = 0;
  p->allocated = ಗಾತ್ರ_2;

  ಹಿಂತಿರುಗಿ p;
}

ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *
bpf_ಕಾರ್ಯಕ್ರಮ_append (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *p, ಶೂನ್ಯ *ದತ್ತಾಂಶ, size_t ಗಾತ್ರ_2)
{
  ಆದರೆ (p->allocated <= p->ಬಳಸಿದ + ಗಾತ್ರ_2)
    {
      p->allocated += ಗಾತ್ರ_2 * 2;
      p = xrealloc (p, p->allocated + ಗಾತ್ರ (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ));
    }
  ಸ್ಮರಣೆ_ನಕಲು (p->ಕಾರ್ಯಕ್ರಮ + p->ಬಳಸಿದ, ದತ್ತಾಂಶ, ಗಾತ್ರ_2);
  p->ಬಳಸಿದ += ಗಾತ್ರ_2;
  ಹಿಂತಿರುಗಿ p;
}

ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *
bpf_ಕಾರ್ಯಕ್ರಮ_init_dev (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ, libcrun_error_t *err arg_ಬಳಸದ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EBPF
  /* taken from systemd.  */
  ರಚನೆ bpf_insn pre_insn[] = {
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
  ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_append (ಕಾರ್ಯಕ್ರಮ, pre_insn, ಗಾತ್ರ (pre_insn));
#ಆದರೆ_ಅಂತ್ಯ
  ಹಿಂತಿರುಗಿ ಕಾರ್ಯಕ್ರಮ;
}

ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *
bpf_ಕಾರ್ಯಕ್ರಮ_append_dev (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *access, ಅಕ್ಷರ type, ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor, bool accept,
                        libcrun_error_t *err arg_ಬಳಸದ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EBPF
  ಪೂರ್ಣಾಂಕ i;
  ಪೂರ್ಣಾಂಕ bpf_access = 0;
  ಪೂರ್ಣಾಂಕ bpf_type = type == 'b' ? BPF_DEVCG_DEV_ಖಂಡ : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ_instructions = 0;
  ರಚನೆ bpf_insn accept_ಖಂಡ[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  ಆದರೆ (access == ಖಾಲಿ)
    access = "";

  ಆದರೆ (ಕಾರ್ಯಕ್ರಮ->ಖಾಸಗಿ & HAS_WILDCARD)
    ಹಿಂತಿರುಗಿ ಕಾರ್ಯಕ್ರಮ;

  ಗಾಗಿ (i = 0; access[i]; i++)
    {
      ಆಯ್ಕೆ (access[i])
        {
        ಸ್ಥಿತಿ 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          ನಿಲ್ಲಿಸು;
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

  ಆದರೆ (has_type)
    {
      ರಚನೆ bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, ಸಂಖ್ಯೆ_instructions) };
      ಸಂಖ್ಯೆ_instructions--;
      ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_append (ಕಾರ್ಯಕ್ರಮ, i, ಗಾತ್ರ (i));
    }
  ಆದರೆ (has_access)
    {
      ರಚನೆ bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, ಸಂಖ್ಯೆ_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, ಸಂಖ್ಯೆ_instructions - 2),
      };
      ಸಂಖ್ಯೆ_instructions -= 3;
      ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_append (ಕಾರ್ಯಕ್ರಮ, i, ಗಾತ್ರ (i));
    }
  ಆದರೆ (has_major)
    {
      ರಚನೆ bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, ಸಂಖ್ಯೆ_instructions) };
      ಸಂಖ್ಯೆ_instructions--;
      ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_append (ಕಾರ್ಯಕ್ರಮ, i, ಗಾತ್ರ (i));
    }
  ಆದರೆ (has_minor)
    {
      ರಚನೆ bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, ಸಂಖ್ಯೆ_instructions) };
      ಸಂಖ್ಯೆ_instructions--;
      ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_append (ಕಾರ್ಯಕ್ರಮ, i, ಗಾತ್ರ (i));
    }

  ಆದರೆ (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    ಕಾರ್ಯಕ್ರಮ->ಖಾಸಗಿ |= HAS_WILDCARD;

  ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_append (ಕಾರ್ಯಕ್ರಮ, accept_ಖಂಡ, ಗಾತ್ರ (accept_ಖಂಡ));
#ಆದರೆ_ಅಂತ್ಯ
  (ಶೂನ್ಯ) access;
  (ಶೂನ್ಯ) type;
  (ಶೂನ್ಯ) major;
  (ಶೂನ್ಯ) minor;
  (ಶೂನ್ಯ) accept;

  ಹಿಂತಿರುಗಿ ಕಾರ್ಯಕ್ರಮ;
}

ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *
bpf_ಕಾರ್ಯಕ್ರಮ_complete_dev (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ, libcrun_error_t *err arg_ಬಳಸದ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EBPF
  ರಚನೆ bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  ಆದರೆ (ಕಾರ್ಯಕ್ರಮ->ಖಾಸಗಿ & HAS_WILDCARD)
    ಹಿಂತಿರುಗಿ ಕಾರ್ಯಕ್ರಮ;

  ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_append (ಕಾರ್ಯಕ್ರಮ, &i, ಗಾತ್ರ (i));
#ಆದರೆ_ಅಂತ್ಯ
  ಹಿಂತಿರುಗಿ ಕಾರ್ಯಕ್ರಮ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_all_progs (ಪೂರ್ಣಾಂಕ dirfd, uint32_t **progs_ಹೊರಹೋಗು, size_t *n_progs_ಹೊರಹೋಗು, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EBPF
  cleanup_free uint32_t *progs = ಖಾಲಿ;
  ಒಕ್ಕೂಟ bpf_attr attr;
  ಪೂರ್ಣಾಂಕ ret;
  size_t cur_ಗಾತ್ರ;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  ಗಾಗಿ (cur_ಗಾತ್ರ = 64; cur_ಗಾತ್ರ <= 4096; cur_ಗಾತ್ರ *= 2)
    {
      progs = xrealloc (progs, ಗಾತ್ರ (uint32_t) * cur_ಗಾತ್ರ);

      memset (&attr, 0, ಗಾತ್ರ (attr));
      attr.query.ಗುರಿ_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_ಗಾತ್ರ;
      attr.query.prog_ids = ptr_ಗಮ್ಯ_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, ಗಾತ್ರ (attr));
    }
  ಆಗ (ret < 0 && errno == ENOSPC)
    ;

  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bpf query");

  *progs_ಹೊರಹೋಗು = progs;
  progs = ಖಾಲಿ;
  *n_progs_ಹೊರಹೋಗು = attr.query.prog_cnt;
  ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) dirfd;

  *progs_ಹೊರಹೋಗು = ಖಾಲಿ;
  *n_progs_ಹೊರಹೋಗು = 0;

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "eBPF not supported");
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
remove_all_progs (ಪೂರ್ಣಾಂಕ dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EBPF
  ಒಕ್ಕೂಟ bpf_attr attr;
  size_t i;

  ಗಾಗಿ (i = 0; i < n_progs; i++)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
      ಪೂರ್ಣಾಂಕ ret;

      memset (&attr, 0, ಗಾತ್ರ (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, ಗಾತ್ರ (attr));
      ಆದರೆ (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          ಆದರೆ (errno == ENOENT)
            ಮುಂದುವರಿ;

          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, ಗಾತ್ರ (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.ಗುರಿ_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, ಗಾತ್ರ (attr));
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          ಆದರೆ (errno == ENOENT)
            ಮುಂದುವರಿ;

          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) dirfd;
  (ಶೂನ್ಯ) progs;
  (ಶೂನ್ಯ) n_progs;
  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "eBPF not supported");
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ebpf_attach_ಕಾರ್ಯಕ್ರಮ (ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ dirfd, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_EBPF
  (ಶೂನ್ಯ) fd;
  (ಶೂನ್ಯ) dirfd;
  (ಶೂನ್ಯ) read_all_progs;
  (ಶೂನ್ಯ) remove_all_progs;

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "eBPF not supported");
#ಇಲ್ಲದಿದ್ದರೆ
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ MAX_ATTEMPTS = 20;
  ಪೂರ್ಣಾಂಕ attempt;

  ಗಾಗಿ (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = ಖಾಲಿ;
      cleanup_close ಪೂರ್ಣಾಂಕ replacefd = -1;
      ಒಕ್ಕೂಟ bpf_attr attr;
      size_t n_progs = 0;
      ಪೂರ್ಣಾಂಕ ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      ಆದರೆ (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, ಗಾತ್ರ (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, ಗಾತ್ರ (attr));
          ಆದರೆ (UNLIKELY (replacefd < 0))
            {
              ಆದರೆ (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  ಮುಂದುವರಿ;
                }
              ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, ಗಾತ್ರ (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.ಗುರಿ_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      ಆದರೆ (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, ಗಾತ್ರ (attr));
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              ಮುಂದುವರಿ;
            }
#  ifdef BPF_F_REPLACE
          ಆದರೆ (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              ಮುಂದುವರಿ;
            }
#  endif
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      ಆದರೆ (replacefd < 0 && n_progs)
        ಹಿಂತಿರುಗಿ remove_all_progs (dirfd, progs, n_progs, err);

      ಹಿಂತಿರುಗಿ 0;
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಶೂನ್ಯ
bump_memlock ()
{
  ರಚನೆ rlimit ಎಲ್ಲೆ;
  ಪೂರ್ಣಾಂಕ ret;

  ಎಲ್ಲೆ.rlim_cur = RLIM_INFINITY;
  ಎಲ್ಲೆ.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &ಎಲ್ಲೆ);
  ಆದರೆ (ret == 0)
    ಹಿಂತಿರುಗಿ;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &ಎಲ್ಲೆ);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ;

  ಎಲ್ಲೆ.rlim_cur = ಎಲ್ಲೆ.rlim_max;
  /* Best effort, ignore errors.  */
  (ಶೂನ್ಯ) setrlimit (RLIMIT_MEMLOCK, &ಎಲ್ಲೆ);
}

ಪೂರ್ಣಾಂಕ
libcrun_ebpf_ತುಂಬು (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pin, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_EBPF
  (ಶೂನ್ಯ) dirfd;
  (ಶೂನ್ಯ) ಕಾರ್ಯಕ್ರಮ;
  (ಶೂನ್ಯ) pin;
  (ಶೂನ್ಯ) ebpf_attach_ಕಾರ್ಯಕ್ರಮ;

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "eBPF not supported");
#ಇಲ್ಲದಿದ್ದರೆ
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಒಕ್ಕೂಟ bpf_attr attr;
  ಪೂರ್ಣಾಂಕ ret;

  memset (&attr, 0, ಗಾತ್ರ (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_ಗಮ್ಯ_u64 (ಕಾರ್ಯಕ್ರಮ->ಕಾರ್ಯಕ್ರಮ);
  attr.insn_cnt = bpf_ಕಾರ್ಯಕ್ರಮ_instructions (ಕಾರ್ಯಕ್ರಮ);
  attr.license = ptr_ಗಮ್ಯ_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_ತುಂಬು, &attr, ಗಾತ್ರ (attr));
  ಆದರೆ (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_ತುಂಬು, &attr, ಗಾತ್ರ (attr));
    }
  ಆದರೆ (fd < 0)
    {
      ಸ್ಥಿರಾಂಕ size_t max_log_ಗಾತ್ರ = 1 << 20;
      cleanup_free ಅಕ್ಷರ *ಲಘುಗಣಕ = ಖಾಲಿ;
      size_t log_ಗಾತ್ರ = 8192;

    retry:
      ಲಘುಗಣಕ = xrealloc (ಲಘುಗಣಕ, log_ಗಾತ್ರ);
      ಲಘುಗಣಕ[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_ಗಮ್ಯ_u64 (ಲಘುಗಣಕ);
      attr.log_ಗಾತ್ರ = log_ಗಾತ್ರ;

      fd = bpf (BPF_PROG_ತುಂಬು, &attr, ಗಾತ್ರ (attr));
      ಆದರೆ (fd < 0)
        {
          ಆದರೆ (errno == ENOSPC && log_ಗಾತ್ರ < max_log_ಗಾತ್ರ)
            {
              /* The provided buffer was not big enough.  */
              log_ಗಾತ್ರ *= 2;
              ಹೋಗು retry;
            }
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bpf create `%s`", ಲಘುಗಣಕ);
        }
    }

  ಆದರೆ (dirfd >= 0)
    {
      ret = ebpf_attach_ಕಾರ್ಯಕ್ರಮ (fd, dirfd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  /* Optionally pin the program to the specified path.  */
  ಆದರೆ (pin)
    {
      unlink (pin);

      memset (&attr, 0, ಗಾತ್ರ (attr));
      attr.pathname = ptr_ಗಮ್ಯ_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, ಗಾತ್ರ (attr));
      ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ
libcrun_ebpf_read_ಕಾರ್ಯಕ್ರಮ (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ **ಕಾರ್ಯಕ್ರಮ_ret, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_EBPF
  (ಶೂನ್ಯ) ಕಾರ್ಯಕ್ರಮ_ret;
  (ಶೂನ್ಯ) ಮಾರ್ಗ;

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "eBPF not supported");
#ಇಲ್ಲದಿದ್ದರೆ
  cleanup_free ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ prog_fd = -1;
  size_t ಬಫರ್_ಗಾತ್ರ = 0;
  ರಚನೆ bpf_prog_info info;
  ಒಕ್ಕೂಟ bpf_attr attr;
  ಪೂರ್ಣಾಂಕ ret;

  memset (&attr, 0, ಗಾತ್ರ (attr));
  attr.pathname = ptr_ಗಮ್ಯ_u64 (ಮಾರ್ಗ);

  prog_fd = bpf (BPF_OBJ_GET, &attr, ಗಾತ್ರ (attr));
  ಆದರೆ (UNLIKELY (prog_fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bpf get `%s`", ಮಾರ್ಗ);

  memset (&info, 0, ಗಾತ್ರ (info));

  memset (&attr, 0, ಗಾತ್ರ (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_ಗಮ್ಯ_u64 (&info);
  attr.info.info_len = ಗಾತ್ರ (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, ಗಾತ್ರ (attr));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bpf get info `%s`", ಮಾರ್ಗ);

  ಬಫರ್_ಗಾತ್ರ = info.xlated_prog_len;
  ಬಫರ್ = xmalloc (ಬಫರ್_ಗಾತ್ರ);

  memset (&info, 0, ಗಾತ್ರ (info));
  info.xlated_prog_insns = ptr_ಗಮ್ಯ_u64 (ಬಫರ್);
  info.xlated_prog_len = ಬಫರ್_ಗಾತ್ರ;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, ಗಾತ್ರ (attr));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bpf get info `%s`", ಮಾರ್ಗ);

  ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_new (ಬಫರ್_ಗಾತ್ರ);
  ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_append (ಕಾರ್ಯಕ್ರಮ, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ);

  *ಕಾರ್ಯಕ್ರಮ_ret = ಕಾರ್ಯಕ್ರಮ;
  ಕಾರ್ಯಕ್ರಮ = ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ
libcrun_ebpf_query_cgroup_progs (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ, uint32_t **progs_ಹೊರಹೋಗು, size_t *n_progs_ಹೊರಹೋಗು, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_EBPF
  (ಶೂನ್ಯ) cgroup_ಮಾರ್ಗ;
  *progs_ಹೊರಹೋಗು = ಖಾಲಿ;
  *n_progs_ಹೊರಹೋಗು = 0;
  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "eBPF not supported");
#ಇಲ್ಲದಿದ್ದರೆ
  cleanup_close ಪೂರ್ಣಾಂಕ cgroup_fd = -1;

  cgroup_fd = open (cgroup_ಮಾರ್ಗ, O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (cgroup_fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open cgroup path `%s`", cgroup_ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ read_all_progs (cgroup_fd, progs_ಹೊರಹೋಗು, n_progs_ಹೊರಹೋಗು, err);
#ಆದರೆ_ಅಂತ್ಯ
}

bool
libcrun_ebpf_cmp_programs (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *program1, ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *program2)
{
  ಆದರೆ (program1->ಬಳಸಿದ != program2->ಬಳಸಿದ)
    ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ memcmp (program1->ಕಾರ್ಯಕ್ರಮ, program2->ಕಾರ್ಯಕ್ರಮ, program1->ಬಳಸಿದ) == 0;
}
