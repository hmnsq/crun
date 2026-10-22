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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "ebpf.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/syscall.h>
#അന്തര്ഭാവ <sys/resource.h>

#ifdef HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
സ്ഥിതം പൂർണ്ണസംഖ്യ
syscall_bpf (പൂർണ്ണസംഖ്യ cmd, സംഘം bpf_attr *attr, unsigned പൂർണ്ണസംഖ്യ size)
{
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_bpf, cmd, attr, size);
}
#    define bpf syscall_bpf
#  endif

#endif

സ്ഥിതം വരിയിൽ uint64_t
ptr_to_u64 (സ്ഥിരം ശൂന്യം *ptr)
{
  തിരികെ (uint64_t) (unsigned long) ptr;
}

എണ്ണൽ
{
  HAS_WILDCARD = 1
};

ഘടന bpf_program
{
  size_t allocated;
  size_t used;
  unsigned പൂർണ്ണസംഖ്യ സ്വകാര്യ;
  അക്ഷരം program[];
};

#ifdef HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((ഘടന bpf_insn) { .code = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(SIZE, DST, SRC, OFF) \
    ((ഘടന bpf_insn) {                   \
        .code = BPF_LDX | BPF_SIZE (SIZE) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((ഘടന bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((ഘടന bpf_insn) { .code = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((ഘടന bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((ഘടന bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((ഘടന bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((ഘടന bpf_insn) { .code = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((ഘടന bpf_insn) { .code = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#endif

#ifdef HAVE_EBPF
സ്ഥിതം size_t
bpf_program_instructions (ഘടന bpf_program *program)
{
  തിരികെ program->used / (വലുപ്പം (ഘടന bpf_insn));
}
#endif

ഘടന bpf_program *
bpf_program_new (size_t size)
{
  ഘടന bpf_program *p = xmalloc (size + വലുപ്പം (ഘടന bpf_program));

  p->used = 0;
  p->സ്വകാര്യ = 0;
  p->allocated = size;

  തിരികെ p;
}

ഘടന bpf_program *
bpf_program_append (ഘടന bpf_program *p, ശൂന്യം *data, size_t size)
{
  എങ്കിൽ (p->allocated <= p->used + size)
    {
      p->allocated += size * 2;
      p = xrealloc (p, p->allocated + വലുപ്പം (ഘടന bpf_program));
    }
  memcpy (p->program + p->used, data, size);
  p->used += size;
  തിരികെ p;
}

ഘടന bpf_program *
bpf_program_init_dev (ഘടന bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  /* taken from systemd.  */
  ഘടന bpf_insn pre_insn[] = {
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
  program = bpf_program_append (program, pre_insn, വലുപ്പം (pre_insn));
#endif
  തിരികെ program;
}

ഘടന bpf_program *
bpf_program_append_dev (ഘടന bpf_program *program, സ്ഥിരം അക്ഷരം *access, അക്ഷരം type, പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor, bool accept,
                        libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  പൂർണ്ണസംഖ്യ i;
  പൂർണ്ണസംഖ്യ bpf_access = 0;
  പൂർണ്ണസംഖ്യ bpf_type = type == 'b' ? BPF_DEVCG_DEV_BLOCK : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  പൂർണ്ണസംഖ്യ സംഖ്യ_instructions = 0;
  ഘടന bpf_insn accept_block[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  എങ്കിൽ (access == ശൂന്യ)
    access = "";

  എങ്കിൽ (program->സ്വകാര്യ & HAS_WILDCARD)
    തിരികെ program;

  വേണ്ടി (i = 0; access[i]; i++)
    {
      തിരഞ്ഞെടുപ്പ് (access[i])
        {
        അവസ്ഥ 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          നിർത്തുക;

        അവസ്ഥ 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          നിർത്തുക;

        അവസ്ഥ 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          നിർത്തുക;
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
  സംഖ്യ_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  എങ്കിൽ (has_type)
    {
      ഘടന bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, സംഖ്യ_instructions) };
      സംഖ്യ_instructions--;
      program = bpf_program_append (program, i, വലുപ്പം (i));
    }
  എങ്കിൽ (has_access)
    {
      ഘടന bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, സംഖ്യ_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, സംഖ്യ_instructions - 2),
      };
      സംഖ്യ_instructions -= 3;
      program = bpf_program_append (program, i, വലുപ്പം (i));
    }
  എങ്കിൽ (has_major)
    {
      ഘടന bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, സംഖ്യ_instructions) };
      സംഖ്യ_instructions--;
      program = bpf_program_append (program, i, വലുപ്പം (i));
    }
  എങ്കിൽ (has_minor)
    {
      ഘടന bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, സംഖ്യ_instructions) };
      സംഖ്യ_instructions--;
      program = bpf_program_append (program, i, വലുപ്പം (i));
    }

  എങ്കിൽ (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    program->സ്വകാര്യ |= HAS_WILDCARD;

  program = bpf_program_append (program, accept_block, വലുപ്പം (accept_block));
#endif
  (ശൂന്യം) access;
  (ശൂന്യം) type;
  (ശൂന്യം) major;
  (ശൂന്യം) minor;
  (ശൂന്യം) accept;

  തിരികെ program;
}

ഘടന bpf_program *
bpf_program_complete_dev (ഘടന bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  ഘടന bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  എങ്കിൽ (program->സ്വകാര്യ & HAS_WILDCARD)
    തിരികെ program;

  program = bpf_program_append (program, &i, വലുപ്പം (i));
#endif
  തിരികെ program;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_all_progs (പൂർണ്ണസംഖ്യ dirfd, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  cleanup_free uint32_t *progs = ശൂന്യ;
  സംഘം bpf_attr attr;
  പൂർണ്ണസംഖ്യ ret;
  size_t cur_size;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  വേണ്ടി (cur_size = 64; cur_size <= 4096; cur_size *= 2)
    {
      progs = xrealloc (progs, വലുപ്പം (uint32_t) * cur_size);

      memset (&attr, 0, വലുപ്പം (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_size;
      attr.query.prog_ids = ptr_to_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, വലുപ്പം (attr));
    }
  ആകുമ്പോൾ (ret < 0 && errno == ENOSPC)
    ;

  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "bpf query");

  *progs_out = progs;
  progs = ശൂന്യ;
  *n_progs_out = attr.query.prog_cnt;
  തിരികെ 0;
#അല്ലെങ്കിൽ
  (ശൂന്യം) dirfd;

  *progs_out = ശൂന്യ;
  *n_progs_out = 0;

  തിരികെ crun_make_error (err, 0, "eBPF not supported");
#endif
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
remove_all_progs (പൂർണ്ണസംഖ്യ dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  സംഘം bpf_attr attr;
  size_t i;

  വേണ്ടി (i = 0; i < n_progs; i++)
    {
      cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
      പൂർണ്ണസംഖ്യ ret;

      memset (&attr, 0, വലുപ്പം (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, വലുപ്പം (attr));
      എങ്കിൽ (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          എങ്കിൽ (errno == ENOENT)
            തുടരുക;

          തിരികെ crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, വലുപ്പം (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, വലുപ്പം (attr));
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          എങ്കിൽ (errno == ENOENT)
            തുടരുക;

          തിരികെ crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  തിരികെ 0;
#അല്ലെങ്കിൽ
  (ശൂന്യം) dirfd;
  (ശൂന്യം) progs;
  (ശൂന്യം) n_progs;
  തിരികെ crun_make_error (err, 0, "eBPF not supported");
#endif
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
ebpf_attach_program (പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ dirfd, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ശൂന്യം) fd;
  (ശൂന്യം) dirfd;
  (ശൂന്യം) read_all_progs;
  (ശൂന്യം) remove_all_progs;

  തിരികെ crun_make_error (err, 0, "eBPF not supported");
#അല്ലെങ്കിൽ
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  സ്ഥിരം പൂർണ്ണസംഖ്യ MAX_ATTEMPTS = 20;
  പൂർണ്ണസംഖ്യ attempt;

  വേണ്ടി (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = ശൂന്യ;
      cleanup_close പൂർണ്ണസംഖ്യ replacefd = -1;
      സംഘം bpf_attr attr;
      size_t n_progs = 0;
      പൂർണ്ണസംഖ്യ ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      എങ്കിൽ (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, വലുപ്പം (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, വലുപ്പം (attr));
          എങ്കിൽ (UNLIKELY (replacefd < 0))
            {
              എങ്കിൽ (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  തുടരുക;
                }
              തിരികെ crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, വലുപ്പം (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      എങ്കിൽ (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, വലുപ്പം (attr));
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              തുടരുക;
            }
#  ifdef BPF_F_REPLACE
          എങ്കിൽ (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              തുടരുക;
            }
#  endif
          തിരികെ crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      എങ്കിൽ (replacefd < 0 && n_progs)
        തിരികെ remove_all_progs (dirfd, progs, n_progs, err);

      തിരികെ 0;
    }
#endif
}

സ്ഥിതം ശൂന്യം
bump_memlock ()
{
  ഘടന rlimit limit;
  പൂർണ്ണസംഖ്യ ret;

  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &limit);
  എങ്കിൽ (ret == 0)
    തിരികെ;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &limit);
  എങ്കിൽ (ret < 0)
    തിരികെ;

  limit.rlim_cur = limit.rlim_max;
  /* Best effort, ignore errors.  */
  (ശൂന്യം) setrlimit (RLIMIT_MEMLOCK, &limit);
}

പൂർണ്ണസംഖ്യ
libcrun_ebpf_load (ഘടന bpf_program *program, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *pin, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ശൂന്യം) dirfd;
  (ശൂന്യം) program;
  (ശൂന്യം) pin;
  (ശൂന്യം) ebpf_attach_program;

  തിരികെ crun_make_error (err, 0, "eBPF not supported");
#അല്ലെങ്കിൽ
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  സംഘം bpf_attr attr;
  പൂർണ്ണസംഖ്യ ret;

  memset (&attr, 0, വലുപ്പം (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_to_u64 (program->program);
  attr.insn_cnt = bpf_program_instructions (program);
  attr.license = ptr_to_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_LOAD, &attr, വലുപ്പം (attr));
  എങ്കിൽ (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_LOAD, &attr, വലുപ്പം (attr));
    }
  എങ്കിൽ (fd < 0)
    {
      സ്ഥിരം size_t max_log_size = 1 << 20;
      cleanup_free അക്ഷരം *log = ശൂന്യ;
      size_t log_size = 8192;

    retry:
      log = xrealloc (log, log_size);
      log[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_to_u64 (log);
      attr.log_size = log_size;

      fd = bpf (BPF_PROG_LOAD, &attr, വലുപ്പം (attr));
      എങ്കിൽ (fd < 0)
        {
          എങ്കിൽ (errno == ENOSPC && log_size < max_log_size)
            {
              /* The provided buffer was not big enough.  */
              log_size *= 2;
              പോകുക retry;
            }
          തിരികെ crun_make_error (err, errno, "bpf create `%s`", log);
        }
    }

  എങ്കിൽ (dirfd >= 0)
    {
      ret = ebpf_attach_program (fd, dirfd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  /* Optionally pin the program to the specified path.  */
  എങ്കിൽ (pin)
    {
      unlink (pin);

      memset (&attr, 0, വലുപ്പം (attr));
      attr.pathname = ptr_to_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, വലുപ്പം (attr));
      എങ്കിൽ (ret < 0)
        തിരികെ crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  തിരികെ 0;
#endif
}

പൂർണ്ണസംഖ്യ
libcrun_ebpf_read_program (ഘടന bpf_program **program_ret, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ശൂന്യം) program_ret;
  (ശൂന്യം) path;

  തിരികെ crun_make_error (err, 0, "eBPF not supported");
#അല്ലെങ്കിൽ
  cleanup_free ഘടന bpf_program *program = ശൂന്യ;
  cleanup_free അക്ഷരം *buffer = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ prog_fd = -1;
  size_t buffer_size = 0;
  ഘടന bpf_prog_info info;
  സംഘം bpf_attr attr;
  പൂർണ്ണസംഖ്യ ret;

  memset (&attr, 0, വലുപ്പം (attr));
  attr.pathname = ptr_to_u64 (path);

  prog_fd = bpf (BPF_OBJ_GET, &attr, വലുപ്പം (attr));
  എങ്കിൽ (UNLIKELY (prog_fd < 0))
    തിരികെ crun_make_error (err, errno, "bpf get `%s`", path);

  memset (&info, 0, വലുപ്പം (info));

  memset (&attr, 0, വലുപ്പം (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_to_u64 (&info);
  attr.info.info_len = വലുപ്പം (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, വലുപ്പം (attr));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "bpf get info `%s`", path);

  buffer_size = info.xlated_prog_len;
  buffer = xmalloc (buffer_size);

  memset (&info, 0, വലുപ്പം (info));
  info.xlated_prog_insns = ptr_to_u64 (buffer);
  info.xlated_prog_len = buffer_size;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, വലുപ്പം (attr));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "bpf get info `%s`", path);

  program = bpf_program_new (buffer_size);
  program = bpf_program_append (program, buffer, buffer_size);

  *program_ret = program;
  program = ശൂന്യ;

  തിരികെ 0;
#endif
}

പൂർണ്ണസംഖ്യ
libcrun_ebpf_query_cgroup_progs (സ്ഥിരം അക്ഷരം *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (ശൂന്യം) cgroup_path;
  *progs_out = ശൂന്യ;
  *n_progs_out = 0;
  തിരികെ crun_make_error (err, 0, "eBPF not supported");
#അല്ലെങ്കിൽ
  cleanup_close പൂർണ്ണസംഖ്യ cgroup_fd = -1;

  cgroup_fd = open (cgroup_path, O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (cgroup_fd < 0))
    തിരികെ crun_make_error (err, errno, "open cgroup path `%s`", cgroup_path);

  തിരികെ read_all_progs (cgroup_fd, progs_out, n_progs_out, err);
#endif
}

bool
libcrun_ebpf_cmp_programs (ഘടന bpf_program *program1, ഘടന bpf_program *program2)
{
  എങ്കിൽ (program1->used != program2->used)
    തിരികെ false;

  തിരികെ memcmp (program1->program, program2->program, program1->used) == 0;
}
