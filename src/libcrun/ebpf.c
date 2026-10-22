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

#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل "ebpf.h"
#شامل "utils.h"
#شامل <unistd.h>
#شامل <sys/syscall.h>
#شامل <sys/resource.h>

#اگر_تعریف_شدہ HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
ساکن صحیح
syscall_bpf (صحیح cmd, اتحاد bpf_attr *attr, غیرعلامتی صحیح سائز)
{
  واپس (صحیح) syscall (__NR_bpf, cmd, attr, سائز);
}
#    define bpf syscall_bpf
#  endif

#اگر_ختم

ساکن اندرونی uint64_t
ptr_مقصد_u64 (ثابت خالی *ptr)
{
  واپس (uint64_t) (غیرعلامتی طویل) ptr;
}

شمار
{
  HAS_WILDCARD = 1
};

ساخت bpf_پروگرام
{
  size_t allocated;
  size_t مستعمل;
  غیرعلامتی صحیح نجی;
  حرف پروگرام[];
};

#اگر_تعریف_شدہ HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((ساخت bpf_insn) { .رمز = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(سائز_2, DST, SRC, OFF) \
    ((ساخت bpf_insn) {                   \
        .رمز = BPF_LDX | BPF_سائز (سائز_2) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((ساخت bpf_insn) { .رمز = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((ساخت bpf_insn) { .رمز = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((ساخت bpf_insn) { .رمز = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((ساخت bpf_insn) { .رمز = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((ساخت bpf_insn) { .رمز = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((ساخت bpf_insn) { .رمز = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((ساخت bpf_insn) { .رمز = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#اگر_ختم

#اگر_تعریف_شدہ HAVE_EBPF
ساکن size_t
bpf_پروگرام_instructions (ساخت bpf_پروگرام *پروگرام)
{
  واپس پروگرام->مستعمل / (حجم (ساخت bpf_insn));
}
#اگر_ختم

ساخت bpf_پروگرام *
bpf_پروگرام_new (size_t سائز)
{
  ساخت bpf_پروگرام *p = xmalloc (سائز + حجم (ساخت bpf_پروگرام));

  p->مستعمل = 0;
  p->نجی = 0;
  p->allocated = سائز;

  واپس p;
}

ساخت bpf_پروگرام *
bpf_پروگرام_append (ساخت bpf_پروگرام *p, خالی *ڈیٹا, size_t سائز)
{
  اگر (p->allocated <= p->مستعمل + سائز)
    {
      p->allocated += سائز * 2;
      p = xrealloc (p, p->allocated + حجم (ساخت bpf_پروگرام));
    }
  حافظہ_نقل (p->پروگرام + p->مستعمل, ڈیٹا, سائز);
  p->مستعمل += سائز;
  واپس p;
}

ساخت bpf_پروگرام *
bpf_پروگرام_init_dev (ساخت bpf_پروگرام *پروگرام, libcrun_error_t *err arg_غیر_مستعمل)
{
#اگر_تعریف_شدہ HAVE_EBPF
  /* taken from systemd.  */
  ساخت bpf_insn pre_insn[] = {
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
  پروگرام = bpf_پروگرام_append (پروگرام, pre_insn, حجم (pre_insn));
#اگر_ختم
  واپس پروگرام;
}

ساخت bpf_پروگرام *
bpf_پروگرام_append_dev (ساخت bpf_پروگرام *پروگرام, ثابت حرف *access, حرف type, صحیح major, صحیح minor, bool accept,
                        libcrun_error_t *err arg_غیر_مستعمل)
{
#اگر_تعریف_شدہ HAVE_EBPF
  صحیح i;
  صحیح bpf_access = 0;
  صحیح bpf_type = type == 'b' ? BPF_DEVCG_DEV_بلاک : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  صحیح نمبر_instructions = 0;
  ساخت bpf_insn accept_بلاک[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  اگر (access == تہی)
    access = "";

  اگر (پروگرام->نجی & HAS_WILDCARD)
    واپس پروگرام;

  برائے (i = 0; access[i]; i++)
    {
      انتخاب (access[i])
        {
        حالت 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          توڑو;

        حالت 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          توڑو;

        حالت 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          توڑو;
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
  نمبر_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  اگر (has_type)
    {
      ساخت bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, نمبر_instructions) };
      نمبر_instructions--;
      پروگرام = bpf_پروگرام_append (پروگرام, i, حجم (i));
    }
  اگر (has_access)
    {
      ساخت bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, نمبر_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, نمبر_instructions - 2),
      };
      نمبر_instructions -= 3;
      پروگرام = bpf_پروگرام_append (پروگرام, i, حجم (i));
    }
  اگر (has_major)
    {
      ساخت bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, نمبر_instructions) };
      نمبر_instructions--;
      پروگرام = bpf_پروگرام_append (پروگرام, i, حجم (i));
    }
  اگر (has_minor)
    {
      ساخت bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, نمبر_instructions) };
      نمبر_instructions--;
      پروگرام = bpf_پروگرام_append (پروگرام, i, حجم (i));
    }

  اگر (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    پروگرام->نجی |= HAS_WILDCARD;

  پروگرام = bpf_پروگرام_append (پروگرام, accept_بلاک, حجم (accept_بلاک));
#اگر_ختم
  (خالی) access;
  (خالی) type;
  (خالی) major;
  (خالی) minor;
  (خالی) accept;

  واپس پروگرام;
}

ساخت bpf_پروگرام *
bpf_پروگرام_complete_dev (ساخت bpf_پروگرام *پروگرام, libcrun_error_t *err arg_غیر_مستعمل)
{
#اگر_تعریف_شدہ HAVE_EBPF
  ساخت bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  اگر (پروگرام->نجی & HAS_WILDCARD)
    واپس پروگرام;

  پروگرام = bpf_پروگرام_append (پروگرام, &i, حجم (i));
#اگر_ختم
  واپس پروگرام;
}

ساکن صحیح
read_all_progs (صحیح dirfd, uint32_t **progs_برآمد, size_t *n_progs_برآمد, libcrun_error_t *err)
{
#اگر_تعریف_شدہ HAVE_EBPF
  cleanup_free uint32_t *progs = تہی;
  اتحاد bpf_attr attr;
  صحیح ret;
  size_t cur_سائز;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  برائے (cur_سائز = 64; cur_سائز <= 4096; cur_سائز *= 2)
    {
      progs = xrealloc (progs, حجم (uint32_t) * cur_سائز);

      memset (&attr, 0, حجم (attr));
      attr.query.ہدف_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_سائز;
      attr.query.prog_ids = ptr_مقصد_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, حجم (attr));
    }
  جب (ret < 0 && errno == ENOSPC)
    ;

  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "bpf query");

  *progs_برآمد = progs;
  progs = تہی;
  *n_progs_برآمد = attr.query.prog_cnt;
  واپس 0;
#ورنہ
  (خالی) dirfd;

  *progs_برآمد = تہی;
  *n_progs_برآمد = 0;

  واپس crun_make_error (err, 0, "eBPF not supported");
#اگر_ختم
}

ساکن صحیح
remove_all_progs (صحیح dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#اگر_تعریف_شدہ HAVE_EBPF
  اتحاد bpf_attr attr;
  size_t i;

  برائے (i = 0; i < n_progs; i++)
    {
      cleanup_close صحیح fd = -1;
      صحیح ret;

      memset (&attr, 0, حجم (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, حجم (attr));
      اگر (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          اگر (errno == ENOENT)
            جاری;

          واپس crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, حجم (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.ہدف_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, حجم (attr));
      اگر (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          اگر (errno == ENOENT)
            جاری;

          واپس crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  واپس 0;
#ورنہ
  (خالی) dirfd;
  (خالی) progs;
  (خالی) n_progs;
  واپس crun_make_error (err, 0, "eBPF not supported");
#اگر_ختم
}

ساکن صحیح
ebpf_attach_پروگرام (صحیح fd, صحیح dirfd, libcrun_error_t *err)
{
#اگر_غیر_تعریف_شدہ HAVE_EBPF
  (خالی) fd;
  (خالی) dirfd;
  (خالی) read_all_progs;
  (خالی) remove_all_progs;

  واپس crun_make_error (err, 0, "eBPF not supported");
#ورنہ
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  ثابت صحیح MAX_ATTEMPTS = 20;
  صحیح attempt;

  برائے (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = تہی;
      cleanup_close صحیح replacefd = -1;
      اتحاد bpf_attr attr;
      size_t n_progs = 0;
      صحیح ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      اگر (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, حجم (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, حجم (attr));
          اگر (UNLIKELY (replacefd < 0))
            {
              اگر (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  جاری;
                }
              واپس crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, حجم (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.ہدف_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      اگر (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, حجم (attr));
      اگر (UNLIKELY (ret < 0))
        {
          اگر (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              جاری;
            }
#  ifdef BPF_F_REPLACE
          اگر (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              جاری;
            }
#  endif
          واپس crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      اگر (replacefd < 0 && n_progs)
        واپس remove_all_progs (dirfd, progs, n_progs, err);

      واپس 0;
    }
#اگر_ختم
}

ساکن خالی
bump_memlock ()
{
  ساخت rlimit حد_بندی;
  صحیح ret;

  حد_بندی.rlim_cur = RLIM_INFINITY;
  حد_بندی.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &حد_بندی);
  اگر (ret == 0)
    واپس;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &حد_بندی);
  اگر (ret < 0)
    واپس;

  حد_بندی.rlim_cur = حد_بندی.rlim_max;
  /* Best effort, ignore errors.  */
  (خالی) setrlimit (RLIMIT_MEMLOCK, &حد_بندی);
}

صحیح
libcrun_ebpf_بھرو (ساخت bpf_پروگرام *پروگرام, صحیح dirfd, ثابت حرف *pin, libcrun_error_t *err)
{
#اگر_غیر_تعریف_شدہ HAVE_EBPF
  (خالی) dirfd;
  (خالی) پروگرام;
  (خالی) pin;
  (خالی) ebpf_attach_پروگرام;

  واپس crun_make_error (err, 0, "eBPF not supported");
#ورنہ
  cleanup_close صحیح fd = -1;
  اتحاد bpf_attr attr;
  صحیح ret;

  memset (&attr, 0, حجم (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_مقصد_u64 (پروگرام->پروگرام);
  attr.insn_cnt = bpf_پروگرام_instructions (پروگرام);
  attr.license = ptr_مقصد_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_بھرو, &attr, حجم (attr));
  اگر (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_بھرو, &attr, حجم (attr));
    }
  اگر (fd < 0)
    {
      ثابت size_t max_log_سائز = 1 << 20;
      cleanup_free حرف *لوگارتھم = تہی;
      size_t log_سائز = 8192;

    retry:
      لوگارتھم = xrealloc (لوگارتھم, log_سائز);
      لوگارتھم[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_مقصد_u64 (لوگارتھم);
      attr.log_سائز = log_سائز;

      fd = bpf (BPF_PROG_بھرو, &attr, حجم (attr));
      اگر (fd < 0)
        {
          اگر (errno == ENOSPC && log_سائز < max_log_سائز)
            {
              /* The provided buffer was not big enough.  */
              log_سائز *= 2;
              جاؤ retry;
            }
          واپس crun_make_error (err, errno, "bpf create `%s`", لوگارتھم);
        }
    }

  اگر (dirfd >= 0)
    {
      ret = ebpf_attach_پروگرام (fd, dirfd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  /* Optionally pin the program to the specified path.  */
  اگر (pin)
    {
      unlink (pin);

      memset (&attr, 0, حجم (attr));
      attr.pathname = ptr_مقصد_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, حجم (attr));
      اگر (ret < 0)
        واپس crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  واپس 0;
#اگر_ختم
}

صحیح
libcrun_ebpf_read_پروگرام (ساخت bpf_پروگرام **پروگرام_ret, ثابت حرف *راستہ, libcrun_error_t *err)
{
#اگر_غیر_تعریف_شدہ HAVE_EBPF
  (خالی) پروگرام_ret;
  (خالی) راستہ;

  واپس crun_make_error (err, 0, "eBPF not supported");
#ورنہ
  cleanup_free ساخت bpf_پروگرام *پروگرام = تہی;
  cleanup_free حرف *بفر = تہی;
  cleanup_close صحیح prog_fd = -1;
  size_t بفر_سائز = 0;
  ساخت bpf_prog_info info;
  اتحاد bpf_attr attr;
  صحیح ret;

  memset (&attr, 0, حجم (attr));
  attr.pathname = ptr_مقصد_u64 (راستہ);

  prog_fd = bpf (BPF_OBJ_GET, &attr, حجم (attr));
  اگر (UNLIKELY (prog_fd < 0))
    واپس crun_make_error (err, errno, "bpf get `%s`", راستہ);

  memset (&info, 0, حجم (info));

  memset (&attr, 0, حجم (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_مقصد_u64 (&info);
  attr.info.info_len = حجم (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, حجم (attr));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "bpf get info `%s`", راستہ);

  بفر_سائز = info.xlated_prog_len;
  بفر = xmalloc (بفر_سائز);

  memset (&info, 0, حجم (info));
  info.xlated_prog_insns = ptr_مقصد_u64 (بفر);
  info.xlated_prog_len = بفر_سائز;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, حجم (attr));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "bpf get info `%s`", راستہ);

  پروگرام = bpf_پروگرام_new (بفر_سائز);
  پروگرام = bpf_پروگرام_append (پروگرام, بفر, بفر_سائز);

  *پروگرام_ret = پروگرام;
  پروگرام = تہی;

  واپس 0;
#اگر_ختم
}

صحیح
libcrun_ebpf_query_cgroup_progs (ثابت حرف *cgroup_راستہ, uint32_t **progs_برآمد, size_t *n_progs_برآمد, libcrun_error_t *err)
{
#اگر_غیر_تعریف_شدہ HAVE_EBPF
  (خالی) cgroup_راستہ;
  *progs_برآمد = تہی;
  *n_progs_برآمد = 0;
  واپس crun_make_error (err, 0, "eBPF not supported");
#ورنہ
  cleanup_close صحیح cgroup_fd = -1;

  cgroup_fd = open (cgroup_راستہ, O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (cgroup_fd < 0))
    واپس crun_make_error (err, errno, "open cgroup path `%s`", cgroup_راستہ);

  واپس read_all_progs (cgroup_fd, progs_برآمد, n_progs_برآمد, err);
#اگر_ختم
}

bool
libcrun_ebpf_cmp_programs (ساخت bpf_پروگرام *program1, ساخت bpf_پروگرام *program2)
{
  اگر (program1->مستعمل != program2->مستعمل)
    واپس false;

  واپس memcmp (program1->پروگرام, program2->پروگرام, program1->مستعمل) == 0;
}
