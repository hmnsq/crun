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

#تعریف_کړه GNU_سرچینه

#ضمن <config.h>
#ضمن "ebpf.h"
#ضمن "utils.h"
#ضمن <unistd.h>
#ضمن <sys/syscall.h>
#ضمن <sys/resource.h>

#که_تعریف_شوی HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
ساكن صحيح
syscall_bpf (صحيح cmd, اتحاد bpf_attr *attr, بینښه صحيح اندازه)
{
  أرجع (صحيح) syscall (__NR_bpf, cmd, attr, اندازه);
}
#    define bpf syscall_bpf
#  endif

#د_که_پای

ساكن مضمن uint64_t
ptr_منزل_u64 (ثابت فراغ *ptr)
{
  أرجع (uint64_t) (بینښه اوږد) ptr;
}

تعداد
{
  HAS_WILDCARD = 1
};

بنية bpf_پروګرام
{
  size_t allocated;
  size_t کارول_شوی;
  بینښه صحيح خاص;
  حرف پروګرام[];
};

#که_تعریف_شوی HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((بنية bpf_insn) { .کوډ = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(اندازه_2, DST, SRC, OFF) \
    ((بنية bpf_insn) {                   \
        .کوډ = BPF_LDX | BPF_اندازه (اندازه_2) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((بنية bpf_insn) { .کوډ = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((بنية bpf_insn) { .کوډ = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((بنية bpf_insn) { .کوډ = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((بنية bpf_insn) { .کوډ = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((بنية bpf_insn) { .کوډ = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((بنية bpf_insn) { .کوډ = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((بنية bpf_insn) { .کوډ = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#د_که_پای

#که_تعریف_شوی HAVE_EBPF
ساكن size_t
bpf_پروګرام_instructions (بنية bpf_پروګرام *پروګرام)
{
  أرجع پروګرام->کارول_شوی / (حجم (بنية bpf_insn));
}
#د_که_پای

بنية bpf_پروګرام *
bpf_پروګرام_new (size_t اندازه)
{
  بنية bpf_پروګرام *p = xmalloc (اندازه + حجم (بنية bpf_پروګرام));

  p->کارول_شوی = 0;
  p->خاص = 0;
  p->allocated = اندازه;

  أرجع p;
}

بنية bpf_پروګرام *
bpf_پروګرام_append (بنية bpf_پروګرام *p, فراغ *معلومات, size_t اندازه)
{
  إذا (p->allocated <= p->کارول_شوی + اندازه)
    {
      p->allocated += اندازه * 2;
      p = xrealloc (p, p->allocated + حجم (بنية bpf_پروګرام));
    }
  حافظه_نقل (p->پروګرام + p->کارول_شوی, معلومات, اندازه);
  p->کارول_شوی += اندازه;
  أرجع p;
}

بنية bpf_پروګرام *
bpf_پروګرام_init_dev (بنية bpf_پروګرام *پروګرام, libcrun_error_t *err arg_ناکارول_شوی)
{
#که_تعریف_شوی HAVE_EBPF
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
  پروګرام = bpf_پروګرام_append (پروګرام, pre_insn, حجم (pre_insn));
#د_که_پای
  أرجع پروګرام;
}

بنية bpf_پروګرام *
bpf_پروګرام_append_dev (بنية bpf_پروګرام *پروګرام, ثابت حرف *access, حرف type, صحيح major, صحيح minor, bool accept,
                        libcrun_error_t *err arg_ناکارول_شوی)
{
#که_تعریف_شوی HAVE_EBPF
  صحيح i;
  صحيح bpf_access = 0;
  صحيح bpf_type = type == 'b' ? BPF_DEVCG_DEV_بلاک : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  صحيح شمېره_instructions = 0;
  بنية bpf_insn accept_بلاک[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  إذا (access == عدم)
    access = "";

  إذا (پروګرام->خاص & HAS_WILDCARD)
    أرجع پروګرام;

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
  شمېره_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  إذا (has_type)
    {
      بنية bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, شمېره_instructions) };
      شمېره_instructions--;
      پروګرام = bpf_پروګرام_append (پروګرام, i, حجم (i));
    }
  إذا (has_access)
    {
      بنية bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, شمېره_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, شمېره_instructions - 2),
      };
      شمېره_instructions -= 3;
      پروګرام = bpf_پروګرام_append (پروګرام, i, حجم (i));
    }
  إذا (has_major)
    {
      بنية bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, شمېره_instructions) };
      شمېره_instructions--;
      پروګرام = bpf_پروګرام_append (پروګرام, i, حجم (i));
    }
  إذا (has_minor)
    {
      بنية bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, شمېره_instructions) };
      شمېره_instructions--;
      پروګرام = bpf_پروګرام_append (پروګرام, i, حجم (i));
    }

  إذا (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    پروګرام->خاص |= HAS_WILDCARD;

  پروګرام = bpf_پروګرام_append (پروګرام, accept_بلاک, حجم (accept_بلاک));
#د_که_پای
  (فراغ) access;
  (فراغ) type;
  (فراغ) major;
  (فراغ) minor;
  (فراغ) accept;

  أرجع پروګرام;
}

بنية bpf_پروګرام *
bpf_پروګرام_complete_dev (بنية bpf_پروګرام *پروګرام, libcrun_error_t *err arg_ناکارول_شوی)
{
#که_تعریف_شوی HAVE_EBPF
  بنية bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  إذا (پروګرام->خاص & HAS_WILDCARD)
    أرجع پروګرام;

  پروګرام = bpf_پروګرام_append (پروګرام, &i, حجم (i));
#د_که_پای
  أرجع پروګرام;
}

ساكن صحيح
read_all_progs (صحيح dirfd, uint32_t **progs_بهر, size_t *n_progs_بهر, libcrun_error_t *err)
{
#که_تعریف_شوی HAVE_EBPF
  cleanup_free uint32_t *progs = عدم;
  اتحاد bpf_attr attr;
  صحيح ret;
  size_t cur_اندازه;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  لكل (cur_اندازه = 64; cur_اندازه <= 4096; cur_اندازه *= 2)
    {
      progs = xrealloc (progs, حجم (uint32_t) * cur_اندازه);

      memset (&attr, 0, حجم (attr));
      attr.query.موخه_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_اندازه;
      attr.query.prog_ids = ptr_منزل_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, حجم (attr));
    }
  طالما (ret < 0 && errno == ENOSPC)
    ;

  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "bpf query");

  *progs_بهر = progs;
  progs = عدم;
  *n_progs_بهر = attr.query.prog_cnt;
  أرجع 0;
#وإلا
  (فراغ) dirfd;

  *progs_بهر = عدم;
  *n_progs_بهر = 0;

  أرجع crun_make_error (err, 0, "eBPF not supported");
#د_که_پای
}

ساكن صحيح
remove_all_progs (صحيح dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#که_تعریف_شوی HAVE_EBPF
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
      attr.موخه_fd = dirfd;
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
#د_که_پای
}

ساكن صحيح
ebpf_attach_پروګرام (صحيح fd, صحيح dirfd, libcrun_error_t *err)
{
#که_نه_تعریف_شوی HAVE_EBPF
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
      attr.موخه_fd = dirfd;
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
#د_که_پای
}

ساكن فراغ
bump_memlock ()
{
  بنية rlimit حد;
  صحيح ret;

  حد.rlim_cur = RLIM_INFINITY;
  حد.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &حد);
  إذا (ret == 0)
    أرجع;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &حد);
  إذا (ret < 0)
    أرجع;

  حد.rlim_cur = حد.rlim_max;
  /* Best effort, ignore errors.  */
  (فراغ) setrlimit (RLIMIT_MEMLOCK, &حد);
}

صحيح
libcrun_ebpf_بارول (بنية bpf_پروګرام *پروګرام, صحيح dirfd, ثابت حرف *pin, libcrun_error_t *err)
{
#که_نه_تعریف_شوی HAVE_EBPF
  (فراغ) dirfd;
  (فراغ) پروګرام;
  (فراغ) pin;
  (فراغ) ebpf_attach_پروګرام;

  أرجع crun_make_error (err, 0, "eBPF not supported");
#وإلا
  cleanup_close صحيح fd = -1;
  اتحاد bpf_attr attr;
  صحيح ret;

  memset (&attr, 0, حجم (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_منزل_u64 (پروګرام->پروګرام);
  attr.insn_cnt = bpf_پروګرام_instructions (پروګرام);
  attr.license = ptr_منزل_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_بارول, &attr, حجم (attr));
  إذا (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_بارول, &attr, حجم (attr));
    }
  إذا (fd < 0)
    {
      ثابت size_t max_log_اندازه = 1 << 20;
      cleanup_free حرف *لوګاریتم = عدم;
      size_t log_اندازه = 8192;

    retry:
      لوګاریتم = xrealloc (لوګاریتم, log_اندازه);
      لوګاریتم[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_منزل_u64 (لوګاریتم);
      attr.log_اندازه = log_اندازه;

      fd = bpf (BPF_PROG_بارول, &attr, حجم (attr));
      إذا (fd < 0)
        {
          إذا (errno == ENOSPC && log_اندازه < max_log_اندازه)
            {
              /* The provided buffer was not big enough.  */
              log_اندازه *= 2;
              اذهب retry;
            }
          أرجع crun_make_error (err, errno, "bpf create `%s`", لوګاریتم);
        }
    }

  إذا (dirfd >= 0)
    {
      ret = ebpf_attach_پروګرام (fd, dirfd, err);
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
#د_که_پای
}

صحيح
libcrun_ebpf_read_پروګرام (بنية bpf_پروګرام **پروګرام_ret, ثابت حرف *لار, libcrun_error_t *err)
{
#که_نه_تعریف_شوی HAVE_EBPF
  (فراغ) پروګرام_ret;
  (فراغ) لار;

  أرجع crun_make_error (err, 0, "eBPF not supported");
#وإلا
  cleanup_free بنية bpf_پروګرام *پروګرام = عدم;
  cleanup_free حرف *بفر = عدم;
  cleanup_close صحيح prog_fd = -1;
  size_t بفر_اندازه = 0;
  بنية bpf_prog_info info;
  اتحاد bpf_attr attr;
  صحيح ret;

  memset (&attr, 0, حجم (attr));
  attr.pathname = ptr_منزل_u64 (لار);

  prog_fd = bpf (BPF_OBJ_GET, &attr, حجم (attr));
  إذا (UNLIKELY (prog_fd < 0))
    أرجع crun_make_error (err, errno, "bpf get `%s`", لار);

  memset (&info, 0, حجم (info));

  memset (&attr, 0, حجم (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_منزل_u64 (&info);
  attr.info.info_len = حجم (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, حجم (attr));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "bpf get info `%s`", لار);

  بفر_اندازه = info.xlated_prog_len;
  بفر = xmalloc (بفر_اندازه);

  memset (&info, 0, حجم (info));
  info.xlated_prog_insns = ptr_منزل_u64 (بفر);
  info.xlated_prog_len = بفر_اندازه;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, حجم (attr));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "bpf get info `%s`", لار);

  پروګرام = bpf_پروګرام_new (بفر_اندازه);
  پروګرام = bpf_پروګرام_append (پروګرام, بفر, بفر_اندازه);

  *پروګرام_ret = پروګرام;
  پروګرام = عدم;

  أرجع 0;
#د_که_پای
}

صحيح
libcrun_ebpf_query_cgroup_progs (ثابت حرف *cgroup_لار, uint32_t **progs_بهر, size_t *n_progs_بهر, libcrun_error_t *err)
{
#که_نه_تعریف_شوی HAVE_EBPF
  (فراغ) cgroup_لار;
  *progs_بهر = عدم;
  *n_progs_بهر = 0;
  أرجع crun_make_error (err, 0, "eBPF not supported");
#وإلا
  cleanup_close صحيح cgroup_fd = -1;

  cgroup_fd = open (cgroup_لار, O_لار | O_CLOEXEC);
  إذا (UNLIKELY (cgroup_fd < 0))
    أرجع crun_make_error (err, errno, "open cgroup path `%s`", cgroup_لار);

  أرجع read_all_progs (cgroup_fd, progs_بهر, n_progs_بهر, err);
#د_که_پای
}

bool
libcrun_ebpf_cmp_programs (بنية bpf_پروګرام *program1, بنية bpf_پروګرام *program2)
{
  إذا (program1->کارول_شوی != program2->کارول_شوی)
    أرجع false;

  أرجع memcmp (program1->پروګرام, program2->پروګرام, program1->کارول_شوی) == 0;
}
