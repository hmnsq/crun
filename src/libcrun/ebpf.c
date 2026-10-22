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

#تعریف_کرو GNU_سروت

#ضمن <config.h>
#ضمن "ebpf.h"
#ضمن "utils.h"
#ضمن <unistd.h>
#ضمن <sys/syscall.h>
#ضمن <sys/resource.h>

#جے_تعریف_شدہ HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
ساكن صحيح
syscall_bpf (صحيح cmd, اتحاد bpf_attr *attr, بنانشانی صحيح ناپ)
{
  أرجع (صحيح) syscall (__NR_bpf, cmd, attr, ناپ);
}
#    define bpf syscall_bpf
#  endif

#جے_انت

ساكن مضمن uint64_t
ptr_منزل_u64 (ثابت فراغ *ptr)
{
  أرجع (uint64_t) (بنانشانی لما) ptr;
}

تعداد
{
  HAS_WILDCARD = 1
};

بنية bpf_پروگرام
{
  size_t allocated;
  size_t ورتیا;
  بنانشانی صحيح خاص;
  حرف پروگرام[];
};

#جے_تعریف_شدہ HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((بنية bpf_insn) { .کوڈ = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(ناپ_2, DST, SRC, OFF) \
    ((بنية bpf_insn) {                   \
        .کوڈ = BPF_LDX | BPF_ناپ (ناپ_2) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((بنية bpf_insn) { .کوڈ = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((بنية bpf_insn) { .کوڈ = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((بنية bpf_insn) { .کوڈ = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((بنية bpf_insn) { .کوڈ = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((بنية bpf_insn) { .کوڈ = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((بنية bpf_insn) { .کوڈ = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((بنية bpf_insn) { .کوڈ = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#جے_انت

#جے_تعریف_شدہ HAVE_EBPF
ساكن size_t
bpf_پروگرام_instructions (بنية bpf_پروگرام *پروگرام)
{
  أرجع پروگرام->ورتیا / (حجم (بنية bpf_insn));
}
#جے_انت

بنية bpf_پروگرام *
bpf_پروگرام_new (size_t ناپ)
{
  بنية bpf_پروگرام *p = xmalloc (ناپ + حجم (بنية bpf_پروگرام));

  p->ورتیا = 0;
  p->خاص = 0;
  p->allocated = ناپ;

  أرجع p;
}

بنية bpf_پروگرام *
bpf_پروگرام_append (بنية bpf_پروگرام *p, فراغ *انکڑے, size_t ناپ)
{
  إذا (p->allocated <= p->ورتیا + ناپ)
    {
      p->allocated += ناپ * 2;
      p = xrealloc (p, p->allocated + حجم (بنية bpf_پروگرام));
    }
  حافظہ_نقل (p->پروگرام + p->ورتیا, انکڑے, ناپ);
  p->ورتیا += ناپ;
  أرجع p;
}

بنية bpf_پروگرام *
bpf_پروگرام_init_dev (بنية bpf_پروگرام *پروگرام, libcrun_error_t *err arg_بن_ورتیا)
{
#جے_تعریف_شدہ HAVE_EBPF
  /* taken from systemd.  */
  بنية bpf_insn pre_insn[] = {
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
#جے_انت
  أرجع پروگرام;
}

بنية bpf_پروگرام *
bpf_پروگرام_append_dev (بنية bpf_پروگرام *پروگرام, ثابت حرف *access, حرف type, صحيح major, صحيح minor, bool accept,
                        libcrun_error_t *err arg_بن_ورتیا)
{
#جے_تعریف_شدہ HAVE_EBPF
  صحيح i;
  صحيح bpf_access = 0;
  صحيح bpf_type = type == 'b' ? BPF_DEVCG_DEV_بلاک : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  صحيح نمبر_instructions = 0;
  بنية bpf_insn accept_بلاک[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  إذا (access == عدم)
    access = "";

  إذا (پروگرام->خاص & HAS_WILDCARD)
    أرجع پروگرام;

  لكل (i = 0; access[i]; i++)
    {
      بدل (access[i])
        {
        حالة 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          اكسر;

        حالة 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          اكسر;

        حالة 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          اكسر;
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

  إذا (has_type)
    {
      بنية bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, نمبر_instructions) };
      نمبر_instructions--;
      پروگرام = bpf_پروگرام_append (پروگرام, i, حجم (i));
    }
  إذا (has_access)
    {
      بنية bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, نمبر_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, نمبر_instructions - 2),
      };
      نمبر_instructions -= 3;
      پروگرام = bpf_پروگرام_append (پروگرام, i, حجم (i));
    }
  إذا (has_major)
    {
      بنية bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, نمبر_instructions) };
      نمبر_instructions--;
      پروگرام = bpf_پروگرام_append (پروگرام, i, حجم (i));
    }
  إذا (has_minor)
    {
      بنية bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, نمبر_instructions) };
      نمبر_instructions--;
      پروگرام = bpf_پروگرام_append (پروگرام, i, حجم (i));
    }

  إذا (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    پروگرام->خاص |= HAS_WILDCARD;

  پروگرام = bpf_پروگرام_append (پروگرام, accept_بلاک, حجم (accept_بلاک));
#جے_انت
  (فراغ) access;
  (فراغ) type;
  (فراغ) major;
  (فراغ) minor;
  (فراغ) accept;

  أرجع پروگرام;
}

بنية bpf_پروگرام *
bpf_پروگرام_complete_dev (بنية bpf_پروگرام *پروگرام, libcrun_error_t *err arg_بن_ورتیا)
{
#جے_تعریف_شدہ HAVE_EBPF
  بنية bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  إذا (پروگرام->خاص & HAS_WILDCARD)
    أرجع پروگرام;

  پروگرام = bpf_پروگرام_append (پروگرام, &i, حجم (i));
#جے_انت
  أرجع پروگرام;
}

ساكن صحيح
read_all_progs (صحيح dirfd, uint32_t **progs_باہرلا, size_t *n_progs_باہرلا, libcrun_error_t *err)
{
#جے_تعریف_شدہ HAVE_EBPF
  cleanup_free uint32_t *progs = عدم;
  اتحاد bpf_attr attr;
  صحيح ret;
  size_t cur_ناپ;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  لكل (cur_ناپ = 64; cur_ناپ <= 4096; cur_ناپ *= 2)
    {
      progs = xrealloc (progs, حجم (uint32_t) * cur_ناپ);

      memset (&attr, 0, حجم (attr));
      attr.query.نشانہ_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_ناپ;
      attr.query.prog_ids = ptr_منزل_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, حجم (attr));
    }
  طالما (ret < 0 && errno == ENOSPC)
    ;

  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "bpf query");

  *progs_باہرلا = progs;
  progs = عدم;
  *n_progs_باہرلا = attr.query.prog_cnt;
  أرجع 0;
#وإلا
  (فراغ) dirfd;

  *progs_باہرلا = عدم;
  *n_progs_باہرلا = 0;

  أرجع crun_make_error (err, 0, "eBPF not supported");
#جے_انت
}

ساكن صحيح
remove_all_progs (صحيح dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#جے_تعریف_شدہ HAVE_EBPF
  اتحاد bpf_attr attr;
  size_t i;

  لكل (i = 0; i < n_progs; i++)
    {
      cleanup_close صحيح fd = -1;
      صحيح ret;

      memset (&attr, 0, حجم (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, حجم (attr));
      إذا (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          إذا (errno == ENOENT)
            استمر;

          أرجع crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, حجم (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.نشانہ_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, حجم (attr));
      إذا (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          إذا (errno == ENOENT)
            استمر;

          أرجع crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  أرجع 0;
#وإلا
  (فراغ) dirfd;
  (فراغ) progs;
  (فراغ) n_progs;
  أرجع crun_make_error (err, 0, "eBPF not supported");
#جے_انت
}

ساكن صحيح
ebpf_attach_پروگرام (صحيح fd, صحيح dirfd, libcrun_error_t *err)
{
#جے_بنا_تعریف HAVE_EBPF
  (فراغ) fd;
  (فراغ) dirfd;
  (فراغ) read_all_progs;
  (فراغ) remove_all_progs;

  أرجع crun_make_error (err, 0, "eBPF not supported");
#وإلا
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  ثابت صحيح MAX_ATTEMPTS = 20;
  صحيح attempt;

  لكل (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = عدم;
      cleanup_close صحيح replacefd = -1;
      اتحاد bpf_attr attr;
      size_t n_progs = 0;
      صحيح ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      إذا (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, حجم (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, حجم (attr));
          إذا (UNLIKELY (replacefd < 0))
            {
              إذا (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  استمر;
                }
              أرجع crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, حجم (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.نشانہ_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      إذا (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, حجم (attr));
      إذا (UNLIKELY (ret < 0))
        {
          إذا (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              استمر;
            }
#  ifdef BPF_F_REPLACE
          إذا (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              استمر;
            }
#  endif
          أرجع crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      إذا (replacefd < 0 && n_progs)
        أرجع remove_all_progs (dirfd, progs, n_progs, err);

      أرجع 0;
    }
#جے_انت
}

ساكن فراغ
bump_memlock ()
{
  بنية rlimit حد_بندی;
  صحيح ret;

  حد_بندی.rlim_cur = RLIM_INFINITY;
  حد_بندی.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &حد_بندی);
  إذا (ret == 0)
    أرجع;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &حد_بندی);
  إذا (ret < 0)
    أرجع;

  حد_بندی.rlim_cur = حد_بندی.rlim_max;
  /* Best effort, ignore errors.  */
  (فراغ) setrlimit (RLIMIT_MEMLOCK, &حد_بندی);
}

صحيح
libcrun_ebpf_لدو (بنية bpf_پروگرام *پروگرام, صحيح dirfd, ثابت حرف *pin, libcrun_error_t *err)
{
#جے_بنا_تعریف HAVE_EBPF
  (فراغ) dirfd;
  (فراغ) پروگرام;
  (فراغ) pin;
  (فراغ) ebpf_attach_پروگرام;

  أرجع crun_make_error (err, 0, "eBPF not supported");
#وإلا
  cleanup_close صحيح fd = -1;
  اتحاد bpf_attr attr;
  صحيح ret;

  memset (&attr, 0, حجم (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_منزل_u64 (پروگرام->پروگرام);
  attr.insn_cnt = bpf_پروگرام_instructions (پروگرام);
  attr.license = ptr_منزل_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_لدو, &attr, حجم (attr));
  إذا (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_لدو, &attr, حجم (attr));
    }
  إذا (fd < 0)
    {
      ثابت size_t max_log_ناپ = 1 << 20;
      cleanup_free حرف *لوگارتھم = عدم;
      size_t log_ناپ = 8192;

    retry:
      لوگارتھم = xrealloc (لوگارتھم, log_ناپ);
      لوگارتھم[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_منزل_u64 (لوگارتھم);
      attr.log_ناپ = log_ناپ;

      fd = bpf (BPF_PROG_لدو, &attr, حجم (attr));
      إذا (fd < 0)
        {
          إذا (errno == ENOSPC && log_ناپ < max_log_ناپ)
            {
              /* The provided buffer was not big enough.  */
              log_ناپ *= 2;
              اذهب retry;
            }
          أرجع crun_make_error (err, errno, "bpf create `%s`", لوگارتھم);
        }
    }

  إذا (dirfd >= 0)
    {
      ret = ebpf_attach_پروگرام (fd, dirfd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  /* Optionally pin the program to the specified path.  */
  إذا (pin)
    {
      unlink (pin);

      memset (&attr, 0, حجم (attr));
      attr.pathname = ptr_منزل_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, حجم (attr));
      إذا (ret < 0)
        أرجع crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  أرجع 0;
#جے_انت
}

صحيح
libcrun_ebpf_read_پروگرام (بنية bpf_پروگرام **پروگرام_ret, ثابت حرف *رستہ, libcrun_error_t *err)
{
#جے_بنا_تعریف HAVE_EBPF
  (فراغ) پروگرام_ret;
  (فراغ) رستہ;

  أرجع crun_make_error (err, 0, "eBPF not supported");
#وإلا
  cleanup_free بنية bpf_پروگرام *پروگرام = عدم;
  cleanup_free حرف *بفر = عدم;
  cleanup_close صحيح prog_fd = -1;
  size_t بفر_ناپ = 0;
  بنية bpf_prog_info info;
  اتحاد bpf_attr attr;
  صحيح ret;

  memset (&attr, 0, حجم (attr));
  attr.pathname = ptr_منزل_u64 (رستہ);

  prog_fd = bpf (BPF_OBJ_GET, &attr, حجم (attr));
  إذا (UNLIKELY (prog_fd < 0))
    أرجع crun_make_error (err, errno, "bpf get `%s`", رستہ);

  memset (&info, 0, حجم (info));

  memset (&attr, 0, حجم (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_منزل_u64 (&info);
  attr.info.info_len = حجم (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, حجم (attr));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "bpf get info `%s`", رستہ);

  بفر_ناپ = info.xlated_prog_len;
  بفر = xmalloc (بفر_ناپ);

  memset (&info, 0, حجم (info));
  info.xlated_prog_insns = ptr_منزل_u64 (بفر);
  info.xlated_prog_len = بفر_ناپ;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, حجم (attr));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "bpf get info `%s`", رستہ);

  پروگرام = bpf_پروگرام_new (بفر_ناپ);
  پروگرام = bpf_پروگرام_append (پروگرام, بفر, بفر_ناپ);

  *پروگرام_ret = پروگرام;
  پروگرام = عدم;

  أرجع 0;
#جے_انت
}

صحيح
libcrun_ebpf_query_cgroup_progs (ثابت حرف *cgroup_رستہ, uint32_t **progs_باہرلا, size_t *n_progs_باہرلا, libcrun_error_t *err)
{
#جے_بنا_تعریف HAVE_EBPF
  (فراغ) cgroup_رستہ;
  *progs_باہرلا = عدم;
  *n_progs_باہرلا = 0;
  أرجع crun_make_error (err, 0, "eBPF not supported");
#وإلا
  cleanup_close صحيح cgroup_fd = -1;

  cgroup_fd = open (cgroup_رستہ, O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (cgroup_fd < 0))
    أرجع crun_make_error (err, errno, "open cgroup path `%s`", cgroup_رستہ);

  أرجع read_all_progs (cgroup_fd, progs_باہرلا, n_progs_باہرلا, err);
#جے_انت
}

bool
libcrun_ebpf_cmp_programs (بنية bpf_پروگرام *program1, بنية bpf_پروگرام *program2)
{
  إذا (program1->ورتیا != program2->ورتیا)
    أرجع false;

  أرجع memcmp (program1->پروگرام, program2->پروگرام, program1->ورتیا) == 0;
}
