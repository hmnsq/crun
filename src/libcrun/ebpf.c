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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "ebpf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/resource.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
ਸਥਿਰ ਪੂਰਨ
syscall_bpf (ਪੂਰਨ cmd, ਸੰਘ bpf_attr *attr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
{
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_bpf, cmd, attr, ਮਾਪ);
}
#    define bpf syscall_bpf
#  endif

#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ uint64_t
ptr_ਮੰਜ਼ਿਲ_u64 (ਅਚਲ ਖਾਲੀ *ptr)
{
  ਵਾਪਸ (uint64_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) ptr;
}

ਗਿਣਤੀ
{
  HAS_WILDCARD = 1
};

ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ
{
  size_t allocated;
  size_t ਵਰਤਿਆ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਜੀ;
  ਅੱਖਰ ਪ੍ਰੋਗਰਾਮ[];
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((ਬਣਤਰ bpf_insn) { .ਕੋਡ = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(ਮਾਪ_2, DST, SRC, OFF) \
    ((ਬਣਤਰ bpf_insn) {                   \
        .ਕੋਡ = BPF_LDX | BPF_ਮਾਪ (ਮਾਪ_2) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((ਬਣਤਰ bpf_insn) { .ਕੋਡ = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((ਬਣਤਰ bpf_insn) { .ਕੋਡ = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((ਬਣਤਰ bpf_insn) { .ਕੋਡ = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((ਬਣਤਰ bpf_insn) { .ਕੋਡ = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((ਬਣਤਰ bpf_insn) { .ਕੋਡ = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((ਬਣਤਰ bpf_insn) { .ਕੋਡ = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((ਬਣਤਰ bpf_insn) { .ਕੋਡ = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
ਸਥਿਰ size_t
bpf_ਪ੍ਰੋਗਰਾਮ_instructions (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ)
{
  ਵਾਪਸ ਪ੍ਰੋਗਰਾਮ->ਵਰਤਿਆ / (ਆਕਾਰ (ਬਣਤਰ bpf_insn));
}
#ਜੇ_ਅੰਤ

ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *
bpf_ਪ੍ਰੋਗਰਾਮ_new (size_t ਮਾਪ)
{
  ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *p = xmalloc (ਮਾਪ + ਆਕਾਰ (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ));

  p->ਵਰਤਿਆ = 0;
  p->ਨਿਜੀ = 0;
  p->allocated = ਮਾਪ;

  ਵਾਪਸ p;
}

ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *
bpf_ਪ੍ਰੋਗਰਾਮ_append (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *p, ਖਾਲੀ *ਅੰਕੜੇ, size_t ਮਾਪ)
{
  ਜੇ (p->allocated <= p->ਵਰਤਿਆ + ਮਾਪ)
    {
      p->allocated += ਮਾਪ * 2;
      p = xrealloc (p, p->allocated + ਆਕਾਰ (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ));
    }
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (p->ਪ੍ਰੋਗਰਾਮ + p->ਵਰਤਿਆ, ਅੰਕੜੇ, ਮਾਪ);
  p->ਵਰਤਿਆ += ਮਾਪ;
  ਵਾਪਸ p;
}

ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *
bpf_ਪ੍ਰੋਗਰਾਮ_init_dev (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ, libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
  /* taken from systemd.  */
  ਬਣਤਰ bpf_insn pre_insn[] = {
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
  ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_append (ਪ੍ਰੋਗਰਾਮ, pre_insn, ਆਕਾਰ (pre_insn));
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਪ੍ਰੋਗਰਾਮ;
}

ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *
bpf_ਪ੍ਰੋਗਰਾਮ_append_dev (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ, ਅਚਲ ਅੱਖਰ *access, ਅੱਖਰ type, ਪੂਰਨ major, ਪੂਰਨ minor, bool accept,
                        libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
  ਪੂਰਨ i;
  ਪੂਰਨ bpf_access = 0;
  ਪੂਰਨ bpf_type = type == 'b' ? BPF_DEVCG_DEV_ਖੰਡ : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  ਪੂਰਨ ਸੰਖਿਆ_instructions = 0;
  ਬਣਤਰ bpf_insn accept_ਖੰਡ[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  ਜੇ (access == ਨਿਰਾ)
    access = "";

  ਜੇ (ਪ੍ਰੋਗਰਾਮ->ਨਿਜੀ & HAS_WILDCARD)
    ਵਾਪਸ ਪ੍ਰੋਗਰਾਮ;

  ਲਈ (i = 0; access[i]; i++)
    {
      ਚੋਣ (access[i])
        {
        ਹਾਲਤ 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          ਰੋਕੋ;

        ਹਾਲਤ 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          ਰੋਕੋ;

        ਹਾਲਤ 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          ਰੋਕੋ;
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
  ਸੰਖਿਆ_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  ਜੇ (has_type)
    {
      ਬਣਤਰ bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, ਸੰਖਿਆ_instructions) };
      ਸੰਖਿਆ_instructions--;
      ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_append (ਪ੍ਰੋਗਰਾਮ, i, ਆਕਾਰ (i));
    }
  ਜੇ (has_access)
    {
      ਬਣਤਰ bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, ਸੰਖਿਆ_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, ਸੰਖਿਆ_instructions - 2),
      };
      ਸੰਖਿਆ_instructions -= 3;
      ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_append (ਪ੍ਰੋਗਰਾਮ, i, ਆਕਾਰ (i));
    }
  ਜੇ (has_major)
    {
      ਬਣਤਰ bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, ਸੰਖਿਆ_instructions) };
      ਸੰਖਿਆ_instructions--;
      ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_append (ਪ੍ਰੋਗਰਾਮ, i, ਆਕਾਰ (i));
    }
  ਜੇ (has_minor)
    {
      ਬਣਤਰ bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, ਸੰਖਿਆ_instructions) };
      ਸੰਖਿਆ_instructions--;
      ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_append (ਪ੍ਰੋਗਰਾਮ, i, ਆਕਾਰ (i));
    }

  ਜੇ (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    ਪ੍ਰੋਗਰਾਮ->ਨਿਜੀ |= HAS_WILDCARD;

  ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_append (ਪ੍ਰੋਗਰਾਮ, accept_ਖੰਡ, ਆਕਾਰ (accept_ਖੰਡ));
#ਜੇ_ਅੰਤ
  (ਖਾਲੀ) access;
  (ਖਾਲੀ) type;
  (ਖਾਲੀ) major;
  (ਖਾਲੀ) minor;
  (ਖਾਲੀ) accept;

  ਵਾਪਸ ਪ੍ਰੋਗਰਾਮ;
}

ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *
bpf_ਪ੍ਰੋਗਰਾਮ_complete_dev (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ, libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
  ਬਣਤਰ bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  ਜੇ (ਪ੍ਰੋਗਰਾਮ->ਨਿਜੀ & HAS_WILDCARD)
    ਵਾਪਸ ਪ੍ਰੋਗਰਾਮ;

  ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_append (ਪ੍ਰੋਗਰਾਮ, &i, ਆਕਾਰ (i));
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਪ੍ਰੋਗਰਾਮ;
}

ਸਥਿਰ ਪੂਰਨ
read_all_progs (ਪੂਰਨ dirfd, uint32_t **progs_ਬਾਹਰਲਾ, size_t *n_progs_ਬਾਹਰਲਾ, libcrun_error_t *err)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
  cleanup_free uint32_t *progs = ਨਿਰਾ;
  ਸੰਘ bpf_attr attr;
  ਪੂਰਨ ret;
  size_t cur_ਮਾਪ;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  ਲਈ (cur_ਮਾਪ = 64; cur_ਮਾਪ <= 4096; cur_ਮਾਪ *= 2)
    {
      progs = xrealloc (progs, ਆਕਾਰ (uint32_t) * cur_ਮਾਪ);

      memset (&attr, 0, ਆਕਾਰ (attr));
      attr.query.ਨਿਸ਼ਾਨਾ_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_ਮਾਪ;
      attr.query.prog_ids = ptr_ਮੰਜ਼ਿਲ_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, ਆਕਾਰ (attr));
    }
  ਜਦੋਂ (ret < 0 && errno == ENOSPC)
    ;

  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "bpf query");

  *progs_ਬਾਹਰਲਾ = progs;
  progs = ਨਿਰਾ;
  *n_progs_ਬਾਹਰਲਾ = attr.query.prog_cnt;
  ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) dirfd;

  *progs_ਬਾਹਰਲਾ = ਨਿਰਾ;
  *n_progs_ਬਾਹਰਲਾ = 0;

  ਵਾਪਸ crun_make_error (err, 0, "eBPF not supported");
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਪੂਰਨ
remove_all_progs (ਪੂਰਨ dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
  ਸੰਘ bpf_attr attr;
  size_t i;

  ਲਈ (i = 0; i < n_progs; i++)
    {
      cleanup_close ਪੂਰਨ fd = -1;
      ਪੂਰਨ ret;

      memset (&attr, 0, ਆਕਾਰ (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, ਆਕਾਰ (attr));
      ਜੇ (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          ਜੇ (errno == ENOENT)
            ਜਾਰੀ;

          ਵਾਪਸ crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, ਆਕਾਰ (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.ਨਿਸ਼ਾਨਾ_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, ਆਕਾਰ (attr));
      ਜੇ (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          ਜੇ (errno == ENOENT)
            ਜਾਰੀ;

          ਵਾਪਸ crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) dirfd;
  (ਖਾਲੀ) progs;
  (ਖਾਲੀ) n_progs;
  ਵਾਪਸ crun_make_error (err, 0, "eBPF not supported");
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਪੂਰਨ
ebpf_attach_ਪ੍ਰੋਗਰਾਮ (ਪੂਰਨ fd, ਪੂਰਨ dirfd, libcrun_error_t *err)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
  (ਖਾਲੀ) fd;
  (ਖਾਲੀ) dirfd;
  (ਖਾਲੀ) read_all_progs;
  (ਖਾਲੀ) remove_all_progs;

  ਵਾਪਸ crun_make_error (err, 0, "eBPF not supported");
#ਨਹੀਂਤਾਂ
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  ਅਚਲ ਪੂਰਨ MAX_ATTEMPTS = 20;
  ਪੂਰਨ attempt;

  ਲਈ (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = ਨਿਰਾ;
      cleanup_close ਪੂਰਨ replacefd = -1;
      ਸੰਘ bpf_attr attr;
      size_t n_progs = 0;
      ਪੂਰਨ ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      ਜੇ (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, ਆਕਾਰ (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, ਆਕਾਰ (attr));
          ਜੇ (UNLIKELY (replacefd < 0))
            {
              ਜੇ (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  ਜਾਰੀ;
                }
              ਵਾਪਸ crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, ਆਕਾਰ (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.ਨਿਸ਼ਾਨਾ_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      ਜੇ (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, ਆਕਾਰ (attr));
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              ਜਾਰੀ;
            }
#  ifdef BPF_F_REPLACE
          ਜੇ (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              ਜਾਰੀ;
            }
#  endif
          ਵਾਪਸ crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      ਜੇ (replacefd < 0 && n_progs)
        ਵਾਪਸ remove_all_progs (dirfd, progs, n_progs, err);

      ਵਾਪਸ 0;
    }
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ
bump_memlock ()
{
  ਬਣਤਰ rlimit ਹੱਦਬੰਦੀ;
  ਪੂਰਨ ret;

  ਹੱਦਬੰਦੀ.rlim_cur = RLIM_INFINITY;
  ਹੱਦਬੰਦੀ.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &ਹੱਦਬੰਦੀ);
  ਜੇ (ret == 0)
    ਵਾਪਸ;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &ਹੱਦਬੰਦੀ);
  ਜੇ (ret < 0)
    ਵਾਪਸ;

  ਹੱਦਬੰਦੀ.rlim_cur = ਹੱਦਬੰਦੀ.rlim_max;
  /* Best effort, ignore errors.  */
  (ਖਾਲੀ) setrlimit (RLIMIT_MEMLOCK, &ਹੱਦਬੰਦੀ);
}

ਪੂਰਨ
libcrun_ebpf_ਲੱਦੋ (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *pin, libcrun_error_t *err)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
  (ਖਾਲੀ) dirfd;
  (ਖਾਲੀ) ਪ੍ਰੋਗਰਾਮ;
  (ਖਾਲੀ) pin;
  (ਖਾਲੀ) ebpf_attach_ਪ੍ਰੋਗਰਾਮ;

  ਵਾਪਸ crun_make_error (err, 0, "eBPF not supported");
#ਨਹੀਂਤਾਂ
  cleanup_close ਪੂਰਨ fd = -1;
  ਸੰਘ bpf_attr attr;
  ਪੂਰਨ ret;

  memset (&attr, 0, ਆਕਾਰ (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_ਮੰਜ਼ਿਲ_u64 (ਪ੍ਰੋਗਰਾਮ->ਪ੍ਰੋਗਰਾਮ);
  attr.insn_cnt = bpf_ਪ੍ਰੋਗਰਾਮ_instructions (ਪ੍ਰੋਗਰਾਮ);
  attr.license = ptr_ਮੰਜ਼ਿਲ_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_ਲੱਦੋ, &attr, ਆਕਾਰ (attr));
  ਜੇ (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_ਲੱਦੋ, &attr, ਆਕਾਰ (attr));
    }
  ਜੇ (fd < 0)
    {
      ਅਚਲ size_t max_log_ਮਾਪ = 1 << 20;
      cleanup_free ਅੱਖਰ *ਲਘੂਗਣਕ = ਨਿਰਾ;
      size_t log_ਮਾਪ = 8192;

    retry:
      ਲਘੂਗਣਕ = xrealloc (ਲਘੂਗਣਕ, log_ਮਾਪ);
      ਲਘੂਗਣਕ[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_ਮੰਜ਼ਿਲ_u64 (ਲਘੂਗਣਕ);
      attr.log_ਮਾਪ = log_ਮਾਪ;

      fd = bpf (BPF_PROG_ਲੱਦੋ, &attr, ਆਕਾਰ (attr));
      ਜੇ (fd < 0)
        {
          ਜੇ (errno == ENOSPC && log_ਮਾਪ < max_log_ਮਾਪ)
            {
              /* The provided buffer was not big enough.  */
              log_ਮਾਪ *= 2;
              ਜਾਓ retry;
            }
          ਵਾਪਸ crun_make_error (err, errno, "bpf create `%s`", ਲਘੂਗਣਕ);
        }
    }

  ਜੇ (dirfd >= 0)
    {
      ret = ebpf_attach_ਪ੍ਰੋਗਰਾਮ (fd, dirfd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  /* Optionally pin the program to the specified path.  */
  ਜੇ (pin)
    {
      unlink (pin);

      memset (&attr, 0, ਆਕਾਰ (attr));
      attr.pathname = ptr_ਮੰਜ਼ਿਲ_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, ਆਕਾਰ (attr));
      ਜੇ (ret < 0)
        ਵਾਪਸ crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ
libcrun_ebpf_read_ਪ੍ਰੋਗਰਾਮ (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ **ਪ੍ਰੋਗਰਾਮ_ret, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
  (ਖਾਲੀ) ਪ੍ਰੋਗਰਾਮ_ret;
  (ਖਾਲੀ) ਰਸਤਾ;

  ਵਾਪਸ crun_make_error (err, 0, "eBPF not supported");
#ਨਹੀਂਤਾਂ
  cleanup_free ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ prog_fd = -1;
  size_t ਬਫਰ_ਮਾਪ = 0;
  ਬਣਤਰ bpf_prog_info info;
  ਸੰਘ bpf_attr attr;
  ਪੂਰਨ ret;

  memset (&attr, 0, ਆਕਾਰ (attr));
  attr.pathname = ptr_ਮੰਜ਼ਿਲ_u64 (ਰਸਤਾ);

  prog_fd = bpf (BPF_OBJ_GET, &attr, ਆਕਾਰ (attr));
  ਜੇ (UNLIKELY (prog_fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "bpf get `%s`", ਰਸਤਾ);

  memset (&info, 0, ਆਕਾਰ (info));

  memset (&attr, 0, ਆਕਾਰ (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_ਮੰਜ਼ਿਲ_u64 (&info);
  attr.info.info_len = ਆਕਾਰ (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, ਆਕਾਰ (attr));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "bpf get info `%s`", ਰਸਤਾ);

  ਬਫਰ_ਮਾਪ = info.xlated_prog_len;
  ਬਫਰ = xmalloc (ਬਫਰ_ਮਾਪ);

  memset (&info, 0, ਆਕਾਰ (info));
  info.xlated_prog_insns = ptr_ਮੰਜ਼ਿਲ_u64 (ਬਫਰ);
  info.xlated_prog_len = ਬਫਰ_ਮਾਪ;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, ਆਕਾਰ (attr));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "bpf get info `%s`", ਰਸਤਾ);

  ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_new (ਬਫਰ_ਮਾਪ);
  ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_append (ਪ੍ਰੋਗਰਾਮ, ਬਫਰ, ਬਫਰ_ਮਾਪ);

  *ਪ੍ਰੋਗਰਾਮ_ret = ਪ੍ਰੋਗਰਾਮ;
  ਪ੍ਰੋਗਰਾਮ = ਨਿਰਾ;

  ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ
libcrun_ebpf_query_cgroup_progs (ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ, uint32_t **progs_ਬਾਹਰਲਾ, size_t *n_progs_ਬਾਹਰਲਾ, libcrun_error_t *err)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
  (ਖਾਲੀ) cgroup_ਰਸਤਾ;
  *progs_ਬਾਹਰਲਾ = ਨਿਰਾ;
  *n_progs_ਬਾਹਰਲਾ = 0;
  ਵਾਪਸ crun_make_error (err, 0, "eBPF not supported");
#ਨਹੀਂਤਾਂ
  cleanup_close ਪੂਰਨ cgroup_fd = -1;

  cgroup_fd = open (cgroup_ਰਸਤਾ, O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (cgroup_fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open cgroup path `%s`", cgroup_ਰਸਤਾ);

  ਵਾਪਸ read_all_progs (cgroup_fd, progs_ਬਾਹਰਲਾ, n_progs_ਬਾਹਰਲਾ, err);
#ਜੇ_ਅੰਤ
}

bool
libcrun_ebpf_cmp_programs (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *program1, ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *program2)
{
  ਜੇ (program1->ਵਰਤਿਆ != program2->ਵਰਤਿਆ)
    ਵਾਪਸ false;

  ਵਾਪਸ memcmp (program1->ਪ੍ਰੋਗਰਾਮ, program2->ਪ੍ਰੋਗਰਾਮ, program1->ਵਰਤਿਆ) == 0;
}
