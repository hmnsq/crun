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

#परिभाषय _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव "ebpf.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/syscall.h>
#अन्तर्भाव <sys/resource.h>

#यदि_परिभाषितम् HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
स्थिर पूर्णाङ्क
syscall_bpf (पूर्णाङ्क cmd, सङ्घ bpf_attr *attr, अचिह्नित पूर्णाङ्क आकार)
{
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_bpf, cmd, attr, आकार);
}
#    define bpf syscall_bpf
#  endif

#यदि_अन्तः

स्थिर अन्तःस्थ uint64_t
ptr_गन्तव्यम्_u64 (ध्रुव शून्य *ptr)
{
  प्रत्यागम (uint64_t) (अचिह्नित दीर्घ) ptr;
}

गणना
{
  HAS_WILDCARD = 1
};

संरचना bpf_क्रमादेशः
{
  size_t allocated;
  size_t प्रयुक्तम्;
  अचिह्नित पूर्णाङ्क गुप्त;
  वर्ण क्रमादेशः[];
};

#यदि_परिभाषितम् HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((संरचना bpf_insn) { .संहिता = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(आकार_2, DST, SRC, OFF) \
    ((संरचना bpf_insn) {                   \
        .संहिता = BPF_LDX | BPF_आकार (आकार_2) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((संरचना bpf_insn) { .संहिता = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((संरचना bpf_insn) { .संहिता = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((संरचना bpf_insn) { .संहिता = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((संरचना bpf_insn) { .संहिता = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((संरचना bpf_insn) { .संहिता = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((संरचना bpf_insn) { .संहिता = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((संरचना bpf_insn) { .संहिता = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_EBPF
स्थिर size_t
bpf_क्रमादेशः_instructions (संरचना bpf_क्रमादेशः *क्रमादेशः)
{
  प्रत्यागम क्रमादेशः->प्रयुक्तम् / (परिमाण (संरचना bpf_insn));
}
#यदि_अन्तः

संरचना bpf_क्रमादेशः *
bpf_क्रमादेशः_new (size_t आकार)
{
  संरचना bpf_क्रमादेशः *p = xmalloc (आकार + परिमाण (संरचना bpf_क्रमादेशः));

  p->प्रयुक्तम् = 0;
  p->गुप्त = 0;
  p->allocated = आकार;

  प्रत्यागम p;
}

संरचना bpf_क्रमादेशः *
bpf_क्रमादेशः_append (संरचना bpf_क्रमादेशः *p, शून्य *दत्त, size_t आकार)
{
  यदि (p->allocated <= p->प्रयुक्तम् + आकार)
    {
      p->allocated += आकार * 2;
      p = xrealloc (p, p->allocated + परिमाण (संरचना bpf_क्रमादेशः));
    }
  स्मृतिं_प्रतिलिख (p->क्रमादेशः + p->प्रयुक्तम्, दत्त, आकार);
  p->प्रयुक्तम् += आकार;
  प्रत्यागम p;
}

संरचना bpf_क्रमादेशः *
bpf_क्रमादेशः_init_dev (संरचना bpf_क्रमादेशः *क्रमादेशः, libcrun_error_t *err arg_अप्रयुक्तम्)
{
#यदि_परिभाषितम् HAVE_EBPF
  /* taken from systemd.  */
  संरचना bpf_insn pre_insn[] = {
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
  क्रमादेशः = bpf_क्रमादेशः_append (क्रमादेशः, pre_insn, परिमाण (pre_insn));
#यदि_अन्तः
  प्रत्यागम क्रमादेशः;
}

संरचना bpf_क्रमादेशः *
bpf_क्रमादेशः_append_dev (संरचना bpf_क्रमादेशः *क्रमादेशः, ध्रुव वर्ण *access, वर्ण type, पूर्णाङ्क major, पूर्णाङ्क minor, bool accept,
                        libcrun_error_t *err arg_अप्रयुक्तम्)
{
#यदि_परिभाषितम् HAVE_EBPF
  पूर्णाङ्क i;
  पूर्णाङ्क bpf_access = 0;
  पूर्णाङ्क bpf_type = type == 'b' ? BPF_DEVCG_DEV_खण्डम् : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  पूर्णाङ्क सङ्ख्या_instructions = 0;
  संरचना bpf_insn accept_खण्डम्[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  यदि (access == रिक्त)
    access = "";

  यदि (क्रमादेशः->गुप्त & HAS_WILDCARD)
    प्रत्यागम क्रमादेशः;

  प्रति (i = 0; access[i]; i++)
    {
      विकल्प (access[i])
        {
        पक्ष 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          भङ्ग;

        पक्ष 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          भङ्ग;

        पक्ष 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          भङ्ग;
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
  सङ्ख्या_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  यदि (has_type)
    {
      संरचना bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, सङ्ख्या_instructions) };
      सङ्ख्या_instructions--;
      क्रमादेशः = bpf_क्रमादेशः_append (क्रमादेशः, i, परिमाण (i));
    }
  यदि (has_access)
    {
      संरचना bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, सङ्ख्या_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, सङ्ख्या_instructions - 2),
      };
      सङ्ख्या_instructions -= 3;
      क्रमादेशः = bpf_क्रमादेशः_append (क्रमादेशः, i, परिमाण (i));
    }
  यदि (has_major)
    {
      संरचना bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, सङ्ख्या_instructions) };
      सङ्ख्या_instructions--;
      क्रमादेशः = bpf_क्रमादेशः_append (क्रमादेशः, i, परिमाण (i));
    }
  यदि (has_minor)
    {
      संरचना bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, सङ्ख्या_instructions) };
      सङ्ख्या_instructions--;
      क्रमादेशः = bpf_क्रमादेशः_append (क्रमादेशः, i, परिमाण (i));
    }

  यदि (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    क्रमादेशः->गुप्त |= HAS_WILDCARD;

  क्रमादेशः = bpf_क्रमादेशः_append (क्रमादेशः, accept_खण्डम्, परिमाण (accept_खण्डम्));
#यदि_अन्तः
  (शून्य) access;
  (शून्य) type;
  (शून्य) major;
  (शून्य) minor;
  (शून्य) accept;

  प्रत्यागम क्रमादेशः;
}

संरचना bpf_क्रमादेशः *
bpf_क्रमादेशः_complete_dev (संरचना bpf_क्रमादेशः *क्रमादेशः, libcrun_error_t *err arg_अप्रयुक्तम्)
{
#यदि_परिभाषितम् HAVE_EBPF
  संरचना bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  यदि (क्रमादेशः->गुप्त & HAS_WILDCARD)
    प्रत्यागम क्रमादेशः;

  क्रमादेशः = bpf_क्रमादेशः_append (क्रमादेशः, &i, परिमाण (i));
#यदि_अन्तः
  प्रत्यागम क्रमादेशः;
}

स्थिर पूर्णाङ्क
read_all_progs (पूर्णाङ्क dirfd, uint32_t **progs_बहिर्गमनम्, size_t *n_progs_बहिर्गमनम्, libcrun_error_t *err)
{
#यदि_परिभाषितम् HAVE_EBPF
  cleanup_free uint32_t *progs = रिक्त;
  सङ्घ bpf_attr attr;
  पूर्णाङ्क ret;
  size_t cur_आकार;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  प्रति (cur_आकार = 64; cur_आकार <= 4096; cur_आकार *= 2)
    {
      progs = xrealloc (progs, परिमाण (uint32_t) * cur_आकार);

      स्मृतिं_पूरय (&attr, 0, परिमाण (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_आकार;
      attr.query.prog_ids = ptr_गन्तव्यम्_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, परिमाण (attr));
    }
  यावत् (ret < 0 && errno == ENOSPC)
    ;

  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "bpf query");

  *progs_बहिर्गमनम् = progs;
  progs = रिक्त;
  *n_progs_बहिर्गमनम् = attr.query.prog_cnt;
  प्रत्यागम 0;
#अन्यथा
  (शून्य) dirfd;

  *progs_बहिर्गमनम् = रिक्त;
  *n_progs_बहिर्गमनम् = 0;

  प्रत्यागम crun_make_error (err, 0, "eBPF not supported");
#यदि_अन्तः
}

स्थिर पूर्णाङ्क
remove_all_progs (पूर्णाङ्क dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#यदि_परिभाषितम् HAVE_EBPF
  सङ्घ bpf_attr attr;
  size_t i;

  प्रति (i = 0; i < n_progs; i++)
    {
      cleanup_close पूर्णाङ्क fd = -1;
      पूर्णाङ्क ret;

      स्मृतिं_पूरय (&attr, 0, परिमाण (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, परिमाण (attr));
      यदि (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          यदि (errno == ENOENT)
            अनुवर्त;

          प्रत्यागम crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      स्मृतिं_पूरय (&attr, 0, परिमाण (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, परिमाण (attr));
      यदि (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          यदि (errno == ENOENT)
            अनुवर्त;

          प्रत्यागम crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  प्रत्यागम 0;
#अन्यथा
  (शून्य) dirfd;
  (शून्य) progs;
  (शून्य) n_progs;
  प्रत्यागम crun_make_error (err, 0, "eBPF not supported");
#यदि_अन्तः
}

स्थिर पूर्णाङ्क
ebpf_attach_क्रमादेशः (पूर्णाङ्क fd, पूर्णाङ्क dirfd, libcrun_error_t *err)
{
#यदि_अपरिभाषितम् HAVE_EBPF
  (शून्य) fd;
  (शून्य) dirfd;
  (शून्य) read_all_progs;
  (शून्य) remove_all_progs;

  प्रत्यागम crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  ध्रुव पूर्णाङ्क MAX_ATTEMPTS = 20;
  पूर्णाङ्क attempt;

  प्रति (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = रिक्त;
      cleanup_close पूर्णाङ्क replacefd = -1;
      सङ्घ bpf_attr attr;
      size_t n_progs = 0;
      पूर्णाङ्क ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      यदि (! skip_replace && n_progs == 1)
        {
          स्मृतिं_पूरय (&attr, 0, परिमाण (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, परिमाण (attr));
          यदि (UNLIKELY (replacefd < 0))
            {
              यदि (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  अनुवर्त;
                }
              प्रत्यागम crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      स्मृतिं_पूरय (&attr, 0, परिमाण (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      यदि (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, परिमाण (attr));
      यदि (UNLIKELY (ret < 0))
        {
          यदि (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              अनुवर्त;
            }
#  ifdef BPF_F_REPLACE
          यदि (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              अनुवर्त;
            }
#  endif
          प्रत्यागम crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      यदि (replacefd < 0 && n_progs)
        प्रत्यागम remove_all_progs (dirfd, progs, n_progs, err);

      प्रत्यागम 0;
    }
#यदि_अन्तः
}

स्थिर शून्य
bump_memlock ()
{
  संरचना rlimit सीमा;
  पूर्णाङ्क ret;

  सीमा.rlim_cur = RLIM_INFINITY;
  सीमा.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &सीमा);
  यदि (ret == 0)
    प्रत्यागम;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &सीमा);
  यदि (ret < 0)
    प्रत्यागम;

  सीमा.rlim_cur = सीमा.rlim_max;
  /* Best effort, ignore errors.  */
  (शून्य) setrlimit (RLIMIT_MEMLOCK, &सीमा);
}

पूर्णाङ्क
libcrun_ebpf_आरोपय (संरचना bpf_क्रमादेशः *क्रमादेशः, पूर्णाङ्क dirfd, ध्रुव वर्ण *pin, libcrun_error_t *err)
{
#यदि_अपरिभाषितम् HAVE_EBPF
  (शून्य) dirfd;
  (शून्य) क्रमादेशः;
  (शून्य) pin;
  (शून्य) ebpf_attach_क्रमादेशः;

  प्रत्यागम crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
  cleanup_close पूर्णाङ्क fd = -1;
  सङ्घ bpf_attr attr;
  पूर्णाङ्क ret;

  स्मृतिं_पूरय (&attr, 0, परिमाण (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_गन्तव्यम्_u64 (क्रमादेशः->क्रमादेशः);
  attr.insn_cnt = bpf_क्रमादेशः_instructions (क्रमादेशः);
  attr.license = ptr_गन्तव्यम्_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_आरोपय, &attr, परिमाण (attr));
  यदि (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_आरोपय, &attr, परिमाण (attr));
    }
  यदि (fd < 0)
    {
      ध्रुव size_t max_log_आकार = 1 << 20;
      cleanup_free वर्ण *लघुगणकः = रिक्त;
      size_t log_आकार = 8192;

    retry:
      लघुगणकः = xrealloc (लघुगणकः, log_आकार);
      लघुगणकः[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_गन्तव्यम्_u64 (लघुगणकः);
      attr.log_आकार = log_आकार;

      fd = bpf (BPF_PROG_आरोपय, &attr, परिमाण (attr));
      यदि (fd < 0)
        {
          यदि (errno == ENOSPC && log_आकार < max_log_आकार)
            {
              /* The provided buffer was not big enough.  */
              log_आकार *= 2;
              गच्छ retry;
            }
          प्रत्यागम crun_make_error (err, errno, "bpf create `%s`", लघुगणकः);
        }
    }

  यदि (dirfd >= 0)
    {
      ret = ebpf_attach_क्रमादेशः (fd, dirfd, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  /* Optionally pin the program to the specified path.  */
  यदि (pin)
    {
      unlink (pin);

      स्मृतिं_पूरय (&attr, 0, परिमाण (attr));
      attr.pathname = ptr_गन्तव्यम्_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, परिमाण (attr));
      यदि (ret < 0)
        प्रत्यागम crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  प्रत्यागम 0;
#यदि_अन्तः
}

पूर्णाङ्क
libcrun_ebpf_read_क्रमादेशः (संरचना bpf_क्रमादेशः **क्रमादेशः_ret, ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
#यदि_अपरिभाषितम् HAVE_EBPF
  (शून्य) क्रमादेशः_ret;
  (शून्य) मार्गः;

  प्रत्यागम crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
  cleanup_free संरचना bpf_क्रमादेशः *क्रमादेशः = रिक्त;
  cleanup_free वर्ण *बफर = रिक्त;
  cleanup_close पूर्णाङ्क prog_fd = -1;
  size_t बफर_आकार = 0;
  संरचना bpf_prog_info info;
  सङ्घ bpf_attr attr;
  पूर्णाङ्क ret;

  स्मृतिं_पूरय (&attr, 0, परिमाण (attr));
  attr.pathname = ptr_गन्तव्यम्_u64 (मार्गः);

  prog_fd = bpf (BPF_OBJ_GET, &attr, परिमाण (attr));
  यदि (UNLIKELY (prog_fd < 0))
    प्रत्यागम crun_make_error (err, errno, "bpf get `%s`", मार्गः);

  स्मृतिं_पूरय (&info, 0, परिमाण (info));

  स्मृतिं_पूरय (&attr, 0, परिमाण (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_गन्तव्यम्_u64 (&info);
  attr.info.info_len = परिमाण (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, परिमाण (attr));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "bpf get info `%s`", मार्गः);

  बफर_आकार = info.xlated_prog_len;
  बफर = xmalloc (बफर_आकार);

  स्मृतिं_पूरय (&info, 0, परिमाण (info));
  info.xlated_prog_insns = ptr_गन्तव्यम्_u64 (बफर);
  info.xlated_prog_len = बफर_आकार;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, परिमाण (attr));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "bpf get info `%s`", मार्गः);

  क्रमादेशः = bpf_क्रमादेशः_new (बफर_आकार);
  क्रमादेशः = bpf_क्रमादेशः_append (क्रमादेशः, बफर, बफर_आकार);

  *क्रमादेशः_ret = क्रमादेशः;
  क्रमादेशः = रिक्त;

  प्रत्यागम 0;
#यदि_अन्तः
}

पूर्णाङ्क
libcrun_ebpf_query_cgroup_progs (ध्रुव वर्ण *cgroup_मार्गः, uint32_t **progs_बहिर्गमनम्, size_t *n_progs_बहिर्गमनम्, libcrun_error_t *err)
{
#यदि_अपरिभाषितम् HAVE_EBPF
  (शून्य) cgroup_मार्गः;
  *progs_बहिर्गमनम् = रिक्त;
  *n_progs_बहिर्गमनम् = 0;
  प्रत्यागम crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
  cleanup_close पूर्णाङ्क cgroup_fd = -1;

  cgroup_fd = उद्घाटय (cgroup_मार्गः, O_मार्गः | O_CLOEXEC);
  यदि (UNLIKELY (cgroup_fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open cgroup path `%s`", cgroup_मार्गः);

  प्रत्यागम read_all_progs (cgroup_fd, progs_बहिर्गमनम्, n_progs_बहिर्गमनम्, err);
#यदि_अन्तः
}

bool
libcrun_ebpf_cmp_programs (संरचना bpf_क्रमादेशः *program1, संरचना bpf_क्रमादेशः *program2)
{
  यदि (program1->प्रयुक्तम् != program2->प्रयुक्तम्)
    प्रत्यागम false;

  प्रत्यागम memcmp (program1->क्रमादेशः, program2->क्रमादेशः, program1->प्रयुक्तम्) == 0;
}
