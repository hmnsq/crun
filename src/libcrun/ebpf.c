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

#अन्तर्भाव <config.h>
#अन्तर्भाव "ebpf.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/syscall.h>
#अन्तर्भाव <sys/resource.h>

#ifdef HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
स्थिर पूर्णांक
syscall_bpf (पूर्णांक cmd, संघ bpf_attr *attr, अचिन्हित पूर्णांक size)
{
  परत (पूर्णांक) syscall (__NR_bpf, cmd, attr, size);
}
#    define bpf syscall_bpf
#  endif

#endif

स्थिर ओळीत uint64_t
ptr_to_u64 (स्थिरांक शून्य *ptr)
{
  परत (uint64_t) (अचिन्हित दीर्घ) ptr;
}

गणना
{
  HAS_WILDCARD = 1
};

संरचना bpf_program
{
  माप_प्रकार allocated;
  माप_प्रकार used;
  अचिन्हित पूर्णांक खाजगी;
  अक्षर program[];
};

#ifdef HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((संरचना bpf_insn) { .code = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(SIZE, DST, SRC, OFF) \
    ((संरचना bpf_insn) {                   \
        .code = BPF_LDX | BPF_SIZE (SIZE) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((संरचना bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((संरचना bpf_insn) { .code = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((संरचना bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((संरचना bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((संरचना bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((संरचना bpf_insn) { .code = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((संरचना bpf_insn) { .code = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#endif

#ifdef HAVE_EBPF
स्थिर माप_प्रकार
bpf_program_instructions (संरचना bpf_program *program)
{
  परत program->used / (आकार (संरचना bpf_insn));
}
#endif

संरचना bpf_program *
bpf_program_new (माप_प्रकार size)
{
  संरचना bpf_program *p = xmalloc (size + आकार (संरचना bpf_program));

  p->used = 0;
  p->खाजगी = 0;
  p->allocated = size;

  परत p;
}

संरचना bpf_program *
bpf_program_append (संरचना bpf_program *p, शून्य *data, माप_प्रकार size)
{
  जर (p->allocated <= p->used + size)
    {
      p->allocated += size * 2;
      p = xrealloc (p, p->allocated + आकार (संरचना bpf_program));
    }
  स_नकल (p->program + p->used, data, size);
  p->used += size;
  परत p;
}

संरचना bpf_program *
bpf_program_init_dev (संरचना bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
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
  program = bpf_program_append (program, pre_insn, आकार (pre_insn));
#endif
  परत program;
}

संरचना bpf_program *
bpf_program_append_dev (संरचना bpf_program *program, स्थिरांक अक्षर *access, अक्षर type, पूर्णांक major, पूर्णांक minor, bool accept,
                        libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  पूर्णांक i;
  पूर्णांक bpf_access = 0;
  पूर्णांक bpf_type = type == 'b' ? BPF_DEVCG_DEV_BLOCK : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  पूर्णांक संख्या_instructions = 0;
  संरचना bpf_insn accept_block[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  जर (access == रिक्त)
    access = "";

  जर (program->खाजगी & HAS_WILDCARD)
    परत program;

  साठी (i = 0; access[i]; i++)
    {
      निवड (access[i])
        {
        स्थिती 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          थांबा;

        स्थिती 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          थांबा;

        स्थिती 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          थांबा;
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

  जर (has_type)
    {
      संरचना bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, संख्या_instructions) };
      संख्या_instructions--;
      program = bpf_program_append (program, i, आकार (i));
    }
  जर (has_access)
    {
      संरचना bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, संख्या_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, संख्या_instructions - 2),
      };
      संख्या_instructions -= 3;
      program = bpf_program_append (program, i, आकार (i));
    }
  जर (has_major)
    {
      संरचना bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, संख्या_instructions) };
      संख्या_instructions--;
      program = bpf_program_append (program, i, आकार (i));
    }
  जर (has_minor)
    {
      संरचना bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, संख्या_instructions) };
      संख्या_instructions--;
      program = bpf_program_append (program, i, आकार (i));
    }

  जर (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    program->खाजगी |= HAS_WILDCARD;

  program = bpf_program_append (program, accept_block, आकार (accept_block));
#endif
  (शून्य) access;
  (शून्य) type;
  (शून्य) major;
  (शून्य) minor;
  (शून्य) accept;

  परत program;
}

संरचना bpf_program *
bpf_program_complete_dev (संरचना bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  संरचना bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  जर (program->खाजगी & HAS_WILDCARD)
    परत program;

  program = bpf_program_append (program, &i, आकार (i));
#endif
  परत program;
}

स्थिर पूर्णांक
read_all_progs (पूर्णांक dirfd, uint32_t **progs_out, माप_प्रकार *n_progs_out, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  cleanup_free uint32_t *progs = रिक्त;
  संघ bpf_attr attr;
  पूर्णांक ret;
  माप_प्रकार cur_size;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  साठी (cur_size = 64; cur_size <= 4096; cur_size *= 2)
    {
      progs = xrealloc (progs, आकार (uint32_t) * cur_size);

      स_रखो (&attr, 0, आकार (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_size;
      attr.query.prog_ids = ptr_to_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, आकार (attr));
    }
  पर्यंत (ret < 0 && त्रुटि_सं == ENOSPC)
    ;

  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "bpf query");

  *progs_out = progs;
  progs = रिक्त;
  *n_progs_out = attr.query.prog_cnt;
  परत 0;
#अन्यथा
  (शून्य) dirfd;

  *progs_out = रिक्त;
  *n_progs_out = 0;

  परत crun_make_error (err, 0, "eBPF not supported");
#endif
}

स्थिर पूर्णांक
remove_all_progs (पूर्णांक dirfd, uint32_t *progs, माप_प्रकार n_progs, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  संघ bpf_attr attr;
  माप_प्रकार i;

  साठी (i = 0; i < n_progs; i++)
    {
      cleanup_close पूर्णांक fd = -1;
      पूर्णांक ret;

      स_रखो (&attr, 0, आकार (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, आकार (attr));
      जर (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          जर (त्रुटि_सं == ENOENT)
            सुरूठेवा;

          परत crun_make_error (err, त्रुटि_सं, "cannot open existing eBPF program");
        }

      स_रखो (&attr, 0, आकार (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, आकार (attr));
      जर (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          जर (त्रुटि_सं == ENOENT)
            सुरूठेवा;

          परत crun_make_error (err, त्रुटि_सं, "cannot detach eBPF program");
        }
    }
  परत 0;
#अन्यथा
  (शून्य) dirfd;
  (शून्य) progs;
  (शून्य) n_progs;
  परत crun_make_error (err, 0, "eBPF not supported");
#endif
}

स्थिर पूर्णांक
ebpf_attach_program (पूर्णांक fd, पूर्णांक dirfd, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (शून्य) fd;
  (शून्य) dirfd;
  (शून्य) read_all_progs;
  (शून्य) remove_all_progs;

  परत crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  स्थिरांक पूर्णांक MAX_ATTEMPTS = 20;
  पूर्णांक attempt;

  साठी (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = रिक्त;
      cleanup_close पूर्णांक replacefd = -1;
      संघ bpf_attr attr;
      माप_प्रकार n_progs = 0;
      पूर्णांक ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      जर (! skip_replace && n_progs == 1)
        {
          स_रखो (&attr, 0, आकार (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, आकार (attr));
          जर (UNLIKELY (replacefd < 0))
            {
              जर (त्रुटि_सं == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  सुरूठेवा;
                }
              परत crun_make_error (err, त्रुटि_सं, "cannot open existing eBPF program");
            }
        }
#  endif

      स_रखो (&attr, 0, आकार (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      जर (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, आकार (attr));
      जर (UNLIKELY (ret < 0))
        {
          जर (त्रुटि_सं == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              सुरूठेवा;
            }
#  ifdef BPF_F_REPLACE
          जर (त्रुटि_सं == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              सुरूठेवा;
            }
#  endif
          परत crun_make_error (err, त्रुटि_सं, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      जर (replacefd < 0 && n_progs)
        परत remove_all_progs (dirfd, progs, n_progs, err);

      परत 0;
    }
#endif
}

स्थिर शून्य
bump_memlock ()
{
  संरचना rlimit limit;
  पूर्णांक ret;

  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &limit);
  जर (ret == 0)
    परत;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &limit);
  जर (ret < 0)
    परत;

  limit.rlim_cur = limit.rlim_max;
  /* Best effort, ignore errors.  */
  (शून्य) setrlimit (RLIMIT_MEMLOCK, &limit);
}

पूर्णांक
libcrun_ebpf_load (संरचना bpf_program *program, पूर्णांक dirfd, स्थिरांक अक्षर *pin, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (शून्य) dirfd;
  (शून्य) program;
  (शून्य) pin;
  (शून्य) ebpf_attach_program;

  परत crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
  cleanup_close पूर्णांक fd = -1;
  संघ bpf_attr attr;
  पूर्णांक ret;

  स_रखो (&attr, 0, आकार (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_to_u64 (program->program);
  attr.insn_cnt = bpf_program_instructions (program);
  attr.license = ptr_to_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_LOAD, &attr, आकार (attr));
  जर (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_LOAD, &attr, आकार (attr));
    }
  जर (fd < 0)
    {
      स्थिरांक माप_प्रकार max_log_size = 1 << 20;
      cleanup_free अक्षर *log = रिक्त;
      माप_प्रकार log_size = 8192;

    retry:
      log = xrealloc (log, log_size);
      log[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_to_u64 (log);
      attr.log_size = log_size;

      fd = bpf (BPF_PROG_LOAD, &attr, आकार (attr));
      जर (fd < 0)
        {
          जर (त्रुटि_सं == ENOSPC && log_size < max_log_size)
            {
              /* The provided buffer was not big enough.  */
              log_size *= 2;
              जा retry;
            }
          परत crun_make_error (err, त्रुटि_सं, "bpf create `%s`", log);
        }
    }

  जर (dirfd >= 0)
    {
      ret = ebpf_attach_program (fd, dirfd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  /* Optionally pin the program to the specified path.  */
  जर (pin)
    {
      unlink (pin);

      स_रखो (&attr, 0, आकार (attr));
      attr.pathname = ptr_to_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, आकार (attr));
      जर (ret < 0)
        परत crun_make_error (err, त्रुटि_सं, "bpf pin to `%s`", pin);
    }

  परत 0;
#endif
}

पूर्णांक
libcrun_ebpf_read_program (संरचना bpf_program **program_ret, स्थिरांक अक्षर *path, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (शून्य) program_ret;
  (शून्य) path;

  परत crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
  cleanup_free संरचना bpf_program *program = रिक्त;
  cleanup_free अक्षर *buffer = रिक्त;
  cleanup_close पूर्णांक prog_fd = -1;
  माप_प्रकार buffer_size = 0;
  संरचना bpf_prog_info info;
  संघ bpf_attr attr;
  पूर्णांक ret;

  स_रखो (&attr, 0, आकार (attr));
  attr.pathname = ptr_to_u64 (path);

  prog_fd = bpf (BPF_OBJ_GET, &attr, आकार (attr));
  जर (UNLIKELY (prog_fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "bpf get `%s`", path);

  स_रखो (&info, 0, आकार (info));

  स_रखो (&attr, 0, आकार (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_to_u64 (&info);
  attr.info.info_len = आकार (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, आकार (attr));
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "bpf get info `%s`", path);

  buffer_size = info.xlated_prog_len;
  buffer = xmalloc (buffer_size);

  स_रखो (&info, 0, आकार (info));
  info.xlated_prog_insns = ptr_to_u64 (buffer);
  info.xlated_prog_len = buffer_size;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, आकार (attr));
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "bpf get info `%s`", path);

  program = bpf_program_new (buffer_size);
  program = bpf_program_append (program, buffer, buffer_size);

  *program_ret = program;
  program = रिक्त;

  परत 0;
#endif
}

पूर्णांक
libcrun_ebpf_query_cgroup_progs (स्थिरांक अक्षर *cgroup_path, uint32_t **progs_out, माप_प्रकार *n_progs_out, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (शून्य) cgroup_path;
  *progs_out = रिक्त;
  *n_progs_out = 0;
  परत crun_make_error (err, 0, "eBPF not supported");
#अन्यथा
  cleanup_close पूर्णांक cgroup_fd = -1;

  cgroup_fd = खोलो (cgroup_path, O_PATH | O_CLOEXEC);
  जर (UNLIKELY (cgroup_fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open cgroup path `%s`", cgroup_path);

  परत read_all_progs (cgroup_fd, progs_out, n_progs_out, err);
#endif
}

bool
libcrun_ebpf_cmp_programs (संरचना bpf_program *program1, संरचना bpf_program *program2)
{
  जर (program1->used != program2->used)
    परत false;

  परत स_भेद (program1->program, program2->program, program1->used) == 0;
}
