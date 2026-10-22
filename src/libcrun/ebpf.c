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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "ebpf.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/syscall.h>
#అన్తర్భావ <sys/resource.h>

#నిర్వచితమైతే HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
స్థిర పూర్ణాంకం
syscall_bpf (పూర్ణాంకం cmd, సంఘం bpf_attr *attr, గుర్తులేని పూర్ణాంకం కొలత)
{
  తిరిగి (పూర్ణాంకం) syscall (__NR_bpf, cmd, attr, కొలత);
}
#    define bpf syscall_bpf
#  endif

#అయితే_ముగింపు

స్థిర అంతఃవరుస uint64_t
ptr_గమ్యం_u64 (స్థిరరాశి శూన్య *ptr)
{
  తిరిగి (uint64_t) (గుర్తులేని దీర్ఘ) ptr;
}

గణన
{
  HAS_WILDCARD = 1
};

నిర్మాణం bpf_కార్యక్రమం
{
  size_t allocated;
  size_t వాడిన;
  గుర్తులేని పూర్ణాంకం అంతరంగ;
  అక్షరం కార్యక్రమం[];
};

#నిర్వచితమైతే HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((నిర్మాణం bpf_insn) { .సంకేతం_2 = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(కొలత_2, DST, SRC, OFF) \
    ((నిర్మాణం bpf_insn) {                   \
        .సంకేతం_2 = BPF_LDX | BPF_కొలత (కొలత_2) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((నిర్మాణం bpf_insn) { .సంకేతం_2 = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((నిర్మాణం bpf_insn) { .సంకేతం_2 = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((నిర్మాణం bpf_insn) { .సంకేతం_2 = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((నిర్మాణం bpf_insn) { .సంకేతం_2 = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((నిర్మాణం bpf_insn) { .సంకేతం_2 = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((నిర్మాణం bpf_insn) { .సంకేతం_2 = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((నిర్మాణం bpf_insn) { .సంకేతం_2 = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_EBPF
స్థిర size_t
bpf_కార్యక్రమం_instructions (నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం)
{
  తిరిగి కార్యక్రమం->వాడిన / (పరిమాణం (నిర్మాణం bpf_insn));
}
#అయితే_ముగింపు

నిర్మాణం bpf_కార్యక్రమం *
bpf_కార్యక్రమం_new (size_t కొలత)
{
  నిర్మాణం bpf_కార్యక్రమం *p = xmalloc (కొలత + పరిమాణం (నిర్మాణం bpf_కార్యక్రమం));

  p->వాడిన = 0;
  p->అంతరంగ = 0;
  p->allocated = కొలత;

  తిరిగి p;
}

నిర్మాణం bpf_కార్యక్రమం *
bpf_కార్యక్రమం_append (నిర్మాణం bpf_కార్యక్రమం *p, శూన్య *దత్తాంశం, size_t కొలత)
{
  ఒకవేళ (p->allocated <= p->వాడిన + కొలత)
    {
      p->allocated += కొలత * 2;
      p = xrealloc (p, p->allocated + పరిమాణం (నిర్మాణం bpf_కార్యక్రమం));
    }
  జ్ఞాపకం_నకలు (p->కార్యక్రమం + p->వాడిన, దత్తాంశం, కొలత);
  p->వాడిన += కొలత;
  తిరిగి p;
}

నిర్మాణం bpf_కార్యక్రమం *
bpf_కార్యక్రమం_init_dev (నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం, libcrun_error_t *err arg_వాడని)
{
#నిర్వచితమైతే HAVE_EBPF
  /* taken from systemd.  */
  నిర్మాణం bpf_insn pre_insn[] = {
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
  కార్యక్రమం = bpf_కార్యక్రమం_append (కార్యక్రమం, pre_insn, పరిమాణం (pre_insn));
#అయితే_ముగింపు
  తిరిగి కార్యక్రమం;
}

నిర్మాణం bpf_కార్యక్రమం *
bpf_కార్యక్రమం_append_dev (నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం, స్థిరరాశి అక్షరం *access, అక్షరం type, పూర్ణాంకం major, పూర్ణాంకం minor, bool accept,
                        libcrun_error_t *err arg_వాడని)
{
#నిర్వచితమైతే HAVE_EBPF
  పూర్ణాంకం i;
  పూర్ణాంకం bpf_access = 0;
  పూర్ణాంకం bpf_type = type == 'b' ? BPF_DEVCG_DEV_ఖండం : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  పూర్ణాంకం సంఖ్య_instructions = 0;
  నిర్మాణం bpf_insn accept_ఖండం[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  ఒకవేళ (access == ఖాళీ)
    access = "";

  ఒకవేళ (కార్యక్రమం->అంతరంగ & HAS_WILDCARD)
    తిరిగి కార్యక్రమం;

  కొరకు (i = 0; access[i]; i++)
    {
      ఎంపిక (access[i])
        {
        స్థితి 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          ఆపు;

        స్థితి 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          ఆపు;

        స్థితి 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          ఆపు;
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
  సంఖ్య_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  ఒకవేళ (has_type)
    {
      నిర్మాణం bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, సంఖ్య_instructions) };
      సంఖ్య_instructions--;
      కార్యక్రమం = bpf_కార్యక్రమం_append (కార్యక్రమం, i, పరిమాణం (i));
    }
  ఒకవేళ (has_access)
    {
      నిర్మాణం bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, సంఖ్య_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, సంఖ్య_instructions - 2),
      };
      సంఖ్య_instructions -= 3;
      కార్యక్రమం = bpf_కార్యక్రమం_append (కార్యక్రమం, i, పరిమాణం (i));
    }
  ఒకవేళ (has_major)
    {
      నిర్మాణం bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, సంఖ్య_instructions) };
      సంఖ్య_instructions--;
      కార్యక్రమం = bpf_కార్యక్రమం_append (కార్యక్రమం, i, పరిమాణం (i));
    }
  ఒకవేళ (has_minor)
    {
      నిర్మాణం bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, సంఖ్య_instructions) };
      సంఖ్య_instructions--;
      కార్యక్రమం = bpf_కార్యక్రమం_append (కార్యక్రమం, i, పరిమాణం (i));
    }

  ఒకవేళ (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    కార్యక్రమం->అంతరంగ |= HAS_WILDCARD;

  కార్యక్రమం = bpf_కార్యక్రమం_append (కార్యక్రమం, accept_ఖండం, పరిమాణం (accept_ఖండం));
#అయితే_ముగింపు
  (శూన్య) access;
  (శూన్య) type;
  (శూన్య) major;
  (శూన్య) minor;
  (శూన్య) accept;

  తిరిగి కార్యక్రమం;
}

నిర్మాణం bpf_కార్యక్రమం *
bpf_కార్యక్రమం_complete_dev (నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం, libcrun_error_t *err arg_వాడని)
{
#నిర్వచితమైతే HAVE_EBPF
  నిర్మాణం bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  ఒకవేళ (కార్యక్రమం->అంతరంగ & HAS_WILDCARD)
    తిరిగి కార్యక్రమం;

  కార్యక్రమం = bpf_కార్యక్రమం_append (కార్యక్రమం, &i, పరిమాణం (i));
#అయితే_ముగింపు
  తిరిగి కార్యక్రమం;
}

స్థిర పూర్ణాంకం
read_all_progs (పూర్ణాంకం dirfd, uint32_t **progs_బహిర్గతం, size_t *n_progs_బహిర్గతం, libcrun_error_t *err)
{
#నిర్వచితమైతే HAVE_EBPF
  cleanup_free uint32_t *progs = ఖాళీ;
  సంఘం bpf_attr attr;
  పూర్ణాంకం ret;
  size_t cur_కొలత;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  కొరకు (cur_కొలత = 64; cur_కొలత <= 4096; cur_కొలత *= 2)
    {
      progs = xrealloc (progs, పరిమాణం (uint32_t) * cur_కొలత);

      memset (&attr, 0, పరిమాణం (attr));
      attr.query.లక్ష్యం_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_కొలత;
      attr.query.prog_ids = ptr_గమ్యం_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, పరిమాణం (attr));
    }
  అయితే (ret < 0 && errno == ENOSPC)
    ;

  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "bpf query");

  *progs_బహిర్గతం = progs;
  progs = ఖాళీ;
  *n_progs_బహిర్గతం = attr.query.prog_cnt;
  తిరిగి 0;
#లేకపోతే
  (శూన్య) dirfd;

  *progs_బహిర్గతం = ఖాళీ;
  *n_progs_బహిర్గతం = 0;

  తిరిగి crun_make_error (err, 0, "eBPF not supported");
#అయితే_ముగింపు
}

స్థిర పూర్ణాంకం
remove_all_progs (పూర్ణాంకం dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#నిర్వచితమైతే HAVE_EBPF
  సంఘం bpf_attr attr;
  size_t i;

  కొరకు (i = 0; i < n_progs; i++)
    {
      cleanup_close పూర్ణాంకం fd = -1;
      పూర్ణాంకం ret;

      memset (&attr, 0, పరిమాణం (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, పరిమాణం (attr));
      ఒకవేళ (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          ఒకవేళ (errno == ENOENT)
            కొనసాగు;

          తిరిగి crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, పరిమాణం (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.లక్ష్యం_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, పరిమాణం (attr));
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          ఒకవేళ (errno == ENOENT)
            కొనసాగు;

          తిరిగి crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  తిరిగి 0;
#లేకపోతే
  (శూన్య) dirfd;
  (శూన్య) progs;
  (శూన్య) n_progs;
  తిరిగి crun_make_error (err, 0, "eBPF not supported");
#అయితే_ముగింపు
}

స్థిర పూర్ణాంకం
ebpf_attach_కార్యక్రమం (పూర్ణాంకం fd, పూర్ణాంకం dirfd, libcrun_error_t *err)
{
#నిర్వచితం_కాకపోతే HAVE_EBPF
  (శూన్య) fd;
  (శూన్య) dirfd;
  (శూన్య) read_all_progs;
  (శూన్య) remove_all_progs;

  తిరిగి crun_make_error (err, 0, "eBPF not supported");
#లేకపోతే
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  స్థిరరాశి పూర్ణాంకం MAX_ATTEMPTS = 20;
  పూర్ణాంకం attempt;

  కొరకు (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = ఖాళీ;
      cleanup_close పూర్ణాంకం replacefd = -1;
      సంఘం bpf_attr attr;
      size_t n_progs = 0;
      పూర్ణాంకం ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      ఒకవేళ (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, పరిమాణం (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, పరిమాణం (attr));
          ఒకవేళ (UNLIKELY (replacefd < 0))
            {
              ఒకవేళ (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  కొనసాగు;
                }
              తిరిగి crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, పరిమాణం (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.లక్ష్యం_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      ఒకవేళ (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, పరిమాణం (attr));
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              కొనసాగు;
            }
#  ifdef BPF_F_REPLACE
          ఒకవేళ (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              కొనసాగు;
            }
#  endif
          తిరిగి crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      ఒకవేళ (replacefd < 0 && n_progs)
        తిరిగి remove_all_progs (dirfd, progs, n_progs, err);

      తిరిగి 0;
    }
#అయితే_ముగింపు
}

స్థిర శూన్య
bump_memlock ()
{
  నిర్మాణం rlimit హద్దు;
  పూర్ణాంకం ret;

  హద్దు.rlim_cur = RLIM_INFINITY;
  హద్దు.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &హద్దు);
  ఒకవేళ (ret == 0)
    తిరిగి;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &హద్దు);
  ఒకవేళ (ret < 0)
    తిరిగి;

  హద్దు.rlim_cur = హద్దు.rlim_max;
  /* Best effort, ignore errors.  */
  (శూన్య) setrlimit (RLIMIT_MEMLOCK, &హద్దు);
}

పూర్ణాంకం
libcrun_ebpf_ఎక్కించు (నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *pin, libcrun_error_t *err)
{
#నిర్వచితం_కాకపోతే HAVE_EBPF
  (శూన్య) dirfd;
  (శూన్య) కార్యక్రమం;
  (శూన్య) pin;
  (శూన్య) ebpf_attach_కార్యక్రమం;

  తిరిగి crun_make_error (err, 0, "eBPF not supported");
#లేకపోతే
  cleanup_close పూర్ణాంకం fd = -1;
  సంఘం bpf_attr attr;
  పూర్ణాంకం ret;

  memset (&attr, 0, పరిమాణం (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_గమ్యం_u64 (కార్యక్రమం->కార్యక్రమం);
  attr.insn_cnt = bpf_కార్యక్రమం_instructions (కార్యక్రమం);
  attr.license = ptr_గమ్యం_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_ఎక్కించు, &attr, పరిమాణం (attr));
  ఒకవేళ (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_ఎక్కించు, &attr, పరిమాణం (attr));
    }
  ఒకవేళ (fd < 0)
    {
      స్థిరరాశి size_t max_log_కొలత = 1 << 20;
      cleanup_free అక్షరం *సంవర్గమానం = ఖాళీ;
      size_t log_కొలత = 8192;

    retry:
      సంవర్గమానం = xrealloc (సంవర్గమానం, log_కొలత);
      సంవర్గమానం[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_గమ్యం_u64 (సంవర్గమానం);
      attr.log_కొలత = log_కొలత;

      fd = bpf (BPF_PROG_ఎక్కించు, &attr, పరిమాణం (attr));
      ఒకవేళ (fd < 0)
        {
          ఒకవేళ (errno == ENOSPC && log_కొలత < max_log_కొలత)
            {
              /* The provided buffer was not big enough.  */
              log_కొలత *= 2;
              వెళ్ళు retry;
            }
          తిరిగి crun_make_error (err, errno, "bpf create `%s`", సంవర్గమానం);
        }
    }

  ఒకవేళ (dirfd >= 0)
    {
      ret = ebpf_attach_కార్యక్రమం (fd, dirfd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  /* Optionally pin the program to the specified path.  */
  ఒకవేళ (pin)
    {
      unlink (pin);

      memset (&attr, 0, పరిమాణం (attr));
      attr.pathname = ptr_గమ్యం_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, పరిమాణం (attr));
      ఒకవేళ (ret < 0)
        తిరిగి crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  తిరిగి 0;
#అయితే_ముగింపు
}

పూర్ణాంకం
libcrun_ebpf_read_కార్యక్రమం (నిర్మాణం bpf_కార్యక్రమం **కార్యక్రమం_ret, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
#నిర్వచితం_కాకపోతే HAVE_EBPF
  (శూన్య) కార్యక్రమం_ret;
  (శూన్య) మార్గం;

  తిరిగి crun_make_error (err, 0, "eBPF not supported");
#లేకపోతే
  cleanup_free నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం = ఖాళీ;
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  cleanup_close పూర్ణాంకం prog_fd = -1;
  size_t బఫరు_కొలత = 0;
  నిర్మాణం bpf_prog_info info;
  సంఘం bpf_attr attr;
  పూర్ణాంకం ret;

  memset (&attr, 0, పరిమాణం (attr));
  attr.pathname = ptr_గమ్యం_u64 (మార్గం);

  prog_fd = bpf (BPF_OBJ_GET, &attr, పరిమాణం (attr));
  ఒకవేళ (UNLIKELY (prog_fd < 0))
    తిరిగి crun_make_error (err, errno, "bpf get `%s`", మార్గం);

  memset (&info, 0, పరిమాణం (info));

  memset (&attr, 0, పరిమాణం (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_గమ్యం_u64 (&info);
  attr.info.info_len = పరిమాణం (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, పరిమాణం (attr));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "bpf get info `%s`", మార్గం);

  బఫరు_కొలత = info.xlated_prog_len;
  బఫరు = xmalloc (బఫరు_కొలత);

  memset (&info, 0, పరిమాణం (info));
  info.xlated_prog_insns = ptr_గమ్యం_u64 (బఫరు);
  info.xlated_prog_len = బఫరు_కొలత;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, పరిమాణం (attr));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "bpf get info `%s`", మార్గం);

  కార్యక్రమం = bpf_కార్యక్రమం_new (బఫరు_కొలత);
  కార్యక్రమం = bpf_కార్యక్రమం_append (కార్యక్రమం, బఫరు, బఫరు_కొలత);

  *కార్యక్రమం_ret = కార్యక్రమం;
  కార్యక్రమం = ఖాళీ;

  తిరిగి 0;
#అయితే_ముగింపు
}

పూర్ణాంకం
libcrun_ebpf_query_cgroup_progs (స్థిరరాశి అక్షరం *cgroup_మార్గం, uint32_t **progs_బహిర్గతం, size_t *n_progs_బహిర్గతం, libcrun_error_t *err)
{
#నిర్వచితం_కాకపోతే HAVE_EBPF
  (శూన్య) cgroup_మార్గం;
  *progs_బహిర్గతం = ఖాళీ;
  *n_progs_బహిర్గతం = 0;
  తిరిగి crun_make_error (err, 0, "eBPF not supported");
#లేకపోతే
  cleanup_close పూర్ణాంకం cgroup_fd = -1;

  cgroup_fd = open (cgroup_మార్గం, O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (cgroup_fd < 0))
    తిరిగి crun_make_error (err, errno, "open cgroup path `%s`", cgroup_మార్గం);

  తిరిగి read_all_progs (cgroup_fd, progs_బహిర్గతం, n_progs_బహిర్గతం, err);
#అయితే_ముగింపు
}

bool
libcrun_ebpf_cmp_programs (నిర్మాణం bpf_కార్యక్రమం *program1, నిర్మాణం bpf_కార్యక్రమం *program2)
{
  ఒకవేళ (program1->వాడిన != program2->వాడిన)
    తిరిగి false;

  తిరిగి memcmp (program1->కార్యక్రమం, program2->కార్యక్రమం, program1->వాడిన) == 0;
}
