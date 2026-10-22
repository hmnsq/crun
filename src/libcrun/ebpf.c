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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "ebpf.h"
#समावेश "utils.h"
#समावेश <unistd.h>
#समावेश <sys/syscall.h>
#समावेश <sys/resource.h>

#अगर_घोषित HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
अटल पूर्णांक
syscall_bpf (पूर्णांक cmd, जोड़ bpf_attr *attr, अचिन्हित पूर्णांक आकार)
{
  वापस (पूर्णांक) syscall (__NR_bpf, cmd, attr, आकार);
}
#    define bpf syscall_bpf
#  endif

#पूर्ण_अगर

अटल अंतरभूत uint64_t
ptr_to_u64 (स्थिर व्योम *ptr)
{
  वापस (uint64_t) (अचिन्हित दीर्घ) ptr;
}

क्रमागत
{
  HAS_WILDCARD = 1
};

काष्ठा bpf_program
{
  माप_प्रकार allocated;
  माप_प्रकार used;
  अचिन्हित पूर्णांक निजी;
  अक्षर program[];
};

#अगर_घोषित HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((काष्ठा bpf_insn) { .code = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(आकार_2, DST, SRC, OFF) \
    ((काष्ठा bpf_insn) {                   \
        .code = BPF_LDX | BPF_आकार (आकार_2) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((काष्ठा bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((काष्ठा bpf_insn) { .code = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((काष्ठा bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((काष्ठा bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((काष्ठा bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((काष्ठा bpf_insn) { .code = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((काष्ठा bpf_insn) { .code = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#पूर्ण_अगर

#अगर_घोषित HAVE_EBPF
अटल माप_प्रकार
bpf_program_instructions (काष्ठा bpf_program *program)
{
  वापस program->used / (माप (काष्ठा bpf_insn));
}
#पूर्ण_अगर

काष्ठा bpf_program *
bpf_program_new (माप_प्रकार आकार)
{
  काष्ठा bpf_program *p = xmalloc (आकार + माप (काष्ठा bpf_program));

  p->used = 0;
  p->निजी = 0;
  p->allocated = आकार;

  वापस p;
}

काष्ठा bpf_program *
bpf_program_append (काष्ठा bpf_program *p, व्योम *तथ्य, माप_प्रकार आकार)
{
  अगर (p->allocated <= p->used + आकार)
    {
      p->allocated += आकार * 2;
      p = xrealloc (p, p->allocated + माप (काष्ठा bpf_program));
    }
  स_नकल (p->program + p->used, तथ्य, आकार);
  p->used += आकार;
  वापस p;
}

काष्ठा bpf_program *
bpf_program_init_dev (काष्ठा bpf_program *program, libcrun_error_t *err arg_unused)
{
#अगर_घोषित HAVE_EBPF
  /* taken from systemd.  */
  काष्ठा bpf_insn pre_insn[] = {
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
  program = bpf_program_append (program, pre_insn, माप (pre_insn));
#पूर्ण_अगर
  वापस program;
}

काष्ठा bpf_program *
bpf_program_append_dev (काष्ठा bpf_program *program, स्थिर अक्षर *access, अक्षर type, पूर्णांक major, पूर्णांक minor, bool accept,
                        libcrun_error_t *err arg_unused)
{
#अगर_घोषित HAVE_EBPF
  पूर्णांक i;
  पूर्णांक bpf_access = 0;
  पूर्णांक bpf_type = type == 'b' ? BPF_DEVCG_DEV_BLOCK : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  पूर्णांक संख्या_instructions = 0;
  काष्ठा bpf_insn accept_block[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  अगर (access == शून्य)
    access = "";

  अगर (program->निजी & HAS_WILDCARD)
    वापस program;

  क्रम (i = 0; access[i]; i++)
    {
      चयन (access[i])
        {
        हाल 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          अवरोध;

        हाल 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          अवरोध;

        हाल 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          अवरोध;
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
  संख्या_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  अगर (has_type)
    {
      काष्ठा bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, संख्या_instructions) };
      संख्या_instructions--;
      program = bpf_program_append (program, i, माप (i));
    }
  अगर (has_access)
    {
      काष्ठा bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, संख्या_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, संख्या_instructions - 2),
      };
      संख्या_instructions -= 3;
      program = bpf_program_append (program, i, माप (i));
    }
  अगर (has_major)
    {
      काष्ठा bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, संख्या_instructions) };
      संख्या_instructions--;
      program = bpf_program_append (program, i, माप (i));
    }
  अगर (has_minor)
    {
      काष्ठा bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, संख्या_instructions) };
      संख्या_instructions--;
      program = bpf_program_append (program, i, माप (i));
    }

  अगर (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    program->निजी |= HAS_WILDCARD;

  program = bpf_program_append (program, accept_block, माप (accept_block));
#पूर्ण_अगर
  (व्योम) access;
  (व्योम) type;
  (व्योम) major;
  (व्योम) minor;
  (व्योम) accept;

  वापस program;
}

काष्ठा bpf_program *
bpf_program_complete_dev (काष्ठा bpf_program *program, libcrun_error_t *err arg_unused)
{
#अगर_घोषित HAVE_EBPF
  काष्ठा bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  अगर (program->निजी & HAS_WILDCARD)
    वापस program;

  program = bpf_program_append (program, &i, माप (i));
#पूर्ण_अगर
  वापस program;
}

अटल पूर्णांक
read_all_progs (पूर्णांक dirfd, uint32_t **progs_out, माप_प्रकार *n_progs_out, libcrun_error_t *err)
{
#अगर_घोषित HAVE_EBPF
  cleanup_free uint32_t *progs = शून्य;
  जोड़ bpf_attr attr;
  पूर्णांक ret;
  माप_प्रकार cur_आकार;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  क्रम (cur_आकार = 64; cur_आकार <= 4096; cur_आकार *= 2)
    {
      progs = xrealloc (progs, माप (uint32_t) * cur_आकार);

      स_रखो (&attr, 0, माप (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_आकार;
      attr.query.prog_ids = ptr_to_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, माप (attr));
    }
  जबतक (ret < 0 && त्रुटि_सं == ENOSPC)
    ;

  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "bpf query");

  *progs_out = progs;
  progs = शून्य;
  *n_progs_out = attr.query.prog_cnt;
  वापस 0;
#अन्यथा
  (व्योम) dirfd;

  *progs_out = शून्य;
  *n_progs_out = 0;

  वापस crun_make_error (err, 0, "eBPF not supported");
#पूर्ण_अगर
}

अटल पूर्णांक
remove_all_progs (पूर्णांक dirfd, uint32_t *progs, माप_प्रकार n_progs, libcrun_error_t *err)
{
#अगर_घोषित HAVE_EBPF
  जोड़ bpf_attr attr;
  माप_प्रकार i;

  क्रम (i = 0; i < n_progs; i++)
    {
      cleanup_close पूर्णांक fd = -1;
      पूर्णांक ret;

      स_रखो (&attr, 0, माप (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, माप (attr));
      अगर (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          अगर (त्रुटि_सं == ENOENT)
            जारी;

          वापस crun_make_error (err, त्रुटि_सं, "cannot open existing eBPF program");
        }

      स_रखो (&attr, 0, माप (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, माप (attr));
      अगर (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          अगर (त्रुटि_सं == ENOENT)
            जारी;

          वापस crun_make_error (err, त्रुटि_सं, "cannot detach eBPF program");
        }
    }
  वापस 0;
#अन्यथा
  (व्योम) dirfd;
  (व्योम) progs;
  (व्योम) n_progs;
  वापस crun_make_error (err, 0, "eBPF not supported");
#पूर्ण_अगर
}

अटल पूर्णांक
ebpf_attach_program (पूर्णांक fd, पूर्णांक dirfd, libcrun_error_t *err)
{
#अगर_अघोषित HAVE_EBPF
  (व्योम) fd;
  (व्योम) dirfd;
  (व्योम) read_all_progs;
  (व्योम) remove_all_progs;

  वापस crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  स्थिर पूर्णांक MAX_ATTEMPTS = 20;
  पूर्णांक attempt;

  क्रम (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = शून्य;
      cleanup_close पूर्णांक replacefd = -1;
      जोड़ bpf_attr attr;
      माप_प्रकार n_progs = 0;
      पूर्णांक ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      अगर (! skip_replace && n_progs == 1)
        {
          स_रखो (&attr, 0, माप (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, माप (attr));
          अगर (UNLIKELY (replacefd < 0))
            {
              अगर (त्रुटि_सं == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  जारी;
                }
              वापस crun_make_error (err, त्रुटि_सं, "cannot open existing eBPF program");
            }
        }
#  endif

      स_रखो (&attr, 0, माप (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      अगर (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, माप (attr));
      अगर (UNLIKELY (ret < 0))
        {
          अगर (त्रुटि_सं == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              जारी;
            }
#  ifdef BPF_F_REPLACE
          अगर (त्रुटि_सं == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              जारी;
            }
#  endif
          वापस crun_make_error (err, त्रुटि_सं, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      अगर (replacefd < 0 && n_progs)
        वापस remove_all_progs (dirfd, progs, n_progs, err);

      वापस 0;
    }
#पूर्ण_अगर
}

अटल व्योम
bump_memlock ()
{
  काष्ठा rlimit limit;
  पूर्णांक ret;

  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &limit);
  अगर (ret == 0)
    वापस;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &limit);
  अगर (ret < 0)
    वापस;

  limit.rlim_cur = limit.rlim_max;
  /* Best effort, ignore errors.  */
  (व्योम) setrlimit (RLIMIT_MEMLOCK, &limit);
}

पूर्णांक
libcrun_ebpf_load (काष्ठा bpf_program *program, पूर्णांक dirfd, स्थिर अक्षर *pin, libcrun_error_t *err)
{
#अगर_अघोषित HAVE_EBPF
  (व्योम) dirfd;
  (व्योम) program;
  (व्योम) pin;
  (व्योम) ebpf_attach_program;

  वापस crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
  cleanup_close पूर्णांक fd = -1;
  जोड़ bpf_attr attr;
  पूर्णांक ret;

  स_रखो (&attr, 0, माप (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_to_u64 (program->program);
  attr.insn_cnt = bpf_program_instructions (program);
  attr.license = ptr_to_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_LOAD, &attr, माप (attr));
  अगर (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_LOAD, &attr, माप (attr));
    }
  अगर (fd < 0)
    {
      स्थिर माप_प्रकार max_log_आकार = 1 << 20;
      cleanup_free अक्षर *log = शून्य;
      माप_प्रकार log_आकार = 8192;

    retry:
      log = xrealloc (log, log_आकार);
      log[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_to_u64 (log);
      attr.log_आकार = log_आकार;

      fd = bpf (BPF_PROG_LOAD, &attr, माप (attr));
      अगर (fd < 0)
        {
          अगर (त्रुटि_सं == ENOSPC && log_आकार < max_log_आकार)
            {
              /* The provided buffer was not big enough.  */
              log_आकार *= 2;
              जाओ retry;
            }
          वापस crun_make_error (err, त्रुटि_सं, "bpf create `%s`", log);
        }
    }

  अगर (dirfd >= 0)
    {
      ret = ebpf_attach_program (fd, dirfd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  /* Optionally pin the program to the specified path.  */
  अगर (pin)
    {
      unlink (pin);

      स_रखो (&attr, 0, माप (attr));
      attr.pathname = ptr_to_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, माप (attr));
      अगर (ret < 0)
        वापस crun_make_error (err, त्रुटि_सं, "bpf pin to `%s`", pin);
    }

  वापस 0;
#पूर्ण_अगर
}

पूर्णांक
libcrun_ebpf_read_program (काष्ठा bpf_program **program_ret, स्थिर अक्षर *path, libcrun_error_t *err)
{
#अगर_अघोषित HAVE_EBPF
  (व्योम) program_ret;
  (व्योम) path;

  वापस crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
  cleanup_free काष्ठा bpf_program *program = शून्य;
  cleanup_free अक्षर *बफर = शून्य;
  cleanup_close पूर्णांक prog_fd = -1;
  माप_प्रकार बफर_आकार = 0;
  काष्ठा bpf_prog_info info;
  जोड़ bpf_attr attr;
  पूर्णांक ret;

  स_रखो (&attr, 0, माप (attr));
  attr.pathname = ptr_to_u64 (path);

  prog_fd = bpf (BPF_OBJ_GET, &attr, माप (attr));
  अगर (UNLIKELY (prog_fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "bpf get `%s`", path);

  स_रखो (&info, 0, माप (info));

  स_रखो (&attr, 0, माप (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_to_u64 (&info);
  attr.info.info_len = माप (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, माप (attr));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "bpf get info `%s`", path);

  बफर_आकार = info.xlated_prog_len;
  बफर = xmalloc (बफर_आकार);

  स_रखो (&info, 0, माप (info));
  info.xlated_prog_insns = ptr_to_u64 (बफर);
  info.xlated_prog_len = बफर_आकार;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, माप (attr));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "bpf get info `%s`", path);

  program = bpf_program_new (बफर_आकार);
  program = bpf_program_append (program, बफर, बफर_आकार);

  *program_ret = program;
  program = शून्य;

  वापस 0;
#पूर्ण_अगर
}

पूर्णांक
libcrun_ebpf_query_cgroup_progs (स्थिर अक्षर *cgroup_path, uint32_t **progs_out, माप_प्रकार *n_progs_out, libcrun_error_t *err)
{
#अगर_अघोषित HAVE_EBPF
  (व्योम) cgroup_path;
  *progs_out = शून्य;
  *n_progs_out = 0;
  वापस crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
  cleanup_close पूर्णांक cgroup_fd = -1;

  cgroup_fd = खोलो (cgroup_path, O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (cgroup_fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open cgroup path `%s`", cgroup_path);

  वापस read_all_progs (cgroup_fd, progs_out, n_progs_out, err);
#पूर्ण_अगर
}

bool
libcrun_ebpf_cmp_programs (काष्ठा bpf_program *program1, काष्ठा bpf_program *program2)
{
  अगर (program1->used != program2->used)
    वापस false;

  वापस स_भेद (program1->program, program2->program, program1->used) == 0;
}
